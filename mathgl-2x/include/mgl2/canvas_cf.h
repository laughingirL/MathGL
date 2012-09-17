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
#include "mgl2/base.h"
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
HMGL mgl_create_graph(int width, int height);
void mgl_delete_graph(HMGL graph);
void mgl_set_size(HMGL gr, int width, int height);
void mgl_set_def_param(HMGL gr);
void mgl_set_quality(HMGL gr, int qual);
void mgl_combine_gr(HMGL gr, HMGL in);
void mgl_finish(HMGL gr);

void mgl_set_tick_len(HMGL gr, mreal len, mreal stt);
void mgl_set_axis_stl(HMGL gr, const char *stl, const char *tck, const char *sub);
void mgl_adjust_ticks(HMGL gr, const char *dir);
void mgl_set_ticks(HMGL gr, char dir, mreal d, int ns, mreal org);
void mgl_set_ticks_str(HMGL gr, char dir, const char *lbl, int add);
void mgl_set_ticks_wcs(HMGL gr, char dir, const wchar_t *lbl, int add);
void mgl_set_ticks_val(HMGL gr, char dir, HCDT val, const char *lbl, int add);
void mgl_set_ticks_valw(HMGL gr, char dir, HCDT val, const wchar_t *lbl, int add);
void mgl_tune_ticks(HMGL gr, int tune, mreal fact_pos);
void mgl_set_tick_templ(HMGL gr, char dir, const char *templ);
void mgl_set_tick_templw(HMGL gr, char dir, const wchar_t *templ);
void mgl_set_ticks_time(HMGL gr, char dir, mreal d, const char *t);
void mgl_set_tick_shift(HMGL gr, mreal sx, mreal sy, mreal sz, mreal sc);

void mgl_box(HMGL graph);
void mgl_box_str(HMGL gr, const char *col, int ticks);
void mgl_axis(HMGL gr, const char *dir, const char *stl, const char *opt);
void mgl_axis_grid(HMGL gr, const char *dir,const char *pen, const char *opt);
void mgl_label(HMGL gr, char dir, const char *text, mreal pos, const char *opt);
void mgl_labelw(HMGL gr, char dir, const wchar_t *text, mreal pos, const char *opt);

void mgl_colorbar(HMGL gr, const char *sch);
void mgl_colorbar_ext(HMGL gr, const char *sch, mreal x, mreal y, mreal w, mreal h);
void mgl_colorbar_val(HMGL gr, HCDT dat, const char *sch);
void mgl_colorbar_val_ext(HMGL gr, HCDT dat, const char *sch,mreal x, mreal y, mreal w, mreal h);

void mgl_add_legend(HMGL gr, const char *text,const char *style);
void mgl_add_legendw(HMGL gr, const wchar_t *text,const char *style);
void mgl_clear_legend(HMGL graph);
void mgl_legend_pos(HMGL gr, mreal x, mreal y, const char *font, const char *opt);
void mgl_legend(HMGL gr, int where, const char *font, const char *opt);
void mgl_set_legend_marks(HMGL gr, int num);

HMEX mgl_create_expr(const char *expr);
void mgl_delete_expr(HMEX ex);
mreal mgl_expr_eval(HMEX ex, mreal x, mreal y,mreal z);
mreal mgl_expr_eval_v(HMEX ex, mreal *var);
mreal mgl_expr_diff(HMEX ex, char dir, mreal x, mreal y,mreal z);
mreal mgl_expr_diff_v(HMEX ex, char dir, mreal *var);

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
void mgl_write_obj(HMGL gr, const char *fname,const char *descr, int use_png);
void mgl_write_stl(HMGL gr, const char *fname,const char *descr);
void mgl_write_off(HMGL gr, const char *fname,const char *descr, int colored);
void mgl_write_xyz(HMGL gr, const char *fname,const char *descr);
//void mgl_write_x3d(HMGL gr, const char *fname,const char *descr);
void mgl_write_wgl(HMGL gr, const char *fname,const char *descr);
void mgl_write_prc(HMGL gr, const char *fname,const char *descr, int make_pdf);
void mgl_write_gif(HMGL gr, const char *fname,const char *descr);
void mgl_start_gif(HMGL gr, const char *fname,int ms);
void mgl_close_gif(HMGL graph);
void mgl_set_plotid(HMGL gr, const char *id);
void mgl_export_mgld(HMGL gr, const char *fname,const char *descr);
void mgl_import_mgld(HMGL gr, const char *fname, int add);
void mgl_write_json(HMGL gr, const char *fname,const char *descr);

