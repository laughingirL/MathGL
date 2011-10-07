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
	mglWindow(int kind=0, int (*draw)(HMGL gr, void *p)=NULL, const char *title="MathGL", void *par=NULL) : mglGraph(-1)
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
	inline void Run()			///< Run main loop for event handling
	{	if(wnd==1)	mgl_qt_run();	else	mgl_fltk_run();	}

	inline 	void ToggleAlpha()	///< Switch on/off transparency (do not overwrite user settings)
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
	inline void ReLoad()	///< Reload user data and update picture
	{	mgl_wnd_reload(gr);	}
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
/// Base class for windows containing MathGL graphics
class mglCanvasW : public mglCanvas
{
public:
	mglCanvasW();
	virtual ~mglCanvasW();

	void SetSize(int w,int h);
	void EndFrame();
	const unsigned char *GetBits();
	void Clf(mglColor Back=NC);
	inline int GetNumFig()	{	return NumFig;	}
	inline int GetCurFig()	{	return CurFig;	}
	inline void SetCurFig(int c)	{	CurFig=c;	}			// stupid thing to pass G++ bug
	inline float GetDelay()	{	return Delay;	}
	inline void SetDelay(float d)	{	Delay=d;	}
	inline mglPoint GetMousePos()	{	return LastMousePos;}	// stupid thing to pass G++ bug
	inline void SetMousePos(mglPoint p)	{	LastMousePos=p;	}
	inline void Setup(bool autoclf, bool showpos, bool clf_upd)
	{	set(autoclf,MGL_AUTO_CLF);	set(showpos,MGL_SHOW_POS);	set(clf_upd,MGL_CLF_ON_UPD);	}

	virtual void ToggleAlpha()=0;	///< Switch on/off transparency (do not overwrite user settings)
	virtual void ToggleLight()=0;	///< Switch on/off lighting (do not overwrite user settings)
	virtual void ToggleZoom()=0;	///< Switch on/off zooming by mouse
	virtual void ToggleRotate()=0;	///< Switch on/off rotation by mouse
	virtual void ToggleNo()=0;		///< Switch off all zooming and rotation
	virtual void Update()=0;		///< Update picture by calling user drawing function
	virtual void Adjust()=0;		///< Adjust size of bitmap to window size
	virtual void NextFrame()=0;		///< Show next frame (if one)
	virtual void PrevFrame()=0;		///< Show previous frame (if one)
	virtual void Animation()=0;		///< Run slideshow (animation) of frames
	void ReLoad();			///< Reload user data and update picture
	/// Create a window for plotting based on callback function (can be NULL).
	virtual void Window(int argc, char **argv, int (*draw)(mglBase *gr, void *p),
						const char *title, void *par=NULL,
						void (*reload)(void *p)=NULL, bool maximize=false)=0;
	void Window(int argc, char **argv, int (*draw)(mglGraph *gr), const char *title, bool maximize=false)
	{	Window(argc,argv,mgl_draw_graph,title,(void*)draw,0,maximize);	}
	/// Create a window for plotting based on class mglDraw.
	void Window(int argc, char **argv, const char *title, mglDraw *draw, bool maximize=false)
	{	Window(argc, argv, mgl_draw_class, title, draw, mgl_reload_class, maximize);	}
	void SetDrawFunc(int (*draw)(mglBase *gr, void *p), void *par=NULL, void (*reload)(void *p)=NULL);

private:
	float Delay;		///< Delay for animation in seconds
	mglPoint LastMousePos;	///< Last mouse position
	int CurFig;		///< Current figure in the list.

	unsigned char *GG;	///< images for all frames (may be too LARGE !!!)
	int NumFig;			///< Number of figures in the list. If 0 then no list and mglCanvas::DrawFunc will called for each drawing.
	void (*LoadFunc)(void *par);
	void *FuncPar;		///< Parameters for drawing function mglCanvas::DrawFunc.
	/// Drawing function for window procedure. It should return the number of frames.
	int (*DrawFunc)(mglBase *gr, void *par);
};
//-----------------------------------------------------------------------------
#endif
