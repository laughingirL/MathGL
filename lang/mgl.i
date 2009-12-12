/***************************************************************************
 * mgl.i is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>, 
 *   Xavier Delacour <xavier.delacour@gmail.com>, 
 *   Alexander Filov <alexander.filov@gmail.com>                           *
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
#define MGL_NO_WIDGET
#ifdef SWIGOCTAVE
%feature("autodoc", 1);
#endif // SWIGOCTAVE

%ignore operator!;
%ignore operator=;
%ignore *::operator=;
%ignore *::operator+=;
%ignore *::operator-=;
%ignore *::operator*=;
%ignore *::operator/=;

%{
#define SWIG_FILE_WITH_INIT
//#include "mgl/mgl_c.h"
#include "mgl/mgl_w.h"
%}

#ifdef SWIGOCTAVE
%rename(__add) operator+;
%rename(__sub) operator-;
%rename(__mul) operator*;
%rename(__div) operator/;
%rename(__eq) operator==;
%rename(__ne) operator!=;
//%typemap(in,noblock=1) (double* d, int rows, int cols) (Matrix tmp) {
//	if (!$input.is_matrix_type()) {
//		error("A must be a matrix");
//	SWIG_fail;
//	}
//	tmp=$input.matrix_value();
//	$1=tmp.data();
//	$2=tmp.rows();
//	$3=tmp.columns();
//}
#endif

#ifdef SWIGPYTHON
%rename(__add__) *::operator+(const mglData&, const mglData&);
%rename(__sub__) *::operator-(const mglData&, const mglData &);
%rename(__mul__) *::operator*(const mglData &, float);
%rename(__div__) *::operator/(const mglData &, float);

//--- NumPy interface by Alexander Filov ---->
// Get the NumPy typemaps
%include "numpy.i"

%init %{
import_array();
%}

%define %apply_numpy_typemaps(TYPE)

%apply (TYPE IN_ARRAY1[ANY]) {(TYPE vector[3])};
%apply (TYPE* IN_ARRAY1, int DIM1) {(TYPE* series, int size)};
%apply (int DIM1, TYPE* IN_ARRAY1) {(int size, TYPE* series)};

%apply (TYPE INPLACE_ARRAY1[ANY]) {(TYPE array[3])};
%apply (TYPE* INPLACE_ARRAY1, int DIM1) {(TYPE* array, int size)};
%apply (int DIM1, TYPE* INPLACE_ARRAY1) {(int size, TYPE* array)};

%apply (TYPE ARGOUT_ARRAY1[ANY]) {(TYPE even[3])};
%apply (TYPE ARGOUT_ARRAY1[ANY]) {(TYPE odd[ 3])};
%apply (TYPE* ARGOUT_ARRAY1, int DIM1) {(TYPE* twoVec, int size)};
%apply (int DIM1, TYPE* ARGOUT_ARRAY1) {(int size, TYPE* threeVec)};

%apply (TYPE IN_ARRAY2[ANY][ANY]) {(TYPE matrix[ANY][ANY])};
%apply (TYPE* IN_ARRAY2, int DIM1, int DIM2) {(TYPE* matrix, int rows, int cols)};
%apply (int DIM1, int DIM2, TYPE* IN_ARRAY2) {(int rows, int cols, TYPE* matrix)};

%apply (TYPE INPLACE_ARRAY2[ANY][ANY]) {(TYPE array[3][3])};
%apply (TYPE* INPLACE_ARRAY2, int DIM1, int DIM2) {(TYPE* array, int rows, int cols)};
%apply (int DIM1, int DIM2, TYPE* INPLACE_ARRAY2) {(int rows, int cols, TYPE* array)};

%apply (TYPE ARGOUT_ARRAY2[ANY][ANY]) {(TYPE lower[3][3])};
%apply (TYPE ARGOUT_ARRAY2[ANY][ANY]) {(TYPE upper[3][3])};

%enddef // %apply_numpy_typemaps() macro
%apply_numpy_typemaps(float )
%apply_numpy_typemaps(double )
//<---Alexander Filov----
#endif

/*
%rename(mglData) mglDataW
%rename(mglParse) mglParseW
%rename(mglGraph) mglGraphW
*/

%include "mgl/mgl_w.h"
%extend mglData
{
	float __getitem__( int i)	{	return self->GetVal(i);	};
	float __paren( int i)		{	return self->GetVal(i);	};
	void __setitem__( int i, float y)	{	self->SetVal(y,i);	};
	void __paren_asgn( int i, float y)	{	self->SetVal(y,i);	};
};
