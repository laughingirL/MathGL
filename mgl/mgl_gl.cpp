/* mgl_gl.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <mathgl.abalakin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
//-----------------------------------------------------------------------------
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <wchar.h>
#include "mgl/mgl_gl.h"
#include "mgl/mgl_c.h"
#include "mgl/mgl_f.h"
#ifdef WIN32
#define swprintf    _snwprintf
#endif
//-----------------------------------------------------------------------------
/// Create mglGraph object in OpenGL mode.
HMGL mgl_create_graph_gl()
{	return new mglGraphGL;	}
/// Create mglGraph object in OpenGL mode.
long mgl_create_graph_gl_()
{	return long(new mglGraphGL);	}
//-----------------------------------------------------------------------------
mglGraphGL::mglGraphGL() : mglGraphAB(1,1)	{}
//-----------------------------------------------------------------------------
mglGraphGL::~mglGraphGL(){}
//-----------------------------------------------------------------------------
void mglGraphGL::Alpha(bool enable)
{
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
}
//-----------------------------------------------------------------------------
void mglGraphGL::Light(int n,mglPoint p,mglColor c, float br,bool infty)
{
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
void mglGraphGL::LightScale()
{
	mglGraphAB::LightScale();
	GLenum light[8] = {GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4,
			GL_LIGHT5,GL_LIGHT6,GL_LIGHT7};
	float pos[4]={0,0,0,0};
	for(int i=0;i<8;i++)
	{
		memcpy(pos,pLight+3*i,3*sizeof(float));
		if(nLight[i])	glLightfv(light[i], GL_POSITION, pos);
	}
}
//-----------------------------------------------------------------------------
void mglGraphGL::Light(int n, bool enable)
{
	GLenum light[8] = {GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4,
			GL_LIGHT5,GL_LIGHT6,GL_LIGHT7};
	if(enable)	glEnable(light[n]);
	else		glDisable(light[n]);
}
//-----------------------------------------------------------------------------
void mglGraphGL::Light(bool enable)
{
	if(enable)	{	glEnable(GL_LIGHTING);	glEnable(GL_NORMALIZE);	}
	else		{	glDisable(GL_LIGHTING);	glDisable(GL_NORMALIZE);	}
}
//-----------------------------------------------------------------------------
void mglGraphGL::View(float TetX,float TetY,float TetZ)
{
	glMatrixMode(GL_PROJECTION);//GL_PROJECTION GL_VIEWPORT GL_MODELVIEW
	glRotated(TetX,1.,0.,0.);
	glRotated(TetY,0.,1.,0.);
	glRotated(TetZ,0.,0.,1.);
}
//-----------------------------------------------------------------------------
void mglGraphGL::Fog(float d, float)
{
/*	if(d>0)
	{
		glFogf(GL_FOG_MODE,GL_EXP);
		glFogf(GL_FOG_DENSITY,5*d);
		glFogfv(GL_FOG_COLOR,back);
		glEnable(GL_FOG);
	}
	else	glDisable(GL_FOG);*/
}
//-----------------------------------------------------------------------------
void mglGraphGL::Clf(mglColor Back)
{
	Fog(0);
	CurrPal = 0;
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
void mglGraphGL::Ball(float x,float y,float z,mglColor col,float alpha)
{
	if(alpha==0)	return;
	if(alpha<0)	{	alpha = -alpha;	}
	else		{	if(!ScalePoint(x,y,z))	return;	}
	if(!col.Valid())	col = mglColor(1.,0.,0.);
	alpha = Transparent ? alpha : 1;
	float p[3] = {x,y,z};	PostScale(p,1);
	glBegin(GL_POINTS);
	glColor4f(col.r,col.g,col.b,alpha);
	glVertex3f(p[0],p[1],p[2]);
	glEnd();
}
//-----------------------------------------------------------------------------
void mglGraphGL::ball(float *p,float *c)
{
	glBegin(GL_POINTS);
	glColor4fv(c);
	glVertex3fv(p);
	glEnd();
}
//-----------------------------------------------------------------------------
void mglGraphGL::Pen(mglColor col, char style,float width)
{
	mglGraphAB::Pen(col,style,width);
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
	if(width>=1)	glLineWidth(width);
	else			glLineWidth(1);
	if(style=='-')	glDisable(GL_LINE_STIPPLE);
	else			glEnable(GL_LINE_STIPPLE);
}
//-----------------------------------------------------------------------------
void mglGraphGL::EndFrame()	{	glEndList();	}
//-----------------------------------------------------------------------------
int mglGraphGL::NewFrame(int id)
{
	Clf();
	Identity();
	CurFrameId = id<=0 ? CurFrameId : id;
	glNewList(CurFrameId,GL_COMPILE);
	CurFrameId++;
	return CurFrameId-1;
}
//-----------------------------------------------------------------------------
unsigned char **mglGraphGL::GetRGBLines(long &width, long &height, unsigned char *&f, bool alpha)
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
void mglGraphGL::trig_plot(float *p0,float *p1,float *p2,float *c0,float *c1,float *c2)
{
	glBegin(GL_TRIANGLES);
	glColor4f(c0[0],c0[1],c0[2],c0[3]);	glVertex3f(p0[0],p0[1],p0[2]);
	glColor4f(c1[1],c1[1],c1[2],c1[3]);	glVertex3f(p1[0],p1[1],p1[2]);
	glColor4f(c2[0],c2[1],c2[2],c2[3]);	glVertex3f(p2[0],p2[1],p2[2]);
	glEnd();
}
//-----------------------------------------------------------------------------
void mglGraphGL::trig_plot_n(float *p0,float *p1,float *p2,float *c0,float *c1,float *c2,float *n0,float *n1,float *n2)
{
	glBegin(GL_TRIANGLES);
	glColor4f(c0[0],c0[1],c0[2],c0[3]);	glNormal3f(n0[0],n0[1],n0[2]);	glVertex3f(p0[0],p0[1],p0[2]);
	glColor4f(c1[1],c1[1],c1[2],c1[3]);	glNormal3f(n1[0],n1[1],n1[2]);	glVertex3f(p1[0],p1[1],p1[2]);
	glColor4f(c2[0],c2[1],c2[2],c2[3]);	glNormal3f(n2[0],n2[1],n2[2]);	glVertex3f(p2[0],p2[1],p2[2]);
	glEnd();
}
//-----------------------------------------------------------------------------
void mglGraphGL::quad_plot(float *p0,float *p1,float *p2,float *p3, float *c0,float *c1,float *c2,float *c3)
{
	glBegin(GL_QUADS);
	glColor4f(c0[0],c0[1],c0[2],c0[3]);	glVertex3f(p0[0],p0[1],p0[2]);
	glColor4f(c1[0],c1[1],c1[2],c1[3]);	glVertex3f(p1[0],p1[1],p1[2]);
	glColor4f(c3[0],c3[1],c3[2],c3[3]);	glVertex3f(p3[0],p3[1],p3[2]);
	glColor4f(c2[0],c2[1],c2[2],c2[3]);	glVertex3f(p2[0],p2[1],p2[2]);
	glEnd();
}
//-----------------------------------------------------------------------------
void mglGraphGL::quad_plot_a(float *p0,float *p1,float *p2,float *p3,
				float a0,float a1,float a2,float a3,float alpha)
{
	register float t,s;
	register long k;
	long n = NumCol-1;
	mglColor c;
	
	glBegin(GL_QUADS);
	s = a0;
	t = alpha*(alpha>0 ? (s+1.f)*(s+1.f) : (1.f-s)*(s-1.f));
	s = n*(s+1.f)/2.f;	k = long(s);	s -= k;
	if(k<n)	c = cmap[k]*(1.f-s) + cmap[k+1]*s;	else	c = cmap[n];
	glColor4f(c.r,c.g,c.b,t);	glVertex3f(p0[0],p0[1],p0[2]);
	
	s = a1;
	t = alpha*(alpha>0 ? (s+1.f)*(s+1.f) : (1.f-s)*(s-1.f));
	s = n*(s+1.f)/2.f;	k = long(s);	s -= k;
	if(k<n)	c = cmap[k]*(1.f-s) + cmap[k+1]*s;	else	c = cmap[n];
	glColor4f(c.r,c.g,c.b,t);	glVertex3f(p1[0],p1[1],p1[2]);
	
	s = a3;
	t = alpha*(alpha>0 ? (s+1.f)*(s+1.f) : (1.f-s)*(s-1.f));
	s = n*(s+1.f)/2.f;	k = long(s);	s -= k;
	if(k<n)	c = cmap[k]*(1.f-s) + cmap[k+1]*s;	else	c = cmap[n];
	glColor4f(c.r,c.g,c.b,t);	glVertex3f(p3[0],p3[1],p3[2]);
	
	s = a2;
	t = alpha*(alpha>0 ? (s+1.f)*(s+1.f) : (1.f-s)*(s-1.f));
	s = n*(s+1.f)/2.f;	k = long(s);	s -= k;
	if(k<n)	c = cmap[k]*(1.f-s) + cmap[k+1]*s;	else	c = cmap[n];
	glColor4f(c.r,c.g,c.b,t);	glVertex3f(p2[0],p2[1],p2[2]);
	glEnd();
}
//-----------------------------------------------------------------------------
void mglGraphGL::quad_plot_n(float *p0,float *p1,float *p2,float *p3,float *c0,float *c1,float *c2,float *c3,float *n0,float *n1,float *n2,float *n3)
{
	glBegin(GL_QUADS);
	glColor4f(c0[0],c0[1],c0[2],c0[3]);	glNormal3f(n0[0],n0[1],n0[2]);	glVertex3f(p0[0],p0[1],p0[2]);
	glColor4f(c1[0],c1[1],c1[2],c1[3]);	glNormal3f(n1[0],n1[1],n1[2]);	glVertex3f(p1[0],p1[1],p1[2]);
	glColor4f(c3[0],c3[1],c3[2],c3[3]);	glNormal3f(n3[0],n3[1],n3[2]);	glVertex3f(p3[0],p3[1],p3[2]);
	glColor4f(c2[0],c2[1],c2[2],c2[3]);	glNormal3f(n2[0],n2[1],n2[2]);	glVertex3f(p2[0],p2[1],p2[2]);
	glEnd();
}
//-----------------------------------------------------------------------------
void mglGraphGL::line_plot(float *p1,float *p2,float *c1,float *c2,bool all)
{
	if(all)		Pen(NC,'-',BaseLineWidth);
	glBegin(GL_LINES);
	glColor3f(c1[0],c1[1],c1[2]);	glVertex3f(p1[0],p1[1],p1[2]);
	glColor3f(c2[0],c2[1],c2[2]);	glVertex3f(p2[0],p2[1],p2[2]);
	glEnd();
}
//-----------------------------------------------------------------------------
void mglGraphGL::mark_plot(float *pp, char type)
{
	register float x=pp[0],y=pp[1],z=pp[2], s=MarkSize*175*font_factor;	// 175 = 0.35*500
	long j;
	if(!ScalePoint(x,y,z))	return;
	Pen(NC,'-',BaseLineWidth);
	glColor3f(CDef[0],CDef[1],CDef[2]);
	if(type=='.')
	{
		glBegin(GL_POINTS);
		glVertex3f(x,y,z);
		glEnd();
	}
	else
	{
		register int i;
		switch(type)
		{
		case '+':
			s = s*1.1;
			glBegin(GL_LINES);
			glVertex3f(x-s,y,z);		glVertex3f(x+s,y,z);
			glVertex3f(x,y-s,z);		glVertex3f(x,y+s,z);
			glEnd();				break;
		case 'x':
			glBegin(GL_LINES);
			glVertex3f(x-s,y-s,z);		glVertex3f(x+s,y+s,z);
			glVertex3f(x+s,y-s,z);		glVertex3f(x-s,y+s,z);
			glEnd();				break;
		case 's':
			glBegin(GL_LINE_LOOP);
			glVertex3f(x-s,y-s,z);		glVertex3f(x+s,y-s,z);
			glVertex3f(x+s,y+s,z);		glVertex3f(x-s,y+s,z);
			glEnd();				break;
		case 'd':
			s = s*1.1;
			glBegin(GL_LINE_LOOP);
			glVertex3f(x,y-s,z);		glVertex3f(x+s,y,z);
			glVertex3f(x,y+s,z);		glVertex3f(x-s,y,z);
			glEnd();				break;
		case '*':
			s = s*1.1;
			glBegin(GL_LINES);
			glVertex3f(x-s,y,z);			glVertex3f(x+s,y,z);
			glVertex3f(x-0.6*s,y-0.8*s,z);	glVertex3f(x+0.6*s,y+0.8*s,z);
			glVertex3f(x+0.6*s,y-0.8*s,z);	glVertex3f(x-0.6*s,y+0.8*s,z);
			glEnd();				break;
		case '^':
			s = s*1.1;
			glBegin(GL_LINE_LOOP);
			glVertex3f(x-s,y-s/2,z);	glVertex3f(x+s,y-s/2,z);
			glVertex3f(x,y+s,z);		glEnd();		break;
		case 'v':
			s = s*1.1;
			glBegin(GL_LINE_LOOP);
			glVertex3f(x-s,y+s/2,z);	glVertex3f(x+s,y+s/2,z);
			glVertex3f(x,y-s,z);		glEnd();		break;
		case 'S':
			glBegin(GL_QUADS);
			glVertex3f(x-s,y-s,z);		glVertex3f(x+s,y-s,z);
			glVertex3f(x+s,y+s,z);		glVertex3f(x-s,y+s,z);
			glEnd();				break;
		case 'D':
			s = s*1.1;
			glBegin(GL_QUADS);
			glVertex3f(x,y-s,z);		glVertex3f(x+s,y,z);
			glVertex3f(x,y+s,z);		glVertex3f(x-s,y,z);
			glEnd();				break;
		case 'T':
			s = s*1.1;
			glBegin(GL_TRIANGLES);
			glVertex3f(x-s,y-s/2,z);	glVertex3f(x+s,y-s/2,z);
			glVertex3f(x,y+s,z);		glEnd();		break;
		case 'V':
			s = s*1.1;
			glBegin(GL_TRIANGLES);
			glVertex3f(x-s,y+s/2,z);	glVertex3f(x+s,y+s/2,z);
			glVertex3f(x,y-s,z);		glEnd();		break;
		case 'o':
			glBegin(GL_LINE_LOOP);
			for(i=0;i<16;i++)
				glVertex3f(x+s*cos(i*M_PI_4/2),y+s*sin(i*M_PI_4/2),z);
			glEnd();	break;
		case 'O':
			glBegin(GL_POLYGON);
			for(i=0;i<16;i++)
				glVertex3f(x+s*cos(i*M_PI_4/2),y+s*sin(i*M_PI_4/2),z);
			glEnd();	break;
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraphGL::InPlot(float x1,float x2,float y1,float y2)
{
	mglGraphAB::InPlot(x1,x2,y1,y2);
	glMatrixMode(GL_MODELVIEW);//GL_MODELVIEW GL_VIEWPORT GL_PROJECTION
	glLoadIdentity();
	glScaled(2,2,2);
	glTranslated(-0.5,-0.5,-0.5);
}
//-----------------------------------------------------------------------------
