/***************************************************************************
 * mgl_f.h is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _MGL_F_H_
#define _MGL_F_H_

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define _GR_	((mglGraph *)(*gr))
#define _PR_	((mglParse *)(*p))
#define _DT_	((mglData *)*d)
#define _D_(d)	*((const mglData *)*(d))
#define _DM_(d)	*((mglData *)*(d))
/*****************************************************************************/
uintptr_t mgl_create_graph_gl_();
uintptr_t mgl_create_graph_idtf_();
/*uintptr_t mgl_create_graph_glut_(int argc, char **argv, int _(*draw)_(mglGraph *gr, const void *p), const char *title, void _(*reload)_(int *next), const void *par);
uintptr_t mgl_create_graph_fltk_(int argc, char **argv, int _(*draw)_(mglGraph *gr, const void *p), const char *title, void _(*reload)_(int *next), const void *par);*/
uintptr_t mgl_create_graph_zb_(int *width, int *height);
uintptr_t mgl_create_graph_ps_(int *width, int *height);
void mgl_delete_graph_(uintptr_t *graph);

uintptr_t mgl_create_graph_glut_(int (*draw)(uintptr_t *gr), const char *title, int);
uintptr_t mgl_create_graph_fltk_(int (*draw)(uintptr_t *gr), const char *title, int);
uintptr_t mgl_create_graph_qt_(int (*draw)(uintptr_t *gr), const char *title, int);
void mgl_fltk_run_();
void mgl_qt_run_();
void mgl_set_show_mouse_pos_(uintptr_t *gr, int *enable);
void mgl_get_last_mouse_pos_(uintptr_t *gr, float *x, float *y, float *z);
//void mgl_fltk_thread_();
//void mgl_qt_thread_();
/*****************************************************************************/
uintptr_t mgl_create_data_();
uintptr_t mgl_create_data_size_(int *nx, int *ny, int *nz);
uintptr_t mgl_create_data_file_(const char *fname, int len);
void mgl_delete_data_(uintptr_t *dat);
/*****************************************************************************/
/*		Setup mglGraph														 */
/*****************************************************************************/
void mgl_set_def_param_(uintptr_t *gr);
void mgl_set_palette_(uintptr_t *gr, const char *colors, int);
void mgl_set_pal_color_(uintptr_t *graph, int *n, float *r, float *g, float *b);
void mgl_set_pal_num_(uintptr_t *graph, int *num);
void mgl_set_rotated_text_(uintptr_t *graph, int *rotated);
void mgl_set_cut_(uintptr_t *graph, int *cut);
void mgl_set_cut_box_(uintptr_t *gr, float *x1,float *y1,float *z1,float *x2,float *y2,float *z2);
void mgl_set_tick_len_(uintptr_t *graph, float *len);
void mgl_set_bar_width_(uintptr_t *graph, float *width);
void mgl_set_base_line_width_(uintptr_t *gr, float *size);
void mgl_set_mark_size_(uintptr_t *graph, float *size);
void mgl_set_arrow_size_(uintptr_t *graph, float *size);
void mgl_set_font_size_(uintptr_t *graph, float *size);
void mgl_set_font_def_(uintptr_t *graph, const char *fnt, int);
void mgl_set_alpha_default_(uintptr_t *graph, float *alpha);
void mgl_set_size_(uintptr_t *graph, int *width, int *height);
void mgl_set_axial_dir_(uintptr_t *graph, const char *dir,int);
void mgl_set_meshnum_(uintptr_t *graph, int *num);
void mgl_set_zoom_(uintptr_t *gr, float *x1, float *y1, float *x2, float *y2);
void mgl_set_plotfactor_(uintptr_t *gr, float *val);
void mgl_set_draw_face_(uintptr_t *gr, int *enable);
void mgl_set_scheme_(uintptr_t *gr, char *sch, int);
void mgl_load_font_(uintptr_t *gr, char *name, char *path, int l, int n);
void mgl_copy_font_(uintptr_t *gr, uintptr_t *gr_from);
void mgl_restore_font_(uintptr_t *gr);
/*****************************************************************************/
/*		Export to file or to memory														 */
/*****************************************************************************/
void mgl_show_image_(uintptr_t *graph, const char *viewer, int *keep, int);
void mgl_write_frame_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
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
const unsigned char *mgl_get_rgb_(uintptr_t *graph);
const unsigned char *mgl_get_rgba_(uintptr_t *graph);
int mgl_get_width_(uintptr_t *graph);
int mgl_get_height_(uintptr_t *graph);
/*****************************************************************************/
/*		Setup frames transparency _(alpha) and lightning						 */
/*****************************************************************************/
int mgl_new_frame_(uintptr_t *graph);
void mgl_end_frame_(uintptr_t *graph);
int mgl_get_num_frame_(uintptr_t *graph);
void mgl_reset_frames_(uintptr_t *graph);
void mgl_set_transp_type_(uintptr_t *graph, int *type);
void mgl_set_transp_(uintptr_t *graph, int *enable);
void mgl_set_alpha_(uintptr_t *graph, int *enable);
void mgl_set_fog_(uintptr_t *graph, float *dist, float *dz);
void mgl_set_light_(uintptr_t *graph, int *enable);
void mgl_set_light_n_(uintptr_t *gr, int *n, int *enable);
void mgl_add_light_(uintptr_t *graph, int *n, float *x, float *y, float *z, char *c, int);
void mgl_add_light_rgb_(uintptr_t *graph, int *n, float *x, float *y, float *z, int *infty,
						float *r, float *g, float *b, float *i);
