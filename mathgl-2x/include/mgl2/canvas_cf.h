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
uintptr_t mgl_create_graph_(int *width, int *height);
void mgl_delete_graph(HMGL gr);
void mgl_delete_graph_(uintptr_t *gr);
void mgl_set_size(HMGL gr, int width, int height);
void mgl_set_size_(uintptr_t *gr, int *width, int *height);
void mgl_set_def_param(HMGL gr);
void mgl_set_def_param_(uintptr_t *gr);
void mgl_set_quality(HMGL gr, int qual);
void mgl_set_quality_(uintptr_t *gr, int *qual);
void mgl_combine_gr(HMGL gr, HMGL in);
void mgl_combine_gr_(uintptr_t *gr, uintptr_t *in);
void mgl_finish(HMGL gr);
void mgl_finish_(uintptr_t *gr);

void mgl_set_tick_len(HMGL gr, double len, double stt);
void mgl_set_tick_len_(uintptr_t *gr, double *len, double *stt);
void mgl_set_axis_stl(HMGL gr, const char *stl, const char *tck, const char *sub);
void mgl_set_axis_stl_(uintptr_t *gr, const char *stl, const char *tck, const char *sub, int,int,int);

void mgl_adjust_ticks(HMGL gr, const char *dir);
void mgl_adjust_ticks_(uintptr_t *gr, const char *dir, int);
void mgl_set_ticks(HMGL gr, char dir, double d, int ns, double org);
void mgl_set_ticks_(uintptr_t *gr, char *dir, double *d, int *ns, double *org, int);
void mgl_set_ticks_str(HMGL gr, char dir, const char *lbl, int add);
void mgl_set_ticks_str_(uintptr_t *gr, const char *dir, const char *lbl, int *add,int,int);
void mgl_set_ticks_wcs(HMGL gr, char dir, const wchar_t *lbl, int add);
void mgl_set_ticks_val(HMGL gr, char dir, HCDT val, const char *lbl, int add);
void mgl_set_ticks_val_(uintptr_t *gr, const char *dir, uintptr_t *val, const char *lbl, int *add,int,int);
void mgl_set_ticks_valw(HMGL gr, char dir, HCDT val, const wchar_t *lbl, int add);
void mgl_tune_ticks(HMGL gr, int tune, double fact_pos);
void mgl_tune_ticks_(uintptr_t *gr, int *tune, double *fact_pos);
void mgl_set_tick_templ(HMGL gr, char dir, const char *templ);
void mgl_set_tick_templ_(uintptr_t *gr, const char *dir, const char *templ,int,int);
void mgl_set_tick_templw(HMGL gr, char dir, const wchar_t *templ);
void mgl_set_ticks_time(HMGL gr, char dir, double d, const char *t);
void mgl_set_ticks_time_(uintptr_t *gr, const char *dir, double *d, const char *t,int,int);
void mgl_set_tick_shift(HMGL gr, double sx, double sy, double sz, double sc);
void mgl_set_tick_shift_(uintptr_t *gr, double *sx, double *sy, double *sz, double *sc);

void mgl_box(HMGL gr);
void mgl_box_(uintptr_t *gr);
void mgl_box_str(HMGL gr, const char *col, int ticks);
void mgl_box_str_(uintptr_t *gr, const char *col, int *ticks, int);
void mgl_axis(HMGL gr, const char *dir, const char *stl, const char *opt);
void mgl_axis_(uintptr_t *gr, const char *dir, const char *stl, const char *opt,int,int,int);
void mgl_axis_grid(HMGL gr, const char *dir,const char *pen, const char *opt);
void mgl_axis_grid_(uintptr_t *gr, const char *dir,const char *pen, const char *opt,int,int,int);
void mgl_label(HMGL gr, char dir, const char *text, double pos, const char *opt);
void mgl_label_(uintptr_t *gr, const char *dir, const char *text, const char *opt,int,int,int);
void mgl_labelw(HMGL gr, char dir, const wchar_t *text, double pos, const char *opt);

