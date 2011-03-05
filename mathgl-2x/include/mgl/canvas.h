/***************************************************************************
 * canvas.h is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef MGL_H
#define MGL_H
#include "mgl/base.h"
/*****************************************************************************/
#ifndef MGL_STACK_ENTRY
#define MGL_STACK_ENTRY	10
#endif
/*****************************************************************************/
#ifdef __cplusplus
//-----------------------------------------------------------------------------
#ifdef HAVE_GIF
#include <gif_lib.h>
#else
struct GifFileType;
#endif
//-----------------------------------------------------------------------------
struct mglPrim;
//-----------------------------------------------------------------------------
struct mglAxis
{
	mglAxis();
	~mglAxis()	{	delete []buf;	}
	void AddLabel(float val, const wchar_t *b);

	float dv,ds;	///< Actual step for ticks and subticks.
	float d;		///< Step for axis ticks (if positive) or its number (if negative).
	int ns;			///< Number of axis subticks.
	wchar_t t[256];	///< Tick template (set NULL to use default one ("%.2g" in simplest case))
	mglPoint dir;	///< Axis direction
	mglPoint a,b;	///< Directions of over axis
	mglPoint org;
	float v0,v1,v2;	///< Center of axis cross section and its range.
	float o;		///< Point of starting ticks numbering (if NAN then Org is used).
	int f;			///< Flag 0x1 - time, 0x2 - manual, 0x4 - fixed dv
	int num;		///< Number of manual ticks
	wchar_t *buf;	///< Buffer for manual ticks
	wchar_t *str[64];	///< Position of manual ticks
	bool upd;		///< Last label is ticks factor
	float val[64];	///< Values of manual ticks
	char ch;		///< Character of axis (like 'x','y','z','c')
};
//-----------------------------------------------------------------------------
/// Class contains all functionality for creating different mathematical plots
class mglCanvas : public mglBase
{
friend struct mglPrim;
public:
	int ObjId;				///< object id for mglPrim
	mglPoint LastMousePos;	///< Last mouse position
	const char *PlotId;	///< Id of plot for saving filename (in GLUT window for example)

	mglCanvas();
	virtual ~mglCanvas();

	/// Set PlotFactor
	inline void SetPlotFactor(float val)
	{	PlotFactor = val>0?val:1.55;	AutoPlotFactor=(val<=0);	}
	/// Set plot quality
	inline void SetQuality(int qual=2)	{	Quality=qual;	}

	///< Set default parameter for plotting
	void DefaultPlotParam();
	/// Set angle of view indepently from mglCanvas::Rotate().
	virtual void View(float tetx,float tetz,float tety=0);
	/// Zoom in or zoom out (if Zoom(0, 0, 1, 1)) a part of picture
	void Zoom(float x1, float y1, float x2, float y2);
	/// Clear transformation matrix.
	inline void Identity(bool rel=false)	{	InPlot(0,1,0,1,rel);	}
	/// Push transformation matrix into stack
	void Push();
	/// Pop transformation matrix from stack
	void Pop();
	/// Clear up the frame
	virtual void Clf(mglColor Back=WC);
	/// Put further plotting in some region of whole frame surface.
	void SubPlot(int nx,int ny,int m, float dx=0, float dy=0);
	void SubPlot(int nx,int ny,int m, const char *style);
	/// Like SubPlot bot "join" several cells
	void MultiPlot(int nx,int ny,int m, int dx, int dy, const char *style);
	/// Put further plotting in column cell of previous subplot
	void ColumnPlot(int num, int i, float d=0);
	/// Put further plotting in cell of stick rotated on angles tet, phi
	void StickPlot(int num, int i, float tet, float phi);
	/// Put further plotting in some region of whole frame surface.
	void InPlot(float x1,float x2,float y1,float y2,bool rel=true);
	/// Set aspect ratio for further plotting.
	void Aspect(float Ax,float Ay,float Az);
	/// Rotate a further plotting.
	void Rotate(float TetX,float TetZ,float TetY=0);
	/// Rotate a further plotting around vector {x,y,z}.
	void RotateN(float Tet,float x,float y,float z);
	/// Set perspective (in range [0,1)) for plot. Set to zero for switching off.
	void Perspective(float a);

