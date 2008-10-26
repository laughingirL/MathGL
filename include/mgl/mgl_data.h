/* mgl_data.h is part of Math Graphic Library
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
#ifndef _MGL_DATA_H_
#define _MGL_DATA_H_
//-----------------------------------------------------------------------------
#include <math.h>
#include <string.h>
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
//-----------------------------------------------------------------------------
#define SMOOTH_NONE		0
#define SMOOTH_LINE_3	1
#define SMOOTH_LINE_5	2
#define SMOOTH_QUAD_5	3
//-----------------------------------------------------------------------------
#define MGL_HIST_IN		0
#define MGL_HIST_SUM	1
#define MGL_HIST_UP		2
#define MGL_HIST_DOWN	3
//-----------------------------------------------------------------------------
#ifndef ipow
#define ipow	ipow_mgl
#endif
#define mgl_min(a,b)	(((a)>(b)) ? (b) : (a))
#define mgl_max(a,b)	(((a)>(b)) ? (a) : (b))
//-----------------------------------------------------------------------------
#ifndef NO_GSL
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#else
struct gsl_vector;
struct gsl_matrix;
#endif
//-----------------------------------------------------------------------------
/// Class for incapsulating point in space
struct mglPoint
{
	float x,y,z;
 	mglPoint(float X=0,float Y=0,float Z=0){x=X;y=Y;z=Z;};
};
inline mglPoint operator+(const mglPoint &a, const mglPoint &b)
{	return mglPoint(a.x+b.x, a.y+b.y, a.z+b.z);	};
inline mglPoint operator-(const mglPoint &a, const mglPoint &b)
{	return mglPoint(a.x-b.x, a.y-b.y, a.z-b.z);	};
inline mglPoint operator*(float b, const mglPoint &a)
{	return mglPoint(a.x*b, a.y*b, a.z*b);	};
inline mglPoint operator*(const mglPoint &a, float b)
{	return mglPoint(a.x*b, a.y*b, a.z*b);	};
inline mglPoint operator/(const mglPoint &a, float b)
{	return mglPoint(a.x/b, a.y/b, a.z/b);	};
inline float operator*(const mglPoint &a, const mglPoint &b)
{	return a.x*b.x+a.y*b.y+a.z*b.z;	};
inline mglPoint operator&(const mglPoint &a, const mglPoint &b)
{	return a - b*((a*b)/(b*b));	};
inline mglPoint operator|(const mglPoint &a, const mglPoint &b)
{	return b*((a*b)/(b*b));	};
inline mglPoint operator^(const mglPoint &a, const mglPoint &b)
{	return mglPoint(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x);	};
inline mglPoint operator!(const mglPoint &a)
{	return (a.x==0 && a.y==0)?mglPoint(1,0,0):mglPoint(-a.y/hypot(a.x,a.y), a.x/hypot(a.x,a.y), 0);	};
inline bool operator==(const mglPoint &a, const mglPoint &b)
{	return !memcmp(&a, &b, sizeof(mglPoint));	}
inline bool operator!=(const mglPoint &a, const mglPoint &b)
{	return memcmp(&a, &b, sizeof(mglPoint));	}
inline float Norm(const mglPoint &p)
{	return sqrt(p.x*p.x+p.y*p.y+p.z*p.z);	};
//-----------------------------------------------------------------------------
/// Class for working with data array
class mglData
{
public:

	long nx;		///< number of points in 1st dimensions ('x' dimension)
	long ny;		///< number of points in 2nd dimensions ('y' dimension)
	long nz;		///< number of points in 3d dimensions ('z' dimension)
	float *a;	  	///< data array
	char *id;		///< column (or slice) names

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
	/// Allocate memory and scanf the data from the string
	void Set(const char *str,int NX,int NY=1,int NZ=1);
	/// Copy the data from other mglData variable
	inline void Set(const mglData &dat)	{	Set(dat.a,dat.nx,dat.ny,dat.nz);	};
	/// Rearange data dimensions
	void Rearrange(int mx, int my=0, int mz=0);

	/// Read data array from HDF file
	void ReadHDF(const char *fname,const char *data);
	/// Save data to HDF file
	void SaveHDF(const char *fname,const char *data,bool rewrite=false) const;
	/// Read data from tab-separated text file with auto determining size
	bool Read(const char *fname);
	/// Read data from text file with specifeid size
	bool Read(const char *fname,int mx,int my=1,int mz=1);
	/// Save whole data array (for ns=-1) or only ns-th slice to text file
	void Save(const char *fname,int ns=-1) const;
	/// Export data array (for ns=-1) or only ns-th slice to PNG file according color scheme
	void Export(const char *fname,const char *scheme,float v1=0,float v2=0,int ns=-1) const;
	/// Import data array from PNG file according color scheme
	void Import(const char *fname,const char *scheme,float v1=0,float v2=1);
	/// Read data from tab-separated text files with auto determining size which filenames are result of sprintf(fname,templ,t) where t=from:step:to
	bool ReadRange(const char *templ, float from, float to, float step=1.f, bool as_slice=false);
	/// Read data from tab-separated text files with auto determining size which filenames are satisfied to template (like "t_*.dat")
	bool ReadAll(const char *templ, bool as_slice=false);
	/// Read data from text file with size specified at beginning of the file
	bool ReadMat(const char *fname, int dim=2);

	/// Create or recreate the array with specified size and fill it by zero
	void Create(int nx,int ny=1,int nz=1);
	/// Extend data dimensions
	void Extend(int n1, int n2=0);
	/// Transpose dimensions of the data (generalization of Transpose)
	void Transpose(const char *dim="yx");
	/// Normalize the data to range [v1,v2]
	void Norm(float v1=0,float v2=1,bool sym=false,int dim=0);
	/// Normalize the data to range [v1,v2] slice by slice
	void NormSl(float v1=0,float v2=1,char dir='z',bool keep_en=true,bool sym=false);
	/// Put value to data element(s)
	void Put(float val, int i=-1, int j=-1, int k=-1);
	/// Put array to data element(s)
	void Put(const mglData &val, int i=-1, int j=-1, int k=-1);
	/// Modify the data by specified formula
	void Modify(const char *eq,int dim=0);
	/// Modify the data by specified formula
	void Modify(const char *eq,const mglData &v, const mglData &w);
	/// Modify the data by specified formula
	void Modify(const char *eq,const mglData &v);
	/// Modify the data by specified formula assuming x,y,z in range [r1,r2]
	void Fill(const char *eq, mglPoint r1, mglPoint r2, const mglData *v=0, const mglData *w=0);
	/// Eqidistantly fill the data to range [x1,x2] in direction \a dir
	void Fill(float x1,float x2,char dir='x');
	/// Get column (or slice) of the data filled by formulas of other named columns
	mglData Column(const char *eq);
	/// Set names for columns (slices)
	void SetColumnId(const char *ids);
	/// Reduce size of the data
	void Squeeze(int rx,int ry=1,int rz=1,bool smooth=true);
	/// Crop the data
	void Crop(int n1, int n2,char dir='x');

	/// Get maximal value of the data
	float Maximal() const;
	/// Get minimal value of the data
	float Minimal() const;
	/// Get maximal value of the data and its position
	float Maximal(int &i,int &j,int &k) const;
	/// Get minimal value of the data and its position
	float Minimal(int &i,int &j,int &k) const;
	/// Get maximal value of the data and its approximated position
	float Maximal(float &x,float &y,float &z) const;
	/// Get minimal value of the data and its approximated position
	float Minimal(float &x,float &y,float &z) const;
	/// Get "energy" and find first (median) and second (width) momentums of data
	float Momentum(char dir,float &m,float &w) const;
	/// Get "energy and find 4 momentums of data: median, width, skewness, kurtosis
	float Momentum(char dir,float &m,float &w,float &s,float &k) const;
	/// Get momentum (1D-array) of data along direction 'dir'. String looks like "x1" for median in x-direction, "x2" for width in x-dir and so on.
	mglData Momentum(char dir, const char *how) const;
	/// Print information about the data (sizes and momentum) to string
	void PrintInfo(char *buf, bool all=false) const;
	/// Print information about the data (sizes and momentum) to FILE (for example, stdout)
	void PrintInfo(FILE *fp) const;
	/// Find position (after specified in i,j,k) of first nonzero value of formula
	float Find(const char *cond, int &i, int &j, int &k) const;
	/// Find position (before specified in i,j,k) of last nonzero value of formula
	float Last(const char *cond, int &i, int &j, int &k) const;
	/// Find position of first in direction 'dir' nonzero value of formula
	int Find(const char *cond, char dir, int i=0, int j=0, int k=0) const;
	/// Find if any nonzero value of formula
	bool FindAny(const char *cond) const;

	/// Smooth the data on specified direction or directions
	void Smooth(int Type,const char *dirs="xyz",float delta=0);
	/// Set as the data envelop
	void Envelop(char dir='x');
	/// Remove phase jump
	void Sew(const char *dirs="xyz", float da=2*M_PI);
	/// Smooth the data on specified direction or directions
	void Smooth(const char *dirs="xyz");
	/// Get sub-array of the data with given fixed indexes
	mglData SubData(int xx,int yy=-1,int zz=-1) const;
	/// Create n-th points distribution of this data values in range [v1, v2]
	mglData Hist(int n,float v1=0,float v2=1, int nsub=0) const;
	/// Create n-th points distribution of this data values in range [v1, v2] with weight \a w
	mglData Hist(const mglData &w, int n,float v1=0,float v2=1, int nsub=0) const;
	/// Get array which is result of summation in given direction or directions
	mglData Sum(const char *dir) const;
	/// Get array which is result of maximal values in given direction or directions
	mglData Max(const char *dir) const;
	/// Get array which is result of minimal values in given direction or directions
	mglData Min(const char *dir) const;
	/// Resize the data to new size of box [x1,x2]*[y1,y2]*[z1,z2]
	mglData Resize(int mx,int my=1,int mz=1,float x1=0,float x2=1,
		float y1=0,float y2=1,float z1=0,float z2=1) const;
	/// Get the data which is direct multiplication (like, d[i,j] = this[i]*a[j] and so on)
	mglData Combine(const mglData &a) const;
	/// Get array which values is result of interpolation this for coordinates from other arrays
	mglData Evaluate(const mglData &idat, bool norm=true) const;
	mglData Evaluate(const mglData &idat, const mglData &jdat, bool norm=true) const;
	mglData Evaluate(const mglData &idat, const mglData &jdat, const mglData &kdat, bool norm=true) const;

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
	/// Mirror the data in given direction (useful for fourier spectrums)
	void Mirror(const char *dir);

	/// Interpolate by 5-th order splain the data to given point \a x,\a y,\a z which normalized in range [0, 1] and evaluate its derivatives
	float Spline5(float x,float y,float z,float &dx,float &dy,float &dz) const;
	/// Interpolate by qubic splain the data to given point x=[0...nx-1], y=[0...ny-1], z=[0...nz-1]
	float Spline(float x,float y=0,float z=0) const;
	/// Interpolate by qubic splain the data to given point \a x,\a y,\a z which normalized in range [0, 1]
	float Spline1(float x,float y=0,float z=0) const
	{	return Spline(x*(nx-1),y*(ny-1),z*(nz-1));	};
	/// Interpolate by linear function the data to given point x=[0...nx-1], y=[0...ny-1], z=[0...nz-1]
	float Linear(float x,float y=0,float z=0) const;
	/// Interpolate by line the data to given point \a x,\a y,\a z which normalized in range [0, 1]
	float Linear1(float x,float y=0,float z=0) const
	{	return Linear(x*(nx-1),y*(ny-1),z*(nz-1));	};

	/// Insert \a num rows after \a at and fill it by formula \a eq
	void InsertRows(int at, int num=1, const char *eq=0);
	/// Insert \a num columns after \a at and fill it by formula \a eq
	void InsertColumns(int at, int num=1, const char *eq=0);
	/// Insert \a num slices after \a at and fill it by formula \a eq
	void InsertSlices(int at, int num=1, const char *eq=0);
	/// Delete \a num rows starting from \a at
	void DeleteRows(int at, int num=1);
	/// Delete \a num rows starting from \a at
	void DeleteColumns(int at, int num=1);
	/// Delete \a num rows starting from \a at
	void DeleteSlices(int at, int num=1);
	/// Insert data
	void Insert(char dir, int at=0, int num=1);
	/// Delete data
	void Delete(char dir, int at=0, int num=1);

	/// Get the value in given cell of the data with border checking
	float v(int i,int j=0,int k=0) const;

	/// Initiate by other mglData variable
	mglData(const mglData &d)		{	a=0;	Set(d);		};
	/// Read data from file
	mglData(const char *fname)	{	a=0;	Read(fname);	}
	/// Allocate the memory for data array and initialize it zero
	mglData(int xx=1,int yy=1,int zz=1)	{	a=0;	Create(xx,yy,zz);	};
	/// Delete the array
	~mglData()			{	if(a)	{	delete []a;	delete []id;	}	};
	/// Copy data from other mglData variable
	void operator=(const mglData &d)	{	Set(d.a,d.nx,d.ny,d.nz);	};
	/// Multiplicate the data by other one for each element
	void operator*=(const mglData &d);
	/// Divide the data by other one for each element
	void operator/=(const mglData &d);
	/// Add the other data
	void operator+=(const mglData &d);
	/// Substract the other data
	void operator-=(const mglData &d);
	/// Multiplicate each element by the number
	void operator*=(float d);
	/// Divide each element by the number
	void operator/=(float d);
	/// Add the number
	void operator+=(float d);
	/// Substract the number
	void operator-=(float d);
};
//-----------------------------------------------------------------------------
void mgl_srnd(long seed);
double mgl_rnd();
double ipow_mgl(double x,int n);
/// Integral data transformation (like Fourier 'f' or 'i', Hankel 'h' or None 'n') for amplitude and phase
mglData TransformA(const mglData &am, const mglData &ph, const char *tr);
/// Integral data transformation (like Fourier 'f' or 'i', Hankel 'h' or None 'n') for real and imaginary parts
mglData Transform(const mglData &re, const mglData &im, const char *tr);
/// Short time fourier analysis for real and imaginary parts. Output is amplitude of partial fourier (result will have size {dn, floor(nx/dn), ny} for dir='x'
mglData STFA(const mglData &re, const mglData &im, int dn, char dir='x');
/// Saves result of PDE solving (|u|^2) for "Hamiltonian" \a ham with initial conditions \a ini
mglData mglPDE(const char *ham, const mglData &ini_re, const mglData &ini_im, mglPoint Min, mglPoint Max, float dz=0.1, float k0=100);
// /// Saves result of PDE solving (|u|^2) for "Hamiltonian" \a ham with initial conditions \a ini along a curve \a ray (must have nx>=7 - x,y,z,px,py,pz,tau)
//mglData mglQO_PDE(char *ham, const mglData &ini_re, const mglData &ini_im, mglData &ray, float r=1, float k0=100, const mglData *xx=0, const mglData *yy=0, const mglData *zz=0);
/// Prepares ray data for mglQO_PDE with starting point \a r0, \a p0
mglData mglRay(const char *ham, mglPoint r0, mglPoint p0, float dt=0.1, float tmax=10);
//-----------------------------------------------------------------------------
#endif