void mgl_set_ambbr_(uintptr_t *gr, float *i);
/*****************************************************************************/
/*		Scale and rotate													 */
/*****************************************************************************/
void mgl_identity_(uintptr_t *graph);
void mgl_clf_(uintptr_t *graph);
void mgl_flush_(uintptr_t *gr);
void mgl_clf_rgb_(uintptr_t *graph, float *r, float *g, float *b);
void mgl_subplot_(uintptr_t *graph, int *nx,int *ny,int *m);
void mgl_subplot_d_(uintptr_t *graph, int *nx,int *ny,int *m,float *dx,float *dy);
void mgl_inplot_(uintptr_t *graph, float *x1,float *x2,float *y1,float *y2);
void mgl_relplot_(uintptr_t *graph, float *x1,float *x2,float *y1,float *y2);
void mgl_columnplot_(uintptr_t *graph, int *num, int *i);
void mgl_aspect_(uintptr_t *graph, float *Ax,float *Ay,float *Az);
void mgl_rotate_(uintptr_t *graph, float *TetX,float *TetZ,float *TetY);
void mgl_rotate_vector_(uintptr_t *graph, float *Tet,float *x,float *y,float *z);
void mgl_perspective_(uintptr_t *graph, float val);
/*****************************************************************************/
/*		Axis functions														 */
/*****************************************************************************/
void mgl_tune_ticks_(uintptr_t *gr, int *tune, float *fact_pos);
void mgl_adjust_ticks_(uintptr_t *gr, const char *dir, int l);
void mgl_set_ticks_dir_(uintptr_t *gr, char *dir, float *d, int *ns, float *org, int);
void mgl_set_ticks_(uintptr_t *graph, float *DX, float *DY, float *DZ);
void mgl_set_subticks_(uintptr_t *graph, int *NX, int *NY, int *NZ);
void mgl_set_caxis_(uintptr_t *graph, float *C1,float *C2);
void mgl_set_axis_(uintptr_t *graph, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2, float *x0, float *y0, float *z0);
void mgl_set_axis_3d_(uintptr_t *graph, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2);
void mgl_set_axis_2d_(uintptr_t *graph, float *x1, float *y1, float *x2, float *y2);
void mgl_set_origin_(uintptr_t *graph, float *x0, float *y0, float *z0);
void mgl_set_tick_origin_(uintptr_t *graph, float *x0, float *y0, float *z0);
void mgl_set_crange_(uintptr_t *graph, uintptr_t *a, int *add);
void mgl_set_xrange_(uintptr_t *graph, uintptr_t *a, int *add);
void mgl_set_yrange_(uintptr_t *graph, uintptr_t *a, int *add);
void mgl_set_zrange_(uintptr_t *graph, uintptr_t *a, int *add);
void mgl_set_auto_(uintptr_t *graph, float *x1, float *x2, float *y1, float *y2, float *z1, float *z2);
void mgl_set_func_(uintptr_t *graph, const char *EqX, const char *EqY, const char *EqZ, int, int, int);
void mgl_set_ternary_(uintptr_t *graph, int *enable);
void mgl_set_cutoff_(uintptr_t *graph, const char *EqC, int);
void mgl_box_(uintptr_t *graph, int *ticks);
void mgl_box_str_(uintptr_t *graph, const char *col, int *ticks, int);
void mgl_box_rgb_(uintptr_t *graph, float *r, float *g, float *b, int *ticks);
void mgl_axis_(uintptr_t *graph, const char *dir,int);
void mgl_axis_grid_(uintptr_t *graph, const char *dir,const char *pen,int,int);
void mgl_label_(uintptr_t *graph, const char *dir, const char *text,int,int);
void mgl_label_ext_(uintptr_t *graph, const char *dir, const char *text, int *pos, float *size, float *shift,int,int);
void mgl_set_xtt_(uintptr_t * graph, const char *templ,int);
void mgl_set_ytt_(uintptr_t * graph, const char *templ,int);
void mgl_set_ztt_(uintptr_t * graph, const char *templ,int);
void mgl_set_ctt_(uintptr_t * graph, const char *templ,int);
/*****************************************************************************/
/*		Simple drawing														 */
/*****************************************************************************/
void mgl_ball_(uintptr_t *graph, float *x,float *y,float *z);
void mgl_ball_rgb_(uintptr_t *graph, float *x, float *y, float *z, float *r, float *g, float *b, float *alpha);
void mgl_ball_str_(uintptr_t *graph, float *x, float *y, float *z, const char *col,int);
void mgl_line_(uintptr_t *graph, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2, const char *pen,int *n,int);

