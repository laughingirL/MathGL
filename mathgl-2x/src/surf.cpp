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
#include "mgl/surf.h"
#include "mgl/data.h"
#include "mgl/eval.h"
//-----------------------------------------------------------------------------
void mgl_mesh_plot(mglBase *gr, long *pos, long n, long m, int how)
{
	int d = gr->MeshNum>0 ? gr->MeshNum+1 : 1;
	register long i,j;
	if(how&1)	for(j=0;j<m;j++)	for(i=0;i<n-1;i+=d)
		gr->line_plot(pos[n*j+i],pos[n*j+i+1]);
	if(how&2)	for(j=0;j<m-1;j+=d)	for(i=0;i<n;i++)
		gr->line_plot(pos[n*j+i],pos[n*j+i+n]);
}
//-----------------------------------------------------------------------------
void mgl_surf_plot(mglBase *gr, long *pos, long n, long m)
{
	register long i,j;
	for(j=0;j<m-1;j++)	for(i=0;i<n-1;i++)
		gr->quad_plot(pos[n*j+i],pos[n*j+i+1],pos[n*j+i+n],pos[n*j+i+n+1]);
}
//-----------------------------------------------------------------------------
//
//	Plot by formulas series
//
//-----------------------------------------------------------------------------
void mgl_fsurf(HMGL gr, const char *eqZ, const char *sch, int n)
{
	if(!gr)	return;
	// TODO: Add strong function variation analisys ???
	if(eqZ==0 || eqZ[0]==0)	return;		// nothing to plot
	if(n<=0)	n=100;
	mglData z(n,n);
	mglFormula *eq = new mglFormula(eqZ);
	register int i,j;
	float dx = (gr->Max.x - gr->Min.x)/(n-1.), dy = (gr->Max.y - gr->Min.y)/(n-1.);
	for(j=0;j<n;j++)	for(i=0;i<n;i++)
		z.a[i+n*j] = eq->Calc(gr->Min.x+i*dx, gr->Min.y+j*dy);
	mgl_surf(gr, &z, sch);
	delete eq;
}
//-----------------------------------------------------------------------------
void mgl_fsurf_xyz(HMGL gr, const char *eqX, const char *eqY, const char *eqZ, const char *sch, int n)
{
	if(!gr)	return;
	// TODO: Add strong function variation analisys ???
	mglData x(n,n), y(n,n), z(n,n);
	if(n<=0)	n=100;
	mglFormula *ex, *ey, *ez;
	ex = new mglFormula(eqX ? eqX : "u");
	ey = new mglFormula(eqY ? eqY : "v");
	ez = new mglFormula(eqZ ? eqZ : "0");
	register int i,j;
	register float u,v;
	for(j=0;j<n;j++)	for(i=0;i<n;i++)
	{
		v = i/(n-1.);	u = j/(n-1.);
		x.a[i+n*j] = ex->Calc(0,v,0,u);
		y.a[i+n*j] = ey->Calc(0,v,0,u);
		z.a[i+n*j] = ez->Calc(0,v,0,u);
	}
	mgl_surf_xy(gr,&x,&y,&z,sch);
	delete ex;	delete ey;	delete ez;
}
//-----------------------------------------------------------------------------
void mgl_fsurf_(uintptr_t *gr, const char *fy, const char *stl, int *n, int ly, int ls)
{	char *s=new char[ly+1];	memcpy(s,fy,ly);	s[ly]=0;
	char *p=new char[ls+1];	memcpy(p,stl,ls);	p[ls]=0;
	mgl_fsurf(_GR_, s, p, *n);	delete []s;		delete []p;	}
