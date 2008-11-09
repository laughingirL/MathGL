/* mgl_crust.cpp is part of Math Graphic Library
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

#include <float.h>
#include "mgl/mgl.h"
#include "mgl/mgl_c.h"
#include "mgl/mgl_f.h"
//-----------------------------------------------------------------------------
//
//	TriPlot series
//
//-----------------------------------------------------------------------------
void mglGraph::TriPlot(const mglData &nums, const mglData &x, const mglData &y, const mglData &z, const char *sch)
{
	long n = x.nx, m = nums.ny;
	if(y.nx!=n || z.nx!=n || nums.nx<3)	{	SetWarn(mglWarnLow,"TriPlot");	return;	}
	SetScheme(sch);
	static int cgid=1;	StartGroup("TriPlot",cgid++);
	float *pp = new float[3*n], *cc = new float[4*n];
	bool *tt = new bool[n];
	long *nn = new long[3*m];
	mglColor c;
	register long i,j,k1,k2,k3;
	for(i=0;i<n;i++)
	{
		pp[3*i] = x.a[i];	pp[3*i+1] = y.a[i];	pp[3*i+2] = z.a[i];
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
		c = GetC(x.a[i], y.a[i], z.a[i]);
		cc[4*i] = c.r;		cc[4*i+1] = c.g;
		cc[4*i+2] = c.b;	cc[4*i+3] = Transparent ? AlphaDef : 1;
	}
	for(i=j=0;i<m;i++)
	{
		k1 = long(nums.a[3*i]+0.1);		if(k1<0 || k1>=n)	continue;
		k2 = long(nums.a[3*i+1]+0.1);	if(k2<0 || k2>=n)	continue;
		k3 = long(nums.a[3*i+2]+0.1);	if(k3<0 || k3>=n)	continue;
		nn[3*j]=k1;	nn[3*j+1]=k2;	nn[3*j+2]=k3;	j++;
	}
	trigs_plot(j,nn,n,pp,cc,tt,sch && strchr(sch,'#'));
	EndGroup();
	delete []pp;	delete []tt;	delete []cc;	delete []nn;
}
//-----------------------------------------------------------------------------
void mglGraph::TriPlot(const mglData &nums, const mglData &x, const mglData &y, const char *sch, float zVal)
{
	if(isnan(zVal))	zVal = Min.z;
	mglData z(x.nx);
	z.Fill(zVal,zVal);
	TriPlot(nums,x,y,z,sch);
}
//-----------------------------------------------------------------------------
//
//	Dots series
//
//-----------------------------------------------------------------------------
void mglGraph::Dots(const mglData &x, const mglData &y, const mglData &z, const char *sch)
{
	long n = x.nx;
	if(y.nx!=n || z.nx!=n)	{	SetWarn(mglWarnDim,"Dots");	return;	}
	static int cgid=1;	StartGroup("Dots",cgid++);
	SetScheme(sch);
	mglColor c;
	for(long i=0;i<n;i++)
	{
		c = GetC(x.a[i], y.a[i], z.a[i]);
		Ball(x.a[i], y.a[i], z.a[i], c);
	}
	EndGroup();
}
//-----------------------------------------------------------------------------
void mglGraph::Dots(const mglData &tr, const char *sch)
{
	long n = tr.ny;
	if(tr.nx<3)	{	SetWarn(mglWarnLow,"Dots");	return;	}
	static int cgid=1;	StartGroup("Dots",cgid++);
	SetScheme(sch);
	mglColor c;
	for(long i=0;i<n;i++)
	{
		c = GetC(tr.a[3*i], tr.a[3*i+1], tr.a[3*i+2]);
		Ball(tr.a[3*i], tr.a[3*i+1], tr.a[3*i+2], c);
	}
	EndGroup();
}
//-----------------------------------------------------------------------------
//
//	Crust series
//
//-----------------------------------------------------------------------------
long mgl_crust(long n,float *pp,long **nn,float rs);
void mglGraph::Crust(const mglData &x, const mglData &y, const mglData &z, const char *sch,float er)
{
	long n = x.nx, m;
	if(y.nx!=n || z.nx!=n)	{	SetWarn(mglWarnDim,"Crust");	return;	}
	static int cgid=1;	StartGroup("Crust",cgid++);
	register long i;
	float *pp = new float[3*n];
	long *nn=0;
	for(i=0;i<n;i++)
	{	pp[3*i] = x.a[i];	pp[3*i+1] = y.a[i];	pp[3*i+2] = z.a[i];	}

	m = mgl_crust(n,pp,&nn,er);
	if(m<=0)	{	delete []pp;	if(nn)	free(nn);	return;	}

	SetScheme(sch);
	mglColor c;
	float *cc = new float[4*n];
	bool *tt = new bool[n];
	for(i=0;i<n;i++)
	{
		c = GetC(pp[3*i],pp[3*i+1],pp[3*i+2]);
		cc[4*i] = c.r;		cc[4*i+1] = c.g;
		cc[4*i+2] = c.b;	cc[4*i+3] = 1;
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
	}
	trigs_plot(m,nn,n,pp,cc,tt,sch && strchr(sch,'#'));
	EndGroup();
	delete []tt;	delete []cc;	delete []pp;	free(nn);
}
//-----------------------------------------------------------------------------
void mglGraph::Crust(const mglData &tr, const char *sch,float er)
{
	if(tr.nx<3)	{	SetWarn(mglWarnLow,"Crust");	return;	}
	Crust(tr.SubData(0), tr.SubData(1), tr.SubData(2),sch,er);
}
//-----------------------------------------------------------------------------
float mgl_dist(float *p1, float *p2)
{
	return (p1[0]-p2[0])*(p1[0]-p2[0])+(p1[1]-p2[1])*(p1[1]-p2[1])+(p1[2]-p2[2])*(p1[2]-p2[2]);
}
float mgl_mult(float *p1, float *p2, float *p0)
{
	return (p1[0]-p0[0])*(p2[0]-p0[0])+(p1[1]-p0[1])*(p2[1]-p0[1])+(p1[2]-p0[2])*(p2[2]-p0[2]);
}
//-----------------------------------------------------------------------------
long mgl_insert_trig(long i1,long i2,long i3,long **n)
{
	static long Cur=0,Max=0;
	if(i1<0 || i2<0 || i3<0)	return Cur;
	if(*n==0)
	{
		Max = 1024;		Cur = 0;
		*n = (long *)malloc(Max*3*sizeof(long));
	}
	if(Cur>=Max)
	{
		Max += 1024;
		*n = (long *)realloc(*n,Max*3*sizeof(long));
	}
	long *nn = *n;
	register long i,k;
	for(k=0;k<Cur;k++)
	{
		i = 3*k;
		if((nn[i]==i1 && nn[i+1]==i2 && nn[i+2]==i3))	return Cur;
		if((nn[i]==i1 && nn[i+1]==i3 && nn[i+2]==i2))	return Cur;
		if((nn[i]==i2 && nn[i+1]==i3 && nn[i+2]==i1))	return Cur;
		if((nn[i]==i2 && nn[i+1]==i1 && nn[i+2]==i3))	return Cur;
		if((nn[i]==i3 && nn[i+1]==i2 && nn[i+2]==i1))	return Cur;
		if((nn[i]==i3 && nn[i+1]==i1 && nn[i+2]==i2))	return Cur;
	}
	i = 3*Cur;
	nn[i]=i1;	nn[i+1]=i2;	nn[i+2]=i3;
	Cur++;
	return Cur;
}
//-----------------------------------------------------------------------------
long mgl_get_next(long k1,long n,long *,long *set,float *qq)
{
	long i,j=-1;
	float r,rm=FLT_MAX;
	for(i=0;i<n;i++)
	{
		if(i==k1 || set[i]>0)	continue;
		r = mgl_dist(qq+3*i, qq+3*k1);
		if(r<rm)	{	rm=r;	j=i;	}
	}
	return j;
}
//-----------------------------------------------------------------------------
long mgl_crust(long n,float *pp,long **nn,float ff)
{
	register long i,j;
	register float r,rm,rs;
	if(ff==0)	ff=2;
	for(rs=0,i=0;i<n;i++)
	{
		for(rm = FLT_MAX,j=0;j<n;j++)
		{
			if(i==j)	continue;
			r = mgl_dist(pp+3*i,pp+3*j);
			if(rm>r)	rm = r;
		}
		rs += sqrt(rm);
	}
	rs *= ff/n;	rs = rs*rs;
	long ind[100], set[100], ii;	// indexes of "close" points, flag that it was added and its number
	float qq[300];	// normalized point coordinates
	long k1,k2,k3,m=0;
	for(i=0;i<n;i++)	// now the triangles will be found
	{
		memset(set,0,100*sizeof(long));
		for(ii=0,j=0;j<n;j++)	// find close vertexes
		{
			r = mgl_dist(pp+3*i,pp+3*j);
			if(r<=rs && j!=i)	{	ind[ii] = j;	ii++;	if(ii==99)	break;}
		}
		if(ii<3)	continue;	// nothing to do
		for(j=0;j<ii;j++)
		{
			k1 = 3*j;	k2 = 3*ind[j];	k3 = 3*i;
			qq[k1] = pp[k2] - pp[k3];
			qq[k1+1] = pp[k2+1] - pp[k3+1];
			qq[k1+2] = pp[k2+2] - pp[k3+2];
			r = sqrt(qq[k1]*qq[k1]+qq[k1+1]*qq[k1+1]+qq[k1+2]*qq[k1+2]);
			qq[k1] /= r;	qq[k1+1] /= r;	qq[k1+2] /= r;
		}
		k1 = 0;
		while((k2=mgl_get_next(k1,ii,ind,set,qq))>0)
		{
			set[k1]=1;
			m = mgl_insert_trig(i,ind[k1],ind[k2],nn);
			k1 = k2;
		}
		m = mgl_insert_trig(i,ind[k1],ind[0],nn);
	}
	return m;
}
//-----------------------------------------------------------------------------
/// Draw triangle mesh for points in arrays \a x, \a y, \a z.
void mgl_triplot_xyz(HMGL gr, const HMDT nums, const HMDT x, const HMDT y, const HMDT z, const char *sch)
{	if(gr&&nums&&x&&y&&z)	gr->TriPlot(*nums, *x, *y, *z, sch);	}
/// Draw triangle mesh for points in arrays \a x, \a y.
void mgl_triplot_xy(HMGL gr, const HMDT nums, const HMDT x, const HMDT y, const char *sch, float zVal)
{	if(gr&&nums&&x&&y)	gr->TriPlot(*nums, *x, *y, sch, zVal);	}
/// Draw dots in points \a x, \a y, \a z.
void mgl_dots(HMGL gr, const HMDT x, const HMDT y, const HMDT z, const char *sch)
{	if(gr&&x&&y&&z)	gr->Dots(*x,*y,*z,sch);	}
/// Draw dots in points \a tr.
void mgl_dots_tr(HMGL gr, const HMDT tr, const char *sch)
{	if(gr&&tr)	gr->Dots(*tr,sch);	}
/// Draw power crust for points in arrays \a x, \a y, \a z.
void mgl_crust(HMGL gr, const HMDT x, const HMDT y, const HMDT z, const char *sch, float er)
{	if(gr&&x&&y&&z)	gr->Crust(*x,*y,*z,sch,er);	}
/// Draw power crust for points in arrays \a tr.
void mgl_crust_tr(HMGL gr, const HMDT tr, const char *sch, float er)
{	if(gr&&tr)	gr->Crust(*tr,sch,er);	}
//-----------------------------------------------------------------------------
//	Fortran interface
//-----------------------------------------------------------------------------
/// Draw triangle mesh for points in arrays \a x, \a y, \a z.
void mgl_triplot_xyz_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && nums && x && y && z)	_GR_->TriPlot(_D_(nums), _D_(x), _D_(y), _D_(z), s);
	delete []s;
}
/// Draw triangle mesh for points in arrays \a x, \a y.
void mgl_triplot_xy_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, const char *sch, float *zVal,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr&&nums&&x&&y)	_GR_->TriPlot(_D_(nums), _D_(x), _D_(y), s, *zVal);
	delete []s;
}
/// Draw dots in points \a x, \a y, \a z.
void mgl_dots_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && x && y && z)	_GR_->Dots(_D_(x),_D_(y),_D_(z),s);
	delete []s;
}
/// Draw dots in points \a tr.
void mgl_dots_tr_(uintptr_t *gr, uintptr_t *tr, const char *sch,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && tr)	_GR_->Dots(_D_(tr),s);
	delete []s;
}
/// Draw power crust for points in arrays \a x, \a y, \a z.
void mgl_crust_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, float *er,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && x && y && z)	_GR_->Crust(_D_(x),_D_(y),_D_(z),s,*er);
	delete []s;
}
/// Draw power crust for points in arrays \a tr.
void mgl_crust_tr_(uintptr_t *gr, uintptr_t *tr, const char *sch, float *er,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && tr)	_GR_->Crust(_D_(tr),s,*er);
	delete []s;
}
//-----------------------------------------------------------------------------
