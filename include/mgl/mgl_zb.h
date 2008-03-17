/* mgl_zb.h is part of Math Graphic Library
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
//-----------------------------------------------------------------------------
#ifndef _MGL_ZB_H_
#define _MGL_ZB_H_
#include "mgl/mgl_ab.h"
//-----------------------------------------------------------------------------
/// Class implement the creation of different mathematical plots using Z-Buffer
class mglGraphZB : public mglGraphAB
{
public:
	/// Initialize ZBuffer drawing and allocate the memory for image with size [Width x Height].
	mglGraphZB(int w=600, int h=400);
	~mglGraphZB();
	/// write n-th slice (for testing)
	void WriteSlice(int n);
	void Finish();
	virtual void Clf(mglColor Back=NC);
	virtual void SetSize(int w,int h);
	void Ball(float x,float y,float z,mglColor col=RC,float alpha=1);
	void Glyph(float x,float y, float f, int nt, const short *trig, int nl, const short *line);
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool FastNoFace;	/// Use fastest (but less accurate) scheme for drawing (without faces)
protected:
	float *Z;			///< Height for given level in Z-direction
	unsigned char *C;	///< Picture for given level in Z-direction

	/// Transform mglColor and alpha value to bits format
	unsigned char* col2int(mglColor c, float alpha,unsigned char *r);	// mglColor -> int
	/// Transform float color and alpha to bits format
	unsigned char* col2int(float *c,float *n,unsigned char *r);
	/// Plot point \a p with color \a c
	void ball(float *p,float *c);
	void pnt_plot(long x,long y,float z,unsigned char c[4]);
	void line_plot(float *p1,float *p2,float *c1,float *c2,bool all=false);
	void line_plot_s(float *p1,float *p2,float *c1,float *c2,bool all=false);
	void trig_plot(float *p0,float *p1,float *p2,
					float *c0,float *c1,float *c2);
	void trig_plot_n(float *p0,float *p1,float *p2,
					float *c0,float *c1,float *c2,
					float *n0,float *n1,float *n2);
	void quad_plot(float *p0,float *p1,float *p2,float *p3,
					float *c0,float *c1,float *c2,float *c3);
	void quad_plot_a(float *p0,float *p1,float *p2,float *p3,
					float a0,float a1,float a2,float a3,float alpha);
	void quad_plot_n(float *p0,float *p1,float *p2,float *p3,
					float *c0,float *c1,float *c2,float *c3,
					float *n0,float *n1,float *n2,float *n3);
	void mark_plot(float *pp, char type);
	void font_line(float *p, unsigned char *r,bool thin=true);
//	void cloud_plot(int nx,int ny,int nz,float *pp,float *a,float alpha);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