const unsigned char *mgl_get_rgb(HMGL graph);
const unsigned char *mgl_get_rgba(HMGL graph);
void mgl_set_obj_id(HMGL gr, int id);
int mgl_get_obj_id(HMGL gr, long x, long y);
int mgl_get_spl_id(HMGL gr, long x, long y);
int mgl_get_width(HMGL graph);
int mgl_get_height(HMGL graph);
void mgl_calc_xyz(HMGL gr, int xs, int ys, mreal *x, mreal *y, mreal *z);
void mgl_calc_scr(HMGL gr, mreal x, mreal y, mreal z, int *xs, int *ys);

int mgl_new_frame(HMGL graph);
void mgl_end_frame(HMGL graph);
int mgl_get_num_frame(HMGL graph);
void mgl_reset_frames(HMGL graph);
void mgl_get_frame(HMGL graph, int i);

void mgl_set_transp_type(HMGL gr, int type);
void mgl_set_alpha(HMGL gr, int enable);
void mgl_set_fog(HMGL gr, mreal d, mreal dz);
void mgl_set_light(HMGL gr, int enable);
void mgl_set_light_n(HMGL gr, int n, int enable);

void mgl_mat_pop(HMGL gr);
void mgl_mat_push(HMGL gr);
void mgl_clf(HMGL graph);
void mgl_clf_rgb(HMGL gr, mreal r, mreal g, mreal b);

void mgl_subplot_d(HMGL gr, int nx,int ny,int m,const char *style, mreal dx, mreal dy);
void mgl_subplot(HMGL gr, int nx,int ny,int m,const char *style);
void mgl_multiplot(HMGL gr, int nx,int ny,int m,int dx,int dy,const char *style);
void mgl_inplot(HMGL gr, mreal x1,mreal x2,mreal y1,mreal y2);
void mgl_relplot(HMGL gr, mreal x1,mreal x2,mreal y1,mreal y2);
void mgl_columnplot(HMGL gr, int num, int ind, mreal d);
void mgl_gridplot(HMGL gr, int nx, int ny, int m, mreal d);
void mgl_stickplot(HMGL gr, int num, int ind, mreal tet, mreal phi);
void mgl_title(HMGL gr, const char *title, const char *stl, mreal size);
void mgl_titlew(HMGL gr, const wchar_t *title, const char *stl, mreal size);
void mgl_set_plotfactor(HMGL gr, mreal val);

void mgl_aspect(HMGL gr, mreal Ax,mreal Ay,mreal Az);
void mgl_rotate(HMGL gr, mreal TetX,mreal TetZ,mreal TetY);
void mgl_view(HMGL gr, mreal TetX,mreal TetZ,mreal TetY);
void mgl_zoom(HMGL gr, mreal x1, mreal y1, mreal x2, mreal y2);
void mgl_rotate_vector(HMGL gr, mreal Tet,mreal x,mreal y,mreal z);
void mgl_perspective(HMGL gr, mreal val);

void mgl_draw_thr(void *);
/******************************************************************************/
uintptr_t mgl_create_graph_(int *width, int *height);
void mgl_delete_graph_(uintptr_t *graph);
void mgl_set_size_(uintptr_t *graph, int *width, int *height);
void mgl_set_def_param_(uintptr_t *gr);
void mgl_set_quality_(uintptr_t *gr, int *qual);
void mgl_combine_gr_(uintptr_t *gr, uintptr_t *in);
void mgl_finish_(uintptr_t *gr);

void mgl_set_tick_len_(uintptr_t *graph, mreal *len, mreal *stt);
void mgl_set_axis_stl_(uintptr_t *graph, const char *stl, const char *tck, const char *sub, int,int,int);
void mgl_set_tick_shift_(uintptr_t *gr, mreal *sx, mreal *sy, mreal *sz, mreal *sc);