void mgl_facex_(uintptr_t* graph, float *x0, float *y0, float *z0, float *wy, float *wz, const char *stl, float *dx, float *dy, int l);
void mgl_facey_(uintptr_t* graph, float *x0, float *y0, float *z0, float *wx, float *wz, const char *stl, float *dx, float *dy, int l);
void mgl_facez_(uintptr_t* graph, float *x0, float *y0, float *z0, float *wx, float *wy, const char *stl, float *dx, float *dy, int l);
void mgl_curve_(uintptr_t* graph, float *x1, float *y1, float *z1, float *dx1, float *dy1, float *dz1, float *x2, float *y2, float *z2, float *dx2, float *dy2, float *dz2, const char *pen,int *n, int l);

void mgl_puts_(uintptr_t *graph, float *x, float *y, float *z,const char *text,int);
void mgl_puts_dir_(uintptr_t *graph, float *x, float *y, float *z, float *dx, float *dy, float *dz, const char *text, float size, int);
void mgl_text_(uintptr_t *graph, float *x, float *y, float *z,const char *text,int);
void mgl_title_(uintptr_t *graph, const char *text, const char *fnt, float *size, int,int);
void mgl_puts_ext_(uintptr_t *graph, float *x, float *y, float *z,const char *text,const char *font,float *size,const char *dir,int,int,int);
void mgl_text_ext_(uintptr_t *graph, float *x, float *y, float *z,const char *text,const char *font,float *size,const char *dir,int,int,int);
void mgl_colorbar_(uintptr_t *graph, const char *sch,int *where,int);
void mgl_colorbar_ext_(uintptr_t *graph, const char *sch,int *where, float *x, float *y, float *w, float *h, int);
void mgl_simple_plot_(uintptr_t *graph, uintptr_t *a, int *type, const char *stl,int);
void mgl_add_legend_(uintptr_t *graph, const char *text,const char *style,int,int);
void mgl_clear_legend_(uintptr_t *graph);
void mgl_legend_xy_(uintptr_t *graph, float *x, float *y, const char *font, float *size, float *llen,int);
void mgl_legend_(uintptr_t *graph, int *where, const char *font, float *size, float *llen,int);
void mgl_set_legend_box_(uintptr_t *gr, int *enable);
/*****************************************************************************/
/*		1D plotting functions												 */
/*****************************************************************************/
void mgl_fplot_(uintptr_t *graph, const char *fy, const char *stl, int *n, int ly, int ls);
void mgl_fplot_xyz_(uintptr_t *graph, const char *fx, const char *fy, const char *fz, const char *stl, int *n, int lx, int ly, int lz, int ls);
void mgl_plot_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen,int);
void mgl_plot_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *pen,int);
void mgl_plot_(uintptr_t *graph, uintptr_t *y,	const char *pen,int);
void mgl_plot_2_(uintptr_t *graph, uintptr_t *a, const char *pen,int);
void mgl_plot_3_(uintptr_t *graph, uintptr_t *a, const char *pen,int);
void mgl_area_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen,int);
void mgl_tens_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *c, const char *pen,int);
void mgl_tens_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *c, const char *pen,int);
void mgl_tens_(uintptr_t *graph, uintptr_t *y, uintptr_t *c, const char *pen,int);
void mgl_area_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *pen,int);
void mgl_area_(uintptr_t *graph, uintptr_t *y, const char *pen,int);
void mgl_area_2_(uintptr_t *graph, uintptr_t *a, const char *pen,int);
void mgl_area_3_(uintptr_t *graph, uintptr_t *a, const char *pen,int);
void mgl_region_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y1, uintptr_t *y2, const char *pen, int *inside, int);
void mgl_region_(uintptr_t *graph, uintptr_t *y1, uintptr_t *y2, const char *pen, int *inside, int);
void mgl_mark_(uintptr_t *graph, float *x,float *y,float *z,const char *mark,int);
void mgl_stem_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen,int);
void mgl_stem_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *pen,int);
void mgl_stem_(uintptr_t *graph, uintptr_t *y,	const char *pen,int);
void mgl_stem_2_(uintptr_t *graph, uintptr_t *a, const char *pen,int);
void mgl_stem_3_(uintptr_t *graph, uintptr_t *a, const char *pen,int);
void mgl_step_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen,int);
void mgl_step_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *pen,int);
void mgl_step_(uintptr_t *graph, uintptr_t *y,	const char *pen,int);
void mgl_step_2_(uintptr_t *graph, uintptr_t *a, const char *pen,int);
void mgl_step_3_(uintptr_t *graph, uintptr_t *a, const char *pen,int);
void mgl_bars_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *pen,int);
void mgl_bars_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *pen,int);
void mgl_bars_(uintptr_t *graph, uintptr_t *y,	const char *pen,int);
void mgl_bars_2_(uintptr_t *graph, uintptr_t *a, const char *pen,int);
void mgl_bars_3_(uintptr_t *graph, uintptr_t *a, const char *pen,int);
void mgl_barh_yx_(uintptr_t *graph, uintptr_t *y, uintptr_t *v, const char *pen,int);
void mgl_barh_(uintptr_t *graph, uintptr_t *v,	const char *pen,int);
/*****************************************************************************/
/*		Advanced 1D plotting functions												 */
/*****************************************************************************/
void mgl_torus_(uintptr_t *graph, uintptr_t *r, uintptr_t *z, const char *pen,int);
void mgl_torus_2_(uintptr_t *graph, uintptr_t *a, const char *pen,int);
void mgl_text_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z,const char *text, const char *font, float *size,int,int);
void mgl_text_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, const char *text, const char *font, float *size,int,int);
void mgl_text_y_(uintptr_t *graph, uintptr_t *y, const char *text, const char *font, float *size,int,int);
void mgl_chart_(uintptr_t *graph, uintptr_t *a, const char *col,int);
void mgl_error_(uintptr_t *graph, uintptr_t *y, uintptr_t *ey, const char *pen,int);
void mgl_error_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *ey, const char *pen,int);
void mgl_error_exy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *ex, uintptr_t *ey, const char *pen,int);
void mgl_mark_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *r, const char *pen,int);
void mgl_mark_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *r, const char *pen,int);
void mgl_mark_y_(uintptr_t *graph, uintptr_t *y, uintptr_t *r, const char *pen,int);
void mgl_tube_xyzr_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *r, const char *pen,int);
void mgl_tube_xyr_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *r, const char *pen,int);
void mgl_tube_r_(uintptr_t *graph, uintptr_t *y, uintptr_t *r, const char *pen,int);
void mgl_tube_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, float *r, const char *pen,int);
void mgl_tube_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, float *r, const char *pen,int);
void mgl_tube_(uintptr_t *graph, uintptr_t *y, float *r, const char *pen,int);

