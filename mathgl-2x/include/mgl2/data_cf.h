/***************************************************************************
 * data_cf.h is part of Math Graphic Library
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
#ifndef _MGL_DATA_CF_H_
#define _MGL_DATA_CF_H_
/******************************************************************************/
#include "mgl2/base.h"
/******************************************************************************/
#if MGL_HAVE_GSL
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#else
struct gsl_vector;
struct gsl_matrix;
#endif
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
void mgl_srnd(long seed);
double mgl_rnd();
double mgl_ipow(double x,int n);
/******************************************************************************/
HMDT mgl_create_data();
HMDT mgl_create_data_size(long nx, long ny, long nz);
HMDT mgl_create_data_file(const char *fname);
void mgl_delete_data(HMDT dat);
const char *mgl_data_info(HCDT dat);
/******************************************************************************/
uintptr_t mgl_create_data_();
uintptr_t mgl_create_data_size_(int *nx, int *ny, int *nz);
uintptr_t mgl_create_data_file_(const char *fname, int len);
void mgl_delete_data_(uintptr_t *dat);
/******************************************************************************/
/*		Data creation functions												 */
/******************************************************************************/
void mgl_data_rearrange(HMDT dat, long mx,long my,long mz);
void mgl_data_link(HMDT dat, mreal *A,long mx,long my,long mz);
void mgl_data_set_float(HMDT dat, const float *A,long mx,long my,long mz);
void mgl_data_set_double(HMDT dat, const double *A,long mx,long my,long mz);
void mgl_data_set_float2(HMDT d, const float **A,long N1,long N2);
void mgl_data_set_double2(HMDT d, const double **A,long N1,long N2);
void mgl_data_set_float3(HMDT d, const float ***A,long N1,long N2,long N3);
void mgl_data_set_double3(HMDT d, const double ***A,long N1,long N2,long N3);
void mgl_data_set(HMDT dat, HCDT a);
void mgl_data_set_vector(HMDT dat, gsl_vector *v);
void mgl_data_set_matrix(HMDT dat, gsl_matrix *m);
void mgl_data_set_value(HMDT dat, mreal v, long i, long j, long k);
mreal mgl_data_get_value(HCDT dat, long i, long j, long k);
void mgl_data_set_values(HMDT dat, const char *val, long nx, long ny, long nz);

int mgl_data_read_hdf(HMDT d,const char *fname,const char *data);
void mgl_data_save_hdf(HCDT d,const char *fname,const char *data,int rewrite);
int mgl_datas_hdf(const char *fname, char *buf, long size);
int mgl_data_read(HMDT dat, const char *fname);
int mgl_data_read_mat(HMDT dat, const char *fname, long dim);
int mgl_data_read_dim(HMDT dat, const char *fname,long mx,long my,long mz);
int mgl_data_read_range(HMDT d, const char *templ, double from, double to, double step, int as_slice);
int mgl_data_read_all(HMDT dat, const char *templ, int as_slice);
void mgl_data_save(HCDT dat, const char *fname,long ns);
void mgl_data_export(HCDT dat, const char *fname, const char *scheme,mreal v1,mreal v2,long ns);
void mgl_data_import(HMDT dat, const char *fname, const char *scheme,mreal v1,mreal v2);

void mgl_data_create(HMDT dat, long nx,long ny,long nz);
void mgl_data_transpose(HMDT dat, const char *dim);
void mgl_data_norm(HMDT dat, mreal v1,mreal v2,long sym,long dim);
void mgl_data_norm_slice(HMDT dat, mreal v1,mreal v2,char dir,long keep_en,long sym);
HMDT mgl_data_subdata(HCDT dat, long xx,long yy,long zz);
HMDT mgl_data_subdata_ext(HCDT dat, HCDT xx, HCDT yy, HCDT zz);
HMDT mgl_data_column(HCDT dat, const char *eq);
void mgl_data_set_id(HMDT d, const char *id);
void mgl_data_fill(HMDT dat, mreal x1,mreal x2,char dir);
void mgl_data_fill_eq(HMGL gr, HMDT dat, const char *eq, HCDT vdat, HCDT wdat,const char *opt);
void mgl_data_grid(HMGL gr, HMDT d, HCDT xdat, HCDT ydat, HCDT zdat,const char *opt);
void mgl_data_put_val(HMDT dat, mreal val, long i, long j, long k);
void mgl_data_put_dat(HMDT dat, HCDT val, long i, long j, long k);
void mgl_data_modify(HMDT dat, const char *eq,long dim);
void mgl_data_modify_vw(HMDT dat, const char *eq,HCDT vdat,HCDT wdat);
void mgl_data_squeeze(HMDT dat, long rx,long ry,long rz,long smooth);
mreal mgl_data_max(HCDT dat);
mreal mgl_data_min(HCDT dat);
mreal *mgl_data_value(HMDT dat, long i,long j,long k);
mreal *mgl_data_data(HMDT dat);

