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
#ifndef _MGL_FLTK_H_
#define _MGL_FLTK_H_
/*****************************************************************************/
#ifdef __cplusplus
#include "mgl/canvas_wnd.h"
//-----------------------------------------------------------------------------
#ifdef __MWERKS__
# define FL_DLL
#endif

#include <FL/Fl.H>
#include <Fl/Fl_Window.H>
#include <Fl/Fl_Scroll.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Menu_Bar.H>
//-----------------------------------------------------------------------------
/// Class is FLTK widget which display MathGL graphics
class Fl_MathGL : public Fl_Widget
{
public:
	Fl_Valuator	*tet_val;	///< pointer to external tet-angle validator
	Fl_Valuator	*phi_val;	///< pointer to external phi-angle validator

	Fl_MathGL(int x, int y, int w, int h, char *label=0);
	~Fl_MathGL();

	/// Update (redraw) plot
	virtual void update();
	/// Set angles for additional plot rotation
	inline void set_angle(float t, float p){	tet = t;	phi = p;	}
	/// Set bitwise flags for general state (1-Alpha, 2-Light)
	inline void set_flag(int f)	{	flag = f;	}
	/// Set flags for handling mouse
	void set_graph(mglCanvas *gr);	///< Set grapher object
	inline void set_graph(mglGraph *Gr)
	{	set_graph(dynamic_cast<mglCanvas *>(Gr->Self()));	}
	/// Get pointer to grapher
	inline HMGL get_graph()	{	return gr;	}
	/// Set drawing functions and its parameter
	inline void set_draw(int (*func)(mglBase *gr, void *par), void *par=0)
	{	draw_func = func;	draw_par = par;	}
	inline void set_draw(mglDraw *dr)
	{	set_draw(mgl_draw_class,(void*)dr);	}
	inline void set_draw(int (*dr)(mglGraph *gr))
	{	set_draw(mgl_draw_graph,(void*)dr);	}
	void set_state(bool z, bool r)	{	zoom = z;	rotate = r;	}
	/// Set zoom in/out region
	inline void set_zoom(mreal X1, mreal Y1, mreal X2, mreal Y2)
	{	x1 = X1;	x2 = X2;	y1 = Y1;	y2 = Y2;	update();	};
	/// Get zoom region
	inline void get_zoom(mreal *X1, mreal *Y1, mreal *X2, mreal *Y2)
	{	*X1 = x1;	*X2 = x2;	*Y1 = y1;	*Y2 = y2;	};
	/// Set popup menu pointer
	inline void set_popup(const Fl_Menu_Item *pmenu, Fl_Widget *wdg, void *v)
	{	popup = pmenu;	wpar = wdg;	vpar = v;	}
protected:
	mglCanvas *gr;		///< pointer to grapher
	void *draw_par;		///< Parameters for drawing function mglCanvasWnd::DrawFunc.
	/// Drawing function for window procedure. It should return the number of frames.
	int (*draw_func)(mglBase *gr, void *par);

	const Fl_Menu_Item *popup;	///< pointer to popup menu items
	Fl_Widget *wpar;			///< widget for popup menu
	void *vpar;					///< parameter for popup menu
	float tet,phi;				///< rotation angles
	bool rotate;				///< flag for handle mouse
	bool zoom;					///< flag for zoom by mouse
	bool wire;
	float x1,x2,y1,y2;			///< zoom region
	int flag;					///< bitwise flag for general state (1-Alpha, 2-Light)
	int x0,y0,xe,ye;			///< mouse position
	char pos[128];

	virtual void draw();		///< quick drawing function
	int handle(int code);		///< handle mouse events
	void resize(int x, int y, int w, int h);	///< resize control
};
//-----------------------------------------------------------------------------
class Fl_MGLView : public Fl_Window
{
public:
	Fl_MathGL *FMGL;		///< Control which draw graphics
	Fl_Scroll *scroll;
	Fl_Menu_Bar	*menu;

	void *par;				///< Parameter for handling animation
	void (*next)(void*);	///< Callback function for next frame
	void (*prev)(void*);	///< Callback function for prev frame
	float (*delay)(void*);	///< Callback function for delay
	void (*reload)(void*);	///< Callback function for reloading

	void toggle_alpha()	{	toggle(alpha, alpha_bt, "Graphics/Alpha");	}
	void toggle_light()	{	toggle(light, light_bt, "Graphics/Light");	}
	void toggle_sshow()	{	toggle(sshow, anim_bt, "Graphics/Slideshow");	}
	void toggle_wire()	{	toggle(wire, wire_bt, "Graphics/Wire");	}
	void toggle_zoom()	{	toggle(zoom, zoom_bt);	}
	void toggle_rotate(){	toggle(rotate, rotate_bt);	}
	void setoff_zoom()	{	setoff(zoom,zoom_bt);	}
	void setoff_rotate(){	setoff(rotate, rotate_bt);	}
	bool is_sshow()		{	return sshow;	}

	Fl_MGLView(int x, int y, int w, int h, char *label=0);
	~Fl_MGLView();
	void update();			///< Update picture by calling user drawing function
protected:
	Fl_Button *alpha_bt, *light_bt, *rotate_bt, *anim_bt, *zoom_bt, *wire_bt;
//	Fl_Counter *tet, *phi;

	int wire, alpha, light;	///< Current states of wire, alpha, light switches (toggle buttons)
	int sshow, rotate, zoom;///< Current states of slideshow, rotate, zoom switches (toggle buttons)

	void toggle(int &val, Fl_Button *b, const char *txt=NULL);
	void setoff(int &val, Fl_Button *b, const char *txt=NULL);
};
//-----------------------------------------------------------------------------
/// Class allows the window creation for displaying plot bitmap with the help of FLTK library
/** NOTE!!! All frames are saved in memory. So animation with many frames require a lot memory and CPU time (for example, for mouse rotation).*/
class mglCanvasFL : public mglCanvasWnd
{
public:
using mglCanvasWnd::Window;
	Fl_Window *Wnd;		///< Pointer to window
	Fl_MGLView *mgl;	///< Pointer to MGL widget with buttons

	mglCanvasFL();
	~mglCanvasFL();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ñëóæåáíûå ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/// Create a window for plotting. Now implemeted only for GLUT.
	void Window(int argc, char **argv, int (*draw)(mglBase *gr, void *p), const char *title,
						void *par=NULL, void (*reload)(void *p)=NULL, bool maximize=false);
	/// Switch on/off transparency (do not overwrite switches in user drawing function)
	void ToggleAlpha();
	/// Switch on/off lighting (do not overwrite switches in user drawing function)
	void ToggleLight();
	void ToggleRotate();	///< Switch on/off rotation by mouse
	void ToggleZoom();		///< Switch on/off zooming by mouse
	void ToggleNo();		///< Switch off all zooming and rotation
	void Update();			///< Update picture by calling user drawing function
	void Adjust();			///< Adjust size of bitmap to window size
	void GotoFrame(int d);	///< Show arbitrary frame (use relative step)
	void Animation();	///< Run animation (I'm too lasy to change it)
};
//-----------------------------------------------------------------------------
void mgl_ask_fltk(const wchar_t *quest, wchar_t *res);
//-----------------------------------------------------------------------------
#endif
#endif
