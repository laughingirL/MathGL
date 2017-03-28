/* data.cpp is part of UDAV
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
#include <FL/Fl_Choice.H>
#include <FL/Fl_Spinner.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Check_Button.H>
#include "mgllab.h"
//-----------------------------------------------------------------------------
void option_in_cb(Fl_Widget*, void *v);
void style_in_cb(Fl_Widget*, void *v);
void datsel_in_cb(Fl_Widget*, void *v);
//-----------------------------------------------------------------------------
void data_file(const char *fn)
{
	static int num=0;
	static char name[32], res[256];
	snprintf(name,32,"mgl_%d",num);	num++;
	mglDataA *v = Parse->AddVar(name);
	mglData *d = dynamic_cast<mglData*>(v);
	mglDataC *c = dynamic_cast<mglDataC*>(v);
	if(d)
	{
		d->Read(fn);
		if(d->nz>1)
			snprintf(res,256,"#read %s '%s'\nrotate 40 60\ncrange %s\nbox\nsurf3 %s\n", name, fn, name, name);
		else if(d->ny>1)
			snprintf(res,256,"#read %s '%s'\nrotate 40 60\ncrange %s\nzrange %s\nbox\nsurf %s\n", name, fn, name, name, name);
		else
			snprintf(res,256,"#read %s '%s'\nyrange %s\nbox\nplot %s\n", name, fn, name, name);
		textbuf->text(res);
	}
	else if(c)
	{
		c->Read(fn);
		if(c->nz>1)
			snprintf(res,256,"#read %s '%s'\nrotate 40 60\ncrange %s\nbox\nsurf3 %s\n", name, fn, name, name);
		else if(c->ny>1)
			snprintf(res,256,"#read %s '%s'\nrotate 40 60\ncrange %s\nzrange %s\nbox\nsurf %s\n", name, fn, name, name, name);
		else
			snprintf(res,256,"#read %s '%s'\nyrange %s\nbox\nplot %s\n", name, fn, name, name);
		textbuf->text(res);
	}
}
//-----------------------------------------------------------------------------
