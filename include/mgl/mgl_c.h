/* mgl_c.h is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <mathgl.abalakin@gmail.com>
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
#ifndef _MGL_C_H_
#define _MGL_C_H_

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
#ifdef __cplusplus
struct mglGraph;
typedef mglGraph *HMGL;
struct mglData;
typedef mglData *HMDT;
struct mglParse;
typedef mglParse *HMPR;
#else
#define HMGL void*
#define HMDT void*
#define HMPR void*
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
HMGL mgl_create_graph_glut(int argc, char **argv, int (*draw)(HMGL gr, void *p),
						const char *title, void (*reload)(int next), void *par);
HMGL mgl_create_graph_fltk(int argc, char **argv, int (*draw)(HMGL gr, void *p),
						const char *title, void (*reload)(int next), void *par);
HMGL mgl_create_graph_qt(int argc, char **argv, int (*draw)(HMGL gr, void *p),
						const char *title, void (*reload)(int next), void *par);
HMGL mgl_create_graph_idtf();
void mgl_fltk_run();
void mgl_qt_run();
void mgl_update(HMGL graph);
void mgl_delete_graph(HMGL graph);
/*****************************************************************************/
HMDT mgl_create_data();
HMDT mgl_create_data_size(int nx, int ny, int nz);
HMDT mgl_create_data_file(const char *fname);
void mgl_delete_data(HMDT dat);
/*****************************************************************************/
HMPR mgl_create_parser();
void mgl_delete_parser(HMPR p);
void mgl_add_param(HMPR p, int id, const char *str);
void mgl_add_paramw(HMPR p, int id, const wchar_t *str);
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
const HMDT mgl_add_var(HMPR, const char *name);
/*===!!! NOTE !!! You must not delete obtained data arrays !!!===============*/
const HMDT mgl_find_var(HMPR, const char *name);
int mgl_parse(HMGL gr, HMPR p, const char *str, int pos);
int mgl_parsew(HMGL gr, HMPR p, const wchar_t *str, int pos);
void mgl_restore_once(HMPR p);
void mgl_parser_allow_setsize(HMPR p, int a);
/*****************************************************************************/
/*		Setup mglGraph														 */
/*****************************************************************************/
void mgl_set_palette(HMGL gr, const char *colors);
void mgl_set_pal_color(HMGL graph, int n, float r, float g, float b);
void mgl_set_pal_num(HMGL graph, int num);
void mgl_set_rotated_text(HMGL graph, int rotated);
void mgl_set_cut(HMGL graph, int cut);
void mgl_set_cut_box(HMGL gr, float x1,float y1,float z1,float x2,float y2,float z2);
void mgl_set_bar_width(HMGL graph, float width);
void mgl_set_base_line_width(HMGL gr, float size);
void mgl_set_mark_size(HMGL graph, float size);
void mgl_set_arrow_size(HMGL graph, float size);
void mgl_set_font_size(HMGL graph, float size);
void mgl_set_alpha_default(HMGL graph, float alpha);
void mgl_set_size(HMGL graph, int width, int height);
void mgl_set_axial_dir(HMGL graph, char dir);
void mgl_set_meshnum(HMGL graph, int num);
void mgl_set_zoom(HMGL gr, float x1, float y1, float x2, float y2);
void mgl_set_plotfactor(HMGL gr, float val);
void mgl_set_draw_face(HMGL gr, int enable);
void mgl_set_scheme(HMGL gr, const char *sch);
void mgl_set_font(HMGL gr, const char *name, const char *path);
void mgl_copy_font(HMGL gr, HMGL gr_from);
void mgl_restore_font(HMGL gr);
/*****************************************************************************/
/*		Export to file or to memory											 */
/*****************************************************************************/
void mgl_write_tif(HMGL graph, const char *fname,const char *descr);
void mgl_write_jpg(HMGL graph, const char *fname,const char *descr);
void mgl_write_png(HMGL graph, const char *fname,const char *descr);
void mgl_write_png_solid(HMGL graph, const char *fname,const char *descr);
void mgl_write_eps(HMGL graph, const char *fname,const char *descr);
void mgl_write_svg(HMGL graph, const char *fname,const char *descr);
const unsigned char *mgl_get_rgb(HMGL graph);
const unsigned char *mgl_get_rgba(HMGL graph);
int mgl_get_width(HMGL graph);
int mgl_get_height(HMGL graph);
/*****************************************************************************/
/*		Setup frames transparency (alpha) and lightning						 */
/*****************************************************************************/
int mgl_new_frame(HMGL graph, int id);
void mgl_end_frame(HMGL graph);
int mgl_get_num_frame(HMGL graph);
void mgl_set_transp_type(HMGL graph, int type);
void mgl_set_transp(HMGL graph, int enable);
void mgl_set_alpha(HMGL graph, int enable);
void mgl_set_fog(HMGL graph, float d, float dz);
void mgl_set_light(HMGL graph, int enable);
void mgl_add_light(HMGL graph, int n, float x, float y, float z, int infty);
void mgl_add_light_rgb(HMGL graph, int n, float x, float y, float z, int infty,
                        float r, float g, float b, float i);
