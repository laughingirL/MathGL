/***************************************************************************
 * surf.cpp is part of Math Graphic Library
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
#include "mgl/define.h"
#include "mgl/volume.h"
#include "mgl/data.h"
#include "mgl/eval.h"
//-----------------------------------------------------------------------------
//
//	CloudQ series
//
//-----------------------------------------------------------------------------
void mgl_cloud_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT a, const char *sch, const char *opt)
{
	if(!(gr->GetQuality()&3))	return;	// do nothing in fast_draw
	long i,j,k,n=a->GetNx(),m=a->GetNy(),l=a->GetNz();
	register int i0;
	if(n<2 || m<2 || l<2)	{	gr->SetWarn(mglWarnLow,"Cloud");	return;	}
	bool both = x->GetNx()*x->GetNy()*x->GetNz()==n*m*l && y->GetNx()*y->GetNy()*y->GetNz()==n*m*l && z->GetNx()*z->GetNy()*z->GetNz()==n*m*l;
	if(!(both || (x->GetNx()==n && y->GetNx()==m && z->GetNx()==l)))
	{	gr->SetWarn(mglWarnDim);	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Cloud",cgid++);

	int tx=1,ty=1,tz=1;
	if(gr->MeshNum>1)
	{	tx=(n-1)/(gr->MeshNum-1);	ty=(m-1)/(gr->MeshNum-1);	tz=(l-1)/(gr->MeshNum-1);}
	if(tx<1)	tx=1;	if(ty<1)	ty=1;	if(tz<1)	tz=1;

	float	alpha = gr->AlphaDef;
	bool inv = sch && strchr(sch,'-');
	bool dot = sch && strchr(sch,'.');
	alpha /= pow(n/tx*m/ty*l/tz,1./3)/20;
	float aa,bb;
	if(alpha>1)	alpha = 1;
	long ss = gr->AddTexture(sch);

	// x, y, z -- have the same size as a
	long pos = (n/tx)*(m/ty)*(l/tz);
	pos = gr->Reserve(pos);
	mglPoint p,q=mglPoint(NAN);
	for(k=0;k<l;k+=tz)	for(j=0;j<m;j+=ty)	for(i=0;i<n;i+=tx)
	{
		p = both ? mglPoint(x->v(i,j,k),y->v(i,j,k),z->v(i,j,k)) : mglPoint(x->v(i),y->v(j),z->v(k));
		aa = gr->GetA(a->v(i,j,k));
		if(inv)	bb = (1-aa)*(1-aa)*alpha;
		else	bb = aa*aa*alpha;
		gr->AddPnt(p,gr->GetC(ss,aa,false),q,bb);	// NOTE: Not thread safe!!!
	}
	n /= tx;	m /= ty;	l /= tz;
	if(dot)	for(i=0;i<n*m*l;i++)	gr->mark_plot(pos+i,'.');
	else	for(i=0;i<n;i++)	for(j=0;j<m;j++)	for(k=0;k<l;k++)
	{
		i0 = pos + i+n*(j+m*k);
		if(i<n-1 && j<m-1)	gr->quad_plot(i0,i0+1,i0+n,i0+n+1);
		if(i<n-1 && k<l-1)	gr->quad_plot(i0,i0+1,i0+n*m,i0+n*m+1);
		if(k<l-1 && j<m-1)	gr->quad_plot(i0,i0+n,i0+n+n*m,i0+n+n*m);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_cloud(HMGL gr, HCDT a, const char *sch, const char *opt)
{
	if(a->GetNx()<2 || a->GetNy()<2 || a->GetNz()<2)
	{	gr->SetWarn(mglWarnLow,"Cloud");	return;	}
	gr->SaveState(opt);
	mglData x(a->GetNx()), y(a->GetNy()),z(a->GetNz());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	z.Fill(gr->Min.z,gr->Max.z);
	mgl_cloud_xyz(gr,&x,&y,&z,a,sch,0);
}
//-----------------------------------------------------------------------------
void mgl_cloud_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_cloud_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(a), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_cloud_(uintptr_t *gr, uintptr_t *a, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_cloud(_GR_, _DA_(a), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Surf3 series
//
//-----------------------------------------------------------------------------
mglPoint mgl_normal_3d(const mglDataA *a, mglPoint p, bool inv)
{
	long n=a->GetNx(), m=a->GetNy(), l=a->GetNz();
	register long i,j,k;
	register float x=p.x, y=p.y, z=p.z;
	float nx=0, ny=0, nz=0;
	i=long(x);	j=long(y);	k=long(z);
	i = i<n-1 ? i:n-2;	j = j<m-1 ? j:m-2;	k = k<l-1 ? k:l-2;
	x-=i;	y-=j;	z-=k;

	if(i<n-1)	nx = a->dvx(i,j,k)*(1-x) + a->dvx(i+1,j,k)*x;
	else		nx = a->dvx(i,j,k);
	if(j<m-1)	ny = a->dvy(i,j,k)*(1-y) + a->dvy(i,j+1,k)*y;
	else		ny = a->dvy(i,j,k);
	if(k<l-1)	nz = a->dvz(i,j,k)*(1-z) + a->dvz(i,j,k+1)*z;
	else		nz = a->dvz(i,j,k);
	return inv ? mglPoint(nx,ny,nz) : mglPoint(-nx,-ny,-nz);
}
//-----------------------------------------------------------------------------
float mgl_normal_1d(const mglDataA *a, float x, bool inv)
{
	register long n=a->GetNx(), i=long(x);	x-=i;
	float nx = a->dvx(i);
	if(i<n-1)	nx = nx*(1-x) + a->dvx(i+1)*x;
	return inv ? nx : -nx;
}
//-----------------------------------------------------------------------------
mglPoint mgl_find_norm(bool both, HCDT x, HCDT y, HCDT z, HCDT a, mglPoint u, bool inv)
{
	mglPoint s = mgl_normal_3d(a,u,inv), t, q;
	if(both)
	{
		t = mgl_normal_3d(x,u,true);	q.x = (s*t)/(t*t);
		t = mgl_normal_3d(y,u,true);	q.y = (s*t)/(t*t);
		t = mgl_normal_3d(z,u,true);	q.z = (s*t)/(t*t);
	}
	else
	{
		q.x = s.x/mgl_normal_1d(x,u.x,true);
		q.y = s.y/mgl_normal_1d(y,u.y,true);
		q.z = s.z/mgl_normal_1d(z,u.z,true);
	}
	return q;
}
//-----------------------------------------------------------------------------
inline float mgl_cos_pp(mglPoint *kk,long i0,long i1,long i2)
{
	mglPoint dp1 = kk[i1]-kk[i0], dp2 = kk[i2]-kk[i0];
	float p1=dp1*dp1,p2=dp2*dp2,pc=dp1*dp2;
	return p1*p2>1e-10 ? pc/sqrt(p1*p2) : NAN;
}
//-----------------------------------------------------------------------------
void mgl_surf3_plot(HMGL gr, long posN, long n,long m,long *kx1,long *kx2,long *ky1,long *ky2, long *kz, mglPoint *kk, bool wire)
{
	register long i,j,k,i0,ii,jj;
	long id[12],us[12],ni;
	float d,d0;

	for(i=0;i<n-1;i++)	for(j=0;j<m-1;j++)
	{
		i0 = i+n*j;
		// find ID of points of Surf3 intersection with cell i0
		memset(id,-1,12*sizeof(long));	ni = 0;
		if(kx1[i0]>=0)		id[ni++] = kx1[i0];
		if(ky1[i0]>=0)		id[ni++] = ky1[i0];
		if(kx1[i0+n]>=0)	id[ni++] = kx1[i0+n];
		if(ky1[i0+1]>=0)	id[ni++] = ky1[i0+1];
		if(kz[i0]>=0)		id[ni++] = kz[i0];
		if(kz[i0+1]>=0)		id[ni++] = kz[i0+1];
		if(kz[i0+n+1]>=0)	id[ni++] = kz[i0+n+1];
		if(kz[i0+n]>=0)		id[ni++] = kz[i0+n];
		if(kx2[i0]>=0)		id[ni++] = kx2[i0];
		if(ky2[i0]>=0)		id[ni++] = ky2[i0];
		if(kx2[i0+n]>=0)	id[ni++] = kx2[i0+n];
		if(ky2[i0+1]>=0)	id[ni++] = ky2[i0+1];
		if(ni<3)	continue;
		// remove points which is too close to first one
		for(jj=1;jj<ni;)
		{
			d = Norm(kk[id[jj]] - kk[id[0]]);
			if(d>1e-5)	jj++;
			else
			{	ni--;	for(ii=jj;ii<ni;ii++)	id[ii]=id[ii+1];	}
		}
		// continue if number of points <3 i.e. there is no triangle
		if(ni<3)	continue;
		memset(us,0,12*sizeof(long));
		// firstly let find most outstanding point
		for(jj=1,ii=2,d0=2;ii<ni;ii++)
		{
			d = mgl_cos_pp(kk,id[0],id[ii],id[1]);
			if(d<d0)	{	d0=d;	jj=ii;	}
		}
		// copy first 2 points as base
		long p1 = posN+id[0], p2 = posN+id[jj];
		// select the same orientation of all triangles of the surface
		us[0] = us[jj] = 1;
		// find all triangles
		for(k=2;k<ni;k++)
		{
			// find closest point in sence cosine of angle
			for(i0=-1,ii=1,d0=-2;ii<ni;ii++)
			{
				if(us[ii])	continue;
				d = mgl_cos_pp(kk,id[0],id[ii],id[jj]);
				if(d>d0)	{	d0=d;	i0=ii;	}
			}
			if(i0<0)	break;	// no more triangles. NOTE: should be never here
			jj = i0;	us[jj]=1;
			gr->trig_plot(p1, p2, posN+id[jj]);	p2 = posN+id[jj];
		}
	}
}
//-----------------------------------------------------------------------------
void mgl_surf3_xyz_val(HMGL gr, float val, HCDT x, HCDT y, HCDT z, HCDT a, const char *sch, const char *opt)
{
	long i,j,k,i1,n=a->GetNx(),m=a->GetNy(),l=a->GetNz();
	long *kx1,*kx2,*ky1,*ky2,*kz;
	bool both, wire = sch && strchr(sch,'#');
	float d;
	if(n<2 || m<2 || l<2)	{	gr->SetWarn(mglWarnLow,"Surf3");	return;	}
	both = x->GetNx()*x->GetNy()*x->GetNz()==n*m*l && y->GetNx()*y->GetNy()*y->GetNz()==n*m*l && z->GetNx()*z->GetNy()*z->GetNz()==n*m*l;
	if(!(both || (x->GetNx()==n && y->GetNx()==m && z->GetNx()==l)))
	{	gr->SetWarn(mglWarnDim,"Surf3");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Surf3",cgid++);

	bool inv = (sch && strchr(sch,'-'));
	long ss = gr->AddTexture(sch);

	kx1 = new long[n*m];	kx2 = new long[n*m];
	ky1 = new long[n*m];	ky2 = new long[n*m];
	kz  = new long[n*m];
	float c=gr->GetC(ss,val);
	long numK = n*m, posN = gr->GetPos(), pos;
	mglPoint *kk = (mglPoint *)malloc(numK*sizeof(mglPoint));
	memset(kk,0,numK*sizeof(mglPoint));

	mglPoint p,q,u;
	for(k=0;k<l;k++)
	{
		memcpy(kx1,kx2,n*m*sizeof(long));	memset(kx2,-1,n*m*sizeof(long));
		memcpy(ky1,ky2,n*m*sizeof(long));	memset(ky2,-1,n*m*sizeof(long));
		memset(kz ,-1,n*m*sizeof(long));
		gr->Reserve(n*m);	gr->Reserve(n*m);
		for(j=0;j<m;j++)	for(i=0;i<n;i++)
		{
			i1 = i+n*j;
			if(i<n-1)
			{
				d = mgl_d(val,a->v(i,j,k),a->v(i+1,j,k));
				if(d>=0 && d<1)
				{
					if(both)	p = mglPoint(x->v(i,j,k)*(1-d)+x->v(i+1,j,k)*d,
									y->v(i,j,k)*(1-d)+y->v(i+1,j,k)*d,
									z->v(i,j,k)*(1-d)+z->v(i+1,j,k)*d);
					else	p = mglPoint(x->v(i)*(1-d)+x->v(i+1)*d, y->v(j), z->v(k));
					u = mglPoint(i+d,j,k);
					q = mgl_find_norm(both, x,y,z,a, u, inv);
					pos = gr->AddPnt(p,c,q)-posN;	// NOTE: Not thread-safe!!!
					if(pos>=numK)
					{
						numK += n*m*(1+(pos/(n*m)));
						kk = (mglPoint *)realloc(kk,numK*sizeof(mglPoint));
					}
					kk[pos] = u;	kx2[i1] = pos;
				}
			}
			if(j<m-1)
			{
				d = mgl_d(val,a->v(i,j,k),a->v(i,j+1,k));
				if(d>=0 && d<1)
				{
					if(both)	p = mglPoint(x->v(i,j,k)*(1-d)+x->v(i,j+1,k)*d,
									y->v(i,j,k)*(1-d)+y->v(i,j+1,k)*d,
									z->v(i,j,k)*(1-d)+z->v(i,j+1,k)*d);
					else	p = mglPoint(x->v(i), y->v(j)*(1-d)+y->v(j+1)*d, z->v(k));
					u = mglPoint(i,j+d,k);
					q = mgl_find_norm(both, x,y,z,a, u, inv);
					pos = gr->AddPnt(p,c,q)-posN;	// NOTE: Not thread-safe!!!
					if(pos>=numK)
					{
						numK += n*m*(1+(pos/(n*m)));
						kk = (mglPoint *)realloc(kk,numK*sizeof(mglPoint));
					}
					kk[pos] = u;	ky2[i1] = pos;
				}
			}
			if(k>0)
			{
				d = mgl_d(val,a->v(i,j,k-1),a->v(i,j,k));
				if(d>=0 && d<1)
				{
					if(both)	p = mglPoint(x->v(i,j,k-1)*(1-d)+x->v(i,j,k)*d,
									y->v(i,j,k-1)*(1-d)+y->v(i,j,k)*d,
									z->v(i,j,k-1)*(1-d)+z->v(i,j,k)*d);
					else	p = mglPoint(x->v(i), y->v(j), z->v(k-1)*(1-d)+z->v(k)*d);
					u = mglPoint(i,j,k+d-1);
					q = mgl_find_norm(both, x,y,z,a, u, inv);
					pos = gr->AddPnt(p,c,q)-posN;	// NOTE: Not thread-safe!!!
					if(pos>=numK)
					{
						numK += n*m*(1+(pos/(n*m)));
						kk = (mglPoint *)realloc(kk,numK*sizeof(mglPoint));
					}
					kk[pos] = u;	kz[i1] = pos;
				}
			}
		}
		if(k>0)	mgl_surf3_plot(gr,posN,n,m,kx1,kx2,ky1,ky2,kz,kk,wire);
	}
	gr->EndGroup();
	delete []kx1;	delete []kx2;	delete []ky1;
	delete []ky2;	delete []kz;	free(kk);
}
//-----------------------------------------------------------------------------
void mgl_surf3_val(HMGL gr, float val, HCDT a, const char *sch, const char *opt)
{
	if(a->GetNx()<2 || a->GetNy()<2 || a->GetNz()<2)	{	gr->SetWarn(mglWarnLow,"Surf3");	return;	}
	gr->SaveState(opt);
	mglData x(a->GetNx()), y(a->GetNy()),z(a->GetNz());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	z.Fill(gr->Min.z,gr->Max.z);
	mgl_surf3_xyz_val(gr,val,&x,&y,&z,a,sch,0);
}
//-----------------------------------------------------------------------------
void mgl_surf3_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT a, const char *sch, const char *opt)
{
	float r = gr->SaveState(opt);
	long num = isnan(r)?3:long(r+0.5);
	for(long i=0;i<num;i++)
	{
		float v = gr->Max.c + (gr->Min.c-gr->Max.c)*(i+1.)/(num+1);
		mgl_surf3_xyz_val(gr,v,x,y,z,a,sch,0);
	}
}
//-----------------------------------------------------------------------------
void mgl_surf3(HMGL gr, HCDT a, const char *sch, const char *opt)
{
	float r = gr->SaveState(opt);
	long num = isnan(r)?3:long(r+0.5);
	for(long i=0;i<num;i++)
	{
		float v = gr->Max.c + (gr->Min.c-gr->Max.c)*(i+1.)/(num+1);
		mgl_surf3_val(gr,v,a,sch,0);
	}
}
//-----------------------------------------------------------------------------
void mgl_surf3_xyz_val_(uintptr_t *gr, float *Val, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_surf3_xyz_val(_GR_, *Val, _DA_(x), _DA_(y), _DA_(z), _DA_(a), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_surf3_val_(uintptr_t *gr, float *Val, uintptr_t *a, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_surf3_val(_GR_, *Val, _DA_(a), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_surf3_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_surf3_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(a), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_surf3_(uintptr_t *gr, uintptr_t *a, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_surf3(_GR_, _DA_(a), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Surf3A series
//
//-----------------------------------------------------------------------------
void mgl_surf3a_xyz_val(HMGL gr, float val, HCDT x, HCDT y, HCDT z, HCDT a, HCDT b, const char *sch, const char *opt)
{
	long i,j,k,i1,n=a->GetNx(),m=a->GetNy(),l=a->GetNz();
	long *kx1,*kx2,*ky1,*ky2,*kz;
	bool both, wire = sch && strchr(sch,'#');
	float d;
	if(n<2 || m<2 || l<2)	{	gr->SetWarn(mglWarnLow,"Surf3A");	return;	}
	both = x->GetNx()*x->GetNy()*x->GetNz()==n*m*l && y->GetNx()*y->GetNy()*y->GetNz()==n*m*l && z->GetNx()*z->GetNy()*z->GetNz()==n*m*l;
	if(!(both || (x->GetNx()==n && y->GetNx()==m && z->GetNx()==l)))
	{	gr->SetWarn(mglWarnDim,"Surf3A");	return;	}
	if(b->GetNx()*b->GetNy()*b->GetNz()!=n*m*l)	{	gr->SetWarn(mglWarnDim,"Surf3A");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Surf3A",cgid++);

	bool inv = (sch && strchr(sch,'-'));
	long ss = gr->AddTexture(sch);

	kx1 = new long[n*m];	kx2 = new long[n*m];
	ky1 = new long[n*m];	ky2 = new long[n*m];
	kz  = new long[n*m];
	float c=gr->GetC(ss,val),aa;
	long numK = n*m, posN = gr->GetPos(), pos;
	mglPoint *kk = (mglPoint *)malloc(numK*sizeof(mglPoint));
	memset(kk,0,numK*sizeof(mglPoint));

	mglPoint p,q,u;
	for(k=0;k<l;k++)
	{
		memcpy(kx1,kx2,n*m*sizeof(long));	memset(kx2,-1,n*m*sizeof(long));
		memcpy(ky1,ky2,n*m*sizeof(long));	memset(ky2,-1,n*m*sizeof(long));
		memset(kz ,-1,n*m*sizeof(long));
		gr->Reserve(n*m);	gr->Reserve(n*m);
		for(j=0;j<m;j++)	for(i=0;i<n;i++)
		{
			i1 = i+n*j;
			if(i<n-1)
			{
				d = mgl_d(val,a->v(i,j,k),a->v(i+1,j,k));
				if(d>=0 && d<1)
				{
					if(both)	p = mglPoint(x->v(i,j,k)*(1-d)+x->v(i+1,j,k)*d,
									y->v(i,j,k)*(1-d)+y->v(i+1,j,k)*d,
									z->v(i,j,k)*(1-d)+z->v(i+1,j,k)*d);
					else	p = mglPoint(x->v(i)*(1-d)+x->v(i+1)*d, y->v(j), z->v(k));
					aa = gr->GetA(b->v(i,j,k)*(1-d)+b->v(i+1,j,k)*d);
					u = mglPoint(i+d,j,k);
					q = mgl_find_norm(both, x,y,z,a, u, inv);
					pos = gr->AddPnt(p,c,q,aa)-posN;	// NOTE: Not thread-safe!!!
					if(pos>=numK)
					{
						numK += n*m*(1+(pos/(n*m)));
						kk = (mglPoint *)realloc(kk,numK*sizeof(mglPoint));
					}
					kk[pos] = u;	kx2[i1] = pos;
				}
			}
			if(j<m-1)
			{
				d = mgl_d(val,a->v(i,j,k),a->v(i,j+1,k));
				if(d>=0 && d<1)
				{
					if(both)	p = mglPoint(x->v(i,j,k)*(1-d)+x->v(i,j+1,k)*d,
									y->v(i,j,k)*(1-d)+y->v(i,j+1,k)*d,
									z->v(i,j,k)*(1-d)+z->v(i,j+1,k)*d);
					else	p = mglPoint(x->v(i), y->v(j)*(1-d)+y->v(j+1)*d, z->v(k));
					aa = gr->GetA(b->v(i,j,k)*(1-d)+b->v(i,j+1,k)*d);
					u = mglPoint(i,j+d,k);
					q = mgl_find_norm(both, x,y,z,a, u, inv);
					pos = gr->AddPnt(p,c,q,aa)-posN;	// NOTE: Not thread-safe!!!
					if(pos>=numK)
					{
						numK += n*m*(1+(pos/(n*m)));
						kk = (mglPoint *)realloc(kk,numK*sizeof(mglPoint));
					}
					kk[pos] = u;	ky2[i1] = pos;
				}
			}
			if(k>0)
			{
				d = mgl_d(val,a->v(i,j,k-1),a->v(i,j,k));
				if(d>=0 && d<1)
				{
					if(both)	p = mglPoint(x->v(i,j,k-1)*(1-d)+x->v(i,j,k)*d,
									y->v(i,j,k-1)*(1-d)+y->v(i,j,k)*d,
									z->v(i,j,k-1)*(1-d)+z->v(i,j,k)*d);
					else	p = mglPoint(x->v(i), y->v(j), z->v(k-1)*(1-d)+z->v(k)*d);
					aa = gr->GetA(b->v(i,j,k-1)*(1-d)+b->v(i,j,k)*d);
					u = mglPoint(i,j,k+d-1);
					q = mgl_find_norm(both, x,y,z,a, u, inv);
					pos = gr->AddPnt(p,c,q,aa)-posN;	// NOTE: Not thread-safe!!!
					if(pos>=numK)
					{
						numK += n*m*(1+(pos/(n*m)));
						kk = (mglPoint *)realloc(kk,numK*sizeof(mglPoint));
					}
					kk[pos] = u;	kz[i1] = pos;
				}
			}
		}
		if(k>0)	mgl_surf3_plot(gr,posN,n,m,kx1,kx2,ky1,ky2,kz,kk,wire);
	}
	gr->EndGroup();
	delete []kx1;	delete []kx2;	delete []ky1;
	delete []ky2;	delete []kz;	free(kk);
}
//-----------------------------------------------------------------------------
void mgl_surf3a_val(HMGL gr, float val, HCDT a, HCDT b, const char *sch, const char *opt)
{
	if(a->GetNx()<2 || a->GetNy()<2 || a->GetNz()<2)	{	gr->SetWarn(mglWarnLow,"Surf3A");	return;	}
	gr->SaveState(opt);
	mglData x(a->GetNx()), y(a->GetNy()),z(a->GetNz());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	z.Fill(gr->Min.z,gr->Max.z);
	mgl_surf3a_xyz_val(gr,val,&x,&y,&z,a,b,sch,0);
}
//-----------------------------------------------------------------------------
void mgl_surf3a_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT a, HCDT b, const char *sch, const char *opt)
{
	float r = gr->SaveState(opt);
	long num = isnan(r)?3:long(r+0.5);
	for(long i=0;i<num;i++)
	{
		float v = gr->Max.c + (gr->Min.c-gr->Max.c)*(i+1.)/(num+1);
		mgl_surf3a_xyz_val(gr,v,x,y,z,a,b,sch,0);
	}
}
//-----------------------------------------------------------------------------
void mgl_surf3a(HMGL gr, HCDT a, HCDT b, const char *sch, const char *opt)
{
	float r = gr->SaveState(opt);
	long num = isnan(r)?3:long(r);
	for(long i=0;i<num;i++)
	{
		float v = gr->Max.c + (gr->Min.c-gr->Max.c)*(i+1.)/(num+1);
		mgl_surf3a_val(gr,v,a,b,sch,0);
	}
}
//-----------------------------------------------------------------------------
void mgl_surf3a_xyz_val_(uintptr_t *gr, float *Val, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, uintptr_t *b, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_surf3a_xyz_val(_GR_, *Val, _DA_(x), _DA_(y), _DA_(z), _DA_(a), _DA_(b), s, o);
	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_surf3a_val_(uintptr_t *gr, float *Val, uintptr_t *a, uintptr_t *b, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	 mgl_surf3a_val(_GR_, *Val, _DA_(a), _DA_(b), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_surf3a_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, uintptr_t *b, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_surf3a_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(a), _DA_(b), s, o);
	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_surf3a_(uintptr_t *gr, uintptr_t *a, uintptr_t *b, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_surf3a(_GR_, _DA_(a), _DA_(b), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Surf3C series
//
//-----------------------------------------------------------------------------
void mgl_surf3c_xyz_val(HMGL gr, float val, HCDT x, HCDT y, HCDT z, HCDT a, HCDT b, const char *sch, const char *opt)
{
	long i,j,k,i1,n=a->GetNx(),m=a->GetNy(),l=a->GetNz();
	long *kx1,*kx2,*ky1,*ky2,*kz;
	bool both, wire = sch && strchr(sch,'#');
	float d;
	if(n<2 || m<2 || l<2)	{	gr->SetWarn(mglWarnLow,"Surf3A");	return;	}
	both = x->GetNx()*x->GetNy()*x->GetNz()==n*m*l && y->GetNx()*y->GetNy()*y->GetNz()==n*m*l && z->GetNx()*z->GetNy()*z->GetNz()==n*m*l;
	if(!(both || (x->GetNx()==n && y->GetNx()==m && z->GetNx()==l)))
	{	gr->SetWarn(mglWarnDim,"Surf3A");	return;	}
	if(b->GetNx()*b->GetNy()*b->GetNz()!=n*m*l)	{	gr->SetWarn(mglWarnDim,"Surf3A");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Surf3A",cgid++);

	bool inv = (sch && strchr(sch,'-'));
	long ss = gr->AddTexture(sch);

	kx1 = new long[n*m];	kx2 = new long[n*m];
	ky1 = new long[n*m];	ky2 = new long[n*m];
	kz  = new long[n*m];
	float c;
	long numK = n*m, posN = gr->GetPos(), pos;
	mglPoint *kk = (mglPoint *)malloc(numK*sizeof(mglPoint));
	memset(kk,0,numK*sizeof(mglPoint));

	mglPoint p,q,u;
	for(k=0;k<l;k++)
	{
		memcpy(kx1,kx2,n*m*sizeof(long));	memset(kx2,-1,n*m*sizeof(long));
		memcpy(ky1,ky2,n*m*sizeof(long));	memset(ky2,-1,n*m*sizeof(long));
		memset(kz ,-1,n*m*sizeof(long));
		gr->Reserve(n*m);	gr->Reserve(n*m);
		for(j=0;j<m;j++)	for(i=0;i<n;i++)
		{
			i1 = i+n*j;
			if(i<n-1)
			{
				d = mgl_d(val,a->v(i,j,k),a->v(i+1,j,k));
				if(d>=0 && d<1)
				{
					if(both)	p = mglPoint(x->v(i,j,k)*(1-d)+x->v(i+1,j,k)*d,
									y->v(i,j,k)*(1-d)+y->v(i+1,j,k)*d,
									z->v(i,j,k)*(1-d)+z->v(i+1,j,k)*d);
					else	p = mglPoint(x->v(i)*(1-d)+x->v(i+1)*d, y->v(j), z->v(k));
					c = gr->GetC(ss,b->v(i,j,k)*(1-d)+b->v(i+1,j,k)*d);
					u = mglPoint(i+d,j,k);
					q = mgl_find_norm(both, x,y,z,a, u, inv);
					pos = gr->AddPnt(p,c,q)-posN;	// NOTE: Not thread-safe!!!
					if(pos>=numK)
					{
						numK += n*m*(1+(pos/(n*m)));
						kk = (mglPoint *)realloc(kk,numK*sizeof(mglPoint));
					}
					kk[pos] = u;	kx2[i1] = pos;
				}
			}
			if(j<m-1)
			{
				d = mgl_d(val,a->v(i,j,k),a->v(i,j+1,k));
				if(d>=0 && d<1)
				{
					if(both)	p = mglPoint(x->v(i,j,k)*(1-d)+x->v(i,j+1,k)*d,
									y->v(i,j,k)*(1-d)+y->v(i,j+1,k)*d,
									z->v(i,j,k)*(1-d)+z->v(i,j+1,k)*d);
					else	p = mglPoint(x->v(i), y->v(j)*(1-d)+y->v(j+1)*d, z->v(k));
					c = gr->GetC(ss,b->v(i,j,k)*(1-d)+b->v(i,j+1,k)*d);
					u = mglPoint(i,j+d,k);
					q = mgl_find_norm(both, x,y,z,a, u, inv);
					pos = gr->AddPnt(p,c,q)-posN;	// NOTE: Not thread-safe!!!
					if(pos>=numK)
					{
						numK += n*m*(1+(pos/(n*m)));
						kk = (mglPoint *)realloc(kk,numK*sizeof(mglPoint));
					}
					kk[pos] = u;	ky2[i1] = pos;
				}
			}
			if(k>0)
			{
				d = mgl_d(val,a->v(i,j,k-1),a->v(i,j,k));
				if(d>=0 && d<1)
				{
					if(both)	p = mglPoint(x->v(i,j,k-1)*(1-d)+x->v(i,j,k)*d,
									y->v(i,j,k-1)*(1-d)+y->v(i,j,k)*d,
									z->v(i,j,k-1)*(1-d)+z->v(i,j,k)*d);
					else	p = mglPoint(x->v(i), y->v(j), z->v(k-1)*(1-d)+z->v(k)*d);
					c = gr->GetC(ss,b->v(i,j,k-1)*(1-d)+b->v(i,j,k)*d);
					u = mglPoint(i,j,k+d-1);
					q = mgl_find_norm(both, x,y,z,a, u, inv);
					pos = gr->AddPnt(p,c,q)-posN;	// NOTE: Not thread-safe!!!
					if(pos>=numK)
					{
						numK += n*m*(1+(pos/(n*m)));
						kk = (mglPoint *)realloc(kk,numK*sizeof(mglPoint));
					}
					kk[pos] = u;	kz[i1] = pos;
				}
			}
		}
		if(k>0)	mgl_surf3_plot(gr,posN,n,m,kx1,kx2,ky1,ky2,kz,kk,wire);
	}
	gr->EndGroup();
	delete []kx1;	delete []kx2;	delete []ky1;
	delete []ky2;	delete []kz;	free(kk);
}
//-----------------------------------------------------------------------------
void mgl_surf3c_val(HMGL gr, float val, HCDT a, HCDT b, const char *sch, const char *opt)
{
	if(a->GetNx()<2 || a->GetNy()<2 || a->GetNz()<2)	{	gr->SetWarn(mglWarnLow,"Surf3C");	return;	}
	gr->SaveState(opt);
	mglData x(a->GetNx()), y(a->GetNy()),z(a->GetNz());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	z.Fill(gr->Min.z,gr->Max.z);
	mgl_surf3c_xyz_val(gr,val,&x,&y,&z,a,b,sch,0);
}
//-----------------------------------------------------------------------------
void mgl_surf3c_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT a, HCDT b, const char *sch, const char *opt)
{
	float r = gr->SaveState(opt);
	long num = isnan(r)?3:long(r+0.5);
	for(long i=0;i<num;i++)
	{
		float v = gr->Max.c + (gr->Min.c-gr->Max.c)*(i+1.)/(num+1);
		mgl_surf3c_xyz_val(gr,v,x,y,z,a,b,sch,0);
	}
}
//-----------------------------------------------------------------------------
void mgl_surf3c(HMGL gr, HCDT a, HCDT b, const char *sch, const char *opt)
{
	float r = gr->SaveState(opt);
	long num = isnan(r)?3:long(r+0.5);
	for(long i=0;i<num;i++)
	{
		float v = gr->Max.c + (gr->Min.c-gr->Max.c)*(i+1.)/(num+1);
		mgl_surf3c_val(gr,v,a,b,sch,0);
	}
}
//-----------------------------------------------------------------------------
void mgl_surf3c_xyz_val_(uintptr_t *gr, float *Val, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, uintptr_t *b, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_surf3c_xyz_val(_GR_, *Val, _DA_(x), _DA_(y), _DA_(z), _DA_(a), _DA_(b), s, o);
	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_surf3c_val_(uintptr_t *gr, float *Val, uintptr_t *a, uintptr_t *b, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	 mgl_surf3c_val(_GR_, *Val, _DA_(a), _DA_(b), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_surf3c_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, uintptr_t *b, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_surf3c_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(a), _DA_(b), s, o);
	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_surf3c_(uintptr_t *gr, uintptr_t *a, uintptr_t *b, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_surf3c(_GR_, _DA_(a), _DA_(b), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Beam series
//
//-----------------------------------------------------------------------------
// flag & 0x1	--	accompanied coordinates
// flag & 0x2	--	project to r*z
// flag & 0x4	--	normalize field
void mgl_beam_val(HMGL gr, float val, HCDT tr, HCDT g1, HCDT g2, HCDT a, float r, const char *stl, int flag)
{
	long n = a->GetNz(),m=a->GetNx(),l=a->GetNy();
	if(n<2 || m<2 || l<2)	{	gr->SetWarn(mglWarnLow);	return;	}
	if(a->Minimal()<0)		{	gr->SetWarn(mglWarnNeg);	return;	}
	if(tr->GetNx()<3 || tr->GetNy()<n || g1->GetNx()<3 || g1->GetNy()<n || g2->GetNx()<3 || g2->GetNy()<n)
	{	gr->SetWarn(mglWarnDim,"Beam");	return;	}
	mglData x(a),y(a),z(a),b(a);
	register long i,j,k,i0;
	float asum=1, asum0=1, amax, aa;
	r = fabs(r);
	if(flag & 4)	for(j=0;j<m*l;j++)	asum0 += a->vthr(j)*a->vthr(j);
	if(asum0==0)	{	gr->SetWarn(mglWarnZero,"Beam");	return;	}
	for(i=0;i<n;i++)
	{
		asum=amax=0;
		if(flag & 4)
		{
			for(j=0;j<m*l;j++)
			{
				aa = a->vthr(j+m*l*i);
				asum += aa*aa;
				amax = amax>aa ? amax : aa;
			}
			if(amax==0)	{	asum=0;	amax=1;	}
			for(j=0;j<m*l;j++)	b.a[j+m*l*i] = b.a[j+m*l*i]*sqrt(asum/asum0)/amax;
		}
		for(j=0;j<m;j++)	for(k=0;k<l;k++)
		{
			i0 = j+m*(k+l*i);
			if(flag & 1)
			{
				x.a[i0] = 2*j/(m-1.)-1;
				y.a[i0] = 2*k/(l-1.)-1;
				z.a[i0] = gr->Max.z*i/(n-1.);
			}
			else
			{
				x.a[i0] = tr->v(0,i) + g1->v(0,i)*(2*j/(m-1.)-1)*r + g2->v(0,i)*(2*k/(l-1.)-1)*r;
				y.a[i0] = tr->v(1,i) + g1->v(1,i)*(2*j/(m-1.)-1)*r + g2->v(1,i)*(2*k/(l-1.)-1)*r;
				z.a[i0] = tr->v(2,i) + g1->v(2,i)*(2*j/(m-1.)-1)*r + g2->v(2,i)*(2*k/(l-1.)-1)*r;
			}
			if(flag & 2)	x.a[i0] = hypot(x.a[i0],y.a[i0]);
		}
	}
	mgl_surf3_xyz_val(gr,val,&x,&y,&z,&b,stl,0);
}
//-----------------------------------------------------------------------------
void mgl_beam(HMGL gr, HCDT tr, HCDT g1, HCDT g2, HCDT a, float r, const char *stl, int flag, int num)
{
	num = num<=0 ? 1 : num;
	for(long i=0;i<num;i++)
	{
		float v = gr->Max.c + (gr->Min.c-gr->Max.c)*(i+1.)/(num+1);
		mgl_beam_val(gr,v,tr,g1,g2,a,r,stl,flag);
	}
}
//-----------------------------------------------------------------------------
void mgl_beam_val_(uintptr_t *gr, float *val, uintptr_t *tr, uintptr_t *g1, uintptr_t *g2, uintptr_t *a, float *r, const char *sch, int *norm,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_beam_val(_GR_, *val,_DA_(tr),_DA_(g1),_DA_(g2),_DA_(a),*r,s,*norm);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_beam_(uintptr_t *gr, uintptr_t *tr, uintptr_t *g1, uintptr_t *g2, uintptr_t *a, float *r, const char *sch, int *norm, int *num,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_beam(_GR_, _DA_(tr), _DA_(g1), _DA_(g2), _DA_(a), *r,s,*norm,*num);	delete []s;	}
//-----------------------------------------------------------------------------
