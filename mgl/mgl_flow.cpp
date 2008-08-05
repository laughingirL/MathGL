/* mgl_flow.cpp is part of Math Graphic Library
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
#include "mgl/mgl.h"
#include "mgl/mgl_c.h"
#include "mgl/mgl_f.h"
//-----------------------------------------------------------------------------
//
//	Flow series
//
//-----------------------------------------------------------------------------
void mglGraph::flow(bool simple, float zVal, float u, float v,
					const mglData &x, const mglData &y, const mglData &ax, const mglData &ay)
{
	long n=10*(ax.nx+ax.ny);
	bool both = x.nx==ax.nx && y.nx==ax.nx && x.ny==ax.ny && y.ny==ax.ny;
	float *pp, *cc;
	bool *tt;
	pp = (float *)malloc(3*n*sizeof(float));
	cc = (float *)malloc(n*sizeof(float));
	tt = (bool  *)malloc(n*sizeof(bool));
	float dt = 0.5/(ax.nx > ax.ny ? ax.nx : ax.ny),e,f,g,ff[4],gg[4],h,s=1;
	if(u<0 || v<0)	{	dt = -dt;	u = -u;		v = -v;		s = -1;}
	register long k=0,m;
	bool end = false;
	do{
		if(k>=n)	// resize array if need
		{
			n += 10*(ax.nx+ax.ny);
			pp = (float *)realloc(pp,3*n*sizeof(float));
			cc = (float *)realloc(cc,n*sizeof(float));
			tt = (bool  *)realloc(tt,n*sizeof(bool));
		}
		// insert point
		pp[3*k] = simple ? Min.x + (Max.x-Min.x)*u :
				(both ? x.Spline1(u,v,0):x.Spline1(u,0,0));
		pp[3*k+1] = simple ? Min.y + (Max.y-Min.y)*v :
				(both ? y.Spline1(u,v,0):y.Spline1(v,0,0));
		pp[3*k+2] = zVal;
		tt[k]  = ScalePoint(pp[3*k],pp[3*k+1],pp[3*k+2]);
		for(m=0;m<k-1;m++)	// determines encircle
		{
			f = pp[3*k]-pp[3*m];
			g = pp[3*k+1]-pp[3*m+1];
			e = pp[3*k+2]-pp[3*m+2];
			if(f*f+g*g+e*e<dt*dt/4.)	{	end = true;	break;	}
		}
		f = ax.Linear1(u,v,0);	g = ay.Linear1(u,v,0);
//		f = ax.Spline1(u,v,0);	g = ay.Spline1(u,v,0);
		h = hypot(f,g);	cc[k] = s*h;	h+=1;
		k++;
		// find next point by midpoint method
		if(h<1e-7)	break;	// stationary point
		ff[0]=f*dt/h;	gg[0]=g*dt/h;
		e = u+ff[0]/2;	h = v+gg[0]/2;
		f = ax.Linear1(e,h,0);	g = ay.Linear1(e,h,0);	h = 1+hypot(f,g);
//		f = ax.Spline1(e,h,0);	g = ay.Spline1(e,h,0);	h = 1+hypot(f,g);
		if(h<1e-7)	break;	// stationary point
		ff[1]=f*dt/h;	gg[1]=g*dt/h;
		e = u+ff[1]/2;	h = v+gg[1]/2;
		f = ax.Linear1(e,h,0);	g = ay.Linear1(e,h,0);	h = 1+hypot(f,g);
//		f = ax.Spline1(e,h,0);	g = ay.Spline1(e,h,0);	h = 1+hypot(f,g);
		if(h<1e-7)	break;	// stationary point
		ff[2]=f*dt/h;	gg[2]=g*dt/h;
		e = u+ff[2];	h = v+gg[2];
		f = ax.Linear1(e,h,0);	g = ay.Linear1(e,h,0);	h = 1+hypot(f,g);
//		f = ax.Spline1(e,h,0);	g = ay.Spline1(e,h,0);	h = 1+hypot(f,g);
		if(h<1e-7)	break;	// stationary point
		ff[3]=f*dt/h;	gg[3]=g*dt/h;
		u += ff[0]/6+ff[1]/3+ff[2]/3+ff[3]/6;
		v += gg[0]/6+gg[1]/3+gg[2]/3+gg[3]/6;
		// condition of end
		end = end || u<0 || v<0 || u>1 || v>1;
	} while(!end);
	curv_plot(k,pp,cc,tt);
	free(pp);	free(cc);	free(tt);
}
//-----------------------------------------------------------------------------
void mglGraph::Flow(const mglData &x, const mglData &y, const mglData &ax, const mglData &ay, const char *sch, int num, bool cnt, float zVal)
{
	float u,v;
	long n=ax.nx, m=ax.ny;
	if(n*m*ax.nz!=ay.nx*ay.ny*ay.nz)	{	SetWarn(mglWarnDim,"Flow");	return;	}
	if(n<2 || m<2)						{	SetWarn(mglWarnLow,"Flow");	return;	}
	bool both = x.nx==n && y.nx==n && x.ny==m && y.ny==m;
	if(!(both || (x.nx==n && y.nx==m)))	{	SetWarn(mglWarnDim,"Flow");	return;	}

	Arrow1 = Arrow2 = '_';
	SetScheme(sch);
	// allocate memory
	if(isnan(zVal)) zVal = Min.z;

	for(long k=0;k<ax.nz;k++)
	{
		if(ax.nz>1)	zVal = Min.z+(Max.z-Min.z)*float(k)/(ax.nz-1);
		for(long i=0;i<num;i++)
		{
			u = 0;	v = (i+1.)/(num+1.);
			flow(false, zVal, u, v, x, y, ax, ay);
			flow(false, zVal, -u, -v, x, y, ax, ay);
			u = 1;	v = (i+1.)/(num+1.);
			flow(false, zVal, u, v, x, y, ax, ay);
			flow(false, zVal, -u, -v, x, y, ax, ay);
			u = (i+1.)/(num+1.);	v = 0;
			flow(false, zVal, u, v, x, y, ax, ay);
			flow(false, zVal, -u, -v, x, y, ax, ay);
			u = (i+1.)/(num+1.);	v = 1;
			flow(false, zVal, u, v, x, y, ax, ay);
			flow(false, zVal, -u, -v, x, y, ax, ay);
			if(cnt)
			{
				u = 0.5;	v = (i+1.)/(num+1.);
				flow(false, zVal, u, v, x, y, ax, ay);
				flow(false, zVal, -u, -v, x, y, ax, ay);
				u = (i+1.)/(num+1.);	v = 0.5;
				flow(false, zVal, u, v, x, y, ax, ay);
				flow(false, zVal, -u, -v, x, y, ax, ay);
			}
		}
	}
	Flush();
}
//-----------------------------------------------------------------------------
void mglGraph::Flow(const mglData &ax, const mglData &ay, const char *sch, int num,
					bool cnt, float zVal)
{
	if(ax.nx*ax.ny!=ay.nx*ay.ny){	SetWarn(mglWarnDim,"Flow");	return;	}
	if(ax.nx<2 || ax.ny<2)		{	SetWarn(mglWarnLow,"Flow");	return;	}
	mglData x(ax.nx), y(ax.ny);
	x.Fill(Min.x,Max.x);
	y.Fill(Min.y,Max.y);
	Flow(x,y,ax,ay,sch,num,cnt,zVal);
}
//-----------------------------------------------------------------------------
void mglGraph::flow(bool simple, float u, float v, float w,
					const mglData &x, const mglData &y, const mglData &z,
					const mglData &ax, const mglData &ay, const mglData &az)
{
	static long n=10*(ax.nx+ax.ny);
	long nn = ax.nx*ax.ny*ax.nz;
	bool both = x.nx*x.ny*x.nz==nn && y.nx*y.ny*y.nz==nn && z.nx*z.ny*z.nz==nn;
	float *pp, *cc;
	bool *tt;
	pp = (float *)malloc(3*n*sizeof(float));
	cc = (float *)malloc(n*sizeof(float));
	tt = (bool  *)malloc(n*sizeof(bool));
	nn = (ax.nx > ax.ny ? ax.nx : ax.ny);
	nn = (nn > ax.nz ? nn : ax.nz);
	float dt = 0.2/nn, e,f,g,ee[4],ff[4],gg[4],h,s=1,u1,v1,w1;
	if(u<0 || v<0 || w<0)
	{	dt = -dt;	u = -u;		v = -v;		w = -w;		s = -1;}
	register long k=0,m;
	bool end = false;
	do{
		if(k>=n)	// resize array if need
		{
			n += 10*(ax.nx+ax.ny);
			pp = (float *)realloc(pp,3*n*sizeof(float));
			cc = (float *)realloc(cc,n*sizeof(float));
			tt = (bool  *)realloc(tt,n*sizeof(bool));
		}
		// insert point
		pp[3*k] = simple ? Min.x + (Max.x-Min.x)*u :
				(both ? x.Spline1(u,v,w):x.Spline1(u,0,0));
		pp[3*k+1] = simple ? Min.y + (Max.y-Min.y)*v :
				(both ? y.Spline1(u,v,w):y.Spline1(v,0,0));
		pp[3*k+2] = simple ? Min.z + (Max.z-Min.z)*w :
				(both ? z.Spline1(u,v,w):z.Spline1(w,0,0));
		tt[k]  = ScalePoint(pp[3*k],pp[3*k+1],pp[3*k+2]);
		for(m=0;m<k-1;m++)	// determines encircle
		{
			f = pp[3*k]-pp[3*m];
			g = pp[3*k+1]-pp[3*m+1];
			e = pp[3*k+2]-pp[3*m+2];
			if(f*f+g*g+e*e<dt*dt/4.)	{	end = true;	break;	}
		}
		e = ax.Linear1(u,v,w);	f = ay.Linear1(u,v,w);	g = az.Linear1(u,v,w);
//		e = ax.Spline1(u,v,w);	f = ay.Spline1(u,v,w);	g = az.Spline1(u,v,w);
		h = sqrt(e*e+f*f+g*g);	cc[k] = s*h;	h+=1;
		k++;
		// find next point by midpoint method
		if(h<1e-7)	break;	// stationary point
		ee[0]=e*dt/h;	ff[0]=f*dt/h;	gg[0]=g*dt/h;
		u1 = u+ee[0]/2;	v1 = v+ff[0]/2;	w1 = w+gg[0]/2;
		e = ax.Linear1(u1,v1,w1);	f = ay.Linear1(u1,v1,w1);
		g = az.Linear1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
//		e = ax.Spline1(u1,v1,w1);	f = ay.Spline1(u1,v1,w1);
//		g = az.Spline1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
		if(h<1e-7)	break;	// stationary point
		ee[1]=e*dt/h;	ff[1]=f*dt/h;	gg[1]=g*dt/h;
		u1 = u+ee[1]/2;	v1 = v+ff[1]/2;	w1 = w+gg[1]/2;
		e = ax.Linear1(u1,v1,w1);	f = ay.Linear1(u1,v1,w1);
		g = az.Linear1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
//		e = ax.Spline1(u1,v1,w1);	f = ay.Spline1(u1,v1,w1);
//		g = az.Spline1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
		if(h<1e-7)	break;	// stationary point
		ee[2]=e*dt/h;	ff[2]=f*dt/h;	gg[2]=g*dt/h;
		u1 = u+ee[2];	v1 = v+ff[2];	w1 = w+gg[2];
		e = ax.Linear1(u1,v1,w1);	f = ay.Linear1(u1,v1,w1);
		g = az.Linear1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
//		e = ax.Spline1(u1,v1,w1);	f = ay.Spline1(u1,v1,w1);
//		g = az.Spline1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
		if(h<1e-7)	break;	// stationary point
		ee[3]=e*dt/h;	ff[3]=f*dt/h;	gg[3]=g*dt/h;
		u += ee[0]/6+ee[1]/3+ee[2]/3+ee[3]/6;
		v += ff[0]/6+ff[1]/3+ff[2]/3+ff[3]/6;
		w += gg[0]/6+gg[1]/3+gg[2]/3+gg[3]/6;
		// condition of end
		end = end || u<0 || v<0 || u>1 || v>1 || w<0 || w>1;
	} while(!end);
	curv_plot(k,pp,cc,tt);
	free(pp);	free(cc);	free(tt);
}
//-----------------------------------------------------------------------------
void mglGraph::Flow(const mglData &x, const mglData &y, const mglData &z, const mglData &ax, const mglData &ay,
					const mglData &az, const char *sch, int num, bool cnt)
{
	float u,v,w;
	long i,j,n=ax.nx,m=ax.ny,l=ax.nz;
	if(ax.nx*ax.ny*ax.nz!=ay.nx*ay.ny*ay.nz || ax.nx*ax.ny*ax.nz!=az.nx*az.ny*az.nz)
	{	SetWarn(mglWarnDim,"Flow");	return;	}
	if(ax.nx<2 || ax.ny<2 || ax.nz<2)
	{	SetWarn(mglWarnLow,"Flow");	return;	}
	bool both = x.nx*x.ny*x.nz==n*m*l && y.nx*y.ny*y.nz==n*m*l && z.nx*z.ny*z.nz==n*m*l;
	if(!(both || (x.nx==n && y.nx==m && z.nx==l)))
	{	SetWarn(mglWarnDim,"Flow");	return;	}

	Arrow1 = Arrow2 = '_';
	SetScheme(sch);

	for(i=0;i<num;i++)	for(j=0;j<num;j++)
	{
		u = (i+1.)/(num+1.);	v = (j+1.)/(num+1.);	w = 0;
		flow(true, u, v, w, x, y, z, ax, ay, az);
		flow(true,-u,-v,-w, x, y, z, ax, ay, az);
		u = (i+1.)/(num+1.);	v = (j+1.)/(num+1.);	w = 1;
		flow(true, u, v, w, x, y, z, ax, ay, az);
		flow(true,-u,-v,-w, x, y, z, ax, ay, az);
		u = 0;	v = (j+1.)/(num+1.);	w = (i+1.)/(num+1.);
		flow(true, u, v, w, x, y, z, ax, ay, az);
		flow(true,-u,-v,-w, x, y, z, ax, ay, az);
		u = 1;	v = (j+1.)/(num+1.);	w = (i+1.)/(num+1.);
		flow(true, u, v, w, x, y, z, ax, ay, az);
		flow(true,-u,-v,-w, x, y, z, ax, ay, az);
		u = (i+1.)/(num+1.);	v = 0;	w = (j+1.)/(num+1.);
		flow(true, u, v, w, x, y, z, ax, ay, az);
		flow(true,-u,-v,-w, x, y, z, ax, ay, az);
		u = (i+1.)/(num+1.);	v = 1;	w = (j+1.)/(num+1.);
		flow(true, u, v, w, x, y, z, ax, ay, az);
		flow(true,-u,-v,-w, x, y, z, ax, ay, az);
		if(cnt)
		{
			u = (i+1.)/(num+1.);	v = (j+1.)/(num+1.);	w = 0.5;
			flow(true, u, v, w, x, y, z, ax, ay, az);
			flow(true,-u,-v,-w, x, y, z, ax, ay, az);
			u = 0.5;	v = (j+1.)/(num+1.);	w = (i+1.)/(num+1.);
			flow(true, u, v, w, x, y, z, ax, ay, az);
			flow(true,-u,-v,-w, x, y, z, ax, ay, az);
			u = (i+1.)/(num+1.);	v = 0.5;	w = (j+1.)/(num+1.);
			flow(true, u, v, w, x, y, z, ax, ay, az);
			flow(true,-u,-v,-w, x, y, z, ax, ay, az);
		}
	}
	Flush();
}
//-----------------------------------------------------------------------------
void mglGraph::Flow(const mglData &ax, const mglData &ay, const mglData &az,
					const char *sch, int num, bool cnt)
{
	if(ax.nx*ax.ny*ax.nz!=ay.nx*ay.ny*ay.nz || ax.nx*ax.ny*ax.nz!=az.nx*az.ny*az.nz)
	{	SetWarn(mglWarnDim,"Flow");	return;	}
	if(ax.nx<2 || ax.ny<2 || ax.nz<2)
	{	SetWarn(mglWarnLow,"Flow");	return;	}
	mglData x(ax.nx), y(ax.ny), z(ax.nz);
	x.Fill(Min.x,Max.x);
	y.Fill(Min.y,Max.y);
	z.Fill(Min.z,Max.z);
	Flow(x,y,z,ax,ay,az,sch,num,cnt);
}
//-----------------------------------------------------------------------------
//
//	Pipe series
//
//-----------------------------------------------------------------------------
void mglGraph::flowr(bool simple, float zVal, float u, float v, const mglData &x, const mglData &y, const mglData &ax, const mglData &ay, float r0)
{
	long n=10*(ax.nx+ax.ny);
	bool both = x.nx==ax.nx && y.nx==ax.nx && x.ny==ax.ny && y.ny==ax.ny;
	float *pp, *cc, *rr;

	pp = (float *)malloc(3*n*sizeof(float));
	cc = (float *)malloc(4*n*sizeof(float));
	rr = (float *)malloc(n*sizeof(float));
	float dt = 0.5/(ax.nx > ax.ny ? ax.nx : ax.ny),e,f,g,ff[4],gg[4],h,s=1,ss;
	if(u<0 || v<0)	{	dt = -dt;	u = -u;		v = -v;		s = -1;}
	ss = 4/((Cmax-Cmin)*(Cmax-Cmin));
	if(r0==0)	r0=0.05;
	register long k=0,m;
	bool end = false;
	do{
		if(k>=n)	// resize array if need
		{
			n += 10*(ax.nx+ax.ny);
			pp = (float *)realloc(pp,3*n*sizeof(float));
			cc = (float *)realloc(cc,4*n*sizeof(float));
			rr = (float *)realloc(rr,n*sizeof(float));
		}
		// insert point
		pp[3*k] = simple ? Min.x + (Max.x-Min.x)*u : (both ? x.Spline1(u,v,0):x.Spline1(u,0,0));
		pp[3*k+1] = simple ? Min.y + (Max.y-Min.y)*v : (both ? y.Spline1(u,v,0):y.Spline1(v,0,0));
		pp[3*k+2] = zVal;
		for(m=0;m<k-1;m++)	// determines encircle
		{
			f = pp[3*k]-pp[3*m];
			g = pp[3*k+1]-pp[3*m+1];
			e = pp[3*k+2]-pp[3*m+2];
			if(f*f+g*g+e*e<dt*dt/4.)	{	end = true;	}
		}
		f = ax.Linear1(u,v,0);	g = ay.Linear1(u,v,0);
//		f = ax.Spline1(u,v,0);	g = ay.Spline1(u,v,0);
		h = hypot(f,g);
		rr[k] = r0>0 ? r0*sqrt(1e-2+ss*h*h)/2 : -r0/sqrt(1e-2+ss*h*h)/5;
		{
			float a = (2*s*h-Cmin-Cmax)/(Cmax-Cmin);
			if(a>1)	a=1;	if(a<-1)	a=-1;
			mglColor c = GetC(a,false);
			cc[4*k] = c.r;	cc[4*k+1] = c.g;	cc[4*k+2] = c.b;	cc[4*k+3] = AlphaDef;
		}
		h+=1;	k++;
		// find next point by midpoint method
		ff[0]=f*dt/h;	gg[0]=g*dt/h;
		e = u+ff[0]/2;	h = v+gg[0]/2;
		f = ax.Linear1(e,h,0);	g = ay.Linear1(e,h,0);	h = 1+hypot(f,g);
//		f = ax.Spline1(e,h,0);	g = ay.Spline1(e,h,0);	h = 1+hypot(f,g);
		ff[1]=f*dt/h;	gg[1]=g*dt/h;
		e = u+ff[1]/2;	h = v+gg[1]/2;
		f = ax.Linear1(e,h,0);	g = ay.Linear1(e,h,0);	h = 1+hypot(f,g);
//		f = ax.Spline1(e,h,0);	g = ay.Spline1(e,h,0);	h = 1+hypot(f,g);
		ff[2]=f*dt/h;	gg[2]=g*dt/h;
		e = u+ff[2];	h = v+gg[2];
		f = ax.Linear1(e,h,0);	g = ay.Linear1(e,h,0);	h = 1+hypot(f,g);
//		f = ax.Spline1(e,h,0);	g = ay.Spline1(e,h,0);	h = 1+hypot(f,g);
		ff[3]=f*dt/h;	gg[3]=g*dt/h;
		u += ff[0]/6+ff[1]/3+ff[2]/3+ff[3]/6;
		v += gg[0]/6+gg[1]/3+gg[2]/3+gg[3]/6;
		// condition of end
		end = end || u<0 || v<0 || u>1 || v>1;
	} while(!end);
	tube_plot(k,pp,cc,rr);
	free(pp);	free(cc);	free(rr);
}
//-----------------------------------------------------------------------------
void mglGraph::Pipe(const mglData &x, const mglData &y, const mglData &ax, const mglData &ay, const char *sch, float r0, int num, bool cnt, float zVal)
{
	float u,v;
	long n=ax.nx, m=ax.ny;
	if(n*m*ax.nz!=ay.nx*ay.ny*ay.nz)	{	SetWarn(mglWarnDim,"Pipe");	return;	}
	if(n<2 || m<2)						{	SetWarn(mglWarnLow,"Pipe");	return;	}
	bool both = x.nx==n && y.nx==n && x.ny==m && y.ny==m;
	if(!(both || (x.nx==n && y.nx==m)))	{	SetWarn(mglWarnDim,"Pipe");	return;	}

	Arrow1 = Arrow2 = '_';
	SetScheme(sch);
	// allocate memory
	if(isnan(zVal)) zVal = Min.z;

	for(long k=0;k<ax.nz;k++)
	{
		if(ax.nz>1)	zVal = Min.z+(Max.z-Min.z)*float(k)/(ax.nz-1);
		for(long i=0;i<num;i++)
		{
			u = 0;	v = (i+1.)/(num+1.);
			flowr(false, zVal, u, v, x, y, ax, ay,r0);
			flowr(false, zVal, -u, -v, x, y, ax, ay,r0);
			u = 1;	v = (i+1.)/(num+1.);
			flowr(false, zVal, u, v, x, y, ax, ay,r0);
			flowr(false, zVal, -u, -v, x, y, ax, ay,r0);
			u = (i+1.)/(num+1.);	v = 0;
			flowr(false, zVal, u, v, x, y, ax, ay,r0);
			flowr(false, zVal, -u, -v, x, y, ax, ay,r0);
			u = (i+1.)/(num+1.);	v = 1;
			flowr(false, zVal, u, v, x, y, ax, ay,r0);
			flowr(false, zVal, -u, -v, x, y, ax, ay,r0);
			if(cnt)
			{
				u = 0.5;	v = (i+1.)/(num+1.);
				flowr(false, zVal, u, v, x, y, ax, ay,r0);
				flowr(false, zVal, -u, -v, x, y, ax, ay,r0);
				u = (i+1.)/(num+1.);	v = 0.5;
				flowr(false, zVal, u, v, x, y, ax, ay,r0);
				flowr(false, zVal, -u, -v, x, y, ax, ay,r0);
			}
		}
	}
	Flush();
}
//-----------------------------------------------------------------------------
void mglGraph::Pipe(const mglData &ax, const mglData &ay, const char *sch, float r0, int num, bool cnt, float zVal)
{
	if(ax.nx*ax.ny!=ay.nx*ay.ny){	SetWarn(mglWarnDim,"Pipe");	return;	}
	if(ax.nx<2 || ax.ny<2)		{	SetWarn(mglWarnLow,"Pipe");	return;	}
	mglData x(ax.nx), y(ax.ny);
	x.Fill(Min.x,Max.x);
	y.Fill(Min.y,Max.y);
	Pipe(x,y,ax,ay,sch,r0,num,cnt,zVal);
}
//-----------------------------------------------------------------------------
void mglGraph::flowr(bool simple, float u, float v, float w,
					const mglData &x, const mglData &y, const mglData &z,
					const mglData &ax, const mglData &ay, const mglData &az, float r0)
{
	static long n=10*(ax.nx+ax.ny);
	long nn = ax.nx*ax.ny*ax.nz;
	bool both = x.nx*x.ny*x.nz==nn && y.nx*y.ny*y.nz==nn && z.nx*z.ny*z.nz==nn;
	float *pp, *cc, *rr;

	pp = (float *)malloc(3*n*sizeof(float));
	cc = (float *)malloc(4*n*sizeof(float));
	rr = (float *)malloc(n*sizeof(float));
	nn = (ax.nx > ax.ny ? ax.nx : ax.ny);
	nn = (nn > ax.nz ? nn : ax.nz);
	float dt = 0.2/nn, e,f,g,ee[4],ff[4],gg[4],h,s=1,u1,v1,w1,ss;
	if(u<0 || v<0 || w<0)
	{	dt = -dt;	u = -u;		v = -v;		w = -w;		s = -1;}
	ss = 4/((Cmax-Cmin)*(Cmax-Cmin));
	if(r0==0)	r0=0.05;
	register long k=0,m;
	bool end = false;
	do{
		if(k>=n)	// resize array if need
		{
			n += 10*(ax.nx+ax.ny);
			pp = (float *)realloc(pp,3*n*sizeof(float));
			cc = (float *)realloc(cc,4*n*sizeof(float));
			rr = (float *)realloc(rr,n*sizeof(bool));
		}
		// insert point
		pp[3*k] = simple ? Min.x + (Max.x-Min.x)*u : (both ? x.Spline1(u,v,w):x.Spline1(u,0,0));
		pp[3*k+1] = simple ? Min.y + (Max.y-Min.y)*v : (both ? y.Spline1(u,v,w):y.Spline1(v,0,0));
		pp[3*k+2] = simple ? Min.z + (Max.z-Min.z)*w : (both ? z.Spline1(u,v,w):z.Spline1(w,0,0));
		for(m=0;m<k-1;m++)	// determines encircle
		{
			f = pp[3*k]-pp[3*m];
			g = pp[3*k+1]-pp[3*m+1];
			e = pp[3*k+2]-pp[3*m+2];
			if(f*f+g*g+e*e<dt*dt/4.)	{	end = true;	break;	}
		}
		e = ax.Linear1(u,v,w);	f = ay.Linear1(u,v,w);	g = az.Linear1(u,v,w);
//		e = ax.Spline1(u,v,w);	f = ay.Spline1(u,v,w);	g = az.Spline1(u,v,w);
		h = sqrt(e*e+f*f+g*g);
		rr[k] = r0>0 ? r0*sqrt(1e-2+ss*h*h)/2 : -r0/sqrt(1e-2+ss*h*h)/5;
		{
			float a = (2*s*h-Cmin-Cmax)/(Cmax-Cmin);
			if(a>1)	a=1;	if(a<-1)	a=-1;
			mglColor c = GetC(a,false);
			cc[4*k] = c.r;	cc[4*k+1] = c.g;	cc[4*k+2] = c.b;	cc[4*k+3] = AlphaDef;
		}
		h+=1;	k++;
		// find next point by midpoint method
		ee[0]=e*dt/h;	ff[0]=f*dt/h;	gg[0]=g*dt/h;
		u1 = u+ee[0]/2;	v1 = v+ff[0]/2;	w1 = w+gg[0]/2;
		e = ax.Linear1(u1,v1,w1);	f = ay.Linear1(u1,v1,w1);
		g = az.Linear1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
//		e = ax.Spline1(u1,v1,w1);	f = ay.Spline1(u1,v1,w1);
//		g = az.Spline1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
		ee[1]=e*dt/h;	ff[1]=f*dt/h;	gg[1]=g*dt/h;
		u1 = u+ee[1]/2;	v1 = v+ff[1]/2;	w1 = w+gg[1]/2;
		e = ax.Linear1(u1,v1,w1);	f = ay.Linear1(u1,v1,w1);
		g = az.Linear1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
//		e = ax.Spline1(u1,v1,w1);	f = ay.Spline1(u1,v1,w1);
//		g = az.Spline1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
		ee[2]=e*dt/h;	ff[2]=f*dt/h;	gg[2]=g*dt/h;
		u1 = u+ee[2];	v1 = v+ff[2];	w1 = w+gg[2];
		e = ax.Linear1(u1,v1,w1);	f = ay.Linear1(u1,v1,w1);
		g = az.Linear1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
//		e = ax.Spline1(u1,v1,w1);	f = ay.Spline1(u1,v1,w1);
//		g = az.Spline1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
		ee[3]=e*dt/h;	ff[3]=f*dt/h;	gg[3]=g*dt/h;
		u += ee[0]/6+ee[1]/3+ee[2]/3+ee[3]/6;
		v += ff[0]/6+ff[1]/3+ff[2]/3+ff[3]/6;
		w += gg[0]/6+gg[1]/3+gg[2]/3+gg[3]/6;
		// condition of end
		end = end || u<0 || v<0 || u>1 || v>1 || w<0 || w>1;
	} while(!end);
	tube_plot(k,pp,cc,rr);
	free(pp);	free(cc);	free(rr);
}
//-----------------------------------------------------------------------------
void mglGraph::Pipe(const mglData &x, const mglData &y, const mglData &z, const mglData &ax, const mglData &ay, const mglData &az, const char *sch, float r0, int num, bool cnt)
{
	float u,v,w;
	long i,j,n=ax.nx,m=ax.ny,l=ax.nz;
	if(ax.nx*ax.ny*ax.nz!=ay.nx*ay.ny*ay.nz || ax.nx*ax.ny*ax.nz!=az.nx*az.ny*az.nz)
	{	SetWarn(mglWarnDim,"Pipe");	return;	}
	if(ax.nx<2 || ax.ny<2 || ax.nz<2)
	{	SetWarn(mglWarnLow,"Pipe");	return;	}
	bool both = x.nx*x.ny*x.nz==n*m*l && y.nx*y.ny*y.nz==n*m*l && z.nx*z.ny*z.nz==n*m*l;
	if(!(both || (x.nx==n && y.nx==m && z.nx==l)))
	{	SetWarn(mglWarnDim,"Pipe");	return;	}

	Arrow1 = Arrow2 = '_';
	SetScheme(sch);

	for(i=0;i<num;i++)	for(j=0;j<num;j++)
	{
		u = (i+1.)/(num+1.);	v = (j+1.)/(num+1.);	w = 0;
		flowr(true, u, v, w, x, y, z, ax, ay, az,r0);
		flowr(true,-u,-v,-w, x, y, z, ax, ay, az,r0);
		u = (i+1.)/(num+1.);	v = (j+1.)/(num+1.);	w = 1;
		flowr(true, u, v, w, x, y, z, ax, ay, az,r0);
		flowr(true,-u,-v,-w, x, y, z, ax, ay, az,r0);
		u = 0;	v = (j+1.)/(num+1.);	w = (i+1.)/(num+1.);
		flowr(true, u, v, w, x, y, z, ax, ay, az,r0);
		flowr(true,-u,-v,-w, x, y, z, ax, ay, az,r0);
		u = 1;	v = (j+1.)/(num+1.);	w = (i+1.)/(num+1.);
		flowr(true, u, v, w, x, y, z, ax, ay, az,r0);
		flowr(true,-u,-v,-w, x, y, z, ax, ay, az,r0);
		u = (i+1.)/(num+1.);	v = 0;	w = (j+1.)/(num+1.);
		flowr(true, u, v, w, x, y, z, ax, ay, az,r0);
		flowr(true,-u,-v,-w, x, y, z, ax, ay, az,r0);
		u = (i+1.)/(num+1.);	v = 1;	w = (j+1.)/(num+1.);
		flowr(true, u, v, w, x, y, z, ax, ay, az,r0);
		flowr(true,-u,-v,-w, x, y, z, ax, ay, az,r0);
		if(cnt)
		{
			u = (i+1.)/(num+1.);	v = (j+1.)/(num+1.);	w = 0.5;
			flowr(true, u, v, w, x, y, z, ax, ay, az,r0);
			flowr(true,-u,-v,-w, x, y, z, ax, ay, az,r0);
			u = 0.5;	v = (j+1.)/(num+1.);	w = (i+1.)/(num+1.);
			flowr(true, u, v, w, x, y, z, ax, ay, az,r0);
			flowr(true,-u,-v,-w, x, y, z, ax, ay, az,r0);
			u = (i+1.)/(num+1.);	v = 0.5;	w = (j+1.)/(num+1.);
			flowr(true, u, v, w, x, y, z, ax, ay, az,r0);
			flowr(true,-u,-v,-w, x, y, z, ax, ay, az,r0);
		}
	}
	Flush();
}
//-----------------------------------------------------------------------------
void mglGraph::Pipe(const mglData &ax, const mglData &ay, const mglData &az,
					const char *sch, float r0, int num, bool cnt)
{
	if(ax.nx*ax.ny*ax.nz!=ay.nx*ay.ny*ay.nz || ax.nx*ax.ny*ax.nz!=az.nx*az.ny*az.nz)
	{	SetWarn(mglWarnDim,"Pipe");	return;	}
	if(ax.nx<2 || ax.ny<2 || ax.nz<2)
	{	SetWarn(mglWarnLow,"Pipe");	return;	}
	mglData x(ax.nx), y(ax.ny), z(ax.nz);
	x.Fill(Min.x,Max.x);
	y.Fill(Min.y,Max.y);
	z.Fill(Min.z,Max.z);
	Pipe(x,y,z,ax,ay,az,sch,r0,num,cnt);
}
//-----------------------------------------------------------------------------
/// Plot flows for vector field {ax,ay} parametrically depended on coordinate {x,y} with color proportional to value |a|
void mgl_flow_xy(HMGL gr, const HMDT x, const HMDT y, const HMDT ax, const HMDT ay, const char *sch, int num, int central, float zVal)
{	if(gr && ay && ax && x && y)	gr->Flow(*x, *y, *ax, *ay, sch, num, central, zVal);	}
/// Plot flows for vector field {ax,ay} with color proportional to value |a|
void mgl_flow_2d(HMGL gr, const HMDT ax, const HMDT ay, const char *sch, int num, int central, float zVal)
{	if(gr && ay && ax)	gr->Flow(*ax, *ay, sch, num, central, zVal);	}
/// Plot flows for 3d vector field {ax,ay,ay} parametrically depended on coordinate {x,y,z} with color proportional to value |a|
void mgl_flow_xyz(HMGL gr, const HMDT x, const HMDT y, const HMDT z, const HMDT ax, const HMDT ay, const HMDT az, const char *sch, int num, int central)
{	if(gr && ay && ax && az && x && y && z)	gr->Flow(*x, *y, *z, *ax, *ay, *az, sch, num, central);	}
/// Plot flows for 3d vector field {ax,ay,ay} with color proportional to value |a|
void mgl_flow_3d(HMGL gr, const HMDT ax, const HMDT ay, const HMDT az, const char *sch, int num, int central)
{	if(gr && ay && ax && az)	gr->Flow(*ax, *ay, *az, sch, num, central);	}
//-----------------------------------------------------------------------------
/// Plot flow pipes for vector field {ax,ay} parametrically depended on coordinate {x,y} with color proportional to value |a|
void mgl_pipe_xy(HMGL gr, const HMDT x, const HMDT y, const HMDT ax, const HMDT ay, const char *sch, float r0, int num, int central, float zVal)
{	if(gr && ay && ax && x && y)	gr->Pipe(*x, *y, *ax, *ay, sch, r0, num, central, zVal);	}
/// Plot flow pipes for vector field {ax,ay} with color proportional to value |a|
void mgl_pipe_2d(HMGL gr, const HMDT ax, const HMDT ay, const char *sch, float r0, int num, int central, float zVal)
{	if(gr && ay && ax)	gr->Pipe(*ax, *ay, sch, r0, num, central, zVal);	}
/// Plot flow pipes for 3d vector field {ax,ay,ay} parametrically depended on coordinate {x,y,z} with color proportional to value |a|
void mgl_pipe_xyz(HMGL gr, const HMDT x, const HMDT y, const HMDT z, const HMDT ax, const HMDT ay, const HMDT az, const char *sch, float r0, int num, int central)
{	if(gr && ay && ax && az && x && y && z)	gr->Pipe(*x, *y, *z, *ax, *ay, *az, sch, r0, num, central);	}
/// Plot flow pipes for 3d vector field {ax,ay,ay} with color proportional to value |a|
void mgl_pipe_3d(HMGL gr, const HMDT ax, const HMDT ay, const HMDT az, const char *sch, float r0, int num, int central)
{	if(gr && ay && ax && az)	gr->Pipe(*ax, *ay, *az, sch, r0, num, central);	}
//-----------------------------------------------------------------------------
//	Fortran interface
//-----------------------------------------------------------------------------
/// Plot flows for vector field {ax,ay} parametrically depended on coordinate {x,y} with color proportional to value |a|
void mgl_flow_xy_(long *gr, long *x, long *y, long *ax, long *ay, const char *sch,
					int *num, int *central, float *zVal,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && ay && ax && x && y)	_GR_->Flow(_D_(x), _D_(y), _D_(ax), _D_(ay), s, *num, *central, *zVal);
	delete []s;
}
/// Plot flows for vector field {ax,ay} with color proportional to value |a|
void mgl_flow_2d_(long *gr, long *ax, long *ay, const char *sch, int *num, int *central, float *zVal,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && ay && ax)	_GR_->Flow(_D_(ax), _D_(ay), s, *num, *central, *zVal);
	delete []s;
}
/// Plot flows for 3d vector field {ax,ay,ay} parametrically depended on coordinate {x,y,z} with color proportional to value |a|
void mgl_flow_xyz_(long *gr, long *x, long *y, long *z, long *ax, long *ay, long *az,
					const char *sch, int *num, int *central,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && ay && ax && az && x && y && z)
		_GR_->Flow(_D_(x), _D_(y), _D_(z), _D_(ax), _D_(ay), _D_(az), s, *num, *central);
	delete []s;
}
/// Plot flows for 3d vector field {ax,ay,ay} with color proportional to value |a|
void mgl_flow_3d_(long *gr, long *ax, long *ay, long *az, const char *sch, int *num, int *central,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && ay && ax && az)	_GR_->Flow(_D_(ax), _D_(ay), _D_(az), s, *num, *central);
	delete []s;
}
//-----------------------------------------------------------------------------
/// Plot flow pipes for vector field {ax,ay} parametrically depended on coordinate {x,y} with color proportional to value |a|
void mgl_pipe_xy_(long *gr, long *x, long *y, long *ax, long *ay, const char *sch, float *r0,
					int *num, int *central, float *zVal,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && ay && ax && x && y)
		_GR_->Pipe(_D_(x), _D_(y), _D_(ax), _D_(ay), s, *r0, *num, *central, *zVal);
	delete []s;
}
/// Plot flow pipes for vector field {ax,ay} with color proportional to value |a|
void mgl_pipe_2d_(long *gr, long *ax, long *ay, const char *sch, float *r0, int *num, int *central, float *zVal,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && ay && ax)	_GR_->Pipe(_D_(ax), _D_(ay), s, *r0, *num, *central, *zVal);
	delete []s;
}
/// Plot flow pipes for 3d vector field {ax,ay,ay} parametrically depended on coordinate {x,y,z} with color proportional to value |a|
void mgl_pipe_xyz_(long *gr, long *x, long *y, long *z, long *ax, long *ay, long *az,
					const char *sch, float *r0, int *num, int *central,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && ay && ax && az && x && y && z)
		_GR_->Pipe(_D_(x), _D_(y), _D_(z), _D_(ax), _D_(ay), _D_(az), s, *r0, *num, *central);
	delete []s;
}
/// Plot flow pipes for 3d vector field {ax,ay,ay} with color proportional to value |a|
void mgl_pipe_3d_(long *gr, long *ax, long *ay, long *az, const char *sch, float *r0, int *num, int *central,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && ay && ax && az)	_GR_->Pipe(_D_(ax), _D_(ay), _D_(az), s, *r0, *num, *central);
	delete []s;
}
//-----------------------------------------------------------------------------