void mgl_set_ambbr(HMGL gr, float i);
/*****************************************************************************/
/*		Scale and rotate													 */
/*****************************************************************************/
void mgl_identity(HMGL graph);
void mgl_clf(HMGL graph);
void mgl_clf_rgb(HMGL graph, float r, float g, float b);
void mgl_subplot(HMGL graph, int nx,int ny,int m);
void mgl_subplot_d(HMGL graph, int nx,int ny,int m, float dx, float dy);
void mgl_inplot(HMGL graph, float x1,float x2,float y1,float y2);
void mgl_aspect(HMGL graph, float Ax,float Ay,float Az);
void mgl_rotate(HMGL graph, float TetX,float TetZ,float TetY);
void mgl_rotate_vector(HMGL graph, float Tet,float x,float y,float z);
void mgl_perspective(HMGL graph, float val);
/*****************************************************************************/
/*		Axis functions														 */
/*****************************************************************************/
void mgl_set_ticks(HMGL graph, float DX, float DY, float DZ);
void mgl_set_subticks(HMGL graph, int NX, int NY, int NZ);
void mgl_set_caxis(HMGL graph, float C1,float C2);
void mgl_set_axis(HMGL graph, float x1, float y1, float z1, float x2, float y2, float z2, float x0, float y0, float z0);
void mgl_set_axis_3d(HMGL graph, float x1, float y1, float z1, float x2, float y2, float z2);
void mgl_set_axis_2d(HMGL graph, float x1, float y1, float x2, float y2);
void mgl_set_origin(HMGL graph, float x0, float y0, float z0);
void mgl_set_tick_origin(HMGL graph, float x0, float y0, float z0);
void mgl_set_crange(HMGL graph, const HMDT a, int add);
void mgl_set_xrange(HMGL graph, const HMDT a, int add);
void mgl_set_yrange(HMGL graph, const HMDT a, int add);
void mgl_set_zrange(HMGL graph, const HMDT a, int add);
void mgl_set_func(HMGL graph, const char *EqX,const char *EqY,const char *EqZ);
void mgl_set_cutoff(HMGL graph, const char *EqC);
void mgl_box(HMGL graph, int ticks);
void mgl_box_str(HMGL graph, const char *col, int ticks);
void mgl_box_rgb(HMGL graph, float r, float g, float b, int ticks);
void mgl_axis(HMGL graph, const char *dir);
void mgl_axis_grid(HMGL graph, const char *dir,const char *pen);
void mgl_label(HMGL graph, char dir, const char *text);
void mgl_label_ext(HMGL graph, char dir, const char *text, int pos, float size, float shift);
void mgl_tune_ticks(HMGL graph, int tune, float fact_pos);
void mgl_set_xtt(HMGL graph, const wchar_t *templ);
void mgl_set_ytt(HMGL graph, const wchar_t *templ);
void mgl_set_ztt(HMGL graph, const wchar_t *templ);
void mgl_set_ctt(HMGL graph, const wchar_t *templ);
/*****************************************************************************/
/*		Simple drawing														 */
/*****************************************************************************/
void mgl_ball(HMGL graph, float x,float y,float z);
void mgl_ball_rgb(HMGL graph, float x, float y, float z, float r, float g, float b, float alpha);
void mgl_ball_str(HMGL graph, float x, float y, float z, const char *col);
void mgl_line(HMGL graph, float x1, float y1, float z1, float x2, float y2, float z2, const char *pen,int n);
void mgl_facex(HMGL graph, float x0, float y0, float z0, float wy, float wz, const char *stl, float dx, float dy);
void mgl_facey(HMGL graph, float x0, float y0, float z0, float wx, float wz, const char *stl, float dx, float dy);
void mgl_facez(HMGL graph, float x0, float y0, float z0, float wx, float wy, const char *stl, float dx, float dy);
void mgl_curve(HMGL graph, float x1, float y1, float z1, float dx1, float dy1, float dz1, float x2, float y2, float z2, float dx2, float dy2, float dz2, const char *pen,int n);