void mgl_textmark_xyzr_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *r, const char *text, const char *fnt,int,int);
void mgl_textmark_xyr_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *r, const char *text, const char *fnt,int,int);
void mgl_textmark_yr_(uintptr_t *graph, uintptr_t *y, uintptr_t *r, const char *text, const char *fnt,int,int);
void mgl_textmark_(uintptr_t *graph, uintptr_t *y, const char *text, const char *fnt,int,int);
/*****************************************************************************/
/*		2D plotting functions												 */
/*****************************************************************************/
void mgl_fsurf_(uintptr_t *graph, const char *fz, const char *stl, int *n, int lz, int ls);
void mgl_fsurf_xyz_(uintptr_t *graph, const char *fx, const char *fy, const char *fz, const char *stl, int *n, int lx, int ly, int lz, int ls);
void mgl_grid_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *stl,float *zVal,int);
void mgl_grid_(uintptr_t *graph, uintptr_t *a,const char *stl,float *zVal,int);
void mgl_mesh_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch,int);
void mgl_mesh_(uintptr_t *graph, uintptr_t *z, const char *sch,int);
void mgl_fall_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch,int);
void mgl_fall_(uintptr_t *graph, uintptr_t *z, const char *sch,int);
void mgl_belt_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch,int);
void mgl_belt_(uintptr_t *graph, uintptr_t *z, const char *sch,int);
void mgl_surf_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch,int);
void mgl_surf_(uintptr_t *graph, uintptr_t *z, const char *sch,int);
void mgl_dens_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch,float *zVal,int);
void mgl_dens_(uintptr_t *graph, uintptr_t *z, const char *sch,float *zVal,int);
void mgl_boxs_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch,float *zVal,int);
void mgl_boxs_(uintptr_t *graph, uintptr_t *z, const char *sch,float *zVal,int);
void mgl_tile_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch,int);
void mgl_tile_(uintptr_t *graph, uintptr_t *z, const char *sch,int);
void mgl_tiles_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *r, const char *sch,int);
void mgl_tiles_(uintptr_t *graph, uintptr_t *z, uintptr_t *r, const char *sch,int);
void mgl_cont_xy_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, float *zVal,int);
void mgl_cont_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *z, const char *sch,float *zVal,int);
void mgl_cont_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, int *Num, float *zVal,int);
void mgl_cont_(uintptr_t *graph, uintptr_t *z, const char *sch, int *Num, float *zVal,int);
void mgl_contf_xy_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, float *zVal,int);
void mgl_contf_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *z, const char *sch,float *zVal,int);
void mgl_contf_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, int *Num, float *zVal,int);
void mgl_contf_(uintptr_t *graph, uintptr_t *z, const char *sch, int *Num, float *zVal,int);
void mgl_axial_xy_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch,int);
void mgl_axial_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *a, const char *sch,int);
void mgl_axial_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch, int *Num,int);
void mgl_axial_(uintptr_t *graph, uintptr_t *a, const char *sch, int *Num,int);
/*****************************************************************************/
/*		Dual plotting functions												 */
/*****************************************************************************/
void mgl_surfc_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *c, const char *sch,int);
void mgl_surfc_(uintptr_t *graph, uintptr_t *z, uintptr_t *c, const char *sch,int);
void mgl_surfa_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *c, const char *sch,int);
void mgl_surfa_(uintptr_t *graph, uintptr_t *z, uintptr_t *c, const char *sch,int);
void mgl_stfa_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *re, uintptr_t *im, int *dn, const char *sch, float *zVal, int);
void mgl_stfa_(uintptr_t *graph, uintptr_t *re, uintptr_t *im, int *dn, const char *sch, float *zVal, int);
void mgl_vect_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch,float *zVal,int);
void mgl_vect_2d_(uintptr_t *graph, uintptr_t *ax, uintptr_t *ay, const char *sch,float *zVal,int);
void mgl_vectl_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch,float *zVal,int);
void mgl_vectl_2d_(uintptr_t *graph, uintptr_t *ax, uintptr_t *ay, const char *sch,float *zVal,int);
void mgl_vectc_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch,float *zVal,int);
void mgl_vectc_2d_(uintptr_t *graph, uintptr_t *ax, uintptr_t *ay, const char *sch,float *zVal,int);
void mgl_vect_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch,int);
void mgl_vect_3d_(uintptr_t *graph, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch,int);
void mgl_vectl_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch,int);
void mgl_vectl_3d_(uintptr_t *graph, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch,int);
void mgl_vectc_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch,int);
void mgl_vectc_3d_(uintptr_t *graph, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch,int);
void mgl_map_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *a, uintptr_t *b, const char *sch, int *ks, int *pnts,int);
void mgl_map_(uintptr_t *graph, uintptr_t *a, uintptr_t *b, const char *sch, int *ks, int *pnts,int);
void mgl_surf3a_xyz_val_(uintptr_t *graph, float *Val, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, uintptr_t *b,
			const char *stl,int);
