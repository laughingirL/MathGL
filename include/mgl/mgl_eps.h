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
	mreal x[4], y[4], zz[4];	// coordinates of corners
	mreal z;			// z-position
	mreal s;			// size (if applicable)
	mreal w;			// width (if applicable)
	mreal c[4];			// color (RGBA)
	char m;				// mark (if applicable)
	int type;			// type of primitive (0 - point, 1 - line, 2 - trig, 3 - quad)
	int style;			// style of pen
	unsigned short dash;// mreal pen dashing
	void Draw(mglGraphPS *gr);
	void DrawGL();
	bool IsSame(mreal wp,mreal *cp,int st);
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
	void Ball(mreal x,mreal y,mreal z,mglColor col=RC,mreal alpha=1);
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
protected:
	mglPrim *P;			///< Primitives (lines, triangles and so on)
	long pNum;			///< Actual number of primitives
	long pMax;			///< Maximal number of primitives

	void ball(mreal *p,mreal *c);
	void line_plot(mreal *p1,mreal *p2,mreal *c1,mreal *c2,bool all=false);
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
	/// add primitive to list
	void add_prim(mglPrim *a);
	/// add lightning to color
	void add_light(mreal *c, mreal n1,mreal n2, mreal n3);
	void pnt_plot(int x,int y, unsigned char c[4]);
	void mark_plot(int x,int y, char type, unsigned char cs[4]);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
