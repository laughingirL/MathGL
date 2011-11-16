/***************************************************************************
 *   Copyright (C) 2008 by Alexey Balakin                                  *
 *   mathgl.abalakin@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QTextBrowser>
#include <QToolButton>
#include <mgl/parser.h>

#include "newcmd_dlg.h"
#include "opt_dlg.h"
#include "style_dlg.h"
extern mglParser parser;
extern QString pathHelp;
//-----------------------------------------------------------------------------
NewCmdDialog::NewCmdDialog(QWidget *p) : QDialog(p,Qt::WindowStaysOnTopHint)
{
	fillList();
	QPushButton *b;
	QToolButton *t;
	QHBoxLayout *m = new QHBoxLayout(this), *a;
	QVBoxLayout *o = new QVBoxLayout;	m->addLayout(o);
	optDialog = new OptionDialog(this);
	stlDialog = new StyleDialog(this);
	type = new QComboBox(this);		o->addWidget(type);
	type->setToolTip(tr("Groups of MGL commands"));
	name = new QComboBox(this);		o->addWidget(name);
	name->setToolTip(tr("MGL commands for selected group"));
	type->addItems(types);	name->addItems(argn[0]);
	info = new QLabel(this);		o->addWidget(info);
	info->setToolTip(tr("Short description of selected command"));
	kind= new QComboBox(this);		o->addWidget(kind);	kind->addItem("");
	kind->setToolTip(tr("Kind of command argument order. The notation is:\n"
						" * Capital arguments are data (like, Ydat);\n"
						" * Argument in '' are strings (like, 'fmt');\n"
						" * Other arguments are numbers (like, zval);\n"
						" * Arguments in [] are optional arguments."));
	args = new QTableWidget(this);	o->addWidget(args);
	args->setToolTip(tr("Command arguments. Bold ones are required arguments.\n"
						"Other are optional arguments but its order is required.\n"
						"You can use '' for default format. See help at right\nfor default values."));
	QStringList sl;	sl<<tr("Argument")<<tr("Value");
	args->setHorizontalHeaderLabels(sl);

	a = new QHBoxLayout;	o->addLayout(a);
	b = new QPushButton(tr("Add style"),this);	a->addWidget(b);
	b->setToolTip(tr("Here you can select the plot style.\nThe result will be placed in 'fmt' argument."));
	connect(b, SIGNAL(clicked()),this,SLOT(insertStl()));
	b = new QPushButton(tr("Add data"),this);	a->addWidget(b);
	connect(b, SIGNAL(clicked()),this,SLOT(insertData()));
	b = new QPushButton(tr("Options"),this);	a->addWidget(b);
	b->setToolTip(tr("Here you can specify command options.\nOptions are used for additional plot tunning."));
	connect(b, SIGNAL(clicked()),this,SLOT(insertOpt()));
	QLabel *l=new QLabel(tr("Options"));	o->addWidget(l);
	opt = new QLineEdit(this);	o->addWidget(opt);
	a = new QHBoxLayout;	o->addLayout(a);
	b = new QPushButton(tr("Cancel"),this);	a->addWidget(b);
	connect(b, SIGNAL(clicked()), this, SLOT(reject()));
	b = new QPushButton(tr("OK"),this);	a->addWidget(b);
	connect(b, SIGNAL(clicked()), this, SLOT(finish()));

	o = new QVBoxLayout;	m->addLayout(o,1);
	a = new QHBoxLayout;	o->addLayout(a);
	help = new QTextBrowser(this);		help->setMinimumWidth(500);
	help->setOpenExternalLinks(false);	o->addWidget(help);
	t = new QToolButton(p);	t->setIcon(QPixmap(":/xpm/go-previous.png"));
	connect(t, SIGNAL(clicked()), help, SLOT(backward()));	a->addWidget(t);
	t = new QToolButton(p);	t->setIcon(QPixmap(":/xpm/go-next.png"));
	connect(t, SIGNAL(clicked()), help, SLOT(forward()));	a->addWidget(t);
	a->addStretch(1);
	t = new QToolButton(p);	t->setIcon(QPixmap(":/xpm/zoom-in.png"));
	connect(t, SIGNAL(clicked()), this, SLOT(zoomIn()));	a->addWidget(t);
	t = new QToolButton(p);	t->setIcon(QPixmap(":/xpm/zoom-out.png"));
	connect(t, SIGNAL(clicked()), this, SLOT(zoomOut()));	a->addWidget(t);

	connect(type, SIGNAL(currentIndexChanged(int)),this,SLOT(typeChanged(int)));
	connect(name, SIGNAL(currentIndexChanged(int)),this,SLOT(nameChanged(int)));
	connect(kind, SIGNAL(currentIndexChanged(int)),this,SLOT(kindChanged(int)));
	type->setCurrentIndex(0);	typeChanged(0);
}
//-----------------------------------------------------------------------------
void NewCmdDialog::zoomIn()
{	QFont f(help->font());	f.setPointSize(f.pointSize()+1);	help->setFont(f);	}
//-----------------------------------------------------------------------------
void NewCmdDialog::zoomOut()
{	QFont f(help->font());	f.setPointSize(f.pointSize()-1);	help->setFont(f);	}
//-----------------------------------------------------------------------------
void NewCmdDialog::parseCmd(const QString &txt)
{
	QString str = txt.trimmed().section(':',0,0);
	QRegExp sep("[ \t]");
	QString cmd = str.section(sep,0,0),a,b;
	bool opt,var,chr;
	for(int i=0;i<17;i++)
	{
		if(cmds[i].contains(cmd))	// find command first
		{
			typeChanged(i);
			nameChanged(cmds[i].indexOf(cmd));
			register int j,j0,k,k0;
			bool ok;
			for(j0=k0=-1,j=0;j<NUM_CH;j++)		// determine set of arguments
			{
				if(argn[j].isEmpty())	break;
				for(ok=true,k=0;k<argn[j].count();k++)
				{
					a = argn[j].at(k);		opt = (a[0]=='_'  || a[0]=='+');
					chr = (a[0]=='\'' || (opt && a[1]=='\''));
					var = (a[0].isUpper() || (opt && a[1].isUpper()));
					b = str.section(sep,k+1,k+1).trimmed();
					if(b.isEmpty())	break;
					if(b[0].isDigit() && (chr || var))	{	ok=false;	break;	}
					if(b[0]=='\'' && !chr)	{	ok=false;	break;	}
				}
				if(ok && (j0<0 || k>k0))	{	j0=j;	k0=k;	}
			}
			if(j0>=0)	// best choice
			{
				kindChanged(j0);
				for(k=0;k<argn[j0].count();k++)
					args->item(k,1)->setText(str.section(sep,k+1,k+1).trimmed());
			}
			return;		// selection is done
		}
	}
}
//-----------------------------------------------------------------------------
void NewCmdDialog::fillList()
{
	types<<tr("1D plots")<<tr("2D plots")<<tr("3D plots")<<tr("Dual plots")
			<<tr("Vector plots")<<tr("Other plots")<<tr("Text and legend")
			<<tr("Create data and I/O")<<tr("Data handling")<<tr("Data extraction")
			<<tr("Axis and colorbar")<<tr("General setup")<<tr("Axis setup")
			<<tr("Scale and rotate")<<tr("Program flow")<<tr("Nonlinear fitting")
			<<tr("Primitives");
	cmds[0]<<"plot"<<"area"<<"bars"<<"barh"<<"boxplot"<<"chart"<<"error"<<"mark"
			<<"region"<<"stem"<<"step"<<"tens"<<"textmark"<<"torus"<<"tube";
	cmds[1]<<"surf"<<"axial"<<"belt"<<"boxs"<<"cont"<<"contd"<<"contf"<<"dens"
			<<"fall"<<"grid2"<<"mesh"<<"tile"<<"grad";
	cmds[2]<<"surf3"<<"cloud"<<"beam"<<"cont3"<<"conta"<<"contf3"<<"contfa"
			<<"dens3"<<"densa"<<"grid3"<<"grida";
	cmds[3]<<"map"<<"stfa"<<"surfa"<<"surfc"<<"tile"<<"surf3a"<<"surf3c";
	cmds[4]<<"flow"<<"pipe"<<"traj"<<"vect"<<"vectc"<<"vectl"<<"dew";
	cmds[5]<<"contx"<<"conty"<<"contz"<<"contfx"<<"contfy"<<"contfz"<<"densx"
			<<"densy"<<"densz"<<"triplot"<<"tricont"<<"quadplot"<<"crust"<<"dots";
	cmds[6]<<"text"<<"title"<<"fgets"<<"legend"<<"addlegend"<<"clearlegend"	<<"legendbox";
	cmds[7]<<"new"<<"var"<<"copy"<<"delete"<<"insert"<<"read"<<"readmat"<<"readall"
			<<"readhdf"<<"save"<<"savehdf"<<"export"<<"import"<<"info"<<"idset";
	cmds[8]<<"fill"<<"fillsample"<<"modify"<<"put"<<"crop"<<"extend"<<"rearrange"<<"squeeze"
			<<"transpose"<<"cumsum"<<"diff"<<"diff2"<<"sinfft"<<"cosfft"<<"hankel"
			<<"envelop"<<"integrate"<<"mirror"<<"norm"<<"normsl"<<"sew"<<"smooth"
			<<"swap"<<"roll"<<"addto"<<"subto"<<"divto"<<"multo";
	cmds[9]<<"combine"<<"evaluate"<<"max"<<"min"<<"hist"<<"jacobian"<<"momentum"
			<<"resize"<<"sum"<<"trace"<<"transform"<<"transforma"<<"stfad"<<"pde"
			<<"qo2d"<<"ray";
	cmds[10]<<"axis"<<"box"<<"colorbar"<<"grid"<<"xlabel"<<"ylabel"<<"zlabel"<<"tlabel";
	cmds[11]<<"alpha"<<"alphadef"<<"transparent"<<"transptype"<<"ambient"<<"light"
			<<"fog"<<"arrowsize"<<"barwidth"<<"linewidth"<<"marksize"<<"plotfactor"
			<<"zoom"<<"cut"<<"axialdir"<<"mesgnum"<<"font"<<"palette"<<"rotatetext";
	cmds[12]<<"axis"<<"ranges"<<"caxis"<<"crange"<<"xrange"<<"yrange"<<"zrange"
			<<"origin"<<"ternary"<<"adjust"<<"ctick"<<"xtick"<<"ytick"<<"ztick"
			<<"ticklen"<<"tickstl";
	cmds[13]<<"subplot"<<"inplot"<<"rotate"<<"aspect"<<"columnplot"<<"perspective";
	cmds[14]<<"call"<<"func"<<"chdir"<<"define"<<"if"<<"elseif"<<"else"<<"endif"<<"for"
			<<"next"<<"once"<<"stop"<<"write"<<"setsize";
	cmds[15]<<"fit"<<"fits"<<"putsfit";
	cmds[16]<<"fplot"<<"fsurf"<<"ball"<<"cone"<<"curve"<<"drop"<<"facex"<<"facey"
			<<"facez"<<"line"<<"rect"<<"sphere";
}
//-----------------------------------------------------------------------------
void NewCmdDialog::typeChanged(int s)
{
	if(s<0 || s>16)	return;
	name->clear();	name->addItems(cmds[s]);	name->setCurrentIndex(0);
}
//-----------------------------------------------------------------------------
void parse(QStringList &sl, const QString &s)
{
	sl.clear();
	int i, ex=0, i1=0;
	bool op=false, sp=true;
	for(i=0;i<s.length();i++)
	{
		if(s[i].isLetterOrNumber() || s[i]=='\'' || s[i]=='(' || s[i]==')' || s[i]=='_')
		{	if(sp)	i1=i;	sp = false;	}
		else
		{
			QString p;
			if(op)	p="_";	else if(ex)	p="+";
			if(!sp)	sl<<p+s.mid(i1,i-i1);
			sp = true;
			if(s[i]=='{')	ex++;
			if(s[i]=='}')	ex--;
			if(s[i]=='[')	op=true;
		}
	}
	if(!sp)	sl<<s.mid(i1);
}
//-----------------------------------------------------------------------------
mglCommand *getCmd(const QString &cmd)
{
	wchar_t *c = new wchar_t[cmd.length()+1];
	cmd.toWCharArray(c);	c[cmd.length()]=0;
	mglCommand *rts = parser.FindCommand(c);
	if(!rts)	rts = parser.FindCommand(c,true);
	delete []c;		return rts;
}
//-----------------------------------------------------------------------------
void NewCmdDialog::nameChanged(int s)
{
	QString n=name->itemText(s), par, a;
	int k;
	if(n.isEmpty())	return;
	QStringList ss;	ss<<(pathHelp+"/"+tr("mgl_en")+".html/");
	help->setSearchPaths(ss);
	help->setSource(tr("mgl_en")+"_1.html#"+n);
	// clear old
	kind->clear();	kinds.clear();	for(k=0;k<NUM_CH;k++)	argn[k].clear();
	// try to find the keyword
	mglCommand *rts=getCmd(n);
	if(!rts)	return;
	info->setText(QString::fromWCharArray(rts->desc));

	par = QString::fromWCharArray(rts->form);
	int i0 = par.indexOf(' ');	// first space if present
	if(i0<0)	{	kind->addItem(par);	return;	}	// no arguments
	// parse kind of arguments
	par = par.mid(i0);
	for(k=0;k<NUM_CH;k++)
	{
		a = par.section('|',k,k);
		if(a.isEmpty())	break;
		a=a.trimmed();
		kinds<<n+" "+a;
		parse(argn[k],a);
	}
	kind->addItems(kinds);	kind->setCurrentIndex(0);
}
//-----------------------------------------------------------------------------
void NewCmdDialog::kindChanged(int s)
{
	if(s<0 || s>NUM_CH-1)	return;
	cmd="";
	int nn = argn[s].count();
//return;
	args->setRowCount(nn);	args->setColumnCount(2);
	QTableWidgetItem *it;
	QString a;
	QFont f;
	bool optional;
	for(int i=0;i<nn;i++)
	{
		it = new QTableWidgetItem;	args->setItem(i,0,it);
		it = new QTableWidgetItem;	args->setItem(i,1,it);
		a = argn[s].at(i);
		optional = (a[0]=='_' || a[0]=='+');	if(optional)	a=a.mid(1);
		f.setItalic(a[0].isUpper());	f.setBold(!optional);
		args->item(i,0)->setText(a);	args->item(i,0)->setFont(f);
		args->item(i,0)->setFlags(Qt::ItemIsEnabled);
		args->item(i,1)->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
	}
}
//-----------------------------------------------------------------------------
void NewCmdDialog::insertData()	{}	// TODO !!!
//-----------------------------------------------------------------------------
void NewCmdDialog::insertOpt()
{	if(optDialog->exec())	opt->setText(optDialog->getOption());	}
//-----------------------------------------------------------------------------
void NewCmdDialog::insertStl()
{
	int s=kind->currentIndex();
	if(s<0 || s>4)
	{	QMessageBox::warning(this,tr("New command"),
			tr("Select first the proper kind of arguments"));
		return;	}
	if(!argn[s].contains("'fmt'") && !argn[s].contains("_'fmt'"))
	{	QMessageBox::warning(this,tr("New command"),
			tr("There is no 'fmt' argument for this command"));
		return;	}
	int i;
	i = argn[s].indexOf("'fmt'");
	if(i<0)	i = argn[s].indexOf("_'fmt'");
	if(!stlDialog->exec())	return;
	args->item(i,1)->setText(stlDialog->getStyle());
}
//-----------------------------------------------------------------------------
void NewCmdDialog::finish()
{
	QString txt,cur;
	int s = kind->currentIndex();
	if(s<0 || s>4)
	{	QMessageBox::warning(this,tr("New command"),
			tr("Select first the proper kind of arguments"));
		cmd="";	return;	}
	cmd = name->currentText();
	int n = argn[s].count(), i;
	bool op=false;
	for(i=0;i<n;i++)
	{
		cur = argn[s].at(i);
		txt = args->item(i,1)->text();
		if(txt.isEmpty())
		{
			if(cur[0]!='_' && cur[0]!='+')
			{	QMessageBox::warning(this,tr("New command"),
					tr("You should specify required argument ")+cur);
				cmd="";	return;	}
			if(argn[s].at(i)[0]=='_')	op = true;
		}
		else
		{
			if(cur[0]=='\'' && txt[0]!='\'')
			{	QMessageBox::warning(this,tr("New command"),
					tr("You should put text inside ' ' for argument ")+cur);
				cmd="";	return;	}
			if(cur[1]=='\'' && txt[0]!='\'')
			{	QMessageBox::warning(this,tr("New command"),
					tr("You should put text inside ' ' for argument ")+cur.mid(1));
				cmd="";	return;	}
			if(cur[0]=='_' && op)
			{	QMessageBox::warning(this,tr("New command"),
					tr("You should specify all optional arguments before ")+cur.mid(1));
				cmd="";	return;	}
			cmd = cmd + ' ' + txt;
		}
	}
	cmd = cmd + opt->text();	accept();	emit result(cmd);
}
//-----------------------------------------------------------------------------
