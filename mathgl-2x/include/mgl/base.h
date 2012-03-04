/***************************************************************************
 * base.h is part of Math Graphic Library
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
#ifndef _MGL_BASE_H_
#define _MGL_BASE_H_
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifndef _MSC_VER
#include <stdint.h>
#endif
#include "mgl/define.h"
/*****************************************************************************/
#ifdef __cplusplus
#include <vector>
#include <string>
#include "mgl/type.h"
#include "mgl/eval.h"
#include "mgl/font.h"
//-----------------------------------------------------------------------------
/// Abstract class for data array
class mglDataA
{
public:
	virtual mreal v(long i,long j=0,long k=0) const = 0;
	virtual mreal vthr(long i) const = 0;
	virtual long GetNx() const = 0;
	virtual long GetNy() const = 0;
	virtual long GetNz() const = 0;
	inline long GetNN() const {	return GetNx()*GetNy()*GetNz();	}
	virtual float Maximal() const = 0;
	virtual float Minimal() const = 0;
	virtual mreal dvx(long i,long j=0,long k=0) const = 0;
//	{	return i>0 ? (i<GetNx()-1 ? (v(i+1,j,k)-v(i-1,j,k))/2 : v(i,j,k)-v(i-1,j,k)) : v(1,j,k)-v(0,j,k);	}
	virtual mreal dvy(long i,long j=0,long k=0) const = 0;
//	{	return j>0 ? (j<GetNy()-1 ? (v(i,j+1,k)-v(i,j-1,k))/2 : v(i,j,k)-v(i,j-1,k)) : v(i,1,k)-v(i,0,k);	}
	virtual mreal dvz(long i,long j=0,long k=0) const = 0;
//	{	return k>0 ? (k<GetNz()-1 ? (v(i,j,k+1)-v(i,j,k-1))/2 : v(i,j,k)-v(i,j,k-1)) : v(i,j,1)-v(i,j,0);	}
};
//-----------------------------------------------------------------------------
inline float mgl_d(float v,float v1,float v2) { return v2!=v1?(v-v1)/(v2-v1):NAN; }
//-----------------------------------------------------------------------------
mglPoint GetX(const mglDataA *x, int i, int j, int k=0);
mglPoint GetY(const mglDataA *y, int i, int j, int k=0);
mglPoint GetZ(const mglDataA *z, int i, int j, int k=0);
inline mglPoint GetX(const mglDataA &x, int i, int j, int k=0)
{	return GetX(&x,i,j,k);	}
inline mglPoint GetY(const mglDataA &y, int i, int j, int k=0)
{	return GetY(&y,i,j,k);	}
inline mglPoint GetZ(const mglDataA &z, int i, int j, int k=0)
{	return GetZ(&z,i,j,k);	}
//-----------------------------------------------------------------------------
/// Structure for simplest primitives
struct mglPrim
{
	// NOTE: n4 is used as mark; n3 -- as pen style for type=0,1,4
	// NOTE: n3 is used as position of txt,font in Ptxt for type=6
	long n1,n2,n3,n4;	///< coordinates of corners
	int type;	///< primitive type (0-point, 1-line, 2-trig, 3-quad, 4-glyph, 6-text)
	int id;		///< object id
	float z;	///< z-position
	float s;	///< size (if applicable) or fscl
	float w;	///< width (if applicable) or ftet
	float p;

