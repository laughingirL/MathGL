/***************************************************************************
 * data.h is part of Math Graphic Library
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
#ifndef _MGL_DATA_H_
#define _MGL_DATA_H_
/*****************************************************************************/
#include "mgl/base.h"
/*****************************************************************************/
#ifndef NO_GSL
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#else
struct gsl_vector;
struct gsl_matrix;
#endif
/*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
void mgl_srnd(long seed);
double mgl_rnd();
double mgl_ipow(double x,int n);
/*****************************************************************************/
HMDT mgl_create_data();
HMDT mgl_create_data_size(long nx, long ny, long nz);
HMDT mgl_create_data_file(const char *fname);
void mgl_delete_data(HMDT dat);
const char *mgl_data_info(HCDT dat);
/*****************************************************************************/
uintptr_t mgl_create_data_();
uintptr_t mgl_create_data_size_(int *nx, int *ny, int *nz);
uintptr_t mgl_create_data_file_(const char *fname, int len);
void mgl_delete_data_(uintptr_t *dat);
/*****************************************************************************/
/*		Data creation functions												 */
/*****************************************************************************/
void mgl_data_rearrange(HMDT dat, long mx,long my,long mz);
void mgl_data_link(HMDT dat, float *A,long mx,long my,long mz);
void mgl_data_set_float(HMDT dat, const float *A,long mx,long my,long mz);
void mgl_data_set_double(HMDT dat, const double *A,long mx,long my,long mz);
void mgl_data_set_float2(HMDT d, const float **A,long N1,long N2);
void mgl_data_set_double2(HMDT d, const double **A,long N1,long N2);
void mgl_data_set_float3(HMDT d, const float ***A,long N1,long N2,long N3);
void mgl_data_set_double3(HMDT d, const double ***A,long N1,long N2,long N3);
void mgl_data_set(HMDT dat, HCDT a);
void mgl_data_set_vector(HMDT dat, gsl_vector *v);
void mgl_data_set_matrix(HMDT dat, gsl_matrix *m);
void mgl_data_set_value(HMDT dat, float v, long i, long j, long k);
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
void mgl_data_export(HCDT dat, const char *fname, const char *scheme,float v1,float v2,long ns);
void mgl_data_import(HMDT dat, const char *fname, const char *scheme,float v1,float v2);

void mgl_data_create(HMDT dat, long nx,long ny,long nz);
void mgl_data_transpose(HMDT dat, const char *dim);
void mgl_data_norm(HMDT dat, float v1,float v2,long sym,long dim);
void mgl_data_norm_slice(HMDT dat, float v1,float v2,char dir,long keep_en,long sym);
HMDT mgl_data_subdata(HCDT dat, long xx,long yy,long zz);
HMDT mgl_data_subdata_ext(HCDT dat, HCDT xx, HCDT yy, HCDT zz);
HMDT mgl_data_column(HCDT dat, const char *eq);
void mgl_data_set_id(HMDT d, const char *id);
void mgl_data_fill(HMDT dat, float x1,float x2,char dir);
void mgl_data_fill_eq(HMGL gr, HMDT dat, const char *eq, HCDT vdat, HCDT wdat,const char *opt);
void mgl_data_put_val(HMDT dat, mreal val, long i, long j, long k);
void mgl_data_put_dat(HMDT dat, HCDT val, long i, long j, long k);
void mgl_data_modify(HMDT dat, const char *eq,long dim);
void mgl_data_modify_vw(HMDT dat, const char *eq,HCDT vdat,HCDT wdat);
void mgl_data_squeeze(HMDT dat, long rx,long ry,long rz,long smooth);
mreal mgl_data_max(HCDT dat);
mreal mgl_data_min(HCDT dat);
float *mgl_data_value(HMDT dat, long i,long j,long k);
const float *mgl_data_data(HCDT dat);

float mgl_data_first(HCDT dat, const char *cond, long *i, long *j, long *k);
float mgl_data_last(HCDT dat, const char *cond, long *i, long *j, long *k);
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
/*****************************************************************************/
/*		Data manipulation functions											 */
/*****************************************************************************/
void mgl_data_smooth(HMDT d, const char *dirs, float delta);
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

void mgl_data_hankel(HMDT dat, const char *dir);
void mgl_data_sinfft(HMDT dat, const char *dir);
void mgl_data_cosfft(HMDT dat, const char *dir);
void mgl_data_fill_sample(HMDT dat, long num, const char *how);

