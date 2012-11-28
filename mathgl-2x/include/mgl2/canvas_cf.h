/***************************************************************************
 * canvas_cf.h is part of Math Graphic Library
 * Copyright (C) 2007-2012 Alexey Balakin <mathgl.abalakin@gmail.ru>       *
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
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

/// Create HMGL object with specified sizes
HMGL mgl_create_graph(int width, int height);
uintptr_t mgl_create_graph_(int *width, int *height);
/// Delete HMGL object
void mgl_delete_graph(HMGL gr);
void mgl_delete_graph_(uintptr_t *gr);
/// Set size of frame in pixels. Normally this function is called internally.
void mgl_set_size(HMGL gr, int width, int height);
void mgl_set_size_(uintptr_t *gr, int *width, int *height);
/// Set default parameters for plotting
void mgl_set_def_param(HMGL gr);
void mgl_set_def_param_(uintptr_t *gr);
/// Set plot quality
void mgl_set_quality(HMGL gr, int qual);
void mgl_set_quality_(uintptr_t *gr, int *qual);
/// Combine plots from 2 canvases. Result will be saved into gr
void mgl_combine_gr(HMGL gr, HMGL in);
void mgl_combine_gr_(uintptr_t *gr, uintptr_t *in);
/// Force preparing the image. It can be useful for OpenGL mode mostly.
void mgl_finish(HMGL gr);
void mgl_finish_(uintptr_t *gr);

/// Set tick length
void mgl_set_tick_len(HMGL gr, double len, double stt);
void mgl_set_tick_len_(uintptr_t *gr, mreal *len, mreal *stt);
/// Set axis and ticks style
void mgl_set_axis_stl(HMGL gr, const char *stl, const char *tck, const char *sub);
void mgl_set_axis_stl_(uintptr_t *gr, const char *stl, const char *tck, const char *sub, int,int,int);

/// Auto adjust ticks
void mgl_adjust_ticks(HMGL gr, const char *dir);
void mgl_adjust_ticks_(uintptr_t *gr, const char *dir, int);
/// Set the ticks parameters
void mgl_set_ticks(HMGL gr, char dir, double d, int ns, double org);
void mgl_set_ticks_(uintptr_t *gr, char *dir, mreal *d, int *ns, mreal *org, int);
/// Set ticks text (\n separated). Use "" to disable this feature.
void mgl_set_ticks_str(HMGL gr, char dir, const char *lbl, int add);
void mgl_set_ticks_str_(uintptr_t *gr, const char *dir, const char *lbl, int *add,int,int);
void mgl_set_ticks_wcs(HMGL gr, char dir, const wchar_t *lbl, int add);
/// Set ticks position and text (\n separated). Use "" to disable this feature.
void mgl_set_ticks_val(HMGL gr, char dir, HCDT val, const char *lbl, int add);
void mgl_set_ticks_val_(uintptr_t *gr, const char *dir, uintptr_t *val, const char *lbl, int *add,int,int);
void mgl_set_ticks_valw(HMGL gr, char dir, HCDT val, const wchar_t *lbl, int add);
/// Tune ticks
void mgl_tune_ticks(HMGL gr, int tune, double fact_pos);
void mgl_tune_ticks_(uintptr_t *gr, int *tune, mreal *fact_pos);
/// Set templates for ticks
void mgl_set_tick_templ(HMGL gr, char dir, const char *templ);
void mgl_set_tick_templ_(uintptr_t *gr, const char *dir, const char *templ,int,int);
void mgl_set_tick_templw(HMGL gr, char dir, const wchar_t *templ);
/// Set time templates for ticks
void mgl_set_ticks_time(HMGL gr, char dir, double d, const char *t);
void mgl_set_ticks_time_(uintptr_t *gr, const char *dir, mreal *d, const char *t,int,int);
/// Set additional shift of tick labels
void mgl_set_tick_shift(HMGL gr, double sx, double sy, double sz, double sc);
void mgl_set_tick_shift_(uintptr_t *gr, mreal *sx, mreal *sy, mreal *sz, mreal *sc);

/// Draws bounding box outside the plotting volume
void mgl_box(HMGL gr);
void mgl_box_(uintptr_t *gr);
/// Draws bounding box outside the plotting volume with color c
void mgl_box_str(HMGL gr, const char *col, int ticks);
void mgl_box_str_(uintptr_t *gr, const char *col, int *ticks, int);
/// Draw axises with ticks in direction(s) dir.
void mgl_axis(HMGL gr, const char *dir, const char *stl, const char *opt);
void mgl_axis_(uintptr_t *gr, const char *dir, const char *stl, const char *opt,int,int,int);
/// Draw grid lines perpendicular to direction(s) dir.
void mgl_axis_grid(HMGL gr, const char *dir,const char *pen, const char *opt);
void mgl_axis_grid_(uintptr_t *gr, const char *dir,const char *pen, const char *opt,int,int,int);
/// Print the label text for axis dir.
void mgl_label(HMGL gr, char dir, const char *text, double pos, const char *opt);
void mgl_label_(uintptr_t *gr, const char *dir, const char *text, const char *opt,int,int,int);
void mgl_labelw(HMGL gr, char dir, const wchar_t *text, double pos, const char *opt);

/// Draw colorbar at edge of axis
void mgl_colorbar(HMGL gr, const char *sch);
void mgl_colorbar_(uintptr_t *gr, const char *sch,int);
/// Draw colorbar at manual position
void mgl_colorbar_ext(HMGL gr, const char *sch, double x, double y, double w, double h);
void mgl_colorbar_ext_(uintptr_t *gr, const char *sch, mreal *x, mreal *y, mreal *w, mreal *h, int);
/// Draw colorbar with manual colors at edge of axis
void mgl_colorbar_val(HMGL gr, HCDT dat, const char *sch);
void mgl_colorbar_val_(uintptr_t *gr, uintptr_t *dat, const char *sch,int);
/// Draw colorbar with manual colors at manual position
void mgl_colorbar_val_ext(HMGL gr, HCDT dat, const char *sch,double x, double y, double w, double h);
void mgl_colorbar_val_ext_(uintptr_t *gr, uintptr_t *dat, const char *sch, mreal *x, mreal *y, mreal *w, mreal *h, int);

/// Add string to legend
void mgl_add_legend(HMGL gr, const char *text,const char *style);
void mgl_add_legend_(uintptr_t *gr, const char *text,const char *style,int,int);
void mgl_add_legendw(HMGL gr, const wchar_t *text,const char *style);
/// Clear saved legend string
void mgl_clear_legend(HMGL gr);
void mgl_clear_legend_(uintptr_t *gr);
/// Draw legend of accumulated strings at position {x,y}
void mgl_legend_pos(HMGL gr, double x, double y, const char *font, const char *opt);
void mgl_legend_pos_(uintptr_t *gr, mreal *x, mreal *y, const char *font, const char *opt,int,int);
/// Draw legend of accumulated strings
void mgl_legend(HMGL gr, int where, const char *font, const char *opt);
void mgl_legend_(uintptr_t *gr, int *where, const char *font, const char *opt,int,int);
/// Set number of marks in legend sample
void mgl_set_legend_marks(HMGL gr, int num);
void mgl_set_legend_marks_(uintptr_t *gr, int *num);

/// Show current image
void mgl_show_image(HMGL gr, const char *viewer, int keep);
void mgl_show_image_(uintptr_t *gr, const char *viewer, int *keep, int);
/// Write the frame in file (depending extension, write current frame if fname is empty)
void mgl_write_frame(HMGL gr, const char *fname,const char *descr);
void mgl_write_frame_(uintptr_t *gr, const char *fname,const char *descr,int,int);
/// Write the frame in file using BMP format
void mgl_write_tga(HMGL gr, const char *fname,const char *descr);
void mgl_write_tga_(uintptr_t *gr, const char *fname,const char *descr,int,int);
/// Write the frame in file using BMP format
void mgl_write_bmp(HMGL gr, const char *fname,const char *descr);
void mgl_write_bmp_(uintptr_t *gr, const char *fname,const char *descr,int,int);
/// Write the frame in file using JPEG format
void mgl_write_jpg(HMGL gr, const char *fname,const char *descr);
void mgl_write_jpg_(uintptr_t *gr, const char *fname,const char *descr,int,int);
/// Write the frame in file using PNG format with transparency
void mgl_write_png(HMGL gr, const char *fname,const char *descr);
void mgl_write_png_(uintptr_t *gr, const char *fname,const char *descr,int,int);
/// Write the frame in file using PNG format without transparency
void mgl_write_png_solid(HMGL gr, const char *fname,const char *descr);
void mgl_write_png_solid_(uintptr_t *gr, const char *fname,const char *descr,int,int);
/// Write the frame in file using PostScript format as bitmap
void mgl_write_bps(HMGL gr, const char *fname,const char *descr);
void mgl_write_bps_(uintptr_t *gr, const char *fname,const char *descr,int,int);
/// Write the frame in file using PostScript format
void mgl_write_eps(HMGL gr, const char *fname,const char *descr);
void mgl_write_eps_(uintptr_t *gr, const char *fname,const char *descr,int,int);
/// Write the frame in file using SVG format
void mgl_write_svg(HMGL gr, const char *fname,const char *descr);
void mgl_write_svg_(uintptr_t *gr, const char *fname,const char *descr,int,int);
/// Write the frame in file using LaTeX format
void mgl_write_tex(HMGL gr, const char *fname,const char *descr);
void mgl_write_tex_(uintptr_t *gr, const char *fname,const char *descr,int,int);
/// Write the frame in file using OBJ format
void mgl_write_obj(HMGL gr, const char *fname,const char *descr, int use_png);
void mgl_write_obj_(uintptr_t *gr, const char *fname,const char *descr, int *use_png,int,int);
/// Write the frame in file using STL format (faces only)
void mgl_write_stl(HMGL gr, const char *fname,const char *descr);
void mgl_write_stl_(uintptr_t *gr, const char *fname,const char *descr,int,int);
/// Write the frame in file using OFF format
void mgl_write_off(HMGL gr, const char *fname,const char *descr, int colored);
void mgl_write_off_(uintptr_t *gr, const char *fname,const char *descr,int *colored,int,int);
/// Write the frame in file using XYZ format
void mgl_write_xyz(HMGL gr, const char *fname,const char *descr);
void mgl_write_xyz_(uintptr_t *gr, const char *fname,const char *descr,int,int);

/*void mgl_write_x3d(HMGL gr, const char *fname,const char *descr);
void mgl_write_x3d_(uintptr_t *gr, const char *fname,const char *descr,int,int);
void mgl_write_wgl(HMGL gr, const char *fname,const char *descr);
void mgl_write_wgl_(uintptr_t *gr, const char *fname,const char *descr,int,int);*/

