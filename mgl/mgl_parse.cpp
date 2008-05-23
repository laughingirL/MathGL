/* mgl_exec.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <mathgl.abalakin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <ctype.h>
#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#include <unistd.h>
#else
#include <direct.h>
#endif
#ifdef WIN32
wchar_t *wcstokw32(wchar_t *wcs, const wchar_t *delim){
    wcstok(wcs,delim);
}
#define swprintf    _snwprintf
#define wcstok(a,b,c) wcstokw32(a,b)
#endif
//#include <unistd.h>
#include "mgl/mgl_parse.h"
#include "mgl/mgl_c.h"
#include "mgl/mgl_f.h"
wchar_t *mgl_wcsdup(const wchar_t *s);
//-----------------------------------------------------------------------------
wchar_t *mgl_str_copy(const char *s)
{
	wchar_t *str = new wchar_t[strlen(s)+1];
	register long i;
	for(i=0;i<strlen(s);i++)	str[i] = s[i];
	str[i] = 0;
	return str;
}
//-----------------------------------------------------------------------------
int mgl_cmd_cmp(const void *a, const void *b)
{
	const mglCommand *aa = (const mglCommand *)a;
	const mglCommand *bb = (const mglCommand *)b;
	return wcscmp(aa->name, bb->name);
}
//-----------------------------------------------------------------------------
// return values : 0 -- OK, 1 -- wrong arguments, 2 -- wrong command
int mglParse::Exec(mglGraph *gr, const wchar_t *com, long n, mglArg *a)
{
	wchar_t ch = com[0];
	int k[10], i;
	for(i=0;i<10;i++)	k[i] = i<n ? a[i].type + 1 : 0;
	for(i=0;i<n;i++)	wcstombs(a[i].s, a[i].w, 1024);
	mglCommand tst, *rts;
	for(i=0;Cmd[i].name[0];i++);	// determine the number of symbols
	tst.name = com;
	rts = (mglCommand *) bsearch(&tst, Cmd, i, sizeof(mglCommand), mgl_cmd_cmp);
	if(!rts)	return 2;
	if(out)	rts->save(out, n, a, k);
	return rts->exec(gr, n, a, k);
}
//-----------------------------------------------------------------------------
void mglVar::MoveAfter(mglVar *var)
{
	if(prev)	prev->next = next;
	if(next)	next->prev = prev;
	prev = next = 0;
	if(var)
	{
		prev = var;
		next = var->next;
		var->next = this;
		if(func==0)	func = var->func;
	}
	if(next)	next->prev = this;
}
//-----------------------------------------------------------------------------
mglVar::~mglVar()
{
	if(func)	func(o);
	if(prev)	prev->next = next;
	if(next)	next->prev = prev;
}
//-----------------------------------------------------------------------------
mglParse::mglParse(bool setsize)
{
	DataList=0;		Cmd = mgls_base_cmd;
	AllowSetSize=setsize;
	Once = true;	Skip = false;	out = 0;
	parlen=320;		Stop=false;		if_pos = 0;
	for(long i=0;i<10;i++)	{	par[i] = 0;	for_stack[i]=0;	opt[i]=false;	}
	opt[10] = false;
	op1 = new wchar_t[2048];	op2 = new wchar_t[2048];
}
//-----------------------------------------------------------------------------
mglParse::~mglParse()
{
	if(DataList)
	{
		while(DataList->next)	delete DataList->next;
		delete DataList;
	}
	for(long i=0;i<10;i++)	if(par[i])	delete [] par[i];
	delete []op1;	delete []op2;
}
//-----------------------------------------------------------------------------
bool mglParse::AddParam(int n, const char *str, bool isstr)
{
	unsigned s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	AddParam(n,wcs,isstr);
	delete []wcs;
}
//-----------------------------------------------------------------------------
int mglParse::Parse(mglGraph *gr, const char *str, long pos)
{
	unsigned s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	Parse(gr,wcs,pos);
	delete []wcs;
}
//-----------------------------------------------------------------------------
mglVar *mglParse::AddVar(const char *str)
{
	unsigned s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	mglVar *v = AddVar(wcs);
	delete []wcs;
	return v;
}
//-----------------------------------------------------------------------------
mglVar *mglParse::FindVar(const char *str)
{
	unsigned s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	mglVar *v = FindVar(wcs);
	delete []wcs;
	return v;
}
//-----------------------------------------------------------------------------
bool mglParse::AddParam(int n, const wchar_t *str, bool isstr)
{
	if(n<0 || n>9 || wcschr(str,'$'))	return false;
	if(!isstr)	parlen += wcslen(str);
	if(par[n])	delete []par[n];
	par[n] = new wchar_t[wcslen(str)+1];
	wcscpy(par[n],str);
	return true;
}
//-----------------------------------------------------------------------------
mglVar *mglParse::FindVar(const wchar_t *name)
{
	mglVar *v=DataList;
	wchar_t *s = mgl_wcsdup(name),*p;
	p = wcschr(s,'.');	if(p)	{	p[0]=0;	p++;	}
	while(v)
	{
		if(!wcscmp(s, v->s))	{	free(s);	return v;	}
		v = v->next;
	}
	free(s);
	return 0;
}
//-----------------------------------------------------------------------------
mglVar *mglParse::AddVar(const wchar_t *name)
{
	mglVar *v = FindVar(name);
	if(v)	return v;
	v = new mglVar;
	wcsncpy(v->s,name,256);
	if(DataList)	v->MoveAfter(DataList);
	else			DataList = v;
	return v;
}
//-----------------------------------------------------------------------------
int mglFindArg(const wchar_t *str)
{
	register long l=0,i;//,j,len=strlen(lst);
	for(i=0;i<long(wcslen(str));i++)
	{
		if(l%2==0 && (str[i]=='#' || str[i]==';'))	return -i;
		if(str[i]=='\'') l++;
		if(l%2==0 && (str[i]==' ' || str[i]=='\t' || str[i]=='\n'))	return i;
	}
	return 0;
}
//-----------------------------------------------------------------------------
void wcstrim_mgl(wchar_t *str);
void wcslwr_mgl(wchar_t *str);
//-----------------------------------------------------------------------------
bool mgls_suffix(const wchar_t *p, mglData *d, float *v)
{
	float x,y,z,k;
	bool ok=false;
	if(!wcscmp(p,L"a"))			{	ok = true;	*v = d->a[0];	}
	else if(!wcscmp(p,L"fst"))	{	ok = true;	int i=-1,j=-1,k=-1;	*v = d->Find(0,i,j,k);	}
	else if(!wcscmp(p,L"lst"))	{	ok = true;	int i=-1,j=-1,k=-1;	*v = d->Last(0,i,j,k);	}
	else if(!wcscmp(p,L"nx"))	{	ok = true;	*v=d->nx;	}
	else if(!wcscmp(p,L"ny"))	{	ok = true;	*v=d->ny;	}
	else if(!wcscmp(p,L"nz"))	{	ok = true;	*v=d->nz;	}
	else if(!wcscmp(p,L"max"))	{	ok = true;	*v=d->Maximal();	}
	else if(!wcscmp(p,L"min"))	{	ok = true;	*v=d->Minimal();	}
	else if(!wcscmp(p,L"sum"))	{	ok = true;	*v=d->Momentum('x',x,y);	}
	else if(!wcscmp(p,L"mx"))	{	ok = true;	d->Maximal(x,y,z);		*v=x/d->nx;	}
	else if(!wcscmp(p,L"my"))	{	ok = true;	d->Maximal(x,y,z);		*v=y/d->ny;	}
	else if(!wcscmp(p,L"mz"))	{	ok = true;	d->Maximal(x,y,z);		*v=z/d->nz;	}
	else if(!wcscmp(p,L"ax"))	{	ok = true;	d->Momentum('x',x,y);	*v=x/d->nx;	}
	else if(!wcscmp(p,L"ay"))	{	ok = true;	d->Momentum('y',x,y);	*v=x/d->ny;	}
	else if(!wcscmp(p,L"az"))	{	ok = true;	d->Momentum('z',x,y);	*v=x/d->nz;	}
	else if(!wcscmp(p,L"wx"))	{	ok = true;	d->Momentum('x',x,y);	*v=y/d->nx;	}
	else if(!wcscmp(p,L"wy"))	{	ok = true;	d->Momentum('y',x,y);	*v=y/d->ny;	}
	else if(!wcscmp(p,L"wz"))	{	ok = true;	d->Momentum('z',x,y);	*v=y/d->nz;	}
	else if(!wcscmp(p,L"sx"))	{	ok = true;	d->Momentum('x',x,y,z,k);	*v=z/d->nx;	}
	else if(!wcscmp(p,L"sy"))	{	ok = true;	d->Momentum('y',x,y,z,k);	*v=z/d->ny;	}
	else if(!wcscmp(p,L"sz"))	{	ok = true;	d->Momentum('z',x,y,z,k);	*v=z/d->nz;	}
	else if(!wcscmp(p,L"kx"))	{	ok = true;	d->Momentum('x',x,y,z,k);	*v=k/d->nx;	}
	else if(!wcscmp(p,L"ky"))	{	ok = true;	d->Momentum('y',x,y,z,k);	*v=k/d->ny;	}
	else if(!wcscmp(p,L"kz"))	{	ok = true;	d->Momentum('z',x,y,z,k);	*v=k/d->nz;	}
	else if(!wcscmp(p,L"aa"))	{	ok = true;	d->Momentum('a',x,y);	*v=x;	}
	else if(!wcscmp(p,L"wa"))	{	ok = true;	d->Momentum('a',x,y);	*v=y;	}
	else if(!wcscmp(p,L"sa"))	{	ok = true;	d->Momentum('a',x,y,z,k);*v=z;	}
	else if(!wcscmp(p,L"ka"))	{	ok = true;	d->Momentum('a',x,y,z,k);*v=k;	}
	return ok;
}
//-----------------------------------------------------------------------------
// convert substrings to arguments
void mglParse::FillArg(int k, wchar_t **arg, mglArg *a)
{
	wchar_t *s, *t, *p=0, *b;
	for(long n=1;n<k;n++)
	{
		mglVar *v, *u;
		a[n-1].type = -1;
		p = wcschr(arg[n],'.');
		if(arg[n][0]!='\'' && (s=wcschr(arg[n],'(')))	// subdata or column in argument
		{
			p = wcschr(arg[n],'.');	if(p)	{	p[0]=0;	p++;	}
			s = mgl_wcsdup(arg[n]);
			t = wcstok (s, L"(,)",&b);
			v = FindVar(t);
			if(v==0)
			{	a[n-1].type = 2;	a[n-1].v = NAN;		free(s);	return;	}
			if(!wcschr(arg[n],'\''))	// this subdata
			{
				long k[3]={-1,-1,-1};
				t = wcstok (0, L"(,)",&b);	if(t)	k[0] = t[0]==':'?-1:wcstol(t,0,0);
				t = wcstok (0, L"(,)",&b);	if(t)	k[1] = t[0]==':'?-1:wcstol(t,0,0);
				t = wcstok (0, L"(,)",&b);	if(t)	k[2] = t[0]==':'?-1:wcstol(t,0,0);
				u = new mglVar;		u->temp = true;
				swprintf(a[n-1].w,2048,L"%ls.SubData(%d,%d,%d)",v->s,k[0],k[1],k[2]);
				u->d = v->d.SubData(k[0],k[1],k[2]);
			}
			else	// this is column formula
			{
				wchar_t *tt = wcstok(0,L"'",&b);
				t = wcstok(0,L"'",&b);	t[-1]=0;
				u = new mglVar;		u->temp = true;
				char *cc = new char[wcslen(tt)+1];
				wcstombs(cc,tt,wcslen(tt)+1);
				swprintf(a[n-1].w,2048,L"%ls.Column(\"%s\")",v->s,cc);
				u->d = v->d.Column(cc);
				delete []cc;
			}
			free(s);
			if(DataList)	u->MoveAfter(DataList);
			else			DataList = u;
			a[n-1].type = 0;	a[n-1].d = &(u->d);
		}
		else if(arg[n][0]=='|')	a[n-1].type = -1;
		else if(arg[n][0]=='\'')
		{
			a[n-1].type = 1;	arg[n][wcslen(arg[n])-1] = 0;
			if(wcslen(arg[n]+1)>=2048)	arg[n][2048]=0;
			wcscpy(a[n-1].w, arg[n]+1);
		}
		else if((v = FindVar(arg[n]))!=0)
		{
			p = wcschr(arg[n],'.');	if(p)	{	p[0]=0;	p++;	}
			a[n-1].type = 0;		a[n-1].d = &(v->d);
			wcscpy(a[n-1].w, v->s);
		}
		else
		{
			a[n-1].type = 2;
			if(!wcscmp(arg[n],L"nan"))	a[n-1].v = NAN;
			else if(!wcscmp(arg[n],L":"))	a[n-1].v = -1;
			else if(!wcscmp(arg[n],L"pi"))	a[n-1].v = M_PI;
			else if(!wcscmp(arg[n],L"-pi"))	a[n-1].v = -M_PI;
			else if(!wcscmp(arg[n],L"on"))	a[n-1].v = 1;
			else if(!wcscmp(arg[n],L"off"))	a[n-1].v = 0;
			else	a[n-1].v = wcstod(arg[n],0);
		}
		// try to find numbers (momentum) for data
		if(a[n-1].type == 0 && p)
			if(mgls_suffix(p,a[n-1].d,&(a[n-1].v)))
				a[n-1].type = 2;
	}
}
//-----------------------------------------------------------------------------
bool check_for_name(const wchar_t *s)
{
	return !isalpha(s[0])||wcschr(s,'.')||wcschr(s,':')||wcschr(s,'(')||wcschr(s,')');
}
//-----------------------------------------------------------------------------
// return values: 0 - not found, 1 - OK, 2 - wrong arguments,
//				3 - wrong command, 4 - string too long
int mglParse::PreExec(mglGraph *gr, long k, wchar_t **arg, mglArg *a)
{
	long n=0;
	mglVar *v;
	if(!wcsncmp(L"read",arg[0],4) || !wcscmp(L"new",arg[0]) || !wcscmp(L"copy",arg[0]) || !wcscmp(L"var",arg[0]) || !wcscmp(L"hist",arg[0]) || !wcscmp(L"max",arg[0]) || !wcscmp(L"min",arg[0]) || !wcscmp(L"sum",arg[0]) || !wcscmp(L"resize",arg[0]) || !wcscmp(L"subdata",arg[0]) || !wcscmp(L"momentum",arg[0]) || !wcscmp(L"fit",arg[0]))
	{
		if(k<3 || check_for_name(arg[1]))	return 2;
		v = AddVar(arg[1]);
		v->d.Create(1,1,1);
		a[0].type = 0;	a[0].d = &(v->d);
		wcscpy(a[0].w, arg[1]);
		n = 1+(Exec(gr, arg[0],k-1,a));
	}
	else if(!wcscmp(L"delete",arg[0]))	// parse command "delete"
	{
		if(k<2)	return 2;
		v = FindVar(arg[1]);
		if(v)
		{
			if(DataList==v)	DataList = v->next;
			delete v;
		}
		n=1;
	}
	else if(!wcscmp(L"list",arg[0]))	// parse command "list"
	{
		if(k<3 || check_for_name(arg[1]))	return 2;
		long nx=0, ny=1,j=0,i,t=0;
		char ch;
		for(i=2;i<k;i++)
		{
			ch = arg[i][0];
			if(a[i-1].type==1)	return 2;
			if(a[i-1].type==0)
			{
				if(t==1)	return 2;
				t=2;	nx++;
			}
			if(a[i-1].type==2)
			{
				if(t==2)	return 2;
				j++;	t=1;
			}
			if(ch=='|' && t==1)		{	nx = j>nx ? j:nx;	j=0;	ny++;	}
		}
		v = AddVar(arg[1]);		n=1;
		if(t==1)	nx = j>nx ? j:nx;
		if(t==1)	// list of numeric values
		{
			v->d.Create(nx,ny);
			j=t=0;
			for(i=2;i<k;i++)
			{
				if(arg[i][0]=='|')	{	t++;	j=0;	}
				else
				{	v->d.a[j+nx*t] = a[i-1].v;	j++;	}
			}
		}
		if(t==2)	// list of data
		{
			mglData *b = a[1].d;
			long nn = 0;
			if(b->nz>1)	return 2;
			if(b->ny>1)
			{
				v->d.Create(b->nx, b->ny, nx);
				nn = b->nx*b->ny;
				for(i=2,j=0;i<k;i++)
				{
					b = a[i-1].d;
					if(b==0 || nn!=b->nx*b->ny)	continue;
					memcpy(v->d.a+j*nn,b->a,nn*(b->nz)*sizeof(float));
					j+=b->nz;
				}
			}
			else
			{
				v->d.Create(b->nx, nx);
				nn = b->nx;
				for(i=2,j=0;i<k;i++)
				{
					b = a[i-1].d;
					if(b==0 || nn!=b->nx)	continue;
					memcpy(v->d.a+j*nn,b->a,nn*(b->ny)*sizeof(float));
					j+=b->ny;
				}
			}
		}
		n=1;
	}
	return n;
}
//-----------------------------------------------------------------------------
// return values: 0 - OK, 1 - wrong arguments, 2 - wrong command, 3 - string too long
int mglParse::Parse(mglGraph *gr, const wchar_t *string, long pos)
{
	if(!gr || Stop)	return 0;
	wchar_t *str, *s = new wchar_t[wcslen(string)+1+parlen],*arg[1024],*t;
	str = s;
	wcscpy(str,string);
	wcstrim_mgl(str);
	// define parameters or start cycle
	if(!wcsncmp(str,L"define",6) && (str[6]==' ' || str[6]=='\t'))
	{
		str += 7;	wcstrim_mgl(str);	int res = 1;
		if(*str=='$' && str[1]>='0' && str[1]<='9')
		{	res = 0;	AddParam(str[1]-'0', str+2);	}
		delete []s;		return res;
	}
	if(!wcsncmp(str,L"for",3) && (str[3]==' ' || str[3]=='\t'))
	{
		t = str+4;
		while(*t && isspace(*t))	t++;
		// if command have format 'for $N ...' then change it to 'for N ...'
		if(*t=='$' && t[1]>='0' && t[1]<='9')	*t = ' ';
	}	
	// parse arguments (parameters $1, ..., $9)
	if(parlen>0)
	{
		wchar_t *sb = new wchar_t[wcslen(string)+1];
		while((t=wcschr(str,'$'))!=0)
		{
			wcscpy(sb,t+2);
			t[0]=0;
			long n = t[1]-'0';
			if(n>=0 && n<=9 && par[n])	wcscat(str,par[n]);
			wcscat(str,sb);
		}
		delete []sb;
	}

	wcstrim_mgl(str);
	if(!wcscmp(str,L"stop"))	{	Stop = true;	delete []s;	return 0;	}
	long k=0,n;
	while(k<1024)	// parse string to substrings (by spaces)
	{
		n = mglFindArg(str);
		if(n<1)
		{
			if(str[-n]==';')	ProcOpt(gr,str-n);
			if(n<0)	str[-n]=0;
			break;
		}
		str[n]=0;	arg[k] = str;	k++;
		str = str+n+1;	wcstrim_mgl(str);
	}
	// try to find last argument
	if(str[0]!=0 && str[0]!='#' && str[0]!=';')	{	arg[k] = str;	k++;	}
	if(k<1) n =0;
	else
	{
		// fill arguments by its values
		mglArg *a = new mglArg[k];
		FillArg(k, arg, a);
		// execute first special (program-flow-control) commands
		n = FlowExec(gr, arg[0],k-1,a);
		if(n)	{	delete []s;	delete []a;	return n-1;	}
		if(Skip || (if_pos>0 && !(if_stack[if_pos-1]&1)))	{	delete []s;	delete []a;	return 0;	}
		if(!wcscmp(arg[0],L"call"))
		{
			n = 1;
			if(a[0].type==1)
			{	n=0;	FILE *fp = fopen(a[0].s,"rt");	Execute(gr,fp);	fclose(fp);	}
			delete []s;	delete []a;	return n;
		}
		if(!wcscmp(arg[0],L"for"))
		{
			n = 1;
			int r = int(a[0].v);
			if(a[0].type==2 && (r>=0 || r<=9))
			{
				if(a[1].type==0)
				{
					n=0;		fval[r] = *(a[1].d);
					fval[r].nx *= fval[r].ny*fval[r].nz;
				}
				else if(a[1].type==2 && a[2].type==2 && a[2].v>a[1].v)
				{
					float step = a[3].type==2?a[3].v:1;
					int m = int(step>0 ? (a[2].v-a[1].v)/step : 0);
					if(m>0)
					{
						n=0;	fval[r].Create(m+1);
						fval[r].Fill(a[1].v, a[2].v);
					}
				}
				if(n==0)
				{
					for(int i=9;i>0;i--)	for_stack[i] = for_stack[i-1];
					for_stack[0] = r+1;		fval[r].nz = pos;
					wchar_t buf[32];		swprintf(buf,32,L"%g",fval[r].a[0]);
					AddParam(r, buf,true);	fval[r].ny = 1;
				}
			}
			delete []s;	delete []a;	return n;
		}
		// alocate new arrays and execute the command itself
		n = PreExec(gr, k, arg, a);
		if(n>0)	n--;
		else if(!wcscmp(L"setsize",arg[0]) && !AllowSetSize)	n = 2;
		else	n = Exec(gr, arg[0],k-1,a);
		delete []a;
	}
	mglVar *v = DataList, *u;
	while(v)	// remove temporary data arrays
	{
		u = v->next;
		if(v->temp)	{	if(DataList==v)	DataList = v->next;		delete v;	}
		v = u;
	}
	// restore plot settings if it was changed
	ProcOpt(gr,0);
	delete []s;
	return n;
}
//-----------------------------------------------------------------------------
int mglParse::FlowExec(mglGraph *gr, const wchar_t *com, long m, mglArg *a)
{
	int n=-1;
	if(!wcscmp(com,L"once"))
	{
		if(a[0].type==2)
		{
			n = 0;
			if(a[0].v)
			{	Skip = !Once;	if(out)	swprintf(out,1024,L"if(!once)\t{\tonce = true;");	}
			else
			{	Skip = Once = false;	if(out)	swprintf(out,1024,L"}");	}
		}
		else n = 1;
	}
	else if(!wcscmp(com,L"if"))
	{
		int cond;
		if(a[0].type==2)
		{
			n = 0;	cond = (a[0].v!=0)?3:0;
			if(out)	swprintf(out,1024,L"if(%g!=0)\t{", a[0].v);
		}
		else if(m>1 && a[0].type==0 && a[1].type==1)
		{
			n = 0;	cond = a[0].d->FindAny(a[1].s)?3:0;
			if(out)	swprintf(out,1024,L"if(%s.FindAny(\"%s\"))\t{", a[0].s, a[1].s);
		}
		else n = 1;
		if(n==0)
		{	if_stack[if_pos] = cond;	if_pos = if_pos<19 ? if_pos+1 : 19;	}
	}
	else if(!wcscmp(com,L"endif"))
	{
		if_pos = if_pos>0 ? if_pos-1 : 0;
		n = 0;	if(out)	swprintf(out,1024,L"}");
	}
	else if(!wcscmp(com,L"else"))
	{
		if(if_pos>0)
		{
			n=0; if_stack[if_pos-1] = (if_stack[if_pos-1]&2)?2:3;
			if(out)	swprintf(out,1024,L"}\telse\t{");
		}
		else n = 1;
	}
	else if(!wcscmp(com,L"elseif"))
	{
		int cond;
		if(if_pos<1 || m<1)	n = 1;
		else if(if_stack[if_pos-1]&2)	{	n = 0;	cond = 2;	}
		else if(a[0].type==2)	{	n = 0;	cond = (a[0].v!=0)?3:0;	}
		else if(m>1 && a[0].type==0 && a[1].type==1)
		{	n = 0;	cond = a[0].d->FindAny(a[1].s)?3:0;	}
		else n = 1;
		if(n==0)
		{
			if_stack[if_pos-1] = cond;
			if(out && a[0].type==2)
				swprintf(out,1024,L"else if(%g!=0)\t{", a[0].v);
			if(out && m>1 && a[0].type==0 && a[1].type==1)
				swprintf(out,1024,L"else if(%s.FindAny(\"%s\"))\t{", a[0].s, a[1].s);
		}
	}
	else if(!wcscmp(com,L"next"))
	{
		int r = for_stack[0]-1;
		n = for_stack[0] ? 0:1;
		if(for_stack[0])
		{
			if(fval[r].ny<fval[r].nx)
			{
				wchar_t buf[32];		swprintf(buf,32,L"%g",fval[r].a[fval[r].ny]);
				AddParam(r, buf,true);	fval[r].ny += 1;
				n = -fval[r].nz-1;
			}
			else
			{
				for(int i=0;i<9;i++)	for_stack[i] = for_stack[i+1];
				for_stack[9] = 0;
			}
		}
	}
	return n+1;
}
//-----------------------------------------------------------------------------
void mglParse::Execute(mglGraph *gr, FILE *fp, bool print)
{
	if(gr==0 || fp==0)	return;
	wchar_t str[8192];
	int r, line=0;
	Stop = false;
	while(!feof(fp))
	{
		if(!fgetws(str,8192,fp))	break;
		line++;
		if(gr->Message) gr->Message[0] = 0;
		r = Parse(gr,str,ftell(fp));
		if(r<0)	{	fseek(fp,-r-1,SEEK_SET);	continue;	}

		if(print)
		{
			if(gr->Message && gr->Message[0])	printf("%s\n",gr->Message);
			if(r==1)	printf("Wrong argument(s) in line %d -- %s\n", line, str);
			if(r==2)	printf("Wrong command in line %d -- %s\n", line, str);
			if(r==3)	printf("String too long line %d -- %s\n", line, str);
		}
	}
}
//-----------------------------------------------------------------------------
void mglParse::Execute(mglGraph *gr, int n, const wchar_t **text, void (*error)(int line, int kind))
{
	if(gr==0 || n<1 || text==0)	return;
	long i, r;
	for(i=0;i<n;i++)
	{
		r = Parse(gr,text[i],i+1);
		if(r<0)	{	i = 1-r;	continue;	}
		if(error)
		{
			if(r>0)	error(i, r);
			if(gr->Message && gr->Message[0])	error(i,0);
		}
	}
}
//-----------------------------------------------------------------------------
int mglParse::Export(wchar_t cpp_out[1024], mglGraph *gr, const wchar_t *str)
{
	*op1 = *op2 = 0;
	out = cpp_out;
	int res = Parse(gr, str);
	out = 0;
	return res;
}
//-----------------------------------------------------------------------------
void mglParse::ProcOpt(mglGraph *gr, wchar_t *str)
{
	wchar_t buf[256]=L"";
	if(str==0)
	{
		if(opt[0])
		{
			gr->Min.x = val[0];	gr->Max.x = val[1];	gr->RecalcBorder();
			if(out)	swprintf(buf,256,L"\tgr->Min.x = tx1;\tgr->Max.x = tx2;}");
			wcscat(op2,buf);
		}
		if(opt[1])
		{
			gr->Min.y = val[2];	gr->Max.y = val[3];	gr->RecalcBorder();
			if(out)	swprintf(buf,256,L"\tgr->Min.y = ty1;\tgr->Max.y = ty2;}");
			wcscat(op2,buf);
		}
		if(opt[2])
		{
			gr->Min.z = val[4];	gr->Max.z = val[5];	gr->RecalcBorder();
			if(out)	swprintf(buf,256,L"\tgr->Min.z = tz1;\tgr->Max.z = tz2;}");
			wcscat(op2,buf);
		}
		if(opt[3])
		{
			gr->Cmin = val[6];	gr->Cmax = val[7];
			if(out)	swprintf(buf,256,L"\tgr->Cmin = tc1;\tgr->Cmax = tc2;}");
			wcscat(op2,buf);
		}
		if(opt[4])
		{
			gr->Ambient(val[8]);
			if(out)	swprintf(buf,256,L"\tgr->Ambient(tam);}");
			wcscat(op2,buf);
		}
		if(opt[5])
		{
			gr->Cut = val[10]!=0;
			if(out)	swprintf(buf,256,L"\tgr->Cut = tct;}");
			wcscat(op2,buf);
		}
		if(opt[6])
		{
			gr->AlphaDef = val[12];
			if(out)	swprintf(buf,256,L"\tgr->AlphaDef = tad;}");
			wcscat(op2,buf);
		}
		if(opt[7])
		{
			gr->MeshNum  = int(val[14]);
			if(out)	swprintf(buf,256,L"\tgr->MeshNum = tmn;}");
			wcscat(op2,buf);
		}
		if(opt[8])
		{
			gr->FontSize = val[16];
			if(out)	swprintf(buf,256,L"\tgr->FontSize = tfs;}");
			wcscat(op2,buf);
		}
		if(opt[9])
		{
			gr->MarkSize = val[17];
			if(out)	swprintf(buf,256,L"\tgr->MarkSize = tms;}");
			wcscat(op2,buf);
		}
		if(opt[10])
		{
			gr->AddLegend(leg, gr->last_style);
			if(out)	swprintf(buf,256,L"\tgr->AddLegend(\"%s\", \"%s\");", leg, gr->last_style);
			wcscat(op2,buf);
		}
		for(long i=0;i<16;i++)	opt[i]=false;
	}
	else
	{
		val[0] = gr->Min.x;		val[1] = gr->Max.x;
		val[2] = gr->Min.y;		val[3] = gr->Max.y;
		val[4] = gr->Min.z;		val[5] = gr->Max.z;
		val[6] = gr->Cmin;		val[7] = gr->Cmax;
		val[8] = gr->AmbBr;		val[10]= gr->Cut;
		val[12]= gr->AlphaDef;	val[14]= gr->MeshNum;
		val[16]= gr->FontSize;	val[17]= gr->MarkSize;
		wchar_t *s = str,*a,*b;
		long n;
		while((s=wcschr(s,';'))!=0)
		{
			s++;	wcstrim_mgl(s);
			n=mglFindArg(s);
			if(n<1)	return;
			s[n]=0;		a=s;	s=s+n+1;	wcstrim_mgl(a);
			float ff = wcstod(s,0);
			if(!wcscmp(a+1,L"range"))
			{
				n=mglFindArg(s);	if(n<1)	return;
				s[n]=0;		b=s;	s=s+n+1;	wcstrim_mgl(b);
				if(a[0]=='x')
				{
					opt[0]=true;	gr->Min.x = wcstod(b,0);	gr->Max.x = wcstod(s,0);
					if(out)	swprintf(buf,256,L"{float tx1=gr->Min.x, tx2=gr->Max.x;\tgr->Min.x=%g;\tgr->Max.x=%g;", wcstod(b,0), wcstod(s,0));
					wcscat(op1,buf);
				}
				else if(a[0]=='y')
				{
					opt[1]=true;	gr->Min.y = wcstod(b,0);	gr->Max.y = wcstod(s,0);
					if(out)	swprintf(buf,256,L"{float ty1=gr->Min.y, ty2=gr->Max.y;\tgr->Min.y=%g;\tgr->Max.y=%g;", wcstod(b,0), wcstod(s,0));
					wcscat(op1,buf);
				}
				else if(a[0]=='z')
				{
					opt[2]=true;	gr->Min.z = wcstod(b,0);	gr->Max.z = wcstod(s,0);
					if(out)	swprintf(buf,256,L"{float tz1=gr->Min.z, tz2=gr->Max.z;\tgr->Min.z=%g;\tgr->Max.z=%g;", wcstod(b,0), wcstod(s,0));
					wcscat(op1,buf);
				}
				else if(a[0]=='c')
				{
					opt[3]=true;	gr->Cmin = wcstod(b,0);		gr->Cmax = wcstod(s,0);
					if(out)	swprintf(buf,256,L"{float tc1=gr->Cmin, tc2=gr->Cmax;\tgr->Cmin=%g;\tgr->Cmax=%g;", wcstod(b,0), wcstod(s,0));
					wcscat(op1,buf);
				}
			}
			else if(!wcscmp(a,L"cut"))
			{
				opt[5]=true;	gr->Cut = (ff!=0 || !wcsncmp(s,L"on",2));
				if(out)	swprintf(buf,256,L"{bool tct=gr->Cut;\tgr->Cut=%s;", gr->Cut?"true":"false");
				wcscat(op1,buf);
			}
			else if(!wcscmp(a,L"meshnum"))
			{
				opt[7]=true;	gr->MeshNum = int(ff);
				if(out)	swprintf(buf,256,L"{int tmn=gr->MeshNum;\tgr->MeshNum=%d;", gr->MeshNum);
				wcscat(op1,buf);
			}
			else if(!wcscmp(a,L"alphadef"))
			{
				opt[6]=true;	gr->AlphaDef = ff;
				if(out)	swprintf(buf,256,L"{float tad=gr->AlphaDef;\tgr->AlphaDef=%g;", ff);
				wcscat(op1,buf);
			}
			else if(!wcscmp(a,L"alpha"))
			{
				opt[6]=true;	gr->AlphaDef = ff;
				if(out)	swprintf(buf,256,L"{float tad=gr->AlphaDef;\tgr->AlphaDef=%g;", ff);
				wcscat(op1,buf);
			}
			else if(!wcscmp(a,L"fontsize"))
			{
				opt[8]=true;	gr->FontSize = ff>0 ? ff : -ff*gr->FontSize;
				if(out)	swprintf(buf,256,L"{float tfs=gr->FontSize;\tgr->FontSize=%g;", gr->FontSize);
				wcscat(op1,buf);
			}
			else if(!wcscmp(a,L"ambient"))
			{
				opt[4]=true;	gr->Ambient(ff);
				if(out)	swprintf(buf,256,L"{float tam=gr->AmbBr;\tgr->Ambient(%g);", ff);
				wcscat(op1,buf);
			}
			else if(!wcscmp(a,L"marksize"))
			{
				opt[9]=true;	gr->MarkSize = ff/50;
				if(out)	swprintf(buf,256,L"{float tad=gr->MarkSize;\tgr->MarkSize=%g/50;", ff);
				wcscat(op1,buf);
			}
			else if(!wcscmp(a,L"legend"))
			{
				b = wcschr(s+1,'\'');	if(b)	*b=0;
				opt[10]=true;	wcscpy(leg,s+1);
			}
		}
	}
}
//-----------------------------------------------------------------------------
HMPR mgl_create_parser()		{	return new mglParse;	}
void mgl_delete_parser(HMPR p)	{	delete p;	}
void mgl_add_param(HMPR p, int id, const char *str)			{	p->AddParam(id,str);	}
void mgl_add_paramw(HMPR p, int id, const wchar_t *str)		{	p->AddParam(id,str);	}
HMDT mgl_add_var(HMPR p, const char *name)	{	mglVar *v=p->AddVar(name);	return &(v->d);	}
HMDT mgl_find_var(HMPR p, const char *name)	{	mglVar *v=p->FindVar(name);	return &(v->d);	}
int mgl_parse(HMGL gr, HMPR p, const char *str, int pos)	{	p->Parse(gr, str, pos);	}
int mgl_parsew(HMGL gr, HMPR p, const wchar_t *str, int pos){	p->Parse(gr, str, pos);	}
void mgl_restore_once(HMPR p)	{	p->RestoreOnce();	}
void mgl_parser_allow_setsize(HMPR p, bool a)	{	p->AllowSetSize = a;	}
//-----------------------------------------------------------------------------
long mgl_create_parser_()	{	return long(new mglParse);	}
void mgl_delete_parser_(long* p)	{	delete _PR_;	}
void mgl_add_param_(long* p, int *id, const char *str, int l)
{
	char *s=new char[l+1];		memcpy(s,str,l);	s[l]=0;
	_PR_->AddParam(*id, s);		delete []s;
}
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
long mgl_add_var_(long* p, const char *name, int l)
{
	char *s=new char[l+1];		memcpy(s,name,l);	s[l]=0;
	mglVar *v=_PR_->AddVar(s);	delete []s;
	return long(&(v->d));
}
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
long mgl_find_var_(long* p, const char *name, int l)
{
	char *s=new char[l+1];		memcpy(s,name,l);	s[l]=0;
	mglVar *v=_PR_->FindVar(s);	delete []s;
	return long(&(v->d));
}
int mgl_parse_(long* gr, long* p, const char *str, int *pos, int l)
{
	char *s=new char[l+1];		memcpy(s,str,l);	s[l]=0;
	_PR_->Parse(_GR_, s, *pos);	delete []s;
}
void mgl_restore_once_(long* p)	{	_PR_->RestoreOnce();	}
void mgl_parser_allow_setsize_(long* p, int *a)
{	_PR_->AllowSetSize = *a;	}
//-----------------------------------------------------------------------------
