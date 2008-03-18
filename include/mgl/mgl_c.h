/* mgl_c.h is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>
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
struct mglGraph;
struct mglData;
typedef mglGraph *HMGL;
typedef mglData *HMDT;
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
HMGL mgl_create_graph_glut(int argc, char **argv, int (*draw)(mglGraph *gr, void *p),
						const char *title, void (*reload)(bool next), void *par);
HMGL mgl_create_graph_fltk(int argc, char **argv, int (*draw)(mglGraph *gr, void *p),
						const char *title, void (*reload)(bool next), void *par);
void mgl_fltk_run();
void mgl_delete_graph(HMGL graph);
/*****************************************************************************/
HMDT mgl_create_data();
HMDT mgl_create_data_size(int nx, int ny, int nz);
HMDT mgl_create_data_file(const char *fname);
void mgl_delete_data(HMDT dat);
/*****************************************************************************/
/*		Setup mglGraph														 */
/*****************************************************************************/
void mgl_set_pal_color(HMGL graph, int n, float r, float g, float b);
void mgl_set_pal_num(HMGL graph, int num);
void mgl_set_rotated_text(HMGL graph, bool rotated);
void mgl_set_cut(HMGL graph, bool cut);
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
void mgl_set_draw_face(HMGL gr, bool enable);
void mgl_set_scheme(HMGL gr, char *sch);
void mgl_set_font(HMGL gr, char *name, char *path);
/*****************************************************************************/
/*		Export to file or to memory														 */
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
void mgl_set_transp(HMGL graph, bool enable);
void mgl_set_alpha(HMGL graph, bool enable);
void mgl_set_fog(HMGL graph, float d, float dz);
void mgl_set_light(HMGL graph, bool enable);
void mgl_add_light(HMGL graph, int n, float x, float y, float z, bool infty);
void mgl_add_light_rgb(HMGL graph, int n, float x, float y, float z, bool infty,
                        float r, float g, float b, float i);
