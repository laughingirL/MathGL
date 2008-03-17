/* mgl_tool.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>
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
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <wchar.h>
#include "mgl/mgl_gl.h"
#ifdef WIN32
#define swprintf    _snwprintf
#endif
//-----------------------------------------------------------------------------
void wcstrim_mgl(wchar_t *str);
int _mgl_tick_ext(float a, float b, wchar_t s[32], float &v);
void _mgl_tick_text(float z, float z0, float d, float v, int kind, wchar_t str[64]);
//-----------------------------------------------------------------------------
mglGraphGL::mglGraphGL() : mglGraph()
{
	tetx=tety=tetz=back[3]=0;
	back[0]=back[1]=back[2]=1;
};
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
	if(d>0)
	{
		glFogf(GL_FOG_MODE,GL_EXP);
		glFogf(GL_FOG_DENSITY,5*d);
		glFogfv(GL_FOG_COLOR,back);
		glEnable(GL_FOG);
	}
	else	glDisable(GL_FOG);
}
//-----------------------------------------------------------------------------
void mglGraphGL::Clf(mglColor Back)
{
	Fog(0);
	CurrPal = 0;
	if(Back==NC)	Back=mglColor(1,1,1);
//	glDepthFunc(GL_LESS);
	glDepthFunc(GL_GREATER);
	back[0]=Back.r;	back[1]=Back.g;	back[2]=Back.b;
	glClearColor(Back.r,Back.g,Back.b,0.);
	glClearDepth(-10.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
}
//-----------------------------------------------------------------------------
void mglGraphGL::InPlot(float x1,float x2,float y1,float y2)
{
	float s3=PlotFactor;
	const char *stl = "k-1";
	font_factor = x2-x1 > y2-y1 ? x2-x1 : y2-y1;
	glMatrixMode(GL_MODELVIEW);//GL_MODELVIEW GL_VIEWPORT GL_PROJECTION
	glLoadIdentity();
	glTranslated((x1+x2-1),(y1+y2-1),-0.5/s3);
	glScaled((x2-x1)/s3,(y2-y1)/s3,0.5/s3);
//	glTranslated(0,0,0.5);
	SelectPen(stl);
	tetx = tety = tetz = 0;
	memset(tetr,0,8*sizeof(float));
}
//-----------------------------------------------------------------------------
void mglGraphGL::Aspect(float Ax,float Ay,float Az)
{
	float a = Ax > Ay ? Ax : Ay;
	a = a > Az ? a : Az;
	glMatrixMode(GL_MODELVIEW);
	glScaled(Ax/a,Ay/a,Az/a);
}
//-----------------------------------------------------------------------------
void mglGraphGL::RotateN(float Tet,float x,float y,float z)
{
	if(Tet)
	{
		glMatrixMode(GL_MODELVIEW);
		glRotated(-Tet,x,y,z);
		if(tetr[0])	{	tetr[4]=Tet;	tetr[5]=x;	tetr[6]=y;	tetr[7]=z;	}
		else		{	tetr[0]=Tet;	tetr[1]=x;	tetr[2]=y;	tetr[3]=z;	}
	}	
}
//-----------------------------------------------------------------------------
void mglGraphGL::Perspective(float a)	// TODO !!! Check !!!
{
	glMatrixMode(GL_PROJECTION);
	glFrustum(-1, 1, -1, 1, a, a-2);
}
//-----------------------------------------------------------------------------
void mglGraphGL::Ball(float x,float y,float z,mglColor col,float alpha)
{
//	alpha = Transparent ? alpha : 1;
	if(alpha==0 || !ScalePoint(x,y,z))	return;
	if(!col.Valid())	col = mglColor(1.,0.,0.);
	glBegin(GL_POINTS);
	glColor4f(col.r,col.g,col.b,alpha);
	glVertex3f(x,y,z);
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
	if(style==0)	return;
	if(col.Valid())	// цвет указан
	{
		def_col = col;
		glColor4f(col.r,col.g,col.b,1);	// все линии не прозрачны!
	}
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
void mglGraphGL::Colorbar(const char *sch,int where)
{
	register long i;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	float *pp, *cc,x,y,z;
	float d,ss=FontSize/(9.*PlotFactor);
	mglColor c;
	SetScheme(sch);
	if(where<0 || where>5)	where = 0;
	pp = new float[6*255];
	cc = new float[6*255];
	for(i=0;i<255;i++)
	{
		d = 2*i/255.;
		pp[6*i+0] = pp[6*i+3] = 0.9*(d-1);
		pp[6*i+1] = pp[6*i+4] = 0.9*(d-1);
		pp[6*i+2] = pp[6*i+5] = 1;
		switch(where)
		{
		case 0:
			pp[6*i]  = 0.9;	pp[6*i+3] = 1;
			break;
		case 1:
			pp[6*i]  = -1;	pp[6*i+3] = -0.9;
			break;
		case 2:
			pp[6*i+1] = 0.9;	pp[6*i+4] = 1;
			break;
		case 3:
			pp[6*i+1] = -1;	pp[6*i+4] = -0.9;
			break;
		}
		c = GetC(Cmin + (Cmax-Cmin)*d/2);
		cc[6*i+0] = cc[6*i+3] = c.r;
		cc[6*i+1] = cc[6*i+4] = c.g;
		cc[6*i+2] = cc[6*i+5] = c.b;
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pp);
	glBegin(GL_QUAD_STRIP);
	for(i=0;i<255;i++)
	{
		glColor3f(cc[6*i],cc[6*i+1],cc[6*i+2]);
		glArrayElement(2*i);
		glColor3f(cc[6*i+3],cc[6*i+4],cc[6*i+5]);
		glArrayElement(2*i+1);
	}
	glEnd();
	glDisableClientState(GL_VERTEX_ARRAY);
	glFlush();

	const char *a="rC";
	if(where==1)	a = "rL";
	if(where==0)	a = "rR";
	float v=0,t;
	int kind=0;
	wchar_t s[32]=L"", str[64];
	if(!ctt && !TickStr && TuneTicks) kind = _mgl_tick_ext(Max.x, Min.x, s, v);

	glColor3f(0,0,0);
	for(i=0;i<5;i++)
	{
		d = 0.5*i;
		x = y = 0.9*(d-1);	z = 1;
		switch(where)
		{
		case 0:	x =  0.87;	break;
		case 1:	x = -0.87;	break;
		case 2:	y =  0.87;	break;
		case 3:	y = -0.87;	break;
		}
		t = Cmin+i*(Cmax-Cmin)/4;
		bool func = false;
		if(TickStr)		func = TickStr('c', t, str);
		if(!func)
		{
			if(ctt)	swprintf(str, 64, ctt, (Cmax-Cmin)/100);
			else	_mgl_tick_text(t,Cmin,(Cmax-Cmin)/100,v,kind,str);
			wcstrim_mgl(str);
		}

		glPushMatrix();
		glTranslated(x,y,z);
		glScaled(ss, ss, ss);
		fnt->Puts(str,a);
		glPopMatrix();
	}

	if(kind&2)
	{
		switch(where)
		{
		case 1:	x = -0.85;	y = 0.75;	break;
		case 2:	x =  0.75;	y = 0.85;	break;
		case 3:	x =  0.75;	y =-0.85;	break;
		default:x =  0.85;	y = 0.75;	break;
		}
		glPushMatrix();
		glTranslated(x,y,z);
		glScaled(ss, ss, ss);
		fnt->Puts(s,a);
		glPopMatrix();
	}
	glPopMatrix();
	delete []pp;
	delete []cc;
}
//-----------------------------------------------------------------------------
void mglGraphGL::Putsw(mglPoint p,const wchar_t *str,const char *font, float size,char dir,float shift)
{
	bool upside = ( ((_sx==-1 ^ (Org.y==Max.y || Org.z==Max.z)) && (dir=='x' || dir=='X')) ||
					((_sy==-1 ^ (Org.x==Max.x || Org.z==Max.z)) && (dir=='y' || dir=='Y')) ||
					((_sz==-1 ^ (Org.y==Max.y || Org.x==Max.x)) && (dir=='z' || dir=='Z')));
	SelectPen("-");
	if(size<0)	size = -size*FontSize;
	if(ScalePuts)
	{
		bool tt = Cut;	Cut = true;
		ScalePoint(p.x,p.y,p.z);
		Cut = tt;
	}
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	static float mat[16],x0=1,y0=0;

	if(RotatedText)
	{
		float x=p.x,y=p.y,z=p.z;
		switch(dir)
		{
		case 'x':	// x label
		case 'X':	// x label
			x+=1;	break;
		case 'y':	// y label
		case 'Y':	// y label
			y+=1;	break;
		case 'i':	// z label
		case 'I':	// z label
			z-=1;	break;
		case 'z':	// z label
		case 'Z':	// z label
			z+=1;		break;
		}
		glTranslated(x,y,z);
		glGetFloatv(GL_MODELVIEW_MATRIX, mat);
		x0 = mat[12];	y0 = mat[13];
		glTranslated(-x,-y,-z);
	}

	glTranslated(p.x,p.y,p.z);
	
	if(tetr[4])	glRotated(tetr[4],tetr[5],tetr[6],tetr[7]);
	if(tetr[0])	glRotated(tetr[0],tetr[1],tetr[2],tetr[3]);
/*	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	mat[0] = mat[5] = mat[10] = mat[15] = 1.;
	mat[1] = mat[2] = mat[3] = mat[4] = mat[6] = mat[7] = mat[8] = mat[9] = mat[11] = 0;
	x0 -= mat[12];	y0 -= mat[13];
	glLoadMatrixf(mat);*/
	float ss = size/(8.*PlotFactor*sqrt(2.));

