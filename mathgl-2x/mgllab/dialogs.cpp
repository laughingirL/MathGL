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
#include <FL/Fl_Output.H>
#include <FL/Fl_Select_Browser.H>
#include <FL/Fl_Toggle_Button.H>
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
{0}};
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
	prop_dlg.show();	
}
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
		w->set_modal();	w->end();
	}
} args_dlg;
//-----------------------------------------------------------------------------
void args_dlg_cb(Fl_Widget *, void *)	{	args_dlg.show();	}
//-----------------------------------------------------------------------------
void cb_calc_key(Fl_Widget *, void *v);
void cb_calc_ins(Fl_Widget *, void *);
void cb_calc_prev(Fl_Widget *, void *);
void cb_calc_edit(Fl_Widget *, void *);
void cb_calc_kind(Fl_Widget *, void *);
void cb_calc_func(Fl_Widget *, void *);
class CalcDlg : public GeneralDlg
{
public:
	Fl_Input *edit;
	Fl_Output *output;
	Fl_Browser *prev;
	Fl_Choice *kind;
	Fl_Choice *func;
	ScriptWindow *e;
	CalcDlg()
	{
		Fl_Button *o;
		w = new Fl_Double_Window(275, 275, mgl_gettext("Calculator"));
		edit = new Fl_Input(5, 5, 240, 25);	//edit->callback(cb_calc_edit);
		o = new Fl_Return_Button(245, 5, 25, 25, "@>");	o->callback(cb_calc_edit);
		output = new Fl_Output(55, 35, 120, 25, mgl_gettext("Result"));
		o = new Fl_Button(180, 35, 90, 25, mgl_gettext("@-> to script"));	o->callback(cb_calc_ins);
		prev = new Fl_Select_Browser(5, 80, 265, 70, mgl_gettext("Previous expressions"));
		prev->align(Fl_Align(FL_ALIGN_TOP_LEFT));	prev->callback(cb_calc_prev);
		o = new Fl_Button(5, 155, 25, 25, mgl_gettext("7"));	o->callback(cb_calc_key,o);
		o = new Fl_Button(35, 155, 25, 25, mgl_gettext("8"));	o->callback(cb_calc_key,o);
		o = new Fl_Button(65, 155, 25, 25, mgl_gettext("9"));	o->callback(cb_calc_key,o);
		o = new Fl_Button(95, 155, 25, 25, mgl_gettext("+"));	o->callback(cb_calc_key,o);
		o = new Fl_Button(125, 155, 25, 25, mgl_gettext("pi"));	o->callback(cb_calc_key,o);
		o = new Fl_Button(5, 185, 25, 25, mgl_gettext("4"));	o->callback(cb_calc_key,o);
		o = new Fl_Button(35, 185, 25, 25, mgl_gettext("5"));	o->callback(cb_calc_key,o);
		o = new Fl_Button(65, 185, 25, 25, mgl_gettext("6"));	o->callback(cb_calc_key,o);
		o = new Fl_Button(95, 185, 25, 25, mgl_gettext("-"));	o->callback(cb_calc_key,o);
		o = new Fl_Button(125, 185, 25, 25, mgl_gettext("^"));	o->callback(cb_calc_key,o);
		o = new Fl_Button(5, 215, 25, 25, mgl_gettext("1"));	o->callback(cb_calc_key,o);
		o = new Fl_Button(35, 215, 25, 25, mgl_gettext("2"));	o->callback(cb_calc_key,o);
		o = new Fl_Button(65, 215, 25, 25, mgl_gettext("3"));	o->callback(cb_calc_key,o);
		o = new Fl_Button(95, 215, 25, 25, mgl_gettext("*"));	o->callback(cb_calc_key,o);
		o = new Fl_Button(125, 215, 25, 25, mgl_gettext("("));	o->callback(cb_calc_key,o);
		o = new Fl_Button(5, 245, 25, 25, mgl_gettext("0"));	o->callback(cb_calc_key,o);
		o = new Fl_Button(35, 245, 25, 25, mgl_gettext("."));	o->callback(cb_calc_key,o);
		o = new Fl_Button(65, 245, 25, 25, mgl_gettext("E"));	o->callback(cb_calc_key,o);
		o = new Fl_Button(95, 245, 25, 25, mgl_gettext("/"));	o->callback(cb_calc_key,o);
		o = new Fl_Button(125, 245, 25, 25, mgl_gettext(")"));	o->callback(cb_calc_key,o);

		Fl_Group* g = new Fl_Group(155, 174, 115, 95, mgl_gettext("Function"));	g->box(FL_DOWN_BOX);
		kind = new Fl_Choice(160, 179, 105, 25);	kind->down_box(FL_BORDER_BOX);	kind->callback(cb_calc_kind);
		kind->add("Basic");	kind->add("Exp and log");	kind->add("Trigonometric");	kind->add("Hyperbolic");
		kind->add("Bessel");	kind->add("Elliptic");	kind->add("Jacobi");	 kind->add("Airy and Gamma");
		kind->add("Exp-integrals"); kind->add("Special");	kind->value(0);
		
		func = new Fl_Choice(160, 209, 105, 25);	func->down_box(FL_BORDER_BOX);
		o = new Fl_Button(160, 239, 105, 25, mgl_gettext("Put function"));	o->callback(cb_calc_func);
		g->end();	w->end();
	}
	void eval()
	{
		const char *eq = edit->value();
		mglData d = Parse->Calc(eq);
		char res[64];	snprintf(res,63,"%g",d.a[0]);
		output->value(res);	result = res;
		char *buf = new char[strlen(res)+strlen(eq)+3];
		sprintf(buf,"%s\t%s",eq,res);
		prev->insert(0,buf);	delete []buf;
	}
	void set_kind()
	{
		int val = kind->value();	func->clear();
		switch(val)
		{
		case 0:	// basic
			func->add("abs()");		func->add("sign()");	func->add("step()");	func->add("sqrt()");
			func->add("mod(,)");	func->add("arg(,)");	break;
		case 1:	// exp and logarithms
			func->add("exp()");		func->add("pow(,)");	func->add("ln()");		func->add("lg()");
			func->add("log(,)");	break;
		case 2:	// trigonometric
			func->add("sin()");		func->add("cos()");		func->add("tan()");		func->add("sinc()");
			func->add("asin()");	func->add("acos()");	func->add("atan()");	break;
		case 3:	// hyperbolic
			func->add("sinh()");	func->add("cosh()");	func->add("tanh()");	func->add("asinh()");
			func->add("acosh()");	func->add("atanh()");	break;
		case 4:	// bessel
			func->add("bessel_j(,)");	func->add("bessel_y(,)");	func->add("bessel_i(,)");	func->add("bessel_k(,)");	break;
		case 5:	// elliptic
			func->add("elliptic_e(,)");	func->add("elliptic_f(,)");	func->add("elliptic_ec()");	func->add("elliptic_kc()");	break;
		case 6:	// jacobi
			func->add("sn(,)");		func->add("cn(,)");		func->add("dn(,)");		func->add("sc(,)");
			func->add("dc(,)");		func->add("nc(,)");		func->add("cs(,)");		func->add("ds(,)");
			func->add("ns(,)");		func->add("sd(,)");		func->add("cd(,)");		func->add("nd(,)");	break;
		case 7:	// airy and gamma
			func->add("airy_ai()");	func->add("airy_bi()");	func->add("airy_dai()");func->add("airy_dbi()");
			func->add("gamma()");	func->add("psi()");		func->add("beta(,)");	break;
		case 8:	// exp integrals
			func->add("ci()");		func->add("si()");		func->add("ei()");		func->add("e1()");
			func->add("e2()");		func->add("ei3()");	break;
		case 9:	// special
			func->add("erf()");		func->add("z()");		func->add("legendre(,)");	func->add("dilog()");
			func->add("eta()");		func->add("zeta()");	func->add("w0()");		func->add("w1()");	break;
		}
//		func->value(0);
	}
} calc_dlg;
//-----------------------------------------------------------------------------
void cb_calc_key(Fl_Widget *, void *v)
{	Fl_Button *o=(Fl_Button *)v;	calc_dlg.edit->insert(o->label());	}
void cb_calc_ins(Fl_Widget *, void *)
{	if(calc_dlg.e)	calc_dlg.e->editor->insert(calc_dlg.output->value());	}
void cb_calc_prev(Fl_Widget *, void *)
{
	const char *s = calc_dlg.prev->text(calc_dlg.prev->value());
	if(s && *s)
	{
		std::string ss(s);	size_t l=ss.length();
		for(size_t i=0;i<l;i++)	if(ss[i]=='\t')	ss[i]=0;
		calc_dlg.edit->value(ss.c_str());
	}
}
void cb_calc_edit(Fl_Widget *, void *)	{	calc_dlg.eval();	}
void cb_calc_kind(Fl_Widget *, void *)	{	calc_dlg.set_kind();	}
void cb_calc_func(Fl_Widget *, void *)
{	const char *s = calc_dlg.func->text();
	if(s && *s)	calc_dlg.edit->insert(s);	}