float mgl_data_spline(HCDT dat, float x,float y,float z);
float mgl_data_linear(HCDT dat, float x,float y,float z);
HMDT mgl_data_trace(HCDT d);
HMDT mgl_data_resize(HCDT dat, long mx,long my,long mz);
HMDT mgl_data_resize_box(HCDT dat, long mx,long my,long mz,float x1,float x2,float y1,float y2,float z1,float z2);
HMDT mgl_data_hist(HCDT dat, long n, float v1, float v2, long nsub);
HMDT mgl_data_hist_w(HCDT dat, HCDT weight, long n, float v1, float v2, long nsub);
HMDT mgl_data_momentum(HCDT dat, char dir, const char *how);
HMDT mgl_data_evaluate(HCDT dat, HCDT idat, HCDT jdat, HCDT kdat, int norm);
void mgl_data_envelop(HMDT dat, char dir);
void mgl_data_sew(HMDT dat, const char *dirs, float da);
void mgl_data_crop(HMDT dat, long n1, long n2, char dir);
/*****************************************************************************/
/*		Data operations														 */
/*****************************************************************************/
void mgl_data_mul_dat(HMDT dat, HCDT d);
void mgl_data_div_dat(HMDT dat, HCDT d);
void mgl_data_add_dat(HMDT dat, HCDT d);
void mgl_data_sub_dat(HMDT dat, HCDT d);
void mgl_data_mul_num(HMDT dat, float d);
void mgl_data_div_num(HMDT dat, float d);
void mgl_data_add_num(HMDT dat, float d);
void mgl_data_sub_num(HMDT dat, float d);
/*****************************************************************************/
HMDT mgl_pde_solve(HMGL gr, const char *ham, HCDT ini_re, HCDT ini_im, float dz, float k0,const char *opt);
HMDT mgl_qo2d_solve(const char *ham, HCDT ini_re, HCDT ini_im, HCDT ray, float r, float k0, HMDT xx, HMDT yy);
HMDT mgl_ray_trace(const char *ham, float x0, float y0, float z0, float px, float py, float pz, float dt, float tmax);
HMDT mgl_jacobian_2d(HCDT x, HCDT y);
HMDT mgl_jacobian_3d(HCDT x, HCDT y, HCDT z);
HMDT mgl_transform_a(HCDT am, HCDT ph, const char *tr);
HMDT mgl_transform(HCDT re, HCDT im, const char *tr);
void mgl_data_fourier(HMDT re, HMDT im, const char *dir);
HMDT mgl_data_stfa(HCDT re, HCDT im, long dn, char dir);

HMDT mgl_triangulation_3d(HCDT x, HCDT y, HCDT z, float er);
HMDT mgl_triangulation_2d(HCDT x, HCDT y, float er);
/*****************************************************************************/
/*		Data creation functions												 */
/*****************************************************************************/
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
uintptr_t mgl_data_subdata_ext_(uintptr_t *dat, uintptr_t *xx,uintptr_t *yy,uintptr_t *zz);
uintptr_t mgl_data_column_(uintptr_t *dat, const char *eq,int l);
void mgl_data_set_id_(uintptr_t *dat, const char *id,int l);
void mgl_data_fill_(uintptr_t *dat, float *x1,float *x2,const char *dir,int);
void mgl_data_fill_eq_(uintptr_t *gr, uintptr_t *dat, const char *eq, uintptr_t *vdat, uintptr_t *wdat,const char *opt, int, int);
void mgl_data_put_val_(uintptr_t *dat, float *val, int *i, int *j, int *k);
void mgl_data_put_dat_(uintptr_t *dat, uintptr_t *val, int *i, int *j, int *k);
void mgl_data_modify_(uintptr_t *dat, const char *eq,int *dim,int);
void mgl_data_modify_vw_(uintptr_t *dat, const char *eq, uintptr_t *vdat, uintptr_t *wdat,int);
void mgl_data_squeeze_(uintptr_t *dat, int *rx,int *ry,int *rz,int *smooth);
float mgl_data_max_(uintptr_t *dat);
float mgl_data_min_(uintptr_t *dat);
const float *mgl_data_data_(uintptr_t *dat);

float mgl_data_first_(uintptr_t *dat, const char *cond, int *i, int *j, int *k, int);
float mgl_data_last_(uintptr_t *dat, const char *cond, int *i, int *j, int *k, int);
int mgl_data_find_(uintptr_t *dat, const char *cond, char *dir, int *i, int *j, int *k, int,int);
int mgl_data_find_any_(uintptr_t *dat, const char *cond, int);
float mgl_data_max_int_(uintptr_t *dat, int *i, int *j, int *k);
float mgl_data_max_real_(uintptr_t *dat, float *x, float *y, float *z);
float mgl_data_min_int_(uintptr_t *dat, int *i, int *j, int *k);
float mgl_data_min_real_(uintptr_t *dat, float *x, float *y, float *z);
float mgl_data_momentum_val_(uintptr_t *dat, char *dir, float *m, float *w, float *s, float *k,int);

uintptr_t mgl_data_combine_(uintptr_t *dat1, uintptr_t *dat2);
void mgl_data_extend_(uintptr_t *dat, int *n1, int *n2);
void mgl_data_insert_(uintptr_t *dat, const char *dir, int *at, int *num, int);
void mgl_data_delete_(uintptr_t *dat, const char *dir, int *at, int *num, int);
/*****************************************************************************/
/*		Data manipulation functions											 */
/*****************************************************************************/
void mgl_data_smooth_(uintptr_t *dat, const char *dirs, float *delta,int);
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

void mgl_data_hankel_(uintptr_t *dat, const char *dir,int);
void mgl_data_sinfft_(uintptr_t *dat, const char *dir,int);
void mgl_data_cosfft_(uintptr_t *dat, const char *dir,int);
void mgl_data_fill_sample_(uintptr_t *dat, int *num, const char *how,int);