//	if(y0==0 && x0==0 && strchr("xyzXYZ",dir))	return;
	if(RotatedText)
	{
		float alpha = 180/M_PI*atan2(y0,x0);
		glRotated(alpha,0,0,1);
		glScaled(ss,ss,ss);
		int s = upside ? -1:1;
		if(strchr("xz",dir))		glTranslated(0,-s*(shift/10+0.25)*font_factor,0);
		else if(strchr("XZ",dir))	glTranslated(0,-s*(shift/10+0.4)*font_factor,0);
		else if(strchr("yi",dir))	glTranslated(0,s*(shift/10+0.25)*font_factor,0);
		else if(strchr("YI",dir))	glTranslated(0,s*(shift/10+0.4)*font_factor,0);
	}
	else
	{
		glScaled(ss, ss, ss);
		glTranslated(-0.25,0,0);
	}
	fnt->Puts(str,font?font:FontDef);
	glPopMatrix();
}
//-----------------------------------------------------------------------------
float mglGraphGL::Putsw(mglPoint p,mglPoint n,const wchar_t *str,char font,float size)
{
	SelectPen("-");
	n = p+n;
	if(size<0)	size = -size*FontSize;
	if(ScalePuts)
	{
		bool tt = Cut;	Cut = true;
		ScalePoint(p.x,p.y,p.z);
		ScalePoint(n.x,n.y,n.z);
		Cut = tt;
	}
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	static float mat[16],x0=0,y0=0;

	glTranslated(n.x, n.y, n.z);
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	x0 = mat[12];	y0 = mat[13];
	glTranslated(p.x-n.x, p.y-n.y, p.z-n.z);

	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	mat[0] = mat[5] = mat[10] = mat[15] = 1.;
	mat[1] = mat[2] = mat[3] = mat[4] = mat[6] = mat[7] = mat[8] = mat[9] = mat[11] = 0;
	x0 -= mat[12];	y0 -= mat[13];
	glLoadMatrixf(mat);
	float ss = size/(8.*PlotFactor);

	float alpha = 180/M_PI*atan2(y0,x0);
	glRotated(alpha,0,0,1);
	glScaled(ss,ss,ss);
	float shift = (font=='t') ? -0.07*font_factor : 0.07*font_factor;
	glTranslated(0,shift,0);

	fnt->Puts(str,"rL");
	glPopMatrix();
	return fnt->Width(str,"rL")*size/8.;
}
//-----------------------------------------------------------------------------
void mglGraphGL::arrow_plot(float *p1,float *p2,char st)
{
	if(!strchr("AVKSDTIO",st))	return;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	static float mat[16],x0=0,y0=0;

	glTranslated(p2[0], p2[1], p2[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	x0 = mat[12];	y0 = mat[13];
	glTranslated(p1[0]-p2[0], p1[1]-p2[1], p1[2]-p2[2]);

	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	mat[0] = mat[5] = mat[10] = mat[15] = 1.;
	mat[1] = mat[2] = mat[3] = mat[4] = mat[6] = mat[7] = mat[8] = mat[9] = mat[11] = 0;
	x0 -= mat[12];	y0 -= mat[13];
	glLoadMatrixf(mat);

	float ss = ArrowSize*0.5*font_factor;
	float alpha = 180/M_PI*atan2(y0,x0);
	glRotated(alpha,0,0,1);
	glScaled(ss,ss,ss);

	switch(st)
	{
	case 'I':
		glBegin(GL_LINES);
		glVertex2f(0.,-1.);		glVertex2f(0.,1.);
		glEnd();	break;
	case 'D':
		glBegin(GL_QUADS);
		glVertex2f(0.,-1.);		glVertex2f(1.,0.);
		glVertex2f(0.,1.);		glVertex2f(-1.,0.);
		glEnd();	break;
	case 'S':
		glBegin(GL_QUADS);
		glVertex2f(-1.,-1.);	glVertex2f(1.,-1.);
		glVertex2f(1.,1.);		glVertex2f(-1.,1.);
		glEnd();	break;
	case 'T':
		glBegin(GL_TRIANGLES);
		glVertex2f(-1.,-1.);	glVertex2f(-1.,1.);
		glVertex2f(1.,0.);		glEnd();	break;
	case 'A':
		glBegin(GL_TRIANGLES);
		glVertex2f(0.,0.);		glVertex2f(2.,-1.);	glVertex2f(1.5,0.);
		glVertex2f(0.,0.);		glVertex2f(1.5,0.);	glVertex2f(2.,1.);
		glEnd();	break;
	case 'K':
		glBegin(GL_TRIANGLES);
		glVertex2f(0.,0.);		glVertex2f(2.,-1.);	glVertex2f(1.5,0.);
		glVertex2f(0.,0.);		glVertex2f(1.5,0.);	glVertex2f(2.,1.);
		glEnd();
		glBegin(GL_LINES);
		glVertex2f(0.,-1.);		glVertex2f(0.,1.);
		glEnd();	break;
	case 'V':
		glBegin(GL_TRIANGLES);
		glVertex2f(0.,0.);		glVertex2f(-2.,-1.);	glVertex2f(-1.5,0.);
		glVertex2f(0.,0.);		glVertex2f(-1.5,0.);	glVertex2f(-2.,1.);
		glEnd();	break;
//	case 'O':
//		ss = MarkSize;	MarkSize = ArrowSize;
//		mark_plot(p1,'O');		MarkSize = ss;
	}
//	UseLight = ul;
	glPopMatrix();
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
void mglGraphGL::Glyph(float x,float y, float f, int nt, const short *trig, int nl, const short *line)
{
	long i,ii;
	if(trig && nt>0)
	{
		glBegin(GL_TRIANGLES);
		for(i=0;i<nt;i++)
		{
			ii = 6*i;	glVertex2f(f*trig[ii]+x,f*trig[ii+1]+y);
			ii+=2;		glVertex2f(f*trig[ii]+x,f*trig[ii+1]+y);
			ii+=2;		glVertex2f(f*trig[ii]+x,f*trig[ii+1]+y);
		}
		glEnd();
	}
	else if(line && nl>0)
	{
		glBegin(GL_LINE_LOOP);
		for(i=0;i<nl;i++)
		{
			ii = 2*i;
			if(line[ii]==0x3fff && line[ii+1]==0x3fff)
			{	glEnd();	glBegin(GL_LINE_LOOP);	}
			else	glVertex2f(f*line[ii]+x,f*line[ii+1]+y);
		}
		glEnd();
	}
	if(nl==-1)
	{
		glBegin(GL_LINES);
		glVertex2f(x, y);
		glVertex2f(fabs(f)+x, y);
		glEnd();
	}
}
//-----------------------------------------------------------------------------