	mglPrim(int t=0)	{	n1=n2=n3=n4=id=0;	z=s=w=p=0;	type = t;	}
};
bool operator<(const mglPrim &a,const mglPrim &b);
bool operator>(const mglPrim &a,const mglPrim &b);
//-----------------------------------------------------------------------------
/// Structure for group of primitives
struct mglGroup
{
	std::vector<long> p;///< list of primitives (not filled!!!)
	int Id;				///< Current list of primitives
	std::string Lbl;	///< Group label
	mglGroup(const char *lbl="", int id=0)	{	Lbl=lbl;	Id=id;	}
};
//-----------------------------------------------------------------------------
/// Structure for text label
struct mglText
{
	std::wstring text;
	std::string stl;
	float val;
	mglText(const wchar_t *txt=0, const char *fnt=0, float v=0)	{	text=txt;	stl=fnt;	val=v;	}
	mglText(const std::wstring &txt, float v=0)	{	text=txt;	val=v;	}
};
//-----------------------------------------------------------------------------
/// Structure for internal point represenatation
struct mglPnt
{
	float xx,yy,zz;	// original coordinates
	float x,y,z;	// coordinates
	float c,t;		// index in color scheme
	float u,v,w;	// normales
	float r,g,b,a;	// RGBA color
	mglPnt()	{	xx=yy=zz=x=y=z=c=t=u=v=w=r=g=b=a=0;	}
};
inline mglPnt operator+(const mglPnt &a, const mglPnt &b)
{	mglPnt c;
	c.x=a.x+b.x;	c.y=a.y+b.y;	c.z=a.z+b.z;	c.c=a.c+b.c;
	c.t=a.t+b.t;	c.u=a.u+b.u;	c.v=a.v+b.v;	c.w=a.w+b.w;
	c.r=a.r+b.r;	c.g=a.g+b.g;	c.b=a.b+b.b;	c.a=a.a+b.a;	return c;	}
inline mglPnt operator-(const mglPnt &a, const mglPnt &b)
{	mglPnt c;
	c.x=a.x-b.x;	c.y=a.y-b.y;	c.z=a.z-b.z;	c.c=a.c-b.c;
	c.t=a.t-b.t;	c.u=a.u-b.u;	c.v=a.v-b.v;	c.w=a.w-b.w;
	c.r=a.r-b.r;	c.g=a.g-b.g;	c.b=a.b-b.b;	c.a=a.a-b.a;	return c;	}
inline mglPnt operator*(const mglPnt &a, float b)
{	mglPnt c;
	c.x=a.x*b;	c.y=a.y*b;	c.z=a.z*b;	c.c=a.c*b;
	c.t=a.t*b;	c.u=a.u*b;	c.v=a.v*b;	c.w=a.w*b;
	c.r=a.r*b;	c.g=a.g*b;	c.b=a.b*b;	c.a=a.a*b;	return c;	}
inline mglPnt operator*(float b, const mglPnt &a)
{	mglPnt c;
	c.x=a.x*b;	c.y=a.y*b;	c.z=a.z*b;	c.c=a.c*b;
	c.t=a.t*b;	c.u=a.u*b;	c.v=a.v*b;	c.w=a.w*b;
	c.r=a.r*b;	c.g=a.g*b;	c.b=a.b*b;	c.a=a.a*b;	return c;	}
//-----------------------------------------------------------------------------
struct mglTexture
{
	mglColor col[514];
	long n;			///< Number of initial colors along u
	mglTexture()	{	n=0;	}
	mglTexture(const char *cols, int smooth=0,float alpha=1)
	{	n=0;	Set(cols,smooth,alpha);	}
	void Clear()	{	n=0;	}
	void Set(const char *cols, int smooth=0,float alpha=1);
	void GetC(float u,float v,mglPnt &p);
	bool IsSame(mglTexture &t);
	void GetRGBA(unsigned char *f);	// Write as BGRA for fastest export to TGA
};
//-----------------------------------------------------------------------------
const mglColor NC(-1,-1,-1);
const mglColor BC( 0, 0, 0);
const mglColor WC( 1, 1, 1);
const mglColor RC( 1, 0, 0);
long mgl_have_color(const char *stl);
//-----------------------------------------------------------------------------
/// Structure for color ID
struct mglColorID
{
	char id;
	mglColor col;
};
extern mglColorID mglColorIds[];
//-----------------------------------------------------------------------------
/// Base class for canvas which handle all basic drawing
class mglBase
{
public:
	mglBase();
	virtual ~mglBase();

	bool Stop;			///< Flag that execution should be terminated.
	mglPoint Min;		///< Lower edge of bounding box for graphics.
	mglPoint Max;		///< Upper edge of bounding box for graphics.
	std::string Mess;	///< Buffer for receiving messages
	int ObjId;			///< object id for mglPrim
	std::vector<mglGroup> Grp;	///< List of groups with names -- need for export
	std::string PlotId;	///< Id of plot for saving filename (in GLUT window for example)

