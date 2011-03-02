/***************************************************************************
 * cont.cpp is part of Math Graphic Library
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
#include <wchar.h>
#include "mgl/surf.h"
#include "mgl/cont.h"
#include "mgl/data.h"
#include "mgl/eval.h"
//-----------------------------------------------------------------------------
//
//	Text printing along a curve
//
//-----------------------------------------------------------------------------
bool same_chain(long f,long i,long *nn)
{
	long j=f;
	while(1)
	{
		j = nn[j];
		if(j==f || j<0)	return false;
		if(j==i)	return true;
	}
}
//-----------------------------------------------------------------------------
void mgl_string_curve(mglBase *gr,long f,long n,long *ff,long *nn,const wchar_t *text, float size, const char *font)
{
	int pos = font && strchr(font,'T');
	wchar_t L[2]=L"a";
	mglPoint p1,n1,p2;

	float w, r, ww, wg=gr->TextHeight()*size/8.;//*font_factor;
	register long i,k,h,pp;

	h=f;	k=nn[f];	// print string symbol-by-symbol
	mglPoint p0=gr->GetPntC(ff[h]),n0=gr->GetPntC(ff[k])-p0, pa;
	mglColor c=gr->GetClrC(ff[h]);

	for(unsigned j=0;j<wcslen(text);j++)
	{
		L[0] = text[j];	pa = pos>0 ? p0 : p0-wg*(!n0);
		gr->ScalePoint(pa);	pp = gr->AddPntN(pa,c,n0,false);
		ww = gr->text_plot(pp,text,font,size);
		p1 = p0+(ww/Norm(n0))*n0;
		// let find closest point
		for(r=1e10,i=0;i<n;i++)
		{
			n1 = gr->GetPntC(ff[i]);
			w = Norm(p1-n1);
			if(w<r)	{	r=w;	k=i;	}
		}
		i=k;	k=nn[i];
		for(h=0;h<n;h++)	if(nn[h]==i)	break;	// h is previous point
		// point not in the same chain (continue by stright line)
		if(k<0 || h>=n || !same_chain(f,i,nn))	{	p0=p1;	continue;	}
		// if last point let change it to previous one
		if(k<0)	{	k=i;	i=h;	}
		p2 = gr->GetPntC(ff[i]);	n1 = gr->GetPntC(ff[k])-p2;
		w = (p1-p2)*n1;
		// go in wrong direction, let turn back
		if((w<0 || w>n1*n1) && i!=h)
		{	p2 = gr->GetPntC(ff[h]);n1 = gr->GetPntC(ff[i])-p2;	k = i;	}
		// under maximum and should skip some points
		if(n1.x*n0.y>n1.y*n0.x && pos<0)
		{
			p1 = p0+((ww/Norm(n0))*n0) + ((wg/Norm(n0)/3)*mglPoint(n0.y,-n0.x,n0.z));
			for(r=1e10,i=0;i<n;i++)	// let find closest point
			{
				n1 = gr->GetPntC(ff[i]);
				w = Norm(p1-n1);
				if(w<r)	{	r=w;	k=i;	}
			}
			i=k;	k=nn[i];
			for(h=0;h<n;h++)	if(nn[h]==i)	break;
			if(k<0 || h>=n || !same_chain(f,i,nn))
			{	p0 = p0+(ww/Norm(n0))*n0;	continue;	}
			if(k<0)	{	k=i;	i=h;	}
			p2 = gr->GetPntC(ff[i]);	n1 = gr->GetPntC(ff[k])-p2;
			w = (p1-p2)*n1;
			if((w<0 || w>n1*n1) && i!=h)
			{	p2 = gr->GetPntC(ff[h]);n1 = gr->GetPntC(ff[i])-p2;	k = i;	}
		}
		// above minimum and should skip some points
		if(n1.x*n0.y<n1.y*n0.x && pos>0)
		{
			p1 = p0+((ww/Norm(n0))*n0) - ((wg/Norm(n0)/3)*mglPoint(n0.y,-n0.x,n0.z));
			for(r=1e10,i=0;i<n;i++)	// let find closest point
			{
				n1 = gr->GetPntC(ff[i]);
				w = Norm(p1-n1);
				if(w<r)	{	r=w;	k=i;	}
			}
			i=k;	k=nn[i];
			for(h=0;h<n;h++)	if(nn[h]==i)	break;
			if(k<0 || h>=n || !same_chain(f,i,nn))
			{	p0 = p0+(ww/Norm(n0))*n0;	continue;	}
			if(k<0)	{	k=i;	i=h;	}
			p2 = gr->GetPntC(ff[i]);	n1 = gr->GetPntC(ff[k])-p2;
			w = (p1-p2)*n1;
			if((w<0 || w>n1*n1) && i!=h)
			{	p2 = gr->GetPntC(ff[h]);n1 = gr->GetPntC(ff[i])-p2;	k = i;	}
		}
		// OK, next point will be
		p0 = p2 + n1*(((p1-p2)*n1)/(n1*n1));
		n0 = n1;
	}
}
//-----------------------------------------------------------------------------
void mgl_textw_xyz(HMGL gr, HCDT x, HCDT y, HCDT z,const wchar_t *text, const char *font, float size)
{
	long n=y->GetNx();
	if(x->GetNx()!=n || z->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"Text");	return;	}
	if(n<2)	{	gr->SetWarn(mglWarnLow,"Text");	return;	}
	static int cgid=1;	gr->StartGroup("TextC",cgid++);

	long *nn = new long[n], *ff = new long[n];
	mglPoint p;
	register long i;
	for(i=0;i<n;i++)
	{
		p = mglPoint(x->v(i),y->v(i),z->v(i));
		gr->ScalePoint(p);	ff[i] = gr->AddPntC(p,NC);
	}
	for(i=1;i<n;i++)	nn[i-1] = i;
	nn[n-1]=-1;
	if(size<0)	size = -size*gr->GetFontSize();
	mgl_string_curve(gr,0,n,ff,nn,text,size,font);
	gr->EndGroup();
	delete []ff;	delete []nn;
}
//-----------------------------------------------------------------------------
void mgl_text_xyz(HMGL gr, HCDT x, HCDT y, HCDT z,const char *text, const char *font, float size)
{
	unsigned s = strlen(text)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,text,s);
	mgl_textw_xyz(gr,x,y,z, wcs, font, size);
	delete []wcs;
}
//-----------------------------------------------------------------------------
void mgl_text_xy(HMGL gr, HCDT x, HCDT y, const char *text, const char *font, float size)
{
	if(y->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"Text");	return;	}
	mglData z(y->GetNx());
	z.Fill(gr->Min.z,gr->Min.z);
	mgl_text_xyz(gr,x,y,&z,text,font,size);
}
//-----------------------------------------------------------------------------
void mgl_text_y(HMGL gr, HCDT y, const char *text, const char *font, float size)
{
	if(y->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"Text");	return;	}
	mglData x(y->GetNx()), z(y->GetNx());
	x.Fill(gr->Min.x,gr->Max.x);
	z.Fill(gr->Min.z,gr->Min.z);
	mgl_text_xyz(gr,&x,y,&z,text,font,size);
}
//-----------------------------------------------------------------------------
void mgl_text_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z,const char *text,const char *font, float *size,int l,int n)
{	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,font,n);	f[n]=0;
	mgl_text_xyz(_GR_, _DA_(x),_DA_(y), _DA_(z), s, f, *size);	delete []s;	delete []f;	}
//-----------------------------------------------------------------------------
void mgl_text_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, const char *text, const char *font, float *size, int l,int n)
{	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,font,n);	f[n]=0;
	mgl_text_xy(_GR_, _DA_(x),_DA_(y),s,f,*size);	delete []s;	delete []f;	}
//-----------------------------------------------------------------------------
void mgl_text_y_(uintptr_t *gr, uintptr_t *y, const char *text, const char *font, float *size, int l,int n)
{	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,font,n);	f[n]=0;
	mgl_text_y(_GR_, _DA_(y),s,f,*size);	delete []s;	delete []f;	}
//-----------------------------------------------------------------------------
//
//	Cont series
//
//-----------------------------------------------------------------------------
// NOTE! All data MUST have the same size! Only first slice is used!
void mgl_cont_gen(HMGL gr, float val, HCDT a, HCDT x, HCDT y, HCDT z, mglColor c, int text,long ak)
{
	long n=a->GetNx(), m=a->GetNy();
	if(n<2 || m<2 || x->GetNx()*x->GetNx()!=n*m || y->GetNx()*y->GetNx()!=n*m || z->GetNx()*z->GetNx()!=n*m)
	{	gr->SetWarn(mglWarnDim,"ContGen");	return;	}

	mglPoint *kk = new mglPoint[2*n*m],p;
	float d, kx, ky;
	register long i,j,k, pc=0;
	// NOTE. Usually number of points is much smaller. So, there is no reservation.
//	gr->ReserveC(2*n*m);

	// add intersection point of isoline and Y axis
	for(i=0;i<n-1;i++)	for(j=0;j<m;j++)
	{
		d = mgl_d(val,a->v(i,j,ak),a->v(i+1,j,ak));
		if(d>=0 && d<1)
		{
			p = mglPoint(x->v(i,j)*(1-d)+x->v(i+1,j)*d,
						y->v(i,j)*(1-d)+y->v(i+1,j)*d,
						z->v(i,j)*(1-d)+z->v(i+1,j)*d);
			if(gr->ScalePoint(p))
				kk[pc] = mglPoint(i+d,j,gr->AddPntC(p,c));	pc++;
		}
	}
	// add intersection point of isoline and X axis
	for(i=0;i<n;i++)	for(j=0;j<m-1;j++)
	{
		d = mgl_d(val,a->v(i,j,ak),a->v(i,j+1,ak));
		if(d>=0 && d<1)
		{
			p = mglPoint(x->v(i,j)*(1-d)+x->v(i,j+1)*d,
						y->v(i,j)*(1-d)+y->v(i,j+1)*d,
						z->v(i,j)*(1-d)+z->v(i,j+1)*d);
			if(gr->ScalePoint(p))
				kk[pc] = mglPoint(i,j+d,gr->AddPntC(p,c));	pc++;
		}
	}
	// deallocate arrays and finish if no point
	if(pc==0)	{	free(kk);	return;	}
	// allocate arrays for curve
	long *nn = new long[pc], *ff = new long[pc];
	for(i=0;i<pc;i++)	nn[i] = ff[i] = -1;
	// connect points to line
	j=-1;
	do{
		if(j>=0)
		{
			kx = kk[j].x;	ky = kk[j].y;		i = -1;
			for(k=0;k<pc;k++)	// find closest point in grid
			{
				if(k==j || k==ff[j] || ff[k]!=-1)	continue;	// point is marked
				if(fabs(kk[k].x-kx)<1e-5 && fabs(kk[k].y-ky)<1e-5)	{	i=k;	break;	}
			}
			if(i<0)	j = -1;
			else				// mark the point
			{	nn[j] = i;	ff[i] = j;	j = nn[i]<0 ? i : -1;	}
		}
		if(j<0)
		{
			for(k=0;k<pc;k++)	if(nn[k]==-1)
			{
				if(kk[k].x==0 || fabs(kk[k].x-n+1)<1e-5 || kk[k].y==0 || fabs(kk[k].y-m+1)<1e-5)
				{	nn[k]=-2;	j = k;	break;	}
			}
			if(j<0)	for(k=0;k<pc;k++)	if(nn[k]==-1)
			{	j = k;	nn[k]=-2;	break;	}
		}
	}while(j>=0);
	for(i=0;i<pc;i++)	{	ff[i] = long(0.5+kk[i].z);	}	// return to PntC numbering

	if(text)
	{
		wchar_t wcs[64];
		mglprintf(wcs,64,L"%4.3g",val);
		float size = gr->GetFontSize()/2;
		mglPoint p[25],t;
		float del = gr->TextWidth(wcs)*size/8.;
		del = del>1 ? del:1;
		long k=1,i,j;
		bool less;
		p[0] =	gr->GetPntC(ff[0]);
		mgl_string_curve(gr,0,pc,ff,nn,wcs,size,"t");
		for(i=1;i<pc;i++)	// print it several times (for contours)
		{
			if(nn[i]<0)	continue;
			less = false;
			t = gr->GetPntC(ff[i]);
			for(j=0;j<k;j++)	if(Norm(t-p[j])<del)	{	less=true;	break;	}
			if(less)	continue;
			p[k] = t;	k++;
			mgl_string_curve(gr,i,pc,ff,nn,wcs,size,"t");
			if(k>=25)	break;
		}
	}
	for(i=0;i<pc;i++)	gr->line_plot(ff[i], ff[nn[i]]);
	delete []kk;	delete []nn;	delete []ff;
}
//-----------------------------------------------------------------------------
void mgl_cont_gen(HMGL gr, float val, HCDT a, HCDT x, HCDT y, HCDT z, const char *sch)
{
	bool text=(sch && strchr(sch,'t'));
	gr->SetPenPal(sch);
	mgl_cont_gen(gr,val,a,x,y,z,gr->CDef,text,0);
}
//-----------------------------------------------------------------------------
void mgl_cont_xy_val(HMGL gr, HCDT v, HCDT x, HCDT y, HCDT z, const char *sch, float zVal)
{
	register long i,j,n=z->GetNx(),m=z->GetNy();
	if(x->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"Cont");	return;	}
	if(n<2 || m<2)		{	gr->SetWarn(mglWarnLow,"Cont");	return;	}
	bool both = x->GetNx()*x->GetNy()==m*n && y->GetNx()*y->GetNy()==m*n;
	if(y->GetNx()!=z->GetNy() && !both)	{	gr->SetWarn(mglWarnDim, "Cont");	return;	}
	static int cgid=1;	gr->StartGroup("Cont",cgid++);

	bool text=(sch && strchr(sch,'t'));
	gr->SetPenPal(sch);
	gr->SetScheme(sch);

	mglData xx, yy, zz(z->GetNx(), z->GetNy());
	if(!both)	// make
	{
		xx.Create(z->GetNx(), z->GetNy());
		yy.Create(z->GetNx(), z->GetNy());
		for(i=0;i<n;i++)	for(j=0;j<m;j++)
		{	xx.a[i+n*j] = x->v(i);	yy.a[i+n*j] = y->v(j);	}
		x = &xx;	y = &yy;
	}
	// x, y -- have the same size z
	float z0, v0;
	for(i=0;i<v->GetNx();i++)	for(j=0;j<z->GetNz();j++)
	{
		z0 = z->GetNz()>1 ? gr->Min.z+(gr->Max.z-gr->Min.z)*float(j)/(z->GetNz()-1) : zVal;
		v0 = v->v(i);		if(isnan(zVal))	z0 = v0;
		zz.Fill(z0,z0);
		mgl_cont_gen(gr,v0,z,x,y,&zz,gr->GetC(v0),text,j);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_cont_val(HMGL gr, HCDT v, HCDT z, const char *sch,float zVal)
{
	if(z->GetNx()<2 || z->GetNy()<2)	{	gr->SetWarn(mglWarnLow,"Cont");	return;	}
	mglData x(z->GetNx(), z->GetNy()), y(z->GetNx(), z->GetNy());
	x.Fill(gr->Min.x,gr->Max.x,'x');
	y.Fill(gr->Min.y,gr->Max.y,'y');
	mgl_cont_xy_val(gr,v,&x,&y,z,sch,zVal);
}
//-----------------------------------------------------------------------------
void mgl_cont_xy(HMGL gr, HCDT x, HCDT y, HCDT z, const char *sch, int Num, float zVal)
{
	if(Num<1)	{	gr->SetWarn(mglWarnCnt,"Cont");	return;	}
	mglData v(Num);
	for(long i=0;i<Num;i++)	v.a[i] = gr->Min.c + (gr->Max.c-gr->Min.c)*float(i+1)/(Num+1);
	mgl_cont_xy_val(gr,&v,x,y,z,sch,zVal);
}
//-----------------------------------------------------------------------------
void mgl_cont(HMGL gr, HCDT z, const char *sch, int Num, float zVal)
{
	if(Num<1)	{	gr->SetWarn(mglWarnCnt,"Cont");	return;	}
	mglData v(Num);
	for(long i=0;i<Num;i++)	v.a[i] = gr->Min.c + (gr->Max.c-gr->Min.c)*float(i+1)/(Num+1);
	mgl_cont_val(gr,&v,z,sch,zVal);
}
//-----------------------------------------------------------------------------
void mgl_cont_xy_val_(uintptr_t *gr, uintptr_t *v, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch, float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_cont_xy_val(_GR_, _DA_(v), _DA_(x), _DA_(y), _DA_(a), s, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_cont_val_(uintptr_t *gr, uintptr_t *v, uintptr_t *a, const char *sch,float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_cont_val(_GR_, _DA_(v), _DA_(a), s, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_cont_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch, int *Num, float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_cont_xy(_GR_, _DA_(x), _DA_(y), _DA_(a), s, *Num, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_cont_(uintptr_t *gr, uintptr_t *a, const char *sch, int *Num, float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_cont(_GR_, _DA_(a), s, *Num, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	ContF series
//
//-----------------------------------------------------------------------------
inline long mgl_add_pnt(HMGL gr, float d, HCDT x, HCDT y, HCDT z, long i1, long j1, long i2, long j2, mglColor c)
{
	long res=-1;
	if(d>0 && d<1)
	{
		mglPoint p,u,v;
		p = mglPoint(x->v(i1,j1)*(1-d)+x->v(i2,j2)*d,
					 y->v(i1,j1)*(1-d)+y->v(i2,j2)*d,
					 z->v(i1,j1)*(1-d)+z->v(i2,j2)*d);
		u = mglPoint(x->dvx(i1,j1)*(1-d)+x->dvx(i2,j2)*d,
					 y->dvx(i1,j1)*(1-d)+y->dvx(i2,j2)*d,
					 z->dvx(i1,j1)*(1-d)+z->dvx(i2,j2)*d);
		v = mglPoint(x->dvy(i1,j1)*(1-d)+x->dvy(i2,j2)*d,
					 y->dvy(i1,j1)*(1-d)+y->dvy(i2,j2)*d,
					 z->dvy(i1,j1)*(1-d)+z->dvy(i2,j2)*d);
		if(gr->ScalePoint(p))	res = gr->AddPntN(p,c,u^v);
	}
	return res;
}
//-----------------------------------------------------------------------------
void mgl_contf_gen(HMGL gr, float v1, float v2, HCDT a, HCDT x, HCDT y, HCDT z, mglColor c, long ak)
{
	long n=a->GetNx(), m=a->GetNy();
	if(n<2 || m<2 || x->GetNx()*x->GetNx()!=n*m || y->GetNx()*y->GetNx()!=n*m || z->GetNx()*z->GetNx()!=n*m)
	{	gr->SetWarn(mglWarnDim,"ContFGen");	return;	}

	float d1, d2;
	register long i,j;
	gr->ReserveN(n*m);
	long *kk = new long[2*n], l1,l2, r1,r2, t1,t2, u1,u2, pos[8],num;
	memset(kk,-1,2*n*sizeof(long));
	float f1,f2;
	for(i=0;i<n-1;i++)	// add intersection points for first line
	{
		f1 = a->v(i,0,ak);		f2 = a->v(i+1,0,ak);
		d1 = mgl_d(v1,f1,f2);
		u1 = mgl_add_pnt(gr,d1,x,y,z,i,0,i+1,0,c);
		if(u1<0 && f1>=v2 && f1<=v1)
		{	d1=0;	u1 = mgl_add_pnt(gr,0,x,y,z,i,0,i,0,c);	}
		d2 = mgl_d(v2,f1,f2);
		u2 = mgl_add_pnt(gr,d1,x,y,z,i,0,i+1,0,c);
		if(u2<0 && f2>=v2 && f2<=v1)
		{	d2=1;	u2 = mgl_add_pnt(gr,0,x,y,z,i+1,0,i+1,0,c);	}
		if(d1<d2)	{	kk[2*i]=u1;	kk[2*i+1]=u2;	}
		else		{	kk[2*i]=u2;	kk[2*i+1]=u1;	}
	}
	for(j=1;j<m;j++)	// add intersection points
	{
		f1 = a->v(i,j-1,ak);	f2 = a->v(i,j,ak);
		d1 = mgl_d(v1,f1,f2);	//	left edge
		r1 = mgl_add_pnt(gr,d1,x,y,z,i,j-1,i,j,c);
		d2 = mgl_d(v2,f1,f2);
		r2 = mgl_add_pnt(gr,d1,x,y,z,i,j-1,i,j,c);
		if(d1>d2)	{	u1=r1;	r1=r2;	r2=u1;	}
		for(i=0;i<n-1;i++)
		{
			l1 = r1;		l2 = r2;	num=0;
			t1 = kk[2*i];	t2 = kk[2*i+1];
			// right edge
			f1 = a->v(i+1,j-1,ak);	f2 = a->v(i+1,j,ak);
			d1 = mgl_d(v1,f1,f2);
			r1 = mgl_add_pnt(gr,d1,x,y,z,i+1,j-1,i+1,j,c);
			d2 = mgl_d(v2,f1,f2);
			r2 = mgl_add_pnt(gr,d1,x,y,z,i+1,j-1,i+1,j,c);
			if(d1>d2)	{	u1=r1;	r1=r2;	r2=u1;	}
			// top edge
			f1 = a->v(i,j,ak);		//f2 = a->v(i+1,j,ak);	// f2 is the same
			d1 = mgl_d(v1,f1,f2);
			u1 = mgl_add_pnt(gr,d1,x,y,z,i,j,i+1,j,c);
			if(u1<0 && f1>=v2 && f1<=v1)
			{	d1=0;	u1 = mgl_add_pnt(gr,0,x,y,z,i,j,i,j,c);	}
			d2 = mgl_d(v2,f1,f2);
			u2 = mgl_add_pnt(gr,d1,x,y,z,i,j,i+1,j,c);
			if(u2<0 && f2>=v2 && f2<=v1)
			{	d2=1;	u2 = mgl_add_pnt(gr,0,x,y,z,i+1,j,i+1,j,c);	}
			if(d1<d2)	{	kk[2*i]=u1;	kk[2*i+1]=u2;	}
			else		{	kk[2*i]=u2;	kk[2*i+1]=u1;	}
			// collect points
			if(t1>=0)	pos[num++] = t1;	if(t2>=0)	pos[num++] = t2;
			if(r1>=0)	pos[num++] = r1;	if(r2>=0)	pos[num++] = r2;
			if(u2>=0)	pos[num++] = u2;	if(u1>=0)	pos[num++] = u1;
			if(l2>=0)	pos[num++] = l2;	if(l1>=0)	pos[num++] = l1;
			// draw it
			if(num==4)	gr->quad_plot(pos[0],pos[1],pos[2],pos[3]);
			else if(num==3)	gr->trig_plot(pos[0],pos[1],pos[2]);
			else if(num==5)
			{
				gr->quad_plot(pos[0],pos[1],pos[2],pos[3]);
				gr->trig_plot(pos[0],pos[3],pos[4]);
			}
			else if(num==6)
			{
				gr->quad_plot(pos[0],pos[1],pos[2],pos[3]);
				gr->quad_plot(pos[0],pos[3],pos[4],pos[5]);
			}
		}
	}
	delete []kk;
}
//-----------------------------------------------------------------------------
void mgl_contf_gen(HMGL gr, float v1, float v2, HCDT a, HCDT x, HCDT y, HCDT z, const char *c)
{
	gr->SetPenPal(c);	mgl_contf_gen(gr,v1,v2,a,x,y,z,gr->CDef,0);
}
//-----------------------------------------------------------------------------
void mgl_contf_xy_val(HMGL gr, HCDT v, HCDT x, HCDT y, HCDT z, const char *sch, float zVal)
{
	register long i,j,n=z->GetNx(),m=z->GetNy();
	if(x->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"ContF");	return;	}
	if(n<2 || m<2)		{	gr->SetWarn(mglWarnLow,"ContF");	return;	}
	bool both = x->GetNx()*x->GetNy()==m*n && y->GetNx()*y->GetNy()==m*n;
	if(y->GetNx()!=z->GetNy() && !both)	{	gr->SetWarn(mglWarnDim, "ContF");	return;	}
	static int cgid=1;	gr->StartGroup("ContF",cgid++);
	gr->SetScheme(sch);

	mglData xx, yy, zz(z->GetNx(), z->GetNy());
	if(!both)	// make
	{
		xx.Create(z->GetNx(), z->GetNy());
		yy.Create(z->GetNx(), z->GetNy());
		for(i=0;i<n;i++)	for(j=0;j<m;j++)
		{	xx.a[i+n*j] = x->v(i);	yy.a[i+n*j] = y->v(j);	}
		x = &xx;	y = &yy;
	}
	// x, y -- have the same size z
	float z0, v0;
	for(i=0;i<v->GetNx()-1;i++)	for(j=0;j<z->GetNz();j++)
	{
		z0 = z->GetNz()>1 ? gr->Min.z+(gr->Max.z-gr->Min.z)*float(j)/(z->GetNz()-1) : zVal;
		v0 = v->v(i);		if(isnan(zVal))	z0 = v0;
		zz.Fill(z0,z0);
		mgl_contf_gen(gr,v0,v->v(i+1),z,x,y,&zz,gr->GetC(v0),j);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_contf_val(HMGL gr, HCDT v, HCDT z, const char *sch,float zVal)
{
	if(z->GetNx()<2 || z->GetNy()<2)	{	gr->SetWarn(mglWarnLow,"Cont");	return;	}
	mglData x(z->GetNx(), z->GetNy()), y(z->GetNx(), z->GetNy());
	x.Fill(gr->Min.x,gr->Max.x,'x');
	y.Fill(gr->Min.y,gr->Max.y,'y');
	mgl_contf_xy_val(gr,v,&x,&y,z,sch,zVal);
}
//-----------------------------------------------------------------------------
void mgl_contf_xy(HMGL gr, HCDT x, HCDT y, HCDT z, const char *sch, int Num, float zVal)
{
	if(Num<1)	{	gr->SetWarn(mglWarnCnt,"Cont");	return;	}
	mglData v(Num);	v.Fill(gr->Min.c, gr->Max.c);
	mgl_contf_xy_val(gr,&v,x,y,z,sch,zVal);
}
//-----------------------------------------------------------------------------
void mgl_contf(HMGL gr, HCDT z, const char *sch, int Num, float zVal)
{
	if(Num<1)	{	gr->SetWarn(mglWarnCnt,"Cont");	return;	}
	mglData v(Num);	v.Fill(gr->Min.c, gr->Max.c);
	mgl_contf_val(gr,&v,z,sch,zVal);
}
//-----------------------------------------------------------------------------
void mgl_contf_xy_val_(uintptr_t *gr, uintptr_t *v, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch, float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_contf_xy_val(_GR_, _DA_(v), _DA_(x), _DA_(y), _DA_(a), s, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_contf_val_(uintptr_t *gr, uintptr_t *v, uintptr_t *a, const char *sch,float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_contf_val(_GR_, _DA_(v), _DA_(a), s, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_contf_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch, int *Num, float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_contf_xy(_GR_, _DA_(x), _DA_(y), _DA_(a), s, *Num, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_contf_(uintptr_t *gr, uintptr_t *a, const char *sch, int *Num, float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_contf(_GR_, _DA_(a), s, *Num, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	ContD series
//
//-----------------------------------------------------------------------------
int mgl_get_ncol(const char *sch, char *res)
{
	int i,j,n=strlen(sch);
	for(i=j=0;i<n;i++)	if(mglColor(sch[i]).Valid())
	{	if(res)	res[j]=sch[i];	j++;	}
	return j;
}
//-----------------------------------------------------------------------------
void mgl_contd_xy_val(HMGL gr, HCDT v, HCDT x, HCDT y, HCDT z, const char *sch, float zVal)
{
	register long i,j,n=z->GetNx(),m=z->GetNy();
	if(x->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"ContD");	return;	}
	if(n<2 || m<2)		{	gr->SetWarn(mglWarnLow,"ContD");	return;	}
	bool both = x->GetNx()*x->GetNy()==m*n && y->GetNx()*y->GetNy()==m*n;
	if(y->GetNx()!=z->GetNy() && !both)	{	gr->SetWarn(mglWarnDim, "ContD");	return;	}
	static int cgid=1;	gr->StartGroup("ContD",cgid++);
	if(!sch || !sch[0])	sch = MGL_DEF_PAL;
	char *cc = new char[strlen(sch)];
	int nc = mgl_get_ncol(sch,cc);

	mglData xx, yy, zz(z->GetNx(), z->GetNy());
	if(!both)	// make
	{
		xx.Create(z->GetNx(), z->GetNy());
		yy.Create(z->GetNx(), z->GetNy());
		for(i=0;i<n;i++)	for(j=0;j<m;j++)
		{	xx.a[i+n*j] = x->v(i);	yy.a[i+n*j] = y->v(j);	}
		x = &xx;	y = &yy;
	}
	// x, y -- have the same size z
	float z0, v0;
	for(i=0;i<v->GetNx()-1;i++)	for(j=0;j<z->GetNz();j++)
	{
		z0 = z->GetNz()>1 ? gr->Min.z+(gr->Max.z-gr->Min.z)*float(j)/(z->GetNz()-1) : zVal;
		v0 = v->v(i);		if(isnan(zVal))	z0 = v0;
		zz.Fill(z0,z0);
		mgl_contf_gen(gr,v0,v->v(i+1),z,x,y,&zz,mglColor(cc[i%nc]),j);
	}
	gr->EndGroup();	delete []cc;
}
//-----------------------------------------------------------------------------
void mgl_contd_val(HMGL gr, HCDT v, HCDT z, const char *sch,float zVal)
{
	if(z->GetNx()<2 || z->GetNy()<2)	{	gr->SetWarn(mglWarnLow,"ContD");	return;	}
	mglData x(z->GetNx(), z->GetNy()), y(z->GetNx(), z->GetNy());
	x.Fill(gr->Min.x,gr->Max.x,'x');
	y.Fill(gr->Min.y,gr->Max.y,'y');
	mgl_contd_xy_val(gr,v,&x,&y,z,sch,zVal);
}
//-----------------------------------------------------------------------------
void mgl_contd_xy(HMGL gr, HCDT x, HCDT y, HCDT z, const char *sch, float zVal)
{
	if(!sch || !sch[0])	sch = MGL_DEF_PAL;
	mglData v(mgl_get_ncol(sch,0)+1);
	v.Fill(gr->Min.c, gr->Max.c);
	mgl_contd_xy_val(gr,&v,x,y,z,sch,zVal);
}
//-----------------------------------------------------------------------------
void mgl_contd(HMGL gr, HCDT z, const char *sch, float zVal)
{
	if(!sch || !sch[0])	sch = MGL_DEF_PAL;
	mglData v(mgl_get_ncol(sch,0)+1);
	v.Fill(gr->Min.c, gr->Max.c);
	mgl_contd_val(gr,&v,z,sch,zVal);
}
//-----------------------------------------------------------------------------
void mgl_contd_xy_val_(uintptr_t *gr, uintptr_t *v, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch, float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_contd_xy_val(_GR_, _DA_(v), _DA_(x), _DA_(y), _DA_(a), s, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_contd_val_(uintptr_t *gr, uintptr_t *v, uintptr_t *a, const char *sch,float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_contd_val(_GR_, _DA_(v), _DA_(a), s, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_contd_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch, float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_contd_xy(_GR_, _DA_(x), _DA_(y), _DA_(a), s, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_contd_(uintptr_t *gr, uintptr_t *a, const char *sch, float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_contd(_GR_, _DA_(a), s, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Cont3 series
//
//-----------------------------------------------------------------------------
struct _mgl_slice	{	mglData x,y,z,a;	};
//-----------------------------------------------------------------------------
void mgl_get_slice(_mgl_slice &s, HCDT x, HCDT y, HCDT z, HCDT a, char dir, float d, bool both)
{
	register long i,j,i0,n=a->GetNx(),m=a->GetNy(),l=a->GetNz(), nx=1,ny=1,p;

	switch(dir)		// îáùàÿ ÷àñòü
	{
	case 'x':	nx = m;	ny = l;	if(d<0)	d = n/2;	break;
	case 'y':	nx = n;	ny = l;	if(d<0)	d = m/2;	break;
	case 'z':	nx = n;	ny = m;	if(d<0)	d = l/2;	break;
	}
	s.x.Create(nx,ny);	s.y.Create(nx,ny);
	s.z.Create(nx,ny);	s.a.Create(nx,ny);
	p = long(d);	d -= p;
	float v;

	if(both)
	{
		switch(dir)
		{
		case 'x':
			for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
			{
				i0 = i+nx*j;
				s.x.a[i0] = x->v(p,i,j)*(1-d) + x->v(p+1,i,j)*d;
				s.y.a[i0] = y->v(p,i,j)*(1-d) + y->v(p+1,i,j)*d;
				s.z.a[i0] = z->v(p,i,j)*(1-d) + z->v(p+1,i,j)*d;
				s.a.a[i0] = a->v(p,i,j)*(1-d) + a->v(p+1,i,j)*d;
			}
			break;
		case 'y':
			for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
			{
				i0 = i+nx*j;
				s.x.a[i0] = x->v(i,p,j)*(1-d) + x->v(i,p+1,j)*d;
				s.y.a[i0] = y->v(i,p,j)*(1-d) + y->v(i,p+1,j)*d;
				s.z.a[i0] = z->v(i,p,j)*(1-d) + z->v(i,p+1,j)*d;
				s.a.a[i0] = a->v(i,p,j)*(1-d) + a->v(i,p+1,j)*d;
			}
			break;
		case 'z':
			for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
			{
				i0 = i+nx*j;
				s.x.a[i0] = x->v(i,j,p)*(1-d) + x->v(i,j,p+1)*d;
				s.y.a[i0] = y->v(i,j,p)*(1-d) + y->v(i,j,p+1)*d;
				s.z.a[i0] = z->v(i,j,p)*(1-d) + z->v(i,j,p+1)*d;
				s.a.a[i0] = a->v(i,j,p)*(1-d) + a->v(i,j,p+1)*d;
			}
			break;
		}
	}
	else	// x, y, z -- vectors
	{
		switch(dir)
		{
		case 'x':
			v = x->v(p)*(1-d)+x->v(p+1)*d;
			for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
			{
				i0 = i+nx*j;	s.x.a[i0] = v;
				s.y.a[i0] = y->v(i);	s.z.a[i0] = z->v(j);
				s.a.a[i0] = a->v(p,i,j)*(1-d) + a->v(p+1,i,j)*d;
			}
			break;
		case 'y':
			v = y->v(p)*(1-d)+y->v(p+1)*d;
			for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
			{
				i0 = i+nx*j;	s.y.a[i0] = v;
				s.x.a[i0] = x->v(i);	s.z.a[i0] = z->v(j);
				s.a.a[i0] = a->v(i,p,j)*(1-d) + a->v(i,p+1,j)*d;
			}
			break;
		case 'z':
			v = z->v(p)*(1-d)+z->v(p+1)*d;
			for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
			{
				i0 = i+nx*j;	s.z.a[i0] = v;
				s.x.a[i0] = x->v(i);	s.y.a[i0] = y->v(j);
				s.a.a[i0] = a->v(i,j,p)*(1-d) + a->v(i,j,p+1)*d;
			}
			break;
		}
	}
}
//-----------------------------------------------------------------------------
void mgl_cont3_xyz_val(HMGL gr, HCDT v, HCDT x, HCDT y, HCDT z, HCDT a, char dir, float sVal, const char *sch)
{
	long n=a->GetNx(),m=a->GetNy(),l=a->GetNz();
	if(n<2 || m<2 || l<2)	{	gr->SetWarn(mglWarnLow,"Cont3");	return;	}
	bool both = x->GetNx()*x->GetNy()*x->GetNz()==n*m*l && y->GetNx()*y->GetNy()*y->GetNz()==n*m*l && z->GetNx()*z->GetNy()*z->GetNz()==n*m*l;
	if(!(both || (x->GetNx()==n && y->GetNx()==m && z->GetNx()==l)))
	{	gr->SetWarn(mglWarnDim,"Cont3");	return;	}
	static int cgid=1;	gr->StartGroup("Cont3",cgid++);

	bool text=(sch && strchr(sch,'t'));
	gr->SetPenPal(sch);
	gr->SetScheme(sch);

	_mgl_slice s;
	mgl_get_slice(s,x,y,z,a,dir,sVal,both);
	for(long i=0;i<v->GetNx();i++)
	{
		float v0 = v->v(i);
		mgl_cont_gen(gr,v0,&s.a,&s.x,&s.y,&s.z,gr->GetC(v0),text,0);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_cont3_val(HMGL gr, HCDT v, HCDT a, char dir, float sVal, const char *sch)
{
	if(a->GetNx()<2 || a->GetNy()<2 || a->GetNz()<2)
	{	gr->SetWarn(mglWarnLow,"Cont3");	return;	}
	mglData x(a->GetNx()), y(a->GetNy()),z(a->GetNz());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	z.Fill(gr->Min.z,gr->Max.z);
	mgl_cont3_xyz_val(gr,v,&x,&y,&z,a,dir,sVal,sch);
}
//-----------------------------------------------------------------------------
void mgl_cont3_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT a, char dir, float sVal, const char *sch, int Num)
{
	if(Num<1)	{	gr->SetWarn(mglWarnCnt,"Cont3");	return;	}
	mglData v(Num);
	for(long i=0;i<Num;i++)	v.a[i] = gr->Min.c + (gr->Max.c-gr->Min.c)*float(i+1)/(Num+1);
	mgl_cont3_xyz_val(gr,&v,x,y,z,a,dir,sVal,sch);
}
//-----------------------------------------------------------------------------
void mgl_cont3(HMGL gr, HCDT a, char dir, float sVal, const char *sch, int Num)
{
	if(Num<1)	{	gr->SetWarn(mglWarnCnt,"Cont3");	return;	}
	mglData v(Num);
	for(long i=0;i<Num;i++)	v.a[i] = gr->Min.c + (gr->Max.c-gr->Min.c)*float(i+1)/(Num+1);
	mgl_cont3_val(gr,&v,a,dir,sVal,sch);
}
//-----------------------------------------------------------------------------
void mgl_cont3_xyz_val_(uintptr_t *gr, uintptr_t *v, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *dir, float *sVal, const char *sch,int,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_cont3_xyz_val(_GR_, _DA_(v), _DA_(x), _DA_(y), _DA_(z), _DA_(a), *dir, *sVal, s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_cont3_val_(uintptr_t *gr, uintptr_t *v, uintptr_t *a, const char *dir, float *sVal, const char *sch,int,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_cont3_val(_GR_, _DA_(v), _DA_(a), *dir, *sVal, s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_cont3_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *dir, float *sVal, const char *sch, int *Num,int,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_cont3_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(a), *dir, *sVal, s, *Num);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_cont3_(uintptr_t *gr, uintptr_t *a, const char *dir, float *sVal, const char *sch, int *Num,int,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_cont3(_GR_, _DA_(a), *dir, *sVal, s, *Num);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Dens3 series
//
//-----------------------------------------------------------------------------
void mgl_dens3_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT a, char dir, float sVal, const char *sch)
{
	long n=a->GetNx(),m=a->GetNy(),l=a->GetNz();
	if(n<2 || m<2 || l<2)	{	gr->SetWarn(mglWarnLow,"Dens3");	return;	}
	bool both = x->GetNx()*x->GetNy()*x->GetNz()==n*m*l && y->GetNx()*y->GetNy()*y->GetNz()==n*m*l && z->GetNx()*z->GetNy()*z->GetNz()==n*m*l;
	if(!(both || (x->GetNx()==n && y->GetNx()==m && z->GetNx()==l)))
	{	gr->SetWarn(mglWarnDim,"Dens3");	return;	}
	static int cgid=1;	gr->StartGroup("Dens3",cgid++);

	_mgl_slice s;
	mgl_get_slice(s,x,y,z,a,dir,sVal,both);
	mgl_surfc_xy(gr,&s.x,&s.y,&s.z,&s.a,sch);
}
//-----------------------------------------------------------------------------
void mgl_dens3(HMGL gr, HCDT a, char dir, float sVal, const char *sch)
{
	if(a->GetNx()<2 || a->GetNy()<2 || a->GetNz()<2)
	{	gr->SetWarn(mglWarnLow,"Dens3");	return;	}
	mglData x(a->GetNx()), y(a->GetNy()),z(a->GetNz());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	z.Fill(gr->Min.z,gr->Max.z);
	mgl_dens3_xyz(gr,&x,&y,&z,a,dir,sVal,sch);
}
//-----------------------------------------------------------------------------
void mgl_dens3_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *dir, float *sVal, const char *sch,int,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_dens3_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(a), *dir, *sVal, s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_dens3_(uintptr_t *gr, uintptr_t *a, const char *dir, float *sVal, const char *sch,int,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_dens3(_GR_, _DA_(a), *dir, *sVal, s);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Grid3 series
//
//-----------------------------------------------------------------------------
void mgl_grid3_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT a, char dir, float sVal, const char *sch)
{
	long n=a->GetNx(),m=a->GetNy(),l=a->GetNz();
	if(n<2 || m<2 || l<2)	{	gr->SetWarn(mglWarnLow,"Grid3");	return;	}
	bool both = x->GetNx()*x->GetNy()*x->GetNz()==n*m*l && y->GetNx()*y->GetNy()*y->GetNz()==n*m*l && z->GetNx()*z->GetNy()*z->GetNz()==n*m*l;
	if(!(both || (x->GetNx()==n && y->GetNx()==m && z->GetNx()==l)))
	{	gr->SetWarn(mglWarnDim,"Grid3");	return;	}
	static int cgid=1;	gr->StartGroup("Grid3",cgid++);

	_mgl_slice s;
	mgl_get_slice(s,x,y,z,a,dir,sVal,both);
	mgl_mesh_xy(gr,&s.x,&s.y,&s.z,sch);
}
//-----------------------------------------------------------------------------
void mgl_grid3(HMGL gr, HCDT a, char dir, float sVal, const char *sch)
{
	if(a->GetNx()<2 || a->GetNy()<2 || a->GetNz()<2)
	{	gr->SetWarn(mglWarnLow,"Grid3");	return;	}
	mglData x(a->GetNx()), y(a->GetNy()), z(a->GetNz());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	z.Fill(gr->Min.z,gr->Max.z);
	mgl_grid3_xyz(gr,&x,&y,&z,a,dir,sVal,sch);
}
//-----------------------------------------------------------------------------
void mgl_grid3_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *dir, float *sVal, const char *sch,int,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_grid3_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(a), *dir, *sVal, s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_grid3_(uintptr_t *gr, uintptr_t *a, const char *dir, float *sVal, const char *sch,int,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_grid3(_GR_, _DA_(a), *dir, *sVal, s);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	ContF3 series
//
//-----------------------------------------------------------------------------
void mgl_contf3_xyz_val(HMGL gr, HCDT v, HCDT x, HCDT y, HCDT z, HCDT a, char dir, float sVal, const char *sch)
{
	long n=a->GetNx(),m=a->GetNy(),l=a->GetNz();
	if(n<2 || m<2 || l<2)	{	gr->SetWarn(mglWarnLow,"ContF3");	return;	}
	bool both = x->GetNx()*x->GetNy()*x->GetNz()==n*m*l && y->GetNx()*y->GetNy()*y->GetNz()==n*m*l && z->GetNx()*z->GetNy()*z->GetNz()==n*m*l;
	if(!(both || (x->GetNx()==n && y->GetNx()==m && z->GetNx()==l)))
	{	gr->SetWarn(mglWarnDim,"ContF3");	return;	}
	static int cgid=1;	gr->StartGroup("ContF3",cgid++);

	gr->SetScheme(sch);
	_mgl_slice s;
	mgl_get_slice(s,x,y,z,a,dir,sVal,both);
	for(long i=0;i<v->GetNx();i++)
	{
		float v0 = v->v(i);
		mgl_contf_gen(gr,v0,v->v(i+1),&s.a,&s.x,&s.y,&s.z,gr->GetC(v0),0);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_contf3_val(HMGL gr, HCDT v, HCDT a, char dir, float sVal, const char *sch)
{
	if(a->GetNx()<2 || a->GetNy()<2 || a->GetNz()<2)
	{	gr->SetWarn(mglWarnLow,"ContF3");	return;	}
	mglData x(a->GetNx()), y(a->GetNy()),z(a->GetNz());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	z.Fill(gr->Min.z,gr->Max.z);
	mgl_contf3_xyz_val(gr,v,&x,&y,&z,a,dir,sVal,sch);
}
//-----------------------------------------------------------------------------
void mgl_contf3_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT a, char dir, float sVal, const char *sch, int Num)
{
	if(Num<1)	{	gr->SetWarn(mglWarnCnt,"ContF3");	return;	}
	mglData v(Num+2);
	for(long i=0;i<Num+2;i++)	v.a[i] = gr->Min.c + (gr->Max.c-gr->Min.c)*float(i)/(Num+1);
	mgl_contf3_xyz_val(gr,&v,x,y,z,a,dir,sVal,sch);
}
//-----------------------------------------------------------------------------
void mgl_contf3(HMGL gr, HCDT a, char dir, float sVal, const char *sch, int Num)
{
	if(Num<1)	{	gr->SetWarn(mglWarnCnt,"ContF3");	return;	}
	mglData v(Num+2);	v.Fill(gr->Min.c, gr->Max.c);
	mgl_contf3_val(gr,&v,a,dir,sVal,sch);
}
//-----------------------------------------------------------------------------
void mgl_contf3_xyz_val_(uintptr_t *gr, uintptr_t *v, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *dir, float *sVal, const char *sch,int,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_contf3_xyz_val(_GR_, _DA_(v), _DA_(x), _DA_(y), _DA_(z), _DA_(a), *dir, *sVal, s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_contf3_val_(uintptr_t *gr, uintptr_t *v, uintptr_t *a, const char *dir, float *sVal, const char *sch,int,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_contf3_val(_GR_, _DA_(v), _DA_(a), *dir, *sVal, s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_contf3_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *dir, float *sVal, const char *sch, int *Num,int,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_contf3_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(a), *dir, *sVal, s, *Num);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_contf3_(uintptr_t *gr, uintptr_t *a, const char *dir, float *sVal, const char *sch, int *Num,int,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_contf3(_GR_, _DA_(a), *dir, *sVal, s, *Num);
	delete []s;
}
//-----------------------------------------------------------------------------
//
//	Axial series
//
//-----------------------------------------------------------------------------
long mgl_find_prev(long i, long pc, long *nn)
{
	for(long k=0;k<pc;k++)	if(nn[k]==i)	return k;
	return -1;
}
void mgl_axial_plot(mglBase *gr,long pc, mglPoint *ff, long *nn,char dir,mglColor cc,bool wire)
{
	mglPoint a(0,0,1),b,c,p,q1,q2;
	if(dir=='x')	a = mglPoint(1,0,0);
	if(dir=='y')	a = mglPoint(0,1,0);
	b = !a;	c = a^b;

	register long i,j,k;
	float fi,si,co;
	long p1,p2,p3,p4;
	gr->ReserveN(pc*82);
	for(i=0;i<pc;i++)
	{
		k = mgl_find_prev(i,pc,nn);
		if(nn[i]<0)	continue;
		q1 = k<0 ? ff[nn[i]]-ff[i]  : (ff[nn[i]]-ff[k])*0.5;
		q2 = nn[nn[i]]<0 ? ff[nn[i]]-ff[i]  : (ff[nn[nn[i]]]-ff[i])*0.5;

		p = a*ff[i].y + c*ff[i].x;			gr->ScalePoint(p);	p1 = gr->AddPntN(p,cc,q1^b);
		p = a*ff[nn[i]].y + c*ff[nn[i]].x;	gr->ScalePoint(p);	p2 = gr->AddPntN(p,cc,q2^b);
		if(wire)	gr->line_plot(p1,p2,true);

		for(j=1;j<41;j++)
		{
			p3 = p1;	p4 = p2;
			fi = j*M_PI/40;		si = sin(fi);	co = cos(fi);
			p = a*ff[i].y + b*(si*ff[i].x) +  c*(co*ff[i].x);
			gr->ScalePoint(p);	p1 = gr->AddPntN(p,cc,q1^(b*co-c*si));
			p = a*ff[nn[i]].y + b*(si*ff[nn[i]].x) +  c*(co*ff[nn[i]].x);
			gr->ScalePoint(p);	p2 = gr->AddPntN(p,cc,q2^(b*co-c*si));
			if(wire)
			{
				gr->line_plot(p1,p2,true);
				gr->line_plot(p3,p1,true);
				gr->line_plot(p4,p2,true);
			}
			else	gr->quad_plot(p3,p4,p1,p2);
		}
	}
}
//-----------------------------------------------------------------------------
// NOTE! All data MUST have the same size! Only first slice is used!
void mgl_axial_gen(HMGL gr, float val, HCDT a, HCDT x, HCDT y, mglColor c, char dir,long ak,bool wire)
{
	long n=a->GetNx(), m=a->GetNy();
	if(n<2 || m<2 || x->GetNx()*x->GetNx()!=n*m || y->GetNx()*y->GetNx()!=n*m)
	{	gr->SetWarn(mglWarnDim,"ContGen");	return;	}

	mglPoint *kk = new mglPoint[2*n*m],*pp = new mglPoint[2*n*m],p;
	float d, kx, ky;
	register long i,j,k, pc=0;
	// NOTE. Usually number of points is much smaller. So, there is no reservation.
//	gr->ReserveC(2*n*m);

	// add intersection point of isoline and Y axis
	for(i=0;i<n-1;i++)	for(j=0;j<m;j++)
	{
		d = mgl_d(val,a->v(i,j,ak),a->v(i+1,j,ak));
		if(d>=0 && d<1)
		{
			pp[pc] = mglPoint(x->v(i,j)*(1-d)+x->v(i+1,j)*d,
						y->v(i,j)*(1-d)+y->v(i+1,j)*d);
			kk[pc] = mglPoint(i+d,j);	pc++;
		}
	}
	// add intersection point of isoline and X axis
	for(i=0;i<n;i++)	for(j=0;j<m-1;j++)
	{
		d = mgl_d(val,a->v(i,j,ak),a->v(i,j+1,ak));
		if(d>=0 && d<1)
		{
			pp[pc] = mglPoint(x->v(i,j)*(1-d)+x->v(i,j+1)*d,
						y->v(i,j)*(1-d)+y->v(i,j+1)*d);
			kk[pc] = mglPoint(i,j+d);	pc++;
		}
	}
	// deallocate arrays and finish if no point
	if(pc==0)	{	free(kk);	return;	}
	// allocate arrays for curve
	long *nn = new long[pc], *ff = new long[pc];
	for(i=0;i<pc;i++)	nn[i] = ff[i] = -1;
	// connect points to line
	j=-1;
	do{
		if(j>=0)
		{
			kx = kk[j].x;	ky = kk[j].y;		i = -1;
			for(k=0;k<pc;k++)	// find closest point in grid
			{
				if(k==j || k==ff[j] || ff[k]!=-1)	continue;	// point is marked
				if(fabs(kk[k].x-kx)<1e-5 && fabs(kk[k].y-ky)<1e-5)	{	i=k;	break;	}
			}
			if(i<0)	j = -1;
			else				// mark the point
			{	nn[j] = i;	ff[i] = j;	j = nn[i]<0 ? i : -1;	}
		}
		if(j<0)
		{
			for(k=0;k<pc;k++)	if(nn[k]==-1)
			{
				if(kk[k].x==0 || fabs(kk[k].x-n+1)<1e-5 || kk[k].y==0 || fabs(kk[k].y-m+1)<1e-5)
				{	nn[k]=-2;	j = k;	break;	}
			}
			if(j<0)	for(k=0;k<pc;k++)	if(nn[k]==-1)
			{	j = k;	nn[k]=-2;	break;	}
		}
	}while(j>=0);
	mgl_axial_plot(gr,pc,pp,nn,dir,c,wire);
	delete []kk;	delete []nn;	delete []ff;	delete []pp;
}
//-----------------------------------------------------------------------------
void mgl_axial_xy_val(HMGL gr, HCDT v, HCDT x, HCDT y, HCDT z, const char *sch)
{
	register long i,j,n=z->GetNx(),m=z->GetNy();
	if(x->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"Axial");	return;	}
	if(n<2 || m<2)		{	gr->SetWarn(mglWarnLow,"Axial");	return;	}
	bool both = x->GetNx()*x->GetNy()==m*n && y->GetNx()*y->GetNy()==m*n;
	if(y->GetNx()!=z->GetNy() && !both)	{	gr->SetWarn(mglWarnDim, "Axial");	return;	}
	static int cgid=1;	gr->StartGroup("Axial",cgid++);

	gr->SetScheme(sch);
	char dir='y';
	if(sch)
	{
		if(strchr(sch,'x'))	dir = 'x';
		if(strchr(sch,'z'))	dir = 'z';
	}

	mglData xx, yy;
	if(!both)	// make
	{
		xx.Create(z->GetNx(), z->GetNy());
		yy.Create(z->GetNx(), z->GetNy());
		for(i=0;i<n;i++)	for(j=0;j<m;j++)
		{	xx.a[i+n*j] = x->v(i);	yy.a[i+n*j] = y->v(j);	}
		x = &xx;	y = &yy;
	}
	// x, y -- have the same size z
	float v0;
	bool wire = sch && strchr(sch,'#');
	for(i=0;i<v->GetNx();i++)	for(j=0;j<z->GetNz();j++)
	{
		v0 = v->v(i);
		mgl_axial_gen(gr,v0,z,x,y,gr->GetC(v0),dir,j,wire);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_axial_val(HMGL gr, HCDT v, HCDT a, const char *sch)
{
	if(a->GetNx()<2 || a->GetNy()<2)	{	gr->SetWarn(mglWarnLow,"Axial");	return;	}
	mglData x(a->GetNx(),a->GetNy()), y(a->GetNx(),a->GetNy());
	if(gr->Max.x*gr->Min.x>=0)	x.Fill(gr->Min.x,gr->Max.x,'x');
	else	x.Fill(0,gr->Max.x,'x');
	y.Fill(gr->Min.y,gr->Max.y,'y');
	mgl_axial_xy_val(gr,v,&x,&y,a,sch);
}
//-----------------------------------------------------------------------------
void mgl_axial_xy(HMGL gr, HCDT x, HCDT y, HCDT a, const char *sch, int Num)
{
	if(Num<1)	{	gr->SetWarn(mglWarnCnt,"Axial");	return;	}
	mglData v(Num);
	for(long i=0;i<Num;i++)	v.a[i] = gr->Min.c + (gr->Max.c-gr->Min.c)*float(i+1)/(Num+1);
	mgl_axial_xy_val(gr,&v,x,y,a,sch);
}
//-----------------------------------------------------------------------------
void mgl_axial(HMGL gr, HCDT a, const char *sch, int Num)
{
	if(Num<1)	{	gr->SetWarn(mglWarnCnt,"Axial");	return;	}
	mglData v(Num);
	for(long i=0;i<Num;i++)	v.a[i] = gr->Min.c + (gr->Max.c-gr->Min.c)*float(i+1)/(Num+1);
	mgl_axial_val(gr,&v,a,sch);
}
//-----------------------------------------------------------------------------
void mgl_axial_xy_val_(uintptr_t *gr, uintptr_t *v, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_axial_xy_val(_GR_, _DA_(v), _DA_(x), _DA_(y), _DA_(a), s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_axial_val_(uintptr_t *gr, uintptr_t *v, uintptr_t *a, const char *sch,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_axial_val(_GR_, _DA_(v), _DA_(a), s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_axial_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch, int *Num,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_axial_xy(_GR_, _DA_(x), _DA_(y), _DA_(a), s, *Num);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_axial_(uintptr_t *gr, uintptr_t *a, const char *sch, int *Num,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_axial(_GR_, _DA_(a), s, *Num);	delete []s;	}
//-----------------------------------------------------------------------------
//
//		Torus series
//
//-----------------------------------------------------------------------------
void mgl_torus(HMGL gr, HCDT r, HCDT z, const char *sch)
{
	long i,j,n=r->GetNx();
	if(n*r->GetNy()!=z->GetNx()*z->GetNy())	{	gr->SetWarn(mglWarnDim,"Torus");	return;	}
	if(n<2)		{	gr->SetWarn(mglWarnLow,"Torus");	return;	}
	static int cgid=1;	gr->StartGroup("Torus",cgid++);

	mglPoint *pp = new mglPoint[n];
	long *nn = new long[n];
	gr->SetScheme(sch);
	char dir='y';
	if(sch)
	{
		if(strchr(sch,'x'))	dir = 'x';
		if(strchr(sch,'z'))	dir = 'z';
	}

	mglColor c = gr->GetC(gr->Min.c);
	for(j=0;j<r->GetNy();j++)
	{
		for(i=0;i<n;i++)
		{
			nn[i] = i<n-1 ? i+1 : -1;
			pp[i] = mglPoint(r->v(i,j), z->v(i,j));
		}
		mgl_axial_plot(gr,n,pp,nn,dir,c,sch && strchr(sch,'#'));
	}
	gr->EndGroup();
	delete []nn;	delete []pp;
}
//-----------------------------------------------------------------------------
void mgl_torus_(uintptr_t *gr, uintptr_t *r, uintptr_t *z, const char *pen,int l)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	mgl_torus(_GR_, _DA_(r), _DA_(z), s);	delete []s;	}
//-----------------------------------------------------------------------------
