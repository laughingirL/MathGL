/***************************************************************************
 * full_test.cpp is part of Math Graphic Library
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
#include <time.h>
#include <locale.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <mgl/mgl_zb.h>
#include <mgl/mgl_eps.h>
#include <mgl/mgl_idtf.h>
//-----------------------------------------------------------------------------
int type = 0;
int dotest  = 0;
int width  = 600;
int height = 400;
int mini = 0;
int big  = 0;
int srnd = 0;
mglData s;
//-----------------------------------------------------------------------------
void save(mglGraph *gr,const char *name,const char *suf="")
{
//	return;
	char buf[128];
	printf("%s\t",name);	fflush(stdout);
	switch(type)
	{
		case 1:	// EPS
			sprintf(buf,"%s%s.eps",name,suf);
			gr->WriteEPS(buf);
//			sprintf(buf,"%s%s.png",name,suf);
//			gr->WritePNG(buf,0,false);	break;
		case 2:	// SVG
			sprintf(buf,"%s%s.svg",name,suf);
			gr->WriteSVG(buf);	break;
		case 3:	// PNG
			sprintf(buf,"%s%s.png",name,suf);
			gr->WritePNG(buf,0,true);	break;
		case 4:	// JPEG
			sprintf(buf,"%s%s.jpg",name,suf);
			gr->WriteJPEG(buf);	break;
		case 5:	// IDTF
			sprintf(buf,"%s%s.idtf",name,suf);
			gr->WriteIDTF(buf);	break;
		case 6:	// GIF
			sprintf(buf,"%s%s.gif",name,suf);
			gr->WriteGIF(buf);	break;
			default:// PNG (no alpha)
			sprintf(buf,"%s%s.png",name,suf);
			gr->WritePNG(buf,0,false);	break;
	}
}
//-----------------------------------------------------------------------------
int sample_tval(mglGraph *gr, const void *)	// font features
{
	gr->Axis(mglPoint(-M_PI, 0), mglPoint(M_PI, 2));
	gr->SetTicksVal('x', 6, -M_PI, "-\\pi", -M_PI/2, "-\\pi/2", 0., "0",
				 0.886, "x^*", M_PI/2, "\\pi/2", M_PI, "\\pi");
	gr->Axis();	gr->Grid();
	gr->Plot("2*cos(x^2)^2", "r2", NAN, 300);
	return 0;
}
//-----------------------------------------------------------------------------
int sample_column(mglGraph *gr, const void *)	// font features
{
	char str[32];
	for(int i=0;i<4;i++)
	{
		gr->ColumnPlot(4,i);
		gr->Box();
		sprintf(str,"Plot %d of 4",i);
		gr->Text(mglPoint(-0.5,0.5),str);
		sprintf(str,"sin(pi*x+pi*%d/2)",i);
		gr->Plot(str);
	}
	return 0;
}
//-----------------------------------------------------------------------------
int sample_map(mglGraph *gr, const void *s)	// full test (in PNG)
{
	mglData a(50, 40), b(50, 40);
	gr->Puts(mglPoint(0, 0), "\\to", "C", -1.4);
	gr->Axis(mglPoint(-1,-1,-2),mglPoint(1,1,2));

	gr->SubPlot(2, 1, 0);
	a.Fill("x", gr->Min, gr->Max);	b.Fill("y", gr->Min, gr->Max);
	gr->Puts(mglPoint(0, 1.1), "\\{x, y\\}", "C", -2);		gr->Box();
	gr->Map(a, b, "brgk", 0, false);

	gr->SubPlot(2, 1, 1);
	a.Fill("(x^3+y^3)/2", gr->Min, gr->Max);
	b.Fill("(x-y)/2", gr->Min, gr->Max);
	gr->Puts(mglPoint(0, 1.1), "\\{\\frac{x^3+y^3}{2}, \\frac{x-y}{2}\\}", "C", -2);
	gr->Box();
	gr->Map(a, b, "brgk", 0, false);
	return 0;
}
//-----------------------------------------------------------------------------
int sample_colors(mglGraph *gr, const void *)	// Color table
{
	gr->Face(mglPoint(-1,  -1), mglPoint(-1,  -0.7), mglPoint(-0.6,-1), mglPoint(-0.6,-0.7), "L#");
	gr->Puts(mglPoint(-0.8,-0.9, 0.01), "L", "C:w", -1.4);
	gr->Face(mglPoint(-0.6,-1), mglPoint(-0.6,-0.7), mglPoint(-0.2,-1), mglPoint(-0.2,-0.7), "E#");
	gr->Puts(mglPoint(-0.4,-0.9, 0.01), "E", "C:w", -1.4);
	gr->Face(mglPoint(-0.2,-1), mglPoint(-0.2,-0.7), mglPoint(0.2,-1), mglPoint(0.2,-0.7), "N#");
	gr->Puts(mglPoint(0,  -0.9, 0.01), "N", "C:w", -1.4);
	gr->Face(mglPoint(0.2,-1), mglPoint(0.2,-0.7), mglPoint(0.6,-1), mglPoint(0.6,-0.7), "U#");
	gr->Puts(mglPoint(0.4,-0.9, 0.01), "U", "C:w", -1.4);
	gr->Face(mglPoint(0.6,-1), mglPoint(0.6,-0.7), mglPoint(1,  -1), mglPoint(1,  -0.7), "Q#");
	gr->Puts(mglPoint(0.8,-0.9, 0.01), "Q", "C:w", -1.4);
	//#lenuq
	gr->Face(mglPoint(-1,  -0.7), mglPoint(-1,  -0.4), mglPoint(-0.6,-0.7), mglPoint(-0.6,-0.4), "l#");
	gr->Puts(mglPoint(-0.8,-0.6, 0.01), "l", "C:k", -1.4);
	gr->Face(mglPoint(-0.6,-0.7), mglPoint(-0.6,-0.4), mglPoint(-0.2,-0.7), mglPoint(-0.2,-0.4), "e#");
	gr->Puts(mglPoint(-0.4,-0.6, 0.01), "e", "C:k", -1.4);
	gr->Face(mglPoint(-0.2,-0.7), mglPoint(-0.2,-0.4), mglPoint(0.2,-0.7), mglPoint(0.2,-0.4), "n#");
	gr->Puts(mglPoint(0,  -0.6, 0.01), "n", "C:k", -1.4);
	gr->Face(mglPoint(0.2,-0.7), mglPoint(0.2,-0.4), mglPoint(0.6,-0.7), mglPoint(0.6,-0.4), "u#");
	gr->Puts(mglPoint(0.4,-0.6, 0.01), "u", "C:k", -1.4);
	gr->Face(mglPoint(0.6,-0.7), mglPoint(0.6,-0.4), mglPoint(1,  -0.7), mglPoint(1,  -0.4), "q#");
	gr->Puts(mglPoint(0.8,-0.6, 0.01), "q", "C:k", -1.4);
	//#CMYkP
	gr->Face(mglPoint(-1,  -0.4), mglPoint(-1,  -0.1), mglPoint(-0.6,-0.4), mglPoint(-0.6,-0.1), "C#");
	gr->Puts(mglPoint(-0.8,-0.3, 0.01), "C", "C:w", -1.4);
	gr->Face(mglPoint(-0.6,-0.4), mglPoint(-0.6,-0.1), mglPoint(-0.2,-0.4), mglPoint(-0.2,-0.1), "M#");
	gr->Puts(mglPoint(-0.4,-0.3, 0.01), "M", "C:w", -1.4);
	gr->Face(mglPoint(-0.2,-0.4), mglPoint(-0.2,-0.1), mglPoint(0.2,-0.4), mglPoint(0.2,-0.1), "Y#");
	gr->Puts(mglPoint(0,  -0.3, 0.01), "Y", "C:w", -1.4);
	gr->Face(mglPoint(0.2,-0.4), mglPoint(0.2,-0.1), mglPoint(0.6,-0.4), mglPoint(0.6,-0.1), "k#");
	gr->Puts(mglPoint(0.4,-0.3, 0.01), "k", "C:w", -1.4);
	gr->Face(mglPoint(0.6,-0.4), mglPoint(0.6,-0.1), mglPoint(1,  -0.4), mglPoint(1,  -0.1), "P#");
	gr->Puts(mglPoint(0.8,-0.3, 0.01), "P", "C:w", -1.4);
	//#cmywp
	gr->Face(mglPoint(-1,  -0.1), mglPoint(-1,   0.2), mglPoint(-0.6,-0.1), mglPoint(-0.6, 0.2), "c#");
	gr->Puts(mglPoint(-0.8, 0, 0.01), "c", "C:k", -1.4);
	gr->Face(mglPoint(-0.6,-0.1), mglPoint(-0.6, 0.2), mglPoint(-0.2,-0.1), mglPoint(-0.2, 0.2), "m#");
	gr->Puts(mglPoint(-0.4, 0, 0.01), "m", "C:k", -1.4);
	gr->Face(mglPoint(-0.2,-0.1), mglPoint(-0.2, 0.2), mglPoint(0.2,-0.1), mglPoint(0.2, 0.2), "y#");
	gr->Puts(mglPoint(0,   0, 0.01), "y", "C:k", -1.4);
	gr->Face(mglPoint(0.2,-0.1), mglPoint(0.2, 0.2), mglPoint(0.6,-0.1), mglPoint(0.6, 0.2), "w#");
	gr->Puts(mglPoint(0.4, 0, 0.01), "w", "C:k", -1.4);
	gr->Face(mglPoint(0.6,-0.1), mglPoint(0.6, 0.2), mglPoint(1,  -0.1), mglPoint(1,   0.2), "p#");
	gr->Puts(mglPoint(0.8, 0, 0.01), "p", "C:k", -1.4);
	//#BGRHW
	gr->Face(mglPoint(-1,   0.2), mglPoint(-1,   0.5), mglPoint(-0.6, 0.2), mglPoint(-0.6, 0.5), "B#");
	gr->Puts(mglPoint(-0.8, 0.3, 0.01), "B", "C:w", -1.4);
	gr->Face(mglPoint(-0.6, 0.2), mglPoint(-0.6, 0.5), mglPoint(-0.2, 0.2), mglPoint(-0.2, 0.5), "G#");
	gr->Puts(mglPoint(-0.4, 0.3, 0.01), "G", "C:w", -1.4);
	gr->Face(mglPoint(-0.2, 0.2), mglPoint(-0.2, 0.5), mglPoint(0.2, 0.2), mglPoint(0.2, 0.5), "R#");
	gr->Puts(mglPoint(0,   0.3, 0.01), "R", "C:w", -1.4);
	gr->Face(mglPoint(0.2, 0.2), mglPoint(0.2, 0.5), mglPoint(0.6, 0.2), mglPoint(0.6, 0.5), "H#");
	gr->Puts(mglPoint(0.4, 0.3, 0.01), "H", "C:w", -1.4);
	gr->Face(mglPoint(0.6, 0.2), mglPoint(0.6, 0.5), mglPoint(1,   0.2), mglPoint(1,   0.5), "W#");
	gr->Puts(mglPoint(0.8, 0.3, 0.01), "W", "C:w", -1.4);
	//#bgrhw
	gr->Face(mglPoint(-1,   0.5), mglPoint(-1,   0.8), mglPoint(-0.6, 0.5), mglPoint(-0.6, 0.8), "b#");
	gr->Puts(mglPoint(-0.8, 0.6, 0.01), "b", "C:k", -1.4);
	gr->Face(mglPoint(-0.6, 0.5), mglPoint(-0.6, 0.8), mglPoint(-0.2, 0.5), mglPoint(-0.2, 0.8), "g#");
	gr->Puts(mglPoint(-0.4, 0.6, 0.01), "g", "C:k", -1.4);
	gr->Face(mglPoint(-0.2, 0.5), mglPoint(-0.2, 0.8), mglPoint(0.2, 0.5), mglPoint(0.2, 0.8), "r#");
	gr->Puts(mglPoint(0,   0.6, 0.01), "r", "C:k", -1.4);
	gr->Face(mglPoint(0.2, 0.5), mglPoint(0.2, 0.8), mglPoint(0.6, 0.5), mglPoint(0.6, 0.8), "h#");
	gr->Puts(mglPoint(0.4, 0.6, 0.01), "h", "C:k", -1.4);
	gr->Face(mglPoint(0.6, 0.5), mglPoint(0.6, 0.8), mglPoint(1,   0.5), mglPoint(1,   0.8), "w#");
	gr->Puts(mglPoint(0.8, 0.6, 0.01), "w", "C:k", -1.4);
	//#brighted
	gr->Face(mglPoint(-1,   0.8), mglPoint(-1,   1.1), mglPoint(-0.6, 0.8), mglPoint(-0.6, 1.1), "r1#");
	gr->Puts(mglPoint(-0.8, 0.9, 0.01), "r1", "C:w", -1.4);
	gr->Face(mglPoint(-0.6, 0.8), mglPoint(-0.6, 1.1), mglPoint(-0.2, 0.8), mglPoint(-0.2, 1.1), "r3#");
	gr->Puts(mglPoint(-0.4, 0.9, 0.01), "r3", "C:w", -1.4);
	gr->Face(mglPoint(-0.2, 0.8), mglPoint(-0.2, 1.1), mglPoint(0.2, 0.8), mglPoint(0.2, 1.1), "r5#");
	gr->Puts(mglPoint(0,   0.9, 0.01), "r5", "C:k", -1.4);
	gr->Face(mglPoint(0.2, 0.8), mglPoint(0.2, 1.1), mglPoint(0.6, 0.8), mglPoint(0.6, 1.1), "r7#");
	gr->Puts(mglPoint(0.4, 0.9, 0.01), "r7", "C:k", -1.4);
	gr->Face(mglPoint(0.6, 0.8), mglPoint(0.6, 1.1), mglPoint(1,   0.8), mglPoint(1,   1.1), "r9#");
	gr->Puts(mglPoint(0.8, 0.9, 0.01), "r9", "C:k", -1.4);
	return 0;
}
//-----------------------------------------------------------------------------
int sample_qo2d(mglGraph *gr, const void *)
{
	gr->VertexColor(true);  // we need smooth color change
	gr->Compression(true);  // try to save space
	mglData r, xx, yy, a, im(128), re(128);
	const char *ham = "p^2+q^2-x-1+i*0.5*(y+x)*(y>-x)";
	r = mglRay(ham, mglPoint(-0.7, -1), mglPoint(0, 0.5), 0.02, 2);
	gr->Plot(r.SubData(0), r.SubData(1), "k");
	gr->Axis();	gr->Label('x', "\\i x");	gr->Label('y', "\\i z");
	// now start beam tracing
	re.Fill("exp(-48*x^2)", gr->Min, gr->Max);
	a = mglQO2d(ham, re, im, r, 1, 30, &xx, &yy);
	gr->CAxis(0, 1);
	gr->Dens(xx, yy, a, "wyrRk");
	gr->Plot("-x", "k|");
	gr->Puts(mglPoint(0, 0.85), "absorption: (x+y)/2 for x+y>0");
	gr->Puts(mglPoint(0.7, -0.05), "central ray");
	gr->Title("Beam and ray tracing", "C", -1.5);
	gr->Compression(false);  //put setting back
	return 0;
}
//-----------------------------------------------------------------------------
int sample_pde(mglGraph *gr, const void *)	// PDE and Ray sample
{
	gr->VertexColor(true);  // we need smooth color change
	gr->Compression(true);  // try to save space
	mglData a,re(128),im(128);
	gr->Axis();
	gr->Label('x', "\\i x");
	gr->Label('y', "\\i z");

	re.Fill("exp(-48*(x+0.7)^2)", gr->Min, gr->Max);
	a = mglPDE("p^2+q^2-x-1+i*0.5*(z+x)*(z>-x)", re, im,
				gr->Min, gr->Max, 0.01, 30);
	a.Transpose("yxz");
	gr->CAxis(0, 1);
	gr->Dens(a,"wyrRk");
	gr->Plot("-x", "k|", type==5?0.01:NAN);
	gr->Puts(mglPoint(0, 0.85), "absorption: (x+z)/2 for x+z>0");
	gr->Title("Equation: \\i{ik_0\\partial_zu + \\Delta u + x\\cdot u + i \\frac{x+z}{2}\\cdot u = 0}", "C", -float(width)/height);
	gr->Compression(false);  //put setting back
	return 0;
}
//-----------------------------------------------------------------------------
int sample_stfa(mglGraph *gr, const void *)	// STFA sample
{
	gr->VertexColor(true);
	mglData a(2000), b(2000);
	a.Fill("cos(50*pi*x)*(x<-.5)+cos(100*pi*x)*(x<0)*(x>-.5)+\
				cos(200*pi*x)*(x<.5)*(x>0)+cos(400*pi*x)*(x>.5)",
				gr->Min, gr->Max);
	gr->SubPlot(1, 2, 0);
	gr->Plot(a);
	gr->Axis();
	gr->Label('x', "\\i t");

	gr->SubPlot(1, 2, 1);
	gr->STFA(a, b, 64, "BbcyrR");
	gr->Axis();
	gr->Label('x', "\\i t");
	gr->Label('y', "\\omega", 0);
	return 0;
}
//-----------------------------------------------------------------------------
int sample_envelop(mglGraph *gr, const void *)	// Envelop reconstruction
{
	mglData a(1000);
	a.Fill("exp(-8*x^2)*sin(10*pi*x)", gr->Min, gr->Max);
	gr->Plot(a, "b");
	a.Envelop('x');
	gr->Plot(a, "r");
	gr->Axis();
	return 0;
}
//-----------------------------------------------------------------------------
int sample_sew(mglGraph *gr, const void *)	// Phase reconstruction
{
	gr->VertexColor(false);
	mglData a(100, 100);
	a.Modify("mod((y^2-(1-x)^2)/2,0.1)");
	gr->Rotate(40, 60);
	gr->Light(true);
	gr->Alpha(true);
	gr->Surf(a, "b");
	a.Sew("xy", 0.1);
	gr->Surf(a, "r");
	gr->Box();
	return 0;
}
//-----------------------------------------------------------------------------
int sample_ae(mglGraph *gr, const void *)	// TeX sample
{
	gr->Puts(mglPoint(0), "\\sqrt{\\frac{\\alpha^{\\gamma^2}+\\overset 1{\\big\\infty}}{\\sqrt3{2+b}}}", 0, -4);
	return 0;
}
//-----------------------------------------------------------------------------
int sample_ad(mglGraph *gr, const void *)	// arrow styles
{
	float a=0.1,b=0.4,c=0.5;
	gr->Line(mglPoint(a,1),mglPoint(b,1),"k-A");		gr->Puts(mglPoint(c,1),"Style 'A' or 'A\\_'","rL");
	gr->Line(mglPoint(a,0.8),mglPoint(b,0.8),"k-V");	gr->Puts(mglPoint(c,0.8),"Style 'V' or 'V\\_'","rL");
	gr->Line(mglPoint(a,0.6),mglPoint(b,0.6),"k-K");	gr->Puts(mglPoint(c,0.6),"Style 'K' or 'K\\_'","rL");
	gr->Line(mglPoint(a,0.4),mglPoint(b,0.4),"k-I");	gr->Puts(mglPoint(c,0.4),"Style 'I' or 'I\\_'","rL");
	gr->Line(mglPoint(a,0.2),mglPoint(b,0.2),"k-D");	gr->Puts(mglPoint(c,0.2),"Style 'D' or 'D\\_'","rL");
	gr->Line(mglPoint(a,0),mglPoint(b,0),"k-S");		gr->Puts(mglPoint(c,0),"Style 'S' or 'S\\_'","rL");
	gr->Line(mglPoint(a,-0.2),mglPoint(b,-0.2),"k-O");	gr->Puts(mglPoint(c,-0.2),"Style 'O' or 'O\\_'","rL");
	gr->Line(mglPoint(a,-0.4),mglPoint(b,-0.4),"k-T");	gr->Puts(mglPoint(c,-0.4),"Style 'T' or 'T\\_'","rL");
	gr->Line(mglPoint(a,-0.6),mglPoint(b,-0.6),"k-_");	gr->Puts(mglPoint(c,-0.6),"Style '\\_' or none","rL");
	gr->Line(mglPoint(a,-0.8),mglPoint(b,-0.8),"k-AS");	gr->Puts(mglPoint(c,-0.8),"Style 'AS'","rL");
	gr->Line(mglPoint(a,-1),mglPoint(b,-1),"k-_A");		gr->Puts(mglPoint(c,-1),"Style '\\_A'","rL");

	a=-1;	b=-0.7;	c=-0.6;
	gr->Line(mglPoint(a,1),mglPoint(b,1),"kAA");		gr->Puts(mglPoint(c,1),"Style 'AA'","rL");
	gr->Line(mglPoint(a,0.8),mglPoint(b,0.8),"kVV");	gr->Puts(mglPoint(c,0.8),"Style 'VV'","rL");
	gr->Line(mglPoint(a,0.6),mglPoint(b,0.6),"kKK");	gr->Puts(mglPoint(c,0.6),"Style 'KK'","rL");
	gr->Line(mglPoint(a,0.4),mglPoint(b,0.4),"kII");	gr->Puts(mglPoint(c,0.4),"Style 'II'","rL");
	gr->Line(mglPoint(a,0.2),mglPoint(b,0.2),"kDD");	gr->Puts(mglPoint(c,0.2),"Style 'DD'","rL");
	gr->Line(mglPoint(a,0),mglPoint(b,0),"kSS");		gr->Puts(mglPoint(c,0),"Style 'SS'","rL");
	gr->Line(mglPoint(a,-0.2),mglPoint(b,-0.2),"kOO");	gr->Puts(mglPoint(c,-0.2),"Style 'OO'","rL");
	gr->Line(mglPoint(a,-0.4),mglPoint(b,-0.4),"kTT");	gr->Puts(mglPoint(c,-0.4),"Style 'TT'","rL");
	gr->Line(mglPoint(a,-0.6),mglPoint(b,-0.6),"k-__");	gr->Puts(mglPoint(c,-0.6),"Style '\\_\\_'","rL");
	gr->Line(mglPoint(a,-0.8),mglPoint(b,-0.8),"k-VA");	gr->Puts(mglPoint(c,-0.8),"Style 'VA'","rL");
	gr->Line(mglPoint(a,-1),mglPoint(b,-1),"k-AV");		gr->Puts(mglPoint(c,-1),"Style 'AV'","rL");

	return 0;
}
//-----------------------------------------------------------------------------
int sample_ac(mglGraph *gr, const void *)	// error boxes
{
	mglData  x(10), y(10), ex(10), ey(10), y0(50);
	x.Modify("2*x-1 + 0.2*rnd-0.1");
	y.Modify("(2*x-1)^2 + 0.2*rnd-0.1");
	y0.Modify("(2*x-1)^2");
	ex.Modify("0.2");
	ey.Modify("0.1");

	gr->Clf();
	gr->SubPlot(2,2,0);
	gr->Error(y,ey,"ko");
	gr->Plot(y0,"r");
	gr->Axis();
	gr->Text(mglPoint(0,1.2,0),"Random {\\i y}");
	gr->SubPlot(2,2,1);
	gr->Error(x,y,ex,"ko");
	gr->Plot(y0,"r");
	gr->Axis();
	gr->Text(mglPoint(0,1.2,0),"Random {\\i x, y}");
	gr->SubPlot(2,2,2);
	gr->Error(x,y,ey,ey,"ko");
	gr->Plot(y0,"r");
	gr->Axis();
	gr->Text(mglPoint(0,1.2,0),"Random {\\i x, y} and 2d boxes");
	gr->SubPlot(2,2,3);
	gr->Text(mglPoint(0,1.2,0),"Random point in 3d space");
	gr->Rotate(40,60);
	for(int i=0;i<10;i++)
		gr->Error(mglPoint(mgl_rnd()-0.5,mgl_rnd()-0.5,mgl_rnd()-0.5), mglPoint(0.1,0.1,0.1),"bo");
	gr->Axis();
	return 0;
}
//-----------------------------------------------------------------------------
int sample_ab(mglGraph *gr, const void *)	// Gaussian beam
{
	gr->VertexColor(true);
	gr->Alpha(true);	gr->Light(true);	gr->Light(0,mglPoint(0,0,1));
	mglData a(30,30,30),b(30,30,30);
	a.Modify("exp(-16*((z-0.5)^2+(y-0.5)^2)/(1+4*x^2))");
	b.Modify("16*((z-0.5)^2+(y-0.5)^2)*(x)/(1+4*x^2)");
	gr->CAxis(0,1);

	gr->SubPlot(2,2,0);	gr->Rotate(40,60);
	gr->Surf3(a,"wgk");	gr->Box();
	gr->SubPlot(2,2,1);	gr->Rotate(40,60);
	gr->DensA(a);		gr->Box();	gr->Axis();
	gr->SubPlot(2,2,2);	gr->Rotate(40,60);
	// Clouds not supported in u3d
(type==5)?  gr->Text(mglPoint(0,0,0),"no clouds in u3d"): gr->CloudQ(a);		gr->Box();
	gr->SubPlot(2,2,3);	gr->Rotate(40,60);
	gr->VertexColor(false);	// for variable transparency the only way is to disable smooth (per-vertex) coloring
	gr->Surf3A(b,a,"q");		gr->Box();
	return 0;
}
//-----------------------------------------------------------------------------
int sample_aa(mglGraph *gr, const void *)	// flag #
{
	gr->VertexColor(true);
	gr->Alpha(true);	gr->Light(true);	gr->Light(0,mglPoint(0,0,1));
	mglData a(30,20);
	a.Modify("0.6*sin(2*pi*x)*sin(3*pi*y) + 0.4*cos(3*pi*(x*y))");

	gr->SubPlot(2,2,0);	gr->Rotate(40,60);
	gr->Surf(a,"BbcyrR#");		gr->Box();
	gr->SubPlot(2,2,1);	gr->Rotate(40,60);
	gr->Dens(a,"BbcyrR#");		gr->Box();
	gr->SubPlot(2,2,2);	gr->Rotate(40,60);
	gr->Cont(a,"BbcyrR#");		gr->Box();
	gr->SubPlot(2,2,3);	gr->Rotate(40,60);
	gr->Axial(a,"BbcyrR#");		gr->Box();
	return 0;
}
//-----------------------------------------------------------------------------
int sample_a9(mglGraph *gr, const void *)	// 2d plot
{
	gr->VertexColor(true);
	gr->Light(true);	gr->Light(0,mglPoint(0,0,1));

	mglData a0(50,40);
	a0.Modify("0.6*sin(2*pi*x)*sin(3*pi*y) + 0.4*cos(3*pi*(x*y))");
	gr->SubPlot(2,2,0);	gr->Rotate(60,40);
	gr->Surf(a0);		gr->Box();

	mglData x(50,40),y(50,40),z(50,40);
	x.Modify("0.8*sin(2*pi*x)*sin(pi*y)");
	y.Modify("0.8*cos(2*pi*x)*sin(pi*y)");
	z.Modify("0.8*cos(pi*y)");
	gr->SubPlot(2,2,1);	gr->Rotate(60,40);
	gr->Surf(x,y,z,"BbwrR");		gr->Box();

	mglData a1(50,40,3);
	a1.Modify("0.6*sin(2*pi*x)*sin(3*pi*y) + 0.4*cos(3*pi*(x*y))");
	a1.Modify("0.6*cos(2*pi*x)*cos(3*pi*y) + 0.4*sin(3*pi*(x*y))",1);
	a1.Modify("0.6*cos(2*pi*x)*cos(3*pi*y) + 0.4*cos(3*pi*(x*y))",2);
	gr->SubPlot(2,2,2);	gr->Rotate(60,40);
	gr->Alpha(true);
	gr->Surf(a1);		gr->Box();

	gr->SubPlot(2,2,3);	gr->Rotate(60,40);
	gr->Dens(a1);		gr->Box();

	return 0;
}
//-----------------------------------------------------------------------------
int sample_a8(mglGraph *gr, const void *)	// 1d plot
{
	mglData y0(50);		y0.Modify("sin(pi*(2*x-1))");
	gr->SubPlot(2,2,0);
	gr->Plot(y0);		gr->Box();

	gr->SubPlot(2,2,1);
	mglData y1(50,2);
	y1.Modify("sin(pi*2*x-pi)");	y1.Modify("cos(pi*2*x-pi)/2",1);
	gr->Plot(y1);		gr->Box();

	mglData x(50);		x.Modify("cos(pi*2*x-pi)");
	gr->Plot(x,y0,"Y+");

	gr->Plot2(y1,"q|");

	gr->SubPlot(2,2,2);	gr->Rotate(60,40);
	mglData z(50);		z.Modify("2*x-1");
	gr->Plot(x,y0,z);		gr->Box();

	mglData y2(10,3);	y2.Modify("cos(pi*(2*x-1-y))");
	y2.Modify("2*x-1",2);
	gr->Plot3(y2,"bo ");

	gr->SubPlot(2,2,3);	gr->Rotate(60,40);
	gr->Bars(x,y0,z,"ri");		gr->Box();
	return 0;
}
//-----------------------------------------------------------------------------
int sample_a7(mglGraph *gr, const void *)	// smoothing
{
	mglData y0(30),y1,y2,y3;
	y0.Modify("0.4*sin(2*pi*x) + 0.3*cos(3*pi*x) - 0.4*sin(4*pi*x)+0.2*rnd");

	y1=y0;	y1.Smooth(SMOOTH_LINE_3);
	y2=y0;	y2.Smooth(SMOOTH_LINE_5);
	y3=y0;	y3.Smooth(SMOOTH_QUAD_5);

	gr->Plot(y0,"k");	gr->AddLegend("NONE","k");
	gr->Plot(y1,"r");	gr->AddLegend("LINE\\_3","r");
	gr->Plot(y2,"g");	gr->AddLegend("LINE\\_5","g");
	gr->Plot(y3,"b");	gr->AddLegend("QUAD\\_5","b");
//	if (type==5)	// Problem in idtf, the white background and text take the same place
//		gr->LegendBox = false;	// TODO: check with new Glyph()
	gr->Legend();		gr->Box();
	gr->ClearLegend();	// clear legend strings
	return 0;
}
//-----------------------------------------------------------------------------
int sample_a6(mglGraph *gr, const void *)	// differentiate
{
	gr->VertexColor(true);
	mglData a(30,40);	a.Modify("x*y");
	gr->Axis(mglPoint(0,0,0),mglPoint(1,1,1));
	gr->SubPlot(2,2,0);	gr->Rotate(60,40);
	gr->Surf(a);		gr->Box();
	gr->Text(mglPoint(0.7,1,1.2),"a(x,y)");
	gr->SubPlot(2,2,1);	gr->Rotate(60,40);
	a.Diff("x");		gr->Surf(a);	gr->Box();
	gr->Text(mglPoint(0.7,1,1.2),"da/dx");
	gr->SubPlot(2,2,2);	gr->Rotate(60,40);
	a.Integral("xy");	gr->Surf(a);	gr->Box();
	gr->Text(mglPoint(0.7,1,1.2),"\\int da/dx dxdy");
	gr->SubPlot(2,2,3);	gr->Rotate(60,40);
	a.Diff2("y");	gr->Surf(a);	gr->Box();
	gr->Text(mglPoint(0.7,1,1.2),"\\int {d^2}a/dxdy dx");
	return 0;
}
//-----------------------------------------------------------------------------
int sample_a5(mglGraph *gr, const void *)	// pen styles
{
	if(type==5)	gr->Puts(mglPoint(0,1.2),"line styles not supported","rL");
	gr->Line(mglPoint(0,1,0),mglPoint(0.3,1,0),"k-");	gr->Puts(mglPoint(0.4,1),"Solid '-'","rL");
	gr->Line(mglPoint(0,0.7,0),mglPoint(0.3,0.7,0),"k|");	gr->Puts(mglPoint(0.4,0.7),"Dash '|'","rL");
	gr->Line(mglPoint(0,0.4,0),mglPoint(0.3,0.4,0),"k;");	gr->Puts(mglPoint(0.4,0.4),"Small dash ';'","rL");
	gr->Line(mglPoint(0,0.1,0),mglPoint(0.3,0.1,0),"kj");	gr->Puts(mglPoint(0.4,0.1),"Dash-dot 'j'","rL");
	gr->Line(mglPoint(0,-0.2,0),mglPoint(0.3,-0.2,0),"ki");	gr->Puts(mglPoint(0.4,-0.2),"Small dash-dot 'i'","rL");
	gr->Line(mglPoint(0,-0.5,0),mglPoint(0.3,-0.5,0),"k:");	gr->Puts(mglPoint(0.4,-0.5),"Dots ':'","rL");
	gr->Line(mglPoint(0,-0.8,0),mglPoint(0.3,-0.8,0),"k ");	gr->Puts(mglPoint(0.4,-0.8),"None ' '","rL");

	gr->Mark(mglPoint(-1,1.2,0),'.');	gr->Puts(mglPoint(-0.7,1.2),"'.'","rL");
	gr->Mark(mglPoint(-1,0.9,0),'+');	gr->Puts(mglPoint(-0.7,0.9),"'+'","rL");
	gr->Mark(mglPoint(-1,0.6,0),'x');	gr->Puts(mglPoint(-0.7,0.6),"'x'","rL");
	gr->Mark(mglPoint(-1,0.3,0),'*');	gr->Puts(mglPoint(-0.7,0.3),"'*'","rL");
	gr->Mark(mglPoint(-1,0.0,0),'s');	gr->Puts(mglPoint(-0.7,0.0),"'s'","rL");
	gr->Mark(mglPoint(-1,-0.3,0),'d');	gr->Puts(mglPoint(-0.7,-0.3),"'d'","rL");
	gr->Mark(mglPoint(-1,-0.6,0),'o');	gr->Puts(mglPoint(-0.7,-0.6),"'o'","rL");
	gr->Mark(mglPoint(-1,-0.9,0),'^');	gr->Puts(mglPoint(-0.7,-0.9),"'\\^'","rL");
	gr->Mark(mglPoint(-1,-1.2,0),'v');	gr->Puts(mglPoint(-0.7,-1.2),"'v'","rL");
	return 0;
}
//-----------------------------------------------------------------------------
int sample_a4(mglGraph *gr, const void *)	// font features
{
//	setlocale(LC_CTYPE, "ru_RU.cp1251");
//	gr->Putsw(mglPoint(0,1),L"Text can be in english и в ёникоде");
	gr->Putsw(mglPoint(0,1),L"Text can be in ASCII and in Unicode");
	gr->Puts(mglPoint(0,0.6),"It can be \\wire{wire}, \\big{big} or #r{colored}");
	gr->Puts(mglPoint(0,0.2),"One can change style in string: "
			"\\b{bold}, \\i{italic, \\b{both}}");
	gr->Puts(mglPoint(0,-0.2),"Easy to \\a{overline} or "
			"\\u{underline}");
	gr->Puts(mglPoint(0,-0.6),"Easy to change indexes ^{up} _{down} @{center}");
	gr->Puts(mglPoint(0,-1),"It parse TeX: \\int \\alpha \\cdot "
			"\\sqrt3{sin(\\pi x)^2 + \\gamma_{i_k}} dx");
	return 0;
}
//-----------------------------------------------------------------------------
int sample_a3(mglGraph *gr, const void *)	// curvilinear coordinates
{
	mglData x(50),y(50),z(50);
	y.Fill(0.5,0.5);
	x.Fill(-1,1);        // create data arrays

	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1),mglPoint(-1,1,-1));
	gr->SetTicks('z', 0.5);  // set tick step to 0.5

	gr->SubPlot(2,2,0);
	gr->Rotate(60,40);
	gr->Plot(x,y,z,"r2");
	gr->Axis(); gr->Grid();
	gr->Text(mglPoint(0,1.3,1),"Cartesian");

	gr->SubPlot(2,2,1);
	gr->Axis("y*sin(pi*x)","y*cos(pi*x)",0);
	gr->Rotate(60,40);
	gr->Plot(x,y,z,"r2");
	gr->Axis(); gr->Grid();
	gr->Text(mglPoint(0,1.3,1),"Cylindrical");

	gr->SubPlot(2,2,2);
	gr->Rotate(60,40);
	gr->Axis("2*y*x","y*y - x*x",0);
	gr->Plot(x,y,z,"r2");
	gr->Axis(); gr->Grid();
	gr->Text(mglPoint(0,1.3,1),"Parabolic");

	gr->SubPlot(2,2,3);
	gr->Rotate(60,40);
	gr->Axis("y*sin(pi*x)","y*cos(pi*x)","x+z");
	gr->Plot(x,y,z,"r2");
	gr->Axis(); gr->Grid();
	gr->Text(mglPoint(0,1.3,1),"Spiral");
	gr->Axis(0,0,0);	// set to default Cartesian
	return 0;
}
//-----------------------------------------------------------------------------
int sample_a2(mglGraph *gr, const void *)	// axis and grid
{
	gr->SubPlot(2,2,0);
	gr->SetTicks('x', 0.4, 3);  // sets tick step to 0.4
	gr->SetTicks('y', 0.4, 3);  // and draws 3 subticks
	gr->Box();                  // should be after the ticks change
	gr->Axis("xy");
	gr->Grid();
	gr->Puts(mglPoint(0,1.3,1),"Axis and grid");

	gr->SetTicks('x');  gr->SetTicks('y'); // restore back
	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1),mglPoint(0,0,0));

	gr->SubPlot(2,2,1);
	gr->Rotate(60,40);
	gr->Axis();
	gr->Label('x',"x");
	gr->Label('y',"y");
	gr->Label('z',"z");
	gr->Puts(mglPoint(0,0,1.5),"Axis and labels");

	gr->SubPlot(2,2,2);
	gr->Rotate(60,40);
	gr->SetTicks('x', 0.2); gr->SetTicks('y', 0.2);
	gr->SetTicks('z', 0.2); // too low step of ticks
	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1),mglPoint(-1,-1,-1));
	gr->Axis();
	gr->Grid();
	gr->Puts(mglPoint(0,0,1.5),"Shift origin and add grid");
	gr->Puts(mglPoint(0,0,1.2),"(note, too many ticks)");

	gr->SubPlot(2,2,3);
	gr->Rotate(60,40);
	gr->SetTicks('x', -6);  // decrease the number of ticks
	gr->SetTicks('y', -6);
	gr->Axis("yz");
	gr->Label('y',"Y axis",0);
	gr->Label('z',"Z axis",0);
	gr->Puts(mglPoint(0,0,1.5),"Remove X axis, and");
	gr->Puts(mglPoint(0,0,1.2),"decrease number of ticks");
	return 0;
}
//-----------------------------------------------------------------------------
int sample_a1(mglGraph *gr, const void *)	// transformation
{
	gr->SubPlot(2,2,0);  // just new axis without rotation and aspects
	gr->Box();
	gr->Text(mglPoint(-1,1.1,1),"Just box","rL");
	gr->InPlot(0.2,0.5,0.7,1);
	gr->Box();
	gr->Text(mglPoint(0,1.2,1),"InPlot example");
	gr->SubPlot(2,2,1);  // new axis with aspect and rotation
	gr->Rotate(60,40);
	gr->Aspect(1,1,1);
	gr->Box();
	gr->Text(mglPoint(1,1,1.5),"Rotate only","rR");
	gr->SubPlot(2,2,2);  // aspect in other direction
	gr->Rotate(60,40);
	gr->Aspect(1,1,2);
	gr->Box();
	gr->Text(mglPoint(0,0,2),"Aspect and Rotate");
	gr->SubPlot(2,2,3);  // rotation before aspect. INCORRECT !!!
	gr->Rotate(60,40);
	gr->Aspect(1,2,2);
	gr->Box();
	gr->Text(mglPoint(0,0,1.5),"Aspect in other direction");
	return 0;
}
//-----------------------------------------------------------------------------
int sample_1d(mglGraph *gr, const void *s)	// full test (in PNG)
{
	const char *suf =(const char *)s;
	mglData y(50,3), x(50), x2(50), y1(50), y2(50), ys(10,3);
	ys.Modify("0.8*sin(pi*(2*x+y/2))+0.2*rnd");
	y.Modify("0.7*sin(2*pi*x) + 0.5*cos(3*pi*x) + 0.2*sin(pi*x)",0);
	y.Modify("sin(2*pi*x)",1);	y.Modify("cos(2*pi*x)",2);
	y1.Modify("0.5+0.3*cos(2*pi*x)");	y2.Modify("0.3*sin(2*pi*x)");
	x.Fill(-1,1,'x');			x2.Modify("0.05+0.03*cos(2*pi*x)");

	mglData  x0(10), y0(10), ex0(10), ey0(10),ch(7,2);
	x0.Modify("2*x-1 + 0.1*rnd-0.05");
	y0.Modify("0.7*sin(2*pi*x) + 0.5*cos(3*pi*x) + 0.2*sin(pi*x) + 0.2*rnd-0.1");
	ey0.Modify("0.2");	ex0.Modify("0.1");	ch.Modify("rnd+0.1");

	gr->Org = mglPoint();
	gr->Clf();	gr->Box();	gr->Plot(y);	save(gr,"plot",suf);
	gr->Clf();	gr->Box();	gr->Tens(y.SubData(-1,0), y.SubData(-1,1));	save(gr,"tens",suf);
	gr->Clf();	gr->Box();	gr->Area(y);	save(gr,"area",suf);
	gr->Clf();	gr->Box();	gr->Stem(y);	save(gr,"stem",suf);
	gr->Clf();	gr->Box();	gr->Step(y);	save(gr,"step",suf);
	gr->Clf();	gr->Box();	gr->Bars(ys);	save(gr,"bars",suf);
	gr->Clf();	gr->Box();	gr->Barh(ys);	save(gr,"barh",suf);
	gr->Clf();	gr->Box();	gr->Region(y2,y1,"r");	gr->Plot(y1,"k2");	gr->Plot(y2,"k2");	save(gr,"region",suf);
	gr->Clf();	gr->Box();	gr->Mark(y,y1,"bs");	save(gr,"mark",suf);
	gr->Clf();	gr->Box();	gr->TextMark(y,y1,"\\gamma");	save(gr,"textmark",suf);
	gr->Clf();	gr->Box();	gr->Plot(y.SubData(-1,0));
	gr->Text(y,"This is very very long string drawn along a curve",":k");
	gr->Text(y,"Another string drawn above a curve","T:r");	save(gr,"text",suf);
	gr->Clf();	gr->Box();	gr->Plot(y.SubData(-1,0));	gr->Error(x0,y0,ex0,ey0,"ko");	save(gr,"error",suf);
	gr->Light(true);	gr->Rotate(40,60);
	gr->VertexColor(false);	// let renderer do the lighting where necessary, since all surfeces are one-color ones
	gr->Unrotate(true);		// we rely on PDF making utility to correctly set camera
	gr->Clf();	gr->Box();	gr->Torus(y1,y2,"pz");		save(gr,"torus",suf);
	gr->Clf();	gr->Box();	gr->Tube(y,0.05);	save(gr,"tube",suf);
	gr->Clf();	gr->Box();	gr->Tube(y,0.05);	gr->Tube(y1,x,y2,x2);	save(gr,"tube_3d",suf);
	gr->Clf();	gr->Box();	gr->Chart(ch,"#");	save(gr,"chart",suf);
	gr->Axis("(y+1)/2*cos(pi*x)","(y+1)/2*sin(pi*x)","");
	gr->Clf();	gr->Box();	gr->Chart(ch,"bgr cmy#");	save(gr,"pie_chart",suf);	gr->Axis(0,0,0);
	gr->Unrotate(false);	// set back
	return 0;
}
//-----------------------------------------------------------------------------
int sample_2d(mglGraph *gr, const void *s)	// full test (in PNG)
{
	const char *suf =(const char *)s;
	mglData  a(50,40), b(50,40);
	a.Modify("0.6*sin(2*pi*x)*sin(3*pi*y) + 0.4*cos(3*pi*(x*y))");
	b.Modify("0.6*cos(2*pi*x)*cos(3*pi*y) + 0.4*cos(3*pi*(x*y))");

	gr->VertexColor(true);
	gr->Clf();	gr->Box();	gr->Cont(a,"BbcyrRt");	save(gr,"contt",suf);
	gr->Clf();	gr->Box();	gr->TileS(a,b,"BbcyrR");save(gr,"tiler",suf);
	gr->Clf();	gr->Box();	gr->Dens(a);	gr->Colorbar();	save(gr,"dens",suf);
	gr->Light(true);	gr->Rotate(40,60);
	gr->Clf();	gr->Box();	gr->Surf(a,"BbcyrR|");	save(gr,"surf_sl",suf);
	gr->Clf();	gr->Box();	gr->Surf(a,"BbcyrR");	save(gr,"surf",suf);
	gr->Clf();	gr->Box();	gr->Tile(a);			save(gr,"tile",suf);
	gr->Clf();	gr->Box();	gr->Belt(a);			save(gr,"belt",suf);
	gr->Clf();	gr->Box();	gr->Mesh(a);			save(gr,"mesh",suf);
	gr->Clf();	gr->Box();	gr->Fall(a);			save(gr,"fall",suf);
	gr->Clf();	gr->Box();	gr->SurfC(a,b);			save(gr,"surfc",suf);
	gr->Clf();	gr->Box();	gr->Org = mglPoint();	gr->Boxs(a);	save(gr,"boxs",suf);
	gr->Clf();	gr->Box();	gr->ContF(a);			save(gr,"contf",suf);
	gr->Clf();	gr->Box();	gr->Cont(a);			save(gr,"cont",suf);
	gr->Clf();	gr->Fog(1);	gr->Box();	type==5?gr->Text(mglPoint(0,0,0),"Fog not supported"):gr->Surf(a);	save(gr,"surf_fog",suf);
	gr->Fog(0);	gr->Alpha(true);
	gr->Clf();	gr->Box();	gr->Surf(a);			save(gr,"surf_alpha",suf);
	gr->VertexColor(false);
	gr->Clf();	gr->Box();	gr->Axial(a);			save(gr,"axial",suf);
	gr->Clf();	gr->Box();	gr->SurfA(a,b);			save(gr,"surfa",suf);
	return 0;
}
//-----------------------------------------------------------------------------
int sample_3d(mglGraph *gr, const void *s)	// full test (in PNG)
{
	const char *suf =(const char *)s;
	mglData  c(60,50,40), d(60,50,40);
	c.Modify("(-2*((2*x-1)^2 + (2*y-1)^2 + (2*z-1)^4 - (2*z-1)^2 - 0.1))");
	d.Modify("1-2*tanh(4*(x+y-1)^2)");

	gr->VertexColor(true);
	gr->Rotate(40,60);
	gr->Clf();	gr->Box();	gr->Surf3(c,"bgrd");	save(gr,"surf3_rgbd",suf);
	gr->Clf();	gr->Box();	gr->ContA(c,"BbcyrR");	save(gr,"conta",suf);
	gr->Clf();	gr->Box();	gr->DensX(c.Sum("x"),"",-1);	gr->DensY(c.Sum("y"),"",1);
	gr->DensZ(c.Sum("z"),"",-1);	save(gr,"dens_xyz",suf);
	gr->Clf();	gr->Box();	gr->ContX(c.Sum("x"),"",-1);	gr->ContY(c.Sum("y"),"",1);
	gr->ContZ(c.Sum("z"),"",-1);	save(gr,"cont_xyz",suf);
	gr->Light(true);
	gr->VertexColor(false);	//  colors are few - it is better to have rendered lighting
	gr->Clf();	gr->Box();	gr->ContFA(c);		save(gr,"contfa",suf);
	gr->Alpha(true);
	gr->Clf();	gr->Box();	gr->Surf3(c);		save(gr,"surf3",suf);
	gr->VertexColor(false);	// for variable transparency the only way is to disable smooth (per-vertex) coloring
	gr->Clf();	gr->Box();	gr->Surf3A(c,d);	save(gr,"surf3a",suf);
	gr->VertexColor(true);
	gr->Clf();	gr->Box();	gr->Surf3C(c,d);	save(gr,"surf3c",suf);
	gr->Light(false);
	gr->Clf();	gr->Box();	gr->Org=mglPoint();	gr->Axis();	gr->DensA(c);	save(gr,"densa",suf);
	gr->Clf();	gr->Box();	type==5?gr->Text(mglPoint(0,0,0),"Clouds not supported"):gr->CloudQ(c,"wyrRk");		save(gr,"cloudq",suf);
	gr->Clf();	gr->Box();	type==5?gr->Text(mglPoint(0,0,0),"Clouds not supported"):gr->CloudP(c,"wyrRk");		save(gr,"cloudp",suf);
	return 0;
}
//-----------------------------------------------------------------------------
int sample_vect2d(mglGraph *gr, const void *s)	// full test (in PNG)
{
	const char *suf =(const char *)s;
	mglData  a(20,30), b(20,30);
	a.Modify("0.6*sin(2*pi*x)*sin(3*pi*y) + 0.4*cos(3*pi*(x*y))");
	b.Modify("0.6*cos(2*pi*x)*cos(3*pi*y) + 0.4*cos(3*pi*(x*y))");

	gr->VertexColor(true);
	gr->Clf();	gr->Box();	gr->Vect(a,b);	save(gr,"vect",suf);
	gr->Clf();	gr->Box();	gr->VectL(a,b);	save(gr,"vectl",suf);
	gr->Clf();	gr->Box();	gr->VectC(a,b);	save(gr,"vectc",suf);
	gr->Clf();	gr->Box();	gr->Flow(a,b);	save(gr,"flow",suf);
	gr->Clf();	gr->Box();	gr->Pipe(a,b);	save(gr,"pipe2",suf);
	gr->Light(true);
	gr->VertexColor(false);
	gr->Compression(true);  // try to save space
	gr->Clf();	gr->Box();	gr->Pipe(a,b);	save(gr,"pipe",suf);
	gr->Clf();	gr->Box();	gr->MeshNum=20;	gr->Dew(a,b);	save(gr,"dew",suf);
	gr->Rotate(40,60);
	gr->Clf();	gr->Box();	gr->MeshNum=10;	gr->Dew(a,b);	save(gr,"dew2",suf);
	gr->Compression(false);  //put setting back
	return 0;
}
//-----------------------------------------------------------------------------
int sample_vect3d(mglGraph *gr, const void *s)	// full test (in PNG)
{
	const char *suf =(const char *)s;
	mglData ex(10,10,10), ey(10,10,10), ez(10,10,10);
	ex.Fill("0.2*x/pow(x^2+y^2+(z-0.3)^2,1.5) - 0.2*x/pow(x^2+y^2+(z+0.3)^2,1.5)", gr->Min, gr->Max);
	ey.Fill("0.2*y/pow(x^2+y^2+(z-0.3)^2,1.5) - 0.2*y/pow(x^2+y^2+(z+0.3)^2,1.5)", gr->Min, gr->Max);
	ez.Fill("0.2*(z-0.3)/pow(x^2+y^2+(z-0.3)^2,1.5) - 0.2*(z+0.3)/pow(x^2+y^2+(z+0.3)^2,1.5)", gr->Min, gr->Max);

	gr->Rotate(40,60);
	gr->Clf();	gr->Box();	gr->Flow(ex,ey,ez,"bwr");	save(gr,"flow3",suf);
	gr->Clf();	gr->Box();	gr->Vect(ex,ey,ez);		save(gr,"vect3",suf);
	gr->Clf();	gr->Box();	gr->VectL(ex,ey,ez);	save(gr,"vectl3",suf);
	gr->Clf();	gr->Box();	gr->VectC(ex,ey,ez);	save(gr,"vectc3",suf);
	gr->Light(true);
	gr->VertexColor(false);
	gr->Compression(true);  // try to save space
	gr->Clf();	gr->Box();	gr->Pipe(ex,ey,ez);		save(gr,"pipe3",suf);
	gr->Compression(false);  //put setting back
	return 0;
}
//-----------------------------------------------------------------------------
int sample_crust(mglGraph *gr, const void *s)
{
	const char *suf =(const char *)s;
	mglData pnts("hotdogs.pts");
	pnts.Norm(-1,1,true);

	gr->VertexColor(true);
	gr->Rotate(40,60);
	gr->Light(true);	gr->Alpha(true);
	gr->Clf();	gr->Box();	gr->Crust(pnts);	save(gr,"crust",suf);
	gr->Clf();	gr->Box();	gr->Dots(pnts);		save(gr,"dots",suf);
	return 0;
}
//-----------------------------------------------------------------------------
int sample_legend(mglGraph *gr, const void *s)
{
	const char *suf =(const char *)s;
	mglData f(50,3);
	f.Modify("sin(2*pi*x*x)",0);	f.Modify("sin(2*pi*x)",1);	f.Modify("sin(2*pi*sqrt(x))",2);
	gr->Axis(mglPoint(0), mglPoint(1));
	gr->Clf();	gr->Box();	gr->Plot(f);	gr->Axis("xy");
	if(type==5)	gr->LegendBox = false;
	gr->AddLegend("sin(\\pi {x^2})","b");	gr->AddLegend("sin(\\pi x)","g*");
	gr->AddLegend("sin(\\pi \\sqrt{x})","r+");	gr->Legend();	save(gr,"legend",suf);
	return 0;
}
//-----------------------------------------------------------------------------
int full_test(mglGraph *gr, const void *s)	// full test (in PNG)
{
	gr->DefaultPlotParam();	sample_1d(gr,s);
	gr->DefaultPlotParam();	sample_2d(gr,s);
	gr->DefaultPlotParam();	sample_3d(gr,s);
	gr->DefaultPlotParam();	sample_crust(gr,s);
	gr->DefaultPlotParam();	sample_legend(gr,s);
	gr->DefaultPlotParam();	sample_vect2d(gr,s);
	gr->DefaultPlotParam();	sample_vect3d(gr,s);
	printf("\n");	fflush(stdout);
	return 0;
}
//-----------------------------------------------------------------------------
int sample_transp(mglGraph *gr, const void *s)	// flag #
{
	gr->DefaultPlotParam();
	if(type==5)
	{
		const char *suf =(const char *)s;
		gr->Text(mglPoint(0,0,0),"Transparency types not supported");
		save(gr,"type0",suf);
		save(gr,"type1",suf);
		save(gr,"type2",suf);
		return 0;
	}
	const char *suf =(const char *)s;
	gr->Alpha(true);	gr->Light(true);	gr->Light(0,mglPoint(0,0,1));
	mglData a(30,20);
	a.Modify("0.6*sin(2*pi*x)*sin(3*pi*y) + 0.4*cos(3*pi*(x*y))");

	gr->SetScheme("BbcyrR");
	gr->TranspType = 0;	gr->Clf();
	gr->SubPlot(2,2,0);	gr->Rotate(40,60);	gr->Surf(a);	gr->Box();
	gr->SubPlot(2,2,1);	gr->Rotate(40,60);	gr->Dens(a);	gr->Box();
	gr->SubPlot(2,2,2);	gr->Rotate(40,60);	gr->Cont(a);	gr->Box();
	gr->SubPlot(2,2,3);	gr->Rotate(40,60);	gr->Axial(a);	gr->Box();
	save(gr,"type0",suf);

	gr->TranspType = 1;	gr->Clf();
	gr->SubPlot(2,2,0);	gr->Rotate(40,60);	gr->Surf(a);	gr->Box();
	gr->SubPlot(2,2,1);	gr->Rotate(40,60);	gr->Dens(a);	gr->Box();
	gr->SubPlot(2,2,2);	gr->Rotate(40,60);	gr->Cont(a);	gr->Box();
	gr->SubPlot(2,2,3);	gr->Rotate(40,60);	gr->Axial(a);	gr->Box();
	save(gr,"type1",suf);

	gr->TranspType = 2;	gr->Clf();
	gr->SubPlot(2,2,0);	gr->Rotate(40,60);	gr->Surf(a);	gr->Box();
	gr->SubPlot(2,2,1);	gr->Rotate(40,60);	gr->Dens(a);	gr->Box();
	gr->SubPlot(2,2,2);	gr->Rotate(40,60);	gr->Cont(a);	gr->Box();
	gr->SubPlot(2,2,3);	gr->Rotate(40,60);	gr->Axial(a);	gr->Box();
	save(gr,"type2",suf);
	gr->TranspType = 0;	gr->Clf();
	return 0;
}
//-----------------------------------------------------------------------------
int sample_hint(mglGraph *gr, const void *s)
{
	const char *suf =(const char *)s;
	mglData  a(50,40), b(50,40), d(50,40), c(61,51,40), y(50,3), y1(50), y2(50);
	a.Modify("0.6*sin(2*pi*x)*sin(3*pi*y) + 0.4*cos(3*pi*(x*y))");
	b.Modify("0.6*cos(2*pi*x)*cos(3*pi*y) + 0.4*cos(3*pi*(x*y))");
	c.Modify("(-2*((2*x-1)^2 + (2*y-1)^2 + (2*z-1)^4 - (2*z-1)^2 - 0.1))");
	y.Modify("0.7*sin(2*pi*x) + 0.5*cos(3*pi*x) + 0.2*sin(pi*x)",0);
	y.Modify("sin(2*pi*x)",1);	y.Modify("cos(2*pi*x)",2);
	y1.Modify("0.3*sin(2*pi*x)");	y2.Modify("0.5+0.3*cos(2*pi*x)");
	for(int i=0;i<a.nx*a.ny;i++)	d.a[i] = hypot(a.a[i],b.a[i]);

	gr->DefaultPlotParam();
	// 2-axis plot
	gr->Clf();	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1),mglPoint(-1,-1,-1));
	gr->Axis();	gr->Label('y',"axis 1",0);	gr->Plot(y1,"b");
	gr->Axis(mglPoint(0,0,0),mglPoint(1,1,1),mglPoint(1,1,1));	// set up second axis
	gr->Axis();	gr->Label('y',"axis 2",0);	gr->Stem(y2,"r");
	save(gr,"2_axis",suf);

	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1));
	gr->VertexColor(true);
	// contours under mesh
	gr->Clf();	gr->Flow(a,b,"br",5,true,type==5?-0.99:NAN);	gr->Dens(d,"BbcyrR");	gr->Box();
	save(gr,"flow_dens",suf);

	gr->Unrotate(true); // we rely on PDF making utility to correctly set camera
	gr->Rotate(40,60);
	// contours on the surface
	gr->Clf();		gr->Surf(a,"kw");	gr->CAxis(-1,0);	gr->Cont(a,"w");
	gr->CAxis(0,1);	gr->Cont(a,"k");	gr->CAxis(-1,1);	gr->Box();
	save(gr,"surf_cont",suf);
	gr->Light(true);
	// contours under mesh
	gr->Clf();	gr->Mesh(a,"BbcyrR");	gr->Cont(a, 0, 7, -1);	gr->Box();
	save(gr,"mesh_cont",suf);
	// contours on the surface
	gr->Clf();	gr->Surf(a,"BbcyrR");	gr->Cont(a,"y");	gr->Box();
	save(gr,"surf_cont_y",suf);
	gr->Clf();	gr->Fog(1,0.1);	gr->Surf(a,"BbcyrR");	gr->Cont(a,"y");	gr->Box();
	save(gr,"surf_cont_fog",suf);
	// caxis and the surface
	gr->Clf();	gr->CAxis(0,1);	gr->Surf(a,"BbcyrR");	gr->Box();	gr->CAxis(-1,1);
	save(gr,"surf_caxis",suf);
	// caxis and the surface
	gr->Clf();	gr->Axis(mglPoint(-1,-1,0),mglPoint(1,1,1));	gr->Cut=false;
	gr->Surf(a,"BbcyrR");	gr->Box();	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1));
	save(gr,"surf_cut",suf);	gr->Cut = true;
	// several light sources
	gr->Light(1,mglPoint(0,1,0),'c');	gr->Light(2,mglPoint(1,0,0),'y');
	gr->Light(3,mglPoint(0,-1,0),'m');	gr->Clf();	gr->Box();
	gr->Surf(a,"h");	gr->Light(1,false);	gr->Light(2,false);	gr->Light(3,false);
	save(gr,"several_light",suf);

	gr->VertexColor(false);	gr->Compression(false); // per-vertex colors and compression are detrimental to transparency
	// CutMin CutMax example
	gr->CutMin = mglPoint(0,-1,-1);	gr->CutMax = mglPoint(1,0,1.1);
	gr->Clf();	gr->Box();	gr->Surf3(-0.5,c,"BbcyrR");
	mglData v(10);	v.Fill(-0.5,1);
	gr->ContF3(v,c,'x',-1,"BbcyrR");	gr->ContF3(v,c,'y',-1,"BbcyrR");
	gr->ContF3(v,c,'z',0,"BbcyrR");		gr->ContF3(v,c,'z',39,"BbcyrR");
	save(gr,"cutminmax2",suf);

	gr->Clf();	gr->Box();	gr->Alpha(true);	gr->Surf3(c,"BbcyrR");
	save(gr,"cutminmax",suf);
	gr->CutMax = gr->CutMin;	// switch off cutting area

	gr->Clf();	gr->Box();
	gr->CutOff("(z>(x+0.5*y-1)^2-1) & (z>(x-0.5*y-1)^2-1)");
	gr->Surf3(c);	gr->CutOff(0);
	save(gr,"surf3_cutoff",suf);

	printf("\n");	fflush(stdout);
	gr->Unrotate(false); // set back
	return 0;
}
//-----------------------------------------------------------------------------
int sample_molecule(mglGraph *gr, const void *)	// flag #
{
	gr->VertexColor(false);	gr->Compression(false); // per-vertex colors and compression are detrimental to transparency
	gr->Alpha(true);	gr->Light(true);

	gr->SubPlot(2,2,0);
	gr->StartGroup("Methane");
	gr->StartGroup("MetaneLabel");
	gr->Puts(mglPoint(0,1.2),"Methane, CH_4",0,-2);
	gr->EndGroup();
	gr->Rotate(60,120);
	gr->Sphere(mglPoint(0,0,0),0.25,"k");
	gr->Drop(mglPoint(0,0,0),mglPoint(0,0,1),0.35,"h",1,2);
	gr->Sphere(mglPoint(0,0,0.7),0.25,"g");
	gr->Drop(mglPoint(0,0,0),mglPoint(-0.94,0,-0.33),0.35,"h",1,2);
	gr->Sphere(mglPoint(-0.66,0,-0.23),0.25,"g");
	gr->Drop(mglPoint(0,0,0),mglPoint(0.47,0.82,-0.33),0.35,"h",1,2);
	gr->Sphere(mglPoint(0.33,0.57,-0.23),0.25,"g");
	gr->Drop(mglPoint(0,0,0),mglPoint(0.47,-0.82,-0.33),0.35,"h",1,2);
	gr->Sphere(mglPoint(0.33,-0.57,-0.23),0.25,"g");
	gr->EndGroup();

	gr->SubPlot(2,2,1);
	gr->StartGroup("Water");
	gr->StartGroup("WaterLabel");
	gr->Puts(mglPoint(0,1.2),"Water, H_{2}O",0,-2);
	gr->EndGroup();
	gr->Rotate(60,100);
	gr->StartGroup("Water_O");
	gr->Sphere(mglPoint(0,0,0),0.25,"r");
	gr->EndGroup();
	gr->StartGroup("Water_Bond_1");
	gr->Drop(mglPoint(0,0,0),mglPoint(0.3,0.5,0),0.3,"m",1,2);
	gr->EndGroup();
	gr->StartGroup("Water_H_1");
	gr->Sphere(mglPoint(0.3,0.5,0),0.25,"g");
	gr->EndGroup();
	gr->StartGroup("Water_Bond_2");
	gr->Drop(mglPoint(0,0,0),mglPoint(0.3,-0.5,0),0.3,"m",1,2);
	gr->EndGroup();
	gr->StartGroup("Water_H_2");
	gr->Sphere(mglPoint(0.3,-0.5,0),0.25,"g");
	gr->EndGroup();
	gr->EndGroup();

	gr->SubPlot(2,2,2);
	gr->StartGroup("Oxygen");
	gr->StartGroup("OxygenLabel");
	gr->Puts(mglPoint(0,1.2),"Oxygen, O_2",0,-2);
	gr->EndGroup();
	gr->Rotate(60,120);
	gr->Drop(mglPoint(0,0.5,0),mglPoint(0,-0.3,0),0.3,"m",1,2);
	gr->Sphere(mglPoint(0,0.5,0),0.25,"r");
	gr->Drop(mglPoint(0,-0.5,0),mglPoint(0,0.3,0),0.3,"m",1,2);
	gr->Sphere(mglPoint(0,-0.5,0),0.25,"r");
	gr->EndGroup();

	gr->SubPlot(2,2,3);
	gr->StartGroup("Ammonia");
	gr->StartGroup("AmmoniaLabel");
	gr->Puts(mglPoint(0,1.2),"Ammonia, NH_3",0,-2);
	gr->EndGroup();
	gr->Rotate(60,120);
	gr->Sphere(mglPoint(0,0,0),0.25,"b");
	gr->Drop(mglPoint(0,0,0),mglPoint(0.33,0.57,0),0.32,"n",1,2);
	gr->Sphere(mglPoint(0.33,0.57,0),0.25,"g");
	gr->Drop(mglPoint(0,0,0),mglPoint(0.33,-0.57,0),0.32,"n",1,2);
	gr->Sphere(mglPoint(0.33,-0.57,0),0.25,"g");
	gr->Drop(mglPoint(0,0,0),mglPoint(-0.65,0,0),0.32,"n",1,2);
	gr->Sphere(mglPoint(-0.65,0,0),0.25,"g");
	gr->EndGroup();

	return 0;
}
//-----------------------------------------------------------------------------
int sample_ternary(mglGraph *gr, const void *)	// flag #
{
	mglData x(50),y(50),rx(10),ry(10), a(20,30);
	a.Modify("4*x*y");
	x.Modify("0.25*(1+cos(2*pi*x))");
	y.Modify("0.25*(1+sin(2*pi*x))");
	rx.Modify("rnd"); ry.Modify("(1-v)*rnd",rx);

	gr->Text(mglPoint(-0.8,1.3), "Ternary plot (x+y+t=1)");
	gr->Ternary(true);
	gr->Plot(x,y,"r2");
	gr->Plot(rx,ry,"q^ ");
	gr->Cont(a,"BbcyrR");
	gr->Line(mglPoint(0.5,0), mglPoint(0,0.75), "g2");
	gr->Axis(); gr->Grid("xyz","B;");
	gr->Label('x',"x comp.");
	gr->Label('y',"y comp.");
	gr->Label('t',"t comp.");
	gr->Ternary(false);
	return 0;
}
//-----------------------------------------------------------------------------
int sample_drops(mglGraph *gr, const void *)	// flag #
{
	gr->VertexColor(false);	// not strictly required, but looks better imho
	gr->Light(true);	gr->Alpha(false);
	gr->Text(mglPoint(-1,1.2),"sh=0");
	gr->Drop(mglPoint(-1,0),mglPoint(0,1),0.5,"r",0);
	gr->Text(mglPoint(-0.33,1.2),"sh=0.33");
	gr->Drop(mglPoint(-0.33,0),mglPoint(0,1),0.5,"r",0.33);
	gr->Text(mglPoint(0.33,1.2),"sh=0.67");
	gr->Drop(mglPoint(0.33,0),mglPoint(0,1),0.5,"r",0.67);
	gr->Text(mglPoint(1,1.2),"sh=1");
	gr->Drop(mglPoint(1,0),mglPoint(0,1),0.5,"r",1);
	gr->Ball(mglPoint(-1,0,1),'k');
	gr->Ball(mglPoint(-0.33,0,1),'k');
	gr->Ball(mglPoint(0.33,0,1),'k');
	gr->Ball(mglPoint(1,0,1),'k');
	gr->Line(mglPoint(-1,0,1),mglPoint(1,0,1),"b");
	return 0;
}
//-----------------------------------------------------------------------------
int sample_fish(mglGraph *gr, const void *)	// flag #
{
	gr->VertexColor(false); // the model is big even without information on point colors
	gr->Compression(true);  // fishes are important, not their scales
	gr->Light(true);	gr->Alpha(false);
	mglData a(10,10), b(10,10), x(10,10), y(10,10);
	a.Modify("0.3*x+rnd"); x.Modify("x+0.1*rnd");
	b.Modify("0.3*y+rnd"); y.Modify("y+0.1*rnd");

	gr->Clf();
	gr->Axis(mglPoint(0,0),mglPoint(1.1,1.1));
	gr->Light(true);
	gr->Dew(x,y,a,b,"BbcyrR");
	gr->Compression(false);  // put it back
	return 0;
}
//-----------------------------------------------------------------------------
int sample_mirror(mglGraph *gr, const void *)	// flag #
{
	gr->VertexColor(true);
	mglData a(30,40),x(30),y1(40),y2(40);
	a.Modify("pi*(1-2*x)*exp(-4*y^2-4*(2*x-1)^2)");
	x.Fill(-1,1); y1.Fill(0,1); y2.Fill(0,-1);

	gr->Alpha(false);
	gr->Rotate(40,60);
	gr->Light(true);	gr->Box();
	gr->Surf(x,y1,a,"r"); gr->Surf(x,y2,a,"b");
	return 0;
}
//-----------------------------------------------------------------------------
int sample_semilog(mglGraph *gr, const void *)	// flag #
{
	mglData x(2000), y(2000);
	x.Modify("0.01/(x+10^(-5))"); y.Modify("sin(1/v)",x);

	gr->Axis(mglPoint(0.01,-1),mglPoint(1000,1));
	gr->Axis("lg(x)",0,0); gr->SetTicks('x', 0);	gr->Box();
	gr->Plot(x,y,"b2");
	gr->Axis(); gr->Grid("xy","g");
	gr->Label('x',"x",0); gr->Label('y', "y = sin 1/x",0);
	return 0;
}
//-----------------------------------------------------------------------------
int sample_loglog(mglGraph *gr, const void *s)	// flag #
{
	mglData x(2000), y(2000);
	x.Modify("pow(10,6*x-3)"); y.Modify("sqrt(1+v^2)",x);

	gr->Axis(mglPoint(0.001,0.1),mglPoint(1000,1000));
	gr->Axis("lg(x)","lg(y)",0);
	gr->SetTicks('x', 0);
	gr->SetTicks('y', 0);
	gr->Box();	gr->Axis(); gr->Grid("xy","g;");
	gr->Plot(x,y,"b2");
	gr->Label('x',"x",0); gr->Label('y', "y=\\sqrt{1+x^2}",0);
	return 0;
}
//-----------------------------------------------------------------------------
int sample_fit(mglGraph *gr, const void *s)	// flag #
{
	mglData rnd(100), in(100), res;
	rnd.Fill("0.4*rnd+0.1+sin(2*pi*x)", gr->Min, gr->Max);
	in.Fill("0.3+sin(2*pi*x)", gr->Min, gr->Max);

	gr->Axis(mglPoint(-1,-2), mglPoint(1,2));
	gr->Plot(rnd, ". ");
	gr->Box();

	mreal ini[3] = {1,1,3};
	gr->Fit(res, rnd, "a+b*sin(c*x)", "abc", ini);
	gr->Plot(res, "r");
	gr->Plot(in, "b");
	gr->Text(mglPoint(-1, -1.3), "fitted:", "L:r", -1);
	gr->PutsFit(mglPoint(0, -1.8), "y = ", "C:r", -1);
	gr->Text(mglPoint(0, 2.2), "initial: y = 0.3+sin(2\\pi x)", "C:b", -1);
	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1),mglPoint(0,0,0));
	return 0;
}
//-----------------------------------------------------------------------------
#include <mgl/mgl_parse.h>
int sample_parser(mglGraph *gr, const void *)	// flag #
{
	float a[100];   // let a_i = sin(4*pi*x), x=0...1
	for(int i=0;i<100;i++)a[i]=sin(4*M_PI*i/99);
	mglParse *parser = new mglParse;
	mglData &d =(parser->AddVar("dat"))->d;
	d.Set(a,100); // set data to variable
	parser->Execute(gr, "plot dat; xrange 0 1\nbox\naxis");
	// you may break script at any line do something
	// and continue after that
	parser->Execute(gr, "xlabel 'x'\nylabel 'y'\nbox");
	// also you may use cycles or conditions in script
	parser->Execute(gr, "for $0 -1 1 0.1\nline 0 0 -1 $0 'r'\nnext");
	return 0;
}
//-----------------------------------------------------------------------------
int all_samples(mglGraph *gr, const void *s)
{
	const char *suf =(const char *)s;
	gr->DefaultPlotParam();	sample_colors(gr,0);save(gr,"colors",  suf);
	gr->DefaultPlotParam();	sample_a1(gr,0);	save(gr,"sample1", suf);
	gr->DefaultPlotParam();	sample_a2(gr,0);	save(gr,"sample2", suf);
	gr->DefaultPlotParam();	sample_a3(gr,0);	save(gr,"sample3", suf);
	gr->DefaultPlotParam();	sample_a4(gr,0);	save(gr,"sample4", suf);
	gr->DefaultPlotParam();	sample_a5(gr,0);	save(gr,"sample5", suf);
	gr->DefaultPlotParam();	sample_a6(gr,0);	save(gr,"sample6", suf);
	gr->DefaultPlotParam();	sample_a7(gr,0);	save(gr,"sample7", suf);
	gr->DefaultPlotParam();	sample_a8(gr,0);	save(gr,"sample8", suf);
	gr->DefaultPlotParam();	sample_a9(gr,0);	save(gr,"sample9", suf);
	gr->DefaultPlotParam();	sample_aa(gr,0);	save(gr,"samplea", suf);
	gr->DefaultPlotParam();	sample_ab(gr,0);	save(gr,"sampleb", suf);
	gr->DefaultPlotParam();	sample_ac(gr,0);	save(gr,"samplec", suf);
	gr->DefaultPlotParam();	sample_ad(gr,0);	save(gr,"sampled", suf);
	gr->DefaultPlotParam();	sample_ae(gr,0);	save(gr,"samplee", suf);
	gr->DefaultPlotParam();	sample_sew(gr,0);	save(gr,"sew",     suf);
	gr->DefaultPlotParam();	sample_mirror(gr,0);save(gr,"mirror",  suf);
	gr->DefaultPlotParam();	sample_fit(gr,0);	save(gr,"fit",     suf);
	gr->DefaultPlotParam();	sample_stfa(gr,0);	save(gr,"stfa",    suf);
	gr->DefaultPlotParam();	sample_pde(gr,0);	save(gr,"pde",     suf);
	gr->DefaultPlotParam();	sample_qo2d(gr,0);	save(gr,"qo2d",    suf);
	gr->DefaultPlotParam();	sample_molecule(gr,0);	save(gr,"molecule",suf);
	gr->DefaultPlotParam();	sample_ternary(gr,0);	save(gr,"ternary", suf);
	gr->DefaultPlotParam();	sample_envelop(gr,0);	save(gr,"envelop", suf);
	gr->DefaultPlotParam();	sample_parser(gr,0);	save(gr,"parser",  suf);
	gr->DefaultPlotParam();	sample_drops(gr,0);		save(gr,"drops",   suf);
	gr->DefaultPlotParam();	sample_fish(gr,0);		save(gr,"fish",    suf);
	gr->DefaultPlotParam();	sample_loglog(gr,0);	save(gr,"loglog",  suf);
	gr->DefaultPlotParam();	sample_semilog(gr,0);	save(gr,"semilog", suf);
	gr->DefaultPlotParam();	sample_map(gr,0);		save(gr,"map", suf);
	gr->DefaultPlotParam();	sample_tval(gr,0);		save(gr,"tval", suf);
	gr->DefaultPlotParam();	sample_column(gr,0);	save(gr,"column", suf);
	printf("\n");	fflush(stdout);
	return 0;
}
//-----------------------------------------------------------------------------
static struct option longopts[] =
{
	{ "test",			no_argument,	&dotest,		1 },
	{ "srnd",			no_argument,	&srnd,			1 },
	{ "png",			no_argument,	&type,			0 },
	{ "width",			required_argument,	NULL,		'w' },
	{ "height",			required_argument,	NULL,		'h' },
	{ "mini",			no_argument,	&mini,			1 },
	{ "big",			no_argument,	&big,			1 },
	{ "idtf",			no_argument,	&type,			5 },
	{ "eps",			no_argument,	&type,			1 },
	{ "svg",			no_argument,	&type,			2 },
	{ "gif",			no_argument,	&type,			6 },
	{ "help",			no_argument,	NULL,			'?' },
	{ NULL,					0,	NULL,			0 }
};
//-----------------------------------------------------------------------------
void usage()
{
	puts (	\
		"-png			- output png\n"					\
		"-width=num		- png picture width\n"				\
		"-height=num		- png picture height\n"				\
		"-mini			- png picture is 200x133\n"			\
		"-big			- png picture is 1200x800\n"			\
		"-idtf			- output idtf\n"				\
		"-eps			- output EPS\n"					\
		"-svg			- output SVG\n"					\
		"-srnd			- produce the same random numbers in any run\n"	\
		"-test			- perform test\n"
		);
	exit(0);
};
//-----------------------------------------------------------------------------
#include "mgl/mgl_parse.h"
int test(mglGraph *gr)
{
	sample_legend(gr,"");
	gr->ShowImage("",false);
//	gr->Box();
//	gr->Puts(mglPoint(0,1),"Text \\a{over} \\u{under} \\frac{aaa}{bbb} \\sqrt{sss}");
//	gr->Puts(mglPoint(0), "\\sqrt{\\frac{\\alpha^{\\gamma^2}+\\overset 1{\\big\\infty}}{\\sqrt3{2+b}}}", 0, -4);
	return 0;

	mglParse par;
	par.AllowSetSize = true;
	FILE *fp=fopen("box.mgl","rt");
	par.Execute(gr,fp);
	fclose(fp);

	gr->ShowImage("",true);
	return 0;
}
//-----------------------------------------------------------------------------
int main(int argc,char **argv)
{
	const char *suf = "";
	int ch;
	mglGraphIDTF u3d;
	mglGraphZB zb;
	mglGraphPS ps;
	mglGraph *gr = NULL;
	while(( ch = getopt_long_only(argc, argv, "", longopts, NULL)) != -1)
		switch(ch)
		{
			case 0:		break;
			case 'w':	width = atoi(optarg);	break;
			case 'h':	height = atoi(optarg);	break;
			case '?':
			default:	usage();
		}

	if(dotest)
	{
//		test(&ps);
		test(&zb);
//		sample_ternary(&zb,"");
		//sample_crust(&zb,0);
		zb.WritePNG("test.png","",false);
/*		ps.WritePNG("test_2.png","",false);
		ps.WriteEPS("test.eps");
		ps.WriteSVG("test.svg");
		zb.WriteGIF("test.gif","");*/
		return 0;
	}
	if(type==5)			gr = &u3d;
	else if(type==1){	gr = &ps;	gr->SetSize(width,height);	}
	else if(type==2){	gr = &ps;	gr->SetSize(width,height);	}
	else			{	gr = &zb;	gr->SetSize(width,height);	}
	if(mini)		{	gr->SetSize(200,133);	suf = "_sm";	}
	if(big)
	{	gr->SetSize(1200,800);	suf = "_lg";	gr->BaseLineWidth = 2;	}

	if(srnd)mgl_srnd(1);
	gr->VertexColor(true);
	gr->Compression(false);
	all_samples(gr,suf);
	sample_transp(gr,suf);
	sample_hint(gr,suf);
	full_test(gr,suf);
	printf("\n");

	return 0;
}
//-----------------------------------------------------------------------------
