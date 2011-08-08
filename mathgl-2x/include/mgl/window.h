/***************************************************************************
 * window.h is part of Math Graphic Library
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
#ifndef _MGL_WINDOW_H_
#define _MGL_WINDOW_H_
/*****************************************************************************/
#include "mgl/mgl.h"
//-----------------------------------------------------------------------------
class mglWindow : public mglGraph
{
protected:
	int wnd;	///< Type of window
public:
	mglWindow(int kind, int (*draw)(HMGL gr, void *p), const char *title="MathGL", void *par=NULL) : mglGraph(-1)
	{
		wnd=kind;
		if(wnd==1)	gr = mgl_create_graph_qt(draw,title,par);
		else		gr = mgl_create_graph_fltk(draw,title,par);
	}
	mglWindow(int kind, int (*draw)(mglGraph *gr), const char *title="MathGL") : mglGraph(-1)
	{
		wnd=kind;
		if(wnd==1)	gr = mgl_create_graph_qt(mgl_draw_graph,title,(void*)draw);
		else		gr = mgl_create_graph_fltk(mgl_draw_graph,title,(void*)draw);
	}
	mglWindow(int kind=0, mglDraw *dr=NULL, const char *title="MathGL") : mglGraph(-1)
	{
		wnd=kind;
		if(wnd==1)	gr = mgl_create_graph_qt(mgl_draw_class,title,dr);
		else		gr = mgl_create_graph_fltk(mgl_draw_class,title,dr);
	}
	inline void Run()
	{	if(wnd==1)	mgl_qt_run();	else	mgl_fltk_run();	}

	inline 	void ToggleAlpha()	///< Switch on/off transparency (do not overwrite user settings)
	{	mgl_wnd_toggle_alpha(gr);	}
	inline void ToggleLight()	///< Switch on/off lighting (do not overwrite user settings)
	{	mgl_wnd_toggle_light(gr);	}
//	inline void ToggleZoom()	///< Switch on/off zooming by mouse
//	{	mgl_wnd_toggle_zoom(gr);	}
	inline void ToggleRotate()	///< Switch on/off rotation by mouse
	{	mgl_wnd_toggle_rotate(gr);	}
	inline void ToggleNo()		///< Switch off all zooming and rotation
	{	mgl_wnd_toggle_no(gr);	}
	inline void Update()		///< Update picture by calling user drawing function
	{	mgl_wnd_update(gr);	}
	inline void ReLoad(bool o)	///< Reload user data and update picture
	{	mgl_wnd_reload(gr, o);	}
	inline void Adjust()		///< Adjust size of bitmap to window size
	{	mgl_wnd_adjust(gr);	}
	inline void NextFrame()		///< Show next frame (if one)
	{	mgl_wnd_next_frame(gr);	}
	inline void PrevFrame()		///< Show previous frame (if one)
	{	mgl_wnd_prev_frame(gr);	}
	inline void Animation()		///< Run slideshow (animation) of frames
	{	mgl_wnd_animation(gr);	}

	inline void SetDelay(float dt)	///< Delay for animation in seconds
	{	mgl_wnd_set_delay(gr, dt);	}
	inline void Setup(bool autoclf, bool showpos, bool clf_upd)
	{	mgl_setup_window(gr, autoclf, showpos, clf_upd);	}
	inline mglPoint LastMousePos()	///< Last mouse position
	{	mglPoint p;	mgl_get_last_mouse_pos(gr,&p.x,&p.y,&p.z);	return p;	}
};
//-----------------------------------------------------------------------------
#endif
