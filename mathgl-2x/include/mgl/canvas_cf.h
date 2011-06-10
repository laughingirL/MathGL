/***************************************************************************
 * canvas_cf.h is part of Math Graphic Library
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
#ifndef MGL_CANVAS_CF_H
#define MGL_CANVAS_CF_H
#include "mgl/base.h"
/*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
HMGL mgl_create_graph(int width, int height);
void mgl_delete_graph(HMGL graph);
void mgl_set_size(HMGL graph, int width, int height);
void mgl_set_def_param(HMGL gr);

void mgl_set_tick_len(HMGL graph, float len, float stt);
void mgl_set_axis_stl(HMGL gr, const char *stl, const char *tck, const char *sub);
void mgl_adjust_ticks(HMGL graph, const char *dir);
void mgl_set_ticks(HMGL graph, char dir, float d, int ns, float org);
void mgl_set_ticks_str(HMGL graph, char dir, const char *lbl);
void mgl_set_ticks_wcs(HMGL graph, char dir, const wchar_t *lbl);
void mgl_set_ticks_val(HMGL graph, char dir, HCDT val, const char *lbl);
void mgl_set_ticks_valw(HMGL gr, char dir, HCDT val, const wchar_t *lbl);
void mgl_tune_ticks(HMGL graph, int tune, float fact_pos);
void mgl_set_tick_templ(HMGL graph, char dir, const char *templ);
void mgl_set_tick_templw(HMGL graph, char dir, const wchar_t *templ);
void mgl_set_tick_time(HMGL gr, char dir, float d, const char *t);

void mgl_box(HMGL graph);
void mgl_box_str(HMGL graph, const char *col, int ticks);
void mgl_axis(HMGL graph, const char *dir);
void mgl_axis_grid(HMGL graph, const char *dir,const char *pen);
void mgl_label(HMGL graph, char dir, const char *text);
void mgl_label_ext(HMGL graph, char dir, const char *text, float pos, float shift);
void mgl_labelw_ext(HMGL graph, char dir, const wchar_t *text, float pos, float shift);
void mgl_label_xy(HMGL graph, float x, float y, const char *text, const char *fnt);
void mgl_labelw_xy(HMGL graph, float x, float y, const wchar_t *text, const char *fnt);

void mgl_colorbar(HMGL graph, const char *sch,int where);
void mgl_colorbar_ext(HMGL graph, const char *sch, int where, float x, float y, float w, float h);
void mgl_colorbar_val(HMGL graph, HCDT dat, const char *sch,int where);
void mgl_colorbar_val_ext(HMGL gr, HCDT dat, const char *sch,int where,float x, float y, float w, float h);

void mgl_add_legend(HMGL graph, const char *text,const char *style);
void mgl_add_legendw(HMGL graph, const wchar_t *text,const char *style);
void mgl_clear_legend(HMGL graph);
void mgl_legend_xy(HMGL graph, float x, float y, const char *font, float size, float llen);
void mgl_legend(HMGL graph, int where, const char *font, float size, float llen);
void mgl_set_legend_box(HMGL gr, int enable);
void mgl_set_legend_marks(HMGL gr, int num);


void mgl_show_image(HMGL graph, const char *viewer, int keep);
void mgl_write_frame(HMGL graph, const char *fname,const char *descr);
void mgl_write_bmp(HMGL graph, const char *fname,const char *descr);
void mgl_write_jpg(HMGL graph, const char *fname,const char *descr);
void mgl_write_png(HMGL graph, const char *fname,const char *descr);
void mgl_write_png_solid(HMGL graph, const char *fname,const char *descr);
void mgl_write_eps(HMGL graph, const char *fname,const char *descr);
void mgl_write_svg(HMGL graph, const char *fname,const char *descr);
void mgl_write_idtf(HMGL graph, const char *fname,const char *descr);
void mgl_write_gif(HMGL graph, const char *fname,const char *descr);
void mgl_start_gif(HMGL graph, const char *fname,int ms);
void mgl_close_gif(HMGL graph);

const unsigned char *mgl_get_rgb(HMGL graph);
const unsigned char *mgl_get_rgba(HMGL graph);
int mgl_get_width(HMGL graph);
int mgl_get_height(HMGL graph);
void mgl_calc_xyz(HMGL gr, int xs, int ys, float *x, float *y, float *z);
void mgl_calc_scr(HMGL gr, float x, float y, float z, int *xs, int *ys);

int mgl_new_frame(HMGL graph);
void mgl_end_frame(HMGL graph);
int mgl_get_num_frame(HMGL graph);
void mgl_reset_frames(HMGL graph);

void mgl_set_transp_type(HMGL graph, int type);
void mgl_set_alpha(HMGL graph, int enable);
void mgl_set_fog(HMGL graph, float d, float dz);
void mgl_set_light(HMGL graph, int enable);
void mgl_set_light_n(HMGL gr, int n, int enable);
void mgl_add_light(HMGL graph, int n, float x, float y, float z, char c);
void mgl_add_light_ext(HMGL graph, int n, float x, float y, float z, char c, float br, bool inf, float ap);

void mgl_mat_pop(HMGL gr);
void mgl_mat_push(HMGL gr);
void mgl_identity(HMGL graph, int rel);
void mgl_clf(HMGL graph);
void mgl_clf_rgb(HMGL graph, float r, float g, float b);

void mgl_subplot(HMGL graph, int nx,int ny,int m);
void mgl_subplot_d(HMGL graph, int nx,int ny,int m, float dx, float dy);
void mgl_subplot_s(HMGL graph, int nx,int ny,int m,const char *style);
void mgl_multiplot(HMGL graph, int nx,int ny,int m,int dx,int dy,const char *style);
void mgl_inplot(HMGL graph, float x1,float x2,float y1,float y2);
void mgl_relplot(HMGL graph, float x1,float x2,float y1,float y2);
void mgl_columnplot(HMGL graph, int num, int ind);
void mgl_columnplot_d(HMGL graph, int num, int ind, float d);
void mgl_stickplot(HMGL graph, int num, int ind, float tet, float phi);
void mgl_aspect(HMGL graph, float Ax,float Ay,float Az);
void mgl_rotate(HMGL graph, float TetX,float TetZ,float TetY);
void mgl_rotate_vector(HMGL graph, float Tet,float x,float y,float z);
void mgl_perspective(HMGL graph, float val);

/*****************************************************************************/
uintptr_t mgl_create_graph_(int *width, int *height);
void mgl_delete_graph_(uintptr_t *graph);
void mgl_set_size_(uintptr_t *graph, int *width, int *height);
void mgl_set_def_param_(uintptr_t *gr);

