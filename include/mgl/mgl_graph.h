/***************************************************************************
 * mgl_graph.h is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <mgl/mgl_c.h>

/*typedef mglGraph *HMGL;
typedef mglData *HMDT;
typedef mglParse *HMPR;*/

// NOTE: OONLY ONE mglGraph and mglParse can be defined!!!
#if(!defined(_MGL_PARSE_H_) && !defined(_MGL_H_))
#define _MGL_PARSE_H_
#define _MGL_H_
//-----------------------------------------------------------------------------
#ifndef _MGL_DATA_H_
/// Class for incapsulating point in space
struct mglPoint
{
	mreal x,y,z;
 	mglPoint(mreal X=0,mreal Y=0,mreal Z=0){x=X;y=Y;z=Z;};
};
inline mglPoint operator+(const mglPoint &a, const mglPoint &b)
{	return mglPoint(a.x+b.x, a.y+b.y, a.z+b.z);	};
inline mglPoint operator-(const mglPoint &a, const mglPoint &b)
{	return mglPoint(a.x-b.x, a.y-b.y, a.z-b.z);	};
inline mglPoint operator*(mreal b, const mglPoint &a)
{	return mglPoint(a.x*b, a.y*b, a.z*b);	};
inline mglPoint operator*(const mglPoint &a, mreal b)
{	return mglPoint(a.x*b, a.y*b, a.z*b);	};
inline mglPoint operator/(const mglPoint &a, mreal b)
{	return mglPoint(a.x/b, a.y/b, a.z/b);	};
inline mreal operator*(const mglPoint &a, const mglPoint &b)
{	return a.x*b.x+a.y*b.y+a.z*b.z;	};
inline mglPoint operator&(const mglPoint &a, const mglPoint &b)
{	return a - b*((a*b)/(b*b));	};
inline mglPoint operator|(const mglPoint &a, const mglPoint &b)
{	return b*((a*b)/(b*b));	};
inline mglPoint operator^(const mglPoint &a, const mglPoint &b)
{	return mglPoint(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x);	};
inline mglPoint operator!(const mglPoint &a)
{	return (a.x==0 && a.y==0)?mglPoint(1,0,0):mglPoint(-a.y/hypot(a.x,a.y), a.x/hypot(a.x,a.y), 0);	};
inline bool operator==(const mglPoint &a, const mglPoint &b)
{	return !memcmp(&a, &b, sizeof(mglPoint));	}
inline bool operator!=(const mglPoint &a, const mglPoint &b)
{	return memcmp(&a, &b, sizeof(mglPoint));	}
inline mreal Norm(const mglPoint &p)
{	return sqrt(p.x*p.x+p.y*p.y+p.z*p.z);	};
#endif
//-----------------------------------------------------------------------------
class mglGraph
{
public:
	HMGL self;
	mglGraph(int kind=0, int width=600, int height=400)
	{
		if(kind==1)	self=mgl_create_graph_ps(width, height);
		else if(kind==2)	self=mgl_create_graph_gl();
		else if(kind==3)	self=mgl_create_graph_idtf();
		else		self=mgl_create_graph_zb(width, height);
	};
	~mglGraph()	{	mgl_delete_graph(self);	};

	inline void DefaultPlotParam()	{	mgl_set_def_param(self);	};
	inline void SetPalColor(int n, float r, float g, float b)
	{	if(n>=0 && n<100)	mgl_set_pal_color(self, n, r, g, b);	};
	inline void SetPalNum(int n)	{	if(n>=0 && n<100)	mgl_set_pal_num(self, n);	};
	inline void SetPalette(const char *colors)	{	mgl_set_palette(self, colors);	};
	inline void SetScheme(const char *colors)	{	mgl_set_scheme(self, colors);	};
	inline void SetRotatedText(bool rotated)	{	mgl_set_rotated_text(self, rotated);	};
	inline void SetCut(bool cut)				{	mgl_set_cut(self, cut);	};
	inline void SetCutBox(float x1,float y1,float z1,float x2,float y2,float z2)
	{	mgl_set_cut_box(self, x1, y1, z1, x2, y2, z2);	};
	inline void SetTickLen(float len, float stt=1)
	{	mgl_set_tick_len(self, len, stt);	};
	inline void SetTickStl(const char *stl, const char *sub=0)
	{	mgl_set_tick_stl(self, stl, sub);	};
	inline void SetBaseLineWidth(float width)	{	mgl_set_base_line_width(self, width);	};
	inline void SetSize(int width, int height)	{	mgl_set_size(self, width, height);	};
	inline void SetBarWidth(float width)	{	mgl_set_bar_width(self, width);	};
	inline void SetMarkSize(float size)		{	mgl_set_mark_size(self, size);	};
	inline void SetArrowSize(float size)	{	mgl_set_arrow_size(self, size);	};
	inline void SetFontSize(float size)		{	mgl_set_font_size(self, size);	};
	inline void SetFontDef(char *fnt)		{	mgl_set_font_def(self, fnt);	};
	inline void SetAlphaDef(float alpha)	{	mgl_set_alpha_default(self, alpha);	};
	inline void SetAxialDir(char dir)		{	mgl_set_axial_dir(self, dir);	};
	inline void SetMeshNum(int num)			{	mgl_set_meshnum(self, num);		};
	inline void Zoom(float x1, float y1, float x2, float y2)
	{	mgl_set_zoom(self, x1, y1, x2, y2);	};
	inline void SetPlotFactor(float val)	{	mgl_set_plotfactor(self, val);	};
	inline void SetDrawFace(bool enable)	{	mgl_set_draw_face(self, enable);};
	inline void SetLegendBox(bool enable)	{	mgl_set_legend_box(self, enable);	};
	inline void LoadFont(const char *name, const char *path=NULL)
	{	mgl_load_font(self, name, path);	};
	inline void CopyFont(mglGraph *gr)		{	mgl_copy_font(self, gr->self);};
	inline void RestoreFont()				{	mgl_restore_font(self);	};

	inline void ShowImage(const char *viewer="kuickshow", bool keep=0)
	{	mgl_show_image(self, viewer, keep);	};
	inline void WriteFrame(const char *fname,const char *descr="")
	{	mgl_write_frame(self, fname, descr);	};
	inline void WriteIDTF(const char *fname,const char *descr="")
	{	mgl_write_idtf(self, fname, descr);	};
	inline void WriteJPEG(const char *fname,const char *descr="")
	{	mgl_write_jpg(self, fname, descr);	};
	inline void WritePNG(const char *fname,const char *descr="", bool alpha=true)
	{	if(alpha)	mgl_write_png(self, fname, descr);
		else	mgl_write_png_solid(self, fname, descr);	};
	inline void WriteEPS(const char *fname,const char *descr="")
	{	mgl_write_eps(self, fname, descr);	};
	inline void WriteSVG(const char *fname,const char *descr="")
	{	mgl_write_svg(self, fname, descr);	};
	inline void WriteGIF(const char *fname,const char *descr="")
	{	mgl_write_gif(self, fname, descr);	};
	inline void StartGIF(const char *fname, int ms=100)
	{	mgl_start_gif(self, fname,ms);	};
	inline void CloseGIF()		{	mgl_close_gif(self);	};
	inline void NewFrame()		{	mgl_new_frame(self);	};
	inline void EndFrame()		{	mgl_end_frame(self);	};
	inline int GetNumFrame()	{	return mgl_get_num_frame(self);	};
	inline void ResetFrames()	{	mgl_reset_frames(self);	};
	inline void Flush()			{	mgl_flush(self);	};