float mgl_data_spline_(uintptr_t *dat, float *x,float *y,float *z);
float mgl_data_linear_(uintptr_t *dat, float *x,float *y,float *z);
uintptr_t mgl_data_trace_(uintptr_t *d);
uintptr_t mgl_data_resize_(uintptr_t *dat, int *mx,int *my,int *mz);
uintptr_t mgl_data_resize_box_(uintptr_t *dat, int *mx,int *my,int *mz,float *x1,float *x2,float *y1,float *y2,float *z1,float *z2);
uintptr_t mgl_data_momentum_(uintptr_t *dat, char *dir, const char *how, int,int);
uintptr_t mgl_data_hist_(uintptr_t *dat, int *n, float *v1, float *v2, int *nsub);
uintptr_t mgl_data_hist_w_(uintptr_t *dat, uintptr_t *weight, int *n, float *v1, float *v2, int *nsub);
uintptr_t mgl_data_evaluate_(uintptr_t *dat, uintptr_t *idat, uintptr_t *jdat, uintptr_t *kdat, int *norm);
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
uintptr_t mgl_pde_solve_(uintptr_t* gr, const char *ham, uintptr_t* ini_re, uintptr_t* ini_im, float *dz, float *k0,const char *opt,int,int);
uintptr_t mgl_qo2d_solve_(const char *ham, uintptr_t* ini_re, uintptr_t* ini_im, uintptr_t* ray, float *r, float *k0, uintptr_t* xx, uintptr_t* yy, int);
uintptr_t mgl_ray_trace_(const char *ham, float *x0, float *y0, float *z0, float *px, float *py, float *pz, float *dt, float *tmax,int);
uintptr_t mgl_jacobian_2d_(uintptr_t* x, uintptr_t* y);
uintptr_t mgl_jacobian_3d_(uintptr_t* x, uintptr_t* y, uintptr_t* z);

uintptr_t mgl_transform_a_(uintptr_t *am, uintptr_t *ph, const char *tr, int);
uintptr_t mgl_transform_(uintptr_t *re, uintptr_t *im, const char *tr, int);
void mgl_data_fourier_(uintptr_t *re, uintptr_t *im, const char *dir, int l);
uintptr_t mgl_data_stfa_(uintptr_t *re, uintptr_t *im, int *dn, char *dir, int);

uintptr_t mgl_triangulation_3d_(uintptr_t *x, uintptr_t *y, uintptr_t *z, float *er);
uintptr_t mgl_triangulation_2d_(uintptr_t *x, uintptr_t *y, float *er);
/*****************************************************************************/
int mgl_data_read_hdf_(uintptr_t *d, const char *fname, const char *data,int l,int n);
void mgl_data_link_(uintptr_t *d, float *A, int *nx,int *ny,int *nz);
void mgl_data_save_hdf_(uintptr_t *d, const char *fname, const char *data, int *rewrite,int l,int n);
int mgl_data_read_range_(uintptr_t *d, const char *fname, double *from, double *to, double *step, int *as_slice,int l);
int mgl_data_read_all_(uintptr_t *d, const char *fname, int *as_slice,int l);

/*****************************************************************************/
#ifdef __cplusplus
}
//-----------------------------------------------------------------------------
#include <vector>
//-----------------------------------------------------------------------------
/// Class for working with data array
class mglData : public mglDataA
{
public:

	long nx;		///< number of points in 1st dimensions ('x' dimension)
	long ny;		///< number of points in 2nd dimensions ('y' dimension)
	long nz;		///< number of points in 3d dimensions ('z' dimension)
	mreal *a;		///< data array
	char *id;		///< column (or slice) names
	bool link;		///< use external data (i.e. don't free it)

	/// Initiate by other mglData variable
	inline mglData(const mglData &d)	{	a=0;	mgl_data_set(this,&d);		}
	inline mglData(const mglDataA *d)	{	a=0;	mgl_data_set(this, d);		}
	inline mglData(bool, mglData *d)	// NOTE: Variable d will be deleted!!!
	{	memcpy(this,d,sizeof(mglData));	d->id=0;	d->link=true;	delete d;	}
	/// Initiate by flat array
	inline mglData(int size, const float *d)	{	a=0;	Set(d,size);	}
	inline mglData(int rows, int cols, const float *d)	{	a=0;	Set(d,cols,rows);	}
	inline mglData(int size, const double *d)	{	a=0;	Set(d,size);	}
	inline mglData(int rows, int cols, const double *d)	{	a=0;	Set(d,cols,rows);	}
	inline mglData(const double *d, int size)	{	a=0;	Set(d,size);	}
	inline mglData(const double *d, int rows, int cols)	{	a=0;	Set(d,cols,rows);	}
	/// Read data from file
	inline mglData(const char *fname)			{	a=0;	Read(fname);	}
	/// Allocate the memory for data array and initialize it zero
	inline mglData(long xx=1,long yy=1,long zz=1)	{	a=0;	Create(xx,yy,zz);	}
	/// Delete the array
	virtual ~mglData()	{	if(id && a)	delete []id;	if(!link && a)	delete []a;	}
	/// Get sizes
	inline long GetNx() const	{	return nx;	}
	inline long GetNy() const	{	return ny;	}
	inline long GetNz() const	{	return nz;	}
	/// Get the value in given cell of the data with border checking
	inline mreal v(long i,long j=0,long k=0) const
	{	i = i>=0 ? (i<nx ? i:nx-1):0;	j = j>=0 ? (j<ny ? j:ny-1):0;
		k = k>=0 ? (k<nz ? k:nz-1):0;	return a[i+nx*(j+ny*k)];	}
	inline mreal vthr(long i) const {	return a[i];	}

