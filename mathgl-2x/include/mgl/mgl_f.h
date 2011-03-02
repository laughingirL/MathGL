/***************************************************************************
 * mgl_f.h is part of Math Graphic Library
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
#ifndef _MGL_F_H_
#define _MGL_F_H_

#include <stdint.h>

#include "mgl/config.h"
#include "mgl/base.h"

#ifdef __cplusplus
extern "C" {
#endif
#define _PR_	((mglParse *)(*p))
/*****************************************************************************/
uintptr_t mgl_create_graph_gl_();
uintptr_t mgl_create_graph_idtf_();
/*uintptr_t mgl_create_graph_glut_(int argc, char **argv, int _(*draw)_(mglGraph *gr, const void *p), const char *title, void _(*reload)_(int *next), const void *par);
uintptr_t mgl_create_graph_fltk_(int argc, char **argv, int _(*draw)_(mglGraph *gr, const void *p), const char *title, void _(*reload)_(int *next), const void *par);*/
uintptr_t mgl_create_graph_zb_(int *width, int *height);
uintptr_t mgl_create_graph_ps_(int *width, int *height);
void mgl_delete_graph_(uintptr_t *graph);
/*****************************************************************************/
uintptr_t mgl_create_parser_();
void mgl_delete_parser_(uintptr_t* p);
void mgl_add_param_(uintptr_t* p, int *id, const char *str, int l);
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
uintptr_t mgl_add_var_(uintptr_t* p, const char *name, int l);
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
uintptr_t mgl_find_var_(uintptr_t* p, const char *name, int l);
int mgl_parse_(uintptr_t* gr, uintptr_t* p, const char *str, int *pos, int l);
void mgl_parse_text_(uintptr_t* gr, uintptr_t* p, const char *str, int l);
void mgl_restore_once_(uintptr_t* p);
void mgl_parser_allow_setsize_(uintptr_t* p, int *a);
/*****************************************************************************/
uintptr_t mgl_create_graph_fltk_(const char *title, int);
uintptr_t mgl_create_graph_qt_(const char *title, int);
void mgl_fltk_run_();
void mgl_qt_run_();
void mgl_get_last_mouse_pos_(uintptr_t *gr, float *x, float *y, float *z);
//void mgl_fltk_thread_();
//void mgl_qt_thread_();

void mgl_wnd_set_delay_(uintptr_t *gr, float *dt);
void mgl_setup_window_(uintptr_t *gr, int *autoclf, int *showpos, int *clf_upd);
void mgl_wnd_toggle_alpha_(uintptr_t *gr);
void mgl_wnd_toggle_light_(uintptr_t *gr);
void mgl_wnd_toggle_zoom_(uintptr_t *gr);
void mgl_wnd_toggle_rotate_(uintptr_t *gr);
void mgl_wnd_toggle_no_(uintptr_t *gr);
void mgl_wnd_update_(uintptr_t *gr);
void mgl_wnd_reload_(uintptr_t *gr, int *o);
void mgl_wnd_adjust_(uintptr_t *gr);
void mgl_wnd_next_frame_(uintptr_t *gr);
void mgl_wnd_prev_frame_(uintptr_t *gr);
void mgl_wnd_animation_(uintptr_t *gr);
/*****************************************************************************/
/*		Setup mglGraph														 */
/*****************************************************************************/
void mgl_set_tick_len_(uintptr_t *graph, float *len, float *stt);
void mgl_set_tick_stl_(uintptr_t *graph, const char *stl, const char *sub, int,int);
void mgl_set_size_(uintptr_t *graph, int *width, int *height);
void mgl_set_zoom_(uintptr_t *gr, float *x1, float *y1, float *x2, float *y2);
/*****************************************************************************/
/*		Axis functions														 */
/*****************************************************************************/
void mgl_tune_ticks_(uintptr_t *gr, int *tune, float *fact_pos);
void mgl_adjust_ticks_(uintptr_t *gr, const char *dir, int l);
void mgl_set_ticks_(uintptr_t *gr, char *dir, float *d, int *ns, float *org, int);

void mgl_box_(uintptr_t *graph, int *ticks);
void mgl_box_str_(uintptr_t *graph, const char *col, int *ticks, int);
void mgl_box_rgb_(uintptr_t *graph, float *r, float *g, float *b, int *ticks);
void mgl_axis_(uintptr_t *graph, const char *dir,int);
void mgl_axis_grid_(uintptr_t *graph, const char *dir,const char *pen,int,int);
void mgl_label_(uintptr_t *graph, const char *dir, const char *text,int,int);
void mgl_label_ext_(uintptr_t *graph, const char *dir, const char *text, float *pos, float *size, float *shift,int,int);
void mgl_label_xy_(uintptr_t *graph, float *x, float *y, const char *text, const char *fnt, float *size,int,int);
void mgl_set_xtt_(uintptr_t * graph, const char *templ,int);
void mgl_set_ytt_(uintptr_t * graph, const char *templ,int);
void mgl_set_ztt_(uintptr_t * graph, const char *templ,int);
void mgl_set_ctt_(uintptr_t * graph, const char *templ,int);
/*****************************************************************************/
/*		Simple drawing														 */
/*****************************************************************************/
void mgl_colorbar_(uintptr_t *graph, const char *sch,int *where,int);
void mgl_colorbar_ext_(uintptr_t *graph, const char *sch,int *where, float *x, float *y, float *w, float *h, int);
void mgl_colorbar_val_(uintptr_t *graph, uintptr_t *dat, const char *sch,int *where,int);
void mgl_add_legend_(uintptr_t *graph, const char *text,const char *style,int,int);
void mgl_clear_legend_(uintptr_t *graph);
void mgl_legend_xy_(uintptr_t *graph, float *x, float *y, const char *font, float *size, float *llen,int);
void mgl_legend_(uintptr_t *graph, int *where, const char *font, float *size, float *llen,int);
void mgl_set_legend_box_(uintptr_t *gr, int *enable);
void mgl_set_legend_marks_(uintptr_t *gr, int *num);
/*****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _mgl_f_h_ */
