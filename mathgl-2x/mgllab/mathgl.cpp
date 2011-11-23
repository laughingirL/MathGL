/* mathgl.cpp is part of UDAV
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <stdio.h>
#include "mgl/mgl.h"
#include "udav.h"
//-----------------------------------------------------------------------------
#include "xpm/alpha.xpm"
#include "xpm/light.xpm"
#include "xpm/alpha_on.xpm"
#include "xpm/light_on.xpm"
#include "xpm/zoom-fit-best.xpm"
#include "xpm/zoom-fit-best-r.xpm"
#include "xpm/film-r.xpm"
#include "xpm/film-b.xpm"
#include "xpm/media-seek-forward.xpm"
#include "xpm/media-seek-backward.xpm"
#include "xpm/go-previous.xpm"
#include "xpm/go-next.xpm"
#include "xpm/go-down.xpm"
#include "xpm/zoom-out.xpm"
#include "xpm/zoom-in.xpm"
#include "xpm/go-up.xpm"
#include "xpm/zoom-original.xpm"
#include "xpm/view-refresh.xpm"
#include "xpm/rotate.xpm"
#include "xpm/rotate_on.xpm"
#include "xpm/document-properties.xpm"
//#include "xpm/preferences-system.xpm"
#include "xpm/wire.xpm"
//=============================================================================
//	Fl_MathGL class realization
//=============================================================================
//-----------------------------------------------------------------------------
extern int internal_font;
mglParse *Parse=0;
void get_doc_dir(char *&docdir);
Fl_Pixmap xpm_a1(alpha_xpm), xpm_a2(alpha_on_xpm);
Fl_Pixmap xpm_l1(light_xpm), xpm_l2(light_on_xpm);
Fl_Pixmap xpm_z1(zoom_fit_best_xpm), xpm_z2(zoom_fit_best_r_xpm);
Fl_Pixmap xpm_s1(film_b_xpm), xpm_s2(film_r_xpm);
Fl_Pixmap xpm_r1(rotate_xpm), xpm_r2(rotate_on_xpm);
//-----------------------------------------------------------------------------
Fl_MGL::Fl_MGL(int x, int y, int w, int h, char *label) : Fl_MathGL(x,y,w,h,label)
{
	if(!Parse)	Parse = new mglParse;

#ifdef WIN32
//	setlocale(LC_TYPE,"russian_Russia.CP1251");
	char *path;
	get_doc_dir(path);
	if(!graph->LoadFont("STIX",path && path[0] ? path : "."))	graph->RestoreFont();
#endif

	parse = Parse;	parse->AllowSetSize(true);
	script = script_pre = 0;
}
//-----------------------------------------------------------------------------
Fl_MGL::~Fl_MGL()	{	clear_scripts();	}
//-----------------------------------------------------------------------------
void Fl_MGL::clear_scripts()
{
	if(script)		free(script);
	if(script_pre)	free(script_pre);
}
//-----------------------------------------------------------------------------
void Fl_MGL::scripts(char *scr, char *pre)
{	clear_scripts();	script=scr;	script_pre=pre;	}
//-----------------------------------------------------------------------------
void Fl_MGL::draw()
{
	Fl_MathGL::draw();
	if(wire)
	{
		char str[5]="0.0";
		int i, h=gr->GetHeight(), w=gr->GetWidth();
		fl_color(192,192,192);
		for(i=1;i<10;i++)
		{
			str[2] = '0'+10-i;	fl_draw(str,30,30+i*h/10);
			fl_line(30,30+i*h/10,30+w,30+i*h/10);
			str[2] = '0'+i;	fl_draw(str,30+i*w/10,30+h);
			fl_line(30+i*w/10,30,30+i*w/10,30+h);
		}
		if(*MouseBuf)	fl_draw(MouseBuf,30,50);
	}
}
//-----------------------------------------------------------------------------
void Fl_MGL::update(mglGraph *gr)
{
	if(!parse)	return;
//	parse->Stop = false;
	if(gr==0)	gr=graph;
	gr->DefaultPlotParam();	gr->Fog(0);
	wire = flag&4;
	gr->Alpha(flag&1);	gr->Light(flag&2);
	gr->View(tet,phi);	gr->Perspective(per);
	gr->Zoom(x1,y1,x2,y2);
//	gr->DrawFace = !rotate;

	if(!script || !strstr(script,"rotate"))	gr->Rotate(0,0);

	char *s,*c,*t,*buf=new char[2048];
//	gr->Message = buf;
	long res,pos=0;
	Fl_Text_Buffer tb;
	if(script_pre)
	{
		s = strdup(script_pre);
		c = strtok(s,"\n");
		while(c)
		{
			buf[0] = 0;
			if(parse->Stop)	break;
			res = parse->Parse(gr,c);
			if(res==1)	{sprintf(buf,gettext("Wrong argument(s) in %s - line (%ld)\n"),c,pos);	tb.append(buf);}
			if(res==2)	{sprintf(buf,gettext("Wrong command in %s - line (%ld)\n"),c,pos);	tb.append(buf);}
			if(gr->Message())
			{
				tb.append(gr->Message());
				sprintf(buf," - line (%ld)\n",pos);
				tb.append(buf);
			}
			c = strtok(0,"\n");	pos++;
		}
		free(s);
	}
	if(script)
	{
		s = strdup(script);	pos=0;
		c=s;	t = strchr(c,'\n');
		if(t)	{*t=0;	t++;}
		while(c)
		{
			if(parse->Stop)	break;
			buf[0] = 0;
			res = parse->Parse(gr,c);
			if(res==1)	sprintf(buf,gettext("Wrong argument(s) in %s"),c);
			if(res==2)	sprintf(buf,gettext("Wrong command in %s"),c);
			if(*buf)
			{
				tb.append(buf);
				sprintf(buf," - line %ld\n",pos);
				tb.append(buf);
			}
			if(t==0)	break;
			c = t;	t = strchr(c,'\n');	pos++;
			if(t)	{*t=0;	t++;}
		}
		free(s);
	}
	gr->Message=0;
	if(tb.length()>0)	fl_alert("%s",tb.text());
	delete []buf;
	if(gr==graph)	size(graph->GetWidth(),graph->GetHeight());
	redraw();
}
//-----------------------------------------------------------------------------
void Fl_MGL::resize(int x, int y, int w, int h)
{
	if(w!=graph->GetWidth() || h!=graph->GetHeight())
	{
		graph->SetSize(w,h);
		update();
	}
	Fl_Widget::resize(x,y,w,h);
}
//-----------------------------------------------------------------------------
int Fl_MGL::handle(int code)
{
	if(popup && code==FL_PUSH && Fl::event_button()==FL_RIGHT_MOUSE)
	{
		const Fl_Menu_Item *m = popup->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);
		if(m)	m->do_callback(wpar, vpar);
	}
	if(code==FL_PUSH && Fl::event_button()==FL_LEFT_MOUSE && status)
	{
		mglPoint p=graph->CalcXYZ(Fl::event_x()-30,Fl::event_y()-30);
		sprintf(MouseBuf,"Abs coor. {%.2g,%.2g}, Lab coor. {%.2g,%.2g}",
				(Fl::event_x()-30.)/w(),(h()-Fl::event_y()+30.)/h(),p.x,p.y);
		status->copy_label(MouseBuf);	redraw();	Fl::flush();
	}
	if((!zoom && !rotate) || Fl::event_button()!=FL_LEFT_MOUSE)
	{
		if(code==FL_FOCUS || code==FL_UNFOCUS)	return 1;
		if(code==FL_KEYUP)
		{
			float x1,x2,y1,y2,d;
			get_zoom(&x1,&y1,&x2,&y2);
			int key=Fl::event_key();
			if(key==FL_Left && Fl::event_state()&FL_META)
			{
				d = (x2-x1)/3;	x1 += d;	x2 += d;
				set_zoom(x1,y1,x2,y2);	update();	return 1;
			}
			if(key==FL_Right && Fl::event_state()&FL_META)
			{
				d = (x2-x1)/3;	x1 -= d;	x2 -= d;
				set_zoom(x1,y1,x2,y2);	update();	return 1;
			}
			if(key==FL_Down && Fl::event_state()&FL_META)
			{
				d = (y2-y1)/3;	y1 += d;	y2 += d;
				set_zoom(x1,y1,x2,y2);	update();	return 1;
			}
			if(key==FL_Up && Fl::event_state()&FL_META)
			{
				d = (y2-y1)/3;	y1 -= d;	y2 -= d;
				set_zoom(x1,y1,x2,y2);	update();	return 1;
			}
			if(!(Fl::event_state()&FL_META) || !strchr("wasdrfzxcv",key))
				return 0;
			if(key=='w')
			{
				tet += 10;	if(tet_val)	tet_val->value(tet);
				update();	return 1;
			}
			if(key=='s')
			{
				tet -= 10;	if(tet_val)	tet_val->value(tet);
				update();	return 1;
			}
			if(key=='a')
			{
				phi += 10;	if(phi_val)	phi_val->value(phi);
				update();	return 1;
			}
			if(key=='d')
			{
				phi -= 10;	if(phi_val)	phi_val->value(phi);
				update();	return 1;
			}
			if(key=='r')
			{	flag = (flag&2) + ((~(flag&1))&1);	update();	return 1;	}
			if(key=='f')
			{	flag = (flag&1) + ((~(flag&2))&2);	update();	return 1;	}
			if(key=='c')
			{	per += 0.1;	update();	return 1;	}
			if(key=='v')
			{	per -= 0.1;	update();	return 1;	}
			if(key=='z')	// zoom in
			{
				d = (y2-y1)/4;	y1 += d;	y2 -= d;
				d = (x2-x1)/4;	x1 += d;	x2 -= d;
				set_zoom(x1,y1,x2,y2);	update();	return 1;
			}
			if(key=='x')	// zoom out
			{
				d = (y2-y1)/2;	y1 -= d;	y2 += d;
				d = (x2-x1)/2;	x1 -= d;	x2 += d;
				set_zoom(x1,y1,x2,y2);	update();	return 1;
			}
		}
		return 0;
	}
	if(code==FL_PUSH)
	{	xe=x0=Fl::event_x();	ye=y0=Fl::event_y();	redraw();	}
	if(code==FL_DRAG)
	{
		xe=Fl::event_x();	ye=Fl::event_y();
//		int w1=w(),h1=h();
		float ff = 240/sqrt(float(w()*h()));
		if(rotate && Fl::event_button()==FL_LEFT_MOUSE)		// rotate
		{
			phi += int((x0-xe)*ff);
			tet += int((y0-ye)*ff);
			if(phi>180)	phi-=360;		if(phi<-180)	phi+=360;
			if(tet>180)	tet-=360;		if(tet<-180)	tet+=360;
			x0 = xe;	y0 = ye;
			update();
		}
/*		if(rotate && Fl::event_button()==FL_MIDDLE_MOUSE)	// shift
		{
			float ff = 1./sqrt(float(w()*h()));
			float dx = (x0-xe)*ff*(x2-x1), dy = (y0-ye)*ff*(y2-y1);
			x1 += dx;	x2 += dx;	y1 -= dy;	y2 -= dy;
			x0 = xe;	y0 = ye;
			update();
		}
		if(rotate && Fl::event_button()==FL_RIGHT_MOUSE)	// zoom and perspective
		{
			float ff = 2.*(y0-ye)/w();
			float cx = (x1+x2)/2, cy = (y1+y2)/2;
			x1 = cx+(x1-cx)*exp(-ff);	x2 = cx+(x2-cx)*exp(-ff);
			y1 = cy+(y1-cy)*exp(-ff);	y2 = cy+(y2-cy)*exp(-ff);
			per += 0.5*(xe-x0)/h();
			if(per<0)	per = 0;	if(per>=1)	per = 0.9999;
			x0 = xe;	y0 = ye;
			update();
		}*/
		redraw();
	}
	if(code==FL_RELEASE)
	{
		if(zoom)
		{
			int w1=w(),h1=h();
			float _x1,_x2,_y1,_y2;
			_x1 = x1+(x2-x1)*(x0-x())/float(w1);	_y1 = y2-(y2-y1)*(ye-y())/float(h1);
			_x2 = x1+(x2-x1)*(xe-x())/float(w1);	_y2 = y2-(y2-y1)*(y0-y())/float(h1);
			x1=_x1;		x2=_x2;		y1=_y1;		y2=_y2;
			if(x1>x2)	{	_x1=x1;	x1=x2;	x2=_x1;	}
			if(y1>y2)	{	_x1=y1;	y1=y2;	y2=_x1;	}
			update();
		}
		else
		{
			if(tet_val)	tet_val->value(tet);
			if(phi_val)	phi_val->value(phi);
		}
		redraw();
	}
	return 1;
}
//=============================================================================
//	Class realization finish
//=============================================================================
void wire_cb(Fl_Widget*, void* v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	e->wire = 1-e->wire;
	e->wire_bt->value(e->wire);
	Fl_Menu_Item *m = (Fl_Menu_Item *)e->menu->find_item(gettext("Graphics/Wire"));
	if(m && e->wire)	m->set();
	if(m && !e->wire)	m->clear();
	e->update_pre();
}
//-------------------------------------------------------------------------void alpha_cb(Fl_Widget*, void* v)
void alpha_cb(Fl_Widget*, void* v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	e->alpha = 1-e->alpha;
	e->alpha_bt->value(e->alpha);
	e->alpha_bt->image(e->alpha?xpm_a2:xpm_a1);	e->alpha_bt->redraw();
	Fl_Menu_Item *m = (Fl_Menu_Item *)e->menu->find_item(gettext("Graphics/Alpha"));
	if(m && e->alpha)	m->set();
	if(m && !e->alpha)	m->clear();
	e->update_pre();
}
//-----------------------------------------------------------------------------
void light_cb(Fl_Widget*, void* v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	e->light = 1-e->light;
	e->light_bt->value(e->light);
	e->light_bt->image(e->light?xpm_l2:xpm_l1);	e->light_bt->redraw();
	Fl_Menu_Item *m = (Fl_Menu_Item *)e->menu->find_item(gettext("Graphics/Light"));
	if(m && e->light)	m->set();
	if(m && !e->light)	m->clear();
	e->update_pre();
}
//-----------------------------------------------------------------------------
void draw_cb(Fl_Widget*, void* v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	e->update_pre();
}
//-----------------------------------------------------------------------------
void norm_cb(Fl_Widget*, void* v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	e->zoom_bt->value(0);	e->zoom_bt->image(xpm_z1);		e->zoom_bt->redraw();
	e->rotate_bt->value(0);	e->rotate_bt->image(xpm_r1);	e->rotate_bt->redraw();

	e->tet->value(0);
	e->phi->value(0);
	e->zoom = e->rotate = false;
	e->graph->set_zoom(0,0,1,1);
	e->update_pre();
}
//-----------------------------------------------------------------------------
void zoom_cb(Fl_Widget*, void* v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	e->rotate_bt->value(0);	e->rotate_bt->image(xpm_r1);	e->rotate_bt->redraw();

	e->graph->set_state(e->zoom_bt->value(), false);
	e->zoom_bt->image(e->zoom_bt->value()?xpm_z2:xpm_z1);
	e->zoom_bt->redraw();
}
//-----------------------------------------------------------------------------
void rotate_cb(Fl_Widget*, void* v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	e->zoom_bt->value(0);	e->zoom_bt->image(xpm_z1);	e->zoom_bt->redraw();

	e->graph->set_state(false, e->rotate_bt->value()!=0);
	e->rotate_bt->image(e->rotate_bt->value()?xpm_r2:xpm_r1);
	e->rotate_bt->redraw();

	if(!e->rotate_bt->value())	e->update_pre();
}
//-----------------------------------------------------------------------------
/*void code_cb(Fl_Widget *, void *v)
{
	ScriptWindow* e = (ScriptWindow*)v;
//	fl_message("setup code are:\n%s",e->setup_dlg->ToScript());
	char *s = e->setup_dlg->ToScript();
	int res=fl_choice(gettext("setup code are:\n%s"),0,gettext("Close"),gettext("Insert"),s);
	if(res==2)
	{
		e->editor->insert_position(0);
		e->editor->insert(s);
		delete e->setup_dlg;
		e->setup_dlg = new SetupDlg;
		e->setup_dlg->CreateDlg();
	}
	free(s);
}*/
//-----------------------------------------------------------------------------
void ScriptWindow::update_pre()
{
	graph->set_state(zoom_bt->value(), rotate_bt->value());
	graph->set_state(alpha + 2*light + 4*wire);
	graph->scripts(textbuf->text(), setup_dlg->OK ? setup_dlg->ToScript():0);
	graph->set_angle(tet->value(), phi->value());
	graph->update();

	mglVar *v = Parse->Self()->DataList;
	while(v)
	{
		if(v->o)	((TableWindow *)v->o)->update(v);
		v = v->next;
	}
}
//-----------------------------------------------------------------------------
void add_suffix(char *fname, const char *ext)
{
	long n=strlen(fname);
	if(n>4 && fname[n-4]=='.')
	{	fname[n-3]=ext[0];	fname[n-2]=ext[1];	fname[n-1]=ext[2];	}
	else	{	strcat(fname,".");	strcat(fname,ext);	}

}
//-----------------------------------------------------------------------------
void oncemore_cb(Fl_Widget*, void*v)
{
	Parse->RestoreOnce();
	ScriptWindow* e = (ScriptWindow*)v;
	e->update_pre();
}
//-----------------------------------------------------------------------------
void adjust_cb(Fl_Widget*, void*v);
void no_cb(Fl_Widget *, void *)	{}
//-----------------------------------------------------------------------------
Fl_Widget *add_graph(ScriptWindow *w)
{
	Fl_Window *w1=new Fl_Window(300,30,630,430,0);
	Fl_Group *g = new Fl_Group(0,0,410,30);
	Fl_Button *o;

	o = new Fl_Button(0, 1, 25, 25);	o->type(FL_TOGGLE_BUTTON);
	o->image(xpm_a1);	o->callback(alpha_cb,w);	w->alpha_bt = o;
	o->tooltip(gettext("Switch on/off transparency in the picture"));
	o->box(FL_PLASTIC_UP_BOX);			o->down_box(FL_PLASTIC_DOWN_BOX);
	o = new Fl_Button(25, 1, 25, 25);	o->type(FL_TOGGLE_BUTTON);
	o->image(xpm_l1);	o->callback(light_cb,w);	w->light_bt = o;
	o->tooltip(gettext("Switch on/off lightning in the picture"));
	o->box(FL_PLASTIC_UP_BOX);			o->down_box(FL_PLASTIC_DOWN_BOX);

	o = new Fl_Button(55, 1, 25, 25);	o->type(FL_TOGGLE_BUTTON);
	o->image(xpm_r1);	o->callback(rotate_cb,w);	w->rotate_bt = o;
	o->tooltip(gettext("Rotate picture by holding left mouse button"));
	o->box(FL_PLASTIC_UP_BOX);			o->down_box(FL_PLASTIC_DOWN_BOX);
	o = new Fl_Button(80, 1, 25, 25);	o->type(FL_TOGGLE_BUTTON);
	o->image(xpm_z1);	o->callback(zoom_cb,w);	w->zoom_bt = o;
	o->tooltip(gettext("Zoom in selected region of the picture"));
	o->box(FL_PLASTIC_UP_BOX);			o->down_box(FL_PLASTIC_DOWN_BOX);
	o = new Fl_Button(105, 1, 25, 25);	o->tooltip(gettext("Return picture to normal zoom"));
	o->image(new Fl_Pixmap(zoom_original_xpm));	o->callback(norm_cb,w);
	o->box(FL_PLASTIC_UP_BOX);	o->down_box(FL_PLASTIC_DOWN_BOX);

	o = new Fl_Button(135, 1, 25, 25);	o->tooltip(gettext("Refresh the picture"));
	o->image(new Fl_Pixmap(view_refresh_xpm));	o->callback(draw_cb,w);
	o->box(FL_PLASTIC_UP_BOX);	o->down_box(FL_PLASTIC_DOWN_BOX);
	o = new Fl_Button(165, 1, 25, 25);	o->tooltip(gettext("Show dialog for picture settings"));
	o->image(new Fl_Pixmap(document_properties_xpm));	o->callback(setup_cb,w);
	o->box(FL_PLASTIC_UP_BOX);	o->down_box(FL_PLASTIC_DOWN_BOX);
	o = new Fl_Button(190, 1, 25, 25);	o->tooltip(gettext("Show/hide grid of absolute coordinates"));
	o->image(new Fl_Pixmap(wire_xpm));	o->callback(wire_cb,w);
	o->box(FL_PLASTIC_UP_BOX);	o->down_box(FL_PLASTIC_DOWN_BOX);
	o->type(FL_TOGGLE_BUTTON);	w->wire_bt = o;

//	o = new Fl_Button(190, 1, 25, 25);	o->tooltip(gettext("Show settings code"));
//	o->image(new Fl_Pixmap(document_properties_xpm));	o->callback(code_cb,w);
//	o->box(FL_PLASTIC_UP_BOX);	o->down_box(FL_PLASTIC_DOWN_BOX);

	w->tet = new Fl_Counter(220, 1, 90, 25, 0);	w->tet->callback(draw_cb,w);
	w->phi = new Fl_Counter(315, 1, 90, 25, 0);	w->phi->callback(draw_cb,w);
	w->tet->lstep(10);	w->tet->step(1);	w->tet->range(-180,180);
	w->tet->tooltip(gettext("Theta angle (tilt z-axis)"));
	w->phi->lstep(10);	w->phi->step(1);	w->phi->range(-180,180);
	w->phi->tooltip(gettext("Phi angle (rotate in x*y plane)"));
	w->tet->box(FL_PLASTIC_UP_BOX);			w->phi->box(FL_PLASTIC_UP_BOX);
	g->end();	g->resizable(0);

	g = new Fl_Group(0,30,30,260);
	o = new Fl_Button(1, 30, 25, 25);		o->tooltip(gettext("Shift the picture up"));
	o->image(new Fl_Pixmap(go_up_xpm));		o->callback(sd_cb,w);
	o->box(FL_PLASTIC_UP_BOX);	o->down_box(FL_PLASTIC_DOWN_BOX);
	o = new Fl_Button(1, 55, 25, 25);		o->tooltip(gettext("Shift the picture left"));
	o->image(new Fl_Pixmap(go_previous_xpm));	o->callback(sr_cb,w);
	o->box(FL_PLASTIC_UP_BOX);	o->down_box(FL_PLASTIC_DOWN_BOX);
	o = new Fl_Button(1, 80, 25, 25);		o->tooltip(gettext("Zoom in the picture"));
	o->image(new Fl_Pixmap(zoom_in_xpm));	o->callback(sz_cb,w);
	o->box(FL_PLASTIC_UP_BOX);	o->down_box(FL_PLASTIC_DOWN_BOX);
	o = new Fl_Button(1, 105, 25, 25);		o->tooltip(gettext("Zoom out the picture"));
	o->image(new Fl_Pixmap(zoom_out_xpm));	o->callback(so_cb,w);
	o->box(FL_PLASTIC_UP_BOX);	o->down_box(FL_PLASTIC_DOWN_BOX);
	o = new Fl_Button(1, 130, 25, 25);		o->tooltip(gettext("Shift the picture right"));
	o->image(new Fl_Pixmap(go_next_xpm));	o->callback(sl_cb,w);
	o->box(FL_PLASTIC_UP_BOX);	o->down_box(FL_PLASTIC_DOWN_BOX);
	o = new Fl_Button(1, 155, 25, 25);		o->tooltip(gettext("Shift the picture down"));
	o->image(new Fl_Pixmap(go_down_xpm));	o->callback(su_cb,w);
	o->box(FL_PLASTIC_UP_BOX);	o->down_box(FL_PLASTIC_DOWN_BOX);

	o = new Fl_Button(1, 185, 25, 25);		o->tooltip(gettext("Show previous slide in slideshow"));
	o->image(new Fl_Pixmap(media_seek_backward_xpm));	o->callback(sprev_cb,w);
	o->box(FL_PLASTIC_UP_BOX);	o->down_box(FL_PLASTIC_DOWN_BOX);
	w->anim_bt = new Fl_Button(1, 210, 25, 25);		w->anim_bt->type(FL_TOGGLE_BUTTON);
	w->anim_bt->image(xpm_s1);	w->anim_bt->callback(sshow_cb,w);
	w->anim_bt->tooltip(gettext("Run/Stop slideshow (graphics animation)"));
	w->anim_bt->box(FL_PLASTIC_UP_BOX);		w->anim_bt->down_box(FL_PLASTIC_DOWN_BOX);
	o = new Fl_Button(1, 235, 25, 25);		o->tooltip(gettext("Show next slide in slideshow"));
	o->image(new Fl_Pixmap(media_seek_forward_xpm));	o->callback(snext_cb,w);
	o->box(FL_PLASTIC_UP_BOX);	o->down_box(FL_PLASTIC_DOWN_BOX);
	g->end();	g->resizable(0);

	w->scroll = new Fl_Scroll(30, 30, 600, 400);
	//w->scroll->begin();
	w->graph = new Fl_MGL(30, 30, 600, 400);
	w->graph->tet_val = w->tet;
	w->graph->phi_val = w->phi;
	w->graph->set_popup(pop_graph,w,w);
	w->scroll->end();

	w1->end();
	w1->resizable(w->scroll);	//w->graph);
	return w1;
}
//-----------------------------------------------------------------------------
