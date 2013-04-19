/***************************************************************************
 * define.h is part of Math Graphic Library
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
#ifndef _MGL_DEFINE_H_
#define _MGL_DEFINE_H_
//-----------------------------------------------------------------------------
#include "mgl2/config.h"
#include "mgl2/dllexport.h"

#define MGL_VER2 	1.3
//-----------------------------------------------------------------------------
#ifdef WIN32 //_MSC_VER needs this before math.h
#define	_USE_MATH_DEFINES
#endif

#ifdef MGL_SRC
#if MGL_HAVE_ZLIB
#include <zlib.h>
#ifndef Z_BEST_COMPRESSION
#define Z_BEST_COMPRESSION 9
#endif
#else
#define gzFile	FILE*
#define gzread(fp,buf,size)	fread(buf,1,size,fp)
#define gzopen	fopen
#define gzclose	fclose
#define gzprintf	fprintf
#define gzgets(fp,str,size)	fgets(str,size,fp)
#define gzgetc	fgetc
#endif
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#if defined(_MSC_VER) || defined(__BORLANDC__)
#define fmin(a,b)	((a)<(b))?(a):(b)
#define fmax(a,b)	((a)>(b))?(a):(b)
#endif

#if defined(_MSC_VER)
#define hypot	_hypot
#define getcwd	_getcwd
#define chdir	_chdir // BORLAND has chdir
#define snprintf _snprintf
#endif

#if defined(_MSC_VER) || defined(__BORLANDC__)
#include <float.h>

const unsigned long mgl_nan[2] = {0xffffffff, 0x7fffffff};
#define NANd	(*(double*)mgl_nan)
#define NANf	(*(float*)&(mgl_nan[1]))
#if MGL_USE_DOUBLE
#define NAN		NANd
#else
#define NAN		NANd
#endif
#endif

#ifndef M_PI
#define M_PI	3.14159265358979323846  /* pi */
#endif
//-----------------------------------------------------------------------------
#ifdef WIN32
#define mglprintf    _snwprintf
#else
#define mglprintf    swprintf
#endif
//#define FLT_EPS	1.1920928955078125e-07
//-----------------------------------------------------------------------------
#if MGL_USE_DOUBLE
typedef double mreal;
#define MGL_EPSILON	(1.+1e-10)
#else
typedef float mreal;
#define MGL_EPSILON	(1.+1e-5)
#endif
#define MGL_FEPSILON	(1.+1e-5)
//-----------------------------------------------------------------------------
#ifndef MGL_CMAP_COLOR
#define MGL_CMAP_COLOR	32
#endif
//-----------------------------------------------------------------------------
#ifndef MGL_DEF_VIEWER
#define MGL_DEF_VIEWER "evince"
#endif
//-----------------------------------------------------------------------------
#define mgl_min(a,b)	(((a)>(b)) ? (b) : (a))
#define mgl_max(a,b)	(((a)>(b)) ? (a) : (b))
#define mgl_isnan(a)	((a)!=(a))
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
enum{	// types of predefined curvelinear coordinate systems
	mglCartesian = 0,	// no transformation
	mglPolar,
	mglSpherical,
	mglParabolic,
	mglParaboloidal,
	mglOblate,
	mglProlate,
	mglElliptic,
	mglToroidal,
	mglBispherical,
	mglBipolar,
	mglLogLog,
	mglLogX,
	mglLogY
};
//-----------------------------------------------------------------------------
// types of drawing
#define MGL_DRAW_WIRE	0	// fastest, no faces
#define MGL_DRAW_FAST	1	// fast, no color interpolation
#define MGL_DRAW_NORM	2	// high quality, slower
#define MGL_DRAW_LMEM	4	// low memory usage (direct to pixel)
//-----------------------------------------------------------------------------
enum{	// Codes for warnings/messages
	mglWarnNone = 0,// Everything OK
	mglWarnDim,		// Data dimension(s) is incompatible
	mglWarnLow, 		// Data dimension(s) is too small
	mglWarnNeg,	 	// Minimal data value is negative
	mglWarnFile, 	// No file or wrong data dimensions
	mglWarnMem,		// Not enough memory
	mglWarnZero, 	// Data values are zero
	mglWarnLeg,		// No legend entries
	mglWarnSlc,		// Slice value is out of range
	mglWarnCnt,		// Number of contours is zero or negative
	mglWarnOpen, 	// Couldn't open file
	mglWarnLId,		// Light: ID is out of range
	mglWarnSize, 	// Setsize: size(s) is zero or negative
	mglWarnFmt,		// Format is not supported for that build
	mglWarnTern, 	// Axis ranges are incompatible
	mglWarnNull, 	// Pointer is NULL
	mglWarnSpc,		// Not enough space for plot
	mglWarnEnd		// Maximal number of warnings (must be last)
};
//-----------------------------------------------------------------------------
#define MGL_DEF_PAL	"bgrcmyhlnqeupH"	// default palette
#define MGL_DEF_SCH	"BbcyrR"	// default palette
#define MGL_COLORS	"kwrgbcymhWRGBCYMHlenpquLENPQU"
//-----------------------------------------------------------------------------
#define MGL_TRANSP_NORM		0x000000
#define MGL_TRANSP_GLASS 	0x000001
#define MGL_TRANSP_LAMP		0x000002
#define MGL_ENABLE_CUT		0x000004 	///< Flag which determines how points outside bounding box are drown.
#define MGL_ENABLE_RTEXT 	0x000008 	///< Use text rotation along axis
#define MGL_AUTO_FACTOR		0x000010 	///< Enable autochange PlotFactor
#define MGL_ENABLE_ALPHA 	0x000020 	///< Flag that Alpha is used
#define MGL_ENABLE_LIGHT 	0x000040 	///< Flag of using lightning
#define MGL_TICKS_ROTATE 	0x000080 	///< Allow ticks rotation
#define MGL_TICKS_SKIP		0x000100 	///< Allow ticks rotation
// flags for internal use only
#define MGL_DISABLE_SCALE	0x000200 	///< Temporary flag for disable scaling (used for axis)
#define MGL_FINISHED 		0x000400 	///< Flag that final picture (i.e. mglCanvas::G) is ready
#define MGL_USE_GMTIME		0x000800 	///< Use gmtime instead of localtime
#define MGL_SHOW_POS		0x001000 	///< Switch to show or not mouse click position
#define MGL_CLF_ON_UPD		0x002000 	///< Clear plot before Update()
#define MGL_NOSUBTICKS		0x004000 	///< Disable subticks drawing (for bounding box)
#define MGL_DIFFUSIVE		0x008000 	///< Use diffusive light instead of specular
#define MGL_VECT_FRAME		0x010000 	///< Use DrwDat to remember all data of frames
#define MGL_REDUCEACC		0x020000 	///< Reduce accuracy of points (to reduc size of output files)
#define MGL_PREFERVC 		0x040000 	///< Prefer vertex color instead of texture if output format supports
#define MGL_ONESIDED 		0x080000 	///< Render only front side of surfaces if output format supports (for debugging)
#define MGL_NO_ORIGIN 		0x100000 	///< Don't draw tick labels at axis origin
//-----------------------------------------------------------------------------
#ifdef __cplusplus
//-----------------------------------------------------------------------------
#include <complex>
typedef std::complex<mreal> dual;
//-----------------------------------------------------------------------------
struct mglThreadD
{
	mreal *a;		// float* array with parameters or results
	const mreal *b,*c,*d,*e;	// float* arrays with parameters
	const long *p;	// long* array with parameters
	const void *v;	// pointer to data/grapher
	int id;			// thread id
	long n;			// total number of iteration
	const char *s;
};
struct mglThreadC
{
	dual *a;		// dual* array with parameters or results
	const dual *b,*c,*d,*e;	// dual* arrays with parameters
	const long *p;	// long* array with parameters
	const void *v;	// pointer to data/grapher
	int id;			// thread id
	long n;			// total number of iteration
	const char *s;
};
struct mglThreadV
{
	mreal *a;		// float* array with parameters or results
	dual *aa;		// dual* array with parameters or results
	const void *b,*c;	// float* arrays with parameters
	const mreal *d;	// float* arrays with parameters
	const long *p;	// long* array with parameters
	const void *v;	// pointer to data/grapher
	int id;			// thread id
	long n;			// total number of iteration
};
struct mglThreadT
{
	void *a; 		// dual* or mreal* array with input or results
	double *b; 		// dual* array with input or results
	const long *p;	// long* array with parameters
	const void *v;	// pointer to table/parameter
	void **w; 		// pointer to workspace
	int id;			// thread id
	long n;			// total number of iteration
	const void *re,*im;
};
/// Start several thread for the task
void MGL_EXPORT mglStartThread(void *(*func)(void *), void (*post)(mglThreadD *,mreal *), long n,
					mreal *a=0, const mreal *b=0, const mreal *c=0, const long *p=0,
					const void *v=0, const mreal *d=0, const mreal *e=0, const char *s=0);
