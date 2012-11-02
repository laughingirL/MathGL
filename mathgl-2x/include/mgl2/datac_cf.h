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
#ifndef _MGL_DATAC_CF_H_
#define _MGL_DATAC_CF_H_
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
#else
typedef void *HADT;
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
/*		Data creation functions													*/
/******************************************************************************/
void mgl_datac_rearrange(HADT dat, long mx,long my,long mz);
void mgl_datac_link(HADT dat, dual *A,long mx,long my,long mz);
void mgl_datac_set_float(HADT dat, const float *A,long mx,long my,long mz);
void mgl_datac_set_double(HADT dat, const double *A,long mx,long my,long mz);
void mgl_datac_set_complex(HADT dat, const dual *A,long mx,long my,long mz);
void mgl_datac_set(HADT dat, HCDT a);
void mgl_datac_set_vector(HADT dat, gsl_vector *v);
void mgl_datac_set_matrix(HADT dat, gsl_matrix *m);
void mgl_datac_set_value(HADT dat, dual v, long i, long j, long k);
void mgl_datac_set_values(HADT dat, const char *val, long nx, long ny, long nz);
dual mgl_datac_get_value(HCDT dat, long i, long j, long k);
dual *mgl_datac_data(HADT dat);
dual *mgl_datac_value(HADT dat, long i,long j,long k);

void mgl_datac_set_ri(HADT dat, HCDT re, HCDT im);
void mgl_datac_set_ap(HADT dat, HCDT abs, HCDT phi);

int mgl_datac_read(HADT dat, const char *fname);
int mgl_datac_read_mat(HADT dat, const char *fname, long dim);
int mgl_datac_read_dim(HADT dat, const char *fname,long mx,long my,long mz);
int mgl_datac_read_range(HADT d, const char *templ, double from, double to, double step, int as_slice);
int mgl_datac_read_all(HADT dat, const char *templ, int as_slice);
void mgl_datac_save(HCDT dat, const char *fname,long ns);

int mgl_datac_read_hdf(HADT d,const char *fname,const char *data);
void mgl_datac_save_hdf(HCDT d,const char *fname,const char *data,int rewrite);
int mgl_datas_hdf(const char *fname, char *buf, long size);

void mgl_datac_create(HADT dat, long nx,long ny,long nz);
void mgl_datac_transpose(HADT dat, const char *dim);
void mgl_datac_set_id(HADT d, const char *id);
void mgl_datac_fill(HADT dat, dual x1,dual x2,char dir);
void mgl_datac_fill_eq(HMGL gr, HADT dat, const char *eq, HCDT vdat, HCDT wdat,const char *opt);

void mgl_datac_modify(HADT dat, const char *eq,long dim);
void mgl_datac_modify_vw(HADT dat, const char *eq,HCDT vdat,HCDT wdat);
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
void mgl_datac_diff2(HADT dat, const char *dir);
void mgl_datac_swap(HADT dat, const char *dir);
void mgl_datac_roll(HADT dat, char dir, long num);
void mgl_datac_mirror(HADT dat, const char *dir);
void mgl_datac_crop(HADT dat, long n1, long n2, char dir);

void mgl_datac_hankel(HADT dat, const char *dir);
void mgl_datac_fft(HADT dat, const char *dir);

HMDT mgl_datac_real(HCDT dat);
HMDT mgl_datac_imag(HCDT dat);
HMDT mgl_datac_abs(HCDT dat);
HMDT mgl_datac_arg(HCDT dat);

dual mgl_datac_linear_ext(HCDT d, mreal x,mreal y,mreal z, dual *dx,dual *dy,dual *dz);
dual mgl_datac_linear(HCDT d, mreal x,mreal y,mreal z);
dual mgl_datac_linear_ext_(uintptr_t *d, mreal *x,mreal *y,mreal *z, dual *dx,dual *dy,dual *dz);
dual mgl_datac_linear_(uintptr_t *d, mreal *x,mreal *y,mreal *z);

/******************************************************************************/
/*		Data creation functions												*/
/******************************************************************************/
uintptr_t mgl_datac_real_(uintptr_t *dat);
uintptr_t mgl_datac_imag_(uintptr_t *dat);
uintptr_t mgl_datac_abs_(uintptr_t *dat);
uintptr_t mgl_datac_arg_(uintptr_t *dat);

