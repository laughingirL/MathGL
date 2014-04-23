/***************************************************************************
 * data_ext.h is part of Math Graphic Library
 * Copyright (C) 2007-2014 Alexey Balakin <mathgl.abalakin@gmail.ru>       *
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
#ifndef _MGL_DATA_EXT_H_
#define _MGL_DATA_EXT_H_

#include "mgl2/data.h"
#include "mgl2/data_cf.h"
#ifdef __cplusplus
//-----------------------------------------------------------------------------
/// Class which present variable as data array
class MGL_EXPORT mglDataV : public mglDataA
{
	long nx;	///< number of points in 1st dimensions ('x' dimension)
	long ny;	///< number of points in 2nd dimensions ('y' dimension)
	long nz;	///< number of points in 3d dimensions ('z' dimension)
	mreal di, dj, dk, a0;
public:

	inline mglDataV(const mglDataV &d)	// NOTE: must be constructor for mglDataV& to exclude copy one
	{	temp=d->temp;	s=d->s;	func=d->func;	o=d->o;
		nx=d.nx;	ny=d.ny;	nz=d.nz;	a0=d.a0;
		di=d.di;	dj=d.dj;	dk=d.dk;	}
	inline mglDataV()	{	di=dj=dk=a0=0;	nx=ny=nz=1;	}
	inline mglDataV(long xx=1,long yy=1,long zz=1)
	{	nx=xx;	ny=yy;	nz=zz;	di=dj=dk=a0=0;	}
	virtual ~mglDataV()	{}

	/// Get sizes
	inline long GetNx() const	{	return nx;	}
	inline long GetNy() const	{	return ny;	}
	inline long GetNz() const	{	return nz;	}

	/// Create or recreate the array with specified size and fill it by zero
	inline void Create(long mx,long my=1,long mz=1)
	{	di=mx>1?di*(nx-1)/(mx-1):0;	dj=my>1?dj*(ny-1)/(my-1):0;
		dk=mz>1?dk*(nz-1)/(mz-1):0;	nx=mx;	ny=my;	nz=mz;	}
	/// Equidistantly fill the data to range [x1,x2] in direction dir
	inline void Fill(mreal x1,mreal x2=NaN,char dir='x')
	{
		di=dj=dk=0;	a0=x1;
		if(mgl_isnum(x2))
		{
			if(dir=='x' && nx>1)	di=(x2-x1)/(nx-1);
			if(dir=='y' && ny>1)	dj=(x2-x1)/(ny-1);
			if(dir=='z' && nz>1)	dk=(x2-x1)/(nz-1);
		}
	}
	mreal Maximal() const
	{	return a0+fmax(fmax(di*(nx-1),dj*(ny-1)),fmax(dk*(nz-1),0));	}
	mreal Minimal() const
	{	return a0+fmin(fmin(di*(nx-1),dj*(ny-1)),fmin(dk*(nz-1),0));	}

	/// Copy data from other mglDataV variable
	inline const mglDataV &operator=(const mglDataV &d)
	{	temp=d->temp;	s=d->s;	func=d->func;	o=d->o;
		nx=d.nx;	ny=d.ny;	nz=d.nz;	a0=d.a0;
		di=d.di;	dj=d.dj;	dk=d.dk;	return d;	}
	inline mreal operator=(mreal val)
	{	di=dj=dk=0;	a0=val;	return val;	}
	/// Get the value in given cell of the data without border checking
	inline mreal v(long i,long j=0,long k=0) const
	{	return a0+di*i+dj*j+dk*k;	}
	inline mreal vthr(long i) const
	{	return a0+di*(i%nx)+dj*((i/nx)%ny)+dk*(i/(nx*ny));	}
	// add for speeding up !!!
	inline mreal dvx(long i,long j=0,long k=0) const	{	return di;	}
	inline mreal dvy(long i,long j=0,long k=0) const	{	return dj;	}
	inline mreal dvz(long i,long j=0,long k=0) const	{	return dk;	}
};
//-----------------------------------------------------------------------------
/// Class which present variable as data array
class MGL_EXPORT mglDataF : public mglDataA
{
	long nx;	///< number of points in 1st dimensions ('x' dimension)
	long ny;	///< number of points in 2nd dimensions ('y' dimension)
	long nz;	///< number of points in 3d dimensions ('z' dimension)
	std::string str;	///< function as string
	mglPoint v1, v2;	///< ranges for coordinates
	HMEX ex;			///< parsed variant
	mreal dx,dy,dz;
	inline void setD()
	{
		dx = nx>1?(v2.x-v1.x)/(nx-1):0;
		dy = ny>1?(v2.y-v1.y)/(ny-1):0;
		dz = nz>1?(v2.z-v1.z)/(nz-1):0;
	}
public:

	inline mglDataF(const mglDataF &d)	// NOTE: must be constructor for mglDataF& to exclude copy one
	{	temp=d->temp;	s=d->s;	func=d->func;	o=d->o;
		nx=d.nx;	ny=d.ny;	nz=d.nz;	v1=d.v1;	v2=d.v2;
		str=d.str;	ex = mgl_create_expr(str.c_str());	setD();	}
	inline mglDataF()	{	ex=0;	v2=mglPoint(1,1,1);	nx=ny=nz=1;	setD();	}
	inline mglDataF(long xx=1,long yy=1,long zz=1)
	{	ex=0;	v2=mglPoint(1,1,1);	nx=xx;	ny=yy;	nz=zz;	setD();	}
	virtual ~mglDataF()	{	mgl_delete_expr(ex);	}

	/// Get sizes
	inline long GetNx() const	{	return nx;	}
	inline long GetNy() const	{	return ny;	}
	inline long GetNz() const	{	return nz;	}

	/// Create or recreate the array with specified size and fill it by zero
	inline void Create(long mx,long my=1,long mz=1)	{	nx=mx;	ny=my;	nz=mz;	setD();	}
	inline void SetRanges(mglPoint p1, mglPoint p2)	{	v1=p1;	v2=p2;	setD();	}
	/// Set formula and coordinates range [r1,r2]
	inline void Fill(const char *eq)
	{	mgl_delete_expr(ex);	
		if(eq && *eq)	{	ex = mgl_create_expr(eq);	str=eq;	}
		else	{	ex=0;	str="";	}	}

	/// Copy data from other mglDataV variable
	inline const mglDataF &operator=(const mglDataF &d)
	{	temp=d->temp;	s=d->s;	func=d->func;	o=d->o;
		nx=d.nx;	ny=d.ny;	nz=d.nz;	v1=d.v1;	v2=d.v2;	setD();
		str=d.str;	ex = mgl_create_expr(str.c_str());	return d;	}
	/// Get the value in given cell of the data without border checking
	inline mreal v(long i,long j=0,long k=0) const
	{	return ex?mgl_expr_eval(ex,v1.x+dx*i, v1.y+dy*j, v1.z+dz*k):0;	}
	inline mreal vthr(long i) const
	{	return ex?mgl_expr_eval(ex,v1.x+dx*(i%nx), v1.y+dy*((i/nx)%ny), v1.z+dz*(i/(nx*ny))/(nz-1)):0;	}
/*	// add for speeding up !!!
	inline mreal dvx(long i,long j=0,long k=0) const
	{	return ex?mgl_expr_diff(ex,'x',v1.x+dx*i, v1.y+dy*j, v1.z+dz*k):0;	}
	inline mreal dvy(long i,long j=0,long k=0) const
	{	return ex?mgl_expr_diff(ex,'y',v1.x+dx*i, v1.y+dy*j, v1.z+dz*k):0;	}
	inline mreal dvz(long i,long j=0,long k=0) const
	{	return ex?mgl_expr_diff(ex,'z',v1.x+dx*i, v1.y+dy*j, v1.z+dz*k):0;	}*/
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#endif
#endif