	/// Link external data array (don't delete it at exit)
	inline void Link(mreal *A, long NX, long NY=1, long NZ=1)
	{	mgl_data_link(this,A,NX,NY,NZ);	}
	inline void Link(mglData *d)	{	Link(d->a,d->nx,d->ny,d->nz);	}
	/// Allocate memory and copy the data from the gsl_vector
	inline void Set(gsl_vector *v)	{	mgl_data_set_vector(this,v);	}
	/// Allocate memory and copy the data from the gsl_matrix
	inline void Set(gsl_matrix *m)	{	mgl_data_set_matrix(this,m);	}

	/// Allocate memory and copy the data from the (float *) array
	inline void Set(const float *A,long NX,long NY=1,long NZ=1)
	{	mgl_data_set_float(this,A,NX,NY,NZ);	}
	/// Allocate memory and copy the data from the (double *) array
	inline void Set(const double *A,long NX,long NY=1,long NZ=1)
	{	mgl_data_set_double(this,A,NX,NY,NZ);	}
	/// Allocate memory and copy the data from the (float **) array
	inline void Set(const float **A,long N1,long N2)
	{	mgl_data_set_float2(this,A,N1,N2);	}
	/// Allocate memory and copy the data from the (double **) array
	inline void Set(const double **A,long N1,long N2)
	{	mgl_data_set_double2(this,A,N1,N2);	}
	/// Allocate memory and copy the data from the (float ***) array
	inline void Set(const float ***A,long N1,long N2,long N3)
	{	mgl_data_set_float3(this,A,N1,N2,N3);	}
	/// Allocate memory and copy the data from the (double ***) array
	inline void Set(const double ***A,long N1,long N2,long N3)
	{	mgl_data_set_double3(this,A,N1,N2,N3);	}
	/// Allocate memory and scanf the data from the string
	inline void Set(const char *str,long NX,long NY=1,long NZ=1)
	{	mgl_data_set_values(this,str,NX,NY,NZ);	}
	/// Import data from abstract type
	inline void Set(HCDT dat)	{	mgl_data_set(this, dat);	}
	inline void Set(const mglData &dat)	{	mgl_data_set(this, &dat);	}
	/// Allocate memory and copy data from std::vector<T>
	inline void Set(const std::vector<int> &d)
	{	if(d.size()<1)	return;
		Create(d.size());	for(long i=0;i<nx;i++)	a[i] = d[i];	}
	inline void Set(const std::vector<float> &d)
	{	if(d.size()<1)	return;
		Create(d.size());	for(long i=0;i<nx;i++)	a[i] = d[i];	}
	inline void Set(const std::vector<double> &d)
	{	if(d.size()<1)	return;
		Create(d.size());	for(long i=0;i<nx;i++)	a[i] = d[i];	}
	/// Rearange data dimensions
	inline void Rearrange(long mx, long my=0, long mz=0)
	{	mgl_data_rearrange(this,mx,my,mz);	}

	/// Read data from tab-separated text file with auto determining size
	inline bool Read(const char *fname)
	{	return mgl_data_read(this,fname); }
	/// Read data from text file with specifeid size
	inline bool Read(const char *fname,long mx,long my=1,long mz=1)
	{	return mgl_data_read_dim(this,fname,mx,my,mz);	}
	/// Save whole data array (for ns=-1) or only ns-th slice to text file
	inline void Save(const char *fname,long ns=-1) const
	{	mgl_data_save(this,fname,ns);	}
	/// Export data array (for ns=-1) or only ns-th slice to PNG file according color scheme
	inline void Export(const char *fname,const char *scheme,mreal v1=0,mreal v2=0,long ns=-1) const
	{	mgl_data_export(this,fname,scheme,v1,v2,ns);	}
	/// Import data array from PNG file according color scheme
	inline void Import(const char *fname,const char *scheme,mreal v1=0,mreal v2=1)
	{	mgl_data_import(this,fname,scheme,v1,v2);	}
	/// Read data from tab-separated text files with auto determining size which filenames are result of sprintf(fname,templ,t) where t=from:step:to
	inline bool ReadRange(const char *templ, double from, double to, double step=1, bool as_slice=false)
	{	return mgl_data_read_range(this,templ,from,to,step,as_slice);	}
	/// Read data from tab-separated text files with auto determining size which filenames are satisfied to template (like "t_*.dat")
	inline bool ReadAll(const char *templ, bool as_slice=false)
	{	return mgl_data_read_all(this, templ, as_slice);	}
	/// Read data from text file with size specified at beginning of the file
	inline bool ReadMat(const char *fname, long dim=2)
	{	return mgl_data_read_mat(this,fname,dim);	}
	/// Read data array from HDF file (parse HDF4 and HDF5 files)
	inline int ReadHDF(const char *fname,const char *data)
	{	return mgl_data_read_hdf(this,fname,data);	}
	/// Save data to HDF file
	inline void SaveHDF(const char *fname,const char *data,bool rewrite=false) const
	{	mgl_data_save_hdf(this,fname,data,rewrite);	}
	/// Put HDF data names into buf as '\t' separated.
	inline int DatasHDF(const char *fname, char *buf, long size)
	{	return mgl_datas_hdf(fname,buf,size);	}

