/***************************************************************************
 * mgl_glut.h is part of Math Graphic Library
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
#ifndef _MGL_GLUT_H_
#define _MGL_GLUT_H_
#include "mgl/mgl_gl.h"
//-----------------------------------------------------------------------------
/// Class allows the window creation under OpenGL with the help of GLUT library
class mglGraphGLUT : public mglGraphGL
{
friend void _mgl_display();
friend void _mgl_key_up(unsigned char ch,int ,int );
friend void _mgl_timer(int);
public:
using mglGraphAB::Window;
	mglGraphGLUT();
	~mglGraphGLUT();
	//void WriteEPS(const char *fname,const char *descr=0);	// экспорт в EPS
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Служебные ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/// Create a window for plotting. Now implemeted only for GLUT.
	void Window(int argc, char **argv, int (*draw)(mglGraph *gr, void *p),
						const char *title,void *par=NULL,
			   			void (*reload)(int next, void *p)=NULL, bool maximize=false);
	/// Switch on/off transparency (do not overwrite switches in user drawing function)
	void ToggleAlpha();
	/// Switch on/off lighting (do not overwrite switches in user drawing function)
	void ToggleLight();
	void ToggleNo();	///< Switch off all zooming and rotation
	void Update();		///< Update picture by calling user drawing function
	void ReLoad(bool o);///< Reload user data and update picture
	void NextFrame();	///< Show next frame (if one)
	void PrevFrame();	///< Show previous frame (if one)
	void Animation();	///< Run slideshow (animation) of frames
private:
	int curr_fig;	///< Current figure in the list.
	int tt;			///< Temporal variable
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
