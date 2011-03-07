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
/*****************************************************************************/
/*		Axis functions														 */
/*****************************************************************************/

/*****************************************************************************/
/*		Simple drawing														 */
/*****************************************************************************/
/*****************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* _mgl_c_h_ */
