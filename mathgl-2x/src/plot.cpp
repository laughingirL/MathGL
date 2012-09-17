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
#include "mgl2/plot.h"
#include "mgl2/eval.h"
#include "mgl2/data.h"
//-----------------------------------------------------------------------------
//
//	Plot by formulas series
//
//-----------------------------------------------------------------------------
void mgl_fplot(HMGL gr, const char *eqY, const char *pen, const char *opt)
{
	if(eqY==0 || eqY[0]==0)	return;		// nothing to plot
	mreal r = gr->SaveState(opt);
	long n = (mgl_isnan(r) || r<=0) ? 100:long(r+0.5);

	mreal *x = (mreal *)malloc(n*sizeof(mreal));
	mreal *y = (mreal *)malloc(n*sizeof(mreal));
	mglFormula *eq = new mglFormula(eqY);
	register int i;
	mreal d = (gr->Max.x - gr->Min.x)/(n-1.), xs, ys, yr, ym=fabs(gr->Max.y - gr->Min.y)/1000;
#define islog(a, b) (((a)>0 && (b)>10*(a)) || ((b)<0 && (a)<10*(b)))
	// initial data filling
	if(gr->Min.x>0 && gr->Max.x>100*gr->Min.x)	for(i=0,d=log(2*gr->Max.x/gr->Min.x)/(n-1);i<n;i++)
	{	x[i]=2*gr->Max.x*exp(d*i)/(2*gr->Max.x/gr->Min.x+exp(d*i));	y[i]=eq->Calc(x[i]);	}
	else if(gr->Max.x<0 && gr->Min.x<100*gr->Max.x)	for(i=0,d=log(2*gr->Min.x/gr->Max.x)/n;i<n;i++)
	{	x[i]=2*gr->Min.x*exp(d*i)/(2*gr->Min.x/gr->Max.x+exp(d*i));	y[i]=eq->Calc(x[i]);	}
	else for(i=0;i<n;i++)
	{	x[i]=gr->Min.x + i*d;	y[i]=eq->Calc(x[i]);	}

	for(i=0;i<n-1 && n<10000;)
	{
		if(gr->Stop)	{	delete eq;	return;	}
		xs=(x[i]+x[i+1])/2;
		ys=(y[i]+y[i+1])/2;	yr=eq->Calc(xs);
		if(fabs(yr-ys)>ym)	// bad approximation here
		{
			x = (mreal *)realloc(x,(n+1)*sizeof(mreal));
			y = (mreal *)realloc(y,(n+1)*sizeof(mreal));
			memmove(x+i+2,x+i+1,(n-i-1)*sizeof(mreal));
			memmove(y+i+2,y+i+1,(n-i-1)*sizeof(mreal));
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
	mreal r = gr->SaveState(opt);
	long n = (mgl_isnan(r) || r<=0) ? 100:long(r+0.5);

	mreal *x = (mreal *)malloc(n*sizeof(mreal));
	mreal *y = (mreal *)malloc(n*sizeof(mreal));
	mreal *z = (mreal *)malloc(n*sizeof(mreal));
	mreal *t = (mreal *)malloc(n*sizeof(mreal));
	mglFormula *ex, *ey, *ez;
	ex = new mglFormula(eqX ? eqX : "0");
	ey = new mglFormula(eqY ? eqY : "0");
	ez = new mglFormula(eqZ ? eqZ : "0");
	register int i;
	mreal ts, xs, ys, zs, xr, yr, zr, xm=fabs(gr->Max.x - gr->Min.x)/1000, ym=fabs(gr->Max.y - gr->Min.y)/1000, zm=fabs(gr->Max.z - gr->Min.z)/1000;
	for(i=0;i<n;i++)	// initial data filling
	{
		if(gr->Stop)	{	delete ex;	delete ey;	delete ez;	return;	}
		t[i] = i/(n-1.);
		x[i] = ex->Calc(0,0,t[i]);
		y[i] = ey->Calc(0,0,t[i]);
		z[i] = ez->Calc(0,0,t[i]);
	}

	for(i=0;i<n-1 && n<10000;)
	{
		if(gr->Stop)	{	delete ex;	delete ey;	delete ez;	return;	}
		ts=(t[i]+t[i+1])/2;
		xs=(x[i]+x[i+1])/2;	xr=ex->Calc(0,0,ts);
		ys=(y[i]+y[i+1])/2;	yr=ey->Calc(0,0,ts);
		zs=(z[i]+z[i+1])/2;	zr=ez->Calc(0,0,ts);
		if(fabs(xr-xs)>xm || fabs(yr-ys)>ym || fabs(zr-zs)>zm)	// bad approximation here
		{
			z = (mreal *)realloc(z,(n+1)*sizeof(mreal));
			t = (mreal *)realloc(t,(n+1)*sizeof(mreal));
			x = (mreal *)realloc(x,(n+1)*sizeof(mreal));
			y = (mreal *)realloc(y,(n+1)*sizeof(mreal));
			memmove(x+i+2,x+i+1,(n-i-1)*sizeof(mreal));
			memmove(y+i+2,y+i+1,(n-i-1)*sizeof(mreal));
			memmove(z+i+2,z+i+1,(n-i-1)*sizeof(mreal));
			memmove(t+i+2,t+i+1,(n-i-1)*sizeof(mreal));
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
	mreal m=a->Minimal(), r=gr->SaveState(opt);
	if(mgl_isnan(r) || r<0)	r = m<0 ? -m:0;
	register long i,j;
	for(j=0;j<ny;j++)
	{
		for(i=0;i<n;i++)
		{
			register mreal v = a->v(i,j);
			x.a[i+(n+1)*j] = (r+v)*cos(2*i*M_PI/n);
			y.a[i+(n+1)*j] = (r+v)*sin(2*i*M_PI/n);
		}
		x.a[n+(n+1)*j] = r+a->v(0,j);	y.a[n+(n+1)*j] = 0;
	}
	mgl_plot_xy(gr,&x,&y,pen,0);
	if(mglchr(pen,'#'))	// draw "grid"
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
//	Candle series
//
//-----------------------------------------------------------------------------
void mgl_candle_xyv(HMGL gr, HCDT x, HCDT v1, HCDT v2, HCDT y1, HCDT y2, const char *pen, const char *opt)
{
	long i,n=v1->GetNx(),pal,nx=x->GetNx();
	bool d1=false,d2=false;
	if(!y1)	{	y1 = new mglData(n);	d1=true;	((mglData *)y1)->Fill(NAN,NAN);	}
	if(!y2)	{	y2 = new mglData(n);	d2=true;	((mglData *)y2)->Fill(NAN,NAN);	}
	if(n<2)	{	gr->SetWarn(mglWarnLow,"Candle");	return;	}
	if(nx<n || v2->GetNx()!=n || y1->GetNx()!=n || y2->GetNx()!=n)
	{	gr->SetWarn(mglWarnDim,"Candle");	return;	}
	static int cgid=1;	gr->StartGroup("Candle",cgid++);
	gr->SaveState(opt);	gr->SetPenPal(pen,&pal);
	gr->NextColor(pal);	gr->Reserve(8*n);
	bool sh = mglchr(pen,'!');

	long n1,n2,n3,n4;
	mreal m1,m2,xx,x1,x2,d;
	mreal zm = gr->AdjustZMin();
	for(i=0;i<n;i++)
	{
		if(gr->Stop)	{	if(d1)	delete y1;	if(d2)	delete y2;	return;	}
		m1=v1->v(i);	m2 = v2->v(i);	xx = x->v(i);
		d = i<nx-1 ? x->v(i+1)-xx : xx-x->v(i-1);
		x1 = xx + d/2*(1-gr->BarWidth);
		x2 = x1 + gr->BarWidth*d;	xx = (x1+x2)/2;
		n1 = gr->AddPnt(mglPoint(xx,y1->v(i),zm));
		n2 = gr->AddPnt(mglPoint(xx,m1,zm));
		gr->line_plot(n1,n2);
		n3 = gr->AddPnt(mglPoint(xx,y2->v(i),zm));
		n4 = gr->AddPnt(mglPoint(xx,m2,zm));
		gr->line_plot(n3,n4);

		n1 = gr->AddPnt(mglPoint(x1,m1,zm));
		n2 = gr->AddPnt(mglPoint(x2,m1,zm));
		n3 = gr->AddPnt(mglPoint(x1,m2,zm));
		n4 = gr->AddPnt(mglPoint(x2,m2,zm));
		gr->line_plot(n1,n2);	gr->line_plot(n1,n3);
		gr->line_plot(n4,n2);	gr->line_plot(n4,n3);
		if(m1>m2)	gr->quad_plot(n1,n2,n3,n4);
		if(sh)	gr->NextColor(pal);
	}
	if(d1)	delete y1;	if(d2)	delete y2;
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_candle_yv(HMGL gr, HCDT v1, HCDT v2, HCDT y1, HCDT y2, const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData x(v1->GetNx()+1);
	x.Fill(gr->Min.x,gr->Max.x);
	mgl_candle_xyv(gr,&x,v1,v2,y1,y2,pen,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void mgl_candle(HMGL gr, HCDT v1, HCDT y1, HCDT y2, const char *pen, const char *opt)
{
	mglData v2(v1);
	v2.Roll('x',1);	v2.a[0]=NAN;
	mgl_candle_yv(gr,v1,&v2,y1,y2,pen,opt);
}
//-----------------------------------------------------------------------------
void mgl_candle_xyv_(uintptr_t *gr, uintptr_t *x, uintptr_t *v1, uintptr_t *v2, uintptr_t *y1, uintptr_t *y2, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_candle_xyv(_GR_,_DA_(x),_DA_(v1),_DA_(v2),_DA_(y1),_DA_(y2),s,o);	delete []s;	delete []o;	}
//-----------------------------------------------------------------------------
void mgl_candle_yv_(uintptr_t *gr, uintptr_t *v1, uintptr_t *v2, uintptr_t *y1, uintptr_t *y2, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_candle_yv(_GR_,_DA_(v1),_DA_(v2),_DA_(y1),_DA_(y2),s,o);	delete []s;	delete []o;	}
//-----------------------------------------------------------------------------
void mgl_candle_(uintptr_t *gr, uintptr_t *y, uintptr_t *y1, uintptr_t *y2, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_candle(_GR_,_DA_(y),_DA_(y1),_DA_(y2),s,o);
	delete []s;	delete []o;	}
//-----------------------------------------------------------------------------
//
//	Plot series
//
//-----------------------------------------------------------------------------
void mgl_plot_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, const char *pen, const char *opt)
{
	long j,m,mx,my,mz,n=y->GetNx(),pal;
	if(mgl_check_dim1(gr,x,y,z,0,"Plot"))	return;

	static int cgid=1;	gr->StartGroup("Plot",cgid++);
	gr->SaveState(opt);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();	m = z->GetNy() > m ? z->GetNy() : m;
	char mk=gr->SetPenPal(pen,&pal);	gr->Reserve(2*n*m);
	bool t1,t2,t3;
	mglPoint p1,nn,p2,pt;
	long n1=-1,n2=-1,n3=-1;
	bool sh = mglchr(pen,'!');

	for(j=0;j<m;j++)
	{
		mx = j<x->GetNy() ? j:0;		my = j<y->GetNy() ? j:0;
		mz = j<z->GetNy() ? j:0;		gr->NextColor(pal);
		t1 = t2 = false;
		register long i;
		for(i=0;i<n;i++)
		{
			if(gr->Stop)	return;
			if(i>0)	{	n2=n1;	p2 = p1;	t2=t1;	}
			p1 = mglPoint(x->v(i,mx), y->v(i,my), z->v(i,mz));
			n1 = gr->AddPnt(p1);	t1 = n1>=0;
			if(mk && t1)	gr->mark_plot(n1,mk);
			if(t1 && t2)
			{
				gr->line_plot(n1,n2);
				if(i==0)	gr->arrow_plot(n1,n2,gr->Arrow1);
				if(i==n-1)	gr->arrow_plot(n1,n2,gr->Arrow2);
			}

			if(i>0 && ((t1 && !t2) || (t2 && !t1)))	// do smoothing
			{
				mreal i1=0, i2=1, ii;
				t3=t1;	n3=n1;	// copy current
				do {
					ii = (i1+i2)/2;
					pt.x = p1.x*ii+p2.x*(1-ii);
					pt.y = p1.y*ii+p2.y*(1-ii);
					pt.z = p1.z*ii+p2.z*(1-ii);
					n1 = gr->AddPnt(pt);	t1 = n1>=0;
					if((t1 && t3) || (t2 && !t1))	i2 = ii;
					else	i1 = ii;
				} while(fabs(i2-i1)>1e-3);
				if(t2)	gr->line_plot(n1,n2);
				else	gr->line_plot(n3,n1);
			}
			if(sh)	gr->NextColor(pal);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_plot_xy(HMGL gr, HCDT x, HCDT y, const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData z(y->GetNx());
	mreal zm = gr->AdjustZMin();	z.Fill(zm,zm);
	mgl_plot_xyz(gr,x,y,&z,pen,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void mgl_plot(HMGL gr, HCDT y, const char *pen, const char *opt)
{
	register long n=y->GetNx();
	if(n<2)	{	gr->SetWarn(mglWarnLow,"Plot");	return;	}
	gr->SaveState(opt);
	mglData x(n), z(n);
	x.Fill(gr->Min.x,gr->Max.x);
	mreal zm = gr->AdjustZMin();	z.Fill(zm,zm);
	mgl_plot_xyz(gr,&x,y,&z,pen,0);
	gr->LoadState();
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
	if(mgl_check_dim1(gr,x,y,z,0,"Tens"))	return;

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Tens",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();	m = z->GetNy() > m ? z->GetNy() : m;
	char mk=gr->SetPenPal(pen, &pal);	gr->Reserve(2*n*m);
	long ss=gr->AddTexture(pen);
	bool t1,t2,t3;
	mglPoint p1,p2,pt,nn;
	long n1=-1,n2=-1,n3=-1;

	for(j=0;j<m;j++)
	{
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		mz = j<z->GetNy() ? j:0;	mc = j<c->GetNy() ? j:0;
		register long i;
		t1 = t2 = false;
		for(i=0;i<n;i++)
		{
			if(gr->Stop)	return;
			if(i>0)	{	n2=n1;	p2=p1;	t2=t1;	}
			p1 = mglPoint(x->v(i,mx), y->v(i,my), z->v(i,mz), c->v(i,mc));
			n1 = gr->AddPnt(p1,gr->GetC(ss,p1.c));	t1 = n1>=0;
			if(mk && t1)	gr->mark_plot(n1,mk);
			if(t1 && t2)
			{
				gr->line_plot(n1,n2);
				if(i==0)	gr->arrow_plot(n1,n2,gr->Arrow1);
				if(i==n-1)	gr->arrow_plot(n1,n2,gr->Arrow2);
			}

			if(i>0 && ((t1 && !t2) || (t2 && !t1)))	// do smoothing
			{
				mreal i1=0, i2=1, ii;
				t3=t1;	n3=n1;	// copy current
				do {
					ii = (i1+i2)/2;
					pt.x = p1.x*ii+p2.x*(1-ii);
					pt.y = p1.y*ii+p2.y*(1-ii);
					pt.z = p1.z*ii+p2.z*(1-ii);
					pt.c = p1.c*ii+p2.c*(1-ii);
					n1 = gr->AddPnt(pt,gr->GetC(ss,pt.c));	t1 = n1>=0;
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
	mreal zm = gr->AdjustZMin();	z.Fill(zm,zm);
	mgl_tens_xyz(gr,x,y,&z,c,pen,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void mgl_tens(HMGL gr, HCDT y, HCDT c, const char *pen, const char *opt)
{
	register long n=y->GetNx();
	if(n<2)	{	gr->SetWarn(mglWarnLow,"Tens");	return;	}
	gr->SaveState(opt);
	mglData x(n), z(n);
	x.Fill(gr->Min.x,gr->Max.x);
	mreal zm = gr->AdjustZMin();	z.Fill(zm,zm);
	mgl_tens_xyz(gr,&x,y,&z,c,pen,0);
	gr->LoadState();
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
	if(mgl_check_dim1(gr,x,y,z,0,"Area"))	return;

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Area3",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();	m = z->GetNy() > m ? z->GetNy() : m;
	bool sh = mglchr(pen,'!');
	bool wire = mglchr(pen,'#');

	mreal z0=gr->GetOrgZ('x');
	mreal c1,c2;
	mglPoint p1,p2,p3,p4,nn;
	long n1,n2,n3,n4;
	gr->SetPenPal(pen,&pal);	gr->Reserve(2*n*m);
//	long s=gr->AddTexture(pen,1);
	for(j=0;j<m;j++)
	{
		c2=c1=gr->NextColor(pal);
		if(gr->GetNumPal(pal)==2*m && !sh)	c2 = gr->NextColor(pal);
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;	mz = j<z->GetNy() ? j:0;

		nn = mglPoint(-y->dvx(0,my),x->dvx(0,mx));
		mglPoint p = mglPoint(x->v(0,mx),y->v(0,my),z->v(0,mz));
		n1 = gr->AddPnt(p,c1,nn);	p.z = z0;	n2 = gr->AddPnt(p,c2,nn);
		for(i=1;i<n;i++)
		{
			if(gr->Stop)	return;
			n3=n1;	n4=n2;
			nn = mglPoint(-y->dvx(i,my),x->dvx(i,mx));
			p = mglPoint(x->v(i,mx),y->v(i,my),z->v(i,mz));
			n1 = gr->AddPnt(p,c1,nn);	p.z = z0;	n2 = gr->AddPnt(p,c2,nn);
			if(wire)
			{
				gr->line_plot(n1,n2);	gr->line_plot(n3,n4);
				gr->line_plot(n1,n3);	gr->line_plot(n4,n2);
			}
			else	gr->quad_plot(n1,n2,n3,n4);
			if(sh)	c2=c1=gr->NextColor(pal);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_area_xy(HMGL gr, HCDT x, HCDT y, const char *pen, const char *opt)
{
	long i,j,n=y->GetNx(),m=y->GetNy(),mx,my,pal;
	if(mgl_check_dim1(gr,x,y,0,0,"Area"))	return;

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Curve",cgid++);
	mreal zm = gr->AdjustZMin();
	mreal y0=gr->GetOrgY('x'), z0;
	mreal c1,c2;
	mglPoint nn=mglPoint(0,0,1);
	long n1,n2,n3,n4;
	bool sh = mglchr(pen,'!');
	bool wire = mglchr(pen,'#');

	gr->SetPenPal(pen,&pal);	gr->Reserve(2*n*m);
//	long s=gr->AddTexture(pen,1);
	for(j=0;j<m;j++)
	{
		c2=c1=gr->NextColor(pal);
		if(gr->GetNumPal(pal)==2*m && !sh)	c2=gr->NextColor(pal);
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		z0 = zm + (m-1-j)*(gr->Max.z-zm)/m;

		mglPoint p = mglPoint(x->v(0,mx),y->v(0,my),z0);
		n1 = gr->AddPnt(p,c1,nn);	p.y = y0;	n2 = gr->AddPnt(p,c2,nn);
		for(i=1;i<n;i++)
		{
			if(gr->Stop)	return;
			n3=n1;	n4=n2;
			p = mglPoint(x->v(i,mx),y->v(i,my),z0);
			n1 = gr->AddPnt(p,c1,nn);	p.y = y0;	n2 = gr->AddPnt(p,c2,nn);
			if(wire)
			{
				gr->line_plot(n1,n2);	gr->line_plot(n3,n4);
				gr->line_plot(n1,n3);	gr->line_plot(n4,n2);
			}
			else	gr->quad_plot(n1,n2,n3,n4);
			if(sh)	c2=c1=gr->NextColor(pal);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_area(HMGL gr, HCDT y, const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData x(y->GetNx());
	x.Fill(gr->Min.x,gr->Max.x);
	mgl_area_xy(gr,&x,y,pen,0);
	gr->LoadState();
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
	if(mgl_check_dim1(gr,x,y1,y2,0,"Region"))	return;
	if(y2->GetNy()!=m)	{	gr->SetWarn(mglWarnDim,"Region");	return;	}

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Region",cgid++);
	mreal c1,c2;
	mglPoint nn=mglPoint(0,0,1);
	long n1,n2,n3,n4;
	mreal xx,f1,f2,f3,f4;
	mreal zm = gr->AdjustZMin();
	bool inside = (mglchr(pen,'i'));	// NOTE: check if 'i' is free (used here for inside flag)
	bool sh = mglchr(pen,'!');

	gr->SetPenPal(pen,&pal);	gr->Reserve(2*n*m);
//	long s=gr->AddTexture(pen,1);
	for(j=0;j<m;j++)
	{
		c2=c1=gr->NextColor(pal);
		if(gr->GetNumPal(pal)==2*m && !sh)	c2=gr->NextColor(pal);
		mx = j<x->GetNy() ? j:0;
		mreal z0 = zm + (m-1-j)*(gr->Max.z-zm)/m;

		f1 = y1->v(0,j);	f2 = y2->v(0,j);	xx = x->v(0,mx);
		n1 = gr->AddPnt(mglPoint(xx,f1,z0),c1,nn);
		n2 = gr->AddPnt(mglPoint(xx,f2,z0),c2,nn);
		for(i=1;i<n;i++)
		{
			if(gr->Stop)	return;
			n3=n1;	n4=n2;	f3=f1;	f4=f2;
			f1 = y1->v(i,j);	f2 = y2->v(i,j);	xx = x->v(i,mx);
			n1 = gr->AddPnt(mglPoint(xx,f1,z0),c1,nn);
			n2 = gr->AddPnt(mglPoint(xx,f2,z0),c2,nn);
			if(!inside || (f2>f1 && f4>f3))	gr->quad_plot(n1,n2,n3,n4);
			if(sh)	c2=c1=gr->NextColor(pal);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_region(HMGL gr, HCDT y1, HCDT y2, const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData x(y1->GetNx());
	x.Fill(gr->Min.x, gr->Max.x);
	mgl_region_xy(gr,&x,y1,y2,pen,0);
	gr->LoadState();
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
	if(mgl_check_dim1(gr,x,y,z,0,"Step"))	return;

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Step3",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();	m = z->GetNy() > m ? z->GetNy() : m;
	bool sh = mglchr(pen,'!');

	char mk=gr->SetPenPal(pen,&pal);	gr->Reserve(2*n*m);
	long n1,n2;
	mglPoint p;
	for(j=0;j<m;j++)
	{
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		mz = j<z->GetNy() ? j:0;	gr->NextColor(pal);
		n1 = gr->AddPnt(mglPoint(x->v(0,mx), y->v(0,my), z->v(0,mz)));
		if(mk)	gr->mark_plot(n1,mk);
		for(i=1;i<n;i++)
		{
			if(gr->Stop)	return;
			n2 = n1;	// horizontal
			p = mglPoint(x->v(i,mx), y->v(i,my), z->v(i-1,mz));
			n1 = gr->AddPnt(p);	gr->line_plot(n1,n2);
			if(i==1)	gr->arrow_plot(n1,n2,gr->Arrow1);

			n2 = n1;	// vertical
			p.z = z->v(i,mz);	n1 = gr->AddPnt(p);
			if(mk)	gr->mark_plot(n1,mk);
			gr->line_plot(n1,n2);
			if(i==n-1)	gr->arrow_plot(n1,n2,gr->Arrow2);
			if(sh)	gr->NextColor(pal);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_step_xy(HMGL gr, HCDT x, HCDT y, const char *pen, const char *opt)
{
	long i,j,m,mx,my,n=y->GetNx(), pal;
	if(mgl_check_dim1(gr,x,y,0,0,"Step"))	return;

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Step",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();
	bool sh = mglchr(pen,'!');

	mreal zVal =gr->AdjustZMin();
	char mk=gr->SetPenPal(pen,&pal);	gr->Reserve(2*n*m);
	long n1,n2;
	mglPoint p;
	for(j=0;j<m;j++)
	{
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		gr->NextColor(pal);
		n1 = gr->AddPnt(mglPoint(x->v(0,mx), y->v(0,my), zVal));
		if(mk)	gr->mark_plot(n1,mk);
		for(i=1;i<n;i++)
		{
			if(gr->Stop)	return;
			n2 = n1;	// horizontal
			p = mglPoint(x->v(i,mx), y->v(i-1,my), zVal);
			n1 = gr->AddPnt(p);		gr->line_plot(n1,n2);
			if(i==1)	gr->arrow_plot(n1,n2,gr->Arrow1);

			n2 = n1;	// vertical
			p.y = y->v(i,my);		n1 = gr->AddPnt(p);
			if(mk)	gr->mark_plot(n1,mk);
			gr->line_plot(n1,n2);
			if(i==n-1)	gr->arrow_plot(n1,n2,gr->Arrow2);
			if(sh)	gr->NextColor(pal);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_step(HMGL gr, HCDT y,	const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData x(y->GetNx());
	x.Fill(gr->Min.x,gr->Max.x);
	mgl_step_xy(gr,&x,y,pen,0);
	gr->LoadState();
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
	if(mgl_check_dim1(gr,x,y,z,0,"Stem"))	return;

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Stem3",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();	m = z->GetNy() > m ? z->GetNy() : m;
	bool sh = mglchr(pen,'!');

	mreal z0=gr->GetOrgZ('x');
	char mk=gr->SetPenPal(pen,&pal);	gr->Reserve(2*n*m);
	long n1,n2;
	for(j=0;j<m;j++)
	{
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		mz = j<z->GetNy() ? j:0;	gr->NextColor(pal);
		for(i=0;i<n;i++)
		{
			if(gr->Stop)	return;
			n1 = gr->AddPnt(mglPoint(x->v(i,mx), y->v(i,my), z->v(i,mz)));
			if(mk)	gr->mark_plot(n1,mk);
			n2 = gr->AddPnt(mglPoint(x->v(i,mx), y->v(i,my), z0));
			gr->line_plot(n1,n2);
			if(sh)	gr->NextColor(pal);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_stem_xy(HMGL gr, HCDT x, HCDT y, const char *pen, const char *opt)
{
	long i,j,m,mx,my,n=y->GetNx(), pal;
	if(mgl_check_dim1(gr,x,y,0,0,"Stem"))	return;

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Stem",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();
	bool sh = mglchr(pen,'!');

	mreal zVal = gr->AdjustZMin(), y0=gr->GetOrgY('x'), vv;
	char mk=gr->SetPenPal(pen,&pal);	gr->Reserve(2*n*m);
	long n1,n2;
	for(j=0;j<m;j++)
	{
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		gr->NextColor(pal);
		for(i=0;i<n;i++)
		{
			if(gr->Stop)	return;
			vv = x->v(i,mx);
			n1 = gr->AddPnt(mglPoint(vv, y->v(i,my), zVal));
			if(mk)	gr->mark_plot(n1,mk);
			n2 = gr->AddPnt(mglPoint(vv, y0, zVal));
			gr->line_plot(n1,n2);
			if(sh)	gr->NextColor(pal);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_stem(HMGL gr, HCDT y,	const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData x(y->GetNx());
	x.Fill(gr->Min.x,gr->Max.x);
	mgl_stem_xy(gr,&x,y,pen,0);
	gr->LoadState();
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
	long i,j,m,mx,my,mz,n=z->GetNx(), pal,nx=x->GetNx(),ny=y->GetNx();
	if(mgl_check_dim1(gr,x,z,y,0,"Bars",true))	return;

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Bars3",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();	m = z->GetNy() > m ? z->GetNy() : m;
	bool sh = mglchr(pen,'!');

	bool wire = mglchr(pen,'#');
	bool above = mglchr(pen,'a'), fall = mglchr(pen,'f');
	if(above)	fall = false;
	mreal c1,c2,c;
	mglPoint p1,p2,p3,p4,nn;
	long n1,n2,n3,n4;
	mreal *dd=new mreal[n], x1,x2,y1,y2,z0,zz,zp,d,vv;
	memset(dd,0,n*sizeof(mreal));

	gr->SetPenPal(pen,&pal);
	if(wire)	gr->Reserve(4*n*m);	else	gr->Reserve(4*n*m);
	for(j=0;j<m;j++)
	{
		c2=c1=gr->NextColor(pal);
		if(gr->GetNumPal(pal)==2*m && !sh)	c2 = gr->NextColor(pal);
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;	mz = j<z->GetNy() ? j:0;
		zp = z0 = gr->GetOrgZ('x');
		for(i=0;i<n;i++)
		{
			if(gr->Stop)	{	delete []dd;	return;	}
			vv = x->v(i,mx);	d = i<nx-1 ? x->v(i+1,mx)-vv : vv-x->v(i-1,mx);
			x1 = vv + d/2*(1-gr->BarWidth);	x2 = x1 + gr->BarWidth*d;
			vv = y->v(i,my);	d = i<ny-1 ? y->v(i+1,my)-vv : vv-y->v(i-1,my);
			y1 = vv + d/2*(1-gr->BarWidth);	y2 = y1 + gr->BarWidth*d;
			vv = zz = z->v(i,mz);
			if(!above)
			{
				x2 = (x2-x1)/m;		x1 += j*x2;		x2 += x1;
				y2 = (y2-y1)/m;		y1 += j*y2;		y2 += y1;
			}
			else
			{	z0 = gr->GetOrgZ('x') + dd[i];	dd[i] += zz;	zz += z0;	}
			if(fall)	{	z0 = zp;	zz += z0;	zp = zz;	}

			c = vv<0 ? c1 : c2;
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
			if(sh)	c2=c1=gr->NextColor(pal);
		}
	}
	gr->EndGroup();	delete []dd;
}
//-----------------------------------------------------------------------------
void mgl_bars_xy(HMGL gr, HCDT x, HCDT y, const char *pen, const char *opt)
{
	long i,j,m,mx,my,n=y->GetNx(),pal;
	if(mgl_check_dim1(gr,x,y,0,0,"Bars",true))	return;

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Bars",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();
	bool sh = mglchr(pen,'!');

	bool wire = mglchr(pen,'#');
	bool above = mglchr(pen,'a'), fall = mglchr(pen,'f');
	if(above)	fall = false;
	mreal c1,c2,c;
	long n1,n2,n3,n4;
	mreal *dd=new mreal[n], x1,x2,yy,y0,yp,d,vv;
	mreal zm = gr->AdjustZMin();
	memset(dd,0,n*sizeof(mreal));

	gr->SetPenPal(pen,&pal);
	if(wire)	gr->Reserve(4*n*m);	else	gr->Reserve(4*n*m);
	for(j=0;j<m;j++)
	{
		c2=c1=gr->NextColor(pal);
		if(gr->GetNumPal(pal)==2*m && !sh)	c2 = gr->NextColor(pal);
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		yp = y0 = gr->GetOrgY('x');
		for(i=0;i<n;i++)
		{
			if(gr->Stop)	{	delete []dd;	return;	}
			vv = x->v(i,mx);	d = i<n-1 ? x->v(i+1,mx)-vv : vv-x->v(i-1,mx);
			x1 = vv + d/2*(1-gr->BarWidth);	x2 = x1 + gr->BarWidth*d;
			vv = yy = y->v(i,my);
			if(!above)
			{	x2 = (x2-x1)/m;		x1 += j*x2;		x2 += x1;	}
			else
			{	y0 = gr->GetOrgY('x') + dd[i];	dd[i] += yy;	yy += y0;	}
			if(fall)	{	y0 = yp;	yy += y0;	yp = yy;	}

			c = vv<0 ? c1 : c2;
			n1 = gr->AddPnt(mglPoint(x1,yy,zm),c);
			n2 = gr->AddPnt(mglPoint(x1,y0,zm),c);
			n3 = gr->AddPnt(mglPoint(x2,y0,zm),c);
			n4 = gr->AddPnt(mglPoint(x2,yy,zm),c);
			if(wire)
			{
				gr->line_plot(n1,n2);	gr->line_plot(n1,n4);
				gr->line_plot(n3,n2);	gr->line_plot(n3,n4);
			}
			else	gr->quad_plot(n1,n2,n4,n3);
			if(sh)	c2=c1=gr->NextColor(pal);
		}
	}
	gr->EndGroup();	delete []dd;
}
//-----------------------------------------------------------------------------
void mgl_bars(HMGL gr, HCDT y, const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData x(y->GetNx()+1);
	x.Fill(gr->Min.x,gr->Max.x);
	mgl_bars_xy(gr,&x,y,pen,0);
	gr->LoadState();
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
	if(mgl_check_dim1(gr,y,v,0,0,"Barh",true))	return;

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Barh",cgid++);
	m = y->GetNy() > v->GetNy() ? y->GetNy() : v->GetNy();
	bool sh = mglchr(pen,'!');

	bool wire = mglchr(pen,'#');
	bool above = mglchr(pen,'a'), fall = mglchr(pen,'f');
	if(above)	fall = false;
	mreal c1,c2,c;
	long n1,n2,n3,n4;
	mreal *dd=new mreal[n], y1,y2,xx,x0,xp,d,vv;
	mreal zm = gr->AdjustZMin();
	memset(dd,0,n*sizeof(mreal));

	gr->SetPenPal(pen,&pal);
	if(wire)	gr->Reserve(4*n*m);	else	gr->Reserve(4*n*m);
	for(j=0;j<m;j++)
	{
		c2=c1=gr->NextColor(pal);
		if(gr->GetNumPal(pal)==2*m && !sh)	c2 = gr->NextColor(pal);
		mx = j<v->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		xp = x0 = gr->GetOrgX('y');
		for(i=0;i<n;i++)
		{
			if(gr->Stop)	{	delete []dd;	return;	}
			vv = y->v(i,my);	d = i<n-1 ? y->v(i+1,my)-vv : vv-y->v(i-1,my);
			y1 = vv + d/2*(1-gr->BarWidth);	y2 = y1 + gr->BarWidth*d;
			vv = xx = v->v(i,mx);
			if(!above)
			{	y2 = (y2-y1)/m;		y1 += j*y2;		y2 += y1;	}
			else
			{	x0 = gr->GetOrgX('y') + dd[i];	dd[i] += xx;	xx += x0;	}
			if(fall)	{	x0 = xp;	xx += x0;	xp = xx;	}

			c = vv<0 ? c1 : c2;
			n1 = gr->AddPnt(mglPoint(xx,y1,zm),c);
			n2 = gr->AddPnt(mglPoint(xx,y2,zm),c);
			n3 = gr->AddPnt(mglPoint(x0,y2,zm),c);
			n4 = gr->AddPnt(mglPoint(x0,y1,zm),c);
			if(wire)
			{
				gr->line_plot(n1,n2);	gr->line_plot(n1,n4);
				gr->line_plot(n3,n2);	gr->line_plot(n3,n4);
			}
			else	gr->quad_plot(n1,n2,n4,n3);
			if(sh)	c2=c1=gr->NextColor(pal);
		}
	}
	gr->EndGroup();	delete []dd;
}
//-----------------------------------------------------------------------------
void mgl_barh(HMGL gr, HCDT v,	const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData y(v->GetNx()+1);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_barh_yx(gr,&y,v,pen,0);
	gr->LoadState();
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
	const mreal *aa = (const mreal *)a;
	const mreal *bb = (const mreal *)b;
	return int(sgn(*aa-*bb));
}
//-----------------------------------------------------------------------------
void mgl_boxplot_xy(HMGL gr, HCDT x, HCDT y, const char *pen, const char *opt)
{
	long n=y->GetNx(), m=y->GetNy(), nx=x->GetNx();
	if(nx<n || nx<2)	{	gr->SetWarn(mglWarnDim,"BoxPlot");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("BoxPlot",cgid++);
	mreal *b = new mreal[5*n], *d = new mreal[m], x1, x2, dd;
	mreal zVal = gr->AdjustZMin(), vv;
	bool sh = mglchr(pen,'!');
	register long i,j;
	for(i=0;i<n;i++)	// find quartiles by itself
	{
		if(gr->Stop)	{	delete []d;	return;	}
		register long mm,k;
		for(mm=j=0;j<m;j++)
		{
			vv = y->v(i,j);
			if(!mgl_isnan(vv))	{	d[mm]=vv;	mm++;	}
		}
		if(m==0)	{	b[i]=NAN;	break;	}
		qsort(d, mm, sizeof(mreal), mgl_cmp_flt);
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
		if(gr->Stop)	{	delete []b;	return;	}
		vv = x->v(i);
		dd = i<nx-1 ? x->v(i+1)-vv : vv-x->v(i-1);
		x1 = vv + dd/2*(1-gr->BarWidth);
		x2 = x1 + gr->BarWidth*dd;
		for(j=0;j<5;j++)	// horizontal lines
		{
			p1=mglPoint(x1,b[i+j*n],zVal);	n1=gr->AddPnt(p1);
			p2=mglPoint(x2,b[i+j*n],zVal);	n2=gr->AddPnt(p2);
			gr->line_plot(n1,n2);
		}
		//vertical lines
		p1=mglPoint(x1,b[i+n],zVal);		n1=gr->AddPnt(p1);
		p2=mglPoint(x1,b[i+3*n],zVal);		n2=gr->AddPnt(p2);
		gr->line_plot(n1,n2);
		p1=mglPoint(x2,b[i+n],zVal);		n1=gr->AddPnt(p1);
		p2=mglPoint(x2,b[i+3*n],zVal);		n2=gr->AddPnt(p2);
		gr->line_plot(n1,n2);
		p1=mglPoint((x1+x2)/2,b[i],zVal);	n1=gr->AddPnt(p1);
		p2=mglPoint((x1+x2)/2,b[i+n],zVal);	n2=gr->AddPnt(p2);
		gr->line_plot(n1,n2);
		p1=mglPoint((x1+x2)/2,b[i+3*n],zVal);	n1=gr->AddPnt(p1);
		p2=mglPoint((x1+x2)/2,b[i+4*n],zVal);	n2=gr->AddPnt(p2);
		gr->line_plot(n1,n2);
		if(sh)	gr->NextColor(pal);
	}
	delete []b;	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_boxplot(HMGL gr, HCDT y, const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData x(y->GetNx()+1);
	x.Fill(gr->Min.x,gr->Max.x);
	mgl_boxplot_xy(gr,&x,y,pen,0);
	gr->LoadState();
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
	long i,j,k,m,mx,my,m1,m2,n=ey->GetNx(),pal;
	if(mgl_check_dim1(gr,x,y,ey,ex,"Error"))	return;

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Error",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();
	m = ex->GetNy() > m ? ex->GetNy() : m;
	m = ey->GetNy() > m ? ey->GetNy() : m;
	bool sh = mglchr(pen,'!');

	bool ma = mglchr(pen,'@');
	char mk = gr->SetPenPal(pen,&pal);
	mreal zVal=gr->AdjustZMin(), vx, vy, ve, vf;
	gr->Reserve(5*n*m);
	if(ma && (mk==0 || !strchr("PXsSdD+xoOC",mk) ))	mk = 'S';
	long n1,n2,n3,n4;
	mglPoint q(NAN,NAN);
	for(j=0;j<m;j++)
	{
		if(gr->Stop)	return;
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		m1 = j<ex->GetNy() ? j:0;	m2 = j<ey->GetNy() ? j:0;
		gr->NextColor(pal);
		if(ma)
		{
			if(strchr("PXsS",mk))	for(i=0;i<n;i++)	// boundary of square
			{
				vx=x->v(i,mx);	ve=ex->v(i,m1);	vy=y->v(i,my);	vf=ey->v(i,m2);
				n1 = gr->AddPnt(mglPoint(vx-ve, vy+vf, zVal),-1,q,-1,11);
				n2 = gr->AddPnt(mglPoint(vx-ve, vy-vf, zVal),-1,q,-1,11);
				n3 = gr->AddPnt(mglPoint(vx+ve, vy+vf, zVal),-1,q,-1,11);
				n4 = gr->AddPnt(mglPoint(vx+ve, vy-vf, zVal),-1,q,-1,11);
				gr->line_plot(n1,n2);	gr->line_plot(n1,n3);
				gr->line_plot(n4,n2);	gr->line_plot(n4,n3);
				if(sh)	gr->NextColor(pal);
			}
			if(strchr("dD",mk))	for(i=0;i<n;i++)	// boundary of rhomb
			{
				vx=x->v(i,mx);	ve=ex->v(i,m1);	vy=y->v(i,my);	vf=ey->v(i,m2);
				n1 = gr->AddPnt(mglPoint(vx, vy+vf, zVal),-1,q,-1,11);
				n2 = gr->AddPnt(mglPoint(vx-ve, vy, zVal),-1,q,-1,11);
				n3 = gr->AddPnt(mglPoint(vx, vy-vf, zVal),-1,q,-1,11);
				n4 = gr->AddPnt(mglPoint(vx+ve, vy, zVal),-1,q,-1,11);
				gr->line_plot(n1,n2);	gr->line_plot(n2,n3);
				gr->line_plot(n3,n4);	gr->line_plot(n4,n1);
				if(sh)	gr->NextColor(pal);
			}
			if(strchr("oOC",mk))	for(i=0;i<n;i++)	// circle
			{
				vx=x->v(i,mx);	ve=ex->v(i,m1);	vy=y->v(i,my);	vf=ey->v(i,m2);
				for(k=0,n2=-1;k<=40;k++)
				{
					n1 = n2;
					n2 = gr->AddPnt(mglPoint(vx+ve*cos(k*M_PI/20),
							vy+vf*sin(k*M_PI/20), zVal),-1,q,-1,11);
					if(k>0)	gr->line_plot(n1,n2);
				}
				if(sh)	gr->NextColor(pal);
			}
			switch(mk)
			{
			case 'P':	case '+':	for(i=0;i<n;i++)
			{
				vx=x->v(i,mx);	ve=ex->v(i,m1);	vy=y->v(i,my);	vf=ey->v(i,m2);
				n1 = gr->AddPnt(mglPoint(vx, vy+vf, zVal),-1,q,-1,11);
				n2 = gr->AddPnt(mglPoint(vx-ve, vy, zVal),-1,q,-1,11);
				n3 = gr->AddPnt(mglPoint(vx, vy-vf, zVal),-1,q,-1,11);
				n4 = gr->AddPnt(mglPoint(vx+ve, vy, zVal),-1,q,-1,11);
				gr->line_plot(n1,n3);	gr->line_plot(n2,n4);
				if(sh)	gr->NextColor(pal);
			}	break;
			case 'X':	case 'x':	for(i=0;i<n;i++)
			{
				vx=x->v(i,mx);	ve=ex->v(i,m1);	vy=y->v(i,my);	vf=ey->v(i,m2);
				n1 = gr->AddPnt(mglPoint(vx-ve, vy+vf, zVal),-1,q,-1,11);
				n2 = gr->AddPnt(mglPoint(vx-ve, vy-vf, zVal),-1,q,-1,11);
				n3 = gr->AddPnt(mglPoint(vx+ve, vy+vf, zVal),-1,q,-1,11);
				n4 = gr->AddPnt(mglPoint(vx+ve, vy-vf, zVal),-1,q,-1,11);
				gr->line_plot(n1,n3);	gr->line_plot(n2,n4);
				if(sh)	gr->NextColor(pal);
			}	break;
			case 'S':	for(i=0;i<n;i++)
			{
				vx=x->v(i,mx);	ve=ex->v(i,m1);	vy=y->v(i,my);	vf=ey->v(i,m2);
				n1 = gr->AddPnt(mglPoint(vx-ve, vy+vf, zVal),-1,q,-1,11);
				n2 = gr->AddPnt(mglPoint(vx-ve, vy-vf, zVal),-1,q,-1,11);
				n3 = gr->AddPnt(mglPoint(vx+ve, vy+vf, zVal),-1,q,-1,11);
				n4 = gr->AddPnt(mglPoint(vx+ve, vy-vf, zVal),-1,q,-1,11);
				gr->quad_plot(n1,n2,n3,n4);
				if(sh)	gr->NextColor(pal);
			}	break;
			case 'D':	for(i=0;i<n;i++)
			{
				vx=x->v(i,mx);	ve=ex->v(i,m1);	vy=y->v(i,my);	vf=ey->v(i,m2);
				n1 = gr->AddPnt(mglPoint(vx, vy+vf, zVal),-1,q,-1,11);
				n2 = gr->AddPnt(mglPoint(vx-ve, vy, zVal),-1,q,-1,11);
				n3 = gr->AddPnt(mglPoint(vx, vy-vf, zVal),-1,q,-1,11);
				n4 = gr->AddPnt(mglPoint(vx+ve, vy, zVal),-1,q,-1,11);
				gr->quad_plot(n1,n2,n3,n4);
				if(sh)	gr->NextColor(pal);
			}	break;
			case 'O':	for(i=0;i<n;i++)
			{
				vx=x->v(i,mx);	ve=ex->v(i,m1);	vy=y->v(i,my);	vf=ey->v(i,m2);
				n3 = gr->AddPnt(mglPoint(vx,vy,zVal));
				for(k=0,n2=-1;k<=40;k++)
				{
					n1 = n2;
					n2 = gr->AddPnt(mglPoint(vx+ve*cos(k*M_PI/20),
							vy+vf*sin(k*M_PI/20), zVal),-1,q,-1,11);
					if(k>0)	gr->trig_plot(n1,n2,n3);
				}
				if(sh)	gr->NextColor(pal);
			}	break;
			case 'C':	for(i=0;i<n;i++)
			{
				vx=x->v(i,mx);	ve=ex->v(i,m1);	vy=y->v(i,my);	vf=ey->v(i,m2);
				gr->mark_plot(gr->AddPnt(mglPoint(vx,vy,zVal),-1,q,-1,3), '.');
				if(sh)	gr->NextColor(pal);
			}
			}
		}
		else	for(i=0;i<n;i++)
		{
			vx=x->v(i,mx);	ve=ex->v(i,m1);	vy=y->v(i,my);	vf=ey->v(i,m2);
			if(mk)	gr->mark_plot(gr->AddPnt(mglPoint(vx,vy,zVal)), mk);

			n1 = gr->AddPnt(mglPoint(vx, vy+vf, zVal),-1,q,-1,11);
			n2 = gr->AddPnt(mglPoint(vx, vy-vf, zVal),-1,q,-1,11);
			gr->line_plot(n1,n2);	gr->arrow_plot(n1,n2,'I');	gr->arrow_plot(n2,n1,'I');

			n1 = gr->AddPnt(mglPoint(vx+ve, vy, zVal),-1,q,-1,11);
			n2 = gr->AddPnt(mglPoint(vx-ve, vy, zVal),-1,q,-1,11);
			gr->line_plot(n1,n2);	gr->arrow_plot(n1,n2,'I');	gr->arrow_plot(n2,n1,'I');
			if(sh)	gr->NextColor(pal);
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
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void mgl_error(HMGL gr, HCDT y, HCDT ey, const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData x(y->GetNx());
	x.Fill(gr->Min.x,gr->Max.x);
	mgl_error_xy(gr,&x,y,ey,pen,0);
	gr->LoadState();
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
void face_plot(mglBase *gr, mglPoint o, mglPoint d1, mglPoint d2, mreal c, bool wire)
{
	const int num=10;
	mglPoint p,nn=d1^d2;
	d1 = d1/num;	d2 = d2/num;
	register long i,j,i0,n=num+1;
	long *id=new long[n*n];
	gr->Reserve(n*n);
	for(j=0;j<n;j++)	for(i=0;i<n;i++)
	{	p = o+d1*i+d2*j;	id[i+n*j] = gr->AddPnt(p,c,nn);
		if(id[i+n*j]<0)
		{	printf("q");	gr->AddPnt(p,c,nn);	}
	}
	for(i=0;i<num;i++)	for(j=0;j<num;j++)
	{
		if(gr->Stop)	{	delete []id;	return;	}
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
			if(gr->Stop)	{	delete []id;	return;	}
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
	bool wire = mglchr(cols,'#');	// draw edges
	register long n=a->GetNx(),i,j=0;
	if(cols)	for(i=0;i<long(strlen(cols));i++)
		if(strchr(MGL_COLORS,cols[i]) || cols[i]==' ')	j++;
	if(j==0)	cols = MGL_DEF_PAL;
	mreal *c = new mreal[strlen(cols)+1],cc;
	long nc=0;			// number of colors
	for(i=0;i<long(strlen(cols));i++)
		if(strchr(MGL_COLORS,cols[i]) || cols[i]==' ')
		{	c[nc]=gr->AddTexture(cols[i]);	nc++;	}

	mreal dy = (gr->Max.y-gr->Min.y)/a->GetNy(), dx, ss, cs, x1, y1, dz=gr->Max.z-gr->Min.z, vv;
	mglPoint d1,d2,o;

	for(j=0;j<a->GetNy();j++)
	{
		y1 = gr->Min.y + dy*j;
		for(i=0,ss=0;i<n;i++)	ss += a->v(i,j);
		if(ss==0)	continue;
		for(cs=0,i=0;i<n;i++)
		{
			if(gr->Stop)	{	delete []c;	return;	}
			vv = a->v(i,j);	dx = vv/ss;	cc = c[i%nc];
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
			cs += vv;
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
	if(mgl_check_dim1(gr,x,y,z,r,"Mark"))	return;

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Mark",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();
	m = z->GetNy() > m ? z->GetNy() : m;
	char mk=gr->SetPenPal(pen,&pal);	gr->Reserve(n*m);
	if(mk==0)	return;
	bool sh = mglchr(pen,'!');

	for(j=0;j<m;j++)
	{
		gr->NextColor(pal);
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		mz = j<z->GetNy() ? j:0;	mr = j<r->GetNy() ? j:0;
		for(int i=0;i<n;i++)
		{
			if(gr->Stop)	return;
			gr->mark_plot(gr->AddPnt(mglPoint(x->v(i,mx),y->v(i,my),z->v(i,mz))), mk, r->v(i,mr));
			if(sh)	gr->NextColor(pal);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_mark_xy(HMGL gr, HCDT x, HCDT y, HCDT r, const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData z(y->GetNx());
	mreal zm = gr->AdjustZMin();	z.Fill(zm,zm);
	mgl_mark_xyz(gr,x,y,&z,r,pen,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void mgl_mark_y(HMGL gr, HCDT y, HCDT r, const char *pen, const char *opt)
{
	register long n=y->GetNx();
	if(n<2)	{	gr->SetWarn(mglWarnLow,"Mark");	return;	}
	gr->SaveState(opt);
	mglData x(n), z(n);
	x.Fill(gr->Min.x,gr->Max.x);
	mreal zm = gr->AdjustZMin();	z.Fill(zm,zm);
	mgl_mark_xyz(gr,&x,y,&z,r,pen,0);
	gr->LoadState();
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
	if(mgl_check_dim1(gr,x,y,z,r,"Tube"))	return;

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Tube",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();
	m = z->GetNy() > m ? z->GetNy() : m;
	m = r->GetNy() > m ? r->GetNy() : m;
	bool sh = mglchr(pen,'!');
	bool wire = mglchr(pen,'#');

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
			t = !l;		t.Normalize();	u = t^l;	u.Normalize();
			q = mglPoint(x->v(i,mx),y->v(i,my),z->v(i,mz));
			mreal si,co,ff, rr=r->v(i,mr), dr=r->dvx(i,mr);
			for(k=0;k<num;k++)
			{
				if(gr->Stop)	{	delete []nn;	return;	}
				ff = k*2*M_PI/(num-1);	co = cos(ff);	si = sin(ff);
				p = q + t*(rr*co) + u*(rr*si);
				d = (t*si - u*co)^(l + t*(dr*co) + u*(dr*si));
				nn[k+num]=nn[k];	nn[k] = gr->AddPnt(p,gr->CDef,wire?mglPoint(NAN,NAN):d,-1,3);
				if(i*k>0 && !wire)	gr->quad_plot(nn[k],nn[k-1],nn[k+num],nn[k+num-1]);
				if(i*k>0 && wire && k%4==0)	gr->line_plot(nn[k],nn[k+num]);
				if(k>0 && wire)		gr->line_plot(nn[k],nn[k-1]);
			}
			if(sh)	gr->NextColor(pal);
		}
	}
	delete []nn;	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_tube_xyr(HMGL gr, HCDT x, HCDT y, HCDT r, const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData z(y->GetNx());
	mreal zm = gr->AdjustZMin();	z.Fill(zm,zm);
	mgl_tube_xyzr(gr,x,y,&z,r,pen,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void mgl_tube_r(HMGL gr, HCDT y, HCDT r, const char *pen, const char *opt)
{
	register long n=y->GetNx();
	if(n<2)	{	gr->SetWarn(mglWarnLow,"Tube");	return;	}
	gr->SaveState(opt);
	mglData x(n), z(n);
	x.Fill(gr->Min.x,gr->Max.x);
	mreal zm = gr->AdjustZMin();	z.Fill(zm,zm);
	mgl_tube_xyzr(gr,&x,y,&z,r,pen,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void mgl_tube(HMGL gr, HCDT y, mreal rr, const char *pen, const char *opt)
{
	register long n=y->GetNx();
	if(n<2)	{	gr->SetWarn(mglWarnLow,"Tube");	return;	}
	gr->SaveState(opt);
	mglData x(n), r(n), z(n);
	x.Fill(gr->Min.x,gr->Max.x);
	r.Fill(rr,rr);
	mreal zm = gr->AdjustZMin();	z.Fill(zm,zm);
	mgl_tube_xyzr(gr,&x,y,&z,&r,pen,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void mgl_tube_xy(HMGL gr, HCDT x, HCDT y, mreal rr, const char *pen, const char *opt)
{
	register long n=y->GetNx();
	if(n<2)	{	gr->SetWarn(mglWarnLow,"Tube");	return;	}
	gr->SaveState(opt);
	mglData r(n), z(n);
	r.Fill(rr,rr);
	mreal zm = gr->AdjustZMin();	z.Fill(zm,zm);
	mgl_tube_xyzr(gr,x,y,&z,&r,pen,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void mgl_tube_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, mreal rr, const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData r(y->GetNx());
	r.Fill(rr,rr);
	mgl_tube_xyzr(gr,x,y,z,&r,pen,0);
	gr->LoadState();
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
void mgl_tube_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, mreal *r, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_tube_xyz(_GR_,_DA_(x),_DA_(y),_DA_(z),*r,s,o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_tube_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, mreal *r, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_tube_xy(_GR_,_DA_(x),_DA_(y),*r,s,o);	delete []s;	delete []o;	}
//-----------------------------------------------------------------------------
void mgl_tube_(uintptr_t *gr, uintptr_t *y, mreal *r, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_tube(_GR_,_DA_(y),*r,s,o);
	delete []s;	delete []o;	}
//-----------------------------------------------------------------------------
//
//	Tape series
//
//-----------------------------------------------------------------------------
void mgl_tape_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, const char *pen, const char *opt)
{
	long j,m,mx,my,mz,n=y->GetNx(),pal;
	if(mgl_check_dim1(gr,x,y,z,0,"Tape"))	return;

	static int cgid=1;	gr->StartGroup("Tape",cgid++);
	mreal ll, rr = gr->SaveState(opt);
	if(rr==0 || mgl_isnan(rr))	rr = mgl_norm(gr->Max-gr->Min)*gr->BarWidth/25;
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();	m = z->GetNy() > m ? z->GetNy() : m;
	gr->SetPenPal(pen,&pal);	gr->Reserve(4*n*m);
	mglPoint p1,p2,q1,q2,l,nn,qn=mglPoint(NAN,NAN);
	long n1=-1,n2=-1,n3=-1,n4=-1, m1=-1,m2=-1,m3=-1,m4=-1;
	bool sh = mglchr(pen,'!'), xo = mglchr(pen,'x'), zo = mglchr(pen,'z'), wire = mglchr(pen,'#');
	if(!xo && !zo)	xo = zo = true;
	mreal c1,c2;

	for(j=0;j<m;j++)
	{
		c2=c1=gr->NextColor(pal);
		if(gr->GetNumPal(pal)==2*m && !sh)	c2 = gr->NextColor(pal);
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;	mz = j<z->GetNy() ? j:0;
		// initial values for normales
		p2 = mglPoint(x->v(0,mx), y->v(0,my), z->v(0,mz));
		l = mglPoint(x->v(1,mx), y->v(1,my), z->v(1,mz)) - p2;	l /= mgl_norm(l);
		q1 = mglPoint(-l.y,l.x,0);	ll = mgl_norm(q1);
		if(ll)	q1 /= ll;	else	q1 = mglPoint(0,1,0);
		q2 = (q1^l);
		if(p2>gr->Min && p2<gr->Max)
		{
			if(xo)	{	n1 = gr->AddPnt(p2,c1,q2,-1,3);	n2 = gr->AddPnt(p2+rr*q1,c1,q2,-1,3);	}
			if(zo)	{	n3 = gr->AddPnt(p2,c2,q1,-1,3);	n4 = gr->AddPnt(p2+rr*q2,c2,q1,-1,3);	}
		}
		else		{	n1=n2=n3=n4=-1;	}
		register long i;
		for(i=1;i<n;i++)
		{
			if(gr->Stop)	return;
			p1 = p2;	p2 = mglPoint(x->v(i,mx), y->v(i,my), z->v(i,mz));
			l = p2-p1;		l /= mgl_norm(l);
			q1 -= l*(l*q1);	q1/= mgl_norm(q1);	q2 = (q1^l);
			m1 = n1;	m2 = n2;	m3 = n3;	m4 = n4;
			if(p2>gr->Min && p2<gr->Max)
			{
				if(xo)
				{
					n1 = gr->AddPnt(p2,c1,wire?qn:q2,-1,3);
					n2 = gr->AddPnt(p2+rr*q1,c1,wire?qn:q2,-1,3);
					if(wire)	gr->line_plot(m2,n2);
					else		gr->quad_plot(n1,n2,m1,m2);
				}
				if(zo)
				{
					n3 = gr->AddPnt(p2,c2,wire?qn:q1,-1,3);
					n4 = gr->AddPnt(p2+rr*q2,c2,wire?qn:q1,-1,3);
					if(wire)	gr->line_plot(m4,n4);
					else		gr->quad_plot(n3,n4,m3,m4);
				}
			}
			if(sh)	c2=c1=gr->NextColor(pal);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_tape_xy(HMGL gr, HCDT x, HCDT y, const char *pen, const char *opt)
{
	gr->SaveState(opt);
	mglData z(y->GetNx());
	mreal zm = gr->AdjustZMin();	z.Fill(zm,zm);
	mgl_tape_xyz(gr,x,y,&z,pen,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void mgl_tape(HMGL gr, HCDT y, const char *pen, const char *opt)
{
	register long n=y->GetNx();
	if(n<2)	{	gr->SetWarn(mglWarnLow,"Plot");	return;	}
	gr->SaveState(opt);
	mglData x(n), z(n);
	x.Fill(gr->Min.x,gr->Max.x);
	mreal zm = gr->AdjustZMin();	z.Fill(zm,zm);
	mgl_tape_xyz(gr,&x,y,&z,pen,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void mgl_tape_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];		memcpy(o,opt,lo);	o[lo]=0;
	mgl_tape_xyz(_GR_, _DA_(x),_DA_(y),_DA_(z),s,o);	delete []s;	delete []o;	}
//-----------------------------------------------------------------------------
void mgl_tape_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];		memcpy(o,opt,lo);	o[lo]=0;
	mgl_tape_xy(_GR_, _DA_(x),_DA_(y),s,o);	delete []s;	delete []o;	}
//-----------------------------------------------------------------------------
void mgl_tape_(uintptr_t *gr, uintptr_t *y,	const char *pen, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	char *o=new char[lo+1];		memcpy(o,opt,lo);	o[lo]=0;
	mgl_tape(_GR_, _DA_(y),s,o);	delete []s;	delete []o;	}
//-----------------------------------------------------------------------------
