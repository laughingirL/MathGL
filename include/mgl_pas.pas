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

interface

uses Windows, Math, Graphics;

const MGL_VERSION	= 6.2;  // mgl 1.7

const
{$IFDEF MSWINDOWS}
  //win - .dll
  libmgl = 'mgl.dll';
  libmglglut = 'libmgl-glut.dll';
  libmglfltk = 'libmgl-fltk.dll';
{$ELSE}
{$IFDEF LINUX}
  //linux - .so
  libmgl = 'mgl.so';
  libmglglut = 'libmgl-glut.so';
  libmglfltk = 'libmgl-fltk.so';
{$ELSE}
  // other platforms?

{$ENDIF}
{$ENDIF}



type TMGLGraph = record
     end;

type TMGLData = record
     end;
type TMGLParse = record
     end;

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

//HMGL mgl_create_graph_glut(int argc, char **argv, int (*draw)(mglGraph *gr, void *p),	const char *title, void (*reload)(bool next), void *par);
type TGLUTDrawFunction = function (gr: HMGL; p: pointer): integer; cdecl;
type TGLUTReloadFunction = procedure (next: boolean); cdecl;
type PPChar = ^PChar;
function mgl_create_graph_glut(argc: integer; argv: PPChar; draw: TGLUTDrawFunction; const title: PChar; reload: TGLUTReloadFunction; par: pointer): HMGL; cdecl; external libmglglut;

//HMGL mgl_create_graph_fltk(int argc, char **argv, int (*draw)(mglGraph *gr, void *p), const char *title, void (*reload)(bool next), void *par);
function mgl_create_graph_fltk(argc: integer; argv: PPChar; draw: TGLUTDrawFunction; const title: PChar; reload: TGLUTReloadFunction; par: pointer): HMGL; cdecl; external libmglfltk;

//HMGL mgl_create_graph_idtf();
function mgl_create_graph_idtf(): HMGL; cdecl; external libmgl;

//void mgl_fltk_run();
procedure mgl_fltk_run(); cdecl; external libmglfltk;

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

//void mgl_add_param(HMPR p, int id, const char *str);
procedure mgl_add_param(p: HMPR; id: integer; const str: PChar); cdecl; external libmgl;

//void mgl_add_paramw(HMPR p, int id, const wchar_t *str);
procedure mgl_add_paramw(p: HMPR; id: integer; const str: PWideChar); cdecl; external libmgl;

///*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
//HMDT mgl_add_var(HMPR, const char *name);
function mgl_add_var(p: HMPR; const name: PChar): HMDT; cdecl; external libmgl;

///*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
//HMDT mgl_find_var(HMPR, const char *name);
function mgl_find_var(p: HMPR; const name: PChar): HMDT; cdecl; external libmgl;

//int mgl_parse(HMGL gr, HMPR p, const char *str, int pos);
function mgl_parse(gr: HMGL; p: HMPR; const str: PChar; pos: integer): integer; cdecl; external libmgl;

//int mgl_parsew(HMGL gr, HMPR p, const wchar_t *str, int pos);
function mgl_parsew(gr: HMGL; p: HMPR; const str: PWideChar; pos: integer): integer; cdecl; external libmgl;

//void mgl_restore_once(HMPR p);
procedure mgl_restore_once(p: HMPR); cdecl; external libmgl;

//void mgl_parser_allow_setsize(HMPR p, bool a);
procedure mgl_parser_allow_setsize(p: HMPR; a: boolean); cdecl; external libmgl;


//*****************************************************************************/
//*		Setup mglGraph														 */
//*****************************************************************************/
//void mgl_set_pal_color(HMGL graph, int n, float r, float g, float b);
procedure mgl_set_pal_color(graph: HMGL; n: integer; r, g, b: single); cdecl; external libmgl;

//void mgl_set_pal_num(HMGL graph, int num);
procedure mgl_set_pal_num(graph: HMGL; num: integer); cdecl; external libmgl;

//void mgl_set_rotated_text(HMGL graph, bool rotated);
procedure mgl_set_rotated_text(graph: HMGL; rotated: boolean); cdecl; external libmgl;

//void mgl_set_cut(HMGL graph, bool cut);
procedure mgl_set_cut(graph: HMGL; cut: boolean); cdecl; external libmgl;

//void mgl_set_cut_box(HMGL gr, float x1,float y1,float z1,float x2,float y2,float z2);
procedure mgl_set_cut_box(gr: HMGL; x1, y1, z1, x2, y2, z2: single); cdecl; external libmgl;

//void mgl_set_bar_width(HMGL graph, float width);
procedure mgl_set_bar_width(graph: HMGL; width: single); cdecl; external libmgl;

//void mgl_set_base_line_width(HMGL gr, float size);
procedure mgl_set_base_line_width(gr: HMGL; size: single); cdecl; external libmgl;

//void mgl_set_mark_size(HMGL graph, float size);
procedure mgl_set_mark_size(graph: HMGL; size: single); cdecl; external libmgl;

//void mgl_set_arrow_size(HMGL graph, float size);
procedure mgl_set_arrow_size(graph: HMGL; size: single); cdecl; external libmgl;

//void mgl_set_font_size(HMGL graph, float size);
procedure mgl_set_font_size(graph: HMGL; size: single); cdecl; external libmgl;

//void mgl_set_alpha_default(HMGL graph, float alpha);
procedure mgl_set_alpha_default(graph: HMGL; alpha: single); cdecl; external libmgl;

//void mgl_set_size(HMGL graph, int width, int height);
procedure mgl_set_size(graph: HMGL; width, height: integer); cdecl; external libmgl;

//void mgl_set_axial_dir(HMGL graph, char dir);
procedure mgl_set_axial_dir(graph: HMGL; dir: char); cdecl; external libmgl;

//void mgl_set_meshnum(HMGL graph, int num);
procedure mgl_set_meshnum(graph: HMGL; num: integer); cdecl; external libmgl;

//void mgl_set_zoom(HMGL gr, float x1, float y1, float x2, float y2);
procedure mgl_set_zoom(gr: HMGL; x1, y1, x2, y2: single); cdecl; external libmgl;

//void mgl_set_plotfactor(HMGL gr, float val);
procedure mgl_set_plotfactor(gr: HMGL; val: single); cdecl; external libmgl;

//void mgl_set_draw_face(HMGL gr, bool enable);
procedure mgl_set_draw_face(gr: HMGL; enable: boolean); cdecl; external libmgl;

//void mgl_set_scheme(HMGL gr, char *sch);
procedure mgl_set_scheme(gr: HMGL; sch: PChar); cdecl; external libmgl;

//void mgl_set_font(HMGL gr, char *name, char *path);
procedure mgl_set_font(gr: HMGL; name: PChar; path: char); cdecl; external libmgl;

//void mgl_copy_font(HMGL gr, HMGL gr_from);
procedure mgl_copy_font(gr, gr_from: HMGL); cdecl; external libmgl;

//void mgl_restore_font(HMGL gr);
procedure mgl_restore_font(gr: HMGL); cdecl; external libmgl;

//*****************************************************************************/
//*		Export to file or to memory														 */
//*****************************************************************************/
//void mgl_write_tif(HMGL graph, const char *fname,const char *descr);
procedure mgl_write_tif(graph: HMGL; const fname: PChar; const descr: PChar); cdecl; external libmgl;

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

//const unsigned char *mgl_get_rgb(HMGL graph);
function mgl_get_rgb(graph: HMGL): PByte; cdecl; external libmgl;

//const unsigned char *mgl_get_rgba(HMGL graph);
function mgl_get_rgba(graph: HMGL): PByte; cdecl; external libmgl;

//int mgl_get_width(HMGL graph);
function mgl_get_width(graph: HMGL): integer; cdecl; external libmgl;

//int mgl_get_height(HMGL graph);
function mgl_get_height(graph: HMGL): integer; cdecl; external libmgl;

///*****************************************************************************/
///*		Setup frames transparency (alpha) and lightning						 */
///*****************************************************************************/
//int mgl_new_frame(HMGL graph, int id);
function mgl_new_frame(graph: HMGL; id: integer): integer; cdecl; external libmgl;

//void mgl_end_frame(HMGL graph);
procedure mgl_end_frame(graph: HMGL); cdecl; external libmgl;

//int mgl_get_num_frame(HMGL graph);
function mgl_get_num_frame(graph: HMGL): integer; cdecl; external libmgl;

//void mgl_set_transp_type(HMGL graph, int type);
procedure mgl_set_transp_type(graph: HMGL; ttype: integer); cdecl; external libmgl;

//void mgl_set_transp(HMGL graph, bool enable);
procedure mgl_set_transp(graph: HMGL; enable: boolean); cdecl; external libmgl;

//void mgl_set_alpha(HMGL graph, bool enable);
procedure mgl_set_alpha(graph: HMGL; enable: boolean); cdecl; external libmgl;

//void mgl_set_fog(HMGL graph, float d, float dz);
procedure mgl_set_fog(graph: HMGL; d, dz: single); cdecl; external libmgl;

