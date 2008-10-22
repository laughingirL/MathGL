#include <time.h>
#include <locale.h>
#include <string.h>
#include <stdio.h>
#include <getopt.h>
#include <mgl/mgl_zb.h>
#include <mgl/mgl_eps.h>
#include <mgl/mgl_idtf.h>
#define IDTF_TYPE 5
int type = IDTF_TYPE;
bool idtf_mode = true;
int pdftransparency = 1;
int vertex_color = 0;
int compression = 1;
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
	case 5:	// IDTF
		sprintf(buf,"%s%s.idtf",name,suf);
		gr->WriteIDTF(buf);	break;
	default:// PNG (no alpha)
		sprintf(buf,"%s%s.png",name,suf);
		gr->WritePNG(buf,0,false);	break;
}
	puts(buf);
}
//-----------------------------------------------------------------------------
int sample_colors(mglGraph *gr)	// arrow styles
{
	gr->Face(mglPoint(-1,  -1), mglPoint(-1, -0.7), mglPoint(-0.6, -1), mglPoint(-0.6, -0.7), "L#");
	gr->Puts(mglPoint(-0.8,-0.9, 0.01), "L", "C:w", -1.4);
	gr->Face(mglPoint(-0.6,-1), mglPoint(-0.6, -0.7), mglPoint(-0.2, -1), mglPoint(-0.2, -0.7), "E#");
	gr->Puts(mglPoint(-0.4,-0.9, 0.01), "E", "C:w", -1.4);
	gr->Face(mglPoint(-0.2,-1), mglPoint(-0.2, -0.7), mglPoint(0.2, -1), mglPoint(0.2, -0.7), "N#");
	gr->Puts(mglPoint( 0,  -0.9, 0.01), "N", "C:w", -1.4);
	gr->Face(mglPoint( 0.2,-1), mglPoint(0.2, -0.7), mglPoint(0.6, -1), mglPoint(0.6, -0.7), "U#");
	gr->Puts(mglPoint( 0.4,-0.9, 0.01), "U", "C:w", -1.4);
	gr->Face(mglPoint( 0.6,-1), mglPoint(0.6, -0.7), mglPoint(1, -1), mglPoint(1, -0.7), "Q#");
	gr->Puts(mglPoint( 0.8,-0.9, 0.01), "Q", "C:w", -1.4);
	//#lenuq
	gr->Face(mglPoint(-1,  -0.7), mglPoint(-1, -0.4), mglPoint(-0.6, -0.7), mglPoint(-0.6, -0.4), "l#");
	gr->Puts(mglPoint(-0.8,-0.6, 0.01), "l", "C:k", -1.4);
	gr->Face(mglPoint(-0.6,-0.7), mglPoint(-0.6, -0.4), mglPoint(-0.2, -0.7), mglPoint(-0.2, -0.4), "e#");
	gr->Puts(mglPoint(-0.4,-0.6, 0.01), "e", "C:k", -1.4);
	gr->Face(mglPoint(-0.2,-0.7), mglPoint(-0.2, -0.4), mglPoint(0.2, -0.7), mglPoint(0.2, -0.4), "n#");
	gr->Puts(mglPoint( 0,  -0.6, 0.01), "n", "C:k", -1.4);
	gr->Face(mglPoint( 0.2,-0.7), mglPoint(0.2, -0.4), mglPoint(0.6, -0.7), mglPoint(0.6, -0.4), "u#");
	gr->Puts(mglPoint( 0.4,-0.6, 0.01), "u", "C:k", -1.4);
	gr->Face(mglPoint( 0.6,-0.7), mglPoint(0.6, -0.4), mglPoint(1, -0.7), mglPoint(1, -0.4), "q#");
	gr->Puts(mglPoint( 0.8,-0.6, 0.01), "q", "C:k", -1.4);
	//#CMYkP
	gr->Face(mglPoint(-1,  -0.4), mglPoint(-1, -0.1), mglPoint(-0.6, -0.4), mglPoint(-0.6, -0.1), "C#");
	gr->Puts(mglPoint(-0.8,-0.3, 0.01), "C", "C:w", -1.4);
	gr->Face(mglPoint(-0.6,-0.4), mglPoint(-0.6, -0.1), mglPoint(-0.2, -0.4), mglPoint(-0.2, -0.1), "M#");
	gr->Puts(mglPoint(-0.4,-0.3, 0.01), "M", "C:w", -1.4);
	gr->Face(mglPoint(-0.2,-0.4), mglPoint(-0.2, -0.1), mglPoint(0.2, -0.4), mglPoint(0.2, -0.1), "Y#");
	gr->Puts(mglPoint( 0,  -0.3, 0.01), "Y", "C:w", -1.4);
	gr->Face(mglPoint( 0.2,-0.4), mglPoint(0.2, -0.1), mglPoint(0.6, -0.4), mglPoint(0.6, -0.1), "k#");
	gr->Puts(mglPoint( 0.4,-0.3, 0.01), "k", "C:w", -1.4);
	gr->Face(mglPoint( 0.6,-0.4), mglPoint(0.6, -0.1), mglPoint(1, -0.4), mglPoint(1, -0.1), "P#");
	gr->Puts(mglPoint( 0.8,-0.3, 0.01), "P", "C:w", -1.4);
	//#cmywp
	gr->Face(mglPoint(-1, -0.1), mglPoint(-1, 0.2), mglPoint(-0.6, -0.1), mglPoint(-0.6, 0.2), "c#");
	gr->Puts(mglPoint(-0.8, 0, 0.01), "c", "C:k", -1.4);
	gr->Face(mglPoint(-0.6,-0.1), mglPoint(-0.6, 0.2), mglPoint(-0.2, -0.1), mglPoint(-0.2, 0.2), "m#");
	gr->Puts(mglPoint(-0.4, 0, 0.01), "m", "C:k", -1.4);
	gr->Face(mglPoint(-0.2,-0.1), mglPoint(-0.2, 0.2), mglPoint(0.2, -0.1), mglPoint(0.2, 0.2), "y#");
	gr->Puts(mglPoint( 0,   0, 0.01), "y", "C:k", -1.4);
	gr->Face(mglPoint( 0.2,-0.1), mglPoint(0.2, 0.2), mglPoint(0.6, -0.1), mglPoint(0.6, 0.2), "w#");
	gr->Puts(mglPoint( 0.4, 0, 0.01), "w", "C:k", -1.4);
	gr->Face(mglPoint( 0.6,-0.1), mglPoint(0.6, 0.2), mglPoint(1, -0.1), mglPoint(1, 0.2), "p#");
	gr->Puts(mglPoint( 0.8, 0, 0.01), "p", "C:k", -1.4);
	//#BGRHW
	gr->Face(mglPoint(-1,   0.2), mglPoint(-1, 0.5), mglPoint(-0.6, 0.2), mglPoint(-0.6, 0.5), "B#");
	gr->Puts(mglPoint(-0.8, 0.3, 0.01), "B", "C:w", -1.4);
	gr->Face(mglPoint(-0.6, 0.2), mglPoint(-0.6, 0.5), mglPoint(-0.2, 0.2), mglPoint(-0.2, 0.5), "G#");
	gr->Puts(mglPoint(-0.4, 0.3, 0.01), "G", "C:w", -1.4);
	gr->Face(mglPoint(-0.2, 0.2), mglPoint(-0.2, 0.5), mglPoint(0.2, 0.2), mglPoint(0.2, 0.5), "R#");
	gr->Puts(mglPoint( 0,   0.3, 0.01), "R", "C:w", -1.4);
	gr->Face(mglPoint( 0.2, 0.2), mglPoint(0.2, 0.5), mglPoint(0.6, 0.2), mglPoint(0.6, 0.5), "H#");
	gr->Puts(mglPoint( 0.4, 0.3, 0.01), "H", "C:w", -1.4);
	gr->Face(mglPoint( 0.6, 0.2), mglPoint(0.6, 0.5), mglPoint(1, 0.2), mglPoint(1, 0.5), "W#");
	gr->Puts(mglPoint( 0.8, 0.3, 0.01), "W", "C:w", -1.4);
	//#bgrhw
	gr->Face(mglPoint(-1,   0.5), mglPoint(-1, 0.8), mglPoint(-0.6, 0.5), mglPoint(-0.6, 0.8), "b#");
	gr->Puts(mglPoint(-0.8, 0.6, 0.01), "b", "C:k", -1.4);
	gr->Face(mglPoint(-0.6, 0.5), mglPoint(-0.6, 0.8), mglPoint(-0.2, 0.5), mglPoint(-0.2, 0.8), "g#");
	gr->Puts(mglPoint(-0.4, 0.6, 0.01), "g", "C:k", -1.4);
	gr->Face(mglPoint(-0.2, 0.5), mglPoint(-0.2, 0.8), mglPoint(0.2, 0.5), mglPoint(0.2, 0.8), "r#");
	gr->Puts(mglPoint( 0,   0.6, 0.01), "r", "C:k", -1.4);
	gr->Face(mglPoint( 0.2, 0.5), mglPoint(0.2, 0.8), mglPoint(0.6, 0.5), mglPoint(0.6, 0.8), "h#");
	gr->Puts(mglPoint( 0.4, 0.6, 0.01), "h", "C:k", -1.4);
	gr->Face(mglPoint( 0.6, 0.5), mglPoint(0.6, 0.8), mglPoint(1, 0.5), mglPoint(1, 0.8), "w#");
	gr->Puts(mglPoint( 0.8, 0.6, 0.01), "w", "C:k", -1.4);
	//#brighted
	gr->Face(mglPoint(-1,   0.8), mglPoint(-1, 1.1), mglPoint(-0.6, 0.8), mglPoint(-0.6, 1.1), "r1#");
	gr->Puts(mglPoint(-0.8, 0.9, 0.01), "r1", "C:w", -1.4);
	gr->Face(mglPoint(-0.6, 0.8), mglPoint(-0.6, 1.1), mglPoint(-0.2, 0.8), mglPoint(-0.2, 1.1), "r3#");
	gr->Puts(mglPoint(-0.4, 0.9, 0.01), "r3", "C:w", -1.4);
	gr->Face(mglPoint(-0.2, 0.8), mglPoint(-0.2, 1.1), mglPoint(0.2, 0.8), mglPoint(0.2, 1.1), "r5#");
	gr->Puts(mglPoint( 0,   0.9, 0.01), "r5", "C:k", -1.4);
	gr->Face(mglPoint( 0.2, 0.8), mglPoint(0.2, 1.1), mglPoint(0.6, 0.8), mglPoint(0.6, 1.1), "r7#");
	gr->Puts(mglPoint( 0.4, 0.9, 0.01), "r7", "C:k", -1.4);
	gr->Face(mglPoint( 0.6, 0.8), mglPoint(0.6, 1.1), mglPoint(1, 0.8), mglPoint(1, 1.1), "r9#");
	gr->Puts(mglPoint( 0.8, 0.9, 0.01), "r9", "C:k", -1.4);
	return 0;
}
//-----------------------------------------------------------------------------
void sample_ae(mglGraph *gr)			// math text
{
	gr->StartGroup("Formula");
	gr->Puts(mglPoint(0), "\\sqrt{\\frac{\\alpha^{\\gamma^2}+\\overset 1{\\big\\infty}}{\\sqrt3{2+b}}}",
			 0, -4);
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void sample_ad(mglGraph *gr)	// arrow styles
{
	gr->VertexColor(false);	gr->Compression(false);	// anything is black anyway and compression is bad for text

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
}
//-----------------------------------------------------------------------------
void sample_ac(mglGraph *gr)	// error boxes
{
	mglData  x(10), y(10), ex(10), ey(10), y0(50);
	x.Modify("2*x-1 + 0.2*rnd-0.1");
	y.Modify("(2*x-1)^2 + 0.2*rnd-0.1");
	y0.Modify("(2*x-1)^2");
	ex.Modify("0.2");
	ey.Modify("0.1");

	gr->SubPlot(2,2,0);
	gr->StartGroup("Random y");
	gr->StartGroup("Random y Error");	gr->Error(y,ey,"ko");				gr->EndGroup();
	gr->StartGroup("Random y Plot");	gr->Plot(y0,"r");				gr->EndGroup();
	gr->StartGroup("Random y Axis");	gr->Axis();					gr->EndGroup();
	gr->StartGroup("Random y Label");	gr->Text(mglPoint(0,1.2,0),"Random {\\i y}");	gr->EndGroup();
	gr->EndGroup();

	gr->SubPlot(2,2,1);
	gr->StartGroup("Random x,y");
	gr->StartGroup("Random x,y Error");	gr->Error(x,y,ex,"ko");					gr->EndGroup();
	gr->StartGroup("Random x,y Plot");	gr->Plot(y0,"r");					gr->EndGroup();
	gr->StartGroup("Random x,y Axis");	gr->Axis();						gr->EndGroup();
	gr->StartGroup("Random x,y Label");	gr->Text(mglPoint(0,1.2,0),"Random {\\i x, y}");	gr->EndGroup();
	gr->EndGroup();

	gr->SubPlot(2,2,2);
	gr->StartGroup("Random x,y and 2d boxes");
	gr->StartGroup("Random x,y and 2d boxes Error");	gr->Error(x,y,ey,ey,"ko");					gr->EndGroup();
	gr->StartGroup("Random x,y and 2d boxes Plot");		gr->Plot(y0,"r");						gr->EndGroup();
	gr->StartGroup("Random x,y and 2d boxes Axis");		gr->Axis();							gr->EndGroup();
	gr->StartGroup("Random x,y and 2d boxes Label");	gr->Text(mglPoint(0,1.2,0),"Random {\\i x, y} and 2d boxes");	gr->EndGroup();
	gr->EndGroup();

	gr->SubPlot(2,2,3);
	gr->StartGroup("Random point in 3d space");
	gr->StartGroup("Random point in 3d space Label");	gr->Text(mglPoint(0,1.2,0),"Random point in 3d space");	gr->EndGroup();
	gr->Rotate(40,60);
	gr->StartGroup("Random point in 3d space Error");
	for(int i=0;i<10;i++)
		gr->Error(mglPoint(mgl_rnd()-0.5,mgl_rnd()-0.5,mgl_rnd()-0.5),
			mglPoint(0.1,0.1,0.1),"bo");
	gr->EndGroup();
	gr->StartGroup("Random point in 3d space Axis");	gr->Axis();	gr->EndGroup();
	gr->EndGroup();
	gr->InPlot(0,1,0,1);
}
//-----------------------------------------------------------------------------
void sample_ab(mglGraph *gr)	// Gaussian beam
{
	gr->VertexColor(false);	gr->Compression(false);	// vertex color prevents transparency (unless set in PDF) and compression is bad it

	gr->Alpha(true);	gr->Light(true);	gr->Light(0,mglPoint(0,0,1));

	mglData a(30,30,30),b(30,30,30);
	a.Modify("exp(-16*((z-0.5)^2+(y-0.5)^2)/(1+4*x^2))");
	b.Modify("16*((z-0.5)^2+(y-0.5)^2)*(x)/(1+4*x^2)");
	gr->CAxis(0,1);

	gr->SubPlot(2,2,0);	gr->Rotate(40,60);
	gr->StartGroup("Surf3 Group");
	gr->StartGroup("Surf3");	gr->Surf3(a,"wgk");	gr->EndGroup();
	gr->StartGroup("Surf3 Box");	gr->Box();		gr->EndGroup();
	gr->EndGroup();

	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1),mglPoint(0,0,0));
	gr->CAxis(0,1);
	// vertex color prevents transparency, unless u3dpreview sets it in PDF for the whole surface
	if (pdftransparency) 
	{
		gr->VertexColor(true);	
		gr->Alpha(false);	// transparency will be set in PDF, not in u3d
		gr->Light(false);	// IMHO lighting is not necessary for this model	
	}
	gr->SubPlot(2,2,1);	gr->Rotate(40,60);
	gr->StartGroup("DensA Group");
	gr->StartGroup("DensA");	gr->DensA(a);		gr->EndGroup();
	gr->StartGroup("DensA Axis");	gr->Axis();		gr->EndGroup();
	gr->StartGroup("DensA Box");	gr->Box();		gr->EndGroup();
	gr->EndGroup();
	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1));
	gr->CAxis(0,1);
	if (pdftransparency)  // put settings back
	{
		gr->Alpha(true);
		gr->Light(true);
	}

	gr->SubPlot(2,2,2);	gr->Rotate(40,60);
	gr->StartGroup("ClodQ Group");
	// Clouds not supported in u3d 
	if ( idtf_mode )
		gr->Text(mglPoint(0,0,0),"no clouds in u3d");
	else
		gr->CloudQ(a);
	gr->StartGroup("CloudQ Box");	gr->Box();		gr->EndGroup();
	gr->EndGroup();

	gr->VertexColor(false);	// for variable transparency the only way is to disable smooth (per-vertex) coloring
	gr->SubPlot(2,2,3);	gr->Rotate(40,60);
	gr->StartGroup("Surf3A Group");
	gr->StartGroup("Surf3A");	gr->Surf3A(b,a,"q");	gr->EndGroup();
	gr->StartGroup("Surf3A Box");	gr->Box();		gr->EndGroup();
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void sample_aa(mglGraph *gr)	// flag #
{
	gr->Alpha(true);	gr->Light(true);	gr->Light(0,mglPoint(0,0,1));
	mglData a(30,20);
	a.Modify("0.6*sin(2*pi*x)*sin(3*pi*y) + 0.4*cos(3*pi*(x*y))");

	gr->SubPlot(2,2,0);	gr->Rotate(40,60);
	// vertex color prevents transparency, unless u3dpreview sets it in PDF for the whole surface
	if (idtf_mode && pdftransparency) 
	{
		gr->VertexColor(true);	
		gr->Alpha(false);	// transparency will be set in PDF, not in u3d
	}
	else
	{
		gr->VertexColor(false);	
	}
	gr->StartGroup("Surf");		gr->Surf(a,"BbcyrR#");	gr->EndGroup();
	gr->StartGroup("Surf Box");	gr->Box();		gr->EndGroup();
	gr->SubPlot(2,2,1);	gr->Rotate(40,60);
	gr->Light(false);	// IMHO lighting is not necessary for this model	
	gr->StartGroup("Dens");		gr->Dens(a,"BbcyrR#");	gr->EndGroup();
	gr->StartGroup("Dens Box");	gr->Box();		gr->EndGroup();
	gr->SubPlot(2,2,2);	gr->Rotate(40,60);
	gr->StartGroup("Cont");		gr->Cont(a,"BbcyrR#");	gr->EndGroup();
	gr->StartGroup("Cont Box");	gr->Box();		gr->EndGroup();
	gr->SubPlot(2,2,3);	gr->Rotate(40,60);
	gr->StartGroup("Axial");	gr->Axial(a,"BbcyrR#");	gr->EndGroup();
	gr->StartGroup("Axial Box");	gr->Box();		gr->EndGroup();
	if (idtf_mode && !pdftransparency) 	// switch lighting back
	{
		gr->Light(true);
		gr->SubPlot(2,2,0);	gr->Rotate(40,60);
	}
}
//-----------------------------------------------------------------------------
void sample_a9(mglGraph *gr)	// 2d plot
{
	gr->Light(true);	gr->Light(0,mglPoint(0,0,1));

	gr->VertexColor(true);	
	mglData a0(50,40);
	a0.Modify("0.6*sin(2*pi*x)*sin(3*pi*y) + 0.4*cos(3*pi*(x*y))");
	gr->SubPlot(2,2,0);	gr->Rotate(60,40);
	gr->Surf(a0);
	gr->StartGroup("UL Box");	gr->Box();		gr->EndGroup();

	mglData x(50,40),y(50,40),z(50,40);
	x.Modify("0.8*sin(2*pi*x)*sin(pi*y)");
	y.Modify("0.8*cos(2*pi*x)*sin(pi*y)");
	z.Modify("0.8*cos(pi*y)");
	gr->SubPlot(2,2,1);	gr->Rotate(60,40);
	gr->StartGroup("Sphere");	gr->Surf(x,y,z,"BbwrR");	gr->EndGroup();
	gr->StartGroup("UR Box");	gr->Box();			gr->EndGroup();

	mglData a1(50,40,3);
	a1.Modify("0.6*sin(2*pi*x)*sin(3*pi*y) + 0.4*cos(3*pi*(x*y))");
	a1.Modify("0.6*cos(2*pi*x)*cos(3*pi*y) + 0.4*sin(3*pi*(x*y))",1);
	a1.Modify("0.6*cos(2*pi*x)*cos(3*pi*y) + 0.4*cos(3*pi*(x*y))",2);
	gr->SubPlot(2,2,2);	gr->Rotate(60,40);
	gr->Compression(false);
	if (idtf_mode && pdftransparency) 
	{
		gr->VertexColor(true);	
		gr->Alpha(false);	// transparency will be set in PDF, not in u3d
	}
	else
	{
		gr->VertexColor(false);	
		gr->Alpha(true);
	}
	gr->Surf(a1);
	gr->StartGroup("LL Box");	gr->Box();		gr->EndGroup();

	gr->SubPlot(2,2,3);	gr->Rotate(60,40);
	if (idtf_mode && pdftransparency)
		gr->Light(false);	// IMHO lighting is not necessary for this model	
	gr->Dens(a1);
	gr->StartGroup("LR Box");	gr->Box();		gr->EndGroup();
	if (idtf_mode && pdftransparency)
		gr->Light(true);
	if (idtf_mode && !pdftransparency)
		{ gr->SubPlot(1,1,0);	gr->Rotate(60,40); }	// Position global light
}
//-----------------------------------------------------------------------------
void sample_a8(mglGraph *gr)	// 1d plot
{
	mglData y0(50);		y0.Modify("sin(pi*(2*x-1))");

	gr->SubPlot(2,2,0);
	gr->Plot(y0);
	gr->StartGroup("UL Box");	gr->Box();		gr->EndGroup();

	mglData y1(50,2);
	y1.Modify("sin(pi*2*x-pi)");	y1.Modify("cos(pi*2*x-pi)/2",1);
	mglData x(50);		x.Modify("cos(pi*2*x-pi)");

	gr->SubPlot(2,2,1);
	gr->Plot(y1);
	gr->Plot(x,y0,"Y+");
	gr->Plot2(y1,"q|");
	gr->StartGroup("UR Box");	gr->Box();		gr->EndGroup();

	mglData z(50);		z.Modify("2*x-1");
	mglData y2(10,3);	y2.Modify("cos(pi*(2*x-1-y))");
	y2.Modify("2*x-1",2);

	gr->SubPlot(2,2,2);	gr->Rotate(60,40);
	gr->Plot(x,y0,z);
	gr->StartGroup("LL Box");	gr->Box();		gr->EndGroup();
	gr->Plot3(y2,"bo ");

	gr->SubPlot(2,2,3);	gr->Rotate(60,40);
	gr->Bars(x,y0,z,"ri");
	gr->StartGroup("LR Box");	gr->Box();		gr->EndGroup();

	gr->InPlot(0,1,0,1);
}
//-----------------------------------------------------------------------------
void sample_a7(mglGraph *gr)	// smoothing
{
	mglData y0(30),y1,y2,y3;
	y0.Modify("0.4*sin(2*pi*x) + 0.3*cos(3*pi*x) - 0.4*sin(4*pi*x)+0.2*rnd");

	y1=y0;	y1.Smooth(SMOOTH_LINE_3);
	y2=y0;	y2.Smooth(SMOOTH_LINE_5);
	y3=y0;	y3.Smooth(SMOOTH_QUAD_5);

	gr->StartGroup("NONE");		gr->Plot(y0,"k");	gr->EndGroup();	gr->AddLegend("NONE","k");
	gr->StartGroup("LINE_3");	gr->Plot(y1,"r");	gr->EndGroup();	gr->AddLegend("LINE\\_3","r");
	gr->StartGroup("LINE_5");	gr->Plot(y2,"g");	gr->EndGroup();	gr->AddLegend("LINE\\_5","g");
	gr->StartGroup("QUAD_5");	gr->Plot(y3,"b");	gr->EndGroup();	gr->AddLegend("QUAD\\_5","b");
	if (idtf_mode)	// Problem in idtf, the white background and tet take the same place
		gr->LegendBox = false;
	gr->StartGroup("Legend");	gr->Legend();	gr->EndGroup();
	gr->StartGroup("Box");		gr->Box();	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void sample_a6(mglGraph *gr)	// differentiate
{
	gr->VertexColor(true);	
	mglData a(30,40);	a.Modify("x*y");
	gr->Axis(mglPoint(0,0,0),mglPoint(1,1,1));

	gr->SubPlot(2,2,0);	gr->Rotate(60,40);
	gr->StartGroup("Function");
	gr->StartGroup("Function Surf");	gr->Surf(a);	gr->EndGroup();
	gr->StartGroup("Function Box");		gr->Box();	gr->EndGroup();
	gr->StartGroup("Function Label");	gr->Text(mglPoint(0.7,1,1.2),"a(x,y)");			gr->EndGroup();
	gr->EndGroup();

	gr->SubPlot(2,2,1);	gr->Rotate(60,40);
	a.Diff("x");
	gr->StartGroup("Diff");
	gr->StartGroup("Diff Surf");		gr->Surf(a);	gr->EndGroup();
	gr->StartGroup("Diff Box");		gr->Box();	gr->EndGroup();
	gr->StartGroup("Diff Label");		gr->Text(mglPoint(0.7,1,1.2),"da/dx");			gr->EndGroup();
	gr->EndGroup();

	gr->SubPlot(2,2,2);	gr->Rotate(60,40);
	a.Integral("xy");
	gr->StartGroup("Integral");
	gr->StartGroup("Integral Surf");	gr->Surf(a);	gr->EndGroup();
	gr->StartGroup("Integral Box");		gr->Box();	gr->EndGroup();
	gr->StartGroup("Integral Label");	gr->Text(mglPoint(0.7,1,1.2),"\\int da/dx dxdy");	gr->EndGroup();
	gr->EndGroup();

	gr->SubPlot(2,2,3);	gr->Rotate(60,40);
	a.Diff2("y");
	gr->StartGroup("Diff2");
	gr->StartGroup("Diff2 Surf");		gr->Surf(a);	gr->EndGroup();
	gr->StartGroup("Diff2 Box");		gr->Box();	gr->EndGroup();
	gr->StartGroup("Diff2 Label");		gr->Text(mglPoint(0.7,1,1.2),"\\int {d^2}a/dxdy dx");	gr->EndGroup();
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void sample_a5(mglGraph *gr)	// pen styles
{
	if (idtf_mode)
		gr->Puts(mglPoint(0,1.2),"line styles not supported","rL");
	gr->Line(mglPoint(0, 1.0,0),mglPoint(0.3, 1.0,0),"k-");	gr->Puts(mglPoint(0.4, 1.0),"Solid '-'","rL");
	gr->Line(mglPoint(0, 0.7,0),mglPoint(0.3, 0.7,0),"k|");	gr->Puts(mglPoint(0.4, 0.7),"Dash '|'","rL");
	gr->Line(mglPoint(0, 0.4,0),mglPoint(0.3, 0.4,0),"k;");	gr->Puts(mglPoint(0.4, 0.4),"Small dash ';'","rL");
	gr->Line(mglPoint(0, 0.1,0),mglPoint(0.3, 0.1,0),"kj");	gr->Puts(mglPoint(0.4, 0.1),"Dash-dot 'j'","rL");
	gr->Line(mglPoint(0,-0.2,0),mglPoint(0.3,-0.2,0),"ki");	gr->Puts(mglPoint(0.4,-0.2),"Small dash-dot 'i'","rL");
	gr->Line(mglPoint(0,-0.5,0),mglPoint(0.3,-0.5,0),"k:");	gr->Puts(mglPoint(0.4,-0.5),"Dots ':'","rL");
	gr->Line(mglPoint(0,-0.8,0),mglPoint(0.3,-0.8,0),"k ");	gr->Puts(mglPoint(0.4,-0.8),"None ' '","rL");

	gr->Mark(mglPoint(-1, 1.2,0),'.');	gr->Puts(mglPoint(-0.7, 1.2),"'.'","rL");
	gr->Mark(mglPoint(-1, 0.9,0),'+');	gr->Puts(mglPoint(-0.7, 0.9),"'+'","rL");
	gr->Mark(mglPoint(-1, 0.6,0),'x');	gr->Puts(mglPoint(-0.7, 0.6),"'x'","rL");
	gr->Mark(mglPoint(-1, 0.3,0),'*');	gr->Puts(mglPoint(-0.7, 0.3),"'*'","rL");
	gr->Mark(mglPoint(-1, 0.0,0),'s');	gr->Puts(mglPoint(-0.7, 0.0),"'s'","rL");
	gr->Mark(mglPoint(-1,-0.3,0),'d');	gr->Puts(mglPoint(-0.7,-0.3),"'d'","rL");
	gr->Mark(mglPoint(-1,-0.6,0),'o');	gr->Puts(mglPoint(-0.7,-0.6),"'o'","rL");
	gr->Mark(mglPoint(-1,-0.9,0),'^');	gr->Puts(mglPoint(-0.7,-0.9),"'\\^'","rL");
	gr->Mark(mglPoint(-1,-1.2,0),'v');	gr->Puts(mglPoint(-0.7,-1.2),"'v'","rL");
}
//-----------------------------------------------------------------------------
void sample_a4(mglGraph *gr)	// font features
{
//	setlocale(LC_CTYPE, "ru_RU.cp1251");
//	gr->Putsw(mglPoint(0,1),L"Text can be in english и в ёникоде");
	gr->Putsw(mglPoint(0,1),L"Text can be in ASCII and in Unicode");
	gr->Puts(mglPoint(0,0.6),"It can be \\wire{wire} and \\big{big}");
	gr->Puts(mglPoint(0,0.2),"One can change style in string: "
		"\\b{bold}, \\i{italic, \\b{both}}");
	gr->Puts(mglPoint(0,-0.2),"Easy to \\a{overline} or "
		"\\u{underline}");
	gr->Puts(mglPoint(0,-0.6),"Easy to change indexes ^{up} _{down} @{center}");
	gr->Puts(mglPoint(0,-1),"It parse TeX: \\int \\alpha \\cdot "
		"\\sqrt3{sin(\\pi x)^2 + \\gamma_{i_k}} dx");
}
//-----------------------------------------------------------------------------
void sample_a3(mglGraph *gr)	// curvilinear coordinates
{
	mglData x(50),y(50),z(50);
	y.Fill(0.5,0.5);
	x.Fill(-1,1);           // create data arrays

	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1),mglPoint(-1,1,-1));
	gr->dx = gr->dy = -5;	gr->dz = 0.5;  // set tick step to 0.5

	gr->SubPlot(2,2,0);
	gr->StartGroup("Cartesian");
	gr->Rotate(60,40);
	gr->StartGroup("Cartesian Plot");	gr->Plot(x,y,z,"r2");				gr->EndGroup();
	gr->StartGroup("Cartesian Axis");	gr->Axis();				gr->EndGroup();
	gr->StartGroup("Cartesian Grid");	gr->Grid();				gr->EndGroup();
	gr->StartGroup("Cartesian Label");	gr->Text(mglPoint(0,1.3,1),"Cartesian");				gr->EndGroup();
	gr->EndGroup();

	gr->SubPlot(2,2,1);
	gr->StartGroup("Cylindrical");
	gr->Axis("y*sin(pi*x)","y*cos(pi*x)",0);
	gr->Rotate(60,40);
	gr->StartGroup("Cylindrical Plot");	gr->Plot(x,y,z,"r2");				gr->EndGroup();
	gr->StartGroup("Cylindrical Axis");	gr->Axis();					gr->EndGroup();
	gr->StartGroup("Cylindrical Grid");	gr->Grid();					gr->EndGroup();
	gr->StartGroup("Cylindrical Label");	gr->Text(mglPoint(0,1.3,1),"Cylindrical");	gr->EndGroup();
	gr->EndGroup();

	gr->SubPlot(2,2,2);
	gr->StartGroup("Parabolic");
	gr->Rotate(60,40);
	gr->Axis("2*y*x","y*y - x*x",0);
	gr->StartGroup("Parabolic Plot");	gr->Plot(x,y,z,"r2");				gr->EndGroup();
	gr->StartGroup("Parabolic Axis");	gr->Axis();					gr->EndGroup();
	gr->StartGroup("Parabolic Grid");	gr->Grid();					gr->EndGroup();
	gr->StartGroup("Parabolic Label");	gr->Text(mglPoint(0,1.3,1),"Parabolic");	gr->EndGroup();
	gr->EndGroup();

	gr->SubPlot(2,2,3);
	gr->StartGroup("Spiral");
	gr->Rotate(60,40);
	gr->Axis("y*sin(pi*x)","y*cos(pi*x)","x+z");
	gr->StartGroup("Spiral Plot");	gr->Plot(x,y,z,"r2");					gr->EndGroup();
	gr->StartGroup("Spiral Axis");	gr->Axis();						gr->EndGroup();
	gr->StartGroup("Spiral Grid");	gr->Grid();						gr->EndGroup();
	gr->StartGroup("Spiral Label");	gr->Text(mglPoint(0,1.3,1),"Spiral");			gr->EndGroup();
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void sample_a2(mglGraph *gr)	// axis and grid
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
	gr->Label('x',"x");	gr->Flush();
	gr->Label('y',"y");	gr->Flush();
	gr->Label('z',"z");	gr->Flush();
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
	gr->Label('y',"Y axis",0);	gr->Flush();
	gr->Label('z',"Z axis",0);	gr->Flush();
	gr->Text(mglPoint(0,0,1.5),"Remove X axis, and","rC");
	gr->Text(mglPoint(0,0,1.2),"decrease number of ticks");

	gr->InPlot(0,1,0,1);
}
//-----------------------------------------------------------------------------
void sample_a1(mglGraph *gr)	// transformation
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

	gr->InPlot(0,1,0,1);
}

#define START(vertex_color)	gr->Clf(); gr->VertexColor(vertex_color); \
		gr->StartGroup("Box"); gr->Box(); gr->EndGroup();
#define SAVE(name)		save(gr,#name,  suf,type);

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

	gr->SubPlot(1,1,0);
	gr->Alpha(false); gr->Compression(compression);
	START(false); gr->MeshNum=20; gr->Dew(a,b,"BbcyrR");	gr->MeshNum=0;	SAVE(dew);
	gr->Rotate(40,60);
	START(false); gr->MeshNum=10; gr->Dew(a,b,"BbcyrR");	gr->MeshNum=0;	SAVE(dew2);
	START(false);		gr->Pipe(a,b,"BbcyrR");		SAVE(pipe);
	START(true);		gr->Surf(a,"BbcyrR");		SAVE(surf);
	START(true);		gr->Surf(a,"BbcyrR|");		SAVE(surf_sl);
	START(true);		gr->Tile(a,"BbcyrR");		SAVE(tile);
	START(true);		gr->Belt(a,"BbcyrR");		SAVE(belt);
	START(false); 		gr->Mesh(a,"BbcyrR");		SAVE(mesh);
	START(false); 		gr->Fall(a,"BbcyrR");		SAVE(fall);
	START(true);		gr->SurfC(a,b,"BbcyrR");	SAVE(surfc);
	START(true);		gr->Boxs(a,"BbcyrR");		SAVE(boxs);
	START(false); 		gr->Flow(ex,ey,ez,"bwr");	SAVE(flow3);
	START(false); 		gr->Vect(ex,ey,ez,"bwr");	SAVE(vect3);
	START(false); 		gr->VectC(ex,ey,ez,"bwr");	SAVE(vectc3);
	START(false);		gr->Pipe(ex,ey,ez,"bwr");	SAVE(pipe3);
	START(false); 		gr->Torus(y1,y2,"pz");		SAVE(torus);
	START(false); 		gr->Crust(pnts,"BbcyrR");	SAVE(crust);
	START(false); 		gr->Dots(pnts,"BbcyrR");	SAVE(dots);
	START(false); 		gr->ContF(a,"BbcyrR");		SAVE(contf);
	START(false); 		gr->Cont(a,"BbcyrR");		SAVE(cont);
	START(false);		gr->ContFA(c,"BbcyrR");		SAVE(contfa);
	if (idtf_mode)
	{ START(false);	gr->Text(mglPoint(0,0,0),"Fog not supported");		SAVE(surf_fog); }
	else
	{ gr->Clf(); gr->Fog(1); gr->Box(); gr->Surf(a,"BbcyrR"); gr->Fog(0);	SAVE(surf_fog); }
	START(false);	gr->Tube(y,0.05); gr->Tube(y1,x,y2,x2);	SAVE(tube);
	START(false);	gr->Tube(y,0.05); gr->Tube(y1,x,y2,x2);	SAVE(tube_3d);
	START(false);		gr->Chart(ch,"#");		SAVE(chart);
	gr->Axis("(y+1)/2*cos(pi*x)","(y+1)/2*sin(pi*x)",0);
	START(false);		gr->Chart(ch,"bgr cmy#");	SAVE(pie_chart);	gr->Axis(0,0,0);

	gr->Alpha(true); gr->Compression(false);
	START(pdftransparency);	gr->Surf(a,"BbcyrR");		SAVE(surf_alpha);
	START(false); 		gr->Axial(a,"BbcyrR");		SAVE(axial);
	START(false);		gr->SurfA(a,b,"BbcyrR");	SAVE(surfa);
	START(false); 		gr->Map(a,b,"brg");		SAVE(map);
	START(false); 		gr->Surf3(c,"BbcyrR");		SAVE(surf3);
	START(pdftransparency);	gr->Surf3(c,"bgrd");		SAVE(surf3_rgbd_light);
	START(false);		gr->Surf3A(c,d,"BbcyrR");	SAVE(surf3a);
	START(pdftransparency);	gr->Surf3C(c,d,"BbcyrR");	SAVE(surf3c);
	START(false);		gr->ContA(c,"BbcyrR");		SAVE(conta);
	gr->Light(false);
	if (idtf_mode)
	{ START(false);	gr->Text(mglPoint(0,0,0),"CloudQ not supported");	SAVE(cloudq); }
	else
	{ START(false);		gr->CloudQ(c,"wyrRk");		SAVE(cloudq); }
	if (idtf_mode)
	{ START(false);	gr->Text(mglPoint(0,0,0),"CloudP not supported");	SAVE(cloudp); }
	else
	{ START(false);		gr->CloudP(c,"wyrRk");		SAVE(cloudp); }
	START(pdftransparency); gr->Axis();	gr->DensA(c,"BbcyrR");	SAVE(densa);
	gr->Alpha(false); gr->Compression(compression);
	START(true);		gr->Surf3(c,"bgrd");		SAVE(surf3_rgbd);
	START(true);	gr->DensX(c.Sum("x"),"BbcyrR",-1); gr->DensY(c.Sum("y"),"BbcyrR",1);
			gr->DensZ(c.Sum("z"),"BbcyrR",-1);			SAVE(dens_xyz);
	START(false);	gr->ContX(c.Sum("x"),"BbcyrR",-1); gr->ContY(c.Sum("y"),"BbcyrR",1);
			gr->ContZ(c.Sum("z"),"BbcyrR",-1);			SAVE(cont_xyz);
	gr->SubPlot(1,1,0);	// 1D Plots
	START(false);	gr->Plot(y);						SAVE(plot);
	START(false);	gr->Area(y);						SAVE(area);
	START(false);	gr->Stem(y);						SAVE(stem);
	START(false);	gr->Step(y);						SAVE(step);
	START(false);	gr->Bars(y);						SAVE(bars);
	START(false);	gr->Barh(y);						SAVE(barh);
	START(false);	gr->Region(y2,y1,"r");	gr->Plot(y1,"k2");	gr->Plot(y2,"k2");	SAVE(region);
	START(false);	gr->Mark(y,y1,"bs");					SAVE(mark);
	START(false);	gr->TextMark(y,y1,"\\gamma");				SAVE(textmark);
	START(false);	gr->Plot(y.SubData(-1,0));
			gr->Text(y,"This is very very long string drawn along a curve",":k");
			gr->Text(y,"Another string drawn above a curve","T:r");	SAVE(text);
	START(false); gr->Plot(y.SubData(-1,0)); gr->Error(x0,y0,ex0,ey0,"ko");	SAVE(error);

	START(true);	gr->Dens(a,"BbcyrR");	gr->Colorbar();			SAVE(dens);
	START(true);	gr->Tile(a,b,"BbcyrR");					SAVE(tiler);
	START(false);	gr->Cont(a,"BbcyrRt");					SAVE(contt);
	START(false);	gr->Vect(a,b,"BbcyrR");					SAVE(vect);
	START(false);	gr->VectC(a,b,"BbcyrR");				SAVE(vectc);
	START(false);	gr->Flow(a,b,"BbcyrR");					SAVE(flow);
	START(true);	gr->Pipe(a,b,"BbcyrR");					SAVE(pipe2);

	f.Modify("sin(2*pi*x*x)",0);	f.Modify("sin(2*pi*x)",1);	f.Modify("sin(2*pi*sqrt(x))",2);
	gr->Axis(mglPoint(0,-1,0),mglPoint(1,1,1));
	START(false);	gr->Plot(f);	gr->Axis("xy");
	if (idtf_mode)	gr->LegendBox = false;
	gr->AddLegend("sin(\\pi {x^2})","b");	gr->AddLegend("sin(\\pi x)","g*");	gr->AddLegend("sin(\\pi \\sqrt{x})","r+");
	gr->Legend();	SAVE(legend);
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
	save(gr,"type0",suf,type);

	gr->TranspType = 1;	gr->Clf();
	gr->SubPlot(2,2,0);	gr->Rotate(40,60);	gr->Surf(a);	gr->Box();
	gr->SubPlot(2,2,1);	gr->Rotate(40,60);	gr->Dens(a);	gr->Box();
	gr->SubPlot(2,2,2);	gr->Rotate(40,60);	gr->Cont(a);	gr->Box();
	gr->SubPlot(2,2,3);	gr->Rotate(40,60);	gr->Axial(a);	gr->Box();
	save(gr,"type1",suf,type);

	gr->TranspType = 2;	gr->Clf();
	gr->SubPlot(2,2,0);	gr->Rotate(40,60);	gr->Surf(a);	gr->Box();
	gr->SubPlot(2,2,1);	gr->Rotate(40,60);	gr->Dens(a);	gr->Box();
	gr->SubPlot(2,2,2);	gr->Rotate(40,60);	gr->Cont(a);	gr->Box();
	gr->SubPlot(2,2,3);	gr->Rotate(40,60);	gr->Axial(a);	gr->Box();
	save(gr,"type2",suf,type);
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
	SAVE(2_axis);

	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1));
	// contours under mesh
	START(true);	gr->Flow(a,b,"br",5,true,-0.99);	gr->Dens(d,"BbcyrR");
	SAVE(flow_dens);

	gr->Rotate(40,60);
	// contours on the surface
	START(true);	gr->Surf(a,"kw");	gr->CAxis(-1,0);	gr->Cont(a,"w");
	gr->CAxis(0,1);	gr->Cont(a,"k");	gr->CAxis(-1,1);
	SAVE(surf_cont);
	gr->Light(true);
	// contours under mesh
	START(true);	gr->Mesh(a,"BbcyrR");	gr->Cont(a, 0, 7, -1);
	SAVE(mesh_cont);
	// contours on the surface
	START(true);	gr->Surf(a,"BbcyrR");	gr->Cont(a,"y");
	SAVE(surf_cont_y);
	// caxis and the surface
	START(true);	gr->CAxis(0,1);	gr->Surf(a,"BbcyrR");	gr->CAxis(-1,1);
	SAVE(surf_caxis);
	// caxis and the surface
	START(true);	gr->Axis(mglPoint(-1,-1,0),mglPoint(1,1,1));	gr->Cut=false;
	gr->Surf(a,"BbcyrR");	gr->Axis(mglPoint(-1,-1,-1),mglPoint(1,1,1));
	SAVE(surf_cut);	gr->Cut = true;

	// several light sources
	START(false);
	gr->Light(1,mglPoint(0,1,0),'c');
	gr->Light(2,mglPoint(1,0,0),'y');
	gr->Light(3,mglPoint(0,-1,0),'m');
	gr->Surf(a,"h");
	SAVE(several_light);
	gr->Light(1,false);
	gr->Light(2,false);
	gr->Light(3,false);

	// CutMin CutMax example
	gr->CutMin = mglPoint(0,-1,-1);	gr->CutMax = mglPoint(1,0,1.1);
	START(false);	gr->Surf3(-0.5,c,"BbcyrR");
	mglData v(10);	v.Fill(-0.5,1);
	gr->ContF3(v,c,'x',-1,"BbcyrR");	gr->ContF3(v,c,'y',-1,"BbcyrR");
	gr->ContF3(v,c,'z',0,"BbcyrR");		gr->ContF3(v,c,'z',39,"BbcyrR");
	SAVE(cutminmax2);

	START(false);	gr->Alpha(true);
	gr->Surf3(c,"BbcyrR");
	SAVE(cutminmax);
	gr->CutMax = gr->CutMin;	// switch off cutting area

	START(false);
	gr->CutOff("(z>(x+0.5*y-1)^2-1) & (z>(x-0.5*y-1)^2-1)");
	gr->Surf3(c);	gr->CutOff(0);
	SAVE(surf3_cutoff);

	return 0;
}
//-----------------------------------------------------------------------------
void sample_molecule(mglGraph *gr)
{
	gr->VertexColor(false);	gr->Compression(false); // per-vertex colors and compression are detrimental to transparency

	gr->Alpha(true);	gr->Light(true);

	gr->SubPlot(2,2,0);
	gr->StartGroup("Methane");
	gr->StartGroup("MetaneLabel");
	gr->Text(mglPoint(0,1.2),"Methane, CH_4",0,-3);
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
	gr->Text(mglPoint(0,1.2),"Water, H{_2}O",0,-3);
	gr->EndGroup();
	gr->Rotate(60,100);
	gr->StartGroup("Water_O");
	gr->Sphere(mglPoint(0,0,0),0.25,"r");
	gr->EndGroup();
	gr->StartGroup("Water_Cloud_1");
	gr->Drop(mglPoint(0,0,0),mglPoint(0.3,0.5,0),0.3,"m",1,2);
	gr->EndGroup();
	gr->StartGroup("Water_H_1");
	gr->Sphere(mglPoint(0.3,0.5,0),0.25,"g");
	gr->EndGroup();
	gr->StartGroup("Water_Cloud_2");
	gr->Drop(mglPoint(0,0,0),mglPoint(0.3,-0.5,0),0.3,"m",1,2);
	gr->EndGroup();
	gr->StartGroup("Water_H_2");
	gr->Sphere(mglPoint(0.3,-0.5,0),0.25,"g");
	gr->EndGroup();
	gr->EndGroup();

	gr->SubPlot(2,2,2);
	gr->StartGroup("Oxygen");
	gr->StartGroup("OxygenLabel");
	gr->Text(mglPoint(0,1.2),"Oxygen, O_2",0,-3);
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
	gr->Text(mglPoint(0,1.2),"Ammonia, NH_3",0,-3);
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

	gr->InPlot(0,1,0,1);
}
//-----------------------------------------------------------------------------
void sample_ternary(mglGraph *gr)
{
	gr->Light(false);	gr->Alpha(false);

	mglData x(50),y(50),rx(10),ry(10), a(20,30);
	a.Modify("4*x*y");
	x.Modify("0.25*(1+cos(2*pi*x))");
	y.Modify("0.25*(1+sin(2*pi*x))");
	rx.Modify("rnd"); ry.Modify("(1-v)*rnd",rx);

	gr->StartGroup("Ternary plot (a+b+c=1)");	gr->Text(mglPoint(0,1.3), "Ternary plot (a+b+c=1)");	gr->EndGroup();
	gr->Ternary(true);
	gr->StartGroup("Plot r2");	gr->Plot(x,y,"r2");		gr->EndGroup();
	gr->StartGroup("Plot q^ ");	gr->Plot(rx,ry,"q^ ");		gr->EndGroup();
	gr->StartGroup("Cont");		gr->Cont(a,"BbcyrR");		gr->EndGroup();
	gr->StartGroup("Line");		gr->Line(mglPoint(0.5,0), mglPoint(0,0.75), "g2");	gr->EndGroup();
	gr->StartGroup("Axis");		gr->Axis();			gr->EndGroup();
	gr->StartGroup("Grid");		gr->Grid("xyz","B;");		gr->EndGroup();
	gr->StartGroup("a comp.");	gr->Label('x',"a comp.", 0);	gr->EndGroup();
	gr->StartGroup("b comp.");	gr->Label('y',"b comp.", 0);	gr->EndGroup();
	gr->StartGroup("c comp.");	gr->Label('z',"c comp.", 0);	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void sample_drops(mglGraph *gr)
{
	gr->Light(true);	gr->Alpha(false);

	gr->StartGroup("sh=0");
		gr->StartGroup("text sh=0");	gr->Text(mglPoint(-1,1.2),"sh=0","rC",-1);		gr->EndGroup();
		gr->StartGroup("drop sh=0");	gr->Drop(mglPoint(-1,0),mglPoint(0,1),0.5,"r",0);	gr->EndGroup();
	gr->EndGroup();
	gr->StartGroup("sh=33");
		gr->StartGroup("text sh=33");	gr->Text(mglPoint(-0.33,1.2),"sh=0.33","rC",-1);	gr->EndGroup();
		gr->StartGroup("drop sh=33");	gr->Drop(mglPoint(-0.33,0),mglPoint(0,1),0.5,"r",0.33);	gr->EndGroup();
	gr->EndGroup();
	gr->StartGroup("sh=67");
		gr->StartGroup("text sh=67");	gr->Text(mglPoint(0.33,1.2),"sh=0.67","rC",-1);		gr->EndGroup();
		gr->StartGroup("drop sh=67");	gr->Drop(mglPoint(0.33,0),mglPoint(0,1),0.5,"r",0.67);	gr->EndGroup();
	gr->EndGroup();
	gr->StartGroup("sh=1");
		gr->StartGroup("text sh=1");	gr->Text(mglPoint(1,1.2),"sh=1","rC",-1);		gr->EndGroup();
		gr->StartGroup("drop sh=1");	gr->Drop(mglPoint(1,0),mglPoint(0,1),0.5,"r",1);	gr->EndGroup();
	gr->EndGroup();
	gr->StartGroup("Balls");
		gr->Ball(mglPoint(-1,0,1),'k');
		gr->Ball(mglPoint(-0.33,0,1),'k');
		gr->Ball(mglPoint(0.33,0,1),'k');
		gr->Ball(mglPoint(1,0,1),'k');
	gr->EndGroup();
	gr->StartGroup("Line");
		gr->Line(mglPoint(-1,0,1),mglPoint(1,0,1),"b");
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void sample_fish(mglGraph *gr)
{
	gr->Light(true);	gr->Alpha(false);

	gr->Axis(mglPoint(0,0),mglPoint(1.1,1.1));

	mglData a(10,10), b(10,10), x(10,10), y(10,10);
	a.Modify("0.3*x+rnd"); x.Modify("x+0.1*rnd");
	b.Modify("0.3*y+rnd"); y.Modify("y+0.1*rnd");

	gr->Dew(x,y,a,b,"BbcyrR");
}
//-----------------------------------------------------------------------------
void sample_mirror(mglGraph *gr)
{
	gr->Light(true);	gr->Alpha(false);

	mglData a(30,40),x(30),y1(40),y2(40);
	a.Modify("pi*(1-2*x)*exp(-4*y^2-4*(2*x-1)^2)");
	x.Fill(-1,1); y1.Fill(0,1); y2.Fill(0,-1);

	gr->Rotate(40,60);
	gr->StartGroup("Box");	gr->Box();	gr->EndGroup();
	gr->StartGroup("Surf");
		gr->StartGroup("RedSurf");	gr->Surf(x,y1,a,"r");	gr->EndGroup();
		gr->StartGroup("BlueSurf");	gr->Surf(x,y2,a,"b");	gr->EndGroup();
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void sample_semilog(mglGraph *gr)
{
	gr->Light(false);	gr->Alpha(false);

	mglData x(2000), y(2000);
	x.Modify("0.01/(x+10^(-5))"); y.Modify("sin(1/v)",x);

	gr->Axis(mglPoint(0.01,-1),mglPoint(1000,1),mglPoint(0.01,-1));
	gr->Axis("lg(x)",0,0); gr->dx = 0;

	gr->StartGroup("Box");		gr->Box();				gr->EndGroup();
	gr->StartGroup("Plot");		gr->Plot(x,y,"b2");			gr->EndGroup();
	gr->StartGroup("Axis");		gr->Axis();				gr->EndGroup();
	gr->StartGroup("Grid");		gr->Grid("xy","g");			gr->EndGroup();
	gr->StartGroup("x");		gr->Label('x',"x",0);			gr->EndGroup();
	gr->StartGroup("y = sin 1/x");	gr->Label('y', "y = sin 1/x",0);	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void sample_loglog(mglGraph *gr)
{
// Problem!
	gr->Light(false);	gr->Alpha(false);

	mglData x(2000), y(2000);
	x.Modify("pow(10,6*x-3)"); y.Modify("sqrt(1+v^2)",x);

	gr->Axis(mglPoint(0.001,0.1),mglPoint(1000,1000),mglPoint(0.001,0.1));
	gr->Axis("lg(x)","lg(y)",0); gr->dx = gr->dy = 0;

	gr->StartGroup("Box");			gr->Box();				gr->EndGroup();
	gr->StartGroup("Plot");			gr->Plot(x,y,"b2");			gr->EndGroup();
	gr->StartGroup("Axis");			gr->Axis();				gr->EndGroup();
	gr->StartGroup("Grid");			gr->Grid("xy","g");			gr->EndGroup();
	gr->StartGroup("x");			gr->Label('x',"x",0);			gr->EndGroup();
	gr->StartGroup("y=(1+x^2)^(1/2)");	gr->Label('y', "y=\\sqrt{1+x^2}",0);	gr->EndGroup();
}
//-----------------------------------------------------------------------------
int sample_fit(mglGraph *gr)
{
	mglData rnd(100), in(100), res;
	rnd.Modify("0.4*rnd+0.1+sin(4*pi*x)", 0);
	in.Modify("0.3+sin(4*pi*x)", 0);
	
	gr->Axis(mglPoint(-1,-2), mglPoint(1,2));
	gr->StartGroup("data");	gr->Plot(rnd, ". ");				gr->EndGroup();
	gr->StartGroup("Box");	gr->Box();					gr->EndGroup();
	
	float ini[3] = {1,1,3};
	gr->Fit(res, rnd, "a+b*sin(c*x)", "abc", ini);
	gr->StartGroup("fitted trendline");	gr->Plot(res, "r");		gr->EndGroup();
	gr->StartGroup("real trendline");	gr->Plot(in, "b");		gr->EndGroup();
	gr->StartGroup("fitted equation");
	gr->Text(mglPoint(-1, -1.3), "fitted:", "L:r", -1);
	gr->PutsFit(mglPoint(0, -1.8), "y = ", "C:r", -1);
	gr->EndGroup();
	gr->StartGroup("initial equation");	gr->Text(mglPoint(0, 2.2), "initial: y = 0.3+sin(2\\pi x)", "C:b", -1);	gr->EndGroup();

	return 0;
}
//-----------------------------------------------------------------------------
#define SAMPLE( FUNC, FNAME) \
	gr->DefaultPlotParam();	gr->Compression(compression);	gr->VertexColor(vertex_color); \
	sample_ ## FUNC(gr);		save(gr,#FNAME,suf,type);
	
int all_samples(mglGraph *gr, const void *s)
{
	const char *suf = (const char *)s;
	gr->DefaultPlotParam();	gr->Compression(false);	gr->VertexColor(false);
	sample_colors(gr);	save(gr,"colors",suf,type);
	gr->SetScheme("BbcyrR");

	SAMPLE(a1,	sample1);
	SAMPLE(a2,	sample2);
	SAMPLE(a3,	sample3);
	SAMPLE(a4,	sample4);
	SAMPLE(a5,	sample5);
	SAMPLE(a6,	sample6);
	SAMPLE(a7,	sample7);
	SAMPLE(a8,	sample8);
	SAMPLE(a9,	sample9);
	SAMPLE(aa,	samplea);
	SAMPLE(ab,	sampleb);
	SAMPLE(ac,	samplec);
	SAMPLE(ad,	sampled);
	SAMPLE(ae,	samplee);
	SAMPLE(molecule,molecule);
	SAMPLE(ternary,	ternary);
	SAMPLE(fit,	fit);
	SAMPLE(drops,	drops);
	SAMPLE(fish,	fish);
	SAMPLE(semilog,	semilog);
	SAMPLE(loglog,	loglog);
	SAMPLE(mirror,	mirror);

	gr->DefaultPlotParam();	gr->Compression(compression);	gr->VertexColor(vertex_color);

	return 0;
}
#undef SAMPLE

static struct option longopts[] = {
	{ "idtf",			no_argument,	&type,			IDTF_TYPE },
	{ "png",			no_argument,	&type,			0 },
	{ "enable-vertex-color",	no_argument,	&vertex_color,		1 },
	{ "disable-vertex-color",	no_argument,	&vertex_color,		0 },
	{ "enable-compression",		no_argument,	&compression,		1 },
	{ "disable-compression",	no_argument,	&compression,		0 },
	{ "enable-pdf-transparency",	no_argument,	&pdftransparency,	1 },
	{ "disable-pdf-transparency",	no_argument,	&pdftransparency,	0 },
	{ "help",			no_argument,	NULL,			'h' },
	{ NULL,					0,	NULL,			0 }
};

void usage()
{
	puts(													 \
	"idtf			- output idtf\n"								 \
	"png			- output png\n"								 \
	" other options affect idtf mode only and take form enable-smth or disable-smth\n"			 \
	"vertex-color		- use by default per-vertex coloring or not\n"					 \
	"compression		- use by default compression or not\n"						 \
	"pdf-transparency	- assume that the pdf making software can set transparency for surfaces\n"	 \
	);
	exit(0);
};


//-----------------------------------------------------------------------------
int main(int argc,char **argv)
{
	const char *suf = "";
	int ch;
	mglGraphIDTF u3d;
	mglGraphZB zb;
	mglGraph *gr = NULL;
	while ((ch = getopt_long_only(argc, argv, "", longopts, NULL)) != -1)
		switch (ch) {
		case 0:
			break;
		case 'h':
		default:
			usage();
		}

	if (type==IDTF_TYPE) 
	{
		gr = &u3d;
		idtf_mode = true;
	}
	else
	{
		gr = &zb;
		idtf_mode = false;
		gr->SetSize(300,300);
	}

//	gr.BaseLineWidth = 2;
	all_samples(gr,suf);
	sample_transp(gr,suf);
	u3d.unrotate_flag = true;
	sample_hint(gr,suf);
	u3d.unrotate_flag = false;
	gr->Org = mglPoint(0,0,0);
	full_test(gr,suf);

	return 0;
}
//-----------------------------------------------------------------------------