/// Write the frame in file using PRC format
void mgl_write_prc(HMGL gr, const char *fname,const char *descr, int make_pdf);
void mgl_write_prc_(uintptr_t *gr, const char *fname,const char *descr, int *make_pdf,int,int);
/// Write the frame in file using GIF format (only for current frame!)
void mgl_write_gif(HMGL gr, const char *fname,const char *descr);
void mgl_write_gif_(uintptr_t *gr, const char *fname,const char *descr,int,int);

/// Start write frames to cinema using GIF format
void mgl_start_gif(HMGL gr, const char *fname,int ms);
void mgl_start_gif_(uintptr_t *gr, const char *fname,int *ms,int);
/// Stop writing cinema using GIF format
void mgl_close_gif(HMGL gr);
void mgl_close_gif_(uintptr_t *gr);
/// Set name of plot for saving filename
void mgl_set_plotid(HMGL gr, const char *id);
void mgl_set_plotid_(uintptr_t *gr, const char *id,int);

/// Export points and primitives in file using MGLD format
void mgl_export_mgld(HMGL gr, const char *fname,const char *descr);
void mgl_export_mgld_(uintptr_t *gr, const char *fname,const char *descr,int,int);
/// Import points and primitives from file using MGLD format
void mgl_import_mgld(HMGL gr, const char *fname, int add);
void mgl_import_mgld_(uintptr_t *gr, const char *fname, int *add, int);
/// Export in JSON format suitable for later drawing by JavaScript
void mgl_write_json(HMGL gr, const char *fname,const char *descr);
void mgl_write_json_(uintptr_t *gr, const char *fname,const char *descr,int,int);