	/// Set size of frame in pixels. Normally this function is called internaly.
	virtual void SetSize(int w,int h);
	/// Get ratio (float width)/(float height).
	inline float GetRatio()	{	return Width/Height;	};
	/// Get RGB bitmap of current state image.
	const unsigned char *GetBits();
	/// Get RGBA bitmap of current state image.
	const unsigned char *GetRGBA();
	/// Get width of the image
	int GetWidth()	{	return Width;	};
	/// Get height of the image
	int GetHeight()	{	return Height;	};
	/// Set allowed drawing region (for multithreading, like subplots)
	void SetDrawReg(int m, int n, int k);
	/// Put drawing from other mglCanvasZB (for multithreading, like subplots)
	void PutDrawReg(int m, int n, int k, mglCanvas *gr);
	/// Calculate 3D coordinate {x,y,z} for screen point {xs,ys}
	mglPoint CalcXYZ(int xs, int ys);
	/// Calculate screen point {xs,ys} for 3D coordinate {x,y,z}
	void CalcScr(mglPoint p, int *xs, int *ys);
	mglPoint CalcScr(mglPoint p);

	/// Write the frame in file using JPEG format
	void WriteJPEG(const char *fname,const char *descr=0);
	/// Write the frame in file using BMP format
	void WriteBMP(const char *fname,const char *descr=0);
	/// Write the frame in file using PNG format with transparency
	void WritePNG(const char *fname,const char *descr=0);
	/// Write the frame in file using PNG format no transparency
	void WritePNGs(const char *fname,const char *descr=0);
	/// Write the frame in file using PostScript format as bitmap
	void WriteBPS(const char *fname,const char *descr=0);
	/// Write the frame in file using PostScript format
	void WriteEPS(const char *fname,const char *descr=0);
	/// Write the frame in file using SVG format
	void WriteSVG(const char *fname,const char *descr=0);
	/// Write the frame in file using IDTF format
	void WriteIDTF(const char *fname,const char *descr=0);
	/// Write the frame in file using GIF format (only for current frame!)
	void WriteGIF(const char *fname,const char *descr=0);
	/// Write the frame in file (depending extension, write current frame if fname is empty)
	void WriteFrame(const char *fname=0, const char *descr=0);
	/// Show currently produced image by Qt or FLTK library
	void ShowImage(const char *viewer=0, bool keep=false);

	/// Create new frame.
	virtual int NewFrame();
	/// Finish frame drawing
	virtual void EndFrame();
	/// Get the number of created frames
	inline int GetNumFrame()	{	return CurFrameId;	}
	/// Reset frames counter (start it from zero)
	inline void ResetFrames()	{	CurFrameId=0;	}
	/// Start write frames to cinema using GIF format
	void StartGIF(const char *fname, int ms=100);
	/// Stop writing cinema using GIF format
	void CloseGIF();
	/// Finish plotting. Normally this function is called internaly.
	virtual void Finish();

	/// Set the transparency on/off.
	virtual bool Alpha(bool enable);
	/// Set the transparency type
	inline void SetTranspType(int val)	{	TranspType=val;	};
	/// Set the fog distance or switch it off (if d=0).
	virtual void Fog(float d, float dz=0.25);
	/// Set the using of light on/off.
	virtual bool Light(bool enable);
	/// Switch on/off the specified light source.
	virtual void Light(int n, bool enable);
	/// Add a light source.
	virtual void AddLight(int n,mglPoint p, char c='w', float bright=0.5, bool infty=true, float ap=0);
	/// Set ambient light brightness
	virtual void Ambient(float bright=0.5);

	/// Set ticks position and text (\n separated). Use n=0 to disable this feature.
	void SetTicksVal(char dir, HCDT v, const char *lbl);
	void SetTicksVal(char dir, HCDT v, const char **lbl);
	void SetTicksVal(char dir, HCDT v, const wchar_t *lbl);
	void SetTicksVal(char dir, HCDT v, const wchar_t **lbl);
	/// Set templates for ticks
	void SetTickTempl(char dir, const wchar_t *t);
	void SetTickTempl(char dir, char *t);
	/// Set templates for ticks
	void SetTimeTick(char dir, float d, const char *t="%F");
	/// Set the ticks parameters
	void SetTicks(char dir, float d=0, int ns=0, float org=NAN);
	/// Auto adjust ticks
	void AdjustTicks(const char *dir="xyzc", bool force=false);
	/// Tune ticks
	inline void SetTuneTicks(int tune, float pos=1.15)
	{	TuneTicks = tune;	FactorPos = pos;	};
	/// Set ticks styles
	void SetTickStl(const char *stl, const char *sub=0);
	/// Set ticks length
	void SetTickLen(float tlen, float stt=1.);

