/***************************************************************************
 * base.cpp is part of Math Graphic Library
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
#include "mgl/base_cf.h"
//-----------------------------------------------------------------------------
//
//		C interfaces
//
//-----------------------------------------------------------------------------
void mgl_set_auto(HMGL gr, float x1, float x2, float y1, float y2, float z1, float z2)
{	gr->SetAutoRanges(x1,x2,y1,y2,z1,z2);	}
int mgl_get_warn(HMGL gr)	{	return gr->GetWarn();	}
void mgl_set_origin(HMGL gr, float x0, float y0, float z0)
{	gr->SetOrigin(x0,y0,z0);	}
void mgl_set_palette(HMGL gr, const char *colors)
{	gr->SetPalette(colors);	}
void mgl_set_meshnum(HMGL gr, int num)	{	gr->SetMeshNum(num);	}
void mgl_set_alpha_default(HMGL gr, float alpha)	{	gr->SetAlphaDef(alpha);	}
//-----------------------------------------------------------------------------
void mgl_set_cut(HMGL gr, int cut)		{	gr->Cut = cut;	}
void mgl_set_cut_box(HMGL gr, float x1,float y1,float z1,float x2,float y2,float z2)
{	gr->SetCutBox(x1,y1,z1,x2,y2,z2);	}
void mgl_set_cutoff(HMGL gr, const char *EqC)	{	gr->CutOff(EqC);	}
//-----------------------------------------------------------------------------
void mgl_set_ternary(HMGL gr, int enable)			{	gr->Ternary(enable);	}
void mgl_set_crange_val(HMGL gr, float v1,float v2)	{	gr->CRange(v1,v2);	}
void mgl_set_xrange_val(HMGL gr, float v1,float v2)	{	gr->XRange(v1,v2);	}
void mgl_set_yrange_val(HMGL gr, float v1,float v2)	{	gr->YRange(v1,v2);	}
void mgl_set_zrange_val(HMGL gr, float v1,float v2)	{	gr->ZRange(v1,v2);	}
void mgl_set_crange(HMGL gr, HCDT a, int add)	{	gr->CRange(_Da_(a),add);	}
void mgl_set_xrange(HMGL gr, HCDT a, int add)	{	gr->XRange(_Da_(a),add);	}
void mgl_set_yrange(HMGL gr, HCDT a, int add)	{	gr->YRange(_Da_(a),add);	}
void mgl_set_zrange(HMGL gr, HCDT a, int add)	{	gr->ZRange(_Da_(a),add);	}
void mgl_set_ranges(HMGL gr, float x1, float y1, float z1, float x2, float y2, float z2)
{	gr->SetRanges(x1,y1,z1,x2,y2,z2);	}
void mgl_set_func(HMGL gr, const char *EqX,const char *EqY,const char *EqZ)
{	gr->SetFunc(EqX,EqY,EqZ);	}
void mgl_set_func_ext(HMGL gr, const char *EqX,const char *EqY,const char *EqZ,const char *EqA)
{	gr->SetFunc(EqX,EqY,EqZ,EqA);	}
void mgl_set_coor(HMGL gr, int how)				{	gr->SetCoor(how);	}
//-----------------------------------------------------------------------------
float mgl_data_get_value(HCDT d, int i, int j, int k)
{	return _Da_(d).v(i,j,k);	}
long mgl_data_get_nx(HCDT d)	{	return	_Da_(d).GetNx();	}
long mgl_data_get_ny(HCDT d)	{	return	_Da_(d).GetNy();	}
long mgl_data_get_nz(HCDT d)	{	return	_Da_(d).GetNz();	}
//-----------------------------------------------------------------------------
void mgl_set_bar_width(HMGL gr, float width)	{	gr->SetBarWidth(width);	}
//-----------------------------------------------------------------------------
//
//		Fortran interfaces
//
//-----------------------------------------------------------------------------
void mgl_set_auto_(uintptr_t *gr, float *x1, float *x2, float *y1, float *y2, float *z1, float *z2)
{	_GR_->SetAutoRanges(*x1,*x2,*y1,*y2,*z1,*z2);	}
void mgl_set_origin_(uintptr_t *gr, float *x0, float *y0, float *z0)
{	_GR_->SetOrigin(*x0,*y0,*z0);	}
int mgl_get_warn_(uintptr_t *gr)	{	return _GR_->GetWarn();	}
void mgl_set_palette_(uintptr_t *gr, const char *colors, int l)
{	char *s=new char[l+1];	memcpy(s,colors,l);	s[l]=0;
	_GR_->SetPalette(s);	delete []s;	}
void mgl_set_meshnum_(uintptr_t *gr, int *num)	{	_GR_->SetMeshNum(*num);	}
void mgl_set_alpha_default_(uintptr_t *gr, float *alpha)	{	_GR_->SetAlphaDef(*alpha);	}
//-----------------------------------------------------------------------------
void mgl_set_cut_box_(uintptr_t *gr, float *x1,float *y1,float *z1,float *x2,float *y2,float *z2)
{	_GR_->SetCutBox(*x1,*y1,*z1,*x2,*y2,*z2);	}
void mgl_set_cut_(uintptr_t *gr, int *cut)					{	_GR_->Cut = *cut;	}
void mgl_set_cutoff_(uintptr_t *gr, const char *EqC, int l)
{	char *s=new char[l+1];	memcpy(s,EqC,l);	s[l]=0;
	_GR_->CutOff(s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_set_ternary_(uintptr_t *gr, int *enable)			{	_GR_->Ternary(*enable);	}
void mgl_set_crange_val_(uintptr_t *gr, float *v1,float *v2){	_GR_->CRange(*v1,*v2);	}
void mgl_set_xrange_val_(uintptr_t *gr, float *v1,float *v2){	_GR_->XRange(*v1,*v2);	}
void mgl_set_yrange_val_(uintptr_t *gr, float *v1,float *v2){	_GR_->YRange(*v1,*v2);	}
void mgl_set_zrange_val_(uintptr_t *gr, float *v1,float *v2){	_GR_->ZRange(*v1,*v2);	}
void mgl_set_crange_(uintptr_t *gr, uintptr_t *a, int *add)	{	_GR_->CRange(*_DA_(a),*add);	}
void mgl_set_xrange_(uintptr_t *gr, uintptr_t *a, int *add)	{	_GR_->XRange(*_DA_(a),*add);	}
void mgl_set_yrange_(uintptr_t *gr, uintptr_t *a, int *add)	{	_GR_->YRange(*_DA_(a),*add);	}
void mgl_set_zrange_(uintptr_t *gr, uintptr_t *a, int *add)	{	_GR_->ZRange(*_DA_(a),*add);	}
void mgl_set_ranges_(uintptr_t *gr, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2)
{	_GR_->SetRanges(*x1,*y1,*z1,*x2,*y2,*z2);	}
void mgl_set_func_(uintptr_t *gr, const char *EqX,const char *EqY,const char *EqZ,int lx,int ly,int lz)
{
	char *sx=new char[lx+1];	memcpy(sx,EqX,lx);	sx[lx]=0;
	char *sy=new char[ly+1];	memcpy(sy,EqY,ly);	sy[ly]=0;
	char *sz=new char[lz+1];	memcpy(sz,EqZ,lz);	sz[lz]=0;
	_GR_->SetFunc(sx,sy,sz);
	delete []sx;	delete []sy;	delete []sz;
}
void mgl_set_func_ext_(uintptr_t *gr, const char *EqX,const char *EqY,const char *EqZ,const char *EqA,int lx,int ly,int lz,int la)
{
	char *sx=new char[lx+1];	memcpy(sx,EqX,lx);	sx[lx]=0;
	char *sy=new char[ly+1];	memcpy(sy,EqY,ly);	sy[ly]=0;
	char *sz=new char[lz+1];	memcpy(sz,EqZ,lz);	sz[lz]=0;
	char *sa=new char[la+1];	memcpy(sa,EqA,la);	sa[la]=0;
	_GR_->SetFunc(sx,sy,sz,sa);
	delete []sx;	delete []sy;	delete []sz;	delete []sa;
}
void mgl_set_coor_(uintptr_t *gr, int *how)
{	_GR_->SetCoor(*how);	}
//-----------------------------------------------------------------------------
float mgl_data_get_value_(uintptr_t *d, int *i, int *j, int *k)
{	return _DA_(d)->v(*i,*j,*k);	}
long mgl_data_get_nx_(uintptr_t *d)	{	return	_DA_(d)->GetNx();	}
long mgl_data_get_ny_(uintptr_t *d)	{	return	_DA_(d)->GetNy();	}
long mgl_data_get_nz_(uintptr_t *d)	{	return	_DA_(d)->GetNz();	}
//-----------------------------------------------------------------------------
void mgl_set_rotated_text(HMGL gr, int rotated)	{	gr->SetRotatedText(rotated);	}
void mgl_set_mark_size(HMGL gr, mreal size)		{	gr->SetMarkSize(size);	}
void mgl_set_arrow_size(HMGL gr, mreal size)	{	gr->SetArrowSize(size);	}
void mgl_set_font_size(HMGL gr, mreal size)		{	gr->SetFontSize(size);	}
void mgl_load_font(HMGL gr, const char *name, const char *path)
{	gr->GetFont()->Load(name,path);	}
void mgl_copy_font(HMGL gr, HMGL gr_from)		{	gr->GetFont()->Copy(gr_from->GetFont());	}
void mgl_restore_font(HMGL gr)	{	gr->GetFont()->Restore();	}
//-----------------------------------------------------------------------------
void mgl_set_bar_width_(uintptr_t *gr, float *width)	{	_GR_->SetBarWidth(*width);	}
void mgl_set_rotated_text_(uintptr_t *gr, int *rotated)	{	_GR_->SetRotatedText(*rotated);	}
void mgl_set_mark_size_(uintptr_t *gr, mreal *size)		{	_GR_->SetMarkSize(*size);	}
void mgl_set_arrow_size_(uintptr_t *gr, mreal *size)	{	_GR_->SetArrowSize(*size);	}
void mgl_set_font_size_(uintptr_t *gr, mreal *size)		{	_GR_->SetFontSize(*size);	}
void mgl_load_font_(uintptr_t *gr, char *name, char *path, int l,int n)
{	char *s=new char[l+1];		memcpy(s,name,l);	s[l]=0;
	char *d=new char[n+1];		memcpy(d,path,n);	d[n]=0;
	_GR_->GetFont()->Load(s,d);	delete []s;		delete []d;	}
void mgl_copy_font_(uintptr_t *gr, uintptr_t *gr_from)
{	_GR_->GetFont()->Copy(((mglBase *)(*gr_from))->GetFont());	}
void mgl_restore_font_(uintptr_t *gr)	{	_GR_->GetFont()->Restore();	}
//-----------------------------------------------------------------------------
