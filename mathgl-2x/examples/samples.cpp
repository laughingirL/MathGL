/***************************************************************************
 * samples.cpp is part of Math Graphic Library
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
#include "mgl2/mgl.h"
//-----------------------------------------------------------------------------
void mgls_prepare1d(mglData *y, mglData *y1=0, mglData *y2=0, mglData *x1=0, mglData *x2=0);
void mgls_prepare2d(mglData *a, mglData *b=0, mglData *v=0);
void mgls_prepare3d(mglData *a, mglData *b=0);
void mgls_prepare2v(mglData *a, mglData *b);
void mgls_prepare3v(mglData *ex, mglData *ey, mglData *ez);
extern int mini;
//-----------------------------------------------------------------------------
struct mglSample	/// Structure for list of samples
{
	const char *name;
	void (*func)(mglGraph*);
	const char *mgl;
};
//-----------------------------------------------------------------------------
void fexport(mglGraph *gr)
{
	mglData a,b,d;	mgls_prepare2v(&a,&b);	d = a;
	for(int i=0;i<a.nx*a.ny;i++)	d.a[i] = hypot(a.a[i],b.a[i]);
	mglData c;	mgls_prepare3d(&c);
	mglData v(10);	v.Fill(-0.5,1);
	gr->SubPlot(2,2,1,"");	gr->Title("Flow + Dens");
	gr->Flow(a,b,"br");	gr->Dens(d,"BbcyrR");	gr->Box();
	gr->SubPlot(2,2,0);	gr->Title("Surf + Cont");	gr->Rotate(50,60);
	gr->Light(true);	gr->Surf(a);	gr->Cont(a,"y");	gr->Box();
	gr->SubPlot(2,2,2);	gr->Title("Mesh + Cont");	gr->Rotate(50,60);
	gr->Box();	gr->Mesh(a);	gr->Cont(a,"_");
	gr->SubPlot(2,2,3);	gr->Title("Surf3 + ContF_3");gr->Rotate(50,60);
	gr->Box();	gr->ContF3(v,c,"z",0);	gr->ContF3(v,c,"x");	gr->ContF3(v,c);
	gr->SetCutBox(mglPoint(0,-1,-1), mglPoint(1,0,1.1));
	gr->ContF3(v,c,"z",c.nz-1);	gr->Surf3(-0.5,c);
	
	/*	gr->WriteJPEG("test.jpg");
	 *	gr->WritePNG("test.png");
	 *	gr->WriteBMP("test.bmp");
	 *	gr->WriteTGA("test.tga");
	 *	gr->WriteEPS("test.eps");
	 *	gr->WriteSVG("test.svg");
	 *	gr->WriteGIF("test.gif");*/
	
	gr->WriteXYZ("test.xyz");
	gr->WriteSTL("test.stl");
	gr->WriteOFF("test.off");
	gr->WriteTEX("test.tex");
	gr->WriteOBJ("test.obj","",true);
	//	gr->WriteX3D("test.x3d");
}
//-----------------------------------------------------------------------------
//		MGL functions for preparing data
//-----------------------------------------------------------------------------
const char *mmgl_dat_prepare = "\nstop\n\nfunc 'prepare1d'\n\
new y 50 3\nmodify y '0.7*sin(2*pi*x)+0.5*cos(3*pi*x)+0.2*sin(pi*x)'\n\
modify y 'sin(2*pi*x)' 1\nmodify y 'cos(2*pi*x)' 2\n\
new x1 50 'x'\nnew x2 50 '0.05-0.03*cos(pi*x)'\n\
new y1 50 '0.5-0.3*cos(pi*x)'\nnew y2 50 '-0.3*sin(pi*x)'\n\
return\n\nfunc 'prepare2d'\n\
new a 50 40 '0.6*sin(pi*(x+1))*sin(1.5*pi*(y+1))+0.4*cos(0.75*pi*(x+1)*(y+1))'\n\
new b 50 40 '0.6*cos(pi*(x+1))*cos(1.5*pi*(y+1))+0.4*cos(0.75*pi*(x+1)*(y+1))'\n\
return\n\nfunc 'prepare3d'\n\
new c 61 50 40 '-2*(x^2+y^2+z^4-z^2)+0.2'\n\
new d 61 50 40 '1-2*tanh((x+y)*(x+y))'\n\
return\n\nfunc 'prepare2v'\n\
new a 20 30 '0.6*sin(pi*(x+1))*sin(1.5*pi*(y+1))+0.4*cos(0.75*pi*(x+1)*(y+1))'\n\
new b 20 30 '0.6*cos(pi*(x+1))*cos(1.5*pi*(y+1))+0.4*cos(0.75*pi*(x+1)*(y+1))'\n\
return\n\nfunc 'prepare3v'\n\
define $1 pow(x*x+y*y+(z-0.3)*(z-0.3)+0.03,1.5)\n\
define $2 pow(x*x+y*y+(z+0.3)*(z+0.3)+0.03,1.5)\n\
new ex 10 10 10 '0.2*x/$1-0.2*x/$2'\n\
new ey 10 10 10 '0.2*y/$1-0.2*y/$2'\n\
new ez 10 10 10 '0.2*(z-0.3)/$1-0.2*(z+0.3)/$2'\nreturn\n";
//-----------------------------------------------------------------------------
//		Sample functions (v.2.*0)
//-----------------------------------------------------------------------------
const char *mmgl_solve="zrange 0 1\nnew x 20 30 '(x+2)/3*cos(pi*y)'\n"
"new y 20 30 '(x+2)/3*sin(pi*y)'\nnew z 20 30 'exp(-6*x^2-2*sin(pi*y)^2)'\n\n"
"subplot 2 1 0:title 'Cartesian space':rotate 30 -40\naxis 'xyzU':box\nxlabel 'x':ylabel 'y'"
"origin 1 1:grid 'xy'\nmesh x y z\n\n"
"# section along 'x' direction\nsolve u x 0.5 'x'\nvar v u.nx 0 1\n"
"evaluate yy y u v\nevaluate xx x u v\nevaluate zz z u v\nplot xx yy zz 'k2o'\n\n"
"# 1st section along 'y' direction\nsolve u1 x -0.5 'y'\nvar v1 u1.nx 0 1\n"
"evaluate yy y v1 u1\nevaluate xx x v1 u1\nevaluate zz z v1 u1\nplot xx yy zz 'b2^'\n\n"
"# 2nd section along 'y' direction\nsolve u2 x -0.5 'y' u1\n"
"evaluate yy y v1 u2\nevaluate xx x v1 u2\nevaluate zz z v1 u2\nplot xx yy zz 'r2v'\n\n"
"subplot 2 1 1:title 'Accompanied space'\nranges 0 1 0 1:origin 0 0\n"
"axis:box:xlabel 'i':ylabel 'j':grid2 z 'h'\n\n"
"plot u v 'k2o':line 0.4 0.5 0.8 0.5 'kA'\n"
"plot v1 u1 'b2^':line 0.5 0.15 0.5 0.3 'bA'\n"
"plot v1 u2 'r2v':line 0.5 0.7 0.5 0.85 'rA'\n";
void smgl_solve(mglGraph *gr)	// solve and evaluate
{
	gr->SetRange('z',0,1);
	mglData x(20,30), y(20,30), z(20,30), xx,yy,zz;
	gr->Fill(x,"(x+2)/3*cos(pi*y)");
	gr->Fill(y,"(x+2)/3*sin(pi*y)");
	gr->Fill(z,"exp(-6*x^2-2*sin(pi*y)^2)");

	gr->SubPlot(2,1,0);	gr->Title("Cartesian space");	gr->Rotate(30,-40);
	gr->Axis("xyzU");	gr->Box();	gr->Label('x',"x");	gr->Label('y',"y");
	gr->SetOrigin(1,1);	gr->Grid("xy");
	gr->Mesh(x,y,z);

	// section along 'x' direction
	mglData u = x.Solve(0.5,'x');
	mglData v(u.nx);	v.Fill(0,1);
	xx = x.Evaluate(u,v);	yy = y.Evaluate(u,v);	zz = z.Evaluate(u,v);
	gr->Plot(xx,yy,zz,"k2o");

	// 1st section along 'y' direction
	mglData u1 = x.Solve(-0.5,'y');
	mglData v1(u1.nx);	v1.Fill(0,1);
	xx = x.Evaluate(v1,u1);	yy = y.Evaluate(v1,u1);	zz = z.Evaluate(v1,u1);
	gr->Plot(xx,yy,zz,"b2^");

	// 2nd section along 'y' direction
	mglData u2 = x.Solve(-0.5,'y',u1);
	xx = x.Evaluate(v1,u2);	yy = y.Evaluate(v1,u2);	zz = z.Evaluate(v1,u2);
	gr->Plot(xx,yy,zz,"r2v");

	gr->SubPlot(2,1,1);	gr->Title("Accompanied space");
	gr->SetRanges(0,1,0,1);	gr->SetOrigin(0,0);
	gr->Axis();	gr->Box();	gr->Label('x',"i");	gr->Label('y',"j");
	gr->Grid(z,"h");

	gr->Plot(u,v,"k2o");	gr->Line(mglPoint(0.4,0.5),mglPoint(0.8,0.5),"kA");
	gr->Plot(v1,u1,"b2^");	gr->Line(mglPoint(0.5,0.15),mglPoint(0.5,0.3),"bA");
	gr->Plot(v1,u2,"r2v");	gr->Line(mglPoint(0.5,0.7),mglPoint(0.5,0.85),"rA");
}
//-----------------------------------------------------------------------------
const char *mmgl_triangulation="new x 100 '2*rnd-1':new y 100 '2*rnd-1':copy z x^2-y^2\n"
"new g 30 30:triangulate d x y\n"
"title 'Triangulation'\nrotate 50 60:box:light on\n"
"triplot d x y z:triplot d x y z '#k'\ndatagrid g x y z:mesh g 'm'\n";
void smgl_triangulation(mglGraph *gr)	// surface triangulation
{
	mglData x(100), y(100), z(100);
	gr->Fill(x,"2*rnd-1");	gr->Fill(y,"2*rnd-1");	gr->Fill(z,"v^2-w^2",x,y);
	mglData d = mglTriangulation(x,y), g(30,30);
	
	if(!mini)	gr->Title("Triangulation");
	gr->Rotate(40,60);	gr->Box();	gr->Light(true);
	gr->TriPlot(d,x,y,z);	gr->TriPlot(d,x,y,z,"#k");

	gr->DataGrid(g,x,y,z);	gr->Mesh(g,"m");
}
//-----------------------------------------------------------------------------
const char *mmgl_alpha="call 'prepare2d'\nsubplot 2 2 0:title 'default':rotate 50 60:box\nsurf a\n"
"subplot 2 2 1:title 'light on':rotate 50 60:box\nlight on:surf a\n"
"subplot 2 2 2:title 'light on; alpha on':rotate 50 60:box\nalpha on:surf a\n"
"subplot 2 2 3:title 'alpha on':rotate 50 60:box\nlight off:surf a\n";
void smgl_alpha(mglGraph *gr)	// alpha and lighting
{
	mglData a;	mgls_prepare2d(&a);
	gr->SubPlot(2,2,0);	gr->Title("default");	gr->Rotate(50,60);
	gr->Box();	gr->Surf(a);
	gr->SubPlot(2,2,1);	gr->Title("light on");	gr->Rotate(50,60);
	gr->Box();	gr->Light(true);	gr->Surf(a);
	gr->SubPlot(2,2,2);	gr->Title("alpha on; light on");	gr->Rotate(50,60);
	gr->Box();	gr->Alpha(true);	gr->Surf(a);
	gr->SubPlot(2,2,3);	gr->Title("alpha on");	gr->Rotate(50,60);
	gr->Box();	gr->Light(false);	gr->Surf(a);
}
//-----------------------------------------------------------------------------
const char *mmgl_schemes="call 'sch' 0 'kw'\ncall 'sch' 1 'wk'\ncall 'sch' 2 'kHCcw'\ncall 'sch' 3 'kBbcw'\n"
"call 'sch' 4 'kRryw'\ncall 'sch' 5 'kGgew'\ncall 'sch' 6 'BbwrR'\ncall 'sch' 7 'BbwgG'\n"
"call 'sch' 8 'GgwmM'\ncall 'sch' 9 'UuwqR'\ncall 'sch' 10 'QqwcC'\ncall 'sch' 11 'CcwyY'\n"
"call 'sch' 12 'bcwyr'\ncall 'sch' 13 'bwr'\ncall 'sch' 14 'BbcyrR'\ncall 'sch' 15 'UbcyqR'\n"
"call 'sch' 16 'BbcwyrR'\ncall 'sch' 17 'bgr'\ncall 'sch' 18 'BbcyrR|'\ncall 'sch' 19 'b\\{g,0.3\\}r'\n"
"stop\nfunc 'sch' 2\nsubplot 2 10 $1 '<>_^' 0.2 0:fsurf 'x' '$2'\n"
"text 0.07+0.5*mod($1,2) 0.92-0.1*int($1/2) '$2' 'A'\nreturn\n";
void smgl_schemes(mglGraph *gr)	// Color table
{
	mglData a(256,2);	a.Fill(-1,1);
	gr->SubPlot(2,10,0,NULL,0.2);	gr->Dens(a,"kw");		gr->Puts(0.07, 0.92, "'kw'", "A");
	gr->SubPlot(2,10,1,NULL,0.2);	gr->Dens(a,"wk");		gr->Puts(0.57, 0.92, "'wk'", "A");
	gr->SubPlot(2,10,2,NULL,0.2);	gr->Dens(a,"kHCcw");	gr->Puts(0.07, 0.82, "'kHCcw'", "A");
	gr->SubPlot(2,10,3,NULL,0.2);	gr->Dens(a,"kBbcw");	gr->Puts(0.57, 0.82, "'kBbcw'", "A");
	gr->SubPlot(2,10,4,NULL,0.2);	gr->Dens(a,"kRryw");	gr->Puts(0.07, 0.72, "'kRryw'", "A");
	gr->SubPlot(2,10,5,NULL,0.2);	gr->Dens(a,"kGgew");	gr->Puts(0.57, 0.72, "'kGgew'", "A");
	gr->SubPlot(2,10,6,NULL,0.2);	gr->Dens(a,"BbwrR");	gr->Puts(0.07, 0.62, "'BbwrR'", "A");
	gr->SubPlot(2,10,7,NULL,0.2);	gr->Dens(a,"BbwgG");	gr->Puts(0.57, 0.62, "'BbwgG'", "A");
	gr->SubPlot(2,10,8,NULL,0.2);	gr->Dens(a,"GgwmM");	gr->Puts(0.07, 0.52, "'GgwmM'", "A");
	gr->SubPlot(2,10,9,NULL,0.2);	gr->Dens(a,"UuwqR");	gr->Puts(0.57, 0.52, "'UuwqR'", "A");
	gr->SubPlot(2,10,10,NULL,0.2);	gr->Dens(a,"QqwcC");	gr->Puts(0.07, 0.42, "'QqwcC'", "A");
	gr->SubPlot(2,10,11,NULL,0.2);	gr->Dens(a,"CcwyY");	gr->Puts(0.57, 0.42, "'CcwyY'", "A");
	gr->SubPlot(2,10,12,NULL,0.2);	gr->Dens(a,"bcwyr");	gr->Puts(0.07, 0.32, "'bcwyr'", "A");
	gr->SubPlot(2,10,13,NULL,0.2);	gr->Dens(a,"bwr");		gr->Puts(0.57, 0.32, "'bwr'", "A");
	gr->SubPlot(2,10,14,NULL,0.2);	gr->Dens(a,"BbcyrR");	gr->Puts(0.07, 0.22, "'BbcyrR'", "A");
	gr->SubPlot(2,10,15,NULL,0.2);	gr->Dens(a,"UbcyqR");	gr->Puts(0.57, 0.22, "'UbcyqR'", "A");
	gr->SubPlot(2,10,16,NULL,0.2);	gr->Dens(a,"BbcwyrR");	gr->Puts(0.07, 0.12, "'BbcwyrR'", "A");
	gr->SubPlot(2,10,17,NULL,0.2);	gr->Dens(a,"bgr");		gr->Puts(0.57, 0.12, "'bgr'", "A");
	gr->SubPlot(2,10,18,NULL,0.2);	gr->Dens(a,"BbcyrR|");	gr->Puts(0.07, 0.02, "'BbcyrR|'", "A");
	gr->SubPlot(2,10,19,NULL,0.2);	gr->Dens(a,"b{g,0.3}r");		gr->Puts(0.57, 0.02, "'b\\{g,0.3\\}r'", "A");
}
//-----------------------------------------------------------------------------
const char *mmgl_curvcoor="origin -1 1 -1\nsubplot 2 2 0:title 'Cartesian':rotate 50 60:fplot '2*t-1' '0.5' '0' '2r':axis:grid\n"
"axis 'y*sin(pi*x)' 'y*cos(pi*x)' '':subplot 2 2 1:title 'Cylindrical':rotate 50 60:fplot '2*t-1' '0.5' '0' '2r':axis:grid\n"
"axis '2*y*x' 'y*y - x*x' '':subplot 2 2 2:title 'Parabolic':rotate 50 60:fplot '2*t-1' '0.5' '0' '2r':axis:grid\n"
"axis 'y*sin(pi*x)' 'y*cos(pi*x)' 'x+z':subplot 2 2 3:title 'Spiral':rotate 50 60:fplot '2*t-1' '0.5' '0' '2r':axis:grid\n";
void smgl_curvcoor(mglGraph *gr)	// curvilinear coordinates
{
	gr->SetOrigin(-1,1,-1);

	gr->SubPlot(2,2,0);	gr->Title("Cartesian");	gr->Rotate(50,60);
	gr->FPlot("2*t-1","0.5","0","r2");
	gr->Axis(); gr->Grid();

	gr->SetFunc("y*sin(pi*x)","y*cos(pi*x)",0);
	gr->SubPlot(2,2,1);	gr->Title("Cylindrical");	gr->Rotate(50,60);
	gr->FPlot("2*t-1","0.5","0","r2");
	gr->Axis(); gr->Grid();

	gr->SetFunc("2*y*x","y*y - x*x",0);
	gr->SubPlot(2,2,2);	gr->Title("Parabolic");	gr->Rotate(50,60);
	gr->FPlot("2*t-1","0.5","0","r2");
	gr->Axis(); gr->Grid();

	gr->SetFunc("y*sin(pi*x)","y*cos(pi*x)","x+z");
	gr->SubPlot(2,2,3);	gr->Title("Spiral");	gr->Rotate(50,60);
	gr->FPlot("2*t-1","0.5","0","r2");
	gr->Axis(); gr->Grid();
	gr->SetFunc(0,0,0);	// set to default Cartesian
}
//-----------------------------------------------------------------------------
const char *mmgl_style="";
void smgl_style(mglGraph *gr)	// pen styles
{
	gr->SubPlot(2,2,0);
	double d,x1,x2,x0,y=0.95;
	d=0.3, x0=0.2, x1=0.5, x2=0.6;
	gr->Line(mglPoint(x0,1-0*d),mglPoint(x1,1-0*d),"k-");	gr->Puts(mglPoint(x2,y-0*d),"Solid '-'",":rL");
	gr->Line(mglPoint(x0,1-1*d),mglPoint(x1,1-1*d),"k|");	gr->Puts(mglPoint(x2,y-1*d),"Long Dash '|'",":rL");
	gr->Line(mglPoint(x0,1-2*d),mglPoint(x1,1-2*d),"k;");	gr->Puts(mglPoint(x2,y-2*d),"Dash ';'",":rL");
	gr->Line(mglPoint(x0,1-3*d),mglPoint(x1,1-3*d),"k=");	gr->Puts(mglPoint(x2,y-3*d),"Small dash '='",":rL");
	gr->Line(mglPoint(x0,1-4*d),mglPoint(x1,1-4*d),"kj");	gr->Puts(mglPoint(x2,y-4*d),"Dash-dot 'j'",":rL");
	gr->Line(mglPoint(x0,1-5*d),mglPoint(x1,1-5*d),"ki");	gr->Puts(mglPoint(x2,y-5*d),"Small dash-dot 'i'",":rL");
	gr->Line(mglPoint(x0,1-6*d),mglPoint(x1,1-6*d),"k:");	gr->Puts(mglPoint(x2,y-6*d),"Dots ':'",":rL");
	gr->Line(mglPoint(x0,1-7*d),mglPoint(x1,1-7*d),"k ");	gr->Puts(mglPoint(x2,y-7*d),"None ' '",":rL");

	d=0.25; x1=-1; x0=-0.8;	y = -0.05;
	gr->Mark(mglPoint(x1,5*d),"k.");		gr->Puts(mglPoint(x0,y+5*d),"'.'",":rL");
	gr->Mark(mglPoint(x1,4*d),"k+");		gr->Puts(mglPoint(x0,y+4*d),"'+'",":rL");
	gr->Mark(mglPoint(x1,3*d),"kx");		gr->Puts(mglPoint(x0,y+3*d),"'x'",":rL");
	gr->Mark(mglPoint(x1,2*d),"k*");		gr->Puts(mglPoint(x0,y+2*d),"'*'",":rL");
	gr->Mark(mglPoint(x1,d),"ks");		gr->Puts(mglPoint(x0,y+d),"'s'",":rL");
	gr->Mark(mglPoint(x1,0),"kd");		gr->Puts(mglPoint(x0,y),"'d'",":rL");
	gr->Mark(mglPoint(x1,-d,0),"ko");	gr->Puts(mglPoint(x0,y-d),"'o'",":rL");
	gr->Mark(mglPoint(x1,-2*d,0),"k^");	gr->Puts(mglPoint(x0,y-2*d),"'\\^'",":rL");
	gr->Mark(mglPoint(x1,-3*d,0),"kv");	gr->Puts(mglPoint(x0,y-3*d),"'v'",":rL");
	gr->Mark(mglPoint(x1,-4*d,0),"k<");	gr->Puts(mglPoint(x0,y-4*d),"'<'",":rL");
	gr->Mark(mglPoint(x1,-5*d,0),"k>");	gr->Puts(mglPoint(x0,y-5*d),"'>'",":rL");

	d=0.25; x1=-0.5; x0=-0.3;	y = -0.05;
	gr->Mark(mglPoint(x1,5*d),"k#.");	gr->Puts(mglPoint(x0,y+5*d),"'\\#.'",":rL");
	gr->Mark(mglPoint(x1,4*d),"k#+");	gr->Puts(mglPoint(x0,y+4*d),"'\\#+'",":rL");
	gr->Mark(mglPoint(x1,3*d),"k#x");	gr->Puts(mglPoint(x0,y+3*d),"'\\#x'",":rL");
	gr->Mark(mglPoint(x1,2*d),"k#*");	gr->Puts(mglPoint(x0,y+2*d),"'\\#*'",":rL");
	gr->Mark(mglPoint(x1,d),"k#s");		gr->Puts(mglPoint(x0,y+d),"'\\#s'",":rL");
	gr->Mark(mglPoint(x1,0),"k#d");		gr->Puts(mglPoint(x0,y),"'\\#d'",":rL");
	gr->Mark(mglPoint(x1,-d,0),"k#o");	gr->Puts(mglPoint(x0,y-d),"'\\#o'",":rL");
	gr->Mark(mglPoint(x1,-2*d,0),"k#^");	gr->Puts(mglPoint(x0,y-2*d),"'\\#\\^'",":rL");
	gr->Mark(mglPoint(x1,-3*d,0),"k#v");	gr->Puts(mglPoint(x0,y-3*d),"'\\#v'",":rL");
	gr->Mark(mglPoint(x1,-4*d,0),"k#<");	gr->Puts(mglPoint(x0,y-4*d),"'\\#<'",":rL");
	gr->Mark(mglPoint(x1,-5*d,0),"k#>");	gr->Puts(mglPoint(x0,y-5*d),"'\\#>'",":rL");

	gr->SubPlot(2,2,1);
	double a=0.1,b=0.4,c=0.5;
	gr->Line(mglPoint(a,1),mglPoint(b,1),"k-A");		gr->Puts(mglPoint(c,1),"Style 'A' or 'A\\_'",":rL");
	gr->Line(mglPoint(a,0.8),mglPoint(b,0.8),"k-V");	gr->Puts(mglPoint(c,0.8),"Style 'V' or 'V\\_'",":rL");
	gr->Line(mglPoint(a,0.6),mglPoint(b,0.6),"k-K");	gr->Puts(mglPoint(c,0.6),"Style 'K' or 'K\\_'",":rL");
	gr->Line(mglPoint(a,0.4),mglPoint(b,0.4),"k-I");	gr->Puts(mglPoint(c,0.4),"Style 'I' or 'I\\_'",":rL");
	gr->Line(mglPoint(a,0.2),mglPoint(b,0.2),"k-D");	gr->Puts(mglPoint(c,0.2),"Style 'D' or 'D\\_'",":rL");
	gr->Line(mglPoint(a,0),mglPoint(b,0),"k-S");		gr->Puts(mglPoint(c,0),"Style 'S' or 'S\\_'",":rL");
	gr->Line(mglPoint(a,-0.2),mglPoint(b,-0.2),"k-O");	gr->Puts(mglPoint(c,-0.2),"Style 'O' or 'O\\_'",":rL");
	gr->Line(mglPoint(a,-0.4),mglPoint(b,-0.4),"k-T");	gr->Puts(mglPoint(c,-0.4),"Style 'T' or 'T\\_'",":rL");
	gr->Line(mglPoint(a,-0.6),mglPoint(b,-0.6),"k-_");	gr->Puts(mglPoint(c,-0.6),"Style '\\_' or none",":rL");
	gr->Line(mglPoint(a,-0.8),mglPoint(b,-0.8),"k-AS");	gr->Puts(mglPoint(c,-0.8),"Style 'AS'",":rL");
	gr->Line(mglPoint(a,-1),mglPoint(b,-1),"k-_A");		gr->Puts(mglPoint(c,-1),"Style '\\_A'",":rL");

	a=-1;	b=-0.7;	c=-0.6;
	gr->Line(mglPoint(a,1),mglPoint(b,1),"kAA");		gr->Puts(mglPoint(c,1),"Style 'AA'",":rL");
	gr->Line(mglPoint(a,0.8),mglPoint(b,0.8),"kVV");	gr->Puts(mglPoint(c,0.8),"Style 'VV'",":rL");
	gr->Line(mglPoint(a,0.6),mglPoint(b,0.6),"kKK");	gr->Puts(mglPoint(c,0.6),"Style 'KK'",":rL");
	gr->Line(mglPoint(a,0.4),mglPoint(b,0.4),"kII");	gr->Puts(mglPoint(c,0.4),"Style 'II'",":rL");
	gr->Line(mglPoint(a,0.2),mglPoint(b,0.2),"kDD");	gr->Puts(mglPoint(c,0.2),"Style 'DD'",":rL");
	gr->Line(mglPoint(a,0),mglPoint(b,0),"kSS");		gr->Puts(mglPoint(c,0),"Style 'SS'",":rL");
	gr->Line(mglPoint(a,-0.2),mglPoint(b,-0.2),"kOO");	gr->Puts(mglPoint(c,-0.2),"Style 'OO'",":rL");
	gr->Line(mglPoint(a,-0.4),mglPoint(b,-0.4),"kTT");	gr->Puts(mglPoint(c,-0.4),"Style 'TT'",":rL");
	gr->Line(mglPoint(a,-0.6),mglPoint(b,-0.6),"k-__");	gr->Puts(mglPoint(c,-0.6),"Style '\\_\\_'",":rL");
	gr->Line(mglPoint(a,-0.8),mglPoint(b,-0.8),"k-VA");	gr->Puts(mglPoint(c,-0.8),"Style 'VA'",":rL");
	gr->Line(mglPoint(a,-1),mglPoint(b,-1),"k-AV");		gr->Puts(mglPoint(c,-1),"Style 'AV'",":rL");

	gr->SubPlot(2,2,2);
	//#LENUQ
	gr->FaceZ(mglPoint(-1,	-1), 0.4, 0.3, "L#");	gr->Puts(mglPoint(-0.8,-0.9), "L", "w:C", -1.4);
	gr->FaceZ(mglPoint(-0.6,-1), 0.4, 0.3, "E#");	gr->Puts(mglPoint(-0.4,-0.9), "E", "w:C", -1.4);
	gr->FaceZ(mglPoint(-0.2,-1), 0.4, 0.3, "N#");	gr->Puts(mglPoint(0,  -0.9), "N", "w:C", -1.4);
	gr->FaceZ(mglPoint(0.2,	-1), 0.4, 0.3, "U#");	gr->Puts(mglPoint(0.4,-0.9), "U", "w:C", -1.4);
	gr->FaceZ(mglPoint(0.6,	-1), 0.4, 0.3, "Q#");	gr->Puts(mglPoint(0.8,-0.9), "Q", "w:C", -1.4);
	//#lenuq
	gr->FaceZ(mglPoint(-1,	-0.7), 0.4, 0.3, "l#");	gr->Puts(mglPoint(-0.8,-0.6), "l", "k:C", -1.4);
	gr->FaceZ(mglPoint(-0.6,-0.7), 0.4, 0.3, "e#");	gr->Puts(mglPoint(-0.4,-0.6), "e", "k:C", -1.4);
	gr->FaceZ(mglPoint(-0.2,-0.7), 0.4, 0.3, "n#");	gr->Puts(mglPoint(0,  -0.6), "n", "k:C", -1.4);
	gr->FaceZ(mglPoint(0.2,	-0.7), 0.4, 0.3, "u#");	gr->Puts(mglPoint(0.4,-0.6), "u", "k:C", -1.4);
	gr->FaceZ(mglPoint(0.6,	-0.7), 0.4, 0.3, "q#");	gr->Puts(mglPoint(0.8,-0.6), "q", "k:C", -1.4);
	//#CMYkP
	gr->FaceZ(mglPoint(-1,	-0.4), 0.4, 0.3, "C#");	gr->Puts(mglPoint(-0.8,-0.3), "C", "w:C", -1.4);
	gr->FaceZ(mglPoint(-0.6,-0.4), 0.4, 0.3, "M#");	gr->Puts(mglPoint(-0.4,-0.3), "M", "w:C", -1.4);
	gr->FaceZ(mglPoint(-0.2,-0.4), 0.4, 0.3, "Y#");	gr->Puts(mglPoint(0,  -0.3), "Y", "w:C", -1.4);
	gr->FaceZ(mglPoint(0.2,	-0.4), 0.4, 0.3, "k#");	gr->Puts(mglPoint(0.4,-0.3), "k", "w:C", -1.4);
	gr->FaceZ(mglPoint(0.6,	-0.4), 0.4, 0.3, "P#");	gr->Puts(mglPoint(0.8,-0.3), "P", "w:C", -1.4);
	//#cmywp
	gr->FaceZ(mglPoint(-1,	-0.1), 0.4, 0.3, "c#");	gr->Puts(mglPoint(-0.8, 0), "c", "k:C", -1.4);
	gr->FaceZ(mglPoint(-0.6,-0.1), 0.4, 0.3, "m#");	gr->Puts(mglPoint(-0.4, 0), "m", "k:C", -1.4);
	gr->FaceZ(mglPoint(-0.2,-0.1), 0.4, 0.3, "y#");	gr->Puts(mglPoint(0,   0), "y", "k:C", -1.4);
	gr->FaceZ(mglPoint(0.2,	-0.1), 0.4, 0.3, "w#");	gr->Puts(mglPoint(0.4, 0), "w", "k:C", -1.4);
	gr->FaceZ(mglPoint(0.6,	-0.1), 0.4, 0.3, "p#");	gr->Puts(mglPoint(0.8, 0), "p", "k:C", -1.4);
	//#BGRHW
	gr->FaceZ(mglPoint(-1,	0.2), 0.4, 0.3, "B#");	gr->Puts(mglPoint(-0.8, 0.3), "B", "w:C", -1.4);
	gr->FaceZ(mglPoint(-0.6,0.2), 0.4, 0.3, "G#");	gr->Puts(mglPoint(-0.4, 0.3), "G", "w:C", -1.4);
	gr->FaceZ(mglPoint(-0.2,0.2), 0.4, 0.3, "R#");	gr->Puts(mglPoint(0,   0.3), "R", "w:C", -1.4);
	gr->FaceZ(mglPoint(0.2,	0.2), 0.4, 0.3, "H#");	gr->Puts(mglPoint(0.4, 0.3), "H", "w:C", -1.4);
	gr->FaceZ(mglPoint(0.6,	0.2), 0.4, 0.3, "W#");	gr->Puts(mglPoint(0.8, 0.3), "W", "w:C", -1.4);
	//#bgrhw
	gr->FaceZ(mglPoint(-1,	0.5), 0.4, 0.3, "b#");	gr->Puts(mglPoint(-0.8, 0.6), "b", "k:C", -1.4);
	gr->FaceZ(mglPoint(-0.6,0.5), 0.4, 0.3, "g#");	gr->Puts(mglPoint(-0.4, 0.6), "g", "k:C", -1.4);
	gr->FaceZ(mglPoint(-0.2,0.5), 0.4, 0.3, "r#");	gr->Puts(mglPoint(0,   0.6), "r", "k:C", -1.4);
	gr->FaceZ(mglPoint(0.2,	0.5), 0.4, 0.3, "h#");	gr->Puts(mglPoint(0.4, 0.6), "h", "k:C", -1.4);
	gr->FaceZ(mglPoint(0.6,	0.5), 0.4, 0.3, "w#");	gr->Puts(mglPoint(0.8, 0.6), "w", "k:C", -1.4);
	//#brighted
	gr->FaceZ(mglPoint(-1,	0.8), 0.4, 0.3, "{r1}#");	gr->Puts(mglPoint(-0.8, 0.9), "\\{r1\\}", "w:C", -1.4);
	gr->FaceZ(mglPoint(-0.6,0.8), 0.4, 0.3, "{r3}#");	gr->Puts(mglPoint(-0.4, 0.9), "\\{r3\\}", "w:C", -1.4);
	gr->FaceZ(mglPoint(-0.2,0.8), 0.4, 0.3, "{r5}#");	gr->Puts(mglPoint(0,   0.9), "\\{r5\\}", "k:C", -1.4);
	gr->FaceZ(mglPoint(0.2,	0.8), 0.4, 0.3, "{r7}#");	gr->Puts(mglPoint(0.4, 0.9), "\\{r7\\}", "k:C", -1.4);
	gr->FaceZ(mglPoint(0.6,	0.8), 0.4, 0.3, "{r9}#");	gr->Puts(mglPoint(0.8, 0.9), "\\{r9\\}", "k:C", -1.4);
	// HEX
	gr->FaceZ(mglPoint(-1, -1.3), 1, 0.3, "{xff9966}#");	gr->Puts(mglPoint(-0.5,-1.2), "\\{xff9966\\}", "k:C", -1.4);
	gr->FaceZ(mglPoint(0,  -1.3), 1, 0.3, "{x83CAFF}#");	gr->Puts(mglPoint( 0.5,-1.2), "\\{x83CAFF\\}", "k:C", -1.4);

	gr->SubPlot(2,2,3);
	char stl[3]="r1", txt[4]="'1'";
	for(int i=0;i<10;i++)
	{
		txt[1]=stl[1]='0'+i;
		gr->Line(mglPoint(-1,0.2*i-1),mglPoint(1,0.2*i-1),stl);
		gr->Puts(mglPoint(1.05,0.2*i-1),txt,":L");
	}
}
//-----------------------------------------------------------------------------
const char *mmgl_text="call 'prepare1d'\nsubplot 2 2 0 ''\ntext 0 1 'Text can be in ASCII and in Unicode'\n"
"text 0 0.6 'It can be \\wire{wire}, \\big{big} or #r{colored}'\n"
"text 0 0.2 'One can change style in string: \\b{bold}, \\i{italic, \\b{both}}'\n"
"text 0 -0.2 'Easy to \\a{overline} or \\u{underline}'\n"
"text 0 -0.6 'Easy to change indexes ^{up} _{down} @{center}'\n"
"text 0 -1 'It parse TeX: \\int \\alpha \\cdot \\\n\\sqrt3{sin(\\pi x)^2 + \\gamma_{i_k}} dx'\n"
"subplot 2 2 1 ''\n text 0 0.5 '\\sqrt{\\frac{\\alpha^{\\gamma^2}+\\overset 1{\\big\\infty}}{\\sqrt3{2+b}}}' '@' -2\n"
"text 0 -0.5 'Text can be printed\\n{}on several lines'\n"
"subplot 2 2 2 '':box:plot y(:,0)\ntext y 'This is very very long string drawn along a curve' 'k'\ntext y 'Another string drawn above a curve' 'Tr'\n"
"subplot 2 2 3 '':line -1 -1 1 -1 'rA':text 0 -1 1 -1 'Horizontal'\n"
"line -1 -1 1 1 'rA':text 0 0 1 1 'At angle' '@'\nline -1 -1 -1 1 'rA':text -1 0 -1 1 'Vertical'\n";
void smgl_text(mglGraph *gr)	// text drawing
{
	if(!mini)	gr->SubPlot(2,2,0,"");
	gr->Putsw(mglPoint(0,1),L"Text can be in ASCII and in Unicode");
	gr->Puts(mglPoint(0,0.6),"It can be \\wire{wire}, \\big{big} or #r{colored}");
	gr->Puts(mglPoint(0,0.2),"One can change style in string: "
	"\\b{bold}, \\i{italic, \\b{both}}");
	gr->Puts(mglPoint(0,-0.2),"Easy to \\a{overline} or "
	"\\u{underline}");
	gr->Puts(mglPoint(0,-0.6),"Easy to change indexes ^{up} _{down} @{center}");
	gr->Puts(mglPoint(0,-1),"It parse TeX: \\int \\alpha \\cdot "
	"\\sqrt3{sin(\\pi x)^2 + \\gamma_{i_k}} dx");
	if(mini)	return;

	gr->SubPlot(2,2,1,"");
	gr->Puts(mglPoint(0,0.5), "\\sqrt{\\frac{\\alpha^{\\gamma^2}+\\overset 1{\\big\\infty}}{\\sqrt3{2+b}}}", "@", -2);
	gr->Puts(mglPoint(0,-0.5),"Text can be printed\non several lines");

	gr->SubPlot(2,2,2,"");
	mglData y;	mgls_prepare1d(&y);
	gr->Box();	gr->Plot(y.SubData(-1,0));
	gr->Text(y,"This is very very long string drawn along a curve","k");
	gr->Text(y,"Another string drawn above a curve","Tr");

	gr->SubPlot(2,2,3,"");
	gr->Line(mglPoint(-1,-1),mglPoint(1,-1),"rA");	gr->Puts(mglPoint(0,-1),mglPoint(1,-1),"Horizontal");
	gr->Line(mglPoint(-1,-1),mglPoint(1,1),"rA");	gr->Puts(mglPoint(0,0),mglPoint(1,1),"At angle","@");
	gr->Line(mglPoint(-1,-1),mglPoint(-1,1),"rA");	gr->Puts(mglPoint(-1,0),mglPoint(-1,1),"Vertical");
}
//-----------------------------------------------------------------------------
const char *mmgl_fonts="define d 0.25\nloadfont 'STIX':text 0 1.1 'default font (STIX)'\nloadfont 'adventor':text 0 1.1-d 'adventor font'\n"
"loadfont 'bonum':text 0 1.1-2*d 'bonum font'\nloadfont 'chorus':text 0 1.1-3*d 'chorus font'\nloadfont 'cursor':text 0 1.1-4*d 'cursor font'\n"
"loadfont 'heros':text 0 1.1-5*d 'heros font'\nloadfont 'heroscn':text 0 1.1-6*d 'heroscn font'\nloadfont 'pagella':text 0 1.1-7*d 'pagella font'\n"
"loadfont 'schola':text 0 1.1-8*d 'schola font'\nloadfont 'termes':text 0 1.1-9*d 'termes font'\nloadfont ''\n";
void smgl_fonts(mglGraph *gr)	// font typefaces
{
	double h=1.1, d=0.25;
	gr->LoadFont("STIX");		gr->Puts(mglPoint(0,h), "default font (STIX)");
	gr->LoadFont("adventor");	gr->Puts(mglPoint(0,h-d), "adventor font");
	gr->LoadFont("bonum");		gr->Puts(mglPoint(0,h-2*d), "bonum font");
	gr->LoadFont("chorus");		gr->Puts(mglPoint(0,h-3*d), "chorus font");
	gr->LoadFont("cursor");		gr->Puts(mglPoint(0,h-4*d), "cursor font");
	gr->LoadFont("heros");		gr->Puts(mglPoint(0,h-5*d), "heros font");
	gr->LoadFont("heroscn");	gr->Puts(mglPoint(0,h-6*d), "heroscn font");
	gr->LoadFont("pagella");	gr->Puts(mglPoint(0,h-7*d), "pagella font");
	gr->LoadFont("schola");		gr->Puts(mglPoint(0,h-8*d), "schola font");
	gr->LoadFont("termes");		gr->Puts(mglPoint(0,h-9*d), "termes font");
	gr->LoadFont("");
}
//-----------------------------------------------------------------------------
const char *mmgl_bars="new ys 10 3 '0.8*sin(pi*(x+y/4+1.25))+0.2*rnd':origin 0 0 0\n"
"subplot 3 2 0 '':title 'Bars plot (default)':box:bars ys\nsubplot 3 2 1 '':title '2 colors':box:bars ys 'cbgGyr'\n"
"subplot 3 2 4 '':title '\"\\#\" style':box:bars ys '#'\n"
"new yc 30 'sin(pi*x)':new xc 30 'cos(pi*x)':new z 30 'x'\nsubplot 3 2 5:title '3d variant':rotate 50 60:box:bars xc yc z 'r'\n"
"ranges -1 1 -3 3:subplot 3 2 2 '':title '\"a\" style':box:bars ys 'a'\nsubplot 3 2 3 '':title '\"f\" style':box:bars ys 'f'\n";
void smgl_bars(mglGraph *gr)
{
	mglData ys(10,3);	ys.Modify("0.8*sin(pi*(2*x+y/2))+0.2*rnd");
	gr->SetOrigin(0,0,0);
	if(!mini)	{	gr->SubPlot(3,2,0,"");	gr->Title("Bars plot (default)");	}
	gr->Box();	gr->Bars(ys);
	if(mini)	return;
	gr->SubPlot(3,2,1,"");	gr->Title("2 colors");	gr->Box();	gr->Bars(ys,"cbgGyr");
	gr->SubPlot(3,2,4,"");	gr->Title("'\\#' style");	gr->Box();	gr->Bars(ys,"#");
	gr->SubPlot(3,2,5);	gr->Title("3d variant");	gr->Rotate(50,60);	gr->Box();
	mglData yc(30), xc(30), z(30);	z.Modify("2*x-1");
	yc.Modify("sin(pi*(2*x-1))");	xc.Modify("cos(pi*2*x-pi)");
	gr->Bars(xc,yc,z,"r");
	gr->SetRanges(-1,1,-3,3);	// increase range since summation can exceed [-1,1]
	gr->SubPlot(3,2,2,"");	gr->Title("'a' style");	gr->Box();	gr->Bars(ys,"a");
	gr->SubPlot(3,2,3,"");	gr->Title("'f' style");	gr->Box();	gr->Bars(ys,"f");
}
//-----------------------------------------------------------------------------
const char *mmgl_barh="new ys 10 3 '0.8*sin(pi*(x+y/4+1.25))+0.2*rnd':origin 0 0 0\n"
"subplot 2 2 0 '':title 'Barh plot (default)':box:barh ys\nsubplot 2 2 1 '':title '2 colors':box:barh ys 'cbgGyr'\n"
"ranges -3 3 -1 1:subplot 2 2 2 '':title '\"a\" style':box:barh ys 'a'\nsubplot 2 2 3 '': title '\"f\" style':box:barh ys 'f'\n";
void smgl_barh(mglGraph *gr)
{
	mglData ys(10,3);	ys.Modify("0.8*sin(pi*(2*x+y/2))+0.2*rnd");
	gr->SetOrigin(0,0,0);
	if(!mini)	{	gr->SubPlot(2,2,0,"");	gr->Title("Barh plot (default)");	}
	gr->Box();	gr->Barh(ys);
	if(mini)	return;
	gr->SubPlot(2,2,1,"");	gr->Title("2 colors");	gr->Box();	gr->Barh(ys,"cbgGyr");
	gr->SetRanges(-3,3,-1,1);	// increase range since summation can exceed [-1,1]
	gr->SubPlot(2,2,2,"");	gr->Title("'a' style");	gr->Box();	gr->Barh(ys,"a");
	gr->SubPlot(2,2,3,"");	gr->Title("'f' style");	gr->Box();	gr->Barh(ys,"f");
}
//-----------------------------------------------------------------------------
const char *mmgl_area="call 'prepare1d'\norigin 0 0 0\nsubplot 2 2 0 '':title 'Area plot (default)':box:area y\n"
"subplot 2 2 1 '':title '2 colors':box:area y 'cbgGyr'\nsubplot 2 2 2 '':title '\"!\" style':box:area y '!'\n"
"new yc 30 'sin(pi*x)':new xc 30 'cos(pi*x)':new z 30 'x'\nsubplot 2 2 3:title '3d variant':rotate 50 60:box:area xc yc z 'r'\n";
void smgl_area(mglGraph *gr)
{
	mglData y;	mgls_prepare1d(&y);	gr->SetOrigin(0,0,0);
	if(!mini)	{	gr->SubPlot(2,2,0,"");	gr->Title("Area plot (default)");	}
	gr->Box();	gr->Area(y);
	if(mini)	return;
	gr->SubPlot(2,2,1,"");	gr->Title("2 colors");	gr->Box();	gr->Area(y,"cbgGyr");
	gr->SubPlot(2,2,2,"");	gr->Title("'!' style");	gr->Box();	gr->Area(y,"!");
	gr->SubPlot(2,2,3);	gr->Title("3d variant");	gr->Rotate(50,60);	gr->Box();
	mglData yc(30), xc(30), z(30);	z.Modify("2*x-1");
	yc.Modify("sin(pi*(2*x-1))");	xc.Modify("cos(pi*2*x-pi)");
	gr->Area(xc,yc,z,"r");
}
//-----------------------------------------------------------------------------
const char *mmgl_plot="call 'prepare1d'\nsubplot 2 2 0 '':title 'Plot plot (default)':box:plot y\n"
"subplot 2 2 2 '':title ''!' style; 'rgb' palette':box:plot y 'o!rgb'\nsubplot 2 2 3 '':title 'just markers':box:plot y ' +'\n"
"new yc 30 'sin(pi*x)':new xc 30 'cos(pi*x)':new z 30 'x'\nsubplot 2 2 1:title '3d variant':rotate 50 60:box:plot xc yc z 'rs'\n";
void smgl_plot(mglGraph *gr)
{
	mglData y;	mgls_prepare1d(&y);	gr->SetOrigin(0,0,0);
	if(!mini)	{	gr->SubPlot(2,2,0,"");	gr->Title("Plot plot (default)");	}
	gr->Box();	gr->Plot(y);
	if(mini)	return;
	gr->SubPlot(2,2,2,"");	gr->Title("'!' style; 'rgb' palette");	gr->Box();	gr->Plot(y,"o!rgb");
	gr->SubPlot(2,2,3,"");	gr->Title("just markers");	gr->Box();	gr->Plot(y," +");
	gr->SubPlot(2,2,1);	gr->Title("3d variant");	gr->Rotate(50,60);	gr->Box();
	mglData yc(30), xc(30), z(30);	z.Modify("2*x-1");
	yc.Modify("sin(pi*(2*x-1))");	xc.Modify("cos(pi*2*x-pi)");
	gr->Plot(xc,yc,z,"rs");
}
//-----------------------------------------------------------------------------
const char *mmgl_tens="call 'prepare1d'\nsubplot 2 2 0 '':title 'Tens plot (default)':box:tens y(:,0) y(:,1)\n"
"subplot 2 2 2 '':title '" " style':box:tens y(:,0) y(:,1) 'o '\n"
"new yc 30 'sin(pi*x)':new xc 30 'cos(pi*x)':new z 30 'x'\n"
"subplot 2 2 1:title '3d variant':rotate 50 60:box:tens xc yc z z 's'\n";
void smgl_tens(mglGraph *gr)
{
	mglData y;	mgls_prepare1d(&y);	gr->SetOrigin(0,0,0);
	if(!mini)	{	gr->SubPlot(2,2,0,"");	gr->Title("Tens plot (default)");	}
	gr->Box();	gr->Tens(y.SubData(-1,0), y.SubData(-1,1));
	if(mini)	return;
	gr->SubPlot(2,2,2,"");	gr->Title("' ' style");	gr->Box();	gr->Tens(y.SubData(-1,0), y.SubData(-1,1),"o ");
	gr->SubPlot(2,2,1);	gr->Title("3d variant");	gr->Rotate(50,60);	gr->Box();
	mglData yc(30), xc(30), z(30);	z.Modify("2*x-1");
	yc.Modify("sin(pi*(2*x-1))");	xc.Modify("cos(pi*2*x-pi)");
	gr->Tens(xc,yc,z,z,"s");
}
//-----------------------------------------------------------------------------
const char *mmgl_region="call 'prepare1d'\ncopy y1 y(:,1):copy y2 y(:,2)\n"
"subplot 2 2 0 '':title 'Region plot (default)':box:region y1 y2:plot y1 'k2':plot y2 'k2'\n"
"subplot 2 2 1 '':title '2 colors':box:region y1 y2 'yr':plot y1 'k2':plot y2 'k2'\n"
"subplot 2 2 2 '':title '\"!\" style':box:region y1 y2 '!':plot y1 'k2':plot y2 'k2'\n"
"subplot 2 2 3 '':title '\"i\" style':box:region y1 y2 'ir':plot y1 'k2':plot y2 'k2'\n";
void smgl_region(mglGraph *gr)
{
	mglData y;	mgls_prepare1d(&y);
	mglData y1 = y.SubData(-1,1), y2 = y.SubData(-1,2);	gr->SetOrigin(0,0,0);
	if(!mini)	{	gr->SubPlot(2,2,0,"");	gr->Title("Region plot (default)");	}
	gr->Box();	gr->Region(y1,y2);	gr->Plot(y1,"k2");	gr->Plot(y2,"k2");
	if(mini)	return;
	gr->SubPlot(2,2,1,"");	gr->Title("2 colors");	gr->Box();	gr->Region(y1,y2,"yr");	gr->Plot(y1,"k2");	gr->Plot(y2,"k2");
	gr->SubPlot(2,2,2,"");	gr->Title("'!' style");	gr->Box();	gr->Region(y1,y2,"!");	gr->Plot(y1,"k2");	gr->Plot(y2,"k2");
	gr->SubPlot(2,2,3,"");	gr->Title("'i' style");	gr->Box();	gr->Region(y1,y2,"ir");	gr->Plot(y1,"k2");	gr->Plot(y2,"k2");
}
//-----------------------------------------------------------------------------
const char *mmgl_stem="call 'prepare1d'\norigin 0 0 0:subplot 2 2 0 '':title 'Stem plot (default)':box:stem y\n"
"new yc 30 'sin(pi*x)':new xc 30 'cos(pi*x)':new z 30 'x'\nsubplot 2 2 1:title '3d variant':rotate 50 60:box:stem xc yc z 'rx'\n"
"subplot 2 2 2 '':title '\"!\" style':box:stem y 'o!rgb'\n";
void smgl_stem(mglGraph *gr)
{
	mglData y;	mgls_prepare1d(&y);	gr->SetOrigin(0,0,0);
	mglData yc(30), xc(30), z(30);	z.Modify("2*x-1");
	yc.Modify("sin(pi*(2*x-1))");	xc.Modify("cos(pi*2*x-pi)");
	if(!mini)	{	gr->SubPlot(2,2,0,"");	gr->Title("Stem plot (default)");	}
	gr->Box();	gr->Stem(y);
	if(mini)	return;
	gr->SubPlot(2,2,1);	gr->Title("3d variant");	gr->Rotate(50,60);
	gr->Box();	gr->Stem(xc,yc,z,"rx");
	gr->SubPlot(2,2,2,"");	gr->Title("'!' style");	gr->Box();	gr->Stem(y,"o!rgb");
}
//-----------------------------------------------------------------------------
const char *mmgl_step="call 'prepare1d'\norigin 0 0 0:subplot 2 2 0 '':title 'Step plot (default)':box:step y\n"
"new yc 30 'sin(pi*x)':new xc 30 'cos(pi*x)':new z 30 'x'\nsubplot 2 2 1:title '3d variant':rotate 50 60:box:step xc yc z 'r'\n"
"subplot 2 2 2 '':title '\"!\" style':box:step y 's!rgb'\n";
void smgl_step(mglGraph *gr)
{
	mglData y;	mgls_prepare1d(&y);	gr->SetOrigin(0,0,0);
	mglData yc(30), xc(30), z(30);	z.Modify("2*x-1");
	yc.Modify("sin(pi*(2*x-1))");	xc.Modify("cos(pi*2*x-pi)");
	if(!mini)	{	gr->SubPlot(2,2,0,"");	gr->Title("Step plot (default)");	}
	gr->Box();	gr->Step(y);
	if(mini)	return;
	gr->SubPlot(2,2,1);	gr->Title("3d variant");	gr->Rotate(50,60);
	gr->Box();	gr->Step(xc,yc,z,"r");
	gr->SubPlot(2,2,2,"");	gr->Title("'!' style");	gr->Box();	gr->Step(y,"s!rgb");
}
//-----------------------------------------------------------------------------
const char *mmgl_boxplot="new a 10 7 '(2*rnd-1)^3/2'\nsubplot 1 1 0 '':title 'Boxplot plot':box:boxplot a";
void smgl_boxplot(mglGraph *gr)	// flow threads and density plot
{
	mglData a(10,7);	a.Modify("(2*rnd-1)^3/2");
	if(!mini)	{	gr->SubPlot(1,1,0,"");	gr->Title("Boxplot plot");	}
	gr->Box();	gr->BoxPlot(a);
}
//-----------------------------------------------------------------------------
const char *mmgl_type0="call 'prepare2d'\nalpha on:light on:transptype 0:clf\nsubplot 2 2 0:rotate 50 60:surf a:box\n"
"subplot 2 2 1:rotate 50 60:dens a:box\nsubplot 2 2 2:rotate 50 60:cont a:box\n"
"subplot 2 2 3:rotate 50 60:axial a:box";
void smgl_type0(mglGraph *gr)	// TranspType = 0
{
	gr->Alpha(true);	gr->Light(true);
	mglData a;	mgls_prepare2d(&a);
	gr->SetTranspType(0);	gr->Clf();
	gr->SubPlot(2,2,0);	gr->Rotate(50,60);	gr->Surf(a);	gr->Box();
	gr->SubPlot(2,2,1);	gr->Rotate(50,60);	gr->Dens(a);	gr->Box();
	gr->SubPlot(2,2,2);	gr->Rotate(50,60);	gr->Cont(a);	gr->Box();
	gr->SubPlot(2,2,3);	gr->Rotate(50,60);	gr->Axial(a);	gr->Box();
}
//-----------------------------------------------------------------------------
const char *mmgl_type1="call 'prepare2d'\nalpha on:light on:transptype 1:clf\nsubplot 2 2 0:rotate 50 60:surf a:box\n"
"subplot 2 2 1:rotate 50 60:dens a:box\nsubplot 2 2 2:rotate 50 60:cont a:box\n"
"subplot 2 2 3:rotate 50 60:axial a:box";
void smgl_type1(mglGraph *gr)	// TranspType = 1
{
	gr->Alpha(true);	gr->Light(true);
	mglData a;	mgls_prepare2d(&a);
	gr->SetTranspType(1);	gr->Clf();
	gr->SubPlot(2,2,0);	gr->Rotate(50,60);	gr->Surf(a);	gr->Box();
	gr->SubPlot(2,2,1);	gr->Rotate(50,60);	gr->Dens(a);	gr->Box();
	gr->SubPlot(2,2,2);	gr->Rotate(50,60);	gr->Cont(a);	gr->Box();
	gr->SubPlot(2,2,3);	gr->Rotate(50,60);	gr->Axial(a);	gr->Box();
}
//-----------------------------------------------------------------------------
const char *mmgl_type2="call 'prepare2d'\nalpha on:light on:transptype 2:clf\nsubplot 2 2 0:rotate 50 60:surf a:box\n"
"subplot 2 2 1:rotate 50 60:dens a:box\nsubplot 2 2 2:rotate 50 60:cont a:box\n"
"subplot 2 2 3:rotate 50 60:axial a:box";
void smgl_type2(mglGraph *gr)	// TranspType = 2
{
	gr->Alpha(true);	gr->Light(true);
	mglData a;	mgls_prepare2d(&a);
	gr->SetTranspType(2);	gr->Clf();
	gr->SubPlot(2,2,0);	gr->Rotate(50,60);	gr->Surf(a);	gr->Box();
	gr->SubPlot(2,2,1);	gr->Rotate(50,60);	gr->Dens(a);	gr->Box();
	gr->SubPlot(2,2,2);	gr->Rotate(50,60);	gr->Cont(a);	gr->Box();
	gr->SubPlot(2,2,3);	gr->Rotate(50,60);	gr->Axial(a);	gr->Box();
}
//-----------------------------------------------------------------------------
const char *mmgl_molecule="alpha on:light on\n"
"subplot 2 2 0 '':title 'Methane, CH_4':rotate 60 120\n"
"sphere 0 0 0 0.25 'k':drop 0 0 0 0 0 1 0.35 'h' 1 2:sphere 0 0 0.7 0.25 'g'\n"
"drop 0 0 0 -0.94 0 -0.33 0.35 'h' 1 2:sphere -0.66 0 -0.23 0.25 'g'\n"
"drop 0 0 0 0.47 0.82 -0.33 0.35 'h' 1 2:sphere 0.33 0.57 -0.23 0.25 'g'\n"
"drop 0 0 0 0.47 -0.82 -0.33 0.35 'h' 1 2:sphere 0.33 -0.57 -0.23 0.25 'g'\n"
"subplot 2 2 1 '':title 'Water, H{_2}O':rotate 60 100\n"
"sphere 0 0 0 0.25 'r':drop 0 0 0 0.3 0.5 0 0.3 'm' 1 2:sphere 0.3 0.5 0 0.25 'g'\n"
"drop 0 0 0 0.3 -0.5 0 0.3 'm' 1 2:sphere 0.3 -0.5 0 0.25 'g'\n"
"subplot 2 2 2 '':title 'Oxygen, O_2':rotate 60 120\n"
"drop 0 0.5 0 0 -0.3 0 0.3 'm' 1 2:sphere 0 0.5 0 0.25 'r'\n"
"drop 0 -0.5 0 0 0.3 0 0.3 'm' 1 2:sphere 0 -0.5 0 0.25 'r'\n"
"subplot 2 2 3 '':title 'Ammonia, NH_3':rotate 60 120\n"
"sphere 0 0 0 0.25 'b':drop 0 0 0 0.33 0.57 0 0.32 'n' 1 2\n"
"sphere 0.33 0.57 0 0.25 'g':drop 0 0 0 0.33 -0.57 0 0.32 'n' 1 2\n"
"sphere 0.33 -0.57 0 0.25 'g':drop 0 0 0 -0.65 0 0 0.32 'n' 1 2\n"
"sphere -0.65 0 0 0.25 'g'\n";
void smgl_molecule(mglGraph *gr)	// example of moleculas
{
	gr->VertexColor(false);	gr->Compression(false); // per-vertex colors and compression are detrimental to transparency
	gr->DoubleSided(false); // we do not get into atoms, while rendering internal surface has negative impact on trasparency
	gr->Alpha(true);	gr->Light(true);

	gr->SubPlot(2,2,0,"");	gr->Title("Methane, CH_4");
	gr->StartGroup("Methane");
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

	gr->SubPlot(2,2,1,"");	gr->Title("Water, H_{2}O");
	gr->StartGroup("Water");
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

	gr->SubPlot(2,2,2,"");	gr->Title("Oxygen, O_2");
	gr->StartGroup("Oxygen");
	gr->Rotate(60,120);
	gr->Drop(mglPoint(0,0.5,0),mglPoint(0,-0.3,0),0.3,"m",1,2);
	gr->Sphere(mglPoint(0,0.5,0),0.25,"r");
	gr->Drop(mglPoint(0,-0.5,0),mglPoint(0,0.3,0),0.3,"m",1,2);
	gr->Sphere(mglPoint(0,-0.5,0),0.25,"r");
	gr->EndGroup();

	gr->SubPlot(2,2,3,"");	gr->Title("Ammonia, NH_3");
	gr->StartGroup("Ammonia");
	gr->Rotate(60,120);
	gr->Sphere(mglPoint(0,0,0),0.25,"b");
	gr->Drop(mglPoint(0,0,0),mglPoint(0.33,0.57,0),0.32,"n",1,2);
	gr->Sphere(mglPoint(0.33,0.57,0),0.25,"g");
	gr->Drop(mglPoint(0,0,0),mglPoint(0.33,-0.57,0),0.32,"n",1,2);
	gr->Sphere(mglPoint(0.33,-0.57,0),0.25,"g");
	gr->Drop(mglPoint(0,0,0),mglPoint(-0.65,0,0),0.32,"n",1,2);
	gr->Sphere(mglPoint(-0.65,0,0),0.25,"g");
	gr->EndGroup();
	gr->DoubleSided( true ); // put back
}
//-----------------------------------------------------------------------------
const char *mmgl_error="call 'prepare1d'\nnew y 50 '0.7*sin(pi*x-pi) + 0.5*cos(3*pi*(x+1)/2) + 0.2*sin(pi*(x+1)/2)'\n"
"new x0 10 'x + 0.1*rnd-0.05':new ex 10 '0.1':new ey 10 '0.2'\n"
"new y0 10 '0.7*sin(pi*x-pi) + 0.5*cos(3*pi*(x+1)/2) + 0.2*sin(pi*(x+1)/2) + 0.2*rnd-0.1'\n"
"subplot 2 2 0 '':title 'Error plot (default)':box:plot y:error x0 y0 ex ey 'k'\n"
"subplot 2 2 1 '':title '\"!\" style; no e_x':box:plot y:error x0 y0 ey 'o!rgb'\n"
"subplot 2 2 2 '':title '\"\\@\" style':alpha on:box:plot y:error x0 y0 ex ey '@'; alpha 0.5\n"
"subplot 2 2 3:title '3d variant':rotate 50 60:axis\n"
"for $1 0 9\n\terrbox 2*rnd-1 2*rnd-1 2*rnd-1 0.2 0.2 0.2 'bo'\nnext\n";
void smgl_error(mglGraph *gr)
{
	mglData y;	mgls_prepare1d(&y);
	mglData x0(10), y0(10), ex0(10), ey0(10);
	double x;
	for(int i=0;i<10;i++)
	{
		x = i/9.;
		x0.a[i] = 2*x-1 + 0.1*mgl_rnd()-0.05;
		y0.a[i] = 0.7*sin(2*M_PI*x)+0.5*cos(3*M_PI*x)+0.2*sin(M_PI*x)+0.2*mgl_rnd()-0.1;
		ey0.a[i]=0.2;	ex0.a[i]=0.1;
	}
	if(!mini)	{	gr->SubPlot(2,2,0,"");	gr->Title("Error plot (default)");	}
	gr->Box();	gr->Plot(y.SubData(-1,0));	gr->Error(x0,y0,ex0,ey0,"ko");
	if(mini)	return;
	gr->SubPlot(2,2,1,"");	gr->Title("'!' style; no e_x");
	gr->Box();	gr->Plot(y.SubData(-1,0));	gr->Error(x0,y0,ey0,"o!rgb");
	gr->SubPlot(2,2,2,"");	gr->Title("'\\@' style");	gr->Alpha(true);
	gr->Box();	gr->Plot(y.SubData(-1,0));	gr->Error(x0,y0,ex0,ey0,"@","alpha 0.5");
	gr->SubPlot(2,2,3);	gr->Title("3d variant");	gr->Rotate(50,60);
	for(int i=0;i<10;i++)
		gr->Error(mglPoint(2*mgl_rnd()-1,2*mgl_rnd()-1,2*mgl_rnd()-1), mglPoint(0.2,0.2,0.2),"bo");
	gr->Axis();
}
//-----------------------------------------------------------------------------
const char *mmgl_chart="new ch 7 2 'rnd+0.1':light on\n"
"subplot 2 2 0:title 'Chart plot (default)':rotate 50 60:box:chart ch\n"
"subplot 2 2 1:title '\"\\#\" style':rotate 50 60:box:chart ch '#'\n"
"subplot 2 2 2:title 'Pie chart; \" \" color':rotate 50 60:\n"
"axis '(y+1)/2*cos(pi*x)' '(y+1)/2*sin(pi*x)' '':box:chart ch 'bgr cmy#'\n"
"subplot 2 2 3:title 'Ring chart; \" \" color':rotate 50 60:\n"
"axis '(y+2)/3*cos(pi*x)' '(y+2)/3*sin(pi*x)' '':box:chart ch 'bgr cmy#'\n";
void smgl_chart(mglGraph *gr)
{
	mglData ch(7,2);	for(int i=0;i<7*2;i++)	ch.a[i]=mgl_rnd()+0.1;
	if(!mini)	{	gr->SubPlot(2,2,0);	gr->Title("Chart plot (default)");	}
	gr->Light(true);	gr->Rotate(50,60);	gr->Box();	gr->Chart(ch);
	if(mini)	return;
	gr->SubPlot(2,2,1);	gr->Title("'\\#' style");
	gr->Rotate(50,60);	gr->Box();	gr->Chart(ch,"#");
	gr->SubPlot(2,2,2);	gr->Title("Pie chart; ' ' color");
	gr->SetFunc("(y+1)/2*cos(pi*x)","(y+1)/2*sin(pi*x)","");
	gr->Rotate(50,60);	gr->Box();	gr->Chart(ch,"bgr cmy#");
	gr->SubPlot(2,2,3);	gr->Title("Ring chart; ' ' color");
	gr->SetFunc("(y+2)/3*cos(pi*x)","(y+2)/3*sin(pi*x)","");
	gr->Rotate(50,60);	gr->Box();	gr->Chart(ch,"bgr cmy#");
}
//-----------------------------------------------------------------------------
const char *mmgl_mark="call 'prepare1d'\nsubplot 1 1 0 '':title 'Mark plot (default)':box:mark y y1 's'\n";
void smgl_mark(mglGraph *gr)
{
	mglData y,y1;	mgls_prepare1d(&y,&y1);
	if(!mini)	{	gr->SubPlot(1,1,0,"");	gr->Title("Mark plot (default)");	}
	gr->Box();	gr->Mark(y,y1,"s");
}
//-----------------------------------------------------------------------------
const char *mmgl_radar="new yr 10 3 '0.4*sin(pi*(x+1.5+y/2)+0.1*rnd)'\n"
"subplot 1 1 0 '':title 'Radar plot (with grid, \"\\#\")':radar yr '#'\n";
void smgl_radar(mglGraph *gr)
{
	mglData yr(10,3);	yr.Modify("0.4*sin(pi*(2*x+y))+0.1*rnd");
	if(!mini)	{	gr->SubPlot(1,1,0,"");	gr->Title("Radar plot (with grid, '\\#')");	}
	gr->Radar(yr,"#");
}
//-----------------------------------------------------------------------------
const char *mmgl_candle="new y 30 'sin(pi*x/2)^2':copy y1 y/2:copy y2 (y+1)/2\n"
"subplot 1 1 0 '':title 'Candle plot (default)'\nyrange 0 1:box:candle y y1 y2\n";
void smgl_candle(mglGraph *gr)
{
	mglData y(30);	gr->Fill(y,"sin(pi*x/2)^2");
	mglData y1(30);	gr->Fill(y1,"v/2",y);
	mglData y2(30);	gr->Fill(y2,"(1+v)/2",y);
	if(!mini)	{	gr->SubPlot(1,1,0,"");	gr->Title("Candle plot (default)");	}
	gr->SetRange('y',0,1);	gr->Box();	gr->Candle(y,y1,y2);
}
//-----------------------------------------------------------------------------
const char *mmgl_textmark="call 'prepare1d'\nsubplot 1 1 0 '':title 'TextMark plot (default)':box:textmark y y1 '\\gamma' 'r'\n";
void smgl_textmark(mglGraph *gr)
{
	mglData y,y1;	mgls_prepare1d(&y,&y1);
	if(!mini)	{	gr->SubPlot(1,1,0,"");	gr->Title("TextMark plot (default)");	}
	gr->Box();	gr->TextMark(y,y1,"\\gamma","r");
}
//-----------------------------------------------------------------------------
const char *mmgl_tube="call 'prepare1d'\nlight on\n"
"new yc 50 'sin(pi*x)':new xc 50 'cos(pi*x)':new z 50 'x':divto y1 20\n"
"subplot 2 2 0 '':title 'Tube plot (default)':box:tube y 0.05\n"
"subplot 2 2 1 '':title 'variable radius':box:tube y y1\n"
"subplot 2 2 2 '':title '\"\\#\" style':box:tube y 0.05 '#'\n"
"subplot 2 2 3:title '3d variant':rotate 50 60:box:tube xc yc z y2 'r'\n";
void smgl_tube(mglGraph *gr)
{
	mglData y,y1,y2;	mgls_prepare1d(&y,&y1,&y2);	y1/=20;
	if(!mini)	{	gr->SubPlot(2,2,0,"");	gr->Title("Tube plot (default)");	}
	gr->Light(true);	gr->Box();	gr->Tube(y,0.05);
	if(mini)	return;
	gr->SubPlot(2,2,1,"");	gr->Title("variable radius");	gr->Box();	gr->Tube(y,y1);
	gr->SubPlot(2,2,2,"");	gr->Title("'\\#' style");	gr->Box();	gr->Tube(y,0.05,"#");
	mglData yc(50), xc(50), z(50);	z.Modify("2*x-1");
	yc.Modify("sin(pi*(2*x-1))");	xc.Modify("cos(pi*2*x-pi)");
	gr->SubPlot(2,2,3);	gr->Title("3d variant");	gr->Rotate(50,60);	gr->Box();	gr->Tube(xc,yc,z,y2,"r");
}
//-----------------------------------------------------------------------------
const char *mmgl_tape="call 'prepare1d'\nnew yc 50 'sin(pi*x)':new xc 50 'cos(pi*x)':new z 50 'x'\n"
"subplot 2 2 0 '':title 'Tape plot (default)':box:tape y:plot y 'k'\n"
"subplot 2 2 1:title '3d variant, 2 colors':rotate 50 60:light on\n"
"box:plot xc yc z 'k':tape xc yc z 'rg'\n"
"subplot 2 2 2:title '3d variant, x only':rotate 50 60\n"
"box:plot xc yc z 'k':tape xc yc z 'xr':tape xc yc z 'xr#'\n"
"subplot 2 2 3:title '3d variant, z only':rotate 50 60\n"
"box:plot xc yc z 'k':tape xc yc z 'zg':tape xc yc z 'zg#'\n";
void smgl_tape(mglGraph *gr)
{
	mglData y;	mgls_prepare1d(&y);
	mglData xc(50), yc(50), z(50);
	yc.Modify("sin(pi*(2*x-1))");
	xc.Modify("cos(pi*2*x-pi)");	z.Fill(-1,1);
	if(!mini)	{	gr->SubPlot(2,2,0,"");	gr->Title("Tape plot (default)");	}
	gr->Box();	gr->Tape(y);	gr->Plot(y,"k");
	if(mini)	return;
	gr->SubPlot(2,2,1);	gr->Title("3d variant, 2 colors");	gr->Rotate(50,60);	gr->Light(true);
	gr->Box();	gr->Plot(xc,yc,z,"k");	gr->Tape(xc,yc,z,"rg");
	gr->SubPlot(2,2,2);	gr->Title("3d variant, x only");	gr->Rotate(50,60);
	gr->Box();	gr->Plot(xc,yc,z,"k");	gr->Tape(xc,yc,z,"xr");	gr->Tape(xc,yc,z,"xr#");
	gr->SubPlot(2,2,3);	gr->Title("3d variant, z only");	gr->Rotate(50,60);
	gr->Box();	gr->Plot(xc,yc,z,"k");	gr->Tape(xc,yc,z,"zg");	gr->Tape(xc,yc,z,"zg#");
}
//-----------------------------------------------------------------------------
const char *mmgl_fog="call 'prepare2d'\ntitle 'Fog sample':rotate 50 60:light on:fog 1\nbox:surf a\n";
void smgl_fog(mglGraph *gr)
{
	mglData a;	mgls_prepare2d(&a);
	if(!mini)	gr->Title("Fog sample");
	gr->Light(true);	gr->Rotate(50,60);	gr->Fog(1);	gr->Box();
	gr->Surf(a);
}
//-----------------------------------------------------------------------------
const char *mmgl_map="new a 50 40 'x':new b 50 40 'y':zrange -2 2:text 0 0 '\\to'\n"
"subplot 2 1 0:text 0 1.1 '\\{x, y\\}' '' -2:box:map a b 'brgk' 0 0\n"
"subplot 2 1 1:text 0 1.1 '\\{\\frac{x^3+y^3}{2}, \\frac{x-y}{2}\\}' '' -2\n"
"box:fill a '(x^3+y^3)/2':fill b '(x-y)/2':map a b 'brgk' 0 0\n";
void smgl_map(mglGraph *gr)	// example of mapping
{
	mglData a(50, 40), b(50, 40);
	gr->Puts(mglPoint(0, 0), "\\to", ":C", -1.4);
	gr->SetRanges(-1,1,-1,1,-2,2);

	gr->SubPlot(2, 1, 0);
	gr->Fill(a,"x");	gr->Fill(b,"y");
	gr->Puts(mglPoint(0, 1.1), "\\{x, y\\}", ":C", -2);		gr->Box();
	gr->Map(a, b, "brgk");

	gr->SubPlot(2, 1, 1);
	gr->Fill(a,"(x^3+y^3)/2");	gr->Fill(b,"(x-y)/2");
	gr->Puts(mglPoint(0, 1.1), "\\{\\frac{x^3+y^3}{2}, \\frac{x-y}{2}\\}", ":C", -2);
	gr->Box();
	gr->Map(a, b, "brgk");
}
//-----------------------------------------------------------------------------
const char *mmgl_stfa="new a 2000:new b 2000\nfill a 'cos(50*pi*x)*(x<-.5)+cos(100*pi*x)*(x<0)*(x>-.5)+\\\n"
"cos(200*pi*x)*(x<.5)*(x>0)+cos(400*pi*x)*(x>.5)'\n"
"subplot 1 2 0 '<_':title 'Initial signal':plot a:axis:xlabel '\\i t'\n"
"subplot 1 2 1 '<_':title 'STFA plot':stfa a b 64:axis:ylabel '\\omega' 0:xlabel '\\i t'\n";
void smgl_stfa(mglGraph *gr)	// STFA sample
{
	mglData a(2000), b(2000);
	gr->Fill(a,"cos(50*pi*x)*(x<-.5)+cos(100*pi*x)*(x<0)*(x>-.5)+\
	cos(200*pi*x)*(x<.5)*(x>0)+cos(400*pi*x)*(x>.5)");
	gr->SubPlot(1, 2, 0,"<_");	gr->Title("Initial signal");
	gr->Plot(a);
	gr->Axis();
	gr->Label('x', "\\i t");

	gr->SubPlot(1, 2, 1,"<_");	gr->Title("STFA plot");
	gr->STFA(a, b, 64);
	gr->Axis();
	gr->Label('x', "\\i t");
	gr->Label('y', "\\omega", 0);
}
//-----------------------------------------------------------------------------
const char *mmgl_qo2d="define $1 'p^2+q^2-x-1+i*0.5*(y+x)*(y>-x)'\n"
"subplot 1 1 0 '<_':title 'Beam and ray tracing'\n"
"ray r $1 -0.7 -1 0 0 0.5 0 0.02 2:plot r(0) r(1) 'k'\naxis:xlabel '\\i x':ylabel '\\i z'\n"
"new re 128 'exp(-48*x^2)':new im 128\nnew xx 1:new yy 1\nqo2d a $1 re im r 1 30 xx yy\n"
"crange 0 1:dens xx yy a 'wyrRk':fplot '-x' 'k|'\n"
"text 0 0.85 'absorption: (x+y)/2 for x+y>0'\ntext 0.7 -0.05 'central ray'";
void smgl_qo2d(mglGraph *gr)
{
	mglData r, xx, yy, a, im(128), re(128);
	const char *ham = "p^2+q^2-x-1+i*0.5*(y+x)*(y>-x)";
	r = mglRay(ham, mglPoint(-0.7, -1), mglPoint(0, 0.5), 0.02, 2);
	if(!mini)	{gr->SubPlot(1,1,0,"<_");	gr->Title("Beam and ray tracing");}
	gr->Plot(r.SubData(0), r.SubData(1), "k");
	gr->Axis();	gr->Label('x', "\\i x");	gr->Label('y', "\\i y");
	// now start beam tracing
	gr->Fill(re,"exp(-48*x^2)");
	a = mglQO2d(ham, re, im, r, xx, yy, 1, 30);
	gr->SetRange('c',0, 1);
	gr->Dens(xx, yy, a, "wyrRk");
	gr->FPlot("-x", "k|");
	gr->Puts(mglPoint(0, 0.85), "absorption: (x+y)/2 for x+y>0");
	gr->Puts(mglPoint(0.7, -0.05), "central ray");
}
//-----------------------------------------------------------------------------
const char *mmgl_pde="new re 128 'exp(-48*(x+0.7)^2)':new im 128\n"
"pde a 'p^2+q^2-x-1+i*0.5*(z+x)*(z>-x)' re im 0.01 30\ntranspose a\n"
"subplot 1 1 0 '<_':title 'PDE solver'\n"
"axis:xlabel '\\i x':ylabel '\\i z'\ncrange 0 1:dens a 'wyrRk'\n"
"fplot '-x' 'k|'\n"
"text 0 0.95 'Equation: ik_0\\partial_zu + \\Delta u + x\\cdot u + i \\frac{x+z}{2}\\cdot u = 0\\n{}absorption: (x+z)/2 for x+z>0'";
void smgl_pde(mglGraph *gr)	// PDE sample
{
	mglData a,re(128),im(128);
	gr->Fill(re,"exp(-48*(x+0.7)^2)");
	a = gr->PDE("p^2+q^2-x-1+i*0.5*(z+x)*(z>-x)", re, im, 0.01, 30);
	a.Transpose("yxz");
	if(!mini)	{gr->SubPlot(1,1,0,"<_");	gr->Title("PDE solver");	}
	gr->SetRange('c',0,1);	gr->Dens(a,"wyrRk");
	gr->Axis();	gr->Label('x', "\\i x");	gr->Label('y', "\\i z");
	gr->FPlot("-x", "k|");
	gr->Puts(mglPoint(0, 0.95), "Equation: ik_0\\partial_zu + \\Delta u + x\\cdot u + i \\frac{x+z}{2}\\cdot u = 0\nabsorption: (x+z)/2 for x+z>0");
}
//-----------------------------------------------------------------------------
const char *mmgl_conta="call 'prepare3d'\ntitle 'Cont3 sample':rotate 50 60:box\ncont3 c 'x':cont3 c:cont3 c 'z'";
void smgl_conta(mglGraph *gr)
{
	mglData c;	mgls_prepare3d(&c);
	if(!mini)	gr->Title("Cont3 sample");
	gr->Rotate(50,60);	gr->Box();
	gr->Cont3(c,"x");	gr->Cont3(c);	gr->Cont3(c,"z");
}
//-----------------------------------------------------------------------------
const char *mmgl_contfa="call 'prepare3d'\ntitle 'Cont3 sample':rotate 50 60:box:light on\n"
"contf3 c 'x':contf3 c:contf3 c 'z'\ncont3 c 'xk':cont3 c 'k':cont3 c 'zk'\n";
void smgl_contfa(mglGraph *gr)
{
	mglData c;	mgls_prepare3d(&c);
	if(!mini)	gr->Title("ContF3 sample");
	gr->Rotate(50,60);	gr->Light(true);	gr->Box();
	gr->ContF3(c,"x");	gr->ContF3(c);		gr->ContF3(c,"z");
	gr->Cont3(c,"kx");	gr->Cont3(c,"k");	gr->Cont3(c,"kz");
}
//-----------------------------------------------------------------------------
const char *mmgl_densa="call 'prepare3d'\ntitle 'Dens3 sample':rotate 50 60:alpha on:alphadef 0.7\n"
"origin 0 0 0:box:axis '_xyz'\ndens3 c 'x':dens3 c ':y':dens3 c 'z'";
void smgl_densa(mglGraph *gr)
{
	mglData c;	mgls_prepare3d(&c);
	if(!mini)	gr->Title("Dens3 sample");
	gr->Rotate(50,60);	gr->Alpha(true);	gr->SetAlphaDef(0.7);
	gr->SetOrigin(0,0,0);	gr->Axis("_xyz");	gr->Box();
	gr->Dens3(c,"x");	gr->Dens3(c);	gr->Dens3(c,"z");
}
//-----------------------------------------------------------------------------
const char *mmgl_dens_xyz="call 'prepare3d'\ntitle 'Dens[XYZ] sample':rotate 50 60:box\n"
"densx {sum c 'x'} '' -1:densy {sum c 'y'} '' 1:densz {sum c 'z'} '' -1";
void smgl_dens_xyz(mglGraph *gr)
{
	mglData c;	mgls_prepare3d(&c);
	if(!mini)	gr->Title("Dens[XYZ] sample");
	gr->Rotate(50,60);	gr->Box();	gr->DensX(c.Sum("x"),0,-1);
	gr->DensY(c.Sum("y"),0,1);		gr->DensZ(c.Sum("z"),0,-1);
}
//-----------------------------------------------------------------------------
const char *mmgl_cont_xyz="call 'prepare3d'\ntitle 'Cont[XYZ] sample':rotate 50 60:box\n"
"contx {sum c 'x'} '' -1:conty {sum c 'y'} '' 1:contz {sum c 'z'} '' -1";
void smgl_cont_xyz(mglGraph *gr)
{
	mglData c;	mgls_prepare3d(&c);
	if(!mini)	gr->Title("Cont[XYZ] sample");
	gr->Rotate(50,60);	gr->Box();	gr->ContX(c.Sum("x"),"",-1);
	gr->ContY(c.Sum("y"),"",1);		gr->ContZ(c.Sum("z"),"",-1);
}
//-----------------------------------------------------------------------------
const char *mmgl_contf_xyz="call 'prepare3d'\ntitle 'ContF[XYZ] sample':rotate 50 60:box\n"
"contfx {sum c 'x'} '' -1:contfy {sum c 'y'} '' 1:contfz {sum c 'z'} '' -1";
void smgl_contf_xyz(mglGraph *gr)
{
	mglData c;	mgls_prepare3d(&c);
	if(!mini)	gr->Title("ContF[XYZ] sample");
	gr->Rotate(50,60);	gr->Box();	gr->ContFX(c.Sum("x"),"",-1);
	gr->ContFY(c.Sum("y"),"",1);	gr->ContFZ(c.Sum("z"),"",-1);
}
//-----------------------------------------------------------------------------
const char *mmgl_cloud="call 'prepare3d'\nsubplot 2 2 0:title 'Cloud plot':rotate 50 60:alpha on:box:cloud c 'wyrRk'\n"
"subplot 2 2 1:title '\"!\" style':rotate 50 60:box:cloud c '!wyrRk'\n"
"subplot 2 2 2:title '\".\" style':rotate 50 60:box:cloud c '.wyrRk'\n"
"subplot 2 2 3:title 'meshnum 10':rotate 50 60:box:cloud c 'wyrRk'; meshnum 10\n";
void smgl_cloud(mglGraph *gr)
{
	mglData c;	mgls_prepare3d(&c);
	if(!mini)	{	gr->SubPlot(2,2,0);	gr->Title("Cloud plot");	}
	gr->Rotate(50,60);	gr->Alpha(true);
	gr->Box();	gr->Cloud(c,"wyrRk");
	if(mini)	return;
	gr->SubPlot(2,2,1);	gr->Title("'!' style");
	gr->Rotate(50,60);	gr->Box();	gr->Cloud(c,"!wyrRk");
	gr->SubPlot(2,2,2);	gr->Title("'.' style");
	gr->Rotate(50,60);	gr->Box();	gr->Cloud(c,".wyrRk");
	gr->SubPlot(2,2,3);	gr->Title("meshnum 10");
	gr->Rotate(50,60);	gr->Box();	gr->Cloud(c,"wyrRk","meshnum 10");
}
//-----------------------------------------------------------------------------
const char *mmgl_cont="call 'prepare2d'\nlist v -0.5 -0.15 0 0.15 0.5\nsubplot 2 2 0:title 'Cont plot (default)':rotate 50 60:box:cont a\n"
"subplot 2 2 1:title 'manual levels':rotate 50 60:box:cont v a\n"
"subplot 2 2 2:title '\"\\_\" style':rotate 50 60:box:cont a '_'\n"
"subplot 2 2 3 '':title '\"t\" style':box:cont a 't'\n";
void smgl_cont(mglGraph *gr)
{
	mglData a,v(5);	mgls_prepare2d(&a);	v.a[0]=-0.5;	v.a[1]=-0.15;	v.a[2]=0;	v.a[3]=0.15;	v.a[4]=0.5;
	if(!mini)	{	gr->SubPlot(2,2,0);	gr->Title("Cont plot (default)");	}
	gr->Rotate(50,60);	gr->Box();	gr->Cont(a);
	if(mini)	return;
	gr->SubPlot(2,2,1);	gr->Title("manual levels");
	gr->Rotate(50,60);	gr->Box();	gr->Cont(v,a);
	gr->SubPlot(2,2,2);	gr->Title("'\\_' style");
	gr->Rotate(50,60);	gr->Box();	gr->Cont(a,"_");
	gr->SubPlot(2,2,3,"");	gr->Title("'t' style");
	gr->Box();	gr->Cont(a,"t");
}
//-----------------------------------------------------------------------------
const char *mmgl_contf="call 'prepare2d'\nlist v -0.5 -0.15 0 0.15 0.5\n"
"new a1 30 40 3 '0.6*sin(2*pi*x+pi*(z+1)/2)*sin(3*pi*y+pi*z) + 0.4*cos(3*pi*(x*y)+pi*(z+1)^2/2)'\n"
"subplot 2 2 0:title 'ContF plot (default)':rotate 50 60:box:contf a\n"
"subplot 2 2 1:title 'manual levels':rotate 50 60:box:contf v a\n"
"subplot 2 2 2:title '\"\\_\" style':rotate 50 60:box:contf a '_'\n"
"subplot 2 2 3:title 'several slices':rotate 50 60:box:contf a1\n";
void smgl_contf(mglGraph *gr)
{
	mglData a,v(5),a1(30,40,3);	mgls_prepare2d(&a);	v.a[0]=-0.5;
	v.a[1]=-0.15;	v.a[2]=0;	v.a[3]=0.15;	v.a[4]=0.5;
	gr->Fill(a1,"0.6*sin(2*pi*x+pi*(z+1)/2)*sin(3*pi*y+pi*z) + 0.4*cos(3*pi*(x*y)+pi*(z+1)^2/2)");

	if(!mini)	{	gr->SubPlot(2,2,0);	gr->Title("ContF plot (default)");	}
	gr->Rotate(50,60);	gr->Box();	gr->ContF(a);
	if(mini)	return;
	gr->SubPlot(2,2,1);	gr->Title("manual levels");
	gr->Rotate(50,60);	gr->Box();	gr->ContF(v,a);
	gr->SubPlot(2,2,2);	gr->Title("'\\_' style");
	gr->Rotate(50,60);	gr->Box();	gr->ContF(a,"_");
	gr->SubPlot(2,2,3);	gr->Title("several slices");
	gr->Rotate(50,60);	gr->Box();	gr->ContF(a1);
}
//-----------------------------------------------------------------------------
const char *mmgl_contd="call 'prepare2d'\nlist v -0.5 -0.15 0 0.15 0.5\n"
"new a1 30 40 3 '0.6*sin(2*pi*x+pi*(z+1)/2)*sin(3*pi*y+pi*z) + 0.4*cos(3*pi*(x*y)+pi*(z+1)^2/2)'\n"
"subplot 2 2 0:title 'ContD plot (default)':rotate 50 60:box:contd a\n"
"subplot 2 2 1:title 'manual levels':rotate 50 60:box:contd v a\n"
"subplot 2 2 2:title '\"\\_\" style':rotate 50 60:box:contd a '_'\n"
"subplot 2 2 3:title 'several slices':rotate 50 60:box:contd a1\n";
void smgl_contd(mglGraph *gr)
{
	mglData a,v(5),a1(30,40,3);	mgls_prepare2d(&a);	v.a[0]=-0.5;
	v.a[1]=-0.15;	v.a[2]=0;	v.a[3]=0.15;	v.a[4]=0.5;
	gr->Fill(a1,"0.6*sin(2*pi*x+pi*(z+1)/2)*sin(3*pi*y+pi*z) + 0.4*cos(3*pi*(x*y)+pi*(z+1)^2/2)");

	if(!mini)	{	gr->SubPlot(2,2,0);	gr->Title("ContD plot (default)");	}
	gr->Rotate(50,60);	gr->Box();	gr->ContD(a);
	if(mini)	return;
	gr->SubPlot(2,2,1);	gr->Title("manual levels");
	gr->Rotate(50,60);	gr->Box();	gr->ContD(v,a);
	gr->SubPlot(2,2,2);	gr->Title("'\\_' style");
	gr->Rotate(50,60);	gr->Box();	gr->ContD(a,"_");
	gr->SubPlot(2,2,3);	gr->Title("several slices");
	gr->Rotate(50,60);	gr->Box();	gr->ContD(a1);
}
//-----------------------------------------------------------------------------
const char *mmgl_contv="call 'prepare2d'\nlist v -0.5 -0.15 0 0.15 0.5\n"
"subplot 2 2 0:title 'ContV plot (default)':rotate 50 60:box:contv a\n"
"subplot 2 2 1:title 'manual levels':rotate 50 60:box:contv v a\n"
"subplot 2 2 2:title '\"\\_\" style':rotate 50 60:box:contv a '_'\n"
"subplot 2 2 3:title 'ContV and ContF':rotate 50 60:light on:box\ncontv a:contf a:cont a 'k'\n";
void smgl_contv(mglGraph *gr)
{
	mglData a,v(5);	mgls_prepare2d(&a);	v.a[0]=-0.5;
	v.a[1]=-0.15;	v.a[2]=0;	v.a[3]=0.15;	v.a[4]=0.5;
	if(!mini)	{	gr->SubPlot(2,2,0);	gr->Title("ContV plot (default)");	}
	gr->Rotate(50,60);	gr->Box();	gr->ContV(a);
	if(mini)	return;
	gr->SubPlot(2,2,1);	gr->Title("manual levels");
	gr->Rotate(50,60);	gr->Box();	gr->ContV(v,a);
	gr->SubPlot(2,2,2);	gr->Title("'\\_' style");
	gr->Rotate(50,60);	gr->Box();	gr->ContV(a,"_");
	gr->SubPlot(2,2,3);	gr->Title("ContV and ContF");
	gr->Rotate(50,60);	gr->Box();	gr->Light(true);
	gr->ContV(a);	gr->ContF(a);	gr->Cont(a,"k");
}
//-----------------------------------------------------------------------------
const char *mmgl_torus="call 'prepare1d'\nsubplot 2 2 0:title 'Torus plot (default)':light on:rotate 50 60:box:torus y1 y2\n"
"subplot 2 2 1:title '\"x\" style':light on:rotate 50 60:box:torus y1 y2 'x'\n"
"subplot 2 2 2:title '\"z\" style':light on:rotate 50 60:box:torus y1 y2 'z'\n"
"subplot 2 2 3:title '\"\\#\" style':light on:rotate 50 60:box:torus y1 y2 '#'\n";
void smgl_torus(mglGraph *gr)
{
	mglData y1,y2;	mgls_prepare1d(0,&y1,&y2);
	if(!mini)	{	gr->SubPlot(2,2,0);	gr->Title("Torus plot (default)");	}
	gr->Light(true);	gr->Rotate(50,60);	gr->Box();	gr->Torus(y1,y2);
	if(mini)	return;
	gr->SubPlot(2,2,1);	gr->Title("'x' style");	gr->Rotate(50,60);	gr->Box();	gr->Torus(y1,y2,"x");
	gr->SubPlot(2,2,2);	gr->Title("'z' style");	gr->Rotate(50,60);	gr->Box();	gr->Torus(y1,y2,"z");
	gr->SubPlot(2,2,3);	gr->Title("'\\#' style");	gr->Rotate(50,60);	gr->Box();	gr->Torus(y1,y2,"#");
}
//-----------------------------------------------------------------------------
const char *mmgl_axial="call 'prepare2d'\nsubplot 2 2 0:title 'Axial plot (default)':light on:alpha on:rotate 50 60:box:axial a\n"
"subplot 2 2 1:title '\"x\" style':light on:rotate 50 60:box:axial a 'x'\n"
"subplot 2 2 2:title '\"z\" style':light on:rotate 50 60:box:axial a 'z'\n"
"subplot 2 2 3:title '\"\\#\" style':light on:rotate 50 60:box:axial a '#'\n";
void smgl_axial(mglGraph *gr)
{
	mglData a;	mgls_prepare2d(&a);
	if(!mini)	{	gr->SubPlot(2,2,0);	gr->Title("Axial plot (default)");	}
	gr->Light(true);	gr->Alpha(true);	gr->Rotate(50,60);	gr->Box();	gr->Axial(a);
	if(mini)	return;
	gr->SubPlot(2,2,1);	gr->Title("'x' style");	gr->Rotate(50,60);	gr->Box();	gr->Axial(a,"x");
	gr->SubPlot(2,2,2);	gr->Title("'z' style");	gr->Rotate(50,60);	gr->Box();	gr->Axial(a,"z");
	gr->SubPlot(2,2,3);	gr->Title("'\\#' style");	gr->Rotate(50,60);	gr->Box();	gr->Axial(a,"#");
}
//-----------------------------------------------------------------------------
const char *mmgl_several_light="call 'prepare2d'\ntitle 'Several light sources':rotate 50 60:light on\n"
"light 1 0 1 0 'c':light 2 1 0 0 'y':light 3 0 -1 0 'm'\nbox:surf a 'h'\n";
void smgl_several_light(mglGraph *gr)	// several light sources
{
	mglData a;	mgls_prepare2d(&a);
	if(!mini)	gr->Title("Several light sources");
	gr->Rotate(50,60);	gr->Light(true);	gr->AddLight(1,mglPoint(0,1,0),'c');
	gr->AddLight(2,mglPoint(1,0,0),'y');	gr->AddLight(3,mglPoint(0,-1,0),'m');
	gr->Box();	gr->Surf(a,"h");
}
//-----------------------------------------------------------------------------
const char *mmgl_surf3="call 'prepare3d'\ntitle 'Surf3 plot':rotate 50 60:light on:alpha on:box:surf3 c";
void smgl_surf3(mglGraph *gr)
{
	mglData c;	mgls_prepare3d(&c);
	if(!mini)	gr->Title("Surf3 plot");
	gr->Rotate(50,60);	gr->Light(true);	gr->Alpha(true);
	gr->Box();	gr->Surf3(c);
}
//-----------------------------------------------------------------------------
const char *mmgl_surf3a="call 'prepare3d'\ntitle 'Surf3 plot':rotate 50 60:light on:alpha on:box:surf3a c d";
void smgl_surf3a(mglGraph *gr)
{
	mglData c,d;	mgls_prepare3d(&c,&d);
	if(!mini)	gr->Title("Surf3A plot");
	gr->Rotate(50,60);	gr->Light(true);	gr->Alpha(true);
	gr->Box();	gr->Surf3A(c,d);
}
//-----------------------------------------------------------------------------
const char *mmgl_surf3c="call 'prepare3d'\ntitle 'Surf3 plot':rotate 50 60:light on:alpha on:box:surf3c c d";
void smgl_surf3c(mglGraph *gr)
{
	mglData c,d;	mgls_prepare3d(&c,&d);
	if(!mini)	gr->Title("Surf3C plot");
	gr->Rotate(50,60);	gr->Light(true);	gr->Alpha(true);
	gr->Box();	gr->Surf3C(c,d);
}
//-----------------------------------------------------------------------------
const char *mmgl_cut="call 'prepare2d'\ncall 'prepare3d'\nsubplot 2 2 0:title 'Cut on (default)':rotate 50 60:light on:box:surf a; zrange -1 0.5\n"
"subplot 2 2 1:title 'Cut off':rotate 50 60:box:surf a; zrange -1 0.5; cut off\n"
"subplot 2 2 2:title 'Cut in box':rotate 50 60:box:alpha on\ncut 0 -1 -1 1 0 1.1:surf3 c\ncut 0 0 0 0 0 0\t# restore back\n"
"subplot 2 2 3:title 'Cut by formula':rotate 50 60:box\ncut '(z>(x+0.5*y-1)^2-1) & (z>(x-0.5*y-1)^2-1)':surf3 c";
void smgl_cut(mglGraph *gr)	// cutting
{
	mglData a,c,v(1);	mgls_prepare2d(&a);	mgls_prepare3d(&c);	v.a[0]=0.5;
	gr->SubPlot(2,2,0);	gr->Title("Cut on (default)");	gr->Rotate(50,60);	gr->Light(true);
	gr->Box();	gr->Surf(a,"","zrange -1 0.5");
	gr->SubPlot(2,2,1);	gr->Title("Cut off");		gr->Rotate(50,60);
	gr->Box();	gr->Surf(a,"","zrange -1 0.5; cut off");
	gr->SubPlot(2,2,2);	gr->Title("Cut in box");	gr->Rotate(50,60);
	gr->SetCutBox(mglPoint(0,-1,-1), mglPoint(1,0,1.1));
	gr->Alpha(true);	gr->Box();	gr->Surf3(c);
	gr->SetCutBox(mglPoint(0), mglPoint(0));	// switch it off
	gr->SubPlot(2,2,3);	gr->Title("Cut by formula");	gr->Rotate(50,60);
	gr->CutOff("(z>(x+0.5*y-1)^2-1) & (z>(x-0.5*y-1)^2-1)");
	gr->Box();	gr->Surf3(c);	gr->CutOff("");	// switch it off
}
//-----------------------------------------------------------------------------
const char *mmgl_traj="call 'prepare1d'\nsubplot 1 1 0 '':title 'Traj plot':box:plot x1 y:traj x1 y y1 y2\n";
void smgl_traj(mglGraph *gr)
{
	mglData x,y,y1,y2;	mgls_prepare1d(&y,&y1,&y2,&x);
	if(!mini)	{gr->SubPlot(1,1,0,"");	gr->Title("Traj plot");}
	gr->Box();	gr->Plot(x,y);	gr->Traj(x,y,y1,y2);
}
//-----------------------------------------------------------------------------
const char *mmgl_mesh="call 'prepare2d'\ntitle 'Mesh plot':rotate 50 60:box:mesh a\n";
void smgl_mesh(mglGraph *gr)
{
	mglData a;	mgls_prepare2d(&a);
	if(!mini)	gr->Title("Mesh plot");
	gr->Rotate(50,60);	gr->Box();	gr->Mesh(a);
}
//-----------------------------------------------------------------------------
const char *mmgl_fall="call 'prepare2d'\ntitle 'Fall plot':rotate 50 60:box:fall a\n";
void smgl_fall(mglGraph *gr)
{
	mglData a;	mgls_prepare2d(&a);
	if(!mini)	gr->Title("Fall plot");
	gr->Rotate(50,60);	gr->Box();	gr->Fall(a);
}
//-----------------------------------------------------------------------------
const char *mmgl_surf="call 'prepare2d'\nsubplot 2 2 0:title 'Surf plot (default)':rotate 50 60:light on:box:surf a\n"
"subplot 2 2 1:title '\"\\#\" style; meshnum 10':rotate 50 60:box:surf a '#'; meshnum 10\n"
"subplot 2 2 2:title '\"|\" style':rotate 50 60:box:surf a '|'\n"
"new x 50 40 '0.8*sin(pi*x)*sin(pi*(y+1)/2)'\nnew y 50 40 '0.8*cos(pi*x)*sin(pi*(y+1)/2)'\nnew z 50 40 '0.8*cos(pi*(y+1)/2)'\n"
"subplot 2 2 3:title 'parametric form':rotate 50 60:box:surf x y z 'BbwrR'\n";
void smgl_surf(mglGraph *gr)
{
	mglData a;	mgls_prepare2d(&a);
	if(!mini)	{	gr->SubPlot(2,2,0);	gr->Title("Surf plot (default)");	}
	gr->Light(true);	gr->Rotate(50,60);	gr->Box();	gr->Surf(a);
	if(mini)	return;
	gr->SubPlot(2,2,1);	gr->Title("'\\#' style; meshnum 10");
	gr->Rotate(50,60);	gr->Box();	gr->Surf(a,"#","meshnum 10");
	gr->SubPlot(2,2,2);	gr->Title("'|' style");
	gr->Rotate(50,60);	gr->Box();	gr->Surf(a,"|");
	gr->SubPlot(2,2,3);	gr->Title("parametric form");
	mglData x(50,40),y(50,40),z(50,40);
	gr->Fill(x,"0.8*sin(pi*x)*sin(pi*(y+1)/2)");
	gr->Fill(y,"0.8*cos(pi*x)*sin(pi*(y+1)/2)");
	gr->Fill(z,"0.8*cos(pi*(y+1)/2)");
	gr->Rotate(50,60);	gr->Box();	gr->Surf(x,y,z,"BbwrR");
}
//-----------------------------------------------------------------------------
const char *mmgl_parser="title 'MGL parser sample'\ncall 'sample'\nstop\nfunc 'sample'\n"
"new dat 100 'sin(2*pi*(x+1))'\nplot dat; xrange 0 1\nbox\naxis\n"
"xlabel 'x'\nylabel 'y'\nbox\nfor $0 -1 1 0.1\nif $0<0\n"
"line 0 0 -1 $0 'r':else:line 0 0 -1 $0 'g'\nendif\nnext";
void smgl_parser(mglGraph *gr)	// example of MGL parsing
{
	gr->Title("MGL parser sample");
	double a[100];   // let a_i = sin(4*pi*x), x=0...1
	for(int i=0;i<100;i++)a[i]=sin(4*M_PI*i/99);
	mglParse *parser = new mglParse;
	mglData *d = parser->AddVar("dat");
	d->Set(a,100); // set data to variable
	parser->Execute(gr, "plot dat; xrange 0 1\nbox\naxis");
	// you may break script at any line do something
	// and continue after that
	parser->Execute(gr, "xlabel 'x'\nylabel 'y'\nbox");
	// also you may use cycles or conditions in script
	parser->Execute(gr, "for $0 -1 1 0.1\nif $0<0\n"
		"line 0 0 -1 $0 'r':else:line 0 0 -1 $0 'g'\n"
		"endif\nnext");
	delete parser;
}
//-----------------------------------------------------------------------------
const char *mmgl_belt="call 'prepare2d'\ntitle 'Belt plot':rotate 50 60:box:belt a\n";
void smgl_belt(mglGraph *gr)
{
	mglData a;	mgls_prepare2d(&a);
	if(!mini)	gr->Title("Belt plot");
	gr->Rotate(50,60);	gr->Box();	gr->Belt(a);
}
//-----------------------------------------------------------------------------
const char *mmgl_dens="call 'prepare2d'\nnew a1 30 40 3 '0.6*sin(2*pi*x+pi*(z+1)/2)*sin(3*pi*y+pi*z) + 0.4*cos(3*pi*(x*y)+pi*(z+1)^2/2)'\n"
"subplot 2 2 0 '':title 'Dens plot (default)':box:dens a\n"
"subplot 2 2 1:title '3d variant':rotate 50 60:box:dens a\n"
"subplot 2 2 2 '':title '\"\\#\" style; meshnum 10':box:dens a '#'; meshnum 10\n"
"subplot 2 2 3:title 'several slices':rotate 50 60:box:dens a1\n";
void smgl_dens(mglGraph *gr)
{
	mglData a,a1(30,40,3);	mgls_prepare2d(&a);
	gr->Fill(a1,"0.6*sin(2*pi*x+pi*(z+1)/2)*sin(3*pi*y+pi*z) + 0.4*cos(3*pi*(x*y)+pi*(z+1)^2/2)");
	if(!mini)	{gr->SubPlot(2,2,0,"");	gr->Title("Dens plot (default)");}
	gr->Box();	gr->Dens(a);
	if(mini)	return;
	gr->SubPlot(2,2,1);	gr->Title("3d variant");
	gr->Rotate(50,60);	gr->Box();	gr->Dens(a);
	gr->SubPlot(2,2,2,"");	gr->Title("'\\#' style; meshnum 10");
	gr->Box();	gr->Dens(a,"#","meshnum 10");
	gr->SubPlot(2,2,3);	gr->Title("several slices");
	gr->Rotate(50,60);		gr->Box();	gr->Dens(a1);
}
//-----------------------------------------------------------------------------
const char *mmgl_surfc="call 'prepare2d'\ntitle 'SurfC plot':rotate 50 60:light on:box:surfc a b\n";
void smgl_surfc(mglGraph *gr)
{
	mglData a,b;	mgls_prepare2d(&a,&b);
	if(!mini)	gr->Title("SurfC plot");	gr->Rotate(50,60);
	gr->Light(true);	gr->Box();	gr->SurfC(a,b);
}
//-----------------------------------------------------------------------------
const char *mmgl_surfa="call 'prepare2d'\ntitle 'SurfA plot':rotate 50 60:light on:alpha on:box:surfa a b\n";
void smgl_surfa(mglGraph *gr)
{
	mglData a,b;	mgls_prepare2d(&a,&b);
	if(!mini)	gr->Title("SurfA plot");	gr->Rotate(50,60);
	gr->Alpha(true);	gr->Light(true);	gr->Box();	gr->SurfA(a,b);
}
//-----------------------------------------------------------------------------
const char *mmgl_tile="call 'prepare2d'\ntitle 'Tile plot':rotate 50 60:box:tile a\n";
void smgl_tile(mglGraph *gr)
{
	mglData a;	mgls_prepare2d(&a);
	if(!mini)	gr->Title("Tile plot");
	gr->Rotate(40,60);	gr->Box();	gr->Tile(a);
}
//-----------------------------------------------------------------------------
const char *mmgl_tiles="call 'prepare2d'\nsubplot 1 1 0 '':title 'Tiles plot':box:tiles a b\n";
void smgl_tiles(mglGraph *gr)
{
	mglData a,b;	mgls_prepare2d(&a,&b);
	if(!mini)	{gr->SubPlot(1,1,0,"");	gr->Title("TileS plot");}
	gr->Box();	gr->TileS(a,b);
}
//-----------------------------------------------------------------------------
const char *mmgl_boxs="call 'prepare2d'\norigin 0 0 0\nsubplot 2 2 0:title 'Boxs plot (default)':rotate 40 60:light on:box:boxs a\n"
"subplot 2 2 1:title '\"\\@\" style':rotate 50 60:box:boxs a '@'\n"
"subplot 2 2 2:title '\"\\#\" style':rotate 50 60:box:boxs a '#'\n"
"subplot 2 2 3:title 'compare with Tile':rotate 50 60:box:tile a\n";
void smgl_boxs(mglGraph *gr)
{
	mglData a;	mgls_prepare2d(&a);
	gr->SetOrigin(0,0,0);	gr->Light(true);
	if(!mini)	{gr->SubPlot(2,2,0);	gr->Title("Boxs plot (default)");}
	gr->Rotate(40,60);	gr->Box();	gr->Boxs(a);
	if(mini)	return;
	gr->SubPlot(2,2,1);	gr->Title("'\\@' style");
	gr->Rotate(50,60);	gr->Box();	gr->Boxs(a,"@");
	gr->SubPlot(2,2,2);	gr->Title("'\\#' style");
	gr->Rotate(50,60);	gr->Box();	gr->Boxs(a,"#");
	gr->SubPlot(2,2,3);	gr->Title("compare with Tile");
	gr->Rotate(50,60);	gr->Box();	gr->Tile(a);
}
//-----------------------------------------------------------------------------
const char *mmgl_fit="new rnd 100 '0.4*rnd+0.1+sin(2*pi*x)'\nnew in 100 '0.3+sin(2*pi*x)'\n"
"list ini 1 1 3:fit res rnd 'a+b*sin(c*x)' 'abc' ini\n"
"title 'Fitting sample':yrange -2 2:box:axis:plot rnd '. '\n"
"plot res 'r':plot in 'b'\ntext -0.9 -1.3 'fitted:' 'r:L'\n"
"putsfit 0 -1.8 'y = ' 'r':text 0 2.2 'initial: y = 0.3+sin(2\\pi x)' 'b'\n";
void smgl_fit(mglGraph *gr)	// nonlinear fitting
{
	mglData rnd(100), in(100), res;
	gr->Fill(rnd,"0.4*rnd+0.1+sin(2*pi*x)");
	gr->Fill(in,"0.3+sin(2*pi*x)");
	double ini[3] = {1,1,3};
	mglData Ini(3,ini);
	res = gr->Fit(rnd, "a+b*sin(c*x)", "abc", Ini);
	if(!mini)	gr->Title("Fitting sample");
	gr->SetRange('y',-2,2);	gr->Box();	gr->Plot(rnd, ". ");
	gr->Axis();		gr->Plot(res, "r");	gr->Plot(in, "b");
	gr->Puts(mglPoint(-0.9, -1.3), "fitted:", "r:L");
	gr->PutsFit(mglPoint(0, -1.8), "y = ", "r");
	gr->Puts(mglPoint(0, 2.2), "initial: y = 0.3+sin(2\\pi x)", "b");
//	gr->SetRanges(mglPoint(-1,-1,-1),mglPoint(1,1,1));	gr->SetOrigin(0,0,0);
}
//-----------------------------------------------------------------------------
const char *mmgl_vecta="call 'prepare3v'\nsubplot 2 1 0:title 'Vect3 sample':rotate 50 60\n"
"origin 0 0 0:box:axis '_xyz'\nvect3 ex ey ez 'x':vect3 ex ey ez:vect3 ex ey ez 'z'\n"
"subplot 2 1 1:title '\"f\" style':rotate 50 60\n"
"origin 0 0 0:box:axis '_xyz'\nvect3 ex ey ez 'fx':vect3 ex ey ez 'f':vect3 ex ey ez 'fz'\n"
"grid3 ex 'Wx':grid3 ex 'W':grid3 ex 'Wz'\n";
void smgl_vecta(mglGraph *gr)
{
	mglData ex,ey,ez;	mgls_prepare3v(&ex,&ey,&ez);
	if(!mini)	{	gr->SubPlot(2,1,0);	gr->Title("Vect3 sample");	}
	gr->Rotate(50,60);	gr->SetOrigin(0,0,0);	gr->Axis("_xyz");	gr->Box();
	gr->Vect3(ex,ey,ez,"x");	gr->Vect3(ex,ey,ez);	gr->Vect3(ex,ey,ez,"z");
	gr->SubPlot(2,1,1);	gr->Title("'f' style");
	gr->Rotate(50,60);	gr->SetOrigin(0,0,0);	gr->Axis("_xyz");	gr->Box();
	gr->Vect3(ex,ey,ez,"fx");	gr->Vect3(ex,ey,ez,"f");	gr->Vect3(ex,ey,ez,"fz");
	gr->Grid3(ex,"Wx");	gr->Grid3(ex,"W");	gr->Grid3(ex,"Wz");
}
//-----------------------------------------------------------------------------
const char *mmgl_vect="call 'prepare2v'\ncall 'prepare3v'\nsubplot 3 2 0 '':title 'Vect plot (default)':box:vect a b\n"
"subplot 3 2 1 '':title '\".\" style; \"=\" style':box:vect a b '.='\n"
"subplot 3 2 2 '':title '\"f\" style':box:vect a b 'f'\n"
"subplot 3 2 3 '':title '\">\" style':box:vect a b '>'\n"
"subplot 3 2 4 '':title '\"<\" style':box:vect a b '<'\n"
"subplot 3 2 5:title '3d variant':rotate 50 60:box:vect ex ey ez\n";
void smgl_vect(mglGraph *gr)
{
	mglData a,b;	mgls_prepare2v(&a,&b);
	if(!mini)	{gr->SubPlot(3,2,0,"");	gr->Title("Vect plot (default)");}
	gr->Box();	gr->Vect(a,b);
	if(mini)	return;
	gr->SubPlot(3,2,1,"");	gr->Title("'.' style; '=' style");	gr->Box();	gr->Vect(a,b,"=.");
	gr->SubPlot(3,2,2,"");	gr->Title("'f' style");	gr->Box();	gr->Vect(a,b,"f");
	gr->SubPlot(3,2,3,"");	gr->Title("'>' style");	gr->Box();	gr->Vect(a,b,">");
	gr->SubPlot(3,2,4,"");	gr->Title("'<' style");	gr->Box();	gr->Vect(a,b,"<");
	mglData ex,ey,ez;	mgls_prepare3v(&ex,&ey,&ez);
	gr->SubPlot(3,2,5);	gr->Title("3d variant");	gr->Rotate(50,60);
	gr->Box();	gr->Vect(ex,ey,ez);
}
//-----------------------------------------------------------------------------
const char *mmgl_flow="call 'prepare2v'\ncall 'prepare3v'\nsubplot 2 2 0 '':title 'Flow plot (default)':box:flow a b\n"
"subplot 2 2 1 '':title '\"v\" style':box:flow a b 'v'\n"
"subplot 2 2 2 '':title 'from edges only':box:flow a b '#'\n"
"subplot 2 2 3:title '3d variant':rotate 50 60:box:flow ex ey ez\n";
void smgl_flow(mglGraph *gr)
{
	mglData a,b;	mgls_prepare2v(&a,&b);
	if(!mini)	{gr->SubPlot(2,2,0,"");	gr->Title("Flow plot (default)");}
	gr->Box();	gr->Flow(a,b);
	if(mini)	return;
	gr->SubPlot(2,2,1,"");	gr->Title("'v' style");	gr->Box();	gr->Flow(a,b,"v");
	gr->SubPlot(2,2,2,"");	gr->Title("'\\#' style");	gr->Box();	gr->Flow(a,b,"#");
	mglData ex,ey,ez;	mgls_prepare3v(&ex,&ey,&ez);
	gr->SubPlot(2,2,3);	gr->Title("3d variant");	gr->Rotate(50,60);
	gr->Box();	gr->Flow(ex,ey,ez);
}
//-----------------------------------------------------------------------------
const char *mmgl_pipe="call 'prepare2v'\ncall 'prepare3v'\nsubplot 2 2 0 '':title 'Pipe plot (default)':light on:box:pipe a b\n"
"subplot 2 2 1 '':title '\"i\" style':box:pipe a b 'i'\n"
"subplot 2 2 2 '':title 'from edges only':box:pipe a b '#'\n"
"subplot 2 2 3:title '3d variant':rotate 50 60:box:pipe ex ey ez '' 0.1\n";
void smgl_pipe(mglGraph *gr)
{
	mglData a,b;	mgls_prepare2v(&a,&b);
	if(!mini)	{gr->SubPlot(2,2,0,"");	gr->Title("Pipe plot (default)");}
	gr->Light(true);	gr->Box();	gr->Pipe(a,b);
	if(mini)	return;
	gr->SubPlot(2,2,1,"");	gr->Title("'i' style");	gr->Box();	gr->Pipe(a,b,"i");
	gr->SubPlot(2,2,2,"");	gr->Title("'\\#' style");	gr->Box();	gr->Pipe(a,b,"#");
	mglData ex,ey,ez;	mgls_prepare3v(&ex,&ey,&ez);
	gr->SubPlot(2,2,3);	gr->Title("3d variant");	gr->Rotate(50,60);
	gr->Box();	gr->Pipe(ex,ey,ez,"",0.1);
}
//-----------------------------------------------------------------------------
const char *mmgl_dew="call 'prepare2v'\nsubplot 1 1 0 '':title 'Dew plot':light on:box:dew a b\n";
void smgl_dew(mglGraph *gr)
{
	mglData a,b;	mgls_prepare2v(&a,&b);
	if(!mini)	{gr->SubPlot(1,1,0,"");	gr->Title("Dew plot");}
	gr->Box();	gr->Light(true);	gr->Dew(a,b);
}
//-----------------------------------------------------------------------------
const char *mmgl_grad="call 'prepare2d'\nsubplot 1 1 0 '':title 'Grad plot':box:grad a:dens a '{u8}w{q8}'\n";
void smgl_grad(mglGraph *gr)
{
	mglData a;	mgls_prepare2d(&a);
	if(!mini)	{gr->SubPlot(1,1,0,"");	gr->Title("Grad plot");}
	gr->Box();	gr->Grad(a);	gr->Dens(a,"{u8}w{q8}");
}
//-----------------------------------------------------------------------------
const char *mmgl_cones="new ys 10 3 '0.8*sin(pi*(x+y/4+1.25))+0.2*rnd'\n"
"subplot 2 2 0:title 'Cones plot':rotate 50 60:light on:origin 0 0 0:box:cones ys\n"
"subplot 2 2 1:title '2 colors':rotate 50 60:light on:origin 0 0 0:box:cones ys 'cbgGyr'\n"
"subplot 2 2 2:title '\"\\#\" style':rotate 50 60:light on:origin 0 0 0:box:cones ys '#'\n"
"subplot 2 2 3:title '\"a\" style':rotate 50 60:zrange -2 2:light on:origin 0 0 0:box:cones ys 'a'\n";
void smgl_cones(mglGraph *gr)
{
	mglData ys(10,3);	ys.Modify("0.8*sin(pi*(2*x+y/2))+0.2*rnd");
	gr->Light(true);	gr->SetOrigin(0,0,0);
	if(!mini)	{	gr->SubPlot(2,2,0);	gr->Title("Cones plot");	}
	gr->Rotate(50,60);	gr->Box();	gr->Cones(ys);
	if(mini)	return;
	gr->SubPlot(2,2,1);	gr->Title("2 colors");
	gr->Rotate(50,60);	gr->Box();	gr->Cones(ys,"cbgGyr");
	gr->SubPlot(2,2,2);	gr->Title("'\\#' style");
	gr->Rotate(50,60);	gr->Box();	gr->Cones(ys,"#");
	gr->SubPlot(2,2,3);	gr->Title("'a' style");
	gr->SetRange('z',-2,2);	// increase range since summation can exceed [-1,1]
	gr->Rotate(50,60);	gr->Box();	gr->Cones(ys,"a");
}
//-----------------------------------------------------------------------------
const char *mmgl_aspect="subplot 2 2 0:box:text -1 1.1 'Just box' ':L'\ninplot 0.2 0.5 0.7 1:box:text 0 1.2 'InPlot example'\n"
"subplot 2 2 1:title 'Rotate only':rotate 50 60:box\nsubplot 2 2 2:title 'Rotate and Aspect':rotate 50 60:aspect 1 1 2:box\n"
"subplot 2 2 3:title 'Aspect in other direction':rotate 50 60:aspect 1 2 2:box\n";
void smgl_aspect(mglGraph *gr)	// transformation
{
	gr->SubPlot(2,2,0);	gr->Box();
	gr->Puts(mglPoint(-1,1.1),"Just box",":L");
	gr->InPlot(0.2,0.5,0.7,1,false);	gr->Box();
	gr->Puts(mglPoint(0,1.2),"InPlot example");
	gr->SubPlot(2,2,1);	gr->Title("Rotate only");
	gr->Rotate(50,60);	gr->Box();
	gr->SubPlot(2,2,2);	gr->Title("Rotate and Aspect");
	gr->Rotate(50,60);	gr->Aspect(1,1,2);	gr->Box();
	gr->SubPlot(2,2,3);	gr->Title("Aspect in other direction");
	gr->Rotate(50,60);	gr->Aspect(1,2,2);	gr->Box();
}
//-----------------------------------------------------------------------------
const char *mmgl_inplot="subplot 3 2 0:title 'StickPlot'\nstickplot 3 0 20 30:box 'r':text 0 0 '0' 'r'\n"
"stickplot 3 1 20 30:box 'g':text 0 0 '1' 'g'\nstickplot 3 2 20 30:box 'b':text 0 0 '2' 'b'\n"
"subplot 3 2 3 '':title 'ColumnPlot'\ncolumnplot 3 0:box 'r':text 0 0 '0' 'r'\n"
"columnplot 3 1:box 'g':text 0 0 '1' 'g'\ncolumnplot 3 2:box 'b':text 0 0 '2' 'b'\n"
"subplot 3 2 4 '':title 'GridPlot'\ngridplot 2 2 0:box 'r':text 0 0 '0' 'r'\n"
"gridplot 2 2 1:box 'g':text 0 0 '1' 'g'\ngridplot 2 2 2:box 'b':text 0 0 '2' 'b'\n"
"gridplot 2 2 3:box 'm':text 0 0 '3' 'm'\nsubplot 3 2 5 '':title 'InPlot':box\n"
"inplot 0.4 1 0.6 1 on:box 'r'\nmultiplot 3 2 1 2 1 '':title 'MultiPlot':box\n";
void smgl_inplot(mglGraph *gr)
{
	gr->SubPlot(3,2,0);	gr->Title("StickPlot");
	gr->StickPlot(3, 0, 20, 30);	gr->Box("r");	gr->Puts(mglPoint(0),"0","r");
	gr->StickPlot(3, 1, 20, 30);	gr->Box("g");	gr->Puts(mglPoint(0),"1","g");
	gr->StickPlot(3, 2, 20, 30);	gr->Box("b");	gr->Puts(mglPoint(0),"2","b");
	gr->SubPlot(3,2,3,"");	gr->Title("ColumnPlot");
	gr->ColumnPlot(3, 0);	gr->Box("r");	gr->Puts(mglPoint(0),"0","r");
	gr->ColumnPlot(3, 1);	gr->Box("g");	gr->Puts(mglPoint(0),"1","g");
	gr->ColumnPlot(3, 2);	gr->Box("b");	gr->Puts(mglPoint(0),"2","b");
	gr->SubPlot(3,2,4,"");	gr->Title("GridPlot");
	gr->GridPlot(2, 2, 0);	gr->Box("r");	gr->Puts(mglPoint(0),"0","r");
	gr->GridPlot(2, 2, 1);	gr->Box("g");	gr->Puts(mglPoint(0),"1","g");
	gr->GridPlot(2, 2, 2);	gr->Box("b");	gr->Puts(mglPoint(0),"2","b");
	gr->GridPlot(2, 2, 3);	gr->Box("m");	gr->Puts(mglPoint(0),"3","m");
	gr->SubPlot(3,2,5,"");	gr->Title("InPlot");	gr->Box();
	gr->InPlot(0.4, 1, 0.6, 1, true);	gr->Box("r");
	gr->MultiPlot(3,2,1, 2, 1,"");	gr->Title("MultiPlot");	gr->Box();
}
//-----------------------------------------------------------------------------
const char *mmgl_combined="call 'prepare2v'\ncall 'prepare3d'\nnew v 10:fill v -0.5 1:copy d sqrt(a^2+b^2)\n"
"subplot 2 2 0:title 'Surf + Cont':rotate 50 60:light on:box:surf a:cont a 'y'\n"
"subplot 2 2 1 '':title 'Flow + Dens':light off:box:flow a b 'br':dens d\n"
"subplot 2 2 2:title 'Mesh + Cont':rotate 50 60:box:mesh a:cont a '_'\n"
"subplot 2 2 3:title 'Surf3 + ContF3':rotate 50 60:light on\n"
"box:contf3 v c 'z' 0:contf3 v c 'x':contf3 v c\ncut 0 -1 -1 1 0 1.1\ncontf3 v c 'z' c.nz-1:surf3 c -0.5\n";
void smgl_combined(mglGraph *gr)	// flow threads and density plot
{
	mglData a,b,d;	mgls_prepare2v(&a,&b);	d = a;
	for(int i=0;i<a.nx*a.ny;i++)	d.a[i] = hypot(a.a[i],b.a[i]);
	mglData c;	mgls_prepare3d(&c);
	mglData v(10);	v.Fill(-0.5,1);
	gr->SubPlot(2,2,1,"");	gr->Title("Flow + Dens");
	gr->Flow(a,b,"br");	gr->Dens(d,"BbcyrR");	gr->Box();
	gr->SubPlot(2,2,0);	gr->Title("Surf + Cont");	gr->Rotate(50,60);
	gr->Light(true);	gr->Surf(a);	gr->Cont(a,"y");	gr->Box();
	gr->SubPlot(2,2,2);	gr->Title("Mesh + Cont");	gr->Rotate(50,60);
	gr->Box();	gr->Mesh(a);	gr->Cont(a,"_");
	gr->SubPlot(2,2,3);	gr->Title("Surf3 + ContF3");gr->Rotate(50,60);
	gr->Box();	gr->ContF3(v,c,"z",0);	gr->ContF3(v,c,"x");	gr->ContF3(v,c);
	gr->SetCutBox(mglPoint(0,-1,-1), mglPoint(1,0,1.1));
	gr->ContF3(v,c,"z",c.nz-1);	gr->Surf3(-0.5,c);
}
//-----------------------------------------------------------------------------
const char *mmgl_axis="subplot 2 2 0:title 'Axis origin, Grid':origin 0 0:axis:grid:fplot 'x^3'\n"
"subplot 2 2 1:title '2 axis':ranges -1 1 -1 1:origin -1 -1:axis:ylabel 'axis_1':fplot 'sin(pi*x)' 'r2'\n"
"ranges 0 1 0 1:origin 1 1:axis:ylabel 'axis_2':fplot 'cos(pi*x)'\n"
"subplot 2 2 3:title 'More axis':origin nan nan:xrange -1 1:axis:xlabel 'x' 0:ylabel 'y_1' 0:fplot 'x^2' 'k'\n"
"yrange -1 1:origin -1.3 -1:axis 'y' 'r':ylabel '#r{y_2}' 0.2:fplot 'x^3' 'r'\n\n"
"subplot 2 2 2:title '4 segments, inverted axis':origin 0 0:\n"
"inplot 0.5 1 0.5 1 on:ranges 0 10 0 2:axis\nfplot 'sqrt(x/2)':xlabel 'W' 1:ylabel 'U' 1\n"
"inplot 0 0.5 0.5 1 on:ranges 1 0 0 2:axis 'x':fplot 'sqrt(x)+x^3':xlabel '\\tau' 1\n"
"inplot 0.5 1 0 0.5 on:ranges 0 10 4 0:axis 'y':fplot 'x/4':ylabel 'L' -1\n"
"inplot 0 0.5 0 0.5 on:ranges 1 0 4 0:fplot '4*x^2'\n";
void smgl_axis(mglGraph *gr)
{
	gr->SubPlot(2,2,0);	gr->Title("Axis origin, Grid");	gr->SetOrigin(0,0);
	gr->Axis();	gr->Grid();	gr->FPlot("x^3");

	gr->SubPlot(2,2,1);	gr->Title("2 axis");
	gr->SetRanges(-1,1,-1,1);	gr->SetOrigin(-1,-1,-1);	// first axis
	gr->Axis();	gr->Label('y',"axis 1",0);	gr->FPlot("sin(pi*x)","r2");
	gr->SetRanges(0,1,0,1);		gr->SetOrigin(1,1,1);		// second axis
	gr->Axis();	gr->Label('y',"axis 2",0);	gr->FPlot("cos(pi*x)");

	gr->SubPlot(2,2,3);	gr->Title("More axis");	gr->SetOrigin(NAN,NAN);	gr->SetRange('x',-1,1);
	gr->Axis();	gr->Label('x',"x",0);	gr->Label('y',"y_1",0);	gr->FPlot("x^2","k");
	gr->SetRanges(-1,1,-1,1);	gr->SetOrigin(-1.3,-1);	// second axis
	gr->Axis("y","r");	gr->Label('y',"#r{y_2}",0.2);	gr->FPlot("x^3","r");

	gr->SubPlot(2,2,2);	gr->Title("4 segments, inverted axis");		gr->SetOrigin(0,0);
	gr->InPlot(0.5,1,0.5,1);	gr->SetRanges(0,10,0,2);	gr->Axis();
	gr->FPlot("sqrt(x/2)");		gr->Label('x',"W",1);	gr->Label('y',"U",1);
	gr->InPlot(0,0.5,0.5,1);	gr->SetRanges(1,0,0,2);	gr->Axis("x");
	gr->FPlot("sqrt(x)+x^3");	gr->Label('x',"\\tau",-1);
	gr->InPlot(0.5,1,0,0.5);	gr->SetRanges(0,10,4,0);	gr->Axis("y");
	gr->FPlot("x/4");	gr->Label('y',"L",-1);
	gr->InPlot(0,0.5,0,0.5);	gr->SetRanges(1,0,4,0);	gr->FPlot("4*x^2");
}
//-----------------------------------------------------------------------------
const char *mmgl_ticks="subplot 3 2 0:title 'Usual axis':axis\n"
"subplot 3 2 1:title 'Too big/small range':ranges -1000 1000 0 0.001:axis\n"
"subplot 3 2 3:title 'Too narrow range':ranges 100 100.1 10 10.01:axis\n"
"subplot 3 2 4:title 'Disable ticks tuning':tuneticks off:axis\n"
"subplot 3 2 2:title 'Manual ticks':ranges -pi pi 0 2:\n"
"xtick -pi '\\pi' -pi/2 '-\\pi/2' 0 '0' 0.886 'x^*' pi/2 '\\pi/2' pi 'pi'\n"
"# or you can use:\n#list v -pi -pi/2 0 0.886 pi/2 pi:xtick v '-\\pi\\n-\\pi/2\\n{}0\\n{}x^*\\n\\pi/2\\n\\pi'\n"
"axis:grid:fplot '2*cos(x^2)^2' 'r2'\n"
"subplot 3 2 5:title 'Time ticks':xrange 0 3e5:ticktime 'x':axis\n";
void smgl_ticks(mglGraph *gr)
{
	gr->SubPlot(3,2,0);	gr->Title("Usual axis");	gr->Axis();
	gr->SubPlot(3,2,1);	gr->Title("Too big/small range");
	gr->SetRanges(-1000,1000,0,0.001);	gr->Axis();
	gr->SubPlot(3,2,3);	gr->Title("Too narrow range");
	gr->SetRanges(100,100.1,10,10.01);	gr->Axis();
	gr->SubPlot(3,2,4);	gr->Title("Disable ticks tuning");
	gr->SetTuneTicks(0);	gr->Axis();

	gr->SubPlot(3,2,2);	gr->Title("Manual ticks");	gr->SetRanges(-M_PI,M_PI, 0, 2);
	double val[]={-M_PI, -M_PI/2, 0, 0.886, M_PI/2, M_PI};
	gr->SetTicksVal('x', mglData(6,val), "-\\pi\n-\\pi/2\n0\nx^*\n\\pi/2\n\\pi");
	gr->Axis();	gr->Grid();	gr->FPlot("2*cos(x^2)^2", "r2");

	gr->SubPlot(3,2,5);	gr->Title("Time ticks");	gr->SetRange('x',0,3e5);
	gr->SetTicksTime('x',0);	gr->Axis();
}
//-----------------------------------------------------------------------------
const char *mmgl_box="subplot 2 2 0:title 'Box (default)':rotate 50 60:box\n"
"subplot 2 2 1:title 'colored':rotate 50 60:box 'r'\n"
"subplot 2 2 2:title 'with faces':rotate 50 60:box '@'\n"
"subplot 2 2 3:title 'both':rotate 50 60:box '@cm'\n";
void smgl_box(mglGraph *gr)
{
	gr->SubPlot(2,2,0);	gr->Title("Box (default)");	gr->Rotate(50,60);	gr->Box();
	gr->SubPlot(2,2,1);	gr->Title("colored");		gr->Rotate(50,60);	gr->Box("r");
	gr->SubPlot(2,2,2);	gr->Title("with faces");	gr->Rotate(50,60);	gr->Box("@");
	gr->SubPlot(2,2,3);	gr->Title("both");	gr->Rotate(50,60);	gr->Box("@cm");
}
//-----------------------------------------------------------------------------
const char *mmgl_loglog="subplot 2 2 0 '<_':title 'Semi-log axis':ranges 0.01 100 -1 1:axis 'lg(x)' '' ''\n"
"axis:grid 'xy' 'g':fplot 'sin(1/x)':xlabel 'x' 0:ylabel 'y = sin 1/x' 0\n"
"subplot 2 2 1 '<_':title 'Log-log axis':ranges 0.01 100 0.1 100:axis 'lg(x)' 'lg(y)' ''\n"
"axis:fplot 'sqrt(1+x^2)':xlabel 'x' 0:ylabel 'y = \\sqrt{1+x^2}' 0\n"
"subplot 2 2 2 '<_':title 'Minus-log axis':ranges -100 -0.01 -100 -0.1:axis '-lg(-x)' '-lg(-y)' ''\n"
"axis:fplot '-sqrt(1+x^2)':xlabel 'x' 0:ylabel 'y = -\\sqrt{1+x^2}' 0\n"
"subplot 2 2 3 '<_':title 'Log-ticks':ranges 0.01 100 0 100:axis 'sqrt(x)' '' ''\n"
"axis:fplot 'x':xlabel 'x' 1:ylabel 'y = x' 0\n";
void smgl_loglog(mglGraph *gr)	// log-log axis
{
	gr->SubPlot(2,2,0,"<_");	gr->Title("Semi-log axis");	gr->SetRanges(0.01,100,-1,1);	gr->SetFunc("lg(x)","");
	gr->Axis();	gr->Grid("xy","g");	gr->FPlot("sin(1/x)");	gr->Label('x',"x",0); gr->Label('y', "y = sin 1/x",0);
	gr->SubPlot(2,2,1,"<_");	gr->Title("Log-log axis");	gr->SetRanges(0.01,100,0.1,100);	gr->SetFunc("lg(x)","lg(y)");
	gr->Axis();	gr->FPlot("sqrt(1+x^2)");	gr->Label('x',"x",0); gr->Label('y', "y = \\sqrt{1+x^2}",0);
	gr->SubPlot(2,2,2,"<_");	gr->Title("Minus-log axis");	gr->SetRanges(-100,-0.01,-100,-0.1);	gr->SetFunc("-lg(-x)","-lg(-y)");
	gr->Axis();	gr->FPlot("-sqrt(1+x^2)");	gr->Label('x',"x",0); gr->Label('y', "y = -\\sqrt{1+x^2}",0);
	gr->SubPlot(2,2,3,"<_");	gr->Title("Log-ticks");	gr->SetRanges(0.1,100,0,100);	gr->SetFunc("sqrt(x)","");
	gr->Axis();	gr->FPlot("x");	gr->Label('x',"x",1); gr->Label('y', "y = x",0);
}
//-----------------------------------------------------------------------------
const char *mmgl_venn="list x -0.3 0 0.3:list y 0.3 -0.3 0.3:list e 0.7 0.7 0.7\n"
"subplot 1 1 0:title 'Venn-like diagram':alpha on:error x y e e '!rgb@#o'";
void smgl_venn(mglGraph *gr)
{
	double xx[3]={-0.3,0,0.3}, yy[3]={0.3,-0.3,0.3}, ee[3]={0.7,0.7,0.7};
	mglData x(3,xx), y(3,yy), e(3,ee);
	gr->SubPlot(1,1,0);	gr->Title("Venn-like diagram");	gr->Alpha(true);	gr->Error(x,y,e,e,"!rgb@#o");
}
//-----------------------------------------------------------------------------
const char *mmgl_stereo="call 'prepare2d'\nlight on\nsubplot 2 1 0:rotate 50 60+1:box:surf a\nsubplot 2 1 1:rotate 50 60-1:box:surf a\n";
void smgl_stereo(mglGraph *gr)
{
	mglData a;	mgls_prepare2d(&a);
	gr->Light(true);
	gr->SubPlot(2,1,0);	gr->Rotate(50,60+1);
	gr->Box();	gr->Surf(a);
	gr->SubPlot(2,1,1);	gr->Rotate(50,60-1);
	gr->Box();	gr->Surf(a);
}
//-----------------------------------------------------------------------------
const char *mmgl_hist="new x 10000 '2*rnd-1':new y 10000 '2*rnd-1':copy z exp(-6*(x^2+y^2))\n"
"hist xx x z:norm xx 0 1:hist yy y z:norm yy 0 1\nmultiplot 3 3 3 2 2 '':ranges -1 1 -1 1 0 1:box:dots x y z 'wyrRk'\n"
"multiplot 3 3 0 2 1 '':ranges -1 1 0 1:box:bars xx\nmultiplot 3 3 5 1 2 '':ranges 0 1 -1 1:box:barh yy\n"
"subplot 3 3 2:text 0.5 0.5 'Hist and\\n{}MultiPlot\\n{}sample' 'a' -3\n";
void smgl_hist(mglGraph *gr)
{
	mglData x(10000), y(10000), z(10000);	gr->Fill(x,"2*rnd-1");	gr->Fill(y,"2*rnd-1");	gr->Fill(z,"exp(-6*(v^2+w^2))",x,y);
	mglData xx=gr->Hist(x,z), yy=gr->Hist(y,z);	xx.Norm(0,1);	yy.Norm(0,1);
	gr->MultiPlot(3,3,3,2,2,"");	gr->SetRanges(-1,1,-1,1,0,1);	gr->Box();	gr->Dots(x,y,z,"wyrRk");
	gr->MultiPlot(3,3,0,2,1,"");	gr->SetRanges(-1,1,0,1);	gr->Box();	gr->Bars(xx);
	gr->MultiPlot(3,3,5,1,2,"");	gr->SetRanges(0,1,-1,1);	gr->Box();	gr->Barh(yy);
	gr->SubPlot(3,3,2);		gr->Puts(mglPoint(0.5,0.5),"Hist and\nMultiPlot\nsample","a",-3);
}
//-----------------------------------------------------------------------------
const char *mmgl_primitives="subplot 2 2 0 '':title 'Line, Curve, Rhomb, Ellipse' '' -1.5\n"
"line -1 -1 -0.5 1 'qAI'\ncurve -0.6 -1 1 1 0 1 1 1 'rA'\nball 0 -0.5 '*':ball 1 -0.1 '*'\n"
"rhomb 0 0.4 1 0.9 0.2 'b#'\nrhomb 0 0 1 0.4 0.2 'cg@'\n"
"ellipse 0 -0.5 1 -0.1 0.2 'u#'\nellipse 0 -1 1 -0.6 0.2 'm@'\n\n"
"light on\nsubplot 2 2 1:title 'Face[xyz]':rotate 50 60:box\n"
"facex 1 0 -1 1 1 'r':facey -1 -1 -1 1 1 'g':facez 1 -1 -1 -1 1 'b'\n"
"face -1 -1 1 -1 1 1 1 -1 0 1 1 1 'm'\n\n"
"subplot 2 2 3 '':title 'Cone'\n"
"cone -0.7 -0.3 0 -0.7 0.7 0.5 0.2 0.1 'b':text -0.7 -0.7 'no edges\\n(default)'\n"
"cone 0 -0.3 0 0 0.7 0.5 0.2 0.1 'g@':text 0 -0.7 'with edges\\n('\\@' style)'\n"
"cone 0.7 -0.3 0 0.7 0.7 0.5 0.2 0.1 'ry':text 0.7 -0.7 '\"arrow\" with\\n{}gradient'\n\n"
"subplot 2 2 2 '':title 'Sphere and Drop'\nline -0.9 0 1 0.9 0 1\n"
"text -0.9 0.4 'sh=0':drop -0.9 0 0 1 0.5 'r' 0:ball -0.9 0 1 'k'n"
"text -0.3 0.6 'sh=0.33':drop -0.3 0 0 1 0.5 'r' 0.33:ball -0.3 0 1 'k'n"
"text 0.3 0.8 'sh=0.67':drop 0.3 0 0 1 0.5 'r' 0.67:ball 0.3 0 1 'k'n"
"text 0.9 1. 'sh=1':drop 0.9 0 0 1 0.5 'r' 1:ball 0.9 0 1 'k'\n\n"
"text -0.9 -1.1 'asp=0.33':drop -0.9 -0.7 0 1 0.5 'b' 0 0.33\n"
"text -0.3 -1.1 'asp=0.67':drop -0.3 -0.7 0 1 0.5 'b' 0 0.67\n"
"text 0.3 -1.1 'asp=1':drop 0.3 -0.7 0 1 0.5 'b' 0 1\n"
"text 0.9 -1.1 'asp=1.5':drop 0.9 -0.7 0 1 0.5 'b' 0 1.5\n";
void smgl_primitives(mglGraph *gr)	// flag #
{
	gr->SubPlot(2,2,0,"");	gr->Title("Line, Curve, Rhomb, Ellipse","",-1.5);
	gr->Line(mglPoint(-1,-1),mglPoint(-0.5,1),"qAI");
	gr->Curve(mglPoint(-0.6,-1),mglPoint(1,1),mglPoint(0,1),mglPoint(1,1),"rA");
	gr->Rhomb(mglPoint(0,0.4),mglPoint(1,0.9),0.2,"b#");
	gr->Rhomb(mglPoint(0,0),mglPoint(1,0.4),0.2,"cg@");
	gr->Ellipse(mglPoint(0,-0.5),mglPoint(1,-0.1),0.2,"u#");
	gr->Ellipse(mglPoint(0,-1),mglPoint(1,-0.6),0.2,"m@");
	gr->Mark(mglPoint(0,-0.5),"*");	gr->Mark(mglPoint(1,-0.1),"*");

	gr->Light(true);
	gr->SubPlot(2,2,1);	gr->Title("Face[xyz]");	gr->Rotate(50,60);	gr->Box();
	gr->FaceX(mglPoint(1,0,-1),1,1,"r");
	gr->FaceY(mglPoint(-1,-1,-1),1,1,"g");
	gr->FaceZ(mglPoint(1,-1,-1),-1,1,"b");
	gr->Face(mglPoint(-1,-1,1),mglPoint(-1,1,1),mglPoint(1,-1,0),mglPoint(1,1,1),"m");

	gr->SubPlot(2,2,3,"");	gr->Title("Cone");
	gr->Cone(mglPoint(-0.7,-0.3),mglPoint(-0.7,0.7,0.5),0.2,0.1,"b");
	gr->Puts(mglPoint(-0.7,-0.7),"no edges\n(default)");
	gr->Cone(mglPoint(0,-0.3),mglPoint(0,0.7,0.5),0.2,0.1,"g@");
	gr->Puts(mglPoint(0,-0.7),"with edges\n('\\@' style)");
	gr->Cone(mglPoint(0.7,-0.3),mglPoint(0.7,0.7,0.5),0.2,0,"ry");
	gr->Puts(mglPoint(0.7,-0.7),"'arrow' with\ngradient");

	gr->SubPlot(2,2,2,"");	gr->Title("Sphere and Drop");	gr->Alpha(false);
	gr->Puts(mglPoint(-0.9,0.4),"sh=0");		gr->Ball(mglPoint(-0.9,0,1),'k');
	gr->Drop(mglPoint(-0.9,0),mglPoint(0,1),0.5,"r",0);
	gr->Puts(mglPoint(-0.3,0.6),"sh=0.33");	gr->Ball(mglPoint(-0.3,0,1),'k');
	gr->Drop(mglPoint(-0.3,0),mglPoint(0,1),0.5,"r",0.33);
	gr->Puts(mglPoint(0.3,0.8),"sh=0.67");		gr->Ball(mglPoint(0.3,0,1),'k');
	gr->Drop(mglPoint(0.3,0),mglPoint(0,1),0.5,"r",0.67);
	gr->Puts(mglPoint(0.9,1),"sh=1");			gr->Ball(mglPoint(0.9,0,1),'k');
	gr->Drop(mglPoint(0.9,0),mglPoint(0,1),0.5,"r",1);
	gr->Line(mglPoint(-0.9,0,1),mglPoint(0.9,0,1),"b");
	
	gr->Puts(mglPoint(-0.9,-1.1),"asp=0.33");
	gr->Drop(mglPoint(-0.9,-0.7),mglPoint(0,1),0.5,"b",0,0.33);
	gr->Puts(mglPoint(-0.3,-1.1),"asp=0.67");
	gr->Drop(mglPoint(-0.3,-0.7),mglPoint(0,1),0.5,"b",0,0.67);
	gr->Puts(mglPoint(0.3,-1.1),"asp=1");
	gr->Drop(mglPoint(0.3,-0.7),mglPoint(0,1),0.5,"b",0,1);
	gr->Puts(mglPoint(0.9,-1.1),"asp=1.5");
	gr->Drop(mglPoint(0.9,-0.7),mglPoint(0,1),0.5,"b",0,1.5);
}
//-----------------------------------------------------------------------------
const char *mmgl_table="new ys 10 3 '0.8*sin(pi*(x+y/4+1.25))+0.2*rnd'\n"
"subplot 2 2 0:title 'Table sample':box\ntable ys 'y_1\\n{}y_2\\n{}y_3'\n\n"
"subplot 2 2 1:title 'no borders, colored'\ntable ys 'y_1\\n{}y_2\\n{}y_3' 'r|'\n\n"
"subplot 2 2 2:title 'no font decrease'\ntable ys 'y_1\\n{}y_2\\n{}y_3' '#'\n\n"
"subplot 2 2 3:title 'manual width and position':box\n"
"table 0.5 0.95 ys 'y_1\\n{}y_2\\n{}y_3' '#';value 0.7\n";
void smgl_table(mglGraph *gr)
{
	mglData ys(10,3);	ys.Modify("0.8*sin(pi*(2*x+y/2))+0.2*rnd");
	if(!mini)	{	gr->SubPlot(2,2,0);	gr->Title("Table plot");	}
	gr->Table(ys,"y_1\ny_2\ny_3");	gr->Box();
	if(mini)	return;
	gr->SubPlot(2,2,1);	gr->Title("no borders, colored");
	gr->Table(ys,"y_1\ny_2\ny_3","r|");
	gr->SubPlot(2,2,2);	gr->Title("no font decrease");
	gr->Table(ys,"y_1\ny_2\ny_3","#");
	gr->SubPlot(2,2,3);	gr->Title("manual width, position");
	gr->Table(0.5, 0.95, ys,"y_1\ny_2\ny_3","#", "value 0.7");	gr->Box();
}
//-----------------------------------------------------------------------------
const char *mmgl_label="new ys 10 '0.2*rnd-0.8*sin(pi*x)'\n"
"subplot 1 1 0 '':title 'Label plot':box:plot ys ' *':label ys 'y=%y'\n";
void smgl_label(mglGraph *gr)
{
	mglData ys(10);	ys.Modify("0.8*sin(pi*2*x)+0.2*rnd");
	if(!mini)	{	gr->SubPlot(1,1,0,"");	gr->Title("Label plot");	}
	gr->Box();	gr->Plot(ys," *");	gr->Label(ys,"y=%y");
}
//-----------------------------------------------------------------------------
const char *mmgl_colorbar="call 'prepare2d'\nnew v 9 'x'\nsubplot 2 2 0:title 'Colorbar out of box':box\n"
"colorbar '<':colorbar '>':colorbar '_':colorbar '^'\n"
"subplot 2 2 1:title 'Colorbar near box':box\n"
"colorbar '<I':colorbar '>I':colorbar '_I':colorbar '^I'\n"
"subplot 2 2 2:title 'manual colors':box:contd v a\n"
"colorbar v '<':colorbar v '>':colorbar v '_':colorbar v '^'\n"
"subplot 2 2 3:title '':text -0.5 1.55 'Color positions' ':C' -2\n"
"colorbar 'bwr>' 0.25 0:text -0.9 1.2 'Default'\n"
"colorbar 'b{w,0.3}r>' 0.5 0:text -0.1 1.2 'Manual'\ncrange 0.01 1e3\n"
"colorbar '>' 0.75 0:text 0.65 1.2 'Normal scale':colorbar '>':text 1.35 1.2 'Log scale'\n";
void smgl_colorbar(mglGraph *gr)
{
	gr->SubPlot(2,2,0);	gr->Title("Colorbar out of box");	gr->Box();
	gr->Colorbar("<");	gr->Colorbar(">");	gr->Colorbar("_");	gr->Colorbar("^");
	gr->SubPlot(2,2,1);	gr->Title("Colorbar near box");		gr->Box();
	gr->Colorbar("<I");	gr->Colorbar(">I");	gr->Colorbar("_I");	gr->Colorbar("^I");
	gr->SubPlot(2,2,2);	gr->Title("manual colors");
	mglData a,v;	mgls_prepare2d(&a,0,&v);
	gr->Box();	gr->ContD(v,a);
	gr->Colorbar(v,"<");	gr->Colorbar(v,">");	gr->Colorbar(v,"_");	gr->Colorbar(v,"^");
	
	gr->SubPlot(2,2,3);	gr->Title(" ");
	gr->Puts(mglPoint(-0.5,1.55),"Color positions",":C",-2);
	gr->Colorbar("bwr>",0.25,0);	gr->Puts(mglPoint(-0.9,1.2),"Default");
	gr->Colorbar("b{w,0.3}r>",0.5,0);	gr->Puts(mglPoint(-0.1,1.2),"Manual");
	
	gr->Puts(mglPoint(1,1.55),"log-scale",":C",-2);
	gr->SetRange('c',0.01,1e3);
	gr->Colorbar(">",0.75,0);	gr->Puts(mglPoint(0.65,1.2),"Normal scale");
	gr->SetFunc("","","","lg(c)");
	gr->Colorbar(">");		gr->Puts(mglPoint(1.35,1.2),"Log scale");
}
//-----------------------------------------------------------------------------
const char *mmgl_legend="addlegend 'sin(\\pi {x^2})' 'b':addlegend 'sin(\\pi x)' 'g*'\n"
"addlegend 'sin(\\pi \\sqrt{x})' 'rd':addlegend 'jsut text' ' ':addlegend 'no indent for this' ''\n"
"subplot 2 2 0 '':title 'Legend (default)':box:legend\n"
"legend 3 'A#':text 0.75 0.65 'Absolute position' 'A'\n"
"subplot 2 2 2 '':title 'coloring':box:legend 0 'r#':legend 1 'Wb#':legend 2 'ygr#'\n"
"subplot 2 2 3 '':title 'manual position':box:legend 0.5 0.5\n";
void smgl_legend(mglGraph *gr)
{
	gr->AddLegend("sin(\\pi {x^2})","b");
	gr->AddLegend("sin(\\pi x)","g*");
	gr->AddLegend("sin(\\pi \\sqrt{x})","rd");
	gr->AddLegend("just text"," ");
	gr->AddLegend("no indent for this","");
	if(!mini)	{gr->SubPlot(2,2,0,"");	gr->Title("Legend (default)");}
	gr->Box();	gr->Legend();
	if(mini)	return;
	gr->Legend(3,"A#");
	gr->Puts(mglPoint(0.75,0.65),"Absolute position","A");
	gr->SubPlot(2,2,2,"");	gr->Title("coloring");	gr->Box();
	gr->Legend(0,"r#");	gr->Legend(1,"Wb#");	gr->Legend(2,"ygr#");
	gr->SubPlot(2,2,3,"");	gr->Title("manual position");	gr->Box();	gr->Legend(0.5,0.5);
}
//-----------------------------------------------------------------------------
const char *mmgl_dat_diff="ranges 0 1 0 1 0 1:new a 30 40 'x*y'\n"
"subplot 2 2 0:title 'a(x,y)':rotate 60 40:surf a:box\n"
"subplot 2 2 1:title 'da/dx':rotate 60 40:diff a 'x':surf a:box\n"
"subplot 2 2 2:title '\\int da/dx dxdy':rotate 60 40:integrate a 'xy':surf a:box\n"
"subplot 2 2 3:title '\\int {d^2}a/dxdy dx':rotate 60 40:diff2 a 'y':surf a:box\n";
void smgl_dat_diff(mglGraph *gr)	// differentiate
{
	gr->SetRanges(0,1,0,1,0,1);
	mglData a(30,40);	a.Modify("x*y");
	gr->SubPlot(2,2,0);	gr->Title("a(x,y)");	gr->Rotate(60,40);
	gr->Surf(a);		gr->Box();
	gr->SubPlot(2,2,1);	gr->Title("da/dx");		gr->Rotate(60,40);
	a.Diff("x");		gr->Surf(a);	gr->Box();
	gr->SubPlot(2,2,2);	gr->Title("\\int da/dx dxdy");	gr->Rotate(60,40);
	a.Integral("xy");	gr->Surf(a);	gr->Box();
	gr->SubPlot(2,2,3);	gr->Title("\\int {d^2}a/dxdy dx");	gr->Rotate(60,40);
	a.Diff2("y");	gr->Surf(a);	gr->Box();
}
//-----------------------------------------------------------------------------
const char *mmgl_dat_extra="subplot 2 2 0 '':title 'Envelop sample':new d1 1000 'exp(-8*x^2)*sin(10*pi*x)'\n"
"axis:plot d1 'b':envelop d1 'x':plot d1 'r'\n"
"subplot 2 2 1 '':title 'Smooth sample':ranges 0 1 0 1\nnew y0 30 '0.4*sin(pi*x) + 0.3*cos(1.5*pi*x) - 0.4*sin(2*pi*x)+0.5*rnd'\n"
"copy y1 y0:smooth y1 'x3':plot y1 'r';legend '\"3\" style'\ncopy y2 y0:smooth y2 'x5':plot y2 'g';legend '\"5\" style'\n"
"copy y3 y0:smooth y3 'x':plot y3 'b';legend 'default'\nplot y0 '{m7}:s';legend 'none'\nlegend:box\n"
"subplot 2 2 2:title 'Sew sample':rotate 50 60:light on:alpha on\nnew d2 100 100 'mod((y^2-(1-x)^2)/2,0.1)'\n"
"box:surf d2 'b':sew d2 'xy' 0.1:surf d2 'r'\n"
"subplot 2 2 3:title 'Resize sample (interpolation)'\nnew x0 10 'rnd':new v0 10 'rnd'\n"
"resize x1 x0 100:resize v1 v0 100\nplot x0 v0 'b+ ':plot x1 v1 'r-':label x0 v0 '%n'\n";
void smgl_dat_extra(mglGraph *gr)	// differentiate
{
	gr->SubPlot(2,2,0,"");	gr->Title("Envelop sample");
	mglData d1(1000);	gr->Fill(d1,"exp(-8*x^2)*sin(10*pi*x)");
	gr->Axis();			gr->Plot(d1, "b");
	d1.Envelop('x');	gr->Plot(d1, "r");

	gr->SubPlot(2,2,1,"");	gr->Title("Smooth sample");
	mglData y0(30),y1,y2,y3;
	gr->SetRanges(0,1,0,1);
	gr->Fill(y0, "0.4*sin(pi*x) + 0.3*cos(1.5*pi*x) - 0.4*sin(2*pi*x)+0.5*rnd");

	y1=y0;	y1.Smooth("x3");
	y2=y0;	y2.Smooth("x5");
	y3=y0;	y3.Smooth("x");

	gr->Plot(y0,"{m7}:s", "legend 'none'");	//gr->AddLegend("none","k");
	gr->Plot(y1,"r", "legend ''3' style'");
	gr->Plot(y2,"g", "legend ''5' style'");
	gr->Plot(y3,"b", "legend 'default'");
	gr->Legend();		gr->Box();

	gr->SubPlot(2,2,2);		gr->Title("Sew sample");
	mglData d2(100, 100);	gr->Fill(d2, "mod((y^2-(1-x)^2)/2,0.1)");
	gr->Rotate(50, 60);	gr->Light(true);	gr->Alpha(true);
	gr->Box();			gr->Surf(d2, "b");
	d2.Sew("xy", 0.1);	gr->Surf(d2, "r");

	gr->SubPlot(2,2,3);		gr->Title("Resize sample (interpolation)");
	mglData x0(10), v0(10), x1, v1;
	gr->Fill(x0,"rnd");		gr->Fill(v0,"rnd");
	x1 = x0.Resize(100);	v1 = v0.Resize(100);
	gr->Plot(x0,v0,"b+ ");	gr->Plot(x1,v1,"r-");
	gr->Label(x0,v0,"%n");
}
//-----------------------------------------------------------------------------
const char *mmgl_ternary="ranges 0 1 0 1 0 1\nnew x 50 '0.25*(1+cos(2*pi*x))'\n"
"new y 50 '0.25*(1+sin(2*pi*x))'\nnew z 50 'x'\nnew a 20 30 '30*x*y*(1-x-y)^2*(x+y<1)'\n"
"new rx 10 'rnd':new ry 10:fill ry '(1-v)*rnd' rx\nlight on\n\n"
"subplot 2 2 0:title 'Ordinary axis 3D':rotate 50 60\nbox:axis:grid\n"
"plot x y z 'r2':surf a '#'\nxlabel 'B':ylabel 'C':zlabel 'Z'\n\n"
"subplot 2 2 1:title 'Ternary axis (x+y+t=1)':ternary 1\nbox:axis:grid 'xyz' 'B;'\n"
"plot x y 'r2':plot rx ry 'q^ ':cont a:line 0.5 0 0 0.75 'g2'\nxlabel 'B':ylabel 'C':tlabel 'A'\n\n"
"subplot 2 2 2:title 'Quaternary axis 3D':rotate 50 60:ternary 2\nbox:axis:grid 'xyz' 'B;'\n"
"plot x y z 'r2':surf a '#'\nxlabel 'B':ylabel 'C':tlabel 'A':zlabel 'D'\n\n"
"subplot 2 2 3:title 'Ternary axis 3D':rotate 50 60:ternary 1\nbox:axis:grid 'xyz' 'B;'\n"
"plot x y z 'r2':surf a '#'\nxlabel 'B':ylabel 'C':tlabel 'A':zlabel 'Z'\n";
void smgl_ternary(mglGraph *gr)	// flag #
{
	gr->SetRanges(0,1,0,1,0,1);
	mglData x(50),y(50),z(50),rx(10),ry(10), a(20,30);
	a.Modify("30*x*y*(1-x-y)^2*(x+y<1)");
	x.Modify("0.25*(1+cos(2*pi*x))");
	y.Modify("0.25*(1+sin(2*pi*x))");
	rx.Modify("rnd"); ry.Modify("(1-v)*rnd",rx);
	z.Modify("x");

	gr->SubPlot(2,2,0);	gr->Title("Ordinary axis 3D");
	gr->Rotate(50,60);		gr->Light(true);
	gr->Plot(x,y,z,"r2");	gr->Surf(a,"BbcyrR#");
	gr->Axis(); gr->Grid();	gr->Box();
	gr->Label('x',"B",1);	gr->Label('y',"C",1);	gr->Label('z',"Z",1);

	gr->SubPlot(2,2,1);	gr->Title("Ternary axis (x+y+t=1)");
	gr->Ternary(1);
	gr->Plot(x,y,"r2");	gr->Plot(rx,ry,"q^ ");	gr->Cont(a,"BbcyrR");
	gr->Line(mglPoint(0.5,0), mglPoint(0,0.75), "g2");
	gr->Axis(); gr->Grid("xyz","B;");
	gr->Label('x',"B");	gr->Label('y',"C");	gr->Label('t',"A");

	gr->SubPlot(2,2,2);	gr->Title("Quaternary axis 3D");
	gr->Rotate(50,60);		gr->Light(true);
	gr->Ternary(2);
	gr->Plot(x,y,z,"r2");	gr->Surf(a,"BbcyrR#");
	gr->Axis(); gr->Grid();	gr->Box();
	gr->Label('t',"A",1);	gr->Label('x',"B",1);
	gr->Label('y',"C",1);	gr->Label('z',"D",1);

	gr->SubPlot(2,2,3);	gr->Title("Ternary axis 3D");
	gr->Rotate(50,60);		gr->Light(true);
	gr->Ternary(1);
	gr->Plot(x,y,z,"r2");	gr->Surf(a,"BbcyrR#");
	gr->Axis(); gr->Grid();	gr->Box();
	gr->Label('t',"A",1);	gr->Label('x',"B",1);
	gr->Label('y',"C",1);	gr->Label('z',"Z",1);
}
//-----------------------------------------------------------------------------
const char *mmgl_projection="ranges 0 1 0 1 0 1\nnew x 50 '0.25*(1+cos(2*pi*x))'\n"
"new y 50 '0.25*(1+sin(2*pi*x))'\nnew z 50 'x'\nnew a 20 30 '30*x*y*(1-x-y)^2*(x+y<1)'\n"
"new rx 10 'rnd':new ry 10:fill ry '(1-v)*rnd' rx\nlight on\n\n"
"title 'Projection sample':ternary 4:rotate 50 60\nbox:axis:grid\n"
"plot x y z 'r2':surf a '#'\nxlabel 'X':ylabel 'Y':zlabel 'Z'\n";
void smgl_projection(mglGraph *gr)	// flag #
{
	gr->SetRanges(0,1,0,1,0,1);
	mglData x(50),y(50),z(50),rx(10),ry(10), a(20,30);
	a.Modify("30*x*y*(1-x-y)^2*(x+y<1)");
	x.Modify("0.25*(1+cos(2*pi*x))");
	y.Modify("0.25*(1+sin(2*pi*x))");
	rx.Modify("rnd"); ry.Modify("(1-v)*rnd",rx);
	z.Modify("x");
	
	if(!mini)	gr->Title("Projection sample");
	gr->Ternary(4);
	gr->Rotate(50,60);		gr->Light(true);
	gr->Plot(x,y,z,"r2");	gr->Surf(a,"#");
	gr->Axis(); gr->Grid();	gr->Box();
	gr->Label('x',"X",1);	gr->Label('y',"Y",1);	gr->Label('z',"Z",1);
}
//-----------------------------------------------------------------------------
const char *mmgl_triplot="list q 0 1 2 3 | 4 5 6 7 | 0 2 4 6 | 1 3 5 7 | 0 4 1 5 | 2 6 3 7\n"
"list xq -1 1 -1 1 -1 1 -1 1\nlist yq -1 -1 1 1 -1 -1 1 1\nlist zq -1 -1 -1 -1 1 1 1 1\nlight on\n"
"subplot 2 1 0:title 'QuadPlot sample':rotate 50 60\n"
"quadplot q xq yq zq 'yr'\nquadplot q xq yq zq '#k'\n"
"list t 0 1 2 | 0 1 3 | 0 2 3 | 1 2 3\n"
"list xt -1 1 0 0\nlist yt -1 -1 1 0\nlist zt -1 -1 -1 1\n"
"subplot 2 1 1:title 'TriPlot sample':rotate 50 60\n"
"triplot t xt yt zt 'b'\ntriplot t xt yt zt '#k'\n";
void smgl_triplot(mglGraph *gr)
{
	double q[] = {0,1,2,3, 4,5,6,7, 0,2,4,6, 1,3,5,7, 0,4,1,5, 2,6,3,7};
	double xc[] = {-1,1,-1,1,-1,1,-1,1}, yc[] = {-1,-1,1,1,-1,-1,1,1}, zc[] = {-1,-1,-1,-1,1,1,1,1};
	mglData qq(6,4,q), xx(8,xc), yy(8,yc), zz(8,zc);
	gr->Light(true);	//gr->Alpha(true);
	gr->SubPlot(2,1,0);	gr->Title("QuadPlot sample");	gr->Rotate(50,60);
	gr->QuadPlot(qq,xx,yy,zz,"yr");
	gr->QuadPlot(qq,xx,yy,zz,"k#");

	double t[] = {0,1,2, 0,1,3, 0,2,3, 1,2,3};
	double xt[] = {-1,1,0,0}, yt[] = {-1,-1,1,0}, zt[] = {-1,-1,-1,1};
	mglData tt(4,3,t), uu(4,xt), vv(4,yt), ww(4,zt);
	gr->SubPlot(2,1,1);	gr->Title("TriPlot sample");	gr->Rotate(50,60);
	gr->TriPlot(tt,uu,vv,ww,"b");
	gr->TriPlot(tt,uu,vv,ww,"k#");
}
//-----------------------------------------------------------------------------
const char *mmgl_dots="new t 1000 'pi*(rnd-0.5)':new f 1000 '2*pi*rnd'\n"
"copy x 0.9*cos(t)*cos(f):copy y 0.9*cos(t)*sin(f):copy z 0.6*sin(t)\n"
"title 'Dots sample':rotate 50 60\nbox:dots x y z\n";
void smgl_dots(mglGraph *gr)
{
	int i, n=1000;
	mglData x(n),y(n),z(n);
	for(i=0;i<n;i++)
	{
		double t=M_PI*(mgl_rnd()-0.5), f=2*M_PI*mgl_rnd();
		x.a[i] = 0.9*cos(t)*cos(f);
		y.a[i] = 0.9*cos(t)*sin(f);
		z.a[i] = 0.6*sin(t);
	}
	if(!mini)	gr->Title("Dots sample");
	gr->Rotate(50,60);	gr->Box();	gr->Dots(x,y,z);
}
//-----------------------------------------------------------------------------
/*void smgl_surf3_rgbd(mglGraph *gr)
{
	mglData c;	mgls_prepare3d(&c);
	gr->Rotate(40,60);	gr->VertexColor(true);
	gr->Box();	gr->Surf3(c,"bgrd");
}*/
//-----------------------------------------------------------------------------
const char *mmgl_mirror="new a 31 41 '-pi*x*exp(-(y+1)^2-4*x^2)'\n"
"subplot 2 2 0:title 'Options for coordinates':alpha on:light on:rotate 40 60:box\n"
"surf a 'r';yrange 0 1:surf a 'b';yrange 0 -1\n"
"subplot 2 2 1:title 'Option \"meshnum\"':rotate 40 60:box\n"
"mesh a 'r'; yrange 0 1:mesh a 'b';yrange 0 -1; meshnum 5\n"
"subplot 2 2 2:title 'Option \"alpha\"':rotate 40 60:box\n"
"surf a 'r';yrange 0 1; alpha 0.7:surf a 'b';yrange 0 -1; alpha 0.3\n"
"subplot 2 2 3 '<_':title 'Option \"legend\"'\n"
"fplot 'x^3' 'r'; legend 'y = x^3':fplot 'cos(pi*x)' 'b'; legend 'y = cos \\pi x'\n"
"box:axis:legend 2\n";
void smgl_mirror(mglGraph *gr)	// flag #
{
	mglData a(31,41);
	gr->Fill(a,"-pi*x*exp(-(y+1)^2-4*x^2)");
	
	if(!mini)	{	gr->SubPlot(2,2,0);	gr->Title("Options for coordinates");	}
	gr->Alpha(true);	gr->Light(true);
	gr->Rotate(40,60);	gr->Box();
	gr->Surf(a,"r","yrange 0 1"); gr->Surf(a,"b","yrange 0 -1");
	if(mini)	return;
	gr->SubPlot(2,2,1);	gr->Title("Option 'meshnum'");
	gr->Rotate(40,60);	gr->Box();
	gr->Mesh(a,"r","yrange 0 1"); gr->Mesh(a,"b","yrange 0 -1; meshnum 5");
	gr->SubPlot(2,2,2);	gr->Title("Option 'alpha'");
	gr->Rotate(40,60);	gr->Box();
	gr->Surf(a,"r","yrange 0 1; alpha 0.7"); gr->Surf(a,"b","yrange 0 -1; alpha 0.3");
	gr->SubPlot(2,2,3,"<_");	gr->Title("Option 'legend'");
	gr->FPlot("x^3","r","legend 'y = x^3'"); gr->FPlot("cos(pi*x)","b","legend 'y = cos \\pi x'");
	gr->Box();	gr->Axis();	gr->Legend(2,"");
}
//-----------------------------------------------------------------------------
mglSample samp[] = {
	{"alpha", smgl_alpha, mmgl_alpha },
	{"area", smgl_area, mmgl_area},
	{"aspect", smgl_aspect, mmgl_aspect },
	{"axial", smgl_axial, mmgl_axial },
	{"axis", smgl_axis, mmgl_axis},
	{"barh", smgl_barh, mmgl_barh},
	{"bars", smgl_bars, mmgl_bars},
	{"belt", smgl_belt, mmgl_belt},
	{"box", smgl_box, mmgl_box},
	{"boxplot", smgl_boxplot, mmgl_boxplot},
	{"boxs", smgl_boxs, mmgl_boxs},
	{"candle", smgl_candle, mmgl_candle},
	{"chart", smgl_chart, mmgl_chart},
	{"cloud", smgl_cloud, mmgl_cloud },
	{"colorbar", smgl_colorbar, mmgl_colorbar},
	{"combined", smgl_combined, mmgl_combined },
	{"cones", smgl_cones, mmgl_cones},
	{"cont", smgl_cont, mmgl_cont},
	{"cont_xyz", smgl_cont_xyz, mmgl_cont_xyz},
	{"conta", smgl_conta, mmgl_conta},
	{"contd", smgl_contd, mmgl_contd},
	{"contf", smgl_contf, mmgl_contf},
	{"contf_xyz", smgl_contf_xyz, mmgl_contf_xyz},
	{"contfa", smgl_contfa, mmgl_contfa},
	{"contv", smgl_contv, mmgl_contv},
//	{"crust", smgl_crust, mmgl_crust},	// TODO: open after triangulation
	{"curvcoor", smgl_curvcoor, mmgl_curvcoor},
	{"cut", smgl_cut, mmgl_cut},
	{"dat_diff", smgl_dat_diff, mmgl_dat_diff},
	{"dat_extra", smgl_dat_extra, mmgl_dat_extra },
	{"dens", smgl_dens, mmgl_dens},
	{"dens_xyz", smgl_dens_xyz, mmgl_dens_xyz},
	{"densa", smgl_densa, mmgl_densa},
	{"dew", smgl_dew, mmgl_dew},
	{"dots", smgl_dots, mmgl_dots},
	{"error", smgl_error, mmgl_error },
	{"fall", smgl_fall, mmgl_fall},
	{"fit", smgl_fit, mmgl_fit},
	{"flow", smgl_flow, mmgl_flow},
	{"fog", smgl_fog, mmgl_fog},
	{"fonts", smgl_fonts, mmgl_fonts},
	{"grad", smgl_grad, mmgl_grad},
	{"hist", smgl_hist, mmgl_hist},
	{"inplot", smgl_inplot, mmgl_inplot},
	{"label", smgl_label, mmgl_label},
	{"legend", smgl_legend, mmgl_legend },
	{"loglog", smgl_loglog, mmgl_loglog},
	{"map", smgl_map, mmgl_map},
	{"mark", smgl_mark, mmgl_mark},
	{"mesh", smgl_mesh, mmgl_mesh},
	{"mirror", smgl_mirror, mmgl_mirror },
	{"molecule", smgl_molecule, mmgl_molecule },
	{"parser", smgl_parser, mmgl_parser},
	{"pde", smgl_pde, mmgl_pde},
	{"pipe", smgl_pipe, mmgl_pipe},
	{"plot", smgl_plot, mmgl_plot},
	{"primitives", smgl_primitives, mmgl_primitives },
	{"projection", smgl_projection, mmgl_projection },
	{"qo2d", smgl_qo2d, mmgl_qo2d},
	{"radar", smgl_radar, mmgl_radar},
	{"region", smgl_region, mmgl_region},
	{"schemes", smgl_schemes, mmgl_schemes },
	{"several_light", smgl_several_light, mmgl_several_light },
	{"solve", smgl_solve, mmgl_solve},
	{"stem", smgl_stem, mmgl_stem},
	{"step", smgl_step, mmgl_step},
	{"stereo", smgl_stereo, mmgl_stereo},
	{"stfa", smgl_stfa, mmgl_stfa},
	{"style", smgl_style, mmgl_style },
	{"surf", smgl_surf, mmgl_surf},
	{"surf3", smgl_surf3, mmgl_surf3},
	{"surf3a", smgl_surf3a, mmgl_surf3a},
	{"surf3c", smgl_surf3c, mmgl_surf3c},
	{"surfa", smgl_surfa, mmgl_surfa},
	{"surfc", smgl_surfc, mmgl_surfc},
	{"table", smgl_table, mmgl_table},
	{"tape", smgl_tape, mmgl_tape},
	{"tens", smgl_tens, mmgl_tens},
	{"ternary", smgl_ternary, mmgl_ternary },
	{"text", smgl_text, mmgl_text},
	{"textmark", smgl_textmark, mmgl_textmark},
	{"ticks", smgl_ticks, mmgl_ticks},
	{"tile", smgl_tile, mmgl_tile},
	{"tiles", smgl_tiles, mmgl_tiles},
	{"torus", smgl_torus, mmgl_torus },
	{"traj", smgl_traj, mmgl_traj},
	{"triangulation",smgl_triangulation, mmgl_triangulation },
	{"triplot", smgl_triplot, mmgl_triplot},
	{"tube", smgl_tube, mmgl_tube},
	{"type0", smgl_type0, mmgl_type0},
	{"type1", smgl_type1, mmgl_type1},
	{"type2", smgl_type2, mmgl_type2},
	{"vect", smgl_vect, mmgl_vect},
	{"vecta", smgl_vecta, mmgl_vecta},
	{"venn", smgl_venn, mmgl_venn},
{"", NULL}};
//-----------------------------------------------------------------------------