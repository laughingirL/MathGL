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
/*****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _mgl_f_h_ */
