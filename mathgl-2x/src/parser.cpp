/***************************************************************************
 * parse.cpp is part of Math Graphic Library
 * Copyright (C) 2007-2012 Alexey Balakin <mathgl.abalakin@gmail.ru>       *
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
#include <ctype.h>
#include "mgl2/parser.h"
#include "mgl2/canvas_cf.h"
//-----------------------------------------------------------------------------
#ifdef WIN32
#include <io.h>
wchar_t *wcstokw32(wchar_t *wcs, const wchar_t *delim)	{	return wcstok(wcs,delim);	}
#define wcstok(a,b,c) wcstokw32(a,b)
#else
#include <unistd.h>
#endif
MGL_EXPORT void (*mgl_ask_func)(const wchar_t *, wchar_t *)=0;
void MGL_EXPORT mgl_ask_gets(const wchar_t *quest, wchar_t *res)
{	printf("%ls\n",quest);	if(!fgetws(res,1024,stdin))	*res=0;	}
//-----------------------------------------------------------------------------
mglFunc::mglFunc(const mglFunc &f)
{	pos=f.pos;	narg=f.narg;	wcsncpy(func,f.func,64);	}
mglFunc::mglFunc(long p, const wchar_t *f)
{
	pos = p;
	register size_t i;
	for(i=0;(isalnum(f[i]) || f[i]=='_') && i<63;i++)	func[i]=f[i];
	func[i]=0;	narg = wcstol(f+i+1,0,0);
	if(narg<0 || narg>9)	narg=0;
}
//-----------------------------------------------------------------------------
size_t mglParser::GetParLen()
{
	register size_t i,s=0,t;
	for(i=0;i<40;i++)
	{	t = par[i].length();	s = s<t?t:s;	}
	return s;
}
//-----------------------------------------------------------------------------
size_t mglParser::GetParLen(const wchar_t *str)
{
	register size_t nump=0, is;
	for(is=0;is<wcslen(str);is++)	if(str[is]=='$')	nump++;
	return nump*GetParLen();
}
//-----------------------------------------------------------------------------
long mglParser::IsFunc(const wchar_t *name, int *na)
{
	register size_t i;
	for(i=0;i<func.size();i++)
	{
		const mglFunc &f = func[i];
		if(!wcscmp(f.func, name))
		{	if(na)	*na=f.narg;	return f.pos;	}
	}
	return 0;
}
//-----------------------------------------------------------------------------
void mglParser::ScanFunc(const wchar_t *line)
{
	static long num=0;
	if(!line)
	{	func.clear();	num=0;	return;	}
	num++;
	if(wcsncmp(line,L"func",4) || line[4]>' ')	return;
	register long i;
	for(i=4;line[i]<=' ' || line[i]=='\'';i++);
	func.push_back(mglFunc(num-1, line+i));
}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT wchar_t *mgl_str_copy(const char *s)
{
	wchar_t *str = new wchar_t[strlen(s)+1];
	register long i;
	for(i=0;i<int(strlen(s));i++)	str[i] = s[i];
	str[i] = 0;
	return str;
}
//-----------------------------------------------------------------------------
int MGL_NO_EXPORT mgl_cmd_cmp(const void *a, const void *b)
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
	if(!AllowFileIO && ( !strncmp(com,"read",4) || !strncmp(com,"save",4) || !strcmp(com,"fgets") || !strcmp(com,"import") || !strcmp(com,"export") ))
		return 0;
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
	size_t s = 15<wcslen(com)?15:wcslen(com);
	char cmd[16];	wcstombs(cmd,com,s+1);	cmd[s]=0;
	return FindCommand(cmd);
}
//-----------------------------------------------------------------------------
// return values : 0 -- OK, 1 -- wrong arguments, 2 -- wrong command, 3 -- unclosed string
int mglParser::Exec(mglGraph *gr, const wchar_t *com, long n, mglArg *a, const wchar_t *var, const wchar_t *opt)
{
	int i;
	const char *id="dsn";
	std::string k;
	for(i=0;i<n;i++)
	{
		k += id[a[i].type];
		a[i].s.assign(a[i].w.begin(),a[i].w.end());
	}
	mglCommand *rts=FindCommand(com);
	if(!rts || rts->type==6)	return 2;
	if(rts->type == 4)
	{
		if(n<1 || check_for_name(var))	return 2;
		mglVar *v = AddVar(var);
		v->Create(1,1,1);
		a[0].type = 0;	a[0].d = v;
		a[0].w = var;	k[0] = 'd';
	}
	char *o=0;
	if(opt && *opt)	// TODO: parse arguments of options
	{
		long len = wcslen(opt);
		o = new char[len+1];
		for(i=0;i<len+1;i++)	o[i]=opt[i];
	}
	int res=rts->exec(gr, n, a, k.c_str(), o);
	if(o)	delete []o;
	return res;
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
	DataList=0;	NumList=0;
//	wchar_t *par[40];	///< Parameter for substituting instead of $1, ..., $9
	InUse = 1;
	Skip=Stop=for_br=false;
	memset(for_stack,0,40*sizeof(int));
	memset(if_stack,0,40*sizeof(int));
	memset(if_for,0,40*sizeof(int));
	if_pos=for_addr=0;
	for(long i=0;i<40;i++)	par[i]=L"";

	Cmd = mgls_base_cmd;
	AllowSetSize=setsize;	AllowFileIO=true;
	Once = true;
	op1 = new wchar_t[4096];	op2 = new wchar_t[4096];
	fval = new mglData[40];
}
//-----------------------------------------------------------------------------
mglParser::~mglParser()
{
	DeleteAll();
	delete []op1;	delete []op2;	delete []fval;
	if(Cmd && Cmd!=mgls_base_cmd)	delete []Cmd;
}
//-----------------------------------------------------------------------------
void mglParser::DeleteAll()
{
	if(DataList)
	{
		while(DataList->next)	delete DataList->next;
		delete DataList;	DataList = 0;
	}
	if(NumList)
	{
		while(NumList->next)	delete NumList->next;
		delete NumList;		NumList = 0;
	}
}
//-----------------------------------------------------------------------------
void mglParser::AddParam(int n, const char *str)
{
	unsigned s = mbstowcs(0,str,0)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	AddParam(n,wcs);
	delete []wcs;
}
//-----------------------------------------------------------------------------
int mglParser::Parse(mglGraph *gr, const char *str, long pos)
{
	size_t s = mbstowcs(0,str,0)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	int r = Parse(gr,wcs,pos);
	delete []wcs;	return r;
}
//-----------------------------------------------------------------------------
mglVar *mglParser::AddVar(const char *str)
{
	size_t s = mbstowcs(0,str,0)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	mglVar *v = AddVar(wcs);
	delete []wcs;
	return v;
}
//-----------------------------------------------------------------------------
mglVar *mglParser::FindVar(const char *str)
{
	if(!str || *str==0) 	return DataList;
	size_t s = mbstowcs(0,str,0)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	mglVar *v = FindVar(wcs);
	delete []wcs;
	return v;
}
//-----------------------------------------------------------------------------
mglNum *mglParser::AddNum(const char *str)
{
	size_t s = mbstowcs(0,str,0)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	mglNum *v = AddNum(wcs);
	delete []wcs;
	return v;
}
//-----------------------------------------------------------------------------
mglNum *mglParser::FindNum(const char *str)
{
	size_t s = mbstowcs(0,str,0)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	mglNum *v = FindNum(wcs);
	delete []wcs;
	return v;
}
//-----------------------------------------------------------------------------
void mglParser::AddParam(int n, const wchar_t *str)
{
//	if(str && n>=0 && n<40 && !wcschr(str,'$'))	par[n] = str;
	if(str && n>=0 && n<40)	par[n] = str;
}
//-----------------------------------------------------------------------------
mglVar *mglParser::FindVar(const wchar_t *name)
{
	if(!name || *name==0)	return DataList;
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
		if(l%2==0 && k==0)
		{
			if(str[i]=='#' || str[i]==';')	return -i;
			if(str[i]<=' ')	return i;
		}
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
			a[n-1].type = 1;
			wchar_t *w=arg[n],ch,*f,buf[32];
			long i,l,ll=wcslen(w);
			for(l=i=1;i<ll;i++)
			{
				if(w[i]=='\'')
				{
					if(i==ll-1)	continue;
					i++;	f = w+i;
					for(;i<ll && w[i]!='\'';i++);
					ch=w[i];	w[i]=0;
					if(*f==',')	f++;
					if(*f==0)	continue;
					mglData d = mglFormulaCalc(f, this);	w[i]=ch;
					mglprintf(buf,32,L"%g",d.a[0]);	a[n-1].w += buf;
				}
				else	a[n-1].w += w[i];
			}
		}
		else if(arg[n][0]=='{')
		{	// this is temp data
			arg[n][wcslen(arg[n])-1] = 0;
			u=new mglVar;	u->temp=true;
			a[n-1].w = L"/*"+std::wstring(arg[n]+1)+L"*/";
			if(DataList)	u->MoveAfter(DataList);
			else			DataList = u;
			a[n-1].type = 0;	a[n-1].d = u;
			ParseDat(gr, arg[n]+1, *u);
		}
		else if((v = FindVar(arg[n]))!=0)	// have to find normal variables (for data creation)
		{	a[n-1].type=0;	a[n-1].d=v;	a[n-1].w=v->s;	}
		else if((f = FindNum(arg[n]))!=0)	// have to find normal variables (for data creation)
		{	a[n-1].type=2;	a[n-1].d=0;	a[n-1].v=f->d;	a[n-1].w = f->s;	}
		else
		{	// parse all numbers and formulas by unified way
			mglData d = mglFormulaCalc(arg[n], this);
			if(d.nx*d.ny*d.nz==1)
			{	a[n-1].type = 2;	a[n-1].v = d.a[0];	}
			else
			{
				u=new mglVar;	u->temp=true;	u->Set(d);
				a[n-1].w = L"/*"+std::wstring(arg[n])+L"*/";
				if(DataList)	u->MoveAfter(DataList);
				else			DataList = u;
				a[n-1].type = 0;	a[n-1].d = u;
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
		v = AddVar(arg[1]);
		if(t==1)	nx = j>nx ? j:nx;
		if(t==1)	// list of numeric values
		{
			v->Create(nx,ny);
			j=t=0;
			for(i=2;i<k;i++)
			{
				if(arg[i][0]=='|')	{	t++;	j=0;	}
				else
				{	v->a[j+nx*t] = a[i-1].v;	j++;	}
			}
		}
		if(t==2)	// list of data
		{
			mglData *b = a[1].d;
			long nn = 0;
			if(b->nz>1)	return 2;
			if(b->ny>1)
			{
				v->Create(b->nx, b->ny, nx);
				nn = b->nx*b->ny;
				for(i=2,j=0;i<k;i++)
				{
					b = a[i-1].d;
					if(b==0 || nn!=b->nx*b->ny)	continue;
					memcpy(v->a+j*nn,b->a,nn*(b->nz)*sizeof(mreal));
					j+=b->nz;
				}
			}
			else
			{
				v->Create(b->nx, nx);
				nn = b->nx;
				for(i=2,j=0;i<k;i++)
				{
					b = a[i-1].d;
					if(b==0 || nn!=b->nx)	continue;
					memcpy(v->a+j*nn,b->a,nn*(b->ny)*sizeof(mreal));
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
	wchar_t *sb = new wchar_t[wcslen(string)+1], *t;
	if(def)	str = str+10;
	register long n;
	while((t=wcschr(str,'$'))!=0)
	{
		wcscpy(sb,t+2);	t[0]=0;
		n = t[1]-'0';	if(n>=0 && n<=9)	wcscat(str,par[n].c_str());
		n = t[1]-'a';	if(n>=0 && n<='z'-'a')	wcscat(str,par[n+10].c_str());
		if(t[1]=='$')	wcscat(str,L"\xffff");
		wcscat(str,sb);
	}
	delete []sb;
	while((t=wcschr(str,L'\xffff'))!=0)	*t='$';
}
//-----------------------------------------------------------------------------
// return values: 0 - OK, 1 - wrong arguments, 2 - wrong command, 3 - string too long, 4 -- unclosed string
int mglParser::Parse(mglGraph *gr, const wchar_t *string, long pos)
{
	if(Stop)	return 0;
	size_t lstr = wcslen(string)+2+GetParLen(string);
	wchar_t *str, *arg[1024],*t;
	wchar_t *s = new wchar_t[lstr];
	memset(s,0,lstr*sizeof(wchar_t));
	wcscpy(s,string);	mgl_wcstrim(s);	str = s;
	long n,k=0,m=0,mm=0;
	// try parse ':' -- several commands in line
	for(n=0;n<long(wcslen(str));n++)
	{
		if(str[n]=='\'' && (n==0 || str[n-1]!='\\'))	k++;
		if(k%2)	continue;
		if(str[n]=='(')	m++;
		if(str[n]==')')	m--;
		if(str[n]=='{')	mm++;
		if(str[n]=='}')	mm--;
		if(str[n]=='#')	break;
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
				AddParam(nn, str);	delete []s;	return 0;
			}
			else	str -= 7;
		}
		if(!wcsncmp(str+3,L"num",3))
		{
			str += 7;	mgl_wcstrim(str);	int res = 1;
			int nn = str[1]<='9' ? str[1]-'0' : (str[1]>='a' ? str[1]-'a'+10:-1);
			if(*str=='$' && nn>=0 && nn<='z'-'a'+10)
			{
				res = 0;	str +=2;	mgl_wcstrim(str);
				mreal d = mglFormulaCalc(str, this).a[0];
				char *buf=new char[128];
				snprintf(buf,128,"%g",d);
				AddParam(nn, buf);	delete []buf;
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
				mreal d=mglFormulaCalc(str, this).a[0];
				wchar_t buf[2]={0,0};	buf[0] = wchar_t(d);
				AddParam(nn, buf);
			}
			delete []s;	return res;
		}
	}
	if(!skip() && !wcsncmp(str,L"ask",3) && (str[3]==' ' || str[3]=='\t'))
	{
		PutArg(string,str,true);
		str += 4;	mgl_wcstrim(str);//	int res = 1;
		int nn = str[1]<='9' ? str[1]-'0' : (str[1]>='a' ? str[1]-'a'+10:-1);
		if(*str=='$' && nn>=0 && nn<='z'-'a'+10)
		{
			static wchar_t res[1024];
			str +=2;	mgl_wcstrim(str);
			if(*str=='\'')	{	str++;	str[wcslen(str)-1]=0;	}
			if(mgl_ask_func)
			{	mgl_ask_func(str,res);	if(*res)	AddParam(nn, res);	}
			delete []s;	return mgl_ask_func?0:1;
		}
		else	return 1;
	}
	wcscpy(str,string);			mgl_wcstrim(str);
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
				v->d = mglFormulaCalc(arg[2],this).a[0];
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
					snprintf(buf,64,"Bad arguments for %ls: %ld instead of %d\n",
							a[0].w.c_str(),k-2,na);
					gr->SetWarn(-1,buf);	n = 1;
				}
				else if(n)
				{
					mglFnStack fn;			fn.pos = pos;
					for(int i=0;i<10;i++)	{	fn.par[i] = par[i];	par[i]=L"";	}
					for(int i=1;i<k-1;i++)	AddParam(i,arg[i+1]);
					fn_stack.push_back(fn);	n--;
				}
				else if(AllowFileIO)	// disable external scripts if AllowFileIO=false
				{
					FILE *fp = fopen(a[0].s.c_str(),"rt");
					if(fp)
					{
						register int i;
						mglParser *prs = new mglParser(AllowSetSize);
						prs->DataList=DataList;	prs->NumList=NumList;	prs->Cmd=Cmd;
						for(i=10;i<30;i++)	prs->AddParam(i,par[i].c_str());
						prs->Execute(gr,fp);
						for(i=10;i<30;i++)	AddParam(i,prs->par[i].c_str());
						DataList=prs->DataList;	prs->DataList=0;
						NumList =prs->NumList;	prs->NumList=0;
						prs->Cmd=0;	delete prs;	fclose(fp);
					}
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
					AddParam(r, buf);	fval[r].ny = 1;
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
	wchar_t *str, *s = new wchar_t[wcslen(string)+1+GetParLen(string)],*arg[32];
	str = s;
	wcscpy(str,string);	mgl_wcstrim(str);
	long n,k=0;
	for(k=0;k<32;k++)	// parse string to substrings (by spaces)
	{
		n = mglFindArg(str);
		if(n<1)	{	if(n<0)	str[-n]=0;	break;	}
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
		int i;
		std::string kk;
		const char *id="dsn";
		for(i=0;i<k;i++)
		{
			kk += id[a[i].type];
			a[i].s.assign(a[i].w.begin(),a[i].w.end());
		}
		mglCommand *rts=FindCommand(arg[0]);
		if(!rts || rts->type!=4)	n = 2;
		else n = rts->exec(gr, k, a, kk.c_str(), 0);
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
			if(a[0].v)	Skip = !Once;
			else	Skip = Once = false;
		}
		else n = 1;
	}
	else if(!Skip && !wcscmp(com,L"if"))
	{
		int cond;
		if(a[0].type==2)
		{	n = 0;	cond = (a[0].v!=0)?3:0;	}
		else if(a[0].type==0)
		{
			n = 0;	a[1].s.assign(a[1].w.begin(),a[1].w.end());
			cond = a[0].d->FindAny((m>1 && a[1].type==1) ? a[1].s.c_str():"u")?3:0;
		}
		else n = 1;
		if(n==0)
		{	if_stack[if_pos] = cond;	if_pos = if_pos<39 ? if_pos+1 : 39;	}
	}
	else if(!Skip && !wcscmp(com,L"endif"))
	{	if_pos = if_pos>0 ? if_pos-1 : 0;	n = 0;	}
	else if(!Skip && !wcscmp(com,L"else"))
	{
		if(if_pos>0)
		{	n=0; if_stack[if_pos-1] = (if_stack[if_pos-1]&2)?2:3;	}
		else n = 1;
	}
	else if(!Skip && !wcscmp(com,L"elseif"))
	{
		int cond;
		if(if_pos<1 || m<1)	n = 1;
		else if(if_stack[if_pos-1]&2)	{	n = 0;	cond = 2;	}
		else if(a[0].type==2)
		{	n = 0;	cond = (a[0].v!=0)?3:0;	}
		else if(a[0].type==0)
		{
			n = 0;	a[1].s.assign(a[1].w.begin(),a[1].w.end());
			cond = a[0].d->FindAny((m>1 && a[1].type==1) ? a[1].s.c_str():"u")?3:0;
		}
		else n = 1;
		if(n==0)	if_stack[if_pos-1] = cond;
	}
	else if(!ifskip() && !Skip && !wcscmp(com,L"break"))
	{
		if(if_pos==if_for[0])	if_pos = if_pos>0 ? if_pos-1 : 0;
	}
	else if(!skip() && !wcscmp(com, L"return"))	// parse command "delete"
	{
		if(fn_stack.size()<1)	return 2;
		const mglFnStack &fn=fn_stack.back();
		for(int i=0;i<10;i++)	par[i]=fn.par[i];
		n = -fn.pos-1;	fn_stack.pop_back();
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
				AddParam(r, buf);	fval[r].ny += 1;
				n = -fval[r].nz-1;
			}
			else
			{
				for(int i=0;i<39;i++)
				{	for_stack[i]=for_stack[i+1];	if_for[i]=if_for[i+1];	}
				for_stack[39] = 0;	for_br=false;
			}
		}
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
				AddParam(r, buf);	fval[r].ny += 1;
				n = -fval[r].nz-1;
			}
			else	for_br = true;
		}
	}
	return n+1;
}
//-----------------------------------------------------------------------------
#include <string>
void mglParser::Execute(mglGraph *gr, FILE *fp, bool print)
{
	if(gr==0 || fp==0)	return;
	std::wstring str;
	wchar_t ch;
	while((ch=fgetwc(fp))!=WEOF)	str.push_back(ch);
//	while(!feof(fp))	str.push_back(fgetwc(fp));
	Execute(gr,str.c_str());
	if(print)	printf("%s\n",gr->Message());
}
//-----------------------------------------------------------------------------
void mglParser::Execute(mglGraph *gr, int n, const wchar_t **text)
{
	if(n<1 || text==0)	return;
	long i, r;
	char buf[64];
	for_br=Skip=false;	if_pos=0;	ScanFunc(0);	fn_stack.clear();
	for(i=0;i<n;i++)	ScanFunc(text[i]);
	for(i=0;i<n;i++)
	{
		gr->SetWarn(-1, "");
		gr->SetObjId(i+1);
		r = Parse(gr,text[i],i+1);
		if(r<0)	{	i = -r-2;	continue;	}
		if(r==1)		snprintf(buf,64,"\nWrong argument(s) in line %ld\n", i+1);
		else if(r==2)	snprintf(buf,64,"\nWrong command in line %ld\n", i+1);
		else if(r==3)	snprintf(buf,64,"\nString too long in line %ld\n", i+1);
		else if(r==4)	snprintf(buf,64,"\nUnbalanced ' in line %ld\n", i+1);
		else if(gr->GetWarn()>0)	snprintf(buf,64," in line %ld\n", i+1);
		else *buf=0;
		if(*buf)	gr->SetWarn(-2,buf);
	}
}
//-----------------------------------------------------------------------------
void mglParser::Execute(mglGraph *gr, const wchar_t *text)
{
	size_t s = wcslen(text)+1;
	wchar_t *wcs = new wchar_t[s];
	const wchar_t **str;
	register size_t i, n=1;
	for(i=0;i<s;i++)	if(text[i]=='\n')	n++;
	str = (const wchar_t **)malloc(n*sizeof(wchar_t *));
	memcpy(wcs, text, s*sizeof(wchar_t));
	str[0] = wcs;	n=1;
	long next=0;
	Stop = false;
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
	Execute(gr, n, str);
	delete []wcs;	free(str);
}
//-----------------------------------------------------------------------------
void mglParser::Execute(mglGraph *gr, const char *text)
{
	size_t s = mbstowcs(0,text,0)+1;
	wchar_t *wcs = new wchar_t[s];
	memset(wcs,0,s*sizeof(wchar_t));
	mbstowcs(wcs,text,s);
	Execute(gr, wcs);
	delete []wcs;
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
HMPR MGL_EXPORT mgl_create_parser()		{	return new mglParser;	}
void MGL_EXPORT mgl_delete_parser(HMPR p)	{	delete p;	}
void MGL_EXPORT mgl_parser_add_param(HMPR p, int id, const char *str)			{	p->AddParam(id,str);	}
void MGL_EXPORT mgl_parser_add_paramw(HMPR p, int id, const wchar_t *str)		{	p->AddParam(id,str);	}
HMDT MGL_EXPORT mgl_parser_add_var(HMPR p, const char *name)	{	mglVar *v=p->AddVar(name);	return v;	}
HMDT MGL_EXPORT mgl_parser_find_var(HMPR p, const char *name)	{	mglVar *v=p->FindVar(name);	return v;	}
void MGL_EXPORT mgl_parser_del_var(HMPR p, const char *name)	{	p->DeleteVar(name);	}
HMDT MGL_EXPORT mgl_parser_add_varw(HMPR p, const wchar_t *name)	{	mglVar *v=p->AddVar(name);	return v;	}
HMDT MGL_EXPORT mgl_parser_find_varw(HMPR p, const wchar_t *name)	{	mglVar *v=p->FindVar(name);	return v;	}
void MGL_EXPORT mgl_parser_del_varw(HMPR p, const wchar_t *name)	{	p->DeleteVar(name);	}
int MGL_EXPORT mgl_parse_line(HMGL gr, HMPR p, const char *str, int pos)
{	return p->Parse(gr, str, pos);	}
int MGL_EXPORT mgl_parse_linew(HMGL gr, HMPR p, const wchar_t *str, int pos)
{	return p->Parse(gr, str, pos);	}
void MGL_EXPORT mgl_parse_text(HMGL gr, HMPR p, const char *str)
{	p->Execute(gr, str);	}
void MGL_EXPORT mgl_parse_textw(HMGL gr, HMPR p, const wchar_t *str)
{	p->Execute(gr, str);	}
void MGL_EXPORT mgl_parse_file(HMGL gr, HMPR p, FILE *fp, int print)
{	p->Execute(gr,fp,print);	}
void MGL_EXPORT mgl_parser_restore_once(HMPR p)	{	p->RestoreOnce();	}
void MGL_EXPORT mgl_parser_stop(HMPR p)	{	p->Stop = true;		}
void MGL_EXPORT mgl_parser_allow_setsize(HMPR p, int a)	{	p->AllowSetSize= a;	}
void MGL_EXPORT mgl_parser_allow_file_io(HMPR p, int a)	{	p->AllowFileIO = a;	}
//-----------------------------------------------------------------------------
#define _PR_	((mglParser *)(*p))
uintptr_t MGL_EXPORT mgl_create_parser_()	{	return uintptr_t(new mglParser);	}
void MGL_EXPORT mgl_delete_parser_(uintptr_t* p)	{	delete _PR_;	}
void MGL_EXPORT mgl_parser_add_param_(uintptr_t* p, int *id, const char *str, int l)
{	char *s=new char[l+1];		memcpy(s,str,l);	s[l]=0;
	_PR_->AddParam(*id, s);		delete []s;	}
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
uintptr_t MGL_EXPORT mgl_parser_add_var_(uintptr_t* p, const char *name, int l)
{	char *s=new char[l+1];		memcpy(s,name,l);	s[l]=0;
	mglVar *v=_PR_->AddVar(s);	delete []s;	return uintptr_t(v);	}
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
uintptr_t MGL_EXPORT mgl_parser_find_var_(uintptr_t* p, const char *name, int l)
{	char *s=new char[l+1];		memcpy(s,name,l);	s[l]=0;
	mglVar *v=_PR_->FindVar(s);	delete []s;	return uintptr_t(v);	}
void MGL_EXPORT mgl_del_var_(uintptr_t* p, const char *name, int l)
{	char *s=new char[l+1];		memcpy(s,name,l);	s[l]=0;
	_PR_->DeleteVar(s);	delete []s;	}
int MGL_EXPORT mgl_parse_line_(uintptr_t* gr, uintptr_t* p, const char *str, int *pos, int l)
{	char *s=new char[l+1];		memcpy(s,str,l);	s[l]=0;
	int r = _PR_->Parse(_GR_, s, *pos);	delete []s;	return r;	}
void MGL_EXPORT mgl_parse_text_(uintptr_t* gr, uintptr_t* p, const char *str, int l)
{	char *s=new char[l+1];		memcpy(s,str,l);	s[l]=0;
	_PR_->Execute(_GR_, s);	delete []s;	}
void MGL_EXPORT mgl_parser_restore_once_(uintptr_t* p)	{	_PR_->RestoreOnce();	}
void MGL_EXPORT mgl_parser_allow_setsize_(uintptr_t* p, int *a)	{	_PR_->AllowSetSize= *a;	}
void MGL_EXPORT mgl_parser_allow_file_io_(uintptr_t* p, int *a)	{	_PR_->AllowFileIO = *a;	}
void MGL_EXPORT mgl_parser_stop_(uintptr_t* p)	{	_PR_->Stop = true;	}
//-----------------------------------------------------------------------------
long MGL_EXPORT mgl_use_parser(HMPR pr, int inc)
{	pr->InUse+=inc;	return pr->InUse;	}
long MGL_EXPORT mgl_use_parser_(uintptr_t *p, int *inc)
{	_PR_->InUse+=*inc;	return _PR_->InUse;	}
//---------------------------------------------------------------------------
int MGL_EXPORT mgl_parser_cmd_type(HMPR pr, const char *name)
{
	mglCommand *cmd = pr->FindCommand(name);
	return cmd ? cmd->type + 1 : 0;
}
int MGL_EXPORT mgl_parser_cmd_type_(uintptr_t* p, const char *str, int l)
{	char *s=new char[l+1];	memcpy(s,str,l);	s[l]=0;
	l = mgl_parser_cmd_type(_PR_, s);	delete []s;	return l;	}
//---------------------------------------------------------------------------
MGL_EXPORT const char *mgl_parser_cmd_desc(HMPR pr, const char *name)
{
	mglCommand *cmd = pr->FindCommand(name);
	return cmd ? cmd->desc : 0;
}
MGL_EXPORT const char *mgl_parser_cmd_frmt(HMPR pr, const char *name)
{
	mglCommand *cmd = pr->FindCommand(name);
	return cmd ? cmd->form : 0;
}
//---------------------------------------------------------------------------
MGL_EXPORT const char *mgl_parser_cmd_name(HMPR pr, long id)
{	return (id<mgl_parser_cmd_num(pr) && id>=0) ? pr->Cmd[id].name:"";	}
long MGL_EXPORT mgl_parser_cmd_num(HMPR pr)
{	register long i=0;	while(pr->Cmd[i].name[0])	i++; 	return i;	}
//---------------------------------------------------------------------------
HMDT MGL_EXPORT mgl_parser_calc(HMPR pr, const char *formula)
{
	size_t s = mbstowcs(0,formula,0)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,formula,s);
	HMDT d = mgl_parser_calcw(pr,wcs);
	delete []wcs;	return d;	
}
HMDT MGL_EXPORT mgl_parser_calcw(HMPR pr, const wchar_t *formula)
{	mglData *d = new mglData(mglFormulaCalc(formula,pr)); 	return d;	}
uintptr_t MGL_EXPORT mgl_parser_calc_(uintptr_t *p, const char *str,int l)
{	char *s=new char[l+1];	memcpy(s,str,l);	s[l]=0;
	uintptr_t d = (uintptr_t)mgl_parser_calc(_PR_, s);	delete []s;	return d;	}
//---------------------------------------------------------------------------
void MGL_EXPORT mgl_parser_del_all(HMPR p)	{	p->DeleteAll();	}
void MGL_EXPORT mgl_parser_del_all_(uintptr_t *p)	{	_PR_->DeleteAll();	}
//---------------------------------------------------------------------------
