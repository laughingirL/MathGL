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
void mgl_set_size(HMGL gr, int width, int height);
void mgl_set_def_param(HMGL gr);
void mgl_set_quality(HMGL gr, int qual);
void mgl_combine_gr(HMGL gr, HMGL in);

void mgl_set_tick_len(HMGL gr, float len, float stt);
void mgl_set_axis_stl(HMGL gr, const char *stl, const char *tck, const char *sub);
void mgl_adjust_ticks(HMGL gr, const char *dir);
void mgl_set_ticks(HMGL gr, char dir, float d, int ns, float org);
void mgl_set_ticks_str(HMGL gr, char dir, const char *lbl, int add);
void mgl_set_ticks_wcs(HMGL gr, char dir, const wchar_t *lbl, int add);
void mgl_set_ticks_val(HMGL gr, char dir, HCDT val, const char *lbl, int add);
void mgl_set_ticks_valw(HMGL gr, char dir, HCDT val, const wchar_t *lbl, int add);
void mgl_tune_ticks(HMGL gr, int tune, float fact_pos);
void mgl_set_tick_templ(HMGL gr, char dir, const char *templ);
void mgl_set_tick_templw(HMGL gr, char dir, const wchar_t *templ);
void mgl_set_tick_time(HMGL gr, char dir, float d, const char *t);

void mgl_box(HMGL graph);
void mgl_box_str(HMGL gr, const char *col, int ticks);
void mgl_axis(HMGL gr, const char *dir, int adj);
void mgl_axis_grid(HMGL gr, const char *dir,const char *pen);
void mgl_label(HMGL gr, char dir, const char *text);
void mgl_label_ext(HMGL gr, char dir, const char *text, float pos, float shift);
void mgl_labelw_ext(HMGL gr, char dir, const wchar_t *text, float pos, float shift);
void mgl_label_xy(HMGL gr, float x, float y, const char *text, const char *fnt);
void mgl_labelw_xy(HMGL gr, float x, float y, const wchar_t *text, const char *fnt);

void mgl_colorbar(HMGL gr, const char *sch,int where);
void mgl_colorbar_ext(HMGL gr, const char *sch, int where, float x, float y, float w, float h);
void mgl_colorbar_val(HMGL gr, HCDT dat, const char *sch,int where);
void mgl_colorbar_val_ext(HMGL gr, HCDT dat, const char *sch,int where,float x, float y, float w, float h);

void mgl_add_legend(HMGL gr, const char *text,const char *style);
void mgl_add_legendw(HMGL gr, const wchar_t *text,const char *style);
void mgl_clear_legend(HMGL graph);
void mgl_legend_xy(HMGL gr, float x, float y, const char *font, float size, float llen);
void mgl_legend(HMGL gr, int where, const char *font, float size, float llen);
void mgl_set_legend_marks(HMGL gr, int num);


void mgl_show_image(HMGL gr, const char *viewer, int keep);
void mgl_write_frame(HMGL gr, const char *fname,const char *descr);
void mgl_write_tga(HMGL gr, const char *fname,const char *descr);
void mgl_write_bmp(HMGL gr, const char *fname,const char *descr);
void mgl_write_jpg(HMGL gr, const char *fname,const char *descr);
void mgl_write_png(HMGL gr, const char *fname,const char *descr);
void mgl_write_png_solid(HMGL gr, const char *fname,const char *descr);
void mgl_write_bps(HMGL gr, const char *fname,const char *descr);
void mgl_write_eps(HMGL gr, const char *fname,const char *descr);
void mgl_write_svg(HMGL gr, const char *fname,const char *descr);
void mgl_write_tex(HMGL gr, const char *fname,const char *descr);
void mgl_write_obj(HMGL gr, const char *fname,const char *descr);
void mgl_write_wgl(HMGL gr, const char *fname,const char *descr);
void mgl_write_idtf(HMGL gr, const char *fname,const char *descr);
void mgl_write_gif(HMGL gr, const char *fname,const char *descr);
void mgl_start_gif(HMGL gr, const char *fname,int ms);
void mgl_close_gif(HMGL graph);
void mgl_set_plotid(HMGL gr, const char *id);

