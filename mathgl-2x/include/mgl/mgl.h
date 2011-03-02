/***************************************************************************
 * mgl.h is part of Math Graphic Library
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
#ifndef _MGL_H_
#define _MGL_H_

#include "mgl/mgl_cf.h"
#include "mgl/data.h"
//-----------------------------------------------------------------------------
const float NaN = NAN;
const float pi = M_PI;
//-----------------------------------------------------------------------------
/// Wrapper class for all graphics
class mglGraph
{
	HMGL gr;
public:
	mglGraph(int kind=0, int width=600, int height=400)
	{
		if(kind==1)		gr=mgl_create_graph_gl();
		else if(kind==2)gr=mgl_create_graph_idtf();
		else	gr=mgl_create_graph(width, height);
	}
	~mglGraph()	{	mgl_delete_graph(gr);	}
	inline HMGL Self()	{	return gr;	}

	inline int  GetWarn()			{	return mgl_get_warn(gr);}
	inline void DefaultPlotParam()	{	mgl_set_def_param(gr);	}
	inline void SetPalColor(int n, float r, float g, float b)
	{	if(n>=0 && n<100)	mgl_set_pal_color(gr, n, r, g, b);	}
	inline void SetPalNum(int n)	{	if(n>=0 && n<100)	mgl_set_pal_num(gr, n);	}
	inline void SetPalette(const char *colors)	{	mgl_set_palette(gr, colors);	}
	inline void SetRotatedText(bool rotated)	{	mgl_set_rotated_text(gr, rotated);	}
	inline void SetCut(bool cut)				{	mgl_set_cut(gr, cut);	}
	inline void SetCutBox(float x1,float y1,float z1,float x2,float y2,float z2)
	{	mgl_set_cut_box(gr, x1, y1, z1, x2, y2, z2);	}
	inline void SetTickLen(float len, float stt=1)
	{	mgl_set_tick_len(gr, len, stt);	}
	inline void SetTickStl(const char *stl, const char *sub=0)
	{	mgl_set_tick_stl(gr, stl, sub);	}
	inline void SetSize(int width, int height)	{	mgl_set_size(gr, width, height);	}
	inline void SetBarWidth(float width)	{	mgl_set_bar_width(gr, width);	}
	inline void SetMarkSize(float size)		{	mgl_set_mark_size(gr, size);	}
	inline void SetArrowSize(float size)	{	mgl_set_arrow_size(gr, size);	}
	inline void SetFontSize(float size)		{	mgl_set_font_size(gr, size);	}
	inline void SetFontDef(char *fnt)		{	mgl_set_font_def(gr, fnt);	}
	inline void SetAlphaDef(float alpha)	{	mgl_set_alpha_default(gr, alpha);	}
	inline void SetMeshNum(int num)			{	mgl_set_meshnum(gr, num);		}
	inline void Zoom(float x1, float y1, float x2, float y2)
	{	mgl_set_zoom(gr, x1, y1, x2, y2);	}
	inline void SetLegendBox(bool enable)	{	mgl_set_legend_box(gr, enable);	}
	inline void SetLegendMarks(int num)		{	mgl_set_legend_marks(gr, num);	}
	inline void LoadFont(const char *name, const char *path=NULL)
	{	mgl_load_font(gr, name, path);	}
	inline void CopyFont(mglGraph *GR)		{	mgl_copy_font(gr, GR->Self());}
	inline void RestoreFont()				{	mgl_restore_font(gr);	}

	inline void ShowImage(const char *viewer=0, bool keep=0)
	{	mgl_show_image(gr, viewer, keep);	}
	inline void WriteFrame(const char *fname,const char *descr="")
	{	mgl_write_frame(gr, fname, descr);	}
	inline void WriteIDTF(const char *fname,const char *descr="")
	{	mgl_write_idtf(gr, fname, descr);	}
	inline void WriteJPEG(const char *fname,const char *descr="")
	{	mgl_write_jpg(gr, fname, descr);	}
	inline void WritePNG(const char *fname,const char *descr="", bool alpha=true)
	{	if(alpha)	mgl_write_png(gr, fname, descr);
		else	mgl_write_png_solid(gr, fname, descr);	}
	inline void WriteEPS(const char *fname,const char *descr="")
	{	mgl_write_eps(gr, fname, descr);	}
	inline void WriteSVG(const char *fname,const char *descr="")
	{	mgl_write_svg(gr, fname, descr);	}
	inline void WriteGIF(const char *fname,const char *descr="")
	{	mgl_write_gif(gr, fname, descr);	}
	inline void StartGIF(const char *fname, int ms=100)
	{	mgl_start_gif(gr, fname,ms);	}
	inline void CloseGIF()		{	mgl_close_gif(gr);	}
	inline void NewFrame()		{	mgl_new_frame(gr);	}
	inline void EndFrame()		{	mgl_end_frame(gr);	}
	inline int GetNumFrame()	{	return mgl_get_num_frame(gr);	}
	inline void ResetFrames()	{	mgl_reset_frames(gr);	}

	inline void GetRGB(char *imgdata, int imglen)
	{
		int w=mgl_get_width(gr);
		int h=mgl_get_height(gr);
		if(imglen>=3*w*h)
		{
			imglen=3*w*h;
			memcpy(imgdata, mgl_get_rgb(gr),imglen);
		}
	}
	inline void GetRGBA(char *imgdata, int imglen)
	{
		int w=mgl_get_width(gr);
		int h=mgl_get_height(gr);
		if(imglen>=4*w*h)
		{
			imglen=4*w*h;
			memcpy(imgdata, mgl_get_rgba(gr),imglen);
		}
	}
	inline void GetBGRN(char *imgdata, int imglen)
	{
		int w,h,i;
		w=mgl_get_width(gr);
		h=mgl_get_height(gr);
		const char *buf=(const char *)mgl_get_rgb(gr);
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
	}
	inline int GetWidth()	{	return mgl_get_width(gr);	}
	inline int GetHeight()	{	return mgl_get_height(gr);}
	inline mglPoint CalcXYZ(int xs, int ys)
	{
		mreal x,y,z;
		mgl_calc_xyz(gr,xs,ys,&x,&y,&z);
		return mglPoint(x,y,z);
	}
	inline mglPoint CalcScr(mglPoint p)
	{
		int xs,ys;
		mgl_calc_scr(gr,p.x,p.y,p.z,&xs,&ys);
		return mglPoint(xs,ys);
	}

	inline void SetTranspType(int type)		{	mgl_set_transp_type(gr, type);}
	inline void Alpha(bool enable)			{	mgl_set_alpha(gr, enable);	}
	inline void Fog(float d, float dz=0.25)	{	mgl_set_fog(gr, d, dz);		}
	inline void Light(bool enable)			{	mgl_set_light(gr, enable);	}
	inline void Light(int n,bool enable)	{	mgl_set_light_n(gr, n, enable);	}
	inline void AddLight(int n, float x, float y, float z, char c='w')
	{	mgl_add_light(gr, n, x, y, z, c);	}
	inline void SetAmbient(float i)			{	mgl_set_ambbr(gr, i);	}

	inline void Identity(bool rel=false)	{	mgl_identity(gr, rel);	}
	inline void Clf(float r=1, float g=1, float b=1)
	{	mgl_clf_rgb(gr, r, g, b);	}
	inline void SubPlot(int nx,int ny,int m, float dx=0, float dy=0)
	{	mgl_subplot_d(gr, nx, ny, m, dx, dy);	}
	inline void SubPlot(int nx,int ny,int m, const char *style)
	{	mgl_subplot_s(gr, nx, ny, m, style);	}
	inline void InPlot(float x1,float x2,float y1,float y2, bool rel=true)
	{	if(rel)	mgl_inplot(gr, x1, x2, y1, y2);
		else	mgl_relplot(gr, x1, x2, y1, y2);	}
	inline void ColumnPlot(int num, int ind, float d=0)
	{	mgl_columnplot_d(gr,num,ind,d);	}
	inline void StickPlot(int num, int i, float tet, float phi)
	{	mgl_stickplot(gr,num,i,tet,phi);	}
	inline void Aspect(float Ax,float Ay,float Az)
	{	mgl_aspect(gr, Ax, Ay, Az);		}
	inline void Rotate(float TetX,float TetZ=0,float TetY=0)
	{	mgl_rotate(gr, TetX, TetZ, TetY);	}
	inline void RotateN(float Tet,float x,float y,float z)
	{	mgl_rotate_vector(gr, Tet, x, y, z);	}
	inline void Perspective(float val)
	{	mgl_perspective(gr, val);	}

	inline void SetTicks(char dir, float d=-5, int ns=0, float org=NAN)
	{	mgl_set_ticks(gr, dir, d, ns, org);	}
	inline void AdjustTicks(const char *dir="xyzc")
	{	mgl_adjust_ticks(gr, dir);	}

	inline void SetCRange(float c1, float c2)
	{	mgl_set_crange_val(gr, c1, c2);	}
	inline void SetRanges(float x1, float x2, float y1, float y2, float z1=0, float z2=0)
	{	mgl_set_ranges(gr, x1, x2, y1, y2, z1, z2);	}
	inline void SetAutoRanges(float x1, float x2, float y1=0, float y2=0, float z1=0, float z2=0)
	{	mgl_set_auto(gr, x1, x2, y1, y2, z1, z2);	}
	inline void SetOrigin(float x0, float y0, float z0=NAN)
	{	mgl_set_origin(gr, x0, y0, z0);	}
	inline void SetCRange(mglData &dat, bool add=false)
	{	mgl_set_crange(gr, &dat, add);	}
	inline void SetXRange(mglData &dat, bool add=false)
	{	mgl_set_xrange(gr, &dat, add);	}
	inline void SetYRange(mglData &dat, bool add=false)
	{	mgl_set_yrange(gr, &dat, add);	}
	inline void SetZRange(mglData &dat, bool add=false)
	{	mgl_set_zrange(gr, &dat, add);	}
	inline void SetFunc(const char *EqX, const char *EqY, const char *EqZ=NULL, const char *EqA=NULL)
	{	mgl_set_func_ext(gr, EqX, EqY, EqZ,EqA);	}
	inline void SetCoor(int how)		{	mgl_set_coor(gr, how);	}
	inline void Ternary(bool val)		{	mgl_set_ternary(gr, val);	}
	inline void CutOff(const char *EqC)	{	mgl_set_cutoff(gr, EqC);	}

	inline void Box(const char *col="k", bool ticks=true)
	{	mgl_box_str(gr, col, ticks);	}
	inline void Axis(const char *dir="xyzt", bool adjust=false)
	{	if(adjust)	mgl_adjust_ticks(gr,dir);	mgl_axis(gr, dir);	}
	inline void Grid(const char *dir="xyzt",const char *pen="B")
	{	mgl_axis_grid(gr, dir, pen);	}
	inline void Label(char dir, const char *text, float pos=+1, float size=-1.4, float shift=0)
	{	mgl_label_ext(gr, dir, text, pos, size, shift);	}
	inline void Label(char dir, const wchar_t *text, float pos=0, float size=-1.4, float shift=0)
	{	mgl_labelw_ext(gr, dir, text, pos, size, shift);	}
	void Label(double x, double y, const char *text, const char *fnt=0, double size=-1.4)
	{	mgl_label_xy(gr,x,y,text,fnt,size);	}
	void Label(double x, double y, const wchar_t *text, const char *fnt=0, double size=-1.4)
	{	mgl_labelw_xy(gr,x,y,text,fnt,size);	}
	inline void SetTuneTicks(int tune, float fact_pos=1.15)
	{	mgl_tune_ticks(gr, tune, fact_pos);	}

	inline void SetXTT(const wchar_t *templ)	{	mgl_set_xttw(gr, templ);	}
	inline void SetYTT(const wchar_t *templ)	{	mgl_set_yttw(gr, templ);	}
	inline void SetZTT(const wchar_t *templ)	{	mgl_set_zttw(gr, templ);	}
	inline void SetCTT(const wchar_t *templ)	{	mgl_set_cttw(gr, templ);	}
	inline void SetXTT(const char *templ)		{	mgl_set_xtt(gr, templ);	}
	inline void SetYTT(const char *templ)		{	mgl_set_ytt(gr, templ);	}
	inline void SetZTT(const char *templ)		{	mgl_set_ztt(gr, templ);	}
	inline void SetCTT(const char *templ)		{	mgl_set_ctt(gr, templ);	}

	inline void Ball(float x, float y, float z, char col='r')
	{	mgl_ball_str(gr, x, y, z, col);	}
	inline void Mark(float x, float y, float z, const char *mark)
	{	mgl_mark(gr, x, y, z, mark);	}
	inline void Line(float x1, float y1, float z1, float x2, float y2, float z2, const char *pen="B",int n=2)
	{	mgl_line(gr, x1, y1, z1, x2, y2, z2, pen, n);	}
	inline void FaceX(float x0, float y0, float z0, float wy, float wz, const char *stl="w", float dx=0, float dy=0)
	{	mgl_facex(gr, x0, y0, z0, wy, wz, stl, dx, dy);	}
	inline void FaceY(float x0, float y0, float z0, float wx, float wz, const char *stl="w", float dx=0, float dy=0)
	{	mgl_facey(gr, x0, y0, z0, wx, wz, stl, dx, dy);	}
	inline void FaceZ(float x0, float y0, float z0, float wx, float wy, const char *stl="w", float dx=0, float dy=0)
	{	mgl_facez(gr, x0, y0, z0, wx, wy, stl, dx, dy);	}
	inline void Curve(float x1, float y1, float z1, float dx1, float dy1, float dz1, float x2, float y2, float z2, float dx2, float dy2, float dz2, const char *pen="B", int n=100)
	{	mgl_curve(gr, x1, y1, z1, dx1, dy1, dz1, x2, y2, z2, dx2, dy2, dz2, pen, n);	}
	inline void Drop(float x0, float y0, float z0, float dx, float dy, float dz, float r, const char *col="r", float shift=1, float ap=1)
	{	mgl_drop(gr, x0, y0, z0, dx, dy, dz, r, col, shift, ap);	}
	inline void Sphere(float x0, float y0, float z0, float r, const char *col="r")
	{	mgl_sphere(gr, x0, y0, z0, r, col);	}
	inline void Cone (float x1, float y1, float z1, float x2, float y2, float z2, float r1, float r2=-1, const char *stl="B", bool edge=false)
	{	mgl_cone(gr, x1,y1,z1,x2,y2,z2,r1,r2,stl,edge);	}

	inline void Putsw(float x, float y, float z,const wchar_t *text,const char *font="C",float size=-1,char dir=0)
	{	mgl_putsw_ext(gr, x, y, z, text, font, size, dir);	}
	inline void Puts(float x, float y, float z,const char *text,const char *font="C",float size=-1,char dir=0)
	{	mgl_puts_ext(gr, x, y, z, text, font, size, dir);	}
	inline void Putsw(float x, float y, float z, float dx, float dy, float dz, const wchar_t *text, float size=-1)
	{	mgl_putsw_dir(gr, x, y, z, dx, dy, dz, text, size);	}
	inline void Puts(float x, float y, float z, float dx, float dy, float dz, const char *text,float size=-1)
	{	mgl_puts_dir(gr, x, y, z, dx, dy, dz, text, size);	}
	inline void Title(const char *text, const char *font="C", float size=-2)
	{	mgl_title(gr, text, font, size);	}
	inline void Title(const wchar_t *text, const char *font="C", float size=-2)
	{	mgl_titlew(gr, text, font, size);	}

	inline void Colorbar(const char *sch="",int where=0)
	{	mgl_colorbar(gr, sch, where);	}
	inline void Colorbar(const char *sch,int where,float x,float y,float w,float h)
	{	mgl_colorbar_ext(gr, sch, where,x,y,w,h);	}
	inline void Colorbar(int where,float x,float y,float w,float h)
	{	mgl_colorbar_ext(gr, 0, where,x,y,w,h);	}
	inline void Colorbar(mglData &val, const char *sch="",int where=0)
	{	mgl_colorbar_val(gr, val.a, sch, where);	}
	inline void AddLegend(const char *text,const char *style)
	{	mgl_add_legend(gr, text, style);	}
	inline void AddLegendw(const wchar_t *text,const char *style)
	{	mgl_add_legendw(gr, text, style);	}
	inline void ClearLegend()
	{	mgl_clear_legend(gr);	}
	inline void Legend(float x, float y, const char *font="L", float size=-0.8, float llen=0.1)
	{	mgl_legend_xy(gr, x, y, font, size, llen);	}
	inline void Legend(int where=3, const char *font="L", float size=-0.8, float llen=0.1)
	{	mgl_legend(gr, where, font, size, llen);	}

	inline void Plot(const char *fy, const char *stl="", int n=100)
	{	mgl_fplot(gr, fy, stl, n);	}
	inline void Plot(const char *fx, const char *fy, const char *fz, const char *stl="", int n=100)
	{	mgl_fplot_xyz(gr, fx, fy, fz, stl, n);	}
	inline void Plot(mglData &x, mglData &y, mglData &z, const char *pen="")
	{	mgl_plot_xyz(gr, &x, &y, &z, pen);	}
	inline void Plot(mglData &x, mglData &y, const char *pen="")
	{	mgl_plot_xy(gr, &x, &y, pen);	}
	inline void Plot(mglData &y, const char *pen="")
	{	mgl_plot(gr, &y, pen);	}
	inline void BoxPlot(mglData &x, mglData &y, const char *pen="")
	{	mgl_boxplot_xy(gr, &x, &y, pen);	}
	inline void BoxPlot(mglData &y, const char *pen="")
	{	mgl_boxplot(gr, &y, pen);	}
	inline void Radar(mglData &a, const char *pen="", float r=-1)
	{	mgl_radar(gr, &a, pen, r);	}
	inline void Tens(mglData &x, mglData &y, mglData &z, mglData &c, const char *pen="")
	{	mgl_tens_xyz(gr, &x, &y, &z, &c, pen);	}
	inline void Tens(mglData &x, mglData &y, mglData &c, const char *pen="")
	{	mgl_tens_xy(gr, &x, &y, &c, pen);	}
	inline void Tens(mglData &y, mglData &c, const char *pen="")
	{	mgl_tens(gr, &y, &c, pen);	}
	inline void Area(mglData &x, mglData &y, mglData &z, const char *pen="")
	{	mgl_area_xyz(gr, &x, &y, &z, pen);	}
	inline void Area(mglData &x, mglData &y, const char *pen="")
	{	mgl_area_xy(gr, &x, &y, pen);	}
	inline void Area(mglData &y, const char *pen="")
	{	mgl_area(gr, &y, pen);	}
	inline void Stem(mglData &x, mglData &y, mglData &z, const char *pen="")
	{	mgl_stem_xyz(gr, &x, &y, &z, pen);	}
	inline void Stem(mglData &x, mglData &y, const char *pen="")
	{	mgl_stem_xy(gr, &x, &y, pen);	}
	inline void Stem(mglData &y, const char *pen="")
	{	mgl_stem(gr, &y, pen);	}
	inline void Step(mglData &x, mglData &y, mglData &z, const char *pen="")
	{	mgl_step_xyz(gr, &x, &y, &z, pen);	}
	inline void Step(mglData &x, mglData &y, const char *pen="")
	{	mgl_step_xy(gr, &x, &y, pen);	}
	inline void Step(mglData &y, const char *pen="")
	{	mgl_step(gr, &y, pen);	}
	inline void Bars(mglData &x, mglData &y, mglData &z, const char *pen="")
	{	mgl_bars_xyz(gr, &x, &y, &z, pen);	}
	inline void Bars(mglData &x, mglData &y, const char *pen="")
	{	mgl_bars_xy(gr, &x, &y, pen);	}
	inline void Bars(mglData &y, const char *pen="")
	{	mgl_bars(gr, &y, pen);	}

	inline void Barh(mglData &y, mglData &v, const char *pen="")
	{	mgl_barh_yx(gr, &y, &v, pen);	}
	inline void Barh(mglData &v, const char *pen="")
	{	mgl_barh(gr, &v, pen);	}
	inline void Region(mglData &y1, mglData &y2, const char *pen=NULL, bool inside=true)
	{	mgl_region(gr, &y1, &y2, pen, inside);	}
	inline void Region(mglData &x, mglData &y1, mglData &y2, const char *pen=NULL, bool inside=true)
	{	mgl_region_xy(gr, &x, &y1, &y2, pen, inside);	}

	inline void Torus(mglData &r, mglData &z, const char *pen="")
	{	mgl_torus(gr, &r, &z, pen);	}
	inline void Text(mglData &x, mglData &y, mglData &z, const char *text, const char *font="", float size=-1)
	{	mgl_text_xyz(gr, &x, &y, &z, text, font, size);	}
	inline void Text(mglData &x, mglData &y, const char *text, const char *font="", float size=-1)
	{	mgl_text_xy(gr, &x, &y, text, font, size);	}
	inline void Text(mglData &y, const char *text, const char *font="", float size=-1)
	{	mgl_text_y(gr, &y, text, font, size);	}
	inline void Chart(mglData &a, const char *colors="")
	{	mgl_chart(gr, &a, colors);	}
	inline void Error(mglData &y, mglData &ey, const char *pen="")
	{	mgl_error(gr, &y, &ey, pen);	}
	inline void Error(mglData &x, mglData &y, mglData &ey, const char *pen="")
	{	mgl_error_xy(gr, &x, &y, &ey, pen);	}
	inline void Error(mglData &x, mglData &y, mglData &ex, mglData &ey, const char *pen="")
	{	mgl_error_exy(gr, &x, &y, &ex, &ey, pen);	}
	inline void Mark(mglData &x, mglData &y, mglData &z, mglData &r, const char *pen)
	{	mgl_mark_xyz(gr, &x, &y, &z, &r, pen);	}
	inline void Mark(mglData &x, mglData &y, mglData &r, const char *pen)
	{	mgl_mark_xy(gr, &x, &y, &r, pen);	}
	inline void Mark(mglData &y, mglData &r, const char *pen)
	{	mgl_mark_y(gr, &y, &r, pen);	}
	inline void TextMark(mglData &x, mglData &y, mglData &z, mglData &r, const char *text, const char *fnt="")
	{	mgl_textmark_xyzr(gr, &x, &y, &z, &r, text, fnt);	}
	inline void TextMark(mglData &x, mglData &y, mglData &r, const char *text, const char *fnt="")
	{	mgl_textmark_xyr(gr, &x, &y, &r, text, fnt);	}
	inline void TextMark(mglData &y, mglData &r, const char *text, const char *fnt="")
	{	mgl_textmark_yr(gr, &y, &r, text, fnt);	}
	inline void TextMark(mglData &y, const char *text, const char *fnt="")
	{	mgl_textmark(gr, &y, text, fnt);	}
	inline void Tube(mglData &x, mglData &y, mglData &z, mglData &r, const char *pen="")
	{	mgl_tube_xyzr(gr, &x, &y, &z, &r, pen);	}
	inline void Tube(mglData &x, mglData &y, mglData &z, float r, const char *pen="")
	{	mgl_tube_xyz(gr, &x, &y, &z, r, pen);	}
	inline void Tube(mglData &x, mglData &y, mglData &r, const char *pen="")
	{	mgl_tube_xyr(gr, &x, &y, &r, pen);	}
	inline void Tube(mglData &x, mglData &y, float r, const char *pen="")
	{	mgl_tube_xy(gr, &x, &y, r, pen);	}
	inline void Tube(mglData &y, mglData &r, const char *pen="")
	{	mgl_tube_r(gr, &y, &r, pen);	}
	inline void Tube(mglData &y, float r, const char *pen="")
	{	mgl_tube(gr, &y, r, pen);	}

	inline void Surf(const char *fz, const char *stl="", int n=100)
	{	mgl_fsurf(gr, fz, stl, n);	}
	inline void Surf(const char *fx, const char *fy, const char *fz, const char *stl="", int n=100)
	{	mgl_fsurf_xyz(gr, fx, fy, fz, stl, n);	}
	inline void Grid(mglData &x, mglData &y, mglData &z, const char *stl="", float zVal=NAN)
	{	mgl_grid_xy(gr, &x, &y, &z, stl, zVal);	}
	inline void Grid(mglData &z, const char *stl="", float zVal=NAN)
	{	mgl_grid(gr, &z, stl, zVal);	}
	inline void Mesh(mglData &x, mglData &y, mglData &z, const char *stl="")
	{	mgl_mesh_xy(gr, &x, &y, &z, stl);	}
	inline void Mesh(mglData &z, const char *stl="")
	{	mgl_mesh(gr, &z, stl);	}
	inline void Fall(mglData &x, mglData &y, mglData &z, const char *stl="")
	{	mgl_fall_xy(gr, &x, &y, &z, stl);	}
	inline void Fall(mglData &z, const char *stl="")
	{	mgl_fall(gr, &z, stl);	}
	inline void Belt(mglData &x, mglData &y, mglData &z, const char *stl="")
	{	mgl_belt_xy(gr, &x, &y, &z, stl);	}
	inline void Belt(mglData &z, const char *stl="")
	{	mgl_belt(gr, &z, stl);	}
	inline void Surf(mglData &x, mglData &y, mglData &z, const char *stl="")
	{	mgl_surf_xy(gr, &x, &y, &z, stl);	}
	inline void Surf(mglData &z, const char *stl="")
	{	mgl_surf(gr, &z, stl);	}
	inline void Tile(mglData &x, mglData &y, mglData &z, const char *stl="")
	{	mgl_tile_xy(gr, &x, &y, &z, stl);	}
	inline void Tile(mglData &z, const char *stl="")
	{	mgl_tile(gr, &z, stl);	}
	inline void TileS(mglData &x, mglData &y, mglData &z, mglData &r, const char *stl="")
	{	mgl_tiles_xy(gr, &x, &y, &z, &r, stl);	}
	inline void TileS(mglData &z, mglData &r, const char *stl="")
	{	mgl_tiles(gr, &z, &r, stl);	}
	inline void Dens(mglData &x, mglData &y, mglData &z, const char *stl="", float zVal=NAN)
	{	mgl_dens_xy(gr, &x, &y, &z, stl, zVal);	}
	inline void Dens(mglData &z, const char *stl="", float zVal=NAN)
	{	mgl_dens(gr, &z, stl, zVal);	}
	inline void Boxs(mglData &x, mglData &y, mglData &z, const char *stl="", float zVal=NAN)
	{	mgl_boxs_xy(gr, &x, &y, &z, stl, zVal);	}
	inline void Boxs(mglData &z, const char *stl="", float zVal=NAN)
	{	mgl_boxs(gr, &z, stl, zVal);	}

	inline void Cont(mglData &v, mglData &x, mglData &y, mglData &z, const char *sch="", float zVal=NAN)
	{	mgl_cont_xy_val(gr, &v, &x, &y, &z, sch, zVal);	}
	inline void Cont(mglData &v, mglData &z, const char *sch="", float zVal=NAN)
	{	mgl_cont_val(gr, &v, &z, sch, zVal);	}
	inline void Cont(mglData &x, mglData &y, mglData &z, const char *sch="", int Num=7, float zVal=NAN)
	{	mgl_cont_xy(gr, &x, &y, &z, sch, Num, zVal);	}
	inline void Cont(mglData &z, const char *sch="", int Num=7, float zVal=NAN)
	{	mgl_cont(gr, &z, sch, Num, zVal);	}

	inline void ContF(mglData &v, mglData &x, mglData &y, mglData &z, const char *sch="", float zVal=NAN)
	{	mgl_contf_xy_val(gr, &v, &x, &y, &z, sch, zVal);	}
	inline void ContF(mglData &v, mglData &z, const char *sch="", float zVal=NAN)
	{	mgl_contf_val(gr, &v, &z, sch, zVal);	}
	inline void ContF(mglData &x, mglData &y, mglData &z, const char *sch="", int Num=7, float zVal=NAN)
	{	mgl_contf_xy(gr, &x, &y, &z, sch, Num, zVal);	}
	inline void ContF(mglData &z, const char *sch="", int Num=7, float zVal=NAN)
	{	mgl_contf(gr, &z, sch, Num, zVal);	}

	inline void ContD(mglData &v, mglData &x, mglData &y, mglData &z, const char *sch="", float zVal=NAN)
	{	mgl_contd_xy_val(gr, &v, &x, &y, &z, sch, zVal);	}
	inline void ContD(mglData &v, mglData &z, const char *sch="", float zVal=NAN)
	{	mgl_contd_val(gr, &v, &z, sch, zVal);	}
	inline void ContD(mglData &x, mglData &y, mglData &z, const char *sch="", float zVal=NAN)
	{	mgl_contd_xy(gr, &x, &y, &z, sch, zVal);	}
	inline void ContD(mglData &z, const char *sch="", float zVal=NAN)
	{	mgl_contd(gr, &z, sch, zVal);	}

	inline void Axial(mglData &v, mglData &x, mglData &y, mglData &z, const char *sch="")
	{	mgl_axial_xy_val(gr, &v, &x, &y, &z, sch);	}
	inline void Axial(mglData &v, mglData &z, const char *sch="")
	{	mgl_axial_val(gr, &v, &z, sch);	}
	inline void Axial(mglData &x, mglData &y, mglData &z, const char *sch="", int Num=3)
	{	mgl_axial_xy(gr, &x, &y, &z, sch, Num);	}
	inline void Axial(mglData &z, const char *sch="", int Num=3)
	{	mgl_axial(gr, &z, sch, Num);	}

	inline void SurfC(mglData &x, mglData &y, mglData &z, mglData &c, const char *sch="")
	{	mgl_surfc_xy(gr, &x, &y, &z, &c, sch);	}
	inline void SurfC(mglData &z, mglData &c, const char *sch="")
	{	mgl_surfc(gr, &z, &c, sch);	}
	inline void SurfA(mglData &x, mglData &y, mglData &z, mglData &c, const char *sch="")
	{	mgl_surfa_xy(gr, &x, &y, &z, &c, sch);	}
	inline void SurfA(mglData &z, mglData &c, const char *sch="")
	{	mgl_surfa(gr, &z, &c, sch);	}
	inline void STFA(mglData &x, mglData &y, mglData &re, mglData &im, int dn, const char *sch="", float zVal=NAN)
	{	mgl_stfa_xy(gr, &x, &y, &re, &im, dn, sch, zVal);	}
	inline void STFA(mglData &re, mglData &im, int dn, const char *sch="", float zVal=NAN)
	{	mgl_stfa(gr, &re, &im, dn, sch, zVal);	}

	inline void Dew(mglData &x, mglData &y, mglData &ax, mglData &ay, const char *sch="", float zVal=NAN)
	{	mgl_dew_xy(gr, &x, &y, &ax, &ay, sch, zVal);	}
	inline void Dew(mglData &ax, mglData &ay, const char *sch="", float zVal=NAN)
	{	mgl_dew_2d(gr, &ax, &ay, sch, zVal);	}
	inline void Traj(mglData &x, mglData &y, mglData &ax, mglData &ay, const char *sch="", float zVal=NAN, float len=0)
	{	mgl_traj_xy(gr, &x, &y, &ax, &ay, sch, zVal, len);	}
	inline void Traj(mglData &x, mglData &y, mglData &z, mglData &ax, mglData &ay, mglData &az, const char *sch="", float len=0)
	{	mgl_traj_xyz(gr, &x, &y, &z, &ax, &ay, &az, sch, len);	}
	inline void Vect(mglData &x, mglData &y, mglData &ax, mglData &ay, const char *sch="", float zVal=NAN, int flag=0)
	{	mgl_vect_xy(gr, &x, &y, &ax, &ay, sch, zVal, flag);	}
	inline void Vect(mglData &ax, mglData &ay, const char *sch="", float zVal=NAN, int flag=0)
	{	mgl_vect_2d(gr, &ax, &ay, sch, zVal, flag);	}
	inline void Vect(mglData &x, mglData &y, mglData &z, mglData &ax, mglData &ay, mglData &az, const char *sch="", int flag=0)
	{	mgl_vect_xyz(gr, &x, &y, &z, &ax, &ay, &az, sch, flag);	}
	inline void Vect(mglData &ax, mglData &ay, mglData &az, const char *sch="", int flag=0)
	{	mgl_vect_3d(gr, &ax, &ay, &az, sch, flag);	}
	inline void Map(mglData &x, mglData &y, mglData &a, mglData &b, const char *sch="", int ks=0, bool pnts=true)
	{	mgl_map_xy(gr, &x, &y, &a, &b, sch, ks, pnts);	}
	inline void Map(mglData &a, mglData &b, const char *sch="", int ks=0, bool pnts=true)
	{	mgl_map(gr, &a, &b, sch, ks, pnts);	}

	inline void Surf3A(float Val, mglData &x, mglData &y, mglData &z, mglData &a, mglData &b, const char *stl="")
	{	mgl_surf3a_xyz_val(gr, Val, &x, &y, &z, &a, &b, stl);	}
	inline void Surf3A(float Val, mglData &a, mglData &b, const char *stl="")
	{	mgl_surf3a_val(gr, Val, &a, &b, stl);	}
	inline void Surf3A(mglData &x, mglData &y, mglData &z, mglData &a, mglData &b, const char *stl="", int num=3)
	{	mgl_surf3a_xyz(gr, &x, &y, &z, &a, &b, stl, num);	}
	inline void Surf3A(mglData &a, mglData &b, const char *stl="", int num=3)
	{	mgl_surf3a(gr, &a, &b, stl, num);	}
	inline void Surf3C(float Val, mglData &x, mglData &y, mglData &z, mglData &a, mglData &b, const char *stl="")
	{	mgl_surf3c_xyz_val(gr, Val, &x, &y, &z, &a, &b, stl);	}
	inline void Surf3C(float Val, mglData &a, mglData &b, const char *stl="")
	{	mgl_surf3c_val(gr, Val, &a, &b, stl);	}
	inline void Surf3C(mglData &x, mglData &y, mglData &z, mglData &a, mglData &b, const char *stl="", int num=3)
	{	mgl_surf3c_xyz(gr, &x, &y, &z, &a, &b, stl, num);	}
	inline void Surf3C(mglData &a, mglData &b, const char *stl="", int num=3)
	{	mgl_surf3c(gr, &a, &b, stl, num);	}

	inline void Flow(mglData &x, mglData &y, mglData &ax, mglData &ay, const char *sch="", int num=5, float zVal=NAN)
	{	mgl_flow_xy(gr, &x, &y, &ax, &ay, sch, num, zVal);	}
	inline void Flow(mglData &ax, mglData &ay, const char *sch="", int num=5, float zVal=NAN)
	{	mgl_flow_2d(gr, &ax, &ay, sch, num, zVal);	}
	inline void Flow(mglData &x, mglData &y, mglData &z, mglData &ax, mglData &ay, mglData &az, const char *sch="", int num=3)
	{	mgl_flow_xyz(gr, &x, &y, &z, &ax, &ay, &az, sch, num);	}
	inline void Flow(mglData &ax, mglData &ay, mglData &az, const char *sch="", int num=3)
	{	mgl_flow_3d(gr, &ax, &ay, &az, sch, num);	}

	inline void FlowP(float x0, float y0, float z0, mglData &x, mglData &y, mglData &ax, mglData &ay, const char *sch="")
	{	mgl_flowp_xy(gr, x0, y0, z0, &x, &y, &ax, &ay, sch);	}
	inline void FlowP(float x0, float y0, float z0, mglData &ax, mglData &ay, const char *sch="")
	{	mgl_flowp_2d(gr, x0, y0, z0, &ax, &ay, sch);	}
	inline void FlowP(float x0, float y0, float z0, mglData &x, mglData &y, mglData &z, mglData &ax, mglData &ay, mglData &az, const char *sch="")
	{	mgl_flowp_xyz(gr, x0, y0, z0, &x, &y, &z, &ax, &ay, &az, sch);	}
	inline void FlowP(float x0, float y0, float z0, mglData &ax, mglData &ay, mglData &az, const char *sch="")
	{	mgl_flowp_3d(gr, x0, y0, z0, &ax, &ay, &az, sch);	}

	inline void Grad(const mglData &x, const mglData &y, const mglData &z, const mglData &phi, const char *sch=0, int num=3)
	{	mgl_grad_xyz(gr,&x,&y,&z,&phi,sch,num);	}
	inline void Grad(const mglData &x, const mglData &y, const mglData &phi, const char *sch=0, int num=5, mreal zVal=NAN)
	{	mgl_grad_xy(gr,&x,&y,&phi,sch,num,zVal);	}
	inline void Grad(const mglData &phi, const char *sch=0, int num=5, mreal zVal=NAN)
	{	mgl_grad(gr,&phi,sch,num,zVal);	}

	inline void Pipe(mglData &x, mglData &y, mglData &ax, mglData &ay, const char *sch="", float r0=0.05, int num=5, float zVal=NAN)
	{	mgl_pipe_xy(gr, &x, &y, &ax, &ay, sch, r0, num, zVal);	}
	inline void Pipe(mglData &ax, mglData &ay, const char *sch="", float r0=0.05, int num=5, float zVal=NAN)
	{	mgl_pipe_2d(gr, &ax, &ay, sch, r0, num, zVal);	}
	inline void Pipe(mglData &x, mglData &y, mglData &z, mglData &ax, mglData &ay, mglData &az, const char *sch="", float r0=0.05, int num=3)
	{	mgl_pipe_xyz(gr, &x, &y, &z, &ax, &ay, &az, sch, r0, num);	}
	inline void Pipe(mglData &ax, mglData &ay, mglData &az, const char *sch="", float r0=0.05, int num=3)
	{	mgl_pipe_3d(gr, &ax, &ay, &az, sch, r0, num);	}

	inline void Grid3(mglData &x, mglData &y, mglData &z, mglData &a, char dir, int sVal, const char *stl="")
	{	mgl_grid3_xyz(gr, &x, &y, &z, &a, dir, sVal, stl);	}
	inline void Grid3(mglData &a, char dir, int sVal, const char *stl="")
	{	mgl_grid3(gr, &a, dir, sVal, stl);	}
	inline void Dens3(mglData &x, mglData &y, mglData &z, mglData &a, char dir, int sVal, const char *stl="")
	{	mgl_dens3_xyz(gr, &x, &y, &z, &a, dir, sVal, stl);	}
	inline void Dens3(mglData &a, char dir, int sVal, const char *stl="")
	{	mgl_dens3(gr, &a, dir, sVal, stl);	}

	inline void Surf3(float Val, mglData &x, mglData &y, mglData &z, mglData &a, const char *stl="")
	{	mgl_surf3_xyz_val(gr, Val, &x, &y, &z, &a, stl);	}
	inline void Surf3(float Val, mglData &a, const char *stl="")
	{	mgl_surf3_val(gr, Val, &a, stl);	}
	inline void Surf3(mglData &x, mglData &y, mglData &z, mglData &a, const char *stl="", int num=3)
	{	mgl_surf3_xyz(gr, &x, &y, &z, &a, stl, num);	}
	inline void Surf3(mglData &a, const char *stl="", int num=3)
	{	mgl_surf3(gr, &a, stl, num);	}
	inline void Cloud(mglData &x, mglData &y, mglData &z, mglData &a, const char *stl="", float alpha=1)
	{	mgl_cloud_xyz(gr, &x, &y, &z, &a, stl, alpha);	}
	inline void Cloud(mglData &a, const char *stl="", float alpha=1)
	{	mgl_cloud(gr, &a, stl, alpha);	}

	inline void Cont3(mglData &v, mglData &x, mglData &y, mglData &z, mglData &a, char dir, int sVal, const char *sch="")
	{	mgl_cont3_xyz_val(gr, &v, &x, &y, &z, &a, dir, sVal, sch);	}
	inline void Cont3(mglData &v, mglData &a, char dir, int sVal, const char *sch="")
	{	mgl_cont3_val(gr, &v, &a, dir, sVal, sch);	}
	inline void Cont3(mglData &x, mglData &y, mglData &z, mglData &a, char dir, int sVal, const char *sch="", int Num=7)
	{	mgl_cont3_xyz(gr, &x, &y, &z, &a, dir, sVal, sch, Num);	}
	inline void Cont3(mglData &a, char dir, int sVal, const char *sch="", int Num=7)
	{	mgl_cont3(gr, &a, dir, sVal, sch, Num);	}

	inline void ContF3(mglData &v, mglData &x, mglData &y, mglData &z, mglData &a, char dir, int sVal, const char *sch="")
	{	mgl_contf3_xyz_val(gr, &v, &x, &y, &z, &a, dir, sVal, sch);	}
	inline void ContF3(mglData &v, mglData &a, char dir, int sVal, const char *sch="")
	{	mgl_contf3_val(gr, &v, &a, dir, sVal, sch);	}
	inline void ContF3(mglData &x, mglData &y, mglData &z, mglData &a, char dir, int sVal, const char *sch="", int Num=7)
	{	mgl_contf3_xyz(gr, &x, &y, &z, &a, dir, sVal, sch, Num);	}
	inline void ContF3(mglData &a, char dir, int sVal, const char *sch="", int Num=7)
	{	mgl_contf3(gr, &a, dir, sVal, sch, Num);	}

	inline void Beam(mglData &tr, mglData &g1, mglData &g2, mglData &a, float r, const char *stl=0, int flag=0, int num=3)
	{	mgl_beam(gr, &tr,&g1,&g2,&a,r,stl,flag,num);	}
	inline void Beam(float val, mglData &tr, mglData &g1, mglData &g2, mglData &a, float r, const char *stl=NULL, int flag=0)
	{	mgl_beam_val(gr,val,&tr,&g1,&g2,&a,r,stl,flag);	}


	inline void TriPlot(mglData &nums, mglData &x, mglData &y, mglData &z, mglData &c, const char *sch="")
	{	mgl_triplot_xyzc(gr, &nums, &x, &y, &z, &c, sch);	}
	inline void TriPlot(mglData &nums, mglData &x, mglData &y, mglData &z, const char *sch="")
	{	mgl_triplot_xyz(gr, &nums, &x, &y, &z, sch);	}
	inline void TriPlot(mglData &nums, mglData &x, mglData &y, const char *sch="", float zVal=NAN)
	{	mgl_triplot_xy(gr, &nums, &x, &y, sch, zVal);	}
	inline void QuadPlot(mglData &nums, mglData &x, mglData &y, mglData &z, mglData &c, const char *sch="")
	{	mgl_quadplot_xyzc(gr, &nums, &x, &y, &z, &c, sch);	}
	inline void QuadPlot(mglData &nums, mglData &x, mglData &y, mglData &z, const char *sch="")
	{	mgl_quadplot_xyz(gr, &nums, &x, &y, &z, sch);	}
	inline void QuadPlot(mglData &nums, mglData &x, mglData &y, const char *sch="", float zVal=NAN)
	{	mgl_quadplot_xy(gr, &nums, &x, &y, sch, zVal);	}

	inline void TriCont(const mglData &nums, const mglData &x, const mglData &y, const mglData &z, const char *sch=0,int num=7,mreal zVal=NAN)
	{	mgl_tricont_xyc(gr, &nums, &x, &y, &z, sch, num, zVal);	}
	inline void TriContV(const mglData &v, const mglData &nums, const mglData &x, const mglData &y, const mglData &z, const char *sch=0,mreal zVal=NAN)
	{	mgl_tricont_xycv(gr, &v, &nums, &x, &y, &z, sch, zVal);	}
	inline void TriCont(const mglData &nums, const mglData &x, const mglData &y, const mglData &z, const mglData &a, const char *sch=0,int num=7,mreal zVal=NAN)
	{	mgl_tricont_xyzc(gr, &nums, &x, &y, &z, &a, sch, num, zVal);	}
	inline void TriContV(const mglData &v, const mglData &nums, const mglData &x, const mglData &y, const mglData &z, const mglData &a, const char *sch=0,mreal zVal=NAN)
	{	mgl_tricont_xyzcv(gr, &v, &nums, &x, &y, &z, &a, sch, zVal);	}

	inline void Dots(mglData &x, mglData &y, mglData &z, const char *sch="")
	{	mgl_dots(gr, &x, &y, &z, sch);	}
	inline void Dots(mglData &x, mglData &y, mglData &z, mglData &a, const char *sch="")
	{	mgl_dots_a(gr, &x, &y, &z, &a, sch);	}
	inline void Crust(mglData &x, mglData &y, mglData &z, const char *sch="", float er=0)
	{	mgl_crust(gr, &x, &y, &z, sch, er);	}

	inline void DensX(mglData &a, const char *stl="", float sVal=NAN)
	{	mgl_dens_x(gr, &a, stl, sVal);	}
	inline void DensY(mglData &a, const char *stl="", float sVal=NAN)
	{	mgl_dens_y(gr, &a, stl, sVal);	}
	inline void DensZ(mglData &a, const char *stl="", float sVal=NAN)
	{	mgl_dens_z(gr, &a, stl, sVal);	}
	inline void ContX(mglData &a, const char *stl="", float sVal=NAN, int Num=7)
	{	mgl_cont_x(gr, &a, stl, sVal, Num);	}
	inline void ContY(mglData &a, const char *stl="", float sVal=NAN, int Num=7)
	{	mgl_cont_y(gr, &a, stl, sVal, Num);	}
	inline void ContZ(mglData &a, const char *stl="", float sVal=NAN, int Num=7)
	{	mgl_cont_z(gr, &a, stl, sVal, Num);	}
	inline void ContX(mglData &v, mglData &a, const char *stl="", float sVal=NAN)
	{	mgl_cont_x_val(gr, &v, &a, stl, sVal);	}
	inline void ContY(mglData &v, mglData &a, const char *stl="", float sVal=NAN)
	{	mgl_cont_y_val(gr, &v, &a, stl, sVal);	}
	inline void ContZ(mglData &v, mglData &a, const char *stl="", float sVal=NAN)
	{	mgl_cont_z_val(gr, &v, &a, stl, sVal);	}
	inline void ContFX(mglData &a, const char *stl="", float sVal=NAN, int Num=7)
	{	mgl_contf_x(gr, &a, stl, sVal, Num);	}
	inline void ContFY(mglData &a, const char *stl="", float sVal=NAN, int Num=7)
	{	mgl_contf_y(gr, &a, stl, sVal, Num);	}
	inline void ContFZ(mglData &a, const char *stl="", float sVal=NAN, int Num=7)
	{	mgl_contf_z(gr, &a, stl, sVal, Num);	}
	inline void ContFX(mglData &v, mglData &a, const char *stl="", float sVal=NAN)
	{	mgl_contf_x_val(gr, &v, &a, stl, sVal);	}
	inline void ContFY(mglData &v, mglData &a, const char *stl="", float sVal=NAN)
	{	mgl_contf_y_val(gr, &v, &a, stl, sVal);	}
	inline void ContFZ(mglData &v, mglData &a, const char *stl="", float sVal=NAN)
	{	mgl_contf_z_val(gr, &v, &a, stl, sVal);	}

	inline mglData Hist(mglData &x, mglData &a)
	{	return mglData(true, mgl_hist_x(gr, &x, &a));	}
	inline mglData Hist(mglData &x, mglData &y, mglData &a)
	{	return mglData(true, mgl_hist_xy(gr, &x, &y, &a));	}
	inline mglData Hist(mglData &x, mglData &y, mglData &z, mglData &a)
	{	return mglData(true, mgl_hist_xyz(gr, &x, &y, &z, &a));	}

	inline mglData Fit(mglData &y, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_1(gr, &y, eq,var,0));	}
	inline mglData Fit(mglData &fit, mglData &y, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_1(gr, &y, eq, var, &ini));	}
	inline mglData Fit2(mglData &fit, mglData &z, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_2(gr, &z, eq, var,0));	}
	inline mglData Fit2(mglData &fit, mglData &z, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_2(gr, &z, eq, var, &ini));	}
	inline mglData Fit3(mglData &fit, mglData &a, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_3(gr, &a, eq, var,0));	}
	inline mglData Fit3(mglData &fit, mglData &a, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_3(gr, &a, eq, var, &ini));	}
	inline mglData Fit(mglData &fit, mglData &x, mglData &y, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_xy(gr, &x, &y, eq, var,0));	}
	inline mglData Fit(mglData &fit, mglData &x, mglData &y, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_xy(gr, &x, &y, eq, var, &ini));	}
	inline mglData Fit(mglData &fit, mglData &x, mglData &y, mglData &z, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_xyz(gr, &x, &y, &z, eq, var,0));	}
	inline mglData Fit(mglData &fit, mglData &x, mglData &y, mglData &z, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_xyz(gr, &x, &y, &z, eq, var, &ini));	}
	inline mglData Fit(mglData &fit, mglData &x, mglData &y, mglData &z, mglData &a, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_xyza(gr, &x, &y, &z, &a, eq, var,0));	}
	inline mglData Fit(mglData &fit, mglData &x, mglData &y, mglData &z, mglData &a, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_xyza(gr, &x, &y, &z, &a, eq,var, &ini));	}
	inline mglData FitS(mglData &fit, mglData &y, mglData &s, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_ys(gr, &y, &s, eq, var,0));	}
	inline mglData FitS(mglData &fit, mglData &y, mglData &s, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_ys(gr, &y, &s, eq, var, &ini));	}
	inline mglData FitS(mglData &fit, mglData &x, mglData &y, mglData &s, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_xys(gr, &x, &y, &s, eq, var,0));	}
	inline mglData FitS(mglData &fit, mglData &x, mglData &y, mglData &s, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_xys(gr, &x, &y, &s, eq, var, &ini));	}
	inline mglData FitS(mglData &fit, mglData &x, mglData &y, mglData &z, mglData &s, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_xyzs(gr, &x, &y, &z, &s, eq, var,0));	}
	inline mglData FitS(mglData &fit, mglData &x, mglData &y, mglData &z, mglData &s, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_xyzs(gr, &x, &y, &z, &s, eq, var, &ini));	}
	inline mglData FitS(mglData &fit, mglData &x, mglData &y, mglData &z, mglData &a, mglData &s, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_xyzas(gr, &x, &y, &z, &a, &s, eq, var,0));	}
	inline mglData FitS(mglData &fit, mglData &x, mglData &y, mglData &z, mglData &a, mglData &s, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_xyzas(gr, &x, &y, &z, &a, &s, eq, var, &ini));	}
	inline void PutsFit(float x, float y, float z, const char *prefix=0, const char *font=0, float size=-1)
	{	mgl_puts_fit(gr, x, y, z, prefix, font, size);	}
	inline const char *GetFit()
	{	return mgl_get_fit(gr);	}

	inline mglData PDE(const char *ham, mglData &ini_re, mglData &ini_im, float dz=0.1, float k0=100)
	{	return mglData(true,mgl_pde_solve(gr,ham,&ini_re,&ini_im,dz,k0));	}
	inline void Fill(mglData &u, const char *eq)
	{	mgl_data_fill_eq(gr, &u, eq, 0, 0);	}
	inline void Fill(mglData &u, const char *eq, mglData &v)
	{	mgl_data_fill_eq(gr, &u, eq, &v, 0);	}
	inline void Fill(mglData &u, const char *eq, mglData &v, mglData &w)
	{	mgl_data_fill_eq(gr, &u, eq, &v, &w);	}
};
//-----------------------------------------------------------------------------
/*class mglParse
{
	HMPR gr;
public:
	mglParse()		{	gr=mgl_create_parser();	}
	~mglParse()	{	mgl_delete_parser(gr);	}
	inline void AddParam(int id, const char *str)	{	mgl_add_param(gr, id, str);	}
	inline void AddParam(int id, const wchar_t *str){	mgl_add_paramw(gr, id, str);	}
	inline mglData AddVar(const char *name)	{	return mglData(mgl_add_var(gr, name),false);	}
	inline mglData FindVar(const char *name)	{	return mglData(mgl_find_var(gr, name),false);	}
	inline int Parse(mglGraph *gr, const char *str, int pos)
	{	return mgl_parse(gr->Self(), gr, str, pos);	}
	inline int Parse(mglGraph *gr, const wchar_t *str, int pos)
	{	return mgl_parsew(gr->Self(), gr, str, pos);	}
	inline void ScanFunc(const wchar_t *line){	mgl_scan_func(gr, line);	}
	inline void Execute(mglGraph *gr, const char *str)
	{	mgl_parse_text(gr->Self(), gr, str);	}
	inline void Execute(mglGraph *gr, const wchar_t *str)
	{	mgl_parsew_text(gr->Self(), gr, str);	}
	inline void RestoreOnce()	{	mgl_restore_once(gr);	}
	inline void AllowSetSize(bool allow)	{	mgl_parser_allow_setsize(gr, allow);	}
};*/
//-----------------------------------------------------------------------------
#endif
