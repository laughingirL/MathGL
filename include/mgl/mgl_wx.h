/***************************************************************************
 * mgl_wx.h.cpp is part of Math Graphic Library
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
#ifndef MGL_WX_H
#define MGL_WX_H
//-----------------------------------------------------------------------------
#include <mgl/mgl_zb.h>
//-----------------------------------------------------------------------------
class wxMathGL;
class wxSpinCtrl;
int mglWxRun();
//-----------------------------------------------------------------------------
class mglGraphWX : public mglGraphZB
{
friend class wxMathGL;
public:
using mglGraphAB::Window;
	int sshow;			///< Current state of animation switch (toggle button)
	wxMathGL *WMGL;		///< Control which draw graphics
	int CurFig;			///< Current figure in the list.
	wxMainWindow *Wnd;	///< Pointer to window

	mglGraphWX();
	~mglGraphWX();

	void SetSize(int w,int h);
	void EndFrame();
	const unsigned char *GetBits();
	void Clf(mglColor Back=NC);
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ñëóæåáíûå ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/// Create a window for plotting. Now implemeted only for GLUT.
	void Window(int argc, char **argv, int (*draw)(mglGraph *gr, void *p),
				const char *title,void *par=NULL,
	  			void (*reload)(int next, void *p)=NULL, bool maximize=false);
	/// Switch on/off transparency (do not overwrite switches in user drawing function)
	void ToggleAlpha();
	/// Switch on/off lighting (do not overwrite switches in user drawing function)
	void ToggleLight();
	void ToggleZoom();	///< Switch on/off zooming by mouse
	void ToggleRotate();///< Switch on/off rotation by mouse
	void ToggleNo();	///< Switch off all zooming and rotation
	void Update();		///< Update picture by calling user drawing function
	void ReLoad(bool o);///< Reload user data and update picture
	void Adjust();		///< Adjust size of bitmap to window size
	void NextFrame();	///< Show next frame (if one)
	void PrevFrame();	///< Show previous frame (if one)
	void Animation();	///< Run slideshow (animation) of frames

protected:
	unsigned char *GG;	///< images for all frames (may be too LARGE !!!)
//	QScrollArea *scroll;	///< Scrolling area
//	QMenu *popup;			///< Popup menu
	wxSpinCtrl *tet, *phi;	///< Spin box for angles
//	QAction *anim;

	void makeMenu();		///< Create menu, toolbar and popup menu
};
//-----------------------------------------------------------------------------
/// Class is Qt widget which display MathGL graphics
#endif
//-----------------------------------------------------------------------------
