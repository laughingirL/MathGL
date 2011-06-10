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
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mgl/define.h"
#include "mgl/eval.h"
#include "mgl/font.h"
/*****************************************************************************/
#ifdef __cplusplus
//-----------------------------------------------------------------------------
/// Class for incapsulating point in space
struct mglPoint
{
	float x,y,z,c;
 	mglPoint(float X=0,float Y=0,float Z=0,float C=0){x=X;y=Y;z=Z;c=C;}
	inline bool IsNAN()		{	return (x!=x || y!=y || z!=z || c!=c);	}
	inline float val(int i)	{	return (i<2 ? (i==0 ? x:y) : (i==2 ? z:c));	}

	inline void operator+=(const mglPoint &a)	{	x+=a.x;	y+=a.y;	z+=a.z;	}
	inline void operator-=(const mglPoint &a)	{	x-=a.x;	y-=a.y;	z-=a.z;	}
	inline void operator+=(float a)	{	x+=a;	y+=a;	z+=a;	}
	inline void operator-=(float a)	{	x-=a;	y-=a;	z-=a;	}
	inline void operator*=(float a)	{	x*=a;	y*=a;	z*=a;	}
	inline void operator/=(float a)	{	x/=a;	y/=a;	z/=a;	}
};
inline mglPoint operator+(const mglPoint &a, const mglPoint &b)
{	return mglPoint(a.x+b.x, a.y+b.y, a.z+b.z, a.c+b.c);	}
inline mglPoint operator-(const mglPoint &a, const mglPoint &b)
{	return mglPoint(a.x-b.x, a.y-b.y, a.z-b.z, a.c-b.c);	}
inline mglPoint operator*(float b, const mglPoint &a)
{	return mglPoint(a.x*b, a.y*b, a.z*b);	}
inline mglPoint operator*(const mglPoint &a, float b)
{	return mglPoint(a.x*b, a.y*b, a.z*b);	}
inline mglPoint operator/(const mglPoint &a, float b)
{	return mglPoint(a.x/b, a.y/b, a.z/b);	}
inline float operator*(const mglPoint &a, const mglPoint &b)
{	return a.x*b.x+a.y*b.y+a.z*b.z;	}
inline mglPoint operator/(const mglPoint &a, const mglPoint &b)
{	return mglPoint(a.x*b.x, a.y*b.y, a.z*b.z);	}
inline mglPoint operator&(const mglPoint &a, const mglPoint &b)
{	return a - b*((a*b)/(b*b));	}
inline mglPoint operator|(const mglPoint &a, const mglPoint &b)
{	return b*((a*b)/(b*b));	}
inline mglPoint operator^(const mglPoint &a, const mglPoint &b)
{	return mglPoint(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x);	}
inline mglPoint operator!(const mglPoint &a)
{	return (a.x==0 && a.y==0)?mglPoint(0,1,0):mglPoint(-a.y/hypot(a.x,a.y), a.x/hypot(a.x,a.y), 0);	}
inline bool operator==(const mglPoint &a, const mglPoint &b)
{	return !memcmp(&a, &b, sizeof(mglPoint));	}
inline bool operator!=(const mglPoint &a, const mglPoint &b)
{	return memcmp(&a, &b, sizeof(mglPoint));	}
inline float Norm(const mglPoint &p)
{	return sqrt(p.x*p.x+p.y*p.y+p.z*p.z);	}
//-----------------------------------------------------------------------------
/// Abstract class for data array
class mglDataA
{
public:
	virtual float v(long i,long j=0,long k=0) const = 0;
	virtual float vthr(long i) const = 0;
	virtual long GetNx() const = 0;
	virtual long GetNy() const = 0;
	virtual long GetNz() const = 0;
	inline long GetNN() const {	return GetNx()*GetNy()*GetNz();	}
	virtual float Maximal() const = 0;
	virtual float Minimal() const = 0;
	inline float dvx(long i,long j=0,long k=0) const
	{	return i>0 ? (i<GetNx()-1 ? (v(i+1,j,k)-v(i-1,j,k))/2 : v(i,j,k)-v(i-1,j,k)) : v(1,j,k)-v(0,j,k);	}
	inline float dvy(long i,long j=0,long k=0) const
	{	return j>0 ? (j<GetNy()-1 ? (v(i,j+1,k)-v(i,j-1,k))/2 : v(i,j,k)-v(i,j-1,k)) : v(i,1,k)-v(i,0,k);	}
	inline float dvz(long i,long j=0,long k=0) const
	{	return k>0 ? (k<GetNz()-1 ? (v(i,j,k+1)-v(i,j,k-1))/2 : v(i,j,k)-v(i,j,k-1)) : v(i,j,1)-v(i,j,0);	}


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
/// Class for incapsulating color
struct mglColor
{
	float r;	///< Red component of color
	float g;	///< Green component of color
	float b;	///< Blue component of color
	float a;	///< Alpha component of color

