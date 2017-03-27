/* main.cpp is part of UDAV
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
#include <ctype.h>
#include <errno.h>
#include <locale.h>
#include <getopt.h>
#include "mgllab.h"
//-----------------------------------------------------------------------------
#ifndef MGL_DOC_DIR
#ifdef WIN32
#define MGL_DOC_DIR ""
#else
#define MGL_DOC_DIR "/usr/local/share/doc/mathgl/"
#endif
#endif
//-----------------------------------------------------------------------------
//int num_windows = 0, auto_exec=1, plastic_scheme=1, internal_font=0;
int num_windows = 0;
int auto_exec;
int exec_save;
int highlight;
int mouse_zoom;
std::string docdir;
std::string fontname;
int lang;
int scheme;
std::string lastfiles[5];
Fl_Preferences pref(Fl_Preferences::USER,"abalakin","mgllab");
//-----------------------------------------------------------------------------
void set_scheme_lang(int s, int l)
{
	static const char *sch[4]={"base","gtk+","plastic","gleam"};
	static const char *loc[3]={"C.UTF8",	"ru_RU.utf8",	"ru_RU.cp1251"};
	static const char *hlp[3]={"doc_en.html","doc_ru.html",	"doc_ru.html"};
	if(s<0 || s>3)	s=1;
	if(l<0 || l>2)	l=1;
	setlocale(LC_CTYPE, loc[l]);
	Fl::scheme(sch[s]);
	scheme = s;	lang = l;
	// TODO reload help file!!!
}
//-----------------------------------------------------------------------------
void save_pref()
{
	pref.set("locale",lang);
	pref.set("scheme",scheme);
	pref.set("help_dir",docdir.c_str());
	pref.set("auto_exec",auto_exec);
	pref.set("exec_save",exec_save);
	pref.set("highlight",highlight);
	pref.set("mouse_zoom",mouse_zoom);
	pref.set("font_kind",font_kind);
	pref.set("font_size",font_size);
	pref.set("font_name",fontname.c_str());
	pref.set("fname1",lastfiles[0].c_str());
	pref.set("fname2",lastfiles[1].c_str());
	pref.set("fname3",lastfiles[2].c_str());
	pref.set("fname4",lastfiles[3].c_str());
	pref.set("fname5",lastfiles[4].c_str());
}
//-----------------------------------------------------------------------------
void load_pref(ScriptWindow *w)
{
	static char *s;
	pref.get("locale",lang,1);
	pref.get("scheme",scheme,2);
	set_scheme_lang(scheme,lang);
	// doc should be freed before next call
	pref.get("help_dir",s,MGL_DOC_DIR);
	if(s)	{	docdir=s;	free(s);	}
	pref.get("auto_exec",auto_exec,1);
	pref.get("exec_save",exec_save,1);
	pref.get("highlight",highlight,1);
	pref.get("mouse_zoom",mouse_zoom,0);
	pref.get("font_kind",font_kind,1);
	pref.get("font_size",font_size,14);
	set_style(font_kind, font_size);
	pref.get("font_name",s,"");
	if(s)	{	fontname=s;	free(s);	}
	if(w && w->graph)
		mgl_load_font(w->graph->get_graph(),fontname.c_str(),NULL);
	pref.get("fname1",s,"");	if(s)	{	lastfiles[0]=s;	free(s);	}
	pref.get("fname2",s,"");	if(s)	{	lastfiles[1]=s;	free(s);	}
	pref.get("fname3",s,"");	if(s)	{	lastfiles[2]=s;	free(s);	}
	pref.get("fname4",s,"");	if(s)	{	lastfiles[3]=s;	free(s);	}
	pref.get("fname5",s,"");	if(s)	{	lastfiles[4]=s;	free(s);	}
}
//-----------------------------------------------------------------------------
void set_title(Fl_Window* w)
{
	std::string title;
	if (filename.empty()) title="Untitled";
	else
	{
		size_t sep = filename.find_last_of('/');
		#ifdef WIN32
		if(sep==std::string::npos)
			sep = filename.find_last_of('\\');
		#endif
		if(sep!=std::string::npos)	title = filename.substr(sep+1);
		else	title = filename;
	}
	if(changed)	title += " *";
	title = title + " - mgllab";
	w->label(title.c_str());
}
//-----------------------------------------------------------------------------
void fname_cb(Fl_Widget*, void *v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	char *file = fl_file_chooser(mgl_gettext("Insert File Name?"), mgl_gettext("All Files (*)"), 0);
	if(file)
	{
		char *str = new char[strlen(file)+4];
		snprintf(str,strlen(file)+4," '%s'",file);
		e->editor->insert(str);
		delete []str;
	}
}
//-----------------------------------------------------------------------------
void new_cb(Fl_Widget*, void*)
{
	if (!check_save()) return;
	filename[0] = '\0';
	textbuf->select(0, textbuf->length());
	textbuf->remove_selection();
	changed = 0;
	textbuf->call_modify_callbacks();
}
//-----------------------------------------------------------------------------
void open_cb(Fl_Widget*, void *v)
{
	if (!check_save()) return;
	char *lastname=0;
	if(filename.empty())	{	pref.get("last_file",lastname,"");	filename=lastname;	}
	char *newfile = fl_file_chooser(mgl_gettext("Open File?"),
		mgl_gettext("MGL files (*.mgl)\tDAT files (*.{dat,csv})\tAll files (*)"), filename.c_str());
	if(lastname)	free(lastname);
	if(newfile != NULL)
	{
		load_file(newfile, -1);
		if(auto_exec)	((ScriptWindow*)v)->graph->update();
	}
}
//-----------------------------------------------------------------------------
void close_cb(Fl_Widget*, void* v)
{
	Fl_Window* w = (Fl_Window*)v;
	if (num_windows == 1 && !check_save())	return;

	w->hide();
	textbuf->remove_modify_callback(changed_cb, w);
	delete w;
	num_windows--;
	if (!num_windows) exit(0);
}
//-----------------------------------------------------------------------------
void quit_cb(Fl_Widget*, void*)
{
	if (changed && !check_save())	return;
	exit(0);
}
//-----------------------------------------------------------------------------
void save_cb(Fl_Widget*w, void*v)
{
	if(filename.empty())	{	saveas_cb(w,v);	return;	}	// No filename - get one!
	else save_file(filename.c_str());
}
//-----------------------------------------------------------------------------
void saveas_cb(Fl_Widget*, void*)
{
	char *newfile, *fname=0;
	FILE *fp=0;
	while(1)
	{
		newfile = fl_file_chooser(mgl_gettext("Save File As?"), "*.mgl", filename.c_str());
		if(!newfile || !newfile[0])	break;
		if(!strchr(newfile,'.'))
		{
			if(fname)	delete []fname;
			fname = new char[strlen(newfile)+5];
			strcpy(fname,newfile);	strcat(fname,".mgl");
			newfile = fname;
		}
		fp = fopen(newfile,"r");
		if(fp)
		{
			fclose(fp);
			if(fl_choice(mgl_gettext("File is exesist. Overwrite it?"),0,mgl_gettext("No"),mgl_gettext(" Yes "))==2)
				break;
		}
		else	break;
	}
	if (newfile != NULL)	save_file(newfile);
	if(fname)	delete []fname;
}
//-----------------------------------------------------------------------------
ScriptWindow *new_view();
void view_cb(Fl_Widget*, void*)
{	Fl_Window* w = new_view();	w->show();	}
//-----------------------------------------------------------------------------
void hint_cb(Fl_Widget*, void*)	{}
//-----------------------------------------------------------------------------
Fl_Menu_Item menuitems[] = {
	{"File", 0,  0, 0, FL_SUBMENU},
		{"New script", 0,  new_cb},
		{"Open file ...", FL_CTRL+'o',  open_cb},
		{"Save file", FL_CTRL+'s',  save_cb},
		{"Save as ...", 0,  saveas_cb, 0, FL_MENU_DIVIDER},
		// TODO{"Print plot", 0,  0, 0, FL_MENU_DIVIDER},
		{"Recent files", 0,  0, 0, FL_SUBMENU|FL_MENU_DIVIDER},
			{"1.", 0,  0},
			{"2.", 0,  0},
			{"3.", 0,  0},
			{"4.", 0,  0},
			{"5.", 0,  0},
			{0},
		{"Exit", 0,  quit_cb},
		{0},
	{"Edit", 0,  0, 0, FL_SUBMENU},
		{"Undo", FL_CTRL+'z',  0},
		{"Redo", FL_CTRL+FL_SHIFT+'z',  0, 0, FL_MENU_DIVIDER},
		{"Cut text", FL_CTRL+'x',  cut_cb},
		{"Copy text", FL_CTRL+'c',  copy_cb},
		{"Paste text", FL_CTRL+'v',  paste_cb},
		{"Select all", FL_CTRL+'a',  0, 0, FL_MENU_DIVIDER},
		{"Find|Replace", FL_CTRL+'f',  0},
		{"Find next", FL_F+3,  find2_cb, 0, FL_MENU_DIVIDER},
		{"Insert", 0,  0, 0, FL_SUBMENU},
			{"File path", FL_META+'p',  ins_path_cb},
			{"Folder path", 0,  ins_fname_cb},
			// TODO{"Command", FL_META+'c',  0},
			// TODO{"Inplot", FL_META+'i',  0},
			{"Fitted formula", FL_META+'f',  ins_fits_cb},
			{"Plot style", 0,  style_dlg_cb},
			{"Option(s)", FL_META+'o',  option_dlg_cb},
			{0},
		{0},
	{"Graphics", 0,  0, 0, FL_SUBMENU},
		{0},
	{"Setup", 0,  0, 0, FL_SUBMENU},
		{"Properties", 0,  prop_dlg_cb},
		{"Set arguments", 0,  args_dlg_cb},
		// TODO{"Plot setup", FL_META+'g',  0, 0, FL_MENU_DIVIDER},
		{"Calculator", FL_F+4,  calc_dlg_cb},
		// TODO{"Messages", FL_F+2,  0, 0, FL_MENU_TOGGLE},
		{0},
	{"Help", 0,  0, 0, FL_SUBMENU},
		{"Help", FL_F+1,  help_cb},
		// TODO{"Hints", 0,  0},
		// TODO{"About", 0,  0},
		{0},
	{0}
};
//-----------------------------------------------------------------------------
void mem_upd_cb(Fl_Widget *, void *v)
{	((ScriptWindow*)v)->mem_init();	}
//-----------------------------------------------------------------------------
ScriptWindow *new_view()
{
	Fl_Tabs* tt;
	Fl_Group *gg;
	ScriptWindow *w = new ScriptWindow(930, 510, mgl_gettext("Untitled - mgllab"));
	w->begin();
	w->menu = new Fl_Menu_Bar(0, 0, 930, 30);
	w->menu->copy(menuitems, w);

	Fl_Tile *t = new Fl_Tile(0,30,930,455);
	tt = new Fl_Tabs(0,30,300,455,0);	w->ltab = tt;
	gg = new Fl_Group(0,30,300,430);	gg->label(mgl_gettext("Script"));
	add_editor(w);	gg->end();
	tt->end();

	tt = new Fl_Tabs(300,30,630,455,0);	w->rtab = tt;
	gg = new Fl_Group(300,30,630,430,mgl_gettext("Canvas"));
	w->graph = new Fl_MGLView(300,30,630,430);	gg->end();
	gg = new Fl_Group(300,30,630,430,mgl_gettext("Help"));
	w->ghelp = gg;	add_help(w);	gg->end();
	gg = new Fl_Group(300,30,630,430,mgl_gettext("Memory"));
	add_mem(w);		gg->end();
	tt->end();

	w->status = new Fl_Box(0,485,930,25,"Ready");
	w->status->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	w->status->color(FL_BACKGROUND_COLOR);
	w->status->box(FL_DOWN_BOX);
	w->draw = new Fl_MGL(w->graph);
	w->draw->status = w->status;
	mgl_makemenu_fltk(w->menu, w->graph);
	
	t->end();	w->end();	w->resizable(t);
	tt->callback(mem_upd_cb, w);
	tt->value(w->graph);
	w->callback((Fl_Callback *)close_cb, w);

	num_windows++;
	return w;
}
//-----------------------------------------------------------------------------
void argument_set(int n, const char *s);
int main(int argc, char **argv)
{
//	Fl::lock();
	mgl_ask_func = mgl_ask_fltk;

#ifdef USE_GETTEXT
//	setlocale (LC_NUMERIC, "");
//	bindtextdomain (PACKAGE, LOCALEDIR);
//	textdomain (PACKAGE);
#endif

	textbuf = new Fl_Text_Buffer;
	style_init();
	ScriptWindow *w = new_view();
	Fl::visual(FL_DOUBLE|FL_RGB);
	load_pref(w);

	char *buf = 0;
	while(1)
	{
		char ch = getopt(argc, argv, "1:2:3:4:5:6:7:8:9:ho:L:");
		if(ch>='1' && ch<='9')	argument_set(ch-'0', optarg);
		else if(ch=='L')	setlocale(LC_CTYPE, optarg);
		else if(ch=='h')
		{
			printf("mglconv convert mgl script to bitmap png file.\nCurrent version is 2.%g\n",MGL_VER2);
			printf("Usage:\tmgllab [parameter(s)] scriptfile\n");
			printf(	"\t-1 str       set str as argument $1 for script\n"
					"\t...          ...\n"
					"\t-9 str       set str as argument $9 for script\n"
					"\t-L loc       set locale to loc\n"
//					"\t-            get script from standard input\n"
					"\t-h           print this message\n" );
			return 0;
		}
		// NOTE: I will not parse stdin here
		else if(ch==-1 && optind<argc)	buf = argv[optind];
		else if(ch==-1 && optind>=argc)	break;
	}

	w->show(1, argv);
	if(buf && *buf && *buf!='-')
	{
		load_file(buf, -1);
		if(auto_exec)	w->graph->update();
	}
	return Fl::run();
}
//-----------------------------------------------------------------------------
