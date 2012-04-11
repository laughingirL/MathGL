/***************************************************************************
 * mgl_glut.h is part of Math Graphic Library
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
#ifndef _MGL_GLUT_H_
#define _MGL_GLUT_H_
#ifdef __cplusplus
#include "mgl2/opengl.h"
#include "mgl2/window.h"
//-----------------------------------------------------------------------------
extern "C" {
#endif
/*****************************************************************************/
void _mgl_key_up(unsigned char ch,int ,int );
HMGL mgl_create_graph_glut(int (*draw)(HMGL gr, void *p), const char *title, void *par, void (*load)(void *p));
/*****************************************************************************/
#ifdef __cplusplus
}
/// Class allows the window creation under OpenGL with the help of GLUT library
class mglCanvasGLUT : public mglCanvasGL
{
friend void _mgl_display();
friend void _mgl_key_up(unsigned char ch,int ,int );
friend void _mgl_timer(int);
public:
	float Delay;	///< Delay for animation in seconds
	bool AutoClf;		///< Clear canvas between drawing

	mglCanvasGLUT();
	mglCanvasGLUT(int (*draw)(mglGraph *gr, void *p), const char *title, void *par=NULL,
				void (*reload)(int next, void *p)=NULL, bool maximize=false);
	virtual ~mglCanvasGLUT();
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ��������� ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/// Create a window for plotting. Now implemeted only for GLUT.
	void Window(int argc, char **argv, int (*draw)(mglBase *gr, void *p),
						const char *title,void *par=NULL,
			   			void (*reload)(void *p)=NULL, bool maximize=false);
	void Window(int argc, char **argv, int (*draw)(mglGraph *gr),
				const char *title, bool maximize=false)
	{	Window(argc,argv,mgl_draw_graph,title,(void*)draw,0,maximize);	}
	/// Create a window for plotting based on class mglDraw.
	void Window(int argc, char **argv, const char *title, mglDraw *draw, bool maximize=false)
	{	Window(argc, argv, mgl_draw_class, title, draw, mgl_reload_class, maximize);	}
	/// Switch on/off transparency (do not overwrite switches in user drawing function)
	void ToggleAlpha()	{	_mgl_key_up('r',0,0);	}
	/// Switch on/off lighting (do not overwrite switches in user drawing function)
	void ToggleLight()	{	_mgl_key_up('f',0,0);	}
	void ToggleNo()		{	_mgl_key_up('n',0,0);	}	///< Switch off all zooming and rotation
	void Update()		{	_mgl_key_up(' ',0,0);	}	///< Update picture by calling user drawing function
	void ReLoad(bool o)	{	_mgl_key_up(o?']':'[',0,0);	}	///< Reload user data and update picture
	void NextFrame()	{	_mgl_key_up('.',0,0);	}	///< Show next frame (if one)
	void PrevFrame()	{	_mgl_key_up(',',0,0);	}	///< Show previous frame (if one)
	void Animation()	{	_mgl_key_up('m',0,0);	}	///< Run slideshow (animation) of frames
private:
	void (*LoadFunc)(void *par);
	void *FuncPar;		///< Parameters for drawing function mglCanvas::DrawFunc.
	/// Drawing function for window procedure. It should return the number of frames.
	int (*DrawFunc)(mglBase *gr, void *par);
	int NumFig;		///< Number of figures in the list. If 0 then no list and mglCanvas::DrawFunc will called for each drawing.
	int curr_fig;	///< Current figure in the list.
	int tt;			///< Temporal variable
};
//-----------------------------------------------------------------------------
class mglGLUT: public mglGraph
{
public:
	mglGLUT(int (*draw)(HMGL gr, void *p), const char *title="MathGL", void *par=0, void (*load)(void *p)=0) : mglGraph(-1)
	{	gr = mgl_create_graph_glut(draw,title,par,load);	}
	mglGLUT(int (*draw)(mglGraph *gr), const char *title="MathGL") : mglGraph(-1)
	{	gr = mgl_create_graph_glut(mgl_draw_graph,title,(void*)draw,0);	}
	mglGLUT(mglDraw *dr=0, const char *title="MathGL") : mglGraph(-1)
	{	mgl_create_graph_glut(mgl_draw_class,title,dr,0);	}
};
//-----------------------------------------------------------------------------
#endif
#endif