	/// Constructor for RGB components manualy
	mglColor(float R,float G,float B, float A=1){	r=R;	g=G;	b=B;	a=A;	}
	/// Constructor set color from character id
	mglColor(char c='k', float bright=1)		{	Set(c,bright);	}
	/// Set color as Red, Green, Blue values
	void Set(float R,float G,float B,float A=1)	{r=R;	g=G;	b=B;	a=A;	}
	/// Set color as Red, Green, Blue values
	void Set(mglColor c, float bright=1);
	/// Check if color is valid
	inline bool Valid()
	{	return (r>=0 && r<=1 && g>=0 && g<=1 && b>=0 && b<=1 && a>=0 && a<=1);	}
	/// Get maximal spectral component
	inline float Norm()
	{	return r>g ? r : (g>b ? g : b);	}
	/// Set color from symbolic id
	void Set(char p, float bright=1);
	/// Copy color from other one
	inline bool operator==(const mglColor &c)
	{	return (r==c.r && g==c.g && b==c.b && a==c.a);	}
	inline bool operator!=(const mglColor &c)
	{	return (r!=c.r || g!=c.g || b!=c.b || a!=c.a);	}
};
inline mglColor operator+(const mglColor &a, const mglColor &b)
{	return mglColor(a.r+b.r, a.g+b.g, a.b+b.b, a.a+b.a);	}
inline mglColor operator-(const mglColor &a, const mglColor &b)
{	return mglColor(a.r-b.r, a.g-b.g, a.b-b.b, a.a-b.a);	}
inline mglColor operator*(const mglColor &a, float b)
{	return mglColor(a.r*b, a.g*b, a.b*b, a.a*b);	}
inline mglColor operator*(float b, const mglColor &a)
{	return mglColor(a.r*b, a.g*b, a.b*b, a.a*b);	}
inline mglColor operator/(const mglColor &a, float b)
{	return mglColor(a.r/b, a.g/b, a.b/b, a.a/b);	}
inline mglColor operator!(const mglColor &a)
{	return mglColor(1-a.r, 1-a.g, 1-a.b, a.a);	}
//-----------------------------------------------------------------------------
struct mglTexture
{
	mglColor col[514];
	long n;			///< Number of initial colors along u
	mglTexture()	{	memset(this,0,sizeof(mglTexture));	}
	~mglTexture()	{	Clear();	}
	void Clear()	{	memset(this,0,sizeof(mglTexture));	}
//	void Set(int nn, mglColor *cc, float a=0);
	void Set(const char *cols, int smooth=0,float alpha=1);
	void GetC(float u,float v,float c[4]);
	bool IsSame(mglTexture &t);
private:
};
//-----------------------------------------------------------------------------
const mglColor NC(-1,-1,-1);
const mglColor BC( 0, 0, 0);
const mglColor WC( 1, 1, 1);
const mglColor RC( 1, 0, 0);
//-----------------------------------------------------------------------------
/// Structure for color ID
struct mglColorID
{
	char id;
	mglColor col;
};
extern mglColorID mglColorIds[];
//-----------------------------------------------------------------------------
class mglBase
{
public:
	mglBase();
	virtual ~mglBase();