void mgl_puts(HMGL graph, float x, float y, float z,const char *text);
void mgl_putsw(HMGL graph, float x, float y, float z,const wchar_t *text);
void mgl_puts_dir(HMGL graph, float x, float y, float z, float dx, float dy, float dz, const char *text, float size);
void mgl_putsw_dir(HMGL graph, float x, float y, float z, float dx, float dy, float dz, const wchar_t *text, float size);
void mgl_text(HMGL graph, float x, float y, float z,const char *text);
void mgl_title(HMGL graph, const char *text, const char *fnt);
void mgl_putsw_ext(HMGL graph, float x, float y, float z,const wchar_t *text,const char *font,float size,char dir);
void mgl_puts_ext(HMGL graph, float x, float y, float z,const char *text,const char *font,float size,char dir);
void mgl_text_ext(HMGL graph, float x, float y, float z,const char *text,const char *font,float size,char dir);
void mgl_colorbar(HMGL graph, const char *sch,int where);
void mgl_simple_plot(HMGL graph, const HMDT a, int type, const char *stl);
void mgl_add_legend(HMGL graph, const char *text,const char *style);
void mgl_add_legendw(HMGL graph, const wchar_t *text,const char *style);
void mgl_clear_legend(HMGL graph);
void mgl_legend_xy(HMGL graph, float x, float y, const char *font, float size, float llen);
void mgl_legend(HMGL graph, int where, const char *font, float size, float llen);
/*****************************************************************************/
/*		1D plotting functions												 */
/*****************************************************************************/
void mgl_fplot(HMGL graph, const char *fy, const char *stl, int n);
void mgl_fplot_xyz(HMGL graph, const char *fx, const char *fy, const char *fz, const char *stl, int n);
void mgl_plot_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *pen);
void mgl_plot_xy(HMGL graph, const HMDT x, const HMDT y, const char *pen);
void mgl_plot(HMGL graph, const HMDT y,	const char *pen);
void mgl_plot_2(HMGL graph, const HMDT a, const char *pen);
void mgl_plot_3(HMGL graph, const HMDT a, const char *pen);
void mgl_area_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *pen);
void mgl_area_xy(HMGL graph, const HMDT x, const HMDT y, const char *pen);
void mgl_area(HMGL graph, const HMDT y, const char *pen);
void mgl_area_2(HMGL graph, const HMDT a, const char *pen);
void mgl_area_3(HMGL graph, const HMDT a, const char *pen);
void mgl_region_xy(HMGL graph, const HMDT x, const HMDT y1, const HMDT y2, const char *pen, int inside);
void mgl_region(HMGL graph, const HMDT y1, const HMDT y2, const char *pen, int inside);
void mgl_mark(HMGL graph, float x,float y,float z,char mark);
void mgl_stem_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *pen);
void mgl_stem_xy(HMGL graph, const HMDT x, const HMDT y, const char *pen);
void mgl_stem(HMGL graph, const HMDT y,	const char *pen);
void mgl_stem_2(HMGL graph, const HMDT a, const char *pen);
void mgl_stem_3(HMGL graph, const HMDT a, const char *pen);
void mgl_step_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *pen);
void mgl_step_xy(HMGL graph, const HMDT x, const HMDT y, const char *pen);
void mgl_step(HMGL graph, const HMDT y,	const char *pen);
void mgl_step_2(HMGL graph, const HMDT a, const char *pen);
void mgl_step_3(HMGL graph, const HMDT a, const char *pen);
void mgl_bars_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *pen);
void mgl_bars_xy(HMGL graph, const HMDT x, const HMDT y, const char *pen);
void mgl_bars(HMGL graph, const HMDT y,	const char *pen);
void mgl_bars_2(HMGL graph, const HMDT a, const char *pen);
void mgl_bars_3(HMGL graph, const HMDT a, const char *pen);
void mgl_barh_yx(HMGL graph, const HMDT y, const HMDT v, const char *pen);
void mgl_barh(HMGL graph, const HMDT v,	const char *pen);
/*****************************************************************************/
/*		Advanced 1D plotting functions												 */
/*****************************************************************************/
void mgl_torus(HMGL graph, const HMDT r, const HMDT z, const char *pen);
void mgl_torus_2(HMGL graph, const HMDT a, const char *pen);
void mgl_text_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z,const char *text, const char *font, float size);
void mgl_text_xy(HMGL graph, const HMDT x, const HMDT y, const char *text, const char *font, float size);
void mgl_text_y(HMGL graph, const HMDT y, const char *text, const char *font, float size);
void mgl_chart(HMGL graph, const HMDT a, const char *col);
void mgl_error(HMGL graph, const HMDT y, const HMDT ey, const char *pen);
void mgl_error_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT ey, const char *pen);
void mgl_error_exy(HMGL graph, const HMDT x, const HMDT y, const HMDT ex, const HMDT ey, const char *pen);
void mgl_mark_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT r, const char *pen);
void mgl_mark_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT r, const char *pen);
void mgl_mark_y(HMGL graph, const HMDT y, const HMDT r, const char *pen);
void mgl_tube_xyzr(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT r, const char *pen);
void mgl_tube_xyr(HMGL graph, const HMDT x, const HMDT y, const HMDT r, const char *pen);
void mgl_tube_r(HMGL graph, const HMDT y, const HMDT r, const char *pen);
void mgl_tube_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, float r, const char *pen);
void mgl_tube_xy(HMGL graph, const HMDT x, const HMDT y, float r, const char *penl);
void mgl_tube(HMGL graph, const HMDT y, float r, const char *pen);