//void mgl_set_light(HMGL graph, bool enable);
procedure mgl_set_light(graph: HMGL; enable: boolean); cdecl; external libmgl;

//void mgl_add_light(HMGL graph, int n, float x, float y, float z, bool infty);
procedure mgl_add_light(graph: HMGL; n: integer; x, y, z: single; infty: boolean); cdecl; external libmgl;

//void mgl_add_light_rgb(HMGL graph, int n, float x, float y, float z, bool infty,
//                        float r, float g, float b, float i);
procedure mgl_add_light_rgb(graph: HMGL; n: integer; x, y, z: single; infty: boolean; r, g, b, i: single); cdecl; external libmgl;

//void mgl_set_ambbr(HMGL gr, float i);
procedure mgl_set_ambbr(graph: HMGL; i: single); cdecl; external libmgl;

//*****************************************************************************/
//*		Scale and rotate													 */
//*****************************************************************************/
//void mgl_identity(HMGL graph);
procedure mgl_identity(graph: HMGL); cdecl; external libmgl;

//void mgl_clf(HMGL graph);
procedure mgl_clf(graph: HMGL); cdecl; external libmgl;

//void mgl_clf_rgb(HMGL graph, float r, float g, float b);
procedure mgl_clf_rgb(graph: HMGL; r, g, b: single); cdecl; external libmgl;

//void mgl_subplot(HMGL graph, int nx,int ny,int m);
procedure mgl_subplot(graph: HMGL; nx, ny, m: integer); cdecl; external libmgl;

//void mgl_subplot_d(HMGL graph, int nx,int ny,int m, float dx, float dy);
procedure mgl_subplot_d(graph: HMGL; nx, ny, m: integer; dx, dy: single); cdecl; external libmgl;

//void mgl_inplot(HMGL graph, float x1,float x2,float y1,float y2);
procedure mgl_inplot(graph: HMGL; x1, x2, y1, y2: single); cdecl; external libmgl;

//void mgl_aspect(HMGL graph, float Ax,float Ay,float Az);
procedure mgl_aspect(graph: HMGL; Ax, Ay, Az: single); cdecl; external libmgl;

//void mgl_rotate(HMGL graph, float TetX,float TetZ,float TetY);
procedure mgl_rotate(graph: HMGL; TetX, TetZ, TetY: single); cdecl; external libmgl;

//void mgl_rotate_vector(HMGL graph, float Tet,float x,float y,float z);
procedure mgl_rotate_vector(graph: HMGL; Tet, x, y, z: single); cdecl; external libmgl;

//void mgl_perspective(HMGL graph, float val);
procedure mgl_perspective(graph: HMGL; val: single); cdecl; external libmgl;

//*****************************************************************************/
//*		Axis functions														 */
//*****************************************************************************/
//void mgl_set_ticks(HMGL graph, float DX, float DY, float DZ);
procedure mgl_set_ticks(graph: HMGL; DX, DY, DZ: single); cdecl; external libmgl;

//void mgl_set_subticks(HMGL graph, int NX, int NY, int NZ);
procedure mgl_set_subticks(graph: HMGL; NX, NY, NZ: integer); cdecl; external libmgl;

//void mgl_set_caxis(HMGL graph, float C1,float C2);
procedure mgl_set_caxis(graph: HMGL; C1, C2: single); cdecl; external libmgl;

//void mgl_set_axis(HMGL graph, float x1, float y1, float z1, float x2, float y2, float z2, float x0, float y0, float z0);
procedure mgl_set_axis(graph: HMGL; x1, y1, z1, x2, y2, z2, x0, y0, z0: single); cdecl; external libmgl;

//void mgl_set_axis_3d(HMGL graph, float x1, float y1, float z1, float x2, float y2, float z2);
procedure mgl_set_axis_3d(graph: HMGL; x1, y1, z1, x2, y2, z2: single); cdecl; external libmgl;

//void mgl_set_axis_2d(HMGL graph, float x1, float y1, float x2, float y2);
procedure mgl_set_axis_2d(graph: HMGL; x1, y1, x2, y2: single); cdecl; external libmgl;

//void mgl_set_origin(HMGL graph, float x0, float y0, float z0);
procedure mgl_set_origin(graph: HMGL; x0, y0, z0: single); cdecl; external libmgl;

//void mgl_set_tick_origin(HMGL graph, float x0, float y0, float z0);
procedure mgl_set_tock_origin(graph: HMGL; x0, y0, z0: single); cdecl; external libmgl;

//void mgl_set_crange(HMGL graph, HMDT a, bool add);
procedure mgl_set_crange(graph: HMGL; a: HMDT; add: boolean); cdecl; external libmgl;

//void mgl_set_xrange(HMGL graph, HMDT a, bool add);
procedure mgl_set_xrange(graph: HMGL; a: HMDT; add: boolean); cdecl; external libmgl;

//void mgl_set_yrange(HMGL graph, HMDT a, bool add);
procedure mgl_set_yrange(graph: HMGL; a: HMDT; add: boolean); cdecl; external libmgl;

//void mgl_set_zrange(HMGL graph, HMDT a, bool add);
procedure mgl_set_zrange(graph: HMGL; a: HMDT; add: boolean); cdecl; external libmgl;

//void mgl_set_func(HMGL graph, const char *EqX,const char *EqY,const char *EqZ);
procedure mgl_set_func(graph: HMGL; const EqX: PChar; const EqY: PChar; const EqZ: PChar); cdecl; external libmgl;

//void mgl_set_cutoff(HMGL graph, const char *EqC);
procedure mgl_set_cutoff(graph: HMGL; const EqC: PChar); cdecl; external libmgl;

//void mgl_box(HMGL graph, bool ticks);
procedure mgl_box(graph: HMGL; ticks: boolean); cdecl; external libmgl;

//void mgl_box_str(HMGL graph, const char *col, bool ticks);
procedure mgl_box_str(graph: HMGL; const col: PChar; ticks: boolean); cdecl; external libmgl;

//void mgl_box_rgb(HMGL graph, float r, float g, float b, bool ticks);
procedure mgl_box_rgb(graph: HMGL; r, g, b: single; ticks: boolean); cdecl; external libmgl;

//void mgl_axis(HMGL graph, const char *dir);
procedure mgl_axis(graph: HMGL; const dir: PChar); cdecl; external libmgl;

//void mgl_axis_grid(HMGL graph, const char *dir,const char *pen);
procedure mgl_axis_grid(graph: HMGL; const dir: PChar; const pen: PChar); cdecl; external libmgl;

//void mgl_label(HMGL graph, char dir, const char *text);
procedure mgl_label(graph: HMGL; dir: char; const text: PChar); cdecl; external libmgl;

//void mgl_label_ext(HMGL graph, char dir, const char *text, int pos, float size);
procedure mgl_label_ext(graph: HMGL; dir: char; const text: PChar; pos: integer; size: single); cdecl; external libmgl;

//void mgl_tune_ticks(HMGL graph, bool tune, float fact_pos);
procedure mgl_tune_ticks(graph: HMGL; tune: boolean; fact_pos: single); cdecl; external libmgl;

//void mgl_set_xtt(HMGL graph, const wchar_t *templ);
procedure mgl_set_xtt(graph: HMGL; const templ: PWideChar); cdecl; external libmgl;

//void mgl_set_ytt(HMGL graph, const wchar_t *templ);
procedure mgl_set_ytt(graph: HMGL; const templ: PWideChar); cdecl; external libmgl;

//void mgl_set_ztt(HMGL graph, const wchar_t *templ);
procedure mgl_set_ztt(graph: HMGL; const templ: PWideChar); cdecl; external libmgl;

//void mgl_set_ctt(HMGL graph, const wchar_t *templ);
procedure mgl_set_ctt(graph: HMGL; const templ: PWideChar); cdecl; external libmgl;

//*****************************************************************************/
//*		Simple drawing														 */
//*****************************************************************************/
//void mgl_ball(HMGL graph, float x,float y,float z);
procedure mgl_ball(graph: HMGL; x, y, z: single); cdecl; external libmgl;

//void mgl_ball_rgb(HMGL graph, float x, float y, float z, float r, float g, float b, float alpha);
procedure mgl_ball_rgb(graph: HMGL; x, y, z, r, g, b, alpha: single); cdecl; external libmgl;

//void mgl_ball_str(HMGL graph, float x, float y, float z, const char *col);
procedure mgl_ball_str(graph: HMGL; x, y, z: single; const col: PChar); cdecl; external libmgl;

//void mgl_line(HMGL graph, float x1, float y1, float z1, float x2, float y2, float z2, const char *pen,int n);
procedure mgl_line(graph: HMGL; x1, y1, z1, x2, y2, z2: single; const pen: PChar; n: integer); cdecl; external libmgl;

//void mgl_facex(HMGL graph, float x0, float y0, float z0, float wy, float wz, const char *stl, float dx, float dy);
procedure mgl_facex(graph: HMGL; x0, y0, z0, wy, wz: single; const stl: PChar; dx, dy: single); cdecl; external libmgl;

