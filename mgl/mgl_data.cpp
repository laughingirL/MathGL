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
#include <string.h>
#include "mgl/mgl_eval.h"
#include "mgl/mgl_data.h"

#ifndef NO_GSL
#include <gsl/gsl_fft_complex.h>
#endif

void mglFillP(int x,int y, const float *a,int nx,int ny,float _p[4][4]);
void mglFillP(int x, const float *a,int nx,float _p[4]);
void mglFillP5(int x,int y, const float *a,int nx,int ny,float _p[6][6]);
void mglFillP5(int x, const float *a,int nx,float _p[6]);
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
void mglData::Smooth(const char *dirs)
{
	int type = SMOOTH_QUAD_5;
	if(strchr(dirs,'0') || strchr(dirs,'1'))	return;
	if(strchr(dirs,'3'))	type = SMOOTH_LINE_3;
	if(strchr(dirs,'5'))	type = SMOOTH_LINE_5;
	Smooth(type, dirs, 0);
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
			else if(Type==SMOOTH_QUAD_5)		b[i0] = (17*y5-5*x2y)/35.;
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
			else if(Type==SMOOTH_QUAD_5)		b[i0] = (17*y5-5*x2y)/35.;
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
			else if(Type==SMOOTH_QUAD_5)		b[i0] = (17*y5-5*x2y)/35.;
			if(delta>0)		b[i0] = mgl_max(a[i0]-delta,mgl_min(a[i0]+delta,b[i0]));
		}
		memcpy(a,b,nx*ny*nz*sizeof(float));
	}
	delete []b;
}
//-----------------------------------------------------------------------------
mglData mglData::Sum(const char *dir) const
{
	register long i,j,k,i0;
	long kx=nx,ky=ny,kz=nz;
	float *b = new float[nx*ny*nz];
	mglData d;
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
float mglData::Linear(float x,float y,float z) const
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
float mglData::Spline(float x,float y,float z) const
{
	float _p[4][4];
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
void mglFillP(int x,int y, const float *a,int nx,int ny,float _p[4][4])
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
void mglFillP(int x, const float *a,int nx,float _p[4])
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
		n2 = n2>0 ? n2 : nx+n2;
		if(n2<0 || n2>=nx || n2<n1)	n2 = nx;
		nn = n2-n1;	b = new float[nn*ny*nz];
		for(i=0;i<ny*nz;i++)
			memcpy(b+nn*i,a+nx*i+n1,nn*sizeof(float));
		nx = nn;	delete []a;		a = b;
		break;
	case 'y':
		n2 = n2>0 ? n2 : ny+n2;
		if(n2<0 || n2>=ny || n2<n1)	n2 = ny;
		nn = n2-n1;	b = new float[nn*nx*nz];
		for(i=0;i<nx;i++)	for(long j=0;j<nz;j++)	for(k=0;k<nn;k++)
			b[i+nx*(k+nn*j)] = a[i+nx*(n1+k+ny*j)];
		ny = nn;	delete []a;		a = b;
		break;
	case 'z':
		n2 = n2>0 ? n2 : nz+n2;
		if(n2<0 || n2>=nz || n2<n1)	n2 = nz;
		nn = n2-n1;	b = new float[nn*nx*ny];
		for(i=0;i<nx*ny;i++)	for(k=0;k<nn;k++)
			b[i+nx*ny*k] = a[i+nx*ny*(n1+k)];
		nz = nn;	delete []a;		a = b;
		break;
	}
}
//-----------------------------------------------------------------------------
mglData mglData::Hist(int n,float v1,float v2,int nsub) const
{
	mglData b;
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
		register float x,y,z,d=1./(abs(nsub)+1),f;
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
mglData mglData::Hist(const mglData &w, int n,float v1,float v2,int nsub) const
{
	mglData b;
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
		register float x,y,z,d=1./(abs(nsub)+1),f,g;
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
mglData mglData::Max(const char *dir) const
{
	register long i,j,k,i0;
	long kx=nx,ky=ny,kz=nz;
	float *b = new float[nx*ny*nz];
	mglData d;
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
mglData mglData::Min(const char *dir) const
{
	register long i,j,k,i0;
	long kx=nx,ky=ny,kz=nz;
	float *b = new float[nx*ny*nz];
	mglData d;
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
float mglData::Last(const char *cond, int &i, int &j, int &k) const
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
float mglData::Find(const char *cond, int &i, int &j, int &k) const
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
int mglData::Find(const char *cond, char dir, int i, int j, int k) const
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
bool mglData::FindAny(const char *cond) const
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
mglData mglTransformA(const mglData &am, const mglData &ph, const char *tr)
{
	int nx = am.nx, ny = am.ny, nz = am.nz;
	if(nx*ny*nz != ph.nx*ph.ny*ph.nz || !tr || tr[0]==0)
		return mglData();
	mglData re(am), im(am);
	for(long i=0;i<nx*ny*nz;i++)
	{	re.a[i] = am.a[i]*cos(ph.a[i]);	im.a[i] = am.a[i]*sin(ph.a[i]);	}
	return mglTransform(re, im, tr);
}
//-----------------------------------------------------------------------------
mglData mglTransform(const mglData &re, const mglData &im, const char *tr)
{
	mglData d;
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
mglData mglSTFA(const mglData &re, const mglData &im, int dn, char dir)
{
	mglData d;
#ifndef NO_GSL
	if(dn<2)	return d;
	dn = 2*(dn/2);
	long nx = re.nx, ny = re.ny;
	if(nx*ny!=im.nx*im.ny)	return d;
	register long i,j,k,i0;
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
		d.Create(mx, mz, my);
//		for(i=0;i<mx*my*mz;i++)		d.a[i] = hypot(a[2*i], a[2*i+1])/dn;
		for(i=0;i<my/2;i++)	for(j=0;j<mx;j++)	for(k=0;k<mz;k++)
		{
			i0 = 2*(j+mx*(dn*i+k*mz));
			d.a[j+mx*(k+mz*(i+my/2))] = hypot(a[i0],a[i0+1])/dn;
			d.a[j+mx*(k+mz*(i))] = hypot(a[i0+mx*my],a[i0+mx*my+1])/dn;
		}
	}
	else
	{
		mx = dn;	my = nx/dn;	mz = ny;
		for(i=0;i<my;i++)	for(j=0;j<mz;j++)
			gsl_fft_complex_forward(a+2*(i*dn+j*nx), 1, dn, wt, ws);
		d.Create(my, mx, mz);
		for(i=0;i<mx/2;i++)	for(j=0;j<my;j++)	for(k=0;k<mz;k++)
		{
			i0 = 2*(i+dn*j+k*nx);
			d.a[j+my*(i+mx/2+mx*k)] = hypot(a[i0],a[i0+1])/dn;
			d.a[j+my*(i+mx*k)] = hypot(a[i0+mx],a[i0+mx+1])/dn;
		}
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
float mglData::Momentum(char dir,float &x,float &w) const
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
float mglData::Momentum(char dir,float &x,float &w,float &s,float &k) const
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
			if(keep_en && j)	e = sqrt(e/e0);
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
			if(keep_en && i)	e = sqrt(e/e0);
			else	{	e0 = e;	e=1;	}
			for(k=0;k<ny*nz;k++)
				b.a[i+nx*k] = (v1 + (v2-v1)*(a[i+nx*k]-m1)/(m2-m1))*e;
		}
	}
	memcpy(a, b.a, nx*ny*nz*sizeof(float));
}
//-----------------------------------------------------------------------------
mglData mglData::Momentum(char dir, const char *how) const
{
	mglData b;
	mglFormula eq(how);
	register long i,j,k,ii;
	float i0,i1,x,y,z;
	switch(dir)
	{
	case 'x':
		b.Create(nx);
		for(i=0;i<nx;i++)
		{
			i0 = i1 = 0;
			for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
			{
				ii = i+nx*(j+ny*k);
				x = i/(nx-1.);	y = j/(ny-1.);	z = k/(nz-1.);
				i0+= a[ii];
				i1+= a[ii]*eq.Calc(x,y,z,a[ii]);
			}
			b.a[i] = i0>0 ? i1/i0 : 0;
		}
		break;
	case 'y':
		b.Create(ny);
		for(j=0;j<ny;j++)
		{
			i0 = i1 = 0;
			for(i=0;i<nx;i++)	for(k=0;k<nz;k++)
			{
				ii = i+nx*(j+ny*k);
				x = i/(nx-1.);	y = j/(ny-1.);	z = k/(nz-1.);
				i0+= a[ii];
				i1+= a[ii]*eq.Calc(x,y,z,a[ii]);
			}
			b.a[i] = i0>0 ? i1/i0 : 0;
		}
		break;
	case 'z':
		b.Create(nz);
		for(k=0;k<nz;k++)
		{
			i0 = i1 = 0;
			for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
			{
				ii = i+nx*(j+ny*k);
				x = i/(nx-1.);	y = j/(ny-1.);	z = k/(nz-1.);
				i0+= a[ii];
				i1+= a[ii]*eq.Calc(x,y,z,a[ii]);
			}
			b.a[i] = i0>0 ? i1/i0 : 0;
		}
		break;
	}
	return b;
}
//-----------------------------------------------------------------------------
void mglData::PrintInfo(FILE *fp) const
{
	if(fp==0)	return;
	char *buf = new char[512];
	PrintInfo(buf);
	fprintf(fp,buf);	fflush(fp);
	delete []buf;
}
//-----------------------------------------------------------------------------
void mglData::PrintInfo(char *buf, bool all) const
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
	if(all)
	{
		float A=0,Wa=0,X=0,Y=0,Z=0,Wx=0,Wy=0,Wz=0;
		Momentum('x',X,Wx);		Momentum('y',Y,Wy);
		Momentum('z',Z,Wz);		Momentum(0,A,Wa);
		sprintf(s,"Averages are:\n<a> = %g\t<x> = %g\t<y> = %g\t<z> = %g\n", A,X,Y,Z);	strcat(buf,s);
		sprintf(s,"Widths (dispersions) are:\nWa = %g\tWx = %g\tWy = %g\tWz = %g\n",
			Wa,Wx,Wy,Wz);	strcat(buf,s);
	}
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
void mglData::InsertColumns(int at, int num, const char *eq)
{
	if(num<1)	return;
	mglData b(nx+num,ny,nz);
	if(at<1)	at=1;	if(at>nx)	at=nx;
	register long i,j,k;
	for(i=0;i<at;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
		b.a[i+(nx+num)*(j+ny*k)] = a[i+nx*(j+ny*k)];
	for(i=at;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
		b.a[i+num+(nx+num)*(j+ny*k)] = a[i+nx*(j+ny*k)];
	if(eq)
	{
		mglFormula e(eq);
		float dx,dy,dz;
		dx = num==1?0:1./(num-1);
		dy = ny==1?0:1./(ny-1);
		dz = nz==1?0:1./(nz-1);
		for(i=0;i<num;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
			b.a[i+at+(nx+num)*(j+ny*k)] = e.Calc(i*dx,j*dy, k*dz);
	}
	Set(b);
}
//-----------------------------------------------------------------------------
void mglData::InsertRows(int at, int num, const char *eq)
{
	if(num<1)	return;
	mglData b(nx,ny+num,nz);
	if(at<1)	at=1;	if(at>nx)	at=nx;
	register long i,j,k;
	for(i=0;i<nx;i++)	for(j=0;j<at;j++)	for(k=0;k<nz;k++)
		b.a[i+nx*(j+(ny+num)*k)] = a[i+nx*(j+ny*k)];
	for(i=0;i<nx;i++)	for(j=at;j<ny;j++)	for(k=0;k<nz;k++)
		b.a[i+nx*(j+num+(ny+num)*k)] = a[i+nx*(j+ny*k)];
	if(eq)
	{
		mglFormula e(eq);
		float dx,dy,dz;
		dy = num==1?0:1./(num-1);
		dx = nx==1?0:1./(nx-1);
		dz = nz==1?0:1./(nz-1);
		for(i=0;i<nx;i++)	for(j=0;j<num;j++)	for(k=0;k<nz;k++)
			b.a[i+nx*(j+at+(ny+num)*k)] = e.Calc(i*dx,j*dy, k*dz);
	}
	Set(b);
}
//-----------------------------------------------------------------------------
void mglData::InsertSlices(int at, int num, const char *eq)
{
	if(num<1)	return;
	mglData b(nx,ny,nz+num);
	if(at<1)	at=1;	if(at>nx)	at=nx;
	register long i,j,k;
	for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<at;k++)
		b.a[i+nx*(j+ny*k)] = a[i+nx*(j+ny*k)];
	for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=at;k<nz;k++)
		b.a[i+nx*(j+ny*(k+num))] = a[i+nx*(j+ny*k)];
	if(eq)
	{
		mglFormula e(eq);
		float dx,dy,dz;
		dz = num==1?0:1./(num-1);
		dx = nx==1?0:1./(nx-1);
		dy = ny==1?0:1./(ny-1);
		for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<num;k++)
			b.a[i+nx*(j+(at+k)*ny)] = e.Calc(i*dx,j*dy, k*dz);
	}
	Set(b);
}
//-----------------------------------------------------------------------------
void mglData::DeleteColumns(int at, int num)
{
	if(num<1 || at<0 || at+num>=nx)	return;
	mglData b(nx-num,ny,nz);
	register long i,j,k;
	for(i=0;i<at;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
		b.a[i+(nx-num)*(j+ny*k)] = a[i+nx*(j+ny*k)];
	for(i=at+num;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
		b.a[i-num+(nx-num)*(j+ny*k)] = a[i+nx*(j+ny*k)];
	Set(b);
}
//-----------------------------------------------------------------------------
void mglData::DeleteRows(int at, int num)
{
	if(num<1 || at<0 || at+num>=ny)	return;
	mglData b(nx,ny-num,nz);
	register long i,j,k;
	for(i=0;i<nx;i++)	for(j=0;j<at;j++)	for(k=0;k<nz;k++)
		b.a[i+nx*(j+(ny-num)*k)] = a[i+nx*(j+ny*k)];
	for(i=0;i<nx;i++)	for(j=at+num;j<ny;j++)	for(k=0;k<nz;k++)
		b.a[i+nx*(j-num+(ny-num)*k)] = a[i+nx*(j+ny*k)];
	Set(b);
}
//-----------------------------------------------------------------------------
void mglData::DeleteSlices(int at, int num)
{
	if(num<1 || at<0 || at+num>=nz)	return;
	mglData b(nx,ny,nz-num);
	register long i,j,k;
	for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<at;k++)
		b.a[i+nx*(j+ny*k)] = a[i+nx*(j+ny*k)];
	for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=at+num;k<nz;k++)
		b.a[i+nx*(j+(k-num)*num)] = a[i+nx*(j+ny*k)];
	Set(b);
}
//-----------------------------------------------------------------------------
void mglData::Insert(char dir, int at, int num)
{
	if(dir=='x')	InsertColumns(at,num);
	if(dir=='y')	InsertRows(at,num);
	if(dir=='z')	InsertSlices(at,num);
}
//-----------------------------------------------------------------------------
void mglData::Delete(char dir, int at, int num)
{
	if(dir=='x')	DeleteColumns(at,num);
	if(dir=='y')	DeleteRows(at,num);
	if(dir=='z')	DeleteSlices(at,num);
}
//-----------------------------------------------------------------------------
float mgl_spline5(float y1[5], float y2[5], int n1, int n2, float d, float &dy)
{
	float a1[4], a2[4], f0,d0,t0,f1,d1,t1, b[6];
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
float mgl_spline3(float y1[3], float y2[3], int n1, int n2, float d, float &dy)
{
	float a1[2], a2[2], f0,d0,d1,f1, b[4];
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
float mglData::Spline5(float x,float y,float z,float &dx,float &dy,float &dz) const
{
	float res=0;
	if(nx<5)	return 0;	// not interpolation for points < 5 !!!
	dx = dy = dz = 0;	x*=nx-1;	y*=ny-1;	z*=nz-1;
	if(ny==1 && nz==1)	// 1D case
	{
		int n = int(x), n1 = n>1 ? 2:n, n2 = n<nx-3 ? 1:5+n-nx;
		res = mgl_spline5(a+n+n1-2, a+n-n2, n1, n2, x-n, dx);
	}
/*	else if(nz==1)		// 2D case
	{
		if(ny<6)	return 0;	// not interpolation for points < 5 !!!
		int n = int(x), n1 = n>1 ? 2:n, n2 = n<nx-3 ? 1:5+n-nx;
		int m = int(y), m1 = m>1 ? 2:m, m2 = m<ny-3 ? 1:5+m-ny;
		float b[6],d[6],dd;
/*		m += m1-2 < -m2 ? m1-2 : -m2;
		for(int i=0;i<6;i++)
			b[i] = mgl_spline5(a+n+n1-2+nx*(m+i), a+n-n2+nx*(m+i), n1, n2, x-n, d[i]);
		res = mgl_spline5(b,b+1

	}*/
	return res;
}
//-----------------------------------------------------------------------------
void mglData::Envelop(char dir)
{
#ifndef NO_GSL
	register int i,j,k,i0;
	double *b = new double[2*nx*ny*nz];
	for(i=0;i<nx*ny*nz;i++)	{	b[2*i] = a[i];	b[2*i+1] = 0;	}
	if(dir=='x' && nx>1)
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
	if(dir=='y' && ny>1)
	{
		gsl_fft_complex_wavetable *wt = gsl_fft_complex_wavetable_alloc(ny);
		gsl_fft_complex_workspace *ws = gsl_fft_complex_workspace_alloc(ny);
		for(i=0;i<nx;i++)	for(j=0;j<nz;j++)
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
	if(dir=='z' && nz>1)
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
#endif
}
//-----------------------------------------------------------------------------
#define omod(x,y)	(y)*((x)>0?int((x)/(y)+0.5):int((x)/(y)-0.5))
void mglData::Sew(const char *dirs, float da)
{
	register int i,j,k,ii;
	register float q;
	if(strchr(dirs,'x') && nx>1)	for(j=0;j<nz*ny;j++)
	{
		ii = j*nx;
		a[1+ii] += omod(a[ii]-a[1+ii], da);
		for(i=2;i<nx;i++)
		{
			q = 2*a[i-1+ii]-a[i-2+ii];
			a[i+ii] += omod(q-a[i+ii], da);
		}
	}
	if(strchr(dirs,'y') && ny>1)	for(j=0;j<nx;j++)	for(k=0;k<nz;k++)
	{
		ii = j+k*nx*ny;
		a[nx+ii] += omod(a[ii]-a[nx+ii], da);
		for(i=2;i<nx;i++)
		{
			q = 2*a[i*nx-nx+ii]-a[i*nx-2*nx+ii];
			a[i*nx+ii] += omod(q-a[i*nx+ii], da);
		}
	}
	if(strchr(dirs,'z') && nz>1)	for(j=0;j<nx*ny;j++)
	{
		ii = nx*ny;
		a[ii+j] += omod(a[j]-a[ii+j], da);
		for(i=2;i<nz;i++)
		{
			q = 2*a[i*ii-ii+j]-a[i*ii-2*ii+j];
			a[i*ii+j] += omod(q-a[i*ii+j], da);
		}
	}
}
//-----------------------------------------------------------------------------
mglData mglData::Evaluate(const mglData &idat, const mglData &jdat, const mglData &kdat, bool norm) const
{
	mglData b;
	register int i,n=idat.nx*idat.ny*idat.nz;
	if(jdat.nx*jdat.ny*jdat.nz!=n || kdat.nx*kdat.ny*kdat.nz!=n)	return b;
	b.Create(idat.nx, idat.ny, idat.nz);
	if(norm)	for(i=0;i<n;i++)	b.a[i] = Spline1(idat.a[i], jdat.a[i], kdat.a[i]);
	else		for(i=0;i<n;i++)	b.a[i] = Spline(idat.a[i], jdat.a[i], kdat.a[i]);
	return b;
}
//-----------------------------------------------------------------------------
mglData mglData::Evaluate(const mglData &idat, const mglData &jdat, bool norm) const
{
	mglData b;
	register int i,n=idat.nx*idat.ny*idat.nz;
	if(jdat.nx*jdat.ny*jdat.nz!=n)	return b;
	b.Create(idat.nx, idat.ny, idat.nz);
	if(norm)	for(i=0;i<n;i++)	b.a[i] = Spline1(idat.a[i], jdat.a[i]);
	else		for(i=0;i<n;i++)	b.a[i] = Spline(idat.a[i], jdat.a[i]);
	return b;
}
//-----------------------------------------------------------------------------
mglData mglData::Evaluate(const mglData &idat, bool norm) const
{
	mglData b;
	register int i,n=idat.nx*idat.ny*idat.nz;
	b.Create(idat.nx, idat.ny, idat.nz);
	if(norm)	for(i=0;i<n;i++)	b.a[i] = Spline1(idat.a[i]);
	else		for(i=0;i<n;i++)	b.a[i] = Spline(idat.a[i]);
	return b;
}
//-----------------------------------------------------------------------------
void mglData::Put(float val, int xx, int yy, int zz)
{
	if(xx>=nx || yy>=ny || zz>=nz)	return;
	register long i,j;
	if(xx<0 && yy<0 && zz<0)	// сам массив
		for(i=0;i<nx*ny*nz;i++)	a[i] = val;
	else if(xx<0 && yy<0)	// 2d
		for(i=0;i<nx*ny;i++)	a[i+zz*nx*ny] = val;
	else if(yy<0 && zz<0)	// 2d
		for(i=0;i<nz*ny;i++)	a[xx+i*nx] = val;
	else if(xx<0 && zz<0)	// 2d
		for(i=0;i<nx;i++)	for(j=0;j<nz;j++)
			a[i+nx*(yy+j*ny)] = val;
	else if(xx<0)
		for(i=0;i<nx;i++)	a[i+nx*(yy+zz*ny)] = val;
	else if(yy<0)
		for(i=0;i<ny;i++)	a[xx+nx*(i+zz*ny)] = val;
	else if(zz<0)
		for(i=0;i<nz;i++)	a[xx+nx*(yy+i*ny)] = val;
	else	a[xx+nx*(yy+zz*ny)] = val;
}
//-----------------------------------------------------------------------------
void mglData::Put(const mglData &val, int xx, int yy, int zz)
{
	if(xx>=nx || yy>=ny || zz>=nz)	return;
	register long i,j,k;
	if(xx<0 && yy<0 && zz<0)	// сам массив
	{
		if(val.nx>=nx && val.ny>=ny && val.nz>=nz)
			for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
				a[i+nx*(j+k*ny)] = val.a[i+val.nx*(j+val.ny*k)];
		else if(val.nx>=nx && val.ny>=ny)
			for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
				a[i+nx*(j+k*ny)] = val.a[i+val.nx*j];
		else if(val.nx>=nx)
			for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
				a[i+nx*(j+k*ny)] = val.a[i];
	}
	else if(xx<0 && yy<0)	// 2d
	{
		if(val.nx>=nx && val.ny>=ny)
			for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
				a[i+nx*(j+zz*ny)] = val.a[i+val.nx*j];
		else if(val.nx>=nx)
			for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
				a[i+nx*(j+zz*ny)] = val.a[i];
	}
	else if(yy<0 && zz<0)	// 2d
	{
		if(val.nx>=ny && val.ny>=nz)
			for(i=0;i<ny;i++)	for(j=0;j<nz;j++)
				a[xx+nx*(i+j*ny)] = val.a[i+val.nx*j];
		else if(val.nx>=ny)
			for(i=0;i<ny;i++)	for(j=0;j<nz;j++)
				a[xx+nx*(i+j*ny)] = val.a[i];
	}
	else if(xx<0 && zz<0)	// 2d
	{
		if(val.nx>=nx && val.ny>=nz)
			for(i=0;i<nx;i++)	for(j=0;j<nz;j++)
				a[i+nx*(yy+j*ny)] = val.a[i+val.nx*j];
		else if(val.nx>=nx)
			for(i=0;i<nx;i++)	for(j=0;j<nz;j++)
				a[i+nx*(yy+j*ny)] = val.a[i];
	}
	else if(xx<0)
	{
		if(val.nx>=nx)	for(i=0;i<nx;i++)
			a[i+nx*(yy+zz*ny)] = val.a[i];
	}
	else if(yy<0)
	{
		if(val.nx>=ny)	for(i=0;i<ny;i++)
			a[xx+nx*(i+zz*ny)] = val.a[i];
	}
	else if(zz<0)
	{
		if(val.nx>=nz)	for(i=0;i<nz;i++)
			a[xx+nx*(yy+i*ny)] = val.a[i];
	}
}
//-----------------------------------------------------------------------------
void mglData::Diff(const mglData &x, const mglData &y)
{
	if(nx<2 || ny<2)	return;
	if(x.nx*x.ny!=nx*ny || y.nx*y.ny!=nx*ny)	return;
	bool same = (x.nz==nz && y.nz==nz);
	float *b = new float[nx*ny*nz],au,av,xu,xv,yu,yv;
	register long i,j,k,i0,i1;
	long kk;
	for(k=0;k<nz;k++)
	{
		kk = same ? 0:-nx*ny*k;
		for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
		{
			i0 = i+nx*(j+ny*k);		i1 = i0 + kk;
			if(i==0)
			{
				au = 3*a[i0]-4*a[i0+1]+a[i0+2];
				xu = 3*x.a[i1]-4*x.a[i1+1]+x.a[i1+2];
				yu = 3*y.a[i1]-4*y.a[i1+1]+y.a[i1+2];
			}
			else if(i==nx-1)
			{
				au = 3*a[i0]-4*a[i0-1]+a[i0-2];
				xu = 3*x.a[i1]-4*x.a[i1-1]+x.a[i1-2];
				yu = 3*y.a[i1]-4*y.a[i1-1]+y.a[i1-2];
			}
			else
			{
				au = a[i0+1]-a[i0-1];
				xu = x.a[i1+1]-x.a[i1-1];
				yu = y.a[i1+1]-y.a[i1-1];
			}
			if(j==0)
			{
				av = 3*a[i0]-4*a[i0+nx]+a[i0+2*nx];
				xv = 3*x.a[i1]-4*x.a[i1+nx]+x.a[i1+2*nx];
				yv = 3*y.a[i1]-4*y.a[i1+nx]+y.a[i1+2*nx];
			}
			else if(j==ny-1)
			{
				av = 3*a[i0]-4*a[i0-nx]+a[i0-2*nx];
				xv = 3*x.a[i1]-4*x.a[i1-nx]+x.a[i1-2*nx];
				yv = 3*y.a[i1]-4*y.a[i1-nx]+y.a[i1-2*nx];
			}
			else
			{
				av = a[i0+nx]-a[i0-nx];
				xv = x.a[i1+nx]-x.a[i1-nx];
				yv = y.a[i1+nx]-y.a[i1-nx];
			}
			b[i0] = (av*yu-au*yv)/(xv*yu-xu*yv);
		}
	}
	delete []a;	a=b;
}
//-----------------------------------------------------------------------------
void mglData::Diff(const mglData &x, const mglData &y, const mglData &z)
{
	if(nx<2 || ny<2)	return;
	if(x.nx*x.ny*x.nz!=nx*ny*nz || y.nx*y.ny*y.nz!=nx*ny*nz || z.nx*z.ny*z.nz!=nx*ny*nz)	return;
	float *b = new float[nx*ny*nz],au,av,aw,xu,xv,xw,yu,yv,yw,zu,zv,zw;
	register long i,j,k,i0,nn=nx*ny;
	for(k=0;k<nz;k++)	for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
	{
		i0 = i+nx*(j+ny*k);
		if(i==0)
		{
			au = 3*a[i0]-4*a[i0+1]+a[i0+2];
			xu = 3*x.a[i0]-4*x.a[i0+1]+x.a[i0+2];
			yu = 3*y.a[i0]-4*y.a[i0+1]+y.a[i0+2];
			zu = 3*z.a[i0]-4*z.a[i0+1]+z.a[i0+2];
		}
		else if(i==nx-1)
		{
			au = 3*a[i0]-4*a[i0-1]+a[i0-2];
			xu = 3*x.a[i0]-4*x.a[i0-1]+x.a[i0-2];
			yu = 3*y.a[i0]-4*y.a[i0-1]+y.a[i0-2];
			zu = 3*z.a[i0]-4*z.a[i0-1]+z.a[i0-2];
		}
		else
		{
			au = a[i0+1]-a[i0-1];
			xu = x.a[i0+1]-x.a[i0-1];
			yu = y.a[i0+1]-y.a[i0-1];
			zu = z.a[i0+1]-z.a[i0-1];
		}
		if(j==0)
		{
			av = 3*a[i0]-4*a[i0+nx]+a[i0+2*nx];
			xv = 3*x.a[i0]-4*x.a[i0+nx]+x.a[i0+2*nx];
			yv = 3*y.a[i0]-4*y.a[i0+nx]+y.a[i0+2*nx];
			zv = 3*z.a[i0]-4*z.a[i0+nx]+z.a[i0+2*nx];
		}
		else if(j==ny-1)
		{
			av = 3*a[i0]-4*a[i0-nx]+a[i0+(ny-3)*nx];
			xv = 3*x.a[i0]-4*x.a[i0-nx]+x.a[i0-2*nx];
			yv = 3*y.a[i0]-4*y.a[i0-nx]+y.a[i0-2*nx];
			zv = 3*z.a[i0]-4*z.a[i0-nx]+z.a[i0-2*nx];
		}
		else
		{
			av = a[i0+nx]-a[i0-nx];
			xv = x.a[i0+nx]-x.a[i0-nx];
			yv = y.a[i0+nx]-y.a[i0-nx];
			zv = z.a[i0+nx]-z.a[i0-nx];
		}
		if(k==0)
		{
			aw = 3*a[i0]-4*a[i0+nn]+a[i0+2*nn];
			xw = 3*x.a[i0]-4*x.a[i0+nn]+x.a[i0+2*nn];
			yw = 3*y.a[i0]-4*y.a[i0+nn]+y.a[i0+2*nn];
			zw = 3*z.a[i0]-4*z.a[i0+nn]+z.a[i0+2*nn];
		}
		else if(k==nz-1)
		{
			aw = 3*a[i0]-4*a[i+(nz-2)*nx*ny]+a[i-2*nn];
			xw = 3*x.a[i0]-4*x.a[i-nn]+x.a[i-2*nn];
			yw = 3*y.a[i0]-4*y.a[i-nn]+y.a[i-2*nn];
			zw = 3*z.a[i0]-4*z.a[i-nn]+z.a[i-2*nn];
		}
		else
		{
			aw = a[i0+nn]-a[i0-nn];
			xw = x.a[i0+nn]-x.a[i0-nn];
			yw = y.a[i0+nn]-y.a[i0-nn];
			zw = z.a[i0+nn]-z.a[i0-nn];
		}
		b[i0] = (au*yv*zw-av*yu*zw-au*yw*zv+aw*yu*zv+av*yw*zu-aw*yv*zu) / (xu*yv*zw-xv*yu*zw-xu*yw*zv+xw*yu*zv+xv*yw*zu-xw*yv*zu);
	}
	delete []a;	a=b;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