void mgl_textmark_xyzr(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT r, const char *text, const char *fnt);
void mgl_textmark_xyr(HMGL graph, const HMDT x, const HMDT y, const HMDT r, const char *text, const char *fnt);
void mgl_textmark_yr(HMGL graph, const HMDT y, const HMDT r, const char *text, const char *fnt);
void mgl_textmark(HMGL graph, const HMDT y, const char *text, const char *fnt);
void mgl_textmarkw_xyzr(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT r, const wchar_t *text, const char *fnt);
void mgl_textmarkw_xyr(HMGL graph, const HMDT x, const HMDT y, const HMDT r, const wchar_t *text, const char *fnt);
void mgl_textmarkw_yr(HMGL graph, const HMDT y, const HMDT r, const wchar_t *text, const char *fnt);
void mgl_textmarkw(HMGL graph, const HMDT y, const wchar_t *text, const char *fnt);
/*****************************************************************************/
/*		2D plotting functions												 */
/*****************************************************************************/
void mgl_fsurf(HMGL graph, const char *fz, const char *stl, int n);
void mgl_fsurf_xyz(HMGL graph, const char *fx, const char *fy, const char *fz, const char *stl, int n);
void mgl_grid_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *stl,float zVal);
void mgl_grid(HMGL graph, const HMDT a,const char *stl,float zVal);
void mgl_mesh_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *sch);
void mgl_mesh(HMGL graph, const HMDT z, const char *sch);
void mgl_fall_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *sch);
void mgl_fall(HMGL graph, const HMDT z, const char *sch);
void mgl_belt_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *sch);
void mgl_belt(HMGL graph, const HMDT z, const char *sch);
void mgl_surf_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *sch);
void mgl_surf(HMGL graph, const HMDT z, const char *sch);
void mgl_dens_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *sch,float zVal);
void mgl_dens(HMGL graph, const HMDT z, const char *sch,float zVal);
void mgl_boxs_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *sch,float zVal);
void mgl_boxs(HMGL graph, const HMDT z, const char *sch,float zVal);
void mgl_tile_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *sch);
void mgl_tile(HMGL graph, const HMDT z, const char *sch);
void mgl_tile_rxy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT r, const char *sch);
void mgl_tile_r(HMGL graph, const HMDT z, const HMDT r, const char *sch);
void mgl_cont_xy_val(HMGL graph, const HMDT v, const HMDT x, const HMDT y, const HMDT z, const char *sch, float zVal);
void mgl_cont_val(HMGL graph, const HMDT v, const HMDT z, const char *sch,float zVal);
void mgl_cont_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *sch, int Num, float zVal);
void mgl_cont(HMGL graph, const HMDT z, const char *sch, int Num, float zVal);
void mgl_contf_xy_val(HMGL graph, const HMDT v, const HMDT x, const HMDT y, const HMDT z, const char *sch, float zVal);
void mgl_contf_val(HMGL graph, const HMDT v, const HMDT z, const char *sch,float zVal);
void mgl_contf_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const char *sch, int Num, float zVal);
void mgl_contf(HMGL graph, const HMDT z, const char *sch, int Num, float zVal);
void mgl_axial_xy_val(HMGL graph, const HMDT v, const HMDT x, const HMDT y, const HMDT a, const char *sch);
void mgl_axial_val(HMGL graph, const HMDT v, const HMDT a, const char *sch);
void mgl_axial_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT a, const char *sch, int Num);
void mgl_axial(HMGL graph, const HMDT a, const char *sch, int Num);
/*****************************************************************************/
/*		Dual plotting functions												 */
/*****************************************************************************/
void mgl_surfc_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT c, const char *sch);
void mgl_surfc(HMGL graph, const HMDT z, const HMDT c, const char *sch);
void mgl_surfa_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT c, const char *sch);
void mgl_surfa(HMGL graph, const HMDT z, const HMDT c, const char *sch);
void mgl_stfa_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT re, const HMDT im, int dn, const char *sch, float zVal);
void mgl_stfa(HMGL graph, const HMDT re, const HMDT im, int dn, const char *sch, float zVal);
void mgl_vect_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT ax, const HMDT ay, const char *sch,float zVal);
void mgl_vect_2d(HMGL graph, const HMDT ax, const HMDT ay, const char *sch,float zVal);
void mgl_vectc_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT ax, const HMDT ay, const char *sch,float zVal);
void mgl_vectc_2d(HMGL graph, const HMDT ax, const HMDT ay, const char *sch,float zVal);
void mgl_vect_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT ax, const HMDT ay, const HMDT az, const char *sch);
void mgl_vect_3d(HMGL graph, const HMDT ax, const HMDT ay, const HMDT az, const char *sch);
void mgl_vectc_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT ax, const HMDT ay, const HMDT az, const char *sch);
void mgl_vectc_3d(HMGL graph, const HMDT ax, const HMDT ay, const HMDT az, const char *sch);
void mgl_map_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT a, const HMDT b, const char *sch, int ks, int pnts);
void mgl_map(HMGL graph, const HMDT a, const HMDT b, const char *sch, int ks, int pnts);
void mgl_surf3a_xyz_val(HMGL graph, float Val, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const HMDT b, const char *stl);
void mgl_surf3a_val(HMGL graph, float Val, const HMDT a, const HMDT b, const char *stl);
void mgl_surf3a_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const HMDT b, const char *stl, int num);
void mgl_surf3a(HMGL graph, const HMDT a, const HMDT b, const char *stl, int num);
void mgl_surf3c_xyz_val(HMGL graph, float Val, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const HMDT b, const char *stl);
void mgl_surf3c_val(HMGL graph, float Val, const HMDT a, const HMDT b, const char *stl);
void mgl_surf3c_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const HMDT b,
			const char *stl, int num);
