/* mgl.h is part of Math Graphic Library
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
#ifndef _MGL_H_
#define _MGL_H_

#define MGL_VERSION	6.1

#include "mgl/mgl_data.h"
#include "mgl/mgl_font.h"
#include <math.h>

#ifndef NUM_COLOR
#define NUM_COLOR	7
#endif
//-----------------------------------------------------------------------------
class mglGraph;
class mglFormula;

enum{
	mglWarnNone = 0,// Everything OK
	mglWarnDim,		// Data dimension(s) is incompatible
	mglWarnLow, 	// Data dimension(s) is too small
	mglWarnNeg, 	// Minimal data value is negative
	mglWarnFile,	// No file or wrong data dimensions
	mglWarnMem,		// Not enough memory
	mglWarnZero,	// Data values are zero
	mglWarnLegA,	// Too many legend entries
	mglWarnLeg,		// No legend entries
	mglWarnSlc,		// Slice value is out of range
	mglWarnCnt,		// Number of contours is zero or negative
	mglWarnOpen,	// Couldn't open file
	mglWarnLId,		// Light: ID is out of range
	mglWarnSize,	// Setsize: size(s) is zero or negative
	mglWarnFmt,		// Format is not supported for that build
	mglWarnEnd		// Maximal number of warnings (must be last)
};
//-----------------------------------------------------------------------------
/// Class for incapsulating color
struct mglColor
{
	float r;	///< Red component of color
	float g;	///< Green component of color
	float b;	///< Blue component of color

	/// Constructor for RGB components manualy
	mglColor(float R,float G,float B){	r=R;	g=G;	b=B;	};
	/// Constructor set color from character id
	mglColor(char c='k'){	Set(c);	};
	/// Set color as Red, Green, Blue values
	void Set(float R,float G,float B)	{r=R;	g=G;	b=B;};
	/// Set color as Red, Green, Blue values
	void Set(mglColor c)	{r=c.r;	g=c.g;	b=c.b;};
	/// Check if color is valid
	inline bool Valid()
	{	return (r>=0 && r<=1 && g>=0 && g<=1 && b>=0 && b<=1);	};
	/// Get maximal spectral component
	inline float Norm()
	{	return r>g ? r : (g>b ? g : b);	};
	/// Set color from symbolic id
	void Set(char p);
	/// Copy color from other one
	bool operator==(mglColor c)
	{	return (r==c.r && g==c.g && b==c.b);	};
};
inline mglColor operator+(mglColor a, mglColor b)
{	return mglColor(a.r+b.r, a.g+b.g, a.b+b.b);	};
inline mglColor operator-(mglColor a, mglColor b)
{	return mglColor(a.r-b.r, a.g-b.g, a.b-b.b);	};
inline mglColor operator*(mglColor a, float b)
{	return mglColor(a.r*b, a.g*b, a.b*b);	};
inline mglColor operator*(float b, mglColor a)
{	return mglColor(a.r*b, a.g*b, a.b*b);	};
inline mglColor operator/(mglColor a, float b)
{	return mglColor(a.r/b, a.g/b, a.b/b);	};
inline mglColor operator!(mglColor a)
{	return mglColor(1-a.r, 1-a.g, 1-a.b);	} //-----------------------------------------------------------------------------
#define NC	mglColor(-1,-1,-1)
#define BC	mglColor( 0, 0, 0)
#define WC	mglColor( 1, 1, 1)
#define RC	mglColor( 1, 0, 0)
//-----------------------------------------------------------------------------
/// Structure for color ID
struct mglColorID
{
	char id;
	mglColor col;
};
extern mglColorID mglColorIds[];
//-----------------------------------------------------------------------------
/// Class for incapsulating point in space
struct mglPoint
{
	float x,y,z;
 	mglPoint(float X=0,float Y=0,float Z=0){x=X;y=Y;z=Z;};
};
inline mglPoint operator+(mglPoint a, mglPoint b)
{	return mglPoint(a.x+b.x, a.y+b.y, a.z+b.z);	};
inline mglPoint operator-(mglPoint a, mglPoint b)
{	return mglPoint(a.x-b.x, a.y-b.y, a.z-b.z);	};
inline mglPoint operator*(float b, mglPoint a)
{	return mglPoint(a.x*b, a.y*b, a.z*b);	};
inline mglPoint operator*(mglPoint a, float b)
{	return mglPoint(a.x*b, a.y*b, a.z*b);	};
inline mglPoint operator/(mglPoint a, float b)
{	return mglPoint(a.x/b, a.y/b, a.z/b);	};
inline float operator*(mglPoint a, mglPoint b)
{	return a.x*b.x+a.y*b.y+a.z*b.z;	};
inline mglPoint operator&(mglPoint a, mglPoint b)
{	return a - b*((a*b)/(b*b));	};
inline mglPoint operator|(mglPoint a, mglPoint b)
{	return b*((a*b)/(b*b));	};
inline mglPoint operator^(mglPoint a, mglPoint b)
{	return mglPoint(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x);	};
inline mglPoint operator!(mglPoint a)
{	return (a.x==0 && a.y==0)?mglPoint(1,0,0):mglPoint(-a.y/hypot(a.x,a.y), a.x/hypot(a.x,a.y), 0);	};
float Norm(mglPoint p);
//-----------------------------------------------------------------------------
/// Structure for the command argument (see mglGraph::Exec()).
struct mglArg
{
	int type;		///< Type of argument {0-data,1-string,2-number}
	mglData *d;		///< Pointer to data (used if type==0)
	char s[2048];	///< String with parameters (used if type==1)
	float v;		///< Numerical value (used if type==2)
	mglArg()	{	type=-1;	d=0;	v=0;	s[0]=0;	};
};
//-----------------------------------------------------------------------------
float GetX(mglData &x, int i, int j, int k);
float GetY(mglData &y, int i, int j, int k);
typedef int (*mgl_save) (const char *fname, int w, int h, unsigned char **);
//-----------------------------------------------------------------------------
/// Class contains base functionality for creating different mathematical plots
class mglGraph
{
friend class mglFont;
friend class mglParse;
public:
	mglPoint Min;		///< Lower edge of bounding box for graphics.
	mglPoint Max;		///< Upper edge of bounding box for graphics.
	float Cmin;			///< Minimal value for data coloring.
	float Cmax;			///< Maximal value for data coloring.
	mglPoint Org;		///< Center of axis cross section.
	bool AutoOrg;		///< Shift Org automatically if it lye outside Min ... Max range
	mglColor Pal[101];	///< Color palette for 1D plotting.
	int NumPal;			///< Number of colors in palette.
	char FontDef[32];	///< Font specification (see mglGraph::Puts). Default is Roman with align at center.
	bool RotatedText;	///< Use text rotation along axis
	const char *PlotId;		///< Id of plot for saving filename (in GLUT window for example)
	int MeshNum;		///< Set approximate number of lines in mglGraph::Mesh and mglGraph::Grid. By default (=0) it draw all lines.

	float dx;			///< Step for axis mark (if positive) or its number (if negative) in x direction.
	float dy;			///< Step for axis mark (if positive) or its number (if negative) in y direction.
	float dz;			///< Step for axis mark (if positive) or its number (if negative) in z direction.
	float NSx;			///< Number of axis submarks in x direction
	float NSy;			///< Number of axis submarks in y direction
	float NSz;			///< Number of axis submarks in z direction.
	mglFormula *fx;		///< Transformation formula for x direction.
	mglFormula *fy;		///< Transformation formula for y direction.
	mglFormula *fz;		///< Transformation formula for z direction.
	mglFormula *fc;		///< Cutting off condition (formula).
	const wchar_t *xtt;	///< X-tick template (set NULL to use default one ("%.2g" in simplest case))
	const wchar_t *ytt;	///< Y-tick template (set NULL to use default one ("%.2g" in simplest case))
	const wchar_t *ztt;	///< Z-tick template (set NULL to use default one ("%.2g" in simplest case))
	const wchar_t *ctt;	///< Colorbar-tick template (set NULL to use default one ("%.2g" in simplest case))
	/// Function for writting tick label to \a str for axis in direction \a dir for value \a val. Must return true if \a str should be used as tick label.
	bool (*TickStr)(char dir, float val, wchar_t str[64]);
	/// Factor for sizing overall plot (should be >1.6, default is =2)
	float PlotFactor;
	/// Enable autochange PlotFactor
	bool AutoPlotFactor;

	/// Flag which determines how points outside bounding box are drown.
	bool Cut;
	mglPoint CutMin;	///< Lower edge of bounding box for cut off.
	mglPoint CutMax;	///< Upper edge of bounding box for cut off.
	/// Type of transparency (no full support in OpenGL mode).
	int TranspType;
	bool Transparent;	///< Flag which temporaly switch on/off transparency for plot
	float BarWidth;		///< Relative width of rectangles in mglGraph::Bars().
	float MarkSize;		///< The size of marks for 1D plots.
	float ArrowSize;	///< The size of arrows.
	float BaseLineWidth;	///< Base line width (as factor). Useful for LARGE bitmap plots.

	float FontSize;		///< The size of font for tick and axis labels
	bool LegendBox;		///< Set on/off drawing legend box.
	char AxialDir;		///< Direction of rotation for Axial() and Torus().
	bool DrawFace;		///< Switch on/off face drawing (for faster plot rotation, redrawing and so on)
	/// Default value of alpha channel for transparency for all plotting functions.
	float AlphaDef;
	char *Message;		///< Buffer for receiving messages
	int WarnCode;		///< Warning code
	bool TuneTicks;		///< Draw tuned ticks with extracted common component
	float FactorPos;	///< Position of axis ticks factor (0 at Min, 1 at Max, 1.1 is default)
	int CirclePnts;		///< Number of points for a circle drawing (used in Tube(), Drop(), Sphere(), Cone())

	/// Set default parameter for plotting
	mglGraph();
	/// Clear the used variables
	virtual ~mglGraph();
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** @name Export functions
	  * These functions export current view to a graphic file.
	  * The filename \a fname should have apropriate extension.
	  * Parameter \a descr gives the short description of the plot.
	  * Just now the compression of TIFF files and transparency in
	  * EPS files are not supported.
	  */
	//@{
	/// Write the frame in file using TIFF format
	virtual void WriteTIFF(const char *fname,const char *descr=0,int compr=0);
	/// Write the frame in file using JPEG format
	virtual void WriteJPEG(const char *fname,const char *descr=0);
	/// Write the frame in file using BMP format
	virtual void WriteBMP(const char *fname,const char *descr=0);
	/// Write the frame in file using PNG format
	virtual void WritePNG(const char *fname,const char *descr=0,bool alpha=true);
	/// Write the frame in file using PostScript format
	virtual void WriteEPS(const char *fname,const char *descr=0);
	/// Write the frame in file using SVG format
	virtual void WriteSVG(const char *fname,const char *descr=0);
	//@}
	/// Create new frame.
	virtual int NewFrame(int id=0);
	/// Finish frame drawing
	virtual void EndFrame();
	/// Get the number of created frames
	int GetNumFrame()	{	return CurFrameId-1;	};
	/// Flush the plotting commands to frame.
	virtual void Flush();
	/// Finish plotting. Normally this function is called internaly.
	virtual void Finish();
	/// Set the transparency on/off.
	virtual void Alpha(bool enable)=0;
	/// Set the fog distance or switch it off (if d=0).
	virtual void Fog(float d, float dz=0.25);
	/// Set the using of light on/off.
	virtual void Light(bool enable)=0;
	/// Switch on/off the specified light source.
	virtual void Light(int n, bool enable); //=0
	/// Add a light source.
	void Light(int n,mglPoint p, char c='w', float bright=0.5, bool infty=true);
	/// Add a light source.
	virtual void Light(int n,mglPoint p, mglColor c, float bright=0.5, bool infty=true); //=0
	/// Set ambient light brightness
	virtual void Ambient(float bright=0.5);
	/// Set colormap scheme for surfaces (usualy called internaly)
	void SetScheme(const char *sch);
	/// Set the parameter of line (usualy called internaly)
	char SelectPen(const char *pen);
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** @name Plot positioning functions
	  * These functions control how and where further plotting will be placed.
	  * There are a curtain order of these functions calls for
	  * the better plot view. First one is mglGraph::SubPlot or mglGraph::InPlot
	  * for specifing the place. After it a mglGraph::Aspect and mglGraph::Rotate.
	  * And finally any other plotting functions may be called. The function mglGraph::View
	  * can be used in any place. */
	//@{
	/// Set angle of view indepently from mglGraph::Rotate().
	virtual void View(float tetx,float tetz,float tety=0);
	void DefaultPlotParam();	///< Set default parameter for plotting
	/// Zoom in or zoom out (if Zoom(0, 0, 1, 1)) a part of picture
	void Zoom(float x1, float y1, float x2, float y2);
	/// Clear transformation matrix.
	void Identity();
	/// Clear up the frame
	virtual void Clf(mglColor Back=WC); //=0
	/// Put further plotting in some region of whole frame surface.
	void SubPlot(int nx,int ny,int m);
	/// Put further plotting in some region of whole frame surface.
	virtual void InPlot(float x1,float x2,float y1,float y2); //=0
	/// Set aspect ratio for further plotting.
	virtual void Aspect(float Ax,float Ay,float Az)=0;
	/// Rotate a further plotting.
	void Rotate(float TetX,float TetZ,float TetY=0);
	/// Rotate a further plotting around vector {x,y,z}.
	virtual void RotateN(float Tet,float x,float y,float z)=0;
	/// Set perspective (in range [0,1)) for plot. Set to zero for switching off.
	virtual void Perspective(float a)=0;
	/// Set size of frame in pixels. Normally this function is called internaly.
	virtual void SetSize(int w,int h);
	//@}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** @name Axis functions
	  * Axis functions control and draw the axes. There is twofold axis
	  * representation in MathGL. First the coordinates of data point are
	  * normalized in box mglGraph::Min X mglGraph::Max. If mglGraph::Cut is true
	  * then point is omitted otherwise it is projected to bounding box.
	  * After it transformation formulas mglGraph::fx, mglGraph::fy, mglGraph::fz
	  * are applied to the data point. Finally, the data point is plotted by
	  * one of function. There is possibility to set members mglGraph::Max,
	  * mglGraph::Min, mglGraph::fx, mglGraph::fy, mglGraph::fz directly. But one
	  * should call mglGraph::RecalcBorder function to setup plotting routines.
	  * The more safe way is to set these values by calling mglGraph::Axis functions.
	  * In this case the function mglGraph::RecalcBorder is called automaticly. */
	//@{
	/// Draws bounding box outside the plotting volume.
	void Box(mglColor pen=NC, bool ticks=true);
	/// Draws bounding box outside the plotting volume with color \a c.
	void Box(const char *col, bool ticks=true);

	/// Safety set values of mglGraph::Cmin and mglGraph::Cmax as minimal and maximal values of data a
	void CRange(mglData &a, bool add = false);
	/// Safety set values of mglGraph::Min.x and mglGraph::Max.x as minimal and maximal values of data a
	void XRange(mglData &a, bool add = false);
	/// Safety set values of mglGraph::Min.x and mglGraph::Max.x as minimal and maximal values of data a
	void YRange(mglData &a, bool add = false);
	/// Safety set values of mglGraph::Min.x and mglGraph::Max.x as minimal and maximal values of data a
	void ZRange(mglData &a, bool add = false);

	/// Safetly set the values of mglGraph::Cmin and mglGraph::Cmax
	void inline CAxis(float C1,float C2)	{	Cmin=C1;	Cmax=C2;	};
	/// Safetly set the value for mglGraph::Min, mglGraph::Max and mglGraph::Org members of the class.
	void Axis(mglPoint Min, mglPoint Max, mglPoint Org=mglPoint(0,0,0));
	/// Safetly set the transformation formulas for coordinate.
	void Axis(const char *EqX,const char *EqY,const char *EqZ);
	/// Safetly set the cutting off condition (formula).
	void CutOff(const char *EqC);
	/// Set to draw Ternary axis (triangle like axis, grid and so on)
	void Ternary(bool tern);
	/// Recalculate internal parameter for correct applies transformation rules. \b Must \b be \b called after any direct changes of members mglGraph::Min, mglGraph::Max, mglGraph::fx, mglGraph::fy, mglGraph::fz.
	void RecalcBorder();
	/// Draw axises with ticks in directions determined by string parameter \a dir.
	void Axis(const char *dir="xyz");
	/// Draw grid lines perpendicular to direction determined by string parameter \a dir.
	void Grid(const char *dir="xyz",const char *pen="B-");
	/// Print the label \a text for axis \a dir.
	void Label(char dir, const char *text, int pos=+1, float size=-1.4, float shift=0);
	/// Print the label \a text for axis \a dir.
	void Label(char dir, const wchar_t *text, int pos=+1, float size=-1.4, float shift=0);
	/// Draw colorbar at edge of axis
	virtual void Colorbar(const char *sch=0,int where=0)=0;
	//@}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** @name Primitive functions
	  * These functions draw the simplest primitives, like line, point, sphere, drop, cone and so on. */
	//@{
	/// Draws the point (ball) at position \a {x,y,z} with color \a col.
	virtual void Ball(float x,float y,float z,mglColor col=RC,float alpha=1)=0;
	/// Draws the point (ball) at position \a p with color \a col.
	inline void Ball(mglPoint p, char col='r')	{	Ball(p.x,p.y,p.z,mglColor(col));	};
	/// Draws the 3d error box e for point p
	void Error(mglPoint p, mglPoint e, const char *pen=0);
	/// Draws the line between points with style \a stl.
	void Line(mglPoint p1, mglPoint p2, const char *stl="B", int num=2);
	/// Draws the spline curve between points with style \a stl.
	void Curve(mglPoint p1, mglPoint d1, mglPoint p2, mglPoint d2, const char *stl="B",int num=100);
	/// Draws the face between points with color \a stl (include interpolation up to 4 colors).
	void Face(mglPoint p1, mglPoint p2, mglPoint p3, mglPoint p4, const char *stl="w", int num=2);
	/// Draws the sphere at point \a p with color \a stl and radius \a r.
	void Sphere(mglPoint p, float r, const char *stl="r");
	/// Draws the drop at point \a p in direction \a q with color \a stl and radius \a r.
	void Drop(mglPoint p, mglPoint q, float r, const char *stl="r", float shift=1, float ap=1);
	/// Draws the drop at point \a p in direction \a q with color \a stl and radius \a r.
	void Drop(mglPoint p, mglPoint q, float r, mglColor c, float shift=1, float ap=1);
	/// Draws the line between points with style \a stl.
	void Cone(mglPoint p1, mglPoint p2, float r1, float r2=-1, const char *stl="B", bool edge=false);
	/// draw mark with different type at position {x,y,z} (no scaling)
	virtual void Mark(mglPoint p,char mark='.');
	/// Draw a set of triangles (or lines if trig==NULL) for glyph from point (0,0). Normally this function is used internally.
	virtual void Glyph(float x,float y, float f,int nt, const short *trig, int nl, const short *line)=0;

	//@}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** @name Text functions
	  * Text functions draw the text. There is a function for drawing text in arbitrary place, in arbitrary direction and along arbitrary curve. The font style for text is specified by string argument. The size argument control the size of text: if positive it give the value if negative it give the value relative to FontSize. The font type (STIX, arial, courier, times and so on) can be selected by function SetFont(), GetFont().*/
	//@{
	/// Set FontSize by size in pt and picture DPI (default is 16 pt for dpi=72)
	virtual void SetFontSizePT(float pt, int dpi=72);
	/// Set FontSize by size in centimeters and picture DPI (default is 0.56 cm = 16 pt)
	inline void SetFontSizeCM(float cm, int dpi=72)
	{	SetFontSizePT(cm*28.45f,dpi);	};
	/// Set FontSize by size in inch and picture DPI (default is 0.22 in = 16 pt)
	inline void SetFontSizeIN(float in, int dpi=72)
	{	SetFontSizePT(in*72.27f,dpi);	};
	/// Set font typeface. Note that each mglFont instance can be used with ONLY ONE mglGraph instance at a moment of time!
	void SetFont(mglFont *f);
	/// Get current typeface. Note that this variable can be deleted at next SetFont() call!
	inline mglFont *GetFont()	{	return fnt;	};
	
	/// Get ratio (real width)/(real height).
	virtual float GetRatio();
	/// Print string \a str in position \a p with font size \a size.
	void Puts(mglPoint p,const char *text,const char *font=0,float size=-1,char dir=0,float shift=0);
	/// Print string \a str in position \a p with font size \a size.
	virtual void Putsw(mglPoint p,const wchar_t *text,const char *font=0,float size=-1,char dir=0,float shift=0)=0;
	/// Print string \a str with font size \a size in position \a p along direction \a l.
	float Puts(mglPoint p,mglPoint l,const char *text,char font='t',float size=-1);
	/// Print string \a str with font size \a size in position \a p along direction \a l.
	virtual float Putsw(mglPoint p,mglPoint l,const wchar_t *text,char font='t',float size=-1)=0;
	/// Print unrotated string \a str in position \a p with font size \a size.
	void Text(mglPoint p,const char *text,const char *font=0,float size=-1.4,char dir=0);
	/// Print unrotated string \a str in position \a p with font size \a size.
	void Text(mglPoint p,const wchar_t *text,const char *font=0,float size=-1.4,char dir=0);
	/// Print formated output in position \a p.
	void Printf(mglPoint p,const char *arg,...);
	/// Print string \a str along curve with font size \a size.
	void Text(mglData &y,const char *text,const char *font=0,float size=-1,float zVal=NAN);
	/// Print string \a str along parametrical curve with font size \a size.
	void Text(mglData &x,mglData &y,const char *text,const char *font=0,float size=-1,float zVal=NAN);
	/// Print string \a str along curve in 3D with font size \a size.
	void Text(mglData &x,mglData &y,mglData &z,const char *text,const char *font=0,float size=-1);
	/// Print string \a str along curve with font size \a size.
	void Text(mglData &y,const wchar_t *text,const char *font=0,float size=-1,float zVal=NAN);
	/// Print string \a str along parametrical curve with font size \a size.
	void Text(mglData &x,mglData &y,const wchar_t *text,const char *font=0,float size=-1,float zVal=NAN);
	/// Print string \a str along curve in 3D with font size \a size.
	void Text(mglData &x,mglData &y,mglData &z,const wchar_t *text,const char *font=0,float size=-1);

	//@}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** @name Legend functions
	  * Legend functions accumulate and draw legend strings. There is twofold legend representation in MathGL. First the string array can be drawn directly. Second the string can be accumulated in internal array (by AddLegend() function) and later drawn. The position of legend can be specified manually or by corner of plot (default is right-top). */
	//@{
	/// Add string to legend
	void AddLegend(const char *text,const char *style);
	/// Add string to legend
	void AddLegend(const wchar_t *text,const char *style);
	/// Clear saved legend string
	void ClearLegend();
	/// Draw legend of accumulated strings at position (x, y) by \a font with \a size
	void Legend(float x, float y, const char *font="rL", float size=-0.8);
	/// Draw legend of accumulated strings by \a font with \a size
	void Legend(int where=0x3, const char *font="rL", float size=-0.8);
	/// Draw legend strings \a text at position (x, y) by \a font with \a size
	void Legend(int n, wchar_t **text, char **style, float x, float y, const char *font="rL", float size=-0.8);
	/// Draw legend of accumulated strings by \a font with \a size
	void Legend(int n, wchar_t **text, char **style, int where=0x3, const char *font="rL", float size=-0.8);
	//@}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ ������� ~~~~~~~~~~~~~~~~~~~~~~~~
	/// Plot data depending on its dimensions and \a type parameter
	void SimplePlot(mglData &a, int type=0, const char *stl=0);
	/// Execute the command
	int Exec(const char *com, long narg, mglArg *args);
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** @name 1D plotting functions
	  * These functions perform plotting of 1D data. 1D means that data
	  * depended from only 1 parameter like parametric \b curve {x(i),y(i),z(i)},
	  * i=1...n. There are 5 generally different types of data representations:
	  * simple line plot (\c Plot), line plot with filling under it (\c Area),
	  * stairs plot (\c Step), bar plot (\c Bars) and vertical lines (\c Stem).
	  * Each type of plotting has similar interface. There are 3D version and
	  * two 2D versions. One of last requires only one array.
	  * The parameters of line and marks are specified by the string argument
	  * (see mglGraph::SelectPen). If the string parameter is NULL
	  * then solid line with color from palette is used. */
	//@{
	/// Draw line plot for points in arrays \a x, \a y, \a z.
	void Plot(mglData &x, mglData &y, mglData &z, const char *pen=0);
	/// Draw line plot for points in arrays \a x, \a y.
	void Plot(mglData &x, mglData &y, const char *pen=0,float zVal=NAN);
	/// Draw line plot for points in arrays \a y.
	void Plot(mglData &y, const char *pen=0,float zVal=NAN);
	/// Draw line plot for points in arrays \a a(0,:),\a a(1,:).
	void Plot2(mglData &a, const char *pen=0,float zVal=NAN);
	/// Draw line plot for points in arrays \a a(0,:),\a a(1,:),\a a(2,:).
	void Plot3(mglData &a, const char *pen=0);
	
	/// Draw area plot for points in arrays \a x, \a y, \a z.
	void Area(mglData &x, mglData &y, mglData &z, const char *pen=0);
	/// Draw area plot for points in arrays \a x, \a y.
	void Area(mglData &x, mglData &y, const char *pen=0,bool sum=false,float zVal=NAN);
	/// Draw area plot for points in arrays \a y.
	void Area(mglData &y, const char *pen=0,bool sum=false,float zVal=NAN);
	/// Draw area plot for points in arrays \a a(0,:),\a a(1,:).
	void Area2(mglData &a, const char *pen=0,float zVal=NAN);
	/// Draw area plot for points in arrays \a a(0,:),\a a(1,:),\a a(2,:).
	void Area3(mglData &a, const char *pen=0);
	
	/// Draw vertical lines from points in arrays \a x, \a y, \a z to mglGraph::Org.
	void Stem(mglData &x, mglData &y, mglData &z, const char *pen=0);
	/// Draw vertical lines from points in arrays \a x, \a y to mglGraph::Org.
	void Stem(mglData &x, mglData &y, const char *pen=0,float zVal=NAN);
	/// Draw vertical lines from points in arrays \a y to mglGraph::Org.
	void Stem(mglData &y, const char *pen=0,float zVal=NAN);
	/// Draw vertical lines from points in arrays \a a(0,:),\a a(1,:) to mglGraph::Org.
	void Stem2(mglData &a, const char *pen=0,float zVal=NAN);
	/// Draw vertical lines from points in arrays \a a(0,:),\a a(1,:),\a a(2,:) to mglGraph::Org.
	void Stem3(mglData &a, const char *pen=0);
	
	/// Draw stairs for points in arrays \a x, \a y, \a z.
	void Step(mglData &x, mglData &y, mglData &z, const char *pen=0);
	/// Draw stairs for points in arrays \a x, \a y.
	void Step(mglData &x, mglData &y, const char *pen=0,float zVal=NAN);
	/// Draw line plot for points in arrays \a y.
	void Step(mglData &y, const char *pen=0,float zVal=NAN);
	/// Draw stairs for points in arrays \a a(0,:),\a a(1,:).
	void Step2(mglData &a, const char *pen=0,float zVal=NAN);
	/// Draw stairs for points in arrays \a a(0,:),\a a(1,:),\a a(2,:).
	void Step3(mglData &a, const char *pen=0);
	
	/// Draw vertical bars from points in arrays \a x, \a y, \a z to mglGraph::Org.
	void Bars(mglData &x, mglData &y, mglData &z, const char *pen=0, bool above=false);
	/// Draw vertical bars from points in arrays \a x, \a y to mglGraph::Org.
	void Bars(mglData &x, mglData &y, const char *pen=0,float zVal=NAN, bool above=false);
	/// Draw vertical bars from points in arrays \a y to mglGraph::Org.
	void Bars(mglData &y, const char *pen=0,float zVal=NAN, bool above=false);
	/// Draw vertical bars from points in arrays \a a(0,:),\a a(1,:) to mglGraph::Org.
	void Bars2(mglData &a, const char *pen=0,float zVal=NAN, bool above=false);
	/// Draw vertical bars from points in arrays \a a(0,:),\a a(1,:),\a a(2,:) to mglGraph::Org.
	void Bars3(mglData &a, const char *pen=0, bool above=false);
	
	/// Draw surface of curve {\a r,\a z} rotatation around Z axis
	void Torus(mglData &r, mglData &z, const char *pen=0);
	/// Draw surface of curve rotatation around Z axis
	void Torus(mglData &z, const char *pen=0);
	/// Draw surface of curve {\a a(0,:),\a a(1,:)} rotatation around Z axis for
	void Torus2(mglData &a, const char *pen=0);
	
	/// Draw chart for data a
	void Chart(mglData &a, const char *col=0);
	
	/// Draw error boxes ey for data y
	void Error(mglData &y, mglData &ey, const char *pen=0,float zVal=NAN);
	/// Draw error boxes ey for data {x,y}
	void Error(mglData &x, mglData &y, mglData &ey, const char *pen=0,float zVal=NAN);
	/// Draw error boxes {ex,ey} for data {x,y}
	void Error(mglData &x, mglData &y, mglData &ex, mglData &ey, const char *pen=0,float zVal=NAN);
	
	/// Draw marks with diffenernt sizes \a r for points in arrays \a x, \a y, \a z.
	void Mark(mglData &x, mglData &y, mglData &z, mglData &r, const char *pen=0);
	/// Draw marks with diffenernt sizes \a r for points in arrays \a x, \a y.
	void Mark(mglData &x, mglData &y, mglData &r, const char *pen=0,float zVal=NAN);
	/// Draw marks with diffenernt sizes \a r for points in arrays \a y.
	void Mark(mglData &y, mglData &r, const char *pen=0,float zVal=NAN);
	
	/// Draw textual marks with diffenernt sizes \a r for points in arrays \a x, \a y, \a z.
	void TextMark(mglData &x, mglData &y, mglData &z, mglData &r, const char *text, const char *fnt=0);
	/// Draw textual marks with diffenernt sizes \a r for points in arrays \a x, \a y.
	void TextMark(mglData &x, mglData &y, mglData &r, const char *text, const char *fnt=0,float zVal=NAN);
	/// Draw textual marks with diffenernt sizes \a r for points in arrays \a y.
	void TextMark(mglData &y, mglData &r, const char *text, const char *fnt=0,float zVal=NAN);
	/// Draw textual marks with diffenernt sizes \a r for points in arrays \a x, \a y, \a z.
	void TextMark(mglData &x, mglData &y, mglData &z, mglData &r, const wchar_t *text, const char *fnt=0);
	/// Draw textual marks with diffenernt sizes \a r for points in arrays \a x, \a y.
	void TextMark(mglData &x, mglData &y, mglData &r, const wchar_t *text, const char *fnt=0,float zVal=NAN);
	/// Draw textual marks with diffenernt sizes \a r for points in arrays \a y.
	void TextMark(mglData &y, mglData &r, const wchar_t *text, const char *fnt=0,float zVal=NAN);
	/// Draw textual marks with diffenernt sizes \a r for points in arrays \a y.
	void TextMark(mglData &y, const char *text, const char *fnt=0,float zVal=NAN);
	/// Draw textual marks with diffenernt sizes \a r for points in arrays \a y.
	void TextMark(mglData &y, const wchar_t *text, const char *fnt=0,float zVal=NAN);
	
	/// Draw tube with radial sizes \a r for points in arrays \a x, \a y, \a z.
	void Tube(mglData &x, mglData &y, mglData &z, mglData &r, const char *pen=0);
	/// Draw tube with radial sizes \a r for points in arrays \a x, \a y.
	void Tube(mglData &x, mglData &y, mglData &r, const char *pen=0,float zVal=NAN);
	/// Draw tube with radial sizes \a r for points in arrays \a y.
	void Tube(mglData &y, mglData &r, const char *pen=0,float zVal=NAN);
	/// Draw tube with constant radial sizes \a r for points in arrays \a x, \a y, \a z.
	void Tube(mglData &x, mglData &y, mglData &z, float r, const char *pen=0);
	/// Draw tube with constant radial sizes \a r for points in arrays \a x, \a y.
	void Tube(mglData &x, mglData &y, float r, const char *pen=0,float zVal=NAN);
	/// Draw tube with constant radial sizes \a r for points in arrays \a y.
	void Tube(mglData &y, float r, const char *pen=0,float zVal=NAN);

	//@}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** @name 2D plotting functions
	  * These functions perform plotting of 2D data. 2D means that
	  * data depend from 2 independent parameters like \b matrix f(x_i,y_j),
	  * i=1...n, j=1...m. There are 6 generally different types of data
	  * representations: simple mesh lines plot (\c Mesh), surface plot
	  * (\c Surf), surface plot by boxes (\c Boxs), density plot
	  * (\c Dens), contour lines plot (\c Cont, \c ContF) and its rotational figure (\c Axial).
	  * \c Cont, \c ContF and \c Axial functions have variants for automatic and manual
	  * selection of level values for contours. Also there are
	  * functions for plotting data grid lines according to the data format
	  * (\c Grid) for enhancing density or contour plots. Each type
	  * of plotting has similar interface. There are 2 kind of versions which handle
	  * the arrays of data and coordinates or only single data array.
	  * Parameters of colouring are specified by the string argument
	  * (see mglGraph::SetScheme). */
	//@{
	/// Draw power crust for points in arrays \a x, \a y, \a z.
	void Crust(mglData &x, mglData &y, mglData &z, const char *sch=0,float er=0);
	/// Draw power crust for points in arrays \a tr.
	void Crust(mglData &tr, const char *sch=0,float er=0);
	/// Draw dots in points \a x, \a y, \a z.
	void Dots(mglData &x, mglData &y, mglData &z, const char *sch=0);
	/// Draw dots in points \a tr.
	void Dots(mglData &tr, const char *sch=0);
	/// Draw triangle mesh for points in arrays \a x, \a y, \a z.
	void TriPlot(mglData &nums, mglData &x, mglData &y, mglData &z, const char *sch=0);
	/// Draw triangle mesh for points in arrays \a x, \a y.
	void TriPlot(mglData &nums, mglData &x, mglData &y, const char *sch=0, float zVal=NAN);
	/// Draw grid lines for density plot of 2d data specified parametrically
	void Grid(mglData &x, mglData &y, mglData &z, const char *stl=0,float zVal=NAN);
	/// Draw grid lines for density plot of 2d data
	void Grid(mglData &a,const char *stl=0,float zVal=NAN);
	/// Draw mesh lines for 2d data specified parametrically
	void Mesh(mglData &x, mglData &y, mglData &z, const char *sch=0);
	/// Draw mesh lines for 2d data
	void Mesh(mglData &z, const char *sch=0);
	/// Draw mesh lines for 2d data specified parametrically
	void Fall(mglData &x, mglData &y, mglData &z, const char *sch=0);
	/// Draw mesh lines for 2d data
	void Fall(mglData &z, const char *sch=0);
	/// Draw belts for 2d data specified parametrically
	void Belt(mglData &x, mglData &y, mglData &z, const char *sch=0);
	/// Draw belts for 2d data
	void Belt(mglData &z, const char *sch=0);
	/// Draw surface for 2d data specified parametrically
	void Surf(mglData &x, mglData &y, mglData &z, const char *sch=0);
	/// Draw surface for 2d data
	void Surf(mglData &z, const char *sch=0);
	/// Draw density plot for surface specified parametrically
	void Dens(mglData &x, mglData &y, mglData &z, const char *sch=0,float zVal=NAN);
	/// Draw density plot for 2d data
	void Dens(mglData &z, const char *sch=0,float zVal=NAN);
	/// Draw density plot for spectra-gramm specified parametrically
	void STFA(mglData &x, mglData &y, mglData &re, mglData &im, int dn, const char *sch=0,float zVal=NAN);
	/// Draw density plot for spectra-gramm
	void STFA(mglData &re, mglData &im, int dn, const char *sch=0,float zVal=NAN);
	/// Draw vertical boxes for 2d data specified parametrically
	void Boxs(mglData &x, mglData &y, mglData &z, const char *sch=0,float zVal=NAN);
	/// Draw vertical boxes for 2d data
	void Boxs(mglData &z, const char *sch=0,float zVal=NAN);
	/// Draw vertical tiles for 2d data specified parametrically
	void Tile(mglData &x, mglData &y, mglData &z, const char *sch=0);
	/// Draw vertical tiles for 2d data
	void Tile(mglData &z, const char *sch=0);
	/// Draw vertical tiles with variable size for 2d data specified parametrically
	void Tile(mglData &x, mglData &y, mglData &z, mglData &z, const char *sch=0);
	/// Draw vertical tiles with variable size for 2d data
	void Tile(mglData &z, mglData &z, const char *sch=0);
	/// Draw contour lines for 2d data specified parametrically
	void Cont(mglData &v, mglData &x, mglData &y, mglData &z, const char *sch=0, float zVal=NAN);
	/// Draw contour lines for 2d data
	void Cont(mglData &v, mglData &z, const char *sch=0,float zVal=NAN);
	/// Draw several contour lines for 2d data specified parametrically
	void Cont(mglData &x, mglData &y, mglData &z, const char *sch=0, int Num=7, float zVal=NAN);
	/// Draw several contour lines for 2d data
	void Cont(mglData &z, const char *sch=0, int Num=7, float zVal=NAN);
	/// Draw axial-symmetric isosurfaces for 2d data specified parametrically
	void Axial(mglData &v, mglData &x, mglData &y, mglData &a, const char *sch=0);
	/// Draw axial-symmetric isosurfaces for 2d data
	void Axial(mglData &v, mglData &a, const char *sch=0);
	/// Draw several axial-symmetric isosurfaces for 2d data specified parametrically
	void Axial(mglData &x, mglData &y, mglData &a, const char *sch=0, int Num=3);
	/// Draw several axial-symmetric isosurfaces for 2d data
	void Axial(mglData &a, const char *sch=0, int Num=3);
	/// Draw solid contours for 2d data specified parametrically
	void ContF(mglData &v, mglData &x, mglData &y, mglData &z, const char *sch=0, float zVal=NAN);
	/// Draw solid contours for 2d data
	void ContF(mglData &v, mglData &z, const char *sch=0,float zVal=NAN);
	/// Draw several solid contours for 2d data specified parametrically
	void ContF(mglData &x, mglData &y, mglData &z, const char *sch=0, int Num=7, float zVal=NAN);
	/// Draw several solid contours for 2d data
	void ContF(mglData &z, const char *sch=0, int Num=7, float zVal=NAN);
	//@}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** @name Dual plotting functions
	  * These plotting functions draw a \b two (\b or \b three) \b matrix simultaneously
	  * in different forms (as coloring, vector field, flow chart or mapping).
	  * The color scheme is specified by the string parameter
	  * (see mglGraph::SetScheme). The array \a x and \a y can be vectors or
	  * matrices with the same size as main matrix. Also there is case when
	  * matrix is plotted along all square \a X*Y specified by corners
	  * mglGraph::Min -- mglGraph::Max.
	  */
	//@{

	/// Plot dew drops for vector field {ax,ay} parametrically depended on coordinate {x,y}
	void Dew(mglData &x, mglData &y, mglData &ax, mglData &ay, const char *sch=0,float zVal=NAN);
	/// Plot dew drops for vector field {ax,ay}
	void Dew(mglData &ax, mglData &ay, const char *sch=0,float zVal=NAN);
	/// Draw surface specified parametrically with coloring by other matrix
	void SurfC(mglData &x, mglData &y, mglData &z, mglData &c, const char *sch=0);
	/// Draw surface specified by matrix with coloring by other matrix
	void SurfC(mglData &z, mglData &c, const char *sch=0);
	/// Draw surface specified parametrically which transparency is determined by other matrix
	void SurfA(mglData &x, mglData &y, mglData &z, mglData &c, const char *sch=0);
	/// Draw surface specified by matrix which transparency is determined by other matrix
	void SurfA(mglData &z, mglData &c, const char *sch=0);
	/// Plot vector field {ax,ay} parametrically depended on coordinate {x,y} with length proportional to value |a|
	void Vect(mglData &x, mglData &y, mglData &ax, mglData &ay, const char *sch=0,float zVal=NAN);
	/// Plot vector field {ax,ay} with length proportional to value |a|
	void Vect(mglData &ax, mglData &ay, const char *sch=0,float zVal=NAN);
	/// Plot vector field {ax,ay} parametrically depended on coordinate {x,y} with color proportional to value |a|
	void VectC(mglData &x, mglData &y, mglData &ax, mglData &ay, const char *sch=0,float zVal=NAN);
	/// Plot vector field {ax,ay} with color proportional to value |a|
	void VectC(mglData &ax, mglData &ay, const char *sch=0,float zVal=NAN);
	/// Plot 3d vector field {ax,ay,ay} parametrically depended on coordinate {x,y,z} with length proportional to value |a|
	void Vect(mglData &x, mglData &y, mglData &z, mglData &ax, mglData &ay, mglData &az, const char *sch=0);
	/// Plot 3d vector field {ax,ay,ay} with length proportional to value |a|
	void Vect(mglData &ax, mglData &ay, mglData &az, const char *sch=0);
	/// Plot 3d vector field {ax,ay,ay} parametrically depended on coordinate {x,y,z} with color proportional to value |a|
	void VectC(mglData &x, mglData &y, mglData &z, mglData &ax, mglData &ay, mglData &az, const char *sch=0);
	/// Plot 3d vector field {ax,ay,ay} with color proportional to value |a|
	void VectC(mglData &ax, mglData &ay, mglData &az, const char *sch=0);
	/// Color map of matrix a to matrix b, both matrix parametrically depend on coordinates
	void Map(mglData &x, mglData &y, mglData &a, mglData &b, const char *sch=0, int ks=0, bool pnts=true);
	/// Color map of matrix a to matrix b
	void Map(mglData &a, mglData &b, const char *sch=0, int ks=0, bool pnts=true);
	/// Draw isosurface for 3d data \a a specified parametrically with alpha proportional to \a b
	void Surf3A(float Val, mglData &x, mglData &y, mglData &z, mglData &a, mglData &b,
				const char *stl=0);
	/// Draw isosurface for 3d data \a a with alpha proportional to \a b
	void Surf3A(float Val, mglData &a, mglData &b, const char *stl=0);
	/// Draw several isosurface for 3d data \a a specified parametrically with alpha proportional to \a b
	void Surf3A(mglData &x, mglData &y, mglData &z, mglData &a, mglData &b,
				const char *stl=0, int num=3);
	/// Draw several isosurface for 3d data \a a with alpha proportional to \a b
	void Surf3A(mglData &a, mglData &b, const char *stl=0, int num=3);
	/// Draw isosurface for 3d data \a a specified parametrically with color proportional to \a b
	void Surf3C(float Val, mglData &x, mglData &y, mglData &z, mglData &a, mglData &b,
				const char *stl=0);
	/// Draw isosurface for 3d data \a a with color proportional to \a b
	void Surf3C(float Val, mglData &a, mglData &b, const char *stl=0);
	/// Draw several isosurface for 3d data \a a specified parametrically with color proportional to \a b
	void Surf3C(mglData &x, mglData &y, mglData &z, mglData &a, mglData &b,
				const char *stl=0, int num=3);
	/// Draw several isosurface for 3d data \a a with color proportional to \a b
	void Surf3C(mglData &a, mglData &b, const char *stl=0, int num=3);
	/// Plot flows for vector field {ax,ay} parametrically depended on coordinate {x,y} with color proportional to value |a|
	void Flow(mglData &x, mglData &y, mglData &ax, mglData &ay, const char *sch=0, int num=5, bool central=true, float zVal=NAN);
	/// Plot flows for vector field {ax,ay} with color proportional to value |a|
	void Flow(mglData &ax, mglData &ay, const char *sch=0, int num=5, bool central=true, float zVal=NAN);
	/// Plot flows for 3d vector field {ax,ay,ay} parametrically depended on coordinate {x,y,z} with color proportional to value |a|
	void Flow(mglData &x, mglData &y, mglData &z, mglData &ax, mglData &ay, mglData &az, const char *sch=0, int num=3, bool central=true);
	/// Plot flows for 3d vector field {ax,ay,ay} with color proportional to value |a|
	void Flow(mglData &ax, mglData &ay, mglData &az, const char *sch=0, int num=3, bool central=true);
	/// Plot flow pipes for vector field {ax,ay} parametrically depended on coordinate {x,y} with color proportional to value |a|
	void Pipe(mglData &x, mglData &y, mglData &ax, mglData &ay, const char *sch=0, float r0=0.05, int num=5, bool central=true, float zVal=NAN);
	/// Plot flow pipes for vector field {ax,ay} with color proportional to value |a|
	void Pipe(mglData &ax, mglData &ay, const char *sch=0, float r0=0.05, int num=5, bool central=true, float zVal=NAN);
	/// Plot flow pipes for 3d vector field {ax,ay,ay} parametrically depended on coordinate {x,y,z} with color proportional to value |a|
	void Pipe(mglData &x, mglData &y, mglData &z, mglData &ax, mglData &ay, mglData &az, const char *sch=0, float r0=0.05, int num=3, bool central=true);
	/// Plot flow pipes for 3d vector field {ax,ay,ay} with color proportional to value |a|
	void Pipe(mglData &ax, mglData &ay, mglData &az, const char *sch=0, float r0=0.05, int num=3, bool central=true);
	//@}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** @name 3D plotting functions
	  * 3D plotting functions draw a 3-ranged \b tensor in different forms.
	  * There are the using of slices, isosurfaces (surfaces of constant
	  * amplitude) or volume intransparency like clouds.
	  * The color scheme is specified by the string parameter
	  * (see mglGraph::SetScheme). The array \a x, \a y and \a z can be vectors or
	  * matrixes with the same size as main matrix. Also there is case when
	  * matrix is plotted along all square \a X*Y*Z specified by corners
	  * mglGraph::Min -- mglGraph::Max.
	  */
	//@{
	/// Draw grid lines for density plot at slice for 3d data specified parametrically
	void Grid3(mglData &x, mglData &y, mglData &z, mglData &a, char dir, int sVal=-1, const char *stl=0);
	/// Draw grid lines for density plot at slice for 3d data
	void Grid3(mglData &a, char dir, int sVal=-1, const char *stl=0);
	/// Draw grid lines for density plot at central slices for 3d data specified parametrically
	void GridA(mglData &x, mglData &y, mglData &z, mglData &a, const char *stl=0);
	/// Draw grid lines for density plot at central slices for 3d data
	void GridA(mglData &a, const char *stl=0);
	/// Draw density plot at slice for 3d data specified parametrically
	void Dens3(mglData &x, mglData &y, mglData &z, mglData &a, char dir, int sVal=-1, const char *stl=0);
	/// Draw density plot at slice for 3d data
	void Dens3(mglData &a, char dir, int sVal=-1, const char *stl=0);
	/// Draw density plot at central slices for 3d data specified parametrically
	void DensA(mglData &x, mglData &y, mglData &z, mglData &a, const char *stl=0);
	/// Draw density plot at central slices for 3d data
	void DensA(mglData &a, const char *stl=0);
	/// Draw isosurface for 3d data specified parametrically
	void Surf3(float Val, mglData &x, mglData &y, mglData &z, mglData &a, const char *stl=0);
	/// Draw isosurface for 3d data
	void Surf3(float Val, mglData &a, const char *stl=0);
	/// Draw several isosurfaces for 3d data specified parametrically
	void Surf3(mglData &x, mglData &y, mglData &z, mglData &a, const char *stl=0, int num=3);
	/// Draw several isosurfaces for 3d beam in curvilinear coordinates
	void Beam(mglData &tr, mglData &g1, mglData &g2, mglData &a, float r,
		const char *stl=0, int flag=0, int num=3);
	/// Draw isosurface for 3d beam in curvilinear coordinates
	void Beam(float val,mglData &tr, mglData &g1, mglData &g2, mglData &a, float r,
		const char *stl=0, int flag=0);
	/// Draw several isosurface for 3d data
	void Surf3(mglData &a, const char *stl=0, int num=3);
	/// Draw contour lines at slice for 3d data specified parametrically
	void Cont3(mglData &v, mglData &x, mglData &y, mglData &z, mglData &a,
				char dir, int sVal=-1, const char *sch=0);
	/// Draw contour lines at slice for 3d data
	void Cont3(mglData &v, mglData &a, char dir, int sVal=-1, const char *sch=0);
	/// Draw several contour lines at slice for 3d data specified parametrically
	void Cont3(mglData &x, mglData &y, mglData &z, mglData &a,
				char dir, int sVal=-1, const char *sch=0, int Num=7);
	/// Draw several contour lines at slice for 3d data
	void Cont3(mglData &a, char dir, int sVal=-1, const char *sch=0, int Num=7);
	/// Draw contour lines at central slices for 3d data specified parametrically
	void ContA(mglData &x, mglData &y, mglData &z, mglData &a,
				const char *sch=0, int Num=7);
	/// Draw contour lines at central slices for 3d data
	void ContA(mglData &a, const char *sch=0, int Num=7);
	/// Draw solid contours at slice for 3d data specified parametrically
	void ContF3(mglData &v, mglData &x, mglData &y, mglData &z, mglData &a,
				char dir, int sVal=-1, const char *sch=0);
	/// Draw solid contours at slice for 3d data
	void ContF3(mglData &v, mglData &a, char dir, int sVal=-1, const char *sch=0);
	/// Draw several solid contours at slice for 3d data specified parametrically
	void ContF3(mglData &x, mglData &y, mglData &z, mglData &a,
				char dir, int sVal=-1, const char *sch=0, int Num=7);
	/// Draw several solid contours at slice for 3d data
	void ContF3(mglData &a, char dir, int sVal=-1, const char *sch=0, int Num=7);
	/// Draw solid contours at central slices for 3d data specified parametrically
	void ContFA(mglData &x, mglData &y, mglData &z, mglData &a,
				const char *sch=0, int Num=7);
	/// Draw solid contours at central slices for 3d data
	void ContFA(mglData &a, const char *sch=0, int Num=7);
	/// Draw a cloud of points for 3d data specified parametrically
	void CloudP(mglData &x, mglData &y, mglData &z, mglData &a,
				const char *stl=0, float alpha=1);
	/// Draw a cloud of points for 3d data
	void CloudP(mglData &a, const char *stl=0, float alpha=1);
	/// Draw a semi-transparent cloud for 3d data specified parametrically
	void CloudQ(mglData &x, mglData &y, mglData &z, mglData &a,
				const char *stl=0, float alpha=1);
	/// Draw a semi-transparent cloud for 3d data
	void CloudQ(mglData &a, const char *stl=0, float alpha=1);
	//@}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** @name Combined plotting functions
	  * These plotting functions draw density plot or contour lines in x, y, or z plain.
	  * If \a a is a tensor (3-dimensional data) then interpolation to a given
	  * sVal is performed. */
	//@{
	/// Draw density plot for data a at x = sVal
	void DensX(mglData &a, const char *stl=0, float sVal=NAN);
	/// Draw density plot for data a at y = sVal
	void DensY(mglData &a, const char *stl=0, float sVal=NAN);
	/// Draw density plot for data a at z = sVal
	void DensZ(mglData &a, const char *stl=0, float sVal=NAN);
	/// Draw several contour plots for data a at x = sVal
	void ContX(mglData &a, const char *stl=0, float sVal=NAN, int Num=7);
	/// Draw several contour plots for data a at y = sVal
	void ContY(mglData &a, const char *stl=0, float sVal=NAN, int Num=7);
	/// Draw several contour plots for data a at z = sVal
	void ContZ(mglData &a, const char *stl=0, float sVal=NAN, int Num=7);
	/// Draw contour plots for data a at x = sVal
	void ContX(mglData &v, mglData &a, const char *stl=0, float sVal=NAN);
	/// Draw contour plots for data a at y = sVal
	void ContY(mglData &v, mglData &a, const char *stl=0, float sVal=NAN);
	/// Draw contour plots for data a at z = sVal
	void ContZ(mglData &v, mglData &a, const char *stl=0, float sVal=NAN);
	/// Draw several contour plots for data a at x = sVal
	void ContFX(mglData &a, const char *stl=0, float sVal=NAN, int Num=7);
	/// Draw several contour plots for data a at y = sVal
	void ContFY(mglData &a, const char *stl=0, float sVal=NAN, int Num=7);
	/// Draw several contour plots for data a at z = sVal
	void ContFZ(mglData &a, const char *stl=0, float sVal=NAN, int Num=7);
	/// Draw contour plots for data a at x = sVal
	void ContFX(mglData &v, mglData &a, const char *stl=0, float sVal=NAN);
	/// Draw contour plots for data a at y = sVal
	void ContFY(mglData &v, mglData &a, const char *stl=0, float sVal=NAN);
	/// Draw contour plots for data a at z = sVal
	void ContFZ(mglData &v, mglData &a, const char *stl=0, float sVal=NAN);
	//@}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
