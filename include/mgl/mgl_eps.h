/* mgl_eps.h is part of Math Graphic Library
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
//-----------------------------------------------------------------------------
#ifndef _MGL_PS_H_
#define _MGL_PS_H_
#include "mgl/mgl_ab.h"
//-----------------------------------------------------------------------------
struct mglPrim;
/// Class implement the creation of different mathematical plots for exporting in PostScript format
class mglGraphPS : public mglGraphAB
{
friend struct mglPrim;
public:
	mglGraphPS(int w=600, int h=400);
	~mglGraphPS();
	void WriteEPS(const char *fname,const char *descr=0);
	void WriteSVG(const char *fname,const char *descr=0);
	void Finish();
	void Clf(mglColor  Back=WC);
	void Ball(float x,float y,float z,mglColor col=RC,float alpha=1);
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
protected:
	mglPrim *P;			///< Primitives (lines, triangles and so on)
	long pNum;			///< Actual number of primitives
	long pMax;			///< Maximal number of primitives

	void ball(float *p,float *c);
	void line_plot(float *p1,float *p2,float *c1,float *c2,bool all=false);
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
	/// add primitive to list
	void add_prim(mglPrim *a);
	/// add lightning to color
	void add_light(float *c, float n1,float n2, float n3);
	void pnt_plot(int x,int y, unsigned char c[4]);
	void mark_plot(int x,int y, char type, unsigned char cs[4]);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