void mgl_colorbar(HMGL gr, const char *sch);
void mgl_colorbar_(uintptr_t *gr, const char *sch,int);
void mgl_colorbar_ext(HMGL gr, const char *sch, double x, double y, double w, double h);
void mgl_colorbar_ext_(uintptr_t *gr, const char *sch, double *x, double *y, double *w, double *h, int);
void mgl_colorbar_val(HMGL gr, HCDT dat, const char *sch);
void mgl_colorbar_val_(uintptr_t *gr, uintptr_t *dat, const char *sch,int);
void mgl_colorbar_val_ext(HMGL gr, HCDT dat, const char *sch,double x, double y, double w, double h);
void mgl_colorbar_val_ext_(uintptr_t *gr, uintptr_t *dat, const char *sch, double *x, double *y, double *w, double *h, int);

void mgl_add_legend(HMGL gr, const char *text,const char *style);
void mgl_add_legend_(uintptr_t *gr, const char *text,const char *style,int,int);
void mgl_add_legendw(HMGL gr, const wchar_t *text,const char *style);
void mgl_clear_legend(HMGL gr);
void mgl_clear_legend_(uintptr_t *gr);
void mgl_legend_pos(HMGL gr, double x, double y, const char *font, const char *opt);
void mgl_legend_pos_(uintptr_t *gr, double *x, double *y, const char *font, const char *opt,int,int);
void mgl_legend(HMGL gr, int where, const char *font, const char *opt);
void mgl_legend_(uintptr_t *gr, int *where, const char *font, const char *opt,int,int);
void mgl_set_legend_marks(HMGL gr, int num);
void mgl_set_legend_marks_(uintptr_t *gr, int *num);

HMEX mgl_create_expr(const char *expr);
uintptr_t mgl_create_expr_(const char *expr, int);
void mgl_delete_expr(HMEX ex);
void mgl_delete_expr_(uintptr_t *ex);
double mgl_expr_eval(HMEX ex, double x, double y,double z);
double mgl_eval_expr_(uintptr_t *ex, double *x, double *y,double *z);
double mgl_expr_eval_v(HMEX ex, mreal *var);
double mgl_expr_diff(HMEX ex, char dir, double x, double y,double z);
double mgl_diff_expr_(uintptr_t *ex, const char *dir, double *x, double *y,double *z, int);
double mgl_expr_diff_v(HMEX ex, char dir, mreal *var);

