/***************************************************************************
 * plot.h is part of Math Graphic Library
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
#include "mgl/base.h"
/*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
void mgl_fplot(HMGL gr, const char *eqY, const char *pen, const char *opt);
void mgl_fplot_xyz(HMGL gr, const char *eqX, const char *eqY, const char *eqZ, const char *pen, const char *opt);
void mgl_fplot_(uintptr_t *gr, const char *fy, const char *stl, const char *opt, int ly, int ls, int lo);
void mgl_fplot_xyz_(uintptr_t *gr, const char *fx, const char *fy, const char *fz, const char *stl, const char *opt, int lx, int ly, int lz, int ls, int lo);
/*****************************************************************************/
void mgl_radar(HMGL graph, HCDT a, const char *pen, const char *opt);
void mgl_radar_(uintptr_t *gr, uintptr_t *a, const char *pen, const char *opt, int l,int lo);
/*****************************************************************************/
void mgl_plot_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, const char *pen, const char *opt);
void mgl_plot_xy(HMGL graph, HCDT x, HCDT y, const char *pen, const char *opt);
void mgl_plot(HMGL graph, HCDT y, const char *pen, const char *opt);
void mgl_plot_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen, const char *opt,int,int);
void mgl_plot_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *pen, const char *opt,int,int);
void mgl_plot_(uintptr_t *graph, uintptr_t *y,	const char *pen, const char *opt,int,int);
/*****************************************************************************/
void mgl_tens_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, HCDT c, const char *pen, const char *opt);
void mgl_tens_xy(HMGL graph, HCDT x, HCDT y, HCDT c, const char *pen, const char *opt);
void mgl_tens(HMGL graph, HCDT y, HCDT c, const char *pen, const char *opt);
void mgl_tens_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *c, const char *pen, const char *opt,int,int);
void mgl_tens_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *c, const char *pen, const char *opt,int,int);
void mgl_tens_(uintptr_t *graph, uintptr_t *y, uintptr_t *c, const char *pen, const char *opt,int,int);
/*****************************************************************************/
void mgl_boxplot_xy(HMGL graph, HCDT x, HCDT a, const char *pen, const char *opt);
void mgl_boxplot(HMGL graph, HCDT a, const char *pen, const char *opt);
/*****************************************************************************/
void mgl_area_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, const char *pen, const char *opt);
void mgl_area_xy(HMGL graph, HCDT x, HCDT y, const char *pen, const char *opt);
void mgl_area(HMGL graph, HCDT y, const char *pen, const char *opt);
/*****************************************************************************/
void mgl_region_xy(HMGL graph, HCDT x, HCDT y1, HCDT y2, const char *pen, const char *opt);
void mgl_region(HMGL graph, HCDT y1, HCDT y2, const char *pen, const char *opt);
/*****************************************************************************/
void mgl_stem_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, const char *pen, const char *opt);
void mgl_stem_xy(HMGL graph, HCDT x, HCDT y, const char *pen, const char *opt);
void mgl_stem(HMGL graph, HCDT y,	const char *pen, const char *opt);
/*****************************************************************************/
void mgl_step_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, const char *pen, const char *opt);
void mgl_step_xy(HMGL graph, HCDT x, HCDT y, const char *pen, const char *opt);
void mgl_step(HMGL graph, HCDT y,	const char *pen, const char *opt);
/*****************************************************************************/
void mgl_bars_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, const char *pen, const char *opt);
void mgl_bars_xy(HMGL graph, HCDT x, HCDT y, const char *pen, const char *opt);
void mgl_bars(HMGL graph, HCDT y,	const char *pen, const char *opt);
/*****************************************************************************/
void mgl_barh_yx(HMGL graph, HCDT y, HCDT v, const char *pen, const char *opt);
void mgl_barh(HMGL graph, HCDT v,	const char *pen, const char *opt);
/*****************************************************************************/
void mgl_chart(HMGL graph, HCDT a, const char *col, const char *opt);
/*****************************************************************************/
void mgl_error_exy(HMGL graph, HCDT x, HCDT y, HCDT ex, HCDT ey, const char *pen, const char *opt);
void mgl_error_xy(HMGL graph, HCDT x, HCDT y, HCDT ey, const char *pen, const char *opt);
void mgl_error(HMGL graph, HCDT y, HCDT ey, const char *pen, const char *opt);
/*****************************************************************************/
void mgl_mark_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, HCDT r, const char *pen, const char *opt);
void mgl_mark_xy(HMGL graph, HCDT x, HCDT y, HCDT r, const char *pen, const char *opt);
void mgl_mark_y(HMGL graph, HCDT y, HCDT r, const char *pen, const char *opt);
/*****************************************************************************/
void mgl_tube_xyzr(HMGL graph, HCDT x, HCDT y, HCDT z, HCDT r, const char *pen, const char *opt);
void mgl_tube_xyr(HMGL graph, HCDT x, HCDT y, HCDT r, const char *pen, const char *opt);
void mgl_tube_r(HMGL graph, HCDT y, HCDT r, const char *pen, const char *opt);
void mgl_tube_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, float r, const char *pen, const char *opt);
void mgl_tube_xy(HMGL graph, HCDT x, HCDT y, float r, const char *penl, const char *opt);
void mgl_tube(HMGL graph, HCDT y, float r, const char *pen, const char *opt);
/*****************************************************************************/
void mgl_boxplot_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *pen, const char *opt,int,int);
void mgl_boxplot_(uintptr_t *graph, uintptr_t *y, const char *pen, const char *opt,int,int);
void mgl_area_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen, const char *opt,int,int);
void mgl_area_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *pen, const char *opt,int,int);
void mgl_area_s_(uintptr_t *graph, uintptr_t *y, const char *pen, const char *opt,int,int);
void mgl_area_(uintptr_t *graph, uintptr_t *y, const char *pen, const char *opt,int,int);
void mgl_region_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y1, uintptr_t *y2, const char *pen, const char *opt,int, int);
void mgl_region_(uintptr_t *graph, uintptr_t *y1, uintptr_t *y2, const char *pen, const char *opt,int, int);
void mgl_mark_(uintptr_t *graph, float *x,float *y,float *z,const char *mark,int);
void mgl_stem_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen, const char *opt,int,int);
void mgl_stem_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *pen, const char *opt,int,int);
void mgl_stem_(uintptr_t *graph, uintptr_t *y,	const char *pen, const char *opt,int,int);
void mgl_step_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen, const char *opt,int,int);
void mgl_step_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *pen, const char *opt,int,int);
void mgl_step_(uintptr_t *graph, uintptr_t *y,	const char *pen, const char *opt,int,int);
void mgl_bars_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen, const char *opt,int,int);
void mgl_bars_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *pen, const char *opt,int,int);
void mgl_bars_(uintptr_t *graph, uintptr_t *y,	const char *pen, const char *opt,int,int);
void mgl_barh_yx_(uintptr_t *graph, uintptr_t *y, uintptr_t *v, const char *pen, const char *opt,int,int);
void mgl_barh_(uintptr_t *graph, uintptr_t *v,	const char *pen, const char *opt,int,int);
void mgl_chart_(uintptr_t *graph, uintptr_t *a, const char *col, const char *opt,int,int);
void mgl_error_(uintptr_t *graph, uintptr_t *y, uintptr_t *ey, const char *pen, const char *opt,int,int);
void mgl_error_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *ey, const char *pen, const char *opt,int,int);
void mgl_error_exy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *ex, uintptr_t *ey, const char *pen, const char *opt,int,int);
void mgl_mark_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *r, const char *pen, const char *opt,int,int);
void mgl_mark_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *r, const char *pen, const char *opt,int,int);
void mgl_mark_y_(uintptr_t *graph, uintptr_t *y, uintptr_t *r, const char *pen, const char *opt,int,int);
void mgl_tube_xyzr_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *r, const char *pen, const char *opt,int,int);
void mgl_tube_xyr_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *r, const char *pen, const char *opt,int,int);
void mgl_tube_r_(uintptr_t *graph, uintptr_t *y, uintptr_t *r, const char *pen, const char *opt,int,int);
void mgl_tube_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, float *r, const char *pen, const char *opt,int,int);
void mgl_tube_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, float *r, const char *pen,int);
void mgl_tube_(uintptr_t *graph, uintptr_t *y, float *r, const char *pen, const char *opt,int,int);
/*****************************************************************************/
#ifdef __cplusplus
}
#endif
/*****************************************************************************/
#endif