protected:
	bool TernAxis;				/// Flag that Ternary axis is used
	float FogDist;				/// Inverse fog distance (fog ~ exp(-FogDist*Z))
	float FogDz;				/// Relative shift of fog
	int _sx,_sy,_sz;			// sign in shift of axis ticks and labels
	char Arrow1, Arrow2;		// Style of arrows at end and at start of curve
	float AmbBr;				///< Default ambient light brightness
	mglFont *fnt;				///< Class for printing vector text
	float font_factor;
	bool UseAlpha;				///< Flag that Alpha is used
	mglColor cmap[NUM_COLOR];	///< Colors for color scheme
	int CurrPal;				///< Current index of palette mglGraph::Pal
	int NumCol;					///< Actual number of colors in color scheme mglGraph::cmap
	float CloudFactor;			///< Factor of transparency in mglGraph::CloudP and mglGraph::CloudQ
	bool ScalePuts;				///< Enable/disable point positions scaling in puts

	int NumLeg;					///< Number of used positions in LegStr and LegStl arrays
	wchar_t *LegStr[100];			///< String array with legend text (see mglGraph::AddLegend)
	char *LegStl[100];			///< String array with legend style (see mglGraph::AddLegend)
	float zoomx1, zoomy1, zoomx2, zoomy2;

	/// Flag which allow to use color scheme along axis
	bool OnCoord;
	int CurFrameId;	///< Number of automaticle created frames

	static mgl_save JPEGSave;	///< Function for saving in JPEG
	static mgl_save TIFFSave;	///< Function for saving in TIFF
	static mgl_save PNGASave;	///< Function for saving in PNG with transparency
	static mgl_save PNGSave;	///< Function for saving in PNG
	static mgl_save BMPSave;	///< Function for saving in BMP
	static int NumUseSave;		///< Number of instances that use mgl_save functions
	/// Get RGB(A) lines for saving in file
	virtual unsigned char **GetRGBLines(long &width, long &height, unsigned char *&f, bool alpha=false);

	/// Get Org.x (parse NAN value)
	virtual float GetOrgX();
	/// Get Org.y (parse NAN value)
	virtual float GetOrgY();
	/// Get Org.z (parse NAN value)
	virtual float GetOrgZ();
	/// Set warning code ant fill Message
	void SetWarn(int code, const char *who="");
	/// Set the parameter lines directly (internaly used by mglGraph::SelectPen)
	virtual void Pen(mglColor col, char style,float width);	//=0
	/// Set the default color
	virtual void DefColor(mglColor c, float alpha=-1)=0;
	/// draw mark with different type at position {x,y,z} (no scaling)
	virtual void Mark(float x,float y,float z,char mark='.')=0;

	/// Draws the point (ball) at position \a p with color \a c.
	virtual void ball(float *p,float *c);
	/// Plot tube between consequently connected lines with color and radius varing
	virtual void tube_plot(long n,float *pp,float *cc,float *rr);
	/// Plot series of consequently connected lines
	virtual void curv_plot(long n,float *pp,bool *tt)=0;
	/// Plot series of consequently connected lines with color varing
	virtual void curv_plot(long n,float *pp,float *cc,bool *tt)=0;
	/// Plot series of arbitrary connected lines.
	virtual void curv_plot(long n,float *pp,bool *tt,long *nn)=0;
	/// Mesh plot depending on positions and colors of vertexes on grid
	virtual void mesh_plot(long n,long m,float *pp,float *cc,bool *tt, int how)=0;
	/// Surface plot depending on positions and colors of vertexes on grid
	virtual void surf_plot(long n,long m,float *pp,float *cc,bool *tt)=0;
	/// Axial-symmetric isosurface based on contour line
	virtual void axial_plot(long n,float *pp,long *nn,long np,bool wire)=0;
	/// Cloud plot depending on positions and alpha of vertexes on 3D grid
	virtual void cloud_plot(long nx,long ny,long nz,float *pp,float *a,float alpha)=0;
	/// Boxs plot depending on positions and colors of vertexes on grid
	virtual void boxs_plot(long n, long m, float *pp, mglColor *cc, bool *tt,
						float Alpha, bool line)=0;
	/// Plot isosurface depending on positions and colors of vertexes on 3D grid
	virtual void surf3_plot(long n,long m,long *kx1,long *kx2,long *ky1,long *ky2,long *kz,
							float *pp,float *cc,float *kk,float *nn,bool wire)=0;
	/// Plot quads depending on positions and colors of vertexes on grid
	virtual void quads_plot(long n, float *pp, float *cc, bool *tt)=0;
	/// Plot quads depending on positions and colors of vertexes on grid
	virtual void trigs_plot(long n, long *nn, long m, float *pp, float *cc, bool *tt,bool wire)=0;
	/// Plot series of unconnected lines.
	virtual void lines_plot(long n, float *pp, float *cc, bool *tt)=0;

	/// Scale coordinates of point for faster plotting also cut off some points
	virtual bool ScalePoint(float &x,float &y,float &z);
	/// fast linear interpolation
	inline float _d(float v,float v1,float v2) { return (v-v1)/(v2-v1); };

	/// Get color depending on single variable \a z, which should be scaled if \a scale=true
	mglColor GetC(float z,bool scale = true);
	/// Get alpha value depending on single variable \a a
	float GetA(float a);
	/// Get color depending on three coordinates \a x,\a y,\a z
	mglColor GetC(float x,float y,float z,bool simple=false);
	/// add point to contour line chain
	long add_cpoint(long &pc,float **p,float **k,bool **t,float x,float y,float z,
				float k1,float k2,bool scale);

