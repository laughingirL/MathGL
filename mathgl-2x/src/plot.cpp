/***************************************************************************
 * plot.cpp is part of Math Graphic Library
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
#include "mgl/plot.h"
#include "mgl/eval.h"
#include "mgl/data.h"
#include <stdlib.h>
//-----------------------------------------------------------------------------
//
//	Plot by formulas series
//
//-----------------------------------------------------------------------------
void mgl_fplot(HMGL gr, const char *eqY, const char *pen, const char *opt)
{
	if(eqY==0 || eqY[0]==0)	return;		// nothing to plot
	float r = gr->SaveState(opt);
	long n = (isnan(r) || r<=0) ? 100:long(r+0.5);

	float *x = (float *)malloc(n*sizeof(float));
	float *y = (float *)malloc(n*sizeof(float));
	mglFormula *eq = new mglFormula(eqY);
	register int i;
	float d = (gr->Max.x - gr->Min.x)/(n-1.), xs, ys, yr, ym=fabs(gr->Max.y - gr->Min.y)/1000;
	for(i=0;i<n;i++)	// initial data filling
	{	x[i]=gr->Min.x + i*d;	y[i]=eq->Calc(x[i]);	}

	for(i=0;i<n-1 && n<10000;)
	{
		xs=(x[i]+x[i+1])/2;
		ys=(y[i]+y[i+1])/2;	yr=eq->Calc(xs);
		if(fabs(yr-ys)>ym)	// bad approximation here
		{
			x = (float *)realloc(x,(n+1)*sizeof(float));
			y = (float *)realloc(y,(n+1)*sizeof(float));
			memmove(x+i+2,x+i+1,(n-i-1)*sizeof(float));
			memmove(y+i+2,y+i+1,(n-i-1)*sizeof(float));
			x[i+1] = xs;	y[i+1] = yr;	n++;
		}
		else i++;
	}

	delete eq;
	mglData yy,xx;
	xx.Set(x,n);	free(x);
	yy.Set(y,n);	free(y);
	mgl_plot_xy(gr,&xx,&yy,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_fplot_xyz(HMGL gr, const char *eqX, const char *eqY, const char *eqZ, const char *pen, const char *opt)
{
	float r = gr->SaveState(opt);
	long n = (isnan(r) || r<=0) ? 100:long(r+0.5);

	float *x = (float *)malloc(n*sizeof(float));
	float *y = (float *)malloc(n*sizeof(float));
	float *z = (float *)malloc(n*sizeof(float));
	float *t = (float *)malloc(n*sizeof(float));
	mglFormula *ex, *ey, *ez;
	ex = new mglFormula(eqX ? eqX : "0");
	ey = new mglFormula(eqY ? eqY : "0");
	ez = new mglFormula(eqZ ? eqZ : "0");
	register int i;
	float ts, xs, ys, zs, xr, yr, zr, xm=fabs(gr->Max.x - gr->Min.x)/1000, ym=fabs(gr->Max.y - gr->Min.y)/1000, zm=fabs(gr->Max.z - gr->Min.z)/1000;
	for(i=0;i<n;i++)	// initial data filling
	{
		t[i] = i/(n-1.);
		x[i] = ex->Calc(0,0,t[i]);
		y[i] = ey->Calc(0,0,t[i]);
		z[i] = ez->Calc(0,0,t[i]);
	}

	for(i=0;i<n-1 && n<10000;)
	{
		ts=(t[i]+t[i+1])/2;
		xs=(x[i]+x[i+1])/2;	xr=ex->Calc(0,0,ts);
		ys=(y[i]+y[i+1])/2;	yr=ey->Calc(0,0,ts);
		zs=(z[i]+z[i+1])/2;	zr=ez->Calc(0,0,ts);
		if(fabs(xr-xs)>xm || fabs(yr-ys)>ym || fabs(zr-zs)>zm)	// bad approximation here
		{
			z = (float *)realloc(z,(n+1)*sizeof(float));
			t = (float *)realloc(t,(n+1)*sizeof(float));
			x = (float *)realloc(x,(n+1)*sizeof(float));
			y = (float *)realloc(y,(n+1)*sizeof(float));
			memmove(x+i+2,x+i+1,(n-i-1)*sizeof(float));
			memmove(y+i+2,y+i+1,(n-i-1)*sizeof(float));
			memmove(z+i+2,z+i+1,(n-i-1)*sizeof(float));
			memmove(t+i+2,t+i+1,(n-i-1)*sizeof(float));
			t[i+1]=ts;	x[i+1]=xr;	y[i+1]=yr;	z[i+1]=zr;	n++;
		}
		else i++;
	}
	delete ex;	delete ey;	delete ez;

	mglData xx,yy,zz;
	xx.Set(x,n);	yy.Set(y,n);	zz.Set(z,n);
	free(x);	free(y);	free(z);	free(t);
	mgl_plot_xyz(gr,&xx,&yy,&zz,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_fplot_(uintptr_t *gr, const char *fy, const char *stl, const char *opt, int ly, int ls, int lo)
{	char *s=new char[ly+1];	memcpy(s,fy,ly);	s[ly]=0;
	char *p=new char[ls+1];	memcpy(p,stl,ls);	p[ls]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_fplot(_GR_, s, p, o);
	delete []s;		delete []p;	delete []o;	}
//-----------------------------------------------------------------------------
void mgl_fplot_xyz_(uintptr_t *gr, const char *fx, const char *fy, const char *fz, const char *stl, const char *opt, int lx, int ly, int lz, int ls, int lo)
{	char *sx=new char[lx+1];	memcpy(sx,fx,lx);	sx[lx]=0;
	char *sy=new char[ly+1];	memcpy(sy,fy,ly);	sy[ly]=0;
	char *sz=new char[lz+1];	memcpy(sz,fz,lz);	sz[lz]=0;
	char *o=new char[lo+1];		memcpy(o,opt,lo);	o[lo]=0;
	char *p=new char[ls+1];		memcpy(p,stl,ls);	p[ls]=0;
	mgl_fplot_xyz(_GR_, sx, sy, sz, p, o);
	delete []sx;	delete []sy;	delete []sz;	delete []p;	delete []o;	}
//-----------------------------------------------------------------------------
//
//	Radar series
//
//-----------------------------------------------------------------------------
void mgl_radar(HMGL gr, HCDT a, const char *pen, const char *opt)
{
	long n = a->GetNx(), ny=a->GetNy();
	if(n<2)	{	gr->SetWarn(mglWarnLow,"Radar");	return;	}
	mglData x(n+1,ny), y(n+1,ny);
	float m=a->Minimal(), r=gr->SaveState(opt);
	if(isnan(r) || r<0)	r = m<0 ? -m:0;
	register long i,j;
	for(j=0;j<ny;j++)
	{
		for(i=0;i<n;i++)
		{
			x.a[i+(n+1)*j] = (r+a->v(i,j))*cos(2*i*M_PI/n);
			y.a[i+(n+1)*j] = (r+a->v(i,j))*sin(2*i*M_PI/n);
		}
		x.a[n+(n+1)*j] = r+a->v(0,j);	y.a[n+(n+1)*j] = 0;
	}
	mgl_plot_xy(gr,&x,&y,pen,0);
	if(pen && strchr(pen,'#'))	// draw "grid"
	{
		m = 1.1*(a->Maximal()+r);
		x.Create(2);	y.Create(2);
		for(i=0;i<n;i++)
		{
			x.a[1]=m*cos(2*i*M_PI/n);
			y.a[1]=m*sin(2*i*M_PI/n);
			mgl_plot_xy(gr,&x,&y,"k",0);
		}
		if(r>0)
		{
			x.Create(101);	y.Create(101);
			for(i=0;i<101;i++)
			{	x.a[i]=r*cos(2*i*M_PI/100);	y.a[i]=r*sin(2*i*M_PI/100);	}
			mgl_plot_xy(gr,&x,&y,"k",0);
		}
	}
}
//-----------------------------------------------------------------------------
void mgl_radar_(uintptr_t *gr, uintptr_t *a, const char *pen, const char *opt, int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];		memcpy(o,opt,lo);	o[lo]=0;
	mgl_radar(_GR_, _DA_(a),s,o);	delete []s;	delete []o;	}
//-----------------------------------------------------------------------------
//
//	Plot series
//
//-----------------------------------------------------------------------------
void mgl_plot_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, const char *pen, const char *opt)
{
	long j,m,mx,my,mz,n=y->GetNx(),pal;
	if(x->GetNx()!=n || z->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"Plot");	return;	}
	if(n<2)	{	gr->SetWarn(mglWarnLow,"Plot");	return;	}
	static int cgid=1;	gr->StartGroup("Plot",cgid++);
	gr->SaveState(opt);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();	m = z->GetNy() > m ? z->GetNy() : m;
	char mk=gr->SetPenPal(pen,&pal);	gr->Reserve(2*n*m);
	bool t1,t2,t3;
	mglPoint p1,p2,p3,nn;
	long n1=-1,n2=-1,n3=-1;

	for(j=0;j<m;j++)
	{
		mx = j<x->GetNy() ? j:0;		my = j<y->GetNy() ? j:0;
		mz = j<z->GetNy() ? j:0;		gr->NextColor(pal);
		t1 = t2 = false;
		register long i;
		for(i=0;i<n;i++)
		{
			if(i>0)	{	n2=n1;	p2=p1;	t2=t1;	}
			p1 = mglPoint(x->v(i,mx), y->v(i,my), z->v(i,mz));
			n1 = gr->AddPnt(p1);	t1 = n1>=0;	// NOT thread-safe!!!
			if(mk && t1)	gr->mark_plot(n1,mk);
			if(t1 && t2)
			{
				gr->line_plot(n1,n2);
				if(i==0)	gr->arrow_plot(n1,n2,gr->Arrow1);
				if(i==n-1)	gr->arrow_plot(n1,n2,gr->Arrow2);
			}

			if(i>0 && ((t1 && !t2) || (t2 && !t1)))	// do smoothing
			{
				float i1=0, i2=1, ii;
				p3=p1;	t3=t1;	n3=n1;	// copy current
				do {
					ii = (i1+i2)/2;
					p1.x = x->v(i,mx)*ii+x->v(i-1,mx)*(1-ii);
					p1.y = y->v(i,my)*ii+y->v(i-1,my)*(1-ii);
					p1.z = z->v(i,mz)*ii+z->v(i-1,mz)*(1-ii);
					n1 = gr->AddPnt(p1);	t1 = n1>=0;	// NOT thread-safe!!!
					if((t1 && t3) || (t2 && !t1))	i2 = ii;
					else	i1 = ii;
				} while(fabs(i2-i1)>1e-3);
				if(t2)	gr->line_plot(n1,n2);
				else	gr->line_plot(n3,n1);
			}
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_plot_xy(HMGL gr, HCDT x, HCDT y, const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData z(y->GetNx());
	z.Fill(gr->Min.z,gr->Min.z);
	mgl_plot_xyz(gr,x,y,&z,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_plot(HMGL gr, HCDT y, const char *pen, const char *opt)
{
	if(y->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"Plot");	return;	}
	gr->SaveState(opt);
	mglData x(y->GetNx()), z(y->GetNx());
	x.Fill(gr->Min.x,gr->Max.x);
	z.Fill(gr->Min.z,gr->Min.z);
	mgl_plot_xyz(gr,&x,y,&z,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_plot_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];		memcpy(o,opt,lo);	o[lo]=0;
	mgl_plot_xyz(_GR_, _DA_(x),_DA_(y),_DA_(z),s,o);	delete []s;	delete []o;	}
//-----------------------------------------------------------------------------
void mgl_plot_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];		memcpy(o,opt,lo);	o[lo]=0;
	mgl_plot_xy(_GR_, _DA_(x),_DA_(y),s,o);	delete []s;	delete []o;	}
//-----------------------------------------------------------------------------
void mgl_plot_(uintptr_t *gr, uintptr_t *y,	const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];		memcpy(o,opt,lo);	o[lo]=0;
	mgl_plot(_GR_, _DA_(y),s,o);	delete []s;	delete []o;	}
//-----------------------------------------------------------------------------
//
//	Tens series
//
//-----------------------------------------------------------------------------
void mgl_tens_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT c, const char *pen, const char *opt)
{
	long j,m,mx,my,mz,mc,n=y->GetNx(), pal;
	if(x->GetNx()!=n || z->GetNx()!=n || c->GetNx()!=n)
	{	gr->SetWarn(mglWarnDim,"Tens");	return;	}
	if(n<2)					{	gr->SetWarn(mglWarnLow,"Tens");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Tens",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();	m = z->GetNy() > m ? z->GetNy() : m;
	char mk=gr->SetPenPal(pen, &pal);	gr->Reserve(2*n*m);
	long ss=gr->AddTexture(pen);
	bool t1,t2,t3;
	mglPoint p1,p2,p3,nn;
	long n1=-1,n2=-1,n3=-1;

	for(j=0;j<m;j++)
	{
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		mz = j<z->GetNy() ? j:0;	mc = j<c->GetNy() ? j:0;
		register long i;
		t1 = t2 = false;
		for(i=0;i<n;i++)
		{
			if(i>0)	{	n2=n1;	p2=p1;	t2=t1;	}
			p1 = mglPoint(x->v(i,mx), y->v(i,my), z->v(i,mz), c->v(i,mc));
			n1 = gr->AddPnt(p1,gr->GetC(ss,p1.c));	t1 = n1>=0;	// NOT thread-safe!!!
			if(mk && t1)	gr->mark_plot(n1,mk);
			if(t1 && t2)
			{
				gr->line_plot(n1,n2);
				if(i==0)	gr->arrow_plot(n1,n2,gr->Arrow1);
				if(i==n-1)	gr->arrow_plot(n1,n2,gr->Arrow2);
			}

			if(i>0 && ((t1 && !t2) || (t2 && !t1)))	// do smoothing
			{
				float i1=0, i2=1, ii;
				p3=p1;	t3=t1;	n3=n1;	// copy current
				do {
					ii = (i1+i2)/2;
					p1.x = x->v(i,mx)*ii+x->v(i-1,mx)*(1-ii);
					p1.y = y->v(i,my)*ii+y->v(i-1,my)*(1-ii);
					p1.z = z->v(i,mz)*ii+z->v(i-1,mz)*(1-ii);
					p1.c = c->v(i,mc)*ii+c->v(i-1,mc)*(1-ii);
					n1 = gr->AddPnt(p1,gr->GetC(ss,p1.c));	t1 = n1>=0;	// NOT thread-safe!!!
					if((t1 && t3) || (t2 && !t1))	i2 = ii;
					else	i1 = ii;
				} while(fabs(i2-i1)>1e-3);
				if(t2)	gr->line_plot(n1,n2);
				else	gr->line_plot(n3,n1);
			}
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_tens_xy(HMGL gr, HCDT x, HCDT y, HCDT c, const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData z(y->GetNx());
	z.Fill(gr->Min.z,gr->Min.z);
	mgl_tens_xyz(gr,x,y,&z,c,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_tens(HMGL gr, HCDT y, HCDT c, const char *pen, const char *opt)
{
	if(y->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"Tens");	return;	}
	gr->SaveState(opt);
	mglData x(y->GetNx()), z(y->GetNx());
	x.Fill(gr->Min.x,gr->Max.x);
	z.Fill(gr->Min.z,gr->Min.z);
	mgl_tens_xyz(gr,&x,y,&z,c,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_tens_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *c, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_tens_xyz(_GR_, _DA_(x),_DA_(y),_DA_(z),_DA_(c),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_tens_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *c, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_tens_xy(_GR_, _DA_(x),_DA_(y),_DA_(c),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_tens_(uintptr_t *gr, uintptr_t *y, uintptr_t *c, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_tens(_GR_, _DA_(y),_DA_(c),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Area series
//
//-----------------------------------------------------------------------------
void mgl_area_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, const char *pen, const char *opt)
{
	long i,j,n=y->GetNx(),m,mx,my,mz,pal;
	if(n<2)		{	gr->SetWarn(mglWarnLow,"Area");	return;	}
	if(x->GetNx()!=n || z->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"Area");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Area3",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();	m = z->GetNy() > m ? z->GetNy() : m;

	float z0=gr->GetOrgZ('x');
	float c1,c2;
	mglPoint p1,p2,p3,p4,nn;
	long n1,n2,n3,n4;
	gr->SetPenPal(pen,&pal);	gr->Reserve(2*n*m);
	long s=gr->AddTexture(pen,1);
	for(j=0;j<m;j++)
	{
		gr->NextColor(pal);	c2=c1=gr->CDef;
		if(gr->GetNumPal(pal)==2*m)
		{	c1 = s+2*j/(2*m-1.);	c2 = s+(2*j+0.999)/(2*m-1);	}
//		if(gr->GetNumPal(pal)==2*m)	c2 = gr->NextColor(pal);
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;	mz = j<z->GetNy() ? j:0;

		nn = mglPoint(-y->dvx(0,my),x->dvx(0,mx));
		n1 = gr->AddPnt(mglPoint(x->v(0,mx),y->v(0,my),z->v(0,mz)),c1,nn);
		n2 = gr->AddPnt(mglPoint(x->v(0,mx),y->v(0,my),z0),c2,nn);
		for(i=1;i<n;i++)
		{
			n3=n1;	n4=n2;
			nn = mglPoint(-y->dvx(i,my),x->dvx(i,mx));
			n1 = gr->AddPnt(mglPoint(x->v(i,mx),y->v(i,my),z->v(i,mz)),c1,nn);
			n2 = gr->AddPnt(mglPoint(x->v(i,mx),y->v(i,my),z0),c2,nn);
			gr->quad_plot(n1,n2,n3,n4);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_area_xy(HMGL gr, HCDT x, HCDT y, const char *pen, const char *opt)
{
	long i,j,n=y->GetNx(),m=y->GetNy(),mx,my,pal;
	if(x->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"Area");	return;	}
	if(n<2)		{	gr->SetWarn(mglWarnLow,"Area");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Curve",cgid++);
	float y0=gr->GetOrgY('x'), z0;
	float c1,c2;
	mglPoint nn=mglPoint(0,0,1);
	long n1,n2,n3,n4;

	float *f=new float[n];	memset(f,0,n*sizeof(float));
	bool sum = (pen && strchr(pen,'a'));

	gr->SetPenPal(pen,&pal);	gr->Reserve(2*n*m);
	long s=gr->AddTexture(pen,1);
	for(j=0;j<m;j++)
	{
		gr->NextColor(pal);	c2=c1=gr->CDef;
		if(gr->GetNumPal(pal)==2*m)
		{	c1 = s+2*j/(2*m-1.);	c2 = s+(2*j+0.999)/(2*m-1);	}
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		z0 = gr->Min.z + (m-1-j)*(gr->Max.z-gr->Min.z)/m;

		n1 = gr->AddPnt(mglPoint(x->v(0,mx),y->v(0,my),z0),c1,nn);
		n2 = gr->AddPnt(mglPoint(x->v(0,mx),sum?0:y0,z0),c2,nn);
		for(i=1;i<n;i++)
		{
			n3=n1;	n4=n2;
			n1 = gr->AddPnt(mglPoint(x->v(i,mx),sum?f[i]:y->v(i,my),z0),c1,nn);
			n2 = gr->AddPnt(mglPoint(x->v(i,mx),sum?f[i]:y0,z0),c2,nn);
			gr->quad_plot(n1,n2,n3,n4);	f[i]+=y->v(i,my);
		}
	}
	gr->EndGroup();	delete []f;
}
//-----------------------------------------------------------------------------
void mgl_area(HMGL gr, HCDT y, const char *pen, const char *opt)
{
	if(y->GetNx()<2)		{	gr->SetWarn(mglWarnLow,"Area");	return;	}
	gr->SaveState(opt);
	mglData x(y->GetNx());
	x.Fill(gr->Min.x,gr->Max.x);
	mgl_area_xy(gr,&x,y,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_area_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_area_xyz(_GR_, _DA_(x),_DA_(y),_DA_(z),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_area_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_area_xy(_GR_, _DA_(x),_DA_(y),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_area_(uintptr_t *gr, uintptr_t *y, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_area(_GR_, _DA_(y),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Region series
//
//-----------------------------------------------------------------------------
void mgl_region_xy(HMGL gr, HCDT x, HCDT y1, HCDT y2, const char *pen, const char *opt)
{
	long i,j, n=y1->GetNx(), m=y1->GetNy(), mx, pal;
	if(x->GetNx()!=n || y2->GetNx()!=n || y2->GetNy()!=m)
	{	gr->SetWarn(mglWarnDim,"Region");	return;	}
	if(n<2)	{	gr->SetWarn(mglWarnLow,"Region");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Region",cgid++);
	float c1,c2;
	mglPoint nn=mglPoint(0,0,1);
	long n1,n2,n3,n4;
	float xx,f1,f2,f3,f4;
	bool inside = (pen && strchr(pen,'i'));	// NOTE: check if 'i' is free (used here for inside flag)

	gr->SetPenPal(pen,&pal);	gr->Reserve(2*n*m);
	long s=gr->AddTexture(pen,1);
	for(j=0;j<m;j++)
	{
		gr->NextColor(pal);	c2=c1=gr->CDef;
		if(gr->GetNumPal(pal)==2*m)
		{	c1 = s+2*j/(2*m-1.);	c2 = s+(2*j+0.999)/(2*m-1);	}
		mx = j<x->GetNy() ? j:0;
		float z0 = gr->Min.z + (m-1-j)*(gr->Max.z-gr->Min.z)/m;

		f1 = y1->v(0,j);	f2 = y2->v(0,j);	xx = x->v(0,mx);
		n1 = gr->AddPnt(mglPoint(xx,f1,z0),c1,nn);
		n2 = gr->AddPnt(mglPoint(xx,f2,z0),c2,nn);
		for(i=1;i<n;i++)
		{
			n3=n1;	n4=n2;	f3=f1;	f4=f2;
			f1 = y1->v(i,j);	f2 = y2->v(i,j);	xx = x->v(i,mx);
			n1 = gr->AddPnt(mglPoint(xx,f1,z0),c1,nn);
			n2 = gr->AddPnt(mglPoint(xx,f2,z0),c2,nn);
			if(!inside || (f2>f1 && f4>f3))
				gr->quad_plot(n1,n2,n3,n4);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_region(HMGL gr, HCDT y1, HCDT y2, const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData x(y1->GetNx());
	x.Fill(gr->Min.z, gr->Max.z);
	mgl_region_xy(gr,&x,y1,y2,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_region_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y1, uintptr_t *y2, const char *pen, const char *opt, int l, int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_region_xy(_GR_, _DA_(x),_DA_(y1),_DA_(y2),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_region_(uintptr_t *gr, uintptr_t *y1, uintptr_t *y2, const char *pen, const char *opt, int l, int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_region(_GR_, _DA_(y1),_DA_(y2),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Step series
//
//-----------------------------------------------------------------------------
void mgl_step_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, const char *pen, const char *opt)
{
	long i,j,m,mx,my,mz,n=y->GetNx(), pal;
	if(x->GetNx()!=n || z->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"Step");	return;	}
	if(n<2)		{	gr->SetWarn(mglWarnLow,"Step");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Step3",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();	m = z->GetNy() > m ? z->GetNy() : m;

	char mk=gr->SetPenPal(pen,&pal);	gr->Reserve(2*n*m);
	long n1,n2;
	for(j=0;j<m;j++)
	{
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		mz = j<z->GetNy() ? j:0;	gr->NextColor(pal);
		n1 = gr->AddPnt(mglPoint(x->v(0,mx), y->v(0,my), z->v(0,mz)));
		if(mk)	gr->mark_plot(n1,mk);
		for(i=1;i<n;i++)
		{
			n2 = n1;	// horizontal
			n1 = gr->AddPnt(mglPoint(x->v(i,mx), y->v(i,my), z->v(i-1,mz)));
			gr->line_plot(n1,n2);
			if(i==1)	gr->arrow_plot(n1,n2,gr->Arrow1);

			n2 = n1;	// vertical
			n1 = gr->AddPnt(mglPoint(x->v(i,mx), y->v(i,my), z->v(i,mz)));
			if(mk)	gr->mark_plot(n1,mk);
			gr->line_plot(n1,n2);
			if(i==n-1)	gr->arrow_plot(n1,n2,gr->Arrow2);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_step_xy(HMGL gr, HCDT x, HCDT y, const char *pen, const char *opt)
{
	long i,j,m,mx,my,n=y->GetNx(), pal;
	if(x->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"Step");	return;	}
	if(n<2)		{	gr->SetWarn(mglWarnLow,"Step");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Step",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();

	float zVal = gr->Min.z;
	char mk=gr->SetPenPal(pen,&pal);	gr->Reserve(2*n*m);
	long n1,n2;
	for(j=0;j<m;j++)
	{
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		gr->NextColor(pal);
		n1 = gr->AddPnt(mglPoint(x->v(0,mx), y->v(0,my), zVal));
		if(mk)	gr->mark_plot(n1,mk);
		for(i=1;i<n;i++)
		{
			n2 = n1;	// horizontal
			n1 = gr->AddPnt(mglPoint(x->v(i,mx), y->v(i-1,my), zVal));
			gr->line_plot(n1,n2);
			if(i==1)	gr->arrow_plot(n1,n2,gr->Arrow1);

			n2 = n1;	// vertical
			n1 = gr->AddPnt(mglPoint(x->v(i,mx), y->v(i,my), zVal));
			if(mk)	gr->mark_plot(n1,mk);
			gr->line_plot(n1,n2);
			if(i==n-1)	gr->arrow_plot(n1,n2,gr->Arrow2);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_step(HMGL gr, HCDT y,	const char *pen, const char *opt)
{
	if(y->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"Step");	return;	}
	gr->SaveState(opt);
	mglData x(y->GetNx());
	x.Fill(gr->Min.x,gr->Max.x);
	mgl_step_xy(gr,&x,y,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_step_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_step_xyz(_GR_, _DA_(x),_DA_(y),_DA_(z),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_step_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_step_xy(_GR_, _DA_(x),_DA_(y),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_step_(uintptr_t *gr, uintptr_t *y,	const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_step(_GR_, _DA_(y),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Stem series
//
//-----------------------------------------------------------------------------
void mgl_stem_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, const char *pen, const char *opt)
{
	long i,j,m,mx,my,mz,n=y->GetNx(), pal;
	if(x->GetNx()!=n || z->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"Stem");	return;	}
	if(n<2)		{	gr->SetWarn(mglWarnLow,"Stem");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Stem3",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();	m = z->GetNy() > m ? z->GetNy() : m;

	float z0=gr->GetOrgZ('x');
	char mk=gr->SetPenPal(pen,&pal);	gr->Reserve(2*n*m);
	long n1,n2;
	for(j=0;j<m;j++)
	{
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		mz = j<z->GetNy() ? j:0;	gr->NextColor(pal);
		for(i=0;i<n;i++)
		{
			n1 = gr->AddPnt(mglPoint(x->v(i,mx), y->v(i,my), z->v(i-1,mz)));
			if(mk)	gr->mark_plot(n1,mk);
			n2 = gr->AddPnt(mglPoint(x->v(i,mx), y->v(i,my), z0));
			gr->line_plot(n1,n2);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_stem_xy(HMGL gr, HCDT x, HCDT y, const char *pen, const char *opt)
{
	long i,j,m,mx,my,n=y->GetNx(), pal;
	if(x->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"Stem");	return;	}
	if(n<2)		{	gr->SetWarn(mglWarnLow,"Stem");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Stem",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();

	float zVal = gr->Min.z;
	float y0=gr->GetOrgY('x');
	char mk=gr->SetPenPal(pen,&pal);	gr->Reserve(2*n*m);
	long n1,n2;
	for(j=0;j<m;j++)
	{
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		gr->NextColor(pal);
		for(i=0;i<n;i++)
		{
			n1 = gr->AddPnt(mglPoint(x->v(i,mx), y->v(i,my), zVal));
			if(mk)	gr->mark_plot(n1,mk);
			n2 = gr->AddPnt(mglPoint(x->v(i,mx), y0, zVal));
			gr->line_plot(n1,n2);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_stem(HMGL gr, HCDT y,	const char *pen, const char *opt)
{
	if(y->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"Stem");	return;	}
	gr->SaveState(opt);
	mglData x(y->GetNx());
	x.Fill(gr->Min.x,gr->Max.x);
	mgl_stem_xy(gr,&x,y,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_stem_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_stem_xyz(_GR_,_DA_(x),_DA_(y),_DA_(z),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_stem_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_stem_xy(_GR_,_DA_(x),_DA_(y),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_stem_(uintptr_t *gr, uintptr_t *y,	const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_stem(_GR_,_DA_(y),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Bars series
//
//-----------------------------------------------------------------------------
void mgl_bars_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, const char *pen, const char *opt)
{
	long i,j,m,mx,my,mz,n=y->GetNx(), pal;
	if(x->GetNx()!=n || z->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"Bars");	return;	}
	if(n<2)		{	gr->SetWarn(mglWarnLow,"Bars");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Bars3",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();	m = z->GetNy() > m ? z->GetNy() : m;

	bool wire = pen && strchr(pen,'#');
	bool above = pen && strchr(pen,'a'), fall = pen && strchr(pen,'f');
	if(above)	fall = false;
	float c1,c2,c;
	mglPoint p1,p2,p3,p4,nn;
	long n1,n2,n3,n4;
	float *dd=new float[n], x1,x2,y1,y2,z0,zz,zp;
	memset(dd,0,n*sizeof(float));

	gr->SetPenPal(pen,&pal);
	if(wire)	gr->Reserve(4*n*m);	else	gr->Reserve(4*n*m);
	for(j=0;j<m;j++)
	{
		gr->NextColor(pal);	c2=c1=gr->CDef;
		if(gr->GetNumPal(pal)==2*m)	c2 = gr->NextColor(pal);
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;	mz = j<z->GetNy() ? j:0;
		zp = z0 = gr->GetOrgZ('x');
		for(i=0;i<n;i++)
		{
			if(i<n-1)
			{
				x2 = x->v(i,mx) + gr->BarWidth*(x->v(i+1,mx)-x->v(i,mx))/2;
				y2 = y->v(i,my) + gr->BarWidth*(y->v(i+1,my)-y->v(i,my))/2;
			}
			else	{	x2 = x->v(i,mx);	y2 = y->v(i,my);	}
			if(i>0)
			{
				x1 = x->v(i,mx) - gr->BarWidth*(x->v(i,mx)-x->v(i-1,mx))/2;
				y1 = y->v(i,my) - gr->BarWidth*(y->v(i,my)-y->v(i-1,my))/2;
			}
			else	{	x1 = x->v(i,mx);	y1 = y->v(i,my);	}
			zz = z->v(i,mz);
			if(!above)
			{
				x2 = (x2-x1)/m;		x1 += j*x2;		x2 += x1;
				y2 = (y2-y1)/m;		y1 += j*y2;		y2 += y1;
			}
			else
			{	z0 = gr->GetOrgZ('x') + dd[i];	dd[i] += zz;	zz += z0;	}
			if(fall)	{	z0 = zp;	zz += z0;	zp = zz;	}

			c = z->v(i,mz)<0 ? c1 : c2;
			nn = mglPoint(-y->dvx(i,my),x->dvx(i,mx));
			p1 = mglPoint(x1,y1,zz);	p2 = mglPoint(x1,y1,z0);
			p3 = mglPoint(x2,y2,z0);	p4 = mglPoint(x2,y2,zz);
			if(wire)
			{
				n1 = gr->AddPnt(p1,c);	n2 = gr->AddPnt(p2,c);
				n3 = gr->AddPnt(p3,c);	n4 = gr->AddPnt(p4,c);
				gr->line_plot(n1,n2);	gr->line_plot(n1,n4);
				gr->line_plot(n3,n2);	gr->line_plot(n3,n4);
			}
			else
			{
				n1 = gr->AddPnt(p1,c,nn);	n2 = gr->AddPnt(p2,c,nn);
				n3 = gr->AddPnt(p3,c,nn);	n4 = gr->AddPnt(p4,c,nn);
				gr->quad_plot(n1,n2,n4,n3);
			}
		}
	}
	gr->EndGroup();	delete []dd;
}
//-----------------------------------------------------------------------------
void mgl_bars_xy(HMGL gr, HCDT x, HCDT y, const char *pen, const char *opt)
{
	long i,j,m,mx,my,n=y->GetNx(),pal;
	if(x->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"Bars");	return;	}
	if(n<2)		{	gr->SetWarn(mglWarnLow,"Bars");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Bars",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();

	bool wire = pen && strchr(pen,'#');
	bool above = pen && strchr(pen,'a')!=0, fall = pen && strchr(pen,'f')!=0;
	if(above)	fall = false;
	float c1,c2,c;
	long n1,n2,n3,n4;
	float *dd=new float[n], x1,x2,yy,y0,yp;
	memset(dd,0,n*sizeof(float));

	float zVal = gr->Min.z;
	gr->SetPenPal(pen,&pal);
	if(wire)	gr->Reserve(4*n*m);	else	gr->Reserve(4*n*m);
	for(j=0;j<m;j++)
	{
		gr->NextColor(pal);	c2=c1=gr->CDef;
		if(gr->GetNumPal(pal)==2*m)	c2 = gr->NextColor(pal);
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		yp = y0 = gr->GetOrgZ('x');
		for(i=0;i<n;i++)
		{
			if(i<n-1)	x2 = x->v(i,mx) + gr->BarWidth*(x->v(i+1,mx)-x->v(i,mx))/2;
			else		x2 = x->v(i,mx);
			if(i>0)		x1 = x->v(i,mx) - gr->BarWidth*(x->v(i,mx)-x->v(i-1,mx))/2;
			else		x1 = x->v(i,mx);
			yy = y->v(i,my);
			if(!above)
			{	x2 = (x2-x1)/m;		x1 += j*x2;		x2 += x1;	}
			else
			{	y0 = gr->GetOrgY('x') + dd[i];	dd[i] += yy;	yy += y0;	}
			if(fall)	{	y0 = yp;	yy += y0;	yp = yy;	}

			c = (y->v(i,my)<0) ? c1 : c2;
			n1 = gr->AddPnt(mglPoint(x1,yy,zVal),c);
			n2 = gr->AddPnt(mglPoint(x1,y0,zVal),c);
			n3 = gr->AddPnt(mglPoint(x2,y0,zVal),c);
			n4 = gr->AddPnt(mglPoint(x2,yy,zVal),c);
			if(wire)
			{
				gr->line_plot(n1,n2);	gr->line_plot(n1,n4);
				gr->line_plot(n3,n2);	gr->line_plot(n3,n4);
			}
			else	gr->quad_plot(n1,n2,n4,n3);
		}
	}
	gr->EndGroup();	delete []dd;
}
//-----------------------------------------------------------------------------
void mgl_bars(HMGL gr, HCDT y,	const char *pen, const char *opt)
{
	if(y->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"Bars");	return;	}
	gr->SaveState(opt);
	mglData x(y->GetNx());
	x.Fill(gr->Min.x,gr->Max.x);
	mgl_bars_xy(gr,&x,y,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_bars_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_bars_xyz(_GR_,_DA_(x),_DA_(y),_DA_(z),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_bars_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_bars_xy(_GR_,_DA_(x),_DA_(y),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_bars_(uintptr_t *gr, uintptr_t *y,	const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_bars(_GR_,_DA_(y),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//		Barh series
//
//-----------------------------------------------------------------------------
void mgl_barh_yx(HMGL gr, HCDT y, HCDT v, const char *pen, const char *opt)
{
	long i,j,m,mx,my,n=v->GetNx(),pal;
	if(y->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"Barh");	return;	}
	if(n<2)		{	gr->SetWarn(mglWarnLow,"Barh");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Barh",cgid++);
	m = y->GetNy() > v->GetNy() ? y->GetNy() : v->GetNy();

	bool wire = pen && strchr(pen,'#');
	bool above = pen && strchr(pen,'a'), fall = pen && strchr(pen,'f');
	if(above)	fall = false;
	float c1,c2,c;
	long n1,n2,n3,n4;
	float *dd=new float[n], y1,y2,xx,x0,xp;
	memset(dd,0,n*sizeof(float));

	float zVal = gr->Min.z;
	gr->SetPenPal(pen,&pal);
	if(wire)	gr->Reserve(4*n*m);	else	gr->Reserve(4*n*m);
	for(j=0;j<m;j++)
	{
		gr->NextColor(pal);	c2=c1=gr->CDef;
		if(gr->GetNumPal(pal)==2*m)	c2 = gr->NextColor(pal);
		mx = j<v->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		xp = x0 = gr->GetOrgX('y');
		for(i=0;i<n;i++)
		{
			if(i<n-1)	y2 = y->v(i,my) + gr->BarWidth*(y->v(i+1,my)-y->v(i,my))/2;
			else		y2 = y->v(i,my);
			if(i>0)		y1 = y->v(i,my) - gr->BarWidth*(y->v(i,my)-y->v(i-1,my))/2;
			else		y1 = y->v(i,my);
			xx = v->v(i,mx);
			if(!above)
			{	y2 = (y2-y1)/m;		y1 += j*y2;		y2 += y1;	}
			else
			{	x0 = gr->GetOrgX('y') + dd[i];	dd[i] += xx;	xx += x0;	}
			if(fall)	{	x0 = xp;	xx += x0;	xp = xx;	}

			c = (v->v(i,mx)<0) ? c1 : c2;
			n1 = gr->AddPnt(mglPoint(xx,y1,zVal),c);
			n2 = gr->AddPnt(mglPoint(xx,y2,zVal),c);
			n3 = gr->AddPnt(mglPoint(x0,y2,zVal),c);
			n4 = gr->AddPnt(mglPoint(x0,y1,zVal),c);
			if(wire)
			{
				gr->line_plot(n1,n2);	gr->line_plot(n1,n4);
				gr->line_plot(n3,n2);	gr->line_plot(n3,n4);
			}
			else	gr->quad_plot(n1,n2,n4,n3);
		}
	}
	gr->EndGroup();	delete []dd;
}
//-----------------------------------------------------------------------------
void mgl_barh(HMGL gr, HCDT v,	const char *pen, const char *opt)
{
	if(v->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"Barh");	return;	}
	gr->SaveState(opt);
	mglData y(v->GetNx());
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_barh_yx(gr,&y,v,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_barh_yx_(uintptr_t *gr, uintptr_t *y, uintptr_t *v, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_barh_yx(_GR_,_DA_(y),_DA_(v),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_barh_(uintptr_t *gr, uintptr_t *v,	const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_barh(_GR_,_DA_(v),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	BoxPlot series
//
//-----------------------------------------------------------------------------
double sgn(double a);
int mgl_cmp_flt(const void *a, const void *b)
{
	const float *aa = (const float *)a;
	const float *bb = (const float *)b;
	return int(sgn(*aa-*bb));
}
//-----------------------------------------------------------------------------
void mgl_boxplot_xy(HMGL gr, HCDT x, HCDT y, const char *pen, const char *opt)
{
	long n=y->GetNx(), m=y->GetNy();
	if(x->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"BoxPlot");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("BoxPlot",cgid++);
	float *b = new float[5*n], *d = new float[m], x1, x2;
	float zVal = gr->Min.z;
	register long i,j;
	for(i=0;i<n;i++)	// find quartiles by itself
	{
		register long mm,k;
		for(mm=j=0;j<m;j++)	if(!isnan(y->v(i,j)))
		{	d[mm]=y->v(i,j);	mm++;	}
		if(m==0)	{	b[i]=NAN;	break;	}
		qsort(d, mm, sizeof(float), mgl_cmp_flt);
		b[i] = d[0];	b[i+4*n] = d[mm-1];		k = mm/4;
		b[i+n] = (mm%4) ? d[k] : (d[k]+d[k-1])/2.;
		b[i+2*n] = (mm%2) ? d[mm/2] : (d[mm/2]+d[mm/2-1])/2.;
		b[i+3*n] = (mm%4) ? d[mm-k-1] : (d[mm-k-1]+d[mm-k])/2.;
	}
	delete []d;

	mglPoint p1,p2;
	long n1,n2,pal;
	gr->SetPenPal(pen,&pal);	gr->NextColor(pal);	gr->Reserve(18*n);
	for(i=0;i<n;i++)
	{
		if(i<n-1)	x2 = x->v(i) + gr->BarWidth*(x->v(i+1)-x->v(i))/2;
		else		x2 = x->v(i);
		if(i>0)		x1 = x->v(i) - gr->BarWidth*(x->v(i)-x->v(i-1))/2;
		else		x1 = x->v(i);
		for(j=0;j<5;j++)	// horizontal lines
		{
			p1=mglPoint(x1,b[i+j*n],zVal);	n1=gr->AddPnt(p1,gr->CDef);
			p2=mglPoint(x2,b[i+j*n],zVal);	n2=gr->AddPnt(p2,gr->CDef);
			gr->line_plot(n1,n2);
		}
		//vertical lines
		p1=mglPoint(x1,b[i+n],zVal);		n1=gr->AddPnt(p1,gr->CDef);
		p2=mglPoint(x1,b[i+3*n],zVal);		n2=gr->AddPnt(p2,gr->CDef);
		gr->line_plot(n1,n2);
		p1=mglPoint(x2,b[i+n],zVal);		n1=gr->AddPnt(p1,gr->CDef);
		p2=mglPoint(x2,b[i+3*n],zVal);		n2=gr->AddPnt(p2,gr->CDef);
		gr->line_plot(n1,n2);
		p1=mglPoint(x->v(i),b[i],zVal);		n1=gr->AddPnt(p1,gr->CDef);
		p2=mglPoint(x->v(i),b[i+n],zVal);	n2=gr->AddPnt(p2,gr->CDef);
		gr->line_plot(n1,n2);
		p1=mglPoint(x->v(i),b[i+3*n],zVal);	n1=gr->AddPnt(p1,gr->CDef);
		p2=mglPoint(x->v(i),b[i+4*n],zVal);	n2=gr->AddPnt(p2,gr->CDef);
		gr->line_plot(n1,n2);
	}
	delete []b;	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_boxplot(HMGL gr, HCDT y, const char *pen, const char *opt)
{
	if(y->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"BoxPlot");	return;	}
	gr->SaveState(opt);
	mglData x(y->GetNx());
	x.Fill(gr->Min.x,gr->Max.x);
	mgl_boxplot_xy(gr,&x,y,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_boxplot_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_boxplot_xy(_GR_,_DA_(x),_DA_(y),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_boxplot_(uintptr_t *gr, uintptr_t *y,	const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_boxplot(_GR_,_DA_(y),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//		Error series
//
//-----------------------------------------------------------------------------
void mgl_error_exy(HMGL gr, HCDT x, HCDT y, HCDT ex, HCDT ey, const char *pen, const char *opt)
{
	long i,j,m,mx,my,m1,m2,n=ey->GetNx(),pal;
	if(x->GetNx()!=n || y->GetNx()!=n || ex->GetNx()!=n )
	{	gr->SetWarn(mglWarnDim,"Error");	return;	}
	if(n<2)		{	gr->SetWarn(mglWarnLow,"Error");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Error",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();
	m = ex->GetNy() > m ? ex->GetNy() : m;
	m = ey->GetNy() > m ? ey->GetNy() : m;

	float zVal = gr->Min.z;
	char mk=gr->SetPenPal(pen,&pal);	gr->Reserve(5*n*m);
	long n1,n2;
	for(j=0;j<m;j++)
	{
		mx = j<x->GetNy() ? j:0;		my = j<y->GetNy() ? j:0;
		m1 = j<ex->GetNy() ? j:0;	m2 = j<ey->GetNy() ? j:0;
		gr->NextColor(pal);
		for(i=0;i<n;i++)
		{
			if(mk)	gr->mark_plot(gr->AddPnt(mglPoint(x->v(i,mx),y->v(i,my),zVal)), mk);

			n1 = gr->AddPnt(mglPoint(x->v(i,mx), y->v(i,my)+ey->v(i,m2), zVal));
			n2 = gr->AddPnt(mglPoint(x->v(i,mx), y->v(i,my)-ey->v(i,m2), zVal));
			gr->line_plot(n1,n2);	gr->arrow_plot(n1,n2,'I');	gr->arrow_plot(n2,n1,'I');

			n1 = gr->AddPnt(mglPoint(x->v(i,mx)+ex->v(i,m1), y->v(i,my), zVal));
			n2 = gr->AddPnt(mglPoint(x->v(i,mx)-ex->v(i,m1), y->v(i,my), zVal));
			gr->line_plot(n1,n2);	gr->arrow_plot(n1,n2,'I');	gr->arrow_plot(n2,n1,'I');
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_error_xy(HMGL gr, HCDT x, HCDT y, HCDT ey, const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData ex(y->GetNx());
	ex.Fill(NAN,NAN);
	mgl_error_exy(gr,x,y,&ex,ey,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_error(HMGL gr, HCDT y, HCDT ey, const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData x(y->GetNx());
	x.Fill(gr->Min.x,gr->Max.x);
	mgl_error_xy(gr,&x,y,ey,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_error_(uintptr_t *gr, uintptr_t *y, uintptr_t *ey, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_error(_GR_,_DA_(y),_DA_(ey),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_error_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ey, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_error_xy(_GR_,_DA_(x),_DA_(y),_DA_(ey),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_error_exy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ex, uintptr_t *ey, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_error_exy(_GR_,_DA_(x),_DA_(y),_DA_(ex),_DA_(ey),s,o);
	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//		Chart series
//
//-----------------------------------------------------------------------------
void face_plot(mglBase *gr, mglPoint o, mglPoint d1, mglPoint d2, float c, bool wire)
{
	const int num=40;
	mglPoint p,nn=d1^d2;
	d1 = d1/num;	d2 = d2/num;
	register long i,j,i0,n=num+1;
	long *id=new long[n*n];
	gr->Reserve(n*n);
	for(j=0;j<n;j++)	for(i=0;i<n;i++)
	{	p = o+d1*i+d2*j;	id[i+n*j] = gr->AddPnt(p,c,nn);	}
	for(i=0;i<num;i++)	for(j=0;j<num;j++)
	{
		i0 = i+n*j;
		gr->quad_plot(id[i0],id[i0+1],id[i0+n],id[i0+n+1]);
	}
	if(wire)
	{
		gr->Reserve(4*n);	gr->SetPenPal("k-");
		long *jj=id+n+1;
		jj[0] = jj[1] = gr->CopyNtoC(id[0],gr->CDef);
		jj[2] = jj[3] = gr->CopyNtoC(id[n*n-1],gr->CDef);
		for(i=1;i<n;i++)
		{
			memcpy(jj+4,jj,4*sizeof(long));
			jj[0] = gr->CopyNtoC(id[i],gr->CDef);
			jj[1] = gr->CopyNtoC(id[n*i],gr->CDef);
			jj[2] = gr->CopyNtoC(id[n*n-1-i],gr->CDef);
			jj[3] = gr->CopyNtoC(id[n*n-1-n*i],gr->CDef);
			gr->line_plot(jj[4],jj[0]);
			gr->line_plot(jj[5],jj[1]);
			gr->line_plot(jj[6],jj[2]);
			gr->line_plot(jj[7],jj[3]);
		}
	}
	delete []id;
}
//-----------------------------------------------------------------------------
void mgl_chart(HMGL gr, HCDT a, const char *cols, const char *opt)
{
	if(a->Minimal()<0)	{	gr->SetWarn(mglWarnNeg,"Chart");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Chart",cgid++);
	bool wire = false;	// draw edges
	register long n=a->GetNx(),i,j;
	if(cols && !strcmp(cols,"#"))	{	wire = true;	cols = 0;	}
	if(!cols)	cols = MGL_DEF_PAL;
	float *c = new float[strlen(cols)+1],cc;
	long nc=0;			// number of colors
	for(i=0;i<long(strlen(cols));i++)
	{
		if(strchr("wkrgbcymhRGBCYMHWlenuqpLENUQP ",cols[i]))
		{	c[nc]=gr->AddTexture(cols[i]);	nc++;	}
		else if(cols[i]=='#')	wire = true;
	}

	float dy = (gr->Max.y-gr->Min.y)/a->GetNy(), dx, ss, cs, x1, y1, dz=gr->Max.z-gr->Min.z;
	mglPoint d1,d2,o;

	for(j=0;j<a->GetNy();j++)
	{
		y1 = gr->Min.y + dy*j;
		for(i=0,ss=0;i<n;i++)	ss += a->v(i,j);
		if(ss==0)	continue;
		for(cs=0,i=0;i<n;i++)
		{
			dx = a->v(i,j)/ss;	cc = c[i%nc];
			if(dx==0)	continue;
			x1 = gr->Min.x + (gr->Max.x-gr->Min.x)*cs/ss;	dx *= (gr->Max.x-gr->Min.x);
			if(cc>=0)
			{
				face_plot(gr,mglPoint(x1,y1,gr->Min.z),mglPoint(dx,0,0),mglPoint(0,0,dz),cc,wire);
				face_plot(gr,mglPoint(x1,y1,gr->Min.z),mglPoint(dx,0,0),mglPoint(0,dy,0),cc,wire);
				face_plot(gr,mglPoint(x1,y1,gr->Min.z),mglPoint(0,dy,0),mglPoint(0,0,dz),cc,wire);

				face_plot(gr,mglPoint(x1+dx,y1+dy,gr->Max.z),mglPoint(-dx,0,0),mglPoint(0,0,-dz),cc,wire);
				face_plot(gr,mglPoint(x1+dx,y1+dy,gr->Max.z),mglPoint(-dx,0,0),mglPoint(0,-dy,0),cc,wire);
				face_plot(gr,mglPoint(x1+dx,y1+dy,gr->Max.z),mglPoint(0,-dy,0),mglPoint(0,0,-dz),cc,wire);
			}
			cs += a->v(i,j);
		}
	}
	gr->EndGroup();	delete []c;
}
//-----------------------------------------------------------------------------
void mgl_chart_(uintptr_t *gr, uintptr_t *a, const char *col, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,col,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_chart(_GR_, _DA_(a), s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Mark series
//
//-----------------------------------------------------------------------------
void mgl_mark_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT r, const char *pen, const char *opt)
{
	long j,m,mx,my,mz,mr,n=y->GetNx(),pal;
	if(x->GetNx()!=n || z->GetNx()!=n || r->GetNx()!=n)
	{	gr->SetWarn(mglWarnDim,"Mark");	return;	}
	if(n<2)	{	gr->SetWarn(mglWarnLow,"Mark");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Mark",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();
	m = z->GetNy() > m ? z->GetNy() : m;
	char mk=gr->SetPenPal(pen,&pal);	gr->Reserve(n*m);
	if(mk==0)	return;

	for(j=0;j<m;j++)
	{
		gr->NextColor(pal);
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		mz = j<z->GetNy() ? j:0;	mr = j<r->GetNy() ? j:0;
		for(int i=0;i<n;i++)
			gr->mark_plot(gr->AddPnt(mglPoint(x->v(i,mx),y->v(i,my),z->v(i,mz))), mk, r->v(i,mr));
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_mark_xy(HMGL gr, HCDT x, HCDT y, HCDT r, const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData z(y->GetNx());
	z.Fill(gr->Min.z,gr->Min.z);
	mgl_mark_xyz(gr,x,y,&z,r,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_mark_y(HMGL gr, HCDT y, HCDT r, const char *pen, const char *opt)
{
	if(y->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"Mark");	return;	}
	gr->SaveState(opt);
	mglData x(y->GetNx()), z(y->GetNx());
	x.Fill(gr->Min.x,gr->Max.x);
	z.Fill(gr->Min.z,gr->Min.z);
	mgl_mark_xyz(gr,&x,y,&z,r,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_mark_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *r, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_mark_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(r),s,o);
	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_mark_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *r, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_mark_xy(_GR_, _DA_(x), _DA_(y), _DA_(r),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_mark_y_(uintptr_t *gr, uintptr_t *y, uintptr_t *r, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_mark_y(_GR_,_DA_(y),_DA_(r),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Tube series
//
//-----------------------------------------------------------------------------
void mgl_tube_xyzr(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT r, const char *pen, const char *opt)
{
	long j,m,mx,my,mz,mr,n=y->GetNx(),pal;
	if(n<2)	{	gr->SetWarn(mglWarnLow,"Tube");	return;	}
	if(x->GetNx()!=n || z->GetNx()!=n || r->GetNx()!=n)
	{	gr->SetWarn(mglWarnDim,"Tube");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Tube",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();
	m = z->GetNy() > m ? z->GetNy() : m;
	m = r->GetNy() > m ? r->GetNy() : m;

	const int num=41;
	gr->SetPenPal(pen,&pal);
	gr->Reserve(n*m*num);
	mglPoint p,l,t,u,q,d;
	long *nn=new long[2*num];
	memset(nn,-1,2*num*sizeof(long));
	for(j=0;j<m;j++)
	{
		gr->NextColor(pal);
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		mz = j<z->GetNy() ? j:0;	mr = j<r->GetNy() ? j:0;
		register long i,k;
		for(i=0;i<n;i++)
		{
			l = mglPoint(x->dvx(i,mx),y->dvx(i,my),z->dvx(i,mz));
			t = !l;		t /= Norm(t);	u = t^l;	u /= Norm(u);
			q = mglPoint(x->v(i,mx),y->v(i,my),z->v(i,mz));
			float si,co,ff, rr=r->v(i,mr), dr=r->dvx(i,mr);
			for(k=0;k<num;k++)
			{
				ff = k*2*M_PI/(num-1);	co = cos(ff);	si = sin(ff);
				p = q + t*(rr*co) + u*(rr*si);
				d = (t*si - u*co)^(l + t*(dr*co) + u*(dr*si));
				nn[k+num]=nn[k];	nn[k] = gr->AddPnt(p,gr->CDef,d,-1,3);
				if(i*k>0)
					gr->quad_plot(nn[k],nn[k-1],nn[k+num],nn[k+num-1]);
			}
		}
	}
	delete []nn;	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_tube_xyr(HMGL gr, HCDT x, HCDT y, HCDT r, const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData z(y->GetNx());
	z.Fill(gr->Min.z,gr->Min.z);
	mgl_tube_xyzr(gr,x,y,&z,r,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_tube_r(HMGL gr, HCDT y, HCDT r, const char *pen, const char *opt)
{
	if(y->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"Tube");	return;	}
	gr->SaveState(opt);
	mglData x(y->GetNx()), z(y->GetNx());
	x.Fill(gr->Min.x,gr->Max.x);
	z.Fill(gr->Min.z,gr->Min.z);
	mgl_tube_xyzr(gr,&x,y,&z,r,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_tube(HMGL gr, HCDT y, float rr, const char *pen, const char *opt)
{
	if(y->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"Tube");	return;	}
	gr->SaveState(opt);
	mglData x(y->GetNx()), r(y->GetNx()), z(y->GetNx());
	x.Fill(gr->Min.x,gr->Max.x);
	r.Fill(rr,rr);
	z.Fill(gr->Min.z,gr->Min.z);
	mgl_tube_xyzr(gr,&x,y,&z,&r,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_tube_xy(HMGL gr, HCDT x, HCDT y, float rr, const char *pen, const char *opt)
{
	if(y->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"Tube");	return;	}
	gr->SaveState(opt);
	mglData r(y->GetNx()), z(y->GetNx());
	r.Fill(rr,rr);
	z.Fill(gr->Min.z,gr->Min.z);
	mgl_tube_xyzr(gr,x,y,&z,&r,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_tube_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, float rr, const char *pen, const char *opt)
{
	if(y->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"Tube");	return;	}
	gr->SaveState(opt);
	mglData r(y->GetNx());
	r.Fill(rr,rr);
	mgl_tube_xyzr(gr,x,y,z,&r,pen,0);
}
//-----------------------------------------------------------------------------
void mgl_tube_xyzr_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *r, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_tube_xyzr(_GR_,_DA_(x),_DA_(y),_DA_(z), _DA_(r),s,o);
	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_tube_xyr_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *r, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_tube_xyr(_GR_,_DA_(x),_DA_(y),_DA_(r),s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_tube_r_(uintptr_t *gr, uintptr_t *y, uintptr_t *r, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_tube_r(_GR_,_DA_(y),_DA_(r),s,o);	delete []s;	delete []o;	}
//-----------------------------------------------------------------------------
void mgl_tube_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, float *r, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_tube_xyz(_GR_,_DA_(x),_DA_(y),_DA_(z),*r,s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_tube_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, float *r, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_tube_xy(_GR_,_DA_(x),_DA_(y),*r,s,o);	delete []s;	delete []o;	}
//-----------------------------------------------------------------------------
void mgl_tube_(uintptr_t *gr, uintptr_t *y, float *r, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_tube(_GR_,_DA_(y),*r,s,o);
	delete []s;	delete []o;	}
//-----------------------------------------------------------------------------