	/// Draws bounding box outside the plotting volume with color \a c.
	void Box(const char *col=0, bool ticks=true);
	/// Draw axises with ticks in directions determined by string parameter \a dir.
	void Axis(const char *dir="xyzt", bool adjust=false);
	/// Draw grid lines perpendicular to direction determined by string parameter \a dir.
	void Grid(const char *dir="xyzt",const char *pen="B-");
	/// Print the label \a text for axis \a dir.
	void Label(char dir, const char *text, float pos=0, float size=-1.4, float shift=0);
	void Labelw(char dir, const wchar_t *text, float pos=0, float size=-1.4, float shift=0);

	/// Print the \a text at arbitrary position of the picture \a x, \a y in range [0,1]x[0,1].
	void Label(float x, float y, const char *text, const char *fnt=0, float size=-1.4, bool rel=false);
	void Labelw(float x, float y, const wchar_t *text, const char *fnt=0, float size=-1.4, bool rel=false);
	/// Print the title text for the picture
	void Title(const wchar_t *text,const char *font=0,float size=-2);
	void Title(const char *text,const char *font=0,float size=-2);

	/// Draw colorbar at edge of axis
	void Colorbar(const char *sch=0,int where=0);
	void inline Colorbar(const char *sch, int where, float x, float y, float w, float h)
	{	SetScheme(sch);	Colorbar(where,x,y,w,h);	};
	void Colorbar(int where, float x, float y, float w, float h);
	/// Draw colorbar at edge of axis
	void Colorbar(HCDT v, const char *sch=0,int where=0);
	void Colorbar(HCDT v, const char *sch, int where, float x, float y, float w, float h);

	/// Add string to legend
	void AddLegend(const char *text,const char *style);
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
	/// Switch on/off box around legend
	inline void SetLegendBox(bool val)		{	LegendBox=val;	};
	/// Number of marks in legend sample
	inline void SetLegendMarks(int num=1)	{	LegendMarks = num>0?num:1;	};

protected:
	bool Finished;		///< Flag that final picture \a mglCanvasZB::G is ready
	float *Z;			///< Height for given level in Z-direction
	unsigned char *C;	///< Picture for given level in Z-direction
	int *OI;			///< ObjId arrays
	unsigned char *G4;	///< Final picture in RGBA format. Prepared in Finish().
	unsigned char *G;	///< Final picture in RGB format. Prepared in Finish().
	mglPrim *P;			///< Primitives (lines, triangles and so on)
	long pNum;			///< Actual number of primitives
	long pMax;			///< Maximal number of primitives
	float PlotFactor;	///< Factor for sizing overall plot (should be >1.5, default is =1.55)
	bool AutoPlotFactor;///< Enable autochange PlotFactor
	float zoomx1, zoomy1, zoomx2, zoomy2;
	unsigned char BDef[4];	///< Background color
	mglAxis ax,ay,az,ac;///< Axis parameters

	int TuneTicks;		///< Draw tuned ticks with extracted common component
	float FactorPos;	///< Position of axis ticks factor (0 at Min, 1 at Max, 1.1 is default)
	float TickLen;		///< Length of tiks (subticks length is sqrt(1+st_t)=1.41... times smaller)
	char TickStl[32];	///< Tick line style. Default is "k"
	char SubTStl[32];	///< Subtick line style. Default is "k"
	float st_t;			///< Subtick-to-tick ratio (ls=lt/sqrt(1+st_t)). Default is 1.

	bool LegendBox;		///< Set on/off drawing legend box.
	int LegendMarks;	///< Number of marks in the Legend
	int NumLeg;			///< Number of used positions in LegStr and LegStl arrays
	wchar_t *LegStr[100];	///< String array with legend text (see mglGraph::AddLegend)
	char *LegStl[100];	///< String array with legend style (see mglGraph::AddLegend)

	int Quality;		///< Quality of plot (0x0-pure, 0x1-fast; 0x2-fine; 0x4 - low memory)
	int TranspType;		/// Type of transparency (no full support in OpenGL mode).
	int CurFrameId;		///< Number of automaticle created frames
	float Persp;		///< Perspective factor (=0 is perspective off)
	int Width;			///< Width of the image
	int Height;			///< Height of the image
	int Depth;			///< Depth of the image
	float B[12];		///< Transformation matrix
	float B1[12];		///< Transformation matrix for colorbar
	float inW, inH;		///< Relative width and height of last InPlot
	bool UseAlpha;		///< Flag that Alpha is used
	bool UseLight;		///< Flag of using lightning
	bool nLight[10];	///< Availability of light sources
	bool iLight[10];	///< Infinity/local position of light sources
	float rLight[30];	///< Position of light sources
	float pLight[30];	///< Actual position of light sources (filled by LightScale() function)
	float aLight[10];	///< Aperture of light sources
	float bLight[10];	///< Brightness of light sources
	float cLight[30];	///< Color of light sources
	float AmbBr;		///< Default ambient light brightness
	float FogDist;		/// Inverse fog distance (fog ~ exp(-FogDist*Z))
	float FogDz;		/// Relative shift of fog

