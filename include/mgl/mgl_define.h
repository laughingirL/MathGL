#ifdef _MSC_VER
#define	_USE_MATH_DEFINES
#include <float.h>
const unsigned long mgl_nan[2] = {0xffffffff, 0x7fffffff};
#define NANd	(*(double*)mgl_nan)
#define NANf	(*(float*)&(mgl_nan[1]))
#define NAN		NANd
#define chdir	_chdir
#endif

#ifdef WIN32
#define swprintf    _snwprintf
#endif

#ifndef isnan
#define isnan(a)		((a)!=(a))
#endif

#include <mgl/config.h>

#if(MGL_USE_DOUBLE==1)
typedef double mreal;
#else
typedef float mreal;
#endif

#define mgl_min(a,b)	(((a)>(b)) ? (b) : (a))
#define mgl_max(a,b)	(((a)>(b)) ? (a) : (b))