	void GetRGB(char *imgdata, int imglen)
	{
		int w=mgl_get_width(self);
		int h=mgl_get_height(self);
		if(imglen>=3*w*h)
		{
			imglen=3*w*h;
			memcpy(imgdata, mgl_get_rgb(self),imglen);
		}
	};
	inline void GetRGBA(char *imgdata, int imglen)
	{
		int w=mgl_get_width(self);
		int h=mgl_get_height(self);
		if(imglen>=4*w*h)
		{
			imglen=4*w*h;
			memcpy(imgdata, mgl_get_rgba(self),imglen);
		}
	};
	inline void GetBGRN(char *imgdata, int imglen)
	{
		int w,h,i;
		w=mgl_get_width(self);
		h=mgl_get_height(self);
		const char *buf=(const char *)mgl_get_rgb(self);
		if(imglen>=4*w*h)
		{
			imglen=4*w*h;
			for(i=0;i<w*h;i++)
			{
				imgdata[4*i]   = buf[3*i+2];
				imgdata[4*i+1] = buf[3*i+1];
				imgdata[4*i+2] = buf[3*i];
				imgdata[4*i+3] = 255;
			}
		}
	};
	inline int GetWidth()	{	return mgl_get_width(self);	};
	inline int GetHeight()	{	return mgl_get_height(self);};
	inline mglPoint CalcXYZ(int xs, int ys)
	{
		mreal x,y,z;
		mgl_calc_xyz(self,xs,ys,&x,&y,&z);
		return mglPoint(x,y,z);
	};

	inline void SetTranspType(int type)		{	mgl_set_transp_type(self, type);};
	inline void SetTransparent(bool enable)	{	mgl_set_transp(self, enable);};
	inline void Alpha(bool enable)			{	mgl_set_alpha(self, enable);	};
	inline void Fog(float d, float dz=0.25)	{	mgl_set_fog(self, d, dz);		};
	inline void Light(bool enable)			{	mgl_set_light(self, enable);	};
	inline void Light(int n,bool enable)	{	mgl_set_light_n(self, n, enable);	};
	inline void AddLight(int n, float x, float y, float z, char c='w')
	{	mgl_add_light(self, n, x, y, z, c);	};
	inline void SetAmbient(float i)			{	mgl_set_ambbr(self, i);	};

	inline void Identity()					{	mgl_identity(self);	};
	inline void Clf(float r=1, float g=1, float b=1)
	{	mgl_clf_rgb(self, r, g, b);	};
	inline void SubPlot(int nx,int ny,int m, float dx=0, float dy=0)
	{	mgl_subplot_d(self, nx, ny, m, dx, dy);	};
	inline void InPlot(float x1,float x2,float y1,float y2, bool rel=false)
	{	if(rel)	mgl_inplot(self, x1, x2, y1, y2);
		else	mgl_relplot(self, x1, x2, y1, y2);	};
	inline void ColumnPlot(int num, int ind)
	{	mgl_columnplot(self,num,ind);	};
	inline void Aspect(float Ax,float Ay,float Az)
	{	mgl_aspect(self, Ax, Ay, Az);		};
	inline void Rotate(float TetX,float TetZ=0,float TetY=0)
	{	mgl_rotate(self, TetX, TetZ, TetY);	};
	inline void RotateN(float Tet,float x,float y,float z)
	{	mgl_rotate_vector(self, Tet, x, y, z);	};
	inline void Perspective(float val)
	{	mgl_perspective(self, val);	};

	inline void SetTicks(char dir, float d=-5, int ns=0, float org=NAN)
	{	mgl_set_ticks_dir(self, dir, d, ns, org);	};
	inline void AdjustTicks(const char *dir="xyz")
	{	mgl_adjust_ticks(self, dir);	};

	inline void SetCRange(float c1, float c2)
	{	mgl_set_caxis(self, c1, c2);	};
	inline void SetRanges(float x1, float x2, float y1, float y2, float z1=0, float z2=0)
	{	mgl_set_axis(self, x1, y1, z1, x2, y2, z2, NAN, NAN, NAN);	};
	inline void SetAutoRanges(float x1, float x2, float y1=0, float y2=0, float z1=0, float z2=0)
	{	mgl_set_auto(self, x1, x2, y1, y2, z1, z2);	};
	inline void SetOrigin(float x0, float y0, float z0=NAN)
	{	mgl_set_origin(self, x0, y0, z0);	};
	inline void SetCRange(mglData *dat, bool add=false)
	{	mgl_set_crange(self, dat, add);	};
	inline void SetXRange(mglData *dat, bool add=false)
	{	mgl_set_xrange(self, dat, add);	};
	inline void SetYRange(mglData *dat, bool add=false)
	{	mgl_set_yrange(self, dat, add);	};
	inline void SetZRange(mglData *dat, bool add=false)
	{	mgl_set_zrange(self, dat, add);	};
	inline void SetFunc(const char *EqX, const char *EqY, const char *EqZ=NULL, const char *EqA=NULL)
	{	mgl_set_func_ext(self, EqX, EqY, EqZ,EqA);	};
	inline void SetCoor(int how)		{	mgl_set_coor(self, how);	};
	inline void Ternary(bool val)		{	mgl_set_ternary(self, val);	};
	inline void CutOff(const char *EqC)	{	mgl_set_cutoff(self, EqC);	};

	inline void Box(const char *col="k", bool ticks=true)
	{	mgl_box_str(self, col, ticks);	};
	inline void Axis(const char *dir="xyz", bool adjust=false)
	{	if(adjust)	mgl_adjust_ticks(self,dir);	mgl_axis(self, dir);	};
	inline void Grid(const char *dir="xyz",const char *pen="B")
	{	mgl_axis_grid(self, dir, pen);	};
	inline void Label(char dir, const char *text, int pos=+1, float size=-1.4, float shift=0)
	{	mgl_label_ext(self, dir, text, pos, size, shift);	};
	inline void SetTuneTicks(bool tune, float fact_pos=1.15)
	{	mgl_tune_ticks(self, tune, fact_pos);	};

	inline void SetXTT(const wchar_t *templ)	{	mgl_set_xttw(self, templ);	};
	inline void SetYTT(const wchar_t *templ)	{	mgl_set_yttw(self, templ);	};
	inline void SetZTT(const wchar_t *templ)	{	mgl_set_zttw(self, templ);	};
	inline void SetCTT(const wchar_t *templ)	{	mgl_set_cttw(self, templ);	};
	inline void SetXTT(const char *templ)		{	mgl_set_xtt(self, templ);	};
	inline void SetYTT(const char *templ)		{	mgl_set_ytt(self, templ);	};
	inline void SetZTT(const char *templ)		{	mgl_set_ztt(self, templ);	};
	inline void SetCTT(const char *templ)		{	mgl_set_ctt(self, templ);	};

	inline void Ball(float x, float y, float z, char col='r')
	{	mgl_ball_str(self, x, y, z, col);	};
	inline void Mark(float x, float y, float z, char mark)
	{	mgl_mark(self, x, y, z, mark);	};
	inline void Line(float x1, float y1, float z1, float x2, float y2, float z2, const char *pen="B",int n=2)
	{	mgl_line(self, x1, y1, z1, x2, y2, z2, pen, n);	};
	inline void FaceX(float x0, float y0, float z0, float wy, float wz, const char *stl="w", float dx=0, float dy=0)
	{	mgl_facex(self, x0, y0, z0, wy, wz, stl, dx, dy);	};
	inline void FaceY(float x0, float y0, float z0, float wx, float wz, const char *stl="w", float dx=0, float dy=0)
	{	mgl_facey(self, x0, y0, z0, wx, wz, stl, dx, dy);	};
	inline void FaceZ(float x0, float y0, float z0, float wx, float wy, const char *stl="w", float dx=0, float dy=0)
	{	mgl_facez(self, x0, y0, z0, wx, wy, stl, dx, dy);	};
	inline void Curve(float x1, float y1, float z1, float dx1, float dy1, float dz1, float x2, float y2, float z2, float dx2, float dy2, float dz2, const char *pen="B", int n=100)
	{	mgl_curve(self, x1, y1, z1, dx1, dy1, dz1, x2, y2, z2, dx2, dy2, dz2, pen, n);	};
	inline void Drop(float x0, float y0, float z0, float dx, float dy, float dz, float r, const char *col="r", float shift=1, float ap=1)
	{	mgl_drop(self, x0, y0, z0, dx, dy, dz, r, col, shift, ap);	};
	inline void Sphere(float x0, float y0, float z0, float r, const char *col="r")
	{	mgl_sphere(self, x0, y0, z0, r, col);	};
	inline void Cone (float x1, float y1, float z1, float x2, float y2, float z2, float r1, float r2=-1, const char *stl="B", bool edge=false)
	{	mgl_cone(self, x1,y1,z1,x2,y2,z2,r1,r2,stl,edge);	};

	inline void Putsw(float x, float y, float z,const wchar_t *text,const char *font="C",float size=-1,char dir=0)
	{	mgl_putsw_ext(self, x, y, z, text, font, size, dir);	};
	inline void Puts(float x, float y, float z,const char *text,const char *font="C",float size=-1,char dir=0)
	{	mgl_puts_ext(self, x, y, z, text, font, size, dir);	};
	inline void Putsw(float x, float y, float z, float dx, float dy, float dz, const wchar_t *text, float size=-1)
	{	mgl_putsw_dir(self, x, y, z, dx, dy, dz, text, size);	};
	inline void Puts(float x, float y, float z, float dx, float dy, float dz, const char *text,float size=-1)
	{	mgl_puts_dir(self, x, y, z, dx, dy, dz, text, size);	};
	inline void Title(const char *text, const char *font="C", float size=-2)
	{	mgl_title(self, text, font, size);	};
	inline void Title(const wchar_t *text, const char *font="C", float size=-2)
	{	mgl_titlew(self, text, font, size);	};

	inline void Colorbar(const char *sch="",int where=0)
	{	mgl_colorbar(self, sch, where);	};
	inline void Colorbar(const char *sch,int where,float x,float y,float w,float h)
	{	mgl_colorbar_ext(self, sch, where,x,y,w,h);	};
	inline void Colorbar(int where,float x,float y,float w,float h)
	{	mgl_colorbar_ext(self, 0, where,x,y,w,h);	};
	inline void Colorbar(mglData *val, const char *sch="",int where=0)
	{	mgl_colorbar_val(self, val, sch, where);	};
	inline void SimplePlot(mglData *a, int type, const char *stl="")
	{	mgl_simple_plot(self, a, type, stl);	};
	inline void AddLegend(const char *text,const char *style)
	{	mgl_add_legend(self, text, style);	};
	inline void AddLegendw(const wchar_t *text,const char *style)
	{	mgl_add_legendw(self, text, style);	};
	inline void ClearLegend()
	{	mgl_clear_legend(self);	};
	inline void Legend(float x, float y, const char *font="L", float size=-0.8, float llen=0.1)
	{	mgl_legend_xy(self, x, y, font, size, llen);	};
	inline void Legend(int where=3, const char *font="L", float size=-0.8, float llen=0.1)
	{	mgl_legend(self, where, font, size, llen);	};

	inline void Plot(const char *fy, const char *stl="", int n=100)
	{	mgl_fplot(self, fy, stl, n);	};
	inline void Plot(const char *fx, const char *fy, const char *fz, const char *stl="", int n=100)
	{	mgl_fplot_xyz(self, fx, fy, fz, stl, n);	};
	inline void Plot(mglData *x, mglData *y, mglData *z, const char *pen="")
	{	mgl_plot_xyz(self, x, y, z, pen);	};
	inline void Plot(mglData *x, mglData *y, const char *pen="")
	{	mgl_plot_xy(self, x, y, pen);	};
	inline void Plot(mglData *y, const char *pen="")
	{	mgl_plot(self, y, pen);	};
	inline void BoxPlot(mglData *x, mglData *y, const char *pen="")
	{	mgl_boxplot_xy(self, x, y, pen);	};
	inline void BoxPlot(mglData *y, const char *pen="")
	{	mgl_boxplot(self, y, pen);	};
	inline void Radar(mglData *a, const char *pen="", float r=-1)
	{	mgl_radar(self, a, pen, r);	};
	inline void Tens(mglData *x, mglData *y, mglData *z, mglData *c, const char *pen="")
	{	mgl_tens_xyz(self, x, y, z, c, pen);	};
	inline void Tens(mglData *x, mglData *y, mglData *c, const char *pen="")
	{	mgl_tens_xy(self, x, y, c, pen);	};
	inline void Tens(mglData *y, mglData *c, const char *pen="")
	{	mgl_tens(self, y, c, pen);	};
	inline void Area(mglData *x, mglData *y, mglData *z, const char *pen="")
	{	mgl_area_xyz(self, x, y, z, pen);	};
	inline void Area(mglData *x, mglData *y, const char *pen="", bool sum=false)
	{	if(sum)	mgl_area_xy(self, x, y, pen);	else	mgl_area_xys(self, x, y, pen);	};
	inline void Area(mglData *y, const char *pen="", bool sum=false)
	{	if(sum)	mgl_area(self, y, pen);	else	mgl_area_s(self, y, pen);	};
	inline void Stem(mglData *x, mglData *y, mglData *z, const char *pen="")
	{	mgl_stem_xyz(self, x, y, z, pen);	};
	inline void Stem(mglData *x, mglData *y, const char *pen="")
	{	mgl_stem_xy(self, x, y, pen);	};
	inline void Stem(mglData *y, const char *pen="")
	{	mgl_stem(self, y, pen);	};
	inline void Step(mglData *x, mglData *y, mglData *z, const char *pen="")
	{	mgl_step_xyz(self, x, y, z, pen);	};
	inline void Step(mglData *x, mglData *y, const char *pen="")
	{	mgl_step_xy(self, x, y, pen);	};
	inline void Step(mglData *y, const char *pen="")
	{	mgl_step(self, y, pen);	};
	inline void Bars(mglData *x, mglData *y, mglData *z, const char *pen="")
	{	mgl_bars_xyz(self, x, y, z, pen);	};
	inline void Bars(mglData *x, mglData *y, const char *pen="")
	{	mgl_bars_xy(self, x, y, pen);	};
	inline void Bars(mglData *y, const char *pen="")
	{	mgl_bars(self, y, pen);	};

