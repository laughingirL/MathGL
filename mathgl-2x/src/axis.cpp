/***************************************************************************
 * axis.cpp is part of Math Graphic Library
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
#include <time.h>
#include <wchar.h>
#include <ctype.h>
#include <stdlib.h>
#include "mgl/data.h"
#include "mgl/canvas.h"
//-----------------------------------------------------------------------------
#define islog(a, b) (((a)>0 && (b)>10*(a)) || ((b)<0 && (a)<10*(b)))
#define sign(a)	((a)<0 ? -1:1)

//-----------------------------------------------------------------------------
wchar_t *mgl_wcsdup(const wchar_t *s)
{
	wchar_t *r = (wchar_t *)malloc((wcslen(s)+1)*sizeof(wchar_t));
	memcpy(r,s,(wcslen(s)+1)*sizeof(wchar_t));
	return r;
}
//-----------------------------------------------------------------------------
void mgl_wcstrim(wchar_t *str)
{
	wchar_t *c = mgl_wcsdup(str);
	unsigned long n=wcslen(str);
	long k;
	for(k=0;k<long(wcslen(str));k++)		if(str[k]>' ')	break;
	wcscpy(c,&(str[k]));
	n = wcslen(c);
	for(k=n-1;k>=0;k--)	if(c[k]>' ')	break;
	c[k+1] = 0;
	wcscpy(str,c);	free(c);
}
//-----------------------------------------------------------------------------
//		Ticks setup
//-----------------------------------------------------------------------------
void mglCanvas::SetAxisStl(const char *stl, const char *tck, const char *sub)
{
	if(!stl || !(*stl))		strcpy(AxisStl,"k");
	else if(strlen(stl)<32)	strcpy(AxisStl,stl);
	if(!tck || !(*tck))		strcpy(TickStl,AxisStl);
	else if(strlen(tck)<32)	strcpy(TickStl,tck);
	if(!sub || !(*sub))		strcpy(SubTStl,AxisStl);
	else if(strlen(sub)<32)	strcpy(SubTStl,sub);
}
//-----------------------------------------------------------------------------
void mglCanvas::SetTickLen(float tlen, float stt)
{	TickLen = tlen?tlen:0.02;	st_t=stt>0?stt:1;	}
//-----------------------------------------------------------------------------
void mglCanvas::SetTicks(char dir, float d, int ns, float org)
{
	if(dir=='x')
	{	ax.d=d;	ax.f=0;	ax.ns=ns;	ax.o=org;	ax.t[0]=0;	ax.txt.clear();	}
	else if(dir=='y')
	{	ay.d=d;	ay.f=0;	ay.ns=ns;	ay.o=org;	ay.t[0]=0;	ay.txt.clear();	}
	else if(dir=='z')
	{	az.d=d;	az.f=0;	az.ns=ns;	az.o=org;	az.t[0]=0;	az.txt.clear();	}
	else if(dir=='c' || dir=='a')
	{	ac.d=d;	ac.f=0;	ac.ns=ns;	ac.o=org;	ac.t[0]=0;	ac.txt.clear();	}
}
//-----------------------------------------------------------------------------
void mglCanvas::SetTicksVal(char dir, HCDT v, const wchar_t *lbl, bool add)
{
	mglAxis &aa=ax;
	bool ff;
	if(dir=='c' || dir=='a')	{	aa = ac;	ff=fa;	}
	else if(dir=='x')	{	aa = ax;	ff=fx;	}
	else if(dir=='y')	{	aa = ay;	ff=fy;	}
	else if(dir=='z')	{	aa = az;	ff=fz;	}
	else return;

	aa.txt.clear();
	if(add)	{	UpdateAxis();	AdjustTicks(aa,ff);	}
	if(!v || !lbl || !lbl[0])	{	aa.f = 0;	return;	}
	aa.f = 2;	aa.ns=0;	aa.ds=0;
	register long i,j,l=0,n=v->GetNx();
	for(i=j=l=0;i<n && lbl[j];j++)
	{
		if(lbl[j]=='\n')
		{
			aa.AddLabel(std::wstring(lbl+l,j-l),v->v(i));
			i++;	l=j+1;
		}
		if(j>1 && lbl[j]=='n' && lbl[j-1]=='\\')
		{
			aa.AddLabel(std::wstring(lbl+l,j-l-1),v->v(i));
			i++;	l=j+1;
		}
	}
	if(i<n && lbl[l])	aa.AddLabel(lbl+l,v->v(i));
}
//-----------------------------------------------------------------------------
void mglCanvas::SetTicksVal(char dir, HCDT v, const char *lbl, bool add)
{
	long ll=strlen(lbl);
	wchar_t *wcs = new wchar_t[ll+1];
	mbstowcs(wcs,lbl,ll);	wcs[ll]=0;
	SetTicksVal(dir,v,wcs,add);
	delete []wcs;
}
//-----------------------------------------------------------------------------
void mglCanvas::SetTicksVal(char dir, const wchar_t *lbl, bool add)
{
	register long i,j,len=wcslen(lbl);
	for(i=0,j=1;j<len;j++)
		if(lbl[j]=='\n' || (lbl[j]=='n' && lbl[j-1]=='\\'))	i++;
	if(i>63)	i=63;
	mglData val(i+1);	val.Fill(Min.x,Max.x);
	SetTicksVal(dir, &val, lbl, add);
}
//-----------------------------------------------------------------------------
void mglCanvas::SetTicksVal(char dir, const char *lbl, bool add)
{
	register long i,j,len=strlen(lbl);
	for(i=0,j=1;j<len;j++)
		if(lbl[j]=='\n' || (lbl[j]=='n' && lbl[j-1]=='\\'))	i++;
	if(i>63)	i=63;
	mglData val(i+1);	val.Fill(Min.x,Max.x);
	SetTicksVal(dir, &val, lbl, add);
}
//-----------------------------------------------------------------------------
void mglCanvas::SetTicksVal(char dir, HCDT v, const wchar_t **lbl, bool add)
{
	mglAxis &aa=ax;
	bool ff;
	if(dir=='c' || dir=='a')	{	aa = ac;	ff=fa;	}
	else if(dir=='x')	{	aa = ax;	ff=fx;	}
	else if(dir=='y')	{	aa = ay;	ff=fy;	}
	else if(dir=='z')	{	aa = az;	ff=fz;	}
	else return;

	aa.txt.clear();
	if(add)	{	UpdateAxis();	AdjustTicks(aa,ff);	}
	if(!v || !lbl)	{	aa.f = 0;	return;	}
	aa.f = 2;	aa.ns=0;	aa.ds=0;
	register long i,n=v->GetNx();
	for(i=0;i<n;i++)	aa.AddLabel(lbl[i],v->v(i));
}
//-----------------------------------------------------------------------------
void mglCanvas::SetTicksVal(char dir, HCDT v, const char **lbl, bool add)
{
	mglAxis &aa=ax;
	bool ff;
	if(dir=='c' || dir=='a')	{	aa = ac;	ff=fa;	}
	else if(dir=='x')	{	aa = ax;	ff=fx;	}
	else if(dir=='y')	{	aa = ay;	ff=fy;	}
	else if(dir=='z')	{	aa = az;	ff=fz;	}
	else return;

	aa.txt.clear();
	if(add)	{	UpdateAxis();	AdjustTicks(aa,ff);	}
	if(!v || !lbl)	{	aa.f = 0;	return;	}
	aa.f = 2;	aa.ns=0;	aa.ds=0;
	register unsigned long i,n=v->GetNx(),l=0;
	for(i=0;i<n;i++)	if(strlen(lbl[i])>l)	l=strlen(lbl[i]);
	wchar_t *str=new wchar_t[l+1];
	for(i=0;i<n;i++)
	{
		mbstowcs(str,lbl[i],strlen(lbl[i])+1);
		aa.AddLabel(str,v->v(i));
	}
	delete []str;
}
//-----------------------------------------------------------------------------
void mglCanvas::SetTickTempl(char dir, const wchar_t *t)
{
	mglAxis &aa=ax;
	if(dir=='c' || dir=='a')	aa = ac;
	else if(dir=='x')	aa = ax;
	else if(dir=='y')	aa = ay;
	else if(dir=='z')	aa = az;
	else return;

	if(aa.f==1)	aa.f = 0;	// remove time ticks
	if(!t || !t[0])	aa.t[0]=0;
	else if(wcslen(t)<255) wcscpy(aa.t,t);
}
//-----------------------------------------------------------------------------
void mglCanvas::SetTickTempl(char dir, const char *t)
{
	mglAxis &aa=ax;
	if(dir=='c' || dir=='a')	aa = ac;
	else if(dir=='x')	aa = ax;
	else if(dir=='y')	aa = ay;
	else if(dir=='z')	aa = az;
	else return;

	if(aa.f==1)	aa.f = 0;	// remove time ticks
	if(!t || !t[0])	aa.t[0]=0;
	else if(strlen(t)<255) mbstowcs(aa.t,t,strlen(t)+1);
}
//-----------------------------------------------------------------------------
void mglCanvas::SetTickTime(char dir, float d, const char *t)
{
	UpdateAxis();	mglAxis &aa=ax;
	if(dir=='c' || dir=='a')	aa = ac;
	else if(dir=='x')	aa = ax;
	else if(dir=='y')	aa = ay;
	else if(dir=='z')	aa = az;
	else return;


	if(!t || !t[0])	// adjust template
	{
		time_t tt;	tm t1,t2;
		tt=aa.v1;	localtime_r(&tt,&t1);
		tt=aa.v2;	localtime_r(&tt,&t2);
		t = t1.tm_yday!=t2.tm_yday ? "%x" : "%X";
	}

	aa.dv = d;	aa.f = 1;	aa.txt.clear();
	if(strlen(t)<255) mbstowcs(aa.t,t,strlen(t)+1);

	if(strchr("xyztuvw",aa.ch))
		aa.org = mglPoint(GetOrgX(aa.ch), GetOrgY(aa.ch), GetOrgZ(aa.ch));
	if(aa.ch=='x')	aa.v0 = aa.org.x;
	if(aa.ch=='y')	aa.v0 = aa.org.y;
	if(aa.ch=='z')	aa.v0 = aa.org.z;

	wchar_t buf[64];
	float v, v0 = isnan(aa.o) ? aa.v0 : aa.o, v1;
	if(aa.v2>aa.v1)
	{	v1 = aa.v2;		v0 = v0 - aa.dv*floor((v0-aa.v1)/aa.dv+1e-3);	}
	else
	{	v1 = aa.v1;		v0 = v0 - aa.dv*floor((v0-aa.v2)/aa.dv+1e-3);	}
	if(v0+aa.dv!=v0 && v1+aa.dv!=v1)	for(v=v0;v<=v1;v+=aa.dv)
	{
		time_t tt = v;	tm tp;		localtime_r(&tt,&tp);
		wcsftime(buf,64,aa.t,&tp);	aa.AddLabel(buf,v);
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::AdjustTicks(const char *dir, bool force)
{
	UpdateAxis();	TuneTicks = true;
	if(strchr(dir,'x'))
	{	if(force)	ax.d=0;	AdjustTicks(ax,fx);	}
	if(strchr(dir,'y'))
	{	if(force)	ay.d=0;	AdjustTicks(ay,fy);	}
	if(strchr(dir,'z'))
	{	if(force)	az.d=0;	AdjustTicks(az,fz);	}
	if(strchr(dir,'a') || strchr(dir,'c'))
	{	if(force)	ac.d=0;	AdjustTicks(ac,fa);	}
}
//-----------------------------------------------------------------------------
void mglCanvas::AdjustTicks(mglAxis &aa, bool ff)
{
	float d = fabs(aa.v2-aa.v1), n;
	if(aa.f>0)	return;
	if(ff && islog(aa.v1,aa.v2))
	{	aa.dv = 0;	aa.ds=0;	}
	else if(aa.d>0)
	{	aa.dv = aa.d;	aa.ds = aa.d/(fabs(aa.ns)+1);	}
	else if(aa.d>-1.5)	// like =0 or =-1
	{
		n = floor(log10(d));	d = floor(d*pow(10.,-n));
		n = pow(10.,n);			aa.o=0;
		if(d==1)	{	aa.dv = n/5;	aa.ds=n/10;	}
		else if(d<4){	aa.dv = n/2;	aa.ds=n/10;	}
		else if(d<7){	aa.dv = n;		aa.ds=n/5;	}
		else		{	aa.dv = 2*n;	aa.ds=n/2;	}
	}
	else	// TODO: Check it!!!
	{
		d /= -aa.d;		n = floor(log10(d));
		int k = int(d*pow(10.,-n)+0.5);
		aa.dv = pow(10.,n)*k;
		aa.o=0;	aa.ds = pow(10.,n);
	}
	LabelTicks(aa);
}
//-----------------------------------------------------------------------------
int mgl_tick_ext(float a, float b, wchar_t s[32], float &v)
{
	int kind = 0;
	if(fabs(a-b)<0.01*fabs(a))
	{
		kind = 1;
		v = fabs(a-b);
		if(v>100.f)
		{
			int k=int(log10(v)-0.01);
			kind=3;		v=mgl_ipow(10.,k);
			mglprintf(s, 32, L"(@{\\times{}10^{%d}})", k);
		}
		if(v<1e-2f)
		{
			int k=int(log10(v)-0.01)-1;
			kind=3;		v=mgl_ipow(10.,k);
			mglprintf(s, 32, L"(@{\\times{}10^{%d}})", k);
		}
	}
	else
	{
		v = fabs(b)>fabs(a)?fabs(b):fabs(a);
		if(v>100.f)
		{
			kind = 2;
			int k=int(log10(v)-0.01);
			v=mgl_ipow(10.,k);
			mglprintf(s, 32, L"\\times 10^{%d}", k);
		}
		if(v<1e-2f)
		{
			kind = 2;
			int k=int(log10(v)-0.01)-1;
			v=mgl_ipow(10.,k);
			mglprintf(s, 32, L"\\times 10^{%d}", k);
		}
	}
	return kind;
}
//-----------------------------------------------------------------------------
void mgl_tick_text(float z, float z0, float d, float v, int kind, wchar_t str[64])
{
	float u = fabs(z)<d ? 0:z;
	if((kind&1) && z>z0)	u = fabs(z-z0)<d ? 0:(z-z0);
	if(kind==2 || (kind==3 && z>z0))	u /= v;
	if((kind&1) && z>z0)
	{
		int n1,n2;
		mglprintf(str, 64, fabs(u)<1 ? L"@{(+%.2g)}" : L"@{(+%.3g)}",u);
		n1=wcslen(str);	mglprintf(str, 64, L"@{(+%g)}",u);	n2=wcslen(str);
		if(n1<n2)	mglprintf(str, 64, fabs(u)<1 ? L"@{(+%.2g)}" : L"@{(+%.3g)}",u);
	}
	else
	{
		int n1,n2;
		mglprintf(str, 64, fabs(u)<1 ? L"%.2g" :  L"%.3g",u);
		n1=wcslen(str);	mglprintf(str, 64, L"%g",u);	n2=wcslen(str);
		if(n1<n2)	mglprintf(str, 64, fabs(u)<1 ? L"%.2g" :  L"%.3g",u);
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::LabelTicks(mglAxis &aa)
{
	if(strchr("xyztuvw",aa.ch))
		aa.org = mglPoint(GetOrgX(aa.ch), GetOrgY(aa.ch), GetOrgZ(aa.ch));
	if(aa.ch=='x')	aa.v0 = aa.org.x;
	if(aa.ch=='y')	aa.v0 = aa.org.y;
	if(aa.ch=='z')	aa.v0 = aa.org.z;

	wchar_t buf[64];
	float v,v0,v1,w;
	if(aa.f)	return;
	aa.txt.clear();
	if(aa.dv==0 && aa.v1>0)	// positive log-scale
	{
		v0 = exp(M_LN10*floor(0.1+log10(aa.v1)));
		for(v=v0;v<=aa.v2*MGL_FLT_EPS;v*=10)	if(v*MGL_FLT_EPS>=aa.v1)
		{
			mglprintf(buf,64,L"10^{%d}",int(floor(0.1+log10(v))));
			aa.AddLabel(buf,v);
		}
	}
	else if(aa.dv==0 && aa.v2<0)	// negative log-scale
	{
		v0 = -exp(M_LN10*floor(0.1+log10(-aa.v2)));
		for(v=v0;v>=aa.v1*MGL_FLT_EPS;v*=10)	if(v*MGL_FLT_EPS<=aa.v2)
		{
			mglprintf(buf,64,L"-10^{%d}",int(floor(0.1+log10(-v))));
			aa.AddLabel(buf,v);
		}
	}
	else if(aa.dv)	// ticks drawing
	{
		int kind=0;
		wchar_t s[32]=L"";
		if(aa.t[0]==0) kind = mgl_tick_ext(aa.v2, aa.v1, s, w);
		if(!TuneTicks)	kind = 0;

		v0 = isnan(aa.o) ? aa.v0 : aa.o;
		if(aa.v2>aa.v1)
		{	v1 = aa.v2;		v0 = v0 - aa.dv*floor((v0-aa.v1)/aa.dv+1e-3);	}
		else
		{	v1 = aa.v1;		v0 = v0 - aa.dv*floor((v0-aa.v2)/aa.dv+1e-3);	}

		if(v0+aa.dv!=v0 && v1+aa.dv!=v1)	for(v=v0;v<=v1;v+=aa.dv)
		{
			if(aa.t[0])
				mglprintf(buf, 64, aa.t, fabs(v)<aa.dv/100 ? 0 : v);
			else
				mgl_tick_text(v,v0,aa.dv/100,w,kind,buf);
			mgl_wcstrim(buf);	aa.AddLabel(buf,v);
		}
		if(kind&2)	aa.AddLabel(s,FactorPos*(aa.v2-aa.v1)+aa.v1);
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::Axis(const char *dir, bool adjust)
{
	if(!dir || !dir[0])	return;
	bool text = !strchr(dir,'_');
	const char *ar = "AKDTVISO";
	char arr=0;
	for(unsigned i=0;i<strlen(ar);i++)
		if(strchr(dir,ar[i]))	{	arr=ar[i];	break;	}

	AdjustTicks(dir,adjust);
	// TODO: Ternary axis labeling ...
	if(strchr(dir,'x'))	DrawAxis(ax, text, arr);
	if(strchr(dir,'y'))	DrawAxis(ay, text, arr);
	if(strchr(dir,'z'))	DrawAxis(az, text, arr);
}
//-----------------------------------------------------------------------------
void mglCanvas::DrawAxis(mglAxis &aa, bool text, char arr)
{
	if(strchr("xyztuvw",aa.ch))
		aa.org = mglPoint(GetOrgX(aa.ch), GetOrgY(aa.ch), GetOrgZ(aa.ch));
	if(aa.ch=='x')	aa.v0 = aa.org.x;
	if(aa.ch=='y')	aa.v0 = aa.org.y;
	if(aa.ch=='z')	aa.v0 = aa.org.z;

	mglPoint d = aa.dir, o = aa.org - d*(aa.org*d);	// "transverse" org
	mglPoint av=(Min+Max)/2, dv,da,db, p;
	dv = mglPoint(sign(av.x-o.x), sign(av.y-o.y), sign(av.z-o.z));
	da = aa.a*(dv*aa.a);	db = aa.b*(dv*aa.b);
	SetPenPal(AxisStl);

	register long i,j,k1,k2;
	p = o + d*aa.v1;	k1 = AddPnt(p,CDef);
	for(i=1;i<31;i++)	// axis itself
	{
		p = o + d*(aa.v1+(aa.v2-aa.v1)*i/30.);
		k2 = k1;	k1 = AddPnt(p,CDef);
		line_plot(k2,k1);
	}
	if(arr)
	{
		p = o + d*(aa.v1+(aa.v2-aa.v1)*1.05);
		k2 = k1;	k1 = AddPnt(p,CDef);
		arrow_plot(k1,k2,arr);
	}

	k2 = aa.txt.size();
	float v, u, v0 = isnan(aa.o) ? aa.v0 : aa.o;
	if(k2>0)	for(i=0;i<k2;i++)
	{
		v = aa.txt[i].val;	u = fabs(v);
		tick_draw(o+d*v, da, db, 0);
		if(aa.dv==0 && fabs(u-exp(M_LN10*floor(0.1+log10(u))))<0.01*u)
			for(j=2;j<10 && v*j<aa.v2;j++)	tick_draw(o+d*(v*j),da,db,1);
	}
	if(aa.ds>0)
	{
		if(aa.v2>aa.v1)	v0 = v0 - aa.ds*floor((v0-aa.v1)/aa.ds+1e-3);
		else			v0 = v0 - aa.ds*floor((v0-aa.v2)/aa.ds+1e-3);
		if(v0+aa.ds!=v0 && aa.v2+aa.ds!=aa.v2)
			for(v=v0;v<aa.v2;v+=aa.ds)	tick_draw(o+d*v,da,db,1);
	}
	if(text)	DrawLabels(aa);
}
//-----------------------------------------------------------------------------
void mglCanvas::DrawLabels(mglAxis &aa)
{
	if(strchr("xyztuvw",aa.ch))
		aa.org = mglPoint(GetOrgX(aa.ch), GetOrgY(aa.ch), GetOrgZ(aa.ch));
	mglPoint d = aa.dir, o = aa.org - d*(aa.org*d);	// "transverse" org
	mglPoint p,q, s=(Min+Max)/2, nn;
	s = s - d*(s*d);

	register long i,k1,n = aa.txt.size();
	char pos[2]="t";
	if(get(MGL_DISABLE_SCALE) && ((aa.dir.x==0 && aa.org.x<0) || (aa.dir.y==0 && aa.org.y>0)))	pos[0]='T';
	float *w=new float[n], h = TextHeight()/4, c=NAN, l=NAN, tet=0, v;	// find sizes
	long *kk=new long[n];
	for(i=0;i<n;i++)
	{
		w[i] = TextWidth(aa.txt[i].text.c_str())/2;
		v = aa.txt[i].val;	kk[i] = AddPnt(o+d*v,-1,d,0,3);
		q=p;	p = GetPnt(kk[i]);	v = i>0 ? (p-q).norm() : NAN;
		c = c<v ? c:v;	l = l<w[i] ? l:w[i];
	}
	c /= 1.1;
	if(!get(MGL_ENABLE_RTEXT) && get(MGL_TICKS_ROTATE))	// try rotate first
	{	tet = c>h ? asin(h/c) : M_PI/2;
		l=h/sin(tet);	for(i=0;i<n;i++)	w[i]=l;	}
	// TODO: do smater points exclusion (i.e. longest and so on)
	long k = get(MGL_TICKS_SKIP) ? 1+l/c : 1;
	if(n>0)	for(i=0;i<n;i++)
	{
		c = aa.txt[i].val;
		if(c>aa.v1 && c<aa.v2 && i%k!=0)	continue;
		p = o+d*c;	k1 = AddPnt(p,-1,d,0,3);
		nn = s-o;	ScalePoint(p,nn,false);
		if(!get(MGL_DISABLE_SCALE))	pos[0]=(nn.y>0 || nn.x<0) ? 'T':'t';
		text_plot(k1, aa.txt[i].text.c_str(), pos, -1, 0.07);
	}
	delete []w;	delete []kk;
}
//-----------------------------------------------------------------------------
void mglCanvas::tick_draw(mglPoint o, mglPoint d1, mglPoint d2, int f)
{
	if(TickLen==0)	return;
	float v = font_factor*TickLen/sqrt(1+f*st_t);
	mglPoint p=o;
	long k1,k2,k3;

	if(*TickStl && !f)	SetPenPal(TickStl);
	if(*SubTStl && f)	SetPenPal(SubTStl);
	ScalePoint(o, d1, false);	d1.Normalize();
	ScalePoint(p, d2, false);	d2.Normalize();
	k2 = AddPnt(p, CDef, mglPoint(NAN), 0, 0);
	p += d1*v;	k1 = AddPnt(p, CDef, mglPoint(NAN), 0, 0);
	p = o+d2*v;	k3 = AddPnt(p, CDef, mglPoint(NAN), 0, 0);
	line_plot(k1,k2);	line_plot(k2,k3);
}
//-----------------------------------------------------------------------------
void mglCanvas::Grid(const char *dir, const char *pen)
{
	if(!dir || !dir[0])	return;

	AdjustTicks(dir,false);
	SetPenPal(pen);
	// TODO: Ternary axis labeling ...
	if(strchr(dir,'x'))	DrawGrid(ax);
	if(strchr(dir,'y'))	DrawGrid(ay);
	if(strchr(dir,'z'))	DrawGrid(az);
}
//-----------------------------------------------------------------------------
void mglCanvas::DrawGrid(mglAxis &aa)
{
	aa.org = mglPoint(GetOrgX(aa.ch), GetOrgY(aa.ch), GetOrgZ(aa.ch));
	mglPoint d=aa.dir, da1,da2,db1,db2,oa,ob, p,q;
	da1 = aa.a*(aa.a*Min);	da2 = aa.a*(aa.a*Max);
	db1 = aa.b*(aa.b*Min);	db2 = aa.b*(aa.b*Max);
	oa  = aa.b*(aa.b*aa.org);	ob  = aa.a*(aa.a*aa.org);

	register long i,j,n=aa.txt.size(),k1,k2;
	float v;

	Reserve(62*n);
	if(n>0)	for(i=0;i<n;i++)
	{
		q = oa+d*aa.txt[i].val;	p = q+da1;	// lines along 'a'
		k1 = AddPnt(p,CDef);
		for(j=1;j<31;j++)
		{
			v = j/30.;
			p = q+da1*(1-v)+da2*v;
			k2 = k1;	k1 = AddPnt(p,CDef);
			line_plot(k2,k1);
		}
		q = ob+d*aa.txt[i].val;	p = q+db1;	// lines along 'b'
		k1 = AddPnt(p,CDef);
		for(j=1;j<31;j++)
		{
			v = j/30.;
			p = q+db1*(1-v)+db2*v;
			k2 = k1;	k1 = AddPnt(p,CDef);
			line_plot(k2,k1);
		}
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::Colorbar(const char *sch,int where)
{	// ‘0’ - right, ‘1’ - left, ‘2’ - above, ‘3’ - under
	if(sch && strchr(sch,'>'))	where = 0;
	if(sch && strchr(sch,'<'))	where = 1;
	if(sch && strchr(sch,'^'))	where = 2;
	if(sch && strchr(sch,'_'))	where = 3;
	if(sch && strchr(sch,'A'))	{	Push();	Identity();	}
	Colorbar(where, where==0?1:0, where==2?1:0, 1, 1, AddTexture(sch));
	if(sch && strchr(sch,'A'))	Pop();
}
//-----------------------------------------------------------------------------
void mglCanvas::Colorbar(int where, float x, float y, float w, float h, long s)
{
//	float d = fabs(Max.c-Min.c);
//	d = floor(d*pow(10,-floor(log10(d))));
//	long n = 50*(d<4?int(2*d+0.5):int(d+0.5))+1;
//	if(d==1.f)	n = 101;
	long n=256;
	mglData v(n);
	if(ac.d || Min.c*Max.c<=0)	v.Fill(Min.c,Max.c);
	else if(Max.c>Min.c && Min.c>0)
	{	v.Fill(log(Min.c), log(Max.c));	v.Modify("exp(u)");		}
	else if(Min.c<Max.c && Max.c<0)
	{	v.Fill(log(-Min.c), log(-Max.c));	v.Modify("-exp(u)");	}
	float *c=new float[n];
	for(long i=0;i<n;i++)	c[i] = GetC(s,v.a[i]);
	colorbar(&v, c, where, x, y, w, h);
	delete []c;
}
//-----------------------------------------------------------------------------
void mglCanvas::Colorbar(HCDT v, const char *sch,int where)
{
	if(sch && strchr(sch,'>'))	where = 0;
	if(sch && strchr(sch,'<'))	where = 1;
	if(sch && strchr(sch,'^'))	where = 2;
	if(sch && strchr(sch,'_'))	where = 3;
	if(sch && strchr(sch,'A'))	{	Push();	Identity();	}
	Colorbar(v,sch,where, where==0?1:0, where==2?1:0, 1, 1);
	if(sch && strchr(sch,'A'))	Pop();
}
//-----------------------------------------------------------------------------
void mglCanvas::Colorbar(HCDT v, const char *sch, int where, float x, float y, float w, float h)
{
	float *c=new float[v->GetNx()];
	if(!sch || !(*sch))	sch = MGL_DEF_PAL;
	long s = AddTexture(sch);	// TODO Check it
	int nc = GetNumPal(s*256);
	float dc = nc>1 ? 1/(MGL_FLT_EPS*(nc-1)):0;
	for(long i=0;i<v->GetNx();i++)	c[i] = s+i*dc;
	colorbar(v, c, where, x, y, w, h);
	delete []c;
}
//-----------------------------------------------------------------------------
void mglCanvas::Label(char dir, const char *str, float pos, float shift)
{
	unsigned s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	Labelw(dir, wcs, pos, shift);
	delete []wcs;
}
//-----------------------------------------------------------------------------
void mglCanvas::Labelw(char dir, const wchar_t *text, float pos, float shift)
{
	float t, x0, y0, z0;
	x0 = GetOrgX(dir);	y0 = GetOrgY(dir);	z0 = GetOrgZ(dir);
	mglPoint p,ss=(Min+Max)/2,q,qq,nn;

	if(dir=='x')	//	TODO: Tern axis & colorbar labels!!!
	{
		AdjustTicks(ax,fx);
		if(ax.dv)	t = (Min.x+Max.x+pos*(Max.x-Min.x))/2;
		else	t = Min.x*pow(Max.x/Min.x, (pos+1)/2);
		nn = mglPoint(0,ss.y-y0,ss.z-z0);
		p = mglPoint(t,y0,z0);	q = mglPoint(1,0,0);
	}
	if(dir=='y')
	{
		AdjustTicks(ay,fy);
		if(ay.dv)	t = (Min.y+Max.y+pos*(Max.y-Min.y))/2;
		else	t = Min.y*pow(Max.y/Min.y, (pos+1)/2);
		nn = mglPoint(ss.x-x0,0,ss.z-z0);
		p = mglPoint(x0,t,z0);	q = mglPoint(0,1,0);
	}
	if(dir=='z')
	{
		AdjustTicks(az,fz);
		if(az.dv)	t = (Min.z+Max.z+pos*(Max.z-Min.z))/2;
		else	t = Min.z*pow(Max.z/Min.z, (pos+1)/2);
		nn = mglPoint(ss.x-x0,ss.y-y0,0);
		p = mglPoint(x0,y0,t);	q = mglPoint(0,0,1);
	}
	ss = p;	ScalePoint(ss,nn,false);
	char font[33],ff[3]=":C";
	if(pos<-0.2)	ff[1]='L';	if(pos>0.2)	ff[1]='R';
	strcpy(font,FontDef);	strcat(font,ff);
	strcat(font,nn.y>1e-5 || nn.x<0 ? "T":"t");
	text_plot(AddPnt(p,-1,q,0),text,font,-1.4,0.3+shift);
}
//-----------------------------------------------------------------------------
void mglCanvas::Label(float x, float y, const char *str, const char *font, bool rel)
{
	unsigned s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	Labelw(x,y,wcs, font, rel);
	delete []wcs;
}
//-----------------------------------------------------------------------------
void mglCanvas::Labelw(float x, float y, const wchar_t *text, const char *font, bool rel)
{
	Push();	Identity(rel);
	mglFormula *ox=fx, *oy=fy, *oz=fz;
	fx = fy = fz = NULL;
	char *f = new char[strlen(font)+1];	memset(f,0,strlen(font)+1);
	strcpy(f,font);
	for(int i=0;f[i];i++)	if(f[i]=='a' || f[i]=='A')	f[i]=' ';
	mglPoint p((Min.x+Max.x)/2+B.pf*(Max.x-Min.x)*(x-0.5),
				(Min.y+Max.y)/2+B.pf*(Max.y-Min.y)*(y-0.5), Max.z);
	text_plot(AddPnt(p,-1,mglPoint(NAN),0),text,f,-1.4,1);
	delete []f;	fx=ox;	fy=oy;	fz=oz;	Pop();
}
//-----------------------------------------------------------------------------
/*void mglCanvas::Title(const wchar_t *str,const char *font)
{
	Push();	Identity();
	mglFormula *ox=fx, *oy=fy, *oz=fz;
	fx = fy = fz = NULL;
	mglPoint p((Min.x+Max.x)/2, Max.y+(Max.y-Min.y)*0.15, (Min.z+Max.z)/2);
	ScalePoint(p);	text_plot(AddPnt(p,-1,mglPoint(NAN),0), str, font, size);
	fx=ox;	fy=oy;	fz=oz;	Pop();
}
//-----------------------------------------------------------------------------
void mglCanvas::Title(const char *str,const char *font)
{
	unsigned s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	Title(wcs, font, size);
	delete []wcs;
}*/
//-----------------------------------------------------------------------------
//void mglCanvas::Box(const char *col, bool ticks)	// TODO: Add color
void mglCanvas::Box(const char *col, bool ticks)
{
	mglPoint o = Org;
	float tl=TickLen;
	if(!ticks)	TickLen=0;
	SetPenPal(col);
	Org = Min;	Axis("xyz_");
	if(TernAxis&1)
	{
		Axis("t_");
		Org.z=Max.z;	Axis("xyt_");
		Org.x=Max.x;	Axis("z_");
		Org.x=Min.x;	Org.y=Max.y;	Axis("z_");
	}
	else if(TernAxis&2)
		Axis("tuv_");
	else
	{
		Org.z=Max.z;	Axis("xy_");
		Org = Max;		Axis("xyz_");
		Org.z=Min.z;	Axis("xy_");
		Org.x=Min.x;	DrawAxis(az,0,0);
		Org.x=Max.x;	Org.y=Min.y;	DrawAxis(az,0,0);
	}
	Org=o;	TickLen=tl;
}
//-----------------------------------------------------------------------------
void mglCanvas::colorbar(HCDT vv, const float *c, int where, float x, float y, float w, float h)
{
	static int cgid=1;	StartGroup("Colorbar",cgid++);
	register unsigned long i,n=vv->GetNx();
	long n1,n2,n3,n4;
	float d,s3=B.pf,ss=1;		// NOTE: colorbar was wider ss=s3*0.9;
	mglPoint p1,p2;

	Push();	set(MGL_DISABLE_SCALE);	B=B1;	B.pf=s3;
	x = 2*x-1;	y = 2*y-1;
	for(i=0;i<n-1;i++)
	{
		d = GetA(vv->v(i))*2-1;
		p1 = p2 = mglPoint((ss*d+s3)*w+x*s3, (ss*d+s3)*h+y*s3, s3);
		switch(where)
		{
			case 1:	p1.x = x*s3;	p2.x = (x+0.1*w)*s3;	break;
			case 2:	p1.y = (y-0.1*h)*s3;	p2.y = y*s3;	break;
			case 3:	p1.y = y*s3;	p2.y = (y+0.1*h)*s3;	break;
			default:p1.x = (x-0.1*w)*s3;	p2.x = x*s3;	break;
		}
		n1 = AddPnt(p1,c[i]);	n2 = AddPnt(p2,c[i]);
		d = GetA(vv->v(i+1))*2-1;
		p1 = p2 = mglPoint((ss*d+s3)*w+x*s3, (ss*d+s3)*h+y*s3, s3);
		switch(where)
		{
			case 1:	p1.x = x*s3;	p2.x = (x+0.1*w)*s3;	break;
			case 2:	p1.y = (y-0.1*h)*s3;	p2.y = y*s3;	break;
			case 3:	p1.y = y*s3;	p2.y = (y+0.1*h)*s3;	break;
			default:p1.x = (x-0.1*w)*s3;	p2.x = x*s3;	break;
		}
		n3 = AddPnt(p1,c[i]);	n4 = AddPnt(p2,c[i]);
		quad_plot(n1,n2,n3,n4);
	}
	if(n<64)
	{
		wchar_t buf[64];
		for(i=0;i<n;i++)
		{
			d = vv->v(i);
			mglprintf(buf,64,ac.t[0]?ac.t:(fabs(d)<1 ? L"%.2g" :  L"%.3g"),d);
			ac.AddLabel(buf,d);
		}
	}
	else	{	UpdateAxis();	AdjustTicks(ac,fa);	}
	// hint for using standard label drawing function
	float cc=AddTexture('k');
	for(i=0;i<ac.txt.size();i++)
	{
		d = ac.txt[i].val = GetA(ac.txt[i].val)*2-1;
		p1 = p2 = mglPoint((ss*d+s3)*w+x*s3, (ss*d+s3)*h+y*s3, s3);
		switch(where)
		{
			case 1:	p1.x = x*s3;	p2.x = (x+0.1*w)*s3;	break;
			case 2:	p1.y = (y-0.1*h)*s3;	p2.y = y*s3;	break;
			case 3:	p1.y = y*s3;	p2.y = (y+0.1*h)*s3;	break;
			default:p1.x = (x-0.1*w)*s3;	p2.x = x*s3;	break;
		}
		n1 = AddPnt(p1,cc);	n2 = AddPnt(p2,cc);
		line_plot(n1,n2);
	}
	ac.dir = mglPoint(ss*w,ss*h,0);
	ac.org = mglPoint(s3*(w+x),s3*(h+y),s3+1);
	switch(where)
	{
		case 1:	ac.dir.x = 0;	ac.org.x = (x+0.1*w)*s3;	break;
		case 2:	ac.dir.y = 0;	ac.org.y = (y-0.1*h)*s3;	break;
		case 3:	ac.dir.y = 0;	ac.org.y = (y+0.1*h)*s3;	break;
		default:ac.dir.x = 0;	ac.org.x = (x-0.1*w)*s3;	break;
	}
	DrawLabels(ac);
	Pop();	clr(MGL_DISABLE_SCALE);	EndGroup();
}
//-----------------------------------------------------------------------------
