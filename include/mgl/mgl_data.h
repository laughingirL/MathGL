/* mgl_data.h is part of Math Graphic Library
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
#ifndef _MGL_DATA_H_
#define _MGL_DATA_H_
//-----------------------------------------------------------------------------
#include <stdio.h>
#ifdef _MSC_VER
#define	_USE_MATH_DEFINES
#include <float.h>
const unsigned long mgl_nan[2] = {0xffffffff, 0x7fffffff};
#define NANd	(*(double*)mgl_nan)
#define NANf	(*(float*)&(mgl_nan[1]))
#define NAN		NANf
#define isnan	_isnan
#define isfinite	_finite
#define copysignf	_copysign
#define chdir	_chdir
#endif

#define SMOOTH_NONE		0
#define SMOOTH_LINE_3	1
#define SMOOTH_LINE_5	2
#define SMOOTH_QUAD_5	3

#define MGL_HIST_IN		0
#define MGL_HIST_SUM	1
#define MGL_HIST_UP		2
#define MGL_HIST_DOWN	3

#ifndef ipow
#define ipow	ipow_mgl
#endif
#define mgl_min(a,b)	(((a)>(b)) ? (b) : (a))
#define mgl_max(a,b)	(((a)>(b)) ? (a) : (b))

#ifndef NO_GSL
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#else
struct gsl_vector;
struct gsl_matrix;
#endif

/// Class for working with data array
class mglData
{
public:

	long nx;			///< number of points in 1st dimensions ('x' dimension)
	long ny;			///< number of points in 2nd dimensions ('y' dimension)
	long nz;			///< number of points in 3d dimensions ('z' dimension)
	float *a;	  	///< data array

	// ~~~~~~~~~~~~~~~~~~~~ операции ~~~~~~~~~~~~~~~~~~~~
	/// Allocate memory and copy the data from the gsl_vector
	void Set(gsl_vector *v);
	/// Allocate memory and copy the data from the gsl_matrix
	void Set(gsl_matrix *m);
	/// Allocate memory and copy the data from the (float *) array
	void Set(const float *A,int NX,int NY=1,int NZ=1);
	/// Allocate memory and copy the data from the (float *) array
	void Set(const double *A,int NX,int NY=1,int NZ=1);
	/// Allocate memory and copy the data from the (float **) array
	void Set(const float **A,int N1,int N2);
	/// Allocate memory and copy the data from the (float **) array
	void Set(const double **A,int N1,int N2);
	/// Allocate memory and copy the data from the (float ***) array
	void Set(const float ***A,int N1,int N2,int N3);
	/// Allocate memory and copy the data from the (float ***) array
	void Set(const double ***A,int N1,int N2,int N3);
	/// Copy the data from other mglData variable
	inline void Set(mglData &dat)	{	Set(dat.a,dat.nx,dat.ny,dat.nz);	};
	
	/// Read data array from HDF file
	void ReadHDF(const char *fname,const char *data);
	/// Save data to HDF file
	void SaveHDF(const char *fname,const char *data,bool rewrite=false);
	/// Read data from tab-separated text file with auto determining size
	bool Read(const char *fname);
	/// Read data from text file with specifeid size
	bool Read(const char *fname,int mx,int my=1,int mz=1);
	/// Save whole data array (for ns=-1) or only ns-th slice to text file
	void Save(const char *fname,int ns=-1);
	/// Export data array (for ns=-1) or only ns-th slice to PNG file according color scheme
	void Export(const char *fname,const char *scheme,float v1=0,float v2=0,int ns=0);
	/// Import data array from PNG file according color scheme
	void Import(const char *fname,const char *scheme,float v1=0,float v2=1);
	/// Read data from tab-separated text files with auto determining size which filenames are result of sprintf(fname,templ,t) where t=from:step:to
	bool ReadRange(const char *templ, float from, float to, float step=1.f, bool as_slice=false);
	/// Read data from tab-separated text files with auto determining size which filenames are satisfied to template (like "t_*.dat")
	bool ReadAll(const char *templ, bool as_slice=false);

	/// Create or recreate the array with specified size and fill it by zero
	void Create(int nx,int ny=1,int nz=1);
	/// Extend data dimensions
	void Extend(int n1, int n2=0);
	/// Transpose dimensions of the data (generalization of Transpose)
	void Transpose(const char *dim="yx");
	/// Normalize the data to range [v1,v2]
	void Norm(float v1=0,float v2=1,bool sym=false,int dim=0);
	/// Eqidistantly fill the data to range [x1,x2] in direction \a dir
	void Fill(float x1,float x2,char dir='x');
	/// Modify the data by specified formula
	void Modify(const char *eq,int dim=0);
	/// Modify the data by specified formula
	void Modify(const char *eq,mglData &v, mglData &w);
	/// Modify the data by specified formula
	void Modify(const char *eq,mglData &v);
	/// Reduce size of the data
	void Squeeze(int rx,int ry=1,int rz=1,bool smooth=true);
	/// Crop the data
	void Crop(int n1, int n2,char dir='x');

	/// Get maximal value of the data
	float Maximal();
	/// Get minimal value of the data
	float Minimal();
	/// Get maximal value of the data and its position
	float Maximal(int &i,int &j,int &k);
	/// Get minimal value of the data and its position
	float Minimal(int &i,int &j,int &k);
	/// Get maximal value of the data and its approximated position
	float Maximal(float &x,float &y,float &z);
	/// Get minimal value of the data and its approximated position
	float Minimal(float &x,float &y,float &z);
	/// Get "energy" and find first (median) and second (width) momentums of data
	float Momentum(char dir,float &m,float &w);
	/// Get "energy and find 4 momentums of data: median, width, skewness, kurtosis
	float Momentum(char dir,float &m,float &w,float &s,float &k);
	/// Print information about the data (sizes and momentum) to string
	void PrintInfo(char *buf);
	/// Print information about the data (sizes and momentum) to FILE (for example, stdout)
	void PrintInfo(FILE *fp);
	/// Find position (after specified in i,j,k) of first nonzero value of formula
	float Find(const char *cond, int &i, int &j, int &k);
	/// Find position (before specified in i,j,k) of last nonzero value of formula
	float Last(const char *cond, int &i, int &j, int &k);
	/// Find position of first in direction 'dir' nonzero value of formula
	int Find(const char *cond, char dir, int i=0, int j=0, int k=0);

	/// Smooth the data on specified direction or directions
	void Smooth(int Type,const char *dirs="xyz",float delta=0);
	/// Get sub-array of the data with given fixed indexes
	mglData &SubData(int xx,int yy=-1,int zz=-1);
	/// Create n-th points distribution of this data values in range [v1, v2]
	mglData &Hist(int n,float v1=0,float v2=1, int nsub=0);
	/// Create n-th points distribution of this data values in range [v1, v2] with weight \a w
	mglData &Hist(mglData &w, int n,float v1=0,float v2=1, int nsub=0);
	/// Get array which is result of summation in given direction or directions
	mglData &Sum(const char *dir);
	/// Get array which is result of maximal values in given direction or directions
	mglData &Max(const char *dir);
	/// Get array which is result of minimal values in given direction or directions
	mglData &Min(const char *dir);
	/// Resize the data to new size of box [x1,x2]*[y1,y2]*[z1,z2]
	mglData &Resize(int mx,int my=1,int mz=1,float x1=0,float x2=1,
		float y1=0,float y2=1,float z1=0,float z2=1);
	/// Get the data which is tensor multiplication (d[i,j] = this[i]*a[j] and so on)
	mglData &Combine(mglData &a);

	/// Cumulative summation the data in given direction or directions
	void CumSum(const char *dir);
	/// Integrate (cumulative summation) the data in given direction or directions
	void Integral(const char *dir);
	/// Differentiate the data in given direction or directions
	void Diff(const char *dir);
	/// Double-differentiate (like laplace operator) the data in given direction
	void Diff2(const char *dir);
	/// Swap left and right part of the data in given direction (useful for fourier spectrums)
	void Swap(const char *dir);
//	/// Fourier transform of the data in given direction or directions
//	void Fourier(const char *dir);
//	/// Hankel transform of the data in given direction or directions
//	void Hankel(const char *dir);

	/// Interpolate by qubic splain the data to given point x=[0...nx-1], y=[0...ny-1], z=[0...nz-1]
	float Spline(float x,float y=0,float z=0);
	/// Interpolate by qubic splain the data to given point \a x,\a y,\a z which normalized in range [0, 1]
	float Spline1(float x,float y=0,float z=0)
	{	return Spline(x*(nx-1),y*(ny-1),z*(nz-1));	};
	/// Interpolate by linear function the data to given point x=[0...nx-1], y=[0...ny-1], z=[0...nz-1]
	float Linear(float x,float y=0,float z=0);
	/// Interpolate by line the data to given point \a x,\a y,\a z which normalized in range [0, 1]
	float Linear1(float x,float y=0,float z=0)
	{	return Linear(x*(nx-1),y*(ny-1),z*(nz-1));	};

	/// Insert a slice to the data at begin and fill it by formula \a eq
	void Insert(const char *eq);		// not ready
	/// Remove first slice of the data
	void PullOut();				// not ready
	/// Get the value in given cell of the data with border checking
	float v(int i,int j=0,int k=0);

	/// Initiate by other mglData variable
	mglData(mglData &d)		{	a=0;	Set(d);		};
	/// Read data from file
	mglData(const char *fname)	{	a=0;	Read(fname);	}
	/// Allocate the memory for data array and initialize it zero
	mglData(int xx=1,int yy=1,int zz=1)	{	a=0;	Create(xx,yy,zz);	};
	/// Delete the array
	~mglData()			{	if(a)	delete []a;		};
	/// Copy data from other mglData variable
	void operator=(mglData &d)	{	Set(d.a,d.nx,d.ny,d.nz);	};
	/// Multiplicate the data by other one for each element
	void operator*=(mglData &d);
	/// Divide the data by other one for each element
	void operator/=(mglData &d);
	/// Add the other data
	void operator+=(mglData &d);
	/// Substract the other data
	void operator-=(mglData &d);
	/// Multiplicate each element by the number
	void operator*=(float d);
	/// Divide each element by the number
	void operator/=(float d);
	/// Add the number
	void operator+=(float d);
	/// Substract the number
	void operator-=(float d);
private:
	float _p[4][4];
};
//-----------------------------------------------------------------------------
double mgl_rnd();
double ipow_mgl(double x,int n);
/// Integral data transformation (like Fourier 'f' or 'i', Hankel 'h' or None 'n') for amplitude and phase
mglData &TransformA(mglData &am, mglData &ph, const char *tr);
/// Integral data transformation (like Fourier 'f' or 'i', Hankel 'h' or None 'n') for real and imaginary parts
mglData &Transform(mglData &re, mglData &im, const char *tr);
/// Short time fourier analysis for real and imaginary parts. Output is amplitude of partial fourier (result will have size {dn, floor(nx/dn), ny} for dir='x'
mglData &STFA(mglData &re, mglData &im, int dn, char dir='x');
//-----------------------------------------------------------------------------
#endif