void mgl_set_ambbr(HMGL gr, float i);
/*****************************************************************************/
/*		Scale and rotate													 */
/*****************************************************************************/
void mgl_identity(HMGL graph);
void mgl_clf(HMGL graph);
void mgl_clf_rgb(HMGL graph, float r, float g, float b);
void mgl_subplot(HMGL graph, int nx,int ny,int m);
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
void mgl_set_crange(HMGL graph, HMDT a, bool add);
void mgl_set_xrange(HMGL graph, HMDT a, bool add);
void mgl_set_yrange(HMGL graph, HMDT a, bool add);
void mgl_set_zrange(HMGL graph, HMDT a, bool add);
void mgl_set_func(HMGL graph, const char *EqX,const char *EqY,const char *EqZ);
void mgl_set_cutoff(HMGL graph, const char *EqC);
void mgl_box(HMGL graph, bool ticks);
void mgl_box_str(HMGL graph, const char *col, bool ticks);
void mgl_box_rgb(HMGL graph, float r, float g, float b, bool ticks);
void mgl_axis(HMGL graph, const char *dir);
void mgl_axis_grid(HMGL graph, const char *dir,const char *pen);
void mgl_label(HMGL graph, char dir, const char *text);
void mgl_label_ext(HMGL graph, char dir, const char *text, int pos, float size);
void mgl_tune_ticks(HMGL graph, bool tune, float fact_pos);
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
void mgl_putsw(HMGL graph, float x, float y, float z,const char *text);
void mgl_puts(HMGL graph, float x, float y, float z,const wchar_t *text);
void mgl_text(HMGL graph, float x, float y, float z,const char *text);
void mgl_puts_ext(HMGL graph, float x, float y, float z,const wchar_t *text,const char *font,float size,char dir);
void mgl_putsw_ext(HMGL graph, float x, float y, float z,const char *text,const char *font,float size,char dir);
void mgl_text_ext(HMGL graph, float x, float y, float z,const char *text,const char *font,float size,char dir);
void mgl_colorbar(HMGL graph, const char *sch,int where);
void mgl_simple_plot(HMGL graph, HMDT a, int type, const char *stl);
void mgl_add_legend(HMGL graph, const char *text,const char *style);
void mgl_add_legendw(HMGL graph, const wchar_t *text,const char *style);
void mgl_clear_legend(HMGL graph);
void mgl_legend_xy(HMGL graph, float x, float y, const char *font, float size);
void mgl_legend(HMGL graph, int where, const char *font, float size);
/*****************************************************************************/
/*		1D plotting functions												 */
/*****************************************************************************/
void mgl_plot_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, const char *pen);
void mgl_plot_xy(HMGL graph, HMDT x, HMDT y, const char *pen);
void mgl_plot(HMGL graph, HMDT y,	const char *pen);
void mgl_plot_2(HMGL graph, HMDT a, const char *pen);
void mgl_plot_3(HMGL graph, HMDT a, const char *pen);
void mgl_area_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, const char *pen);
void mgl_area_xy(HMGL graph, HMDT x, HMDT y, const char *pen);
void mgl_area(HMGL graph, HMDT y, const char *pen);
void mgl_area_2(HMGL graph, HMDT a, const char *pen);
void mgl_area_3(HMGL graph, HMDT a, const char *pen);
void mgl_mark(HMGL graph, float x,float y,float z,char mark);
void mgl_stem_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, const char *pen);
void mgl_stem_xy(HMGL graph, HMDT x, HMDT y, const char *pen);
void mgl_stem(HMGL graph, HMDT y,	const char *pen);
void mgl_stem_2(HMGL graph, HMDT a, const char *pen);
void mgl_stem_3(HMGL graph, HMDT a, const char *pen);
void mgl_step_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, const char *pen);
void mgl_step_xy(HMGL graph, HMDT x, HMDT y, const char *pen);
void mgl_step(HMGL graph, HMDT y,	const char *pen);
void mgl_step_2(HMGL graph, HMDT a, const char *pen);
void mgl_step_3(HMGL graph, HMDT a, const char *pen);
void mgl_bars_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, const char *pen);
void mgl_bars_xy(HMGL graph, HMDT x, HMDT y, const char *pen);
void mgl_bars(HMGL graph, HMDT y,	const char *pen);
void mgl_bars_2(HMGL graph, HMDT a, const char *pen);
void mgl_bars_3(HMGL graph, HMDT a, const char *pen);
/*****************************************************************************/
/*		Advanced 1D plotting functions												 */
/*****************************************************************************/
void mgl_torus(HMGL graph, HMDT r, HMDT z, const char *pen);
void mgl_torus_2(HMGL graph, HMDT a, const char *pen);
void mgl_text_xyz(HMGL graph, HMDT x, HMDT y, HMDT z,const char *text, const char *font, float size);
void mgl_text_xy(HMGL graph, HMDT x, HMDT y, const char *text, const char *font, float size);
void mgl_text_y(HMGL graph, HMDT y, const char *text, const char *font, float size);
void mgl_chart(HMGL graph, HMDT a, const char *col);
void mgl_error(HMGL graph, HMDT y, HMDT ey, const char *pen);
void mgl_error_xy(HMGL graph, HMDT x, HMDT y, HMDT ey, const char *pen);
void mgl_error_exy(HMGL graph, HMDT x, HMDT y, HMDT ex, HMDT ey, const char *pen);
void mgl_mark_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT r, const char *pen);
void mgl_mark_xy(HMGL graph, HMDT x, HMDT y, HMDT r, const char *pen);
void mgl_mark_y(HMGL graph, HMDT y, HMDT r, const char *pen);
void mgl_tube_xyzr(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT r, const char *pen);
void mgl_tube_xyr(HMGL graph, HMDT x, HMDT y, HMDT r, const char *pen);
void mgl_tube_r(HMGL graph, HMDT y, HMDT r, const char *pen);
void mgl_tube_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, float r, const char *pen);
void mgl_tube_xy(HMGL graph, HMDT x, HMDT y, float r, const char *penl);
void mgl_tube(HMGL graph, HMDT y, float r, const char *pen);
/*****************************************************************************/
/*		2D plotting functions												 */
/*****************************************************************************/
void mgl_grid_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *stl,float zVal);
void mgl_grid(HMGL graph, HMDT a,const char *stl,float zVal);
void mgl_mesh_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *sch);
void mgl_mesh(HMGL graph, HMDT z, const char *sch);
void mgl_fall_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *sch);
void mgl_fall(HMGL graph, HMDT z, const char *sch);
void mgl_belt_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *sch);
void mgl_belt(HMGL graph, HMDT z, const char *sch);
void mgl_surf_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *sch);
void mgl_surf(HMGL graph, HMDT z, const char *sch);
void mgl_dens_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *sch,float zVal);
void mgl_dens(HMGL graph, HMDT z, const char *sch,float zVal);
void mgl_boxs_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *sch,float zVal);
void mgl_boxs(HMGL graph, HMDT z, const char *sch,float zVal);
void mgl_tile_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *sch);
void mgl_tile(HMGL graph, HMDT z, const char *sch);
void mgl_cont_xy_val(HMGL graph, HMDT v, HMDT x, HMDT y, HMDT z, const char *sch, float zVal);
void mgl_cont_val(HMGL graph, HMDT v, HMDT z, const char *sch,float zVal);
void mgl_cont_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *sch, int Num, float zVal);
void mgl_cont(HMGL graph, HMDT z, const char *sch, int Num, float zVal);
void mgl_contf_xy_val(HMGL graph, HMDT v, HMDT x, HMDT y, HMDT z, const char *sch, float zVal);
void mgl_contf_val(HMGL graph, HMDT v, HMDT z, const char *sch,float zVal);
void mgl_contf_xy(HMGL graph, HMDT x, HMDT y, HMDT z, const char *sch, int Num, float zVal);
void mgl_contf(HMGL graph, HMDT z, const char *sch, int Num, float zVal);
void mgl_axial_xy_val(HMGL graph, HMDT v, HMDT x, HMDT y, HMDT a, const char *sch);
void mgl_axial_val(HMGL graph, HMDT v, HMDT a, const char *sch);
void mgl_axial_xy(HMGL graph, HMDT x, HMDT y, HMDT a, const char *sch, int Num);
void mgl_axial(HMGL graph, HMDT a, const char *sch, int Num);
/*****************************************************************************/
/*		Dual plotting functions												 */
/*****************************************************************************/
void mgl_surfc_xy(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT c, const char *sch);
void mgl_surfc(HMGL graph, HMDT z, HMDT c, const char *sch);
void mgl_surfa_xy(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT c, const char *sch);
void mgl_surfa(HMGL graph, HMDT z, HMDT c, const char *sch);
void mgl_vect_xy(HMGL graph, HMDT x, HMDT y, HMDT ax, HMDT ay, const char *sch,float zVal);
void mgl_vect_2d(HMGL graph, HMDT ax, HMDT ay, const char *sch,float zVal);
void mgl_vectc_xy(HMGL graph, HMDT x, HMDT y, HMDT ax, HMDT ay, const char *sch,float zVal);
void mgl_vectc_2d(HMGL graph, HMDT ax, HMDT ay, const char *sch,float zVal);
void mgl_vect_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT ax, HMDT ay, HMDT az, const char *sch);
void mgl_vect_3d(HMGL graph, HMDT ax, HMDT ay, HMDT az, const char *sch);
void mgl_vectc_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT ax, HMDT ay, HMDT az, const char *sch);
void mgl_vectc_3d(HMGL graph, HMDT ax, HMDT ay, HMDT az, const char *sch);
void mgl_map_xy(HMGL graph, HMDT x, HMDT y, HMDT a, HMDT b, const char *sch, int ks, bool pnts);
void mgl_map(HMGL graph, HMDT a, HMDT b, const char *sch, int ks, bool pnts);
void mgl_surf3a_xyz_val(HMGL graph, float Val, HMDT x, HMDT y, HMDT z, HMDT a, HMDT b,
			const char *stl);
