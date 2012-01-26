/***************************************************************************
 * parse.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <stdlib.h>
#include <ctype.h>
#include <wchar.h>
//-----------------------------------------------------------------------------
#include "mgl/parser.h"
#include "mgl/canvas_cf.h"
//-----------------------------------------------------------------------------
#ifdef WIN32
#include <io.h>
wchar_t *wcstokw32(wchar_t *wcs, const wchar_t *delim)	{	return wcstok(wcs,delim);	}
#define wcstok(a,b,c) wcstokw32(a,b)
#else
#include <unistd.h>
#endif
//-----------------------------------------------------------------------------
mglFunc::mglFunc(long p, const wchar_t *f, mglFunc *prev)
{
	pos = p;	next = prev;
	register long i;
	for(i=0;(isalnum(f[i]) || f[i]=='_') && i<31;i++)	func[i]=f[i];
	func[i]=0;
	narg = wcstol(f+i+1,0,0);
	if(narg<0 || narg>9)	narg=0;
}
//-----------------------------------------------------------------------------
long mglParser::IsFunc(const wchar_t *name, int *na)
{
	mglFunc *f=func;
	while(f)
	{
		if(f->func==name)
		{	if(na)	*na=f->narg;	return f->pos;	}
		f = f->next;
	}
	return 0;
}
//-----------------------------------------------------------------------------
void mglParser::ScanFunc(const wchar_t *line)
{
	static long num=0;
	if(!line)
	{	if(func)	delete func;
		num=0;	func=0;	return;	}
	num++;
	if(wcsncmp(line,L"func",4) || line[4]>' ')	return;
	register long i;
	for(i=4;line[i]<=' ' || line[i]=='\'';i++);
	func = new mglFunc(num-1, line+i, func);
}
//-----------------------------------------------------------------------------
wchar_t *mgl_str_copy(const char *s)
{
	wchar_t *str = new wchar_t[strlen(s)+1];
	register long i;
	for(i=0;i<int(strlen(s));i++)	str[i] = s[i];
	str[i] = 0;
	return str;
}
//-----------------------------------------------------------------------------
int mgl_cmd_cmp(const void *a, const void *b)
{
	const mglCommand *aa = (const mglCommand *)a;
	const mglCommand *bb = (const mglCommand *)b;
	return strcmp(aa->name, bb->name);
}
//-----------------------------------------------------------------------------
bool check_for_name(const wchar_t *s)
{
	return !isalpha(s[0])||wcschr(s,'.')||wcschr(s,':')||wcschr(s,'(')||wcschr(s,')');
}
//-----------------------------------------------------------------------------
mglCommand *mglParser::FindCommand(const char *com)
{
	mglCommand tst, *rts, *cmd = Cmd;
	long i;
	for(i=0;cmd[i].name[0];i++);	// determine the number of symbols
	tst.name = com;
	rts = (mglCommand *) bsearch(&tst, cmd, i, sizeof(mglCommand), mgl_cmd_cmp);
	return rts;
}
//-----------------------------------------------------------------------------
mglCommand *mglParser::FindCommand(const wchar_t *com)
{
	char cmd[16];	wcstombs(cmd,com,wcslen(com)+1);
	return FindCommand(cmd);
}
//-----------------------------------------------------------------------------
// return values : 0 -- OK, 1 -- wrong arguments, 2 -- wrong command, 3 -- unclosed string
int mglParser::Exec(mglGraph *gr, const wchar_t *com, long n, mglArg *a, const wchar_t *var, const wchar_t *opt)
{
	int k[10], i;
	for(i=0;i<10;i++)	k[i] = i<n ? a[i].type + 1 : 0;
	for(i=0;i<n;i++)	a[i].s.assign(a[i].w.begin(),a[i].w.end());
	mglCommand *rts=FindCommand(com);
	if(!rts || rts->type==6)	return 2;
	if(rts->type == 4)
	{
		if(n<1 || check_for_name(var))	return 2;
		mglVar *v = AddVar(var);
		v->d.Create(1,1,1);
		a[0].type = 0;	a[0].d = &(v->d);
		a[0].w = var;	k[0] = 1;
	}
	char *o=0;
	if(opt && *opt)	// TODO: parse arguments of options
	{
		long len = wcslen(opt);
		o = new char[len+1];
		for(i=0;i<len+1;i++)	o[i]=opt[i];
	}
	if(out)	rts->save(out, n, a, k, o);
	int res=rts->exec(gr, n, a, k, o);
	if(o)	delete []o;
	return res;
}
//-----------------------------------------------------------------------------
void mglVar::MoveAfter(mglVar *var)
{
	if(prev)	prev->next = next;
	if(next)	next->prev = prev;
	prev = next = 0;
	if(var)
	{
		prev = var;	next = var->next;
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
void mglNum::MoveAfter(mglNum *var)
{
	if(prev)	prev->next = next;
	if(next)	next->prev = prev;
	prev = next = 0;
	if(var)
	{	prev = var;	next = var->next;	var->next = this;	}
	if(next)	next->prev = this;
}
//-----------------------------------------------------------------------------
mglNum::~mglNum()
{
	if(prev)	prev->next = next;
	if(next)	next->prev = prev;
}
//-----------------------------------------------------------------------------
mglParser::mglParser(bool setsize)
{
	DataList=0;	NumList=0;	func=0;	fn_stack=0;
//	wchar_t *par[40];	///< Parameter for substituting instead of $1, ..., $9
	out=0;	*leg=0;	InUse = 1;
	Skip=Stop=for_br=false;
	memset(for_stack,0,40*sizeof(int));
	memset(if_stack,0,40*sizeof(int));
	memset(if_for,0,40*sizeof(int));
	if_pos=fn_pos=fn_num=for_addr=0;
	for(long i=0;i<40;i++)	par[i]=0;

	Cmd = mgls_base_cmd;
	AllowSetSize=setsize;
	Once = true;	parlen=320;
	op1 = new wchar_t[4096];	op2 = new wchar_t[4096];
	fval = new mglData[40];
}
//-----------------------------------------------------------------------------
mglParser::~mglParser()
{
	if(DataList)
	{
		while(DataList->next)	delete DataList->next;
		delete DataList;
	}
	for(long i=0;i<40;i++)	if(par[i])	delete [](par[i]);
	delete []op1;	delete []op2;	delete []fval;
	if(Cmd!=mgls_base_cmd)	delete []Cmd;
	if(fn_stack)	free(fn_stack);
}
//-----------------------------------------------------------------------------
bool mglParser::AddParam(int n, const char *str, bool isstr)
{
	unsigned s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	bool r = AddParam(n,wcs,isstr);
	delete []wcs;	return r;
}
//-----------------------------------------------------------------------------
int mglParser::Parse(mglGraph *gr, const char *str, long pos)
{
	unsigned s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	int r = Parse(gr,wcs,pos);
	delete []wcs;	return r;
}
//-----------------------------------------------------------------------------
mglVar *mglParser::AddVar(const char *str)
{
	unsigned s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	mglVar *v = AddVar(wcs);
	delete []wcs;
	return v;
}
//-----------------------------------------------------------------------------
mglVar *mglParser::FindVar(const char *str)
{
	unsigned s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	mglVar *v = FindVar(wcs);
	delete []wcs;
	return v;
}
//-----------------------------------------------------------------------------
mglNum *mglParser::AddNum(const char *str)
{
	unsigned s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	mglNum *v = AddNum(wcs);
	delete []wcs;
	return v;
}
//-----------------------------------------------------------------------------
mglNum *mglParser::FindNum(const char *str)
{
	unsigned s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	mglNum *v = FindNum(wcs);
	delete []wcs;
	return v;
}
//-----------------------------------------------------------------------------
bool mglParser::AddParam(int n, const wchar_t *str, bool isstr)
{
	if(n<0 || n>39 || wcschr(str,'$'))	return false;
	if(!isstr)	parlen += wcslen(str);
	if(par[n])	delete [](par[n]);
	par[n] = new wchar_t[wcslen(str)+1];
	wcscpy(par[n],str);
	return true;
}
//-----------------------------------------------------------------------------
mglVar *mglParser::FindVar(const wchar_t *name)
{
	mglVar *v=DataList;
	while(v)
	{
		if(v->s==name)	return v;
		v = v->next;
	}
	return 0;
}
//-----------------------------------------------------------------------------
mglVar *mglParser::AddVar(const wchar_t *name)
{
	mglVar *v = FindVar(name);
	if(v)	return v;
	v = new mglVar;	v->s = name;
	if(DataList)	v->MoveAfter(DataList);
	else			DataList = v;
	return v;
}
//-----------------------------------------------------------------------------
mglNum *mglParser::FindNum(const wchar_t *name)
{
	mglNum *v=NumList;
	while(v)
	{
		if(v->s==name)	return v;
		v = v->next;
	}
	return 0;
}
//-----------------------------------------------------------------------------
mglNum *mglParser::AddNum(const wchar_t *name)
{
	mglNum *v = FindNum(name);
	if(v)	return v;
	v = new mglNum;	v->s = name;
	if(NumList)		v->MoveAfter(NumList);
	else			NumList = v;
	return v;
}
//-----------------------------------------------------------------------------
int mglFindArg(const wchar_t *str)
{
	register long l=0,k=0,i;//,j,len=strlen(lst);
	for(i=0;i<long(wcslen(str));i++)
	{
		if(str[i]=='\'') l++;
		if(str[i]=='{') k++;
		if(str[i]=='}') k--;
		if(l%2==0 && k==0 && (str[i]=='#' || str[i]==';'))	return -i;
		if(l%2==0 && k==0 && (str[i]<=' '))	return i;
	}
	return 0;
}
//-----------------------------------------------------------------------------
bool mgls_suffix(const wchar_t *p, mglData *d, mreal *v)
{
	mreal x,y,z,k;
	bool ok=false;
	if(!wcscmp(p,L"a"))			{	ok = true;	*v = d->a[0];	}
	else if(!wcscmp(p,L"fst"))	{	ok = true;	long i=-1,j=-1,l=-1;	*v = d->Find(0,i,j,l);	}
	else if(!wcscmp(p,L"lst"))	{	ok = true;	long i=-1,j=-1,l=-1;	*v = d->Last(0,i,j,l);	}
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
mglData mglFormulaCalc(const wchar_t *string, mglParser *arg);
void mglParser::FillArg(mglGraph *gr, int k, wchar_t **arg, mglArg *a)
{
	register long n;
	for(n=1;n<k;n++)
	{
		mglVar *v, *u;
		mglNum *f;
		a[n-1].type = -1;
		if(arg[n][0]=='|')	a[n-1].type = -1;
		else if(arg[n][0]=='\'')
		{	// this is string (simplest case)
			a[n-1].type = 1;	arg[n][wcslen(arg[n])-1] = 0;
			if(wcslen(arg[n]+1)>=2048)	arg[n][2048]=0;
			a[n-1].w = arg[n]+1;
		}
		else if(arg[n][0]=='{')
		{	// this is temp data
			arg[n][wcslen(arg[n])-1] = 0;
			u=new mglVar;	u->temp=true;
			a[n-1].w = L"/*"+std::wstring(arg[n])+L"*/";
			if(DataList)	u->MoveAfter(DataList);
			else			DataList = u;
			a[n-1].type = 0;	a[n-1].d = &(u->d);
			ParseDat(gr, arg[n]+1, u->d);	// TODO: Check it
		}
		else if((v = FindVar(arg[n]))!=0)	// have to find normal variables (for data creation)
		{	a[n-1].type=0;	a[n-1].d=&(v->d);	a[n-1].w=v->s;	}
		else if((f = FindNum(arg[n]))!=0)	// have to find normal variables (for data creation)
		{	a[n-1].type=2;	a[n-1].d=0;	a[n-1].v=f->d;	a[n-1].w = f->s;	}
		else
		{	// parse all numbers and formulas by unified way
			const mglData &d=mglFormulaCalc(arg[n], this);
			if(d.nx*d.ny*d.nz==1)
			{	a[n-1].type = 2;	a[n-1].v = d.a[0];	}
			else
			{
				u=new mglVar;	u->temp=true;	u->d=d;
				a[n-1].w = L"/*"+std::wstring(arg[n])+L"*/";
				if(DataList)	u->MoveAfter(DataList);
				else			DataList = u;
				a[n-1].type = 0;	a[n-1].d = &(u->d);
			}
		}
	}
}
//-----------------------------------------------------------------------------
// return values: 0 - not found, 1 - OK, 2 - wrong arguments, 3 - wrong command, 4 - string too long
int mglParser::PreExec(mglGraph *, long k, wchar_t **arg, mglArg *a)
{
	long n=0;
	mglVar *v;
	if(!wcscmp(L"delete",arg[0]))	// parse command "delete"
	{
		if(k<2)	return 2;
		DeleteVar(arg[1]);	n=1;
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
					memcpy(v->d.a+j*nn,b->a,nn*(b->nz)*sizeof(mreal));
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
					memcpy(v->d.a+j*nn,b->a,nn*(b->ny)*sizeof(mreal));
					j+=b->ny;
				}
			}
		}
		n=1;
	}
	return n;
}
//-----------------------------------------------------------------------------
void mglParser::PutArg(const wchar_t *string, wchar_t *str, bool def)
{
	if(parlen>0)
	{
		wchar_t *sb = new wchar_t[wcslen(string)+1], *t;
		if(def)	str = str+10;
		register long n;
		while((t=wcschr(str,'$'))!=0)
		{
			wcscpy(sb,t+2);	t[0]=0;
			n = t[1]-'0';	if(n>=0 && n<=9 && par[n])	wcscat(str,par[n]);
			n = t[1]-'a';	if(n>=0 && n<='z'-'a' && par[n+10])	wcscat(str,par[n+10]);
			if(t[1]=='$')	wcscat(str,L"\xffff");
			wcscat(str,sb);
		}
		delete []sb;
		while((t=wcschr(str,L'\xffff'))!=0)	*t='$';
	}
}
//-----------------------------------------------------------------------------
// return values: 0 - OK, 1 - wrong arguments, 2 - wrong command, 3 - string too long, 4 -- unclosed string
int mglParser::Parse(mglGraph *gr, const wchar_t *string, long pos)
{
	if(Stop)	return 0;
	wchar_t *str, *s = new wchar_t[wcslen(string)+1+40*parlen],*arg[1024],*t;
	str = s;
	wcscpy(str,string);
	mgl_wcstrim(str);
	long n,k=0,m=0,mm=0;
	// try parse ':' -- several commands in line
	for(n=0;n<long(wcslen(str));n++)
	{
		if(str[n]=='\'' && (n==0 || str[n-1]!='\\'))	k++;
		if(str[n]=='(' && k%2==0)	m++;
		if(str[n]==')' && k%2==0)	m--;
		if(str[n]=='{' && k%2==0)	mm++;
		if(str[n]=='}' && k%2==0)	mm--;
		if(str[n]=='#' && k%2==0)	break;
		if((str[n]==':' || str[n]=='\n') && k%2==0 && m==0 && mm==0)
		{
			str[n]=0;
			int res=Parse(gr,str,pos);
			if(!res)	res=Parse(gr,str+n+1,pos);
			delete []s;	return res;
		}
	}
	if(k%2 || m || mm)	return 4;	// strings is not closed
	// define parameters or start cycle
	if(!skip() && !wcsncmp(str,L"def",3) && (str[6]==' ' || str[6]=='\t'))
	{
		PutArg(string,str,true);
		if(!wcsncmp(str+3,L"ine",3))
		{
			str += 7;	mgl_wcstrim(str);//	int res = 1;
			int nn = str[1]<='9' ? str[1]-'0' : (str[1]>='a' ? str[1]-'a'+10:-1);
			if(*str=='$' && nn>=0 && nn<='z'-'a'+10)
			{
				str +=2;	mgl_wcstrim(str);
				AddParam(n, str);	delete []s;	return 0;
			}
		}
		if(!wcsncmp(str+3,L"num",3))
		{
			str += 7;	mgl_wcstrim(str);	int res = 1;
			int nn = str[1]<='9' ? str[1]-'0' : (str[1]>='a' ? str[1]-'a'+10:-1);
			if(*str=='$' && nn>=0 && nn<='z'-'a'+10)
			{
				res = 0;	str +=2;	mgl_wcstrim(str);
				const mglData &d=mglFormulaCalc(str, this);
				char *buf=new char[128];
				sprintf(buf,"%g",d.a[0]);
				AddParam(n, buf);	delete []buf;
			}
			delete []s;	return res;
		}
		if(!wcsncmp(str+3,L"chr",3))
		{
			str += 7;	mgl_wcstrim(str);	int res = 1;
			int nn = str[1]<='9' ? str[1]-'0' : (str[1]>='a' ? str[1]-'a'+10:-1);
			if(*str=='$' && nn>=0 && nn<='z'-'a'+10)
			{
				res = 0;	str +=2;	mgl_wcstrim(str);
				const mglData &d=mglFormulaCalc(str, this);
				wchar_t buf[2]={0,0};	buf[0] = wchar_t(d.a[0]);
				AddParam(n, buf);
			}
			delete []s;	return res;
		}
	}
	if(!skip() && !wcsncmp(str,L"for",3) && (str[3]==' ' || str[3]=='\t'))
	{
		for(t=str+4;*t<=' ';t++);
		// if command have format 'for $N ...' then change it to 'for N ...'
		if(*t=='$' && t[1]>='0' && t[1]<='9')	*t = ' ';
		if(*t=='$' && t[1]>='a' && t[1]<='z')	*t = ' ';
	}

	// parse arguments (parameters $1, ..., $9)
	PutArg(string,str,false);	mgl_wcstrim(str);

	wchar_t *opt=0;
	for(k=0;k<1024;k++)	// parse string to substrings (by spaces)
	{
		n = mglFindArg(str);
		if(n<1)	// this is option
		{
			if(str[-n]==';')	opt = str-n+1;
			if(n<0)	str[-n]=0;
			break;
		}
		str[n]=0;	arg[k] = str;//	k++;
		str = str+n+1;	mgl_wcstrim(str);
	}
	// try to find last argument
	if(str[0]!=0 && str[0]!='#' && str[0]!=';')	{	arg[k] = str;	k++;	}
	if(k<1) n =0;
	else
	{
		// fill arguments by its values
		mglArg *a = new mglArg[k];
		FillArg(gr, k, arg, a);
		// execute first special (program-flow-control) commands
		if(!skip() && !wcscmp(arg[0],L"stop"))
		{	Stop = true;	delete []s;	delete []a;	return 0;	}
		if(!wcscmp(arg[0],L"func"))
		{	delete []s;	delete []a;	return 0;	}
		n = FlowExec(gr, arg[0],k-1,a);
		if(n)		{	delete []s;	delete []a;	return n-1;	}
		if(skip())	{	delete []s;	delete []a;	return 0;	}
		if(!wcscmp(arg[0],L"define"))
		{
			if(k==3)
			{
				mglNum *v=AddNum(arg[1]);
				mglData d=mglFormulaCalc(arg[2],this);
				v->d = d.a[0];
			}
			delete []s;	delete []a;	return k==3?0:1;
		}
		if(!wcscmp(arg[0],L"call"))
		{
			n = 1;
			if(a[0].type==1)
			{
				int na=0;
				a[0].s.assign(a[0].w.begin(),a[0].w.end());
				n=-IsFunc(a[0].w.c_str(),&na);
				if(n && k!=na+2)
				{
					char buf[64];
					sprintf(buf,"Bad arguments for %ls: %ld instead of %d\n",
							a[0].w.c_str(),k-2,na);
					gr->SetWarn(-1,buf);	n = 1;
				}
				else if(n)
				{
					if(!fn_stack)
					{	fn_num = 100;
						fn_stack = (mglFnStack*)malloc(fn_num*sizeof(mglFnStack));
						memset(fn_stack,0,fn_num*sizeof(mglFnStack));	}
					if(fn_pos >= fn_num)
					{	fn_num+= 100;
						fn_stack = (mglFnStack*)realloc(fn_stack,fn_num*sizeof(mglFnStack));
						memset(fn_stack+fn_num-100,0,100*sizeof(mglFnStack));	}
					memcpy(fn_stack[fn_pos].par,par+1,9*sizeof(wchar_t*));
					memset(par+1,0,9*sizeof(wchar_t*));
					for(int i=1;i<k-1;i++)	AddParam(i,arg[i+1]);
					fn_stack[fn_pos].pos = pos;	fn_pos++;	n--;
				}
				else
				{
					FILE *fp = fopen(a[0].s.c_str(),"rt");
					if(fp)	{	Execute(gr,fp);	fclose(fp);	}
					else	n=1;
				}
			}
			delete []s;	delete []a;	return n;
		}
		if(!wcscmp(arg[0],L"for"))
		{
			n = 1;
			char ch = arg[1][0];
			int r = ch-'0';
			if(ch>='a' && ch<='z')	r = 10+ch-'a';
//			int r = int(a[0].v);
			if(arg[1][1]==0 && (r>=0 || r<=39))
			{
				if(a[1].type==0)
				{
					n=0;		fval[r] = *(a[1].d);
					fval[r].nx *= fval[r].ny*fval[r].nz;
				}
				else if(a[1].type==2 && a[2].type==2 && a[2].v>a[1].v)
				{
					mreal step = a[3].type==2?a[3].v:1;
					mm = int(step>0 ? (a[2].v-a[1].v)/step : 0);
					if(mm>0)
					{
						n=0;	fval[r].Create(mm+1);
						for(int ii=0;ii<mm+1;ii++)
							fval[r].a[ii] = a[1].v + step*ii;
					}
				}
				if(n==0)
				{
					for(int i=39;i>0;i--)
					{	for_stack[i]=for_stack[i-1];	if_for[i]=if_for[i-1];	}
					for_stack[0] = r+1;		fval[r].nz = pos;	if_for[0]=if_pos;
					wchar_t buf[32];		mglprintf(buf,32,L"%g",fval[r].a[0]);
					AddParam(r, buf,true);	fval[r].ny = 1;
				}
			}
			delete []s;	delete []a;	return n;
		}
		// alocate new arrays and execute the command itself
		n = PreExec(gr, k, arg, a);
		if(n>0)	n--;
		else if(!wcscmp(L"setsize",arg[0]) && !AllowSetSize)	n = 2;
		else	n = Exec(gr, arg[0],k-1,a, arg[1], opt);
		delete []a;
	}
	mglVar *v = DataList, *u;
	while(v)	// remove temporary data arrays
	{
		u = v->next;
		if(v->temp)	{	if(DataList==v)	DataList = v->next;		delete v;	}
		v = u;
	}
	delete []s;	return n;
}
//-----------------------------------------------------------------------------
// return values: 0 - OK, 1 - wrong arguments, 2 - wrong command, 3 - string too long, 4 -- unclosed string
int mglParser::ParseDat(mglGraph *gr, const wchar_t *string, mglData &res)
{
	wchar_t *str, *s = new wchar_t[wcslen(string)+1+parlen],*arg[32];
	str = s;
	wcscpy(str,string);	mgl_wcstrim(str);
	long n,k=0;
	for(k=0;k<32;k++)	// parse string to substrings (by spaces)
	{
		n = mglFindArg(str);
		if(n<0)	{	str[-n]=0;	break;	}
		str[n]=0;	arg[k] = str;//	k++;
		str = str+n+1;	mgl_wcstrim(str);
	}
	// try to find last argument
	if(str[0]!=0 && str[0]!='#' && str[0]!=';')	{	arg[k] = str;	k++;	}
	if(k<1) n =0;
	else
	{	// fill arguments by its values
		mglArg *a = new mglArg[k+1];
		FillArg(gr, k, arg, a+1);	a[0].type=0;	a[0].d=&res;
		// alocate new arrays and execute the command itself
		int kk[10], i;
		for(i=0;i<10;i++)	kk[i] = i<=k ? a[i].type + 1 : 0;
		for(i=0;i<=k;i++)	a[i].s.assign(a[i].w.begin(),a[i].w.end());
		mglCommand *rts=FindCommand(arg[0]);
		if(!rts || rts->type!=4)	return 2;
		n = rts->exec(gr, k, a, kk, 0);
		delete []a;
	}
	delete []s;	return n;
}
//-----------------------------------------------------------------------------
int mglParser::FlowExec(mglGraph *, const wchar_t *com, long m, mglArg *a)
{
	int n=-1;
	if(!ifskip() && !wcscmp(com,L"once"))
	{
		if(a[0].type==2)
		{
			n = 0;
			if(a[0].v)
			{	Skip = !Once;	if(out)	mglprintf(out,1024,L"if(!once)\t{\tonce = true;");	}
			else
			{	Skip = Once = false;	if(out)	mglprintf(out,1024,L"}");	}
		}
		else n = 1;
	}
	else if(!Skip && !wcscmp(com,L"if"))
	{
		int cond;
		if(a[0].type==2)
		{
			n = 0;	cond = (a[0].v!=0)?3:0;
			if(out)	mglprintf(out,1024,L"if(%g!=0)\t{", a[0].v);
		}
		else if(a[0].type==0)
		{
			n = 0;	a[1].s.assign(a[1].w.begin(),a[1].w.end());
			cond = a[0].d->FindAny((m>1 && a[1].type==1) ? a[1].s.c_str():"u")?3:0;
			if(out)	mglprintf(out,1024,L"if(%ls.FindAny(\"%s\"))\t{",
						a[0].w.c_str(), (m>1 && a[1].type==1) ? a[1].s.c_str():"u");
		}
		else n = 1;
		if(n==0)
		{	if_stack[if_pos] = cond;	if_pos = if_pos<39 ? if_pos+1 : 39;	}
	}
	else if(!Skip && !wcscmp(com,L"endif"))
	{
		if_pos = if_pos>0 ? if_pos-1 : 0;
		n = 0;	if(out)	mglprintf(out,1024,L"}");
	}
	else if(!Skip && !wcscmp(com,L"else"))
	{
		if(if_pos>0)
		{
			n=0; if_stack[if_pos-1] = (if_stack[if_pos-1]&2)?2:3;
			if(out)	mglprintf(out,1024,L"}\telse\t{");
		}
		else n = 1;
	}
	else if(!Skip && !wcscmp(com,L"elseif"))
	{
		int cond;
		if(if_pos<1 || m<1)	n = 1;
		else if(if_stack[if_pos-1]&2)	{	n = 0;	cond = 2;	}
		else if(a[0].type==2)
		{
			n = 0;	cond = (a[0].v!=0)?3:0;
			if(out)	mglprintf(out,1024,L"else if(%g!=0)\t{", a[0].v);
		}
		else if(a[0].type==0)
		{
			n = 0;	a[1].s.assign(a[1].w.begin(),a[1].w.end());
			cond = a[0].d->FindAny((m>1 && a[1].type==1) ? a[1].s.c_str():"u")?3:0;
			if(out)	mglprintf(out,1024,L"else if(%ls.FindAny(\"%s\"))\t{",
						a[0].w.c_str(), (m>1 && a[1].type==1) ? a[1].s.c_str():"u");
		}
		else n = 1;
		if(n==0)	if_stack[if_pos-1] = cond;
	}
	else if(!ifskip() && !Skip && !wcscmp(com,L"break"))
	{
		if(if_pos==if_for[0])	if_pos = if_pos>0 ? if_pos-1 : 0;
		if(out)	mglprintf(out,1024,L"break;");	for_br = true;
	}
	else if(!skip() && !wcscmp(com, L"return"))	// parse command "delete"
	{
		if(!fn_pos)	return 2;
		fn_pos--;	n = -fn_stack[fn_pos].pos-1;
		for(int i=1;i<10;i++)	if(par[i])	delete []par[i];
		memcpy(par+1,fn_stack[fn_pos].par,9*sizeof(wchar_t*));
	}
	else if(!ifskip() && !Skip && !wcscmp(com,L"next"))
	{
		if(if_pos==if_for[0])	if_pos = if_pos>0 ? if_pos-1 : 0;
		int r = for_stack[0]-1;
		n = for_stack[0] ? 0:1;
		if(for_stack[0])
		{
			if(fval[r].ny<fval[r].nx && !for_br)
			{
				wchar_t buf[32];		mglprintf(buf,32,L"%g",fval[r].a[fval[r].ny]);
				AddParam(r, buf,true);	fval[r].ny += 1;
				n = -fval[r].nz-1;
			}
			else
			{
				for(int i=0;i<39;i++)
				{	for_stack[i]=for_stack[i+1];	if_for[i]=if_for[i+1];	}
				for_stack[39] = 0;	for_br=false;
			}
		}
		if(out)	mglprintf(out,1024,L"}");
	}
	else if(!ifskip() && !Skip && !wcscmp(com,L"continue"))
	{
		if(if_pos==if_for[0])	if_pos = if_pos>0 ? if_pos-1 : 0;
		int r = for_stack[0]-1;
		n = for_stack[0] ? 0:1;
		if(for_stack[0])
		{
			if(fval[r].ny<fval[r].nx)
			{
				wchar_t buf[32];		mglprintf(buf,32,L"%g",fval[r].a[fval[r].ny]);
				AddParam(r, buf,true);	fval[r].ny += 1;
				n = -fval[r].nz-1;
			}
			else	for_br = true;
		}
		if(out)	mglprintf(out,1024,L"continue;");
	}
	return n+1;
}
//-----------------------------------------------------------------------------
void mgl_error_print(const char *mess, void *)
{	if(mess && *mess)	printf("%s\n",mess);	}
#include <string>
void mglParser::Execute(mglGraph *gr, FILE *fp, bool print)
{
	if(gr==0 || fp==0)	return;
	std::wstring str;
	while(!feof(fp))	str.push_back(fgetwc(fp));
	Execute(gr,str.c_str(),print?mgl_error_print:NULL);
}
//-----------------------------------------------------------------------------
void mglParser::Execute(mglGraph *gr, int n, const wchar_t **text, void (*error)(const char *mes, void *par), int high, void *par)
{
	if(n<1 || text==0)	return;
	long i, r;
	char buf[64];
	for_br=Skip=false;	if_pos=fn_pos=0;	ScanFunc(0);
	for(i=0;i<n;i++)	ScanFunc(text[i]);
	for(i=0;i<n;i++)
	{
		gr->SetWarn(-1, NULL);
		if(i==high)	gr->Highlight();
		gr->SetObjId(i+1);
		r = Parse(gr,text[i],i+1);
		if(r<0)	{	i = -r-2;	continue;	}
		if(r==1)		sprintf(buf,"\nWrong argument(s) in line %ld\n", i+1);
		else if(r==2)	sprintf(buf,"\nWrong command in line %ld\n", i+1);
		else if(r==3)	sprintf(buf,"\nString too long in line %ld\n", i+1);
		else if(r==4)	sprintf(buf,"\nUnbalanced ' in line %ld\n", i+1);
		else if(gr->GetWarn()>0)	sprintf(buf," in line %ld\n", i+1);
		else *buf=0;
		if(*buf)	gr->SetWarn(-2,buf);
	}
	const char *mess=gr->Message();
	if(error && mess && *mess)	error(mess,par);
}
//-----------------------------------------------------------------------------
void mglParser::Execute(mglGraph *gr, const wchar_t *text, void (*error)(const char *mes, void *par), int high, void *par)
{
	unsigned s = wcslen(text)+1;
	wchar_t *wcs = new wchar_t[s];
	const wchar_t **str;
	register unsigned i, n=1;
	for(i=0;i<s;i++)	if(text[i]=='\n')	n++;
	str = (const wchar_t **)malloc(n*sizeof(wchar_t *));
	memcpy(wcs, text, s*sizeof(wchar_t));
	str[0] = wcs;	n=1;
	long next=0;
	for(i=0;i<s;i++)
	{
		if(text[i]=='\\')	next = i;
		else if(text[i]>' ')next = 0;
		if(text[i]=='\n')
		{	// if string need to be continued then I but ' ' instead of 0x0 and
			// pointer next string to 0x0. Last one for keeping number of strings.
			if(next)
			{	wcs[i]=wcs[next]=' ';	str[n] = wcs+s-1;	next=0;	}
			else
			{	wcs[i]=0;	str[n] = wcs+i+1;	}
			n++;

		}
	}
	Execute(gr, n, str, error, high, par);
	delete []wcs;	free(str);
}
//-----------------------------------------------------------------------------
void mglParser::Execute(mglGraph *gr, const char *text, void (*error)(const char *mes, void *par), int high, void *par)
{
	unsigned s = strlen(text)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,text,s);
	Execute(gr, wcs, error, high, par);
	delete []wcs;
}
//-----------------------------------------------------------------------------
int mglParser::Export(wchar_t cpp_out[1024], mglGraph *gr, const wchar_t *str)
{
	*op1 = *op2 = 0;
	out = cpp_out;
	int res = Parse(gr, str);
	out = 0;
	return res;
}
//-----------------------------------------------------------------------------
void mglParser::DeleteVar(mglVar *v)
{
	if(!v)	return;
	if(DataList==v)	DataList = v->next;
	delete v;
}
//-----------------------------------------------------------------------------
void mglParser::DeleteVar(const char *name)
{
	mglVar *v = FindVar(name);
	DeleteVar(v);
}
//-----------------------------------------------------------------------------
void mglParser::DeleteVar(const wchar_t *name)
{
	mglVar *v = FindVar(name);
	DeleteVar(v);
}
//-----------------------------------------------------------------------------
void mglParser::AddCommand(mglCommand *cmd, int mc)
{
	int i, mp;
	if(mc<1)
	{	for(i=0;cmd[i].name[0];i++){};	mc = i;	}
	// determine the number of symbols
	for(i=0;Cmd[i].name[0];i++){};	mp = i;
	mglCommand *buf = new mglCommand[mp+mc+1];
	memcpy(buf, cmd, mc*sizeof(mglCommand));
	memcpy(buf+mc, Cmd, (mp+1)*sizeof(mglCommand));
	qsort(buf, mp+mc, sizeof(mglCommand), mgl_cmd_cmp);
	if(Cmd!=mgls_base_cmd)   delete []Cmd;
	Cmd = buf;
}
//-----------------------------------------------------------------------------
HMPR mgl_create_parser()		{	return new mglParser;	}
void mgl_delete_parser(HMPR p)	{	delete p;	}
void mgl_add_param(HMPR p, int id, const char *str)			{	p->AddParam(id,str);	}
void mgl_add_paramw(HMPR p, int id, const wchar_t *str)		{	p->AddParam(id,str);	}
HMDT mgl_add_var(HMPR p, const char *name)	{	mglVar *v=p->AddVar(name);	return &(v->d);	}
HMDT mgl_find_var(HMPR p, const char *name)	{	mglVar *v=p->FindVar(name);	return &(v->d);	}
void mgl_del_var(HMPR p, const char *name)	{	p->DeleteVar(name);	}
int mgl_parse(HMGL gr, HMPR p, const char *str, int pos)
{	return p->Parse(gr, str, pos);	}
int mgl_parsew(HMGL gr, HMPR p, const wchar_t *str, int pos)
{	return p->Parse(gr, str, pos);	}
void mgl_parse_text(HMGL gr, HMPR p, const char *str, void (*error)(const char *mes, void *par), int high, void *par)
{	p->Execute(gr, str, error, high, par);	}
void mgl_parsew_text(HMGL gr, HMPR p, const wchar_t *str, void (*error)(const char *mes, void *par), int high, void *par)
{	p->Execute(gr, str, error, high, par);	}
void mgl_parse_file(HMGL gr, HMPR p, FILE *fp, int print)
{	p->Execute(gr,fp,print);	}
void mgl_restore_once(HMPR p)	{	p->RestoreOnce();	}
void mgl_parser_stop(HMPR p)	{	p->Stop = true;		}
void mgl_parser_allow_setsize(HMPR p, int a)	{	p->AllowSetSize = a;	}
//-----------------------------------------------------------------------------
#define _PR_	((mglParser *)(*p))
uintptr_t mgl_create_parser_()	{	return uintptr_t(new mglParser);	}
void mgl_delete_parser_(uintptr_t* p)	{	delete _PR_;	}
void mgl_add_param_(uintptr_t* p, int *id, const char *str, int l)
{	char *s=new char[l+1];		memcpy(s,str,l);	s[l]=0;
	_PR_->AddParam(*id, s);		delete []s;	}
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
uintptr_t mgl_add_var_(uintptr_t* p, const char *name, int l)
{	char *s=new char[l+1];		memcpy(s,name,l);	s[l]=0;
	mglVar *v=_PR_->AddVar(s);	delete []s;
	return uintptr_t(&(v->d));	}
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
uintptr_t mgl_find_var_(uintptr_t* p, const char *name, int l)
{	char *s=new char[l+1];		memcpy(s,name,l);	s[l]=0;
	mglVar *v=_PR_->FindVar(s);	delete []s;
	return uintptr_t(&(v->d));	}
