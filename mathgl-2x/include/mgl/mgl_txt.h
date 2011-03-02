/***************************************************************************
 * mgl_1D.h is part of Math Graphic Library
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
#ifndef _MGL_1D_H_
#define _MGL_1D_H_
#include <stdint.h>
#include "mgl/base.h"
/*****************************************************************************/
#ifdef __cplusplus
class mglBasePlot : public mglBase
{
public:
	/// Draw curve for formula with x in range [Min.x, Max.x]
	void Plot(const char *eqY, const char *pen=0, float zVal=NAN, int n=0);
	/// Draw curve for formulas parametrically depended on t in range [0,1]
	void Plot(const char *eqX, const char *eqY, const char *eqZ, const char *pen=0, int n=0);

	/// Draw line plot for points in arrays x, y, z.
	void Plot(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *pen=0);
	void Plot(const mglDataA &x, const mglDataA &y, const char *pen=0,float zVal=NAN);
	void Plot(const mglDataA &y, const char *pen=0,float zVal=NAN);

	/// Draw radar chart (plot in curved coordinates)
	void Radar(const mglDataA &a, const char *stl=0, float r=-1);

	void BoxPlot(const mglDataA &x, const mglDataA &a, const char *stl=0, float zVal=NAN);
	void BoxPlot(const mglDataA &a, const char *stl=0, float zVal=NAN);

	/// Draw line plot for points in arrays x, y, z which is colored by c (like tension plot). Parameter pen set color scheme and line styles (dashing and width).
	void Tens(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &c, const char *pen=0);
	void Tens(const mglDataA &x, const mglDataA &y, const mglDataA &c, const char *pen=0,float zVal=NAN);
	void Tens(const mglDataA &y, const mglDataA &c, const char *pen=0,float zVal=NAN);

	/// Fill area between curves y1 and y2 (if inside=false) or area for y1<=y<=y2 (if inside=true).
	void Region(const mglDataA &y1, const mglDataA &y2, const char *pen=0, float zVal=NAN, bool inside=true);
	void Region(const mglDataA &x, const mglDataA &y1, const mglDataA &y2, const char *pen=0, float zVal=NAN, bool inside=true);

	/// Draw area plot for points in arrays x, y, z.
	void Area(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *pen=0);
	void Area(const mglDataA &x, const mglDataA &y, const char *pen=0, float zVal=NAN);
	void Area(const mglDataA &y, const char *pen=0, float zVal=NAN);

	/// Draw vertical lines from points in arrays x, y, z to mglGraph::Org.
	void Stem(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *pen=0);
	void Stem(const mglDataA &x, const mglDataA &y, const char *pen=0,float zVal=NAN);
	void Stem(const mglDataA &y, const char *pen=0,float zVal=NAN);

	/// Draw stairs for points in arrays x, y, z.
	void Step(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *pen=0);
	void Step(const mglDataA &x, const mglDataA &y, const char *pen=0,float zVal=NAN);
	void Step(const mglDataA &y, const char *pen=0,float zVal=NAN);

	/// Draw vertical bars from points in arrays x, y, z to mglGraph::Org.
	void Bars(const mglDataA &x, const mglDataA &y, const mglDataA &z, const char *pen=0);
	void Bars(const mglDataA &x, const mglDataA &y, const char *pen=0,float zVal=NAN);
	void Bars(const mglDataA &y, const char *pen=0,float zVal=NAN);

	/// Draw vertical bars from points in arrays x, y to mglGraph::Org.
	void Barh(const mglDataA &y, const mglDataA &v, const char *pen=0,float zVal=NAN);
	void Barh(const mglDataA &v, const char *pen=0,float zVal=NAN);

	/// Draw chart for data a
	void Chart(const mglDataA &a, const char *col=0);

	/// Draw error boxes {ex,ey} for data {x,y}
	void Error(const mglDataA &x, const mglDataA &y, const mglDataA &ex, const mglDataA &ey, const char *pen=0,float zVal=NAN);
	void Error(const mglDataA &x, const mglDataA &y, const mglDataA &ey, const char *pen=0,float zVal=NAN);
	void Error(const mglDataA &y, const mglDataA &ey, const char *pen=0,float zVal=NAN);

