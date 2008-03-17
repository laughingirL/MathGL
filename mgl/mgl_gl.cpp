/* mgl_gl.cpp is part of Math Graphic Library
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
#include <stdlib.h>
#include <string.h>
#include "mgl/mgl_gl.h"
#include "mgl/mgl_c.h"
#include "mgl/mgl_f.h"
#define MGL_HOW	GL_AMBIENT_AND_DIFFUSE
//#define MGL_HOW	GL_DIFFUSE
//-----------------------------------------------------------------------------
float mgl_spc[4] = {1.,1.,1.,1},mgl_spf = 50;
float mgl_cos_pp(float *pp,long i0,long i1,long i2);
//-----------------------------------------------------------------------------
//
//	Low level plot functions for OpenGL
//
//-----------------------------------------------------------------------------
/// Create mglGraph object in OpenGL mode.
HMGL mgl_create_graph_gl()
{	return new mglGraphGL;	}
/// Create mglGraph object in OpenGL mode.
long mgl_create_graph_gl_()
{	return long(new mglGraphGL);	}
//-----------------------------------------------------------------------------
void mglGraphGL::DefColor(mglColor c, float alpha)
{
	if(alpha<0 || alpha>1)	alpha = AlphaDef;
	if(c.Valid())
		glColor4f(c.r,c.g,c.b,Transparent ? alpha : 1);
	else
		glColor4f(def_col.r,def_col.g,def_col.b,Transparent ? alpha : 1);
}
//-----------------------------------------------------------------------------
void mglGraphGL::curv_plot(long n,float *pp,bool *tt)
{
	register long i;
	bool ss;
	if(!pp)	return;
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pp);

	ss = true;
	for(i=0;i<n;i++)
		if(!tt || tt[i])
		{
			if(ss)	glBegin(GL_LINE_STRIP);
			ss = false;
			glArrayElement(i);
		}
		else if(!ss)
		{	glEnd();	ss = true;	}
	if(!ss)	glEnd();
	glDisableClientState(GL_VERTEX_ARRAY);
	if(Arrow1>='A' && (!tt || tt[n-1]))	arrow_plot(pp+3*n-3,pp+3*n-6,Arrow1);
	if(Arrow2>='A' && (!tt || tt[0]))	arrow_plot(pp,pp+3,Arrow2);
}
//-----------------------------------------------------------------------------
void mglGraphGL::curv_plot(long n,float *pp,float *cc,bool *tt)
{
	register long i;
	float a;
	bool ss;
	mglColor c;

	if(!pp)	return;
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pp);

	ss = true;
	for(i=0;i<n;i++)
		if(!tt || tt[i])
		{
			if(ss)	glBegin(GL_LINE_STRIP);
			ss = false;
			if(cc[i]==0)	continue;
			a = (2*cc[i]-Cmin-Cmax)/(Cmax-Cmin);
			if(a>1)	a=1;	if(a<-1)	a=-1;
			c = GetC(a,false);
			glColor4f(c.r, c.g, c.b, (a+1)/2);
			glArrayElement(i);
		}
		else if(!ss)
		{	glEnd();	ss = true;	}
	if(!ss)	glEnd();
	glDisableClientState(GL_VERTEX_ARRAY);
}
//-----------------------------------------------------------------------------
void mglGraphGL::curv_plot(long n,float *pp,bool *tt,long *nn)
{
	register long i,k;
	bool ss;
	if(!pp || !nn)	return;
	long *kk = new long[n];
	memset(kk,0,n*sizeof(long));
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pp);

	ss = true;
	k=0;
	for(i=0;i<n;i++)
	{
		if(!tt || tt[k])
		{
			if(ss)	glBegin(GL_LINE_STRIP);
			ss = false;
			glArrayElement(k);
		}
		else if(!ss)
		{	glEnd();	ss = true;	}
		kk[k] = 1;		k = nn[k];
		if(kk[k] && k>=0)
		{

			if(!tt || tt[k])
			{
				if(ss)	glBegin(GL_LINE_STRIP);
				ss = false;
				glArrayElement(k);
			}
			if(!ss)	{	glEnd();	ss = true;	}
		}
		if(k<0 || k>=n || kk[k]!=0)
		{
			for(k=0;k<n;k++)
				if(kk[k]==0)	break;
			if(!ss)
			{	glEnd();	ss = true;	}
		}
	}
	if(!ss)	glEnd();

	glDisableClientState(GL_VERTEX_ARRAY);
	delete []kk;
}
//-----------------------------------------------------------------------------
void mglGraphGL::Mark(float x,float y,float z, char type)
{
	long j;
	Pen(NC,'-',BaseLineWidth);
	if(type=='.')
	{
		glBegin(GL_POINTS);
		if(ScalePoint(x,y,z))	glVertex3f(x,y,z);
		glEnd();
	}
	else
	{
		if(!ScalePoint(x,y,z))	return;
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x,y,z);
		glScalef(MarkSize,MarkSize,MarkSize);
		switch(type)
		{
		case '+':
			glBegin(GL_LINES);
			glVertex3f(-1,0,0);		glVertex3f(1,0,0);
			glVertex3f(0,-1,0);		glVertex3f(0,1,0);
			glEnd();				break;
		case 'x':
			glBegin(GL_LINES);
			glVertex3f(-1,-1,0);	glVertex3f(1,1,0);
			glVertex3f(1,-1,0);		glVertex3f(-1,1,0);
			glEnd();				break;
		case 's':
			glBegin(GL_LINE_STRIP);
			glVertex3f(-1,-1,0);	glVertex3f(1,-1,0);
			glVertex3f(1,1,0);		glVertex3f(-1,1,0);
			glVertex3f(-1,-1,0);
			glEnd();				break;
		case 'd':
			glBegin(GL_LINE_STRIP);
			glVertex3f(0,-1,0);		glVertex3f(1,0,0);
			glVertex3f(0,1,0);		glVertex3f(-1,0,0);
			glVertex3f(0,-1,0);
			glEnd();				break;
		case '*':
			glBegin(GL_LINES);
			for(j=0;j<6;j++)
			{
				glVertex3f(0,0,0);
				glVertex3f(cos(2*M_PI*j/6.),sin(2*M_PI*j/6.),0);
			}
			glEnd();				break;
		case '^':
			glBegin(GL_LINE_STRIP);
			glVertex3f(0,1,0);		glVertex3f(0.7,-0.5,0);
			glVertex3f(-0.7,-0.5,0);glVertex3f(0,1,0);
			glEnd();				break;
		case 'v':
			glBegin(GL_LINE_STRIP);
			glVertex3f(0,-1,0);		glVertex3f(0.7,0.5,0);
			glVertex3f(-0.7,0.5,0);	glVertex3f(0,-1,0);
			glEnd();				break;
		case 'o':
			glBegin(GL_LINE_STRIP);
			for(j=0;j<7;j++)
				glVertex3f(cos(2*M_PI*j/6.),sin(2*M_PI*j/6.),0);
			glEnd();				break;
		}
		glPopMatrix();
	}
}
//-----------------------------------------------------------------------------
void mglGraphGL::mesh_plot(long n,long m,float *pp,float *cc,bool *tt,int how)
{
	register long i,j,i0;
	bool ss;
	long dx=1,dy=1;
	if(MeshNum>1)	{	dx=(n-1)/(MeshNum-1);	dy=(m-1)/(MeshNum-1);	}
	if(dx<1)	dx=1;
	if(dy<1)	dy=1;
	if(!pp)	return;
#ifndef NO_COLOR_ARRAY
	if(cc)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, cc);
	}
#endif
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pp);
	if(how&1)	for(i=0;i<n;i+=dx)
	{
		ss = true;
		for(j=0;j<m;j++)
			if(!tt || tt[i+n*j])
			{
				if(ss)	glBegin(GL_LINE_STRIP);
				ss = false;
				i0 = i+n*j;
#ifdef NO_COLOR_ARRAY
				if(cc)	glColor3f(cc[3*i0],cc[3*i0+1],cc[3*i0+2]);
#endif
				glArrayElement(i0);
			}
			else if(!ss)
			{	glEnd();	ss = true;	}
		if(!ss)	glEnd();
	}
	if(how&2)	for(j=0;j<m;j+=dy)
	{
		ss = true;
		for(i=0;i<n;i++)
			if(!tt || tt[i+n*j])
			{
				if(ss)	glBegin(GL_LINE_STRIP);
				ss = false;
				i0 = i+n*j;
#ifdef NO_COLOR_ARRAY
				if(cc)	glColor3f(cc[3*i0],cc[3*i0+1],cc[3*i0+2]);
#endif
				glArrayElement(i0);
			}
			else if(!ss)
			{	glEnd();	ss = true;	}
		if(!ss)	glEnd();
	}
	glDisableClientState(GL_VERTEX_ARRAY);
#ifndef NO_COLOR_ARRAY
	if(cc)	glDisableClientState(GL_COLOR_ARRAY);
#endif
}
//-----------------------------------------------------------------------------
void mglGraphGL::surf_plot(long n,long m,float *pp,float *cc,bool *tt)
{
	if(!DrawFace)	return;
	register long i,j,i0;
	bool ss;
	if(!pp)	return;
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pp);
#ifndef NO_COLOR_ARRAY
	if(cc)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, 0, cc);
	}
#endif

	float *nn = new float[3*n*m];
	normal(n,m,pp,nn);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, nn);

	glColorMaterial(GL_FRONT_AND_BACK, MGL_HOW);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mgl_spc);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mgl_spf);

	for(i=0;i<n-1;i++)
	{
		ss = true;
		for(j=0;j<m-1;j++)
		{
			i0 = i+n*j;
			if(!tt || (tt[i0] && tt[i0+1] && tt[i0+n] && tt[i0+1+n]))
			{
				if(ss)
				{
					glBegin(GL_QUAD_STRIP);
#ifdef NO_COLOR_ARRAY
					if(cc)	glColor4f(cc[4*i0],cc[4*i0+1],cc[4*i0+2],cc[4*i0+3]);
#endif
					glArrayElement(i0);
#ifdef NO_COLOR_ARRAY
					if(cc)	glColor4f(cc[4*i0+4],cc[4*i0+5],cc[4*i0+6],cc[4*i0+7]);
#endif
					glArrayElement(i0+1);
				}
				i0+=n;
#ifdef NO_COLOR_ARRAY
				if(cc)	glColor4f(cc[4*i0],cc[4*i0+1],cc[4*i0+2],cc[4*i0+3]);
#endif
				glArrayElement(i0);
#ifdef NO_COLOR_ARRAY
				if(cc)	glColor4f(cc[4*i0+4],cc[4*i0+5],cc[4*i0+6],cc[4*i0+7]);
#endif
				glArrayElement(i0+1);
				ss = false;
			}
			else if(!ss)
			{	glEnd();	ss = true;	}
		}
		if(!ss)	glEnd();
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
#ifndef NO_COLOR_ARRAY
	if(cc)	glDisableClientState(GL_COLOR_ARRAY);
#endif
	delete []nn;
}
//-----------------------------------------------------------------------------
void mglGraphGL::boxs_plot(long n, long m, float *pp, mglColor *cc, bool *tt,
						float Alpha,bool )
{
	if(!DrawFace)	return;
	register long i,j,i0,i1;
	bool h1,h2,s1,s2,s3,s4;
	if(!pp || !cc)	return;
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pp);

	glColorMaterial(GL_FRONT_AND_BACK, MGL_HOW);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mgl_spc);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mgl_spf);

	glBegin(GL_QUADS);
	for(i=0;i<n-1;i++)
	{
		for(j=0;j<m-1;j++)
		{
			i0 = i+n*j;
			i1 = 4*(i+(n-1)*j) + n*m;

			h1=h2=s1=s2=s3=s4=true;
			if(tt)
			{
				if(!tt[i0])		h1 = s1 = s2 = false;
				if(!tt[i0+1])	h1 = s1 = s3 = false;
				if(!tt[i0+n])	h1 = s2 = s4 = false;
				if(!tt[i0+1+n])	h1 = s3 = s4 = false;
				if(!tt[i1])		h2 = s1 = s2 = false;
				if(!tt[i1+1])	h2 = s1 = s3 = false;
				if(!tt[i1+2])	h2 = s2 = s4 = false;
				if(!tt[i1+3])	h2 = s3 = s4 = false;
			}


			glColor4f(cc[i0].r,cc[i0].g,cc[i0].b,Alpha);
			if(h1)	// дно
			{
				normal(pp+3*i0,pp+3*(i0+1),pp+3*(i0+n+1));
				glArrayElement(i0);		glArrayElement(i0+1);
				glArrayElement(i0+n+1);	glArrayElement(i0+n);
			}
			if(h2)	// верх
			{
				normal(pp+3*i1,pp+3*(i1+1),pp+3*(i1+3));
				glArrayElement(i1);		glArrayElement(i1+1);
				glArrayElement(i1+3);	glArrayElement(i1+2);
			}
			if(s1)	// лево
			{
				normal(pp+3*i0,pp+3*(i0+1),pp+3*(i1+1));
				glArrayElement(i0);		glArrayElement(i0+1);
				glArrayElement(i1+1);	glArrayElement(i1);
			}
			if(s2)	// право
			{
				normal(pp+3*i0,pp+3*(i0+n),pp+3*(i1+2));
				glArrayElement(i0);		glArrayElement(i0+n);
				glArrayElement(i1+2);		glArrayElement(i1);
			}
			if(s3)	// спереде
			{
				normal(pp+3*i0+3,pp+3*(i0+1+n),pp+3*(i1+3));
				glArrayElement(i0+1);	glArrayElement(i0+1+n);
				glArrayElement(i1+3);	glArrayElement(i1+1);
			}
			if(s4)	// сзади
			{
				normal(pp+3*(i0+n),pp+3*(i0+1+n),pp+3*(i1+3));
				glArrayElement(i0+n);	glArrayElement(i0+1+n);
				glArrayElement(i1+3);	glArrayElement(i1+2);
			}
		}
	}
	glEnd();
	glDisableClientState(GL_VERTEX_ARRAY);
}
//-----------------------------------------------------------------------------
void mglGraphGL::cloud_plot(long nx,long ny,long nz,float *pp,float *a,float alpha)
{
	if(!DrawFace)	return;
	register long i,j,k,i0;
	long nn=nx*ny;
	if(!pp || !a)	return;
	float ad;
	mglColor c;
	bool ss;
	float *cc=new float[4*nx*ny*nz],v;
	bool *tt=new bool[4*nx*ny*nz];
	if(!cc || !tt)
	{	delete []cc;	delete []tt;	return;	}
	for(i=0;i<nx*ny*nz;i++)
	{
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
		if(tt[i])
		{
			v = GetA(a[i]);
			ad = alpha>0 ? alpha*(v+1)*(v+1)/4 : alpha*(v-1)*(v-1)/4;
//			ad = alpha>0 ? alpha*(v+1)/2 : alpha*(v-1)/2;
			if(OnCoord)	c = GetC(pp[3*i],pp[3*i+1],pp[3*i+2]);
			else		c = GetC(v,false);
			cc[4*i]=c.r;	cc[4*i+1]=c.g;
			cc[4*i+2]=c.b;	cc[4*i+3]= ad;
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pp);
#ifndef NO_COLOR_ARRAY
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_FLOAT, 0, cc);
#endif
	glColorMaterial(GL_FRONT_AND_BACK, MGL_HOW);
	float sss[4] = {0,0,0,1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sss);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);

	for(i=0;i<nx-1;i++)	for(j=0;j<ny;j++)
	{
		ss = true;
		for(k=0;k<nz;k++)
		{
			i0 = i+nx*(j+ny*k);
			if(tt[i0] && tt[i0+1])
			{
				if(ss)	{	glBegin(GL_QUAD_STRIP);	ss = false;	}
#ifdef NO_COLOR_ARRAY
				if(cc)	glColor4f(cc[4*i0],cc[4*i0+1],cc[4*i0+2],cc[4*i0+3]);
#endif
				glArrayElement(i0);
#ifdef NO_COLOR_ARRAY
				if(cc)	glColor4f(cc[4*i0+4],cc[4*i0+5],cc[4*i0+6],cc[4*i0+7]);
#endif
				glArrayElement(i0+1);
			}
			else if(!ss)	{	glEnd();	ss = true;	}
		}
		if(!ss)	glEnd();
	}
	for(k=0;k<nz;k++)	for(j=0;j<ny-1;j++)
	{
		ss = true;
		for(i=0;i<nx;i++)
		{
			i0 = i+nx*(j+ny*k);
			if(tt[i0] && tt[i0+nx])
			{
				if(ss)	{	glBegin(GL_QUAD_STRIP);	ss = false;	}
#ifdef NO_COLOR_ARRAY
				if(cc)	glColor4f(cc[4*i0],cc[4*i0+1],cc[4*i0+2],cc[4*i0+3]);
#endif
				glArrayElement(i0);
#ifdef NO_COLOR_ARRAY
				if(cc)	glColor4f(cc[4*(i0+nx)],cc[4*(i0+nx)+1],cc[4*(i0+nx)+2],cc[4*(i0+nx)+3]);
#endif
				glArrayElement(i0+nx);
			}
			else if(!ss)	{	glEnd();	ss = true;	}
		}
		if(!ss)	glEnd();
	}
	for(k=0;k<nz-1;k++)	for(j=0;j<ny;j++)
	{
		ss = true;
		for(i=0;i<nx;i++)
		{
			i0 = i+nx*(j+ny*k);
			if(tt[i0] && tt[i0+nn])
			{
				if(ss)	{	glBegin(GL_QUAD_STRIP);	ss = false;	}
#ifdef NO_COLOR_ARRAY
				if(cc)	glColor4f(cc[4*i0],cc[4*i0+1],cc[4*i0+2],cc[4*i0+3]);
#endif
				glArrayElement(i0);
#ifdef NO_COLOR_ARRAY
				if(cc)	glColor4f(cc[4*(i0+nn)],cc[4*(i0+nn)+1],cc[4*(i0+nn)+2],cc[4*(i0+nn)+3]);
#endif
				glArrayElement(i0+nn);
			}
			else if(!ss)	{	glEnd();	ss = true;	}
		}
		if(!ss)	glEnd();
	}
	glDisableClientState(GL_VERTEX_ARRAY);
#ifndef NO_COLOR_ARRAY
	glDisableClientState(GL_COLOR_ARRAY);
#endif
	glFlush();
	delete []cc;	delete []tt;
}
//-----------------------------------------------------------------------------
void mglGraphGL::surf3_plot(long n,long m,long *kx1,long *kx2,long *ky1,long *ky2,
							long *kz,float *pp,float *cc,float *kk,float *nn,
							bool wire)
{
	if(!DrawFace && !wire)	return;
	register long i,j,k,i0,ii,jj=0;
	long id[8],us[8],ni,k1,k2;
	float d,d0;
	if(nn)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, nn);
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pp);
#ifndef NO_COLOR_ARRAY
	if(cc)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, 0, cc);
	}
#endif
	if(!wire)
	{
		glColorMaterial(GL_FRONT_AND_BACK, MGL_HOW);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mgl_spc);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mgl_spf);
	}

	for(i=0;i<n-1;i++)	for(j=0;j<m-1;j++)
	{
		i0 = i+n*j;
		memset(id,-1,8*sizeof(long));	ni = 0;
		memset(us,0,8*sizeof(long));
		if(kx1[i0]>=0)		id[ni++] = kx1[i0];
		if(ky1[i0]>=0)		id[ni++] = ky1[i0];
		if(kx1[i0+n]>=0)	id[ni++] = kx1[i0+n];
		if(ky1[i0+1]>=0)	id[ni++] = ky1[i0+1];
		if(kz[i0]>=0)		id[ni++] = kz[i0];
		if(kz[i0+1]>=0)		id[ni++] = kz[i0+1];
		if(kz[i0+n+1]>=0)	id[ni++] = kz[i0+n+1];
		if(kz[i0+n]>=0)		id[ni++] = kz[i0+n];
		if(kx2[i0]>=0)		id[ni++] = kx2[i0];
		if(ky2[i0]>=0)		id[ni++] = ky2[i0];
		if(kx2[i0+n]>=0)	id[ni++] = kx2[i0+n];
		if(ky2[i0+1]>=0)	id[ni++] = ky2[i0+1];

		if(ni>2)
		{
			glBegin(wire ? GL_LINE_LOOP : GL_TRIANGLE_FAN);
			k1 = k2 = 0;
			if(cc)	glColor4f(cc[4*id[0]],cc[4*id[0]+1],cc[4*id[0]+2],cc[4*id[0]+3]);
			glArrayElement(id[0]);	us[0]=1;
			for(k=1;k<ni;k++)
			{
				i0 = -1;	d0 = -2;
				for(ii=1;ii<ni;ii++)
				{
					if(us[ii])	continue;
					d = k>1 ? mgl_cos_pp(kk,id[0],id[ii],id[jj]) :
							-mgl_cos_pp(kk,id[0],id[ii],id[1]);
					if(d>d0)	{	d0=d;	i0=ii;	}
				}
				if(i0<0)	break;
				jj = i0;	us[jj]=1;
				if(k1==0)	k1 = jj;	else	if(k2==0)	k2 = jj;
#ifdef NO_COLOR_ARRAY
				if(cc)	glColor4f(cc[4*id[jj]],cc[4*id[jj]+1],cc[4*id[jj]+2],cc[4*id[jj]+3]);
#endif
				glArrayElement(id[jj]);
			}
#ifdef NO_COLOR_ARRAY
			if(cc)	glColor4f(cc[4*id[0]],cc[4*id[0]+1],cc[4*id[0]+2],cc[4*id[0]+3]);
#endif
			glArrayElement(id[0]);
			//normal(pp+id[0],pp+id[k1],pp+id[k2]);
			glEnd();
		}
	}
	glDisableClientState(GL_VERTEX_ARRAY);
#ifndef NO_COLOR_ARRAY
	if(cc)	glDisableClientState(GL_COLOR_ARRAY);
#endif
	if(nn)	glDisableClientState(GL_NORMAL_ARRAY);

}
//-----------------------------------------------------------------------------
void mglGraphGL::axial_plot(long n,float *pp,long *nn,long np, bool wire)
{
	if(!DrawFace && !wire)	return;
	register long i,j;
	float p1[6],n1[6];
	bool ss,gg;
	float r1,r2,d,y1,y2,c1,s1,ph;

	long k0=0,k1=1,k2=2;
	if(AxialDir=='y')	{	k0=0;k1=2;k2=1;	}
	if(AxialDir=='x')	{	k0=2;k1=1;k2=0;	}

	glColorMaterial(GL_FRONT_AND_BACK, MGL_HOW);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mgl_spc);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mgl_spf);
	if(!pp || !nn)	return;

	for(i=0;i<n;i++)
	{
		if(nn[i]<0)	continue;
		r1 = pp[3*i];		y1 = pp[3*i+1];
		r2 = pp[3*nn[i]];	y2 = pp[3*nn[i]+1];
		ss = true;
		for(j=0;j<=np;j++)
		{
			ph = 2.*j*M_PI/np;
			c1 = cos(ph);	s1 = sin(ph);
			p1[k0] = r1*c1;		p1[k2] = y1;	p1[k1] = r1*s1;
			p1[k0+3] = r2*c1;	p1[k2+3] = y2;	p1[k1+3] = r2*s1;
			d = y2-y1;
			n1[k0] = -d*c1;	n1[k2] = r2-r1;		n1[k1] = -d*s1;
			if(!wire && nn[nn[i]]>=0)
			{
				d = pp[3*nn[nn[i]]+1]-y2;
				n1[k0+3] = -d*c1;	n1[k2+3] = pp[3*nn[nn[i]]]-r2;	n1[k1+3] = -d*s1;
			}
			else
			{	n1[3] = n1[0];	n1[4] = n1[1];	n1[5] = n1[2];	}
			gg = ScalePoint(p1[0],p1[1],p1[2]) && ScalePoint(p1[3],p1[4],p1[5]);

			if(!gg && !ss)	{	glEnd();	ss = true;	}
			if(gg)
			{
				if(ss)	glBegin(wire ? GL_LINE_LOOP : GL_QUAD_STRIP);
				ss = false;
				glNormal3f(n1[0],n1[1],n1[2]);
				glVertex3f(p1[0],p1[1],p1[2]);
				glNormal3f(n1[3],n1[4],n1[5]);
				glVertex3f(p1[3],p1[4],p1[5]);
			}
		}
		if(!ss)	{	glEnd();	ss = true;	}
	}
}
//-----------------------------------------------------------------------------
void mglGraphGL::trigs_plot(long n, long *nn, long /*m*/, float *pp, float *cc, bool *tt,bool wire)
{
	if(!DrawFace && !wire)	return;

	register long i,i0,j1,j2,j3;
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pp);
#ifndef NO_COLOR_ARRAY
	if(cc)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, 0, cc);
	}
