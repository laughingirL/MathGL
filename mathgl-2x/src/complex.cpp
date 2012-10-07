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
#include "mgl2/datac.h"
#include "mgl2/evalc.h"

#if MGL_HAVE_GSL
#include <gsl/gsl_fft_complex.h>
#include <gsl/gsl_dht.h>
#include <gsl/gsl_sf.h>
#endif
//-----------------------------------------------------------------------------
void mglStartThreadC(void *(*func)(void *), void (*post)(mglThreadC *,dual *), long n,
					dual *a, const dual *b, const dual *c, const long *p,
					const void *v, const dual *d, const dual *e, const char *s)
{
	if(!func)	return;
#if MGL_HAVE_PTHREAD
	if(mglNumThr<1)	mgl_set_num_thr(0);
	if(mglNumThr>1)
	{
		pthread_t *tmp=new pthread_t[mglNumThr];
		mglThreadC *par=new mglThreadC[mglNumThr];
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
		mglThreadC par;
		par.n=n;	par.a=a;	par.b=b;	par.c=c;	par.d=d;
		par.p=p;	par.v=v;	par.s=s;	par.e=e;	par.id=0;
		func(&par);
		if(post)	post(&par,a);
	}
}
//-----------------------------------------------------------------------------
void mglStartThreadV(void *(*func)(void *), long n, dual *a, const void *b,
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
		{	par[i].n=n;	par[i].a=0;	par[i].b=b;	par[i].c=c;	par[i].d=d;
			par[i].p=p;	par[i].v=v;	par[i].id=i;par[i].aa=a;	}
		for(i=0;i<mglNumThr;i++)	pthread_create(tmp+i, 0, func, par+i);
		for(i=0;i<mglNumThr;i++)	pthread_join(tmp[i], 0);
		delete []tmp;	delete []par;
	}
	else