/// Get RGB values of current bitmap
const unsigned char *mgl_get_rgb(HMGL gr);
const unsigned char *mgl_get_rgb_(uintptr_t *gr);
/// Get RGBA values of current bitmap
const unsigned char *mgl_get_rgba(HMGL gr);
const unsigned char *mgl_get_rgba_(uintptr_t *gr);
/// Set object/subplot id
void mgl_set_obj_id(HMGL gr, int id);
void mgl_set_obj_id_(uintptr_t *gr, int *id);
/// Get object id
int mgl_get_obj_id(HMGL gr, int x, int y);
int mgl_get_obj_id_(uintptr_t *gr, int *x, int *y);
/// Get subplot id
int mgl_get_spl_id(HMGL gr, int x, int y);
int mgl_get_spl_id_(uintptr_t *gr, int *x, int *y);
/// Get width of the image
int mgl_get_width(HMGL gr);
int mgl_get_width_(uintptr_t *gr);
/// Get height of the image
int mgl_get_height(HMGL gr);
int mgl_get_height_(uintptr_t *gr);
/// Calculate 3D coordinate {x,y,z} for screen point {xs,ys}
void mgl_calc_xyz(HMGL gr, int xs, int ys, mreal *x, mreal *y, mreal *z);
void mgl_calc_xyz_(uintptr_t *gr, int *xs, int *ys, mreal *x, mreal *y, mreal *z);
/// Calculate screen point {xs,ys} for 3D coordinate {x,y,z}
void mgl_calc_scr(HMGL gr, double x, double y, double z, int *xs, int *ys);
void mgl_calc_scr_(uintptr_t *gr, mreal *x, mreal *y, mreal *z, int *xs, int *ys);
/// Check if {xs,ys} is close to active point with accuracy d, and return its position or -1
long mgl_is_active(HMGL gr, int xs, int ys, int d);
long mgl_is_active_(uintptr_t *gr, int *xs, int *ys, int *d);

