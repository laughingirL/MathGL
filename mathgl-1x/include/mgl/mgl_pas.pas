//**************************************************************************
// mgl_pas.pas is part of Math Graphic Library                             *
// Copyright (C) 2008 Mikhail Barg <mikemvpi@yandex.ru>                    *
//                                                                         *
//   This program is free software; you can redistribute it and/or modify  *
//   it under the terms of the GNU Library General Public License as       *
//   published by the Free Software Foundation; either version 2 of the    *
//   License, or (at your option) any later version.                       *
//                                                                         *
//   This program is distributed in the hope that it will be useful,       *
//   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
//   GNU General Public License for more details.                          *
//                                                                         *
//   You should have received a copy of the GNU Library General Public     *
//   License along with this program; if not, write to the                 *
//   Free Software Foundation, Inc.,                                       *
//   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
//**************************************************************************

unit mgl_pas;

{$IFDEF FPC}
  {$MODE DELPHI }
  {$PACKENUM 4}    (* use 4-byte enums *)
  {$PACKRECORDS C} (* C/C++-compatible record packing *)
{$ELSE}
  {$MINENUMSIZE 4} (* use 4-byte enums *)
{$ENDIF}

{$IFDEF DARWIN}
  {$linklib mgl}
{$ENDIF}

interface

uses
{$IFDEF MSWINDOWS}
  Windows, Graphics,
{$ENDIF}
  Math;

const
  MGL_VERSION = 11.2;

//* This define enables double precision in MathGL */
  MGL_USE_DOUBLE = 0;

const
{$IFDEF MSWINDOWS}
  //win - .dll
  libmgl = 'mgl.dll';
  libmglglut = 'libmgl-glut.dll';
  libmglfltk = 'libmgl-fltk.dll';
  libmglqt   = 'libmgl-qt.dll';
{$ELSE}
{$IFDEF LINUX}
  //linux - .so
  libmgl = 'mgl.so';
  libmglglut = 'libmgl-glut.so';
  libmglfltk = 'libmgl-fltk.so';
  libmglqt   = 'libmgl-qt.so';
{$ELSE}
{$IFDEF DARWIN}
  //darwin - .dylib
  libmgl = 'mgl.dylib';
  libmglglut = 'libmgl-glut.dylib';
  libmglfltk = 'libmgl-fltk.dylib';
  libmglqt   = 'libmgl-qt.dylib';
{$ELSE}
  // other platforms?

{$ENDIF}
{$ENDIF}
{$ENDIF}

{$IF (MGL_USE_DOUBLE = 0)}
  type mreal = double;
{$ELSE}
  type mreal = real;
{$IFEND}
  Pmreal = ^mreal;

type TNGLDraw = record
     end;
type TMGLGraph = record
     end;
type TMGLData = record
     end;
type TMGLParse = record
     end;

type HMDR = ^TNGLDraw;
type HMGL = ^TMGLGraph;
type HMDT = ^TMGLData;
type HMPR = ^TMGLParse;

//*****************************************************************************/

//HMGL mgl_create_graph_gl();
function mgl_create_graph_gl(): HMGL; cdecl; external libmgl;

//HMGL mgl_create_graph_zb(int width, int height);
function mgl_create_graph_zb(width, height: integer): HMGL; cdecl; external libmgl;

//HMGL mgl_create_graph_ps(int width, int height);
function mgl_create_graph_ps(width, height: integer): HMGL; cdecl; external libmgl;

//HMGL mgl_create_graph_glut(int (*draw)(HMGL gr, void *p), const char *title, void *par);
type TGLUTDrawFunction = function (gr: HMGL; p: pointer): integer; cdecl;
type PPChar = ^PChar;
function mgl_create_graph_glut(draw: TGLUTDrawFunction; const title: PChar; par: pointer): HMGL; cdecl; external libmglglut;

//HMGL mgl_create_graph_fltk(int (*draw)(HMGL gr, void *p), const char *title, void *par);
function mgl_create_graph_fltk(draw: TGLUTDrawFunction; const title: PChar; par: pointer): HMGL; cdecl; external libmglfltk;

//HMGL mgl_create_graph_qt(int (*draw)(HMGL gr, void *p), const char *title, void *par);
function mgl_create_graph_qt(draw: TGLUTDrawFunction; const title: PChar; par: pointer): HMGL; cdecl; external libmglqt;

//HMGL mgl_create_graph_idtf();
function mgl_create_graph_idtf(): HMGL; cdecl; external libmgl;

//void mgl_fltk_run();
procedure mgl_fltk_run(); cdecl; external libmglfltk;

//void mgl_qt_run();
procedure mgl_qt_run(); cdecl; external libmglqt;

//void mgl_wnd_set_delay(HMGL gr, mreal dt);
procedure mgl_wnd_set_delay(gr: HMGL; dt: mreal); cdecl; external libmgl;

//void mgl_wnd_set_auto_clf(HMGL gr, int val);
procedure mgl_wnd_set_auto_clf(gr: HMGL; val: integer); cdecl; external libmgl;

//void mgl_wnd_set_show_mouse_pos(HMGL gr, int val);
procedure mgl_wnd_set_show_mouse_pos(gr: HMGL; vaj: integer); cdecl; external libmgl;

//void mgl_wnd_set_clf_update(HMGL gr, int val);
procedure mgl_wnd_set_clf_update(gr: HMGL; val: integer); cdecl; external libmgl;

//void mgl_wnd_toggle_alpha(HMGL gr);
procedure mgl_wnd_toggle_alpha(gr: HMGL); cdecl; external libmgl;

//void mgl_wnd_toggle_light(HMGL gr);
procedure mgl_wnd_toggle_light(gr: HMGL); cdecl; external libmgl;

//void mgl_wnd_toggle_zoom(HMGL gr);
procedure mgl_wnd_toggle_zoom(gr: HMGL); cdecl; external libmgl;

//void mgl_wnd_toggle_rotate(HMGL gr);
procedure mgl_wnd_toggle_rotate(gr: HMGL); cdecl; external libmgl;

//void mgl_wnd_toggle_no(HMGL gr);
procedure mgl_wnd_toggle_no(gr: HMGL); cdecl; external libmgl;

//void mgl_wnd_update(HMGL gr);
procedure mgl_wnd_update(gr: HMGL); cdecl; external libmgl;

//void mgl_wnd_reload(HMGL gr, int o);
procedure mgl_wnd_reload(gr: HMGL; o: integer); cdecl; external libmgl;

//void mgl_wnd_adjust(HMGL gr);
procedure mgl_wnd_adjust(gr: HMGL); cdecl; external libmgl;

//void mgl_wnd_next_frame(HMGL gr);
procedure mgl_wnd_next_frame(gr: HMGL); cdecl; external libmgl;

//void mgl_wnd_prev_frame(HMGL gr);
procedure mgl_wnd_prev_frame(gr: HMGL); cdecl; external libmgl;

//void mgl_wnd_animation(HMGL gr);
procedure mgl_wnd_animation(gr: HMGL); cdecl; external libmgl;

//void mgl_set_show_mouse_pos(HMGL gr, int enable);
procedure mgl_set_show_mouse_pos(graph: HMGL; enable: integer); cdecl; external libmgl;

//void mgl_get_last_mouse_pos(HMGL gr, mreal *x, mreal *y, mreal *z);
procedure mgl_get_last_mouse_pos(gr: HMGL; x: Pmreal; y: Pmreal; z: Pmreal); cdecl; external libmgl;

//void mgl_calc_xyz(HMGL gr, int xs, int ys, mreal *x, mreal *y, mreal *z);
procedure mgl_calc_xyz(gr: HMGL; xs: integer; ys: integer; x: Pmreal; y: Pmreal; z: Pmreal); cdecl; external libmgl;

//void mgl_calc_scr(HMGL gr, mreal x, mreal y, mreal z, int *xs, int *ys);
procedure mgl_calc_scr(gr: HMGL; x: Pmreal; y: Pmreal; z: Pmreal; xs: Pinteger; ys: Pinteger); cdecl; external libmgl;

//void mgl_update(HMGL graph);
procedure mgl_update(graph: HMGL); cdecl; external libmgl;

//void mgl_delete_graph(HMGL graph);
procedure mgl_delete_graph(graph: HMGL); cdecl; external libmgl;

//*****************************************************************************/

//HMDT mgl_create_data();
function mgl_create_data(): HMDT; cdecl; external libmgl;

//HMDT mgl_create_data_size(int nx, int ny, int nz);
function mgl_create_data_size(nx, ny, nz: integer): HMDT; cdecl; external libmgl;

//HMDT mgl_create_data_file(const char *fname);
function mgl_create_data_file(const fname: PChar): HMDT; cdecl; external libmgl;

//void mgl_delete_data(HMDT dat);
procedure mgl_delete_data(dat: HMDT); cdecl; external libmgl;

//*****************************************************************************/

//HMPR mgl_create_parser();
function mgl_create_parser(): HMPR; cdecl; external libmgl;

//void mgl_delete_parser(HMPR p);
procedure mgl_delete_parser(p: HMPR); cdecl; external libmgl;

//void mgl_scan_func(HMPR p, const wchar_t *line);
procedure mgl_scan_func(p: HMPR; const line: PWideChar); cdecl; external libmgl;

//void mgl_add_param(HMPR p, int id, const char *str);
procedure mgl_add_param(p: HMPR; id: integer; const str: PChar); cdecl; external libmgl;

//void mgl_add_paramw(HMPR p, int id, const wchar_t *str);
procedure mgl_add_paramw(p: HMPR; id: integer; const str: PWideChar); cdecl; external libmgl;

//*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
//HMDT mgl_add_var(HMPR, const char *name);
function mgl_add_var(p: HMPR; const name: PChar): HMDT; cdecl; external libmgl;

//*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
//HMDT mgl_find_var(HMPR, const char *name);
function mgl_find_var(p: HMPR; const name: PChar): HMDT; cdecl; external libmgl;

//int mgl_parse(HMGL gr, HMPR p, const char *str, int pos);
function mgl_parse(gr: HMGL; p: HMPR; const str: PChar; pos: integer): integer; cdecl; external libmgl;

//int mgl_parsew(HMGL gr, HMPR p, const wchar_t *str, int pos);
function mgl_parsew(gr: HMGL; p: HMPR; const str: PWideChar; pos: integer): integer; cdecl; external libmgl;

//void mgl_parse_text(HMGL gr, HMPR p, const char *str);
procedure mgl_parse_text(gr: HMGL; p: HMPR; const str: PChar); cdecl; external libmgl;

//void mgl_parsew_text(HMGL gr, HMPR p, const wchar_t *str);
procedure mgl_parsew_text(gr: HMGL; p: HMPR; const str: PWideChar); cdecl; external libmgl;

//void mgl_restore_once(HMPR p);
procedure mgl_restore_once(p: HMPR); cdecl; external libmgl;

//void mgl_parser_allow_setsize(HMPR p, int a);
procedure mgl_parser_allow_setsize(p: HMPR; a: integer); cdecl; external libmgl;


//*****************************************************************************/
//*             Setup mglGraph                                                */
//*****************************************************************************/

//void mgl_set_def_param(HMGL gr);
procedure mgl_set_def_param(graph: HMGL); cdecl; external libmgl;

//void mgl_set_palette(HMGL gr, const char *colors);
procedure mgl_set_palette(graph: HMGL; colors: PChar); cdecl; external libmgl;

//void mgl_set_pal_color(HMGL graph, int n, mreal r, mreal g, mreal b);
procedure mgl_set_pal_color(graph: HMGL; n: integer; r, g, b: mreal); cdecl; external libmgl;

//void mgl_set_pal_num(HMGL graph, int num);
procedure mgl_set_pal_num(graph: HMGL; num: integer); cdecl; external libmgl;

//void mgl_set_rotated_text(HMGL graph, int rotated);
procedure mgl_set_rotated_text(graph: HMGL; rotated: integer); cdecl; external libmgl;

//void mgl_set_cut(HMGL graph, int cut);
procedure mgl_set_cut(graph: HMGL; cut: integer); cdecl; external libmgl;

//void mgl_set_cut_box(HMGL gr, mreal x1,mreal y1,mreal z1,mreal x2,mreal y2,mreal z2);
procedure mgl_set_cut_box(gr: HMGL; x1, y1, z1, x2, y2, z2: mreal); cdecl; external libmgl;

//void mgl_set_tick_len(HMGL graph, mreal len);
procedure mgl_set_tick_len(graph: HMGL; len: mreal); cdecl; external libmgl;

//void mgl_set_bar_width(HMGL graph, mreal width);
procedure mgl_set_bar_width(graph: HMGL; width: mreal); cdecl; external libmgl;

//void mgl_set_base_line_width(HMGL gr, mreal size);
procedure mgl_set_base_line_width(gr: HMGL; size: mreal); cdecl; external libmgl;

//void mgl_set_mark_size(HMGL graph, mreal size);
procedure mgl_set_mark_size(graph: HMGL; size: mreal); cdecl; external libmgl;

//void mgl_set_arrow_size(HMGL graph, mreal size);
procedure mgl_set_arrow_size(graph: HMGL; size: mreal); cdecl; external libmgl;

//void mgl_set_font_size(HMGL graph, mreal size);
procedure mgl_set_font_size(graph: HMGL; size: mreal); cdecl; external libmgl;

//void mgl_set_font_def(HMGL graph, const char *fnt);
procedure mgl_set_font_def(graph: HMGL; fnt: PChar); cdecl; external libmgl;

