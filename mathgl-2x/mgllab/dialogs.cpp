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
#include <string.h>
#include <FL/Fl_Spinner.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Select_Browser.H>
#include <FL/Fl_Toggle_Button.H>
//-----------------------------------------------------------------------------
const char *cols = " wbgrcmylenuqphkWBGRCMYLENUQPH";
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
		help_path->align(FL_ALIGN_TOP_LEFT);
		o = new Fl_Button(310, 65, 25, 25, mgl_gettext("..."));	o->callback(cb_filech, 0);
		font_path = new Fl_File_Input(5, 110, 305, 35, mgl_gettext("Path for MathGL font (without extension)"));
		font_path->align(FL_ALIGN_TOP_LEFT);
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
		if(e->graph->get_graph())
			mgl_load_font(e->graph->get_graph(),fontname.c_str(),NULL);
		set_scheme_lang(scheme_w->value(),lang_w->value());
		example_cb(NULL, e);	e->graph->parent()->show();
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
{	prop_dlg.e = (ScriptWindow *)v;	prop_dlg.show();	}
//-----------------------------------------------------------------------------
void ins_fname_cb(Fl_Widget *, void *v)
{
	static std::string prev;
	ScriptWindow* e = (ScriptWindow*)v;
	char *s = fl_file_chooser(mgl_gettext("Select file name"), "DAT files (*.{dat,csv})\tHDF files (*.{hdf,h5})\tAll files (*.*)", prev.c_str(), 0);
	if(s)
	{
		std::string ss=prev=s;	ss = '\''+ss+'\'';
		if(e)	e->editor->insert(ss.c_str());
		else	cb_args_set(ss.c_str());
	}
}
//-----------------------------------------------------------------------------
void ins_path_cb(Fl_Widget *, void *v)
{
	static std::string prev;
	ScriptWindow* e = (ScriptWindow*)v;
	char *s = fl_dir_chooser(mgl_gettext("Select folder name"), prev.c_str(), 0);
	if(s)
	{
		std::string ss=prev=s;	ss = '\''+ss+'\'';
		if(e)	e->editor->insert(ss.c_str());
		else	cb_args_set(ss.c_str());
	}
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
	Fl_Input *arg[10];
public:
	void cb_ok()
	{
		for(int i=0;i<10;i++)	Parse->AddParam(i,arg[i]->value());
		hide();
	}
	ArgsDlg()
	{
		w = new Fl_Double_Window(290, 320, mgl_gettext("Set script arguments"));
		arg[1] = new Fl_Input(5, 20, 135, 30, mgl_gettext("String for $1"));
		arg[1]->align(FL_ALIGN_TOP_LEFT);
		arg[2] = new Fl_Input(150, 20, 135, 30, mgl_gettext("String for $2"));
		arg[2]->align(FL_ALIGN_TOP_LEFT);
		arg[3] = new Fl_Input(5, 75, 135, 30, mgl_gettext("String for $3"));
		arg[3]->align(FL_ALIGN_TOP_LEFT);
		arg[4] = new Fl_Input(150, 75, 135, 30, mgl_gettext("String for $4"));
		arg[4]->align(FL_ALIGN_TOP_LEFT);
		arg[5] = new Fl_Input(5, 130, 135, 30, mgl_gettext("String for $5"));
		arg[5]->align(FL_ALIGN_TOP_LEFT);
		arg[6] = new Fl_Input(150, 130, 135, 30, mgl_gettext("String for $6"));
		arg[6]->align(FL_ALIGN_TOP_LEFT);
		arg[7] = new Fl_Input(5, 185, 135, 30, mgl_gettext("String for $7"));
		arg[7]->align(FL_ALIGN_TOP_LEFT);
		arg[8] = new Fl_Input(150, 185, 135, 30, mgl_gettext("String for $8"));
		arg[8]->align(FL_ALIGN_TOP_LEFT);
		arg[9] = new Fl_Input(5, 240, 135, 30, mgl_gettext("String for $9"));
		arg[9]->align(FL_ALIGN_TOP_LEFT);
		arg[0] = new Fl_Input(150, 240, 135, 30, mgl_gettext("String for $0"));
		arg[0]->align(FL_ALIGN_TOP_LEFT);
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
	CalcDlg()
	{
		Fl_Button *o;	Fl_Group* g, *gg;
		w = new Fl_Double_Window(275, 275, mgl_gettext("Calculator"));
		g = new Fl_Group(5, 5, 265, 25);
		edit = new Fl_Input(5, 5, 240, 25);	//edit->callback(cb_calc_edit);
		o = new Fl_Return_Button(245, 5, 25, 25, "@>");
		o->callback(cb_calc_edit);	g->end();	g->resizable(edit);
		g = new Fl_Group(5, 35, 265, 25);
		output = new Fl_Output(55, 35, 120, 25, mgl_gettext("Result"));
		o = new Fl_Button(180, 35, 90, 25, mgl_gettext("@-> to script"));
		o->callback(cb_calc_ins);	g->end();	g->resizable(output);
		prev = new Fl_Select_Browser(5, 80, 265, 70, mgl_gettext("Previous expressions"));
		prev->align(FL_ALIGN_TOP_LEFT);	prev->callback(cb_calc_prev);
		static int widths[] = { 200, 65, 0 };  // widths for each column
		prev->column_widths(widths);	prev->column_char('\t');
		gg = new Fl_Group(5, 155, 265, 115);
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

			g = new Fl_Group(155, 175, 115, 95, mgl_gettext("Function"));
			kind = new Fl_Choice(160, 179, 105, 25);	kind->callback(cb_calc_kind);
			kind->add("Basic");	kind->add("Exp and log");	kind->add("Trigonometric");	
			kind->add("Hyperbolic");	kind->add("Bessel");	kind->add("Elliptic");	
			kind->add("Jacobi");	 kind->add("Airy and Gamma");
			kind->add("Exp-integrals"); kind->add("Special");	kind->value(0);
			
			func = new Fl_Choice(160, 209, 105, 25);
			o = new Fl_Button(160, 239, 105, 25, mgl_gettext("Put function"));	o->callback(cb_calc_func);
			g->end();	g->box(FL_DOWN_BOX);
		gg->end();	gg->resizable(g);
		
		w->end();	w->resizable(prev);
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
		else if(ext)	ext->value(result.c_str());
		else	cb_args_set(result.c_str());
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
std::string get_color(Fl_Choice *c, Fl_Spinner *s, Fl_Input *p)
{
	std::string res;
	const char *ps = p->value();
	int cv = c->value(), sv = s->value();
	int pv = ps?10*atof(ps):-1;
	if(pv<0 || pv>9)	ps = NULL;
	if(cv>0 && cv<strlen(cols))
	{
		if(ps)
		{
			if(sv>0 && sv<10 && sv!=5)
			{
				char buf[16];	snprintf(buf,16,"{%c%d,0.%d}",cols[cv],sv,pv);
				res = buf;
			}
			else	res = cols[cv];
		}
		else
		{
			if(sv>0 && sv<10 && sv!=5)
			{
				char buf[16];	snprintf(buf,16,"{%c%d}",cols[cv],sv);
				res = buf;
			}
			else	res = cols[cv];
		}
	}
	return res;
}
//-----------------------------------------------------------------------------
void cb_style_upd(Fl_Widget *, void *);
void cb_style_sch(Fl_Widget *, void *);
class StyleDlg : public GeneralDlg
{
	Fl_Choice *arr1, *dash, *arr2;
	Fl_Choice *mark;
	Fl_Check_Button *solid, *user;
	Fl_Spinner *width;
	Fl_Button *dash_m[16];
	
	Fl_Choice *c[8], *sch;
	Fl_Spinner *s[8];
	Fl_Input *p[8];
	Fl_Choice *axial, *contt, *mask, *angle;
	Fl_Spinner *msize;
	Fl_Input *alpha;
	Fl_Button *mask_m[64];
	Fl_Check_Button *wire, *sharp;
	
	Fl_Check_Button *bold, *ital, *twire, *uline, *oline, *plain;
	Fl_Choice *align, *vert;
	
	Fl_Group *gline, *gsurf, *gfont;
	Fl_Output *res;
	Fl_MathGL *gr;
	std::string script;
public:
	Fl_Input *ext;
	StyleDlg()
	{
		Fl_Group *g;	Fl_Button *o;
		w = new Fl_Double_Window(380, 540, mgl_gettext("Plot style"));
		Fl_Tabs* tt = new Fl_Tabs(0, 5, 375, 235);
		gline = new Fl_Group(0, 30, 375, 210, mgl_gettext("Line style"));
			arr1 = new Fl_Choice(5, 50, 110, 25, mgl_gettext("Arrow at start"));
			arr1->align(FL_ALIGN_TOP_LEFT);	arr1->copy(arrows);
			arr1->callback(cb_style_upd);
			dash = new Fl_Choice(125, 50, 110, 25, mgl_gettext("Dashing"));
			dash->align(FL_ALIGN_TOP_LEFT);	dash->copy(dashing);
			dash->callback(cb_style_upd);
			arr2 = new Fl_Choice(245, 50, 110, 25, mgl_gettext("Arrow at end"));
			arr2->align(FL_ALIGN_TOP_LEFT);	arr2->copy(arrows);
			arr2->callback(cb_style_upd);
			mark = new Fl_Choice(125, 80, 110, 25, mgl_gettext("Marks"));
			mark->copy(markers);	mark->callback(cb_style_upd);
			solid = new Fl_Check_Button(240, 80, 55, 25, mgl_gettext("solid"));
			user = new Fl_Check_Button(300, 80, 55, 25, mgl_gettext("user"));
			solid->callback(cb_style_upd);	user->callback(cb_style_upd);
			width = new Fl_Spinner(125, 110, 110, 25, mgl_gettext("Width"));
			width->range(1,9);	width->value(1);	width->callback(cb_style_upd);
			for(int i=0;i<16;i++)
			{
				dash_m[i] = new Fl_Toggle_Button(10+20*i, 210, 20, 20);
				dash_m[i]->callback(cb_style_upd);
			}
			dash_m[0]->label(mgl_gettext("Manual dashing"));
			dash_m[0]->align(FL_ALIGN_TOP_LEFT);
			gline->end();
		gsurf = new Fl_Group(0, 30, 375, 210, mgl_gettext("Color scheme"));	gsurf->hide();
			axial = new Fl_Choice(5, 50, 110, 25, mgl_gettext("Axial direction"));
			axial->align(FL_ALIGN_TOP_LEFT);	axial->callback(cb_style_upd);
			axial->add("none");	axial->add("x");	axial->add("y");	axial->add("z");
			contt = new Fl_Choice(125, 50, 110, 25, mgl_gettext("Text on contours"));
			contt->add("none");	contt->add("under");	contt->add("above");
			contt->align(FL_ALIGN_TOP_LEFT);	contt->callback(cb_style_upd);
			alpha = new Fl_Input(255, 50, 110, 25, mgl_gettext("Transparency"));
			alpha->align(FL_ALIGN_TOP_LEFT);	alpha->callback(cb_style_upd);
			wire = new Fl_Check_Button(125, 80, 115, 25, mgl_gettext("Wire or mesh"));
			wire->callback(cb_style_upd);
			sharp = new Fl_Check_Button(250, 80, 110, 25, mgl_gettext("Sharp colors"));
			sharp->callback(cb_style_upd);
			g = new Fl_Group(10, 105, 360, 130, mgl_gettext("Mask"));
				g->box(FL_ENGRAVED_BOX);	g->align(FL_ALIGN_TOP_LEFT);
				mask = new Fl_Choice(80, 110, 95, 25, mgl_gettext("Kind"));
				mask->copy(masks);	mask->callback(cb_style_upd);
				angle = new Fl_Choice(80, 140, 95, 25, mgl_gettext("Rotation"));
				angle->add("none");	angle->add("+45");	angle->add("perp");	angle->add("-45");
				angle->callback(cb_style_upd);
				msize = new Fl_Spinner(80, 170, 95, 25, mgl_gettext("Size"));
				msize->range(1,9);	msize->value(1);	msize->callback(cb_style_upd);
				for(int i=0;i<8;i++)	for(int j=0;j<8;j++)
				{
					mask_m[i+8*j] = new Fl_Toggle_Button(240+15*i, 110+15*(7-j), 15, 15);
					mask_m[i+8*j]->callback(cb_style_upd);
				}
			g->end();	gsurf->end();
		gfont = new Fl_Group(0, 30, 375, 210, mgl_gettext("Text style"));	gfont->hide();
			bold = new Fl_Check_Button(5, 40, 150, 25, mgl_gettext("Bold style"));
			ital = new Fl_Check_Button(5, 65, 150, 25, mgl_gettext("Italic style"));
			twire = new Fl_Check_Button(5, 90, 150, 25, mgl_gettext("Wire style"));
			uline = new Fl_Check_Button(5, 115, 150, 25, mgl_gettext("Underline"));
			oline = new Fl_Check_Button(5, 140, 150, 25, mgl_gettext("Overline"));
			bold->callback(cb_style_upd);	ital->callback(cb_style_upd);
			twire->callback(cb_style_upd);	uline->callback(cb_style_upd);
			oline->callback(cb_style_upd);
			align = new Fl_Choice(270, 40, 95, 25, mgl_gettext("Text align"));
			align->add("left");	align->add("center");	align->add("right");
			align->value(1);	align->callback(cb_style_upd);
			vert = new Fl_Choice(270, 75, 95, 25, mgl_gettext("Vertical align"));
			vert->add("default");	vert->add("center");	vert->add("under");
			vert->value(0);	vert->callback(cb_style_upd);
		gfont->end();	tt->end();	tt->callback(cb_style_upd);

		g = new Fl_Group(0, 265, 375, 155, mgl_gettext("Color(s) or color scheme"));
		g->box(FL_ENGRAVED_BOX);	g->align(FL_ALIGN_TOP_LEFT);
			sch = new Fl_Choice(170, 270, 115, 25, mgl_gettext("Popular color schemes"));
			sch->add("BbcyrR");	sch->add("kw");		sch->add("wk");		sch->add("bwr");
			sch->add("kHCcw");	sch->add("kBbcw");	sch->add("kRryw");	sch->add("kGgew");
			sch->add("BbwrR");	sch->add("BbwgG");	sch->add("GgwmM");	sch->add("UuwqR");
			sch->add("QqwcC");	sch->add("CcwyY");	sch->add("bcwyr");	sch->add("wUrqy");
			sch->add("UbcyqR");	sch->add("bgr");	sch->callback(cb_style_sch);
			plain = new Fl_Check_Button(290, 270, 80, 25, mgl_gettext("plain"));
			plain->callback(cb_style_upd);
			for(int i=0;i<8;i++)
			{
				c[i] = new Fl_Choice(5+185*(i/4), 300+30*(i%4), 95, 25);
				c[i]->copy(colors);	c[i]->callback(cb_style_upd);
				c[i]->tooltip(mgl_gettext("Value for i-th color"));
				s[i] = new Fl_Spinner(105+185*(i/4), 300+30*(i%4), 40, 25);
				s[i]->range(1,9);	s[i]->value(5);
				c[i]->tooltip(mgl_gettext("Brightness of i-th color"));
				s[i]->callback(cb_style_upd);
				p[i] = new Fl_Input(145+185*(i/4), 300+30*(i%4), 40, 25);
				p[i]->tooltip(mgl_gettext("Relative position of i-th color"));
				p[i]->callback(cb_style_upd);
			}
			g->end();
		res = new Fl_Output(5, 440, 370, 25, mgl_gettext("Resulting string"));
		res->align(FL_ALIGN_TOP_LEFT);
		o = new Fl_Button(300, 475, 75, 25, mgl_gettext("Cancel"));	o->callback(cb_dlg_cancel,this);
		o = new Fl_Return_Button(300, 505, 75, 25, mgl_gettext("OK"));	o->callback(cb_dlg_ok,this);
		gr = new Fl_MathGL(5, 470, 285, 65);	gr->box(FL_ENGRAVED_BOX);
		mgl_set_size(gr->get_graph(),285,65);	gr->align(FL_ALIGN_LEFT);
		w->set_modal();	w->end();
	}
	void set_scheme()
	{
		const char *ss = sch->text();
		if(!ss || *ss==0)	return;
		for(int i=0;i<8;i++)
		{	p[i]->value(NULL);	s[i]->value(5);	c[i]->value(0);	}
		for(int i=0;i<8;i++)
		{
			if(ss[i]==0)	break;
			size_t pos = strchr(cols,ss[i])-cols;
			c[i]->value(pos);
		}
		update();
	}
	void stl_color()
	{
		result.clear();
		for(int i=0;i<8;i++)
			result += get_color(c[i],s[i],p[i]);
	}
	void stl_line()
	{
		stl_color();
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
		script = "new a 5 5 'y':plot a "+result+";size 8";
	}
	void stl_surf()
	{
		stl_color();
		int v = contt->value();	const char *tt="Tt";
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
#ifdef WIN32
			char buf[128];	snprintf(buf,128,"{s%llX}",mask);
#else
			char buf[128];	snprintf(buf,128,"{s%lX}",mask);
#endif
			result += buf;
		}
		if(wire->value())	result += '#';
		if(sharp->value())	result += '|';
		if(plain->value())	result += '%';
		v = atoi(alpha->value());
		if(v>0 && v<10)	result = result + "{A"+char(v+'0')+'}';
		v = axial->value();	const char *ax="xyz";
		if(v>0 && v<4)	result = result+':'+ax[v-1];
		result = '\''+result+'\'';
		script = "fsurf 'x' "+result;
	}
	void stl_font()
	{
		stl_color();
		const char *a="LCR";
		result = result+':'+a[align->value()];
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
			script = "clf:subplot 1 1 0 '':"+script;
			mgl_parse_text(gr->get_graph(), pr.Self(), script.c_str());
			gr->update();
		busy=0;	}
	}
	void cb_ok()
	{
		if(e)	e->editor->insert(result.c_str());
		else if(ext)	ext->value(result.c_str());
		else	cb_args_set(result.c_str());
		hide();
	}
} style_dlg;
//-----------------------------------------------------------------------------
void cb_style_upd(Fl_Widget *, void *)	{	style_dlg.update();	}
void cb_style_sch(Fl_Widget *, void *)	{	style_dlg.set_scheme();	}
//-----------------------------------------------------------------------------
void style_dlg_cb(Fl_Widget *, void *v)
{	style_dlg.ext=NULL;	style_dlg.e=(ScriptWindow *)v;	style_dlg.show();	}
//-----------------------------------------------------------------------------
void style_in_cb(Fl_Widget *, void *v)
{	style_dlg.ext=(Fl_Input*)v;	style_dlg.e=NULL;	style_dlg.show();	}
//-----------------------------------------------------------------------------
void cb_datsel_upd(Fl_Widget *, void *);
void cb_datsel_act(Fl_Widget *, void *);
class DatSelDlg : public GeneralDlg
{
	Fl_Choice *name;
	Fl_Choice *oper;
	Fl_Choice *dir;
	Fl_Spinner *x1, *x2, *y1, *y2, *z1, *z2;
	Fl_Input *clmn;
	Fl_Check_Button *ax, *ay, *az;
public:
	Fl_Input *ext;
	DatSelDlg()
	{
		Fl_Button *o;
		w = new Fl_Double_Window(355, 255, mgl_gettext("Select data argument"));
		name = new Fl_Choice(105, 5, 145, 25, mgl_gettext("Data name"));
		name->callback(cb_datsel_upd);
		x1 = new Fl_Spinner(105, 35, 60, 25, mgl_gettext("X-slice from"));
		x2 = new Fl_Spinner(190, 35, 60, 25, mgl_gettext("to"));
		ax = new Fl_Check_Button(260, 35, 90, 25, mgl_gettext("all"));	ax->callback(cb_datsel_act);
		y1 = new Fl_Spinner(105, 65, 60, 25, mgl_gettext("Y-slice from"));
		y2 = new Fl_Spinner(190, 65, 60, 25, mgl_gettext("to"));
		ay = new Fl_Check_Button(260, 65, 90, 25, mgl_gettext("all"));	ay->callback(cb_datsel_act);
		z1 = new Fl_Spinner(105, 95, 60, 25, mgl_gettext("Z-slice from"));
		z2 = new Fl_Spinner(190, 95, 60, 25, mgl_gettext("to"));
		az = new Fl_Check_Button(260, 95, 90, 25, mgl_gettext("all"));	az->callback(cb_datsel_act);
		clmn = new Fl_Input(105, 125, 245, 25, mgl_gettext("Column expr"));
		oper = new Fl_Choice(105, 155, 130, 25, mgl_gettext("Operation"));
		oper->add("none");	oper->add("max");	oper->add("min");	oper->add("sum");
		oper->add("trace");	oper->add("pulse");	oper->value(0);
		dir = new Fl_Choice(285, 155, 65, 25, mgl_gettext("along"));
		dir->add("none");	dir->add("x");	dir->add("y");	dir->add("z");
		dir->add("xy");		dir->add("xz");	dir->add("yz");
		o = new Fl_Button(190, 190, 75, 25, mgl_gettext("Cancel"));	o->callback(cb_dlg_cancel,this);
		o = new Fl_Return_Button(275, 190, 75, 25, mgl_gettext("OK"));	o->callback(cb_dlg_ok,this);
		w->set_modal();	w->end();
	}
	void cb_ok()
	{
		result.clear();
		if(name->value()<0)
		{	fl_alert(mgl_gettext("You need to select data array"));	return;	}
		std::string data = name->text();
		const char *eq = clmn->value();
		int rx=ax->value(), ry=ay->value(), rz=az->value();
		int vx1=x1->value(), vy1=y1->value(), vz1=z1->value();
		int vx2=x2->value(), vy2=y2->value(), vz2=z2->value();
		if(eq && *eq)	data = data+"('"+eq+"')";
		else	if(!rx || !ry || !rz)
		{
			char bx[256],by[256],bz[256];
			if(rx)	strcpy(bx,"(:");
			else if(vx2<=vx1)	snprintf(bx,255,"(%d",vx1);
			else	snprintf(bx,255,"(%d:%d",vx1,vx2);
			if(ry)	strcpy(by,",:");
			else if(vy2<=vy1)	snprintf(by,255,",%d",vy1);
			else	snprintf(by,255,",%d:%d",vy1,vy2);
			if(vz2<=vz1)	snprintf(bz,255,",%d)",vz1);
			else	snprintf(bz,255,",%d:%d)",vz1,vz2);
			if(!rz)	data = data+bx+by+bz;
			else if(!ry)	data = data+bx+by+')';
			else if(!rx)	data = data+bx+')';
		}
		if(oper->value()>0)
		{
			result = result+'{'+oper->text()+' '+data;
			if(dir->value()>0)	result = result+" '"+dir->text()+"'}";
			else	result += '}';
		}
		else	result = data;
		if(e)	e->editor->insert(result.c_str());
		else if(ext)	ext->value(result.c_str());
		else	cb_args_set(result.c_str());
		hide();
	}
	void init()
	{
		name->clear();
		long n = Parse->GetNumVar();
		for(long i=0;i<n;i++)
		{
			HCDT d = Parse->GetVar(i);
			if(!d->temp)
			{
				const wchar_t *ss = d->s.c_str();
				size_t s=wcstombs(0,ss,0);	char *buf=new char[s+1];
				wcstombs(buf,ss,s); buf[s]=0;
				name->add(buf);	delete []buf;
			}
		}
		x1->value(0);	x2->value(0);
		y1->value(0);	y2->value(0);
		z1->value(0);	z2->value(0);
	}
	void update()
	{
		HCDT d = Parse->FindVar(name->text());
		if(d)
		{
			long nx=d->GetNx()-1, ny=d->GetNy()-1, nz=d->GetNz()-1;
			x1->range(0,nx);	if(x1->value()>nx)	x1->value(0);
			x2->range(0,nx);	if(x2->value()>nx)	x2->value(0);
			y1->range(0,ny);	if(y1->value()>ny)	y1->value(0);
			y2->range(0,ny);	if(y2->value()>ny)	y2->value(0);
			z1->range(0,nz);	if(z1->value()>nz)	z1->value(0);
			z2->range(0,nz);	if(z2->value()>nz)	z2->value(0);
			ax->value(1);	ay->value(1);	az->value(1);
			x1->deactivate();	y1->deactivate();	z1->deactivate();
			x2->deactivate();	y2->deactivate();	z2->deactivate();
			clmn->value("");
		}
	}
	void activate()
	{
		if(!ax->value())	{	x1->activate();	x2->activate();	}
		else	{	x1->deactivate();	x2->deactivate();	}
		if(!ay->value())	{	y1->activate();	y2->activate();	}
		else	{	y1->deactivate();	y2->deactivate();	}
		if(!az->value())	{	z1->activate();	z2->activate();	}
		else	{	z1->deactivate();	z2->deactivate();	}
	}
} datsel_dlg;
//-----------------------------------------------------------------------------
void cb_datsel_upd(Fl_Widget *, void *)	{	datsel_dlg.update();	}
void cb_datsel_act(Fl_Widget *, void *)	{	datsel_dlg.activate();	}
//-----------------------------------------------------------------------------
void datsel_dlg_cb(Fl_Widget *, void *v)
{	datsel_dlg.ext=NULL;	datsel_dlg.e=(ScriptWindow *)v;	datsel_dlg.show();	}
//-----------------------------------------------------------------------------
void datsel_in_cb(Fl_Widget *, void *v)
{	datsel_dlg.ext=(Fl_Input*)v;	datsel_dlg.e=NULL;	datsel_dlg.show();	}
//-----------------------------------------------------------------------------
void cb_cmd_type(Fl_Widget*, void*);
void cb_cmd_cmd(Fl_Widget*, void*);
void cb_cmd_var(Fl_Widget*, void*);
void cb_cmd_args(Fl_Widget*, void*);
class NewCmdDlg : public GeneralDlg
{
	Fl_Choice *type, *cmd, *var;
	Fl_Group *desc;
	Fl_Select_Browser *args;
	Fl_Input *opt;
	Fl_Help_View *help;
	std::vector<std::string> cmds[17];	///< commands divided by type
public:
	NewCmdDlg()
	{
		Fl_Button *o;
		w = new Fl_Double_Window(780, 300, mgl_gettext("New command"));
		Fl_Group *g = new Fl_Group(5,5,315,320);
		type = new Fl_Choice(80, 5, 270, 25, mgl_gettext("Type"));
		type->tooltip(mgl_gettext("Groups of MGL commands"));
		type->callback(cb_cmd_type);
		type->add(mgl_gettext("1D plots"));
		type->add(mgl_gettext("2D plots"));
		type->add(mgl_gettext("3D plots"));
		type->add(mgl_gettext("Dual plots"));
		type->add(mgl_gettext("Vector plots"));
		type->add(mgl_gettext("Other plots"));
		type->add(mgl_gettext("Text and legend"));
		type->add(mgl_gettext("Create data and IO"));
		type->add(mgl_gettext("Data transform"));
		type->add(mgl_gettext("Data handling"));
		type->add(mgl_gettext("Axis and colorbar"));
		type->add(mgl_gettext("Axis setup"));
		type->add(mgl_gettext("General setup"));
		type->add(mgl_gettext("Scale and rotate"));
		type->add(mgl_gettext("Program flow"));
		type->add(mgl_gettext("Primitives"));

		cmd = new Fl_Choice(80, 35, 270, 25, mgl_gettext("Command"));
		cmd->tooltip(mgl_gettext("MGL commands for selected group"));
		cmd->callback(cb_cmd_cmd);
		var = new Fl_Choice(80, 95, 270, 25, mgl_gettext("Variant"));
		var->tooltip(mgl_gettext("Variant of command argument order. The notation is:\n"
								" * Capital arguments are data (like, Ydat);\n"
								" * Argument in '' are strings (like, 'fmt');\n"
								" * Other arguments are numbers (like, zval);\n"
								" * Arguments in [] are optional arguments."));
		var->callback(cb_cmd_var);
		desc = new Fl_Group(0, 65, 350, 25, mgl_gettext("Description"));
		desc->box(FL_ENGRAVED_BOX);	desc->labelsize(12);
		desc->align(FL_ALIGN_CENTER);	desc->end();
		desc->tooltip(mgl_gettext("Short description of selected command"));
		args = new Fl_Select_Browser(5, 140, 345, 95, mgl_gettext("Arguments"));
		args->align(FL_ALIGN_TOP_LEFT);	args->callback(cb_cmd_args);
		args->tooltip(mgl_gettext("Command arguments. Bold ones are required arguments.\n"
			"Other are optional arguments but its order is required.\n"
			"You can use '' for default format. See help at right\nfor default values."));
		static int widths[] = { 95, 250, 0 };  // widths for each column
		args->column_widths(widths);	args->column_char('\t');
		
		opt = new Fl_Input(60, 240, 265, 25, mgl_gettext("Options"));
		o = new Fl_Button(325, 240, 25, 25, "...");	o->callback(option_in_cb,opt);

		o = new Fl_Button(190, 270, 75, 25, mgl_gettext("Cancel"));	o->callback(cb_dlg_cancel,this);
		o = new Fl_Return_Button(275, 270, 75, 25, mgl_gettext("OK"));	o->callback(cb_dlg_ok,this);
		g->end();	g->resizable(args);

		help = new Fl_Help_View(360, 5, 415, 290);	help->labelsize(12);
		w->set_modal();	w->end();	w->resizable(help);
	}
	void init()	// fill cmds from parser for all categories
	{
		long i, n = Parse->GetCmdNum();
		for(i=0;i<n;i++)
		{
			std::string name = Parse->GetCmdName(i);
			switch(Parse->CmdType(name.c_str()))
			{
				case 1:	cmds[5].push_back(name);	break;
				case 2:	cmds[5].push_back(name);	break;
				case 3:	cmds[12].push_back(name);	break;
				case 4:	cmds[9].push_back(name);	break;
				case 5:	cmds[7].push_back(name);	break;
				case 6:	cmds[13].push_back(name);	break;
				case 7:	cmds[14].push_back(name);	break;
				case 8:	cmds[0].push_back(name);	break;
				case 9:	cmds[1].push_back(name);	break;
				case 10:cmds[2].push_back(name);	break;
				case 11:cmds[3].push_back(name);	break;
				case 12:cmds[4].push_back(name);	break;
				case 13:cmds[10].push_back(name);	break;
				case 14:cmds[15].push_back(name);	break;
				case 15:cmds[11].push_back(name);	break;
				case 16:cmds[6].push_back(name);	break;
				case 17:cmds[8].push_back(name);	break;
			}
		}
		type->value(0);	type_sel();
	}
	void type_sel()	// fill list of commands for selected type
	{
		int t = type->value();	cmd->clear();
		for(size_t i=0;i<cmds[t].size();i++)	cmd->add(cmds[t][i].c_str());
		cmd->value(0);	cmd_sel();
	}
	void cmd_sel()	// fill list of variants for selected command
	{
		const char *c = cmd->text();
		desc->label(Parse->CmdDesc(c));
		static std::string str = docdir+"/mgl_en.html#"+c;
		help->load(str.c_str());
		std::string par = Parse->CmdFormat(c), cname;
		std::vector<std::string> vars;
		size_t isp = par.find_first_of(' ');
		if(isp<par.length())
		{
			cname = par.substr(0,isp+1);
			par = par.substr(isp+1);
			while((isp=par.find_first_of('|'))<par.length())
			{
				vars.push_back(cname+par.substr(0,isp));
				par = par.substr(isp+1);
			}
			vars.push_back(cname+par);
		}
		else	vars.push_back(par);
		var->clear();
		for(size_t i=0;i<vars.size();i++)	var->add(vars[i].c_str());
		var->value(0);	var_sel();
	}
	void var_sel()	// fill list of arguments for selected variant
	{
		std::string par = var->text(), sec;
		size_t isp = par.find_first_of(' ');
		par = par.substr(isp+1);	// remove command name
		isp = par.find_first_of('[');	// here secional args starts
		sec = isp<par.length()?par.substr(isp+1,par.length()-isp-2):"";
		par = isp>0?par.substr(0,isp-1):"";
		args->clear();
		while((isp=par.find_first_of(' '))<par.length())
		{
			args->add(("@b "+par.substr(0,isp)).c_str());
			par = par.substr(isp+1);
		}
		if(!par.empty())	args->add(("@b "+par).c_str());
		while((isp=sec.find_first_of(' '))<sec.length())
		{
			args->add(sec.substr(0,isp).c_str());
			sec = sec.substr(isp+1);
		}
		if(!sec.empty())	args->add(sec.c_str());
	}
	void args_sel()	// fill argument by calling external dialog
	{
		int a = args->value();
		const char *s = args->text(a);	if(!s || *s==0)	return;
		std::string arg = s, val;
		size_t isp = arg.find_first_of('\t');
		val = arg.substr(isp+1);	arg = arg.substr(0,isp);
		if(arg[0]=='@')	arg = arg.substr(3);
		std::string question = mgl_gettext("Enter value for ")+arg+mgl_gettext(" argument");
		if(arg[0]>='A' && arg[0]<='Z')	datsel_dlg_cb(0,0);	// this is data
		else if(arg=="'fmt'")	style_dlg_cb(0,0);	// this is style
		else if(arg=="'fname'")	ins_fname_cb(0,0);	// this is file name
		else if(arg=="'path'")	ins_path_cb(0,0);	// this is path
		else if(arg=="'dir'")	dirsel_dlg_cb(0,0);	// this is path
		else if(arg[0]=='\'')	// this is general string
		{
			const char *s = fl_input(question.c_str(),"");
			if(s)
			{	std::string ss=s;	args_set(('\''+ss+'\'').c_str());	}
		}
		else	// this is general constant
		{
			const char *s = fl_input(question.c_str(),"");
			if(s)	args_set(s);
		}
	}
	void args_set(const char *val)	// set value for current argument
	{
		int a = args->value();
		const char *s = args->text(a);	if(!s || *s==0)	return;
		std::string arg = s;
		size_t isp = arg.find_first_of('\t');
		arg = arg.substr(0,isp)+'\t'+val;
		args->text(a,arg.c_str());
	}
	void cb_ok()
	{
		std::string par = var->text(), a;
		size_t isp = par.find_first_of(' ');
		result = par.substr(0,isp);	// command name
		for(int i=0;i<args->size();i++)
		{
			const char *s = args->text(i);
			if(!s)	continue;
			const char *p = strchr(s,'\t');
			if(s[0]=='@' && !p)
			{
				fl_alert(mgl_gettext("Required argument %s is not specified!"),s+3);	return;
			}
			if(p)	result = result+' '+p;
		}
		result += opt->value();
		if(e)	e->editor->insert(result.c_str());
		hide();
	}
	void set_cmd(const char *line)	// TODO
	{}
} newcmd_dlg;
//-----------------------------------------------------------------------------
void cb_cmd_type(Fl_Widget*, void*)	{	newcmd_dlg.type_sel();	}
void cb_cmd_cmd(Fl_Widget*, void*)	{	newcmd_dlg.cmd_sel();	}
void cb_cmd_var(Fl_Widget*, void*)	{	newcmd_dlg.var_sel();	}
void cb_cmd_args(Fl_Widget*, void*)	{	newcmd_dlg.args_sel();	}
void cb_args_set(const char *val)	{	newcmd_dlg.args_set(val);	}
//-----------------------------------------------------------------------------
void newcmd_dlg_cb(Fl_Widget*,void *v)
{	newcmd_dlg.e=(ScriptWindow *)v;	newcmd_dlg.show();	}
//-----------------------------------------------------------------------------
void cb_setup_save(Fl_Widget*,void *v);
class SetupDlg : public GeneralDlg
{
	Fl_Choice *xlpos, *ylpos, *zlpos, *clpos;
	Fl_Float_Input *x1, *x2, *x0, *xtick, *xstick, *xotick;
	Fl_Input *xlabel, *xtmpl, *xfact;
	Fl_Float_Input *y1, *y2, *y0, *ytick, *ystick, *yotick;
	Fl_Input *ylabel, *ytmpl, *yfact;
	Fl_Float_Input *z1, *z2, *z0, *ztick, *zstick, *zotick;
	Fl_Input *zlabel, *ztmpl, *zfact;
	Fl_Float_Input *c1, *c2, *c0, *ctick;
	Fl_Input *clabel, *ctmpl, *cfact;
	Fl_Float_Input *alphadef, *ambient, *diffuse;
	Fl_Input *palette, *font_stl, *axis_stl, *plotid;
	Fl_Float_Input *fog, *fog_dz, *pendelta;
	Fl_Float_Input *meshnum, *facenum, *arr_size, *bar_size;
	Fl_Float_Input *mrk_size, *txt_size, *tick_size;

	Fl_Check_Button *alpha, *light, *cut, *attach;
	Fl_Check_Button *origintick, *gray, *rotatetext;
	Fl_Choice *time, *tunetick, *ternary, *transptype;
	Fl_Spinner *variant;
	
	Fl_Toggle_Button *lb[10];
	Fl_Choice *lc[10];
	Fl_Float_Input *lx[10], *ly[10], *lz[10], *lbr[10];
	Fl_Input *fname;
public:
	SetupDlg()
	{
		Fl_Button *o;	Fl_Group *g, *gg;
		w = new Fl_Double_Window(525, 395, mgl_gettext("Setup script"));
		Fl_Tabs* tt = new Fl_Tabs(0, 0, 525, 355);
		gg = new Fl_Group(0, 25, 525, 330, mgl_gettext("Axis settings"));
			new Fl_Box(85, 30, 100, 25, mgl_gettext("X axis"));
			x1 = new Fl_Float_Input(85, 55, 100, 25, mgl_gettext("Minimal"));
			x2 = new Fl_Float_Input(85, 85, 100, 25, mgl_gettext("Maximal"));
			x0 = new Fl_Float_Input(85, 115, 100, 25, mgl_gettext("Origin"));
			xlabel = new Fl_Input(85, 145, 100, 25, mgl_gettext("Label"));
			xtick = new Fl_Float_Input(85, 205, 100, 25, mgl_gettext("Ticks"));
			xlpos = new Fl_Choice(85, 175, 100, 25, mgl_gettext("at position"));
			xlpos->add(mgl_gettext("left"));	xlpos->add(mgl_gettext("center"));
			xlpos->add(mgl_gettext("right"));	xlpos->value(1);
			xstick = new Fl_Float_Input(85, 235, 100, 25, mgl_gettext("Subticks"));
			xotick = new Fl_Float_Input(85, 265, 100, 25, mgl_gettext("Ticks start"));
			xtmpl = new Fl_Input(85, 295, 100, 25, mgl_gettext("Template"));
			xfact = new Fl_Input(85, 325, 100, 25, mgl_gettext("Template"));		
			new Fl_Box(195, 30, 100, 25, mgl_gettext("Y axis"));
			y1 = new Fl_Float_Input(195, 55, 100, 25);
			y2 = new Fl_Float_Input(195, 85, 100, 25);
			y0 = new Fl_Float_Input(195, 115, 100, 25);
			ylabel = new Fl_Input(195, 145, 100, 25);
			ytick = new Fl_Float_Input(195, 205, 100, 25);
			ylpos = new Fl_Choice(195, 175, 100, 25);
			ylpos->add(mgl_gettext("left"));	ylpos->add(mgl_gettext("center"));
			ylpos->add(mgl_gettext("right"));	ylpos->value(1);
			ystick = new Fl_Float_Input(195, 235, 100, 25);
			yotick = new Fl_Float_Input(195, 265, 100, 25);
			ytmpl = new Fl_Input(195, 295, 100, 25);
			yfact = new Fl_Input(195, 325, 100, 25);
			new Fl_Box(305, 30, 100, 25, mgl_gettext("Z axis"));
			z1 = new Fl_Float_Input(305, 55, 100, 25);
			z2 = new Fl_Float_Input(305, 85, 100, 25);
			z0 = new Fl_Float_Input(305, 115, 100, 25);
			zlabel = new Fl_Input(305, 145, 100, 25);
			ztick = new Fl_Float_Input(305, 205, 100, 25);
			zlpos = new Fl_Choice(305, 175, 100, 25);
			zlpos->add(mgl_gettext("left"));	zlpos->add(mgl_gettext("center"));
			zlpos->add(mgl_gettext("right"));	zlpos->value(1);
			zstick = new Fl_Float_Input(305, 235, 100, 25);
			zotick = new Fl_Float_Input(305, 265, 100, 25);
			ztmpl = new Fl_Input(305, 295, 100, 25);
			zfact = new Fl_Input(305, 325, 100, 25);
			new Fl_Box(415, 30, 100, 25, mgl_gettext("C axis"));
			c1 = new Fl_Float_Input(415, 55, 100, 25);
			c2 = new Fl_Float_Input(415, 85, 100, 25);
			c0 = new Fl_Float_Input(415, 115, 100, 25);
			clabel = new Fl_Input(415, 145, 100, 25);
			clpos = new Fl_Choice(415, 175, 100, 25);
			clpos->add(mgl_gettext("left"));	clpos->add(mgl_gettext("center"));
			clpos->add(mgl_gettext("right"));	clpos->value(1);
			ctmpl = new Fl_Input(415, 295, 100, 25);
			cfact = new Fl_Input(415, 325, 100, 25);
			gg->end();
		gg = new Fl_Group(0, 25, 525, 330, mgl_gettext("General settings"));	gg->hide();
			g = new Fl_Group(5, 45, 180, 305, mgl_gettext("Colors"));	g->box(FL_ENGRAVED_BOX);
			alphadef = new Fl_Float_Input(80, 55, 100, 25, mgl_gettext("AlphaDef"));
			ambient = new Fl_Float_Input(80, 85, 100, 25, mgl_gettext("Ambient"));
			diffuse = new Fl_Float_Input(80, 115, 100, 25, mgl_gettext("Diffuse"));
			palette = new Fl_Input(80, 145, 100, 25, mgl_gettext("Palette"));
			font_stl = new Fl_Input(80, 175, 100, 25, mgl_gettext("Font"));
			axis_stl = new Fl_Input(80, 205, 100, 25, mgl_gettext("Axis"));
			fog = new Fl_Float_Input(80, 235, 100, 25, mgl_gettext("Fog"));
			fog_dz = new Fl_Float_Input(80, 265, 100, 25, mgl_gettext("Fog dist."));
			gray = new Fl_Check_Button(80, 290, 100, 25, mgl_gettext("grayscale"));
			alpha = new Fl_Check_Button(10, 315, 65, 25, mgl_gettext("alpha"));
			light = new Fl_Check_Button(80, 315, 100, 25, mgl_gettext("lighting"));
			g->end();
			g = new Fl_Group(185, 45, 180, 250, mgl_gettext("Sizes"));	g->box(FL_ENGRAVED_BOX);
			meshnum = new Fl_Float_Input(260, 55, 100, 25, mgl_gettext("meshnum"));
			facenum = new Fl_Float_Input(260, 85, 100, 25, mgl_gettext("facenum"));
			arr_size = new Fl_Float_Input(260, 115, 100, 25, mgl_gettext("arrows"));
			bar_size = new Fl_Float_Input(260, 145, 100, 25, mgl_gettext("bars"));
			mrk_size = new Fl_Float_Input(260, 175, 100, 25, mgl_gettext("markers"));
			txt_size = new Fl_Float_Input(260, 205, 100, 25, mgl_gettext("text"));
			tick_size = new Fl_Float_Input(260, 235, 100, 25, mgl_gettext("ticks"));
			pendelta = new Fl_Float_Input(260, 265, 100, 25, mgl_gettext("pen blur"));
			g->end();
			g = new Fl_Group(365, 45, 155, 305, mgl_gettext("Others"));	g->box(FL_ENGRAVED_BOX);
			cut = new Fl_Check_Button(370, 50, 115, 25, mgl_gettext("cutting"));
			attach = new Fl_Check_Button(370, 70, 115, 25, mgl_gettext("attach light"));
			origintick = new Fl_Check_Button(370, 90, 115, 25, mgl_gettext("no origin tick"));
			rotatetext = new Fl_Check_Button(370, 110, 115, 25, mgl_gettext("rotate text"));
			
			time = new Fl_Choice(370, 150, 145, 25, mgl_gettext("Time ticks"));
			time->add("none");	time->add("x");	time->add("y");	time->add("z");
			time->align(FL_ALIGN_TOP_LEFT);	time->value(0);
			tunetick = new Fl_Choice(370, 195, 145, 25, mgl_gettext("Tune ticks"));
			tunetick->add(mgl_gettext("none"));	tunetick->add(mgl_gettext("factor"));
			tunetick->add(mgl_gettext("increment"));	tunetick->add(mgl_gettext("both"));	
			tunetick->align(FL_ALIGN_TOP_LEFT);	tunetick->value(0);
			ternary = new Fl_Choice(370, 235, 145, 25, mgl_gettext("Ternary"));
			ternary->add(mgl_gettext("none"));	ternary->add(mgl_gettext("ternary"));
			ternary->add(mgl_gettext("quaternary"));	ternary->add(mgl_gettext("projection"));
			ternary->add(mgl_gettext("ternary proj"));	ternary->add(mgl_gettext("quaternary proj"));
			ternary->align(FL_ALIGN_TOP_LEFT);	ternary->value(0);
			transptype = new Fl_Choice(370, 275, 145, 25, mgl_gettext("Transparency type"));
			transptype->add(mgl_gettext("default"));	ternary->add(mgl_gettext("glass-like"));
			transptype->add(mgl_gettext("lamp-like"));
			transptype->align(FL_ALIGN_TOP_LEFT);	transptype->value(0);
			variant = new Fl_Spinner(370, 315, 145, 25, mgl_gettext("Variant"));
			variant->align(FL_ALIGN_TOP_LEFT);	variant->range(0,100);	variant->value(0);
			g->end();
		plotid = new Fl_Input(190, 321, 172, 25, mgl_gettext("Plot ID"));
		plotid->align(FL_ALIGN_TOP_LEFT);
		gg->end();
		gg = new Fl_Group(0, 25, 525, 330, mgl_gettext("Light settings"));	gg->hide();
			static const char *id[10]={"0:","1:","2:","3:""4:","5:","6:","7:","8:","9:"};
			for(int i=0;i<10;i++)
			{
				lb[i] = new Fl_Toggle_Button(10, 50+30*i, 25, 25, id[i]);
				lx[i] = new Fl_Float_Input(40, 50+30*i, 90, 25);
				ly[i] = new Fl_Float_Input(135, 50+30*i, 90, 25);
				lz[i] = new Fl_Float_Input(230, 50+30*i, 90, 25);
				lc[i] = new Fl_Choice(325, 50+30*i, 95, 25);
				lbr[i]= new Fl_Float_Input(425, 50+30*i, 90, 25);
				lc[i]->copy(colors);	lc[i]->value(0);
			}
			lx[0]->label(mgl_gettext("X position"));	lx[0]->align(FL_ALIGN_TOP_LEFT);
			ly[0]->label(mgl_gettext("Y position"));	ly[0]->align(FL_ALIGN_TOP_LEFT);
			lz[0]->label(mgl_gettext("Z position"));	lz[0]->align(FL_ALIGN_TOP_LEFT);
			lc[0]->label(mgl_gettext("Color"));			lc[0]->align(FL_ALIGN_TOP_LEFT);
			lbr[0]->label(mgl_gettext("Brightness"));	lbr[0]->align(FL_ALIGN_TOP_LEFT);
			gg->end();	tt->end();
		o = new Fl_Button(365, 365, 75, 25, mgl_gettext("Cancel"));	o->callback(cb_dlg_cancel,this);
		o = new Fl_Return_Button(445, 365, 75, 25, mgl_gettext("OK"));	o->callback(cb_dlg_ok,this);
		fname = new Fl_Input(100, 365, 175, 25, mgl_gettext("File to export"));
		o = new Fl_Button(275, 365, 25, 25, mgl_gettext("@->"));	o->callback(cb_setup_save);
		o->tooltip(mgl_gettext("Keep empty to put at beginning of main script."));
		w->end();
	}
	void prepare()
	{
		result.clear();
		const char *s1, *s2, *s3, *s4;
		s1=x1->value();	s2=x2->value();
		if(s1 && *s1 && s2 && *s2)	result = result+"xrange "+s1+' '+s2+'\n';
		s1=y1->value();	s2=y2->value();
		if(s1 && *s1 && s2 && *s2)	result = result+"yrange "+s1+' '+s2+'\n';
		s1=z1->value();	s2=z2->value();
		if(s1 && *s1 && s2 && *s2)	result = result+"zrange "+s1+' '+s2+'\n';
		s1=c1->value();	s2=c2->value();
		if(s1 && *s1 && s2 && *s2)	result = result+"crange "+s1+' '+s2+'\n';
		s1=x0->value();	s2=y0->value();	s3=z0->value();
		if(s3 && *s3)	result = result+"origin "+(s1?s1:"nan")+' '+(s2?s2:"nan")+' '+s3+'\n';
		else if(s2 && *s2)	result = result+"origin "+(s1?s1:"nan")+' '+s2+'\n';
		else if(s1 && *s1)	result = result+"origin "+s1+"nan\n";
		
		s1=xtmpl->value();	if(s1 && *s1)	result = result+"xtick '"+s1+"'\n";
		s1=ytmpl->value();	if(s1 && *s1)	result = result+"ytick '"+s1+"'\n";
		s1=ztmpl->value();	if(s1 && *s1)	result = result+"ztick '"+s1+"'\n";
		s1=ctmpl->value();	if(s1 && *s1)	result = result+"ctick '"+s1+"'\n";
		
		s1=xtick->value();	s2=xstick->value();	s3=xotick->value();	s4=xfact->value();
		if(s4 && *s4)	result = result+"xtick "+(s1?s1:"0")+' '+(s2?s2:"0")+' '+(s3?s3:"nan")+" '"+s4+"'\n";
		else if(s3 && *s3)	result = result+"xtick "+(s1?s1:"0")+' '+(s2?s2:"0")+' '+s3+'\n';
		else if(s2 && *s2)	result = result+"xtick "+(s1?s1:"0")+' '+s2+'\n';
		else if(s1 && *s1)	result = result+"xtick "+s1+'\n';
		s1=ytick->value();	s2=ystick->value();	s3=yotick->value();	s4=yfact->value();
		if(s4 && *s4)	result = result+"ytick "+(s1?s1:"0")+' '+(s2?s2:"0")+' '+(s3?s3:"nan")+" '"+s4+"'\n";
		else if(s3 && *s3)	result = result+"ytick "+(s1?s1:"0")+' '+(s2?s2:"0")+' '+s3+'\n';
		else if(s2 && *s2)	result = result+"ytick "+(s1?s1:"0")+' '+s2+'\n';
		else if(s1 && *s1)	result = result+"ytick "+s1+'\n';
		s1=ztick->value();	s2=zstick->value();	s3=zotick->value();	s4=zfact->value();
		if(s4 && *s4)	result = result+"ztick "+(s1?s1:"0")+' '+(s2?s2:"0")+' '+(s3?s3:"nan")+" '"+s4+"'\n";
		else if(s3 && *s3)	result = result+"ztick "+(s1?s1:"0")+' '+(s2?s2:"0")+' '+s3+'\n';
		else if(s2 && *s2)	result = result+"ztick "+(s1?s1:"0")+' '+s2+'\n';
		else if(s1 && *s1)	result = result+"ztick "+s1+'\n';
		s1=ctick->value();	s4=cfact->value();
		if(s4 && *s4)	result = result+"ctick "+(s1?s1:"0")+" '"+s4+"'\n";
		else if(s1 && *s1)	result = result+"ctick "+s1+'\n';
		const char *pos[3]={"' -1\n","' 0\n","' 1\n"};
		s1=xlabel->value();	if(s1 && *s1)	result = result+"xlabel '"+s1+pos[xlpos->value()];
		s1=ylabel->value();	if(s1 && *s1)	result = result+"ylabel '"+s1+pos[ylpos->value()];
		s1=zlabel->value();	if(s1 && *s1)	result = result+"zlabel '"+s1+pos[zlpos->value()];
//TODO	s1=clabel->value();	if(s1 && *s1)	result = result+"clabel '"+s1+pos[clpos->value()];
		
		s1=alphadef->value();	if(s1 && *s1)	result = result+"alphadef "+s1+'\n';
		s1=ambient->value();	if(s1 && *s1)	result = result+"ambient "+s1+'\n';
		s1=diffuse->value();	if(s1 && *s1)	result = result+"diffuse "+s1+'\n';
		s1=palette->value();	if(s1 && *s1)	result = result+"palette '"+s1+"'\n";
		s1=plotid->value();		if(s1 && *s1)	result = result+"plotid '"+s1+"'\n";
		s1=axis_stl->value();	if(s1 && *s1)	result = result+"axisstl '"+s1+"'\n";
		s1=meshnum->value();	if(s1 && *s1)	result = result+"meshnum "+s1+'\n';
		s1=facenum->value();	if(s1 && *s1)	result = result+"facenum "+s1+'\n';
		s1=arr_size->value();	if(s1 && *s1)	result = result+"arrowsize "+s1+'\n';
		s1=bar_size->value();	if(s1 && *s1)	result = result+"barwidth "+s1+'\n';
		s1=mrk_size->value();	if(s1 && *s1)	result = result+"marksize "+s1+'\n';
		s1=pendelta->value();	if(s1 && *s1)	result = result+"pendelta "+s1+'\n';
		s1=tick_size->value();	if(s1 && *s1)	result = result+"ticklen "+s1+'\n';

		s1=font_stl->value();	s2=txt_size->value();
		if(s2 && *s2)	result = result+"font '"+(s1?s1:"")+"' "+s2+'\n';
		else if(s1 && *s1)	result = result+"font '"+s1+"'\n";
		s1=fog->value();	s2=fog_dz->value();
		if(s1 && *s1 && s2 && *s2)	result = result+"fog "+s1+' '+s2+'\n';
		else if(s1 && *s1)	result = result+"font "+s1+"\n";

		if(alpha->value())	result = result+"alpha on\n";
		if(light->value())	result = result+"light on\n";
		if(cut->value())	result = result+"cut on\n";
		if(attach->value())	result = result+"attachlight on\n";
		if(gray->value())	result = result+"gray on\n";
		if(rotatetext->value())	result = result+"rotatetext on\n";
		if(origintick->value())	result = result+"origintick off\n";
		if(variant->value()>0)
		{	char buf[32];	snprintf(buf,31,"variant %ld\n",mgl_int(variant->value()));	result += buf;	}
		const char *stime[4]={"''\n","'x'\n","'y'\n","'z'\n"};
		if(time->value()>0)	result = result+"timetick "+stime[time->value()];
		const char *stune[4]={" 0\n"," 1\n"," 2\n"," 3\n"};
		if(tunetick->value()>0)	result = result+"tunetick "+stune[tunetick->value()];
		const char *stern[6]={" 0\n"," 1\n"," 2\n"," 4\n"," 5\n"," 6\n"};
		if(ternary->value()>0)	result = result+"ternary "+stern[ternary->value()];
		const char *stype[3]={" 0\n"," 1\n"," 2\n"};
		if(transptype->value()>0)	result = result+"ternary "+stype[transptype->value()];
		for(int i=0;i<10;i++)
		{
			if(!lb[i]->value())	continue;
			s1 = lx[i]->value();	s2 = ly[i]->value();
			s3 = lz[i]->value();	s4 = lbr[i]->value();
			char col = cols[lc[i]->value()];	if(col==' ')	col='w';
			if(s1 && *s1 && s2 && *s2 && s3 && *s3)
			{
				result = result+"light "+char('0'+i)+' '+s1+' '+s2+' '+s3+" '"+col+'\'';
				if(s4 && *s4)	result = result+' '+s4;
				result += '\n';
			}
		}
	}
	void cb_ok()
	{
		prepare();
		if(e)	{	e->draw->script=result;	e->graph->update();	}
		hide();
	}
	void save()
	{
		prepare();
		const char *s=fname->value();
		if(s && *s)
		{
			FILE *fp=fopen(s,"wt");
			if(fp)	{	fputs(result.c_str(),fp);	fclose(fp);	}
			else	fl_alert(mgl_gettext("Couldn't open file %s"),s);
		}
		else	textbuf->insert(0,result.c_str());
	}
} setup_dlg;
//-----------------------------------------------------------------------------
void setup_dlg_cb(Fl_Widget*,void *v)
{	setup_dlg.e = (ScriptWindow*)v;	setup_dlg.show();	}
//-----------------------------------------------------------------------------
void cb_setup_save(Fl_Widget*,void *v)	{	setup_dlg.save();	}
//-----------------------------------------------------------------------------