private:
	char last_style[64];
	float _tetx,_tety,_tetz;
	/// Actual lower edge of bounding box after applying transformation formulas.
	mglPoint FMin;
	/// Actual upper edge of bounding box after applying transformation formulas.
	mglPoint FMax;

	static void *jmodule;		///< Module for JPEG function
	static void *tmodule;		///< Module for TIFF function
	void InitSaveFunc();		///< Load modules for JPEG, TIFF functions (first times)
	void FreeSaveFunc();		///< Free modules for JPEG, TIFF functions (last times)
	void ClearEq();				///< Clear the used variables for axis transformation
	mglColor GetC2(float x,float y);

	/// Print curved text
	void font_curve(long n,float *pp,bool *tt,long *nn,const wchar_t *text,
					int pos,float size);
	void string_curve(long f,long n,float *pp,long *nn,const wchar_t *text,
					float size, int pos);

	/// Contour plot depending on positions and colors of vertexes on grid
	void cont_plot(float val,long n,long m,float *a,float *x,float *y,float *z,
				float zdef,bool axial,bool wire,int text);
	/// Contour plot depending on positions and colors of vertexes on grid
	void contf_plot(float v1,float v2,long n,long m,float *a,float *x,float *y,float *z,
				float zdef);
	/// make single flow thread for 2D case
	void flow(bool simple, float zVal, float u, float v,
				mglData &x, mglData &y, mglData &ax, mglData &ay);
	/// make single flow thread for 3D case
	void flow(bool simple, float u, float v, float w,
				mglData &x, mglData &y, mglData &z,
				mglData &ax, mglData &ay, mglData &az);
	/// make single flow tube for 2D case
	void flowr(bool simple, float zVal, float u, float v,
				mglData &x, mglData &y, mglData &ax, mglData &ay, float r0);
	/// make single flow tube for 3D case
	void flowr(bool simple, float u, float v, float w,
				mglData &x, mglData &y, mglData &z,
				mglData &ax, mglData &ay, mglData &az, float r0);
	/// Put alpha valued point for Cloud like plot
	void AVertex(float x,float y,float z, float a,float alpha);
	/// add point to isosurface chain
	long add_spoint(long &pc,float **p,float **k,float **c,float **n,
			float x,float y,float z,float nx,float ny,float nz,
			float k1,float k2,float k3,float a);

	/// Set internal boundng box depending on transformation formula
	void SetFBord(float x,float y,float z);
	/// Set color depending on it value
	void Color(float a,float a1=0,float a2=0);	// ���� �����
	/// Draw x axis
	void AxisX(bool text);
	/// Draw y axis
	void AxisY(bool text);
	/// Draw z axis
	void AxisZ(bool text);
	/// Draw y axis for Ternary plot
	void AxisTY(bool text);
	/// Draw z axis for Ternary plot
	void AxisTZ(bool text);
	/// Draw ticks on box
	void TickBox();
	/// Draw tick
	void DrawTick(float *pp,bool sub);
	/// Execute commands a*
	int exec_a(const char *com, long n, mglArg *a,int k[9]);
	/// Execute commands b*
	int exec_b(const char *com, long n, mglArg *a,int k[9]);
	/// Execute commands c*
	int exec_c(const char *com, long n, mglArg *a,int k[9]);
	/// Execute commands co*
	int exec_co(const char *com, long n, mglArg *a,int k[9]);
	/// Execute commands d*
	int exec_d(const char *com, long n, mglArg *a,int k[9]);
	/// Execute commands f*
	int exec_f(const char *com, long n, mglArg *a,int k[9]);
	/// Execute commands g*
	int exec_g(const char *com, long n, mglArg *a,int k[9]);
	/// Execute commands l*
	int exec_l(const char *com, long n, mglArg *a,int k[9]);
	/// Execute commands m*
	int exec_m(const char *com, long n, mglArg *a,int k[9]);
	/// Execute commands r*
	int exec_r(const char *com, long n, mglArg *a,int k[9]);
	/// Execute commands s*
	int exec_s(const char *com, long n, mglArg *a,int k[9]);
	/// Execute commands su*
	int exec_su(const char *com, long n, mglArg *a,int k[9]);
	/// Execute commands t*
	int exec_t(const char *com, long n, mglArg *a,int k[9]);
	/// Execute commands [vxyz]*
	int exec_vz(const char *com, long n, mglArg *a,int k[9]);
	/// Execute commands [eih]*
	int exec_misc1(const char *com, long n, mglArg *a,int k[9]);
	/// Execute commands [nop]*
	int exec_misc2(const char *com, long n, mglArg *a,int k[9]);
};
//-----------------------------------------------------------------------------
/// Structure for the mglData handling (see mglParse class).
struct mglVar
{
	mglData d;		///< Data itself
	char s[64];		///< Data name
	void *o;		///< Pointer to external object
	mglVar *next;	///< Pointer to next instance in list
	mglVar *prev;	///< Pointer to prev instance in list
	bool temp;		///< This temporar variable
	void (*func)(void *);	///< Callback function for destroying

	mglVar()	{	o=0;	s[0]=0;		next=prev=0;	func=0;	temp=false;	};
	~mglVar();
	/// Move variable after \a var and copy \a func from \a var (if \a func is 0)
	void MoveAfter(mglVar *var);
};
//-----------------------------------------------------------------------------
/// Structure for the command argument (see mglGraph::Exec()).
class mglParse
{
friend void mgl_export(char *out, const char *in, int type);
public:
	mglVar *DataList;	///< List with data and its names
	bool AllowSetSize;	///< Allow using setsize command
	bool Stop;			///< Stop command was. Flag prevent further execution

	mglParse(bool setsize=false);
	~mglParse();
	/// Parse and execute the string of MGL script
	int Parse(mglGraph *gr, const char *str);
	/// Execute MGL script file \a fname
	void Execute(mglGraph *gr, const char *fname);
	/// Find variable or return 0 if absent
	mglVar *FindVar(const char *name);
	/// Find variable or create it if absent
	mglVar *AddVar(const char *name);
	/// Add string for parameter $1, ..., $9
	bool AddParam(int n, const char *str);
	/// Restore Once flag
	inline void RestoreOnce()	{	Once = true;	};
private:
	long parlen;	///< Length of parameter strings
	char *par[10];	///< Parameter for substituting instead of $1, ..., $9
	char leg[128];	///< Buffer for legend
	bool opt[16];	///< Set on/off optional parameters for command argument
	float val[20];	///< Values for optional parameters
	bool Once;		///< Flag for command which should be executed only once
	bool Skip;		///< Flag that commands should be skiped

	/// Fill arguments \a a from strings
	void FillArg(int n, char **arg, mglArg *a);
	/// PreExecute stage -- parse some commands and create variables
	int PreExec(mglGraph *gr, long n, char **arg, mglArg *a);
	/// Process optional arguments
	void ProcOpt(mglGraph *gr, char *str);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
