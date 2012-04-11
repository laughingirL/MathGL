/***************************************************************************
 * other.h is part of Math Graphic Library
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
#ifndef _MGL_OTHER_H_
#define _MGL_OTHER_H_
#include "mgl2/base.h"
/*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
void mgl_triplot_xyzc(HMGL gr, HCDT nums, HCDT x, HCDT y, HCDT z, HCDT c, const char *sch, const char *opt);
void mgl_triplot_xyz(HMGL gr, HCDT nums, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt);
void mgl_triplot_xy(HMGL gr, HCDT nums, HCDT x, HCDT y, const char *sch, const char *opt);

void mgl_quadplot_xyzc(HMGL gr, HCDT nums, HCDT x, HCDT y, HCDT z, HCDT c, const char *sch, const char *opt);
void mgl_quadplot_xyz(HMGL gr, HCDT nums, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt);
void mgl_quadplot_xy(HMGL gr, HCDT nums, HCDT x, HCDT y, const char *sch, const char *opt);

void mgl_tricont_xyzcv(HMGL gr, HCDT v, HCDT nums, HCDT x, HCDT y, HCDT z, HCDT c, const char *sch, const char *opt);
void mgl_tricont_xycv(HMGL gr, HCDT v, HCDT nums, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt);
void mgl_tricont_xyzc(HMGL gr, HCDT nums, HCDT x, HCDT y, HCDT z, HCDT c, const char *sch, const char *opt);
void mgl_tricont_xyc(HMGL gr, HCDT nums, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt);

void mgl_dots(HMGL gr, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt);
void mgl_dots_a(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT a, const char *sch, const char *opt);

void mgl_crust(HMGL gr, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt);

void mgl_dens_x(HMGL graph, HCDT a, const char *stl, float sVal, const char *opt);
void mgl_dens_y(HMGL graph, HCDT a, const char *stl, float sVal, const char *opt);
void mgl_dens_z(HMGL graph, HCDT a, const char *stl, float sVal, const char *opt);

void mgl_cont_x(HMGL graph, HCDT a, const char *stl, float sVal, const char *opt);
void mgl_cont_y(HMGL graph, HCDT a, const char *stl, float sVal, const char *opt);
void mgl_cont_z(HMGL graph, HCDT a, const char *stl, float sVal, const char *opt);

void mgl_cont_x_val(HMGL graph, HCDT v, HCDT a, const char *stl, float sVal, const char *opt);
void mgl_cont_y_val(HMGL graph, HCDT v, HCDT a, const char *stl, float sVal, const char *opt);
void mgl_cont_z_val(HMGL graph, HCDT v, HCDT a, const char *stl, float sVal, const char *opt);

void mgl_contf_x(HMGL graph, HCDT a, const char *stl, float sVal, const char *opt);
void mgl_contf_y(HMGL graph, HCDT a, const char *stl, float sVal, const char *opt);
void mgl_contf_z(HMGL graph, HCDT a, const char *stl, float sVal, const char *opt);

void mgl_contf_x_val(HMGL graph, HCDT v, HCDT a, const char *stl, float sVal, const char *opt);
void mgl_contf_y_val(HMGL graph, HCDT v, HCDT a, const char *stl, float sVal, const char *opt);
void mgl_contf_z_val(HMGL graph, HCDT v, HCDT a, const char *stl, float sVal, const char *opt);

/*****************************************************************************/

void mgl_triplot_xyzc_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *c, const char *sch, const char *opt,int,int);
void mgl_triplot_xyz_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int,int);
void mgl_triplot_xy_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, const char *sch, const char *opt,int,int);
void mgl_quadplot_xyzc_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *c, const char *sch, const char *opt,int,int);
void mgl_quadplot_xyz_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int,int);
void mgl_quadplot_xy_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, const char *sch, const char *opt,int,int);

void mgl_tricont_xyzcv_(uintptr_t *gr, uintptr_t *v, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *c, const char *sch, const char *opt,int,int);
void mgl_tricont_xycv_(uintptr_t *gr, uintptr_t *v, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int,int);
void mgl_tricont_xyzc_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *c, const char *sch, const char *opt,int, int);
void mgl_tricont_xyc_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int, int);

void mgl_dots_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int,int);
void mgl_dots_a_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *sch, const char *opt,int,int);
void mgl_dots_tr_(uintptr_t *gr, uintptr_t *tr, const char *sch, const char *opt,int,int);
void mgl_crust_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int,int);
void mgl_crust_tr_(uintptr_t *gr, uintptr_t *tr, const char *sch, const char *opt,int,int);

void mgl_dens_x_(uintptr_t *graph, uintptr_t *a, const char *stl, float *sVal, const char *opt,int,int);
void mgl_dens_y_(uintptr_t *graph, uintptr_t *a, const char *stl, float *sVal, const char *opt,int,int);
void mgl_dens_z_(uintptr_t *graph, uintptr_t *a, const char *stl, float *sVal, const char *opt,int,int);
void mgl_cont_x_(uintptr_t *graph, uintptr_t *a, const char *stl, float *sVal, const char *opt,int,int);
void mgl_cont_y_(uintptr_t *graph, uintptr_t *a, const char *stl, float *sVal, const char *opt,int,int);
void mgl_cont_z_(uintptr_t *graph, uintptr_t *a, const char *stl, float *sVal, const char *opt,int,int);
void mgl_cont_x_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *a, const char *stl, float *sVal, const char *opt,int,int);
void mgl_cont_y_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *a, const char *stl, float *sVal, const char *opt,int,int);
void mgl_cont_z_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *a, const char *stl, float *sVal, const char *opt,int,int);
void mgl_contf_x_(uintptr_t *graph, uintptr_t *a, const char *stl, float *sVal, const char *opt,int,int);
void mgl_contf_y_(uintptr_t *graph, uintptr_t *a, const char *stl, float *sVal, const char *opt,int,int);
void mgl_contf_z_(uintptr_t *graph, uintptr_t *a, const char *stl, float *sVal, const char *opt,int,int);
void mgl_contf_x_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *a, const char *stl, float *sVal, const char *opt,int,int);
void mgl_contf_y_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *a, const char *stl, float *sVal, const char *opt,int,int);
void mgl_contf_z_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *a, const char *stl, float *sVal, const char *opt,int,int);

/*****************************************************************************/
#ifdef __cplusplus
}
#endif
/*****************************************************************************/
#endif