//-----------------------------------------------------------------------------
void calc_dlg_cb(Fl_Widget *, void *v)
{	calc_dlg.e = (ScriptWindow *)v;	calc_dlg.show();	}
//-----------------------------------------------------------------------------
void cb_option_change(Fl_Widget *, void *);
class OptionDlg : public GeneralDlg
{
	Fl_Input *x1, *x2, *y1, *y2, *z1, *z2, *c1, *c2;
	Fl_Input *val, *meshnum, *size;
	Fl_Choice *cut, *light;
	Fl_Input *alpha, *amb, *dif, *legend;
public:
	ScriptWindow *e;
	Fl_Input *ext;
	OptionDlg()
	{
		e=NULL;	ext=NULL;
		Fl_Button *o;
		w = new Fl_Double_Window(570, 185, mgl_gettext("Add command option(s)"));
		x1 = new Fl_Input(65, 5, 85, 25, mgl_gettext("X-range"));
		x1->tooltip(mgl_gettext("Minimal value of X for cutting or for coordinate filling"));
		x2 = new Fl_Input(160, 5, 85, 25, mgl_gettext("-"));
		x2->tooltip(mgl_gettext("Maximal value of X for cutting or for coordinate filling"));
		y1 = new Fl_Input(65, 35, 85, 25, mgl_gettext("Y-range"));
		y1->tooltip(mgl_gettext("Minimal value of Y for cutting or for coordinate filling"));
		y2 = new Fl_Input(160, 35, 85, 25, mgl_gettext("-"));
		y2->tooltip(mgl_gettext("Maximal value of Y for cutting or for coordinate filling"));
		z1 = new Fl_Input(65, 65, 85, 25, mgl_gettext("Z-range"));
		z1->tooltip(mgl_gettext("Minimal value of Z for cutting or for coordinate filling"));
		z2 = new Fl_Input(160, 65, 85, 25, mgl_gettext("-"));
		z2->tooltip(mgl_gettext("Maximal value of Z for cutting or for coordinate filling"));
		c1 = new Fl_Input(65, 95, 85, 25, mgl_gettext("C-range"));	c1->deactivate();
		c1->tooltip(mgl_gettext("Low border for determining color or alpha"));
		c2 = new Fl_Input(160, 95, 85, 25, mgl_gettext("-"));		c2->deactivate();
		c2->tooltip(mgl_gettext("Upper border for determining color or alpha"));
		val = new Fl_Input(325, 5, 85, 25, mgl_gettext("Value"));	val->labelfont(1);
		meshnum = new Fl_Input(325, 35, 85, 25, mgl_gettext("MeshNum"));
		meshnum->tooltip(mgl_gettext("Approximate number of mesh lines in plot"));
		size = new Fl_Input(325, 65, 85, 25, mgl_gettext("Size"));
		size->tooltip(mgl_gettext("Set size for text, marks and others"));
		cut = new Fl_Choice(325, 95, 85, 25, mgl_gettext("Cutting"));
		cut->add(mgl_gettext("default"));	cut->add("on");	cut->add("off");
		cut->tooltip(mgl_gettext("Set cutting off/on for particular plot"));
		alpha = new Fl_Input(480, 5, 85, 25, mgl_gettext("Alpha"));
		alpha->tooltip(mgl_gettext("Alpha value (transparency) of surface or cloud"));
		amb = new Fl_Input(480, 35, 85, 25, mgl_gettext("Ambient"));
		amb->tooltip(mgl_gettext("Own brightness of the surface"));
		dif = new Fl_Input(480, 65, 85, 25, mgl_gettext("Diffuse"));
		dif->tooltip(mgl_gettext("Intensity of diffuse light"));
		light = new Fl_Choice(480, 95, 85, 25, mgl_gettext("Light"));
		light->add(mgl_gettext("default"));	light->add("on");	light->add("off");
		cut->tooltip(mgl_gettext("Set lighting off/on for particular plot"));
		legend = new Fl_Input(65, 125, 500, 25, mgl_gettext("Legend"));
		cut->tooltip(mgl_gettext("Add legend entry for the plot"));
		o = new Fl_Button(385, 155, 85, 25, mgl_gettext("Cancel"));	o->callback(cb_dlg_cancel,this);
		o = new Fl_Return_Button(480, 155, 85, 25, mgl_gettext("OK"));	o->callback(cb_dlg_ok,this);
		w->set_modal();	w->end();
	}
	void cb_ok()
	{
		int v;	const char *s1, *s2;	result.clear();
		s1=val->value();	if(s1 && *s1)	result = result+";value "+s1;
		s1=x1->value();	s2=x2->value();	if(s1 && *s1 && s2 && *s2)	result = result+";xrange "+s1+' '+s2;
		s1=y1->value();	s2=y2->value();	if(s1 && *s1 && s2 && *s2)	result = result+";yrange "+s1+' '+s2;
		s1=z1->value();	s2=z2->value();	if(s1 && *s1 && s2 && *s2)	result = result+";zrange "+s1+' '+s2;
//		s1=c1->value();	s2=c2->value();	if(s1 && *s1 && s2 && *s2)	result = result+";crange "+s1+' '+s2;
		s1=legend->value();	if(s1 && *s1)	result = result+";legend '"+s1+"'";
		s1=size->value();	if(s1 && *s1)	result = result+";size "+s1;
		s1=alpha->value();	if(s1 && *s1)	result = result+";alpha "+s1;
		s1=amb->value();	if(s1 && *s1)	result = result+";ambient "+s1;
		s1=dif->value();	if(s1 && *s1)	result = result+";diffuse "+s1;
		s1=meshnum->value();if(s1 && *s1)	result = result+";meshnum "+s1;
		v=cut->value();		if(v==1 || v==2)	result = result+";cut "+cut->text();
		v=light->value();	if(v==1 || v==2)	result = result+";light "+light->text();
		if(e)	e->editor->insert(result.c_str());
		if(ext)	ext->value(result.c_str());
		hide();
	}
} option_dlg;
//-----------------------------------------------------------------------------
void option_dlg_cb(Fl_Widget *, void *v)
{	option_dlg.ext=NULL;	option_dlg.e=(ScriptWindow *)v;	option_dlg.show();	}
//-----------------------------------------------------------------------------
void option_in_cb(Fl_Widget *, void *v)
{	option_dlg.ext=(Fl_Input*)v;	option_dlg.e=NULL;	option_dlg.show();	}
//-----------------------------------------------------------------------------
class DirSelDlg : public GeneralDlg
{
	Fl_Choice *dir;
public:
	ScriptWindow *e;
	Fl_Input *ext;
	DirSelDlg()
	{
		Fl_Button *o;
		w = new Fl_Double_Window(220, 85, mgl_gettext("Select direction"));
		dir = new Fl_Choice(80, 10, 135, 25, mgl_gettext("Direction"));
		dir->add("xyz");	dir->add("x");	dir->add("y");	dir->add("z");
		dir->add("xy");	dir->add("xz");	dir->add("yz");
		o = new Fl_Button(55, 50, 75, 25, mgl_gettext("Cancel"));	o->callback(cb_dlg_cancel,this);
		o = new Fl_Return_Button(140, 50, 75, 25, mgl_gettext("OK"));	o->callback(cb_dlg_ok,this);
		w->set_modal();	w->end();
	}
	void cb_ok()
	{
		const char *s = dir->text();
		if(s && *s)	result = std::string("'")+s+"'";
		if(e)	e->editor->insert(result.c_str());
		if(ext)	ext->value(result.c_str());
		hide();
	}
} dirsel_dlg;
//-----------------------------------------------------------------------------
void dirsel_dlg_cb(Fl_Widget *, void *v)
{	dirsel_dlg.ext=NULL;	dirsel_dlg.e=(ScriptWindow *)v;	dirsel_dlg.show();	}
//-----------------------------------------------------------------------------
void dirsel_in_cb(Fl_Widget *, void *v)
{	dirsel_dlg.ext=(Fl_Input*)v;	dirsel_dlg.e=NULL;	dirsel_dlg.show();	}
//-----------------------------------------------------------------------------
//const char *arr = "_AKVITSDOX";
Fl_Menu_Item arrows[] = {
	{("'_' none")},		//_
	{("'A' arrow")},	//A
	{("'K' size")},		//K
	{("'V' back arrow")},//V
	{("'I' stop")},		//I
	{("'T' triangle")},	//T
	{("'S' square")},	//S
	{("'D' rhomb")},	//D
	{("'O' circle")},	//O
	{("'X' cross")},	//X
	{0}};