void mgl_surf3c(HMGL graph, const HMDT a, const HMDT b, const char *stl, int num);
void mgl_flow_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT ax, const HMDT ay, const char *sch, int num, int central, float zVal);
void mgl_flow_2d(HMGL graph, const HMDT ax, const HMDT ay, const char *sch, int num, int central, float zVal);
void mgl_flow_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT ax, const HMDT ay, const HMDT az, const char *sch, int num, int central);
void mgl_flow_3d(HMGL graph, const HMDT ax, const HMDT ay, const HMDT az, const char *sch, int num, int central);
void mgl_pipe_xy(HMGL graph, const HMDT x, const HMDT y, const HMDT ax, const HMDT ay, const char *sch, float r0, int num, int central, float zVal);
void mgl_pipe_2d(HMGL graph, const HMDT ax, const HMDT ay, const char *sch, float r0, int num, int central, float zVal);
void mgl_pipe_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT ax, const HMDT ay, const HMDT az, const char *sch, float r0, int num, int central);
void mgl_pipe_3d(HMGL graph, const HMDT ax, const HMDT ay, const HMDT az, const char *sch, float r0, int num, int central);
void mgl_dew_xy(HMGL gr, const HMDT x, const HMDT y, const HMDT ax, const HMDT ay, const char *sch,float zVal);
void mgl_dew_2d(HMGL gr, const HMDT ax, const HMDT ay, const char *sch,float zVal);
/*****************************************************************************/
/*		3D plotting functions												 */
/*****************************************************************************/
void mgl_grid3_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, char dir, int sVal, const char *stl);
void mgl_grid3(HMGL graph, const HMDT a, char dir, int sVal, const char *stl);
void mgl_grid3_all_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const char *stl);
void mgl_grid3_all(HMGL graph, const HMDT a, const char *stl);
void mgl_dens3_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, char dir, int sVal, const char *stl);
void mgl_dens3(HMGL graph, const HMDT a, char dir, int sVal, const char *stl);
void mgl_dens3_all_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const char *stl);
void mgl_dens3_all(HMGL graph, const HMDT a, const char *stl);
void mgl_surf3_xyz_val(HMGL graph, float Val, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const char *stl);
void mgl_surf3_val(HMGL graph, float Val, const HMDT a, const char *stl);
void mgl_surf3_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const char *stl, int num);
void mgl_surf3(HMGL graph, const HMDT a, const char *stl, int num);
void mgl_cont3_xyz_val(HMGL graph, const HMDT v, const HMDT x, const HMDT y, const HMDT z, const HMDT a,
			char dir, int sVal, const char *sch);