mreal mgl_data_get_value(HCDT d, long i, long j, long k);
long mgl_data_get_nx(HCDT d);
long mgl_data_get_ny(HCDT d);
long mgl_data_get_nz(HCDT d);

mreal mgl_data_first(HCDT dat, const char *cond, long *i, long *j, long *k);
mreal mgl_data_last(HCDT dat, const char *cond, long *i, long *j, long *k);
long mgl_data_find(HCDT dat, const char *cond, char dir, long i, long j, long k);
int mgl_data_find_any(HCDT dat, const char *cond);
mreal mgl_data_max_int(HCDT dat, long *i, long *j, long *k);
mreal mgl_data_max_real(HCDT dat, mreal *x, mreal *y, mreal *z);
mreal mgl_data_min_int(HCDT dat, long *i, long *j, long *k);
mreal mgl_data_min_real(HCDT dat, mreal *x, mreal *y, mreal *z);
mreal mgl_data_momentum_val(HCDT d, char dir, mreal *m, mreal *w, mreal *s, mreal *k);

HMDT mgl_data_combine(HCDT dat1, HCDT dat2);
void mgl_data_extend(HMDT dat, long n1, long n2);
void mgl_data_insert(HMDT dat, char dir, long at, long num);
void mgl_data_delete(HMDT dat, char dir, long at, long num);
/******************************************************************************/
/*		Data manipulation functions											 */
/******************************************************************************/
void mgl_data_smooth(HMDT d, const char *dirs, mreal delta);
HMDT mgl_data_sum(HCDT dat, const char *dir);
HMDT mgl_data_max_dir(HCDT dat, const char *dir);
HMDT mgl_data_min_dir(HCDT dat, const char *dir);
void mgl_data_cumsum(HMDT dat, const char *dir);
void mgl_data_integral(HMDT dat, const char *dir);
void mgl_data_diff(HMDT dat, const char *dir);
void mgl_data_diff_par(HMDT dat, HCDT v1, HCDT v2, HCDT v3);
void mgl_data_diff2(HMDT dat, const char *dir);
void mgl_data_swap(HMDT dat, const char *dir);
void mgl_data_roll(HMDT dat, char dir, long num);
void mgl_data_mirror(HMDT dat, const char *dir);
void mgl_data_sort(HMDT dat, long idx, long idy);

void mgl_data_hankel(HMDT dat, const char *dir);
void mgl_data_sinfft(HMDT dat, const char *dir);
void mgl_data_cosfft(HMDT dat, const char *dir);
void mgl_data_fill_sample(HMDT dat, const char *how);

mreal mgl_data_spline(HCDT dat, mreal x,mreal y,mreal z);
mreal mgl_data_linear(HCDT dat, mreal x,mreal y,mreal z);
mreal mgl_data_spline_ext(HCDT dat, mreal x,mreal y,mreal z, mreal *dx,mreal *dy,mreal *dz);
mreal mgl_data_linear_ext(HCDT dat, mreal x,mreal y,mreal z, mreal *dx,mreal *dy,mreal *dz);
mreal mgl_data_solve_1d(HCDT dat, mreal val, int spl, long i0);
HMDT mgl_data_solve(HCDT dat, mreal val, char dir, HCDT i0, int norm);