	/// Draw marks with diffenernt sizes \a r for points in arrays x, y, z.
	void Mark(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &r, const char *pen=0);
	void Mark(const mglDataA &x, const mglDataA &y, const mglDataA &r, const char *pen=0,float zVal=NAN);
	void Mark(const mglDataA &y, const mglDataA &r, const char *pen=0,float zVal=NAN);

	/// Draw tube with radial sizes \a r for points in arrays x, y, z.
	void Tube(const mglDataA &x, const mglDataA &y, const mglDataA &z, const mglDataA &r, const char *pen=0);
	void Tube(const mglDataA &x, const mglDataA &y, const mglDataA &r, const char *pen=0,float zVal=NAN);
	void Tube(const mglDataA &y, const mglDataA &r, const char *pen=0,float zVal=NAN);
	void Tube(const mglDataA &x, const mglDataA &y, const mglDataA &z, float r, const char *pen=0);
	void Tube(const mglDataA &x, const mglDataA &y, float r, const char *pen=0,float zVal=NAN);
	void Tube(const mglDataA &y, float r, const char *pen=0,float zVal=NAN);

	/// Set relative width of rectangles in Bars, Barh, BoxPlot
	inline void SetBarWidth(float val)	{	BarWidth=val;	};

protected:
	float BarWidth;		///< Relative width of rectangles in mglGraph::Bars().
	void face_plot(mglPoint o, mglPoint d1, mglPoint d2, mglColor c, bool wire);
};
/*****************************************************************************/
extern "C" {
#endif
/*****************************************************************************/
void mgl_fplot(HMGL graph, const char *fy, const char *stl, int n);
void mgl_fplot_xyz(HMGL graph, const char *fx, const char *fy, const char *fz, const char *stl, int n);
void mgl_plot_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, const char *pen);
void mgl_plot_xy(HMGL graph, HCDT x, HCDT y, const char *pen);
void mgl_plot(HMGL graph, HCDT y, const char *pen);
void mgl_radar(HMGL graph, HCDT a, const char *pen, mreal r);
void mgl_boxplot_xy(HMGL graph, HCDT x, HCDT a, const char *pen);
void mgl_boxplot(HMGL graph, HCDT a, const char *pen);
void mgl_tens_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, HCDT c, const char *pen);
void mgl_tens_xy(HMGL graph, HCDT x, HCDT y, HCDT c, const char *pen);
void mgl_tens(HMGL graph, HCDT y, HCDT c,	const char *pen);
void mgl_area_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, const char *pen);
void mgl_area_xy(HMGL graph, HCDT x, HCDT y, const char *pen);
void mgl_area_xys(HMGL graph, HCDT x, HCDT y, const char *pen);
void mgl_area_s(HMGL graph, HCDT y, const char *pen);
void mgl_area(HMGL graph, HCDT y, const char *pen);
void mgl_region_xy(HMGL graph, HCDT x, HCDT y1, HCDT y2, const char *pen, int inside);
void mgl_region(HMGL graph, HCDT y1, HCDT y2, const char *pen, int inside);
void mgl_mark(HMGL graph, mreal x,mreal y,mreal z,char mark);
void mgl_stem_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, const char *pen);
void mgl_stem_xy(HMGL graph, HCDT x, HCDT y, const char *pen);
void mgl_stem(HMGL graph, HCDT y,	const char *pen);
void mgl_step_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, const char *pen);
void mgl_step_xy(HMGL graph, HCDT x, HCDT y, const char *pen);
void mgl_step(HMGL graph, HCDT y,	const char *pen);
void mgl_bars_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, const char *pen);
void mgl_bars_xy(HMGL graph, HCDT x, HCDT y, const char *pen);
void mgl_bars(HMGL graph, HCDT y,	const char *pen);
void mgl_barh_yx(HMGL graph, HCDT y, HCDT v, const char *pen);
void mgl_barh(HMGL graph, HCDT v,	const char *pen);
void mgl_chart(HMGL graph, HCDT a, const char *col);
void mgl_error(HMGL graph, HCDT y, HCDT ey, const char *pen);
void mgl_error_xy(HMGL graph, HCDT x, HCDT y, HCDT ey, const char *pen);
void mgl_error_exy(HMGL graph, HCDT x, HCDT y, HCDT ex, HCDT ey, const char *pen);
void mgl_mark_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, HCDT r, const char *pen);
void mgl_mark_xy(HMGL graph, HCDT x, HCDT y, HCDT r, const char *pen);
void mgl_mark_y(HMGL graph, HCDT y, HCDT r, const char *pen);
void mgl_tube_xyzr(HMGL graph, HCDT x, HCDT y, HCDT z, HCDT r, const char *pen);
void mgl_tube_xyr(HMGL graph, HCDT x, HCDT y, HCDT r, const char *pen);
void mgl_tube_r(HMGL graph, HCDT y, HCDT r, const char *pen);
void mgl_tube_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, mreal r, const char *pen);
void mgl_tube_xy(HMGL graph, HCDT x, HCDT y, mreal r, const char *penl);
void mgl_tube(HMGL graph, HCDT y, mreal r, const char *pen);
/*****************************************************************************/
void mgl_fplot_(uintptr_t *graph, const char *fy, const char *stl, int *n, int ly, int ls);
void mgl_fplot_xyz_(uintptr_t *graph, const char *fx, const char *fy, const char *fz, const char *stl, int *n, int lx, int ly, int lz, int ls);
void mgl_plot_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen,int);
void mgl_plot_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *pen,int);
void mgl_plot_(uintptr_t *graph, uintptr_t *y,	const char *pen,int);
void mgl_radar_(uintptr_t *graph, uintptr_t *a,	const char *pen, mreal *r, int);
void mgl_boxplot_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *pen,int);
void mgl_boxplot_(uintptr_t *graph, uintptr_t *y, const char *pen,int);
void mgl_tens_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *c, const char *pen,int);
void mgl_tens_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *c, const char *pen,int);
void mgl_tens_(uintptr_t *graph, uintptr_t *y, uintptr_t *c, const char *pen,int);
void mgl_area_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen,int);
void mgl_area_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *pen,int);
void mgl_area_s_(uintptr_t *graph, uintptr_t *y, const char *pen,int);
void mgl_area_(uintptr_t *graph, uintptr_t *y, const char *pen,int);
void mgl_region_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y1, uintptr_t *y2, const char *pen, int *inside, int);
void mgl_region_(uintptr_t *graph, uintptr_t *y1, uintptr_t *y2, const char *pen, int *inside, int);
void mgl_mark_(uintptr_t *graph, mreal *x,mreal *y,mreal *z,const char *mark,int);
void mgl_stem_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen,int);
void mgl_stem_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *pen,int);
void mgl_stem_(uintptr_t *graph, uintptr_t *y,	const char *pen,int);
void mgl_step_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen,int);
void mgl_step_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *pen,int);
void mgl_step_(uintptr_t *graph, uintptr_t *y,	const char *pen,int);
void mgl_bars_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen,int);
void mgl_bars_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *pen,int);
void mgl_bars_(uintptr_t *graph, uintptr_t *y,	const char *pen,int);
void mgl_barh_yx_(uintptr_t *graph, uintptr_t *y, uintptr_t *v, const char *pen,int);
void mgl_barh_(uintptr_t *graph, uintptr_t *v,	const char *pen,int);
void mgl_chart_(uintptr_t *graph, uintptr_t *a, const char *col,int);
void mgl_error_(uintptr_t *graph, uintptr_t *y, uintptr_t *ey, const char *pen,int);
void mgl_error_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *ey, const char *pen,int);
void mgl_error_exy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *ex, uintptr_t *ey, const char *pen,int);
void mgl_mark_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *r, const char *pen,int);
void mgl_mark_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *r, const char *pen,int);
void mgl_mark_y_(uintptr_t *graph, uintptr_t *y, uintptr_t *r, const char *pen,int);
void mgl_tube_xyzr_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *r, const char *pen,int);
void mgl_tube_xyr_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *r, const char *pen,int);
void mgl_tube_r_(uintptr_t *graph, uintptr_t *y, uintptr_t *r, const char *pen,int);
void mgl_tube_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, mreal *r, const char *pen,int);
void mgl_tube_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, mreal *r, const char *pen,int);
void mgl_tube_(uintptr_t *graph, uintptr_t *y, mreal *r, const char *pen,int);
/*****************************************************************************/
#ifdef __cplusplus
}
#endif
/*****************************************************************************/
#endif
