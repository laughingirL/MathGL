/***************************************************************************
 * define.h is part of Math Graphic Library
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
//-----------------------------------------------------------------------------
#ifndef _MGL_DEFINE_H_
#define _MGL_DEFINE_H_
//-----------------------------------------------------------------------------
#ifdef WIN32 //_MSC_VER needs this before math.h
#define	_USE_MATH_DEFINES
#endif

#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MGL_VER2	0.0

//#ifdef WIN32
#ifdef _MSC_VER
#define hypot _hypot
#define getcwd _getcwd
#define isfinite _finite
#define chdir	_chdir // BORLAND has chdir
#include <float.h>

extern mglData s;const unsigned long mgl_nan[2] = {0xffffffff, 0x7fffffff};
#define NANd	(*(double*)mgl_nan)
#define NANf	(*(float*)&(mgl_nan[1]))
#if (MGL_USE_DOUBLE==1)
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
#define MGL_FLT_EPS	(1.+1e-06)
//-----------------------------------------------------------------------------
#ifndef isnan
#define isnan(a)		((a)!=(a))
#endif
//-----------------------------------------------------------------------------
#include <mgl/config.h>
//-----------------------------------------------------------------------------
#if (MGL_USE_DOUBLE==1)
typedef double mreal;
#else
typedef float mreal;
#endif
//-----------------------------------------------------------------------------
#ifndef MGL_CMAP_COLOR
#define MGL_CMAP_COLOR	32
#endif
//-----------------------------------------------------------------------------
#ifndef MGL_STACK_ENTRY
#define MGL_STACK_ENTRY	10
#endif
//-----------------------------------------------------------------------------
#ifndef MGL_DEF_VIEWER
#define MGL_DEF_VIEWER "evince"
#endif
//-----------------------------------------------------------------------------
#ifndef MGL_FONT_PATH
#ifdef WIN32
#define MGL_FONT_PATH "."	// path to fonts
#else
#define MGL_FONT_PATH "/usr/local/share/mathgl/fonts"	// path to fonts
#endif
#endif
//-----------------------------------------------------------------------------
#define mgl_min(a,b)	(((a)>(b)) ? (b) : (a))
#define mgl_max(a,b)	(((a)>(b)) ? (a) : (b))
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
	mglBipolar
};
//-----------------------------------------------------------------------------
// types of drawing
#define MGL_DRAW_WIRE	0	// fastest, no faces
#define MGL_DRAW_FAST	1	// fast, no color interpolation
#define MGL_DRAW_NORM	2	// high quality, slower
#define MGL_DRAW_LMEM	4	// low memory usage (direct to pixel)
//-----------------------------------------------------------------------------
// types of vector plot
#define MGL_VEC_COL	1	// use 2 colors only
#define MGL_VEC_LEN	2	// use fixed length
#define MGL_VEC_END	4	// draw vector to point
#define MGL_VEC_MID	8	// draw vector with point at center
#define MGL_VEC_DOT	16	// draw dot instead of vector
#define MGL_VEC_GRD	32	// enable color gradient along vector/dash
#define MGL_VECTC MGL_VEC_LEN|MGL_VEC_DOT|MGL_VEC_GRD
#define MGL_VECTL MGL_VEC_COL|MGL_VEC_DOT|MGL_VEC_GRD
//-----------------------------------------------------------------------------
enum{	// Codes for warnings/messages
	mglWarnNone = 0,// Everything OK
	mglWarnDim,		// Data dimension(s) is incompatible
	mglWarnLow, 	// Data dimension(s) is too small
	mglWarnNeg, 	// Minimal data value is negative
	mglWarnFile,	// No file or wrong data dimensions
	mglWarnMem,		// Not enough memory
	mglWarnZero,	// Data values are zero
	mglWarnLeg,		// No legend entries
	mglWarnSlc,		// Slice value is out of range
	mglWarnCnt,		// Number of contours is zero or negative
	mglWarnOpen,	// Couldn't open file
	mglWarnLId,		// Light: ID is out of range
	mglWarnSize,	// Setsize: size(s) is zero or negative
	mglWarnFmt,		// Format is not supported for that build
	mglWarnTern,	// Axis ranges are incompatible
	mglWarnNull,	// Pointer is NULL
	mglWarnSpc,		// Not enough space for plot
	mglWarnEnd		// Maximal number of warnings (must be last)
};
//-----------------------------------------------------------------------------
#ifndef MGL_DEF_PAL
#define MGL_DEF_PAL	"bgrcmyhlnqeupH"	// default palette
#endif
//-----------------------------------------------------------------------------
#define MGL_TRANSP_NORM		0x0000
#define MGL_TRANSP_GLASS	0x0001
#define MGL_TRANSP_LAMP		0x0002
#define MGL_ENABLE_CUT		0x0004	///< Flag which determines how points outside bounding box are drown.
#define MGL_ENABLE_RTEXT	0x0008	///< Use text rotation along axis
#define MGL_AUTO_FACTOR		0x0010	///< Enable autochange PlotFactor
#define MGL_ENABLE_ALPHA	0x0020	///< Flag that Alpha is used
#define MGL_ENABLE_LIGHT	0x0040	///< Flag of using lightning
#define MGL_TICKS_ROTATE	0x0080	///< Allow ticks rotation
#define MGL_TICKS_SKIP		0x0100	///< Allow ticks rotation
// flags for internal use only
#define MGL_DISABLE_SCALE	0x0200	///< Temporary flag for disable scaling (used for axis)
#define MGL_FINISHED		0x0400	///< Flag that final picture \a mglCanvas::G is ready
#define MGL_AUTO_CLF		0x0800	///< Clear canvas between drawing
#define MGL_SHOW_POS		0x1000	///< Switch to show or not mouse click position
#define MGL_CLF_ON_UPD		0x2000	///< Clear plot before Update()
//-----------------------------------------------------------------------------
//#define mgl_realloc(T,o,no,nn) {T *_tmp = new T[nn]; memcpy(_tmp,o,(no)*sizeof(T)); delete []o; o=_tmp;}
//-----------------------------------------------------------------------------
#ifdef __cplusplus
template<typename T> int mgl_cmp(const void *x, const void *y)
{
	const T *a=(const T*)x, *b=(const T*)y;
	return *a>*b ? -1:1;
}
template<typename T> class mglArray {
public:
	mglArray()	{	dat = 0;	len=pos=0;	}
	~mglArray()	{	free(dat); }
	mglArray(const mglArray<T>& A)	{	*this=A;	}
	mglArray<T>& operator= (const mglArray<T>& A)
	{	free(dat);	len=A.len;	pos=A.pos;	dat=(T*)realloc(0,sizeof(T));
		memccpy(dat,A.dat,len*sizeof(T));	return *this;	}
	T const& operator[](int i) const{	return dat[i]; }
	T&       operator[](int i)		{	return dat[i]; }
	long size() const	{	return pos;	}
	void erase(long p)	{	if(p>0 && p<=len)	pos = p-1;	}
	T const& pop_back()
	{	T const&d=dat[pos];	if(pos>0)	pos--;	return d;	}
	void push_back(T const& d)
	{	reserve(pos+2-len);	dat[pos]=d;	pos++;	}
	void reserve(long d)
	{	if(d>0)	{	len+=d;	dat=(T*)realloc(dat,len*sizeof(T));	}	}
	void clear()
	{	pos=0;	len=1;	if(dat)	free(dat);	dat = (T*)realloc(0,sizeof(T));	}
	void sort()
	{	qsort(dat,pos,sizeof(T),mgl_cmp<T>);	}
private:
	long len, pos;
	T*  dat;
};
//-----------------------------------------------------------------------------
struct mglThread
{
	mreal *a;		// float* array with parameters or results
	const mreal *b,*c,*d,*e;	// float* array with parameters
	const long *p;		// long* array with parameters
	void *v;		// pointer to data/grapher
	int id;			// thread id
	long n;			// total number of iteration
	char *s;
};
/// Start several thread for the task
void mglStartThread(void *(*func)(void *), void (*post)(mglThread *,mreal *), long n,
					mreal *a=0, const mreal *b=0, const mreal *c=0, const long *p=0,
					void *v=0, const mreal *d=0, const mreal *e=0, char *s=0);
void mglSetNumThr(int n=0);	///< Set number of thread for plotting and data handling
extern int mglNumThr;		///< Number of thread for plotting and data handling
//-----------------------------------------------------------------------------
class mglGraph;
class mglBase;
/// Class for drawing in windows (like, mglCanvasFL, mglCanvasQT and so on)
/// Make inherited class and redefine Draw() function if you don't want to use function pointers.
struct mglDraw
{
	virtual int Draw(mglGraph *)	{	return 0;	};
	virtual void Reload(int)	{};
};
int mgl_draw_class(mglBase *gr, void *p);
void mgl_reload_class(int next, void *p);
typedef int (*draw_func)(mglGraph *gr);
int mgl_draw_graph(mglBase *gr, void *p);
//-----------------------------------------------------------------------------
extern "C" {
#endif
void mgl_test_txt(const char *str, ...);
void mgl_set_test_mode(int enable);
#ifdef __cplusplus
}
#endif
#ifdef HAVE_PTHREAD
#include <pthread.h>
#define MGL_PUSH(a,v,m)		{pthread_mutex_lock(&m);	a.push_back(v);	pthread_mutex_unlock(&m);}
#else
#define MGL_PUSH(a,v,m)		a.push_back(v);
#endif
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