void mgl_show_image(HMGL gr, const char *viewer, int keep);
void mgl_show_image_(uintptr_t *gr, const char *viewer, int *keep, int);
void mgl_write_frame(HMGL gr, const char *fname,const char *descr);
void mgl_write_frame_(uintptr_t *gr, const char *fname,const char *descr,int,int);
void mgl_write_tga(HMGL gr, const char *fname,const char *descr);
void mgl_write_tga_(uintptr_t *gr, const char *fname,const char *descr,int,int);
void mgl_write_bmp(HMGL gr, const char *fname,const char *descr);
void mgl_write_bmp_(uintptr_t *gr, const char *fname,const char *descr,int,int);
void mgl_write_jpg(HMGL gr, const char *fname,const char *descr);
void mgl_write_jpg_(uintptr_t *gr, const char *fname,const char *descr,int,int);
void mgl_write_png(HMGL gr, const char *fname,const char *descr);
void mgl_write_png_(uintptr_t *gr, const char *fname,const char *descr,int,int);
void mgl_write_png_solid(HMGL gr, const char *fname,const char *descr);
void mgl_write_png_solid_(uintptr_t *gr, const char *fname,const char *descr,int,int);
void mgl_write_bps(HMGL gr, const char *fname,const char *descr);
void mgl_write_eps(HMGL gr, const char *fname,const char *descr);
void mgl_write_eps_(uintptr_t *gr, const char *fname,const char *descr,int,int);
void mgl_write_svg(HMGL gr, const char *fname,const char *descr);
void mgl_write_svg_(uintptr_t *gr, const char *fname,const char *descr,int,int);
void mgl_write_tex(HMGL gr, const char *fname,const char *descr);
void mgl_write_tex_(uintptr_t *gr, const char *fname,const char *descr,int,int);
void mgl_write_obj(HMGL gr, const char *fname,const char *descr, int use_png);
void mgl_write_obj_(uintptr_t *gr, const char *fname,const char *descr, int *use_png,int,int);
void mgl_write_stl(HMGL gr, const char *fname,const char *descr);
void mgl_write_stl_(uintptr_t *gr, const char *fname,const char *descr,int,int);
void mgl_write_off(HMGL gr, const char *fname,const char *descr, int colored);
void mgl_write_off_(uintptr_t *gr, const char *fname,const char *descr,int *colored,int,int);
void mgl_write_xyz(HMGL gr, const char *fname,const char *descr);
void mgl_write_xyz_(uintptr_t *gr, const char *fname,const char *descr,int,int);
/*void mgl_write_x3d(HMGL gr, const char *fname,const char *descr);
void mgl_write_x3d_(uintptr_t *gr, const char *fname,const char *descr,int,int);*/
void mgl_write_wgl(HMGL gr, const char *fname,const char *descr);
void mgl_write_wgl_(uintptr_t *gr, const char *fname,const char *descr,int,int);
void mgl_write_prc(HMGL gr, const char *fname,const char *descr, int make_pdf);
void mgl_write_prc_(uintptr_t *gr, const char *fname,const char *descr, int *make_pdf,int,int);
void mgl_write_gif(HMGL gr, const char *fname,const char *descr);
void mgl_write_gif_(uintptr_t *gr, const char *fname,const char *descr,int,int);

void mgl_start_gif(HMGL gr, const char *fname,int ms);
void mgl_start_gif_(uintptr_t *gr, const char *fname,int *ms,int);
void mgl_close_gif(HMGL gr);
void mgl_close_gif_(uintptr_t *gr);
void mgl_set_plotid(HMGL gr, const char *id);
void mgl_set_plotid_(uintptr_t *gr, const char *id,int);

void mgl_export_mgld(HMGL gr, const char *fname,const char *descr);
void mgl_export_mgld_(uintptr_t *gr, const char *fname,const char *descr,int,int);
void mgl_import_mgld(HMGL gr, const char *fname, int add);
void mgl_import_mgld_(uintptr_t *gr, const char *fname, int *add, int);
void mgl_write_json(HMGL gr, const char *fname,const char *descr);
void mgl_write_json_(uintptr_t *gr, const char *fname,const char *descr,int,int);

const unsigned char *mgl_get_rgb(HMGL gr);
const unsigned char *mgl_get_rgb_(uintptr_t *gr);
const unsigned char *mgl_get_rgba(HMGL gr);
const unsigned char *mgl_get_rgba_(uintptr_t *gr);
void mgl_set_obj_id(HMGL gr, int id);
void mgl_set_obj_id_(uintptr_t *gr, int *id);
int mgl_get_obj_id(HMGL gr, long x, long y);
int mgl_get_obj_id_(uintptr_t *gr, int *x, int *y);
int mgl_get_spl_id(HMGL gr, long x, long y);
int mgl_get_spl_id_(uintptr_t *gr, int *x, int *y);
int mgl_get_width(HMGL gr);
int mgl_get_width_(uintptr_t *gr);
int mgl_get_height(HMGL gr);
int mgl_get_height_(uintptr_t *gr);
void mgl_calc_xyz(HMGL gr, int xs, int ys, double *x, double *y, double *z);
void mgl_calc_xyz_(uintptr_t *gr, int *xs, int *ys, double *x, double *y, double *z);
void mgl_calc_scr(HMGL gr, double x, double y, double z, int *xs, int *ys);
void mgl_calc_scr_(uintptr_t *gr, double *x, double *y, double *z, int *xs, int *ys);
long mgl_is_active(HMGL gr, int xs, int ys, int d);
long mgl_is_active_(uintptr_t *gr, int *xs, int *ys, int *d);

