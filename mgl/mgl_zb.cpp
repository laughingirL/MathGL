/* mgl_zb.cpp is part of Math Graphic Library
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
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "mgl/mgl_zb.h"
#include "mgl/mgl_c.h"
#include "mgl/mgl_f.h"
#define imax(a,b)	(a)>(b) ? (a) : (b)
#define imin(a,b)	(a)<(b) ? (a) : (b)

//#define MGL_ABUF_8

//#define MGL_SIMPLE_LINE
//-----------------------------------------------------------------------------
/// Create mglGraph object in ZBuffer mode.
HMGL mgl_create_graph_zb(int width, int height)
{    return new mglGraphZB(width,height);	}
/// Create mglGraph object in ZBuffer mode.
long mgl_create_graph_zb_(int *width, int *height)
{    return long(new mglGraphZB(*width,*height));	}
//-----------------------------------------------------------------------------
void mglGraphZB::Ball(float x,float y,float z,mglColor col,float alpha)
{
	unsigned char r[4];
	Finished = false;
	alpha = Transparent ? alpha : 1;
	float p[3] = {x,y,z}, v, u = 1./(9*PenWidth*PenWidth);
	PostScale(p,1);
	r[0] = (unsigned char)(255.f*col.r);
	r[1] = (unsigned char)(255.f*col.g);
	r[2] = (unsigned char)(255.f*col.b);

	bool aa=UseAlpha;	UseAlpha = true;
	register long i,j;
	long w = long(5.5+PenWidth);
	for(i=-w;i<=w;i++)	for(j=-w;j<=w;j++)
	{
		v = (i*i+j*j)*u;
		r[3] = (unsigned char)(255.f*alpha*exp(-6.f*v));
		if(r[3]==0)	continue;
		pnt_plot(long(p[0])+i,long(p[1])+j,p[2],r);
	}
	UseAlpha = aa;
}
//-----------------------------------------------------------------------------
void mglGraphZB::ball(float *p,float *c)
{
	unsigned char r[4];
	Finished = false;
	r[0] = (unsigned char)(255.f*c[0]);
	r[1] = (unsigned char)(255.f*c[1]);
	r[2] = (unsigned char)(255.f*c[2]);

	bool aa=UseAlpha;	UseAlpha = true;
	register long i,j;
	long w = long(5.5+PenWidth);
	float v, u = 1./(4*PenWidth*PenWidth);
	for(i=-w;i<=w;i++)	for(j=-w;j<=w;j++)
	{
		v = (i*i+j*j)*u;
		r[3] = (unsigned char)(255.f*c[3]*exp(-6.f*v));
		if(r[3]==0)	continue;
		pnt_plot(long(p[0])+i,long(p[1])+j,p[2],r);
	}
	UseAlpha = aa;
}
//-----------------------------------------------------------------------------
unsigned char* mglGraphZB::col2int(mglColor c,float a,unsigned char *r)
{
	float cc[4] = {c.r,c.g,c.b,a};
	return col2int(cc,0,r);
}
//-----------------------------------------------------------------------------
unsigned char* mglGraphZB::col2int(float *c,float *n,unsigned char *r)
{
	register long i,j;
	static unsigned char u[4];
	register float b0=c[0],b1=c[1],b2=c[2];
	if(r==0) r = u;
	if(c[3]<=0)	{	memset(r,0,4*sizeof(unsigned char));	return r;	}
	if(UseLight && n)
	{
		float d0,d1,d2,nn;
		b0 *= AmbBr;		b1 *= AmbBr;		b2 *= AmbBr;
		for(i=0;i<10;i++)
		{
			if(!nLight[i])	continue;
			j = 3*i;
			nn = 2*(n[0]*pLight[j]+n[1]*pLight[j+1]+n[2]*pLight[j+2]) /
					(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);
			d0 = pLight[j] - n[0]*nn;
			d1 = pLight[j+1]-n[1]*nn;
			d2 = pLight[j+2]-n[2]*nn;
			nn = 1 + d2/sqrt(d0*d0+d1*d1+d2*d2);

			nn = exp(-aLight[i]*nn)*bLight[i]*2;
			b0 += nn*cLight[j];
			b1 += nn*cLight[j+1];
			b2 += nn*cLight[j+2];
		}
		b0 = b0<1 ? b0 : 1;
		b1 = b1<1 ? b1 : 1;
		b2 = b2<1 ? b2 : 1;
	}
	r[0] = (unsigned char)(255.f*b0);	r[1] = (unsigned char)(255.f*b1);
	r[2] = (unsigned char)(255.f*b2);
	// c[3] should be <1 but I additionally check it here
	r[3] = UseAlpha && c[3]<1 ? (unsigned char)(255.f*c[3]) : 255;
	return r;
}
//-----------------------------------------------------------------------------
void mglGraphZB::pnt_plot(long x,long y,float z,unsigned char c[4])
{
	long i0=x+Width*(Height-1-y), n=Width*Height;
	if(x<0 || x>=Width || y<0 || y>=Height)	return;
	float *zz = Z+i0;
#ifdef MGL_ABUF_8
	unsigned char *cc = C+4*i0,*cf=cc+28*n;
#else
	unsigned char *cc = C+4*i0,*cf=cc+12*n;
#endif
	
	float zf = FogDist*(z/Depth-0.5-FogDz);
	if(zf<0)
	{
		int d = int(255.f-255.f*exp(5.f*zf));
		unsigned char cb[4] = {BDef[0], BDef[1], BDef[2], d};
		if(d==255)	return;
		combine(c,cb);
	}
	if(DrawFace || !FastNoFace)
	{
#ifdef MGL_ABUF_8
		if(z>zz[3*n])
		{
#endif
			if(z>zz[n])
			{
#ifdef MGL_ABUF_8
				combine(cf,cc+24*n);	zz[7*n] = zz[6*n];
				zz[6*n] = zz[5*n];		memcpy(cc+24*n,cc+20*n,4);
				zz[5*n] = zz[4*n];		memcpy(cc+20*n,cc+16*n,4);
				zz[4*n] = zz[3*n];		memcpy(cc+16*n,cc+12*n,4);
				zz[3*n] = zz[2*n];		memcpy(cc+12*n,cc+8*n,4);
#else
				combine(cf,cc+8*n);		zz[3*n] = zz[2*n];
#endif
				zz[2*n] = zz[1*n];		memcpy(cc+8*n,cc+4*n,4);
				if(z>zz[0])	// shift point on slice down and paste new point
				{
					zz[1*n] = zz[0*n];		memcpy(cc+4*n,cc,4);
					zz[0] = z;				memcpy(cc,c,4);
				}
				else	// shift point on slice down and paste new point
				{
					zz[n] = z;				memcpy(cc+4*n,c,4);
				}
			}
			else
			{
				if(z>zz[2*n])	// shift point on slice down and paste new point
				{
#ifdef MGL_ABUF_8
					combine(cf,cc+24*n);	zz[7*n] = zz[6*n];
					zz[6*n] = zz[5*n];		memcpy(cc+24*n,cc+20*n,4);
					zz[5*n] = zz[4*n];		memcpy(cc+20*n,cc+16*n,4);
					zz[4*n] = zz[3*n];		memcpy(cc+16*n,cc+12*n,4);
					zz[3*n] = zz[2*n];		memcpy(cc+12*n,cc+8*n,4);
#else
					combine(cf,cc+8*n);		zz[3*n] = zz[2*n];
#endif
					zz[2*n] = z;			memcpy(cc+8*n,c,4);
				}
				else	// shift point on slice down and paste new point
				{
#ifdef MGL_ABUF_8
					combine(cf,cc+24*n);	zz[7*n] = zz[6*n];
					zz[6*n] = zz[5*n];		memcpy(cc+24*n,cc+20*n,4);
					zz[5*n] = zz[4*n];		memcpy(cc+20*n,cc+16*n,4);
					zz[4*n] = zz[3*n];		memcpy(cc+16*n,cc+12*n,4);
					zz[3*n] = z;			memcpy(cc+12*n,c,4);
#else
					if(z>zz[3*n])	// point upper the background
					{	zz[3*n]=z;		combine(cf,c);	}
					else			// point below the background
					{	combine(c,cf);	memcpy(cf,c,4);	}
#endif
				}
			}
#ifdef MGL_ABUF_8
		}
		else
		{
			if(z>zz[5*n])
			{
				if(z>zz[4*n])	// shift point on slice down and paste new point
				{
					combine(cf,cc+24*n);	zz[7*n] = zz[6*n];
					zz[6*n] = zz[5*n];		memcpy(cc+24*n,cc+20*n,4);
					zz[5*n] = zz[4*n];		memcpy(cc+20*n,cc+16*n,4);
					zz[4*n] = z;			memcpy(cc+16*n,c,4);
				}
				else	// shift point on slice down and paste new point
				{
					combine(cf,cc+24*n);	zz[7*n] = zz[6*n];
					zz[6*n] = zz[5*n];		memcpy(cc+24*n,cc+20*n,4);
					zz[5*n] = z;			memcpy(cc+20*n,c,4);
				}
			}
			else
			{
				if(z>zz[6*n])	// shift point on slice down and paste new point
				{
					combine(cf,cc+24*n);	zz[7*n] = zz[6*n];
					zz[6*n] = z;			memcpy(cc+24*n,c,4);
				}
				else	if(z>zz[7*n])	// point upper the background
				{	zz[7*n]=z;		combine(cf,c);	}
				else			// point below the background
				{	combine(c,cf);	memcpy(cf,c,4);	}
			}
		}
#endif
	}
	else
	{
		if(z>zz[0])	// point upper the background
		{	zz[0]=z;		memcpy(cc,c,4);	}
	}
}
//-----------------------------------------------------------------------------
void mglGraphZB::Finish()
{
	register long i,n=Width*Height,i0;
//	unsigned char c[4],def=TranspType!=2 ? 255:0,alf=TranspType!=2 ? 0:255,*cc;
	unsigned char c[4],alf=TranspType!=2 ? 0:255,*cc;
	if(DrawFace || !FastNoFace)	for(i=0;i<n;i++)
	{
		i0 = 4*i;	cc = C+i0;
		c[0]=BDef[0];	c[1]=BDef[1];	c[2]=BDef[2];	c[3]=alf;
#ifdef MGL_ABUF_8
		combine(c,cc+28*n);		combine(c,cc+24*n);
		combine(c,cc+20*n);		combine(c,cc+16*n);
#endif
		combine(c,cc+12*n);		combine(c,cc+8*n);
		combine(c,cc+4*n);		combine(c,cc);
		memcpy(G4+i0,c,4);
		c[0]=BDef[0];	c[1]=BDef[1];	c[2]=BDef[2];	c[3]=255;
		combine(c,G4+i0);	memcpy(G+3*i,c,3);
	}
	else 	for(i=0;i<n;i++)
	{
		i0 = 4*i;	cc = C+i0;
		c[0]=BDef[0];	c[1]=BDef[1];	c[2]=BDef[2];	c[3]=alf;
		combine(c,cc);		memcpy(G4+i0,c,4);
		c[0]=BDef[0];	c[1]=BDef[1];	c[2]=BDef[2];	c[3]=255;
		combine(c,G4+i0);	memcpy(G+3*i,c,3);
	}
	Finished = true;
}
//-----------------------------------------------------------------------------
void mglGraphZB::Clf(mglColor Back)
{
	Fog(0);
	pPos = CurrPal = 0;		PDef = 0xffff;
	if(Back==NC)		Back = mglColor(1,1,1);
	if(TranspType==2)	Back = mglColor(0,0,0);
	col2int(Back,1,BDef);
	register long i,n=Width*Height;
	memset(C,0,32*n);
	for(i=0;i<n;i++)	Z[i] = -1e20f;
//	for(i=1;i<Height;i++)	memcpy(Z+i*Width,Z,Width*sizeof(float));
	for(i=1;i<8;i++)	memcpy(Z+i*n,Z,n*sizeof(float));
	Finished = false;
}
//-----------------------------------------------------------------------------
/* Bilinear interpolation r(u,v) = r0 + (r1-r0)*u + (r2-20)*v + (r3+30-r1-r2)*u*v is used (where r is one of {x,y,z,R,G,B,A}. Variables u,v are determined for each point (x,y) and selected one pair which 0<u<1 and 0<v<1.*/
void mglGraphZB::quad_plot(float *pp0,float *pp1,float *pp2,float *pp3,
					float *cc0,float *cc1,float *cc2,float *cc3)
{
	unsigned char r[4];
	long y1,x1,y2,x2;
	float d1[3],d2[3],d3[3],c1[4],c2[4],c3[4];
	float cs[4],ns[3],dd,dsx,dsy;

/*	ps[0] = (pp0[0]+pp1[0]+pp2[0]+pp3[0])/4;
	ps[1] = (pp0[1]+pp1[1]+pp2[1]+pp3[1])/4;
	ps[2] = (pp0[2]+pp1[2]+pp2[2]+pp3[2])/4;
	cs[0] = (cc0[0]+cc1[0]+cc2[0]+cc3[0])/4;
	cs[1] = (cc0[1]+cc1[1]+cc2[1]+cc3[1])/4;
	cs[2] = (cc0[2]+cc1[2]+cc2[2]+cc3[2])/4;
	cs[3] = (cc0[3]+cc1[3]+cc2[3]+cc3[3])/4;
	trig_plot(ps,pp0,pp1,cs,cc0,cc1);
	trig_plot(ps,pp3,pp1,cs,cc3,cc1);
	trig_plot(ps,pp2,pp3,cs,cc2,cc3);
	trig_plot(ps,pp0,pp2,cs,cc0,cc3);
	return;*/

	d1[0] = pp1[0]-pp0[0];	d2[0] = pp2[0]-pp0[0];	d3[0] = pp3[0]+pp0[0]-pp1[0]-pp2[0];
	d1[1] = pp1[1]-pp0[1];	d2[1] = pp2[1]-pp0[1];	d3[1] = pp3[1]+pp0[1]-pp1[1]-pp2[1];
	d1[2] = pp1[2]-pp0[2];	d2[2] = pp2[2]-pp0[2];	d3[2] = pp3[2]+pp0[2]-pp1[2]-pp2[2];
	c1[0] = cc1[0]-cc0[0];	c2[0] = cc2[0]-cc0[0];	c3[0] = cc3[0]+cc0[0]-cc1[0]-cc2[0];
	c1[1] = cc1[1]-cc0[1];	c2[1] = cc2[1]-cc0[1];	c3[1] = cc3[1]+cc0[1]-cc1[1]-cc2[1];
	c1[2] = cc1[2]-cc0[2];	c2[2] = cc2[2]-cc0[2];	c3[2] = cc3[2]+cc0[2]-cc1[2]-cc2[2];
	c1[3] = cc1[3]-cc0[3];	c2[3] = cc2[3]-cc0[3];	c3[3] = cc3[3]+cc0[3]-cc1[3]-cc2[3];

	x1 = imin(imin(long(pp0[0]),long(pp3[0])),imin(long(pp1[0]),long(pp2[0])));	// bounding box
	y1 = imin(imin(long(pp0[1]),long(pp3[1])),imin(long(pp1[1]),long(pp2[1])));
	x2 = imax(imax(long(pp0[0]),long(pp3[0])),imax(long(pp1[0]),long(pp2[0])));
	y2 = imax(imax(long(pp0[1]),long(pp3[1])),imax(long(pp1[1]),long(pp2[1])));

	dd=d1[0]*d2[1]-d1[1]*d2[0];
	dsx =-4*(d2[1]*d3[0] - d2[0]*d3[1])*d1[1];
	dsy = 4*(d2[1]*d3[0] - d2[0]*d3[1])*d1[0];

	register long i,j,g;
	register float u,v,s,xx,yy,q;
	for(i=x1;i<=x2;i++)	for(j=y1;j<=y2;j++)
	{
		xx = (i-pp0[0]);	yy = (j-pp0[1]);
		s = dsx*xx + dsy*yy + (dd+d3[1]*xx-d3[0]*yy)*(dd+d3[1]*xx-d3[0]*yy);
		if(s<0)	continue;	// no solution
		s = sqrt(s);
		q = d3[0]*yy - d3[1]*xx + dd + s;
		u = q ? 2.f*(d2[1]*xx - d2[0]*yy)/q : -1.f;
		q = d3[1]*xx - d3[0]*yy + dd + s;
		v = q ? 2.f*(d1[0]*yy - d1[1]*xx)/q : -1.f;
		g = u<0.f || u>1.f || v<0.f || v>1.f;
		if(g)	// first root bad
		{
			q = d3[0]*yy - d3[1]*xx + dd - s;
			u = q ? 2.f*(d2[1]*xx - d2[0]*yy)/q : -1.f;
			q = d3[1]*xx - d3[0]*yy + dd - s;
			v = q ? 2.f*(d1[0]*yy - d1[1]*xx)/q : -1.f;
			g = u<0.f || u>1.f || v<0.f || v>1.f;
			if(g)	continue;	// second root bad
		}
		s = u*v;
		cs[0] = cc0[0]+c1[0]*u+c2[0]*v+c3[0]*s;
		cs[1] = cc0[1]+c1[1]*u+c2[1]*v+c3[1]*s;
		cs[2] = cc0[2]+c1[2]*u+c2[2]*v+c3[2]*s;
		cs[3] = cc0[3]+c1[3]*u+c2[3]*v+c3[3]*s;
		ns[0]=(d2[2]+d3[2]*u)*(d1[1]+d3[1]*v)-(d2[1]+d3[1]*u)*(d1[2]+d3[2]*v);	// normal vector
		ns[1]=(d2[0]+d3[0]*u)*(d1[2]+d3[2]*v)-(d2[2]+d3[2]*u)*(d1[0]+d3[0]*v);
		ns[2]=(d2[1]+d3[1]*u)*(d1[0]+d3[0]*v)-(d2[0]+d3[0]*u)*(d1[1]+d3[1]*v);
		pnt_plot(i,j,pp0[2]+d1[2]*u+d2[2]*v+d3[2]*s,col2int(cs,ns,r));
	}
}
//-----------------------------------------------------------------------------
/* Bilinear interpolation r(u,v) = r0 + (r1-r0)*u + (r2-20)*v + (r3+30-r1-r2)*u*v is used (where r is one of {x,y,z,R,G,B,A}. Variables u,v are determined 	for each point (x,y) and selected one pair which 0<u<1 and 0<v<1. 	In difference mglGraphZB::quad_plot the normal is also interpolated.*/
void mglGraphZB::quad_plot_n(float *pp0,float *pp1,float *pp2,float *pp3,
					float *cc0,float *cc1,float *cc2,float *cc3,
					float *nn0,float *nn1,float *nn2,float *nn3)
{
	unsigned char r[4];
	long y1,x1,y2,x2;
	float d1[3],d2[3],d3[3],c1[4],c2[4],c3[4],n1[3],n2[3],n3[3];
	float cs[4],ns[3],dd,dsx,dsy;

	d1[0] = pp1[0]-pp0[0];	d2[0] = pp2[0]-pp0[0];	d3[0] = pp3[0]+pp0[0]-pp1[0]-pp2[0];
	d1[1] = pp1[1]-pp0[1];	d2[1] = pp2[1]-pp0[1];	d3[1] = pp3[1]+pp0[1]-pp1[1]-pp2[1];
	d1[2] = pp1[2]-pp0[2];	d2[2] = pp2[2]-pp0[2];	d3[2] = pp3[2]+pp0[2]-pp1[2]-pp2[2];
	c1[0] = cc1[0]-cc0[0];	c2[0] = cc2[0]-cc0[0];	c3[0] = cc3[0]+cc0[0]-cc1[0]-cc2[0];
	c1[1] = cc1[1]-cc0[1];	c2[1] = cc2[1]-cc0[1];	c3[1] = cc3[1]+cc0[1]-cc1[1]-cc2[1];
	c1[2] = cc1[2]-cc0[2];	c2[2] = cc2[2]-cc0[2];	c3[2] = cc3[2]+cc0[2]-cc1[2]-cc2[2];
	c1[3] = cc1[3]-cc0[3];	c2[3] = cc2[3]-cc0[3];	c3[3] = cc3[3]+cc0[3]-cc1[3]-cc2[3];
	n1[0] = nn1[0]-nn0[0];	n2[0] = nn2[0]-nn0[0];	n3[0] = nn3[0]+nn0[0]-nn1[0]-nn2[0];
	n1[1] = nn1[1]-nn0[1];	n2[1] = nn2[1]-nn0[1];	n3[1] = nn3[1]+nn0[1]-nn1[1]-nn2[1];
	n1[2] = nn1[2]-nn0[2];	n2[2] = nn2[2]-nn0[2];	n3[2] = nn3[2]+nn0[2]-nn1[2]-nn2[2];

	x1 = imin(imin(long(pp0[0]),long(pp3[0])),imin(long(pp1[0]),long(pp2[0])));	// bounding box
	y1 = imin(imin(long(pp0[1]),long(pp3[1])),imin(long(pp1[1]),long(pp2[1])));
	x2 = imax(imax(long(pp0[0]),long(pp3[0])),imax(long(pp1[0]),long(pp2[0])));
	y2 = imax(imax(long(pp0[1]),long(pp3[1])),imax(long(pp1[1]),long(pp2[1])));

	dd=d1[0]*d2[1]-d1[1]*d2[0];
	dsx =-4*(d2[1]*d3[0] - d2[0]*d3[1])*d1[1];
	dsy = 4*(d2[1]*d3[0] - d2[0]*d3[1])*d1[0];

	register long i,j,g;
	register float u,v,s,xx,yy,q;
	for(i=x1;i<=x2;i++)	for(j=y1;j<=y2;j++)
	{
		xx = (i-pp0[0]);	yy = (j-pp0[1]);
		s = dsx*xx + dsy*yy + (dd+d3[1]*xx-d3[0]*yy)*(dd+d3[1]*xx-d3[0]*yy);
		if(s<0)	continue;	// no solution
		s = sqrt(s);
		q = d3[0]*yy - d3[1]*xx + dd + s;
		u = q ? 2.f*(d2[1]*xx - d2[0]*yy)/q : -1.f;
		q = d3[1]*xx - d3[0]*yy + dd + s;
		v = q ? 2.f*(d1[0]*yy - d1[1]*xx)/q : -1.f;
		g = u<0.f || u>1.f || v<0.f || v>1.f;
		if(g)	// first root bad
		{
			q = d3[0]*yy - d3[1]*xx + dd - s;
			u = q ? 2.f*(d2[1]*xx - d2[0]*yy)/q : -1.f;
			q = d3[1]*xx - d3[0]*yy + dd - s;
			v = q ? 2.f*(d1[0]*yy - d1[1]*xx)/q : -1.f;
			g = u<0.f || u>1.f || v<0.f || v>1.f;
			if(g)	continue;	// second root bad
		}
		s = u*v;
		cs[0] = cc0[0]+c1[0]*u+c2[0]*v+c3[0]*s;
		cs[1] = cc0[1]+c1[1]*u+c2[1]*v+c3[1]*s;
		cs[2] = cc0[2]+c1[2]*u+c2[2]*v+c3[2]*s;
		cs[3] = cc0[3]+c1[3]*u+c2[3]*v+c3[3]*s;
		ns[0] = nn0[0]+n1[0]*u+n2[0]*v+n3[0]*s;
		ns[1] = nn0[1]+n1[1]*u+n2[1]*v+n3[1]*s;
		ns[2] = nn0[2]+n1[2]*u+n2[2]*v+n3[2]*s;
		pnt_plot(i,j,pp0[2]+d1[2]*u+d2[2]*v+d3[2]*s,col2int(cs,ns,r));
	}
}
//-----------------------------------------------------------------------------
/* Bilinear interpolation r(u,v) = r0 + (r1-r0)*u + (r2-20)*v + (r3+30-r1-r2)*u*v is used (where r is one of {x,y,z,R,G,B,A}. Variables u,v are determined for each point (x,y) and selected one pair which 0<u<1 and 0<v<1.*/
void mglGraphZB::quad_plot_a(float *pp0,float *pp1,float *pp2,float *pp3,
					float aa0,float aa1,float aa2,float aa3, float alpha)
{
	unsigned char r[4];
	long y1,x1,y2,x2;
	float d1[3],d2[3],d3[3],a1,a2,a3;
	float dd,dsx,dsy;

	d1[0] = pp1[0]-pp0[0];	d2[0] = pp2[0]-pp0[0];	d3[0] = pp3[0]+pp0[0]-pp1[0]-pp2[0];
	d1[1] = pp1[1]-pp0[1];	d2[1] = pp2[1]-pp0[1];	d3[1] = pp3[1]+pp0[1]-pp1[1]-pp2[1];
	d1[2] = pp1[2]-pp0[2];	d2[2] = pp2[2]-pp0[2];	d3[2] = pp3[2]+pp0[2]-pp1[2]-pp2[2];
	a1 = aa1-aa0;			a2 = aa2-aa0;			a3 = aa3+aa0-aa1-aa2;

	x1 = imin(imin(long(pp0[0]),long(pp3[0])),imin(long(pp1[0]),long(pp2[0])));	// bounding box
	y1 = imin(imin(long(pp0[1]),long(pp3[1])),imin(long(pp1[1]),long(pp2[1])));
	x2 = imax(imax(long(pp0[0]),long(pp3[0])),imax(long(pp1[0]),long(pp2[0])));
	y2 = imax(imax(long(pp0[1]),long(pp3[1])),imax(long(pp1[1]),long(pp2[1])));

	dd=d1[0]*d2[1]-d1[1]*d2[0];
	dsx =-4*(d2[1]*d3[0] - d2[0]*d3[1])*d1[1];
	dsy = 4*(d2[1]*d3[0] - d2[0]*d3[1])*d1[0];

	register long i,j,k;
	bool g;
	long n = NumCol-1;
	register float u,v,s,xx,yy,q;
	mglColor c;
	for(i=x1;i<=x2;i++)	for(j=y1;j<=y2;j++)
	{
		xx = (i-pp0[0]);	yy = (j-pp0[1]);
		s = dsx*xx + dsy*yy + (dd+d3[1]*xx-d3[0]*yy)*(dd+d3[1]*xx-d3[0]*yy);
		if(s<0)	continue;	// no solution
		s = sqrt(s);
		q = d3[0]*yy - d3[1]*xx + dd + s;
		u = q ? 2.f*(d2[1]*xx - d2[0]*yy)/q : -1.f;
		q = d3[1]*xx - d3[0]*yy + dd + s;
		v = q ? 2.f*(d1[0]*yy - d1[1]*xx)/q : -1.f;
		g = u<0.f || u>1.f || v<0.f || v>1.f;
		if(g)	// first root bad
		{
			q = d3[0]*yy - d3[1]*xx + dd - s;
			u = q ? 2.f*(d2[1]*xx - d2[0]*yy)/q : -1.f;
			q = d3[1]*xx - d3[0]*yy + dd - s;
			v = q ? 2.f*(d1[0]*yy - d1[1]*xx)/q : -1.f;
			g = u<0.f || u>1.f || v<0.f || v>1.f;
			if(g)	continue;	// second root bad
		}
		
		s = aa0 + a1*u + a2*v + a3*u*v;
		r[3] = (unsigned char)(255.f*alpha*(alpha>0 ? (s+1.f)*(s+1.f) : (1.f-s)*(s-1.f)));
		s = (s+1.f)/2.f;	s *= n;
		k = long(s);		s -= k;
		if(k<n)	c = cmap[k]*(1.f-s)+cmap[k+1]*s;
		else	c = cmap[n];
		r[0] = (unsigned char)(255.f*c.r);
		r[1] = (unsigned char)(255.f*c.g);
		r[2] = (unsigned char)(255.f*c.b);
		s = pp0[2]+d1[2]*u+d2[2]*v+d3[2]*u*v;
		pnt_plot(i,j, s, r);
	}
}
//-----------------------------------------------------------------------------
/* Linear interpolation r(u,v) = r0 + (r1-r0)*u + (r2-20)*v is used (where r is one of {x,y,z,R,G,B,A}. Variables u,v are determined for each point (x,y). Point plotted is u>0 and v>0 and u+v<1.*/
void mglGraphZB::trig_plot(float *pp0,float *pp1,float *pp2,
					float *cc0,float *cc1,float *cc2)
{
	unsigned char r[4];
	long y1,x1,y2,x2;
	float d1[3],d2[3],c1[4],c2[4];
	float cs[4],ns[3],dxu,dxv,dyu,dyv;

	d1[0] = pp1[0]-pp0[0];	d2[0] = pp2[0]-pp0[0];
	d1[1] = pp1[1]-pp0[1];	d2[1] = pp2[1]-pp0[1];
	dxu = d2[0]*d1[1] - d1[0]*d2[1];
	if(fabs(dxu)<1e-5)	return;		// points lies on the same line

	d1[2] = pp1[2]-pp0[2];	d2[2] = pp2[2]-pp0[2];
	c1[0] = cc1[0]-cc0[0];	c2[0] = cc2[0]-cc0[0];
	c1[1] = cc1[1]-cc0[1];	c2[1] = cc2[1]-cc0[1];
	c1[2] = cc1[2]-cc0[2];	c2[2] = cc2[2]-cc0[2];
	c1[3] = cc1[3]-cc0[3];	c2[3] = cc2[3]-cc0[3];
	ns[0]=d2[2]*d1[1]-d2[1]*d1[2];					// normal vector
	ns[1]=d2[0]*d1[2]-d2[2]*d1[0];
	ns[2]=d2[1]*d1[0]-d2[0]*d1[1];

	dyv =-d1[0]/dxu;	dxv = d1[1]/dxu;
	dyu = d2[0]/dxu;	dxu =-d2[1]/dxu;

	x1 = imin(long(pp0[0]),imin(long(pp1[0]),long(pp2[0])));	// bounding box
	y1 = imin(long(pp0[1]),imin(long(pp1[1]),long(pp2[1])));
	x2 = imax(long(pp0[0]),imax(long(pp1[0]),long(pp2[0])));
	y2 = imax(long(pp0[1]),imax(long(pp1[1]),long(pp2[1])));

	register float u,v,xx,yy;
	register long i,j,g;
	for(i=x1;i<=x2;i++)	for(j=y1;j<=y2;j++)
	{
		xx = (i-pp0[0]);	yy = (j-pp0[1]);
		u = dxu*xx+dyu*yy;	v = dxv*xx+dyv*yy;
		g = u<0 || v<0 || u+v>1;
		if(g)	continue;

		cs[0] = cc0[0]+c1[0]*u+c2[0]*v;
		cs[1] = cc0[1]+c1[1]*u+c2[1]*v;
		cs[2] = cc0[2]+c1[2]*u+c2[2]*v;
		cs[3] = cc0[3]+c1[3]*u+c2[3]*v;
		pnt_plot(i,j,pp0[2]+d1[2]*u+d2[2]*v,col2int(cs,ns,r));
	}
}
//-----------------------------------------------------------------------------
/* Linear interpolation r(u,v) = r0 + (r1-r0)*u + (r2-20)*v is used (where r is one of {x,y,z,R,G,B,A}. Variables u,v are determined for each point (x,y). Point plotted is u>0 and v>0 and u+v<1.*/
void mglGraphZB::trig_plot_n(float *pp0,float *pp1,float *pp2,
					float *cc0,float *cc1,float *cc2,
					float *nn0,float *nn1,float *nn2)
{
	unsigned char r[4];
	long y1,x1,y2,x2;
	float d1[3],d2[3],c1[4],c2[4],n1[3],n2[3];
	float cs[4],ns[3],dxu,dxv,dyu,dyv;

	d1[0] = pp1[0]-pp0[0];	d2[0] = pp2[0]-pp0[0];
	d1[1] = pp1[1]-pp0[1];	d2[1] = pp2[1]-pp0[1];
	dxu = d2[0]*d1[1] - d1[0]*d2[1];
	if(fabs(dxu)<1e-5)	return;		// points lies on the same line

	d1[2] = pp1[2]-pp0[2];	d2[2] = pp2[2]-pp0[2];
	c1[0] = cc1[0]-cc0[0];	c2[0] = cc2[0]-cc0[0];
	c1[1] = cc1[1]-cc0[1];	c2[1] = cc2[1]-cc0[1];
	c1[2] = cc1[2]-cc0[2];	c2[2] = cc2[2]-cc0[2];
	c1[3] = cc1[3]-cc0[3];	c2[3] = cc2[3]-cc0[3];
	n1[0] = nn1[0]-nn0[0];	n2[0] = nn2[0]-nn0[0];
	n1[1] = nn1[1]-nn0[1];	n2[1] = nn2[1]-nn0[1];
	n1[2] = nn1[2]-nn0[2];	n2[2] = nn2[2]-nn0[2];

	dyv =-d1[0]/dxu;	dxv = d1[1]/dxu;
	dyu = d2[0]/dxu;	dxu =-d2[1]/dxu;

	x1 = imin(long(pp0[0]),imin(long(pp1[0]),long(pp2[0])));	// bounding box
	y1 = imin(long(pp0[1]),imin(long(pp1[1]),long(pp2[1])));
	x2 = imax(long(pp0[0]),imax(long(pp1[0]),long(pp2[0])));
	y2 = imax(long(pp0[1]),imax(long(pp1[1]),long(pp2[1])));

	register float u,v,xx,yy;
	register long i,j,g;
	for(i=x1;i<=x2;i++)	for(j=y1;j<=y2;j++)
	{
		xx = (i-pp0[0]);	yy = (j-pp0[1]);
		u = dxu*xx+dyu*yy;	v = dxv*xx+dyv*yy;
		g = u<0 || v<0 || u+v>1;
		if(g)	continue;

		cs[0] = cc0[0]+c1[0]*u+c2[0]*v;
		cs[1] = cc0[1]+c1[1]*u+c2[1]*v;
		cs[2] = cc0[2]+c1[2]*u+c2[2]*v;
		cs[3] = cc0[3]+c1[3]*u+c2[3]*v;
		ns[0] = nn0[0]+n1[0]*u+n2[0]*v;
		ns[1] = nn0[1]+n1[1]*u+n2[1]*v;
		ns[2] = nn0[2]+n1[2]*u+n2[2]*v;
		pnt_plot(i,j,pp0[2]+d1[2]*u+d2[2]*v,col2int(cs,ns,r));
	}
}
//-----------------------------------------------------------------------------
void mglGraphZB::line_plot_s(float *x1,float *x2,float *y1,float *y2,bool /*all*/)
{
	float kx,ky,kz,t,dd=1;
	float c10,c11,c12;

	register long i;
	kx = ky = kz = t = 0;
	if(fabs(x1[0]-x2[0])>fabs(x1[1]-x2[1]))
	{
		dd = 1./fabs(x2[0]-x1[0]);		t = fabs(x2[0]-x1[0]);
	}
	else if(fabs(x1[1]-x2[1])!=0)
	{
		dd = 1./fabs(x2[1]-x1[1]);		t = fabs(x2[1]-x1[1]);
	}
	kx=(x2[0]-x1[0])*dd;	ky=(x2[1]-x1[1])*dd;	kz=(x2[2]-x1[2])*dd;
	c10=(y2[0]-y1[0])*dd;	c11=(y2[1]-y1[1])*dd;	c12=(y2[2]-y1[2])*dd;
	unsigned char r[4]={255,255,255,255};
	for(i=0;i<=t;i++)
	{
		r[0] = (unsigned char)(255.f*(y1[0]+c10*i));
		r[1] = (unsigned char)(255.f*(y1[1]+c11*i));
		r[2] = (unsigned char)(255.f*(y1[2]+c12*i));
		pnt_plot(long(x1[0]+kx*i), long(x1[1]+ky*i), x1[2]+kz*i,r);
	}
}
//-----------------------------------------------------------------------------
void mglGraphZB::line_plot(float *pp0,float *pp1,float *cc0,float *cc1,bool all)
{
	if(!DrawFace && FastNoFace)	{	line_plot_s(pp0,pp1,cc0,cc1,all);	return;	}
	unsigned char r[4];
	long y1,x1,y2,x2;
	float dxu,dxv,dyu,dyv,dd;

	float d10,d11,d12, c10,c11,c12, b;

	d10 = pp1[0]-pp0[0];	d11 = pp1[1]-pp0[1];
	dd = sqrt(d10*d10 + d11*d11);
	if(dd<1e-5)	return;		// points lies on the vertical line
	b = PenWidth*PenWidth;	//(PenWidth<1?1:PenWidth);

	d12 = pp1[2]-pp0[2];
	c10 = cc1[0]-cc0[0];	c11 = cc1[1]-cc0[1];
	c12 = cc1[2]-cc0[2];	//c13 = cc1[3]-cc0[3];

	dxv = d11/dd;	dyv =-d10/dd;
	dxu = d10/dd;	dyu = d11/dd;

	x1 = imin(long(pp0[0]),long(pp1[0]));	// bounding box
	y1 = imin(long(pp0[1]),long(pp1[1]));
	x2 = imax(long(pp0[0]),long(pp1[0]));
	y2 = imax(long(pp0[1]),long(pp1[1]));
	x1 -= int(PenWidth+3.5);	y1 -= int(PenWidth+3.5);
	x2 += int(PenWidth+3.5);	y2 += int(PenWidth+3.5);

	bool aa=UseAlpha;
	register float u,v,xx,yy;
	register long i,j;
	register bool tt;
	UseAlpha = true;
	for(i=x1;i<=x2;i++)	for(j=y1;j<=y2;j++)
	{
		xx = (i-pp0[0]);	yy = (j-pp0[1]);
		u = dxu*xx+dyu*yy;	v = dxv*xx+dyv*yy;	v = v*v;
		if(u<0)			{	v += u*u;			u = 0;	}
		else if(u>dd)	{	v += (u-dd)*(u-dd);	u = dd;	}
		if(v>b)		continue;
		tt = all || (PDef & (1<<long(fmod(pPos+u/PenWidth, 16))));
		if(!tt)		continue;
		u /= dd;
		r[0] = (unsigned char)(255.f*(cc0[0]+c10*u));	r[1] = (unsigned char)(255.f*(cc0[1]+c11*u));
		r[2] = (unsigned char)(255.f*(cc0[2]+c12*u));	r[3] = (unsigned char)(255.f*exp(-6.f*v/b));
		pnt_plot(i,j,pp0[2]+d12*u+PenWidth,r);
	}
	pPos = fmod(pPos+dd/PenWidth, 16);
	UseAlpha = aa;
}
//-----------------------------------------------------------------------------
void mglGraphZB::mark_plot(float *pp, char type)
{
	unsigned char cs[4]={(unsigned char)(255.f*CDef[0]), (unsigned char)(255.f*CDef[1]),
						(unsigned char)(255.f*CDef[2]), (unsigned char)(255.f*CDef[3])};
	float p[12]={0,0,pp[2],0,0,pp[2],0,0,pp[2],0,0,pp[2]};
	float v, ss=MarkSize*0.35*font_factor;
	register long i,j,s;
	if(type=='.' || ss==0)
	{
		bool aa=UseAlpha;	UseAlpha = true;
		s = long(5.5+PenWidth);
		for(i=-s;i<=s;i++)	for(j=-s;j<=s;j++)
		{
			v = (i*i+j*j)/(9*PenWidth*PenWidth);
			cs[3] = (unsigned char)(255.f*exp(-6.f*v));
			if(cs[3]==0)	continue;
			pnt_plot(long(pp[0])+i,long(pp[1])+j,pp[2],cs);
		}
		UseAlpha = aa;
	}
	else
	{
		float pw = PenWidth;	PenWidth = BaseLineWidth;
		int pd = PDef;	PDef = 0xffff;
		switch(type)
		{
		case '+':
			ss = ss*1.1;
			p[0] = pp[0]-ss;	p[1] = pp[1];	p[3] = pp[0]+ss;	p[4] = pp[1];
			line_plot(p,p+3,CDef,CDef);
			p[0] = pp[0];	p[1] = pp[1]-ss;	p[3] = pp[0];	p[4] = pp[1]+ss;
			line_plot(p,p+3,CDef,CDef);
			break;
		case 'x':
			p[0] = pp[0]-ss;	p[1] = pp[1]-ss;	p[3] = pp[0]+ss;	p[4] = pp[1]+ss;
			line_plot(p,p+3,CDef,CDef);
			p[0] = pp[0]+ss;	p[1] = pp[1]-ss;	p[3] = pp[0]-ss;	p[4] = pp[1]+ss;
			line_plot(p,p+3,CDef,CDef);
			break;
		case 's':
			p[0] = pp[0]-ss;	p[1] = pp[1]-ss;	p[3] = pp[0]+ss;	p[4] = pp[1]-ss;
			line_plot(p,p+3,CDef,CDef);
			p[0] = pp[0]+ss;	p[1] = pp[1]-ss;	p[3] = pp[0]+ss;	p[4] = pp[1]+ss;
			line_plot(p,p+3,CDef,CDef);
			p[0] = pp[0]+ss;	p[1] = pp[1]+ss;	p[3] = pp[0]-ss;	p[4] = pp[1]+ss;
			line_plot(p,p+3,CDef,CDef);
			p[0] = pp[0]-ss;	p[1] = pp[1]+ss;	p[3] = pp[0]-ss;	p[4] = pp[1]-ss;
			line_plot(p,p+3,CDef,CDef);
			break;
		case 'd':
			ss = ss*1.1;
			p[0] = pp[0];	p[1] = pp[1]-ss;	p[3] = pp[0]+ss;	p[4] = pp[1];
			line_plot(p,p+3,CDef,CDef);
			p[0] = pp[0]+ss;	p[1] = pp[1];	p[3] = pp[0];	p[4] = pp[1]+ss;
			line_plot(p,p+3,CDef,CDef);
			p[0] = pp[0];	p[1] = pp[1]+ss;	p[3] = pp[0]-ss;	p[4] = pp[1];
			line_plot(p,p+3,CDef,CDef);
			p[0] = pp[0]-ss;	p[1] = pp[1];	p[3] = pp[0];	p[4] = pp[1]-ss;
			line_plot(p,p+3,CDef,CDef);
			break;
		case '*':
			ss = ss*1.1;
			p[0] = pp[0]-ss;	p[1] = pp[1];	p[3] = pp[0]+ss;	p[4] = pp[1];
			line_plot(p,p+3,CDef,CDef);
			p[0] = pp[0]-0.6*ss;	p[1] = pp[1]-0.8*ss;	p[3] = pp[0]+0.6*ss;	p[4] = pp[1]+0.8*ss;
			line_plot(p,p+3,CDef,CDef);
			p[0] = pp[0]-0.6*ss;	p[1] = pp[1]+0.8*ss;	p[3] = pp[0]+0.6*ss;	p[4] = pp[1]-0.8*ss;
			line_plot(p,p+3,CDef,CDef);
			break;
		case '^':
			ss = ss*1.1;
			p[0] = pp[0]-ss;	p[1] = pp[1]-ss/2;	p[3] = pp[0];	p[4] = pp[1]+ss;
			line_plot(p,p+3,CDef,CDef);
			p[0] = pp[0]-ss;	p[1] = pp[1]-ss/2;	p[3] = pp[0]+ss;	p[4] = pp[1]-ss/2;
			line_plot(p,p+3,CDef,CDef);
			p[0] = pp[0]+ss;	p[1] = pp[1]-ss/2;	p[3] = pp[0];	p[4] = pp[1]+ss;
			line_plot(p,p+3,CDef,CDef);
			break;
		case 'v':
			ss = ss*1.1;
			p[0] = pp[0]-ss;	p[1] = pp[1]+ss/2;	p[3] = pp[0];	p[4] = pp[1]-ss;
			line_plot(p,p+3,CDef,CDef);
			p[0] = pp[0]-ss;	p[1] = pp[1]+ss/2;	p[3] = pp[0]+ss;	p[4] = pp[1]+ss/2;
			line_plot(p,p+3,CDef,CDef);
			p[0] = pp[0]+ss;	p[1] = pp[1]+ss/2;	p[3] = pp[0];	p[4] = pp[1]-ss;
			line_plot(p,p+3,CDef,CDef);
			break;
		case 'S':
			for(i=long(-ss);i<=long(ss);i++)	for(j=long(-ss);j<=long(ss);j++)
				pnt_plot(long(pp[0])+i,long(pp[1])+j,pp[2],cs);
			break;
		case 'D':
			ss = ss*1.1;
			for(i=long(-ss);i<=long(ss);i++)	for(j=long(-ss);j<=long(ss);j++)
				if(abs(i)+abs(j)<=long(ss))
					pnt_plot(long(pp[0])+i,long(pp[1])+j,pp[2],cs);
			break;
		case 'T':
			ss = ss*1.1;
			for(i=long(-ss);i<=long(ss);i++)	for(j=long(-ss/2);j<=long(ss);j++)
				if(3*abs(i)+2*j<=2*long(ss))
					pnt_plot(long(pp[0])+i,long(pp[1])+j,pp[2],cs);
			break;
		case 'V':
			ss = ss*1.1;
			for(i=long(-ss);i<=long(ss);i++)	for(j=long(-ss);j<=long(ss/2);j++)
				if(3*abs(i)-2*j<=2*long(ss))
					pnt_plot(long(pp[0])+i,long(pp[1])+j,pp[2],cs);
			break;
		case 'o':
			for(i=long(-4*ss);i<=long(4*ss);i++)
				pnt_plot(long(pp[0]+ss*cos(i*M_PI_4/ss)),
					long(pp[1]+ss*sin(i*M_PI_4/ss)),pp[2],cs);
			break;
		case 'O':
			for(i=long(-ss);i<=long(ss);i++)	for(j=long(-ss);j<=long(ss);j++)
			{
				if(i*i+j*j>=ss*ss)	continue;
				pnt_plot(long(pp[0])+i,long(pp[1])+j,pp[2],cs);
			}
			break;
		}
		PDef = pd;	PenWidth = pw;
	}
}
//-----------------------------------------------------------------------------
void mglGraphZB::SetSize(int w,int h)
{
	if(C)	{	delete []C;	delete []Z;	}
	C = new unsigned char[w*h*32];		// ����� *1 ��� TranspType>0 !!!
	Z = new float[w*h*8];
	mglGraphAB::SetSize(w,h);
}
//-----------------------------------------------------------------------------
mglGraphZB::mglGraphZB(int w,int h) : mglGraphAB(w,h)
{
	C = 0;	FastNoFace = true;
	SetSize(w,h);
}
//-----------------------------------------------------------------------------
mglGraphZB::~mglGraphZB()
{
	if(C)	{	delete []C;	delete []Z;	}
}
//-----------------------------------------------------------------------------
void mglGraphZB::WriteSlice(int n)
{
	unsigned char **p;
	char fname[32];
	sprintf(fname,"%d.png",n);

	p = (unsigned char **)malloc(Height * sizeof(unsigned char *));
	for(long i=0;i<Height;i++)	p[i] = C+4*Width*i + n*4*Width*Height;

	PNGASave(fname, Width, Height, p);
	free(p);
}
//-----------------------------------------------------------------------------
void mglGraphZB::font_line(float *p, unsigned char *c,bool thin)
{
	unsigned char r[4];
	PostScale(p,2,false);

	float d10,d11,d12, b = PenWidth*PenWidth;
	long y1,x1,y2,x2;
	float dxu,dxv,dyu,dyv,dd;
	register float u,v,xx,yy;

	d10 = p[3]-p[0];	d11 = p[4]-p[1];	d12 = p[5]-p[2];
	dd = sqrt(d10*d10 + d11*d11);
	if(dd<1e-5)	return;		// points lies on the vertical line
	dxv = d11/dd;	dyv =-d10/dd;	dxu = d10/dd;	dyu = d11/dd;	d12 /= dd;

	x1 = imin(long(p[0]),long(p[3]));	// bounding box
	y1 = imin(long(p[1]),long(p[4]));
	x2 = imax(long(p[0]),long(p[3]));
	y2 = imax(long(p[1]),long(p[4]));
	x1 -= int(PenWidth+3.5);	y1 -= int(PenWidth+3.5);
	x2 += int(PenWidth+3.5);	y2 += int(PenWidth+3.5);

	register long i,j;
	if(!thin)	b*=4;
	bool aa=UseAlpha;	UseAlpha = true;
	for(i=x1;i<=x2;i++)	for(j=y1;j<=y2;j++)
	{
		xx = (i-p[0]);	yy = (j-p[1]);
		u = dxu*xx+dyu*yy;	v = dxv*xx+dyv*yy;	v = v*v;
		if(u<0)			{	v += u*u;			u = 0;	}
		else if(u>dd)	{	v += (u-dd)*(u-dd);	u = dd;	}
		if(v>b)		continue;
		memcpy(r,c,4);
		r[3] = (unsigned char)(255.f*exp(-6.f*v/b));
		pnt_plot(i,j,p[2]+d12*u+PenWidth,r);
	}
	UseAlpha = aa;
}
//-----------------------------------------------------------------------------
void mglGraphZB::Glyph(float x,float y, float f, int nt, const short *trig, int nl, const short *line)
{
	long ik,ii,il;
	unsigned char c[4]={int(255*CDef[0]),int(255*CDef[1]),int(255*CDef[2]),255}, r[4];
	float dxu,dxv,dyu,dyv,dd, p[9];
	register long i,j,g;
	if(trig && nt>0)
	{
		long y1,x1,y2,x2;
		for(ik=0;ik<nt;ik++)
		{
			ii = 6*ik;	p[0]=f*trig[ii]+x;	p[1]=f*trig[ii+1]+y;	p[2]=0;
			ii+=2;		p[3]=f*trig[ii]+x;	p[4]=f*trig[ii+1]+y;	p[5]=0;
			ii+=2;		p[6]=f*trig[ii]+x;	p[7]=f*trig[ii+1]+y;	p[8]=0;
			PostScale(p,3,false);
	
			float d1[3],d2[3];		
			d1[0] = p[3]-p[0];	d2[0] = p[6]-p[0];
			d1[1] = p[4]-p[1];	d2[1] = p[7]-p[1];
			dxu = d2[0]*d1[1] - d1[0]*d2[1];
			if(fabs(dxu)<1e-5)	continue;		// points lies on the same line
			d1[2] = p[5]-p[2];	d2[2] = p[8]-p[2];
			dyv =-d1[0]/dxu;	dxv = d1[1]/dxu;
			dyu = d2[0]/dxu;	dxu =-d2[1]/dxu;

			x1 = imin(long(p[0]),imin(long(p[3]),long(p[6])));	// bounding box
			y1 = imin(long(p[1]),imin(long(p[4]),long(p[7])));
			x2 = imax(long(p[0]),imax(long(p[3]),long(p[6])));
			y2 = imax(long(p[1]),imax(long(p[4]),long(p[7])));
		
			register float u,v,xx,yy;
			for(i=x1;i<=x2;i++)	for(j=y1;j<=y2;j++)
			{
				xx = (i-p[0]);	yy = (j-p[1]);
				u = dxu*xx+dyu*yy;	v = dxv*xx+dyv*yy;
				g = u<0 || v<0 || u+v>1;
				if(g)	continue;
				memcpy(r,c,4);
				pnt_plot(i,j,p[2]+d1[2]*u+d2[2]*v+PenWidth,r);
			}
		}
	}
	if(line && nl>0)	// draw even for solid font for smoothing
	{
		il = 0;
		for(ik=0;ik<nl;ik++)
		{
			ii = 2*ik;
			if(line[ii]==0x3fff && line[ii+1]==0x3fff)	// line breakthrough
			{	il = ik+1;	continue;	}
			else if(ik==nl-1 || (line[ii+2]==0x3fff && line[ii+3]==0x3fff))
			{	// enclose the circle. May be in future this block should be commented
				p[0]=f*line[ii]+x;	p[1]=f*line[ii+1]+y;	p[2]=0;	ii=2*il;
				p[3]=f*line[ii]+x;	p[4]=f*line[ii+1]+y;	p[5]=0;
			}
			else
			{	// normal line
				p[0]=f*line[ii]+x;	p[1]=f*line[ii+1]+y;	p[2]=0;	ii+=2;
				p[3]=f*line[ii]+x;	p[4]=f*line[ii+1]+y;	p[5]=0;
			}
			font_line(p,c);
		}
	}
	if(nl==-1)	// overline or underline
	{
		p[0]=x;		p[1]=y;	p[2]=0;
		p[3]=fabs(f)+x;	p[4]=y;	p[5]=0;
		font_line(p,c,false);
	}
}
//-----------------------------------------------------------------------------