void MGL_EXPORT mglStartThreadV(void *(*func)(void *), long n, mreal *a, const void *b=0,
					const void *c=0, const long *p=0, const void *v=0, const mreal *d=0);
void MGL_EXPORT mglStartThreadV(void *(*func)(void *), long n, dual *a, const void *b=0,
					const void *c=0, const long *p=0, const void *v=0, const mreal *d=0);
void MGL_EXPORT mglStartThreadC(void *(*func)(void *), void (*post)(mglThreadC *,dual *), long n,
					dual *a=0, const dual *b=0, const dual *c=0, const long *p=0,
					const void *v=0, const dual *d=0, const dual *e=0, const char *s=0);
void MGL_EXPORT mglStartThreadT(void *(*func)(void *), long n, void *a, double *b, const void *v=0,
					void **w=0, const long *p=0, const void *re=0, const void *im=0);
MGL_EXPORT extern int mglNumThr;		///< Number of thread for plotting and data handling
//-----------------------------------------------------------------------------
extern "C" {
#else
#include <complex.h>
#if MGL_USE_DOUBLE
typedef double _Complex dual;
#else
typedef float _Complex dual;
#endif
#endif
/// Get RGB values for given color id or fill by -1 if no one found
void MGL_EXPORT mgl_chrrgb(char id, float rgb[3]);
/// Check if string contain color id and return its number
long MGL_EXPORT mgl_have_color(const char *stl);
/// Find symbol in string excluding {} and return its position or NULL
const char *mglchr(const char *str, char ch);
/// Set number of thread for plotting and data handling
void MGL_EXPORT mgl_set_num_thr(int n);
void MGL_EXPORT mgl_test_txt(const char *str, ...);
void MGL_EXPORT mgl_set_test_mode(int enable);
/// Remove spaces at begining and at the end of the string
void MGL_EXPORT mgl_strtrim(char *str);
void MGL_EXPORT mgl_wcstrim(wchar_t *str);
/** Change register to lowercase (only for ANSI symbols) */
void MGL_EXPORT mgl_strlwr(char *str);
void MGL_EXPORT mgl_wcslwr(wchar_t *str);
/// Convert wchar_t* string into char* one
void MGL_EXPORT mgl_wcstombs(char *dst, const wchar_t *src, int size);
#ifdef __cplusplus
}
#endif
//#if MGL_HAVE_PTHREAD && defined(MGL_SRC)
#if MGL_HAVE_PTHREAD
#include <pthread.h>
#define MGL_PUSH(a,v,m)		{pthread_mutex_lock(&m);	a.push_back(v);	pthread_mutex_unlock(&m);}
#else
#define MGL_PUSH(a,v,m)		a.push_back(v);
#endif
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