//-----------------------------------------------------------------------------
//const char *stl = "-|j;i=: d";
Fl_Menu_Item dashing[] = {
	{("'-' solid")},	//-
	{("'|' dash")},		//|
	{("'j' dash dot")},	//j
	{("';' small dash")},	//;
	{("'i' small dash dot")},	//i
	{("'=' very small dash")},	//=
	{("':' dots")},		//:
	{("' ' none")},		//
	{("manual")},		// d
	{0}};
//-----------------------------------------------------------------------------
//const char *mrk = "*o+xsd.^v<>";
Fl_Menu_Item markers[] = {
	{("none")},				//
	{("'*' star")},			//*
	{("'o' circle")},		//o
	{("'+' cross")},		//+
	{("'x' skew cross")},	//x
	{("'s' square")},		//s
	{("'d' rhomb")},		//d
	{("'.' point")},		//.
	{("'^' triangle up")},	//^
	{("'v' triangle down")},//v
	{("'<' triangle left")},//<
	{("'>' triangle right")},//>
	{0}};
//-----------------------------------------------------------------------------
//const char *msk = " -+=;joOsS~<>dD*^M";
Fl_Menu_Item masks[] = {
	{("none")},			//
	{("'-' lines")},		//-
	{("'+' cross")},		//+
	{("'=' double lines")},	//=
	{("';' dash")},			//;
	{("'j' dash dot")},		//j
	{("'o' circle")},		//o
	{("'O' solid circle")},	//O
	{("'s' square")},		//s
	{("'S' solid square")},	//S
	{("'~' tilde")},		//~
	{("'<' triangle left")},//<
	{("'>' triangle right")},//>
	{("'d' rhomb")},		//d
	{("'D' solid rhomb")},	//D
	{("'*' stars")},		//*
	{("'^' hats")},			//^
	{("manual")},			//M
	{0}};
