/* mgl_qt.h is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>
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
//-----------------------------------------------------------------------------
#ifndef QMGLCANVAS_H
#define QMGLCANVAS_H
//-----------------------------------------------------------------------------
#include <QWidget>
#include <QPixmap>
#include "mgl/mgl_zb.h"
//-----------------------------------------------------------------------------
class QTextEdit;
class QMenu;
class QMainWindow;
class QMathGL;
class QScrollArea;
class QSpinBox;
class QTimer;
int mglQtRun();
//-----------------------------------------------------------------------------
class mglGraphQT : public mglGraphZB
{
friend class QMathGL;
public:
	int sshow;		///< Current state of animation switch (toggle button)
	QMathGL *QMGL;	///< Control which draw graphics
	int CurFig;		///< Current figure in the list.
	QMainWindow *Wnd;	///< Pointer to window

	mglGraphQT();
	~mglGraphQT();

	void SetSize(int w,int h);
	void EndFrame();
	const unsigned char *GetBits();
	void Clf(mglColor Back=NC);
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Служебные ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/// Create a window for plotting. Now implemeted only for GLUT.
	void Window(int argc, char **argv, int (*draw)(mglGraph *gr, void *p),
						const char *title,void *par=NULL,
	  					void (*reload)(int next)=NULL);
	/// Switch on/off transparency (do not overwrite switches in user drawing function)
	void ToggleAlpha();
	/// Switch on/off lighting (do not overwrite switches in user drawing function)
	void ToggleLight();
	void ToggleZoom();	///< Switch on/off zooming by mouse
	void ToggleRotate();///< Switch on/off rotation by mouse
	void ToggleNo();	///< Switch off all zooming and rotation
	void Update();		///< Update picture by calling user drawing function
	void ReLoad(bool o);///< Reload user data and update picture
	void Adjust();		///< Adjust size of bitmap to window size
	void NextFrame();	///< Show next frame (if one)
	void PrevFrame();	///< Show previous frame (if one)
	void Animation();	///< Run slideshow (animation) of frames

protected:
	unsigned char *GG;	///< images for all frames (may be too LARGE !!!)
	int NumFig;			///< Number of figures in the list. If 0 then no list and mglGraph::DrawFunc will called for each drawing.
	void (*LoadFunc)(int);
	void *FuncPar;		///< Parameters for drawing function mglGraph::DrawFunc.
	/// Drawing function for window procedure. It should return the number of frames.
	int (*DrawFunc)(mglGraph *gr, void *par);
	QScrollArea *scroll;	///< Scrolling area
	QMenu *popup;			///< Popup menu
	QSpinBox *tet, *phi;	///< Spin box for angles
	QAction *anim;

	void makeMenu();		///< Create menu, toolbar and popup menu
};
//-----------------------------------------------------------------------------
/// Class for displaying the result of MGL script parsing
class QMathGL : public QWidget
{
Q_OBJECT
public:
	mglGraphAB *graph;	///< Built-in mglGraph-er instance (used by default)
	QString appName;	///< Application name for message boxes
	bool autoResize;	///< Allow auto resizing (default is false)
	void *draw_par;		///< Parameters for drawing function mglGraph::DrawFunc.
	/// Drawing function for window procedure. It should return the number of frames.
	int (*draw_func)(mglGraph *gr, void *par);
	int animDelay;		///< Animation delay in ms

	QMathGL(QWidget *parent = 0, Qt::WindowFlags f = 0);
	~QMathGL();
	double getRatio()	{	return double(graph->GetWidth())/graph->GetHeight();	};
	void setPopup(QMenu *p)	{	popup = p;	};	///< Set popup menu pointer
	void setSize(int w, int h);		///< Set window/picture sizes

	int getPer()	{return per;};		///< Get perspective value
	int getPhi()	{return phi;};		///< Get Phi-angle value
	int getTet()	{return tet;};		///< Get Theta-angle value
	bool getAlpha()	{return alpha;};	///< Get transparency state
	bool getLight()	{return light;};	///< Get lightning state
	bool getZoom()	{return zoom;};		///< Get mouse zooming state
	bool getRotate(){return rotate;};	///< Get mouse rotation state

public slots:
	void refresh();
	void update(mglGraph *gr=0);	///< Update picture
	void copy();			///< copy graphics to clipboard
//	void stop();			///< Stop execution
	void setPer(int p);		///< Set perspective value
	void setPhi(int p);		///< Set Phi-angle value
	void setTet(int t);		///< Set Theta-angle value
	void setAlpha(bool a);	///< Switch on/off transparency
	void setLight(bool l);	///< Switch on/off lightning
	void setZoom(bool z);	///< Switch on/off mouse zooming
	void setRotate(bool r);	///< Switch on/off mouse rotation
	void zoomIn();			///< Zoom in graphics
	void zoomOut();			///< Zoom out graphics
	void restore();			///< Restore zoom and rotation to default values
//	void reload();			///< Reload data and execute script
	void shiftLeft();		///< Shift graphics to left direction
	void shiftRight();		///< Shift graphics to right direction
	void shiftUp();			///< Shift graphics to up direction
	void shiftDown();		///< Shift graphics to down direction
	void exportPNG(QString fname="");	///< export to PNG file
	void exportPNGs(QString fname="");	///< export to PNG file (no transparency)
	void exportJPG(QString fname="");	///< export to JPEG file
	void exportTIF(QString fname="");	///< export to TIFF file
	void exportBPS(QString fname="");	///< export to bitmap EPS file
	void exportEPS(QString fname="");	///< export to vector EPS file
	void exportSVG(QString fname="");	///< export to SVG file
	void exportIDTF(QString fname="");	///< export to IDTF file
	void setMGLFont(QString path);		///< restore/load font for graphics
	void print();		///< Print plot
	//----These functions are executed only if graph is mglGraphQT instance----
	void adjust();		///< Adjust plot size to fill entire window
	void nextSlide();	///< Show next slide
	void prevSlide();	///< Show previous slide
	void animation(bool st=true);	///< Start animation
	void about();		///< Show about information
	void aboutQt();		///< Show information about Qt version
signals:
	void phiChanged(int);	///< Phi angle changed (by mouse or by toolbar)
	void tetChanged(int);	///< Tet angle changed (by mouse or by toolbar)
	void perChanged(int);	///< Perspective changed (by mouse or by toolbar)
	void alphaChanged(bool);	///< Transparency changed (by toolbar)
	void lightChanged(bool);	///< Lighting changed (by toolbar)
	void zoomChanged(bool);		///< Zooming changed (by toolbar)
	void rotateChanged(bool);	///< Rotation changed (by toolbar)
protected:
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);

	QPixmap pic;		///< Pixmap for drawing (changed by update)
	double tet, phi;	///< Rotation angles
	double per;			///< Value of perspective ( must be in [0,1) )
	bool alpha;			///< Transparency state
	bool light;			///< Lightning state
	bool zoom;			///< Mouse zoom state
	bool rotate;		///< Mouse rotation state
	float x1,x2,y1,y2;	///< Zoom in region
	bool showMessage;	///< Flag for showing messages (enabled by each execute())
	QMenu *popup;		///< Pointer to pop-up menu
	QTimer *timer;		///< Timer for animation
private:
	int x0, y0, xe, ye;		///< Temporary variables for mouse
	uchar *grBuf;
};
//-----------------------------------------------------------------------------
/// Convert bitmap from mglGraphAB to QPixmap
void convertFromGraph(QPixmap &pic, mglGraphAB *gr, uchar **buf);
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