	inline void Barh(mglData *y, mglData *v, const char *pen="")
	{	mgl_barh_yx(self, y, v, pen);	};
	inline void Barh(mglData *v, const char *pen="")
	{	mgl_barh(self, v, pen);	};
	inline void Region(mglData *y1, mglData *y2, const char *pen=NULL, bool inside=true)
	{	mgl_region(self, y1,y2,pen,inside);	};
	inline void Region(mglData *x, mglData *y1, mglData *y2, const char *pen=NULL, bool inside=true)
	{	mgl_region_xy(self, x,y1,y2,pen,inside);	};

	inline void Torus(mglData *r, mglData *z, const char *pen="")
	{	mgl_torus(self, r, z, pen);	};
	inline void Text(mglData *x, mglData *y, mglData *z, const char *text, const char *font="", float size=-1)
	{	mgl_text_xyz(self, x, y, z, text, font, size);	};
	inline void Text(mglData *x, mglData *y, const char *text, const char *font="", float size=-1)
	{	mgl_text_xy(self, x, y, text, font, size);	};
	inline void Text(mglData *y, const char *text, const char *font="", float size=-1)
	{	mgl_text_y(self, y, text, font, size);	};
	inline void Chart(mglData *a, const char *colors="")
	{	mgl_chart(self, a, colors);	};
	inline void Error(mglData *y, mglData *ey, const char *pen="")
	{	mgl_error(self, y, ey, pen);	};
	inline void Error(mglData *x, mglData *y, mglData *ey, const char *pen="")
	{	mgl_error_xy(self, x, y, ey, pen);	};
	inline void Error(mglData *x, mglData *y, mglData *ex, mglData *ey, const char *pen="")
	{	mgl_error_exy(self, x, y, ex, ey, pen);	};
	inline void Mark(mglData *x, mglData *y, mglData *z, mglData *r, const char *pen)
	{	mgl_mark_xyz(self, x, y, z, r, pen);	};
	inline void Mark(mglData *x, mglData *y, mglData *r, const char *pen)
	{	mgl_mark_xy(self, x, y, r, pen);	};
	inline void Mark(mglData *y, mglData *r, const char *pen)
	{	mgl_mark_y(self, y, r, pen);	};
	inline void TextMark(mglData *x, mglData *y, mglData *z, mglData *r, const char *text, const char *fnt="")
	{	mgl_textmark_xyzr(self, x, y, z, r, text, fnt);	};
	inline void TextMark(mglData *x, mglData *y, mglData *r, const char *text, const char *fnt="")
	{	mgl_textmark_xyr(self, x, y, r, text, fnt);	};
	inline void TextMark(mglData *y, mglData *r, const char *text, const char *fnt="")
	{	mgl_textmark_yr(self, y, r, text, fnt);	};
	inline void TextMark(mglData *y, const char *text, const char *fnt="")
	{	mgl_textmark(self, y, text, fnt);	};
	inline void Tube(mglData *x, mglData *y, mglData *z, mglData *r, const char *pen="")
	{	mgl_tube_xyzr(self, x, y, z, r, pen);	};
	inline void Tube(mglData *x, mglData *y, mglData *z, float r, const char *pen="")
	{	mgl_tube_xyz(self, x, y, z, r, pen);	};
	inline void Tube(mglData *x, mglData *y, mglData *r, const char *pen="")
	{	mgl_tube_xyr(self, x, y, r, pen);	};
	inline void Tube(mglData *x, mglData *y, float r, const char *pen="")
	{	mgl_tube_xy(self, x, y, r, pen);	};
	inline void Tube(mglData *y, mglData *r, const char *pen="")
	{	mgl_tube_r(self, y, r, pen);	};
	inline void Tube(mglData *y, float r, const char *pen="")
	{	mgl_tube(self, y, r, pen);	};

	inline void Surf(const char *fz, const char *stl="", int n=100)
	{	mgl_fsurf(self, fz, stl, n);	};
	inline void Surf(const char *fx, const char *fy, const char *fz, const char *stl="", int n=100)
	{	mgl_fsurf_xyz(self, fx, fy, fz, stl, n);	};
	inline void Grid(mglData *x, mglData *y, mglData *z, const char *stl="", float zVal=NAN)
	{	mgl_grid_xy(self, x, y, z, stl, zVal);	};
	inline void Grid(mglData *z, const char *stl="", float zVal=NAN)
	{	mgl_grid(self, z, stl, zVal);	};
	inline void Mesh(mglData *x, mglData *y, mglData *z, const char *stl="")
	{	mgl_mesh_xy(self, x, y, z, stl);	};
	inline void Mesh(mglData *z, const char *stl="")
	{	mgl_mesh(self, z, stl);	};
	inline void Fall(mglData *x, mglData *y, mglData *z, const char *stl="")
	{	mgl_fall_xy(self, x, y, z, stl);	};
	inline void Fall(mglData *z, const char *stl="")
	{	mgl_fall(self, z, stl);	};
	inline void Belt(mglData *x, mglData *y, mglData *z, const char *stl="")
	{	mgl_belt_xy(self, x, y, z, stl);	};
	inline void Belt(mglData *z, const char *stl="")
	{	mgl_belt(self, z, stl);	};
	inline void Surf(mglData *x, mglData *y, mglData *z, const char *stl="")
	{	mgl_surf_xy(self, x, y, z, stl);	};
	inline void Surf(mglData *z, const char *stl="")
	{	mgl_surf(self, z, stl);	};
	inline void Tile(mglData *x, mglData *y, mglData *z, const char *stl="")
	{	mgl_tile_xy(self, x, y, z, stl);	};
	inline void Tile(mglData *z, const char *stl="")
	{	mgl_tile(self, z, stl);	};
	inline void TileS(mglData *x, mglData *y, mglData *z, mglData *r, const char *stl="")
	{	mgl_tiles_xy(self, x, y, z, r, stl);	};
	inline void TileS(mglData *z, mglData *r, const char *stl="")
	{	mgl_tiles(self, z, r, stl);	};
	inline void Dens(mglData *x, mglData *y, mglData *z, const char *stl="", float zVal=NAN)
	{	mgl_dens_xy(self, x, y, z, stl, zVal);	};
	inline void Dens(mglData *z, const char *stl="", float zVal=NAN)
	{	mgl_dens(self, z, stl, zVal);	};
	inline void Boxs(mglData *x, mglData *y, mglData *z, const char *stl="", float zVal=NAN)
	{	mgl_boxs_xy(self, x, y, z, stl, zVal);	};
	inline void Boxs(mglData *z, const char *stl="", float zVal=NAN)
	{	mgl_boxs(self, z, stl, zVal);	};

