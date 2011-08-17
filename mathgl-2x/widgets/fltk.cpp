/***************************************************************************
 * mgl_fltk.cpp is part of Math Graphic Library
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
#include <FL/Fl_Pixmap.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_File_Chooser.H>
#include <unistd.h>

#include "mgl/fltk.h"

#ifdef USE_GETTEXT
	#include <libintl.h>
#else
// Workaround for gcc 4.2
	#ifndef _LIBINTL_H
	#define gettext(x)	(x)
	#endif
#endif

//-----------------------------------------------------------------------------
#include "xpm/alpha_on.xpm"
#include "xpm/light_on.xpm"
//#include "xpm/zoom_on.xpm"
#include "xpm/show_on.xpm"
#include "xpm/rotate_on.xpm"
#include "xpm/show_sl.xpm"
#include "xpm/next_sl.xpm"
#include "xpm/prev_sl.xpm"
//#include "xpm/left_1.xpm"
//#include "xpm/right_1.xpm"
//#include "xpm/down_1.xpm"
//#include "xpm/norm_1.xpm"
//#include "xpm/zoom_1.xpm"
//#include "xpm/up_1.xpm"
#include "xpm/alpha.xpm"
#include "xpm/light.xpm"
//#include "xpm/zoom_in.xpm"
#include "xpm/zoom_out.xpm"
#include "xpm/rotate.xpm"
#include "xpm/ok.xpm"
//-----------------------------------------------------------------------------
Fl_Pixmap xpm_a1(alpha_xpm), xpm_a2(alpha_on_xpm);
Fl_Pixmap xpm_l1(light_on_xpm), xpm_l2(light_xpm);
//Fl_Pixmap xpm_z1(zoom_in_xpm), xpm_z2(zoom_on_xpm);
Fl_Pixmap xpm_s1(show_sl_xpm), xpm_s2(show_on_xpm);
Fl_Pixmap xpm_r1(rotate_xpm), xpm_r2(rotate_on_xpm);
//-----------------------------------------------------------------------------
//
//		class Fl_MathGL
//
//-----------------------------------------------------------------------------
Fl_MathGL::Fl_MathGL(int xx, int yy, int ww, int hh, char *lbl) : Fl_Widget(xx,yy,ww,hh,lbl)
{
	graph = new mglCanvas;
	rotate = false;		tet=phi=0;
	flag=x0=y0=xe=ye=0;
	tet_val = phi_val = 0;
	draw_par = 0;	draw_func = 0;
}
//-----------------------------------------------------------------------------
Fl_MathGL::~Fl_MathGL()	{}
//-----------------------------------------------------------------------------
void Fl_MathGL::set_graph(mglCanvas *gr)
{
	if(!gr)	return;
	delete graph;	graph=gr;
}
//-----------------------------------------------------------------------------
void Fl_MathGL::draw()
{
	const unsigned char *g = graph ? graph->GetBits() : 0;
	if(g)	fl_draw_image(g, x(), y(), graph->GetWidth(), graph->GetHeight(), 3);
}
//-----------------------------------------------------------------------------
void Fl_MathGL::update(mglCanvas *gr)
{
	if(gr==0)	gr=graph;
	if(gr==0)	return;
	if(draw_func)
	{
		if(gr!=graph || graph->get(MGL_CLF_ON_UPD))	gr->DefaultPlotParam();
		gr->ResetFrames();
		char *buf = new char[2048];	*buf = 0;
		gr->Message = buf;
		gr->Alpha(flag&1);	gr->Light(flag&2);
		gr->View(tet,phi);	gr->Clf();
//		gr->DrawFace = !rotate;		// TODO: switch to fast drawing here
		draw_func(gr, draw_par);
		if(*buf)	fl_message("%s",buf);
		gr->Message = 0;	delete []buf;
	}
	if(gr==graph && (graph->GetWidth()!=w() || graph->GetHeight()!=h()))
		size(graph->GetWidth(), graph->GetHeight());
	redraw();	Fl::flush();
}
//-----------------------------------------------------------------------------
void Fl_MathGL::resize(int xx, int yy, int ww, int hh)
{	Fl_Widget::resize(xx,yy,ww,hh);	}
//-----------------------------------------------------------------------------
int Fl_MathGL::handle(int code)
{
	if(popup && code==FL_PUSH && Fl::event_button()==FL_RIGHT_MOUSE)
	{
		const Fl_Menu_Item *m = popup->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);
		if(m)	m->do_callback(wpar, vpar);
	}
	if(graph->get(MGL_SHOW_POS) && !rotate && code==FL_PUSH && Fl::event_button()==FL_LEFT_MOUSE)
	{
		mglPoint p = graph->CalcXYZ(Fl::event_x()-x(), Fl::event_y()-y());
		char s[128];
		sprintf(s,"x=%g, y=%g, z=%g",p.x,p.y,p.z);
		draw();	fl_color(FL_BLACK);		fl_draw(s,40,70);
	}
	if(!rotate || Fl::event_button()!=FL_LEFT_MOUSE)
	{
		if(code==FL_FOCUS || code==FL_UNFOCUS)	return 1;
		if(code==FL_KEYUP)
		{
			int key=Fl::event_key();
			if(!strchr(" .,wasdrfx",key))	return 0;
			if(key==' ')
			{	update();	return 1;	}
			if(key=='w')
			{
				tet += 10;
				if(tet_val)	tet_val->value(tet);
				update();	return 1;
			}
			if(key=='s')
			{
				tet -= 10;
				if(tet_val)	tet_val->value(tet);
				update();	return 1;
			}
			if(key=='a')
			{
				phi += 10;
				if(phi_val)	phi_val->value(phi);
				update();	return 1;
			}
			if(key=='d')
			{
				phi -= 10;
				if(phi_val)	phi_val->value(phi);
				update();	return 1;
			}
			if(key=='x')
			{
				mglCanvasFL *g=dynamic_cast<mglCanvasFL *>(graph);
				if(g && g->FMGL==this)
				{	g->Wnd->hide();	return 1;	}
				else	return 0;
//				exit(0);
			}
			if(key==',')
			{
				mglCanvasFL *g=dynamic_cast<mglCanvasFL *>(graph);
				if(g && g->FMGL==this)
				{	g->PrevFrame();	return 1;	}
				else	return 0;
			}
			if(key=='.')
			{
				mglCanvasFL *g=dynamic_cast<mglCanvasFL *>(graph);
				if(g && g->FMGL==this)
				{	g->NextFrame();	return 1;	}
				else	return 0;
			}
			if(key=='r')
			{
				flag = (flag&2) + ((~(flag&1))&1);
				update();	return 1;
			}
			if(key=='f')
			{
				flag = (flag&1) + ((~(flag&2))&2);
				update();	return 1;
			}
		}
		return 0;
	}
	if(code==FL_PUSH)
	{	xe=x0=Fl::event_x();	ye=y0=Fl::event_y();	}
	if(code==FL_DRAG)
	{
		xe=Fl::event_x();	ye=Fl::event_y();
		mreal ff = 240/sqrt(mreal(w()*h()));
		if(rotate)
		{
			phi += int((x0-xe)*ff);
			tet += int((y0-ye)*ff);
			if(phi>180)	phi-=360;		if(phi<-180)	phi+=360;
			if(tet>180)	tet-=360;		if(tet<-180)	tet+=360;
			x0 = xe;	y0 = ye;
			update();
		}
		redraw();
	}
	if(code==FL_RELEASE)
	{
		if(tet_val)	tet_val->value(tet);
		if(phi_val)	phi_val->value(phi);
		redraw();
	}
	return 1;
}
//-----------------------------------------------------------------------------
//
//		class mglCanvasFL
//
//-----------------------------------------------------------------------------
mglCanvasFL::mglCanvasFL() : mglCanvasW()
{	Wnd = 0;	alpha = light = sshow = 0;	}
mglCanvasFL::~mglCanvasFL()	{	if(Wnd)	delete Wnd;	}
//-----------------------------------------------------------------------------
void mglCanvasFL::NextFrame()
{
	if(GetNumFig()>0)
	{	SetCurFig(GetCurFig()>GetNumFig()-1 ? 0 : GetCurFig()+1);	FMGL->redraw();	}
}
//-----------------------------------------------------------------------------
void mglCanvasFL::PrevFrame()
{
	if(GetNumFig()>0)
	{	SetCurFig(GetCurFig()<0 ? GetNumFig()-1 : GetCurFig()-1);	FMGL->redraw();	}
}
//-----------------------------------------------------------------------------
void mglCanvasFL::ToggleAlpha()
{
	alpha = 1-alpha;
	alpha_bt->value(alpha);
	alpha_bt->image(alpha?xpm_a2:xpm_a1);	alpha_bt->redraw();
	Fl_Menu_Item *m = (Fl_Menu_Item *)menu->find_item(gettext("Graphics/Alpha"));
	if(m && alpha)	m->set();
	if(m && !alpha)	m->clear();
	Update();
}
void alpha_cb(Fl_Widget*, void* v)
{	mglCanvasFL *g=dynamic_cast<mglCanvasFL*>((mglCanvas*)v);	if(g)	g->ToggleAlpha();	}
//-----------------------------------------------------------------------------
void mglCanvasFL::ToggleLight()
{
	light = 1-light;
	light_bt->value(light);
	light_bt->image(light?xpm_l2:xpm_l1);	light_bt->redraw();
	Fl_Menu_Item *m = (Fl_Menu_Item *)menu->find_item(gettext("Graphics/Light"));
	if(m && light)	m->set();
	if(m && !light)	m->clear();
	Update();
}
void light_cb(Fl_Widget*, void* v)
{	mglCanvasFL *g=dynamic_cast<mglCanvasFL*>((mglCanvas*)v);	if(g)	g->ToggleLight();	}
//-----------------------------------------------------------------------------
void draw_cb(Fl_Widget*, void* v)
{	mglCanvasFL *g=dynamic_cast<mglCanvasFL*>((mglCanvas*)v);	if(g)	g->Update();	}
//-----------------------------------------------------------------------------
void mglCanvasFL::ToggleNo()
{
	rotate_bt->value(0);	rotate_bt->image(xpm_r1);	rotate_bt->redraw();
	tet->value(0);			phi->value(0);
	rotate = false;			Update();
}
void norm_cb(Fl_Widget*, void* v)
{	mglCanvasFL *g=dynamic_cast<mglCanvasFL*>((mglCanvas*)v);	if(g)	g->ToggleNo();	}
//-----------------------------------------------------------------------------
void mglCanvasFL::ToggleRotate()
{
	FMGL->set_state(rotate_bt->value()!=0);
	rotate_bt->image(rotate_bt->value()?xpm_r2:xpm_r1);
	rotate_bt->redraw();
	if(!rotate_bt->value())	Update();
}
void rotate_cb(Fl_Widget*, void* v)
{	mglCanvasFL *g=dynamic_cast<mglCanvasFL*>((mglCanvas*)v);	if(g)	g->ToggleRotate();	}
//-----------------------------------------------------------------------------
void mglCanvasFL::Update()
{
	FMGL->set_state(rotate_bt->value());
	FMGL->set_flag(alpha + 2*light);
	FMGL->set_angle(tet->value(), phi->value());
	SetCurFig(0);
	FMGL->update();
}
//-----------------------------------------------------------------------------
void export_png_cb(Fl_Widget*, void* v)
{
	char *fname = fl_file_chooser(gettext("Save File As?"), "*.png", 0);
	if(!fname || !fname[0])	return;
	((mglCanvas*)v)->WritePNG(fname);
}
//-----------------------------------------------------------------------------
void export_bps_cb(Fl_Widget*, void* v)
{
	char *fname = fl_file_chooser(gettext("Save File As?"), "*.eps", 0);
	if(!fname || !fname[0])	return;
	((mglCanvas*)v)->WriteEPS(fname);
}
//-----------------------------------------------------------------------------
void export_pngn_cb(Fl_Widget*, void* v)
{
	char *fname = fl_file_chooser(gettext("Save File As?"), "*.png", 0);
	if(!fname || !fname[0])	return;
	((mglCanvas*)v)->WritePNGs(fname,0);
}
//-----------------------------------------------------------------------------
void export_jpeg_cb(Fl_Widget*, void* v)
{
	char *fname = fl_file_chooser(gettext("Save File As?"), "*.jpg", 0);
	if(!fname || !fname[0])	return;
	((mglCanvas*)v)->WriteJPEG(fname);
}
//-----------------------------------------------------------------------------
void export_svg_cb(Fl_Widget*, void* v)
{
	char *fname = fl_file_chooser(gettext("Save File As?"), "*.svg", 0);
	if(!fname || !fname[0])	return;
	((mglCanvas*)v)->WriteSVG(fname);
}
//-----------------------------------------------------------------------------
void export_eps_cb(Fl_Widget*, void* v)
{
	char *fname = fl_file_chooser(gettext("Save File As?"), "*.eps", 0);
	if(!fname || !fname[0])	return;
	((mglCanvas*)v)->WriteEPS(fname);
}
//-----------------------------------------------------------------------------
void oncemore_cb(Fl_Widget*, void*v)
{	mglCanvasFL *g=dynamic_cast<mglCanvasFL*>((mglCanvas*)v);	if(g)	g->ReLoad();	}
//-----------------------------------------------------------------------------
void mglCanvasFL::Adjust()
{
	SetSize(scroll->w(), scroll->h());
	FMGL->size(scroll->w(), scroll->h());
	Update();
}
void adjust_cb(Fl_Widget*, void*v)
{	mglCanvasFL *g=dynamic_cast<mglCanvasFL*>((mglCanvas*)v);	if(g)	g->Adjust();	}
//-----------------------------------------------------------------------------
void quit_cb(Fl_Widget*, void*v)
{	mglCanvasFL *g=dynamic_cast<mglCanvasFL*>((mglCanvas*)v);	if(g)	g->Wnd->hide();	}
//-----------------------------------------------------------------------------
void snext_cb(Fl_Widget*, void* v)
{	mglCanvasFL *g=dynamic_cast<mglCanvasFL*>((mglCanvas*)v);	if(g)	g->NextFrame();	}
//-----------------------------------------------------------------------------
void sprev_cb(Fl_Widget*, void* v)
{	mglCanvasFL *g=dynamic_cast<mglCanvasFL*>((mglCanvas*)v);	if(g)	g->PrevFrame();	}
//-----------------------------------------------------------------------------
void time_cb(void *v)
{
	mglCanvasFL *g=dynamic_cast<mglCanvasFL*>((mglCanvas*)v);
	if(!g || !g->sshow)	return;
	g->NextFrame();
	Fl::repeat_timeout(g->GetDelay(), time_cb, v);
}
//-----------------------------------------------------------------------------
void mglCanvasFL::Animation()
{
	sshow = GetNumFig()<2 ? 0 : 1-sshow;
	anim_bt->value(sshow);
	anim_bt->image(sshow?xpm_s2:xpm_s1);	anim_bt->redraw();
	Fl_Menu_Item *m = (Fl_Menu_Item *)menu->find_item(gettext("Animate/Slideshow"));
	if(m && sshow)	m->set();
	if(m && !sshow)	m->clear();
	if(sshow)	Fl::add_timeout(GetDelay(), time_cb, this);
}
void sshow_cb(Fl_Widget *, void *v)
{	mglCanvasFL *g=dynamic_cast<mglCanvasFL*>((mglCanvas*)v);	if(g)	g->Animation();	}
void no_cb(Fl_Widget *, void *)	{}
//-----------------------------------------------------------------------------
Fl_Menu_Item pop_graph[15] = {
	{ gettext("Export"), 0, no_cb, 0, FL_SUBMENU,0,0,0,0},
		{ gettext("... as PNG"),	0, export_png_cb,0,0,0,0,0,0 },
		{ gettext("... as PNG (solid)"),	0, export_pngn_cb,0,0,0,0,0,0 },
		{ gettext("... as JPEG"),	0, export_jpeg_cb,0,0,0,0,0,0 },
		{ gettext("... as SVG"),	0, export_svg_cb,0,0,0,0,0,0 },
		{ gettext("... as vector EPS"),	0, export_eps_cb,0,0,0,0,0,0 },
		{ gettext("... as bitmap EPS"),	0, export_bps_cb, 0, FL_MENU_DIVIDER,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0 },
	{ gettext("Copy graphics"),	0, 0, 0, FL_MENU_INACTIVE|FL_MENU_DIVIDER,0,0,0,0},
	{ gettext("Normal view"),	0, norm_cb,0,0,0,0,0,0 },
	{ gettext("Redraw plot"),	0, draw_cb,0,0,0,0,0,0 },
	{ gettext("Adjust size"),	0, adjust_cb,0,0,0,0,0,0 },
	{ gettext("Reload data"),	0, oncemore_cb,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0 }
};
//-----------------------------------------------------------------------------
Fl_Menu_Item menuitems[] = {
	{ gettext("Export"), 0, 0, 0, FL_SUBMENU,0,0,0,0 },
		{ gettext(".. as PNG"),	FL_ALT + 'p', export_png_cb,0,0,0,0,0,0 },
		{ gettext(".. as PNG (solid)"),	FL_ALT + 'f', export_pngn_cb,0,0,0,0,0,0 },
		{ gettext(".. as JPEG"),	FL_ALT + 'j', export_jpeg_cb,0,0,0,0,0,0 },
		{ gettext(".. as SVG"),	FL_ALT + 's', export_svg_cb,0,0,0,0,0,0 },
		{ gettext(".. as vector EPS"),	FL_ALT + 'e', export_eps_cb,0,0,0,0,0,0 },
		{ gettext(".. as bitmap EPS"),	0, export_bps_cb, 0, FL_MENU_DIVIDER,0,0,0,0 },
		{ gettext("Exit"),			FL_ALT + 'x', quit_cb,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0 },
	{ gettext("Animate"), 0, 0, 0, FL_SUBMENU,0,0,0,0 },
		{ gettext("Slideshow"),	FL_CTRL + FL_F + 5, sshow_cb, 0, FL_MENU_TOGGLE,0,0,0,0 },
		{ gettext("Next frame"),0, snext_cb,0,0,0,0,0,0 },
		{ gettext("Prev frame"),0, sprev_cb,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0 },
	{ gettext("Graphics"), 0, 0, 0, FL_SUBMENU,0,0,0,0 },
		{ gettext("Alpha"),			FL_ALT + 'a', alpha_cb, 0, FL_MENU_TOGGLE,0,0,0,0 },
		{ gettext("Light"),			FL_ALT + 'l', light_cb, 0, FL_MENU_TOGGLE,0,0,0,0 },
		{ gettext("Copy graphics"),	0, 0, 0, FL_MENU_INACTIVE|FL_MENU_DIVIDER,0,0,0,0},
		{ gettext("Normal view"),	FL_ALT + ' ', norm_cb,0,0,0,0,0,0 },
		{ gettext("Redraw plot"),FL_F + 5, draw_cb,0,0,0,0,0,0 },
		{ gettext("Adjust size"),	FL_F + 6, adjust_cb,0,0,0,0,0,0 },
		{ gettext("Reload data"),	0, oncemore_cb,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0 }
};
//-----------------------------------------------------------------------------
void mglCanvasFL::Window(int argc, char **argv, int (*draw)(mglBase *gr, void *p), const char *title, void *par, void (*reload)(void *p), bool maximize)
{
	SetDrawFunc(draw, par, reload);
	if(Wnd)	{	Wnd->label(title);	Wnd->show();	return;	}

	Fl_Window *w1=new Fl_Double_Window(0,0,830,660,title);
	menu = new Fl_Menu_Bar(0, 0, 830, 30);
	menu->copy(menuitems, this);

	Fl_Group *g = new Fl_Group(0,30,410,30);
	Fl_Button *o;

	alpha_bt = new Fl_Button(0, 31, 25, 25);		alpha_bt->type(FL_TOGGLE_BUTTON);
	alpha_bt->image(xpm_a1);	alpha_bt->callback(alpha_cb,this);
	alpha_bt->tooltip(gettext("Switch on/off transparency in the picture"));
	alpha_bt->box(FL_PLASTIC_UP_BOX);			alpha_bt->down_box(FL_PLASTIC_DOWN_BOX);
	light_bt = new Fl_Button(25, 31, 25, 25);		light_bt->type(FL_TOGGLE_BUTTON);
	light_bt->image(xpm_l1);	light_bt->callback(light_cb,this);
	light_bt->tooltip(gettext("Switch on/off lightning in the picture"));
	light_bt->box(FL_PLASTIC_UP_BOX);			light_bt->down_box(FL_PLASTIC_DOWN_BOX);

	rotate_bt = new Fl_Button(55, 31, 25, 25);	rotate_bt->type(FL_TOGGLE_BUTTON);
	rotate_bt->image(xpm_r1);	rotate_bt->callback(rotate_cb,this);
	rotate_bt->tooltip(gettext("Rotate picture by holding left mouse button"));
	rotate_bt->box(FL_PLASTIC_UP_BOX);			rotate_bt->down_box(FL_PLASTIC_DOWN_BOX);
	o = new Fl_Button(105, 31, 25, 25);	o->tooltip(gettext("Return picture to normal zoom"));
	o->image(new Fl_Pixmap(zoom_out_xpm));	o->callback(norm_cb,this);
	o->box(FL_PLASTIC_UP_BOX);	o->down_box(FL_PLASTIC_DOWN_BOX);

	o = new Fl_Button(135, 31, 25, 25);	o->tooltip(gettext("Refresh the picture"));
	o->image(new Fl_Pixmap(ok_xpm));	o->callback(draw_cb,this);
	o->box(FL_PLASTIC_UP_BOX);	o->down_box(FL_PLASTIC_DOWN_BOX);

	tet = new Fl_Counter(170, 31, 90, 25, 0);	tet->callback(draw_cb,this);
	phi = new Fl_Counter(265, 31, 90, 25, 0);	phi->callback(draw_cb,this);
	tet->lstep(10);	tet->step(1);	tet->range(-180,180);
	tet->tooltip(gettext("Theta angle (tilt z-axis)"));
	phi->lstep(10);	phi->step(1);	phi->range(-180,180);
	phi->tooltip(gettext("Phi angle (rotate in x*y plane)"));
	tet->box(FL_PLASTIC_UP_BOX);			phi->box(FL_PLASTIC_UP_BOX);
	g->end();	g->resizable(0);

	g = new Fl_Group(0,60,30,260);
	o = new Fl_Button(1, 60, 25, 25);		o->tooltip(gettext("Show previous frame in slideshow"));
	o->image(new Fl_Pixmap(prev_sl_xpm));	o->callback(sprev_cb,this);
	o->box(FL_PLASTIC_UP_BOX);	o->down_box(FL_PLASTIC_DOWN_BOX);
	anim_bt = new Fl_Button(1, 85, 25, 25);		anim_bt->type(FL_TOGGLE_BUTTON);
	anim_bt->image(xpm_s1);	anim_bt->callback(sshow_cb,this);
	anim_bt->tooltip(gettext("Run/Stop slideshow (graphics animation)"));
	anim_bt->box(FL_PLASTIC_UP_BOX);		anim_bt->down_box(FL_PLASTIC_DOWN_BOX);
	o = new Fl_Button(1, 110, 25, 25);		o->tooltip(gettext("Show next frame in slideshow"));
	o->image(new Fl_Pixmap(next_sl_xpm));	o->callback(snext_cb,this);
	o->box(FL_PLASTIC_UP_BOX);	o->down_box(FL_PLASTIC_DOWN_BOX);

	g->end();	g->resizable(0);

	scroll = new Fl_Scroll(30, 60, 800, 600);
	//scroll->begin();
	FMGL = new Fl_MathGL(30, 60, 800, 600);
	FMGL->tet_val = tet;
	FMGL->phi_val = phi;
	FMGL->set_popup(pop_graph,w1,this);
	FMGL->set_graph(this);
	FMGL->set_draw(draw, par);
	scroll->end();

	w1->end();
	Wnd = w1;
	w1->resizable(scroll);	//w->graph);

	if(maximize)
	{
		int x,y,w,h;
		Fl::screen_xywh(x,y,w,h);
		w1->resize(x,y,w,h);
		Adjust();
	}

	char *tmp[1];	tmp[0]=new char[1];	tmp[0][0]=0;
	w1->show(argv ? argc:0, argv ? argv:tmp);
	delete []tmp[0];
}
//-----------------------------------------------------------------------------
HMGL mgl_create_graph_fltk(int (*draw)(HMGL gr, void *p), const char *title, void *par)
{
	mglCanvasFL *g = new mglCanvasFL;
	g->Window(0,0,draw,title,par);
	return g;
}
void mgl_fltk_run()		{	Fl::run();	}
//-----------------------------------------------------------------------------
uintptr_t mgl_create_graph_fltk_(const char *title, int l)
{
	char *s = new char[l+1];	memcpy(s,title,l);	s[l]=0;
	uintptr_t t = uintptr_t(mgl_create_graph_fltk(0,s,0));
	delete []s;	return t;
}
void mgl_fltk_run_()	{	mgl_fltk_run();	}
//-----------------------------------------------------------------------------
void *mgl_fl_tmp(void *)	{	Fl::run();	return 0;	}
/*void mgl_fltk_thread()
{
	static pthread_t tmp;
	pthread_create(&tmp, 0, mgl_fl_tmp, 0);
	pthread_detach(tmp);
}*/
//-----------------------------------------------------------------------------