//void mgl_facey(HMGL graph, float x0, float y0, float z0, float wx, float wz, const char *stl, float dx, float dy);
procedure mgl_facey(graph: HMGL; x0, y0, z0, wx, wz: single; const stl: PChar; dx, dy: single); cdecl; external libmgl;

//void mgl_facez(HMGL graph, float x0, float y0, float z0, float wx, float wy, const char *stl, float dx, float dy);
procedure mgl_facez(graph: HMGL; x0, y0, z0, wx, wy: single; const stl: PChar; dx, dy: single); cdecl; external libmgl;

//void mgl_curve(HMGL graph, float x1, float y1, float z1, float dx1, float dy1, float dz1, float x2, float y2, float z2, float dx2, float dy2, float dz2, const char *pen,int n);
procedure mgl_curve(graph: HMGL; x1, y1, z1, dx1, dy1, dz1, x2, y2, z2, dx2, dy2, dz2: single; const pen: PChar; n: integer); cdecl; external libmgl;

//void mgl_putsw(HMGL graph, float x, float y, float z,const wchar_t *text);
procedure mgl_putsw(graph: HMGL; x, y, z: single; const text: PWideChar); cdecl; external libmgl;

//void mgl_puts(HMGL graph, float x, float y, float z,const char *text);
procedure mgl_puts(graph: HMGL; x, y, z: single; const text: PChar); cdecl; external libmgl;

//void mgl_text(HMGL graph, float x, float y, float z,const char *text);
procedure mgl_text(graph: HMGL; x, y, z: single; const text: PChar); cdecl; external libmgl;

//void mgl_puts_ext(HMGL graph, float x, float y, float z,const char *text,const char *font,float size,char dir);
procedure mgl_puts_ext(graph: HMGL; x, y, z: single; const text: PChar; const font: PChar; size: single; dir: char); cdecl; external libmgl;

//void mgl_putsw_ext(HMGL graph, float x, float y, float z,const wchar_t *text,const char *font,float size,char dir);
procedure mgl_putsw_ext(graph: HMGL; x, y, z: single; const text: PWideChar; const font: PChar; size: single; dir: char); cdecl; external libmgl;

//void mgl_text_ext(HMGL graph, float x, float y, float z,const char *text,const char *font,float size,char dir);
procedure mgl_text_ext(graph: HMGL; x, y, z: single; const text: PChar; const font: PChar; size: single; dir: char); cdecl; external libmgl;

//void mgl_colorbar(HMGL graph, const char *sch,int where);
procedure mgl_colorbar(graph: HMGL; const sch: PCHar; where: integer); cdecl; external libmgl;

//void mgl_simple_plot(HMGL graph, HMDT a, int type, const char *stl);
procedure mgl_simple_plot(graph: HMGL; a: HMDT; ttype: integer; stl: PChar); cdecl; external libmgl;

//void mgl_add_legend(HMGL graph, const char *text,const char *style);
procedure mgl_add_legend(graph: HMGL; const text: PChar; const style: PChar); cdecl; external libmgl;

//void mgl_add_legendw(HMGL graph, const wchar_t *text,const char *style);
procedure mgl_add_legendw(graph: HMGL; const text: PWideChar; const style: PChar); cdecl; external libmgl;

//void mgl_clear_legend(HMGL graph);
procedure mgl_clear_legend(graph: HMGL); cdecl; external libmgl;

//void mgl_legend_xy(HMGL graph, float x, float y, const char *font, float size);
procedure mgl_legend_xy(graph: HMGL; x, y: single; const font: PChar; size: single); cdecl; external libmgl;

//void mgl_legend(HMGL graph, int where, const char *font, float size);
procedure mgl_legend(graph: HMGL; where: integer; const font: PChar; size: single); cdecl; external libmgl;

