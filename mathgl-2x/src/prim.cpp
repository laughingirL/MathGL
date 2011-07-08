/***************************************************************************
 * prim.cpp is part of Math Graphic Library
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
#include <wchar.h>
#include <stdlib.h>
#include "mgl/prim.h"
#include "mgl/data.h"
//-----------------------------------------------------------------------------
//
//	Mark & Curve series
//
//-----------------------------------------------------------------------------
void mgl_mark(HMGL gr, float x,float y,float z,const char *mark)
{
	char mk = gr->SetPenPal(mark);
	mglPoint p(x,y,z);
	gr->mark_plot(gr->AddPnt(p,gr->CDef,mglPoint(NAN),-1,3),mk);
}
//-----------------------------------------------------------------------------
void mgl_mark_(uintptr_t *gr, mreal *x, mreal *y, mreal *z, const char *pen,int l)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	mgl_mark(_GR_, *x,*y,*z,s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_ball(HMGL gr, float x,float y,float z)
{
	mglPoint p(x,y,z);
	gr->mark_plot(gr->AddPnt(p,gr->AddTexture('r'),mglPoint(NAN),-1,3),'.');
}
//-----------------------------------------------------------------------------
void mgl_ball_(uintptr_t *gr, mreal *x,mreal *y,mreal *z)
{	mgl_ball(_GR_, *x,*y,*z);	}
//-----------------------------------------------------------------------------
void mgl_line(HMGL gr, float x1, float y1, float z1, float x2, float y2, float z2, const char *pen,int n)
{
	static int cgid=1;	gr->StartGroup("Line",cgid++);
	if(isnan(z1) || isnan(z2))	z1=z2=gr->Min.z;
	mglPoint p1(x1,y1,z1), p2(x2,y2,z2), p=p1,nn=mglPoint(NAN);
	gr->SetPenPal(pen);
	n = (n<2) ? 2 : n;

	register long i,k1,k2;
	register float s;
	gr->Reserve(n);
	k1 = gr->AddPnt(p,gr->CDef,nn,-1,3);
	for(i=1;i<n;i++)
	{
		s = i/float(n-1);	p = p1*(1-s)+p2*s;	k2 = k1;
		k1 = gr->AddPnt(p,gr->CDef,nn,-1,3);
		gr->line_plot(k2,k1);
		if(i==1)	gr->arrow_plot(k2,k1,gr->Arrow1);
		if(i==n-1)	gr->arrow_plot(k1,k2,gr->Arrow2);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_line_(uintptr_t *gr, mreal *x1, mreal *y1, mreal *z1, mreal *x2, mreal *y2, mreal *z2, const char *pen,int *n,int l)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	mgl_line(_GR_, *x1,*y1,*z1, *x2,*y2,*z2,s,*n);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_curve(HMGL gr, float x1, float y1, float z1, float dx1, float dy1, float dz1, float x2, float y2, float z2, float dx2, float dy2, float dz2, const char *pen,int n)
{
	static int cgid=1;	gr->StartGroup("Curve",cgid++);
	mglPoint p1(x1,y1,z1), p2(x2,y2,z2), d1(dx1,dy1,dz1), d2(dx2,dy2,dz2), a,b,p=p1,nn=mglPoint(NAN);
	a = 3*(p2-p1)-d2-2*d1;	b = d1+d2-2*(p2-p1);
	n = (n<2) ? 2 : n;
	gr->SetPenPal(pen);

	register long i,k1,k2;
	register float s;
	gr->Reserve(n);
	k1=gr->AddPnt(p,gr->CDef,nn,-1,3);
	for(i=0;i<n;i++)
	{
		s = i/(n-1.);	p = p1+s*(d1+s*(a+s*b));	k2 = k1;
		k1 = gr->AddPnt(p,gr->CDef,nn,-1,3);
		gr->line_plot(k2,k1);
		if(i==1)	gr->arrow_plot(k2,k1,gr->Arrow1);
		if(i==n-1)	gr->arrow_plot(k1,k2,gr->Arrow2);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_curve_(uintptr_t* gr, mreal *x1, mreal *y1, mreal *z1, mreal *dx1, mreal *dy1, mreal *dz1, mreal *x2, mreal *y2, mreal *z2, mreal *dx2, mreal *dy2, mreal *dz2, const char *pen,int *n, int l)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	mgl_curve(_GR_, *x1,*y1,*z1, *dx1,*dy1,*dz1, *x2,*y2,*z2, *dx2,*dy2,*dz2, s, *n);	delete []s;}
//-----------------------------------------------------------------------------
void mgl_error_box(HMGL gr, float x, float y, float z, float ex, float ey, float ez, const char *pen)
{
	static int cgid=1;	gr->StartGroup("ErBox",cgid++);
	char mk=gr->SetPenPal(pen);
	mglPoint p(x,y,z), q,nn=mglPoint(NAN);
	gr->Reserve(7);
	long k1,k2;
	q = p;	q.x += ex;	k1 = gr->AddPnt(q,gr->CDef,nn,0,3);
	q = p;	q.x -= ex;	k2 = gr->AddPnt(q,gr->CDef,nn,0,3);
	gr->line_plot(k1,k2);	gr->arrow_plot(k1,k2,'I');	gr->arrow_plot(k2,k1,'I');
	q = p;	q.y += ey;	k1 = gr->AddPnt(q,gr->CDef,nn,0,3);
	q = p;	q.y -= ey;	k2 = gr->AddPnt(q,gr->CDef,nn,0,3);
	gr->line_plot(k1,k2);	gr->arrow_plot(k1,k2,'I');	gr->arrow_plot(k2,k1,'I');
	q = p;	q.z += ez;	k1 = gr->AddPnt(q,gr->CDef,nn,0,3);
	q = p;	q.z -= ez;	k2 = gr->AddPnt(q,gr->CDef,nn,0,3);
	gr->line_plot(k1,k2);	gr->arrow_plot(k1,k2,'I');	gr->arrow_plot(k2,k1,'I');
	if(mk)	gr->mark_plot(gr->AddPnt(p,gr->CDef,nn,0,3),mk);
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_error_box_(uintptr_t *gr, mreal *x1, mreal *y1, mreal *z1, mreal *x2, mreal *y2, mreal *z2, const char *pen,int l)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	mgl_error_box(_GR_, *x1,*y1,*z1, *x2,*y2,*z2,s);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Face series
//
//-----------------------------------------------------------------------------
void mgl_face(HMGL gr, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, const char *stl)
{
	static int cgid=1;	gr->StartGroup("Face",cgid++);
	long pal;
	gr->SetPenPal(stl,&pal);
	float c1,c2,c3,c4,zz=(gr->Min.z+gr->Max.z)/2;
	c1=c2=c3=c4=gr->CDef;
	if(isnan(z0))	z0 = zz;	if(isnan(z1))	z1 = zz;
	if(isnan(z2))	z2 = zz;	if(isnan(z3))	z3 = zz;
	mglPoint p1(x0,y0,z0), p2(x1,y1,z1), p3(x2,y2,z2), p4(x3,y3,z3);
	if(gr->GetNumPal(pal)>=4)
	{	c2=gr->NextColor(pal);	c3=gr->NextColor(pal);	c4=gr->NextColor(pal);	}
	mglPoint q1,q2,q3,q4;
	q1 = (p2-p1)^(p3-p1);	q4 = (p2-p4)^(p3-p4);
	q2 = (p1-p2)^(p4-p2);	q3 = (p1-p3)^(p4-p3);
	gr->Reserve(4);
	long k1,k2,k3,k4;
	k1 = gr->AddPnt(p1,c1,q1,-1,3);	k2 = gr->AddPnt(p2,c2,q2,-1,3);
	k3 = gr->AddPnt(p3,c3,q3,-1,3);	k4 = gr->AddPnt(p4,c4,q4,-1,3);
	gr->quad_plot(k1,k2,k3,k4);
	if(strchr(stl,'#'))
	{
		gr->Reserve(4);
		pal = gr->AddTexture('k');
		k1=gr->CopyNtoC(k1,pal);	k2=gr->CopyNtoC(k2,pal);
		k3=gr->CopyNtoC(k3,pal);	k4=gr->CopyNtoC(k4,pal);
		gr->line_plot(k1,k2);		gr->line_plot(k1,k3);
		gr->line_plot(k3,k4);		gr->line_plot(k2,k4);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_facex(HMGL gr, float x0, float y0, float z0, float wy, float wz, const char *stl, float d1, float d2)
{	mgl_face(gr, x0,y0,z0, x0,y0+wy,z0, x0,y0,z0+wz, x0,y0+wy+d1,z0+wz+d2, stl);	}
//-----------------------------------------------------------------------------
void mgl_facey(HMGL gr, float x0, float y0, float z0, float wx, float wz, const char *stl, float d1, float d2)
{	mgl_face(gr, x0,y0,z0, x0+wx,y0,z0, x0,y0,z0+wz, x0+wx+d1,y0,z0+wz+d2, stl);	}
//-----------------------------------------------------------------------------
void mgl_facez(HMGL gr, float x0, float y0, float z0, float wx, float wy, const char *stl, float d1, float d2)
{	mgl_face(gr, x0,y0,z0, x0,y0+wy,z0, x0+wx,y0,z0, x0+wx+d1,y0+wy+d2,z0, stl);	}
//-----------------------------------------------------------------------------
void mgl_facex_(uintptr_t* gr, mreal *x0, mreal *y0, mreal *z0, mreal *wy, mreal *wz, const char *stl, mreal *dx, mreal *dy, int l)
{	char *s=new char[l+1];	memcpy(s,stl,l);	s[l]=0;
	mgl_facex(_GR_, *x0,*y0,*z0,*wy,*wz,s,*dx,*dy);	delete []s;
}
//-----------------------------------------------------------------------------
void mgl_facey_(uintptr_t* gr, mreal *x0, mreal *y0, mreal *z0, mreal *wx, mreal *wz, const char *stl, mreal *dx, mreal *dy, int l)
{	char *s=new char[l+1];	memcpy(s,stl,l);	s[l]=0;
	mgl_facey(_GR_, *x0,*y0,*z0,*wx,*wz,s,*dx,*dy);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_facez_(uintptr_t* gr, mreal *x0, mreal *y0, mreal *z0, mreal *wx, mreal *wy, const char *stl, mreal *dx, mreal *dy, int l)
{	char *s=new char[l+1];	memcpy(s,stl,l);	s[l]=0;
	mgl_facez(_GR_, *x0,*y0,*z0,*wx,*wy,s,*dx,*dy);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_face_(uintptr_t* gr, float *x0, float *y0, float *z0, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2, float *x3, float *y3, float *z3, const char *stl, int l)
{	char *s=new char[l+1];	memcpy(s,stl,l);	s[l]=0;
	mgl_face(_GR_, *x0,*y0,*z0, *x1,*y1,*z1, *x2,*y2,*z2, *x3,*y3,*z3, stl);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Cone
//
//-----------------------------------------------------------------------------
void mgl_cone(HMGL gr, float x1, float y1, float z1, float x2, float y2, float z2, float r1, float r2, const char *stl, int edge)
{
	if(r2<0)	r2=r1;
	if(r1==0 && r2==0)	return;

	static int cgid=1;	gr->StartGroup("Cone",cgid++);
	mglPoint p1(x1,y1,z1), p2(x2,y2,z2), p,q, d=p2-p1,a,b;
	a=!d;	a.Normalize();		b=d^a;	b.Normalize();
	long ss=gr->AddTexture(stl);
	float c1=gr->GetC(ss,p1.z), c2=gr->GetC(ss,p2.z);
	long *kk=new long[164],k1=-1,k2=-1;
	gr->Reserve(edge?166:82);
	if(edge)
	{
		k1=gr->AddPnt(p1,c1,d,-1,3);
		k2=gr->AddPnt(p2,c2,d,-1,3);
	}
	float f,si,co, dr=r2-r1;
	register long i;
	for(i=0;i<41;i++)
	{
		f = i*M_PI/20;	co = cos(f);	si = sin(f);
		p = p1+(r1*co)*a+(r1*si)*b;
		q = (si*a-co*b)^(d + (dr*co)*a + (dr*si)*b);
		kk[i] = gr->AddPnt(p,c1,q,-1,3);
		if(edge)	kk[i+82] = gr->AddPnt(p,c1,d,-1,3);
		p = p2+(r2*co)*a+(r2*si)*b;
		kk[i+41] = gr->AddPnt(p,c2,q,-1,3);
		if(edge)	kk[i+123] = gr->AddPnt(p,c2,d,-1,3);
	}
	for(i=0;i<40;i++)
	{
		gr->quad_plot(kk[i],kk[i+1],kk[i+41],kk[i+42]);
		if(edge)
		{
			gr->trig_plot(k1,kk[i+82],kk[i+83]);
			gr->trig_plot(k2,kk[i+123],kk[i+124]);
		}
	}
	gr->EndGroup();	delete []kk;
}
//-----------------------------------------------------------------------------
void mgl_cone_(uintptr_t* gr, mreal *x1, mreal *y1, mreal *z1, mreal *x2, mreal *y2, mreal *z2, mreal *r1, mreal *r2, const char *stl, int *edge, int l)
{	char *s=new char[l+1];	memcpy(s,stl,l);	s[l]=0;
	mgl_cone(_GR_, *x1,*y1,*z1, *x2,*y2,*z2,*r1,*r2,s,*edge);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Ellipse & Rhomb
//
//-----------------------------------------------------------------------------
void mgl_ellipse(HMGL gr, float x1, float y1, float z1, float x2, float y2, float z2, float r, const char *stl)
{
	const int n = 41;
	long pal,n0,n1=-1,n2,m1=-1,m2;
	gr->SetPenPal(stl,&pal);
	float c=gr->NextColor(pal);
	float k=(gr->GetNumPal(pal)>1)?gr->NextColor(pal):gr->AddTexture('k');
	bool wire = !(stl && strchr(stl,'#'));

	gr->Reserve(2*n+1);
	mglPoint p1(x1,y1,z1), p2(x2,y2,z2), v=p2-p1, u=mglPoint(0,0,1)^v, q=u^v, p, s;
	u = (r/u.norm())*u;	s = (p1+p2)/2.;	v *=0.5+r/v.norm();
	// central point first
	n0 = gr->AddPnt(p1,c,q,-1,3);
	for(long i=0;i<n;i++)
	{
		float t = i*2*M_PI/(n-1.);
		p = s+v*cos(t)+u*sin(t);
		n2 = n1;	n1 = gr->AddPnt(p,c,q,-1,3);
		m2 = m1;	m1 = gr->CopyNtoC(n1,k);
		if(i>0)
		{
			if(wire)	gr->trig_plot(n0,n1,n2);
			gr->line_plot(m1,m2);
		}
	}
}
//-----------------------------------------------------------------------------
void mgl_rhomb(HMGL gr, float x1, float y1, float z1, float x2, float y2, float z2, float r, const char *stl)
{
	long pal, n1,n2,n3,n4;
	gr->SetPenPal(stl,&pal);
	float c=gr->NextColor(pal);
	float k=(gr->GetNumPal(pal)>1)?gr->NextColor(pal):gr->AddTexture('k');
	bool wire = !(stl && strchr(stl,'#'));
	gr->Reserve(8);
	mglPoint p1(x1,y1,z1), p2(x2,y2,z2), u=mglPoint(0,0,1)^(p1-p2), q=u^(p1-p2), p, s,qq;
	u = (r/u.norm())*u;	s = (p1+p2)/2.;
	p = p1;	q = qq;	n1 = gr->AddPnt(p,c,qq,-1,3);
	p = s+u;q = qq;	n2 = gr->AddPnt(p,c,qq,-1,3);
	p = p2;	q = qq;	n3 = gr->AddPnt(p,c,qq,-1,3);
	p = s-u;q = qq;	n4 = gr->AddPnt(p,c,qq,-1,3);
	if(wire)	gr->quad_plot(n1,n2,n3,n4);
	n1 = gr->CopyNtoC(n1,k);	n2 = gr->CopyNtoC(n2,k);
	n3 = gr->CopyNtoC(n3,k);	n4 = gr->CopyNtoC(n4,k);
	gr->line_plot(n1,n2);	gr->line_plot(n2,n3);
	gr->line_plot(n3,n4);	gr->line_plot(n4,n1);
}
//-----------------------------------------------------------------------------
void mgl_ellipse_(uintptr_t* gr, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2, float *r, const char *stl,int l)
{	char *s=new char[l+1];	memcpy(s,stl,l);	s[l]=0;
	mgl_ellipse(_GR_,*x1,*y1,*z1,*x2,*y2,*z2,*r,s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_rhomb_(uintptr_t* gr, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2, float *r, const char *stl,int l)
{	char *s=new char[l+1];	memcpy(s,stl,l);	s[l]=0;
	mgl_rhomb(_GR_,*x1,*y1,*z1,*x2,*y2,*z2,*r,s);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Sphere & Drop
//
//-----------------------------------------------------------------------------
void mgl_sphere(HMGL gr, float x, float y, float z, float r, const char *stl)
{	mgl_drop(gr,x,y,z,1,0,0,2*r,stl,0,1);	}
//-----------------------------------------------------------------------------
void mgl_sphere_(uintptr_t* gr, mreal *x, mreal *y, mreal *z, mreal *r, const char *stl,int l)
{	char *s=new char[l+1];	memcpy(s,stl,l);	s[l]=0;
	mgl_sphere(_GR_, *x,*y,*z,*r,s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_drop(HMGL gr, mglPoint p, mglPoint q, float r, float c, float sh, float a)
{
	mglPoint p1,p2,pp,qq;
	if(q.norm()==0)	{	q = mglPoint(1,0,0);	sh=0;	}
	q.Normalize();	p1 = !q;	p2 = q^p1;	r /= 2;

	static int cgid=1;	gr->StartGroup("Drop",cgid++);
	const int n = 41;
	register long i,j;
	gr->Reserve(n*n);
	long *nn=new long[2*n];

	float u,v,x,y,z,rr,dr, co,si;
	for(i=0;i<n;i++)	for(j=0;j<n;j++)
	{
		u = i*M_PI/(n-1.);	v = 2*M_PI*j/(n-1.)-1;
		si = sin(u);	co = cos(u);
		rr = r*a*si*(1.+sh*co)/(1+sh);
		dr = r*a/(1+sh)*(co*(1.+sh*co) - sh*si*si);
		x = rr*cos(v);	y = rr*sin(v);
		z = r*(1+sh)*(co+sh);
		pp = p + p1*x + p2*y + q*z;
		qq = (p1*sin(v)-p2*cos(v))^(p1*(dr*cos(v)) + p2*(dr*sin(v)) - q*(r*(1+sh)*si));
		nn[j+n]=nn[j];	nn[j]=gr->AddPnt(pp,c,qq,-1,3);
		if(i*j>0)	gr->quad_plot(nn[j-1], nn[j], nn[j+n-1], nn[j+n]);
	}
	delete []nn;	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_drop(HMGL gr, float x1, float y1, float z1, float x2, float y2, float z2, float r, const char *stl, float sh, float a)
{
	float c=gr->AddTexture((stl && stl[0]) ? stl[0]:'r');
	mgl_drop(gr,mglPoint(x1,y1,z1), mglPoint(x2,y2,z2), r, c, sh, a);
}
//-----------------------------------------------------------------------------
void mgl_drop_(uintptr_t* gr, mreal *x1, mreal *y1, mreal *z1, mreal *x2, mreal *y2, mreal *z2, mreal *r, const char *stl, mreal *shift, mreal *ap, int l)
{	char *s=new char[l+1];	memcpy(s,stl,l);	s[l]=0;
	mgl_drop(_GR_, *x1,*y1,*z1, *x2,*y2,*z2, *r,s,*shift,*ap);	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Dew series
//
//-----------------------------------------------------------------------------
void mgl_dew_xy(HMGL gr, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch, const char *opt)
{
	long i,j,n=ax->GetNx(),m=ax->GetNy(),k;
	if(n*m*ax->GetNz()!=ay->GetNx()*ay->GetNy()*ay->GetNz())	{	gr->SetWarn(mglWarnDim,"Dew");	return;	}
	if(n<2 || m<2)						{	gr->SetWarn(mglWarnLow,"Dew");	return;	}
	bool both = x->GetNx()==n && y->GetNx()==n && x->GetNy()==m && y->GetNy()==m;
	if(!(both || (x->GetNx()==n && y->GetNx()==m)))	{	gr->SetWarn(mglWarnDim,"Dew");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("DewXY",cgid++);

	float xm,ym,dx,dy,dd;
	long ss = gr->AddTexture(sch);
	bool inv = sch && strchr(sch,'A');
	float	zVal = gr->Min.z;
	long tx=1,ty=1;
	if(gr->MeshNum>1)	{	tx=(n-1)/(gr->MeshNum-1);	ty=(m-1)/(gr->MeshNum-1);	}
	if(tx<1)	tx=1;	if(ty<1)	ty=1;

	for(i=0,xm=0;i<m*n*ax->GetNz();i++)
	{
		ym = hypot(ax->vthr(i),ay->vthr(i));
		xm = xm>ym ? xm : ym;
	}
	xm = 1./MGL_FLT_EPS/(xm==0 ? 1:xm);
	mglPoint p,q;

	for(k=0;k<ax->GetNz();k++)
	{
		if(ax->GetNz()>1)	zVal = gr->Min.z+(gr->Max.z-gr->Min.z)*float(k)/(ax->GetNz()-1);
		for(i=0;i<n;i+=tx)	for(j=0;j<m;j+=ty)
		{
			float xx=GetX(x,i,j,k).x, yy=GetY(y,i,j,k).x;
			dx = i<n-1 ? (GetX(x,i+1,j,k).x-xx) : (xx-GetX(x,i-1,j,k).x);
			dy = j<m-1 ? (GetY(y,i,j+1,k).x-yy) : (yy-GetY(y,i,j-1,k).x);
			dx *= tx;	dy *= ty;

			p = mglPoint(xx, yy, zVal);
			q = inv ? mglPoint(-ax->v(i,j,k),-ay->v(i,j,k),0) : mglPoint(ax->v(i,j,k),ay->v(i,j,k),0);
			p = mglPoint(xx, yy, zVal);		dd = q.norm();
			float ccc = gr->GetC(ss,dd*xm,false);
			mgl_drop(gr,p,q,(dx<dy?dx:dy)/2,ccc,dd*xm,1);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_dew_2d(HMGL gr, HCDT ax, HCDT ay, const char *sch, const char *opt)
{
	if(ax->GetNx()<2 || ax->GetNy()<2)	{	gr->SetWarn(mglWarnLow,"Vect");	return;	}
	gr->SaveState(opt);
	mglData x(ax->GetNx()), y(ax->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_dew_xy(gr,&x,&y,ax,ay,sch,0);
}
//-----------------------------------------------------------------------------
void mgl_dew_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_dew_xy(_GR_, _DA_(x), _DA_(y), _DA_(ax), _DA_(ay), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_dew_2d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_dew_2d(_GR_, _DA_(ax), _DA_(ay), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Puts series
//
//-----------------------------------------------------------------------------
void mgl_puts(HMGL gr, float x, float y, float z,const char *text, const char *font, float size)
{	mgl_puts_dir(gr, x, y, z, NAN, 0, 0, text, font, size);	}
void mgl_putsw(HMGL gr, float x, float y, float z,const wchar_t *text, const char *font, float size)
{	mgl_putsw_dir(gr, x, y, z, NAN, 0, 0, text, font, size);	}
//-----------------------------------------------------------------------------
void mgl_puts_dir(HMGL gr, float x, float y, float z, float dx, float dy, float dz, const char *text, const char *font, float size)
{
	long len = strlen(text);
	wchar_t *buf = new wchar_t[len+1];
	mbstowcs(buf,text,len);	buf[len]=0;
	mgl_putsw_dir(gr, x, y, z, dx, dy, dz, buf, font, size);
	delete []buf;
}
//-----------------------------------------------------------------------------
void mgl_putsw_dir(HMGL gr, float x, float y, float z, float dx, float dy, float dz, const wchar_t *text, const char *font, float size)
{
	mglPoint p(x,y,z), d(dx,dy,dz);
	long k = gr->AddPnt(p,-1,d,-1,3);
	gr->text_plot(k,text,font,size);
}
//-----------------------------------------------------------------------------
void mgl_puts_(uintptr_t *gr, float *x, float *y, float *z,const char *text, const char *font, float *size, int l, int n)
{	wchar_t *s=new wchar_t[l+1];	mbstowcs(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,font,n);	f[n]=0;
	mgl_putsw_dir(_GR_, *x, *y, *z, NAN, 0, 0, s, f, *size);
	delete []s;	delete []f;	}
//-----------------------------------------------------------------------------
void mgl_puts_dir_(uintptr_t *gr, float *x, float *y, float *z, float *dx, float *dy, float *dz, const char *text, const char *font, float *size, int l, int n)
{	wchar_t *s=new wchar_t[l+1];	mbstowcs(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,font,n);	f[n]=0;
	mgl_putsw_dir(_GR_, *x, *y, *z, *dx, *dy, *dz, s, f, *size);
	delete []s;	delete []f;	}
//-----------------------------------------------------------------------------
//
//	TextMark series
//
//-----------------------------------------------------------------------------
void mgl_textmarkw_xyzr(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT r, const wchar_t *text, const char *fnt, const char *opt)
{
	long j,m,mx,my,mz,mr,n=y->GetNx();
	if(x->GetNx()!=n || z->GetNx()!=n || r->GetNx()!=n)
	{	gr->SetWarn(mglWarnDim,"Mark");	return;	}
	if(n<2)	{	gr->SetWarn(mglWarnLow,"Mark");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("TextMark",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();	m = z->GetNy() > m ? z->GetNy() : m;
	gr->Reserve(n*m);

	mglPoint p,q(NAN);
	for(j=0;j<m;j++)
	{
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		mz = j<z->GetNy() ? j:0;	mr = j<r->GetNy() ? j:0;
		register long i,k;
		for(i=0;i<n;i++)
		{
			p = mglPoint(x->v(i,mx), y->v(i,my), z->v(i,mz));
			k = gr->AddPnt(p,-1,q);
			gr->text_plot(k, text, fnt, -0.5*fabs(r->v(i,mr)));
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_textmarkw_xyr(HMGL gr, HCDT x, HCDT y, HCDT r, const wchar_t *text, const char *fnt, const char *opt)
{
	gr->SaveState(opt);
	mglData z(y->GetNx());	z.Fill(gr->Min.z,gr->Min.z);
	mgl_textmarkw_xyzr(gr,x,y,&z,r,text,fnt,0);
}
//-----------------------------------------------------------------------------
void mgl_textmarkw_yr(HMGL gr, HCDT y, HCDT r, const wchar_t *text, const char *fnt, const char *opt)
{
	if(y->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"TextMark");	return;	}
	gr->SaveState(opt);
	mglData x(y->GetNx());	x.Fill(gr->Min.x,gr->Max.x);
	mglData z(y->GetNx());	z.Fill(gr->Min.z,gr->Min.z);
	mgl_textmarkw_xyzr(gr,&x,y,&z,r,text,fnt,0);
}
//-----------------------------------------------------------------------------
void mgl_textmarkw(HMGL gr, HCDT y, const wchar_t *text, const char *fnt, const char *opt)
{
	if(y->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"TextMark");	return;	}
	gr->SaveState(opt);
	mglData r(y->GetNx());	r.Fill(1,1);
	mglData x(y->GetNx());	x.Fill(gr->Min.x,gr->Max.x);
	mglData z(y->GetNx());	z.Fill(gr->Min.z,gr->Min.z);
	mgl_textmarkw_xyzr(gr,&x,y,&z,&r,text,fnt,0);
}
//-----------------------------------------------------------------------------
void mgl_textmark_xyzr(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT r, const char *str, const char *fnt, const char *opt)
{	long s = strlen(str)+1;	wchar_t *wcs = new wchar_t[s];	mbstowcs(wcs,str,s);
	mgl_textmarkw_xyzr(gr, x, y, z, r, wcs, fnt, opt);	delete []wcs;	}
//-----------------------------------------------------------------------------
void mgl_textmark_xyr(HMGL gr, HCDT x, HCDT y, HCDT r, const char *str, const char *fnt, const char *opt)
{	long s = strlen(str)+1;	wchar_t *wcs = new wchar_t[s];	mbstowcs(wcs,str,s);
	mgl_textmarkw_xyr(gr, x, y, r, wcs, fnt, opt);	delete []wcs;	}
//-----------------------------------------------------------------------------
void mgl_textmark_yr(HMGL gr, HCDT y, HCDT r, const char *str, const char *fnt, const char *opt)
{	long s = strlen(str)+1;	wchar_t *wcs = new wchar_t[s];	mbstowcs(wcs,str,s);
	mgl_textmarkw_yr(gr, y, r, wcs, fnt, opt);	delete []wcs;	}
//-----------------------------------------------------------------------------
void mgl_textmark(HMGL gr, HCDT y, const char *str, const char *fnt, const char *opt)
{	long s = strlen(str)+1;	wchar_t *wcs = new wchar_t[s];	mbstowcs(wcs,str,s);
	mgl_textmarkw(gr, y, wcs, fnt, opt);	delete []wcs;	}
//-----------------------------------------------------------------------------
void mgl_textmark_xyzr_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *r, const char *text, const char *fnt, const char *opt, int l,int n,int lo)
{	wchar_t *s=new wchar_t[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,fnt,n);	f[n]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_textmarkw_xyzr(_GR_, _DA_(x),_DA_(y),_DA_(z),_DA_(r),s,f, o);
	delete []o;	delete []s;		delete []f;	}
//-----------------------------------------------------------------------------
void mgl_textmark_xyr_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *r, const char *text, const char *fnt, const char *opt, int l,int n,int lo)
{	wchar_t *s=new wchar_t[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,fnt,n);	f[n]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_textmarkw_xyr(_GR_, _DA_(x),_DA_(y),_DA_(r),s,f, o);
	delete []o;	delete []s;	delete []f;	}
//-----------------------------------------------------------------------------
void mgl_textmark_yr_(uintptr_t *gr, uintptr_t *y, uintptr_t *r, const char *text, const char *fnt, const char *opt, int l,int n,int lo)
{	wchar_t *s=new wchar_t[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,fnt,n);	f[n]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_textmarkw_yr(_GR_, _DA_(y),_DA_(r),s,f, o);	delete []o;	delete []s;	delete []f;	}
//-----------------------------------------------------------------------------
void mgl_textmark_(uintptr_t *gr, uintptr_t *y, const char *text, const char *fnt, const char *opt, int l,int n,int lo)
{	wchar_t *s=new wchar_t[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,fnt,n);	f[n]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_textmarkw(_GR_, _DA_(y),s,f, o);	delete []o;	delete []s;	delete []f;	}
//-----------------------------------------------------------------------------
//
//	PutsFit series
//
//-----------------------------------------------------------------------------
extern char mglFitRes[1024];	///< Last fitted formula
void mgl_puts_fit(HMGL gr, float x, float y, float z, const char *pre, const char *font, float size)
{
	long n = strlen(mglFitRes)+(pre?strlen(pre):0)+1;
	char *buf = new char[n];
	if(pre)	sprintf(buf,"%s%s",pre,mglFitRes);
	else	strcpy(buf,mglFitRes);
	mgl_puts(gr,x,y,z,buf,font,size);
	delete []buf;
}
void mgl_puts_fit_(uintptr_t* gr, mreal *x, mreal *y, mreal *z, const char *prefix, const char *font, mreal *size, int l, int n)
{
	char *s=new char[l+1];	memcpy(s,prefix,l);	s[l]=0;
	char *d=new char[n+1];	memcpy(d,font,n);	d[n]=0;
	mgl_puts_fit(_GR_, *x,*y,*z, s, d, *size);
	delete []s;		delete []d;
}
//-----------------------------------------------------------------------------