const unsigned char *mgl_get_rgb(HMGL graph);
const unsigned char *mgl_get_rgba(HMGL graph);
void mgl_set_obj_id(HMGL gr, int id);
int mgl_get_obj_id(HMGL gr, long x, long y);
int mgl_get_spl_id(HMGL gr, long x, long y);
int mgl_get_width(HMGL graph);
int mgl_get_height(HMGL graph);
void mgl_calc_xyz(HMGL gr, int xs, int ys, float *x, float *y, float *z);
void mgl_calc_scr(HMGL gr, float x, float y, float z, int *xs, int *ys);

int mgl_new_frame(HMGL graph);
void mgl_end_frame(HMGL graph);
int mgl_get_num_frame(HMGL graph);
void mgl_reset_frames(HMGL graph);

void mgl_set_transp_type(HMGL gr, int type);
void mgl_set_alpha(HMGL gr, int enable);
void mgl_set_fog(HMGL gr, float d, float dz);
void mgl_set_light(HMGL gr, int enable);
void mgl_set_light_n(HMGL gr, int n, int enable);

void mgl_mat_pop(HMGL gr);
void mgl_mat_push(HMGL gr);
void mgl_clf(HMGL graph);
void mgl_clf_rgb(HMGL gr, float r, float g, float b);

void mgl_subplot(HMGL gr, int nx,int ny,int m);
void mgl_subplot_d(HMGL gr, int nx,int ny,int m, float dx, float dy);
void mgl_subplot_s(HMGL gr, int nx,int ny,int m,const char *style);
void mgl_multiplot(HMGL gr, int nx,int ny,int m,int dx,int dy,const char *style);
void mgl_inplot(HMGL gr, float x1,float x2,float y1,float y2);
void mgl_relplot(HMGL gr, float x1,float x2,float y1,float y2);
void mgl_columnplot(HMGL gr, int num, int ind);
void mgl_columnplot_d(HMGL gr, int num, int ind, float d);
void mgl_stickplot(HMGL gr, int num, int ind, float tet, float phi);
void mgl_title(HMGL gr, const char *title, const char *stl, float size);
void mgl_titlew(HMGL gr, const wchar_t *title, const char *stl, float size);

void mgl_aspect(HMGL gr, float Ax,float Ay,float Az);
void mgl_rotate(HMGL gr, float TetX,float TetZ,float TetY);
void mgl_view(HMGL gr, float TetX,float TetZ,float TetY);
void mgl_rotate_vector(HMGL gr, float Tet,float x,float y,float z);
void mgl_perspective(HMGL gr, float val);

/*****************************************************************************/
uintptr_t mgl_create_graph_(int *width, int *height);
void mgl_delete_graph_(uintptr_t *graph);
void mgl_set_size_(uintptr_t *graph, int *width, int *height);
void mgl_set_def_param_(uintptr_t *gr);
void mgl_set_quality_(uintptr_t *gr, int *qual);
void mgl_combine_gr_(uintptr_t *gr, uintptr_t *in);

void mgl_set_tick_len_(uintptr_t *graph, float *len, float *stt);
void mgl_set_axis_stl_(uintptr_t *graph, const char *stl, const char *tck, const char *sub, int,int,int);

