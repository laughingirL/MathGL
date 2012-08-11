/***************************************************************************
 * data.cpp is part of Math Graphic Library
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
#include "mgl2/data.h"
#include "mgl2/eval.h"

int mglNumThr=0;
void mglFillP(long x,long y, const mreal *a,long nx,long ny,mreal _p[4][4]);
void mglFillP(long x, const mreal *a,long nx,mreal _p[4]);
void mglFillP5(long x,long y, const mreal *a,long nx,long ny,mreal _p[6][6]);
void mglFillP5(long x, const mreal *a,long nx,mreal _p[6]);
//-----------------------------------------------------------------------------
#if MGL_HAVE_PTHREAD
#ifdef WIN32
#include <windows.h>
#include <process.h>
#elif defined(__APPLE__) || defined (__FreeBSD__)
#include <sys/sysctl.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <sys/sysinfo.h>
#endif
void mgl_set_num_thr(int n)
{
#ifdef WIN32
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	mglNumThr = n>0 ? n : systemInfo.dwNumberOfProcessors;
#elif defined (__APPLE__) || defined(__FreeBSD__)
	int numProcessors = 1;
	size_t size = sizeof(numProcessors);
	sysctlbyname("hw.ncpu", &numProcessors, &size, NULL, 0);
	mglNumThr = n>0 ? n : numProcessors;
#else
	mglNumThr = n>0 ? n : get_nprocs_conf();
#endif
}
#else
void mgl_set_num_thr(int)	{	mglNumThr = 1;	}
#endif
//-----------------------------------------------------------------------------
void mglStartThread(void *(*func)(void *), void (*post)(mglThreadD *,mreal *), long n, mreal *a,
	const mreal *b, const mreal *c, const long *p, void *v, const mreal *d, const mreal *e, const char *s)
{
	if(!func)	return;
#if MGL_HAVE_PTHREAD
	if(mglNumThr<1)	mgl_set_num_thr(0);
	if(mglNumThr>1)
	{
		pthread_t *tmp=new pthread_t[mglNumThr];
		mglThreadD *par=new mglThreadD[mglNumThr];
		register long i;
		for(i=0;i<mglNumThr;i++)	// put parameters into the structure
		{	par[i].n=n;	par[i].a=a;	par[i].b=b;	par[i].c=c;	par[i].d=d;
			par[i].p=p;	par[i].v=v;	par[i].s=s;	par[i].e=e;	par[i].id=i;	}
		for(i=0;i<mglNumThr;i++)	pthread_create(tmp+i, 0, func, par+i);
		for(i=0;i<mglNumThr;i++)	pthread_join(tmp[i], 0);
		if(post)	post(par,a);
		delete []tmp;	delete []par;
	}
	else
#endif
	{
		mglNumThr = 1;
		mglThreadD par;
		par.n=n;	par.a=a;	par.b=b;	par.c=c;	par.d=d;
		par.p=p;	par.v=v;	par.s=s;	par.e=e;	par.id=0;
		func(&par);
		if(post)	post(&par,a);
	}
}
//-----------------------------------------------------------------------------
double mgl_ipow(double x,int n)
{
	double t;
	if(n==2)	return x*x;
	if(n==1)	return x;
	if(n<0)		return 1./mgl_ipow(x,-n);
	if(n==0)	return 1;
	t = mgl_ipow(x,n/2);	t = t*t;
	if(n%2==1)	t *= x;
	return t;
}
//-----------------------------------------------------------------------------
void *mgl_smth_x(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,d3,d5, nx=t->p[0];
	mreal *b=t->a, delta=t->c[0], y5,y3,x2y;
	const mreal *a=t->b;
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		j = i%nx;
		d3 = d5 = 0;
		if(j==0)	{	d3 = 1;		d5 = 2;	}
		if(j==1)	{	d5 = 1;	}
		if(j==nx-1)	{	d3 = -1;	d5 = -2;}
		if(j==nx-2)	{	d5 = -1;}
		y3 = (a[i+d3-1] + a[i+d3] + a[i+d3+1]);
		y5 = (a[i+d5-2] + a[i+d5-1] + a[i+d5] + a[i+d5+1] + a[i+d5+2]);
		x2y= (a[i+d5+1] + 4*a[i+d5+2] + 4*a[i+d5-2] + a[i+d5-1]);
		j = t->p[2];
		if(d3)	b[i] = a[i];
		else if(j==SMOOTH_LINE_3 || d5)	b[i] = y3/3.;
		else if(j==SMOOTH_LINE_5)		b[i] = y5/5.;
		else if(j==SMOOTH_QUAD_5)		b[i] = (17*y5-5*x2y)/35.;
		if(delta>0)		b[i] = mgl_max(a[i]-delta,mgl_min(a[i]+delta,b[i]));
	}
	return 0;
}
void *mgl_smth_y(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,d3,d5, nx=t->p[0],ny=t->p[1];
	mreal *b=t->a, delta=t->c[0], y5,y3,x2y;
	const mreal *a=t->b;
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		j = (i/nx)%ny;
		d3 = d5 = 0;
		if(j==0)	{	d3 = 1;		d5 = 2;	}
		if(j==1)	{	d5 = 1;	}
		if(j==ny-1)	{	d3 = -1;	d5 = -2;}
		if(j==ny-2)	{	d5 = -1;}
		y3 = (a[i+nx*(d3-1)] + a[i+nx*d3] + a[i+nx*(d3+1)]);
		y5 = (a[i+nx*(d5-2)] + a[i+nx*(d5-1)] + a[i+nx*d5] + a[i+nx*(d5+1)] + a[i+nx*(d5+2)]);
		x2y= (a[i+nx*(d5+1)] + 4*a[i+nx*(d5+2)] + 4*a[i+nx*(d5-2)] + a[i+nx*(d5-1)]);
		j = t->p[2];
		if(d3)	b[i] = a[i];
		else if(j==SMOOTH_LINE_3 || d5)	b[i] = y3/3.;
		else if(j==SMOOTH_LINE_5)		b[i] = y5/5.;
		else if(j==SMOOTH_QUAD_5)		b[i] = (17*y5-5*x2y)/35.;
		if(delta>0)		b[i] = mgl_max(a[i]-delta,mgl_min(a[i]+delta,b[i]));
	}
	return 0;
}
void *mgl_smth_z(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,d3,d5, nn=t->p[0]*t->p[1], nz=t->n/nn;
	mreal *b=t->a, delta=t->c[0], y5,y3,x2y;
	const mreal *a=t->b;
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		j = i/nn;
		d3 = d5 = 0;
		if(j==0)	{	d3 = 1;		d5 = 2;	}
		if(j==1)	{	d5 = 1;	}
		if(j==nz-1)	{	d3 = -1;	d5 = -2;}
		if(j==nz-2)	{	d5 = -1;}
		y3 = (a[i+nn*(d3-1)] + a[i+nn*d3] + a[i+nn*(d3+1)]);
		y5 = (a[i+nn*(d5-2)] + a[i+nn*(d5-1)] + a[i+nn*d5] + a[i+nn*(d5+1)] + a[i+nn*(d5+2)]);
		x2y= (a[i+nn*(d5+1)] + 4*a[i+nn*(d5+2)] + 4*a[i+nn*(d5-2)] + a[i+nn*(d5-1)]);
		j = t->p[2];
		if(d3)	b[i] = a[i];
		else if(j==SMOOTH_LINE_3 || d5)	b[i] = y3/3.;
		else if(j==SMOOTH_LINE_5)		b[i] = y5/5.;
		else if(j==SMOOTH_QUAD_5)		b[i] = (17*y5-5*x2y)/35.;
		if(delta>0)		b[i] = mgl_max(a[i]-delta,mgl_min(a[i]+delta,b[i]));
	}
	return 0;
}
void mgl_data_smooth(HMDT d, const char *dirs, mreal delta)
{
	long Type = SMOOTH_QUAD_5;
	if(!dirs || *dirs==0)	dirs = "xyz";
	if(strchr(dirs,'0') || strchr(dirs,'1'))	return;
	if(strchr(dirs,'3'))	Type = SMOOTH_LINE_3;
	if(strchr(dirs,'5'))	Type = SMOOTH_LINE_5;
	long nx=d->nx,ny=d->ny,nz=d->nz;
//	if(Type == SMOOTH_NONE)	return;
	long p[3]={nx,ny,Type};
	mreal *b = new mreal[nx*ny*nz],dd=delta;
	// ����������� �� x
	memset(b,0,nx*ny*nz*sizeof(mreal));
	if(nx>4 && strchr(dirs,'x'))
	{
		mglStartThread(mgl_smth_x,0,nx*ny*nz,b,d->a,&dd,p);
		memcpy(d->a,b,nx*ny*nz*sizeof(mreal));
		memset(b,0,nx*ny*nz*sizeof(mreal));
	}
	if(ny>4 && strchr(dirs,'y'))
	{
		mglStartThread(mgl_smth_y,0,nx*ny*nz,b,d->a,&dd,p);
		memcpy(d->a,b,nx*ny*nz*sizeof(mreal));
		memset(b,0,nx*ny*nz*sizeof(mreal));
	}
	if(nz>4 && strchr(dirs,'z'))
	{
		mglStartThread(mgl_smth_z,0,nx*ny*nz,b,d->a,&dd,p);
		memcpy(d->a,b,nx*ny*nz*sizeof(mreal));
	}
	delete []b;
}
void mgl_data_smooth_(uintptr_t *d, const char *dir, mreal *delta,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_smooth(_DT_,s,*delta);		delete []s;	}
//-----------------------------------------------------------------------------
void *mgl_csum_z(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j, nz=t->p[2], nn=t->n;
	mreal *b=t->a;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		b[i] = a[i];
		for(j=1;j<nz;j++)	b[i+j*nn] = b[i+j*nn-nn] + a[i+j*nn];
	}
	return 0;
}
void *mgl_csum_y(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k, nx=t->p[0], ny=t->p[1], nn=t->n;
	mreal *b=t->a;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = (i%nx)+nx*ny*(i/nx);		b[k] = a[k];
		for(j=1;j<ny;j++)	b[k+j*nx] = b[k+j*nx-nx] + a[k+nx*j];
	}
	return 0;
}
void *mgl_csum_x(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k, nx=t->p[0], nn=t->n;
	mreal *b=t->a;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = i*nx;			b[k] = a[k];
		for(j=1;j<nx;j++)	b[j+k] = b[j+k-1] + a[j+k];
	}
	return 0;
}
void mgl_data_cumsum(HMDT d, const char *dir)
{
	if(!dir || *dir==0)	return;
	long nx=d->nx,ny=d->ny,nz=d->nz,nn=nx*ny*nz;
	long p[3]={nx,ny,nz};
	mreal *b = new mreal[nn];
	memcpy(b,d->a,nn*sizeof(mreal));
	if(strchr(dir,'z') && nz>1)
	{
		mglStartThread(mgl_csum_z,0,nx*ny,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(mreal));
	}
	if(strchr(dir,'y') && ny>1)
	{
		mglStartThread(mgl_csum_y,0,nx*nz,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(mreal));
	}
	if(strchr(dir,'x') && nx>1)
	{
		mglStartThread(mgl_csum_x,0,nz*ny,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(mreal));
	}
	delete []b;
}
void mgl_data_cumsum_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_cumsum(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void *mgl_int_z(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j, nz=t->p[2], nn=t->n;
	mreal *b=t->a, dd=0.5/nz;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		b[i] = 0;
		for(j=1;j<nz;j++)	b[i+j*nn] = b[i+j*nn-nn] + (a[i+nn*j]+a[i+j*nn-nn])*dd;
	}
	return 0;
}
void *mgl_int_y(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k, nx=t->p[0], ny=t->p[1], nn=t->n;
	mreal *b=t->a, dd=0.5/ny;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = (i%nx)+nx*ny*(i/nx);	b[k] = 0;
		for(j=1;j<ny;j++)	b[k+j*nx] = b[k+j*nx-nx] + (a[k+j*nx]+a[k+j*nx-nx])*dd;
	}
	return 0;
}
void *mgl_int_x(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k, nx=t->p[0], nn=t->n;
	mreal *b=t->a, dd=0.5/nx;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = i*nx;			b[k] = 0;
		for(j=1;j<nx;j++)	b[j+k] = b[j+k-1] + (a[j+k]+a[j+k-1])*dd;
	}
	return 0;
}
void mgl_data_integral(HMDT d, const char *dir)
{
	if(!dir || *dir==0)	return;
	long nx=d->nx,ny=d->ny,nz=d->nz,nn=nx*ny*nz;
	long p[3]={nx,ny,nz};
	mreal *b = new mreal[nn];
	memcpy(b,d->a,nn*sizeof(mreal));
	if(strchr(dir,'z') && nz>1)
	{
		mglStartThread(mgl_int_z,0,nx*ny,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(mreal));
	}
	if(strchr(dir,'y') && ny>1)
	{
		mglStartThread(mgl_int_y,0,nx*nz,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(mreal));
	}
	if(strchr(dir,'x') && nx>1)
	{
		mglStartThread(mgl_int_x,0,nz*ny,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(mreal));
	}
	delete []b;
}
void mgl_data_integral_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_integral(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void *mgl_dif_z(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j, nz=t->p[2], nn=t->n;
	mreal *b=t->a, dd=0.5*nz;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		b[i] = -(3*a[i]-4*a[i+nn]+a[i+2*nn])*dd;
		b[i+(nz-1)*nn] = (3*a[i+(nz-1)*nn]-4*a[i+(nz-2)*nn]+a[i+(nz-3)*nn])*dd;
		for(j=1;j<nz-1;j++)		b[i+j*nn] = (a[i+j*nn+nn]-a[i+j*nn-nn])*dd;
	}
	return 0;
}
void *mgl_dif_y(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k, nx=t->p[0], ny=t->p[1], nn=t->n;
	mreal *b=t->a, dd=0.5*ny;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = (i%nx)+nx*ny*(i/nx);
		b[k] = -(3*a[k]-4*a[k+nx]+a[k+2*nx])*dd;
		b[k+(ny-1)*nx] = (3*a[k+(ny-1)*nx]-4*a[k+(ny-2)*nx]+a[k+(ny-3)*nx])*dd;
		for(j=1;j<ny-1;j++)	b[k+j*nx] = (a[k+j*nx+nx]-a[k+j*nx-nx])*dd;
	}
	return 0;
}
void *mgl_dif_x(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k, nx=t->p[0], nn=t->n;
	mreal *b=t->a, dd=0.5*nx;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = i*nx;
		b[k] = -(3*a[k]-4*a[k+1]+a[k+2])*dd;
		b[k+nx-1] = (3*a[k+nx-1]-4*a[k+nx-2]+a[k+nx-3])*dd;
		for(j=1;j<nx-1;j++)	b[j+k] = (a[j+k+1]-a[j+k-1])*dd;
	}
	return 0;
}
void mgl_data_diff(HMDT d, const char *dir)
{
	if(!dir || *dir==0)	return;
	long nx=d->nx,ny=d->ny,nz=d->nz,nn=nx*ny*nz;
	long p[3]={nx,ny,nz};
	mreal *b = new mreal[nn];
	if(strchr(dir,'z') && nz>1)
	{
		mglStartThread(mgl_dif_z,0,nx*ny,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(mreal));
	}
	if(strchr(dir,'y') && ny>1)
	{
		mglStartThread(mgl_dif_y,0,nx*nz,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(mreal));
	}
	if(strchr(dir,'x') && nx>1)
	{
		mglStartThread(mgl_dif_x,0,nz*ny,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(mreal));
	}
	delete []b;
}
void mgl_data_diff_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_diff(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void *mgl_dif2_z(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j, nz=t->p[2], nn=t->n;
	mreal *b=t->a, dd=0.5*nz*nz;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		b[i] = b[i+(nz-1)*nn] = 0;
		for(j=1;j<nz-1;j++)		b[i+j*nn] = (a[i+j*nn+nn]+a[i+j*nn-nn]-2*a[i+j*nn])*dd;
	}
	return 0;
}
void *mgl_dif2_y(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k, nx=t->p[0], ny=t->p[1], nn=t->n;
	mreal *b=t->a, dd=0.5*ny*ny;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = (i%nx)+nx*ny*(i/nx);	b[k] = b[k+(ny-1)*nx] = 0;
		for(j=1;j<ny-1;j++)	b[k+j*nx] = (a[k+j*nx+nx]+a[k+j*nx-nx]-2*a[k+j*nx])*dd;
	}
	return 0;
}
void *mgl_dif2_x(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k, nx=t->p[0], nn=t->n;
	mreal *b=t->a, dd=0.5*nx*nx;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = i*nx;			b[k] = b[k+nx-1] = 0;
		for(j=1;j<nx-1;j++)	b[j+k] = (a[j+k+1]+a[j+k-1]-2*a[j+k])*dd;
	}
	return 0;
}
void mgl_data_diff2(HMDT d, const char *dir)
{
	if(!dir || *dir==0)	return;
	long nx=d->nx,ny=d->ny,nz=d->nz,nn=nx*ny*nz;
	long p[3]={nx,ny,nz};
	mreal *b = new mreal[nn];
	if(strchr(dir,'z') && nz>1)
	{
		mglStartThread(mgl_dif2_z,0,nx*ny,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(mreal));
	}
	if(strchr(dir,'y') && ny>1)
	{
		mglStartThread(mgl_dif2_y,0,nx*nz,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(mreal));
	}
	if(strchr(dir,'x') && nx>1)
	{
		mglStartThread(mgl_dif2_x,0,nz*ny,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(mreal));
	}
	delete []b;
}
void mgl_data_diff2_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_diff2(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_data_swap(HMDT d, const char *dir)
{
	if(!dir || *dir==0)	return;
	if(strchr(dir,'z') && d->nz>1)	mgl_data_roll(d,'z',d->nz/2);
	if(strchr(dir,'y') && d->ny>1)	mgl_data_roll(d,'y',d->ny/2);
	if(strchr(dir,'x') && d->nx>1)	mgl_data_roll(d,'x',d->nx/2);
}
void mgl_data_swap_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_swap(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_data_roll(HMDT dd, char dir, long num)
{
	long nx=dd->nx,ny=dd->ny,nz=dd->nz;
	register long i,d;
	mreal *b,*a=dd->a;
	if(dir=='z' && nz>1)
	{
		d = num>0 ? num%nz : (num+nz*(1+num/nz))%nz;
		if(d==0)	return;		// nothing to do
		b = new mreal[nx*ny*nz];
		memcpy(b,a+nx*ny*d,nx*ny*(nz-d)*sizeof(mreal));
		memcpy(b+nx*ny*(nz-d),a,nx*ny*d*sizeof(mreal));
		memcpy(a,b,nx*ny*nz*sizeof(mreal));	delete []b;
	}
	if(dir=='y' && ny>1)
	{
		d = num>0 ? num%ny : (num+ny*(1+num/ny))%ny;
		if(d==0)	return;		// nothing to do
		b = new mreal[nx*ny*nz];
		memcpy(b,a+nx*d,(nx*ny*nz-nx*d)*sizeof(mreal));
		for(i=0;i<nz;i++)
			memcpy(b+nx*(ny-d)+nx*ny*i,a+nx*ny*i,nx*d*sizeof(mreal));
		memcpy(a,b,nx*ny*nz*sizeof(mreal));	delete []b;
	}
	if(dir=='x' && nx>1)
	{
		d = num>0 ? num%nx : (num+nx*(1+num/nx))%nx;
		if(d==0)	return;		// nothing to do
		b = new mreal[nx*ny*nz];
		memcpy(b,a+d,(nx*ny*nz-d)*sizeof(mreal));
		for(i=0;i<nz*ny;i++)
			memcpy(b+nx-d+nx*i,a+nx*i,d*sizeof(mreal));
		memcpy(a,b,nx*ny*nz*sizeof(mreal));	delete []b;
	}
}
void mgl_data_roll_(uintptr_t *d, const char *dir, int *num, int)
{	mgl_data_roll(_DT_,*dir,*num);	}
//-----------------------------------------------------------------------------
void mgl_data_mirror(HMDT d, const char *dir)
{
	if(!dir || *dir==0)	return;
	long nx=d->nx,ny=d->ny,nz=d->nz;
	register long i,j,k,i0,j0;
	mreal b, *a=d->a;
	if(strchr(dir,'z') && nz>1)
	{
		for(j=0;j<nz/2;j++)	for(i=0;i<nx*ny;i++)
		{
			i0 = i+j*nx*ny;	j0 = i+(nz-1-j)*nx*ny;
			b = a[i0];	a[i0] = a[j0];	a[j0] = b;
		}
	}
	if(strchr(dir,'y') && ny>1)
	{
		for(k=0;k<nz;k++)	for(i=0;i<nx;i++)
		{
			j0 = i+nx*ny*k;
			for(j=0;j<ny/2;j++)
			{
				i0 = j0+(ny-1-j)*nx;	b = a[j0+j*nx];
				a[j0+j*nx] = a[i0];	a[i0] = b;
			}
		}
	}
	if(strchr(dir,'x') && nx>1)
	{
		for(j=0;j<ny*nz;j++)
		{
			j0 = j*nx;
			for(i=0;i<nx/2;i++)
			{	i0 = nx-1-i+j0;	b = a[i+j0];	a[i+j0] = a[i0];	a[i0] = b;	}
		}
	}
}
void mgl_data_mirror_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_mirror(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
mreal mgl_data_linear(HCDT d, mreal x,mreal y,mreal z)
{
	long kx=long(x), ky=long(y), kz=long(z);
	mreal b0,b1;
	const mglData *dd=dynamic_cast<const mglData *>(d);
	if(dd)
	{
		long nx=dd->nx, ny=dd->ny, nz=dd->nz, dn=ny>1?nx:0;
		kx = kx<nx-1 ? kx:nx-2;	kx = kx>=0 ? kx:0;
		ky = ky<ny-1 ? ky:ny-2;	ky = ky>=0 ? ky:0;
		kz = kz<nz-1 ? kz:nz-2;	kz = kz>=0 ? kz:0;
		x -= kx;	if(nx==1)	x=0;
		y -= ky;	if(ny==1)	y=0;
		z -= kz;	if(nz==1)	z=0;

		const mreal *aa=dd->a+kx+nx*(ky+ny*kz), *bb = aa+(nz>1?nx*ny:0);
		b0 = aa[0]*(1-x-y+x*y) + x*(1-y)*aa[1] + y*(1-x)*aa[dn] + x*y*aa[1+dn];
		b1 = bb[0]*(1-x-y+x*y) + x*(1-y)*bb[1] + y*(1-x)*bb[dn] + x*y*bb[1+dn];
	}
	else
	{
		long n=d->GetNx(), ny=d->GetNy(), nz=d->GetNz();
		kx = (kx>=0 ? (kx<n ? kx:n -1):0);
		ky = (ky>=0 ? (ky<ny? ky:ny-1):0);
		kz = (kz>=0 ? (kz<nz? kz:nz-1):0);
		x -= kx;	y -= ky;	z -= kz;

		b0 = d->v(kx,ky,kz)*(1-x-y+x*y) + x*(1-y)*d->v(kx+1,ky,kz) +
			y*(1-x)*d->v(kx,ky+1,kz) + x*y*d->v(kx+1,ky+1,kz);
		kz++;
		b1 = d->v(kx,ky,kz)*(1-x-y+x*y) + x*(1-y)*d->v(kx+1,ky,kz) +
			y*(1-x)*d->v(kx,ky+1,kz) + x*y*d->v(kx+1,ky+1,kz);
	}
	return b0 + z*(b1-b0);
}
//-----------------------------------------------------------------------------
mreal mgl_data_spline(HCDT d, mreal x,mreal y,mreal z)
{
	const mglData *dd=dynamic_cast<const mglData *>(d);
	if(!d)	return 0;	// NOTE: don't support general arrays
	return mglSpline3(dd->a,dd->nx,dd->ny,dd->nz,x,y,z,0,0,0);
}
//-----------------------------------------------------------------------------
mreal mglSpline3(const mreal *a, long nx, long ny, long nz, mreal x, mreal y, mreal z,mreal *dx, mreal *dy, mreal *dz)
{
	if(!a || nx<1 || ny<1 || nz<1)	return 0;
	mreal _p[4][4];
	register long i,j;
	register mreal fx=1, fy=1;
	long kx=long(x),ky=long(y),kz=long(z);
	bool dd = (dx && dy && dz);
	mreal b=0;
	x = x>0 ?(x<nx-1 ? x:nx-1):0;
	y = y>0 ?(y<ny-1 ? y:ny-1):0;
	z = z>0 ?(z<nz-1 ? z:nz-1):0;
	//	if(x<0 || y<0 || z<0 || x>nx-1 || y>ny-1 || z>nz-1)		return 0;
	if(dd)	{	*dx=*dy=*dz=0;	}
	if(nz>1 && z!=kz)		// 3d interpolation
	{						// TODO: add dx,dy,dz evaluation
		mreal b1[4]={0,0,0,0};
		if(kx>nx-2)	kx = nx-2;
		if(ky>ny-2)	ky = ny-2;
		if(kz>nz-2)	kz = nz-2;

		long kk=1;
		if(kz==0)	{	kk=0;	}
		else if(nz>3 && kz==nz-2)	{	kk=2;	}
		for(long k=0;k<4;k++)
		{
			mglFillP(kx, ky, a+(kz+k-kk)*nx*ny, nx, ny, _p);
			for(i=0,fx=1;i<4;i++)
			{
				for(j=0,fy=1;j<4;j++)
				{	b1[k] += fy*fx*_p[i][j];	fy *= y-ky;	}
				fx *= x-kx;
			}
		}
		mglFillP(kk, b1, nz>3 ? 4:3, _p[0]);
		for(i=0,fx=1,b=0;i<4;i++)
		{	b += fx*_p[0][i];	fx *= z-kz;	}
	}
	else if(ny>1 && y!=ky)	// 2d interpolation
	{
		if(kx>nx-2)	kx = nx-2;
		if(ky>ny-2)	ky = ny-2;
		mglFillP(kx, ky, a+kz*nx*ny, nx, ny, _p);
		fx = 1;	b = 0;
		for(i=0;i<4;i++)
		{
			fy = 1;
			for(j=0;j<4;j++)
			{
				b += fy*fx*_p[i][j];
				if(dd)
				{
					*dx+= i*fy*fx*_p[i][j];
					*dy+= j*fy*fx*_p[i][j];
				}
				fy *= y-ky;
			}
			fx *= x-kx;
		}
		if(dd)	{	*dx /= x-kx;	*dy /= y-ky;	}
	}
	else if(nx>1 && x!=kx)	// 1d interpolation
	{
		if(kx>nx-2)	kx = nx-2;
		mglFillP(kx, a+(ky+ny*kz)*nx, nx, _p[0]);
		for(i=0,fx=1,b=0;i<4;i++)
		{
			b += fx*_p[0][i];
			if(dd)	*dx+= i*fx*_p[0][i];
			fx *= x-kx;
		}
		if(dd)	*dx /= x-kx;
	}
	else					// no interpolation
		b = a[kx+nx*(ky+ny*kz)];
	return b;
}
//-----------------------------------------------------------------------------
void mglFillP(long x,long y, const mreal *a,long nx,long ny,mreal _p[4][4])
{
	mreal sx[4]={0,0,0,0},sy[4]={0,0,0,0},f[4]={0,0,0,0},d[4]={0,0,0,0};
	if(x<0 || y<0 || x>nx-2 || y>ny-2)
	{
		memset(_p[0],0,4*sizeof(mreal));
		memset(_p[1],0,4*sizeof(mreal));
		memset(_p[2],0,4*sizeof(mreal));
		memset(_p[3],0,4*sizeof(mreal));
		return;
	}
	// �������� �������
	f[0]=a[x+nx*y];		f[1]=a[x+nx*(y+1)];
	if(nx>1)	{	f[2]=a[x+1+nx*y];	f[3]=a[x+1+nx*(y+1)];	}
	else		{	f[2] = f[0];	f[3] = f[1];	}
	// ����������� �� x
	if(nx>1)
	{
		if(x==0)
		{
			sx[0]=a[x+1+y*nx]-a[x+nx*y];
			sx[1]=a[x+1+nx*(y+1)]-a[x+nx*(y+1)];
		}
		else
		{
			sx[0]=(a[x+1+nx*y]-a[x-1+nx*y])/2;
			sx[1]=(a[x+1+nx*(y+1)]-a[x-1+nx*(y+1)])/2;
		}
	}
	if(x==nx-2)
	{
		sx[2]=a[x+1+nx*y]-a[x+nx*y];
		sx[3]=a[x+1+nx*(y+1)]-a[x+nx*(y+1)];
	}
	else
	{
		sx[2]=(a[x+2+nx*y]-a[x+nx*y])/2;
		sx[3]=(a[x+2+nx*(y+1)]-a[x+nx*(y+1)])/2;
	}
	// ����������� �� y
	if(y==0)
	{
		sy[0]=a[x+nx*(y+1)]-a[x+nx*y];
		sy[2]=a[x+1+nx*(y+1)]-a[x+1+nx*y];
	}
	else
	{
		sy[0]=(a[x+nx*(y+1)]-a[x+nx*(y-1)])/2;
		sy[2]=(a[x+1+nx*(y+1)]-a[x+1+nx*(y-1)])/2;
	}
	if(y==ny-2)
	{
		sy[1]=a[x+nx*(y+1)]-a[x+nx*y];
		sy[3]=a[x+1+nx*(y+1)]-a[x+1+nx*y];
	}
	else
	{
		sy[1]=(a[x+nx*(y+2)]-a[x+nx*y])/2;
		sy[3]=(a[x+1+nx*(y+2)]-a[x+1+nx*y])/2;
	}
	// ������������ �����������
	if(nx>1)
	{
		// ������ d[0]
		if(y==0 && x==0)
			d[0]=(a[x+1+nx*(y+1)]-a[x+nx*(y+1)]-a[x+1+nx*y]+a[x+nx*y]);
		else if(y==0)
			d[0]=(a[x+1+nx*(y+1)]-a[x-1+nx*(y+1)]-a[x+1+nx*y]+a[x-1+nx*y])/2;
		else if(x==0)
			d[0]=(a[x+1+nx*(y+1)]-a[x+nx*(y+1)]-a[x+1+nx*(y-1)]+a[x+nx*(y-1)])/2;
		else
			d[0]=(a[x+1+nx*(y+1)]-a[x-1+nx*(y+1)]-a[x+1+nx*(y-1)]+a[x-1+nx*(y-1)])/4;
		// ������ d[1]
		if(y==ny-2 && x==0)
			d[1]=(a[x+1+nx*(y+1)]-a[x+nx*(y+1)]-a[x+1+nx*y]+a[x+nx*y]);
		else if(y==ny-2)
			d[1]=(a[x+1+nx*(y+1)]-a[x-1+nx*(y+1)]-a[x+1+nx*y]+a[x-1+nx*y])/2;
		else if(x==0)
			d[1]=(a[x+1+nx*(y+2)]-a[x+nx*(y+2)]-a[x+1+nx*y]+a[x+nx*y])/2;
		else
			d[1]=(a[x+1+nx*(y+2)]-a[x-1+nx*(y+2)]-a[x+1+nx*y]+a[x-1+nx*y])/4;
		// ������ d[2]
		if(y==0 && x==nx-2)
			d[2]=(a[x+1+nx*(y+1)]-a[x+nx*(y+1)]-a[x+1+nx*y]+a[x+nx*y]);
		else if(y==0)
			d[2]=(a[x+2+nx*(y+1)]-a[x+nx*(y+1)]-a[x+2+nx*y]+a[x+nx*y])/2;
		else if(x==nx-2)
			d[2]=(a[x+1+nx*(y+1)]-a[x+nx*(y+1)]-a[x+1+nx*(y-1)]+a[x+nx*(y-1)])/2;
		else
			d[2]=(a[x+2+nx*(y+1)]-a[x+nx*(y+1)]-a[x+2+nx*(y-1)]+a[x+nx*(y-1)])/4;
		// ������ d[3]
		if(y==ny-2 && x==nx-2)
			d[3]=(a[x+1+nx*(y+1)]-a[x+nx*(y+1)]-a[x+1+nx*y]+a[x+nx*y]);
		else if(y==ny-2)
			d[3]=(a[x+2+nx*(y+1)]-a[x+nx*(y+1)]-a[x+2+nx*y]+a[x+nx*y])/2;
		else if(x==nx-2)
			d[3]=(a[x+1+nx*(y+2)]-a[x+nx*(y+2)]-a[x+1+nx*y]+a[x+nx*y])/2;
		else
			d[3]=(a[x+2+nx*(y+2)]-a[x+nx*(y+2)]-a[x+2+nx*y]+a[x+nx*y])/4;
	}
	// ��������� ������������ ��������
	_p[0][0]=f[0];		_p[1][0]=sx[0];
	_p[2][0]=3*(f[2]-f[0])-2*sx[0]-sx[2];
	_p[3][0]=sx[0]+sx[2]+2*(f[0]-f[2]);
	_p[0][1]=sy[0];		_p[1][1]=d[0];
	_p[2][1]=3*(sy[2]-sy[0])-2*d[0]-d[2];
	_p[3][1]=d[0]+d[2]+2*(sy[0]-sy[2]);
	_p[0][2]=3*(f[1]-f[0])-2*sy[0]-sy[1];
	_p[1][2]=3*(sx[1]-sx[0])-2*d[0]-d[1];
	_p[2][2]=9*(f[0]-f[1]-f[2]+f[3])+6*(sy[0]-sy[2]+sx[0]-sx[1])+
		3*(sx[2]-sx[3]+sy[1]-sy[3])+2*(d[1]+d[2])+4*d[0]+d[3];
	_p[3][2]=6*(f[1]+f[2]-f[0]-f[3])+3*(sx[1]-sx[0]+sx[3]-sx[2])+
		4*(sy[2]-sy[0])+2*(sy[3]-sy[1]-d[0]-d[2])-d[1]-d[3];
	_p[0][3]=2*(f[0]-f[1])+sy[0]+sy[1];
	_p[1][3]=2*(sx[0]-sx[1])+d[0]+d[1];
	_p[2][3]=6*(f[1]+f[2]-f[0]-f[3])+3*(sy[2]-sy[1]+sy[3]-sy[0])+
		4*(sx[1]-sx[0])+2*(sx[3]-sx[2]-d[0]-d[1])-d[2]-d[3];
	_p[3][3]=d[0]+d[1]+d[2]+d[3]+4*(f[0]-f[1]-f[2]+f[3])+
		2*(sx[0]-sx[1]+sx[2]-sx[3]+sy[0]-sy[2]+sy[1]-sy[3]);
}
//-----------------------------------------------------------------------------
void mglFillP(long x, const mreal *a,long nx,mreal _p[4])
{
	if(x<0 || x>nx-2)
	{
		memset(_p,0,4*sizeof(mreal));
		return;
	}
	mreal s[2],f[2];
	// �������� �������
	f[0]=a[x];		f[1]=a[x+1];
	// ����������� �� x
	if(x==0)	s[0]=a[x+1]-a[x];
	else		s[0]=(a[x+1]-a[x-1])/2;
	if(x==nx-2)	s[1]=a[x+1]-a[x];
	else		s[1]=(a[x+2]-a[x])/2;
	// ��������� ������������ ��������
	_p[0]=f[0];		_p[1]=s[0];
	_p[2]=3*(f[1]-f[0])-2*s[0]-s[1];
	_p[3]=s[0]+s[1]+2*(f[0]-f[1]);
}
//-----------------------------------------------------------------------------
mreal mgl_data_spline_(uintptr_t *d, mreal *x,mreal *y,mreal *z)
{	return mgl_data_spline(_DA_(d),*x,*y,*z);	}
mreal mgl_data_linear_(uintptr_t *d, mreal *x,mreal *y,mreal *z)
{	return mgl_data_linear(_DA_(d),*x,*y,*z);	}
//-----------------------------------------------------------------------------
void mgl_data_crop(HMDT d, long n1, long n2, char dir)
{
	long nx=d->nx,ny=d->ny,nz=d->nz, nn;
	register long i,k;
	mreal *b;
	if(n1<0)	n1=0;
	switch(dir)
	{
	case 'x':
		n2 = n2>0 ? n2 : nx+n2;
		if(n2<0 || n2>=nx || n2<n1)	n2 = nx;
		nn = n2-n1;	b = new mreal[nn*ny*nz];
		for(i=0;i<ny*nz;i++)
			memcpy(b+nn*i,d->a+nx*i+n1,nn*sizeof(mreal));
		d->nx = nn;	if(!d->link)	delete []d->a;
		d->a = b;	d->link=false;	d->NewId();
		break;
	case 'y':
		n2 = n2>0 ? n2 : ny+n2;
		if(n2<0 || n2>=ny || n2<n1)	n2 = ny;
		nn = n2-n1;	b = new mreal[nn*nx*nz];
		for(long j=0;j<nz;j++)	for(k=0;k<nn;k++)
			memcpy(b+nx*(k+nn*j),d->a+nx*(n1+k+ny*j),nx*sizeof(mreal));
		d->ny = nn;	if(!d->link)	delete []d->a;
		d->a = b;	d->link=false;
		break;
	case 'z':
		n2 = n2>0 ? n2 : nz+n2;
		if(n2<0 || n2>=nz || n2<n1)	n2 = nz;
		nn = n2-n1;	b = new mreal[nn*nx*ny];
		memcpy(b,d->a+nx*ny*n1,nn*nx*ny*sizeof(mreal));
		d->nz = nn;	if(!d->link)	delete []d->a;
		d->a = b;	d->link=false;
		break;
	}
}
void mgl_data_crop_(uintptr_t *d, int *n1, int *n2, const char *dir,int)
{	mgl_data_crop(_DT_,*n1,*n2,*dir);	}
//-----------------------------------------------------------------------------
mreal mgl_data_last(HCDT d, const char *cond, long *i, long *j, long *k)
{
	long nx=d->GetNx(),ny=d->GetNy(),nz=d->GetNz();
	if(!cond)	cond = "u";
	mglFormula eq(cond);
	if(*i<0 || *i>=nx)	*i=nx;
	if(*j<0 || *j>=ny)	*j=ny-1;
	if(*k<0 || *k>=nz)	*k=nz-1;
	register long i0 = *i+nx*(*j+ny*(*k))-1;
	mreal x,y,z,dx=nx>1?1/(nx-1.):0,dy=ny>1?1/(ny-1.):0,dz=nz>1?1/(nz-1.):0;
	for(;i0>=0;i0--)
	{
		x = dx*(i0%nx);		y = dy*((i0/nx)%ny);	z = dz*(i0/(nx*ny));
		if(eq.Calc(x,y,z,d->vthr(i0)))	break;
	}
	*i = i0%nx;	*j = (i0/nx)%ny;	*k = i0/(nx*ny);
	return i0>=0 ? d->vthr(i0) : NAN;	// NOTE: Return NAN if false
}
mreal mgl_data_last_(uintptr_t *d, const char *cond, int *i, int *j, int *k, int l)
{	long ii,jj,kk;	char *s=new char[l+1];	memcpy(s,cond,l);	s[l]=0;
	mreal res = mgl_data_last(_DT_,s,&ii,&jj,&kk);	*i=ii;	*j=jj;	*k=kk;
	delete []s;		return res;	}
//-----------------------------------------------------------------------------
mreal mgl_data_first(HCDT d, const char *cond, long *i, long *j, long *k)
{
	long nx=d->GetNx(),ny=d->GetNy(),nz=d->GetNz();
	if(!cond)	cond = "u";
	mglFormula eq(cond);
	if(*i<0 || *i>=nx)	*i=nx;
	if(*j<0 || *j>=ny)	*j=ny-1;
	if(*k<0 || *k>=nz)	*k=nz-1;
	register long i0 = *i+nx*(*j+ny*(*k))-1;
	mreal x,y,z,dx=nx>1?1/(nx-1.):0,dy=ny>1?1/(ny-1.):0,dz=nz>1?1/(nz-1.):0;
	for(;i0<nx*ny*nz;i0--)
	{
		x = dx*(i0%nx);		y = dy*((i0/nx)%ny);	z = dz*(i0/(nx*ny));
		if(eq.Calc(x,y,z,d->vthr(i0)))	break;
	}
	*i = i0%nx;	*j = (i0/nx)%ny;	*k = i0/(nx*ny);
	return i0<nx*ny*nz ? d->vthr(i0) : NAN;	// NOTE: Return NAN if false
}
mreal mgl_data_first_(uintptr_t *d, const char *cond, int *i, int *j, int *k, int l)
{	long ii,jj,kk;	char *s=new char[l+1];	memcpy(s,cond,l);	s[l]=0;
	mreal res = mgl_data_first(_DT_,s,&ii,&jj,&kk);	*i=ii;	*j=jj;	*k=kk;
	delete []s;		return res;	}
//-----------------------------------------------------------------------------
long mgl_data_find(HCDT d, const char *cond, char dir, long i, long j, long k)
{
	long nx=d->GetNx(),ny=d->GetNy(),nz=d->GetNz();
	register long m=-1;
	if(!cond)	cond = "u";
	mglFormula eq(cond);
	mreal x=i/(nx-1.),y=j/(ny-1.),z=k/(nz-1.);
	if(dir=='x' && nx>1)	for(m=i;m<nx;m++)
		if(eq.Calc(m/(nx-1.),y,z,d->v(m,j,k)))	break;
	if(dir=='y' && ny>1)	for(m=j;m<ny;m++)
		if(eq.Calc(x,m/(ny-1.),z,d->v(i,m,k)))	break;
	if(dir=='z' && nz>1)	for(m=k;m<nz;m++)
		if(eq.Calc(x,y,m/(nz-1.),d->v(i,j,m)))	break;
	return m;
}
int mgl_data_find_(uintptr_t *d, const char *cond, char *dir, int *i, int *j, int *k, int l, int)
{	char *s=new char[l+1];	memcpy(s,cond,l);	s[l]=0;
	int res = mgl_data_find(_DT_,s,*dir,*i,*j,*k);	delete []s;	return res;	}
//-----------------------------------------------------------------------------
int mgl_data_find_any(HCDT d, const char *cond)
{
	long nx=d->GetNx(),ny=d->GetNy(),nz=d->GetNz();
	register long i,j,k;
	register mreal x,y,z;
	bool cc = false;
	if(!cond || *cond==0)	cond = "u";
	mglFormula eq(cond);
	for(k=0;k<nz;k++)	for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
	{
		x=i/(nx-1.);	y=j/(ny-1.);	z=k/(nz-1.);
		if(eq.Calc(x,y,z,d->v(i,j,k)))	{	cc = true;	break;	}
	}
	return cc;
}
int mgl_data_find_any_(uintptr_t *d, const char *cond, int l)
{	char *s=new char[l+1];	memcpy(s,cond,l);	s[l]=0;
	int res = mgl_data_find_any(_DT_,s);	delete []s;	return res;	}
//-----------------------------------------------------------------------------
mreal mgl_data_momentum_val(HCDT dd, char dir, mreal *x, mreal *w, mreal *s, mreal *k)
{
	long nx=dd->GetNx(),ny=dd->GetNy(),nz=dd->GetNz();
	mreal i0=0,i1=0,i2=0,i3=0,i4=0,d,t,v;
	register long i;
	const mglData *md = dynamic_cast<const mglData *>(dd);
	if(dd)	switch(dir)
	{
	case 'x':
		for(i=0;i<nx*ny*nz;i++)
		{
			d = i%nx;		t = d*d;
			v = md->a[i];i0+= v;
			i1+= v*d;		i2+= v*t;
			i3+= v*d*t;		i4+= v*t*t;
		}
		break;
	case 'y':
		for(i=0;i<nx*ny*nz;i++)
		{
			d = (i/nx)%ny;	t = d*d;
			v = md->a[i];i0+= v;
			i1+= v*d;		i2+= v*t;
			i3+= v*d*t;		i4+= v*t*t;
		}
		break;
	case 'z':
		for(i=0;i<nx*ny*nz;i++)
		{
			d = i/(nx*ny);	t = d*d;
			v = md->a[i];i0+= v;
			i1+= v*d;		i2+= v*t;
			i3+= v*d*t;		i4+= v*t*t;
		}
		break;
	default:	// "self-dispersion"
		i0 = nx*ny*nz;
		for(i=0;i<nx*ny*nz;i++)
		{
			v=md->a[i];	t  = v*v;
			i1+= v;			i2+= t;
			i3+= v*t;		i4+= t*t;
		}
	}
	else	switch(dir)
	{
	case 'x':
		for(i=0;i<nx*ny*nz;i++)
		{
			d = i%nx;		t = d*d;
			v = dd->vthr(i);i0+= v;
			i1+= v*d;		i2+= v*t;
			i3+= v*d*t;		i4+= v*t*t;
		}
		break;
	case 'y':
		for(i=0;i<nx*ny*nz;i++)
		{
			d = (i/nx)%ny;	t = d*d;
			v = dd->vthr(i);i0+= v;
			i1+= v*d;		i2+= v*t;
			i3+= v*d*t;		i4+= v*t*t;
		}
		break;
	case 'z':
		for(i=0;i<nx*ny*nz;i++)
		{
			d = i/(nx*ny);	t = d*d;
			v = dd->vthr(i);i0+= v;
			i1+= v*d;		i2+= v*t;
			i3+= v*d*t;		i4+= v*t*t;
		}
		break;
	default:	// "self-dispersion"
		i0 = nx*ny*nz;
		for(i=0;i<nx*ny*nz;i++)
		{
			v=dd->vthr(i);	t  = v*v;
			i1+= v;			i2+= t;
			i3+= v*t;		i4+= t*t;
		}
	}
	if(i0==0)	return 0;	d=i1/i0;
	if(x)	*x=d;
	if(w)	*w=i2>d*d*i0 ? sqrt(i2/i0-d*d) : 0;
	if(s)	*s=i3/i0;
	if(k)	*k=i4/(i0*3);
	return i0;
}
mreal mgl_data_momentum_val_(uintptr_t *d, char *dir, mreal *m, mreal *w, mreal *s, mreal *k,int)
{	mreal mm,ww,ss,kk,aa;
	aa = mgl_data_momentum_val(_DT_,*dir,&mm,&ww,&ss,&kk);
	*m=mm;	*w=ww;	*s=ss;	*k=kk;	return aa;	}
//-----------------------------------------------------------------------------
void mgl_data_norm_slice(HMDT d, mreal v1,mreal v2,char dir,long keep_en,long sym)
{
	long nx=d->nx, ny=d->ny, nz=d->nz;
	mreal *a=d->a;
	mglData b(*d);
	register long i,k;
	register mreal e0=1, e=1, m1, m2, aa;
	if(sym)	{	v2 = -v1>v2 ? -v1:v2;	v1 = -v2;	}
	if(dir=='z' && nz>1)
	{
		for(k=0;k<nz;k++)
		{
			m1 = 1e20;	m2 = -1e20;
			for(i=0;i<nx*ny;i++)
			{
				aa = a[i+nx*ny*k];
				m1 = m1<aa ? m1 : aa;
				m2 = m2>aa ? m2 : aa;
				e += aa*aa;
			}
			if(m1==m2)	m2+=1;
			if(sym)	{	m2 = -m1>m2 ? -m1:m2;	m1 = -m2;	}
			if(keep_en && k)	e = sqrt(e/e0);
			else	{	e0 = e;	e=1;	}
			for(i=0;i<nx*ny;i++)
				b.a[i+nx*ny*k] = (v1 + (v2-v1)*(a[i+nx*ny*k]-m1)/(m2-m1))*e;
		}
	}
	else if(dir=='y' && ny>1)
	{
		register long j;
		for(j=0;j<ny;j++)
		{
			m1 = 1e20;	m2 = -1e20;
			for(k=0;k<nz;k++)	for(i=0;i<nx;i++)
			{
				aa = a[i+nx*(j+ny*k)];
				m1 = m1<aa ? m1 : aa;
				m2 = m2>aa ? m2 : aa;
				e += aa*aa;
			}
			if(m1==m2)	m2+=1;
			if(sym)	{	m2 = -m1>m2 ? -m1:m2;	m1 = -m2;	}
			if(keep_en && j)	e = sqrt(e/e0);
			else	{	e0 = e;	e=1;	}
			for(k=0;k<nz;k++)	for(i=0;i<nx;i++)
				b.a[i+nx*(j+ny*k)] = (v1 + (v2-v1)*(a[i+nx*(j+ny*k)]-m1)/(m2-m1))*e;
		}
	}
	else if(dir=='x' && nx>1)
	{
		for(i=0;i<nx;i++)
		{
			m1 = 1e20;	m2 = -1e20;
			for(k=0;k<ny*nz;k++)
			{
				aa = a[i+nx*k];
				m1 = m1<aa ? m1 : aa;
				m2 = m2>aa ? m2 : aa;
				e += aa*aa;
			}
			if(m1==m2)	m2+=1;
			if(sym)	{	m2 = -m1>m2 ? -m1:m2;	m1 = -m2;	}
			if(keep_en && i)	e = sqrt(e/e0);
			else	{	e0 = e;	e=1;	}
			for(k=0;k<ny*nz;k++)
				b.a[i+nx*k] = (v1 + (v2-v1)*(a[i+nx*k]-m1)/(m2-m1))*e;
		}
	}
	memcpy(d->a, b.a, nx*ny*nz*sizeof(mreal));
}
void mgl_data_norm_slice_(uintptr_t *d, mreal *v1,mreal *v2,char *dir,int *keep_en,int *sym,int )
{	mgl_data_norm_slice(_DT_,*v1,*v2,*dir,*keep_en,*sym);	}
//-----------------------------------------------------------------------------
const char *mgl_data_info(HCDT d)	// NOTE: Not thread safe function!
{
	static char buf[512];
	char s[128];	buf[0]=0;
	sprintf(s,"nx = %ld\tny = %ld\tnz = %ld\n",d->GetNx(),d->GetNy(),d->GetNz());	strcat(buf,s);

	long i=0,j=0,k=0;
	mreal A=0,Wa=0,X=0,Y=0,Z=0,Wx=0,Wy=0,Wz=0, b;
	b = mgl_data_max_int(d,&i,&j,&k);
	sprintf(s,"Maximum is %g\t at x = %ld\ty = %ld\tz = %ld\n", b,i,j,k);	strcat(buf,s);
	b = mgl_data_min_int(d,&i,&j,&k);
	sprintf(s,"Minimum is %g\t at x = %ld\ty = %ld\tz = %ld\n", b,i,j,k);	strcat(buf,s);

	mgl_data_momentum_val(d,'a',&A,&Wa,0,0);	mgl_data_momentum_val(d,'x',&X,&Wx,0,0);
	mgl_data_momentum_val(d,'y',&Y,&Wy,0,0);	mgl_data_momentum_val(d,'z',&Z,&Wz,0,0);
	sprintf(s,"Averages are:\n<a> = %g\t<x> = %g\t<y> = %g\t<z> = %g\n", A,X,Y,Z);	strcat(buf,s);
	sprintf(s,"Widths are:\nWa = %g\tWx = %g\tWy = %g\tWz = %g\n", Wa,Wx,Wy,Wz);	strcat(buf,s);
	return buf;
}
//-----------------------------------------------------------------------------
void mgl_data_insert(HMDT d, char dir, long at, long num)
{
	if(num<1)	return;
	at = at<0 ? 0:at;
	register long i,k,nn;
	long nx=d->nx, ny=d->ny, nz=d->nz;
	mglData b;
	if(dir=='x')
	{
		if(at>nx)	at=nx;
		nn=nx+num;	b.Create(nn,ny,nz);
		for(k=0;k<ny*nz;k++)
		{
			if(at>0)	memcpy(b.a+nn*k, d->a+nx*k,at*sizeof(mreal));
			if(at<nx)	memcpy(b.a+at+num+nn*k, d->a+at+nx*k,(nx-at)*sizeof(mreal));
			for(i=0;i<num;i++)	b.a[nn*k+at+i]=d->a[nx*k+at];	// copy values
		}
		d->Set(b);	nx+=num;
	}
	if(dir=='y')
	{
		if(at>ny)	at=ny;
		nn=num+ny;	b.Create(nx,nn,nz);
		for(k=0;k<nz;k++)
		{
			if(at>0)	memcpy(b.a+nx*nn*k, d->a+nx*ny*k,at*nx*sizeof(mreal));
			if(at<ny)	memcpy(b.a+nx*(at+num+nn*k), d->a+nx*(at+ny*k),(ny-at)*nx*sizeof(mreal));
			for(i=0;i<num;i++)	memcpy(b.a+nx*(nn*k+at+i),d->a+nx*(ny*k+at),nx*sizeof(mreal));
		}
		d->Set(b);	ny+=num;
	}
	if(dir=='z')
	{
		if(at>nz)	at=nz;
		b.Create(nx,ny,nz+num);
		if(at>0)	memcpy(b.a, d->a,at*nx*ny*sizeof(mreal));
		if(at<nz)	memcpy(b.a+nx*ny*(at+num), d->a+nx*ny*at,(nz-at)*nx*ny*sizeof(mreal));
		for(i=0;i<num;i++)	memcpy(b.a+nx*ny*(at+i),d->a+nx*ny*at,nx*ny*sizeof(mreal));
		d->Set(b);	nz+=num;
	}
}
//-----------------------------------------------------------------------------
void mgl_data_delete(HMDT d, char dir, long at, long num)
{
	if(num<1 || at<0)	return;
	mglData b;
	long nx=d->nx, ny=d->ny, nz=d->nz;
	register long k,nn;
	if(dir=='x')
	{
		if(at+num>=nx)	return;
		nn=nx-num;	b.Create(nn,ny,nz);
		for(k=0;k<ny*nz;k++)
		{
			if(at>0)	memcpy(b.a+nn*k, d->a+nx*k,at*sizeof(mreal));
			memcpy(b.a+at+nn*k, d->a+at+num+nx*k,(nx-at-num)*sizeof(mreal));
		}
		d->Set(b);	nx-=num;
	}
	if(dir=='y')
	{
		if(at+num>=ny)	return;
		nn=ny-num;	b.Create(nx,nn,nz);
		for(k=0;k<nz;k++)
		{
			if(at>0)	memcpy(b.a+nx*nn*k, d->a+nx*ny*k,at*nx*sizeof(mreal));
			memcpy(b.a+nx*(at+nn*k), d->a+nx*(at+num+ny*k),(ny-at-num)*nx*sizeof(mreal));
		}
		d->Set(b);	ny-=num;
	}
	if(dir=='z')
	{
		if(at+num>=nz)	return;
		b.Create(nx,ny,nz-num);
		if(at>0)	memcpy(b.a, d->a,at*nx*ny*sizeof(mreal));
		memcpy(b.a+nx*ny*at, d->a+nx*ny*(at+num),(nz-at-num)*nx*ny*sizeof(mreal));
		d->Set(b);	nz-=num;
	}
}
//-----------------------------------------------------------------------------
void mgl_data_insert_(uintptr_t *d, const char *dir, int *at, int *num, int)
{	mgl_data_insert(_DT_,*dir,*at,*num);	}
void mgl_data_delete_(uintptr_t *d, const char *dir, int *at, int *num, int)
{	mgl_data_delete(_DT_,*dir,*at,*num);	}
//-----------------------------------------------------------------------------
mreal mgl_spline5(mreal y1[5], mreal y2[5], long n1, long n2, mreal d, mreal &dy)
{
	mreal a1[4], a2[4], f0,d0,t0,f1,d1,t1, b[6];
	a1[0] = -(3*y1[4]-16*y1[3]+36*y1[2]-48*y1[1]+25*y1[0])/12;
	a1[1] = (11*y1[4]-56*y1[3]+114*y1[2]-104*y1[1]+35*y1[0])/12;
	a1[2] = -(3*y1[4]-14*y1[3]+24*y1[2]-18*y1[1]+5*y1[0])/4;
	a1[3] = (y1[4]-4*y1[3]+6*y1[2]-4*y1[1]+y1[0])/6;
	a2[0] = -(3*y2[4]-16*y2[3]+36*y2[2]-48*y2[1]+25*y2[0])/12;
	a2[1] = (11*y2[4]-56*y2[3]+114*y2[2]-104*y2[1]+35*y2[0])/12;
	a2[2] = -(3*y2[4]-14*y2[3]+24*y2[2]-18*y2[1]+5*y2[0])/4;
	a2[3] = (y2[4]-4*y2[3]+6*y2[2]-4*y2[1]+y2[0])/6;
	n2++;
	f0 = y1[n1];	d0 = a1[0]+n1*(a1[1]+n1*(a1[2]+n1*a1[3]));	t0 = a1[1]/2+a1[2]*n1+1.5*n2*n2*a2[3];
	f1 = y2[n2];	d1 = a2[0]+n2*(a2[1]+n2*(a2[2]+n2*a2[3]));	t1 = a2[1]/2+a2[2]*n2+1.5*n2*n2*a2[3];
	b[0] = f0;	b[1] = d0;	b[2] = t0;
	b[3] = 10*(f1-f0)+t1-3*t0-4*d1-6*d0;
	b[4] = 15*(f0-f1)-2*t1+3*t0+7*d1+8*d0;
	b[5] = 6*(f1-f0)+t1-t0-3*d1-3*d0;
	dy = b[1] + d*(2*b[2]+d*(3*b[3]+d*(4*b[4]+d*5*b[5])));
	return b[0] + d*(b[1]+d*(b[2]+d*(b[3]+d*(b[4]+d*b[5]))));
}
//-----------------------------------------------------------------------------
mreal mgl_spline3(mreal y1[3], mreal y2[3], long n1, long n2, mreal d, mreal &dy)
{
	mreal a1[2], a2[2], f0,d0,d1,f1, b[4];
	a1[0] = -(y1[2]-4*y1[1]+3*y1[0])/2;
	a1[1] = y1[2]-2*y1[1]+y1[0];
	a2[0] = -(y2[2]-4*y2[1]+3*y2[0])/2;
	a2[1] = y2[2]-2*y2[1]+y2[0];
	n2++;
	f0 = y1[n1];	d0 = a1[0]+a1[1]*n1;
	f1 = y2[n2];	d1 = a2[0]+a2[1]*n2;
	b[0] = f0;	b[1] = d0;
	b[2] = 3*(f1-f0)-d1-2*d0;
	b[3] = 2*(f0-f1)+d1+d0;
	dy = b[1] + d*(2*b[2]+d*3*b[3]);
	return b[0] + d*(b[1]+d*(b[2]+d*b[3]));
}
//-----------------------------------------------------------------------------
/*mreal mglData::Spline5(mreal x,mreal y,mreal z,mreal &dx,mreal &dy,mreal &dz) const
{
	mreal res=0;
	if(nx<5)	return 0;	// not interpolation for points < 5 !!!
	dx = dy = dz = 0;	x*=nx-1;	y*=ny-1;	z*=nz-1;
	if(ny==1 && nz==1)	// 1D case
	{
		long n = long(x), n1 = n>1 ? 2:n, n2 = n<nx-3 ? 1:5+n-nx;
		res = mgl_spline5(a+n+n1-2, a+n-n2, n1, n2, x-n, dx);
	}
	else if(nz==1)		// 2D case
	{
	}
	return res;
}*/
//-----------------------------------------------------------------------------
#define omod(x,y)	(y)*((x)>0?int((x)/(y)+0.5):int((x)/(y)-0.5))
void mgl_omod(mreal *a, mreal da, int nx, int n)
{
	register long i,ii;
	bool qq=true;
	register mreal q;
	for(i=1;i<nx;i++)
	{
		ii = i*n;
		if(mgl_isnan(a[ii-n]))	{	qq=true;	continue;	}
		if(qq)
		{
			a[ii] += omod(a[ii-n]-a[ii], da);
			qq=false;
		}
		else
		{
			q = 2*a[ii-n]-a[ii-2*n];
			a[ii] += omod(q-a[ii], da);
		}
	}
}
//-----------------------------------------------------------------------------
void *mgl_sew_z(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i, nz=t->p[2], nn=t->n;
	for(i=t->id;i<nn;i+=mglNumThr)	mgl_omod(t->a+i, t->b[0], nz, nn);
	return 0;
}
void *mgl_sew_y(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i, nx=t->p[0], ny=t->p[1], nn=t->n;
	for(i=t->id;i<nn;i+=mglNumThr)	mgl_omod(t->a+(i%nx)+nx*ny*(i/nx), t->b[0], ny, nx);
	return 0;
}
void *mgl_sew_x(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i, nx=t->p[0], nn=t->n;
	for(i=t->id;i<nn;i+=mglNumThr)	mgl_omod(t->a+i*nx, t->b[0], nx, 1);
	return 0;
}
void mgl_data_sew(HMDT d, const char *dirs, mreal delta)
{
	if(!dirs || *dirs==0)	return;
	long nx=d->nx, ny=d->ny, nz=d->nz;
	long p[3]={nx,ny,nz};
	mreal da = delta;
	if(strchr(dirs,'x') && nx>1)	mglStartThread(mgl_sew_x,0,nz*ny,d->a,&da,0,p);
	if(strchr(dirs,'y') && ny>1)	mglStartThread(mgl_sew_y,0,nz*nx,d->a,&da,0,p);
	if(strchr(dirs,'z') && nz>1)	mglStartThread(mgl_sew_z,0,nx*ny,d->a,&da,0,p);
}
void mgl_data_sew_(uintptr_t *d, const char *dirs, mreal *da, int l)
{	char *s=new char[l+1];	memcpy(s,dirs,l);	s[l]=0;
	mgl_data_sew(_DT_,s,*da);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_data_put_val(HMDT d, mreal val, long xx, long yy, long zz)
{
	register long nx=d->nx, ny=d->ny, nz=d->nz;
	if(xx>=nx || yy>=ny || zz>=nz)	return;
	mreal *a=d->a;
	register long i,j;
	if(xx<0 && yy<0 && zz<0)for(i=0;i<nx*ny*nz;i++)	a[i] = val;
	else if(xx<0 && yy<0)	for(i=0;i<nx*ny;i++)	a[i+zz*nx*ny] = val;
	else if(yy<0 && zz<0)	for(i=0;i<nz*ny;i++)	a[xx+i*nx] = val;
	else if(xx<0 && zz<0)	for(i=0;i<nx;i++)	for(j=0;j<nz;j++)	a[i+nx*(yy+j*ny)] = val;
	else if(xx<0)	for(i=0;i<nx;i++)	a[i+nx*(yy+zz*ny)] = val;
	else if(yy<0)	for(i=0;i<ny;i++)	a[xx+nx*(i+zz*ny)] = val;
	else if(zz<0)	for(i=0;i<nz;i++)	a[xx+nx*(yy+i*ny)] = val;
	else	a[xx+nx*(yy+zz*ny)] = val;
}
//-----------------------------------------------------------------------------
void mgl_data_put_dat(HMDT d, HCDT v, long xx, long yy, long zz)
{
	register long nx=d->nx, ny=d->ny, nz=d->nz;
	if(xx>=nx || yy>=ny || zz>=nz)	return;
	const mglData *mv = dynamic_cast<const mglData *>(v);
	mreal *a=d->a, vv=v->v(0);
	const mreal *b = mv?mv->a:0;
	long vx=v->GetNx(), vy=v->GetNy(), vz=v->GetNz();
	register long i,j,k;
	if(xx<0 && yy<0 && zz<0)	// whole array
	{
		if(vx>=nx && vy>=ny && vz>=nz)
			for(k=0;k<nz;k++)	for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
				a[i+nx*(j+k*ny)] = b?b[i+vx*(j+k*vy)]:v->v(i,j,k);
		else if(vx>=nx && vy>=ny)
			for(k=0;k<nz;k++)	for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
				a[i+nx*(j+k*ny)] = b?b[i+vx*j]:v->v(i,j);
		else if(vx>=nx)	for(k=0;k<ny*nz;k++)	for(i=0;i<nx;i++)
				a[i+nx*k] = b?b[i]:v->v(i);
		else	for(i=0;i<nx*ny*nz;i++)	a[i] = vv;
	}
	else if(xx<0 && yy<0)	// 2d
	{
		if(vx>=nx && vy>=ny)	for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
				a[i+nx*(j+zz*ny)] = b?b[i+vx*j]:v->v(i,j);
		else if(vx>=nx)	for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
				a[i+nx*(j+zz*ny)] = b?b[i]:v->v(i);
		else	for(i=0;i<nx*ny;i++)	a[i+nx*ny*zz] = vv;
	}
	else if(yy<0 && zz<0)	// 2d
	{
		if(vx>=ny && vy>=nz)	for(j=0;j<nz;j++)	for(i=0;i<ny;i++)
				a[xx+nx*(i+j*ny)] = b?b[i+vx*j]:v->v(i,j);
		else if(vx>=ny)	for(j=0;j<nz;j++)	for(i=0;i<ny;i++)
				a[xx+nx*(i+j*ny)] = b?b[i]:v->v(i);
		else	for(i=0;i<ny*nz;i++)	a[xx+nx*i] = vv;
	}
	else if(xx<0 && zz<0)	// 2d
	{
		if(vx>=nx && vy>=nz)	for(j=0;j<nz;j++)	for(i=0;i<nx;i++)
				a[i+nx*(yy+j*ny)] = b?b[i+vx*j]:v->v(i,j);
		else if(vx>=nx)	for(j=0;j<nz;j++)	for(i=0;i<nx;i++)
				a[i+nx*(yy+j*ny)] = b?b[i]:v->v(i);
		else	for(j=0;j<nz;j++)	for(i=0;i<nx;i++)
				a[i+nx*(yy+j*ny)] = vv;
	}
	else if(xx<0)
	{
		if(vx>=nx)	for(i=0;i<nx;i++)	a[i+nx*(yy+zz*ny)] = b?b[i]:v->v(i);
		else for(i=0;i<nx;i++)	a[i+nx*(yy+zz*ny)] = vv;
	}
	else if(yy<0)
	{
		if(vx>=ny)	for(i=0;i<ny;i++)	a[xx+nx*(i+zz*ny)] = b?b[i]:v->v(i);
		else for(i=0;i<ny;i++)	a[xx+nx*(i+zz*ny)] = vv;
	}
	else if(zz<0)
	{
		if(vx>=nz)	for(i=0;i<nz;i++)	a[xx+nx*(yy+i*ny)] = b?b[i]:v->v(i);
		else for(i=0;i<nz;i++)	a[xx+nx*(yy+i*ny)] = vv;
	}
	else	a[xx+nx*(yy+ny*zz)] = vv;
}
//-----------------------------------------------------------------------------
void mgl_data_put_val_(uintptr_t *d, mreal *val, int *i, int *j, int *k)
{	mgl_data_put_val(_DT_,*val, *i,*j,*k);	}
void mgl_data_put_dat_(uintptr_t *d, uintptr_t *val, int *i, int *j, int *k)
{	mgl_data_put_dat(_DT_,_DA_(val), *i,*j,*k);	}
//-----------------------------------------------------------------------------
void *mgl_diff_3(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k, i0, nx=t->p[0], ny=t->p[1], nz=t->p[2], nn=t->n;
	mreal *b=t->a,au,av,aw,xu,xv,xw,yu,yv,yw,zu,zv,zw;
	const mreal *a=t->b, *x=t->c, *y=t->d, *z=t->e;
	for(i0=t->id;i0<nn;i0+=mglNumThr)
	{
		i=i0%nx;	j=((i0/nx)%ny);	k=i0/(nx*ny);
		if(i==0)
		{
			au = 3*a[i0]-4*a[i0+1]+a[i0+2];
			xu = 3*x[i0]-4*x[i0+1]+x[i0+2];
			yu = 3*y[i0]-4*y[i0+1]+y[i0+2];
			zu = 3*z[i0]-4*z[i0+1]+z[i0+2];
		}
		else if(i==nx-1)
		{
			au = 3*a[i0]-4*a[i0-1]+a[i0-2];
			xu = 3*x[i0]-4*x[i0-1]+x[i0-2];
			yu = 3*y[i0]-4*y[i0-1]+y[i0-2];
			zu = 3*z[i0]-4*z[i0-1]+z[i0-2];
		}
		else
		{
			au = a[i0+1]-a[i0-1];
			xu = x[i0+1]-x[i0-1];
			yu = y[i0+1]-y[i0-1];
			zu = z[i0+1]-z[i0-1];
		}
		if(j==0)
		{
			av = 3*a[i0]-4*a[i0+nx]+a[i0+2*nx];
			xv = 3*x[i0]-4*x[i0+nx]+x[i0+2*nx];
			yv = 3*y[i0]-4*y[i0+nx]+y[i0+2*nx];
			zv = 3*z[i0]-4*z[i0+nx]+z[i0+2*nx];
		}
		else if(j==ny-1)
		{
			av = 3*a[i0]-4*a[i0-nx]+a[i0+(ny-3)*nx];
			xv = 3*x[i0]-4*x[i0-nx]+x[i0-2*nx];
			yv = 3*y[i0]-4*y[i0-nx]+y[i0-2*nx];
			zv = 3*z[i0]-4*z[i0-nx]+z[i0-2*nx];
		}
		else
		{
			av = a[i0+nx]-a[i0-nx];
			xv = x[i0+nx]-x[i0-nx];
			yv = y[i0+nx]-y[i0-nx];
			zv = z[i0+nx]-z[i0-nx];
		}
		if(k==0)
		{
			aw = 3*a[i0]-4*a[i0+nn]+a[i0+2*nn];
			xw = 3*x[i0]-4*x[i0+nn]+x[i0+2*nn];
			yw = 3*y[i0]-4*y[i0+nn]+y[i0+2*nn];
			zw = 3*z[i0]-4*z[i0+nn]+z[i0+2*nn];
		}
		else if(k==nz-1)
		{
			aw = 3*a[i0]-4*a[i+(nz-2)*nx*ny]+a[i-2*nn];
			xw = 3*x[i0]-4*x[i-nn]+x[i-2*nn];
			yw = 3*y[i0]-4*y[i-nn]+y[i-2*nn];
			zw = 3*z[i0]-4*z[i-nn]+z[i-2*nn];
		}
		else
		{
			aw = a[i0+nn]-a[i0-nn];
			xw = x[i0+nn]-x[i0-nn];
			yw = y[i0+nn]-y[i0-nn];
			zw = z[i0+nn]-z[i0-nn];
		}
		b[i0] = (au*yv*zw-av*yu*zw-au*yw*zv+aw*yu*zv+av*yw*zu-aw*yv*zu) / (xu*yv*zw-xv*yu*zw-xu*yw*zv+xw*yu*zv+xv*yw*zu-xw*yv*zu);
	}
	return 0;
}
void *mgl_diff_2(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j, i0,i1, nx=t->p[0], ny=t->p[1], nn=t->n, same=t->p[2];
	mreal *b=t->a,au,av,xu,xv,yu,yv;
	const mreal *a=t->b, *x=t->c, *y=t->d;
	for(i0=t->id;i0<nn;i0+=mglNumThr)
	{
		i=i0%nx;	j=((i0/nx)%ny);
		i1 = same ? i0 : i0%(nx*ny);
		if(i==0)
		{
			au = 3*a[i0]-4*a[i0+1]+a[i0+2];
			xu = 3*x[i1]-4*x[i1+1]+x[i1+2];
			yu = 3*y[i1]-4*y[i1+1]+y[i1+2];
		}
		else if(i==nx-1)
		{
			au = 3*a[i0]-4*a[i0-1]+a[i0-2];
			xu = 3*x[i1]-4*x[i1-1]+x[i1-2];
			yu = 3*y[i1]-4*y[i1-1]+y[i1-2];
		}
		else
		{
			au = a[i0+1]-a[i0-1];
			xu = x[i1+1]-x[i1-1];
			yu = y[i1+1]-y[i1-1];
		}
		if(j==0)
		{
			av = 3*a[i0]-4*a[i0+nx]+a[i0+2*nx];
			xv = 3*x[i1]-4*x[i1+nx]+x[i1+2*nx];
			yv = 3*y[i1]-4*y[i1+nx]+y[i1+2*nx];
		}
		else if(j==ny-1)
		{
			av = 3*a[i0]-4*a[i0-nx]+a[i0-2*nx];
			xv = 3*x[i1]-4*x[i1-nx]+x[i1-2*nx];
			yv = 3*y[i1]-4*y[i1-nx]+y[i1-2*nx];
		}
		else
		{
			av = a[i0+nx]-a[i0-nx];
			xv = x[i1+nx]-x[i1-nx];
			yv = y[i1+nx]-y[i1-nx];
		}
		b[i0] = (av*yu-au*yv)/(xv*yu-xu*yv);
	}
	return 0;
}
void *mgl_diff_1(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,i0,i1, nx=t->p[0], nn=t->n, same=t->p[1];
	mreal *b=t->a,au,xu;
	const mreal *a=t->b, *x=t->c;
	for(i0=t->id;i0<nn;i0+=mglNumThr)
	{
		i=i0%nx;	i1 = same ? i0 : i;
		if(i==0)
		{
			au = 3*a[i0]-4*a[i0+1]+a[i0+2];
			xu = 3*x[i1]-4*x[i1+1]+x[i1+2];
		}
		else if(i==nx-1)
		{
			au = 3*a[i0]-4*a[i0-1]+a[i0-2];
			xu = 3*x[i1]-4*x[i1-1]+x[i1-2];
		}
		else
		{
			au = a[i0+1]-a[i0-1];
			xu = x[i1+1]-x[i1-1];
		}
		b[i0] = au/xu;
	}
	return 0;
}
void mgl_data_diff_par(HMDT d, HCDT v1, HCDT v2, HCDT v3)
{	// NOTE: only for mglData!!!
	const mglData *x = dynamic_cast<const mglData *>(v1);
	const mglData *y = dynamic_cast<const mglData *>(v2);
	const mglData *z = dynamic_cast<const mglData *>(v3);
	long nx=d->nx,ny=d->ny,nz=d->nz, nn=nx*ny*nz;
	if(nx<2 || ny<2)	return;
	mreal *b = new mreal[nn];	memset(b,0,nn*sizeof(mreal));
	long p[3]={nx,ny,nz};

	if(x&&y&&z && x->nx*x->ny*x->nz==nn && y->nx*y->ny*y->nz==nn && z->nx*z->ny*z->nz==nn)
		mglStartThread(mgl_diff_3,0,nn,b,d->a,x->a,p,0,y->a,z->a);
	else if(x&&y && x->nx*x->ny==nx*ny && y->nx*y->ny==nx*ny)
	{
		p[2]=(x->nz==nz && y->nz==nz);
		mglStartThread(mgl_diff_2,0,nn,b,d->a,x->a,p,0,y->a);
	}
	else if(x && x->nx==nx)
	{
		p[1]=(x->ny*x->nz==ny*nz);
		mglStartThread(mgl_diff_1,0,nn,b,d->a,x->a,p,0,0);
	}
	memcpy(d->a,b,nn*sizeof(mreal));	delete []b;
}
void mgl_data_diff_par_(uintptr_t *d, uintptr_t *v1, uintptr_t *v2, uintptr_t *v3)
{	mgl_data_diff_par(_DT_,_DA_(v1),_DA_(v2),_DA_(v3));	}
//-----------------------------------------------------------------------------
void mgl_data_set_value(HMDT dat, mreal v, long i, long j, long k)
{	if(i>=0 && i<dat->nx && j>=0 && j<dat->ny && k>=0 && k<dat->nz)	dat->a[i+dat->nx*(j+dat->ny*k)]=v;	}
void mgl_data_set_value_(uintptr_t *d, mreal *v, int *i, int *j, int *k)
{	mgl_data_set_value(_DT_,*v,*i,*j,*k);	}
//-----------------------------------------------------------------------------
mreal mgl_data_get_value(HCDT dat, long i, long j, long k)
{	return (i>=0 && i<dat->GetNx() && j>=0 && j<dat->GetNy() && k>=0 && k<dat->GetNz()) ? dat->v(i,j,k):NAN;	}
mreal mgl_data_get_value_(uintptr_t *d, int *i, int *j, int *k)
{	return mgl_data_get_value(_DA_(d),*i,*j,*k);	}
//-----------------------------------------------------------------------------
mreal *mgl_data_data(HMDT dat)	{	return dat->a;	}
//-----------------------------------------------------------------------------
mreal *mgl_data_value(HMDT dat, long i,long j,long k)
{	register long ii=i*dat->nx*(j+dat->ny*k);
	return	ii>=0 && ii<dat->GetNN() ? dat->a+ii : 0;	}
//-----------------------------------------------------------------------------
