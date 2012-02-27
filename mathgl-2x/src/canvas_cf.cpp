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
#include "mgl/canvas_cf.h"
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
void mgl_set_obj_id(HMGL gr, int id)	{	_Gr_->SetObjId(id);	}
int mgl_get_obj_id(HMGL gr, long x, long y)	{	return _Gr_->GetObjId(x,y);	}
int mgl_get_spl_id(HMGL gr, long x, long y)	{	return _Gr_->GetSplId(x,y);	}
//-----------------------------------------------------------------------------
int mgl_new_frame(HMGL gr)		{	return _Gr_->NewFrame();	}
void mgl_end_frame(HMGL gr)		{	_Gr_->EndFrame();	}
int mgl_get_num_frame(HMGL gr)	{	return _Gr_->GetNumFrame();	}
void mgl_reset_frames(HMGL gr)	{	_Gr_->ResetFrames();	}
//-----------------------------------------------------------------------------
void mgl_set_transp_type(HMGL gr, int type)			{	_Gr_->SetTranspType(type);	}
void mgl_set_alpha(HMGL gr, int enable)				{	_Gr_->Alpha(enable);	}
void mgl_set_fog(HMGL gr, float d, float dz)		{	_Gr_->Fog(d,dz);		}
void mgl_set_light(HMGL gr, int enable)				{	_Gr_->Light(enable);	}
void mgl_set_light_n(HMGL gr, int n, int enable)	{	_Gr_->Light(n, enable);	}
void mgl_add_light_ext(HMGL gr, int n, float x, float y, float z, char c, float br, float ap)
{	_Gr_->AddLight(n,mglPoint(x,y,z),c,br,ap);	}
void mgl_add_light_loc(HMGL gr, int n, float x, float y, float z, float dx, float dy, float dz, char c, float br, float ap)
{	_Gr_->AddLight(n,mglPoint(x,y,z),mglPoint(dx,dy,dz),c,br,ap);	}
void mgl_add_light(HMGL gr, int n, float x, float y, float z)
{	_Gr_->AddLight(n,mglPoint(x,y,z));	}
//-----------------------------------------------------------------------------
void mgl_mat_push(HMGL gr)	{	_Gr_->Push();	}
void mgl_mat_pop(HMGL gr)	{	_Gr_->Pop();	}
void mgl_clf(HMGL gr)	{	_Gr_->Clf();	}
void mgl_clf_rgb(HMGL gr, float r, float g, float b){	_Gr_->Clf(mglColor(r,g,b));	}
//-----------------------------------------------------------------------------
void mgl_subplot(HMGL gr, int nx,int ny,int m)
{	mgl_subplot_d(gr,nx,ny,m,0,0);	}
//-----------------------------------------------------------------------------
void mgl_subplot_d(HMGL gr, int nx,int ny,int m,float dx,float dy)
{
	float x1,x2,y1,y2;
	int mx = m%nx, my = m/nx;
	if(_Gr_->get(MGL_AUTO_FACTOR))	{	dx /= 1.55;	dy /= 1.55;	}
	else	{	dx /= 2;	dy /= 2;	}
	x1 = (mx+dx)/nx;		x2 = (mx+1+dx)/nx;
	y2 = 1.f-(my+dy)/ny;	y1 = 1.f-(my+1+dy)/ny;
	_Gr_->InPlot(x1,x2,y1,y2,false);
}
//-----------------------------------------------------------------------------
void mgl_subplot_s(HMGL gr, int nx,int ny,int m,const char *style)
{
	float x1,x2,y1,y2;
	int mx = m%nx, my = m/nx;
	x1 = float(mx)/nx;		x2 = float(mx+1)/nx;
	y2 = 1.f-float(my)/ny;	y1 = 1.f-float(my+1)/ny;
	_Gr_->InPlot(x1,x2,y1,y2,style);
}
//-----------------------------------------------------------------------------
void mgl_multiplot(HMGL gr, int nx,int ny,int m,int dx,int dy,const char *style)
{
	float x1,x2,y1,y2;
	int mx = m%nx, my = m/nx;
	dx = (dx<1 || dx+mx>nx) ? 1 : dx;
	dy = (dy<1 || dy+my>ny) ? 1 : dy;
	x1 = float(mx)/nx;		x2 = float(mx+dx)/nx;
	y2 = 1-float(my)/ny;	y1 = 1-float(my+dy)/ny;
	_Gr_->InPlot(x1,x2,y1,y2,style);
}
//-----------------------------------------------------------------------------
void mgl_inplot(HMGL gr, float x1,float x2,float y1,float y2)
{	_Gr_->InPlot(x1,x2,y1,y2,false);	}
void mgl_relplot(HMGL gr, float x1,float x2,float y1,float y2)
{	_Gr_->InPlot(x1,x2,y1,y2,true);	}
//-----------------------------------------------------------------------------
void mgl_columnplot(HMGL gr, int num, int i, float dd)
{
	float pf=_Gr_->GetPlotFactor();
	float d = i/(num+pf-1), w = pf/(num+pf-1);
	_Gr_->InPlot(0,1,d,d+w*(1-dd),true);
}
//-----------------------------------------------------------------------------
void mgl_gridplot(HMGL gr, int nx, int ny, int i, float dd)
{
	int ix=i%nx, iy=i/nx;
	float pf=_Gr_->GetPlotFactor();
	float dx = ix/(nx+pf-1), wx = pf/(nx+pf-1);
	float dy = iy/(ny+pf-1), wy = pf/(ny+pf-1);
	_Gr_->InPlot(dx,dx+wx*(1-dd),dy,dy+wy*(1-dd),true);
}
//-----------------------------------------------------------------------------
void mgl_stickplot(HMGL gr, int num, int i, float tet, float phi)
{	_Gr_->StickPlot(num, i, tet, phi);	}
//-----------------------------------------------------------------------------
void mgl_aspect(HMGL gr, float Ax,float Ay,float Az)
{	_Gr_->Aspect(Ax,Ay,Az);	}
void mgl_rotate(HMGL gr, float TetX,float TetZ,float TetY)
{	_Gr_->Rotate(TetX,TetZ,TetY);	}
void mgl_view(HMGL gr, float TetX,float TetZ,float TetY)
{	_Gr_->View(TetX,TetZ,TetY);	}
void mgl_zoom(HMGL gr, float x1, float y1, float x2, float y2)
{	_Gr_->Zoom(x1,y1,x2,y2);	}
void mgl_rotate_vector(HMGL gr, float Tet,float x,float y,float z)
{	_Gr_->RotateN(Tet,x,y,z);	}
void mgl_perspective(HMGL gr, float val)
{	_Gr_->Perspective(val);	}
void mgl_title(HMGL gr, const char *title, const char *stl, float size)
{	_Gr_->Title(title,stl,size);	}
void mgl_titlew(HMGL gr, const wchar_t *title, const char *stl, float size)
{	_Gr_->Title(title,stl,size);	}
//-----------------------------------------------------------------------------
int mgl_new_frame_(uintptr_t *gr)		{	return _GR_->NewFrame();	}
void mgl_end_frame_(uintptr_t *gr)		{	_GR_->EndFrame();	}
int mgl_get_num_frame_(uintptr_t *gr)	{	return _GR_->GetNumFrame();	}
void mgl_reset_frames_(uintptr_t *gr)	{	_GR_->ResetFrames();	}
//-----------------------------------------------------------------------------
void mgl_set_transp_type_(uintptr_t *gr, int *type)		{	_GR_->SetTranspType(*type);	}
void mgl_set_alpha_(uintptr_t *gr, int *enable)			{	_GR_->Alpha(*enable);	}
void mgl_set_fog_(uintptr_t *gr, float *d, float *dz)	{	_GR_->Fog(*d, *dz);		}
void mgl_set_light_(uintptr_t *gr, int *enable)			{	_GR_->Light(*enable);	}
void mgl_set_light_n_(uintptr_t *gr, int *n, int *enable)
{	_GR_->Light(*n, *enable);	}
void mgl_add_light_(uintptr_t *gr, int *n, float *x, float *y, float *z)
{	_GR_->AddLight(*n,mglPoint(*x,*y,*z));	}
void mgl_add_light_ext_(uintptr_t *gr, int *n, float *x, float *y, float *z, char *c, float *br, float *ap, int)
{	_GR_->AddLight(*n,mglPoint(*x,*y,*z),*c,*br,*ap);	}
void mgl_add_light_loc_(uintptr_t *gr, int *n, float *x, float *y, float *z, float *dx, float *dy, float *dz, char *c, float *br, float *ap, int)
{	_GR_->AddLight(*n,mglPoint(*x,*y,*z),mglPoint(*dx,*dy,*dz),*c,*br,*ap);	}
//-----------------------------------------------------------------------------
void mgl_mat_push_(uintptr_t *gr)	{	_GR_->Push();	}
void mgl_mat_pop_(uintptr_t *gr)	{	_GR_->Pop();	}
void mgl_clf_(uintptr_t *gr)
{	_GR_->Clf();	}
void mgl_clf_rgb_(uintptr_t *gr, float *r, float *g, float *b)
{	_GR_->Clf(mglColor(*r,*g,*b));	}
//-----------------------------------------------------------------------------
void mgl_subplot_(uintptr_t *gr, int *nx,int *ny,int *m)
{	mgl_subplot_d(_GR_,*nx,*ny,*m,0,0);	}
void mgl_subplot_d_(uintptr_t *gr, int *nx,int *ny,int *m,float *dx,float *dy)
{	mgl_subplot_d(_GR_,*nx,*ny,*m,*dx,*dy);	}
void mgl_subplot_s_(uintptr_t *gr, int *nx,int *ny,int *m,const char *st,int l)
{	char *s=new char[l+1];	memcpy(s,st,l);	s[l]=0;
	mgl_subplot_s(_GR_,*nx,*ny,*m,s);	delete []s;	}
