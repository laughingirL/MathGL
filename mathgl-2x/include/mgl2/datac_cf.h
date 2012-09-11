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
class mglDataC;
typedef mglDataC* HADT;
extern "C" {
#endif
/******************************************************************************/
HADT mgl_create_datac();
HADT mgl_create_datac_size(long nx, long ny, long nz);
HADT mgl_create_datac_file(const char *fname);
void mgl_delete_datac(HADT dat);
/******************************************************************************/
uintptr_t mgl_create_datac_();
uintptr_t mgl_create_datac_size_(int *nx, int *ny, int *nz);
uintptr_t mgl_create_datac_file_(const char *fname, int len);
void mgl_delete_datac_(uintptr_t *dat);
/******************************************************************************/
/*		Data creation functions												*/
/******************************************************************************/
void mgl_datac_rearrange(HADT dat, long mx,long my,long mz);
void mgl_datac_link(HADT dat, mreal *A,long mx,long my,long mz);
void mgl_datac_set_float(HADT dat, const float *A,long mx,long my,long mz);
void mgl_datac_set_double(HADT dat, const double *A,long mx,long my,long mz);
void mgl_datac_set_float2(HADT d, const float **A,long N1,long N2);
void mgl_datac_set_double2(HADT d, const double **A,long N1,long N2);
void mgl_datac_set_float3(HADT d, const float ***A,long N1,long N2,long N3);
void mgl_datac_set_double3(HADT d, const double ***A,long N1,long N2,long N3);
void mgl_datac_set(HADT dat, HCDT a);
void mgl_datac_set_vector(HADT dat, gsl_vector *v);
void mgl_datac_set_matrix(HADT dat, gsl_matrix *m);
void mgl_datac_set_value(HADT dat, mreal v, long i, long j, long k);
void mgl_datac_set_values(HADT dat, const char *val, long nx, long ny, long nz);

int mgl_datac_read(HADT dat, const char *fname);
int mgl_datac_read_mat(HADT dat, const char *fname, long dim);
int mgl_datac_read_dim(HADT dat, const char *fname,long mx,long my,long mz);
int mgl_datac_read_range(HADT d, const char *templ, double from, double to, double step, int as_slice);
int mgl_datac_read_all(HADT dat, const char *templ, int as_slice);
void mgl_datac_save(HADT dat, const char *fname,long ns);

void mgl_datac_create(HADT dat, long nx,long ny,long nz);
void mgl_datac_transpose(HADT dat, const char *dim);
void mgl_datac_set_id(HADT d, const char *id);
void mgl_datac_fill(HADT dat, mreal x1,mreal x2,char dir);
void mgl_datac_fill_eq(HMGL gr, HADT dat, const char *eq, HCDT vdat, HCDT wdat,const char *opt);

void mgl_datac_modify(HADT dat, const char *eq,long dim);
void mgl_datac_modify_vw(HADT dat, const char *eq,HCDT vdat,HCDT wdat);
void mgl_datac_modify_cvw(HADT dat, const char *eq,HADT vdat,HADT wdat);
void mgl_datac_squeeze(HADT dat, long rx,long ry,long rz,long smooth);

void mgl_datac_extend(HADT dat, long n1, long n2);
void mgl_datac_insert(HADT dat, char dir, long at, long num);
void mgl_datac_delete(HADT dat, char dir, long at, long num);
/******************************************************************************/
/*		Data manipulation functions											*/
/******************************************************************************/
void mgl_datac_smooth(HADT d, const char *dirs, mreal delta);
void mgl_datac_cumsum(HADT dat, const char *dir);
void mgl_datac_integral(HADT dat, const char *dir);
void mgl_datac_diff(HADT dat, const char *dir);
void mgl_datac_diff_par(HADT dat, HCDT v1, HCDT v2, HCDT v3);
void mgl_datac_diff2(HADT dat, const char *dir);
void mgl_datac_swap(HADT dat, const char *dir);
void mgl_datac_roll(HADT dat, char dir, long num);
void mgl_datac_mirror(HADT dat, const char *dir);
void mgl_datac_crop(HADT dat, long n1, long n2, char dir);

void mgl_datac_hankel(HADT dat, const char *dir);
void mgl_datac_fft(HADT dat, const char *dir);
void mgl_datac_sinfft(HADT dat, const char *dir);
void mgl_datac_cosfft(HADT dat, const char *dir);
/******************************************************************************/
/*		Data creation functions												*/
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
