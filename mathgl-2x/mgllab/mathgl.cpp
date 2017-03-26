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
#include "mgllab.h"
//-----------------------------------------------------------------------------
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
		animate_cb(0,this);
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
		animate_cb(0,this);
		if(NArgs==0)	return;
	}
	ArgCur = ArgCur>0 ? ArgCur-1 : NArgs-1;
	Parse->AddParam(0,Args[ArgCur]);
	update();
}
//-----------------------------------------------------------------------------
Fl_MGL::Fl_MGL(Fl_MGLView *GR)
{
	if(!Parse)	Parse = new mglParse;
	Parse->AllowSetSize(true);
	ArgBuf = 0;	NArgs = ArgCur = 0;
	script = script_pre = 0;
	gr = GR;	gr->par = this;
	gr->next = udav_next;	gr->delay = udav_delay;
	gr->prev = udav_prev;	gr->reload = udav_reload;
	gr->FMGL->set_draw(this);
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
	if(!script || !strstr(script,"rotate"))	mgl_rotate(gr->get_graph(),0,0,0);

	gr->update();
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