void mgl_datac_rearrange_(uintptr_t *dat, int *mx, int *my, int *mz);
void mgl_datac_set_float_(uintptr_t *dat, const float *A,int *NX,int *NY,int *NZ);
void mgl_datac_set_double_(uintptr_t *dat, const double *A,int *NX,int *NY,int *NZ);
void mgl_datac_set_complex_(uintptr_t *d, const double *A,int *NX,int *NY,int *NZ);
void mgl_datac_set_(uintptr_t *dat, uintptr_t *a);
void mgl_datac_set_value_(uintptr_t *d, mreal *v, int *i, int *j, int *k);
void mgl_datac_set_values_(uintptr_t *d, const char *val, int *nx, int *ny, int *nz, int l);
void mgl_datac_set_ri_(uintptr_t *dat, uintptr_t *re, uintptr_t *im);
void mgl_datac_set_ap_(uintptr_t *dat, uintptr_t *abs, uintptr_t *phi);

int mgl_datac_read_(uintptr_t *d, const char *fname,int l);
int mgl_datac_read_mat_(uintptr_t *dat, const char *fname, int *dim, int);
int mgl_datac_read_dim_(uintptr_t *dat, const char *fname,int *mx,int *my,int *mz,int);
void mgl_datac_save_(uintptr_t *dat, const char *fname,int *ns,int);
void mgl_data_link_(uintptr_t *d, float *A, int *nx,int *ny,int *nz);
int mgl_data_read_range_(uintptr_t *d, const char *fname, double *from, double *to, double *step, int *as_slice,int l);
int mgl_data_read_all_(uintptr_t *d, const char *fname, int *as_slice,int l);

void mgl_datac_create_(uintptr_t *dat, int *nx,int *ny,int *nz);
void mgl_datac_transpose_(uintptr_t *dat, const char *dim,int);
void mgl_datac_set_id_(uintptr_t *dat, const char *id,int l);
void mgl_datac_fill_(uintptr_t *dat, mreal *x1,mreal *x2,const char *dir,int);
void mgl_datac_fill_eq_(uintptr_t *gr, uintptr_t *dat, const char *eq, uintptr_t *vdat, uintptr_t *wdat,const char *opt, int, int);
void mgl_datac_modify_(uintptr_t *dat, const char *eq,int *dim,int);
void mgl_datac_modify_vw_(uintptr_t *dat, const char *eq, uintptr_t *vdat, uintptr_t *wdat,int);
void mgl_datac_squeeze_(uintptr_t *dat, int *rx,int *ry,int *rz,int *smooth);

void mgl_datac_extend_(uintptr_t *dat, int *n1, int *n2);
void mgl_datac_insert_(uintptr_t *dat, const char *dir, int *at, int *num, int);
void mgl_datac_delete_(uintptr_t *dat, const char *dir, int *at, int *num, int);
/******************************************************************************/
/*		Data manipulation functions											 */
/******************************************************************************/
void mgl_datac_smooth_(uintptr_t *dat, const char *dirs, mreal *delta,int);
void mgl_datac_cumsum_(uintptr_t *dat, const char *dir,int);
void mgl_datac_integral_(uintptr_t *dat, const char *dir,int);
void mgl_datac_diff_(uintptr_t *dat, const char *dir,int);
void mgl_datac_diff2_(uintptr_t *dat, const char *dir,int);
void mgl_datac_swap_(uintptr_t *dat, const char *dir,int);
void mgl_datac_roll_(uintptr_t *dat, const char *dir, int *num, int);
void mgl_datac_mirror_(uintptr_t *dat, const char *dir,int);
void mgl_data_crop_(uintptr_t *dat, int *n1, int *n2, const char *dir,int);

void mgl_datac_hankel_(uintptr_t *dat, const char *dir,int);
void mgl_datac_fft_(uintptr_t *dat, const char *dir,int);
void mgl_datac_sinfft_(uintptr_t *dat, const char *dir,int);
void mgl_datac_cosfft_(uintptr_t *dat, const char *dir,int);

int mgl_datac_read_hdf_(uintptr_t *d, const char *fname, const char *data,int l,int n);
void mgl_datac_save_hdf_(uintptr_t *d, const char *fname, const char *data, int *rewrite,int l,int n);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
