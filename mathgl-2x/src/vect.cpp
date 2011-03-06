/***************************************************************************
 * vect.cpp is part of Math Graphic Library
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
#include "mgl/vect.h"
#include "mgl/eval.h"
#include "mgl/data.h"
#include <stdlib.h>
//-----------------------------------------------------------------------------
//
//	Traj series
//
//-----------------------------------------------------------------------------
void mgl_traj_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT ax, HCDT ay, HCDT az, const char *sch,float len)
{
	long m,mx,my,mz,nx,ny,nz,n=ax->GetNx(),pal;
	if(n<2)	{	gr->SetWarn(mglWarnLow,"Traj");	return;	}
	if(n!=x->GetNx() || z->GetNx()!=n || y->GetNx()!=n || ay->GetNx()!=n || az->GetNx()!=n)
	{	gr->SetWarn(mglWarnDim,"Traj");	return;	}
	static int cgid=1;	gr->StartGroup("Traj",cgid++);

	register long i, j;
	// find maximum
	i = ax->GetNy()>ay->GetNy() ? ax->GetNy():ay->GetNy();	j = z->GetNy()>az->GetNy() ? z->GetNy():az->GetNy();
	m = x->GetNy()>y->GetNy() ? x->GetNy():y->GetNy();		if(i>m)	m=i;	if(j>m)	m=j;
	gr->SetPenPal(sch,&pal);	gr->ReserveC(4*n*m);

	float dx,dy,dz,dd,da,xm=0;
	mglPoint p1,p2;
	for(j=0;j<m;j++)	for(i=0;i<n;i++)	// find maximal amplitude of vector field
	{
		mx = j<ax->GetNy() ? j:0;	my = j<ay->GetNy() ? j:0;	mz = j<az->GetNy() ? j:0;
		da = sqrt(ax->v(i,mx)*ax->v(i,mx)+ay->v(i,my)*ay->v(i,my)+az->v(i,mz)*az->v(i,mz));
		xm = xm>da ? xm : da;
	}
	xm = 1./(xm ? sqrt(xm):1);
	for(j=0;j<m;j++) // start prepare arrows
	{
		gr->NextColor(pal);
		for(i=0;i<n;i++)
		{
			nx = j<x->GetNy() ? j:0;	ny = j<y->GetNy() ? j:0;	nz = j<z->GetNy() ? j:0;
			mx = j<ax->GetNy() ? j:0;	my = j<ay->GetNy() ? j:0;	mz = j<az->GetNy() ? j:0;
			da = sqrt(ax->v(i,mx)*ax->v(i,mx)+ay->v(i,my)*ay->v(i,my)+az->v(i,mz)*az->v(i,mz));
			if(len==0)
			{
				if(i<n-1)
				{	dx=x->v(i+1,nx)-x->v(i,nx);	dy=y->v(i+1,ny)-y->v(i,ny);	dz=z->v(i+1,nz)-z->v(i,nz);	}
				else
				{	dx=x->v(i,nx)-x->v(i-1,nx);	dy=y->v(i,ny)-y->v(i-1,ny);	dz=z->v(i,nz)-z->v(i-1,nz);	}
				dd = da ? 1/da : 0;		dd *= sqrt(dx*dx+dy*dy+dz*dz);
			}
			else dd = len;

			p1 = mglPoint(x->v(i,nx), y->v(i,ny), z->v(i,nz));
			p2 = mglPoint(x->v(i,nx)+dd*ax->v(i,mx), y->v(i,ny)+dd*ay->v(i,my), z->v(i,nz)+dd*az->v(i,mz));
			gr->ScalePoint(p1);		nx = gr->AddPntC(p1,gr->CDef);
			gr->ScalePoint(p2);		ny = gr->AddPntC(p2,gr->CDef);
			gr->vect_plot(nx,ny);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_traj_xy(HMGL gr, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch,float zVal,float len)
{
	if(ax->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"Traj");	return;	}
	if(x->GetNx()!=ax->GetNx() || y->GetNx()!=ax->GetNx() || ay->GetNx()!=ax->GetNx())
	{	gr->SetWarn(mglWarnDim,"Traj");	return;	}
	mglData z(x->GetNx()), az(x->GetNx());
	if(isnan(zVal))	zVal = gr->Min.z;
	z.Fill(zVal,zVal);
	mgl_traj_xyz(gr,x,y,&z,ax,ay,&az,sch,len);
}
//-----------------------------------------------------------------------------
void mgl_traj_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch, float *zVal, float *len,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_traj_xy(_GR_, _DA_(x), _DA_(y), _DA_(ax), _DA_(ay), s, *zVal, *len);
	delete []s;
}
//-----------------------------------------------------------------------------
void mgl_traj_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch,float *len,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_traj_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(ax), _DA_(ay), _DA_(az), s, *len);
	delete []s;
}
//-----------------------------------------------------------------------------
//
//	Vect series
//
//-----------------------------------------------------------------------------
void mgl_vect_xy(HMGL gr, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch,float zVal,int flag)
{
	long i,j,n=ax->GetNx(),m=ax->GetNy(),k;
	if(n*m*ax->GetNz()!=ay->GetNx()*ay->GetNy()*ay->GetNz())	{	gr->SetWarn(mglWarnDim,"Vect");	return;	}
	if(n<2 || m<2)						{	gr->SetWarn(mglWarnLow,"Vect");	return;	}
	bool both = x->GetNx()==n && y->GetNx()==n && x->GetNy()==m && y->GetNy()==m;
	if(!(both || (x->GetNx()==n && y->GetNx()==m)))	{	gr->SetWarn(mglWarnDim,"Vect");	return;	}
	static int cgid=1;	gr->StartGroup("Vect",cgid++);

	long ss = gr->AddTexture(sch);
	gr->ReserveC(4*n*m);
	if(isnan(zVal))	zVal = gr->Min.z;

	long tx=1,ty=1;
	if(gr->MeshNum>1)	{	tx=(n-1)/(gr->MeshNum-1);	ty=(m-1)/(gr->MeshNum-1);	}
	if(tx<1)	tx=1;	if(ty<1)	ty=1;
	float xm=0,ym,dx,dy;
	float dd,dm=(fabs(gr->Max.c)+fabs(gr->Min.c))*1e-5;

	for(k=0;k<ax->GetNz();k++)	for(j=0;j<m;j++)	for(i=0;i<n;i++)
	{
		ym = ax->v(i,j,k)*ax->v(i,j,k)+ay->v(i,j,k)*ay->v(i,j,k);
		xm = xm>ym ? xm : ym;
	}
	xm = 1./(xm==0 ? 1:sqrt(xm));
	long n1,n2;
	mglPoint p1,p2;
	float c1,c2, xx,yy;

	for(k=0;k<ax->GetNz();k++)
	{
		if(ax->GetNz()>1)	zVal = gr->Min.z+(gr->Max.z-gr->Min.z)*float(k)/(ax->GetNz()-1);
		for(i=0;i<n;i+=tx)	for(j=0;j<m;j+=ty)
		{
			xx = GetX(x,i,j,k).x;	yy = GetY(y,i,j,k).x;
			dx = i<n-1 ? (GetX(x,i+1,j,k).x-xx) : (xx-GetX(x,i-1,j,k).x);
			dy = j<m-1 ? (GetY(y,i,j+1,k).x-yy) : (yy-GetY(y,i,j-1,k).x);
			dx *= tx;	dy *= ty;	dd = hypot(ax->v(i,j,k),ay->v(i,j,k));
			dx *= (flag&MGL_VEC_LEN) ? (dd>dm ? ax->v(i,j,k)/dd : 0) : ax->v(i,j,k)*xm;
			dy *= (flag&MGL_VEC_LEN) ? (dd>dm ? ay->v(i,j,k)/dd : 0) : ay->v(i,j,k)*xm;

			if(flag & MGL_VEC_END)
			{	p1 = mglPoint(xx-dx,yy-dy,zVal);	p2 = mglPoint(xx,yy,zVal);	}
			else if(flag & MGL_VEC_MID)
			{	p1=mglPoint(xx-dx/2,yy-dy/2,zVal);	p2=mglPoint(xx+dx/2,yy+dy/2,zVal);	}
			else
			{	p1 = mglPoint(xx,yy,zVal);	p2 = mglPoint(xx+dx,yy+dy,zVal);	}
			if(flag&MGL_VEC_COL)	{	c1 = c2 = ss;	}	else
			{	c1=gr->GetC(ss,dd*xm*1.5-1,false);	c2=gr->GetC(ss,dd*xm*1.5-0.5,false);}
			gr->ScalePoint(p1);		n1=gr->AddPntC(p1,c1);
			gr->ScalePoint(p2);		n2=gr->AddPntC(p2,c2);
			if(flag & MGL_VEC_DOT)	gr->line_plot(n1,n2);
			else	gr->vect_plot(n1,n2);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_vect_2d(HMGL gr, HCDT ax, HCDT ay, const char *sch,float zVal,int flag)
{
	mglData x(ax->GetNx()), y(ax->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_vect_xy(gr,&x,&y,ax,ay,sch,zVal,flag);
}
//-----------------------------------------------------------------------------
void mgl_vect_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch, float *zVal,int *flag,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_vect_xy(_GR_, _DA_(x), _DA_(y), _DA_(ax), _DA_(ay), s, *zVal, *flag);
	delete []s;
}
//-----------------------------------------------------------------------------
void mgl_vect_2d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, const char *sch, float *zVal,int *flag,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_vect_2d(_GR_, _DA_(ax), _DA_(ay), s, *zVal,*flag);
	delete []s;
}
//-----------------------------------------------------------------------------
//
//	Vect3 series
//
//-----------------------------------------------------------------------------
void mgl_vect_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT ax, HCDT ay, HCDT az, const char *sch,int flag)
{
	register long i,j,n=ax->GetNx(),m=ax->GetNy(),l=ax->GetNz(),k;
	if(n*m*l!=ay->GetNx()*ay->GetNy()*ay->GetNz() || ax->GetNx()*ax->GetNy()*ax->GetNz()!=az->GetNx()*az->GetNy()*az->GetNz())
	{	gr->SetWarn(mglWarnDim,"Vect");	return;	}
	if(n<2 || m<2 || l<2)	{	gr->SetWarn(mglWarnLow,"Vect");	return;	}
	bool both = x->GetNx()*x->GetNy()*x->GetNz()==n*m*l && y->GetNx()*y->GetNy()*y->GetNz()==n*m*l && z->GetNx()*z->GetNy()*z->GetNz()==n*m*l;
	if(!(both || (x->GetNx()==n && y->GetNx()==m && z->GetNx()==l)))
	{	gr->SetWarn(mglWarnDim,"Vect");	return;	}
	static int cgid=1;	gr->StartGroup("Vect3",cgid++);

	float xm=0,ym,dx,dy,dz,dd,dm=(fabs(gr->Max.c)+fabs(gr->Min.c))*1e-5;
	long ss = gr->AddTexture(sch);
	gr->ReserveC(2*n*m*l);
	long tx=1,ty=1,tz=1;
	if(gr->MeshNum>1)
	{	tx=(n-1)/(gr->MeshNum-1);	ty=(m-1)/(gr->MeshNum-1);	tz=(l-1)/(gr->MeshNum-1);}
	if(tx<1)	tx=1;	if(ty<1)	ty=1;	if(tz<1)	tz=1;

	for(k=0;k<l;k++)	for(j=0;j<m;j++)	for(i=0;i<n;i++)
	{
		ym = ax->v(i,j,k)*ax->v(i,j,k)+ay->v(i,j,k)*ay->v(i,j,k)+az->v(i,j,k)*az->v(i,j,k);
		xm = xm>ym ? xm : ym;
	}
	xm = 1./(xm==0 ? 1:sqrt(xm));

	long n1,n2;
	mglPoint p1,p2;
	float c1,c2, xx,yy,zz;

	for(k=0;k<l;k+=tz)	for(i=0;i<n;i+=tx)	for(j=0;j<m;j+=ty)
	{
		xx = GetX(x,i,j,k).x;	yy = GetY(y,i,j,k).x;	zz = GetY(z,i,j,k).x;
		dx = i<n-1 ? (GetX(x,i+1,j,k).x-xx) : (xx-GetX(x,i-1,j,k).x);
		dy = j<m-1 ? (GetY(y,i,j+1,k).x-yy) : (yy-GetY(y,i,j-1,k).x);
		dz = k<l-1 ? (GetZ(z,i,j,k+1).x-zz) : (zz-GetZ(z,i,j,k-1).x);
		dx *= tx;	dy *= ty;	dz *= tz;
		dd = sqrt(ax->v(i,j,k)*ax->v(i,j,k)+ay->v(i,j,k)*ay->v(i,j,k)+az->v(i,j,k)*az->v(i,j,k));
		dx *= (flag&MGL_VEC_LEN) ? (dd>dm ? ax->v(i,j,k)/dd : 0) : ax->v(i,j,k)*xm;
		dy *= (flag&MGL_VEC_LEN) ? (dd>dm ? ay->v(i,j,k)/dd : 0) : ay->v(i,j,k)*xm;
		dz *= (flag&MGL_VEC_LEN) ? (dd>dm ? az->v(i,j,k)/dd : 0) : az->v(i,j,k)*xm;

		if(flag & MGL_VEC_END)
		{	p1 = mglPoint(xx-dx,yy-dy,zz-dz);	p2 = mglPoint(xx,yy,zz);	}
		else if(flag & MGL_VEC_MID)
		{	p1=mglPoint(xx-dx/2,yy-dy/2,zz-dz/2);	p2=mglPoint(xx+dx/2,yy+dy/2,zz+dz/2);	}
		else
		{	p1 = mglPoint(xx,yy,zz);	p2 = mglPoint(xx+dx,yy+dy,zz+dz);	}
		if(flag&MGL_VEC_COL)	{	c1 = c2 = ss;	}	else
		{	c1=gr->GetC(ss,dd*xm*1.5-1,false);	c2=gr->GetC(ss,dd*xm*1.5-0.5,false);	}
		gr->ScalePoint(p1);		n1=gr->AddPntC(p1,c1);
		gr->ScalePoint(p2);		n2=gr->AddPntC(p2,c2);
		if(flag & MGL_VEC_DOT)	gr->line_plot(n1,n2);
		else	gr->vect_plot(n1,n2);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_vect_3d(HMGL gr, HCDT ax, HCDT ay, HCDT az, const char *sch,int flag)
{
	mglData x(ax->GetNx()), y(ax->GetNy()), z(ax->GetNz());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	z.Fill(gr->Min.z,gr->Max.z);
	mgl_vect_xyz(gr,&x,&y,&z,ax,ay,az,sch,flag);
}
//-----------------------------------------------------------------------------
void mgl_vect_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch,int *flag,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_vect_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(ax), _DA_(ay), _DA_(az), s, *flag);
	delete []s;
}
//-----------------------------------------------------------------------------
void mgl_vect_3d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, int *flag, int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_vect_3d(_GR_, _DA_(ax), _DA_(ay), _DA_(az), s, *flag);
	delete []s;
}
//-----------------------------------------------------------------------------
//
//	Flow series
//
//-----------------------------------------------------------------------------
void flow(mglBase *gr, float zVal, float u, float v, const mglData &x, const mglData &y, const mglData &ax, const mglData &ay,long ss)
{
	long n=10*(ax.nx+ax.ny);
	bool both = x.nx==ax.nx && y.nx==ax.nx && x.ny==ax.ny && y.ny==ax.ny;

	mglPoint *pp = (mglPoint *)malloc(n*sizeof(mglPoint)), dp;
	float *cc = (float *)malloc(n*sizeof(float));

	float dt = 0.5/(ax.nx > ax.ny ? ax.nx : ax.ny),e,f,g,ff[4],gg[4],h,s=1;
	if(u<0 || v<0)	{	dt = -dt;	u = -u;		v = -v;		s = -1;}
	register long k=0,m;
	bool end = false;
	do{
		if(k>=n)	// resize array if need
		{
			n += 10*(ax.nx+ax.ny);
			pp = (mglPoint *)realloc(pp,n*sizeof(mglPoint));
			cc = (float *)realloc(cc,n*sizeof(float));
		}
		// insert point
		pp[k].x = both ? x.Spline1(u,v,0):x.Spline1(u,0,0);
		pp[k].y = both ? y.Spline1(u,v,0):y.Spline1(v,0,0);
		pp[k].z = zVal;
		gr->ScalePoint(pp[k]);
		for(m=0;m<k-1;m++)	// determines encircle
			if(Norm(pp[k]-pp[m])<dt*dt/4.)	{	end = true;	break;	}
		f = ax.Linear1(u,v,0);	g = ay.Linear1(u,v,0);
		h = hypot(f,g);	cc[k] = gr->GetC(ss,s*h);	h+=1;
		k++;
		// find next point by midpoint method
		if(h<1e-7)	break;	// stationary point
		ff[0]=f*dt/h;	gg[0]=g*dt/h;
		e = u+ff[0]/2;	h = v+gg[0]/2;
		f = ax.Linear1(e,h,0);	g = ay.Linear1(e,h,0);	h = 1+hypot(f,g);
		if(h<1e-7)	break;	// stationary point
		ff[1]=f*dt/h;	gg[1]=g*dt/h;
		e = u+ff[1]/2;	h = v+gg[1]/2;
		f = ax.Linear1(e,h,0);	g = ay.Linear1(e,h,0);	h = 1+hypot(f,g);
		if(h<1e-7)	break;	// stationary point
		ff[2]=f*dt/h;	gg[2]=g*dt/h;
		e = u+ff[2];	h = v+gg[2];
		f = ax.Linear1(e,h,0);	g = ay.Linear1(e,h,0);	h = 1+hypot(f,g);
		if(h<1e-7)	break;	// stationary point
		ff[3]=f*dt/h;	gg[3]=g*dt/h;
		u += ff[0]/6+ff[1]/3+ff[2]/3+ff[3]/6;
		v += gg[0]/6+gg[1]/3+gg[2]/3+gg[3]/6;
		// condition of end
		end = end || u<0 || v<0 || u>1 || v>1;
	} while(!end);
	register long i,j;
	gr->ReserveC(k);	j = gr->AddPntC(pp[0],cc[0]);
	for(i=1;i<k;i++)
	{
		k=j;	j = gr->AddPntC(pp[i],cc[i]);
		gr->line_plot(k,j);
	}
	free(pp);	free(cc);
}
//-----------------------------------------------------------------------------
void mgl_flow_xy(HMGL gr, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch, int num, float zVal)
{
	float u,v;
	long n=ax->GetNx(), m=ax->GetNy();
	if(n*m*ax->GetNz()!=ay->GetNx()*ay->GetNy()*ay->GetNz())	{	gr->SetWarn(mglWarnDim,"Flow");	return;	}
	if(n<2 || m<2)						{	gr->SetWarn(mglWarnLow,"Flow");	return;	}
	static int cgid=1;	gr->StartGroup("Flow",cgid++);
	bool both = x->GetNx()==n && y->GetNx()==n && x->GetNy()==m && y->GetNy()==m;
	if(!(both || (x->GetNx()==n && y->GetNx()==m)))	{	gr->SetWarn(mglWarnDim,"Flow");	return;	}

	long ss = gr->AddTexture(sch);
	// allocate memory
	if(isnan(zVal)) zVal = gr->Min.z;
	bool cnt=(num>0);
	num = abs(num);
	mglData xx(x), yy(y), bx(ax), by(ay);

	for(long k=0;k<ax->GetNz();k++)
	{
		if(ax->GetNz()>1)	zVal = gr->Min.z+(gr->Max.z-gr->Min.z)*float(k)/(ax->GetNz()-1);
		for(long i=0;i<num;i++)
		{
			u = 0;	v = (i+1.)/(num+1.);
			flow(gr, zVal, u, v, xx, yy, bx, by,ss);
			flow(gr, zVal, -u, -v, xx, yy, bx, by,ss);
			u = 1;	v = (i+1.)/(num+1.);
			flow(gr, zVal, u, v, xx, yy, bx, by,ss);
			flow(gr, zVal, -u, -v, xx, yy, bx, by,ss);
			u = (i+1.)/(num+1.);	v = 0;
			flow(gr, zVal, u, v, xx, yy, bx, by,ss);
			flow(gr, zVal, -u, -v, xx, yy, bx, by,ss);
			u = (i+1.)/(num+1.);	v = 1;
			flow(gr, zVal, u, v, xx, yy, bx, by,ss);
			flow(gr, zVal, -u, -v, xx, yy, bx, by,ss);
			if(cnt)
			{
				u = 0.5;	v = (i+1.)/(num+1.);
				flow(gr, zVal, u, v, xx, yy, bx, by,ss);
				flow(gr, zVal, -u, -v, xx, yy, bx, by,ss);
				u = (i+1.)/(num+1.);	v = 0.5;
				flow(gr, zVal, u, v, xx, yy, bx, by,ss);
				flow(gr, zVal, -u, -v, xx, yy, bx, by,ss);
			}
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_flow_2d(HMGL gr, HCDT ax, HCDT ay, const char *sch, int num, float zVal)
{
	mglData x(ax->GetNx()), y(ax->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_flow_xy(gr,&x,&y,ax,ay,sch,num,zVal);
}
//-----------------------------------------------------------------------------
void mgl_flowp_xy(HMGL gr, float x0, float y0, float z0, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch)
{
	mglPoint p(x0,y0,z0);
	float u,v;
	long n=ax->GetNx(), m=ax->GetNy();
	if(n*m*ax->GetNz()!=ay->GetNx()*ay->GetNy()*ay->GetNz())	{	gr->SetWarn(mglWarnDim,"Flow");	return;	}
	if(n<2 || m<2)						{	gr->SetWarn(mglWarnLow,"Flow");	return;	}
	static int cgid=1;	gr->StartGroup("FlowP",cgid++);
	bool both = x->GetNx()==n && y->GetNx()==n && x->GetNy()==m && y->GetNy()==m;
	if(!(both || (x->GetNx()==n && y->GetNx()==m)))	{	gr->SetWarn(mglWarnDim,"Flow");	return;	}

	long ss = gr->AddTexture(sch);
	// find coordinates u, v
	register long i,j;
	register float d, dm=1e7;
	long i0=0,j0=0;
	for(i=0;i<n;i++)	for(j=0;j<m;j++)	// first find closest
	{
		d = both ? hypot(x->v(i,j)-p.x,y->v(i,j)-p.y) : hypot(x->v(i)-p.x,y->v(j)-p.y);
		if(d<dm)	{	i0=i;	j0=j;	dm=d;	}
	}
	if(dm==0)	{	u = i0/float(n);	v = j0/float(m);	}	// we find it
	else
	{
		float dxu,dxv,dyu,dyv, dx, dy;
		if(both)
		{
			dx = x->v(i0,j0)-p.x;	dy = y->v(i0,j0)-p.y;
			dxu= x->dvx(i0,j0);		dyu= y->dvx(i0,j0);
			dxv= x->dvy(i0,j0);		dyv= y->dvy(i0,j0);
			d = dxv*dyu-dxu*dyv;
			u = (i0+(dxv*dy-dx*dyv)/d)/n;
			v = (j0-(dxu*dy-dx*dyu)/d)/m;
		}
		else
		{
			dx = x->v(i0)-p.x;	dy = y->v(j0)-p.y;
			dxu= x->dvx(i0);	dyv= y->dvx(j0);
			u = (i0+dx/dxu)/n;	v = (j0+dy/dyv)/m;
		}
	}
	mglData xx(x), yy(y), bx(ax), by(ay);
	flow(false, p.z, u, v, xx, yy, bx, by,ss);
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_flowp_2d(HMGL gr, float x0, float y0, float z0, HCDT ax, HCDT ay, const char *sch)
{
	mglData x(ax->GetNx()), y(ax->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_flowp_xy(gr,x0,y0,z0,&x,&y,ax,ay,sch);
}
//-----------------------------------------------------------------------------
void flow(mglBase *gr, float u, float v, float w, const mglData &x, const mglData &y, const mglData &z, const mglData &ax, const mglData &ay, const mglData &az,long ss)
{
	static long n=10*(ax.nx+ax.ny);
	long nn = ax.nx*ax.ny*ax.nz;
	bool both = x.nx*x.ny*x.nz==nn && y.nx*y.ny*y.nz==nn && z.nx*z.ny*z.nz==nn;
	mglPoint *pp = (mglPoint *)malloc(n*sizeof(mglPoint));
	float *cc = (float *)malloc(n*sizeof(float));

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
			pp = (mglPoint *)realloc(pp,n*sizeof(mglPoint));
			cc = (float *)realloc(cc,n*sizeof(float));
		}
		// insert point
		pp[k].x = both ? x.Spline1(u,v,w):x.Spline1(u,0,0);
		pp[k].y = both ? y.Spline1(u,v,w):y.Spline1(v,0,0);
		pp[k].z = both ? z.Spline1(u,v,w):z.Spline1(w,0,0);
		gr->ScalePoint(pp[k]);
		for(m=0;m<k-1;m++)	// determines encircle
			if(Norm(pp[k]-pp[m])<dt*dt/4.)	{	end = true;	break;	}
		e = ax.Linear1(u,v,w);	f = ay.Linear1(u,v,w);	g = az.Linear1(u,v,w);
		h = sqrt(e*e+f*f+g*g);	cc[k] = gr->GetC(ss,s*h);	h+=1;
		k++;
		// find next point by midpoint method
		if(h<1e-7)	break;	// stationary point
		ee[0]=e*dt/h;	ff[0]=f*dt/h;	gg[0]=g*dt/h;
		u1 = u+ee[0]/2;	v1 = v+ff[0]/2;	w1 = w+gg[0]/2;
		e = ax.Linear1(u1,v1,w1);	f = ay.Linear1(u1,v1,w1);
		g = az.Linear1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
		if(h<1e-7)	break;	// stationary point
		ee[1]=e*dt/h;	ff[1]=f*dt/h;	gg[1]=g*dt/h;
		u1 = u+ee[1]/2;	v1 = v+ff[1]/2;	w1 = w+gg[1]/2;
		e = ax.Linear1(u1,v1,w1);	f = ay.Linear1(u1,v1,w1);
		g = az.Linear1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
		if(h<1e-7)	break;	// stationary point
		ee[2]=e*dt/h;	ff[2]=f*dt/h;	gg[2]=g*dt/h;
		u1 = u+ee[2];	v1 = v+ff[2];	w1 = w+gg[2];
		e = ax.Linear1(u1,v1,w1);	f = ay.Linear1(u1,v1,w1);
		g = az.Linear1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
		if(h<1e-7)	break;	// stationary point
		ee[3]=e*dt/h;	ff[3]=f*dt/h;	gg[3]=g*dt/h;
		u += ee[0]/6+ee[1]/3+ee[2]/3+ee[3]/6;
		v += ff[0]/6+ff[1]/3+ff[2]/3+ff[3]/6;
		w += gg[0]/6+gg[1]/3+gg[2]/3+gg[3]/6;
		// condition of end
		end = end || u<0 || v<0 || u>1 || v>1 || w<0 || w>1;
	} while(!end);
	register long i,j;
	gr->ReserveC(k);	j = gr->AddPntC(pp[0],cc[0]);
	for(i=1;i<k;i++)
	{
		j = gr->AddPntC(pp[i],cc[i]);
		gr->line_plot(j-1,j);
	}
	free(pp);	free(cc);
}
//-----------------------------------------------------------------------------
void mgl_flow_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT ax, HCDT ay, HCDT az, const char *sch, int num)
{
	float u,v,w;
	long i,j,n=ax->GetNx(),m=ax->GetNy(),l=ax->GetNz();
	if(ax->GetNx()*ax->GetNy()*ax->GetNz()!=ay->GetNx()*ay->GetNy()*ay->GetNz() || ax->GetNx()*ax->GetNy()*ax->GetNz()!=az->GetNx()*az->GetNy()*az->GetNz())
	{	gr->SetWarn(mglWarnDim,"Flow");	return;	}
	if(ax->GetNx()<2 || ax->GetNy()<2 || ax->GetNz()<2)
	{	gr->SetWarn(mglWarnLow,"Flow");	return;	}
	bool both = x->GetNx()*x->GetNy()*x->GetNz()==n*m*l && y->GetNx()*y->GetNy()*y->GetNz()==n*m*l && z->GetNx()*z->GetNy()*z->GetNz()==n*m*l;
	if(!(both || (x->GetNx()==n && y->GetNx()==m && z->GetNx()==l)))
	{	gr->SetWarn(mglWarnDim,"Flow");	return;	}
	static int cgid=1;	gr->StartGroup("Flow3",cgid++);
	bool cnt=(num>0);	num = abs(num);	// redefine central parater
	long ss = gr->AddTexture(sch);

	mglData xx(x), yy(y), zz(z), bx(ax), by(ay), bz(az);
	for(i=0;i<num;i++)	for(j=0;j<num;j++)
	{
		u = (i+1.)/(num+1.);	v = (j+1.)/(num+1.);	w = 0;
		flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss);
		flow(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,ss);
		u = (i+1.)/(num+1.);	v = (j+1.)/(num+1.);	w = 1;
		flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss);
		flow(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,ss);
		u = 0;	v = (j+1.)/(num+1.);	w = (i+1.)/(num+1.);
		flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss);
		flow(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,ss);
		u = 1;	v = (j+1.)/(num+1.);	w = (i+1.)/(num+1.);
		flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss);
		flow(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,ss);
		u = (i+1.)/(num+1.);	v = 0;	w = (j+1.)/(num+1.);
		flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss);
		flow(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,ss);
		u = (i+1.)/(num+1.);	v = 1;	w = (j+1.)/(num+1.);
		flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss);
		flow(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,ss);
		if(cnt)
		{
			u = (i+1.)/(num+1.);	v = (j+1.)/(num+1.);	w = 0.5;
			flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss);
			flow(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,ss);
			u = 0.5;	v = (j+1.)/(num+1.);	w = (i+1.)/(num+1.);
			flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss);
			flow(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,ss);
			u = (i+1.)/(num+1.);	v = 0.5;	w = (j+1.)/(num+1.);
			flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss);
			flow(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,ss);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_flow_3d(HMGL gr, HCDT ax, HCDT ay, HCDT az, const char *sch, int num)
{
	mglData x(ax->GetNx()), y(ax->GetNy()), z(ax->GetNz());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	z.Fill(gr->Min.z,gr->Max.z);
	mgl_flow_xyz(gr,&x,&y,&z,ax,ay,az,sch,num);
}
//-----------------------------------------------------------------------------
void mgl_flowp_xyz(HMGL gr, float x0, float y0, float z0, HCDT x, HCDT y, HCDT z, HCDT ax, HCDT ay, HCDT az, const char *sch)
{
	mglPoint p(x0,y0,z0);
	float u,v,w;
	long n=ax->GetNx(),m=ax->GetNy(),l=ax->GetNz();
	if(ax->GetNx()*ax->GetNy()*ax->GetNz()!=ay->GetNx()*ay->GetNy()*ay->GetNz() || ax->GetNx()*ax->GetNy()*ax->GetNz()!=az->GetNx()*az->GetNy()*az->GetNz())
	{	gr->SetWarn(mglWarnDim,"Flow");	return;	}
	if(ax->GetNx()<2 || ax->GetNy()<2 || ax->GetNz()<2)
	{	gr->SetWarn(mglWarnLow,"Flow");	return;	}
	bool both = x->GetNx()*x->GetNy()*x->GetNz()==n*m*l && y->GetNx()*y->GetNy()*y->GetNz()==n*m*l && z->GetNx()*z->GetNy()*z->GetNz()==n*m*l;
	if(!(both || (x->GetNx()==n && y->GetNx()==m && z->GetNx()==l)))
	{	gr->SetWarn(mglWarnDim,"Flow");	return;	}
	static int cgid=1;	gr->StartGroup("FlowP3",cgid++);
	long ss = gr->AddTexture(sch);

	// find coordinates u, v, w
	register long i,j,k,ii;
	register float d, dm=1e7;
	long i0=0,j0=0,k0=0;
	float dx,dy,dz;
	for(i=0;i<n;i++)	for(j=0;j<m;j++)	for(k=0;k<l;k++)	// first find closest
	{
		ii = i+n*(j+m*k);
		if(both)
		{	dx = x->v(i,j,k)-p.x;	dy = y->v(i,j,k)-p.y;	dz = x->v(i,j,k)-p.z;	}
		else
		{	dx = x->v(i)-p.x;	dy = y->v(j)-p.y;	dz = x->v(k)-p.z;	}
		d = sqrt(dx*dx+dy*dy+dz*dz);
		if(d<dm)	{	i0=i;	j0=j;	k0=k;	dm=d;	}
	}
	if(dm==0)	// we find it
	{	u=i0/float(n);	v=j0/float(m);	w=k0/float(l);	}
	else
	{
		float dxu,dxv,dxw,dyu,dyv,dyw,dzu,dzv,dzw;
		if(both)
		{
			ii = i0+n*j0;
			dx = x->v(i0,j0,k0)-p.x;	dy = y->v(i0,j0,k0)-p.y;	dz = z->v(i0,j0,k0)-p.z;
			dxu= x->dvx(i0,j0,k0);		dyu= y->dvx(i0,j0,k0);		dzu= z->dvx(i0,j0,k0);
			dxv= x->dvy(i0,j0,k0);		dyv= y->dvy(i0,j0,k0);		dzv= z->dvy(i0,j0,k0);
			dxw= x->dvz(i0,j0,k0);		dyw= y->dvz(i0,j0,k0);		dzw= z->dvz(i0,j0,k0);
			d = dxu*(dyw*dzv-dyv*dzw)+dxv*(dyu*dzw-dyw*dzu)+dxw*(dyv*dzu-dyu*dzv);
			u = (i0+(dx*(dyw*dzv-dyv*dzw)+dxv*(dy*dzw-dyw*dz)+dxw*(dyv*dz-dy*dzv))/d)/n;
			v = (j0-(dx*(dyw*dzu-dyu*dzw)+dxu*(dy*dzw-dyw*dz)+dxw*(dyu*dz-dy*dzu))/d)/m;
			w = (i0+(dx*(dyv*dzu-dyu*dzv)+dxu*(dy*dzv-dyv*dz)+dxv*(dyu*dz-dy*dzu))/d)/l;
		}
		else
		{
			dx = x->v(i0)-p.x;	dy = y->v(j0)-p.y;	dz = z->v(k0)-p.z;
			dxu= x->dvx(i0);	dyv= y->dvx(j0);	dzw= z->dvx(k0);
			u = (i0+dx/dxu)/n;	v = (j0+dy/dyv)/m;	w = (k0+dz/dzw)/m;
		}
	}
	mglData xx(x), yy(y), zz(z), bx(ax), by(ay), bz(az);
	flow(false, u, v, w, xx, yy, zz, bx, by, bz,ss);
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_flowp_3d(HMGL gr, float x0, float y0, float z0, HCDT ax, HCDT ay, HCDT az, const char *sch)
{
	mglData x(ax->GetNx()), y(ax->GetNy()), z(ax->GetNz());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	z.Fill(gr->Min.z,gr->Max.z);
	mgl_flowp_xyz(gr, x0,y0,z0, &x,&y,&z,ax,ay,az,sch);
}
//-----------------------------------------------------------------------------
//
//	Grad series
//
//-----------------------------------------------------------------------------
void mgl_grad_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT phi, const char *sch, int num)
{
	mglData ax(phi), ay(phi),az(phi),xx(phi),yy(phi),zz(phi);
	long n=xx.nx, m=xx.ny, l=xx.nz, nn = n*m*l;
	if(x->GetNx()*x->GetNy()*x->GetNz()==nn && y->GetNx()*y->GetNy()*y->GetNz()==nn && x->GetNx()*x->GetNy()*x->GetNz()==nn)
	{	xx.Set(x);	yy.Set(y);	zz.Set(z);	}	// nothing to do
	else if(x->GetNx()==n && y->GetNx()==m && z->GetNx()==l)
	{	// prepare data
		register long i,j,k,i0;
		for(i=0;i<n;i++)	for(j=0;j<m;j++)	for(k=0;k<l;k++)
		{	i0 = i+n*(j+m*k);	xx.a[i0] = x->v(i);
			yy.a[i0] = y->v(j);	zz.a[i0] = z->v(k);	}
	}
	else	{	gr->SetWarn(mglWarnDim,"Grad");	return;	}
	ax.Diff(xx,yy,zz);	ay.Diff(yy,xx,zz);	az.Diff(zz,xx,yy);
	mgl_flow_xyz(gr,&xx,&yy,&zz,&ax,&ay,&az,sch,num);
}
//-----------------------------------------------------------------------------
void mgl_grad_xy(HMGL gr, HCDT x, HCDT y, HCDT phi, const char *sch, int num, float zVal)
{
	mglData ax(phi), ay(phi),xx(phi),yy(phi);
	long n = phi->GetNx(), m=phi->GetNy(), nn=n*m;
	if(x->GetNx()*x->GetNy()==nn && y->GetNx()*y->GetNy()==nn)	{	xx.Set(x);	yy.Set(y);	}
	else if(x->GetNx()==n && y->GetNx()==m)
	{
		register long i,j,i0;
		for(i=0;i<n;i++)	for(j=0;j<m;j++)
		{	i0 = i+n*j;	xx.a[i0] = x->v(i);	yy.a[i0] = y->v(j);	}
	}
	else	{	gr->SetWarn(mglWarnDim,"Grad");	return;	}
	ax.Diff(xx,yy);	ay.Diff(yy,xx);
	mgl_flow_xy(gr,&xx,&yy,&ax,&ay,sch,num,zVal);
}
//-----------------------------------------------------------------------------
void mgl_grad(HMGL gr, HCDT phi, const char *sch, int num, float zVal)
{
	mglData x(phi->GetNx()), y(phi->GetNy()), z(phi->GetNz());
	x.Fill(gr->Min.x,gr->Max.x);	y.Fill(gr->Min.y,gr->Max.y);	z.Fill(gr->Min.z,gr->Max.z);
	if(phi->GetNz()==1)	mgl_grad_xy(gr,&x,&y,phi,sch,num?num:5,zVal);
	else				mgl_grad_xyz(gr,&x,&y,&z,phi,sch,num?num:3);
}
//-----------------------------------------------------------------------------
//
//	Pipe series
//
//-----------------------------------------------------------------------------
void flowr(mglBase *gr, float zVal, float u, float v, const mglData &x, const mglData &y, const mglData &ax, const mglData &ay, float r0,long sc)
{
	long n=10*(ax.nx+ax.ny);
	bool both = x.nx==ax.nx && y.nx==ax.nx && x.ny==ax.ny && y.ny==ax.ny;

	mglPoint *pp = (mglPoint *)malloc(n*sizeof(mglPoint)), dp;
	float *cc = (float *)malloc(n*sizeof(float));

	float dt = 0.5/(ax.nx > ax.ny ? ax.nx : ax.ny),e,f,g,ff[4],gg[4],h,s=1;
	float ss = 	4/mgl_ipow(gr->Max.c - gr->Min.c,2);
	if(u<0 || v<0)	{	dt = -dt;	u = -u;		v = -v;		s = -1;}
	register long k=0,m;
	bool end = false;
	do{
		if(k>=n)	// resize array if need
		{
			n += 10*(ax.nx+ax.ny);
			pp = (mglPoint *)realloc(pp,n*sizeof(mglPoint));
			cc = (float *)realloc(cc,n*sizeof(float));
		}
		// insert point
		pp[k].x = both ? x.Spline1(u,v,0):x.Spline1(u,0,0);
		pp[k].y = both ? y.Spline1(u,v,0):y.Spline1(v,0,0);
		pp[k].z = zVal;
		gr->ScalePoint(pp[k]);
		for(m=0;m<k-1;m++)	// determines encircle
			if(Norm(pp[k]-pp[m])<dt*dt/4.)	{	end = true;	break;	}
		f = ax.Linear1(u,v,0);	g = ay.Linear1(u,v,0);
		h = hypot(f,g);	cc[k] = gr->GetC(sc,s*h);	h+=1;
		pp[k].c = r0>0 ? r0*sqrt(1e-2+ss*h*h)/2 : -r0/sqrt(1e-2+ss*h*h)/5;
		k++;
		// find next point by midpoint method
		if(h<1e-7)	break;	// stationary point
		ff[0]=f*dt/h;	gg[0]=g*dt/h;
		e = u+ff[0]/2;	h = v+gg[0]/2;
		f = ax.Linear1(e,h,0);	g = ay.Linear1(e,h,0);	h = 1+hypot(f,g);
		if(h<1e-7)	break;	// stationary point
		ff[1]=f*dt/h;	gg[1]=g*dt/h;
		e = u+ff[1]/2;	h = v+gg[1]/2;
		f = ax.Linear1(e,h,0);	g = ay.Linear1(e,h,0);	h = 1+hypot(f,g);
		if(h<1e-7)	break;	// stationary point
		ff[2]=f*dt/h;	gg[2]=g*dt/h;
		e = u+ff[2];	h = v+gg[2];
		f = ax.Linear1(e,h,0);	g = ay.Linear1(e,h,0);	h = 1+hypot(f,g);
		if(h<1e-7)	break;	// stationary point
		ff[3]=f*dt/h;	gg[3]=g*dt/h;
		u += ff[0]/6+ff[1]/3+ff[2]/3+ff[3]/6;
		v += gg[0]/6+gg[1]/3+gg[2]/3+gg[3]/6;
		// condition of end
		end = end || u<0 || v<0 || u>1 || v>1;
	} while(!end);
	register long i,j, i0;
	const int num=41;
	if(k<2)	return;	// nothing to plot
	long pos = gr->ReserveN(num*k);
	mglPoint p,l,t,q,d;
	for(i=0;i<k;i++)
	{
		if(i==0)		l = pp[1]-pp[0];
		else if(i==k-1)	l = pp[i]-pp[i-1];
		else	l = pp[i+1]-pp[i-1];
		t = !l;	t/=Norm(t);		q = t^l;	q/=Norm(q);
		float si,co,ff, rr=pp[i].c,dr=l.c;
		for(j=0;j<num;j++)
		{
			ff = j*2*M_PI/(num-1);	co = cos(ff);	si = sin(ff);
			p = pp[i] + t*(rr*co) + q*(rr*si);
			d = (t*si - q*co)^(l + t*(dr*co) + q*(dr*si));
			gr->ScalePoint(p);	gr->AddPntN(p,cc[i],d);
		}
	}
	for(j=0;j<num-1;j++)	for(i=0;i<n-1;i++)
	{
		i0 = pos+j+num*i;
		gr->quad_plot(i0,i0+1,i0+1+num,i0+num);
	}
	free(pp);	free(cc);
}
//-----------------------------------------------------------------------------
void mgl_pipe_xy(HMGL gr, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch, float r0, int num, float zVal)
{
	float u,v;
	long n=ax->GetNx(), m=ax->GetNy();
	if(n*m*ax->GetNz()!=ay->GetNx()*ay->GetNy()*ay->GetNz())	{	gr->SetWarn(mglWarnDim,"Pipe");	return;	}
	if(n<2 || m<2)						{	gr->SetWarn(mglWarnLow,"Pipe");	return;	}
	bool both = x->GetNx()==n && y->GetNx()==n && x->GetNy()==m && y->GetNy()==m;
	if(!(both || (x->GetNx()==n && y->GetNx()==m)))	{	gr->SetWarn(mglWarnDim,"Pipe");	return;	}
	static int cgid=1;	gr->StartGroup("Pipe",cgid++);

	long ss = gr->AddTexture(sch);
	// allocate memory
	if(isnan(zVal)) zVal = gr->Min.z;
	bool cnt=(num>0);	num = abs(num);

	mglData xx(x), yy(y), bx(ax), by(ay);
	for(long k=0;k<ax->GetNz();k++)
	{
		if(ax->GetNz()>1)	zVal = gr->Min.z+(gr->Max.z-gr->Min.z)*float(k)/(ax->GetNz()-1);
		for(long i=0;i<num;i++)
		{
			u = 0;	v = (i+1.)/(num+1.);
			flowr(gr, zVal, u, v, xx, yy, bx, by,r0,ss);
			flowr(gr, zVal, -u, -v, xx, yy, bx, by,r0,ss);
			u = 1;	v = (i+1.)/(num+1.);
			flowr(gr, zVal, u, v, xx, yy, bx, by,r0,ss);
			flowr(gr, zVal, -u, -v, xx, yy, bx, by,r0,ss);
			u = (i+1.)/(num+1.);	v = 0;
			flowr(gr, zVal, u, v, xx, yy, bx, by,r0,ss);
			flowr(gr, zVal, -u, -v, xx, yy, bx, by,r0,ss);
			u = (i+1.)/(num+1.);	v = 1;
			flowr(gr, zVal, u, v, xx, yy, bx, by,r0,ss);
			flowr(gr, zVal, -u, -v, xx, yy, bx, by,r0,ss);
			if(cnt)
			{
				u = 0.5;	v = (i+1.)/(num+1.);
				flowr(gr, zVal, u, v, xx, yy, bx, by,r0,ss);
				flowr(gr, zVal, -u, -v, xx, yy, bx, by,r0,ss);
				u = (i+1.)/(num+1.);	v = 0.5;
				flowr(gr, zVal, u, v, xx, yy, bx, by,r0,ss);
				flowr(gr, zVal, -u, -v, xx, yy, bx, by,r0,ss);
			}
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_pipe_2d(HMGL gr, HCDT ax, HCDT ay, const char *sch, float r0, int num, float zVal)
{
	mglData x(ax->GetNx()), y(ax->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_pipe_xy(gr,&x,&y,ax,ay,sch,r0,num,zVal);
}
//-----------------------------------------------------------------------------
void flowr(mglBase *gr, float u, float v, float w, const mglData &x, const mglData &y, const mglData &z, const mglData &ax, const mglData &ay, const mglData &az, float r0,long sc)
{
	static long n=10*(ax.nx+ax.ny);
	long nn = ax.nx*ax.ny*ax.nz;
	bool both = x.nx*x.ny*x.nz==nn && y.nx*y.ny*y.nz==nn && z.nx*z.ny*z.nz==nn;
	mglPoint *pp = (mglPoint *)malloc(n*sizeof(mglPoint));
	float *cc = (float *)malloc(n*sizeof(float));

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
			pp = (mglPoint *)realloc(pp,n*sizeof(mglPoint));
			cc = (float *)realloc(cc,n*sizeof(float));
		}
		// insert point
		pp[k].x = both ? x.Spline1(u,v,w):x.Spline1(u,0,0);
		pp[k].y = both ? y.Spline1(u,v,w):y.Spline1(v,0,0);
		pp[k].z = both ? z.Spline1(u,v,w):z.Spline1(w,0,0);
		gr->ScalePoint(pp[k]);
		for(m=0;m<k-1;m++)	// determines encircle
			if(Norm(pp[k]-pp[m])<dt*dt/4.)	{	end = true;	break;	}
		e = ax.Linear1(u,v,w);	f = ay.Linear1(u,v,w);	g = az.Linear1(u,v,w);
		h = sqrt(e*e+f*f+g*g);	cc[k] = gr->GetC(sc,s*h);	h+=1;
		k++;
		// find next point by midpoint method
		if(h<1e-7)	break;	// stationary point
		ee[0]=e*dt/h;	ff[0]=f*dt/h;	gg[0]=g*dt/h;
		u1 = u+ee[0]/2;	v1 = v+ff[0]/2;	w1 = w+gg[0]/2;
		e = ax.Linear1(u1,v1,w1);	f = ay.Linear1(u1,v1,w1);
		g = az.Linear1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
		if(h<1e-7)	break;	// stationary point
		ee[1]=e*dt/h;	ff[1]=f*dt/h;	gg[1]=g*dt/h;
		u1 = u+ee[1]/2;	v1 = v+ff[1]/2;	w1 = w+gg[1]/2;
		e = ax.Linear1(u1,v1,w1);	f = ay.Linear1(u1,v1,w1);
		g = az.Linear1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
		if(h<1e-7)	break;	// stationary point
		ee[2]=e*dt/h;	ff[2]=f*dt/h;	gg[2]=g*dt/h;
		u1 = u+ee[2];	v1 = v+ff[2];	w1 = w+gg[2];
		e = ax.Linear1(u1,v1,w1);	f = ay.Linear1(u1,v1,w1);
		g = az.Linear1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
		if(h<1e-7)	break;	// stationary point
		ee[3]=e*dt/h;	ff[3]=f*dt/h;	gg[3]=g*dt/h;
		u += ee[0]/6+ee[1]/3+ee[2]/3+ee[3]/6;
		v += ff[0]/6+ff[1]/3+ff[2]/3+ff[3]/6;
		w += gg[0]/6+gg[1]/3+gg[2]/3+gg[3]/6;
		// condition of end
		end = end || u<0 || v<0 || u>1 || v>1 || w<0 || w>1;
	} while(!end);
	register long i,j, i0;
	const int num=41;
	if(k<2)	return;	// nothing to plot
	long pos = gr->ReserveN(num*k);
	mglPoint p,l,t,q;
	for(i=0;i<k;i++)
	{
		if(i==0)		l = pp[1]-pp[0];
		else if(i==k-1)	l = pp[i]-pp[i-1];
		else	l = pp[i+1]-pp[i-1];
		l = l/Norm(l);	t = !l;		q = t^l;
		float rr=pp[i].c;
		for(j=0;j<num;j++)
		{
			p = pp[i] + t*(rr*cos(j*2*M_PI/(num-1))) + q*(rr*sin(j*2*M_PI/(num-1)));
			gr->ScalePoint(p);	gr->AddPntN(p,cc[i],p-q);
		}
	}
	for(j=0;j<num-1;j++)	for(i=0;i<n-1;i++)
	{
		i0 = pos+j+num*i;
		gr->quad_plot(i0,i0+1,i0+1+num,i0+num);
	}
	free(pp);	free(cc);
}
//-----------------------------------------------------------------------------
void mgl_pipe_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT ax, HCDT ay, HCDT az, const char *sch, float r0, int num)
{
	float u,v,w;
	long i,j,n=ax->GetNx(),m=ax->GetNy(),l=ax->GetNz();
	if(ax->GetNx()*ax->GetNy()*ax->GetNz()!=ay->GetNx()*ay->GetNy()*ay->GetNz() || ax->GetNx()*ax->GetNy()*ax->GetNz()!=az->GetNx()*az->GetNy()*az->GetNz())
	{	gr->SetWarn(mglWarnDim,"Pipe");	return;	}
	if(ax->GetNx()<2 || ax->GetNy()<2 || ax->GetNz()<2)
	{	gr->SetWarn(mglWarnLow,"Pipe");	return;	}
	bool both = x->GetNx()*x->GetNy()*x->GetNz()==n*m*l && y->GetNx()*y->GetNy()*y->GetNz()==n*m*l && z->GetNx()*z->GetNy()*z->GetNz()==n*m*l;
	if(!(both || (x->GetNx()==n && y->GetNx()==m && z->GetNx()==l)))
	{	gr->SetWarn(mglWarnDim,"Pipe");	return;	}
	static int cgid=1;	gr->StartGroup("Pipe3",cgid++);

	long ss = gr->AddTexture(sch);
	bool cnt=(num>0);	num = abs(num);

	mglData xx(x), yy(y), zz(z), bx(ax), by(ay), bz(az);
	for(i=0;i<num;i++)	for(j=0;j<num;j++)
	{
		u = (i+1.)/(num+1.);	v = (j+1.)/(num+1.);	w = 0;
		flowr(gr, u, v, w, xx, yy, zz, bx, by, bz,r0,ss);
		flowr(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,r0,ss);
		u = (i+1.)/(num+1.);	v = (j+1.)/(num+1.);	w = 1;
		flowr(gr, u, v, w, xx, yy, zz, bx, by, bz,r0,ss);
		flowr(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,r0,ss);
		u = 0;	v = (j+1.)/(num+1.);	w = (i+1.)/(num+1.);
		flowr(gr, u, v, w, xx, yy, zz, bx, by, bz,r0,ss);
		flowr(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,r0,ss);
		u = 1;	v = (j+1.)/(num+1.);	w = (i+1.)/(num+1.);
		flowr(gr, u, v, w, xx, yy, zz, bx, by, bz,r0,ss);
		flowr(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,r0,ss);
		u = (i+1.)/(num+1.);	v = 0;	w = (j+1.)/(num+1.);
		flowr(gr, u, v, w, xx, yy, zz, bx, by, bz,r0,ss);
		flowr(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,r0,ss);
		u = (i+1.)/(num+1.);	v = 1;	w = (j+1.)/(num+1.);
		flowr(gr, u, v, w, xx, yy, zz, bx, by, bz,r0,ss);
		flowr(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,r0,ss);
		if(cnt)
		{
			u = (i+1.)/(num+1.);	v = (j+1.)/(num+1.);	w = 0.5;
			flowr(gr, u, v, w, xx, yy, zz, bx, by, bz,r0,ss);
			flowr(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,r0,ss);
			u = 0.5;	v = (j+1.)/(num+1.);	w = (i+1.)/(num+1.);
			flowr(gr, u, v, w, xx, yy, zz, bx, by, bz,r0,ss);
			flowr(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,r0,ss);
			u = (i+1.)/(num+1.);	v = 0.5;	w = (j+1.)/(num+1.);
			flowr(gr, u, v, w, xx, yy, zz, bx, by, bz,r0,ss);
			flowr(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,r0,ss);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_pipe_3d(HMGL gr, HCDT ax, HCDT ay, HCDT az, const char *sch, float r0, int num)
{
	mglData x(ax->GetNx()), y(ax->GetNy()), z(ax->GetNz());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	z.Fill(gr->Min.z,gr->Max.z);
	mgl_pipe_xyz(gr,&x,&y,&z,ax,ay,az,sch,r0,num);
}
//-----------------------------------------------------------------------------
//	Fortran interface
//-----------------------------------------------------------------------------
/// Plot flows for vector field {ax,ay} parametrically depended on coordinate {x,y} with color proportional to value |a|
void mgl_flow_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch,
					int *num, float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_flow_xy(_GR_, _DA_(x), _DA_(y), _DA_(ax), _DA_(ay), s, *num, *zVal);	delete []s;	}
/// Plot flows for vector field {ax,ay} with color proportional to value |a|
void mgl_flow_2d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, const char *sch, int *num, float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_flow_2d(_GR_, _DA_(ax), _DA_(ay), s, *num, *zVal);	delete []s;	}
/// Plot flows for 3d vector field {ax,ay,ay} parametrically depended on coordinate {x,y,z} with color proportional to value |a|
void mgl_flow_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, int *num,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_flow_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(ax), _DA_(ay), _DA_(az), s, *num);	delete []s;	}
/// Plot flows for 3d vector field {ax,ay,ay} with color proportional to value |a|
void mgl_flow_3d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, int *num,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_flow_3d(_GR_, _DA_(ax), _DA_(ay), _DA_(az), s, *num);	delete []s;	}
//-----------------------------------------------------------------------------
/// Plot flows for vector field {ax,ay} parametrically depended on coordinate {x,y} with color proportional to value |a|
void mgl_flowp_xy_(uintptr_t *gr, float *x0, float *y0, float *z0, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch, int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_flowp_xy(_GR_, *x0,*y0,*z0, _DA_(x), _DA_(y), _DA_(ax), _DA_(ay), s);	delete []s;
}
/// Plot flows for vector field {ax,ay} with color proportional to value |a|
void mgl_flowp_2d_(uintptr_t *gr, float *x0, float *y0, float *z0, uintptr_t *ax, uintptr_t *ay, const char *sch, int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_flowp_2d(_GR_, *x0,*y0,*z0, _DA_(ax), _DA_(ay), s);	delete []s;	}
/// Plot flows for 3d vector field {ax,ay,ay} parametrically depended on coordinate {x,y,z} with color proportional to value |a|
void mgl_flowp_xyz_(uintptr_t *gr, float *x0, float *y0, float *z0, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_flowp_xyz(_GR_, *x0,*y0,*z0, _DA_(x), _DA_(y), _DA_(z), _DA_(ax), _DA_(ay), _DA_(az), s);	delete []s;	}
/// Plot flows for 3d vector field {ax,ay,ay} with color proportional to value |a|
void mgl_flowp_3d_(uintptr_t *gr, float *x0, float *y0, float *z0, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_flowp_3d(_GR_, *x0,*y0,*z0, _DA_(ax), _DA_(ay), _DA_(az), s);	delete []s;	}
//-----------------------------------------------------------------------------
/// Plot flow pipes for vector field {ax,ay} parametrically depended on coordinate {x,y} with color proportional to value |a|
void mgl_pipe_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch, float *r0, int *num, float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_pipe_xy(_GR_, _DA_(x), _DA_(y), _DA_(ax), _DA_(ay), s, *r0, *num, *zVal);	delete []s;	}
void mgl_pipe_2d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, const char *sch, float *r0, int *num, float *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_pipe_2d(_GR_, _DA_(ax), _DA_(ay), s, *r0, *num, *zVal);	delete []s;	}
/// Plot flow pipes for 3d vector field {ax,ay,ay} parametrically depended on coordinate {x,y,z} with color proportional to value |a|
void mgl_pipe_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, float *r0, int *num,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_pipe_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(ax), _DA_(ay), _DA_(az), s, *r0, *num);	delete []s;	}
void mgl_pipe_3d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, float *r0, int *num,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_pipe_3d(_GR_, _DA_(ax), _DA_(ay), _DA_(az), s, *r0, *num);	delete []s;	}
