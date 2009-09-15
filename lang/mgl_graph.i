/***************************************************************************
 * mgl_graph.i is part of Math Graphic Library
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

%module mathgl

%{
#include <math.h>
#include "mgl/mgl_c.h"
%}

typedef mglGraph *HMGL;
typedef mglData *HMDT;
typedef mglParse *HMPR;

struct mglGraph{};
struct mglParse{};

%extend mglParse {
	mglParse()
	{	return mgl_create_parser();	}
	~mglParse()
	{	return mgl_delete_parser(self);	}
	void AddParam(int id, const char *str)
	{	return mgl_add_param(self, id, str);	}
	void AddParam(int id, const wchar_t *str)
	{	return mgl_add_paramw(self, id, str);	}
	const mglData *AddVar(const char *name)
	{	return mgl_add_var(self, name);	}
	const mglData *FindVar(const char *name)
	{	return mgl_find_var(self, name);}
	int Parse(mglGraph *gr, const char *str, int pos)
	{	return mgl_parse(gr, self, str, pos);	}
	int Parse(mglGraph *gr, const wchar_t *str, int pos)
	{	return mgl_parsew(gr, self, str, pos);	}
	void Execute(mglGraph *gr, const char *str)
	{	return mgl_parse_text(gr, self, str);	}
	void Execute(mglGraph *gr, const wchar_t *str)
	{	return mgl_parsew_text(gr, self, str);	}
	void RestoreOnce()
	{	return mgl_restore_once(self);	}
	void AllowSetSize(bool allow)
	{	return mgl_parser_allow_setsize(self, allow);	}
};

%extend mglGraph {
	mglGraph(int kind=0, int width=600, int height=400)
	{
		if(kind==1)	return mgl_create_graph_ps(width, height);
		else if(kind==2)	return mgl_create_graph_gl();
		else if(kind==3)	return mgl_create_graph_idtf();
		return mgl_create_graph_zb(width, height);
	}
/*	mglGraph(mglDraw *dr, const char *title="MathGL window", int kind=0)
	{
		if(kind==1)	return mgl_create_graph_fltk_dr(dr, title);
		if(kind==2)	return mgl_create_graph_glut_dr(dr, title);
		return mgl_create_graph_qt_dr(dr, title);
	}*/
	~mglGraph()
	{	mgl_delete_graph(self);	}

	void DefaultPlotParam()
	{	mgl_set_def_param(self);	}
	void SetPalColor(int n, float r, float g, float b)
	{	if(n>=0 && n<100)	mgl_set_pal_color(self, n, r, g, b);	}
	void SetPalNum(int n)
	{	if(n>=0 && n<100)	mgl_set_pal_num(self, n);	}
	void SetPalette(const char *colors)
	{	mgl_set_palette(self, colors);	}
	void SetScheme(const char *colors)
	{	mgl_set_scheme(self, colors);	}
	void SetRotatedText(bool rotated)
	{	mgl_set_rotated_text(self, rotated);	}
	void SetCut(bool cut)
	{	mgl_set_cut(self, cut);	}
	void SetCutBox(float x1,float y1,float z1,float x2,float y2,float z2)
	{	mgl_set_cut_box(self, x1, y1, z1, x2, y2, z2);	}
	void SetTickLen(float len, float stt=1)
	{	mgl_set_tick_len(self, len, stt);	}
	void SetTickStl(const char *stl, const char *sub=0)
	{	mgl_set_tick_stl(self, stl, sub);	}
	void SetBarWidth(float width)
	{	mgl_set_bar_width(self, width);	}
	void SetBaseLineWidth(float width)
	{	mgl_set_base_line_width(self, width);	}
	void SetMarkSize(float size)
	{	mgl_set_mark_size(self, size);	}
	void SetArrowSize(float size)
	{	mgl_set_arrow_size(self, size);	}
	void SetFontSize(float size)
	{	mgl_set_font_size(self, size);	}
	void SetFontDef(char *fnt)
	{	mgl_set_font_def(self, fnt);	}
	void SetAlphaDef(float alpha)
	{	mgl_set_alpha_default(self, alpha);	}
	void SetSize(int width, int height)
	{	mgl_set_size(self, width, height);	}
	void SetAxialDir(char dir)
	{	mgl_set_axial_dir(self, dir);	}
	void SetMeshNum(int num)
	{	mgl_set_meshnum(self, num);		}
	void Zoom(float x1, float y1, float x2, float y2)
	{	mgl_set_zoom(self, x1, y1, x2, y2);	}
	void SetPlotFactor(float val)
	{	mgl_set_plotfactor(self, val);	}
	void SetDrawFace(bool enable)
	{	mgl_set_draw_face(self, enable);}
	void SetLegendBox(bool enable)
	{	mgl_set_legend_box(self, enable);	}
	void SetScheme(const char *sch)
	{	mgl_set_scheme(self, sch);		}
	void LoadFont(const char *name, const char *path=NULL)
	{	mgl_load_font(self, name, path);	}
	void CopyFont(mglGraph *gr)
	{	mgl_copy_font(self, gr);}
	void RestoreFont()
	{	mgl_restore_font(self);	}

	void ShowImage(const char *viewer="kuickshow", bool keep=0)
	{	mgl_show_image(self, viewer, keep);	}
	void WriteFrame(const char *fname,const char *descr="")
	{	mgl_write_frame(self, fname, descr);	}
	void WriteIDTF(const char *fname,const char *descr="")
	{	mgl_write_idtf(self, fname, descr);	}
	void WriteJPEG(const char *fname,const char *descr="")
	{	mgl_write_jpg(self, fname, descr);	}
	void WritePNG(const char *fname,const char *descr="")
	{	mgl_write_png(self, fname, descr);	}
	void WritePNGSolid(const char *fname,const char *descr="")
	{	mgl_write_png_solid(self, fname, descr);	}
	void WriteEPS(const char *fname,const char *descr="")
	{	mgl_write_eps(self, fname, descr);	}
	void WriteSVG(const char *fname,const char *descr="")
	{	mgl_write_svg(self, fname, descr);	}
	void WriteGIF(const char *fname,const char *descr="")
	{	mgl_write_gif(self, fname, descr);	}
	void StartGIF(const char *fname, int ms=100)
	{	mgl_start_gif(self, fname,ms);	}
	void CloseGIF()
	{	mgl_close_gif(self);	}
	void NewFrame()
	{	mgl_new_frame(self);	}
	void EndFrame()
	{	mgl_end_frame(self);	}
	int GetNumFrame()
	{	return mgl_get_num_frame(self);	}
	void ResetFrames()
	{	mgl_reset_frames(self);	}

	void Flush()
	{	mgl_flush(self);	}

	void GetRGB(char *imgdata, int imglen)
	{
		int w=mgl_get_width(self);
		int h=mgl_get_height(self);
		if(imglen>=3*w*h)
		{
			imglen=3*w*h;
			memcpy(imgdata, mgl_get_rgb(self),imglen);
		}
	}
	void GetRGBA(char *imgdata, int imglen)
	{
		int w=mgl_get_width(self);
		int h=mgl_get_height(self);
		if(imglen>=4*w*h)
		{
			imglen=4*w*h;
			memcpy(imgdata, mgl_get_rgba(self),imglen);
		}
	}
	void GetBGRN(char *imgdata, int imglen)
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
	}
	int GetWidth()
	{	return mgl_get_width(self);	}
	int GetHeight()
	{	return mgl_get_height(self);}
	mglPoint CalcXYZ(int xs, int ys)
	{
#ifdef MGL_USE_DOUBLE
		double x,y,z;
#else
		float x,y,z;
#endif
		mgl_calc_xyz(self,xs,ys,&x,&y,&z);
		return mglPoint(x,y,z);
	}

	void SetTranspType(int type)
	{	mgl_set_transp_type(self, type);}
	void SetTransparent(bool enable)
	{	mgl_set_transp(self, enable);}
	void Alpha(bool enable)
	{	mgl_set_alpha(self, enable);	}
	void Fog(float d, float dz=0.25)
	{	mgl_set_fog(self, d, dz);		}
	void Light(bool enable)
	{	mgl_set_light(self, enable);	}
	void Light(int n,bool enable)
	{	mgl_set_light_n(self, n, enable);	}
	void AddLight(int n, float x, float y, float z, char c='w')
	{	mgl_add_light(self, n, x, y, z, c);	}
	void SetAmbient(float i)
	{	mgl_set_ambbr(self, i);	}

	void Identity()
	{	mgl_identity(self);	}
	void Clf(float r=1, float g=1, float b=1)
	{	mgl_clf_rgb(self, r, g, b);	}
	void SubPlot(int nx,int ny,int m, float dx=0, float dy=0)
	{	mgl_subplot_d(self, nx, ny, m, dx, dy);	}
	void InPlot(float x1,float x2,float y1,float y2, bool rel=false)
	{	if(rel)	mgl_inplot(self, x1, x2, y1, y2);
		else	mgl_relplot(self, x1, x2, y1, y2);	}
	void ColumnPlot(int num, int ind)
	{	mgl_columnplot(self,num,ind);	}
	void Aspect(float Ax,float Ay,float Az)
	{	mgl_aspect(self, Ax, Ay, Az);		}
	void Rotate(float TetX,float TetZ=0,float TetY=0)
	{	mgl_rotate(self, TetX, TetZ, TetY);	}
	void RotateN(float Tet,float x,float y,float z)
	{	mgl_rotate_vector(self, Tet, x, y, z);	}
	void Perspective(float val)
	{	mgl_perspective(self, val);	}

	void SetTicks(char dir, float d=-5, int ns=0, float org=NaN)
	{	mgl_set_ticks_dir(self, dir, d, ns, org);	}