void mgl_surf3a_val_(uintptr_t *graph, float *Val, uintptr_t *a, uintptr_t *b, const char *stl,int);
void mgl_surf3a_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, uintptr_t *b,
			const char *stl, int *num,int);
void mgl_surf3a_(uintptr_t *graph, uintptr_t *a, uintptr_t *b, const char *stl, int *num,int);
void mgl_surf3c_xyz_val_(uintptr_t *graph, float *Val, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, uintptr_t *b,
			const char *stl,int);
void mgl_surf3c_val_(uintptr_t *graph, float *Val, uintptr_t *a, uintptr_t *b, const char *stl,int);
void mgl_surf3c_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, uintptr_t *b,
			const char *stl, int *num,int);
void mgl_surf3c_(uintptr_t *graph, uintptr_t *a, uintptr_t *b, const char *stl, int *num,int);
void mgl_flow_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch, int *num, int *central, float *zVal,int);
void mgl_flow_2d_(uintptr_t *graph, uintptr_t *ax, uintptr_t *ay, const char *sch, int *num, int *central, float *zVal,int);
void mgl_flow_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, int *num, int *central,int);
void mgl_flow_3d_(uintptr_t *graph, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, int *num, int *central,int);
void mgl_pipe_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch, float *r0, int *num, int *central, float *zVal,int);
void mgl_pipe_2d_(uintptr_t *graph, uintptr_t *ax, uintptr_t *ay, const char *sch, float *r0, int *num, int *central, float *zVal,int);
void mgl_pipe_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, float *r0, int *num, int *central,int);
void mgl_pipe_3d_(uintptr_t *graph, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, float *r0, int *num, int *central,int);
void mgl_dew_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch, float *zVal,int l);
void mgl_dew_2d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, const char *sch, float *zVal,int l);
/*****************************************************************************/
/*		3D plotting functions												 */
/*****************************************************************************/
void mgl_grid3_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *dir, int *sVal, const char *stl,int,int);
void mgl_grid3_(uintptr_t *graph, uintptr_t *a, const char *dir, int *sVal, const char *stl,int,int);
void mgl_grid3_all_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *stl,int);
void mgl_grid3_all_(uintptr_t *graph, uintptr_t *a, const char *stl,int);
void mgl_dens3_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *dir, int *sVal, const char *stl,int,int);
void mgl_dens3_(uintptr_t *graph, uintptr_t *a, const char *dir, int *sVal, const char *stl,int,int);
void mgl_dens3_all_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *stl,int);
void mgl_dens3_all_(uintptr_t *graph, uintptr_t *a, const char *stl,int);
void mgl_surf3_xyz_val_(uintptr_t *graph, float *Val, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *stl,int);
void mgl_surf3_val_(uintptr_t *graph, float *Val, uintptr_t *a, const char *stl,int);
void mgl_surf3_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *stl, int *num,int);
void mgl_surf3_(uintptr_t *graph, uintptr_t *a, const char *stl, int *num,int);
void mgl_cont3_xyz_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a,
			const char *dir, int *sVal, const char *sch,int,int);
void mgl_cont3_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *a, const char *dir, int *sVal, const char *sch,int,int);
void mgl_cont3_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a,
			const char *dir, int *sVal, const char *sch, int *Num,int,int);
void mgl_cont3_(uintptr_t *graph, uintptr_t *a, const char *dir, int *sVal, const char *sch, int *Num,int,int);
void mgl_cont_all_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a,
			const char *sch, int *Num,int);
void mgl_cont_all_(uintptr_t *graph, uintptr_t *a, const char *sch, int *Num,int);
void mgl_cloudp_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a,
			const char *stl, float *alpha,int);
