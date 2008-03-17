/* mgl_gl.h is part of Math Graphic Library
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
#include "mgl/mgl.h"
//-----------------------------------------------------------------------------
/// Class implements the creation of different mathematical plots under OpenGL
class mglGraphGL : public mglGraph
{
public:
	mglGraphGL();
	~mglGraphGL();
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ —лужебные ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void Flush() {glFlush();};		// заканчивает рисование
	void Finish(){glFinish();};		// заканчивает рисование
	void Alpha(bool enable);		// вкл/выкл прозрачности
	void Light(bool enable);		// вкл/выкл освещенности
	void Light(int n, bool enable);
	void Fog(float d, float dz=0.25);
	// добавл€ет источник света
	void Light(int n,mglPoint p, mglColor c=NC, float br=0.5, bool infty=true);
	void View(float tetX,float tetY,float tetZ);// углы зрени€
	void Clf(mglColor Back=NC);					// очистка области рисовани€
	void InPlot(float x1,float x2,float y1,float y2);	// подграфик произвольно
	void Aspect(float Ax,float Ay,float Az);			// введение аспекта дл€ подграфика
	void RotateN(float Tet,float x,float y,float z);	// вращение относительно оси
	void Perspective(float a);
	//	рисование точки (м€чика)
	void Ball(float x,float y,float z,mglColor col=RC,float alpha=1);
	void Putsw(mglPoint p,const wchar_t *text,const char *font=0,float size=-1,char dir=0,float shift=0);
	float Putsw(mglPoint p,mglPoint l,const wchar_t *text,char font='t',float size=-1);
	// рисование точки разного типа
	void Mark(float x,float y,float z,char mark='.');
	void Glyph(float x,float y, float f, int nt, const short *trig, int nl, const short *line);
	// рисование линейки цвета сбоку от графика
	void Colorbar(const char *sch=0,int where=0);
	/// Create new frame.
	int NewFrame(int id=-1);
	/// Finish frame drawing
	void EndFrame();
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
protected:
	float tetx, tety, tetz;	///< rotation angle values
	float back[4];
	float tetr[8];
	mglColor def_col;

	void DefColor(mglColor c, float alpha=-1);
	void Pen(mglColor col, char style,float width);

	unsigned char **GetRGBLines(long &w, long &h, unsigned char *&f, bool solid=true);
	void ball(float *p,float *c);
	void arrow_plot(float *p1,float *p2,char st);
	void curv_plot(long n,float *pp,bool *tt);
	void curv_plot(long n,float *pp,float *cc,bool *tt);
	void curv_plot(long n,float *pp,bool *tt,long *nn);
	void mesh_plot(long n,long m,float *pp,float *cc,bool *tt,int how);
	void surf_plot(long n,long m,float *pp,float *cc,bool *tt);
	void axial_plot(long n,float *pp,long *nn,long np, bool wire);
	void cloud_plot(long nx,long ny,long nz,float *pp,float *a,float alpha);
	void boxs_plot(long n, long m, float *pp, mglColor *cc,
				bool *tt,float Alpha, bool line);
	void surf3_plot(long n,long m,long *kx1,long *kx2, long *ky1,long *ky2,long *kz,
					float *pp,float *cc,float *kk,float *nn,bool wire);
	void quads_plot(long n, float *pp, float *cc, bool *tt);
	void trigs_plot(long n, long *nn, long m, float *pp, float *cc, bool *tt,bool wire);
	void lines_plot(long n, float *pp, float *cc, bool *tt);
	void normal(float *p1,float *p2,float *p3);
	void normal(long nx,long ny,float *pp,float *nn);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
