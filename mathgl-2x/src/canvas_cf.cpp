/***************************************************************************
 * canvas_cf.cpp is part of Math Graphic Library
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
#include "mgl/canvas.h"
//-----------------------------------------------------------------------------
#undef _GR_
#define _GR_	((mglCanvas *)(*gr))
#define _Gr_	((mglCanvas *)(gr))
//-----------------------------------------------------------------------------
const unsigned char *mgl_get_rgb(HMGL gr)	{	return _Gr_->GetBits();	}
const unsigned char *mgl_get_rgba(HMGL gr)	{	return _Gr_->GetRGBA();	}
int mgl_get_width(HMGL gr)		{	return _Gr_->GetWidth();	}
int mgl_get_height(HMGL gr)		{	return _Gr_->GetHeight();	}
void mgl_calc_xyz(HMGL gr, int xs, int ys, mreal *x, mreal *y, mreal *z)
{	mglPoint p = _Gr_->CalcXYZ(xs,ys);	*x = p.x;	*y = p.y;	*z = p.z;	}
void mgl_calc_scr(HMGL gr, mreal x, mreal y, mreal z, int *xs, int *ys)
{	_Gr_->CalcScr(mglPoint(x,y,z),xs,ys);	}
//-----------------------------------------------------------------------------
int mgl_new_frame(HMGL gr)		{	return _Gr_->NewFrame();	}
void mgl_end_frame(HMGL gr)		{	_Gr_->EndFrame();	}
int mgl_get_num_frame(HMGL gr)	{	return _Gr_->GetNumFrame();	}
void mgl_reset_frames(HMGL gr)	{	_Gr_->ResetFrames();	}
//-----------------------------------------------------------------------------
void mgl_set_transp_type(HMGL gr, int type)			{	_Gr_->SetTranspType(type);	}
void mgl_set_alpha(HMGL gr, int enable)				{	_Gr_->Alpha(enable);	}
void mgl_set_fog(HMGL gr, float d, float dz)		{	_Gr_->Fog(d,dz);	}
void mgl_set_ambbr(HMGL gr, float i)				{	_Gr_->Ambient(i);	}
void mgl_set_light(HMGL gr, int enable)				{	_Gr_->Light(enable);	}
void mgl_set_light_n(HMGL gr, int n, int enable)	{	_Gr_->Light(n, enable);	}
void mgl_add_light_ext(HMGL gr, int n, float x, float y, float z, char c, float br, bool inf, float ap)
{	_Gr_->AddLight(n,mglPoint(x,y,z),c,br,inf,ap);	}
void mgl_add_light(HMGL gr, int n, float x, float y, float z, char c)
{	_Gr_->AddLight(n,mglPoint(x,y,z),c,0.5);	}
//-----------------------------------------------------------------------------
void mgl_mat_push(HMGL gr)	{	_Gr_->Push();	}
void mgl_mat_pop(HMGL gr)	{	_Gr_->Pop();	}
void mgl_identity(HMGL gr, int rel)	{	_Gr_->Identity(rel);	}
void mgl_clf(HMGL gr)	{	_Gr_->Clf();	}
void mgl_clf_rgb(HMGL gr, float r, float g, float b){	_Gr_->Clf(mglColor(r,g,b));	}
//-----------------------------------------------------------------------------
void mgl_subplot(HMGL gr, int nx,int ny,int m)		{	_Gr_->SubPlot(nx,ny,m);	}
void mgl_subplot_d(HMGL gr, int nx,int ny,int m,float dx,float dy)
{	_Gr_->SubPlot(nx,ny,m,dx,dy);	}
void mgl_subplot_s(HMGL gr, int nx,int ny,int m,const char *style)
{	_Gr_->SubPlot(nx,ny,m,style);	}
void mgl_inplot(HMGL gr, float x1,float x2,float y1,float y2)
{	_Gr_->InPlot(x1,x2,y1,y2,false);	}
void mgl_relplot(HMGL gr, float x1,float x2,float y1,float y2)
{	_Gr_->InPlot(x1,x2,y1,y2,true);	}
void mgl_columnplot(HMGL gr, int num, int i)
{	_Gr_->ColumnPlot(num,i);	}
void mgl_columnplot_d(HMGL gr, int num, int i, float d)
{	_Gr_->ColumnPlot(num,i,d);	}
void mgl_stickplot(HMGL gr, int num, int i, float tet, float phi)
{	_Gr_->StickPlot(num, i, tet, phi);	}
void mgl_aspect(HMGL gr, float Ax,float Ay,float Az)
{	_Gr_->Aspect(Ax,Ay,Az);	}
void mgl_rotate(HMGL gr, float TetX,float TetZ,float TetY)
{	_Gr_->Rotate(TetX,TetZ,TetY);	}
void mgl_rotate_vector(HMGL gr, float Tet,float x,float y,float z)
{	_Gr_->RotateN(Tet,x,y,z);	}
void mgl_perspective(HMGL gr, float val)
{	_Gr_->Perspective(val);	}
//-----------------------------------------------------------------------------
int mgl_new_frame_(uintptr_t *gr)		{	return _GR_->NewFrame();	}
void mgl_end_frame_(uintptr_t *gr)		{	_GR_->EndFrame();	}
int mgl_get_num_frame_(uintptr_t *gr)	{	return _GR_->GetNumFrame();	}
void mgl_reset_frames_(uintptr_t *gr)	{	_GR_->ResetFrames();	}
//-----------------------------------------------------------------------------
void mgl_set_transp_type_(uintptr_t *gr, int *type)		{	_GR_->SetTranspType(*type);	}
void mgl_set_alpha_(uintptr_t *gr, int *enable)			{	_GR_->Alpha(*enable);	}
void mgl_set_fog_(uintptr_t *gr, float *d, float *dz)	{	_GR_->Fog(*d, *dz);	}
void mgl_set_light_(uintptr_t *gr, int *enable)			{	_GR_->Light(*enable);	}
void mgl_set_light_n_(uintptr_t *gr, int *n, int *enable)
{	_GR_->Light(*n, *enable);	}
void mgl_add_light_(uintptr_t *gr, int *n, float *x, float *y, float *z, char *c, int)
{	_GR_->AddLight(*n,mglPoint(*x,*y,*z),*c);	}
void mgl_add_light_rgb_(uintptr_t *gr, int *n, float *x, float *y, float *z, char *c, float *br, int *inf, float *ap, int)
{	_GR_->AddLight(*n,mglPoint(*x,*y,*z),*c,*br,*inf,*ap);	}
void mgl_set_ambbr_(uintptr_t *gr, float *i)
{	_GR_->Ambient(*i);	}
//-----------------------------------------------------------------------------
void mgl_mat_push_(uintptr_t *gr)	{	_GR_->Push();	}
void mgl_mat_pop_(uintptr_t *gr)	{	_GR_->Pop();	}
void mgl_identity_(uintptr_t *gr, int *rel)	{	_GR_->Identity(*rel);	}
void mgl_clf_(uintptr_t *gr)
{	_GR_->Clf();	}
void mgl_clf_rgb_(uintptr_t *gr, float *r, float *g, float *b)
{	_GR_->Clf(mglColor(*r,*g,*b));	}
//-----------------------------------------------------------------------------
void mgl_subplot_(uintptr_t *gr, int *nx,int *ny,int *m)
{	_GR_->SubPlot(*nx,*ny,*m);	}
void mgl_subplot_d_(uintptr_t *gr, int *nx,int *ny,int *m,float *dx,float *dy)
{	_GR_->SubPlot(*nx,*ny,*m,*dx,*dy);	}
void mgl_subplot_s_(uintptr_t *gr, int *nx,int *ny,int *m,const char *st,int l)
{	char *s=new char[l+1];	memcpy(s,st,l);	s[l]=0;
	_GR_->SubPlot(*nx,*ny,*m,s);	delete []s;	}
void mgl_inplot_(uintptr_t *gr, float *x1,float *x2,float *y1,float *y2)
{	_GR_->InPlot(*x1,*x2,*y1,*y2,false);	}
void mgl_relplot_(uintptr_t *gr, float *x1,float *x2,float *y1,float *y2)
{	_GR_->InPlot(*x1,*x2,*y1,*y2,true);	}
void mgl_columnplot_(uintptr_t *gr, int *num, int *i)
{	_GR_->ColumnPlot(*num,*i);	}
void mgl_columnplot_d_(uintptr_t *gr, int *num, int *i, float *d)
{	_GR_->ColumnPlot(*num,*i,*d);	}
void mgl_stickplot_(uintptr_t *gr, int *num, int *i, float *tet, float *phi)
{	_GR_->StickPlot(*num, *i, *tet, *phi);	}
void mgl_aspect_(uintptr_t *gr, float *Ax,float *Ay,float *Az)
{	_GR_->Aspect(*Ax,*Ay,*Az);	}
void mgl_rotate_(uintptr_t *gr, float *TetX,float *TetZ,float *TetY)
{	_GR_->Rotate(*TetX,*TetZ,*TetY);	}
void mgl_rotate_vector_(uintptr_t *gr, float *Tet,float *x,float *y,float *z)
{	_GR_->RotateN(*Tet,*x,*y,*z);	}
void mgl_perspective_(uintptr_t *gr, float val)
{	_GR_->Perspective(val);	}
//-----------------------------------------------------------------------------
const unsigned char *mgl_get_rgb_(uintptr_t *gr)	{	return gr ? _GR_->GetBits():0;	}
const unsigned char *mgl_get_rgba_(uintptr_t *gr){	return gr ? _GR_->GetRGBA():0;	}
int mgl_get_width_(uintptr_t *gr)	{	return gr ? _GR_->GetWidth():0;	}
int mgl_get_height_(uintptr_t *gr)	{	return gr ? _GR_->GetHeight():0;}
void mgl_calc_xyz_(uintptr_t *gr, int *xs, int *ys, mreal *x, mreal *y, mreal *z)
{	mglPoint p = _GR_->CalcXYZ(*xs,*ys);	*x = p.x;	*y = p.y;	*z = p.z;	}
void mgl_calc_scr_(uintptr_t *gr, mreal *x, mreal *y, mreal *z, int *xs, int *ys)
{	_GR_->CalcScr(mglPoint(*x,*y,*z),xs,ys);	}
//-----------------------------------------------------------------------------