void mgl_cloudp_(uintptr_t *graph, uintptr_t *a, const char *stl, float *alpha,int);
void mgl_cloudq_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a,
			const char *stl, float *alpha,int);
void mgl_cloudq_(uintptr_t *graph, uintptr_t *a, const char *stl, float *alpha,int);
void mgl_contf3_xyz_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a,
			const char *dir, int *sVal, const char *sch,int,int);
void mgl_contf3_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *a, const char *dir, int *sVal, const char *sch,int,int);
void mgl_contf3_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a,
			const char *dir, int *sVal, const char *sch, int *Num,int,int);
void mgl_contf3_(uintptr_t *graph, uintptr_t *a, const char *dir, int *sVal, const char *sch, int *Num,int,int);
void mgl_contf_all_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a,
			const char *sch, int *Num,int);
void mgl_contf_all_(uintptr_t *graph, uintptr_t *a, const char *sch, int *Num,int);
void mgl_beam_val_(uintptr_t *gr, float *val, uintptr_t *tr, uintptr_t *g1, uintptr_t *g2, uintptr_t *a,
				float *r, const char *sch, int *norm,int l);
void mgl_beam_(uintptr_t *gr, uintptr_t *tr, uintptr_t *g1, uintptr_t *g2, uintptr_t *a, float *r,
			const char *sch, int *norm, int *num,int l);