void mgl_adjust_ticks_(uintptr_t *gr, const char *dir, int l);
void mgl_set_ticks_(uintptr_t *gr, char *dir, float *d, int *ns, float *org, int);
void mgl_set_ticks_str_(uintptr_t *gr, const char *dir, const char *lbl, int *add,int,int l);
void mgl_set_ticks_val_(uintptr_t *gr, const char *dir, uintptr_t *val, const char *lbl, int *add,int,int l);
void mgl_tune_ticks_(uintptr_t *gr, int *tune, float *fact_pos);
void mgl_set_tick_templ_(uintptr_t *gr, const char *dir, const char *templ,int,int l);
void mgl_set_tick_time_(uintptr_t *gr, const char *dir, float *d, const char *t,int,int l);
void mgl_box_(uintptr_t *gr);
void mgl_box_str_(uintptr_t *gr, const char *col, int *ticks, int l);
void mgl_axis_(uintptr_t *gr, const char *dir, int *adj,int l);
void mgl_axis_grid_(uintptr_t *gr, const char *dir,const char *pen,int l,int n);
void mgl_label_(uintptr_t *gr, const char *dir, const char *text,int,int l);
void mgl_label_ext_(uintptr_t *gr, const char *dir, const char *text, float *pos, float *shift,int,int l);
void mgl_label_xy_(uintptr_t *gr, float *x, float *y, const char *txt, const char *fnt,int l,int n);
/*****************************************************************************/
void mgl_colorbar_(uintptr_t *gr, const char *sch,int *where,int l);
void mgl_colorbar_ext_(uintptr_t *gr, const char *sch,int *where, float *x, float *y, float *w, float *h, int l);
void mgl_colorbar_val_(uintptr_t *gr, uintptr_t *dat, const char *sch,int *where,int l);
void mgl_colorbar_val_ext_(uintptr_t *gr, uintptr_t *dat, const char *sch,int *where, float *x, float *y, float *w, float *h, int l);

void mgl_add_legend_(uintptr_t *gr, const char *text,const char *style,int l,int n);
void mgl_clear_legend_(uintptr_t *gr);
void mgl_legend_xy_(uintptr_t *gr, float *x, float *y, const char *font, float *size, float *llen,int l);
void mgl_legend_(uintptr_t *gr, int *where, const char *font, float *size, float *llen,int l);
void mgl_set_legend_marks_(uintptr_t *gr, int *num);


void mgl_show_image_(uintptr_t *graph, const char *viewer, int *keep, int);
void mgl_write_frame_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_tga_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
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
void mgl_write_tga_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_obj_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_tex_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_wgl_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_set_plotid_(uintptr_t *gr, const char *id,int l);

const unsigned char *mgl_get_rgb_(uintptr_t *graph);
const unsigned char *mgl_get_rgba_(uintptr_t *graph);
void mgl_set_obj_id_(uintptr_t *graph, int *id);
int mgl_get_obj_id_(uintptr_t *graph, int *x, int *y);
int mgl_get_spl_id_(uintptr_t *graph, int *x, int *y);
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
void mgl_add_light(HMGL gr, int n, float x, float y, float z, char c);
void mgl_add_light_ext_(uintptr_t *graph, int *n, float *x, float *y, float *z, char *c, float *br, int *inf, float *ap, int);
void mgl_add_light_ext(HMGL gr, int n, float x, float y, float z, char c, float br, int inf, float ap);

void mgl_mat_push_(uintptr_t *gr);
void mgl_mat_pop_(uintptr_t *gr);
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