	/// Create or recreate the array with specified size and fill it by zero
	inline void Create(long mx,long my=1,long mz=1)
	{	mgl_data_create(this,mx,my,mz);	}
	/// Extend data dimensions
	inline void Extend(long n1, long n2=0)
	{	mgl_data_extend(this,n1,n2);	}
	/// Insert data
	inline void Insert(char dir, long at=0, long num=1)
	{	mgl_data_insert(this,dir,at,num);	}
	/// Delete data
	inline void Delete(char dir, long at=0, long num=1)
	{	mgl_data_delete(this,dir,at,num);	}

	/// Transpose dimensions of the data (generalization of Transpose)
	inline void Transpose(const char *dim="yx")
	{	mgl_data_transpose(this,dim);	}
	/// Normalize the data to range [v1,v2]
	inline void Norm(mreal v1=0,mreal v2=1,bool sym=false,long dim=0)
	{	mgl_data_norm(this,v1,v2,sym,dim);	}
	/// Normalize the data to range [v1,v2] slice by slice
	inline void NormSl(mreal v1=0,mreal v2=1,char dir='z',bool keep_en=true,bool sym=false)
	{	mgl_data_norm_slice(this,v1,v2,dir,keep_en,sym);	}
	/// Put value to data element(s)
	inline void Put(mreal val, long i=-1, long j=-1, long k=-1)
	{	mgl_data_put_val(this,val,i,j,k);	}
	/// Put array to data element(s)
	inline void Put(const mglData &dat, long i=-1, long j=-1, long k=-1)
	{	mgl_data_put_dat(this,&dat,i,j,k);	}
	/// Modify the data by specified formula
	inline void Modify(const char *eq,long dim=0)
	{	mgl_data_modify(this, eq, dim);	}
	/// Modify the data by specified formula
	inline void Modify(const char *eq,const mglData &v, const mglData &w)
	{	mgl_data_modify_vw(this,eq,&v,&w);	}
	/// Modify the data by specified formula
	inline void Modify(const char *eq,const mglData &v)
	{	mgl_data_modify_vw(this,eq,&v,0);	}
	/// Modify the data by specified formula assuming x,y,z in range [r1,r2]
	inline void FillEq(mglBase *gr, const char *eq, const mglData *v=0, const mglData *w=0,const char *opt="")
	{	mgl_data_fill_eq(gr,this,eq,v,w,opt);	}
	/// Eqidistantly fill the data to range [x1,x2] in direction \a dir
	inline void Fill(mreal x1,mreal x2,char dir='x')
	{	return mgl_data_fill(this,x1,x2,dir);	}
	/// Set names for columns (slices)
	inline void SetColumnId(const char *ids)
	{	mgl_data_set_id(this,ids);	}
	/// Make new id
	inline void NewId()
	{	delete []id;	id=new char[nx];	memset(id,0,nx*sizeof(char));	}
	/// Reduce size of the data
	inline void Squeeze(long rx,long ry=1,long rz=1,bool smooth=false)
	{	mgl_data_squeeze(this,rx,ry,rz,smooth);	}
	/// Crop the data
	inline void Crop(long n1, long n2,char dir='x')
	{	mgl_data_crop(this,n1,n2,dir);	}

	/// Get maximal value of the data
	inline float Maximal() const	{	return mgl_data_max(this);	}
	/// Get minimal value of the data
	inline float Minimal() const	{	return mgl_data_min(this);	}
	/// Get maximal value of the data and its position
	inline mreal Maximal(long &i,long &j,long &k) const
	{	return mgl_data_max_int(this,&i,&j,&k);	}
	/// Get minimal value of the data and its position
	inline mreal Minimal(long &i,long &j,long &k) const
	{	return mgl_data_min_int(this,&i,&j,&k);	}
	/// Get maximal value of the data and its approximated position
	inline mreal Maximal(mreal &x,mreal &y,mreal &z) const
	{	return mgl_data_max_real(this,&x,&y,&z);	}
	/// Get minimal value of the data and its approximated position
	inline mreal Minimal(mreal &x,mreal &y,mreal &z) const
	{	return mgl_data_min_real(this,&x,&y,&z);	}
	/// Get "energy" and find first (median) and second (width) momentums of data
	inline mreal Momentum(char dir,mreal &m,mreal &w) const
	{	return mgl_data_momentum_val(this,dir,&m,&w,0,0);	}
	/// Get "energy and find 4 momentums of data: median, width, skewness, kurtosis
	inline mreal Momentum(char dir,mreal &m,mreal &w,mreal &s,mreal &k) const
	{	return mgl_data_momentum_val(this,dir,&m,&w,&s,&k);	}
	/// Print information about the data (sizes and momentum) to string
	inline const char *PrintInfo() const	{	return mgl_data_info(this);	}
	/// Print information about the data (sizes and momentum) to FILE (for example, stdout)
	inline void PrintInfo(FILE *fp) const
	{	if(fp)	{	fprintf(fp,"%s",mgl_data_info(this));	fflush(fp);	}	}