void mgl_adjust_ticks_(uintptr_t *gr, const char *dir, int l);
void mgl_set_ticks_(uintptr_t *gr, char *dir, mreal *d, int *ns, mreal *org, int);
void mgl_set_ticks_str_(uintptr_t *gr, const char *dir, const char *lbl, int *add,int,int l);
void mgl_set_ticks_val_(uintptr_t *gr, const char *dir, uintptr_t *val, const char *lbl, int *add,int,int l);
void mgl_tune_ticks_(uintptr_t *gr, int *tune, mreal *fact_pos);
void mgl_set_tick_templ_(uintptr_t *gr, const char *dir, const char *templ,int,int l);
void mgl_set_ticks_time_(uintptr_t *gr, const char *dir, mreal *d, const char *t,int,int l);
void mgl_box_(uintptr_t *gr);
void mgl_box_str_(uintptr_t *gr, const char *col, int *ticks, int l);
void mgl_axis_(uintptr_t *gr, const char *dir, const char *stl, const char *opt,int,int,int);
void mgl_axis_grid_(uintptr_t *gr, const char *dir,const char *pen, const char *opt,int,int,int);
void mgl_label_(uintptr_t *gr, const char *dir, const char *text, const char *opt,int,int,int);
/******************************************************************************/
void mgl_colorbar_(uintptr_t *gr, const char *sch,int l);
void mgl_colorbar_ext_(uintptr_t *gr, const char *sch, mreal *x, mreal *y, mreal *w, mreal *h, int l);
void mgl_colorbar_val_(uintptr_t *gr, uintptr_t *dat, const char *sch,int l);
void mgl_colorbar_val_ext_(uintptr_t *gr, uintptr_t *dat, const char *sch, mreal *x, mreal *y, mreal *w, mreal *h, int l);

void mgl_add_legend_(uintptr_t *gr, const char *text,const char *style,int l,int n);
void mgl_clear_legend_(uintptr_t *gr);
void mgl_legend_pos_(uintptr_t *gr, mreal *x, mreal *y, const char *font, const char *opt,int,int);
void mgl_legend_(uintptr_t *gr, int *where, const char *font, const char *opt,int,int);
void mgl_set_legend_marks_(uintptr_t *gr, int *num);

uintptr_t mgl_create_expr_(const char *expr, int);
void mgl_delete_expr_(uintptr_t *ex);
mreal mgl_eval_expr_(uintptr_t *ex, mreal *x, mreal *y,mreal *z);
mreal mgl_diff_expr_(uintptr_t *ex, const char *dir, mreal *x, mreal *y,mreal *z, int);

void mgl_show_image_(uintptr_t *graph, const char *viewer, int *keep, int);
void mgl_write_frame_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_tga_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_bmp_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_jpg_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_png_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_png_solid_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_eps_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_svg_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_prc_(uintptr_t *graph, const char *fname,const char *descr, int *make_pdf,int lf,int ld);
void mgl_write_gif_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_start_gif_(uintptr_t *graph, const char *fname,int *ms,int l);
void mgl_close_gif_(uintptr_t *graph);
void mgl_write_obj_(uintptr_t *graph, const char *fname,const char *descr, int *use_png,int lf,int ld);
void mgl_write_stl_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_off_(uintptr_t *graph, const char *fname,const char *descr,int *colored,int lf,int ld);
void mgl_write_xyz_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
//void mgl_write_x3d_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_tex_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_wgl_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_set_plotid_(uintptr_t *gr, const char *id,int l);
void mgl_export_mgld_(uintptr_t *gr, const char *fname,const char *descr,int l,int n);
void mgl_import_mgld_(uintptr_t *gr, const char *fname, int *add, int l);
void mgl_write_json_(uintptr_t *gr, const char *fname,const char *descr,int l,int n);

const unsigned char *mgl_get_rgb_(uintptr_t *graph);
const unsigned char *mgl_get_rgba_(uintptr_t *graph);
void mgl_set_obj_id_(uintptr_t *graph, int *id);
int mgl_get_obj_id_(uintptr_t *graph, int *x, int *y);
int mgl_get_spl_id_(uintptr_t *graph, int *x, int *y);
int mgl_get_width_(uintptr_t *graph);
int mgl_get_height_(uintptr_t *graph);
void mgl_calc_xyz_(uintptr_t *gr, int *xs, int *ys, mreal *x, mreal *y, mreal *z);
void mgl_calc_scr_(uintptr_t *gr, mreal *x, mreal *y, mreal *z, int *xs, int *ys);

int mgl_new_frame_(uintptr_t *graph);
void mgl_end_frame_(uintptr_t *graph);
int mgl_get_num_frame_(uintptr_t *graph);
void mgl_reset_frames_(uintptr_t *graph);
void mgl_get_frame_(uintptr_t *graph, int *i);

