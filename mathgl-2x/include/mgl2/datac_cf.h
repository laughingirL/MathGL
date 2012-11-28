/***************************************************************************
 * data_cf.h is part of Math Graphic Library
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
#ifndef _MGL_DATAC_CF_H_
#define _MGL_DATAC_CF_H_
//-----------------------------------------------------------------------------
#include "mgl2/base.h"
//-----------------------------------------------------------------------------
#if MGL_HAVE_GSL
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#else
struct gsl_vector;
struct gsl_matrix;
#endif
//-----------------------------------------------------------------------------
#ifdef __cplusplus
class mglDataC;
typedef mglDataC* HADT;
extern "C" {
#else
typedef void *HADT;
#endif

/// Create HMDT object
HADT mgl_create_datac();
uintptr_t mgl_create_datac_();
/// Create HMDT object with specified sizes
HADT mgl_create_datac_size(long nx, long ny, long nz);
uintptr_t mgl_create_datac_size_(int *nx, int *ny, int *nz);
/// Create HMDT object with data from file
HADT mgl_create_datac_file(const char *fname);
uintptr_t mgl_create_datac_file_(const char *fname, int len);
/// Delete HMDT object
void mgl_delete_datac(HADT dat);
void mgl_delete_datac_(uintptr_t *dat);

/// Rearange data dimensions
void mgl_datac_rearrange(HADT dat, long mx,long my,long mz);
void mgl_datac_rearrange_(uintptr_t *dat, int *mx, int *my, int *mz);
/// Link external data array (don't delete it at exit)
void mgl_datac_link(HADT dat, dual *A,long mx,long my,long mz);
void mgl_datac_link_(uintptr_t *d, dual *A, int *nx,int *ny,int *nz);
/// Allocate memory and copy the data from the (float *) array
void mgl_datac_set_float(HADT dat, const float *A,long mx,long my,long mz);
void mgl_datac_set_float_(uintptr_t *dat, const float *A,int *NX,int *NY,int *NZ);
/// Allocate memory and copy the data from the (double *) array
void mgl_datac_set_double(HADT dat, const double *A,long mx,long my,long mz);
void mgl_datac_set_double_(uintptr_t *dat, const double *A,int *NX,int *NY,int *NZ);
/// Allocate memory and copy the data from the (dual *) array
void mgl_datac_set_complex(HADT dat, const dual *A,long mx,long my,long mz);
void mgl_datac_set_complex_(uintptr_t *d, const dual *A,int *NX,int *NY,int *NZ);
/// Import data from abstract type
void mgl_datac_set(HADT dat, HCDT a);
void mgl_datac_set_(uintptr_t *dat, uintptr_t *a);
/// Allocate memory and copy the data from the gsl_vector
void mgl_datac_set_vector(HADT dat, gsl_vector *v);
/// Allocate memory and copy the data from the gsl_matrix
void mgl_datac_set_matrix(HADT dat, gsl_matrix *m);
/// Set value of data element [i,j,k]
void mgl_datac_set_value(HADT dat, dual v, long i, long j, long k);
void mgl_datac_set_value_(uintptr_t *d, mreal *v, int *i, int *j, int *k);
/// Get value of data element [i,j,k]
dual mgl_datac_get_value(HCDT dat, long i, long j, long k);
/// Allocate memory and scanf the data from the string
void mgl_datac_set_values(HADT dat, const char *val, long nx, long ny, long nz);
void mgl_datac_set_values_(uintptr_t *d, const char *val, int *nx, int *ny, int *nz, int l);

/// Returns pointer to internal data array
dual *mgl_datac_data(HADT dat);
/// Returns pointer to data element [i,j,k]
dual *mgl_datac_value(HADT dat, long i,long j,long k);

/// Set the data from HCDT objects for real and imaginary parts
void mgl_datac_set_ri(HADT dat, HCDT re, HCDT im);
void mgl_datac_set_ri_(uintptr_t *dat, uintptr_t *re, uintptr_t *im);
/// Set the data from HCDT objects as amplitude and phase of complex data
void mgl_datac_set_ap(HADT dat, HCDT abs, HCDT phi);
void mgl_datac_set_ap_(uintptr_t *dat, uintptr_t *abs, uintptr_t *phi);

/// Read data from tab-separated text file with auto determining size
int mgl_datac_read(HADT dat, const char *fname);
int mgl_datac_read_(uintptr_t *d, const char *fname,int l);
/// Read data from text file with size specified at beginning of the file
int mgl_datac_read_mat(HADT dat, const char *fname, long dim);
int mgl_datac_read_mat_(uintptr_t *dat, const char *fname, int *dim, int);
/// Read data from text file with specifeid size
int mgl_datac_read_dim(HADT dat, const char *fname,long mx,long my,long mz);
int mgl_datac_read_dim_(uintptr_t *dat, const char *fname,int *mx,int *my,int *mz,int);
/// Read data from tab-separated text files with auto determining size which filenames are result of sprintf(fname,templ,t) where t=from:step:to
int mgl_datac_read_range(HADT d, const char *templ, double from, double to, double step, int as_slice);
int mgl_data_read_range_(uintptr_t *d, const char *fname, mreal *from, mreal *to, mreal *step, int *as_slice,int l);
/// Read data from tab-separated text files with auto determining size which filenames are satisfied to template (like "t_*.dat")
int mgl_datac_read_all(HADT dat, const char *templ, int as_slice);
int mgl_data_read_all_(uintptr_t *d, const char *fname, int *as_slice,int l);
/// Save whole data array (for ns=-1) or only ns-th slice to text file
void mgl_datac_save(HCDT dat, const char *fname,long ns);
void mgl_datac_save_(uintptr_t *dat, const char *fname,int *ns,int);

/// Read data array from HDF file (parse HDF4 and HDF5 files)
int mgl_datac_read_hdf(HADT d,const char *fname,const char *data);
int mgl_datac_read_hdf_(uintptr_t *d, const char *fname, const char *data,int l,int n);
/// Save data to HDF file
void mgl_datac_save_hdf(HCDT d,const char *fname,const char *data,int rewrite);
void mgl_datac_save_hdf_(uintptr_t *d, const char *fname, const char *data, int *rewrite,int l,int n);

/// Create or recreate the array with specified size and fill it by zero
void mgl_datac_create(HADT dat, long nx,long ny,long nz);
void mgl_datac_create_(uintptr_t *dat, int *nx,int *ny,int *nz);
/// Transpose dimensions of the data (generalization of Transpose)
void mgl_datac_transpose(HADT dat, const char *dim);
void mgl_datac_transpose_(uintptr_t *dat, const char *dim,int);
/// Set names for columns (slices)
void mgl_datac_set_id(HADT d, const char *id);
void mgl_datac_set_id_(uintptr_t *dat, const char *id,int l);
/// Equidistantly fill the data to range [x1,x2] in direction dir
void mgl_datac_fill(HADT dat, dual x1,dual x2,char dir);
void mgl_datac_fill_(uintptr_t *dat, mreal *x1,mreal *x2,const char *dir,int);
/// Modify the data by specified formula assuming x,y,z in range [r1,r2]
void mgl_datac_fill_eq(HMGL gr, HADT dat, const char *eq, HCDT vdat, HCDT wdat,const char *opt);
void mgl_datac_fill_eq_(uintptr_t *gr, uintptr_t *dat, const char *eq, uintptr_t *vdat, uintptr_t *wdat,const char *opt, int, int);
/// Modify the data by specified formula
void mgl_datac_modify(HADT dat, const char *eq,long dim);
void mgl_datac_modify_(uintptr_t *dat, const char *eq,int *dim,int);
/// Modify the data by specified formula
void mgl_datac_modify_vw(HADT dat, const char *eq,HCDT vdat,HCDT wdat);
void mgl_datac_modify_vw_(uintptr_t *dat, const char *eq, uintptr_t *vdat, uintptr_t *wdat,int);

/// Reduce size of the data
void mgl_datac_squeeze(HADT dat, long rx,long ry,long rz,long smooth);
void mgl_datac_squeeze_(uintptr_t *dat, int *rx,int *ry,int *rz,int *smooth);
/// Extend data dimensions
void mgl_datac_extend(HADT dat, long n1, long n2);
void mgl_datac_extend_(uintptr_t *dat, int *n1, int *n2);
/// Insert data rows/columns/slices
void mgl_datac_insert(HADT dat, char dir, long at, long num);
void mgl_datac_insert_(uintptr_t *dat, const char *dir, int *at, int *num, int);
/// Delete data rows/columns/slices
void mgl_datac_delete(HADT dat, char dir, long at, long num);
void mgl_datac_delete_(uintptr_t *dat, const char *dir, int *at, int *num, int);

/// Smooth the data on specified direction or directions
void mgl_datac_smooth(HADT d, const char *dirs, mreal delta);
void mgl_datac_smooth_(uintptr_t *dat, const char *dirs, mreal *delta,int);
/// Cumulative summation the data in given direction or directions
void mgl_datac_cumsum(HADT dat, const char *dir);
void mgl_datac_cumsum_(uintptr_t *dat, const char *dir,int);
/// Integrate (cumulative summation) the data in given direction or directions
void mgl_datac_integral(HADT dat, const char *dir);
void mgl_datac_integral_(uintptr_t *dat, const char *dir,int);
/// Differentiate the data in given direction or directions
void mgl_datac_diff(HADT dat, const char *dir);
void mgl_datac_diff_(uintptr_t *dat, const char *dir,int);
/// Double-differentiate (like Laplace operator) the data in given direction
void mgl_datac_diff2(HADT dat, const char *dir);
void mgl_datac_diff2_(uintptr_t *dat, const char *dir,int);
/// Swap left and right part of the data in given direction (useful for Fourier spectrum)
void mgl_datac_swap(HADT dat, const char *dir);
void mgl_datac_swap_(uintptr_t *dat, const char *dir,int);
/// Roll data along direction dir by num slices
void mgl_datac_roll(HADT dat, char dir, long num);
void mgl_datac_roll_(uintptr_t *dat, const char *dir, int *num, int);
/// Mirror the data in given direction (useful for Fourier spectrum)
void mgl_datac_mirror(HADT dat, const char *dir);
void mgl_datac_mirror_(uintptr_t *dat, const char *dir,int);
/// Crop the data
void mgl_datac_crop(HADT dat, long n1, long n2, char dir);
void mgl_data_crop_(uintptr_t *dat, int *n1, int *n2, const char *dir,int);

/// Apply Hankel transform
void mgl_datac_hankel(HADT dat, const char *dir);
void mgl_datac_hankel_(uintptr_t *dat, const char *dir,int);
/// Apply Fourier transform
void mgl_datac_fft(HADT dat, const char *dir);
void mgl_datac_fft_(uintptr_t *dat, const char *dir,int);

HMDT mgl_datac_real(HCDT dat);
uintptr_t mgl_datac_real_(uintptr_t *dat);
HMDT mgl_datac_imag(HCDT dat);
uintptr_t mgl_datac_imag_(uintptr_t *dat);
HMDT mgl_datac_abs(HCDT dat);
uintptr_t mgl_datac_abs_(uintptr_t *dat);
HMDT mgl_datac_arg(HCDT dat);
uintptr_t mgl_datac_arg_(uintptr_t *dat);

/// Interpolate by linear function the data to given point x=[0...nx-1], y=[0...ny-1], z=[0...nz-1]
dual mgl_datac_linear(HCDT d, mreal x,mreal y,mreal z);
dual mgl_datac_linear_(uintptr_t *d, mreal *x,mreal *y,mreal *z);
/// Interpolate by linear function the data and return its derivatives at given point x=[0...nx-1], y=[0...ny-1], z=[0...nz-1]
dual mgl_datac_linear_ext(HCDT d, mreal x,mreal y,mreal z, dual *dx,dual *dy,dual *dz);
dual mgl_datac_linear_ext_(uintptr_t *d, mreal *x,mreal *y,mreal *z, dual *dx,dual *dy,dual *dz);

#ifdef __cplusplus
}
#endif
//-----------------------------------------------------------------------------
#endif