#endif
	if(wire)
	{
		glBegin(GL_LINES);
		for(i=0;i<n;i++)
		{
			j1 = nn[3*i];	j2 = nn[3*i+1];	j3 = nn[3*i+2];
			if(tt && (!tt[j1] || !tt[j2] || !tt[j3]))	continue;
			i0 = 16*i;
#ifdef NO_COLOR_ARRAY
			if(cc)	glColor4f(cc[4*j1],cc[4*j1+1],cc[4*j1+2],cc[4*j1+3]);
			glArrayElement(j1);
			if(cc)	glColor4f(cc[4*j2],cc[4*j2+1],cc[4*j2+2],cc[4*j2+3]);
			glArrayElement(j2);
			if(cc)	glColor4f(cc[4*j2],cc[4*j2+1],cc[4*j2+2],cc[4*j2+3]);
			glArrayElement(j2);
			if(cc)	glColor4f(cc[4*j3],cc[4*j3+1],cc[4*j3+2],cc[4*j3+3]);
			glArrayElement(j3);
			if(cc)	glColor4f(cc[4*j3],cc[4*j3+1],cc[4*j3+2],cc[4*j3+3]);
			glArrayElement(j3);
			if(cc)	glColor4f(cc[4*j1],cc[4*j1+1],cc[4*j1+2],cc[4*j1+3]);
			glArrayElement(j1);
#else
			glArrayElement(j1);		glArrayElement(j2);
			glArrayElement(j2);		glArrayElement(j3);
			glArrayElement(j3);		glArrayElement(j1);
#endif
		}
	}
	else
	{
		glBegin(GL_TRIANGLES);
		for(i=0;i<n;i++)
		{
			j1 = nn[3*i];	j2 = nn[3*i+1];	j3 = nn[3*i+2];
			if(tt && (!tt[j1] || !tt[j2] || !tt[j3]))	continue;
			i0 = 16*i;
#ifdef NO_COLOR_ARRAY
			if(cc)	glColor4f(cc[4*j1],cc[4*j1+1],cc[4*j1+2],cc[4*j1+3]);
			glArrayElement(j1);
			if(cc)	glColor4f(cc[4*j2],cc[4*j2+1],cc[4*j2+2],cc[4*j2+3]);
			glArrayElement(j2);
			if(cc)	glColor4f(cc[4*j3],cc[4*j3+1],cc[4*j3+2],cc[4*j3+3]);
			glArrayElement(j3);
#else
			glArrayElement(j1);	glArrayElement(j2);	glArrayElement(j3);
#endif
		}
	}
	glEnd();
	glDisableClientState(GL_VERTEX_ARRAY);