HMDT mgl_data_trace(HCDT d);
HMDT mgl_data_resize(HCDT dat, long mx,long my,long mz);
HMDT mgl_data_resize_box(HCDT dat, long mx,long my,long mz,mreal x1,mreal x2,mreal y1,mreal y2,mreal z1,mreal z2);
HMDT mgl_data_hist(HCDT dat, long n, mreal v1, mreal v2, long nsub);
HMDT mgl_data_hist_w(HCDT dat, HCDT weight, long n, mreal v1, mreal v2, long nsub);
HMDT mgl_data_momentum(HCDT dat, char dir, const char *how);
HMDT mgl_data_evaluate(HCDT dat, HCDT idat, HCDT jdat, HCDT kdat, int norm);
void mgl_data_envelop(HMDT dat, char dir);
void mgl_data_sew(HMDT dat, const char *dirs, mreal da);
void mgl_data_crop(HMDT dat, long n1, long n2, char dir);
void mgl_data_clean(HMDT dat, long id);
/******************************************************************************/
/*		Data operations														 */
/******************************************************************************/
void mgl_data_mul_dat(HMDT dat, HCDT d);
void mgl_data_div_dat(HMDT dat, HCDT d);
void mgl_data_add_dat(HMDT dat, HCDT d);
void mgl_data_sub_dat(HMDT dat, HCDT d);
void mgl_data_mul_num(HMDT dat, mreal d);
void mgl_data_div_num(HMDT dat, mreal d);
void mgl_data_add_num(HMDT dat, mreal d);
void mgl_data_sub_num(HMDT dat, mreal d);
/******************************************************************************/
HMDT mgl_pde_solve(HMGL gr, const char *ham, HCDT ini_re, HCDT ini_im, mreal dz, mreal k0,const char *opt);
HMDT mgl_qo2d_solve(const char *ham, HCDT ini_re, HCDT ini_im, HCDT ray, mreal r, mreal k0, HMDT xx, HMDT yy);
HMDT mgl_ray_trace(const char *ham, mreal x0, mreal y0, mreal z0, mreal px, mreal py, mreal pz, mreal dt, mreal tmax);
HMDT mgl_jacobian_2d(HCDT x, HCDT y);
HMDT mgl_jacobian_3d(HCDT x, HCDT y, HCDT z);
HMDT mgl_transform_a(HCDT am, HCDT ph, const char *tr);
HMDT mgl_transform(HCDT re, HCDT im, const char *tr);
void mgl_data_fourier(HMDT re, HMDT im, const char *dir);
HMDT mgl_data_stfa(HCDT re, HCDT im, long dn, char dir);

HMDT mgl_triangulation_3d(HCDT x, HCDT y, HCDT z);
HMDT mgl_triangulation_2d(HCDT x, HCDT y);
/******************************************************************************/
/*		Data creation functions												 */
/******************************************************************************/
void mgl_data_rearrange_(uintptr_t *dat, int *mx, int *my, int *mz);
void mgl_data_set_float_(uintptr_t *dat, const float *A,int *NX,int *NY,int *NZ);
void mgl_data_set_double_(uintptr_t *dat, const double *A,int *NX,int *NY,int *NZ);
void mgl_data_set_float1_(uintptr_t *d, const float *A,int *N1);
void mgl_data_set_double1_(uintptr_t *d, const double *A,int *N1);
void mgl_data_set_float2_(uintptr_t *d, const float *A,int *N1,int *N2);
void mgl_data_set_double2_(uintptr_t *d, const double *A,int *N1,int *N2);
void mgl_data_set_float3_(uintptr_t *d, const float *A,int *N1,int *N2,int *N3);
void mgl_data_set_double3_(uintptr_t *d, const double *A,int *N1,int *N2,int *N3);
void mgl_data_set_(uintptr_t *dat, uintptr_t *a);

void mgl_data_set_value_(uintptr_t *d, mreal *v, int *i, int *j, int *k);
void mgl_data_set_values_(uintptr_t *d, const char *val, int *nx, int *ny, int *nz, int l);
mreal mgl_data_get_value_(uintptr_t *d, int *i, int *j, int *k);
long mgl_data_get_nx_(uintptr_t *d);
long mgl_data_get_ny_(uintptr_t *d);
long mgl_data_get_nz_(uintptr_t *d);

