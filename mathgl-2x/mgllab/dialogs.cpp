/* setup.cpp is part of UDAV
 * Copyright (C) 2007-2014 Alexey Balakin <mathgl.abalakin@gmail.ru>
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
#include "mgl2/mgl.h"
#include "mgllab.h"
#include <FL/Fl_Spinner.H>
//-----------------------------------------------------------------------------
Fl_Menu_Item colors[] = {
	{"-----", 0,0,0,0,0,0,0, 0},	//
	{mgl_gettext("white"), 0,0,0,0,0,0,0, fl_rgb_color(0,0,0)},			//w
	{mgl_gettext("blue"), 0,0,0,0,0,0,0, fl_rgb_color(0,0,255)},		//b
	{mgl_gettext("lime"), 0,0,0,0,0,0,0, fl_rgb_color(0,255,0)},		//g
	{mgl_gettext("red"), 0,0,0,0,0,0,0, fl_rgb_color(255,0,0)},			//r
	{mgl_gettext("cyan"), 0,0,0,0,0,0,0, fl_rgb_color(0,255,255)},		//c
	{mgl_gettext("magenta"), 0,0,0,0,0,0,0, fl_rgb_color(255,0,255)},	//m
	{mgl_gettext("yellow"), 0,0,0,0,0,0,0, fl_rgb_color(255,255,0)},	//y
	{mgl_gettext("springgreen"), 0,0,0,0,0,0,0, fl_rgb_color(0,255,127)},//l
	{mgl_gettext("lawngreen"), 0,0,0,0,0,0,0, fl_rgb_color(127,255,0)},	//e
	{mgl_gettext("skyblue"), 0,0,0,0,0,0,0, fl_rgb_color(0,127,255)},	//n
	{mgl_gettext("blueviolet"), 0,0,0,0,0,0,0, fl_rgb_color(127,0,255)},//u
	{mgl_gettext("orange"), 0,0,0,0,0,0,0, fl_rgb_color(255,127,0)},	//q
	{mgl_gettext("deeppink"), 0,0,0,0,0,0,0, fl_rgb_color(255,0,127)},	//p
	{mgl_gettext("gray"), 0,0,0,0,0,0,0, fl_rgb_color(127,127,127)},	//h

	{mgl_gettext("black"), 0,0,0,0,0,0,0, fl_rgb_color(0,0,0)},			//k
	{mgl_gettext("lightgray"), 0,0,0,0,0,0,0, fl_rgb_color(179,179,179)},//W
	{mgl_gettext("navy"), 0,0,0,0,0,0,0, fl_rgb_color(0,0,127)},		//B
	{mgl_gettext("green"), 0,0,0,0,0,0,0, fl_rgb_color(0,127,0)},		//G
	{mgl_gettext("maroon"), 0,0,0,0,0,0,0, fl_rgb_color(127,0,0)},		//R
	{mgl_gettext("teal"), 0,0,0,0,0,0,0, fl_rgb_color(0,127,127)},		//C
	{mgl_gettext("purple"), 0,0,0,0,0,0,0, fl_rgb_color(127,0,127)},	//M
	{mgl_gettext("olive"), 0,0,0,0,0,0,0, fl_rgb_color(127,127,0)},		//Y
	{mgl_gettext("seagreen"), 0,0,0,0,0,0,0, fl_rgb_color(0,127,77)},	//L
	{mgl_gettext("darklawn"), 0,0,0,0,0,0,0, fl_rgb_color(77,127,0)},	//E
	{mgl_gettext("darkskyblue"), 0,0,0,0,0,0,0, fl_rgb_color(0,77,127)},//N
	{mgl_gettext("indigo"), 0,0,0,0,0,0,0, fl_rgb_color(77,0,127)},		//U
	{mgl_gettext("brown"), 0,0,0,0,0,0,0, fl_rgb_color(127,77,0)},		//Q
	{mgl_gettext("darkpink"), 0,0,0,0,0,0,0, fl_rgb_color(127,0,77)},	//P
	{mgl_gettext("darkgray"), 0,0,0,0,0,0,0, fl_rgb_color(77,77,77)},	//H
{0, 0,0,0,0,0,0,0, 0}};
//-----------------------------------------------------------------------------
void cb_dlg_cancel(Fl_Widget*, void *v)	{	if(v)	((GeneralDlg*)v)->hide();	}
void cb_dlg_ok(Fl_Widget*, void *v)		{	if(v)	((GeneralDlg*)v)->cb_ok();	}
void cb_filech(Fl_Widget*, void *v);
//-----------------------------------------------------------------------------
class PropDlg : public GeneralDlg
{
	friend void cb_filech(Fl_Widget*, void *v);
	Fl_Choice *fkind;
	Fl_Spinner *fsize;	
	Fl_File_Input *help_path;	
	Fl_File_Input *font_path;	
	Fl_Check_Button *auto_exec_w;
	Fl_Check_Button *exec_save_w;
	Fl_Check_Button *complete_w;
	Fl_Check_Button *highlight_w;
	Fl_Check_Button *mouse_zoom_w;
	Fl_Choice *lang_w;
	Fl_Choice *scheme_w;
public:
	HMGL gr;
	PropDlg()
	{
		Fl_Button *o;
		w = new Fl_Double_Window(340, 365, mgl_gettext("Properties"));
		w->align(Fl_Align(FL_ALIGN_CLIP|FL_ALIGN_INSIDE));
		fkind = new Fl_Choice(75, 10, 90, 25, mgl_gettext("Font kind"));
		fkind->add("Helvetica");	fkind->add("Courier");	fkind->add("Times");
		fsize = new Fl_Spinner(245, 10, 90, 25, mgl_gettext("Font size"));
		help_path = new Fl_File_Input(5, 55, 305, 35, mgl_gettext("Path for help files"));
		help_path->align(Fl_Align(FL_ALIGN_TOP_LEFT));
		o = new Fl_Button(310, 65, 25, 25, mgl_gettext("..."));	o->callback(cb_filech, 0);
		font_path = new Fl_File_Input(5, 110, 305, 35, mgl_gettext("Path for MathGL font (without extension)"));
		font_path->align(Fl_Align(FL_ALIGN_TOP_LEFT));
		o = new Fl_Button(310, 120, 25, 25, mgl_gettext("..."));	o->callback(cb_filech, (void *)1);
		auto_exec_w = new Fl_Check_Button(5, 145, 330, 25, mgl_gettext("Execute script after loading"));
		exec_save_w = new Fl_Check_Button(5, 170, 330, 25, mgl_gettext("Save file before redrawing"));
		complete_w = new Fl_Check_Button(5, 195, 330, 25, mgl_gettext("Enable keywords completion"));
		complete_w->deactivate();	// TODO
		highlight_w = new Fl_Check_Button(5, 220, 330, 25, mgl_gettext("Highlight current object(s)"));
		mouse_zoom_w = new Fl_Check_Button(5, 245, 330, 25, mgl_gettext("Enable mouse wheel for zooming"));
		lang_w = new Fl_Choice(160, 275, 175, 25, mgl_gettext("Language for mgllab"));
		lang_w->add("C.UTF8");	lang_w->add("ru_RU.utf8");	lang_w->add("ru_RU.cp1251");
		scheme_w = new Fl_Choice(160, 305, 175, 25, mgl_gettext("Widget scheme"));
		scheme_w->add("base");	scheme_w->add("gtk+");	scheme_w->add("plastic");	scheme_w->add("gleam");
		o = new Fl_Button(85, 340, 75, 25, mgl_gettext("Cancel"));	o->callback(cb_dlg_cancel,this);
		o = new Fl_Return_Button(180, 340, 75, 25, mgl_gettext("OK"));	o->callback(cb_dlg_ok,this);
		w->set_modal();	w->end();
	}
	void init()
	{
		fkind->value(styletable[0].font/4);
		fsize->value(styletable[0].size);
		font_path->value(fontname.c_str());
		help_path->value(docdir.c_str());
		auto_exec_w->value(auto_exec);
		exec_save_w->value(exec_save);
//		complete_w->value(complete_word);
		highlight_w->value(highlight);
		mouse_zoom_w->value(mouse_zoom);
		lang_w->value(lang);
		scheme_w->value(scheme);
	}
	void cb_ok()
	{
		set_style(fkind->value(),fsize->value());
		auto_exec = auto_exec_w->value();
		exec_save = exec_save_w->value();
		highlight = highlight_w->value();
		mouse_zoom = mouse_zoom_w->value();
		docdir = help_path->value();
		fontname = font_path->value();
		if(gr)	mgl_load_font(gr,fontname.c_str(),NULL);
		set_scheme_lang(scheme_w->value(),lang_w->value());
		save_pref();	hide();
	}
} prop_dlg;
//-----------------------------------------------------------------------------
void ins_fname_cb(Fl_Widget *, void *v)
{
	static std::string prev;
	ScriptWindow* e = (ScriptWindow*)v;
	char *s = fl_file_chooser(mgl_gettext("Select file name"), "DAT files (*.{dat,csv})\tHDF files (*.{hdf,h5})\tAll files (*.*)", prev.c_str(), 0);
	if(s)
	{	std::string ss=prev=s;	ss = '\''+ss+'\'';	e->editor->insert(ss.c_str());	}
}
//-----------------------------------------------------------------------------
void ins_path_cb(Fl_Widget *, void *v)
{
	static std::string prev;
	ScriptWindow* e = (ScriptWindow*)v;
	char *s = fl_dir_chooser(mgl_gettext("Select folder name"), prev.c_str(), 0);
	if(s)
	{	std::string ss=prev=s;	ss = '\''+ss+'\'';	e->editor->insert(ss.c_str());	}
}
//-----------------------------------------------------------------------------
void ins_fits_cb(Fl_Widget *, void *v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	HMGL gr = e->graph->get_graph();
	std::string ss=mgl_get_fit(gr);
	if(ss.empty())	fl_alert(mgl_gettext("There is no fitted formula!"));
	else	{	ss = '\''+ss+'\'';	e->editor->insert(ss.c_str());	}
}
//-----------------------------------------------------------------------------
void cb_filech(Fl_Widget*, void *v)
{
	if(v)
	{
		char *s = fl_file_chooser(mgl_gettext("Font file name"), "MGL font files (*.vfm*)", prop_dlg.font_path->value(), 0);
		if(s)
		{	char *e = strstr(s,".vfm");	if(e)	*e=0;
			prop_dlg.font_path->value(s);	}
	}
	else
	{
		char *s = fl_dir_chooser(mgl_gettext("Folder for help files"), prop_dlg.help_path->value(), 0);
		if(s)	prop_dlg.help_path->value(s);
	}
}
void prop_dlg_cb(Fl_Widget *, void *v)
{
	prop_dlg.gr = v?((ScriptWindow *)v)->graph->get_graph():NULL;
	prop_dlg.init();	prop_dlg.show();	
}
//-----------------------------------------------------------------------------
class ArgsDlg : public GeneralDlg
{
	Fl_Input *arg1;
	Fl_Input *arg2;
	Fl_Input *arg3;
	Fl_Input *arg4;
	Fl_Input *arg5;
	Fl_Input *arg6;
	Fl_Input *arg7;
	Fl_Input *arg8;
	Fl_Input *arg9;
	Fl_Input *arg0;
public:
	void cb_ok()
	{
		Parse->AddParam(0,arg0->value());
		Parse->AddParam(1,arg1->value());
		Parse->AddParam(2,arg2->value());
		Parse->AddParam(3,arg3->value());
		Parse->AddParam(4,arg4->value());
		Parse->AddParam(5,arg5->value());
		Parse->AddParam(6,arg6->value());
		Parse->AddParam(7,arg7->value());
		Parse->AddParam(8,arg8->value());
		Parse->AddParam(9,arg9->value());
		hide();
	}
	ArgsDlg()
	{
		w = new Fl_Double_Window(290, 320, mgl_gettext("Set script arguments"));
		arg1 = new Fl_Input(5, 20, 135, 30, mgl_gettext("String for $1"));
		arg1->align(Fl_Align(FL_ALIGN_TOP_LEFT));
		arg2 = new Fl_Input(150, 20, 135, 30, mgl_gettext("String for $2"));
		arg2->align(Fl_Align(FL_ALIGN_TOP_LEFT));
		arg3 = new Fl_Input(5, 75, 135, 30, mgl_gettext("String for $3"));
		arg3->align(Fl_Align(FL_ALIGN_TOP_LEFT));
		arg4 = new Fl_Input(150, 75, 135, 30, mgl_gettext("String for $4"));
		arg4->align(Fl_Align(FL_ALIGN_TOP_LEFT));
		arg5 = new Fl_Input(5, 130, 135, 30, mgl_gettext("String for $5"));
		arg5->align(Fl_Align(FL_ALIGN_TOP_LEFT));
		arg6 = new Fl_Input(150, 130, 135, 30, mgl_gettext("String for $6"));
		arg6->align(Fl_Align(FL_ALIGN_TOP_LEFT));
		arg7 = new Fl_Input(5, 185, 135, 30, mgl_gettext("String for $7"));
		arg7->align(Fl_Align(FL_ALIGN_TOP_LEFT));
		arg8 = new Fl_Input(150, 185, 135, 30, mgl_gettext("String for $8"));
		arg8->align(Fl_Align(FL_ALIGN_TOP_LEFT));
		arg9 = new Fl_Input(5, 240, 135, 30, mgl_gettext("String for $9"));
		arg9->align(Fl_Align(FL_ALIGN_TOP_LEFT));
		arg0 = new Fl_Input(150, 240, 135, 30, mgl_gettext("String for $0"));
		arg0->align(Fl_Align(FL_ALIGN_TOP_LEFT));
		Fl_Button* o = new Fl_Button(60, 290, 75, 25, mgl_gettext("Cancel"));
		o->callback(cb_dlg_cancel, this);
		o = new Fl_Return_Button(155, 290, 75, 25, mgl_gettext("Set"));
		o->callback(cb_dlg_ok, this);
		w->end();
	}
} args_dlg;
//-----------------------------------------------------------------------------
void args_dlg_cb(Fl_Widget *, void *v)	{	args_dlg.show();	}
//-----------------------------------------------------------------------------