/// Create new frame.
int mgl_new_frame(HMGL gr);
int mgl_new_frame_(uintptr_t *gr);
/// Finish frame drawing
void mgl_end_frame(HMGL gr);
void mgl_end_frame_(uintptr_t *gr);
/// Get the number of created frames
int mgl_get_num_frame(HMGL gr);
int mgl_get_num_frame_(uintptr_t *gr);
/// Reset frames counter (start it from zero)
void mgl_reset_frames(HMGL gr);
void mgl_reset_frames_(uintptr_t *gr);
/// Get drawing data for i-th frame (work if MGL_VECT_FRAME is set on)
void mgl_get_frame(HMGL gr, int i);
void mgl_get_frame_(uintptr_t *gr, int *i);
/// Append drawing data from i-th frame (work if MGL_VECT_FRAME is set on)
void mgl_show_frame(HMGL gr, int i);
void mgl_show_frame_(uintptr_t *gr, int *i);
/// Delete primitives for i-th frame (work if MGL_VECT_FRAME is set on)
void mgl_del_frame(HMGL gr, int i);
void mgl_del_frame_(uintptr_t *gr, int *i);

/// Set the transparency type (0 - usual, 1 - glass, 2 - lamp)
void mgl_set_transp_type(HMGL gr, int type);
void mgl_set_transp_type_(uintptr_t *gr, int *type);
/// Set the transparency on/off.
void mgl_set_alpha(HMGL gr, int enable);
void mgl_set_alpha_(uintptr_t *gr, int *enable);
/// Set the fog distance or switch it off (if d=0).
void mgl_set_fog(HMGL gr, double d, double dz);
void mgl_set_fog_(uintptr_t *gr, mreal *dist, mreal *dz);
/// Set the using of light on/off.
void mgl_set_light(HMGL gr, int enable);
void mgl_set_light_(uintptr_t *gr, int *enable);
/// Switch on/off the specified light source.
void mgl_set_light_n(HMGL gr, int n, int enable);
void mgl_set_light_n_(uintptr_t *gr, int *n, int *enable);

/// Add white light source at infinity.
void mgl_add_light(HMGL gr, int n, double x, double y, double z);
void mgl_add_light_(uintptr_t *gr, int *n, mreal *x, mreal *y, mreal *z);
/// Add light source at infinity (more settings).
void mgl_add_light_ext(HMGL gr, int n, double x, double y, double z, char c, double br, double ap);
void mgl_add_light_ext_(uintptr_t *gr, int *n, mreal *x, mreal *y, mreal *z, char *c, mreal *br, mreal *ap, int);
/// Add local light source.
void mgl_add_light_loc(HMGL gr, int n, double x, double y, double z, double dx, double dy, double dz, char c, double br, double ap);
void mgl_add_light_loc_(uintptr_t *gr, int *n, mreal *x, mreal *y, mreal *z, mreal *dx, mreal *dy, mreal *dz, char *c, mreal *br, mreal *ap, int);

