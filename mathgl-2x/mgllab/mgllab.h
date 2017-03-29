/* udav.h is part of UDAV
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
//-----------------------------------------------------------------------------
#ifndef _UDAV_H_
#define _UDAV_H_
//-----------------------------------------------------------------------------
#ifdef __MWERKS__
# define FL_DLL
#endif
#ifdef USE_GETTEXT
	#include <libintl.h>
#else
	#define mgl_gettext(x)	(x)
#endif
#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_ask.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Counter.H>
#include <Fl/Fl_Scroll.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Help_View.H>
#include <Fl/Fl_Table.H>
#include <Fl/Fl_Round_Button.H>
#include <Fl/Fl_Float_Input.H>
#include <Fl/Fl_Multiline_Input.H>
//-----------------------------------------------------------------------------
#include "mgl2/Fl_MathGL.h"
//-----------------------------------------------------------------------------
extern mglParse *Parse;
extern Fl_Menu_Item colors[];
class Fl_MGL;
//-----------------------------------------------------------------------------
extern Fl_Preferences pref;
extern Fl_Text_Display::Style_Table_Entry styletable[9];
extern int changed;	///< flag of script is changed or not
extern std::string filename;	///< Current filename
extern std::string lastfiles[5];///< Last opened files
extern std::string search;		///< Text to search
extern Fl_Text_Buffer *textbuf;
//-----------------------------------------------------------------------------
extern int auto_exec;	///< Enable auto execution
extern int exec_save;	///< Save before running
extern int highlight;	///< Highlight current line
extern int mouse_zoom;	///< Use mouse wheel for zooming
extern std::string docdir;	///< Path to help files
extern std::string	fontname;	///< Path to font files
extern int lang;	///< Locale for script and help files
extern int scheme;	///< FLTK scheme
extern int font_kind;	///< Editor font kind
extern int font_size;	///< Editor font size
//-----------------------------------------------------------------------------
void set_scheme_lang(int s, int l);		///< Set FLTK scheme and locale
void set_style(int fkind, int fsize);	///< Change the style of highlight
void style_init();	///< Initialize the style buffer
void save_pref();	///< Apply and save preferences
void load_pref();	///< Load preferences
void add_filename(const char *fname);	///< Add filename to lastfiles
//-----------------------------------------------------------------------------
class Fl_Data_Table : public Fl_Table
{
private:
	int row, col;
	Fl_Input* input;
protected:
	void draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H);
	static void event_callback(Fl_Widget*, void*v)
	{	((Fl_Data_Table*)v)->cell_click();	}
	void cell_click();

public:
	mglDataA *data;
	int nx, ny, sl;

	Fl_Data_Table(int x, int y, int w, int h, const char *l=0);
    ~Fl_Data_Table() { }

	void set_value();
    void rows(int val) { if (input->visible()) input->do_callback(); Fl_Table::rows(val); }
    void cols(int val) { if (input->visible()) input->do_callback(); Fl_Table::cols(val); }
    inline int rows() { return Fl_Table::rows(); }
    inline int cols() { return Fl_Table::cols(); }
};
//-----------------------------------------------------------------------------
struct AnimateDlg
{
	friend void animate_dlg_cb(Fl_Widget *, void *v);
	friend void animate_rad_cb(Fl_Widget *, void *v);
	friend void fill_animate(const char *text);
	friend void animate_put_cb(Fl_Widget *, void *);
public:
	Fl_Window* wnd;
	int OK;
	AnimateDlg()	{	memset(this,0,sizeof(AnimateDlg));	create_dlg();	}
	~AnimateDlg()	{	delete wnd;	}
	void FillResult(Fl_MGL* e);
protected:
	bool swap;
	Fl_Round_Button *rt, *rv;
	Fl_Multiline_Input *txt;
	Fl_Float_Input *x0, *x1, *dx, *dt;
	Fl_Check_Button *save;
	void create_dlg();
};
//-----------------------------------------------------------------------------
class Fl_MGL : public mglDraw
{
friend class AnimateDlg;
public:
	Fl_MGLView *gr;
	Fl_Widget *status;		///< StatusBar for mouse coordinates
	const char *AnimBuf;		///< buffer for animation
	const char **AnimS0;
	int AnimNum;
	mreal AnimDelay;
	std::string script;		///< script with settings

	Fl_MGL(Fl_MGLView *GR);
	~Fl_MGL();

	/// Drawing itself
	int Draw(mglGraph *);
	/// Update (redraw) plot
	void update();
	/// Show next frame
	void next_frame();
	/// Show prev frame
	void prev_frame();
	/// Get pointer to grapher
	HMGL get_graph()	{	return gr->FMGL->get_graph();	}

protected:
	char *Args[1000], *ArgBuf;
	int NArgs, ArgCur;

};
//-----------------------------------------------------------------------------
struct TableWindow : public Fl_Window
{
public:
	TableWindow(int x, int y, int w, int h, const char* t=0);
	~TableWindow();
	void update(mglDataA *v);
	void refresh();
	void set_slice(long s);
	inline long get_slice() { return sl; }
	inline long num_slice()	{	return nz;	}
	void go_home();

	Fl_Data_Table *data;
	Fl_Menu_Bar	*menu;
//	Fl_Output *main;
	Fl_Counter *slice;
	mglDataA *var;
protected:
//	long nx,ny,nz;
	long nz;
	long sl;		// current slice
	char sl_id[64];	// slice id
};
//-----------------------------------------------------------------------------
class ScriptWindow : public Fl_Double_Window
{
public:
	ScriptWindow(int w, int h, const char* t);
	~ScriptWindow();

	Fl_Window	*replace_dlg;
	Fl_Input	*replace_find;
	Fl_Input	*replace_with;
	Fl_Button	*replace_all;
	Fl_Return_Button	*replace_next;
	Fl_Button	*replace_cancel;
	Fl_Text_Editor		*editor;
	Fl_Menu_Bar	*menu;
	Fl_Tabs *ltab, *rtab;
	Fl_Help_View *hd;
	Fl_Input *link_cmd;
	Fl_Group *ghelp;
	Fl_Browser *var;
	Fl_Box *status;

	void mem_init();
	void mem_pressed(int n);
	char		search[256];
	Fl_MGLView	*graph;
	Fl_MGL		*draw;
};
//-----------------------------------------------------------------------------
class GeneralDlg
{
protected:
	Fl_Double_Window *w;
public:
	mglDataA *dat;
	ScriptWindow *e;
	std::string result;
	virtual void cb_ok(){}
	virtual void init()	{	result.clear();	}
	void show()	{	init();	w->show();	}
	void hide()	{	w->hide();	}
};
static void cb_dlg_cancel(Fl_Widget*, void*);
static void cb_dlg_ok(Fl_Widget*, void*);
//-----------------------------------------------------------------------------
// Editor window functions
void find2_cb(Fl_Widget *, void *);
void replall_cb(Fl_Widget *, void *);
void replace2_cb(Fl_Widget *, void *);
void replcan_cb(Fl_Widget *, void *);
void replace_cb(Fl_Widget *, void *);
void find_cb(Fl_Widget *, void *);
void insert_cb(Fl_Widget *, void *);
//-----------------------------------------------------------------------------
void paste_cb(Fl_Widget *, void *);
void select_all_cb(Fl_Widget *, void *);
void undo_cb(Fl_Widget *, void *);
void copy_cb(Fl_Widget *, void *);
void cut_cb(Fl_Widget *, void *);
void delete_cb(Fl_Widget *, void *);
void changed_cb(int, int nInserted, int nDeleted,int, const char*, void* v);
void ins_fname_cb(Fl_Widget *, void *);
void ins_path_cb(Fl_Widget *, void *);
void ins_fits_cb(Fl_Widget *, void *);
//-----------------------------------------------------------------------------
// General callback functions
void new_cb(Fl_Widget *, void *);
void open_cb(Fl_Widget *, void *);
void save_cb(Fl_Widget*, void*);
void saveas_cb(Fl_Widget*, void*);
void help_cb(Fl_Widget*, void*);
//-----------------------------------------------------------------------------
// Graphical callback functions
void setup_cb(Fl_Widget *, void *);
void style_cb(Fl_Widget *, void *);
void argument_cb(Fl_Widget *, void *);
void variables_cb(Fl_Widget *, void *);
void settings_cb(Fl_Widget *, void *);
//-----------------------------------------------------------------------------
// Dialogs callback functions
void close_dlg_cb(Fl_Widget *w, void *);
void font_cb(Fl_Widget *, void *v);
void line_cb(Fl_Widget *, void *v);
void face_cb(Fl_Widget *, void *v);
void data_cb(Fl_Widget *, void *v);
//-----------------------------------------------------------------------------
void style_init(void);
int check_save(void);
void load_file(const char *newfile, int ipos, ScriptWindow *e);
void save_file(const char *newfile, ScriptWindow *e);
Fl_Widget *add_editor(ScriptWindow *w);
Fl_Widget *add_mem(ScriptWindow *w);
void set_title(Fl_Window* w);
//-----------------------------------------------------------------------------
// Animation
void animate_cb(Fl_Widget *, void *v);
void fill_animate(const char *text);
//-----------------------------------------------------------------------------
Fl_Widget *add_help(ScriptWindow *w);
void help_cb(Fl_Widget*, void*v);
void link_cb(Fl_Widget*, void*v);
void example_cb(Fl_Widget*, void*v);
void about_cb(Fl_Widget*, void*);
//-----------------------------------------------------------------------------
void prop_dlg_cb(Fl_Widget*, void*);
void calc_dlg_cb(Fl_Widget*, void*);
void args_dlg_cb(Fl_Widget*, void*);
void option_dlg_cb(Fl_Widget*, void*);
void dirsel_dlg_cb(Fl_Widget*, void*);
void datsel_dlg_cb(Fl_Widget*, void*);
void style_dlg_cb(Fl_Widget*, void*);
void newcmd_dlg_cb(Fl_Widget*,void*);
void setup_dlg_cb(Fl_Widget*,void *);
void inplot_dlg_upd(Fl_Widget*,void*);
void cb_args_set(const char *val);	///< set value for argument in newcmd_dlg
//-----------------------------------------------------------------------------
extern Fl_Text_Buffer *textbuf;
extern std::string filename;
extern int	changed;
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