//-----------------------------------------------------------------------------
std::string get_color(Fl_Choice *c, Fl_Spinner *s)
{
	std::string res;
	const char *cols = " wbgrcmylenuqphkWBGRCMYLENUQPH";
	int cv = c?c->value():0, sv = s?s->value():-1;
	if(cv>0 && cv<strlen(cols))
	{
		if(sv>0 && sv<10 && sv!=5)
		{
			char buf[16];	snprintf(buf,16,"{%c%d}",cols[cv],sv);
			res = buf;
		}
		else	res = cols[cv];
	}
	return res;
}
//-----------------------------------------------------------------------------
void cb_style_upd(Fl_Widget *, void *);
class StyleDlg : public GeneralDlg
{
	Fl_Choice *arr1, *dash, *arr2;
	Fl_Choice *col;
	Fl_Spinner *satur;
	Fl_Choice *mark;
	Fl_Check_Button *solid, *user;
	Fl_Spinner *width;
	Fl_Button *dash_m[16];
	
	Fl_Choice *c[7];
	Fl_Spinner *s[7];
	Fl_Choice *axial, *contt, *mask, *angle;
	Fl_Spinner *msize;
	Fl_Input *alpha;
	Fl_Button *mask_m[64];
	Fl_Check_Button *wire, *sharp;
	