/// Pop transformation matrix from stack
void mgl_mat_pop(HMGL gr);
void mgl_mat_pop_(uintptr_t *gr);
/// Push transformation matrix into stack
void mgl_mat_push(HMGL gr);
void mgl_mat_push_(uintptr_t *gr);

/// Clear up the frame
void mgl_clf(HMGL gr);
void mgl_clf_(uintptr_t *gr);
/// Clear up the frame and fill background by specified color
void mgl_clf_rgb(HMGL gr, double r, double g, double b);
void mgl_clf_rgb_(uintptr_t *gr, mreal *r, mreal *g, mreal *b);

/// Put further plotting in some region of whole frame.
void mgl_subplot(HMGL gr, int nx,int ny,int m,const char *style);
void mgl_subplot_(uintptr_t *gr, int *nx,int *ny,int *m, const char *s,int);
/// Put further plotting in some region of whole frame and shift it by distance {dx,dy}.
void mgl_subplot_d(HMGL gr, int nx,int ny,int m,const char *style, double dx, double dy);
void mgl_subplot_d_(uintptr_t *gr, int *nx,int *ny,int *m, mreal *dx, mreal *dy);
/// Like mgl_subplot() but "join" several cells
void mgl_multiplot(HMGL gr, int nx,int ny,int m,int dx,int dy,const char *style);
void mgl_multiplot_(uintptr_t *gr, int *nx,int *ny,int *m,int *dx,int *dy, const char *s,int);
/// Put further plotting in a region of whole frame.
void mgl_inplot(HMGL gr, double x1,double x2,double y1,double y2);
void mgl_inplot_(uintptr_t *gr, mreal *x1, mreal *x2, mreal *y1, mreal *y2);
/// Put further plotting in a region of current subplot/inplot.
void mgl_relplot(HMGL gr, double x1,double x2,double y1,double y2);
void mgl_relplot_(uintptr_t *gr, mreal *x1, mreal *x2, mreal *y1, mreal *y2);
/// Put further plotting in column cell of previous subplot/inplot.
void mgl_columnplot(HMGL gr, int num, int ind, double d);
void mgl_columnplot_(uintptr_t *gr, int *num, int *i, mreal *d);
/// Put further plotting in matrix cell of previous subplot/inplot.
void mgl_gridplot(HMGL gr, int nx, int ny, int m, double d);
void mgl_gridplot_(uintptr_t *gr, int *nx, int *ny, int *i, mreal *d);
/// Put further plotting in cell of stick rotated on angles tet, phi.
void mgl_stickplot(HMGL gr, int num, int ind, double tet, double phi);
void mgl_stickplot_(uintptr_t *gr, int *num, int *i, mreal *tet, mreal *phi);
/// Add title for current subplot/inplot.
void mgl_title(HMGL gr, const char *title, const char *stl, double size);
void mgl_title_(uintptr_t *gr, const char *title, const char *stl, mreal *size, int,int);
void mgl_titlew(HMGL gr, const wchar_t *title, const char *stl, double size);
/// Set factor of plot size
void mgl_set_plotfactor(HMGL gr, double val);
void mgl_set_plotfactor_(uintptr_t *gr, mreal *val);

/// Set aspect ratio for further plotting.
void mgl_aspect(HMGL gr, double Ax,double Ay,double Az);
void mgl_aspect_(uintptr_t *gr, mreal *Ax, mreal *Ay, mreal *Az);
/// Rotate a further plotting.
void mgl_rotate(HMGL gr, double TetX,double TetZ,double TetY);
void mgl_rotate_(uintptr_t *gr, mreal *TetX, mreal *TetZ, mreal *TetY);
/// Rotate a further plotting around vector {x,y,z}.
void mgl_rotate_vector(HMGL gr, double Tet,double x,double y,double z);
void mgl_rotate_vector_(uintptr_t *gr, mreal *Tet, mreal *x, mreal *y, mreal *z);
/// Set perspective (in range [0,1)) for plot. Set to zero for switching off.
void mgl_perspective(HMGL gr, double val);
void mgl_perspective_(uintptr_t *gr, double val);
/// Set angle of view independently from Rotate().
void mgl_view(HMGL gr, double TetX,double TetZ,double TetY);
void mgl_view_(uintptr_t *gr, mreal *TetX, mreal *TetZ, mreal *TetY);
/// Zoom in/out a part of picture (use mgl_zoom(0, 0, 1, 1) for restore default)
void mgl_zoom(HMGL gr, double x1, double y1, double x2, double y2);
void mgl_zoom_(uintptr_t *gr, mreal *x1, mreal *y1, mreal *x2, mreal *y2);

