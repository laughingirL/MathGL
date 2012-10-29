/***************************************************************************
 * base_cf.h is part of Math Graphic Library
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
#ifndef _MGL_BASE_CF_H_
#define _MGL_BASE_CF_H_
#include "mgl2/base.h"
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
int mgl_get_warn(HMGL gr);
int mgl_get_warn_(uintptr_t *gr);
void mgl_set_warn(HMGL gr, int code, const char *text);
void mgl_set_warn_(uintptr_t *gr, int *code, const char *text,int);
const char *mgl_get_mess(HMGL gr);
int mgl_get_flag(HMGL gr, long flag);
void mgl_set_flag(HMGL gr, int val, long flag);

long mgl_use_graph(HMGL gr, int inc);
long mgl_use_graph_(uintptr_t *gr, int *inc);
void mgl_start_group(HMGL gr, const char *);
void mgl_start_group_(uintptr_t *gr, const char *,int);
void mgl_end_group(HMGL gr);
void mgl_end_group_(uintptr_t *gr);
void mgl_highlight(HMGL gr, int id);
void mgl_highlight_(uintptr_t *gr, int *id);

void mgl_set_palette(HMGL gr, const char *colors);
void mgl_set_palette_(uintptr_t *gr, const char *colors, int);
void mgl_set_pal_color_(uintptr_t *gr, int *n, double *r, double *g, double *b);
void mgl_set_pal_num_(uintptr_t *gr, int *num);

void mgl_set_alpha_default(HMGL gr, double alpha);
void mgl_set_alpha_default_(uintptr_t *gr, double *alpha);
void mgl_set_bar_width(HMGL gr, double width);
void mgl_set_bar_width_(uintptr_t *gr, double *width);
void mgl_set_meshnum(HMGL gr, int num);
void mgl_set_meshnum_(uintptr_t *gr, int *num);
void mgl_set_facenum(HMGL gr, int num);
void mgl_set_facenum_(uintptr_t *gr, int *num);
void mgl_set_ambbr(HMGL gr, double i);
void mgl_set_ambbr_(uintptr_t *gr, double *i);
void mgl_set_light_dif(HMGL gr, int enable);
void mgl_set_light_dif_(uintptr_t *gr, int *enable);

void mgl_set_rdc_acc(HMGL gr, int reduce);
void mgl_set_rdc_acc_(uintptr_t *gr, int *reduce);
void mgl_set_cut(HMGL gr, int cut);
void mgl_set_cut_(uintptr_t *gr, int *cut);
void mgl_set_cut_box(HMGL gr, double x1,double y1,double z1,double x2,double y2,double z2);
void mgl_set_cut_box_(uintptr_t *gr, double *x1,double *y1,double *z1,double *x2,double *y2,double *z2);

void mgl_set_ranges(HMGL gr, double x1, double x2, double y1, double y2, double z1, double z2);
void mgl_set_ranges_(uintptr_t *gr, double *x1, double *x2, double *y1, double *y2, double *z1, double *z2);
void mgl_set_range_val(HMGL gr, char dir, double v1,double v2);
void mgl_set_range_val_(uintptr_t *gr, const char *dir, double *v1,double *v2,int);
void mgl_set_range_dat(HMGL gr, char dir, HCDT a, int add);
void mgl_set_range_dat_(uintptr_t *gr, const char *dir, uintptr_t *a, int *add,int);
void mgl_zoom_axis(HMGL gr, double x1,double y1,double z1,double c1,double x2,double y2,double z2,double c2);
void mgl_zoom_axis_(uintptr_t *gr, double *x1,double *y1,double *z1,double *c1,double *x2,double *y2,double *z2,double *c2);

void mgl_set_origin(HMGL gr, double x0, double y0, double z0);
void mgl_set_origin_(uintptr_t *gr, double *x0, double *y0, double *z0);
void mgl_set_func(HMGL gr, const char *EqX,const char *EqY,const char *EqZ,const char *EqA);
void mgl_set_func_(uintptr_t *gr, const char *EqX, const char *EqY, const char *EqZ, const char *EqA, int, int, int, int);
void mgl_set_coor(HMGL gr, int how);
void mgl_set_coor_(uintptr_t *gr, int *how);
void mgl_set_ternary(HMGL gr, int enable);
void mgl_set_ternary_(uintptr_t *gr, int *enable);
void mgl_set_cutoff(HMGL gr, const char *EqC);
void mgl_set_cutoff_(uintptr_t *gr, const char *EqC, int);

void mgl_set_tick_rotate(HMGL gr, int enable);
void mgl_set_tick_rotate_(uintptr_t *gr, int *enable);
void mgl_set_tick_skip(HMGL gr, int enable);
void mgl_set_tick_skip_(uintptr_t *gr, int *enable);

void mgl_set_mark_size(HMGL gr, double size);
void mgl_set_mark_size_(uintptr_t *gr, double *size);
void mgl_set_arrow_size(HMGL gr, double size);
void mgl_set_arrow_size_(uintptr_t *gr, double *size);
void mgl_set_font_size(HMGL gr, double size);
void mgl_set_font_size_(uintptr_t *gr, double *size);
void mgl_set_font_def(HMGL gr, const char *fnt);
void mgl_set_font_def_(uintptr_t *gr, const char *fnt, int);
void mgl_set_rotated_text(HMGL gr, int enable);
void mgl_set_rotated_text_(uintptr_t *gr, int *enable);
void mgl_load_font(HMGL gr, const char *name, const char *path);
void mgl_load_font_(uintptr_t *gr, char *name, char *path, int l, int n);
void mgl_copy_font(HMGL gr, HMGL gr_from);
void mgl_copy_font_(uintptr_t *gr, uintptr_t *gr_from);
void mgl_restore_font(HMGL gr);
void mgl_restore_font_(uintptr_t *gr);

void mgl_set_color(char id, double r, double g, double b);
void mgl_set_color_(char *id, double *r, double *g, double *b, int);
void mgl_set_def_sch(HMGL gr, const char *sch);
void mgl_set_def_sch_(uintptr_t *gr, const char *sch,int);
void mgl_set_def_schv(HMGL gr, HCDT val, const char *sch);
void mgl_set_def_schv_(uintptr_t *gr, uintptr_t *val, const char *sch,int);
void mgl_clear_unused(HMGL gr);
void mgl_clear_unused_(uintptr_t *gr);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif
