/***************************************************************************
 * fltk_example.cpp is part of Math Graphic Library
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
#include "mgl/mgl.h"
#include "mgl/mgl_fltk.h"
//-----------------------------------------------------------------------------
int test_wnd(mglGraph *gr);
int sample(mglGraph *gr);
int sample_1(mglGraph *gr);
int sample_2(mglGraph *gr);
int sample_3(mglGraph *gr);
int sample_d(mglGraph *gr);
//-----------------------------------------------------------------------------
#include <pthread.h>
#include <unistd.h>
mglPoint pnt;  // some global variable for changable data
void *mgl_fltk_tmp(void *)	{	mgl_fltk_run();	return 0;	}
//-----------------------------------------------------------------------------
int main(int argc,char **argv)
{
#ifdef PTHREAD_SAMPLE
	mglGraphFLTK gr;
	gr.Window(argc,argv,NULL,"test",0,0);  // create window
	gr.ClfOnUpdate = false;
	static pthread_t tmp;
	pthread_create(&tmp, 0, mgl_fltk_tmp, 0);
	pthread_detach(tmp);    // run window handling in the separate thread
	for(int i=0;i<10;i++)   // do calculation
	{
		sleep(2);             // which can be very long
		pnt = mglPoint(2*mgl_rnd()-1,2*mgl_rnd()-1);
		gr.Clf();             // make new drawing
		gr.Line(mglPoint(),pnt,"Ar2");
		char str[10] = "i=0";	str[3] = '0'+i;
		gr.Text(mglPoint(),"");
		gr.Update();       // update window
	}
	return 0;   // finish calculations and close the window*/
#else
	char key = 0;
	if(argc>1)	key = (argv[1][0]!='-') ? argv[1][0] : argv[1][1];
	if(!key)	printf("You may specify argument '1', '2', '3' or 'd' for viewing examples of 1d, 2d, 3d or dual plotting\n");
	mglCanvasFL gr;
	switch(key)
	{
	case '1':	gr.Window(argc,argv,sample_1,"1D plots");	break;
	case '2':	gr.Window(argc,argv,sample_2,"2D plots");	break;
	case '3':	gr.Window(argc,argv,sample_3,"3D plots");	break;
	case 'd':	gr.Window(argc,argv,sample_d,"Dual plots");	break;
	case 't':	gr.Window(argc,argv,test_wnd,"Testing");	break;
	default:	gr.Window(argc,argv,sample,"Example of molecules");	break;
	}
	mgl_fltk_run();
	return 0;
#endif
}
//-----------------------------------------------------------------------------
