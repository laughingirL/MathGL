/***************************************************************************
 * mgl_gl.h is part of Math Graphic Library
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
#ifndef _MGL_GL_H_
#define _MGL_GL_H_
#ifdef _MSC_VER
#include <windows.h>
#endif
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include "mgl/mgl_ab.h"
//-----------------------------------------------------------------------------
/// Class implements the creation of different mathematical plots under OpenGL
class mglGraphGL : public mglGraphAB
{
public:
	mglGraphGL();
	~mglGraphGL();
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void Flush() {glFlush();};
	void Finish(){glFinish();};
	void Alpha(bool enable);
	void Light(bool enable);
	void Light(int n, bool enable);
	void Fog(float d, float dz=0.25);
	void Light(int n,mglPoint p, mglColor c=NC, float br=0.5, bool infty=true);
	void View(float tetX,float tetY,float tetZ);
	void Clf(mglColor Back=NC);
	int NewFrame();
	void EndFrame();
	void InPlot(float x1,float x2,float y1,float y2,bool rel=false);
	void SetSize(int ,int ){};

	void Ball(float x,float y,float z,mglColor col=RC,float alpha=1);
//	void Glyph(float x,float y, float f, int nt, const short *trig, int nl, const short *line);
protected:
	mglColor def_col;

//	void DefColor(mglColor c, float alpha=-1);
	void Pen(mglColor col, char style,float width);
	unsigned char **GetRGBLines(long &w, long &h, unsigned char *&f, bool solid=true);

	void ball(float *p,float *c);

	void LightScale();
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
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
