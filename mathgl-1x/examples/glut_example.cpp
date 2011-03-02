/***************************************************************************
 * glut_example.cpp is part of Math Graphic Library
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
#include <stdio.h>
#include <mgl/mgl_glut.h>
//-----------------------------------------------------------------------------
int test(mglGraph *gr, void *);
int sample(mglGraph *gr, void *);
int sample_1(mglGraph *gr, void *);
int sample_2(mglGraph *gr, void *);
int sample_3(mglGraph *gr, void *);
int sample_d(mglGraph *gr, void *);
//-----------------------------------------------------------------------------
int main(int argc,char **argv)
{
	mglGraphGLUT gr;
	char key = 0;
	if(argc>1 && argv[1][0]!='-')	key = argv[1][0];
	else	printf("You may specify argument '1', '2', '3' or 'd' for viewing examples of 1d, 2d, 3d or dual plotting");
	switch(key)
	{
	case '1':	gr.Window(0,0,sample_1,"1D plots");	break;
	case '2':	gr.Window(0,0,sample_2,"2D plots");	break;
	case '3':	gr.Window(0,0,sample_3,"3D plots");	break;
	case 'd':	gr.Window(0,0,sample_d,"Dual plots");	break;
	default:	gr.Window(0,0,sample,"Example of molecules");	break;
	}
	return 0;
}
//-----------------------------------------------------------------------------
