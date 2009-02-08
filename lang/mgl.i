/***************************************************************************
 * mgl.i is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
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

%module mathgl

%{
#include <math.h>
#include "mgl/mgl_c.h"

const float NaN = NAN;
const float pi = M_PI;
const int on = 1;
const int off = 0;
%}

typedef mglGraph *HMGL;
typedef mglData *HMDT;
typedef mglParse *HMPR;

const float NaN = NAN;
const float pi = M_PI;
const int on = 1;
const int off = 0;

#idef SWIGOCTAVE
%feature("autodoc", 1);
#end // SWIGOCTAVE

#include "mgl/mgl_c.h"
/* %include mgl_base.i */
/* %include mgl_data.i */

%ignore operator=;
%ignore *::operator=;
%ignore operator!;
%ignore operator*(float, const mglPoint &);

%rename(Mul) operator*(const mglPoint &, float);
%rename(Div) operator/(const mglPoint &, float);
%rename(__add__) operator+(const mglPoint&, const mglPoint&);
%rename(__sub__) operator-(const mglPoint &, const mglPoint &);
%rename(__mul__) operator*(const mglPoint &, const mglPoint &);
%rename(__div__) operator/(const mglPoint &, const mglPoint &);
%rename(__eq__) operator==(const mglPoint &, const mglPoint &);
%rename(__ne__) operator!=(const mglPoint &, const mglPoint &);
%rename(__ror__) operator|(const mglPoint &, const mglPoint &);
%rename(__rand__) operator&(const mglPoint &, const mglPoint &);
%rename(__rxor__) operator^(const mglPoint &, const mglPoint &);

%{
#include "mgl/mgl_data.h"
%}


%include mgl/mgl_data.h

%extend mglData
{
	float __getitem__( int i)	{	return self->a[i];	};
	void __setitem__( int i, float y)	{	self->a[i]=y;	};
};

%include mgl_graph.i