void mgl_title_(uintptr_t *gr, const char *title, const char *stl, float *size, int,int);
void mgl_aspect_(uintptr_t *graph, float *Ax,float *Ay,float *Az);
void mgl_rotate_(uintptr_t *graph, float *TetX,float *TetZ,float *TetY);
void mgl_view_(uintptr_t *graph, float *TetX,float *TetZ,float *TetY);
void mgl_rotate_vector_(uintptr_t *graph, float *Tet,float *x,float *y,float *z);
void mgl_perspective_(uintptr_t *graph, float val);
/*****************************************************************************/
int mgl_fortran_func(HMGL gr, void *);
HMGL mgl_create_graph_qt(int (*draw)(HMGL gr, void *p), const char *title, void *par);
HMGL mgl_create_graph_fltk(int (*draw)(HMGL gr, void *p), const char *title, void *par);
void mgl_fltk_run();
void mgl_qt_run();
/*****************************************************************************/
uintptr_t mgl_create_graph_qt_(const char *title, int);
uintptr_t mgl_create_graph_fltk_(const char *title, int);
void mgl_fltk_run_();
void mgl_qt_run_();
/*****************************************************************************/
void mgl_wnd_set_delay(HMGL gr, float dt);
void mgl_setup_window(HMGL gr, int autoclf, int showpos, int clf_upd);
void mgl_wnd_toggle_alpha(HMGL gr);
void mgl_wnd_toggle_light(HMGL gr);
//void mgl_wnd_toggle_zoom(HMGL gr);
void mgl_wnd_toggle_rotate(HMGL gr);
void mgl_wnd_toggle_no(HMGL gr);
void mgl_wnd_update(HMGL gr);
void mgl_wnd_reload(HMGL gr);
void mgl_wnd_adjust(HMGL gr);
void mgl_wnd_next_frame(HMGL gr);
void mgl_wnd_prev_frame(HMGL gr);
void mgl_wnd_animation(HMGL gr);
void mgl_get_last_mouse_pos(HMGL gr, float *x, float *y, float *z);
/*****************************************************************************/
void mgl_get_last_mouse_pos_(uintptr_t *gr, float *x, float *y, float *z);
void mgl_wnd_set_delay_(uintptr_t *gr, float *dt);
void mgl_setup_window_(uintptr_t *gr, int *autoclf, int *showpos, int *clf_upd);
void mgl_wnd_toggle_alpha_(uintptr_t *gr);
void mgl_wnd_toggle_light_(uintptr_t *gr);
//void mgl_wnd_toggle_zoom_(uintptr_t *gr);
void mgl_wnd_toggle_rotate_(uintptr_t *gr);
void mgl_wnd_toggle_no_(uintptr_t *gr);
void mgl_wnd_update_(uintptr_t *gr);
void mgl_wnd_reload_(uintptr_t *gr);
void mgl_wnd_adjust_(uintptr_t *gr);
void mgl_wnd_next_frame_(uintptr_t *gr);
void mgl_wnd_prev_frame_(uintptr_t *gr);
void mgl_wnd_animation_(uintptr_t *gr);
/*****************************************************************************/
void mgl_mpi_send(HMGL gr, int id);
void mgl_mpi_recv(HMGL gr, int id);
void mgl_mpi_send_(uintptr_t *gr, int *id);
void mgl_mpi_recv_(uintptr_t *gr, int *id);
/*****************************************************************************/
HMPR mgl_create_parser();
void mgl_delete_parser(HMPR p);
void mgl_add_param(HMPR p, int id, const char *str);
void mgl_add_paramw(HMPR p, int id, const wchar_t *str);
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
HMDT mgl_add_var(HMPR p, const char *name);
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
HMDT mgl_find_var(HMPR p, const char *name);
void mgl_del_var(HMPR p, const char *name);
int mgl_parse(HMGL gr, HMPR p, const char *str, int pos);
int mgl_parsew(HMGL gr, HMPR p, const wchar_t *str, int pos);
void mgl_parse_file(HMGL gr, HMPR p, FILE *fp, int print);
void mgl_parse_text(HMGL gr, HMPR p, const char *str, void (*error)(const char *mes), int high);
void mgl_parsew_text(HMGL gr, HMPR p, const wchar_t *str, void (*error)(const char *mes), int high);
void mgl_restore_once(HMPR p);
void mgl_parser_allow_setsize(HMPR p, int a);
void mgl_parser_stop(HMPR p);
/*****************************************************************************/
uintptr_t mgl_create_parser_();
void mgl_delete_parser_(uintptr_t* p);
void mgl_add_param_(uintptr_t* p, int *id, const char *str, int l);
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
uintptr_t mgl_add_var_(uintptr_t* p, const char *name, int l);
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
uintptr_t mgl_find_var_(uintptr_t* p, const char *name, int l);
void mgl_del_var_(uintptr_t* p, const char *name, int l);
int mgl_parse_(uintptr_t* gr, uintptr_t* p, const char *str, int *pos, int l);
void mgl_parse_text_(uintptr_t* gr, uintptr_t* p, const char *str, int l);
void mgl_restore_once_(uintptr_t* p);
void mgl_parser_allow_setsize_(uintptr_t* p, int *a);
void mgl_parser_stop_(uintptr_t* p);
/*****************************************************************************/
#ifdef __cplusplus
}
#endif
/*****************************************************************************/
#endif