void mgl_multiplot_(uintptr_t *gr, int *nx,int *ny,int *m,int *dx,int *dy,const char *st,int l)
{	char *s=new char[l+1];	memcpy(s,st,l);	s[l]=0;
	mgl_multiplot(_GR_,*nx,*ny,*m,*dx,*dy,s);	delete []s;	}
void mgl_inplot_(uintptr_t *gr, float *x1,float *x2,float *y1,float *y2)
{	_GR_->InPlot(*x1,*x2,*y1,*y2,false);	}
void mgl_relplot_(uintptr_t *gr, float *x1,float *x2,float *y1,float *y2)
{	_GR_->InPlot(*x1,*x2,*y1,*y2,true);	}
void mgl_columnplot_(uintptr_t *gr, int *num, int *i, float *d)
{	mgl_columnplot(_GR_,*num,*i,*d);	}
void mgl_columnplot_d_(uintptr_t *gr, int *nx, int *ny, int *i, float *d)
{	mgl_gridplot(_GR_,*nx,*ny,*i,*d);	}
void mgl_stickplot_(uintptr_t *gr, int *num, int *i, float *tet, float *phi)
{	_GR_->StickPlot(*num, *i, *tet, *phi);	}

void mgl_title_(uintptr_t *gr, const char *title, const char *stl, float *size, int l,int m)
{	char *t=new char[l+1];	memcpy(t,title,l);	t[l]=0;
	char *s=new char[m+1];	memcpy(s,stl,m);	s[m]=0;
	_GR_->Title(t,s,*size);	delete []s;	delete []t;	}
void mgl_aspect_(uintptr_t *gr, float *Ax,float *Ay,float *Az)
{	_GR_->Aspect(*Ax,*Ay,*Az);	}
void mgl_rotate_(uintptr_t *gr, float *TetX,float *TetZ,float *TetY)
{	_GR_->Rotate(*TetX,*TetZ,*TetY);	}
void mgl_view_(uintptr_t *gr, float *TetX,float *TetZ,float *TetY)
{	_GR_->View(*TetX,*TetZ,*TetY);	}
void mgl_zoom_(uintptr_t *gr, float *x1, float *y1, float *x2, float *y2)
{	_GR_->Zoom(*x1,*y1,*x2,*y2);	}
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
void mgl_set_obj_id_(uintptr_t *gr, int *id)		{	_GR_->SetObjId(*id);	}
int mgl_get_obj_id_(uintptr_t *gr, int *x, int *y)	{	return _GR_->GetObjId(*x,*y);	}
int mgl_get_spl_id_(uintptr_t *gr, int *x, int *y)	{	return _GR_->GetSplId(*x,*y);	}
//-----------------------------------------------------------------------------
HMGL mgl_create_graph(int width, int height)
{	return new mglCanvas(width,height);	}
void mgl_delete_graph(HMGL gr)	{	delete gr;	}
void mgl_set_size(HMGL gr, int width, int height)
{	_Gr_->SetSize(width, height);	}
void mgl_set_def_param(HMGL gr)	{	_Gr_->DefaultPlotParam();	}
void mgl_combine_gr(HMGL gr, HMGL in)
{	const mglCanvas *gg = dynamic_cast<const mglCanvas *>(in);
	if(gg)	_Gr_->Combine(gg);	}
