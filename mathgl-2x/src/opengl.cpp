#ifdef WIN32
#include <windows.h>
#endif
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include "mgl2/opengl.h"
#include <algorithm>
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
void mglCanvasGL::Finish(bool fast)
{
	if(Prm.size()>0)
	{
		PreparePrim(fast);
		glVertexPointer(3, GL_FLOAT, sizeof(mglPnt), &(Pnt[0].x));
		glNormalPointer(GL_FLOAT, sizeof(mglPnt), &(Pnt[0].u));
		glColorPointer(4, GL_FLOAT, sizeof(mglPnt), &(Pnt[0].r));
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		int pdef=PDef;
		mreal ss=pPos, ww=PenWidth;
		mglPrim p;
		for(size_t i=0;i<Prm.size();i++)
		{
			p=Prm[i];	PDef=p.n3;	pPos=p.s;	PenWidth=p.w;
			switch(p.type)
			{
			case 0:	mark_draw(p.n1,p.n4,p.s,0);	break;
			case 1:	line_draw(p.n1,p.n2,0);		break;
			case 2:	trig_draw(p.n1,p.n2,p.n3,true,0);	break;
			case 3:	quad_draw(p.n1,p.n2,p.n3,p.n4,0);	break;
			case 4:	glyph_draw(&p,0);	break;
			}
		}
		PDef=pdef;	pPos=ss;	PenWidth=ww;
	}
	glFinish();
//	glBegin(GL_LINES);	glColor3f(0,0,1);	glVertex2f(0.1,0.1);	glVertex2f(0.9,0.9);	glEnd();
}
//-----------------------------------------------------------------------------
bool mglCanvasGL::Alpha(bool enable)
{
	if(enable)
	{
		set(MGL_ENABLE_ALPHA);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		if((Flag&3)==1)	glBlendFunc(GL_DST_COLOR, GL_ZERO);
		else if((Flag&3)==2) glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		else glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		clr(MGL_ENABLE_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
	}
	return mglCanvas::Alpha(enable);
}
//-----------------------------------------------------------------------------
void mglCanvasGL::AddLight(int n,mglPoint r,mglPoint d,char cc, mreal br,mreal /*ap*/)
{
	mglColor c(cc);
	mglColor AmbLight = mglColor(AmbBr,AmbBr,AmbBr);
	mglColor DifLight = mglColor(br,br,br);
	GLenum lght[8] = {GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4,
			GL_LIGHT5,GL_LIGHT6,GL_LIGHT7};
	float amb[4], pos[4],dif[4],dir[4];
	bool inf = mgl_isnan(r.x);
	if(n<0 || n>7)	{	SetWarn(mglWarnLId,"AddLight");	return;	}
	if(c.Valid())
	{
		DifLight = c*br;
		AmbLight = c*AmbBr;
	}
	dif[0] = DifLight.r;	dif[1] = DifLight.g;
	dif[2] = DifLight.b;	dif[3] = 1.;
	amb[0] = AmbLight.r;	amb[1] = AmbLight.g;
	amb[2] = AmbLight.b;	amb[3] = 1.;
	if(inf)
	{	pos[0] = d.x;	pos[1] = d.y;	pos[2] = d.z;	pos[3] = 0;	}
	else
	{	pos[0] = r.x;	pos[1] = r.y;	pos[2] = r.z;	pos[3] = 1;	}
	dir[0] = d.x;	dir[1] = d.y;	dir[2] = d.z;
	glShadeModel(GL_SMOOTH);
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 5.0);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, pos);

	glLightfv(lght[n], GL_AMBIENT, amb);
	glLightfv(lght[n], GL_DIFFUSE, dif);
	//glLightfv(lght[n], GL_SPECULAR, spc);
	glLightfv(lght[n], GL_POSITION, pos);
	if(!inf)	glLightfv(lght[n], GL_SPOT_DIRECTION, dir);
	glEnable(lght[n]);
}
//-----------------------------------------------------------------------------
void mglCanvasGL::Light(int n, bool enable)
{
	GLenum lght[8] = {GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4,
			GL_LIGHT5,GL_LIGHT6,GL_LIGHT7};
	if(enable)	glEnable(lght[n]);
	else		glDisable(lght[n]);
}
//-----------------------------------------------------------------------------
bool mglCanvasGL::Light(bool enable)
{
	if(enable)	{	glEnable(GL_LIGHTING);	glEnable(GL_NORMALIZE);}
	else		{	glDisable(GL_LIGHTING);	glDisable(GL_NORMALIZE);	}
	return mglCanvas::Light(enable);
}
//-----------------------------------------------------------------------------
void mglCanvasGL::LightScale()
{
	mglCanvas::LightScale();
	GLenum ll[8] = {GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4,
			GL_LIGHT5,GL_LIGHT6,GL_LIGHT7};
	float pos[4]={0,0,0,0};
	for(int i=0;i<8;i++)
	{
		pos[0] = light[i].p.x;
		pos[1] = light[i].p.y;
		pos[2] = light[i].p.z;
		if(light[i].n)	glLightfv(ll[i], GL_POSITION, pos);
	}
}
//-----------------------------------------------------------------------------
void mglCanvasGL::Zoom(mreal x1, mreal y1, mreal x2, mreal y2)
{
	glMatrixMode(GL_PROJECTION);//GL_PROJECTION GL_VIEWPORT GL_MODELVIEW
	glLoadIdentity();
	glScaled(x2-x1,y2-y1,1);
	glTranslated((x1+x2-1)/2,(y1+y2-1)/2,0);
}
//-----------------------------------------------------------------------------
void mglCanvasGL::View(mreal TetX,mreal TetY,mreal TetZ)
{
	glMatrixMode(GL_PROJECTION);//GL_PROJECTION GL_VIEWPORT GL_MODELVIEW
	glRotated(TetX,1.,0.,0.);
	glRotated(TetY,0.,1.,0.);
	glRotated(TetZ,0.,0.,1.);
}
//-----------------------------------------------------------------------------
void mglCanvasGL::Fog(mreal d, mreal)
{
	if(d>0)		// TODO: Check fog to OpenGL mode
	{
		float back[4]={BDef[0]/255.f,BDef[1]/255.f,BDef[2]/255.f,BDef[3]/255.f};
		glFogf(GL_FOG_MODE,GL_EXP);
		glFogf(GL_FOG_DENSITY,5*d);
		glFogfv(GL_FOG_COLOR,back);
		glEnable(GL_FOG);
	}
	else	glDisable(GL_FOG);
}
//-----------------------------------------------------------------------------
void mglCanvasGL::Clf(mglColor Back)
{
	mglCanvas::Clf(Back);
//	glDepthFunc(GL_LESS);
	glDepthFunc(GL_GREATER);
//	back[0]=Back.r;	back[1]=Back.g;	back[2]=Back.b;
	glClearColor(Back.r,Back.g,Back.b,0.);
	glClearDepth(-10.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);

	glMatrixMode(GL_MODELVIEW);//GL_MODELVIEW GL_VIEWPORT GL_PROJECTION
	glLoadIdentity();
//	glScaled(1.5,1.5,1.5);
//	glTranslated(-0.5,-0.5,-0.5);
}
//-----------------------------------------------------------------------------
void mglCanvasGL::set_pen(unsigned style,mreal width)
{
	if(style==0)	return;
	if(style!=0xffff)
	{
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(int(width+0.5),style);
	}
	else	glDisable(GL_LINE_STIPPLE);
	if(width>0)		glLineWidth(width);
	else			glLineWidth(1);
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
	Clf();	Identity();
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
void mglCanvasGL::trig_draw(long k1, long k2, long k3, bool, mglDrawReg *)
{
	if(k1<0 || k2<0 || k3<0)	return;
	glBegin(GL_TRIANGLES);
	glArrayElement(k1);	glArrayElement(k2);	glArrayElement(k3);
	glEnd();
}
//-----------------------------------------------------------------------------
void mglCanvasGL::quad_draw(long k1, long k2, long k3, long k4, mglDrawReg *)
{
	if(k1<0 || k2<0 || k3<0 || k4<0)	return;
	glBegin(GL_QUADS);
	glArrayElement(k1);	glArrayElement(k2);
	glArrayElement(k4);	glArrayElement(k3);
	glEnd();
}
//-----------------------------------------------------------------------------
void mglCanvasGL::line_draw(long k1, long k2, mglDrawReg *)
{
	if(k1<0 || k2<0 || PDef==0)	return;
/*	unsigned pdef = PDef*0x10001;
	pdef = pdef << (int(100*pPos+0.5)%16);
	set_pen(pdef&0xffff,PenWidth);*/
	set_pen(PDef,PenWidth);
	glBegin(GL_LINES);
	glArrayElement(k1);	glArrayElement(k2);
	glEnd();
}
//-----------------------------------------------------------------------------
void mglCanvasGL::pnt_draw(long k1, mglDrawReg *)
{
	if(k1<0)	return;
	glBegin(GL_POINTS);
	glArrayElement(k1);
	glEnd();
}
//-----------------------------------------------------------------------------

