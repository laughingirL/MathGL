/***************************************************************************
 * volume.h is part of Math Graphic Library
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
#ifndef _MGL_VOL_H_
#define _MGL_VOL_H_
#include "mgl2/base.h"
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
void mgl_surf3_xyz_val(HMGL graph, double Val, HCDT x, HCDT y, HCDT z, HCDT a, const char *stl, const char *opt);
void mgl_surf3_val(HMGL graph, double Val, HCDT a, const char *stl, const char *opt);
void mgl_surf3_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, HCDT a, const char *stl, const char *opt);
void mgl_surf3(HMGL graph, HCDT a, const char *stl, const char *opt);

void mgl_surf3a_xyz_val(HMGL graph, double Val, HCDT x, HCDT y, HCDT z, HCDT a, HCDT b, const char *stl, const char *opt);
void mgl_surf3a_val(HMGL graph, double Val, HCDT a, HCDT b, const char *stl, const char *opt);
void mgl_surf3a_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, HCDT a, HCDT b, const char *stl, const char *opt);
void mgl_surf3a(HMGL graph, HCDT a, HCDT b, const char *stl, const char *opt);

void mgl_surf3c_xyz_val(HMGL graph, double Val, HCDT x, HCDT y, HCDT z, HCDT a, HCDT b, const char *stl, const char *opt);
void mgl_surf3c_val(HMGL graph, double Val, HCDT a, HCDT b, const char *stl, const char *opt);
void mgl_surf3c_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, HCDT a, HCDT b, const char *stl, const char *opt);
void mgl_surf3c(HMGL graph, HCDT a, HCDT b, const char *stl, const char *opt);

// NOTE: now style '.' draws dots instead of faces
void mgl_cloud_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, HCDT a, const char *stl, const char *opt);
void mgl_cloud(HMGL graph, HCDT a, const char *stl, const char *opt);

void mgl_beam_val(HMGL graph, double Val, HCDT tr, HCDT g1, HCDT g2, HCDT a, double r, const char *stl, int norm);
void mgl_beam(HMGL graph, HCDT tr, HCDT g1, HCDT g2, HCDT a, double r, const char *stl, int norm, int num);

/******************************************************************************/
void mgl_surf3_xyz_val_(uintptr_t *graph, double *Val, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *stl, const char *opt,int,int);
void mgl_surf3_val_(uintptr_t *graph, double *Val, uintptr_t *a, const char *stl, const char *opt,int,int);
void mgl_surf3_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *stl, const char *opt,int,int);
void mgl_surf3_(uintptr_t *graph, uintptr_t *a, const char *stl, const char *opt,int,int);

void mgl_surf3a_xyz_val_(uintptr_t *graph, double *Val, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, uintptr_t *b, const char *stl, const char *opt,int,int);
void mgl_surf3a_val_(uintptr_t *graph, double *Val, uintptr_t *a, uintptr_t *b, const char *stl, const char *opt,int,int);
void mgl_surf3a_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, uintptr_t *b, const char *stl, const char *opt,int,int);
void mgl_surf3a_(uintptr_t *graph, uintptr_t *a, uintptr_t *b, const char *stl, const char *opt,int,int);

void mgl_surf3c_xyz_val_(uintptr_t *graph, double *Val, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, uintptr_t *b, const char *stl, const char *opt,int,int);
void mgl_surf3c_val_(uintptr_t *graph, double *Val, uintptr_t *a, uintptr_t *b, const char *stl, const char *opt,int,int);
void mgl_surf3c_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, uintptr_t *b, const char *stl, const char *opt,int,int);
void mgl_surf3c_(uintptr_t *graph, uintptr_t *a, uintptr_t *b, const char *stl, const char *opt,int,int);

void mgl_cloudp_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *stl, const char *opt,int,int);
void mgl_cloudp_(uintptr_t *graph, uintptr_t *a, const char *stl, const char *opt,int,int);
void mgl_cloud_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *stl, const char *opt,int,int);
void mgl_cloud_(uintptr_t *graph, uintptr_t *a, const char *stl, const char *opt,int,int);

void mgl_beam_val_(uintptr_t *gr, double *val, uintptr_t *tr, uintptr_t *g1, uintptr_t *g2, uintptr_t *a, double *r, const char *sch, int *norm,int l);
void mgl_beam_(uintptr_t *gr, uintptr_t *tr, uintptr_t *g1, uintptr_t *g2, uintptr_t *a, double *r, const char *sch, int *norm, int *num,int l);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif
