/***************************************************************************
 * mgl_data_w.h is part of Math Graphic Library
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
#ifndef _MGL_DATA_W_H_
#define _MGL_DATA_W_H_
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
//-----------------------------------------------------------------------------
#ifndef NO_GSL
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#else
struct gsl_vector;
struct gsl_matrix;
#endif
#include "mgl/mgl_define.h"
#include "mgl/mgl_c.h"
struct mglData;
typedef mglData* HMDT;
//-----------------------------------------------------------------------------
/// Class for working with data array
class mglDataW
{
public:
	HMDT a;
	mglDataW(int xx=1,int yy=1,int zz=1)	{	a=mgl_create_data_size(xx,yy,zz);	};
	mglDataW(HMDT aa)	{	a=aa;	};
	mglDataW(const char *fname)	{	a=mgl_create_data_file(fname);	}
	~mglDataW()	{	if(a)	mgl_delete_data(a);	};
	mglDataW(const mglDataW &d)	{	a=mgl_create_data();	mgl_data_set(a,d.a);	};
	mglDataW(int size, const float *d)
	{	a=mgl_create_data();	mgl_data_set_float(a,d,size,1,1);	};
	mglDataW(int rows, int cols, const float *d)
	{	a=mgl_create_data();	mgl_data_set_float(a,d,cols,rows,1);	};
	mglDataW(int size, const double *d)
	{	a=mgl_create_data();	mgl_data_set_double(a,d,size,1,1);	};
	mglDataW(int rows, int cols, const double *d)
	{	a=mgl_create_data();	mgl_data_set_double(a,d,cols,rows,1);	};
	// ~~~~~~~~~~~~~~~~~~~~ операции ~~~~~~~~~~~~~~~~~~~~
	inline void Set(gsl_vector *v)	{	mgl_data_set_vector(a,v);	};
	inline void Set(gsl_matrix *m)	{	mgl_data_set_matrix(a,m);	};
	inline void Set(const mglDataW &dat)	{	mgl_data_set(a,dat.a);	};
	inline void Set(const float *A,int NX,int NY=1,int NZ=1)	{	mgl_data_set_float(a,A,NX,NY,NZ);	};
	inline void Set(const double *A,int NX,int NY=1,int NZ=1)	{	mgl_data_set_double(a,A,NX,NY,NZ);	};
	inline void Set(const float **A,int N1,int N2)				{	mgl_data_set_float2(a,A,N1,N2);		};
	inline void Set(const double **A,int N1,int N2)				{	mgl_data_set_double2(a,A,N1,N2);	};
	inline void Set(const float ***A,int N1,int N2,int N3)		{	mgl_data_set_float3(a,A,N1,N2,N3);	};
	inline void Set(const double ***A,int N1,int N2,int N3)		{	mgl_data_set_double3(a,A,N1,N2,N3);	};
	inline void Rearrange(int mx, int my=0, int mz=0)			{	mgl_data_rearrange(a,mx,my,mz);	};
	void Set(const char *str,int NX,int NY=1,int NZ=1)	{	mgl_data_set_values(a,str,NX,NY,NZ);	};

	bool Read(const char *fname)	{	return mgl_data_read(a,fname);	};
	bool Read(const char *fname,int mx,int my=1,int mz=1)
	{	return mgl_data_read_dim(a,fname,mx,my,mz);	};
	void Save(const char *fname,int ns=-1)	{	mgl_data_save(a,fname,ns);	};
	void Export(const char *fname,const char *scheme,mreal v1=0,mreal v2=0,int ns=-1)
	{	mgl_data_export(a,fname,scheme,v1,v2,ns);	};
	void Import(const char *fname,const char *scheme,mreal v1=0,mreal v2=1)
	{	mgl_data_import(a,fname,scheme,v1,v2);	};
	bool ReadMat(const char *fname, int dim=2)
	{	return mgl_data_read_mat(a,fname,dim);	};

	void Create(int nx,int ny=1,int nz=1)	{	mgl_data_create(a,nx,ny,nz);	};
	void Extend(int n1, int n2=0)			{	mgl_data_extend(a,n1,n2);	};
	void Transpose(const char *dim="yx")	{	mgl_data_transpose(a,dim);	};
	void Norm(mreal v1=0,mreal v2=1,bool sym=false,int dim=0)
	{	mgl_data_norm(a,v1,v2,sym,dim);	};
	void NormSl(mreal v1=0,mreal v2=1,char dir='z',bool keep_en=true,bool sym=false)
	{	mgl_data_norm_slice(a,v1,v2,dir,keep_en,sym);	};
	void Put(mreal val, int i=-1, int j=-1, int k=-1)
	{	mgl_data_put_val(a, val, i, j, k);	};
	void Put(mglDataW &val, int i=-1, int j=-1, int k=-1)
	{	mgl_data_put_dat(a, val.a, i,j,k);	};
	void Modify(const char *eq,int dim=0)	{	mgl_data_modify(a,eq,dim);	};
	void Modify(const char *eq, mglDataW &v, mglDataW &w)
	{	mgl_data_modify_vw(a,eq,v.a, w.a);	};
	void Modify(const char *eq,const mglDataW &v){	mgl_data_modify_vw(a,eq,v.a, NULL);	};
	void Fill(mreal x1,mreal x2,char dir='x')	{	mgl_data_fill(a,x1,x2,dir);	};
	mglDataW Column(const char *eq)	{	return mglDataW(mgl_data_column(a,eq));	};
	void SetColumnId(const char *ids)			{	mgl_data_set_id(a,ids);	};
	void Squeeze(int rx,int ry=1,int rz=1,bool smooth=false)
	{	mgl_data_squeeze(a,rx,ry,rz,smooth);	};
	void Crop(int n1, int n2,char dir='x')		{	mgl_data_crop(a,n1,n2,dir);	}

	mreal Maximal()	{	return mgl_data_max(a);	};
	mreal Minimal()	{	return mgl_data_min(a);	};
	mglDataW Momentum(char dir, const char *how)
	{	return mglDataW(mgl_data_momentum(a, dir, how));	}

	void Smooth(int Type,const char *dirs="xyz",mreal delta=0)
	{	mgl_data_smooth(a,Type,delta,dirs);	};
	void Envelop(char dir='x')	{	mgl_data_envelop(a,dir);	};
	void Sew(const char *dirs="xyz", mreal da=2*M_PI)
	{	mgl_data_sew(a,dirs,da);	};
	void Smooth(const char *dirs="xyz")
	{
		int type = SMOOTH_QUAD_5;
		if(strchr(dirs,'0') || strchr(dirs,'1'))	return;
		if(strchr(dirs,'3'))	type = SMOOTH_LINE_3;
		if(strchr(dirs,'5'))	type = SMOOTH_LINE_5;
		mgl_data_smooth(a,type,0,dirs);
	}
	mglDataW SubData(int xx,int yy=-1,int zz=-1)
	{	return mglDataW(mgl_data_subdata(a,xx,yy,zz));	};
	mglDataW SubData(const mglDataW &xx, const mglDataW &yy, const mglDataW &zz)
	{	return mglDataW(mgl_data_subdata_ext(a,xx.a,yy.a,zz.a));	};
	mglDataW Hist(int n,mreal v1=0,mreal v2=1, int nsub=0)
	{	return mglDataW(mgl_data_hist(a,n,v1,v2,nsub));	};
	mglDataW Hist(const mglDataW &w, int n,mreal v1=0,mreal v2=1, int nsub=0)
	{	return mglDataW(mgl_data_hist_w(a,w.a,n,v1,v2,nsub));	};
	mglDataW Sum(const char *dir)
	{	return mglDataW(mgl_data_sum(a,dir));	};
	mglDataW Max(const char *dir)
	{	return mglDataW(mgl_data_max_dir(a,dir));	};
	mglDataW Min(const char *dir)
	{	return mglDataW(mgl_data_min_dir(a,dir));	};
	mglDataW Resize(int mx,int my=1,int mz=1,mreal x1=0,mreal x2=1,
		mreal y1=0,mreal y2=1,mreal z1=0,mreal z2=1)
	{	return mglDataW(mgl_data_resize_box(a, mx, my, mz, x1, x2, y1, y2, z1, z2));	};
	mglDataW Combine(const mglDataW &d)
	{	return mglDataW(mgl_data_combine(a,d.a));	};
	mglDataW Evaluate(const mglDataW &idat, bool norm=true)
	{	return mglDataW(mgl_data_evaluate_i(a, idat.a, norm));	};
	mglDataW Evaluate(const mglDataW &idat, const mglDataW &jdat, bool norm=true)
	{	return mglDataW(mgl_data_evaluate_ij(a, idat.a, jdat.a, norm));	};
	mglDataW Evaluate(const mglDataW &idat, const mglDataW &jdat, const mglDataW &kdat, bool norm=true)
	{	return mglDataW(mgl_data_evaluate_ijk(a, idat.a, jdat.a, kdat.a, norm));	};

	void CumSum(const char *dir)	{	mgl_data_cumsum(a,dir);	};
	void Integral(const char *dir)	{	mgl_data_integral(a,dir);	};
	void Diff(const char *dir)		{	mgl_data_diff(a,dir);	};
	void Diff(const mglDataW &v1, const mglDataW &v2)
	{	mgl_data_diff_par(a,v1.a,v2.a,NULL);	};
	void Diff(const mglDataW &v1, const mglDataW &v2, const mglDataW &v3)
	{	mgl_data_diff_par(a,v1.a,v2.a,v3.a);	};
	void Diff2(const char *dir)		{	mgl_data_diff2(a,dir);	};
	void Swap(const char *dir)		{	mgl_data_swap(a,dir);	};
	void Mirror(const char *dir)	{	mgl_data_mirror(a,dir);	};

	mreal Spline(mreal x,mreal y=0,mreal z=0)
	{	return mgl_data_spline(a,x,y,z);	};
	inline mreal Spline1(mreal x,mreal y=0,mreal z=0)
	{	return mgl_data_spline1(a,x,y,z);	};
	mreal Linear(mreal x,mreal y=0,mreal z=0)
	{	return mgl_data_linear(a,x,y,z);	};
	inline mreal Linear1(mreal x,mreal y=0,mreal z=0)
	{	return mgl_data_linear1(a,x,y,z);	};

	void Insert(char dir, int at=0, int num=1)	{	mgl_data_insert(a,dir,at,num);	};
	void Delete(char dir, int at=0, int num=1)	{	mgl_data_delete(a,dir,at,num);	};

	mreal v(int i,int j=0,int k=0)		{	return mgl_data_get_value(a,i,j,k);	};
	mreal GetVal(int i,int j=0,int k=0)	{	return mgl_data_get_value(a,i,j,k);	};
	void SetVal(mreal v,int i,int j=0,int k=0)	{	mgl_data_set_value(a,v,i,j,k);	};
	long GetNx()	{	return mgl_data_get_nx(a);	};
	long GetNy()	{	return mgl_data_get_ny(a);	};
	long GetNz()	{	return mgl_data_get_nz(a);	};
	const mreal *GetData()	{	return mgl_data_data(a);	};
	void operator=(const mglDataW &d)	{	mgl_data_set(a,d.a);	};
	void operator*=(const mglDataW &d)	{	mgl_data_mul_dat(a,d.a);};
	void operator/=(const mglDataW &d)	{	mgl_data_div_dat(a,d.a);};
	void operator+=(const mglDataW &d)	{	mgl_data_add_dat(a,d.a);};
	void operator-=(const mglDataW &d)	{	mgl_data_sub_dat(a,d.a);};
	void operator*=(mreal d)	{	mgl_data_mul_num(a,d);	};
	void operator/=(mreal d)	{	mgl_data_div_num(a,d);	};
	void operator+=(mreal d)	{	mgl_data_add_num(a,d);	};
	void operator-=(mreal d)	{	mgl_data_sub_num(a,d);	};
};
//-----------------------------------------------------------------------------
#endif
