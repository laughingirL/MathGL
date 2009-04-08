/***************************************************************************
 * mgl_qt.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
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
#include <QTimer>
#include <QApplication>
#include <QMouseEvent>
#include <qmessagebox.h>
#include <qmenu.h>
#include <qclipboard.h>
#include <qtextedit.h>
#include <qpainter.h>
#include <qcursor.h>
#include <qimage.h>
#include <QScrollArea>
#include <QMainWindow>
#include <QToolBar>
#include <QMenuBar>
#include <QSpinBox>
#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>
#include <stdio.h>
#include "mgl/mgl_idtf.h"
#include "mgl/mgl_eps.h"
#include "mgl/mgl_qt.h"
#include "mgl/mgl_c.h"
#include "mgl/mgl_f.h"
//-----------------------------------------------------------------------------
#include "xpm/fileprint.xpm"
#include "xpm/copy.xpm"
#include "xpm/left_1.xpm"
#include "xpm/right_1.xpm"
#include "xpm/down_1.xpm"
#include "xpm/norm_1.xpm"
#include "xpm/zoom_1.xpm"
#include "xpm/up_1.xpm"
#include "xpm/alpha.xpm"
#include "xpm/light.xpm"
#include "xpm/zoom_in.xpm"
#include "xpm/zoom_out.xpm"
#include "xpm/rotate.xpm"
#include "xpm/ok.xpm"
#include "xpm/show_sl.xpm"
#include "xpm/next_sl.xpm"
#include "xpm/prev_sl.xpm"
//-----------------------------------------------------------------------------
const QString scriptName("default");
//-----------------------------------------------------------------------------
//
//		class QMathGL
//
//-----------------------------------------------------------------------------
QMathGL::QMathGL(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f)
{
	autoResize = false;	draw_par = 0;	draw_func = 0;
	graph = new mglGraphZB;
	popup = 0;		grBuf = 0;
	phi = tet = per = 0;
	x1 = y1 = 0;	x2 = y2 = 1;
	alpha = light = zoom = rotate = false;
	resize(600, 400);
//	resize(graph->GetWidth(), graph->GetHeight());
//	convertFromGraph(pic, graph, &grBuf);

	timer = new QTimer(this);	animDelay = 1000;
	connect(timer, SIGNAL(timeout()), this, SLOT(nextSlide()));
}
//-----------------------------------------------------------------------------
QMathGL::~QMathGL()	{	delete graph;	if(grBuf)	delete []grBuf;	}
//-----------------------------------------------------------------------------
void QMathGL::setGraph(mglGraphAB *gr)
{
	if(!gr)	return;
	delete graph;	graph=gr;
}
//-----------------------------------------------------------------------------
void QMathGL::setDraw(int (*func)(mglGraph *gr, void *par), void *par)
{	draw_func = func;	draw_par = par;	}
//-----------------------------------------------------------------------------
int mgl_draw_class(mglGraph *gr, void *p);
void QMathGL::setDraw(mglDraw *dr)
{	draw_func = mgl_draw_class;	draw_par = dr;	}
//-----------------------------------------------------------------------------
void QMathGL::paintEvent(QPaintEvent *)
{
	QPainter paint;
	paint.begin(this);
	paint.drawPixmap(0,0,pic);
	if(zoom)	paint.drawRect(x0,y0,xe-x0,ye-y0);
	if(graph->ShowMousePos && !mousePos.isEmpty())
		paint.drawText(0,12,mousePos);
	paint.end();
}
//-----------------------------------------------------------------------------
void QMathGL::resizeEvent(QResizeEvent *ev)
{
	if(autoResize)
	{	graph->SetSize(ev->size().width(), ev->size().height());	update();	}
	else	resize(graph->GetWidth(), graph->GetHeight());
}
//-----------------------------------------------------------------------------
void QMathGL::setPer(int p)
{
	if(per!=p && p>=0 && p<100)
	{	per = 100*p;	emit perChanged(p);	update();	}
}
//-----------------------------------------------------------------------------
void QMathGL::setPhi(int p)
{	if(phi!=p)	{	phi = p;	emit phiChanged(p);	update();	}	}
//-----------------------------------------------------------------------------
void QMathGL::setTet(int t)
{	if(tet!=t)	{	tet = t;	emit tetChanged(t);	update();	}	}
//-----------------------------------------------------------------------------
void QMathGL::setAlpha(bool a)
{	if(alpha!=a)	{	alpha = a;	emit alphaChanged(a);	update();	}	}
//-----------------------------------------------------------------------------
void QMathGL::setLight(bool l)
{	if(light!=l)	{	light = l;	emit lightChanged(l);	update();	}	}
//-----------------------------------------------------------------------------
void QMathGL::setZoom(bool z)
{
	if(zoom!=z)
	{
		zoom=z;	rotate=false;	update();
		emit zoomChanged(z);	emit rotateChanged(false);
	}
}
//-----------------------------------------------------------------------------
void QMathGL::setRotate(bool r)
{
	if(rotate!=r)
	{
		zoom=false;	rotate=r;	update();
		emit zoomChanged(false);	emit rotateChanged(r);
	}
}
//-----------------------------------------------------------------------------
void QMathGL::shiftDown()
{	mreal d=(y2-y1)/3;	y1+=d;	y2+=d;	update();	}
//-----------------------------------------------------------------------------
void QMathGL::shiftUp()
{	mreal d=(y2-y1)/3;	y1-=d;	y2-=d;	update();	}
//-----------------------------------------------------------------------------
void QMathGL::shiftRight()
{	mreal d=(x2-x1)/3;	x1-=d;	x2-=d;	update();	}
//-----------------------------------------------------------------------------
void QMathGL::shiftLeft()
{	mreal d=(x2-x1)/3;	x1+=d;	x2+=d;	update();	}
//-----------------------------------------------------------------------------
void QMathGL::restore()
{
	setPhi(0);	setTet(0);	setPer(0);
	x1=y1=0;	x2=y2=1;	zoom=rotate=false;
	emit zoomChanged(false);	emit rotateChanged(false);
	update();
}
//-----------------------------------------------------------------------------
void QMathGL::zoomIn()
{
	mreal d;
	d = (y2-y1)/4;	y1 += d;	y2 -= d;
	d = (x2-x1)/4;	x1 += d;	x2 -= d;
	update();
}
//-----------------------------------------------------------------------------
void QMathGL::zoomOut()
{
	mreal d;
	d = (y2-y1)/2;	y1 -= d;	y2 += d;
	d = (x2-x1)/2;	x1 -= d;	x2 += d;
	update();
}
//-----------------------------------------------------------------------------
void QMathGL::update(mglGraph *gr)
{
	if(gr==0)	gr = graph;
	if(gr==0)	return;
	gr->DefaultPlotParam();
	gr->Alpha(alpha);	gr->Light(light);
	gr->View(tet,phi);	gr->Org = mglPoint(NAN,NAN,NAN);
	gr->Perspective(per);
	gr->Zoom(x1,y1,x2,y2);
	gr->DrawFace = !rotate;

	char *buf=new char[2048];	buf[0]=0;	gr->Message = buf;
	if(draw_func)	draw_func(gr, draw_par);
	if(buf[0] != 0)	QMessageBox::warning(this, appName, buf);
	gr->Message = 0;		delete []buf;
	mousePos="";
	if(gr==graph)	refresh();
}
//-----------------------------------------------------------------------------
void QMathGL::refresh()
{
	convertFromGraph(pic, graph, &grBuf);
	if(pic.size()!=size())	setSize(pic.width(), pic.height());
	repaint();
}
//-----------------------------------------------------------------------------
void QMathGL::mousePressEvent(QMouseEvent *ev)
{
	if(!zoom && !rotate && ev->button()&Qt::LeftButton)
	{
		mglPoint p = graph->CalcXYZ(ev->x(), ev->y());
		mousePos.sprintf("x=%g, y=%g, z=%g",p.x,p.y,p.z);
		repaint();
		emit mouseClick(p.x,p.y,p.z);
	}
	xe=x0=ev->x();	ye=y0=ev->y();	ev->accept();
}
//-----------------------------------------------------------------------------
void QMathGL::mouseReleaseEvent(QMouseEvent *ev)
{
	if(ev->button()&Qt::LeftButton)
	{
		if(zoom)
		{
			int w1=width(),h1=height();
			mreal _x1,_x2,_y1,_y2;
			_x1 = x1+(x2-x1)*(x0-x())/mreal(w1);	_y1 = y2-(y2-y1)*(ye-y())/mreal(h1);
			_x2 = x1+(x2-x1)*(xe-x())/mreal(w1);	_y2 = y2-(y2-y1)*(y0-y())/mreal(h1);
			x1=_x1;		x2=_x2;		y1=_y1;		y2=_y2;
			if(x1>x2)	{	_x1=x1;	x1=x2;	x2=_x1;	}
			if(y1>y2)	{	_x1=y1;	y1=y2;	y2=_x1;	}
			x0 = xe;	y0 = ye;
			update();
		}
	}
	if(ev->button()&Qt::RightButton && popup && !rotate)	// popup menu
		popup->popup(QCursor::pos());
	ev->accept();
}
//-----------------------------------------------------------------------------
void QMathGL::mouseMoveEvent(QMouseEvent *ev)
{
	xe=ev->x();	ye=ev->y();
	if(rotate)
	{
		if(ev->buttons()&Qt::LeftButton)	// rotate
		{
			mreal ff = 240/sqrt(mreal(width()*height()));
			phi += int((x0-xe)*ff);
			tet += int((y0-ye)*ff);
			if(phi>180)	phi-=360;		if(phi<-180)	phi+=360;
			if(tet>180)	tet-=360;		if(tet<-180)	tet+=360;
			emit tetChanged(int(tet));		emit phiChanged(int(phi));
			update();
		}
		if(ev->buttons()&Qt::RightButton)	// zoom and perspective
		{
			mreal ff = 2.*(y0-ye)/width(), gg = 0.5*(xe-x0)/height();
			mreal cx = (x1+x2)/2, cy = (y1+y2)/2;
			x1 = cx+(x1-cx)*exp(-ff);	x2 = cx+(x2-cx)*exp(-ff);
			y1 = cy+(y1-cy)*exp(-ff);	y2 = cy+(y2-cy)*exp(-ff);
			per = per + gg;
			if(per<0)	per = 0;	if(per>=1)	per = 0.9999;
			if(gg)	emit perChanged(int(per));
			update();
		}
		if(ev->buttons()&Qt::MidButton)	// shift
		{
			mreal ff = 1./sqrt(mreal(width()*height()));
			mreal dx = (x0-xe)*ff*(x2-x1), dy = (y0-ye)*ff*(y2-y1);
			x1 += dx;	x2 += dx;	y1 -= dy;	y2 -= dy;
		}
		x0 = xe;	y0 = ye;
		update();
	}
	if(zoom)	update();
	ev->accept();
}
//-----------------------------------------------------------------------------
QString setExtension(QString &fname, const char *ext)
{
	QString oname;
	if(fname.right(4)!="."+QString(ext))
//		oname = fname.left(fname.length()-3)+ext;
//	else
		oname = fname+"."+QString(ext);
	return oname;
}
//-----------------------------------------------------------------------------
void QMathGL::exportPNG(QString fname)
{
	if(fname.isEmpty())	fname = scriptName;
	if(fname.isEmpty())	QMessageBox::critical(this, appName, tr("No filename."),QMessageBox::Ok,0,0);
	else	graph->WritePNG(setExtension(fname,"png").toAscii(), appName.toAscii());
}
//-----------------------------------------------------------------------------
void QMathGL::exportPNGs(QString fname)
{
	if(fname.isEmpty())	fname = scriptName;
	if(fname.isEmpty())	QMessageBox::critical(this, appName, tr("No filename."),QMessageBox::Ok,0,0);
	else	graph->WritePNG(setExtension(fname,"png").toAscii(), appName.toAscii(), false);
}
//-----------------------------------------------------------------------------
void QMathGL::exportJPG(QString fname)
{
	if(fname.isEmpty())	fname = scriptName;
	if(fname.isEmpty())	QMessageBox::critical(this, appName, tr("No filename."),QMessageBox::Ok,0,0);
	else	graph->WriteJPEG(setExtension(fname,"jpg").toAscii(), appName.toAscii());
}
//-----------------------------------------------------------------------------
void QMathGL::exportBPS(QString fname)
{
	if(fname.isEmpty())	fname = scriptName;
	if(fname.isEmpty())	QMessageBox::critical(this, appName, tr("No filename."),QMessageBox::Ok,0,0);
	else
	{
		setlocale(LC_ALL, "C");
		graph->WriteEPS(setExtension(fname,"eps").toAscii(), appName.toAscii());
		setlocale(LC_ALL, "");
	}
}
//-----------------------------------------------------------------------------
void QMathGL::exportEPS(QString fname)
{
	if(fname.isEmpty())	fname = scriptName;
	if(fname.isEmpty())	QMessageBox::critical(this, appName, tr("No filename."),QMessageBox::Ok,0,0);
	else
	{
		mglGraphPS *ps = new mglGraphPS(graph->GetWidth(), graph->GetHeight());
		ps->SetFont(graph->GetFont());
		update(ps);
		setlocale(LC_ALL, "C");
		ps->WriteEPS(setExtension(fname,"eps").toAscii(), appName.toAscii());
		setlocale(LC_ALL, "");
		delete ps;
	}
}
//-----------------------------------------------------------------------------
void QMathGL::exportSVG(QString fname)
{
	if(fname.isEmpty())	fname = scriptName;
	if(fname.isEmpty())	QMessageBox::critical(this, appName, tr("No filename."),QMessageBox::Ok,0,0);
	else
	{
		mglGraphPS *ps = new mglGraphPS(graph->GetWidth(), graph->GetHeight());
		ps->SetFont(graph->GetFont());
		update(ps);
		setlocale(LC_ALL, "C");
		ps->WriteSVG(setExtension(fname,"svg").toAscii(), appName.toAscii());
		setlocale(LC_ALL, "");
		delete ps;
	}
}
//-----------------------------------------------------------------------------
void QMathGL::exportIDTF(QString fname)
{
	if(fname.isEmpty())	fname = scriptName;
	if(fname.isEmpty())	QMessageBox::critical(this, appName, tr("No filename."),QMessageBox::Ok,0,0);
	else
	{
		mglGraphIDTF *ps = new mglGraphIDTF;
		ps->SetFont(graph->GetFont());
		update(ps);
		setlocale(LC_ALL, "C");
		ps->WriteIDTF(setExtension(fname,"idtf").toAscii(), appName.toAscii());
		setlocale(LC_ALL, "");
		delete ps;
	}
}
//-----------------------------------------------------------------------------
void convertFromGraph(QPixmap &pic, mglGraphAB *gr, uchar **buf)
{
	const uchar *bb = gr->GetBits();
	register long i,w=gr->GetWidth(), h=gr->GetHeight();
	if(*buf)	delete [](*buf);
	*buf = new uchar[4*w*h];
	for(i=0;i<w*h;i++)
	{
		(*buf)[4*i]   = bb[3*i+2];
		(*buf)[4*i+1] = bb[3*i+1];
		(*buf)[4*i+2] = bb[3*i];
		(*buf)[4*i+3] = 255;
	}
	QImage img(*buf, w, h, QImage::Format_RGB32);
	pic = QPixmap::fromImage(img);
}
//-----------------------------------------------------------------------------
void QMathGL::copy()
{
	QClipboard *cb = QApplication::clipboard();
	cb->setPixmap(pic, QClipboard::Clipboard);
}
//-----------------------------------------------------------------------------
void QMathGL::setMGLFont(QString path)
{
	if(path.isEmpty())	graph->GetFont()->Restore();
	else	graph->GetFont()->Load(path.toAscii());
}
//-----------------------------------------------------------------------------
void QMathGL::setSize(int w, int h)
{	graph->SetSize(w,h);	resize(w, h);	update();	};
//-----------------------------------------------------------------------------
void QMathGL::adjust()
{
	graph->Adjust();
}
//-----------------------------------------------------------------------------
void QMathGL::nextSlide()
{
	graph->NextFrame();
}
//-----------------------------------------------------------------------------
void QMathGL::prevSlide()
{
	graph->PrevFrame();
}
//-----------------------------------------------------------------------------
void QMathGL::animation(bool st)
{
	mglGraphQT *gr = dynamic_cast<mglGraphQT *>(graph);
	if(st)	timer->start(gr ? gr->Delay*1000 : animDelay);
	else	timer->stop();
}
//-----------------------------------------------------------------------------
void QMathGL::about()
{
	QString s = tr("MathGL v. 1.") + QString::number(MGL_VERSION) + tr("\n(c) Alexey Balakin, 2007\nhttp://mathgl.sourceforge.net/");
	QMessageBox::about(this, tr("MathGL - about"), s);
}
//-----------------------------------------------------------------------------
void QMathGL::aboutQt()	{	QMessageBox::aboutQt(this, tr("About Qt"));	}
//-----------------------------------------------------------------------------
void QMathGL::print()
{
	QPrinter *printer = new QPrinter;
	QPrintDialog printDlg(printer, this);
	if (printDlg.exec() == QDialog::Accepted)
	{
		QPainter p;
		if(!p.begin(printer))	return;	// paint on printer
		QRect r = p.viewport();
		int w = r.width(), h = r.height(), h1;
		h1 = int(w/getRatio());
		if(h1<h)	h = h1;	else	w = int(h*getRatio());
		mglGraphZB gr(w, h);
		if(w*h > 240000)	gr.BaseLineWidth = sqrt(w*h/2.4e5);
		update(&gr);

		uchar *grBuf=0;
		QPixmap pic;
		convertFromGraph(pic, &gr, &grBuf);
		p.drawPixmap(0,0,pic);
		delete []grBuf;
	}
	delete printer;
}
//-----------------------------------------------------------------------------
//
//		class mglGraphQT
//
//-----------------------------------------------------------------------------
mglGraphQT::mglGraphQT() : mglGraphZB()
{
	GG = 0;	Wnd = 0;	anim=0;
	NumFig = 0;	CurFig = -1;
}
//-----------------------------------------------------------------------------
mglGraphQT::~mglGraphQT()
{	if(GG) free(GG);	}
//-----------------------------------------------------------------------------
void mglGraphQT::SetSize(int w,int h)
{
	if(GG)	free(GG);	GG = 0;
	mglGraphZB::SetSize(w,h);
}
//-----------------------------------------------------------------------------
void mglGraphQT::EndFrame()
{
	CurFig = CurFrameId-1;
	if(!GG)
	{
		GG = (unsigned char *)malloc(3*Width*Height);
		NumFig = 1;		CurFig = 0;
	}
	else if(CurFig>NumFig-1)
	{
		GG = (unsigned char *)realloc(GG,3*(NumFig+1)*Width*Height);
		NumFig++;
	}
	mglGraph::EndFrame();
	memcpy(GG + CurFig*Width*Height*3,G,3*Width*Height);
	CurFig++;
}
//-----------------------------------------------------------------------------
void mglGraphQT::Clf(mglColor Back)
{
	if(AutoClf)	mglGraphZB::Clf(Back);
}
//-----------------------------------------------------------------------------
const unsigned char *mglGraphQT::GetBits()
{
	Finish();
	unsigned char *g = G;
	if(GG && NumFig>0 && CurFig<NumFig && CurFig>=0)
		g = GG + CurFig*Width*Height*3;
	return g;
}
//-----------------------------------------------------------------------------
void mglGraphQT::NextFrame()
{
	if(NumFig>0)	{	CurFig = CurFig>NumFig-1 ? 0 : CurFig+1;	QMGL->refresh();	}
}
//-----------------------------------------------------------------------------
void mglGraphQT::PrevFrame()
{
	if(NumFig>0)
	{	CurFig = CurFig<0 ? NumFig-1 : CurFig-1;	QMGL->refresh();	}
}
//-----------------------------------------------------------------------------
void mglGraphQT::Animation()
{
	if(anim)
	{
		bool s = anim->isChecked();
		anim->setChecked(!s);
		QMGL->animation(!s);
	}
}
//-----------------------------------------------------------------------------
void mglGraphQT::ToggleAlpha()	{	QMGL->setAlpha(!QMGL->getAlpha());	}
//-----------------------------------------------------------------------------
void mglGraphQT::ToggleLight()	{	QMGL->setLight(!QMGL->getLight());	}
//-----------------------------------------------------------------------------
void mglGraphQT::ToggleNo()		{	QMGL->restore();	}
//-----------------------------------------------------------------------------
void mglGraphQT::ToggleZoom()	{	QMGL->setZoom(!QMGL->getZoom());	}
//-----------------------------------------------------------------------------
void mglGraphQT::ToggleRotate()	{	QMGL->setRotate(!QMGL->getRotate());}
//-----------------------------------------------------------------------------
void mglGraphQT::Update()		{	CurFig=0;	QMGL->restore();	}
//-----------------------------------------------------------------------------
void mglGraphQT::ReLoad(bool o)	{	if(LoadFunc){LoadFunc(o, FuncPar);Update();}	}
//-----------------------------------------------------------------------------
void mglGraphQT::Adjust()
{
	SetSize(scroll->width(), scroll->height());
	QMGL->setSize(scroll->width(), scroll->height());
	Update();
}
//-----------------------------------------------------------------------------
void mglGraphQT::Window(int argc, char **argv, int (*draw)(mglGraph *gr, void *p), const char *title, void *par, void (*reload)(int next, void *p), bool maximize)
{
	NumFig=0;	CurFig=0;
	CurFrameId = 0;
	int n = draw(this,par);
	if(n<NumFig && n>=0)	NumFig = n;
	DrawFunc = draw;		FuncPar = par;
	LoadFunc = reload;
	if(Wnd)
	{
		Wnd->setWindowTitle(title);
		if(maximize)
		{	Wnd->showMaximized();	}
		else	Wnd->show();
		return;
	}

	if(!qApp && argc>0)
	{
		QApplication *a;
		if(!argv)
		{
			static char tmp[2][1];
			tmp[0][0]=tmp[1][0]=0;	argc=1;
			a = new QApplication(argc, (char **)tmp);
		}
		else
			a = new QApplication(argc, argv);
		a->connect(a, SIGNAL(lastWindowClosed()), a, SLOT(quit()));
	}

	Wnd = new QMainWindow;	Wnd->resize(650,480);
	Wnd->setWindowTitle(title);
	scroll = new QScrollArea(Wnd);
	QMGL = new QMathGL(Wnd);
	QMGL->setPopup(popup);	QMGL->setGraph(this);
	QMGL->setDraw(draw, par);
	QMGL->update();			makeMenu();
	scroll->setWidget(QMGL);
	Wnd->setCentralWidget(scroll);
	if(maximize)
	{	Wnd->showMaximized();	}
	else	Wnd->show();
}
//-----------------------------------------------------------------------------
HMGL mgl_create_graph_qt(HMDR dr, const char *title)
{
	mglGraphQT *g = new mglGraphQT;
	g->Window(0,0,dr,title);
	return g;
}
//-----------------------------------------------------------------------------
HMGL mgl_create_graph_qt(int (*draw)(HMGL gr, void *p), const char *title, void *par)
{
	mglGraphQT *g = new mglGraphQT;
	g->Window(0,0,draw,title,par);
	return g;
}
//-----------------------------------------------------------------------------
uintptr_t mgl_create_graph_qt_(int (*draw)(uintptr_t *gr), const char *title, int l)
{
	mglGraphQT *g = new mglGraphQT;
	char *s = new char[l+1];	memcpy(s,title,l);	s[l]=0;
	g->Window(0,0,mgl_fortran_func,s,(void*)draw);
	delete []s;
	return uintptr_t(g);
}
//-----------------------------------------------------------------------------
int mglQtRun()		{	return qApp ? qApp->exec():0;	}
//-----------------------------------------------------------------------------
void *mgl_qt_tmp(void *)	{	mglQtRun();	return 0;	}
/*void mgl_qt_thread()
{
	static pthread_t tmp;
	pthread_create(&tmp, 0, mgl_qt_tmp, 0);
	pthread_detach(tmp);
}*/
void mgl_qt_run()	{	mglQtRun();	}
//-----------------------------------------------------------------------------
void mgl_qt_run_()	{	mglQtRun();	}
//-----------------------------------------------------------------------------
//void mgl_qt_thread_()	{	mgl_qt_thread();	}
//-----------------------------------------------------------------------------
#define TR	QObject::tr
void mglGraphQT::makeMenu()
{
	QAction *a;
	QMenu *o, *oo;
	QToolBar *bb;

	popup = new QMenu(Wnd);
	// file menu
	{
		o = Wnd->menuBar()->addMenu(TR("&File"));
		oo = new QMenu(TR("&Export as ..."),Wnd);
		oo->addAction(TR("PNG"), QMGL, SLOT(exportPNG()),Qt::ALT+Qt::Key_P);
		oo->addAction(TR("solid PNG"), QMGL, SLOT(exportPNGs()),Qt::ALT+Qt::Key_F);
		oo->addAction(TR("JPEG"), QMGL, SLOT(exportJPG()),Qt::ALT+Qt::Key_J);
		oo->addAction(TR("bitmap EPS"), QMGL, SLOT(exportBPS()));
		oo->addAction(TR("vector EPS"), QMGL, SLOT(exportEPS()),Qt::ALT+Qt::Key_E);
		oo->addAction(TR("SVG"), QMGL, SLOT(exportSVG()),Qt::ALT+Qt::Key_S);
		oo->addAction(TR("IDTF"), QMGL, SLOT(exportIDTF()));
		o->addMenu(oo);
		popup->addMenu(oo);

		o->addSeparator();
		a = new QAction(QPixmap(fileprint), TR("Print &graphics"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(print()));
		a->setToolTip(TR("Open printer dialog and print graphics\t(CTRl+P)"));
		a->setShortcut(Qt::CTRL+Qt::Key_P);	o->addAction(a);
		o->addSeparator();
		o->addAction(TR("&Close"), Wnd, SLOT(close()), Qt::CTRL+Qt::Key_W);
	}
	// graphics menu
	{
		bb = new QToolBar(TR("Graphics"),Wnd);
		Wnd->addToolBar(Qt::TopToolBarArea, bb);
		o = Wnd->menuBar()->addMenu(TR("&Graphics"));
		a = new QAction(QPixmap(alpha_xpm), TR("&Alpha"), Wnd);
		a->setShortcut(Qt::ALT+Qt::Key_T);	a->setCheckable(true);
		Wnd->connect(a, SIGNAL(toggled(bool)), QMGL, SLOT(setAlpha(bool)));
		Wnd->connect(QMGL, SIGNAL(alphaChanged(bool)), a, SLOT(setOn(bool)));
		a->setToolTip(TR("Switch on/off TRansparency for the graphics (Alt+T)."));
		o->addAction(a);		bb->addAction(a);
		a = new QAction(QPixmap(light_xpm), TR("&Light"), Wnd);
		a->setShortcut(Qt::ALT+Qt::Key_L);	a->setCheckable(true);
		Wnd->connect(a, SIGNAL(toggled(bool)), QMGL, SLOT(setLight(bool)));
		Wnd->connect(QMGL, SIGNAL(lightChanged(bool)), a, SLOT(setOn(bool)));
		a->setToolTip(TR("Switch on/off lightning for the graphics (Alt+L)."));
		o->addAction(a);		bb->addAction(a);
		a = new QAction(QPixmap(rotate_xpm), TR("&Rotate by mouse"), Wnd);
		a->setCheckable(true);
		Wnd->connect(a, SIGNAL(toggled(bool)), QMGL, SLOT(setRotate(bool)));
		Wnd->connect(QMGL, SIGNAL(rotateChanged(bool)), a, SLOT(setOn(bool)));
		a->setToolTip(TR("Switch on/off mouse handling of the graphics\n(rotation, shifting, zooming and perspective)."));
		bb->addAction(a);
		a = new QAction(QPixmap(zoom_in_xpm), TR("&Zoom by mouse"), Wnd);
		a->setCheckable(true);
		Wnd->connect(a, SIGNAL(toggled(bool)), QMGL, SLOT(setZoom(bool)));
		Wnd->connect(QMGL, SIGNAL(zoomChanged(bool)), a, SLOT(setOn(bool)));
		a->setToolTip(TR("Switch on/off mouse zoom of selected region."));
		bb->addAction(a);
		o->addSeparator();
		a = new QAction(QPixmap(zoom_out_xpm), TR("Res&tore"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(restore()));
		a->setToolTip(TR("Restore default graphics rotation, zoom and perspective (Alt+Space)."));
		a->setShortcut(Qt::ALT+Qt::Key_Space);
		o->addAction(a);	bb->addAction(a);	popup->addAction(a);
		bb->addSeparator();
		a = new QAction(QPixmap(ok_xpm), TR("Re&draw"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(update()));
		a->setToolTip(TR("Execute script and redraw graphics (F5)."));
		a->setShortcut(Qt::Key_F5);
		o->addAction(a);	bb->addAction(a);	popup->addAction(a);
		a = new QAction(TR("&Adjust size"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(adjust()));
		a->setToolTip(TR("Change canvas size to fill whole region (F6)."));
		a->setShortcut(Qt::Key_F6);		o->addAction(a);
		a = new QAction(QPixmap(copy_xpm), TR("&Copy plot"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(copy()));
		a->setToolTip(TR("Copy graphics to clipboard (CTRl+C)."));
		a->setShortcut(Qt::CTRL+Qt::Key_C);
		o->addAction(a);		bb->addAction(a);	popup->addAction(a);
		bb->addSeparator();
		tet = new QSpinBox(Wnd);	tet->setWrapping(true);
		bb->addWidget(tet);	tet->setRange(-180, 180);	tet->setSingleStep(10);
		Wnd->connect(tet, SIGNAL(valueChanged(int)), QMGL, SLOT(setTet(int)));
		Wnd->connect(QMGL, SIGNAL(tetChanged(int)), tet, SLOT(setValue(int)));
		tet->setToolTip(TR("Set value of \\theta angle."));
		bb->addSeparator();
		phi = new QSpinBox(Wnd);	phi->setWrapping(true);
		bb->addWidget(phi);	phi->setRange(-180, 180);	phi->setSingleStep(10);
		Wnd->connect(phi, SIGNAL(valueChanged(int)), QMGL, SLOT(setPhi(int)));
		Wnd->connect(QMGL, SIGNAL(phiChanged(int)), phi, SLOT(setValue(int)));
		phi->setToolTip(TR("Set value of \\phi angle."));
//	bb->addSeparator();
	}
	// zooming menu
	{
		oo = o->addMenu(TR("Zoom/move"));
		bb = new QToolBar(TR("Zoom graphics"),Wnd);
		Wnd->addToolBar(Qt::LeftToolBarArea, bb);
		a = new QAction(QPixmap(left_1_xpm), TR("Move &left"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(shiftLeft()));
		a->setToolTip(TR("Move graphics left by 1/3 of its width."));
		bb->addAction(a);		oo->addAction(a);
		a = new QAction(QPixmap(up_1_xpm), TR("Move &up"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(shiftUp()));
		a->setToolTip(TR("Move graphics up by 1/3 of its height."));
		bb->addAction(a);		oo->addAction(a);
		a = new QAction(QPixmap(zoom_1_xpm), TR("Zoom &in"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(zoomIn()));
		a->setToolTip(TR("Zoom in graphics."));
		bb->addAction(a);		oo->addAction(a);
		a = new QAction(QPixmap(norm_1_xpm), TR("Zoom &out"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(zoomOut()));
		a->setToolTip(TR("Zoom out graphics."));
		bb->addAction(a);		oo->addAction(a);
		a = new QAction(QPixmap(down_1_xpm), TR("Move &down"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(shiftDown()));
		a->setToolTip(TR("Move graphics up down 1/3 of its height."));
		bb->addAction(a);		oo->addAction(a);
		a = new QAction(QPixmap(right_1_xpm), TR("Move &right"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(shiftRight()));
		a->setToolTip(TR("Move graphics right by 1/3 of its width."));
		bb->addAction(a);		oo->addAction(a);
	}
	// animation menu
	{
		o = Wnd->menuBar()->addMenu(TR("&Animation"));
		bb = new QToolBar(TR("Animation"),Wnd);
		Wnd->addToolBar(Qt::LeftToolBarArea, bb);
		a = new QAction(QPixmap(next_sl_xpm), TR("&Next slide"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(nextSlide()));
		a->setToolTip(TR("Show next slide (Alt+Right)."));
		a->setShortcut(Qt::ALT+Qt::Key_Right);	o->addAction(a);		bb->addAction(a);
		a = new QAction(QPixmap(show_sl_xpm), TR("&Slideshow"), Wnd);
		a->setCheckable(true);	anim = a;
		Wnd->connect(a, SIGNAL(toggled(bool)), QMGL, SLOT(animation(bool)));
		a->setToolTip(TR("Run slideshow (CTRl+F5)."));
		a->setShortcut(Qt::CTRL+Qt::Key_F5);	o->addAction(a);		bb->addAction(a);
		a = new QAction(QPixmap(prev_sl_xpm), TR("&Prev slide"), Wnd);
		Wnd->connect(a, SIGNAL(activated()), QMGL, SLOT(prevSlide()));
		a->setToolTip(TR("Show previous slide (Alt+Left)."));
		a->setShortcut(Qt::ALT+Qt::Key_Left);	o->addAction(a);		bb->addAction(a);
	}

	Wnd->menuBar()->addSeparator();
	o = Wnd->menuBar()->addMenu(TR("&Help"));
	o->addAction(TR("About"), QMGL, SLOT(about()));
	o->addAction(TR("About &Qt"), QMGL, SLOT(aboutQt()));
}
//-----------------------------------------------------------------------------
/*//
//	Function for mglGraphAB::ShowImage()
//
//-----------------------------------------------------------------------------
void mgl_show_img_qt(const char *title, mglGraphAB *gr, bool same, bool thread)
{
	static QMainWindow *Wnd=NULL;
	static mglQShowImg *img=NULL;
	if(same && Wnd)		// if prev window then show it
	{
		convertFromGraph(img->pic, gr, &(img->buf));
		Wnd->setWindowTitle(title);
		Wnd->show();	return;
	}
	if(!qApp)	// if no application then create it
	{
		QApplication *a;
//		if(!argv)
		{
			static char tmp[2][1];
			tmp[0][0]=tmp[1][0]=0;
			int argc=1;
			a = new QApplication(argc, (char **)tmp);
		}
		a->connect(a, SIGNAL(lastWindowClosed()), a, SLOT(quit()));
	}
	// create new window
	Wnd = new QMainWindow;	Wnd->resize(gr->GetWidth(),gr->GetHeight());
	Wnd->setWindowTitle(title);
	img = new mglQShowImg(Wnd);
	convertFromGraph(img->pic, gr, &(img->buf));
	Wnd->setCentralWidget(img);
	// setup menu
	QMenu *o = Wnd->menuBar()->addMenu(TR("&File"));
	o->addAction(TR("Save"), img, SLOT(save()), Qt::CTRL+Qt::Key_S);
	o->addAction(TR("Copy"), img, SLOT(copy()), Qt::CTRL+Qt::Key_C);
	o->addAction(TR("Print"), img, SLOT(print()), Qt::CTRL+Qt::Key_P);
	o->addSeparator();
	o->addAction(TR("&Close"), Wnd, SLOT(close()), Qt::CTRL+Qt::Key_W);
	Wnd->menuBar()->addSeparator();
	o = Wnd->menuBar()->addMenu(TR("&Help"));
	o->addAction(TR("About"), img, SLOT(about()));
	o->addAction(TR("About &Qt"), img, SLOT(aboutQt()));
	// now show it
	Wnd->show();
	if(thread)	mgl_qt_thread();	else mgl_qt_run();
}
//-----------------------------------------------------------------------------
mglQShowImg::mglQShowImg(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f)
{	buf=NULL;	}
//-----------------------------------------------------------------------------
void mglQShowImg::paintEvent(QPaintEvent *)
{
	QPainter paint;
	paint.begin(this);
	paint.drawPixmap(0,0,pic);
	paint.end();
}
//-----------------------------------------------------------------------------
void mglQShowImg::resizeEvent(QResizeEvent *)
{	resize(pic.width(), pic.height());	}
//-----------------------------------------------------------------------------
void mglQShowImg::about()
{
	QString s = tr("MathGL v. 1.") + QString::number(MGL_VERSION) +
			tr("\n(c) Alexey Balakin, 2007\nhttp://mathgl.sourceforge.net/");
	QMessageBox::about(this, tr("MathGL - about"), s);
}
//-----------------------------------------------------------------------------
void mglQShowImg::aboutQt()	{	QMessageBox::aboutQt(this, tr("About Qt"));	}
//-----------------------------------------------------------------------------
void mglQShowImg::copy()
{
	QClipboard *cb = QApplication::clipboard();
	cb->setPixmap(pic, QClipboard::Clipboard);
}
//-----------------------------------------------------------------------------
void mglQShowImg::print()
{
	QPrinter *printer = new QPrinter;
	QPrintDialog printDlg(printer, this);
	if (printDlg.exec() == QDialog::Accepted)
	{
		QPainter p;
		if(!p.begin(printer))	return;	// paint on printer
		p.drawPixmap(0,0,pic);
	}
	delete printer;
}
//-----------------------------------------------------------------------------
void mglQShowImg::save()
{
	QString fn = QFileDialog::getSaveFileName(this, tr("MathGL - save image"), "", tr("PNG files (*.png)\nJPEG files (*.jpg)\nAll files (*.*)"));
	if(!fn.isEmpty())	pic.save(fn);
}
//-----------------------------------------------------------------------------
*/