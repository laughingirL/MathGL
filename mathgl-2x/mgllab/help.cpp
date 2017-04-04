/* help.cpp is part of UDAV
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
#include "mgllab.h"
#include <ctype.h>
#include <FL/Fl_Select_Browser.H>
//-----------------------------------------------------------------------------
void help_cb(Fl_Widget*, void*v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	int cur = e->editor->insert_position(), br=0;
	int beg = textbuf->line_start(cur);
	const char *s = textbuf->text();
	for(int i=beg;i<cur;i++)
	{
		if(strchr("({[",s[i]))	br++;
		if(strchr(")}]",s[i]))	br--;
		if(br==0 && s[i]==':' && i+1<cur)	beg=i+1;
	}
	for(br=beg;s[br]>' ' && s[br]!=':';br++);
	std::string cmd(s+beg,br-beg);
	e->link_cmd->value(cmd.c_str());
	static std::string str;
	str = helpname+cmd;
	e->hd->load(str.c_str());
	if(e->rtab)	e->rtab->value(e->ghelp);
}
//-----------------------------------------------------------------------------
void link_cb(Fl_Widget*, void*v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	const char *s = e->link_cmd->value();
	if(!s)	s="Examples";
	static std::string str;
	str = helpname+s;
	e->hd->load(str.c_str());
	if(e->rtab)	e->rtab->value(e->ghelp);
}
//-----------------------------------------------------------------------------
void example_cb(Fl_Widget*, void*v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	static std::string str = helpname+"Examples";
	e->hd->load(str.c_str());	e->rtab->value(e->ghelp);
	if(e->rtab)	e->rtab->value(e->ghelp);
}
//-----------------------------------------------------------------------------
void help_in_cb(Fl_Widget*, void*v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	e->hd->textsize(e->hd->textsize()+1);
}
//-----------------------------------------------------------------------------
void help_out_cb(Fl_Widget*, void*v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	e->hd->textsize(e->hd->textsize()-1);
}
//-----------------------------------------------------------------------------
#include "xpm/udav.xpm"
void about_cb(Fl_Widget*, void*)
{
	static char s[128];
	snprintf(s,128,mgl_gettext("mgllab v. 2.%g\n(c) Alexey Balakin, 2017\nhttp://mathgl.sf.net/"), MGL_VER2);
	Fl_Double_Window* w = new Fl_Double_Window(355, 130, "About UDAV");
	Fl_Box* o = new Fl_Box(10, 15, 65, 65);
	o->box(FL_UP_BOX);	o->color(55);	o->image(new Fl_Pixmap(udav_xpm));
	o = new Fl_Box(85, 15, 260, 65);	o->label(s);
	Fl_Button *b = new Fl_Return_Button(255, 90, 90, 30, "Close");
	b->callback(close_dlg_cb,w);
	w->end();	w->set_modal();	w->show();
}
//-----------------------------------------------------------------------------
#include "xpm/zoom-out.xpm"
#include "xpm/zoom-in.xpm"
#include "xpm/help-faq.xpm"
Fl_Widget *add_help(ScriptWindow *w)
{
	Fl_Window *w1=new Fl_Window(300,30,630,430,0);
	Fl_Group *g = new Fl_Group(0,0,290,30);
	Fl_Button *o;

	w->link_cmd = new Fl_Input(0,1,150,25);
	w->link_cmd->when(FL_WHEN_CHANGED);
	w->link_cmd->callback(link_cb,w);

	o = new Fl_Button(155, 1, 25, 25);	o->tooltip(mgl_gettext("MGL samples and hints"));
	o->image(new Fl_Pixmap(help_faq_xpm));	o->callback(example_cb,w);
	o = new Fl_Button(180, 1, 25, 25);	o->tooltip(mgl_gettext("Increase font size"));
	o->image(new Fl_Pixmap(zoom_in_xpm));	o->callback(help_in_cb,w);
	o = new Fl_Button(205, 1, 25, 25);	o->tooltip(mgl_gettext("Decrease font size"));
	o->image(new Fl_Pixmap(zoom_out_xpm));	o->callback(help_out_cb,w);

	g->end();	g->resizable(0);

	w->hd = new Fl_Help_View(0,28,630,400);
	w1->end();	link_cb(w,w);
	w1->resizable(w->hd);	return w1;
}
//-----------------------------------------------------------------------------
void mem_dlg_cb0(Fl_Widget *, void *v)
{	((ScriptWindow*)v)->mem_pressed(0);	}
//-----------------------------------------------------------------------------
void mem_dlg_cb1(Fl_Widget *, void *v)
{	((ScriptWindow*)v)->mem_pressed(1);	}
//-----------------------------------------------------------------------------
void mem_dlg_cb2(Fl_Widget *, void *v)
{	((ScriptWindow*)v)->mem_pressed(2);	}
//-----------------------------------------------------------------------------
void mem_dlg_cb3(Fl_Widget *, void *v)
{	((ScriptWindow*)v)->mem_pressed(3);	}
//-----------------------------------------------------------------------------
void mem_update_cb(Fl_Widget *, void *v)
{	((ScriptWindow*)v)->mem_init();	}
//-----------------------------------------------------------------------------
Fl_Widget *add_mem(ScriptWindow *w)
{
	static int widths[] = {220,205,0};
	Fl_Button *o;
	Fl_Box *b;
	Fl_Window *wnd = new Fl_Window(300,30,630,430,0);

	b = new Fl_Box(0, 10, 630, 25, mgl_gettext("Existed data arrays"));	b->labeltype(FL_ENGRAVED_LABEL);
	b = new Fl_Box(0, 35, 220, 25, mgl_gettext("name"));
	b->box(FL_THIN_UP_BOX);	b->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	b = new Fl_Box(220, 35, 205, 25, mgl_gettext("dimensions"));
	b->box(FL_THIN_UP_BOX);	b->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	b = new Fl_Box(425, 35, 205, 25, mgl_gettext("mem. usage"));
	b->box(FL_THIN_UP_BOX);	b->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

	w->var = new Fl_Select_Browser(0, 60, 630, 335);	w->var->column_char('\t');
	w->var->align(FL_ALIGN_TOP);	w->var->column_widths(widths);
	w->var->tooltip(mgl_gettext("List of available data."));

	o = new Fl_Button(10, 400, 95, 25, mgl_gettext("Edit"));	o->callback(mem_dlg_cb0,w);
	o->tooltip(mgl_gettext("Open table with selected data for editing."));
	o = new Fl_Button(120, 400, 95, 25, mgl_gettext("Info"));	o->callback(mem_dlg_cb1,w);
	o->tooltip(mgl_gettext("Data information and preview."));
	o = new Fl_Button(230, 400, 95, 25, mgl_gettext("Delete"));	o->callback(mem_dlg_cb2,w);
	o->tooltip(mgl_gettext("Delete selected data."));
	o = new Fl_Button(340, 400, 95, 25, mgl_gettext("New"));	o->callback(mem_dlg_cb3,w);
	o->tooltip(mgl_gettext("Open dialog for new data creation."));
	o = new Fl_Button(450, 400, 95, 25, mgl_gettext("Refresh"));	o->callback(mem_update_cb,w);
	o->tooltip(mgl_gettext("Refresh list of variables."));
//	o = new Fl_Button(120, 335, 95, 25, mgl_gettext("Load"));	o->callback(mem_dlg_cb,(void *)4);
//	o = new Fl_Button(230, 335, 95, 25, mgl_gettext("Save"));	o->callback(mem_dlg_cb,(void *)5);
//	o = new Fl_Button(10, 370, 95, 25, mgl_gettext("Update"));	o->callback(mem_upd_cb,0);
	wnd->end();	wnd->resizable(w->var);	return wnd;
}
//-----------------------------------------------------------------------------
void ScriptWindow::mem_init()
{
	char str[128];
	var->clear();
	for(long i=0;i<Parse->GetNumVar();i++)
	{
		mglDataA *v = Parse->GetVar(i);
		if(v && !v->temp)
		{
			long sv = 0;
			if(dynamic_cast<mglData*>(v))	sv = v->GetNN()*sizeof(mreal)+sizeof(mglData);
			else if(dynamic_cast<mglDataC*>(v))	sv = v->GetNN()*sizeof(dual)+sizeof(mglDataC);
			else if(dynamic_cast<mglDataV*>(v))	sv = sizeof(mglDataV);
			else if(dynamic_cast<mglDataW*>(v))	sv = sizeof(mglDataW);
			else if(dynamic_cast<mglDataF*>(v))	sv = sizeof(mglDataF);
			else if(dynamic_cast<mglDataR*>(v))	sv = sizeof(mglDataR);
			else if(dynamic_cast<mglDataT*>(v))	sv = sizeof(mglDataT);
			const char *ext[]={mgl_gettext("unknown"),"b","Kb","Mb","Gb","Tb","Pb","Eb","Zb","Yb"}, *e;
			if(sv==0)	e = ext[0];
#if MGL_SIZEOF_LONG>4
//			else if((sv>>80L)>0)	{	e=ext[9];	sv = sv>>80L;	}
//			else if((sv>>70L)>0)	{	e=ext[8];	sv = sv>>70L;	}
			else if((sv>>60L)>0)	{	e=ext[7];	sv = sv>>60L;	}
			else if((sv>>50L)>0)	{	e=ext[6];	sv = sv>>50L;	}
			else if((sv>>40L)>0)	{	e=ext[5];	sv = sv>>40L;	}
#endif
			else if((sv>>30L)>0)	{	e=ext[4];	sv = sv>>30L;	}
			else if((sv>>20L)>0)	{	e=ext[3];	sv = sv>>20L;	}
			else if((sv>>10L)>0)	{	e=ext[2];	sv = sv>>10L;	}
			else	e=ext[1];
			snprintf(str,128,"%ls\t%ld*%ld*%ld\t%ld %s", v->s.c_str(), v->GetNx(), v->GetNy(), v->GetNz(), sv, e);
			var->add(str,v);
		}
	}
	for(long i=0;i<Parse->GetNumConst();i++)
	{
		mglNum *v = Parse->GetConst(i);
		snprintf(str,128,"%ls\t%s\t%lu b", v->s.c_str(), (mgl_gettext("const=")+mgl_str_num(v->c)).c_str(), sizeof(mglNum));
		var->add(str,v);
	}
}
//-----------------------------------------------------------------------------
void ScriptWindow::mem_pressed(int kind)
{
	TableWindow *w;
	int ind = var->value();
	mglDataA *v = (mglDataA *)var->data(ind);
	if(!v && kind!=3)	return;
	if(kind==0)
	{
		w = (TableWindow *)v->o;
		if(!w)	w = new TableWindow(this);
		w->update(v);	w->show();
	}
	else if(kind==1)
	{
		info_dlg_cb(v);
// 	static char res[128];
// 		const wchar_t *s=v->s.c_str();
// 		if(v->GetNz()>1)		snprintf(res,128,"crange %ls:rotate 40 60:box:surf3 %ls\n", s,s);
// 		else if(v->GetNy()>1)	snprintf(res,128,"zrange %ls:rotate 40 60:box:surf %ls\n", s,s);
// 		else				snprintf(res,128,"yrange %ls:box:plot %ls\n", s,s);
// 		textbuf->text(res);
	}
	else if(kind==2)
		Parse->DeleteVar(v->s.c_str());
	else if(kind==3)
	{
		const char *name = fl_input(mgl_gettext("Enter name for new variable"),"dat");
		if(!name)	return;
		v = Parse->AddVar(name);
		w = v->o? (TableWindow*)v->o:new TableWindow(this);
		w->update(v);	w->show();
	}
	mem_init();
}
//-----------------------------------------------------------------------------
const char *hints[] = {
	mgl_gettext("You can shift axis range by pressing middle button and moving mouse. Also, you can zoom in/out axis range by using mouse wheel."),
	mgl_gettext("You can rotate/shift/zoom whole plot by mouse. Just press 'Rotate' toolbutton, click image and hold a mouse button: left button for rotation, right button for zoom/perspective, middle button for shift."),
	mgl_gettext("You may quickly draw the data from file. Just use: udav 'filename.dat' in command line."),
	mgl_gettext("You can copy the current image to clipboard by pressing Ctrl-Shift-C. Later you can paste it directly into yours document or presentation."),
	mgl_gettext("You can export image into a set of format (EPS, SVG, PNG, JPEG) by pressing right mouse button inside image and selecting 'Export as ...'."),
	mgl_gettext("You can setup colors for script highlighting in Property dialog. Just select menu item 'Settings/Properties'."),
	mgl_gettext("You can save the parameter of animation inside MGL script by using comment started from '##a ' or '##c ' for loops."),
	mgl_gettext("New drawing never clears things drawn already. For example, you can make a surface with contour lines by calling commands 'surf' and 'cont' one after another (in any order). "),
	mgl_gettext("You can put several plots in the same image by help of commands 'subplot' or 'inplot'."),
	mgl_gettext("All indexes (of data arrays, subplots and so on) are always start from 0."),
	mgl_gettext("You can edit MGL file in any text editor. Also you can run it in console by help of commands: mglconv, mglview."),
	mgl_gettext("You can use command 'once on|off' for marking the block which should be executed only once. For example, this can be the block of large data reading/creating/handling. Press F9 (or menu item 'Graphics/Reload') to re-execute this block."),
	mgl_gettext("You can use command 'stop' for terminating script parsing. It is useful if you don't want to execute a part of script."),
	mgl_gettext("You can type arbitrary expression as input argument for data or number. In last case (for numbers), the first value of data array is used."),
	mgl_gettext("There is powerful calculator with a lot of special functions. You can use buttons or keyboard to type the expression. Also you can use existed variables in the expression."),
	mgl_gettext("The calculator can help you to put complex expression in the script. Just type the expression (which may depend on coordinates x,y,z and so on) and put it into the script."),
	mgl_gettext("You can easily insert file or folder names, last fitted formula or numerical value of selection by using menu Edit|Insert."),
	mgl_gettext("The special dialog (Edit|Insert|New Command) help you select the command, fill its arguments and put it into the script."),
	mgl_gettext("You can put several plotting commands in the same line or in separate function, for highlighting all of them simultaneously."),
	NULL
};
//-----------------------------------------------------------------------------
void cb_hint_prev(Fl_Widget*,void*);
void cb_hint_next(Fl_Widget*,void*);
class HintDlg : public GeneralDlg
{
	Fl_Help_View *hint;
	Fl_Check_Button *start;
	int cur;
public:
	HintDlg()
	{
		Fl_Button *o;
		w = new Fl_Double_Window(280, 265);	cur=0;
		hint = new Fl_Help_View(10, 10, 260, 185);
		hint->value(hints[0]);
		start = new Fl_Check_Button(10, 200, 260, 25, mgl_gettext("Show hint on startup"));
		o = new Fl_Button(10, 230, 80, 25, mgl_gettext("@<-  Prev"));
		o->callback(cb_hint_prev);
		o = new Fl_Button(100, 230, 80, 25, mgl_gettext("Next @->"));
		o->callback(cb_hint_next);
		o = new Fl_Return_Button(190, 230, 80, 25, mgl_gettext("Close"));
		o->callback(cb_dlg_ok,this);
		w->end();
	}
	void init()
	{	int sh;	pref.get("show_hint",sh,1);	start->value(sh);	}
	void cb_ok()
	{	pref.set("show_hint",start->value());	hide();	}
	void prev()
	{
		int n=0;	while(hints[n])	n++;
		cur = cur>0?cur-1:n-1;
		hint->value(hints[cur]);
	}
	void next()
	{
		int n=0;	while(hints[n])	n++;
		cur = cur<n-1?cur+1:0;
		hint->value(hints[cur]);
	}
} hint_dlg;
//-----------------------------------------------------------------------------
void cb_hint_prev(Fl_Widget*,void*)	{	hint_dlg.prev();	}
void cb_hint_next(Fl_Widget*,void*)	{	hint_dlg.next();	}
//-----------------------------------------------------------------------------
void hint_dlg_cb(Fl_Widget*,void *)	{	hint_dlg.show();	}
//-----------------------------------------------------------------------------
void cb_info_prev(Fl_Widget*,void*);
void cb_info_next(Fl_Widget*,void*);
void cb_info_1d(Fl_Widget*,void*);
void cb_info_2d(Fl_Widget*,void*);
void cb_info_3d(Fl_Widget*,void*);
class InfoDlg : public GeneralDlg
{
	Fl_Multiline_Output *out;
	Fl_MathGL *gr;
	long nx, ny, nz;
	long sl;
	int plot=0;
	std::string name;
public:
	InfoDlg()
	{
		Fl_Button *o;
		w = new Fl_Double_Window(420, 530);
		out = new Fl_Multiline_Output(10, 25, 400, 150, mgl_gettext("Information"));
		out->align(FL_ALIGN_TOP_LEFT);
		gr = new Fl_MathGL(10, 220, 400, 300);	gr->box(FL_ENGRAVED_BOX);
		mgl_set_size(gr->get_graph(),400,300);
		o = new Fl_Button(10, 185, 25, 25, "@<-");
		o->callback(cb_info_prev,this);
		o = new Fl_Button(40, 185, 75, 25, mgl_gettext("1D view"));
		o->callback(cb_info_1d,this);
		o = new Fl_Button(120, 185, 75, 25, mgl_gettext("2D view"));
		o->callback(cb_info_2d,this);
		o = new Fl_Button(200, 185, 75, 25, mgl_gettext("3D view"));
		o->callback(cb_info_3d,this);
		o = new Fl_Button(280, 185, 25, 25, "@->");
		o->callback(cb_info_next,this);
		o = new Fl_Return_Button(335, 185, 75, 25, mgl_gettext("Close"));
		o->callback(cb_dlg_cancel,this);
		w->set_modal();	w->end();
	}
	void update()
	{
		if(!dat)	return;
		std::string script;
		char buf[32];
		switch(plot)
		{
		case 0:
			if(sl<0)	sl=ny-1;
			if(sl>=ny)	sl=0;
			snprintf(buf,31,"%ld",sl);
			script = "subplot 1 1 0 '<_':xrange 0 1:yrange " + name + ":plot " + name + "(:," + buf + "):axis:box:text 1 1 'sl=" + buf + "' 'r:aR'";
			break;
		case 1:
			if(sl<0)	sl=nz-1;
			if(sl>=nz)	sl=0;
			snprintf(buf,31,"%ld",sl);
			script = "subplot 1 1 0 '':crange " + name + ":dens " + name + "(:,:," + buf + "):box:text 1 1 'sl=" + buf + "' 'r:aR'";
			break;
		case 2:
			script = "rotate 40 60:light on:crange " + name + ":surf3 " + name + ":box";
			break;
		}
		script = "clf:"+script;
		mgl_set_def_param(gr->get_graph());
		mgl_parse_text(gr->get_graph(), Parse->Self(), script.c_str());
		gr->update();
	}
	void prev()		{	sl--;	update();	}
	void next()		{	sl++;	update();	}
	void plot_1d()	{	plot=0;	update();	}
	void plot_2d()	{	if(ny>1)	{	plot=1;	update();	}	}
	void plot_3d()	{	if(nz>1)	{	plot=2;	update();	}	}
	void init()
	{
		nx=ny=nz=sl=plot=0;
		if(dat)
		{
			nx=dat->GetNx();	ny=dat->GetNy();	nz=dat->GetNz();
			result = dat->PrintInfo();	out->value(result.c_str());
			name = wcstombs(dat->s);
			if(nz>1)		plot_3d();
			else if(ny>1)	plot_2d();
			else			plot_1d();
		}
	}
} info_dlg;
//-----------------------------------------------------------------------------
void info_dlg_cb(mglDataA *d)
{	if(d)	{	info_dlg.dat=d;	info_dlg.show();	}	}
//-----------------------------------------------------------------------------
void cb_info_prev(Fl_Widget*,void*)	{	info_dlg.prev();	}
void cb_info_next(Fl_Widget*,void*)	{	info_dlg.next();	}
void cb_info_1d(Fl_Widget*,void*)	{	info_dlg.plot_1d();	}
void cb_info_2d(Fl_Widget*,void*)	{	info_dlg.plot_2d();	}
void cb_info_3d(Fl_Widget*,void*)	{	info_dlg.plot_3d();	}
//-----------------------------------------------------------------------------