	inline void Cont(mglData *v, mglData *x, mglData *y, mglData *z, const char *sch="", float zVal=NAN)
	{	mgl_cont_xy_val(self, v, x, y, z, sch, zVal);	};
	inline void Cont(mglData *v, mglData *z, const char *sch="", float zVal=NAN)
	{	mgl_cont_val(self, v, z, sch, zVal);	};
	inline void Cont(mglData *x, mglData *y, mglData *z, const char *sch="", int Num=7, float zVal=NAN)
	{	mgl_cont_xy(self, x, y, z, sch, Num, zVal);	};
	inline void Cont(mglData *z, const char *sch="", int Num=7, float zVal=NAN)
	{	mgl_cont(self, z, sch, Num, zVal);	};
	
	inline void ContF(mglData *v, mglData *x, mglData *y, mglData *z, const char *sch="", float zVal=NAN)
	{	mgl_contf_xy_val(self, v, x, y, z, sch, zVal);	};
	inline void ContF(mglData *v, mglData *z, const char *sch="", float zVal=NAN)
	{	mgl_contf_val(self, v, z, sch, zVal);	};
	inline void ContF(mglData *x, mglData *y, mglData *z, const char *sch="", int Num=7, float zVal=NAN)
	{	mgl_contf_xy(self, x, y, z, sch, Num, zVal);	};
	inline void ContF(mglData *z, const char *sch="", int Num=7, float zVal=NAN)
	{	mgl_contf(self, z, sch, Num, zVal);	};
	
	inline void ContD(mglData *v, mglData *x, mglData *y, mglData *z, const char *sch="", float zVal=NAN)
	{	mgl_contd_xy_val(self, v, x, y, z, sch, zVal);	};
	inline void ContD(mglData *v, mglData *z, const char *sch="", float zVal=NAN)
	{	mgl_contd_val(self, v, z, sch, zVal);	};
	inline void ContD(mglData *x, mglData *y, mglData *z, const char *sch="", int Num=7, float zVal=NAN)
	{	mgl_contd_xy(self, x, y, z, sch, Num, zVal);	};
	inline void ContD(mglData *z, const char *sch="", int Num=7, float zVal=NAN)
	{	mgl_contd(self, z, sch, Num, zVal);	};
	
	inline void Axial(mglData *v, mglData *x, mglData *y, mglData *z, const char *sch="")
	{	mgl_axial_xy_val(self, v, x, y, z, sch);	};
	inline void Axial(mglData *v, mglData *z, const char *sch="")
	{	mgl_axial_val(self, v, z, sch);	};
	inline void Axial(mglData *x, mglData *y, mglData *z, const char *sch="", int Num=3)
	{	mgl_axial_xy(self, x, y, z, sch, Num);	};
	inline void Axial(mglData *z, const char *sch="", int Num=3)
	{	mgl_axial(self, z, sch, Num);	};

	inline void SurfC(mglData *x, mglData *y, mglData *z, mglData *c, const char *sch="")
	{	mgl_surfc_xy(self, x, y, z, c, sch);	};
	inline void SurfC(mglData *z, mglData *c, const char *sch="")
	{	mgl_surfc(self, z, c, sch);	};
	inline void SurfA(mglData *x, mglData *y, mglData *z, mglData *c, const char *sch="")
	{	mgl_surfa_xy(self, x, y, z, c, sch);	};
	inline void SurfA(mglData *z, mglData *c, const char *sch="")
	{	mgl_surfa(self, z, c, sch);	};
	inline void STFA(mglData *x, mglData *y, mglData *re, mglData *im, int dn, const char *sch="", float zVal=NAN)
	{	mgl_stfa_xy(self, x, y, re, im, dn, sch, zVal);	};
	inline void STFA(mglData *re, mglData *im, int dn, const char *sch="", float zVal=NAN)
	{	mgl_stfa(self, re, im, dn, sch, zVal);	};

