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
#include "udav.h"
//-----------------------------------------------------------------------------
#ifndef MGL_DOC_DIR
#ifdef WIN32
#define MGL_DOC_DIR ""
#else
#define MGL_DOC_DIR "/usr/local/share/doc/mathgl/"
#endif
#endif
//-----------------------------------------------------------------------------
int num_windows = 0, auto_exec=1, plastic_scheme=1, internal_font=0;
Fl_Preferences pref(Fl_Preferences::USER,"abalakin","mgllab");
char *docdir=0;
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
		mgl_gettext("MGL Files (*.mgl)\tDAT Files (*.{dat,csv})\tAll Files (*)"), filename.c_str());
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
// Fl_Menu_Item menuitems[] = {
// 	{ mgl_gettext("File"), 0, 0, 0, FL_SUBMENU },
// 		{ mgl_gettext("File/New File"),			0, new_cb },
// 		{ mgl_gettext("File/Open File..."),		FL_CTRL + 'o', open_cb },
// 		{ mgl_gettext("File/Insert File..."),	FL_CTRL + 'i', insert_cb },
// 		{ mgl_gettext("File/Save File"),			FL_CTRL + 's', save_cb },
// 		{ mgl_gettext("File/Save File As..._"),	FL_CTRL + FL_SHIFT + 's', saveas_cb, 0, FL_MENU_DIVIDER },
// /*TODO	{ mgl_gettext("Export"), 0, 0, 0, 	FL_SUBMENU },*/
// 		{ mgl_gettext("File/New View"),		FL_ALT + 'w', view_cb },
// 		{ mgl_gettext("File/Close View_"),	FL_CTRL + 'w', close_cb, 0, FL_MENU_DIVIDER },
// 		{ mgl_gettext("File/Exit"),			FL_ALT + 'x', quit_cb },
// 	{ mgl_gettext("Edit"), 0, 0, 0, FL_SUBMENU },
// 		{ mgl_gettext("Edit/Cut"),			FL_CTRL + 'x', cut_cb },
// 		{ mgl_gettext("Edit/Copy"),			FL_CTRL + 'c', copy_cb },
// 		{ mgl_gettext("Edit/Paste"),			FL_CTRL + 'v', paste_cb },
// 		{ mgl_gettext("Edit/Delete"),		0, delete_cb, 0, FL_MENU_DIVIDER },
// 		{ mgl_gettext("Edit/Insert"), 0, 0, 0, 	FL_SUBMENU },
// 			{ mgl_gettext("Edit/Insert/options"),	FL_ALT + 'o', option_cb },
// 			{ mgl_gettext("Edit/Insert/style"),		FL_ALT + 'i', style_cb },
// 			{ mgl_gettext("Edit/Insert/filename"),	0, fname_cb },
// 			{ mgl_gettext("Edit/Insert/command"),	FL_ALT + 'c', command_cb },
// 		{ mgl_gettext("Edit/Properties"),	0, settings_cb },
// 		{ mgl_gettext("Search"), 0, 0, 0, FL_SUBMENU },
// 			{ mgl_gettext("Edit/Search/Find..."),		FL_CTRL + 'f', find_cb },
// 			{ mgl_gettext("Edit/Search/Find Again"),	FL_F + 3, find2_cb },
// 			{ mgl_gettext("Edit/Search/Replace..."),	FL_CTRL + 'r', replace_cb },
// 			{ mgl_gettext("Edit/Search/Replace Again"), FL_F + 4, replace2_cb },
// 		{ 0 },
// /*TODO{ mgl_gettext("Graphics"), 0, 0, 0, FL_SUBMENU },*/
// /*TODO{ mgl_gettext("Data"), 0, 0, 0, FL_SUBMENU },*/
// 	{ mgl_gettext("Help"), 0, 0, 0, FL_SUBMENU },
// 		{ mgl_gettext("Help/MGL Help"),		FL_F + 1, help_cb },
// 		{ mgl_gettext("Help/MGL Examples"),	0, example_cb },
// 		{ mgl_gettext("Help/Hints and FAQ"),	0, hint_cb , 0, FL_MENU_INACTIVE},
// 		{ mgl_gettext("Help/About UDAV"),	0, about_cb },
// 		{ 0 },
// 	{ 0 }
// };
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
			// TODO{"Command", FL_META+'c',  0},
			// TODO{"Inplot", FL_META+'i',  0},
			// TODO{"Fitted formula", FL_META+'f',  0},
			// TODO{"Plot style", 0,  0},
			// TODO{"Option(s)", FL_META+'o',  0},
			// TODO{"File path", FL_META+'p',  0},
			// TODO{"Folder path", 0,  0},
			// TODO{"Numeric value", FL_META+'n',  0},
			{0},
		{0},
	{"Plot", 0,  0, 0, FL_SUBMENU},
		// TODO{"Alpha", FL_CTRL+'t',  0, 0, FL_MENU_TOGGLE},
		// TODO{"Light", FL_CTRL+'l',  0, 0, FL_MENU_TOGGLE},
		// TODO{"Grid", FL_CTRL+'g',  0, 0, FL_MENU_TOGGLE|FL_MENU_DIVIDER},
		// TODO{"Restore zoom", FL_CTRL+'r',  0},
		// TODO{"Update plot", FL_F+5,  0},
		// TODO{"Adjust size", FL_F+6,  0},
		// TODO{"Stop", 0,  0, 0, FL_MENU_DIVIDER},
		// TODO{"Copy image", FL_CTRL+FL_SHIFT+'c',  0},
		// TODO{"Copy click coor.", 0,  0, 0, FL_MENU_DIVIDER},
		{"Add objects", 0,  0, 0, FL_SUBMENU},
			// TODO{"Line", 0,  0},
			// TODO{"Arc", 0,  0},
			// TODO{"Curve", 0,  0},
			// TODO{"Rectangle", 0,  0},
			// TODO{"Rhombus", 0,  0},
			// TODO{"Ellipse", 0,  0},
			// TODO{"Polygon", 0,  0},
			// TODO{"Marker", 0,  0},
			// TODO{"Text", 0,  0},
			{0},
		{"Selection", 0,  0, 0, FL_SUBMENU},
			// TODO{"Hide", 0,  0},
			// TODO{"Delete", 0,  0},
			// TODO{"Move up", 0,  0},
			// TODO{"Move down", 0,  0},
			// TODO{"Show hidden", FL_F+8,  0, 0, FL_MENU_TOGGLE},
			{0},
		{"Export as 2D", 0,  0, 0, FL_SUBMENU},
			{"PNG", FL_ALT+'p',  export_png_cb},
			{"solid PNG", FL_ALT+'f',  export_spng_cb},
			{"JPEG", FL_ALT+'j',  export_jpg_cb},
			{"GIF", FL_ALT+'g',  export_gif_cb},
			{"BMP", FL_ALT+'b',  export_bmp_cb},
			{"bitmap EPS", 0,  export_bps_cb},
			{"vector EPS", FL_ALT+'e',  export_eps_cb},
			{"SVG", FL_ALT+'s',  export_svg_cb},
			{"LaTeX", FL_ALT+'l',  export_tex_cb},
			{0},
		{"Export as 3D", 0,  0, 0, FL_SUBMENU},
			{"3D PDF", FL_ALT+'d',  export_3pdf_cb},
			{"PRC", 0,  export_prc_cb},
			{"OBJ", FL_ALT+'o',  export_obj_cb},
			{"STL", 0,  export_stl_cb},
			{"XYZ", 0,  export_xyz_cb},
			{0},
		{"Transform", 0,  0, 0, FL_SUBMENU},
			// TODO{"Move left", FL_ALT+FL_Left,  0},
			// TODO{"Move up", FL_ALT+FL_Up,  0},
			// TODO{"Zoom in", FL_ALT+'=',  0},
			// TODO{"Zoom out", FL_ALT+'-',  0},
			// TODO{"Move down", FL_ALT+FL_Down,  0},
			// TODO{"Move right", FL_ALT+FL_Right,  0, 0, FL_MENU_DIVIDER},
			// TODO{"Rotate up", FL_CTRL+FL_Up,  0},
			// TODO{"Rotate down", FL_CTRL+FL_Down,  0},
			// TODO{"Rotate left", FL_CTRL+FL_Left,  0},
			// TODO{"Rotate right", FL_CTRL+FL_Right,  0},
			{0},
		{"Animation", 0,  0, 0, FL_SUBMENU},
			// TODO{"Next slide", FL_CTRL+'.',  0},
			// TODO{"Slideshow", FL_CTRL+FL_F+5,  0},
			// TODO{"Prev. slide", FL_CTRL+',',  0},
			// TODO{"Setup show", FL_CTRL+'w',  0},
			{0},
		{0},
	{"Setup", 0,  0, 0, FL_SUBMENU},
		// TODO{"Properties", 0,  /*cb_Properties*/},
		// TODO{"Set arguments", 0,  /*cb_Set*/},
		// TODO{"Plot setup", FL_META+'g',  0, 0, FL_MENU_DIVIDER},
		// TODO{"Calculator", FL_F+4,  0, 0, FL_MENU_TOGGLE},
		// TODO{"Messages", FL_F+2,  0, 0, FL_MENU_TOGGLE},
		{0},
	{"Help", 0,  0, 0, FL_SUBMENU},
		// TODO{"Help", FL_F+1,  0},
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
	ScriptWindow *w = new ScriptWindow(930, 510, "Untitled - mgllab");
	w->begin();
	w->menu = new Fl_Menu_Bar(0, 0, 930, 30);

