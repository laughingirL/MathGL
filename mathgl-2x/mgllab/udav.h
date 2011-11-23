/* udav.h is part of UDAV
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
	#define gettext(x)	(x)
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
//-----------------------------------------------------------------------------
#ifdef USE_PLASTIC
	#define UDAV_UP_BOX			FL_PLASTIC_UP_BOX
	#define UDAV_DOWN_BOX		FL_PLASTIC_DOWN_BOX
	#define UDAV_EDIT_BOX		FL_PLASTIC_THIN_DOWN_BOX
	#define UDAV_THIN_UP_BOX	FL_PLASTIC_THIN_UP_BOX
#else
	#define UDAV_UP_BOX			FL_GTK_UP_BOX
	#define UDAV_DOWN_BOX		FL_GTK_DOWN_BOX
	#define UDAV_EDIT_BOX		FL_GTK_DOWN_BOX
	#define UDAV_THIN_UP_BOX	FL_GTK_THIN_UP_BOX
#endif
//-----------------------------------------------------------------------------
#include "mgl/parser.h"
#include "mgl/fltk.h"
//-----------------------------------------------------------------------------
extern mglParse *Parse;
extern Fl_Menu_Item colors[];
extern Fl_Preferences pref;
//-----------------------------------------------------------------------------
class Fl_Data_Table : public Fl_Table
{
private:
	int row, col;
	Fl_Input* input;
protected:
	void draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H);
	static void event_callback(Fl_Widget*, void*v)
	{	((Fl_Data_Table*)v)->cell_click();	};
	void cell_click();

public:
	float *data;
	int nx, ny;

	Fl_Data_Table(int x, int y, int w, int h, const char *l=0);
    ~Fl_Data_Table() { }

	void set_value();
    void rows(int val) { if (input->visible()) input->do_callback(); Fl_Table::rows(val); }
    void cols(int val) { if (input->visible()) input->do_callback(); Fl_Table::cols(val); }
    inline int rows() { return Fl_Table::rows(); }
    inline int cols() { return Fl_Table::cols(); }
};
//-----------------------------------------------------------------------------
class Fl_MGL : public Fl_MathGL
{
public:
	mglParse	*parse;		///< pointer to external parser
	Fl_Widget *status;		///< StatusBar for mouse coordinates
	const char *AnimBuf;	///< buffer for animation
	const char **AnimS0;
	int AnimNum;
	float AnimDelay;

	Fl_MGL(int x, int y, int w, int h, char *label=0);
	~Fl_MGL();

	/// Update (redraw) plot
	void update();
	/// Set main \a scr and optional \a pre scripts for execution
	void scripts(char *scr, char *pre);
	/// Clear scripts internally saved
	void clear_scripts();
	/// Set extra flags
	void set_state(long fl);

protected:
	bool wire;			///< flag for show absolute grid
	bool alpha;			///< flag for manual transparency
	bool light;			///< flag for manual lighting
	char *script;		///< main script
	char *script_pre;	///< script with settings
	char MouseBuf[128];	///< Text of mouse coordinates

	void draw();		///< quick drawing function
};
//-----------------------------------------------------------------------------
struct TableWindow : public Fl_Window
{
public:
	TableWindow(int x, int y, int w, int h, const char* t=0);
	~TableWindow();
	void update(mglVar *v);
	void refresh();
	void set_slice(long s);
	inline long get_slice() { return sl; };
	inline long num_slice()	{	return nz;	};
	void go_home();

	Fl_Data_Table *data;
	Fl_Menu_Bar	*menu;
//	Fl_Output *main;
	Fl_Counter *slice;
	mglVar *var;
protected:
//	long nx,ny,nz;
	long nz;
	long sl;		// current slice
	char sl_id[64];	// slice id
};
//-----------------------------------------------------------------------------
class SetupDlg
{
public:
	Fl_Window *wnd;
	bool OK;
	Fl_Input *templ;

	SetupDlg()	{	memset(this,0,sizeof(SetupDlg));	};
	~SetupDlg()	{	delete wnd;	};
	void CreateDlg();
	char *ToScript();
private:
	Fl_Input *xmin, *ymin, *zmin, *cmin;
	Fl_Input *xmax, *ymax, *zmax, *cmax;
	Fl_Input *xorg, *yorg, *zorg;
	Fl_Input *xlab, *ylab, *zlab, *font;
	Fl_Choice *xpos, *ypos, *zpos, *axial, *cid[10];
	Fl_Input *xtik, *ytik, *ztik;
	Fl_Input *xsub, *ysub, *zsub;
	Fl_Input *alphad, *ambient, *basew, *mesh, *size;
	Fl_Check_Button *alpha, *light, *rotate, *lid[10];
	Fl_Input *xid[10], *yid[10], *zid[10], *bid[10];
	Fl_Help_View *code;

	void CreateGen();
	void CreateLid();
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
	Fl_Group *ghelp, *gplot;
	Fl_Browser *var;
	Fl_Box *status;

	void mem_init();
	void mem_pressed(int n);
	SetupDlg 	*setup_dlg;
	char		search[256];

//=============== Graphical part ==============================================
	Fl_Button	*alpha_bt, *light_bt, *rotate_bt, *zoom_bt, *anim_bt, *wire_bt;
	Fl_Counter	*phi, *tet;
	Fl_MGL		*graph;
	Fl_Scroll	*scroll;

	int alpha, light, sshow, wire;
	bool zoom, rotate;

	void update_pre();

//=============== Animation ===================================================
	char *Args[1000], *ArgBuf;
	int NArgs, ArgCur;
};
//-----------------------------------------------------------------------------
// Editor window functions
void find2_cb(Fl_Widget *, void *);
void replall_cb(Fl_Widget *, void *);
void replace2_cb(Fl_Widget *, void *);
void replcan_cb(Fl_Widget *, void *);
void insert_cb(Fl_Widget *, void *);
void paste_cb(Fl_Widget *, void *);
void replace_cb(Fl_Widget *, void *);
void copy_cb(Fl_Widget *, void *);
void cut_cb(Fl_Widget *, void *);
void find_cb(Fl_Widget *, void *);
void delete_cb(Fl_Widget *, void *);
void changed_cb(int, int nInserted, int nDeleted,int, const char*, void* v);
//-----------------------------------------------------------------------------
// General callback functions
void new_cb(Fl_Widget *, void *);
void open_cb(Fl_Widget *, void *);
void save_cb(Fl_Widget*, void*);
void saveas_cb(Fl_Widget*, void*);
void help_cb(Fl_Widget*, void*);
//-----------------------------------------------------------------------------
// Graphical callback functions
void export_png_cb(Fl_Widget *, void *);
void export_pngn_cb(Fl_Widget *, void *);
void export_jpeg_cb(Fl_Widget *, void *);
void export_svg_cb(Fl_Widget *, void *);
void export_eps_cb(Fl_Widget *, void *);
void export_bps_cb(Fl_Widget *, void *);
void alpha_cb(Fl_Widget *, void *);
void light_cb(Fl_Widget *, void *);
void setup_cb(Fl_Widget *, void *);
void style_cb(Fl_Widget *, void *);
void option_cb(Fl_Widget *, void *);
void argument_cb(Fl_Widget *, void *);
void variables_cb(Fl_Widget *, void *);
void settings_cb(Fl_Widget *, void *);
void norm_cb(Fl_Widget *, void *);
void draw_cb(Fl_Widget *, void *);
void oncemore_cb(Fl_Widget*, void*);
void command_cb(Fl_Widget *, void *);
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
void load_file(char *newfile, int ipos);
void save_file(char *newfile);
Fl_Widget *add_editor(ScriptWindow *w);
Fl_Widget *add_graph(ScriptWindow *w);
Fl_Widget *add_mem(ScriptWindow *w);
void set_title(Fl_Window* w);
//-----------------------------------------------------------------------------
// Animation
void animate_cb(Fl_Widget *, void *v);
void sshow_cb(Fl_Widget *, void *v);
void snext_cb(Fl_Widget *, void *v);
void sprev_cb(Fl_Widget *, void *v);
void fill_animate(const char *text);
//-----------------------------------------------------------------------------
Fl_Widget *add_help(ScriptWindow *w);
void help_cb(Fl_Widget*, void*v);
void example_cb(Fl_Widget*, void*v);
void about_cb(Fl_Widget*, void*);
//-----------------------------------------------------------------------------
void newcmd_cb(Fl_Widget*,void*);
//-----------------------------------------------------------------------------
extern Fl_Text_Buffer	*textbuf;
extern char	filename[256];
extern int	changed;
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