void mgl_cont3_val(HMGL graph, const HMDT v, const HMDT a, char dir, int sVal, const char *sch);
void mgl_cont3_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a,
			char dir, int sVal, const char *sch, int Num);
void mgl_cont3(HMGL graph, const HMDT a, char dir, int sVal, const char *sch, int Num);
void mgl_cont_all_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a,
			const char *sch, int Num);
void mgl_cont_all(HMGL graph, const HMDT a, const char *sch, int Num);
void mgl_cloudp_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a,
			const char *stl, float alpha);
void mgl_cloudp(HMGL graph, const HMDT a, const char *stl, float alpha);
void mgl_cloudq_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a,
			const char *stl, float alpha);
void mgl_cloudq(HMGL graph, const HMDT a, const char *stl, float alpha);
void mgl_contf3_xyz_val(HMGL graph, const HMDT v, const HMDT x, const HMDT y, const HMDT z, const HMDT a,
			char dir, int sVal, const char *sch);
void mgl_contf3_val(HMGL graph, const HMDT v, const HMDT a, char dir, int sVal, const char *sch);
void mgl_contf3_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a,
			char dir, int sVal, const char *sch, int Num);
void mgl_contf3(HMGL graph, const HMDT a, char dir, int sVal, const char *sch, int Num);
void mgl_contf_all_xyz(HMGL graph, const HMDT x, const HMDT y, const HMDT z, const HMDT a,
			const char *sch, int Num);
void mgl_contf_all(HMGL graph, const HMDT a, const char *sch, int Num);
void mgl_beam_val(HMGL graph, float Val, const HMDT tr, const HMDT g1, const HMDT g2, const HMDT a,
		float r, const char *stl, int norm);
void mgl_beam(HMGL graph, const HMDT tr, const HMDT g1, const HMDT g2, const HMDT a, float r,
		const char *stl, int norm, int num);