//	w->menu->add(mgl_gettext("File"), 0, 0, 0, FL_SUBMENU);	
// 	w->menu->add(mgl_gettext("File/New File"), "", new_cb);
// 	w->menu->add(mgl_gettext("File/Open File..."), "^o", open_cb, w);
// 	w->menu->add(mgl_gettext("File/Insert File..."),	"^i", insert_cb, w);
// 	w->menu->add(mgl_gettext("File/Save File"), "^s", save_cb, w);
// 	w->menu->add(mgl_gettext("File/Save File As..."), 0, saveas_cb, w, FL_MENU_DIVIDER);
// 	/*TODO	{ mgl_gettext("Export"), 0, 0, 0, 	FL_SUBMENU },*/
// 	w->menu->add(mgl_gettext("File/New View"), "#w", view_cb, w);
// 	w->menu->add(mgl_gettext("File/Close View"), "^w", close_cb, w, FL_MENU_DIVIDER);
// 	w->menu->add(mgl_gettext("File/Exit"), "#x", quit_cb);

	w->menu->copy(menuitems, w);

	Fl_Tile *t = new Fl_Tile(0,30,930,455);
	tt = new Fl_Tabs(0,30,300,455,0);	tt->box(UDAV_UP_BOX);	w->ltab = tt;
	gg = new Fl_Group(0,30,300,430);	gg->label(mgl_gettext("Script"));
	add_editor(w);	gg->end();
	tt->end();

	tt = new Fl_Tabs(300,30,630,455,0);	tt->box(UDAV_UP_BOX);	w->rtab = tt;
	gg = new Fl_Group(300,30,630,430,mgl_gettext("Canvas"));
	w->graph = new Fl_MGL(300,30,630,430);	gg->end();
	gg = new Fl_Group(300,30,630,430,mgl_gettext("Help"));
	w->ghelp = gg;	add_help(w);	gg->end();
	gg = new Fl_Group(300,30,630,430,mgl_gettext("Memory"));
	add_mem(w);		gg->end();
	tt->end();

	w->status = new Fl_Box(0,485,930,25,"Ready");
	w->status->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	w->status->color(FL_BACKGROUND_COLOR);
	w->status->box(FL_DOWN_BOX);
	w->graph->status = w->status;

	t->end();	w->end();	w->resizable(t);
	tt->callback(mem_upd_cb, w);
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
	char *buf, *buf2, ch;
	pref.get("locale",buf,"ru_RU.cp1251");	setlocale(LC_CTYPE, buf);	free(buf);
	pref.get("plastic_scheme",plastic_scheme,1);
	pref.get("internal_font",internal_font,0);
	pref.get("auto_exec",auto_exec,1);
	pref.get("help_dir",docdir,MGL_DOC_DIR);	// docdir should be freed at exit

	Fl::visual(FL_DOUBLE|FL_RGB);
	if(plastic_scheme) Fl::scheme("gtk+");

#ifdef USE_GETTEXT
//	setlocale (LC_NUMERIC, "");
//	bindtextdomain (PACKAGE, LOCALEDIR);
//	textdomain (PACKAGE);
#endif

	textbuf = new Fl_Text_Buffer;
	style_init();
	ScriptWindow *w = new_view();

	pref.get("font_dir",buf2,"");
	pref.get("font_name",buf,"");
	mgl_load_font(w->graph->FMGL->get_graph(),buf,buf2);
	if(buf)	free(buf);
	if(buf2)	free(buf2);

	buf = 0;
	while(1)
	{
		ch = getopt(argc, argv, "1:2:3:4:5:6:7:8:9:ho:L:");
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
			free(docdir);	return 0;
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
