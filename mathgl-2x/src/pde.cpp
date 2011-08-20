/***************************************************************************
 * mgl_pde.cpp is part of Math Graphic Library
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
#include "mgl/data.h"
#include <complex>
#define dual	std::complex<double>
#define GAMMA	0.1
#ifndef NO_GSL
#include <gsl/gsl_fft_complex.h>
#endif
//-----------------------------------------------------------------------------
struct mgl_pde_ham
{
	dual *a,*hxy,*hxv,*huv,*huy;
	mglFormula *eqs;
	long nx,ny;
	mreal xx,yy,xs,ys,dx,dy,dq,dp,zz;
	double dd;
};
void *mgl_pde_hprep(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	mgl_pde_ham *f = (mgl_pde_ham *)t->v;
	mglFormula *eqs = f->eqs;
	register long i,j,i0, nx=2*f->nx, ny=2*f->ny;
	mreal var[MGL_VS];
	dual *a = f->a,*h;
	memset(var,0,('z'-'a')*sizeof(mreal));
	var['z'-'a'] = f->zz;
	for(i0=t->id,h=f->hxy;i0<t->n;i0+=mglNumThr)
	{
		i = i0%nx;	j = i0/nx;			var['u'-'a'] = abs(a[i0]);
		var['x'-'a'] = f->xx+f->dx*i;	var['p'-'a'] = 0;
		var['y'-'a'] = f->yy+f->dy*j;	var['q'-'a'] = 0;
		h[i0] = dual(-eqs->CalcD(var,'i'), eqs->Calc(var))*f->dd;
	}
	if(f->ny>2)	for(i0=t->id,h=f->huy;i0<t->n;i0+=mglNumThr)	// step 3/2
	{
		i = i0%nx;	j = i0/nx;			var['u'-'a'] = abs(a[i0]);
		var['x'-'a'] = f->xs;			var['p'-'a'] = f->dp*(i<nx/2 ? i:nx-i);
		var['y'-'a'] = f->yy+f->dy*j;	var['q'-'a'] = 0;
		h[i0] = dual(-eqs->CalcD(var,'i'), eqs->Calc(var))*f->dd;
	}
	for(i0=t->id,h=f->huv;i0<t->n;i0+=mglNumThr)	// step 2
	{
		i = i0%nx;	j = i0/nx;			var['u'-'a'] = abs(a[i0]);
		var['x'-'a'] = f->xs;			var['p'-'a'] = f->dp*(i<nx/2 ? i:nx-i);
		var['y'-'a'] = f->ys;			var['q'-'a'] = f->dq*(j<ny/2 ? j:ny-j);
		h[i0] = dual(-eqs->CalcD(var,'i'), eqs->Calc(var))*f->dd;
	}
	if(f->ny>2)	for(i0=t->id,h=f->hxv;i0<t->n;i0+=mglNumThr)	// step 3/2
	{
		i = i0%nx;	j = i0/nx;			var['u'-'a'] = abs(a[i0]);
		var['x'-'a'] = f->xx+f->dx*i;	var['p'-'a'] = 0;
		var['y'-'a'] = f->ys;			var['q'-'a'] = f->dq*(j<ny/2 ? j:ny-j);
		h[i0] = dual(-eqs->CalcD(var,'i'), eqs->Calc(var))*f->dd;
	}
	return 0;
}
// Solve equation du/dz = ham(p,q,x,y,z,|u|)[u] where p=d/dx, q=d/dy. At this moment simplified form of ham is supported: ham = f(p,q,z) + g(x,y,z,'u'), where variable 'u'=|u| (for allowing solve nonlinear problems). You may specify imaginary part like ham = p^2 + i*x*(x>0) but only if dependence on variable 'i' is linear (i.e. ham = hre+i*him).
HMDT mgl_pde_solve(HMGL gr, const char *ham, HCDT ini_re, HCDT ini_im, float dz, float k0, const char *opt)
{
	gr->SaveState(opt);
	mglPoint Min=gr->Min, Max=gr->Max;
	mglData *res=new mglData;
	int nx=ini_re->GetNx(), ny=ini_re->GetNy();
	int nz = int((Max.z-Min.z)/dz)+1;
	if(nx<2 || nz<2 || Max.x==Min.x)			// Too small data
	{	gr->SetWarn(mglWarnLow,"PDE");	return res;	}
	if(ini_im->GetNx()*ini_im->GetNy() != nx*ny)// Wrong dimensions
	{	gr->SetWarn(mglWarnDim,"PDE");	return res;	}
	mgl_data_create(res, nz, nx, ny);
#ifndef NO_GSL
	mglFormula eqs(ham);
	dual *a = new dual[4*nx*ny], hh0;	// Add "damping" area
	dual *hxy = new dual[4*nx*ny], *hxv = new dual[4*nx*ny];
	dual *huy = new dual[4*nx*ny], *huv = new dual[4*nx*ny];
	dual *hx = new dual[2*nx], *hv = new dual[2*ny];
	dual *hy = new dual[2*ny], *hu = new dual[2*nx];
	mreal *dmp = new mreal[4*nx*ny];
	memset(a,0,4*nx*ny*sizeof(dual));
	register int i,j,k,i0;
	for(j=0;j<ny;j++)	for(i=0;i<nx;i++)	// Initial conditions
	{
		i0 = i+nx/2+2*nx*(j+ny/2);
		a[i0] = dual(ini_re->v(i,j), ini_im->v(i,j));
		res->a[nz*(i+nx*j)] = abs(a[i0]);
	}
	memset(dmp,0,4*nx*ny*sizeof(mreal));
	for(j=0;j<2*ny;j++)	for(i=0;i<2*nx;i++)	// step 1
	{
		i0 = i+2*nx*j;
		if(i<nx/2)		dmp[i0] += GAMMA*mgl_ipow((nx/2-i)/(nx/2.),2);
		if(i>3*nx/2)	dmp[i0] += GAMMA*mgl_ipow((i-3*nx/2-1)/(nx/2.),2);
		if(j<ny/2)		dmp[i0] += GAMMA*mgl_ipow((ny/2-j)/(ny/2.),2);
		if(j>3*ny/2)	dmp[i0] += GAMMA*mgl_ipow((j-3*ny/2-1)/(ny/2.),2);
	}
	mreal dx = (Max.x-Min.x)/(nx-1), dy = ny>1?(Max.y-Min.y)/(ny-1):0;
	mreal dp = M_PI/(Max.x-Min.x)/k0, dq = M_PI/(Max.y-Min.y)/k0;
	mreal xs=(Min.x+Max.x)/2, ys=(Min.y+Max.y)/2;
	double xx = Min.x - dx*nx/2, yy = Min.x - dy*ny/2;
	double ff = ny>1?4*nx*ny:2*nx, dd = k0*dz;

	mgl_pde_ham tmp;tmp.eqs = &eqs;
	tmp.nx = nx;	tmp.ny = ny;	tmp.a=a;		tmp.hxy=hxy;
	tmp.hxv=hxv;	tmp.huy=huy;	tmp.huv=huv;	tmp.dd = dd;
	tmp.xx = xx;	tmp.xs = xs;	tmp.dx = dx;	tmp.dp = dp;
	tmp.yy = yy;	tmp.ys = ys;	tmp.dy = dy;	tmp.dq = dq;

	// prepare fft. NOTE: slow procedures due to unknown nx, ny.
	gsl_fft_complex_wavetable *wtx = gsl_fft_complex_wavetable_alloc(2*nx);
	gsl_fft_complex_workspace *wsx = gsl_fft_complex_workspace_alloc(2*nx);
	gsl_fft_complex_wavetable *wty = gsl_fft_complex_wavetable_alloc(2*ny);
	gsl_fft_complex_workspace *wsy = gsl_fft_complex_workspace_alloc(2*ny);
	for(k=1;k<nz;k++)
	{
		tmp.zz = Min.z+dz*k;
		memset(hxy,0,4*nx*ny*sizeof(dual));	memset(hxv,0,4*nx*ny*sizeof(dual));
		memset(huv,0,4*nx*ny*sizeof(dual));	memset(huy,0,4*nx*ny*sizeof(dual));
		mglStartThread(mgl_pde_hprep,0,4*nx*ny,0,0,0,0,&tmp);
		for(i=0;i<2*nx;i++)	{	hx[i] = hxv[i];			hu[i] = huv[i];		}
		for(j=0;j<2*ny;j++)	{	hy[j] = huy[2*nx*j];	hv[j] = huv[2*nx*j];}
		// rearrange arrays
		hh0=hu[0]/2.;
		if(ny>1)	for(i=0;i<2*nx;i++) for(j=0;j<2*ny;j++)
		{
			i0 = i+2*nx*j;
			huy[i0] -= (hu[i]+hy[j]-hh0)/2.;	huv[i0] -= (hu[i]+hv[j]-hh0)/2.;
			hxy[i0] -= (hx[i]+hy[j]-hh0)/2.;	hxv[i0] -= (hx[i]+hv[j]-hh0)/2.;
		}
		// solve equation
		for(i=0;i<4*nx*ny;i++)	a[i] *= exp(hxy[i])*exp(-double(dmp[i]*dz))/ff;
		for(i=0;i<2*ny;i++)		gsl_fft_complex_transform((double *)(a+i*2*nx), 1, 2*nx, wtx, wsx, forward);
		for(i=0;i<4*nx*ny;i++)	a[i] *= exp(huy[i]);
		if(ny>1) for(i=0;i<2*nx;i++)	gsl_fft_complex_transform((double *)(a+i), 2*nx, 2*ny, wty, wsy, forward);
		for(i=0;i<4*nx*ny;i++)	a[i] *= exp(huv[i]);
		for(i=0;i<2*ny;i++)		gsl_fft_complex_transform((double *)(a+2*i*nx), 1, 2*nx, wtx, wsx, backward);
		for(i=0;i<4*nx*ny;i++)	a[i] *= exp(hxv[i]);
		if(ny>1) for(i=0;i<2*nx;i++)	gsl_fft_complex_transform((double *)(a+i), 2*nx, 2*ny, wty, wsy, backward);
		for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	// save result
		{
			i0 = i+nx/2+2*nx*(j+ny/2);
			res->a[k+nz*(i+nx*j)] = abs(a[i0]);
		}
	}
	gsl_fft_complex_workspace_free(wsx);
	gsl_fft_complex_wavetable_free(wtx);
	gsl_fft_complex_workspace_free(wsy);
	gsl_fft_complex_wavetable_free(wty);
	delete []a;		delete []dmp;
	delete []hxy;	delete []hxv;	delete []huy;	delete []huv;
	delete []hx;	delete []hy;	delete []hu;	delete []hv;
#endif
	gr->LoadState();
	return res;
}
//-----------------------------------------------------------------------------
// Solve GO ray equation like dr/dt = d ham/dp, dp/dt = -d ham/dr where ham = ham(x,y,z,p,q,v,t) and px=p, py=q, pz=v. The starting point (at t=0) is r0, p0. Result is array of {x,y,z,p,q,v,t}
HMDT mgl_ray_trace(const char *ham, float x0, float y0, float z0, float px, float py, float pz, float dt, float tmax)
{
	mglData *res=new mglData;
	if(tmax<dt)	return res;	// nothing to do
	int nt = int(tmax/dt)+1;
	mreal x[6], k1[6], k2[6], k3[6], hh=dt/2;
	mgl_data_create(res,7,nt,1);
	mgl_data_set_id(res,"xyzpqvt");
#ifndef NO_GSL
	mglFormula eqs(ham);
	// initial conditions
	x[0] = res->a[0] = x0;	x[1] = res->a[1] = y0;	x[2] = res->a[2] = z0;
	x[3] = res->a[3] = px;	x[4] = res->a[4] = py;	x[5] = res->a[5] = pz;
	res->a[6] = 0;
	// Runge Kutta scheme of 4th order
	char v[7]="xyzpqv";
	mreal var[MGL_VS];	memset(var,0,MGL_VS*sizeof(mreal));
	register int i,k;
	for(k=1;k<nt;k++)
	{
		// 		md->H(cy,k1);
		var['t'-'a']=k*dt;		for(i=0;i<6;i++)	var[v[i]-'a'] = x[i];
		k1[0] = eqs.CalcD(var,'p');	k1[3] = -eqs.CalcD(var,'x');
		k1[1] = eqs.CalcD(var,'q');	k1[4] = -eqs.CalcD(var,'y');
		k1[2] = eqs.CalcD(var,'v');	k1[5] = -eqs.CalcD(var,'z');
		// 		ty = cy/(k1*hh);	md->H(ty,k2);
		var['t'-'a']=k*dt+hh;	for(i=0;i<6;i++)	var[v[i]-'a'] = x[i]+k1[i]*hh;
		k2[0] = eqs.CalcD(var,'p');	k2[3] = -eqs.CalcD(var,'x');
		k2[1] = eqs.CalcD(var,'q');	k2[4] = -eqs.CalcD(var,'y');
		k2[2] = eqs.CalcD(var,'v');	k2[5] = -eqs.CalcD(var,'z');
		//		ty = cy/(k2*hh);	md->H(ty,k3);
		var['t'-'a']=k*dt+hh;	for(i=0;i<6;i++)	var[v[i]-'a'] = x[i]+k2[i]*hh;
		k3[0] = eqs.CalcD(var,'p');	k3[3] = -eqs.CalcD(var,'x');
		k3[1] = eqs.CalcD(var,'q');	k3[4] = -eqs.CalcD(var,'y');
		k3[2] = eqs.CalcD(var,'v');	k3[5] = -eqs.CalcD(var,'z');
		//		ty = cy/(k2*h);	k3+=k2;	md->H(ty,k2);
		var['t'-'a']=k*dt+dt;	for(i=0;i<6;i++)
		{	var[v[i]-'a'] = x[i]+k2[i]*dt;	k3[i] += k2[i];	}
		k2[0] = eqs.CalcD(var,'p');	k2[3] = -eqs.CalcD(var,'x');
		k2[1] = eqs.CalcD(var,'q');	k2[4] = -eqs.CalcD(var,'y');
		k2[2] = eqs.CalcD(var,'v');	k2[5] = -eqs.CalcD(var,'z');
		//		cy /= (k1+k2+k3*2.)*(h/6);
		for(i=0;i<6;i++)
			res->a[i+7*k] = x[i] += (k1[i]+k2[i]+2*k3[i])*dt/6;
		res->a[6+7*k] = dt*k;
	}
#endif
	return res;
}
//-----------------------------------------------------------------------------
struct mgl_ap
{
	double x0,y0,x1,y1;	// vectors {l, g1, g2}
	double t1,ch,q1,pt,dt,d1;	// theta_{1,2}, chi, q_{1,2}, p_t, dtau, dq_{1,2}
	mgl_ap()	{	memset(this,0,sizeof(mgl_ap));	};
};
//-----------------------------------------------------------------------------
void mgl_init_ra(int n, const mreal *r, mgl_ap *ra)	// prepare some intermediate data for mglPDE2d
{
	register double tt;
	tt = hypot(r[7]-r[0], r[8]-r[1]);
	ra[0].x1 = (r[8]-r[1])/tt;
	ra[0].y1 = (r[0]-r[7])/tt;
	register long i;
	for(i=1;i<n;i++)	// NOTE: no parallel due to dependence on prev point!
	{
		ra[i].dt = r[6+7*i] - r[7*i-1];
		ra[i].x0 = r[7*i] - r[7*i-7];	// NOTE: very rough formulas
		ra[i].y0 = r[7*i+1] - r[7*i-6];	// for corresponding with dt one
		tt = sqrt(ra[i].x0*ra[i].x0 + ra[i].y0*ra[i].y0);
		ra[i].x0 /= tt;	ra[i].y0 /= tt;
		ra[i].ch = tt/ra[i].dt;
		tt = ra[i].x0*ra[i-1].x1 + ra[i].y0*ra[i-1].y1;
		ra[i].x1 = ra[i-1].x1 - tt*ra[i].x0;	// vector g_1
		ra[i].y1 = ra[i-1].y1 - tt*ra[i].y0;
		ra[i].t1 = tt/(ra[i].dt*ra[i].ch);
		tt = sqrt(ra[i].x1*ra[i].x1 + ra[i].y1*ra[i].y1);
		ra[i].x1 /= tt;	ra[i].y1 /= tt;
		// other parameters
		ra[i].pt = r[7*i+3]*ra[i].x0 + r[7*i+4]*ra[i].y0;
		ra[i].q1 = r[7*i+3]*ra[i].x1 + r[7*i+4]*ra[i].y1;
		ra[i].d1 = (ra[i].q1-ra[i-1].q1)/(ra[i].dt*ra[i].ch);
	}
	memcpy(ra,ra+1,sizeof(mgl_ap));	// setup zero point
	ra[0].pt = r[3]*ra[0].x0 + r[4]*ra[0].y0;
	ra[0].q1 = r[3]*ra[0].x1 + r[4]*ra[0].y1;
}
//-----------------------------------------------------------------------------
struct mgl_qo2d_ham
{
	dual *hx, *hu, *a, h0;
	double *ru, *rx, *dmp;
	mreal *r, dr, dk, pt0;
	mgl_ap *ra;
	mglFormula *h;
};
void *mgl_qo2d_hprep(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	mgl_qo2d_ham *f = (mgl_qo2d_ham *)t->v;
	mgl_ap *ra = f->ra;
	mglFormula *h = f->h;
	const mreal *r = f->r;
	mreal var[MGL_VS], pt0=f->pt0, tt, x1, hh;
	memset(var,0,MGL_VS*sizeof(mreal));
	register long i, nx=t->n;
	for(i=t->id;i<nx;i+=mglNumThr)
	{
		// x terms
		x1 = (2*i-nx+1)*f->dr;
		var['x'-'a'] = r[0] + ra->x1*x1;	// new coordiantes
		var['y'-'a'] = r[1] + ra->y1*x1;
		hh = 1 - ra->t1*x1;	hh = sqrt(sqrt(0.041+hh*hh*hh*hh));
		tt = (ra->pt + ra->d1*x1)/hh - ra->pt;
		var['p'-'a'] = r[3] + ra->x0*tt;	// new momentums
		var['q'-'a'] = r[4] + ra->y0*tt;	var['u'-'a'] = abs(f->a[i]);
		f->rx[i] = (h->CalcD(var,'p')*ra->x0 + h->CalcD(var,'q')*ra->y0)/ra->ch;
		f->hx[i] = dual(h->Calc(var), -h->CalcD(var,'i'));
		// u-y terms
		x1 = f->dk/2*(i<nx/2 ? i:i-nx);
		var['x'-'a'] = r[0];	var['y'-'a'] = r[1];
		var['p'-'a'] = r[3] + ra->x1*x1;	// new momentums
		var['q'-'a'] = r[4] + ra->y1*x1;	var['u'-'a'] = 0;
		f->ru[i] = (h->CalcD(var,'p')*ra->x0 + h->CalcD(var,'q')*ra->y0)/ra->ch;
		f->hu[i] = dual(h->Calc(var), -h->CalcD(var,'i'));

		f->rx[i] = f->rx[i]>0.3*pt0 ? f->rx[i] : 0.3*pt0;
		f->hx[i] = (f->hx[i]-f->h0/2.)/f->rx[i];
		if(imag(f->hx[i])>0)	f->hx[i] = f->hx[i].real();
		f->ru[i] = f->ru[i]>0.3*pt0 ? f->ru[i] : 0.3*pt0;
		f->hu[i] = (f->hu[i]-f->h0/2.)/f->ru[i];
		if(imag(f->hu[i])>0)	f->hu[i] = f->hu[i].real();
		// add boundary conditions for x-direction
		f->hx[i] -= dual(0,f->dmp[i]);
	}
	return 0;
}
HMDT mgl_qo2d_solve(const char *ham, HCDT ini_re, HCDT ini_im, HCDT ray_dat, float r, float k0, HMDT xx, HMDT yy)
{
	mglData *res=new mglData;
	const mglData *ray=dynamic_cast<const mglData *>(ray_dat);	// NOTE: Ray must be mglData!
	if(!ray)	return res;
	int nx=ini_re->GetNx(), nt=ray->ny;
	if(nx<2 || ini_im->GetNx()!=nx || nt<2)	return res;
	mgl_data_create(res,nx,nt,1);
#ifndef NO_GSL
	dual *a=new dual[2*nx], *hu=new dual[2*nx],  *hx=new dual[2*nx], h0;
	double *ru=new double[2*nx],	*rx=new double[2*nx],	*dmp=new double[2*nx],
			*pu=new double[2*nx],	*px=new double[2*nx];
	mgl_ap *ra = new mgl_ap[nt];	mgl_init_ra(ray->ny, ray->a, ra);	// ray
	register int i;
	mglFormula h(ham);
	mreal var[MGL_VS], dr = r/(nx-1), dk = M_PI*(nx-1)/(k0*r*nx), tt, x1, hh, B1, pt0;
	memset(var,0,MGL_VS*sizeof(mreal));

	memset(dmp,0,2*nx*sizeof(double));
	for(i=0;i<nx/2;i++)	// prepare damping
	{
		x1 = (nx/2-i)/(nx/2.);
		dmp[2*nx-1-i] = dmp[i] = 30*GAMMA*x1*x1/k0;
	}
	for(i=0;i<nx;i++)	a[i+nx/2] = dual(ini_re->v(i),ini_im->v(i));	// ini
	for(i=0;i<2*nx;i++)	{	rx[i] = ru[i] = 1;	}
	gsl_fft_complex_wavetable *wtx = gsl_fft_complex_wavetable_alloc(2*nx);
	gsl_fft_complex_workspace *wsx = gsl_fft_complex_workspace_alloc(2*nx);
	if(xx && yy)	{	xx->Create(nx,nt);	yy->Create(nx,nt);	}

	mgl_qo2d_ham tmp;
	tmp.hx=hx;	tmp.rx=rx;	tmp.hu=hu;	tmp.ru=ru;	tmp.dmp=dmp;
	tmp.dr=dr;	tmp.dk=dk;	tmp.h=&h;	tmp.a=a;
	// start calculation
	for(int k=0;k<nt;k++)
	{
		for(i=0;i<nx;i++)	// "save"
			res->a[i+k*nx]=abs(a[i+nx/2])*sqrt(ra[0].ch/ra[k].ch);
		if(xx && yy)	for(i=0;i<nx;i++)	// prepare xx, yy
		{
			x1 = (2*i-nx+1)*dr;
			xx->a[i+k*nx] = ray->a[7*k] + ra[k].x1*x1;	// new coordiantes
			yy->a[i+k*nx] = ray->a[7*k+1] + ra[k].y1*x1;
		}
		memcpy(px,rx,2*nx*sizeof(double));
		memcpy(pu,ru,2*nx*sizeof(double));
		hh = ra[k].pt*(1/sqrt(sqrt(1.041))-1);	// 0.041=0.45^4 -- minimal value of h
		var['x'-'a'] = ray->a[7*k];	var['y'-'a'] = ray->a[7*k+1];
		var['p'-'a'] = ray->a[7*k+3] + ra[k].x0*hh;
		var['q'-'a'] = ray->a[7*k+4] + ra[k].y0*hh;	var['u'-'a'] = 0;
		h0 = dual(h.Calc(var), -h.CalcD(var,'i'));
		pt0 = (h.CalcD(var,'p')*ra[k].x0 + h.CalcD(var,'q')*ra[k].y0)/ra[k].ch;

		tmp.h0=h0;	tmp.pt0=pt0;	tmp.r=ray->a+7*k;	tmp.ra=ra+k;
		mglStartThread(mgl_qo2d_hprep,0,2*nx,0,0,0,0,&tmp);

		// Calculate B1
		hh = ra[k].pt*(1/sqrt(sqrt(1.041))-1);
		var['x'-'a'] = ray->a[7*k];	// new coordiantes
		var['y'-'a'] = ray->a[7*k+1];
		var['p'-'a'] = ray->a[7*k+3] + ra[k].x0*hh;	// new momentums
		var['q'-'a'] = ray->a[7*k+4] + ra[k].y0*hh;
		tt = h.CalcD(var,'p')*ra[k].x1 + h.CalcD(var,'q')*ra[k].y1;
		var['x'-'a'] = ray->a[7*k] + ra[k].x1*dr;	// new coordiantes
		var['y'-'a'] = ray->a[7*k+1] + ra[k].y1*dr;
		hh = 1 - ra[k].t1*dr;	hh = sqrt(sqrt(0.041+hh*hh*hh*hh));
		hh = (ra[k].ch*ra[k].pt + ra[k].d1*dr)/(hh*ra[k].ch) - ra[k].pt;
		var['p'-'a'] = ray->a[7*k+3] + ra[k].x0*hh;	// new momentums
		var['q'-'a'] = ray->a[7*k+4] + ra[k].y0*hh;
		B1 = h.CalcD(var,'p')*ra[k].x1 + h.CalcD(var,'q')*ra[k].y1;
		B1 = (B1-tt)/dr;
		// Step for field
		dual dt = dual(0, -ra[k].dt*k0);
		for(i=0;i<2*nx;i++)		a[i] *= exp(hx[i]*dt);
		gsl_fft_complex_transform((double *)a, 1, 2*nx, wtx, wsx, forward);
		for(i=0;i<2*nx;i++)		a[i] *= exp(hu[i]*dt)/(2.*nx);
		gsl_fft_complex_transform((double *)a, 1, 2*nx, wtx, wsx, backward);
		double a1=0, a2=0;
		for(i=0;i<2*nx;i++)	a1 += norm(a[i]);
		hx[0] = hx[2*nx-1] = 0.;
		for(i=1;i<2*nx-1;i++)	hx[i] = (B1*ra[k].dt*(i-nx))*(a[i+1]-a[i-1]);
		for(i=0;i<2*nx;i++)	{	a[i] += hx[i];	a2 += norm(a[i]);	}
		a1 = sqrt(a1/a2);
		for(i=0;i<2*nx;i++)	a[i] *= a1;
	}
	gsl_fft_complex_workspace_free(wsx);
	gsl_fft_complex_wavetable_free(wtx);
	delete []a;		delete []hu;	delete []hx;	delete []ra;	delete []dmp;
	delete []rx;	delete []ru;	delete []px;	delete []pu;
#endif
	return res;
}
//-----------------------------------------------------------------------------
void *mgl_jacob2(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,i0,ip,im,jp,jm, nx=t->p[0], ny=t->p[1];
	mreal *r=t->a;
	const mreal *x=t->b, *y=t->c;
	for(i0=t->id;i0<t->n;i0+=mglNumThr)
	{
		i=i0%nx;	j=i0/nx;
		ip = i<nx-1 ? 1:0;	jp = j<ny-1 ? nx:0;
		im = i>0 ? -1:0;	jm = j>0 ? -nx:0;
		r[i0] = (x[i0+ip]-x[i0+im])*(y[i0+jp]-y[i0+jm]) -
				(y[i0+ip]-y[i0+im])*(x[i0+jp]-x[i0+jm]);
		r[i0] *= mreal((nx-1)*(ny-1)) / ((ip-im)*(jp-jm));
	}
	return 0;
}
HMDT mgl_jacobian_2d(HCDT x, HCDT y)
{
	int nx = x->GetNx(), ny=x->GetNy();
	mglData *r=new mglData;
	if(nx!=y->GetNx() || ny!=y->GetNy() || nx<2 || ny<2)	return	r;
	mgl_data_create(r,nx,ny,1);
	const mglData *xx=dynamic_cast<const mglData *>(x);
	const mglData *yy=dynamic_cast<const mglData *>(y);
	if(xx && yy)
	{
		long p[2]={nx,ny};
		mglStartThread(mgl_jacob2,0,nx*ny,r->a,xx->a,yy->a,p);
	}
	else	// slow variant
	{
		register long i,j,ip,im,jp,jm;
		for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
		{
			im = i>0 ? i-1:i;	ip = i<nx-1 ? i+1:i;
			jm = j>0 ? j-1:j;	jp = j<ny-1 ? j+1:j;
			r->a[i+nx*j] = (x->v(ip,j)-x->v(im,j))*(y->v(i,jp)-y->v(i,jm)) -
						(y->v(ip,j)-y->v(im,j))*(x->v(i,jp)-x->v(i,jm));
			r->a[i+nx*j] *= mreal((nx-1)*(ny-1)) / ((ip-im)*(jp-jm));
		}
	}
	return r;
}
//-----------------------------------------------------------------------------
void *mgl_jacob3(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	register long i,j,k,i0,ip,im,jp,jm,kp,km, nx=t->p[0], ny=t->p[1], nz=t->p[2];
	mreal *r=t->a;
	const mreal *x=t->b, *y=t->c, *z=t->d;
	for(i0=t->id;i0<t->n;i0+=mglNumThr)
	{
		i=i0%nx;	j=(i0/nx)%ny;	k=i0/(nx*ny);
		ip = i<nx-1 ? 1:0;	jp = j<ny-1 ? nx:0;	kp = k<nz-1 ? nx*ny:0;
		im = i>0 ? -1:0;	jm = j>0 ? -nx:0;	km = k>0 ? -nx*ny:0;
		r[i0] = (x[i0+ip]-x[i0+im])*(y[i0+jp]-y[i0+jm])*(z[i0+kp]-z[i0+km]) -
				(x[i0+ip]-x[i0+im])*(y[i0+kp]-y[i0+km])*(z[i0+jp]-z[i0+jm]) -
				(x[i0+jp]-x[i0+jm])*(y[i0+ip]-y[i0+im])*(z[i0+kp]-z[i0+km]) +
				(x[i0+jp]-x[i0+jm])*(y[i0+kp]-y[i0+km])*(z[i0+ip]-z[i0+im]) +
				(x[i0+kp]-x[i0+km])*(y[i0+ip]-y[i0+im])*(z[i0+jp]-z[i0+jm]) -
				(x[i0+kp]-x[i0+km])*(y[i0+jp]-y[i0+jm])*(z[i0+ip]-z[i0+im]);
		r[i0] *= mreal((nx-1)*(ny-1)*(nz-1)) / ((ip-im)*(jp-jm)*(kp-km));
	}
	return 0;
}
HMDT mgl_jacobian_3d(HCDT x, HCDT y, HCDT z)
{
	int nx = x->GetNx(), ny=x->GetNy(), nz=x->GetNz(), nn = nx*ny*nz;
	mglData *r=new mglData;
	if(nx<2 || ny<2 || nz<2)	return	r;
	if(nn!=y->GetNx()*y->GetNy()*y->GetNz() || nn!=z->GetNx()*z->GetNy()*z->GetNz())	return r;
	mgl_data_create(r,nx,ny,nz);
	const mglData *xx=dynamic_cast<const mglData *>(x);
	const mglData *yy=dynamic_cast<const mglData *>(y);
	const mglData *zz=dynamic_cast<const mglData *>(z);
	if(xx && yy && zz)
	{
		long p[3]={nx,ny,nz};
		mglStartThread(mgl_jacob3,0,nx*ny*nz,r->a,xx->a,yy->a,p,0,zz->a);
	}
	else	// slow variant
	{
		register long i,j,k,ip,im,jp,jm,kp,km,i0;
		for(k=0;k<nz;k++)	for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
		{
			im = i>0 ? i-1:i;	ip = i<nx-1 ? i+1:i;
			jm = j>0 ? j-1:j;	jp = j<ny-1 ? j+1:j;
			km = k>0 ? k-1:k;	kp = k<nz-1 ? k+1:k;
			i0 = i+nx*(j+ny*k);
			r->a[i0] = (x->v(ip,j,k)-x->v(im,j,k))*(y->v(i,jp,k)-y->v(i,jm,k))*(z->v(i,j,kp)-z->v(i,j,km)) -
					(x->v(ip,j,k)-x->v(im,j,k))*(y->v(i,j,kp)-y->v(i,j,km))*(z->v(i,jp,k)-z->v(i,jm,k)) -
					(x->v(i,jp,k)-x->v(i,jm,k))*(y->v(ip,j,k)-y->v(im,j,k))*(z->v(i,j,kp)-z->v(i,j,km)) +
					(x->v(i,jp,k)-x->v(i,jm,k))*(y->v(i,j,kp)-y->v(i,j,km))*(z->v(ip,j,k)-z->v(im,j,k)) +
					(x->v(i,j,kp)-x->v(i,j,km))*(y->v(ip,j,k)-y->v(im,j,k))*(z->v(i,jp,k)-z->v(i,jm,k)) -
					(x->v(i,j,kp)-x->v(i,j,km))*(y->v(i,jp,k)-y->v(i,jm,k))*(z->v(ip,j,k)-z->v(im,j,k));
			r->a[i0] *= mreal((nx-1)*(ny-1)*(nz-1)) / ((ip-im)*(jp-jm)*(kp-km));
		}

	}
	return r;
}
//-----------------------------------------------------------------------------
uintptr_t mgl_pde_solve_(uintptr_t* gr, const char *ham, uintptr_t* ini_re, uintptr_t* ini_im, mreal *dz, mreal *k0, const char *opt, int l, int lo)
{	char *s=new char[l+1];	memcpy(s,ham,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	uintptr_t res = uintptr_t(mgl_pde_solve(_GR_, s, _DA_(ini_re), _DA_(ini_im), *dz, *k0, o));
	delete []o;	delete []s;	return res;	}
uintptr_t mgl_ray_trace_(const char *ham, mreal *x0, mreal *y0, mreal *z0, mreal *px, mreal *py, mreal *pz, mreal *dt, mreal *tmax,int l)
{	char *s=new char[l+1];	memcpy(s,ham,l);	s[l]=0;
	uintptr_t res = uintptr_t(mgl_ray_trace(s, *x0,*y0,*z0, *px,*py,*pz, *dt,*tmax));
	delete []s;	return res;	}
uintptr_t mgl_qo2d_solve_(const char *ham, uintptr_t* ini_re, uintptr_t* ini_im, uintptr_t* ray, mreal *r, mreal *k0, uintptr_t* xx, uintptr_t* yy, int l)
{	char *s=new char[l+1];	memcpy(s,ham,l);	s[l]=0;
	uintptr_t res = uintptr_t(mgl_qo2d_solve(s, _DA_(ini_re), _DA_(ini_im), _DA_(ray), *r, *k0, _DM_(xx), _DM_(yy)));
	delete []s;	return res;	}
uintptr_t mgl_jacobian_2d_(uintptr_t* x, uintptr_t* y)
{	return uintptr_t(mgl_jacobian_2d(_DA_(x), _DA_(y)));	}
uintptr_t mgl_jacobian_3d_(uintptr_t* x, uintptr_t* y, uintptr_t* z)
{	return uintptr_t(mgl_jacobian_3d(_DA_(x), _DA_(y), _DA_(z)));	}
//-----------------------------------------------------------------------------
