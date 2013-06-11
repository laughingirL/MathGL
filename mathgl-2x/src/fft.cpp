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
void MGL_EXPORT mglStartThreadT(void *(*func)(void *), long n, void *a, double *b, const void *v, void **w, const long *p, const void *re, const void *im)
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
struct mglFFTdata
{
	int thx,thy,thz;
	long wnx,wny,wnz;		// sizes for FFT
	long hnx,hny,hnz;		// sizes for Hankel
	void *wtx,*wty,*wtz;	// tables for FFT
	void **wsx,**wsy,**wsz;	// spaces for FFT
	void *htx,*hty,*htz;	// tables for Hankel
	mglFFTdata()	{	memset(this,0,sizeof(mglFFTdata));	}
	~mglFFTdata()	{	Clear();	}
	void Clear()
	{
		if(wnx)	{	wnx=0;	mgl_fft_free(wtx,wsx,thx);	delete []wsx;	}
		if(wny)	{	wny=0;	mgl_fft_free(wty,wsy,thy);	delete []wsy;	}
		if(wnz)	{	wnz=0;	mgl_fft_free(wtz,wsz,thz);	delete []wsz;	}
#if MGL_HAVE_GSL
		if(hnx)	{	hnx=0;	gsl_dht_free((gsl_dht*)htx);	}
		if(hny)	{	hny=0;	gsl_dht_free((gsl_dht*)hty);	}
		if(hnz)	{	hnz=0;	gsl_dht_free((gsl_dht*)htz);	}
#endif
	}
} mgl_fft_data;
void MGL_EXPORT mgl_clear_fft()	{	mgl_fft_data.Clear();	}
//-----------------------------------------------------------------------------
MGL_EXPORT void *mgl_fft_alloc(long n, void **space, long nthr)
{
#if MGL_HAVE_GSL
	for(long i=0;i<nthr;i++)	space[i] = gsl_fft_complex_workspace_alloc(n);
	return gsl_fft_complex_wavetable_alloc(n);
#else
	register long i,j;
	for(i=0;i<nthr;i++)	space[i] = new double[2*n];
	double *c = new double[2*n*n];
	for(i=0;i<n;i++)	for(j=0;j<n;j++)
	{	c[2*(i+n*j)]=cos(2*M_PI*i*j/n);	c[2*(i+n*j)+1]=-sin(2*M_PI*i*j/n);	}
	return c;
#endif
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_fft_free(void *wt, void **ws, long nthr)
{
#if MGL_HAVE_GSL
	for(long i=0;i<nthr;i++)	gsl_fft_complex_workspace_free((gsl_fft_complex_workspace*)(ws[i]));
	gsl_fft_complex_wavetable_free((gsl_fft_complex_wavetable*)wt);
#else
	delete []((double*)wt);
	for(long i=0;i<nthr;i++)	delete []((double*)(ws[i]));
#endif
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_fft(double *x, long s, long n, const void *wt, void *ws, int inv)
{
#if MGL_HAVE_GSL
gsl_fft_complex_transform(x, s, n, (const gsl_fft_complex_wavetable*)wt, (gsl_fft_complex_workspace*)ws, inv?backward:forward);
#else	// NOTE this is VERY slow!
	const double *c = (const double *)wt;
	double *d = (double *)ws, f = inv?1./n:1;
	memset(d,0,2*n*sizeof(double));
	register long i,j,ii,jj;
	if(inv)	for(i=0;i<n;i++)	for(j=0;j<n;j++)
	{
		ii = 2*(i+n*j);	jj = 2*j*s;
		d[2*i] 	+= x[jj]*c[ii]+x[jj+1]*c[ii+1];
		d[2*i+1]+= x[jj+1]*c[ii]-x[jj]*c[ii+1];
	}
	else	for(i=0;i<n;i++)	for(j=0;j<n;j++)
	{
		ii = 2*(i+n*j);	jj = 2*j*s;
		d[2*i] 	+= x[jj]*c[ii]-x[jj+1]*c[ii+1];
		d[2*i+1]+= x[jj+1]*c[ii]+x[jj]*c[ii+1];
	}
	for(j=0;j<n;j++)
	{	jj = 2*j*s;	x[jj] = d[2*j]*f;	x[jj+1] = d[2*j+1]*f;	}
#endif
}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void* mgl_fftx(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,nx=t->p[0];
	for(i=t->id;i<t->n;i+=mglNumThr)
		mgl_fft(t->b+2*nx*i, 1, nx, t->v, t->w[t->id], t->p[3]);
	return 0;
}
MGL_NO_EXPORT void* mgl_ffty(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,nx=t->p[0],ny=t->p[1];
	for(i=t->id;i<t->n;i+=mglNumThr)
		mgl_fft(t->b+2*(i%nx)+2*nx*ny*(i/nx), nx, ny, t->v, t->w[t->id], t->p[3]);
	return 0;
}
MGL_NO_EXPORT void* mgl_fftz(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,nx=t->p[0],ny=t->p[1],nz=t->p[2];
	for(i=t->id;i<t->n;i+=mglNumThr)
		mgl_fft(t->b+2*i, nx*ny, nz, t->v, t->w[t->id], t->p[3]);
	return 0;
}
void MGL_EXPORT mgl_datac_fft(HADT d, const char *dir)
{
	if(!dir || *dir==0)	return;
	long nx = d->nx, ny = d->ny, nz = d->nz;
	if(mglNumThr<1)	mgl_set_num_thr(0);	// manually set number of threads
	void *wt=0, **ws=0;
	bool clear=false;
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
		if(mgl_fft_data.thx==mglNumThr && mgl_fft_data.wnx==nx)
		{	ws = mgl_fft_data.wsx;	wt = mgl_fft_data.wtx;	}
		else
		{	ws = new void*[mglNumThr];	clear = true;
			wt = mgl_fft_alloc(nx,ws,mglNumThr);	}
		mglStartThreadT(mgl_fftx,ny*nz,0,a,wt,ws,par);
		if(mgl_fft_data.wnx==0)
		{	mgl_fft_data.wsx = ws;	mgl_fft_data.wtx = wt;	clear = false;
			mgl_fft_data.thx=mglNumThr;	mgl_fft_data.wnx=nx;	}
	}
	if(strchr(dir,'y') && ny>1)
	{
		if(mgl_fft_data.thy==mglNumThr && mgl_fft_data.wny==ny)
		{	ws = mgl_fft_data.wsy;	wt = mgl_fft_data.wty;	}
		else
		{	ws = new void*[mglNumThr];	clear = true;
			wt = mgl_fft_alloc(ny,ws,mglNumThr);	}
		mglStartThreadT(mgl_ffty,nx*nz,0,a,wt,ws,par);
		if(mgl_fft_data.wny==0)
		{	mgl_fft_data.wsy = ws;	mgl_fft_data.wty = wt;	clear = false;
			mgl_fft_data.thy=mglNumThr;	mgl_fft_data.wny=ny;	}
	}
	if(strchr(dir,'z') && nz>1)
	{
		if(mgl_fft_data.thz==mglNumThr && mgl_fft_data.wnz==nz)
		{	ws = mgl_fft_data.wsz;	wt = mgl_fft_data.wtz;	}
		else
		{	ws = new void*[mglNumThr];	clear = true;
			wt = mgl_fft_alloc(nz,ws,mglNumThr);	}
		mglStartThreadT(mgl_fftz,nx*ny,0,a,wt,ws,par);
		if(mgl_fft_data.wnz==0)
		{	mgl_fft_data.wsz = ws;	mgl_fft_data.wtz = wt;	clear = false;
			mgl_fft_data.thz=mglNumThr;	mgl_fft_data.wnz=nz;	}
	}
	if(clear)	{	mgl_fft_free(wt,ws,mglNumThr);	delete []ws;	}
#if !MGL_USE_DOUBLE
	for(i=0;i<nx*ny*nz;i++)	d->a[i] = dual(a[2*i], a[2*i+1]);
	delete []a;
#endif
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_data_fourier(HMDT re, HMDT im, const char *dir)
{
	if(!dir || *dir==0)	return;
	long nx = re->nx, ny = re->ny, nz = re->nz;
	if(nx*ny*nz != im->nx*im->ny*im->nz || !dir || dir[0]==0)	return;
	if(mglNumThr<1)	mgl_set_num_thr(0);	// manually set number of threads
	bool clear=false;
	void *wt=0, **ws=0;
	long par[4]={nx,ny,nz,strchr(dir,'i')!=0}, i;
	double *a = new double[2*nx*ny*nz];
	for(i=0;i<nx*ny*nz;i++)
	{	a[2*i] = re->a[i];	a[2*i+1] = im->a[i];	}
	if(strchr(dir,'x') && nx>1)
	{
		if(mgl_fft_data.thx==mglNumThr && mgl_fft_data.wnx==nx)
		{	ws = mgl_fft_data.wsx;	wt = mgl_fft_data.wtx;	}
		else
		{	ws = new void*[mglNumThr];	clear = true;
			wt = mgl_fft_alloc(nx,ws,mglNumThr);	}
		mglStartThreadT(mgl_fftx,ny*nz,0,a,wt,ws,par);
		if(mgl_fft_data.wnx==0)
		{	mgl_fft_data.wsx = ws;	mgl_fft_data.wtx = wt;	clear = false;
			mgl_fft_data.thx=mglNumThr;	mgl_fft_data.wnx=nx;	}
	}
	if(strchr(dir,'y') && ny>1)
	{
		if(mgl_fft_data.thy==mglNumThr && mgl_fft_data.wny==ny)
		{	ws = mgl_fft_data.wsy;	wt = mgl_fft_data.wty;	}
		else
		{	ws = new void*[mglNumThr];	clear = true;
			wt = mgl_fft_alloc(ny,ws,mglNumThr);	}
		mglStartThreadT(mgl_ffty,nx*nz,0,a,wt,ws,par);
		if(mgl_fft_data.wny==0)
		{	mgl_fft_data.wsy = ws;	mgl_fft_data.wty = wt;	clear = false;
			mgl_fft_data.thy=mglNumThr;	mgl_fft_data.wny=ny;	}
	}
	if(strchr(dir,'z') && nz>1)
	{
		if(mgl_fft_data.thz==mglNumThr && mgl_fft_data.wnz==nz)
		{	ws = mgl_fft_data.wsz;	wt = mgl_fft_data.wtz;	}
		else
		{	ws = new void*[mglNumThr];	clear = true;
			wt = mgl_fft_alloc(nz,ws,mglNumThr);	}
		mglStartThreadT(mgl_fftz,nx*ny,0,a,wt,ws,par);
		if(mgl_fft_data.wnz==0)
		{	mgl_fft_data.wsz = ws;	mgl_fft_data.wtz = wt;	clear = false;
			mgl_fft_data.thz=mglNumThr;	mgl_fft_data.wnz=nz;	}
	}
	if(clear)	{	mgl_fft_free(wt,ws,mglNumThr);	delete []ws;	}
	for(i=0;i<nx*ny*nz;i++)
	{	re->a[i] = a[2*i];	im->a[i] = a[2*i+1];	}
	delete []a;
}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void* mgl_envx(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,nx=t->p[0],ny=t->p[1],nz=t->p[2];
	double *b = t->b+2*nx*t->id;
	mreal *a = (mreal*)t->a;
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		for(j=0;j<nx;j++)	{	b[2*j] = a[j+i*nx];	b[2*j+1] = 0;	}
		mgl_fft(b, 1, nx, t->v, t->w[t->id], false);
		for(j=0;j<nx;j++)
		{	b[j] /= nx/2.;	b[j+nx] = 0;	}
		mgl_fft(b, 1, nx, t->v, t->w[t->id], true);
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
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		for(j=0;j<ny;j++)	{	b[2*j] = a[(i%nx)+nx*(j+ny*(i/nx))];	b[2*j+1] = 0;	}
		mgl_fft(b, 1, ny, t->v, t->w[t->id], false);
		for(j=0;j<ny;j++)
		{	b[j] /= ny/2.;	b[j+ny] = 0;	}
		mgl_fft(b, 1, ny, t->v, t->w[t->id], true);
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
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		for(j=0;j<nz;j++)	{	b[2*j] = a[j*k+i];	b[2*j+1] = 0;	}
		mgl_fft(b, 1, nz, t->v, t->w[t->id], false);
		for(j=0;j<nz;j++)
		{	b[j] /= nz/2.;	b[j+nz] = 0;	}
		mgl_fft(b, 1, nz, t->v, t->w[t->id], true);
		for(j=0;j<nz;j++)	a[j*k+i] = hypot(b[2*j], b[2*j+1]);
	}
	return 0;
}
void MGL_EXPORT mgl_data_envelop(HMDT d, char dir)
{
	register long i;
	long nx=d->nx,ny=d->ny,nz=d->nz,par[3]={nx,ny,nz};
	if(mglNumThr<1)	mgl_set_num_thr(0);	// manually set number of threads
	bool clear=false;
	void *wt=0, **ws=0;
	double *b = 0;
	if(dir=='x' && nx>1)
	{
		if(mgl_fft_data.thx==mglNumThr && mgl_fft_data.wnx==nx)
		{	ws = mgl_fft_data.wsx;	wt = mgl_fft_data.wtx;	}
		else
		{	ws = new void*[mglNumThr];	clear = true;
			wt = mgl_fft_alloc(nx,ws,mglNumThr);	}
		b = new double[2*nx*mglNumThr];
		mglStartThreadT(mgl_envx,ny*nz,d->a,b,wt,ws,par);
		if(mgl_fft_data.wnx==0)
		{	mgl_fft_data.wsx = ws;	mgl_fft_data.wtx = wt;	clear = false;
			mgl_fft_data.thx=mglNumThr;	mgl_fft_data.wnx=nx;	}
	}
	if(dir=='y' && ny>1)
	{
		if(mgl_fft_data.thy==mglNumThr && mgl_fft_data.wny==ny)
		{	ws = mgl_fft_data.wsy;	wt = mgl_fft_data.wty;	}
		else
		{	ws = new void*[mglNumThr];	clear = true;
			wt = mgl_fft_alloc(ny,ws,mglNumThr);	}
		b = new double[2*ny*mglNumThr];
		mglStartThreadT(mgl_envy,nx*nz,d->a,b,wt,ws,par);
		if(mgl_fft_data.wny==0)
		{	mgl_fft_data.wsy = ws;	mgl_fft_data.wty = wt;	clear = false;
			mgl_fft_data.thy=mglNumThr;	mgl_fft_data.wny=ny;	}
	}
	if(dir=='z' && nz>1)
	{
		if(mgl_fft_data.thz==mglNumThr && mgl_fft_data.wnz==nz)
		{	ws = mgl_fft_data.wsz;	wt = mgl_fft_data.wtz;	}
		else
		{	ws = new void*[mglNumThr];	clear = true;
			wt = mgl_fft_alloc(nz,ws,mglNumThr);	}
		b = new double[2*nz*mglNumThr];
		mglStartThreadT(mgl_envz,nx*ny,d->a,b,wt,ws,par);
		if(mgl_fft_data.wnz==0)
		{	mgl_fft_data.wsz = ws;	mgl_fft_data.wtz = wt;	clear = false;
			mgl_fft_data.thz=mglNumThr;	mgl_fft_data.wnz=nz;	}
	}
	for(i=0;i<nx*ny*nz;i++)	d->a[i] = hypot(b[2*i], b[2*i+1]);
	if(clear)	{	mgl_fft_free(wt,ws,mglNumThr);	delete []ws;	}
	if(b)	delete []b;
}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void* mgl_stfa1(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,k,ii,i0,mx=t->p[0],my=t->p[1],mz=t->p[2],dn=t->p[3],dd=dn/2,ny=t->p[4];
	double *a = t->b+4*dn*t->id,ff;
	mreal *d = (mreal*)t->a;
	HCDT re = (HCDT)t->re, im = (HCDT)t->im;
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
		mgl_fft(a, 1, 2*dn, t->v, t->w[t->id], false);
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
	mreal *d = (mreal*)t->a;
	HCDT re = (HCDT)t->re, im = (HCDT)t->im;
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
		mgl_fft(a, 1, 2*dn, t->v, t->w[t->id], false);
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
	if(dn<2)	return 0;
	dn = 2*(dn/2);
	long nx = re->GetNx(), ny = re->GetNy();
	if(nx*ny!=im->GetNx()*im->GetNy())	return 0;
	register long i,j,k,i0,dd=dn/2;
	if(mglNumThr<1)	mgl_set_num_thr(0);	// manually set number of threads
	double *a = new double[4*dn*mglNumThr],ff;
	void **ws=new void*[mglNumThr], *wt = mgl_fft_alloc(2*dn,ws,mglNumThr);
	long mx,my,mz;
	mglData *d=new mglData;
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
	mgl_fft_free(wt,ws,mglNumThr);
	delete []ws;	delete []a;
	return d;
}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void* mgl_sinx(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,k,nx=t->p[0];
	double *b = t->b+2*nx*t->id, f=sqrt(2./nx);
	mreal *a = (mreal*)t->a;
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		k = i*nx;	memset(b,0,2*nx*sizeof(double));
		for(j=1;j<nx;j++)	b[2*j]=sin(M_PI*j/nx)*(a[j+k]+a[nx-j+k])+(a[j+k]-a[nx-j+k])*0.5;
		mgl_fft(b,1,nx,t->v,t->w[t->id],false);
		a[k]=0;	a[k+1]=b[0]*f/2;	// fill sinfft
		for(j=1;j<nx/2;j++)
		{
			a[k+2*j] = -b[2*j+1]*f;
			a[k+2*j+1] = a[k+2*j-1]+b[2*j]*f;
		}
		if(nx%2)	a[nx-1] = -b[nx]*f;
	}
	return 0;
}
MGL_NO_EXPORT void* mgl_siny(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long ii,i,j,k,nx=t->p[0],ny=t->p[1],nz=t->p[2];
	double *b = t->b+2*ny*t->id, f=sqrt(2./ny);
	mreal *a = (mreal*)t->a;
	for(ii=t->id;ii<t->n;ii+=mglNumThr)
	{
		i = ii%nx;	k = ii/nx;	memset(b,0,2*ny*sizeof(double));
		for(j=1;j<ny;j++)	b[2*j]=sin(M_PI*j/ny)*(a[i+nx*(ny*k+j)]+a[i+nx*(ny*k+ny-j)])+(a[i+nx*(ny*k+j)]-a[i+nx*(ny*k+ny-j)])*0.5;
		mgl_fft(b,1,ny,t->v,t->w[t->id],false);
		a[i+nx*ny*k]=0;	a[i+nx*(ny*k+1)]=b[0]*f/2;	// fill sinfft
		for(j=1;j<ny/2;j++)
		{
			a[i+nx*(ny*k+2*j)] = -b[2*j+1]*f;
			a[i+nx*(ny*k+2*j+1)] = a[i+nx*(ny*k+2*j-1)]+b[2*j]*f;
		}
		if(ny%2)	a[i+nx*(ny*k+ny-1)] = -b[ny]*f;
	}
	return 0;
}
MGL_NO_EXPORT void* mgl_sinz(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,nx=t->p[0],ny=t->p[1],nz=t->p[2],k=nx*ny;
	double *b = t->b+2*nz*t->id, f=sqrt(2./nz);
	mreal *a = (mreal*)t->a;
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		memset(b,0,2*nz*sizeof(double));
		for(j=1;j<nz;j++)	b[2*j]=sin(M_PI*j/nz)*(a[i+k*j]+a[i+k*(nz-j)])+(a[i+k*j]-a[i+k*(nz-j)])*0.5;
		mgl_fft(b,1,nz,t->v,t->w[t->id],false);
		a[i]=0;	a[i+k]=b[0]*f/2;	// fill sinfft
		for(j=1;j<nz/2;j++)
		{
			a[i+k*2*j] = -b[2*j+1]*f;
			a[i+k*(2*j+1)] = a[i+k*(2*j-1)]+b[2*j]*f;
		}
		if(nz%2)	a[i+k*nz-k] = -b[nz]*f;
	}
	return 0;
}
void MGL_EXPORT mgl_data_sinfft(HMDT d, const char *dir)	// use DST-1
{
	if(!dir || *dir==0)	return;
	double *b = 0;
	if(mglNumThr<1)	mgl_set_num_thr(0);	// manually set number of threads
	bool clear=false;
	void *wt=0, **ws=0;
	long nx=d->nx, ny=d->ny, nz=d->nz;
	long par[3]={nx,ny,nz}, i;
	if(strchr(dir,'x') && nx>1)
	{
		if(mgl_fft_data.thx==mglNumThr && mgl_fft_data.wnx==nx)
		{	ws = mgl_fft_data.wsx;	wt = mgl_fft_data.wtx;	}
		else
		{	ws = new void*[mglNumThr];	clear = true;
			wt = mgl_fft_alloc(nx,ws,mglNumThr);	}
		b = new double[2*nx*mglNumThr];
		mglStartThreadT(mgl_sinx,ny*nz,d->a,b,wt,ws,par);
		if(mgl_fft_data.wnx==0)
		{	mgl_fft_data.wsx = ws;	mgl_fft_data.wtx = wt;	clear = false;
			mgl_fft_data.thx=mglNumThr;	mgl_fft_data.wnx=nx;	}
	}
	if(strchr(dir,'y') && ny>1)
	{
		if(mgl_fft_data.thy==mglNumThr && mgl_fft_data.wny==ny)
		{	ws = mgl_fft_data.wsy;	wt = mgl_fft_data.wty;	}
		else
		{	ws = new void*[mglNumThr];	clear = true;
			wt = mgl_fft_alloc(ny,ws,mglNumThr);	}
		b = new double[2*ny*mglNumThr];
		mglStartThreadT(mgl_siny,nx*nz,d->a,b,wt,ws,par);
		if(mgl_fft_data.wny==0)
		{	mgl_fft_data.wsy = ws;	mgl_fft_data.wty = wt;	clear = false;
			mgl_fft_data.thy=mglNumThr;	mgl_fft_data.wny=ny;	}
	}
	if(strchr(dir,'z') && nz>1)
	{
		if(mgl_fft_data.thz==mglNumThr && mgl_fft_data.wnz==nz)
		{	ws = mgl_fft_data.wsz;	wt = mgl_fft_data.wtz;	}
		else
		{	ws = new void*[mglNumThr];	clear = true;
			wt = mgl_fft_alloc(nz,ws,mglNumThr);	}
		b = new double[2*nz*mglNumThr];
		mglStartThreadT(mgl_sinz,nx*ny,d->a,b,wt,ws,par);
		if(mgl_fft_data.wnz==0)
		{	mgl_fft_data.wsz = ws;	mgl_fft_data.wtz = wt;	clear = false;
			mgl_fft_data.thz=mglNumThr;	mgl_fft_data.wnz=nz;	}
	}
	if(clear)	{	mgl_fft_free(wt,ws,mglNumThr);	delete []ws;	}
	if(b)	delete []b;
}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void* mgl_cosx(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,k,nx=t->p[0],nn=nx-1;
	double *b = t->b+2*nx*t->id, f=sqrt(2./nn);
	mreal *a = (mreal*)t->a;
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		k = i*nx;	memset(b,0,2*nx*sizeof(double));
		for(j=0;j<nn;j++)	b[2*j]=(a[j+k]+a[nn-j+k])*0.5-sin(M_PI*j/nn)*(a[j+k]-a[nn-j+k]);
		mgl_fft(b,1,nn,t->v,t->w[t->id],false);
		double f1=0.5*(a[k]-a[nn+k]), s=-1;
		a[nn+k]=0.5*(a[k]+a[nn+k]*(nn%2?-1:1));
		for(j=1;j<nn;j++)
		{
			f1 += a[j+k]*cos(M_PI*j/nn);
			a[nn+k] += a[j+k]*s;	s = -s;
		}
		a[k]=b[0]*f;	a[1+k]=f1*f;	a[nn+k]*=f;	// fill cosfft
		for(j=1;j<nn/2;j++)
		{
			a[2*j+k] = b[2*j]*f;
			a[2*j+1+k] = a[2*j-1+k]-b[2*j+1]*f;
		}
		if(nn%2)	a[nn-1+k] = b[nn-1]*f;
	}
	return 0;
}
MGL_NO_EXPORT void* mgl_cosy(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long ii,i,j,k,nx=t->p[0],ny=t->p[1],nz=t->p[2],nn=ny-1;
	double *b = t->b+2*ny*t->id, f=sqrt(2./nn);
	mreal *a = (mreal*)t->a;
	for(ii=t->id;ii<t->n;ii+=mglNumThr)
	{
		i = ii%nx;	k = ii/nx;	memset(b,0,2*ny*sizeof(double));
		for(j=0;j<nn;j++)	b[2*j]=(a[i+nx*(ny*k+j)]+a[i+nx*(ny*k+nn-j)])*0.5-sin(M_PI*j/nn)*(a[i+nx*(ny*k+j)]-a[i+nx*(ny*k+nn-j)]);
		mgl_fft(b,1,nn,t->v,t->w[t->id],false);
		double f1=0.5*(a[i+nx*ny*k]-a[i+nx*(ny*k+nn)]), s=-1;
		a[i+nx*(ny*k+nn)]=0.5*(a[i+nx*ny*k]+a[i+nx*(ny*k+nn)]*(nn%2?-1:1));
		for(j=1;j<nn;j++)
		{
			f1 += a[i+nx*(ny*k+j)]*cos(M_PI*j/nn);
			a[i+nx*(ny*k+nn)] += a[i+nx*(ny*k+j)]*s;	s = -s;
		}
		a[i+nx*ny*k]=b[0]*f;	a[i+nx*(ny*k+1)]=f1*f;	a[i+nx*(ny*k+nn)]*=f;	// fill cosfft
		for(j=1;j<nn/2;j++)
		{
			a[i+nx*(ny*k+2*j)] = b[2*j]*f;
			a[i+nx*(ny*k+2*j+1)] = a[i+nx*(ny*k+2*j-1)]-b[2*j+1]*f;
		}
		if(nn%2)	a[i+nx*(ny*k+nn-1)] = b[nn-1]*f;
	}
	return 0;
}
MGL_NO_EXPORT void* mgl_cosz(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,nx=t->p[0],ny=t->p[1],nz=t->p[2],k=nx*ny,nn=nz-1;
	double *b = t->b+2*nz*t->id, f=sqrt(2./nn);
	mreal *a = (mreal*)t->a;
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		memset(b,0,2*nz*sizeof(double));

		for(j=0;j<nn;j++)	b[2*j]=(a[i+k*j]+a[i+k*(nn-j)])*0.5-sin(M_PI*j/nn)*(a[i+k*j]-a[i+k*(nn-j)]);
		mgl_fft(b,1,nn,t->v,t->w[t->id],false);
		double f1=0.5*(a[i]-a[i+k*nn]), s=-1;
		a[i+k*nn]=0.5*(a[i]+a[i+k*nn]*(nn%2?-1:1));
		for(j=1;j<nn;j++)
		{
			f1 += a[i+k*j]*cos(M_PI*j/nn);
			a[i+k*nn] += a[i+k*j]*s;	s = -s;
		}
		a[i]=b[0]*f;	a[i+k]=f1*f;	a[i+k*nn]*=f;	// fill cosfft
		for(j=1;j<nn/2;j++)
		{
			a[i+k*2*j] = b[2*j]*f;
			a[i+k*2*j+k] = a[i+k*2*j-k]-b[2*j+1]*f;
		}
		if(nn%2)	a[i+k*nn-k] = b[nn-1]*f;
	}
	return 0;
}
void MGL_EXPORT mgl_data_cosfft(HMDT d, const char *dir)
{
	if(!dir || *dir==0)	return;
	double *b = 0;
	if(mglNumThr<1)	mgl_set_num_thr(0);	// manually set number of threads
	bool clear=false;
	void *wt=0, **ws=0;
	long nx=d->nx, ny=d->ny, nz=d->nz;
	long par[3]={nx,ny,nz}, i;
	if(strchr(dir,'x') && nx>1)
	{
		if(mgl_fft_data.thx==mglNumThr && mgl_fft_data.wnx==nx-1)
		{	ws = mgl_fft_data.wsx;	wt = mgl_fft_data.wtx;	}
		else
		{	ws = new void*[mglNumThr];	clear = true;
			wt = mgl_fft_alloc(nx-1,ws,mglNumThr);	}
		b = new double[2*nx*mglNumThr];
		mglStartThreadT(mgl_cosx,ny*nz,d->a,b,wt,ws,par);
		if(mgl_fft_data.wnx==0)
		{	mgl_fft_data.wsx = ws;	mgl_fft_data.wtx = wt;	clear = false;
			mgl_fft_data.thx=mglNumThr;	mgl_fft_data.wnx=nx-1;	}
	}
	if(strchr(dir,'y') && ny>1)
	{
		if(mgl_fft_data.thy==mglNumThr && mgl_fft_data.wny==ny-1)
		{	ws = mgl_fft_data.wsy;	wt = mgl_fft_data.wty;	}
		else
		{	ws = new void*[mglNumThr];	clear = true;
			wt = mgl_fft_alloc(ny-1,ws,mglNumThr);	}
		b = new double[2*ny*mglNumThr];
		mglStartThreadT(mgl_cosy,nx*nz,d->a,b,wt,ws,par);
		if(mgl_fft_data.wny==0)
		{	mgl_fft_data.wsy = ws;	mgl_fft_data.wty = wt;	clear = false;
			mgl_fft_data.thy=mglNumThr;	mgl_fft_data.wny=ny-1;	}
	}
	if(strchr(dir,'z') && nz>1)
	{
		if(mgl_fft_data.thz==mglNumThr && mgl_fft_data.wnz==nz-1)
		{	ws = mgl_fft_data.wsz;	wt = mgl_fft_data.wtz;	}
		else
		{	ws = new void*[mglNumThr];	clear = true;
			wt = mgl_fft_alloc(nz-1,ws,mglNumThr);	}
		b = new double[2*nz*mglNumThr];
		mglStartThreadT(mgl_cosz,nx*ny,d->a,b,wt,ws,par);
		if(mgl_fft_data.wnz==0)
		{	mgl_fft_data.wsz = ws;	mgl_fft_data.wtz = wt;	clear = false;
			mgl_fft_data.thz=mglNumThr;	mgl_fft_data.wnz=nz-1;	}
	}
	if(clear)	{	mgl_fft_free(wt,ws,mglNumThr);	delete []ws;	}
	if(b)	delete []b;
}
//-----------------------------------------------------------------------------
HMDT MGL_EXPORT mgl_transform_a(HCDT am, HCDT ph, const char *tr)
{
	long nx = am->GetNx(), ny = am->GetNy(), nz = am->GetNz();
	if(nx*ny*nz != ph->GetNx()*ph->GetNy()*ph->GetNz() || !tr || tr[0]==0)
		return 0;
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
		return 0;
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
void MGL_EXPORT mgl_data_envelop_(uintptr_t *d, const char *dir, int)
{	mgl_data_envelop(_DT_,*dir);	}
//-----------------------------------------------------------------------------
#if MGL_HAVE_GSL
MGL_NO_EXPORT void* mgl_chnkx(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,nx=t->p[0];
	double *b = t->b+3*nx*t->id;
	dual *a = (dual*)t->a;
	const gsl_dht *dht = (const gsl_dht*)t->v;

	double mm = gsl_sf_bessel_zero_J0(nx+1);
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		for(j=0;j<nx;j++)	b[j] = real(a[j+nx*i]);
		gsl_dht_apply(dht,b,b+nx);
		for(j=0;j<nx;j++)	b[j] = imag(a[j+nx*i]);
		gsl_dht_apply(dht,b,b+2*nx);
		for(j=0;j<nx;j++)	a[j+nx*i] = dual(b[j+nx],b[j+2*nx])*mreal(mm);
	}
	return 0;
}
MGL_NO_EXPORT void* mgl_chnky(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long ii,i,j,k,nx=t->p[0],ny=t->p[1];
	double *b = t->b+3*ny*t->id;
	dual *a = (dual*)t->a;
	const gsl_dht *dht = (const gsl_dht*)t->v;
	
	double mm = gsl_sf_bessel_zero_J0(ny+1);
	for(ii=t->id;ii<t->n;ii+=mglNumThr)
	{
		i = ii%nx;	k = ii/nx;
		for(j=0;j<ny;j++)	b[j] = real(a[i+nx*(j+ny*k)]);
		gsl_dht_apply(dht,b,b+ny);
		for(j=0;j<ny;j++)	b[j] = imag(a[i+nx*(j+ny*k)]);
		gsl_dht_apply(dht,b,b+2*ny);
		for(j=0;j<ny;j++)	a[i+nx*(j+ny*k)] = dual(b[j+ny],b[j+2*ny])*mreal(mm);
	}
	return 0;
}
MGL_NO_EXPORT void* mgl_chnkz(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,k=t->p[0]*t->p[1],nz=t->p[2];
	double *b = t->b+3*nz*t->id;
	dual *a = (dual*)t->a;
	const gsl_dht *dht = (const gsl_dht*)t->v;
	
	double mm = gsl_sf_bessel_zero_J0(nz+1);
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		for(j=0;j<nz;j++)	b[j] = real(a[i+j*k]);
		gsl_dht_apply(dht,b,b+nz);
		for(j=0;j<nz;j++)	b[j] = imag(a[i+j*k]);
		gsl_dht_apply(dht,b,b+2*nz);
		for(j=0;j<nz;j++)	a[i+j*k] = dual(b[j+nz],b[j+2*nz])*mreal(mm);
	}
	return 0;
}
void MGL_EXPORT mgl_datac_hankel(HADT d, const char *dir)
{
	if(!dir || *dir==0)	return;
	if(mglNumThr<1)	mgl_set_num_thr(0);	// manually set number of threads
	gsl_dht *dht=0;
	bool clear = false;
	double *b=0;
	long nx=d->nx, ny=d->ny, nz=d->nz;
	long par[3]={nx,ny,nz};
	if(strchr(dir,'x') && nx>1)
	{
		if(mgl_fft_data.thx==mglNumThr && mgl_fft_data.hnx==nx)
		{	dht = (gsl_dht *)mgl_fft_data.htx;	}
		else
		{	dht = gsl_dht_new(nx,0,1);	clear = true;	}
		b = new double[3*nx*mglNumThr];
		mglStartThreadT(mgl_chnkx,ny*nz,d->a,b,dht,0,par);
		if(mgl_fft_data.hnx==0)	// TODO: make different th[xyz] for hankel ???
		{	mgl_fft_data.htx = dht;	clear = false;
			mgl_fft_data.thx=mglNumThr;	mgl_fft_data.hnx=nx;	}
	}
	if(strchr(dir,'y') && ny>1)
	{
		if(mgl_fft_data.thy==mglNumThr && mgl_fft_data.hny==ny)
		{	dht = (gsl_dht *)mgl_fft_data.hty;	}
		else
		{	dht = gsl_dht_new(ny,0,1);	clear = true;	}
		b = new double[3*ny*mglNumThr];
		mglStartThreadT(mgl_chnky,nx*nz,d->a,b,dht,0,par);
		if(mgl_fft_data.hny==0)	// TODO: make different th[xyz] for hankel ???
		{	mgl_fft_data.hty = dht;	clear = false;
			mgl_fft_data.thy=mglNumThr;	mgl_fft_data.hny=ny;	}
	}
	if(strchr(dir,'z') && nz>1)
	{
		if(mgl_fft_data.thz==mglNumThr && mgl_fft_data.hnz==nz)
		{	dht = (gsl_dht *)mgl_fft_data.htz;	}
		else
		{	dht = gsl_dht_new(nz,0,1);	clear = true;	}
		b = new double[3*nz*mglNumThr];
		mglStartThreadT(mgl_chnkz,nx*ny,d->a,b,dht,0,par);
		if(mgl_fft_data.hnz==0)	// TODO: make different th[xyz] for hankel ???
		{	mgl_fft_data.htz = dht;	clear = false;
			mgl_fft_data.thz=mglNumThr;	mgl_fft_data.hnz=nz;	}
	}
	if(clear)	gsl_dht_free(dht);
	if(b)	delete []b;
}
#else
void MGL_EXPORT mgl_datac_hankel(HADT , const char *){}
#endif
void MGL_EXPORT mgl_datac_hankel_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_datac_hankel(_DC_,s);	delete []s;	}
//-----------------------------------------------------------------------------
#if MGL_HAVE_GSL
MGL_NO_EXPORT void* mgl_hnkx(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,nx=t->p[0];
	double *b = t->b+2*nx*t->id;
	mreal *a = (mreal*)t->a;
	const gsl_dht *dht = (const gsl_dht*)t->v;
	
	double mm = gsl_sf_bessel_zero_J0(nx+1);
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		for(j=0;j<nx;j++)	b[j] = a[j+nx*i];
		gsl_dht_apply(dht,b,b+nx);
		for(j=0;j<nx;j++)	a[j+nx*i] = b[j+nx]*mreal(mm);
	}
	return 0;
}
MGL_NO_EXPORT void* mgl_hnky(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long ii,i,j,k,nx=t->p[0],ny=t->p[1];
	double *b = t->b+2*ny*t->id;
	mreal *a = (mreal*)t->a;
	const gsl_dht *dht = (const gsl_dht*)t->v;
	
	double mm = gsl_sf_bessel_zero_J0(ny+1);
	for(ii=t->id;ii<t->n;ii+=mglNumThr)
	{
		i = ii%nx;	k = ii/nx;
		for(j=0;j<ny;j++)	b[j] = a[i+nx*(j+ny*k)];
		gsl_dht_apply(dht,b,b+ny);
		for(j=0;j<ny;j++)a[i+nx*(j+ny*k)] = b[j+ny]*mreal(mm);
	}
	return 0;
}
MGL_NO_EXPORT void* mgl_hnkz(void *par)
{
	mglThreadT *t=(mglThreadT *)par;
	register long i,j,k=t->p[0]*t->p[1],nz=t->p[2];
	double *b = t->b+2*nz*t->id;
	mreal *a = (mreal*)t->a;
	const gsl_dht *dht = (const gsl_dht*)t->v;
	
	double mm = gsl_sf_bessel_zero_J0(nz+1);
	for(i=t->id;i<t->n;i+=mglNumThr)
	{
		for(j=0;j<nz;j++)	b[j] = a[i+j*k];
		gsl_dht_apply(dht,b,b+nz);
		for(j=0;j<nz;j++)	a[i+j*k] = b[j+nz]*mreal(mm);
	}
	return 0;
}
void MGL_EXPORT mgl_data_hankel(HMDT d, const char *dir)
{
	if(!dir || *dir==0)	return;
	if(mglNumThr<1)	mgl_set_num_thr(0);	// manually set number of threads
	bool clear = false;
	gsl_dht *dht=0;
	double *b=0;
	long nx=d->nx, ny=d->ny, nz=d->nz;
	long par[3]={nx,ny,nz};
	if(strchr(dir,'x') && nx>1)
	{
		if(mgl_fft_data.thx==mglNumThr && mgl_fft_data.hnx==nx)
		{	dht = (gsl_dht *)mgl_fft_data.htx;	}
		else
		{	dht = gsl_dht_new(nx,0,1);	clear = true;	}
		b = new double[2*nx*mglNumThr];
		mglStartThreadT(mgl_hnkx,ny*nz,d->a,b,dht,0,par);
		if(mgl_fft_data.hnx==0)	// TODO: make different th[xyz] for hankel ???
		{	mgl_fft_data.htx = dht;	clear = false;
			mgl_fft_data.thx=mglNumThr;	mgl_fft_data.hnx=nx;	}
	}
	if(strchr(dir,'y') && ny>1)
	{
		if(mgl_fft_data.thy==mglNumThr && mgl_fft_data.hny==ny)
		{	dht = (gsl_dht *)mgl_fft_data.hty;	}
		else
		{	dht = gsl_dht_new(ny,0,1);	clear = true;	}
		b = new double[2*ny*mglNumThr];
		mglStartThreadT(mgl_hnky,nx*nz,d->a,b,dht,0,par);
		if(mgl_fft_data.hny==0)	// TODO: make different th[xyz] for hankel ???
		{	mgl_fft_data.hty = dht;	clear = false;
			mgl_fft_data.thy=mglNumThr;	mgl_fft_data.hny=ny;	}
	}
	if(strchr(dir,'z') && nz>1)
	{
		if(mgl_fft_data.thz==mglNumThr && mgl_fft_data.hnz==nz)
		{	dht = (gsl_dht *)mgl_fft_data.htz;	}
		else
		{	dht = gsl_dht_new(nz,0,1);	clear = true;	}
		b = new double[2*nz*mglNumThr];
		mglStartThreadT(mgl_hnkz,nx*ny,d->a,b,dht,0,par);
		if(mgl_fft_data.hnz==0)	// TODO: make different th[xyz] for hankel ???
		{	mgl_fft_data.htz = dht;	clear = false;
			mgl_fft_data.thz=mglNumThr;	mgl_fft_data.hnz=nz;	}
	}
	if(clear)	gsl_dht_free(dht);
	if(b)	delete []b;
}
#else
void MGL_EXPORT mgl_data_hankel(HMDT , const char *){}
#endif
void MGL_EXPORT mgl_data_hankel_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_hankel(_DT_,s);	delete []s;	}
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
void MGL_EXPORT mgl_datac_fft_(uintptr_t *d, const char *dir, int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_datac_fft(_DC_,s);	delete []s;	}
void MGL_EXPORT mgl_data_fourier_(uintptr_t *re, uintptr_t *im, const char *dir, int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_fourier(_DM_(re),_DM_(im),s);	delete []s;	}
uintptr_t MGL_EXPORT mgl_data_stfa_(uintptr_t *re, uintptr_t *im, int *dn, char *dir, int)
{	return uintptr_t(mgl_data_stfa(_DA_(re),_DA_(im),*dn,*dir));	}
void MGL_EXPORT mgl_data_cosfft_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_cosfft(_DT_,s);	delete []s;	}
void MGL_EXPORT mgl_data_sinfft_(uintptr_t *d, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	mgl_data_sinfft(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