int mgl_data_read_(uintptr_t *d, const char *fname,int l);
int mgl_data_read_mat_(uintptr_t *dat, const char *fname, int *dim, int);
int mgl_data_read_dim_(uintptr_t *dat, const char *fname,int *mx,int *my,int *mz,int);
void mgl_data_save_(uintptr_t *dat, const char *fname,int *ns,int);
void mgl_data_export_(uintptr_t *dat, const char *fname, const char *scheme,mreal *v1,mreal *v2,int *ns,int,int);
void mgl_data_import_(uintptr_t *dat, const char *fname, const char *scheme,mreal *v1,mreal *v2,int,int);
void mgl_data_create_(uintptr_t *dat, int *nx,int *ny,int *nz);
void mgl_data_transpose_(uintptr_t *dat, const char *dim,int);
void mgl_data_norm_(uintptr_t *dat, mreal *v1,mreal *v2,int *sym,int *dim);
void mgl_data_norm_slice_(uintptr_t *dat, mreal *v1,mreal *v2,char *dir,int *keep_en,int *sym,int l);
uintptr_t mgl_data_subdata_(uintptr_t *dat, int *xx,int *yy,int *zz);
uintptr_t mgl_data_subdata_ext_(uintptr_t *dat, uintptr_t *xx,uintptr_t *yy,uintptr_t *zz);
uintptr_t mgl_data_column_(uintptr_t *dat, const char *eq,int l);
void mgl_data_set_id_(uintptr_t *dat, const char *id,int l);
void mgl_data_fill_(uintptr_t *dat, mreal *x1,mreal *x2,const char *dir,int);
void mgl_data_fill_eq_(uintptr_t *gr, uintptr_t *dat, const char *eq, uintptr_t *vdat, uintptr_t *wdat,const char *opt, int, int);
void mgl_data_grid_(uintptr_t *gr, uintptr_t *dat, uintptr_t *xdat, uintptr_t *ydat, uintptr_t *zdat, const char *opt,int);
void mgl_data_put_val_(uintptr_t *dat, mreal *val, int *i, int *j, int *k);
void mgl_data_put_dat_(uintptr_t *dat, uintptr_t *val, int *i, int *j, int *k);
void mgl_data_modify_(uintptr_t *dat, const char *eq,int *dim,int);
void mgl_data_modify_vw_(uintptr_t *dat, const char *eq, uintptr_t *vdat, uintptr_t *wdat,int);
void mgl_data_squeeze_(uintptr_t *dat, int *rx,int *ry,int *rz,int *smooth);
mreal mgl_data_max_(uintptr_t *dat);
mreal mgl_data_min_(uintptr_t *dat);

mreal mgl_data_first_(uintptr_t *dat, const char *cond, int *i, int *j, int *k, int);
mreal mgl_data_last_(uintptr_t *dat, const char *cond, int *i, int *j, int *k, int);
int mgl_data_find_(uintptr_t *dat, const char *cond, char *dir, int *i, int *j, int *k, int,int);
int mgl_data_find_any_(uintptr_t *dat, const char *cond, int);
mreal mgl_data_max_int_(uintptr_t *dat, int *i, int *j, int *k);
mreal mgl_data_max_real_(uintptr_t *dat, mreal *x, mreal *y, mreal *z);
mreal mgl_data_min_int_(uintptr_t *dat, int *i, int *j, int *k);
mreal mgl_data_min_real_(uintptr_t *dat, mreal *x, mreal *y, mreal *z);
mreal mgl_data_momentum_val_(uintptr_t *dat, char *dir, mreal *m, mreal *w, mreal *s, mreal *k,int);
void mgl_data_clean_(uintptr_t *dat, int *id);

uintptr_t mgl_data_combine_(uintptr_t *dat1, uintptr_t *dat2);
void mgl_data_extend_(uintptr_t *dat, int *n1, int *n2);
void mgl_data_insert_(uintptr_t *dat, const char *dir, int *at, int *num, int);
void mgl_data_delete_(uintptr_t *dat, const char *dir, int *at, int *num, int);
/******************************************************************************/
/*		Data manipulation functions											 */
/******************************************************************************/
void mgl_data_smooth_(uintptr_t *dat, const char *dirs, mreal *delta,int);
uintptr_t mgl_data_sum_(uintptr_t *dat, const char *dir,int);
uintptr_t mgl_data_max_dir_(uintptr_t *dat, const char *dir,int);
uintptr_t mgl_data_min_dir_(uintptr_t *dat, const char *dir,int);
void mgl_data_cumsum_(uintptr_t *dat, const char *dir,int);
void mgl_data_integral_(uintptr_t *dat, const char *dir,int);
void mgl_data_diff_(uintptr_t *dat, const char *dir,int);
void mgl_data_diff_par_(uintptr_t *dat, uintptr_t *v1, uintptr_t *v2, uintptr_t *v3);
void mgl_data_diff2_(uintptr_t *dat, const char *dir,int);
void mgl_data_swap_(uintptr_t *dat, const char *dir,int);
void mgl_data_roll_(uintptr_t *dat, const char *dir, int *num, int);
void mgl_data_mirror_(uintptr_t *dat, const char *dir,int);
void mgl_data_sort_(uintptr_t *dat, int *idx, int *idy);

void mgl_data_hankel_(uintptr_t *dat, const char *dir,int);
void mgl_data_sinfft_(uintptr_t *dat, const char *dir,int);
void mgl_data_cosfft_(uintptr_t *dat, const char *dir,int);
void mgl_data_fill_sample_(uintptr_t *dat, const char *how,int);

