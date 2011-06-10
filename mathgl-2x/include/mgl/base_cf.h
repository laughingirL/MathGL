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
#include <stdint.h>
#include "mgl/base.h"
/*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
int mgl_get_warn(HMGL gr);
void mgl_set_warn(HMGL gr, int code);
long mgl_use_graph(HMGL gr, int inc);
void mgl_start_group(HMGL gr, const char *);
void mgl_end_group(HMGL gr);
void mgl_test_txt(const char *str, ...);
void mgl_set_test_mode(int enable);

void mgl_set_palette(HMGL gr, const char *colors);
void mgl_set_alpha_default(HMGL graph, float alpha);
void mgl_set_bar_width(HMGL graph, float width);
void mgl_set_meshnum(HMGL graph, int num);
void mgl_set_ambbr(HMGL gr, float i);

void mgl_set_cut(HMGL graph, int cut);
void mgl_set_cut_box(HMGL gr, float x1,float y1,float z1,float x2,float y2,float z2);
void mgl_set_ranges(HMGL graph, float x1, float x2, float y1, float y2, float z1, float z2);
void mgl_set_range_val(HMGL gr, char dir, float v1,float v2);
void mgl_set_range_dat(HMGL gr, char dir, HCDT a, int add);

void mgl_set_origin(HMGL graph, float x0, float y0, float z0);
void mgl_set_auto(HMGL gr, float x1, float x2, float y1, float y2, float z1, float z2, float c1, float c2);
void mgl_set_func(HMGL graph, const char *EqX,const char *EqY,const char *EqZ);
void mgl_set_func_ext(HMGL graph, const char *EqX,const char *EqY,const char *EqZ,const char *EqA);
void mgl_set_coor(HMGL gr, int how);
void mgl_set_ternary(HMGL gr, int enable);
void mgl_set_cutoff(HMGL graph, const char *EqC);

float mgl_data_get_value(HCDT d, long i, long j, long k);
long mgl_data_get_nx(HCDT d);
long mgl_data_get_ny(HCDT d);
long mgl_data_get_nz(HCDT d);

void mgl_set_mark_size(HMGL graph, float size);
void mgl_set_arrow_size(HMGL graph, float size);
void mgl_set_font_size(HMGL graph, float size);
void mgl_set_font_def(HMGL graph, const char *fnt);
void mgl_set_rotated_text(HMGL graph, int rotated);
void mgl_load_font(HMGL gr, const char *name, const char *path);
void mgl_copy_font(HMGL gr, HMGL gr_from);
void mgl_restore_font(HMGL gr);
/*****************************************************************************/
int mgl_get_warn_(uintptr_t *gr);
void mgl_set_warn_(uintptr_t *gr, int *code);
long mgl_use_graph_(uintptr_t *gr, int *inc);
void mgl_start_group_(uintptr_t *gr, const char *,int);
void mgl_end_group_(uintptr_t *gr);

void mgl_set_palette_(uintptr_t *gr, const char *colors, int);
void mgl_set_pal_color_(uintptr_t *graph, int *n, float *r, float *g, float *b);
void mgl_set_pal_num_(uintptr_t *graph, int *num);
void mgl_set_alpha_default_(uintptr_t *graph, float *alpha);
void mgl_set_bar_width_(uintptr_t *graph, float *width);
void mgl_set_meshnum_(uintptr_t *graph, int *num);
void mgl_set_ambbr_(uintptr_t *gr, float *i);

void mgl_set_cut_(uintptr_t *graph, int *cut);
void mgl_set_cut_box_(uintptr_t *gr, float *x1,float *y1,float *z1,float *x2,float *y2,float *z2);
void mgl_set_ranges_(uintptr_t *graph, float *x1, float *x2, float *y1, float *y2, float *z1, float *z2);
void mgl_set_range_val_(uintptr_t *gr, const char *dir, float *v1,float *v2,int);
void mgl_set_range_dat_(uintptr_t *gr, const char *dir, uintptr_t *a, int *add,int);

void mgl_set_origin_(uintptr_t *gr, float *x0, float *y0, float *z0);
void mgl_set_auto_(uintptr_t *gr, float *x1, float *x2, float *y1, float *y2, float *z1, float *z2, float *c1, float *c2);
void mgl_set_func_(uintptr_t *graph, const char *EqX, const char *EqY, const char *EqZ, int, int, int);
void mgl_set_func_ext_(uintptr_t *graph, const char *EqX, const char *EqY, const char *EqZ, const char *EqA, int, int, int, int);
void mgl_set_coor_(uintptr_t *gr, int *how);
void mgl_set_ternary_(uintptr_t *graph, int *enable);
void mgl_set_cutoff_(uintptr_t *graph, const char *EqC, int);

float mgl_data_get_value_(uintptr_t *d, int *i, int *j, int *k);
long mgl_data_get_nx_(uintptr_t *d);
long mgl_data_get_ny_(uintptr_t *d);
long mgl_data_get_nz_(uintptr_t *d);

void mgl_set_mark_size_(uintptr_t *graph, float *size);
void mgl_set_arrow_size_(uintptr_t *graph, float *size);
void mgl_set_font_size_(uintptr_t *graph, float *size);
void mgl_set_font_def_(uintptr_t *graph, const char *fnt, int);
void mgl_set_rotated_text_(uintptr_t *graph, int *rotated);
void mgl_load_font_(uintptr_t *gr, char *name, char *path, int l, int n);
void mgl_copy_font_(uintptr_t *gr, uintptr_t *gr_from);
void mgl_restore_font_(uintptr_t *gr);
/*****************************************************************************/
#ifdef __cplusplus
}
#endif
/*****************************************************************************/
#endif
