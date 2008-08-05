/* mgl_f.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <mathgl.abalakin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "mgl/mgl_f.h"
#include "mgl/mgl.h"
#include "mgl/mgl_ab.h"
//-----------------------------------------------------------------------------
/// Delete mglGraph object. MUST be called for each mgl_create_* call.
void mgl_delete_graph_(long *gr)
{	if(gr)	delete (mglGraph *)(*gr);	}
//-----------------------------------------------------------------------------
/// Create mglData object.
long mgl_create_data_()
{	return (long)(new mglData());	};
/// Create mglData object with specified sizes.
long mgl_create_data_size_(int *nx, int *ny, int *nz)
{	return (long)(new mglData(*nx,*ny,*nz));	};
/// Create mglData object from data file.
long mgl_create_data_file_(const char *fname,int l)
{
	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	long r = long(new mglData(s));	delete []s;
	return r;
}
/// Delete mglData object. MUST be called for each mgl_create_data_* call.
void mgl_delete_data_(long *dat)
{	if(dat)	delete (mglData *)(*dat);	};
//-----------------------------------------------------------------------------
//		Setup mglGraph
//-----------------------------------------------------------------------------
/// Set color in mglGraph::Pal array at index \a n.
void mgl_set_palette_(long *gr, const char *colors, int l)
{
	char *s=new char[l+1];	memcpy(s,colors,l);	s[l]=0;
	if(gr)	_GR_->SetPalette(s);
	delete []s;
}
/// Set color in mglGraph::Pal array at index \a n.
void mgl_set_pal_color_(long *gr, int *n, float *r, float *g, float *b)
{	if(gr && *n<100)	_GR_->Pal[*n] = mglColor(*r,*g,*b);	}
/// Set number of colors in mglGraph::Pal array.
void mgl_set_pal_num_(long *gr, int *num)
{	if(gr && *num<100)	_GR_->NumPal = *num;	}
/// Set mglGraph::RotatedText.
void mgl_set_rotated_text_(long *gr, int *rotated)
{	_GR_->RotatedText = *rotated;	}
/// Set mglGraph::Cut.
void mgl_set_cut_(long *gr, int *cut)
{	_GR_->Cut = *cut;	}
/// Set mglGraph::CutMin, mglGraph::CutMax.
void mgl_set_cut_box_(long *gr, float *x1,float *y1,float *z1,float *x2,float *y2,float *z2)
{	_GR_->CutMin = mglPoint(*x1,*y1,*z1);	_GR_->CutMax = mglPoint(*x2,*y2,*z2);	}
/// Set type of transparency mglGraph::TranspType.
void mgl_set_transp_type_(long *gr, int *type)
{	_GR_->TranspType = *type;	}
/// Switch on/off transparency mglGraph::Transparent.
void mgl_set_transp_(long *gr, int *enable)
{	_GR_->Transparent = *enable;	}
/// Set width of rectangles in mglGraph::Bars().
void mgl_set_bar_width_(long *gr, float *width)
{	_GR_->BarWidth = *width;	}
/// Set size of marks mglGraph::BaseLineWidth.
void mgl_set_base_line_width_(long *gr, float *size)
{	_GR_->BaseLineWidth = *size;	}
/// Set size of marks mglGraph::MarkSize.
void mgl_set_mark_size_(long *gr, float *size)
{	_GR_->MarkSize = *size;	}
/// Set size of arrows mglGraph::ArrowSize.
void mgl_set_arrow_size_(long *gr, float *size)
{	_GR_->ArrowSize = *size;	}
/// Set default font size mglGraph::FontSize.
void mgl_set_font_size_(long *gr, float *size)
{	_GR_->FontSize = *size;	}
/// Set default alpha value mglGraph::AlphaDef.
void mgl_set_alpha_default_(long *gr, float *alpha)
{	_GR_->AlphaDef = *alpha;	}
/// Set size of frame in pixels. Normally this function is called internaly.
void mgl_set_size_(long *gr, int *width, int *height)
{	_GR_->SetSize(*width,*height);	}
void mgl_set_axial_dir_(long *gr, const char *dir, int)
{	_GR_->AxialDir = *dir;	}
void mgl_set_meshnum_(long *gr, int *num)
{	_GR_->MeshNum = *num;	}
void mgl_set_draw_face_(long *gr, int *enable)
{	_GR_->DrawFace = *enable;	}
void mgl_set_scheme_(long *gr, char *sch, int l)
{
	char *s;
	if(sch && sch[0])
	{	s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;	}
	else
	{	s=new char[7];		strcpy(s,"BbcyrR");			}
	_GR_->SetScheme(s);		delete []s;
}
/// Set font facename
void mgl_set_font_(long *gr, char *name, char *path, int l,int n)
{
	char *s=new char[l+1];		memcpy(s,name,l);	s[l]=0;
	char *d=new char[n+1];		memcpy(d,path,n);	d[n]=0;
	_GR_->GetFont()->Load(s,d);	delete []s;		delete []d;
}
/// Copy font data from another HMGL object
void mgl_copy_font_(long *gr, long *gr_from)
{	_GR_->GetFont()->Copy(((mglGraph *)(*gr_from))->GetFont());	}
/// Restore font data
void mgl_restore_font_(long *gr)
{	_GR_->GetFont()->Restore();	}
//-----------------------------------------------------------------------------
//		Export to file
//-----------------------------------------------------------------------------
/// Write the frame in file using PostScript format
void mgl_write_eps_(long *gr, const char *fname,const char *descr,int l,int n)
{
	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *d=new char[n+1];	memcpy(d,descr,n);	d[n]=0;
	_GR_->WriteEPS(s,d);	delete []s;		delete []d;
}
/// Write the frame in file using ScalableVectorGraphics format
void mgl_write_svg_(long *gr, const char *fname,const char *descr,int l,int n)
{
	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *d=new char[n+1];	memcpy(d,descr,n);	d[n]=0;
	_GR_->WriteSVG(s,d);	delete []s;		delete []d;
}
//-----------------------------------------------------------------------------
//		Setup frames transparency (alpha) and lightning
//-----------------------------------------------------------------------------
/// Create new frame.
int mgl_new_frame_(long *gr, int *id)
{	return _GR_->NewFrame(*id);	}
/// Finish frame drawing
void mgl_end_frame_(long *gr)
{	_GR_->EndFrame();	}
/// Get the number of created frames
int mgl_get_num_frame_(long *gr)
{	return _GR_->GetNumFrame();	}
/// Set the transparency on/off.
void mgl_set_alpha_(long *gr, int *enable)
{	_GR_->Alpha(*enable);	}
/// Set the fog on/off.
void mgl_set_fog_(long *gr, float *d, float *dz)
{	_GR_->Fog(*d, *dz);	}
/// Set the using of light on/off.
void mgl_set_light_(long *gr, int *enable)
{	_GR_->Light(*enable);	}
/// Add white light source.
void mgl_add_light_(long *gr, int *n, float *x, float *y, float *z, int *infty)
{	_GR_->Light(*n,mglPoint(*x,*y,*z),'w',0.5,*infty);	}
/// Add a light source with color {r,g,b}.
void mgl_add_light_rgb_(long *gr, int *n, float *x, float *y, float *z, int *infty,
						float *r, float *g, float *b,float *i)
{	_GR_->Light(*n,mglPoint(*x,*y,*z),mglColor(*r,*g,*b),*i,*infty);	}
// Set ambient light brightness
void mgl_set_ambbr_(long *gr, float *i)
{	_GR_->Ambient(*i);	}
//-----------------------------------------------------------------------------
//		Scale and rotate
//-----------------------------------------------------------------------------
/// Clear transformation matrix.
void mgl_identity_(long *gr)
{	_GR_->Identity();	}
/// Clear up the frame
void mgl_clf_(long *gr)
{	_GR_->Clf();	}
/// Clear up the frame
void mgl_clf_rgb_(long *gr, float *r, float *g, float *b)
{	_GR_->Clf(mglColor(*r,*g,*b));	}
/// Put further plotting in some region of whole frame surface.
void mgl_subplot_(long *gr, int *nx,int *ny,int *m)
{	_GR_->SubPlot(*nx,*ny,*m);	}
/// Put further plotting in some region of whole frame surface.
void mgl_subplot_d_(long *gr, int *nx,int *ny,int *m,float *dx,float *dy)
{	_GR_->SubPlot(*nx,*ny,*m,*dx,*dy);	}
/// Put further plotting in some region of whole frame surface.
void mgl_inplot_(long *gr, float *x1,float *x2,float *y1,float *y2)
{	_GR_->InPlot(*x1,*x2,*y1,*y2);	}
/// Set aspect ratio for further plotting.
void mgl_aspect_(long *gr, float *Ax,float *Ay,float *Az)
{	_GR_->Aspect(*Ax,*Ay,*Az);	}
/// Rotate a further plotting.
void mgl_rotate_(long *gr, float *TetX,float *TetZ,float *TetY)
{	_GR_->Rotate(*TetX,*TetZ,*TetY);	}
/// Rotate a further plotting around vector {x,y,z}.
void mgl_rotate_vector_(long *gr, float *Tet,float *x,float *y,float *z)
{	_GR_->RotateN(*Tet,*x,*y,*z);	}
void mgl_perspective_(long *gr, float val)
{	_GR_->Perspective(val);	}
//-----------------------------------------------------------------------------
//		Axis functions
//-----------------------------------------------------------------------------
/// Switch on/off ticks tunning and set factor position for tunned ticks.
void mgl_tune_ticks_(long *gr, int *tune, float *fact_pos)
{	_GR_->TuneTicks = *tune;	_GR_->FactorPos = *fact_pos;	}
/// Set ticks interval mglGraph::dx, mglGraph::dy, mglGraph::dz.
void mgl_set_ticks_(long *gr, float *DX, float *DY, float *DZ)
{	_GR_->dx=*DX;	_GR_->dy=*DY;	_GR_->dz=*DZ;	}
/// Set number of subticks mglGraph::NSx, mglGraph::NSy, mglGraph::NSz.
void mgl_set_subticks_(long *gr, int *NX, int *NY, int *NZ)
{	_GR_->NSx=*NX;	_GR_->NSy=*NY;	_GR_->NSz=*NZ;	}
/// Set the values of mglGraph::Cmin and mglGraph::Cmax
void mgl_set_caxis_(long *gr, float *C1,float *C2)
{	_GR_->CAxis(*C1,*C2);	}
void mgl_set_crange_(long *gr, long *a, int *add)	{	_GR_->CRange(_D_(a),*add);	}
void mgl_set_xrange_(long *gr, long *a, int *add)	{	_GR_->XRange(_D_(a),*add);	}
void mgl_set_yrange_(long *gr, long *a, int *add)	{	_GR_->YRange(_D_(a),*add);	}
void mgl_set_zrange_(long *gr, long *a, int *add)	{	_GR_->ZRange(_D_(a),*add);	}
/// Safetly set the value for mglGraph::Min, mglGraph::Max and mglGraph::Org members of the class.
void mgl_set_axis_(long *gr, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2, float *x0, float *y0, float *z0)
{	_GR_->Axis(mglPoint(*x1,*y1,*z1),mglPoint(*x2,*y2,*z2),mglPoint(*x0,*y0,*z0));	}
/// Safetly set the transformation formulas for coordinate.
void mgl_set_func_(long *gr, const char *EqX,const char *EqY,const char *EqZ,int lx,int ly,int lz)
{
	char *sx=new char[lx+1];	memcpy(sx,EqX,lx);	sx[lx]=0;
	char *sy=new char[ly+1];	memcpy(sy,EqY,ly);	sy[ly]=0;
	char *sz=new char[lz+1];	memcpy(sz,EqZ,lz);	sz[lz]=0;
	_GR_->Axis(sx,sy,sz);
	delete []sx;	delete []sy;	delete []sz;
}
void mgl_set_cutoff_(long *gr, const char *EqC, int l)
{
	char *s=new char[l+1];	memcpy(s,EqC,l);	s[l]=0;
	_GR_->CutOff(s);
	delete []s;
}
/// Draws bounding box outside the plotting volume by default color.
void mgl_box_(long *gr, int *ticks)
{	_GR_->Box(NC,*ticks);	}
/// Draws bounding box outside the plotting volume with color \a c.
void mgl_box_str_(long *gr, const char *col, int *ticks,int l)
{
	char *s=new char[l+1];	memcpy(s,col,l);	s[l]=0;
	_GR_->Box(s,*ticks);	delete []s;
}
/// Draws bounding box outside the plotting volume.
void mgl_box_rgb_(long *gr, float *r, float *g, float *b, int *ticks)
{	_GR_->Box(mglColor(*r,*g,*b),*ticks);	}
/// Draw axises with ticks in directions determined by string parameter \a dir.
void mgl_axis_(long *gr, const char *dir,int l)
{	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;	_GR_->Axis(s);	delete []s;	}
/// Draw grid lines perpendicular to direction determined by string parameter \a dir.
void mgl_axis_grid_(long *gr, const char *dir,const char *pen,int l,int n)
{
	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	char *p=new char[n+1];	memcpy(p,pen,n);	p[n]=0;
	_GR_->Grid(s,p);	delete []s;	delete []p;
}
/// Print the label \a text for axis \a dir.
void mgl_label_(long *gr, const char *dir, const char *text,int,int l)
{
	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	_GR_->Label(*dir, s);	delete []s;
}
/// Print the label \a text for axis \a dir.
void mgl_label_ext_(long *gr, const char *dir, const char *text, int *pos, float *size, float *shift,int,int l)
{
	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	_GR_->Label(*dir, s, *pos, *size, *shift);	delete []s;
}
//-----------------------------------------------------------------------------
//		Simple drawing
//-----------------------------------------------------------------------------
/// Draws the red point (ball) at position \a {x,y,z}.
void mgl_ball_(long *gr, float *x,float *y,float *z)
{	_GR_->Ball(mglPoint(*x,*y,*z));	}
/// Draws the point (ball) at position \a {x,y,z} with color {r,g,b}.
void mgl_ball_rgb_(long *gr, float *x, float *y, float *z, float *r, float *g, float *b, float *alpha)
{	_GR_->Ball(*x,*y,*z,mglColor(*r,*g,*b),*alpha);	}
/// Draws the point (ball) at position \a p with color \a col.
void mgl_ball_str_(long *gr, float *x, float *y, float *z, const char *col,int)
{	_GR_->Ball(mglPoint(*x,*y,*z),*col);	}
/// Draws the line between points with style \a sch.
void mgl_line_(long *gr, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2,
				const char *pen,int *n,int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	_GR_->Line(mglPoint(*x1,*y1,*z1),mglPoint(*x2,*y2,*z2),s,*n);	delete []s;
}
void mgl_facex_(long* gr, float *x0, float *y0, float *z0, float *wy, float *wz, const char *stl, float *dx, float *dy, int l)
{
	char *s=new char[l+1];	memcpy(s,stl,l);	s[l]=0;
	_GR_->FaceX(*x0,*y0,*z0,*wy,*wz,s,*dx,*dy);	delete []s;
}
void mgl_facey_(long* gr, float *x0, float *y0, float *z0, float *wx, float *wz, const char *stl, float *dx, float *dy, int l)
{
	char *s=new char[l+1];	memcpy(s,stl,l);	s[l]=0;
	_GR_->FaceX(*x0,*y0,*z0,*wx,*wz,s,*dx,*dy);	delete []s;
}
void mgl_facez_(long* gr, float *x0, float *y0, float *z0, float *wx, float *wy, const char *stl, float *dx, float *dy, int l)
{
	char *s=new char[l+1];	memcpy(s,stl,l);	s[l]=0;
	_GR_->FaceX(*x0,*y0,*z0,*wx,*wy,s,*dx,*dy);	delete []s;
}
void mgl_curve_(long* gr, float *x1, float *y1, float *z1, float *dx1, float *dy1, float *dz1, float *x2, float *y2, float *z2, float *dx2, float *dy2, float *dz2, const char *pen,int *n, int l)
{
	char *s=new char[l+1];	memcpy(s,pen,l);	s[l]=0;
	_GR_->Curve(mglPoint(*x1,*y1,*z1), mglPoint(*dx1,*dy1,*dz1), mglPoint(*x2,*y2,*z2), mglPoint(*dx2,*dy2,*dz2), s, *n);	delete []s;
}

/// Print string \a str in position \a p with font size \a size.
void mgl_puts_(long *gr, float *x, float *y, float *z,const char *text,int l)
{
	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	_GR_->Puts(mglPoint(*x,*y,*z),s);	delete []s;
}
/// Print string \a str in position \a p along direction \a d with font size \a size.
void mgl_puts_dir_(long *gr, float *x, float *y, float *z, float *dx, float *dy, float *dz, const char *text, float *size, int l)
{
	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	_GR_->Puts(mglPoint(*x,*y,*z), mglPoint(*dx,*dy,*dz), s, 't', *size);
	delete []s;
}
/// Print unrotated string \a str in position \a p with font size \a size.
void mgl_text_(long *gr, float *x, float *y, float *z,const char *text,int l)
{
	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	_GR_->Text(mglPoint(*x,*y,*z),s);	delete []s;
}
/// Print string \a str in position \a p with font size \a size.
void mgl_puts_ext_(long *gr, float *x, float *y, float *z,const char *text,
					const char *font, float *size, const char *dir,int l,int n,int)
{
	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,font,n);	f[n]=0;
	_GR_->Puts(mglPoint(*x,*y,*z),s,f,*size,*dir);	delete []s;	delete []f;
}
/// Print unrotated string \a str in position \a p with font size \a size.
void mgl_text_ext_(long *gr, float *x, float *y, float *z,const char *text,
					const char *font, float *size,const char *dir,int l,int n,int)
{
	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,font,n);	f[n]=0;
	_GR_->Text(mglPoint(*x,*y,*z),s,f,*size,*dir);	delete []s;	delete []f;
}
/// Draw colorbar at edge of axis
void mgl_colorbar_(long *gr, const char *sch,int *where,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	_GR_->Colorbar(s,*where);	delete []s;
}
/// Plot data depending on its dimensions and \a type parameter
void mgl_simple_plot_(long *gr, long *a, int *type, const char *sch,int l)
{
	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	if(gr && a)	_GR_->SimplePlot(_D_(a),*type,s);
	delete []s;
}
//-----------------------------------------------------------------------------
//		Plot legend drawing
//-----------------------------------------------------------------------------
/// Add string to legend
void mgl_add_legend_(long *gr, const char *text,const char *style,int l,int n)
{
	char *s=new char[l+1];	memcpy(s,text,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,style,n);	f[n]=0;
	if(gr)	_GR_->AddLegend(s,f);	delete []s;	delete []f;
}
/// Clear saved legend string
void mgl_clear_legend_(long *gr)
{	if(gr)	_GR_->ClearLegend();	}
/// Draw legend of accumulated strings at position (x, y) by \a font with \a size
void mgl_legend_xy_(long *gr, float *x, float *y, const char *font, float *size, float *llen,int l)
{
	char *s=new char[l+1];	memcpy(s,font,l);	s[l]=0;
	if(gr)	_GR_->Legend(*x, *y, s, *size,*llen);		delete []s;
}
/// Draw legend of accumulated strings by \a font with \a size
void mgl_legend_(long *gr, int *where, const char *font, float *size, float *llen,int l)
{
	char *s=new char[l+1];	memcpy(s,font,l);	s[l]=0;
	if(gr)	_GR_->Legend(*where, s, *size,*llen);	delete []s;
}
//-----------------------------------------------------------------------------
const unsigned char *mgl_get_rgb_(long *graph)
{
	mglGraphAB *g = dynamic_cast<mglGraphAB *>((mglGraph *)(*graph));
	return g ? g->GetBits():0;
}
const unsigned char *mgl_get_rgba_(long *graph)
{
	mglGraphAB *g = dynamic_cast<mglGraphAB *>((mglGraph *)(*graph));
	return g ? g->GetRGBA():0;
}
int mgl_get_width_(long *graph)
{
	mglGraphAB *g = dynamic_cast<mglGraphAB *>((mglGraph *)(*graph));
	return g ? g->GetWidth():0;
}
int mgl_get_height_(long *graph)
{
	mglGraphAB *g = dynamic_cast<mglGraphAB *>((mglGraph *)(*graph));
	return g ? g->GetHeight():0;
}
//-----------------------------------------------------------------------------
float mgl_data_get_value_(long *d, int *i, int *j, int *k)
{	return	_DT_->a[*i+_DT_->nx*(*j+*k*_DT_->ny)];	}
void mgl_data_set_value_(long *d, float *v, int *i, int *j, int *k)
{	_DT_->a[*i+_DT_->nx*(*j+*k*_DT_->ny)] = *v;	}
//-----------------------------------------------------------------------------
/// Zoom in/out a part of picture
void mgl_set_zoom_(long *gr, float *x1, float *y1, float *x2, float *y2)
{	_GR_->Zoom(*x1,*y1,*x2,*y2);	}
void mgl_set_plotfactor_(long *gr, float *val)
{	_GR_->PlotFactor = *val;	}
void mgl_set_axis_3d_(long *gr, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2)
{	_GR_->Axis(mglPoint(*x1,*y1,*z1),mglPoint(*x2,*y2,*z2));	}
void mgl_set_axis_2d_(long *gr, float *x1, float *y1, float *x2, float *y2)
{	_GR_->Axis(mglPoint(*x1,*y1),mglPoint(*x2,*y2));	}
void mgl_set_origin_(long *gr, float *x0, float *y0, float *z0)
{	_GR_->Org = mglPoint(*x0,*y0,*z0);	}
void mgl_set_tick_origin_(long *gr, float *x0, float *y0, float *z0)
{	_GR_->OrgT = mglPoint(*x0,*y0,*z0);	}
//-----------------------------------------------------------------------------
