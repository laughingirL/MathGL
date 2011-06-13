/***************************************************************************
 * canvas_gl.h is part of Math Graphic Library
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
#ifndef MGL_CANVAS_GL_H
#define MGL_CANVAS_GL_H
/*****************************************************************************/
#ifdef __cplusplus
#include "mgl/canvas.h"

class mglCanvasGL : public mglCanvas
{
public:
	mglCanvasGL();
	virtual ~mglCanvasGL();

	virtual void SetQuality(int qual=0)	{	Quality=2;	}
	virtual void Finish();
	virtual void SetSize(int w,int h)	{}
	virtual void View(float tetX,float tetY,float tetZ);
	virtual int NewFrame();
	virtual void EndFrame();

	virtual bool Alpha(bool enable);
	virtual void Fog(float d, float dz=0.25);
	virtual bool Light(bool enable);
	virtual void Light(int n, bool enable);
	virtual void AddLight(int n,mglPoint p, char c='w', float bright=0.5, bool infty=true, float ap=0);
	virtual void Clf(mglColor Back=WC);

protected:
	virtual void line_draw(long p1, long p2);
	virtual void trig_draw(long p1, long p2, long p3, bool anorm=false);
	virtual void quad_draw(long p1, long p2, long p3, long p4);
	virtual void pnt_draw(long p);

	virtual unsigned char **GetRGBLines(long &w, long &h, unsigned char *&f, bool solid=true);
	virtual void LightScale();
	virtual void SetPen(char style,float width);
};
extern "C" {
#endif
/*****************************************************************************/
HMGL mgl_create_graph_gl();
/*****************************************************************************/
uintptr_t mgl_create_graph_gl_();
/*****************************************************************************/
#ifdef __cplusplus
}
#endif
/*****************************************************************************/
#endif

