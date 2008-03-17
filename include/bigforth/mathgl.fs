\ Mathgl library interface (mathgl.sf.net)
\ Copyright (C) 2007, Sergey Plis
\
\ This program is free software; you can redistribute it and/or modify
\ it under the terms of the GNU General Public License as published by
\ the Free Software Foundation; either version 2 of the License, or
\ (at your option) any later version.
\
\ This program is distributed in the hope that it will be useful,
\ but WITHOUT ANY WARRANTY; without even the implied warranty of
\ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
\ GNU General Public License for more details.

Module mathgl
also dos

library libmgl libmgl.so
library libmgl-glut libmgl-glut.so
library libmgl-fltk libmgl-fltk.so

legacy off
\ --------- Создание и удаление графического объекта
libmgl mgl_create_graph_gl              (ptr) mgl_create_graph_gl
libmgl mgl_create_graph_zb      int int (ptr) mgl_create_graph_zb
libmgl mgl_create_graph_ps      int int (ptr) mgl_create_graph_ps
libmgl-glut mgl_create_graph_glut [ 6 ] ints (ptr) mgl_create_graph_glut
libmgl-fltk mgl_create_graph_fltk [ 6 ] ints (ptr) mgl_create_graph_fltk
libmgl-fltk mgl_fltk_run                    (void) mgl_fltk_run
libmgl mgl_delete_graph            ptr (void) mgl_delete_graph
\ --------- Создание и удаление массива данных
libmgl mgl_create_data                  (ptr) mgl_create_data
libmgl mgl_create_data_size int int int (ptr) mgl_create_data_size
libmgl mgl_create_data_file         ptr (ptr) mgl_create_data_file
libmgl mgl_delete_data             ptr (void) mgl_delete_data
\ --------- Настройка графики
\ --------- Setup mglGraph
libmgl mgl_set_pal_num            ptr int (void) mgl_set_pal_num
libmgl mgl_set_pal_color sf sf sf int ptr (void) mgl_set_pal_color
libmgl mgl_set_rotated_text       int ptr (void) mgl_set_rotated_text
libmgl mgl_set_cut                int ptr (void) mgl_set_cut
libmgl mgl_set_cut_box sf sf sf sf sf sf ptr (void) mgl_set_cut_box
libmgl mgl_set_bar_width           sf ptr (void) mgl_set_bar_width
libmgl mgl_set_base_line_width     sf ptr (void) mgl_set_base_line_width
libmgl mgl_set_mark_size           sf ptr (void) mgl_set_mark_size
libmgl mgl_set_arrow_size          sf ptr (void) mgl_set_arrow_size
libmgl mgl_set_font_size           sf ptr (void) mgl_set_font_size
libmgl mgl_set_alpha_default       sf ptr (void) mgl_set_alpha_default
libmgl mgl_set_size           int int ptr (void) mgl_set_size
libmgl mgl_set_axial_dir          int ptr (void) mgl_set_axial_dir
libmgl mgl_set_meshnum            int ptr (void) mgl_set_meshnum
libmgl mgl_set_zoom       sf sf sf sf ptr (void) mgl_set_zoom
libmgl mgl_set_draw_face          int ptr (void) mgl_set_draw_face
\ --------- Экспорт графики в файл
\ --------- Export to file or to memory
libmgl mgl_write_tif ptr ptr ptr (void) mgl_write_tif
libmgl mgl_write_jpg ptr ptr ptr (void) mgl_write_jpg
libmgl mgl_write_png ptr ptr ptr (void) mgl_write_png
libmgl mgl_write_png_solid ptr ptr ptr (void) mgl_write_png_solid
libmgl mgl_write_eps ptr ptr ptr (void) mgl_write_eps
libmgl mgl_write_svg ptr ptr ptr (void) mgl_write_svg
libmgl mgl_get_rgb            ptr (ptr) mgl_get_rgb
libmgl mgl_get_rgba           ptr (ptr) mgl_get_rgba
libmgl mgl_get_width          ptr (int) mgl_get_width
libmgl mgl_get_height         ptr (int) mgl_get_height
\ --------- Кадры, прозрачность и освещение
\ --------- Setup frames transparency (alpha) and lightning
libmgl mgl_new_frame        int ptr (int) mgl_new_frame
libmgl mgl_end_frame           ptr (void) mgl_end_frame
libmgl mgl_get_num_frame       ptr (void) mgl_get_num_frame
libmgl mgl_set_transp_type int ptr (void) mgl_set_transp_type
libmgl mgl_set_transp      int ptr (void) mgl_set_transp
libmgl mgl_set_alpha       int ptr (void) mgl_set_alpha
libmgl mgl_set_fog       sf sf ptr (void) mgl_set_fog
libmgl mgl_set_light       int ptr (void) mgl_set_light
libmgl mgl_add_light int sf sf sf int ptr (void) mgl_add_light
libmgl mgl_add_light_rgb sf sf sf sf int sf sf sf int ptr (void) mgl_add_light_rgb
libmgl mgl_set_ambbr sf ptr (void) mgl_set_ambbr
\ --------- Масштабирование и вращение графиков
\ --------- Scale and rotate
libmgl mgl_identity                  ptr (void) mgl_identity
libmgl mgl_clf                       ptr (void) mgl_clf
libmgl mgl_clf_rgb          sf sf sf ptr (void) mgl_clf_rgb
libmgl mgl_subplot       int int int ptr (void) mgl_subplot
libmgl mgl_inplot        sf sf sf sf ptr (void) mgl_inplot
libmgl mgl_aspect           sf sf sf ptr (void) mgl_aspect
libmgl mgl_rotate           sf sf sf ptr (void) mgl_rotate
libmgl mgl_rotate_vector sf sf sf sf ptr (void) mgl_rotate_vector
\ --------- Функции осей координат
\              Axis functions
libmgl mgl_set_ticks      sf sf sf ptr  (void) mgl_set_ticks
libmgl mgl_set_subticks int int int ptr (void) mgl_set_subticks
libmgl mgl_set_caxis          sf sf ptr (void) mgl_set_caxis
libmgl mgl_set_axis int int int sf sf sf sf sf sf ptr (void) mgl_set_axis
libmgl mgl_set_axis_3d  sf sf sf sf sf sf ptr (void) mgl_set_axis_3d
libmgl mgl_set_axis_2d  sf sf sf sf ptr (void) mgl_set_axis_2d
libmgl mgl_set_origin   sf sf sf ptr (void) mgl_set_origin
libmgl mgl_set_crange    int ptr ptr (void) mgl_set_crange
libmgl mgl_set_xrange    int ptr ptr (void) mgl_set_xrange
libmgl mgl_set_yrange    int ptr ptr (void) mgl_set_yrange
libmgl mgl_set_zrange    int ptr ptr (void) mgl_set_zrange
libmgl mgl_set_func  ptr ptr ptr ptr (void) mgl_set_func
libmgl mgl_set_cutoff        int ptr (void) mgl_set_cutoff
libmgl mgl_box               int ptr (void) mgl_box
libmgl mgl_box_str       int ptr ptr (void) mgl_box_str
libmgl mgl_box_rgb int sf sf  sf ptr (void) mgl_box_rgb
libmgl mgl_axis              ptr ptr (void) mgl_axis
libmgl mgl_axis_grid     ptr ptr ptr (void) mgl_axis_grid
libmgl mgl_label         ptr int ptr (void) mgl_label
libmgl mgl_label_ext   sf [ 4 ] ints (void) mgl_label_ext
libmgl mgl_tune_ticks     sf int ptr (void) mgl_tune_ticks
\ --------- Простейшее рисование
\              Simple drawing
libmgl mgl_ball sf sf sf ptr (void) mgl_ball
libmgl mgl_ball_rgb sf sf sf sf sf sf sf ptr (void) mgl_ball_rgb
libmgl mgl_ball_str ptr sf sf sf ptr (void) mgl_ball_str
libmgl mgl_line ptr sf sf sf sf sf sf ptr (void) mgl_line
libmgl mgl_puts ptr sf sf sf ptr (void) mgl_puts
libmgl mgl_text ptr sf sf sf ptr (void) mgl_text
libmgl mgl_puts_ext ptr sf ptr ptr sf sf sf ptr (void) mgl_puts_ext
libmgl mgl_text_ext ptr sf ptr ptr sf sf sf ptr (void) mgl_text_ext
libmgl mgl_colorbar int ptr ptr (void) mgl_colorbar
libmgl mgl_simple_plot ptr int ptr ptr (void) mgl_simple_plot
libmgl mgl_add_legend ptr ptr ptr (void) mgl_add_legend
libmgl mgl_clear_legend ptr (void) mgl_clear_legend
libmgl mgl_legend_xy sf ptr sf sf ptr (void) mgl_legend_xy
libmgl mgl_legend sf ptr int ptr (void) mgl_legend
\ --------- 1D графики
\              1D plotting functions
libmgl mgl_plot_xyz ptr ptr ptr ptr ptr (void) mgl_plot_xyz
libmgl mgl_plot_xy  ptr ptr ptr ptr (void) mgl_plot_xy
libmgl mgl_plot     ptr ptr ptr (void) mgl_plot
libmgl mgl_plot_2   ptr ptr ptr (void) mgl_plot_2
libmgl mgl_plot_3   ptr ptr ptr (void) mgl_plot_3
libmgl mgl_area_xyz ptr ptr ptr ptr ptr (void) mgl_area_xyz
libmgl mgl_area_xy  ptr ptr ptr ptr (void) mgl_area_xy
libmgl mgl_area     ptr ptr ptr (void) mgl_area
libmgl mgl_area_2   ptr ptr ptr (void) mgl_area_2
libmgl mgl_area_3   ptr ptr ptr (void) mgl_area_3
libmgl mgl_mark     ptr df df df int (void) mgl_mark
libmgl mgl_stem_xyz ptr ptr ptr ptr ptr (void) mgl_stem_xyz
libmgl mgl_stem_xy  ptr ptr ptr ptr (void) mgl_stem_xy
libmgl mgl_stem     ptr ptr ptr (void) mgl_stem
libmgl mgl_stem_2   ptr ptr ptr (void) mgl_stem_2
libmgl mgl_stem_3   ptr ptr ptr (void) mgl_stem_3
libmgl mgl_step_xyz ptr ptr ptr ptr ptr (void) mgl_step_xyz
libmgl mgl_step_xy  ptr ptr ptr ptr (void) mgl_step_xy
libmgl mgl_step     ptr ptr ptr (void) mgl_step
libmgl mgl_step_2   ptr ptr ptr (void) mgl_step_2
libmgl mgl_step_3   ptr ptr ptr (void) mgl_step_3
libmgl mgl_bars_xyz ptr ptr ptr ptr ptr (void) mgl_bars_xyz
libmgl mgl_bars_xy  ptr ptr ptr ptr (void) mgl_bars_xy
libmgl mgl_bars     ptr ptr ptr (void) mgl_bars
libmgl mgl_bars_2   ptr ptr ptr (void) mgl_bars_2
libmgl mgl_bars_3   ptr ptr ptr (void) mgl_bars_3
\ --------- усложненные 1D графики 
\               Advanced 1D plotting functions
libmgl mgl_torus      ptr ptr ptr ptr (void) mgl_torus
libmgl mgl_torus_2        ptr ptr ptr (void) mgl_torus_2
libmgl mgl_text_xyz     sf [ 6 ] ints (void) mgl_text_xyz
libmgl mgl_text_xy      sf [ 5 ] ints (void) mgl_text_xy
libmgl mgl_text_y       sf [ 4 ] ints (void) mgl_text_y
libmgl mgl_chart          ptr ptr ptr (void) mgl_chart
libmgl mgl_error           [ 4 ] ints (void) mgl_error
libmgl mgl_error_xy        [ 5 ] ints (void) mgl_error_xy
libmgl mgl_error_exy       [ 6 ] ints (void) mgl_error_exy
libmgl mgl_mark_xyz        [ 6 ] ints (void) mgl_mark_xyz
libmgl mgl_mark_xy         [ 5 ] ints (void) mgl_mark_xy
libmgl mgl_mark_y          [ 4 ] ints (void) mgl_mark_y
libmgl mgl_tube_xyr        [ 6 ] ints (void) mgl_tube_xyr
libmgl mgl_tube_r          [ 4 ] ints (void) mgl_tube_r
libmgl mgl_tube_xyz ptr sf [ 4 ] ints (void) mgl_tube_xyz
libmgl mgl_tube_xy  ptr sf [ 3 ] ints (void) mgl_tube_xy
libmgl mgl_tube     ptr sf [ 2 ] ints (void) mgl_tube
\ --------- 2D графики
\           2D plotting functions
libmgl mgl_grid_xy sf [ 5 ] ints (void) mgl_grid_xy
libmgl mgl_grid sf    [ 3 ] ints (void) mgl_grid
libmgl mgl_mesh_xy    [ 4 ] ints  (void) mgl_mesh_xy
libmgl mgl_mesh       [ 3 ] ints (void) mgl_mesh
libmgl mgl_fall_xy    [ 4 ] ints (void) mgl_fall_xy
libmgl mgl_fall       [ 3 ] ints (void) mgl_fall
libmgl mgl_belt_xy    [ 5 ] ints (void) mgl_belt_xy
libmgl mgl_belt       [ 3 ] ints (void) mgl_belt
libmgl mgl_surf_xy    [ 5 ] ints (void) mgl_surf_xy
libmgl mgl_surf       [ 3 ] ints (void) mgl_surf
libmgl mgl_dens_xy sf [ 5 ] ints (void) mgl_dens_xy
libmgl mgl_dens sf    [ 3 ] ints (void) mgl_dens
libmgl mgl_boxs_xy sf [ 5 ] ints (void) mgl_boxs_xy
libmgl mgl_boxs sf    [ 3 ] ints (void) mgl_boxs
libmgl mgl_tile_xy    [ 5 ] ints (void) mgl_tile_xy
libmgl mgl_tile       [ 3 ] ints (void) mgl_tile
libmgl mgl_cont_xy_val  sf [ 6 ] ints (void) mgl_cont_xy_val
libmgl mgl_cont_val     sf [ 4 ] ints (void) mgl_cont_val
libmgl mgl_cont_xy      sf [ 6 ] ints (void) mgl_cont_xy
libmgl mgl_cont         sf [ 4 ] ints (void) mgl_cont
libmgl mgl_axial_xy_val    [ 6 ] ints (void) mgl_axial_xy_val
libmgl mgl_axial_val       [ 4 ] ints (void) mgl_axial_val
libmgl mgl_axial_xy        [ 6 ] ints (void) mgl_axial_xy
libmgl mgl_axial           [ 4 ] ints (void) mgl_axial
libmgl mgl_contf_xy_val sf [ 6 ] ints (void) mgl_contf_xy_val
libmgl mgl_contf_val    sf [ 4 ] ints (void) mgl_contf_val
libmgl mgl_contf_xy     sf [ 6 ] ints (void) mgl_contf_xy
libmgl mgl_contf        sf [ 4 ] ints (void) mgl_contf
\ --------- Парные графики
\           Dual plotting functions
libmgl mgl_surfc_xy ptr ptr ptr ptr ptr ptr (void) mgl_surfc_xy
libmgl mgl_surfc ptr ptr ptr ptr (void) mgl_surfc
libmgl mgl_surfa_xy ptr ptr ptr ptr ptr ptr (void) mgl_surfa_xy
libmgl mgl_surfa ptr ptr ptr ptr (void) mgl_surfa
libmgl mgl_vect_xy ptr ptr ptr ptr ptr ptr sf (void) mgl_vect_xy
libmgl mgl_vect_2d ptr ptr ptr ptr sf (void) mgl_vect_2d
libmgl mgl_vectc_xy ptr ptr ptr ptr ptr ptr sf (void) mgl_vectc_xy
libmgl mgl_vectc_2d ptr ptr ptr ptr sf (void) mgl_vectc_2d
libmgl mgl_vect_xyz ptr ptr ptr ptr ptr ptr ptr ptr (void) mgl_vect_xyz
libmgl mgl_vect_3d ptr ptr ptr ptr ptr (void) mgl_vect_3d
libmgl mgl_vectc_xyz ptr ptr ptr ptr ptr ptr ptr ptr (void) mgl_vectc_xyz
libmgl mgl_vectc_3d ptr ptr ptr ptr ptr (void) mgl_vectc_3d
libmgl mgl_map_xy ptr ptr ptr ptr ptr ptr int int (void) mgl_map_xy
libmgl mgl_map ptr ptr ptr ptr int int (void) mgl_map
libmgl mgl_surf3a_xyz_val ptr ptr ptr ptr ptr ptr sf ptr (void) mgl_surf3a_xyz_val
libmgl mgl_surf3a_val ptr ptr ptr sf ptr (void) mgl_surf3a_val
libmgl mgl_surf3a_xyz int ptr ptr ptr ptr ptr ptr ptr (void) mgl_surf3a_xyz
libmgl mgl_surf3a int ptr ptr ptr ptr (void) mgl_surf3a
libmgl mgl_surf3c_xyz_val ptr ptr ptr ptr ptr ptr sf ptr (void) mgl_surf3c_xyz_val
libmgl mgl_surf3c_val ptr ptr ptr sf ptr (void) mgl_surf3c_val
libmgl mgl_surf3c_xyz int ptr ptr ptr ptr ptr ptr ptr (void) mgl_surf3c_xyz
libmgl mgl_surf3c int ptr ptr ptr ptr (void) mgl_surf3c
libmgl mgl_flow_xy sf int int ptr ptr ptr ptr ptr ptr (void) mgl_flow_xy
libmgl mgl_flow_2d sf int int ptr ptr ptr ptr (void) mgl_flow_2d
libmgl mgl_flow_xyz int int ptr ptr ptr ptr ptr ptr ptr ptr (void) mgl_flow_xyz
libmgl mgl_flow_3d int int ptr ptr ptr ptr ptr (void) mgl_flow_3d
libmgl mgl_pipe_xy    sf int int sf [ 6 ] ints (void) mgl_pipe_xy
libmgl mgl_pipe_2d    sf int int sf [ 4 ] ints (void) mgl_pipe_2d
libmgl mgl_pipe_xyz      int int sf [ 7 ] ints (void) mgl_pipe_xyz
libmgl mgl_pipe_3d       int int sf [ 5 ] ints (void) mgl_pipe_3d
\ --------- 3D графики
\           3D plotting functions
libmgl mgl_grid3_xyz ptr ptr ptr ptr ptr int int ptr (void) mgl_grid3_xyz
libmgl mgl_grid3 ptr ptr int int ptr (void) mgl_grid3
libmgl mgl_grid3_all_xyz ptr ptr ptr ptr ptr ptr (void) mgl_grid3_all_xyz
libmgl mgl_grid3_all ptr ptr ptr (void) mgl_grid3_all
libmgl mgl_dens3_xyz ptr ptr ptr ptr ptr int int ptr (void) mgl_dens3_xyz
libmgl mgl_dens3 ptr ptr int int ptr (void) mgl_dens3
libmgl mgl_dens3_all_xyz ptr ptr ptr ptr ptr ptr (void) mgl_dens3_all_xyz
libmgl mgl_dens3_all ptr ptr ptr (void) mgl_dens3_all
libmgl mgl_surf3_xyz_val ptr ptr ptr ptr ptr sf ptr (void) mgl_surf3_xyz_val
libmgl mgl_surf3_val ptr ptr sf ptr (void) mgl_surf3_val
libmgl mgl_surf3_xyz ptr ptr ptr ptr ptr ptr int (void) mgl_surf3_xyz
libmgl mgl_surf3 ptr ptr ptr int (void) mgl_surf3
libmgl mgl_cont3_xyz_val ptr ptr ptr ptr ptr ptr int int ptr (void) mgl_cont3_xyz_val
libmgl mgl_cont3_val ptr ptr ptr int int ptr (void) mgl_cont3_val
libmgl mgl_cont3_xyz ptr ptr ptr ptr ptr int int ptr int (void) mgl_cont3_xyz
libmgl mgl_cont3 ptr ptr int int ptr int (void) mgl_cont3
libmgl mgl_cont_all_xyz ptr ptr ptr ptr ptr ptr int (void) mgl_cont_all_xyz
libmgl mgl_cont_all ptr ptr ptr int (void) mgl_cont_all
libmgl mgl_cloudp_xyz sf ptr ptr ptr ptr ptr ptr (void) mgl_cloudp_xyz
libmgl mgl_cloudp sf ptr ptr ptr (void) mgl_cloudp
libmgl mgl_cloudq_xyz sf ptr ptr ptr ptr ptr ptr (void) mgl_cloudq_xyz
libmgl mgl_cloudq sf ptr ptr ptr (void) mgl_cloudq
libmgl mgl_contf3_xyz_val             [ 6 ] ints (void) mgl_contf3_xyz_val
libmgl mgl_contf3_val                 [ 9 ] ints (void) mgl_contf3_val
libmgl mgl_contf3                     [ 6 ] ints (void) mgl_contf3
libmgl mgl_contf_all_xyz              [ 7 ] ints (void) mgl_contf_all_xyz
libmgl mgl_contf_all                  [ 4 ] ints (void) mgl_contf_all
libmgl mgl_beam_val int int sf [ 4 ] ints sf int (void) mgl_beam_val
libmgl mgl_beam int int int sf [ 5 ]        ints (void) mgl_beam_val
\ ---------
\            Triangular plotting functions  
libmgl mgl_triplot_xyz ptr ptr ptr ptr ptr ptr (void) mgl_triplot_xyz
libmgl mgl_triplot_xy sf ptr ptr ptr ptr ptr (void) mgl_triplot_xy
libmgl mgl_dots ptr ptr ptr ptr ptr (void) mgl_dots
libmgl mgl_dots_tr ptr ptr ptr (void) mgl_dots_tr
libmgl mgl_crust sf ptr ptr ptr ptr ptr (void) mgl_crust
libmgl mgl_crust_tr sf ptr ptr ptr  (void) mgl_crust_tr
\ --------- Прочие графики
\           Combined plotting functions
libmgl mgl_dens_x sf ptr ptr ptr (void) mgl_dens_x
libmgl mgl_dens_y sf ptr ptr ptr (void) mgl_dens_y
libmgl mgl_dens_z sf ptr ptr ptr (void) mgl_dens_z
libmgl mgl_cont_x int sf ptr ptr ptr (void) mgl_cont_x
libmgl mgl_cont_y ptr sf ptr ptr ptr (void) mgl_cont_y
libmgl mgl_cont_z ptr sf ptr ptr ptr (void) mgl_cont_z
libmgl mgl_cont_x_val sf ptr ptr ptr ptr (void) mgl_cont_x_val
libmgl mgl_cont_y_val sf ptr ptr ptr ptr (void) mgl_cont_y_val
libmgl mgl_cont_z_val sf ptr ptr ptr ptr (void) mgl_cont_z_val
libmgl mgl_contf_x int sf int int int (void) mgl_contf_x
libmgl mgl_contf_y int sf int int int (void) mgl_contf_y
libmgl mgl_contf_z int sf int int int (void) mgl_contf_z
libmgl mgl_contf_x_val  sf [ 4 ] ints (void) mgl_contf_x_val
libmgl mgl_contf_y_val  sf [ 4 ] ints (void) mgl_contf_y_val
libmgl mgl_contf_z_val  sf [ 4 ] ints (void) mgl_contf_z_val
\ --------- Заполнение массивов данных
\            Data creation functions
libmgl mgl_data_set_float  [ 5 ] ints (void) mgl_data_set_float
libmgl mgl_data_set_double [ 5 ] ints (void) mgl_data_set_double
libmgl mgl_data_set_double2 int int ptr ptr (void) mgl_data_set_double2
libmgl mgl_data_set_double3 int int int ptr ptr (void) mgl_data_set_double3
libmgl mgl_data_set ptr ptr (void) mgl_data_set
libmgl mgl_data_set_vector ptr ptr (void) mgl_data_set_vector
libmgl mgl_data_set_matrix ptr ptr (void) mgl_data_set_matrix
libmgl mgl_data_read ptr ptr (int) mgl_data_read
libmgl mgl_data_read_dim ptr ptr int int int (int) mgl_data_read_dim
libmgl mgl_data_save ptr ptr int (void) mgl_data_save
libmgl mgl_data_export int sf sf ptr ptr ptr (void) mgl_data_export
libmgl mgl_data_import sf sf ptr ptr ptr (void) mgl_data_import
libmgl mgl_data_create int int int ptr (void) mgl_data_create
libmgl mgl_data_transpose ptr (void) mgl_data_transpose
libmgl mgl_data_norm int int df df ptr (void) mgl_data_norm
libmgl mgl_data_subdata ptr int int int (ptr) mgl_data_subdata
libmgl mgl_data_fill int df df ptr (void) mgl_data_fill
libmgl mgl_data_modify ptr ptr int (void) mgl_data_modify
libmgl mgl_data_squeeze ptr int int int int (void) mgl_data_squeeze
libmgl mgl_data_max ptr (fp) mgl_data_max
libmgl mgl_data_min ptr (fp) mgl_data_min
\ libmgl mgl_data_insert ptr ptr (void) mgl_data_insert
\ libmgl mgl_data_pullout ptr (void) mgl_data_pullout
libmgl mgl_data_value ptr int int int (fp) mgl_data_value
libmgl mgl_data_data ptr (ptr) mgl_data_data
libmgl mgl_data_combine int int (ptr) mgl_data_combine
libmgl mgl_data_extend int int ptr (void) mgl_data_extend
\ --------- Изменение массивов данных
\          Data manipulation functions
libmgl mgl_data_smooth ptr sf int ptr (void) mgl_data_smooth
libmgl mgl_data_sum         ptr ptr (ptr) mgl_data_sum
libmgl mgl_data_integral ptr   ptr (void) mgl_data_integral
libmgl mgl_data_diff     ptr   ptr (void) mgl_data_diff
libmgl mgl_data_diff2    ptr   ptr (void) mgl_data_diff2
libmgl mgl_data_swap     ptr   ptr (void) mgl_data_swap
libmgl mgl_data_spline  sf sf sf ptr (fp) mgl_data_spline
libmgl mgl_data_spline1 sf sf sf ptr (fp) mgl_data_spline1
libmgl mgl_data_linear sf sf  sf ptr (fp) mgl_data_linear
libmgl mgl_data_linear1 sf sf sf ptr (fp) mgl_data_linear1
libmgl mgl_data_resize sf sf sf sf sf sf int int int ptr (ptr) mgl_data_resize
\ --------- Операции с массивами данных
\              Data operations
libmgl mgl_data_mul_dat ptr ptr (void) mgl_data_mul_dat
libmgl mgl_data_div_dat ptr ptr (void) mgl_data_div_dat
libmgl mgl_data_add_dat ptr ptr (void) mgl_data_add_dat
libmgl mgl_data_sub_dat ptr ptr (void) mgl_data_sub_dat
libmgl mgl_data_mul_num sf  ptr (void) mgl_data_mul_num
libmgl mgl_data_div_num sf  ptr (void) mgl_data_div_num
libmgl mgl_data_add_num sf  ptr (void) mgl_data_add_num
libmgl mgl_data_sub_num sf  ptr (void) mgl_data_sub_num