/// Send graphical information to node id using MPI
void mgl_mpi_send(HMGL gr, int id);
void mgl_mpi_send_(uintptr_t *gr, int *id);
/// Receive graphical information from node id using MPI
void mgl_mpi_recv(HMGL gr, int id);
void mgl_mpi_recv_(uintptr_t *gr, int *id);

//-----------------------------------------------------------------------------
void mgl_draw_thr(void *);
/// Callback function for mouse click
void mgl_set_click_func(HMGL gr, void (*func)(void *p));

/// Delay for animation in seconds
void mgl_wnd_set_delay(HMGL gr, double dt);
void mgl_wnd_set_delay_(uintptr_t *gr, mreal *dt);
/// Set window properties
void mgl_setup_window(HMGL gr, int clf_upd, int showpos);
void mgl_setup_window_(uintptr_t *gr, int *clf_upd, int *showpos);
/// Switch on/off transparency (do not overwrite user settings)
void mgl_wnd_toggle_alpha(HMGL gr);
void mgl_wnd_toggle_alpha_(uintptr_t *gr);
/// Switch on/off lighting (do not overwrite user settings)
void mgl_wnd_toggle_light(HMGL gr);
void mgl_wnd_toggle_light_(uintptr_t *gr);
/// Switch on/off zooming by mouse
void mgl_wnd_toggle_zoom(HMGL gr);
void mgl_wnd_toggle_zoom_(uintptr_t *gr);
/// Switch on/off rotation by mouse
void mgl_wnd_toggle_rotate(HMGL gr);
void mgl_wnd_toggle_rotate_(uintptr_t *gr);
/// Switch off all zooming and rotation
void mgl_wnd_toggle_no(HMGL gr);
void mgl_wnd_toggle_no_(uintptr_t *gr);
/// Update picture by calling user drawing function
void mgl_wnd_update(HMGL gr);
void mgl_wnd_update_(uintptr_t *gr);
/// Reload user data and update picture
void mgl_wnd_reload(HMGL gr);
void mgl_wnd_reload_(uintptr_t *gr);
/// Adjust size of bitmap to window size
void mgl_wnd_adjust(HMGL gr);
void mgl_wnd_adjust_(uintptr_t *gr);
/// Show next frame (if one)
void mgl_wnd_next_frame(HMGL gr);
void mgl_wnd_next_frame_(uintptr_t *gr);
/// Show previous frame (if one)
void mgl_wnd_prev_frame(HMGL gr);
void mgl_wnd_prev_frame_(uintptr_t *gr);
/// Run slideshow (animation) of frames
void mgl_wnd_animation(HMGL gr);
void mgl_wnd_animation_(uintptr_t *gr);
/// Get last mouse position
void mgl_get_last_mouse_pos(HMGL gr, mreal *x, mreal *y, mreal *z);
void mgl_get_last_mouse_pos_(uintptr_t *gr, mreal *x, mreal *y, mreal *z);
//-----------------------------------------------------------------------------
/// Create HMPR object for parsing MGL scripts
HMPR mgl_create_parser();
uintptr_t mgl_create_parser_();
/// Change counter of HMPR uses (for advanced users only). Non-zero counter prevent automatic object removing.
long mgl_use_parser(HMPR p, int inc);
long mgl_use_parser_(uintptr_t* , int *inc);
/// Delete HMPR object
void mgl_delete_parser(HMPR p);
void mgl_delete_parser_(uintptr_t* p);
/// Set value for parameter $N
void mgl_parser_add_param(HMPR p, int id, const char *str);
void mgl_parser_add_param_(uintptr_t* p, int *id, const char *str, int);
void mgl_parser_add_paramw(HMPR p, int id, const wchar_t *str);

/// Find variable with given name or add a new one
/// NOTE !!! You must not delete obtained data arrays !!!
HMDT mgl_parser_add_var(HMPR p, const char *name);
uintptr_t mgl_parser_add_var_(uintptr_t* p, const char *name, int);
HMDT mgl_parser_add_varw(HMPR p, const wchar_t *name);
/// Find variable with given name or return NULL if no one
/// NOTE !!! You must not delete obtained data arrays !!!
HMDT mgl_parser_find_var(HMPR p, const char *name);
uintptr_t mgl_parser_find_var_(uintptr_t* p, const char *name, int);
HMDT mgl_parser_find_varw(HMPR p, const wchar_t *name);

