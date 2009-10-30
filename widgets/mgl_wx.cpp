/***************************************************************************
 * mgl_wx.cpp is part of Math Graphic Library                              *
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
#include <wx/dcclient.h>
#include <wx/msgdlg.h>
#include "mgl/mgl_wx.h"
//-----------------------------------------------------------------------------
/*
#include "xpm/fileprint.xpm"
#include "xpm/copy.xpm"
#include "xpm/left_1.xpm"
#include "xpm/right_1.xpm"
#include "xpm/down_1.xpm"
#include "xpm/norm_1.xpm"
#include "xpm/zoom_1.xpm"
#include "xpm/up_1.xpm"
#include "xpm/alpha.xpm"
#include "xpm/light.xpm"
#include "xpm/zoom_in.xpm"
#include "xpm/zoom_out.xpm"
#include "xpm/rotate.xpm"
#include "xpm/ok.xpm"
#include "xpm/show_sl.xpm"
#include "xpm/next_sl.xpm"
#include "xpm/prev_sl.xpm"
//-----------------------------------------------------------------------------
const wxString ScriptName(L"default");
enum
{
	TIMER_ID=1000,
	LAST_ID
};
BEGIN_EVENT_TABLE(wxMathGL, wxWindow)
//  EVT_MENU    (wxID_EXIT, MyFrame::OnExit)
//  EVT_MENU    (DO_TEST,   MyFrame::DoTest)
//  EVT_SIZE    (           MyFrame::OnSize)
//  EVT_BUTTON  (BUTTON1,   MyFrame::OnButton1)
	EVT_TIMER	(TIMER_ID,	wxMathGL::OnNextSlide)
	EVT_PAINT	(wxMathGL::OnPaint)
	EVT_SIZE	(wxMathGL::OnSize)
	EVT_LEFT_DOWN	(wxMathGL::OnMouseLeftDown)
	EVT_RIGHT_DOWN	(wxMathGL::OnMouseDown)
	EVT_MIDDLE_DOWN	(wxMathGL::OnMouseDown)
	EVT_LEFT_UP		(wxMathGL::OnMouseLeftUp)
	EVT_RIGHT_UP	(wxMathGL::OnMouseRightUp)
	EVT_MOTION		(wxMathGL::OnMouseMove)
END_EVENT_TABLE()
//-----------------------------------------------------------------------------
//
//		class wxMathGL
//
//-----------------------------------------------------------------------------
wxMathGL::wxMathGL(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) : wxWindow(parent,id,pos,size,style,name)
{
	AutoResize = false;	draw_par = 0;	draw_func = 0;
	graph = new mglGraphZB;
	popup = 0;		//!grBuf = 0;
	phi = tet = per = 0;
	x1 = y1 = 0;	x2 = y2 = 1;
	alpha = light = zoom = rotate = false;
	SetSize(600, 400);
	timer = new wxTimer(this,TIMER_ID);	AnimDelay = 1000;
}
//-----------------------------------------------------------------------------
wxMathGL::~wxMathGL()	{	delete graph;	}//!if(grBuf)	delete []grBuf;	}
//-----------------------------------------------------------------------------
void wxMathGL::SetGraph(mglGraphAB *gr)
{
	if(!gr)	return;
	delete graph;	graph=gr;
}
//-----------------------------------------------------------------------------
void wxMathGL::SetDraw(int (*func)(mglGraph *gr, void *par), void *par)
{	draw_func = func;	draw_par = par;	}
//-----------------------------------------------------------------------------
int mgl_draw_class(mglGraph *gr, void *p);
void wxMathGL::SetDraw(mglDraw *dr)
{	draw_func = mgl_draw_class;	draw_par = dr;	}
//-----------------------------------------------------------------------------
void wxMathGL::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	dc.DrawBitmap(wxBitmap(pic),0,0);
	if(zoom)	dc.DrawRectangle(x0,y0,xe-x0,ye-y0);
	if(graph->ShowMousePos && !MousePos.IsEmpty())
		dc.DrawText(MousePos,0,12);
}
//-----------------------------------------------------------------------------
void wxMathGL::OnSize(wxSizeEvent& event)
{
	wxSize ev = event.GetSize();
	if(graph->GetWidth()==ev.GetWidth() && graph->GetHeight()==ev.GetHeight())
		return;
	if(AutoResize)
	{	graph->SetSize(ev.GetWidth(), ev.GetHeight());	Update();	}
	else	SetSize(graph->GetWidth(), graph->GetHeight());
}
//-----------------------------------------------------------------------------
void wxMathGL::OnNextSlide(wxTimerEvent& evt)	{	NextSlide();	}
//-----------------------------------------------------------------------------
void wxMathGL::SetPer(int p)
{
	if(per!=p && p>=0 && p<100)
	{	per = 100*p;	PerChanged(p);	Update();	}
}
//-----------------------------------------------------------------------------
void wxMathGL::SetPhi(int p)
{	if(phi!=p)	{	phi = p;	PhiChanged(p);	Update();	}	}
//-----------------------------------------------------------------------------
void wxMathGL::SetTet(int t)
{	if(tet!=t)	{	tet = t;	TetChanged(t);	Update();	}	}
//-----------------------------------------------------------------------------
void wxMathGL::SetAlpha(bool a)
{	if(alpha!=a)	{	alpha = a;	AlphaChanged(a);	Update();	}	}
//-----------------------------------------------------------------------------
void wxMathGL::SetLight(bool l)
{	if(light!=l)	{	light = l;	LightChanged(l);	Update();	}	}
//-----------------------------------------------------------------------------
void wxMathGL::SetZoom(bool z)
{
	if(zoom!=z)
	{
		zoom=z;	rotate=false;	Update();
		ZoomChanged(z);	RotateChanged(false);
	}
}
//-----------------------------------------------------------------------------
void wxMathGL::SetRotate(bool r)
{
	if(rotate!=r)
	{
		zoom=false;	rotate=r;	Update();
		ZoomChanged(false);	RotateChanged(r);
	}
}
//-----------------------------------------------------------------------------
void wxMathGL::ShiftDown()
{	mreal d=(y2-y1)/3;	y1+=d;	y2+=d;	Update();	}
//-----------------------------------------------------------------------------
void wxMathGL::ShiftUp()
{	mreal d=(y2-y1)/3;	y1-=d;	y2-=d;	Update();	}
//-----------------------------------------------------------------------------
void wxMathGL::ShiftRight()
{	mreal d=(x2-x1)/3;	x1-=d;	x2-=d;	Update();	}
//-----------------------------------------------------------------------------
void wxMathGL::ShiftLeft()
{	mreal d=(x2-x1)/3;	x1+=d;	x2+=d;	Update();	}
//-----------------------------------------------------------------------------
void wxMathGL::Restore()
{
	SetPhi(0);	SetTet(0);	SetPer(0);
	x1=y1=0;	x2=y2=1;	zoom=rotate=false;
	ZoomChanged(false);	RotateChanged(false);
	Update();
}
//-----------------------------------------------------------------------------
void wxMathGL::ZoomIn()
{
	mreal d;
	d = (y2-y1)/4;	y1 += d;	y2 -= d;
	d = (x2-x1)/4;	x1 += d;	x2 -= d;
	Update();
}
//-----------------------------------------------------------------------------
void wxMathGL::ZoomOut()
{
	mreal d;
	d = (y2-y1)/2;	y1 -= d;	y2 += d;
	d = (x2-x1)/2;	x1 -= d;	x2 += d;
	Update();
}
//-----------------------------------------------------------------------------
void wxMathGL::Update(mglGraph *gr)
{
	if(gr==0)	gr = graph;
	if(gr==0 || draw_func==0)	return;
	gr->DefaultPlotParam();
	gr->Alpha(alpha);	gr->Light(light);
	gr->View(tet,phi);	gr->Org = mglPoint(NAN,NAN,NAN);
	gr->Perspective(per);
	gr->Zoom(x1,y1,x2,y2);
	gr->DrawFace = !rotate;

	char *buf=new char[2048];	buf[0]=0;	gr->Message = buf;
	draw_func(gr, draw_par);
	if(buf[0] != 0)
	{
		wxMessageDialog dlg(this, wxString(buf,wxConvLocal), appName, wxOK);
		dlg.ShowModal();
	}
	gr->Message = 0;		delete []buf;
	MousePos.Empty();
	if(gr==graph)	Refresh();
}
//-----------------------------------------------------------------------------
void wxMathGL::OnMouseLeftDown(wxMouseEvent &ev)
{
	long x=ev.GetX(), y=ev.GetY();
	if(!zoom && !rotate)
	{
		mglPoint p = graph->CalcXYZ(x, y);
		MousePos.Printf(wxT("x=%g, y=%g, z=%g"),p.x,p.y,p.z);
		Refresh();
//		emit mouseClick(p.x,p.y,p.z);
	}
	xe=x0=x;	ye=y0=y;	ev.Skip();
}
//-----------------------------------------------------------------------------
void wxMathGL::OnMouseDown(wxMouseEvent &ev)
{
	xe=x0=ev.GetX();	ye=y0=ev.GetY();	ev.Skip();
}
//-----------------------------------------------------------------------------
void wxMathGL::OnMouseLeftUp(wxMouseEvent &ev)
{
	if(zoom)
	{
		int w1=GetSize().GetWidth(),h1=GetSize().GetHeight();
		mreal _x1,_x2,_y1,_y2;
		_x1 = x1+(x2-x1)*(x0-GetPosition().x)/mreal(w1);
		_y1 = y2-(y2-y1)*(ye-GetPosition().y)/mreal(h1);
		_x2 = x1+(x2-x1)*(xe-GetPosition().x)/mreal(w1);
		_y2 = y2-(y2-y1)*(y0-GetPosition().y)/mreal(h1);
		x1=_x1;		x2=_x2;		y1=_y1;		y2=_y2;
		if(x1>x2)	{	_x1=x1;	x1=x2;	x2=_x1;	}
		if(y1>y2)	{	_x1=y1;	y1=y2;	y2=_x1;	}
		x0 = xe;	y0 = ye;
		Update();
	}
}
//-----------------------------------------------------------------------------
void wxMathGL::OnMouseRightUp(wxMouseEvent &ev)
{
	if(popup && !rotate)	// popup menu
		PopupMenu(popup, ev.GetPosition());
}
//-----------------------------------------------------------------------------
void wxMathGL::OnMouseMove(wxMouseEvent &ev)
{
	long w=GetSize().GetWidth(), h=GetSize().GetHeight();
	xe=ev.GetX();	ye=ev.GetY();
	if(rotate)
	{
		if(ev.ButtonDown(wxMOUSE_BTN_LEFT))	// rotate
		{
			mreal ff = 240/sqrt(mreal(w*h));
			phi += int((x0-xe)*ff);
			tet += int((y0-ye)*ff);
			if(phi>180)	phi-=360;		if(phi<-180)	phi+=360;
			if(tet>180)	tet-=360;		if(tet<-180)	tet+=360;
			TetChanged(int(tet));		PhiChanged(int(phi));
//			Update();
		}
		if(ev.ButtonDown(wxMOUSE_BTN_RIGHT))	// zoom and perspective
		{
			mreal ff = 2.*(y0-ye)/w, gg = 0.5*(xe-x0)/h;
			mreal cx = (x1+x2)/2, cy = (y1+y2)/2;
			x1 = cx+(x1-cx)*exp(-ff);	x2 = cx+(x2-cx)*exp(-ff);
			y1 = cy+(y1-cy)*exp(-ff);	y2 = cy+(y2-cy)*exp(-ff);
			per = per + gg;
			if(per<0)	per = 0;	if(per>=1)	per = 0.9999;
			if(gg)	PerChanged(int(per));
//			Update();
		}
		if(ev.ButtonDown(wxMOUSE_BTN_MIDDLE))	// shift
		{
			mreal ff = 1./sqrt(mreal(w*h));
			mreal dx = (x0-xe)*ff*(x2-x1), dy = (y0-ye)*ff*(y2-y1);
			x1 += dx;	x2 += dx;	y1 -= dy;	y2 -= dy;
		}
		x0 = xe;	y0 = ye;
		Update();
	}
	if(zoom)	Update();
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
*/