	/// Auto adjust ticks
	void AdjustTicks(mglAxis &aa, bool ff);
	/// Prepare labels for ticks
	void LabelTicks(mglAxis &aa);
	/// Draw axis
	void DrawAxis(mglAxis &aa, bool text=true, char arr=0);
	/// Draw axis grid lines
	void DrawGrid(mglAxis &aa);
	/// Update axis ranges
	inline void UpdateAxis()
	{	ax.v0=Org.x;	ay.v0=Org.y;	az.v0=Org.z;	ac.v0=Org.c;
		ax.v1=Min.x;	ay.v1=Min.y;	az.v1=Min.z;	ac.v1=Min.c;
		ax.v2=Max.x;	ay.v2=Max.y;	az.v2=Max.z;	ac.v2=Max.c;	}
	/// Scale coordinates and cut off some points
	bool ScalePoint(mglPoint &p, bool use_nan=true)
	{	bool res=mglBase::ScalePoint(p, use_nan);	PostScale(p);	return res;	}
	void LightScale();	///< Additionally scale positions of light sources
	/// Additionally scale normals \a n
	void NormScale(mglPoint &n);
	float GetOrgX(char dir);	///< Get Org.x (parse NAN value)
	float GetOrgY(char dir);	///< Get Org.y (parse NAN value)
	float GetOrgZ(char dir);	///< Get Org.z (parse NAN value)

	void mark_plot(long p, char type, float size=1);	// position in pntC
	void arrow_plot(long p1, long p2, char st);			// position in pntC
	void line_plot(long p1, long p2, bool fromN=false);	// position in pntC
	void trig_plot(long p1, long p2, long p3);			// position in pntN
	void quad_plot(long p1, long p2, long p3, long p4);	// position in pntN
	void Glyph(float x, float y, float f, int style, long icode, char col);
	float text_plot(long p,const wchar_t *text,const char *fnt,float size,float sh=0);	// position in pntN

	void add_prim(mglPrim &a);	///< add primitive to list
	void mark_draw(float *p, char type, float size);
	void arrow_draw(float *p1, float *p2, char st, float size);
	virtual void line_draw(float *p1, float *p2);
	virtual void trig_draw(float *p1, float *p2, float *p3,bool anorm=false);
	virtual void quad_draw(float *p1, float *p2, float *p3, float *p4);
	void glyph_draw(float *p, float f, int style, long icode);

private:
	float _tetx,_tety,_tetz;			// extra angles
	float stack[MGL_STACK_ENTRY*13];	// stack for transformation matrixes
	int st_pos;
	float font_factor;
	int dr_nx1, dr_nx2, dr_ny1, dr_ny2;	// Allowed drawing region
	GifFileType *gif;
	float fscl,ftet;	///< last scale and rotation for glyphs

	/// Draw generic colorbar
	void colorbar(HCDT v, const mglColor *s, int where, float x, float y, float w, float h);
	/// Draw labels for ticks
	void DrawLabels(mglAxis &aa);
	/// Draw tick
	void tick_draw(mglPoint o, mglPoint d1, mglPoint d2, int f);
	/// Plot point \a p with color \a c
	void pnt_plot(long x,long y,float z,unsigned char c[4]);
	unsigned char **GetRGBLines(long &w, long &h, unsigned char *&f, bool solid=true);
	float FindOptOrg(char dir, int ind);
	/// Transform mglColor and alpha value to bits format
	unsigned char* col2int(mglColor c, float alpha,unsigned char *r);	// mglColor -> int
	/// Transform float color and alpha to bits format
	unsigned char* col2int(float *c,float *n,unsigned char *r);
	/// Combine colors in 2 plane.
	void combine(unsigned char *c1,unsigned char *c2);
	/// Fast drawing of line between 2 points
	void fast_draw(float *p1, float *p2);

