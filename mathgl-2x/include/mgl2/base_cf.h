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
void mgl_set_warn(HMGL gr, int code, const char *text);
const char *mgl_get_mess(HMGL gr);
int mgl_get_flag(HMGL gr, long flag);
void mgl_set_flag(HMGL gr, int val, long flag);

long mgl_use_graph(HMGL gr, int inc);
void mgl_start_group(HMGL gr, const char *);
void mgl_end_group(HMGL gr);
void mgl_highlight(HMGL gr, int id);

void mgl_set_palette(HMGL gr, const char *colors);
void mgl_set_alpha_default(HMGL graph, mreal alpha);
void mgl_set_bar_width(HMGL graph, mreal width);
void mgl_set_meshnum(HMGL graph, int num);
void mgl_set_facenum(HMGL graph, int num);
void mgl_set_ambbr(HMGL gr, mreal i);
void mgl_set_light_dif(HMGL gr, int enable);

void mgl_set_rdc_acc(HMGL graph, int reduce);
void mgl_set_cut(HMGL graph, int cut);
void mgl_set_cut_box(HMGL gr, mreal x1,mreal y1,mreal z1,mreal x2,mreal y2,mreal z2);
void mgl_set_ranges(HMGL graph, mreal x1, mreal x2, mreal y1, mreal y2, mreal z1, mreal z2);
void mgl_set_range_val(HMGL gr, char dir, mreal v1,mreal v2);
void mgl_set_range_dat(HMGL gr, char dir, HCDT a, int add);
void mgl_zoom_axis(HMGL gr, mreal x1,mreal y1,mreal z1,mreal c1,mreal x2,mreal y2,mreal z2,mreal c2);

void mgl_set_origin(HMGL graph, mreal x0, mreal y0, mreal z0);
void mgl_set_func(HMGL graph, const char *EqX,const char *EqY,const char *EqZ,const char *EqA);
void mgl_set_coor(HMGL gr, int how);
void mgl_set_ternary(HMGL gr, int enable);
void mgl_set_cutoff(HMGL graph, const char *EqC);

void mgl_set_tick_rotate(HMGL graph, int enable);
void mgl_set_tick_skip(HMGL graph, int enable);

void mgl_set_mark_size(HMGL graph, mreal size);
void mgl_set_arrow_size(HMGL graph, mreal size);
void mgl_set_font_size(HMGL graph, mreal size);
void mgl_set_font_def(HMGL graph, const char *fnt);
void mgl_set_rotated_text(HMGL graph, int enable);
void mgl_load_font(HMGL gr, const char *name, const char *path);
void mgl_copy_font(HMGL gr, HMGL gr_from);
void mgl_restore_font(HMGL gr);
void mgl_set_color(char id, mreal r, mreal g, mreal b);
void mgl_set_def_sch(HMGL gr, const char *sch);
void mgl_set_def_schv(HMGL gr, HCDT val, const char *sch);
void mgl_clear_unused(HMGL gr);
/******************************************************************************/
void mgl_clear_unused_(uintptr_t *gr);
void mgl_set_def_sch_(uintptr_t *gr, const char *sch,int);
void mgl_set_def_schv_(uintptr_t *gr, uintptr_t *val, const char *sch,int);
void mgl_set_color_(char *id, mreal *r, mreal *g, mreal *b, int);
int mgl_get_warn_(uintptr_t *gr);
void mgl_set_warn_(uintptr_t *gr, int *code, const char *text,int);
long mgl_use_graph_(uintptr_t *gr, int *inc);
void mgl_start_group_(uintptr_t *gr, const char *,int);
void mgl_end_group_(uintptr_t *gr);
void mgl_highlight_(uintptr_t *gr, int *id);

void mgl_set_palette_(uintptr_t *gr, const char *colors, int);
void mgl_set_pal_color_(uintptr_t *graph, int *n, mreal *r, mreal *g, mreal *b);
void mgl_set_pal_num_(uintptr_t *graph, int *num);
void mgl_set_alpha_default_(uintptr_t *graph, mreal *alpha);
void mgl_set_bar_width_(uintptr_t *graph, mreal *width);
void mgl_set_meshnum_(uintptr_t *graph, int *num);
void mgl_set_facenum_(uintptr_t *graph, int *num);
void mgl_set_ambbr_(uintptr_t *gr, mreal *i);
void mgl_set_light_dif_(uintptr_t *graph, int *enable);

void mgl_set_rdc_acc_(uintptr_t *graph, int *reduce);
void mgl_set_cut_(uintptr_t *graph, int *cut);
void mgl_set_cut_box_(uintptr_t *gr, mreal *x1,mreal *y1,mreal *z1,mreal *x2,mreal *y2,mreal *z2);
void mgl_set_ranges_(uintptr_t *graph, mreal *x1, mreal *x2, mreal *y1, mreal *y2, mreal *z1, mreal *z2);
void mgl_set_range_val_(uintptr_t *gr, const char *dir, mreal *v1,mreal *v2,int);
void mgl_set_range_dat_(uintptr_t *gr, const char *dir, uintptr_t *a, int *add,int);
void mgl_zoom_axis_(uintptr_t *gr, mreal *x1,mreal *y1,mreal *z1,mreal *c1,mreal *x2,mreal *y2,mreal *z2,mreal *c2);

void mgl_set_origin_(uintptr_t *gr, mreal *x0, mreal *y0, mreal *z0);
void mgl_set_func_(uintptr_t *graph, const char *EqX, const char *EqY, const char *EqZ, const char *EqA, int, int, int, int);
void mgl_set_coor_(uintptr_t *gr, int *how);
void mgl_set_ternary_(uintptr_t *graph, int *enable);
void mgl_set_cutoff_(uintptr_t *graph, const char *EqC, int);

void mgl_set_tick_rotate_(uintptr_t *graph, int *enable);
void mgl_set_tick_skip_(uintptr_t *graph, int *enable);

void mgl_set_mark_size_(uintptr_t *graph, mreal *size);
void mgl_set_arrow_size_(uintptr_t *graph, mreal *size);
void mgl_set_font_size_(uintptr_t *graph, mreal *size);
void mgl_set_font_def_(uintptr_t *graph, const char *fnt, int);
void mgl_set_rotated_text_(uintptr_t *graph, int *enable);
void mgl_load_font_(uintptr_t *gr, char *name, char *path, int l, int n);
void mgl_copy_font_(uintptr_t *gr, uintptr_t *gr_from);
void mgl_restore_font_(uintptr_t *gr);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif
