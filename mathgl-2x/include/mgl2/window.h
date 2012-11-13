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
#ifndef _MGL_WINDOW_H_
#define _MGL_WINDOW_H_

#include "mgl2/mgl.h"
//-----------------------------------------------------------------------------
/// Class for drawing in windows (like, mglCanvasFL, mglCanvasQT and so on)
/// Make inherited class and redefine Draw() function if you don't want to use function pointers.
struct mglDraw
{
	virtual int Draw(mglGraph *)=0;	///< Function for drawing
	virtual void Reload()	{}		///< Function for reloading data
	virtual void Click()	{}		///< Callback function on mouse click
	virtual ~mglDraw()	{}
#if MGL_HAVE_PTHREAD
	pthread_t thr;
	bool running;
	mglDraw()	{	running=false;	}
	virtual void Calc()	{}			///< Function for calculations
	inline void Run()				///< Run calculations in other thread
	{	mgl_draw_thr(this);	}
#else
	mglDraw(){}
#endif
};
//-----------------------------------------------------------------------------
extern "C" {
int mgl_draw_graph(HMGL gr, void *p);
// NOTE: mgl_draw_class() and mgl_draw_load() use mglWindow* only. Don't use it with inherited classes
int mgl_draw_class(HMGL gr, void *p);
void mgl_click_class(void *p);
void mgl_reload_class(void *p);
}
//-----------------------------------------------------------------------------
#if MGL_HAVE_QT
#define MGL_WND_KIND	1
#else
#define MGL_WND_KIND	0
#endif
//-----------------------------------------------------------------------------
/// Wrapper class for windows displaying graphics
class mglWindow : public mglGraph
{
friend int mgl_draw_class(HMGL gr, void *p);
friend void mgl_click_class(void *p);
friend void mgl_reload_class(void *p);
protected:
	mglDraw *dr;
	int wnd;	///< Type of window
public:
	mglWindow(const char *title="MathGL") : mglGraph(-1)
	{	wnd=0;	dr=0;	gr = mgl_create_window(wnd, 0,title,0,0);	mgl_use_graph(gr,1);	}
	inline int RunThr()		///< Run main loop for event handling in separate thread (for FLTK only)
	{	return wnd==0 ? mgl_fltk_thr():0;	}
#ifndef SWIG
	mglWindow(int (*draw)(HMGL gr, void *p), const char *title="MathGL", void *par=NULL, int kind=MGL_WND_KIND, void (*load)(void *p)=0) : mglGraph(-1)
	{
		wnd=kind;	dr=0;
		gr = mgl_create_window(wnd, draw,title,par,load);
		mgl_use_graph(gr,1);
	}
	mglWindow(int (*draw)(mglGraph *gr), const char *title="MathGL", int kind=MGL_WND_KIND) : mglGraph(-1)
	{
		wnd=kind;	dr=0;
		gr = mgl_create_window(wnd, mgl_draw_graph,title,(void*)draw,0);
		mgl_use_graph(gr,1);
	}
	mglWindow(mglDraw *draw, const char *title="MathGL", int kind=MGL_WND_KIND) : mglGraph(-1)
	{
		wnd=kind;	dr=draw;
		gr = mgl_create_window(wnd, mgl_draw_class,title,this,mgl_reload_class);
		mgl_use_graph(gr,1); 	mgl_set_click_func(gr, mgl_click_class);
	}
	inline int Run()			///< Run main loop for event handling
	{	return mgl_wnd_run(wnd);	}
#endif

	inline void ToggleAlpha()	///< Switch on/off transparency (do not overwrite user settings)
	{	mgl_wnd_toggle_alpha(gr);	}
	inline void ToggleLight()	///< Switch on/off lighting (do not overwrite user settings)
	{	mgl_wnd_toggle_light(gr);	}
	inline void ToggleZoom()	///< Switch on/off zooming by mouse
	{	mgl_wnd_toggle_zoom(gr);	}
	inline void ToggleRotate()	///< Switch on/off rotation by mouse
	{	mgl_wnd_toggle_rotate(gr);	}
	inline void ToggleNo()		///< Switch off all zooming and rotation
	{	mgl_wnd_toggle_no(gr);	}
	inline void Update()		///< Update picture by calling user drawing function
	{	mgl_wnd_update(gr);	}
	inline void ReLoad()		///< Reload user data and update picture
	{	mgl_wnd_reload(gr);	}
	inline void Adjust()		///< Adjust size of bitmap to window size
	{	mgl_wnd_adjust(gr);	}
	inline void NextFrame()		///< Show next frame (if one)
	{	mgl_wnd_next_frame(gr);	}
	inline void PrevFrame()		///< Show previous frame (if one)
	{	mgl_wnd_prev_frame(gr);	}
	inline void Animation()		///< Run slideshow (animation) of frames
	{	mgl_wnd_animation(gr);	}
	void SetClickFunc(void (*func)(void *p))	///< Callback function for mouse click
	{	if(!dr)	mgl_set_click_func(gr,func);	}

	inline void SetDelay(double dt)	///< Delay for animation in seconds
	{	mgl_wnd_set_delay(gr, dt);	}
	inline void Setup(bool clf_upd=true, bool showpos=false)
	{	mgl_setup_window(gr, clf_upd, showpos);	}
	inline mglPoint LastMousePos()		///< Last mouse position
	{	double x,y,z;	mgl_get_last_mouse_pos(gr,&x,&y,&z);	return mglPoint(x,y,z);	}
};
//-----------------------------------------------------------------------------
#endif