void mgl_surf3a_val(HMGL graph, float Val, HMDT a, HMDT b, const char *stl);
void mgl_surf3a_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a, HMDT b,
			const char *stl, int num);
void mgl_surf3a(HMGL graph, HMDT a, HMDT b, const char *stl, int num);
void mgl_surf3c_xyz_val(HMGL graph, float Val, HMDT x, HMDT y, HMDT z, HMDT a, HMDT b,
			const char *stl);
void mgl_surf3c_val(HMGL graph, float Val, HMDT a, HMDT b, const char *stl);
void mgl_surf3c_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a, HMDT b,
			const char *stl, int num);
void mgl_surf3c(HMGL graph, HMDT a, HMDT b, const char *stl, int num);
void mgl_flow_xy(HMGL graph, HMDT x, HMDT y, HMDT ax, HMDT ay, const char *sch, int num, bool central, float zVal);
void mgl_flow_2d(HMGL graph, HMDT ax, HMDT ay, const char *sch, int num, bool central, float zVal);
void mgl_flow_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT ax, HMDT ay, HMDT az, const char *sch, int num, bool central);
void mgl_flow_3d(HMGL graph, HMDT ax, HMDT ay, HMDT az, const char *sch, int num, bool central);
void mgl_pipe_xy(HMGL graph, HMDT x, HMDT y, HMDT ax, HMDT ay, const char *sch, float r0, int num, bool central, float zVal);
void mgl_pipe_2d(HMGL graph, HMDT ax, HMDT ay, const char *sch, float r0, int num, bool central, float zVal);
void mgl_pipe_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT ax, HMDT ay, HMDT az, const char *sch, float r0, int num, bool central);
void mgl_pipe_3d(HMGL graph, HMDT ax, HMDT ay, HMDT az, const char *sch, float r0, int num, bool central);
/*****************************************************************************/
/*		3D plotting functions												 */
/*****************************************************************************/
void mgl_grid3_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a, char dir, int sVal, const char *stl);
void mgl_grid3(HMGL graph, HMDT a, char dir, int sVal, const char *stl);
void mgl_grid3_all_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a, const char *stl);
void mgl_grid3_all(HMGL graph, HMDT a, const char *stl);
void mgl_dens3_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a, char dir, int sVal, const char *stl);
void mgl_dens3(HMGL graph, HMDT a, char dir, int sVal, const char *stl);
void mgl_dens3_all_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a, const char *stl);
void mgl_dens3_all(HMGL graph, HMDT a, const char *stl);
void mgl_surf3_xyz_val(HMGL graph, float Val, HMDT x, HMDT y, HMDT z, HMDT a, const char *stl);
void mgl_surf3_val(HMGL graph, float Val, HMDT a, const char *stl);
void mgl_surf3_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a, const char *stl, int num);
void mgl_surf3(HMGL graph, HMDT a, const char *stl, int num);
void mgl_cont3_xyz_val(HMGL graph, HMDT v, HMDT x, HMDT y, HMDT z, HMDT a,
			char dir, int sVal, const char *sch);
