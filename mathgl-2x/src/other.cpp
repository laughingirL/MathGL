/***************************************************************************
 * other.cpp is part of Math Graphic Library
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
#include "mgl2/other.h"
#include "mgl2/surf.h"
#include "mgl2/cont.h"
#include "mgl2/eval.h"
#include "mgl2/data.h"
#include <stdlib.h>
//-----------------------------------------------------------------------------
//
//	DensX, DensY, DensZ series
//
//-----------------------------------------------------------------------------
void mgl_dens_x(HMGL gr, HCDT a, const char *sch, float sv, const char *opt)
{
	register long i,j,k,n=a->GetNx(),m=a->GetNy(),l=a->GetNz();
	if(mgl_isnan(sv))	sv = gr->GetOrgX('x');
	if(n<2 || m<2)	{	gr->SetWarn(mglWarnLow,"DensX");	return;	}
	if(sv<gr->Min.x || sv>gr->Max.x)	{	gr->SetWarn(mglWarnSlc,"DensX");	return;	}
	mglData xx,yy,zz,aa;
	gr->SaveState(opt);

	if(l>1)
	{
		aa.Create(m,l);	xx.Create(m,l);	yy.Create(m,l);	zz.Create(m,l);
		float d = (n-1)*(sv - gr->Min.x)/(gr->Max.x - gr->Min.x);
		k = long(d);	d = d - k;
		if(k>n-2)	{	k=n-2;	d=1;	}
		if(k<0)		{	k=0;	d=0;	}
		for(j=0;j<l;j++)	for(i=0;i<m;i++)
			aa.a[i+m*j] = a->v(k,i,j)*(1-d) + d*a->v(k+1,i,j);
		a = &aa;
	}
	else
	{	xx.Create(n,m);	yy.Create(n,m);	zz.Create(n,m);	}
	xx.Fill(sv, sv);
	yy.Fill(gr->Min.y, gr->Max.y,'x');
	zz.Fill(gr->Min.z, gr->Max.z,'y');
	mgl_surfc_xy(gr,&xx,&yy,&zz,a,sch,0);
}
//-----------------------------------------------------------------------------
void mgl_dens_y(HMGL gr, HCDT a, const char *sch, float sv, const char *opt)
{
	register long i,j,k,n=a->GetNx(),m=a->GetNy(),l=a->GetNz();
	if(mgl_isnan(sv))	sv = gr->GetOrgX('x');
	if(n<2 || m<2)	{	gr->SetWarn(mglWarnLow,"DensY");	return;	}
	if(sv<gr->Min.x || sv>gr->Max.x)	{	gr->SetWarn(mglWarnSlc,"DensY");	return;	}
	mglData xx,yy,zz,aa;
	gr->SaveState(opt);

	if(l>1)
	{
		aa.Create(n,l);	xx.Create(n,l);	yy.Create(n,l);	zz.Create(n,l);
		float d = (m-1)*(sv - gr->Min.y)/(gr->Max.y - gr->Min.y);
		k = long(d);	d = d - k;
		if(k>m-2)	{	k=m-2;	d=1;	}
		if(k<0)		{	k=0;	d=0;	}
		for(j=0;j<l;j++)	for(i=0;i<n;i++)
			aa.a[i+n*j] = a->v(i,k,j)*(1-d) + d*a->v(i,k+1,j);
		a = &aa;
	}
	else
	{	xx.Create(n,m);	yy.Create(n,m);	zz.Create(n,m);	}
	yy.Fill(sv, sv);
	xx.Fill(gr->Min.x, gr->Max.x,'x');
	zz.Fill(gr->Min.z, gr->Max.z,'y');
	mgl_surfc_xy(gr,&xx,&yy,&zz,a,sch,0);
}
//-----------------------------------------------------------------------------
void mgl_dens_z(HMGL gr, HCDT a, const char *sch, float sv, const char *opt)
{
	register long i,j,k,n=a->GetNx(),m=a->GetNy(),l=a->GetNz();
	if(mgl_isnan(sv))	sv = gr->GetOrgX('x');
	if(n<2 || m<2)	{	gr->SetWarn(mglWarnLow,"DensZ");	return;	}
	if(sv<gr->Min.x || sv>gr->Max.x)	{	gr->SetWarn(mglWarnSlc,"DensZ");	return;	}
	mglData xx,yy,zz,aa;
	gr->SaveState(opt);

	xx.Create(n,m);	yy.Create(n,m);	zz.Create(n,m);
	if(l>1)
	{
		aa.Create(n,m);
		float d = (l-1)*(sv - gr->Min.z)/(gr->Max.z - gr->Min.z);
		k = long(d);	d = d - k;
		if(k>l-2)	{	k=l-2;	d=1;	}
		if(k<0)		{	k=0;	d=0;	}
		for(j=0;j<m;j++)	for(i=0;i<n;i++)
			aa.a[i+n*j] = a->v(i,j,k)*(1-d) + d*a->v(i,j,k+1);
		a = &aa;
	}
	zz.Fill(sv, sv);
	yy.Fill(gr->Min.y, gr->Max.y,'y');
	xx.Fill(gr->Min.x, gr->Max.x,'x');
	mgl_surfc_xy(gr,&xx,&yy,&zz,a,sch,0);
}
//-----------------------------------------------------------------------------
void mgl_dens_x_(uintptr_t *gr, uintptr_t *a, const char *sch, float *sv, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_dens_x(_GR_, _DA_(a), s, *sv, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_dens_y_(uintptr_t *gr, uintptr_t *a, const char *sch, float *sv, const char *opt,int l,int lo)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_dens_y(_GR_, _DA_(a), s, *sv, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_dens_z_(uintptr_t *gr, uintptr_t *a, const char *sch, float *sv, const char *opt,int l,int lo)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_dens_z(_GR_, _DA_(a), s, *sv, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	ContX, ContY, ContZ series
//
//-----------------------------------------------------------------------------
void mgl_cont_gen(HMGL gr, float val, HCDT a, HCDT x, HCDT y, HCDT z, float c, int text,long ak);
void mgl_cont_x_val(HMGL gr, HCDT v, HCDT a, const char *sch, float sv, const char *opt)
{
	register long i,j,k,n=a->GetNx(),m=a->GetNy(),l=a->GetNz();
	if(mgl_isnan(sv))	sv = gr->GetOrgX('x');
	if(n<2 || m<2)	{	gr->SetWarn(mglWarnLow,"ContX");	return;	}
	if(sv<gr->Min.x || sv>gr->Max.x)	{	gr->SetWarn(mglWarnSlc,"ContX");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("ContX",cgid++);
	mglData xx,yy,zz,aa;

	bool text=(sch && strchr(sch,'t'));
	long ss=gr->AddTexture(sch);
	gr->SetPenPal(sch);

	if(l>1)
	{
		aa.Create(m,l);	xx.Create(m,l);	yy.Create(m,l);	zz.Create(m,l);
		float d = (n-1)*(sv - gr->Min.x)/(gr->Max.x - gr->Min.x);
		k = long(d);	d = d - k;
		if(k>n-2)	{	k=n-2;	d=1;	}
		if(k<0)		{	k=0;	d=0;	}
		for(j=0;j<l;j++)	for(i=0;i<m;i++)
			aa.a[i+m*j] = a->v(k,i,j)*(1-d) + d*a->v(k+1,i,j);
		a = &aa;
	}
	else
	{	xx.Create(n,m);	yy.Create(n,m);	zz.Create(n,m);	}
	xx.Fill(sv, sv);
	yy.Fill(gr->Min.y, gr->Max.y,'x');
	zz.Fill(gr->Min.z, gr->Max.z,'y');
	for(i=0;i<v->GetNx();i++)
	{
		register float v0 = v->v(i);
		mgl_cont_gen(gr,v0,a,&xx,&yy,&zz,gr->GetC(ss,v0),text,0);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_cont_y_val(HMGL gr, HCDT v, HCDT a, const char *sch, float sv, const char *opt)
{
	register long i,j,k,n=a->GetNx(),m=a->GetNy(),l=a->GetNz();
	if(mgl_isnan(sv))	sv = gr->GetOrgX('x');
	if(n<2 || m<2)	{	gr->SetWarn(mglWarnLow,"ContY");	return;	}
	if(sv<gr->Min.x || sv>gr->Max.x)	{	gr->SetWarn(mglWarnSlc,"ContY");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("ContY",cgid++);
	mglData xx,yy,zz,aa;

	bool text=(sch && strchr(sch,'t'));
	long ss=gr->AddTexture(sch);
	gr->SetPenPal(sch);

	if(l>1)
	{
		aa.Create(n,l);	xx.Create(n,l);	yy.Create(n,l);	zz.Create(n,l);
		float d = (m-1)*(sv - gr->Min.y)/(gr->Max.y - gr->Min.y);
		k = long(d);	d = d - k;
		if(k>m-2)	{	k=m-2;	d=1;	}
		if(k<0)		{	k=0;	d=0;	}
		for(j=0;j<l;j++)	for(i=0;i<n;i++)
			aa.a[i+n*j] = a->v(i,k,j)*(1-d) + d*a->v(i,k+1,j);
		a = &aa;
	}
	else
	{	xx.Create(n,m);	yy.Create(n,m);	zz.Create(n,m);	}
	yy.Fill(sv, sv);
	xx.Fill(gr->Min.x, gr->Max.x,'x');
	zz.Fill(gr->Min.z, gr->Max.z,'y');
	for(i=0;i<v->GetNx();i++)
	{
		register float v0 = v->v(i);
		mgl_cont_gen(gr,v0,a,&xx,&yy,&zz,gr->GetC(ss,v0),text,0);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_cont_z_val(HMGL gr, HCDT v, HCDT a, const char *sch, float sv, const char *opt)
{
	register long i,j,k,n=a->GetNx(),m=a->GetNy(),l=a->GetNz();
	if(mgl_isnan(sv))	sv = gr->GetOrgX('x');
	if(n<2 || m<2)	{	gr->SetWarn(mglWarnLow,"ContZ");	return;	}
	if(sv<gr->Min.x || sv>gr->Max.x)	{	gr->SetWarn(mglWarnSlc,"ContZ");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("ContZ",cgid++);
	mglData xx,yy,zz,aa;

	bool text=(sch && strchr(sch,'t'));
	long ss=gr->AddTexture(sch);
	gr->SetPenPal(sch);

	xx.Create(n,m);	yy.Create(n,m);	zz.Create(n,m);
	if(l>1)
	{
		aa.Create(n,m);
		float d = (l-1)*(sv - gr->Min.z)/(gr->Max.z - gr->Min.z);
		k = long(d);	d = d - k;
		if(k>l-2)	{	k=l-2;	d=1;	}
		if(k<0)		{	k=0;	d=0;	}
		for(j=0;j<m;j++)	for(i=0;i<n;i++)
			aa.a[i+n*j] = a->v(i,j,k)*(1-d) + d*a->v(i,j,k+1);
		a = &aa;
	}
	zz.Fill(sv, sv);
	yy.Fill(gr->Min.y, gr->Max.y,'y');
	xx.Fill(gr->Min.x, gr->Max.x,'x');
	for(i=0;i<v->GetNx();i++)
	{
		register float v0 = v->v(i);
		mgl_cont_gen(gr,v0,a,&xx,&yy,&zz,gr->GetC(ss,v0),text,0);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_cont_x(HMGL gr, HCDT a, const char *sch, float sv, const char *opt)
{
	float r = gr->SaveState(opt);
	long Num = (mgl_isnan(r) || r<=0) ? 7:long(r+0.5);
	mglData v(Num);
	for(long i=0;i<Num;i++)	v.a[i] = gr->Min.c + (gr->Max.c-gr->Min.c)*float(i+1)/(Num+1);
	mgl_cont_x_val(gr,&v,a,sch,sv,0);
}
//-----------------------------------------------------------------------------
void mgl_cont_y(HMGL gr, HCDT a, const char *sch, float sv, const char *opt)
{
	float r = gr->SaveState(opt);
	long Num = (mgl_isnan(r) || r<=0) ? 7:long(r+0.5);
	mglData v(Num);
	for(long i=0;i<Num;i++)	v.a[i] = gr->Min.c + (gr->Max.c-gr->Min.c)*float(i+1)/(Num+1);
	mgl_cont_y_val(gr,&v,a,sch,sv,0);
}
//-----------------------------------------------------------------------------
void mgl_cont_z(HMGL gr, HCDT a, const char *sch, float sv, const char *opt)
{
	float r = gr->SaveState(opt);
	long Num = (mgl_isnan(r) || r<=0) ? 7:long(r+0.5);
	mglData v(Num);
	for(long i=0;i<Num;i++)	v.a[i] = gr->Min.c + (gr->Max.c-gr->Min.c)*float(i+1)/(Num+1);
	mgl_cont_z_val(gr,&v,a,sch,sv,0);
}
//-----------------------------------------------------------------------------
void mgl_cont_x_(uintptr_t *gr, uintptr_t *a, const char *sch, float *sv, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_cont_x(_GR_, _DA_(a), s, *sv, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_cont_y_(uintptr_t *gr, uintptr_t *a, const char *sch, float *sv, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_cont_y(_GR_, _DA_(a), s, *sv, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_cont_z_(uintptr_t *gr, uintptr_t *a, const char *sch, float *sv, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_cont_z(_GR_, _DA_(a), s, *sv, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_cont_x_val_(uintptr_t *gr, uintptr_t *v, uintptr_t *a, const char *sch, float *sv, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_cont_x_val(_GR_, _DA_(v), _DA_(a), s, *sv, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_cont_y_val_(uintptr_t *gr, uintptr_t *v, uintptr_t *a, const char *sch, float *sv, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_cont_y_val(_GR_, _DA_(v), _DA_(a), s, *sv, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_cont_z_val_(uintptr_t *gr, uintptr_t *v, uintptr_t *a, const char *sch, float *sv, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_cont_z_val(_GR_, _DA_(v), _DA_(a), s, *sv, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	ContFX, ContFY, ContFZ series
//
//-----------------------------------------------------------------------------
void mgl_contf_gen(HMGL gr, float v1, float v2, HCDT a, HCDT x, HCDT y, HCDT z, float c, long ak);
void mgl_contf_x_val(HMGL gr, HCDT v, HCDT a, const char *sch, float sv, const char *opt)
{
	register long i,j,k,n=a->GetNx(),m=a->GetNy(),l=a->GetNz();
	if(mgl_isnan(sv))	sv = gr->GetOrgX('x');
	if(n<2 || m<2)	{	gr->SetWarn(mglWarnLow,"ContFX");	return;	}
	if(sv<gr->Min.x || sv>gr->Max.x)	{	gr->SetWarn(mglWarnSlc,"ContFX");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("ContFX",cgid++);
	mglData xx,yy,zz,aa;
	long ss=gr->AddTexture(sch);

	if(l>1)
	{
		aa.Create(m,l);	xx.Create(m,l);	yy.Create(m,l);	zz.Create(m,l);
		float d = (n-1)*(sv - gr->Min.x)/(gr->Max.x - gr->Min.x);
		k = long(d);	d = d - k;
		if(k>n-2)	{	k=n-2;	d=1;	}
		if(k<0)		{	k=0;	d=0;	}
		for(j=0;j<l;j++)	for(i=0;i<m;i++)
			aa.a[i+m*j] = a->v(k,i,j)*(1-d) + d*a->v(k+1,i,j);
		a = &aa;
	}
	else
	{	xx.Create(n,m);	yy.Create(n,m);	zz.Create(n,m);	}
	xx.Fill(sv, sv);
	yy.Fill(gr->Min.y, gr->Max.y,'x');
	zz.Fill(gr->Min.z, gr->Max.z,'y');
	for(i=0;i<v->GetNx()-1;i++)
	{
		register float v0 = v->v(i);
		mgl_contf_gen(gr,v0,v->v(i+1),a,&xx,&yy,&zz,gr->GetC(ss,v0),0);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_contf_y_val(HMGL gr, HCDT v, HCDT a, const char *sch, float sv, const char *opt)
{
	register long i,j,k,n=a->GetNx(),m=a->GetNy(),l=a->GetNz();
	if(mgl_isnan(sv))	sv = gr->GetOrgX('x');
	if(n<2 || m<2)	{	gr->SetWarn(mglWarnLow,"ContFY");	return;	}
	if(sv<gr->Min.x || sv>gr->Max.x)	{	gr->SetWarn(mglWarnSlc,"ContFY");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("ContFY",cgid++);
	mglData xx,yy,zz,aa;
	long ss=gr->AddTexture(sch);

	if(l>1)
	{
		aa.Create(n,l);	xx.Create(n,l);	yy.Create(n,l);	zz.Create(n,l);
		float d = (m-1)*(sv - gr->Min.y)/(gr->Max.y - gr->Min.y);
		k = long(d);	d = d - k;
		if(k>m-2)	{	k=m-2;	d=1;	}
		if(k<0)		{	k=0;	d=0;	}
		for(j=0;j<l;j++)	for(i=0;i<n;i++)
			aa.a[i+n*j] = a->v(i,k,j)*(1-d) + d*a->v(i,k+1,j);
		a = &aa;
	}
	else
	{	xx.Create(n,m);	yy.Create(n,m);	zz.Create(n,m);	}
	yy.Fill(sv, sv);
	xx.Fill(gr->Min.x, gr->Max.x,'x');
	zz.Fill(gr->Min.z, gr->Max.z,'y');
	for(i=0;i<v->GetNx()-1;i++)
	{
		register float v0 = v->v(i);
		mgl_contf_gen(gr,v0,v->v(i+1),a,&xx,&yy,&zz,gr->GetC(ss,v0),0);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_contf_z_val(HMGL gr, HCDT v, HCDT a, const char *sch, float sv, const char *opt)
{
	register long i,j,k,n=a->GetNx(),m=a->GetNy(),l=a->GetNz();
	if(mgl_isnan(sv))	sv = gr->GetOrgX('x');
	if(n<2 || m<2)	{	gr->SetWarn(mglWarnLow,"ContFZ");	return;	}
	if(sv<gr->Min.x || sv>gr->Max.x)	{	gr->SetWarn(mglWarnSlc,"ContFZ");	return;	}
	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("ContFZ",cgid++);
	mglData xx,yy,zz,aa;
	long ss=gr->AddTexture(sch);

	xx.Create(n,m);	yy.Create(n,m);	zz.Create(n,m);
	if(l>1)
	{
		aa.Create(n,m);
		float d = (l-1)*(sv - gr->Min.z)/(gr->Max.z - gr->Min.z);
		k = long(d);	d = d - k;
		if(k>l-2)	{	k=l-2;	d=1;	}
		if(k<0)		{	k=0;	d=0;	}
		for(j=0;j<m;j++)	for(i=0;i<n;i++)
			aa.a[i+n*j] = a->v(i,j,k)*(1-d) + d*a->v(i,j,k+1);
		a = &aa;
	}
	zz.Fill(sv, sv);
	yy.Fill(gr->Min.y, gr->Max.y,'y');
	xx.Fill(gr->Min.x, gr->Max.x,'x');
	for(i=0;i<v->GetNx()-1;i++)
	{
		register float v0 = v->v(i);
		mgl_contf_gen(gr,v0,v->v(i+1),a,&xx,&yy,&zz,gr->GetC(ss,v0),0);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void mgl_contf_x(HMGL gr, HCDT a, const char *sch, float sv, const char *opt)
{
	float r = gr->SaveState(opt);
	long Num = (mgl_isnan(r) || r<=0) ? 7:long(r+0.5);
	mglData v(Num);	v.Fill(gr->Min.c, gr->Max.c);
	mgl_contf_x_val(gr,&v,a,sch,sv,0);
}
//-----------------------------------------------------------------------------
void mgl_contf_y(HMGL gr, HCDT a, const char *sch, float sv, const char *opt)
{
	float r = gr->SaveState(opt);
	long Num = (mgl_isnan(r) || r<=0) ? 7:long(r+0.5);
	mglData v(Num);	v.Fill(gr->Min.c, gr->Max.c);
	mgl_contf_y_val(gr,&v,a,sch,sv,0);
}
//-----------------------------------------------------------------------------
void mgl_contf_z(HMGL gr, HCDT a, const char *sch, float sv, const char *opt)
{
	float r = gr->SaveState(opt);
	long Num = (mgl_isnan(r) || r<=0) ? 7:long(r+0.5);
	mglData v(Num);	v.Fill(gr->Min.c, gr->Max.c);
	mgl_contf_z_val(gr,&v,a,sch,sv,0);
}
//-----------------------------------------------------------------------------
void mgl_contf_x_(uintptr_t *gr, uintptr_t *a, const char *sch, float *sv, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_contf_x(_GR_, _DA_(a), s, *sv, o);	delete []o;	delete []s;	}
/// Draw several contour plots for data a at y = *sv
void mgl_contf_y_(uintptr_t *gr, uintptr_t *a, const char *sch, float *sv, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_contf_y(_GR_, _DA_(a), s, *sv, o);	delete []o;	delete []s;	}
/// Draw several contour plots for data a at z = *sv
void mgl_contf_z_(uintptr_t *gr, uintptr_t *a, const char *sch, float *sv, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_contf_z(_GR_, _DA_(a), s, *sv, o);	delete []o;	delete []s;	}
/// Draw contour plots for data a at x = *sv
void mgl_contf_x_val_(uintptr_t *gr, uintptr_t *v, uintptr_t *a, const char *sch, float *sv, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_contf_x_val(_GR_, _DA_(v), _DA_(a), s, *sv, o);	delete []o;	delete []s;}
/// Draw contour plots for data a at y = *sv
void mgl_contf_y_val_(uintptr_t *gr, uintptr_t *v, uintptr_t *a, const char *sch, float *sv, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_contf_y_val(_GR_, _DA_(v), _DA_(a), s, *sv, o);	delete []o;	delete []s;}
/// Draw contour plots for data a at z = *sv
void mgl_contf_z_val_(uintptr_t *gr, uintptr_t *v, uintptr_t *a, const char *sch, float *sv, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_contf_z_val(_GR_, _DA_(v), _DA_(a), s, *sv, o);	delete []o;	delete []s;}
//-----------------------------------------------------------------------------