//void mgl_set_alpha_default(HMGL graph, mreal alpha);
procedure mgl_set_alpha_default(graph: HMGL; alpha: mreal); cdecl; external libmgl;

//void mgl_set_size(HMGL graph, int width, int height);
procedure mgl_set_size(graph: HMGL; width, height: integer); cdecl; external libmgl;

//void mgl_set_axial_dir(HMGL graph, char dir);
procedure mgl_set_axial_dir(graph: HMGL; dir: char); cdecl; external libmgl;

//void mgl_set_meshnum(HMGL graph, int num);
procedure mgl_set_meshnum(graph: HMGL; num: integer); cdecl; external libmgl;

//void mgl_set_zoom(HMGL gr, mreal x1, mreal y1, mreal x2, mreal y2);
procedure mgl_set_zoom(gr: HMGL; x1, y1, x2, y2: mreal); cdecl; external libmgl;

//void mgl_set_plotfactor(HMGL gr, mreal val);
procedure mgl_set_plotfactor(gr: HMGL; val: mreal); cdecl; external libmgl;

//void mgl_set_draw_face(HMGL gr, int enable);
procedure mgl_set_draw_face(gr: HMGL; enable: integer); cdecl; external libmgl;

//void mgl_set_scheme(HMGL gr, const char *sch);
procedure mgl_set_scheme(gr: HMGL; const sch: PChar); cdecl; external libmgl;

//void mgl_load_font(HMGL gr, const char *name, const char *path);
procedure mgl_load_font(gr: HMGL; const name: PChar; const path: char); cdecl; external libmgl;

//void mgl_copy_font(HMGL gr, HMGL gr_from);
procedure mgl_copy_font(gr, gr_from: HMGL); cdecl; external libmgl;

//void mgl_restore_font(HMGL gr);
procedure mgl_restore_font(gr: HMGL); cdecl; external libmgl;

//int mgl_get_warn(HMGL gr);
function mgl_get_warn(gr: HMGL): integer; cdecl; external libmgl;

//*****************************************************************************/
//*             Export to file or to memory                                   */
//*****************************************************************************/

//void mgl_show_image(HMGL graph, const char *viewer, int keep);
procedure mgl_show_image(graph: HMGL; const viewer: PChar; keep: integer); cdecl; external libmgl;

//void mgl_write_frame(HMGL graph, const char *fname,const char *descr);
procedure mgl_write_frame(graph: HMGL; const fname: PChar; const descr: PChar); cdecl; external libmgl;

//void mgl_write_bmp(HMGL graph, const char *fname,const char *descr);
procedure mgl_write_bmp(graph: HMGL; const fname: PChar; const descr: PChar); cdecl; external libmgl;

//void mgl_write_jpg(HMGL graph, const char *fname,const char *descr);
procedure mgl_write_jpg(graph: HMGL; const fname: PChar; const descr: PChar); cdecl; external libmgl;

//void mgl_write_png(HMGL graph, const char *fname,const char *descr);
procedure mgl_write_png(graph: HMGL; const fname: PChar; const descr: PChar); cdecl; external libmgl;

//void mgl_write_png_solid(HMGL graph, const char *fname,const char *descr);
procedure mgl_write_png_solid(graph: HMGL; const fname: PChar; const descr: PChar); cdecl; external libmgl;

//void mgl_write_eps(HMGL graph, const char *fname,const char *descr);
procedure mgl_write_eps(graph: HMGL; const fname: PChar; const descr: PChar); cdecl; external libmgl;

//void mgl_write_svg(HMGL graph, const char *fname,const char *descr);
procedure mgl_write_svg(graph: HMGL; const fname: PChar; const descr: PChar); cdecl; external libmgl;

//void mgl_write_idtf(HMGL graph, const char *fname,const char *descr);
procedure mgl_write_idtf(graph: HMGL; const fname: PChar; const descr: PChar); cdecl; external libmgl;

//void mgl_write_gif(HMGL graph, const char *fname,const char *descr);
procedure mgl_write_gif(graph: HMGL; const fname: PChar; const descr: PChar); cdecl; external libmgl;

//void mgl_start_gif(HMGL graph, const char *fname,int ms);
procedure mgl_start_gif(graph: HMGL; const fname: PChar; ms: integer); cdecl; external libmgl;

//void mgl_close_gif(HMGL graph);
procedure mgl_close_gif(graph: HMGL); cdecl; external libmgl;

//const unsigned char *mgl_get_rgb(HMGL graph);
function mgl_get_rgb(graph: HMGL): PByte; cdecl; external libmgl;

//const unsigned char *mgl_get_rgba(HMGL graph);
function mgl_get_rgba(graph: HMGL): PByte; cdecl; external libmgl;

//int mgl_get_width(HMGL graph);
function mgl_get_width(graph: HMGL): integer; cdecl; external libmgl;

//int mgl_get_height(HMGL graph);
function mgl_get_height(graph: HMGL): integer; cdecl; external libmgl;

//*****************************************************************************/
//*             Setup frames transparency (alpha) and lightning               */
//*****************************************************************************/

//int mgl_new_frame(HMGL graph);
function mgl_new_frame(graph: HMGL): integer; cdecl; external libmgl;

//void mgl_end_frame(HMGL graph);
procedure mgl_end_frame(graph: HMGL); cdecl; external libmgl;

//int mgl_get_num_frame(HMGL graph);
function mgl_get_num_frame(graph: HMGL): integer; cdecl; external libmgl;

//void mgl_reset_frames(HMGL graph);
procedure mgl_reset_frames(graph: HMGL); cdecl; external libmgl;

//void mgl_set_transp_type(HMGL graph, int type);
procedure mgl_set_transp_type(graph: HMGL; ttype: integer); cdecl; external libmgl;

//void mgl_set_transp(HMGL graph, int enable);
procedure mgl_set_transp(graph: HMGL; enable: integer); cdecl; external libmgl;

//void mgl_set_alpha(HMGL graph, int enable);
procedure mgl_set_alpha(graph: HMGL; enable: integer); cdecl; external libmgl;

//void mgl_set_fog(HMGL graph, mreal d, mreal dz);
procedure mgl_set_fog(graph: HMGL; d, dz: mreal); cdecl; external libmgl;

//void mgl_set_light(HMGL graph, int enable);
procedure mgl_set_light(graph: HMGL; enable: integer); cdecl; external libmgl;

//void mgl_set_light_n(HMGL gr, int n, int enable);
procedure mgl_set_light_n(graph: HMGL; n: integer; enable: integer); cdecl; external libmgl;

//void mgl_add_light(HMGL graph, int n, mreal x, mreal y, mreal z, char c);
procedure mgl_add_light(graph: HMGL; n: integer; x, y, z: mreal; c: char); cdecl; external libmgl;

//void mgl_add_light_rgb(HMGL graph, int n, mreal x, mreal y, mreal z, int infty, mreal r, mreal g, mreal b, mreal i);
procedure mgl_add_light_rgb(graph: HMGL; n: integer; x, y, z: mreal; infty: integer; r, g, b, i: mreal); cdecl; external libmgl;

//void mgl_set_ambbr(HMGL gr, mreal i);
procedure mgl_set_ambbr(graph: HMGL; i: mreal); cdecl; external libmgl;

//*****************************************************************************/
//*             Scale and rotate                                              */
//*****************************************************************************/

//void mgl_mat_pop(HMGL gr);
procedure mgl_mat_pop(graph: HMGL); cdecl; external libmgl;

//void mgl_mat_push(HMGL gr);
procedure mgl_mat_push(graph: HMGL); cdecl; external libmgl;

//void mgl_identity(HMGL graph, int rel);
procedure mgl_identity(graph: HMGL; rel: integer); cdecl; external libmgl;

//void mgl_clf(HMGL graph);
procedure mgl_clf(graph: HMGL); cdecl; external libmgl;

//void mgl_flush(HMGL gr);
procedure mgl_flush(gr: HMGL); cdecl; external libmgl;

//void mgl_clf_rgb(HMGL graph, mreal r, mreal g, mreal b);
procedure mgl_clf_rgb(graph: HMGL; r, g, b: mreal); cdecl; external libmgl;

//void mgl_subplot(HMGL graph, int nx,int ny,int m);
procedure mgl_subplot(graph: HMGL; nx, ny, m: integer); cdecl; external libmgl;

//void mgl_subplot_d(HMGL graph, int nx,int ny,int m, mreal dx, mreal dy);
procedure mgl_subplot_d(graph: HMGL; nx, ny, m: integer; dx, dy: mreal); cdecl; external libmgl;

//void mgl_subplot_s(HMGL graph, int nx,int ny,int m,const char *style);
procedure mgl_subplot_s(graph: HMGL; nx, ny, m: integer; style: Pchar); cdecl; external libmgl;

//void mgl_inplot(HMGL graph, mreal x1,mreal x2,mreal y1,mreal y2);
procedure mgl_inplot(graph: HMGL; x1, x2, y1, y2: mreal); cdecl; external libmgl;

//void mgl_relplot(HMGL graph, mreal x1,mreal x2,mreal y1,mreal y2);
procedure mgl_relplot(graph: HMGL; x1, x2, y1, y2: mreal); cdecl; external libmgl;

//void mgl_columnplot(HMGL graph, int num, int ind);
procedure mgl_columnplot(graph: HMGL; num, ind: integer); cdecl; external libmgl;

//void mgl_columnplot_d(HMGL graph, int num, int ind, mreal d);
procedure mgl_columnplot_d(graph: HMGL; num, ind: integer; d: mreal); cdecl; external libmgl;

//void mgl_stickplot(HMGL graph, int num, int ind, mreal tet, mreal phi);
procedure mgl_stickplot(graph: HMGL; num, ind: integer; tet, phi: mreal); cdecl; external libmgl;

//void mgl_aspect(HMGL graph, mreal Ax,mreal Ay,mreal Az);
procedure mgl_aspect(graph: HMGL; Ax, Ay, Az: mreal); cdecl; external libmgl;

//void mgl_rotate(HMGL graph, mreal TetX,mreal TetZ,mreal TetY);
procedure mgl_rotate(graph: HMGL; TetX, TetZ, TetY: mreal); cdecl; external libmgl;

//void mgl_rotate_vector(HMGL graph, mreal Tet,mreal x,mreal y,mreal z);
procedure mgl_rotate_vector(graph: HMGL; Tet, x, y, z: mreal); cdecl; external libmgl;

//void mgl_perspective(HMGL graph, mreal val);
procedure mgl_perspective(graph: HMGL; val: mreal); cdecl; external libmgl;

//*****************************************************************************/
//*             Axis functions                                                */
//*****************************************************************************/

//void mgl_adjust_ticks(HMGL graph, const char *dir);
procedure mgl_adjust_ticks(graph: HMGL; dir: Pchar); cdecl; external libmgl;

//void mgl_set_ticks(HMGL graph, mreal DX, mreal DY, mreal DZ);
procedure mgl_set_ticks(graph: HMGL; DX, DY, DZ: mreal); cdecl; external libmgl;

//void mgl_set_subticks(HMGL graph, int NX, int NY, int NZ);
procedure mgl_set_subticks(graph: HMGL; NX, NY, NZ: integer); cdecl; external libmgl;

//void mgl_set_ticks_dir(HMGL graph, char dir, mreal d, int ns, mreal org);
procedure mgl_set_ticks_dir(graph: HMGL; dir: char; d: mreal; ns: integer; org:mreal); cdecl; external libmgl;

//void mgl_set_ticks_val(HMGL graph, char dir, int n, double val, const char *lbl, ...);
// oops.. variadic paramerers functions are not supported

//void mgl_set_ticks_vals(HMGL graph, char dir, int n, mreal *val, const char **lbl);
procedure mgl_set_ticks_vals(graph: HMGL; dir: char; n: integer; val: Pmreal; lbl: PChar); cdecl; external libmgl;

//void mgl_set_caxis(HMGL graph, mreal C1,mreal C2);
procedure mgl_set_caxis(graph: HMGL; C1, C2: mreal); cdecl; external libmgl;

//void mgl_set_axis(HMGL graph, mreal x1, mreal y1, mreal z1, mreal x2, mreal y2, mreal z2, mreal x0, mreal y0, mreal z0);
procedure mgl_set_axis(graph: HMGL; x1, y1, z1, x2, y2, z2, x0, y0, z0: mreal); cdecl; external libmgl;

//void mgl_set_axis_3d(HMGL graph, mreal x1, mreal y1, mreal z1, mreal x2, mreal y2, mreal z2);
procedure mgl_set_axis_3d(graph: HMGL; x1, y1, z1, x2, y2, z2: mreal); cdecl; external libmgl;

//void mgl_set_axis_2d(HMGL graph, mreal x1, mreal y1, mreal x2, mreal y2);
procedure mgl_set_axis_2d(graph: HMGL; x1, y1, x2, y2: mreal); cdecl; external libmgl;

//inline void mgl_set_ranges(HMGL graph, mreal x1, mreal x2, mreal y1, mreal y2, mreal z1, mreal z2)
//{	mgl_set_axis_3d(graph, x1,y1,z1,x2,y2,z2);	};
procedure mgl_set_ranges(graph: HMGL; x1, x2, y1, y2, z1, z2: mreal); inline;

//void mgl_set_origin(HMGL graph, mreal x0, mreal y0, mreal z0);
procedure mgl_set_origin(graph: HMGL; x0, y0, z0: mreal); cdecl; external libmgl;

//void mgl_set_tick_origin(HMGL graph, mreal x0, mreal y0, mreal z0);
procedure mgl_set_tick_origin(graph: HMGL; x0, y0, z0: mreal); cdecl; external libmgl;