/*	void SetTicks(char dir, int n, float *val, const char **lbl)
	{	mgl_set_ticks_vals(self, dir, n, val, lbl);	}*/
	void AdjustTicks(const char *dir)
	{	mgl_adjust_ticks(self, dir);	}

/*	TODO: %varargs
	void mgl_set_ticks_val(HMGL graph, char dir, int n, double val, const char *lbl, ...);*/


	void SetCRange(float c1, float c2)
	{	mgl_set_caxis(self, c1, c2);	}
	void SetRanges(float x1, float x2, float y1, float y2, float z1=0, float z2=0)
	{	mgl_set_axis(self, x1, y1, z1, x2, y2, z2, NaN, NaN, NaN);	}
	void SetAutoRanges(float x1, float x2, float y1=0, float y2=0, float z1=0, float z2=0)
	{	mgl_set_auto(self, x1, x2, y1, y2, z1, z2);	}
	void SetOrigin(float x0, float y0, float z0=NaN)
	{	mgl_set_origin(self, x0, y0, z0);	}
	void SetCRange(mglData *dat, bool add=false)
	{	mgl_set_crange(self, dat, add);	}
	void SetXRange(mglData *dat, bool add=false)
	{	mgl_set_xrange(self, dat, add);	}
	void SetYRange(mglData *dat, bool add=false)
	{	mgl_set_yrange(self, dat, add);	}
	void SetZRange(mglData *dat, bool add=false)
	{	mgl_set_zrange(self, dat, add);	}
	void SetFunc(const char *EqX, const char *EqY, const char *EqZ=NULL)
	{	mgl_set_func(self, EqX, EqY, EqZ);	}
	void SetCoor(int how)
	{	mgl_set_coor(self, how);	}
	void Ternary(bool val)
	{	mgl_set_ternary(self, val);	}
	void CutOff(const char *EqC)
	{	mgl_set_cutoff(self, EqC);	}

	void Box(const char *col="k", bool ticks=true)
	{	mgl_box_str(self, col, ticks);	}
	void Axis(const char *dir="xyz", bool adjust=false)
	{	if(adjust)	mgl_adjust_ticks(self,dir);
		mgl_axis(self, dir);	}
	void Grid(const char *dir="xyz",const char *pen="B")
	{	mgl_axis_grid(self, dir, pen);	}
	void Label(char dir, const char *text, int pos=+1, float size=-1.4, float shift=0)
	{	mgl_label_ext(self, dir, text, pos, size, shift);	}
	void SetTuneTicks(bool tune, float fact_pos=1.15)
	{	mgl_tune_ticks(self, tune, fact_pos);	}
	void SetTickTemplX(const wchar_t *templ)
	{	mgl_set_xttw(self, templ);	}
	void SetTickTemplY(const wchar_t *templ)
	{	mgl_set_yttw(self, templ);	}
	void SetTickTemplZ(const wchar_t *templ)
	{	mgl_set_zttw(self, templ);	}
	void SetTickTemplC(const wchar_t *templ)
	{	mgl_set_cttw(self, templ);	}
	void SetTickTemplX(const char *templ)
	{	mgl_set_xtt(self, templ);	}
	void SetTickTemplY(const char *templ)
	{	mgl_set_ytt(self, templ);	}
	void SetTickTemplZ(const char *templ)
	{	mgl_set_ztt(self, templ);	}
	void SetTickTemplC(const char *templ)
	{	mgl_set_ctt(self, templ);	}

	void SetXTT(const wchar_t *templ)
	{	mgl_set_xttw(self, templ);	}
	void SetYTT(const wchar_t *templ)
	{	mgl_set_yttw(self, templ);	}
	void SetZTT(const wchar_t *templ)
	{	mgl_set_zttw(self, templ);	}
	void SetCTT(const wchar_t *templ)
	{	mgl_set_cttw(self, templ);	}
	void SetXTT(const char *templ)
	{	mgl_set_xtt(self, templ);	}
	void SetYTT(const char *templ)
	{	mgl_set_ytt(self, templ);	}
	void SetZTT(const char *templ)
	{	mgl_set_ztt(self, templ);	}
	void SetCTT(const char *templ)
	{	mgl_set_ctt(self, templ);	}

	void Ball(float x, float y, float z, const char *col="r")
	{	mgl_ball_str(self, x, y, z, col);	}
	void Mark(float x, float y, float z, char mark)
	{	mgl_mark(self, x, y, z, mark);	}
	void Line(float x1, float y1, float z1, float x2, float y2, float z2, const char *pen="B",int n=2)
	{	mgl_line(self, x1, y1, z1, x2, y2, z2, pen, n);	}
	void FaceX(float x0, float y0, float z0, float wy, float wz, const char *stl="w", float dx=0, float dy=0)
	{	mgl_facex(self, x0, y0, z0, wy, wz, stl, dx, dy);	}
	void FaceY(float x0, float y0, float z0, float wx, float wz, const char *stl="w", float dx=0, float dy=0)
	{	mgl_facey(self, x0, y0, z0, wx, wz, stl, dx, dy);	}
	void FaceZ(float x0, float y0, float z0, float wx, float wy, const char *stl="w", float dx=0, float dy=0)
	{	mgl_facez(self, x0, y0, z0, wx, wy, stl, dx, dy);	}
	void Curve(float x1, float y1, float z1, float dx1, float dy1, float dz1, float x2, float y2, float z2, float dx2, float dy2, float dz2, const char *pen="B", int n=100)
	{	mgl_curve(self, x1, y1, z1, dx1, dy1, dz1, x2, y2, z2, dx2, dy2, dz2, pen, n);	}
	void Drop(float x0, float y0, float z0, float dx, float dy, float dz, float r, const char *col="r", float shift=1, float ap=1)
	{	mgl_drop(self, x0, y0, z0, dx, dy, dz, r, col, shift, ap);	}
	void Sphere(float x0, float y0, float z0, float r, const char *col="r")
	{	mgl_sphere(self, x0, y0, z0, r, col);	}
	void Cone (float x1, float y1, float z1, float x2, float y2, float z2, float r1, float r2=-1, const char *stl="B", bool edge=false)
	{	mgl_cone(self, x1,y1,z1,x2,y2,z2,r1,r2,stl,edge);	}

	void Putsw(float x, float y, float z,const wchar_t *text,const char *font="C",float size=-1,char dir=0)
	{	mgl_putsw_ext(self, x, y, z, text, font, size, dir);	}
	void Puts(float x, float y, float z,const char *text,const char *font="C",float size=-1,char dir=0)
	{	mgl_puts_ext(self, x, y, z, text, font, size, dir);	}
	void Putsw(float x, float y, float z, float dx, float dy, float dz, const wchar_t *text, float size=-1)
	{	mgl_putsw_dir(self, x, y, z, dx, dy, dz, text, size);	}
	void Puts(float x, float y, float z, float dx, float dy, float dz, const char *text,float size=-1)
	{	mgl_puts_dir(self, x, y, z, dx, dy, dz, text, size);	}
	void Title(const char *text, const char *font="C", float size=-2)
	{	mgl_title(self, text, font, size);	}
	void Title(const wchar_t *text, const char *font="C", float size=-2)
	{	mgl_titlew(self, text, font, size);	}

	void Colorbar(const char *sch="",int where=0)
	{	mgl_colorbar(self, sch, where);	}
	void Colorbar(const char *sch,int where,float x,float y,float w,float h)
	{	mgl_colorbar_ext(self, sch, where,x,y,w,h);	}
	void Colorbar(int where,float x,float y,float w,float h)
	{	mgl_colorbar_ext(self, 0, where,x,y,w,h);	}
	void Colorbar(mglData *val, const char *sch="",int where=0)
	{	mgl_colorbar_val(self, val, sch, where);	}
	void SimplePlot(mglData *a, int type, const char *stl="")
	{	mgl_simple_plot(self, a, type, stl);	}
	void AddLegend(const char *text,const char *style)
	{	mgl_add_legend(self, text, style);	}
	void AddLegendw(const wchar_t *text,const char *style)
	{	mgl_add_legendw(self, text, style);	}
	void ClearLegend()
	{	mgl_clear_legend(self);	}
	void Legend(float x, float y, const char *font="L", float size=-0.8, float llen=0.1)
	{	mgl_legend_xy(self, x, y, font, size, llen);	}
	void Legend(int where=3, const char *font="L", float size=-0.8, float llen=0.1)
	{	mgl_legend(self, where, font, size, llen);	}

	void Plot(const char *fy, const char *stl="", int n=100)
	{	mgl_fplot(self, fy, stl, n);	}
	void Plot(const char *fx, const char *fy, const char *fz, const char *stl="", int n=100)
	{	mgl_fplot_xyz(self, fx, fy, fz, stl, n);	}
	void Plot(mglData *x, mglData *y, mglData *z, const char *pen="")
	{	mgl_plot_xyz(self, x, y, z, pen);	}
	void Plot(mglData *x, mglData *y, const char *pen="")
	{	mgl_plot_xy(self, x, y, pen);	}
	void Plot(mglData *y, const char *pen="")
	{	mgl_plot(self, y, pen);	}
	void BoxPlot(mglData *x, mglData *y, const char *pen="")
	{	mgl_boxplot_xy(self, x, y, pen);	}
	void BoxPlot(mglData *y, const char *pen="")
	{	mgl_boxplot(self, y, pen);	}
	void Radar(mglData *a, const char *pen="", float r=-1)
	{	mgl_radar(self, a, pen, r);	}
	void Tens(mglData *x, mglData *y, mglData *z, mglData *c, const char *pen="")
	{	mgl_tens_xyz(self, x, y, z, c, pen);	}
	void Tens(mglData *x, mglData *y, mglData *c, const char *pen="")
	{	mgl_tens_xy(self, x, y, c, pen);	}
	void Tens(mglData *y, mglData *c, const char *pen="")
	{	mgl_tens(self, y, c, pen);	}
	void Area(mglData *x, mglData *y, mglData *z, const char *pen="")
	{	mgl_area_xyz(self, x, y, z, pen);	}
	void Area(mglData *x, mglData *y, const char *pen="")
	{	mgl_area_xy(self, x, y, pen);	}
	void Area(mglData *y, const char *pen="")
	{	mgl_area(self, y, pen);	}
	void AreaSum(mglData *x, mglData *y, const char *pen="")
	{	mgl_area_xys(self, x, y, pen);	}
	void Stem(mglData *x, mglData *y, mglData *z, const char *pen="")
	{	mgl_stem_xyz(self, x, y, z, pen);	}
	void Stem(mglData *x, mglData *y, const char *pen="")
	{	mgl_stem_xy(self, x, y, pen);	}
	void Stem(mglData *y, const char *pen="")
	{	mgl_stem(self, y, pen);	}
	void Step(mglData *x, mglData *y, mglData *z, const char *pen="")
	{	mgl_step_xyz(self, x, y, z, pen);	}
	void Step(mglData *x, mglData *y, const char *pen="")
	{	mgl_step_xy(self, x, y, pen);	}
	void Step(mglData *y, const char *pen="")
	{	mgl_step(self, y, pen);	}
	void Bars(mglData *x, mglData *y, mglData *z, const char *pen="")
	{	mgl_bars_xyz(self, x, y, z, pen);	}
	void Bars(mglData *x, mglData *y, const char *pen="")
	{	mgl_bars_xy(self, x, y, pen);	}
	void Bars(mglData *y, const char *pen="")
	{	mgl_bars(self, y, pen);	}

	void Barh(mglData *y, mglData *v, const char *pen="")
	{	mgl_barh_yx(self, y, v, pen);	}
	void Barh(mglData *v, const char *pen="")
	{	mgl_barh(self, v, pen);	}
	void Region(mglData *y1, mglData *y2, const char *pen=NULL, bool inside=true)
	{	mgl_region(self, y1,y2,pen,inside);	}
	void Region(mglData *x, mglData *y1, mglData *y2, const char *pen=NULL, bool inside=true)
	{	mgl_region_xy(self, x,y1,y2,pen,inside);	}

	void Torus(mglData *r, mglData *z, const char *pen="")
	{	mgl_torus(self, r, z, pen);	}
	void Torus(mglData *z, const char *pen="")
	{	mgl_torus_2(self, z, pen);	}
	void Text(mglData *x, mglData *y, mglData *z, const char *text, const char *font="", float size=-1)
	{	mgl_text_xyz(self, x, y, z, text, font, size);	}
	void Text(mglData *x, mglData *y, const char *text, const char *font="", float size=-1)
	{	mgl_text_xy(self, x, y, text, font, size);	}
	void Text(mglData *y, const char *text, const char *font="", float size=-1)
	{	mgl_text_y(self, y, text, font, size);	}
	void Chart(mglData *a, const char *colors="")
	{	mgl_chart(self, a, colors);	}
	void Error(mglData *y, mglData *ey, const char *pen="")
	{	mgl_error(self, y, ey, pen);	}
	void Error(mglData *x, mglData *y, mglData *ey, const char *pen="")
	{	mgl_error_xy(self, x, y, ey, pen);	}
	void Error(mglData *x, mglData *y, mglData *ex, mglData *ey, const char *pen="")
	{	mgl_error_exy(self, x, y, ex, ey, pen);	}
	void Mark(mglData *x, mglData *y, mglData *z, mglData *r, const char *pen)
	{	mgl_mark_xyz(self, x, y, z, r, pen);	}
	void Mark(mglData *x, mglData *y, mglData *r, const char *pen)
	{	mgl_mark_xy(self, x, y, r, pen);	}
	void Mark(mglData *y, mglData *r, const char *pen)
	{	mgl_mark_y(self, y, r, pen);	}
	void TextMark(mglData *x, mglData *y, mglData *z, mglData *r, const char *text, const char *fnt="")
	{	mgl_textmark_xyzr(self, x, y, z, r, text, fnt);	}
	void TextMark(mglData *x, mglData *y, mglData *r, const char *text, const char *fnt="")
	{	mgl_textmark_xyr(self, x, y, r, text, fnt);	}
	void TextMark(mglData *y, mglData *r, const char *text, const char *fnt="")
	{	mgl_textmark_yr(self, y, r, text, fnt);	}
	void TextMark(mglData *y, const char *text, const char *fnt="")
	{	mgl_textmark(self, y, text, fnt);	}
	void Tube(mglData *x, mglData *y, mglData *z, mglData *r, const char *pen="")
	{	mgl_tube_xyzr(self, x, y, z, r, pen);	}
	void Tube(mglData *x, mglData *y, mglData *z, float r, const char *pen="")
	{	mgl_tube_xyz(self, x, y, z, r, pen);	}
	void Tube(mglData *x, mglData *y, mglData *r, const char *pen="")
	{	mgl_tube_xyr(self, x, y, r, pen);	}
	void Tube(mglData *x, mglData *y, float r, const char *pen="")
	{	mgl_tube_xy(self, x, y, r, pen);	}
	void Tube(mglData *y, mglData *r, const char *pen="")
	{	mgl_tube_r(self, y, r, pen);	}
	void Tube(mglData *y, float r, const char *pen="")
	{	mgl_tube(self, y, r, pen);	}

	void Surf(const char *fz, const char *stl="", int n=100)
	{	mgl_fsurf(self, fz, stl, n);	}
	void Surf(const char *fx, const char *fy, const char *fz, const char *stl="", int n=100)
	{	mgl_fsurf_xyz(self, fx, fy, fz, stl, n);	}
	void Grid(mglData *x, mglData *y, mglData *z, const char *stl="", float zVal=NaN)
	{	mgl_grid_xy(self, x, y, z, stl, zVal);	}
	void Grid(mglData *z, const char *stl="", float zVal=NaN)
	{	mgl_grid(self, z, stl, zVal);	}
	void Mesh(mglData *x, mglData *y, mglData *z, const char *stl="")
	{	mgl_mesh_xy(self, x, y, z, stl);	}
	void Mesh(mglData *z, const char *stl="")
	{	mgl_mesh(self, z, stl);	}
	void Fall(mglData *x, mglData *y, mglData *z, const char *stl="")
	{	mgl_fall_xy(self, x, y, z, stl);	}
	void Fall(mglData *z, const char *stl="")
	{	mgl_fall(self, z, stl);	}
	void Belt(mglData *x, mglData *y, mglData *z, const char *stl="")
	{	mgl_belt_xy(self, x, y, z, stl);	}
	void Belt(mglData *z, const char *stl="")
	{	mgl_belt(self, z, stl);	}
	void Surf(mglData *x, mglData *y, mglData *z, const char *stl="")
	{	mgl_surf_xy(self, x, y, z, stl);	}
	void Surf(mglData *z, const char *stl="")
	{	mgl_surf(self, z, stl);	}
	void Tile(mglData *x, mglData *y, mglData *z, const char *stl="")
	{	mgl_tile_xy(self, x, y, z, stl);	}
	void Tile(mglData *z, const char *stl="")
	{	mgl_tile(self, z, stl);	}
	void TileS(mglData *x, mglData *y, mglData *z, mglData *r, const char *stl="")
	{	mgl_tiles_xy(self, x, y, z, r, stl);	}
	void TileS(mglData *z, mglData *r, const char *stl="")
	{	mgl_tiles(self, z, r, stl);	}
	void Dens(mglData *x, mglData *y, mglData *z, const char *stl="", float zVal=NaN)
	{	mgl_dens_xy(self, x, y, z, stl, zVal);	}
	void Dens(mglData *z, const char *stl="", float zVal=NaN)
	{	mgl_dens(self, z, stl, zVal);	}
	void Boxs(mglData *x, mglData *y, mglData *z, const char *stl="", float zVal=NaN)
	{	mgl_boxs_xy(self, x, y, z, stl, zVal);	}
	void Boxs(mglData *z, const char *stl="", float zVal=NaN)
	{	mgl_boxs(self, z, stl, zVal);	}

	void Cont(mglData *v, mglData *x, mglData *y, mglData *z, const char *sch="", float zVal=NaN)
	{	mgl_cont_xy_val(self, v, x, y, z, sch, zVal);	}
	void Cont(mglData *v, mglData *z, const char *sch="", float zVal=NaN)
	{	mgl_cont_val(self, v, z, sch, zVal);	}
	void Cont(mglData *x, mglData *y, mglData *z, const char *sch="", int Num=7, float zVal=NaN)
	{	mgl_cont_xy(self, x, y, z, sch, Num, zVal);	}
	void Cont(mglData *z, const char *sch="", int Num=7, float zVal=NaN)
	{	mgl_cont(self, z, sch, Num, zVal);	}
	
	void ContF(mglData *v, mglData *x, mglData *y, mglData *z, const char *sch="", float zVal=NaN)
	{	mgl_contf_xy_val(self, v, x, y, z, sch, zVal);	}
	void ContF(mglData *v, mglData *z, const char *sch="", float zVal=NaN)
	{	mgl_contf_val(self, v, z, sch, zVal);	}
	void ContF(mglData *x, mglData *y, mglData *z, const char *sch="", int Num=7, float zVal=NaN)
	{	mgl_contf_xy(self, x, y, z, sch, Num, zVal);	}
	void ContF(mglData *z, const char *sch="", int Num=7, float zVal=NaN)
	{	mgl_contf(self, z, sch, Num, zVal);	}
	
	void ContD(mglData *v, mglData *x, mglData *y, mglData *z, const char *sch="", float zVal=NaN)
	{	mgl_contd_xy_val(self, v, x, y, z, sch, zVal);	}
	void ContD(mglData *v, mglData *z, const char *sch="", float zVal=NaN)
	{	mgl_contd_val(self, v, z, sch, zVal);	}
	void ContD(mglData *x, mglData *y, mglData *z, const char *sch="", int Num=7, float zVal=NaN)
	{	mgl_contd_xy(self, x, y, z, sch, Num, zVal);	}
	void ContD(mglData *z, const char *sch="", int Num=7, float zVal=NaN)
	{	mgl_contd(self, z, sch, Num, zVal);	}
	
	void Axial(mglData *v, mglData *x, mglData *y, mglData *z, const char *sch="")
	{	mgl_axial_xy_val(self, v, x, y, z, sch);	}
	void Axial(mglData *v, mglData *z, const char *sch="")
	{	mgl_axial_val(self, v, z, sch);	}
	void Axial(mglData *x, mglData *y, mglData *z, const char *sch="", int Num=3)
	{	mgl_axial_xy(self, x, y, z, sch, Num);	}
	void Axial(mglData *z, const char *sch="", int Num=3)
	{	mgl_axial(self, z, sch, Num);	}

	void SurfC(mglData *x, mglData *y, mglData *z, mglData *c, const char *sch="")
	{	mgl_surfc_xy(self, x, y, z, c, sch);	}
	void SurfC(mglData *z, mglData *c, const char *sch="")
	{	mgl_surfc(self, z, c, sch);	}
	void SurfA(mglData *x, mglData *y, mglData *z, mglData *c, const char *sch="")
	{	mgl_surfa_xy(self, x, y, z, c, sch);	}
	void SurfA(mglData *z, mglData *c, const char *sch="")
	{	mgl_surfa(self, z, c, sch);	}
	void STFA(mglData *x, mglData *y, mglData *re, mglData *im, int dn, const char *sch="", float zVal=NaN)
	{	mgl_stfa_xy(self, x, y, re, im, dn, sch, zVal);	}
	void STFA(mglData *re, mglData *im, int dn, const char *sch="", float zVal=NaN)
	{	mgl_stfa(self, re, im, dn, sch, zVal);	}

	void Dew(mglData *x, mglData *y, mglData *ax, mglData *ay, const char *sch="", float zVal=NaN)
	{	mgl_dew_xy(self, x, y, ax, ay, sch, zVal);	}
	void Dew(mglData *ax, mglData *ay, const char *sch="", float zVal=NaN)
	{	mgl_dew_2d(self, ax, ay, sch, zVal);	}
	void Traj(mglData *x, mglData *y, mglData *ax, mglData *ay, const char *sch="", float zVal=NaN, float len=0)
	{	mgl_traj_xy(self, x, y, ax, ay, sch, zVal, len);	}
	void Traj(mglData *x, mglData *y, mglData *z, mglData *ax, mglData *ay, mglData *az, const char *sch="", float len=0)
	{	mgl_traj_xyz(self, x, y, z, ax, ay, az, sch, len);	}
	void Vect(mglData *x, mglData *y, mglData *ax, mglData *ay, const char *sch="", float zVal=NaN, int flag=0)
	{	mgl_vect_xy(self, x, y, ax, ay, sch, zVal, flag);	}
	void Vect(mglData *ax, mglData *ay, const char *sch="", float zVal=NaN, int flag=0)
	{	mgl_vect_2d(self, ax, ay, sch, zVal, flag);	}
	void VectL(mglData *x, mglData *y, mglData *ax, mglData *ay, const char *sch="", float zVal=NaN)
	{	mgl_vectl_xy(self, x, y, ax, ay, sch, zVal);	}
	void VectL(mglData *ax, mglData *ay, const char *sch="", float zVal=NaN)
	{	mgl_vectl_2d(self, ax, ay, sch, zVal);	}
	void VectC(mglData *x, mglData *y, mglData *ax, mglData *ay, const char *sch="", float zVal=NaN)
	{	mgl_vectc_xy(self, x, y, ax, ay, sch, zVal);	}
	void VectC(mglData *ax, mglData *ay, const char *sch="", float zVal=NaN)
	{	mgl_vectc_2d(self, ax, ay, sch, zVal);	}
	void Vect(mglData *x, mglData *y, mglData *z, mglData *ax, mglData *ay, mglData *az, const char *sch="", int flag=0)
	{	mgl_vect_xyz(self, x, y, z, ax, ay, az, sch, flag);	}
	void Vect(mglData *ax, mglData *ay, mglData *az, const char *sch="", int flag=0)
	{	mgl_vect_3d(self, ax, ay, az, sch, flag);	}
	void VectL(mglData *x, mglData *y, mglData *z, mglData *ax, mglData *ay, mglData *az, const char *sch="")
	{	mgl_vectl_xyz(self, x, y, z, ax, ay, az, sch);	}
	void VectL(mglData *ax, mglData *ay, mglData *az, const char *sch="")
	{	mgl_vectl_3d(self, ax, ay, az, sch);	}
	void VectC(mglData *x, mglData *y, mglData *z, mglData *ax, mglData *ay, mglData *az, const char *sch="")
	{	mgl_vectc_xyz(self, x, y, z, ax, ay, az, sch);	}
	void VectC(mglData *ax, mglData *ay, mglData *az, const char *sch="")
	{	mgl_vectc_3d(self, ax, ay, az, sch);	}
	void Map(mglData *x, mglData *y, mglData *a, mglData *b, const char *sch="", int ks=0, bool pnts=true)
	{	mgl_map_xy(self, x, y, a, b, sch, ks, pnts);	}
	void Map(mglData *a, mglData *b, const char *sch="", int ks=0, bool pnts=true)
	{	mgl_map(self, a, b, sch, ks, pnts);	}

	void Surf3A(float Val, mglData *x, mglData *y, mglData *z, mglData *a, mglData *b, const char *stl="")
	{	mgl_surf3a_xyz_val(self, Val, x, y, z, a, b, stl);	}
	void Surf3A(float Val, mglData *a, mglData *b, const char *stl="")
	{	mgl_surf3a_val(self, Val, a, b, stl);	}
	void Surf3A(mglData *x, mglData *y, mglData *z, mglData *a, mglData *b, const char *stl="", int num=3)
	{	mgl_surf3a_xyz(self, x, y, z, a, b, stl, num);	}
	void Surf3A(mglData *a, mglData *b, const char *stl="", int num=3)
	{	mgl_surf3a(self, a, b, stl, num);	}
	void Surf3C(float Val, mglData *x, mglData *y, mglData *z, mglData *a, mglData *b, const char *stl="")
	{	mgl_surf3c_xyz_val(self, Val, x, y, z, a, b, stl);	}
	void Surf3C(float Val, mglData *a, mglData *b, const char *stl="")
	{	mgl_surf3c_val(self, Val, a, b, stl);	}
	void Surf3C(mglData *x, mglData *y, mglData *z, mglData *a, mglData *b, const char *stl="", int num=3)
	{	mgl_surf3c_xyz(self, x, y, z, a, b, stl, num);	}
	void Surf3C(mglData *a, mglData *b, const char *stl="", int num=3)
	{	mgl_surf3c(self, a, b, stl, num);	}

	void Flow(mglData *x, mglData *y, mglData *ax, mglData *ay, const char *sch="", int num=5, bool central=true, float zVal=NaN)
	{	mgl_flow_xy(self, x, y, ax, ay, sch, num, central, zVal);	}
	void Flow(mglData *ax, mglData *ay, const char *sch="", int num=5, bool central=true, float zVal=NaN)
	{	mgl_flow_2d(self, ax, ay, sch, num, central, zVal);	}
	void Flow(mglData *x, mglData *y, mglData *z, mglData *ax, mglData *ay, mglData *az, const char *sch="", int num=3, bool central=true)
	{	mgl_flow_xyz(self, x, y, z, ax, ay, az, sch, num, central);	}
	void Flow(mglData *ax, mglData *ay, mglData *az, const char *sch="", int num=3, bool central=true)
	{	mgl_flow_3d(self, ax, ay, az, sch, num, central);	}

	void Flow(float x0, float y0, float z0, mglData *x, mglData *y, mglData *ax, mglData *ay, const char *sch="")
	{	mgl_flowp_xy(self, x0, y0, z0, x, y, ax, ay, sch);	}
	void Flow(float x0, float y0, float z0, mglData *ax, mglData *ay, const char *sch="")
	{	mgl_flowp_2d(self, x0, y0, z0, ax, ay, sch);	}
	void Flow(float x0, float y0, float z0, mglData *x, mglData *y, mglData *z, mglData *ax, mglData *ay, mglData *az, const char *sch="")
	{	mgl_flowp_xyz(self, x0, y0, z0, x, y, z, ax, ay, az, sch);	}
	void Flow(float x0, float y0, float z0, mglData *ax, mglData *ay, mglData *az, const char *sch="")
	{	mgl_flowp_3d(self, x0, y0, z0, ax, ay, az, sch);	}

	void Pipe(mglData *x, mglData *y, mglData *ax, mglData *ay, const char *sch="", float r0=0.05, int num=5, bool central=true, float zVal=NaN)
	{	mgl_pipe_xy(self, x, y, ax, ay, sch, r0, num, central, zVal);	}
	void Pipe(mglData *ax, mglData *ay, const char *sch="", float r0=0.05, int num=5, bool central=true, float zVal=NaN)
	{	mgl_pipe_2d(self, ax, ay, sch, r0, num, central, zVal);	}
	void Pipe(mglData *x, mglData *y, mglData *z, mglData *ax, mglData *ay, mglData *az, const char *sch="", float r0=0.05, int num=3, bool central=true)
	{	mgl_pipe_xyz(self, x, y, z, ax, ay, az, sch, r0, num, central);	}
	void Pipe(mglData *ax, mglData *ay, mglData *az, const char *sch="", float r0=0.05, int num=3, bool central=true)
	{	mgl_pipe_3d(self, ax, ay, az, sch, r0, num, central);	}

	void Grid3(mglData *x, mglData *y, mglData *z, mglData *a, char dir, int sVal, const char *stl="")
	{	mgl_grid3_xyz(self, x, y, z, a, dir, sVal, stl);	}
	void Grid3(mglData *a, char dir, int sVal, const char *stl="")
	{	mgl_grid3(self, a, dir, sVal, stl);	}
	void GridA(mglData *x, mglData *y, mglData *z, mglData *a, const char *stl="")
	{	mgl_grid3_all_xyz(self, x, y, z, a, stl);	}
	void GridA(mglData *a, const char *stl="")
	{	mgl_grid3_all(self, a, stl);	}
	void Dens3(mglData *x, mglData *y, mglData *z, mglData *a, char dir, int sVal, const char *stl="")
	{	mgl_dens3_xyz(self, x, y, z, a, dir, sVal, stl);	}
	void Dens3(mglData *a, char dir, int sVal, const char *stl="")
	{	mgl_dens3(self, a, dir, sVal, stl);	}
	void DensA(mglData *x, mglData *y, mglData *z, mglData *a, const char *stl="")
	{	mgl_dens3_all_xyz(self, x, y, z, a, stl);	}
	void DensA(mglData *a, const char *stl="")
	{	mgl_dens3_all(self, a, stl);	}

	void Surf3(float Val, mglData *x, mglData *y, mglData *z, mglData *a, const char *stl="")
	{	mgl_surf3_xyz_val(self, Val, x, y, z, a, stl);	}
	void Surf3(float Val, mglData *a, const char *stl="")
	{	mgl_surf3_val(self, Val, a, stl);	}
	void Surf3(mglData *x, mglData *y, mglData *z, mglData *a, const char *stl="", int num=3)
	{	mgl_surf3_xyz(self, x, y, z, a, stl, num);	}
	void Surf3(mglData *a, const char *stl="", int num=3)
	{	mgl_surf3(self, a, stl, num);	}
	void Cloud(mglData *x, mglData *y, mglData *z, mglData *a, const char *stl="", float alpha=1)
	{	mgl_cloudq_xyz(self, x, y, z, a, stl, alpha);	}
	void Cloud(mglData *a, const char *stl="", float alpha=1)
	{	mgl_cloudq(self, a, stl, alpha);	}

	void Cont3(mglData *v, mglData *x, mglData *y, mglData *z, mglData *a, char dir, int sVal, const char *sch="")
	{	mgl_cont3_xyz_val(self, v, x, y, z, a, dir, sVal, sch);	}
	void Cont3(mglData *v, mglData *a, char dir, int sVal, const char *sch="")
	{	mgl_cont3_val(self, v, a, dir, sVal, sch);	}
	void Cont3(mglData *x, mglData *y, mglData *z, mglData *a, char dir, int sVal, const char *sch="", int Num=7)
	{	mgl_cont3_xyz(self, x, y, z, a, dir, sVal, sch, Num);	}
	void Cont3(mglData *a, char dir, int sVal, const char *sch="", int Num=7)
	{	mgl_cont3(self, a, dir, sVal, sch, Num);	}
	void ContA(mglData *x, mglData *y, mglData *z, mglData *a, const char *sch="", int Num=7)
	{	mgl_cont_all_xyz(self, x, y, z, a, sch, Num);	}
	void ContA(mglData *a, const char *sch="", int Num=7)
	{	mgl_cont_all(self, a, sch, Num);	}

	void ContF3(mglData *v, mglData *x, mglData *y, mglData *z, mglData *a, char dir, int sVal, const char *sch="")
	{	mgl_contf3_xyz_val(self, v, x, y, z, a, dir, sVal, sch);	}
	void ContF3(mglData *v, mglData *a, char dir, int sVal, const char *sch="")
	{	mgl_contf3_val(self, v, a, dir, sVal, sch);	}
	void ContF3(mglData *x, mglData *y, mglData *z, mglData *a, char dir, int sVal, const char *sch="", int Num=7)
	{	mgl_contf3_xyz(self, x, y, z, a, dir, sVal, sch, Num);	}
	void ContF3(mglData *a, char dir, int sVal, const char *sch="", int Num=7)
	{	mgl_contf3(self, a, dir, sVal, sch, Num);	}
	void ContFA(mglData *x, mglData *y, mglData *z, mglData *a, const char *sch="", int Num=7)
	{	mgl_contf_all_xyz(self, x, y, z, a, sch, Num);	}
	void ContFA(mglData *a, const char *sch="", int Num=7)
	{	mgl_contf_all(self, a, sch, Num);	}

	void Beam(mglData *tr, mglData *g1, mglData *g2, mglData *a, float r, const char *stl=0, int flag=0, int num=3)
	{	mgl_beam(self, tr,g1,g2,a,r,stl,flag,num);	}
	void Beam(float val, mglData *tr, mglData *g1, mglData *g2, mglData *a, float r, const char *stl=NULL, int flag=0)
	{	mgl_beam_val(self,val,tr,g1,g2,a,r,stl,flag);	}


	void TriPlot(mglData *nums, mglData *x, mglData *y, mglData *z, mglData *c, const char *sch="")
	{	mgl_triplot_xyzc(self, nums, x, y, z, c, sch);	}
	void TriPlot(mglData *nums, mglData *x, mglData *y, mglData *z, const char *sch="")
	{	mgl_triplot_xyz(self, nums, x, y, z, sch);	}
	void TriPlot(mglData *nums, mglData *x, mglData *y, const char *sch="", float zVal=NaN)
	{	mgl_triplot_xy(self, nums, x, y, sch, zVal);	}
	void Dots(mglData *x, mglData *y, mglData *z, const char *sch="")
	{	mgl_dots(self, x, y, z, sch);	}
	void Dots(mglData *x, mglData *y, mglData *z, mglData *a, const char *sch="")
	{	mgl_dots_a(self, x, y, z, a, sch);	}
	void Crust(mglData *x, mglData *y, mglData *z, const char *sch="", float er=0)
	{	mgl_crust(self, x, y, z, sch, er);	}

	void DensX(mglData *a, const char *stl="", float sVal=NaN)
	{	mgl_dens_x(self, a, stl, sVal);	}
	void DensY(mglData *a, const char *stl="", float sVal=NaN)
	{	mgl_dens_y(self, a, stl, sVal);	}
	void DensZ(mglData *a, const char *stl="", float sVal=NaN)
	{	mgl_dens_z(self, a, stl, sVal);	}
	void ContX(mglData *a, const char *stl="", float sVal=NaN, int Num=7)
	{	mgl_cont_x(self, a, stl, sVal, Num);	}
	void ContY(mglData *a, const char *stl="", float sVal=NaN, int Num=7)
	{	mgl_cont_y(self, a, stl, sVal, Num);	}
	void ContZ(mglData *a, const char *stl="", float sVal=NaN, int Num=7)
	{	mgl_cont_z(self, a, stl, sVal, Num);	}
	void ContX(mglData *v, mglData *a, const char *stl="", float sVal=NaN)
	{	mgl_cont_x_val(self, v, a, stl, sVal);	}
	void ContY(mglData *v, mglData *a, const char *stl="", float sVal=NaN)
	{	mgl_cont_y_val(self, v, a, stl, sVal);	}
	void ContZ(mglData *v, mglData *a, const char *stl="", float sVal=NaN)
	{	mgl_cont_z_val(self, v, a, stl, sVal);	}
	void ContFX(mglData *a, const char *stl="", float sVal=NaN, int Num=7)
	{	mgl_contf_x(self, a, stl, sVal, Num);	}
	void ContFY(mglData *a, const char *stl="", float sVal=NaN, int Num=7)
	{	mgl_contf_y(self, a, stl, sVal, Num);	}
	void ContFZ(mglData *a, const char *stl="", float sVal=NaN, int Num=7)
	{	mgl_contf_z(self, a, stl, sVal, Num);	}
	void ContFX(mglData *v, mglData *a, const char *stl="", float sVal=NaN)
	{	mgl_contf_x_val(self, v, a, stl, sVal);	}
	void ContFY(mglData *v, mglData *a, const char *stl="", float sVal=NaN)
	{	mgl_contf_y_val(self, v, a, stl, sVal);	}
	void ContFZ(mglData *v, mglData *a, const char *stl="", float sVal=NaN)
	{	mgl_contf_z_val(self, v, a, stl, sVal);	}

	float Fit(mglData *fit, mglData *y, const char *eq, const char *var)
	{	return mgl_fit_1(self, fit, y,eq,var,0);	}
	float Fit(mglData *fit, mglData *y, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_1_d(self, fit, y,eq,var,ini);	}
	float Fit2(mglData *fit, mglData *z, const char *eq, const char *var)
	{	return mgl_fit_2(self, fit, z,eq,var,0);	}
	float Fit2(mglData *fit, mglData *z, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_2_d(self, fit, z,eq,var,ini);	}
	float Fit3(mglData *fit, mglData *a, const char *eq, const char *var)
	{	return mgl_fit_3(self, fit, a,eq,var,0);	}
	float Fit3(mglData *fit, mglData *a, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_3_d(self, fit, a,eq,var,ini);	}
	float Fit(mglData *fit, mglData *x, mglData *y, const char *eq, const char *var)
	{	return mgl_fit_xy(self, fit, x,y,eq,var,0);	}
	float Fit(mglData *fit, mglData *x, mglData *y, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_xy_d(self, fit, x,y,eq,var,ini);	}
	float Fit(mglData *fit, mglData *x, mglData *y, mglData *z, const char *eq, const char *var)
	{	return mgl_fit_xyz(self, fit, x,y,z,eq,var,0);	}
	float Fit(mglData *fit, mglData *x, mglData *y, mglData *z, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_xyz_d(self, fit, x,y,z,eq,var,ini);	}
	float Fit(mglData *fit, mglData *x, mglData *y, mglData *z, mglData *a, const char *eq, const char *var)
	{	return mgl_fit_xyza(self, fit, x,y,z,a,eq,var,0);	}
	float Fit(mglData *fit, mglData *x, mglData *y, mglData *z, mglData *a, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_xyza_d(self, fit, x,y,z,a,eq,var,ini);	}
	float FitS(mglData *fit, mglData *y, mglData *s, const char *eq, const char *var)
	{	return mgl_fit_ys(self, fit, y,s,eq,var,0);	}
	float FitS(mglData *fit, mglData *y, mglData *s, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_ys_d(self, fit, y,s,eq,var,ini);	}
	float FitS(mglData *fit, mglData *x, mglData *y, mglData *s, const char *eq, const char *var)
	{	return mgl_fit_xys(self, fit, x,y,s,eq,var,0);	}
	float FitS(mglData *fit, mglData *x, mglData *y, mglData *s, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_xys_d(self, fit, x,y,s,eq,var,ini);	}
	float FitS(mglData *fit, mglData *x, mglData *y, mglData *z, mglData *s, const char *eq, const char *var)
	{	return mgl_fit_xyzs(self, fit, x,y,z,s,eq,var,0);	}
	float FitS(mglData *fit, mglData *x, mglData *y, mglData *z, mglData *s, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_xyzs_d(self, fit, x,y,z,s,eq,var,ini);	}
	float FitS(mglData *fit, mglData *x, mglData *y, mglData *z, mglData *a, mglData *s, const char *eq, const char *var)
	{	return mgl_fit_xyzas(self, fit, x,y,z,a,s,eq,var,0);	}
	float FitS(mglData *fit, mglData *x, mglData *y, mglData *z, mglData *a, mglData *s, const char *eq, const char *var, mglData *ini)
	{	return mgl_fit_xyzas_d(self, fit, x,y,z,a,s,eq,var,ini);	}
	void PutsFit(float x, float y, float z, const char *prefix=0, const char *font=0, float size=-1)
	{	mgl_puts_fit(self, x,y,z, prefix, font, size);	}

	mglData *PDE(const char *ham, mglData *ini_re, mglData *ini_im, float dz=0.1, float k0=100)
	{	return mgl_pde_solve(self,ham,ini_re,ini_im,dz,k0);	}
	void Fill(mglData *u, const char *eq, mglData *v=NULL, mglData *w=NULL)
	{	mgl_data_fill_eq(self, u,eq,v,w);	}
};