	mglPoint Min;		///< Lower edge of bounding box for graphics.
	mglPoint Max;		///< Upper edge of bounding box for graphics.
	bool Cut;			///< Flag which determines how points outside bounding box are drown.
	char *Message;		///< Buffer for receiving messages
	int ObjId;			///< object id for mglPrim

	float CDef;			///< Default (current) color in texture
	float AlphaDef;		///< Default value of alpha channel (transparency)
	float BarWidth;		///< Relative width of rectangles in Bars().
	int MeshNum;		///< Set approximate number of lines in Mesh and Grid. By default (=0) it draw all lines.
	char Arrow1, Arrow2;///< Style of arrows at end and at start of curve
	long InUse;			///< Smart pointer (number of users)

	/// Set values of mglGraph::Min and mglGraph::Max
	inline void SetRanges(float x1, float x2, float y1, float y2, float z1=0, float z2=0, float c1=0, float c2=0)
	{	SetRanges(mglPoint(x1,y1,z1,c1),mglPoint(x2,y2,z2,c2));	}
	void SetRanges(mglPoint v1, mglPoint v2);
	/// Set values of mglGraph::Cmin and mglGraph::Cmax as minimal and maximal values of data a
	void CRange(const mglDataA &a, bool add = false, float fact=0);
	inline void CRange(float v1,float v2)	{	Min.c=v1;	Max.c=v2;	RecalcCRange();	}
	/// Set values of mglGraph::Min.x and mglGraph::Max.x as minimal and maximal values of data a
	void XRange(const mglDataA &a, bool add = false, float fact=0);
	inline void XRange(float v1,float v2)	{	Min.x=v1;	Max.x=v2;	RecalcBorder();	}
	/// Set values of mglGraph::Min.x and mglGraph::Max.x as minimal and maximal values of data a
	void YRange(const mglDataA &a, bool add = false, float fact=0);
	inline void YRange(float v1,float v2)	{	Min.y=v1;	Max.y=v2;	RecalcBorder();	}
	/// Set values of mglGraph::Min.x and mglGraph::Max.x as minimal and maximal values of data a
	void ZRange(const mglDataA &a, bool add = false, float fact=0);
	inline void ZRange(float v1,float v2)	{	Min.z=v1;	Max.z=v2;	RecalcBorder();	}
	/// Set ranges for automatic variables
	void SetAutoRanges(float x1, float x2, float y1=0, float y2=0, float z1=0, float z2=0, float c1=0, float c2=0);
	/// Set axis origin
	inline void SetOrigin(float x0, float y0, float z0=NAN, float c0=NAN)
	{	Org=mglPoint(x0,y0,z0,c0);	}
	/// Save ranges into internal variable and put parsed
	float SaveState(const char *opt);
	/// Return previous value of SaveState()
	inline float PrevValue()	{	return prev_val;	};
	/// Load ranges from internal variable
	void LoadState();

	/// Safetly set the transformation formulas for coordinate.
	void SetFunc(const char *EqX, const char *EqY, const char *EqZ=0, const char *EqA=0);
	/// Set the predefined transformation rules
	void SetCoor(int how);
	/// Safetly set the cutting off condition (formula).
	void CutOff(const char *EqCut);
	/// Set to draw Ternary axis (triangle like axis, grid and so on)
	void Ternary(int tern);

	/// Set cutting for points outside of bounding box
	inline void SetCut(bool val)	{	Cut=val;	}
	/// Set additional cutting box
	inline void SetCutBox(float x1, float y1, float z1, float x2, float y2, float z2)
	{	CutMin=mglPoint(x1,y1,z1);	CutMax=mglPoint(x2,y2,z2);	}
	inline void SetCutBox(mglPoint v1, mglPoint v2)	{	CutMin=v1;	CutMax=v2;	}