/*****************************************************************************/
/*		Triangular plotting functions											 */
/*****************************************************************************/
void mgl_triplot_xyz(HMGL gr, const HMDT nums, const HMDT x, const HMDT y, const HMDT z, const char *sch);
void mgl_triplot_xy(HMGL gr, const HMDT nums, const HMDT x, const HMDT y, const char *sch, float zVal);
void mgl_dots(HMGL gr, const HMDT x, const HMDT y, const HMDT z, const char *sch);
void mgl_dots_tr(HMGL gr, const HMDT tr, const char *sch);
void mgl_crust(HMGL gr, const HMDT x, const HMDT y, const HMDT z, const char *sch, float er);
void mgl_crust_tr(HMGL gr, const HMDT tr, const char *sch, float er);
/*****************************************************************************/
/*		Combined plotting functions											 */
/*****************************************************************************/
void mgl_dens_x(HMGL graph, const HMDT a, const char *stl, float sVal);
void mgl_dens_y(HMGL graph, const HMDT a, const char *stl, float sVal);
void mgl_dens_z(HMGL graph, const HMDT a, const char *stl, float sVal);
void mgl_cont_x(HMGL graph, const HMDT a, const char *stl, float sVal, int Num);
void mgl_cont_y(HMGL graph, const HMDT a, const char *stl, float sVal, int Num);
void mgl_cont_z(HMGL graph, const HMDT a, const char *stl, float sVal, int Num);
void mgl_cont_x_val(HMGL graph, const HMDT v, const HMDT a, const char *stl, float sVal);
void mgl_cont_y_val(HMGL graph, const HMDT v, const HMDT a, const char *stl, float sVal);
void mgl_cont_z_val(HMGL graph, const HMDT v, const HMDT a, const char *stl, float sVal);
void mgl_contf_x(HMGL graph, const HMDT a, const char *stl, float sVal, int Num);
void mgl_contf_y(HMGL graph, const HMDT a, const char *stl, float sVal, int Num);
void mgl_contf_z(HMGL graph, const HMDT a, const char *stl, float sVal, int Num);
void mgl_contf_x_val(HMGL graph, const HMDT v, const HMDT a, const char *stl, float sVal);
void mgl_contf_y_val(HMGL graph, const HMDT v, const HMDT a, const char *stl, float sVal);
void mgl_contf_z_val(HMGL graph, const HMDT v, const HMDT a, const char *stl, float sVal);
/*****************************************************************************/
/*		Data creation functions												 */
/*****************************************************************************/
void mgl_data_rearrange(HMDT dat, int mx, int my, int mz);
void mgl_data_set_float(HMDT dat, const float *A,int NX,int NY,int NZ);
void mgl_data_set_double(HMDT dat, const double *A,int NX,int NY,int NZ);
void mgl_data_set_float2(HMDT d, const float **A,int N1,int N2);
void mgl_data_set_double2(HMDT d, const double **A,int N1,int N2);
void mgl_data_set_float3(HMDT d, const float ***A,int N1,int N2,int N3);
void mgl_data_set_double3(HMDT d, const double ***A,int N1,int N2,int N3);
void mgl_data_set(HMDT dat, const HMDT a);
void mgl_data_set_vector(HMDT dat, gsl_vector *v);
void mgl_data_set_matrix(HMDT dat, gsl_matrix *m);

float mgl_data_get_value(const HMDT dat, int i, int j, int k);
void mgl_data_set_value(HMDT dat, float v, int i, int j, int k);
void mgl_data_set_values(HMDT dat, const char *val, int nx, int ny, int nz);
int mgl_data_read(HMDT dat, const char *fname);
int mgl_data_read_mat(HMDT dat, const char *fname, int dim);
int mgl_data_read_dim(HMDT dat, const char *fname,int mx,int my,int mz);
void mgl_data_save(HMDT dat, const char *fname,int ns);
void mgl_data_export(HMDT dat, const char *fname, const char *scheme,float v1,float v2,int ns);
void mgl_data_import(HMDT dat, const char *fname, const char *scheme,float v1,float v2);
void mgl_data_create(HMDT dat, int nx,int ny,int nz);
void mgl_data_transpose(HMDT dat, const char *dim);
void mgl_data_norm(HMDT dat, float v1,float v2,int sym,int dim);
void mgl_data_norm_slice(HMDT dat, float v1,float v2,char dir,int keep_en,int sym);
HMDT mgl_data_subdata(const HMDT dat, int xx,int yy,int zz);
HMDT mgl_data_column(const HMDT dat, const char *eq);
void mgl_data_set_id(HMDT d, const char *id);
void mgl_data_fill(HMDT dat, float x1,float x2,char dir);
void mgl_data_modify(HMDT dat, const char *eq,int dim);
void mgl_data_modify_vw(HMDT dat, const char *eq,const HMDT vdat,const HMDT wdat);
void mgl_data_squeeze(HMDT dat, int rx,int ry,int rz,int smooth);
float mgl_data_max(HMDT dat);
float mgl_data_min(HMDT dat);
float *mgl_data_value(HMDT dat, int i,int j,int k);
const float *mgl_data_data(HMDT dat);
HMDT mgl_data_combine(const HMDT dat1, const HMDT dat2);
void mgl_data_extend(HMDT dat, int n1, int n2);
/*****************************************************************************/
/*		Data manipulation functions											 */
/*****************************************************************************/
void mgl_data_smooth(HMDT dat, int Type,float delta,const char *dirs);
HMDT mgl_data_sum(const HMDT dat, const char *dir);
HMDT mgl_data_max_dir(const HMDT dat, const char *dir);
HMDT mgl_data_min_dir(const HMDT dat, const char *dir);
void mgl_data_cumsum(HMDT dat, const char *dir);
void mgl_data_integral(HMDT dat, const char *dir);
void mgl_data_diff(HMDT dat, const char *dir);
void mgl_data_diff2(HMDT dat, const char *dir);
void mgl_data_swap(HMDT dat, const char *dir);
void mgl_data_mirror(HMDT dat, const char *dir);
float mgl_data_spline(HMDT dat, float x,float y,float z);
float mgl_data_spline1(HMDT dat, float x,float y,float z);
float mgl_data_linear(HMDT dat, float x,float y,float z);
float mgl_data_linear1(HMDT dat, float x,float y,float z);
HMDT mgl_data_resize(const HMDT dat, int mx,int my,int mz,float x1,float x2,
	float y1,float y2,float z1,float z2);
