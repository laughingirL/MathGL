/***************************************************************************
 * mgl_zb.h is part of Math Graphic Library
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
#ifndef _MGL_ZB_H_
#define _MGL_ZB_H_
#include "mgl/mgl_ab.h"
//-----------------------------------------------------------------------------
/// Class implement the creation of different mathematical plots using Z-Buffer
class mglGraphZB : public mglGraphAB
{
friend void *mgl_thr_ball(void *p);
friend void *mgl_thr_dfin(void *p);
friend void *mgl_thr_ffin(void *p);
friend void *mgl_thr_quad(void *p);
friend void *mgl_thr_quaa(void *p);
friend void *mgl_thr_trig(void *p);
friend void *mgl_thr_trin(void *p);

public:
	/// Initialize ZBuffer drawing and allocate the memory for image with size [Width x Height].
	mglGraphZB(int w=600, int h=400);
	~mglGraphZB();
	/// write n-th slice (for testing)
	void WriteSlice(int n);
	void Finish();
	virtual void Clf(mglColor Back=NC);
	virtual void SetSize(int w,int h);
	void Ball(mreal x,mreal y,mreal z,mglColor col=RC,mreal alpha=1);
	void Glyph(mreal x, mreal y, mreal f, int style, long icode, char col);
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool FastNoFace;	/// Use fastest (but less accurate) scheme for drawing (without faces)
protected:
	mreal *Z;			///< Height for given level in Z-direction
	unsigned char *C;	///< Picture for given level in Z-direction

	/// Transform mglColor and alpha value to bits format
	unsigned char* col2int(mglColor c, mreal alpha,unsigned char *r);	// mglColor -> int
	/// Transform mreal color and alpha to bits format
	unsigned char* col2int(mreal *c,mreal *n,unsigned char *r);
	/// Plot point \a p with color \a c
	void ball(mreal *p,mreal *c);
	void pnt_plot(long x,long y,mreal z,unsigned char c[4]);
	void line_plot(mreal *p1,mreal *p2,mreal *c1,mreal *c2,bool all=false);
	void line_plot_s(mreal *p1,mreal *p2,mreal *c1,mreal *c2,bool all=false);
	void trig_plot(mreal *p0,mreal *p1,mreal *p2,
					mreal *c0,mreal *c1,mreal *c2);
	void trig_plot_n(mreal *p0,mreal *p1,mreal *p2,
					mreal *c0,mreal *c1,mreal *c2,
					mreal *n0,mreal *n1,mreal *n2);
	void quad_plot(mreal *p0,mreal *p1,mreal *p2,mreal *p3,
					mreal *c0,mreal *c1,mreal *c2,mreal *c3);
	void quad_plot_a(mreal *p0,mreal *p1,mreal *p2,mreal *p3,
					mreal a0,mreal a1,mreal a2,mreal a3,mreal alpha);
	void quad_plot_n(mreal *p0,mreal *p1,mreal *p2,mreal *p3,
					mreal *c0,mreal *c1,mreal *c2,mreal *c3,
					mreal *n0,mreal *n1,mreal *n2,mreal *n3);
	void mark_plot(mreal *pp, char type);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