#endif
	{
		mglNumThr = 1;
		mglThreadV par;
		par.n=n;	par.a=0;	par.b=b;	par.c=c;	par.d=d;
		par.p=p;	par.v=v;	par.id=0;	par.aa=a;
		func(&par);
	}
}
//-----------------------------------------------------------------------------
void *mgl_csmth_x(void *par)
{
	mglThreadC *t=(mglThreadC *)par;
	register long i,j,d3,d5, nx=t->p[0];
	dual *b=t->a, y5,y3,x2y;
	const dual *a=t->b;
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
		x2y= (a[i+d5+1] + mgl4*a[i+d5+2] + mgl4*a[i+d5-2] + a[i+d5-1]);
		j = t->p[2];
		if(d3)	b[i] = a[i];
		else if(j==SMOOTH_LINE_3 || d5)	b[i] = y3/mgl3;
		else if(j==SMOOTH_LINE_5)		b[i] = y5/mreal(5);
		else if(j==SMOOTH_QUAD_5)		b[i] = (mreal(17)*y5-mreal(5)*x2y)/mreal(35);
	}
	return 0;
}
void *mgl_csmth_y(void *par)
{
	mglThreadC *t=(mglThreadC *)par;
	register long i,j,d3,d5, nx=t->p[0],ny=t->p[1];
	dual *b=t->a, y5,y3,x2y;
	const dual *a=t->b;
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
		x2y= (a[i+nx*(d5+1)] + mgl4*a[i+nx*(d5+2)] + mgl4*a[i+nx*(d5-2)] + a[i+nx*(d5-1)]);
		j = t->p[2];
		if(d3)	b[i] = a[i];
		else if(j==SMOOTH_LINE_3 || d5)	b[i] = y3/mgl3;
		else if(j==SMOOTH_LINE_5)		b[i] = y5/mreal(5);
		else if(j==SMOOTH_QUAD_5)		b[i] = (mreal(17)*y5-mreal(5)*x2y)/mreal(35);
	}
	return 0;
}
void *mgl_csmth_z(void *par)
{
	mglThreadC *t=(mglThreadC *)par;
	register long i,j,d3,d5, nn=t->p[0]*t->p[1], nz=t->n/nn;
	dual *b=t->a, y5,y3,x2y;
	const dual *a=t->b;
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
		x2y= (a[i+nn*(d5+1)] + mgl4*a[i+nn*(d5+2)] + mgl4*a[i+nn*(d5-2)] + a[i+nn*(d5-1)]);
		j = t->p[2];
		if(d3)	b[i] = a[i];
		else if(j==SMOOTH_LINE_3 || d5)	b[i] = y3/mgl3;
		else if(j==SMOOTH_LINE_5)		b[i] = y5/mreal(5);
		else if(j==SMOOTH_QUAD_5)		b[i] = (mreal(17)*y5-mreal(5)*x2y)/mreal(35);
	}
	return 0;
}
void mgl_datac_smooth(HADT d, const char *dirs, mreal )
{
	long Type = SMOOTH_QUAD_5;
	if(!dirs || *dirs==0)	dirs = "xyz";
	if(strchr(dirs,'0') || strchr(dirs,'1'))	return;
	if(strchr(dirs,'3'))	Type = SMOOTH_LINE_3;
	if(strchr(dirs,'5'))	Type = SMOOTH_LINE_5;
	long nx=d->nx,ny=d->ny,nz=d->nz;
//	if(Type == SMOOTH_NONE)	return;
	long p[3]={nx,ny,Type};
	dual *b = new dual[nx*ny*nz];
	// ����������� �� x
	memset(b,0,nx*ny*nz*sizeof(dual));
	if(nx>4 && strchr(dirs,'x'))
	{
		mglStartThreadC(mgl_csmth_x,0,nx*ny*nz,b,d->a,0,p);
		memcpy(d->a,b,nx*ny*nz*sizeof(dual));
		memset(b,0,nx*ny*nz*sizeof(dual));
	}
	if(ny>4 && strchr(dirs,'y'))
	{
		mglStartThreadC(mgl_csmth_y,0,nx*ny*nz,b,d->a,0,p);
		memcpy(d->a,b,nx*ny*nz*sizeof(dual));
		memset(b,0,nx*ny*nz*sizeof(dual));
	}
	if(nz>4 && strchr(dirs,'z'))
	{
		mglStartThreadC(mgl_csmth_z,0,nx*ny*nz,b,d->a,0,p);
		memcpy(d->a,b,nx*ny*nz*sizeof(dual));
	}
	delete []b;
}
void mgl_datac_smooth_(uintptr_t *d, const char *dir, mreal *delta,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_datac_smooth(_DC_,s,*delta);		delete []s;	}
//-----------------------------------------------------------------------------
void *mgl_ccsum_z(void *par)
{
	mglThreadC *t=(mglThreadC *)par;
	register long i,j, nz=t->p[2], nn=t->n;
	dual *b=t->a;
	const dual *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		b[i] = a[i];
		for(j=1;j<nz;j++)	b[i+j*nn] = b[i+j*nn-nn] + a[i+j*nn];
	}
	return 0;
}
void *mgl_ccsum_y(void *par)
{
	mglThreadC *t=(mglThreadC *)par;
	register long i,j,k, nx=t->p[0], ny=t->p[1], nn=t->n;
	dual *b=t->a;
	const dual *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = (i%nx)+nx*ny*(i/nx);		b[k] = a[k];
		for(j=1;j<ny;j++)	b[k+j*nx] = b[k+j*nx-nx] + a[k+nx*j];
	}
	return 0;
}
void *mgl_ccsum_x(void *par)
{
	mglThreadC *t=(mglThreadC *)par;
	register long i,j,k, nx=t->p[0], nn=t->n;
	dual *b=t->a;
	const dual *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = i*nx;			b[k] = a[k];
		for(j=1;j<nx;j++)	b[j+k] = b[j+k-1] + a[j+k];
	}
	return 0;
}
void mgl_datac_cumsum(HADT d, const char *dir)
{
	if(!dir || *dir==0)	return;
	long nx=d->nx,ny=d->ny,nz=d->nz,nn=nx*ny*nz;
	long p[3]={nx,ny,nz};
	dual *b = new dual[nn];
	memcpy(b,d->a,nn*sizeof(dual));
	if(strchr(dir,'z') && nz>1)
	{
		mglStartThreadC(mgl_ccsum_z,0,nx*ny,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(dual));
	}
	if(strchr(dir,'y') && ny>1)
	{
		mglStartThreadC(mgl_ccsum_y,0,nx*nz,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(dual));
	}
	if(strchr(dir,'x') && nx>1)
	{
		mglStartThreadC(mgl_ccsum_x,0,nz*ny,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(dual));
	}
	delete []b;
}
void mgl_datac_cumsum_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_datac_cumsum(_DC_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void *mgl_cint_z(void *par)
{
	mglThreadC *t=(mglThreadC *)par;
	register long i,j, nz=t->p[2], nn=t->n;
	dual *b=t->a, dd=0.5/nz;
	const dual *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		b[i] = 0;
		for(j=1;j<nz;j++)	b[i+j*nn] = b[i+j*nn-nn] + (a[i+nn*j]+a[i+j*nn-nn])*dd;
	}
	return 0;
}
void *mgl_cint_y(void *par)
{
	mglThreadC *t=(mglThreadC *)par;
	register long i,j,k, nx=t->p[0], ny=t->p[1], nn=t->n;
	dual *b=t->a, dd=0.5/ny;
	const dual *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = (i%nx)+nx*ny*(i/nx);	b[k] = 0;
		for(j=1;j<ny;j++)	b[k+j*nx] = b[k+j*nx-nx] + (a[k+j*nx]+a[k+j*nx-nx])*dd;
	}
	return 0;
}
void *mgl_cint_x(void *par)
{
	mglThreadC *t=(mglThreadC *)par;
	register long i,j,k, nx=t->p[0], nn=t->n;
	dual *b=t->a, dd=0.5/nx;
	const dual *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = i*nx;			b[k] = 0;
		for(j=1;j<nx;j++)	b[j+k] = b[j+k-1] + (a[j+k]+a[j+k-1])*dd;
	}
	return 0;
}
void mgl_datac_integral(HADT d, const char *dir)
{
	if(!dir || *dir==0)	return;
	long nx=d->nx,ny=d->ny,nz=d->nz,nn=nx*ny*nz;
	long p[3]={nx,ny,nz};
	dual *b = new dual[nn];
	memcpy(b,d->a,nn*sizeof(dual));
	if(strchr(dir,'z') && nz>1)
	{
		mglStartThreadC(mgl_cint_z,0,nx*ny,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(dual));
	}
	if(strchr(dir,'y') && ny>1)
	{
		mglStartThreadC(mgl_cint_y,0,nx*nz,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(dual));
	}
	if(strchr(dir,'x') && nx>1)
	{
		mglStartThreadC(mgl_cint_x,0,nz*ny,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(dual));
	}
	delete []b;
}
void mgl_datac_integral_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_datac_integral(_DC_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void *mgl_cdif_z(void *par)
{
	mglThreadC *t=(mglThreadC *)par;
	register long i,j, nz=t->p[2], nn=t->n;
	dual *b=t->a, dd=0.5*nz;
	const dual *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		b[i] = -(mgl3*a[i]-mgl4*a[i+nn]+a[i+2*nn])*dd;
		b[i+(nz-1)*nn] = (mgl3*a[i+(nz-1)*nn]-mgl4*a[i+(nz-2)*nn]+a[i+(nz-3)*nn])*dd;
		for(j=1;j<nz-1;j++)		b[i+j*nn] = (a[i+j*nn+nn]-a[i+j*nn-nn])*dd;
	}
	return 0;
}
void *mgl_cdif_y(void *par)
{
	mglThreadC *t=(mglThreadC *)par;
	register long i,j,k, nx=t->p[0], ny=t->p[1], nn=t->n;
	dual *b=t->a, dd=0.5*ny;
	const dual *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = (i%nx)+nx*ny*(i/nx);
		b[k] = -(mgl3*a[k]-mgl4*a[k+nx]+a[k+2*nx])*dd;
		b[k+(ny-1)*nx] = (mgl3*a[k+(ny-1)*nx]-mgl4*a[k+(ny-2)*nx]+a[k+(ny-3)*nx])*dd;
		for(j=1;j<ny-1;j++)	b[k+j*nx] = (a[k+j*nx+nx]-a[k+j*nx-nx])*dd;
	}
	return 0;
}
void *mgl_cdif_x(void *par)
{
	mglThreadC *t=(mglThreadC *)par;
	register long i,j,k, nx=t->p[0], nn=t->n;
	dual *b=t->a, dd=0.5*nx;
	const dual *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = i*nx;
		b[k] = -(mgl3*a[k]-mgl4*a[k+1]+a[k+2])*dd;
		b[k+nx-1] = (mgl3*a[k+nx-1]-mgl4*a[k+nx-2]+a[k+nx-3])*dd;
		for(j=1;j<nx-1;j++)	b[j+k] = (a[j+k+1]-a[j+k-1])*dd;
	}
	return 0;
}
void mgl_datac_diff(HADT d, const char *dir)
{
	if(!dir || *dir==0)	return;
	long nx=d->nx,ny=d->ny,nz=d->nz,nn=nx*ny*nz;
	long p[3]={nx,ny,nz};
	dual *b = new dual[nn];
	if(strchr(dir,'z') && nz>1)
	{
		mglStartThreadC(mgl_cdif_z,0,nx*ny,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(dual));
	}
	if(strchr(dir,'y') && ny>1)
	{
		mglStartThreadC(mgl_cdif_y,0,nx*nz,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(dual));
	}
	if(strchr(dir,'x') && nx>1)
	{
		mglStartThreadC(mgl_cdif_x,0,nz*ny,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(dual));
	}
	delete []b;
}
void mgl_datac_diff_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_datac_diff(_DC_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void *mgl_cdif2_z(void *par)
{
	mglThreadC *t=(mglThreadC *)par;
	register long i,j, nz=t->p[2], nn=t->n;
	dual *b=t->a, dd=0.5*nz*nz;
	const dual *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		b[i] = b[i+(nz-1)*nn] = 0;
		for(j=1;j<nz-1;j++)		b[i+j*nn] = (a[i+j*nn+nn]+a[i+j*nn-nn]-mgl2*a[i+j*nn])*dd;
	}
	return 0;
}
void *mgl_cdif2_y(void *par)
{
	mglThreadC *t=(mglThreadC *)par;
	register long i,j,k, nx=t->p[0], ny=t->p[1], nn=t->n;
	dual *b=t->a, dd=0.5*ny*ny;
	const dual *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = (i%nx)+nx*ny*(i/nx);	b[k] = b[k+(ny-1)*nx] = 0;
		for(j=1;j<ny-1;j++)	b[k+j*nx] = (a[k+j*nx+nx]+a[k+j*nx-nx]-mgl2*a[k+j*nx])*dd;
	}
	return 0;
}
void *mgl_cdif2_x(void *par)
{
	mglThreadC *t=(mglThreadC *)par;
	register long i,j,k, nx=t->p[0], nn=t->n;
	dual *b=t->a, dd=0.5*nx*nx;
	const dual *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = i*nx;			b[k] = b[k+nx-1] = 0;
		for(j=1;j<nx-1;j++)	b[j+k] = (a[j+k+1]+a[j+k-1]-mgl2*a[j+k])*dd;
	}
	return 0;
}
void mgl_datac_diff2(HADT d, const char *dir)
{
	if(!dir || *dir==0)	return;
	long nx=d->nx,ny=d->ny,nz=d->nz,nn=nx*ny*nz;
	long p[3]={nx,ny,nz};
	dual *b = new dual[nn];
	if(strchr(dir,'z') && nz>1)
	{
		mglStartThreadC(mgl_cdif2_z,0,nx*ny,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(dual));
	}
	if(strchr(dir,'y') && ny>1)
	{
		mglStartThreadC(mgl_cdif2_y,0,nx*nz,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(dual));
	}
	if(strchr(dir,'x') && nx>1)
	{
		mglStartThreadC(mgl_cdif2_x,0,nz*ny,b,d->a,0,p);
		memcpy(d->a,b,nn*sizeof(dual));
	}
	delete []b;
}
void mgl_datac_diff2_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_datac_diff2(_DC_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_datac_swap(HADT d, const char *dir)
{
	if(!dir || *dir==0)	return;
	if(strchr(dir,'z') && d->nz>1)	mgl_datac_roll(d,'z',d->nz/2);
	if(strchr(dir,'y') && d->ny>1)	mgl_datac_roll(d,'y',d->ny/2);
	if(strchr(dir,'x') && d->nx>1)	mgl_datac_roll(d,'x',d->nx/2);
}
void mgl_datac_swap_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_datac_swap(_DC_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_datac_roll(HADT dd, char dir, long num)
{
	long nx=dd->nx,ny=dd->ny,nz=dd->nz;
	register long i,d;
	dual *b,*a=dd->a;
	if(dir=='z' && nz>1)
	{
		d = num>0 ? num%nz : (num+nz*(1+num/nz))%nz;
		if(d==0)	return;		// nothing to do
		b = new dual[nx*ny*nz];
		memcpy(b,a+nx*ny*d,nx*ny*(nz-d)*sizeof(dual));
		memcpy(b+nx*ny*(nz-d),a,nx*ny*d*sizeof(dual));
		memcpy(a,b,nx*ny*nz*sizeof(dual));	delete []b;
	}
	if(dir=='y' && ny>1)
	{
		d = num>0 ? num%ny : (num+ny*(1+num/ny))%ny;
		if(d==0)	return;		// nothing to do
		b = new dual[nx*ny*nz];
		memcpy(b,a+nx*d,(nx*ny*nz-nx*d)*sizeof(dual));
		for(i=0;i<nz;i++)
			memcpy(b+nx*(ny-d)+nx*ny*i,a+nx*ny*i,nx*d*sizeof(dual));
		memcpy(a,b,nx*ny*nz*sizeof(dual));	delete []b;
	}
	if(dir=='x' && nx>1)
	{
		d = num>0 ? num%nx : (num+nx*(1+num/nx))%nx;
		if(d==0)	return;		// nothing to do
		b = new dual[nx*ny*nz];
		memcpy(b,a+d,(nx*ny*nz-d)*sizeof(dual));
		for(i=0;i<nz*ny;i++)
			memcpy(b+nx-d+nx*i,a+nx*i,d*sizeof(dual));
		memcpy(a,b,nx*ny*nz*sizeof(dual));	delete []b;
	}
}
void mgl_datac_roll_(uintptr_t *d, const char *dir, int *num, int)
{	mgl_datac_roll(_DC_,*dir,*num);	}
//-----------------------------------------------------------------------------
void mgl_datac_mirror(HADT d, const char *dir)
{
	if(!dir || *dir==0)	return;
	long nx=d->nx,ny=d->ny,nz=d->nz;
	register long i,j,k,i0,j0;
	dual b, *a=d->a;
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
void mgl_datac_mirror_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_datac_mirror(_DC_,s);	delete []s;	}
//-----------------------------------------------------------------------------
dual mgl_datac_linear_ext(HCDT d, mreal x,mreal y,mreal z, dual *dx,dual *dy,dual *dz)
{
	long kx=long(x), ky=long(y), kz=long(z);
	dual b0,b1;
	bool dif = (dx && dy && dz);
	const mglDataC *dd=dynamic_cast<const mglDataC *>(d);
	if(dd)
	{
		long nx=dd->nx, ny=dd->ny, nz=dd->nz, dn=ny>1?nx:0;
		kx = kx<nx-1 ? kx:nx-2;	kx = kx>=0 ? kx:0;
		ky = ky<ny-1 ? ky:ny-2;	ky = ky>=0 ? ky:0;
		kz = kz<nz-1 ? kz:nz-2;	kz = kz>=0 ? kz:0;
		x -= kx;	if(nx==1)	x=0;
		y -= ky;	if(ny==1)	y=0;
		z -= kz;	if(nz==1)	z=0;

		const dual *aa=dd->a+kx+nx*(ky+ny*kz), *bb = aa+(nz>1?nx*ny:0);
		b0 = aa[0]*(1-x-y+x*y) + x*(1-y)*aa[1] + y*(1-x)*aa[dn] + x*y*aa[1+dn];
		b1 = bb[0]*(1-x-y+x*y) + x*(1-y)*bb[1] + y*(1-x)*bb[dn] + x*y*bb[1+dn];
		if(dif)
		{	*dx = aa[1]-aa[0];	*dy = aa[dn]-aa[0];	*dz = bb[0]-aa[0];	}
	}
	else
	{
		long n=d->GetNx(), ny=d->GetNy(), nz=d->GetNz();
		kx = (kx>=0 ? (kx<n ? kx:n -1):0);
		ky = (ky>=0 ? (ky<ny? ky:ny-1):0);
		kz = (kz>=0 ? (kz<nz? kz:nz-1):0);
		x -= kx;	y -= ky;	z -= kz;

		dual a0 = d->v(kx,ky,kz), a1 = d->v(kx+1,ky,kz), a2 = d->v(kx,ky+1,kz);
		if(dif)	{	*dx = a1-a0;	*dy = a2-a0;	*dz = -a0;	}
		b0 = a0*(1-x-y+x*y) + x*(1-y)*a1 +
			y*(1-x)*a2 + x*y*d->v(kx+1,ky+1,kz);
		kz++;
		a0 = d->v(kx,ky,kz);
		if(dif)	*dz += a0;
		b1 = a0*(1-x-y+x*y) + x*(1-y)*d->v(kx+1,ky,kz) +
			y*(1-x)*d->v(kx,ky+1,kz) + x*y*d->v(kx+1,ky+1,kz);
	}
	return b0 + z*(b1-b0);
}
dual mgl_datac_linear(HCDT d, mreal x,mreal y,mreal z)
{	return mgl_datac_linear_ext(d, x,y,z, 0,0,0);	}
//-----------------------------------------------------------------------------
dual mgl_datac_linear_(uintptr_t *d, mreal *x,mreal *y,mreal *z)
{	return mgl_datac_linear(_DA_(d),*x,*y,*z);	}
dual mgl_datac_linear_ext_(uintptr_t *d, mreal *x,mreal *y,mreal *z, dual *dx,dual *dy,dual *dz)
{	return mgl_datac_linear_ext(_DA_(d),*x,*y,*z,dx,dy,dz);	}
//-----------------------------------------------------------------------------
void mgl_datac_crop(HADT d, long n1, long n2, char dir)
{
	long nx=d->nx,ny=d->ny,nz=d->nz, nn;
	register long i,k;
	dual *b;
	if(n1<0)	n1=0;
	switch(dir)
	{
	case 'x':
		n2 = n2>0 ? n2 : nx+n2;
		if(n2<0 || n2>=nx || n2<n1)	n2 = nx;
		nn = n2-n1;	b = new dual[nn*ny*nz];
		for(i=0;i<ny*nz;i++)
			memcpy(b+nn*i,d->a+nx*i+n1,nn*sizeof(dual));
		d->nx = nn;	if(!d->link)	delete []d->a;
		d->a = b;	d->link=false;	d->NewId();
		break;
	case 'y':
		n2 = n2>0 ? n2 : ny+n2;
		if(n2<0 || n2>=ny || n2<n1)	n2 = ny;
		nn = n2-n1;	b = new dual[nn*nx*nz];
		for(long j=0;j<nz;j++)	for(k=0;k<nn;k++)
			memcpy(b+nx*(k+nn*j),d->a+nx*(n1+k+ny*j),nx*sizeof(dual));
		d->ny = nn;	if(!d->link)	delete []d->a;
		d->a = b;	d->link=false;
		break;
	case 'z':
		n2 = n2>0 ? n2 : nz+n2;
		if(n2<0 || n2>=nz || n2<n1)	n2 = nz;
		nn = n2-n1;	b = new dual[nn*nx*ny];
		memcpy(b,d->a+nx*ny*n1,nn*nx*ny*sizeof(dual));
		d->nz = nn;	if(!d->link)	delete []d->a;
		d->a = b;	d->link=false;
		break;
	}
}
void mgl_datac_crop_(uintptr_t *d, int *n1, int *n2, const char *dir,int)
{	mgl_datac_crop(_DC_,*n1,*n2,*dir);	}
//-----------------------------------------------------------------------------
void mgl_datac_insert(HADT d, char dir, long at, long num)
{
	if(num<1)	return;
	at = at<0 ? 0:at;
	register long i,k,nn;
	long nx=d->nx, ny=d->ny, nz=d->nz;
	mglDataC b;
	if(dir=='x')
	{
		if(at>nx)	at=nx;
		nn=nx+num;	b.Create(nn,ny,nz);
		for(k=0;k<ny*nz;k++)
		{
			if(at>0)	memcpy(b.a+nn*k, d->a+nx*k,at*sizeof(dual));
			if(at<nx)	memcpy(b.a+at+num+nn*k, d->a+at+nx*k,(nx-at)*sizeof(dual));
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
			if(at>0)	memcpy(b.a+nx*nn*k, d->a+nx*ny*k,at*nx*sizeof(dual));
			if(at<ny)	memcpy(b.a+nx*(at+num+nn*k), d->a+nx*(at+ny*k),(ny-at)*nx*sizeof(dual));
			for(i=0;i<num;i++)	memcpy(b.a+nx*(nn*k+at+i),d->a+nx*(ny*k+at),nx*sizeof(dual));
		}
		d->Set(b);	ny+=num;
	}
	if(dir=='z')
	{
		if(at>nz)	at=nz;
		b.Create(nx,ny,nz+num);
		if(at>0)	memcpy(b.a, d->a,at*nx*ny*sizeof(dual));
		if(at<nz)	memcpy(b.a+nx*ny*(at+num), d->a+nx*ny*at,(nz-at)*nx*ny*sizeof(dual));
		for(i=0;i<num;i++)	memcpy(b.a+nx*ny*(at+i),d->a+nx*ny*at,nx*ny*sizeof(dual));
		d->Set(b);	nz+=num;
	}
}
//-----------------------------------------------------------------------------
void mgl_datac_delete(HADT d, char dir, long at, long num)
{
	if(num<1 || at<0)	return;
	mglDataC b;
	long nx=d->nx, ny=d->ny, nz=d->nz;
	register long k,nn;
	if(dir=='x')
	{
		if(at+num>=nx)	return;
		nn=nx-num;	b.Create(nn,ny,nz);
		for(k=0;k<ny*nz;k++)
		{
			if(at>0)	memcpy(b.a+nn*k, d->a+nx*k,at*sizeof(dual));
			memcpy(b.a+at+nn*k, d->a+at+num+nx*k,(nx-at-num)*sizeof(dual));
		}
		d->Set(b);	nx-=num;
	}
	if(dir=='y')
	{
		if(at+num>=ny)	return;
		nn=ny-num;	b.Create(nx,nn,nz);
		for(k=0;k<nz;k++)
		{
			if(at>0)	memcpy(b.a+nx*nn*k, d->a+nx*ny*k,at*nx*sizeof(dual));
			memcpy(b.a+nx*(at+nn*k), d->a+nx*(at+num+ny*k),(ny-at-num)*nx*sizeof(dual));
		}
		d->Set(b);	ny-=num;
	}
	if(dir=='z')
	{
		if(at+num>=nz)	return;
		b.Create(nx,ny,nz-num);
		if(at>0)	memcpy(b.a, d->a,at*nx*ny*sizeof(dual));
		memcpy(b.a+nx*ny*at, d->a+nx*ny*(at+num),(nz-at-num)*nx*ny*sizeof(dual));
		d->Set(b);	nz-=num;
	}
}
//-----------------------------------------------------------------------------
void mgl_datac_insert_(uintptr_t *d, const char *dir, int *at, int *num, int)
{	mgl_datac_insert(_DC_,*dir,*at,*num);	}
void mgl_datac_delete_(uintptr_t *d, const char *dir, int *at, int *num, int)
{	mgl_datac_delete(_DC_,*dir,*at,*num);	}
//-----------------------------------------------------------------------------
void mgl_datac_set_value(HADT dat, dual v, long i, long j, long k)
{
	if(i>=0 && i<dat->nx && j>=0 && j<dat->ny && k>=0 && k<dat->nz)
		dat->a[i+dat->nx*(j+dat->ny*k)]=v;
}
void mgl_datac_set_value_(uintptr_t *d, dual *v, int *i, int *j, int *k)
{	mgl_datac_set_value(_DC_,*v,*i,*j,*k);	}
//-----------------------------------------------------------------------------
dual mgl_datac_get_value(HCDT dat, long i, long j, long k)
{
	if(i<0 || i>=dat->GetNx() || j<0 || j>=dat->GetNy() || k<0 || k>=dat->GetNz())
		return NAN;
	const mglDataC *d = dynamic_cast<const mglDataC*>(dat);
	return d ? d->a[i+d->nx*(j+d->nz*k)] : dat->v(i,j,k);	
}
dual mgl_datac_get_value_(uintptr_t *d, int *i, int *j, int *k)
{	return mgl_datac_get_value(_DA_(d),*i,*j,*k);	}
//-----------------------------------------------------------------------------
dual *mgl_datac_data(HADT dat)	{	return dat->a;	}
//-----------------------------------------------------------------------------
dual *mgl_datac_value(HADT dat, long i,long j,long k)
{	register long ii=i*dat->nx*(j+dat->ny*k);
	return	ii>=0 && ii<dat->GetNN() ? dat->a+ii : 0;	}
//-----------------------------------------------------------------------------
void mgl_datac_fft(HADT d, const char *dir)
{
#if MGL_HAVE_GSL
	if(!dir || *dir==0)	return;
	long nx = d->nx, ny = d->ny, nz = d->nz;
	double *a = new double[2*nx*ny*nz];
	register long i,j;
	gsl_fft_direction how = strchr(dir,'i')?backward:forward;
	for(i=0;i<nx*ny*nz;i++)
	{	a[2*i] = real(d->a[i]);	a[2*i+1] = imag(d->a[i]);	}
	if(strchr(dir,'x') && nx>1)
	{
		gsl_fft_complex_wavetable *wt = gsl_fft_complex_wavetable_alloc(nx);
		gsl_fft_complex_workspace *ws = gsl_fft_complex_workspace_alloc(nx);
		for(i=0;i<ny*nz;i++)
			gsl_fft_complex_transform(a+2*i*nx, 1, nx, wt, ws, how);
		gsl_fft_complex_workspace_free(ws);
		gsl_fft_complex_wavetable_free(wt);
	}
	if(strchr(dir,'y') && ny>1)
	{
		gsl_fft_complex_wavetable *wt = gsl_fft_complex_wavetable_alloc(ny);
		gsl_fft_complex_workspace *ws = gsl_fft_complex_workspace_alloc(ny);
		for(j=0;j<nz;j++)	for(i=0;i<nx;i++)
			gsl_fft_complex_transform(a+2*i+2*j*nx*ny, nx, ny, wt, ws, how);
		gsl_fft_complex_workspace_free(ws);
		gsl_fft_complex_wavetable_free(wt);
	}
	if(strchr(dir,'z') && nz>1)
	{
		gsl_fft_complex_wavetable *wt = gsl_fft_complex_wavetable_alloc(nz);
		gsl_fft_complex_workspace *ws = gsl_fft_complex_workspace_alloc(nz);
		for(i=0;i<ny*nx;i++)
			gsl_fft_complex_transform(a+2*i, nx*ny, nz, wt, ws, how);
		gsl_fft_complex_workspace_free(ws);
		gsl_fft_complex_wavetable_free(wt);
	}
	for(i=0;i<nx*ny*nz;i++)	d->a[i] = dual(a[2*i], a[2*i+1]);
	delete []a;
#endif
}
void mgl_datac_fft_(uintptr_t *d, const char *dir, int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_datac_fft(_DC_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_datac_hankel(HADT d, const char *dir)
{
	#if MGL_HAVE_GSL
	if(!dir || *dir==0)	return;
	double *ai=0, *af=0, *ag=0;
	mreal mm;
	gsl_dht *dht=0;
	register long i,j,k;
	long nx=d->nx, ny=d->ny, nz=d->nz;
	dual *a=d->a;
	if(strchr(dir,'x') && nx>1)
	{
		ai = new double[nx];	af = new double[nx];	ag = new double[nx];
		dht = gsl_dht_new(nx,0,1);
		mm = gsl_sf_bessel_zero_J0(nx+1);
		for(i=0;i<ny*nz;i++)
		{
			for(j=0;j<nx;j++)	ai[j] = real(a[j+nx*i]);
			gsl_dht_apply(dht,ai,af);
			for(j=0;j<nx;j++)	ai[j] = imag(a[j+nx*i]);
			gsl_dht_apply(dht,ai,ag);
			for(j=0;j<nx;j++)	a[j+nx*i] = dual(af[j],ag[j])*mm;
		}
	}
	if(strchr(dir,'y') && ny>1)
	{
		ai = new double[ny];	af = new double[ny];	ag = new double[ny];
		dht = gsl_dht_new(ny,0,1);
		mm = gsl_sf_bessel_zero_J0(ny+1);
		for(i=0;i<nx;i++)	for(k=0;k<nz;k++)
		{
			for(j=0;j<nx;j++)	ai[j] = real(a[i+nx*(j+ny*k)]);
			gsl_dht_apply(dht,ai,af);
			for(j=0;j<nx;j++)	ai[j] = imag(a[i+nx*(j+ny*k)]);
			gsl_dht_apply(dht,ai,ag);
			for(j=0;j<nx;j++)	a[i+nx*(j+ny*k)] = dual(af[j],ag[j])*mm;
		}
	}
	if(strchr(dir,'z') && nz>1)
	{
		ai = new double[nz];	af = new double[nz];	ag = new double[nz];
		dht = gsl_dht_new(nz,0,1);
		mm = gsl_sf_bessel_zero_J0(nz+1);
		k = nx*ny;	for(i=0;i<k;i++)
		{
			for(j=0;j<nz;j++)	ai[j] = real(a[i+j*k]);
			gsl_dht_apply(dht,ai,af);
			for(j=0;j<nz;j++)	ai[j] = imag(a[i+j*k]);
			gsl_dht_apply(dht,ai,ag);
			for(j=0;j<nz;j++)	a[i+j*k] = dual(af[j],ag[j])*mm;
		}
	}
	if(ai)	{	delete []ai;	delete []af;	gsl_dht_free(dht);	}
	#endif
}
void mgl_datac_hankel_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
mgl_datac_hankel(_DC_,s);	delete []s;	}
//-----------------------------------------------------------------------------