mreal mgl_data_spline_(uintptr_t *dat, mreal *x,mreal *y,mreal *z);
mreal mgl_data_linear_(uintptr_t *dat, mreal *x,mreal *y,mreal *z);
mreal mgl_data_spline_ext_(uintptr_t *dat, mreal *x,mreal *y,mreal *z, mreal *dx,mreal *dy,mreal *dz);
mreal mgl_data_linear_ext_(uintptr_t *dat, mreal *x,mreal *y,mreal *z, mreal *dx,mreal *dy,mreal *dz);
mreal mgl_data_solve_1d_(uintptr_t *dat, mreal *val, int *spl, int *i0);
uintptr_t mgl_data_solve_(uintptr_t *dat, mreal *val, const char *dir, uintptr_t *i0, int *norm,int);

uintptr_t mgl_data_trace_(uintptr_t *d);
uintptr_t mgl_data_resize_(uintptr_t *dat, int *mx,int *my,int *mz);
uintptr_t mgl_data_resize_box_(uintptr_t *dat, int *mx,int *my,int *mz,mreal *x1,mreal *x2,mreal *y1,mreal *y2,mreal *z1,mreal *z2);
uintptr_t mgl_data_momentum_(uintptr_t *dat, char *dir, const char *how, int,int);
uintptr_t mgl_data_hist_(uintptr_t *dat, int *n, mreal *v1, mreal *v2, int *nsub);
uintptr_t mgl_data_hist_w_(uintptr_t *dat, uintptr_t *weight, int *n, mreal *v1, mreal *v2, int *nsub);
uintptr_t mgl_data_evaluate_(uintptr_t *dat, uintptr_t *idat, uintptr_t *jdat, uintptr_t *kdat, int *norm);
void mgl_data_envelop_(uintptr_t *dat, const char *dir, int);
void mgl_data_sew_(uintptr_t *dat, const char *dirs, mreal *da, int);
void mgl_data_crop_(uintptr_t *dat, int *n1, int *n2, const char *dir,int);
/******************************************************************************/
/*		Data operations														 */
/******************************************************************************/
void mgl_data_mul_dat_(uintptr_t *dat, uintptr_t *d);
void mgl_data_div_dat_(uintptr_t *dat, uintptr_t *d);
void mgl_data_add_dat_(uintptr_t *dat, uintptr_t *d);
void mgl_data_sub_dat_(uintptr_t *dat, uintptr_t *d);
void mgl_data_mul_num_(uintptr_t *dat, mreal *d);
void mgl_data_div_num_(uintptr_t *dat, mreal *d);
void mgl_data_add_num_(uintptr_t *dat, mreal *d);
void mgl_data_sub_num_(uintptr_t *dat, mreal *d);
/******************************************************************************/
/*		Nonlinear fitting													 */
/******************************************************************************/
uintptr_t mgl_pde_solve_(uintptr_t* gr, const char *ham, uintptr_t* ini_re, uintptr_t* ini_im, mreal *dz, mreal *k0,const char *opt,int,int);
uintptr_t mgl_qo2d_solve_(const char *ham, uintptr_t* ini_re, uintptr_t* ini_im, uintptr_t* ray, mreal *r, mreal *k0, uintptr_t* xx, uintptr_t* yy, int);
uintptr_t mgl_ray_trace_(const char *ham, mreal *x0, mreal *y0, mreal *z0, mreal *px, mreal *py, mreal *pz, mreal *dt, mreal *tmax,int);
uintptr_t mgl_jacobian_2d_(uintptr_t* x, uintptr_t* y);
uintptr_t mgl_jacobian_3d_(uintptr_t* x, uintptr_t* y, uintptr_t* z);

uintptr_t mgl_transform_a_(uintptr_t *am, uintptr_t *ph, const char *tr, int);
uintptr_t mgl_transform_(uintptr_t *re, uintptr_t *im, const char *tr, int);
void mgl_data_fourier_(uintptr_t *re, uintptr_t *im, const char *dir, int l);
uintptr_t mgl_data_stfa_(uintptr_t *re, uintptr_t *im, int *dn, char *dir, int);

uintptr_t mgl_triangulation_3d_(uintptr_t *x, uintptr_t *y, uintptr_t *z);
uintptr_t mgl_triangulation_2d_(uintptr_t *x, uintptr_t *y);
/******************************************************************************/
int mgl_data_read_hdf_(uintptr_t *d, const char *fname, const char *data,int l,int n);
void mgl_data_link_(uintptr_t *d, float *A, int *nx,int *ny,int *nz);
void mgl_data_save_hdf_(uintptr_t *d, const char *fname, const char *data, int *rewrite,int l,int n);
int mgl_data_read_range_(uintptr_t *d, const char *fname, double *from, double *to, double *step, int *as_slice,int l);
int mgl_data_read_all_(uintptr_t *d, const char *fname, int *as_slice,int l);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