	inline void Dew(mglData *x, mglData *y, mglData *ax, mglData *ay, const char *sch="", float zVal=NAN)
	{	mgl_dew_xy(self, x, y, ax, ay, sch, zVal);	};
	inline void Dew(mglData *ax, mglData *ay, const char *sch="", float zVal=NAN)
	{	mgl_dew_2d(self, ax, ay, sch, zVal);	};
	inline void Traj(mglData *x, mglData *y, mglData *ax, mglData *ay, const char *sch="", float zVal=NAN, float len=0)
	{	mgl_traj_xy(self, x, y, ax, ay, sch, zVal, len);	};
	inline void Traj(mglData *x, mglData *y, mglData *z, mglData *ax, mglData *ay, mglData *az, const char *sch="", float len=0)
	{	mgl_traj_xyz(self, x, y, z, ax, ay, az, sch, len);	};
	inline void Vect(mglData *x, mglData *y, mglData *ax, mglData *ay, const char *sch="", float zVal=NAN, int flag=0)
	{	mgl_vect_xy(self, x, y, ax, ay, sch, zVal, flag);	};
	inline void Vect(mglData *ax, mglData *ay, const char *sch="", float zVal=NAN, int flag=0)
	{	mgl_vect_2d(self, ax, ay, sch, zVal, flag);	};
	inline void VectL(mglData *x, mglData *y, mglData *ax, mglData *ay, const char *sch="", float zVal=NAN)
	{	mgl_vectl_xy(self, x, y, ax, ay, sch, zVal);	};
	inline void VectL(mglData *ax, mglData *ay, const char *sch="", float zVal=NAN)
	{	mgl_vectl_2d(self, ax, ay, sch, zVal);	};
	inline void VectC(mglData *x, mglData *y, mglData *ax, mglData *ay, const char *sch="", float zVal=NAN)
	{	mgl_vectc_xy(self, x, y, ax, ay, sch, zVal);	};
	inline void VectC(mglData *ax, mglData *ay, const char *sch="", float zVal=NAN)
	{	mgl_vectc_2d(self, ax, ay, sch, zVal);	};
	inline void Vect(mglData *x, mglData *y, mglData *z, mglData *ax, mglData *ay, mglData *az, const char *sch="", int flag=0)
	{	mgl_vect_xyz(self, x, y, z, ax, ay, az, sch, flag);	};
	inline void Vect(mglData *ax, mglData *ay, mglData *az, const char *sch="", int flag=0)
	{	mgl_vect_3d(self, ax, ay, az, sch, flag);	};
	inline void VectL(mglData *x, mglData *y, mglData *z, mglData *ax, mglData *ay, mglData *az, const char *sch="")
	{	mgl_vectl_xyz(self, x, y, z, ax, ay, az, sch);	};
	inline void VectL(mglData *ax, mglData *ay, mglData *az, const char *sch="")
	{	mgl_vectl_3d(self, ax, ay, az, sch);	};
	inline void VectC(mglData *x, mglData *y, mglData *z, mglData *ax, mglData *ay, mglData *az, const char *sch="")
	{	mgl_vectc_xyz(self, x, y, z, ax, ay, az, sch);	};
	inline void VectC(mglData *ax, mglData *ay, mglData *az, const char *sch="")
	{	mgl_vectc_3d(self, ax, ay, az, sch);	};
	inline void Map(mglData *x, mglData *y, mglData *a, mglData *b, const char *sch="", int ks=0, bool pnts=true)
	{	mgl_map_xy(self, x, y, a, b, sch, ks, pnts);	};
	inline void Map(mglData *a, mglData *b, const char *sch="", int ks=0, bool pnts=true)
	{	mgl_map(self, a, b, sch, ks, pnts);	};

	inline void Surf3A(float Val, mglData *x, mglData *y, mglData *z, mglData *a, mglData *b, const char *stl="")
	{	mgl_surf3a_xyz_val(self, Val, x, y, z, a, b, stl);	};
	inline void Surf3A(float Val, mglData *a, mglData *b, const char *stl="")
	{	mgl_surf3a_val(self, Val, a, b, stl);	};
	inline void Surf3A(mglData *x, mglData *y, mglData *z, mglData *a, mglData *b, const char *stl="", int num=3)
	{	mgl_surf3a_xyz(self, x, y, z, a, b, stl, num);	};
	inline void Surf3A(mglData *a, mglData *b, const char *stl="", int num=3)
	{	mgl_surf3a(self, a, b, stl, num);	};
	inline void Surf3C(float Val, mglData *x, mglData *y, mglData *z, mglData *a, mglData *b, const char *stl="")
	{	mgl_surf3c_xyz_val(self, Val, x, y, z, a, b, stl);	};
	inline void Surf3C(float Val, mglData *a, mglData *b, const char *stl="")
	{	mgl_surf3c_val(self, Val, a, b, stl);	};
	inline void Surf3C(mglData *x, mglData *y, mglData *z, mglData *a, mglData *b, const char *stl="", int num=3)
	{	mgl_surf3c_xyz(self, x, y, z, a, b, stl, num);	};
	inline void Surf3C(mglData *a, mglData *b, const char *stl="", int num=3)
	{	mgl_surf3c(self, a, b, stl, num);	};

	inline void Flow(mglData *x, mglData *y, mglData *ax, mglData *ay, const char *sch="", int num=5, bool central=true, float zVal=NAN)
	{	mgl_flow_xy(self, x, y, ax, ay, sch, num, central, zVal);	};
	inline void Flow(mglData *ax, mglData *ay, const char *sch="", int num=5, bool central=true, float zVal=NAN)
	{	mgl_flow_2d(self, ax, ay, sch, num, central, zVal);	};
	inline void Flow(mglData *x, mglData *y, mglData *z, mglData *ax, mglData *ay, mglData *az, const char *sch="", int num=3, bool central=true)
	{	mgl_flow_xyz(self, x, y, z, ax, ay, az, sch, num, central);	};
	inline void Flow(mglData *ax, mglData *ay, mglData *az, const char *sch="", int num=3, bool central=true)
	{	mgl_flow_3d(self, ax, ay, az, sch, num, central);	};

	inline void FlowP(float x0, float y0, float z0, mglData *x, mglData *y, mglData *ax, mglData *ay, const char *sch="")
	{	mgl_flowp_xy(self, x0, y0, z0, x, y, ax, ay, sch);	};
	inline void FlowP(float x0, float y0, float z0, mglData *ax, mglData *ay, const char *sch="")
	{	mgl_flowp_2d(self, x0, y0, z0, ax, ay, sch);	};
	inline void FlowP(float x0, float y0, float z0, mglData *x, mglData *y, mglData *z, mglData *ax, mglData *ay, mglData *az, const char *sch="")
	{	mgl_flowp_xyz(self, x0, y0, z0, x, y, z, ax, ay, az, sch);	};
	inline void FlowP(float x0, float y0, float z0, mglData *ax, mglData *ay, mglData *az, const char *sch="")
	{	mgl_flowp_3d(self, x0, y0, z0, ax, ay, az, sch);	};

	inline void Pipe(mglData *x, mglData *y, mglData *ax, mglData *ay, const char *sch="", float r0=0.05, int num=5, bool central=true, float zVal=NAN)
	{	mgl_pipe_xy(self, x, y, ax, ay, sch, r0, num, central, zVal);	};
	inline void Pipe(mglData *ax, mglData *ay, const char *sch="", float r0=0.05, int num=5, bool central=true, float zVal=NAN)
	{	mgl_pipe_2d(self, ax, ay, sch, r0, num, central, zVal);	};
	inline void Pipe(mglData *x, mglData *y, mglData *z, mglData *ax, mglData *ay, mglData *az, const char *sch="", float r0=0.05, int num=3, bool central=true)
	{	mgl_pipe_xyz(self, x, y, z, ax, ay, az, sch, r0, num, central);	};
	inline void Pipe(mglData *ax, mglData *ay, mglData *az, const char *sch="", float r0=0.05, int num=3, bool central=true)
	{	mgl_pipe_3d(self, ax, ay, az, sch, r0, num, central);	};

