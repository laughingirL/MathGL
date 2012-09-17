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
#include <QTableWidget>
#include <QLabel>
#include <QAction>
#include <QLayout>
#include <QMenuBar>
#include <QLineEdit>
#include <QMenu>
#include <QClipboard>
#include <QStatusBar>
#include <QFileDialog>
#include <QPushButton>
#include <QApplication>
#include <QInputDialog>
#include <QToolButton>
#include <QSpinBox>
#include <mgl2/mgl.h>
//-----------------------------------------------------------------------------
#include "dat_pnl.h"
#include "info_dlg.h"
#include "xpm/table.xpm"
//-----------------------------------------------------------------------------
extern mglParse parser;
void updateDataItems();
void addDataPanel(QWidget *wnd, QWidget *w, QString name);
void deleteDat(void *o)		{	if(o)	delete ((DatPanel *)o);	}
void refreshData(QWidget *w)	{	((DatPanel *)w)->refresh();	}
//-----------------------------------------------------------------------------
QWidget *newDataWnd(InfoDialog *inf, QWidget *wnd, mglVar *v)
{
	DatPanel *t = new DatPanel(inf);
	if(v)	t->setVar(v);
	addDataPanel(wnd,t,t->dataName());
	return t;
}
//-----------------------------------------------------------------------------
DatPanel::DatPanel(InfoDialog *inf, QWidget *parent) : QWidget(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);
	kz = nx = ny = nz = 0;	var = 0;
	ready = false;	infoDlg = inf;
	QBoxLayout *v,*h,*m;

	menu = new QMenu(tr("&Data"),this);
	v = new QVBoxLayout(this);
	h = new QHBoxLayout();	v->addLayout(h);	toolTop(h);
	h = new QHBoxLayout();	v->addLayout(h);
	m = new QVBoxLayout();	h->addLayout(m);	toolLeft(m);
	tab = new QTableWidget(this);	h->addWidget(tab);
	connect(tab, SIGNAL(cellChanged(int,int)), this, SLOT(putValue(int, int)));

	setWindowIcon(QPixmap(table_xpm));
}
//-----------------------------------------------------------------------------
DatPanel::~DatPanel()	{	if(var)	var->o = 0;	}
//-----------------------------------------------------------------------------
void DatPanel::refresh()
{
	bool rc = false;
	if(!var)	return;
	infoDlg->allowRefresh=false;
	if(nx!=var->nx)	{	nx = var->nx;	tab->setColumnCount(nx);	rc=true;	}
	if(ny!=var->ny)	{	ny = var->ny;	tab->setRowCount(ny);	rc=true;	}
	if(kz>=var->nz)	{	kz = 0;			emit sliceChanged(0);	}
	if(nz!=var->ny)	{	nz = var->nz;	emit nzChanged(nz);		}
	id = QString(var->id.c_str());
	if(nz==1 && ny>1 && !id.isEmpty())
	{
		QStringList head;
		QString s;
		for(int i=0;i<ny;i++)
		{
			s = QString("%1").arg(i);
			if(id[i]>='a' && id[i]<='z')	s=s+" ("+id[i]+")";
			head<<s;
		}
		tab->setHorizontalHeaderLabels(head);
	}
	register long i,j,m=var->s.length();
	register mreal f;
	QString s,d;
	if(rc)
	{
		QStringList sh,sv;
		for(i=0;i<nx;i++)	sh<<QString::number(i);
		tab->setHorizontalHeaderLabels(sh);
		for(i=0;i<ny;i++)	sv<<QString::number(i);
		tab->setVerticalHeaderLabels(sv);
		for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
			tab->setItem(j,i,new QTableWidgetItem);
	}
	for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
	{
		f = var->GetVal(i,j,kz);
		if(mgl_isnan(f))	s = "nan";
		else	s.sprintf("%g",f);
		tab->item(j,i)->setText(s);
	}
	infoDlg->allowRefresh=true;	infoDlg->refresh();
	QChar *ss = new QChar[m+1];
	for(i=0;i<m;i++)	ss[i] = var->s[i];
	s = QString(ss, m);	delete []ss;
	d.sprintf("%d * %d * %d", nx, ny, nz);
	ready = true;
}
//-----------------------------------------------------------------------------
void DatPanel::setVar(mglVar *v)
{
	ready = false;
	if(var)	var->o = 0;
	var = v;	infoDlg->setVar(v);
	nx = ny = nz = kz = 0;
	if(v)
	{
		QString s = QString::fromStdWString(v->s);
		v->o = this;	v->func = deleteDat;
		refresh();
		setWindowTitle(s + tr(" - UDAV variable"));
		infoDlg->setWindowTitle(s + tr(" - UDAV preview"));
	}
	else
	{	tab->setColumnCount(0);	tab->setRowCount(0);	emit nzChanged(nz);	}
	emit sliceChanged(0);
}
//-----------------------------------------------------------------------------
void DatPanel::setSlice(int k)
{
	if(k>=nz)	k=nz-1;	if(k<0)	k=0;
	if(k!=kz)
	{
		infoDlg->setSlice(k);
		emit sliceChanged(k);
		kz = k;		refresh();
	}
}
//-----------------------------------------------------------------------------
void DatPanel::putValue(int r, int c)
{
	if(!var || r<0 || c<0 || r>=ny || c>=nx || !ready)	return;
	QString s = tab->item(r,c)->text().toLower();
	mreal f;
	f = s=="nan" ? NAN : s.toDouble();
	if(f!=var->GetVal(c,r,kz))
	{
		if(mgl_isnan(f))	s="nan";	else	s.sprintf("%g", f);
		tab->item(r,c)->setText(s);
	}
	var->SetVal(f,c,r,kz);
	infoDlg->refresh();
}
//-----------------------------------------------------------------------------
void DatPanel::imprt()
{
	QString fn = QFileDialog::getOpenFileName(this, tr("UDAV - Import PNG"), "", tr("Data files (*.dat)\nAll files (*.*)"));
	if(!fn.isEmpty())
	{
		bool ok;
		QString s = QInputDialog::getText(this, tr("UDAV - Export to PNG"), tr("Enter color scheme for picture.\nNote that data will be normalized in range [0,1]."), QLineEdit::Normal, "BbcyrR", &ok);
		if(ok)	var->Import(fn.toAscii(), s.toAscii());
		refresh();
	}
}
//-----------------------------------------------------------------------------
void DatPanel::exprt()
{
	QString fn = QFileDialog::getOpenFileName(this, tr("UDAV - Import PNG"), "", tr("PNG files (*.png)\nAll files (*.*)"));
	if(!fn.isEmpty())
	{
		bool ok;
		QString s = QInputDialog::getText(this, tr("UDAV - Export to PNG"), tr("Enter color scheme for picture"), QLineEdit::Normal, "BbcyrR", &ok);
		if(ok)	var->Export(fn.toAscii(), s.toAscii());
	}
}
//-----------------------------------------------------------------------------
void DatPanel::save()
{
	QString fn = QFileDialog::getSaveFileName(this, tr("UDAV - Save data"), "", tr("Data files (*.dat)\nAll files (*.*)"));
	if(!fn.isEmpty())	var->Save(fn.toAscii());
}
//-----------------------------------------------------------------------------
void DatPanel::load()
{
	QString fn = QFileDialog::getOpenFileName(this, tr("UDAV - Load data"), "", tr("Data files (*.dat)\nAll files (*.*)"));
	if(!fn.isEmpty())	{	var->Read(fn.toAscii());	refresh();	}
}
//-----------------------------------------------------------------------------
void DatPanel::copy()
{
	QTableWidgetSelectionRange ts = tab->selectedRanges().first();
	register long i,j;
	QString res, s;
	for(j=ts.topRow();j<=ts.bottomRow();j++)
	{
		for(i=ts.leftColumn();i<=ts.rightColumn();i++)
		{
			res = res + tab->item(j,i)->text();
			if(i<ts.rightColumn())	res = res + "\t";
		}
		res = res + "\n";
	}
	QApplication::clipboard()->setText(res, QClipboard::Clipboard);
}
//-----------------------------------------------------------------------------
void DatPanel::paste()
{
	QString txt = QApplication::clipboard()->text(QClipboard::Clipboard);
	QString s, t;
	int r = tab->currentRow(), c = tab->currentColumn(), i, j;
	for(i=0;i<ny-r;i++)
	{
		s = txt.section('\n',i,i,QString::SectionSkipEmpty);
		if(s.isEmpty())	break;
		for(j=0;j<nx-c;j++)
		{
			t = s.section('\t',j,j,QString::SectionSkipEmpty);
			if(t.isEmpty())	{	j=nx;	continue;	}
			var->SetVal(t.toDouble(),j+c,i+r,kz);
		}
	}
	refresh();
}
//-----------------------------------------------------------------------------
void DatPanel::plot()	// TODO: plot dialog
{

}
//-----------------------------------------------------------------------------
void DatPanel::list()	// TODO: in which script insert ???
{
/*	if(nx*ny+ny > 1020)
	{	QMessageBox::warning(this, tr("UDAV - To list conversion"), tr("Too many numbers (>1000) on slice"), QMessageBox::Ok, 0, 0);	return;	}
	if(nz > 1)
		QMessageBox::information(this, tr("UDAV - To list conversion"), tr("Only current slice will be inserted"), QMessageBox::Ok, 0, 0);
	QString res = "list\t", s;
	register long i,j;
	for(j=0;j<ny;j++)
	{
		for(i=0;i<nx;i++)
		{
			s.sprintf("%g\t",d->a[i+nx*(j+kz*ny)]);
			res += s;
		}
		if(j<ny-1)	res = res + "|\t";
	}*/
}
//-----------------------------------------------------------------------------
void DatPanel::byformula()
{
	bool ok;
	QString s = QInputDialog::getText(this, tr("UDAV - Fill data"), tr("Enter formula for data filling.\nNote that variables x,y,z supposed to be in range [0,1]."), QLineEdit::Normal, "", &ok);
	if(ok)	{	var->Modify(s.toAscii());	refresh();	}
}
//-----------------------------------------------------------------------------
void DatPanel::inrange()
{
	QString v1("-1"), v2("1"), dir("x");
	if(sizesDialog(tr("UDAV - Fill data"), tr("Enter range for data and direction of filling"), tr("From"), tr("To"), tr("Direction"), v1, v2, dir))
	{
		var->Fill(v1.toDouble(), v2.toDouble(), dir[0].toLatin1());
		refresh();
	}
}
//-----------------------------------------------------------------------------
void DatPanel::norm()
{
	QString v1("0"), v2("1"), how;
	if(sizesDialog(tr("UDAV - Normalize data"), tr("Enter range for final data"), tr("From"), tr("To"), tr("Symmetrical?"), v1, v2, how))
	{
		var->Norm(v1.toDouble(), v2.toDouble(), (how=="on" || how.contains('s')));
		refresh();
	}
}
//-----------------------------------------------------------------------------
void DatPanel::normsl()
{
	QString v1("0"), v2("1"), dir("z");
	if(sizesDialog(tr("UDAV - Normalize by slice"), tr("Enter range for final data"), tr("From"), tr("To"), tr("Direction"), v1, v2, dir))
	{
		var->NormSl(v1.toDouble(), v2.toDouble(), dir[0].toLatin1());
		refresh();
	}
}
//-----------------------------------------------------------------------------
void DatPanel::create()
{
	QString mx, my("1"), mz("1");
	if(sizesDialog(tr("UDAV - Clear data"), tr("Enter new data sizes"), tr("X-size"), tr("Y-size"), tr("Z-size"), mx, my, mz))
	{
		var->Create(mx.toInt(), my.toInt(), mz.toInt());
		refresh();	updateDataItems();
	}
}
//-----------------------------------------------------------------------------
void DatPanel::reSize()
{
	QString mx, my, mz;
	mx.sprintf("%d",nx);	my.sprintf("%d",ny);	mz.sprintf("%d",nz);
	if(sizesDialog(tr("UDAV - Resize data"), tr("Enter new data sizes"), tr("X-size"), tr("Y-size"), tr("Z-size"), mx, my, mz))
	{
		var->Set(var->Resize(mx.toInt(), my.toInt(), mz.toInt()));
		refresh();	updateDataItems();
	}
}
//-----------------------------------------------------------------------------
void DatPanel::squize()
{
	QString mx("1"), my("1"), mz("1");
	if(sizesDialog(tr("UDAV - Squeeze data"), tr("Enter step of saved points. For example, '1' save all, '2' save each 2nd point, '3' save each 3d and so on."), tr("X-direction"), tr("Y-direction"), tr("Z-direction"), mx, my, mz))
	{
		var->Squeeze(mx.toInt(), my.toInt(), mz.toInt());
		refresh();	updateDataItems();
	}
}
//-----------------------------------------------------------------------------
void DatPanel::crop()
{
	QString n1("1"), n2("1"), dir;
	if(sizesDialog(tr("UDAV - Crop data"), tr("Enter range of saved date."), tr("From"), tr("To"), tr("Direction"), n1, n2, dir))
	{
		var->Squeeze(n1.toInt(), n2.toInt(), dir[0].toLatin1());
		refresh();	updateDataItems();
	}
}
//-----------------------------------------------------------------------------
void DatPanel::rearrange()
{
	QString mx, my, mz;
	mx.sprintf("%d",nx);	my.sprintf("%d",ny);	mz.sprintf("%d",nz);
	if(sizesDialog(tr("UDAV - Rearrange data"), tr("Enter new data sizes"), tr("X-size"), tr("Y-size"), tr("Z-size"), mx, my, mz))
	{
		var->Rearrange(mx.toInt(), my.toInt(), mz.toInt());
		refresh();	updateDataItems();
	}
}
//-----------------------------------------------------------------------------
void DatPanel::transp()
{
	bool ok;
	QString s = QInputDialog::getText(this, tr("UDAV - Transpose data"), tr("Enter new order of dimensions.\nFor example, 'yx' or 'yxz' for transpose x-y, 'zyx' for transposing x-z and so on."), QLineEdit::Normal, "yx", &ok);
	if(ok)	{	var->Transpose(s.toAscii());	refresh();	updateDataItems();	}
}
//-----------------------------------------------------------------------------
void DatPanel::smooth()
{
	bool ok;
	QString s = QInputDialog::getText(this, tr("UDAV - Smooth data"), tr("Enter direction(s) for smoothing.\nOptionally you may enter the kind of smoothing by 3 or by 5 points. For example 'xy3' - smooth only in x and y directions and use 3-points scheme."), QLineEdit::Normal, "xyz", &ok);
	if(ok)	{	var->Smooth(s.toAscii().constData());	refresh();	}
}
//-----------------------------------------------------------------------------
void DatPanel::cumsum()
{
	bool ok;
	QString s = QInputDialog::getText(this, tr("UDAV - Summarize data"), tr("Enter direction(s) for cumulative summation.\nFor example 'xy' - summate along x and y directions."), QLineEdit::Normal, "", &ok);
	if(ok)	{	var->CumSum(s.toAscii());	refresh();	}
}
//-----------------------------------------------------------------------------
void DatPanel::integr()
{
	bool ok;
	QString s = QInputDialog::getText(this, tr("UDAV - Integrate data"), tr("Enter direction(s) for integration.\nFor example 'xy' - integrate along x and y directions."), QLineEdit::Normal, "", &ok);
	if(ok)	{	var->Integral(s.toAscii());	refresh();	}
}
//-----------------------------------------------------------------------------
void DatPanel::diff()
{
	bool ok;
	QString s = QInputDialog::getText(this, tr("UDAV - Differentiate data"), tr("Enter direction(s) for differentiation.\nFor example 'xy' - differentiate along x and y directions."), QLineEdit::Normal, "", &ok);
	if(ok)	{	var->Diff(s.toAscii());		refresh();	}
}
//-----------------------------------------------------------------------------
void DatPanel::diff2()
{
	bool ok;
	QString s = QInputDialog::getText(this, tr("UDAV - Laplace transform"), tr("Enter direction(s) for laplace transform.\nFor example 'xy' - do transform along x and y directions."), QLineEdit::Normal, "", &ok);
	if(ok)	{	var->Diff2(s.toAscii());	refresh();	}
}
//-----------------------------------------------------------------------------
void DatPanel::swap()
{
	bool ok;
	QString s = QInputDialog::getText(this, tr("UDAV - Swap data"), tr("Enter direction(s) for swapping (exchange left and right parts).\nFor example 'xy' - swap along x and y directions. Useful for Fourier spectrum."), QLineEdit::Normal, "", &ok);
	if(ok)	{	var->Swap(s.toAscii());		refresh();	}
}
//-----------------------------------------------------------------------------
void DatPanel::mirror()
{
	bool ok;
	QString s = QInputDialog::getText(this, tr("UDAV - Mirror data"), tr("Enter direction(s) for mirroring.\nFor example 'xy' - mirror along x and y directions."), QLineEdit::Normal, "", &ok);
	if(ok)	{	var->Swap(s.toAscii());	refresh();	}
}
//-----------------------------------------------------------------------------
void DatPanel::sumof()
{
	QString name, val;
	if(namesDialog(tr("UDAV - Sum along ..."), tr("Specify direction(s) of summation"), name, val))
	{
		mglVar *v = parser.AddVar(name.toAscii());
		v->Set(var->Sum(val.toAscii()));
		updateDataItems();
	}
}
//-----------------------------------------------------------------------------
void DatPanel::maxof()
{
	QString name, val;
	if(namesDialog(tr("UDAV - Max along ..."), tr("Specify direction(s) of maximal values"), name, val))
	{
		mglVar *v = parser.AddVar(name.toAscii());
		v->Set(var->Max(val.toAscii()));
		updateDataItems();
	}
}
//-----------------------------------------------------------------------------
void DatPanel::minof()
{
	QString name, val;
	if(namesDialog(tr("UDAV - Min along ..."), tr("Specify direction(s) of minimal values"), name, val))
	{
		mglVar *v = parser.AddVar(name.toAscii());
		v->Set(var->Min(val.toAscii()));
		updateDataItems();
	}
}
//-----------------------------------------------------------------------------
void DatPanel::momentx()
{
	QString name, val;
	if(namesDialog(tr("UDAV - Momentum along 'x'"),
		tr("Specify which momentum evaluate.\nThe momentum is res_i = sum_jk how(x_i,y_j,z_k) a_jk/ sum_jk a_jk.\nCoordinates x, y, z are data indexes normalized in range [0,1]."), name, val))
	{
		mglVar *v = parser.AddVar(name.toAscii());
		v->Set(var->Momentum('x', val.toAscii()));
		updateDataItems();
	}
}
//-----------------------------------------------------------------------------
void DatPanel::momenty()
{
	QString name, val;
	if(namesDialog(tr("UDAV - Momentum along 'y'"),
		tr("Specify which momentum evaluate.\nThe momentum is res_j = sum_ik how(x_i,y_j,z_k) a_ik/ sum_ik a_ik.\nCoordinates x, y, z are data indexes normalized in range [0,1]."), name, val))
	{
		mglVar *v = parser.AddVar(name.toAscii());
		v->Set(var->Momentum('y', val.toAscii()));
		updateDataItems();
	}
}
//-----------------------------------------------------------------------------
void DatPanel::momentz()
{
	QString name, val;
	if(namesDialog(tr("UDAV - Momentum along 'z'"),
		tr("Specify which momentum evaluate.\nThe momentum is res_k = sum_ij how(x_i,y_j,z_k) a_ij/ sum_ij a_ij.\nCoordinates x, y, z are data indexes normalized in range [0,1]."), name, val))
	{
		mglVar *v = parser.AddVar(name.toAscii());
		v->Set(var->Momentum('z', val.toAscii()));
		updateDataItems();
	}
}
//-----------------------------------------------------------------------------
void DatPanel::hist()
{
	QLabel *l;
	QLineEdit *id, *v1, *v2;
	QSpinBox *nm;
	QPushButton *b;
	QDialog *d = new QDialog(this);	d->setWindowTitle(tr("UDAV - Make histogram"));
	QGridLayout *g = new QGridLayout(d);
	l = new QLabel(tr("From"), d);	g->addWidget(l,0,0);
	l = new QLabel(tr("To"), d);	g->addWidget(l,0,1);
	v1 = new QLineEdit(d);	g->addWidget(v1,1,0);
	v2 = new QLineEdit(d);	g->addWidget(v2,1,1);
	l = new QLabel(tr("Number of points"), d);	g->addWidget(l,2,0);
	l = new QLabel(tr("Put in variable"), d);	g->addWidget(l,2,1);
	nm = new QSpinBox(d);	nm->setRange(2,8192);	g->addWidget(nm,3,0);
	id = new QLineEdit(d);	nm->setSingleStep(10);	g->addWidget(id,3,1);
	b = new QPushButton(tr("Cancel"), d);	g->addWidget(b,4,0);
	connect(b, SIGNAL(clicked()), d, SLOT(reject()));
	b = new QPushButton(tr("OK"), d);		g->addWidget(b,4,1);
	connect(b, SIGNAL(clicked()), d, SLOT(accept()));	b->setDefault(true);
	// now execute dialog and get values
	bool res = d->exec();
	if(res && !v1->text().isEmpty() && !v2->text().isEmpty() && !id->text().isEmpty())
	{
		mglVar *vv = parser.AddVar(id->text().toAscii());
		if(!vv)	return;
		vv->Set(var->Hist(nm->value(), v1->text().toDouble(), v2->text().toDouble()));
		updateDataItems();
	}
}
//-----------------------------------------------------------------------------
void DatPanel::addto()
{
	bool ok;
	QString s = QInputDialog::getText(this, tr("UDAV - Change data"), tr("Enter number for adding to data elements:"), QLineEdit::Normal, "", &ok);
	if(ok)	{	*var += s.toDouble();		refresh();	}
}
//-----------------------------------------------------------------------------
void DatPanel::subto()
{
	bool ok;
	QString s = QInputDialog::getText(this, tr("UDAV - Change data"), tr("Enter number for subtraction from data elements:"), QLineEdit::Normal, "", &ok);
	if(ok)	{	*var -= s.toDouble();		refresh();	}
}
//-----------------------------------------------------------------------------
void DatPanel::divto()
{
	bool ok;
	QString s = QInputDialog::getText(this, tr("UDAV - Change data"), tr("Enter number for division of data elements:"), QLineEdit::Normal, "", &ok);
	if(ok)	{	*var /= s.toDouble();		refresh();	}
}
//-----------------------------------------------------------------------------
void DatPanel::multo()
{
	bool ok;
	QString s = QInputDialog::getText(this, tr("UDAV - Change data"), tr("Enter number for multiplication of data elements:"), QLineEdit::Normal, "", &ok);
	if(ok)	{	*var *= s.toDouble();		refresh();	}
}
//-----------------------------------------------------------------------------
void DatPanel::first()	{	setSlice(0);	}
//-----------------------------------------------------------------------------
void DatPanel::last()	{	setSlice(nz-1);	}
//-----------------------------------------------------------------------------
void DatPanel::next()	{	setSlice(kz+1);	}
//-----------------------------------------------------------------------------
void DatPanel::prev()	{	setSlice(kz-1);	}
//-----------------------------------------------------------------------------
void DatPanel::gosl()
{
	bool ok;
	QString s = QInputDialog::getText(this, tr("UDAV - Go to slice"), tr("Enter slice id:"), QLineEdit::Normal, "0", &ok);
	if(ok)	setSlice(s.toInt());
}
//-----------------------------------------------------------------------------
void DatPanel::setNz(int nz)	{	sb->setMaximum(nz-1);	}
//-----------------------------------------------------------------------------
bool DatPanel::sizesDialog(const QString &cap, const QString &lab, const QString &desc1, const QString &desc2, const QString &desc3, QString &val1, QString &val2, QString &val3)
{
	QLabel *l;
	QLineEdit *f1, *f2, *f3;
	QPushButton *b;
	QDialog *d = new QDialog(this);
	d->setWindowTitle(cap);
	QVBoxLayout *v = new QVBoxLayout(d);
	l = new QLabel(lab, d);	v->addWidget(l);
	l = new QLabel(tr("NOTE: All fields must be filled!"), d);	v->addWidget(l);
	QGridLayout *g = new QGridLayout();	v->addLayout(g);
	l = new QLabel(desc1, d);		g->addWidget(l, 0, 0);
	l = new QLabel(desc2, d);		g->addWidget(l, 0, 1);
	l = new QLabel(desc3, d);		g->addWidget(l, 0, 2);
	f1 = new QLineEdit(val1, d);	g->addWidget(f1, 1, 0);
	f2 = new QLineEdit(val2, d);	g->addWidget(f2, 1, 1);
	f3 = new QLineEdit(val3, d);	g->addWidget(f3, 1, 2);
	QHBoxLayout *h = new QHBoxLayout();	v->addLayout(h);
	h->addStretch(1);
	b = new QPushButton(tr("Cancel"), d);	h->addWidget(b);
	connect(b, SIGNAL(clicked()), d, SLOT(reject()));
	b = new QPushButton(tr("OK"), d);		h->addWidget(b);
	connect(b, SIGNAL(clicked()), d, SLOT(accept()));
	b->setDefault(true);
	// now execute dialog and get values
	bool res = d->exec();
	val1 = f1->text();	val2 = f2->text();	val3 = f3->text();
	if(val1.isEmpty() || val2.isEmpty() || val3.isEmpty())	res = false;
	delete d;
	return res;
}
//-----------------------------------------------------------------------------
bool DatPanel::namesDialog(const QString &cap, const QString &lab, QString &name, QString &val)
{
	QLabel *l;
	QLineEdit *f1, *f2;
	QPushButton *b;
	QDialog *d = new QDialog(this);
	d->setWindowTitle(cap);
	QVBoxLayout *v = new QVBoxLayout(d);
	l = new QLabel(lab, d);		v->addWidget(l);
	l = new QLabel(tr("NOTE: All fields must be filled!"), d);	v->addWidget(l);
	f1 = new QLineEdit(val, d);	v->addWidget(f1);
	l = new QLabel(tr("Enter the name for new variable"), d);	v->addWidget(l);
	f2 = new QLineEdit(d);		v->addWidget(f2);
	QHBoxLayout *h = new QHBoxLayout();	v->addLayout(h);	h->addStretch(1);
	b = new QPushButton(tr("Cancel"), d);	h->addWidget(b);
	connect(b, SIGNAL(clicked()), d, SLOT(reject()));
	b = new QPushButton(tr("OK"), d);		h->addWidget(b);
	connect(b, SIGNAL(clicked()), d, SLOT(accept()));
	b->setDefault(true);
	// now execute dialog and get values
	bool res = d->exec();
	val = f1->text();	name = f2->text();
	if(val.isEmpty() || name.isEmpty())	res = false;
	delete d;
	return res;
}
//-----------------------------------------------------------------------------
#include "xpm/plot.xpm"
#include "xpm/size.xpm"
#include "xpm/smth.xpm"
#include "xpm/oper_d.xpm"
#include "xpm/oper_s.xpm"
#include "xpm/oper_a.xpm"
#include "xpm/oper_m.xpm"
#include "xpm/crop.xpm"
#include "xpm/tran.xpm"
#include "xpm/integr.xpm"
#include "xpm/diff.xpm"
#include "xpm/diff2.xpm"
#include "xpm/squize.xpm"
#include "xpm/sum.xpm"
#include "xpm/func.xpm"
#include "xpm/swap.xpm"
//-----------------------------------------------------------------------------
void DatPanel::toolTop(QBoxLayout *l)
{
	QAction *a;
	QMenu *o;
	QToolButton *bb;

	// file menu
	o = menu->addMenu(tr("&File"));
	a = new QAction(QPixmap(":/xpm/document-open.png"), tr("&Load data"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(load()));
	a->setToolTip(tr("Load data from file. Data will be deleted only\nat exit but UDAV will not ask to save it."));
	a->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_O);	o->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	a = new QAction(QPixmap(":/xpm/document-import.png"), tr("&Import PNG"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(imprt()));
	a->setToolTip(tr("Import data from PNG picture with specified color scheme.\nData will be deleted only at exit but UDAV will not ask it saving."));
	o->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	a = new QAction(QPixmap(":/xpm/document-save.png"), tr("&Save data"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(save()));
	a->setToolTip(tr("Save data to tab-separeted file."));
	a->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_S);	o->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	a = new QAction(QPixmap(":/xpm/document-export.png"), tr("&Export PNG"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(exprt()));
	a->setToolTip(tr("Export data to PNG picture. The colors defined by \nspecified color scheme. The same as in 'dens' command."));
	o->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

//	o->addSeparator();	bb->addSeparator();
//	a = new QAction(QPixmap(insert_xpm), tr("Insert as &list"), this);
//	connect(a, SIGNAL(triggered()), this, SLOT(list()));
//	o->addAction(a);
//	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);


	a = new QAction(QPixmap(plot_xpm), tr("Plot &data"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(plot()));
	a->setToolTip(tr("Plot data in new script window. You may select the kind\nof plot, its style and so on."));
	o->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	a = new QAction(QPixmap(":/xpm/edit-copy.png"), tr("Copy &data"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(copy()));
	a->setToolTip(tr("Copy range of numbers to clipboard."));
	o->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	a = new QAction(QPixmap(":/xpm/edit-paste.png"), tr("Paste &data"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(copy()));
	a->setToolTip(tr("Paste range of numbers from clipboard."));
	o->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	// navigation menu
	o = menu->addMenu(tr("&Navigate"));
	a = new QAction(QPixmap(":/xpm/go-first.png"), tr("&First slice"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(first()));
	a->setToolTip(tr("Go to the first data slice for 3D data."));
	o->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	a = new QAction(QPixmap(":/xpm/go-previous.png"), tr("&Prev. slice"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(prev()));
	a->setToolTip(tr("Go to the previous data slice for 3D data."));
	o->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	sb = new QSpinBox(this);
	l->addWidget(sb);	sb->setRange(0,0);
	sb->setToolTip(tr("Go to the specified data slice for 3D data."));
	connect(sb, SIGNAL(valueChanged(int)), this, SLOT(setSlice(int)));
	connect(this, SIGNAL(sliceChanged(int)), sb, SLOT(setValue(int)));
	connect(this, SIGNAL(nzChanged(int)), this, SLOT(setNz(int)));

	a = new QAction(tr("Go to slice"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(gosl()));
	a->setToolTip(tr("Go to the specified data slice for 3D data."));
	o->addAction(a);

	a = new QAction(QPixmap(":/xpm/go-next.png"), tr("Next slice"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(next()));
	a->setToolTip(tr("Go to the next data slice for 3D data."));
	o->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	a = new QAction(QPixmap(":/xpm/go-last.png"), tr("Last slice"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(last()));
	a->setToolTip(tr("Go to the last data slice for 3D data."));
	o->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);
}
//-----------------------------------------------------------------------------
void DatPanel::toolLeft(QBoxLayout *l)
{
	QAction *a;
	QMenu *o, *oo;
	QToolButton *bb;

	// size menu
	o = menu->addMenu(tr("&Sizes"));
	a = new QAction(QPixmap(":/xpm/document-new.png"), tr("&Create new"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(create()));
	a->setToolTip(tr("Recreate the data with new sizes and fill it by zeros (Ctrl+N)."));
	a->setShortcut(Qt::CTRL+Qt::Key_N);	o->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	a = new QAction(QPixmap(size_xpm), tr("&Resize"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(reSize()));
	a->setToolTip(tr("Resize (interpolate) the data to specified sizes (Ctrl+R)."));
	a->setShortcut(Qt::CTRL+Qt::Key_R);	o->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	a = new QAction(QPixmap(squize_xpm), tr("&Squeeze"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(squize()));
	a->setToolTip(tr("Keep only each n-th element of the data array."));
	o->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	a = new QAction(QPixmap(crop_xpm), tr("Cro&p"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(crop()));
	a->setToolTip(tr("Crop the data edges. Useful to cut off the zero-filled area."));
	o->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	a = new QAction(QPixmap(tran_xpm), tr("&Transpose"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(transp()));
	a->setToolTip(tr("Transpose data dimensions, like x<->y or x<->z and so on."));
	o->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	a = new QAction(tr("Re&arrange"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(rearrange()));
	a->setToolTip(tr("Rearrange data sizes without changing data values."));
	o->addAction(a);

	// modify menu
	o = menu->addMenu(tr("&Modify"));
	a = new QAction(QPixmap(func_xpm), tr("By &formula"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(byformula()));
	a->setToolTip(tr("Change data values according to formula depended on 'x', 'y' and 'z'\nvariables. A set of special function is availible also."));
	a->setShortcut(Qt::CTRL+Qt::Key_M);	o->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	a = new QAction(tr("Fill in &range"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(inrange()));
	a->setToolTip(tr("Fill data equidistantly from one value to another."));
	o->addAction(a);
	a = new QAction(tr("&Normalize"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(norm()));
	a->setToolTip(tr("Normalize data so that its minimal\nand maximal values be in specified range."));
	o->addAction(a);
	a = new QAction(tr("Norm. s&lices"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(normsl()));
	a->setToolTip(tr("Normalize each data slice perpendicular to some direction\nso that its minimal and maximal values be in specified range."));
	o->addAction(a);
	a = new QAction(QPixmap(smth_xpm), tr("&Smooth data"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(smooth()));
	a->setToolTip(tr("Smooth data by one of 4 methods along specified direction(s)."));
	o->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	oo = menu->addMenu(tr("&Operators"));
	a = new QAction(QPixmap(sum_xpm), tr("&Cum. sum"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(cumsum()));
	a->setToolTip(tr("Summate data values along specified direction(s)."));
	oo->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	a = new QAction(QPixmap(integr_xpm), tr("&Integrate"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(integr()));
	a->setToolTip(tr("Integrate data values along specified direction(s)."));
	oo->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	a = new QAction(QPixmap(diff_xpm), tr("&Differentiate"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(diff()));
	a->setToolTip(tr("Differentiate data values along specified direction(s)."));
	oo->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	a = new QAction(QPixmap(diff2_xpm), tr("&Laplace"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(diff2()));
	a->setToolTip(tr("Double differentiate data values along specified direction(s)."));
	oo->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	a = new QAction(QPixmap(swap_xpm), tr("&Swap"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(swap()));
	a->setToolTip(tr("Swap left and right data part along specified direction(s).\nThis operation is useful for data after Fourier transform."));
	oo->addAction(a);
	bb = new QToolButton(this);	l->addWidget(bb);	bb->setDefaultAction(a);

	a = new QAction(tr("&Mirror"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(mirror()));
	a->setToolTip(tr("Mirror left and right data part along specified direction(s).\nThis operation do like index change from 'i' to 'n-i'."));
	oo->addAction(a);

	oo = menu->addMenu(tr("&Algebraic"));
	a = new QAction(QPixmap(oper_a_xpm), tr("&Add"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(addto()));
	a->setToolTip(tr("Add a number to all data values."));
	oo->addAction(a);
	a = new QAction(QPixmap(oper_s_xpm), tr("&Subtract"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(subto()));
	a->setToolTip(tr("Subtract a number to all data values."));
	oo->addAction(a);
	a = new QAction(QPixmap(oper_m_xpm), tr("&Multiply"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(multo()));
	a->setToolTip(tr("Multiply all data values by a number."));
	oo->addAction(a);
	a = new QAction(QPixmap(oper_d_xpm), tr("&Divide"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(divto()));
	a->setToolTip(tr("Divide all data values by a number."));
	oo->addAction(a);

	oo = menu->addMenu(tr("A&nother data"));
	a = new QAction(tr("&Sum of"), this);	oo->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(sumof()));
	a = new QAction(tr("M&in of"), this);	oo->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(minof()));
	a = new QAction(tr("M&ax of"), this);	oo->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(maxof()));
	a = new QAction(tr("Momentum along &x"), this);	oo->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(momentx()));
	a = new QAction(tr("Momentum along &y"), this);	oo->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(momenty()));
	a = new QAction(tr("Momentum along &z"), this);	oo->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(momentz()));
	a = new QAction(tr("&Histogram"), this);	oo->addAction(a);
	connect(a, SIGNAL(triggered()), this, SLOT(hist()));

	l->addStretch(1);
}
//-----------------------------------------------------------------------------
QString DatPanel::dataName()	{	return QString::fromStdWString(var->s);	}
//-----------------------------------------------------------------------------