void mgl_cont3_val(HMGL graph, HMDT v, HMDT a, char dir, int sVal, const char *sch);
void mgl_cont3_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a,
			char dir, int sVal, const char *sch, int Num);
void mgl_cont3(HMGL graph, HMDT a, char dir, int sVal, const char *sch, int Num);
void mgl_cont_all_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a,
			const char *sch, int Num);
void mgl_cont_all(HMGL graph, HMDT a, const char *sch, int Num);
void mgl_cloudp_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a,
			const char *stl, float alpha);
void mgl_cloudp(HMGL graph, HMDT a, const char *stl, float alpha);
void mgl_cloudq_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a,
			const char *stl, float alpha);
void mgl_cloudq(HMGL graph, HMDT a, const char *stl, float alpha);
void mgl_contf3_xyz_val(HMGL graph, HMDT v, HMDT x, HMDT y, HMDT z, HMDT a,
			char dir, int sVal, const char *sch);
void mgl_contf3_val(HMGL graph, HMDT v, HMDT a, char dir, int sVal, const char *sch);
void mgl_contf3_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a,
			char dir, int sVal, const char *sch, int Num);
void mgl_contf3(HMGL graph, HMDT a, char dir, int sVal, const char *sch, int Num);
void mgl_contf_all_xyz(HMGL graph, HMDT x, HMDT y, HMDT z, HMDT a,
			const char *sch, int Num);