#ifndef NO_COLOR_ARRAY
	if(cc)	glDisableClientState(GL_COLOR_ARRAY);
#endif
}
//-----------------------------------------------------------------------------
void mglGraphGL::quads_plot(long n,float *pp,float *cc,bool *tt)
{
	if(!DrawFace)	return;
	register long i,i0;
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pp);
#ifndef NO_COLOR_ARRAY
	if(cc)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, 0, cc);
	}
#endif
	glBegin(GL_QUADS);
	for(i=0;i<n;i++)
	{
		if(tt && (!tt[4*i] || !tt[4*i+1] || !tt[4*i+1] || !tt[4*i+1]))
			continue;
		i0 = 16*i;
#ifdef NO_COLOR_ARRAY
		if(cc)	glColor4f(cc[i0],cc[i0+1],cc[i0+2],cc[i0+3]);
		glArrayElement(4*i);
		if(cc)	glColor4f(cc[i0+4],cc[i0+5],cc[i0+6],cc[i0+7]);
		glArrayElement(4*i+1);
		if(cc)	glColor4f(cc[i0+8],cc[i0+9],cc[i0+10],cc[i0+11]);
		glArrayElement(4*i+2);
		if(cc)	glColor4f(cc[i0+12],cc[i0+13],cc[i0+14],cc[i0+15]);
		glArrayElement(4*i+3);
#else
		glArrayElement(4*i);	glArrayElement(4*i+1);
		glArrayElement(4*i+2);	glArrayElement(4*i+3);
#endif
	}
	glEnd();
	glDisableClientState(GL_VERTEX_ARRAY);
