/***************************************************************************
 * fft.cpp is part of Math Graphic Library
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
#include "mgl2/datac.h"
#include "mgl2/data.h"

#if MGL_HAVE_GSL
#include <gsl/gsl_fft_complex.h>
#include <gsl/gsl_dht.h>
#include <gsl/gsl_sf.h>
#endif
//-----------------------------------------------------------------------------
void MGL_EXPORT mglStartThreadT(void *(*func)(void *), long n, void *a, double *b, const void *v, void *w, const long *p, const void *re, const void *im)
{
	if(!func)	return;
#if MGL_HAVE_PTHREAD
	if(mglNumThr<1)	mgl_set_num_thr(0);
	if(mglNumThr>1)
	{
		pthread_t *tmp=new pthread_t[mglNumThr];
		mglThreadT *par=new mglThreadT[mglNumThr];
		register long i;
		for(i=0;i<mglNumThr;i++)	// put parameters into the structure
		{	par[i].n=n;	par[i].a=a;	par[i].v=v;	par[i].w=w;	par[i].b=b;
			par[i].p=p;	par[i].re=re;	par[i].im=im;	par[i].id=i;	}
		for(i=0;i<mglNumThr;i++)	pthread_create(tmp+i, 0, func, par+i);
		for(i=0;i<mglNumThr;i++)	pthread_join(tmp[i], 0);
		delete []tmp;	delete []par;
	}
	else
#endif
	{
		mglNumThr = 1;
		mglThreadT par;
		par.n=n;	par.a=a;	par.b=b;	par.v=v;	par.w=w;
		par.p=p;	par.re=re;	par.im=im;	par.id=0;
		func(&par);
	}
}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void* mgl_fftx(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,nx=t->p[0],ny=t->p[1],nz=t->p[2];
	gsl_fft_direction how=t->p[3]?backward:forward;
	double *a = t->b;
	const gsl_fft_complex_wavetable *wt = (const gsl_fft_complex_wavetable *)t->v;
	gsl_fft_complex_workspace **ws = (gsl_fft_complex_workspace **)t->w;
	for(i=t->id;i<t->n;i+=mglNumThr)
		gsl_fft_complex_transform(a+2*nx*i, 1, nx, wt, ws[t->id], how);
	return 0;
}
MGL_NO_EXPORT void* mgl_ffty(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,nx=t->p[0],ny=t->p[1],nz=t->p[2];
	gsl_fft_direction how=t->p[3]?backward:forward;
	double *a = t->b;
	const gsl_fft_complex_wavetable *wt = (const gsl_fft_complex_wavetable *)t->v;
	gsl_fft_complex_workspace **ws = (gsl_fft_complex_workspace **)t->w;
	for(i=t->id;i<t->n;i+=mglNumThr)
		gsl_fft_complex_transform(a+2*(i%nx)+2*nx*ny*(i/nx), nx, ny, wt, ws[t->id], how);
	return 0;
}
MGL_NO_EXPORT void* mgl_fftz(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,nx=t->p[0],ny=t->p[1],nz=t->p[2];
	gsl_fft_direction how=t->p[3]?backward:forward;
	double *a = t->b;
	const gsl_fft_complex_wavetable *wt = (const gsl_fft_complex_wavetable *)t->v;
	gsl_fft_complex_workspace **ws = (gsl_fft_complex_workspace **)t->w;
	for(i=t->id;i<t->n;i+=mglNumThr)
		gsl_fft_complex_transform(a+2*i, nx*ny, nz, wt, ws[t->id], how);
	return 0;
}
void MGL_EXPORT mgl_datac_fft(HADT d, const char *dir)
{
#if MGL_HAVE_GSL
	if(!dir || *dir==0)	return;
	long nx = d->nx, ny = d->ny, nz = d->nz;
	if(mglNumThr<1)	mgl_set_num_thr(0);	// manually set number of threads
	gsl_fft_complex_wavetable *wt=0;
	gsl_fft_complex_workspace *ws[mglNumThr];
	long par[4]={nx,ny,nz,strchr(dir,'i')!=0}, i;
#if MGL_USE_DOUBLE
	double *a = (double *)(d->a);
#else
	double *a = new double[2*nx*ny*nz];	// manually convert to double
	for(i=0;i<nx*ny*nz;i++)
	{	a[2*i] = real(d->a[i]);	a[2*i+1] = imag(d->a[i]);	}
#endif
	if(strchr(dir,'x') && nx>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(nx);
		for(i=0;i<mglNumThr;i++)	ws[i] = gsl_fft_complex_workspace_alloc(nx);
		mglStartThreadT(mgl_fftx,ny*nz,0,a,wt,ws,par);
	}
	if(strchr(dir,'y') && ny>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(ny);
		for(i=0;i<mglNumThr;i++)	ws[i] = gsl_fft_complex_workspace_alloc(ny);
		mglStartThreadT(mgl_ffty,nx*nz,0,a,wt,ws,par);
	}
	if(strchr(dir,'z') && nz>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(nz);
		for(i=0;i<mglNumThr;i++)	ws[i] = gsl_fft_complex_workspace_alloc(nz);
		mglStartThreadT(mgl_fftz,nx*ny,0,a,wt,ws,par);
	}
	if(wt)
	{
		for(i=0;i<mglNumThr;i++)	gsl_fft_complex_workspace_free(ws[i]);
		gsl_fft_complex_wavetable_free(wt);
	}
#if !MGL_USE_DOUBLE
	for(i=0;i<nx*ny*nz;i++)	d->a[i] = dual(a[2*i], a[2*i+1]);
	delete []a;
#endif
#endif
}
void MGL_EXPORT mgl_datac_fft_(uintptr_t *d, const char *dir, int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_datac_fft(_DC_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_fourier(HMDT re, HMDT im, const char *dir)
{
#if MGL_HAVE_GSL
	if(!dir || *dir==0)	return;
	long nx = re->nx, ny = re->ny, nz = re->nz;
	if(nx*ny*nz != im->nx*im->ny*im->nz || !dir || dir[0]==0)	return;
	if(mglNumThr<1)	mgl_set_num_thr(0);	// manually set number of threads
	gsl_fft_complex_wavetable *wt=0;
	gsl_fft_complex_workspace *ws[mglNumThr];
	long par[4]={nx,ny,nz,strchr(dir,'i')!=0}, i;
	double *a = new double[2*nx*ny*nz];
	for(i=0;i<nx*ny*nz;i++)
	{	a[2*i] = re->a[i];	a[2*i+1] = im->a[i];	}
	if(strchr(dir,'x') && nx>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(nx);
		for(i=0;i<mglNumThr;i++)	ws[i] = gsl_fft_complex_workspace_alloc(nx);
		mglStartThreadT(mgl_fftx,ny*nz,0,a,wt,ws,par);
	}
	if(strchr(dir,'y') && ny>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(ny);
		for(i=0;i<mglNumThr;i++)	ws[i] = gsl_fft_complex_workspace_alloc(ny);
		mglStartThreadT(mgl_ffty,nx*nz,0,a,wt,ws,par);
	}
	if(strchr(dir,'z') && nz>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(nz);
		for(i=0;i<mglNumThr;i++)	ws[i] = gsl_fft_complex_workspace_alloc(nz);
		mglStartThreadT(mgl_fftz,nx*ny,0,a,wt,ws,par);
	}
	if(wt)
	{
		for(i=0;i<mglNumThr;i++)	gsl_fft_complex_workspace_free(ws[i]);
		gsl_fft_complex_wavetable_free(wt);
	}
	for(i=0;i<nx*ny*nz;i++)
	{	re->a[i] = a[2*i];	im->a[i] = a[2*i+1];	}
	delete []a;
#endif
}
void MGL_EXPORT mgl_data_fourier_(uintptr_t *re, uintptr_t *im, const char *dir, int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_fourier(_DM_(re),_DM_(im),s);	delete []s;	}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void* mgl_stfa1(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,k,ii,i0,mx=t->p[0],my=t->p[1],mz=t->p[2],dn=t->p[3],dd=dn/2,ny=t->p[4];
	double *a = t->b+4*dn*t->id,ff;
	dual *d = (dual*)t->a;
	HCDT re = (HCDT)t->re, im = (HCDT)t->im;
	const gsl_fft_complex_wavetable *wt = (const gsl_fft_complex_wavetable *)t->v;
	gsl_fft_complex_workspace **ws = (gsl_fft_complex_workspace **)t->w;
	for(ii=t->id;ii<t->n;ii+=mglNumThr)
	{
		i = ii%mx;	j = ii/mx;
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
		gsl_fft_complex_forward(a, 1, 2*dn, wt, ws[t->id]);
		for(k=0;k<dd;k++)
		{
			i0 = i+mx*(j+mz*k);
			d[i0+mx*mz*dd] = hypot(a[4*k],a[4*k+1])/dn;
			d[i0] = hypot(a[4*k+2*dn],a[4*k+2*dn+1])/dn;
		}
	}
	return 0;
}
MGL_NO_EXPORT void* mgl_stfa2(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,k,ii,i0,mx=t->p[0],my=t->p[1],mz=t->p[2],dn=t->p[3],dd=dn/2,nx=t->p[4];
	double *a = t->b+4*dn*t->id,ff;
	dual *d = (dual*)t->a;
	HCDT re = (HCDT)t->re, im = (HCDT)t->im;
	const gsl_fft_complex_wavetable *wt = (const gsl_fft_complex_wavetable *)t->v;
	gsl_fft_complex_workspace **ws = (gsl_fft_complex_workspace **)t->w;
	for(ii=t->id;ii<t->n;ii+=mglNumThr)
	{
		i = ii%my;	j = ii/mx;
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
		gsl_fft_complex_forward(a, 1, 2*dn, wt, ws[t->id]);
		for(k=0;k<dd;k++)
		{
			i0 = i+my*(k+mx*j);
			d[i0+dd*my] = hypot(a[4*k],a[4*k+1])/dn;
			d[i0] = hypot(a[4*k+2*dn],a[4*k+2*dn+1])/dn;
		}
	}
	return 0;
}
HMDT MGL_EXPORT mgl_data_stfa(HCDT re, HCDT im, long dn, char dir)
{
	mglData *d=new mglData;
#if MGL_HAVE_GSL
	if(dn<2)	return d;
	dn = 2*(dn/2);
	long nx = re->GetNx(), ny = re->GetNy();
	if(nx*ny!=im->GetNx()*im->GetNy())	return d;
	register long i,j,k,i0,dd=dn/2;
	if(mglNumThr<1)	mgl_set_num_thr(0);	// manually set number of threads
	double *a = new double[4*dn*mglNumThr],ff;
	gsl_fft_complex_wavetable *wt = gsl_fft_complex_wavetable_alloc(2*dn);
	gsl_fft_complex_workspace *ws[mglNumThr];
	for(i=0;i<mglNumThr;i++)	ws[i] = gsl_fft_complex_workspace_alloc(nx);
	long mx,my,mz;
	if(dir=='y')
	{
		mx = nx;	my = dn;	mz = ny/dn;
		mgl_data_create(d, mx, mz, my);
		long par[5]={mx,my,mz,dn,ny};
		mglStartThreadT(mgl_stfa1,mx*mz,d->a,a,wt,ws,par,re,im);
	}
	else
	{
		mx = dn;	my = nx/dn;	mz = ny;
		mgl_data_create(d, my, mx, mz);
		long par[5]={mx,my,mz,dn,nx};
		mglStartThreadT(mgl_stfa2,my*mz,d->a,a,wt,ws,par,re,im);
	}
	delete []a;
	for(i=0;i<mglNumThr;i++)	gsl_fft_complex_workspace_free(ws[i]);
	gsl_fft_complex_wavetable_free(wt);
#endif
	return d;
}
uintptr_t MGL_EXPORT mgl_data_stfa_(uintptr_t *re, uintptr_t *im, int *dn, char *dir, int)
{	return uintptr_t(mgl_data_stfa(_DA_(re),_DA_(im),*dn,*dir));	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_fill_sample(HMDT d, const char *how)
{
	if(!how || *how==0)	return;
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
void MGL_EXPORT mgl_data_fill_sample_(uintptr_t *d, const char *how,int l)
{	char *s=new char[l+1];	memcpy(s,how,l);	s[l]=0;
	mgl_data_fill_sample(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void* mgl_cosx(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,k,nx=t->p[0],ny=t->p[1],nz=t->p[2];
	double *b = t->b+4*nx*t->id;
	mreal *a = (mreal*)t->a;
	const gsl_fft_complex_wavetable *wt = (const gsl_fft_complex_wavetable *)t->v;
	gsl_fft_complex_workspace **ws = (gsl_fft_complex_workspace **)t->w;
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		k = i*nx;	memset(b,0,4*nx*sizeof(double));	b[0] = b[2*nx] = a[k];
		for(j=1;j<nx;j++)	{	b[2*j] = b[4*nx-2*j] = a[k+j];	}
		gsl_fft_complex_transform(b, 1, 2*nx, wt, ws[t->id], forward);
		for(j=0;j<nx;j++)	a[k+j] = b[2*j]/sqrt(2.*nx);
	}
	return 0;
}
MGL_NO_EXPORT void* mgl_cosy(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long ii,i,j,k,nx=t->p[0],ny=t->p[1],nz=t->p[2];
	double *b = t->b+4*ny*t->id;
	mreal *a = (mreal*)t->a;
	const gsl_fft_complex_wavetable *wt = (const gsl_fft_complex_wavetable *)t->v;
	gsl_fft_complex_workspace **ws = (gsl_fft_complex_workspace **)t->w;
	for(ii=t->id;ii<t->n;ii+=mglNumThr)
	{
		i = ii%nx;	k = ii/nx;
		memset(b,0,4*ny*sizeof(double));	b[0] = b[2*ny] = a[i+nx*ny*k];
		for(j=1;j<ny;j++)	{	b[2*j] = a[i+nx*(ny*k+j)];	b[4*ny-2*j] = a[i+nx*(ny*k+j)];	}
		gsl_fft_complex_transform(b, 1, 2*ny, wt, ws[t->id], forward);
		for(j=0;j<ny;j++)	a[i+nx*(ny*k+j)] = b[2*j]/sqrt(2.*ny);
	}
	return 0;
}
MGL_NO_EXPORT void* mgl_cosz(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,nx=t->p[0],ny=t->p[1],nz=t->p[2],k=nx*ny;
	double *b = t->b+4*nz*t->id;
	mreal *a = (mreal*)t->a;
	const gsl_fft_complex_wavetable *wt = (const gsl_fft_complex_wavetable *)t->v;
	gsl_fft_complex_workspace **ws = (gsl_fft_complex_workspace **)t->w;
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		memset(b,0,4*nz*sizeof(double));	b[0] = b[2*nz] = a[i];
		for(j=1;j<nz;j++)	{	b[2*j] = a[i+k*j];	b[4*nz-2*j] = a[i+k*j];	}
		gsl_fft_complex_transform(b, 1, 2*nz, wt, ws[t->id], forward);
		for(j=0;j<nz;j++)	a[i+k*j] = b[2*j]/sqrt(2.*nz);
	}
	return 0;
}
void MGL_EXPORT mgl_data_cosfft(HMDT d, const char *dir)
{
#if MGL_HAVE_GSL
	if(!dir || *dir==0)	return;
	double *b = 0;
	if(mglNumThr<1)	mgl_set_num_thr(0);	// manually set number of threads
	gsl_fft_complex_wavetable *wt=0;
	gsl_fft_complex_workspace *ws[mglNumThr];
	long nx=d->nx, ny=d->ny, nz=d->nz;
	long par[3]={nx,ny,nz}, i;
	if(strchr(dir,'x') && nx>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(2*nx);
		for(i=0;i<mglNumThr;i++)	ws[i] = gsl_fft_complex_workspace_alloc(2*nx);
		b = new double[4*nx*mglNumThr];
		mglStartThreadT(mgl_cosx,ny*nz,d->a,b,wt,ws,par);
	}
	if(strchr(dir,'y') && ny>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(2*ny);
		for(i=0;i<mglNumThr;i++)	ws[i] = gsl_fft_complex_workspace_alloc(2*ny);
		b = new double[4*ny*mglNumThr];
		mglStartThreadT(mgl_cosy,nx*nz,d->a,b,wt,ws,par);
	}
	if(strchr(dir,'z') && nz>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(2*nz);
		for(i=0;i<mglNumThr;i++)	ws[i] = gsl_fft_complex_workspace_alloc(2*nz);
		b = new double[4*nz*mglNumThr];
		mglStartThreadT(mgl_cosz,nx*ny,d->a,b,wt,ws,par);
	}
	if(b)
	{	for(i=0;i<mglNumThr;i++)	gsl_fft_complex_workspace_free(ws[i]);
		delete []b;	gsl_fft_complex_wavetable_free(wt);	}
#endif
}
void MGL_EXPORT mgl_data_cosfft_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_cosfft(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void* mgl_sinx(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,k,nx=t->p[0],ny=t->p[1],nz=t->p[2];
	double *b = t->b+4*nx*t->id;
	mreal *a = (mreal*)t->a;
	const gsl_fft_complex_wavetable *wt = (const gsl_fft_complex_wavetable *)t->v;
	gsl_fft_complex_workspace **ws = (gsl_fft_complex_workspace **)t->w;
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		k = i*nx;	memset(b,0,4*nx*sizeof(double));	//b[0] = b[2*nx] = 0;
		for(j=1;j<nx;j++)	{	b[2*j] = a[k+j];	b[4*nx-2*j] = -a[k+j];	}
		gsl_fft_complex_transform(b, 1, 2*nx, wt, ws[t->id], forward);
		for(j=0;j<nx;j++)	a[k+j] = -b[2*j+1]/sqrt(2.*nx);
	}
	return 0;
}
MGL_NO_EXPORT void* mgl_siny(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long ii,i,j,k,nx=t->p[0],ny=t->p[1],nz=t->p[2];
	double *b = t->b+4*ny*t->id;
	mreal *a = (mreal*)t->a;
	const gsl_fft_complex_wavetable *wt = (const gsl_fft_complex_wavetable *)t->v;
	gsl_fft_complex_workspace **ws = (gsl_fft_complex_workspace **)t->w;
	for(ii=t->id;ii<t->n;ii+=mglNumThr)
	{
		i = ii%nx;	k = ii/nx;	memset(b,0,4*ny*sizeof(double));	//b[0] = b[2*ny] = 0;
		for(j=1;j<ny;j++)	{	b[2*j] = a[i+nx*(ny*k+j)];	b[4*ny-2*j] = -a[i+nx*(ny*k+j)];	}
		gsl_fft_complex_transform(b, 1, 2*ny, wt, ws[t->id], forward);
		for(j=0;j<ny;j++)	a[i+nx*(ny*k+j)] = -b[2*j+1]/sqrt(2.*ny);
	}
	return 0;
}
MGL_NO_EXPORT void* mgl_sinz(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,nx=t->p[0],ny=t->p[1],nz=t->p[2],k=nx*ny;
	double *b = t->b+4*nz*t->id;
	mreal *a = (mreal*)t->a;
	const gsl_fft_complex_wavetable *wt = (const gsl_fft_complex_wavetable *)t->v;
	gsl_fft_complex_workspace **ws = (gsl_fft_complex_workspace **)t->w;
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		memset(b,0,4*nz*sizeof(double));	//b[0] = b[2*nx] = 0;
		for(j=1;j<nz;j++)	{	b[2*j] = a[i+k*j];	b[4*nz-2*j] = -a[i+k*j];	}
		gsl_fft_complex_transform(b, 1, 2*nz, wt, ws[t->id], forward);
		for(j=0;j<nz;j++)	a[i+k*j] = -b[2*j+1]/sqrt(2.*nz);
	}
	return 0;
}
void MGL_EXPORT mgl_data_sinfft(HMDT d, const char *dir)
{
#if MGL_HAVE_GSL
	if(!dir || *dir==0)	return;
	double *b = 0;
	if(mglNumThr<1)	mgl_set_num_thr(0);	// manually set number of threads
	gsl_fft_complex_wavetable *wt=0;
	gsl_fft_complex_workspace *ws[mglNumThr];
	long nx=d->nx, ny=d->ny, nz=d->nz;
	long par[3]={nx,ny,nz}, i;
	if(strchr(dir,'x') && nx>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(2*nx);
		for(i=0;i<mglNumThr;i++)	ws[i] = gsl_fft_complex_workspace_alloc(2*nx);
		b = new double[4*nx*mglNumThr];
		mglStartThreadT(mgl_sinx,ny*nz,d->a,b,wt,ws,par);
	}
	if(strchr(dir,'y') && ny>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(2*ny);
		for(i=0;i<mglNumThr;i++)	ws[i] = gsl_fft_complex_workspace_alloc(2*ny);
		b = new double[4*ny*mglNumThr];
		mglStartThreadT(mgl_siny,nx*nz,d->a,b,wt,ws,par);
	}
	if(strchr(dir,'z') && nz>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(2*nz);
		for(i=0;i<mglNumThr;i++)	ws[i] = gsl_fft_complex_workspace_alloc(2*nz);
		b = new double[4*nz*mglNumThr];
		mglStartThreadT(mgl_sinz,nx*ny,d->a,b,wt,ws,par);
	}
	if(b)
	{	for(i=0;i<mglNumThr;i++)	gsl_fft_complex_workspace_free(ws[i]);
		delete []b;	gsl_fft_complex_wavetable_free(wt);	}
#endif
}
void MGL_EXPORT mgl_data_sinfft_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_sinfft(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
HMDT MGL_EXPORT mgl_transform_a(HCDT am, HCDT ph, const char *tr)
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
HMDT MGL_EXPORT mgl_transform(HCDT re, HCDT im, const char *tr)
{
	if(!tr || *tr==0)	return 0;
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
		if(tr[0]=='i')	str[0]='x';
		if(tr[1]=='i')	str[1]='y';
		if(tr[2]=='i')	str[2]='z';
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
uintptr_t MGL_EXPORT mgl_transform_a_(uintptr_t *am, uintptr_t *ph, const char *tr, int l)
{	char *s=new char[l+1];	memcpy(s,tr,l);	s[l]=0;
	uintptr_t res = uintptr_t(mgl_transform_a(_DA_(am),_DA_(ph),s));
	delete []s;		return res;	}
uintptr_t MGL_EXPORT mgl_transform_(uintptr_t *re, uintptr_t *im, const char *tr, int l)
{	char *s=new char[l+1];	memcpy(s,tr,l);	s[l]=0;
	uintptr_t res = uintptr_t(mgl_transform(_DA_(re),_DA_(im),s));
	delete []s;		return res;	}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void* mgl_envx(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,nx=t->p[0],ny=t->p[1],nz=t->p[2];
	double *b = t->b+2*nx*t->id;
	mreal *a = (mreal*)t->a;
	const gsl_fft_complex_wavetable *wt = (const gsl_fft_complex_wavetable *)t->v;
	gsl_fft_complex_workspace **ws = (gsl_fft_complex_workspace **)t->w;
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		for(j=0;j<nx;j++)	{	b[2*j] = a[j+i*nx];	b[2*j+1] = 0;	}
		gsl_fft_complex_transform(b, 1, nx, wt, ws[t->id], forward);
		for(j=0;j<nx;j++)
		{	b[j] /= nx/2.;	b[j+nx] = 0;	}
		gsl_fft_complex_transform(b, 1, nx, wt, ws[t->id], backward);
		for(j=0;j<nx;j++)	a[j+i*nx] = hypot(b[2*j], b[2*j+1]);
	}
	return 0;
}
MGL_NO_EXPORT void* mgl_envy(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,nx=t->p[0],ny=t->p[1],nz=t->p[2];
	double *b = t->b+2*ny*t->id;
	mreal *a = (mreal*)t->a;
	const gsl_fft_complex_wavetable *wt = (const gsl_fft_complex_wavetable *)t->v;
	gsl_fft_complex_workspace **ws = (gsl_fft_complex_workspace **)t->w;
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		for(j=0;j<ny;j++)	{	b[2*j] = a[(i%nx)+nx*(j+ny*(i/nx))];	b[2*j+1] = 0;	}
		gsl_fft_complex_transform(b, 1, ny, wt, ws[t->id], forward);
		for(j=0;j<ny;j++)
		{	b[j] /= ny/2.;	b[j+ny] = 0;	}
		gsl_fft_complex_transform(b, 1, ny, wt, ws[t->id], backward);
		for(j=0;j<ny;j++)	a[(i%nx)+nx*(j+ny*(i/nx))] = hypot(b[2*j], b[2*j+1]);
	}
	return 0;
}
MGL_NO_EXPORT void* mgl_envz(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,nx=t->p[0],ny=t->p[1],nz=t->p[2],k=nx*ny;
	double *b = t->b+2*nz*t->id;
	mreal *a = (mreal*)t->a;
	const gsl_fft_complex_wavetable *wt = (const gsl_fft_complex_wavetable *)t->v;
	gsl_fft_complex_workspace **ws = (gsl_fft_complex_workspace **)t->w;
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		for(j=0;j<nz;j++)	{	b[2*j] = a[j*k+i];	b[2*j+1] = 0;	}
		gsl_fft_complex_transform(b, 1, nz, wt, ws[t->id], forward);
		for(j=0;j<nz;j++)
		{	b[j] /= nz/2.;	b[j+nz] = 0;	}
		gsl_fft_complex_transform(b, 1, nz, wt, ws[t->id], backward);
		for(j=0;j<nz;j++)	a[j*k+i] = hypot(b[2*j], b[2*j+1]);
	}
	return 0;
}
void MGL_EXPORT mgl_data_envelop(HMDT d, char dir)
{
#if MGL_HAVE_GSL
	register long i;
	long nx=d->nx,ny=d->ny,nz=d->nz,par[3]={nx,ny,nz};
	if(mglNumThr<1)	mgl_set_num_thr(0);	// manually set number of threads
	gsl_fft_complex_wavetable *wt=0;
	gsl_fft_complex_workspace *ws[mglNumThr];
	double *b = 0;
	if(dir=='x' && nx>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(nx);
		for(i=0;i<mglNumThr;i++)	ws[i] = gsl_fft_complex_workspace_alloc(nx);
		b = new double[2*nx*mglNumThr];
		mglStartThreadT(mgl_envx,ny*nz,d->a,b,wt,ws,par);
	}
	if(dir=='y' && ny>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(ny);
		for(i=0;i<mglNumThr;i++)	ws[i] = gsl_fft_complex_workspace_alloc(ny);
		b = new double[2*ny*mglNumThr];
		mglStartThreadT(mgl_envy,nx*nz,d->a,b,wt,ws,par);
	}
	if(dir=='z' && nz>1)
	{
		wt = gsl_fft_complex_wavetable_alloc(nz);
		for(i=0;i<mglNumThr;i++)	ws[i] = gsl_fft_complex_workspace_alloc(nz);
		b = new double[2*nz*mglNumThr];
		mglStartThreadT(mgl_envz,nx*ny,d->a,b,wt,ws,par);
	}
	for(i=0;i<nx*ny*nz;i++)	d->a[i] = hypot(b[2*i], b[2*i+1]);
	if(b)
	{	for(i=0;i<mglNumThr;i++)	gsl_fft_complex_workspace_free(ws[i]);
		delete []b;	gsl_fft_complex_wavetable_free(wt);	}
#endif
}
void MGL_EXPORT mgl_data_envelop_(uintptr_t *d, const char *dir, int)
{	mgl_data_envelop(_DT_,*dir);	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_datac_hankel(HADT d, const char *dir)
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
void MGL_EXPORT mgl_datac_hankel_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_datac_hankel(_DC_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_hankel(HMDT d, const char *dir)
{
#if MGL_HAVE_GSL
	if(!dir || *dir==0)	return;
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
void MGL_EXPORT mgl_data_hankel_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_hankel(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
