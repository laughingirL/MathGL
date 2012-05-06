/***************************************************************************
 * mgl_cf.cpp is part of Math Graphic Library
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
#ifndef _MGL_CF_H_
#define _MGL_CF_H_
/*****************************************************************************/
#include "mgl2/base_cf.h"
#include "mgl2/data_cf.h"
#include "mgl2/cont.h"
#include "mgl2/fit.h"
#include "mgl2/plot.h"
#include "mgl2/surf.h"
#include "mgl2/volume.h"
#include "mgl2/vect.h"
#include "mgl2/prim.h"
#include "mgl2/other.h"
#include "mgl2/canvas_cf.h"
#include "mgl2/addon.h"
/*****************************************************************************/
#ifndef NO_OPENGL
#ifdef __cplusplus
extern "C" {
#endif
HMGL mgl_create_graph_gl();
uintptr_t mgl_create_graph_gl_();
#ifdef __cplusplus
}
#endif
#endif
/*****************************************************************************/
#endif