	/// Find position (after specified in i,j,k) of first nonzero value of formula
	inline mreal Find(const char *cond, long &i, long &j, long &k) const
	{	return mgl_data_first(this,cond,&i,&j,&k);	}
	/// Find position (before specified in i,j,k) of last nonzero value of formula
	inline mreal Last(const char *cond, long &i, long &j, long &k) const
	{	return mgl_data_last(this,cond,&i,&j,&k);	}
	/// Find position of first in direction 'dir' nonzero value of formula
	inline long Find(const char *cond, char dir, long i=0, long j=0, long k=0) const
	{	return mgl_data_find(this,cond,dir,i,j,k);	}
	/// Find if any nonzero value of formula
	inline bool FindAny(const char *cond) const
	{	return mgl_data_find_any(this,cond);	}

	/// Smooth the data on specified direction or directions
	inline void Smooth(const char *dirs="xyz",mreal delta=0)
	{	mgl_data_smooth(this,dirs,delta);	}
	/// Set as the data envelop
	inline void Envelop(char dir='x')
	{	mgl_data_envelop(this,dir);	}
	/// Remove phase jump
	inline void Sew(const char *dirs="xyz", mreal da=2*M_PI)
	{	mgl_data_sew(this,dirs,da);	}

	/// Get column (or slice) of the data filled by formulas of other named columns
	inline mglData Column(const char *eq) const
	{	return mglData(true,mgl_data_column(this,eq));	}
	/// Get momentum (1D-array) of data along direction 'dir'. String looks like "x1" for median in x-direction, "x2" for width in x-dir and so on.
	inline mglData Momentum(char dir, const char *how) const
	{	return mglData(true,mgl_data_momentum(this,dir,how));	}
	/// Get sub-array of the data with given fixed indexes
	inline mglData SubData(long xx,long yy=-1,long zz=-1) const
	{	return mglData(true,mgl_data_subdata(this,xx,yy,zz));	}
	inline mglData SubData(const mglData &xx, const mglData &yy, const mglData &zz) const
	{	return mglData(true,mgl_data_subdata_ext(this,&xx,&yy,&zz));	}
	/// Get trace of the data array
	inline mglData Trace() const
	{	return mglData(true,mgl_data_trace(this));	}
	/// Create n-th points distribution of this data values in range [v1, v2]
	mglData Hist(long n,mreal v1=0,mreal v2=1, long nsub=0) const
	{	return mglData(true,mgl_data_hist(this,n,v1,v2,nsub));	}
	/// Create n-th points distribution of this data values in range [v1, v2] with weight \a w
	mglData Hist(const mglData &w, long n,mreal v1=0,mreal v2=1, long nsub=0) const
	{	return mglData(true,mgl_data_hist_w(this,&w,n,v1,v2,nsub));	}
	/// Get array which is result of summation in given direction or directions
	inline mglData Sum(const char *dir) const
	{	return mglData(true,mgl_data_sum(this,dir));	}
	/// Get array which is result of maximal values in given direction or directions
	inline mglData Max(const char *dir) const
	{	return mglData(true,mgl_data_max_dir(this,dir));	}
	/// Get array which is result of minimal values in given direction or directions
	inline mglData Min(const char *dir) const
	{	return mglData(true,mgl_data_min_dir(this,dir));	}
	/// Resize the data to new size of box [x1,x2]*[y1,y2]*[z1,z2]
	inline mglData Resize(long mx,long my=1,long mz=1, mreal x1=0,mreal x2=1, mreal y1=0,mreal y2=1, mreal z1=0,mreal z2=1) const
	{	return mglData(true,mgl_data_resize_box(this,mx,my,mz,x1,x2,y1,y2,z1,z2));	}
	/// Get the data which is direct multiplication (like, d[i,j] = this[i]*a[j] and so on)
	inline mglData Combine(const mglData &dat) const
	{	return mglData(true,mgl_data_combine(this,&dat));	}
	/// Get array which values is result of interpolation this for coordinates from other arrays
	inline mglData Evaluate(const mglData &idat, bool norm=true) const
	{	return mglData(true,mgl_data_evaluate(this,&idat,0,0,norm));	}
	inline mglData Evaluate(const mglData &idat, const mglData &jdat, bool norm=true) const
	{	return mglData(true,mgl_data_evaluate(this,&idat,&jdat,0,norm));	}
	inline mglData Evaluate(const mglData &idat, const mglData &jdat, const mglData &kdat, bool norm=true) const
	{	return mglData(true,mgl_data_evaluate(this,&idat,&jdat,&kdat,norm));	}