void mgl_contf_all(HMGL graph, HMDT a, const char *sch, int Num);
void mgl_beam_val(HMGL graph, float Val, HMDT tr, HMDT g1, HMDT g2, HMDT a,
		float r, const char *stl, int norm);
void mgl_beam(HMGL graph, HMDT tr, HMDT g1, HMDT g2, HMDT a, float r,
		const char *stl, int norm, int num);
/*****************************************************************************/
/*		Triangular plotting functions											 */
/*****************************************************************************/
void mgl_triplot_xyz(HMGL gr, HMDT nums, HMDT x, HMDT y, HMDT z, const char *sch);
void mgl_triplot_xy(HMGL gr, HMDT nums, HMDT x, HMDT y, const char *sch, float zVal);
void mgl_dots(HMGL gr, HMDT x, HMDT y, HMDT z, const char *sch);
void mgl_dots_tr(HMGL gr, HMDT tr, const char *sch);
void mgl_crust(HMGL gr, HMDT x, HMDT y, HMDT z, const char *sch, float er);
void mgl_crust_tr(HMGL gr, HMDT tr, const char *sch, float er);
/*****************************************************************************/
/*		Combined plotting functions											 */
/*****************************************************************************/
void mgl_dens_x(HMGL graph, HMDT a, const char *stl, float sVal);
void mgl_dens_y(HMGL graph, HMDT a, const char *stl, float sVal);
void mgl_dens_z(HMGL graph, HMDT a, const char *stl, float sVal);
void mgl_cont_x(HMGL graph, HMDT a, const char *stl, float sVal, int Num);
void mgl_cont_y(HMGL graph, HMDT a, const char *stl, float sVal, int Num);
void mgl_cont_z(HMGL graph, HMDT a, const char *stl, float sVal, int Num);
void mgl_cont_x_val(HMGL graph, HMDT v, HMDT a, const char *stl, float sVal);
void mgl_cont_y_val(HMGL graph, HMDT v, HMDT a, const char *stl, float sVal);
void mgl_cont_z_val(HMGL graph, HMDT v, HMDT a, const char *stl, float sVal);
void mgl_contf_x(HMGL graph, HMDT a, const char *stl, float sVal, int Num);
void mgl_contf_y(HMGL graph, HMDT a, const char *stl, float sVal, int Num);
void mgl_contf_z(HMGL graph, HMDT a, const char *stl, float sVal, int Num);
void mgl_contf_x_val(HMGL graph, HMDT v, HMDT a, const char *stl, float sVal);
void mgl_contf_y_val(HMGL graph, HMDT v, HMDT a, const char *stl, float sVal);
void mgl_contf_z_val(HMGL graph, HMDT v, HMDT a, const char *stl, float sVal);
/*****************************************************************************/
/*		Data creation functions												 */
/*****************************************************************************/
void mgl_data_set_float(HMDT dat, const float *A,int NX,int NY,int NZ);
void mgl_data_set_double(HMDT dat, const double *A,int NX,int NY,int NZ);
void mgl_data_set_float2(HMDT d, const float **A,int N1,int N2);
void mgl_data_set_double2(HMDT d, const double **A,int N1,int N2);
void mgl_data_set_float3(HMDT d, const float ***A,int N1,int N2,int N3);
void mgl_data_set_double3(HMDT d, const double ***A,int N1,int N2,int N3);
void mgl_data_set(HMDT dat, HMDT a);
void mgl_data_set_vector(HMDT dat, gsl_vector *v);
void mgl_data_set_matrix(HMDT dat, gsl_matrix *m);

