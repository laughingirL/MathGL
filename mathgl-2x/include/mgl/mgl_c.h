/***************************************************************************
 * mgl_c.h is part of Math Graphic Library
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
#ifndef _MGL_C_H_
#define _MGL_C_H_

#include "mgl/config.h"

#include "mgl/base.h"

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
//#ifdef _MGL_DATA_H_
#ifdef __cplusplus
class mglDraw;
typedef mglDraw* HMDR;
class mglParse;
typedef mglParse* HMPR;
#else
typedef void* HMDR;
typedef void* HMPR;
#endif

#ifndef NO_GSL
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#else
struct gsl_vector;
struct gsl_matrix;
#endif
/*****************************************************************************/
HMGL mgl_create_graph_gl();
HMGL mgl_create_graph_zb(int width, int height);
HMGL mgl_create_graph_ps(int width, int height);
HMGL mgl_create_graph_idtf();
#ifndef MGL_NO_WIDGET
int mgl_fortran_func(HMGL gr, void *);
HMGL mgl_create_graph_glut(int (*draw)(HMGL gr, void *p), const char *title, void *par);
HMGL mgl_create_graph_fltk(int (*draw)(HMGL gr, void *p), const char *title, void *par);
HMGL mgl_create_graph_qt(int (*draw)(HMGL gr, void *p), const char *title, void *par);
HMGL mgl_create_graph_glut_dr(HMDR dr, const char *title);
HMGL mgl_create_graph_fltk_dr(HMDR dr, const char *title);
HMGL mgl_create_graph_qt_dr(HMDR dr, const char *title);
void mgl_fltk_run();
void mgl_qt_run();

void mgl_wnd_set_delay(HMGL gr, float dt);
void mgl_setup_window(HMGL gr, int autoclf, int showpos, int clf_upd);
void mgl_wnd_toggle_alpha(HMGL gr);
void mgl_wnd_toggle_light(HMGL gr);
void mgl_wnd_toggle_zoom(HMGL gr);
void mgl_wnd_toggle_rotate(HMGL gr);
void mgl_wnd_toggle_no(HMGL gr);
void mgl_wnd_update(HMGL gr);
void mgl_wnd_reload(HMGL gr, int o);
void mgl_wnd_adjust(HMGL gr);
void mgl_wnd_next_frame(HMGL gr);
void mgl_wnd_prev_frame(HMGL gr);
void mgl_wnd_animation(HMGL gr);
#endif
void mgl_set_show_mouse_pos(HMGL gr, int enable);
void mgl_get_last_mouse_pos(HMGL gr, float *x, float *y, float *z);
//void mgl_fltk_thread();
//void mgl_qt_thread();
void mgl_update(HMGL graph);
void mgl_delete_graph(HMGL graph);
/*****************************************************************************/
HMPR mgl_create_parser();
void mgl_delete_parser(HMPR p);
void mgl_scan_func(HMPR p, const wchar_t *line);
void mgl_add_param(HMPR p, int id, const char *str);
void mgl_add_paramw(HMPR p, int id, const wchar_t *str);
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
HMDT mgl_add_var(HMPR, const char *name);
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
HMDT mgl_find_var(HMPR, const char *name);
int mgl_parse(HMGL gr, HMPR p, const char *str, int pos);
int mgl_parsew(HMGL gr, HMPR p, const wchar_t *str, int pos);
void mgl_parse_text(HMGL gr, HMPR p, const char *str);
void mgl_parsew_text(HMGL gr, HMPR p, const wchar_t *str);
void mgl_restore_once(HMPR p);
void mgl_parser_allow_setsize(HMPR p, int a);
/*****************************************************************************/
/*		Setup mglGraph														 */
/*****************************************************************************/
void mgl_set_tick_len(HMGL graph, float len, float stt);
void mgl_set_tick_stl(HMGL gr, const char *stl, const char *sub);
void mgl_set_size(HMGL graph, int width, int height);
void mgl_set_zoom(HMGL gr, float x1, float y1, float x2, float y2);
/*****************************************************************************/
/*		Axis functions														 */
/*****************************************************************************/
void mgl_adjust_ticks(HMGL graph, const char *dir);
void mgl_set_ticks(HMGL graph, char dir, float d, int ns, float org);
void mgl_set_ticks_val(HMGL graph, char dir, int n, double val, const char *lbl, ...);
void mgl_set_ticks_vals(HMGL graph, char dir, int n, float *val, const char **lbl);

void mgl_box(HMGL graph, int ticks);
void mgl_box_str(HMGL graph, const char *col, int ticks);
void mgl_box_rgb(HMGL graph, float r, float g, float b, int ticks);
void mgl_axis(HMGL graph, const char *dir);
void mgl_axis_grid(HMGL graph, const char *dir,const char *pen);
void mgl_label(HMGL graph, char dir, const char *text);
void mgl_label_ext(HMGL graph, char dir, const char *text, float pos, float size, float shift);
void mgl_labelw_ext(HMGL graph, char dir, const wchar_t *text, float pos, float size, float shift);
void mgl_label_xy(HMGL graph, float x, float y, const char *text, const char *fnt, float size);
void mgl_labelw_xy(HMGL graph, float x, float y, const wchar_t *text, const char *fnt, float size);
void mgl_tune_ticks(HMGL graph, int tune, float fact_pos);
void mgl_set_xttw(HMGL graph, const wchar_t *templ);
void mgl_set_yttw(HMGL graph, const wchar_t *templ);
void mgl_set_zttw(HMGL graph, const wchar_t *templ);
void mgl_set_cttw(HMGL graph, const wchar_t *templ);
void mgl_set_xtt(HMGL graph, const char *templ);
void mgl_set_ytt(HMGL graph, const char *templ);
void mgl_set_ztt(HMGL graph, const char *templ);
void mgl_set_ctt(HMGL graph, const char *templ);
/*****************************************************************************/
/*		Simple drawing														 */
/*****************************************************************************/
void mgl_colorbar(HMGL graph, const char *sch,int where);
void mgl_colorbar_ext(HMGL graph, const char *sch, int where, float x, float y, float w, float h);
void mgl_colorbar_val(HMGL graph, HCDT dat, const char *sch,int where);

void mgl_add_legend(HMGL graph, const char *text,const char *style);
void mgl_add_legendw(HMGL graph, const wchar_t *text,const char *style);
void mgl_clear_legend(HMGL graph);
void mgl_legend_xy(HMGL graph, float x, float y, const char *font, float size, float llen);
void mgl_legend(HMGL graph, int where, const char *font, float size, float llen);
void mgl_set_legend_box(HMGL gr, int enable);
void mgl_set_legend_marks(HMGL gr, int num);
/*****************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* _mgl_c_h_ */