	/// Cumulative summation the data in given direction or directions
	inline void CumSum(const char *dir)	{	mgl_data_cumsum(this,dir);	}
	/// Integrate (cumulative summation) the data in given direction or directions
	inline void Integral(const char *dir)	{	mgl_data_integral(this,dir);	}
	/// Differentiate the data in given direction or directions
	inline void Diff(const char *dir)	{	mgl_data_diff(this,dir);	}
	/// Differentiate the parametrically specified data along direction v1 with v2=const
	inline void Diff(const mglData &v1, const mglData &v2)
	{	mgl_data_diff_par(this,&v1,&v2,0);	}
	/// Differentiate the parametrically specified data along direction v1 with v2,v3=const
	inline void Diff(const mglData &v1, const mglData &v2, const mglData &v3)
	{	mgl_data_diff_par(this,&v1,&v2,&v3);	}
	/// Double-differentiate (like laplace operator) the data in given direction
	inline void Diff2(const char *dir)	{	mgl_data_diff2(this,dir);	}
	/// Swap left and right part of the data in given direction (useful for fourier spectrums)
	inline void Swap(const char *dir)	{	mgl_data_swap(this,dir);	}
	/// Roll data along direction \a dir by \a num slices
	inline void Roll(char dir, long num)	{	mgl_data_roll(this,dir,num);	}
	/// Mirror the data in given direction (useful for fourier spectrums)
	inline void Mirror(const char *dir)	{	mgl_data_mirror(this,dir);	}

	/// Hankel transform
	inline void Hankel(const char *dir)	{	mgl_data_hankel(this,dir);	}
	/// Sin-Fourier transform
	inline void SinFFT(const char *dir)	{	mgl_data_sinfft(this,dir);	}
	/// Cos-Fourier transform
	inline void CosFFT(const char *dir)	{	mgl_data_cosfft(this,dir);	}
	/// Fill data by 'x'/'k' samples for Hankel ('h') or Fourier ('f') transform
	inline void FillSample(long n, const char *how)
	{	mgl_data_fill_sample(this,n,how);	}

	/// Interpolate by qubic splain the data to given point x=[0...nx-1], y=[0...ny-1], z=[0...nz-1]
	inline mreal Spline(mreal x,mreal y=0,mreal z=0) const
	{	return mgl_data_spline(this, x,y,z);	}
	/// Interpolate by qubic splain the data to given point \a x,\a y,\a z which normalized in range [0, 1]
	inline mreal Spline1(mreal x,mreal y=0,mreal z=0) const
	{	return mgl_data_spline(this, x*(nx-1),y*(ny-1),z*(nz-1));	}
	/// Interpolate by linear function the data to given point x=[0...nx-1], y=[0...ny-1], z=[0...nz-1]
	inline mreal Linear(mreal x,mreal y=0,mreal z=0)	const
	{	return mgl_data_linear(this,x,y,z);	}
	/// Interpolate by line the data to given point \a x,\a y,\a z which normalized in range [0, 1]
	inline mreal Linear1(mreal x,mreal y=0,mreal z=0) const
	{	return mgl_data_linear(this,x*(nx-1),y*(ny-1),z*(nz-1));	}

	/// Copy data from other mglData variable
	inline mglData &operator=(const mglData &d)
	{	if(this!=&d)	Set(d.a,d.nx,d.ny,d.nz);	return *this;	}
	inline mglData &operator=(mreal v)
	{	for(long i=0;i<nx*ny*nz;i++)	a[i]=v;		return *this;	}
	/// Multiplicate the data by other one for each element
	inline void operator*=(const mglData &d)	{	mgl_data_mul_dat(this,&d);	}
	/// Divide the data by other one for each element
	inline void operator/=(const mglData &d)	{	mgl_data_div_dat(this,&d);	}
	/// Add the other data
	inline void operator+=(const mglData &d)	{	mgl_data_add_dat(this,&d);	}
	/// Substract the other data
	inline void operator-=(const mglData &d)	{	mgl_data_sub_dat(this,&d);	}
	/// Multiplicate each element by the number
	inline void operator*=(mreal d)		{	mgl_data_mul_num(this,d);	}
	/// Divide each element by the number
	inline void operator/=(mreal d)		{	mgl_data_div_num(this,d);	}
	/// Add the number
	inline void operator+=(mreal d)		{	mgl_data_add_num(this,d);	}
	/// Substract the number
	inline void operator-=(mreal d)		{	mgl_data_sub_num(this,d);	}
	// NOTE see 13.10 for operator(), operator[] -- m.b. I should add it ???
};
//-----------------------------------------------------------------------------
inline mglData operator*(const mglData &b, const mglData &d)
{	mglData a(b);	a*=d;	return a;	}
inline mglData operator*(mreal b, const mglData &d)
{	mglData a(d);	a*=b;	return a;	}
inline mglData operator*(const mglData &d, mreal b)
{	mglData a(d);	a*=b;	return a;	}
inline mglData operator-(const mglData &b, const mglData &d)
{	mglData a(b);	a-=d;	return a;	}
inline mglData operator-(mreal b, const mglData &d)
{	mglData a(d);	a-=b;	return a;	}
inline mglData operator-(const mglData &d, mreal b)
{	mglData a(d);	a-=b;	return a;	}
inline mglData operator+(const mglData &b, const mglData &d)
{	mglData a(b);	a+=d;	return a;	}
inline mglData operator+(mreal b, const mglData &d)
{	mglData a(d);	a+=b;	return a;	}
inline mglData operator+(const mglData &d, mreal b)
{	mglData a(d);	a+=b;	return a;	}
inline mglData operator/(const mglData &b, const mglData &d)
{	mglData a(b);	a/=d;	return a;	}
inline mglData operator/(const mglData &d, mreal b)
{	mglData a(d);	a/=b;	return a;	}
inline bool operator==(const mglData &b, const mglData &d)
{	if(b.nx!=d.nx || b.ny!=d.ny || b.ny!=d.ny)	return false;
	return !memcmp(b.a,d.a,b.nx*b.ny*b.nz*sizeof(mreal));	}
