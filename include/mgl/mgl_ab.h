/* mgl_ab.h is part of Math Graphic Library
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
#ifndef _MGL_AB_H_
#define _MGL_AB_H_
#include "mgl/mgl.h"
//-----------------------------------------------------------------------------
/// Class implement the creation of different mathematical plots using Z-ordering
class mglGraphAB : public mglGraph
{
public:
	/// Initialize ZBuffer drawing and allocate the memory for image with size [Width x Height].
	mglGraphAB(int w=600, int h=400);
	~mglGraphAB();
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Служебные ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void WriteEPS(const char *fname,const char *descr=0);
	virtual void WriteSVG(const char *fname,const char *descr=0);
	virtual int NewFrame(int id=0);
	virtual void EndFrame();

	void SetFontSizePT(float pt, int dpi=72);
	void Alpha(bool enable)	{UseAlpha=enable;};
	void Light(bool enable)	{UseLight=enable;};
	void Light(int n, bool enable);
	void Light(int n,mglPoint p, mglColor c=WC, float br=0.5, bool infty=true);

	void InPlot(float x1,float x2,float y1,float y2);
	void Aspect(float Ax,float Ay,float Az);
	void RotateN(float Tet,float x,float y,float z);
	void Perspective(float a);

//	virtual void Ball(float x,float y,float z,mglColor col=RC,float alpha=1)=0;
	virtual void SetSize(int w,int h);

	void Mark(float x,float y,float z,char mark='.');
	void Glyph(float x,float y, float f, int nt, const short *trig, int nl, const short *line);
	float GetRatio()	{	return B1[0]/B1[4];	};
	void Putsw(mglPoint p,const wchar_t *text,const char *font=0,float size=-1,char dir=0,float shift=0);
	float Putsw(mglPoint p,mglPoint l,const wchar_t *text,char font='t',float size=-1);
	void Pen(mglColor col, char style,float width);

	void Colorbar(const char *sch=0,int where=0);
	/// Get RGB bitmap of current state image.
	virtual const unsigned char *GetBits();
	/// Get RGBA bitmap of current state image.
	const unsigned char *GetRGBA(){	Finish();	return G4;	};
	/// Get width of the image
	int GetWidth()	{	return Width;	};
	/// Get height of the image
	int GetHeight()	{	return Height;	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
protected:
	unsigned char *G4;			///< Final picture in RGBA format. Prepared after calling mglGraphZB::Finish().
	unsigned char *G;			///< Final picture in RGB format. Prepared after calling mglGraphZB::Finish().
	float CDef[4];				///< Default color
	unsigned char BDef[4];		///< Background color
	float Persp;		///< Perspective factor (=0 is perspective off)
	int xPos;			///< Shifting plot in X-direction (used by PostScale() function)
	int yPos;			///< Shifting plot in Y-direction (used by PostScale() function)
	int zPos;			///< Shifting plot in depth (used by PostScale() function)
	int Width;			///< Width of the image
	int Height;			///< Height of the image
	int Depth;			///< Depth of the image
	float B[9];			///< Transformation matrix (used by PostScale() function)
	float B1[9];		///< Transformation matrix for colorbar
	unsigned PDef;		///< Pen bit mask
	float pPos;			///< Current position in pen mask
	bool UseLight;		///< Flag of using lightning
	bool nLight[10];	///< Availability of light sources
	float rLight[30];	///< Position of light sources
	float pLight[30];	///< Actual position of light sources (filled by PostScale() function)
	float aLight[10];	///< Aperture of light sources
	float bLight[10];	///< Brightness of light sources
	float cLight[30];	///< Color of light sources
	bool Finished;		///< Flag that final picture \a mglGraphZB::G is ready
	float PenWidth;		///< Pen width for further line plotting (must be >0 !!!)
	bool NoAutoFactor;	///< Temporary variable

	unsigned char **GetRGBLines(long &w, long &h, unsigned char *&f, bool solid=true);
	virtual void ball(float *p,float *c)=0;
	/// Additionally scale points \a p (array with length 3*n) for positioning in image
	void PostScale(float *p,long n,bool full=true);
	/// Additionally scale normals \a s (array with length 3*n)
	void NormScale(float *s,long n);
	/// Set default color
	void DefColor(mglColor c, float alpha=-1);

	/// Draw line between points \a p1,\a p2 with color \a c1, \a c2 at edges
	virtual void line_plot(float *p1,float *p2,float *c1,float *c2,bool all=false)=0;
	/// Draw triangle between points \a p0,\a p1,\a p2 with color \a c0, \a c1, \a c2 at edges
	virtual void trig_plot(float *p0,float *p1,float *p2,float *c0,float *c1,float *c2)=0;
	/// Draw triangle between points \a p0,\a p1,\a p2 with color \a c0, \a c1, \a c2 at edges
	virtual void trig_plot_n(float *p0,float *p1,float *p2,float *c0,float *c1,float *c2,
					float *n0,float *n1,float *n2)=0;
	/// Draw face of points \a p0,\a p1,\a p2,\a p3 with color \a c0, \a c1, \a c2, \a c3 at edges
	virtual void quad_plot(float *p0,float *p1,float *p2,float *p3,
					float *c0,float *c1,float *c2,float *c3)=0;
	/// Draw face of points \a p0,\a p1,\a p2,\a p3 with values \a a0, \a a1, \a a2, \a a3 at edges
	virtual void quad_plot_a(float *p0,float *p1,float *p2,float *p3,
					float a0,float a1,float a2,float a3,float alpha)=0;
	/// Draw face of points \a p0,\a p1,\a p2,\a p3 with color \a c0, \a c1, \a c2, \a c3 at edges
	virtual void quad_plot_n(float *p0,float *p1,float *p2,float *p3,
					float *c0,float *c1,float *c2,float *c3,
					float *n0,float *n1,float *n2,float *n3)=0;
	/// Draw mark at position \a pp with style \a type
	virtual void mark_plot(float *pp, char type)=0;
	/// Combine colors in 2 plane.
	void combine(unsigned char *c1,unsigned char *c2);	// смешение цветов
	virtual void cloud_plot(long nx,long ny,long nz,float *pp,float *a,float alpha);
	// рисование примитивов для mglGraph
	void arrow_plot(float *p1,float *p2,char st);
	void curv_plot(long n,float *pp,bool *tt);
	void curv_plot(long n,float *pp,bool *tt,long *nn);
	void curv_plot(long n,float *pp,float *cc,bool *tt);
	void mesh_plot(long n,long m,float *pp,float *cc,bool *tt,int how);
	void wire_plot(long n,long m,float *pp,float *cc,bool *tt);
	void surf_plot(long n,long m,float *pp,float *cc,bool *tt);
	void axial_plot(long n,float *pp,long *nn,long np, bool wire);
	void boxs_plot(long n, long m, float *pp, mglColor *cc,
				bool *tt,float Alpha, bool line);
	void surf3_plot(long n,long m,long *kx1,long *kx2,long *ky1,long *ky2,long *kz,
							float *pp,float *cc,float *kk,float *nn,bool wire);
	void trigs_plot(long n, long *nn, long m, float *pp, float *cc, bool *tt,bool wire);
	void quads_plot(long n, float *pp, float *cc, bool *tt);
	void lines_plot(long n, float *pp, float *cc, bool *tt);
	float GetOrgX();
	float GetOrgY();
	float GetOrgZ();
private:
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
