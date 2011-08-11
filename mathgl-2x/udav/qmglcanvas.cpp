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

#include <QApplication>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMenu>
#include <QClipboard>
#include <QTextEdit>
#include <QPainter>
#include <QCursor>
#include <QImage>
#include <QFileInfo>
#include <QProcess>
#include <QStringList>
#include <stdio.h>
#include <mgl/parser.h>
//#include <mgl/mgl_idtf.h> //TODO: Add later
#include "qmglcanvas.h"
extern mglParse parser;
extern bool mglAutoPure;
//-----------------------------------------------------------------------------
QMGLCanvas::QMGLCanvas(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f)
{
	autoResize = false;
	textMGL = warnMGL = 0;	appName = tr("UDAV");
	popup = 0;	grBuf = 0;	phi = tet = per = 0;
	alpha = light = rotate = grid = false;
	graph = new mglGraph;
	resize(graph->GetWidth(), graph->GetHeight());
	convertFromGraph(pic, graph, &grBuf);
	thr = new MGLThread();	thr->cnv = this;
}
//-----------------------------------------------------------------------------
QMGLCanvas::~QMGLCanvas()
{	delete thr;	delete graph;	if(grBuf)	delete []grBuf;	}
//-----------------------------------------------------------------------------
void QMGLCanvas::paintEvent(QPaintEvent *)
{
	QPainter paint;
	paint.begin(this);
	paint.drawPixmap(0,0,pic);
	if(grid)
	{
		int i, h=pic.height(), w=pic.width();
		paint.setPen(QColor(192,192,192));
		for(i=1;i<10;i++)
		{
			paint.drawText(0,i*h/10,QString::number(1-i*0.1));
			paint.drawLine(0,i*h/10,w,i*h/10);
			paint.drawText(i*w/10,h,QString::number(i*0.1));
			paint.drawLine(i*w/10,0,i*w/10,h);
		}
	}
	paint.end();
}
//-----------------------------------------------------------------------------
void QMGLCanvas::resizeEvent(QResizeEvent *ev)
{
	if(autoResize && ev->size().width()>0 && ev->size().height()>0)
	{	graph->SetSize(ev->size().width(), ev->size().height());	execute();	repaint();	}
	else	resize(graph->GetWidth(), graph->GetHeight());
}
//-----------------------------------------------------------------------------
void QMGLCanvas::setPer(int p)
{
	if(per!=p && p>=0 && p<100)
	{	per = 100*p;	emit perChanged(p);	execute();	}
}
//-----------------------------------------------------------------------------
void QMGLCanvas::imgSize(int w, int h)
{	if(w<=0 || h<=0)	return;
	graph->SetSize(w,h);	execute();	}
