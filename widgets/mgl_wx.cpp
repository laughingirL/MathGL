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
#include <wx/clipbrd.h>
#include <wx/dataobj.h>
#include "mgl/mgl_idtf.h"
#include "mgl/mgl_eps.h"
#include "mgl/mgl_wx.h"
#include "mgl/mgl_c.h"
#include "mgl/mgl_f.h"
//-----------------------------------------------------------------------------
const wxString ScriptName(L"default");
enum
{
	TIMER_ID=1000,
	LAST_ID
};
BEGIN_EVENT_TABLE(wxMathGL, wxWindow)
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
	dc.DrawBitmap(pic,0,0);
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
	{	per = 100*p;	Update();	}
}
//-----------------------------------------------------------------------------
void wxMathGL::SetPhi(int p)
{	if(phi!=p)	{	phi = p;	Update();	}	}
//-----------------------------------------------------------------------------
void wxMathGL::SetTet(int t)
{	if(tet!=t)	{	tet = t;	Update();	}	}
//-----------------------------------------------------------------------------
void wxMathGL::SetAlpha(bool a)
{	if(alpha!=a)	{	alpha = a;	Update();	}	}
//-----------------------------------------------------------------------------
void wxMathGL::SetLight(bool l)
{	if(light!=l)	{	light = l;	Update();	}	}
//-----------------------------------------------------------------------------
void wxMathGL::SetZoom(bool z)
{	if(zoom!=z)	{	zoom=z;	rotate=false;	Update();	}	}
//-----------------------------------------------------------------------------
void wxMathGL::SetRotate(bool r)
{	if(rotate!=r)	{	zoom=false;	rotate=r;	Update();	}	}
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
	if(gr==graph)	Repaint();
}
//-----------------------------------------------------------------------------
void convertFromGraph(wxBitmap &pic, mglGraphAB *gr, unsigned char **buf)
{
	const unsigned char *bb = gr->GetBits();
	register long i,w=gr->GetWidth(), h=gr->GetHeight();
	if(*buf)	delete [](*buf);
	*buf = new unsigned char[4*w*h];
	for(i=0;i<w*h;i++)
	{
		(*buf)[4*i]   = bb[3*i+2];
		(*buf)[4*i+1] = bb[3*i+1];
		(*buf)[4*i+2] = bb[3*i];
		(*buf)[4*i+3] = 255;
	}
	wxImage img(w, h, *buf);
	wxBitmap bmp(img,32);	pic = bmp;
}
//-----------------------------------------------------------------------------
void wxMathGL::Repaint()
{
	convertFromGraph(pic, graph, &grBuf);
	wxSize sz=GetSize();
	if(pic.GetWidth()!=sz.GetWidth() || pic.GetHeight()!=sz.GetHeight())
		SetSize(pic.GetWidth(), pic.GetHeight());
	Refresh();
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
{	xe=x0=ev.GetX();	ye=y0=ev.GetY();	ev.Skip();	}
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
{	if(popup && !rotate)	PopupMenu(popup, ev.GetPosition());	}
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
//	if(zoom)	Update();
	if(zoom)	Refresh(0);
}
//-----------------------------------------------------------------------------
wxString mglSetExtension(wxString &fname, const char *ext)
{
	wxString oname;
	if(fname.Right(4)!=wxChar('.')+wxString(ext,*wxConvCurrent))
		oname = fname+wxChar('.')+wxString(ext,*wxConvCurrent);
	return oname;
}
//-----------------------------------------------------------------------------
void wxMathGL::ExportPNG(wxString fname)
{
	if(fname.IsEmpty())	fname = ScriptName;
	if(fname.IsEmpty())	wxMessageBox(appName, wxT("No filename."),wxOK|wxICON_ERROR ,this);
	else	graph->WritePNG(mglSetExtension(fname,"png").char_str(), appName.char_str());
}
//-----------------------------------------------------------------------------
void wxMathGL::ExportPNGs(wxString fname)
{
	if(fname.IsEmpty())	fname = ScriptName;
	if(fname.IsEmpty())	wxMessageBox(appName, wxT("No filename."),wxOK|wxICON_ERROR ,this);
	else	graph->WritePNG(mglSetExtension(fname,"png").char_str(), appName.char_str(), false);
}
//-----------------------------------------------------------------------------
void wxMathGL::ExportJPG(wxString fname)
{
	if(fname.IsEmpty())	fname = ScriptName;
	if(fname.IsEmpty())	wxMessageBox(appName, wxT("No filename."),wxOK|wxICON_ERROR ,this);
	else	graph->WriteJPEG(mglSetExtension(fname,"jpg").char_str(), appName.char_str());
}
//-----------------------------------------------------------------------------
void wxMathGL::ExportBPS(wxString fname)
{
	if(fname.IsEmpty())	fname = ScriptName;
	if(fname.IsEmpty())	wxMessageBox(appName, wxT("No filename."),wxOK|wxICON_ERROR ,this);
	else
	{
		setlocale(LC_ALL, "C");
		graph->WriteEPS(mglSetExtension(fname,"eps").char_str(), appName.char_str());
		setlocale(LC_ALL, "");
	}
}
//-----------------------------------------------------------------------------
void wxMathGL::ExportEPS(wxString fname)
{
	if(fname.IsEmpty())	fname = ScriptName;
	if(fname.IsEmpty())	wxMessageBox(appName, wxT("No filename."),wxOK|wxICON_ERROR ,this);
	else
	{
		mglGraphPS *ps = new mglGraphPS(graph->GetWidth(), graph->GetHeight());
		ps->SetFont(graph->GetFont());
		Update(ps);
		setlocale(LC_ALL, "C");
		ps->WriteEPS(mglSetExtension(fname,"eps").char_str(), appName.char_str());
		setlocale(LC_ALL, "");
		delete ps;
	}
}
//-----------------------------------------------------------------------------
void wxMathGL::ExportSVG(wxString fname)
{
	if(fname.IsEmpty())	fname = ScriptName;
	if(fname.IsEmpty())	wxMessageBox(appName, wxT("No filename."),wxOK|wxICON_ERROR ,this);
	else
	{
		mglGraphPS *ps = new mglGraphPS(graph->GetWidth(), graph->GetHeight());
		ps->SetFont(graph->GetFont());
		Update(ps);
		setlocale(LC_ALL, "C");
		ps->WriteSVG(mglSetExtension(fname,"svg").char_str(), appName.char_str());
		setlocale(LC_ALL, "");
		delete ps;
	}
}
//-----------------------------------------------------------------------------
void wxMathGL::ExportIDTF(wxString fname)
{
	if(fname.IsEmpty())	fname = ScriptName;
	if(fname.IsEmpty())	wxMessageBox(appName, wxT("No filename."),wxOK|wxICON_ERROR ,this);
	else
	{
		mglGraphIDTF *ps = new mglGraphIDTF;
		ps->SetFont(graph->GetFont());
		Update(ps);
		setlocale(LC_ALL, "C");
		ps->WriteIDTF(mglSetExtension(fname,"idtf").char_str(), appName.char_str());
		setlocale(LC_ALL, "");
		delete ps;
	}
}
//-----------------------------------------------------------------------------
void wxMathGL::Copy()
{
	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData( new wxBitmapDataObject(pic) );
		wxTheClipboard->Close();
	}
}
//-----------------------------------------------------------------------------
void wxMathGL::SetMGLFont(wxString path)
{
	if(path.IsEmpty())	graph->GetFont()->Restore();
	else	graph->GetFont()->Load(path.char_str());
}
//-----------------------------------------------------------------------------
void wxMathGL::SetSize(int w, int h)
{	graph->SetSize(w,h);	wxWindow::SetSize(w, h);	Update();	};
//-----------------------------------------------------------------------------
void wxMathGL::Adjust()		{	graph->Adjust();	}
//-----------------------------------------------------------------------------
void wxMathGL::NextSlide()	{	graph->NextFrame();	}
//-----------------------------------------------------------------------------
void wxMathGL::PrevSlide()	{	graph->PrevFrame();	}
//-----------------------------------------------------------------------------
void wxMathGL::Animation(bool st)
{
/*	mglGraphWX *gr = dynamic_cast<mglGraphWX *>(graph);
	if(st)	timer->Start(gr ? int(gr->Delay*1000) : AnimDelay);
	else	timer->Stop();*/
}
//-----------------------------------------------------------------------------
void wxMathGL::About()
{
	wxString s = wxT("MathGL v. 1.") + wxString::Format(wxT("%g"),MGL_VERSION) +
		wxT("\n(c) Alexey Balakin, 2007\nhttp://mathgl.sourceforge.net/");
	wxMessageBox(s, wxT("MathGL - about"), wxOK|wxICON_INFORMATION, this);
}
//-----------------------------------------------------------------------------
//
//		class mglGraphWX
//
//-----------------------------------------------------------------------------
#ifdef ENABLE_MGLGRAPHWX
#include <wx/app.h>
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
mglGraphWX::mglGraphWX() : mglGraphZB()
{
	GG = 0;	Wnd = 0;
	NumFig = 0;	CurFig = -1;
}
//-----------------------------------------------------------------------------
mglGraphWX::~mglGraphWX()
{	if(GG) free(GG);	}
//-----------------------------------------------------------------------------
void mglGraphWX::SetSize(int w,int h)
{
	if(GG)	free(GG);	GG = 0;
	mglGraphZB::SetSize(w,h);
}
//-----------------------------------------------------------------------------
void mglGraphWX::EndFrame()
{
	CurFig = CurFrameId-1;
	if(!GG)
	{
		GG = (unsigned char *)malloc(3*Width*Height);
		NumFig = 1;		CurFig = 0;
	}
	else if(CurFig>NumFig-1)
	{
		GG = (unsigned char *)realloc(GG,3*(NumFig+1)*Width*Height);
		NumFig++;
	}
	mglGraph::EndFrame();
	memcpy(GG + CurFig*Width*Height*3,G,3*Width*Height);
	CurFig++;
}
//-----------------------------------------------------------------------------
void mglGraphWX::Clf(mglColor Back)
{
	if(AutoClf)	mglGraphZB::Clf(Back);
}
//-----------------------------------------------------------------------------
const unsigned char *mglGraphWX::GetBits()
{
	Finish();
	unsigned char *g = G;
	if(GG && NumFig>0 && CurFig<NumFig && CurFig>=0)
		g = GG + CurFig*Width*Height*3;
	return g;
}
//-----------------------------------------------------------------------------
void mglGraphWX::NextFrame()
{
	if(NumFig>0)	{	CurFig = CurFig>NumFig-1 ? 0 : CurFig+1;	WMGL->Repaint();	}
}
//-----------------------------------------------------------------------------
void mglGraphWX::PrevFrame()
{
	if(NumFig>0)	{	CurFig = CurFig<0 ? NumFig-1 : CurFig-1;	WMGL->Repaint();	}
}
//-----------------------------------------------------------------------------
void mglGraphWX::ToggleAlpha()	{	WMGL->SetAlpha(!WMGL->GetAlpha());	}
//-----------------------------------------------------------------------------
void mglGraphWX::ToggleLight()	{	WMGL->SetLight(!WMGL->GetLight());	}
//-----------------------------------------------------------------------------
void mglGraphWX::ToggleNo()		{	WMGL->Restore();	}
//-----------------------------------------------------------------------------
void mglGraphWX::ToggleZoom()	{	WMGL->SetZoom(!WMGL->GetZoom());	}
//-----------------------------------------------------------------------------
void mglGraphWX::ToggleRotate()	{	WMGL->SetRotate(!WMGL->GetRotate());}
//-----------------------------------------------------------------------------
void mglGraphWX::Update()		{	CurFig=0;	WMGL->Restore();	}
//-----------------------------------------------------------------------------
void mglGraphWX::ReLoad(bool o)	{	if(LoadFunc){LoadFunc(o, FuncPar);Update();}	}
//-----------------------------------------------------------------------------
void mglGraphWX::Adjust()
{
	wxSize sz = scroll->GetSize();
	SetSize(sz.GetWidth(), sz.GetHeight());
	WMGL->SetSize(sz.GetWidth(), sz.GetHeight());
	Update();
}
//-----------------------------------------------------------------------------
void mglGraphWX::Animation()
{
/*	if(anim)
	{
		bool s = anim->isChecked();
		anim->setChecked(!s);
		WMGL->Animation(!s);
	}*/
}
//-----------------------------------------------------------------------------
void mglGraphWX::Window(int argc, char **argv, int (*draw)(mglGraph *gr, void *p), const char *title, void *par, void (*reload)(int next, void *p), bool maximize)
{
	NumFig=0;	CurFig=0;
	CurFrameId = 0;
	int n = draw ? draw(this,par) : 0;
	if(n<NumFig && n>=0)	NumFig = n;
	DrawFunc = draw;		FuncPar = par;
	LoadFunc = reload;
	if(Wnd)
	{
		Wnd->SetLabel(wxString(title,wxConvLibc));
		if(maximize)
		{	Wnd->SetSize(Wnd->GetMaxSize());	}
		else	Wnd->Show();
		return;
	}

	if(!wxTheApp)	{	new wxApp;	wxTheApp->SetExitOnFrameDelete(true);	}

	Wnd = new wxWindow;	Wnd->SetSize(650,480);
	Wnd->SetLabel(wxString(title,wxConvLibc));
	scroll = new wxScrolledWindow(Wnd);
	WMGL = new wxMathGL(scroll);
	WMGL->SetPopup(popup);	WMGL->SetGraph(this);
	WMGL->SetDraw(draw, par);
	MakeMenu();
	WMGL->Update();
	if(maximize)	Wnd->SetSize(Wnd->GetMaxSize());
	Wnd->Show();
}
//-----------------------------------------------------------------------------
/*void mglGraphQT::makeMenu()
{
	QAction *a;
	QMenu *o, *oo;
	QToolBar *bb;

	popup = new QMenu(Wnd);
	// file menu
	{
		o = Wnd->menuBar()->addMenu(TR("&File"));
		oo = new QMenu(TR("&Export as ..."),Wnd);
		oo->addAction(TR("PNG"), QMGL, SLOT(exportPNG()),Qt::ALT+Qt::Key_P);
		oo->addAction(TR("solid PNG"), QMGL, SLOT(exportPNGs()),Qt::ALT+Qt::Key_F);
		oo->addAction(TR("JPEG"), QMGL, SLOT(exportJPG()),Qt::ALT+Qt::Key_J);
		oo->addAction(TR("bitmap EPS"), QMGL, SLOT(exportBPS()));
		oo->addAction(TR("vector EPS"), QMGL, SLOT(exportEPS()),Qt::ALT+Qt::Key_E);
		oo->addAction(TR("SVG"), QMGL, SLOT(exportSVG()),Qt::ALT+Qt::Key_S);
		oo->addAction(TR("IDTF"), QMGL, SLOT(exportIDTF()));
		o->addMenu(oo);
		popup->addMenu(oo);

		o->addSeparator();
		a = new QAction(QPixmap(fileprint), TR("Print &graphics"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(print()));
		a->setToolTip(TR("Open printer dialog and print graphics\t(CTRl+P)"));
		a->setShortcut(Qt::CTRL+Qt::Key_P);	o->addAction(a);
		o->addSeparator();
		o->addAction(TR("&Close"), Wnd, SLOT(close()), Qt::CTRL+Qt::Key_W);
	}
	// graphics menu
	{
		bb = new QToolBar(TR("Graphics"),Wnd);
		Wnd->addToolBar(Qt::TopToolBarArea, bb);
		o = Wnd->menuBar()->addMenu(TR("&Graphics"));
		a = new QAction(QPixmap(alpha_xpm), TR("&Alpha"), Wnd);
		a->setShortcut(Qt::ALT+Qt::Key_T);	a->setCheckable(true);
		Wnd->connect(a, SIGNAL(toggled(bool)), QMGL, SLOT(setAlpha(bool)));
		Wnd->connect(QMGL, SIGNAL(alphaChanged(bool)), a, SLOT(setOn(bool)));
		a->setToolTip(TR("Switch on/off TRansparency for the graphics (Alt+T)."));
		o->addAction(a);		bb->addAction(a);
		a = new QAction(QPixmap(light_xpm), TR("&Light"), Wnd);
		a->setShortcut(Qt::ALT+Qt::Key_L);	a->setCheckable(true);
		Wnd->connect(a, SIGNAL(toggled(bool)), QMGL, SLOT(setLight(bool)));
		Wnd->connect(QMGL, SIGNAL(lightChanged(bool)), a, SLOT(setOn(bool)));
		a->setToolTip(TR("Switch on/off lightning for the graphics (Alt+L)."));
		o->addAction(a);		bb->addAction(a);
		a = new QAction(QPixmap(rotate_xpm), TR("&Rotate by mouse"), Wnd);
		a->setCheckable(true);
		Wnd->connect(a, SIGNAL(toggled(bool)), QMGL, SLOT(setRotate(bool)));
		Wnd->connect(QMGL, SIGNAL(rotateChanged(bool)), a, SLOT(setOn(bool)));
		a->setToolTip(TR("Switch on/off mouse handling of the graphics\n(rotation, shifting, zooming and perspective)."));
		bb->addAction(a);
		a = new QAction(QPixmap(zoom_in_xpm), TR("&Zoom by mouse"), Wnd);
		a->setCheckable(true);
		Wnd->connect(a, SIGNAL(toggled(bool)), QMGL, SLOT(setZoom(bool)));
		Wnd->connect(QMGL, SIGNAL(zoomChanged(bool)), a, SLOT(setOn(bool)));
		a->setToolTip(TR("Switch on/off mouse zoom of selected region."));
		bb->addAction(a);
		o->addSeparator();
		a = new QAction(QPixmap(zoom_out_xpm), TR("Res&tore"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(restore()));
		a->setToolTip(TR("Restore default graphics rotation, zoom and perspective (Alt+Space)."));
		a->setShortcut(Qt::ALT+Qt::Key_Space);
		o->addAction(a);	bb->addAction(a);	popup->addAction(a);
		bb->addSeparator();
		a = new QAction(QPixmap(ok_xpm), TR("Re&draw"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(update()));
		a->setToolTip(TR("Execute script and redraw graphics (F5)."));
		a->setShortcut(Qt::Key_F5);
		o->addAction(a);	bb->addAction(a);	popup->addAction(a);
		a = new QAction(TR("&Adjust size"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(adjust()));
		a->setToolTip(TR("Change canvas size to fill whole region (F6)."));
		a->setShortcut(Qt::Key_F6);		o->addAction(a);
		a = new QAction(QPixmap(copy_xpm), TR("&Copy plot"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(copy()));
		a->setToolTip(TR("Copy graphics to clipboard (CTRl+C)."));
		a->setShortcut(Qt::CTRL+Qt::Key_C);
		o->addAction(a);		bb->addAction(a);	popup->addAction(a);
		bb->addSeparator();
		tet = new QSpinBox(Wnd);	tet->setWrapping(true);
		bb->addWidget(tet);	tet->setRange(-180, 180);	tet->setSingleStep(10);
		Wnd->connect(tet, SIGNAL(valueChanged(int)), QMGL, SLOT(setTet(int)));
		Wnd->connect(QMGL, SIGNAL(tetChanged(int)), tet, SLOT(setValue(int)));
		tet->setToolTip(TR("Set value of \\theta angle."));
		bb->addSeparator();
		phi = new QSpinBox(Wnd);	phi->setWrapping(true);
		bb->addWidget(phi);	phi->setRange(-180, 180);	phi->setSingleStep(10);
		Wnd->connect(phi, SIGNAL(valueChanged(int)), QMGL, SLOT(setPhi(int)));
		Wnd->connect(QMGL, SIGNAL(phiChanged(int)), phi, SLOT(setValue(int)));
		phi->setToolTip(TR("Set value of \\phi angle."));
//	bb->addSeparator();
	}
	// zooming menu
	{
		oo = o->addMenu(TR("Zoom/move"));
		bb = new QToolBar(TR("Zoom graphics"),Wnd);
		Wnd->addToolBar(Qt::LeftToolBarArea, bb);
		a = new QAction(QPixmap(left_1_xpm), TR("Move &left"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(shiftLeft()));
		a->setToolTip(TR("Move graphics left by 1/3 of its width."));
		bb->addAction(a);		oo->addAction(a);
		a = new QAction(QPixmap(up_1_xpm), TR("Move &up"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(shiftUp()));
		a->setToolTip(TR("Move graphics up by 1/3 of its height."));
		bb->addAction(a);		oo->addAction(a);
		a = new QAction(QPixmap(zoom_1_xpm), TR("Zoom &in"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(zoomIn()));
		a->setToolTip(TR("Zoom in graphics."));
		bb->addAction(a);		oo->addAction(a);
		a = new QAction(QPixmap(norm_1_xpm), TR("Zoom &out"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(zoomOut()));
		a->setToolTip(TR("Zoom out graphics."));
		bb->addAction(a);		oo->addAction(a);
		a = new QAction(QPixmap(down_1_xpm), TR("Move &down"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(shiftDown()));
		a->setToolTip(TR("Move graphics up down 1/3 of its height."));
		bb->addAction(a);		oo->addAction(a);
		a = new QAction(QPixmap(right_1_xpm), TR("Move &right"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(shiftRight()));
		a->setToolTip(TR("Move graphics right by 1/3 of its width."));
		bb->addAction(a);		oo->addAction(a);
	}
	// animation menu
	{
		o = Wnd->menuBar()->addMenu(TR("&Animation"));
		bb = new QToolBar(TR("Animation"),Wnd);
		Wnd->addToolBar(Qt::LeftToolBarArea, bb);
		a = new QAction(QPixmap(next_sl_xpm), TR("&Next slide"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(nextSlide()));
		a->setToolTip(TR("Show next slide (Alt+Right)."));
		a->setShortcut(Qt::ALT+Qt::Key_Right);	o->addAction(a);		bb->addAction(a);
		a = new QAction(QPixmap(show_sl_xpm), TR("&Slideshow"), Wnd);
		a->setCheckable(true);	anim = a;
		Wnd->connect(a, SIGNAL(toggled(bool)), QMGL, SLOT(animation(bool)));
		a->setToolTip(TR("Run slideshow (CTRl+F5)."));
		a->setShortcut(Qt::CTRL+Qt::Key_F5);	o->addAction(a);		bb->addAction(a);
		a = new QAction(QPixmap(prev_sl_xpm), TR("&Prev slide"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(prevSlide()));
		a->setToolTip(TR("Show previous slide (Alt+Left)."));
		a->setShortcut(Qt::ALT+Qt::Key_Left);	o->addAction(a);		bb->addAction(a);
	}

	Wnd->menuBar()->addSeparator();
	o = Wnd->menuBar()->addMenu(TR("&Help"));
	o->addAction(TR("About"), QMGL, SLOT(about()));
	o->addAction(TR("About &Qt"), QMGL, SLOT(aboutQt()));
}*/
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
HMGL mgl_create_graph_wx_dr(HMDR dr, const char *title)
{
	mglGraphWX *g = new mglGraphWX;
	g->Window(0,0,dr,title);
	return g;
}
//-----------------------------------------------------------------------------
HMGL mgl_create_graph_wx(int (*draw)(HMGL gr, void *p), const char *title, void *par)
{
	mglGraphWX *g = new mglGraphWX;
	g->Window(0,0,draw,title,par);
	return g;
}
//-----------------------------------------------------------------------------
//uintptr_t mgl_create_graph_qt_(int (*draw)(uintptr_t *gr), const char *title, int l)
uintptr_t mgl_create_graph_wx_(const char *title, int l)
{
	mglGraphWX *g = new mglGraphWX;
	char *s = new char[l+1];	memcpy(s,title,l);	s[l]=0;
//	g->Window(0,0,mgl_fortran_func,s,(void*)draw);
	g->Window(0,0,0,s,0,0);
	delete []s;
	return uintptr_t(g);
}
//-----------------------------------------------------------------------------
int mglWxRun()		{	return wxTheApp ? wxTheApp->MainLoop():0;	}
//-----------------------------------------------------------------------------
void mgl_wx_run_()	{	mglWxRun();	}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
