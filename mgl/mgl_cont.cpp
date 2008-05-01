/* mgl_cont.cpp is part of Math Graphic Library
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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "mgl/mgl.h"
#include "mgl/mgl_c.h"
#include "mgl/mgl_f.h"
#ifdef WIN32
#define swprintf    _snwprintf
#endif
//-----------------------------------------------------------------------------
struct _mgl_slice
{
	float *x,*y,*z,*a;
	long nx,ny;
	_mgl_slice()	{	x=y=z=a=NULL;	nx=ny=0;	};
	~_mgl_slice()
	{	if(a)	{	delete []x;	delete []y;	delete []z;	delete []a;	}	};
};
//-----------------------------------------------------------------------------
//
//	Cont series
//
//-----------------------------------------------------------------------------
long mglGraph::add_cpoint(long &pc,float **p,float **k,bool **t,
			float x,float y,float z,float k1,float k2,bool scale)
{
	static long pMax=0;
	if(*p==NULL)
	{
		pMax = 100;	pc = 0;
		*p = (float *)malloc(3*pMax*sizeof(float));
		*k = (float *)malloc(2*pMax*sizeof(float));
		*t = (bool *)malloc(pMax*sizeof(bool));
	}
	if(scale)	t[0][pc] = ScalePoint(x,y,z);
	p[0][3*pc] = x;		p[0][3*pc+1] = y;	p[0][3*pc+2] = z;
	k[0][2*pc] = k1;	k[0][2*pc+1] = k2;
	pc++;
	if(pc>=pMax)
	{
		pMax += 100;
		*p = (float *)realloc(*p,3*pMax*sizeof(float));
		*k = (float *)realloc(*k,2*pMax*sizeof(float));
		*t = (bool *)realloc(*t,pMax*sizeof(bool));
	}
	return pc;
}
//-----------------------------------------------------------------------------
void mglGraph::cont_plot(float val,long n,long m,float *a,
	float *x,float *y,float *z,float zdef,bool axial,bool wire,int text)
{
	float *pp=NULL,*kk=NULL,xx,yy,zz,d,kx,ky;
	bool *tt=NULL;
	long pc=0;
	long *nn,*ff;
	register long i,j,k,i0;

	if(a==NULL || n<2 || m<2)	return;
	if(isnan(zdef))	zdef = Min.z;
	// add intersection point of isoline and Y axis
	for(i=0;i<n-1;i++)	for(j=0;j<m;j++)
	{
		i0 = i+n*j;
		d = _d(val,a[i0],a[i0+1]);
		if(d>=0 && d<1)
		{
			kx = i+d;	ky = j;
			xx = x[i0]*(1.f-d)+x[i0+1]*d;
			yy = y[i0]*(1.f-d)+y[i0+1]*d;
			zz = z ? z[i0]*(1.f-d)+z[i0+1]*d : zdef;
			add_cpoint(pc,&pp,&kk,&tt,xx,yy,zz,kx,ky,!axial);
		}
	}
	// add intersection point of isoline and X axis
	for(i=0;i<n;i++)	for(j=0;j<m-1;j++)
	{
		i0 = i+n*j;
		d = _d(val,a[i0],a[i0+n]);
		if(d>0 && d<1)
		{
			kx = i;		ky = j+d;
			xx = x[i0]*(1.f-d)+x[i0+n]*d;
			yy = y[i0]*(1.f-d)+y[i0+n]*d;
			zz = z ? z[i0]*(1.f-d)+z[i0+n]*d : zdef;
			add_cpoint(pc,&pp,&kk,&tt,xx,yy,zz,kx,ky,!axial);
		}
	}
	// deallocate arrays and finish if no point
	if(pc==0)	{	free(kk);	free(pp);	free(tt);	return;	}
	// allocate arrays for curve
	nn = new long[pc];	ff = new long[pc];
	for(i=0;i<pc;i++)	nn[i] = ff[i] = -1;
	// connect points to line
	long i11,i12,i21,i22,j11,j12,j21,j22;
	j=-1;
	do{
		if(j>=0)
		{
			kx = kk[2*j];	ky = kk[2*j+1];		i = -1;
			i11 = long(kx);	i12 = long(kx-1e-10);
			j11 = long(ky);	j12 = long(ky-1e-10);
			for(k=0;k<pc;k++)	// find closest point in grid
			{
				if(k==j || k==ff[j] || ff[k]!=-1)	continue;	// point is marked
				i21 = long(kk[2*k]);	i22 = long(kk[2*k]-1e-7);
				j21 = long(kk[2*k+1]);	j22 = long(kk[2*k+1]-1e-7);
				register bool cond = (i11==i21 || i11==i22 || i12==i21 || i12==i22) &&
						(j11==j21 || j11==j22 || j12==j21 || j12==j22);
				if(cond){	i=k;	break;	}
			}
			if(i<0)	j = -1;
			else		// mark the point
			{	nn[j] = i;	ff[i] = j;	j = nn[i]<0 ? i : -1;	}
		}
		if(j<0)
		{
			for(k=0;k<pc;k++)	if(nn[k]==-1)
			{
				if(kk[2*k]==0 || kk[2*k]==n-1 || kk[2*k+1]==0 || kk[2*k+1]==m-1)
				{	nn[k]=-2;	j = k;	break;	}
			}
			if(j<0)	for(k=0;k<pc;k++)	if(nn[k]==-1)
			{	j = k;	nn[k]=-2;	break;	}
		}
	}while(j>=0);

	if(text)
	{
		wchar_t wcs[64];
		swprintf(wcs,64,L"%4.3g",val);
		font_curve(pc,pp,tt,nn,wcs,text,-0.5);
	}
	if(axial)	axial_plot(pc,pp,nn,64,wire);
	else 		curv_plot(pc,pp,tt,nn);
	Flush();	free(kk);	free(pp);	free(tt);
	delete []nn;	delete []ff;
}
//-----------------------------------------------------------------------------
void mglGraph::Cont(mglData &v, mglData &x, mglData &y, mglData &z,
					const char *sch, float zVal)
{
	register long i,j,n=z.nx,m=z.ny;
	if(x.nx!=z.nx)		{	SetWarn(mglWarnDim,"Cont");	return;	}
	if(z.nx<2 || z.ny<2){	SetWarn(mglWarnLow,"Cont");	return;	}
	// x, y -- не подходят по размерам
	if(y.nx!=z.ny && (x.ny!=z.ny || y.nx!=z.nx || y.ny!=z.ny))
	{	SetWarn(mglWarnDim);	return;	}

	long text=0;
	if(sch && strchr(sch,'t'))	text=-1;
	if(sch && strchr(sch,'T'))	text=+1;
	SelectPen(sch);
	SetScheme(sch);
	// x, y -- матрицы как и z
	if(x.nx*x.ny==m*n && y.nx*y.ny==m*n)
	{
		for(i=0;i<v.nx;i++)	for(j=0;j<z.nz;j++)
		{
			if(isfinite (zVal) && z.nz>1)
				zVal = Min.z+(Max.z-Min.z)*float(j)/(z.nz-1);
			Color(v.a[i]);
			cont_plot(v.a[i],n,m,z.a+j*m*n,x.a+j*m*n,y.a+j*m*n,
					NULL,isnan(zVal) ? v.a[i] : zVal,false,false,text);
		}
	}
	// x, y -- вектора
	else
	{
		float *xx,*yy;
		xx = new float[n*m];	yy = new float[n*m];
		for(i=0;i<n;i++)	for(j=0;j<m;j++)
		{	xx[i+n*j] = x.a[i];	yy[i+n*j] = y.a[j];	}
		for(i=0;i<v.nx;i++)	for(j=0;j<z.nz;j++)
		{
			if(isfinite (zVal) && z.nz>1)
				zVal = Min.z+(Max.z-Min.z)*float(j)/(z.nz-1);
			Color(v.a[i]);
			cont_plot(v.a[i],n,m,z.a+j*m*n,xx,yy,NULL,
					isnan(zVal) ? v.a[i] : zVal,false,false,text);
		}
		delete []xx;	delete []yy;
	}
	if(sch && strchr(sch,'#'))
	{
		if(isfinite(zVal))	Grid(x,y,z,"k",zVal);
		else				Mesh(x,y,z);
	}
	SetScheme(sch);
}
//-----------------------------------------------------------------------------
void mglGraph::Cont(mglData &v, mglData &z, const char *sch, float zVal)
{
	if(z.nx<2 || z.ny<2)	{	SetWarn(mglWarnLow,"Cont");	return;	}
	mglData x(z.nx), y(z.ny);
	x.Fill(Min.x,Max.x);
	y.Fill(Min.y,Max.y);
	Cont(v,x,y,z,sch,zVal);
}
//-----------------------------------------------------------------------------
void mglGraph::Cont(mglData &x, mglData &y, mglData &z, const char *sch,
					int Num, float zVal)
{
	if(Num<1)	{	SetWarn(mglWarnCnt,"Cont");	return;	}
	mglData v(Num);
	for(long i=0;i<Num;i++)	v.a[i] = Cmin + (Cmax-Cmin)*float(i+1)/(Num+1);
	Cont(v,x,y,z,sch,zVal);
}
//-----------------------------------------------------------------------------
void mglGraph::Cont(mglData &z, const char *sch, int Num, float zVal)
{
	if(Num<1)	{	SetWarn(mglWarnCnt,"Cont");	return;	}
	mglData v(Num);
	for(long i=0;i<Num;i++)	v.a[i] = Cmin + (Cmax-Cmin)*float(i+1)/(Num+1);
	Cont(v,z,sch,zVal);
}
//-----------------------------------------------------------------------------
//
//	ContF series
//
//-----------------------------------------------------------------------------
long mgl_add_quad(float **p, float *pq)
{
	static long Cur=0,Max=0;
	if(*p==0)
	{
		Max = 256;		Cur = 0;
		*p = (float *)malloc(Max*12*sizeof(float));
	}
	if(Cur>=Max)
	{
		Max += 256;
		*p = (float *)realloc(*p,Max*12*sizeof(float));
	}
	memcpy((*p) + 12*Cur, pq, 12*sizeof(float));
	Cur++;
	return Cur;
}
//-----------------------------------------------------------------------------
float mgl_get(long i0,long n,float *x,float px,float py)
{
	return x[i0]*(1+py*px-py-px)+x[i0+n]*py*(1-px)+x[i0+1]*px*(1-py)+x[i0+n+1]*py*px;
}
//-----------------------------------------------------------------------------
void mglGraph::contf_plot(float v1, float v2,long n,long m,float *a,
	float *x,float *y,float *z,float zdef)
{
	float *pp=NULL,ps[18],px[12],py[12],d;
	bool *tt;
	register long i,j,k,i0,l,h=0;
	for(i=0;i<n-1;i++)	for(j=0;j<m-1;j++)
	{
		k=0;
		i0 = i+n*j;
		memset(px,0,12*sizeof(float));	memset(py,0,12*sizeof(float));
		if(a[i0]>= v1 && a[i0]<=v2)			{	px[k]=0;	py[k]=0;	k++;	}
		d = _d(v1,a[i0],a[i0+1]);		if(d>=0 && d<1)	{	px[k]=d;	py[k]=0;	k++;	}
		d = _d(v2,a[i0],a[i0+1]);		if(d>=0 && d<1)	{	px[k]=d;	py[k]=0;	k++;	}
		if(a[i0+1]>= v1 && a[i0+1]<=v2)		{	px[k]=1;	py[k]=0;	k++;	}
		d = _d(v1,a[i0+1],a[i0+1+n]);	if(d>=0 && d<1)	{	px[k]=1;	py[k]=d;	k++;	}
		d = _d(v2,a[i0+1],a[i0+1+n]);	if(d>=0 && d<1)	{	px[k]=1;	py[k]=d;	k++;	}
		if(a[i0+1+n]>= v1 && a[i0+1+n]<=v2)	{	px[k]=1;	py[k]=1;	k++;	}
		d = _d(v1,a[i0+n],a[i0+1+n]);	if(d>=0 && d<1)	{	px[k]=d;	py[k]=1;	k++;	}
		d = _d(v2,a[i0+n],a[i0+1+n]);	if(d>=0 && d<1)	{	px[k]=d;	py[k]=1;	k++;	}
		if(a[i0+n]>= v1 && a[i0+n]<=v2)		{	px[k]=0;	py[k]=1;	k++;	}
		d = _d(v1,a[i0],a[i0+n]);		if(d>=0 && d<1)	{	px[k]=0;	py[k]=d;	k++;	}
		d = _d(v2,a[i0],a[i0+n]);		if(d>=0 && d<1)	{	px[k]=0;	py[k]=d;	k++;	}
		for(l=0;l<6;l++)
		{
			ps[3*l] = mgl_get(i0,n,x,px[l],py[l]);
			ps[3*l+1] = mgl_get(i0,n,y,px[l],py[l]);
			ps[3*l+2] = z!=0?mgl_get(i0,n,z,px[l],py[l]):zdef;
		}
		switch(k)
		{
		case 3:
			memcpy(ps+9,ps+6,3*sizeof(float));
			ps[9] *= 1+1e-4;
			h=mgl_add_quad(&pp,ps);	break;
		case 4:
			h=mgl_add_quad(&pp,ps);	break;
		case 5:
			h=mgl_add_quad(&pp,ps);
			memcpy(ps+3,ps,3*sizeof(float));
			memcpy(ps+6,ps,3*sizeof(float));
			ps[3] = ps[3]*(1+1e-5);
			h=mgl_add_quad(&pp,ps+3);
			break;
		case 6:
			h=mgl_add_quad(&pp,ps);
			memcpy(ps+6,ps,3*sizeof(float));
			h=mgl_add_quad(&pp,ps+6);	break;
		}
	}
	if(h<1)	return;
	tt = new bool[4*h];
	for(i=0;i<h;i++)
	{
		j=12*i;
		tt[4*i] = ScalePoint(pp[j],pp[j+1],pp[j+2]);
		tt[4*i+1] = ScalePoint(pp[j+3],pp[j+4],pp[j+5]);
		tt[4*i+2] = ScalePoint(pp[j+6],pp[j+7],pp[j+8]);
		tt[4*i+3] = ScalePoint(pp[j+9],pp[j+10],pp[j+11]);
	}
	quads_plot(h,pp,0,tt);
	free(pp);	delete []tt;
}
//-----------------------------------------------------------------------------
void mglGraph::ContF(mglData &v, mglData &x, mglData &y, mglData &z,
					const char *sch, float zVal)
{
	register long i,j,n=z.nx,m=z.ny;
	if(x.nx!=z.nx)		{	SetWarn(mglWarnDim,"ContF");	return;	}
	if(z.nx<2 || z.ny<2){	SetWarn(mglWarnLow,"ContF");	return;	}
	// x, y -- не подходят по размерам
	if(y.nx!=z.ny && (x.ny!=z.ny || y.nx!=z.nx || y.ny!=z.ny))
	{	SetWarn(mglWarnDim);	return;	}

	SetScheme(sch);
	// x, y -- матрицы как и z
	if(x.nx*x.ny==m*n && y.nx*y.ny==m*n)
	{
		for(i=0;i<v.nx-1;i++)	for(j=0;j<z.nz;j++)
		{
			if(isfinite(zVal) && z.nz>1)
				zVal = Min.z+(Max.z-Min.z)*float(j)/(z.nz-1);
			Color(v.a[i]);
			contf_plot(v.a[i],v.a[i+1],n,m,z.a+j*m*n,x.a+j*m*n,y.a+j*m*n,
					NULL,isnan(zVal) ? v.a[i] : zVal);
		}
	}
	// x, y -- вектора
	else
	{
		float *xx,*yy;
		xx = new float[n*m];	yy = new float[n*m];
		for(i=0;i<n;i++)	for(j=0;j<m;j++)
		{	xx[i+n*j] = x.a[i];	yy[i+n*j] = y.a[j];	}
		for(i=0;i<v.nx-1;i++)	for(j=0;j<z.nz;j++)
		{
			if(isfinite(zVal) && z.nz>1)
				zVal = Min.z+(Max.z-Min.z)*float(j)/(z.nz-1);
			Color(v.a[i]);
			contf_plot(v.a[i],v.a[i+1],n,m,z.a+j*m*n,xx,yy,NULL,
					isnan(zVal) ? v.a[i] : zVal);
		}
		delete []xx;	delete []yy;
	}
}
//-----------------------------------------------------------------------------
void mglGraph::ContF(mglData &v, mglData &z, const char *sch, float zVal)
{
	if(z.nx<2 || z.ny<2)	{	SetWarn(mglWarnLow,"ContF");	return;	}
	mglData x(z.nx), y(z.ny);
	x.Fill(Min.x,Max.x);
	y.Fill(Min.y,Max.y);
	ContF(v,x,y,z,sch,zVal);
}
//-----------------------------------------------------------------------------
void mglGraph::ContF(mglData &x, mglData &y, mglData &z, const char *sch,
					int Num, float zVal)
{
	if(Num<1)	{	SetWarn(mglWarnCnt,"ContF");	return;	}
	mglData v(Num+2);
	for(long i=0;i<Num+2;i++)	v.a[i] = Cmin + (Cmax-Cmin)*float(i)/(Num+1);
	ContF(v,x,y,z,sch,zVal);
}
//-----------------------------------------------------------------------------
void mglGraph::ContF(mglData &z, const char *sch, int Num, float zVal)
{
	if(Num<1)	{	SetWarn(mglWarnCnt,"ContF");	return;	}
	mglData v(Num+2);
	for(long i=0;i<Num+2;i++)	v.a[i] = Cmin + (Cmax-Cmin)*float(i)/(Num+1);
	ContF(v,z,sch,zVal);
}
//-----------------------------------------------------------------------------
//
//	Axial series
//
//-----------------------------------------------------------------------------
void mglGraph::Axial(mglData &v, mglData &x, mglData &y, mglData &z,
					const char *sch)
{
	register long i,j,k,n=z.nx,m=z.ny;
	if(x.nx!=z.nx)		{	SetWarn(mglWarnDim,"Axial");	return;	}
	if(z.nx<2 || z.ny<2){	SetWarn(mglWarnLow,"Axial");	return;	}
	// x, y -- не подходят по размерам
	if(y.nx!=z.ny && (x.ny!=z.ny || y.nx!=z.nx || y.ny!=z.ny))
	{	SetWarn(mglWarnDim);	return;	}

	SetScheme(sch);
	if(sch)
	{
		if(strchr(sch,'x'))	AxialDir = 'x';
		else if(strchr(sch,'z'))	AxialDir = 'z';
//		else if(strchr(sch,'y'))	AxialDir = 'y';
		else AxialDir = 'y';
	}

	// x, y -- матрицы как и z
	if(x.nx*x.ny==m*n && y.nx*y.ny==m*n)
	{
		for(i=v.nx-1;i>=0;i--)	for(k=0;k<z.nz;k++)
		{
			Color(v.a[i]);
			cont_plot(v.a[i],n,m,z.a,x.a,y.a,NULL,0,true,sch && strchr(sch,'#'),0);
		}
	}
	// x, y -- вектора
	else
	{
		float *xx,*yy;
		xx = new float[n*m];	yy = new float[n*m];
		for(i=0;i<n;i++)	for(j=0;j<m;j++)
		{	xx[i+n*j] = x.a[i];	yy[i+n*j] = y.a[j];	}
		for(i=v.nx-1;i>=0;i--)	for(k=0;k<z.nz;k++)
		{
			Color(v.a[i]);
			cont_plot(v.a[i],n,m,z.a,xx,yy,NULL,0,true,sch && strchr(sch,'#'),0);
		}
		delete []xx;	delete []yy;
	}
}
//-----------------------------------------------------------------------------
void mglGraph::Axial(mglData &v, mglData &z, const char *sch)
{
	if(z.nx<2 || z.ny<2)	{	SetWarn(mglWarnLow,"Axial");	return;	}
	mglData x(z.nx), y(z.ny);
	if(Max.x*Min.x>=0)	x.Fill(Min.x,Max.x);
	else	x.Fill(0,Max.x);
	y.Fill(Min.y,Max.y);
	Axial(v,x,y,z,sch);
}
//-----------------------------------------------------------------------------
void mglGraph::Axial(mglData &x, mglData &y, mglData &z, const char *sch, int Num)
{
	if(Num<1)	{	SetWarn(mglWarnCnt,"Axial");	return;	}
	mglData v(Num);
	for(long i=0;i<Num;i++)	v.a[i] = Cmin + (Cmax-Cmin)*float(i+1)/(Num+1);
	Axial(v,x,y,z,sch);
}
//-----------------------------------------------------------------------------
void mglGraph::Axial(mglData &z, const char *sch, int Num)
{
	if(Num<1)	{	SetWarn(mglWarnCnt,"Axial");	return;	}
	mglData v(Num);
	for(long i=0;i<Num;i++)	v.a[i] = Cmin + (Cmax-Cmin)*float(i+1)/(Num+1);
	Axial(v,z,sch);
}
//-----------------------------------------------------------------------------
//
//	Cont3 series
//
//-----------------------------------------------------------------------------
void get_slice(_mgl_slice &s, mglData &x, mglData &y, mglData &z,
				mglData &a, char dir, long sVal)
{
	register long i,j,k,n=a.nx,m=a.ny,l=a.nz;
	mglData d;
	switch(dir)		// общая часть
	{
	case 'x':
		s.nx = m;	s.ny = l;	if(sVal<0)	sVal = n/2;
		d = a.SubData(sVal,-1,-1);	break;
	case 'y':
		s.nx = n;	s.ny = l;	if(sVal<0)	sVal = m/2;
		d = a.SubData(-1,sVal,-1);	break;
	case 'z':
		s.nx = n;	s.ny = m;	if(sVal<0)	sVal = l/2;
		d = a.SubData(-1,-1,sVal);	break;
	}
	long size = s.nx*s.ny;
	s.x = new float[size];	s.y = new float[size];
	s.z = new float[size];	s.a = new float[size];
	memcpy(s.a,d.a,size*sizeof(float));
	if(x.nx*x.ny*x.nz==n*m*l && y.nx*y.ny*y.nz==n*m*l && z.nx*z.ny*z.nz==n*m*l)
	{
		switch(dir)
		{
		case 'x':
			d = x.SubData(sVal,-1,-1);	memcpy(s.x,d.a,size*sizeof(float));
			d = y.SubData(sVal,-1,-1);	memcpy(s.y,d.a,size*sizeof(float));
			d = z.SubData(sVal,-1,-1);	memcpy(s.z,d.a,size*sizeof(float));
			break;
		case 'y':
			d = x.SubData(-1,sVal,-1);	memcpy(s.x,d.a,size*sizeof(float));
			d = y.SubData(-1,sVal,-1);	memcpy(s.y,d.a,size*sizeof(float));
			d = z.SubData(-1,sVal,-1);	memcpy(s.z,d.a,size*sizeof(float));
			break;
		case 'z':
			d = x.SubData(-1,-1,sVal);	memcpy(s.x,d.a,size*sizeof(float));
			d = y.SubData(-1,-1,sVal);	memcpy(s.y,d.a,size*sizeof(float));
			d = z.SubData(-1,-1,sVal);	memcpy(s.z,d.a,size*sizeof(float));
			break;
		}
	}
	else if(x.nx==n && y.nx==m && z.nx==l)	// x, y -- вектора
	{
		float ff;
		switch(dir)
		{
		case 'x':
			ff = x.a[sVal];
			for(i=0;i<m;i++)	for(j=0;j<l;j++)
			{	k = i+m*j;	s.x[k] = ff;	s.y[k] = y.a[i];	s.z[k] = z.a[j];	}
			break;
		case 'y':
			ff = y.a[sVal];
			for(i=0;i<n;i++)	for(j=0;j<l;j++)
			{	k = i+n*j;	s.y[k] = ff;	s.z[k] = z.a[j];	s.x[k] = x.a[i];	}
			break;
		case 'z':
			ff = z.a[sVal];
			for(i=0;i<n;i++)	for(j=0;j<m;j++)
			{	k = i+n*j;	s.z[k] = ff;	s.x[k] = x.a[i];	s.y[k] = y.a[j];	}
			break;
		}
	}
}
//-----------------------------------------------------------------------------
void get_slice(_mgl_slice &s, mglData &a, char dir, long sVal,
			mglPoint Min, mglPoint Max)
{
	register long i,j,k,n=a.nx,m=a.ny,l=a.nz;
	float x1,y1,z1;
	mglData d;

	switch(dir)		// общая часть
	{
	case 'x':
		s.nx = m;	s.ny = l;	if(sVal<0)	sVal = n/2;
		d = a.SubData(sVal,-1,-1);	break;
	case 'y':
		s.nx = n;	s.ny = l;	if(sVal<0)	sVal = m/2;
		d = a.SubData(-1,sVal,-1);	break;
	case 'z':
		s.nx = n;	s.ny = m;	if(sVal<0)	sVal = l/2;
		d = a.SubData(-1,-1,sVal);	break;
	}
	long size = s.nx*s.ny;
	s.x = new float[size];	s.y = new float[size];
	s.z = new float[size];	s.a = new float[size];
	memcpy(s.a,d.a,size*sizeof(float));
	x1 = Min.x + (Max.x-Min.x)*float(sVal)/(n-1.);
	y1 = Min.y + (Max.y-Min.y)*float(sVal)/(m-1.);
	z1 = Min.z + (Max.z-Min.z)*float(sVal)/(l-1.);

	switch(dir)
	{
	case 'x':
		for(i=0;i<m;i++)	for(j=0;j<l;j++)
		{
			k = i+m*j;		s.y[k] = Min.y + (Max.y-Min.y)*i/(m-1.);
			s.x[k] = x1;	s.z[k] = Min.z + (Max.z-Min.z)*j/(l-1.);
		}
		break;
	case 'y':
		for(i=0;i<n;i++)	for(j=0;j<l;j++)
		{
			k = i+n*j;		s.x[k] = Min.x + (Max.x-Min.x)*i/(n-1.);
			s.y[k] = y1;	s.z[k] = Min.z + (Max.z-Min.z)*j/(l-1.);
		}
		break;
	case 'z':
		for(i=0;i<n;i++)	for(j=0;j<m;j++)
		{
			k = i+n*j;		s.y[k] = Min.y + (Max.y-Min.y)*j/(m-1.);
			s.z[k] = z1;	s.x[k] = Min.x + (Max.x-Min.x)*i/(n-1.);
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void mglGraph::Cont3(mglData &v, mglData &x, mglData &y, mglData &z, mglData &a,
					char dir, int sVal, const char *sch)
{
	register long i;
	_mgl_slice s;

	long n=a.nx,m=a.ny,l=a.nz;
	if(n<2 || m<2 || l<2)	{	SetWarn(mglWarnLow,"Cont3");	return;	}
	bool both = x.nx*x.ny*x.nz==n*m*l && y.nx*y.ny*y.nz==n*m*l && z.nx*z.ny*z.nz==n*m*l;
	if(!(both || (x.nx==n && y.nx==m && z.nx==l)))
	{	SetWarn(mglWarnDim,"Cont3");	return;	}

	long text=0;
	if(sch && strchr(sch,'t'))	text=-1;
	if(sch && strchr(sch,'T'))	text=+1;
	SelectPen(sch);
	SetScheme(sch);
	get_slice(s,x,y,z,a,dir,sVal);	// готовим память
	if(s.a==NULL)	return;
	for(i=0;i<v.nx;i++)	// строим контуры
	{
		Color(v.a[i]);
		cont_plot(v.a[i],s.nx,s.ny,s.a,s.x,s.y,s.z,0,false,false,text);
	}
	if(sch && strchr(sch,'#'))	Grid3(x,y,z,a,dir,sVal,"k");
	SetScheme(sch);
}
//-----------------------------------------------------------------------------
void mglGraph::Cont3(mglData &v, mglData &a, char dir, int sVal, const char *sch)
{
	if(a.nx<2 || a.ny<2 || a.nz<2)
	{	SetWarn(mglWarnLow,"Cont3");	return;	}
	mglData x(a.nx), y(a.ny),z(a.nz);
	x.Fill(Min.x,Max.x);
	y.Fill(Min.y,Max.y);
	z.Fill(Min.z,Max.z);
	Cont3(v,x,y,z,a,dir,sVal,sch);
}
//-----------------------------------------------------------------------------
void mglGraph::Cont3(mglData &x, mglData &y, mglData &z, mglData &a,
					char dir, int sVal, const char *sch, int Num)
{
	if(Num<1)	{	SetWarn(mglWarnCnt,"Cont3");	return;	}
	mglData v(Num);
	for(long i=0;i<Num;i++)	v.a[i] = Cmin + (Cmax-Cmin)*float(i+1)/(Num+1);
	Cont3(v,x,y,z,a,dir,sVal,sch);
}
//-----------------------------------------------------------------------------
void mglGraph::Cont3(mglData &a, char dir, int sVal, const char *sch, int Num)
{
	if(Num<1)	{	SetWarn(mglWarnCnt,"Cont3");	return;	}
	mglData v(Num);
	for(long i=0;i<Num;i++)	v.a[i] = Cmin + (Cmax-Cmin)*float(i+1)/(Num+1);
	Cont3(v,a,dir,sVal,sch);
}
//-----------------------------------------------------------------------------
//
//	Dens3 series
//
//-----------------------------------------------------------------------------
void mglGraph::Dens3(mglData &x, mglData &y, mglData &z, mglData &a,
					char dir, int sVal, const char *sch)
{
	register long i,j,i0;
	_mgl_slice s;
	long n=a.nx,m=a.ny,l=a.nz;
	if(n<2 || m<2 || l<2)	{	SetWarn(mglWarnLow,"Dens3");	return;	}
	bool both = x.nx*x.ny*x.nz==n*m*l && y.nx*y.ny*y.nz==n*m*l && z.nx*z.ny*z.nz==n*m*l;
	if(!(both || (x.nx==n && y.nx==m && z.nx==l)))
	{	SetWarn(mglWarnDim,"Dens3");	return;	}

	SetScheme(sch);
	get_slice(s,x,y,z,a,dir,sVal);	// готовим память
	if(s.a==NULL)	return;
	mglColor col;
	float *pp = new float[3*s.nx*s.ny], *cc = new float[4*s.nx*s.ny];
	bool *tt = new bool[s.nx*s.ny];
	for(i=0;i<s.nx;i++)	for(j=0;j<s.ny;j++)	// создаем массив точек
	{
		i0 = i+s.nx*j;
		pp[3*i0+0] = s.x[i0];
		pp[3*i0+1] = s.y[i0];
		pp[3*i0+2] = s.z[i0];
		col = GetC(s.a[i0]);
		cc[4*i0+0] = col.r;	cc[4*i0+1] = col.g;
		cc[4*i0+2] = col.b;	cc[4*i0+3] = Transparent ? AlphaDef:1;
		tt[i0] = ScalePoint(pp[3*i0+0],pp[3*i0+1],pp[3*i0+2]);
	}
	surf_plot(s.nx, s.ny, pp, cc, tt);
	Flush();
	delete []pp;	delete []tt;	delete []cc;
	if(sch && strchr(sch,'#'))	Grid3(x,y,z,a,dir,sVal,"k");
	SetScheme(sch);
}
//-----------------------------------------------------------------------------
void mglGraph::Dens3(mglData &a, char dir, int sVal, const char *sch)
{
	if(a.nx<2 || a.ny<2 || a.nz<2)
	{	SetWarn(mglWarnLow,"Dens3");	return;	}
	mglData x(a.nx), y(a.ny),z(a.nz);
	x.Fill(Min.x,Max.x);
	y.Fill(Min.y,Max.y);
	z.Fill(Min.z,Max.z);
	Dens3(x,y,z,a,dir,sVal,sch);
}
//-----------------------------------------------------------------------------
//
//	Grid3 series
//
//-----------------------------------------------------------------------------
void mglGraph::Grid3(mglData &x, mglData &y, mglData &z, mglData &a,
					char dir, int sVal, const char *sch)
{
	register long i,j,i0,i1;
	_mgl_slice s;
	long n=a.nx,m=a.ny,l=a.nz;
	if(n<2 || m<2 || l<2)	{	SetWarn(mglWarnLow,"Grid3");	return;	}
	bool both = x.nx*x.ny*x.nz==n*m*l && y.nx*y.ny*y.nz==n*m*l && z.nx*z.ny*z.nz==n*m*l;
	if(!(both || (x.nx==n && y.nx==m && z.nx==l)))
	{	SetWarn(mglWarnDim,"Grid3");	return;	}

	bool sm = dir < 'x';
	if(sm)	dir += 'x'-'X';
	SelectPen(sch);
	get_slice(s,x,y,z,a,dir,sVal);	// готовим память
	if(s.a==NULL)	return;
	mglColor col;
	float *pp = new float[3*s.nx*s.ny];
	bool *tt = new bool[s.nx*s.ny];
	if(sm && s.nx>15 && s.ny>15)
	{
		for(i=0;i<10;i++)	for(j=0;j<10;j++)	// создаем массив точек
		{
			i0 = ((s.nx-1)*i/9)+s.nx*((s.ny-1)*j/9);
			i1 = i+10*j;
			pp[3*i1+0] = s.x[i0];
			pp[3*i1+1] = s.y[i0];
			pp[3*i1+2] = s.z[i0];
			tt[i1] = ScalePoint(pp[3*i1+0],pp[3*i1+1],pp[3*i1+2]);
		}
		s.nx = s.ny = 10;
	}
	else
	{
		for(i=0;i<s.nx;i++)	for(j=0;j<s.ny;j++)	// создаем массив точек
		{
			i0 = i+s.nx*j;
			pp[3*i0+0] = s.x[i0];
			pp[3*i0+1] = s.y[i0];
			pp[3*i0+2] = s.z[i0];
			tt[i0] = ScalePoint(pp[3*i0+0],pp[3*i0+1],pp[3*i0+2]);
		}
	}
	mesh_plot(s.nx, s.ny, pp, NULL, tt,3);
	Flush();
	delete []pp;	delete []tt;
}
//-----------------------------------------------------------------------------
void mglGraph::Grid3(mglData &a, char dir, int sVal, const char *sch)
{
	if(a.nx<2 || a.ny<2 || a.nz<2)
	{	SetWarn(mglWarnLow,"Grid3");	return;	}
	mglData x(a.nx), y(a.ny), z(a.nz);
	x.Fill(Min.x,Max.x);
	y.Fill(Min.y,Max.y);
	z.Fill(Min.z,Max.z);
	Grid3(x,y,z,a,dir,sVal,sch);
}
//-----------------------------------------------------------------------------
//
//	Text printing along some curve
//
//-----------------------------------------------------------------------------
void mglGraph::ContF3(mglData &v, mglData &x, mglData &y, mglData &z, mglData &a,
					char dir, int sVal, const char *sch)
{
	register long i;
	_mgl_slice s;
	long n=a.nx,m=a.ny,l=a.nz;
	if(n<2 || m<2 || l<2)	{	SetWarn(mglWarnLow,"ContF3");	return;	}
	bool both = x.nx*x.ny*x.nz==n*m*l && y.nx*y.ny*y.nz==n*m*l && z.nx*z.ny*z.nz==n*m*l;
	if(!(both || (x.nx==n && y.nx==m && z.nx==l)))
	{	SetWarn(mglWarnDim,"ContF3");	return;	}

	SetScheme(sch);
	get_slice(s,x,y,z,a,dir,sVal);	// готовим память
	if(s.a==NULL)	return;
	for(i=0;i<v.nx-1;i++)	// строим контуры
	{
		Color(v.a[i]);
		contf_plot(v.a[i],v.a[i+1],s.nx,s.ny,s.a,s.x,s.y,s.z,0);
	}
}
//-----------------------------------------------------------------------------
void mglGraph::ContF3(mglData &v, mglData &a, char dir, int sVal, const char *sch)
{
	if(a.nx<2 || a.ny<2 || a.nz<2)
	{	SetWarn(mglWarnLow,"ContF3");	return;	}
	mglData x(a.nx), y(a.ny),z(a.nz);
	x.Fill(Min.x,Max.x);
	y.Fill(Min.y,Max.y);
	z.Fill(Min.z,Max.z);
	ContF3(v,x,y,z,a,dir,sVal,sch);
}
//-----------------------------------------------------------------------------
void mglGraph::ContF3(mglData &x, mglData &y, mglData &z, mglData &a,
					char dir, int sVal, const char *sch, int Num)
{
	if(Num<1)	{	SetWarn(mglWarnCnt,"ContF3");	return;	}
	mglData v(Num+2);
	for(long i=0;i<Num+2;i++)	v.a[i] = Cmin + (Cmax-Cmin)*float(i)/(Num+1);
	ContF3(v,x,y,z,a,dir,sVal,sch);
}
//-----------------------------------------------------------------------------
void mglGraph::ContF3(mglData &a, char dir, int sVal, const char *sch, int Num)
{
	if(Num<1)	{	SetWarn(mglWarnCnt,"ContF3");	return;	}
	mglData v(Num+2);
	for(long i=0;i<Num+2;i++)	v.a[i] = Cmin + (Cmax-Cmin)*float(i)/(Num+1);
	ContF3(v,a,dir,sVal,sch);
}
//-----------------------------------------------------------------------------
//
//	Text printing along some curve
//
//-----------------------------------------------------------------------------
void mglGraph::Text(mglData &y,const wchar_t *text,const char *font,float size,float zVal)
{
	if(y.nx<2)	{	SetWarn(mglWarnLow,"Text");	return;	}
	mglData x(y.nx);
	x.Fill(Min.x,Max.x);
	Text(x,y,text,font,size,zVal);
}
//-----------------------------------------------------------------------------
void mglGraph::Text(mglData &x,mglData &y,const wchar_t *text,const char *font,float size,float zVal)
{
	mglData z(y.nx);
	if(isnan(zVal))	zVal = Min.z;
	z.Fill(zVal,zVal);
	Text(x,y,z,text,font,size);
}
//-----------------------------------------------------------------------------
void mglGraph::Text(mglData &x,mglData &y,mglData &z,const wchar_t *text,const char *font,float size)
{
	long n=y.nx;
	if(x.nx!=n || z.nx!=n)	{	SetWarn(mglWarnDim,"Text");	return;	}
	if(n<2)					{	SetWarn(mglWarnLow,"Text");	return;	}

	char col=TranspType!=2 ? 'k':'w', stl[3]="-k";
	const char *f = strchr(font,':');	if(f)	col=f[1];
	stl[1] = col;
	SelectPen(stl);		// May be I should use "-0" ?????

	float *pp = new float[3*n];
	bool *tt = new bool[n];
	long *nn = new long[n];
	register long i,k;
	for(i=0;i<n;i++)
	{
		k = 3*i;	nn[i]=i+1;	pp[k+0] = x.a[i];
		pp[k+1] = y.a[i];		pp[k+2] = z.a[i];
		tt[i] = ScalePoint(pp[k],pp[k+1],pp[k+2]);
	}
	nn[n-1]=-1;
	font_curve(n,pp,tt,nn,text,strchr(font,'T')?1:-1,size);
	delete []tt;	delete []pp;	delete []nn;
}
//-----------------------------------------------------------------------------
void mglGraph::GridA(mglData &x, mglData &y, mglData &z, mglData &a,
					const char *stl)
{
	Grid3(x,y,z,a,'X',-1,stl);
	Grid3(x,y,z,a,'Y',-1,stl);
	Grid3(x,y,z,a,'Z',-1,stl);
}
//-----------------------------------------------------------------------------
void mglGraph::GridA(mglData &a, const char *stl)
{
	Grid3(a,'X',-1,stl);
	Grid3(a,'Y',-1,stl);
	Grid3(a,'Z',-1,stl);
}
//-----------------------------------------------------------------------------
void mglGraph::DensA(mglData &x, mglData &y, mglData &z, mglData &a, const char *stl)
{
	Dens3(x,y,z,a,'x',-1,stl);
	Dens3(x,y,z,a,'y',-1,stl);
	Dens3(x,y,z,a,'z',-1,stl);
}
//-----------------------------------------------------------------------------
void mglGraph::DensA(mglData &a, const char *stl)
{
	Dens3(a,'x',-1,stl);
	Dens3(a,'y',-1,stl);
	Dens3(a,'z',-1,stl);
}
//-----------------------------------------------------------------------------
void mglGraph::ContA(mglData &x, mglData &y, mglData &z, mglData &a, const char *stl, int Num)
{
	Cont3(x,y,z,a,'x',-1,stl,Num);
	Cont3(x,y,z,a,'y',-1,stl,Num);
	Cont3(x,y,z,a,'z',-1,stl,Num);
}
//-----------------------------------------------------------------------------
void mglGraph::ContA(mglData &a, const char *stl, int Num)
{
	Cont3(a,'x',-1,stl,Num);
	Cont3(a,'y',-1,stl,Num);
	Cont3(a,'z',-1,stl,Num);
}
//-----------------------------------------------------------------------------
void mglGraph::ContFA(mglData &x, mglData &y, mglData &z, mglData &a, const char *stl, int Num)
{
	ContF3(x,y,z,a,'x',-1,stl,Num);
	ContF3(x,y,z,a,'y',-1,stl,Num);
	ContF3(x,y,z,a,'z',-1,stl,Num);
}
//-----------------------------------------------------------------------------
void mglGraph::ContFA(mglData &a, const char *stl, int Num)
{
	ContF3(a,'x',-1,stl,Num);
	ContF3(a,'y',-1,stl,Num);
	ContF3(a,'z',-1,stl,Num);
}
//-----------------------------------------------------------------------------
/// Draw contour lines at slice for 3d data specified parametrically
void mgl_cont3_xyz_val(HMGL gr, HMDT v, HMDT x, HMDT y, HMDT z, HMDT a,
			char dir, int sVal, const char *sch)
{	if(gr && a && x && y && z && v)	gr->Cont3(*v, *x, *y, *z, *a, dir, sVal, sch);	}
/// Draw contour lines at slice for 3d data
void mgl_cont3_val(HMGL gr, HMDT v, HMDT a, char dir, int sVal, const char *sch)
{	if(gr && a && v)	gr->Cont3(*v, *a, dir, sVal, sch);	}
/// Draw several contour lines at slice for 3d data specified parametrically
void mgl_cont3_xyz(HMGL gr, HMDT x, HMDT y, HMDT z, HMDT a,
			char dir, int sVal, const char *sch, int Num)
{	if(gr && a && x && y && z)	gr->Cont3(*x, *y, *z, *a, dir, sVal, sch, Num);	}
/// Draw several contour lines at slice for 3d data
void mgl_cont3(HMGL gr, HMDT a, char dir, int sVal, const char *sch, int Num)
{	if(gr && a)	gr->Cont3(*a, dir, sVal, sch, Num);	}
/// Draw contour lines at central slices for 3d data specified parametrically
void mgl_cont_all_xyz(HMGL gr, HMDT x, HMDT y, HMDT z, HMDT a,
			const char *sch, int Num)
{	if(gr && a && x && y && z)	gr->ContA(*x, *y, *z, *a, sch, Num);	}
/// Draw contour lines at central slices for 3d data
void mgl_cont_all(HMGL gr, HMDT a, const char *sch, int Num)
{	if(gr && a)	gr->ContA(*a, sch, Num);	}
/// Draw contour lines for 2d data specified parametrically
void mgl_cont_xy_val(HMGL gr, HMDT v, HMDT x, HMDT y, HMDT a, const char *sch, float zVal)
{	if(gr && a && x && y && v)	gr->Cont(*v, *x, *y, *a, sch, zVal);	}
/// Draw contour lines for 2d data
void mgl_cont_val(HMGL gr, HMDT v, HMDT a, const char *sch,float zVal)
{	if(gr && a && v)	gr->Cont(*v, *a, sch, zVal);	}
/// Draw several contour lines for 2d data specified parametrically
void mgl_cont_xy(HMGL gr, HMDT x, HMDT y, HMDT a, const char *sch, int Num, float zVal)
{	if(gr && a && x && y)	gr->Cont(*x, *y, *a, sch, Num, zVal);	}
/// Draw several contour lines for 2d data
void mgl_cont(HMGL gr, HMDT a, const char *sch, int Num, float zVal)
{	if(gr && a)	gr->Cont(*a, sch, Num, zVal);	}
/// Draw grid lines for density plot at slice for 3d data specified parametrically
void mgl_grid3_xyz(HMGL gr, HMDT x, HMDT y, HMDT z, HMDT a, char dir, int sVal, const char *sch)
{	if(gr && a && x && y && z)	gr->Grid3(*x, *y, *z, *a, dir, sVal, sch);	}
/// Draw grid lines for density plot at slice for 3d data
void mgl_grid3(HMGL gr, HMDT a, char dir, int sVal, const char *sch)
{	if(gr && a)	gr->Grid3(*a, dir, sVal, sch);	}
/// Draw grid lines for density plot at central slices for 3d data specified parametrically
void mgl_grid3_all_xyz(HMGL gr, HMDT x, HMDT y, HMDT z, HMDT a, const char *sch)
{	if(gr && a && x && y && z)	gr->GridA(*x, *y, *z, *a, sch);	}
/// Draw grid lines for density plot at central slices for 3d data
void mgl_grid3_all(HMGL gr, HMDT a, const char *sch)
{	if(gr && a)	gr->GridA(*a, sch);	}
/// Draw density plot at slice for 3d data specified parametrically
void mgl_dens3_xyz(HMGL gr, HMDT x, HMDT y, HMDT z, HMDT a, char dir, int sVal, const char *sch)
{	if(gr && a && x && y && z)	gr->Dens3(*x, *y, *z, *a, dir, sVal, sch);	}
/// Draw density plot at slice for 3d data
void mgl_dens3(HMGL gr, HMDT a, char dir, int sVal, const char *sch)
{	if(gr && a)	gr->Dens3(*a, dir, sVal, sch);	}
/// Draw density plot at central slices for 3d data specified parametrically
void mgl_dens3_all_xyz(HMGL gr, HMDT x, HMDT y, HMDT z, HMDT a, const char *sch)
{	if(gr && a && x && y && z)	gr->DensA(*x, *y, *z, *a, sch);	}
/// Draw density plot at central slices for 3d data
void mgl_dens3_all(HMGL gr, HMDT a, const char *sch)
{	if(gr && a)	gr->DensA(*a, sch);	}
/// Draw axial-symmetric isosurfaces for 2d data specified parametrically
void mgl_axial_xy_val(HMGL gr, HMDT v, HMDT x, HMDT y, HMDT a, const char *sch)
{	if(gr && a && x && y && v)	gr->Axial(*v, *x, *y, *a, sch);	}
/// Draw axial-symmetric isosurfaces for 2d data
void mgl_axial_val(HMGL gr, HMDT v, HMDT a, const char *sch)
{	if(gr && a && v)	gr->Axial(*v, *a, sch);	}
/// Draw several axial-symmetric isosurfaces for 2d data specified parametrically
void mgl_axial_xy(HMGL gr, HMDT x, HMDT y, HMDT a, const char *sch, int Num)
{	if(gr && a && x && y)	gr->Axial(*x, *y, *a, sch, Num);	}
/// Draw several axial-symmetric isosurfaces for 2d data
void mgl_axial(HMGL gr, HMDT a, const char *sch, int Num)
{	if(gr && a)	gr->Axial(*a, sch, Num);	}
//-----------------------------------------------------------------------------
/// Draw solid contours at slice for 3d data specified parametrically
void mgl_contf3_xyz_val(HMGL gr, HMDT v, HMDT x, HMDT y, HMDT z, HMDT a,
			char dir, int sVal, const char *sch)
{	if(gr && a && x && y && z && v)	gr->ContF3(*v, *x, *y, *z, *a, dir, sVal, sch);	}
/// Draw solid contours at slice for 3d data
void mgl_contf3_val(HMGL gr, HMDT v, HMDT a, char dir, int sVal, const char *sch)
{	if(gr && a && v)	gr->ContF3(*v, *a, dir, sVal, sch);	}
/// Draw several solid contours at slice for 3d data specified parametrically
void mgl_contf3_xyz(HMGL gr, HMDT x, HMDT y, HMDT z, HMDT a,
			char dir, int sVal, const char *sch, int Num)
{	if(gr && a && x && y && z)	gr->ContF3(*x, *y, *z, *a, dir, sVal, sch, Num);	}
/// Draw several solid contours at slice for 3d data
void mgl_contf3(HMGL gr, HMDT a, char dir, int sVal, const char *sch, int Num)
{	if(gr && a)	gr->ContF3(*a, dir, sVal, sch, Num);	}
/// Draw solid contours at central slices for 3d data specified parametrically
void mgl_contf_all_xyz(HMGL gr, HMDT x, HMDT y, HMDT z, HMDT a,
			const char *sch, int Num)
{	if(gr && a && x && y && z)	gr->ContFA(*x, *y, *z, *a, sch, Num);	}
/// Draw solid contours at central slices for 3d data
void mgl_contf_all(HMGL gr, HMDT a, const char *sch, int Num)
{	if(gr && a)	gr->ContFA(*a, sch, Num);	}
/// Draw solid contours for 2d data specified parametrically
void mgl_contf_xy_val(HMGL gr, HMDT v, HMDT x, HMDT y, HMDT a, const char *sch, float zVal)
{	if(gr && a && x && y && v)	gr->ContF(*v, *x, *y, *a, sch, zVal);	}
/// Draw solid contours for 2d data
void mgl_contf_val(HMGL gr, HMDT v, HMDT a, const char *sch,float zVal)
{	if(gr && a && v)	gr->ContF(*v, *a, sch, zVal);	}
/// Draw several solid contours for 2d data specified parametrically
void mgl_contf_xy(HMGL gr, HMDT x, HMDT y, HMDT a, const char *sch, int Num, float zVal)
{	if(gr && a && x && y)	gr->ContF(*x, *y, *a, sch, Num, zVal);	}
/// Draw several solid contours for 2d data
void mgl_contf(HMGL gr, HMDT a, const char *sch, int Num, float zVal)
{	if(gr && a)	gr->ContF(*a, sch, Num, zVal);	}
/// Print string \a text along curve in 3D with font size \a size.
void mgl_text_xyz(HMGL gr, HMDT x, HMDT y, HMDT z,const char *text,
				const char *font, float size)
{	if(gr && x && y && z)	gr->Text(*x,*y,*z,text,font,size);	}
/// Print string \a text along parametrical curve with font size \a size.
void mgl_text_xy(HMGL gr, HMDT x, HMDT y, const char *text, const char *font, float size)
{	if(gr && x && y)	gr->Text(*x,*y,text,font,size);	}
/// Print string \a text along curve with font size \a size.
void mgl_text_y(HMGL gr, HMDT y, const char *text, const char *font, float size)
{	if(gr && y)	gr->Text(*y,text,font,size);	}
//-----------------------------------------------------------------------------
//	Fortran interface
//-----------------------------------------------------------------------------
/// Draw contour lines at slice for 3d data specified parametrically
void mgl_cont3_xyz_val_(long *gr, long *v, long *x, long *y, long *z, long *a, const char *dir,
						int *sVal, const char *sch,int,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && x && y && z && v)	_GR_->Cont3(_D_(v), _D_(x), _D_(y), _D_(z), _D_(a), *dir, *sVal, s);
	delete []s;
}
/// Draw contour lines at slice for 3d data
void mgl_cont3_val_(long *gr, long *v, long *a, const char *dir, int *sVal, const char *sch,int,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && v)	_GR_->Cont3(_D_(v), _D_(a), *dir, *sVal, s);
	delete []s;
}
/// Draw several contour lines at slice for 3d data specified parametrically
void mgl_cont3_xyz_(long *gr, long *x, long *y, long *z, long *a, const char *dir, int *sVal,
					const char *sch, int *Num,int,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && x && y && z)	_GR_->Cont3(_D_(x), _D_(y), _D_(z), _D_(a), *dir, *sVal, s, *Num);
	delete []s;
}
/// Draw several contour lines at slice for 3d data
void mgl_cont3_(long *gr, long *a, const char *dir, int *sVal, const char *sch, int *Num,int,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a)	_GR_->Cont3(_D_(a), *dir, *sVal, s, *Num);
	delete []s;
}
/// Draw contour lines at central slices for 3d data specified parametrically
void mgl_cont_all_xyz_(long *gr, long *x, long *y, long *z, long *a, const char *sch, int *Num,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && x && y && z)	_GR_->ContA(_D_(x), _D_(y), _D_(z), _D_(a), s, *Num);
	delete []s;
}
/// Draw contour lines at central slices for 3d data
void mgl_cont_all_(long *gr, long *a, const char *sch, int *Num,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a)	_GR_->ContA(_D_(a), s, *Num);
	delete []s;
}
/// Draw contour lines for 2d data specified parametrically
void mgl_cont_xy_val_(long *gr, long *v, long *x, long *y, long *a, const char *sch, float *zVal,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && x && y && v)	_GR_->Cont(_D_(v), _D_(x), _D_(y), _D_(a), s, *zVal);
	delete []s;
}
/// Draw contour lines for 2d data
void mgl_cont_val_(long *gr, long *v, long *a, const char *sch,float *zVal,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && v)	_GR_->Cont(_D_(v), _D_(a), s, *zVal);
	delete []s;
}
/// Draw several contour lines for 2d data specified parametrically
void mgl_cont_xy_(long *gr, long *x, long *y, long *a, const char *sch, int *Num, float *zVal,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && x && y)	_GR_->Cont(_D_(x), _D_(y), _D_(a), s, *Num, *zVal);
	delete []s;
}
/// Draw several contour lines for 2d data
void mgl_cont_(long *gr, long *a, const char *sch, int *Num, float *zVal,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a)	_GR_->Cont(_D_(a), s, *Num, *zVal);
	delete []s;
}
/// Draw grid lines for density plot at slice for 3d data specified parametrically
void mgl_grid3_xyz_(long *gr, long *x, long *y, long *z, long *a, const char *dir, int *sVal, const char *sch,int,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && x && y && z)	_GR_->Grid3(_D_(x), _D_(y), _D_(z), _D_(a), *dir, *sVal, s);
	delete []s;
}
/// Draw grid lines for density plot at slice for 3d data
void mgl_grid3_(long *gr, long *a, const char *dir, int *sVal, const char *sch,int,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a)	_GR_->Grid3(_D_(a), *dir, *sVal, s);
	delete []s;
}
/// Draw grid lines for density plot at central slices for 3d data specified parametrically
void mgl_grid3_all_xyz_(long *gr, long *x, long *y, long *z, long *a, const char *sch,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && x && y && z)	_GR_->GridA(_D_(x), _D_(y), _D_(z), _D_(a), s);
	delete []s;
}
/// Draw grid lines for density plot at central slices for 3d data
void mgl_grid3_all_(long *gr, long *a, const char *sch,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a)	_GR_->GridA(_D_(a), s);
	delete []s;
}
/// Draw density plot at slice for 3d data specified parametrically
void mgl_dens3_xyz_(long *gr, long *x, long *y, long *z, long *a, const char *dir, int *sVal, const char *sch,int,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && x && y && z)	_GR_->Dens3(_D_(x), _D_(y), _D_(z), _D_(a), *dir, *sVal, s);
	delete []s;
}
/// Draw density plot at slice for 3d data
void mgl_dens3_(long *gr, long *a, const char *dir, int *sVal, const char *sch,int,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a)	_GR_->Dens3(_D_(a), *dir, *sVal, s);
	delete []s;
}
/// Draw density plot at central slices for 3d data specified parametrically
void mgl_dens3_all_xyz_(long *gr, long *x, long *y, long *z, long *a, const char *sch,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && x && y && z)	_GR_->DensA(_D_(x), _D_(y), _D_(z), _D_(a), s);
	delete []s;
}
/// Draw density plot at central slices for 3d data
void mgl_dens3_all_(long *gr, long *a, const char *sch,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a)	_GR_->DensA(_D_(a), s);
	delete []s;
}
/// Draw axial-symmetric isosurfaces for 2d data specified parametrically
void mgl_axial_xy_val_(long *gr, long *v, long *x, long *y, long *a, const char *sch,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && x && y && v)	_GR_->Axial(_D_(v), _D_(x), _D_(y), _D_(a), s);
	delete []s;
}
/// Draw axial-symmetric isosurfaces for 2d data
void mgl_axial_val_(long *gr, long *v, long *a, const char *sch,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && v)	_GR_->Axial(_D_(v), _D_(a), s);
	delete []s;
}
/// Draw several axial-symmetric isosurfaces for 2d data specified parametrically
void mgl_axial_xy_(long *gr, long *x, long *y, long *a, const char *sch, int *Num,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && x && y)	_GR_->Axial(_D_(x), _D_(y), _D_(a), s, *Num);
	delete []s;
}
/// Draw several axial-symmetric isosurfaces for 2d data
void mgl_axial_(long *gr, long *a, const char *sch, int *Num,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a)	_GR_->Axial(_D_(a), s, *Num);
	delete []s;
}
//-----------------------------------------------------------------------------
/// Draw solid contours at slice for 3d data specified parametrically
void mgl_contf3_xyz_val_(long *gr, long *v, long *x, long *y, long *z, long *a,
			const char *dir, int *sVal, const char *sch,int,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && x && y && z && v)	_GR_->ContF3(_D_(v), _D_(x), _D_(y), _D_(z), _D_(a), *dir, *sVal, s);
	delete []s;
}
/// Draw solid contours at slice for 3d data
void mgl_contf3_val_(long *gr, long *v, long *a, const char *dir, int *sVal, const char *sch,int,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && v)	_GR_->ContF3(_D_(v), _D_(a), *dir, *sVal, s);
	delete []s;
}
/// Draw several solid contours at slice for 3d data specified parametrically
void mgl_contf3_xyz_(long *gr, long *x, long *y, long *z, long *a,
			const char *dir, int *sVal, const char *sch, int *Num,int,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && x && y && z)	_GR_->ContF3(_D_(x), _D_(y), _D_(z), _D_(a), *dir, *sVal, s, *Num);
	delete []s;
}
/// Draw several solid contours at slice for 3d data
void mgl_contf3_(long *gr, long *a, const char *dir, int *sVal, const char *sch, int *Num,int,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a)	_GR_->ContF3(_D_(a), *dir, *sVal, s, *Num);
	delete []s;
}
/// Draw solid contours at central slices for 3d data specified parametrically
void mgl_contf_all_xyz_(long *gr, long *x, long *y, long *z, long *a,
			const char *sch, int *Num,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && x && y && z)	_GR_->ContFA(_D_(x), _D_(y), _D_(z), _D_(a), s, *Num);
	delete []s;
}
/// Draw solid contours at central slices for 3d data
void mgl_contf_all_(long *gr, long *a, const char *sch, int *Num,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a)	_GR_->ContFA(_D_(a), s, *Num);
	delete []s;
}
/// Draw solid contours for 2d data specified parametrically
void mgl_contf_xy_val_(long *gr, long *v, long *x, long *y, long *a, const char *sch, float *zVal,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && x && y && v)	_GR_->ContF(_D_(v), _D_(x), _D_(y), _D_(a), s, *zVal);
	delete []s;
}
/// Draw solid contours for 2d data
void mgl_contf_val_(long *gr, long *v, long *a, const char *sch,float *zVal,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && v)	_GR_->ContF(_D_(v), _D_(a), s, *zVal);
	delete []s;
}
/// Draw several solid contours for 2d data specified parametrically
void mgl_contf_xy_(long *gr, long *x, long *y, long *a, const char *sch, int *Num, float *zVal,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a && x && y)	_GR_->ContF(_D_(x), _D_(y), _D_(a), s, *Num, *zVal);
	delete []s;
}
/// Draw several solid contours for 2d data
void mgl_contf_(long *gr, long *a, const char *sch, int *Num, float *zVal,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a)	_GR_->ContF(_D_(a), s, *Num, *zVal);
	delete []s;
}
/// Print string \a text along curve in 3D with font size \a size.
void mgl_text_xyz_(long *gr, long *x, long *y, long *z,const char *text,
				const char *font, float *size,int l,int n)
{
	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,font,n);	f[n]=0;
	if(gr && x && y && z)	_GR_->Text(_D_(x),_D_(y), _D_(z), s, f, *size);
	delete []s;		delete []f;
}
/// Print string \a text along parametrical curve with font size \a size.
void mgl_text_xy_(long *gr, long *x, long *y, const char *text, const char *font, float *size, int l,int n)
{
	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,font,n);	f[n]=0;
	if(gr && x && y)	_GR_->Text(_D_(x),_D_(y),s,f,*size);
	delete []s;		delete []f;
}
/// Print string \a text along curve with font size \a size.
void mgl_text_y_(long *gr, long *y, const char *text, const char *font, float *size, int l,int n)
{
	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,font,n);	f[n]=0;
	if(gr && y)	_GR_->Text(_D_(y),s,f,*size);
	delete []s;		delete []f;
}
//-----------------------------------------------------------------------------