#ifndef NO_COLOR_ARRAY
	if(cc)	glDisableClientState(GL_COLOR_ARRAY);
#endif
}
//-----------------------------------------------------------------------------
void mglGraphGL::lines_plot(long n,float *pp,float *cc,bool *tt)
{
	register long i;
	mglColor c;
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pp);
	glBegin(GL_LINES);
	for(i=0;i<n;i++)
	{
		if(tt && (!tt[2*i] || !tt[2*i+1]))	continue;
		if(cc)
		{
			if(cc[i]==0)	continue;
			c = GetC(cc[i]-0.5,false);
			glColor4f(c.r, c.g, c.b, AlphaDef);
		}
		else	glColor3f(cmap[0].r,cmap[0].g,cmap[0].b);
		glArrayElement(2*i);
		if(cc)
		{
			c = GetC(cc[i],false);
			glColor4f(c.r, c.g, c.b, AlphaDef);
		}
		else	glColor3f(cmap[1].r,cmap[1].g,cmap[1].b);
		glArrayElement(2*i+1);
	}
	glEnd();
	glDisableClientState(GL_VERTEX_ARRAY);
}
//-----------------------------------------------------------------------------
void mglGraphGL::normal(long nx,long ny,float *pp,float *nn)
{
	register long i,j,i1,i0;
	float ax,ay,az,bx,by,bz;
	for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
	{
		i1 = 3*((i<nx-1 ? i:i-1) + nx*(j<ny-1 ? j:j-1));
		ax=pp[i1+3]-pp[i1];		ay=pp[i1+4]-pp[i1+1];	az=pp[i1+5]-pp[i1+2];
		i0 = i1+3*nx;
		bx=pp[i0]-pp[i1];		by=pp[i0+1]-pp[i1+1];	bz=pp[i0+2]-pp[i1+2];
		i0 = 3*(i+nx*j);
		nn[i0] = ay*bz-az*by;	nn[i0+1] = az*bx-ax*bz;	nn[i0+2] = ax*by-ay*bx;
	}
}
//-----------------------------------------------------------------------------
void mglGraphGL::normal(float *p1,float *p2,float *p3)
{
	float n[3];
	float ax=p2[0]-p1[0],ay=p2[1]-p1[1],az=p2[2]-p1[2];
	float bx=p3[0]-p1[0],by=p3[1]-p1[1],bz=p3[2]-p1[2];
	n[0] = ay*bz-az*by;
	n[1] = az*bx-ax*bz;
	n[2] = ax*by-ay*bx;
	glNormal3fv(n);
}
//-----------------------------------------------------------------------------
mglGraphGL::~mglGraphGL(){}
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