int mgl_new_frame(HMGL gr);
int mgl_new_frame_(uintptr_t *gr);
void mgl_end_frame(HMGL gr);
void mgl_end_frame_(uintptr_t *gr);
int mgl_get_num_frame(HMGL gr);
int mgl_get_num_frame_(uintptr_t *gr);
void mgl_reset_frames(HMGL gr);
void mgl_reset_frames_(uintptr_t *gr);
void mgl_get_frame(HMGL gr, int i);
void mgl_get_frame_(uintptr_t *gr, int *i);

void mgl_set_transp_type(HMGL gr, int type);
void mgl_set_transp_type_(uintptr_t *gr, int *type);
void mgl_set_alpha(HMGL gr, int enable);
void mgl_set_alpha_(uintptr_t *gr, int *enable);
void mgl_set_fog(HMGL gr, double d, double dz);
void mgl_set_fog_(uintptr_t *gr, double *dist, double *dz);
void mgl_set_light(HMGL gr, int enable);
void mgl_set_light_(uintptr_t *gr, int *enable);
void mgl_set_light_n(HMGL gr, int n, int enable);
void mgl_set_light_n_(uintptr_t *gr, int *n, int *enable);

void mgl_add_light(HMGL gr, int n, double x, double y, double z);
void mgl_add_light_(uintptr_t *gr, int *n, double *x, double *y, double *z);
void mgl_add_light_ext(HMGL gr, int n, double x, double y, double z, char c, double br, double ap);
void mgl_add_light_ext_(uintptr_t *gr, int *n, double *x, double *y, double *z, char *c, double *br, double *ap, int);
void mgl_add_light_loc(HMGL gr, int n, double x, double y, double z, double dx, double dy, double dz, char c, double br, double ap);
void mgl_add_light_loc_(uintptr_t *gr, int *n, double *x, double *y, double *z, double *dx, double *dy, double *dz, char *c, double *br, double *ap, int);

void mgl_mat_pop(HMGL gr);
void mgl_mat_pop_(uintptr_t *gr);
void mgl_mat_push(HMGL gr);
void mgl_mat_push_(uintptr_t *gr);
void mgl_clf(HMGL gr);
void mgl_clf_(uintptr_t *gr);
void mgl_clf_rgb(HMGL gr, double r, double g, double b);
void mgl_clf_rgb_(uintptr_t *gr, double *r, double *g, double *b);

void mgl_subplot_d(HMGL gr, int nx,int ny,int m,const char *style, double dx, double dy);
void mgl_subplot_d_(uintptr_t *gr, int *nx,int *ny,int *m,double *dx,double *dy);
void mgl_subplot(HMGL gr, int nx,int ny,int m,const char *style);
void mgl_subplot_(uintptr_t *gr, int *nx,int *ny,int *m, const char *s,int);
void mgl_multiplot(HMGL gr, int nx,int ny,int m,int dx,int dy,const char *style);
void mgl_multiplot_(uintptr_t *gr, int *nx,int *ny,int *m,int *dx,int *dy, const char *s,int);
void mgl_inplot(HMGL gr, double x1,double x2,double y1,double y2);
void mgl_inplot_(uintptr_t *gr, double *x1,double *x2,double *y1,double *y2);
void mgl_relplot(HMGL gr, double x1,double x2,double y1,double y2);
void mgl_relplot_(uintptr_t *gr, double *x1,double *x2,double *y1,double *y2);
void mgl_columnplot(HMGL gr, int num, int ind, double d);
void mgl_columnplot_(uintptr_t *gr, int *num, int *i, double *d);
void mgl_gridplot(HMGL gr, int nx, int ny, int m, double d);
void mgl_gridplot_(uintptr_t *gr, int *nx, int *ny, int *i, double *d);
void mgl_stickplot(HMGL gr, int num, int ind, double tet, double phi);
void mgl_stickplot_(uintptr_t *gr, int *num, int *i, double *tet, double *phi);
void mgl_title(HMGL gr, const char *title, const char *stl, double size);
void mgl_title_(uintptr_t *gr, const char *title, const char *stl, double *size, int,int);
void mgl_titlew(HMGL gr, const wchar_t *title, const char *stl, double size);
void mgl_set_plotfactor(HMGL gr, double val);
void mgl_set_plotfactor_(uintptr_t *gr, double *val);