/// Delete variable with name
void mgl_parser_del_var(HMPR p, const char *name);
void mgl_parser_del_var_(uintptr_t* p, const char *name, int);
void mgl_parser_del_varw(HMPR p, const wchar_t *name);
/// Delete all data variables
void mgl_parser_del_all(HMPR p);
void mgl_parser_del_all_(uintptr_t *p);

/// Parse and draw single line of the MGL script
int mgl_parse_line(HMGL gr, HMPR p, const char *str, int pos);
int mgl_parse_line_(uintptr_t* gr, uintptr_t* p, const char *str, int *pos, int);
int mgl_parse_linew(HMGL gr, HMPR p, const wchar_t *str, int pos);
/// Execute and draw script from the file
void mgl_parse_file(HMGL gr, HMPR p, FILE *fp, int print);
/// Execute MGL script text with '\n' separated lines
void mgl_parse_text(HMGL gr, HMPR p, const char *str);
void mgl_parse_text_(uintptr_t* gr, uintptr_t* p, const char *str, int);
void mgl_parse_textw(HMGL gr, HMPR p, const wchar_t *str);

/// Restore once flag
void mgl_parser_restore_once(HMPR p);
void mgl_parser_restore_once_(uintptr_t* p);
/// Allow changing size of the picture
void mgl_parser_allow_setsize(HMPR p, int a);
void mgl_parser_allow_setsize_(uintptr_t* p, int *a);
/// Allow reading/saving files
void mgl_parser_allow_file_io(HMPR p, int a);
void mgl_parser_allow_file_io_(uintptr_t* p, int *a);
/// Set flag to stop script parsing
void mgl_parser_stop(HMPR p);
void mgl_parser_stop_(uintptr_t* p);

/// Return type of command: 0 - not found, 1 - data plot, 2 - other plot,
///		3 - setup, 4 - data handle, 5 - data create, 6 - subplot, 7 - program
///		8 - 1d plot, 9 - 2d plot, 10 - 3d plot, 11 - dd plot, 12 - vector plot
///		13 - axis, 14 - primitives, 15 - axis setup, 16 - text/legend, 17 - data transform
int mgl_parser_cmd_type(HMPR pr, const char *name);
int mgl_parser_cmd_type_(uintptr_t* p, const char *name, int);
/// Return description of MGL command
const char *mgl_parser_cmd_desc(HMPR pr, const char *name);
/// Return string of command format (command name and its argument[s])
const char *mgl_parser_cmd_frmt(HMPR pr, const char *name);
/// Get name of command with nmber n
const char *mgl_parser_cmd_name(HMPR pr, long id);
/// Get number of defined commands
long mgl_parser_cmd_num(HMPR pr);

/// Return result of formula evaluation
HMDT mgl_parser_calc(HMPR pr, const char *formula);
uintptr_t mgl_parser_calc_(uintptr_t *pr, const char *formula,int);
HMDT mgl_parser_calcw(HMPR pr, const wchar_t *formula);

//-----------------------------------------------------------------------------
/// Create HMEX object for expression evaluating
HMEX mgl_create_expr(const char *expr);
uintptr_t mgl_create_expr_(const char *expr, int);
/// Delete HMEX object
void mgl_delete_expr(HMEX ex);
void mgl_delete_expr_(uintptr_t *ex);
/// Return value of expression for given x,y,z variables
double mgl_expr_eval(HMEX ex, double x, double y,double z);
double mgl_eval_expr_(uintptr_t *ex, mreal *x, mreal *y, mreal *z);
/// Return value of expression for given variables
double mgl_expr_eval_v(HMEX ex, mreal *var);
/// Return value of expression differentiation over variable dir for given x,y,z variables
double mgl_expr_diff(HMEX ex, char dir, double x, double y,double z);
double mgl_diff_expr_(uintptr_t *ex, const char *dir, mreal *x, mreal *y, mreal *z, int);
/// Return value of expression differentiation over variable dir for given variables
double mgl_expr_diff_v(HMEX ex, char dir, mreal *var);

#ifdef __cplusplus
}
#endif
//-----------------------------------------------------------------------------
#endif