	/// Additionally scale points \a p (array with length 3*n) for positioning in image
	void PostScale(mglPoint &p);
	inline void PostScale(mglPoint *p,long n)	{	for(long i=0;i<n;i++)	PostScale(p[i]);	}
	/// Additionally scale normals \a s (array with length 3*n)
	void NormScale(float *s,long n);

	void InPlot(float x1,float x2,float y1,float y2, const char *style);
	void put_line(void *fp, bool gz, long i, float wp,float *cp,int st, const char *ifmt, const char *nfmt, bool neg);
	void put_desc(void *fp, bool gz, const char *pre, const char *ln1, const char *ln2, const char *ln3, const char *suf);
	void put_color(float *c, mglPrim *p);

	void glyph_fill(mreal *p, mreal f, int nt, const short *trig);
	void glyph_wire(mreal *p, mreal f, int nl, const short *line);
	void glyph_line(mreal *p, mreal f, bool solid);
};
//-----------------------------------------------------------------------------
/// Structure for simplest primitive
struct mglPrim
{
	long n1,n2,n3,n4;	///< coordinates of corners
	float z;			///< z-position
	float s;			///< size (if applicable) or fscl
	float w;			///< width (if applicable) or ftet
	wchar_t m;			///< mark or symbol id (if applicable)
	int type;			///< type of primitive (0 - point, 1 - line, 2 - trig, 3 - quad, 4 - glyph, 5 - arrow)
	int style;			///< style of pen
	int id;				///< object id
	mglCanvas *gr;		///< pointer to owner

