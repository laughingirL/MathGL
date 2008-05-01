#include <time.h>
#include <locale.h>
#include <string.h>
#include <stdio.h>
#include <mgl/mgl_zb.h>
#include <mgl/mgl_eps.h>
//-----------------------------------------------------------------------------
mglData s;
//-----------------------------------------------------------------------------
void save(mglGraph *gr,const char *name,const char *suf="",int type=0)
{
//	return;
	char buf[128];
	switch(type)
	{
	case 1:	// EPS
		sprintf(buf,"%s%s.eps",name,suf);
		gr->WriteEPS(buf);	break;
	case 2:	// SVG
		sprintf(buf,"%s%s.svg",name,suf);
		gr->WriteSVG(buf);	break;
	case 3:	// PNG
		sprintf(buf,"%s%s.png",name,suf);
		gr->WritePNG(buf,0,true);	break;
	case 4:	// JPEG
		sprintf(buf,"%s%s.jpg",name,suf);
		gr->WriteJPEG(buf);	break;
	default:// PNG (no alpha)
		sprintf(buf,"%s%s.png",name,suf);
		gr->WritePNG(buf,0,false);	break;
	}
}
//-----------------------------------------------------------------------------
int sample_ae(mglGraph *gr, const void *)	// arrow styles
{
	gr->Puts(mglPoint(0), "\\sqrt{\\frac{\\alpha^{\\gamma^2}+\\overset 1{\\big\\infty}}{\\sqrt3{2+b}}}",
			 0, -4);
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
		gr->Error(mglPoint(mgl_rnd()-0.5,mgl_rnd()-0.5,mgl_rnd()-0.5),
			mglPoint(0.1,0.1,0.1),"bo");
	gr->Axis();
	return 0;
}
//-----------------------------------------------------------------------------
int sample_ab(mglGraph *gr, const void *)	// Gaussian beam
{
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
	gr->CloudQ(a);		gr->Box();
	gr->SubPlot(2,2,3);	gr->Rotate(40,60);
	gr->Surf3A(b,a,"q");gr->Box();
	return 0;
}
//-----------------------------------------------------------------------------
int sample_aa(mglGraph *gr, const void *)	// flag #
{
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
	gr->Legend();		gr->Box();
	gr->ClearLegend();	// clear legend strings
	return 0;
}
//-----------------------------------------------------------------------------
int sample_a6(mglGraph *gr, const void *)	// differentiate
{
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
	gr->Line(mglPoint(0,1,0),mglPoint(0.3,1,0),"k-");		gr->Puts(mglPoint(0.4,1),"Solid '-'","rL");
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
	setlocale(LC_CTYPE, "ru_RU.cp1251");
	gr->Puts(mglPoint(0,1),"Text can be in english и на русском");
	gr->Puts(mglPoint(0,0.6),"It can be \\wire{wire} and \\big{big}");
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
	x.Fill(-1,1);           // create data arrays

	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1),mglPoint(-1,1,-1));
	gr->dx = gr->dy = -5;	gr->dz = 0.5;  // set tick step to 0.5

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
	gr->dx = gr->dy = 0.5;  // set tick step to 0.5
	gr->NSx = gr->NSy = 3;  // and draw 3 subticks
	gr->Box();              // should be after change dx,dy,dz
	gr->Axis("xy");
	gr->Grid();
	gr->Text(mglPoint(0,1.3,1),"Axis and grid","rC");

	gr->dx = gr->dy = -5;   // restore back
	gr->NSx = gr->NSy = 0;
	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1),mglPoint(0,0,0));

	gr->dx = gr->dy = gr->dz = -5;   // restore back
	gr->SubPlot(2,2,1);
	gr->Rotate(60,40);
	gr->Axis();
	gr->Label('x',"x");
	gr->Label('y',"y");
	gr->Label('z',"z");
	gr->Text(mglPoint(0,0,1.5),"Axis and labels");

	gr->dx = gr->dy = gr->dz = 0.2;   // too low step of ticks
	gr->SubPlot(2,2,2);
	gr->Rotate(60,40);
	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1),mglPoint(-1,-1,-1));
	gr->Axis();
	gr->Grid();
	gr->Text(mglPoint(0,0,1.5),"Shift origin and add grid");
	gr->Text(mglPoint(0,0,1.2),"(note, too many ticks)");

	gr->SubPlot(2,2,3);
	gr->Rotate(60,40);
	gr->dz = gr->dy = -5;   // decrease number of ticks
	gr->Axis("yz");
	gr->Label('y',"Y axis",0);
	gr->Label('z',"Z axis",0);
	gr->Text(mglPoint(0,0,1.5),"Remove X axis, and","rC");
	gr->Text(mglPoint(0,0,1.2),"decrease number of ticks");
	return 0;
}
//-----------------------------------------------------------------------------
int sample_a1(mglGraph *gr, const void *)	// transformation
{
	gr->SubPlot(2,2,0);     // just new axis without rotation and aspects
	gr->Box();
	gr->Text(mglPoint(-1,1.1,1),"Just box","rL");
	gr->InPlot(0.2,0.5,0.7,1);
	gr->Box();
	gr->Text(mglPoint(0,1.2,1),"InPlot example");
	gr->SubPlot(2,2,1);     // new axis with aspect and rotation
	gr->Rotate(60,40);
	gr->Aspect(1,1,1);
	gr->Box();
	gr->Text(mglPoint(1,1,1.5),"Rotate only","rR");
	gr->SubPlot(2,2,2);     // aspect in other direction
	gr->Rotate(60,40);
	gr->Aspect(1,1,2);
	gr->Box();
	gr->Text(mglPoint(0,0,2),"Aspect and Rotate");
	gr->SubPlot(2,2,3);     // rotation before aspect. INCORRECT !!!
	gr->Rotate(60,40);
	gr->Aspect(1,2,2);
	gr->Box();
	gr->Text(mglPoint(0,0,1.5),"Aspect in other direction");
	return 0;
}
//-----------------------------------------------------------------------------
int full_test(mglGraph *gr, const void *s)	// full test (in PNG)
{
	const char *suf = (const char *)s;
	mglData pnts("hotdogs.pts");
	pnts.Norm(-1,1,true);
	mglData  a(50,40), b(50,40), c(60,50,40), d(60,50,40);
	mglData y(50,3), x(50), x2(50), y1(50), y2(50), f(50,3);
	mglData ex(10,10,10), ey(10,10,10), ez(10,10,10);
	
/*	int i,j,k;
	float xx,yy,zz,dd;
	for(i=0;i<10;i++)	for(j=0;j<10;j++)	for(k=0;k<10;k++)
	{
		xx = i*0.1-0.5;	yy = j*0.1-0.5;	zz = k*0.1-0.35;	dd = k*0.1-0.65;
		ex.a[i+10*(j+10*k)] = 0.1*xx/pow(xx*xx+yy*yy+zz*zz,1.5) - 0.1*xx/pow(xx*xx+yy*yy+dd*dd,1.5);
		ey.a[i+10*(j+10*k)] = 0.1*yy/pow(xx*xx+yy*yy+zz*zz,1.5) - 0.1*yy/pow(xx*xx+yy*yy+dd*dd,1.5);
		ez.a[i+10*(j+10*k)] = 0.1*zz/pow(xx*xx+yy*yy+zz*zz,1.5) - 0.1*dd/pow(xx*xx+yy*yy+dd*dd,1.5);
	}
	for(i=0;i<60;i++)	for(j=0;j<50;j++)	for(k=0;k<40;k++)
	{
		xx = i/30.-1;	yy = j/25.-1;	zz = k/20.-1;
		c.a[i+60*(j+50*k)] = -2*(xx*xx + yy*yy + zz*zz*zz*zz - zz*zz - 0.1);
		d.a[i+60*(j+50*k)] = 1-2*tanh((xx+yy)*(xx+yy));
	}*/
	ex.Modify("0.1*(x-0.5)/pow((x-0.5)^2+(y-0.5)^2 + (z-0.35)^2,1.5) - 0.1*(x-0.5)/pow((x-0.5)^2 + (y-0.5)^2+(z-0.65)^2,1.5)");
	ey.Modify("0.1*(y-0.5)/pow((x-0.5)^2+(y-0.5)^2 + (z-0.35)^2,1.5) - 0.1*(y-0.5)/pow((x-0.5)^2 + (y-0.5)^2+(z-0.65)^2,1.5)");
	ez.Modify("0.1*(z-0.35)/pow((x-0.5)^2+(y-0.5)^2 + (z-0.35)^2,1.5) - 0.1*(z-0.65)/pow((x-0.5)^2+(y-0.5)^2 + (z-0.65)^2,1.5)");
	a.Modify("0.6*sin(2*pi*x)*sin(3*pi*y) + 0.4*cos(3*pi*(x*y))");
	b.Modify("0.6*cos(2*pi*x)*cos(3*pi*y) + 0.4*cos(3*pi*(x*y))");
	c.Modify("(-2*((2*x-1)^2 + (2*y-1)^2 + (2*z-1)^4 - (2*z-1)^2 - 0.1))");
	d.Modify("1-2*tanh(4*(x+y-1)^2)");
	y.Modify("0.7*sin(2*pi*x) + 0.5*cos(3*pi*x) + 0.2*sin(pi*x)",0);
	y.Modify("sin(2*pi*x)",1);	y.Modify("cos(2*pi*x)",2);
	y1.Modify("0.5+0.3*cos(2*pi*x)");
	y2.Modify("0.3*sin(2*pi*x)");
	x.Fill(-1,1,'x');
	x2.Modify("0.05+0.03*cos(2*pi*x)");

	mglData  x0(10), y0(10), ex0(10), ey0(10),ch(7,2);
	x0.Modify("2*x-1 + 0.1*rnd-0.05");
	y0.Modify("0.7*sin(2*pi*x) + 0.5*cos(3*pi*x) + 0.2*sin(pi*x) + 0.2*rnd-0.1");
	ey0.Modify("0.2");	ex0.Modify("0.1");	ch.Modify("rnd+0.1");

	gr->SubPlot(1,1,0);	gr->Alpha(false);
	gr->Clf();	gr->Box();	gr->MeshNum=20;	gr->Dew(a,b,"BbcyrR");	gr->MeshNum=0;	save(gr,"dew",suf);
	gr->Rotate(40,60);
	gr->Clf();	gr->Box();	gr->MeshNum=10;	gr->Dew(a,b,"BbcyrR");	gr->MeshNum=0;	save(gr,"dew2",suf);
	gr->Clf();	gr->Box();	gr->Pipe(a,b,"BbcyrR");		save(gr,"pipe",suf);
	gr->Clf();	gr->Box();	gr->Surf(a,"BbcyrR");		save(gr,"surf",suf);
	gr->Clf();	gr->Box();	gr->Tile(a,"BbcyrR");		save(gr,"tile",suf);
	gr->Clf();	gr->Box();	gr->Belt(a,"BbcyrR");		save(gr,"belt",suf);
	gr->Clf();	gr->Box();	gr->Mesh(a,"BbcyrR");		save(gr,"mesh",suf);
	gr->Clf();	gr->Box();	gr->Fall(a,"BbcyrR");		save(gr,"fall",suf);
	gr->Clf();	gr->Box();	gr->SurfC(a,b,"BbcyrR");	save(gr,"surfc",suf);
	gr->Clf();	gr->Box();	gr->Boxs(a,"BbcyrR");		save(gr,"boxs",suf);
	gr->Clf();	gr->Box();	gr->Flow(ex,ey,ez,"bwr");	save(gr,"flow3",suf);
	gr->Clf();	gr->Box();	gr->Vect(ex,ey,ez,"bwr");	save(gr,"vect3",suf);
	gr->Clf();	gr->Box();	gr->VectC(ex,ey,ez,"bwr");	save(gr,"vectc3",suf);
	gr->Clf();	gr->Box();	gr->Pipe(ex,ey,ez,"bwr");	save(gr,"pipe3",suf);
	gr->Clf();	gr->Box();	gr->Torus(y1,y2,"pz");		save(gr,"torus",suf);
	gr->Clf();	gr->Box();	gr->Crust(pnts,"BbcyrR");	save(gr,"crust",suf);
	gr->Clf();	gr->Box();	gr->Dots(pnts,"BbcyrR");	save(gr,"dots",suf);
	gr->Clf();	gr->Box();	gr->ContF(a,"BbcyrR");		save(gr,"contf",suf);
	gr->Clf();	gr->Box();	gr->Cont(a,"BbcyrR");		save(gr,"cont",suf);
	gr->Clf();	gr->Box();	gr->ContFA(c,"BbcyrR");		save(gr,"contfa",suf);
	gr->Clf();	gr->Fog(1);	gr->Box();	gr->Surf(a,"BbcyrR");	gr->Fog(0);	save(gr,"surf_fog",suf);
	gr->Clf();	gr->Box();	gr->Tube(y,0.05);	gr->Tube(y1,x,y2,x2);	save(gr,"tube",suf);
	gr->Clf();	gr->Box();	gr->Chart(ch,"#");	save(gr,"chart",suf);
	gr->Axis("(y+1)/2*cos(pi*x)","(y+1)/2*sin(pi*x)",0);
	gr->Clf();	gr->Box();	gr->Chart(ch,"#");	save(gr,"pie_chart",suf);	gr->Axis(0,0,0);

	gr->Alpha(true);
	gr->Clf();	gr->Box();	gr->Surf(a,"BbcyrR");		save(gr,"surf_alpha",suf);
	gr->Clf();	gr->Box();	gr->Axial(a,"BbcyrR");		save(gr,"axial",suf);
	gr->Clf();	gr->Box();	gr->SurfA(a,b,"BbcyrR");	save(gr,"surfa",suf);
	gr->Clf();	gr->Box();	gr->Map(a,b,"brg");			save(gr,"map",suf);
	gr->Clf();	gr->Box();	gr->Surf3(c,"BbcyrR");		save(gr,"surf3",suf);
	gr->Clf();	gr->Box();	gr->Surf3(c,"bgrd");		save(gr,"surf3_rgbd_light",suf);
	gr->Clf();	gr->Box();	gr->Surf3A(c,d,"BbcyrR");	save(gr,"surf3a",suf);
	gr->Clf();	gr->Box();	gr->Surf3C(c,d,"BbcyrR");	save(gr,"surf3c",suf);
	gr->Clf();	gr->Box();	gr->ContA(c,"BbcyrR");		save(gr,"conta",suf);
	gr->Light(false);
	gr->Clf();	gr->Box();	gr->CloudQ(c,"wyrRk");		save(gr,"cloudq",suf);
	gr->Clf();	gr->Box();	gr->CloudP(c,"wyrRk");		save(gr,"cloudp",suf);
	gr->Clf();	gr->Box();	gr->Axis();	gr->DensA(c,"BbcyrR");	save(gr,"densa",suf);
	gr->Alpha(false);
	gr->Clf();	gr->Box();	gr->Surf3(c,"bgrd");		save(gr,"surf3_rgbd",suf);
	gr->Clf();	gr->Box();	gr->DensX(c.Sum("x"),"BbcyrR",-1);	gr->DensY(c.Sum("y"),"BbcyrR",1);
	gr->DensZ(c.Sum("z"),"BbcyrR",-1);		save(gr,"dens_xyz",suf);
	gr->Clf();	gr->Box();	gr->ContX(c.Sum("x"),"BbcyrR",-1);	gr->ContY(c.Sum("y"),"BbcyrR",1);
	gr->ContZ(c.Sum("z"),"BbcyrR",-1);		save(gr,"cont_xyz",suf);
	gr->SubPlot(1,1,0);	// 1D Plots
	gr->Clf();	gr->Box();	gr->Plot(y);	save(gr,"plot",suf);
	gr->Clf();	gr->Box();	gr->Area(y);	save(gr,"area",suf);
	gr->Clf();	gr->Box();	gr->Stem(y);	save(gr,"stem",suf);
	gr->Clf();	gr->Box();	gr->Step(y);	save(gr,"step",suf);
	gr->Clf();	gr->Box();	gr->Bars(y);	save(gr,"bars",suf);
	gr->Clf();	gr->Box();	gr->Mark(y,y1,"bs");	save(gr,"mark",suf);
	gr->Clf();	gr->Box();	gr->TextMark(y,y1,"\\gamma");	save(gr,"textmark",suf);
	gr->Clf();	gr->Box();	gr->Plot(y.SubData(-1,0));
	gr->Text(y,"This is very long string drawn along a curve",":k");
	gr->Text(y,"Another string drawn above a curve","T:r");	save(gr,"text",suf);
	gr->Clf();	gr->Box();	gr->Plot(y.SubData(-1,0));	gr->Error(x0,y0,ex0,ey0,"ko");	save(gr,"error",suf);

	gr->Clf();	gr->Box();	gr->Dens(a,"BbcyrR");	gr->Colorbar();	save(gr,"dens",suf);
	gr->Clf();	gr->Box();	gr->Tile(a,b,"BbcyrR");		save(gr,"tiler",suf);
	gr->Clf();	gr->Box();	gr->Cont(a,"BbcyrRt");		save(gr,"contt",suf);
	gr->Clf();	gr->Box();	gr->Vect(a,b,"BbcyrR");		save(gr,"vect",suf);
	gr->Clf();	gr->Box();	gr->VectC(a,b,"BbcyrR");	save(gr,"vectc",suf);
	gr->Clf();	gr->Box();	gr->Flow(a,b,"BbcyrR");		save(gr,"flow",suf);
	gr->Clf();	gr->Box();	gr->Pipe(a,b,"BbcyrR");		save(gr,"pipe2",suf);

	f.Modify("sin(2*pi*x*x)",0);	f.Modify("sin(2*pi*x)",1);	f.Modify("sin(2*pi*sqrt(x))",2);
	gr->Axis(mglPoint(0,-1,0),mglPoint(1,1,1));	gr->Clf();	gr->Box();
	gr->Plot(f);	gr->Axis("xy");	gr->AddLegend("sin(\\pi {x^2})","b");
	gr->AddLegend("sin(\\pi x)","g*");	gr->AddLegend("sin(\\pi \\sqrt{x})","r+");
	gr->Legend();	save(gr,"legend",suf);
	gr->ClearLegend();	// clear legend strings
	return 0;
}
//-----------------------------------------------------------------------------
int sample_crust(mglGraph *gr, const void *)
{
	mglData pnts("hotdogs.pts");
	pnts.Norm(-1,1,true);

	gr->Rotate(40,60);
	gr->Light(true);
	gr->Alpha(true);
	gr->Box();
	gr->Crust(pnts);
	//gr->Dots(pnts,"k");
	return 0;
}
//-----------------------------------------------------------------------------
int sample_transp(mglGraph *gr, const void *s)	// flag #
{
	const char *suf = (const char *)s;
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
	const char *suf = (const char *)s;
	mglData  a(50,40), b(50,40), d(50,40), c(61,51,40), y(50,3), y1(50), y2(50);
	a.Modify("0.6*sin(2*pi*x)*sin(3*pi*y) + 0.4*cos(3*pi*(x*y))");
	b.Modify("0.6*cos(2*pi*x)*cos(3*pi*y) + 0.4*cos(3*pi*(x*y))");
	c.Modify("(-2*((2*x-1)^2 + (2*y-1)^2 + (2*z-1)^4 - (2*z-1)^2 - 0.1))");
	y.Modify("0.7*sin(2*pi*x) + 0.5*cos(3*pi*x) + 0.2*sin(pi*x)",0);
	y.Modify("sin(2*pi*x)",1);	y.Modify("cos(2*pi*x)",2);
	y1.Modify("0.3*sin(2*pi*x)");	y2.Modify("0.5+0.3*cos(2*pi*x)");
	for(int i=0;i<a.nx*a.ny;i++)	d.a[i] = hypot(a.a[i],b.a[i]);

	gr->SubPlot(1,1,0);	gr->Clf();	gr->Alpha(false);	gr->Light(false);
	// 2-axis plot
	gr->Clf();	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1),mglPoint(-1,-1,-1));
	gr->Axis();		gr->Label('y',"axis 1",0);	gr->Plot(y1,"b");
	gr->Axis(mglPoint(0,0,0),mglPoint(1,1,1),mglPoint(1,1,1));	// set up second axis
	gr->Axis();		gr->Label('y',"axis 2",0);	gr->Stem(y2,"r");
	save(gr,"2_axis",suf);

	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1));
	// contours under mesh
	gr->Clf();	gr->Flow(a,b,"br");	gr->Dens(d,"BbcyrR");	gr->Box();
	save(gr,"flow_dens",suf);

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

	return 0;
}
//-----------------------------------------------------------------------------
int sample_molecule(mglGraph *gr, const void *)	// flag #
{
	gr->Alpha(true); gr->Light(true);

	gr->SubPlot(2,2,0);
	gr->Puts(mglPoint(0,1.2),"Methane, CH_4",0,-2); gr->Rotate(60,120);
	gr->Sphere(mglPoint(0,0,0),0.25,"k");
	gr->Drop(mglPoint(0,0,0),mglPoint(0,0,1),0.35,"h",1,2);
	gr->Sphere(mglPoint(0,0,0.7),0.25,"g");
	gr->Drop(mglPoint(0,0,0),mglPoint(-0.94,0,-0.33),0.35,"h",1,2);
	gr->Sphere(mglPoint(-0.66,0,-0.23),0.25,"g");
	gr->Drop(mglPoint(0,0,0),mglPoint(0.47,0.82,-0.33),0.35,"h",1,2);
	gr->Sphere(mglPoint(0.33,0.57,-0.23),0.25,"g");
	gr->Drop(mglPoint(0,0,0),mglPoint(0.47,-0.82,-0.33),0.35,"h",1,2);
	gr->Sphere(mglPoint(0.33,-0.57,-0.23),0.25,"g");

	gr->SubPlot(2,2,1);
	gr->Puts(mglPoint(0,1.2),"Water, H_{2}O",0,-2); gr->Rotate(60,100);
	gr->Sphere(mglPoint(0,0,0),0.25,"r");
	gr->Drop(mglPoint(0,0,0),mglPoint(0.3,0.5,0),0.3,"m",1,2);
	gr->Sphere(mglPoint(0.3,0.5,0),0.25,"g");
	gr->Drop(mglPoint(0,0,0),mglPoint(0.3,-0.5,0),0.3,"m",1,2);
	gr->Sphere(mglPoint(0.3,-0.5,0),0.25,"g");

	gr->SubPlot(2,2,2);
	gr->Puts(mglPoint(0,1.2),"Oxygen, O_2",0,-2); gr->Rotate(60,120);
	gr->Drop(mglPoint(0,0.5,0),mglPoint(0,-0.3,0),0.3,"m",1,2);
	gr->Sphere(mglPoint(0,0.5,0),0.25,"r");
	gr->Drop(mglPoint(0,-0.5,0),mglPoint(0,0.3,0),0.3,"m",1,2);
	gr->Sphere(mglPoint(0,-0.5,0),0.25,"r");

	gr->SubPlot(2,2,3);
	gr->Puts(mglPoint(0,1.2),"Ammonia, NH_3",0,-2); gr->Rotate(60,120);
	gr->Sphere(mglPoint(0,0,0),0.25,"b");
	gr->Drop(mglPoint(0,0,0),mglPoint(0.33,0.57,0),0.32,"n",1,2);
	gr->Sphere(mglPoint(0.33,0.57,0),0.25,"g");
	gr->Drop(mglPoint(0,0,0),mglPoint(0.33,-0.57,0),0.32,"n",1,2);
	gr->Sphere(mglPoint(0.33,-0.57,0),0.25,"g");
	gr->Drop(mglPoint(0,0,0),mglPoint(-0.65,0,0),0.32,"n",1,2);
	gr->Sphere(mglPoint(-0.65,0,0),0.25,"g");
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

	gr->Text(mglPoint(0,1.3), "Ternary plot (a+b+c=1)");
	gr->Ternary(true);
	gr->Plot(x,y,"r2");
	gr->Plot(rx,ry,"q^ ");
	gr->Cont(a,"BbcyrR");
	gr->Line(mglPoint(0.5,0), mglPoint(0,0.75), "g2");
	gr->Axis(); gr->Grid("xyz","B;");
	gr->Label('x',"a comp.", 0);
	gr->Label('y',"b comp.", 0);
	gr->Label('z',"c comp.", 0);
	gr->Ternary(false);
	return 0;
}
//-----------------------------------------------------------------------------
int sample_drops(mglGraph *gr, const void *s)	// flag #
{
	const char *suf = (const char *)s;
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
	save(gr,"drops",suf);

	mglData a(10,10), b(10,10), x(10,10), y(10,10);
	a.Modify("0.3*x+rnd"); x.Modify("x+0.1*rnd");
	b.Modify("0.3*y+rnd"); y.Modify("y+0.1*rnd");

	gr->Clf();
	gr->Axis(mglPoint(0,0),mglPoint(1.1,1.1));
	gr->Light(true);
	gr->Dew(x,y,a,b,"BbcyrR");
	save(gr,"fish",suf);

	return 0;
}
//-----------------------------------------------------------------------------
int sample_mirror(mglGraph *gr, const void *)	// flag #
{
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
int sample_logaxis(mglGraph *gr, const void *s)	// flag #
{
	const char *suf = (const char *)s;
	mglData x(2000), y(2000);
	x.Modify("0.01/(x+10^(-5))"); y.Modify("sin(1/v)",x);

	gr->Axis(mglPoint(0.01,-1),mglPoint(1000,1),mglPoint(0.01,-1));
	gr->Axis("lg(x)",0,0); gr->dx = 0;	gr->Box();
	gr->Plot(x,y,"b2");
	gr->Axis(); gr->Grid("xy","g");
	gr->Label('x',"x",0); gr->Label('y', "y = sin 1/x",0);
	save(gr,"semilog",suf);

	x.Modify("pow(10,6*x-3)"); y.Modify("sqrt(1+v^2)",x);
	gr->Axis(mglPoint(0.001,0.1),mglPoint(1000,1000),mglPoint(0.001,0.1));
	gr->Axis("lg(x)","lg(y)",0); gr->dx = gr->dy = 0;

	gr->Clf();	gr->Box();
	gr->Plot(x,y,"b2");
	gr->Axis(); gr->Grid("xy","g;");
	gr->Label('x',"x",0); gr->Label('y', "y=\\sqrt{1+x^2}",0);
	save(gr,"loglog",suf);

	gr->dx = gr->dy = gr->dz = -5;   // restore back
	gr->Axis(0,0,0);
	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1),mglPoint(0,0,0));
	return 0;
}
//-----------------------------------------------------------------------------
int all_samples(mglGraph *gr, const void *s)
{
	const char *suf = (const char *)s;
	gr->SubPlot(1,1,0);gr->Clf();sample_a1(gr,0);	save(gr,"sample1",suf);
	gr->SubPlot(1,1,0);gr->Clf();sample_a2(gr,0);	save(gr,"sample2",suf);
	gr->SubPlot(1,1,0);gr->Clf();sample_a3(gr,0);	save(gr,"sample3",suf);
	gr->SubPlot(1,1,0);gr->Clf();sample_a4(gr,0);	save(gr,"sample4",suf);
	gr->SubPlot(1,1,0);gr->Clf();sample_a5(gr,0);	save(gr,"sample5",suf);
	gr->SubPlot(1,1,0);gr->Clf();sample_a6(gr,0);	save(gr,"sample6",suf);
	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1));
	gr->SubPlot(1,1,0);gr->Clf();sample_a7(gr,0);	save(gr,"sample7",suf);
	gr->SubPlot(1,1,0);gr->Clf();sample_a8(gr,0);	save(gr,"sample8",suf);
	gr->SubPlot(1,1,0);gr->Clf();sample_a9(gr,0);	save(gr,"sample9",suf);
	gr->SubPlot(1,1,0);gr->Clf();sample_aa(gr,0);	save(gr,"samplea",suf);
	gr->SubPlot(1,1,0);gr->Clf();sample_ab(gr,0);	save(gr,"sampleb",suf);
	gr->SubPlot(1,1,0);gr->Clf();sample_ac(gr,0);	save(gr,"samplec",suf);
	gr->SubPlot(1,1,0);gr->Clf();sample_ad(gr,0);	save(gr,"sampled",suf);
	gr->SubPlot(1,1,0);gr->Clf();sample_ae(gr,0);	save(gr,"samplee",suf);
	gr->SubPlot(1,1,0);gr->Clf();sample_molecule(gr,0);	save(gr,"molecule",suf);
	gr->SubPlot(1,1,0);gr->Clf();sample_ternary(gr,0);	save(gr,"ternary",suf);
	gr->SubPlot(1,1,0);gr->Clf();sample_drops(gr,s);
	gr->SubPlot(1,1,0);gr->Clf();sample_logaxis(gr,s);
	gr->SubPlot(1,1,0);gr->Clf();sample_mirror(gr,0);	save(gr,"mirror",suf);
	gr->CAxis(-1,1);
	return 0;
}
//-----------------------------------------------------------------------------
int time_test(mglGraph *gr, const void *)	// full test (in PNG)
{
	time_t t1,t2;
	int i;
	mglData pnts("hotdogs.pts");
	pnts.Norm(-1,1,true);
	mglData  a(50,40), b(50,40), c(50,40,30), d(50,40,30);
	mglData y(50,3), x(50), x2(50), y1(50), y2(50), f(50,3);
	mglData ex(10,10,10), ey(10,10,10), ez(10,10,10);
	
	ex.Modify("0.1*(x-0.5)/pow((x-0.5)^2+(y-0.5)^2 + (z-0.35)^2,1.5) - 0.1*(x-0.5)/pow((x-0.5)^2 + (y-0.5)^2+(z-0.65)^2,1.5)");
	ey.Modify("0.1*(y-0.5)/pow((x-0.5)^2+(y-0.5)^2 + (z-0.35)^2,1.5) - 0.1*(y-0.5)/pow((x-0.5)^2 + (y-0.5)^2+(z-0.65)^2,1.5)");
	ez.Modify("0.1*(z-0.35)/pow((x-0.5)^2+(y-0.5)^2 + (z-0.35)^2,1.5) - 0.1*(z-0.65)/pow((x-0.5)^2+(y-0.5)^2 + (z-0.65)^2,1.5)");
	a.Modify("0.6*sin(2*pi*x)*sin(3*pi*y) + 0.4*cos(3*pi*(x*y))");
	b.Modify("0.6*cos(2*pi*x)*cos(3*pi*y) + 0.4*cos(3*pi*(x*y))");
	c.Modify("(-2*((2*x-1)^2 + (2*y-1)^2 + (2*z-1)^4 - (2*z-1)^2 - 0.1))");
	d.Modify("1-2*tanh(4*(x+y-1)^2)");
	y.Modify("0.7*sin(2*pi*x) + 0.5*cos(3*pi*x) + 0.2*sin(pi*x)",0);
	y.Modify("sin(2*pi*x)",1);	y.Modify("cos(2*pi*x)",2);
	y1.Modify("0.5+0.3*cos(2*pi*x)");
	y2.Modify("0.3*sin(2*pi*x)");
	x.Fill(-1,1,'x');
	x2.Modify("0.05+0.03*cos(2*pi*x)");

	mglData  x0(10), y0(10), ex0(10), ey0(10),ch(7,2);
	x0.Modify("2*x-1 + 0.1*rnd-0.05");
	y0.Modify("0.7*sin(2*pi*x) + 0.5*cos(3*pi*x) + 0.2*sin(pi*x) + 0.2*rnd-0.1");
	ey0.Modify("0.2");	ex0.Modify("0.1");	ch.Modify("rnd+0.1");

	gr->SubPlot(1,1,0);	gr->Alpha(false);
	t1 = time(NULL);
	for(i=0;i<100;i++)	gr->Clf();
	t2 = time(NULL);	printf("Clf: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
//	for(i=0;i<100;i++)	{	gr->Clf();	gr->MeshNum=20;	gr->Dew(a,b,"BbcyrR");	gr->MeshNum=0;}
	t2 = time(NULL);	printf("Dew: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	gr->Rotate(40,60);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Box();	}
	t2 = time(NULL);	printf("Box: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->MeshNum=20;	gr->Dew(a,b,"BbcyrR");	gr->MeshNum=0;}
	t2 = time(NULL);	printf("Dew rotated: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Pipe(a,b,"BbcyrR");	}
	t2 = time(NULL);	printf("Pipe: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Surf(a,"BbcyrR");	}
	t2 = time(NULL);	printf("Surf: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Tile(a,"BbcyrR");	}
	t2 = time(NULL);	printf("Tile: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Belt(a,"BbcyrR");	}
	t2 = time(NULL);	printf("Belt: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Mesh(a,"BbcyrR");	}
	t2 = time(NULL);	printf("Mesh: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Fall(a,"BbcyrR");	}
	t2 = time(NULL);	printf("Fall: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->SurfC(a,b,"BbcyrR");	}
	t2 = time(NULL);	printf("SurfC: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Boxs(a,"BbcyrR");	}
	t2 = time(NULL);	printf("Boxs: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Flow(ex,ey,ez,"bwr");	}
	t2 = time(NULL);	printf("Flow 3d: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Vect(ex,ey,ez,"bwr");	}
	t2 = time(NULL);	printf("Vect 3d: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->VectC(ex,ey,ez,"bwr");	}
	t2 = time(NULL);	printf("VectC 3d: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Pipe(ex,ey,ez,"bwr");	}
	t2 = time(NULL);	printf("Pipe 3d: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Torus(y1,y2,"pz");	}
	t2 = time(NULL);	printf("Torus: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Crust(pnts,"BbcyrR");	}
	t2 = time(NULL);	printf("Crust: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Dots(pnts,"BbcyrR");	}
	t2 = time(NULL);	printf("Dots: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->ContF(a,"BbcyrR");	}
	t2 = time(NULL);	printf("ContF: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Cont(a,"BbcyrR");	}
	t2 = time(NULL);	printf("Cont: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->ContFA(c,"BbcyrR");	}
	t2 = time(NULL);	printf("ContFA: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Fog(1);	gr->Surf(a,"BbcyrR");	gr->Fog(0);	}
	t2 = time(NULL);	printf("Fog: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Tube(y,0.05);	}
	t2 = time(NULL);	printf("Tube: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Chart(ch,"#");	}
	t2 = time(NULL);	printf("Chart: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	gr->Axis("(y+1)/2*cos(pi*x)","(y+1)/2*sin(pi*x)",0);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Chart(ch,"#");	}
	gr->Axis(0,0,0);	gr->Alpha(true);
	t2 = time(NULL);	printf("Pie: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Surf(a,"BbcyrR");	}
	t2 = time(NULL);	printf("Surf transp: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Axial(a,"BbcyrR");	}
	t2 = time(NULL);	printf("Axial: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->SurfA(a,b,"BbcyrR");	}
	t2 = time(NULL);	printf("SurfA: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Map(a,b,"BbcyrR");	}
	t2 = time(NULL);	printf("Map: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Surf3(c,"BbcyrR");	}
	t2 = time(NULL);	printf("Surf3: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Surf3(c,"bgrd");	}
	t2 = time(NULL);	printf("Surf3 rgbd: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Surf3A(c,d,"BbcyrR");	}
	t2 = time(NULL);	printf("Surf3A: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Surf3C(c,d,"BbcyrR");	}
	t2 = time(NULL);	printf("Surf3C: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->ContA(c,"BbcyrR");	}
	t2 = time(NULL);	printf("ContA: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	gr->Light(false);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->CloudQ(c,"wyrRk");	}
	t2 = time(NULL);	printf("CloudQ: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->CloudP(c,"wyrRk");	}
	t2 = time(NULL);	printf("CloudP: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->DensA(c,"BbcyrR");	}
	t2 = time(NULL);	printf("DensA: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	gr->Alpha(false);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Surf3(c,"bgrd");	}
	t2 = time(NULL);	printf("Surf3 rgbd no alpha: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->DensX(c.Sum("x"),"BbcyrR",-1);	gr->DensY(c.Sum("y"),"BbcyrR",1);
	gr->DensZ(c.Sum("z"),"BbcyrR",-1);	}
	t2 = time(NULL);	printf("Dens[XYZ]: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->ContX(c.Sum("x"),"BbcyrR",-1);	gr->ContY(c.Sum("y"),"BbcyrR",1);
	gr->ContZ(c.Sum("z"),"BbcyrR",-1);	}
	t2 = time(NULL);	printf("Cont[XYZ]: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	gr->SubPlot(1,1,0);	// 1D Plots
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Plot(y);	}
	t2 = time(NULL);	printf("Plot: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Area(y);	}
	t2 = time(NULL);	printf("Area: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Stem(y);	}
	t2 = time(NULL);	printf("Stem: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Step(y);	}
	t2 = time(NULL);	printf("Step: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Bars(y);	}
	t2 = time(NULL);	printf("Bars: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Mark(y,y1,"bs");	}
	t2 = time(NULL);	printf("Mark: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->TextMark(y,y1,"\\gamma");	}
	t2 = time(NULL);	printf("TextMark: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Plot(y.SubData(-1,0));
	gr->Text(y,"This is very long string drawn along a curve",":k");
	gr->Text(y,"Another string drawn above a curve","T:r");	}
	t2 = time(NULL);	printf("Text: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Plot(y.SubData(-1,0));	gr->Error(x0,y0,ex0,ey0,"ko");	}
	t2 = time(NULL);	printf("Error: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Dens(a,"BbcyrR");	}
	t2 = time(NULL);	printf("Dens: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Tile(a,b,"BbcyrR");	}
	t2 = time(NULL);	printf("TileR: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Cont(a,"BbcyrRt");	}
	t2 = time(NULL);	printf("Cont: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Vect(a,b,"BbcyrR");	}
	t2 = time(NULL);	printf("Vect: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->VectC(a,b,"BbcyrR");	}
	t2 = time(NULL);	printf("VectC: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Flow(a,b,"BbcyrR");	}
	t2 = time(NULL);	printf("Flow: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	for(i=0;i<100;i++)	{	gr->Clf();	gr->Pipe(a,b,"BbcyrR");	}
	t2 = time(NULL);	printf("Pipe 3d: %g ms\n", 10*difftime(t2,t1));	t1 = time(NULL);
	return 0;
}
//-----------------------------------------------------------------------------
int main(int argc,char **argv)
{
	const char *suf = "";
	mglGraphZB zb;
	mglGraphPS ps;
	mglGraph &gr = zb;
	if(argc>1)
	{
		if(!strcmp(argv[1],"mini"))
		{	gr.SetSize(200,133);	suf = "_sm";	}
		if(!strcmp(argv[1],"big"))
		{
			gr.SetSize(1200,800);	suf = "_lg";
			gr.BaseLineWidth = 2;
		}
	}

	all_samples(&gr,suf);
	sample_transp(&gr,suf);
	sample_hint(&gr,suf);
	full_test(&gr,suf);

/*	printf("\n\n\t\t mglGraphZB (600*400) \n");	fflush(stdout);
	zb.SetSize(600,400);	time_test(&zb,0);
	printf("\n\n\t\t mglGraphZB (1200*800) \n");	fflush(stdout);
	zb.BaseLineWidth = 2;
	zb.SetSize(1200,800);	time_test(&zb,0);
	
	printf("\n\n\t\t mglGraphPS (600*400) \n");	fflush(stdout);
	ps.SetSize(600,400);	time_test(&ps,0);
	printf("\n\n\t\t mglGraphPS (1200*800) \n");	fflush(stdout);
	ps.BaseLineWidth = 2;
	ps.SetSize(1200,800);	time_test(&ps,0);
*/	
	return 0;
}
//-----------------------------------------------------------------------------