	inline void Grid3(mglData *x, mglData *y, mglData *z, mglData *a, char dir, int sVal, const char *stl="")
	{	mgl_grid3_xyz(self, x, y, z, a, dir, sVal, stl);	};
	inline void Grid3(mglData *a, char dir, int sVal, const char *stl="")
	{	mgl_grid3(self, a, dir, sVal, stl);	};
	inline void GridA(mglData *x, mglData *y, mglData *z, mglData *a, const char *stl="")
	{	mgl_grid3_all_xyz(self, x, y, z, a, stl);	};
	inline void GridA(mglData *a, const char *stl="")
	{	mgl_grid3_all(self, a, stl);	};
	inline void Dens3(mglData *x, mglData *y, mglData *z, mglData *a, char dir, int sVal, const char *stl="")
	{	mgl_dens3_xyz(self, x, y, z, a, dir, sVal, stl);	};
	inline void Dens3(mglData *a, char dir, int sVal, const char *stl="")
	{	mgl_dens3(self, a, dir, sVal, stl);	};
	inline void DensA(mglData *x, mglData *y, mglData *z, mglData *a, const char *stl="")
	{	mgl_dens3_all_xyz(self, x, y, z, a, stl);	};
	inline void DensA(mglData *a, const char *stl="")
	{	mgl_dens3_all(self, a, stl);	};

	inline void Surf3(float Val, mglData *x, mglData *y, mglData *z, mglData *a, const char *stl="")
	{	mgl_surf3_xyz_val(self, Val, x, y, z, a, stl);	};
	inline void Surf3(float Val, mglData *a, const char *stl="")
	{	mgl_surf3_val(self, Val, a, stl);	};
	inline void Surf3(mglData *x, mglData *y, mglData *z, mglData *a, const char *stl="", int num=3)
	{	mgl_surf3_xyz(self, x, y, z, a, stl, num);	};
	inline void Surf3(mglData *a, const char *stl="", int num=3)
	{	mgl_surf3(self, a, stl, num);	};
	inline void Cloud(mglData *x, mglData *y, mglData *z, mglData *a, const char *stl="", float alpha=1)
	{	mgl_cloud_xyz(self, x, y, z, a, stl, alpha);	};
	inline void Cloud(mglData *a, const char *stl="", float alpha=1)
	{	mgl_cloud(self, a, stl, alpha);	};

	inline void Cont3(mglData *v, mglData *x, mglData *y, mglData *z, mglData *a, char dir, int sVal, const char *sch="")
	{	mgl_cont3_xyz_val(self, v, x, y, z, a, dir, sVal, sch);	};
	inline void Cont3(mglData *v, mglData *a, char dir, int sVal, const char *sch="")
	{	mgl_cont3_val(self, v, a, dir, sVal, sch);	};
	inline void Cont3(mglData *x, mglData *y, mglData *z, mglData *a, char dir, int sVal, const char *sch="", int Num=7)
	{	mgl_cont3_xyz(self, x, y, z, a, dir, sVal, sch, Num);	};
	inline void Cont3(mglData *a, char dir, int sVal, const char *sch="", int Num=7)
	{	mgl_cont3(self, a, dir, sVal, sch, Num);	};
	inline void ContA(mglData *x, mglData *y, mglData *z, mglData *a, const char *sch="", int Num=7)
	{	mgl_cont_all_xyz(self, x, y, z, a, sch, Num);	};
	inline void ContA(mglData *a, const char *sch="", int Num=7)
	{	mgl_cont_all(self, a, sch, Num);	};

	inline void ContF3(mglData *v, mglData *x, mglData *y, mglData *z, mglData *a, char dir, int sVal, const char *sch="")
	{	mgl_contf3_xyz_val(self, v, x, y, z, a, dir, sVal, sch);	};
	inline void ContF3(mglData *v, mglData *a, char dir, int sVal, const char *sch="")
	{	mgl_contf3_val(self, v, a, dir, sVal, sch);	};
	inline void ContF3(mglData *x, mglData *y, mglData *z, mglData *a, char dir, int sVal, const char *sch="", int Num=7)
	{	mgl_contf3_xyz(self, x, y, z, a, dir, sVal, sch, Num);	};
	inline void ContF3(mglData *a, char dir, int sVal, const char *sch="", int Num=7)
	{	mgl_contf3(self, a, dir, sVal, sch, Num);	};
	inline void ContFA(mglData *x, mglData *y, mglData *z, mglData *a, const char *sch="", int Num=7)
	{	mgl_contf_all_xyz(self, x, y, z, a, sch, Num);	};
	inline void ContFA(mglData *a, const char *sch="", int Num=7)
	{	mgl_contf_all(self, a, sch, Num);	};

	inline void Beam(mglData *tr, mglData *g1, mglData *g2, mglData *a, float r, const char *stl=0, int flag=0, int num=3)
	{	mgl_beam(self, tr,g1,g2,a,r,stl,flag,num);	};
	inline void Beam(float val, mglData *tr, mglData *g1, mglData *g2, mglData *a, float r, const char *stl=NULL, int flag=0)
	{	mgl_beam_val(self,val,tr,g1,g2,a,r,stl,flag);	};


	inline void TriPlot(mglData *nums, mglData *x, mglData *y, mglData *z, mglData *c, const char *sch="")
	{	mgl_triplot_xyzc(self, nums, x, y, z, c, sch);	};
	inline void TriPlot(mglData *nums, mglData *x, mglData *y, mglData *z, const char *sch="")
	{	mgl_triplot_xyz(self, nums, x, y, z, sch);	};
	inline void TriPlot(mglData *nums, mglData *x, mglData *y, const char *sch="", float zVal=NAN)
	{	mgl_triplot_xy(self, nums, x, y, sch, zVal);	};
	inline void Dots(mglData *x, mglData *y, mglData *z, const char *sch="")
	{	mgl_dots(self, x, y, z, sch);	};
	inline void Dots(mglData *x, mglData *y, mglData *z, mglData *a, const char *sch="")
	{	mgl_dots_a(self, x, y, z, a, sch);	};
	inline void Crust(mglData *x, mglData *y, mglData *z, const char *sch="", float er=0)
	{	mgl_crust(self, x, y, z, sch, er);	};