//-----------------------------------------------------------------------------
void mgl_fsurf_xyz_(uintptr_t *gr, const char *fx, const char *fy, const char *fz, const char *stl, int *n, int lx, int ly, int lz, int ls)
{
	char *sx=new char[lx+1];	memcpy(sx,fx,lx);	sx[lx]=0;
	char *sy=new char[ly+1];	memcpy(sy,fy,ly);	sy[ly]=0;
	char *sz=new char[lz+1];	memcpy(sz,fz,lz);	sz[lz]=0;
	char *p=new char[ls+1];	memcpy(p,stl,ls);	p[ls]=0;
	mgl_fsurf_xyz(_GR_, sx, sy, sz, p, *n);
	delete []sx;	delete []sy;	delete []sz;	delete []p;
}
//-----------------------------------------------------------------------------
//
//	Mesh series
//
//-----------------------------------------------------------------------------
void mgl_mesh_xy(HMGL gr, HCDT x, HCDT y, HCDT z, const char *sch)
{
	if(!gr)	return;
	register long i,j,k,n=z->GetNx(),m=z->GetNy();
	if(x->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"Mesh");	return;	}
	if(n<2 || m<2)		{	gr->SetWarn(mglWarnLow,"Mesh");	return;	}
	if(y->GetNx()!=m && (x->GetNy()!=m || y->GetNx()!=n || y->GetNy()!=m))
	{	gr->SetWarn(mglWarnDim);	return;	};
	static int cgid=1;	gr->StartGroup("Mesh",cgid++);
	gr->SetPenPal("-");
	long ss = gr->AddTexture(sch);
	long *pos = new long[n*m];
	gr->ReserveC(n*m*z->GetNz());

	mglPoint p;
	float c;
	for(k=0;k<z->GetNz();k++)
	{
		for(j=0;j<m;j++)	for(i=0;i<n;i++)
		{
			p = mglPoint(GetX(x,i,j,k).x, GetY(y,i,j,k).x, z->v(i,j,k));
			c = gr->GetC(ss,p.z);	gr->ScalePoint(p);
			pos[i+n*j] = gr->AddPntC(p,c);
		}
		mgl_mesh_plot(gr,pos,n,m,3);
	}
	delete []pos;	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_mesh(HMGL gr, HCDT z, const char *sch)
{
	if(!gr)	return;
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"Mesh");	return;	}
	mglData x(z->GetNx()), y(z->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_mesh_xy(gr,&x,&y,z,sch);
}
//-----------------------------------------------------------------------------
void mgl_mesh_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_mesh_xy(_GR_, _DA_(x), _DA_(y), _DA_(a), s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_mesh_(uintptr_t *gr, uintptr_t *a, const char *sch,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_mesh(_GR_, _DA_(a), s);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Fall series
//
//-----------------------------------------------------------------------------
void mgl_fall_xy(HMGL gr, HCDT x, HCDT y, HCDT z, const char *sch)
{
	if(!gr)	return;
	register long i,j,k,n=z->GetNx(),m=z->GetNy();
	if(x->GetNx()!=z->GetNx())		{	gr->SetWarn(mglWarnDim,"Fall");	return;	}
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"Fall");	return;	}
	if(y->GetNx()!=z->GetNy() && (x->GetNy()!=z->GetNy() || y->GetNx()!=z->GetNx() || y->GetNy()!=z->GetNy()))
	{	gr->SetWarn(mglWarnDim);	return;	}
	static int cgid=1;	gr->StartGroup("Fall",cgid++);
	gr->SetPenPal("-");
	long ss = gr->AddTexture(sch);
	long *pos = new long[n*m];
	gr->ReserveC(n*m*z->GetNz());

	mglPoint p;
	float c;
	for(k=0;k<z->GetNz();k++)
	{
		for(j=0;j<m;j++)	for(i=0;i<n;i++)
		{
			p = mglPoint(GetX(x,i,j,k).x, GetY(y,i,j,k).x, z->v(i,j,k));
			c = gr->GetC(ss,p.z);	gr->ScalePoint(p);
			pos[i+n*j] = gr->AddPntC(p,c);
		}
		mgl_mesh_plot(gr,pos,n,m, (sch && strchr(sch,'x')) ? 2:1);
	}
	delete []pos;	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_fall(HMGL gr, HCDT z, const char *sch)
{
	if(!gr)	return;
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"Fall");	return;	}
	mglData x(z->GetNx()), y(z->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_fall_xy(gr,&x,&y,z,sch);
}
//-----------------------------------------------------------------------------
void mgl_fall_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_fall_xy(_GR_, _DA_(x), _DA_(y), _DA_(a), s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_fall_(uintptr_t *gr, uintptr_t *a, const char *sch,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_fall(_GR_, _DA_(a), s);		delete []s;	}
//-----------------------------------------------------------------------------
//
//	Grid series
//
//-----------------------------------------------------------------------------
void mgl_grid_xy(HMGL gr, HCDT x, HCDT y, HCDT z, const char *sch,float zVal)
{
	if(!gr)	return;
	register long i,j,k,n=z->GetNx(),m=z->GetNy();
	if(x->GetNx()!=z->GetNx())		{	gr->SetWarn(mglWarnDim,"Grid");	return;	}
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"Grid");	return;	}
	if(y->GetNx()!=z->GetNy() && (x->GetNy()!=z->GetNy() || y->GetNx()!=z->GetNx() || y->GetNy()!=z->GetNy()))
	{	gr->SetWarn(mglWarnDim);	return;	}
	static int cgid=1;	gr->StartGroup("Grid",cgid++);
	if(isnan(zVal))	zVal = gr->Min.z;
	gr->SetPenPal(sch?sch:"k-");
	long *pos = new long[n*m];
	gr->ReserveC(n*m*z->GetNz());

	mglPoint p;
	for(k=0;k<z->GetNz();k++)
	{
		if(z->GetNz()>1)	zVal = gr->Min.z+(gr->Max.z-gr->Min.z)*float(k)/(z->GetNz()-1);
		for(j=0;j<m;j++)	for(i=0;i<n;i++)
		{
			p = mglPoint(GetX(x,i,j,k).x, GetY(y,i,j,k).x, zVal);
			gr->ScalePoint(p);	pos[i+n*j] = gr->AddPntC(p,gr->CDef);
		}
		mgl_mesh_plot(gr,pos,n,m,3);
	}
	delete []pos;	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_grid(HMGL gr, HCDT z,const char *sch,float zVal)
{
	if(!gr || !z)	return;
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"Grid");	return;	}
	mglData x(z->GetNx()), y(z->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_grid_xy(gr,&x,&y,z,sch,zVal);
}
//-----------------------------------------------------------------------------
void mgl_grid_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch,float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_grid_xy(_GR_,_DA_(x), _DA_(y), _DA_(a), s, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_grid_(uintptr_t *gr, uintptr_t *a,const char *sch,float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_grid(_GR_, _DA_(a), s, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Surf series
//
//-----------------------------------------------------------------------------
void mgl_surf_xy(HMGL gr, HCDT x, HCDT y, HCDT z, const char *sch)
{
	if(!gr)	return;
	register long i,j,k,n=z->GetNx(),m=z->GetNy();
	if(x->GetNx()!=n)	{	gr->SetWarn(mglWarnDim,"Surf");	return;	}
	if(n<2 || m<2)		{	gr->SetWarn(mglWarnLow,"Surf");	return;	}
	if(y->GetNx()!=m && (x->GetNy()!=m || y->GetNx()!=n || y->GetNy()!=m))
	{	gr->SetWarn(mglWarnDim);	return;	};
	static int cgid=1;	gr->StartGroup("Surf",cgid++);
	long ss = gr->AddTexture(sch);
	long *pos = new long[n*m];
	gr->ReserveN(n*m*z->GetNz());

	mglPoint p,q,s,xx,yy;
	float c;
	for(k=0;k<z->GetNz();k++)
	{
		for(j=0;j<m;j++)	for(i=0;i<n;i++)
		{
			xx = GetX(x,i,j,k);		yy = GetY(y,i,j,k);
			p = mglPoint(xx.x, yy.x, z->v(i,j,k));
			q = mglPoint(xx.y, yy.y, z->dvx(i,j,k));
			s = mglPoint(xx.z, yy.z, z->dvy(i,j,k));
			c = gr->GetC(ss,p.z);	gr->ScalePoint(p);
			pos[i+n*j] = gr->AddPntN(p,c,q^s);
		}
		mgl_surf_plot(gr,pos,n,m);
		if(sch && strchr(sch,'#'))
		{
			gr->ReserveC(n*m);	gr->SetPenPal("k-");
			for(i=0;i<n*m;i++)
				pos[i] = gr->CopyNtoC(pos[i],gr->CDef);
			mgl_mesh_plot(gr,pos,n,m,3);
		}
	}
	delete []pos;	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_surf(HMGL gr, HCDT z, const char *sch)
{
	if(!gr)	return;
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"Surf");	return;	}
	mglData x(z->GetNx()), y(z->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_surf_xy(gr,&x,&y,z,sch);
}
//-----------------------------------------------------------------------------
void mgl_surf_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_surf_xy(_GR_, _DA_(x), _DA_(y), _DA_(a), s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_surf_(uintptr_t *gr, uintptr_t *a, const char *sch,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_surf(_GR_, _DA_(a), s);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Belt series
//
//-----------------------------------------------------------------------------
void mgl_belt_xy(HMGL gr, HCDT x, HCDT y, HCDT z, const char *sch)
{
	if(!gr)	return;
	register long i,j,k,n=z->GetNx(),m=z->GetNy();
	if(x->GetNx()!=z->GetNx())		{	gr->SetWarn(mglWarnDim,"Belt");	return;	}
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"Belt");	return;	}
	if(y->GetNx()!=z->GetNy() && (x->GetNy()!=z->GetNy() || y->GetNx()!=z->GetNx() || y->GetNy()!=z->GetNy()))
	{	gr->SetWarn(mglWarnDim);	return;	}
	static int cgid=1;	gr->StartGroup("Belt",cgid++);

	long ss = gr->AddTexture(sch);
	long *pos = new long[2*(n>m?n:m)];
	gr->ReserveN(2*n*m*z->GetNz());
	bool how = !(sch && strchr(sch,'x'));

	mglPoint p1,p2,q,s,xx,yy;
	float c;
	for(k=0;k<z->GetNz();k++)
	{
		if(how)	for(i=0;i<n-1;i++)
		{
			for(j=0;j<m;j++)
			{
				xx = GetX(x,i,j,k);		yy = GetY(y,i,j,k);
				p1 = mglPoint(xx.x, yy.x, z->v(i,j,k));
				s = mglPoint(xx.z, yy.z, z->dvy(i,j,k));
				q = mglPoint(xx.y, yy.y, 0);	s = q^s;
				c = gr->GetC(ss,p1.z);
				p2 = mglPoint(GetX(x,i+1,j,k).x,GetY(y,i+1,j,k).x,p1.z);
				gr->ScalePoint(p1);	pos[2*j] = gr->AddPntN(p1,c,s);
				gr->ScalePoint(p2);	pos[2*j+1]=gr->AddPntN(p2,c,s);
			}
			mgl_surf_plot(gr,pos,2,m);
		}
		else	for(j=0;j<m-1;j++)
		{
			for(i=0;i<n;i++)	// ñîçäàåì ìàññèâ òî÷åê
			{
				xx = GetX(x,i,j,k);		yy = GetY(y,i,j,k);
				p1 = mglPoint(xx.x, yy.x, z->v(i,j,k));
				q = mglPoint(xx.y, yy.y, z->dvx(i,j,k));
				s = mglPoint(xx.z, yy.z, 0);	s = q^s;
				c = gr->GetC(ss,p1.z);
				p2 = mglPoint(GetX(x,i,j+1,k).x,GetY(y,i,j+1,k).x,p1.z);
				gr->ScalePoint(p1);	pos[2*i] = gr->AddPntN(p1,c,s);
				gr->ScalePoint(p2);	pos[2*i+1]=gr->AddPntN(p2,c,s);
			}
			mgl_surf_plot(gr,pos,2,n);
		}
	}
	delete []pos; gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_belt(HMGL gr, HCDT z, const char *sch)
{
	if(!gr)	return;
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"Belt");	return;	}
	mglData x(z->GetNx()), y(z->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_belt_xy(gr,&x,&y,z,sch);
}
//-----------------------------------------------------------------------------
void mgl_belt_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_belt_xy(_GR_, _DA_(x), _DA_(y), _DA_(a), s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_belt_(uintptr_t *gr, uintptr_t *a, const char *sch,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_belt(_GR_, _DA_(a), s);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Dens series
//
//-----------------------------------------------------------------------------
void mgl_dens_xy(HMGL gr, HCDT x, HCDT y, HCDT z, const char *sch,float zVal)
{
	if(!gr || !z || !x || !y)	return;
	register long i,j,k,n=z->GetNx(),m=z->GetNy();
	if(x->GetNx()!=z->GetNx())		{	gr->SetWarn(mglWarnDim,"Dens");	return;	}
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"Dens");	return;	}
	if(y->GetNx()!=z->GetNy() && (x->GetNy()!=z->GetNy() || y->GetNx()!=z->GetNx() || y->GetNy()!=z->GetNy()))
	{	gr->SetWarn(mglWarnDim);	return;	}
	static int cgid=1;	gr->StartGroup("Dens",cgid++);
	if(isnan(zVal))	zVal = gr->Min.z;

	long ss = gr->AddTexture(sch);
	long *pos = new long[n*m];
	gr->ReserveN(n*m*z->GetNz());

	mglPoint p,s=mglPoint(0,0,1);
	float zz, c;
	for(k=0;k<z->GetNz();k++)
	{
		if(z->GetNz()>1)
			zVal = gr->Min.z+(gr->Max.z-gr->Min.z)*float(k)/(z->GetNz()-1);
		for(j=0;j<m;j++)	for(i=0;i<n;i++)	// ñîçäàåì ìàññèâ òî÷åê
		{
			p = mglPoint(GetX(x,i,j,k).x, GetY(y,i,j,k).x, zVal);
			zz = z->v(i,j,k);	c = gr->GetC(ss,zz);
			gr->ScalePoint(p);	if(isnan(zz))	p.x = NAN;
			pos[i+n*j] = gr->AddPntN(p,c,s);
		}
		mgl_surf_plot(gr,pos,n,m);
		if(sch && strchr(sch,'#'))
		{
			gr->ReserveC(n*m);	gr->SetPenPal("k-");
			for(i=0;i<n*m;i++)
				pos[i] = gr->CopyNtoC(pos[i],gr->CDef);
			mgl_mesh_plot(gr,pos,n,m,3);
		}
	}
	delete []pos;	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_dens(HMGL gr, HCDT z, const char *sch,float zVal)
{
	if(!gr || !z)	return;
	mglData x(z->GetNx()), y(z->GetNy());
	x.Fill(gr->Min.x, gr->Max.x);
	y.Fill(gr->Min.y, gr->Max.y);
	mgl_dens_xy(gr,&x,&y,z,sch,zVal);
}
//-----------------------------------------------------------------------------
void mgl_dens_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch,float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_dens_xy(_GR_,_DA_(x), _DA_(y), _DA_(a), s, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_dens_(uintptr_t *gr, uintptr_t *a, const char *sch,float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_dens(_GR_,_DA_(a), s, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	STFA series
//
//-----------------------------------------------------------------------------
void mgl_stfa_xy(HMGL gr, HCDT x, HCDT y, HCDT re, HCDT im, int dn, const char *sch, float zVal)
{	mglData tmp(mglSTFA(*re,*im,dn,'x'));	mgl_dens_xy(gr,x,y,&tmp,sch,zVal);	}
//-----------------------------------------------------------------------------
void mgl_stfa(HMGL gr, HCDT re, HCDT im, int dn, const char *sch, float zVal)
{	mglData tmp(mglSTFA(*re,*im,dn,'x'));	mgl_dens(gr,&tmp,sch,zVal);	}
//-----------------------------------------------------------------------------
void mgl_stfa_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *re, uintptr_t *im, int *dn, const char *sch, float *zVal, int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_stfa_xy(_GR_,_DA_(x), _DA_(y), _DA_(re), _DA_(im), *dn, s, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_stfa_(uintptr_t *gr, uintptr_t *re, uintptr_t *im, int *dn, const char *sch, float *zVal, int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_stfa(_GR_,_DA_(re), _DA_(im), *dn, s, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	SurfC series
//
//-----------------------------------------------------------------------------
void mgl_surfc_xy(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT c, const char *sch)
{
	if(!gr)	return;
	register long i,j,k,n=z->GetNx(),m=z->GetNy();
	if(x->GetNx()!=z->GetNx())		{	gr->SetWarn(mglWarnDim,"SurfC");	return;	}
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"SurfC");	return;	}
	if(z->GetNx()*z->GetNy()*z->GetNz()!=c->GetNx()*c->GetNy()*c->GetNz())
	{	gr->SetWarn(mglWarnDim);	return;	}
	if(y->GetNx()!=z->GetNy() && (x->GetNy()!=z->GetNy() || y->GetNx()!=z->GetNx() || y->GetNy()!=z->GetNy()))
	{	gr->SetWarn(mglWarnDim);	return;	}
	static int cgid=1;	gr->StartGroup("SurfC",cgid++);

	long ss = gr->AddTexture(sch);
	long *pos = new long[n*m];
	gr->ReserveN(n*m*z->GetNz());
	float col;

	mglPoint p,q,s,xx,yy;
	for(k=0;k<z->GetNz();k++)
	{
		for(j=0;j<m;j++)	for(i=0;i<n;i++)
		{
			xx = GetX(x,i,j,k);		yy = GetY(y,i,j,k);
			p = mglPoint(xx.x, yy.x, z->v(i,j,k));
			q = mglPoint(xx.y, yy.y, z->dvx(i,j,k));
			s = mglPoint(xx.z, yy.z, z->dvy(i,j,k));
			col = gr->GetC(ss,c->v(i,j,k));
			gr->ScalePoint(p);
			pos[i+n*j] = gr->AddPntN(p,col,q^s);
		}
		mgl_surf_plot(gr,pos,n,m);
		if(sch && strchr(sch,'#'))
		{
			gr->ReserveC(n*m);	gr->SetPenPal("k-");
			for(i=0;i<n*m;i++)
				pos[i] = gr->CopyNtoC(pos[i],gr->CDef);
			mgl_mesh_plot(gr,pos,n,m,3);
		}
	}
	delete []pos;	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_surfc(HMGL gr, HCDT z, HCDT c, const char *sch)
{
	if(!gr)	return;
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"SurfC");	return;	}
	mglData x(z->GetNx()), y(z->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_surfc_xy(gr,&x,&y,z,c,sch);
}
//-----------------------------------------------------------------------------
void mgl_surfc_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *sch,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_surfc_xy(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(a), s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_surfc_(uintptr_t *gr, uintptr_t *z, uintptr_t *a, const char *sch,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_surfc(_GR_, _DA_(z), _DA_(a), s);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	SurfA series
//
//-----------------------------------------------------------------------------
void mgl_surfa_xy(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT c, const char *sch)
{
	if(!gr)	return;
	register long i,j;
	long k,n=z->GetNx(),m=z->GetNy();
	if(x->GetNx()!=z->GetNx())		{	gr->SetWarn(mglWarnDim,"SurfA");	return;	}
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"SurfA");	return;	}
	if(z->GetNx()*z->GetNy()*z->GetNz()!=c->GetNx()*c->GetNy()*c->GetNz())
	{	gr->SetWarn(mglWarnDim);	return;	}
	if(y->GetNx()!=z->GetNy() && (x->GetNy()!=z->GetNy() || y->GetNx()!=z->GetNx() || y->GetNy()!=z->GetNy()))
	{	gr->SetWarn(mglWarnDim);	return;	}
	static int cgid=1;	gr->StartGroup("SurfA",cgid++);

	long ss = gr->AddTexture(sch);
	long *pos = new long[n*m];
	gr->ReserveN(n*m*z->GetNz());

	mglPoint p,q,s,xx,yy;
	for(k=0;k<z->GetNz();k++)
	{
		for(j=0;j<m;j++)	for(i=0;i<n;i++)
		{
			xx = GetX(x,i,j,k);		yy = GetY(y,i,j,k);
			p = mglPoint(xx.x, yy.x, z->v(i,j,k));
			q = mglPoint(xx.y, yy.y, z->dvx(i,j,k));
			s = mglPoint(xx.z, yy.z, z->dvy(i,j,k));
			// NOTE: before was c.a ~ a^2 !!!
			gr->ScalePoint(p);
			pos[i+n*j] = gr->AddPntN(p,gr->GetC(ss,p.z),q^s,gr->GetA(c->v(i,j,k)));
		}
		mgl_surf_plot(gr,pos,n,m);
		if(sch && strchr(sch,'#'))
		{
			gr->ReserveC(n*m);	gr->SetPenPal("k-");
			for(i=0;i<n*m;i++)
				pos[i] = gr->CopyNtoC(pos[i],gr->CDef);
			mgl_mesh_plot(gr,pos,n,m,3);
		}
	}
	delete []pos;	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_surfa(HMGL gr, HCDT z, HCDT c, const char *sch)
{
	if(!gr)	return;
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"SurfC");	return;	}
	mglData x(z->GetNx()), y(z->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_surfa_xy(gr,&x,&y,z,c,sch);
}
//-----------------------------------------------------------------------------
void mgl_surfa_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *sch,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_surfa_xy(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(a), s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_surfa_(uintptr_t *gr, uintptr_t *z, uintptr_t *a, const char *sch,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_surfa(_GR_, _DA_(z), _DA_(a), s);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Boxs series
//
//-----------------------------------------------------------------------------
void mgl_boxs_xy(HMGL gr, HCDT x, HCDT y, HCDT z, const char *sch,float zVal)
{
	if(!gr)	return;
	register long i,j,k,n=z->GetNx(),m=z->GetNy();
	if(x->GetNx()!=z->GetNx())		{	gr->SetWarn(mglWarnDim,"Boxs");	return;	}
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"Boxs");	return;	}
	if(y->GetNx()!=z->GetNy() && (x->GetNy()!=z->GetNy() || y->GetNx()!=z->GetNx() || y->GetNy()!=z->GetNy()))
	{	gr->SetWarn(mglWarnDim);	return;	}
	static int cgid=1;	gr->StartGroup("Boxs",cgid++);
	if(isnan(zVal))	zVal = gr->GetOrgZ('x');

	long ss = gr->AddTexture(sch);
	gr->ReserveN(12*n*m*z->GetNz());

	mglPoint p1,p2,p3,p4,q,s,t=mglPoint(0,0,1),xx,yy;
	float zz,z1,z2,x1,y1,c;
	long k1,k2,k3,k4;
	for(k=0;k<z->GetNz();k++)
	{
		for(i=0;i<n-1;i++)	for(j=0;j<m-1;j++)
		{
			zz = z->v(i,j,k);		c  = gr->GetC(ss,zz);
			xx = GetX(x,i,j,k);		yy = GetY(y,i,j,k);
			x1 = GetX(x,i+1,j,k).x;	y1 = GetY(y,i,j+1,k).x;
			z1 = z->v(i+1,j,k);		z2 = z->v(i,j+1,k);
			q = mglPoint(xx.y,yy.y,0);
			s = mglPoint(xx.z,yy.z,0);
			p1 = mglPoint(xx.x,yy.x,zz);	gr->ScalePoint(p1);	k1 = gr->AddPntN(p1,c,t);
			p2 = mglPoint(x1,yy.x,zz);		gr->ScalePoint(p2);	k2 = gr->AddPntN(p2,c,t);
			p3 = mglPoint(xx.x,y1,zz);		gr->ScalePoint(p3);	k3 = gr->AddPntN(p3,c,t);
			p4 = mglPoint(x1,y1,zz);		gr->ScalePoint(p4);	k4 = gr->AddPntN(p4,c,t);
			gr->quad_plot(k1,k2,k3,k4);
			p1 = mglPoint(x1,yy.x,zz);		gr->ScalePoint(p1);	k1 = gr->AddPntN(p1,c,q);
			p2 = mglPoint(x1,y1,zz);		gr->ScalePoint(p2);	k2 = gr->AddPntN(p2,c,q);
			p3 = mglPoint(x1,yy.x,z1);		gr->ScalePoint(p3);	k3 = gr->AddPntN(p3,c,q);
			p4 = mglPoint(x1,y1,z1);		gr->ScalePoint(p4);	k4 = gr->AddPntN(p4,c,q);
			gr->quad_plot(k1,k2,k3,k4);
			p1 = mglPoint(xx.x,y1,zz);		gr->ScalePoint(p1);	k1 = gr->AddPntN(p1,c,s);
			p2 = mglPoint(x1,y1,zz);		gr->ScalePoint(p2);	k2 = gr->AddPntN(p2,c,s);
			p3 = mglPoint(xx.x,y1,z2);		gr->ScalePoint(p3);	k3 = gr->AddPntN(p3,c,s);
			p4 = mglPoint(x1,y1,z2);		gr->ScalePoint(p4);	k4 = gr->AddPntN(p4,c,s);
			gr->quad_plot(k1,k2,k3,k4);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_boxs(HMGL gr, HCDT z, const char *sch,float zVal)
{
	if(!gr)	return;
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"Boxs");	return;	}
	mglData x(z->GetNx()), y(z->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_boxs_xy(gr,&x,&y,z,sch,zVal);
}
//-----------------------------------------------------------------------------
void mgl_boxs_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch,float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_boxs_xy(_GR_, _DA_(x), _DA_(y), _DA_(a), s,*zVal);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_boxs_(uintptr_t *gr, uintptr_t *a, const char *sch,float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_boxs(_GR_, _DA_(a), s,*zVal);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Tile series
//
//-----------------------------------------------------------------------------
void mgl_tile_xy(HMGL gr, HCDT x, HCDT y, HCDT z, const char *sch)
{
	if(!gr)	return;
	register long i,j,k,n=z->GetNx(),m=z->GetNy();
	if(x->GetNx()!=z->GetNx())		{	gr->SetWarn(mglWarnDim,"Tile");	return;	}
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"Tile");	return;	}
	if(y->GetNx()!=z->GetNy() && (x->GetNy()!=z->GetNy() || y->GetNx()!=z->GetNx() || y->GetNy()!=z->GetNy()))
	{	gr->SetWarn(mglWarnDim);	return;	}
	static int cgid=1;	gr->StartGroup("Tile",cgid++);

	long ss = gr->AddTexture(sch);
	gr->ReserveN(4*n*m*z->GetNz());

	mglPoint p1,p2,p3,p4,s=mglPoint(0,0,1);
	float zz,x1,x2,y1,y2,c;
	long k1,k2,k3,k4;
	for(k=0;k<z->GetNz();k++)
	{
		for(j=0;j<m-1;j++)	for(i=0;i<n-1;i++)
		{
			zz = z->v(i,j,k);		c = gr->GetC(ss,zz);
			x1 = GetX(x,i,j,k).x;	y1 = GetY(y,i,j,k).x;
			x2 = GetX(x,i+1,j,k).x;	y2 = GetY(y,i,j+1,k).x;
			p1 = mglPoint(x1,y1,zz);	gr->ScalePoint(p1);	k1 = gr->AddPntN(p1,c,s);
			p2 = mglPoint(x2,y1,zz);	gr->ScalePoint(p2);	k2 = gr->AddPntN(p2,c,s);
			p3 = mglPoint(x1,y2,zz);	gr->ScalePoint(p3);	k3 = gr->AddPntN(p3,c,s);
			p4 = mglPoint(x2,y2,zz);	gr->ScalePoint(p4);	k4 = gr->AddPntN(p4,c,s);
			gr->quad_plot(k1,k2,k3,k4);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_tile(HMGL gr, HCDT z, const char *sch)
{
	if(!gr)	return;
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"Tile");	return;	}
	mglData x(z->GetNx()), y(z->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_tile_xy(gr,&x,&y,z,sch);
}
//-----------------------------------------------------------------------------
void mgl_tile_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_tile_xy(_GR_, _DA_(x), _DA_(y), _DA_(a), s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_tile_(uintptr_t *gr, uintptr_t *a, const char *sch,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_tile(_GR_, _DA_(a), s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_tiles_xy(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT s, const char *sch)
{
	if(!gr)	return;
	register long i,j,k,n=z->GetNx(),m=z->GetNy();
	if(x->GetNx()!=z->GetNx() || s->GetNx()*s->GetNy()*s->GetNz()!=z->GetNx()*z->GetNy()*z->GetNz())
	{	gr->SetWarn(mglWarnDim,"Tile");	return;	}
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"Tile");	return;	}
	if(y->GetNx()!=z->GetNy() && (x->GetNy()!=z->GetNy() || y->GetNx()!=z->GetNx() || y->GetNy()!=z->GetNy()))
	{	gr->SetWarn(mglWarnDim);	return;	}
	static int cgid=1;	gr->StartGroup("TileS",cgid++);

	long cc = gr->AddTexture(sch);
	gr->ReserveN(4*n*m*z->GetNz());

	mglPoint p1,p2,p3,p4,t=mglPoint(0,0,1);
	float zz,x1,x2,x3,x4,y1,y2,y3,y4,ss,sm,c;
	long k1,k2,k3,k4;
	for(k=0;k<z->GetNz();k++)
	{
		for(j=0;j<m-1;j++)	for(i=0;i<n-1;i++)
		{
			zz = z->v(i,j,k);	c = gr->GetC(cc,zz);
			// TODO check it!!!
			ss = (1-gr->GetA(s->v(i,j,k)))/2;	sm = 1-ss;

			x1 = GetX(x,i,j,k).x;		y1 = GetY(y,i,j,k).x;
			x2 = GetX(x,i+1,j,k).x-x1;	y2 = GetY(y,i+1,j,k).x-y1;
			x4 = GetX(x,i,j+1,k).x-x1;	y4 = GetY(y,i,j+1,k).x-y1;
			x3 = GetX(x,i+1,j+1,k).x-x2-x4-x1;
			y3 = GetY(y,i+1,j+1,k).x-y2-y4-y1;

			p1 = mglPoint(x1+x2*ss+x4*ss+x3*ss*ss, y1+y2*ss+y4*ss+y3*ss*ss, zz);
			gr->ScalePoint(p1);	k1 = gr->AddPntN(p1,c,t);
			p2 = mglPoint(x1+x2*sm+x4*ss+x3*ss*sm, y1+y2*sm+y4*ss+y3*ss*sm, zz);
			gr->ScalePoint(p2);	k2 = gr->AddPntN(p2,c,t);
			p3 = mglPoint(x1+x2*sm+x4*sm+x3*sm*sm, y1+y2*sm+y4*sm+y3*sm*sm, zz);
			gr->ScalePoint(p3);	k3 = gr->AddPntN(p3,c,t);
			p4 = mglPoint(x1+x2*ss+x4*sm+x3*ss*sm, y1+y2*ss+y4*sm+y3*ss*sm);
			gr->ScalePoint(p4);	k4 = gr->AddPntN(p4,c,t);
			gr->quad_plot(k1,k2,k3,k4);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_tiles(HMGL gr, HCDT z, HCDT s, const char *sch)
{
	if(!gr)	return;
	if(z->GetNx()<2 || z->GetNy()<2){	gr->SetWarn(mglWarnLow,"Tile");	return;	}
	mglData x(z->GetNx()), y(z->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_tiles_xy(gr,&x,&y,z,s,sch);
}
//-----------------------------------------------------------------------------
void mgl_tiles_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *a, uintptr_t *r, const char *sch,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_tiles_xy(_GR_, _DA_(x), _DA_(y), _DA_(a), _DA_(r), s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_tiles_(uintptr_t *gr, uintptr_t *a, uintptr_t *r, const char *sch,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_tiles(_GR_, _DA_(a), _DA_(r), s);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Map series
//
//-----------------------------------------------------------------------------
void mgl_map_xy(HMGL gr, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch, int ks, int pnts)
{
	if(!gr)	return;
	register long i,j,n=ax->GetNx(),m=ax->GetNy();
	if(n*m!=ay->GetNx()*ay->GetNy())	{	gr->SetWarn(mglWarnDim,"Map");	return;	}
	if(n<2 || m<2)			{	gr->SetWarn(mglWarnLow,"Map");	return;	}
	bool both = x->GetNx()==n && y->GetNx()==n && x->GetNy()==m && y->GetNy()==m;
	if(!(both || (x->GetNx()==n && y->GetNx()==m)))
	{	gr->SetWarn(mglWarnDim,"Map");	return;	}
	static int cgid=1;	gr->StartGroup("Map",cgid++);

	long ss = gr->AddTexture((sch && *sch)?sch:"rgb",2);
	if(ks<0 || ks>=ay->GetNz() || ks>=ax->GetNz())	ks = 0;
	long s = both ? n:1, s1, s2;

	float xdy,xdx,ydx,ydy,xx,yy;
	mglPoint p,t=mglPoint(NAN);
	long *pos = new long[n*m];
	gr->ReserveN(n*m);

	for(j=0;j<m;j++)	for(i=0;i<n;i++)
	{
		s1 = i>0 ? 1:0;		s2 = i<n-1 ? 1:0;
		xdx = (ax->v(i+s2,j,ks)-ax->v(i-s1,j,ks))/(GetX(x,i+s2,j,ks).x-GetX(x,i-s1,j,ks).x);
		ydx = (ay->v(i+s2,j,ks)-ay->v(i-s1,j,ks))/(GetX(x,i+s2,j,ks).x-GetX(x,i-s1,j,ks).x);
		s1 = j>0 ? s:0;		s2 = j<m-1 ? s:0;
		xdy = (ax->v(i,j+s2,ks)-ax->v(i,j-s1,ks))/(GetY(y,i,j+s2,ks).x-GetY(y,i,j-s1,ks).x);
		ydy = (ay->v(i,j+s2,ks)-ay->v(i,j-s1,ks))/(GetY(y,i,j+s2,ks).x-GetY(y,i,j-s1,ks).x);
		xdx = xdx*ydy - xdy*ydx;	// Jacobian

		p = mglPoint(ax->v(i,j,ks), ay->v(i,j,ks), xdx);
		xx = (x->v(i,j,ks) - gr->Min.x)/(gr->Max.x - gr->Min.x);
		yy = (y->v(i,j,ks) - gr->Min.y)/(gr->Max.y - gr->Min.y);
		gr->ScalePoint(p);
		pos[i+n*j] = gr->AddPntN(p,gr->GetC(ss,xx),t,yy);
	}
	if(pnts)
	{
		gr->ReserveC(n*m);
		for(i=0;i<n*m;i++)
		gr->mark_plot(gr->CopyNtoC(pos[i]),'.',-1);
	}
	else	mgl_surf_plot(gr,pos,n,m);
	delete []pos;	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_map(HMGL gr, HCDT ax, HCDT ay, const char *sch, int ks, int pnts)
{

	if(ax->GetNx()<2 || ax->GetNy()<2)	{	gr->SetWarn(mglWarnLow,"Map");	return;	}
	mglData x(ax->GetNx()), y(ax->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_map_xy(gr,&x,&y,ax,ay,sch,ks,pnts);
}
//-----------------------------------------------------------------------------
void mgl_map_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *a, uintptr_t *b, const char *sch, int *ks, int *pnts,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_map_xy(_GR_, _DA_(x), _DA_(y), _DA_(a), _DA_(b), s, *ks, *pnts);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_map_(uintptr_t *gr, uintptr_t *a, uintptr_t *b, const char *sch, int *ks, int *pnts,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_map(_GR_, _DA_(a), _DA_(b), s, *ks, *pnts);	delete []s;	}
//-----------------------------------------------------------------------------