//void mgl_set_crange(HMGL graph, const HMDT a, int add);
procedure mgl_set_crange(graph: HMGL; const a: HMDT; add: integer); cdecl; external libmgl;

//void mgl_set_xrange(HMGL graph, const HMDT a, int add);
procedure mgl_set_xrange(graph: HMGL; const a: HMDT; add: integer); cdecl; external libmgl;

//void mgl_set_yrange(HMGL graph, const HMDT a, int add);
procedure mgl_set_yrange(graph: HMGL; const a: HMDT; add: integer); cdecl; external libmgl;

//void mgl_set_zrange(HMGL graph, const HMDT a, int add);
procedure mgl_set_zrange(graph: HMGL; const a: HMDT; add: integer); cdecl; external libmgl;

//void mgl_set_auto(HMGL graph, mreal x1, mreal x2, mreal y1, mreal y2, mreal z1, mreal z2);
procedure mgl_set_auto(graph: HMGL; x1, x2, y1, y2, z1, z2: mreal); cdecl; external libmgl;

//void mgl_set_func(HMGL graph, const char *EqX,const char *EqY,const char *EqZ);
procedure mgl_set_func(graph: HMGL; const EqX: PChar; const EqY: PChar; const EqZ: PChar); cdecl; external libmgl;

//void mgl_set_func_ext(HMGL graph, const char *EqX,const char *EqY,const char *EqZ,const char *EqA);
procedure mgl_set_func_ext(graph: HMGL; const EqX: PChar; const EqY: PChar; const EqZ: PChar; const EqA: PChar); cdecl; external libmgl;

//void mgl_set_coor(HMGL gr, int how);
procedure mgl_set_coor(graph: HMGL; how: integer); cdecl; external libmgl;

//void mgl_set_ternary(HMGL gr, int enable);
procedure mgl_set_ternary(graph: HMGL; enable: integer); cdecl; external libmgl;

//void mgl_set_cutoff(HMGL graph, const char *EqC);
procedure mgl_set_cutoff(graph: HMGL; const EqC: PChar); cdecl; external libmgl;

//void mgl_box(HMGL graph, int ticks);
procedure mgl_box(graph: HMGL; ticks: integer); cdecl; external libmgl;

//void mgl_box_str(HMGL graph, const char *col, int ticks);
procedure mgl_box_str(graph: HMGL; const col: PChar; ticks: integer); cdecl; external libmgl;

//void mgl_box_rgb(HMGL graph, mreal r, mreal g, mreal b, int ticks);
procedure mgl_box_rgb(graph: HMGL; r, g, b: mreal; ticks: integer); cdecl; external libmgl;

//void mgl_axis(HMGL graph, const char *dir);
procedure mgl_axis(graph: HMGL; const dir: PChar); cdecl; external libmgl;

//void mgl_axis_grid(HMGL graph, const char *dir,const char *pen);
procedure mgl_axis_grid(graph: HMGL; const dir: PChar; const pen: PChar); cdecl; external libmgl;

//void mgl_label(HMGL graph, char dir, const char *text);
procedure mgl_label(graph: HMGL; dir: char; const text: PChar); cdecl; external libmgl;

//void mgl_label_ext(HMGL graph, char dir, const char *text, mreal pos, mreal size, mreal shift);
procedure mgl_label_ext(graph: HMGL; dir: char; const text: PChar; pos: mreal; size: mreal; shift: mreal); cdecl; external libmgl;

//void mgl_labelw_ext(HMGL graph, char dir, const wchar_t *text, mreal pos, mreal size, mreal shift);
procedure mgl_labelw_ext(graph: HMGL; dir: char; const text: PWideChar; pos: mreal; size: mreal; shift: mreal); cdecl; external libmgl;

//void mgl_label_xy(HMGL graph, mreal x, mreal y, const char *text, const char *fnt, mreal size);
procedure mgl_label_xy(graph: HMGL; x, y: mreal; const text: PChar; fnt: PChar; size: mreal); cdecl; external libmgl;

//void mgl_labelw_xy(HMGL graph, mreal x, mreal y, const wchar_t *text, const char *fnt, mreal size);
procedure mgl_labelw_xy(graph: HMGL; x, y: mreal; const text: PWideChar; fnt: PChar; size: mreal); cdecl; external libmgl;

//void mgl_tune_ticks(HMGL graph, int tune, mreal fact_pos);
procedure mgl_tune_ticks(graph: HMGL; tune: integer; fact_pos: mreal); cdecl; external libmgl;

//void mgl_set_xttw(HMGL graph, const wchar_t *templ);
procedure mgl_set_xttw(graph: HMGL; const templ: PWideChar); cdecl; external libmgl;

//void mgl_set_yttw(HMGL graph, const wchar_t *templ);
procedure mgl_set_yttw(graph: HMGL; const templ: PWideChar); cdecl; external libmgl;

//void mgl_set_zttw(HMGL graph, const wchar_t *templ);
procedure mgl_set_zttw(graph: HMGL; const templ: PWideChar); cdecl; external libmgl;

//void mgl_set_cttw(HMGL graph, const wchar_t *templ);
procedure mgl_set_cttw(graph: HMGL; const templ: PWideChar); cdecl; external libmgl;

//void mgl_set_xtt(HMGL graph, const char *templ);
procedure mgl_set_xtt(graph: HMGL; const templ: PChar); cdecl; external libmgl;

//void mgl_set_ytt(HMGL graph, const char *templ);
procedure mgl_set_ytt(graph: HMGL; const templ: PChar); cdecl; external libmgl;

//void mgl_set_ztt(HMGL graph, const char *templ);
procedure mgl_set_ztt(graph: HMGL; const templ: PChar); cdecl; external libmgl;

//void mgl_set_ctt(HMGL graph, const char *templ);
procedure mgl_set_ctt(graph: HMGL; const templ: PChar); cdecl; external libmgl;

//*****************************************************************************/
//*             Simple drawing                                                */
//*****************************************************************************/

//void mgl_ball(HMGL graph, mreal x,mreal y,mreal z);
procedure mgl_ball(graph: HMGL; x, y, z: mreal); cdecl; external libmgl;

//void mgl_ball_rgb(HMGL graph, mreal x, mreal y, mreal z, mreal r, mreal g, mreal b, mreal alpha);
procedure mgl_ball_rgb(graph: HMGL; x, y, z, r, g, b, alpha: mreal); cdecl; external libmgl;

//void mgl_ball_str(HMGL graph, mreal x, mreal y, mreal z, const char *col);
procedure mgl_ball_str(graph: HMGL; x, y, z: mreal; const col: PChar); cdecl; external libmgl;

//void mgl_line(HMGL graph, mreal x1, mreal y1, mreal z1, mreal x2, mreal y2, mreal z2, const char *pen,int n);
procedure mgl_line(graph: HMGL; x1, y1, z1, x2, y2, z2: mreal; const pen: PChar; n: integer); cdecl; external libmgl;

//void mgl_facex(HMGL graph, mreal x0, mreal y0, mreal z0, mreal wy, mreal wz, const char *stl, mreal dx, mreal dy);
procedure mgl_facex(graph: HMGL; x0, y0, z0, wy, wz: mreal; const stl: PChar; dx, dy: mreal); cdecl; external libmgl;

//void mgl_facey(HMGL graph, mreal x0, mreal y0, mreal z0, mreal wx, mreal wz, const char *stl, mreal dx, mreal dy);
procedure mgl_facey(graph: HMGL; x0, y0, z0, wx, wz: mreal; const stl: PChar; dx, dy: mreal); cdecl; external libmgl;

//void mgl_facez(HMGL graph, mreal x0, mreal y0, mreal z0, mreal wx, mreal wy, const char *stl, mreal dx, mreal dy);
procedure mgl_facez(graph: HMGL; x0, y0, z0, wx, wy: mreal; const stl: PChar; dx, dy: mreal); cdecl; external libmgl;

//void mgl_curve(HMGL graph, mreal x1, mreal y1, mreal z1, mreal dx1, mreal dy1, mreal dz1, mreal x2, mreal y2, mreal z2, mreal dx2, mreal dy2, mreal dz2, const char *pen,int n);
procedure mgl_curve(graph: HMGL; x1, y1, z1, dx1, dy1, dz1, x2, y2, z2, dx2, dy2, dz2: mreal; const pen: PChar; n: integer); cdecl; external libmgl;

//void mgl_puts(HMGL graph, mreal x, mreal y, mreal z,const char *text);
procedure mgl_puts(graph: HMGL; x, y, z: mreal; const text: PChar); cdecl; external libmgl;

//void mgl_putsw(HMGL graph, mreal x, mreal y, mreal z,const wchar_t *text);
procedure mgl_putsw(graph: HMGL; x, y, z: mreal; const text: PWideChar); cdecl; external libmgl;

//void mgl_puts_dir(HMGL graph, mreal x, mreal y, mreal z, mreal dx, mreal dy, mreal dz, const char *text, mreal size);
procedure mgl_puts_dir(graph: HMGL; x, y, z, dx, dy, dz: mreal; const text: PChar; size: mreal); cdecl; external libmgl;

//void mgl_putsw_dir(HMGL graph, mreal x, mreal y, mreal z, mreal dx, mreal dy, mreal dz, const wchar_t *text, mreal size);
procedure mgl_putsw_dir(graph: HMGL; x, y, z, dx, dy, dz: mreal; const text: PWideChar; size: mreal); cdecl; external libmgl;

//void mgl_text(HMGL graph, mreal x, mreal y, mreal z,const char *text);
procedure mgl_text(graph: HMGL; x, y, z: mreal; const text: PChar); cdecl; external libmgl;

//void mgl_title(HMGL graph, const char *text, const char *fnt, mreal size);
procedure mgl_title(graph: HMGL; const text: PChar; const fnt: PChar; size: mreal); cdecl; external libmgl;

//void mgl_titlew(HMGL graph, const wchar_t *text, const char *fnt, mreal size);
procedure mgl_titlew(graph: HMGL; const text: PWideChar; const fnt: PChar; size: mreal); cdecl; external libmgl;

//void mgl_putsw_ext(HMGL graph, mreal x, mreal y, mreal z,const wchar_t *text,const char *font,mreal size,char dir);
procedure mgl_putsw_ext(graph: HMGL; x, y, z: mreal; const text: PWideChar; const font: PChar; size: mreal; dir: char); cdecl; external libmgl;

//void mgl_puts_ext(HMGL graph, mreal x, mreal y, mreal z,const char *text,const char *font,mreal size,char dir);
procedure mgl_puts_ext(graph: HMGL; x, y, z: mreal; const text: PChar; const font: PChar; size: mreal; dir: char); cdecl; external libmgl;

//void mgl_text_ext(HMGL graph, mreal x, mreal y, mreal z,const char *text,const char *font,mreal size,char dir);
procedure mgl_text_ext(graph: HMGL; x, y, z: mreal; const text: PChar; const font: PChar; size: mreal; dir: char); cdecl; external libmgl;

//void mgl_colorbar(HMGL graph, const char *sch,int where);
procedure mgl_colorbar(graph: HMGL; const sch: PCHar; where: integer); cdecl; external libmgl;

//void mgl_colorbar_ext(HMGL graph, const char *sch, int where, mreal x, mreal y, mreal w, mreal h);
procedure mgl_colorbar_ext(graph: HMGL; const sch: PCHar; where: integer; x, y, w, h: mreal); cdecl; external libmgl;

//void mgl_colorbar_val(HMGL graph, const HMDT dat, const char *sch,int where);
procedure mgl_colorbar_val(graph: HMGL; const dat: HMDT; const sch: PChar; where: integer); cdecl; external libmgl;

//void mgl_simple_plot(HMGL graph, const HMDT a, int type, const char *stl);
procedure mgl_simple_plot(graph: HMGL; const a: HMDT; ttype: integer; stl: PChar); cdecl; external libmgl;

//void mgl_add_legend(HMGL graph, const char *text,const char *style);
procedure mgl_add_legend(graph: HMGL; const text: PChar; const style: PChar); cdecl; external libmgl;

//void mgl_add_legendw(HMGL graph, const wchar_t *text,const char *style);
procedure mgl_add_legendw(graph: HMGL; const text: PWideChar; const style: PChar); cdecl; external libmgl;

//void mgl_clear_legend(HMGL graph);
procedure mgl_clear_legend(graph: HMGL); cdecl; external libmgl;

//void mgl_legend_xy(HMGL graph, mreal x, mreal y, const char *font, mreal size, mreal llen);
procedure mgl_legend_xy(graph: HMGL; x, y: mreal; const font: PChar; size: mreal; llen: mreal); cdecl; external libmgl;

//void mgl_legend(HMGL graph, int where, const char *font, mreal size, mreal llen);
procedure mgl_legend(graph: HMGL; where: integer; const font: PChar; size: mreal; llen: mreal); cdecl; external libmgl;

//void mgl_set_legend_box(HMGL gr, int enable);
procedure mgl_set_legend_box(graph: HMGL; enable: integer); cdecl; external libmgl;

//void mgl_set_legend_marks(HMGL gr, int num);
procedure mgl_set_legend_marks(graph: HMGL; num: integer); cdecl; external libmgl;

//*****************************************************************************/
//*             1D plotting functions                                         */
//*****************************************************************************/

//void mgl_fplot(HMGL graph, const char *fy, const char *stl, int n);
procedure mgl_fplot(graph: HMGL; const fy: PChar; const pen: PChar; n: integer); cdecl; external libmgl;

