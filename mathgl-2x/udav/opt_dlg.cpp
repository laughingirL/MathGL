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
#include <QPushButton>
#include <QMessageBox>
#include "opt_dlg.h"
//-----------------------------------------------------------------------------
OptionDialog::OptionDialog(QWidget *parent) : QDialog(parent)
{
	setWindowTitle(tr("UDAV - Add options"));
	QHBoxLayout *a;
	QGridLayout *g;
	QLabel *lbl;
	QPushButton *b;
	QVBoxLayout *o = new QVBoxLayout(this);	//o->setSpacing(6);
	g = new QGridLayout();		o->addLayout(g);
	lbl = new QLabel(tr("X-range"),this);	g->addWidget(lbl,0,0);
	x1 = new QLineEdit(this);	g->addWidget(x1,0,1);
	x2 = new QLineEdit(this);	g->addWidget(x2,0,2);
	lbl = new QLabel(tr("Y-range"),this);	g->addWidget(lbl,0,3);
	y1 = new QLineEdit(this);	g->addWidget(y1,0,4);
	y2 = new QLineEdit(this);	g->addWidget(y2,0,5);
	lbl = new QLabel(tr("Z-range"),this);	g->addWidget(lbl,1,0);
	z1 = new QLineEdit(this);	g->addWidget(z1,1,1);
	z2 = new QLineEdit(this);	g->addWidget(z2,1,2);
	lbl = new QLabel(tr("C-range"),this);	g->addWidget(lbl,1,3);
	c1 = new QLineEdit(this);	g->addWidget(c1,1,4);
	c2 = new QLineEdit(this);	g->addWidget(c2,1,5);
	g = new QGridLayout();		o->addLayout(g);
	lbl = new QLabel(tr("Alpha"),this);		g->addWidget(lbl,0,0);
	alpha = new QLineEdit(this);	g->addWidget(alpha,1,0);
	lbl = new QLabel(tr("Ambient"),this);	g->addWidget(lbl,0,1);
	amb = new QLineEdit(this);		g->addWidget(amb,1,1);
	lbl = new QLabel(tr("Mesh num"),this);	g->addWidget(lbl,0,2);
	mn = new QLineEdit(this);		g->addWidget(mn,1,2);
	lbl = new QLabel(tr("Font size"),this);	g->addWidget(lbl,0,3);
	fs = new QLineEdit(this);		g->addWidget(fs,1,3);
	lbl = new QLabel(tr("Cutting"),this);	g->addWidget(lbl,0,4);
	cut = new QComboBox(this);		g->addWidget(cut,1,4);
	cut->insertItem(0,tr("on"));	cut->insertItem(1,tr("off"));
	a = new QHBoxLayout();	o->addLayout(a);
	lbl = new QLabel(tr("Legend"),this);	a->addWidget(lbl);
	leg = new QLineEdit(this);	a->addWidget(leg);
	a = new QHBoxLayout();	o->addLayout(a);
	a->addStretch(1);
	b = new QPushButton(tr("Cancel"), this);	a->addWidget(b);
	connect(b, SIGNAL(clicked()),this, SLOT(reject()));
	b = new QPushButton(tr("OK"), this);		a->addWidget(b);
	connect(b, SIGNAL(clicked()),this, SLOT(prepareResult()));
	b->setDefault(true);
}
//-----------------------------------------------------------------------------
OptionDialog::~OptionDialog()	{}
//-----------------------------------------------------------------------------
void OptionDialog::prepareResult()
{
	result = "";
	if(!x1->text().isEmpty() && !x2->text().isEmpty())
		result = result + "; xrange "+x1->text()+" "+x2->text();
	if((x1->text().isEmpty()) ^ (x2->text().isEmpty()))
	{
		QMessageBox::warning(this,tr("UDAV - command options"), tr("Both fields in xrange must be filled"));
		return;
	}
	if(!y1->text().isEmpty() && !y2->text().isEmpty())
		result = result + "; yrange "+y1->text()+" "+y2->text();
	if((y1->text().isEmpty()) ^ (y2->text().isEmpty()))
	{
		QMessageBox::warning(this,tr("UDAV - command options"), tr("Both fields in yrange must be filled"));
		return;
	}
	if(!z1->text().isEmpty() && !z2->text().isEmpty())
		result = result + "; zrange "+z1->text()+" "+z2->text();
	if((z1->text().isEmpty()) ^ (z2->text().isEmpty()))
	{
		QMessageBox::warning(this,tr("UDAV - command options"), tr("Both fields in zrange must be filled"));
		return;
	}
	if(!c1->text().isEmpty() && !c2->text().isEmpty())
		result = result + "; crange "+c1->text()+" "+c2->text();
	if((c1->text().isEmpty()) ^ (c2->text().isEmpty()))
	{
		QMessageBox::warning(this,tr("UDAV - command options"), tr("Both fields in crange must be filled"));
		return;
	}
	if(!alpha->text().isEmpty())	result = result+"; alpha "+alpha->text();
	if(!amb->text().isEmpty())	result = result+"; ambient "+amb->text();
	if(!mn->text().isEmpty())	result = result+"; meshnum "+mn->text();
	if(!fs->text().isEmpty())	result = result+"; fontsize "+fs->text();
	if(cut->currentIndex()==0)	result = result+"; cut on";
	if(cut->currentIndex()==1)	result = result+"; cut off";
	if(!leg->text().isEmpty())	result = result+"; legend '"+leg->text()+"'";
	accept();
}
//-----------------------------------------------------------------------------