void mgl_set_tick_len_(uintptr_t *graph, float *len, float *stt);
void mgl_set_axis_stl_(uintptr_t *graph, const char *stl, const char *tck, const char *sub, int,int,int);

void mgl_adjust_ticks_(uintptr_t *gr, const char *dir, int l);
void mgl_set_ticks_(uintptr_t *gr, char *dir, float *d, int *ns, float *org, int);
void mgl_set_ticks_str_(uintptr_t *gr, const char *dir, const char *lbl,int,int l);
void mgl_set_ticks_val_(uintptr_t *gr, const char *dir, uintptr_t *val, const char *lbl,int,int l);
void mgl_tune_ticks_(uintptr_t *gr, int *tune, float *fact_pos);
void mgl_set_tick_templ_(uintptr_t *gr, const char *dir, const char *templ,int,int l);
void mgl_set_tick_time_(uintptr_t *gr, const char *dir, float *d, const char *t,int,int l);
void mgl_box_(uintptr_t *gr);
void mgl_box_str_(uintptr_t *gr, const char *col, int *ticks, int l);
void mgl_axis_(uintptr_t *gr, const char *dir,int l);
void mgl_axis_grid_(uintptr_t *gr, const char *dir,const char *pen,int l,int n);
void mgl_label_(uintptr_t *gr, const char *dir, const char *text,int,int l);
void mgl_label_ext_(uintptr_t *gr, const char *dir, const char *text, float *pos, float *shift,int,int l);
void mgl_label_xy_(uintptr_t *gr, float *x, float *y, const char *txt, const char *fnt,int l,int n);
//-----------------------------------------------------------------------------
void mgl_colorbar_(uintptr_t *gr, const char *sch,int *where,int l);
void mgl_colorbar_ext_(uintptr_t *gr, const char *sch,int *where, float *x, float *y, float *w, float *h, int l);
void mgl_colorbar_val_(uintptr_t *gr, uintptr_t *dat, const char *sch,int *where,int l);
void mgl_colorbar_val_ext_(uintptr_t *gr, uintptr_t *dat, const char *sch,int *where, float *x, float *y, float *w, float *h, int l);