/*****************************************************************************/
/*		Triangular plotting functions											 */
/*****************************************************************************/
void mgl_triplot_xyzc_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *c, const char *sch,int);
void mgl_triplot_xyz_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch,int);
void mgl_triplot_xy_(uintptr_t *gr, uintptr_t *nums, uintptr_t *x, uintptr_t *y, const char *sch, float *zVal,int);
void mgl_dots_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch,int);
void mgl_dots_tr_(uintptr_t *gr, uintptr_t *tr, const char *sch,int);
void mgl_crust_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, float *er,int);
void mgl_crust_tr_(uintptr_t *gr, uintptr_t *tr, const char *sch, float *er,int);
/*****************************************************************************/
/*		Combined plotting functions											 */
/*****************************************************************************/
void mgl_dens_x_(uintptr_t *graph, uintptr_t *a, const char *stl, float *sVal,int);
void mgl_dens_y_(uintptr_t *graph, uintptr_t *a, const char *stl, float *sVal,int);
void mgl_dens_z_(uintptr_t *graph, uintptr_t *a, const char *stl, float *sVal,int);
void mgl_cont_x_(uintptr_t *graph, uintptr_t *a, const char *stl, float *sVal, int *Num,int);
void mgl_cont_y_(uintptr_t *graph, uintptr_t *a, const char *stl, float *sVal, int *Num,int);
void mgl_cont_z_(uintptr_t *graph, uintptr_t *a, const char *stl, float *sVal, int *Num,int);
void mgl_cont_x_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *a, const char *stl, float *sVal,int);
void mgl_cont_y_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *a, const char *stl, float *sVal,int);
void mgl_cont_z_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *a, const char *stl, float *sVal,int);
void mgl_contf_x_(uintptr_t *graph, uintptr_t *a, const char *stl, float *sVal, int *Num,int);
void mgl_contf_y_(uintptr_t *graph, uintptr_t *a, const char *stl, float *sVal, int *Num,int);
void mgl_contf_z_(uintptr_t *graph, uintptr_t *a, const char *stl, float *sVal, int *Num,int);
void mgl_contf_x_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *a, const char *stl, float *sVal,int);
void mgl_contf_y_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *a, const char *stl, float *sVal,int);
void mgl_contf_z_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *a, const char *stl, float *sVal,int);
/*****************************************************************************/
/*		Data creation functions												 */
/*****************************************************************************/
void mgl_data_rearrange_(uintptr_t *dat, int *mx, int *my, int *mz);
void mgl_data_set_real_(uintptr_t *dat, const float *A,int *NX,int *NY,int *NZ);
void mgl_data_set_double_(uintptr_t *dat, const double *A,int *NX,int *NY,int *NZ);
void mgl_data_set_real1_(uintptr_t *d, const float *A,int *N1);
void mgl_data_set_double1_(uintptr_t *d, const double *A,int *N1);
void mgl_data_set_real2_(uintptr_t *d, const float *A,int *N1,int *N2);
void mgl_data_set_double2_(uintptr_t *d, const double *A,int *N1,int *N2);
void mgl_data_set_real3_(uintptr_t *d, const float *A,int *N1,int *N2,int *N3);
void mgl_data_set_double3_(uintptr_t *d, const double *A,int *N1,int *N2,int *N3);
void mgl_data_set_(uintptr_t *dat, uintptr_t *a);
float mgl_data_get_value_(uintptr_t *d, int *i, int *j, int *k);
int mgl_data_get_nx_(uintptr_t *d);
int mgl_data_get_ny_(uintptr_t *d);
int mgl_data_get_nz_(uintptr_t *d);
void mgl_data_set_value_(uintptr_t *d, float *v, int *i, int *j, int *k);
void mgl_data_set_values_(uintptr_t *d, const char *val, int *nx, int *ny, int *nz, int l);
int mgl_data_read_(uintptr_t *d, const char *fname,int l);
int mgl_data_read_mat_(uintptr_t *dat, const char *fname, int *dim, int);
int mgl_data_read_dim_(uintptr_t *dat, const char *fname,int *mx,int *my,int *mz,int);
void mgl_data_save_(uintptr_t *dat, const char *fname,int *ns,int);
void mgl_data_export_(uintptr_t *dat, const char *fname, const char *scheme,float *v1,float *v2,int *ns,int,int);
void mgl_data_import_(uintptr_t *dat, const char *fname, const char *scheme,float *v1,float *v2,int,int);
void mgl_data_create_(uintptr_t *dat, int *nx,int *ny,int *nz);
void mgl_data_transpose_(uintptr_t *dat, const char *dim,int);
void mgl_data_norm_(uintptr_t *dat, float *v1,float *v2,int *sym,int *dim);
void mgl_data_norm_slice_(uintptr_t *dat, float *v1,float *v2,char *dir,int *keep_en,int *sym,int l);
uintptr_t mgl_data_subdata_(uintptr_t *dat, int *xx,int *yy,int *zz);
uintptr_t mgl_data_column_(uintptr_t *dat, const char *eq,int l);
void mgl_data_set_id_(uintptr_t *dat, const char *id,int l);
void mgl_data_fill_(uintptr_t *dat, float *x1,float *x2,const char *dir,int);
void mgl_data_fill_eq_(uintptr_t *gr, uintptr_t *dat, const char *eq, uintptr_t *vdat, uintptr_t *wdat, int);
void mgl_data_put_val_(uintptr_t *dat, float *val, int *i, int *j, int *k);
void mgl_data_put_dat_(uintptr_t *dat, uintptr_t *val, int *i, int *j, int *k);
void mgl_data_modify_(uintptr_t *dat, const char *eq,int *dim,int);
void mgl_data_modify_vw_(uintptr_t *dat, const char *eq, uintptr_t *vdat, uintptr_t *wdat,int);
void mgl_data_squeeze_(uintptr_t *dat, int *rx,int *ry,int *rz,int *smooth);
float mgl_data_max_(uintptr_t *dat);
float mgl_data_min_(uintptr_t *dat);
const float *mgl_data_data_(uintptr_t *dat);
uintptr_t mgl_data_combine_(uintptr_t *dat1, uintptr_t *dat2);
void mgl_data_extend_(uintptr_t *dat, int *n1, int *n2);
void mgl_data_insert_(uintptr_t *dat, const char *dir, int *at, int *num, int);
void mgl_data_delete_(uintptr_t *dat, const char *dir, int *at, int *num, int);
/*****************************************************************************/
/*		Data manipulation functions											 */
/*****************************************************************************/
void mgl_data_smooth_(uintptr_t *dat, int *Type,float *delta,const char *dirs,int);
uintptr_t mgl_data_sum_(uintptr_t *dat, const char *dir,int);
uintptr_t mgl_data_max_dir_(uintptr_t *dat, const char *dir,int);
uintptr_t mgl_data_min_dir_(uintptr_t *dat, const char *dir,int);
void mgl_data_cumsum_(uintptr_t *dat, const char *dir,int);
void mgl_data_integral_(uintptr_t *dat, const char *dir,int);
void mgl_data_diff_(uintptr_t *dat, const char *dir,int);
void mgl_data_diff_par_(uintptr_t *dat, uintptr_t *v1, uintptr_t *v2, uintptr_t *v3);
void mgl_data_diff2_(uintptr_t *dat, const char *dir,int);
void mgl_data_swap_(uintptr_t *dat, const char *dir,int);
void mgl_data_mirror_(uintptr_t *dat, const char *dir,int);
float mgl_data_spline_(uintptr_t *dat, float *x,float *y,float *z);
float mgl_data_spline1_(uintptr_t *dat, float *x,float *y,float *z);
float mgl_data_linear_(uintptr_t *dat, float *x,float *y,float *z);
float mgl_data_linear1_(uintptr_t *dat, float *x,float *y,float *z);
uintptr_t mgl_data_resize_(uintptr_t *dat, int *mx,int *my,int *mz);
uintptr_t mgl_data_resize_box_(uintptr_t *dat, int *mx,int *my,int *mz,float *x1,float *x2,float *y1,float *y2,float *z1,float *z2);
uintptr_t mgl_data_momentum_(uintptr_t *dat, char *dir, const char *how, int,int);
uintptr_t mgl_data_hist_(uintptr_t *dat, int *n, float *v1, float *v2, int *nsub);
uintptr_t mgl_data_hist_w_(uintptr_t *dat, uintptr_t *weight, int *n, float *v1, float *v2, int *nsub);
uintptr_t mgl_data_evaluate_i_(uintptr_t *dat, uintptr_t *idat, int *norm);
uintptr_t mgl_data_evaluate_ij_(uintptr_t *dat, uintptr_t *idat, uintptr_t *jdat, int *norm);
uintptr_t mgl_data_evaluate_ijk_(uintptr_t *dat, uintptr_t *idat, uintptr_t *jdat, uintptr_t *kdat, int *norm);
void mgl_data_envelop_(uintptr_t *dat, const char *dir, int);
void mgl_data_sew_(uintptr_t *dat, const char *dirs, float *da, int);
void mgl_data_crop_(uintptr_t *dat, int *n1, int *n2, const char *dir,int);
/*****************************************************************************/
/*		Data operations														 */
/*****************************************************************************/
void mgl_data_mul_dat_(uintptr_t *dat, uintptr_t *d);
void mgl_data_div_dat_(uintptr_t *dat, uintptr_t *d);
void mgl_data_add_dat_(uintptr_t *dat, uintptr_t *d);
void mgl_data_sub_dat_(uintptr_t *dat, uintptr_t *d);
void mgl_data_mul_num_(uintptr_t *dat, float *d);
void mgl_data_div_num_(uintptr_t *dat, float *d);
void mgl_data_add_num_(uintptr_t *dat, float *d);
void mgl_data_sub_num_(uintptr_t *dat, float *d);
/*****************************************************************************/
/*		Nonlinear fitting													 */
/*****************************************************************************/
float mgl_fit_1_(uintptr_t* gr, uintptr_t* fit, uintptr_t* y, const char *eq, const char *var, float *ini, int l, int n);
float mgl_fit_2_(uintptr_t* gr, uintptr_t* fit, uintptr_t* z, const char *eq, const char *var, float *ini, int l, int n);
float mgl_fit_3_(uintptr_t* gr, uintptr_t* fit, uintptr_t* a, const char *eq, const char *var, float *ini, int l, int n);
float mgl_fit_xy_(uintptr_t* gr, uintptr_t* fit, uintptr_t* x, uintptr_t* y, const char *eq, const char *var, float *ini, int l, int n);
float mgl_fit_xyz_(uintptr_t* gr, uintptr_t* fit, uintptr_t* x, uintptr_t* y, uintptr_t* z, const char *eq, const char *var, float *ini, int l, int n);
float mgl_fit_xyza_(uintptr_t* gr, uintptr_t* fit, uintptr_t* x, uintptr_t* y, uintptr_t* z, uintptr_t* a, const char *eq, const char *var, float *ini, int l, int n);
float mgl_fit_ys_(uintptr_t* gr, uintptr_t* fit, uintptr_t* y, uintptr_t* ss, const char *eq, const char *var, float *ini, int l, int n);
float mgl_fit_xys_(uintptr_t* gr, uintptr_t* fit, uintptr_t* x, uintptr_t* y, uintptr_t* ss, const char *eq, const char *var, float *ini, int l, int n);
float mgl_fit_xyzs_(uintptr_t* gr, uintptr_t* fit, uintptr_t* x, uintptr_t* y, uintptr_t* z, uintptr_t* ss, const char *eq, const char *var, float *ini, int l, int n);
float mgl_fit_xyzas_(uintptr_t* gr, uintptr_t* fit, uintptr_t* x, uintptr_t* y, uintptr_t* z, uintptr_t* a, uintptr_t* ss, const char *eq, const char *var, float *ini, int l, int n);
void mgl_puts_fit_(uintptr_t* gr, float *x, float *y, float *z, const char *prefix, const char *font, float *size, int l, int n);
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
void mgl_sphere_(uintptr_t* graph, float *x, float *y, float *z, float *r, const char *stl, int);
void mgl_drop_(uintptr_t* graph, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2, float *r, const char *stl, float *shift, float *ap, int);
void mgl_cone_(uintptr_t* graph, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2, float *r1, float *r2, const char *stl, int *edge, int);