//-----------------------------------------------------------------------------
void mgl_set_tick_len(HMGL gr, float len, float stt)
{	_Gr_->SetTickLen(len,stt);	}
void mgl_set_axis_stl(HMGL gr, const char *stl, const char *tck, const char *sub)
{	_Gr_->SetAxisStl(stl,tck,sub);	}
void mgl_tune_ticks(HMGL gr, int tune, float pos)
{	_Gr_->SetTuneTicks(tune,pos);	}
void mgl_adjust_ticks(HMGL gr, const char *dir)
{	_Gr_->AdjustTicks(dir);	}
void mgl_set_ticks(HMGL gr, char dir, float d, int ns, float org)
{	_Gr_->SetTicks(dir,d,ns,org);	}
void mgl_set_ticks_str(HMGL gr, char dir, const char *lbl, int add)
{	_Gr_->SetTicksVal(dir,lbl,add);	}
void mgl_set_ticks_wcs(HMGL gr, char dir, const wchar_t *lbl, int add)
{	_Gr_->SetTicksVal(dir,lbl,add);	}
void mgl_set_ticks_val(HMGL gr, char dir, HCDT val, const char *lbl, int add)
{	_Gr_->SetTicksVal(dir,val,lbl,add);	}
void mgl_set_ticks_valw(HMGL gr, char dir, HCDT val, const wchar_t *lbl, int add)
{	_Gr_->SetTicksVal(dir,val,lbl,add);	}
void mgl_set_tick_templ(HMGL gr, char dir, const char *templ)
{	_Gr_->SetTickTempl(dir,templ);	}
void mgl_set_tick_templw(HMGL gr, char dir, const wchar_t *templ)
{	_Gr_->SetTickTempl(dir,templ);	}
void mgl_set_tick_time(HMGL gr, char dir, float d, const char *t)
{	_Gr_->SetTickTime(dir,d,t);	}
//-----------------------------------------------------------------------------
void mgl_box(HMGL gr)	{	_Gr_->Box();	}
void mgl_box_str(HMGL gr, const char *col, int ticks)
{	_Gr_->Box(col,ticks);	}
void mgl_axis(HMGL gr, const char *dir, int adj)
{	_Gr_->Axis(dir,adj);	}
void mgl_axis_grid(HMGL gr, const char *dir,const char *pen)
{	_Gr_->Grid(dir,pen);	}
void mgl_label(HMGL gr, char dir, const char *text)
{	_Gr_->Label(dir,text);	}
void mgl_label_ext(HMGL gr, char dir, const char *text, float pos, float shift)
{	_Gr_->Label(dir,text,pos,shift);	}
void mgl_labelw_ext(HMGL gr, char dir, const wchar_t *text, float pos, float shift)
{	_Gr_->Labelw(dir,text,pos,shift);	}
void mgl_label_pos(HMGL gr, float x, float y, const char *text, const char *fnt)
{	_Gr_->Label(x,y,text,fnt);	}
void mgl_labelw_pos(HMGL gr, float x, float y, const wchar_t *text, const char *fnt)
{	_Gr_->Labelw(x,y,text,fnt);	}
//-----------------------------------------------------------------------------
void mgl_colorbar(HMGL gr, const char *sch,int where)
{	_Gr_->Colorbar(sch,where);	}
void mgl_colorbar_ext(HMGL gr, const char *sch, int where, float x, float y, float w, float h)
{	_Gr_->Colorbar(sch,where,x,y,w,h);	}
void mgl_colorbar_val(HMGL gr, HCDT dat, const char *sch,int where)
{	_Gr_->Colorbar(dat,sch,where);	}
void mgl_colorbar_val_ext(HMGL gr, HCDT dat, const char *sch,int where,float x, float y, float w, float h)
{	_Gr_->Colorbar(dat,sch,where,x,y,w,h);	}
//-----------------------------------------------------------------------------
void mgl_add_legend(HMGL gr, const char *text,const char *style)
{	_Gr_->AddLegend(text,style);	}
void mgl_add_legendw(HMGL gr, const wchar_t *text,const char *style)
{	_Gr_->AddLegend(text,style);	}
void mgl_clear_legend(HMGL gr)
{	_Gr_->ClearLegend();	}
void mgl_legend_pos(HMGL gr, float x, float y, const char *font, float size, float llen)
{	_Gr_->Legend(x,y,font,size,llen);	}
void mgl_legend(HMGL gr, int where, const char *font, float size, float llen)
{	_Gr_->Legend(where,font,size,llen);	}
void mgl_set_legend_marks(HMGL gr, int num)
{	_Gr_->SetLegendMarks(num);	}
//-----------------------------------------------------------------------------
uintptr_t mgl_create_graph_(int *width, int *height)
{	return uintptr_t(new mglCanvas(*width,*height));	}
void mgl_delete_graph_(uintptr_t *gr)	{	delete _GR_;	}
void mgl_set_size_(uintptr_t *gr, int *width, int *height)
{	_GR_->SetSize(*width,*height);	}
void mgl_set_def_param_(uintptr_t *gr)	{	_GR_->DefaultPlotParam();	}
void mgl_combine_gr_(uintptr_t *gr, uintptr_t *in)
{	_GR_->Combine((mglCanvas *)in);	}
//-----------------------------------------------------------------------------
void mgl_set_tick_len_(uintptr_t *gr, float *len, float *stt)
{	_GR_->SetTickLen(*len, *stt);	}
void mgl_set_axis_stl_(uintptr_t *gr, const char *stl, const char *tck, const char *sub, int l,int m,int n)
{	char *a=new char[l+1];	memcpy(a,stl,l);	a[l]=0;
	char *t=new char[m+1];	memcpy(t,tck,m);	t[m]=0;
	char *s=new char[n+1];	memcpy(s,sub,n);	s[n]=0;
	_GR_->SetAxisStl(a,t,s);	delete []a;	delete []s;	delete []t;	}
