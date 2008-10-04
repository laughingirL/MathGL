/* mgl_f.h is part of Math Graphic Library
 * Copyright _(C) 2007 Alexey Balakin <mathgl.abalakin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef _MGL_F_H_
#define _MGL_F_H_

#ifdef __cplusplus
extern "C" {
#endif
#define _GR_	((mglGraph *)(*gr))
#define _PR_	((mglParse *)(*p))
#define _DT_	((mglData *)*d)
#define _D_(d)	*((const mglData *)*(d))
#define _DM_(d)	*((mglData *)*(d))
/*****************************************************************************/
long mgl_create_graph_gl_();
long mgl_create_graph_idtf_();
/*long mgl_create_graph_glut_(int argc, char **argv, int _(*draw)_(mglGraph *gr, const void *p), const char *title, void _(*reload)_(int *next), const void *par);
long mgl_create_graph_fltk_(int argc, char **argv, int _(*draw)_(mglGraph *gr, const void *p), const char *title, void _(*reload)_(int *next), const void *par);*/
long mgl_create_graph_zb_(int *width, int *height);
long mgl_create_graph_ps_(int *width, int *height);
void mgl_delete_graph_(long *graph);

long mgl_create_graph_glut_(int (*draw)(long *gr, void *p), const char *title, void *par, int);
long mgl_create_graph_fltk_(int (*draw)(long *gr, void *p), const char *title, void *par, int);
long mgl_create_graph_qt_(int (*draw)(long *gr, void *p), const char *title, void *par, int);
void mgl_fltk_run_();
void mgl_qt_run_();
/*****************************************************************************/
long mgl_create_data_();
long mgl_create_data_size_(int *nx, int *ny, int *nz);
long mgl_create_data_file_(const char *fname, int len);
void mgl_delete_data_(long *dat);
/*****************************************************************************/
/*		Setup mglGraph														 */
/*****************************************************************************/
void mgl_set_palette_(long *gr, const char *colors, int);
void mgl_set_pal_color_(long *graph, int *n, float *r, float *g, float *b);
void mgl_set_pal_num_(long *graph, int *num);
void mgl_set_rotated_text_(long *graph, int *rotated);
void mgl_set_cut_(long *graph, int *cut);
void mgl_set_cut_box_(long *gr, float *x1,float *y1,float *z1,float *x2,float *y2,float *z2);
void mgl_set_bar_width_(long *graph, float *width);
void mgl_set_base_line_width_(long *gr, float *size);
void mgl_set_mark_size_(long *graph, float *size);
void mgl_set_arrow_size_(long *graph, float *size);
void mgl_set_font_size_(long *graph, float *size);
void mgl_set_alpha_default_(long *graph, float *alpha);
void mgl_set_size_(long *graph, int *width, int *height);
void mgl_set_axial_dir_(long *graph, const char *dir,int);
void mgl_set_meshnum_(long *graph, int *num);
void mgl_set_zoom_(long *gr, float *x1, float *y1, float *x2, float *y2);
void mgl_set_plotfactor_(long *gr, float *val);
void mgl_set_draw_face_(long *gr, int *enable);
void mgl_set_scheme_(long *gr, char *sch, int);
void mgl_set_font_(long *gr, char *name, char *path, int l, int n);
void mgl_copy_font_(long *gr, long *gr_from);
void mgl_restore_font_(long *gr);
/*****************************************************************************/
/*		Export to file or to memory														 */
/*****************************************************************************/
void mgl_write_tif_(long *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_jpg_(long *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_png_(long *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_png_solid_(long *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_eps_(long *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_svg_(long *graph, const char *fname,const char *descr,int lf,int ld);
const unsigned char *mgl_get_rgb_(long *graph);
const unsigned char *mgl_get_rgba_(long *graph);
int mgl_get_width_(long *graph);
int mgl_get_height_(long *graph);
/*****************************************************************************/
/*		Setup frames transparency _(alpha) and lightning						 */
/*****************************************************************************/
int mgl_new_frame_(long *graph, int *id);
void mgl_end_frame_(long *graph);
int mgl_get_num_frame_(long *graph);
void mgl_set_transp_type_(long *graph, int *type);
void mgl_set_transp_(long *graph, int *enable);
void mgl_set_alpha_(long *graph, int *enable);
void mgl_set_fog_(long *graph, float *dist, float *dz);
void mgl_set_light_(long *graph, int *enable);
void mgl_add_light_(long *graph, int *n, float *x, float *y, float *z, int *infty);
void mgl_add_light_rgb_(long *graph, int *n, float *x, float *y, float *z, int *infty,
						float *r, float *g, float *b, float *i);
void mgl_set_ambbr_(long *gr, float *i);
/*****************************************************************************/
/*		Scale and rotate													 */
/*****************************************************************************/
void mgl_identity_(long *graph);
void mgl_clf_(long *graph);
void mgl_clf_rgb_(long *graph, float *r, float *g, float *b);
void mgl_subplot_(long *graph, int *nx,int *ny,int *m);
void mgl_subplot_d_(long *graph, int *nx,int *ny,int *m,float *dx,float *dy);
void mgl_inplot_(long *graph, float *x1,float *x2,float *y1,float *y2);
void mgl_aspect_(long *graph, float *Ax,float *Ay,float *Az);
void mgl_rotate_(long *graph, float *TetX,float *TetZ,float *TetY);
void mgl_rotate_vector_(long *graph, float *Tet,float *x,float *y,float *z);
void mgl_perspective_(long *graph, float val);
/*****************************************************************************/
/*		Axis functions														 */
/*****************************************************************************/
void mgl_tune_ticks_(long *gr, int *tune, float *fact_pos);
void mgl_set_ticks_(long *graph, float *DX, float *DY, float *DZ);
void mgl_set_subticks_(long *graph, int *NX, int *NY, int *NZ);
void mgl_set_caxis_(long *graph, float *C1,float *C2);
void mgl_set_axis_(long *graph, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2, float *x0, float *y0, float *z0);
void mgl_set_axis_3d_(long *graph, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2);
void mgl_set_axis_2d_(long *graph, float *x1, float *y1, float *x2, float *y2);
void mgl_set_origin_(long *graph, float *x0, float *y0, float *z0);
void mgl_set_tick_origin_(long *graph, float *x0, float *y0, float *z0);
void mgl_set_crange_(long *graph, long *a, int *add);
void mgl_set_xrange_(long *graph, long *a, int *add);
void mgl_set_yrange_(long *graph, long *a, int *add);
void mgl_set_zrange_(long *graph, long *a, int *add);
void mgl_set_func_(long *graph, const char *EqX, const char *EqY, const char *EqZ, int, int, int);
void mgl_set_cutoff_(long *graph, const char *EqC, int);
void mgl_box_(long *graph, int *ticks);
void mgl_box_str_(long *graph, const char *col, int *ticks, int);
void mgl_box_rgb_(long *graph, float *r, float *g, float *b, int *ticks);
void mgl_axis_(long *graph, const char *dir,int);
void mgl_axis_grid_(long *graph, const char *dir,const char *pen,int,int);
void mgl_label_(long *graph, const char *dir, const char *text,int,int);
void mgl_label_ext_(long *graph, const char *dir, const char *text, int *pos, float *size, float *shift,int,int);
/*****************************************************************************/
/*		Simple drawing														 */
/*****************************************************************************/
void mgl_ball_(long *graph, float *x,float *y,float *z);
void mgl_ball_rgb_(long *graph, float *x, float *y, float *z, float *r, float *g, float *b, float *alpha);
void mgl_ball_str_(long *graph, float *x, float *y, float *z, const char *col,int);
void mgl_line_(long *graph, float *x1, float *y1, float *z1, float *x2, float *y2, float *z2, const char *pen,int *n,int);

void mgl_facex_(long* graph, float *x0, float *y0, float *z0, float *wy, float *wz, const char *stl, float *dx, float *dy, int l);
void mgl_facey_(long* graph, float *x0, float *y0, float *z0, float *wx, float *wz, const char *stl, float *dx, float *dy, int l);
void mgl_facez_(long* graph, float *x0, float *y0, float *z0, float *wx, float *wy, const char *stl, float *dx, float *dy, int l);
void mgl_curve_(long* graph, float *x1, float *y1, float *z1, float *dx1, float *dy1, float *dz1, float *x2, float *y2, float *z2, float *dx2, float *dy2, float *dz2, const char *pen,int *n, int l);

void mgl_puts_(long *graph, float *x, float *y, float *z,const char *text,int);
void mgl_puts_dir_(long *graph, float *x, float *y, float *z, float *dx, float *dy, float *dz, const char *text, float size, int);
void mgl_text_(long *graph, float *x, float *y, float *z,const char *text,int);
void mgl_title_(long *graph, const char *text, const char *fnt,int,int);
void mgl_puts_ext_(long *graph, float *x, float *y, float *z,const char *text,const char *font,float *size,const char *dir,int,int,int);
void mgl_text_ext_(long *graph, float *x, float *y, float *z,const char *text,const char *font,float *size,const char *dir,int,int,int);
void mgl_colorbar_(long *graph, const char *sch,int *where,int);
void mgl_simple_plot_(long *graph, long *a, int *type, const char *stl,int);
void mgl_add_legend_(long *graph, const char *text,const char *style,int,int);
void mgl_clear_legend_(long *graph);
void mgl_legend_xy_(long *graph, float *x, float *y, const char *font, float *size, float *llen,int);
void mgl_legend_(long *graph, int *where, const char *font, float *size, float *llen,int);
/*****************************************************************************/
/*		1D plotting functions												 */
/*****************************************************************************/
void mgl_fplot_(long *graph, const char *fy, const char *stl, float *zval, int *n, int ly, int ls);
void mgl_fplot_xyz_(long *graph, const char *fx, const char *fy, const char *fz, const char *stl, int *n, int lx, int ly, int lz, int ls);
void mgl_plot_xyz_(long *graph, long *x, long *y, long *z, const char *pen,int);
void mgl_plot_xy_(long *graph, long *x, long *y, const char *pen,int);
void mgl_plot_(long *graph, long *y,	const char *pen,int);
void mgl_plot_2_(long *graph, long *a, const char *pen,int);
void mgl_plot_3_(long *graph, long *a, const char *pen,int);
void mgl_area_xyz_(long *graph, long *x, long *y, long *z, const char *pen,int);
void mgl_area_xy_(long *graph, long *x, long *y, const char *pen,int);
void mgl_area_(long *graph, long *y, const char *pen,int);
void mgl_area_2_(long *graph, long *a, const char *pen,int);
void mgl_area_3_(long *graph, long *a, const char *pen,int);
void mgl_region_xy_(long *graph, long *x, long *y1, long *y2, const char *pen, int *inside, int);
void mgl_region_(long *graph, long *y1, long *y2, const char *pen, int *inside, int);
void mgl_mark_(long *graph, float *x,float *y,float *z,const char *mark,int);
void mgl_stem_xyz_(long *graph, long *x, long *y, long *z, const char *pen,int);
void mgl_stem_xy_(long *graph, long *x, long *y, const char *pen,int);
void mgl_stem_(long *graph, long *y,	const char *pen,int);
void mgl_stem_2_(long *graph, long *a, const char *pen,int);
void mgl_stem_3_(long *graph, long *a, const char *pen,int);
void mgl_step_xyz_(long *graph, long *x, long *y, long *z, const char *pen,int);
void mgl_step_xy_(long *graph, long *x, long *y, const char *pen,int);
void mgl_step_(long *graph, long *y,	const char *pen,int);
void mgl_step_2_(long *graph, long *a, const char *pen,int);
void mgl_step_3_(long *graph, long *a, const char *pen,int);
void mgl_bars_xyz_(long *graph, long *x, long *y, long *z, const char *pen,int);
void mgl_bars_xy_(long *graph, long *x, long *y, const char *pen,int);
void mgl_bars_(long *graph, long *y,	const char *pen,int);
void mgl_bars_2_(long *graph, long *a, const char *pen,int);
void mgl_bars_3_(long *graph, long *a, const char *pen,int);
void mgl_barh_yx_(long *graph, long *y, long *v, const char *pen,int);
void mgl_barh_(long *graph, long *v,	const char *pen,int);
/*****************************************************************************/
/*		Advanced 1D plotting functions												 */
/*****************************************************************************/
void mgl_torus_(long *graph, long *r, long *z, const char *pen,int);
void mgl_torus_2_(long *graph, long *a, const char *pen,int);
void mgl_text_xyz_(long *graph, long *x, long *y, long *z,const char *text, const char *font, float *size,int,int);
void mgl_text_xy_(long *graph, long *x, long *y, const char *text, const char *font, float *size,int,int);
void mgl_text_y_(long *graph, long *y, const char *text, const char *font, float *size,int,int);
void mgl_chart_(long *graph, long *a, const char *col,int);
void mgl_error_(long *graph, long *y, long *ey, const char *pen,int);
void mgl_error_xy_(long *graph, long *x, long *y, long *ey, const char *pen,int);
void mgl_error_exy_(long *graph, long *x, long *y, long *ex, long *ey, const char *pen,int);
void mgl_mark_xyz_(long *graph, long *x, long *y, long *z, long *r, const char *pen,int);
void mgl_mark_xy_(long *graph, long *x, long *y, long *r, const char *pen,int);
void mgl_mark_y_(long *graph, long *y, long *r, const char *pen,int);
void mgl_tube_xyzr_(long *graph, long *x, long *y, long *z, long *r, const char *pen,int);
void mgl_tube_xyr_(long *graph, long *x, long *y, long *r, const char *pen,int);
void mgl_tube_r_(long *graph, long *y, long *r, const char *pen,int);
void mgl_tube_xyz_(long *graph, long *x, long *y, long *z, float *r, const char *pen,int);
void mgl_tube_xy_(long *graph, long *x, long *y, float *r, const char *pen,int);
void mgl_tube_(long *graph, long *y, float *r, const char *pen,int);

void mgl_textmark_xyzr_(long *graph, long *x, long *y, long *z, long *r, const char *text, const char *fnt,int,int);
void mgl_textmark_xyr_(long *graph, long *x, long *y, long *r, const char *text, const char *fnt,int,int);
void mgl_textmark_yr_(long *graph, long *y, long *r, const char *text, const char *fnt,int,int);
void mgl_textmark_(long *graph, long *y, const char *text, const char *fnt,int,int);
/*****************************************************************************/
/*		2D plotting functions												 */
/*****************************************************************************/
void mgl_fsurf_(long *graph, const char *fz, const char *stl, int *n, int lz, int ls);
void mgl_fsurf_xyz_(long *graph, const char *fx, const char *fy, const char *fz, const char *stl, int *n, int lx, int ly, int lz, int ls);
void mgl_grid_xy_(long *graph, long *x, long *y, long *z, const char *stl,float *zVal,int);
void mgl_grid_(long *graph, long *a,const char *stl,float *zVal,int);
void mgl_mesh_xy_(long *graph, long *x, long *y, long *z, const char *sch,int);
void mgl_mesh_(long *graph, long *z, const char *sch,int);
void mgl_fall_xy_(long *graph, long *x, long *y, long *z, const char *sch,int);
void mgl_fall_(long *graph, long *z, const char *sch,int);
void mgl_belt_xy_(long *graph, long *x, long *y, long *z, const char *sch,int);
void mgl_belt_(long *graph, long *z, const char *sch,int);
void mgl_surf_xy_(long *graph, long *x, long *y, long *z, const char *sch,int);
void mgl_surf_(long *graph, long *z, const char *sch,int);
void mgl_dens_xy_(long *graph, long *x, long *y, long *z, const char *sch,float *zVal,int);
void mgl_dens_(long *graph, long *z, const char *sch,float *zVal,int);
void mgl_boxs_xy_(long *graph, long *x, long *y, long *z, const char *sch,float *zVal,int);
void mgl_boxs_(long *graph, long *z, const char *sch,float *zVal,int);
void mgl_tile_xy_(long *graph, long *x, long *y, long *z, const char *sch,int);
void mgl_tile_(long *graph, long *z, const char *sch,int);
void mgl_tile_rxy_(long *graph, long *x, long *y, long *z, long *r, const char *sch,int);
void mgl_tile_r_(long *graph, long *z, long *r, const char *sch,int);
void mgl_cont_xy_val_(long *graph, long *v, long *x, long *y, long *z, const char *sch, float *zVal,int);
void mgl_cont_val_(long *graph, long *v, long *z, const char *sch,float *zVal,int);
void mgl_cont_xy_(long *graph, long *x, long *y, long *z, const char *sch, int *Num, float *zVal,int);
void mgl_cont_(long *graph, long *z, const char *sch, int *Num, float *zVal,int);
void mgl_contf_xy_val_(long *graph, long *v, long *x, long *y, long *z, const char *sch, float *zVal,int);
void mgl_contf_val_(long *graph, long *v, long *z, const char *sch,float *zVal,int);
void mgl_contf_xy_(long *graph, long *x, long *y, long *z, const char *sch, int *Num, float *zVal,int);
void mgl_contf_(long *graph, long *z, const char *sch, int *Num, float *zVal,int);
void mgl_axial_xy_val_(long *graph, long *v, long *x, long *y, long *a, const char *sch,int);
void mgl_axial_val_(long *graph, long *v, long *a, const char *sch,int);
void mgl_axial_xy_(long *graph, long *x, long *y, long *a, const char *sch, int *Num,int);
void mgl_axial_(long *graph, long *a, const char *sch, int *Num,int);
/*****************************************************************************/
/*		Dual plotting functions												 */
/*****************************************************************************/
void mgl_surfc_xy_(long *graph, long *x, long *y, long *z, long *c, const char *sch,int);
void mgl_surfc_(long *graph, long *z, long *c, const char *sch,int);
void mgl_surfa_xy_(long *graph, long *x, long *y, long *z, long *c, const char *sch,int);
void mgl_surfa_(long *graph, long *z, long *c, const char *sch,int);
void mgl_stfa_xy_(long *graph, long *x, long *y, long *re, long *im, int *dn, const char *sch, float *zVal, int);
void mgl_stfa_(long *graph, long *re, long *im, int *dn, const char *sch, float *zVal, int);
void mgl_vect_xy_(long *graph, long *x, long *y, long *ax, long *ay, const char *sch,float *zVal,int);
void mgl_vect_2d_(long *graph, long *ax, long *ay, const char *sch,float *zVal,int);
void mgl_vectc_xy_(long *graph, long *x, long *y, long *ax, long *ay, const char *sch,float *zVal,int);
void mgl_vectc_2d_(long *graph, long *ax, long *ay, const char *sch,float *zVal,int);
void mgl_vect_xyz_(long *graph, long *x, long *y, long *z, long *ax, long *ay, long *az, const char *sch,int);
void mgl_vect_3d_(long *graph, long *ax, long *ay, long *az, const char *sch,int);
void mgl_vectc_xyz_(long *graph, long *x, long *y, long *z, long *ax, long *ay, long *az, const char *sch,int);
void mgl_vectc_3d_(long *graph, long *ax, long *ay, long *az, const char *sch,int);
void mgl_map_xy_(long *graph, long *x, long *y, long *a, long *b, const char *sch, int *ks, int *pnts,int);
void mgl_map_(long *graph, long *a, long *b, const char *sch, int *ks, int *pnts,int);
void mgl_surf3a_xyz_val_(long *graph, float *Val, long *x, long *y, long *z, long *a, long *b,
			const char *stl,int);
void mgl_surf3a_val_(long *graph, float *Val, long *a, long *b, const char *stl,int);
void mgl_surf3a_xyz_(long *graph, long *x, long *y, long *z, long *a, long *b,
			const char *stl, int *num,int);
void mgl_surf3a_(long *graph, long *a, long *b, const char *stl, int *num,int);
void mgl_surf3c_xyz_val_(long *graph, float *Val, long *x, long *y, long *z, long *a, long *b,
			const char *stl,int);
void mgl_surf3c_val_(long *graph, float *Val, long *a, long *b, const char *stl,int);
void mgl_surf3c_xyz_(long *graph, long *x, long *y, long *z, long *a, long *b,
			const char *stl, int *num,int);
void mgl_surf3c_(long *graph, long *a, long *b, const char *stl, int *num,int);
void mgl_flow_xy_(long *graph, long *x, long *y, long *ax, long *ay, const char *sch, int *num, int *central, float *zVal,int);
void mgl_flow_2d_(long *graph, long *ax, long *ay, const char *sch, int *num, int *central, float *zVal,int);
void mgl_flow_xyz_(long *graph, long *x, long *y, long *z, long *ax, long *ay, long *az, const char *sch, int *num, int *central,int);
void mgl_flow_3d_(long *graph, long *ax, long *ay, long *az, const char *sch, int *num, int *central,int);
void mgl_pipe_xy_(long *graph, long *x, long *y, long *ax, long *ay, const char *sch, float *r0, int *num, int *central, float *zVal,int);
void mgl_pipe_2d_(long *graph, long *ax, long *ay, const char *sch, float *r0, int *num, int *central, float *zVal,int);
void mgl_pipe_xyz_(long *graph, long *x, long *y, long *z, long *ax, long *ay, long *az, const char *sch, float *r0, int *num, int *central,int);
void mgl_pipe_3d_(long *graph, long *ax, long *ay, long *az, const char *sch, float *r0, int *num, int *central,int);
void mgl_dew_xy_(long *gr, long *x, long *y, long *ax, long *ay, const char *sch, float *zVal,int l);
void mgl_dew_2d_(long *gr, long *ax, long *ay, const char *sch, float *zVal,int l);
/*****************************************************************************/
/*		3D plotting functions												 */
/*****************************************************************************/
void mgl_grid3_xyz_(long *graph, long *x, long *y, long *z, long *a, const char *dir, int *sVal, const char *stl,int,int);
void mgl_grid3_(long *graph, long *a, const char *dir, int *sVal, const char *stl,int,int);
void mgl_grid3_all_xyz_(long *graph, long *x, long *y, long *z, long *a, const char *stl,int);
void mgl_grid3_all_(long *graph, long *a, const char *stl,int);
void mgl_dens3_xyz_(long *graph, long *x, long *y, long *z, long *a, const char *dir, int *sVal, const char *stl,int,int);
void mgl_dens3_(long *graph, long *a, const char *dir, int *sVal, const char *stl,int,int);
void mgl_dens3_all_xyz_(long *graph, long *x, long *y, long *z, long *a, const char *stl,int);
void mgl_dens3_all_(long *graph, long *a, const char *stl,int);
void mgl_surf3_xyz_val_(long *graph, float *Val, long *x, long *y, long *z, long *a, const char *stl,int);
void mgl_surf3_val_(long *graph, float *Val, long *a, const char *stl,int);
void mgl_surf3_xyz_(long *graph, long *x, long *y, long *z, long *a, const char *stl, int *num,int);
void mgl_surf3_(long *graph, long *a, const char *stl, int *num,int);
void mgl_cont3_xyz_val_(long *graph, long *v, long *x, long *y, long *z, long *a,
			const char *dir, int *sVal, const char *sch,int,int);
void mgl_cont3_val_(long *graph, long *v, long *a, const char *dir, int *sVal, const char *sch,int,int);
void mgl_cont3_xyz_(long *graph, long *x, long *y, long *z, long *a,
			const char *dir, int *sVal, const char *sch, int *Num,int,int);
void mgl_cont3_(long *graph, long *a, const char *dir, int *sVal, const char *sch, int *Num,int,int);
void mgl_cont_all_xyz_(long *graph, long *x, long *y, long *z, long *a,
			const char *sch, int *Num,int);
void mgl_cont_all_(long *graph, long *a, const char *sch, int *Num,int);
void mgl_cloudp_xyz_(long *graph, long *x, long *y, long *z, long *a,
			const char *stl, float *alpha,int);
void mgl_cloudp_(long *graph, long *a, const char *stl, float *alpha,int);
void mgl_cloudq_xyz_(long *graph, long *x, long *y, long *z, long *a,
			const char *stl, float *alpha,int);
void mgl_cloudq_(long *graph, long *a, const char *stl, float *alpha,int);
void mgl_contf3_xyz_val_(long *graph, long *v, long *x, long *y, long *z, long *a,
			const char *dir, int *sVal, const char *sch,int,int);
void mgl_contf3_val_(long *graph, long *v, long *a, const char *dir, int *sVal, const char *sch,int,int);
void mgl_contf3_xyz_(long *graph, long *x, long *y, long *z, long *a,
			const char *dir, int *sVal, const char *sch, int *Num,int,int);
void mgl_contf3_(long *graph, long *a, const char *dir, int *sVal, const char *sch, int *Num,int,int);
void mgl_contf_all_xyz_(long *graph, long *x, long *y, long *z, long *a,
			const char *sch, int *Num,int);
void mgl_contf_all_(long *graph, long *a, const char *sch, int *Num,int);
void mgl_beam_val_(long *gr, float *val, long *tr, long *g1, long *g2, long *a,
				float *r, const char *sch, int *norm,int l);
void mgl_beam_(long *gr, long *tr, long *g1, long *g2, long *a, float *r,
			const char *sch, int *norm, int *num,int l);
/*****************************************************************************/
/*		Triangular plotting functions											 */
/*****************************************************************************/
void mgl_triplot_xyz_(long *gr, long *nums, long *x, long *y, long *z, const char *sch,int);
void mgl_triplot_xy_(long *gr, long *nums, long *x, long *y, const char *sch, float *zVal,int);
void mgl_dots_(long *gr, long *x, long *y, long *z, const char *sch,int);
void mgl_dots_tr_(long *gr, long *tr, const char *sch,int);
void mgl_crust_(long *gr, long *x, long *y, long *z, const char *sch, float *er,int);
void mgl_crust_tr_(long *gr, long *tr, const char *sch, float *er,int);
/*****************************************************************************/
/*		Combined plotting functions											 */
/*****************************************************************************/
void mgl_dens_x_(long *graph, long *a, const char *stl, float *sVal,int);
void mgl_dens_y_(long *graph, long *a, const char *stl, float *sVal,int);
void mgl_dens_z_(long *graph, long *a, const char *stl, float *sVal,int);
void mgl_cont_x_(long *graph, long *a, const char *stl, float *sVal, int *Num,int);
void mgl_cont_y_(long *graph, long *a, const char *stl, float *sVal, int *Num,int);
void mgl_cont_z_(long *graph, long *a, const char *stl, float *sVal, int *Num,int);
void mgl_cont_x_val_(long *graph, long *v, long *a, const char *stl, float *sVal,int);
void mgl_cont_y_val_(long *graph, long *v, long *a, const char *stl, float *sVal,int);
void mgl_cont_z_val_(long *graph, long *v, long *a, const char *stl, float *sVal,int);
void mgl_contf_x_(long *graph, long *a, const char *stl, float *sVal, int *Num,int);
void mgl_contf_y_(long *graph, long *a, const char *stl, float *sVal, int *Num,int);
void mgl_contf_z_(long *graph, long *a, const char *stl, float *sVal, int *Num,int);
void mgl_contf_x_val_(long *graph, long *v, long *a, const char *stl, float *sVal,int);
void mgl_contf_y_val_(long *graph, long *v, long *a, const char *stl, float *sVal,int);
void mgl_contf_z_val_(long *graph, long *v, long *a, const char *stl, float *sVal,int);
/*****************************************************************************/
/*		Data creation functions												 */
/*****************************************************************************/
void mgl_data_rearrange_(long *dat, int *mx, int *my, int *mz);
void mgl_data_set_real_(long *dat, const float *A,int *NX,int *NY,int *NZ);
void mgl_data_set_double_(long *dat, const double *A,int *NX,int *NY,int *NZ);
void mgl_data_set_real1_(long *d, const float *A,int *N1);
void mgl_data_set_double1_(long *d, const double *A,int *N1);
void mgl_data_set_real2_(long *d, const float *A,int *N1,int *N2);
void mgl_data_set_double2_(long *d, const double *A,int *N1,int *N2);
void mgl_data_set_real3_(long *d, const float *A,int *N1,int *N2,int *N3);
void mgl_data_set_double3_(long *d, const double *A,int *N1,int *N2,int *N3);
void mgl_data_set_(long *dat, long *a);
float mgl_data_get_value_(long *d, int *i, int *j, int *k);
void mgl_data_set_value_(long *d, float *v, int *i, int *j, int *k);
void mgl_data_set_values_(long *d, const char *val, int *nx, int *ny, int *nz, int l);
int mgl_data_read_(long *d, const char *fname,int l);
int mgl_data_read_mat_(long *dat, const char *fname, int *dim, int);
int mgl_data_read_dim_(long *dat, const char *fname,int *mx,int *my,int *mz,int);
void mgl_data_save_(long *dat, const char *fname,int *ns,int);
void mgl_data_export_(long *dat, const char *fname, const char *scheme,float *v1,float *v2,int *ns,int,int);
void mgl_data_import_(long *dat, const char *fname, const char *scheme,float *v1,float *v2,int,int);
void mgl_data_create_(long *dat, int *nx,int *ny,int *nz);
void mgl_data_transpose_(long *dat, const char *dim,int);
void mgl_data_norm_(long *dat, float *v1,float *v2,int *sym,int *dim);
void mgl_data_norm_slice_(long *dat, float *v1,float *v2,char *dir,int *keep_en,int *sym,int l);
long mgl_data_subdata_(long *dat, int *xx,int *yy,int *zz);
long mgl_data_column_(long *dat, const char *eq,int l);
void mgl_data_set_id_(long *dat, const char *id,int l);
void mgl_data_fill_(long *dat, float *x1,float *x2,const char *dir,int);
void mgl_data_modify_(long *dat, const char *eq,int *dim,int);
void mgl_data_modify_vw_(long *dat, const char *eq, long *vdat, long *wdat,int);
void mgl_data_squeeze_(long *dat, int *rx,int *ry,int *rz,int *smooth);
float mgl_data_max_(long *dat);
float mgl_data_min_(long *dat);
const float *mgl_data_data_(long *dat);
long mgl_data_combine_(long *dat1, long *dat2);
void mgl_data_extend_(long *dat, int *n1, int *n2);
/*****************************************************************************/
/*		Data manipulation functions											 */
/*****************************************************************************/
void mgl_data_smooth_(long *dat, int *Type,float *delta,const char *dirs,int);
long mgl_data_sum_(long *dat, const char *dir,int);
long mgl_data_max_dir_(long *dat, const char *dir,int);
long mgl_data_min_dir_(long *dat, const char *dir,int);
void mgl_data_cumsum_(long *dat, const char *dir,int);
void mgl_data_integral_(long *dat, const char *dir,int);
void mgl_data_diff_(long *dat, const char *dir,int);
void mgl_data_diff2_(long *dat, const char *dir,int);
void mgl_data_swap_(long *dat, const char *dir,int);
void mgl_data_mirror_(long *dat, const char *dir,int);
float mgl_data_spline_(long *dat, float *x,float *y,float *z);
float mgl_data_spline1_(long *dat, float *x,float *y,float *z);
float mgl_data_linear_(long *dat, float *x,float *y,float *z);
float mgl_data_linear1_(long *dat, float *x,float *y,float *z);
long mgl_data_resize_(long *dat, int *mx,int *my,int *mz,float *x1,float *x2,float *y1,float *y2,float *z1,float *z2);
long mgl_data_hist_(long *dat, int *n, float *v1, float *v2, int *nsub);
long mgl_data_hist_w_(long *dat, long *weight, int *n, float *v1, float *v2, int *nsub);
void mgl_data_crop_(long *dat, int *n1, int *n2, const char *dir,int);
/*****************************************************************************/
/*		Data operations														 */
/*****************************************************************************/
void mgl_data_mul_dat_(long *dat, long *d);
void mgl_data_div_dat_(long *dat, long *d);
void mgl_data_add_dat_(long *dat, long *d);
void mgl_data_sub_dat_(long *dat, long *d);
void mgl_data_mul_num_(long *dat, float *d);
void mgl_data_div_num_(long *dat, float *d);
void mgl_data_add_num_(long *dat, float *d);
void mgl_data_sub_num_(long *dat, float *d);
/*****************************************************************************/
/*		Nonlinear fitting													 */
/*****************************************************************************/
float mgl_fit_1_(long* gr, long* fit, long* y, const char *eq, const char *var, float *ini, int *print, int l, int n);
float mgl_fit_2_(long* gr, long* fit, long* z, const char *eq, const char *var, float *ini, int *print, int l, int n);
float mgl_fit_3_(long* gr, long* fit, long* a, const char *eq, const char *var, float *ini, int *print, int l, int n);
float mgl_fit_xy_(long* gr, long* fit, long* x, long* y, const char *eq, const char *var, float *ini, int *print, int l, int n);
float mgl_fit_xyz_(long* gr, long* fit, long* x, long* y, long* z, const char *eq, const char *var, float *ini, int *print, int l, int n);
float mgl_fit_xyza_(long* gr, long* fit, long* x, long* y, long* z, long* a, const char *eq, const char *var, float *ini, int *print, int l, int n);
float mgl_fit_ys_(long* gr, long* fit, long* y, long* ss, const char *eq, const char *var, float *ini, int *print, int l, int n);
float mgl_fit_xys_(long* gr, long* fit, long* x, long* y, long* ss, const char *eq, const char *var, float *ini, int *print, int l, int n);
float mgl_fit_xyzs_(long* gr, long* fit, long* x, long* y, long* z, long* ss, const char *eq, const char *var, float *ini, int *print, int l, int n);
float mgl_fit_xyzas_(long* gr, long* fit, long* x, long* y, long* z, long* a, long* ss, const char *eq, const char *var, float *ini, int *print, int l, int n);
void mgl_puts_fit_(long* gr, float *x, float *y, float *z, const char *prefix, const char *font, float *size, int l, int n);
/*****************************************************************************/
long mgl_create_parser_();
void mgl_delete_parser_(long* p);
void mgl_add_param_(long* p, int *id, const char *str, int l);
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
long mgl_add_var_(long* p, const char *name, int l);
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
long mgl_find_var_(long* p, const char *name, int l);
int mgl_parse_(long* gr, long* p, const char *str, int *pos, int l);
void mgl_restore_once_(long* p);
void mgl_parser_allow_setsize_(long* p, int *a);
/*****************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* _mgl_f_h_ */