HMDT mgl_data_hist(const HMDT dat, int n, float v1, float v2, int nsub);
HMDT mgl_data_hist_w(const HMDT dat, const HMDT weight, int n, float v1, float v2, int nsub);
HMDT mgl_data_evaluate_i(const HMDT dat, const HMDT idat, int norm);
HMDT mgl_data_evaluate_ij(const HMDT dat, const HMDT idat, const HMDT jdat, int norm);
HMDT mgl_data_evaluate_ijk(const HMDT dat, const HMDT idat, const HMDT jdat, const HMDT kdat, int norm);
void mgl_data_envelop(HMDT dat, char dir);
void mgl_data_sew(HMDT dat, const char *dirs, float da);
void mgl_data_crop(HMDT dat, int n1, int n2, char dir);
/*****************************************************************************/
/*		Data operations														 */
/*****************************************************************************/
void mgl_data_mul_dat(HMDT dat, const HMDT d);
void mgl_data_div_dat(HMDT dat, const HMDT d);
void mgl_data_add_dat(HMDT dat, const HMDT d);
void mgl_data_sub_dat(HMDT dat, const HMDT d);
void mgl_data_mul_num(HMDT dat, float d);
void mgl_data_div_num(HMDT dat, float d);
void mgl_data_add_num(HMDT dat, float d);
void mgl_data_sub_num(HMDT dat, float d);
/*****************************************************************************/
/*		Nonlinear fitting													 */
/*****************************************************************************/
float mgl_fit_1(HMGL gr, HMDT fit, const HMDT y, const char *eq, const char *var, float *ini, int print);
float mgl_fit_2(HMGL gr, HMDT fit, const HMDT z, const char *eq, const char *var, float *ini, int print);
float mgl_fit_3(HMGL gr, HMDT fit, const HMDT a, const char *eq, const char *var, float *ini, int print);
float mgl_fit_xy(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const char *eq, const char *var, float *ini, int print);
float mgl_fit_xyz(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT z, const char *eq, const char *var, float *ini, int print);
float mgl_fit_xyza(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const char *eq, const char *var, float *ini, int print);
float mgl_fit_ys(HMGL gr, HMDT fit, const HMDT y, const HMDT s, const char *eq, const char *var, float *ini, int print);
float mgl_fit_xys(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT s, const char *eq, const char *var, float *ini, int print);
float mgl_fit_xyzs(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT z, const HMDT s, const char *eq, const char *var, float *ini, int print);
float mgl_fit_xyzas(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const HMDT s, const char *eq, const char *var, float *ini, int print);

float mgl_fit_1_d(HMGL gr, HMDT fit, const HMDT y, const char *eq, const char *var, HMDT ini, int print);
float mgl_fit_2_d(HMGL gr, HMDT fit, const HMDT z, const char *eq, const char *var, HMDT ini, int print);
float mgl_fit_3_d(HMGL gr, HMDT fit, const HMDT a, const char *eq, const char *var, HMDT ini, int print);
float mgl_fit_xy_d(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const char *eq, const char *var, HMDT ini, int print);
float mgl_fit_xyz_d(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT z, const char *eq, const char *var, HMDT ini, int print);
float mgl_fit_xyza_d(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const char *eq, const char *var, HMDT ini, int print);
float mgl_fit_ys_d(HMGL gr, HMDT fit, const HMDT y, const HMDT s, const char *eq, const char *var, HMDT ini, int print);
float mgl_fit_xys_d(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT s, const char *eq, const char *var, HMDT ini, int print);
float mgl_fit_xyzs_d(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT z, const HMDT s, const char *eq, const char *var, HMDT ini, int print);
float mgl_fit_xyzas_d(HMGL gr, HMDT fit, const HMDT x, const HMDT y, const HMDT z, const HMDT a, const HMDT s, const char *eq, const char *var, HMDT ini, int print);

void mgl_puts_fit(HMGL gr, float x, float y, float z, const char *prefix, const char *font, float size);
/*****************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* _mgl_c_h_ */
