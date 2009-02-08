/***************************************************************************
 * mgl_eps.h is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
//-----------------------------------------------------------------------------
#ifndef _MGL_PS_H_
#define _MGL_PS_H_
#include "mgl/mgl_ab.h"
//-----------------------------------------------------------------------------
struct mglGraphPS;
struct mglPrim
{
	float x[4], y[4], zz[4];	// coordinates of corners
	float z;			// z-position
	float s;			// size (if applicable)
	float w;			// width (if applicable)
	float c[4];			// color (RGBA)
	char m;				// mark (if applicable)
	int type;			// type of primitive (0 - point, 1 - line, 2 - trig, 3 - quad)
	int style;			// style of pen
	unsigned short dash;// real pen dashing
	void Draw(mglGraphPS *gr);
	void DrawGL();
	bool IsSame(float wp,float *cp,int st);
	void SetStyle(unsigned PDef, int pPos);
	mglPrim(int t=0)	{	memset(this,0,sizeof(mglPrim));	type = t;	c[3]=1;	};
};
//-----------------------------------------------------------------------------
/// Class implement the creation of different mathematical plots for exporting in PostScript format
class mglGraphPS : public mglGraphAB
{
friend struct mglPrim;
public:
	mglGraphPS(int w=600, int h=400);
	~mglGraphPS();
	void WriteEPS(const char *fname,const char *descr=0);
	void WriteSVG(const char *fname,const char *descr=0);
	virtual void Finish();
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
