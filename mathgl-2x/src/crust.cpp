/***************************************************************************
 * crust.cpp is part of Math Graphic Library
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
#include <float.h>
#include "mgl/other.h"
#include "mgl/data.h"
//-----------------------------------------------------------------------------
//
//	TriPlot series
//
//-----------------------------------------------------------------------------
void mgl_triplot_xyzc(HMGL gr, HCDT nums, HCDT x, HCDT y, HCDT z, HCDT a, const char *sch, const char *opt)
{
	long n = x->GetNx(), m = nums->GetNy();
	if(y->GetNx()!=n || z->GetNx()!=n || nums->GetNx()<3)	{	gr->SetWarn(mglWarnLow,"TriPlot");	return;	}
	if(a->GetNx()!=m && a->GetNx()!=n)	{	gr->SetWarn(mglWarnLow,"TriPlot");	return;	}
	long ss=gr->AddTexture(sch);
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("TriPlot",cgid++);
	mglPoint p,q=mglPoint(NAN);

	register long i,k,k1,k2,k3;
	long nc = a->GetNx();
	if(nc!=n && nc>=m)	// colors per triangle
	{
		gr->Reserve(m*3);
		for(i=0;i<m;i++)
		{
			if(gr->Stop)	return;
			k = long(nums->v(0,i)+0.5);
			p = mglPoint(x->v(k), y->v(k), z->v(k));
			k1 = gr->AddPnt(p,gr->GetC(ss,a->v(k)),q);
			k = long(nums->v(1,i)+0.5);
			p = mglPoint(x->v(k), y->v(k), z->v(k));
			k2 = gr->AddPnt(p,gr->GetC(ss,a->v(k)),q);
			k = long(nums->v(2,i)+0.5);
			p = mglPoint(x->v(k), y->v(k), z->v(k));
			k3 = gr->AddPnt(p,gr->GetC(ss,a->v(k)),q);
			gr->trig_plot(k1,k2,k3);
		}
	}
	else if(nc>=n)		// colors per point
	{
		gr->Reserve(n);
		long *kk = new long[n];
		for(k=0;k<n;k++)	// TODO: add averaged normales here
		{
			if(gr->Stop)	{	delete []kk;	return;	}
			p = mglPoint(x->v(k), y->v(k), z->v(k));
			kk[k] = gr->AddPnt(p,gr->GetC(ss,a->v(k)),q);
		}
		for(i=0;i<m;i++)
		{
			if(gr->Stop)	{	delete []kk;	return;	}
			k1 = long(nums->v(0,i)+0.5);
			k2 = long(nums->v(1,i)+0.5);
			k3 = long(nums->v(2,i)+0.5);
			gr->trig_plot(kk[k1],kk[k2],kk[k3]);
		}
		delete []kk;
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_triplot_xyz(HMGL gr, HCDT nums, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt)
{	mgl_triplot_xyzc(gr,nums,x,y,z,z,sch,opt);	}
//-----------------------------------------------------------------------------
void mgl_triplot_xy(HMGL gr, HCDT nums, HCDT x, HCDT y, const char *sch, const char *opt)
{
	gr->SaveState(opt);
	mglData z(x->GetNx());	z.Fill(gr->Min.z,gr->Min.z);
	mgl_triplot_xyzc(gr,nums,x,y,&z,&z,sch,0);
}
//-----------------------------------------------------------------------------
void mgl_triplot_xyzc_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *c, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_triplot_xyzc(_GR_, _DA_(nums), _DA_(x), _DA_(y), _DA_(z), _DA_(c), s, o);
	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_triplot_xyz_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_triplot_xyz(_GR_, _DA_(nums), _DA_(x), _DA_(y), _DA_(z), s, o);
	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_triplot_xy_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_triplot_xy(_GR_, _DA_(nums), _DA_(x), _DA_(y), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	QuadPlot series
//
//-----------------------------------------------------------------------------
void mgl_quadplot_xyzc(HMGL gr, HCDT nums, HCDT x, HCDT y, HCDT z, HCDT a, const char *sch, const char *opt)
{
	long n = x->GetNx(), m = nums->GetNy();
	if(y->GetNx()!=n || z->GetNx()!=n || nums->GetNx()<4)	{	gr->SetWarn(mglWarnLow,"QuadPlot");	return;	}
	if(a->GetNx()!=m && a->GetNx()!=n)	{	gr->SetWarn(mglWarnLow,"QuadPlot");	return;	}
	long ss=gr->AddTexture(sch);
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("QuadPlot",cgid++);
	mglPoint p,q=mglPoint(NAN);

	register long i,k,k1,k2,k3,k4;
	long nc = a->GetNx();
	if(nc!=n && nc>=m)	// colors per triangle
	{
		gr->Reserve(m*4);
		for(i=0;i<m;i++)
		{
			if(gr->Stop)	return;
			k = floor(nums->v(0,i)+0.5);
			p = mglPoint(x->v(k), y->v(k), z->v(k));
			k1 = gr->AddPnt(p,gr->GetC(ss,a->v(k)),q);
			k = floor(nums->v(1,i)+0.5);
			p = mglPoint(x->v(k), y->v(k), z->v(k));
			k2 = gr->AddPnt(p,gr->GetC(ss,a->v(k)),q);
			k = floor(nums->v(2,i)+0.5);
			p = mglPoint(x->v(k), y->v(k), z->v(k));
			k3 = gr->AddPnt(p,gr->GetC(ss,a->v(k)),q);
			k = floor(nums->v(3,i)+0.5);
			p = mglPoint(x->v(k), y->v(k), z->v(k));
			k4 = gr->AddPnt(p,gr->GetC(ss,a->v(k)),q);
			gr->quad_plot(k1,k2,k3,k4);
		}
	}
	else if(nc>=n)		// colors per point
	{
		gr->Reserve(n);
		long *kk = new long[n];
		for(k=0;k<n;k++)	// TODO: add averaged normales here
		{
			if(gr->Stop)	{	delete []kk;	return;	}
			p = mglPoint(x->v(k), y->v(k), z->v(k));
			kk[k] = gr->AddPnt(p,gr->GetC(ss,a->v(k)),q);
		}
		for(i=0;i<m;i++)
		{
			if(gr->Stop)	{	delete []kk;	return;	}
			k1 = floor(nums->v(0,i)+0.5);
			k2 = floor(nums->v(1,i)+0.5);
			k3 = floor(nums->v(2,i)+0.5);
			k4 = floor(nums->v(3,i)+0.5);
			gr->quad_plot(kk[k1],kk[k2],kk[k3],kk[k4]);
		}
		delete []kk;
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_quadplot_xyz(HMGL gr, HCDT nums, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt)
{	mgl_quadplot_xyzc(gr,nums,x,y,z,z,sch,opt);	}
//-----------------------------------------------------------------------------
void mgl_quadplot_xy(HMGL gr, HCDT nums, HCDT x, HCDT y, const char *sch, const char *opt)
{
	gr->SaveState(opt);
	mglData z(x->GetNx());	z.Fill(gr->Min.z,gr->Min.z);
	mgl_quadplot_xyzc(gr,nums,x,y,&z,&z,sch,0);
}
//-----------------------------------------------------------------------------
void mgl_quadplot_xyzc_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *c, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_quadplot_xyzc(_GR_, _DA_(nums), _DA_(x), _DA_(y), _DA_(z), _DA_(c), s, o);
	delete []o;	delete []s;}
//-----------------------------------------------------------------------------
void mgl_quadplot_xyz_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_quadplot_xyzc(_GR_, _DA_(nums), _DA_(x), _DA_(y), _DA_(z), _DA_(z), s, o);
	delete []o;	delete []s;}
//-----------------------------------------------------------------------------
void mgl_quadplot_xy_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_quadplot_xy(_GR_, _DA_(nums), _DA_(x), _DA_(y), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	TriCont series
//
//-----------------------------------------------------------------------------
void mgl_tricont_line(HMGL gr, float val, long k1, long k2, long k3, HCDT x, HCDT y, HCDT z, HCDT a, bool zVal,float c)
{
	float d1,d2;
	mglPoint p1,p2,n;
	d1 = mgl_d(val,a->v(k1),a->v(k2));
	d2 = mgl_d(val,a->v(k1),a->v(k3));
	if(d1<0 || d1>1 || d2<0 || d2>1)	return;
	p1 = mglPoint(x->v(k1)*(1-d1)+x->v(k2)*d1, y->v(k1)*(1-d1)+y->v(k2)*d1,
				zVal?z->v(k1)*(1-d1)+z->v(k2)*d1:gr->Min.z);
	if(!gr->ScalePoint(p1,n))	return;
	p2 = mglPoint(x->v(k1)*(1-d2)+x->v(k3)*d2, y->v(k1)*(1-d2)+y->v(k3)*d2,
				zVal?z->v(k1)*(1-d2)+z->v(k3)*d2:gr->Min.z);
	if(!gr->ScalePoint(p2,n))	return;

	k1 = gr->AddPnt(p1,c);	k2 = gr->AddPnt(p2,c);
	gr->line_plot(k1,k2);
}
//-----------------------------------------------------------------------------
void mgl_tricont_xyzcv(HMGL gr, HCDT v, HCDT nums, HCDT x, HCDT y, HCDT z, HCDT a, const char *sch, const char *opt)
{
	long n = x->GetNx(), m = nums->GetNy();
	if(y->GetNx()!=n || z->GetNx()!=n || nums->GetNx()<3)	{	gr->SetWarn(mglWarnLow,"TriCont");	return;	}
	if(a->GetNx()!=m && a->GetNx()!=n)	{	gr->SetWarn(mglWarnLow,"TriCont");	return;	}
	long ss=gr->AddTexture(sch);
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("TriCont",cgid++);
	float val;
	register long i,k;
	long k1,k2,k3;
	bool zVal = (sch && strchr(sch,'_'));
	for(k=0;k<v->GetNx();k++)	for(i=0;i<m;i++)
	{
		if(gr->Stop)	return;
		k1 = long(nums->v(0,i)+0.1);	if(k1<0 || k1>=n)	continue;
		k2 = long(nums->v(1,i)+0.1);	if(k2<0 || k2>=n)	continue;
		k3 = long(nums->v(2,i)+0.1);	if(k3<0 || k3>=n)	continue;
		val = v->v(k);
		float c = gr->GetC(ss,val);
		mgl_tricont_line(gr,val, k1,k2,k3,x,y,z,a,zVal,c);
		mgl_tricont_line(gr,val, k2,k1,k3,x,y,z,a,zVal,c);
		mgl_tricont_line(gr,val, k3,k2,k1,x,y,z,a,zVal,c);
	}
}
//-----------------------------------------------------------------------------
void mgl_tricont_xyzc(HMGL gr, HCDT nums, HCDT x, HCDT y, HCDT z, HCDT a, const char *sch, const char *opt)
{
	float r = gr->SaveState(opt);
	long n = (isnan(r) || r<=0) ? 7:long(r+0.5);
	mglData v(n);
	for(long i=0;i<n;i++)	v.a[i] = gr->Min.c + (gr->Max.c-gr->Min.c)*float(i+1)/(n+1);
	mgl_tricont_xyzcv(gr,&v,nums,x,y,z,a,sch,0);
}
//-----------------------------------------------------------------------------
void mgl_tricont_xyc(HMGL gr, HCDT nums, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt)
{	mgl_tricont_xyzc(gr,nums,x,y,z,z,sch,opt);	}
//-----------------------------------------------------------------------------
void mgl_tricont_xycv(HMGL gr, HCDT v, HCDT nums, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt)
{	mgl_tricont_xyzcv(gr,v,nums,x,y,z,z,sch,opt);	}
//-----------------------------------------------------------------------------
void mgl_tricont_xyzcv_(uintptr_t *gr, uintptr_t *v, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *c, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_tricont_xyzcv(_GR_, _DA_(v), _DA_(nums), _DA_(x), _DA_(y), _DA_(z), _DA_(c), s, o);
	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_tricont_xycv_(uintptr_t *gr, uintptr_t *v, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_tricont_xycv(_GR_, _DA_(v), _DA_(nums), _DA_(x), _DA_(y), _DA_(z), s, o);
	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_tricont_xyzc_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *c, const char *sch, const char *opt, int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_tricont_xyzc(_GR_, _DA_(nums), _DA_(x), _DA_(y), _DA_(z), _DA_(c), s, o);
	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_tricont_xyc_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt, int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_tricont_xyc(_GR_, _DA_(nums), _DA_(x), _DA_(y), _DA_(z), s, o);
	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Dots series
//
//-----------------------------------------------------------------------------
void mgl_dots_a(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT a, const char *sch, const char *opt)
{
	long n = x->GetNx();
	if(y->GetNx()!=n || z->GetNx()!=n || a->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"Dots");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Dots",cgid++);
	char mk=gr->SetPenPal(sch);
	long ss=gr->AddTexture(sch), pp;
	if(mk==0)	mk='.';
	gr->Reserve(n);
	mglPoint p;

	for(long i=0;i<n;i++)
	{
		if(gr->Stop)	return;
		p = mglPoint(x->v(i),y->v(i),z->v(i));
		pp = gr->AddPnt(p,gr->GetC(ss,a->v(i)));
		gr->mark_plot(pp, mk);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_dots(HMGL gr, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt)
{	mgl_dots_a(gr, x, y, z, z, sch, opt);	}
//-----------------------------------------------------------------------------
void mgl_dots_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_dots(_GR_, _DA_(x),_DA_(y),_DA_(z),s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_dots_a_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_dots_a(_GR_, _DA_(x),_DA_(y),_DA_(z),_DA_(a),s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	mglTriangulation
//
//-----------------------------------------------------------------------------
long mgl_crust(long n,mglPoint *pp,long **nn,float ff);
HMDT mgl_triangulation_3d(HCDT x, HCDT y, HCDT z, float er)
{
	mglData *nums=new mglData;
	long n = x->GetNx(), m;
	if(y->GetNx()!=n || z->GetNx()!=n)	return nums;
	register long i;
	mglPoint *pp = new mglPoint[n];
	long *nn=0;
	for(i=0;i<n;i++)	pp[i] = mglPoint(x->v(i), y->v(i), z->v(i));
	m = mgl_crust(n,pp,&nn,er);

	if(m>0)
	{
		nums->Create(3,m);
		for(i=0;i<3*m;i++)	nums->a[i]=nn[i];
	}
	delete []pp;	free(nn);	return nums;
}
//-----------------------------------------------------------------------------
HMDT mgl_triangulation_2d(HCDT x, HCDT y, float er)
{
	mglData z(x->GetNx());
	return mgl_triangulation_3d(x,y,&z,er);
}
//-----------------------------------------------------------------------------
uintptr_t mgl_triangulation_3d_(uintptr_t *x, uintptr_t *y, uintptr_t *z, float *er)
{	return uintptr_t(mgl_triangulation_3d(_DA_(x),_DA_(y),_DA_(z),*er));	}
uintptr_t mgl_triangulation_2d_(uintptr_t *x, uintptr_t *y, float *er)
{	return uintptr_t(mgl_triangulation_2d(_DA_(x),_DA_(y),*er));	}
//-----------------------------------------------------------------------------
//
//	Crust series
//
//-----------------------------------------------------------------------------
void mgl_crust(HMGL gr, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt)
{
	if(y->GetNx()!=x->GetNx() || z->GetNx()!=x->GetNx())
	{	gr->SetWarn(mglWarnDim,"Crust");	return;	}
	HMDT nums = mgl_triangulation_3d(x, y, z, 0);
	mgl_triplot_xyzc(gr,nums,x,y,z,z,sch,opt);
	mgl_delete_data(nums);
}
//-----------------------------------------------------------------------------
void mgl_crust_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_crust(_GR_, _DA_(x),_DA_(y),_DA_(z),s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
long mgl_insert_trig(long i1,long i2,long i3,long **n)
{
	static long Cur=0,Max=0;
	if(i1<0 || i2<0 || i3<0)	return Cur;
	if(*n==0)
	{
		Max = 1024;		Cur = 0;
		*n = (long *)malloc(Max*3*sizeof(long));
		memset(*n,0,Max*3*sizeof(long));
	}
	if(Cur>=Max)
	{
		Max += 1024;
		*n = (long *)realloc(*n,Max*3*sizeof(long));
		memset(*n+3*(Max-1024),0,3*1024*sizeof(long));
	}
	long *nn = *n;
	register long i,k1;
	if(i1>i3)	{	k1=i1;	i1=i3;	i3=k1;	}	// simple sorting
	if(i1>i2)	{	k1=i1;	i1=i2;	i2=k1;	}
	if(i2>i3)	{	k1=i2;	i2=i3;	i3=k1;	}
	for(i=0;i<Cur;i++)	// check if it is unique
	{
		nn = *n + 3*i;
		if(nn[0]==i1 && nn[1]==i2 && nn[2]==i3)	return Cur;
	}
	nn = *n + 3*Cur;
	nn[0]=i1;	nn[1]=i2;	nn[2]=i3;
	Cur++;	return Cur;
}
//-----------------------------------------------------------------------------
long mgl_get_next(long k1,long n,long *,long *set,mglPoint *qq)
{
	long i,j=-1;
	float r,rm=FLT_MAX;
	for(i=0;i<n;i++)
	{
		if(i==k1 || set[i]>0)	continue;
		r = mgl_norm(qq[i]-qq[k1]);
		if(r<rm)	{	rm=r;	j=i;	}
	}
	return j;
}
//-----------------------------------------------------------------------------
long mgl_crust(long n,mglPoint *pp,long **nn,float ff)
{	// TODO: update to normal algorithm
	register long i,j;
	register float r,rm,rs;
	if(ff<=0)	ff=2;
	for(rs=0,i=0;i<n;i++)
	{
		for(rm = FLT_MAX,j=0;j<n;j++)
		{
			if(i==j)	continue;
			r = mgl_norm(pp[i]-pp[j]);
			if(rm>r)	rm = r;
		}
		rs += sqrt(rm);
	}
	rs *= ff/n;	rs = rs*rs;		// "average" distance
	long ind[100], set[100], ii;	// indexes of "close" points, flag that it was added and its number
	mglPoint qq[100];	// normalized point coordinates
	long k1,k2,k3,m=0;
	for(i=0;i<n;i++)	// now the triangles will be found
	{
		memset(set,0,100*sizeof(long));
		for(ii=0,j=0;j<n;j++)	// find close vertexes
		{
			r = mgl_norm(pp[i]-pp[j]);
			if(r<=rs && j!=i)	{	ind[ii] = j;	ii++;	if(ii==99)	break;}
		}
		if(ii<3)	continue;	// nothing to do
		for(j=0;j<ii;j++)
		{
			k1 = j;	k2 = ind[j];	k3 = i;
			qq[k1] = pp[k2] - pp[k3];	r = qq[k1].norm();
			qq[k1] /= r;
		}
		k1 = 0;
		while((k2=mgl_get_next(k1,ii,ind,set,qq))>0)
		{
			set[k1]=1;
			m = mgl_insert_trig(i,ind[k1],ind[k2],nn);
			k1 = k2;
		}
		m = mgl_insert_trig(i,ind[k1],ind[0],nn);
	}
	return m;
}
//-----------------------------------------------------------------------------