float mgl_data_get_value(HMDT dat, int i, int j, int k);
void mgl_data_set_value(HMDT dat, int i, int j, int k, float v);
bool mgl_data_read(HMDT dat, const char *fname);
bool mgl_data_read_mat(HMDT dat, const char *fname, int dim);
bool mgl_data_read_dim(HMDT dat, const char *fname,int mx,int my,int mz);
void mgl_data_save(HMDT dat, const char *fname,int ns);
void mgl_data_export(HMDT dat, const char *fname, const char *scheme,float v1,float v2,int ns);
void mgl_data_import(HMDT dat, const char *fname, const char *scheme,float v1,float v2);
void mgl_data_create(HMDT dat, int nx,int ny,int nz);
void mgl_data_transpose(HMDT dat, const char *dim);
void mgl_data_norm(HMDT dat, float v1,float v2,bool sym,int dim);
HMDT mgl_data_subdata(HMDT dat, int xx,int yy,int zz);
void mgl_data_fill(HMDT dat, float x1,float x2,char dir);
void mgl_data_modify(HMDT dat, const char *eq,int dim);
void mgl_data_modify_vw(HMDT dat, const char *eq,HMDT vdat,HMDT wdat);
void mgl_data_squeeze(HMDT dat, int rx,int ry,int rz,bool smooth);
float mgl_data_max(HMDT dat);
float mgl_data_min(HMDT dat);
void mgl_data_insert(HMDT dat, const char *eq);
void mgl_data_pullout(HMDT dat);
float &mgl_data_value(HMDT dat, int i,int j,int k);
const float *mgl_data_data(HMDT dat);
HMDT mgl_data_combine(HMDT dat1, HMDT dat2);
void mgl_data_extend(HMDT dat, int n1, int n2);
/*****************************************************************************/
/*		Data manipulation functions											 */
/*****************************************************************************/
void mgl_data_smooth(HMDT dat, int Type,float delta,const char *dirs);
HMDT mgl_data_sum(HMDT dat, const char *dir);
HMDT mgl_data_max_dir(HMDT dat, const char *dir);
HMDT mgl_data_min_dir(HMDT dat, const char *dir);
void mgl_data_cumsum(HMDT dat, const char *dir);
void mgl_data_integral(HMDT dat, const char *dir);
void mgl_data_diff(HMDT dat, const char *dir);
void mgl_data_diff2(HMDT dat, const char *dir);
void mgl_data_swap(HMDT dat, const char *dir);
float mgl_data_spline(HMDT dat, float x,float y,float z);
float mgl_data_spline1(HMDT dat, float x,float y,float z);
float mgl_data_linear(HMDT dat, float x,float y,float z);
float mgl_data_linear1(HMDT dat, float x,float y,float z);
HMDT mgl_data_resize(HMDT dat, int mx,int my,int mz,float x1,float x2,
	float y1,float y2,float z1,float z2);
HMDT mgl_data_hist(HMDT dat, int n, float v1, float v2, int nsub);
HMDT mgl_data_hist_w(HMDT dat, HMDT weight, int n, float v1, float v2, int nsub);
void mgl_data_crop(HMDT dat, int n1, int n2, char dir);
/*****************************************************************************/
/*		Data operations														 */
/*****************************************************************************/
void mgl_data_mul_dat(HMDT dat, HMDT d);
void mgl_data_div_dat(HMDT dat, HMDT d);
void mgl_data_add_dat(HMDT dat, HMDT d);
void mgl_data_sub_dat(HMDT dat, HMDT d);
void mgl_data_mul_num(HMDT dat, float d);
void mgl_data_div_num(HMDT dat, float d);
void mgl_data_add_num(HMDT dat, float d);
void mgl_data_sub_num(HMDT dat, float d);
/*****************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* _mgl_c_h_ */