void mgl_add_legend_(uintptr_t *gr, const char *text,const char *style,int l,int n);
void mgl_clear_legend_(uintptr_t *gr);
void mgl_legend_xy_(uintptr_t *gr, float *x, float *y, const char *font, float *size, float *llen,int l);
void mgl_legend_(uintptr_t *gr, int *where, const char *font, float *size, float *llen,int l);
void mgl_set_legend_box_(uintptr_t *gr, int *enable);
void mgl_set_legend_marks_(uintptr_t *gr, int *num);


void mgl_show_image_(uintptr_t *graph, const char *viewer, int *keep, int);
void mgl_write_frame_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_bmp_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_jpg_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_png_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_png_solid_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_eps_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_svg_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_idtf_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_gif_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_start_gif_(uintptr_t *graph, const char *fname,int *ms,int l);
void mgl_close_gif_(uintptr_t *graph);

const unsigned char *mgl_get_rgb_(uintptr_t *graph);
const unsigned char *mgl_get_rgba_(uintptr_t *graph);
int mgl_get_width_(uintptr_t *graph);
int mgl_get_height_(uintptr_t *graph);
void mgl_calc_xyz_(uintptr_t *gr, int *xs, int *ys, float *x, float *y, float *z);
void mgl_calc_scr_(uintptr_t *gr, float *x, float *y, float *z, int *xs, int *ys);

int mgl_new_frame_(uintptr_t *graph);
void mgl_end_frame_(uintptr_t *graph);
int mgl_get_num_frame_(uintptr_t *graph);
void mgl_reset_frames_(uintptr_t *graph);

void mgl_set_transp_type_(uintptr_t *graph, int *type);
void mgl_set_alpha_(uintptr_t *graph, int *enable);
void mgl_set_fog_(uintptr_t *graph, float *dist, float *dz);
void mgl_set_light_(uintptr_t *graph, int *enable);
void mgl_set_light_n_(uintptr_t *gr, int *n, int *enable);
void mgl_add_light_(uintptr_t *graph, int *n, float *x, float *y, float *z, char *c, int);
void mgl_add_light(HMGL graph, int n, float x, float y, float z, char c);
void mgl_add_light_ext_(uintptr_t *graph, int *n, float *x, float *y, float *z, char *c, float *br, int *inf, float *ap, int);

void mgl_mat_push_(uintptr_t *gr);
void mgl_mat_pop_(uintptr_t *gr);
void mgl_identity_(uintptr_t *graph, int *rel);
void mgl_clf_(uintptr_t *graph);
void mgl_clf_rgb_(uintptr_t *graph, float *r, float *g, float *b);

void mgl_subplot_(uintptr_t *graph, int *nx,int *ny,int *m);
void mgl_subplot_d_(uintptr_t *graph, int *nx,int *ny,int *m,float *dx,float *dy);
void mgl_subplot_s_(uintptr_t *graph, int *nx,int *ny,int *m, const char *s,int);
void mgl_multiplot_(uintptr_t *graph, int *nx,int *ny,int *m,int *dx,int *dy, const char *s,int);
void mgl_inplot_(uintptr_t *graph, float *x1,float *x2,float *y1,float *y2);
void mgl_relplot_(uintptr_t *graph, float *x1,float *x2,float *y1,float *y2);
void mgl_columnplot_(uintptr_t *graph, int *num, int *i);
void mgl_columnplot_d_(uintptr_t *graph, int *num, int *i, float *d);
void mgl_stickplot_(uintptr_t *graph, int *num, int *i, float *tet, float *phi);
void mgl_aspect_(uintptr_t *graph, float *Ax,float *Ay,float *Az);
void mgl_rotate_(uintptr_t *graph, float *TetX,float *TetZ,float *TetY);
void mgl_rotate_vector_(uintptr_t *graph, float *Tet,float *x,float *y,float *z);
void mgl_perspective_(uintptr_t *graph, float val);


/*void mgl_title(HMGL graph, const char *text, const char *fnt, float size);
void mgl_titlew(HMGL graph, const wchar_t *text, const char *fnt, float size);
void mgl_title_(uintptr_t *graph, const char *text, const char *fnt, float *size, int,int);*/

/*****************************************************************************/
#ifdef __cplusplus
}
#endif
/*****************************************************************************/
#endif

