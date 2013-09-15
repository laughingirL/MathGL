/***************************************************************************
 * data.cpp is part of Math Graphic Library
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
#include <time.h>
#include "mgl2/data.h"
#include "mgl2/eval.h"

#include "interp.hpp"

MGL_EXPORT int mglNumThr=0;
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
void MGL_EXPORT mgl_set_num_thr(int n)
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
void MGL_EXPORT mgl_set_num_thr(int)	{	mglNumThr = 1;	}
#endif
//-----------------------------------------------------------------------------
void MGL_EXPORT mglStartThread(void *(*func)(void *), void (*post)(mglThreadD *,mreal *), long n,
					mreal *a, const mreal *b, const mreal *c, const long *p,
					const void *v, const mreal *d, const mreal *e, const char *s)
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
void MGL_EXPORT mglStartThreadV(void *(*func)(void *), long n, mreal *a, const void *b,
					 const void *c, const long *p, const void *v, const mreal *d)
{
	if(!func)	return;
#if MGL_HAVE_PTHREAD
	if(mglNumThr<1)	mgl_set_num_thr(0);
	if(mglNumThr>1)
	{
		pthread_t *tmp=new pthread_t[mglNumThr];
		mglThreadV *par=new mglThreadV[mglNumThr];
		register long i;
		for(i=0;i<mglNumThr;i++)	// put parameters into the structure
		{	par[i].n=n;	par[i].a=a;	par[i].b=b;	par[i].c=c;	par[i].d=d;
			par[i].p=p;	par[i].v=v;	par[i].id=i;par[i].aa=0;	}
		for(i=0;i<mglNumThr;i++)	pthread_create(tmp+i, 0, func, par+i);
		for(i=0;i<mglNumThr;i++)	pthread_join(tmp[i], 0);
		delete []tmp;	delete []par;
	}
	else
#endif
	{
		mglNumThr = 1;
		mglThreadV par;
		par.n=n;	par.a=a;	par.b=b;	par.c=c;	par.d=d;
		par.p=p;	par.v=v;	par.id=0;	par.aa=0;
		func(&par);
	}
}
//-----------------------------------------------------------------------------
mreal mglSpline3(const mreal *a, long nx, long ny, long nz, mreal x, mreal y, mreal z,mreal *dx, mreal *dy, mreal *dz)
{	return mglSpline3t<mreal>(a,nx,ny,nz,x,y,z,dx,dy,dz);	}
//-----------------------------------------------------------------------------
mreal mglLinear(const mreal *a, long nx, long ny, long nz, mreal x, mreal y, mreal z)
{	return mglLineart<mreal>(a,nx,ny,nz,x,y,z);	}
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
double mgl_ipow_(mreal *x,int *n)	{	return mgl_ipow(*x,*n);	}
//-----------------------------------------------------------------------------
double mgl_get_time(const char *time, const char *fmt)
{
#if !defined(WIN32)
	tm t;
	strptime(time,fmt,&t);
	return timegm(&t);
#else
	return NAN;
#endif
}
double mgl_get_time_(const char *time, const char *fmt,int l,int m)
{	char *s=new char[l+1];	memcpy(s,time,l);	s[l]=0;
	char *f=new char[m+1];	memcpy(f,fmt,m); 	f[m]=0;
	double t=mgl_get_time(s,f);	delete []s;	delete []f;	return t;	}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void *mgl_smth_x(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nx=t->p[0];
	mreal *b=t->a, delta=t->c[0], y5,y3,x2y;
	const mreal *a=t->b;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for private(y3,y5,x2y)
#endif
	for(long i=t->id;i<t->n;i+=mglNumThr)
	{
		register long j = i%nx, d3 = 0, d5 = 0;
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
MGL_NO_EXPORT void *mgl_smth_y(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nx=t->p[0],ny=t->p[1];
	mreal *b=t->a, delta=t->c[0], y5,y3,x2y;
	const mreal *a=t->b;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for private(y3,y5,x2y)
#endif
	for(long i=t->id;i<t->n;i+=mglNumThr)
	{
		register long j = (i/nx)%ny, d3 = 0, d5 = 0;
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
MGL_NO_EXPORT void *mgl_smth_z(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long nn=t->p[0]*t->p[1], nz=t->n/nn;
	mreal *b=t->a, delta=t->c[0], y5,y3,x2y;
	const mreal *a=t->b;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for private(y3,y5,x2y)
#endif
	for(long i=t->id;i<t->n;i+=mglNumThr)
	{
		register long j = i/nn, d3 = 0, d5 = 0;
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
void MGL_EXPORT mgl_data_smooth(HMDT d, const char *dirs, mreal delta)
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
void MGL_EXPORT mgl_data_smooth_(uintptr_t *d, const char *dir, mreal *delta,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_smooth(_DT_,s,*delta);		delete []s;	}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void *mgl_csum_z(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nz=t->p[2], nn=t->n;
	mreal *b=t->a;
	const mreal *a=t->b;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for
#endif
	for(long i=t->id;i<nn;i+=mglNumThr)
	{
		b[i] = a[i];
		for(long j=1;j<nz;j++)	b[i+j*nn] = b[i+j*nn-nn] + a[i+j*nn];
	}
	return 0;
}
MGL_NO_EXPORT void *mgl_csum_y(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nx=t->p[0], ny=t->p[1], nn=t->n;
	mreal *b=t->a;
	const mreal *a=t->b;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for
#endif
	for(long i=t->id;i<nn;i+=mglNumThr)
	{
		register long k = (i%nx)+nx*ny*(i/nx);		b[k] = a[k];
		for(long j=1;j<ny;j++)	b[k+j*nx] = b[k+j*nx-nx] + a[k+nx*j];
	}
	return 0;
}
MGL_NO_EXPORT void *mgl_csum_x(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nx=t->p[0], nn=t->n;
	mreal *b=t->a;
	const mreal *a=t->b;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for
#endif
	for(long i=t->id;i<nn;i+=mglNumThr)
	{
		register long k = i*nx;			b[k] = a[k];
		for(long j=1;j<nx;j++)	b[j+k] = b[j+k-1] + a[j+k];
	}
	return 0;
}
void MGL_EXPORT mgl_data_cumsum(HMDT d, const char *dir)
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
void MGL_EXPORT mgl_data_cumsum_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_cumsum(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void *mgl_int_z(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nz=t->p[2], nn=t->n;
	mreal *b=t->a, dd=0.5/nz;
	const mreal *a=t->b;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for
#endif
	for(long i=t->id;i<nn;i+=mglNumThr)
	{
		b[i] = 0;
		for(long j=1;j<nz;j++)	b[i+j*nn] = b[i+j*nn-nn] + (a[i+nn*j]+a[i+j*nn-nn])*dd;
	}
	return 0;
}
MGL_NO_EXPORT void *mgl_int_y(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nx=t->p[0], ny=t->p[1], nn=t->n;
	mreal *b=t->a, dd=0.5/ny;
	const mreal *a=t->b;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for
#endif
	for(long i=t->id;i<nn;i+=mglNumThr)
	{
		register long k = (i%nx)+nx*ny*(i/nx);	b[k] = 0;
		for(long j=1;j<ny;j++)	b[k+j*nx] = b[k+j*nx-nx] + (a[k+j*nx]+a[k+j*nx-nx])*dd;
	}
	return 0;
}
MGL_NO_EXPORT void *mgl_int_x(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nx=t->p[0], nn=t->n;
	mreal *b=t->a, dd=0.5/nx;
	const mreal *a=t->b;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for
#endif
	for(long i=t->id;i<nn;i+=mglNumThr)
	{
		register long k = i*nx;			b[k] = 0;
		for(long j=1;j<nx;j++)	b[j+k] = b[j+k-1] + (a[j+k]+a[j+k-1])*dd;
	}
	return 0;
}
void MGL_EXPORT mgl_data_integral(HMDT d, const char *dir)
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
void MGL_EXPORT mgl_data_integral_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_integral(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void *mgl_dif_z(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nz=t->p[2], nn=t->n;
	mreal *b=t->a, dd=0.5*nz;
	const mreal *a=t->b;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for
#endif
	for(long i=t->id;i<nn;i+=mglNumThr)
	{
		b[i] = -(3*a[i]-4*a[i+nn]+a[i+2*nn])*dd;
		b[i+(nz-1)*nn] = (3*a[i+(nz-1)*nn]-4*a[i+(nz-2)*nn]+a[i+(nz-3)*nn])*dd;
		for(long j=1;j<nz-1;j++)		b[i+j*nn] = (a[i+j*nn+nn]-a[i+j*nn-nn])*dd;
	}
	return 0;
}
MGL_NO_EXPORT void *mgl_dif_y(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nx=t->p[0], ny=t->p[1], nn=t->n;
	mreal *b=t->a, dd=0.5*ny;
	const mreal *a=t->b;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for
#endif
	for(long i=t->id;i<nn;i+=mglNumThr)
	{
		register long k = (i%nx)+nx*ny*(i/nx);
		b[k] = -(3*a[k]-4*a[k+nx]+a[k+2*nx])*dd;
		b[k+(ny-1)*nx] = (3*a[k+(ny-1)*nx]-4*a[k+(ny-2)*nx]+a[k+(ny-3)*nx])*dd;
		for(long j=1;j<ny-1;j++)	b[k+j*nx] = (a[k+j*nx+nx]-a[k+j*nx-nx])*dd;
	}
	return 0;
}
MGL_NO_EXPORT void *mgl_dif_x(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nx=t->p[0], nn=t->n;
	mreal *b=t->a, dd=0.5*nx;
	const mreal *a=t->b;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for
#endif
	for(long i=t->id;i<nn;i+=mglNumThr)
	{
		register long k = i*nx;
		b[k] = -(3*a[k]-4*a[k+1]+a[k+2])*dd;
		b[k+nx-1] = (3*a[k+nx-1]-4*a[k+nx-2]+a[k+nx-3])*dd;
		for(long j=1;j<nx-1;j++)	b[j+k] = (a[j+k+1]-a[j+k-1])*dd;
	}
	return 0;
}
void MGL_EXPORT mgl_data_diff(HMDT d, const char *dir)
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
void MGL_EXPORT mgl_data_diff_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_diff(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void *mgl_dif2_z(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nz=t->p[2], nn=t->n;
	mreal *b=t->a, dd=0.5*nz*nz;
	const mreal *a=t->b;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for
#endif
	for(long i=t->id;i<nn;i+=mglNumThr)
	{
		b[i] = b[i+(nz-1)*nn] = 0;
		for(long j=1;j<nz-1;j++)		b[i+j*nn] = (a[i+j*nn+nn]+a[i+j*nn-nn]-2*a[i+j*nn])*dd;
	}
	return 0;
}
MGL_NO_EXPORT void *mgl_dif2_y(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nx=t->p[0], ny=t->p[1], nn=t->n;
	mreal *b=t->a, dd=0.5*ny*ny;
	const mreal *a=t->b;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for
#endif
	for(long i=t->id;i<nn;i+=mglNumThr)
	{
		register long k = (i%nx)+nx*ny*(i/nx);	b[k] = b[k+(ny-1)*nx] = 0;
		for(long j=1;j<ny-1;j++)	b[k+j*nx] = (a[k+j*nx+nx]+a[k+j*nx-nx]-2*a[k+j*nx])*dd;
	}
	return 0;
}
MGL_NO_EXPORT void *mgl_dif2_x(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nx=t->p[0], nn=t->n;
	mreal *b=t->a, dd=0.5*nx*nx;
	const mreal *a=t->b;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for
#endif
	for(long i=t->id;i<nn;i+=mglNumThr)
	{
		register long k = i*nx;			b[k] = b[k+nx-1] = 0;
		for(long j=1;j<nx-1;j++)	b[j+k] = (a[j+k+1]+a[j+k-1]-2*a[j+k])*dd;
	}
	return 0;
}
void MGL_EXPORT mgl_data_diff2(HMDT d, const char *dir)
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
void MGL_EXPORT mgl_data_diff2_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_diff2(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_swap(HMDT d, const char *dir)
{
	if(!dir || *dir==0)	return;
	if(strchr(dir,'z') && d->nz>1)	mgl_data_roll(d,'z',d->nz/2);
	if(strchr(dir,'y') && d->ny>1)	mgl_data_roll(d,'y',d->ny/2);
	if(strchr(dir,'x') && d->nx>1)	mgl_data_roll(d,'x',d->nx/2);
}
void MGL_EXPORT mgl_data_swap_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_swap(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_roll(HMDT dd, char dir, long num)
{
	long nx=dd->nx,ny=dd->ny,nz=dd->nz, d;
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
#pragma omp parallel for
		for(long i=0;i<nz;i++)
			memcpy(b+nx*(ny-d)+nx*ny*i,a+nx*ny*i,nx*d*sizeof(mreal));
		memcpy(a,b,nx*ny*nz*sizeof(mreal));	delete []b;
	}
	if(dir=='x' && nx>1)
	{
		d = num>0 ? num%nx : (num+nx*(1+num/nx))%nx;
		if(d==0)	return;		// nothing to do
		b = new mreal[nx*ny*nz];
		memcpy(b,a+d,(nx*ny*nz-d)*sizeof(mreal));
#pragma omp parallel for
		for(long i=0;i<nz*ny;i++)
			memcpy(b+nx-d+nx*i,a+nx*i,d*sizeof(mreal));
		memcpy(a,b,nx*ny*nz*sizeof(mreal));	delete []b;
	}
}
void MGL_EXPORT mgl_data_roll_(uintptr_t *d, const char *dir, int *num, int)
{	mgl_data_roll(_DT_,*dir,*num);	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_mirror(HMDT d, const char *dir)
{
	if(!dir || *dir==0)	return;
	long nx=d->nx,ny=d->ny,nz=d->nz;
	mreal *a=d->a;
	if(strchr(dir,'z') && nz>1)
	{
#pragma omp parallel for collapse(2)
		for(long i=0;i<nx*ny;i++)	for(long j=0;j<nz/2;j++)
		{
			register long i0 = i+j*nx*ny, j0 = i+(nz-1-j)*nx*ny;
			register mreal b = a[i0];	a[i0] = a[j0];	a[j0] = b;
		}
	}
	if(strchr(dir,'y') && ny>1)
	{
#pragma omp parallel for
		for(long i=0;i<nx*nz;i++)
		{
			register long j0 = (i%nx)+nx*ny*(i/nx);
			for(long j=0;j<ny/2;j++)
			{
				register long i0 = j0+(ny-1-j)*nx;
				register mreal b = a[j0+j*nx];	a[j0+j*nx] = a[i0];	a[i0] = b;
			}
		}
	}
	if(strchr(dir,'x') && nx>1)
	{
#pragma omp parallel for
		for(long j=0;j<ny*nz;j++)
		{
			register long j0 = j*nx;
			for(long i=0;i<nx/2;i++)
			{
				register long i0 = nx-1-i+j0;
				register mreal b = a[i+j0];	a[i+j0] = a[i0];	a[i0] = b;
			}
		}
	}
}
void MGL_EXPORT mgl_data_mirror_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_mirror(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_clean(HMDT d, long id)
{
	if(id<0 || id+1>d->nx)	return;
	register long i,j,n=d->nx,m=d->ny;
	mreal *b = new mreal[m*n], *a=d->a;
	for(i=j=0;i+1<m;i++)
	{
		if(a[id+n*i]!=a[id+n*i+n])	// this can be saved
		{
#pragma omp parallel for
			for(long k=0;k<n;k++)	b[k+n*j]=a[k+n*i];
			j++;
		}
	}
	// always save last row
	i=n*(m-1);
#pragma omp parallel for
	for(long k=0;k<n;k++)	b[k+n*j]=a[k+i];
	j++;
	memcpy(a,b,n*j*sizeof(mreal));	d->ny = j;
	delete []b;
}
void MGL_EXPORT mgl_data_clean_(uintptr_t *d, int *id)	{	mgl_data_clean(_DT_,*id);	}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void *mgl_solve_x(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nx=t->p[0], ny=t->p[1], nz=t->p[2], n1=t->p[3]?nx-1:1, nn=t->n;
	const mreal *a=t->b, *ii=t->c;
	mreal *b=t->a,val=t->d[0],da = 1e-5*(val?fabs(val):1);
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for
#endif
	for(long l=t->id;l<nn;l+=mglNumThr)
	{
		mreal y1,y2,v,v0,dx,dy,dz,x;
		register long j=l%ny, k=l/ny;	b[l] = NAN;
		if(ii && ii[l]!=ii[l])	continue;
		register long i0 = ii?(ii[l]*n1+1):0;
		if(i0>nx-2)	continue;
		if(a)	for(long i=i0+1;i<nx;i++)
		{
			y1=a[i-1+nx*l];	y2=a[i+nx*l];
			if(val==y1)	{	b[l] = i-1;	break;	}
			if(val==y2)	{	b[l] = i;	break;	}
			if((y1-val)*(y2-val)<0)
			{
				x = i-1 + (val-y1)/(y2-y1);
				v0 = v = mglSpline3(a,nx,ny,nz,x,j,k, &dx,&dy,&dz);
				unsigned kk=0;
				while(fabs(v-val)>da || dx==0)
				{
					x += (val-v)/dx;		kk++;
					v = mglSpline3(a,nx,ny,nz,x,j,k, &dx,&dy,&dz);
					if(kk>=10)
					{
						b[l] = x = fabs(v-val)<fabs(v0-val) ? x:i-1 + (val-y1)/(y2-y1);
						break;
					}
				}
				b[l] = x;	break;
			}
		}
		else 	for(long i=i0+1;i<nx;i++)
		{
			HCDT d=(HCDT)t->v;
			y1=d->v(i-1,j,k);	y2=d->v(i,j,k);
			if(val==y1)	{	b[l] = i-1;	break;	}
			if(val==y2)	{	b[l] = i;	break;	}
			if((y1-val)*(y2-val)<0)
			{	b[l] = i-1 + (val-y1)/(y2-y1);	break;	}
		}
		b[l] /= n1;
	}
	return 0;
}
MGL_NO_EXPORT void *mgl_solve_y(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nx=t->p[0], ny=t->p[1], nz=t->p[2], n1=t->p[3]?ny-1:1, nn=t->n;
	const mreal *a=t->b, *ii=t->c;
	mreal *b=t->a,val=t->d[0],da = 1e-5*(val?fabs(val):1);
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for
#endif
	for(long l=t->id;l<nn;l+=mglNumThr)
	{
		mreal y1,y2,v,v0,dx,dy,dz,x;
		register long j=l%nx, k=l/nx;	b[l] = NAN;
		if(ii && ii[l]!=ii[l])	continue;
		register long i0 = ii?(ii[l]*n1+1):0;
		if(i0>ny-2)	continue;
		if(a)	for(long i=i0+1;i<ny;i++)
		{
			y1=a[j+nx*(i-1+ny*k)];	y2=a[j+nx*(i+ny*k)];
			if(val==y1)	{	b[l] = i-1;	break;	}
			if(val==y2)	{	b[l] = i;	break;	}
			if((y1-val)*(y2-val)<0)
			{
				x = i-1 + (val-y1)/(y2-y1);
				v0 = v = mglSpline3(a,nx,ny,nz,j,x,k, &dx,&dy,&dz);
				unsigned kk=0;
				while(fabs(v-val)>da || dy==0)
				{
					x += (val-v)/dy;		kk++;
					v = mglSpline3(a,nx,ny,nz,j,x,k, &dx,&dy,&dz);
					if(kk>=10)
					{
						b[l] = x = fabs(v-val)<fabs(v0-val) ? x:i-1 + (val-y1)/(y2-y1);
						break;
					}
				}
				b[l] = x;	break;
			}
		}
		else 	for(long i=i0+1;i<ny;i++)
		{
			HCDT d=(HCDT)t->v;
			y1=d->v(j,i-1,k);	y2=d->v(j,i,k);
			if(val==y1)	{	b[l] = i-1;	break;	}
			if(val==y2)	{	b[l] = i;	break;	}
			if((y1-val)*(y2-val)<0)
			{	b[l] = i-1 + (val-y1)/(y2-y1);	break;	}
		}
		b[l] /= n1;
	}
	return 0;
}
MGL_NO_EXPORT void *mgl_solve_z(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nx=t->p[0], ny=t->p[1], nz=t->p[2], n1=t->p[3]?nz-1:1, nn=t->n;
	const mreal *a=t->b, *ii=t->c;
	mreal *b=t->a,val=t->d[0],da = 1e-5*(val?fabs(val):1);
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for
#endif
	for(long l=t->id;l<nn;l+=mglNumThr)
	{
		mreal y1,y2,v,v0,dx,dy,dz,x;
		register long j=l%nx, k=l/nx;	b[l] = NAN;
		if(ii && ii[l]!=ii[l])	continue;
		register long i0 = ii?(ii[l]*n1+1):0;
		if(i0>nz-2)	continue;
		if(a)	for(long i=i0+1;i<nz;i++)
		{
			y1=a[nn*i-nn+l];	y2=a[nn*i+l];
			if(val==y1)	{	b[l] = i-1;	break;	}
			if(val==y2)	{	b[l] = i;	break;	}
			if((y1-val)*(y2-val)<0)
			{
				x = i-1 + (val-y1)/(y2-y1);
				v0 = v = mglSpline3(a,nx,ny,nz,j,k,x, &dx,&dy,&dz);
				unsigned kk=0;
				while(fabs(v-val)>da || dz==0)
				{
					x += (val-v)/dz;		kk++;
					v = mglSpline3(a,nx,ny,nz,j,k,x, &dx,&dy,&dz);
					if(kk>=10)
					{
						b[l] = x = fabs(v-val)<fabs(v0-val) ? x:i-1 + (val-y1)/(y2-y1);
						break;
					}
				}
				b[l] = x;	break;
			}
		}
		else 	for(long i=i0+1;i<nz;i++)
		{
			HCDT d=(HCDT)t->v;
			y1=d->v(j,k,i-1);	y2=d->v(j,k,i);
			if(val==y1)	{	b[l] = i-1;	break;	}
			if(val==y2)	{	b[l] = i;	break;	}
			if((y1-val)*(y2-val)<0)
			{	b[l] = i-1 + (val-y1)/(y2-y1);	break;	}
		}
		b[l] /= n1;
	}
	return 0;
}
HMDT MGL_EXPORT mgl_data_solve(HCDT dat, mreal val, char dir, HCDT i0, int norm)
{
	const mglData *i = dynamic_cast<const mglData *>(i0);
	const mglData *d = dynamic_cast<const mglData *>(dat);
	long p[4]={dat->GetNx(), dat->GetNy(), dat->GetNz(), norm};
	const mreal *ii=0;
	mglData *r=new mglData;
	if(dir=='x' && p[0]>1)
	{
		r->Create(p[1],p[2]);
		if(i && i->nx*i->ny==p[1]*p[2])	ii = i->a;
		mglStartThread(mgl_solve_x,0,p[1]*p[2],r->a,d?d->a:0,ii,p,dat,&val);
	}
	if(dir=='y' && p[1]>1)
	{
		r->Create(p[0],p[2]);
		if(i && i->nx*i->ny==p[0]*p[2])	ii = i->a;
		mglStartThread(mgl_solve_y,0,p[0]*p[2],r->a,d?d->a:0,ii,p,dat,&val);
	}
	if(dir=='z' && p[2]>1)
	{
		r->Create(p[0],p[1]);
		if(i && i->nx*i->ny==p[0]*p[1])	ii = i->a;
		mglStartThread(mgl_solve_z,0,p[0]*p[1],r->a,d?d->a:0,ii,p,dat,&val);
	}
	return r;
}
//-----------------------------------------------------------------------------
mreal MGL_EXPORT mgl_data_solve_1d(HCDT d, mreal val, int spl, long i0)
{
	mreal x=0, y1, y2, a, a0, dx=0,dy,dz, da = 1e-5*(val?fabs(val):1);
	long nx = d->GetNx();
	if(i0<0 || i0>=nx)	i0=0;
	const mglData *dd=dynamic_cast<const mglData *>(d);
	if(dd)	for(long i=i0+1;i<nx;i++)
	{
		y1=dd->a[i-1];	y2=dd->a[i];
		if(val==y1)	return i-1;
		if(val==y2)	return i;
		if((y1-val)*(y2-val)<0)
		{
			x = i-1 + (val-y1)/(y2-y1);
			a0 = a = mgl_data_spline_ext(d, x,0,0, &dx,&dy,&dz);
			unsigned k=0;
			if(spl)	while(fabs(a-val)>da || dx==0)
			{
				x += (val-a)/dx;		k++;
				a = mgl_data_spline_ext(d, x,0,0, &dx,&dy,&dz);
				if(k>=10)
					return fabs(a-val)<fabs(a0-val) ? x:i-1 + (val-y1)/(y2-y1);
			}
			return x;
		}
	}
	else 	for(long i=i0+1;i<nx;i++)
	{
		y1=d->v(i-1);	y2=d->v(i);
		if(val==y1)	return i-1;
		if(val==y2)	return i;
		if((y1-val)*(y2-val)<0)
			return i-1 + (val-y1)/(y2-y1);

	}
	return NAN;
}
//-----------------------------------------------------------------------------
mreal MGL_EXPORT mgl_data_linear_ext(HCDT d, mreal x,mreal y,mreal z, mreal *dx,mreal *dy,mreal *dz)
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
		b0 = x||y ? aa[0]*(1-x-y+x*y) + x*(1-y)*aa[1] + y*(1-x)*aa[dn] + x*y*aa[1+dn] : aa[0];
		b1 = x||y ? bb[0]*(1-x-y+x*y) + x*(1-y)*bb[1] + y*(1-x)*bb[dn] + x*y*bb[1+dn] : bb[0];
		if(dx)	*dx = aa[1]-aa[0];	if(dy)	*dy = aa[dn]-aa[0];	if(dz)	*dz = bb[0]-aa[0];
	}
	else
	{
		long n=d->GetNx(), ny=d->GetNy(), nz=d->GetNz();
		kx = (kx>=0 ? (kx<n ? kx:n -1):0);
		ky = (ky>=0 ? (ky<ny? ky:ny-1):0);
		kz = (kz>=0 ? (kz<nz? kz:nz-1):0);
		x -= kx;	y -= ky;	z -= kz;

		mreal a0 = d->v(kx,ky,kz), a1 = d->v(kx+1,ky,kz), a2 = d->v(kx,ky+1,kz);
		if(dx)	*dx = a1-a0;	if(dy)	*dy = a2-a0;	if(dz)	*dz = -a0;
		if(x||y)	b0 = a0*(1-x-y+x*y) + x*(1-y)*a1 + y*(1-x)*a2 + x*y*d->v(kx+1,ky+1,kz);
		else 	b0 = a0;
		kz++;
		a0 = d->v(kx,ky,kz); 	a1 = d->v(kx+1,ky,kz);	a2 = d->v(kx,ky+1,kz);
		if(dz)	*dz += a0;
		if(x||y)	b1 = a0*(1-x-y+x*y) + x*(1-y)*a1 + y*(1-x)*a2 + x*y*d->v(kx+1,ky+1,kz);
		else 	b1 = a0;
	}
	return b0 + z*(b1-b0);
}
//-----------------------------------------------------------------------------
mreal MGL_EXPORT mgl_data_linear(HCDT d, mreal x,mreal y,mreal z)
{	return mgl_data_linear_ext(d, x,y,z, 0,0,0);	}
//-----------------------------------------------------------------------------
mreal MGL_EXPORT mgl_data_spline(HCDT d, mreal x,mreal y,mreal z)
{
	const mglData *dd=dynamic_cast<const mglData *>(d);
	if(!d)	return 0;	// NOTE: don't support general arrays
	return mglSpline3(dd->a,dd->nx,dd->ny,dd->nz,x,y,z,0,0,0);
}
//-----------------------------------------------------------------------------
mreal MGL_EXPORT mgl_data_spline_ext(HCDT d, mreal x,mreal y,mreal z, mreal *dx,mreal *dy,mreal *dz)
{
	const mglData *dd=dynamic_cast<const mglData *>(d);
	if(!d)	return 0;	// NOTE: don't support general arrays
	return mglSpline3(dd->a,dd->nx,dd->ny,dd->nz,x,y,z,dx,dy,dz);
}
//-----------------------------------------------------------------------------
mreal MGL_EXPORT mgl_data_spline_(uintptr_t *d, mreal *x,mreal *y,mreal *z)
{	return mgl_data_spline(_DA_(d),*x,*y,*z);	}
mreal MGL_EXPORT mgl_data_linear_(uintptr_t *d, mreal *x,mreal *y,mreal *z)
{	return mgl_data_linear(_DA_(d),*x,*y,*z);	}
mreal MGL_EXPORT mgl_data_spline_ext_(uintptr_t *d, mreal *x,mreal *y,mreal *z, mreal *dx,mreal *dy,mreal *dz)
{	return mgl_data_spline_ext(_DA_(d),*x,*y,*z,dx,dy,dz);	}
mreal MGL_EXPORT mgl_data_linear_ext_(uintptr_t *d, mreal *x,mreal *y,mreal *z, mreal *dx,mreal *dy,mreal *dz)
{	return mgl_data_linear_ext(_DA_(d),*x,*y,*z,dx,dy,dz);	}
mreal MGL_EXPORT mgl_data_solve_1d_(uintptr_t *d, mreal *val, int *spl, int *i0)
{	return mgl_data_solve_1d(_DA_(d),*val, *spl, *i0);	}
uintptr_t MGL_EXPORT mgl_data_solve_(uintptr_t *d, mreal *val, const char *dir, uintptr_t *i0, int *norm,int)
{	return uintptr_t(mgl_data_solve(_DA_(d),*val, *dir, _DA_(i0), *norm));	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_crop(HMDT d, long n1, long n2, char dir)
{
	long nx=d->nx,ny=d->ny,nz=d->nz, nn;
	mreal *b;
	if(n1<0)	n1=0;
	switch(dir)
	{
	case 'x':
		n2 = n2>0 ? n2 : nx+n2;
		if(n2<0 || n2>=nx || n2<n1)	n2 = nx;
		nn = n2-n1;	b = new mreal[nn*ny*nz];
#pragma omp parallel for
		for(long i=0;i<ny*nz;i++)
			memcpy(b+nn*i,d->a+nx*i+n1,nn*sizeof(mreal));
		d->nx = nn;	if(!d->link)	delete []d->a;
		d->a = b;	d->link=false;	d->NewId();
		break;
	case 'y':
		n2 = n2>0 ? n2 : ny+n2;
		if(n2<0 || n2>=ny || n2<n1)	n2 = ny;
		nn = n2-n1;	b = new mreal[nn*nx*nz];
#pragma omp parallel for
		for(long i=0;i<nn;i++)	for(long j=0;j<nz;j++)
			memcpy(b+nx*(i+nn*j),d->a+nx*(n1+i+ny*j),nx*sizeof(mreal));
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
void MGL_EXPORT mgl_data_crop_(uintptr_t *d, int *n1, int *n2, const char *dir,int)
{	mgl_data_crop(_DT_,*n1,*n2,*dir);	}
//-----------------------------------------------------------------------------
mreal MGL_EXPORT mgl_data_last(HCDT d, const char *cond, long *i, long *j, long *k)
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
mreal MGL_EXPORT mgl_data_last_(uintptr_t *d, const char *cond, int *i, int *j, int *k, int l)
{	long ii=*i,jj=*j,kk=*k;	char *s=new char[l+1];	memcpy(s,cond,l);	s[l]=0;
	mreal res = mgl_data_last(_DT_,s,&ii,&jj,&kk);	*i=ii;	*j=jj;	*k=kk;
	delete []s;		return res;	}
//-----------------------------------------------------------------------------
mreal MGL_EXPORT mgl_data_first(HCDT d, const char *cond, long *i, long *j, long *k)
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
mreal MGL_EXPORT mgl_data_first_(uintptr_t *d, const char *cond, int *i, int *j, int *k, int l)
{	long ii=*i,jj=*j,kk=*k;	char *s=new char[l+1];	memcpy(s,cond,l);	s[l]=0;
	mreal res = mgl_data_first(_DT_,s,&ii,&jj,&kk);	*i=ii;	*j=jj;	*k=kk;
	delete []s;		return res;	}
//-----------------------------------------------------------------------------
long MGL_EXPORT mgl_data_find(HCDT d, const char *cond, char dir, long i, long j, long k)
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
int MGL_EXPORT mgl_data_find_(uintptr_t *d, const char *cond, char *dir, int *i, int *j, int *k, int l, int)
{	char *s=new char[l+1];	memcpy(s,cond,l);	s[l]=0;
	int res = mgl_data_find(_DT_,s,*dir,*i,*j,*k);	delete []s;	return res;	}
//-----------------------------------------------------------------------------
int MGL_EXPORT mgl_data_find_any(HCDT d, const char *cond)
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
int MGL_EXPORT mgl_data_find_any_(uintptr_t *d, const char *cond, int l)
{	char *s=new char[l+1];	memcpy(s,cond,l);	s[l]=0;
	int res = mgl_data_find_any(_DT_,s);	delete []s;	return res;	}
//-----------------------------------------------------------------------------
mreal MGL_EXPORT mgl_data_momentum_val(HCDT dd, char dir, mreal *x, mreal *w, mreal *s, mreal *k)
{
	long nx=dd->GetNx(),ny=dd->GetNy(),nz=dd->GetNz();
	mreal i0=0,i1=0,i2=0,i3=0,i4=0;
	const mglData *md = dynamic_cast<const mglData *>(dd);
	if(dd)	switch(dir)
	{
	case 'x':
#pragma omp parallel for reduction(+:i0,i1,i2,i3,i4)
		for(long i=0;i<nx*ny*nz;i++)
		{
			register mreal d = i%nx, t = d*d, v = md->a[i];
			i0+= v;	i1+= v*d;	i2+= v*t;
			i3+= v*d*t;		i4+= v*t*t;
		}
		break;
	case 'y':
#pragma omp parallel for reduction(+:i0,i1,i2,i3,i4)
		for(long i=0;i<nx*ny*nz;i++)
		{
			register mreal d = (i/nx)%ny, t = d*d, v = md->a[i];
			i0+= v;	i1+= v*d;	i2+= v*t;
			i3+= v*d*t;		i4+= v*t*t;
		}
		break;
	case 'z':
#pragma omp parallel for reduction(+:i0,i1,i2,i3,i4)
		for(long i=0;i<nx*ny*nz;i++)
		{
			register mreal d = i/(nx*ny), t = d*d, v = md->a[i];
			i0+= v;	i1+= v*d;	i2+= v*t;
			i3+= v*d*t;		i4+= v*t*t;
		}
		break;
	default:	// "self-dispersion"
		i0 = nx*ny*nz;
#pragma omp parallel for reduction(+:i1,i2,i3,i4)
		for(long i=0;i<nx*ny*nz;i++)
		{
			register mreal v=md->a[i], t  = v*v;
			i1+= v;			i2+= t;
			i3+= v*t;		i4+= t*t;
		}
	}
	else	switch(dir)
	{
	case 'x':
#pragma omp parallel for reduction(+:i0,i1,i2,i3,i4)
		for(long i=0;i<nx*ny*nz;i++)
		{
			register mreal d = i%nx, t = d*d, v = dd->vthr(i);
			i0+= v;	i1+= v*d;	i2+= v*t;
			i3+= v*d*t;		i4+= v*t*t;
		}
		break;
	case 'y':
#pragma omp parallel for reduction(+:i0,i1,i2,i3,i4)
		for(long i=0;i<nx*ny*nz;i++)
		{
			register mreal d = (i/nx)%ny, t = d*d, v = dd->vthr(i);
			i0+= v;	i1+= v*d;	i2+= v*t;
			i3+= v*d*t;		i4+= v*t*t;
		}
		break;
	case 'z':
#pragma omp parallel for reduction(+:i0,i1,i2,i3,i4)
		for(long i=0;i<nx*ny*nz;i++)
		{
			register mreal d = i/(nx*ny), t = d*d, v = dd->vthr(i);
			i0+= v;	i1+= v*d;	i2+= v*t;
			i3+= v*d*t;		i4+= v*t*t;
		}
		break;
	default:	// "self-dispersion"
		i0 = nx*ny*nz;
#pragma omp parallel for reduction(+:i1,i2,i3,i4)
		for(long i=0;i<nx*ny*nz;i++)
		{
			register mreal v = dd->vthr(i), t = v*v;
			i1+= v;			i2+= t;
			i3+= v*t;		i4+= t*t;
		}
	}
	if(i0==0)	return 0;
	register mreal d=i1/i0;
	if(x)	*x=d;
	if(w)	*w=i2>d*d*i0 ? sqrt(i2/i0-d*d) : 0;
	if(s)	*s=i3/i0;
	if(k)	*k=i4/(i0*3);
	return i0;
}
mreal MGL_EXPORT mgl_data_momentum_val_(uintptr_t *d, char *dir, mreal *m, mreal *w, mreal *s, mreal *k,int)
{	mreal mm,ww,ss,kk,aa;
	aa = mgl_data_momentum_val(_DT_,*dir,&mm,&ww,&ss,&kk);
	*m=mm;	*w=ww;	*s=ss;	*k=kk;	return aa;	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_norm_slice(HMDT d, mreal v1,mreal v2,char dir,long keep_en,long sym)
{
	long nx=d->nx, ny=d->ny, nz=d->nz;
	mreal *a=d->a;
	mglData b(*d);
	register mreal e0=1, e, m1, m2, aa;
	if(sym)	{	v2 = -v1>v2 ? -v1:v2;	v1 = -v2;	}
	if(dir=='z' && nz>1)
	{
//#pragma omp parallel for private(m1,m2,aa,e,e0)	// TODO add omp comparison here
		for(long k=0;k<nz;k++)
		{
			m1 = 1e20;	m2 = -1e20;	e=0;
			for(long i=0;i<nx*ny;i++)
			{
				aa = a[i+nx*ny*k];
				m1 = m1<aa ? m1 : aa;
				m2 = m2>aa ? m2 : aa;
				e += aa*aa;
			}
			if(m1==m2)	m2+=1;
			if(sym)	{	m2 = -m1>m2 ? -m1:m2;	m1 = -m2;	}
			if(keep_en && k)	e = e0>0?sqrt(e/e0):1;
			else	{	e0 = e;	e=1;	}
			for(long i=0;i<nx*ny;i++)
				b.a[i+nx*ny*k] = (v1 + (v2-v1)*(a[i+nx*ny*k]-m1)/(m2-m1))*e;
		}
	}
	else if(dir=='y' && ny>1)
	{
//#pragma omp parallel for private(m1,m2,aa,e,e0)	// TODO add omp comparison here
		for(long j=0;j<ny;j++)
		{
			m1 = 1e20;	m2 = -1e20;	e=0;
			for(long k=0;k<nz;k++)	for(long i=0;i<nx;i++)
			{
				aa = a[i+nx*(j+ny*k)];
				m1 = m1<aa ? m1 : aa;
				m2 = m2>aa ? m2 : aa;
				e += aa*aa;
			}
			if(m1==m2)	m2+=1;
			if(sym)	{	m2 = -m1>m2 ? -m1:m2;	m1 = -m2;	}
			if(keep_en && j)	e = e0>0?sqrt(e/e0):1;
			else	{	e0 = e;	e=1;	}
#pragma omp parallel for collapse(2)
			for(long k=0;k<nz;k++)	for(long i=0;i<nx;i++)
				b.a[i+nx*(j+ny*k)] = (v1 + (v2-v1)*(a[i+nx*(j+ny*k)]-m1)/(m2-m1))*e;
		}
	}
	else if(dir=='x' && nx>1)
	{
//#pragma omp parallel for private(m1,m2,aa,e,e0)	// TODO add omp comparison here
		for(long i=0;i<nx;i++)
		{
			m1 = 1e20;	m2 = -1e20;	e=0;
			for(long k=0;k<ny*nz;k++)
			{
				aa = a[i+nx*k];
				m1 = m1<aa ? m1 : aa;
				m2 = m2>aa ? m2 : aa;
				e += aa*aa;
			}
			if(m1==m2)	m2+=1;
			if(sym)	{	m2 = -m1>m2 ? -m1:m2;	m1 = -m2;	}
			if(keep_en && i)	e = e0>0?sqrt(e/e0):1;
			else	{	e0 = e;	e=1;	}
#pragma omp parallel for
			for(long k=0;k<ny*nz;k++)
				b.a[i+nx*k] = (v1 + (v2-v1)*(a[i+nx*k]-m1)/(m2-m1))*e;
		}
	}
	memcpy(d->a, b.a, nx*ny*nz*sizeof(mreal));
}
void MGL_EXPORT mgl_data_norm_slice_(uintptr_t *d, mreal *v1,mreal *v2,char *dir,int *keep_en,int *sym,int )
{	mgl_data_norm_slice(_DT_,*v1,*v2,*dir,*keep_en,*sym);	}
//-----------------------------------------------------------------------------
MGL_EXPORT const char *mgl_data_info(HCDT d)	// NOTE: Not thread safe function!
{
	static char buf[512];
	char s[128];	buf[0]=0;
	snprintf(s,128,"nx = %ld\tny = %ld\tnz = %ld\n",d->GetNx(),d->GetNy(),d->GetNz());	strcat(buf,s);

	long i=0,j=0,k=0;
	mreal A=0,Wa=0,X=0,Y=0,Z=0,Wx=0,Wy=0,Wz=0, b;
	b = mgl_data_max_int(d,&i,&j,&k);
	snprintf(s,128,"Maximum is %g\t at x = %ld\ty = %ld\tz = %ld\n", b,i,j,k);	strcat(buf,s);
	b = mgl_data_min_int(d,&i,&j,&k);
	snprintf(s,128,"Minimum is %g\t at x = %ld\ty = %ld\tz = %ld\n", b,i,j,k);	strcat(buf,s);

	mgl_data_momentum_val(d,'a',&A,&Wa,0,0);	mgl_data_momentum_val(d,'x',&X,&Wx,0,0);
	mgl_data_momentum_val(d,'y',&Y,&Wy,0,0);	mgl_data_momentum_val(d,'z',&Z,&Wz,0,0);
	snprintf(s,128,"Averages are:\n<a> = %g\t<x> = %g\t<y> = %g\t<z> = %g\n", A,X,Y,Z);	strcat(buf,s);
	snprintf(s,128,"Widths are:\nWa = %g\tWx = %g\tWy = %g\tWz = %g\n", Wa,Wx,Wy,Wz);	strcat(buf,s);
	return buf;
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_insert(HMDT d, char dir, long at, long num)
{
	if(num<1)	return;
	at = at<0 ? 0:at;
	long nn, nx=d->nx, ny=d->ny, nz=d->nz;
	mglData b;
	if(dir=='x')
	{
		if(at>nx)	at=nx;
		nn=nx+num;	b.Create(nn,ny,nz);
#pragma omp parallel for
		for(long k=0;k<ny*nz;k++)
		{
			if(at>0)	memcpy(b.a+nn*k, d->a+nx*k,at*sizeof(mreal));
			if(at<nx)	memcpy(b.a+at+num+nn*k, d->a+at+nx*k,(nx-at)*sizeof(mreal));
			for(long i=0;i<num;i++)	b.a[nn*k+at+i]=d->a[nx*k+at];	// copy values
		}
		d->Set(b);	nx+=num;
	}
	if(dir=='y')
	{
		if(at>ny)	at=ny;
		nn=num+ny;	b.Create(nx,nn,nz);
#pragma omp parallel for
		for(long k=0;k<nz;k++)
		{
			if(at>0)	memcpy(b.a+nx*nn*k, d->a+nx*ny*k,at*nx*sizeof(mreal));
			if(at<ny)	memcpy(b.a+nx*(at+num+nn*k), d->a+nx*(at+ny*k),(ny-at)*nx*sizeof(mreal));
			for(long i=0;i<num;i++)	memcpy(b.a+nx*(nn*k+at+i),d->a+nx*(ny*k+at),nx*sizeof(mreal));
		}
		d->Set(b);	ny+=num;
	}
	if(dir=='z')
	{
		if(at>nz)	at=nz;
		b.Create(nx,ny,nz+num);
		if(at>0)	memcpy(b.a, d->a,at*nx*ny*sizeof(mreal));
		if(at<nz)	memcpy(b.a+nx*ny*(at+num), d->a+nx*ny*at,(nz-at)*nx*ny*sizeof(mreal));
#pragma omp parallel for
		for(long i=0;i<num;i++)	memcpy(b.a+nx*ny*(at+i),d->a+nx*ny*at,nx*ny*sizeof(mreal));
		d->Set(b);	nz+=num;
	}
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_delete(HMDT d, char dir, long at, long num)
{
	if(num<1 || at<0)	return;
	mglData b;
	long nx=d->nx, ny=d->ny, nz=d->nz, nn;
	if(dir=='x')
	{
		if(at+num>=nx)	return;
		nn=nx-num;	b.Create(nn,ny,nz);
#pragma omp parallel for
		for(long k=0;k<ny*nz;k++)
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
#pragma omp parallel for
		for(long k=0;k<nz;k++)
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
void MGL_EXPORT mgl_data_insert_(uintptr_t *d, const char *dir, int *at, int *num, int)
{	mgl_data_insert(_DT_,*dir,*at,*num);	}
void MGL_EXPORT mgl_data_delete_(uintptr_t *d, const char *dir, int *at, int *num, int)
{	mgl_data_delete(_DT_,*dir,*at,*num);	}
//-----------------------------------------------------------------------------
mreal MGL_EXPORT mgl_spline5(mreal y1[5], mreal y2[5], long n1, long n2, mreal d, mreal &dy)
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
mreal MGL_EXPORT mgl_spline3(mreal y1[3], mreal y2[3], long n1, long n2, mreal d, mreal &dy)
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
void MGL_NO_EXPORT mgl_omod(mreal *a, mreal da, int nx, int n)
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
MGL_NO_EXPORT void *mgl_sew_z(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nz=t->p[2], nn=t->n;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for
#endif
	for(long i=t->id;i<nn;i+=mglNumThr)	mgl_omod(t->a+i, t->b[0], nz, nn);
	return 0;
}
MGL_NO_EXPORT void *mgl_sew_y(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nx=t->p[0], ny=t->p[1], nn=t->n;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for
#endif
	for(long i=t->id;i<nn;i+=mglNumThr)	mgl_omod(t->a+(i%nx)+nx*ny*(i/nx), t->b[0], ny, nx);
	return 0;
}
MGL_NO_EXPORT void *mgl_sew_x(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nx=t->p[0], nn=t->n;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for
#endif
	for(long i=t->id;i<nn;i+=mglNumThr)	mgl_omod(t->a+i*nx, t->b[0], nx, 1);
	return 0;
}
void MGL_EXPORT mgl_data_sew(HMDT d, const char *dirs, mreal delta)
{
	if(!dirs || *dirs==0)	return;
	long nx=d->nx, ny=d->ny, nz=d->nz;
	long p[3]={nx,ny,nz};
	mreal da = delta;
	if(strchr(dirs,'x') && nx>1)	mglStartThread(mgl_sew_x,0,nz*ny,d->a,&da,0,p);
	if(strchr(dirs,'y') && ny>1)	mglStartThread(mgl_sew_y,0,nz*nx,d->a,&da,0,p);
	if(strchr(dirs,'z') && nz>1)	mglStartThread(mgl_sew_z,0,nx*ny,d->a,&da,0,p);
}
void MGL_EXPORT mgl_data_sew_(uintptr_t *d, const char *dirs, mreal *da, int l)
{	char *s=new char[l+1];	memcpy(s,dirs,l);	s[l]=0;
	mgl_data_sew(_DT_,s,*da);	delete []s;	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_put_val(HMDT d, mreal val, long xx, long yy, long zz)
{
	register long nx=d->nx, ny=d->ny, nz=d->nz;
	if(xx>=nx || yy>=ny || zz>=nz)	return;
	mreal *a=d->a;
	if(xx<0 && yy<0 && zz<0)
#pragma omp parallel for
		for(long i=0;i<nx*ny*nz;i++)	a[i] = val;
	else if(xx<0 && yy<0)
#pragma omp parallel for
		for(long i=0;i<nx*ny;i++)	a[i+zz*nx*ny] = val;
	else if(yy<0 && zz<0)
#pragma omp parallel for
		for(long i=0;i<nz*ny;i++)	a[xx+i*nx] = val;
	else if(xx<0 && zz<0)
#pragma omp parallel for collapse(2)
		for(long i=0;i<nx;i++)	for(long j=0;j<nz;j++)	a[i+nx*(yy+j*ny)] = val;
	else if(xx<0)
#pragma omp parallel for
		for(long i=0;i<nx;i++)	a[i+nx*(yy+zz*ny)] = val;
	else if(yy<0)
#pragma omp parallel for
		for(long i=0;i<ny;i++)	a[xx+nx*(i+zz*ny)] = val;
	else if(zz<0)
#pragma omp parallel for
		for(long i=0;i<nz;i++)	a[xx+nx*(yy+i*ny)] = val;
	else	a[xx+nx*(yy+zz*ny)] = val;
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_put_dat(HMDT d, HCDT v, long xx, long yy, long zz)
{
	register long nx=d->nx, ny=d->ny, nz=d->nz;
	if(xx>=nx || yy>=ny || zz>=nz)	return;
	const mglData *mv = dynamic_cast<const mglData *>(v);
	mreal *a=d->a, vv=v->v(0);
	const mreal *b = mv?mv->a:0;
	long vx=v->GetNx(), vy=v->GetNy(), vz=v->GetNz();
	if(xx<0 && yy<0 && zz<0)	// whole array
	{
		if(vx>=nx && vy>=ny && vz>=nz)
#pragma omp parallel for
			for(long ii=0;ii<nx*ny*nz;ii++)
			{	register long i=ii%nx, j=(ii/nx)%ny, k=ii/(nx*ny);
				a[ii] = b?b[i+vx*(j+k*vy)]:v->v(i,j,k);	}
		else if(vx>=nx && vy>=ny)
#pragma omp parallel for
			for(long ii=0;ii<nx*ny*nz;ii++)
			{	register long i=ii%nx, j=(ii/nx)%ny;
				a[ii] = b?b[i+vx*j]:v->v(i,j);	}
		else if(vx>=nx)
#pragma omp parallel for
			for(long ii=0;ii<nx*ny*nz;ii++)
			{	register long i=ii%nx;	a[ii] = b?b[i]:v->v(i);	}
		else
#pragma omp parallel for
			for(long ii=0;ii<nx*ny*nz;ii++)	a[ii] = vv;
	}
	else if(xx<0 && yy<0)	// 2d
	{
		zz*=nx*ny;
		if(vx>=nx && vy>=ny)
#pragma omp parallel for
			for(long ii=0;ii<nx*ny;ii++)
			{	register long i=ii%nx, j=ii/nx;
				a[ii+zz] = b?b[i+vx*j]:v->v(i,j);	}
		else if(vx>=nx)
#pragma omp parallel for
			for(long ii=0;ii<nx*ny;ii++)
			{	register long i=ii%nx;	a[ii+zz] = b?b[i]:v->v(i);	}
		else
#pragma omp parallel for
			for(long ii=0;ii<nx*ny;ii++) 	a[ii+zz] = vv;
	}
	else if(yy<0 && zz<0)	// 2d
	{
		if(vx>=ny && vy>=nz)
#pragma omp parallel for
			for(long ii=0;ii<ny*nz;ii++)
			{	register long i=ii%ny, j=ii/ny;
				a[ii*nx+xx] = b?b[i+vx*j]:v->v(i,j);	}
		else if(vx>=ny)
#pragma omp parallel for
			for(long ii=0;ii<ny*nz;ii++)
			{	register long i=ii%ny;	a[ii*nx+xx] = b?b[i]:v->v(i);	}
		else
#pragma omp parallel for
			for(long ii=0;ii<ny*nz;ii++) 	a[ii*nx+xx] = vv;
	}
	else if(xx<0 && zz<0)	// 2d
	{
		yy *= nx;	zz = nx*ny;
		if(vx>=nx && vy>=nz)
#pragma omp parallel for
			for(long ii=0;ii<nx*nz;ii++)
			{	register long i=ii%nx, j=ii/nx;
				a[i+yy+j*zz] = b?b[i+vx*j]:v->v(i,j);	}
		else if(vx>=nx)
#pragma omp parallel for
			for(long ii=0;ii<nx*nz;ii++)
			{	register long i=ii%nx, j=ii/nx;
				a[i+yy+j*zz] = b?b[i]:v->v(i);	}
		else
#pragma omp parallel for
			for(long ii=0;ii<nx*nz;ii++)
			{	register long i=ii%nx, j=ii/nx;
				a[i+yy+j*zz] = vv;	}
	}
	else if(xx<0)
	{
		xx = nx*(yy+zz*ny);
		if(vx>=nx)
#pragma omp parallel for
			for(long i=0;i<nx;i++)	a[i+xx] = b?b[i]:v->v(i);
		else
#pragma omp parallel for
			for(long i=0;i<nx;i++)	a[i+xx] = vv;
	}
	else if(yy<0)
	{
		xx += zz*nx*ny;
		if(vx>=ny)
#pragma omp parallel for
			for(long i=0;i<ny;i++)	a[xx+nx*i] = b?b[i]:v->v(i);
		else
#pragma omp parallel for
			for(long i=0;i<ny;i++)	a[xx+nx*i] = vv;
	}
	else if(zz<0)
	{
		xx += nx*yy;	yy = nx*ny;
		if(vx>=nz)
#pragma omp parallel for
			for(long i=0;i<nz;i++)	a[xx+yy*i] = b?b[i]:v->v(i);
		else
#pragma omp parallel for
			for(long i=0;i<nz;i++)	a[xx+yy*i] = vv;
	}
	else	a[xx+nx*(yy+ny*zz)] = vv;
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_put_val_(uintptr_t *d, mreal *val, int *i, int *j, int *k)
{	mgl_data_put_val(_DT_,*val, *i,*j,*k);	}
void MGL_EXPORT mgl_data_put_dat_(uintptr_t *d, uintptr_t *val, int *i, int *j, int *k)
{	mgl_data_put_dat(_DT_,_DA_(val), *i,*j,*k);	}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void *mgl_diff_3(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nx=t->p[0], ny=t->p[1], nz=t->p[2], nn=t->n;
	mreal *b=t->a,au,av,aw,xu,xv,xw,yu,yv,yw,zu,zv,zw;
	const mreal *a=t->b, *x=t->c, *y=t->d, *z=t->e;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for private(au,av,aw,xu,xv,xw,yu,yv,yw,zu,zv,zw)
#endif
	for(long i0=t->id;i0<nn;i0+=mglNumThr)
	{
		register long i=i0%nx, j=((i0/nx)%ny), k=i0/(nx*ny);
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
MGL_NO_EXPORT void *mgl_diff_2(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long nx=t->p[0], ny=t->p[1], nn=t->n, same=t->p[2];
	mreal *b=t->a,au,av,xu,xv,yu,yv;
	const mreal *a=t->b, *x=t->c, *y=t->d;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for private(au,av,xu,xv,yu,yv)
#endif
	for(long i0=t->id;i0<nn;i0+=mglNumThr)
	{
		register long i=i0%nx, j=((i0/nx)%ny), i1 = same ? i0 : i0%(nx*ny);
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
MGL_NO_EXPORT void *mgl_diff_1(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	long nx=t->p[0], nn=t->n, same=t->p[1];
	mreal *b=t->a,au,xu;
	const mreal *a=t->b, *x=t->c;
#if !MGL_HAVE_PTHREAD
#pragma omp parallel for private(au,xu)
#endif
	for(long i0=t->id;i0<nn;i0+=mglNumThr)
	{
		register long i=i0%nx, i1 = same ? i0 : i;
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
void MGL_EXPORT mgl_data_diff_par(HMDT d, HCDT v1, HCDT v2, HCDT v3)
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
void MGL_EXPORT mgl_data_diff_par_(uintptr_t *d, uintptr_t *v1, uintptr_t *v2, uintptr_t *v3)
{	mgl_data_diff_par(_DT_,_DA_(v1),_DA_(v2),_DA_(v3));	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_set_value(HMDT dat, mreal v, long i, long j, long k)
{	if(i>=0 && i<dat->nx && j>=0 && j<dat->ny && k>=0 && k<dat->nz)	dat->a[i+dat->nx*(j+dat->ny*k)]=v;	}
void MGL_EXPORT mgl_data_set_value_(uintptr_t *d, mreal *v, int *i, int *j, int *k)
{	mgl_data_set_value(_DT_,*v,*i,*j,*k);	}
//-----------------------------------------------------------------------------
mreal MGL_EXPORT mgl_data_get_value(HCDT dat, long i, long j, long k)
{	return (i>=0 && i<dat->GetNx() && j>=0 && j<dat->GetNy() && k>=0 && k<dat->GetNz()) ? dat->v(i,j,k):NAN;	}
mreal MGL_EXPORT mgl_data_get_value_(uintptr_t *d, int *i, int *j, int *k)
{	return mgl_data_get_value(_DA_(d),*i,*j,*k);	}
//-----------------------------------------------------------------------------
MGL_EXPORT mreal *mgl_data_data(HMDT dat)	{	return dat->a;	}
//-----------------------------------------------------------------------------
MGL_EXPORT mreal *mgl_data_value(HMDT dat, long i,long j,long k)
{	register long ii=i*dat->nx*(j+dat->ny*k);
	return	ii>=0 && ii<dat->GetNN() ? dat->a+ii : 0;	}
//-----------------------------------------------------------------------------
long MGL_EXPORT mgl_data_get_nx(HCDT dat)	{	return dat->GetNx();	}
long MGL_EXPORT mgl_data_get_ny(HCDT dat)	{	return dat->GetNy();	}
long MGL_EXPORT mgl_data_get_nz(HCDT dat)	{	return dat->GetNz();	}
long MGL_EXPORT mgl_data_get_nx_(uintptr_t *d)	{	return _DA_(d)->GetNx();	}
long MGL_EXPORT mgl_data_get_ny_(uintptr_t *d)	{	return _DA_(d)->GetNy();	}
long MGL_EXPORT mgl_data_get_nz_(uintptr_t *d)	{	return _DA_(d)->GetNz();	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_join(HMDT d, HCDT v)
{
	long nx=d->nx, ny=d->ny, nz=d->nz, k=nx*ny*nz;
	const mglData *mv = dynamic_cast<const mglData *>(v);
	long vx=v->GetNx(), vy=v->GetNy(), vz=v->GetNz();

	if(nx==vx && ny==vy && (nz>1 || vz>1))
	{
		mreal *b = new mreal[nx*ny*(nz+vz)];
		memcpy(b,d->a,nx*ny*nz*sizeof(mreal));
		if(mv)	memcpy(b+nx*ny*nz,mv->a,nx*ny*vz*sizeof(mreal));
		else
#pragma omp parallel for
			for(long i=0;i<nx*ny*vz;i++)	b[k+i] = v->vthr(i);
		if(!d->link)	delete []d->a;	d->nz += vz;
		d->a = b;	d->link=false;	d->NewId();
	}
	else if(nx==vx && (ny>1 || vy>1))
	{
		ny *= nz;	vy *= vz;
		mreal *b = new mreal[nx*(ny+vy)];
		memcpy(b,d->a,nx*ny*sizeof(mreal));
		if(mv)	memcpy(b+nx*ny,mv->a,nx*vy*sizeof(mreal));
		else
#pragma omp parallel for
			for(long i=0;i<nx*vy;i++)	b[k+i] = v->vthr(i);
		if(!d->link)	delete []d->a;
		d->nz = 1;	d->ny = ny+vy;
		d->a = b;	d->link=false;	d->NewId();
	}
	else
	{
		nx *= ny*nz;	vx *= vy*vz;
		mreal *b = new mreal[nx+vx];
		memcpy(b,d->a,nx*sizeof(mreal));
		if(mv)	memcpy(b+nx,mv->a,vx*sizeof(mreal));
		else
#pragma omp parallel for
			for(long i=0;i<vx;i++)	b[k+i] = v->vthr(i);
		if(!d->link)	delete []d->a;
		d->nz = d->ny = 1;	d->nx = nx+vx;
		d->a = b;	d->link=false;	d->NewId();
	}
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_join_(uintptr_t *d, uintptr_t *val)
{	mgl_data_join(_DT_,_DA_(val));	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_refill_x(HMDT dat, HCDT xdat, HCDT vdat, mreal x1, mreal x2, long sl)
{
	long nx=dat->nx,mx=vdat->GetNx(),nn=dat->ny*dat->nz;
	mreal acx=1e-6*fabs(x2-x1);
	if(mx!=xdat->GetNx())	return;	// incompatible dimensions
#pragma omp parallel for
	for(long i=0;i<nx;i++)
	{
		mreal xx = x1+(x2-x1)*i/(nx-1.),d,val,dx=0,t1,t2;
		val = mgl_data_spline_ext(xdat,dx,0,0,&d,0,0);
		do	// use Newton method to find root
		{
			dx += (xx-val)/d;
			val = mgl_data_spline_ext(xdat,dx,0,0,&d,0,0);
		}	while(fabs(xx-val)>acx);
		d = mgl_data_spline(vdat,dx,0,0);
		if(sl<0)	for(long j=0;j<nn;j++)	dat->a[i+j*nx] = d;
		else	dat->a[i+sl*nx] = d;
	}
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_refill_xy(HMDT dat, HCDT xdat, HCDT ydat, HCDT vdat, mreal x1, mreal x2, mreal y1, mreal y2, long sl)
{
	long nx=dat->nx,ny=dat->ny,nz=dat->nz,mx=vdat->GetNx(),my=vdat->GetNy(),nn=nx*ny;
	bool both=(xdat->GetNN()==vdat->GetNN() && ydat->GetNN()==vdat->GetNN());
	if(!both && (xdat->GetNx()!=mx || ydat->GetNx()!=my))	return;	// incompatible dimensions
	mreal acx=1e-6*fabs(x2-x1), acy=1e-6*fabs(y2-y1);
	if(both)
#pragma omp parallel for collapse(2)
		for(long j=0;j<ny;j++)	for(long i=0;i<nx;i++)
		{
			mreal xx = x1+(x2-x1)*i/(nx-1.),dxx,dxy,vx,dx=0,dd;
			mreal yy = y1+(y2-y1)*j/(ny-1.),dyx,dyy,vy,dy=0;
			vx = mgl_data_spline_ext(xdat,dx,dy,0,&dxx,&dxy,0);
			vy = mgl_data_spline_ext(ydat,dx,dy,0,&dyx,&dyy,0);
			do	// use Newton method to find root
			{
				dd = dxy*dyx-dxx*dyy;
				dx += (dxy*(yy-vy)-dyy*(xx-vx))/dd;
				dy += (dyx*(xx-vx)-dxx*(yy-vy))/dd;
				vx = mgl_data_spline_ext(xdat,dx,dy,0,&dxx,&dxy,0);
				vy = mgl_data_spline_ext(ydat,dx,dy,0,&dyx,&dyy,0);
			}	while(fabs(xx-vx)>acx && fabs(yy-vy)>acy);	// this valid for linear interpolation
			dd = mgl_data_spline(vdat,dx,dy,0);
			register long i0=i+nx*j;
			if(sl<0)	for(long k=0;k<nz;k++)	dat->a[i0+k*nn] = dd;
			else	dat->a[i0+sl*nn] = dd;
		}
	else
#pragma omp parallel for collapse(2)
		for(long j=0;j<ny;j++)	for(long i=0;i<nx;i++)
		{
			mreal xx = x1+(x2-x1)*i/(nx-1.),d,val,dx=0;
			mreal yy = y1+(y2-y1)*j/(ny-1.),dy=0;
			val = mgl_data_spline_ext(xdat,dx,0,0,&d,0,0);
			do	// use Newton method to find root
			{
				dx += (xx-val)/d;
				val = mgl_data_spline_ext(xdat,dx,0,0,&d,0,0);
			}	while(fabs(xx-val)>acx);	// this valid for linear interpolation
			val = mgl_data_spline_ext(ydat,dy,0,0,&d,0,0);
			do	// use Newton method to find root
			{
				dy = (yy-val)/d;
				val = mgl_data_spline_ext(ydat,dy,0,0,&d,0,0);
			}	while(fabs(yy-val)>acy);	// this valid for linear interpolation
			d = mgl_data_spline(vdat,dx,dy,0);
			register long i0=i+nx*j;
			if(sl<0)	for(long k=0;k<nz;k++)	dat->a[i0+k*nn] = d;
			else	dat->a[i0+sl*nn] = d;
		}
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_refill_xyz(HMDT dat, HCDT xdat, HCDT ydat, HCDT zdat, HCDT vdat, mreal x1, mreal x2, mreal y1, mreal y2, mreal z1, mreal z2)
{
	long nx=dat->nx,ny=dat->ny,nz=dat->nz,mx=vdat->GetNx(),my=vdat->GetNy(),mz=vdat->GetNz();
	bool both=(xdat->GetNN()==vdat->GetNN() && ydat->GetNN()==vdat->GetNN() && zdat->GetNN()==vdat->GetNN());
	if(!both && (xdat->GetNx()!=mx || ydat->GetNx()!=my || zdat->GetNx()!=mz))	return;	// incompatible dimensions
	mreal acx=1e-6*fabs(x2-x1), acy=1e-6*fabs(y2-y1), acz=1e-6*fabs(z2-z1);
	if(both)
#pragma omp parallel for collapse(3)
		for(long k=0;k<nz;k++)	for(long j=0;j<ny;j++)	for(long i=0;i<nx;i++)
		{
			mreal xx = x1+(x2-x1)*i/(nx-1.),dxx,dxy,dxz,vx,dx=0,dd;
			mreal yy = y1+(y2-y1)*j/(ny-1.),dyx,dyy,dyz,vy,dy=0;
			mreal zz = z1+(z2-z1)*k/(nz-1.),dzx,dzy,dzz,vz,dz=0;
			vx = mgl_data_spline_ext(xdat,dx,dy,dz,&dxx,&dxy,&dxz);
			vy = mgl_data_spline_ext(ydat,dx,dy,dz,&dyx,&dyy,&dyz);
			vz = mgl_data_spline_ext(zdat,dx,dy,dz,&dzx,&dzy,&dzz);
			do	// use Newton method to find root
			{
				dd = -dxx*dyy*dzz+dxy*dyx*dzz+dxx*dyz*dzy-dxz*dyx*dzy-dxy*dyz*dzx+dxz*dyy*dzx;
				dx += ((dyz*dzy-dyy*dzz)*(xx-vx)+(dxy*dzz-dxz*dzy)*(yy-vy)+(dxz*dyy-dxy*dyz)*(zz-vz))/dd;
				dy += ((dyx*dzz-dyz*dzx)*(xx-vx)+(dxz*dzx-dxx*dzz)*(yy-vy)+(dxx*dyz-dxz*dyx)*(zz-vz))/dd;
				dz += ((dyy*dzx-dyx*dzy)*(xx-vx)+(dxx*dzy-dxy*dzx)*(yy-vy)+(dxy*dyx-dxx*dyy)*(zz-vz))/dd;
				vx = mgl_data_spline_ext(xdat,dx,dy,dz,&dxx,&dxy,&dxz);
				vy = mgl_data_spline_ext(ydat,dx,dy,dz,&dyx,&dyy,&dyz);
				vz = mgl_data_spline_ext(zdat,dx,dy,dz,&dzx,&dzy,&dzz);
			}	while(fabs(xx-vx)>acx && fabs(yy-vy)>acy && fabs(zz-vz)>acz);	// this valid for linear interpolation
			dat->a[i+nx*(j+ny*k)] = mgl_data_spline(vdat,dx,dy,dz);
		}
	else
#pragma omp parallel for collapse(3)
		for(long k=0;k<nz;k++)	for(long j=0;j<ny;j++)	for(long i=0;i<nx;i++)
		{
			mreal xx = x1+(x2-x1)*i/(nx-1.),d,val,dx=0;
			mreal yy = y1+(y2-y1)*j/(ny-1.),dy=0;
			mreal zz = z1+(z2-z1)*k/(nz-1.),dz=0;
			val = mgl_data_spline_ext(xdat,dx,0,0,&d,0,0);
			do	// use Newton method to find root
			{
				dx += (xx-val)/d;
				val = mgl_data_spline_ext(xdat,dx,0,0,&d,0,0);
			}	while(fabs(xx-val)>acx);	// this valid for linear interpolation
			val = mgl_data_spline_ext(ydat,dy,0,0,&d,0,0);
			do	// use Newton method to find root
			{
				dy = (yy-val)/d;
				val = mgl_data_spline_ext(ydat,dy,0,0,&d,0,0);
			}	while(fabs(yy-val)>acy);	// this valid for linear interpolation
			val = mgl_data_spline_ext(zdat,dz,0,0,&d,0,0);
			do	// use Newton method to find root
			{
				dz += (zz-val)/d;
				val = mgl_data_spline_ext(zdat,dz,0,0,&d,0,0);
			}	while(fabs(zz-val)>acz);	// this valid for linear interpolation
			dat->a[i+nx*(j+ny*k)] = mgl_data_spline(vdat,dx,dy,dz);
		}
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_refill_gr(HMDT dat, HMGL gr, HCDT xdat, HCDT ydat, HCDT zdat, HCDT vdat, long sl, const char *opt)
{
	if(!vdat)	return;
	gr->SaveState(opt);
	if(!ydat && !zdat)	mgl_data_refill_x(dat,xdat,vdat,gr->Min.x,gr->Max.x,sl);
//	else if(!xdat && !zdat)	mgl_data_refill_x(dat,ydat,vdat,gr->Min.y,gr->Max.y,sl);
//	else if(!xdat && !ydat)	mgl_data_refill_x(dat,zdat,vdat,gr->Min.z,gr->Max.z,sl);
	else if(!zdat)	mgl_data_refill_xy(dat,xdat,ydat,vdat,gr->Min.x,gr->Max.x,gr->Min.y,gr->Max.y,sl);
//	else if(!ydat)	mgl_data_refill_xy(dat,xdat,zdat,vdat,gr->Min.x,gr->Max.x,gr->Min.z,gr->Max.z,sl);
//	else if(!xdat)	mgl_data_refill_xy(dat,ydat,zdat,vdat,gr->Min.y,gr->Max.y,gr->Min.z,gr->Max.z,sl);
	else	mgl_data_refill_xyz(dat,xdat,ydat,zdat,vdat,gr->Min.x,gr->Max.x,gr->Min.y,gr->Max.y,gr->Min.z,gr->Max.z);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_refill_x_(uintptr_t *d, uintptr_t *xdat, uintptr_t *vdat, mreal *x1, mreal *x2, long *sl)
{	mgl_data_refill_x(_DT_,_DA_(xdat),_DA_(vdat),*x1,*x2,*sl);	}
void MGL_EXPORT mgl_data_refill_xy_(uintptr_t *d, uintptr_t *xdat, uintptr_t *ydat, uintptr_t *vdat, mreal *x1, mreal *x2, mreal *y1, mreal *y2, long *sl)
{	mgl_data_refill_xy(_DT_,_DA_(xdat),_DA_(ydat),_DA_(vdat),*x1,*x2,*y1,*y2,*sl);	}
void MGL_EXPORT mgl_data_refill_xyz_(uintptr_t *d, uintptr_t *xdat, uintptr_t *ydat, uintptr_t *zdat, uintptr_t *vdat, mreal *x1, mreal *x2, mreal *y1, mreal *y2, mreal *z1, mreal *z2)
{	mgl_data_refill_xyz(_DT_,_DA_(xdat),_DA_(ydat),_DA_(zdat),_DA_(vdat),*x1,*x2,*y1,*y2,*z1,*z2);	}
void MGL_EXPORT mgl_data_refill_gr_(uintptr_t *d, uintptr_t *gr, uintptr_t *xdat, uintptr_t *ydat, uintptr_t *zdat, uintptr_t *vdat, long *sl, const char *opt,int l)
{	char *s=new char[l+1];	memcpy(s,opt,l);	s[l]=0;
	mgl_data_refill_gr(_DT_,_GR_,_DA_(xdat),_DA_(ydat),_DA_(zdat),_DA_(vdat),*sl,s);	delete []s;	}
//-----------------------------------------------------------------------------