	/// Set ambient light brightness
	virtual void SetAmbient(float bright=0.5);
	/// Set default value of alpha-channel
	inline void SetAlphaDef(float val)	{	AlphaDef=val;	};
	/// Set default palette
	inline void SetPalette(const char *colors)
	{	txt[0].Set((colors && *colors)?colors:MGL_DEF_PAL,-1);	}
	inline long GetNumPal(long id)	{	return txt[abs(id)/256].n;	}
	/// Set default color scheme
	inline void SetDefScheme(const char *colors)
	{	txt[1].Set((colors && *colors)?colors:"BbcyrR");	}

	/// Set number of mesh lines
	inline void SetMeshNum(int val)	{	MeshNum=val;	};
	/// Set relative width of rectangles in Bars, Barh, BoxPlot
	inline void SetBarWidth(float val)	{	BarWidth=val;	};
	/// Set size of marks
	inline void SetMarkSize(float val)	{	MarkSize=val;	}
	/// Set size of arrows
	inline void SetArrowSize(float val)	{	ArrowSize=val;	}

	/// Set warning code ant fill Message
	void SetWarn(int code, const char *who="");
	int inline GetWarn()	{	return WarnCode;	}

	virtual void StartGroup (const char *){}
	virtual void StartAutoGroup (const char *){}
	void StartGroup(const char *name, int id);
	virtual void EndGroup()	{	LoadState();	}

	/// Set FontSize by size in pt and picture DPI (default is 16 pt for dpi=72)
	virtual void SetFontSizePT(float pt, int dpi=72){	FontSize = pt*27.f/dpi;	}
	/// Set FontSize by size in centimeters and picture DPI (default is 0.56 cm = 16 pt)
	inline void SetFontSizeCM(float cm, int dpi=72)	{	SetFontSizePT(cm*28.45f,dpi);	};
	/// Set FontSize by size in inch and picture DPI (default is 0.22 in = 16 pt)
	inline void SetFontSizeIN(float in, int dpi=72)	{	SetFontSizePT(in*72.27f,dpi);	};
	/// Set font typeface. Note that each mglFont instance can be used with ONLY ONE mglGraph instance at a moment of time!
	void SetFont(mglFont *f);
	/// Get current typeface. Note that this variable can be deleted at next SetFont() call!
	inline mglFont *GetFont()	{	return fnt;	};
	/// Restore font
	inline void RestoreFont()	{	fnt->Restore();	};
	/// Load font from file
	inline void LoadFont (const char *name, const char *path=NULL)
	{	fnt->Load(name,path);	};
	/// Copy font from another mglGraph instance
	inline void CopyFont(mglBase *gr)	{	fnt->Copy(gr->GetFont());	};
	/// Set default font size
	inline void SetFontSize(float val)	{	FontSize=val>0 ? val:FontSize*val;	};
	inline float GetFontSize()		{	return FontSize;	};
	inline float TextWidth(const wchar_t *text)	{	return fnt->Width(text,FontDef);	};
	inline float TextHeight()	{	return fnt->Height(FontDef); };
	/// Set to use or not text rotation
	inline void SetRotatedText(bool val)	{	RotatedText=val;	};
	/// Set default font style and color
	inline void SetFontDef(const char *fnt)	{	strncpy(FontDef, fnt, 31);	};

	/// Set plot quality
	virtual void SetQuality(int qual=MGL_DRAW_NORM)	{	Quality=qual;	}
	inline int GetQuality()	{	return Quality;	}

	// ~~~~~~~~~~~~~~~~~~~~~~ Developer functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// NOTE! Following 3 functions are NOT thread-safe!!!
	/// Add point to the pntN and return its position
	long AddPnt(mglPoint p, float c=-1, mglPoint n=mglPoint(NAN), float a=-1, int scl=1);
	long CopyNtoC(long k, float c);
	long CopyProj(long from, mglPoint p, mglPoint n);	// NOTE: this is not-thread-safe!!!
	long Reserve(long n);		///< Allocate n-cells for pntC and return current position

