/***************************************************************************
 * mgl_wx.h.cpp is part of Math Graphic Library
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
#ifndef MGL_WX_H
#define MGL_WX_H
//-----------------------------------------------------------------------------
#include <wx/window.h>
#include <wx/image.h>
#include <wx/timer.h>
#include <wx/bitmap.h>
#include <mgl/mgl_zb.h>
//-----------------------------------------------------------------------------
/// Class is Wx widget which display MathGL graphics
class wxMathGL : public wxWindow
{
public:
	wxString appName;	///< Application name for message boxes
	bool AutoResize;	///< Allow auto resizing (default is false)
	int AnimDelay;		///< Animation delay in ms

	wxMathGL(wxWindow *parent, wxWindowID id=-1, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0, const wxString& name=wxPanelNameStr);
	~wxMathGL();
	double GetRatio()	{	return double(graph->GetWidth())/graph->GetHeight();	};
	void SetPopup(wxMenu *p)	{	popup = p;	};	///< Set popup menu pointer
	void SetSize(int w, int h);		///< Set window/picture sizes
	void SetGraph(mglGraphAB *gr);	///< Set grapher object
	/// Set drawing functions and its parameter
	void SetDraw(int (*func)(mglGraph *gr, void *par),	void *par=0);
	void SetDraw(mglDraw *dr);		///< Set drawing functions from mglDraw class

	int GetPer()	{return per;};		///< Get perspective value
	int GetPhi()	{return phi;};		///< Get Phi-angle value
	int GetTet()	{return tet;};		///< Get Theta-angle value
	bool GetAlpha()	{return alpha;};	///< Get transparency state
	bool GetLight()	{return light;};	///< Get lightning state
	bool GetZoom()	{return zoom;};		///< Get mouse zooming state
	bool GetRotate(){return rotate;};	///< Get mouse rotation state

	void Repaint();
	void Update(mglGraph *gr=0);	///< Update picture
	void Copy();			///< copy graphics to clipboard
	void Print();			///< Print plot
//	void Stop();			///< Stop execution
	void SetPer(int p);		///< Set perspective value
	void SetPhi(int p);		///< Set Phi-angle value
	void SetTet(int t);		///< Set Theta-angle value
	void SetAlpha(bool a);	///< Switch on/off transparency
	void SetLight(bool l);	///< Switch on/off lightning
	void SetZoom(bool z);	///< Switch on/off mouse zooming
	void SetRotate(bool r);	///< Switch on/off mouse rotation
	void ZoomIn();			///< Zoom in graphics
	void ZoomOut();			///< Zoom out graphics
	void Restore();			///< Restore zoom and rotation to default values
//	void Reload();			///< Reload data and execute script
	void ShiftLeft();		///< Shift graphics to left direction
	void ShiftRight();		///< Shift graphics to right direction
	void ShiftUp();			///< Shift graphics to up direction
	void ShiftDown();		///< Shift graphics to down direction
	void ExportPNG(wxString fname=L"");	///< export to PNG file
	void ExportPNGs(wxString fname=L"");	///< export to PNG file (no transparency)
	void ExportJPG(wxString fname=L"");	///< export to JPEG file
	void ExportBPS(wxString fname=L"");	///< export to bitmap EPS file
	void ExportEPS(wxString fname=L"");	///< export to vector EPS file
	void ExportSVG(wxString fname=L"");	///< export to SVG file
	void ExportIDTF(wxString fname=L"");	///< export to IDTF file
	void SetMGLFont(wxString path);		///< restore/load font for graphics
	//----These functions are executed only if graph is mglGraphQT instance----
	void Adjust();		///< Adjust plot size to fill entire window
	void NextSlide();	///< Show next slide
	void PrevSlide();	///< Show previous slide
	void Animation(bool st=true);	///< Start animation
	void About();		///< Show about information
	void AboutQt();		///< Show information about Qt version

protected:
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnNextSlide(wxTimerEvent& evt);	///< Show next slide
	void OnMouseLeftDown(wxMouseEvent &ev);
	void OnMouseDown(wxMouseEvent &ev);
	void OnMouseLeftUp(wxMouseEvent &ev);
	void OnMouseRightUp(wxMouseEvent &ev);
	void OnMouseMove(wxMouseEvent &ev);
//	void MousePressEvent(QMouseEvent *);
//	void MouseReleaseEvent(QMouseEvent *);
//	void MouseMoveEvent(QMouseEvent *);

	mglGraphAB *graph;	///< Built-in mglGraph-er instance (used by default)
	void *draw_par;		///< Parameters for drawing function mglGraph::DrawFunc.
	/// Drawing function for window procedure. It should return the number of frames.
	int (*draw_func)(mglGraph *gr, void *par);
	wxString MousePos;	///< Last mouse position
	wxBitmap pic;		///< Pixmap for drawing (changed by update)
	double tet, phi;	///< Rotation angles
	double per;			///< Value of perspective ( must be in [0,1) )
	bool alpha;			///< Transparency state
	bool light;			///< Lightning state
	bool zoom;			///< Mouse zoom state
	bool rotate;		///< Mouse rotation state
	mreal x1,x2,y1,y2;	///< Zoom in region
	bool showMessage;	///< Flag for showing messages (enabled by each execute())
	wxMenu *popup;		///< Pointer to pop-up menu
	wxTimer *timer;		///< Timer for animation
	DECLARE_EVENT_TABLE()
private:
	int x0, y0, xe, ye;		///< Temporary variables for mouse
	unsigned char *grBuf;
};
//-----------------------------------------------------------------------------
#ifdef ENABLE_MGLGRAPHWX
#include <wx/scrolwin.h>
class wxSpinCtrl;
class wxMenu;
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
	wxWindow *Wnd;		///< Pointer to window

	mglGraphWX();
	~mglGraphWX();

	void EndFrame();
	void SetSize(int w,int h);
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
	wxScrolledWindow *scroll;	///< Scrolling area
	wxMenu *popup;			///< Popup menu
	wxSpinCtrl *tet, *phi;	///< Spin box for angles
//	QAction *anim;

	void MakeMenu();		///< Create menu, toolbar and popup menu
};
#endif
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
