#include <stdlib.h>
#include <string.h>

#include "mgl/mgl_data.h"
#include "mgl/mgl_c.h"
#include "mgl/mgl_f.h"

//-----------------------------------------------------------------------------
//		Data operations
//-----------------------------------------------------------------------------
/// Multiplicate the data by other one for each element
void mgl_data_mul_dat(HMDT d, HMDT b){	*d *= *b;	}
/// Divide the data by other one for each element
void mgl_data_div_dat(HMDT d, HMDT b) {	*d /= *b;	}
/// Add the other data
void mgl_data_add_dat(HMDT d, HMDT b) {	*d += *b;	}
/// Substract the other data
void mgl_data_sub_dat(HMDT d, HMDT b) {	*d -= *b;	}
/// Multiplicate each element by the number
void mgl_data_mul_num(HMDT d, float b) {	*d *= b;	}
/// Divide each element by the number
void mgl_data_div_num(HMDT d, float b) {	*d /= b;	}
/// Add the number
void mgl_data_add_num(HMDT d, float b) {	*d += b;	}
/// Substract the number
void mgl_data_sub_num(HMDT d, float b) {	*d -= b;	}
//-----------------------------------------------------------------------------
/// Resize the data to new size of box [x1,x2]*[y1,y2]*[z1,z2]
HMDT mgl_data_resize(HMDT d, int mx,int my,int mz,float x1,float x2,
	float y1,float y2,float z1,float z2)
{	return new mglData(d->Resize(mx,my,mz,x1,x2,y1,y2,z1,z2));	}
/// Get sub-array of the data with given fixed indexes
HMDT mgl_data_subdata(HMDT d, int xx,int yy,int zz)
{	return new mglData(d->SubData(xx,yy,zz));	}
/// Eqidistantly fill the data to range [x1,x2] in direction \a dir
void mgl_data_fill(HMDT d, float x1,float x2,char dir)
{	d->Fill(x1,x2,dir);	}
/// Modify the data by specified formula
void mgl_data_modify(HMDT d, const char *eq,int dim)
{	d->Modify(eq,dim);	}
/// Modify the data by specified formula
void mgl_data_modify_vw(HMDT d, const char *eq, HMDT v, HMDT w)
{	if(d&&v&&w)	d->Modify(eq,*v,*w);	else if(d&&v)	d->Modify(eq,*v);	}
/// Create or recreate the array with specified size and fill it by zero
void mgl_data_create(HMDT d, int nx,int ny,int nz)
{	d->Create(nx,ny,nz);	}
/// Transpose the x<->y dimensions of the data
void mgl_data_transpose(HMDT d, const char *dim)	{	d->Transpose(dim);	}
/// Normalize the data to range [v1,v2]
void mgl_data_norm(HMDT d, float v1,float v2,bool sym,int dim)
{	d->Norm(v1,v2,sym,dim);	}
/// Reduce size of the data
void mgl_data_squeeze(HMDT d, int rx,int ry,int rz,bool smooth)
{	d->Squeeze(rx,ry,rz,smooth);	}
/// Get maximal value of the data
float mgl_data_max(HMDT d)	{	return d->Maximal();	}
/// Get minimal value of the data
float mgl_data_min(HMDT d)	{	return d->Minimal();	}
//// Insert a slice to the data at begin and fill it by formula \a eq
//void mgl_data_insert(HMDT d, const char *eq)	{	d->Insert(eq);	}
//// Remove first slice of the data
//void mgl_data_pullout(HMDT d)	{	d->PullOut();	}
/// Get the value in given cell of the data with border checking
float &mgl_data_value(HMDT d, int i,int j,int k)	{	return d->a[i+d->nx*(j+d->ny*k)];	}
/// Swap left and right part of the data in given direction (useful for fourier spectrums)
void mgl_data_swap(HMDT d, const char *dir)
{	d->Swap(dir);	}
//-----------------------------------------------------------------------------
/// Allocate memory and copy the data from the (float *) array
void mgl_data_set_float(HMDT d, const float *A,int NX,int NY,int NZ)
{	d->Set(A,NX,NY,NZ);	}
/// Allocate memory and copy the data from the (float *) array
void mgl_data_set_double(HMDT d, const double *A,int NX,int NY,int NZ)
{	d->Set(A,NX,NY,NZ);	}
/// Allocate memory and copy the data from the (float **) array
void mgl_data_set_float2(HMDT d, const float **A,int N1,int N2)
{	d->Set(A,N1,N2);	}
/// Allocate memory and copy the data from the (float **) array
void mgl_data_set_double2(HMDT d, const double **A,int N1,int N2)
{	d->Set(A,N1,N2);	}
/// Allocate memory and copy the data from the (float ***) array
void mgl_data_set_float3(HMDT d, const float ***A,int N1,int N2,int N3)
{	d->Set(A,N1,N2,N3);	}
/// Allocate memory and copy the data from the (float ***) array
void mgl_data_set_double3(HMDT d, const double ***A,int N1,int N2,int N3)
{	d->Set(A,N1,N2,N3);	}
/// Copy the data from other mglData variable
void mgl_data_set(HMDT d, HMDT a)	{	d->Set(*a);	}
/// Read data from tab-separated text file with auto determining size
bool mgl_data_read(HMDT d, const char *fname)	{	return d->Read(fname);	}
/// Read data from tab-separated text file with built-in sizes
bool mgl_data_read_mat(HMDT d, const char *fname, int dim)
{	return d->ReadMat(fname,dim);	}
/// Read data from text file with specifeid size
bool mgl_data_read_dim(HMDT d, const char *fname,int mx,int my,int mz)
{	return d->Read(fname,mx,my,mz);	}
/// Save whole data array (for ns=-1) or only ns-th slice to text file
void mgl_data_save(HMDT d, const char *fname,int ns)
{	d->Save(fname,ns);	}
const float *mgl_data_data(HMDT d)
{	return d->a;	}
/// Get the data which is tensor multiplication (d[i,j] = a[i]*b[j] and so on)
HMDT mgl_data_combine(HMDT a, HMDT b)
{	return new mglData(a->Combine(*b));	}
/// Extend data dimensions
void mgl_data_extend(HMDT dat, int n1, int n2)
{	dat->Extend(n1,n2);	}
//-----------------------------------------------------------------------------
void mgl_data_set_vector(HMDT dat, gsl_vector *v)
{	dat->Set(v);	}
void mgl_data_set_matrix(HMDT dat, gsl_matrix *m)
{	dat->Set(m);	}
//-----------------------------------------------------------------------------
//		Data operations (Fortran)
//-----------------------------------------------------------------------------
/// Multiplicate the data by other one for each element
void mgl_data_mul_dat_(long *d, long *b){	_D_(d) *= _D_(b);	}
/// Divide the data by other one for each element
void mgl_data_div_dat_(long *d, long *b) {	_D_(d) /= _D_(b);	}
/// Add the other data
void mgl_data_add_dat_(long *d, long *b) {	_D_(d) += _D_(b);	}
/// Substract the other data
void mgl_data_sub_dat_(long *d, long *b) {	_D_(d) -= _D_(b);	}
/// Multiplicate each element by the number
void mgl_data_mul_num_(long *d, float *b) {	_D_(d) *= *b;	}
/// Divide each element by the number
void mgl_data_div_num_(long *d, float *b) {	_D_(d) /= *b;	}
/// Add the number
void mgl_data_add_num_(long *d, float *b) {	_D_(d) += *b;	}
/// Substract the number
void mgl_data_sub_num_(long *d, float *b) {	_D_(d) -= *b;	}
//-----------------------------------------------------------------------------
/// Resize the data to new size of box [x1,x2]*[y1,y2]*[z1,z2]
long mgl_data_resize_(long *d, int *mx,int *my,int *mz,float *x1,float *x2,
						float *y1,float *y2,float *z1,float *z2)
{	return long(new mglData(_DT_->Resize(*mx,*my,*mz,*x1,*x2,*y1,*y2,*z1,*z2)));	}
/// Get sub-array of the data with given fixed indexes
long mgl_data_subdata_(long *d, int *xx,int *yy,int *zz)
{	return long(new mglData(_DT_->SubData(*xx,*yy,*zz)));	}
/// Eqidistantly fill the data to range [x1,x2] in direction \a dir
void mgl_data_fill_(long *d, float *x1,float *x2,const char *dir,int)
{	_DT_->Fill(*x1,*x2,*dir);	}
/// Modify the data by specified formula
void mgl_data_modify_(long *d, const char *eq,int *dim,int l)
{
	char *s=new char[l+1];	memcpy(s,eq,l);	s[l]=0;
	_DT_->Modify(s,*dim);
	delete []s;
}
/// Modify the data by specified formula
void mgl_data_modify_vw_(long *d, const char *eq, long *v, long *w,int l)
{
	char *s=new char[l+1];	memcpy(s,eq,l);	s[l]=0;
	if(d&&v&&w)		_DT_->Modify(s,_D_(v),_D_(w));
	else if(d&&v)	_DT_->Modify(s,_D_(v));
	delete []s;
}
/// Create or recreate the array with specified size and fill it by zero
void mgl_data_create_(long *d, int *nx,int *ny,int *nz)
{	_DT_->Create(*nx,*ny,*nz);	}
/// Transpose the x<->y dimensions of the data
void mgl_data_transpose_(long *d, const char *dim,int l)
{
	char *s=new char[l+1];	memcpy(s,dim,l);	s[l]=0;
	_DT_->Transpose(s);
	delete []s;
}
/// Normalize the data to range [v1,v2]
void mgl_data_norm_(long *d, float *v1,float *v2,int *sym,int *dim)
{	_DT_->Norm(*v1,*v2,*sym,*dim);	}
/// Reduce size of the data
void mgl_data_squeeze_(long *d, int *rx,int *ry,int *rz,int *smooth)
{	_DT_->Squeeze(*rx,*ry,*rz,*smooth);	}
/// Get maximal value of the data
float mgl_data_max_(long *d)	{	return _DT_->Maximal();	}
/// Get minimal value of the data
float mgl_data_min_(long *d)	{	return _DT_->Minimal();	}
//// Insert a slice to the data at begin and fill it by formula \a eq
//void mgl_data_insert(long *d, const char *eq)	{	d->Insert(eq);	}
//// Remove first slice of the data
//void mgl_data_pullout(long *d)	{	d->PullOut();	}
/// Get the value in given cell of the data with border checking
//float mgl_data_value_(long *d, int *i,int *j,int *k)	{	return d->a[*i+d->nx*(*j+d->ny**k)];	}
/// Swap left and right part of the data in given direction (useful for fourier spectrums)
void mgl_data_swap_(long *d, const char *dir,int l)
{
	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	_DT_->Swap(s);
	delete []s;
}
//-----------------------------------------------------------------------------
/// Allocate memory and copy the data from the (float *) array
void mgl_data_set_real1_(long *d, const float *A,int *NX)
{	_DT_->Set(A,*NX);	}
/// Allocate memory and copy the data from the (float *) array
void mgl_data_set_double1_(long *d, const double *A,int *NX)
{	_DT_->Set(A,*NX);	}
/// Allocate memory and copy the data from the (float *) array
void mgl_data_set_real_(long *d, const float *A,int *NX,int *NY,int *NZ)
{	_DT_->Set(A,*NX,*NY,*NZ);	}
/// Allocate memory and copy the data from the (float *) array
void mgl_data_set_double_(long *d, const double *A,int *NX,int *NY,int *NZ)
{	_DT_->Set(A,*NX,*NY,*NZ);	}
/// Allocate memory and copy the data from the (float **) array
void mgl_data_set_real2_(long *d, const float *A,int *N1,int *N2)
{	_DT_->Set(A,*N1,*N2);	}
/// Allocate memory and copy the data from the (float **) array
void mgl_data_set_double2_(long *d, const double *A,int *N1,int *N2)
{	_DT_->Set(A,*N1,*N2);	}
/// Allocate memory and copy the data from the (float ***) array
void mgl_data_set_real3_(long *d, const float *A,int *N1,int *N2,int *N3)
{	_DT_->Set(A,*N1,*N2,*N3);	}
/// Allocate memory and copy the data from the (float ***) array
void mgl_data_set_double3_(long *d, const double *A,int *N1,int *N2,int *N3)
{	_DT_->Set(A,*N1,*N2,*N3);	}
/// Copy the data from other mglData variable
void mgl_data_set_(long *d, long *a)	{	_DT_->Set(_D_(a));	}
/// Read data from tab-separated text file with auto determining size
int mgl_data_read_(long *d, const char *fname,int l)
{
	char *s=new char[l+1];		memcpy(s,fname,l);	s[l]=0;
	int r = _DT_->Read(s);	delete []s;			return r;
}
/// Read data from tab-separated text file with buit-in sizes
int mgl_data_read_(long *d, const char *fname,int *dim,int l)
{
	char *s=new char[l+1];		memcpy(s,fname,l);	s[l]=0;
	int r = _DT_->Read(s,*dim);	delete []s;			return r;
}
/// Read data from text file with specifeid size
int mgl_data_read_dim_(long *d, const char *fname,int *mx,int *my,int *mz,int l)
{
	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	int r = _DT_->Read(s,*mx,*my,*mz);
	delete []s;			return r;
}
/// Save whole data array (for ns=-1) or only ns-th slice to text file
void mgl_data_save_(long *d, const char *fname,int *ns,int l)
{
	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	_DT_->Save(s,*ns);		delete []s;
}
const float *mgl_data_data_(long *d)
{	return _DT_->a;	}
/// Get the data which is tensor multiplication (d[i,j] = a[i]*b[j] and so on)
long mgl_data_combine_(long *a, long *b)
{	return (long)(new mglData(((mglData *)*a)->Combine(_D_(b))));	}
/// Extend data dimensions
void mgl_data_extend_(long *d, int *n1, int *n2)
{	_DT_->Extend(*n1,*n2);	}
//-----------------------------------------------------------------------------
//		Data manipulation functions
//-----------------------------------------------------------------------------
/// Smooth the data on specified direction or directions
void mgl_data_smooth(HMDT d, int Type,float delta,const char *dirs)
{	d->Smooth(Type,dirs,delta);	}
/// Get array which is result of summation in given direction or directions
HMDT mgl_data_sum(HMDT d, const char *dir)
{	return new mglData(d->Sum(dir));	}
/// Integrate (cumulative summation) the data in given direction or directions
void mgl_data_integral(HMDT d, const char *dir)
{	d->Integral(dir);	}
/// Differentiate the data in given direction or directions
void mgl_data_diff(HMDT d, const char *dir)
{	d->Diff(dir);	}
/// Double-differentiate (like laplace operator) the data in given direction
void mgl_data_diff2(HMDT d, const char *dir)
{	d->Diff2(dir);	}
/// Interpolate by qubic splain the data to given point x=[0...nx-1], y=[0...ny-1], z=[0...nz-1]
float mgl_data_spline(HMDT d, float x,float y,float z)
{	return d->Spline(x,y,z);	}
/// Interpolate by qubic splain the data to given point \a x,\a y,\a z which normalized in range [0, 1]
float mgl_data_spline1(HMDT d, float x,float y,float z)
{	return d->Spline1(x,y,z);	}
/// Interpolate by linear function the data to given point x=[0...nx-1], y=[0...ny-1], z=[0...nz-1]
float mgl_data_linear(HMDT d, float x,float y,float z)
{	return d->Linear(x,y,z);	}
/// Interpolate by line the data to given point \a x,\a y,\a z which normalized in range [0, 1]
float mgl_data_linear1(HMDT d, float x,float y,float z)
{	return d->Linear1(x,y,z);	}
/// Create n-th points distribution of this data values in range [v1, v2]
HMDT mgl_data_hist(HMDT d, int n, float v1, float v2, int nsub)
{	return new mglData(d->Hist(n,v1,v2,nsub));	}
/// Create n-th points distribution of this data values in range [v1, v2] with weight \a w
HMDT mgl_data_hist_w(HMDT d, HMDT w, int n, float v1, float v2, int nsub)
{	return new mglData(d->Hist(*w,n,v1,v2,nsub));	}
/// Cumulative summation the data in given direction or directions
void mgl_data_cumsum(HMDT d, const char *dir)
{	d->CumSum(dir);	}
/// Crop the data (cut off data edges)
void mgl_data_crop(HMDT d, int n1, int n2, char dir)
{	d->Crop(n1,n2,dir);	}
/// Get array which is result of maximal values in given direction or directions
HMDT mgl_data_max_dir(HMDT d, const char *dir)
{	return new mglData(d->Max(dir));	}
/// Get array which is result of minimal values in given direction or directions
HMDT mgl_data_min_dir(HMDT d, const char *dir)
{	return new mglData(d->Min(dir));	}
//-----------------------------------------------------------------------------
//		Data manipulation functions (Fortran)
//-----------------------------------------------------------------------------
/// Smooth the data on specified direction or directions
void mgl_data_smooth_(long *d, int *Type,float *delta,const char *dir,int l)
{
	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	_DT_->Smooth(*Type,s,*delta);				delete []s;
}
/// Get array which is result of summation in given direction or directions
long mgl_data_sum_(long *d, const char *dir,int l)
{
	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	long r=long(new mglData(_DT_->Sum(s)));		delete []s;
	return r;
}
/// Integrate (cumulative summation) the data in given direction or directions
void mgl_data_integral_(long *d, const char *dir,int l)
{
	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	_DT_->Integral(s);		delete []s;
}
/// Differentiate the data in given direction or directions
void mgl_data_diff_(long *d, const char *dir,int l)
{
	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	_DT_->Diff(s);			delete []s;
}
/// Double-differentiate (like laplace operator) the data in given direction
void mgl_data_diff2_(long *d, const char *dir,int l)
{
	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	_DT_->Diff2(s);			delete []s;
}
/// Interpolate by qubic splain the data to given point x=[0...nx-1], y=[0...ny-1], z=[0...nz-1]
float mgl_data_spline_(long *d, float *x,float *y,float *z)
{	return _DT_->Spline(*x,*y,*z);	}
/// Interpolate by qubic splain the data to given point \a x,\a y,\a z which normalized in range [0, 1]
float mgl_data_spline1_(long *d, float *x,float *y,float *z)
{	return _DT_->Spline1(*x,*y,*z);	}
/// Interpolate by linear function the data to given point x=[0...nx-1], y=[0...ny-1], z=[0...nz-1]
float mgl_data_linear_(long *d, float *x,float *y,float *z)
{	return _DT_->Linear(*x,*y,*z);	}
/// Interpolate by line the data to given point \a x,\a y,\a z which normalized in range [0, 1]
float mgl_data_linear1_(long *d, float *x,float *y,float *z)
{	return _DT_->Linear1(*x,*y,*z);	}
/// Create n-th points distribution of this data values in range [v1, v2]
long mgl_data_hist_(long *d, int *n, float *v1, float *v2, int *nsub)
{	return (long)(new mglData(_DT_->Hist(*n,*v1,*v2,*nsub)));	}
/// Create n-th points distribution of this data values in range [v1, v2] with weight \a w
long mgl_data_hist_w_(long *d, long *w, int *n, float *v1, float *v2, int *nsub)
{	return (long)(new mglData(_DT_->Hist(*((mglData *)*w),*n,*v1,*v2,*nsub)));	}
/// Cumulative summation the data in given direction or directions
void mgl_data_cumsum_(long *d, const char *dir,int l)
{
	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	_DT_->CumSum(s);		delete []s;
}
/// Crop the data (cut off data edges)
void mgl_data_crop_(long *d, int *n1, int *n2, const char *dir,int)
{	_DT_->Crop(*n1,*n2,*dir);	}
/// Get array which is result of maximal values in given direction or directions
long mgl_data_max_dir_(long *d, const char *dir,int l)
{
	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	long r = long(new mglData(_DT_->Max(s)));	delete []s;
	return r;
}
/// Get array which is result of minimal values in given direction or directions
long mgl_data_min_dir_(long *d, const char *dir,int l)
{
	char *s=new char[l+1];	memcpy(s,dir,l);	s[l]=0;
	long r = long(new mglData(_DT_->Min(s)));	delete []s;
	return r;
}
//-----------------------------------------------------------------------------
