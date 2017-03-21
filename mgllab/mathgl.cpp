/* mathgl.cpp is part of UDAV
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
#include "udav.h"
//-----------------------------------------------------------------------------
#include "xpm/alpha.xpm"
#include "xpm/light.xpm"
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
#include "xpm/document-properties.xpm"
//#include "xpm/preferences-system.xpm"
#include "xpm/wire.xpm"
//-----------------------------------------------------------------------------
extern int internal_font;
mglParse *Parse=0;
//-----------------------------------------------------------------------------
void udav_error(const char *Message, void *v)
{	((Fl_MGL*)v)->status->label(Message);	}
mreal udav_delay(void *v)
{	return ((Fl_MGL*)v)->AnimDelay;	}
void udav_reload(void *v)
{	Parse->RestoreOnce();	((Fl_MGL*)v)->update();	}
//-----------------------------------------------------------------------------
void udav_next(void *v)	{	((Fl_MGL*)v)->next_frame();	}
void Fl_MGL::next_frame()
{
	if(NArgs==0)
	{
		animate_cb(this,this);
		if(NArgs==0)	return;
	}
	ArgCur = (ArgCur+1) % NArgs;
	Parse->AddParam(0,Args[ArgCur]);
	update();
}
//-----------------------------------------------------------------------------
void udav_prev(void *v)	{	((Fl_MGL*)v)->prev_frame();	}
void Fl_MGL::prev_frame()
{
	if(NArgs==0)
	{
		animate_cb(this,this);
		if(NArgs==0)	return;
	}
	ArgCur = ArgCur>0 ? ArgCur-1 : NArgs-1;
	Parse->AddParam(0,Args[ArgCur]);
	update();
}
//-----------------------------------------------------------------------------
Fl_MGL::Fl_MGL(int x, int y, int w, int h, const char *label) : Fl_MGLView(x,y,w,h,label)
{
	if(!Parse)	Parse = new mglParse;
	Parse->AllowSetSize(true);
	ArgBuf = 0;	NArgs = ArgCur = 0;
	script = script_pre = 0;	par = this;
	next = udav_next;	delay = udav_delay;
	prev = udav_prev;	reload = udav_reload;
	FMGL->set_draw(this);
/*#ifdef WIN32
//	setlocale(LC_TYPE,"russian_Russia.CP1251");
	char *path;
	get_doc_dir(path);
	if(!FMGL->GetFont()->Load("STIX",path && path[0] ? path : "."))	FMGL->GetFont()->Restore();
	free(path);
#endif*/
}
//-----------------------------------------------------------------------------
Fl_MGL::~Fl_MGL()	{	clear_scripts();	if(ArgBuf)	delete []ArgBuf;	}
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
int Fl_MGL::Draw(mglGraph *gr)
{
	Parse->Execute(gr,script_pre);
	Parse->Execute(gr,script);
	if(textbuf)
	{
		char *text = textbuf->text();
		Parse->Execute(gr,text);
		free(text);
	}
	status->label(gr->Message());
	return 0;
}
//-----------------------------------------------------------------------------
void Fl_MGL::update()
{
	// NOTE: hint for old style View(). May be I should remove it!
	if(!script || !strstr(script,"rotate"))	mgl_rotate(FMGL->get_graph(),0,0,0);

	Fl_MGLView::update();
	for(long i=0;i<Parse->GetNumVar();i++)
	{
		mglDataA *v = Parse->GetVar(i);
		if(v && v->o)	((TableWindow *)v->o)->update(v);
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
void export_png_cb(Fl_Widget*, void *v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	std::string fname = filename.empty()?"mgllab.png":filename+".png";
	mgl_write_png(e->graph->get_graph(), fname.c_str(),"");
}
//-----------------------------------------------------------------------------
void export_spng_cb(Fl_Widget*, void *v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	std::string fname = filename.empty()?"mgllab.png":filename+".png";
	mgl_write_png_solid(e->graph->get_graph(), fname.c_str(),"");
}
//-----------------------------------------------------------------------------
void export_jpg_cb(Fl_Widget*, void *v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	std::string fname = filename.empty()?"mgllab.jpg":filename+".jpg";
	mgl_write_jpg(e->graph->get_graph(), fname.c_str(),"");
}
//-----------------------------------------------------------------------------
void export_gif_cb(Fl_Widget*, void *v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	std::string fname = filename.empty()?"mgllab.gif":filename+".gif";
	mgl_write_gif(e->graph->get_graph(), fname.c_str(),"");
}
//-----------------------------------------------------------------------------
void export_bmp_cb(Fl_Widget*, void *v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	std::string fname = filename.empty()?"mgllab.bmp":filename+".bmp";
	mgl_write_bmp(e->graph->get_graph(), fname.c_str(),"");
}
//-----------------------------------------------------------------------------
void export_eps_cb(Fl_Widget*, void *v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	std::string fname = filename.empty()?"mgllab.eps":filename+".eps";
	mgl_write_eps(e->graph->get_graph(), fname.c_str(),"");
}
//-----------------------------------------------------------------------------
void export_bps_cb(Fl_Widget*, void *v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	std::string fname = filename.empty()?"mgllab.eps":filename+".eps";
	mgl_write_bps(e->graph->get_graph(), fname.c_str(),"");
}
//-----------------------------------------------------------------------------
void export_svg_cb(Fl_Widget*, void *v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	std::string fname = filename.empty()?"mgllab.svg":filename+".svg";
	mgl_write_svg(e->graph->get_graph(), fname.c_str(),"");
}
//-----------------------------------------------------------------------------
void export_tex_cb(Fl_Widget*, void *v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	std::string fname = filename.empty()?"mgllab.tex":filename+".tex";
	mgl_write_tex(e->graph->get_graph(), fname.c_str(),"");
}
//-----------------------------------------------------------------------------
void export_prc_cb(Fl_Widget*, void *v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	std::string fname = filename.empty()?"mgllab.prc":filename+".prc";
	mgl_write_prc(e->graph->get_graph(), fname.c_str(),"",0);
}
//-----------------------------------------------------------------------------
void export_obj_cb(Fl_Widget*, void *v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	std::string fname = filename.empty()?"mgllab.obj":filename+".obj";
	mgl_write_obj(e->graph->get_graph(), fname.c_str(),"",0);
}
//-----------------------------------------------------------------------------
void export_xyz_cb(Fl_Widget*, void *v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	std::string fname = filename.empty()?"mgllab.xyz":filename+".xyz";
	mgl_write_xyz(e->graph->get_graph(), fname.c_str(),"");
}
//-----------------------------------------------------------------------------
void export_3pdf_cb(Fl_Widget*, void *v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	std::string fname = filename.empty()?"mgllab.pdf":filename+".pdf";
	mgl_write_prc(e->graph->get_graph(), fname.c_str(),"",1);
}
//-----------------------------------------------------------------------------
void export_stl_cb(Fl_Widget*, void *v)
{
	ScriptWindow* e = (ScriptWindow*)v;
	std::string fname = filename.empty()?"mgllab.stl":filename+".stl";
	mgl_write_stl(e->graph->get_graph(), fname.c_str(),"");
}
//-----------------------------------------------------------------------------
