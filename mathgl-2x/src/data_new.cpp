/***************************************************************************
 * data_new.cpp is part of Math Graphic Library
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
#include <ctype.h>
#include "mgl2/data.h"

#if MGL_HAVE_GSL
#include <gsl/gsl_fft_complex.h>
#include <gsl/gsl_dht.h>
#include <gsl/gsl_sf.h>
#endif
//-----------------------------------------------------------------------------
void mgl_data_envelop(HMDT d, char dir)
{
#if MGL_HAVE_GSL
	register long i,j,k,i0;
	long nx=d->nx,ny=d->ny,nz=d->nz,nn=nx*ny*nz;
	double *b = new double[2*nn];
	mreal *a=d->a;
	for(i=0;i<nn;i++)	{	b[2*i] = a[i];	b[2*i+1] = 0;	}
	if((dir=='x') & (nx>1))
	{
		gsl_fft_complex_wavetable *wt = gsl_fft_complex_wavetable_alloc(nx);
		gsl_fft_complex_workspace *ws = gsl_fft_complex_workspace_alloc(nx);
		for(i=0;i<ny*nz;i++)
		{
			gsl_fft_complex_transform(b+2*i*nx, 1, nx, wt, ws, forward);
			for(j=0;j<nx;j++)
			{	b[j+2*i*nx] /= nx/2.;	b[j+nx+2*i*nx] = 0;	}
			gsl_fft_complex_transform(b+2*i*nx, 1, nx, wt, ws, backward);
		}
		gsl_fft_complex_workspace_free(ws);
		gsl_fft_complex_wavetable_free(wt);
	}
	if((dir=='y') & (ny>1))
	{
		gsl_fft_complex_wavetable *wt = gsl_fft_complex_wavetable_alloc(ny);
		gsl_fft_complex_workspace *ws = gsl_fft_complex_workspace_alloc(ny);
		for(j=0;j<nz;j++)	for(i=0;i<nx;i++)
		{
			i0 = 2*i+2*j*nx*ny;
			gsl_fft_complex_transform(b+i0, nx, ny, wt, ws, forward);
			for(k=0;k<ny;k++)
			{	b[i0+k*2*nx] /= ny/2.;	b[i0+2*nx*k+2*nx*ny] = 0;	}
			gsl_fft_complex_transform(b+i0, nx, ny, wt, ws, backward);
		}
		gsl_fft_complex_workspace_free(ws);
		gsl_fft_complex_wavetable_free(wt);
	}
	if((dir=='z') & (nz>1))
	{
		gsl_fft_complex_wavetable *wt = gsl_fft_complex_wavetable_alloc(nz);
		gsl_fft_complex_workspace *ws = gsl_fft_complex_workspace_alloc(nz);
		for(i=0;i<ny*nx;i++)
		{
			i0 = 2*nx*ny;
			gsl_fft_complex_transform(b+2*i, nx*ny, nz, wt, ws, forward);
			for(j=0;j<nz;j++)
			{	b[i+j*i0] /= nz/2.;	b[i+j*i0+nz*i0] = 0;	}
			gsl_fft_complex_transform(b+2*i, nx*ny, nz, wt, ws, backward);
		}
		gsl_fft_complex_workspace_free(ws);
		gsl_fft_complex_wavetable_free(wt);
	}
	for(i=0;i<nx*ny*nz;i++)	a[i] = hypot(b[2*i], b[2*i+1]);
	delete []b;
#endif
}
void mgl_data_envelop_(uintptr_t *d, const char *dir, int)
{	mgl_data_envelop(_DT_,*dir);	}
//-----------------------------------------------------------------------------
HMDT mgl_data_trace(HCDT d)
{
	long nx=d->GetNx(),ny=d->GetNy(),nz=d->GetNz();
	mglData *r=new mglData(nx);
	register long i;
	if((ny>=nx) & (nz>=nx))	for(i=0;i<nx;i++)	r->a[i] = d->v(i,i,i);
	else if(ny>=nx)		for(i=0;i<nx;i++)	r->a[i] = d->v(i,i);
	else		for(i=0;i<nx;i++)	r->a[i] = d->v(i);
	return r;
}
uintptr_t mgl_data_trace_(uintptr_t *d)
{	return uintptr_t(mgl_data_trace(_DT_));	}
//-----------------------------------------------------------------------------
HMDT mgl_data_subdata_ext(HCDT d, HCDT xx, HCDT yy, HCDT zz)
{
	long n=0,m=0,l=0,i,j,k,i0;
	mreal x,y,z;
	mglData *r=new mglData;
	bool ix=false, iy=false, iz=false;
	if(xx->GetNz()>1)	// 3d data
	{
		n = xx->GetNx();	m = xx->GetNy();	l = xx->GetNz();
		j = yy->GetNx()*yy->GetNy()*yy->GetNz();	if((j>1) & (j!=n*m*l))	return r;	// wrong sizes
		k = zz->GetNx()*zz->GetNy()*zz->GetNz();	if((k>1) & (k!=n*m*l))	return r;	// wrong sizes
		ix = true;	iy = j>1;	iz = k>1;
	}
	else if(yy->GetNz()>1)
	{
		n = yy->GetNx();	m = yy->GetNy();	l = yy->GetNz();
		j = xx->GetNx()*xx->GetNy()*xx->GetNz();	if((j>1) & (j!=n*m*l))	return r;	// wrong sizes
		k = zz->GetNx()*zz->GetNy()*zz->GetNz();	if((k>1) & (k!=n*m*l))	return r;	// wrong sizes
		iy = true;	ix = j>1;	iz = k>1;
	}
	else if(zz->GetNz()>1)
	{
		n = zz->GetNx();	m = zz->GetNy();	l = zz->GetNz();
		j = yy->GetNx()*yy->GetNy()*yy->GetNz();	if((j>1) & (j!=n*m*l))	return r;	// wrong sizes
		k = xx->GetNx()*xx->GetNy()*xx->GetNz();	if((k>1) & (k!=n*m*l))	return r;	// wrong sizes
		iz = true;	iy = j>1;	ix = k>1;
	}
	else if(xx->GetNy()>1)	// 2d data
	{
		n = xx->GetNx();	m = xx->GetNy();	l = 1;
		j = yy->GetNx()*yy->GetNy();	if((j>1) & (j!=n*m))	return r;	// wrong sizes
		k = zz->GetNx()*zz->GetNy();	if((k>1) & (k!=n*m))	return r;	// wrong sizes
		ix = true;	iy = j>1;	iz = k>1;
	}
	else if(yy->GetNy()>1)
	{
		n = yy->GetNx();	m = yy->GetNy();	l = 1;
		j = xx->GetNx()*xx->GetNy();	if((j>1) & (j!=n*m))	return r;	// wrong sizes
		k = zz->GetNx()*zz->GetNy();	if((k>1) & (k!=n*m))	return r;	// wrong sizes
		iy = true;	ix = j>1;	iz = k>1;
	}
	else if(zz->GetNy()>1)
	{
		n = zz->GetNx();	m = zz->GetNy();	l = 1;
		j = yy->GetNx()*yy->GetNy();	if((j>1) & (j!=n*m))	return r;	// wrong sizes
		k = xx->GetNx()*xx->GetNy();	if((k>1) & (k!=n*m))	return r;	// wrong sizes
		iz = true;	iy = j>1;	ix = k>1;
	}
	long nx=d->GetNx(),ny=d->GetNy(),nz=d->GetNz();
	mreal vx=xx->v(0), vy=yy->v(0), vz=zz->v(0);
	if(n*m*l>1)	// this is 2d or 3d data
	{
		r->Create(n,m,l);
		for(i0=0;i0<n*m*l;i0++)
		{
			x = ix?xx->vthr(i0):vx;
			y = iy?yy->vthr(i0):vy;
			z = iz?zz->vthr(i0):vz;
			r->a[i0] = mgl_data_linear(d,x,y,z);
		}
		return r;
	}
	// this is 1d data -> try as normal SubData()
	if((xx->GetNx()>1) | (vx>=0))	{	n=xx->GetNx();	ix=true;	}
	else	{	n=nx;	ix=false;	}
	if((yy->GetNx()>1) | (vy>=0))	{	m=yy->GetNx();	iy=true;	}
	else	{	m=ny;	iy=false;	}
	if((zz->GetNx()>1) | (vz>=0))	{	l=zz->GetNx();	iz=true;	}
	else	{	l=nz;	iz=false;	}
	r->Create(n,m,l);
	for(k=0;k<l;k++)	for(j=0;j<m;j++)	for(i=0;i<n;i++)
	{
		x = ix?xx->v(i):i;	y = iy?yy->v(j):j;	z = iz?zz->v(k):k;
		r->a[i+n*(j+m*k)] = mgl_data_linear(d,x,y,z);
	}
	if(m==1)	{	r->ny=r->nz;	r->nz=1;	}// "squeeze" dimensions
	if(n==1)	{	r->nx=r->ny;	r->ny=r->nz;	r->nz=1;	r->NewId();}
	return r;
}
//-----------------------------------------------------------------------------
HMDT mgl_data_subdata(HCDT d, long xx,long yy,long zz)
{
	mglData x,y,z;
	x.a[0]=xx;	y.a[0]=yy;	z.a[0]=zz;
	return mgl_data_subdata_ext(d,&x,&y,&z);
}
//-----------------------------------------------------------------------------
uintptr_t mgl_data_subdata_(uintptr_t *d, int *xx,int *yy,int *zz)
{	return uintptr_t(mgl_data_subdata(_DT_,*xx,*yy,*zz));	}
uintptr_t mgl_data_subdata_ext_(uintptr_t *d, uintptr_t *xx, uintptr_t *yy, uintptr_t *zz)
{	return uintptr_t(mgl_data_subdata_ext(_DT_,_DA_(xx),_DA_(yy),_DA_(zz)));	}
//-----------------------------------------------------------------------------
void *mgl_column(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	mglFormula *f = (mglFormula *)t->v;
	register long i,j, nx=t->p[0];
	mreal *b=t->a, var[MGL_VS];
	const mreal *a=t->b;
	memset(var,0,('z'-'a')*sizeof(mreal));
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		for(j=0;j<nx;j++)
			if((t->s[j]>='a') & (t->s[j]<='z'))
				var[t->s[j]-'a'] = a[j+nx*i];
		b[i] = f->Calc(var);
	}
	return 0;
}
HMDT mgl_data_column(HCDT dat, const char *eq)
{	// NOTE: only for mglData (for speeding up)
	long nx=dat->GetNx(),ny=dat->GetNy(),nz=dat->GetNz();
	mglFormula f(eq);
	mglData *r=new mglData(ny,nz);
	const mglData *d=dynamic_cast<const mglData *>(dat);
	if(d)	mglStartThread(mgl_column,0,ny*nz,r->a,d->a,0,&nx,&f,0,0,d->id);
	return r;
}
uintptr_t mgl_data_column_(uintptr_t *d, const char *eq,int l)
{	char *s=new char[l+1];	memcpy(s,eq,l);	s[l]=0;
	uintptr_t r = uintptr_t(mgl_data_column(_DT_,s));
	delete []s;	return r;	}
//-----------------------------------------------------------------------------
void *mgl_resize(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k,i0, nx=t->p[0]+0.1, ny=t->p[1]+0.1;
	long n1=t->p[3]+0.1,n2=t->p[4]+0.1,n3=t->p[5]+0.1;
	mreal *b=t->a;
	const mreal *a=t->b, *c=t->c;
	for(i0=t->id;i0<t->n;i0+=mglNumThr)
	{
		i=i0%nx;	j=((i0/nx)%ny);	k=i0/(nx*ny);
		b[i0] = mglSpline3(a,n1,n2,n3, c[0]+i*c[1], c[2]+j*c[3], c[4]+k*c[5]);
	}
	return 0;
}
HMDT mgl_data_resize_box(HCDT dat, long mx,long my,long mz, float x1,float x2, float y1,float y2, float z1,float z2)
{	// NOTE: only for mglData (for speeding up)
	mx = mx<1 ? 1:mx;	my = my<1 ? 1:my;	mz = mz<1 ? 1:mz;
	mglData *r=new mglData(mx,my,mz);
	const mglData *d=dynamic_cast<const mglData *>(dat);
	if(!d)	return r;
	register long nx = d->nx-1, ny = d->ny-1, nz = d->nz-1;
	mreal par[6]={nx*x1,0,ny*y1,0,nz*z1,0};
	long nn[6]={mx,my,mz,nx+1,ny+1,nz+1};
	if(mx>1)	par[1] = nx*(x2-x1)/(mx-1);
	if(my>1)	par[3] = ny*(y2-y1)/(my-1);
	if(mz>1)	par[5] = nz*(z2-z1)/(mz-1);
	mglStartThread(mgl_resize,0,mx*my*mz,r->a,d->a,par,nn);
	return r;
}
HMDT mgl_data_resize(HCDT d, long mx,long my,long mz)
{	return mgl_data_resize_box(d, mx,my,mz,0,1,0,1,0,1);	}
uintptr_t mgl_data_resize_(uintptr_t *d, int *mx,int *my,int *mz)
{	return uintptr_t(mgl_data_resize(_DT_,*mx,*my,*mz));	}
uintptr_t mgl_data_resize_box_(uintptr_t *d, int *mx,int *my,int *mz, float *x1,float *x2, float *y1,float *y2, float *z1,float *z2)
{	return uintptr_t(mgl_data_resize_box(_DT_,*mx,*my,*mz,*x1,*x2,*y1,*y2,*z1,*z2));	}
//-----------------------------------------------------------------------------
void *mgl_combine(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i0, nx=t->p[0];
	mreal *b=t->a;
	const mreal *c=t->b, *d=t->c;
	for(i0=t->id;i0<t->n;i0+=mglNumThr)	b[i0] = c[i0%nx]*d[i0/nx];
	return 0;
}
HMDT mgl_data_combine(HCDT d1, HCDT d2)
{	// NOTE: only for mglData (for speeding up)
	const mglData *a=dynamic_cast<const mglData *>(d1);
	const mglData *b=dynamic_cast<const mglData *>(d2);

	mglData *r=new mglData;
	if((a->nz>1) | ((a->ny>1) & (b->ny>1)) | (b->nz>1))	return r;
	long n1=a->ny,n2=b->nx;
	bool dim2=true;
	if(a->ny==1)	{	n1=b->nx;	n2=b->ny;	dim2 = false;	}
	r->Create(a->nx,n1,n2);
	if(dim2)	n1=a->nx*a->ny;	else	{	n1=a->nx;	n2=b->nx*b->ny;	}
	mglStartThread(mgl_combine,0,n1*n2,r->a,a->a,b->a,&n1);
	return r;
}
uintptr_t mgl_data_combine_(uintptr_t *a, uintptr_t *b)
{	return uintptr_t(mgl_data_combine(_DA_(a),_DA_(b)));	}
//-----------------------------------------------------------------------------
void *mgl_sum_z(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j, nz=t->p[2], nn=t->n;
	mreal *b=t->a;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		for(j=0,b[i]=0;j<nz;j++)	b[i] += a[i+nn*j];
		b[i] /= nz;
	}
	return 0;
}
void *mgl_sum_y(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k, nx=t->p[0], ny=t->p[1], nn=t->n;
	mreal *b=t->a;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = (i%nx)+nx*ny*(i/nx);
		for(j=0,b[i]=0;j<ny;j++)	b[i] += a[k+nx*j];
		b[i] /= ny;
	}
	return 0;
}
void *mgl_sum_x(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k, nx=t->p[0], nn=t->n;
	mreal *b=t->a;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = i*nx;
		for(j=0,b[i]=0;j<nx;j++)	b[i] += a[j+k];
		b[i] /= nx;
	}
	return 0;
}
HMDT mgl_data_sum(HCDT dat, const char *dir)
{
	long nx=dat->GetNx(),ny=dat->GetNy(),nz=dat->GetNz();
	long p[3]={nx,ny,nz};
	mreal *b = new mreal[nx*ny*nz];
	mreal *c = new mreal[nx*ny*nz];

	const mglData *d=dynamic_cast<const mglData *>(dat);
	if(d)	memcpy(c,d->a,nx*ny*nz*sizeof(mreal));
	else	for(long i=0;i<nx*ny*nz;i++)	c[i]=dat->vthr(i);

	if(strchr(dir,'z') && nz>1)
	{
		mglStartThread(mgl_sum_z,0,nx*ny,b,c,0,p);
		memcpy(c,b,nx*ny*sizeof(mreal));	p[2] = 1;
	}
	if(strchr(dir,'y') && ny>1)
	{
		mglStartThread(mgl_sum_y,0,nx*p[2],b,c,0,p);
		memcpy(c,b,nx*p[2]*sizeof(mreal));	p[1] = p[2];	p[2] = 1;
	}
	if(strchr(dir,'x') && nx>1)
	{
		mglStartThread(mgl_sum_x,0,p[1]*p[2],b,c,0,p);
		p[0] = p[1];	p[1] = p[2];	p[2] = 1;
	}
	mglData *r=new mglData(p[0],p[1],p[2]);
	memcpy(r->a,b,p[0]*p[1]*p[2]*sizeof(mreal));
	delete []b;	delete []c;	return r;
}
uintptr_t mgl_data_sum_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	uintptr_t r=uintptr_t(mgl_data_sum(_DT_,s));	delete []s;	return r;	}
//-----------------------------------------------------------------------------
void *mgl_max_z(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j, nz=t->p[2], nn=t->n;
	mreal *b=t->a;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
		for(j=1,b[i]=a[i];j<nz;j++)	if(b[i]<a[i+nn*j]) b[i] = a[i+nn*j];
	return 0;
}
void *mgl_max_y(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k, nx=t->p[0], ny=t->p[1], nn=t->n;
	mreal *b=t->a;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = (i%nx)+nx*ny*(i/nx);
		for(j=1,b[i]=a[k];j<ny;j++)	if(b[i]<a[k+nx*j])	b[i]=a[k+nx*j];
	}
	return 0;
}
void *mgl_max_x(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k, nx=t->p[0], nn=t->n;
	mreal *b=t->a;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = i*nx;
		for(j=1,b[i]=a[k];j<nx;j++)	if(b[i]<a[j+k])	b[i]=a[j+k];
	}
	return 0;
}
HMDT mgl_data_max_dir(HCDT dat, const char *dir)
{
	long nx=dat->GetNx(),ny=dat->GetNy(),nz=dat->GetNz();
	long p[3]={nx,ny,nz};
	mreal *b = new mreal[nx*ny*nz];
	mreal *c = new mreal[nx*ny*nz];

	const mglData *d=dynamic_cast<const mglData *>(dat);
	if(d)	memcpy(c,d->a,nx*ny*nz*sizeof(mreal));
	else	for(long i=0;i<nx*ny*nz;i++)	c[i]=dat->vthr(i);

	if(strchr(dir,'z') && nz>1)
	{
		mglStartThread(mgl_max_z,0,nx*ny,b,c,0,p);
		memcpy(c,b,nx*ny*sizeof(mreal));	p[2] = 1;
	}
	if(strchr(dir,'y') && ny>1)
	{
		mglStartThread(mgl_max_y,0,nx*p[2],b,c,0,p);
		memcpy(c,b,nx*p[2]*sizeof(mreal));	p[1] = p[2];	p[2] = 1;
	}
	if(strchr(dir,'x') && nx>1)
	{
		mglStartThread(mgl_max_x,0,p[1]*p[2],b,c,0,p);
		p[0] = p[1];	p[1] = p[2];	p[2] = 1;
	}
	mglData *r=new mglData(p[0],p[1],p[2]);
	memcpy(r->a,b,p[0]*p[1]*p[2]*sizeof(mreal));
	delete []b;	delete []c;	return r;
}
uintptr_t mgl_data_max_dir_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	uintptr_t r=uintptr_t(mgl_data_max_dir(_DT_,s));	delete []s;	return r;	}
//-----------------------------------------------------------------------------
void *mgl_min_z(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j, nz=t->p[2], nn=t->n;
	mreal *b=t->a;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
		for(j=1,b[i]=a[i];j<nz;j++)	if(b[i]<a[i+nn*j]) b[i] = a[i+nn*j];
	return 0;
}
void *mgl_min_y(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k, nx=t->p[0], ny=t->p[1], nn=t->n;
	mreal *b=t->a;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = (i%nx)+nx*ny*(i/nx);
		for(j=1,b[i]=a[k];j<ny;j++)	if(b[i]<a[k+nx*j])	b[i]=a[k+nx*j];
	}
	return 0;
}
void *mgl_min_x(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k, nx=t->p[0], nn=t->n;
	mreal *b=t->a;
	const mreal *a=t->b;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = i*nx;
		for(j=1,b[i]=a[k];j<nx;j++)	if(b[i]<a[j+k])	b[i]=a[j+k];
	}
	return 0;
}
HMDT mgl_data_min_dir(HCDT dat, const char *dir)
{
	long nx=dat->GetNx(),ny=dat->GetNy(),nz=dat->GetNz();
	long p[3]={nx,ny,nz};
	mreal *b = new mreal[nx*ny*nz];
	mreal *c = new mreal[nx*ny*nz];

	const mglData *d=dynamic_cast<const mglData *>(dat);
	if(d)	memcpy(c,d->a,nx*ny*nz*sizeof(mreal));
	else	for(long i=0;i<nx*ny*nz;i++)	c[i]=dat->vthr(i);

	if(strchr(dir,'z') && nz>1)
	{
		mglStartThread(mgl_min_z,0,nx*ny,b,c,0,p);
		memcpy(c,b,nx*ny*sizeof(mreal));	p[2] = 1;
	}
	if(strchr(dir,'y') && ny>1)
	{
		mglStartThread(mgl_min_y,0,nx*p[2],b,c,0,p);
		memcpy(c,b,nx*p[2]*sizeof(mreal));	p[1] = p[2];	p[2] = 1;
	}
	if(strchr(dir,'x') && nx>1)
	{
		mglStartThread(mgl_min_x,0,p[1]*p[2],b,c,0,p);
		p[0] = p[1];	p[1] = p[2];	p[2] = 1;
	}
	mglData *r=new mglData(p[0],p[1],p[2]);
	memcpy(r->a,b,p[0]*p[1]*p[2]*sizeof(mreal));
	delete []b;	delete []c;	return r;
}
uintptr_t mgl_data_min_dir_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	uintptr_t r=uintptr_t(mgl_data_min_dir(_DT_,s));	delete []s;	return r;	}
//-----------------------------------------------------------------------------
void *mgl_mom_z(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k,ii;
	long nx=t->p[0], ny=t->p[1], nz=t->p[2], nn=t->n;
	mreal *b=t->a, i0,i1,x,y,z;
	const mreal *a=t->b;
	const mglFormula *eq = (mglFormula *)t->v;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		i0 = i1 = 0;
		for(k=0;k<nx;k++)	for(j=0;j<ny;j++)
		{
			ii = k+nx*(j+ny*i);
			x = k/(nx-1.);	y = j/(ny-1.);	z = i/(nz-1.);
			i0+= a[ii];
			i1+= a[ii]*eq->Calc(x,y,z,a[ii]);
		}
		b[i] = i0>0 ? i1/i0 : 0;
	}
	return 0;
}
void *mgl_mom_y(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k,ii;
	long nx=t->p[0], ny=t->p[1], nz=t->p[2], nn=t->n;
	mreal *b=t->a, i0,i1,x,y,z;
	const mreal *a=t->b;
	const mglFormula *eq = (mglFormula *)t->v;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		i0 = i1 = 0;
		for(j=0;j<nx;j++)	for(k=0;k<nz;k++)
		{
			ii = j+nx*(i+ny*k);
			x = j/(nx-1.);	y = i/(ny-1.);	z = k/(nz-1.);
			i0+= a[ii];
			i1+= a[ii]*eq->Calc(x,y,z,a[ii]);
		}
		b[i] = i0>0 ? i1/i0 : 0;
	}
	return 0;
}
void *mgl_mom_x(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k,ii;
	long nx=t->p[0], ny=t->p[1], nz=t->p[2], nn=t->n;
	mreal *b=t->a, i0,i1,x,y,z;
	const mreal *a=t->b;
	const mglFormula *eq = (mglFormula *)t->v;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		i0 = i1 = 0;
		for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
		{
			ii = i+nx*(j+ny*k);
			x = i/(nx-1.);	y = j/(ny-1.);	z = k/(nz-1.);
			i0+= a[ii];
			i1+= a[ii]*eq->Calc(x,y,z,a[ii]);
		}
		b[i] = i0>0 ? i1/i0 : 0;
	}
	return 0;
}
HMDT mgl_data_momentum(HCDT dat, char dir, const char *how)
{	// NOTE: only for mglData (for speeding up)
	long nx=dat->GetNx(),ny=dat->GetNy(),nz=dat->GetNz();
	const mglData *d=dynamic_cast<const mglData *>(dat);
	mglData *b=new mglData;
	if(!d)	return b;
	mglFormula eq(how);
	long p[3]={nx,ny,nz};
	if(dir=='x')
	{	b->Create(nx);	mglStartThread(mgl_mom_x,0,nx,b->a,d->a,0,p,&eq);	}
	if(dir=='y')
	{	b->Create(ny);	mglStartThread(mgl_mom_y,0,ny,b->a,d->a,0,p,&eq);	}
	if(dir=='z')
	{	b->Create(nz);	mglStartThread(mgl_mom_z,0,nz,b->a,d->a,0,p,&eq);	}
	return b;
}
uintptr_t mgl_data_momentum_(uintptr_t *d, char *dir, const char *how, int,int l)
{	char *s=new char[l+1];	memcpy(s,how,l);	s[l]=0;
	uintptr_t r=uintptr_t(mgl_data_momentum(_DT_,*dir, s));
	delete []s;	return r;	}
//-----------------------------------------------------------------------------
void *mgl_eval(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i, nx=t->p[0], ny=t->p[1], nz=t->p[2], n1=t->p[3], nn=t->n;
	const mreal *a=t->b, *ii=t->c, *jj=t->d, *kk=t->e;
	mreal *b=t->a,x,y,z;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		x=ii?ii[i]:0;	y=jj?jj[i]:0;	z=kk?kk[i]:0;
		if(n1)	{	x*=nx-1;	y*=ny-1;	z*=nz-1;	}
		b[i] = mglSpline3(a,nx,ny,nz, x,y,z);
	}
	return 0;
}
HMDT mgl_data_evaluate(HCDT dat, HCDT idat, HCDT jdat, HCDT kdat, int norm)
{	// NOTE: only for mglData (for speeding up)
	mglData *r=new mglData;
	const mglData *d=dynamic_cast<const mglData *>(dat);
	const mglData *i=dynamic_cast<const mglData *>(idat);
	const mglData *j=dynamic_cast<const mglData *>(jdat);
	const mglData *k=dynamic_cast<const mglData *>(kdat);
	if(!d | !i)	return r;

	long p[4]={i->nx,i->ny,i->nz,norm};
	register long n=i->nx*i->ny*i->nz;
	if(j && j->nx*j->ny*j->nz!=n)	return r;
	if(k && k->nx*k->ny*k->nz!=n)	return r;
	r->Create(i->nx,i->ny,i->nz);
	mglStartThread(mgl_eval,0,n,r->a,d->a,i->a,p,0,j?j->a:0,k?k->a:0);
	return r;
}
uintptr_t mgl_data_evaluate_(uintptr_t *d, uintptr_t *idat, uintptr_t *jdat, uintptr_t *kdat, int *norm)
{	return uintptr_t(mgl_data_evaluate(_DT_,_DA_(idat),_DA_(jdat),_DA_(kdat),*norm));	}
//-----------------------------------------------------------------------------
void mgl_data_fourier(HMDT re, HMDT im, const char *dir)
{
#if MGL_HAVE_GSL
	long nx = re->nx, ny = re->ny, nz = re->nz;
	if(nx*ny*nz != im->nx*im->ny*im->nz || !dir || dir[0]==0)	return;
	double *a = new double[2*nx*ny*nz];
	register long i,j;
	gsl_fft_direction how = strchr(dir,'i')?backward:forward;
	for(i=0;i<nx*ny*nz;i++)
	{	a[2*i] = re->a[i];	a[2*i+1] = im->a[i];	}
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
	for(i=0;i<nx*ny*nz;i++)
	{	re->a[i] = a[2*i];	im->a[i] = a[2*i+1];	}
	delete []a;
#endif
}
void mgl_data_fourier_(uintptr_t *re, uintptr_t *im, const char *dir, int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_fourier(_DM_(re),_DM_(im),s);	delete []s;	}
//-----------------------------------------------------------------------------
HMDT mgl_data_stfa(HCDT re, HCDT im, long dn, char dir)
{
	mglData *d=new mglData;
#if MGL_HAVE_GSL
	if(dn<2)	return d;
	dn = 2*(dn/2);
	long nx = re->GetNx(), ny = re->GetNy();
	if(nx*ny!=im->GetNx()*im->GetNy())	return d;
	register long i,j,k,i0,dd=dn/2;
	double *a = new double[4*dn],ff;
//	for(i=0;i<nx*ny;i++)	{	a[2*i] = re.a[i];	a[2*i+1] = im.a[i];	}
	gsl_fft_complex_wavetable *wt = gsl_fft_complex_wavetable_alloc(2*dn);
	gsl_fft_complex_workspace *ws = gsl_fft_complex_workspace_alloc(2*dn);
	long mx,my,mz;
	if(dir=='y')
	{
		mx = nx;	my = dn;	mz = ny/dn;
		mgl_data_create(d, mx, mz, my);
		for(j=0;j<mz;j++)	for(i=0;i<mx;i++)
		{
			for(k=0;k<2*dn;k++)
			{
				i0 = k-dd+j*dn;		ff = 1;
				if(i0<0)	i0=0;	else if(i0>=ny)	i0=ny-1;
				if(k<dd)
				{	ff = 0.5*(k-dd/2.)/dd;		ff=0.5+ff*(3-ff*ff);	}
				else if(k>=dn+dd)
				{	ff = 0.5*(k-3.5*dd)/dd;	ff=0.5-ff*(3-ff*ff);	}
				a[2*k] = re->v(i,i0)*ff;	a[2*k+1] = im->v(i,i0)*ff;
			}
			gsl_fft_complex_forward(a, 1, 2*dn, wt, ws);
			for(k=0;k<dd;k++)
			{
				i0 = i+mx*(j+mz*k);
				d->a[i0+mx*mz*dd] = hypot(a[4*k],a[4*k+1])/dn;
				d->a[i0] = hypot(a[4*k+2*dn],a[4*k+2*dn+1])/dn;
			}
		}
	}
	else
	{
		mx = dn;	my = nx/dn;	mz = ny;
		mgl_data_create(d, my, mx, mz);
		for(j=0;j<mz;j++)	for(i=0;i<my;i++)
		{
			for(k=0;k<2*dn;k++)
			{
				i0 = k-dd+i*dn;		ff = 1;
				if(i0<0)	i0=0;	else if(i0>=nx)	i0=nx-1;
				if(k<dd)
				{	ff = 0.5*(k-dd/2.)/dd;	ff=0.5+ff*(3-ff*ff);	}
				else if(k>=3*dd)
				{	ff = 0.5*(k-3.5*dd)/dd;	ff=0.5-ff*(3-ff*ff);	}
				a[2*k] = re->v(i0,j)*ff;	a[2*k+1] = im->v(i0,j)*ff;
			}
			gsl_fft_complex_forward(a, 1, 2*dn, wt, ws);
			for(k=0;k<dd;k++)
			{
				i0 = i+my*(k+mx*j);
				d->a[i0+dd*my] = hypot(a[4*k],a[4*k+1])/dn;
				d->a[i0] = hypot(a[4*k+2*dn],a[4*k+2*dn+1])/dn;
			}
		}
	}
	delete []a;
	gsl_fft_complex_workspace_free(ws);
	gsl_fft_complex_wavetable_free(wt);
#endif
	return d;
}
uintptr_t mgl_data_stfa_(uintptr_t *re, uintptr_t *im, int *dn, char *dir, int)
{	return uintptr_t(mgl_data_stfa(_DA_(re),_DA_(im),*dn,*dir));	}
//-----------------------------------------------------------------------------
void mgl_data_fill_sample(HMDT d, const char *how)
{
	bool xx = strchr(how,'x');
	register long i,n=d->nx;
	mreal *aa=d->a;
	if(strchr(how,'h'))	// Hankel
	{
#if MGL_HAVE_GSL
		gsl_dht *dht = gsl_dht_new(n,0,1);
		for(i=0;i<n;i++)
			aa[i] = xx ? gsl_dht_x_sample(dht, i) : gsl_dht_k_sample(dht, i);
		gsl_dht_free(dht);
#endif
	}
	else	// Fourier
	{
		if(xx)	for(i=0;i<n;i++)	aa[i] = mreal(2*i-n)/n;
		else	for(i=0;i<n;i++)	aa[i] = M_PI*(i<n/2 ? i:i-n);
	}
	for(i=1;i<d->ny*d->nz;i++)	memcpy(aa+i*n,aa,n*sizeof(mreal));
}
void mgl_data_fill_sample_(uintptr_t *d, const char *how,int l)
{	char *s=new char[l+1];	memcpy(s,how,l);	s[l]=0;
	mgl_data_fill_sample(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_data_hankel(HMDT d, const char *dir)
{
#if MGL_HAVE_GSL
	double *ai=0, *af=0, mm;
	gsl_dht *dht=0;
	register long i,j,k;
	long nx=d->nx, ny=d->ny, nz=d->nz;
	mreal *a=d->a;
	if(strchr(dir,'x') && nx>1)
	{
		ai = new double[nx];	af = new double[nx];
		dht = gsl_dht_new(nx,0,1);
		mm = gsl_sf_bessel_zero_J0(nx+1);
		for(i=0;i<ny*nz;i++)
		{
			for(j=0;j<nx;j++)	ai[j] = a[j+nx*i];
			gsl_dht_apply(dht,ai,af);
			for(j=0;j<nx;j++)	a[j+nx*i] = af[j]*mm;
		}
	}
	if(strchr(dir,'y') && ny>1)
	{
		ai = new double[ny];	af = new double[ny];
		dht = gsl_dht_new(ny,0,1);
		mm = gsl_sf_bessel_zero_J0(ny+1);
		for(i=0;i<nx;i++)	for(k=0;k<nz;k++)
		{
			for(j=0;j<nx;j++)	ai[j] = a[i+nx*(j+ny*k)];
			gsl_dht_apply(dht,ai,af);
			for(j=0;j<nx;j++)	a[i+nx*(j+ny*k)] = af[j]*mm;
		}
	}
	if(strchr(dir,'z') && nz>1)
	{
		ai = new double[nz];	af = new double[nz];
		dht = gsl_dht_new(nz,0,1);
		mm = gsl_sf_bessel_zero_J0(nz+1);
		k = nx*ny;	for(i=0;i<k;i++)
		{
			for(j=0;j<nz;j++)	ai[j] = a[i+j*k];
			gsl_dht_apply(dht,ai,af);
			for(j=0;j<nz;j++)	a[i+j*k] = af[j]*mm;
		}
	}
	if(ai)	{	delete []ai;	delete []af;	gsl_dht_free(dht);	}
#endif
}
void mgl_data_hankel_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_hankel(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_data_cosfft(HMDT d, const char *dir)
{
#if MGL_HAVE_GSL
	double *b = 0;
	gsl_fft_complex_wavetable *wt=0;
	gsl_fft_complex_workspace *ws=0;
	register long i,j,k;
	long nx=d->nx, ny=d->ny, nz=d->nz;
	mreal *a=d->a;
	if(strchr(dir,'x') && nx>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(2*nx);
		ws = gsl_fft_complex_workspace_alloc(2*nx);
		b = new double[4*nx];
		for(i=0;i<ny*nz;i++)
		{
			k = i*nx;	memset(b,0,4*nx*sizeof(double));	b[0] = b[2*nx] = a[k];
			for(j=1;j<nx;j++)	{	b[2*j] = a[k+j];	b[4*nx-2*j] = a[k+j];	}
			gsl_fft_complex_transform(b, 1, 2*nx, wt, ws, forward);
			for(j=0;j<nx;j++)	a[k+j] = b[2*j]/sqrt(2.*nx);
		}
	}
	if(strchr(dir,'y') && ny>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(2*ny);
		ws = gsl_fft_complex_workspace_alloc(2*ny);
		b = new double[4*ny];
		for(i=0;i<nx;i++)	for(k=0;k<nz;k++)
		{
			memset(b,0,4*ny*sizeof(double));	b[0] = b[2*ny] = a[i+nx*ny*k];
			for(j=1;j<ny;j++)	{	b[2*j] = a[i+nx*(ny*k+j)];	b[4*ny-2*j] = a[i+nx*(ny*k+j)];	}
			gsl_fft_complex_transform(b, 1, 2*ny, wt, ws, forward);
			for(j=0;j<ny;j++)	a[i+nx*(ny*k+j)] = b[2*j]/sqrt(2.*ny);
		}
	}
	if(strchr(dir,'z') && nz>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(2*nz);
		ws = gsl_fft_complex_workspace_alloc(2*nz);
		b = new double[4*nz];	k = nx*ny;
		for(i=0;i<k;i++)
		{
			memset(b,0,4*nz*sizeof(double));	b[0] = b[2*nx] = a[i];
			for(j=1;j<nx;j++)	{	b[2*j] = a[i+k*j];	b[4*nx-2*j] = a[i+k*j];	}
			gsl_fft_complex_transform(b, 1, 2*nz, wt, ws, forward);
			for(j=0;j<nz;j++)	a[i+k*j] = b[2*j]/sqrt(2.*nz);
		}
	}
	if(b)
	{	delete []b;
		gsl_fft_complex_workspace_free(ws);
		gsl_fft_complex_wavetable_free(wt);	}
#endif
}
void mgl_data_cosfft_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_cosfft(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_data_sinfft(HMDT d, const char *dir)
{
#if MGL_HAVE_GSL
	double *b = 0;
	gsl_fft_complex_wavetable *wt=0;
	gsl_fft_complex_workspace *ws=0;
	register long i,j,k;
	long nx=d->nx, ny=d->ny, nz=d->nz;
	mreal *a=d->a;
	if(strchr(dir,'x') && nx>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(2*nx);
		ws = gsl_fft_complex_workspace_alloc(2*nx);
		b = new double[4*nx];
		for(i=0;i<ny*nz;i++)
		{
			k = i*nx;	memset(b,0,4*nx*sizeof(double));	b[0] = a[k];	b[2*nx] = -a[k];
			for(j=1;j<nx;j++)	{	b[2*j] = a[k+j];	b[4*nx-2*j] = -a[k+j];	}
			gsl_fft_complex_transform(b, 1, 2*nx, wt, ws, forward);
			for(j=0;j<nx;j++)	a[k+j] = -b[2*j+1]/sqrt(2.*nx);
		}
	}
	if(strchr(dir,'y') && ny>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(2*ny);
		ws = gsl_fft_complex_workspace_alloc(2*ny);
		b = new double[4*ny];
		for(i=0;i<nx;i++)	for(k=0;k<nz;k++)
		{
			memset(b,0,4*ny*sizeof(double));	b[0] = a[i+nx*ny*k];	b[2*ny] = -a[i+nx*ny*k];
			for(j=1;j<ny;j++)	{	b[2*j] = a[i+nx*(ny*k+j)];	b[4*ny-2*j] = -a[i+nx*(ny*k+j)];	}
			gsl_fft_complex_transform(b, 1, 2*ny, wt, ws, forward);
			for(j=0;j<ny;j++)	a[i+nx*(ny*k+j)] = -b[2*j+1]/sqrt(2.*ny);
		}
	}
	if(strchr(dir,'z') && nz>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(2*nz);
		ws = gsl_fft_complex_workspace_alloc(2*nz);
		b = new double[4*nz];	k = nx*ny;
		for(i=0;i<k;i++)
		{
			memset(b,0,4*nz*sizeof(double));	b[0] = a[i];	b[2*nx] = -a[i];
			for(j=1;j<nx;j++)	{	b[2*j] = a[i+k*j];	b[4*nx-2*j] = -a[i+k*j];	}
			gsl_fft_complex_transform(b, 1, 2*nz, wt, ws, forward);
			for(j=0;j<nz;j++)	a[i+k*j] = -b[2*j+1]/sqrt(2.*nz);
		}
	}
	if(b)
	{	delete []b;
		gsl_fft_complex_workspace_free(ws);
		gsl_fft_complex_wavetable_free(wt);	}
#endif
}
void mgl_data_sinfft_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_sinfft(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
HMDT mgl_transform_a(HCDT am, HCDT ph, const char *tr)
{
	long nx = am->GetNx(), ny = am->GetNy(), nz = am->GetNz();
	if(nx*ny*nz != ph->GetNx()*ph->GetNy()*ph->GetNz() || !tr || tr[0]==0)
		return (new mglData);
	mglData re(nx,ny,nz), im(nx,ny,nz);
	const mglData *da=dynamic_cast<const mglData *>(am);
	const mglData *dp=dynamic_cast<const mglData *>(ph);
	if(da && dp)	for(long i=0;i<nx*ny*nz;i++)
	{	re.a[i] = da->a[i]*cos(dp->a[i]);
		im.a[i] = da->a[i]*sin(dp->a[i]);	}
	else	for(long i=0;i<nx*ny*nz;i++)
	{	re.a[i] = am->vthr(i)*cos(ph->vthr(i));
		im.a[i] = am->vthr(i)*sin(ph->vthr(i));	}
	return mgl_transform(&re, &im, tr);
}
//-----------------------------------------------------------------------------
HMDT mgl_transform(HCDT re, HCDT im, const char *tr)
{
	long nx = re->GetNx(), ny = re->GetNy(), nz = re->GetNz();
	if(nx*ny*nz != im->GetNx()*im->GetNy()*im->GetNz() || !tr || tr[0]==0)
		return (new mglData);
	mglData rr(re),ii(im);
	if(strchr(tr,'i') && strchr(tr,'f'))	// general case
	{
		if(tr[0]=='f')	mgl_data_fourier(&rr,&ii,"x");
		if(tr[0]=='i')	mgl_data_fourier(&rr,&ii,"xi");
		if(tr[1]=='f')	mgl_data_fourier(&rr,&ii,"y");
		if(tr[1]=='i')	mgl_data_fourier(&rr,&ii,"yi");
		if(tr[2]=='f')	mgl_data_fourier(&rr,&ii,"z");
		if(tr[2]=='i')	mgl_data_fourier(&rr,&ii,"zi");
	}
	else if(strchr(tr,'f'))	// do Fourier only once for speeding up
	{
		char str[4] = "   ";
		if(tr[0]=='f')	str[0]='x';
		if(tr[1]=='f')	str[1]='y';
		if(tr[2]=='f')	str[2]='z';
		mgl_data_fourier(&rr,&ii,str);
	}
	else if(strchr(tr,'i'))	// do Fourier only once for speeding up
	{
		char str[5] = "   i";
		if(tr[0]=='f')	str[0]='x';
		if(tr[1]=='f')	str[1]='y';
		if(tr[2]=='f')	str[2]='z';
		mgl_data_fourier(&rr,&ii,str);
	}
	else if(strchr(tr,'s'))	// do Fourier only once for speeding up
	{
		if(tr[0]=='s')	{	rr.SinFFT("x");	ii.SinFFT("x");	}
		if(tr[1]=='s')	{	rr.SinFFT("y");	ii.SinFFT("y");	}
		if(tr[2]=='s')	{	rr.SinFFT("z");	ii.SinFFT("z");	}
	}
	else if(strchr(tr,'c'))	// do Fourier only once for speeding up
	{
		if(tr[0]=='c')	{	rr.CosFFT("x");	ii.CosFFT("x");	}
		if(tr[1]=='c')	{	rr.CosFFT("y");	ii.CosFFT("y");	}
		if(tr[2]=='c')	{	rr.CosFFT("z");	ii.CosFFT("z");	}
	}
	else if(strchr(tr,'h'))	// do Fourier only once for speeding up
	{
		if(tr[0]=='h')	{	rr.Hankel("x");	ii.Hankel("x");	}
		if(tr[1]=='h')	{	rr.Hankel("y");	ii.Hankel("y");	}
		if(tr[2]=='h')	{	rr.Hankel("z");	ii.Hankel("z");	}
	}
	mglData *d = new mglData(nx, ny, nz);
	register long i;
	for(i=0;i<nx*ny*nz;i++)	d->a[i] = hypot(rr.a[i],ii.a[i]);
	return d;
}
//-----------------------------------------------------------------------------
uintptr_t mgl_transform_a_(uintptr_t *am, uintptr_t *ph, const char *tr, int l)
{	char *s=new char[l+1];	memcpy(s,tr,l);	s[l]=0;
	uintptr_t res = uintptr_t(mgl_transform_a(_DA_(am),_DA_(ph),s));
	delete []s;		return res;	}
uintptr_t mgl_transform_(uintptr_t *re, uintptr_t *im, const char *tr, int l)
{	char *s=new char[l+1];	memcpy(s,tr,l);	s[l]=0;
	uintptr_t res = uintptr_t(mgl_transform(_DA_(re),_DA_(im),s));
	delete []s;		return res;	}
//-----------------------------------------------------------------------------
void *mgl_eqmul(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i, nx=t->p[0];
	mreal *b=t->a;
	const mreal *a=t->b;
	for(i=t->id;i<t->n;i+=mglNumThr)	b[i] *= a[i%nx];
	return 0;
}
void mgl_data_mul_dat(HMDT d, HCDT a)
{
	long n, nx=d->nx, ny=d->ny, nz=d->nz;
	const mglData *b = dynamic_cast<const mglData *>(a);
	if(b)
	{
		if((b->nz==1) & (b->ny==1) & (nx==b->nx))
		{	n=nx;		mglStartThread(mgl_eqmul,0,nx*ny*nz,d->a,b->a,0,&n);	}
		else if((b->nz==1) & (ny==b->ny) & (nx==b->nx))
		{	n=nx*ny;	mglStartThread(mgl_eqmul,0,nx*ny*nz,d->a,b->a,0,&n);	}
		else if((nz==b->nz) & (ny==b->ny) & (nx==b->nx))
		{	n=nx*ny*nz;	mglStartThread(mgl_eqmul,0,nx*ny*nz,d->a,b->a,0,&n);	}
	}
	else	for(long k=0;k<ny;k++)	for(long j=0;j<ny;j++)	for(long i=0;i<nx;i++)
		d->a[i+nx*(j+ny*k)] *= a->v(i,j,k);
}
//-----------------------------------------------------------------------------
void mgl_data_mul_num(HMDT d, float a)
{
	long n=1, nx=d->nx, ny=d->ny, nz=d->nz;	mreal aa=a;
	mglStartThread(mgl_eqmul,0,nx*ny*nz,d->a,&aa,0,&n);
}
//-----------------------------------------------------------------------------
void *mgl_eqdiv(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i, nx=t->p[0];
	mreal *b=t->a;
	const mreal *a=t->b;
	for(i=t->id;i<t->n;i+=mglNumThr)	b[i] /= a[i%nx];
	return 0;
}
void mgl_data_div_dat(HMDT d, HCDT a)
{
	long n, nx=d->nx, ny=d->ny, nz=d->nz;
	const mglData *b = dynamic_cast<const mglData *>(a);
	if(b)
	{
		if((b->nz==1) & (b->ny==1) & (nx==b->nx))
		{	n=nx;		mglStartThread(mgl_eqdiv,0,nx*ny*nz,d->a,b->a,0,&n);	}
		else if((b->nz==1) & (ny==b->ny) & (nx==b->nx))
		{	n=nx*ny;	mglStartThread(mgl_eqdiv,0,nx*ny*nz,d->a,b->a,0,&n);	}
		else if((nz==b->nz) & (ny==b->ny) & (nx==b->nx))
		{	n=nx*ny*nz;	mglStartThread(mgl_eqdiv,0,nx*ny*nz,d->a,b->a,0,&n);	}
	}
	else	for(long k=0;k<ny;k++)	for(long j=0;j<ny;j++)	for(long i=0;i<nx;i++)
		d->a[i+nx*(j+ny*k)] /= a->v(i,j,k);
}
//-----------------------------------------------------------------------------
void mgl_data_div_num(HMDT d, float a)
{
	long n=1, nx=d->nx, ny=d->ny, nz=d->nz;	mreal aa=a;
	mglStartThread(mgl_eqdiv,0,nx*ny*nz,d->a,&aa,0,&n);
}
//-----------------------------------------------------------------------------
void *mgl_eqadd(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i, nx=t->p[0];
	mreal *b=t->a;
	const mreal *a=t->b;
	for(i=t->id;i<t->n;i+=mglNumThr)	b[i] += a[i%nx];
	return 0;
}
void mgl_data_add_dat(HMDT d, HCDT a)
{
	long n, nx=d->nx, ny=d->ny, nz=d->nz;
	const mglData *b = dynamic_cast<const mglData *>(a);
	if(b)
	{
		if((b->nz==1) & (b->ny==1) & (nx==b->nx))
		{	n=nx;		mglStartThread(mgl_eqadd,0,nx*ny*nz,d->a,b->a,0,&n);	}
		else if((b->nz==1) & (ny==b->ny) & (nx==b->nx))
		{	n=nx*ny;	mglStartThread(mgl_eqadd,0,nx*ny*nz,d->a,b->a,0,&n);	}
		else if((nz==b->nz) & (ny==b->ny) & (nx==b->nx))
		{	n=nx*ny*nz;	mglStartThread(mgl_eqadd,0,nx*ny*nz,d->a,b->a,0,&n);	}
	}
	else	for(long k=0;k<ny;k++)	for(long j=0;j<ny;j++)	for(long i=0;i<nx;i++)
		d->a[i+nx*(j+ny*k)] += a->v(i,j,k);
}
//-----------------------------------------------------------------------------
void mgl_data_add_num(HMDT d, float a)
{
	long n=1, nx=d->nx, ny=d->ny, nz=d->nz;	mreal aa=a;
	mglStartThread(mgl_eqadd,0,nx*ny*nz,d->a,&aa,0,&n);
}
//-----------------------------------------------------------------------------
void *mgl_eqsub(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i, nx=t->p[0];
	mreal *b=t->a;
	const mreal *a=t->b;
	for(i=t->id;i<t->n;i+=mglNumThr)	b[i] -= a[i%nx];
	return 0;
}
void mgl_data_sub_dat(HMDT d, HCDT a)
{
	long n, nx=d->nx, ny=d->ny, nz=d->nz;
	const mglData *b = dynamic_cast<const mglData *>(a);
	if(b)
	{
		if((b->nz==1) & (b->ny==1) & (nx==b->nx))
		{	n=nx;		mglStartThread(mgl_eqsub,0,nx*ny*nz,d->a,b->a,0,&n);	}
		else if((b->nz==1) & (ny==b->ny) & (nx==b->nx))
		{	n=nx*ny;	mglStartThread(mgl_eqsub,0,nx*ny*nz,d->a,b->a,0,&n);	}
		else if((nz==b->nz) & (ny==b->ny) & (nx==b->nx))
		{	n=nx*ny*nz;	mglStartThread(mgl_eqsub,0,nx*ny*nz,d->a,b->a,0,&n);	}
	}
	else	for(long k=0;k<ny;k++)	for(long j=0;j<ny;j++)	for(long i=0;i<nx;i++)
		d->a[i+nx*(j+ny*k)] -= a->v(i,j,k);
}
//-----------------------------------------------------------------------------
void mgl_data_sub_num(HMDT d, float a)
{
	long n=1, nx=d->nx, ny=d->ny, nz=d->nz;	mreal aa=a;
	mglStartThread(mgl_eqsub,0,nx*ny*nz,d->a,&aa,0,&n);
}
//-----------------------------------------------------------------------------
void mgl_data_mul_dat_(uintptr_t *d, uintptr_t *b)	{	mgl_data_mul_dat(_DT_, _DA_(b));	}
void mgl_data_div_dat_(uintptr_t *d, uintptr_t *b)	{	mgl_data_div_dat(_DT_, _DA_(b));	}
void mgl_data_add_dat_(uintptr_t *d, uintptr_t *b)	{	mgl_data_add_dat(_DT_, _DA_(b));	}
void mgl_data_sub_dat_(uintptr_t *d, uintptr_t *b)	{	mgl_data_sub_dat(_DT_, _DA_(b));	}
void mgl_data_mul_num_(uintptr_t *d, float *b)		{	mgl_data_mul_num(_DT_, *b);	}
void mgl_data_div_num_(uintptr_t *d, float *b)		{	mgl_data_div_num(_DT_, *b);	}
void mgl_data_add_num_(uintptr_t *d, float *b)		{	mgl_data_add_num(_DT_, *b);	}
void mgl_data_sub_num_(uintptr_t *d, float *b)		{	mgl_data_sub_num(_DT_, *b);	}
//-----------------------------------------------------------------------------
void mgl_hist_p(mglThreadD *t,mreal *a)
{
	register long i,j,n=t[0].p[0];
	mreal *b;
	for(i=0;i<mglNumThr;i++)
	{
		b = t[i].a;
		for(j=0;j<n;j++)	a[j] += b[j];
		delete []b;
	}
}
void *mgl_hist_1(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,k, nn=t->n, n = t->p[0];
	mreal *b=new mreal[n], *v=(mreal *)t->v;
	const mreal *a=t->b, *c=t->c;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		k = long(n*(a[i]-v[0])/(v[1]-v[0]));
		if((k>=0) & (k<n))	b[k] += c ? c[i]:1.;
	}
	t->a = b;	return 0;
}
void *mgl_hist_2(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,k, nn=t->n, n = t->p[0];
	long ns=t->p[1], nx=t->p[2], ny=t->p[3], nz=t->p[4];
	mreal *b=new mreal[n], *v=(mreal *)t->v, f,w=1, x,y,z, d=1./ns;
	const mreal *a=t->b, *c=t->c;
	bool sp = n>0;
	for(i=t->id;i<nn;i+=mglNumThr)
	{
		x = d*(i%(nx*ns));
		y = d*((i/(nx*ns))%(ny*ns));
		z = d*(i/(nx*ny*ns*ns));
		f = sp ? mglSpline3(a,nx,ny,nz,x,y,z) : mglLinear(a,nx,ny,nz,x,y,z);
		if(c)	w = sp ? mglSpline3(c,nx,ny,nz,x,y,z) : mglLinear(c,nx,ny,nz,x,y,z);
		k = long(n*(f-v[0])/(v[1]-v[0]));
		if((k>=0) & (k<n))	b[k] += w * d*d*d;
	}
	t->a = b;	return 0;
}
HMDT mgl_data_hist(HCDT dat, long n, float v1, float v2, long nsub)
{
	mglData *b=new mglData;		// NOTE: For mglData only!
	const mglData *d = dynamic_cast<const mglData *>(dat);
	if((n<2) | (v1==v2) | !d)	return b;
	mgl_data_create(b,n,1,1);
	mreal v[2]={v1,v2};
	long nx=d->nx, ny=d->ny, nz=d->nz;
	long ns=abs(nsub)+1, p[5]={n,ns,nx,ny,nz};
	if(nsub==0)	mglStartThread(mgl_hist_1,mgl_hist_p, nx*ny*nz, b->a,d->a,0,p,v);
	else	mglStartThread(mgl_hist_2,mgl_hist_p, nx*ny*nz*ns*ns*ns, b->a,d->a,0,p,v);
	return b;
}
//-----------------------------------------------------------------------------
HMDT mgl_data_hist_w(HCDT dat, HCDT weight, long n, float v1, float v2, long nsub)
{
	mglData *b=new mglData;		// NOTE: For mglData only!
	const mglData *d = dynamic_cast<const mglData *>(dat);
	const mglData *w = dynamic_cast<const mglData *>(weight);
	if((n<2) | (v1==v2) | !d | !w)	return b;
	mgl_data_create(b,n,1,1);
	mreal v[2]={v1,v2};

	long nx=d->nx, ny=d->ny, nz=d->nz;
	long ns=abs(nsub)+1, p[5]={n,ns,nx,ny,nz};
	if(nsub==0)	mglStartThread(mgl_hist_1,mgl_hist_p, nx*ny*nz, b->a,d->a,w->a,p,v);
	else	mglStartThread(mgl_hist_2,mgl_hist_p, nx*ny*nz*ns*ns*ns, b->a,d->a,w->a,p,v);
	return b;
}
//-----------------------------------------------------------------------------
uintptr_t mgl_data_hist_(uintptr_t *d, int *n, float *v1, float *v2, int *nsub)
{	return uintptr_t(mgl_data_hist(_DT_,*n,*v1,*v2,*nsub));	}
uintptr_t mgl_data_hist_w_(uintptr_t *d, uintptr_t *w, int *n, float *v1, float *v2, int *nsub)
{	return uintptr_t(mgl_data_hist_w(_DT_,_DA_(w),*n,*v1,*v2,*nsub));	}
//-----------------------------------------------------------------------------
mreal mglLinear(const mreal *a, long nx, long ny, long nz, mreal x, mreal y, mreal z)
{
	if(!a | (nx<1) | (ny<1) | (nz<1))	return 0;
	register long i0;
	long kx,ky,kz;
	mreal b=0,dx,dy,dz,b1,b0;
	if((x<0) | (y<0) | (z<0) | (x>nx-1) | (y>ny-1) | (z>nz-1))
		return 0;
	if((nz>1) & (z!=floor(z)))		// 3d interpolation
	{
		kx=long(x);	ky=long(y);	kz=long(z);
		dx = x-kx;	dy = y-ky;	dz = z-kz;

		i0 = kx+nx*(ky+ny*kz);
		b0 = a[i0]*(1-dx-dy+dx*dy) + dx*(1-dy)*a[i0+1] +
			dy*(1-dx)*a[i0+nx] + dx*dy*a[i0+nx+1];
		i0 = kx+nx*(ky+ny*(kz+1));
		b1 = a[i0]*(1-dx-dy+dx*dy) + dx*(1-dy)*a[i0+1] +
			dy*(1-dx)*a[i0+nx] + dx*dy*a[i0+nx+1];
		b = b0 + dz*(b1-b0);
	}
	else if((ny>1) & (y!=floor(y)))	// 2d interpolation
	{
		kx=long(x);	ky=long(y);
		dx = x-kx;	dy=y-ky;
		i0 = kx+nx*ky;
		b = a[i0]*(1-dx-dy+dx*dy) + dx*(1-dy)*a[i0+1] +
			dy*(1-dx)*a[i0+nx] + dx*dy*a[i0+nx+1];
	}
	else if((nx>1) & (x!=floor(x)))	// 1d interpolation
	{
		kx = long(x);
		b = a[kx] + (x-kx)*(a[kx+1]-a[kx]);
	}
	else						// no interpolation
		b = a[long(x+nx*(y+ny*z))];
	return b;
}
//-----------------------------------------------------------------------------
long mgl_idx_var;
int mgl_cmd_idx(const void *a, const void *b)
{
	mreal *aa = (mreal *)a, *bb = (mreal *)b;
	return (aa[mgl_idx_var]>bb[mgl_idx_var] ? 1:(aa[mgl_idx_var]<bb[mgl_idx_var]?-1:0) );
}
void mgl_data_sort(HMDT dat, long idx, long idy)
{
	mglData *d = dynamic_cast<mglData *>(dat);
	if(!d || ((idx>=d->nx) | (idx<0)))	return;
	bool single = ((d->nz==1) | (idy<0));
	if((idy<0) | (idy>d->ny))	idy=0;
	mgl_idx_var = idx+d->nx*idy;	// NOTE: not thread safe!!!
	if(single)	qsort(d->a, d->ny*d->nz, d->nx*sizeof(mreal), mgl_cmd_idx);
	else		qsort(d->a, d->nz, d->ny*d->nx*sizeof(mreal), mgl_cmd_idx);
}
//-----------------------------------------------------------------------------
void mgl_data_sort_(uintptr_t *d, int *idx, int *idy)
{	mgl_data_sort(_DT_,*idx,*idy);	}
//-----------------------------------------------------------------------------