	inline void DensX(mglData *a, const char *stl="", float sVal=NAN)
	{	mgl_dens_x(self, a, stl, sVal);	};
	inline void DensY(mglData *a, const char *stl="", float sVal=NAN)
	{	mgl_dens_y(self, a, stl, sVal);	};
	inline void DensZ(mglData *a, const char *stl="", float sVal=NAN)
	{	mgl_dens_z(self, a, stl, sVal);	};
	inline void ContX(mglData *a, const char *stl="", float sVal=NAN, int Num=7)
	{	mgl_cont_x(self, a, stl, sVal, Num);	};
	inline void ContY(mglData *a, const char *stl="", float sVal=NAN, int Num=7)
	{	mgl_cont_y(self, a, stl, sVal, Num);	};
	inline void ContZ(mglData *a, const char *stl="", float sVal=NAN, int Num=7)
	{	mgl_cont_z(self, a, stl, sVal, Num);	};
	inline void ContX(mglData *v, mglData *a, const char *stl="", float sVal=NAN)
	{	mgl_cont_x_val(self, v, a, stl, sVal);	};
	inline void ContY(mglData *v, mglData *a, const char *stl="", float sVal=NAN)
	{	mgl_cont_y_val(self, v, a, stl, sVal);	};
	inline void ContZ(mglData *v, mglData *a, const char *stl="", float sVal=NAN)
	{	mgl_cont_z_val(self, v, a, stl, sVal);	};
	inline void ContFX(mglData *a, const char *stl="", float sVal=NAN, int Num=7)
	{	mgl_contf_x(self, a, stl, sVal, Num);	};
	inline void ContFY(mglData *a, const char *stl="", float sVal=NAN, int Num=7)
	{	mgl_contf_y(self, a, stl, sVal, Num);	};
	inline void ContFZ(mglData *a, const char *stl="", float sVal=NAN, int Num=7)
	{	mgl_contf_z(self, a, stl, sVal, Num);	};
	inline void ContFX(mglData *v, mglData *a, const char *stl="", float sVal=NAN)
	{	mgl_contf_x_val(self, v, a, stl, sVal);	};
	inline void ContFY(mglData *v, mglData *a, const char *stl="", float sVal=NAN)
	{	mgl_contf_y_val(self, v, a, stl, sVal);	};
	inline void ContFZ(mglData *v, mglData *a, const char *stl="", float sVal=NAN)
	{	mgl_contf_z_val(self, v, a, stl, sVal);	};

	inline float Fit(mglData *fit, mglData *y, const char *eq, const char *var)
	{	return mgl_fit_1(self, fit, y,eq,var,0);	};
	inline float Fit(mglData *fit, mglData *y, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_1_d(self, fit, y,eq,var,ini);	};
	inline float Fit2(mglData *fit, mglData *z, const char *eq, const char *var)
	{	return mgl_fit_2(self, fit, z,eq,var,0);	};
	inline float Fit2(mglData *fit, mglData *z, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_2_d(self, fit, z,eq,var,ini);	};
	inline float Fit3(mglData *fit, mglData *a, const char *eq, const char *var)
	{	return mgl_fit_3(self, fit, a,eq,var,0);	};
	inline float Fit3(mglData *fit, mglData *a, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_3_d(self, fit, a,eq,var,ini);	};
	inline float Fit(mglData *fit, mglData *x, mglData *y, const char *eq, const char *var)
	{	return mgl_fit_xy(self, fit, x,y,eq,var,0);	};
	inline float Fit(mglData *fit, mglData *x, mglData *y, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_xy_d(self, fit, x,y,eq,var,ini);	};
	inline float Fit(mglData *fit, mglData *x, mglData *y, mglData *z, const char *eq, const char *var)
	{	return mgl_fit_xyz(self, fit, x,y,z,eq,var,0);	};
	inline float Fit(mglData *fit, mglData *x, mglData *y, mglData *z, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_xyz_d(self, fit, x,y,z,eq,var,ini);	};
	inline float Fit(mglData *fit, mglData *x, mglData *y, mglData *z, mglData *a, const char *eq, const char *var)
	{	return mgl_fit_xyza(self, fit, x,y,z,a,eq,var,0);	};
	inline float Fit(mglData *fit, mglData *x, mglData *y, mglData *z, mglData *a, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_xyza_d(self, fit, x,y,z,a,eq,var,ini);	};
	inline float FitS(mglData *fit, mglData *y, mglData *s, const char *eq, const char *var)
	{	return mgl_fit_ys(self, fit, y,s,eq,var,0);	};
	inline float FitS(mglData *fit, mglData *y, mglData *s, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_ys_d(self, fit, y,s,eq,var,ini);	};
	inline float FitS(mglData *fit, mglData *x, mglData *y, mglData *s, const char *eq, const char *var)
	{	return mgl_fit_xys(self, fit, x,y,s,eq,var,0);	};
	inline float FitS(mglData *fit, mglData *x, mglData *y, mglData *s, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_xys_d(self, fit, x,y,s,eq,var,ini);	};
	inline float FitS(mglData *fit, mglData *x, mglData *y, mglData *z, mglData *s, const char *eq, const char *var)
	{	return mgl_fit_xyzs(self, fit, x,y,z,s,eq,var,0);	};
	inline float FitS(mglData *fit, mglData *x, mglData *y, mglData *z, mglData *s, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_xyzs_d(self, fit, x,y,z,s,eq,var,ini);	};
	inline float FitS(mglData *fit, mglData *x, mglData *y, mglData *z, mglData *a, mglData *s, const char *eq, const char *var)
	{	return mgl_fit_xyzas(self, fit, x,y,z,a,s,eq,var,0);	};
	inline float FitS(mglData *fit, mglData *x, mglData *y, mglData *z, mglData *a, mglData *s, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_xyzas_d(self, fit, x,y,z,a,s,eq,var,ini);	};
	inline void PutsFit(float x, float y, float z, const char *prefix=0, const char *font=0, float size=-1)
	{	mgl_puts_fit(self, x,y,z, prefix, font, size);	};

	inline mglData *PDE(const char *ham, mglData *ini_re, mglData *ini_im, float dz=0.1, float k0=100)
	{	return mgl_pde_solve(self,ham,ini_re,ini_im,dz,k0);	};
	inline void Fill(mglData *u, const char *eq, mglData *v=NULL, mglData *w=NULL)
	{	mgl_data_fill_eq(self, u,eq,v,w);	};
};;
//-----------------------------------------------------------------------------
class mglParse
{
public:
	HMPR self;
	mglParse()
	{	self=mgl_create_parser();	};
	~mglParse()
	{	mgl_delete_parser(self);	};
	inline void AddParam(int id, const char *str)	{	mgl_add_param(self, id, str);	};
	inline void AddParam(int id, const wchar_t *str){	mgl_add_paramw(self, id, str);	};
	inline const mglData *AddVar(const char *name)	{	return mgl_add_var(self, name);	};
	inline const mglData *FindVar(const char *name)	{	return mgl_find_var(self, name);};
	inline int Parse(mglGraph *gr, const char *str, int pos)
	{	return mgl_parse(gr->self, self, str, pos);	};
	inline int Parse(mglGraph *gr, const wchar_t *str, int pos)
	{	return mgl_parsew(gr->self, self, str, pos);	};
	inline void Execute(mglGraph *gr, const char *str)
	{	mgl_parse_text(gr->self, self, str);	};
	inline void Execute(mglGraph *gr, const wchar_t *str)
	{	mgl_parsew_text(gr->self, self, str);	};
	inline void RestoreOnce()	{	mgl_restore_once(self);	};
	inline void AllowSetSize(bool allow)	{	mgl_parser_allow_setsize(self, allow);	};
};;
//-----------------------------------------------------------------------------
#else
#warning "MathGL: Wrapper is disabled -- you've already included normal mglGraph class"
#endif
//-----------------------------------------------------------------------------
