/***************************************************************************
 * base.cpp is part of Math gric Library
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
int mgl_get_warn(HMGL gr)	{	return gr->GetWarn();	}
void mgl_set_warn(HMGL gr, int code)	{	return gr->SetWarn(code);	}
void mgl_buf_warn(HMGL gr, char *buf)	{	gr->Message = buf;	}
void mgl_set_origin(HMGL gr, float x0, float y0, float z0)
{	gr->SetOrigin(x0,y0,z0);	}
void mgl_set_palette(HMGL gr, const char *colors)
{	gr->SetPalette(colors);	}
void mgl_set_meshnum(HMGL gr, int num)	{	gr->SetMeshNum(num);	}
void mgl_set_alpha_default(HMGL gr, float alpha)	{	gr->SetAlphaDef(alpha);	}
//-----------------------------------------------------------------------------
void mgl_set_cut(HMGL gr, int cut)		{	gr->SetCut(cut);	}
void mgl_set_cut_box(HMGL gr, float x1,float y1,float z1,float x2,float y2,float z2)
{	gr->SetCutBox(x1,y1,z1,x2,y2,z2);	}
void mgl_set_cutoff(HMGL gr, const char *EqC)	{	gr->CutOff(EqC);	}
//-----------------------------------------------------------------------------
void mgl_set_ternary(HMGL gr, int enable)			{	gr->Ternary(enable);	}
void mgl_set_range_val(HMGL gr, char dir, float v1,float v2)
{
	if(dir=='c')		gr->CRange(v1,v2);
	else if(dir=='x')	gr->XRange(v1,v2);
	else if(dir=='y')	gr->YRange(v1,v2);
	else if(dir=='z')	gr->ZRange(v1,v2);	}
void mgl_set_range_dat(HMGL gr, char dir, HCDT a, int add)
{
	if(dir=='c')		gr->CRange(_Da_(a),add);
	else if(dir=='x')	gr->XRange(_Da_(a),add);
	else if(dir=='y')	gr->YRange(_Da_(a),add);
	else if(dir=='z')	gr->ZRange(_Da_(a),add);	}
void mgl_set_ranges(HMGL gr, float x1, float y1, float z1, float x2, float y2, float z2)
{	gr->SetRanges(x1,y1,z1,x2,y2,z2);	}
void mgl_set_func(HMGL gr, const char *EqX,const char *EqY,const char *EqZ,const char *EqA)
{	gr->SetFunc(EqX,EqY,EqZ,EqA);	}
void mgl_set_coor(HMGL gr, int how)	{	gr->SetCoor(how);	}
//-----------------------------------------------------------------------------
float mgl_data_get_value(HCDT d, long i, long j, long k)	{	return _Da_(d).v(i,j,k);	}
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
void mgl_set_origin_(uintptr_t *gr, float *x0, float *y0, float *z0)
{	_GR_->SetOrigin(*x0,*y0,*z0);	}
int mgl_get_warn_(uintptr_t *gr)	{	return _GR_->GetWarn();	}
void mgl_set_warn_(uintptr_t *gr, int *code)	{	return _GR_->SetWarn(*code);	}
void mgl_set_palette_(uintptr_t *gr, const char *colors, int l)
{	char *s=new char[l+1];	memcpy(s,colors,l);	s[l]=0;
	_GR_->SetPalette(s);	delete []s;	}
void mgl_set_meshnum_(uintptr_t *gr, int *num)	{	_GR_->SetMeshNum(*num);	}
void mgl_set_alpha_default_(uintptr_t *gr, float *alpha)	{	_GR_->SetAlphaDef(*alpha);	}
//-----------------------------------------------------------------------------
void mgl_set_cut_box_(uintptr_t *gr, float *x1,float *y1,float *z1,float *x2,float *y2,float *z2)
{	_GR_->SetCutBox(*x1,*y1,*z1,*x2,*y2,*z2);	}
void mgl_set_cut_(uintptr_t *gr, int *cut)	{	_GR_->SetCut(*cut);	}
void mgl_set_cutoff_(uintptr_t *gr, const char *EqC, int l)
{	char *s=new char[l+1];	memcpy(s,EqC,l);	s[l]=0;
	_GR_->CutOff(s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_set_ternary_(uintptr_t *gr, int *enable)	{	_GR_->Ternary(*enable);	}
void mgl_set_range_val_(uintptr_t *gr, const char *dir, float *v1,float *v2,int)
{
	if(*dir=='c')		_GR_->CRange(*v1,*v2);
	else if(*dir=='x')	_GR_->XRange(*v1,*v2);
	else if(*dir=='y')	_GR_->YRange(*v1,*v2);
	else if(*dir=='z')	_GR_->ZRange(*v1,*v2);	}
void mgl_set_range_dat_(uintptr_t *gr, const char *dir, uintptr_t *a, int *add,int)
{
	if(*dir=='c')		_GR_->CRange(*_DA_(a),*add);
	else if(*dir=='x')	_GR_->XRange(*_DA_(a),*add);
	else if(*dir=='y')	_GR_->YRange(*_DA_(a),*add);
	else if(*dir=='z')	_GR_->ZRange(*_DA_(a),*add);	}
void mgl_set_ranges_(uintptr_t *gr, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2)
{	_GR_->SetRanges(*x1,*y1,*z1,*x2,*y2,*z2);	}
void mgl_set_func_(uintptr_t *gr, const char *EqX,const char *EqY,const char *EqZ,const char *EqA,int lx,int ly,int lz,int la)
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
void mgl_set_tick_rotate(HMGL gr, int enable){	gr->SetTickRotate(enable);	}
void mgl_set_tick_skip(HMGL gr, int enable)	{	gr->SetTickSkip(enable);	}
void mgl_set_tick_rotate_(uintptr_t *gr,int *enable){	_GR_->SetTickRotate(*enable);	}
void mgl_set_tick_skip_(uintptr_t *gr, int *enable)	{	_GR_->SetTickSkip(*enable);	}
//-----------------------------------------------------------------------------
void mgl_set_rotated_text(HMGL gr, int enable)	{	gr->SetRotatedText(enable);	}
void mgl_set_mark_size(HMGL gr, mreal size)		{	gr->SetMarkSize(size);	}
void mgl_set_arrow_size(HMGL gr, mreal size)	{	gr->SetArrowSize(size);	}
void mgl_set_font_size(HMGL gr, mreal size)		{	gr->SetFontSize(size);	}
void mgl_set_font_def(HMGL gr, const char *fnt)	{	gr->SetFontDef(fnt);	}
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
void mgl_set_font_def_(uintptr_t *gr, char *name, int l)
{	char *s=new char[l+1];		memcpy(s,name,l);	s[l]=0;
	_GR_->SetFontDef(s);	delete []s;	}
void mgl_load_font_(uintptr_t *gr, char *name, char *path, int l,int n)
{	char *s=new char[l+1];		memcpy(s,name,l);	s[l]=0;
	char *d=new char[n+1];		memcpy(d,path,n);	d[n]=0;
	_GR_->GetFont()->Load(s,d);	delete []s;		delete []d;	}
void mgl_copy_font_(uintptr_t *gr, uintptr_t *gr_from)
{	_GR_->GetFont()->Copy(((mglBase *)(*gr_from))->GetFont());	}
void mgl_restore_font_(uintptr_t *gr)	{	_GR_->GetFont()->Restore();	}
//-----------------------------------------------------------------------------
void mgl_start_group(HMGL gr, const char *s)	{	gr->StartAutoGroup(s);	}
void mgl_end_group(HMGL gr)	{	gr->EndGroup();	}
void mgl_start_group_(uintptr_t *gr, const char *name,int l)
{	char *s=new char[l+1];		memcpy(s,name,l);	s[l]=0;
	_GR_->StartAutoGroup(s);	delete []s;	}
void mgl_end_group_(uintptr_t *gr)	{	_GR_->EndGroup();	}
//-----------------------------------------------------------------------------
#include <stdarg.h>
bool mglTestMode=false;
void mgl_test_txt(const char *str, ...)
{
	if(mglTestMode)
	{
		char buf[256];
		va_list lst;
		va_start(lst,str);
		vsprintf(buf,str,lst);
		va_end(lst);
		printf("TEST: %s\n",buf);
		fflush(stdout);
	}
}
void mgl_set_test_mode(int enable)
{	mglTestMode=enable;	}
//---------------------------------------------------------------------------
long mgl_use_graph(HMGL gr, int inc)
{	gr->InUse+=inc;	return gr->InUse;	}
long mgl_use_graph_(uintptr_t *gr, int *inc)
{	_GR_->InUse+=*inc;	return _GR_->InUse;	}
//---------------------------------------------------------------------------
void mgl_set_ambbr(HMGL gr, float i)		{	gr->SetAmbient(i);	}
void mgl_set_ambbr_(uintptr_t *gr, float *i){	_GR_->SetAmbient(*i);	}
//---------------------------------------------------------------------------