	inline long GetPos()	{	return pos;	}
	inline mglPoint GetPnt(long i)
	{	return mglPoint(pnt[12*i],pnt[12*i+1],pnt[12*i+2]);	}
	inline float GetClrC(long i)
	{	return pnt[12*i+3];	}
	/// Scale coordinates and cut off some points
	virtual bool ScalePoint(mglPoint &p, mglPoint &n, bool use_nan=true);

	virtual float GetOrgX(char dir)=0;	///< Get Org.x (parse NAN value)
	virtual float GetOrgY(char dir)=0;	///< Get Org.y (parse NAN value)
	virtual float GetOrgZ(char dir)=0;	///< Get Org.z (parse NAN value)

	/// Get color depending on single variable z, which should be scaled if scale=true
	inline float GetC(long s,float z,bool scale = true)
	{	return s+(scale?GetA(z):z);	}
	/// Get alpha value depending on single variable \a a
	float GetA(float a);
	/// Set pen/palette
	char SetPenPal(const char *stl, long *id=0);
	/// Add texture (like color scheme) and return the position of first color
	long AddTexture(const char *cols, int smooth=0);
	float AddTexture(char col);
	/// Set next color from palette
	float NextColor(long &id);

	virtual void mark_plot(long p, char type, float size=1)=0;		// position in pntC
	virtual void arrow_plot(long p1, long p2, char st)=0;			// position in pntC
	virtual void line_plot(long p1, long p2)=0;						// position in pntC
	virtual void trig_plot(long p1, long p2, long p3)=0;			// position in pntN
	virtual void quad_plot(long p1, long p2, long p3, long p4)=0;	// position in pntN
	virtual void Glyph(float x, float y, float f, int style, long icode, char col)=0;
	virtual float text_plot(long p,const wchar_t *text,const char *fnt,float size=-1,float sh=0)=0;	// position in pntN
	void vect_plot(long p1, long p2);	// position in pntC

protected:
	mglPoint FMin;		///< Actual lower edge after transformation formulas.
	mglPoint FMax;		///< Actual upper edge after transformation formulas.
	mglPoint Org;		///< Center of axis cross section.
	int WarnCode;		///< Warning code
	float *pnt;			///< Pointer to {coor*3,texture*2,normale*3,color*4} of the data
	long num;			///< Number of allocated points in pnt
	long pos;			///< Current position in pnt
	int TernAxis;		///< Flag that Ternary axis is used
	unsigned PDef;		///< Pen bit mask
	float pPos;			///< Current position in pen mask
	float PenWidth;		///< Pen width for further line plotting (must be >0 !!!)
	mglTexture *txt;	///< Pointer to textures
	long numT;			///< Number of textures
	float AmbBr;		///< Default ambient light brightness

	mglFont *fnt;		///< Class for printing vector text
	float FontSize;		///< The size of font for tick and axis labels
	char FontDef[32];	///< Font specification (see mglGraph::Puts). Default is Roman with align at center.
	bool RotatedText;	///< Use text rotation along axis
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

	virtual void LightScale()=0;			///< Scale positions of light sources
	virtual void SetPen(char style,float width);

private:
	mglPoint MinS;		///< Saved lower edge of bounding box for graphics.
	mglPoint MaxS;		///< Saved upper edge of bounding box for graphics.
	float MSS, ASS, FSS, ADS, MNS, CSS, LSS;	///< Saved state
	bool saved;			///< State is saved
	float prev_val;		///< previous value or zero (if no one)

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
typedef mglBase*  HMGL;
typedef mglData* HMDT;
typedef const mglDataA* HCDT;
#else
typedef void *HMGL;
typedef void *HMDT;
typedef const void *HCDT;
#endif
/*****************************************************************************/
#endif