void mgl_aspect(HMGL gr, double Ax,double Ay,double Az);
void mgl_aspect_(uintptr_t *gr, double *Ax,double *Ay,double *Az);
void mgl_rotate(HMGL gr, double TetX,double TetZ,double TetY);
void mgl_rotate_(uintptr_t *gr, double *TetX,double *TetZ,double *TetY);
void mgl_view(HMGL gr, double TetX,double TetZ,double TetY);
void mgl_view_(uintptr_t *gr, double *TetX,double *TetZ,double *TetY);
void mgl_zoom(HMGL gr, double x1, double y1, double x2, double y2);
void mgl_zoom_(uintptr_t *gr, double *x1, double *y1, double *x2, double *y2);
void mgl_rotate_vector(HMGL gr, double Tet,double x,double y,double z);
void mgl_rotate_vector_(uintptr_t *gr, double *Tet,double *x,double *y,double *z);
void mgl_perspective(HMGL gr, double val);
void mgl_perspective_(uintptr_t *gr, double val);

void mgl_draw_thr(void *);
/******************************************************************************/
HMGL mgl_create_graph_qt(int (*draw)(HMGL gr, void *p), const char *title, void *par, void (*load)(void *p));
uintptr_t mgl_create_graph_qt_(const char *title, int);
HMGL mgl_create_graph_fltk(int (*draw)(HMGL gr, void *p), const char *title, void *par, void (*load)(void *p));
uintptr_t mgl_create_graph_fltk_(const char *title, int);
HMGL mgl_create_graph_wx(int (*draw)(HMGL gr, void *p), const char *title, void *par, void (*load)(void *p));
uintptr_t mgl_create_graph_wx_(const char *title, int);
void mgl_set_click_func(HMGL gr, void (*func)(void *p));
int mgl_fltk_run();
int mgl_fltk_run_();
int mgl_fltk_thr();
int mgl_qt_run();
int mgl_qt_run_();
int mgl_wx_run();
int mgl_wx_run_();
/******************************************************************************/
void mgl_wnd_set_delay(HMGL gr, double dt);
void mgl_wnd_set_delay_(uintptr_t *gr, double *dt);
void mgl_setup_window(HMGL gr, int clf_upd, int showpos);
void mgl_setup_window_(uintptr_t *gr, int *clf_upd, int *showpos);
void mgl_wnd_toggle_alpha(HMGL gr);
void mgl_wnd_toggle_alpha_(uintptr_t *gr);
void mgl_wnd_toggle_light(HMGL gr);
void mgl_wnd_toggle_light_(uintptr_t *gr);
void mgl_wnd_toggle_zoom(HMGL gr);
void mgl_wnd_toggle_zoom_(uintptr_t *gr);
void mgl_wnd_toggle_rotate(HMGL gr);
void mgl_wnd_toggle_rotate_(uintptr_t *gr);
void mgl_wnd_toggle_no(HMGL gr);
void mgl_wnd_toggle_no_(uintptr_t *gr);
void mgl_wnd_update(HMGL gr);
void mgl_wnd_update_(uintptr_t *gr);
void mgl_wnd_reload(HMGL gr);
void mgl_wnd_reload_(uintptr_t *gr);
void mgl_wnd_adjust(HMGL gr);
void mgl_wnd_adjust_(uintptr_t *gr);
void mgl_wnd_next_frame(HMGL gr);
void mgl_wnd_next_frame_(uintptr_t *gr);
void mgl_wnd_prev_frame(HMGL gr);
void mgl_wnd_prev_frame_(uintptr_t *gr);
void mgl_wnd_animation(HMGL gr);
void mgl_wnd_animation_(uintptr_t *gr);
void mgl_get_last_mouse_pos(HMGL gr, double *x, double *y, double *z);
void mgl_get_last_mouse_pos_(uintptr_t *gr, double *x, double *y, double *z);
/******************************************************************************/
void mgl_mpi_send(HMGL gr, int id);
void mgl_mpi_send_(uintptr_t *gr, int *id);
void mgl_mpi_recv(HMGL gr, int id);
void mgl_mpi_recv_(uintptr_t *gr, int *id);
/******************************************************************************/
HMPR mgl_create_parser();
uintptr_t mgl_create_parser_();
long mgl_use_parser(HMPR p, int inc);
long mgl_use_parser_(uintptr_t* , int *inc);
void mgl_delete_parser(HMPR p);
void mgl_delete_parser_(uintptr_t* p);
void mgl_parser_add_param(HMPR p, int id, const char *str);
void mgl_parser_add_param_(uintptr_t* p, int *id, const char *str, int);
void mgl_parser_add_paramw(HMPR p, int id, const wchar_t *str);

