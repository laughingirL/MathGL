/***************************************************************************
 * pde.cpp is part of Math Graphic Library
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
#include "mgl2/data.h"
#include "mgl2/eval.h"
#include <complex>
#define dual	std::complex<double>
#define GAMMA	0.1
#if MGL_HAVE_GSL
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
MGL_NO_EXPORT void *mgl_pde_hprep(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	const mgl_pde_ham *f = (const mgl_pde_ham *)t->v;
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
HMDT MGL_EXPORT mgl_pde_solve(HMGL gr, const char *ham, HCDT ini_re, HCDT ini_im, mreal dz, mreal k0, const char *opt)
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
#if MGL_HAVE_GSL
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
//	double xx = Min.x - dx*nx/2, yy = Min.x - dy*ny/2;
	double ff = ny>1?4*nx*ny:2*nx, dd = k0*dz;

	mgl_pde_ham tmp;tmp.eqs = &eqs;
	tmp.nx = nx;	tmp.ny = ny;	tmp.a=a;		tmp.hxy=hxy;
	tmp.hxv=hxv;	tmp.huy=huy;	tmp.huv=huv;	tmp.dd = dd;
	tmp.xx = Min.x-dx*(nx/2);	tmp.xs = xs;	tmp.dx = dx;	tmp.dp = dp;
	tmp.yy = Min.y-dy*(ny/2);	tmp.ys = ys;	tmp.dy = dy;	tmp.dq = dq;

	// prepare fft. NOTE: slow procedures due to unknown nx, ny.
	gsl_fft_complex_wavetable *wtx = gsl_fft_complex_wavetable_alloc(2*nx);
	gsl_fft_complex_workspace *wsx = gsl_fft_complex_workspace_alloc(2*nx);
	gsl_fft_complex_wavetable *wty = gsl_fft_complex_wavetable_alloc(2*ny);
	gsl_fft_complex_workspace *wsy = gsl_fft_complex_workspace_alloc(2*ny);
	for(k=1;k<nz;k++)
	{
		if(gr->Stop)
		{
			gsl_fft_complex_workspace_free(wsx);	gsl_fft_complex_wavetable_free(wtx);
			gsl_fft_complex_workspace_free(wsy);	gsl_fft_complex_wavetable_free(wty);
			delete []a;		delete []dmp;	delete []hxy;	delete []hxv;
			delete []huy;	delete []huv;	delete []hx;	delete []hy;
			delete []hu;	delete []hv;	return 0;
		}
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
HMDT MGL_EXPORT mgl_ray_trace(const char *ham, mreal x0, mreal y0, mreal z0, mreal px, mreal py, mreal pz, mreal dt, mreal tmax)
{
	mglData *res=new mglData;
	if(tmax<dt)	return res;	// nothing to do
	int nt = int(tmax/dt)+1;
	mgl_data_create(res,7,nt,1);
	mgl_data_set_id(res,"xyzpqvt");
#if MGL_HAVE_GSL
	mreal x[6], k1[6], k2[6], k3[6], hh=dt/2;
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
		{	var[v[i]-'a'] = x[i]+k3[i]*dt;	k3[i] += k2[i];	}
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
	double x0,y0,z0,x1,y1,z1,x2,y2,z2;	// vectors {l, g1, g2}
	double t1,t2,ch,q1,q2,pt,dt,d1,d2;	// theta_{1,2}, chi, q_{1,2}, p_t, dtau, dq_{1,2}
	mgl_ap()	{	memset(this,0,sizeof(mgl_ap));	}
};
//-----------------------------------------------------------------------------
void MGL_NO_EXPORT mgl_init_ra(int n, const mreal *r, mgl_ap *ra)	// prepare some intermediate data for QO (3d case)
{
	register double tt;
	tt = hypot(r[7]-r[0], r[8]-r[1]);
	if(tt)
	{
		ra[0].x1 = (r[8]-r[1])/tt;
		ra[0].y1 = (r[0]-r[7])/tt;
		ra[0].z1 = 0;
	}
	else	{	ra[0].x1 = ra[0].y1 = 0;	ra[0].z1 = 1;	}
	ra[0].x0 = r[7] - r[0];	ra[0].y0 = r[8] - r[1];	ra[0].z0 = r[9] - r[2];
	tt = sqrt(ra[0].x0*ra[0].x0 + ra[0].y0*ra[0].y0 + ra[0].z0*ra[0].z0);
	ra[0].x0 /= tt;	ra[0].y0 /= tt;	ra[0].z0 /= tt;
	ra[0].x2 = ra[0].y1*ra[0].z0 - ra[0].y0*ra[0].z1;	// vector g_2
	ra[0].y2 = ra[0].z1*ra[0].x0 - ra[0].z0*ra[0].x1;
	ra[0].z2 = ra[0].x1*ra[0].y0 - ra[0].x0*ra[0].y1;
	register long i;
	for(i=1;i<n;i++)	// NOTE: no parallel due to dependence on prev point!
	{
		ra[i].dt = r[6+7*i] - r[7*i-1];
		ra[i].x0 = r[7*i] - r[7*i-7];	// NOTE: very rough formulas
		ra[i].y0 = r[7*i+1] - r[7*i-6];	// for corresponding with dt one
		ra[i].z0 = r[7*i+2] - r[7*i-5];	// for corresponding with dt one
		tt = sqrt(ra[i].x0*ra[i].x0 + ra[i].y0*ra[i].y0 + ra[i].z0*ra[i].z0);
		ra[i].x0 /= tt;	ra[i].y0 /= tt;	ra[i].z0 /= tt;
		ra[i].ch = tt/ra[i].dt;
		tt = ra[i].x0*ra[i-1].x1 + ra[i].y0*ra[i-1].y1 + ra[i].z0*ra[i-1].z1;
		ra[i].x1 = ra[i-1].x1 - tt*ra[i].x0;	// vector g_1
		ra[i].y1 = ra[i-1].y1 - tt*ra[i].y0;
		ra[i].z1 = ra[i-1].z1 - tt*ra[i].z0;
		ra[i].t1 = tt/(ra[i].dt*ra[i].ch);
		tt = sqrt(ra[i].x1*ra[i].x1 + ra[i].y1*ra[i].y1 + ra[i].z1*ra[i].z1);
		ra[i].x1 /= tt;	ra[i].y1 /= tt;	ra[i].z1 /= tt;	// norm for reducing numeric error
		ra[i].x2 = ra[i].y1*ra[i].z0 - ra[i].y0*ra[i].z1;	// vector g_2
		ra[i].y2 = ra[i].z1*ra[i].x0 - ra[i].z0*ra[i].x1;
		ra[i].z2 = ra[i].x1*ra[i].y0 - ra[i].x0*ra[i].y1;
		tt = ra[i].x0*ra[i-1].x2 + ra[i].y0*ra[i-1].y2 + ra[i].z0*ra[i-1].z2;
		ra[i].t2 = tt/(ra[i].dt*ra[i].ch);
		// other parameters
		ra[i].pt = r[7*i+3]*ra[i].x0 + r[7*i+4]*ra[i].y0 + r[7*i+5]*ra[i].z0;
		ra[i].q1 = r[7*i+3]*ra[i].x1 + r[7*i+4]*ra[i].y1 + r[7*i+5]*ra[i].z1;
		ra[i].q2 = r[7*i+3]*ra[i].x2 + r[7*i+4]*ra[i].y2 + r[7*i+5]*ra[i].z2;
		ra[i].d1 = (ra[i].q1-ra[i-1].q1)/(ra[i].dt*ra[i].ch);
		ra[i].d2 = (ra[i].q2-ra[i-1].q2)/(ra[i].dt*ra[i].ch);
	}
	memcpy(ra,ra+1,sizeof(mgl_ap));	// setup zero point
	ra[0].pt = r[3]*ra[0].x0 + r[4]*ra[0].y0 + r[5]*ra[0].z0;
	ra[0].q1 = r[3]*ra[0].x1 + r[4]*ra[0].y1 + r[5]*ra[0].z1;
	ra[0].q2 = r[3]*ra[0].x2 + r[4]*ra[0].y2 + r[5]*ra[0].z2;
}
//-----------------------------------------------------------------------------
//
//		QO2d series
//
//-----------------------------------------------------------------------------
struct mgl_qo2d_ham
{
	dual *hx, *hu, *a, h0;
	double *dmp;
	mreal *r, dr, dk;
	mgl_ap *ra;
	dual (*ham)(mreal u, mreal x, mreal y, mreal px, mreal py, void *par);
	void *par;
};
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void *mgl_qo2d_hprep(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	mgl_qo2d_ham *f = (mgl_qo2d_ham *)t->v;
	mgl_ap *ra = f->ra;
	
	const mreal *r = f->r;
	mreal tt, x1, hh;
	register long i, nx=t->n;
	for(i=t->id;i<nx;i+=mglNumThr)
	{
		// x terms
		x1 = (2*i-nx+1)*f->dr;	hh = 1 - ra->t1*x1;
		hh = sqrt(sqrt(0.041+hh*hh*hh*hh));
		tt = (ra->pt + ra->d1*x1)/hh - ra->pt;
		f->hx[i] = f->ham(abs(f->a[i]), r[0]+ra->x1*x1, r[1]+ra->y1*x1, r[3]+ra->x0*tt, r[4]+ra->y0*tt, f->par) - f->h0/2.;
		// u-y terms
		x1 = f->dk/2*(i<nx/2 ? i:i-nx);
		f->hu[i] = f->ham(0, r[0], r[1], r[3]+ra->x1*x1, r[4]+ra->y1*x1, f->par) - f->h0/2.;

		if(imag(f->hx[i])>0)	f->hx[i] = f->hx[i].real();
		if(imag(f->hu[i])>0)	f->hu[i] = f->hu[i].real();
		// add boundary conditions for x-direction
		f->hx[i] -= dual(0,f->dmp[i]);
	}
	return 0;
}
//-----------------------------------------------------------------------------
HMDT MGL_EXPORT mgl_qo2d_func(dual (*ham)(mreal u, mreal x, mreal y, mreal px, mreal py, void *par), void *par, HCDT ini_re, HCDT ini_im, HCDT ray_dat, mreal r, mreal k0, HMDT xx, HMDT yy)
{
	mglData *res=new mglData;
	const mglData *ray=dynamic_cast<const mglData *>(ray_dat);	// NOTE: Ray must be mglData!
	if(!ray)	return res;
	int nx=ini_re->GetNx(), nt=ray->ny;
	if(nx<2 || ini_im->GetNx()!=nx || nt<2)	return res;
	mgl_data_create(res,nx,nt,1);
#if MGL_HAVE_GSL
	dual *a=new dual[2*nx], *hu=new dual[2*nx],  *hx=new dual[2*nx];
	double *dmp=new double[2*nx];
	mgl_ap *ra = new mgl_ap[nt];	mgl_init_ra(ray->ny, ray->a, ra);	// ray
	register int i;

	mreal dr = r/(nx-1), dk = M_PI*(nx-1)/(k0*r*nx), tt, x1, hh, B1, pt0;

	memset(dmp,0,2*nx*sizeof(double));
	for(i=0;i<nx/2;i++)	// prepare damping
	{
		x1 = (nx/2-i)/(nx/2.);
		dmp[2*nx-1-i] = dmp[i] = 30*GAMMA*x1*x1/k0;
	}
	for(i=0;i<nx;i++)	a[i+nx/2] = dual(ini_re->v(i),ini_im->v(i));	// init
	gsl_fft_complex_wavetable *wtx = gsl_fft_complex_wavetable_alloc(2*nx);
	gsl_fft_complex_workspace *wsx = gsl_fft_complex_workspace_alloc(2*nx);
	if(xx && yy)	{	xx->Create(nx,nt);	yy->Create(nx,nt);	}

	mgl_qo2d_ham tmp;	// parameters for Hamiltonian calculation
	tmp.hx=hx;	tmp.hu=hu;	tmp.dmp=dmp;	tmp.par=par;
	tmp.dr=dr;	tmp.dk=dk;	tmp.ham=ham;	tmp.a=a;
	// start calculation
	for(int k=0;k<nt;k++)
	{
		for(i=0;i<nx;i++)	// "save"
			res->a[i+k*nx]=abs(a[i+nx/2])*sqrt(ra[0].ch/ra[k].ch);
		if(xx && yy)	for(i=0;i<nx;i++)	// prepare xx, yy
		{
			x1 = (2*i-nx+1)*dr;
			xx->a[i+k*nx] = ray->a[7*k] + ra[k].x1*x1;	// new coordinates
			yy->a[i+k*nx] = ray->a[7*k+1] + ra[k].y1*x1;
		}
		tmp.r=ray->a+7*k;	tmp.ra=ra+k;
		hh = ra[k].pt*(1/sqrt(sqrt(1.041))-1);	// 0.041=0.45^4 -- minimal value of h
		tmp.h0 = ham(0, tmp.r[0], tmp.r[1], tmp.r[3]+ra[k].x0*hh, tmp.r[4]+ra[k].x0*hh, par);
		mglStartThread(mgl_qo2d_hprep,0,2*nx,0,0,0,0,&tmp);
		// Step for field
		dual dt = dual(0, -ra[k].dt*k0);
		for(i=0;i<2*nx;i++)		a[i] *= exp(hx[i]*dt);
		gsl_fft_complex_transform((double *)a, 1, 2*nx, wtx, wsx, forward);
		for(i=0;i<2*nx;i++)		a[i] *= exp(hu[i]*dt)/(2.*nx);
		gsl_fft_complex_transform((double *)a, 1, 2*nx, wtx, wsx, backward);

/*		// Calculate B1			// TODO make more general scheme later!!!
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
		double a1=0, a2=0;
		for(i=0;i<2*nx;i++)	a1 += norm(a[i]);
		hx[0] = hx[2*nx-1] = 0.;
		for(i=1;i<2*nx-1;i++)	hx[i] = (B1*ra[k].dt*(i-nx))*(a[i+1]-a[i-1]);
		for(i=0;i<2*nx;i++)	{	a[i] += hx[i];	a2 += norm(a[i]);	}
		a1 = sqrt(a1/a2);
		for(i=0;i<2*nx;i++)	a[i] *= a1;*/
	}
	gsl_fft_complex_workspace_free(wsx);
	gsl_fft_complex_wavetable_free(wtx);
	delete []a;		delete []hu;	delete []hx;	delete []ra;	delete []dmp;