//-----------------------------------------------------------------------------
void QMGLCanvas::setPhi(int p)
{	if(phi!=p)	{	phi = p;	emit phiChanged(p);	execute();	}	}
//-----------------------------------------------------------------------------
void QMGLCanvas::setTet(int t)
{	if(tet!=t)	{	tet = t;	emit tetChanged(t);	execute();	}	}
//-----------------------------------------------------------------------------
void QMGLCanvas::setGrid(bool a)
{	if(grid!=a)		{	grid = a;	emit gridChanged(a);	execute();	}	}
//-----------------------------------------------------------------------------
void QMGLCanvas::setAlpha(bool a)
{	if(alpha!=a)	{	alpha = a;	emit alphaChanged(a);	execute();	}	}
//-----------------------------------------------------------------------------
void QMGLCanvas::setLight(bool l)
{	if(light!=l)	{	light = l;	emit lightChanged(l);	execute();	}	}
//-----------------------------------------------------------------------------
void QMGLCanvas::setRotate(bool r)
{
	if(rotate!=r)
	{
		rotate=r;
		emit rotateChanged(r);
		execute();
	}
}
//-----------------------------------------------------------------------------
void QMGLCanvas::restore()
{
	setPhi(0);	setTet(0);	setPer(0);	rotate=false;
	emit rotateChanged(false);
	execute();
}
//-----------------------------------------------------------------------------
void QMGLCanvas::execute(mglGraph *gr, int line)
{
	if(!textMGL && autoResize)	{	run(defText, false, gr);	return;	}
	else if(!textMGL)	return;
	run(textMGL->toPlainText(), true, gr, line);
	if(pic.size()!=size())	setSize(pic.width(), pic.height());
	emit refreshData();
}
//-----------------------------------------------------------------------------
QString QMGLCanvas::run(const QString &text, bool showm, mglGraph *gr, int line)
{
	defText = text;
	if(gr==0)	gr=graph;
	if(gr==0)	return QString(tr("Not initialized!"));
	gr->DefaultPlotParam();
	gr->Alpha(alpha);	gr->Light(light);
	gr->View(tet,phi);	gr->SetOrigin(NAN,NAN,NAN);
	gr->Perspective(per);
//	gr->DrawFace = !(rotate && mglAutoPure);
	if(text.count("rotate ")==0 && text.count("rotate\t")==0)	gr->Rotate(0,0);
	parser.Stop = false;
	parser.AllowSetSize = true;
	showMessage = showm;

	if(warnMGL && !warnMGL->toPlainText().isEmpty())	warnMGL->clear();
	// At least now I don't want use setup script ! So I skip this code
	if(!isHidden())	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	if(!thr->isRunning())
	{
		thr->gr = gr;	thr->text = text;
		thr->line = line;	thr->start();
	}
	if(rotate)	thr->wait();
	else	while(thr->isRunning())	qApp->processEvents();
	if(warnMGL)	warnMGL->setText(thr->warn);
	if(!isHidden())	QApplication::restoreOverrideCursor();
	if(gr==graph)
	{	convertFromGraph(pic, graph, &grBuf);	repaint();	}
	return thr->mes;
}
//-----------------------------------------------------------------------------
void MGLThread::run()
{
	int i,r,n=text.count('\n')+1;
	char *buf=new char[2048];
	gr->Message(buf);	warn = "";
	wchar_t *str = new wchar_t[text.length()+2];
	QString cur;
	bool high;
	// Start parsing the text (just slightly modified copy from mgl_parse.cpp)
	parser.ScanFunc(0);		for(i=0;i<n;i++)
	{
		cur = text.section('\n',i,i);	high = false;
		r = cur.toWCharArray(str);	str[r] = 0;
		parser.ScanFunc(str);
	}
	for(i=0;i<n && !parser.Stop;i++)
	{
		cur = text.section('\n',i,i);	high = false;

		if(i==line)
		{
			wchar_t *s = new wchar_t[cur.length()+1];
			register int j;
			for(j=0;j<cur.length() && !cur[j].isSpace() && cur[j]!=':';j++)
				s[j] = text[j].toLatin1();
			s[j]=0;
			mglCommand *rts = parser.FindCommand(s);
			if(rts && (rts->type==0 || rts->type==1))	high = true;
			delete []s;
		}

		r = cur.toWCharArray(str);	str[r] = 0;
		buf[0] = 0;			gr->SetObjId(i+1);
		setlocale(LC_ALL, "C");
		r = parser.Parse(gr,str,i+1);
		setlocale(LC_ALL, "");
		if(r<0)	{	i = -r-2;	continue;	}
		if(r>0)
		{
			mes = tr("In line ")+QString::number(i);
			if(r==1)	mes += tr(" -- wrong argument(s):\t");
			if(r==2)	mes += tr(" -- wrong command:\t");
			if(r==3)	mes += tr(" -- string too long:\t");
			if(r==4)	mes += tr(" -- unbalanced ' :\t");
			mes += cur;
			warn += mes + "\n";
		}
		if(buf && *buf)
		{
			mes = tr("In line ")+QString::number(i)+" :\t";
			mes = mes + cur + "\n" + QString(buf) + "\n";
			warn += mes + "\n";
		}
	}
	mes = QString(buf);
	gr->Message(0);	delete []buf;	delete []str;
}
//-----------------------------------------------------------------------------
void QMGLCanvas::mouseDoubleClickEvent(QMouseEvent *ev)
{
	int oi = graph->GetObjId(ev->x(),ev->y())-1;
	emit objChanged(oi);
}
//-----------------------------------------------------------------------------
void QMGLCanvas::mousePressEvent(QMouseEvent *ev)
{
	if(!rotate && ev->button()&Qt::LeftButton)
	{
		mglPoint p = graph->CalcXYZ(ev->x(), ev->y());
		clickPos.sprintf("x=%g, y=%g, z=%g  --  Xabs=%.3g, Yabs=%.3g",
					p.x,p.y,p.z,double(ev->x())/graph->GetWidth(),
					1-double(ev->y())/graph->GetHeight());
		emit posChanged(clickPos);
	}
	xe=x0=ev->x();	ye=y0=ev->y();
	ev->accept();
}
//-----------------------------------------------------------------------------
void QMGLCanvas::mouseReleaseEvent(QMouseEvent *ev)
{
	if(ev->button()&Qt::RightButton && popup && !rotate)	// popup menu
		popup->popup(QCursor::pos());
	ev->accept();
}
//-----------------------------------------------------------------------------
void QMGLCanvas::mouseMoveEvent(QMouseEvent *ev)
{
	xe=ev->x();	ye=ev->y();
	if(rotate)
	{
		if(ev->buttons()&Qt::LeftButton)	// rotate
		{
			float ff = 240/sqrt(float(width()*height()));
			phi += int((x0-xe)*ff);
			tet += int((y0-ye)*ff);
			if(phi>180)	phi-=360;		if(phi<-180)	phi+=360;
			if(tet>180)	tet-=360;		if(tet<-180)	tet+=360;
			emit tetChanged(int(tet));		emit phiChanged(int(phi));
			execute();
		}
/*		if(ev->buttons()&Qt::RightButton)	// zoom and perspective
		{
			float ff = 2.*(y0-ye)/width(), gg = 0.5*(xe-x0)/height();
			float cx = (x1+x2)/2, cy = (y1+y2)/2;
			x1 = cx+(x1-cx)*exp(-ff);	x2 = cx+(x2-cx)*exp(-ff);
			y1 = cy+(y1-cy)*exp(-ff);	y2 = cy+(y2-cy)*exp(-ff);
			per = per + gg;
			if(per<0)	per = 0;	if(per>=1)	per = 0.9999;
			if(gg)	emit perChanged(int(per));
			execute();
		}
		if(ev->buttons()&Qt::MidButton)	// shift
		{
			float ff = 1./sqrt(float(width()*height()));
			float dx = (x0-xe)*ff*(x2-x1), dy = (y0-ye)*ff*(y2-y1);
			x1 += dx;	x2 += dx;	y1 -= dy;	y2 -= dy;
		}*/
		x0 = xe;	y0 = ye;
//		execute();
	}
	ev->accept();
}
//-----------------------------------------------------------------------------
QString setExtension(QString &fname, const char *ext)
{
	QString oname;
	if(fname.right(4)==".mgl")
		oname = fname.left(fname.length()-3)+ext;
	else
		oname = fname+"."+QString(ext);
	return oname;
}
//-----------------------------------------------------------------------------
void QMGLCanvas::startGIF(int animDelay, QString fname)
{	if(fname.isEmpty())	fname = scriptName;
	if(fname.isEmpty())	QMessageBox::critical(this, appName, tr("No filename. Please save script first."),QMessageBox::Ok,0,0);
	else
	{
		graph->StartGIF(setExtension(fname,"gif").toAscii(), animDelay);
		graph->ResetFrames();
	}
}
//-----------------------------------------------------------------------------
void QMGLCanvas::closeGIF()		{	graph->CloseGIF();	}
//-----------------------------------------------------------------------------
void QMGLCanvas::exportGIF(QString fname)
{	if(fname.isEmpty())	fname = scriptName;
	if(fname.isEmpty())	QMessageBox::critical(this, appName, tr("No filename. Please save script first."),QMessageBox::Ok,0,0);
	else	graph->WriteGIF(setExtension(fname,"gif").toAscii(), appName.toAscii());
}
//-----------------------------------------------------------------------------
void QMGLCanvas::exportPNG(QString fname)
{	if(fname.isEmpty())	fname = scriptName;
	if(fname.isEmpty())	QMessageBox::critical(this, appName, tr("No filename. Please save script first."),QMessageBox::Ok,0,0);
	else	graph->WritePNG(setExtension(fname,"png").toAscii(), appName.toAscii());
}
//-----------------------------------------------------------------------------
void QMGLCanvas::exportPNGs(QString fname)
{	if(fname.isEmpty())	fname = scriptName;
	if(fname.isEmpty())	QMessageBox::critical(this, appName, tr("No filename. Please save script first."),QMessageBox::Ok,0,0);
	else	graph->WritePNG(setExtension(fname,"png").toAscii(), appName.toAscii(), false);
}
//-----------------------------------------------------------------------------
void QMGLCanvas::exportJPG(QString fname)
{	if(fname.isEmpty())	fname = scriptName;
	if(fname.isEmpty())	QMessageBox::critical(this, appName, tr("No filename. Please save script first."),QMessageBox::Ok,0,0);
	else	graph->WriteJPEG(setExtension(fname,"jpg").toAscii(), appName.toAscii());
}
//-----------------------------------------------------------------------------
void QMGLCanvas::exportBPS(QString fname)
{
	if(fname.isEmpty())	fname = scriptName;
	if(fname.isEmpty())	QMessageBox::critical(this, appName, tr("No filename. Please save script first."),QMessageBox::Ok,0,0);
	else
	{
		setlocale(LC_ALL, "C");
				graph->WriteBPS(setExtension(fname,"eps.gz").toAscii(), appName.toAscii());
		setlocale(LC_ALL, "");
	}
}
//-----------------------------------------------------------------------------
void QMGLCanvas::exportEPS(QString fname)
{
	if(fname.isEmpty())	fname = scriptName;
	if(fname.isEmpty())	QMessageBox::critical(this, appName, tr("No filename. Please save script first."),QMessageBox::Ok,0,0);
	else
		{
				setlocale(LC_ALL, "C");
				graph->WriteEPS(setExtension(fname,"eps.gz").toAscii(), appName.toAscii());
				setlocale(LC_ALL, "");
		}
}
//-----------------------------------------------------------------------------
void QMGLCanvas::exportSVG(QString fname)
{
	if(fname.isEmpty())	fname = scriptName;
	if(fname.isEmpty())	QMessageBox::critical(this, appName, tr("No filename. Please save script first."),QMessageBox::Ok,0,0);
	else
		{
				setlocale(LC_ALL, "C");
				graph->WriteSVG(setExtension(fname,"svgz").toAscii(), appName.toAscii());
				setlocale(LC_ALL, "");
		}
}
//-----------------------------------------------------------------------------
void QMGLCanvas::exportIDTF(QString fname)
{
	if(fname.isEmpty())	fname = scriptName;
	if(fname.isEmpty())	QMessageBox::critical(this, appName, tr("No filename. Please save script first."),QMessageBox::Ok,0,0);
	else
	{
/*		mglGraphIDTF *ps = new mglGraphIDTF;
		ps->SetFont(graph->GetFont());
		execute(ps);
		setlocale(LC_ALL, "C");
		ps->WriteIDTF(setExtension(fname,"idtf").toAscii(), appName.toAscii());
		setlocale(LC_ALL, "");
				delete ps;*/
	}
}
//-----------------------------------------------------------------------------
void QMGLCanvas::exportCPP(QString fname)
{
	if(fname.isEmpty())	fname = scriptName;
	if(fname.isEmpty())	QMessageBox::critical(this, appName, tr("No filename. Please save script first."),QMessageBox::Ok,0,0);
	else
	{
/*		wchar_t out[1024];
		FILE *fp = fopen(setExtension(fname,"cpp").toAscii(),"wt");
		mglGraphPS *ps = new mglGraphPS(graph->GetWidth(), graph->GetHeight());
		int i, r, n = textMGL->toPlainText().count('\n')+1;
		wchar_t *str = new wchar_t[textMGL->toPlainText().length()+2];
		QString cur;
		fwprintf(fp,L"int draw_func(mglGraph *gr, void *)\n{\n");
		fwprintf(fp,L"\tstatic bool once = false;\n");
		for(i=0;i<n;i++)
		{
			cur = textMGL->toPlainText().section('\n',i,i);
			r = cur.toWCharArray(str);	str[r] = 0;
			out[0] = 0;		r = 0;
			setlocale(LC_ALL, "C");
			parser.Export(out,ps,str);
			setlocale(LC_ALL, "");
			fwprintf(fp,L"\t//%ls\n",str);
			if(*out)
			{
				if(*(parser.op1))	fwprintf(fp,L"%ls\n",parser.op1);
				fwprintf(fp,L"\t%ls\n",out);
				if(*(parser.op2))	fwprintf(fp,L"%ls\n",parser.op2);
			}
		}
		fwprintf(fp,L"\treturn 0;\n}\n");
		fclose(fp);
		delete []str;
				delete ps;*/
	}
}
//-----------------------------------------------------------------------------
void QMGLCanvas::reload()	{	parser.RestoreOnce();	execute();	}
//-----------------------------------------------------------------------------
//void QMGLCanvas::stop()		{	parser.Stop = true;	}
void QMGLCanvas::stop()		{	thr->terminate();	}
//-----------------------------------------------------------------------------
void convertFromGraph(QPixmap &pic, mglGraph *gr, uchar **buf)
{
	register long w=gr->GetWidth(), h=gr->GetHeight();
	if(*buf)	delete [](*buf);
	*buf = new uchar[4*w*h];
	gr->GetBGRN(*buf,4*w*h);
	QImage img(*buf, w, h, QImage::Format_RGB32);
	pic = QPixmap::fromImage(img);
}
//-----------------------------------------------------------------------------
void QMGLCanvas::copy()
{	QApplication::clipboard()->setPixmap(pic, QClipboard::Clipboard);	}
//-----------------------------------------------------------------------------
void QMGLCanvas::copyClickCoor()
{	QApplication::clipboard()->setText(clickPos);	}
//-----------------------------------------------------------------------------
void QMGLCanvas::setMGLFont(QString path)
{
	if(path.isEmpty())	graph->RestoreFont();
	else
	{
		QFileInfo fi(path);
		graph->LoadFont(fi.baseName().toAscii(), fi.path().toAscii());
	}
	execute();
}
//-----------------------------------------------------------------------------
void QMGLCanvas::setSize(int w, int h)
{	if(w<=0 || h<=0)	return;
	graph->SetSize(w,h);	resize(w, h);	execute();	}
//-----------------------------------------------------------------------------