	void Draw();
	bool IsSame(float wp,float *cp,int st);
	float inline xx()	{	return gr->pntC[7*n1];		}
	float inline yy()	{	return gr->pntC[7*n1+1];	}
	mglPrim(int t=0)	{	memset(this,0,sizeof(mglPrim));	type = t;	}
//	~mglPrim()	{	if(raw)	delete []raw;	};
	inline void operator=(mglPrim &a)	{	memcpy(this,&a,sizeof(mglPrim));	}
};
//-----------------------------------------------------------------------------
extern "C" {
#endif
/*****************************************************************************/
void mgl_show_image(HMGL graph, const char *viewer, int keep);
void mgl_write_frame(HMGL graph, const char *fname,const char *descr);
void mgl_write_bmp(HMGL graph, const char *fname,const char *descr);
void mgl_write_jpg(HMGL graph, const char *fname,const char *descr);
void mgl_write_png(HMGL graph, const char *fname,const char *descr);
void mgl_write_png_solid(HMGL graph, const char *fname,const char *descr);
void mgl_write_eps(HMGL graph, const char *fname,const char *descr);
void mgl_write_svg(HMGL graph, const char *fname,const char *descr);
void mgl_write_idtf(HMGL graph, const char *fname,const char *descr);
void mgl_write_gif(HMGL graph, const char *fname,const char *descr);
void mgl_start_gif(HMGL graph, const char *fname,int ms);
void mgl_close_gif(HMGL graph);

const unsigned char *mgl_get_rgb(HMGL graph);
const unsigned char *mgl_get_rgba(HMGL graph);
int mgl_get_width(HMGL graph);
int mgl_get_height(HMGL graph);
void mgl_calc_xyz(HMGL gr, int xs, int ys, float *x, float *y, float *z);
void mgl_calc_scr(HMGL gr, float x, float y, float z, int *xs, int *ys);

int mgl_new_frame(HMGL graph);
void mgl_end_frame(HMGL graph);
int mgl_get_num_frame(HMGL graph);
void mgl_reset_frames(HMGL graph);

void mgl_set_transp_type(HMGL graph, int type);
void mgl_set_alpha(HMGL graph, int enable);
void mgl_set_fog(HMGL graph, float d, float dz);
void mgl_set_light(HMGL graph, int enable);
void mgl_set_light_n(HMGL gr, int n, int enable);
void mgl_add_light(HMGL graph, int n, float x, float y, float z, char c);
void mgl_add_light_ext(HMGL graph, int n, float x, float y, float z, char c, float br, bool inf, float ap);
void mgl_set_ambbr(HMGL gr, float i);

void mgl_mat_pop(HMGL gr);
void mgl_mat_push(HMGL gr);
void mgl_identity(HMGL graph, int rel);
void mgl_clf(HMGL graph);
void mgl_clf_rgb(HMGL graph, float r, float g, float b);

void mgl_subplot(HMGL graph, int nx,int ny,int m);
void mgl_subplot_d(HMGL graph, int nx,int ny,int m, float dx, float dy);
void mgl_subplot_s(HMGL graph, int nx,int ny,int m,const char *style);
void mgl_inplot(HMGL graph, float x1,float x2,float y1,float y2);
void mgl_relplot(HMGL graph, float x1,float x2,float y1,float y2);
void mgl_columnplot(HMGL graph, int num, int ind);
void mgl_columnplot_d(HMGL graph, int num, int ind, float d);
void mgl_stickplot(HMGL graph, int num, int ind, float tet, float phi);
void mgl_aspect(HMGL graph, float Ax,float Ay,float Az);
void mgl_rotate(HMGL graph, float TetX,float TetZ,float TetY);
void mgl_rotate_vector(HMGL graph, float Tet,float x,float y,float z);
void mgl_perspective(HMGL graph, float val);

/*****************************************************************************/
void mgl_show_image_(uintptr_t *graph, const char *viewer, int *keep, int);
void mgl_write_frame_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_bmp_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_jpg_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_png_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_png_solid_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_eps_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_svg_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_idtf_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_write_gif_(uintptr_t *graph, const char *fname,const char *descr,int lf,int ld);
void mgl_start_gif_(uintptr_t *graph, const char *fname,int *ms,int l);
void mgl_close_gif_(uintptr_t *graph);

const unsigned char *mgl_get_rgb_(uintptr_t *graph);
const unsigned char *mgl_get_rgba_(uintptr_t *graph);
int mgl_get_width_(uintptr_t *graph);
int mgl_get_height_(uintptr_t *graph);
void mgl_calc_xyz_(uintptr_t *gr, int *xs, int *ys, float *x, float *y, float *z);
void mgl_calc_scr_(uintptr_t *gr, float *x, float *y, float *z, int *xs, int *ys);

int mgl_new_frame_(uintptr_t *graph);
void mgl_end_frame_(uintptr_t *graph);
int mgl_get_num_frame_(uintptr_t *graph);
void mgl_reset_frames_(uintptr_t *graph);

void mgl_set_transp_type_(uintptr_t *graph, int *type);
void mgl_set_alpha_(uintptr_t *graph, int *enable);
void mgl_set_fog_(uintptr_t *graph, float *dist, float *dz);
void mgl_set_light_(uintptr_t *graph, int *enable);
void mgl_set_light_n_(uintptr_t *gr, int *n, int *enable);
void mgl_add_light_(uintptr_t *graph, int *n, float *x, float *y, float *z, char *c, int);
void mgl_add_light(HMGL graph, int n, float x, float y, float z, char c);
void mgl_add_light_ext_(HMGL graph, int n, float x, float y, float z, char c, float br, bool inf, float ap);
void mgl_add_light_rgb_(uintptr_t *graph, int *n, float *x, float *y, float *z, char *c, float *br, int *inf, float *ap, int);
void mgl_set_ambbr_(uintptr_t *gr, float *i);

void mgl_mat_push_(uintptr_t *gr);
void mgl_mat_pop_(uintptr_t *gr);
void mgl_identity_(uintptr_t *graph, int *rel);
void mgl_clf_(uintptr_t *graph);
void mgl_clf_rgb_(uintptr_t *graph, float *r, float *g, float *b);

void mgl_subplot_(uintptr_t *graph, int *nx,int *ny,int *m);
void mgl_subplot_d_(uintptr_t *graph, int *nx,int *ny,int *m,float *dx,float *dy);
void mgl_subplot_s_(uintptr_t *graph, int *nx,int *ny,int *m, const char *s,int);
void mgl_inplot_(uintptr_t *graph, float *x1,float *x2,float *y1,float *y2);
void mgl_relplot_(uintptr_t *graph, float *x1,float *x2,float *y1,float *y2);
void mgl_columnplot_(uintptr_t *graph, int *num, int *i);
void mgl_columnplot_d_(uintptr_t *graph, int *num, int *i, float *d);
void mgl_stickplot_(uintptr_t *graph, int *num, int *i, float *tet, float *phi);
void mgl_aspect_(uintptr_t *graph, float *Ax,float *Ay,float *Az);
void mgl_rotate_(uintptr_t *graph, float *TetX,float *TetZ,float *TetY);
void mgl_rotate_vector_(uintptr_t *graph, float *Tet,float *x,float *y,float *z);
void mgl_perspective_(uintptr_t *graph, float val);
/*****************************************************************************/
#ifdef __cplusplus
}
#endif
/*****************************************************************************/
#endif