	float CDef;			///< Default (current) color in texture
	float AlphaDef;		///< Default value of alpha channel (transparency)
	float BarWidth;		///< Relative width of rectangles in Bars().
	int MeshNum;		///< Set approximate number of lines in Mesh and Grid. By default (=0) it draw all lines.
	char Arrow1, Arrow2;///< Style of arrows at end and at start of curve
	long InUse;			///< Smart pointer (number of users)
	long Flag;			///< Flags for controlling drawing

	inline bool get(long fl)	{	return Flag&fl;	}
	inline void set(long fl)	{	Flag |= fl;	}
	inline void clr(long fl)	{	Flag &=~fl;	}
	inline void set(bool v,long fl)	{	Flag = v ? Flag|fl : Flag&(~fl);	}

	/// Set values of mglGraph::Min and mglGraph::Max
	inline void SetRanges(float x1, float x2, float y1, float y2, float z1=0, float z2=0, float c1=0, float c2=0)
	{	SetRanges(mglPoint(x1,y1,z1,c1),mglPoint(x2,y2,z2,c2));	}
	void SetRanges(mglPoint v1, mglPoint v2);
	/// Set values of mglGraph::Cmin and mglGraph::Cmax as minimal and maximal values of data a
	void CRange(const mglDataA &a, bool add = false, float fact=0);
	inline void CRange(float v1,float v2)	{	if(v1!=v2)	{Min.c=v1;	Max.c=v2;	RecalcCRange();}	}
	/// Set values of mglGraph::Min.x and mglGraph::Max.x as minimal and maximal values of data a
	void XRange(const mglDataA &a, bool add = false, float fact=0);
	inline void XRange(float v1,float v2)	{	if(v1!=v2)	{Min.x=v1;	Max.x=v2;	RecalcBorder();}	}
	/// Set values of mglGraph::Min.x and mglGraph::Max.x as minimal and maximal values of data a
	void YRange(const mglDataA &a, bool add = false, float fact=0);
	inline void YRange(float v1,float v2)	{	if(v1!=v2)	{Min.y=v1;	Max.y=v2;	RecalcBorder();}	}
	/// Set values of mglGraph::Min.x and mglGraph::Max.x as minimal and maximal values of data a
	void ZRange(const mglDataA &a, bool add = false, float fact=0);
	inline void ZRange(float v1,float v2)	{	if(v1!=v2)	{Min.z=v1;	Max.z=v2;	RecalcBorder();}	}
	/// Set ranges for automatic variables
	void SetAutoRanges(float x1, float x2, float y1=0, float y2=0, float z1=0, float z2=0, float c1=0, float c2=0);
	/// Set axis origin
	inline void SetOrigin(float x0, float y0, float z0=NAN, float c0=NAN)
	{	Org=mglPoint(x0,y0,z0,c0);	}
	/// Save ranges into internal variable and put parsed
	float SaveState(const char *opt);
	/// Load ranges from internal variable
	void LoadState();

	/// Safetly set the transformation formulas for coordinate.
	void SetFunc(const char *EqX, const char *EqY, const char *EqZ=0, const char *EqA=0);
	/// Set one of predefined transformation rule
	void SetCoor(int how);
	/// Safetly set the cutting off condition (formula).
	void CutOff(const char *EqCut);
	/// Set to draw Ternary axis (triangle like axis, grid and so on)
	void Ternary(int tern);

	/// Set cutting for points outside of bounding box
	inline void SetCut(bool val)	{	set(val, MGL_ENABLE_CUT);	}
	/// Set additional cutting box
	inline void SetCutBox(float x1, float y1, float z1, float x2, float y2, float z2)
	{	CutMin=mglPoint(x1,y1,z1);	CutMax=mglPoint(x2,y2,z2);	}
	inline void SetCutBox(mglPoint v1, mglPoint v2)	{	CutMin=v1;	CutMax=v2;	}

