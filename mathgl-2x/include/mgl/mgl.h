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
	bool cl;
public:
	mglGraph(int kind=0, int width=600, int height=400)
	{
		if(kind==1)		gr=mgl_create_graph_gl();
//		else if(kind==2)gr=mgl_create_graph_idtf();
		else	gr=mgl_create_graph(width, height);
		cl = true;
	}
	mglGraph(HMGL graph)	{	gr = graph;		cl = false;	}
	~mglGraph()			{	if(cl)	mgl_delete_graph(gr);	}
	inline HMGL Self()	{	return gr;	}

	inline int  GetWarn()			{	return mgl_get_warn(gr);}
	inline void SetWarn(int code)	{	mgl_set_warn(gr, code);	}
	inline void DefaultPlotParam()	{	mgl_set_def_param(gr);	}

	inline void SetPalette(const char *colors)	{	mgl_set_palette(gr, colors);	}
	inline void SetRotatedText(bool rotated)	{	mgl_set_rotated_text(gr, rotated);	}
	inline void SetCut(bool cut)				{	mgl_set_cut(gr, cut);	}
	inline void SetCutBox(mglPoint p1, mglPoint p2)
	{	mgl_set_cut_box(gr, p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);	}
	inline void SetTickLen(float len, float stt=1)
	{	mgl_set_tick_len(gr, len, stt);	}
	inline void SetTickStl(const char *stl, const char *sub=0)
	{	mgl_set_tick_stl(gr, stl, sub);	}
	inline void SetTickTime(char dir, float d, const char *t="%F")
	{	mgl_set_tick_time(gr,dir,d,t);	}
	inline void SetTicksVal(char dir, const mglDataA &v, const char *lbl)
	{	mgl_set_ticks_val(gr,dir,&v,lbl);	}
	inline void SetTicksVal(char dir, const mglDataA &v, const wchar_t *lbl)
	{	mgl_set_ticks_valw(gr,dir,&v,lbl);	}
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

	inline void Compression(bool){}		// TODO: Add later -- IDTF
	inline void VertexColor(bool){}		// TODO: Add later -- IDTF
	inline void DoubleSided(bool){}		// TODO: Add later -- IDTF
	inline void TextureColor(bool){}	// TODO: Add later -- IDTF
	inline void StartGroup(const char *name)		{	mgl_start_group(gr, name);	}
	inline void EndGroup()					{	mgl_end_group(gr);	}

	inline void SetTranspType(int type)		{	mgl_set_transp_type(gr, type);}
	inline void Alpha(bool enable)			{	mgl_set_alpha(gr, enable);	}
	inline void Fog(float d, float dz=0.25)	{	mgl_set_fog(gr, d, dz);		}
	inline void Light(bool enable)			{	mgl_set_light(gr, enable);	}
	inline void Light(int n,bool enable)	{	mgl_set_light_n(gr, n, enable);	}
	inline void AddLight(int n, mglPoint p, char col='w', float bright=0.5, bool infty=true, float ap=0)
	{	mgl_add_light_ext(gr, n, p.x, p.y, p.z, col, bright, infty, ap);	}
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

	inline void SetRange(char dir, float c1, float c2)
	{	mgl_set_range_val(gr, dir, c1, c2);	}
	inline void SetRange(char dir, const mglDataA &dat, bool add=false)
	{	mgl_set_range_dat(gr, dir, &dat, add);	}
	inline void SetRanges(float x1, float x2, float y1, float y2, float z1=0, float z2=0)
	{	mgl_set_ranges(gr, x1, x2, y1, y2, z1, z2);	}
	inline void SetRanges(mglPoint p1, mglPoint p2)
	{	mgl_set_ranges(gr, p1.x, p2.x, p1.y, p2.y, p1.z, p2.z);	}
	inline void SetAutoRanges(float x1, float x2, float y1=0, float y2=0, float z1=0, float z2=0, float c1=0, float c2=0)
	{	mgl_set_auto(gr, x1, x2, y1, y2, z1, z2, c1, c2);	}
	inline void SaveState()	{	mgl_save_state(gr);	}
	inline void LoadState()	{	mgl_load_state(gr);	}


	inline void SetOrigin(mglPoint p)
	{	mgl_set_origin(gr, p.x, p.y, p.z);	}
	inline void SetOrigin(float x0, float y0, float z0=NAN)
	{	mgl_set_origin(gr, x0, y0, z0);	}
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

	inline void SetTickTempl(char dir, const wchar_t *t)
	{	mgl_set_tick_templw(gr,dir,t);	}
	inline void SetTickTempl(char dir, char *t)
	{	mgl_set_tick_templ(gr,dir,t);	}

	inline void Ball(mglPoint p, char c)
	{	char s[3]={'.',c,0};	mgl_mark(gr, p.x, p.y, p.z, s);	}
	inline void Mark(mglPoint p, const char *mark)
	{	mgl_mark(gr, p.x, p.y, p.z, mark);	}
	inline void Line(mglPoint p1, mglPoint p2, const char *pen="B",int n=2)
	{	mgl_line(gr, p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, pen, n);	}

	inline void Face(mglPoint p1, mglPoint p2, mglPoint p3, mglPoint p4, const char *stl="r")
	{	mgl_face(gr, p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z, p4.x, p4.y, p4.z, stl);	}
	inline void FaceX(mglPoint p, float wy, float wz, const char *stl="w", float dx=0, float dy=0)
	{	mgl_facex(gr, p.x, p.y, p.z, wy, wz, stl, dx, dy);	}
	inline void FaceY(mglPoint p, float wx, float wz, const char *stl="w", float dx=0, float dy=0)
	{	mgl_facey(gr, p.x, p.y, p.z, wx, wz, stl, dx, dy);	}
	inline void FaceZ(mglPoint p, float wx, float wy, const char *stl="w", float dx=0, float dy=0)
	{	mgl_facez(gr, p.x, p.y, p.z, wx, wy, stl, dx, dy);	}
	inline void Curve(mglPoint p1, mglPoint d1, mglPoint p2, mglPoint d2, const char *pen="B", int n=100)
	{	mgl_curve(gr, p1.x, p1.y, p1.z, d1.x, d1.y, d1.z, p2.x, p2.y, p2.z, d2.x, d2.y, d2.z, pen, n);	}
	inline void Drop(mglPoint p, mglPoint d, float r, const char *col="r", float shift=1, float ap=1)
	{	mgl_drop(gr, p.x, p.y, p.z, d.x, d.y, d.z, r, col, shift, ap);	}
	inline void Sphere(mglPoint p, float r, const char *col="r")
	{	mgl_sphere(gr, p.x, p.y, p.z, r, col);	}
	inline void Cone(mglPoint p1, mglPoint p2, float r1, float r2=-1, const char *stl="B", bool edge=false)
	{	mgl_cone(gr, p1.x, p1.y, p1.z, p2.x, p2.y, p2.z,r1,r2,stl,edge);	}
	inline void Error(mglPoint p, mglPoint e, const char *pen="k")
	{	mgl_error_box(gr, p.x, p.y, p.z, e.x, e.y, e.z, pen);	}

	inline void Putsw(mglPoint p,const wchar_t *text,const char *font="C",float size=-1)
	{	mgl_putsw(gr, p.x, p.y, p.z, text, font, size);	}
	inline void Puts(mglPoint p,const char *text,const char *font="C",float size=-1)
	{	mgl_puts(gr, p.x, p.y, p.z, text, font, size);	}
	inline void Putsw(mglPoint p, mglPoint d, const wchar_t *text, const char *font="C", float size=-1)
	{	mgl_putsw_dir(gr, p.x, p.y, p.z, d.x, d.y, d.z, text, font, size);	}
	inline void Puts(mglPoint p, mglPoint d, const char *text, const char *font="C", float size=-1)
	{	mgl_puts_dir(gr, p.x, p.y, p.z, d.x, d.y, d.z, text, font, size);	}
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
	inline void Colorbar(const mglDataA &val, const char *sch="",int where=0)
	{	mgl_colorbar_val(gr, &val, sch, where);	}
	inline void Colorbar(const mglDataA &val, const char *sch,int where,float x,float y,float w,float h)
	{	mgl_colorbar_val_ext(gr, &val, sch, where, x,y,w,h);	}

	inline void AddLegend(const char *text,const char *style)
	{	mgl_add_legend(gr, text, style);	}
	inline void AddLegend(const wchar_t *text,const char *style)
	{	mgl_add_legendw(gr, text, style);	}
	inline void ClearLegend()
	{	mgl_clear_legend(gr);	}
	inline void Legend(float x, float y, const char *font="L", float size=-0.8, float llen=0.1)
	{	mgl_legend_xy(gr, x, y, font, size, llen);	}
	inline void Legend(int where=3, const char *font="L", float size=-0.8, float llen=0.1)
	{	mgl_legend(gr, where, font, size, llen);	}

	inline void Plot(const char *fy, const char *stl="")
	{	mgl_fplot(gr, fy, stl, 100);	}
	inline void Plot(const char *fx, const char *fy, const char *fz, const char *stl="")
	{	mgl_fplot_xyz(gr, fx, fy, fz, stl, 100);	}
	inline void Plot(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *pen="")
	{	mgl_plot_xyz(gr, &x, &y, &z, pen);	}
	inline void Plot(const mglDataA &x, const mglDataA &y, const char *pen="")
	{	mgl_plot_xy(gr, &x, &y, pen);	}
	inline void Plot(const mglDataA &y, const char *pen="")
	{	mgl_plot(gr, &y, pen);	}
	inline void BoxPlot(const mglDataA &x, const mglDataA &y, const char *pen="")
	{	mgl_boxplot_xy(gr, &x, &y, pen);	}
	inline void BoxPlot(const mglDataA &y, const char *pen="")
	{	mgl_boxplot(gr, &y, pen);	}
	inline void Radar(const mglDataA &a, const char *pen="", float r=-1)
	{	mgl_radar(gr, &a, pen, r);	}
	inline void Tens(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &c, const char *pen="")
	{	mgl_tens_xyz(gr, &x, &y, &z, &c, pen);	}
	inline void Tens(const mglDataA &x, const mglDataA &y, const mglDataA &c, const char *pen="")
	{	mgl_tens_xy(gr, &x, &y, &c, pen);	}
	inline void Tens(const mglDataA &y, const mglDataA &c, const char *pen="")
	{	mgl_tens(gr, &y, &c, pen);	}
	inline void Area(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *pen="")
	{	mgl_area_xyz(gr, &x, &y, &z, pen);	}
	inline void Area(const mglDataA &x, const mglDataA &y, const char *pen="")
	{	mgl_area_xy(gr, &x, &y, pen);	}
	inline void Area(const mglDataA &y, const char *pen="")
	{	mgl_area(gr, &y, pen);	}
	inline void Stem(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *pen="")
	{	mgl_stem_xyz(gr, &x, &y, &z, pen);	}
	inline void Stem(const mglDataA &x, const mglDataA &y, const char *pen="")
	{	mgl_stem_xy(gr, &x, &y, pen);	}
	inline void Stem(const mglDataA &y, const char *pen="")
	{	mgl_stem(gr, &y, pen);	}
	inline void Step(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *pen="")
	{	mgl_step_xyz(gr, &x, &y, &z, pen);	}
	inline void Step(const mglDataA &x, const mglDataA &y, const char *pen="")
	{	mgl_step_xy(gr, &x, &y, pen);	}
	inline void Step(const mglDataA &y, const char *pen="")
	{	mgl_step(gr, &y, pen);	}
	inline void Bars(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *pen="")
	{	mgl_bars_xyz(gr, &x, &y, &z, pen);	}
	inline void Bars(const mglDataA &x, const mglDataA &y, const char *pen="")
	{	mgl_bars_xy(gr, &x, &y, pen);	}
	inline void Bars(const mglDataA &y, const char *pen="")
	{	mgl_bars(gr, &y, pen);	}

	inline void Barh(const mglDataA &y, const mglDataA &v, const char *pen="")
	{	mgl_barh_yx(gr, &y, &v, pen);	}
	inline void Barh(const mglDataA &v, const char *pen="")
	{	mgl_barh(gr, &v, pen);	}
	inline void Region(const mglDataA &y1, const mglDataA &y2, const char *pen=NULL, bool inside=true)
	{	mgl_region(gr, &y1, &y2, pen, inside);	}
	inline void Region(const mglDataA &x, const mglDataA &y1, const mglDataA &y2, const char *pen=NULL, bool inside=true)
	{	mgl_region_xy(gr, &x, &y1, &y2, pen, inside);	}

	inline void Torus(const mglDataA &r, const mglDataA &z, const char *pen="")
	{	mgl_torus(gr, &r, &z, pen);	}
	inline void Chart(const mglDataA &a, const char *colors="")
	{	mgl_chart(gr, &a, colors);	}
	inline void Error(const mglDataA &y, const mglDataA &ey, const char *pen="")
	{	mgl_error(gr, &y, &ey, pen);	}
	inline void Error(const mglDataA &x, const mglDataA &y, const mglDataA &ey, const char *pen="")
	{	mgl_error_xy(gr, &x, &y, &ey, pen);	}
	inline void Error(const mglDataA &x, const mglDataA &y, const mglDataA &ex, const mglDataA &ey, const char *pen="")
	{	mgl_error_exy(gr, &x, &y, &ex, &ey, pen);	}
	inline void Mark(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &r, const char *pen)
	{	mgl_mark_xyz(gr, &x, &y, &z, &r, pen);	}
	inline void Mark(const mglDataA &x, const mglDataA &y, const mglDataA &r, const char *pen)
	{	mgl_mark_xy(gr, &x, &y, &r, pen);	}
	inline void Mark(const mglDataA &y, const mglDataA &r, const char *pen)
	{	mgl_mark_y(gr, &y, &r, pen);	}
	inline void Tube(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &r, const char *pen="")
	{	mgl_tube_xyzr(gr, &x, &y, &z, &r, pen);	}
	inline void Tube(const mglDataA &x, const mglDataA &y, const mglDataA &z, float r, const char *pen="")
	{	mgl_tube_xyz(gr, &x, &y, &z, r, pen);	}
	inline void Tube(const mglDataA &x, const mglDataA &y, const mglDataA &r, const char *pen="")
	{	mgl_tube_xyr(gr, &x, &y, &r, pen);	}
	inline void Tube(const mglDataA &x, const mglDataA &y, float r, const char *pen="")
	{	mgl_tube_xy(gr, &x, &y, r, pen);	}
	inline void Tube(const mglDataA &y, const mglDataA &r, const char *pen="")
	{	mgl_tube_r(gr, &y, &r, pen);	}
	inline void Tube(const mglDataA &y, float r, const char *pen="")
	{	mgl_tube(gr, &y, r, pen);	}


	inline void TextMark(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &r, const char *text, const char *fnt="")
	{	mgl_textmark_xyzr(gr, &x, &y, &z, &r, text, fnt);	}
	inline void TextMark(const mglDataA &x, const mglDataA &y, const mglDataA &r, const char *text, const char *fnt="")
	{	mgl_textmark_xyr(gr, &x, &y, &r, text, fnt);	}
	inline void TextMark(const mglDataA &y, const mglDataA &r, const char *text, const char *fnt="")
	{	mgl_textmark_yr(gr, &y, &r, text, fnt);	}
	inline void TextMark(const mglDataA &y, const char *text, const char *fnt="")
	{	mgl_textmark(gr, &y, text, fnt);	}
	inline void TextMark(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &r, const wchar_t *text, const char *fnt="")
	{	mgl_textmarkw_xyzr(gr, &x, &y, &z, &r, text, fnt);	}
	inline void TextMark(const mglDataA &x, const mglDataA &y, const mglDataA &r, const wchar_t *text, const char *fnt="")
	{	mgl_textmarkw_xyr(gr, &x, &y, &r, text, fnt);	}
	inline void TextMark(const mglDataA &y, const mglDataA &r, const wchar_t *text, const char *fnt="")
	{	mgl_textmarkw_yr(gr, &y, &r, text, fnt);	}
	inline void TextMark(const mglDataA &y, const wchar_t *text, const char *fnt="")
	{	mgl_textmarkw(gr, &y, text, fnt);	}
	inline void Text(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *text, const char *font="", float size=-1)
	{	mgl_text_xyz(gr, &x, &y, &z, text, font, size);	}
	inline void Text(const mglDataA &x, const mglDataA &y, const char *text, const char *font="", float size=-1)
	{	mgl_text_xy(gr, &x, &y, text, font, size);	}
	inline void Text(const mglDataA &y, const char *text, const char *font="", float size=-1)
	{	mgl_text_y(gr, &y, text, font, size);	}
	inline void Text(const mglDataA &x, const mglDataA &y, const mglDataA &z, const wchar_t *text, const char *font="", float size=-1)
	{	mgl_textw_xyz(gr, &x, &y, &z, text, font, size);	}
	inline void Text(const mglDataA &x, const mglDataA &y, const wchar_t *text, const char *font="", float size=-1)
	{	mgl_textw_xy(gr, &x, &y, text, font, size);	}
	inline void Text(const mglDataA &y, const wchar_t *text, const char *font="", float size=-1)
	{	mgl_textw_y(gr, &y, text, font, size);	}

	inline void Surf(const char *fz, const char *stl="", int n=100)
	{	mgl_fsurf(gr, fz, stl, n);	}
	inline void Surf(const char *fx, const char *fy, const char *fz, const char *stl="", int n=100)
	{	mgl_fsurf_xyz(gr, fx, fy, fz, stl, n);	}
	inline void Grid(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *stl="", float zVal=NAN)
	{	mgl_grid_xy(gr, &x, &y, &z, stl, zVal);	}
	inline void Grid(const mglDataA &z, const char *stl="", float zVal=NAN)
	{	mgl_grid(gr, &z, stl, zVal);	}
	inline void Mesh(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *stl="")
	{	mgl_mesh_xy(gr, &x, &y, &z, stl);	}
	inline void Mesh(const mglDataA &z, const char *stl="")
	{	mgl_mesh(gr, &z, stl);	}
	inline void Fall(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *stl="")
	{	mgl_fall_xy(gr, &x, &y, &z, stl);	}
	inline void Fall(const mglDataA &z, const char *stl="")
	{	mgl_fall(gr, &z, stl);	}
	inline void Belt(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *stl="")
	{	mgl_belt_xy(gr, &x, &y, &z, stl);	}
	inline void Belt(const mglDataA &z, const char *stl="")
	{	mgl_belt(gr, &z, stl);	}
	inline void Surf(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *stl="")
	{	mgl_surf_xy(gr, &x, &y, &z, stl);	}
	inline void Surf(const mglDataA &z, const char *stl="")
	{	mgl_surf(gr, &z, stl);	}
	inline void Tile(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *stl="")
	{	mgl_tile_xy(gr, &x, &y, &z, stl);	}
	inline void Tile(const mglDataA &z, const char *stl="")
	{	mgl_tile(gr, &z, stl);	}
	inline void TileS(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &r, const char *stl="")
	{	mgl_tiles_xy(gr, &x, &y, &z, &r, stl);	}
	inline void TileS(const mglDataA &z, const mglDataA &r, const char *stl="")
	{	mgl_tiles(gr, &z, &r, stl);	}
	inline void Dens(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *stl="", float zVal=NAN)
	{	mgl_dens_xy(gr, &x, &y, &z, stl, zVal);	}
	inline void Dens(const mglDataA &z, const char *stl="", float zVal=NAN)
	{	mgl_dens(gr, &z, stl, zVal);	}
	inline void Boxs(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *stl="", float zVal=NAN)
	{	mgl_boxs_xy(gr, &x, &y, &z, stl, zVal);	}
	inline void Boxs(const mglDataA &z, const char *stl="", float zVal=NAN)
	{	mgl_boxs(gr, &z, stl, zVal);	}

	inline void Cont(const mglDataA &v, const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *sch="", float zVal=NAN)
	{	mgl_cont_xy_val(gr, &v, &x, &y, &z, sch, zVal);	}
	inline void Cont(const mglDataA &v, const mglDataA &z, const char *sch="", float zVal=NAN)
	{	mgl_cont_val(gr, &v, &z, sch, zVal);	}
	inline void Cont(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *sch="", int Num=7, float zVal=NAN)
	{	mgl_cont_xy(gr, &x, &y, &z, sch, Num, zVal);	}
	inline void Cont(const mglDataA &z, const char *sch="", int Num=7, float zVal=NAN)
	{	mgl_cont(gr, &z, sch, Num, zVal);	}

	inline void ContF(const mglDataA &v, const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *sch="", float zVal=NAN)
	{	mgl_contf_xy_val(gr, &v, &x, &y, &z, sch, zVal);	}
	inline void ContF(const mglDataA &v, const mglDataA &z, const char *sch="", float zVal=NAN)
	{	mgl_contf_val(gr, &v, &z, sch, zVal);	}
	inline void ContF(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *sch="", int Num=7, float zVal=NAN)
	{	mgl_contf_xy(gr, &x, &y, &z, sch, Num, zVal);	}
	inline void ContF(const mglDataA &z, const char *sch="", int Num=7, float zVal=NAN)
	{	mgl_contf(gr, &z, sch, Num, zVal);	}

	inline void ContD(const mglDataA &v, const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *sch="", float zVal=NAN)
	{	mgl_contd_xy_val(gr, &v, &x, &y, &z, sch, zVal);	}
	inline void ContD(const mglDataA &v, const mglDataA &z, const char *sch="", float zVal=NAN)
	{	mgl_contd_val(gr, &v, &z, sch, zVal);	}
	inline void ContD(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *sch="", float zVal=NAN)
	{	mgl_contd_xy(gr, &x, &y, &z, sch, zVal);	}
	inline void ContD(const mglDataA &z, const char *sch="", float zVal=NAN)
	{	mgl_contd(gr, &z, sch, zVal);	}

	inline void Axial(const mglDataA &v, const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *sch="")
	{	mgl_axial_xy_val(gr, &v, &x, &y, &z, sch);	}
	inline void Axial(const mglDataA &v, const mglDataA &z, const char *sch="")
	{	mgl_axial_val(gr, &v, &z, sch);	}
	inline void Axial(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *sch="", int Num=3)
	{	mgl_axial_xy(gr, &x, &y, &z, sch, Num);	}
	inline void Axial(const mglDataA &z, const char *sch="", int Num=3)
	{	mgl_axial(gr, &z, sch, Num);	}

	inline void SurfC(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &c, const char *sch="")
	{	mgl_surfc_xy(gr, &x, &y, &z, &c, sch);	}
	inline void SurfC(const mglDataA &z, const mglDataA &c, const char *sch="")
	{	mgl_surfc(gr, &z, &c, sch);	}
	inline void SurfA(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &c, const char *sch="")
	{	mgl_surfa_xy(gr, &x, &y, &z, &c, sch);	}
	inline void SurfA(const mglDataA &z, const mglDataA &c, const char *sch="")
	{	mgl_surfa(gr, &z, &c, sch);	}
	inline void STFA(const mglDataA &x, const mglDataA &y, const mglDataA &re, const mglDataA &im, int dn, const char *sch="", float zVal=NAN)
	{	mgl_stfa_xy(gr, &x, &y, &re, &im, dn, sch, zVal);	}
	inline void STFA(const mglDataA &re, const mglDataA &im, int dn, const char *sch="", float zVal=NAN)
	{	mgl_stfa(gr, &re, &im, dn, sch, zVal);	}

	inline void Dew(const mglDataA &x, const mglDataA &y, const mglDataA &ax, const mglDataA &ay, const char *sch="", float zVal=NAN)
	{	mgl_dew_xy(gr, &x, &y, &ax, &ay, sch, zVal);	}
	inline void Dew(const mglDataA &ax, const mglDataA &ay, const char *sch="", float zVal=NAN)
	{	mgl_dew_2d(gr, &ax, &ay, sch, zVal);	}
	inline void Traj(const mglDataA &x, const mglDataA &y, const mglDataA &ax, const mglDataA &ay, const char *sch="", float zVal=NAN, float len=0)
	{	mgl_traj_xy(gr, &x, &y, &ax, &ay, sch, zVal, len);	}
	inline void Traj(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &ax, const mglDataA &ay, const mglDataA &az, const char *sch="", float len=0)
	{	mgl_traj_xyz(gr, &x, &y, &z, &ax, &ay, &az, sch, len);	}
	inline void Vect(const mglDataA &x, const mglDataA &y, const mglDataA &ax, const mglDataA &ay, const char *sch="", float zVal=NAN, int flag=0)
	{	mgl_vect_xy(gr, &x, &y, &ax, &ay, sch, zVal, flag);	}
	inline void Vect(const mglDataA &ax, const mglDataA &ay, const char *sch="", float zVal=NAN, int flag=0)
	{	mgl_vect_2d(gr, &ax, &ay, sch, zVal, flag);	}
	inline void Vect(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &ax, const mglDataA &ay, const mglDataA &az, const char *sch="", int flag=0)
	{	mgl_vect_xyz(gr, &x, &y, &z, &ax, &ay, &az, sch, flag);	}
	inline void Vect(const mglDataA &ax, const mglDataA &ay, const mglDataA &az, const char *sch="", int flag=0)
	{	mgl_vect_3d(gr, &ax, &ay, &az, sch, flag);	}
	inline void Map(const mglDataA &x, const mglDataA &y, const mglDataA &a, const mglDataA &b, const char *sch="", int ks=0, bool pnts=true)
	{	mgl_map_xy(gr, &x, &y, &a, &b, sch, ks, pnts);	}
	inline void Map(const mglDataA &a, const mglDataA &b, const char *sch="", int ks=0, bool pnts=true)
	{	mgl_map(gr, &a, &b, sch, ks, pnts);	}

	inline void Surf3A(float Val, const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, const mglDataA &b, const char *stl="")
	{	mgl_surf3a_xyz_val(gr, Val, &x, &y, &z, &a, &b, stl);	}
	inline void Surf3A(float Val, const mglDataA &a, const mglDataA &b, const char *stl="")
	{	mgl_surf3a_val(gr, Val, &a, &b, stl);	}
	inline void Surf3A(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, const mglDataA &b, const char *stl="", int num=3)
	{	mgl_surf3a_xyz(gr, &x, &y, &z, &a, &b, stl, num);	}
	inline void Surf3A(const mglDataA &a, const mglDataA &b, const char *stl="", int num=3)
	{	mgl_surf3a(gr, &a, &b, stl, num);	}
	inline void Surf3C(float Val, const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, const mglDataA &b, const char *stl="")
	{	mgl_surf3c_xyz_val(gr, Val, &x, &y, &z, &a, &b, stl);	}
	inline void Surf3C(float Val, const mglDataA &a, const mglDataA &b, const char *stl="")
	{	mgl_surf3c_val(gr, Val, &a, &b, stl);	}
	inline void Surf3C(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, const mglDataA &b, const char *stl="", int num=3)
	{	mgl_surf3c_xyz(gr, &x, &y, &z, &a, &b, stl, num);	}
	inline void Surf3C(const mglDataA &a, const mglDataA &b, const char *stl="", int num=3)
	{	mgl_surf3c(gr, &a, &b, stl, num);	}

	inline void Flow(const mglDataA &x, const mglDataA &y, const mglDataA &ax, const mglDataA &ay, const char *sch="", int num=5, float zVal=NAN)
	{	mgl_flow_xy(gr, &x, &y, &ax, &ay, sch, num, zVal);	}
	inline void Flow(const mglDataA &ax, const mglDataA &ay, const char *sch="", int num=5, float zVal=NAN)
	{	mgl_flow_2d(gr, &ax, &ay, sch, num, zVal);	}
	inline void Flow(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &ax, const mglDataA &ay, const mglDataA &az, const char *sch="", int num=3)
	{	mgl_flow_xyz(gr, &x, &y, &z, &ax, &ay, &az, sch, num);	}
	inline void Flow(const mglDataA &ax, const mglDataA &ay, const mglDataA &az, const char *sch="", int num=3)
	{	mgl_flow_3d(gr, &ax, &ay, &az, sch, num);	}

	inline void FlowP(mglPoint p, const mglDataA &x, const mglDataA &y, const mglDataA &ax, const mglDataA &ay, const char *sch="")
	{	mgl_flowp_xy(gr, p.x, p.y, p.z, &x, &y, &ax, &ay, sch);	}
	inline void FlowP(mglPoint p, const mglDataA &ax, const mglDataA &ay, const char *sch="")
	{	mgl_flowp_2d(gr, p.x, p.y, p.z, &ax, &ay, sch);	}
	inline void FlowP(mglPoint p, const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &ax, const mglDataA &ay, const mglDataA &az, const char *sch="")
	{	mgl_flowp_xyz(gr, p.x, p.y, p.z, &x, &y, &z, &ax, &ay, &az, sch);	}
	inline void FlowP(mglPoint p, const mglDataA &ax, const mglDataA &ay, const mglDataA &az, const char *sch="")
	{	mgl_flowp_3d(gr, p.x, p.y, p.z, &ax, &ay, &az, sch);	}

	inline void Grad(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &phi, const char *sch=0, int num=3)
	{	mgl_grad_xyz(gr,&x,&y,&z,&phi,sch,num);	}
	inline void Grad(const mglDataA &x, const mglDataA &y, const mglDataA &phi, const char *sch=0, int num=5, mreal zVal=NAN)
	{	mgl_grad_xy(gr,&x,&y,&phi,sch,num,zVal);	}
	inline void Grad(const mglDataA &phi, const char *sch=0, int num=5, mreal zVal=NAN)
	{	mgl_grad(gr,&phi,sch,num,zVal);	}

	inline void Pipe(const mglDataA &x, const mglDataA &y, const mglDataA &ax, const mglDataA &ay, const char *sch="", float r0=0.05, int num=5, float zVal=NAN)
	{	mgl_pipe_xy(gr, &x, &y, &ax, &ay, sch, r0, num, zVal);	}
	inline void Pipe(const mglDataA &ax, const mglDataA &ay, const char *sch="", float r0=0.05, int num=5, float zVal=NAN)
	{	mgl_pipe_2d(gr, &ax, &ay, sch, r0, num, zVal);	}
	inline void Pipe(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &ax, const mglDataA &ay, const mglDataA &az, const char *sch="", float r0=0.05, int num=3)
	{	mgl_pipe_xyz(gr, &x, &y, &z, &ax, &ay, &az, sch, r0, num);	}
	inline void Pipe(const mglDataA &ax, const mglDataA &ay, const mglDataA &az, const char *sch="", float r0=0.05, int num=3)
	{	mgl_pipe_3d(gr, &ax, &ay, &az, sch, r0, num);	}

	inline void Grid3(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, char dir, float sVal=-1, const char *stl="")
	{	mgl_grid3_xyz(gr, &x, &y, &z, &a, dir, sVal, stl);	}
	inline void Grid3(const mglDataA &a, char dir, float sVal=-1, const char *stl="")
	{	mgl_grid3(gr, &a, dir, sVal, stl);	}
	inline void Dens3(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, char dir, float sVal=-1, const char *stl="")
	{	mgl_dens3_xyz(gr, &x, &y, &z, &a, dir, sVal, stl);	}
	inline void Dens3(const mglDataA &a, char dir, float sVal=-1, const char *stl="")
	{	mgl_dens3(gr, &a, dir, sVal, stl);	}

	inline void Surf3(float Val, const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, const char *stl="")
	{	mgl_surf3_xyz_val(gr, Val, &x, &y, &z, &a, stl);	}
	inline void Surf3(float Val, const mglDataA &a, const char *stl="")
	{	mgl_surf3_val(gr, Val, &a, stl);	}
	inline void Surf3(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, const char *stl="", int num=3)
	{	mgl_surf3_xyz(gr, &x, &y, &z, &a, stl, num);	}
	inline void Surf3(const mglDataA &a, const char *stl="", int num=3)
	{	mgl_surf3(gr, &a, stl, num);	}
	inline void Cloud(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, const char *stl="", float alpha=1)
	{	mgl_cloud_xyz(gr, &x, &y, &z, &a, stl, alpha);	}
	inline void Cloud(const mglDataA &a, const char *stl="", float alpha=1)
	{	mgl_cloud(gr, &a, stl, alpha);	}

	inline void Cont3(const mglDataA &v, const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, char dir, float sVal=-1, const char *sch="")
	{	mgl_cont3_xyz_val(gr, &v, &x, &y, &z, &a, dir, sVal, sch);	}
	inline void Cont3(const mglDataA &v, const mglDataA &a, char dir, float sVal=-1, const char *sch="")
	{	mgl_cont3_val(gr, &v, &a, dir, sVal, sch);	}
	inline void Cont3(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, char dir, float sVal=-1, const char *sch="", int Num=7)
	{	mgl_cont3_xyz(gr, &x, &y, &z, &a, dir, sVal, sch, Num);	}
	inline void Cont3(const mglDataA &a, char dir, float sVal=-1, const char *sch="", int Num=7)
	{	mgl_cont3(gr, &a, dir, sVal, sch, Num);	}

	inline void ContF3(const mglDataA &v, const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, char dir, float sVal=-1, const char *sch="")
	{	mgl_contf3_xyz_val(gr, &v, &x, &y, &z, &a, dir, sVal, sch);	}
	inline void ContF3(const mglDataA &v, const mglDataA &a, char dir, float sVal=-1, const char *sch="")
	{	mgl_contf3_val(gr, &v, &a, dir, sVal, sch);	}
	inline void ContF3(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, char dir, float sVal=-1, const char *sch="", int Num=7)
	{	mgl_contf3_xyz(gr, &x, &y, &z, &a, dir, sVal, sch, Num);	}
	inline void ContF3(const mglDataA &a, char dir, float sVal=-1, const char *sch="", int Num=7)
	{	mgl_contf3(gr, &a, dir, sVal, sch, Num);	}

	inline void Beam(const mglDataA &tr, const mglDataA &g1, const mglDataA &g2, const mglDataA &a, float r, const char *stl=0, int flag=0, int num=3)
	{	mgl_beam(gr, &tr,&g1,&g2,&a,r,stl,flag,num);	}
	inline void Beam(float val, const mglDataA &tr, const mglDataA &g1, const mglDataA &g2, const mglDataA &a, float r, const char *stl=NULL, int flag=0)
	{	mgl_beam_val(gr,val,&tr,&g1,&g2,&a,r,stl,flag);	}


	inline void TriPlot(const mglDataA &nums, const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &c, const char *sch="")
	{	mgl_triplot_xyzc(gr, &nums, &x, &y, &z, &c, sch);	}
	inline void TriPlot(const mglDataA &nums, const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *sch="")
	{	mgl_triplot_xyz(gr, &nums, &x, &y, &z, sch);	}
	inline void TriPlot(const mglDataA &nums, const mglDataA &x, const mglDataA &y, const char *sch="", float zVal=NAN)
	{	mgl_triplot_xy(gr, &nums, &x, &y, sch, zVal);	}
	inline void QuadPlot(const mglDataA &nums, const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &c, const char *sch="")
	{	mgl_quadplot_xyzc(gr, &nums, &x, &y, &z, &c, sch);	}
	inline void QuadPlot(const mglDataA &nums, const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *sch="")
	{	mgl_quadplot_xyz(gr, &nums, &x, &y, &z, sch);	}
	inline void QuadPlot(const mglDataA &nums, const mglDataA &x, const mglDataA &y, const char *sch="", float zVal=NAN)
	{	mgl_quadplot_xy(gr, &nums, &x, &y, sch, zVal);	}

	inline void TriCont(const mglDataA &nums, const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *sch=0,int num=7,mreal zVal=NAN)
	{	mgl_tricont_xyc(gr, &nums, &x, &y, &z, sch, num, zVal);	}
	inline void TriContV(const mglDataA &v, const mglDataA &nums, const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *sch=0,mreal zVal=NAN)
	{	mgl_tricont_xycv(gr, &v, &nums, &x, &y, &z, sch, zVal);	}
	inline void TriCont(const mglDataA &nums, const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, const char *sch=0,int num=7,mreal zVal=NAN)
	{	mgl_tricont_xyzc(gr, &nums, &x, &y, &z, &a, sch, num, zVal);	}
	inline void TriContV(const mglDataA &v, const mglDataA &nums, const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, const char *sch=0,mreal zVal=NAN)
	{	mgl_tricont_xyzcv(gr, &v, &nums, &x, &y, &z, &a, sch, zVal);	}

	inline void Dots(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *sch="")
	{	mgl_dots(gr, &x, &y, &z, sch);	}
	inline void Dots(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, const char *sch="")
	{	mgl_dots_a(gr, &x, &y, &z, &a, sch);	}
	inline void Crust(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *sch="", float er=0)
	{	mgl_crust(gr, &x, &y, &z, sch, er);	}

	inline void DensX(const mglDataA &a, const char *stl="", float sVal=NAN)
	{	mgl_dens_x(gr, &a, stl, sVal);	}
	inline void DensY(const mglDataA &a, const char *stl="", float sVal=NAN)
	{	mgl_dens_y(gr, &a, stl, sVal);	}
	inline void DensZ(const mglDataA &a, const char *stl="", float sVal=NAN)
	{	mgl_dens_z(gr, &a, stl, sVal);	}
	inline void ContX(const mglDataA &a, const char *stl="", float sVal=NAN, int Num=7)
	{	mgl_cont_x(gr, &a, stl, sVal, Num);	}
	inline void ContY(const mglDataA &a, const char *stl="", float sVal=NAN, int Num=7)
	{	mgl_cont_y(gr, &a, stl, sVal, Num);	}
	inline void ContZ(const mglDataA &a, const char *stl="", float sVal=NAN, int Num=7)
	{	mgl_cont_z(gr, &a, stl, sVal, Num);	}
	inline void ContX(const mglDataA &v, const mglDataA &a, const char *stl="", float sVal=NAN)
	{	mgl_cont_x_val(gr, &v, &a, stl, sVal);	}
	inline void ContY(const mglDataA &v, const mglDataA &a, const char *stl="", float sVal=NAN)
	{	mgl_cont_y_val(gr, &v, &a, stl, sVal);	}
	inline void ContZ(const mglDataA &v, const mglDataA &a, const char *stl="", float sVal=NAN)
	{	mgl_cont_z_val(gr, &v, &a, stl, sVal);	}
	inline void ContFX(const mglDataA &a, const char *stl="", float sVal=NAN, int Num=7)
	{	mgl_contf_x(gr, &a, stl, sVal, Num);	}
	inline void ContFY(const mglDataA &a, const char *stl="", float sVal=NAN, int Num=7)
	{	mgl_contf_y(gr, &a, stl, sVal, Num);	}
	inline void ContFZ(const mglDataA &a, const char *stl="", float sVal=NAN, int Num=7)
	{	mgl_contf_z(gr, &a, stl, sVal, Num);	}
	inline void ContFX(const mglDataA &v, const mglDataA &a, const char *stl="", float sVal=NAN)
	{	mgl_contf_x_val(gr, &v, &a, stl, sVal);	}
	inline void ContFY(const mglDataA &v, const mglDataA &a, const char *stl="", float sVal=NAN)
	{	mgl_contf_y_val(gr, &v, &a, stl, sVal);	}
	inline void ContFZ(const mglDataA &v, const mglDataA &a, const char *stl="", float sVal=NAN)
	{	mgl_contf_z_val(gr, &v, &a, stl, sVal);	}

	inline mglData Hist(const mglDataA &x, const mglDataA &a)
	{	return mglData(true, mgl_hist_x(gr, &x, &a));	}
	inline mglData Hist(const mglDataA &x, const mglDataA &y, const mglDataA &a)
	{	return mglData(true, mgl_hist_xy(gr, &x, &y, &a));	}
	inline mglData Hist(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a)
	{	return mglData(true, mgl_hist_xyz(gr, &x, &y, &z, &a));	}

	inline mglData Fit(const mglDataA &y, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_1(gr, &y, eq,var,0));	}
	inline mglData Fit(const mglDataA &y, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_1(gr, &y, eq, var, &ini));	}
	inline mglData Fit2(const mglDataA &z, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_2(gr, &z, eq, var,0));	}
	inline mglData Fit2(const mglDataA &z, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_2(gr, &z, eq, var, &ini));	}
	inline mglData Fit3(const mglDataA &a, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_3(gr, &a, eq, var,0));	}
	inline mglData Fit3(const mglDataA &a, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_3(gr, &a, eq, var, &ini));	}
	inline mglData Fit(const mglDataA &x, const mglDataA &y, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_xy(gr, &x, &y, eq, var,0));	}
	inline mglData Fit(const mglDataA &x, const mglDataA &y, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_xy(gr, &x, &y, eq, var, &ini));	}
	inline mglData Fit(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_xyz(gr, &x, &y, &z, eq, var,0));	}
	inline mglData Fit(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_xyz(gr, &x, &y, &z, eq, var, &ini));	}
	inline mglData Fit(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_xyza(gr, &x, &y, &z, &a, eq, var,0));	}
	inline mglData Fit(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_xyza(gr, &x, &y, &z, &a, eq,var, &ini));	}
	inline mglData FitS(const mglDataA &y, const mglDataA &s, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_ys(gr, &y, &s, eq, var,0));	}
	inline mglData FitS(const mglDataA &y, const mglDataA &s, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_ys(gr, &y, &s, eq, var, &ini));	}
	inline mglData FitS(const mglDataA &x, const mglDataA &y, const mglDataA &s, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_xys(gr, &x, &y, &s, eq, var,0));	}
	inline mglData FitS(const mglDataA &x, const mglDataA &y, const mglDataA &s, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_xys(gr, &x, &y, &s, eq, var, &ini));	}
	inline mglData FitS(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &s, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_xyzs(gr, &x, &y, &z, &s, eq, var,0));	}
	inline mglData FitS(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &s, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_xyzs(gr, &x, &y, &z, &s, eq, var, &ini));	}
	inline mglData FitS(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, const mglDataA &s, const char *eq, const char *var)
	{	return mglData(true,mgl_fit_xyzas(gr, &x, &y, &z, &a, &s, eq, var,0));	}
	inline mglData FitS(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &a, const mglDataA &s, const char *eq, const char *var, mglData &ini)
	{	return mglData(true,mgl_fit_xyzas(gr, &x, &y, &z, &a, &s, eq, var, &ini));	}
	inline void PutsFit(mglPoint p, const char *prefix=0, const char *font=0, float size=-1)
	{	mgl_puts_fit(gr, p.x, p.y, p.z, prefix, font, size);	}
	inline const char *GetFit()
	{	return mgl_get_fit(gr);	}

	inline mglData PDE(const char *ham, const mglDataA &ini_re, const mglDataA &ini_im, float dz=0.1, float k0=100)
	{	return mglData(true,mgl_pde_solve(gr,ham,&ini_re,&ini_im,dz,k0));	}
	inline void Fill(mglData &u, const char *eq)
	{	mgl_data_fill_eq(gr, &u, eq, 0, 0);	}
	inline void Fill(mglData &u, const char *eq, const mglDataA &v)
	{	mgl_data_fill_eq(gr, &u, eq, &v, 0);	}
	inline void Fill(mglData &u, const char *eq, const mglDataA &v, const mglDataA &w)
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
