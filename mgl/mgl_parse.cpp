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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#include <unistd.h>
#else
#include <direct.h>
#endif
//#include <unistd.h>
#include "mgl/mgl_parse.h"
char *mgl_strdup(const char *s);
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
	return strcmp(aa->name, bb->name);
}
//-----------------------------------------------------------------------------
// return values : 0 -- OK, 1 -- wrong arguments, 2 -- wrong command
int mglParse::Exec(mglGraph *gr, const char *com, long n, mglArg *a)
{
	char ch = com[0];
	int k[9], i;
	for(i=0;i<9;i++)	k[i] = i<n ? a[i].type + 1 : 0;
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
	Once = true;	Skip = false;
	parlen=320;		Stop=false;		if_pos = 0;
	for(long i=0;i<10;i++)	{	par[i] = 0;	for_stack[i]=0;	opt[i]=false;	}
}
//-----------------------------------------------------------------------------
bool mglParse::AddParam(int n, const char *str, bool isstr)
{
	if(n<0 || n>9 || strchr(str,'$'))	return false;
	if(!isstr)	parlen += strlen(str);
	if(par[n])	delete []par[n];
	par[n] = new char[strlen(str)+1];
	strcpy(par[n],str);
	return true;
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
}
//-----------------------------------------------------------------------------
mglVar *mglParse::FindVar(const char *name)
{
	mglVar *v=DataList;
	char *s = mgl_strdup(name),*p;
	p = strchr(s,'.');	if(p)	{	p[0]=0;	p++;	}
	while(v)
	{
		if(!strcmp(s, v->s))	{	free(s);	return v;	}
		v = v->next;
	}
	free(s);
	return 0;
}
//-----------------------------------------------------------------------------
mglVar *mglParse::AddVar(const char *name)
{
	mglVar *v = FindVar(name);
	if(v)	return v;
	v = new mglVar;
	strncpy(v->s,name,256);
	if(DataList)	v->MoveAfter(DataList);
	else			DataList = v;
	return v;
}
//-----------------------------------------------------------------------------
int mglFindArg(const char *str)
{
	register long l=0,i;//,j,len=strlen(lst);
	for(i=0;i<long(strlen(str));i++)
	{
		if(l%2==0 && (str[i]=='#' || str[i]==';'))	return -i;
		if(str[i]=='\'') l++;
		if(l%2==0 && (str[i]==' ' || str[i]=='\t' || str[i]=='\n'))	return i;
	}
	return 0;
}
//-----------------------------------------------------------------------------
void strtrim_mgl(char *str);
void strlwr_mgl(char *str);
//-----------------------------------------------------------------------------
bool mgls_suffix(const char *p, mglData *d, float *v)
{
	float x,y,z,k;
	bool ok=false;
	if(!strcmp(p,"a"))			{	ok = true;	*v = d->a[0];	}
	else if(!strcmp(p,"fst"))	{	ok = true;	int i=-1,j=-1,k=-1;	*v = d->Find(0,i,j,k);	}
	else if(!strcmp(p,"lst"))	{	ok = true;	int i=-1,j=-1,k=-1;	*v = d->Last(0,i,j,k);	}
	else if(!strcmp(p,"nx"))	{	ok = true;	*v=d->nx;	}
	else if(!strcmp(p,"ny"))	{	ok = true;	*v=d->ny;	}
	else if(!strcmp(p,"nz"))	{	ok = true;	*v=d->nz;	}
	else if(!strcmp(p,"max"))	{	ok = true;	*v=d->Maximal();	}
	else if(!strcmp(p,"min"))	{	ok = true;	*v=d->Minimal();	}
	else if(!strcmp(p,"sum"))	{	ok = true;	*v=d->Momentum('x',x,y);	}
	else if(!strcmp(p,"mx"))	{	ok = true;	d->Maximal(x,y,z);	*v=x/d->nx;	}
	else if(!strcmp(p,"my"))	{	ok = true;	d->Maximal(x,y,z);	*v=y/d->ny;	}
	else if(!strcmp(p,"mz"))	{	ok = true;	d->Maximal(x,y,z);	*v=z/d->nz;	}
	else if(!strcmp(p,"ax"))	{	ok = true;	d->Momentum('x',x,y);*v=x/d->nx;	}
	else if(!strcmp(p,"ay"))	{	ok = true;	d->Momentum('y',x,y);*v=x/d->ny;	}
	else if(!strcmp(p,"az"))	{	ok = true;	d->Momentum('z',x,y);*v=x/d->nz;	}
	else if(!strcmp(p,"wx"))	{	ok = true;	d->Momentum('x',x,y);*v=y/d->nx;	}
	else if(!strcmp(p,"wy"))	{	ok = true;	d->Momentum('y',x,y);*v=y/d->ny;	}
	else if(!strcmp(p,"wz"))	{	ok = true;	d->Momentum('z',x,y);*v=y/d->nz;	}
	else if(!strcmp(p,"sx"))	{	ok = true;	d->Momentum('x',x,y,z,k);	*v=z/d->nx;	}
	else if(!strcmp(p,"sy"))	{	ok = true;	d->Momentum('y',x,y,z,k);	*v=z/d->ny;	}
	else if(!strcmp(p,"sz"))	{	ok = true;	d->Momentum('z',x,y,z,k);	*v=z/d->nz;	}
	else if(!strcmp(p,"kx"))	{	ok = true;	d->Momentum('x',x,y,z,k);	*v=k/d->nx;	}
	else if(!strcmp(p,"ky"))	{	ok = true;	d->Momentum('y',x,y,z,k);	*v=k/d->ny;	}
	else if(!strcmp(p,"kz"))	{	ok = true;	d->Momentum('z',x,y,z,k);	*v=k/d->nz;	}
	else if(!strcmp(p,"aa"))	{	ok = true;	d->Momentum('a',x,y);	*v=x;	}
	else if(!strcmp(p,"wa"))	{	ok = true;	d->Momentum('a',x,y);	*v=y;	}
	else if(!strcmp(p,"sa"))	{	ok = true;	d->Momentum('a',x,y,z,k);*v=z;	}
	else if(!strcmp(p,"ka"))	{	ok = true;	d->Momentum('a',x,y,z,k);*v=k;	}
	return ok;
}
//-----------------------------------------------------------------------------
// convert substrings to arguments
void mglParse::FillArg(int k, char **arg, mglArg *a)
{
	char *s, *t, *p=0;
	for(long n=1;n<k;n++)
	{
		mglVar *v, *u;
		a[n-1].type = -1;
		p = strchr(arg[n],'.');
		if(arg[n][0]!='\'' && (s=strchr(arg[n],'(')))	// subdata or column in argument
		{
			p = strchr(arg[n],'.');	if(p)	{	p[0]=0;	p++;	}
			s = mgl_strdup(arg[n]);
			t = strtok (s, "(,)");
			v = FindVar(t);
			if(v==0)
			{	a[n-1].type = 2;	a[n-1].v = NAN;		free(s);	return;	}
			if(!strchr(arg[n],'\''))	// this subdata
			{
				long k[3]={-1,-1,-1};
				t = strtok (0, "(,)");	if(t)	k[0] = t[0]==':'?-1:atoi(t);
				t = strtok (0, "(,)");	if(t)	k[1] = t[0]==':'?-1:atoi(t);
				t = strtok (0, "(,)");	if(t)	k[2] = t[0]==':'?-1:atoi(t);
				u = new mglVar;		u->temp = true;
				snprintf(a[n-1].s,2048,"%s.SubData(%d,%d,%d)",v->s,k[0],k[1],k[2]);
				u->d = v->d.SubData(k[0],k[1],k[2]);
			}
			else	// this is column formula
			{
				char *tt = strtok(0,"'");
				t = strtok(0,"'");	t[-1]=0;
				u = new mglVar;		u->temp = true;
				snprintf(a[n-1].s,2048,"%s.Column(\"%s\")",v->s,tt);
				u->d = v->d.Column(tt);
			}
			free(s);
			if(DataList)	u->MoveAfter(DataList);
			else			DataList = u;
			a[n-1].type = 0;	a[n-1].d = &(u->d);
		}
		else if(arg[n][0]=='|')	a[n-1].type = -1;
		else if(arg[n][0]=='\'')
		{
			a[n-1].type = 1;	arg[n][strlen(arg[n])-1] = 0;
			if(strlen(arg[n]+1)>=2048)	arg[n][2048]=0;
			strcpy(a[n-1].s, arg[n]+1);
		}
		else if((v = FindVar(arg[n]))!=0)
		{
			p = strchr(arg[n],'.');	if(p)	{	p[0]=0;	p++;	}
			a[n-1].type = 0;		a[n-1].d = &(v->d);
			strcpy(a[n-1].s, v->s);
		}
		else
		{
			a[n-1].type = 2;
			if(!strcmp(arg[n],"nan"))	a[n-1].v = NAN;
			else if(!strcmp(arg[n],":"))	a[n-1].v = -1;
			else if(!strcmp(arg[n],"pi"))	a[n-1].v = M_PI;
			else if(!strcmp(arg[n],"-pi"))	a[n-1].v = -M_PI;
			else if(!strcmp(arg[n],"on"))	a[n-1].v = 1;
			else if(!strcmp(arg[n],"off"))	a[n-1].v = 0;
			else	a[n-1].v = atof(arg[n]);
		}
		// try to find numbers (momentum) for data
		if(a[n-1].type == 0 && p)
			if(mgls_suffix(p,a[n-1].d,&(a[n-1].v)))
				a[n-1].type = 2;
	}
}
//-----------------------------------------------------------------------------
bool check_for_name(const char *s)
{
	return !isalpha(s[0])||strchr(s,'.')||strchr(s,':')||strchr(s,'(')||strchr(s,')');
}
//-----------------------------------------------------------------------------
// return values: 0 - not found, 1 - OK, 2 - wrong arguments,
//				3 - wrong command, 4 - string too long
int mglParse::PreExec(mglGraph *gr, long k, char **arg, mglArg *a)
{
	long n=0;
	mglVar *v;
	if(!strncmp("read",arg[0],4) || !strcmp("new",arg[0]) || !strcmp("copy",arg[0]) || !strcmp("var",arg[0]) || !strcmp("hist",arg[0]) || !strcmp("max",arg[0]) || !strcmp("min",arg[0]) || !strcmp("sum",arg[0]) || !strcmp("resize",arg[0]) || !strcmp("subdata",arg[0]))
	{
		if(k<3 || check_for_name(arg[1]))	return 2;
		v = AddVar(arg[1]);
		v->d.Create(1,1,1);
		a[0].type = 0;	a[0].d = &(v->d);
		strcpy(a[0].s, arg[1]);
		n = 1+(Exec(gr, arg[0],k-1,a));
	}
	else if(!strcmp("delete",arg[0]))	// parse command "delete"
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
	else if(!strcmp("list",arg[0]))	// parse command "list"
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
int mglParse::Parse(mglGraph *gr, const char *string, long pos)
{
	if(!gr || Stop)	return 0;
	char *str, *s = new char[strlen(string)+1+parlen],*arg[1024],*t;
	str = s;
	strcpy(str,string);
	strtrim_mgl(str);
	// define parameters or start cycle
	if(!memcmp(str,"define",6) && (str[6]==' ' || str[6]=='\t'))
	{
		str += 7;	strtrim_mgl(str);	int res = 1;
		if(*str=='$' && str[1]>='0' && str[1]<='9')
		{	res = 0;	AddParam(str[1]-'0', str+2);	}
		delete []s;		return res;
	}
	if(!memcmp(str,"for",3) && (str[3]==' ' || str[3]=='\t'))
	{
		while(*str && isspace(*str))	str++;
		// if command have format 'for $N ...' then change it to 'for N ...'
		if(*str=='$' && str[1]>='0' && str[1]<='9')
			*str = ' ';
	}	
	// parse arguments (parameters $1, ..., $9)
	if(parlen>0)
	{
		char *sb = new char[strlen(string)+1];
		while((t=strchr(str,'$'))!=0)
		{
			strcpy(sb,t+2);
			t[0]=0;
			long n = t[1]-'0';
			if(n>=0 && n<=9 && par[n])	strcat(str,par[n]);
			strcat(str,sb);
		}
		delete []sb;
	}

	strtrim_mgl(str);
	if(!strcmp(str,"stop"))	{	Stop = true;	delete []s;	return 0;	}

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
		str = str+n+1;	strtrim_mgl(str);
	}
	// try to find last argument
	if(str[0]!=0 && str[0]!='#' && str[0]!=';')	{	arg[k] = str;	k++;	}
	if(k<1) n =0;
	else
	{
		// fill arguments by its values
		mglArg *a = new mglArg[k];
		FillArg(k, arg, a);
		if(for_addr)
		{	fval[for_addr-1].nz = pos;	for_addr = 0;	}
		// execute first special (program-flow-control) commands
		n = FlowExec(gr, arg[0],k-1,a);
		if(n)	{	delete []s;	delete []a;	return n-1;	}
		if(Skip || (if_pos>0 && !(if_stack[if_pos-1]&1)))	{	delete []s;	delete []a;	return 0;	}
		if(!strcmp(arg[0],"call"))
		{
			n = 1;
			if(a[0].type==1)
			{	n=0;	FILE *fp = fopen(a[0].s,"rt");	Execute(gr,fp);	fclose(fp);	}
			delete []s;	delete []a;	return n;
		}
		if(!strcmp(arg[0],"for"))
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
						n=0;	fval[r].Create(m);
						fval[r].Fill(a[1].v, a[2].v);
					}
				}
				if(n==0)
				{
					for(int i=9;i>0;i--)	for_stack[i] = for_stack[i-1];
					for_addr = for_stack[0] = r+1;
					char buf[32];			sprintf(buf,"%g",fval[r].a[0]);
					AddParam(r, buf,true);	fval[r].ny = 1;
				}
			}
			delete []s;	delete []a;	return n;
		}
		// alocate new arrays and execute the command itself
		n = PreExec(gr, k, arg, a);
		if(n>0)	n--;
		else if(!strcmp("setsize",arg[0]) && !AllowSetSize)	n = 2;
		else	n = Exec(gr, arg[0],k-1,a);
		delete []a;
	}
	mglVar *v = DataList, *u;
	while(v)	// remove temporary data arrays
	{
		u = v->next;
		if(v->temp)
		{	if(DataList==v)	DataList = v->next;		delete v;	}
		v = u;
	}
	// restore plot settings if it was changed
	ProcOpt(gr,0);
	delete []s;
	return n;
}
//-----------------------------------------------------------------------------
int mglParse::FlowExec(mglGraph *gr, const char *com, long m, mglArg *a)
{
	int n=-1;
	if(!strcmp(com,"once"))
	{
		if(a[0].type==2)
		{
			n = 0;
			if(a[0].v)
			{	Skip = !Once;	if(out)	sprintf(out,"if(!once)\t{\tonce = true;");	}
			else
			{	Skip = Once = false;	if(out)	sprintf(out,"}");	}
		}
		else n = 1;
	}
	else if(!strcmp(com,"if"))
	{
		int cond;
		if(a[0].type==2)
		{
			n = 0;	cond = (a[0].v!=0)?3:0;
			if(out)	sprintf(out,"if(%g!=0)\t{", a[0].v);
		}
		else if(m>1 && a[0].type==0 && a[1].type==1)
		{
			n = 0;	cond = a[0].d->FindAny(a[1].s)?3:0;
			if(out)	sprintf(out,"if(%s.FindAny(\"%s\"))\t{", a[0].s, a[1].s);
		}
		else n = 1;
		if(n==0)
		{	if_stack[if_pos] = cond;	if_pos = if_pos<19 ? if_pos+1 : 19;	}
	}
	else if(!strcmp(com,"endif"))
	{
		if_pos = if_pos>0 ? if_pos-1 : 0;
		n = 0;	if(out)	sprintf(out,"}");
	}
	else if(!strcmp(com,"else"))
	{
		if(if_pos>0)
		{
			n=0; if_stack[if_pos-1] = (if_stack[if_pos-1]&2)?2:3;
			if(out)	sprintf(out,"}\telse\t{");
		}
		else n = 1;
	}
	else if(!strcmp(com,"elseif"))
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
				sprintf(out,"else if(%g!=0)\t{", a[0].v);
			if(out && m>1 && a[0].type==0 && a[1].type==1)
				sprintf(out,"else if(%s.FindAny(\"%s\"))\t{", a[0].s, a[1].s);
		}
	}
	else if(!strcmp(com,"next"))
	{
		int r = for_stack[0]-1;
		n = for_stack[0] ? 0:1;
		if(for_stack[0])
		{
			if(fval[r].ny<fval[r].nx)
			{
				char buf[32];			sprintf(buf,"%g",fval[r].a[0]);
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
	char str[8192];
	int r, line=0;
	Stop = false;
	while(!feof(fp))
	{
		if(!fgets(str,8192,fp))	break;
		line++;
		if(gr->Message) gr->Message[0] = 0;
		r = Parse(gr,str,ftell(fp));
		if(r<0)	{	fseek(fp,1-r,0);	continue;	}
		if(printf)
		{
			if(gr->Message && gr->Message[0])	printf("%s\n",gr->Message);
			if(r==1)	printf("Wrong argument(s) in line %d (%s)\n", line, str);
			if(r==2)	printf("Wrong command in line %d (%s)\n", line, str);
			if(r==3)	printf("String too long line %d (%s)\n", line, str);
		}
	}
}
//-----------------------------------------------------------------------------
void mglParse::Execute(mglGraph *gr, int n, const char **text, void (*error)(int line, int kind))
{
	if(gr==0 || n<1 || text==0)	return;
	long i, r;
	for(i=0;i<n;i++)
	{
		r = Parse(gr,text[i],n);
		if(r<0)	{	i = 1-r;	continue;	}
		if(error)
		{
			if(r>0)	error(i, r);
			if(gr->Message && gr->Message[0])	error(i,0);
		}
	}
}
//-----------------------------------------------------------------------------
int mglParse::Export(char cpp_out[1024], mglGraph *gr, const char *str)
{
	out = cpp_out;
	int res = Parse(gr, str);
	out = 0;
	return res;
}
//-----------------------------------------------------------------------------
void mglParse::ProcOpt(mglGraph *gr, char *str)
{
	if(str==0)
	{
		if(opt[0])	{	gr->Min.x = val[0];	gr->Max.x = val[1];	gr->RecalcBorder();	}
		if(opt[1])	{	gr->Min.y = val[2];	gr->Max.y = val[3];	gr->RecalcBorder();	}
		if(opt[2])	{	gr->Min.z = val[4];	gr->Max.z = val[5];	gr->RecalcBorder();	}
		if(opt[3])	{	gr->Cmin = val[6];	gr->Cmax = val[7];	}
		if(opt[4])	gr->Ambient(val[8]);
		if(opt[5])	gr->Cut = val[10]!=0;
		if(opt[6])	gr->AlphaDef = val[12];
		if(opt[7])	gr->MeshNum  = int(val[14]);
		if(opt[8])	gr->FontSize = val[16];
		if(opt[9])	gr->MarkSize = val[17];
		if(opt[10])	gr->AddLegend(leg, gr->last_style);
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
		char *s = str,*a,*b;
		long n;
		while((s=strchr(s,';'))!=0)
		{
			s++;	strtrim_mgl(s);
			n=mglFindArg(s);
			if(n<1)	return;
			s[n]=0;		a=s;	s=s+n+1;	strtrim_mgl(a);
			float ff = atof(s);
			if(!strcmp(a+1,"range"))
			{
				n=mglFindArg(s);	if(n<1)	return;
				s[n]=0;		b=s;	s=s+n+1;	strtrim_mgl(b);
				if(a[0]=='x')
				{	opt[0]=true;	gr->Min.x = atof(b);	gr->Max.x = atof(s);}
				else if(a[0]=='y')
				{	opt[1]=true;	gr->Min.y = atof(b);	gr->Max.y = atof(s);}
				else if(a[0]=='z')
				{	opt[2]=true;	gr->Min.z = atof(b);	gr->Max.z = atof(s);}
				else if(a[0]=='c')
				{	opt[3]=true;	gr->Cmin = atof(b);		gr->Cmax = atof(s);	}
				//gr->RecalcBorder();
			}
			else if(!strcmp(a,"cut"))
			{	opt[5]=true;	gr->Cut = (ff!=0 || !strncmp(s,"on",2));	}
			else if(!strcmp(a,"meshnum"))
			{	opt[7]=true;	gr->MeshNum = int(ff);	}
			else if(!strcmp(a,"alphadef"))
			{	opt[6]=true;	gr->AlphaDef = ff;	}
			else if(!strcmp(a,"alpha"))
			{	opt[6]=true;	gr->AlphaDef = ff;	}
			else if(!strcmp(a,"fontsize"))
			{	opt[8]=true;	gr->FontSize = ff>0 ? ff : -ff*gr->FontSize;	}
			else if(!strcmp(a,"ambient"))
			{	opt[4]=true;	gr->Ambient(ff);	}
			else if(!strcmp(a,"marksize"))
			{	opt[9]=true;	gr->MarkSize = ff/50;	}
			else if(!strcmp(a,"legend"))
			{	opt[10]=true;	strcpy(leg,s);	}
		}
	}
}
//-----------------------------------------------------------------------------