	/// Set the using of light on/off.
	virtual bool Light(bool enable)
	{	bool t=get(MGL_ENABLE_LIGHT);	set(enable,MGL_ENABLE_LIGHT);	return t;	}
	/// Set ambient light brightness
	virtual void SetAmbient(float bright=0.5);
	/// Use diffusive light (only for local light sources)
	inline void SetDifLight(bool dif)	{	set(dif,MGL_DIFFUSIVE);	}
	/// Set the transparency on/off.
	virtual bool Alpha(bool enable)
	{	bool t=get(MGL_ENABLE_ALPHA);	set(enable,MGL_ENABLE_ALPHA);	return t;	}
	/// Set default value of alpha-channel
	inline void SetAlphaDef(float val)	{	AlphaDef=val;	};
	/// Set default palette
	inline void SetPalette(const char *colors)
	{	Txt[0].Set(mgl_have_color(colors)?colors:MGL_DEF_PAL,-1);	}
	inline long GetNumPal(long id)	{	return Txt[abs(id)/256].n;	}
	/// Set default color scheme
	inline void SetDefScheme(const char *colors)
	{	Txt[1].Set(mgl_have_color(colors)?colors:"BbcyrR");	}

	/// Set number of mesh lines
	inline void SetMeshNum(int val)	{	MeshNum=val;	};
	/// Set relative width of rectangles in Bars, Barh, BoxPlot
	inline void SetBarWidth(float val)	{	BarWidth=val;	};
	/// Set size of marks
	inline void SetMarkSize(float val)	{	MarkSize=0.02*val;	}
	/// Set size of arrows
	inline void SetArrowSize(float val)	{	ArrowSize=0.03*val;	}

	/// Set warning code ant fill Message
	void SetWarn(int code, const char *who="");
	int inline GetWarn()	{	return WarnCode;	}

	virtual void StartAutoGroup (const char *)=0;
	void StartGroup(const char *name, int id);
	virtual void EndGroup()=0;	//	{	LoadState();	}
	/// Highlight next group
	inline void Highlight()	{	set(MGL_HIGHLIGHT);	}

	/// Set FontSize by size in pt and picture DPI (default is 16 pt for dpi=72)
	virtual void SetFontSizePT(float pt, int dpi=72){	FontSize = pt*27.f/dpi;	}
	/// Set FontSize by size in centimeters and picture DPI (default is 0.56 cm = 16 pt)
	inline void SetFontSizeCM(float cm, int dpi=72)	{	SetFontSizePT(cm*28.45f,dpi);	};
	/// Set FontSize by size in inch and picture DPI (default is 0.22 in = 16 pt)
	inline void SetFontSizeIN(float in, int dpi=72)	{	SetFontSizePT(in*72.27f,dpi);	};
	/// Set font typeface. Note that each mglFont instance can be used with ONLY ONE mglGraph instance at a moment of time!
	void SetFont(mglFont *f);
	/// Get current typeface. Note that this variable can be deleted at next SetFont() call!
	inline mglFont *GetFont()	{	return fnt;	}
	/// Restore font
	inline void RestoreFont()	{	fnt->Restore();	}
	/// Load font from file
	inline void LoadFont (const char *name, const char *path=NULL)
	{	fnt->Load(name,path);	};
	/// Copy font from another mglGraph instance
	inline void CopyFont(mglBase *gr)	{	fnt->Copy(gr->GetFont());	}
	/// Set default font size
	inline void SetFontSize(float val)	{	FontSize=val>0 ? val:FontSize*val;	}
	inline float GetFontSize()		{	return FontSize;	};
	inline float TextWidth(const wchar_t *text, const char *font, float size)
	{	return (size<0?-size*FontSize:size)*font_factor*fnt->Width(text,(font&&*font)?font:FontDef)/8;	}
	inline float TextHeight(const char *font, float size)
	{	return (size<0?-size*FontSize:size)*font_factor*fnt->Height(font?font:FontDef)/8; }
	inline float FontFactor()	{	return font_factor;	}
	virtual float GetRatio();
	/// Set to use or not text rotation
	inline void SetRotatedText(bool val)	{	set(val,MGL_ENABLE_RTEXT);	}
	/// Set default font style and color
	inline void SetFontDef(const char *font)	{	strncpy(FontDef, font, 31);	}
	/// Set to use or not text rotation
	inline void SetTickRotate(bool val)	{	set(val,MGL_TICKS_ROTATE);	}
	/// Set to use or not text rotation
	inline void SetTickSkip(bool val)	{	set(val,MGL_TICKS_SKIP);	}

