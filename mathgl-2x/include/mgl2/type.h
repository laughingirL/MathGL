/***************************************************************************
 * type.h is part of Math Graphic Library
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
#ifndef _MGL_TYPE_H_
#define _MGL_TYPE_H_
#if !defined(_MSC_VER) && !defined(__BORLANDC__)
#include <stdint.h>
#endif
#include "mgl2/define.h"
//-----------------------------------------------------------------------------
const mreal Pi = M_PI;
const mreal NaN = NAN;
//-----------------------------------------------------------------------------
#define MGL_SET_XYZ(p,xx,yy,zz)		{p.x=(xx);p.y=(yy);p.z=(zz);}
#define MGL_SET_XY(p,xx,yy)			{p.x=(xx);p.y=(yy);p.z=0;}
#define MGL_SET_RGBA(p,rr,gg,bb,aa)	{p.r=(rr);p.g=(gg);p.b=(bb);p.a=(aa);}
#define MGL_SET_RGB(p,rr,gg,bb)		{p.r=(rr);p.g=(gg);p.b=(bb);}
//-----------------------------------------------------------------------------
/// Class for incapsulating point in space
struct mglPoint
{
	mreal x,y,z,c;
	mglPoint(mreal X=0,mreal Y=0,mreal Z=0,mreal C=0){x=X;y=Y;z=Z;c=C;}
	inline bool IsNAN()		{	return (x!=x || y!=y || z!=z || c!=c);	}
	inline mreal val(int i)	{	return (i<2 ? (i==0 ? x:y) : (i==2 ? z:c));	}
	inline mreal norm()		{	return sqrt(x*x+y*y+z*z);	}
	inline void Normalize()	{	mreal v=norm();	x/=v;	y/=v;	z/=v;	}

	inline void operator+=(const mglPoint &a)	{	x+=a.x;	y+=a.y;	z+=a.z;	c+=a.c;	}
	inline void operator-=(const mglPoint &a)	{	x-=a.x;	y-=a.y;	z-=a.z;	c-=a.c;	}
	inline void operator+=(mreal a)	{	x+=a;	y+=a;	z+=a;	}
	inline void operator-=(mreal a)	{	x-=a;	y-=a;	z-=a;	}
	inline void operator*=(mreal a)	{	x*=a;	y*=a;	z*=a;	}
	inline void operator/=(mreal a)	{	x/=a;	y/=a;	z/=a;	}
};
#ifndef SWIG
inline mglPoint operator+(const mglPoint &a, const mglPoint &b)
{	return mglPoint(a.x+b.x, a.y+b.y, a.z+b.z, a.c+b.c);	}
inline mglPoint operator-(const mglPoint &a, const mglPoint &b)
{	return mglPoint(a.x-b.x, a.y-b.y, a.z-b.z, a.c-b.c);	}
inline mglPoint operator-(const mglPoint &a)
{	return mglPoint(-a.x, -a.y, -a.z, -a.c);	}
inline mglPoint operator*(mreal b, const mglPoint &a)
{	return mglPoint(a.x*b, a.y*b, a.z*b);	}
inline mglPoint operator*(const mglPoint &a, mreal b)
{	return mglPoint(a.x*b, a.y*b, a.z*b);	}
inline mglPoint operator/(const mglPoint &a, mreal b)
{	return mglPoint(a.x/b, a.y/b, a.z/b);	}
inline mreal operator*(const mglPoint &a, const mglPoint &b)
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
{	mreal f=hypot(a.x,a.y);	return f==0?mglPoint(0.,1.,0.):mglPoint(-a.y/f, a.x/f, 0);	}
inline bool operator==(const mglPoint &a, const mglPoint &b)
{	return !memcmp(&a, &b, sizeof(mglPoint));	}
inline bool operator!=(const mglPoint &a, const mglPoint &b)
{	return memcmp(&a, &b, sizeof(mglPoint));	}
inline bool operator<(const mglPoint &a, const mglPoint &b)
{	return a.x<=b.x && a.y<=b.y && a.z<=b.z;	}
inline bool operator>(const mglPoint &a, const mglPoint &b)
{	return a.x>=b.x && a.y>=b.y && a.z>=b.z;	}
inline mreal mgl_norm(const mglPoint &p)
{	return sqrt(p.x*p.x+p.y*p.y+p.z*p.z);	}
#endif
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
	void Set(float R,float G,float B,float A=1)	{	r=R;	g=G;	b=B;	a=A;	}
	/// Set color as Red, Green, Blue values
	void Set(mglColor c, float bright=1);
	/// Check if color is valid
	inline bool Valid()
	{	return (r>=0 && r<=1 && g>=0 && g<=1 && b>=0 && b<=1 && a>=0 && a<=1);	}
	/// Get maximal spectral component
	inline float Norm()
	{	return r>g ? r : (g>b ? g : b);	}
	inline float NormS()
	{	return r*r+g*g+b*b;	}
	/// Set color from symbolic id
	void Set(char p, float bright=1);
	/// Copy color from other one
	inline bool operator==(const mglColor &c) const
	{	return !memcmp(this, &c, sizeof(mglColor));	}
	inline bool operator!=(const mglColor &c) const
	{	return memcmp(this, &c, sizeof(mglColor));		}
	inline bool operator<(const mglColor &c) const
	{	return memcmp(this, &c, sizeof(mglColor))<0;	}
	// transparency still the same
	inline void operator*=(float v)				{	r*=v;	g*=v;	b*=v;	}
	inline void operator+=(const mglColor &c)	{	r+=c.r;	g+=c.g;	b+=c.b;	}
	inline void operator-=(const mglColor &c)	{	r-=c.r;	g-=c.g;	b-=c.b;	}
};
#ifndef SWIG
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
#endif
//-----------------------------------------------------------------------------
#endif
