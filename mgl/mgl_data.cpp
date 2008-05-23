/* mgl_data.cpp is part of Math Graphic Library
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
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mgl/mgl_eval.h"
#include "mgl/mgl_data.h"

#ifndef NO_GSL
#include <gsl/gsl_fft_complex.h>
#include <gsl/gsl_fft_real.h>
#endif

void mglFillP(int x,int y,float *a,int nx,int ny,float _p[4][4]);
void mglFillP(int x,float *a,int nx,float _p[4]);
//-----------------------------------------------------------------------------
double ipow_mgl(double x,int n)
{
	double t;
	if(n==2)	return x*x;
	if(n==1)	return x;
	if(n<0)		return 1/ipow(x,-n);
	if(n==0)	return 1;
	t = ipow(x,n/2);	t = t*t;
	if(n%2==1)	t *= x;
	return t;
}
//-----------------------------------------------------------------------------
void mglData::Smooth(int Type,const char *dirs,float delta)
{
	if(Type == SMOOTH_NONE)	return;
	long i,j,k,i0,nn=nx*ny,d3,d5;
	float y5,y3,x2y;
	float *b = new float[nx*ny*nz];
	// сглаживание по x
	memset(b,0,nx*ny*nz*sizeof(float));
	if(nx>4 && strchr(dirs,'x'))
	{
		for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
		{
			i0 = i+nx*(j+ny*k);
			d3 = d5 = 0;
			if(i==0)	{	d3 = 1;		d5 = 2;	}
			if(i==1)	{	d5 = 1;	}
			if(i==nx-1)	{	d3 = -1;	d5 = -2;}
			if(i==nx-2)	{	d5 = -1;}
			y3 = (a[i0+d3-1] + a[i0+d3] + a[i0+d3+1]);
//			xy3= (a[i0+d3+1] - a[i0+d3-1]);
			y5 = (a[i0+d5-2] + a[i0+d5-1] + a[i0+d5] +
						a[i0+d5+1] + a[i0+d5+2]);
//			xy = (a[i0+d5+1] + 2*a[i0+d5+2] - 2*a[i0+d5-2] - a[i0+d5-1]);
			x2y= (a[i0+d5+1] + 4*a[i0+d5+2] + 4*a[i0+d5-2] + a[i0+d5-1]);

			if(d3)	b[i0] = a[i0];
			else if(Type==SMOOTH_LINE_3 || d5)	b[i0] = y3/3.;
			else if(Type==SMOOTH_LINE_5)		b[i0] = y5/5.;
			else if(Type==SMOOTH_QUAD_5)				b[i0] = (17*y5-5*x2y)/35.;
			if(delta>0)		b[i0] = mgl_max(a[i0]-delta,mgl_min(a[i0]+delta,b[i0]));
		}
		memcpy(a,b,nx*ny*nz*sizeof(float));	memset(b,0,nx*ny*nz*sizeof(float));
	}
	if(ny>4 && strchr(dirs,'y'))
	{
		for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
		{
			i0 = i+nx*(j+ny*k);
			d3 = d5 = 0;
			if(j==0)	{	d3 = 1;		d5 = 2;	}
			if(j==1)	{	d5 = 1;	}
			if(j==ny-1)	{	d3 = -1;	d5 = -2;}
			if(j==ny-2)	{	d5 = -1;}
			y3 = (a[i0+nx*(d3-1)] + a[i0+nx*d3] + a[i0+nx*(d3+1)]);
//			xy3= (a[i0+nx*(d3+1)] - a[i0+nx*(d3-1)]);
			y5 = (a[i0+nx*(d5-2)] + a[i0+nx*(d5-1)] + a[i0+nx*d5] +
						a[i0+nx*(d5+1)] + a[i0+nx*(d5+2)]);
//			xy = (a[i0+nx*(d5+1)] + 2*a[i0+nx*(d5+2)] -
//						2*a[i0+nx*(d5-2)] - a[i0+nx*(d5-1)]);
			x2y= (a[i0+nx*(d5+1)] + 4*a[i0+nx*(d5+2)] +
						4*a[i0+nx*(d5-2)] + a[i0+nx*(d5-1)]);
			if(d3)	b[i0] = a[i0];
			else if(Type==SMOOTH_LINE_3 || d5)	b[i0] = y3/3.;
			else if(Type==SMOOTH_LINE_5)		b[i0] = y5/5.;
			else if(Type==SMOOTH_QUAD_5)				b[i0] = (17*y5-5*x2y)/35.;
			if(delta>0)		b[i0] = mgl_max(a[i0]-delta,mgl_min(a[i0]+delta,b[i0]));
		}
		memcpy(a,b,nx*ny*nz*sizeof(float));	memset(b,0,nx*ny*nz*sizeof(float));
	}
	if(nz>4 && strchr(dirs,'z'))
	{
		for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
		{
			i0 = i+nx*(j+ny*k);
			d3 = d5 = 0;
			if(k==0)	{	d3 = 1;		d5 = 2;	}
			if(k==1)	{	d5 = 1;	}
			if(k==nz-1)	{	d3 = -1;	d5 = -2;}
			if(k==nz-2)	{	d5 = -1;}
			y3 = (a[i0+nn*(d3-1)] + a[i0+nn*d3] + a[i0+nn*(d3+1)]);
//			xy3= (a[i0+nn*(d3+1)] - a[i0+nn*(d3-1)]);
			y5 = (a[i0+nn*(d5-2)] + a[i0+nn*(d5-1)] + a[i0+nn*d5] +
						a[i0+nn*(d5+1)] + a[i0+nn*(d5+2)]);
//			xy = (a[i0+nn*(d5+1)] + 2*a[i0+nn*(d5+2)] -
//						2*a[i0+nn*(d5-2)] - a[i0+nn*(d5-1)]);
			x2y= (a[i0+nn*(d5+1)] + 4*a[i0+nn*(d5+2)] +
						4*a[i0+nn*(d5-2)] + a[i0+nn*(d5-1)]);
			if(d3)	b[i0] = a[i0];
			else if(Type==SMOOTH_LINE_3 || d5)	b[i0] = y3/3.;
			else if(Type==SMOOTH_LINE_5)		b[i0] = y5/5.;
			else if(Type==SMOOTH_QUAD_5)				b[i0] = (17*y5-5*x2y)/35.;
			if(delta>0)		b[i0] = mgl_max(a[i0]-delta,mgl_min(a[i0]+delta,b[i0]));
		}
		memcpy(a,b,nx*ny*nz*sizeof(float));
	}
	delete []b;
}
//-----------------------------------------------------------------------------
mglData &mglData::Sum(const char *dir)
{
	register long i,j,k,i0;
	long kx=nx,ky=ny,kz=nz;
	float *b = new float[nx*ny*nz];
	static mglData d;
	memset(b,0,nx*ny*nz*sizeof(float));
	if(strchr(dir,'z') && kz>1)
	{
		for(i=0;i<kx*ky;i++)
		{
			for(j=0;j<kz;j++)
				b[i] += a[i+kx*ky*j];
			b[i] /= kz;
		}
		kz = 1;
	}
	if(strchr(dir,'y') && ky>1)
	{
		for(i=0;i<kx;i++)  for(k=0;k<kz;k++)
		{
			i0 = i+kx*ky*k;
			for(j=0;j<ky;j++)
				b[i+kx*k] += a[i0+kx*j];
			b[i+kx*k] /= ky;
		}
		ky = kz;  kz = 1;
	}
	if(strchr(dir,'x') && kx>1)
	{
		for(j=0;j<ky*kz;j++)
		{
			for(i=0;i<kx;i++)
				b[j] += a[i+kx*j];
			b[j] /= kx;
		}
		kx = ky;  ky = kz;  kz = 1;
	}
	d.Set(b,kx,ky,kz);
	delete []b;
	return d;
}
//-----------------------------------------------------------------------------
void mglData::CumSum(const char *dir)
{
	register long i,j,k,i0;
	//mglData d;
	float *b = new float[nx*ny*nz];
	memcpy(b,a,nx*ny*nz*sizeof(float));
	if(strchr(dir,'z') && nz>1)
	{
		for(i=0;i<nx*ny;i++)
		{
			b[i] = a[i];
			for(j=1;j<nz;j++)	b[i+j*nx*ny] = b[i+(j-1)*nx*ny] + a[i+nx*ny*j];
		}
		memcpy(a,b,nx*ny*nz*sizeof(float));
	}
	if(strchr(dir,'y') && ny>1)
	{
		for(i=0;i<nx;i++)  for(k=0;k<nz;k++)
		{
			i0 = i+nx*ny*k;		b[i0] = a[i0];
			for(j=1;j<ny;j++)	b[i0+j*nx] = b[i0+j*nx-nx] + a[i0+nx*j];
		}
		memcpy(a,b,nx*ny*nz*sizeof(float));
	}
	if(strchr(dir,'x') && nx>1)
	{
		for(j=0;j<ny*nz;j++)
		{
			i0 = j*nx;			b[i0] = a[i0];
			for(i=1;i<nx;i++)	b[i+i0] = b[i+i0-1] + a[i+i0];
		}
		memcpy(a,b,nx*ny*nz*sizeof(float));
	}
	delete []b;
}
//-----------------------------------------------------------------------------
void mglData::Integral(const char *dir)
{
	register long i,j,k,i0;
	//mglData d;
	float *b = new float[nx*ny*nz];
	memcpy(b,a,nx*ny*nz*sizeof(float));
	if(strchr(dir,'z') && nz>1)
	{
		for(i=0;i<nx*ny;i++)
		{
			b[i] = 0;
			for(j=1;j<nz;j++)
				b[i+j*nx*ny] = b[i+(j-1)*nx*ny] + (a[i+nx*ny*j]+a[i+nx*ny*(j-1)])/2/nz;
		}
		memcpy(a,b,nx*ny*nz*sizeof(float));
	}
	if(strchr(dir,'y') && ny>1)
	{
		for(i=0;i<nx;i++)  for(k=0;k<nz;k++)
		{
			i0 = i+nx*ny*k;
			b[i0] = 0;
			for(j=1;j<ny;j++)
				b[i0+j*nx] = b[i0+j*nx-nx] + (a[i0+nx*j]+a[i0+j*nx-nx])/2/ny;
		}
		memcpy(a,b,nx*ny*nz*sizeof(float));
	}
	if(strchr(dir,'x') && nx>1)
	{
		for(j=0;j<ny*nz;j++)
		{
			i0 = j*nx;
			b[i0] = 0;
			for(i=1;i<nx;i++)
				b[i+i0] = b[i+i0-1] + (a[i+i0]+a[i+i0-1])/2/nx;
		}
		memcpy(a,b,nx*ny*nz*sizeof(float));
	}
	delete []b;
}
//-----------------------------------------------------------------------------
void mglData::Diff(const char *dir)
{
	register long i,j,k,i0;
	float *b = new float[nx*ny*nz];
	if(strchr(dir,'z') && nz>1)
	{
		for(i=0;i<nx*ny;i++)
		{
			b[i] = -(3*a[i]-4*a[i+nx*ny]+a[i+2*nx*ny])*nz/2;
			b[i+(nz-1)*nx*ny] = (3*a[i+(nz-1)*nx*ny]-4*a[i+(nz-2)*nx*ny]+a[i+(nz-3)*nx*ny])*nz/2;
			for(j=1;j<nz-1;j++)
				b[i+j*nx*ny] = (a[i+nx*ny*(j+1)]-a[i+nx*ny*(j-1)])*nz/2;
		}
		memcpy(a,b,nx*ny*nz*sizeof(float));
	}
	if(strchr(dir,'y') && ny>1)
	{
		for(i=0;i<nx;i++)  for(k=0;k<nz;k++)
		{
			i0 = i+nx*ny*k;
			b[i0] = -(3*a[i0]-4*a[i0+nx]+a[i0+2*nx])*ny/2;
			b[i0+(ny-1)*nx] = (3*a[i0+(ny-1)*nx]-4*a[i0+(ny-2)*nx]+a[i0+(ny-3)*nx])*ny/2;
			for(j=1;j<ny-1;j++)
				b[i0+j*nx] = (a[i0+nx*j+nx]-a[i0+j*nx-nx])*ny/2;
		}
		memcpy(a,b,nx*ny*nz*sizeof(float));
	}
	if(strchr(dir,'x') && nx>1)
	{
		for(j=0;j<ny*nz;j++)
		{
			i0 = j*nx;
			b[i0] = -(3*a[i0]-4*a[i0+1]+a[i0+2])*nx/2;
			b[i0+nx-1] = (3*a[i0+nx-1]-4*a[i0+nx-2]+a[i0+nx-3])*nx/2;
			for(i=1;i<nx-1;i++)
				b[i+i0] = (a[i+i0+1]-a[i+i0-1])*nx/2;
		}
		memcpy(a,b,nx*ny*nz*sizeof(float));
	}
	delete []b;
}
//-----------------------------------------------------------------------------
void mglData::Diff2(const char *dir)
{
	register long i,j,k,i0;
	float *b = new float[nx*ny*nz];
	if(strchr(dir,'z') && nz>1)
	{
		for(i=0;i<nx*ny;i++)
		{
			b[i] = b[i+(nz-1)*nx*ny] = 0;
			for(j=1;j<nz-1;j++)
				b[i+j*nx*ny] = (a[i+nx*ny*(j+1)]-2*a[i+nx*ny*j]+a[i+nx*ny*(j-1)])*nz*nz/2;
		}
		memcpy(a,b,nx*ny*nz*sizeof(float));
	}
	if(strchr(dir,'y') && ny>1)
	{
		for(i=0;i<nx;i++)  for(k=0;k<nz;k++)
		{
			i0 = i+nx*ny*k;
			b[i0] = b[i0+(ny-1)*nx] = 0;
			for(j=1;j<ny-1;j++)
				b[i0+j*nx] = (a[i0+nx*j+nx]-2*a[i0+j*nx]+a[i0+j*nx-nx])*ny*ny/2;
		}
		memcpy(a,b,nx*ny*nz*sizeof(float));
	}
	if(strchr(dir,'x') && nx>1)
	{
		for(j=0;j<ny*nz;j++)
		{
			i0 = j*nx;
			b[i0] = b[i0+nx-1] = 0;
			for(i=1;i<nx-1;i++)
				b[i+i0] = (a[i+i0+1]-2*a[i+i0]+a[i+i0-1])*nx*nx/2;
		}
		memcpy(a,b,nx*ny*nz*sizeof(float));
	}
	delete []b;
}
//-----------------------------------------------------------------------------
float mglData::Spline(float x,float y,float z)
{
	register long i,j;
	register float fx=1, fy=1;
	long kx=long(x),ky=long(y),kz=long(z);
	float b=0;
	if(x<0 || y<0 || z<0 || x>nx-1 || y>ny-1 || z>nz-1)
		return 0;
	if(nz>1 && z!=kz)		// 3d interpolation
	{
		float b1[4]={0,0,0,0};
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
			{	b += fy*fx*_p[i][j];	fy *= y-ky;	}
			fx *= x-kx;
		}
	}
	else if(nx>1 && x!=kx)	// 1d interpolation
	{
		if(kx>nx-2)	kx = nx-2;
		mglFillP(kx, a+(ky+ny*kz)*nx, nx, _p[0]);
		for(i=0,fx=1,b=0;i<4;i++)
		{	b += fx*_p[0][i];	fx *= x-kx;	}
	}
	else					// no interpolation
		b = a[kx+nx*(ky+ny*kz)];
	return b;
}
//-----------------------------------------------------------------------------
float mglData::Linear(float x,float y,float z)
{
	register long i0;
	long kx,ky,kz;
	float b=0,dx,dy,dz,b1,b0;
	if(x<0 || y<0 || z<0 || x>nx-1 || y>ny-1 || z>nz-1)
		return 0;
	if(nz>1 && z!=long(z))		// 3d interpolation
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
	else if(ny>1 && y!=long(y))	// 2d interpolation
	{
		kx=long(x);	ky=long(y);
		dx = x-kx;	dy=y-ky;
		i0 = kx+nx*ky;
		b = a[i0]*(1-dx-dy+dx*dy) + dx*(1-dy)*a[i0+1] +
			dy*(1-dx)*a[i0+nx] + dx*dy*a[i0+nx+1];
	}
	else if(nx>1 && x!=long(x))	// 1d interpolation
	{
		kx = long(x);
		b = a[kx] + (x-kx)*(a[kx+1]-a[kx]);
	}
	else						// no interpolation
	{
		b = a[long(x+nx*(y+ny*z))];
	}
	return b;
}
//-----------------------------------------------------------------------------
void mglFillP(int x,int y,float *a,int nx,int ny,float _p[4][4])
{
	float sx[4]={0,0,0,0},sy[4]={0,0,0,0},f[4]={0,0,0,0},d[4]={0,0,0,0};
	if(x<0 || y<0 || x>nx-2 || y>ny-2)
	{
		memset(_p[0],0,4*sizeof(float));
		memset(_p[1],0,4*sizeof(float));
		memset(_p[2],0,4*sizeof(float));
		memset(_p[3],0,4*sizeof(float));
		return;
	}
	// значения функции
	f[0]=a[x+nx*y];		f[1]=a[x+nx*(y+1)];
	if(nx>1)	{	f[2]=a[x+1+nx*y];	f[3]=a[x+1+nx*(y+1)];	}
	else		{	f[2] = f[0];	f[3] = f[1];	}
	// производные по x
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
	// производные по y
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
	// перекрестные производные
	if(nx>1)
	{
		// расчет d[0]
		if(y==0 && x==0)
			d[0]=(a[x+1+nx*(y+1)]-a[x+nx*(y+1)]-a[x+1+nx*y]+a[x+nx*y]);
		else if(y==0)
			d[0]=(a[x+1+nx*(y+1)]-a[x-1+nx*(y+1)]-a[x+1+nx*y]+a[x-1+nx*y])/2;
		else if(x==0)
			d[0]=(a[x+1+nx*(y+1)]-a[x+nx*(y+1)]-a[x+1+nx*(y-1)]+a[x+nx*(y-1)])/2;
		else
			d[0]=(a[x+1+nx*(y+1)]-a[x-1+nx*(y+1)]-a[x+1+nx*(y-1)]+a[x-1+nx*(y-1)])/4;
		// расчет d[1]
		if(y==ny-2 && x==0)
			d[1]=(a[x+1+nx*(y+1)]-a[x+nx*(y+1)]-a[x+1+nx*y]+a[x+nx*y]);
		else if(y==ny-2)
			d[1]=(a[x+1+nx*(y+1)]-a[x-1+nx*(y+1)]-a[x+1+nx*y]+a[x-1+nx*y])/2;
		else if(x==0)
			d[1]=(a[x+1+nx*(y+2)]-a[x+nx*(y+2)]-a[x+1+nx*y]+a[x+nx*y])/2;
		else
			d[1]=(a[x+1+nx*(y+2)]-a[x-1+nx*(y+2)]-a[x+1+nx*y]+a[x-1+nx*y])/4;
		// расчет d[2]
		if(y==0 && x==nx-2)
			d[2]=(a[x+1+nx*(y+1)]-a[x+nx*(y+1)]-a[x+1+nx*y]+a[x+nx*y]);
		else if(y==0)
			d[2]=(a[x+2+nx*(y+1)]-a[x+nx*(y+1)]-a[x+2+nx*y]+a[x+nx*y])/2;
		else if(x==nx-2)
			d[2]=(a[x+1+nx*(y+1)]-a[x+nx*(y+1)]-a[x+1+nx*(y-1)]+a[x+nx*(y-1)])/2;
		else
			d[2]=(a[x+2+nx*(y+1)]-a[x+nx*(y+1)]-a[x+2+nx*(y-1)]+a[x+nx*(y-1)])/4;
		// расчет d[3]
		if(y==ny-2 && x==nx-2)
			d[3]=(a[x+1+nx*(y+1)]-a[x+nx*(y+1)]-a[x+1+nx*y]+a[x+nx*y]);
		else if(y==ny-2)
			d[3]=(a[x+2+nx*(y+1)]-a[x+nx*(y+1)]-a[x+2+nx*y]+a[x+nx*y])/2;
		else if(x==nx-2)
			d[3]=(a[x+1+nx*(y+2)]-a[x+nx*(y+2)]-a[x+1+nx*y]+a[x+nx*y])/2;
		else
			d[3]=(a[x+2+nx*(y+2)]-a[x+nx*(y+2)]-a[x+2+nx*y]+a[x+nx*y])/4;
	}
	// вычисляем коэффициенты полинома
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
void mglFillP(int x,float *a,int nx,float _p[4])
{
	if(x<0 || x>nx-2)
	{
		memset(_p,0,4*sizeof(float));
		return;
	}
	float s[2],f[2];
	// значения функции
	f[0]=a[x];		f[1]=a[x+1];
	// производные по x
	if(x==0)	s[0]=a[x+1]-a[x];
	else		s[0]=(a[x+1]-a[x-1])/2;
	if(x==nx-2)	s[1]=a[x+1]-a[x];
	else		s[1]=(a[x+2]-a[x])/2;
	// вычисляем коэффициенты полинома
	_p[0]=f[0];		_p[1]=s[0];
	_p[2]=3*(f[1]-f[0])-2*s[0]-s[1];
	_p[3]=s[0]+s[1]+2*(f[0]-f[1]);
}
//-----------------------------------------------------------------------------
void mglData::Crop(int n1,int n2,char dir)
{
	long nn;
	register long i,k;
	float *b;
	if(n1<0)	n1=0;
	switch(dir)
	{
	case 'x':
		if(n2<0 || n2>=nx)	n2 = nx;
		nn = n2-n1;	b = new float[nn*ny*nz];
		for(i=0;i<ny*nz;i++)
			memcpy(b+nn*i,a+nx*i+n1,nn*sizeof(float));
		nx = nn;	delete []a;		a = b;
		break;
	case 'y':
		if(n2<0 || n2>=ny)	n2 = ny;
		nn = n2-n1;	b = new float[nn*ny*nz];
		for(i=0;i<nx;i++)	for(long j=0;j<nz;j++)	for(k=0;k<nn;k++)
			b[i+nx*(k+nn*j)] = a[i+nx*(n1+k+ny*j)];
		ny = nn;	delete []a;		a = b;
		break;
	case 'z':
		if(n2<0 || n2>=nz)	n2 = nz;
		nn = n2-n1;	b = new float[nn*nx*ny];
		for(i=0;i<nx*ny;i++)	for(k=0;k<nn;k++)
			b[i+nx*ny*k] = a[i+nx*ny*(n1+k)];
		nz = nn;	delete []a;		a = b;
		break;
	}
}
//-----------------------------------------------------------------------------
mglData &mglData::Hist(int n,float v1,float v2,int nsub)
{
	static mglData b;
	register long i,k;
	b.Create(n);
	if(v1==v2)	return b;
	if(nsub==0)	for(i=0;i<nx*ny*nz;i++)
	{
		k = long(n*(a[i]-v1)/(v2-v1));
		if(k>=0 && k<n)	b.a[k] +=1;
	}
	else
	{
		register float x,y,z,d=1./(abs(n)+1),f;
		bool sp = n>0;
		for(x=0;x<nx;x+=d)	for(y=0;y<ny;y+=d)	for(z=0;z<nz;z+=d)
		{
			f = sp ? Spline(x,y,z) : Linear(x,y,z);
			k = long(n*(f-v1)/(v2-v1));
			if(k>=0 && k<n)	b.a[k] += d*d*d;
		}
	}
	return b;
}
//-----------------------------------------------------------------------------
mglData &mglData::Hist(mglData &w, int n,float v1,float v2,int nsub)
{
	static mglData b;
	register long i,k;
	b.Create(n);
	if(v1==v2 || nx*ny*nz!=w.nx*w.ny*w.nz)	return b;
	if(nsub==0)	for(i=0;i<nx*ny*nz;i++)
	{
		k = long(n*(a[i]-v1)/(v2-v1));
		if(k>=0 && k<n)	b.a[k] += w.a[i];
	}
	else
	{
		register float x,y,z,d=1./(abs(n)+1),f,g;
		bool sp = n>0;
		for(x=0;x<nx;x+=d)	for(y=0;y<ny;y+=d)	for(z=0;z<nz;z+=d)
		{
			if(sp)
			{	f = Spline(x,y,z);	g = w.Spline(x,y,z);	}
			else
			{	f = Linear(x,y,z);	g = w.Linear(x,y,z);	}
			k = long(n*(f-v1)/(v2-v1));
			if(k>=0 && k<n)	b.a[k] += g*d*d*d;
		}
	}
	return b;
}
//-----------------------------------------------------------------------------
mglData &mglData::Max(const char *dir)
{
	register long i,j,k,i0;
	long kx=nx,ky=ny,kz=nz;
	float *b = new float[nx*ny*nz];
	static mglData d;
//	memset(b,0,nx*ny*nz*sizeof(float));
	if(strchr(dir,'z') && kz>1)
	{
		for(i=0;i<kx*ky;i++)
		{
			b[i] = a[i];
			for(j=1;j<kz;j++)
				b[i] = b[i] > a[i+kx*ky*j] ? b[i] : a[i+kx*ky*j];
		}
		kz = 1;
	}
	if(strchr(dir,'y') && ky>1)
	{
		for(i=0;i<kx;i++)  for(k=0;k<kz;k++)
		{
			i0 = i+kx*ky*k;
			b[i+kx*k] = a[i0];
			for(j=1;j<ky;j++)
				b[i+kx*k] = b[i+kx*k] > a[i0+kx*j] ? b[i+kx*k] : a[i0+kx*j];
		}
		ky = kz;  kz = 1;
	}
	if(strchr(dir,'x') && kx>1)
	{
		for(j=0;j<ky*kz;j++)
		{
			b[j] = a[kx*j];
			for(i=1;i<kx;i++)
				b[j] = b[j] > a[i+kx*j] ? b[j] : a[i+kx*j];
		}
		kx = ky;  ky = kz;  kz = 1;
	}
	d.Set(b,kx,ky,kz);
	delete []b;
	return d;
}
//-----------------------------------------------------------------------------
mglData &mglData::Min(const char *dir)
{
	register long i,j,k,i0;
	long kx=nx,ky=ny,kz=nz;
	float *b = new float[nx*ny*nz];
	static mglData d;
//	memset(b,0,nx*ny*nz*sizeof(float));
	if(strchr(dir,'z') && kz>1)
	{
		for(i=0;i<kx*ky;i++)
		{
			b[i] = a[i];
			for(j=1;j<kz;j++)
				b[i] = b[i] < a[i+kx*ky*j] ? b[i] : a[i+kx*ky*j];
		}
		kz = 1;
	}
	if(strchr(dir,'y') && ky>1)
	{
		for(i=0;i<kx;i++)  for(k=0;k<kz;k++)
		{
			i0 = i+kx*ky*k;
			b[i+kx*k] = a[i0];
			for(j=1;j<ky;j++)
				b[i+kx*k] = b[i+kx*k] < a[i0+kx*j] ? b[i+kx*k] : a[i0+kx*j];
		}
		ky = kz;  kz = 1;
	}
	if(strchr(dir,'x') && kx>1)
	{
		for(j=0;j<ky*kz;j++)
		{
			b[j] = a[kx*j];
			for(i=1;i<kx;i++)
				b[j] = b[j] < a[i+kx*j] ? b[j] : a[i+kx*j];
		}
		kx = ky;  ky = kz;  kz = 1;
	}
	d.Set(b,kx,ky,kz);
	delete []b;
	return d;
}
//-----------------------------------------------------------------------------
float mglData::Last(const char *cond, int &i, int &j, int &k)
{
	if(!cond)	cond = "u";
	mglFormula eq(cond);
	if(i<0 || i>=nx)	i=nx;
	if(j<0 || j>=ny)	j=ny-1;
	if(k<0 || k>=nz)	k=nz-1;
	long i0 = i+nx*(j+ny*k)-1;
	float x,y,z,dx=nx>1?1/(nx-1.):0,dy=ny>1?1/(ny-1.):0,dz=nz>1?1/(nz-1.):0;
	for(;i0>=0;i0--)
	{
		x = dx*(i0%nx);		y = dy*((i0/nx)%ny);	z = dz*(i0/(nx*ny));
		if(eq.Calc(x,y,z,a[i0]))	break;
	}
	i = i0%nx;	j = (i0/nx)%ny;	k = i0/(nx*ny);
	return i0<nx*ny*nz ? a[i0] : 0;
}
//-----------------------------------------------------------------------------
float mglData::Find(const char *cond, int &i, int &j, int &k)
{
	if(!cond)	cond = "u";
	mglFormula eq(cond);
	if(i<0 || i>=nx)	i=-1;
	if(j<0 || j>=ny)	j=0;
	if(k<0 || k>=nz)	k=0;
	long i0 = i+nx*(j+ny*k)+1;
	float x,y,z,dx=nx>1?1/(nx-1.):0,dy=ny>1?1/(ny-1.):0,dz=nz>1?1/(nz-1.):0;
	for(;i0<nx*ny*nz;i0++)
	{
		x = dx*(i0%nx);		y = dy*((i0/nx)%ny);	z = dz*(i0/(nx*ny));
		if(eq.Calc(x,y,z,a[i0]))	break;
	}
	i = i0%nx;	j = (i0/nx)%ny;	k = i0/(nx*ny);
	return i0<nx*ny*nz ? a[i0] : 0;
}
//-----------------------------------------------------------------------------
int mglData::Find(const char *cond, char dir, int i, int j, int k)
{
	register int m=-1;
	if(!cond)	cond = "u";
	mglFormula eq(cond);
	float x=i/(nx-1.),y=j/(ny-1.),z=k/(nz-1.);
	if(dir=='x' && nx>1)
	{
		for(m=i;m<nx;m++)
		{
			x = m/(nx-1.);
			if(eq.Calc(x,y,z,a[m+nx*(j+ny*k)]))	break;
		}
	}
	if(dir=='y' && ny>1)
	{
		for(m=j;m<ny;m++)
		{
			y = m/(ny-1.);
			if(eq.Calc(x,y,z,a[i+nx*(m+ny*k)]))	break;
		}
	}
	if(dir=='z' && nz>1)
	{
		for(m=k;m<nz;m++)
		{
			z = m/(nz-1.);
			if(eq.Calc(x,y,z,a[i+nx*(j+ny*m)]))	break;
		}
	}
	return m;
}
//-----------------------------------------------------------------------------
bool mglData::FindAny(const char *cond)
{
	register long i,j,k;
	register float x,y,z;
	bool cc = false;
	if(!cond || *cond==0)	cond = "u";
	mglFormula eq(cond);
	for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
	{
		x=i/(nx-1.);	y=j/(ny-1.);	z=k/(nz-1.);
		if(eq.Calc(x,y,z,a[i+nx*(j+ny*k)]))	{	cc = true;	break;	}
	}
	return cc;
}
//-----------------------------------------------------------------------------
mglData &TransformA(mglData &am, mglData &ph, const char *tr)
{
	static mglData d;
#ifndef NO_GSL
	int nx = am.nx, ny = am.ny, nz = am.nz;
	if(nx*ny*nz != ph.nx*ph.ny*ph.nz || !tr || tr[0]==0)
	{	d.Create(1,1,1);	return d;	}
	double *a = new double[2*nx*ny*nz];
	register long i,j;
	for(i=0;i<nx*ny*nz;i++)
	{	a[2*i] = am.a[i]*cos(ph.a[i]);	a[2*i+1] = am.a[i]*sin(ph.a[i]);	}
	if((tr[0]=='f' || tr[0]=='i') && nx>1)
	{
		gsl_fft_complex_wavetable *wt = gsl_fft_complex_wavetable_alloc(nx);
		gsl_fft_complex_workspace *ws = gsl_fft_complex_workspace_alloc(nx);
		for(i=0;i<ny*nz;i++)
			gsl_fft_complex_transform(a+2*i*nx, 1, nx, wt, ws, tr[0]=='f'?forward:backward);
		gsl_fft_complex_workspace_free(ws);
		gsl_fft_complex_wavetable_free(wt);
	}
	if((tr[1]=='f' || tr[1]=='i') && ny>1)
	{
		gsl_fft_complex_wavetable *wt = gsl_fft_complex_wavetable_alloc(ny);
		gsl_fft_complex_workspace *ws = gsl_fft_complex_workspace_alloc(ny);
		for(i=0;i<nx;i++)	for(j=0;j<nz;j++)
			gsl_fft_complex_transform(a+2*i+2*j*nx*ny, nx, ny, wt, ws, tr[1]=='f'?forward:backward);
		gsl_fft_complex_workspace_free(ws);
		gsl_fft_complex_wavetable_free(wt);
	}
	if((tr[2]=='f' || tr[2]=='i') && nz>1)
	{
		gsl_fft_complex_wavetable *wt = gsl_fft_complex_wavetable_alloc(nz);
		gsl_fft_complex_workspace *ws = gsl_fft_complex_workspace_alloc(nz);
		for(i=0;i<ny*nx;i++)
			gsl_fft_complex_transform(a+2*i, nx*ny, nz, wt, ws, tr[2]=='f'?forward:backward);
		gsl_fft_complex_workspace_free(ws);
		gsl_fft_complex_wavetable_free(wt);
	}
	d.Create(nx, ny, nz);
	for(i=0;i<nx*ny*nz;i++)		d.a[i] = hypot(a[2*i],a[2*i+1]);
#endif
	return d;
}
//-----------------------------------------------------------------------------
mglData &Transform(mglData &re, mglData &im, const char *tr)
{
	static mglData d;
#ifndef NO_GSL
	int nx = re.nx, ny = re.ny, nz = re.nz;
	if(nx*ny*nz != im.nx*im.ny*im.nz || !tr || tr[0]==0)
	{	d.Create(1,1,1);	return d;	}
	double *a = new double[2*nx*ny*nz];
	register long i,j;
	for(i=0;i<nx*ny*nz;i++)
	{	a[2*i] = re.a[i];	a[2*i+1] = im.a[i];	}
	if((tr[0]=='f' || tr[0]=='i') && nx>1)
	{
		gsl_fft_complex_wavetable *wt = gsl_fft_complex_wavetable_alloc(nx);
		gsl_fft_complex_workspace *ws = gsl_fft_complex_workspace_alloc(nx);
		for(i=0;i<ny*nz;i++)
			gsl_fft_complex_transform(a+2*i*nx, 1, nx, wt, ws, tr[0]=='f'?forward:backward);
		gsl_fft_complex_workspace_free(ws);
		gsl_fft_complex_wavetable_free(wt);
	}
	if((tr[1]=='f' || tr[1]=='i') && ny>1)
	{
		gsl_fft_complex_wavetable *wt = gsl_fft_complex_wavetable_alloc(ny);
		gsl_fft_complex_workspace *ws = gsl_fft_complex_workspace_alloc(ny);
		for(i=0;i<nx;i++)	for(j=0;j<nz;j++)
			gsl_fft_complex_transform(a+2*i+2*j*nx*ny, nx, ny, wt, ws, tr[1]=='f'?forward:backward);
		gsl_fft_complex_workspace_free(ws);
		gsl_fft_complex_wavetable_free(wt);
	}
	if((tr[2]=='f' || tr[2]=='i') && nz>1)
	{
		gsl_fft_complex_wavetable *wt = gsl_fft_complex_wavetable_alloc(nz);
		gsl_fft_complex_workspace *ws = gsl_fft_complex_workspace_alloc(nz);
		for(i=0;i<ny*nx;i++)
			gsl_fft_complex_transform(a+2*i, nx*ny, nz, wt, ws, tr[2]=='f'?forward:backward);
		gsl_fft_complex_workspace_free(ws);
		gsl_fft_complex_wavetable_free(wt);
	}
	d.Create(nx, ny, nz);
	for(i=0;i<nx*ny*nz;i++)		d.a[i] = hypot(a[2*i],a[2*i+1]);
#endif
	return d;
}
//-----------------------------------------------------------------------------
mglData &STFA(mglData &re, mglData &im, int dn, char dir)
{
	static mglData d;
#ifndef NO_GSL
	long nx = re.nx, ny = re.ny;
	if(nx*ny!=im.nx*im.ny)	return d;
	register long i,j,k;
	double *a = new double[2*nx*ny];
	for(i=0;i<nx*ny;i++)	{	a[2*i] = re.a[i];	a[2*i+1] = im.a[i];	}
	gsl_fft_complex_wavetable *wt = gsl_fft_complex_wavetable_alloc(dn);
	gsl_fft_complex_workspace *ws = gsl_fft_complex_workspace_alloc(dn);
	long mx,my,mz;
	if(dir=='y')
	{
		mx = nx;	my = dn;	mz = ny/dn;
		for(i=0;i<mx;i++)	for(j=0;j<mz;j++)
			gsl_fft_complex_forward(a+2*(i+j*mx*my), mx, dn, wt, ws);
		d.Create(mx, my, mz);
		for(i=0;i<mx*my*mz;i++)		d.a[i] = hypot(a[2*i], a[2*i+1]);
	}
	else
	{
		mx = dn;	my = nx/dn;	mz = ny;
		for(i=0;i<my;i++)	for(j=0;j<mz;j++)
		gsl_fft_complex_forward(a+2*(i*dn+j*nx), 1, dn, wt, ws);
		d.Create(mx, my, mz);
		for(i=0;i<mx;i++)	for(j=0;j<my;j++)	for(k=0;k<mz;k++)
			d.a[i+mx*(j+mz*k)] = hypot(a[2*(i+dn*j+k*nx)],a[2*(i+dn*j+k*nx)+1]);
	}
	gsl_fft_complex_workspace_free(ws);
	gsl_fft_complex_wavetable_free(wt);
#endif
	return d;
}
//-----------------------------------------------------------------------------
void mglData::Swap(const char *dir)
{
	register long i,j,k,i0,nn,j0;
	float b;
	if(strchr(dir,'z') && nz>1)
	{
		for(i=0;i<nx*ny;i++)
		{
			nn = (nz/2)*nx*ny;
			for(j=0;j<nz/2;j++)
			{	i0 = i+j*nx*ny;	b = a[i0];	a[i0] = a[i0+nn];	a[i0+nn] = b;	}
		}
	}
	if(strchr(dir,'y') && ny>1)
	{
		nn = (ny/2)*nx;
		for(i=0;i<nx;i++)  for(k=0;k<nz;k++)
		{
			j0 = i+nx*ny*k;
			for(j=0;j<ny/2;j++)
			{	i0 = j0+j*nx;	b = a[i0];	a[i0] = a[i0+nn];	a[i0+nn] = b;	}
		}
	}
	if(strchr(dir,'x') && nx>1)
	{
		nn = nx/2;
		for(j=0;j<ny*nz;j++)
		{
			j0 = j*nx;
			for(i=0;i<nx/2;i++)
			{	i0 = i+j0;	b = a[i0];	a[i0] = a[i0+nn];	a[i0+nn] = b;	}
		}
	}
}
//-----------------------------------------------------------------------------
void mglData::Mirror(const char *dir)
{
	register long i,j,k,i0,j0;
	float b;
	if(strchr(dir,'z') && nz>1)
	{
		for(i=0;i<nx*ny;i++)	for(j=0;j<nz/2;j++)
		{
			i0 = i+j*nx*ny;	j0 = i+(nz-1-j)*nx*ny;
			b = a[i0];	a[i0] = a[j0];	a[j0] = b;
		}
	}
	if(strchr(dir,'y') && ny>1)
	{
		for(i=0;i<nx;i++)  for(k=0;k<nz;k++)
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
//-----------------------------------------------------------------------------
float mglData::Momentum(char dir,float &x,float &w)
{
	float i0=0,i1=0,i2=0,d;
	register long i;
	switch(dir)
	{
		case 'x':
			for(i=0;i<nx*ny*nz;i++)
			{
				d = i%nx;		i0+= a[i];
				i1+= a[i]*d;	i2+= a[i]*d*d;
			}
			break;
		case 'y':
			for(i=0;i<nx*ny*nz;i++)
			{
				d = (i/nx)%ny;	i0+= a[i];
				i1+= a[i]*d;	i2+= a[i]*d*d;
			}
			break;
		case 'z':
			for(i=0;i<nx*ny*nz;i++)
			{
				d = i/(nx*ny);	i0+= a[i];
				i1+= a[i]*d;	i2+= a[i]*d*d;
			}
			break;
		default:	// "self-dispersion"
			i0 = nx*ny*nz;
			for(i=0;i<nx*ny*nz;i++)
			{
				i1+= a[i];	i2+= a[i]*a[i];
			}
	}
	if(i0==0)	return 0;
	x = i1/i0;	w = i2>x*x*i0 ? sqrt(i2/i0 - x*x) : 0;
	return i0;
}
//-----------------------------------------------------------------------------
float mglData::Momentum(char dir,float &x,float &w,float &s,float &k)
{
	float i0=0,i1=0,i2=0,d,t;
	register long i;
	i0 = Momentum(dir,x,w);
	if(i0==0 || w==0)	return 0;
	switch(dir)
	{
		case 'x':
			for(i=0;i<nx*ny*nz;i++)
			{
				d = (i%nx - x)/w;		t = d*d;
				i1+= a[i]*t*d;	i2+= a[i]*t*t;
			}
			break;
		case 'y':
			for(i=0;i<nx*ny*nz;i++)
			{
				d = ((i/nx)%ny - x)/w;	t = d*d;
				i1+= a[i]*t*d;	i2+= a[i]*t*t;
			}
			break;
		case 'z':
			for(i=0;i<nx*ny*nz;i++)
			{
				d = (i/(nx*ny) - x)/w;	t = d*d;
				i1+= a[i]*t*d;	i2+= a[i]*t*t;
			}
			break;
		default:	// "self-dispersion"
			for(i=0;i<nx*ny*nz;i++)
			{
				d = (a[i] - x)/w;		t = d*d;
				i1+= t*d;	i2+= t*t;
			}
	}
	s = i1/i0;	k = i2/(i0*3);
	return i0;
}
//-----------------------------------------------------------------------------
void mglData::NormSl(float v1, float v2, char dir, bool keep_en, bool sym)
{
	mglData b(*this);
	register long i,k;
	register float e0=1, e=1, m1, m2, aa;
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
			for(i=0;i<nx;i++)	for(k=0;k<nz;k++)
			{
				aa = a[i+nx*(j+ny*k)];
				m1 = m1<aa ? m1 : aa;
				m2 = m2>aa ? m2 : aa;
				e += aa*aa;
			}
			if(m1==m2)	m2+=1;
			if(keep_en && k)	e = sqrt(e/e0);
			else	{	e0 = e;	e=1;	}
			for(i=0;i<nx;i++)	for(k=0;k<nz;k++)
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
			if(keep_en && k)	e = sqrt(e/e0);
			else	{	e0 = e;	e=1;	}
			for(k=0;k<ny*nz;k++)
				b.a[i+nx*k] = (v1 + (v2-v1)*(a[i+nx*k]-m1)/(m2-m1))*e;
		}
	}
	memcpy(a, b.a, nx*ny*nz*sizeof(float));
}
//-----------------------------------------------------------------------------
mglData &mglData::Momentum(char dir, const char *how)
{
	static mglData b;
	float i0=0,i1=0,d,aa;
	register long i,j,k;
	int n=1;
	char var = 0;
	if(strchr(how,'x'))	var = 'x';
	if(strchr(how,'y'))	var = 'y';
	if(strchr(how,'z'))	var = 'z';
	for(i=0;i<10;i++)	if(strchr(how,'0'+i))	{	n = i;	break;	}
	switch(dir)
	{
	case 'x':
		b.Create(nx);
		for(i=0;i<nx;i++)
		{
			i0 = i1 = 0;
			for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
			{
				d = (var=='z') ? k:j;
				i0+= a[i+nx*(j+ny*k)];
				i1+= a[i+nx*(j+ny*k)]*ipow(d,n);
			}
			b.a[i] = n>0 ? i1/i0 : i0;
		}
		break;
	case 'y':
		b.Create(ny);
		for(j=0;j<ny;j++)
		{
			i0 = i1 = 0;
			for(i=0;i<nx;i++)	for(k=0;k<nz;k++)
			{
				d = (var=='z') ? k:i;
				i0+= a[i+nx*(j+ny*k)];
				i1+= a[i+nx*(j+ny*k)]*ipow(d,n);
			}
			b.a[j] = n>0 ? i1/i0 : i0;
		}
		break;
	case 'z':
		b.Create(nz);
		for(k=0;k<nz;k++)
		{
			i0 = i1 = 0;
			for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
			{
				d = (var=='y') ? j:i;
				i0+= a[i+nx*(j+ny*k)];
				i1+= a[i+nx*(j+ny*k)]*ipow(d,n);
			}
			b.a[k] = n>0 ? i1/i0 : i0;
		}
		break;
	}
	return b;
}
//-----------------------------------------------------------------------------
void mglData::PrintInfo(FILE *fp)
{
	if(fp==0)	return;
	char *buf = new char[512];
	PrintInfo(buf);
	fprintf(fp,buf);	fflush(fp);
	delete []buf;
}
//-----------------------------------------------------------------------------
void mglData::PrintInfo(char *buf)
{
	if(buf==0)	return;
	char s[128];
	buf[0]=0;
	sprintf(s,"nx = %ld\tny = %ld\tnz = %ld\n",nx,ny,nz);	strcat(buf,s);

	int i=0,j=0,k=0;
	float b = Maximal(i,j,k);
	sprintf(s,"Maximum is %g\t at coordinates x = %d\ty = %d\tz = %d\n", b,i,j,k);	strcat(buf,s);
	b = Minimal(i,j,k);
	sprintf(s,"Minimum is %g\t at coordinates x = %d\ty = %d\tz = %d\n", b,i,j,k);	strcat(buf,s);
	float A=0,Wa=0,X=0,Y=0,Z=0,Wx=0,Wy=0,Wz=0;
	Momentum('x',X,Wx);		Momentum('y',Y,Wy);
	Momentum('z',Z,Wz);		Momentum(0,A,Wa);
	sprintf(s,"Averages are:\n<a> = %g\t<x> = %g\t<y> = %g\t<z> = %g\n", A,X,Y,Z);	strcat(buf,s);
	sprintf(s,"Widths (dispersions) are:\nWa = %g\tWx = %g\tWy = %g\tWz = %g\n\n",
		Wa,Wx,Wy,Wz);	strcat(buf,s);
}
//-----------------------------------------------------------------------------
void mglData::Rearrange(int mx, int my, int mz)
{
	if(mx<1)	return;	// wrong mx
	if(my<1)	{	my = nx*ny*nz/mx;	mz = 1;	}
	else if(mz<1)	mz = (nx*ny*nz)/(mx*my);
	long m = mx*my*mz;
	if(m==0 || m>nx*ny*nz)	return;	// too high desired dimensions
	nx = mx;	ny = my;	nz = mz;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
