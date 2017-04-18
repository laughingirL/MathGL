/* table.cpp is part of UDAV
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
#include <FL/Fl_Spinner.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Round_Button.H>
#include "mgllab.h"

//-----------------------------------------------------------------------------
void addto_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	const char *s = fl_input(_mgl("Enter number for addition to data values"),0);
	HMDT d = dynamic_cast<HMDT>(e->var);
	if(d && s)	{	mgl_data_add_num(d, atof(s));	e->refresh();	}
	HADT c = dynamic_cast<HADT>(e->var);
	if(c && s)	{	mgl_datac_add_num(c, mgl_atoc(s,true));	e->refresh();	}
}
//-----------------------------------------------------------------------------
void subto_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	const char *s = fl_input(_mgl("Enter number for subtraction from data values"),0);
	HMDT d = dynamic_cast<HMDT>(e->var);
	if(d && s)	{	mgl_data_sub_num(d, atof(s));	e->refresh();	}
	HADT c = dynamic_cast<HADT>(e->var);
	if(c && s)	{	mgl_datac_sub_num(c, mgl_atoc(s,true));	e->refresh();	}
}
//-----------------------------------------------------------------------------
void multo_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	const char *s = fl_input(_mgl("Enter number for multiplication of data values"),0);
	HMDT d = dynamic_cast<HMDT>(e->var);
	if(d && s)	{	mgl_data_mul_num(d, atof(s));	e->refresh();	}
	HADT c = dynamic_cast<HADT>(e->var);
	if(c && s)	{	mgl_datac_mul_num(c, mgl_atoc(s,true));	e->refresh();	}
}
//-----------------------------------------------------------------------------
void divto_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	const char *s = fl_input(_mgl("Enter number for division of data values"),0);
	HMDT d = dynamic_cast<HMDT>(e->var);
	if(d && s)	{	mgl_data_div_num(d, atof(s));	e->refresh();	}
	HADT c = dynamic_cast<HADT>(e->var);
	if(c && s)	{	mgl_datac_div_num(c, mgl_atoc(s,true));	e->refresh();	}
}
//-----------------------------------------------------------------------------
class XYZDlg : public GeneralDlg
{
	Fl_Box *box;
	Fl_Check_Button *wch;
	Fl_Spinner *wmx, *wmy, *wmz;
public:
	bool OK;
	double rx()	{	return wmx->value();	}
	double ry()	{	return wmy->value();	}
	double rz()	{	return wmz->value();	}
	double nx()	{	return mgl_int(wmx->value());	}
	double ny()	{	return mgl_int(wmy->value());	}
	double nz()	{	return mgl_int(wmz->value());	}
	double ch()	{	return wch->value();	}
	void setup(const char *desc, const char *ch=NULL)
	{
		if(desc)	box->label(desc);
		if(ch && *ch)
		{	wch->label(ch);	wch->activate();	}
		else
		{	wch->label(_mgl("not used"));
			wch->deactivate();	}
	}
	void init()	{	OK=false;	}
	XYZDlg()
	{
		w = new Fl_Double_Window(325, 125, _mgl("Change data sizes"));
		box = new Fl_Box(10, 10, 305, 40);
		box->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
		wmx = new Fl_Spinner(30, 55, 75, 25, "mx");
		wmx->tooltip(_mgl("New size of data on 1st dimension (x-direction)"));
		wmy = new Fl_Spinner(135, 55, 75, 25, "my");
		wmy->tooltip(_mgl("New size of data on 2nd dimension (y-direction)"));
		wmz = new Fl_Spinner(240, 55, 75, 25, "mz");
		wmz->tooltip(_mgl("New size of data on 3d dimension (z-direction)"));
		wch = new Fl_Check_Button(15, 90, 95, 25);
		Fl_Button *o = new Fl_Button(125, 90, 85, 25, _mgl("Cancel"));
		o->tooltip(_mgl("Do nothing and close window"));	o->callback(cb_dlg_cancel,this);
		o = new Fl_Return_Button(230, 90, 85, 25, _mgl("Change"));
		o->tooltip(_mgl("Change (resize) data"));	o->callback(cb_dlg_ok,this);
		w->end();	w->set_modal();
	}
	void cb_ok()	{	OK=true;	hide();	}
	bool run()	{	OK=false;	w->show();	while(w->shown())	Fl::wait();	return OK;	}
} xyz_dlg;
//-----------------------------------------------------------------------------
void new_dat_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	xyz_dlg.setup(_mgl("Specify new data size\nData will be zero filled"));
	if(xyz_dlg.run())
	{
		HMDT d = dynamic_cast<HMDT>(e->var);
		if(d)
		{	d->Create(xyz_dlg.nx(), xyz_dlg.ny(), xyz_dlg.nz());	e->refresh();	}
		HADT c = dynamic_cast<HADT>(e->var);
		if(c)
		{	c->Create(xyz_dlg.nx(), xyz_dlg.ny(), xyz_dlg.nz());	e->refresh();	}
	}
}
//-----------------------------------------------------------------------------
void resize_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	xyz_dlg.setup(_mgl("Specify new data size\nData will be interpolated"));
	if(xyz_dlg.run())
	{
		HMDT d = dynamic_cast<HMDT>(e->var);
		if(d)
		{	*d = d->Resize(xyz_dlg.nx(), xyz_dlg.ny(), xyz_dlg.nz());	e->refresh();	}
		HADT c = dynamic_cast<HADT>(e->var);
		if(c)
		{	*c = c->Resize(xyz_dlg.nx(), xyz_dlg.ny(), xyz_dlg.nz());	e->refresh();	}
	}
}
//-----------------------------------------------------------------------------
void squeeze_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	xyz_dlg.setup(_mgl("Specify the skiping step\nEach m-th point will be skiped"), _mgl("smoothed"));	if(xyz_dlg.run())
	{
		HMDT d = dynamic_cast<HMDT>(e->var);
		if(d)
		{	d->Squeeze(xyz_dlg.nx(), xyz_dlg.ny(), xyz_dlg.nz(), xyz_dlg.ch());	e->refresh();	}
		HADT c = dynamic_cast<HADT>(e->var);
		if(c)
		{	c->Squeeze(xyz_dlg.nx(), xyz_dlg.ny(), xyz_dlg.nz(), xyz_dlg.ch());	e->refresh();	}
	}
}
//-----------------------------------------------------------------------------
class ChngDlg : public GeneralDlg
{
	Fl_Check_Button *dx, *dy, *dz;
	Fl_Choice *kind, *type;
	Fl_Float_Input *val;
public:
	ChngDlg()
	{
		Fl_Menu_Item k[]={{_mgl("Smooth")}, {_mgl("CumSum")}, { _mgl("Integrate")},
		{ _mgl("Difference")}, { _mgl("Double diff.")}, { _mgl("Swap parts")},
		{ _mgl("Sinus FFT")}, { _mgl("Cosine FFT")}, { _mgl("Hankel")},
		{ _mgl("Mirror")}, { _mgl("Roll")}, { _mgl("Sew phase")}, { _mgl("Envelop")}, {0}};
		Fl_Menu_Item t[]={{_mgl("Linear *3")}, {_mgl("Linear *5")}, {_mgl("Parabolic *5")},{0}};
		w = new Fl_Double_Window(165, 215, _mgl("Change data"));
		kind = new Fl_Choice(10, 25, 145, 25, _mgl("Type of operation"));
		kind->align(FL_ALIGN_TOP_LEFT);	kind->copy(k);
		dx = new Fl_Check_Button(10, 55, 140, 25, _mgl("apply in x direction"));
		dy = new Fl_Check_Button(10, 80, 140, 25, _mgl("apply in y direction"));
		dz = new Fl_Check_Button(10, 105, 140, 25, _mgl("apply in z direction"));
		type = new Fl_Choice(10, 145, 145, 25, _mgl("Type of smoothing"));
		type->align(FL_ALIGN_TOP_LEFT);	type->copy(t);
		val = new Fl_Float_Input(10, 145, 145, 25, _mgl("Numeric parameter"));
		val->align(FL_ALIGN_TOP_LEFT);
		Fl_Button *o;
		o = new Fl_Button(10, 180, 65, 25, _mgl("Cancel"));	o->callback(cb_dlg_cancel,this);
		o = new Fl_Return_Button(90, 180, 65, 25, _mgl("Do"));	o->callback(cb_dlg_ok,this);
		w->end();	w->set_modal();
	}
	void cb_ok()
	{
		result.clear();
		if(dx->value())	result += 'x';
		if(dy->value())	result += 'y';
		if(dz->value())	result += 'z';
		if(result.empty())
		{	fl_alert(_mgl("You need to specify direction(s)"));	return;	}
		if(type->value()==0)	result += '3';
		if(type->value()==1)	result += '5';
		HMDT d = dynamic_cast<HMDT>(dat);
		HADT c = dynamic_cast<HADT>(dat);
		const char *r = result.c_str();
		bool err = false;
		double v = val->value() ? atof(val->value()) : 0;
		if(d)	switch(kind->value())
		{
		case 0:	d->Smooth(r);	break;
		case 1:	d->CumSum(r);	break;
		case 2:	d->Integral(r);	break;
		case 3:	d->Diff(r);		break;
		case 4:	d->Diff2(r);	break;
		case 5:	d->Swap(r);		break;
		case 6:	d->SinFFT(r);	break;
		case 7:	d->CosFFT(r);	break;
		case 8:	d->Hankel(r);	break;
		case 9:	d->Mirror(r);	break;
		case 10:d->Roll(*r ,v);	break;
		case 11:d->Sew(r);		break;
		case 12:d->Envelop(*r);	break;
		}
		else if(c)	switch(kind->value())
		{
		case 0:	c->Smooth(r);	break;
		case 1:	c->CumSum(r);	break;
		case 2:	c->Integral(r);	break;
		case 3:	c->Diff(r);		break;
		case 4:	c->Diff2(r);	break;
		case 5:	c->Swap(r);		break;
		case 6:	c->SinFFT(r);	break;
		case 7:	c->CosFFT(r);	break;
		case 8:	c->Hankel(r);	break;
		case 9:	c->Mirror(r);	break;
		case 10:c->Roll(*r, v);	break;
		case 11:err=true;		break;
		case 12:c->Envelop(*r);	break;
		}
		else	err=true;
		if(err)	fl_alert(_mgl("Operation is not supported for this type of data."));
		else	hide();
	}
	void run(int k, mglDataA *d)
	{
		init();	dat=d;	kind->value(k);
		if(k)	{	type->hide();	val->show();	}
		else	{	type->show();	val->hide();	}
		w->show();	while(w->shown())	Fl::wait();
	}
} chng_dlg;
//-----------------------------------------------------------------------------
void smooth_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	chng_dlg.run(0, e->var);
	if(!chng_dlg.result.empty())	e->refresh();
}
//-----------------------------------------------------------------------------
void cumsum_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	chng_dlg.run(1, e->var);
	if(!chng_dlg.result.empty())	e->refresh();
}
//-----------------------------------------------------------------------------
void integr_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	chng_dlg.run(2, e->var);
	if(!chng_dlg.result.empty())	e->refresh();
}
//-----------------------------------------------------------------------------
void diff_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	chng_dlg.run(3, e->var);
	if(!chng_dlg.result.empty())	e->refresh();
}
//-----------------------------------------------------------------------------
void diff2_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	chng_dlg.run(4, e->var);
	if(!chng_dlg.result.empty())	e->refresh();
}
//-----------------------------------------------------------------------------
void swap_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	chng_dlg.run(5, e->var);
	if(!chng_dlg.result.empty())	e->refresh();
}
//-----------------------------------------------------------------------------
void sinfft_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	chng_dlg.run(6, e->var);
	if(!chng_dlg.result.empty())	e->refresh();
}
//-----------------------------------------------------------------------------
void cosfft_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	chng_dlg.run(7, e->var);
	if(!chng_dlg.result.empty())	e->refresh();
}
//-----------------------------------------------------------------------------
void hankel_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	chng_dlg.run(8, e->var);
	if(!chng_dlg.result.empty())	e->refresh();
}
//-----------------------------------------------------------------------------
void mirror_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	chng_dlg.run(9, e->var);
	if(!chng_dlg.result.empty())	e->refresh();
}
//-----------------------------------------------------------------------------
void roll_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	chng_dlg.run(10, e->var);
	if(!chng_dlg.result.empty())	e->refresh();
}
//-----------------------------------------------------------------------------
void sew_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	chng_dlg.run(11, e->var);
	if(!chng_dlg.result.empty())	e->refresh();
}
//-----------------------------------------------------------------------------
void envelop_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	chng_dlg.run(12, e->var);
	if(!chng_dlg.result.empty())	e->refresh();
}
//-----------------------------------------------------------------------------
class NwdtDlg : public GeneralDlg
{
	Fl_Check_Button *dx, *dy, *dz;
	Fl_Choice *kind;
	Fl_Input *name;
public:
	NwdtDlg()
	{
		Fl_Menu_Item k[]={{_mgl("Summation of")}, {_mgl("Maximum of")}, { _mgl("Minimum of")}, { _mgl("Pulse prop.")}, {0}};
		w = new Fl_Double_Window(165, 215, _mgl("Extract data"));
		kind = new Fl_Choice(10, 25, 145, 25, _mgl("Type of operation"));
		kind->align(FL_ALIGN_TOP_LEFT);	kind->copy(k);
		dx = new Fl_Check_Button(10, 55, 140, 25, _mgl("apply in x direction"));
		dy = new Fl_Check_Button(10, 80, 140, 25, _mgl("apply in y direction"));
		dz = new Fl_Check_Button(10, 105, 140, 25, _mgl("apply in z direction"));
		name = new Fl_Input(10, 145, 145, 25, _mgl("Name for output"));
		name->align(FL_ALIGN_TOP_LEFT);

		Fl_Button *o;
		o = new Fl_Button(10, 180, 65, 25, _mgl("Cancel"));	o->callback(cb_dlg_cancel,this);
		o = new Fl_Return_Button(90, 180, 65, 25, _mgl("Do"));	o->callback(cb_dlg_ok,this);
		w->set_modal();	w->end();
	}
	void cb_ok()
	{
		result.clear();
		if(dx->value())	result += 'x';
		if(dy->value())	result += 'y';
		if(dz->value())	result += 'z';
		if(result.empty())
		{	fl_alert(_mgl("You need to specify direction(s)"));	return;	}
		const char *s = name->value();
		if(!s || !s[0])
		{	fl_alert(_mgl("You need to provide output name"));	return;	}
		mglDataA *out = Parse->AddVar(s);

		HMDT d = dynamic_cast<HMDT>(dat);
		HADT c = dynamic_cast<HADT>(dat);
		const char *r = result.c_str();
		if(d)	switch(kind->value())
		{
			case 0:	*out = d->Sum(r);	break;
			case 1:	*out = d->Max(r);	break;
			case 2:	*out = d->Min(r);	break;
			case 3:	*out = d->Pulse(*r);break;
		}
		if(c)	switch(kind->value())
		{
			case 0:	*out = c->Sum(r);	break;
			case 1:	*out = c->Max(r);	break;
			case 2:	*out = c->Min(r);	break;
			case 3:	out = mgl_data_pulse(c,*r);	break;
		}
		hide();
	}
	void run(int k, mglDataA *d)
	{
		init();	dat=d;	kind->value(k);
		w->show();	while(w->shown())	Fl::wait();
	}
} nwdt_dlg;
//-----------------------------------------------------------------------------
void asum_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	nwdt_dlg.run(0, e->var);
	if(!chng_dlg.result.empty())	e->refresh();
}
//-----------------------------------------------------------------------------
void amax_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	nwdt_dlg.run(1, e->var);
	if(!chng_dlg.result.empty())	e->refresh();
}
//-----------------------------------------------------------------------------
void amin_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	nwdt_dlg.run(2, e->var);
	if(!chng_dlg.result.empty())	e->refresh();
}
//-----------------------------------------------------------------------------
void pulse_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	nwdt_dlg.run(3, e->var);
	if(!chng_dlg.result.empty())	e->refresh();
}
//-----------------------------------------------------------------------------
void load_dat_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	const char *newfile = mgl_file_chooser(_mgl("Load Data?"),
		_mgl("DAT Files \t*.{dat,csv}\nHDF Files \t*.{h5,hdf}"));
	if(newfile)
	{
		const char *ext = fl_filename_ext(newfile);
		HMDT d = dynamic_cast<HMDT>(e->var);
		HADT c = dynamic_cast<HADT>(e->var);
		if(!strcmp(ext,"h5") || !strcmp(ext,"hdf"))	// this is HDF file
		{	// TODO add dialog with choice of HDF names
			const char *name = fl_input(_mgl("Enter name of data"),"");
			if(name)
			{
				if(d)	d->ReadHDF(newfile,name);
				if(c)	c->ReadHDF(newfile,name);
				e->refresh();
			}
		}
		else
		{
			if(d)	d->Read(newfile);
			if(c)	c->Read(newfile);
			e->refresh();
		}
	}
}
//-----------------------------------------------------------------------------
void save_dat_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	const char *newfile = mgl_file_chooser(_mgl("Save Data?"),
		_mgl("DAT Files \t*.{dat,csv}\nHDF Files \t*.{h5,hdf}"), true);
	if(newfile)
	{
		const char *ext = fl_filename_ext(newfile);
		if(!strcmp(ext,"h5") || !strcmp(ext,"hdf"))	// this is HDF file
		{
			std::string name = wcstombs(e->var->s);
			e->var->SaveHDF(newfile, name.c_str());
		}
		else	e->var->Save(newfile);
	}
}
//-----------------------------------------------------------------------------
void exp_dat_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	const char *scheme, *newfile = mgl_file_chooser(_mgl("Export Data?"),
		_mgl("PNG Files \t*.png"), true);
	if(newfile)
	{	// TODO show dialog for color scheme
		scheme = fl_input(_mgl("Enter color scheme"),MGL_DEF_SCH);
		if(scheme)	e->var->Export(newfile,scheme);
	}
}
//-----------------------------------------------------------------------------
void imp_dat_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	const char *scheme, *newfile = mgl_file_chooser(_mgl("Import Data?"),
		_mgl("PNG Files \t*.png"));
	HMDT d = dynamic_cast<HMDT>(e->var);
	if(d && newfile)
	{
		scheme = fl_input(_mgl("Enter color scheme"),MGL_DEF_SCH);
		if(scheme)
		{	d->Import(newfile,scheme);	e->refresh();	}
	}
	if(!d)	fl_alert(_mgl("This operation is not supported for this kind of data."));
}
//-----------------------------------------------------------------------------
void list_dat_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	HMDT d = dynamic_cast<HMDT>(e->var);
	HADT c = dynamic_cast<HADT>(e->var);
	if(!d && !c)
	{	fl_message(_mgl("Incorrect type of base data"));	return;	}
	if(e->var->GetNz()>1)	fl_message(_mgl("Only current slice will be inserted"));

	std::string list = "list " + wcstombs(e->var->s);
	long k=e->get_slice(), nx=e->var->GetNx(), ny=e->var->GetNy();
	for(long j=0;j<ny;j++)
	{
		for(long i=0;i<nx;i++)
		{
			if(d)	list += '\t'+mgl_str_num(d->a[i+nx*(j+k*ny)]);
			if(c)	list += '\t'+mgl_str_num(c->a[i+nx*(j+k*ny)]);
		}
		if(j<ny-1)	list += "\t|";
	}
	textbuf->insert(0,list.c_str());
}
//-----------------------------------------------------------------------------
void modify_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	const char *eq=fl_input(_mgl("Enter formula for data modification\nHere x, y, z in range [0,1], u is data value"),0);
	if(eq)
	{
		HMDT d = dynamic_cast<HMDT>(e->var);
		HADT c = dynamic_cast<HADT>(e->var);
		if(d)	{	d->Modify(eq);	e->refresh();	}
		if(c)	{	c->Modify(eq);	e->refresh();	}
	}
}
//-----------------------------------------------------------------------------
class NrmDlg : public GeneralDlg
{
	Fl_Value_Input *wmin, *wmax;
	Fl_Choice *dir;
	Fl_Check_Button *wsym;
public:
	NrmDlg()
	{
		Fl_Menu_Item k[]={{"x"}, {"y"}, { "z"}, {0}};
		w = new Fl_Double_Window(135, 215);
		wmin = new Fl_Value_Input(10, 25, 115, 25, _mgl("Minimal value (v1)"));
		wmin->align(FL_ALIGN_TOP_LEFT);
		wmin->tooltip(_mgl("Minimal value for resulting data values"));
		wmax = new Fl_Value_Input(10, 70, 115, 25, _mgl("Maximal value (v2)"));
		wmax->align(FL_ALIGN_TOP_LEFT);
		wmax->tooltip(_mgl("Maximal value for resulting data values"));
		dir = new Fl_Choice(10, 115, 115, 25, _mgl("Direction"));
		dir->align(FL_ALIGN_TOP_LEFT);	dir->copy(k);	dir->value(0);
		dir->tooltip(_mgl("Direction along which data will be filled"));
		wsym = new Fl_Check_Button(10, 115, 115, 25, _mgl("Symmetrical range"));
		wsym->tooltip(_mgl("Normalize in symmetrical range: -max(|v1|,|v2|) ... max(|v1|,|v2|)"));

		Fl_Button *o;
		o = new Fl_Button(25, 150, 85, 25, _mgl("Cancel"));	o->callback(cb_dlg_cancel,this);
		o->tooltip(_mgl("Do nothing and close this window"));
		o = new Fl_Return_Button(25, 180, 85, 25, _mgl("Change"));	o->callback(cb_dlg_ok,this);
		o->tooltip(_mgl("Change data values and close this window"));
		w->set_modal();	w->end();
	}
	double min()	{	return wmin->value();	}
	double max()	{	return wmax->value();	}
	int sym()		{	return wsym->value();	}
	bool ok()		{	return !result.empty();	}
	void cb_ok()
	{	result = dir->value();	hide();	}
	void run(const char *lbl)
	{
		init();	w->label(lbl?lbl:"");
		w->show();	while(w->shown())	Fl::wait();
	}
} nrm_dlg;
//-----------------------------------------------------------------------------
void fill_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	nrm_dlg.run(_mgl("Fill in range"));
	if(nrm_dlg.ok())
	{
		HMDT d = dynamic_cast<HMDT>(e->var);
		HADT c = dynamic_cast<HADT>(e->var);
		char ch = nrm_dlg.result[0];
		if(d)	{	d->Fill(nrm_dlg.min(),nrm_dlg.max(),ch);	e->refresh();	}
		if(c)	{	c->Fill(nrm_dlg.min(),nrm_dlg.max(),ch);	e->refresh();	}
	}
}
//-----------------------------------------------------------------------------
void normal_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	nrm_dlg.run(_mgl("Fill in range"));
	if(nrm_dlg.ok())
	{
		HMDT d = dynamic_cast<HMDT>(e->var);
		HADT c = dynamic_cast<HADT>(e->var);
		if(d)	{	d->Norm(nrm_dlg.min(),nrm_dlg.max(),nrm_dlg.sym());	e->refresh();	}
		if(c)	{	c->Fill(nrm_dlg.min(),nrm_dlg.max(),nrm_dlg.sym());	e->refresh();	}
	}
}
//-----------------------------------------------------------------------------
struct CropDlg : public GeneralDlg
{
	Fl_Spinner *x1,*x2, *y1,*y2, *z1,*z2;
	CropDlg()
	{
		w = new Fl_Double_Window(230, 155, _mgl("Crop data"));
		x1 = new Fl_Spinner(45, 25, 80, 25, _mgl("Lower bound"));	x1->align(FL_ALIGN_TOP);
		x2 = new Fl_Spinner(140, 25, 80, 25, _mgl("Upper bound"));	x2->align(FL_ALIGN_TOP);
		y1 = new Fl_Spinner(45, 55, 80, 25);
		y2 = new Fl_Spinner(140, 55, 80, 25);
		z1 = new Fl_Spinner(45, 85, 80, 25);
		z2 = new Fl_Spinner(140, 85, 80, 25);

		new Fl_Box(15, 25, 25, 25, "X");
		new Fl_Box(15, 55, 25, 25, "Y");
		new Fl_Box(15, 85, 25, 25, "Z");
		Fl_Button *o;
		o = new Fl_Button(45, 120, 75, 25, _mgl("Cancel"));	o->callback(cb_dlg_cancel,this);
		o->tooltip(_mgl("Do nothing and close this window"));
		o = new Fl_Return_Button(145, 120, 75, 25, _mgl("Crop"));	o->callback(cb_dlg_ok,this);
		o->tooltip(_mgl("Change data values and close this window"));
		w->set_modal();	w->end();
	}
	void init()
	{
		long nx=dat->GetNx(), ny=dat->GetNy(), nz=dat->GetNz();
		x1->range(0,nx-1);	x1->value(0);
		x2->range(0,nx-1);	x2->value(nx-1);
		y1->range(0,nx-1);	y1->value(0);
		y2->range(0,nx-1);	y2->value(ny-1);
		z1->range(0,nx-1);	z1->value(0);
		z2->range(0,nx-1);	z2->value(nz-1);
	}
	void cb_ok()
	{
		long n1,n2;
		HMDT d = dynamic_cast<HMDT>(dat);
		HADT c = dynamic_cast<HADT>(dat);
		n1=mgl_int(x1->value());	n2=mgl_int(x2->value());
		if(d)	d->Crop(n1,n2,'x');
		if(c)	c->Crop(n1,n2,'x');
		n1=mgl_int(y1->value());	n2=mgl_int(y2->value());
		if(d)	d->Crop(n1,n2,'y');
		if(c)	c->Crop(n1,n2,'y');
		n1=mgl_int(z1->value());	n2=mgl_int(z2->value());
		if(d)	d->Crop(n1,n2,'z');
		if(c)	c->Crop(n1,n2,'z');
	}
	void run(mglDataA *d)
	{
		dat = d;	init();
		w->show();	while(w->shown())	Fl::wait();
	}
} crop_dlg;
//-----------------------------------------------------------------------------
void crop_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	crop_dlg.run(e->var);	e->refresh();
}
//-----------------------------------------------------------------------------
struct TrspDlg : public GeneralDlg
{
	Fl_Choice *how;
public:
	TrspDlg()
	{
		Fl_Menu_Item k[]={{"yxz"}, {"zxy"}, {"zyx"}, {"yzx"}, {"xzy"}, {0}};
		w = new Fl_Double_Window(200, 90, _mgl("Transpose data"));
		how = new Fl_Choice(10, 20, 180, 25, _mgl("New order of dimensions"));
		how->align(FL_ALIGN_TOP_LEFT);	how->copy(k);	how->value(0);
		Fl_Button *o;
		o = new Fl_Button(30, 55, 75, 25, _mgl("Cancel"));	o->callback(cb_dlg_cancel,this);
		o->tooltip(_mgl("Do nothing and close this window"));
		o = new Fl_Return_Button(115, 55, 75, 25, _mgl("Do"));	o->callback(cb_dlg_ok,this);
		o->tooltip(_mgl("Change data values and close this window"));
		w->set_modal();	w->end();
	}
	void cb_ok()
	{
		HMDT d = dynamic_cast<HMDT>(dat);
		HADT c = dynamic_cast<HADT>(dat);
		if(d)	d->Transpose(how->text());
		if(c)	c->Transpose(how->text());
	}
	void run(mglDataA *d)
	{
		dat = d;	init();
		w->show();	while(w->shown())	Fl::wait();
	}
} trsp_dlg;
//-----------------------------------------------------------------------------
void transp_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	trsp_dlg.run(e->var);	e->refresh();
}
//-----------------------------------------------------------------------------
void first_sl_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	e->slice->value(0);
	e->set_slice(0);
	e->go_home();
}
//-----------------------------------------------------------------------------
void last_sl_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	e->slice->value(e->num_slice()-1);
	e->set_slice(e->num_slice()-1);
	e->go_home();
}
//-----------------------------------------------------------------------------
void prev_sl_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	int p = int(e->slice->value())-1;
	if(p<0)	p = 0;
	e->slice->value(p);		e->set_slice(p);
	e->go_home();
}
//-----------------------------------------------------------------------------
void next_sl_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	int p = int(e->slice->value())+1;
	if(p>=e->num_slice())	p = e->num_slice()-1;
	e->slice->value(p);		e->set_slice(p);
	e->go_home();
}
//-----------------------------------------------------------------------------
void first_cl_cb(Fl_Widget*, void*v)
{
	TableWindow* e = (TableWindow*)v;
	e->go_home();
}
//-----------------------------------------------------------------------------
void change_sl_cb(Fl_Widget*w, void*v)
{
	TableWindow* e = (TableWindow*)v;
	e->set_slice(long(e->slice->value()));
	e->go_home();
}
//-----------------------------------------------------------------------------
Fl_Menu_Item tablemenu[60] = {
	{ _mgl("General"), 0, 0, 0, FL_SUBMENU },
		{ _mgl("Load from file"),0, load_dat_cb },
		{ _mgl("Import from PNG"),0, imp_dat_cb },
		{ _mgl("Save to file"),	0, save_dat_cb },
		{ _mgl("Export to PNG"),	0, exp_dat_cb, 0, FL_MENU_DIVIDER },
		{ _mgl("Insert as list"),0, list_dat_cb },
//		{ _mgl("Plot data"),		0, plot_dat_cb },
//		{ _mgl("Info for data"),	0, info_dat_cb },
		{ 0 },
	{ _mgl("Sizes"), 0, 0, 0, FL_SUBMENU },
		{ _mgl("Create new"),	0, new_dat_cb },
		{ _mgl("Resize"),		0, resize_cb },
		{ _mgl("Squeeze"),	0, squeeze_cb },
		{ _mgl("Crop"),		0, crop_cb },
		{ _mgl("Transpose"),	0, transp_cb },
//		{ _mgl("Extend"),	0, extend_cb },
		{ 0 },
	{ _mgl("Fill"), 0, 0, 0, FL_SUBMENU },
		{ _mgl("By formula"),	0, modify_cb },
		{ _mgl("In range"),	0, fill_cb },
		{ _mgl("Normalize"),	0, normal_cb },
		{ 0 },
	{ _mgl("Change"), 0, 0, 0, FL_SUBMENU },
		{ _mgl("Smooth"),	0, smooth_cb },
		{ _mgl("CumSum"),	0, cumsum_cb },
		{ _mgl("Integrate"),	0, integr_cb },
		{ _mgl("Difference"),0, diff_cb },
		{ _mgl("Laplacian"),	0, diff2_cb },
		{ _mgl("Swap parts"),0, swap_cb },
		{ _mgl("Sin FFT"),	0, sinfft_cb },
		{ _mgl("Cos FFT"),	0, cosfft_cb },
		{ _mgl("Hankel"),	0, hankel_cb },
//		{ _mgl("Wavelet"),	0, wavelet_cb },
		{ _mgl("Mirror"),	0, mirror_cb },
		{ _mgl("Roll"),		0, roll_cb },
		{ _mgl("Sew phase"),	0, sew_cb },
		{ _mgl("Envelop"),	0, envelop_cb },
		{ 0 },
	{ _mgl("Another"), 0, 0, 0, FL_SUBMENU },
//		{ _mgl("Histogram of"),	0, hist_cb },
		{ _mgl("Summation of"),	0, asum_cb },
		{ _mgl("Maximum of"),	0, amax_cb },
		{ _mgl("Minimum of"),	0, amin_cb },
		{ _mgl("Pulse prop."),	0, pulse_cb },
		{ 0 },
	{ _mgl("Operations"), 0, 0, 0, FL_SUBMENU },
		{ _mgl("Add to"),		0, addto_cb },
		{ _mgl("Subtract to"),0, subto_cb },
		{ _mgl("Multiply by"),0, multo_cb },
		{ _mgl("Divide by"),	0, divto_cb },
		{ 0 },
	{ _mgl("Navigation"), 0, 0, 0, FL_SUBMENU },
		{ _mgl("First slice"), FL_CTRL + FL_F + 1, first_sl_cb },
		{ _mgl("Prev slice"), FL_CTRL + FL_F + 2, prev_sl_cb },
		{ _mgl("Next slice"), FL_CTRL + FL_F + 3, next_sl_cb },
		{ _mgl("Last slice"),	FL_CTRL + FL_F + 4, last_sl_cb, 0, FL_MENU_DIVIDER },
		{ _mgl("First cell"), FL_ALT + FL_F + 1, first_cl_cb },
//		{ _mgl("Last cell"), FL_ALT + FL_F + 2, last_cl_cb },
//		{ _mgl("Center grid"), FL_ALT + FL_F + 3, center_cl_cb },
		{ 0 },
	{ 0 }
};
//-----------------------------------------------------------------------------
void plot_dat_cb(Fl_Widget*,void *v)
{
	TableWindow *e=(TableWindow*)v;
	info_dlg_cb(e->var);
}
//-----------------------------------------------------------------------------
#include "../widgets/image.h"
#include "xpm/document-import.xpm"
#include "xpm/document-export.xpm"
#include "xpm/diff.xpm"
#include "xpm/func.xpm"
#include "xpm/size.xpm"
#include "xpm/tran.xpm"
#include "xpm/crop.xpm"
#include "xpm/go-first.xpm"
#include "xpm/go-last.xpm"
TableWindow::TableWindow(ScriptWindow *e)
{
	main = e;
	const int ww = 600, hh = 430;	// initial width (>300) and height (>430)
	w = new Fl_Double_Window(ww,430);		var = 0;
	menu = new Fl_Menu_Bar(0, 0, ww, 30);	menu->copy(tablemenu, this);
	Fl_Button *o;
	Fl_Group *g = new Fl_Group(0,0,30,350);
	o = new Fl_Button(0, 30, 25, 25);	o->image(img_new);
	o->callback(new_dat_cb,this);		o->tooltip(_mgl("Create new data with zero filling"));
	o = new Fl_Button(0, 55, 25, 25);	o->image(img_load);
	o->callback(load_dat_cb,this);		o->tooltip(_mgl("Load data from file"));
	o = new Fl_Button(0, 80, 25, 25);	o->image(new Fl_Pixmap(document_import_xpm));
	o->callback(imp_dat_cb,this);		o->tooltip(_mgl("Import data from PNG file"));
	o = new Fl_Button(0, 105, 25, 25);	o->image(img_save);
	o->callback(save_dat_cb,this);		o->tooltip(_mgl("Save data to file"));
	o = new Fl_Button(0, 130, 25, 25);	o->image(new Fl_Pixmap(document_export_xpm));
	o->callback(exp_dat_cb,this);		o->tooltip(_mgl("Export data to PNG file"));

	o = new Fl_Button(0, 160, 25, 25);	o->image(img_insert);
	o->callback(list_dat_cb,this);		o->tooltip(_mgl("Insert to script as 'list' command"));
	o = new Fl_Button(0, 185, 25, 25);	o->image(img_plot);
	o->callback(plot_dat_cb,this);		o->tooltip(_mgl("Data info and preview"));

	o = new Fl_Button(0, 215, 25, 25);	o->image(new Fl_Pixmap(diff_xpm));
	o->callback(smooth_cb,this);		o->tooltip(_mgl("Apply operator (smoothing, integration, difference ...) to data"));
	o = new Fl_Button(0, 240, 25, 25);	o->image(new Fl_Pixmap(func_xpm));
	o->callback(modify_cb,this);		o->tooltip(_mgl("Fill data by formula"));
	o = new Fl_Button(0, 265, 25, 25);	o->image(new Fl_Pixmap(size_xpm));
	o->callback(resize_cb,this);		o->tooltip(_mgl("Resize data with smoothing"));
	o = new Fl_Button(0, 290, 25, 25);	o->image(new Fl_Pixmap(crop_xpm));
	o->callback(crop_cb,this);		o->tooltip(_mgl("Crop (cut off edges) data"));
	o = new Fl_Button(0, 315, 25, 25);	o->image(new Fl_Pixmap(tran_xpm));
	o->callback(transp_cb,this);		o->tooltip(_mgl("Transpose data dimensions"));
	g->end();	g->resizable(0);

	g = new Fl_Group(30, 30, 200, 30);
	o = new Fl_Button(30, 30, 25, 25);	o->image(new Fl_Pixmap(go_first_xpm));
	o->callback(first_sl_cb,this);		o->tooltip(_mgl("Go to first slice (Ctrl-F1)"));
	slice = new Fl_Counter(55, 30, 90, 25, 0);	slice->callback(change_sl_cb,this);
	slice->lstep(10);	slice->step(1);	slice->tooltip(_mgl("Id of slice on third (z-) dimension"));
	o = new Fl_Button(147, 30, 25, 25);	o->image(new Fl_Pixmap(go_last_xpm));
	o->callback(last_sl_cb,this);		o->tooltip(_mgl("Go to last slice (Ctrl-F4)"));
	g->end();	g->resizable(0);

	data = new Fl_Data_Table(30,60,ww-30,hh-60);
	data->row_header(1);	data->row_header_width(80);
	data->row_resize(1);	data->rows(1);
	data->row_height_all(25);
	data->col_header(1);	data->col_header_height(25);
	data->col_resize(1);	data->cols(1);
	data->col_width_all(80);

	w->end();	w->resizable(data);
}
//-----------------------------------------------------------------------------
TableWindow::~TableWindow()	{	if(var)	var->o=NULL;	Fl::delete_widget(w);	}
//-----------------------------------------------------------------------------
void delete_cb(void *v)
{
	if(v)
	{
		TableWindow *w = (TableWindow *)v;
		w->var->o=NULL;		delete w;
	}
}
//-----------------------------------------------------------------------------
void TableWindow::update(mglDataA *v)
{
	static std::string name;
	if(v==0)	return;
	name = wcstombs(v->s);
	w->label(name.c_str());
	v->func = delete_cb;
	if(var)	var->o = 0;
	var = v; 	v->o = this;
	refresh();
}
//-----------------------------------------------------------------------------
void TableWindow::refresh()
{
	if(var==0)	return;
	w->deactivate();	nz = var->GetNz();
	sl = 0;	slice->range(0,nz-1);

	data->rows(var->GetNy());	data->cols(var->GetNx());
	data->ny = var->GetNy();	data->nx = var->GetNx();
	data->data = var;
	w->activate();
//	show();
}
//-----------------------------------------------------------------------------
void TableWindow::set_slice(long s)
{
	if(s>=0 && s<nz)
	{
		data->sl = sl = s;
		refresh();
	}
}
//-----------------------------------------------------------------------------
void TableWindow::go_home()
{
}
//-----------------------------------------------------------------------------
