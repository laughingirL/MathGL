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
	mglPoint p(x,y,z);	gr->ScalePoint(p,false);
	gr->mark_plot(gr->AddPntC(p,gr->CDef),mk);
}
//-----------------------------------------------------------------------------
void mgl_mark_(uintptr_t *gr, mreal *x, mreal *y, mreal *z, const char *pen,int l)
{	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	mgl_mark(_GR_, *x,*y,*z,s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_ball(HMGL gr, float x,float y,float z)
{
	mglPoint p(x,y,z);	gr->ScalePoint(p,false);
	gr->mark_plot(gr->AddPntC(p,gr->AddTexture('r')),'.');
}
//-----------------------------------------------------------------------------
void mgl_ball_(uintptr_t *gr, mreal *x,mreal *y,mreal *z)
{	mgl_ball(_GR_, *x,*y,*z);	}
//-----------------------------------------------------------------------------
void mgl_line(HMGL gr, float x1, float y1, float z1, float x2, float y2, float z2, const char *pen,int n)
{
	static int cgid=1;	gr->StartGroup("Line",cgid++);
	if(isnan(z1) || isnan(z2))	z1=z2=gr->Min.z;
	mglPoint p1(x1,y1,z1), p2(x2,y2,z2), p=p1;
	gr->SetPenPal(pen);
	n = (n<2) ? 2 : n;

	register long i,k1,k2;
	register float s;
	gr->ReserveC(n);
	gr->ScalePoint(p,false);	k1 = gr->AddPntC(p,gr->CDef);
	for(i=1;i<n;i++)
	{
		s = i/float(n-1);	p = p1*(1-s)+p2*s;	k2 = k1;
		gr->ScalePoint(p,false);	k1 = gr->AddPntC(p,gr->CDef);
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
	mglPoint p1(x1,y1,z1), p2(x2,y2,z2), d1(dx1,dy1,dz1), d2(dx2,dy2,dz2), a,b,p=p1;
	a = 3*(p2-p1)-d2-2*d1;	b = d1+d2-2*(p2-p1);
	n = (n<2) ? 2 : n;
	gr->SetPenPal(pen);

	register long i,k1,k2;
	register float s;
	gr->ReserveC(n);
	gr->ScalePoint(p,false);	k1=gr->AddPntC(p,gr->CDef);
	for(i=0;i<n;i++)
	{
		s = i/(n-1.);	p = p1+s*(d1+s*(a+s*b));	k2 = k1;
		gr->ScalePoint(p,false);	k1 = gr->AddPntC(p,gr->CDef);
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
	mglPoint p(x,y,z), q;
	gr->ReserveC(7);
	long k1,k2;
	q = p;	q.x += ex;	gr->ScalePoint(q,false);	k1 = gr->AddPntC(q,gr->CDef);
	q = p;	q.x -= ex;	gr->ScalePoint(q,false);	k2 = gr->AddPntC(q,gr->CDef);
	gr->line_plot(k1,k2);	gr->arrow_plot(k1,k2,'I');	gr->arrow_plot(k2,k1,'I');
	q = p;	q.y += ey;	gr->ScalePoint(q,false);	k1 = gr->AddPntC(q,gr->CDef);
	q = p;	q.y -= ey;	gr->ScalePoint(q,false);	k2 = gr->AddPntC(q,gr->CDef);
	gr->line_plot(k1,k2);	gr->arrow_plot(k1,k2,'I');	gr->arrow_plot(k2,k1,'I');
	q = p;	q.z += ez;	gr->ScalePoint(q,false);	k1 = gr->AddPntC(q,gr->CDef);
	q = p;	q.z -= ez;	gr->ScalePoint(q,false);	k2 = gr->AddPntC(q,gr->CDef);
	gr->line_plot(k1,k2);	gr->arrow_plot(k1,k2,'I');	gr->arrow_plot(k2,k1,'I');
	if(mk)
	{	gr->ScalePoint(p,false);	k1 = gr->AddPntC(p,gr->CDef);	gr->mark_plot(k1,mk);	}
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
	gr->ReserveN(4);
	long k1,k2,k3,k4;
	gr->ScalePoint(p1,false);		k1 = gr->AddPntN(p1,c1,q1);
	gr->ScalePoint(p2,false);		k2 = gr->AddPntN(p2,c2,q2);
	gr->ScalePoint(p3,false);		k3 = gr->AddPntN(p3,c3,q3);
	gr->ScalePoint(p4,false);		k4 = gr->AddPntN(p4,c4,q4);
	gr->quad_plot(k1,k2,k3,k4);
	if(strchr(stl,'#'))
	{
		gr->ReserveC(4);
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
	a=!d;	a/=Norm(a);		b=d^a;	b/=Norm(b);
	long ss=gr->AddTexture(stl);
	float c1=gr->GetC(ss,p1.z), c2=gr->GetC(ss,p2.z);
	long *kk=new long[164],k1=-1,k2=-1;
	gr->ReserveN(edge?166:82);
	if(edge)
	{
		p = p1;	gr->ScalePoint(p,false);	k1=gr->AddPntN(p,c1,d);
		p = p2;	gr->ScalePoint(p,false);	k2=gr->AddPntN(p,c2,d);
	}
	float f,si,co, dr=r2-r1;
	register long i;
	for(i=0;i<41;i++)
	{
		f = i*M_PI/20;	co = cos(f);	si = sin(f);
		p = p1+(r1*co)*a+(r1*si)*b;		gr->ScalePoint(p,false);
		q = (si*a-co*b)^(d + (dr*co)*a + (dr*si)*b);
		kk[i] = gr->AddPntN(p,c1,q);
		if(edge)	kk[i+82] = gr->AddPntN(p,c1,d);
		p = p2+(r2*co)*a+(r2*si)*b;		gr->ScalePoint(p,false);
		kk[i+41] = gr->AddPntN(p,c2,q);
		if(edge)	kk[i+123] = gr->AddPntN(p,c2,d);
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
	if(Norm(q)==0)	{	q = mglPoint(1,0,0);	sh=0;	}
	q = q/Norm(q);	p1 = !q;	p2 = q^p1;	r /= 2;

	static int cgid=1;	gr->StartGroup("Drop",cgid++);
	const int n = 41;
	register long i,j;
	long pos=gr->ReserveN(n*n);

	float u,v,x,y,z,rr,dr, co,si;
	for(i=0;i<n;i++)	for(j=0;j<n;j++)
	{
		u = i*M_PI/(n-1.);	v = 2*M_PI*j/(n-1.)-1;
		si = sin(u);	co = cos(u);
		rr = r*a*si*(1.+sh*co)/(1+sh);
		dr = r*a/(1+sh)*(co*(1.+sh*co) - sh*si*si);
		x = rr*cos(v);	y = rr*sin(v);
		z = r*(1+sh)*(co+sh);
		pp = p + p1*x + p2*y + q*z;	gr->ScalePoint(pp,false);
		qq = (p1*sin(v)-p2*cos(v))^(p1*(dr*cos(v)) + p2*(dr*sin(v)) - q*(r*(1+sh)*si));
		gr->AddPntN(pp,c,qq);
	}
	for(i=0;i<n-1;i++)	for(j=0;j<n-1;j++)
		gr->quad_plot(pos+j+i*n, pos+j+i*n+1, pos+j+i*n+n, pos+j+i*n+n+1);
	gr->EndGroup();
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
void mgl_dew_xy(HMGL gr, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch,float zVal)
{
	long i,j,n=ax->GetNx(),m=ax->GetNy(),k;
	if(n*m*ax->GetNz()!=ay->GetNx()*ay->GetNy()*ay->GetNz())	{	gr->SetWarn(mglWarnDim,"Dew");	return;	}
	if(n<2 || m<2)						{	gr->SetWarn(mglWarnLow,"Dew");	return;	}
	bool both = x->GetNx()==n && y->GetNx()==n && x->GetNy()==m && y->GetNy()==m;
	if(!(both || (x->GetNx()==n && y->GetNx()==m)))	{	gr->SetWarn(mglWarnDim,"Dew");	return;	}
	static int cgid=1;	gr->StartGroup("DewXY",cgid++);

	float xm,ym,dx,dy,dd;
	long ss = gr->AddTexture(sch);
	bool inv = sch && strchr(sch,'A');
	if(isnan(zVal))	zVal = gr->Min.z;
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
//			if(!gr->ScalePoint(p))	continue;	//	don't plot unvisible drops
			q = inv ? mglPoint(-ax->v(i,j,k),-ay->v(i,j,k),0) : mglPoint(ax->v(i,j,k),ay->v(i,j,k),0);
			p = mglPoint(xx, yy, zVal);		dd = Norm(q);
			float ccc = gr->GetC(ss,dd*xm,false);
			mgl_drop(gr,p,q,(dx<dy?dx:dy)/2,ccc,dd*xm,1);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_dew_2d(HMGL gr, HCDT ax, HCDT ay, const char *sch,float zVal)
{
	if(ax->GetNx()<2 || ax->GetNy()<2)	{	gr->SetWarn(mglWarnLow,"Vect");	return;	}
	mglData x(ax->GetNx()), y(ax->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_dew_xy(gr,&x,&y,ax,ay,sch,zVal);
}
//-----------------------------------------------------------------------------
void mgl_dew_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch, mreal *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_dew_xy(_GR_, _DA_(x), _DA_(y), _DA_(ax), _DA_(ay), s, *zVal);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_dew_2d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, const char *sch, mreal *zVal,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	mgl_dew_2d(_GR_, _DA_(ax), _DA_(ay), s, *zVal);	delete []s;	}
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
	gr->ScalePoint(p,false);
	long k = gr->AddPntN(p,-1,d);
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
void mgl_textmarkw_xyzr(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT r, const wchar_t *text, const char *fnt)
{
	long j,m,mx,my,mz,mr,n=y->GetNx();
	if(x->GetNx()!=n || z->GetNx()!=n || r->GetNx()!=n)
	{	gr->SetWarn(mglWarnDim,"Mark");	return;	}
	if(n<2)	{	gr->SetWarn(mglWarnLow,"Mark");	return;	}
	static int cgid=1;	gr->StartGroup("TextMark",cgid++);
	m = x->GetNy() > y->GetNy() ? x->GetNy() : y->GetNy();	m = z->GetNy() > m ? z->GetNy() : m;
	gr->ReserveN(n*m);

	mglPoint p,q(NAN);
	for(j=0;j<m;j++)
	{
		mx = j<x->GetNy() ? j:0;	my = j<y->GetNy() ? j:0;
		mz = j<z->GetNy() ? j:0;	mr = j<r->GetNy() ? j:0;
		register long i,k;
		for(i=0;i<n;i++)
		{
			p = mglPoint(x->v(i,mx), y->v(i,my), z->v(i,mz));
			gr->ScalePoint(p);	k = gr->AddPntN(p,-1,q);
			gr->text_plot(k, text, fnt, -0.5*fabs(r->v(i,mr)));
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_textmarkw_xyr(HMGL gr, HCDT x, HCDT y, HCDT r, const wchar_t *text, const char *fnt)
{
	mglData z(y->GetNx());	z.Fill(gr->Min.z,gr->Min.z);
	mgl_textmarkw_xyzr(gr,x,y,&z,r,text,fnt);
}
//-----------------------------------------------------------------------------
void mgl_textmarkw_yr(HMGL gr, HCDT y, HCDT r, const wchar_t *text, const char *fnt)
{
	if(y->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"TextMark");	return;	}
	mglData x(y->GetNx());	x.Fill(gr->Min.x,gr->Max.x);
	mglData z(y->GetNx());	z.Fill(gr->Min.z,gr->Min.z);
	mgl_textmarkw_xyzr(gr,&x,y,&z,r,text,fnt);
}
//-----------------------------------------------------------------------------
void mgl_textmarkw(HMGL gr, HCDT y, const wchar_t *text, const char *fnt)
{
	if(y->GetNx()<2)	{	gr->SetWarn(mglWarnLow,"TextMark");	return;	}
	mglData r(y->GetNx());	r.Fill(1,1);
	mglData x(y->GetNx());	x.Fill(gr->Min.x,gr->Max.x);
	mglData z(y->GetNx());	z.Fill(gr->Min.z,gr->Min.z);
	mgl_textmarkw_xyzr(gr,&x,y,&z,&r,text,fnt);
}
//-----------------------------------------------------------------------------
void mgl_textmark_xyzr(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT r, const char *str, const char *fnt)
{
	long s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];	mbstowcs(wcs,str,s);
	mgl_textmarkw_xyzr(gr, x, y, z, r, wcs, fnt);	delete []wcs;
}
//-----------------------------------------------------------------------------
void mgl_textmark_xyr(HMGL gr, HCDT x, HCDT y, HCDT r, const char *str, const char *fnt)
{
	long s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];	mbstowcs(wcs,str,s);
	mgl_textmarkw_xyr(gr, x, y, r, wcs, fnt);	delete []wcs;
}
//-----------------------------------------------------------------------------
void mgl_textmark_yr(HMGL gr, HCDT y, HCDT r, const char *str, const char *fnt)
{
	long s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];	mbstowcs(wcs,str,s);
	mgl_textmarkw_yr(gr, y, r, wcs, fnt);		delete []wcs;
}
//-----------------------------------------------------------------------------
void mgl_textmark(HMGL gr, HCDT y, const char *str, const char *fnt)
{
	long s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];	mbstowcs(wcs,str,s);
	mgl_textmarkw(gr, y, wcs, fnt);	delete []wcs;
}
//-----------------------------------------------------------------------------
void mgl_textmark_xyzr_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *r, const char *text, const char *fnt, int l,int n)
{	wchar_t *s=new wchar_t[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,fnt,n);	f[n]=0;
	mgl_textmarkw_xyzr(_GR_, _DA_(x),_DA_(y),_DA_(z),_DA_(r),s,f);	delete []s;		delete []f;	}
//-----------------------------------------------------------------------------
void mgl_textmark_xyr_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *r, const char *text, const char *fnt, int l,int n)
{	wchar_t *s=new wchar_t[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,fnt,n);	f[n]=0;
	mgl_textmarkw_xyr(_GR_, _DA_(x),_DA_(y),_DA_(r),s,f);	delete []s;		delete []f;	}
//-----------------------------------------------------------------------------
void mgl_textmark_yr_(uintptr_t *gr, uintptr_t *y, uintptr_t *r, const char *text, const char *fnt, int l,int n)
{	wchar_t *s=new wchar_t[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,fnt,n);	f[n]=0;
	mgl_textmarkw_yr(_GR_, _DA_(y),_DA_(r),s,f);	delete []s;		delete []f;	}
//-----------------------------------------------------------------------------
void mgl_textmark_(uintptr_t *gr, uintptr_t *y, const char *text, const char *fnt, int l,int n)
{	wchar_t *s=new wchar_t[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,fnt,n);	f[n]=0;
	mgl_textmarkw(_GR_, _DA_(y),s,f);	delete []s;		delete []f;	}
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