//void mgl_fplot_xyz(HMGL graph, const char *fx, const char *fy, const char *fz, const char *stl, int n);
procedure mgl_fplot_xyz(graph: HMGL; const fx: PChar; const fy: PChar; const fz: PChar; const pen: PChar; n: integer); cdecl; external libmgl;

//void mgl_plot_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *pen);
procedure mgl_plot_xyz(graph: HMGL; const x, y, z: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_plot_xy(HMGL graph, const HMDT x, const HMDT y, const char *pen);
procedure mgl_plot_xy(graph: HMGL; const x, y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_plot(HMGL graph, const HMDT y, const char *pen);
procedure mgl_plot(graph: HMGL; const y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_radar(HMGL graph, const HMDT a, const char *pen, mreal r);
procedure mgl_radar(graph: HMGL; const a: HMDT; const pen: PChar; r: mreal); cdecl; external libmgl;

//void mgl_boxplot_xy(HMGL graph, const HMDT x, const HMDT a, const char *pen);
procedure mgl_boxplot_xy(graph: HMGL; const x: HMDT; const a: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_boxplot(HMGL graph, const HMDT a, const char *pen);
procedure mgl_boxplot(graph: HMGL; const a: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_tens_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT c, const char *pen);
procedure mgl_tens_xyz(graph: HMGL; const x, y, z, c: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_tens_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT c,        const char *pen);
procedure mgl_tens_xy(graph: HMGL; const x, y, c: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_tens(HMGL graph, const HMDT y,       const HMDT c,   const char *pen);
procedure mgl_tens(graph: HMGL; const y, c: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_area_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *pen);
procedure mgl_area_xyz(graph: HMGL; const x, y, z: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_area_xy(HMGL graph, const HMDT x, const HMDT y, const char *pen);
procedure mgl_area_xy(graph: HMGL; const x, y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_area_xys(HMGL graph, const HMDT x, const HMDT y, const char *pen);
procedure mgl_area_xys(graph: HMGL; const x, y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_area_s(HMGL graph, const HMDT y, const char *pen);
procedure mgl_area_s(graph: HMGL; const y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_area(HMGL graph, const HMDT y, const char *pen);
procedure mgl_area(graph: HMGL; const y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_region_xy(HMGL graph, const HMDT x, const HMDT y1, const HMDT y2, const char *pen, int inside);
procedure mgl_region_xy(graph: HMGL; x, y1, y2: HMDT; const pen: PChar; inside: integer); cdecl; external libmgl;

//void mgl_region(HMGL graph, const HMDT y1, const HMDT y2, const char *pen, int inside);
procedure mgl_region(graph: HMGL; y1, y2: HMDT; const pen: PChar; inside: integer); cdecl; external libmgl;

//void mgl_mark(HMGL graph, mreal x,mreal y,mreal z,char mark);
procedure mgl_mark(graph: HMGL; x, y, z: mreal; mark: char); cdecl; external libmgl;

//void mgl_stem_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *pen);
procedure mgl_stem_xyz(graph: HMGL; const x, y, z: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_stem_xy(HMGL graph, const HMDT x, const HMDT y, const char *pen);
procedure mgl_stem_xy(graph: HMGL; const x, y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_stem(HMGL graph, const HMDT y,       const char *pen);
procedure mgl_stem(graph: HMGL; const y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_step_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *pen);
procedure mgl_step_xyz(graph: HMGL; const x, y, z: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_step_xy(HMGL graph, const HMDT x, const HMDT y, const char *pen);
procedure mgl_step_xy(graph: HMGL; const x, y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_step(HMGL graph, const HMDT y,       const char *pen);
procedure mgl_step(graph: HMGL; const y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_bars_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *pen);
procedure mgl_bars_xyz(graph: HMGL; const x, y, z: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_bars_xy(HMGL graph, const HMDT x, const HMDT y, const char *pen);
procedure mgl_bars_xy(graph: HMGL; const x, y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_bars(HMGL graph, const HMDT y,       const char *pen);
procedure mgl_bars(graph: HMGL; const y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_barh_yx(HMGL graph, const HMDT y, const HMDT v, const char *pen);
procedure mgl_barh_yx(graph: HMGL; const y, v: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_barh(HMGL graph, const HMDT v,       const char *pen);
procedure mgl_barh(graph: HMGL; const v: HMDT; const pen: PChar); cdecl; external libmgl;

//*****************************************************************************/
//*             Advanced 1D plotting functions                                */
//*****************************************************************************/

//void mgl_torus(HMGL graph, const HMDT r, const HMDT z, const char *pen);
procedure mgl_torus(graph: HMGL; const r, z: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_text_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z,const char *text, const char *font, mreal size);
procedure mgl_text_xyz(graph: HMGL; const x, y, z: HMDT; const text: PChar; const font: PChar; size: mreal); cdecl; external libmgl;

//void mgl_text_xy(HMGL graph, const HMDT x, const HMDT y, const char *text, const char *font, mreal size);
procedure mgl_text_xy(graph: HMGL; const x, y: HMDT; const text: PChar; const font: PChar; size: mreal); cdecl; external libmgl;

//void mgl_text_y(HMGL graph, const HMDT y, const char *text, const char *font, mreal size);
procedure mgl_text_y(graph: HMGL; const y: HMDT; const text: PChar; const font: PChar; size: mreal); cdecl; external libmgl;

//void mgl_chart(HMGL graph, const HMDT a, const char *col);
procedure mgl_chart(graph: HMGL; const a: HMDT; const col: PChar); cdecl; external libmgl;

//void mgl_error(HMGL graph, const HMDT y, const HMDT ey, const char *pen);
procedure mgl_error(graph: HMGL; const y, ey: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_error_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT ey, const char *pen);
procedure mgl_error_xy(graph: HMGL; const x, y, ey: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_error_exy(HMGL graph, const HMDT x, const HMDT y, const HMDT ex, const HMDT ey, const char *pen);
procedure mgl_error_exy(graph: HMGL; const x, y, ex, ey: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_mark_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT r, const char *pen);
procedure mgl_mark_xyz(graph: HMGL; const x, y, z, r: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_mark_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT r, const char *pen);
procedure mgl_mark_xy(graph: HMGL; const x, y, z: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_mark_y(HMGL graph, const HMDT y, const HMDT r, const char *pen);
procedure mgl_mark_y(graph: HMGL; const y, r: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_tube_xyzr(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT r, const char *pen);
procedure mgl_tube_xyzr(graph: HMGL; const x, y, z, r: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_tube_xyr(HMGL graph, const HMDT x, const HMDT y, const HMDT r, const char *pen);
procedure mgl_tube_xyr(graph: HMGL; const x, y, r: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_tube_r(HMGL graph, const HMDT y, const HMDT r, const char *pen);
procedure mgl_tube_r(graph: HMGL; const y, r: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_tube_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, mreal r, const char *pen);
procedure mgl_tube_xyz(graph: HMGL; const x, y, z: HMDT; r: mreal; const pen: PChar); cdecl; external libmgl;

//void mgl_tube_xy(HMGL graph, const HMDT x, const HMDT y, mreal r, const char *penl);
procedure mgl_tube_xy(graph: HMGL; const x, y:  HMDT; r: mreal; const penl: PChar); cdecl; external libmgl;

//void mgl_tube(HMGL graph, const HMDT y, mreal r, const char *pen);
procedure mgl_tube(graph: HMGL; const y: HMDT; r: mreal; const pen: PChar); cdecl; external libmgl;

//void mgl_textmark_xyzr(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT r, const char *text, const char *fnt);
procedure mgl_textmark_xyzr(graph: HMGL; const x, y, z, r: HMDT; const text: PChar; const fnt: PChar); cdecl; external libmgl;

//void mgl_textmark_xyr(HMGL graph, const HMDT x, const HMDT y, const HMDT r, const char *text, const char *fnt);
procedure mgl_textmark_xyr(graph: HMGL; const x, y, r: HMDT; const text: PChar; const fnt: PChar); cdecl; external libmgl;

//void mgl_textmark_yr(HMGL graph, const HMDT y, const HMDT r, const char *text, const char *fnt);
procedure mgl_textmark_yr(graph: HMGL; const y, r: HMDT; const text: PChar; const fnt: PChar); cdecl; external libmgl;

//void mgl_textmark(HMGL graph, const HMDT y, const char *text, const char *fnt);
procedure mgl_textmark(graph: HMGL; const y: HMDT; const text: PChar; const fnt: PChar); cdecl; external libmgl;

//void mgl_textmarkw_xyzr(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT r, const wchar_t *text, const char *fnt);
procedure mgl_textmarkw_xyzr(graph: HMGL; const x, y, z, r: HMDT; const text: PWideChar; const fnt: PChar); cdecl; external libmgl;

//void mgl_textmarkw_xyr(HMGL graph, const HMDT x, const HMDT y, const HMDT r, const wchar_t *text, const char *fnt);
procedure mgl_textmarkw_xyr(graph: HMGL; const x, y, r: HMDT; const text: PWideChar; const fnt: PChar); cdecl; external libmgl;

//void mgl_textmarkw_yr(HMGL graph, const HMDT y, const HMDT r, const wchar_t *text, const char *fnt);
procedure mgl_textmarkw_yr(graph: HMGL; const y, r: HMDT; const text: PWideChar; const fnt: PChar); cdecl; external libmgl;

//void mgl_textmarkw(HMGL graph, const HMDT y, const wchar_t *text, const char *fnt);
procedure mgl_textmarkw(graph: HMGL; const y: HMDT; const text: PWideChar; const fnt: PChar); cdecl; external libmgl;

//*****************************************************************************/
//*             2D plotting functions                                         */
//*****************************************************************************/

//void mgl_fsurf(HMGL graph, const char *fz, const char *stl, int n);
procedure mgl_fsurf(graph: HMGL; const fz: PChar; const stl: PChar; n: integer); cdecl; external libmgl;

//void mgl_fsurf_xyz(HMGL graph, const char *fx, const char *fy, const char *fz, const char *stl, int n);
procedure mgl_fsurf_xyz(graph: HMGL; const fx: PChar; const fy: PChar; const fz: PChar; const stl: PChar; n: integer); cdecl; external libmgl;

//void mgl_grid_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *stl,mreal zVal);
procedure mgl_grid_xy(graph: HMGL; const x, y, z: HMDT; const stl: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_grid(HMGL graph, const HMDT a,const char *stl,mreal zVal);
procedure mgl_grid(const graph: HMGL; a: HMDT; const stl: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_mesh_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *sch);
procedure mgl_mesh_xy(graph: HMGL; const x, y, z: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_mesh(HMGL graph, const HMDT z, const char *sch);
procedure mgl_mesh(graph: HMGL; const z: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_fall_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *sch);
procedure mgl_fall_xy(graph: HMGL; const x, y, z: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_fall(HMGL graph, const HMDT z, const char *sch);
procedure mgl_fall(graph: HMGL; const z: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_belt_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *sch);
procedure mgl_belt_xy(graph: HMGL; const x, y, z: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_belt(HMGL graph, const HMDT z, const char *sch);
procedure mgl_belt(graph: HMGL; const z: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_surf_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *sch);
procedure mgl_surf_xy(graph: HMGL; const x, y, z: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_surf(HMGL graph, const HMDT z, const char *sch);
procedure mgl_surf(graph: HMGL; const z: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_dens_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *sch,mreal zVal);
procedure mgl_dens_xy(graph: HMGL; const x, y, z: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_dens(HMGL graph, const HMDT z, const char *sch,mreal zVal);
procedure mgl_dens(graph: HMGL; const z: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_boxs_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *sch,mreal zVal);
procedure mgl_boxs_xy(graph: HMGL; const x, y, z: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_boxs(HMGL graph, const HMDT z, const char *sch,mreal zVal);
procedure mgl_boxs(graph: HMGL; const z: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_tile_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *sch);
procedure mgl_tile_xy(graph: HMGL; const x, y, z: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_tile(HMGL graph, const HMDT z, const char *sch);
procedure mgl_tile(graph: HMGL; const z: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_tiles_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT r, const char *sch);
procedure mgl_tiles_xy(graph: HMGL; const x, y, z, r: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_tiles(HMGL graph, const HMDT z, const HMDT r, const char *sch);
procedure mgl_tiles(graph: HMGL; const z, r: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_cont_xy_val(HMGL graph, const HMDT v, const HMDT x, const HMDT y, const HMDT z, const char *sch, mreal zVal);
procedure mgl_cont_xy_val(graph: HMGL; const v, x, y, z: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_cont_val(HMGL graph, const HMDT v, const HMDT z, const char *sch,mreal zVal);
procedure mgl_cont_val(graph: HMGL; const v, z: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_cont_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *sch, int Num, mreal zVal);
procedure mgl_cont_xy(graph: HMGL; const x, y, z: HMDT; const sch: PChar; Num: integer; zVal: mreal); cdecl; external libmgl;

//void mgl_cont(HMGL graph, const HMDT z, const char *sch, int Num, mreal zVal);
procedure mgl_cont(graph: HMGL; const z: HMDT; const sch: PChar; Num: integer; zVal: mreal); cdecl; external libmgl;

//void mgl_contf_xy_val(HMGL graph, const HMDT v, const HMDT x, const HMDT y, const HMDT z, const char *sch, mreal zVal);
procedure mgl_contf_xy_val(graph: HMGL; const v, x, y, z: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_contf_val(HMGL graph, const HMDT v, const HMDT z, const char *sch,mreal zVal);
procedure mgl_contf_val(graph: HMGL; const v, z: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_contf_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *sch, int Num, mreal zVal);
procedure mgl_contf_xy(graph: HMGL; const x, y, z: HMDT; const sch: PChar; Num: integer; zVal: mreal); cdecl; external libmgl;

//void mgl_contf(HMGL graph, const HMDT z, const char *sch, int Num, mreal zVal);
procedure mgl_contf(graph: HMGL; const z: HMDT; const sch: PChar; Num: integer; zVal: mreal); cdecl; external libmgl;

//void mgl_axial_xy_val(HMGL graph, const HMDT v, const HMDT x, const HMDT y, const HMDT a, const char *sch);
procedure mgl_axial_xy_val(graph: HMGL; const v, x, y, a: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_axial_val(HMGL graph, const HMDT v, const HMDT a, const char *sch);
procedure mgl_axial_val(graph: HMGL; const v, a: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_axial_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT a, const char *sch, int Num);
procedure mgl_axial_xy(graph: HMGL; const x, y, a: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_axial(HMGL graph, const HMDT a, const char *sch, int Num);
procedure mgl_axial(graph: HMGL; const a: HMDT; const sch: PChar; Num: integer); cdecl; external libmgl;

//*****************************************************************************/
//*             Dual plotting functions                                       */
//*****************************************************************************/

//void mgl_surfc_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT c, const char *sch);
procedure mgl_surfc_xy(graph: HMGL; const x, y, z, c: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_surfc(HMGL graph, const HMDT z, const HMDT c, const char *sch);
procedure mgl_surfc(graph: HMGL; const z, c: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_surfa_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT c, const char *sch);
procedure mgl_surfa_xy(graph: HMGL; const x, y, z, c: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_surfa(HMGL graph, const HMDT z, const HMDT c, const char *sch);
procedure mgl_surfa(graph: HMGL; const z, c: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_stfa_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT re, const HMDT im, int dn, const char *sch, mreal zVal);
procedure mgl_stfa_xy(graph: HMGL; const x, y, re, im: HMDT; dn: integer; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_stfa(HMGL graph, const HMDT re, const HMDT im, int dn, const char *sch, mreal zVal);
procedure mgl_stfa(graph: HMGL; const re, im: HMDT; dn: integer; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_vect_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT ax, const HMDT ay, const char *sch,mreal zVal);
procedure mgl_vect_xy(graph: HMGL; const x, y, z, ax, ay: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_vect_2d(HMGL graph, const HMDT ax, const HMDT ay, const char *sch,mreal zVal);
procedure mgl_vect_2d(graph: HMGL; const ax, ay: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_vectl_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT ax, const HMDT ay, const char *sch,mreal zVal);
procedure mgl_vectl_xy(graph: HMGL; const x, y, z, ax, ay: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_vectl_2d(HMGL graph, const HMDT ax, const HMDT ay, const char *sch,mreal zVal);
procedure mgl_vectl_2d(graph: HMGL; const ax, ay: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_vectc_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT ax, const HMDT ay, const char *sch,mreal zVal);
procedure mgl_vectc_xy(graph: HMGL; const x, y, ax, ay: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_vectc_2d(HMGL graph, const HMDT ax, const HMDT ay, const char *sch,mreal zVal);
procedure mgl_vectc_2d(graph: HMGL; const ax, ay: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_vect_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT ax, const HMDT ay, const HMDT az, const char *sch);
procedure mgl_vect_xyz(graph: HMGL; const x, y, z, ax, ay, az: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_vect_3d(HMGL graph, const HMDT ax, const HMDT ay, const HMDT az, const char *sch);
procedure mgl_vect_3d(graph: HMGL; const ax, ay, az: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_vectl_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT ax, const HMDT ay, const HMDT az, const char *sch);
procedure mgl_vectl_xyz(graph: HMGL; const x, y, z, ax, ay, az: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_vectl_3d(HMGL graph, const HMDT ax, const HMDT ay, const HMDT az, const char *sch);
procedure mgl_vectl_3d(graph: HMGL; const ax, ay, az: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_vectc_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT ax, const HMDT ay, const HMDT az, const char *sch);
procedure mgl_vectc_xyz(graph: HMGL; const x, y, z, ax, ay, az: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_vectc_3d(HMGL graph, const HMDT ax, const HMDT ay, const HMDT az, const char *sch);
procedure mgl_vectc_3d(graph: HMGL; const ax, ay, az: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_map_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT a, const HMDT b, const char *sch, int ks, int pnts);
procedure mgl_map_xy(graph: HMGL; const x, y, a, b: HMDT; const sch: PChar; ks: integer; pnts: integer); cdecl; external libmgl;

//void mgl_map(HMGL graph, const HMDT a, const HMDT b, const char *sch, int ks, int pnts);
procedure mgl_map(graph: HMGL; const a, b: HMDT; const sch: PChar; ks: integer; pnts: integer); cdecl; external libmgl;

//void mgl_surf3a_xyz_val(HMGL graph, mreal Val, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const HMDT b, const char *stl);
procedure mgl_surf3a_xyz_val(graph: HMGL; Val: mreal; const x, y, z, a, b: HMDT; const stla: PChar); cdecl; external libmgl;

//void mgl_surf3a_val(HMGL graph, mreal Val, const HMDT a, const HMDT b, const char *stl);
procedure mgl_surf3a_val(graph: HMGL; Val: mreal; const a, b: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_surf3a_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const HMDT b, const char *stl, int num);
procedure mgl_surf3a_xyz(graph: HMGL; const x, y, z, a, b: HMDT; const stl: PChar; num: integer); cdecl; external libmgl;

//void mgl_surf3a(HMGL graph, const HMDT a, const HMDT b, const char *stl, int num);
procedure mgl_surf3a(graph: HMGL; const a, b: HMDT; const stl: PChar; num: integer); cdecl; external libmgl;

//void mgl_surf3c_xyz_val(HMGL graph, mreal Val, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const HMDT b, const char *stl);
procedure mgl_surf3c_xyz_val(graph: HMGL; Val: mreal; const x, y, z, a, b: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_surf3c_val(HMGL graph, mreal Val, const HMDT a, const HMDT b, const char *stl);
procedure mgl_surf3c_val(graph: HMGL; Val: mreal; const a, b: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_surf3c_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const HMDT b,
//                      const char *stl, int num);
procedure mgl_surf3c_xyz(graph: HMGL; const x, y, z, a, b: HMDT; const stl: PChar; num: integer); cdecl; external libmgl;

//void mgl_surf3c(HMGL graph, const HMDT a, const HMDT b, const char *stl, int num);
procedure mgl_surf3c(graph: HMGL; const a, b: HMDT; const stl: PChar; num: integer); cdecl; external libmgl;

//void mgl_flow_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT ax, const HMDT ay, const char *sch, int num, int central, mreal zVal);
procedure mgl_flow_xy(graph: HMGL; const x, y, ax, ay: HMDT; const sch: PChar; num: integer; central: integer; zVal: mreal); cdecl; external libmgl;

//void mgl_flow_2d(HMGL graph, const HMDT ax, const HMDT ay, const char *sch, int num, int central, mreal zVal);
procedure mgl_flow_2d(graph: HMGL; const ax, ay: HMDT; const sch: PChar; num: integer; central: integer; zVal: mreal); cdecl; external libmgl;

//void mgl_flow_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT ax, const HMDT ay, const HMDT az, const char *sch, int num, int central);
procedure mgl_flow_xyz(graph: HMGL; const x, y, z, ax, ay, az: HMDT; const sch: PChar; num: integer; central: integer); cdecl; external libmgl;

//void mgl_flow_3d(HMGL graph, const HMDT ax, const HMDT ay, const HMDT az, const char *sch, int num, int central);
procedure mgl_flow_3d(graph: HMGL; const ax, ay, az: HMDT; const sch: PChar; num: integer; central: integer); cdecl; external libmgl;

//void mgl_pipe_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT ax, const HMDT ay, const char *sch, mreal r0, int num, int central, mreal zVal);
procedure mgl_pipe_xy(graph: HMGL; const x, y, ax, ay: HMDT; const sch: PChar; r0: mreal; num: integer; central: integer; zVal: mreal); cdecl; external libmgl;

//void mgl_pipe_2d(HMGL graph, const HMDT ax, const HMDT ay, const char *sch, mreal r0, int num, int central, mreal zVal);
procedure mgl_pipe_2d(graph: HMGL; const ax, ay: HMDT; const sch: PChar; r0: mreal; num: integer; central: integer; zVal: mreal); cdecl; external libmgl;

//void mgl_pipe_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT ax, const HMDT ay, const HMDT az, const char *sch, mreal r0, int num, int central);
procedure mgl_pipe_xyz(graph: HMGL; const x, y, z, ax, ay, az: HMDT; const sch: PChar; r0: mreal; num: integer; central: integer); cdecl; external libmgl;

//void mgl_pipe_3d(HMGL graph, const HMDT ax, const HMDT ay, const HMDT az, const char *sch, mreal r0, int num, int central);
procedure mgl_pipe_3d(graph: HMGL; const ax, ay, az: HMDT; const sch: PChar; r0: mreal; num: integer; central: integer); cdecl; external libmgl;

//void mgl_dew_xy(HMGL gr, const HMDT x, const HMDT y, const HMDT ax, const HMDT ay, const char *sch,mreal zVal);
procedure mgl_dew_xy(gr: HMGL; const x, y, z, ax, ay: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_dew_2d(HMGL gr, const HMDT ax, const HMDT ay, const char *sch,mreal zVal);
procedure mgl_dew_2d(gr: HMGL; const ax, ay: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_grad_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT ph, const char *sch, int num);
procedure mgl_grad_xyz(gr: HMGL; const x, y, z, ph: HMDT; const sch: PChar; num: integer); cdecl; external libmgl;

//void mgl_grad_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT ph, const char *sch, int num, mreal zVal);
procedure mgl_grad_xy(gr: HMGL; const x, y, ph: HMDT; const sch: PChar; num: integer; zVal: mreal); cdecl; external libmgl;

//void mgl_grad(HMGL graph, const HMDT ph, const char *sch, int num, mreal zVal);
procedure mgl_grad(gr: HMGL; const ph: HMDT; const sch: PChar; num: integer; zVal: mreal); cdecl; external libmgl;

//*****************************************************************************/
//*             3D plotting functions                                         */
//*****************************************************************************/

//void mgl_grid3_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, char dir, int sVal, const char *stl);
procedure mgl_grid3_xyz(graph: HMGL; const x, y, z, a: HMDT; dir: char; sVal: integer; const stl: PChar); cdecl; external libmgl;

//void mgl_grid3(HMGL graph, const HMDT a, char dir, int sVal, const char *stl);
procedure mgl_grid3(graph: HMGL; const a: HMDT; dir: char; sVal: integer; const stl: PChar); cdecl; external libmgl;

//void mgl_grid3_all_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const char *stl);
procedure mgl_grid3_all_xyz(graph: HMGL; const x, y, z, a: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_grid3_all(HMGL graph, const HMDT a, const char *stl);
procedure mgl_grid3_all(graph: HMGL; const a: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_dens3_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, char dir, int sVal, const char *stl);
procedure mgl_dens3_xyz(graph: HMGL; const x, y, z, a: HMDT; dir: char; sVal: integer; const stl: PChar); cdecl; external libmgl;

//void mgl_dens3(HMGL graph, const HMDT a, char dir, int sVal, const char *stl);
procedure mgl_dens3(graph: HMGL; const a: HMDT; dir: char; sVal: integer; const stl: PChar); cdecl; external libmgl;

//void mgl_dens3_all_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const char *stl);
procedure mgl_dens3_all_xyz(graph: HMGL; const x, y, z, a: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_dens3_all(HMGL graph, const HMDT a, const char *stl);
procedure mgl_dens3_all(graph: HMGL; const a: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_surf3_xyz_val(HMGL graph, mreal Val, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const char *stl);
procedure mgl_surf3_xyz_val(graph: HMGL; Val: mreal; const x, y, z, a: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_surf3_val(HMGL graph, mreal Val, const HMDT a, const char *stl);
procedure mgl_surf3_val(graph: HMGL; Val: mreal; const a: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_surf3_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const char *stl, int num);
procedure mgl_surf3_xyz(graph: HMGL; const x, y, z, a: HMDT; const stl: PChar; num: integer); cdecl; external libmgl;

//void mgl_surf3(HMGL graph, const HMDT a, const char *stl, int num);
procedure mgl_surf3(graph: HMGL; const a: HMDT; const stl: PChar; num: integer); cdecl; external libmgl;

//void mgl_cont3_xyz_val(HMGL graph, const HMDT v, const HMDT x, const HMDT y, const HMDT z, const HMDT a, char dir, int sVal, const char *sch);
procedure mgl_cont3_xyz_val(graph: HMGL; const v, x, y, z, a: HMDT; dir: char; sVal: integer; const sch: PChar); cdecl; external libmgl;

//void mgl_cont3_val(HMGL graph, const HMDT v, const HMDT a, char dir, int sVal, const char *sch);
procedure mgl_cont3_val(graph: HMGL; const v, a: HMDT; dir: char; sVal: integer; const sch: PChar); cdecl; external libmgl;

//void mgl_cont3_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, char dir, int sVal, const char *sch, int Num);
procedure mgl_cont3_xyz(graph: HMGL; const x, y, z, a: HMDT; dir: char; sVal: integer; const sch: PChar; Num: integer); cdecl; external libmgl;

//void mgl_cont3(HMGL graph, const HMDT a, char dir, int sVal, const char *sch, int Num);
procedure mgl_cont3(graph: HMGL; const a: HMDT; dir: char; sVal: integer; const stl: PChar; Num: integer); cdecl; external libmgl;

//void mgl_cont_all_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const char *sch, int Num);
procedure mgl_cont_all_xyz(graph: HMGL; const x, y, z, a: HMDT; const sch: PChar; Num: integer); cdecl; external libmgl;

//void mgl_cont_all(HMGL graph, const HMDT a, const char *sch, int Num);
procedure mgl_cont_all(graph: HMGL; const a: HMDT; const sch: PChar; Num: integer); cdecl; external libmgl;

//void mgl_cloudp_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const char *stl, mreal alpha);
procedure mgl_cloudp_xyz(graph: HMGL; x, y, z, a: HMDT; const stl: PChar; alpha: mreal); cdecl; external libmgl;

//void mgl_cloudp(HMGL graph, const HMDT a, const char *stl, mreal alpha);
procedure mgl_cloudp(graph: HMGL; const a: HMDT; const stl: PChar; alpha: mreal); cdecl; external libmgl;

//void mgl_cloud_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const char *stl, mreal alpha);
procedure mgl_cloud_xyz(graph: HMGL; const x, y, z, a: HMDT; const stl: PChar; alpha: mreal); cdecl; external libmgl;

//void mgl_cloud(HMGL graph, const HMDT a, const char *stl, mreal alpha);
procedure mgl_cloud(graph: HMGL; const a: HMDT; const stl: PChar; alpha: mreal); cdecl; external libmgl;

//void mgl_contf3_xyz_val(HMGL graph, const HMDT v, const HMDT x, const HMDT y, const HMDT z, const HMDT a, char dir, int sVal, const char *sch);
procedure mgl_contf3_xyz_val(graph: HMGL; const v, x, y, z, a: HMDT; dir: char; sVal: integer; const sch: PChar); cdecl; external libmgl;

//void mgl_contf3_val(HMGL graph, const HMDT v, const HMDT a, char dir, int sVal, const char *sch);
procedure mgl_contf3_val(graph: HMGL; const v, a: HMDT; dir: char; sVal: integer; const stl: PChar); cdecl; external libmgl;

//void mgl_contf3_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, char dir, int sVal, const char *sch, int Num);
procedure mgl_contf3_xyz(graph: HMGL; const x, y, z, a: HMDT; dir: char; sVal: integer; const sch: PChar; Num: integer); cdecl; external libmgl;

//void mgl_contf3(HMGL graph, const HMDT a, char dir, int sVal, const char *sch, int Num);
procedure mgl_contf3(graph: HMGL; const a: HMDT; dir: char; sVal: integer; const sch: PChar; Num: integer); cdecl; external libmgl;

//void mgl_contf_all_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const char *sch, int Num);
procedure mgl_contf_all_xyz(graph: HMGL; const x, y, z, a: HMDT; const sch: PChar; Num: integer); cdecl; external libmgl;

//void mgl_contf_all(HMGL graph, const HMDT a, const char *sch, int Num);
procedure mgl_contf_all(graph: HMGL; const a: HMDT; const sch: PChar; Num: integer); cdecl; external libmgl;

//void mgl_beam_val(HMGL graph, mreal Val, const HMDT tr, const HMDT g1, const HMDT g2, const HMDT a, mreal r, const char *stl, int norm);
procedure mgl_beam_val(graph: HMGL; Val: mreal; const tr, g1, g2, a: HMDT; r: mreal; const stl: PChar; norm: integer); cdecl; external libmgl;

//void mgl_beam(HMGL graph, const HMDT tr, const HMDT g1, const HMDT g2, const HMDT a, mreal r, const char *stl, int norm, int num);
procedure mgl_beam(graph: HMGL; const tr, g1, g2, a: HMDT; r: mreal; const stl: PChar; norm, num: integer); cdecl; external libmgl;

//*****************************************************************************/
//*             Triangular plotting functions                                 */
//*****************************************************************************/

//void mgl_triplot_xyzc(HMGL gr, const HMDT nums, const HMDT x, const HMDT y, const HMDT z, const HMDT c, const char *sch);
procedure mgl_triplot_xyzc(graph: HMGL; const nums, x, y, z, c: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_triplot_xyz(HMGL gr, const HMDT nums, const HMDT x, const HMDT y, const HMDT z, const char *sch);
procedure mgl_triplot_xyz(graph: HMGL; const nums, x, y, z: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_triplot_xy(HMGL gr, const HMDT nums, const HMDT x, const HMDT y, const char *sch, mreal zVal);
procedure mgl_triplot_xy(graph: HMGL; const nums, x, y: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_quadplot_xyzc(HMGL gr, const HMDT nums, const HMDT x, const HMDT y, const HMDT z, const HMDT c, const char *sch);
procedure mgl_quadplot_xyzc(graph: HMGL; const nums, x, y, z, c: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_quadplot_xyz(HMGL gr, const HMDT nums, const HMDT x, const HMDT y, const HMDT z, const char *sch);
procedure mgl_quadplot_xyz(graph: HMGL; const nums, x, y, z: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_quadplot_xy(HMGL gr, const HMDT nums, const HMDT x, const HMDT y, const char *sch, mreal zVal);
procedure mgl_quadplot_xy(graph: HMGL; const nums, x, y: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_tricont_xyzcv(HMGL gr, const HMDT v, const HMDT nums, const HMDT x, const HMDT y, const HMDT z, const HMDT c, const char *sch, mreal zVal);
procedure mgl_tricont_xyzcv(graph: HMGL; const v, nums, x, y, z, c: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_tricont_xyzv(HMGL gr, const HMDT v, const HMDT nums, const HMDT x, const HMDT y, const HMDT z, const char *sch, mreal zVal);
procedure mgl_tricont_xyzv(graph: HMGL; const v, nums, x, y, z: HMDT; const sch: PChar; zVal: mreal); cdecl; external libmgl;

//void mgl_tricont_xyzc(HMGL gr, const HMDT nums, const HMDT x, const HMDT y, const HMDT z, const HMDT c, const char *sch, int n, mreal zVal);
procedure mgl_tricont_xyzc(graph: HMGL; const nums, x, y, z, c: HMDT; const sch: PChar; n: integer; zVal: mreal); cdecl; external libmgl;

//void mgl_tricont_xyz(HMGL gr, const HMDT nums, const HMDT x, const HMDT y, const HMDT z, const char *sch, int n, mreal zVal);
procedure mgl_tricont_xyz(graph: HMGL; const nums, x, y, z: HMDT; const sch: PChar; n: integer; zVal: mreal); cdecl; external libmgl;

//void mgl_dots(HMGL gr, const HMDT x, const HMDT y, const HMDT z, const char *sch);
procedure mgl_dots(graph: HMGL; const x, y, z: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_dots_a(HMGL gr, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const char *sch);
procedure mgl_dots_a(graph: HMGL; const x, y, z, a: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_dots_tr(HMGL gr, const HMDT tr, const char *sch);
procedure mgl_dots_tr(graph: HMGL; const tr: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_crust(HMGL gr, const HMDT x, const HMDT y, const HMDT z, const char *sch, mreal er);
procedure mgl_crust(graph: HMGL; const x, y, z: HMDT; const sch: PChar; er: mreal); cdecl; external libmgl;

//void mgl_crust_tr(HMGL gr, const HMDT tr, const char *sch, mreal er);
procedure mgl_crust_tr(graph: HMGL; const tr: HMDT; const sch: PChar; er: mreal); cdecl; external libmgl;

//*****************************************************************************/
//*             Combined plotting functions                                   */
//*****************************************************************************/

//void mgl_dens_x(HMGL graph, const HMDT a, const char *stl, mreal sVal);
procedure mgl_dens_x(graph: HMGL; const a: HMDT; const stl: PChar; sVal: mreal); cdecl; external libmgl;

//void mgl_dens_y(HMGL graph, const HMDT a, const char *stl, mreal sVal);
procedure mgl_dens_y(graph: HMGL; const a: HMDT; const stl: PChar; sVal: mreal); cdecl; external libmgl;

//void mgl_dens_z(HMGL graph, const HMDT a, const char *stl, mreal sVal);
procedure mgl_dens_z(graph: HMGL; const a: HMDT; const stl: PChar; sVal: mreal); cdecl; external libmgl;

//void mgl_cont_x(HMGL graph, const HMDT a, const char *stl, mreal sVal, int Num);
procedure mgl_cont_x(graph: HMGL; const a: HMDT; const stl: PChar; sVal: mreal; Num: integer); cdecl; external libmgl;

//void mgl_cont_y(HMGL graph, const HMDT a, const char *stl, mreal sVal, int Num);
procedure mgl_cont_y(graph: HMGL; const a: HMDT; const stl: PChar; sVal: mreal; Num: integer); cdecl; external libmgl;

//void mgl_cont_z(HMGL graph, const HMDT a, const char *stl, mreal sVal, int Num);
procedure mgl_cont_z(graph: HMGL; const a: HMDT; const stl: PChar; sVal: mreal; Num: integer); cdecl; external libmgl;

//void mgl_cont_x_val(HMGL graph, const HMDT v, const HMDT a, const char *stl, mreal sVal);
procedure mgl_cont_x_val(graph: HMGL; const v, a: HMDT; const stl: PChar; sVal: mreal); cdecl; external libmgl;

//void mgl_cont_y_val(HMGL graph, const HMDT v, const HMDT a, const char *stl, mreal sVal);
procedure mgl_cont_y_val(graph: HMGL; const v, a: HMDT; const stl: PChar; sVal: mreal); cdecl; external libmgl;

//void mgl_cont_z_val(HMGL graph, const HMDT v, const HMDT a, const char *stl, mreal sVal);
procedure mgl_cont_z_val(graph: HMGL; const v, a: HMDT; const stl: PChar; sVal: mreal); cdecl; external libmgl;

//void mgl_contf_x(HMGL graph, const HMDT a, const char *stl, mreal sVal, int Num);
procedure mgl_contf_x(graph: HMGL; const a: HMDT; const stl: PChar; sVal: mreal; Num: integer); cdecl; external libmgl;

//void mgl_contf_y(HMGL graph, const HMDT a, const char *stl, mreal sVal, int Num);
procedure mgl_contf_y(graph: HMGL; const a: HMDT; const stl: PChar; sVal: mreal; Num: integer); cdecl; external libmgl;

//void mgl_contf_z(HMGL graph, const HMDT a, const char *stl, mreal sVal, int Num);
procedure mgl_contf_z(graph: HMGL; const a: HMDT; const stl: PChar; sVal: mreal; Num: integer); cdecl; external libmgl;

//void mgl_contf_x_val(HMGL graph, const HMDT v, const HMDT a, const char *stl, mreal sVal);
procedure mgl_contf_x_val(graph: HMGL; const v, a: HMDT; const stl: PChar; sVal: mreal); cdecl; external libmgl;

//void mgl_contf_y_val(HMGL graph, const HMDT v, const HMDT a, const char *stl, mreal sVal);
procedure mgl_contf_y_val(graph: HMGL; const v, a: HMDT; const stl: PChar; sVal: mreal); cdecl; external libmgl;

//void mgl_contf_z_val(HMGL graph, const HMDT v, const HMDT a, const char *stl, mreal sVal);
procedure mgl_contf_z_val(graph: HMGL; const v, a: HMDT; const stl: PChar; sVal: mreal); cdecl; external libmgl;

//*****************************************************************************/
//*             Data creation functions                                       */
//*****************************************************************************/

//void mgl_data_rearrange(HMDT dat, int mx, int my, int mz);
procedure mgl_data_rearrange(dat: HMDT; mx, my, mz: integer); cdecl; external libmgl;

//void mgl_data_set_mreal(HMDT dat, const mreal *A,int NX,int NY,int NZ);
procedure mgl_data_set_mreal(dat: HMDT; const A: Pmreal; NX, NY, NZ: integer); cdecl; external libmgl;

//void mgl_data_set_double(HMDT dat, const double *A,int NX,int NY,int NZ);
procedure mgl_data_set_double(dat: HMDT; const A: PDouble; NX, NY, NZ: integer); cdecl; external libmgl;

//void mgl_data_set_mreal2(HMDT d, const mreal **A,int N1,int N2);
type PPmreal = ^Pmreal;
procedure mgl_data_set_mreal2(d: HMDT; const A: PPmreal; N1, N2: integer); cdecl; external libmgl;

//void mgl_data_set_double2(HMDT d, const double **A,int N1,int N2);
type PPDouble = ^PDouble;
procedure mgl_data_set_double2(d: HMDT; const A: PPDouble; N1, N2: integer); cdecl; external libmgl;

//void mgl_data_set_mreal3(HMDT d, const mreal ***A,int N1,int N2,int N3);
type PPPmreal = ^PPmreal;
procedure mgl_data_set_mreal3(d: HMDT; const A: PPPmreal; N1, N2, N3: integer); cdecl; external libmgl;

//void mgl_data_set_double3(HMDT d, const double ***A,int N1,int N2,int N3);
type PPPDouble = ^PPDouble;
procedure mgl_data_set_double3(d: HMDT; const A: PPPDouble; N1, N2: integer); cdecl; external libmgl;

//void mgl_data_set(HMDT dat, const HMDT a);
procedure mgl_data_set(dat: HMDT; const a: HMDT); cdecl; external libmgl;


type TGSLVector = record
     end;

type TGSLMatrix = record
     end;

type PGSLVector = ^TGSLVector;
type PGSLMatrix = ^TGSLMatrix;

//void mgl_data_set_vector(HMDT dat, gsl_vector *v);
procedure mgl_data_set_vector(dat: HMDT; v: PGSLVector); cdecl; external libmgl;

//void mgl_data_set_matrix(HMDT dat, gsl_matrix *m);
procedure mgl_data_set_matrix(dat: HMDT; m: PGSLMatrix); cdecl; external libmgl;

//mreal mgl_data_get_value(const HMDT dat, int i, int j, int k);
function mgl_data_get_value(const dat: HMDT; i, j, k: integer): mreal; cdecl; external libmgl;

//int mgl_data_get_nx(const HMDT dat);
function mgl_data_get_nx(const dat: HMDT): integer; cdecl; external libmgl;

//int mgl_data_get_ny(const HMDT dat);
function mgl_data_get_ny(const dat: HMDT): integer; cdecl; external libmgl;

//int mgl_data_get_nz(const HMDT dat);
function mgl_data_get_nz(const dat: HMDT): integer; cdecl; external libmgl;

//void mgl_data_set_value(HMDT dat, mreal v, int i, int j, int k);
procedure mgl_data_set_value(dat: HMDT; v: mreal; i, j, k: integer); cdecl; external libmgl;

//void mgl_data_set_values(HMDT dat, const char *val, int nx, int ny, int nz);
procedure mgl_data_set_values(dat: HMDT; const val: PChar; nx, ny, nz: integer); cdecl; external libmgl;

//int mgl_data_read(HMDT dat, const char *fname);
function mgl_data_read(dat: HMDT; const fname: PChar): integer; cdecl; external libmgl;

//int mgl_data_read_mat(HMDT dat, const char *fname, int dim);
function mgl_data_read_mat(dat: HMDT; const fname: PChar; dim: integer): integer; cdecl; external libmgl;

//int mgl_data_read_dim(HMDT dat, const char *fname,int mx,int my,int mz);
function mgl_data_read_dim(dat: HMDT; const fname: PChar; mx, my, mz: integer): integer; cdecl; external libmgl;

//void mgl_data_save(HMDT dat, const char *fname,int ns);
procedure mgl_data_save(dat: HMDT; const fname: PChar; ns: integer); cdecl; external libmgl;

//void mgl_data_export(HMDT dat, const char *fname, const char *scheme,mreal v1,mreal v2,int ns);
procedure mgl_data_export(dat: HMDT; const fname: PChar; const scheme: PChar; v1, v2: mreal; ns: integer); cdecl; external libmgl;

//void mgl_data_import(HMDT dat, const char *fname, const char *scheme,mreal v1,mreal v2);
procedure mgl_data_import(dat: HMDT; const fname: PChar; const scheme: PChar; v1, v2: mreal); cdecl; external libmgl;

//void mgl_data_create(HMDT dat, int nx,int ny,int nz);
procedure mgl_data_create(dat: HMDT; nx, ny, nz: integer); cdecl; external libmgl;

//void mgl_data_transpose(HMDT dat, const char *dim);
procedure mgl_data_transpose(dat: HMDT; const dim: PChar); cdecl; external libmgl;

//void mgl_data_norm(HMDT dat, mreal v1,mreal v2,int sym,int dim);
procedure mgl_data_norm(dat: HMDT; v1, v2: mreal; sym: integer; dim: integer); cdecl; external libmgl;

//void mgl_data_norm_slice(HMDT dat, mreal v1,mreal v2,char dir,int keep_en,int sym);
procedure mgl_data_norm_slice(dat: HMDT; v1, v2: mreal; dir: char; keep_en, sym: integer); cdecl; external libmgl;

//HMDT mgl_data_subdata(const HMDT dat, int xx,int yy,int zz);
function mgl_data_subdata(const dat: HMDT; xx, yy, zz: integer): HMDT; cdecl; external libmgl;

//HMDT mgl_data_subdata_ext(const HMDT dat, const HMDT xx, const HMDT yy, const HMDT zz);
function mgl_data_subdata_ext(const dat, xx, yy, zz: HMDT): HMDT; cdecl; external libmgl;

//HMDT mgl_data_column(const HMDT dat, const char *eq);
function mgl_data_column(const dat: HMDT; const eq: PChar): HMDT; cdecl; external libmgl;

//void mgl_data_set_id(HMDT d, const char *id);
procedure mgl_data_set_id(d: HMDT; const id: PChar); cdecl; external libmgl;

//void mgl_data_fill(HMDT dat, mreal x1,mreal x2,char dir);
procedure mgl_data_fill(dat: HMDT; x1, x2: mreal; dir: char); cdecl; external libmgl;

//void mgl_data_fill_eq(HMGL gr, HMDT dat, const char *eq, const HMDT vdat, const HMDT wdat);
procedure mgl_data_fill_eq(graph: HMGL; dat: HMDT; const eq: PChar; vdat, wdat: HMDT); cdecl; external libmgl;

//void mgl_data_put_val(HMDT dat, mreal val, int i, int j, int k);
procedure mgl_data_put_val(dat: HMDT; val: mreal; i, j, k: integer); cdecl; external libmgl;

//void mgl_data_put_dat(HMDT dat, const HMDT val, int i, int j, int k);
procedure mgl_data_put_dat(dat: HMDT; const val: HMDT; i, j, k: integer); cdecl; external libmgl;

//void mgl_data_modify(HMDT dat, const char *eq,int dim);
procedure mgl_data_modify(dat: HMDT; const eq: PChar; dim: integer); cdecl; external libmgl;

//void mgl_data_modify_vw(HMDT dat, const char *eq,const HMDT vdat,const HMDT wdat);
procedure mgl_data_modify_vw(dat: HMDT; const eq: PChar; const vdat, wdat: HMDT); cdecl; external libmgl;

//void mgl_data_squeeze(HMDT dat, int rx,int ry,int rz,int smooth);
procedure mgl_data_squeeze(dat: HMDT; rx, ry, rz, smooth: integer); cdecl; external libmgl;

//mreal mgl_data_max(const HMDT dat);
function mgl_data_max(const dat: HMDT): mreal; cdecl; external libmgl;

//mreal mgl_data_min(const HMDT dat);
function mgl_data_min(const dat: HMDT): mreal; cdecl; external libmgl;

//mreal *mgl_data_value(HMDT dat, int i,int j,int k);
function mgl_data_value(dat: HMDT; i, j, k: integer): Pmreal; cdecl; external libmgl;

//const mreal *mgl_data_data(const HMDT dat);
function mgl_data_data(const dat: HMDT): Pmreal; cdecl; external libmgl;

//mreal mgl_data_first(const HMDT dat, const char *cond, int *i, int *j, int *k);
function mgl_data_first(const dat: HMDT; const cond: PChar; i, j, k: Pinteger): mreal; cdecl; external libmgl;

//mreal mgl_data_last(const HMDT dat, const char *cond, int *i, int *j, int *k);
function mgl_data_last(const dat: HMDT; const cond: PChar; i, j, k: Pinteger): mreal; cdecl; external libmgl;

//int mgl_data_find(const HMDT dat, const char *cond, char dir, int i, int j, int k);
function mgl_data_find(const dat: HMDT; const cond: PChar; dir: char; i, j, k: integer): integer; cdecl; external libmgl;

//int mgl_data_find_any(const HMDT dat, const char *cond);
function mgl_data_find_any(const dat: HMDT; const cond: PChar): integer; cdecl; external libmgl;

//mreal mgl_data_max_int(const HMDT dat, int *i, int *j, int *k);
function mgl_data_max_int(const dat: HMDT; i, j, k: Pinteger): mreal; cdecl; external libmgl;

//mreal mgl_data_max_real(const HMDT dat, mreal *x, mreal *y, mreal *z);
function mgl_data_max_real(const dat: HMDT; x, y, z: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_data_min_int(const HMDT dat, int *i, int *j, int *k);
function mgl_data_min_int(const dat: HMDT; i, j, k: Pinteger): mreal; cdecl; external libmgl;

//mreal mgl_data_min_real(const HMDT dat, mreal *x, mreal *y, mreal *z);
function mgl_data_min_real(const dat: HMDT; x, y, z: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_data_momentum_mw(const HMDT dat, char dir, mreal *m, mreal *w);
function mgl_data_momentum_mw(const dat: HMDT; dir: char; m, w: Pmreal): mreal; cdecl; external libmgl;

//HMDT mgl_data_combine(const HMDT dat1, const HMDT dat2);
function mgl_data_combine(const dat1, dat2: HMDT): HMDT; cdecl; external libmgl;

//void mgl_data_extend(HMDT dat, int n1, int n2);
procedure mgl_data_extend(dat: HMDT; n1, n2: integer); cdecl; external libmgl;

//void mgl_data_insert(HMDT dat, char dir, int at, int num);
procedure mgl_data_insert(dat: HMDT; dir: char; pos, num: integer); cdecl; external libmgl;

//void mgl_data_delete(HMDT dat, char dir, int at, int num);
procedure mgl_data_delete(dat: HMDT; dir: char; pos, num: integer); cdecl; external libmgl;

//*****************************************************************************/
//*             Data manipulation functions                                   */
//*****************************************************************************/

//void mgl_data_smooth(HMDT dat, int Type,mreal delta,const char *dirs);
procedure mgl_data_smooth(dat: HMDT; tType: integer; delpa: mreal; const dirs: PChar); cdecl; external libmgl;

//HMDT mgl_data_sum(const HMDT dat, const char *dir);
function mgl_data_sum(const dat: HMDT; const dir: PChar): HMDT; cdecl; external libmgl;

//HMDT mgl_data_max_dir(const HMDT dat, const char *dir);
function mgl_data_max_dir(const dat: HMDT; const dir: PChar): HMDT; cdecl; external libmgl;

//HMDT mgl_data_min_dir(const HMDT dat, const char *dir);
function mgl_data_min_dir(const dat: HMDT; const dir: PChar): HMDT; cdecl; external libmgl;

//void mgl_data_cumsum(HMDT dat, const char *dir);
procedure mgl_data_cumsum(dat: HMDT; const dir: PChar); cdecl; external libmgl;

//void mgl_data_integral(HMDT dat, const char *dir);
procedure mgl_data_integral(dat: HMDT; const dir: PChar); cdecl; external libmgl;

//void mgl_data_diff(HMDT dat, const char *dir);
procedure mgl_data_diff(dat: HMDT; const dir: PChar); cdecl; external libmgl;

//void mgl_data_diff_par(HMDT dat, const HMDT v1, const HMDT v2, const HMDT v3);
procedure mgl_data_diff_par(dat, v1, v2, v3: HMDT); cdecl; external libmgl;

//void mgl_data_diff2(HMDT dat, const char *dir);
procedure mgl_data_diff2(dat: HMDT; const dir: PChar); cdecl; external libmgl;

//void mgl_data_swap(HMDT dat, const char *dir);
procedure mgl_data_swap(dat: HMDT; const dir: PChar); cdecl; external libmgl;

//void mgl_data_roll(HMDT dat, char dir, int num);
procedure mgl_data_roll(dat: HMDT; dir: char; num: integer); cdecl; external libmgl;

//void mgl_data_mirror(HMDT dat, const char *dir);
procedure mgl_data_mirror(dat: HMDT; const dir: PChar); cdecl; external libmgl;

//void mgl_data_hankel(HMDT dat, const char *dir);
procedure mgl_data_hankel(dat: HMDT; const dir: PChar); cdecl; external libmgl;

//void mgl_data_sinfft(HMDT dat, const char *dir);
procedure mgl_data_sinfft(dat: HMDT; const dir: PChar); cdecl; external libmgl;

//void mgl_data_cosfft(HMDT dat, const char *dir);
procedure mgl_data_cosfft(dat: HMDT; const dir: PChar); cdecl; external libmgl;

//void mgl_data_fill_sample(HMDT dat, int num, const char *how);
procedure mgl_data_fill_sample(dat: HMDT; num: integer; const how: PChar); cdecl; external libmgl;

//mreal mgl_data_spline(const HMDT dat, mreal x,mreal y,mreal z);
function mgl_data_spline(const dat: HMDT; x, y, z: mreal): mreal; cdecl; external libmgl;

//mreal mgl_data_spline1(const HMDT dat, mreal x,mreal y,mreal z);
function mgl_data_spline1(const dat: HMDT; x, y, z: mreal): mreal; cdecl; external libmgl;

//mreal mgl_data_linear(const HMDT dat, mreal x,mreal y,mreal z);
function mgl_data_linear(const dat: HMDT; x, y, z: mreal): mreal; cdecl; external libmgl;

//mreal mgl_data_linear1(const HMDT dat, mreal x,mreal y,mreal z);
function mgl_data_linear1(const dat: HMDT; x, y, z: mreal): mreal; cdecl; external libmgl;

//HMDT mgl_data_resize(const HMDT dat, int mx,int my,int mz);
function mgl_data_resize(const dat: HMDT; mx, my, mz: integer): HMDT; cdecl; external libmgl;

//HMDT mgl_data_resize_box(const HMDT dat, int mx,int my,int mz,mreal x1,mreal x2,
//              mreal y1,mreal y2,mreal z1,mreal z2);
function mgl_data_resize_box(const dat: HMDT; mx, my, mz: integer; x1, x2, y1, y2, z1, z2: mreal): HMDT; cdecl; external libmgl;

//HMDT mgl_data_hist(const HMDT dat, int n, mreal v1, mreal v2, int nsub);
function mgl_data_hist(const dat: HMDT; n: integer; v1, v2: mreal; nsub: integer): HMDT; cdecl; external libmgl;

//HMDT mgl_data_hist_w(const HMDT dat, const HMDT weight, int n, mreal v1, mreal v2, int nsub);
function mgl_data_hist_w(const dat, weight: HMDT; n: integer; v1, v2: mreal; nsub: integer): HMDT; cdecl; external libmgl;

//HMDT mgl_data_momentum(const HMDT dat, char dir, const char *how);
function mgl_data_momentum(dat: HMDT; dir: char; const how: PChar): HMDT; cdecl; external libmgl;

//HMDT mgl_data_evaluate_i(const HMDT dat, const HMDT idat, int norm);
function mgl_data_evaluate_i(dat, idat: HMDT; norm: char): HMDT; cdecl; external libmgl;

//HMDT mgl_data_evaluate_ij(const HMDT dat, const HMDT idat, const HMDT jdat, int norm);
function mgl_data_evaluate_ij(dat, idat, jdat: HMDT; norm: char): HMDT; cdecl; external libmgl;

//HMDT mgl_data_evaluate_ijk(const HMDT dat, const HMDT idat, const HMDT jdat, const HMDT kdat, int norm);
function mgl_data_evaluate_ijk(dat, idat, jdat, kdat: HMDT; norm: char): HMDT; cdecl; external libmgl;

//void mgl_data_envelop(HMDT dat, char dir);
procedure mgl_data_envelop(dat: HMDT; dir: char); cdecl; external libmgl;

//void mgl_data_sew(HMDT dat, const char *dirs, mreal da);
procedure mgl_data_sew(dat: HMDT; dirs: PChar; da: mreal); cdecl; external libmgl;

//void mgl_data_crop(HMDT dat, int n1, int n2, char dir);
procedure mgl_data_crop(dat: HMDT; n1, n2: integer; dir: char); cdecl; external libmgl;

//*****************************************************************************/
//*             Data operations                                               */
//*****************************************************************************/

//void mgl_data_mul_dat(HMDT dat, const HMDT d);
procedure mgl_data_mul_dat(dat: HMDT; const d: HMDT); cdecl; external libmgl;

//void mgl_data_div_dat(HMDT dat, const HMDT d);
procedure mgl_data_div_dat(dat: HMDT; const d: HMDT); cdecl; external libmgl;

//void mgl_data_add_dat(HMDT dat, const HMDT d);
procedure mgl_data_add_dat(dat: HMDT; const d: HMDT); cdecl; external libmgl;

//void mgl_data_sub_dat(HMDT dat, const HMDT d);
procedure mgl_data_sub_dat(dat: HMDT; const d: HMDT); cdecl; external libmgl;

//void mgl_data_mul_num(HMDT dat, mreal d);
procedure mgl_data_mul_num(dat: HMDT; d: mreal); cdecl; external libmgl;

//void mgl_data_div_num(HMDT dat, mreal d);
procedure mgl_data_div_num(dat: HMDT; d: mreal); cdecl; external libmgl;

//void mgl_data_add_num(HMDT dat, mreal d);
procedure mgl_data_add_num(dat: HMDT; d: mreal); cdecl; external libmgl;

//void mgl_data_sub_num(HMDT dat, mreal d);
procedure mgl_data_sub_num(dat: HMDT; d: mreal); cdecl; external libmgl;

//*****************************************************************************/
//*             Nonlinear fitting                                             */
//*****************************************************************************/

//void mgl_hist_x(HMGL gr, HMDT res, const HMDT x, const HMDT a);
procedure mgl_hist_x(gr: HMGL; res: HMDT; const x, a: HMDT); cdecl; external libmgl;

//void mgl_hist_xy(HMGL gr, HMDT res, const HMDT x, const HMDT y, const HMDT a);
procedure mgl_hist_xy(gr: HMGL; res: HMDT; const x, y, a: HMDT); cdecl; external libmgl;

//void mgl_hist_xyz(HMGL gr, HMDT res, const HMDT x, const HMDT y, const HMDT z, const HMDT a);
procedure mgl_hist_xyz(gr: HMGL; res: HMDT; const x, y, z, a: HMDT); cdecl; external libmgl;

//mreal mgl_fit_1(HMGL gr, HMDT fit, const HMDT y, const char *eq, const char *var, mreal *ini);
function mgl_fit_1(gr: HMGL; fit: HMDT; const y: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_2(HMGL gr, HMDT fit, const HMDT z, const char *eq, const char *var, mreal *ini);
function mgl_fit_2(gr: HMGL; fit: HMDT; const z: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_3(HMGL gr, HMDT fit, const HMDT a, const char *eq, const char *var, mreal *ini);
function mgl_fit_3(gr: HMGL; fit: HMDT; const a: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_xy(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const char *eq, const char *var, mreal *ini);
function mgl_fit_xy(gr: HMGL; fit: HMDT; const x, y: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_xyz(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT z, const char *eq, const char *var, mreal *ini);
function mgl_fit_xyz(gr: HMGL; fit: HMDT; const x, y, z: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_xyza(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const char *eq, const char *var, mreal *ini);
function mgl_fit_xyza(gr: HMGL; fit: HMDT; const x, y, z, a: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_ys(HMGL gr, HMDT fit, const HMDT y, const HMDT s, const char *eq, const char *var, mreal *ini);
function mgl_fit_ys(gr: HMGL; fit: HMDT; const y, s: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_xys(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT s, const char *eq, const char *var, mreal *ini);
function mgl_fit_xys(gr: HMGL; fit: HMDT; const x, y, s: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_xyzs(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT z, const HMDT s, const char *eq, const char *var, mreal *ini);
function mgl_fit_xyzs(gr: HMGL; fit: HMDT; const x, y, z, s: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_xyzas(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const HMDT s, const char *eq, const char *var, mreal *ini);
function mgl_fit_xyzas(gr: HMGL; fit: HMDT; const x, y, z, a, s: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_1_d(HMGL gr, HMDT fit, const HMDT y, const char *eq, const char *var, HMDT ini);
function mgl_fit_1_d(gr: HMGL; fit: HMDT; const y: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_2_d(HMGL gr, HMDT fit, const HMDT z, const char *eq, const char *var, HMDT ini);
function mgl_fit_2_d(gr: HMGL; fit: HMDT; const z: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_3_d(HMGL gr, HMDT fit, const HMDT a, const char *eq, const char *var, HMDT ini);
function mgl_fit_3_d(gr: HMGL; fit: HMDT; const a: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_xy_d(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const char *eq, const char *var, HMDT ini);
function mgl_fit_xy_d(gr: HMGL; fit: HMDT; const x, y: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_xyz_d(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT z, const char *eq, const char *var, HMDT ini);
function mgl_fit_xyz_d(gr: HMGL; fit: HMDT; const x, y, z: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_xyza_d(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const char *eq, const char *var, HMDT ini);
function mgl_fit_xyza_d(gr: HMGL; fit: HMDT; const x, y, z, a: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_ys_d(HMGL gr, HMDT fit, const HMDT y, const HMDT s, const char *eq, const char *var, HMDT ini);
function mgl_fit_ys_d(gr: HMGL; fit: HMDT; const y, s: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_xys_d(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT s, const char *eq, const char *var, HMDT ini);
function mgl_fit_xys_d(gr: HMGL; fit: HMDT; const x, y, s: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_xyzs_d(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT z, const HMDT s, const char *eq, const char *var, HMDT ini);
function mgl_fit_xyzs_d(gr: HMGL; fit: HMDT; const x, y, z, s: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//mreal mgl_fit_xyzas_d(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const HMDT s, const char *eq, const char *var, HMDT ini);
function mgl_fit_xyzas_d(gr: HMGL; fit: HMDT; const x, y, z, a, s: HMDT; const eq: PChar; const vvar: PChar; ini: Pmreal): mreal; cdecl; external libmgl;

//void mgl_puts_fit(HMGL gr, mreal x, mreal y, mreal z, const char *prefix, const char *font, mreal size);
procedure mgl_puts_fit(gr: HMGL; x, y, z: mreal; const prefix: PChar; const font: PChar; size: mreal); cdecl; external libmgl;

//const char *mgl_get_fit(HMGL gr);
function mgl_get_fit(gr: HMGL): PChar; cdecl; external libmgl;

//*****************************************************************************/

//void mgl_sphere(HMGL graph, mreal x, mreal y, mreal z, mreal r, const char *stl);
procedure mgl_sphere(gr: HMGL; x, y, z, r: mreal; const stl: PChar); cdecl; external libmgl;

//void mgl_drop(HMGL graph, mreal x1, mreal y1, mreal z1, mreal x2, mreal y2, mreal z2, mreal r, const char *stl, mreal shift, mreal ap);
procedure mgl_drop(gr: HMGL; x1, y1, z1, x2, y2, z2, r: mreal; const stl: PChar; shift, ap: mreal); cdecl; external libmgl;

//void mgl_cone(HMGL graph, mreal x1, mreal y1, mreal z1, mreal x2, mreal y2, mreal z2, mreal r1, mreal r2, const char *stl, int edge);
procedure mgl_cone(gr: HMGL; x1, y1, z1, x2, y2, z2, r1, r2: mreal; const stl: PChar; edge: integer); cdecl; external libmgl;

//HMDT mgl_pde_solve(HMGL gr, const char *ham, const HMDT ini_re, const HMDT ini_im, mreal dz, mreal k0);
function mgl_pde_solve(gr: HMGL; const ham: PChar; ini_re, ini_im: HMDT; dz, k0: mreal): HMDT; cdecl; external libmgl;

//HMDT mgl_qo2d_solve(const char *ham, const HMDT ini_re, const HMDT ini_im, const HMDT ray, mreal r, mreal k0, HMDT xx, HMDT yy);
function mgl_qo2d_solve(const ham: PChar; ini_re, ini_im, ray: HMDT; r, k0: mreal; xx, yy: HMDT): HMDT; cdecl; external libmgl;

//HMDT mgl_af2d_solve(const char *ham, const HMDT ini_re, const HMDT ini_im, const HMDT ray, mreal r, mreal k0, HMDT xx, HMDT yy);
function mgl_af2d_solve(const ham: PChar; ini_re, ini_im, ray: HMDT; r, k0: mreal; xx, yy: HMDT): HMDT; cdecl; external libmgl;

//HMDT mgl_ray_trace(const char *ham, mreal x0, mreal y0, mreal z0, mreal px, mreal py, mreal pz, mreal dt, mreal tmax);
function mgl_ray_trace(const ham: PChar; x0, y0, z0, px, py, pz, dt, tmax: mreal): HMDT; cdecl; external libmgl;

//HMDT mgl_jacobian_2d(const HMDT x, const HMDT y);
function mgl_jacobian_2d(x, y: HMDT): HMDT; cdecl; external libmgl;

//HMDT mgl_jacobian_3d(const HMDT x, const HMDT y, const HMDT z);
function mgl_jacobian_3d(x, y, z: HMDT): HMDT; cdecl; external libmgl;

//HMDT mgl_transform_a(const HMDT am, const HMDT ph, const char *tr);
function mgl_transform_a(am, ph: HMDT; tr: PChar): HMDT; cdecl; external libmgl;

//HMDT mgl_transform(const HMDT re, const HMDT im, const char *tr);
function mgl_transform(re, im: HMDT; tr: PChar): HMDT; cdecl; external libmgl;

//HMDT mgl_data_stfa(const HMDT re, const HMDT im, int dn, char dir);
function mgl_data_stfa(re, im: HMDT; dn: integer; dir: char): HMDT; cdecl; external libmgl;

//*****************************************************************************/


{$IFDEF MSWINDOWS}

//*****************************************************************************/
// Delphi - specific
//*****************************************************************************/

procedure mgl_begin();
procedure mgl_end();

procedure mgl_draw_on_canvas(gr: HMGL; width, height: integer; canvas: TCanvas; switchXY: boolean = false);

implementation

var _FPUCW: word;

procedure mgl_begin();
 begin
  _FPUCW := Get8087CW();     // backup current FPU CW
  Set8087CW(_FPUCW or $3F); // masking all FPU exceptions
 end;

procedure mgl_end();
 begin
  Set8087CW(_FPUCW);         // restore old FPU CW
 end;

procedure mgl_draw_on_canvas(gr: HMGL; width, height: integer; canvas: TCanvas; switchXY: boolean = false);
  var i, j: integer;
      bytes: PByte;
      col: TColor;
 begin
  bytes := mgl_get_rgb(gr);

  if (not switchXY) then
   for j := 0 to height - 1 do
    for i := 0 to width - 1 do
     begin
      col := 0;
      col := col or (bytes^);
      inc(bytes);
      col := col or (bytes^) shl 8;
      inc(bytes);
       col := col or (bytes^) shl 16;
      inc(bytes);
      canvas.Pixels[i, j] := col;
    end
  else
   for j := height - 1 downto 0 do
    for i := 0 to width - 1 do
     begin
      col := 0;
      col := col or (bytes^);
      inc(bytes);
      col := col or (bytes^) shl 8;
      inc(bytes);
       col := col or (bytes^) shl 16;
      inc(bytes);
      canvas.Pixels[j, i] := col;
     end;
 end;

{$ELSE}

implementation

{$ENDIF}

procedure mgl_set_ranges(graph: HMGL; x1, x2, y1, y2, z1, z2: mreal);
begin
  mgl_set_axis_3d(graph, x1, y1, z1, x2, y2, z2);
end;

end.