void mgl_set_transp_type_(uintptr_t *graph, int *type);
void mgl_set_alpha_(uintptr_t *graph, int *enable);
void mgl_set_fog_(uintptr_t *graph, mreal *dist, mreal *dz);
void mgl_set_light_(uintptr_t *graph, int *enable);
void mgl_set_light_n_(uintptr_t *gr, int *n, int *enable);
void mgl_add_light_(uintptr_t *graph, int *n, mreal *x, mreal *y, mreal *z);
void mgl_add_light(HMGL gr, int n, mreal x, mreal y, mreal z);
void mgl_add_light_ext_(uintptr_t *graph, int *n, mreal *x, mreal *y, mreal *z, char *c, mreal *br, mreal *ap, int);
void mgl_add_light_ext(HMGL gr, int n, mreal x, mreal y, mreal z, char c, mreal br, mreal ap);
void mgl_add_light_loc_(uintptr_t *graph, int *n, mreal *x, mreal *y, mreal *z, mreal *dx, mreal *dy, mreal *dz, char *c, mreal *br, mreal *ap, int);
void mgl_add_light_loc(HMGL gr, int n, mreal x, mreal y, mreal z, mreal dx, mreal dy, mreal dz, char c, mreal br, mreal ap);

void mgl_mat_push_(uintptr_t *gr);
void mgl_mat_pop_(uintptr_t *gr);
void mgl_clf_(uintptr_t *graph);
void mgl_clf_rgb_(uintptr_t *graph, mreal *r, mreal *g, mreal *b);

void mgl_subplot_(uintptr_t *graph, int *nx,int *ny,int *m);
void mgl_subplot_d_(uintptr_t *graph, int *nx,int *ny,int *m,mreal *dx,mreal *dy);
void mgl_subplot_s_(uintptr_t *graph, int *nx,int *ny,int *m, const char *s,int);
void mgl_multiplot_(uintptr_t *graph, int *nx,int *ny,int *m,int *dx,int *dy, const char *s,int);
void mgl_inplot_(uintptr_t *graph, mreal *x1,mreal *x2,mreal *y1,mreal *y2);
void mgl_relplot_(uintptr_t *graph, mreal *x1,mreal *x2,mreal *y1,mreal *y2);
void mgl_columnplot_(uintptr_t *graph, int *num, int *i, mreal *d);
void mgl_gridplot_(uintptr_t *graph, int *nx, int *ny, int *i, mreal *d);
void mgl_stickplot_(uintptr_t *graph, int *num, int *i, mreal *tet, mreal *phi);
void mgl_set_plotfactor_(uintptr_t *graph, mreal *val);

