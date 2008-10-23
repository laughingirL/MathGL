/* mgl_data.i is part of Math Graphic Library
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

%module mathgl

%{
#include <math.h>
#include "mgl/mgl_c.h"
%}

struct mglData{};

%extend mglData {
	mglData(int nx=1, int ny=1, int nz=1)
	{	return mgl_create_data_size(nx, ny, nz);	}
	mglData(mglData *a)
	{	HMDT d=mgl_create_data();	mgl_data_set(d,a);	return d;}
	mglData(const char *fname)
	{	HMDT d=mgl_create_data();	mgl_data_read(d, fname);	return d;}
	~mglData()
	{	mgl_delete_data(self);	}

	void Create(int nx,int ny=1,int nz=1)
	{	mgl_data_create(self, nx, ny, nz);	}
	void SetD(double *A,int NX,int NY=1,int NZ=1)
	{	mgl_data_set_double(self, A, NX, NY, NZ);	}
	void SetF(float *A,int NX,int NY=1,int NZ=1)
	{	mgl_data_set_float(self, A, NX, NY, NZ);	}
	void Set(mglData *a)
	{	mgl_data_set(self, a);	}
	void SetV(gsl_vector *v)
	{	mgl_data_set_vector(self, v);	}
	void SetM(gsl_matrix *m)
	{	mgl_data_set_matrix(self, m);	}
	float Get(int i, int j=0, int k=0)
	{	return mgl_data_get_value(self, i, j, k);	}
	void Set(float v, int i, int j=0, int k=0)
	{	mgl_data_set_value(self, v, i, j, k);	}
	void Set(const char *val, int nx, int ny=1, int nz=1)
	{	mgl_data_set_values(self, val, nx, ny, nz);	}

	bool Read(const char *fname, int mx=0, int my=1, int mz=1)
	{	return (mx<=0) ? mgl_data_read(self, fname):mgl_data_read_dim(self, fname, mx, my, mz);	}
	bool ReadMat(const char *fname, int dim)
	{	return mgl_data_read_mat(self, fname, dim);	}
	void Save(const char *fname,int ns=0)
	{	mgl_data_save(self, fname, ns);	}
	void Export(const char *fname, const char *scheme="BbcyrR", float v1=0, float v2=0, int ns=0)
	{	mgl_data_export(self, fname, scheme, v1, v2, ns);	}
	void Import(const char *fname, const char *scheme="BbcyrR", float v1=0, float v2=1)
	{	mgl_data_import(self, fname, scheme, v1, v2);	}

	void Rearrange(int mx, int my=0, int mz=0)
	{	mgl_data_rearrange(self, mx, my, mz);	}
	void Transpose(const char *dim="yxz")
	{	mgl_data_transpose(self, dim);	}
	void Norm(float v1,float v2,bool sym=false,int dim=0)
	{	mgl_data_norm(self, v1, v2, sym, dim);	}
	void NormSl(float v1,float v2,char dir='z',bool keep_en=true,bool sym=false)
	{	mgl_data_norm_slice(self, v1, v2, dir, keep_en, sym);	}
	mglData *SubData(int xx, int yy, int zz)
	{	return mgl_data_subdata(self, xx, yy, zz);	}
	mglData *Column(const char *eq)
	{	return mgl_data_column(self, eq);	}
	void SetId(const char *id)
	{	mgl_data_set_id(self, id);	}
	void Fill(float v1, float v2, char dir='x')
	{	mgl_data_fill(self, v1, v2, dir);	}
	void Fill(mglGraph *gr, const char *eq, mglData *v=NULL, mglData *w=NULL)
	{	mgl_data_fill_eq(gr, self, eq, v, w);	}
	void Modify(const char *eq, mglData *vdat=NULL, mglData *udat=NULL)
	{	mgl_data_modify_vw(self, eq, vdat, udat);	}
	void Squeeze(int rx,int ry=1,int rz=1,bool smooth=true)
	{	mgl_data_squeeze(self, rx, ry, rz, smooth);	}
	void Crop(int n1, int n2, char dir='x')
	{	mgl_data_crop(self, n1, n2, dir);	}
	float Maximal()
	{	return mgl_data_max(self);	}
	float Minimal()
	{	return mgl_data_min(self);	}
	mglData *Combine(mglData *dat2)
	{	return mgl_data_combine(self, dat2);	}
	void Extend(int n1, int n2=0)
	{	mgl_data_extend(self, n1, n2);	}

	mglData *Sum(const char *dir="xyz")
	{	return mgl_data_sum(self, dir);	}
	mglData *Max(const char *dir="xyz")
	{	return mgl_data_max_dir(self, dir);	}
	mglData *Min(const char *dir="xyz")
	{	return mgl_data_min_dir(self, dir);	}

	void Smooth(int Type,float delta=0,const char *dirs="xyz")
	{	mgl_data_smooth(self, Type, delta, dirs);	}
	void CumSum(const char *dir="xyz")
	{	mgl_data_cumsum(self, dir);	}
	void Integral(const char *dir="xyz")
	{	mgl_data_integral(self, dir);	}
	void Diff(const char *dir="xyz")
	{	mgl_data_diff(self, dir);	}
	void Diff2(const char *dir="xyz")
	{	mgl_data_diff2(self, dir);	}
	void Swap(const char *dir="xyz")
	{	mgl_data_swap(self, dir);	}
	void Mirror(const char *dir="xyz")
	{	mgl_data_mirror(self, dir);	}
	void Sew(const char *dir="xyz", double da=2*M_PI)
	{	mgl_data_sew(self, dir,da);	}
	void Envelop(char dir='x')
	{	mgl_data_envelop(self, dir);	}

	float Spline(float x, float y=0, float z=0)
	{	return mgl_data_spline1(self, x, y, z);	}
	float Linear(float x, float y=0, float z=0)
	{	return mgl_data_linear1(self, x, y, z);	}

	mglData *Resize(int mx, int my=1, int mz=1, float x1=0, float x2=1, float y1=0, float y2=1, float z1=0, float z2=1)
	{	return mgl_data_resize_box(self, mx, my, mz, x1, x2, y1, y2, z1, z2);	}
	mglData *Hist(int n, float v1=0, float v2=1, int nsub=0)
	{	return mgl_data_hist(self, n, v1, v2, nsub);	}
	mglData *Hist(mglData *weight, int n, float v1=0, float v2=1, int nsub=0)
	{	return mgl_data_hist_w(self, weight, n, v1, v2, nsub);	}
	mglData *Momentum(char dir, const char *how)
	{	return mgl_data_momentum(self, dir, how);	}
	mglData *Combine(mglData *d)
	{	return mgl_data_combine(self, d);	}
	mglData *Evaluate(mglData *i, bool norm=true)
	{	return mgl_data_evaluate_i(self, i, norm);	}
	mglData *Evaluate(mglData *i, mglData *j, bool norm=true)
	{	return mgl_data_evaluate_ij(self, i, j, norm);	}
	mglData *Evaluate(mglData *i, mglData *j, mglData *k, bool norm=true)
	{	return mgl_data_evaluate_ijk(self, i, j, k, norm);	}

/*	void operator=(mglData a)
	{	return mgl_data_set(a);	}
	void operator*=(mglData d)
	{	return mgl_data_mul_dat(self, d);	}
	void operator/=(mglData d)
	{	return mgl_data_div_dat(self, d);	}
	void operator+=(mglData d)
	{	return mgl_data_add_dat(self, d);	}
	void operator-=(mglData d)
	{	return mgl_data_sub_dat(self, d);	}
	void operator*=(float d)
	{	return mgl_data_mul_num(self, d);	}
	void operator/=(float d)
	{	return mgl_data_div_num(self, d);	}
	void operator+=(float d)
	{	return mgl_data_add_num(self, d);	}
	void operator-=(float d)
	{	return mgl_data_sub_num(self, d);	}*/
}
