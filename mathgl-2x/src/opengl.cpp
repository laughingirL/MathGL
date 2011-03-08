#ifdef WIN32
#include <windows.h>
#endif
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include "mgl/canvas_gl.h"
//-----------------------------------------------------------------------------
/// Create mglGraph object in OpenGL mode.
HMGL mgl_create_graph_gl()
{	return new mglCanvasGL;	}
/// Create mglGraph object in OpenGL mode.
uintptr_t mgl_create_graph_gl_()
{	return uintptr_t(new mglCanvasGL);	}
//-----------------------------------------------------------------------------
mglCanvasGL::mglCanvasGL() : mglCanvas(1,1)	{}
//-----------------------------------------------------------------------------
mglCanvasGL::~mglCanvasGL(){}
//-----------------------------------------------------------------------------
bool mglCanvasGL::Alpha(bool enable)
{
	bool t = UseAlpha;
	if(enable)
	{
		UseAlpha = true;
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		if(TranspType==1)	glBlendFunc(GL_DST_COLOR, GL_ZERO);
//		else if(TranspType==2) glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		else glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		UseAlpha = false;
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
	}
	return t;
}
//-----------------------------------------------------------------------------
void mglCanvasGL::AddLight(int n,mglPoint p,char cc, float br,bool infty,float ap)
{
	mglColor c(cc);
	mglColor AmbLight = mglColor(AmbBr,AmbBr,AmbBr);
	mglColor DifLight = mglColor(br,br,br);
	GLenum light[8] = {GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4,
			GL_LIGHT5,GL_LIGHT6,GL_LIGHT7};
	float amb[4], pos[4],dif[4];
	if(n<0 || n>7)	{	SetWarn(mglWarnLId);	return;	}
	if(c.Valid())
	{
		DifLight = c*br;
		AmbLight = c*AmbBr;
	}
	dif[0] = DifLight.r;	dif[1] = DifLight.g;
	dif[2] = DifLight.b;	dif[3] = 1.;
	amb[0] = AmbLight.r;	amb[1] = AmbLight.g;
	amb[2] = AmbLight.b;	amb[3] = 1.;
	pos[0] = p.x;		pos[1] = p.y;
	pos[2] = p.z;		pos[3] = infty ? 0:1;
	glShadeModel(GL_SMOOTH);
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 5.0);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, pos);

	glLightfv(light[n], GL_AMBIENT, amb);
	glLightfv(light[n], GL_DIFFUSE, dif);
	//glLightfv(light[n], GL_SPECULAR, spc);
	glLightfv(light[n], GL_POSITION, pos);
	glEnable(light[n]);
}
//-----------------------------------------------------------------------------
void mglCanvasGL::Light(int n, bool enable)
{
	GLenum light[8] = {GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4,
			GL_LIGHT5,GL_LIGHT6,GL_LIGHT7};
	if(enable)	glEnable(light[n]);
	else		glDisable(light[n]);
}
//-----------------------------------------------------------------------------
bool mglCanvasGL::Light(bool enable)
{
	bool t = UseLight;
	UseLight = enable;
	if(enable)	{	glEnable(GL_LIGHTING);	glEnable(GL_NORMALIZE);}
	else		{	glDisable(GL_LIGHTING);	glDisable(GL_NORMALIZE);	}
	return t;
}
//-----------------------------------------------------------------------------
void mglCanvasGL::LightScale()
{
	mglCanvas::LightScale();
	GLenum light[8] = {GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4,
			GL_LIGHT5,GL_LIGHT6,GL_LIGHT7};
	float pos[4]={0,0,0,0};
	for(int i=0;i<8;i++)
	{
		pos[0] = pLight[3*i];
		pos[1] = pLight[3*i+1];
		pos[2] = pLight[3*i+2];
		if(nLight[i])	glLightfv(light[i], GL_POSITION, pos);
	}
}
//-----------------------------------------------------------------------------
void mglCanvasGL::View(float TetX,float TetY,float TetZ)
{
	glMatrixMode(GL_PROJECTION);//GL_PROJECTION GL_VIEWPORT GL_MODELVIEW
	glRotated(TetX,1.,0.,0.);
	glRotated(TetY,0.,1.,0.);
	glRotated(TetZ,0.,0.,1.);
}
//-----------------------------------------------------------------------------
void mglCanvasGL::Fog(float , float)
{
/*	if(d>0)		// TODO: Add fog to OpenGL mode
	{
		glFogf(GL_FOG_MODE,GL_EXP);
		glFogf(GL_FOG_DENSITY,5*d);
		glFogfv(GL_FOG_COLOR,back);
		glEnable(GL_FOG);
	}
	else	glDisable(GL_FOG);*/
}
//-----------------------------------------------------------------------------
void mglCanvasGL::Clf(mglColor Back)
{
	Fog(0);
	if(Back==NC)	Back=mglColor(1,1,1);
//	glDepthFunc(GL_LESS);
	glDepthFunc(GL_GREATER);
//	back[0]=Back.r;	back[1]=Back.g;	back[2]=Back.b;
	glClearColor(Back.r,Back.g,Back.b,0.);
	glClearDepth(-10.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
}
//-----------------------------------------------------------------------------
void mglCanvasGL::SetPen(char style,float width)
{
	if(style==0)	return;
	if(style!='-')	switch(style)
	{
	case '|': glLineStipple(int(width),0xff00);	break;
	case ';': glLineStipple(int(width),0xf0f0);	break;
	case ':': glLineStipple(int(width),0x8888);	break;
	case 'j': glLineStipple(int(width),0xfe10);	break;
	case 'i': glLineStipple(int(width),0xe4e4);	break;
	case ' ': glLineStipple(int(width),0x0000);	break;
	}
	if(width>0)		glLineWidth(width);
	else			glLineWidth(1);
	if(style=='-')	glDisable(GL_LINE_STIPPLE);
	else			glEnable(GL_LINE_STIPPLE);
}
//-----------------------------------------------------------------------------
void mglCanvasGL::EndFrame()
{
//	mglGraph::EndFrame();
	glEndList();
}
//-----------------------------------------------------------------------------
int mglCanvasGL::NewFrame()
{
	Clf();
	Identity();
	glNewList(CurFrameId,GL_COMPILE);
	CurFrameId++;
	return CurFrameId-1;
}
//-----------------------------------------------------------------------------
unsigned char **mglCanvasGL::GetRGBLines(long &width, long &height, unsigned char *&f, bool alpha)
{
	long x, y, d = alpha ? 4:3;
	GLint w[4];
	glGetIntegerv(GL_VIEWPORT,w);
	x=w[0];	y=w[1];	width=w[2];	height=w[3];
	unsigned char **p;

	p = (unsigned char **)malloc(height * sizeof(unsigned char *));
	f = (unsigned char *) malloc(width*height * sizeof(unsigned char)*d);
	for(long i=0;i<height;i++)	p[i] = f+d*width*(height-1-i);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(x, y, width, height, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, f);
	return p;
}
//-----------------------------------------------------------------------------
void mglCanvasGL::trig_draw(float *p1, float *p2, float *p3,bool anorm)
{
	float c[4];
	glBegin(GL_TRIANGLES);
	txt[long(p1[3])].GetC(p1[3],p1[4],c);
	glColor4f(c[1],c[1],c[2],c[3]);	glVertex3f(p1[0],p1[1],p1[2]);
	txt[long(p2[3])].GetC(p2[3],p2[4],c);
	glColor4f(c[0],c[1],c[2],c[3]);	glVertex3f(p2[0],p2[1],p2[2]);
	txt[long(p3[3])].GetC(p3[3],p3[4],c);
	glColor4f(c[0],c[1],c[2],c[3]);	glVertex3f(p3[0],p3[1],p3[2]);
	glEnd();
}
//-----------------------------------------------------------------------------
void mglCanvasGL::quad_draw(float *p0, float *p1, float *p2, float *p3)
{
	float c[4];
	glBegin(GL_QUADS);
	txt[long(p0[3])].GetC(p0[3],p0[4],c);
	glColor4f(c[0],c[1],c[2],c[3]);	glVertex3f(p0[0],p0[1],p0[2]);
	txt[long(p1[3])].GetC(p1[3],p1[4],c);
	glColor4f(c[0],c[1],c[2],c[3]);	glVertex3f(p1[0],p1[1],p1[2]);
	txt[long(p3[3])].GetC(p3[3],p3[4],c);
	glColor4f(c[0],c[1],c[2],c[3]);	glVertex3f(p3[0],p3[1],p3[2]);
	txt[long(p2[3])].GetC(p2[3],p2[4],c);
	glColor4f(c[0],c[1],c[2],c[3]);	glVertex3f(p2[0],p2[1],p2[2]);
	glEnd();
}
//-----------------------------------------------------------------------------
void mglCanvasGL::line_draw(float *p1, float *p2)
{
	float c[4];
	glBegin(GL_LINES);
	txt[long(p1[3])].GetC(p1[3],0,c);
	glColor4f(c[0],c[1],c[2],c[3]);	glVertex3f(p1[0],p1[1],p1[2]);
	txt[long(p2[3])].GetC(p2[3],0,c);
	glColor4f(c[0],c[1],c[2],c[3]);	glVertex3f(p2[0],p2[1],p2[2]);
	glEnd();
}
//-----------------------------------------------------------------------------
void mglCanvasGL::pnt_draw(long x,long y,float z,unsigned char cc[4])
{
	float c[4]={cc[0]/255.,cc[1]/255.,cc[2]/255.,cc[3]/255.};
	glBegin(GL_POINTS);
	glColor4f(c[0],c[1],c[2],c[3]);	glVertex3f(x,y,z);
	glEnd();
}
//-----------------------------------------------------------------------------

