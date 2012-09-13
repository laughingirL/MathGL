/***************************************************************************
 * prim.h is part of Math Graphic Library
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
#ifndef _MGL_PRIM_H_
#define _MGL_PRIM_H_
#include "mgl2/base.h"
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
void mgl_mark(HMGL gr, mreal x,mreal y,mreal z,const char *mark);
void mgl_ball(HMGL gr, mreal x,mreal y,mreal z);
/******************************************************************************/
void mgl_line(HMGL gr, mreal x1, mreal y1, mreal z1, mreal x2, mreal y2, mreal z2, const char *pen,int n);
void mgl_curve(HMGL gr, mreal x1, mreal y1, mreal z1, mreal dx1, mreal dy1, mreal dz1, mreal x2, mreal y2, mreal z2, mreal dx2, mreal dy2, mreal dz2, const char *pen,int n);
/******************************************************************************/
void mgl_error_box(HMGL gr, mreal x, mreal y, mreal z, mreal ex, mreal ey, mreal ez, const char *pen);
/******************************************************************************/
void mgl_face(HMGL gr, mreal x0, mreal y0, mreal z0, mreal x1, mreal y1, mreal z1, mreal x2, mreal y2, mreal z2, mreal x3, mreal y3, mreal z3, const char *stl);
void mgl_facex(HMGL gr, mreal x0, mreal y0, mreal z0, mreal wy, mreal wz, const char *stl, mreal dx, mreal dy);
void mgl_facey(HMGL gr, mreal x0, mreal y0, mreal z0, mreal wx, mreal wz, const char *stl, mreal dx, mreal dy);
void mgl_facez(HMGL gr, mreal x0, mreal y0, mreal z0, mreal wx, mreal wy, const char *stl, mreal dx, mreal dy);
/******************************************************************************/
void mgl_sphere(HMGL gr, mreal x, mreal y, mreal z, mreal r, const char *stl);
void mgl_drop(HMGL gr, mreal x1, mreal y1, mreal z1, mreal x2, mreal y2, mreal z2, mreal r, const char *stl, mreal shift, mreal ap);
void mgl_cone(HMGL gr, mreal x1, mreal y1, mreal z1, mreal x2, mreal y2, mreal z2, mreal r1, mreal r2, const char *stl);
void mgl_ellipse(HMGL gr, mreal x1, mreal y1, mreal z1, mreal x2, mreal y2, mreal z2, mreal r, const char *stl);
void mgl_rhomb(HMGL gr, mreal x1, mreal y1, mreal z1, mreal x2, mreal y2, mreal z2, mreal r, const char *stl);
/******************************************************************************/
void mgl_cones_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, const char *pen, const char *opt);
void mgl_cones_xz(HMGL graph, HCDT x, HCDT z, const char *pen, const char *opt);
void mgl_cones(HMGL graph, HCDT z,	const char *pen, const char *opt);
/******************************************************************************/
void mgl_dew_xy(HMGL gr, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch, const char *opt);
void mgl_dew_2d(HMGL gr, HCDT ax, HCDT ay, const char *sch, const char *optl);
/******************************************************************************/
void mgl_puts(HMGL graph, mreal x, mreal y, mreal z,const char *text, const char *font, mreal size);
void mgl_putsw(HMGL graph, mreal x, mreal y, mreal z,const wchar_t *text, const char *font, mreal size);
/******************************************************************************/
void mgl_puts_dir(HMGL graph, mreal x, mreal y, mreal z, mreal dx, mreal dy, mreal dz, const char *text, const char *font, mreal size);
void mgl_putsw_dir(HMGL graph, mreal x, mreal y, mreal z, mreal dx, mreal dy, mreal dz, const wchar_t *text, const char *font, mreal size);
/******************************************************************************/
void mgl_textmark_xyzr(HMGL graph, HCDT x, HCDT y, HCDT z, HCDT r, const char *text, const char *fnt, const char *opt);
void mgl_textmark_xyr(HMGL graph, HCDT x, HCDT y, HCDT r, const char *text, const char *fnt, const char *opt);
void mgl_textmark_yr(HMGL graph, HCDT y, HCDT r, const char *text, const char *fnt, const char *opt);
void mgl_textmark(HMGL graph, HCDT y, const char *text, const char *fnt, const char *opt);
void mgl_textmarkw_xyzr(HMGL graph, HCDT x, HCDT y, HCDT z, HCDT r, const wchar_t *text, const char *fnt, const char *opt);
void mgl_textmarkw_xyr(HMGL graph, HCDT x, HCDT y, HCDT r, const wchar_t *text, const char *fnt, const char *opt);
void mgl_textmarkw_yr(HMGL graph, HCDT y, HCDT r, const wchar_t *text, const char *fnt, const char *opt);
void mgl_textmarkw(HMGL graph, HCDT y, const wchar_t *text, const char *fnt, const char *opt);
/******************************************************************************/
void mgl_label_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, const char *text, const char *fnt, const char *opt);
void mgl_labelw_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, const wchar_t *text, const char *fnt, const char *opt);
void mgl_label_xy(HMGL graph, HCDT x, HCDT y, const char *text, const char *fnt, const char *opt);
void mgl_labelw_xy(HMGL graph, HCDT x, HCDT y, const wchar_t *text, const char *fnt, const char *opt);
void mgl_label_y(HMGL graph, HCDT y, const char *text, const char *fnt, const char *opt);
void mgl_labelw_y(HMGL graph, HCDT y, const wchar_t *text, const char *fnt, const char *opt);
/******************************************************************************/
void mgl_tablew(HMGL gr, HCDT val, const wchar_t *text, const char *fnt, const char *opt);
void mgl_table(HMGL gr, HCDT val, const char *text, const char *fnt, const char *opt);
/******************************************************************************/
/******************************************************************************/
void mgl_mark_(uintptr_t *gr, mreal *x,mreal *y,mreal *z,const char *mark,int);
void mgl_ball_(uintptr_t *gr, mreal *x,mreal *y,mreal *z);
/******************************************************************************/
void mgl_line_(uintptr_t *gr, mreal *x1, mreal *y1, mreal *z1, mreal *x2, mreal *y2, mreal *z2, const char *pen,int *n,int);
void mgl_curve_(uintptr_t* gr, mreal *x1, mreal *y1, mreal *z1, mreal *dx1, mreal *dy1, mreal *dz1, mreal *x2, mreal *y2, mreal *z2, mreal *dx2, mreal *dy2, mreal *dz2, const char *pen,int *n, int l);
/******************************************************************************/
void mgl_error_box_(uintptr_t* gr, mreal *x, mreal *y, mreal *z, mreal *ex, mreal *ey, mreal *ez, const char *pen, int);
/******************************************************************************/
void mgl_face_(uintptr_t* gr, mreal *x0, mreal *y0, mreal *z0, mreal *x1, mreal *y1, mreal *z1, mreal *x2, mreal *y2, mreal *z2, mreal *x3, mreal *y3, mreal *z3, const char *stl, int);
void mgl_facex_(uintptr_t* gr, mreal *x0, mreal *y0, mreal *z0, mreal *wy, mreal *wz, const char *stl, mreal *dx, mreal *dy, int l);
void mgl_facey_(uintptr_t* gr, mreal *x0, mreal *y0, mreal *z0, mreal *wx, mreal *wz, const char *stl, mreal *dx, mreal *dy, int l);
void mgl_facez_(uintptr_t* gr, mreal *x0, mreal *y0, mreal *z0, mreal *wx, mreal *wy, const char *stl, mreal *dx, mreal *dy, int l);
/******************************************************************************/
void mgl_sphere_(uintptr_t* gr, mreal *x, mreal *y, mreal *z, mreal *r, const char *stl, int);
void mgl_drop_(uintptr_t* gr, mreal *x1, mreal *y1, mreal *z1, mreal *x2, mreal *y2, mreal *z2, mreal *r, const char *stl, mreal *shift, mreal *ap, int);
void mgl_cone_(uintptr_t* gr, mreal *x1, mreal *y1, mreal *z1, mreal *x2, mreal *y2, mreal *z2, mreal *r1, mreal *r2, const char *stl, int);
void mgl_ellipse_(uintptr_t* gr, mreal *x1, mreal *y1, mreal *z1, mreal *x2, mreal *y2, mreal *z2, mreal *r, const char *stl, int);
void mgl_rhomb_(uintptr_t* gr, mreal *x1, mreal *y1, mreal *z1, mreal *x2, mreal *y2, mreal *z2, mreal *r, const char *stl, int);
/******************************************************************************/
void mgl_cones_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen, const char *opt,int,int);
void mgl_coners_xz_(uintptr_t *graph, uintptr_t *x, uintptr_t *z, const char *pen, const char *opt,int,int);
void mgl_cones_(uintptr_t *graph, uintptr_t *z,	const char *pen, const char *opt,int,int);
/******************************************************************************/
void mgl_puts_(uintptr_t *graph, mreal *x, mreal *y, mreal *z,const char *text, const char *font, mreal *size, int, int);
void mgl_puts_dir_(uintptr_t *graph, mreal *x, mreal *y, mreal *z, mreal *dx, mreal *dy, mreal *dz, const char *text, const char *font, mreal *size, int, int);
/******************************************************************************/
void mgl_textmark_xyzr_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *r, const char *text, const char *fnt, const char *opt,int,int,int);
void mgl_textmark_xyr_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *r, const char *text, const char *fnt, const char *opt,int,int,int);
void mgl_textmark_yr_(uintptr_t *graph, uintptr_t *y, uintptr_t *r, const char *text, const char *fnt, const char *opt,int,int,int);
void mgl_textmark_(uintptr_t *graph, uintptr_t *y, const char *text, const char *fnt, const char *opt,int,int,int);
/******************************************************************************/
void mgl_label_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *text, const char *fnt, const char *opt,int,int,int);
void mgl_label_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *text, const char *fnt, const char *opt,int,int,int);
void mgl_label_y_(uintptr_t *graph, uintptr_t *y, const char *text, const char *fnt, const char *opt,int,int,int);
/******************************************************************************/
void mgl_table_(uintptr_t *gr, uintptr_t *val, const char *text, const char *fnt, const char *opt,int,int,int);
/******************************************************************************/
void mgl_dew_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch, const char *opt,int,int);
void mgl_dew_2d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, const char *sch, const char *opt,int,int l);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif
