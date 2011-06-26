/***************************************************************************
 * window.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "mgl/window.h"
#include "mgl/mgl.h"
//-----------------------------------------------------------------------------
mglCanvasW::mglCanvasW() : mglCanvas()
{
	AutoClf=true;	ClfOnUpdate=true;
	Delay=0.5;		ShowMousePos=false;
	LoadFunc=0;	FuncPar=0;	DrawFunc=0;
}
//-----------------------------------------------------------------------------
int mgl_draw_class(mglBase *gr, void *p)
{	return p ? ((mglDraw *)p)->Draw(gr) : 0;	}
void mgl_reload_class(int next, void *p)
{	if(p)	((mglDraw *)p)->Reload(next);	}
void mglCanvasW::Window(int argc, char **argv, const char *title, mglDraw *draw, bool maximize)
{	Window(argc, argv, mgl_draw_class, title, draw, mgl_reload_class, maximize);	}
//-----------------------------------------------------------------------------
typedef int (*draw_func)(mglGraph *gr);
int mgl_draw_graph(mglBase *gr, void *p)
{
	mglGraph g(gr);
	draw_func func = (draw_func)(p);
	return func ? func(&g) : 0;
}
void mglCanvasW::Window(int argc, char **argv, int (*draw)(mglGraph *gr), const char *title, bool maximize)
{	Window(argc,argv,mgl_draw_graph,title,(void*)draw,0,maximize);	}
//-----------------------------------------------------------------------------
void mgl_wnd_set_delay(HMGL gr, mreal dt)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->Delay = dt;	}
void mgl_wnd_set_auto_clf(HMGL gr, int val)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->AutoClf = val;	}
void mgl_wnd_set_show_mouse_pos(HMGL gr, int val)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->ShowMousePos = val;	}
void mgl_wnd_set_clf_update(HMGL gr, int val)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->ClfOnUpdate = val;	}
void mgl_wnd_toggle_alpha(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->ToggleAlpha();	}
void mgl_wnd_toggle_light(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->ToggleLight();	}
void mgl_wnd_toggle_zoom(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->ToggleZoom();	}
void mgl_wnd_toggle_rotate(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->ToggleRotate();	}
void mgl_wnd_toggle_no(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->ToggleNo();	}
void mgl_wnd_update(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->Update();	}
void mgl_wnd_reload(HMGL gr, int o)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->ReLoad(o);	}
void mgl_wnd_adjust(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->Adjust();	}
void mgl_wnd_next_frame(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->NextFrame();	}
void mgl_wnd_prev_frame(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->PrevFrame();	}
void mgl_wnd_animation(HMGL gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>(gr);	if(g)	g->Animation();	}
//-----------------------------------------------------------------------------
void mgl_wnd_set_delay_(uintptr_t *gr, mreal *dt)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->Delay = *dt;	}
void mgl_wnd_set_auto_clf_(uintptr_t *gr, int *val)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->AutoClf = *val;	}
void mgl_wnd_set_show_mouse_pos_(uintptr_t *gr, int *val)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->ShowMousePos = *val;	}
void mgl_wnd_set_clf_update_(uintptr_t *gr, int *val)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->ClfOnUpdate = *val;	}
void mgl_wnd_toggle_alpha_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->ToggleAlpha();	}
void mgl_wnd_toggle_light_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->ToggleLight();	}
void mgl_wnd_toggle_zoom_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->ToggleZoom();	}
void mgl_wnd_toggle_rotate_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->ToggleRotate();	}
void mgl_wnd_toggle_no_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->ToggleNo();	}
void mgl_wnd_update_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->Update();	}
void mgl_wnd_reload_(uintptr_t *gr, int *o)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->ReLoad(*o);	}
void mgl_wnd_adjust_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->Adjust();	}
void mgl_wnd_next_frame_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->NextFrame();	}
void mgl_wnd_prev_frame_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->PrevFrame();	}
void mgl_wnd_animation_(uintptr_t *gr)
{	mglCanvasW *g = dynamic_cast<mglCanvasW *>((HMGL)(*gr));
	if(g)	g->Animation();	}
//-----------------------------------------------------------------------------