uintptr_t mgl_pde_solve_(uintptr_t* gr, const char *ham, uintptr_t* ini_re, uintptr_t* ini_im, float *dz, float *k0,int);
uintptr_t mgl_qo2d_solve_(const char *ham, uintptr_t* ini_re, uintptr_t* ini_im, uintptr_t* ray, float *r, float *k0, uintptr_t* xx, uintptr_t* yy, int);
uintptr_t mgl_af2d_solve_(const char *ham, uintptr_t* ini_re, uintptr_t* ini_im, uintptr_t* ray, float *r, float *k0, uintptr_t* xx, uintptr_t* yy, int);
uintptr_t mgl_ray_trace_(const char *ham, float *x0, float *y0, float *z0, float *px, float *py, float *pz, float *dt, float *tmax,int);
void mgl_data_fill_eq_(uintptr_t* gr, uintptr_t* res, const char *eq, uintptr_t* vdat, uintptr_t* wdat,int);
uintptr_t mgl_jacobian_2d_(uintptr_t* x, uintptr_t* y);
uintptr_t mgl_jacobian_3d_(uintptr_t* x, uintptr_t* y, uintptr_t* z);

uintptr_t mgl_transform_a_(uintptr_t *am, uintptr_t *ph, const char *tr, int);
uintptr_t mgl_transform_(uintptr_t *re, uintptr_t *im, const char *tr, int);
uintptr_t mgl_data_stfa_(uintptr_t *re, uintptr_t *im, int *dn, char *dir, int);

#ifdef __cplusplus
}
#endif

#endif /* _mgl_f_h_ */