	/// Add string to legend
	void AddLegend(const char *text,const char *style);
	void AddLegend(const wchar_t *text,const char *style);
	/// Clear saved legend string
	inline void ClearLegend()	{	Leg.clear();	}

	/// Set plot quality
	virtual void SetQuality(int qual=MGL_DRAW_NORM)	{	Quality=qual;	}
	inline int GetQuality()	{	return Quality;	}

	// ~~~~~~~~~~~~~~~~~~~~~~ Developer functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/// Add point to the pntN and return its position
	long AddPnt(mglPoint p, float c=-1, mglPoint n=mglPoint(NAN), float a=-1, int scl=1);
	long CopyNtoC(long k, float c);
	long CopyProj(long from, mglPoint p, mglPoint n);
	virtual void Reserve(long n);		///< Allocate n-cells for pntC and return current position
	/// Set to reduce accuracy of points (to reduc size of output files)
	inline void SetReduceAcc(bool val)	{	set(val, MGL_REDUCEACC);	}

//	inline long GetPos()	{	return Pnt.size()-1;	}
	inline mglPoint GetPntP(long i)
	{	const mglPnt &p=Pnt[i];	return mglPoint(p.x,p.y,p.z);	}
	inline float GetClrC(long i)	{	return Pnt[i].c;	}
	inline long GetPntNum()			{	return Pnt.size();	}
	inline mglPnt GetPnt(long i)	{	return Pnt[i];		}
	inline mglPrim &GetPrm(long i)	{	return Prm[i];		}
	inline long GetPrmNum()			{	return Prm.size();	}
	inline mglText GetPtx(long i)	{	return Ptx[i];		}
	inline long GetPtxNum()			{	return Ptx.size();	}
	inline mglTexture GetTxt(long i){	return Txt[i];		}
	inline long GetTxtNum()			{	return Txt.size();	}
	/// Scale coordinates and cut off some points
	virtual bool ScalePoint(mglPoint &p, mglPoint &n, bool use_nan=true);

	virtual float GetOrgX(char dir)=0;	///< Get Org.x (parse NAN value)
	virtual float GetOrgY(char dir)=0;	///< Get Org.y (parse NAN value)
	virtual float GetOrgZ(char dir)=0;	///< Get Org.z (parse NAN value)

	/// Get color depending on single variable z, which should be scaled if scale=true
	inline float GetC(long s,float z,bool scale = true)
	{	return s+(scale?GetA(z):(z>0?z/MGL_FLT_EPS:0));	}
	/// Get alpha value depending on single variable \a a
	float GetA(float a);
	/// Set pen/palette
	char SetPenPal(const char *stl, long *id=0);
	/// Add texture (like color scheme) and return the position of first color
	long AddTexture(const char *cols, int smooth=0);
//	inline float AddTexture(char col)	{	return AddTexture(mglColor(col));	};
	float AddTexture(mglColor col);
	inline void DefColor(mglColor col)	{	CDef = AddTexture(col);	}
	/// Set next color from palette
	float NextColor(long &id);