//*****************************************************************************/
//*		1D plotting functions												 */
//*****************************************************************************/
//void mgl_plot_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, const char *pen);
procedure mgl_plot_xyz(graph: HMGL; x, y, z: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_plot_xy(HMGL graph, HMDT x, HMDT y, const char *pen);
procedure mgl_plot_xy(graph: HMGL; x, y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_plot(HMGL graph, HMDT y,	const char *pen);
procedure mgl_plot(graph: HMGL; y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_plot_2(HMGL graph, HMDT a, const char *pen);
procedure mgl_plot_2(graph: HMGL; a: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_plot_3(HMGL graph, HMDT a, const char *pen);
procedure mgl_plot_3(graph: HMGL; a: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_area_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, const char *pen);
procedure mgl_area_xyz(graph: HMGL; x, y, z: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_area_xy(HMGL graph, HMDT x, HMDT y, const char *pen);
procedure mgl_area_xy(graph: HMGL; x, y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_area(HMGL graph, HMDT y, const char *pen);
procedure mgl_area(graph: HMGL; y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_area_2(HMGL graph, HMDT a, const char *pen);
procedure mgl_area_2(graph: HMGL; a: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_area_3(HMGL graph, HMDT a, const char *pen);
procedure mgl_area_3(graph: HMGL; a: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_mark(HMGL graph, float x,float y,float z,char mark);
procedure mgl_mark(graph: HMGL; x, y, z: single; mark: char); cdecl; external libmgl;

//void mgl_stem_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, const char *pen);
procedure mgl_stem_xyz(graph: HMGL; x, y, z: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_stem_xy(HMGL graph, HMDT x, HMDT y, const char *pen);
procedure mgl_stem_xy(graph: HMGL; x, y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_stem(HMGL graph, HMDT y,	const char *pen);
procedure mgl_stem(graph: HMGL; y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_stem_2(HMGL graph, HMDT a, const char *pen);
procedure mgl_stem_2(graph: HMGL; a: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_stem_3(HMGL graph, HMDT a, const char *pen);
procedure mgl_stem_3(graph: HMGL; a: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_step_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, const char *pen);
procedure mgl_step_xyz(graph: HMGL; x, y, z: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_step_xy(HMGL graph, HMDT x, HMDT y, const char *pen);
procedure mgl_step_xy(graph: HMGL; x, y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_step(HMGL graph, HMDT y,	const char *pen);
procedure mgl_step(graph: HMGL; y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_step_2(HMGL graph, HMDT a, const char *pen);
procedure mgl_step_2(graph: HMGL; a: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_step_3(HMGL graph, HMDT a, const char *pen);
procedure mgl_step_3(graph: HMGL; a: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_bars_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, const char *pen);
procedure mgl_bars_xyz(graph: HMGL; x, y, z: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_bars_xy(HMGL graph, HMDT x, HMDT y, const char *pen);
procedure mgl_bars_xy(graph: HMGL; x, y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_bars(HMGL graph, HMDT y,	const char *pen);
procedure mgl_bars(graph: HMGL; y: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_bars_2(HMGL graph, HMDT a, const char *pen);
procedure mgl_bars_2(graph: HMGL; a: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_bars_3(HMGL graph, HMDT a, const char *pen);
procedure mgl_bars_3(graph: HMGL; a: HMDT; const pen: PChar); cdecl; external libmgl;

//*****************************************************************************/
//*		Advanced 1D plotting functions												 */
//*****************************************************************************/
//void mgl_torus(HMGL graph, HMDT r, HMDT z, const char *pen);
procedure mgl_torus(graph: HMGL; r, z: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_torus_2(HMGL graph, HMDT a, const char *pen);
procedure mgl_torus_2(graph: HMGL; a: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_text_xyz(HMGL graph, HMDT x, HMDT y, HMDT z,const char *text, const char *font, float size);
procedure mgl_text_xyz(graph: HMGL; x, y, z: HMDT; const text: PChar; const font: PChar; size: single); cdecl; external libmgl;

//void mgl_text_xy(HMGL graph, HMDT x, HMDT y, const char *text, const char *font, float size);
procedure mgl_text_xy(graph: HMGL; x, y: HMDT; const text: PChar; const font: PChar; size: single); cdecl; external libmgl;

//void mgl_text_y(HMGL graph, HMDT y, const char *text, const char *font, float size);
procedure mgl_text_y(graph: HMGL; y: HMDT; const text: PChar; const font: PChar; size: single); cdecl; external libmgl;

//void mgl_chart(HMGL graph, HMDT a, const char *col);
procedure mgl_chart(graph: HMGL; a: HMDT; const col: PChar); cdecl; external libmgl;

//void mgl_error(HMGL graph, HMDT y, HMDT ey, const char *pen);
procedure mgl_error(graph: HMGL; y, ey: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_error_xy(HMGL graph, HMDT x, HMDT y, HMDT ey, const char *pen);
procedure mgl_error_xy(graph: HMGL; x, y, ey: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_error_exy(HMGL graph, HMDT x, HMDT y, HMDT ex, HMDT ey, const char *pen);
procedure mgl_error_exy(graph: HMGL; x, y, ex, ey: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_mark_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT r, const char *pen);
procedure mgl_mark_xyz(graph: HMGL; x, y, z, r: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_mark_xy(HMGL graph, HMDT x, HMDT y, HMDT r, const char *pen);
procedure mgl_mark_xy(graph: HMGL; x, y, z: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_mark_y(HMGL graph, HMDT y, HMDT r, const char *pen);
procedure mgl_mark_y(graph: HMGL; y, r: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_tube_xyzr(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT r, const char *pen);
procedure mgl_tube_xyzr(graph: HMGL; x, y, z, r: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_tube_xyr(HMGL graph, HMDT x, HMDT y, HMDT r, const char *pen);
procedure mgl_tube_xyr(graph: HMGL; x, y, r: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_tube_r(HMGL graph, HMDT y, HMDT r, const char *pen);
procedure mgl_tube_r(graph: HMGL; y, r: HMDT; const pen: PChar); cdecl; external libmgl;

//void mgl_tube_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, float r, const char *pen);
procedure mgl_tube_xyz(graph: HMGL; x, y, z: HMDT; r: single; const pen: PChar); cdecl; external libmgl;

//void mgl_tube_xy(HMGL graph, HMDT x, HMDT y, float r, const char *penl);
procedure mgl_tube_xy(graph: HMGL; x, y:  HMDT; r: single; const penl: PChar); cdecl; external libmgl;

//void mgl_tube(HMGL graph, HMDT y, float r, const char *pen);
procedure mgl_tube(graph: HMGL; y: HMDT; r: single; const pen: PChar); cdecl; external libmgl;



//void mgl_textmark_xyzr(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT r, const char *text, const char *fnt);
procedure mgl_textmark_xyzr(graph: HMGL; x, y, z, r: HMDT; const text: PChar; const fnt: PChar); cdecl; external libmgl;

//void mgl_textmark_xyr(HMGL graph, HMDT x, HMDT y, HMDT r, const char *text, const char *fnt);
procedure mgl_textmark_xyr(graph: HMGL; x, y, r: HMDT; const text: PChar; const fnt: PChar); cdecl; external libmgl;

//void mgl_textmark_yr(HMGL graph, HMDT y, HMDT r, const char *text, const char *fnt);
procedure mgl_textmark_yr(graph: HMGL; y, r: HMDT; const text: PChar; const fnt: PChar); cdecl; external libmgl;

//void mgl_textmark(HMGL graph, HMDT y, const char *text, const char *fnt);
procedure mgl_textmark(graph: HMGL; y: HMDT; const text: PChar; const fnt: PChar); cdecl; external libmgl;

//void mgl_textmarkw_xyzr(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT r, const wchar_t *text, const char *fnt);
procedure mgl_textmarkw_xyzr(graph: HMGL; x, y, z, r: HMDT; const text: PWideChar; const fnt: PChar); cdecl; external libmgl;

//void mgl_textmarkw_xyr(HMGL graph, HMDT x, HMDT y, HMDT r, const wchar_t *text, const char *fnt);
procedure mgl_textmarkw_xyr(graph: HMGL; x, y, r: HMDT; const text: PWideChar; const fnt: PChar); cdecl; external libmgl;

//void mgl_textmarkw_yr(HMGL graph, HMDT y, HMDT r, const wchar_t *text, const char *fnt);
procedure mgl_textmarkw_yr(graph: HMGL; y, r: HMDT; const text: PWideChar; const fnt: PChar); cdecl; external libmgl;

//void mgl_textmarkw(HMGL graph, HMDT y, const wchar_t *text, const char *fnt);
procedure mgl_textmarkw(graph: HMGL; y: HMDT; const text: PWideChar; const fnt: PChar); cdecl; external libmgl;

//*****************************************************************************/
//*		2D plotting functions												 */
//*****************************************************************************/
//void mgl_grid_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *stl,float zVal);
procedure mgl_grid_xy(graph: HMGL; x, y, z: HMDT; const stl: PChar; zVal: single); cdecl; external libmgl;

//void mgl_grid(HMGL graph, HMDT a,const char *stl,float zVal);
procedure mgl_grid(graph: HMGL; a: HMDT; const stl: PChar; zVal: single); cdecl; external libmgl;

//void mgl_mesh_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *sch);
procedure mgl_mesh_xy(graph: HMGL; x, y, z: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_mesh(HMGL graph, HMDT z, const char *sch);
procedure mgl_mesh(graph: HMGL; z: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_fall_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *sch);
procedure mgl_fall_xy(graph: HMGL; x, y, z: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_fall(HMGL graph, HMDT z, const char *sch);
procedure mgl_fall(graph: HMGL; z: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_belt_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *sch);
procedure mgl_belt_xy(graph: HMGL; x, y, z: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_belt(HMGL graph, HMDT z, const char *sch);
procedure mgl_belt(graph: HMGL; z: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_surf_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *sch);
procedure mgl_surf_xy(graph: HMGL; x, y, z: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_surf(HMGL graph, HMDT z, const char *sch);
procedure mgl_surf(graph: HMGL; z: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_dens_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *sch,float zVal);
procedure mgl_dens_xy(graph: HMGL; x, y, z: HMDT; const sch: PChar; zVal: single); cdecl; external libmgl;

//void mgl_dens(HMGL graph, HMDT z, const char *sch,float zVal);
procedure mgl_dens(graph: HMGL; z: HMDT; const sch: PChar; zVal: single); cdecl; external libmgl;

//void mgl_boxs_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *sch,float zVal);
procedure mgl_boxs_xy(graph: HMGL; x, y, z: HMDT; const sch: PChar; zVal: single); cdecl; external libmgl;

//void mgl_boxs(HMGL graph, HMDT z, const char *sch,float zVal);
procedure mgl_boxs(graph: HMGL; z: HMDT; const sch: PChar; zVal: single); cdecl; external libmgl;

//void mgl_tile_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *sch);
procedure mgl_tile_xy(graph: HMGL; x, y, z: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_tile(HMGL graph, HMDT z, const char *sch);
procedure mgl_tile(graph: HMGL; z: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_tile_rxy(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT r, const char *sch);
procedure mgl_tile_rxy(graph: HMGL; x, y, z, r: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_tile_r(HMGL graph, HMDT z, HMDT r, const char *sch);
procedure mgl_tile_r(graph: HMGL; z, r: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_cont_xy_val(HMGL graph, HMDT v, HMDT x, HMDT y, HMDT z, const char *sch, float zVal);
procedure mgl_cont_xy_val(graph: HMGL; v, x, y, z: HMDT; const sch: PChar; zVal: single); cdecl; external libmgl;

//void mgl_cont_val(HMGL graph, HMDT v, HMDT z, const char *sch,float zVal);
procedure mgl_cont_val(graph: HMGL; v, z: HMDT; const sch: PChar; zVal: single); cdecl; external libmgl;

//void mgl_cont_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *sch, int Num, float zVal);
procedure mgl_cont_xy(graph: HMGL; x, y, z: HMDT; const sch: PChar; Num: integer; zVal: single); cdecl; external libmgl;

//void mgl_cont(HMGL graph, HMDT z, const char *sch, int Num, float zVal);
procedure mgl_cont(graph: HMGL; z: HMDT; const sch: PChar; Num: integer; zVal: single); cdecl; external libmgl;

//void mgl_contf_xy_val(HMGL graph, HMDT v, HMDT x, HMDT y, HMDT z, const char *sch, float zVal);
procedure mgl_contf_xy_val(graph: HMGL; v, x, y, z: HMDT; const sch: PChar; zVal: single); cdecl; external libmgl;

//void mgl_contf_val(HMGL graph, HMDT v, HMDT z, const char *sch,float zVal);
procedure mgl_contf_val(graph: HMGL; v, z: HMDT; const sch: PChar; zVal: single); cdecl; external libmgl;

//void mgl_contf_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *sch, int Num, float zVal);
procedure mgl_contf_xy(graph: HMGL; x, y, z: HMDT; const sch: PChar; Num: integer; zVal: single); cdecl; external libmgl;

//void mgl_contf(HMGL graph, HMDT z, const char *sch, int Num, float zVal);
procedure mgl_contf(graph: HMGL; z: HMDT; const sch: PChar; Num: integer; zVal: single); cdecl; external libmgl;

//void mgl_axial_xy_val(HMGL graph, HMDT v, HMDT x, HMDT y, HMDT a, const char *sch);
procedure mgl_axial_xy_val(graph: HMGL; v, x, y, a: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_axial_val(HMGL graph, HMDT v, HMDT a, const char *sch);
procedure mgl_axial_val(graph: HMGL; v, a: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_axial_xy(HMGL graph, HMDT x, HMDT y, HMDT a, const char *sch, int Num);
procedure mgl_axial_xy(graph: HMGL; x, y, a: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_axial(HMGL graph, HMDT a, const char *sch, int Num);
procedure mgl_axial(graph: HMGL; a: HMDT; const sch: PChar; Num: integer); cdecl; external libmgl;

//*****************************************************************************/
//*		Dual plotting functions												 */
//*****************************************************************************/
//void mgl_surfc_xy(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT c, const char *sch);
procedure mgl_surfc_xy(graph: HMGL; x, y, z, c: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_surfc(HMGL graph, HMDT z, HMDT c, const char *sch);
procedure mgl_surfc(graph: HMGL; z, c: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_surfa_xy(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT c, const char *sch);
procedure mgl_surfa_xy(graph: HMGL; x, y, z, c: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_surfa(HMGL graph, HMDT z, HMDT c, const char *sch);
procedure mgl_surfa(graph: HMGL; z, c: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_stfa_xy(HMGL graph, HMDT x, HMDT y, HMDT re, HMDT im, int dn, const char *sch, float zVal);
procedure mgl_stfa_xy(graph: HMGL; x, y, re, im: HMDT; dn: integer; const sch: PChar; zVal: single); cdecl; external libmgl;

//void mgl_stfa(HMGL graph, HMDT re, HMDT im, int dn, const char *sch, float zVal);
procedure mgl_stfa(graph: HMGL; re, im: HMDT; dn: integer; const sch: PChar; zVal: single); cdecl; external libmgl;

//void mgl_vect_xy(HMGL graph, HMDT x, HMDT y, HMDT ax, HMDT ay, const char *sch,float zVal);
procedure mgl_vect_xy(graph: HMGL; x, y, z, ax, ay: HMDT; const sch: PChar; zVal: single); cdecl; external libmgl;

//void mgl_vect_2d(HMGL graph, HMDT ax, HMDT ay, const char *sch,float zVal);
procedure mgl_vect_2d(graph: HMGL; ax, ay: HMDT; const sch: PChar; zVal: single); cdecl; external libmgl;

//void mgl_vectc_xy(HMGL graph, HMDT x, HMDT y, HMDT ax, HMDT ay, const char *sch,float zVal);
procedure mgl_vectc_xy(graph: HMGL; x, y, ax, ay: HMDT; const sch: PChar; zVal: single); cdecl; external libmgl;

//void mgl_vectc_2d(HMGL graph, HMDT ax, HMDT ay, const char *sch,float zVal);
procedure mgl_vectc_2d(graph: HMGL; ax, ay: HMDT; const sch: PChar; zVal: single); cdecl; external libmgl;

//void mgl_vect_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT ax, HMDT ay, HMDT az, const char *sch);
procedure mgl_vect_xyz(graph: HMGL; x, y, z, ax, ay, az: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_vect_3d(HMGL graph, HMDT ax, HMDT ay, HMDT az, const char *sch);
procedure mgl_vect_3d(graph: HMGL; ax, ay, az: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_vectc_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT ax, HMDT ay, HMDT az, const char *sch);
procedure mgl_vectc_xyz(graph: HMGL); cdecl; external libmgl;

//void mgl_vectc_3d(HMGL graph, HMDT ax, HMDT ay, HMDT az, const char *sch);
procedure mgl_vectc_3d(graph: HMGL; ax, ay, az: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_map_xy(HMGL graph, HMDT x, HMDT y, HMDT a, HMDT b, const char *sch, int ks, bool pnts);
procedure mgl_map_xy(graph: HMGL; x, y, a, b: HMDT; const sch: PChar; ks: integer; pnts: boolean); cdecl; external libmgl;

//void mgl_map(HMGL graph, HMDT a, HMDT b, const char *sch, int ks, bool pnts);
procedure mgl_map(graph: HMGL; a, b: HMDT; const sch: PChar; ks: integer; pnts: boolean); cdecl; external libmgl;

//void mgl_surf3a_xyz_val(HMGL graph, float Val, HMDT x, HMDT y, HMDT z, HMDT a, HMDT b,
//			const char *stl);
procedure mgl_surf3a_xyz_val(graph: HMGL; Val: single; x, y, z, a, b: HMDT; const stla: PChar); cdecl; external libmgl;

//void mgl_surf3a_val(HMGL graph, float Val, HMDT a, HMDT b, const char *stl);
procedure mgl_surf3a_val(graph: HMGL; Val: single; a, b: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_surf3a_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a, HMDT b,
//			const char *stl, int num);
procedure mgl_surf3a_xyz(graph: HMGL; x, y, z, a, b: HMDT; const stl: PChar; num: integer); cdecl; external libmgl;

//void mgl_surf3a(HMGL graph, HMDT a, HMDT b, const char *stl, int num);
procedure mgl_surf3a(graph: HMGL; a, b: HMDT; const stl: PChar; num: integer); cdecl; external libmgl;

//void mgl_surf3c_xyz_val(HMGL graph, float Val, HMDT x, HMDT y, HMDT z, HMDT a, HMDT b,
//			const char *stl);
procedure mgl_surf3c_xyz_val(graph: HMGL; Val: single; x, y, z, a, b: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_surf3c_val(HMGL graph, float Val, HMDT a, HMDT b, const char *stl);
procedure mgl_surf3c_val(graph: HMGL; Val: single; a, b: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_surf3c_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a, HMDT b,
//			const char *stl, int num);
procedure mgl_surf3c_xyz(graph: HMGL; x, y, z, a, b: HMDT; const stl: PChar; num: integer); cdecl; external libmgl;

//void mgl_surf3c(HMGL graph, HMDT a, HMDT b, const char *stl, int num);
procedure mgl_surf3c(graph: HMGL; a, b: HMDT; const stl: PChar; num: integer); cdecl; external libmgl;

//void mgl_flow_xy(HMGL graph, HMDT x, HMDT y, HMDT ax, HMDT ay, const char *sch, int num, bool central, float zVal);
procedure mgl_flow_xy(graph: HMGL; x, y, ax, ay: HMDT; const sch: PChar; num: integer; central: boolean; zVal: single); cdecl; external libmgl;

//void mgl_flow_2d(HMGL graph, HMDT ax, HMDT ay, const char *sch, int num, bool central, float zVal);
procedure mgl_flow_2d(graph: HMGL; ax, ay: HMDT; const sch: PChar; num: integer; central: boolean; zVal: single); cdecl; external libmgl;

//void mgl_flow_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT ax, HMDT ay, HMDT az, const char *sch, int num, bool central);
procedure mgl_flow_xyz(graph: HMGL; x, y, z, ax, ay, az: HMDT; const sch: PChar; num: integer; central: boolean); cdecl; external libmgl;

//void mgl_flow_3d(HMGL graph, HMDT ax, HMDT ay, HMDT az, const char *sch, int num, bool central);
procedure mgl_flow_3d(graph: HMGL; ax, ay, az: HMDT; const sch: PChar; num: integer; central: boolean); cdecl; external libmgl;

//void mgl_pipe_xy(HMGL graph, HMDT x, HMDT y, HMDT ax, HMDT ay, const char *sch, float r0, int num, bool central, float zVal);
procedure mgl_pipe_xy(graph: HMGL; x, y, ax, ay: HMDT; const sch: PChar; r0: single; num: integer; central: boolean; zVal: single); cdecl; external libmgl;

//void mgl_pipe_2d(HMGL graph, HMDT ax, HMDT ay, const char *sch, float r0, int num, bool central, float zVal);
procedure mgl_pipe_2d(graph: HMGL; ax, ay: HMDT; const sch: PChar; r0: single; num: integer; central: boolean; zVal: single); cdecl; external libmgl;

//void mgl_pipe_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT ax, HMDT ay, HMDT az, const char *sch, float r0, int num, bool central);
procedure mgl_pipe_xyz(graph: HMGL; x, y, z, ax, ay, az: HMDT; const sch: PChar; r0: single; num: integer; central: boolean); cdecl; external libmgl;

//void mgl_pipe_3d(HMGL graph, HMDT ax, HMDT ay, HMDT az, const char *sch, float r0, int num, bool central);
procedure mgl_pipe_3d(graph: HMGL; ax, ay, az: HMDT; const sch: PChar; r0: single; num: integer; central: boolean); cdecl; external libmgl;

//*****************************************************************************/
//*		3D plotting functions												 */
//*****************************************************************************/
//void mgl_grid3_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a, char dir, int sVal, const char *stl);
procedure mgl_grid3_xyz(graph: HMGL; x, y, z, a: HMDT; dir: char; sVal: integer; const stl: PChar); cdecl; external libmgl;

//void mgl_grid3(HMGL graph, HMDT a, char dir, int sVal, const char *stl);
procedure mgl_grid3(graph: HMGL; a: HMDT; dir: char; sVal: integer; const stl: PChar); cdecl; external libmgl;

//void mgl_grid3_all_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a, const char *stl);
procedure mgl_grid3_all_xyz(graph: HMGL; x, y, z, a: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_grid3_all(HMGL graph, HMDT a, const char *stl);
procedure mgl_grid3_all(graph: HMGL; a: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_dens3_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a, char dir, int sVal, const char *stl);
procedure mgl_dens3_xyz(graph: HMGL; x, y, z, a: HMDT; dir: char; sVal: integer; const stl: PChar); cdecl; external libmgl;

//void mgl_dens3(HMGL graph, HMDT a, char dir, int sVal, const char *stl);
procedure mgl_dens3(graph: HMGL; a: HMDT; dir: char; sVal: integer; const stl: PChar); cdecl; external libmgl;

//void mgl_dens3_all_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a, const char *stl);
procedure mgl_dens3_all_xyz(graph: HMGL; x, y, z, a: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_dens3_all(HMGL graph, HMDT a, const char *stl);
procedure mgl_dens3_all(graph: HMGL; a: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_surf3_xyz_val(HMGL graph, float Val, HMDT x, HMDT y, HMDT z, HMDT a, const char *stl);
procedure mgl_surf3_xyz_val(graph: HMGL; Val: single; x, y, z, a: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_surf3_val(HMGL graph, float Val, HMDT a, const char *stl);
procedure mgl_surf3_val(graph: HMGL; Val: single; a: HMDT; const stl: PChar); cdecl; external libmgl;

//void mgl_surf3_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a, const char *stl, int num);
procedure mgl_surf3_xyz(graph: HMGL; x, y, z, a: HMDT; const stl: PChar; num: integer); cdecl; external libmgl;

//void mgl_surf3(HMGL graph, HMDT a, const char *stl, int num);
procedure mgl_surf3(graph: HMGL; a: HMDT; const stl: PChar; num: integer); cdecl; external libmgl;

//void mgl_cont3_xyz_val(HMGL graph, HMDT v, HMDT x, HMDT y, HMDT z, HMDT a,
//			char dir, int sVal, const char *sch);
procedure mgl_cont3_xyz_val(graph: HMGL; v, x, y, z, a: HMDT; dir: char; sVal: integer; const sch: PChar); cdecl; external libmgl;

//void mgl_cont3_val(HMGL graph, HMDT v, HMDT a, char dir, int sVal, const char *sch);
procedure mgl_cont3_val(graph: HMGL; v, a: HMDT; dir: char; sVal: integer; const sch: PChar); cdecl; external libmgl;

//void mgl_cont3_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a,
//			char dir, int sVal, const char *sch, int Num);
procedure mgl_cont3_xyz(graph: HMGL; x, y, z, a: HMDT; dir: char; sVal: integer; const sch: PChar; Num: integer); cdecl; external libmgl;

//void mgl_cont3(HMGL graph, HMDT a, char dir, int sVal, const char *sch, int Num);
procedure mgl_cont3(graph: HMGL; a: HMDT; dir: char; sVal: integer; const stl: PChar; Num: integer); cdecl; external libmgl;

//void mgl_cont_all_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a,
//			const char *sch, int Num);
procedure mgl_cont_all_xyz(graph: HMGL; x, y, z, a: HMDT; const sch: PChar; Num: integer); cdecl; external libmgl;

//void mgl_cont_all(HMGL graph, HMDT a, const char *sch, int Num);
procedure mgl_cont_all(graph: HMGL; a: HMDT; const sch: PChar; Num: integer); cdecl; external libmgl;

//void mgl_cloudp_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a,
//			const char *stl, float alpha);
procedure mgl_cloudp_xyz(graph: HMGL; x, y, z, a: HMDT; const stl: PChar; alpha: single); cdecl; external libmgl;

//void mgl_cloudp(HMGL graph, HMDT a, const char *stl, float alpha);
procedure mgl_cloudp(graph: HMGL; a: HMDT; const stl: PChar; alpha: single); cdecl; external libmgl;

//void mgl_cloudq_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a,
//			const char *stl, float alpha);
procedure mgl_cloudq_xyz(graph: HMGL; x, y, z, a: HMDT; const stl: PChar; alpha: single); cdecl; external libmgl;

//void mgl_cloudq(HMGL graph, HMDT a, const char *stl, float alpha);
procedure mgl_cloudq(graph: HMGL; a: HMDT; const stl: PChar; alpha: single); cdecl; external libmgl;

//void mgl_contf3_xyz_val(HMGL graph, HMDT v, HMDT x, HMDT y, HMDT z, HMDT a,
// 			char dir, int sVal, const char *sch);
procedure mgl_contf3_xyz_val(graph: HMGL; v, x, y, z, a: HMDT; dir: char; sVal: integer; const sch: PChar); cdecl; external libmgl;

//void mgl_contf3_val(HMGL graph, HMDT v, HMDT a, char dir, int sVal, const char *sch);
procedure mgl_contf3_val(graph: HMGL; v, a: HMDT; dir: char; sVal: integer; const stl: PChar); cdecl; external libmgl;

//void mgl_contf3_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a,
//			char dir, int sVal, const char *sch, int Num);
procedure mgl_contf3_xyz(graph: HMGL; x, y, z, a: HMDT; dir: char; sVal: integer; const sch: PChar; Num: integer); cdecl; external libmgl;

//void mgl_contf3(HMGL graph, HMDT a, char dir, int sVal, const char *sch, int Num);
procedure mgl_contf3(graph: HMGL; a: HMDT; dir: char; sVal: integer; const sch: PChar; Num: integer); cdecl; external libmgl;

//void mgl_contf_all_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a,
//			const char *sch, int Num);
procedure mgl_contf_all_xyz(graph: HMGL; x, y, z, a: HMDT; const sch: PChar; Num: integer); cdecl; external libmgl;

//void mgl_contf_all(HMGL graph, HMDT a, const char *sch, int Num);
procedure mgl_contf_all(graph: HMGL; a: HMDT; const sch: PChar; Num: integer); cdecl; external libmgl;

//void mgl_beam_val(HMGL graph, float Val, HMDT tr, HMDT g1, HMDT g2, HMDT a,
//		float r, const char *stl, int norm);
procedure mgl_beam_val(graph: HMGL; Val: single; tr, g1, g2, a: HMDT; r: single; const stl: PChar; norm: integer); cdecl; external libmgl;

//void mgl_beam(HMGL graph, HMDT tr, HMDT g1, HMDT g2, HMDT a, float r,
//		const char *stl, int norm, int num);
procedure mgl_beam(graph: HMGL; tr, g1, g2, a: HMDT; r: single; const stl: PChar; norm, num: integer); cdecl; external libmgl;

//*****************************************************************************/
//*		Triangular plotting functions											 */
//*****************************************************************************/
//void mgl_triplot_xyz(HMGL gr, HMDT nums, HMDT x, HMDT y, HMDT z, const char *sch);
procedure mgl_triplot_xyz(graph: HMGL; nums, x, y, z: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_triplot_xy(HMGL gr, HMDT nums, HMDT x, HMDT y, const char *sch, float zVal);
procedure mgl_triplot_xy(graph: HMGL; nums, x, y: HMDT; const sch: PChar; zVal: single); cdecl; external libmgl;

//void mgl_dots(HMGL gr, HMDT x, HMDT y, HMDT z, const char *sch);
procedure mgl_dots(graph: HMGL; x, y, z: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_dots_tr(HMGL gr, HMDT tr, const char *sch);
procedure mgl_dots_tr(graph: HMGL; tr: HMDT; const sch: PChar); cdecl; external libmgl;

//void mgl_crust(HMGL gr, HMDT x, HMDT y, HMDT z, const char *sch, float er);
procedure mgl_crust(graph: HMGL; x, y, z: HMDT; const sch: PChar; er: single); cdecl; external libmgl;

//void mgl_crust_tr(HMGL gr, HMDT tr, const char *sch, float er);
procedure mgl_crust_tr(graph: HMGL; tr: HMDT; const sch: PChar; er: single); cdecl; external libmgl;

//*****************************************************************************/
//*		Combined plotting functions											 */
//*****************************************************************************/
//void mgl_dens_x(HMGL graph, HMDT a, const char *stl, float sVal);
procedure mgl_dens_x(graph: HMGL; a: HMDT; const stl: PChar; sVal: single); cdecl; external libmgl;

//void mgl_dens_y(HMGL graph, HMDT a, const char *stl, float sVal);
procedure mgl_dens_y(graph: HMGL; a: HMDT; const stl: PChar; sVal: single); cdecl; external libmgl;

//void mgl_dens_z(HMGL graph, HMDT a, const char *stl, float sVal);
procedure mgl_dens_z(graph: HMGL; a: HMDT; const stl: PChar; sVal: single); cdecl; external libmgl;

//void mgl_cont_x(HMGL graph, HMDT a, const char *stl, float sVal, int Num);
procedure mgl_cont_x(graph: HMGL; a: HMDT; const stl: PChar; sVal: single; Num: integer); cdecl; external libmgl;

//void mgl_cont_y(HMGL graph, HMDT a, const char *stl, float sVal, int Num);
procedure mgl_cont_y(graph: HMGL; a: HMDT; const stl: PChar; sVal: single; Num: integer); cdecl; external libmgl;

//void mgl_cont_z(HMGL graph, HMDT a, const char *stl, float sVal, int Num);
procedure mgl_cont_z(graph: HMGL; a: HMDT; const stl: PChar; sVal: single; Num: integer); cdecl; external libmgl;

//void mgl_cont_x_val(HMGL graph, HMDT v, HMDT a, const char *stl, float sVal);
procedure mgl_cont_x_val(graph: HMGL; v, a: HMDT; const stl: PChar; sVal: single); cdecl; external libmgl;

//void mgl_cont_y_val(HMGL graph, HMDT v, HMDT a, const char *stl, float sVal);
procedure mgl_cont_y_val(graph: HMGL; v, a: HMDT; const stl: PChar; sVal: single); cdecl; external libmgl;

//void mgl_cont_z_val(HMGL graph, HMDT v, HMDT a, const char *stl, float sVal);
procedure mgl_cont_z_val(graph: HMGL; v, a: HMDT; const stl: PChar; sVal: single); cdecl; external libmgl;

//void mgl_contf_x(HMGL graph, HMDT a, const char *stl, float sVal, int Num);
procedure mgl_contf_x(graph: HMGL; a: HMDT; const stl: PChar; sVal: single; Num: integer); cdecl; external libmgl;

//void mgl_contf_y(HMGL graph, HMDT a, const char *stl, float sVal, int Num);
procedure mgl_contf_y(graph: HMGL; a: HMDT; const stl: PChar; sVal: single; Num: integer); cdecl; external libmgl;

//void mgl_contf_z(HMGL graph, HMDT a, const char *stl, float sVal, int Num);
procedure mgl_contf_z(graph: HMGL; a: HMDT; const stl: PChar; sVal: single; Num: integer); cdecl; external libmgl;

//void mgl_contf_x_val(HMGL graph, HMDT v, HMDT a, const char *stl, float sVal);
procedure mgl_contf_x_val(graph: HMGL; v, a: HMDT; const stl: PChar; sVal: single); cdecl; external libmgl;

//void mgl_contf_y_val(HMGL graph, HMDT v, HMDT a, const char *stl, float sVal);
procedure mgl_contf_y_val(graph: HMGL; v, a: HMDT; const stl: PChar; sVal: single); cdecl; external libmgl;

//void mgl_contf_z_val(HMGL graph, HMDT v, HMDT a, const char *stl, float sVal);
procedure mgl_contf_z_val(graph: HMGL; v, a: HMDT; const stl: PChar; sVal: single); cdecl; external libmgl;

//*****************************************************************************/
//*		Data creation functions												 */
//*****************************************************************************/
//void mgl_data_rearrange(HMDT dat, int mx, int my, int mz);
procedure mgl_data_rearrange(dat: HMDT; mx, my, mz: integer); cdecl; external libmgl;

//void mgl_data_set_float(HMDT dat, const float *A,int NX,int NY,int NZ);
procedure mgl_data_set_float(dat: HMDT; const A: PSingle; NX, NY, NZ: integer); cdecl; external libmgl;

//void mgl_data_set_double(HMDT dat, const double *A,int NX,int NY,int NZ);
procedure mgl_data_set_double(dat: HMDT; const A: PDouble; NX, NY, NZ: integer); cdecl; external libmgl;

//void mgl_data_set_float2(HMDT d, const float **A,int N1,int N2);
type PPSingle = ^PSingle;
procedure mgl_data_set_float2(d: HMDT; const A: PPSingle; N1, N2: integer); cdecl; external libmgl;

//void mgl_data_set_double2(HMDT d, const double **A,int N1,int N2);
type PPDouble = ^PDouble;
procedure mgl_data_set_double2(d: HMDT; const A: PPDouble; N1, N2: integer); cdecl; external libmgl;

//void mgl_data_set_float3(HMDT d, const float ***A,int N1,int N2,int N3);
type PPPSingle = ^PPSingle;
procedure mgl_data_set_float3(d: HMDT; const A: PPPSingle; N1, N2, N3: integer); cdecl; external libmgl;

//void mgl_data_set_double3(HMDT d, const double ***A,int N1,int N2,int N3);
type PPPDouble = ^PPDouble;
procedure mgl_data_set_double3(d: HMDT; const A: PPPDouble; N1, N2: integer); cdecl; external libmgl;

//void mgl_data_set(HMDT dat, HMDT a);
procedure mgl_data_set(dat, a: HMDT); cdecl; external libmgl;


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


//float mgl_data_get_value(HMDT dat, int i, int j, int k);
function mgl_data_get_value(dat: HMDT; i, j, k: integer): single; cdecl; external libmgl;

//void mgl_data_set_value(HMDT dat, int i, int j, int k, float v);
procedure mgl_data_set_value(dat: HMDT; i, j, k: integer; v: single); cdecl; external libmgl;

//bool mgl_data_read(HMDT dat, const char *fname);
function mgl_data_read(dat: HMDT; const fname: PChar): boolean; cdecl; external libmgl;

//bool mgl_data_read_mat(HMDT dat, const char *fname, int dim);
function mgl_data_read_mat(dat: HMDT; const fname: PChar; dim: integer): boolean; cdecl; external libmgl;

//bool mgl_data_read_dim(HMDT dat, const char *fname,int mx,int my,int mz);
function mgl_data_read_dim(dat: HMDT; const fname: PChar; mx, my, mz: integer): boolean; cdecl; external libmgl;

//void mgl_data_save(HMDT dat, const char *fname,int ns);
procedure mgl_data_save(dat: HMDT; const fname: PChar; ns: integer); cdecl; external libmgl;

//void mgl_data_export(HMDT dat, const char *fname, const char *scheme,float v1,float v2,int ns);
procedure mgl_data_export(dat: HMDT; const fname: PChar; const scheme: PChar; v1, v2: single; ns: integer); cdecl; external libmgl;

//void mgl_data_import(HMDT dat, const char *fname, const char *scheme,float v1,float v2);
procedure mgl_data_import(dat: HMDT; const fname: PChar; const scheme: PChar; v1, v2: single); cdecl; external libmgl;

//void mgl_data_create(HMDT dat, int nx,int ny,int nz);
procedure mgl_data_create(dat: HMDT; nx, ny, nz: integer); cdecl; external libmgl;

//void mgl_data_transpose(HMDT dat, const char *dim);
procedure mgl_data_transpose(dat: HMDT; const dim: PChar); cdecl; external libmgl;

//void mgl_data_norm(HMDT dat, float v1,float v2,bool sym,int dim);
procedure mgl_data_norm(dat: HMDT; v1, v2: single; sym: boolean; dim: integer); cdecl; external libmgl;

//void mgl_data_norm_slice(HMDT dat, float v1,float v2,char dir,bool keep_en,bool sym);
procedure mgl_data_norm_slice(dat: HMDT; v1, v2: single; dir: char; keep_en, sym: boolean); cdecl; external libmgl;

//HMDT mgl_data_subdata(HMDT dat, int xx,int yy,int zz);
function mgl_data_subdata(dat: HMDT; xx, yy, zz: integer): HMDT; cdecl; external libmgl;

//HMDT mgl_data_column(HMDT dat, const char *eq);
function mgl_data_column(dat: HMDT; const eq: PChar): HMDT; cdecl; external libmgl;

//void mgl_data_set_id(HMDT d, const char *id);
procedure mgl_data_set_id(d: HMDT; const id: PChar); cdecl; external libmgl;

//void mgl_data_fill(HMDT dat, float x1,float x2,char dir);
procedure mgl_data_fill(dat: HMDT; x1, x2: single; dir: char); cdecl; external libmgl;

//void mgl_data_modify(HMDT dat, const char *eq,int dim);
procedure mgl_data_modify(dat: HMDT; const eq: PChar; dim: integer); cdecl; external libmgl;

//void mgl_data_modify_vw(HMDT dat, const char *eq,HMDT vdat,HMDT wdat);
procedure mgl_data_modify_vw(dat: HMDT; const eq: PChar; vdat, wdat: HMDT); cdecl; external libmgl;

//void mgl_data_squeeze(HMDT dat, int rx,int ry,int rz,bool smooth);
procedure mgl_data_squeeze(dat: HMDT; rx, ry, rz: integer; smooth: boolean); cdecl; external libmgl;

//float mgl_data_max(HMDT dat);
function mgl_data_max(dat: HMDT): single; cdecl; external libmgl;

//float mgl_data_min(HMDT dat);
function mgl_data_min(dat: HMDT): single; cdecl; external libmgl;

//void mgl_data_insert(HMDT dat, const char *eq);
procedure mgl_data_insert(dat: HMDT; const eq: PChar); cdecl; external libmgl;

//void mgl_data_pullout(HMDT dat);
procedure mgl_data_pullout(dat: HMDT); cdecl; external libmgl;

//float &mgl_data_value(HMDT dat, int i,int j,int k);
function mgl_data_value(dat: HMDT; i, j, k: integer): single; cdecl; external libmgl;

//const float *mgl_data_data(HMDT dat);
function float(dat: HMDT): PSingle; cdecl; external libmgl;

//HMDT mgl_data_combine(HMDT dat1, HMDT dat2);
function mgl_data_combine(dat1, dat2: HMDT): HMDT; cdecl; external libmgl;

//void mgl_data_extend(HMDT dat, int n1, int n2);
procedure mgl_data_extend(dat: HMDT; n1, n2: integer); cdecl; external libmgl;

//*****************************************************************************/
//*		Data manipulation functions											 */
//*****************************************************************************/
//void mgl_data_smooth(HMDT dat, int Type,float delta,const char *dirs);
procedure mgl_data_smooth(dat: HMDT; tType: integer; delpa: single; const dirs: PChar); cdecl; external libmgl;

//HMDT mgl_data_sum(HMDT dat, const char *dir);
function mgl_data_sum(dat: HMDT; const dir: PChar): HMDT; cdecl; external libmgl;

//HMDT mgl_data_max_dir(HMDT dat, const char *dir);
function mgl_data_max_dir(dat: HMDT; const dir: PChar): HMDT; cdecl; external libmgl;

//HMDT mgl_data_min_dir(HMDT dat, const char *dir);
function mgl_data_min_dir(dat: HMDT; const dir: PChar): HMDT; cdecl; external libmgl;

//void mgl_data_cumsum(HMDT dat, const char *dir);
procedure mgl_data_cumsum(dat: HMDT; const dir: PChar); cdecl; external libmgl;

//void mgl_data_integral(HMDT dat, const char *dir);
procedure mgl_data_integral(dat: HMDT; const dir: PChar); cdecl; external libmgl;

//void mgl_data_diff(HMDT dat, const char *dir);
procedure mgl_data_diff(dat: HMDT; const dir: PChar); cdecl; external libmgl;

//void mgl_data_diff2(HMDT dat, const char *dir);
procedure mgl_data_diff2(dat: HMDT; const dir: PChar); cdecl; external libmgl;

//void mgl_data_swap(HMDT dat, const char *dir);
procedure mgl_data_swap(dat: HMDT; const dir: PChar); cdecl; external libmgl;

//void mgl_data_mirror(HMDT dat, const char *dir);
procedure mgl_data_mirror(dat: HMDT; const dir: PChar); cdecl; external libmgl;

//float mgl_data_spline(HMDT dat, float x,float y,float z);
function mgl_data_spline(dat: HMDT; x, y, z: single): single; cdecl; external libmgl;

//float mgl_data_spline1(HMDT dat, float x,float y,float z);
function mgl_data_spline1(dat: HMDT; x, y, z: single): single; cdecl; external libmgl;

//float mgl_data_linear(HMDT dat, float x,float y,float z);
function mgl_data_linear(dat: HMDT; x, y, z: single): single; cdecl; external libmgl;

//float mgl_data_linear1(HMDT dat, float x,float y,float z);
function mgl_data_linear1(dat: HMDT; x, y, z: single): single; cdecl; external libmgl;

//HMDT mgl_data_resize(HMDT dat, int mx,int my,int mz,float x1,float x2,
//	float y1,float y2,float z1,float z2);
function mgl_data_resize(dat: HMDT; mx, my, mz: integer; x1, x2, y1, y2, z1, z2: single): HMDT; cdecl; external libmgl;

//HMDT mgl_data_hist(HMDT dat, int n, float v1, float v2, int nsub);
function mgl_data_hist(dat: HMDT; n: integer; v1, v2: single; nsub: integer): HMDT; cdecl; external libmgl;

//HMDT mgl_data_hist_w(HMDT dat, HMDT weight, int n, float v1, float v2, int nsub);
function mgl_data_hist_w(dat, weight: HMDT; n: integer; v1, v2: single; nsub: integer): HMDT; cdecl; external libmgl;

//void mgl_data_crop(HMDT dat, int n1, int n2, char dir);
procedure mgl_data_crop(dat: HMDT; n1, n2: integer; dir: char); cdecl; external libmgl;

//*****************************************************************************/
//*		Data operations														 */
//*****************************************************************************/
//void mgl_data_mul_dat(HMDT dat, HMDT d);
procedure mgl_data_mul_dat(dat, d: HMDT); cdecl; external libmgl;

//void mgl_data_div_dat(HMDT dat, HMDT d);
procedure mgl_data_div_dat(dat, d: HMDT); cdecl; external libmgl;

//void mgl_data_add_dat(HMDT dat, HMDT d);
procedure mgl_data_add_dat(dat, d: HMDT); cdecl; external libmgl;

//void mgl_data_sub_dat(HMDT dat, HMDT d);
procedure mgl_data_sub_dat(dat, d: HMDT); cdecl; external libmgl;

//void mgl_data_mul_num(HMDT dat, float d);
procedure mgl_data_mul_num(dat: HMDT; d: single); cdecl; external libmgl;

//void mgl_data_div_num(HMDT dat, float d);
procedure mgl_data_div_num(dat: HMDT; d: single); cdecl; external libmgl;

//void mgl_data_add_num(HMDT dat, float d);
procedure mgl_data_add_num(dat: HMDT; d: single); cdecl; external libmgl;

//void mgl_data_sub_num(HMDT dat, float d);
procedure mgl_data_sub_num(dat: HMDT; d: single); cdecl; external libmgl;

//*****************************************************************************/
//*		Nonlinear fitting													 */
//*****************************************************************************/
//float mgl_fit_1(HMGL gr, HMDT fit, HMDT y, const char *eq, const char *var, float *ini, bool print);
function mgl_fit_1(gr: HMGL; fit, y: HMDT; const eq: PChar; const vvar: PChar; ini: PSingle; print: boolean): single; cdecl; external libmgl;

//float mgl_fit_2(HMGL gr, HMDT fit, HMDT z, const char *eq, const char *var, float *ini, bool print);
function mgl_fit_2(gr: HMGL; fit, z: HMDT; const eq: PChar; const vvar: PChar; ini: PSingle; print: boolean): single; cdecl; external libmgl;

//float mgl_fit_3(HMGL gr, HMDT fit, HMDT a, const char *eq, const char *var, float *ini, bool print);
function mgl_fit_3(gr: HMGL; fit, a: HMDT; const eq: PChar; const vvar: PChar; ini: PSingle; print: boolean): single; cdecl; external libmgl;

//float mgl_fit_xy(HMGL gr, HMDT fit, HMDT x, HMDT y, const char *eq, const char *var, float *ini, bool print);
function mgl_fit_xy(gr: HMGL; fit, x, y: HMDT; const eq: PChar; const vvar: PChar; ini: PSingle; print: boolean): single; cdecl; external libmgl;

//float mgl_fit_xyz(HMGL gr, HMDT fit, HMDT x, HMDT y, HMDT z, const char *eq, const char *var, float *ini, bool print);
function mgl_fit_xyz(gr: HMGL; fit, x, y, z: HMDT; const eq: PChar; const vvar: PChar; ini: PSingle; print: boolean): single; cdecl; external libmgl;

//float mgl_fit_xyza(HMGL gr, HMDT fit, HMDT x, HMDT y, HMDT z, HMDT a, const char *eq, const char *var, float *ini, bool print);
function mgl_fit_xyza(gr: HMGL; fit, x, y, z, a: HMDT; const eq: PChar; const vvar: PChar; ini: PSingle; print: boolean): single; cdecl; external libmgl;

//float mgl_fit_ys(HMGL gr, HMDT fit, HMDT y, HMDT s, const char *eq, const char *var, float *ini, bool print);
function mgl_fit_ys(gr: HMGL; fit, y, s: HMDT; const eq: PChar; const vvar: PChar; ini: PSingle; print: boolean): single; cdecl; external libmgl;

//float mgl_fit_xys(HMGL gr, HMDT fit, HMDT x, HMDT y, HMDT s, const char *eq, const char *var, float *ini, bool print);
function mgl_fit_xys(gr: HMGL; fit, x, y, s: HMDT; const eq: PChar; const vvar: PChar; ini: PSingle; print: boolean): single; cdecl; external libmgl;

//float mgl_fit_xyzs(HMGL gr, HMDT fit, HMDT x, HMDT y, HMDT z, HMDT s, const char *eq, const char *var, float *ini, bool print);
function mgl_fit_xyzs(gr: HMGL; fit, x, y, z, s: HMDT; const eq: PChar; const vvar: PChar; ini: PSingle; print: boolean): single; cdecl; external libmgl;

//float mgl_fit_xyzas(HMGL gr, HMDT fit, HMDT x, HMDT y, HMDT z, HMDT a, HMDT s, const char *eq, const char *var, float *ini, bool print);
function mgl_fit_xyzas(gr: HMGL; fit, x, y, z, a, s: HMDT; const eq: PChar; const vvar: PChar; ini: PSingle; print: boolean): single; cdecl; external libmgl;

//void mgl_puts_fit(HMGL gr, float x, float y, float z, const char *prefix, const char *font, float size);
procedure mgl_puts_fit(gr: HMGL; x, y, z: single; const prefix: PChar; const font: PChar; size: single); cdecl; external libmgl;

//*****************************************************************************/

//*****************************************************************************/
//*****************************************************************************/


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

end.