void mgl_adjust_ticks_(uintptr_t *gr, const char *dir, int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	_GR_->AdjustTicks(s);	delete []s;	}
void mgl_set_ticks_(uintptr_t *gr, char *dir, float *d, int *ns, float *org, int)
{	_GR_->SetTicks(*dir, *d, *ns, *org);	}
void mgl_set_ticks_str_(uintptr_t *gr, const char *dir, const char *lbl, int *add,int,int l)
{	char *s=new char[l+1];	memcpy(s,lbl,l);	s[l]=0;
	_GR_->SetTicksVal(*dir,s,*add);	delete []s;	}
void mgl_set_ticks_val_(uintptr_t *gr, const char *dir, uintptr_t *val, const char *lbl, int *add,int,int l)
{	char *s=new char[l+1];	memcpy(s,lbl,l);	s[l]=0;
	_GR_->SetTicksVal(*dir,_DA_(val),s,*add);	delete []s;	}
void mgl_tune_ticks_(uintptr_t *gr, int *tune, float *fact_pos)
{	_GR_->SetTuneTicks(*tune, *fact_pos);	}
void mgl_set_tick_templ_(uintptr_t *gr, const char *dir, const char *templ,int,int l)
{	char *s=new char[l+1];	memcpy(s,templ,l);	s[l]=0;
	_GR_->SetTickTempl(*dir,s);	delete []s;	}