	virtual void mark_plot(long p, char type, float size=1)=0;
	virtual void arrow_plot(long p1, long p2, char st)=0;
	virtual void line_plot(long p1, long p2)=0;
	virtual void trig_plot(long p1, long p2, long p3)=0;
	virtual void quad_plot(long p1, long p2, long p3, long p4)=0;
	virtual void Glyph(float x, float y, float f, int style, long icode, float col)=0;
	virtual float text_plot(long p,const wchar_t *text,const char *fnt,float size=-1,float sh=0,float  col=-('k'),bool rot=true)=0;
	void vect_plot(long p1, long p2, float s=1);
	inline float mark_size()	{	return MarkSize*font_factor;	}
	inline char last_color()	{	return *last_style;	}

protected:
	mglPoint FMin;		///< Actual lower edge after transformation formulas.
	mglPoint FMax;		///< Actual upper edge after transformation formulas.
	mglPoint Org;		///< Center of axis cross section.
	int WarnCode;		///< Warning code
	std::vector<mglPnt> Pnt;	///< Internal points
	std::vector<mglPrim> Prm;	///< Primitives (lines, triangles and so on) -- need for export
	std::vector<mglPrim> Sub;	///< InPlot regions {n1=x1,n2=x2,n3=y1,n4=y2,id}
	std::vector<mglText> Ptx;	///< Text labels for mglPrim
	std::vector<mglText> Leg;	///< Text labels for legend
	std::vector<mglTexture> Txt;///< Pointer to textures
#ifdef HAVE_PTHREAD
	pthread_mutex_t mutexPnt, mutexTxt, mutexLeg;
#endif

	int TernAxis;		///< Flag that Ternary axis is used
	unsigned PDef;		///< Pen bit mask
	float pPos;			///< Current position in pen mask
	float PenWidth;		///< Pen width for further line plotting (must be >0 !!!)
//	long numT;			///< Number of textures
	float AmbBr;		///< Default ambient light brightness

	mglFont *fnt;		///< Class for printing vector text
	float FontSize;		///< The size of font for tick and axis labels
	char FontDef[32];	///< Font specification (see mglGraph::Puts). Default is Roman with align at center.
	int Quality;		///< Quality of plot (0x0-pure, 0x1-fast; 0x2-fine; 0x4 - low memory)

	mglFormula *fx;		///< Transformation formula for x direction.
	mglFormula *fy;		///< Transformation formula for y direction.
	mglFormula *fz;		///< Transformation formula for z direction.
	mglFormula *fa;		///< Transformation formula for coloring.
	mglFormula *fc;		///< Cutting off condition (formula).

	long CurrPal;		///< Current palette index
	float MarkSize;		///< The size of marks for 1D plots.
	float ArrowSize;	///< The size of arrows.
	char last_style[64];///< Last pen style
	float font_factor;	///< Font scaling factor

	virtual void LightScale()=0;			///< Scale positions of light sources

private:
	mglPoint MinS;		///< Saved lower edge of bounding box for graphics.
	mglPoint MaxS;		///< Saved upper edge of bounding box for graphics.
	float MSS, ASS, FSS, ADS, MNS, LSS;	///< Saved state
	long CSS;			///< Saved flags
	bool saved;			///< State is saved
	std::string leg_str;///< text to be save in legend

	mglPoint CutMin;	///< Lower edge of bounding box for cut off.
	mglPoint CutMax;	///< Upper edge of bounding box for cut off.

	void RecalcCRange();	///< Recalculate internal parameter for correct coloring.
	void RecalcBorder();	///< Recalculate internal parameter for correct transformation rules.
	void SetFBord(float x,float y,float z);	///< Set internal boundng box depending on transformation formula
	void ClearEq();			///< Clear the used variables for axis transformation
};
//-----------------------------------------------------------------------------
#define _Da_(d)	(*((const mglDataA *)(d)))
#define _DA_(a)	((const mglDataA *)*(a))
#define _GR_	((mglBase *)(*gr))
//-----------------------------------------------------------------------------
//#define _D_(d)	*((mglData *)*(d))
#define _DM_(a)	((mglData *)*(a))
#define _DT_	((mglData *)*d)
//-----------------------------------------------------------------------------
class mglData;
class mglParser;
typedef mglBase*  HMGL;
typedef mglData* HMDT;
typedef mglParser* HMPR;
typedef const mglDataA* HCDT;
#else
typedef void *HMGL;
typedef void *HMDT;
typedef void *HMPR;
typedef const void *HCDT;
#endif
/*****************************************************************************/
#endif