/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
HMDT mgl_parser_add_var(HMPR p, const char *name);
uintptr_t mgl_parser_add_var_(uintptr_t* p, const char *name, int);
HMDT mgl_parser_add_varw(HMPR p, const wchar_t *name);
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
HMDT mgl_parser_find_var(HMPR p, const char *name);
uintptr_t mgl_parser_find_var_(uintptr_t* p, const char *name, int);
HMDT mgl_parser_find_varw(HMPR p, const wchar_t *name);

void mgl_parser_del_var(HMPR p, const char *name);
void mgl_parser_del_var_(uintptr_t* p, const char *name, int);
void mgl_parser_del_varw(HMPR p, const wchar_t *name);
void mgl_parser_del_all(HMPR p);
void mgl_parser_del_all_(uintptr_t *p);

int mgl_parse_line(HMGL gr, HMPR p, const char *str, int pos);
int mgl_parse_line_(uintptr_t* gr, uintptr_t* p, const char *str, int *pos, int);
int mgl_parse_linew(HMGL gr, HMPR p, const wchar_t *str, int pos);
void mgl_parse_file(HMGL gr, HMPR p, FILE *fp, int print);
void mgl_parse_text(HMGL gr, HMPR p, const char *str);
void mgl_parse_text_(uintptr_t* gr, uintptr_t* p, const char *str, int);
void mgl_parse_textw(HMGL gr, HMPR p, const wchar_t *str);

void mgl_parser_restore_once(HMPR p);
void mgl_parser_restore_once_(uintptr_t* p);
void mgl_parser_allow_setsize(HMPR p, int a);
void mgl_parser_allow_setsize_(uintptr_t* p, int *a);
void mgl_parser_allow_file_io(HMPR p, int a);
void mgl_parser_allow_file_io_(uintptr_t* p, int *a);
void mgl_parser_stop(HMPR p);
void mgl_parser_stop_(uintptr_t* p);

int mgl_parser_cmd_type(HMPR pr, const char *name);
int mgl_parser_cmd_type_(uintptr_t* p, const char *name, int);
const char *mgl_parser_cmd_desc(HMPR pr, const char *name);
const char *mgl_parser_cmd_frmt(HMPR pr, const char *name);
const char *mgl_parser_cmd_name(HMPR pr, long id);
long mgl_parser_cmd_num(HMPR pr);

HMDT mgl_parser_calc(HMPR pr, const char *formula);
uintptr_t mgl_parser_calc_(uintptr_t *pr, const char *formula,int);
HMDT mgl_parser_calcw(HMPR pr, const wchar_t *formula);
//int mgl_parser_find_cmdw(HMPR pr, const wchar_t *name);
/******************************************************************************/
/******************************************************************************/
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif

