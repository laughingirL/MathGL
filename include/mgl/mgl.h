/* mgl.h is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <mathgl.abalakin@gmail.com>
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

#define MGL_VERSION	7.1

#include "mgl/mgl_data.h"
#include "mgl/mgl_font.h"

#ifndef NUM_COLOR
#define NUM_COLOR	32
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
	void Set(mglColor c, float bright=1);
	/// Check if color is valid
	inline bool Valid()
	{	return (r>=0 && r<=1 && g>=0 && g<=1 && b>=0 && b<=1);	};
	/// Get maximal spectral component
	inline float Norm()
	{	return r>g ? r : (g>b ? g : b);	};
	/// Set color from symbolic id
	void Set(char p, float bright=1);
	/// Copy color from other one
	bool operator==(const mglColor &c)
	{	return (r==c.r && g==c.g && b==c.b);	};
};
inline mglColor operator+(const mglColor &a, const mglColor &b)
{	return mglColor(a.r+b.r, a.g+b.g, a.b+b.b);	};
inline mglColor operator-(const mglColor &a, const mglColor &b)
{	return mglColor(a.r-b.r, a.g-b.g, a.b-b.b);	};
inline mglColor operator*(const mglColor &a, float b)
{	return mglColor(a.r*b, a.g*b, a.b*b);	};
inline mglColor operator*(float b, const mglColor &a)
{	return mglColor(a.r*b, a.g*b, a.b*b);	};
inline mglColor operator/(const mglColor &a, float b)
{	return mglColor(a.r/b, a.g/b, a.b/b);	};
inline mglColor operator!(const mglColor &a)
{	return mglColor(1-a.r, 1-a.g, 1-a.b);	}
//-----------------------------------------------------------------------------
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
float GetX(const mglData &x, int i, int j, int k);
float GetY(const mglData &y, int i, int j, int k);
float GetZ(const mglData &z, int i, int j, int k);
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
	mglPoint OrgT;		///< Point of starting ticks numbering (if NAN then Org is used).
	bool AutoOrg;		///< Shift Org automatically if it lye outside Min ... Max range
	mglColor Pal[101];	///< Color palette for 1D plotting.
	int NumPal;			///< Number of colors in palette.
	char FontDef[32];	///< Font specification (see mglGraph::Puts). Default is Roman with align at center.
	bool RotatedText;	///< Use text rotation along axis
	const char *PlotId;	///< Id of plot for saving filename (in GLUT window for example)
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
	float PlotFactor;	///< Factor for sizing overall plot (should be >1.6, default is =2)
	bool AutoPlotFactor;///< Enable autochange PlotFactor

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
	float AlphaDef;		///< Default value of alpha channel (transparency) for all plotting functions.
	char *Message;		///< Buffer for receiving messages
	int WarnCode;		///< Warning code
	bool TuneTicks;		///< Draw tuned ticks with extracted common component
	float FactorPos;	///< Position of axis ticks factor (0 at Min, 1 at Max, 1.1 is default)
	int CirclePnts;		///< Number of points for a circle drawing (used in Tube(), Drop(), Sphere(), Cone())
	int FitPnts;		///< Number of output points in fitting

	mglGraph();			///< Set default parameter for plotting
	virtual ~mglGraph();///< Clear the used variables
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
	/// Write the frame in file using IDTF format
	virtual void WriteIDTF(const char *fname,const char *descr=0);
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
	/// Set palette
	void SetPalette(const char *colors);
	/// Set colormap scheme for surfaces (usualy called internaly)
	void SetScheme(const char *sch);
	/// Set the parameter of line (usualy called internaly)
	char SelectPen(const char *pen);
	/// Set the ticks parameters
	void SetTicks(char dir, float d=-5, int ns=0, float org=NAN);
	/// Set warning code ant fill Message
	void SetWarn(int code, const char *who="");
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
	/// Restore transformation matrix after last InPlot()
	virtual void RestoreM()=0;
	/// Clear up the frame
	virtual void Clf(mglColor Back=WC); //=0
	/// Put further plotting in some region of whole frame surface.
	void SubPlot(int nx,int ny,int m, float dx=0, float dy=0);
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
	void CRange(const mglData &a, bool add = false);
	/// Safety set values of mglGraph::Min.x and mglGraph::Max.x as minimal and maximal values of data a
	void XRange(const mglData &a, bool add = false);
	/// Safety set values of mglGraph::Min.x and mglGraph::Max.x as minimal and maximal values of data a
	void YRange(const mglData &a, bool add = false);
	/// Safety set values of mglGraph::Min.x and mglGraph::Max.x as minimal and maximal values of data a
	void ZRange(const mglData &a, bool add = false);

	/// Safetly set the values of mglGraph::Cmin and mglGraph::Cmax
	void inline CAxis(float C1,float C2)	{	Cmin=C1;	Cmax=C2;	};
	/// Safetly set the value for mglGraph::Min, mglGraph::Max and mglGraph::Org members of the class.
	void Axis(mglPoint Min, mglPoint Max, mglPoint Org=mglPoint(NAN,NAN,NAN));
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
	void FaceX(float x0, float y0, float z0, float wy, float wz, const char *stl="w", float dx=0, float dy=0);
	void FaceY(float x0, float y0, float z0, float wx, float wz, const char *stl="w", float dx=0, float dy=0);
	void FaceZ(float x0, float y0, float z0, float wx, float wy, const char *stl="w", float dx=0, float dy=0);
	/// Draws the sphere at point \a p with color \a stl and radius \a r.
	void Sphere(mglPoint p, float r, const char *stl="r");
	/// Draws the ellipsoid at point \a p with color \a stl and radiuses \a r.
	void Ellipse(mglPoint p, mglPoint r, const char *stl="r");
	/// Draws the drop at point \a p in direction \a q with color \a stl and radius \a r.
	void Drop(mglPoint p, mglPoint q, float r, const char *stl="r", float shift=1, float ap=1);
	/// Draws the drop at point \a p in direction \a q with color \a stl and radius \a r.
	void Drop(mglPoint p, mglPoint q, float r, mglColor c, float shift=1, float ap=1);
	/// Draws the line between points with style \a stl.
	void Cone(mglPoint p1, mglPoint p2, float r1, float r2=-1, const char *stl="B", bool edge=false);
	/// draw mark with different type at position {x,y,z} (no scaling)
	void Mark(mglPoint p,char mark='.');
	/// Draw a set of triangles (or lines if trig==NULL) for glyph from point (0,0). Normally this function is used internally.
	virtual void Glyph(float x,float y, float f,int nt, const short *trig, int nl, const short *line)=0;
	//@}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** @name Fitting functions
	  * These functions fit data to formula (find formula parameters for best fitting the data points). This functions do not draw obtained curve itself.*/
	//@{
	/// Fit data along x-direction for each data row. Data 'fit' will contain values for found formula.
	float Fit(mglData &fit, const mglData &y, const char *eq, const char *var, float *ini=0, bool print=false);
	float Fit(mglData &fit, const mglData &y, const char *eq, const char *var, mglData &ini, bool print=false);
	/// Fit data along x-,y-directions for each data slice. Data 'fit' will contain values for found formula.
	float Fit2(mglData &fit, const mglData &z, const char *eq, const char *var, float *ini=0, bool print=false);
	float Fit2(mglData &fit, const mglData &z, const char *eq, const char *var, mglData &ini, bool print=false);
	/// Fit data along all directions. Data 'fit' will contain values for found formula.
	float Fit3(mglData &fit, const mglData &a, const char *eq, const char *var, float *ini=0, bool print=false);
	float Fit3(mglData &fit, const mglData &a, const char *eq, const char *var, mglData &ini, bool print=false);
	/// Fit data along x-direction for each data row. Data 'fit' will contain values for found formula.
	float Fit(mglData &fit, const mglData &x, const mglData &y, const char *eq, const char *var, float *ini=0, bool print=false);
	float Fit(mglData &fit, const mglData &x, const mglData &y, const char *eq, const char *var, mglData &ini, bool print=false);
	/// Fit data along x-,y-directions for each data slice. Data 'fit' will contain values for found formula.
	float Fit(mglData &fit, const mglData &x, const mglData &y, const mglData &z, const char *eq, const char *var, float *ini=0, bool print=false);
	float Fit(mglData &fit, const mglData &x, const mglData &y, const mglData &z, const char *eq, const char *var, mglData &ini, bool print=false);
	/// Fit data along all directions. Data 'fit' will contain values for found formula.
	float Fit(mglData &fit, const mglData &x, const mglData &y, const mglData &z, const mglData &a, const char *eq, const char *var, float *ini=0, bool print=false);
	float Fit(mglData &fit, const mglData &x, const mglData &y, const mglData &z, const mglData &a, const char *eq, const char *var, mglData &ini, bool print=false);
	/// Fit data with dispersion s along x-direction for each data row. Data 'fit' will contain values for found formula.
	float FitS(mglData &fit, const mglData &y, const mglData &s, const char *eq, const char *var, float *ini=0, bool print=false);
	float FitS(mglData &fit, const mglData &y, const mglData &s, const char *eq, const char *var, mglData &ini, bool print=false);
	/// Fit data with dispersion s along x-direction for each data row. Data 'fit' will contain values for found formula.
	float FitS(mglData &fit, const mglData &x, const mglData &y, const mglData &s, const char *eq, const char *var, float *ini=0, bool print=false);
	float FitS(mglData &fit, const mglData &x, const mglData &y, const mglData &s, const char *eq, const char *var, mglData &ini, bool print=false);
	/// Fit data with dispersion s along x-,y-directions for each data slice. Data 'fit' will contain values for found formula.
	float FitS(mglData &fit, const mglData &x, const mglData &y, const mglData &z, const mglData &s, const char *eq, const char *var, float *ini=0, bool print=false);
	float FitS(mglData &fit, const mglData &x, const mglData &y, const mglData &z, const mglData &s, const char *eq, const char *var, mglData &ini, bool print=false);
	/// Fit data with dispersion s along all directions. Data 'fit' will contain values for found formula.
	float FitS(mglData &fit, const mglData &x, const mglData &y, const mglData &z, const mglData &a, const mglData &s, const char *eq, const char *var, float *ini=0, bool print=false);
	float FitS(mglData &fit, const mglData &x, const mglData &y, const mglData &z, const mglData &a, const mglData &s, const char *eq, const char *var, mglData &ini, bool print=false);
	/// Print fitted last formula (with coefficients)
	void PutsFit(mglPoint p, const char *prefix=0, const char *font=0, float size=-1);
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
	/// Print the title text for the picture
	void Title(const wchar_t *text,const char *font=0,float size=-2);
	void Title(const char *text,const char *font=0,float size=-2);
	/// Print formated output in position \a p.
	void Printf(mglPoint p,const char *arg,...);
	/// Print string \a str along curve with font size \a size.
	void Text(const mglData &y,const char *text,const char *font=0,float size=-1,float zVal=NAN);
	/// Print string \a str along parametrical curve with font size \a size.
	void Text(const mglData &x,const mglData &y,const char *text,const char *font=0,float size=-1,float zVal=NAN);
	/// Print string \a str along curve in 3D with font size \a size.
	void Text(const mglData &x,const mglData &y,const mglData &z,const char *text,const char *font=0,float size=-1);
	/// Print string \a str along curve with font size \a size.
	void Text(const mglData &y,const wchar_t *text,const char *font=0,float size=-1,float zVal=NAN);
	/// Print string \a str along parametrical curve with font size \a size.
	void Text(const mglData &x,const mglData &y,const wchar_t *text,const char *font=0,float size=-1,float zVal=NAN);
	/// Print string \a str along curve in 3D with font size \a size.
	void Text(const mglData &x,const mglData &y,const mglData &z,const wchar_t *text,const char *font=0,float size=-1);

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
	void Legend(float x, float y, const char *font="rL", float size=-0.8, float llen=0.1);
	/// Draw legend of accumulated strings by \a font with \a size
	void Legend(int where=0x3, const char *font="rL", float size=-0.8, float llen=0.1);
	/// Draw legend strings \a text at position (x, y) by \a font with \a size
	void Legend(int n, wchar_t **text, char **style, float x, float y, const char *font="rL", float size=-0.8, float llen=0.1);
	/// Draw legend of accumulated strings by \a font with \a size
	void Legend(int n, wchar_t **text, char **style, int where=0x3, const char *font="rL", float size=-0.8, float llen=0.1);
	//@}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ ������� ~~~~~~~~~~~~~~~~~~~~~~~~
	/// Plot data depending on its dimensions and \a type parameter
	void SimplePlot(const mglData &a, int type=0, const char *stl=0);
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** @name 1D plotting functions
	  * These functions perform plotting of 1D data. 1D means that data
	  * depended from only 1 parameter like parametric \b curve {x(i),y(i),z(i)},
	  * i=1...n. There are 5 generally different types of data representations:
	  * simple line plot (\c Plot), line plot with filling under it (\c Area),
	  * stairs plot (\c Step), bar plot (\c Bars,\c Barh) and vertical lines (\c Stem).
	  * Each type of plotting has similar interface. There are 3D version and
	  * two 2D versions. One of last requires only one array.
	  * The parameters of line and marks are specified by the string argument
	  * (see mglGraph::SelectPen). If the string parameter is NULL
	  * then solid line with color from palette is used. */
	//@{
	/// Draw curve for formula with x in range [Min.x, Max.x]
	void Plot(const char *eqY, const char *pen=0, float zVal=NAN, int n=100);
	/// Draw curve for formulas parametrically depended on t in range [0,1]
	void Plot(const char *eqX, const char *eqY, const char *eqZ, const char *pen=0, int n=100);

	/// Draw line plot for points in arrays \a x, \a y, \a z.
	void Plot(const mglData &x, const mglData &y, const mglData &z, const char *pen=0);
	/// Draw line plot for points in arrays \a x, \a y.
	void Plot(const mglData &x, const mglData &y, const char *pen=0,float zVal=NAN);
	/// Draw line plot for points in arrays \a y.
	void Plot(const mglData &y, const char *pen=0,float zVal=NAN);
	/// Draw line plot for points in arrays \a a(0,:),\a a(1,:).
	void Plot2(const mglData &a, const char *pen=0,float zVal=NAN);
	/// Draw line plot for points in arrays \a a(0,:),\a a(1,:),\a a(2,:).
	void Plot3(const mglData &a, const char *pen=0);

	/// Fill area between curves y1 and y2 (if inside=false) or area for y1<=y<=y2 (if inside=true).
	void Region(const mglData &y1, const mglData &y2, const char *pen=0, float zVal=NAN, bool inside=true);
	/// Fill area between curves y1 and y2 (if inside=false) or area for y1<=y<=y2 (if inside=true) for parametrically specified points.
	void Region(const mglData &x, const mglData &y1, const mglData &y2, const char *pen=0, float zVal=NAN, bool inside=true);

	/// Draw area plot for points in arrays \a x, \a y, \a z.
	void Area(const mglData &x, const mglData &y, const mglData &z, const char *pen=0);
	/// Draw area plot for points in arrays \a x, \a y.
	void Area(const mglData &x, const mglData &y, const char *pen=0,bool sum=false,float zVal=NAN);
	/// Draw area plot for points in arrays \a y.
	void Area(const mglData &y, const char *pen=0,bool sum=false,float zVal=NAN);
	/// Draw area plot for points in arrays \a a(0,:),\a a(1,:).
	void Area2(const mglData &a, const char *pen=0,float zVal=NAN);
	/// Draw area plot for points in arrays \a a(0,:),\a a(1,:),\a a(2,:).
	void Area3(const mglData &a, const char *pen=0);

	/// Draw vertical lines from points in arrays \a x, \a y, \a z to mglGraph::Org.
	void Stem(const mglData &x, const mglData &y, const mglData &z, const char *pen=0);
	/// Draw vertical lines from points in arrays \a x, \a y to mglGraph::Org.
	void Stem(const mglData &x, const mglData &y, const char *pen=0,float zVal=NAN);
	/// Draw vertical lines from points in arrays \a y to mglGraph::Org.
	void Stem(const mglData &y, const char *pen=0,float zVal=NAN);
	/// Draw vertical lines from points in arrays \a a(0,:),\a a(1,:) to mglGraph::Org.
	void Stem2(const mglData &a, const char *pen=0,float zVal=NAN);
	/// Draw vertical lines from points in arrays \a a(0,:),\a a(1,:),\a a(2,:) to mglGraph::Org.
	void Stem3(const mglData &a, const char *pen=0);

	/// Draw stairs for points in arrays \a x, \a y, \a z.
	void Step(const mglData &x, const mglData &y, const mglData &z, const char *pen=0);
	/// Draw stairs for points in arrays \a x, \a y.
	void Step(const mglData &x, const mglData &y, const char *pen=0,float zVal=NAN);
	/// Draw line plot for points in arrays \a y.
	void Step(const mglData &y, const char *pen=0,float zVal=NAN);
	/// Draw stairs for points in arrays \a a(0,:),\a a(1,:).
	void Step2(const mglData &a, const char *pen=0,float zVal=NAN);
	/// Draw stairs for points in arrays \a a(0,:),\a a(1,:),\a a(2,:).
	void Step3(const mglData &a, const char *pen=0);

	/// Draw vertical bars from points in arrays \a x, \a y, \a z to mglGraph::Org.
	void Bars(const mglData &x, const mglData &y, const mglData &z, const char *pen=0, bool above=false);
	/// Draw vertical bars from points in arrays \a x, \a y to mglGraph::Org.
	void Bars(const mglData &x, const mglData &y, const char *pen=0,float zVal=NAN, bool above=false);
	/// Draw vertical bars from points in arrays \a y to mglGraph::Org.
	void Bars(const mglData &y, const char *pen=0,float zVal=NAN, bool above=false);
	/// Draw vertical bars from points in arrays \a a(0,:),\a a(1,:) to mglGraph::Org.
	void Bars2(const mglData &a, const char *pen=0,float zVal=NAN, bool above=false);
	/// Draw vertical bars from points in arrays \a a(0,:),\a a(1,:),\a a(2,:) to mglGraph::Org.
	void Bars3(const mglData &a, const char *pen=0, bool above=false);

	/// Draw vertical bars from points in arrays \a x, \a y to mglGraph::Org.
	void Barh(const mglData &y, const mglData &v, const char *pen=0,float zVal=NAN, bool above=false);
	/// Draw vertical bars from points in arrays \a y to mglGraph::Org.
	void Barh(const mglData &v, const char *pen=0,float zVal=NAN, bool above=false);

	/// Draw surface of curve {\a r,\a z} rotatation around Z axis
	void Torus(const mglData &r, const mglData &z, const char *pen=0);
	/// Draw surface of curve rotatation around Z axis
	void Torus(const mglData &z, const char *pen=0);
	/// Draw surface of curve {\a a(0,:),\a a(1,:)} rotatation around Z axis for
	void Torus2(const mglData &a, const char *pen=0);

	/// Draw chart for data a
	void Chart(const mglData &a, const char *col=0);

	/// Draw error boxes ey for data y
	void Error(const mglData &y, const mglData &ey, const char *pen=0,float zVal=NAN);
	/// Draw error boxes ey for data {x,y}
	void Error(const mglData &x, const mglData &y, const mglData &ey, const char *pen=0,float zVal=NAN);
	/// Draw error boxes {ex,ey} for data {x,y}
	void Error(const mglData &x, const mglData &y, const mglData &ex, const mglData &ey, const char *pen=0,float zVal=NAN);

	/// Draw marks with diffenernt sizes \a r for points in arrays \a x, \a y, \a z.
	void Mark(const mglData &x, const mglData &y, const mglData &z, const mglData &r, const char *pen=0);
	/// Draw marks with diffenernt sizes \a r for points in arrays \a x, \a y.
	void Mark(const mglData &x, const mglData &y, const mglData &r, const char *pen=0,float zVal=NAN);
	/// Draw marks with diffenernt sizes \a r for points in arrays \a y.
	void Mark(const mglData &y, const mglData &r, const char *pen=0,float zVal=NAN);

	/// Draw textual marks with diffenernt sizes \a r for points in arrays \a x, \a y, \a z.
	void TextMark(const mglData &x, const mglData &y, const mglData &z, const mglData &r, const char *text, const char *fnt=0);
	/// Draw textual marks with diffenernt sizes \a r for points in arrays \a x, \a y.
	void TextMark(const mglData &x, const mglData &y, const mglData &r, const char *text, const char *fnt=0,float zVal=NAN);
	/// Draw textual marks with diffenernt sizes \a r for points in arrays \a y.
	void TextMark(const mglData &y, const mglData &r, const char *text, const char *fnt=0,float zVal=NAN);
	/// Draw textual marks with diffenernt sizes \a r for points in arrays \a x, \a y, \a z.
	void TextMark(const mglData &x, const mglData &y, const mglData &z, const mglData &r, const wchar_t *text, const char *fnt=0);
	/// Draw textual marks with diffenernt sizes \a r for points in arrays \a x, \a y.
	void TextMark(const mglData &x, const mglData &y, const mglData &r, const wchar_t *text, const char *fnt=0,float zVal=NAN);
	/// Draw textual marks with diffenernt sizes \a r for points in arrays \a y.
	void TextMark(const mglData &y, const mglData &r, const wchar_t *text, const char *fnt=0,float zVal=NAN);
	/// Draw textual marks with diffenernt sizes \a r for points in arrays \a y.
	void TextMark(const mglData &y, const char *text, const char *fnt=0,float zVal=NAN);
	/// Draw textual marks with diffenernt sizes \a r for points in arrays \a y.
	void TextMark(const mglData &y, const wchar_t *text, const char *fnt=0,float zVal=NAN);

	/// Draw tube with radial sizes \a r for points in arrays \a x, \a y, \a z.
	void Tube(const mglData &x, const mglData &y, const mglData &z, const mglData &r, const char *pen=0);
	/// Draw tube with radial sizes \a r for points in arrays \a x, \a y.
	void Tube(const mglData &x, const mglData &y, const mglData &r, const char *pen=0,float zVal=NAN);
	/// Draw tube with radial sizes \a r for points in arrays \a y.
	void Tube(const mglData &y, const mglData &r, const char *pen=0,float zVal=NAN);
	/// Draw tube with constant radial sizes \a r for points in arrays \a x, \a y, \a z.
	void Tube(const mglData &x, const mglData &y, const mglData &z, float r, const char *pen=0);
	/// Draw tube with constant radial sizes \a r for points in arrays \a x, \a y.
	void Tube(const mglData &x, const mglData &y, float r, const char *pen=0,float zVal=NAN);
	/// Draw tube with constant radial sizes \a r for points in arrays \a y.
	void Tube(const mglData &y, float r, const char *pen=0,float zVal=NAN);

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
	/// Draw curve for formula with x,y in range [Min, Max]
	void Surf(const char *eqZ, const char *sch=0, int n=100);
	/// Draw curve for formulas parametrically depended on u,v in range [0,1]
	void Surf(const char *eqX, const char *eqY, const char *eqZ, const char *sch=0, int n=100);


	/// Draw power crust for points in arrays \a x, \a y, \a z.
	void Crust(const mglData &x, const mglData &y, const mglData &z, const char *sch=0,float er=0);
	/// Draw power crust for points in arrays \a tr.
	void Crust(const mglData &tr, const char *sch=0,float er=0);
	/// Draw dots in points \a x, \a y, \a z.
	void Dots(const mglData &x, const mglData &y, const mglData &z, const char *sch=0);
	/// Draw dots in points \a tr.
	void Dots(const mglData &tr, const char *sch=0);
	/// Draw triangle mesh for points in arrays \a x, \a y, \a z.
	void TriPlot(const mglData &nums, const mglData &x, const mglData &y, const mglData &z, const char *sch=0);
	/// Draw triangle mesh for points in arrays \a x, \a y.
	void TriPlot(const mglData &nums, const mglData &x, const mglData &y, const char *sch=0, float zVal=NAN);
	/// Draw grid lines for density plot of 2d data specified parametrically
	void Grid(const mglData &x, const mglData &y, const mglData &z, const char *stl=0,float zVal=NAN);
	/// Draw grid lines for density plot of 2d data
	void Grid(const mglData &a,const char *stl=0,float zVal=NAN);
	/// Draw mesh lines for 2d data specified parametrically
	void Mesh(const mglData &x, const mglData &y, const mglData &z, const char *sch=0);
	/// Draw mesh lines for 2d data
	void Mesh(const mglData &z, const char *sch=0);
	/// Draw mesh lines for 2d data specified parametrically
	void Fall(const mglData &x, const mglData &y, const mglData &z, const char *sch=0);
	/// Draw mesh lines for 2d data
	void Fall(const mglData &z, const char *sch=0);
	/// Draw belts for 2d data specified parametrically
	void Belt(const mglData &x, const mglData &y, const mglData &z, const char *sch=0);
	/// Draw belts for 2d data
	void Belt(const mglData &z, const char *sch=0);
	/// Draw surface for 2d data specified parametrically
	void Surf(const mglData &x, const mglData &y, const mglData &z, const char *sch=0);
	/// Draw surface for 2d data
	void Surf(const mglData &z, const char *sch=0);
	/// Draw density plot for surface specified parametrically
	void Dens(const mglData &x, const mglData &y, const mglData &z, const char *sch=0,float zVal=NAN);
	/// Draw density plot for 2d data
	void Dens(const mglData &z, const char *sch=0,float zVal=NAN);
	/// Draw density plot for spectra-gramm specified parametrically
	void STFA(const mglData &x, const mglData &y, const mglData &re, const mglData &im, int dn, const char *sch=0,float zVal=NAN);
	/// Draw density plot for spectra-gramm
	void STFA(const mglData &re, const mglData &im, int dn, const char *sch=0,float zVal=NAN);
	/// Draw vertical boxes for 2d data specified parametrically
	void Boxs(const mglData &x, const mglData &y, const mglData &z, const char *sch=0,float zVal=NAN);
	/// Draw vertical boxes for 2d data
	void Boxs(const mglData &z, const char *sch=0,float zVal=NAN);
	/// Draw vertical tiles for 2d data specified parametrically
	void Tile(const mglData &x, const mglData &y, const mglData &z, const char *sch=0);
	/// Draw vertical tiles for 2d data
	void Tile(const mglData &z, const char *sch=0);
	/// Draw contour lines for 2d data specified parametrically
	void Cont(const mglData &v, const mglData &x, const mglData &y, const mglData &z, const char *sch=0, float zVal=NAN);
	/// Draw contour lines for 2d data
	void Cont(const mglData &v, const mglData &z, const char *sch=0,float zVal=NAN);
	/// Draw several contour lines for 2d data specified parametrically
	void Cont(const mglData &x, const mglData &y, const mglData &z, const char *sch=0, int Num=7, float zVal=NAN);
	/// Draw several contour lines for 2d data
	void Cont(const mglData &z, const char *sch=0, int Num=7, float zVal=NAN);
	/// Draw axial-symmetric isosurfaces for 2d data specified parametrically
	void Axial(const mglData &v, const mglData &x, const mglData &y, const mglData &a, const char *sch=0);
	/// Draw axial-symmetric isosurfaces for 2d data
	void Axial(const mglData &v, const mglData &a, const char *sch=0);
	/// Draw several axial-symmetric isosurfaces for 2d data specified parametrically
	void Axial(const mglData &x, const mglData &y, const mglData &a, const char *sch=0, int Num=3);
	/// Draw several axial-symmetric isosurfaces for 2d data
	void Axial(const mglData &a, const char *sch=0, int Num=3);
	/// Draw solid contours for 2d data specified parametrically
	void ContF(const mglData &v, const mglData &x, const mglData &y, const mglData &z, const char *sch=0, float zVal=NAN);
	/// Draw solid contours for 2d data
	void ContF(const mglData &v, const mglData &z, const char *sch=0,float zVal=NAN);
	/// Draw several solid contours for 2d data specified parametrically
	void ContF(const mglData &x, const mglData &y, const mglData &z, const char *sch=0, int Num=7, float zVal=NAN);
	/// Draw several solid contours for 2d data
	void ContF(const mglData &z, const char *sch=0, int Num=7, float zVal=NAN);
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
	void Dew(const mglData &x, const mglData &y, const mglData &ax, const mglData &ay, const char *sch=0,float zVal=NAN);
	/// Plot dew drops for vector field {ax,ay}
	void Dew(const mglData &ax, const mglData &ay, const char *sch=0,float zVal=NAN);
	/// Draw surface specified parametrically with coloring by other matrix
	void SurfC(const mglData &x, const mglData &y, const mglData &z, const mglData &c, const char *sch=0);
	/// Draw surface specified by matrix with coloring by other matrix
	void SurfC(const mglData &z, const mglData &c, const char *sch=0);
	/// Draw surface specified parametrically which transparency is determined by other matrix
	void SurfA(const mglData &x, const mglData &y, const mglData &z, const mglData &c, const char *sch=0);
	/// Draw surface specified by matrix which transparency is determined by other matrix
	void SurfA(const mglData &z, const mglData &c, const char *sch=0);
	/// Draw vertical tiles with variable size for 2d data specified parametrically
	void TileS(const mglData &x, const mglData &y, const mglData &z, const mglData &r, const char *sch=0);
	/// Draw vertical tiles with variable size for 2d data
	void TileS(const mglData &z, const mglData &r, const char *sch=0);
	/// Plot vector field {ax,ay} parametrically depended on coordinate {x,y} with length proportional to value |a|
	void Vect(const mglData &x, const mglData &y, const mglData &ax, const mglData &ay, const char *sch=0,float zVal=NAN);
	/// Plot vector field {ax,ay} with length proportional to value |a|
	void Vect(const mglData &ax, const mglData &ay, const char *sch=0,float zVal=NAN);
	/// Plot vector field {ax,ay} parametrically depended on coordinate {x,y} with color proportional to value |a|
	void VectC(const mglData &x, const mglData &y, const mglData &ax, const mglData &ay, const char *sch=0,float zVal=NAN);
	/// Plot vector field {ax,ay} with color proportional to value |a|
	void VectC(const mglData &ax, const mglData &ay, const char *sch=0,float zVal=NAN);
	/// Plot 3d vector field {ax,ay,ay} parametrically depended on coordinate {x,y,z} with length proportional to value |a|
	void Vect(const mglData &x, const mglData &y, const mglData &z, const mglData &ax, const mglData &ay, const mglData &az, const char *sch=0);
	/// Plot 3d vector field {ax,ay,ay} with length proportional to value |a|
	void Vect(const mglData &ax, const mglData &ay, const mglData &az, const char *sch=0);
	/// Plot 3d vector field {ax,ay,ay} parametrically depended on coordinate {x,y,z} with color proportional to value |a|
	void VectC(const mglData &x, const mglData &y, const mglData &z, const mglData &ax, const mglData &ay, const mglData &az, const char *sch=0);
	/// Plot 3d vector field {ax,ay,ay} with color proportional to value |a|
	void VectC(const mglData &ax, const mglData &ay, const mglData &az, const char *sch=0);
	/// Color map of matrix a to matrix b, both matrix parametrically depend on coordinates
	void Map(const mglData &x, const mglData &y, const mglData &a, const mglData &b, const char *sch=0, int ks=0, bool pnts=true);
	/// Color map of matrix a to matrix b
	void Map(const mglData &a, const mglData &b, const char *sch=0, int ks=0, bool pnts=true);
	/// Draw isosurface for 3d data \a a specified parametrically with alpha proportional to \a b
	void Surf3A(float Val, const mglData &x, const mglData &y, const mglData &z, const mglData &a, const mglData &b,
				const char *stl=0);
	/// Draw isosurface for 3d data \a a with alpha proportional to \a b
	void Surf3A(float Val, const mglData &a, const mglData &b, const char *stl=0);
	/// Draw several isosurface for 3d data \a a specified parametrically with alpha proportional to \a b
	void Surf3A(const mglData &x, const mglData &y, const mglData &z, const mglData &a, const mglData &b,
				const char *stl=0, int num=3);
	/// Draw several isosurface for 3d data \a a with alpha proportional to \a b
	void Surf3A(const mglData &a, const mglData &b, const char *stl=0, int num=3);
	/// Draw isosurface for 3d data \a a specified parametrically with color proportional to \a b
	void Surf3C(float Val, const mglData &x, const mglData &y, const mglData &z, const mglData &a, const mglData &b,
				const char *stl=0);
	/// Draw isosurface for 3d data \a a with color proportional to \a b
	void Surf3C(float Val, const mglData &a, const mglData &b, const char *stl=0);
	/// Draw several isosurface for 3d data \a a specified parametrically with color proportional to \a b
	void Surf3C(const mglData &x, const mglData &y, const mglData &z, const mglData &a, const mglData &b,
				const char *stl=0, int num=3);
	/// Draw several isosurface for 3d data \a a with color proportional to \a b
	void Surf3C(const mglData &a, const mglData &b, const char *stl=0, int num=3);
	/// Plot flows for vector field {ax,ay} parametrically depended on coordinate {x,y} with color proportional to value |a|
	void Flow(const mglData &x, const mglData &y, const mglData &ax, const mglData &ay, const char *sch=0, int num=5, bool central=true, float zVal=NAN);
	/// Plot flows for vector field {ax,ay} with color proportional to value |a|
	void Flow(const mglData &ax, const mglData &ay, const char *sch=0, int num=5, bool central=true, float zVal=NAN);
	/// Plot flows for 3d vector field {ax,ay,ay} parametrically depended on coordinate {x,y,z} with color proportional to value |a|
	void Flow(const mglData &x, const mglData &y, const mglData &z, const mglData &ax, const mglData &ay, const mglData &az, const char *sch=0, int num=3, bool central=true);
	/// Plot flows for 3d vector field {ax,ay,ay} with color proportional to value |a|
	void Flow(const mglData &ax, const mglData &ay, const mglData &az, const char *sch=0, int num=3, bool central=true);
	/// Plot flow pipes for vector field {ax,ay} parametrically depended on coordinate {x,y} with color proportional to value |a|
	void Pipe(const mglData &x, const mglData &y, const mglData &ax, const mglData &ay, const char *sch=0, float r0=0.05, int num=5, bool central=true, float zVal=NAN);
	/// Plot flow pipes for vector field {ax,ay} with color proportional to value |a|
	void Pipe(const mglData &ax, const mglData &ay, const char *sch=0, float r0=0.05, int num=5, bool central=true, float zVal=NAN);
	/// Plot flow pipes for 3d vector field {ax,ay,ay} parametrically depended on coordinate {x,y,z} with color proportional to value |a|
	void Pipe(const mglData &x, const mglData &y, const mglData &z, const mglData &ax, const mglData &ay, const mglData &az, const char *sch=0, float r0=0.05, int num=3, bool central=true);
	/// Plot flow pipes for 3d vector field {ax,ay,ay} with color proportional to value |a|
	void Pipe(const mglData &ax, const mglData &ay, const mglData &az, const char *sch=0, float r0=0.05, int num=3, bool central=true);
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
	void Grid3(const mglData &x, const mglData &y, const mglData &z, const mglData &a, char dir, int sVal=-1, const char *stl=0);
	/// Draw grid lines for density plot at slice for 3d data
	void Grid3(const mglData &a, char dir, int sVal=-1, const char *stl=0);
	/// Draw grid lines for density plot at central slices for 3d data specified parametrically
	void GridA(const mglData &x, const mglData &y, const mglData &z, const mglData &a, const char *stl=0);
	/// Draw grid lines for density plot at central slices for 3d data
	void GridA(const mglData &a, const char *stl=0);
	/// Draw density plot at slice for 3d data specified parametrically
	void Dens3(const mglData &x, const mglData &y, const mglData &z, const mglData &a, char dir, int sVal=-1, const char *stl=0);
	/// Draw density plot at slice for 3d data
	void Dens3(const mglData &a, char dir, int sVal=-1, const char *stl=0);
	/// Draw density plot at central slices for 3d data specified parametrically
	void DensA(const mglData &x, const mglData &y, const mglData &z, const mglData &a, const char *stl=0);
	/// Draw density plot at central slices for 3d data
	void DensA(const mglData &a, const char *stl=0);
	/// Draw isosurface for 3d data specified parametrically
	void Surf3(float Val, const mglData &x, const mglData &y, const mglData &z, const mglData &a, const char *stl=0);
	/// Draw isosurface for 3d data
	void Surf3(float Val, const mglData &a, const char *stl=0);
	/// Draw several isosurfaces for 3d data specified parametrically
	void Surf3(const mglData &x, const mglData &y, const mglData &z, const mglData &a, const char *stl=0, int num=3);
	/// Draw several isosurfaces for 3d beam in curvilinear coordinates
	void Beam(const mglData &tr, const mglData &g1, const mglData &g2, const mglData &a, float r, const char *stl=0, int flag=0, int num=3);
	/// Draw isosurface for 3d beam in curvilinear coordinates
	void Beam(float val,const mglData &tr, const mglData &g1, const mglData &g2, const mglData &a, float r, const char *stl=0, int flag=0);
	/// Draw several isosurface for 3d data
	void Surf3(const mglData &a, const char *stl=0, int num=3);
	/// Draw contour lines at slice for 3d data specified parametrically
	void Cont3(const mglData &v, const mglData &x, const mglData &y, const mglData &z, const mglData &a,
				char dir, int sVal=-1, const char *sch=0);
	/// Draw contour lines at slice for 3d data
	void Cont3(const mglData &v, const mglData &a, char dir, int sVal=-1, const char *sch=0);
	/// Draw several contour lines at slice for 3d data specified parametrically
	void Cont3(const mglData &x, const mglData &y, const mglData &z, const mglData &a,
				char dir, int sVal=-1, const char *sch=0, int Num=7);
	/// Draw several contour lines at slice for 3d data
	void Cont3(const mglData &a, char dir, int sVal=-1, const char *sch=0, int Num=7);
	/// Draw contour lines at central slices for 3d data specified parametrically
	void ContA(const mglData &x, const mglData &y, const mglData &z, const mglData &a,
				const char *sch=0, int Num=7);
	/// Draw contour lines at central slices for 3d data
	void ContA(const mglData &a, const char *sch=0, int Num=7);
	/// Draw solid contours at slice for 3d data specified parametrically
	void ContF3(const mglData &v, const mglData &x, const mglData &y, const mglData &z, const mglData &a,
				char dir, int sVal=-1, const char *sch=0);
	/// Draw solid contours at slice for 3d data
	void ContF3(const mglData &v, const mglData &a, char dir, int sVal=-1, const char *sch=0);
	/// Draw several solid contours at slice for 3d data specified parametrically
	void ContF3(const mglData &x, const mglData &y, const mglData &z, const mglData &a,
				char dir, int sVal=-1, const char *sch=0, int Num=7);
	/// Draw several solid contours at slice for 3d data
	void ContF3(const mglData &a, char dir, int sVal=-1, const char *sch=0, int Num=7);
	/// Draw solid contours at central slices for 3d data specified parametrically
	void ContFA(const mglData &x, const mglData &y, const mglData &z, const mglData &a,
				const char *sch=0, int Num=7);
	/// Draw solid contours at central slices for 3d data
	void ContFA(const mglData &a, const char *sch=0, int Num=7);
	/// Draw a cloud of points for 3d data specified parametrically
	void CloudP(const mglData &x, const mglData &y, const mglData &z, const mglData &a,
				const char *stl=0, float alpha=1);
	/// Draw a cloud of points for 3d data
	void CloudP(const mglData &a, const char *stl=0, float alpha=1);
	/// Draw a semi-transparent cloud for 3d data specified parametrically
	void CloudQ(const mglData &x, const mglData &y, const mglData &z, const mglData &a,
				const char *stl=0, float alpha=1);
	/// Draw a semi-transparent cloud for 3d data
	void CloudQ(const mglData &a, const char *stl=0, float alpha=1);
	//@}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** @name Combined plotting functions
	  * These plotting functions draw density plot or contour lines in x, y, or z plain.
	  * If \a a is a tensor (3-dimensional data) then interpolation to a given
	  * sVal is performed. */
	//@{
	/// Draw density plot for data a at x = sVal
	void DensX(const mglData &a, const char *stl=0, float sVal=NAN);
	/// Draw density plot for data a at y = sVal
	void DensY(const mglData &a, const char *stl=0, float sVal=NAN);
	/// Draw density plot for data a at z = sVal
	void DensZ(const mglData &a, const char *stl=0, float sVal=NAN);
	/// Draw several contour plots for data a at x = sVal
	void ContX(const mglData &a, const char *stl=0, float sVal=NAN, int Num=7);
	/// Draw several contour plots for data a at y = sVal
	void ContY(const mglData &a, const char *stl=0, float sVal=NAN, int Num=7);
	/// Draw several contour plots for data a at z = sVal
	void ContZ(const mglData &a, const char *stl=0, float sVal=NAN, int Num=7);
	/// Draw contour plots for data a at x = sVal
	void ContX(const mglData &v, const mglData &a, const char *stl=0, float sVal=NAN);
	/// Draw contour plots for data a at y = sVal
	void ContY(const mglData &v, const mglData &a, const char *stl=0, float sVal=NAN);
	/// Draw contour plots for data a at z = sVal
	void ContZ(const mglData &v, const mglData &a, const char *stl=0, float sVal=NAN);
	/// Draw several contour plots for data a at x = sVal
	void ContFX(const mglData &a, const char *stl=0, float sVal=NAN, int Num=7);
	/// Draw several contour plots for data a at y = sVal
	void ContFY(const mglData &a, const char *stl=0, float sVal=NAN, int Num=7);
	/// Draw several contour plots for data a at z = sVal
	void ContFZ(const mglData &a, const char *stl=0, float sVal=NAN, int Num=7);
	/// Draw contour plots for data a at x = sVal
	void ContFX(const mglData &v, const mglData &a, const char *stl=0, float sVal=NAN);
	/// Draw contour plots for data a at y = sVal
	void ContFY(const mglData &v, const mglData &a, const char *stl=0, float sVal=NAN);
	/// Draw contour plots for data a at z = sVal
	void ContFZ(const mglData &v, const mglData &a, const char *stl=0, float sVal=NAN);
	//@}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** @name IDTF specific functions
	  * These functions are useful only in IDTF mode
	  * VertexColor enables smooth color change.
	  * Compression gives smaller files, but quality degrades.
	  * Unrotate reverses the current space transform before saving,
	  * in an attempt to make MathGl axes similar to Adobe Reader axes
	  * It makes positioning cameras and crossections easier
	  * Groups contain objects and other groups, they are used to select a part of a model to
	  * zoom to / make invizible / make transparent / etc.
	  *  */
	//@{
	virtual void VertexColor(bool enable);
	virtual void Compression(bool enable);
	virtual void Unrotate(bool enable);
	virtual void StartGroup (const char *name);
	virtual void EndGroup();
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
	char PalNames[101];			///< IDs of colors in the palette
	int CurrPal;				///< Current index of palette mglGraph::Pal
	int NumCol;					///< Actual number of colors in color scheme mglGraph::cmap
	float CloudFactor;			///< Factor of transparency in mglGraph::CloudP and mglGraph::CloudQ
	bool ScalePuts;				///< Enable/disable point positions scaling in puts
	bool SmoothColorbar;		///< Use color interpolation in colorbar (default is true)

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
	virtual float GetOrgX(char dir);
	/// Get Org.y (parse NAN value)
	virtual float GetOrgY(char dir);
	/// Get Org.z (parse NAN value)
	virtual float GetOrgZ(char dir);
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
	/// Draw line between points \a p1,\a p2 with color \a c1, \a c2 at edges
	virtual void line_plot(float *p1,float *p2,float *c1,float *c2,bool all=false)=0;

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
	char *fit_res;				///< Last fitted formula

	static void *jmodule;		///< Module for JPEG function
	static void *tmodule;		///< Module for TIFF function
	void InitSaveFunc();		///< Load modules for JPEG, TIFF functions (first times)
	void FreeSaveFunc();		///< Free modules for JPEG, TIFF functions (last times)
	void ClearEq();				///< Clear the used variables for axis transformation
	mglColor GetC2(float x,float y);

	/// Prepare fitted formula
	void PrepareFitEq(float chi, const char *eq, const char *var, float *par, bool print);
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
				const mglData &x, const mglData &y, const mglData &ax, const mglData &ay);
	/// make single flow thread for 3D case
	void flow(bool simple, float u, float v, float w,
				const mglData &x, const mglData &y, const mglData &z,
				const mglData &ax, const mglData &ay, const mglData &az);
	/// make single flow tube for 2D case
	void flowr(bool simple, float zVal, float u, float v,
				const mglData &x, const mglData &y, const mglData &ax, const mglData &ay, float r0);
	/// make single flow tube for 3D case
	void flowr(bool simple, float u, float v, float w,
				const mglData &x, const mglData &y, const mglData &z,
				const mglData &ax, const mglData &ay, const mglData &az, float r0);
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
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