//-----------------------------------------------------------------------------
mreal mglLinear(const mreal *a, long nx, long ny, long nz, mreal x, mreal y, mreal z);
mreal mglSpline3(const mreal *a, long nx, long ny, long nz, mreal x, mreal y, mreal z,mreal *dx=0, mreal *dy=0, mreal *dz=0);
//-----------------------------------------------------------------------------
/// Integral data transformation (like Fourier 'f' or 'i', Hankel 'h' or None 'n') for amplitude and phase
inline mglData mglTransformA(const mglDataA &am, const mglDataA &ph, const char *tr)
{	return mgl_transform_a(&am,&ph,tr);	}
/// Integral data transformation (like Fourier 'f' or 'i', Hankel 'h' or None 'n') for mreal and imaginary parts
inline mglData mglTransform(const mglDataA &re, const mglDataA &im, const char *tr)
{	return mgl_transform(&re,&im,tr);	}
/// Apply Fourier transform for the data and save result into it
inline void mglFourier(mglData &re, mglData &im, const char *dir)
{	mgl_data_fourier(&re,&im,dir);	}
/// Short time fourier analysis for mreal and imaginary parts. Output is amplitude of partial fourier (result will have size {dn, floor(nx/dn), ny} for dir='x'
inline mglData mglSTFA(const mglDataA &re, const mglDataA &im, long dn, char dir='x')
{	return mglData(true, mgl_data_stfa(&re,&im,dn,dir));	}
//-----------------------------------------------------------------------------
/// Saves result of PDE solving (|u|^2) for "Hamiltonian" \a ham with initial conditions \a ini
inline mglData mglPDE(mglBase *gr, const char *ham, const mglData &ini_re, const mglData &ini_im, mreal dz=0.1, mreal k0=100,const char *opt="")
{	return mglData(true, mgl_pde_solve(gr,ham, &ini_re, &ini_im, dz, k0,opt));	}
/// Saves result of PDE solving for "Hamiltonian" \a ham with initial conditions \a ini along a curve \a ray (must have nx>=7 - x,y,z,px,py,pz,tau or nx=5 - x,y,px,py,tau)
inline mglData mglQO2d(const char *ham, const mglData &ini_re, const mglData &ini_im, const mglData &ray, mreal r=1, mreal k0=100, mglData *xx=0, mglData *yy=0)
{	return mglData(true, mgl_qo2d_solve(ham, &ini_re, &ini_im, &ray, r, k0, xx, yy));	}
/// Prepares ray data for mglQO_PDE with starting point \a r0, \a p0
inline mglData mglRay(const char *ham, mglPoint r0, mglPoint p0, mreal dt=0.1, mreal tmax=10)
{	return mglData(true, mgl_ray_trace(ham, r0.x, r0.y, r0.z, p0.x, p0.y, p0.z, dt, tmax));	}
/// Calculate Jacobian determinant for D{x(u,v), y(u,v)} = dx/du*dy/dv-dx/dv*dy/du
inline mglData mglJacobian(const mglData &x, const mglData &y)
{	return mglData(true, mgl_jacobian_2d(&x, &y));	}
/// Calculate Jacobian determinant for D{x(u,v,w), y(u,v,w), z(u,v,w)}
inline mglData mglJacobian(const mglData &x, const mglData &y, const mglData &z)
{	return mglData(true, mgl_jacobian_3d(&x, &y, &z));	}
// Do something like Delone triangulation
inline mglData mglTriangulation(const mglData &x, const mglData &y, const mglData &z, float er=0)
{	return mglData(true,mgl_triangulation_3d(&x,&y,&z,er));	}
inline mglData mglTriangulation(const mglData &x, const mglData &y, float er=0)
{	return mglData(true,mgl_triangulation_2d(&x,&y,er));	}
//-----------------------------------------------------------------------------
#endif
#endif