void mgl_del_var_(uintptr_t* p, const char *name, int l)
{	char *s=new char[l+1];		memcpy(s,name,l);	s[l]=0;
	_PR_->DeleteVar(s);	delete []s;	}
int mgl_parse_(uintptr_t* gr, uintptr_t* p, const char *str, int *pos, int l)
{	char *s=new char[l+1];		memcpy(s,str,l);	s[l]=0;
	int r = _PR_->Parse(_GR_, s, *pos);	delete []s;	return r;	}
void mgl_parse_text_(uintptr_t* gr, uintptr_t* p, const char *str, int l)
{	char *s=new char[l+1];		memcpy(s,str,l);	s[l]=0;
	_PR_->Execute(_GR_, s);	delete []s;	}
void mgl_restore_once_(uintptr_t* p)	{	_PR_->RestoreOnce();	}
void mgl_parser_allow_setsize_(uintptr_t* p, int *a)
{	_PR_->AllowSetSize = *a;	}
void mgl_parser_stop_(uintptr_t* p)	{	_PR_->Stop = true;	}
//-----------------------------------------------------------------------------
long mgl_use_parser(HMPR pr, int inc)
{	pr->InUse+=inc;	return pr->InUse;	}
long mgl_use_parser_(uintptr_t *p, int *inc)
{	_PR_->InUse+=*inc;	return _PR_->InUse;	}
//---------------------------------------------------------------------------
int mgl_cmd_type(HMPR pr, const char *name)
{
	mglCommand *cmd = pr->FindCommand(name);
	return cmd ? cmd->type + 1 : 0;
}
int mgl_cmd_type_(uintptr_t* p, const char *str, int l)
{	char *s=new char[l+1];	memcpy(s,str,l);	s[l]=0;
	l = mgl_cmd_type(_PR_, s);	delete []s;	return l;	}
//---------------------------------------------------------------------------
const char *mgl_cmd_desc(HMPR pr, const char *name)
{
	mglCommand *cmd = pr->FindCommand(name);
	return cmd ? cmd->desc : 0;
}
const char *mgl_cmd_frmt(HMPR pr, const char *name)
{
	mglCommand *cmd = pr->FindCommand(name);
	return cmd ? cmd->form : 0;
}
//---------------------------------------------------------------------------