void mgl_title_(uintptr_t *gr, const char *title, const char *stl, mreal *size, int,int);
void mgl_aspect_(uintptr_t *graph, mreal *Ax,mreal *Ay,mreal *Az);
void mgl_rotate_(uintptr_t *graph, mreal *TetX,mreal *TetZ,mreal *TetY);
void mgl_view_(uintptr_t *graph, mreal *TetX,mreal *TetZ,mreal *TetY);
void mgl_zoom_(uintptr_t *graph, mreal *x1, mreal *y1, mreal *x2, mreal *y2);
void mgl_rotate_vector_(uintptr_t *graph, mreal *Tet,mreal *x,mreal *y,mreal *z);
void mgl_perspective_(uintptr_t *graph, mreal val);
/******************************************************************************/
HMGL mgl_create_graph_qt(int (*draw)(HMGL gr, void *p), const char *title, void *par, void (*load)(void *p));
HMGL mgl_create_graph_fltk(int (*draw)(HMGL gr, void *p), const char *title, void *par, void (*load)(void *p));
void mgl_set_click_func(HMGL gr, void (*func)(void *p));
int mgl_fltk_run();
int mgl_fltk_thr();
int mgl_qt_run();
/******************************************************************************/
uintptr_t mgl_create_graph_qt_(const char *title, int);
uintptr_t mgl_create_graph_fltk_(const char *title, int);
int mgl_fltk_run_();
int mgl_qt_run_();
/******************************************************************************/
void mgl_wnd_set_delay(HMGL gr, mreal dt);
void mgl_setup_window(HMGL gr, int clf_upd, int showpos);
void mgl_wnd_toggle_alpha(HMGL gr);
void mgl_wnd_toggle_light(HMGL gr);
void mgl_wnd_toggle_zoom(HMGL gr);
void mgl_wnd_toggle_rotate(HMGL gr);
void mgl_wnd_toggle_no(HMGL gr);
void mgl_wnd_update(HMGL gr);
void mgl_wnd_reload(HMGL gr);
void mgl_wnd_adjust(HMGL gr);
void mgl_wnd_next_frame(HMGL gr);
void mgl_wnd_prev_frame(HMGL gr);
void mgl_wnd_animation(HMGL gr);
void mgl_get_last_mouse_pos(HMGL gr, mreal *x, mreal *y, mreal *z);
/******************************************************************************/
void mgl_get_last_mouse_pos_(uintptr_t *gr, mreal *x, mreal *y, mreal *z);
void mgl_wnd_set_delay_(uintptr_t *gr, mreal *dt);
void mgl_setup_window_(uintptr_t *gr, int *clf_upd, int *showpos);
void mgl_wnd_toggle_alpha_(uintptr_t *gr);
void mgl_wnd_toggle_light_(uintptr_t *gr);
void mgl_wnd_toggle_zoom_(uintptr_t *gr);
void mgl_wnd_toggle_rotate_(uintptr_t *gr);
void mgl_wnd_toggle_no_(uintptr_t *gr);
void mgl_wnd_update_(uintptr_t *gr);
void mgl_wnd_reload_(uintptr_t *gr);
void mgl_wnd_adjust_(uintptr_t *gr);
void mgl_wnd_next_frame_(uintptr_t *gr);
void mgl_wnd_prev_frame_(uintptr_t *gr);
void mgl_wnd_animation_(uintptr_t *gr);
/******************************************************************************/
void mgl_mpi_send(HMGL gr, int id);
void mgl_mpi_recv(HMGL gr, int id);
void mgl_mpi_send_(uintptr_t *gr, int *id);
void mgl_mpi_recv_(uintptr_t *gr, int *id);
/******************************************************************************/
HMPR mgl_create_parser();
long mgl_use_parser(HMPR p, int inc);
void mgl_delete_parser(HMPR p);
void mgl_parse_add_param(HMPR p, int id, const char *str);
void mgl_parse_add_paramw(HMPR p, int id, const wchar_t *str);
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
HMDT mgl_parse_add_var(HMPR p, const char *name);
HMDT mgl_parse_add_varw(HMPR p, const wchar_t *name);
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
HMDT mgl_parse_find_var(HMPR p, const char *name);
HMDT mgl_parse_find_varw(HMPR p, const wchar_t *name);
void mgl_parse_del_var(HMPR p, const char *name);
void mgl_parse_del_varw(HMPR p, const wchar_t *name);
void mgl_parse_del_all(HMPR p);
int mgl_parse(HMGL gr, HMPR p, const char *str, int pos);
int mgl_parsew(HMGL gr, HMPR p, const wchar_t *str, int pos);
void mgl_parse_file(HMGL gr, HMPR p, FILE *fp, int print);
void mgl_parse_text(HMGL gr, HMPR p, const char *str);
void mgl_parse_textw(HMGL gr, HMPR p, const wchar_t *str);
void mgl_parse_restore_once(HMPR p);
void mgl_parser_allow_setsize(HMPR p, int a);
void mgl_parser_stop(HMPR p);
int mgl_parse_cmd_type(HMPR pr, const char *name);
const char *mgl_parse_cmd_desc(HMPR pr, const char *name);
const char *mgl_parse_cmd_frmt(HMPR pr, const char *name);
const char *mgl_parse_cmd_name(HMPR pr, long id);
long mgl_parse_cmd_num(HMPR pr);
HMDT mgl_parse_calc(HMPR pr, const char *formula);
HMDT mgl_parse_calcw(HMPR pr, const wchar_t *formula);
//int mgl_parser_find_cmdw(HMPR pr, const wchar_t *name);
/******************************************************************************/
uintptr_t mgl_create_parser_();
long mgl_use_parser_(uintptr_t* , int *inc);
void mgl_delete_parser_(uintptr_t* p);
void mgl_parse_add_param_(uintptr_t* p, int *id, const char *str, int l);
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
uintptr_t mgl_parse_add_var_(uintptr_t* p, const char *name, int l);
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
uintptr_t mgl_parse_find_var_(uintptr_t* p, const char *name, int l);
void mgl_parse_del_var_(uintptr_t* p, const char *name, int l);
void mgl_parse_del_all_(uintptr_t *p);
int mgl_parse_(uintptr_t* gr, uintptr_t* p, const char *str, int *pos, int l);
void mgl_parse_text_(uintptr_t* gr, uintptr_t* p, const char *str, int l);
void mgl_parse_restore_once_(uintptr_t* p);
void mgl_parser_allow_setsize_(uintptr_t* p, int *a);
void mgl_parser_stop_(uintptr_t* p);
int mgl_parse_cmd_type_(uintptr_t* p, const char *name, int l);
uintptr_t mgl_parse_calc_(uintptr_t *pr, const char *formula,int l);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif

