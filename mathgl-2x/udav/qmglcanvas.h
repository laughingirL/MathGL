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
#ifndef QMGLCANVAS_H
#define QMGLCANVAS_H
//-----------------------------------------------------------------------------
#include <qwidget.h>
#include <QPixmap>
#include <QThread>
#include <mgl/mgl.h>
class QTextEdit;
class QMenu;
class QMGLCanvas;
/// Class for making picture in separate thread
class MGLThread : public QThread
{
friend class QMGLCanvas;
Q_OBJECT
protected:
	QMGLCanvas *cnv;
		mglGraph *gr;
	QString mes;	///< gr->Message after execution
	QString text;	///< text for parsing
	int line;		///< line to highlite
	QString warn;
	void run();
};
//-----------------------------------------------------------------------------
/// Class for displaying the result of MGL script parsing
class QMGLCanvas : public QWidget
{
friend class MGLThread;
Q_OBJECT
public:
	bool autoResize;		///< Allow auto resizing (default is false)
	QString appName;		///< Application name for message boxes
	QTextEdit *textMGL;		///< Editor with MGL script body
	QTextEdit *warnMGL;		///< Buffer for messages and warnings
	QString scriptName;		///< Default script name which used for exporting
		mglGraph *graph;		///< Built-in mglGraph-er instance (used by default)

	QMGLCanvas(QWidget *parent = 0, Qt::WindowFlags f = 0);
	~QMGLCanvas();
	double getRatio()	{	return double(graph->GetWidth())/graph->GetHeight();	};
	void setPopup(QMenu *p)	{	popup = p;	};
	void setSize(int w, int h);
	/// Execute script from string
		QString run(const QString &text, bool showm=true, mglGraph *gr=0, int line=-1);
	const QPixmap &getPic()	{	return pic;	};	///< Pixmap for drawing
signals:
	void refreshData();
public slots:
	void execute(mglGraph *gr=0, int line=-1);	///< Execute script (and update picture)
	void copy();			///< copy graphics to clipboard
	void copyClickCoor();	///< copy click coordinates to clipboard
	void stop();			///< Stop execution
	void setPer(int p);		///< Set perspective value
	void setPhi(int p);		///< Set Phi-angle value
	void setTet(int t);		///< Set Theta-angle value
	void setGrid(bool a);	///< Switch on/off grid of absolute coor
	void setAlpha(bool a);	///< Switch on/off transparency
	void setLight(bool l);	///< Switch on/off lightning
	void setRotate(bool r);	///< Switch on/off mouse rotation
	void imgSize(int w, int h);	///< Set image size
	void restore();			///< Restore zoom and rotation to default values
	void reload();			///< Reload data and execute script
	void exportPNG(QString fname="");	///< export to PNG file
	void exportPNGs(QString fname="");	///< export to PNG file (no transparency)
	void exportJPG(QString fname="");	///< export to JPEG file
	void exportGIF(QString fname="");	///< export to GIF file
	void exportBPS(QString fname="");	///< export to bitmap EPS file
	void exportEPS(QString fname="");	///< export to vector EPS file
	void exportSVG(QString fname="");	///< export to SVG file
	void exportCPP(QString fname="");	///< export to CPP file
	void exportIDTF(QString fname="");	///< export to IDTF file
	void setMGLFont(QString path);		///< restore/load font for graphics
	void startGIF(int animDelay=500, QString fname="");
	void closeGIF();
signals:
	void phiChanged(int);
	void tetChanged(int);
	void perChanged(int);
	void gridChanged(bool);
	void alphaChanged(bool);
	void lightChanged(bool);
	void rotateChanged(bool);
	void posChanged(QString message);	///< user click to show mouse position
	void objChanged(int objId);			///< User double-click to select object/line
protected:
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseDoubleClickEvent(QMouseEvent *);

	MGLThread *thr;		///< Thread for making picture
	QPixmap pic;		///< Pixmap for drawing (changed by update)
	double tet, phi;	///< Rotation angles
	double per;			///< Value of perspective ( must be in [0,1) )
	bool grid;			///< Grid state
	bool alpha;			///< Transparency state
	bool light;			///< Lightning state
	bool rotate;		///< Mouse rotation state
	bool showMessage;	///< Flag for showing messages (enabled by each execute())
	QMenu *popup;		///< Pointer to pop-up menu
	QString clickPos;	///< Position of last mouse click
private:
	int x0, y0, xe, ye;		///< Temporary variables for mouse
	uchar *grBuf;
	QString defText;		///< Default text for execution
};
//-----------------------------------------------------------------------------
/// Convert bitmap from mglGraph to QPixmap
void convertFromGraph(QPixmap &pic, mglGraph *gr, uchar **buf);
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
