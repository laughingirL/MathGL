/* main.cpp is part of UDAV
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <locale.h>
#include "udav.h"
//-----------------------------------------------------------------------------
char	title[256];
int num_windows = 0, auto_exec=1, plastic_scheme=1, internal_font=0;
Fl_Preferences pref(Fl_Preferences::USER,"abalakin","UDAV");
//-----------------------------------------------------------------------------
void set_title(Fl_Window* w)
{
	if (filename[0] == '\0') strcpy(title, "Untitled");
	else
	{
		char *slash;
		slash = strrchr(filename, '/');
#ifdef WIN32
		if (slash == NULL) slash = strrchr(filename, '\\');
#endif
		if (slash != NULL) strcpy(title, slash + 1);
		else strcpy(title, filename);
	}
	if (changed) strcat(title, gettext(" (modified)"));
	w->label(title);
}
//-----------------------------------------------------------------------------
void fname_cb(Fl_Widget*, void *v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	char *file = fl_file_chooser(gettext("Insert File Name?"), gettext("All Files (*)"), 0);
	if(file)
	{
		char *str = new char[strlen(file)+4];
		sprintf(str," '%s'",file);
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
	if(*filename==0)	{	pref.get("last_file",lastname,"");	strcpy(filename, lastname);	}
	char *newfile = fl_file_chooser(gettext("Open File?"),
		gettext("MGL Files (*.mgl)\tDAT Files (*.{dat,csv})\tAll Files (*)"), filename);
	if(lastname)	free(lastname);
	if(newfile != NULL)
	{
		load_file(newfile, -1);
		if(auto_exec)	((ScriptWindow*)v)->update_pre();
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
	if (filename[0] == '\0')	{	saveas_cb(w,v);	return;	}	// No filename - get one!
	else save_file(filename);
}
//-----------------------------------------------------------------------------
void saveas_cb(Fl_Widget*, void*)
{
	char *newfile, *fname=0;
	FILE *fp=0;
	while(1)
	{
		newfile = fl_file_chooser(gettext("Save File As?"), "*.mgl", filename);
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
			if(fl_choice(gettext("File is exesist. Overwrite it?"),0,gettext("No"),gettext(" Yes "))==2)
				break;
		}
		else	break;
	}
	if (newfile != NULL)	save_file(newfile);
	if(fname)	delete []fname;
}
//-----------------------------------------------------------------------------
ScriptWindow *new_view();
//-----------------------------------------------------------------------------
void view_cb(Fl_Widget*, void*)
{	Fl_Window* w = new_view();	w->show();	}
//-----------------------------------------------------------------------------
void hint_cb(Fl_Widget*, void*)	{}
//-----------------------------------------------------------------------------
void adjust_cb(Fl_Widget*, void*v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	e->graph->size(e->scroll->w(),e->scroll->h());
}
//-----------------------------------------------------------------------------
Fl_Menu_Item menuitems[] = {
	{ gettext("File"), 0, 0, 0, FL_SUBMENU },
		{ gettext("New File"),			0, new_cb },
		{ gettext("Open File..."),		FL_CTRL + 'o', open_cb },
		{ gettext("Insert File..."),	FL_CTRL + 'i', insert_cb },
		{ gettext("Save File"),			FL_CTRL + 's', save_cb },
		{ gettext("Save File As..."),	FL_CTRL + FL_SHIFT + 's', saveas_cb, 0, FL_MENU_DIVIDER },
		{ gettext("Export"), 0, 0, 0, 	FL_SUBMENU },
			{ gettext("... as PNG"),	FL_ALT + 'p', export_png_cb },
			{ gettext("... as PNG (solid)"),	FL_ALT + 'f', export_pngn_cb },
			{ gettext("... as JPEG"),	FL_ALT + 'j', export_jpeg_cb },
			{ gettext("... as SVG"),	FL_ALT + 's', export_svg_cb },
			{ gettext("... as vector EPS"),	FL_ALT + 'e', export_eps_cb },
			{ gettext("... as bitmap EPS"),	0, export_bps_cb, 0, FL_MENU_DIVIDER },
			{0},
		{ gettext("New View"),		FL_ALT + 'w', view_cb },
		{ gettext("Close View"),	FL_CTRL + 'w', close_cb, 0, FL_MENU_DIVIDER },
		{ gettext("Exit"),			FL_ALT + 'x', quit_cb },
		{ 0 },
	{ gettext("Edit"), 0, 0, 0, FL_SUBMENU },
		{ gettext("Cut"),			FL_CTRL + 'x', cut_cb },
		{ gettext("Copy"),			FL_CTRL + 'c', copy_cb },
		{ gettext("Paste"),			FL_CTRL + 'v', paste_cb },
		{ gettext("Delete"),		0, delete_cb, 0, FL_MENU_DIVIDER },
		{ gettext("Insert"), 0, 0, 0, 	FL_SUBMENU },
			{ gettext("options"),	FL_ALT + 'o', option_cb },
			{ gettext("style"),		FL_ALT + 'i', style_cb },
			{ gettext("filename"),	0, fname_cb },
			{ gettext("command"),	FL_ALT + 'c', command_cb },
			{ 0 },
		{ gettext("Properties"),	0, settings_cb },
		{ 0 },
	{ gettext("Search"), 0, 0, 0, FL_SUBMENU },
		{ gettext("Find..."),		FL_CTRL + 'f', find_cb },
		{ gettext("Find Again"),	FL_F + 3, find2_cb },
		{ gettext("Replace..."),	FL_CTRL + 'r', replace_cb },
		{ gettext("Replace Again"), FL_F + 4, replace2_cb },
		{ 0 },
	{ gettext("Animate"), 0, 0, 0, FL_SUBMENU },
		{ gettext("Slideshow"),	FL_CTRL + FL_F + 5, sshow_cb, 0, FL_MENU_TOGGLE },
		{ gettext("Next slide"),0, snext_cb },
		{ gettext("Prev slide"),0, sprev_cb },
		{ gettext("Parameters"),0, animate_cb },
		{ 0 },
	{ gettext("Graphics"), 0, 0, 0, FL_SUBMENU },
		{ gettext("Alpha"),			FL_ALT + 'a', alpha_cb, 0, FL_MENU_TOGGLE },
		{ gettext("Light"),			FL_ALT + 'l', light_cb, 0, FL_MENU_TOGGLE },
		{ gettext("Settings"),		FL_F + 2, setup_cb },
		{ gettext("Copy graphics"),	0, 0, 0, FL_MENU_INACTIVE|FL_MENU_DIVIDER},
		{ gettext("Normal view"),	FL_ALT + ' ', norm_cb },
		{ gettext("Execute script"),FL_F + 5, draw_cb },
		{ gettext("Adjust size"),	FL_F + 6, adjust_cb },
		{ gettext("Reload data"),	FL_F + 9, oncemore_cb },
		{ gettext("Script arguments"),	0, argument_cb },
		{ 0 },
/*	{ gettext("Data"), 0, 0, 0, FL_SUBMENU },
		{ gettext("Edit data"),		FL_ALT + 'd', table_cb },
		{ gettext("List of data"),	FL_ALT + 'v', variables_cb },
		{ gettext("Reload data"),	FL_F + 9, oncemore_cb },
		{ 0 },*/
	{ gettext("Help"), 0, 0, 0, FL_SUBMENU },
		{ gettext("MGL Help"),		FL_F + 1, help_cb },
		{ gettext("MGL Examples"),	0, example_cb },
		{ gettext("Hints and FAQ"),	0, hint_cb , 0, FL_MENU_INACTIVE},
		{ gettext("About UDAV"),	0, about_cb },
		{ 0 },
	{ 0 }
};
//-----------------------------------------------------------------------------
void mem_upd_cb(Fl_Widget *, void *v)
{	((ScriptWindow*)v)->mem_init();	}
//-----------------------------------------------------------------------------
ScriptWindow *new_view()
{
	Fl_Tabs* tt;
	Fl_Group *gg;
	ScriptWindow *w = new ScriptWindow(930, 510, title);
	w->begin();
	w->menu = new Fl_Menu_Bar(0, 0, 930, 30);
	w->menu->copy(menuitems, w);

	Fl_Tile *t = new Fl_Tile(0,30,930,455);
	tt = new Fl_Tabs(0,30,300,455,0);	tt->box(UDAV_UP_BOX);
	gg = new Fl_Group(0,30,300,430);	gg->label(gettext("Script"));
	add_editor(w);	gg->end();
	tt->end();		w->ltab = tt;

	tt = new Fl_Tabs(300,30,930,455,0);	tt->box(UDAV_UP_BOX);
	gg = new Fl_Group(300,30,930,430);	gg->label(gettext("Canvas"));
	add_graph(w);	gg->end();	w->gplot = gg;
	gg = new Fl_Group(300,30,930,430);	gg->label(gettext("Help"));
	add_help(w);	gg->end();	w->ghelp = gg;
	gg = new Fl_Group(300,30,930,430);	gg->label(gettext("Memory"));
	add_mem(w);		gg->end();
	tt->end();		w->rtab = tt;

	w->status = new Fl_Box(0,485,930,25,"Ready");
	w->status->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	w->status->color(FL_BACKGROUND_COLOR);
	w->status->box(FL_DOWN_BOX);
	w->graph->status = w->status;

	t->end();	t->resizable(t);
	w->end();	w->resizable(t);
	tt->callback(mem_upd_cb, w);
	w->callback((Fl_Callback *)close_cb, w);

	num_windows++;
	return w;
}
//-----------------------------------------------------------------------------
void argument_set(int n, const char *s);
int main(int argc, char **argv)
{
	char *buf, *buf2;
	pref.get("locale",buf,"ru_RU.cp1251");	setlocale(LC_CTYPE, buf);	free(buf);
	pref.get("plastic_scheme",plastic_scheme,1);
	pref.get("internal_font",internal_font,0);
	pref.get("auto_exec",auto_exec,1);
#ifdef USE_GETTEXT
//	setlocale (LC_ALL, "");
//	bindtextdomain (PACKAGE, LOCALEDIR);
//	textdomain (PACKAGE);
#endif

	Fl::visual(FL_DOUBLE);
	if(plastic_scheme) Fl::scheme("plastic");
#ifdef DOC_DIR
	char dir[64];
	if(!pref.get("help_dir",dir,"",64));
		pref.set("help_dir",DOC_DIR);
#endif
	textbuf = new Fl_Text_Buffer;
	style_init();
	ScriptWindow *w = new_view();

	pref.get("font_dir",buf2,"");
	pref.get("font_name",buf,"");
	w->graph->set_font(buf,buf2);
	if(buf)		free(buf);
	if(buf2)	free(buf2);

	w->show(1, argv);
	for(int i=1;i<argc;i++)
	{
		if(argv[i][0]!='-')
		{
			load_file(argv[i], -1);
			if(auto_exec)	w->update_pre();
		}
		else
		{
			char ch = argv[i][1];
			if(ch>='0' && ch<='9')	argument_set(ch-'0',argv[i]+2);
		}
	}
	return Fl::run();
}
//-----------------------------------------------------------------------------