#endif
	return res;
}
//-----------------------------------------------------------------------------
dual MGL_NO_EXPORT mgl_ham2d(mreal u, mreal x, mreal y, mreal px, mreal py, void *par)
{
	mglFormula *h = (mglFormula *)par;
	mreal var[MGL_VS];	memset(var,0,MGL_VS*sizeof(mreal));
	var['x'-'a'] = x;	var['y'-'a'] = y;	var['u'-'a'] = u;
	var['p'-'a'] = px;	var['q'-'a'] = py;
	return dual(h->Calc(var), -h->CalcD(var,'i'));
}
//-----------------------------------------------------------------------------
HMDT MGL_EXPORT mgl_qo2d_solve(const char *ham, HCDT ini_re, HCDT ini_im, HCDT ray_dat, mreal r, mreal k0, HMDT xx, HMDT yy)
{
	mglFormula h(ham);
	return mgl_qo2d_func(mgl_ham2d, &h, ini_re, ini_im, ray_dat, r, k0, xx, yy);
}
//-----------------------------------------------------------------------------
//
//		QO2d series
//
//-----------------------------------------------------------------------------
struct mgl_qo3d_ham
{
	dual *hxy, *huv, *hxv, *huy, *a;
	dual *hx, *hy, *hu, *hv, h0;
	double *dmp;
	mreal *r, dr, dk;
	mgl_ap *ra;
	dual (*ham)(mreal u, mreal x, mreal y, mreal z, mreal px, mreal py, mreal pz, void *par);
	void *par;
};
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void *mgl_qo3d_hprep(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	mgl_qo3d_ham *f = (mgl_qo3d_ham *)t->v;
	mgl_ap *ra = f->ra;

	const mreal *r = f->r;
	mreal tt, x1, x2, hh;
	register long ii,i,j, nx=t->n;
	for(ii=t->id;ii<nx*nx;ii+=mglNumThr)
	{
		i = ii%nx;	j = ii/nx;
		// x-y terms
		x1 = (2*i-nx+1)*f->dr;	x2 = (2*j-nx+1)*f->dr;	hh = 1-ra->t1*x1-ra->t2*x2;
		hh = sqrt(sqrt(0.041+hh*hh*hh*hh));
		tt = (ra->pt + ra->d1*x1 + ra->d2*x2)/hh - ra->pt;
		f->hxy[ii] = f->ham(abs(f->a[i]), r[0]+ra->x1*x1+ra->x2*x2, r[1]+ra->y1*x1+ra->y2*x2, r[2]+ra->z1*x1+ra->z2*x2, r[3]+ra->x0*tt, r[4]+ra->y0*tt, r[5]+ra->z0*tt, f->par);
		// x-v terms
		x1 = (2*i-nx+1)*f->dr;	x2 = f->dk/2*(j<nx/2 ? j:j-nx);	hh = 1-ra->t1*x1;
		hh = sqrt(sqrt(0.041+hh*hh*hh*hh));
		tt = (ra->pt + ra->d1*x1)/hh - ra->pt;
		f->hxv[ii] = f->ham(0, r[0]+ra->x1*x1, r[1]+ra->y1*x1, r[2]+ra->z1*x1, r[3]+ra->x0*tt+ra->x2*x2, r[4]+ra->y0*tt+ra->y2*x2, r[5]+ra->z0*tt+ra->z2*x2, f->par);
		// u-y terms
		x1 = f->dk/2*(i<nx/2 ? i:i-nx);	x2 = (2*j-nx+1)*f->dr;	hh = 1-ra->t2*x2;
		hh = sqrt(sqrt(0.041+hh*hh*hh*hh));
		tt = (ra->pt + ra->d2*x2)/hh - ra->pt;
		f->huy[ii] = f->ham(0, r[0]+ra->x2*x2, r[1]+ra->y2*x2, r[2]+ra->z2*x2, r[3]+ra->x1*x1+ra->x0*tt, r[4]+ra->y1*x1+ra->y0*tt, r[5]+ra->z1*x1+ra->z0*tt, f->par);
		// u-y terms
		x1 = f->dk/2*(i<nx/2 ? i:i-nx);	x2 = f->dk/2*(j<nx/2 ? j:j-nx);
		f->huv[ii] = f->ham(0, r[0], r[1], r[2], r[3]+ra->x1*x1+ra->x2*x2, r[4]+ra->y1*x1+ra->y2*x2, r[5]+ra->z1*x1+ra->z2*x2, f->par);
	}
	return 0;
}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void *mgl_qo3d_post(void *par)
{
	mglThreadD *t=(mglThreadD *)par;
	mgl_qo3d_ham *f = (mgl_qo3d_ham *)t->v;
	
	register long ii,i,j, nx=t->n;
	for(ii=t->id;ii<nx*nx;ii+=mglNumThr)
	{
		i = ii%nx;	j = ii/nx;
		f->hxy[ii] -= (f->hx[i]+f->hy[j]-f->h0/2.)/2.;
		if(imag(f->hxy[ii])>0)	f->hxy[ii] = f->hxy[ii].real();
		f->hxv[ii] -= (f->hx[i]+f->hv[j]-f->h0/2.)/2.;
		if(imag(f->hxv[ii])>0)	f->hxv[ii] = f->hxv[ii].real();
		f->huy[ii] -= (f->hu[i]+f->hy[j]-f->h0/2.)/2.;
		if(imag(f->huy[ii])>0)	f->huy[ii] = f->huy[ii].real();
		f->huv[ii] -= (f->hu[i]+f->hv[j]-f->h0/2.)/2.;
		if(imag(f->huv[ii])>0)	f->huv[ii] = f->huv[ii].real();
		// add boundary conditions for x-direction
		f->hxy[ii] -= dual(0,f->dmp[ii]);
	}
	return 0;
}
//-----------------------------------------------------------------------------
HMDT MGL_EXPORT mgl_qo3d_func(dual (*ham)(mreal u, mreal x, mreal y, mreal z, mreal px, mreal py, mreal pz, void *par), void *par, HCDT ini_re, HCDT ini_im, HCDT ray_dat, mreal r, mreal k0, HMDT xx, HMDT yy, HMDT zz)
{
	mglData *res=new mglData;
	const mglData *ray=dynamic_cast<const mglData *>(ray_dat);	// NOTE: Ray must be mglData!
	if(!ray)	return res;
	int nx=ini_re->GetNx(), nt=ray->ny;	// NOTE: only square grids are supported now (for simplicity)
	if(nx<2 || ini_re->GetNx()!=nx || ini_im->GetNx()*ini_im->GetNy()!=nx*nx || nt<2)	return res;
	mgl_data_create(res,nx,nx,nt);
#if MGL_HAVE_GSL
	dual *a=new dual[4*nx*nx], *huv=new dual[4*nx*nx],  *hxy=new dual[4*nx*nx], *huy=new dual[4*nx*nx],  *hxv=new dual[4*nx*nx];
	dual *hu=new dual[2*nx],  *hx=new dual[2*nx], *hy=new dual[2*nx],  *hv=new dual[2*nx];
	mreal *dmp=new mreal[4*nx*nx];
	mgl_ap *ra = new mgl_ap[nt];
	mgl_init_ra(ray->ny, ray->a, ra);	// prepare ray
	register int i,j,ii;

	mreal dr = r/(nx-1), dk = M_PI*(nx-1)/(k0*r*nx), tt, x1, x2, hh, B1, pt0;

	memset(dmp,0,4*nx*nx*sizeof(mreal));
	for(i=0;i<nx/2;i++)	for(j=0;j<nx/2;j++)	// prepare damping
	{
		x1 = (nx/2-i)/(nx/2.);	x2 = (nx/2-j)/(nx/2.);
		dmp[2*nx-1-i] = dmp[i] = 30*GAMMA*x1*x1/k0;
		dmp[(2*nx-1-j)*2*nx] += 30*GAMMA*x2*x2/k0;
		dmp[j*2*nx] += 30*GAMMA*x2*x2/k0;
	}
	for(i=0;i<nx;i++)	for(j=0;j<nx;j++)	// init
		a[i+nx/2+2*nx*(j+nx/2)] = dual(ini_re->v(i,j),ini_im->v(i,j));
	gsl_fft_complex_wavetable *wtx = gsl_fft_complex_wavetable_alloc(2*nx);
	gsl_fft_complex_workspace *wsx = gsl_fft_complex_workspace_alloc(2*nx);
	if(xx && yy && zz)	{	xx->Create(nx,nx,nt);	yy->Create(nx,nx,nt);	zz->Create(nx,nx,nt);	}

	mgl_qo3d_ham tmp;	// parameters for Hamiltonian calculation
	tmp.hxy=hxy;	tmp.hx=hx;	tmp.huv=huv;	tmp.hu=hu;
	tmp.huy=huy;	tmp.hy=hy;	tmp.hxv=hxv;	tmp.hv=hv;
	tmp.dmp=dmp;	tmp.par=par;
	tmp.dr=dr;	tmp.dk=dk;	tmp.ham=ham;	tmp.a=a;
	// start calculation
	for(int k=0;k<nt;k++)
	{
		for(i=0;i<nx;i++)	for(j=0;j<nx;j++)	// "save"
			res->a[i+nx*(j+k*nx)]=abs(a[i+nx/2+2*nx*(j+nx/2)])*sqrt(ra[0].ch/ra[k].ch);
		if(xx && yy && zz)	for(i=0;i<nx;i++)	for(j=0;j<nx;j++)	// prepare xx, yy, zz
		{
			x1 = (2*i-nx+1)*dr;	x2 = (2*j-nx+1)*dr;
			xx->a[i+nx*(j+k*nx)] = ray->a[7*k] + ra[k].x1*x1 + ra[k].x2*x2;	// new coordinates
			yy->a[i+nx*(j+k*nx)] = ray->a[7*k+1] + ra[k].y1*x1 + ra[k].y2*x2;
			zz->a[i+nx*(j+k*nx)] = ray->a[7*k+2] + ra[k].z1*x1 + ra[k].z2*x2;
		}
		tmp.r=ray->a+7*k;	tmp.ra=ra+k;
		mglStartThread(mgl_qo3d_hprep,0,2*nx,0,0,0,0,&tmp);	tmp.h0 = huv[0];
		for(i=0;i<2*nx;i++)	// fill intermediate arrays
		{
			tmp.hx[i] = hxv[i];	tmp.hy[i] = huy[i*2*nx];
			tmp.hv[i] = huv[i];	tmp.hu[i] = huv[i*2*nx];
		}
		mglStartThread(mgl_qo3d_post,0,2*nx,0,0,0,0,&tmp);
		// Step for field
		dual dt = dual(0, -ra[k].dt*k0);	// TODO: this part can be paralleled
		for(i=0;i<4*nx*nx;i++)	a[i] *= exp(hxy[i]*dt);		// x-y
		for(i=0;i<2*nx;i++)	// x->u
			gsl_fft_complex_transform((double *)(a+i*2*nx), 1, 2*nx, wtx, wsx, forward);
		for(i=0;i<4*nx*nx;i++)	a[i] *= exp(huy[i]*dt);		// u-y
		for(i=0;i<2*nx;i++)	// y->v
			gsl_fft_complex_transform((double *)(a+i), 2*nx, 2*nx, wtx, wsx, forward);
		for(i=0;i<4*nx*nx;i++)	a[i] *= exp(huv[i]*dt)/(4.*nx*nx);	// u-v
		for(i=0;i<2*nx;i++)	// u->x
			gsl_fft_complex_transform((double *)(a+i*2*nx), 1, 2*nx, wtx, wsx, backward);
		for(i=0;i<4*nx*nx;i++)	a[i] *= exp(hxv[i]*dt);		// x-v
		for(i=0;i<2*nx;i++)	// v->y
			gsl_fft_complex_transform((double *)(a+i), 2*nx, 2*nx, wtx, wsx, backward);
		
/*		// Calculate B1			// TODO make more general scheme later!!!
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
		double a1=0, a2=0;
		for(i=0;i<2*nx;i++)	a1 += norm(a[i]);
		hx[0] = hx[2*nx-1] = 0.;
		for(i=1;i<2*nx-1;i++)	hx[i] = (B1*ra[k].dt*(i-nx))*(a[i+1]-a[i-1]);
		for(i=0;i<2*nx;i++)	{	a[i] += hx[i];	a2 += norm(a[i]);	}
		a1 = sqrt(a1/a2);
		for(i=0;i<2*nx;i++)	a[i] *= a1;*/
	}
	gsl_fft_complex_workspace_free(wsx);
	gsl_fft_complex_wavetable_free(wtx);
	delete []a;
	delete []ra;
	delete []dmp;
	delete []huv;	delete []hxy;	delete []hxv;	delete []huy;
	delete []hu;	delete []hx;	delete []hv;	delete []hy;
#endif
	return res;
}
//-----------------------------------------------------------------------------
dual MGL_NO_EXPORT mgl_ham3d(mreal u, mreal x, mreal y, mreal z, mreal px, mreal py, mreal pz, void *par)
{
	mglFormula *h = (mglFormula *)par;
	mreal var[MGL_VS];	memset(var,0,MGL_VS*sizeof(mreal));
	var['x'-'a'] = x;	var['y'-'a'] = y;	var['z'-'a'] = z;	var['u'-'a'] = u;
	var['p'-'a'] = px;	var['q'-'a'] = py;	var['v'-'a'] = pz;
	return dual(h->Calc(var), -h->CalcD(var,'i'));
}
//-----------------------------------------------------------------------------
HMDT MGL_EXPORT mgl_qo3d_solve(const char *ham, HCDT ini_re, HCDT ini_im, HCDT ray_dat, mreal r, mreal k0, HMDT xx, HMDT yy, HMDT zz)
{
	mglFormula h(ham);
	return mgl_qo3d_func(mgl_ham3d, &h, ini_re, ini_im, ray_dat, r, k0, xx, yy, zz);
}
//-----------------------------------------------------------------------------
MGL_NO_EXPORT void *mgl_jacob2(void *par)
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
HMDT MGL_EXPORT mgl_jacobian_2d(HCDT x, HCDT y)
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
MGL_NO_EXPORT void *mgl_jacob3(void *par)
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
HMDT MGL_EXPORT mgl_jacobian_3d(HCDT x, HCDT y, HCDT z)
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
uintptr_t MGL_EXPORT mgl_pde_solve_(uintptr_t* gr, const char *ham, uintptr_t* ini_re, uintptr_t* ini_im, mreal *dz, mreal *k0, const char *opt, int l, int lo)
{	char *s=new char[l+1];	memcpy(s,ham,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	uintptr_t res = uintptr_t(mgl_pde_solve(_GR_, s, _DA_(ini_re), _DA_(ini_im), *dz, *k0, o));
	delete []o;	delete []s;	return res;	}
uintptr_t MGL_EXPORT mgl_ray_trace_(const char *ham, mreal *x0, mreal *y0, mreal *z0, mreal *px, mreal *py, mreal *pz, mreal *dt, mreal *tmax,int l)
{	char *s=new char[l+1];	memcpy(s,ham,l);	s[l]=0;
	uintptr_t res = uintptr_t(mgl_ray_trace(s, *x0,*y0,*z0, *px,*py,*pz, *dt,*tmax));
	delete []s;	return res;	}
uintptr_t MGL_EXPORT mgl_qo2d_solve_(const char *ham, uintptr_t* ini_re, uintptr_t* ini_im, uintptr_t* ray, mreal *r, mreal *k0, uintptr_t* xx, uintptr_t* yy, int l)
{	char *s=new char[l+1];	memcpy(s,ham,l);	s[l]=0;
	uintptr_t res = uintptr_t(mgl_qo2d_solve(s, _DA_(ini_re), _DA_(ini_im), _DA_(ray), *r, *k0, _DM_(xx), _DM_(yy)));
	delete []s;	return res;	}
uintptr_t MGL_EXPORT mgl_qo3d_solve_(const char *ham, uintptr_t* ini_re, uintptr_t* ini_im, uintptr_t* ray, mreal *r, mreal *k0, uintptr_t* xx, uintptr_t* yy, uintptr_t* zz, int l)
{	char *s=new char[l+1];	memcpy(s,ham,l);	s[l]=0;
	uintptr_t res = uintptr_t(mgl_qo3d_solve(s, _DA_(ini_re), _DA_(ini_im), _DA_(ray), *r, *k0, _DM_(xx), _DM_(yy), _DM_(zz)));
	delete []s;	return res;	}
uintptr_t MGL_EXPORT mgl_jacobian_2d_(uintptr_t* x, uintptr_t* y)
{	return uintptr_t(mgl_jacobian_2d(_DA_(x), _DA_(y)));	}
uintptr_t MGL_EXPORT mgl_jacobian_3d_(uintptr_t* x, uintptr_t* y, uintptr_t* z)
{	return uintptr_t(mgl_jacobian_3d(_DA_(x), _DA_(y), _DA_(z)));	}
//-----------------------------------------------------------------------------