void mgl_set_tick_time_(uintptr_t *gr, const char *dir, float *d, const char *t,int,int l)
{	char *s=new char[l+1];	memcpy(s,t,l);	s[l]=0;
	_GR_->SetTickTime(*dir,*d,s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_box_(uintptr_t *gr)	{	_GR_->Box();	}
void mgl_box_str_(uintptr_t *gr, const char *col, int *ticks, int l)
{	char *s=new char[l+1];	memcpy(s,col,l);	s[l]=0;
	_GR_->Box(s,*ticks);	delete []s;	}
void mgl_axis_(uintptr_t *gr, const char *dir, int *adj,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;	_GR_->Axis(s,*adj);	delete []s;	}
void mgl_axis_grid_(uintptr_t *gr, const char *dir,const char *pen,int l,int n)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	char *p=new char[n+1];	memcpy(p,pen,n);	p[n]=0;
	_GR_->Grid(s,p);	delete []s;	delete []p;	}
void mgl_label_(uintptr_t *gr, const char *dir, const char *text,int,int l)
{	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	_GR_->Label(*dir, s);	delete []s;	}
void mgl_label_ext_(uintptr_t *gr, const char *dir, const char *text, float *pos, float *shift,int,int l)
{	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	_GR_->Label(*dir, s, *pos, *shift);	delete []s;	}
void mgl_label_pos_(uintptr_t *gr, float *x, float *y, const char *txt, const char *fnt,int l,int n)
{	char *s=new char[l+1];	memcpy(s,txt,l);	s[l]=0;
	char *p=new char[n+1];	memcpy(p,fnt,n);	p[n]=0;
	_GR_->Label(*x,*y,s,p);	delete []s;	delete []p;	}
//-----------------------------------------------------------------------------
void mgl_colorbar_(uintptr_t *gr, const char *sch,int *where,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	_GR_->Colorbar(s,*where);	delete []s;	}
void mgl_colorbar_ext_(uintptr_t *gr, const char *sch,int *where, float *x, float *y, float *w, float *h, int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	_GR_->Colorbar(s,*where,*x,*y,*w,*h);	delete []s;	}
void mgl_colorbar_val_(uintptr_t *gr, uintptr_t *dat, const char *sch,int *where,int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	_GR_->Colorbar(_DA_(dat), s,*where);	delete []s;	}
void mgl_colorbar_val_ext_(uintptr_t *gr, uintptr_t *dat, const char *sch,int *where, float *x, float *y, float *w, float *h, int l)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	_GR_->Colorbar(_DA_(dat),s,*where,*x,*y,*w,*h);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_add_legend_(uintptr_t *gr, const char *text,const char *style,int l,int n)
{	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,style,n);	f[n]=0;
	_GR_->AddLegend(s,f);	delete []s;	delete []f;	}
void mgl_clear_legend_(uintptr_t *gr)	{	if(gr)	_GR_->ClearLegend();	}
void mgl_legend_pos_(uintptr_t *gr, float *x, float *y, const char *font, float *size, float *llen,int l)
{	char *s=new char[l+1];	memcpy(s,font,l);	s[l]=0;
	_GR_->Legend(*x, *y, s, *size,*llen);	delete []s;	}
void mgl_legend_(uintptr_t *gr, int *where, const char *font, float *size, float *llen,int l)
{	char *s=new char[l+1];	memcpy(s,font,l);	s[l]=0;
	_GR_->Legend(*where, s, *size,*llen);	delete []s;	}
void mgl_set_legend_marks_(uintptr_t *gr, int *num)
{	_GR_->SetLegendMarks(*num);	}
//-----------------------------------------------------------------------------
void mgl_set_quality(HMGL gr, int qual)			{	_Gr_->SetQuality(qual);	}
void mgl_set_quality_(uintptr_t *gr, int *qual)	{	_GR_->SetQuality(*qual);	}
//-----------------------------------------------------------------------------
void mgl_set_plotid(HMGL gr, const char *id)	{	_Gr_->PlotId = id;	}
void mgl_set_plotid_(uintptr_t *gr, const char *id,int l)
{	char *s=new char[l+1];	memcpy(s,id,l);	s[l]=0;
	_GR_->PlotId = s;	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_mpi_send(HMGL gr, int id)	{	_Gr_->MPI_Send(id);	}
void mgl_mpi_recv(HMGL gr, int id)	{	_Gr_->MPI_Recv(id);	}
void mgl_mpi_send_(uintptr_t *gr, int *id)	{	_GR_->MPI_Send(*id);	}
void mgl_mpi_recv_(uintptr_t *gr, int *id)	{	_GR_->MPI_Recv(*id);	}
//-----------------------------------------------------------------------------
void mgl_wnd_set_delay_(uintptr_t *gr, mreal *dt)	{	_GR_->SetDelay(*dt);	}
void mgl_wnd_set_delay(HMGL gr, mreal dt)	{	_Gr_->SetDelay(dt);	}
//-----------------------------------------------------------------------------
