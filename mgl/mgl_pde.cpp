/* mgl_pde.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <mathgl.abalakin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "mgl/mgl_eval.h"
#include "mgl/mgl_data.h"
#include <complex>
#define dual	std::complex<double>
#define GAMMA	0.1
#ifndef NO_GSL
#include <gsl/gsl_fft_complex.h>
#endif
//-----------------------------------------------------------------------------
// Solving equation du/dz = ham(p,q,x,y,z,|u|)[u] where p=d/dx, q=d/dy. At this moment simplified form of ham is supported: ham = f(p,q,z) + g(x,y,z,'u'), where variable 'u'=|u| (for allowing solve nonlinear problems). You may specify imaginary part like ham = p^2 + i*x*(x>0) but only if dependence on variable 'i' is linear (i.e. ham = hre+i*him).
mglData mglPDE(const char *ham, const mglData &ini_re, const mglData &ini_im, mglPoint Min, mglPoint Max, float dz, float k0)
{
	mglData res;
	int nx=ini_re.nx, ny=ini_re.ny;
	int nz = int((Max.z-Min.z)/dz)+1;
	if(nx<2 || nz<1 || Max.x==Min.x)	return res;	// Too small data
	if(ini_im.nx*ini_im.ny != nx*ny)	return res;		// Wrong dimensions
	res.Create(nz, nx, ny);
	mglFormula eqs(ham);
	dual *a = new dual[4*nx*ny], h, h0, h1, h2;	// Add "damping" area
	memset(a,0,4*nx*ny*sizeof(dual));
	register int i,j,k,i0;
	for(j=0;j<ny;j++)	for(i=0;i<nx;i++)	// Initial conditions
	{
		i0 = i+nx/2+2*ny*(j+ny/2);
		a[i0] = dual(ini_re.a[i+nx*j], ini_im.a[i+nx*j]);
		res.a[nz*(i+nx*j)] = abs(a[i0]);
	}
	float dx = (Max.x-Min.x)/(nx-1), dy = ny>1?(Max.y-Min.y)/(ny-1):0;
	float dp = M_PI/(Max.x-Min.x)/k0, dq = M_PI/(Max.y-Min.y)/k0;
	float var['z'-'a'+1], xs=(Min.x+Max.x)/2, ys=(Min.y+Max.y)/2, tmp;
	double xx = Min.x - dx*nx/2, yy = Min.x - dy*ny/2;
	double ff = ny>1?4*nx*ny:2*nx, dd = k0*dz;
	memset(var,0,('z'-'a'+1)*sizeof(float));
	// prepare fft. NOTE: slow procedures due to unknown nx, ny.
	gsl_fft_complex_wavetable *wtx = gsl_fft_complex_wavetable_alloc(2*nx);
	gsl_fft_complex_workspace *wsx = gsl_fft_complex_workspace_alloc(2*nx);
	gsl_fft_complex_wavetable *wty = gsl_fft_complex_wavetable_alloc(2*ny);
	gsl_fft_complex_workspace *wsy = gsl_fft_complex_workspace_alloc(2*ny);
	for(k=1;k<nz;k++)
	{
		var['z'-'a'] = Min.z+dz*k;
		for(j=0;j<2*ny;j++)	for(i=0;i<2*nx;i++)	// step 1
		{
			i0 = i+2*nx*j;
			var['x'-'a'] = xx+dx*i;		var['p'-'a'] = 0;
			var['y'-'a'] = yy+dy*j;		var['q'-'a'] = 0;
			var['u'-'a'] = abs(a[i0]);
			h = dual(-eqs.CalcD(var,'i'), eqs.Calc(var))*dd;
			tmp = 0;
			if(i<nx/2)		tmp += GAMMA*ipow((nx/2-i)/(nx/2.),2);
			if(i>3*nx/2)	tmp += GAMMA*ipow((i-3*nx/2-1)/(nx/2.),2);
			if(j<ny/2)		tmp += GAMMA*ipow((ny/2-j)/(ny/2.),2);
			if(j>3*ny/2)	tmp += GAMMA*ipow((j-3*ny/2-1)/(ny/2.),2);
			a[i0] *= exp(h)*exp(-tmp*dz);
		}
		// "central" point
		var['x'-'a'] = xs;	var['y'-'a'] = ys;
		var['u'-'a'] = var['p'-'a'] = var['q'-'a'] = 0;
		h0 = dual(-eqs.CalcD(var,'i'), eqs.Calc(var))*dd;
		// do fft
		for(i=0;i<ny;i++)
			gsl_fft_complex_transform((double *)(a+i*2*nx), 1, 2*nx, wtx, wsx, forward);
		if(ny>1) for(j=0;j<2*ny;j++)	for(i=0;i<2*nx;i++)	// step 3/2
		{
			i0 = i+2*nx*j;
			var['x'-'a'] = xs;			var['p'-'a'] = 0;
			var['y'-'a'] = yy+dy*j;		var['q'-'a'] = 0;
			h1 = dual(-eqs.CalcD(var,'i'), eqs.Calc(var))*dd;
			var['x'-'a'] = xs;			var['p'-'a'] = dp*(i<nx ? i:2*nx-i);
			var['y'-'a'] = ys;			var['q'-'a'] = 0;
			h2 = dual(-eqs.CalcD(var,'i'), eqs.Calc(var))*dd;
			var['x'-'a'] = xs;			var['p'-'a'] = dp*(i<nx ? i:2*nx-i);
			var['y'-'a'] = yy+dy*j;		var['q'-'a'] = 0;
			h = dual(-eqs.CalcD(var,'i'), eqs.Calc(var))*dd;
			a[i0] *= exp(h-h1-h2+h0);
		}
		if(ny>1) for(i=0;i<nx;i++)
			gsl_fft_complex_transform((double *)(a+i), 2*nx, 2*ny, wty, wsy, forward);
		for(j=0;j<2*ny;j++)	for(i=0;i<2*nx;i++)	// step 2
		{
			i0 = i+2*nx*j;
			var['x'-'a'] = xs;			var['p'-'a'] = dp*(i<nx ? i:2*nx-i);
			var['y'-'a'] = ys;			var['q'-'a'] = dq*(j<ny ? j:2*ny-j);
			h = dual(-eqs.CalcD(var,'i'), eqs.Calc(var))*dd;
			a[i0] *= exp(h-h0)/ff;
		}
		// do ifft
		if(ny>1) for(i=0;i<nx;i++)
			gsl_fft_complex_transform((double *)(a+i), 2*nx, 2*ny, wty, wsy, backward);
		if(ny>1) for(j=0;j<2*ny;j++)	for(i=0;i<2*nx;i++)	// step 3/2
		{
			i0 = i+2*nx*j;
			var['x'-'a'] = xx+dx*i;		var['p'-'a'] = 0;
			var['y'-'a'] = ys;			var['q'-'a'] = 0;
			h1 = dual(-eqs.CalcD(var,'i'), eqs.Calc(var))*dd;
			var['x'-'a'] = xs;			var['p'-'a'] = 0;
			var['y'-'a'] = ys;			var['q'-'a'] = dq*(j<ny ? j:2*ny-j);
			h2 = dual(-eqs.CalcD(var,'i'), eqs.Calc(var))*dd;
			var['x'-'a'] = xx+dx*i;		var['p'-'a'] = 0;
			var['y'-'a'] = ys;			var['q'-'a'] = dq*(j<ny ? j:2*ny-j);
			h = dual(-eqs.CalcD(var,'i'), eqs.Calc(var))*dd;
			a[i0] *= exp(h-h1-h2+h0);
		}
		for(i=0;i<ny;i++)
			gsl_fft_complex_transform((double *)(a+2*i*nx), 1, 2*nx, wtx, wsx, backward);
		// save result
		for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
			res.a[k+nz*(i+nx*j)] = abs(a[i+nx/2+2*ny*(j+ny/2)]);
	}
	gsl_fft_complex_workspace_free(wsx);
	gsl_fft_complex_wavetable_free(wtx);
	gsl_fft_complex_workspace_free(wsy);
	gsl_fft_complex_wavetable_free(wty);
	delete []a;		return res;
}
//-----------------------------------------------------------------------------
// Solve GO ray equation like dr/dt = d ham/dp, dp/dt = -d ham/dr where ham = ham(x,y,z,p,q,v,t) and px=p, py=q, pz=v. The starting point (at t=0) is r0, p0. Result is array of {x,y,z,p,q,v,t}
mglData mglRay(const char *ham, mglPoint r0, mglPoint p0, float dt, float tmax)
{
	mglData res;
	if(tmax<dt)	return res;	// nothing to do
	int nt = int(tmax/dt)+1;
	float x[6], k1[6], k2[6], k3[6], hh=dt/2;
	res.Create(7,nt);	res.SetColumnId("xyzpqvt");
	mglFormula eqs(ham);
	// initial conditions
	x[0] = res.a[0] = r0.x;	x[1] = res.a[1] = r0.y;	x[2] = res.a[2] = r0.z;
	x[3] = res.a[3] = p0.x;	x[4] = res.a[4] = p0.y;	x[5] = res.a[5] = p0.z;
	res.a[6] = 0;
	// Runge Kutta scheme of 4th order
	char v[7]="xyzpqv";
	float var['z'-'a'+1];
	register int i,k;
	for(k=1;k<nt;k++)
	{
		var['t'-'a']=k*dt;
		// 		md->H(cy,k1);
		for(i=0;i<6;i++)	var[v[i]-'a'] = x[i];
		k1[0] = eqs.CalcD(var,'p');	k1[3] = -eqs.CalcD(var,'x');
		k1[1] = eqs.CalcD(var,'q');	k1[4] = -eqs.CalcD(var,'y');
		k1[2] = eqs.CalcD(var,'v');	k1[5] = -eqs.CalcD(var,'z');
		// 		ty = cy/(k1*hh);	md->H(ty,k2);
		for(i=0;i<6;i++)	var[v[i]-'a'] = x[i]+k1[i]*hh;
		k2[0] = eqs.CalcD(var,'p');	k2[3] = -eqs.CalcD(var,'x');
		k2[1] = eqs.CalcD(var,'q');	k2[4] = -eqs.CalcD(var,'y');
		k2[2] = eqs.CalcD(var,'v');	k2[5] = -eqs.CalcD(var,'z');
		//		ty = cy/(k2*hh);	md->H(ty,k3);
		for(i=0;i<6;i++)	var[v[i]-'a'] = x[i]+k2[i]*hh;
		k3[0] = eqs.CalcD(var,'p');	k3[3] = -eqs.CalcD(var,'x');
		k3[1] = eqs.CalcD(var,'q');	k3[4] = -eqs.CalcD(var,'y');
		k3[2] = eqs.CalcD(var,'v');	k3[5] = -eqs.CalcD(var,'z');
		//		ty = cy/(k2*h);	k3+=k2;	md->H(ty,k2);
		for(i=0;i<6;i++)
		{	var[v[i]-'a'] = x[i]+k2[i]*dt;	k3[i] += k2[i];	}
		k2[0] = eqs.CalcD(var,'p');	k2[3] = -eqs.CalcD(var,'x');
		k2[1] = eqs.CalcD(var,'q');	k2[4] = -eqs.CalcD(var,'y');
		k2[2] = eqs.CalcD(var,'v');	k2[5] = -eqs.CalcD(var,'z');
		//		cy /= (k1+k2+k3*2.)*(h/6);
		for(i=0;i<6;i++)
			res.a[i+7*k] = x[i] += (k1[i]+k2[i]+2*k3[i])*dt/6;
		res.a[6+7*k] = dt*k;
	}
	return res;
}
//-----------------------------------------------------------------------------
mglData mglQO_PDE(char *ham, const mglData &ini_re, const mglData &ini_im, mglData &ray, float r, const mglData *xx, const mglData *yy, const mglData *zz);
//-----------------------------------------------------------------------------