	Fl_Check_Button *bold, *ital, *twire, *uline, *oline, *tsch;
	Fl_Choice *tcol, *align, *vert;
	
	Fl_Group *gline, *gsurf, *gfont, *gmask;
	Fl_Output *res;
	Fl_MathGL *gr;
	std::string script;
public:
	ScriptWindow *e;
	Fl_Input *ext;
	StyleDlg()
	{
		Fl_Group *g;	Fl_Button *o;
		w = new Fl_Double_Window(380, 380, mgl_gettext("Select plot style"));
		Fl_Tabs* tt = new Fl_Tabs(0, 5, 375, 260);

		gline = new Fl_Group(0, 30, 375, 235, mgl_gettext("Line style"));	//gline->hide();
			arr1 = new Fl_Choice(5, 50, 110, 25, mgl_gettext("Arrow at start"));
			arr1->align(Fl_Align(FL_ALIGN_TOP_LEFT));	arr1->copy(arrows);		arr1->callback(cb_style_upd);
			dash = new Fl_Choice(125, 50, 110, 25, mgl_gettext("Dashing"));
			dash->align(Fl_Align(FL_ALIGN_TOP_LEFT));	dash->copy(dashing);	dash->callback(cb_style_upd);
			arr2 = new Fl_Choice(245, 50, 110, 25, mgl_gettext("Arrow at end"));
			arr2->align(Fl_Align(FL_ALIGN_TOP_LEFT));	arr2->copy(arrows);		arr2->callback(cb_style_upd);
			col = new Fl_Choice(125, 80, 110, 25, mgl_gettext("Color"));
			col->copy(colors);		col->callback(cb_style_upd);
			satur = new Fl_Spinner(285, 80, 40, 25, mgl_gettext("Satur."));
			satur->range(1,9);	satur->value(5);	col->callback(cb_style_upd);
			mark = new Fl_Choice(125, 110, 110, 25, mgl_gettext("Marks"));
			mark->copy(markers);	mark->callback(cb_style_upd);
			solid = new Fl_Check_Button(240, 110, 55, 25, mgl_gettext("solid"));	solid->callback(cb_style_upd);
			user = new Fl_Check_Button(300, 110, 55, 25, mgl_gettext("user"));		user->callback(cb_style_upd);
			width = new Fl_Spinner(125, 140, 110, 25, mgl_gettext("Width"));
			width->range(1,9);	width->value(1);	width->callback(cb_style_upd);
			for(int i=0;i<16;i++)
			{
				dash_m[i] = new Fl_Toggle_Button(10+20*i, 185, 20, 20);
				dash_m[i]->callback(cb_style_upd);
			}
			dash_m[0]->label(mgl_gettext("Manual dashing"));
			dash_m[0]->align(Fl_Align(FL_ALIGN_TOP_LEFT));
			gline->end();
		gsurf = new Fl_Group(0, 30, 375, 235, mgl_gettext("Color scheme"));	gsurf->hide();
			for(int i=0;i<7;i++)
			{
				c[i] = new Fl_Choice(5, 50+30*i, 95, 25);	c[i]->copy(colors);
				c[i]->callback(cb_style_upd);
				s[i] = new Fl_Spinner(105, 50+30*i, 40, 25);
				s[i]->range(1,9);	s[i]->value(5);
				s[i]->callback(cb_style_upd);
			}
			c[0]->align(FL_ALIGN_TOP_LEFT);	c[0]->label(mgl_gettext("Color order"));
			s[0]->align(FL_ALIGN_TOP);		s[0]->label(mgl_gettext("Satur."));
			axial = new Fl_Choice(275, 50, 95, 25, mgl_gettext("Axial direction"));	axial->callback(cb_style_upd);
			axial->add("none");	axial->add("x");	axial->add("y");	axial->add("z");
			contt = new Fl_Choice(275, 80, 95, 25, mgl_gettext("Text on contours"));
			contt->add("none");	contt->add("under");	contt->add("above");	contt->callback(cb_style_upd);
			mask = new Fl_Choice(275, 110, 95, 25, mgl_gettext("Mask for bitmap"));
			mask->copy(masks);	mask->callback(cb_style_upd);
			angle = new Fl_Choice(275, 140, 95, 25, mgl_gettext("Mask rotation"));	angle->callback(cb_style_upd);
			angle->add("none");	angle->add("+45");	angle->add("perp");	angle->add("-45");
			msize = new Fl_Spinner(275, 170, 95, 25, mgl_gettext("Mask size"));
			msize->range(1,9);	msize->value(1);	msize->callback(cb_style_upd);
			alpha = new Fl_Input(275, 200, 95, 25, mgl_gettext("Transparency"));	alpha->callback(cb_style_upd);
			wire = new Fl_Check_Button(155, 230, 115, 25, mgl_gettext("Wire or mesh"));	wire->callback(cb_style_upd);
			sharp = new Fl_Check_Button(275, 230, 95, 25, mgl_gettext("Sharp"));		sharp->callback(cb_style_upd);
			gsurf->end();
		gfont = new Fl_Group(0, 30, 375, 235, mgl_gettext("Text style"));	gfont->hide();
			bold = new Fl_Check_Button(5, 40, 150, 25, mgl_gettext("Bold style"));	bold->callback(cb_style_upd);
			ital = new Fl_Check_Button(5, 65, 150, 25, mgl_gettext("Italic style"));ital->callback(cb_style_upd);
			twire = new Fl_Check_Button(5, 90, 150, 25, mgl_gettext("Wire style"));	twire->callback(cb_style_upd);
			uline = new Fl_Check_Button(5, 115, 150, 25, mgl_gettext("Underline"));	uline->callback(cb_style_upd);
			oline = new Fl_Check_Button(5, 140, 150, 25, mgl_gettext("Overline"));	oline->callback(cb_style_upd);
//			tsch = new Fl_Check_Button(5, 165, 150, 25, mgl_gettext("Use color scheme"));	tsch->callback(cb_style_upd);
			tcol = new Fl_Choice(260, 45, 95, 25, mgl_gettext("Text color"));
			tcol->copy(colors);	tcol->callback(cb_style_upd);
			align = new Fl_Choice(260, 80, 95, 25, mgl_gettext("Text align"));	align->callback(cb_style_upd);
			align->add("left");	align->add("center");	align->add("right");	align->value(1);
			vert = new Fl_Choice(260, 115, 95, 25, mgl_gettext("Vertical align"));	vert->callback(cb_style_upd);
			vert->add("default");	vert->add("center");	vert->add("under");	vert->value(0);
			gfont->end();
		gmask = new Fl_Group(0, 30, 375, 235, mgl_gettext("Manual mask"));	gmask->hide();
			for(int i=0;i<8;i++)	for(int j=0;j<8;j++)
			{
				mask_m[i+8*j] = new Fl_Toggle_Button(15+20*i, 45+20*(7-j), 20, 20);
				mask_m[i+8*j]->callback(cb_style_upd);
			}
			gmask->end();
		tt->end();

		res = new Fl_Output(5, 285, 370, 25, mgl_gettext("Resulting string"));
		res->align(FL_ALIGN_TOP_LEFT);
		o = new Fl_Button(300, 320, 75, 25, mgl_gettext("Cancel"));	o->callback(cb_dlg_cancel,this);
		o = new Fl_Return_Button(300, 350, 75, 25, mgl_gettext("OK"));	o->callback(cb_dlg_ok,this);

		gr = new Fl_MathGL(5, 315, 285, 65);	gr->box(FL_ENGRAVED_BOX);
		mgl_set_size(gr->get_graph(),285,65);
		w->set_modal();	w->end();
	}
	void stl_line()
	{
		result = get_color(col,satur);
		char dsh = dash->text()[1];
		char a1 = arr1->text()[1], a2 = arr2->text()[1];
		const char *s = mark->text();
		if(*s=='\'')
		{
			if(user->value())	result += '&';
			if(solid->value())	result += '#';
			result += s[1];
		}
		int v = width->value();
		if(v>1)	result += char(v+'0');
		if(a1!='_')	result = result + a1 + a2;
		else if(a2!='_')	result += a2;
		if(dsh=='a')
		{
			unsigned long mask=0;
			for(int i=0;i<16;i++)	if(dash_m[i]->value())	mask += 1<<i;
			char buf[128];	snprintf(buf,128,"{d%lX}",mask);
			result += buf;
		}
		else if(dsh!='-')	result += dsh;
		result = '\''+result+'\'';
		script = "new a 5:plot a "+result+";size 10";
	}
	void stl_surf()
	{
		int v;
		for(int i=0;i<7;i++)	result += get_color(c[i],s[i]);
		v = contt->value();	const char *tt="Tt";
		if(v>0 && v<3)	result += tt[v-1];
		const char *m = mask->text();
		if(*m=='\'')
		{
			v = angle->value();	const char *rr="/I\\";
			if(v>0 && v<4)	result += rr[v-1];
			v = msize->value();	if(v>1)	result += char(v+'0');
			result += m[1];
		}
		if(*m=='m')
		{
			v = angle->value();	const char *rr="/I\\";
			if(v>0 && v<4)	result += rr[v-1];
			v = msize->value();	if(v>1)	result += char(v+'0');
			uint64_t mask=0;
			for(int i=0;i<64;i++)	if(mask_m[i]->value())	mask += uint64_t(1)<<i;
			char buf[128];	snprintf(buf,128,"{s%llX}",mask);
			result += buf;
		}
		if(wire->value())	result += '#';
		if(sharp->value())	result += '|';
		v = atoi(alpha->value());
		if(v>0 && v<10)	result = result + "{A"+char(v+'0')+'}';
		v = axial->value();	const char *ax="xyz";
		if(v>0 && v<4)	result = result+':'+ax[v-1];
		result = '\''+result+'\'';
		script = "fsurf 'x' "+result;
	}
	void stl_font()
	{
		const char *a="LCR";
		result = get_color(tcol,NULL)+':'+a[align->value()];
		if(bold->value())	result += 'b';
		if(ital->value())	result += 'i';
		if(twire->value())	result += 'w';
		if(uline->value())	result += 'u';
		if(oline->value())	result += 'o';
		if(vert->value()==1)	result += 'V';
		if(vert->value()==2)	result += 'T';
		result = '\''+result+'\'';
		script = "text 0 0 'Font test' "+result+";size -8";
	}
	void update()
	{
		static int busy=0;
		if(!busy)	{	busy=1;
			result.clear();
			if(gline->visible())		stl_line();
			else if(gfont->visible())	stl_font();
			else	stl_surf();
			res->value(result.c_str());
			mglParse pr;
			script = "clf:subplot 1 1 0 '':box:"+script;
			mgl_parse_text(gr->get_graph(), pr.Self(), script.c_str());
			gr->update();
		busy=0;	}
	}
	void cb_ok()
	{
		if(e)	e->editor->insert(result.c_str());
		if(ext)	ext->value(result.c_str());
		hide();
	}
} style_dlg;
//-----------------------------------------------------------------------------
void cb_style_upd(Fl_Widget *, void *)	{	style_dlg.update();	}
//-----------------------------------------------------------------------------
void style_dlg_cb(Fl_Widget *, void *v)
{	style_dlg.ext=NULL;	style_dlg.e=(ScriptWindow *)v;	style_dlg.show();	}
//-----------------------------------------------------------------------------
void style_in_cb(Fl_Widget *, void *v)
{	style_dlg.ext=(Fl_Input*)v;	style_dlg.e=NULL;	style_dlg.show();	}
//-----------------------------------------------------------------------------
void cb_datsel_upd(Fl_Widget *, void *);
class DatSelDlg : public GeneralDlg
{
	Fl_Choice *name;
	Fl_Choice *oper;
	Fl_Choice *dir;
	Fl_Spinner *x1, *x2, *y1, *y2, *z1, *z2;
	Fl_Output *res;
	Fl_Input *clmn;
	Fl_Check_Button *ax, *ay, *az;
public:
	ScriptWindow *e;
	Fl_Input *ext;
	DatSelDlg()
	{
		Fl_Button *o;
		w = new Fl_Double_Window(355, 255, mgl_gettext("Select data argument"));
		name = new Fl_Choice(105, 5, 145, 25, mgl_gettext("Data name"));
		name->callback(cb_datsel_upd);
		x1 = new Fl_Spinner(105, 35, 60, 25, mgl_gettext("X-slice from"));
		x2 = new Fl_Spinner(190, 35, 60, 25, mgl_gettext("to"));
		ax = new Fl_Check_Button(260, 35, 90, 25, mgl_gettext("all"));	ax->callback(cb_datsel_upd);
		y1 = new Fl_Spinner(105, 65, 60, 25, mgl_gettext("Y-slice from"));
		y2 = new Fl_Spinner(190, 65, 60, 25, mgl_gettext("to"));
		ay = new Fl_Check_Button(260, 65, 90, 25, mgl_gettext("all"));	ay->callback(cb_datsel_upd);
		z1 = new Fl_Spinner(105, 95, 60, 25, mgl_gettext("Z-slice from"));
		z2 = new Fl_Spinner(190, 95, 60, 25, mgl_gettext("to"));
		az = new Fl_Check_Button(260, 95, 90, 25, mgl_gettext("all"));	az->callback(cb_datsel_upd);
		clmn = new Fl_Input(105, 125, 245, 25, mgl_gettext("Column expr"));
		oper = new Fl_Choice(105, 155, 130, 25, mgl_gettext("Operation"));
		oper->add("none");	oper->add("max");	oper->add("min");	oper->add("sum");
		oper->add("trace");	oper->add("pulse");	oper->value(0);
		dir = new Fl_Choice(285, 155, 65, 25, mgl_gettext("along"));
		dir->add("none");	dir->add("x");	dir->add("y");	dir->add("z");
		dir->add("xy");		dir->add("xz");	dir->add("yz");
		res = new Fl_Output(60, 190, 290, 25, mgl_gettext("Result"));
		o = new Fl_Button(190, 225, 75, 25, mgl_gettext("Cancel"));	o->callback(cb_dlg_cancel,this);
		o = new Fl_Return_Button(275, 225, 75, 25, mgl_gettext("OK"));	o->callback(cb_dlg_ok,this);
		w->set_modal();	w->end();
	}
	void cb_ok()
	{
		result.clear();
		if(name->value()<0)
		{	fl_alert(mgl_gettext("You need to select data array"));	return;	}
		std::string data = name->text();
		const char *eq = clmn->value();
		if(eq && *eq)	data = data+"('"+eq+"')";
		else
		{
		}
		if(oper->value()>0)
		{
			result = result+'{'+oper->text()+' '+data;
			if(dir->value()>0)	result = result+" '"+dir->text()+"'}";
			else	result += '}';
		}
		else	result = data;
		if(e)	e->editor->insert(result.c_str());
		if(ext)	ext->value(result.c_str());
		hide();
	}
	void init()
	{}
	void update()
	{}
} datsel_dlg;
//-----------------------------------------------------------------------------
void cb_datsel_upd(Fl_Widget *, void *)	{	datsel_dlg.update();	}
//-----------------------------------------------------------------------------
void datsel_dlg_cb(Fl_Widget *, void *v)
{	datsel_dlg.ext=NULL;	datsel_dlg.e=(ScriptWindow *)v;	datsel_dlg.show();	}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