legacy on

previous
Module;

\ Change zero to one to run the test as
\ > bigforth mathgl.fs
0 [IF]
    also float also mathgl
    600 600 mgl_create_graph_zb value gr
    \ gr 192e fdup fdup mgl_clf_rgb
    gr 0 mgl_set_alpha
    gr 1 mgl_set_transp_type
    gr 1 mgl_set_light
    30 20 1 mgl_create_data_size value a
    a 0" 0.6*sin(2*pi*x)*sin(3*pi*y) + 0.4*cos(3*pi*(x*y))" 0 mgl_data_modify
    gr 2 2 0 mgl_subplot
    gr !40 !60 !0 mgl_rotate
    gr a 0" BbcyrR#" mgl_surf
    gr 1 mgl_box
    gr 2 2 1 mgl_subplot
    gr !40 !60 !0 mgl_rotate
    gr a 0" BbcyrR" !0 mgl_dens
    gr 1 mgl_box
    gr 2 2 2 mgl_subplot
    gr !40 !60 !0 mgl_rotate
    gr a 0" BbcyrR" 7 !0 mgl_cont
    gr 1 mgl_box
    gr 2 2 3 mgl_subplot
    gr !40 !60 !0 mgl_rotate
    gr a 0" BbcyrR" 3 mgl_axial
    gr 1 mgl_box
\ don’t forget to save graphics /
    gr 0" sample.png" 0 mgl_write_png
    bye
[THEN]
