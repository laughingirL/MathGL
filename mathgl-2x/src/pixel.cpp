/***************************************************************************
 * pixel.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <stdlib.h>
#include <math.h>
#include "mgl/canvas.h"
//-----------------------------------------------------------------------------
void mglCanvas::SetSize(int w,int h)
{
	if(w<=0 || h<=0)	{	SetWarn(mglWarnSize);	return;	}
	Width = w;	Height = h;	Depth = long(sqrt(float(w*h)));
	if(G)	{	delete []G;	delete []C;	delete []Z;	delete []G4;delete []OI;	}
	G = new unsigned char[w*h*3];
	G4 = new unsigned char[w*h*4];
	C = new unsigned char[w*h*12];
	Z = new float[w*h*3];	// only 3 planes
	OI= new int[w*h];
	InPlot(0,1,0,1);	SetDrawReg(1,1,0);	Persp = 0;
	Clf();
}
//-----------------------------------------------------------------------------
void mglCanvas::PostScale(mglPoint &p)
{
	mglPoint q=p/(2*PlotFactor);
	p.x = (B[9] + q.x*B[0] + q.y*B[1] + q.z*B[2] - zoomx1*Width)/zoomx2;
	p.y = (B[10]+ q.x*B[3] + q.y*B[4] + q.z*B[5] - zoomy1*Height)/zoomy2;
	p.z = (B[11]+ q.x*B[6] + q.y*B[7] + q.z*B[8])/sqrt(zoomx2*zoomy2);
	if(Persp)
	{
		register float d = (1-Persp*Depth/2)/(1-Persp*p.z);
		p.x = Width/2 + d*(p.x-Width/2);
		p.y = Height/2 + d*(p.y-Height/2);
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::LightScale()
{
	float *x;
	register float nn;
	register long i,j;
	for(i=0;i<10;i++)
	{
		if(!nLight[i])	continue;
		j=3*i;		x = rLight+j;
		pLight[j] = (x[0]*B[0] + x[1]*B[1] + x[2]*B[2])/zoomx2;
		pLight[j+1] = (x[0]*B[3] + x[1]*B[4] + x[2]*B[5])/zoomy2;
		pLight[j+2] = (x[0]*B[6] + x[1]*B[7] + x[2]*B[8])/sqrt(zoomx2*zoomy2);
		nn=sqrt(pLight[j]*pLight[j]+pLight[j+1]*pLight[j+1]+pLight[j+2]*pLight[j+2]);
		pLight[j] /= nn;	pLight[j+1] /= nn;	pLight[j+2] /= nn;
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::NormScale(mglPoint &p)
{
	mglPoint y=p;
	p.x = (y.x*B[0] + y.y*B[1] + y.z*B[2])/zoomx2;
	p.y = (y.x*B[3] + y.y*B[4] + y.z*B[5])/zoomy2;
	p.z = (y.x*B[6] + y.y*B[7] + y.z*B[8])/sqrt(zoomx2*zoomy2);
}
//-----------------------------------------------------------------------------
// NOTE: Perspective, transformation formulas and lists are not support just now !!! Also it use LAST InPlot parameters!!!
mglPoint mglCanvas::CalcXYZ(int xs, int ys)
{
	float s3 = 2*PlotFactor, x, y, z;
	ys = Height - ys;
	float xx = xs*zoomx2 - B[9] + zoomx1*Width;
	float yy = ys*zoomy2 - B[10]+ zoomy1*Height;
	float d1=B[0]*B[4]-B[1]*B[3], d2=B[1]*B[5]-B[2]*B[4], d3=B[0]*B[5]-B[2]*B[3];
	if(fabs(d1) > fabs(d2) && fabs(d1) > fabs(d3))	// x-y plane
	{
		z = 0;
		x = s3*(B[4]*xx-B[1]*yy)/d1;
		y = s3*(B[0]*yy-B[3]*xx)/d1;
	}
	else if(fabs(d2) > fabs(d3))	// y-z
	{
		x = 0;
		y = s3*(B[5]*xx-B[2]*yy)/d2;
		z = s3*(B[1]*yy-B[4]*xx)/d2;
	}
	else	// x-z
	{
		y = 0;
		x = s3*(B[5]*xx-B[2]*yy)/d3;
		z = s3*(B[0]*yy-B[3]*xx)/d3;
	}
	LastMousePos = mglPoint(Min.x + (Max.x-Min.x)*(x+1)/2,
					Min.y + (Max.y-Min.y)*(y+1)/2,
					Min.z + (Max.z-Min.z)*(z+1)/2);
	return LastMousePos;
}
//-----------------------------------------------------------------------------
void mglCanvas::CalcScr(mglPoint p, int *xs, int *ys)
{
	ScalePoint(p);
	if(xs)	*xs=int(p.x);
	if(ys)	*ys=int(p.y);
}
//-----------------------------------------------------------------------------
mglPoint mglCanvas::CalcScr(mglPoint p)
{	int x,y;	CalcScr(p,&x,&y);	return mglPoint(x,y);	}
//-----------------------------------------------------------------------------
void mglCanvas::SetDrawReg(int nx, int ny, int m)
{
	int mx = m%nx, my = m/nx;
	dr_nx1 = Width*mx/nx;		dr_ny1 = Height-Height*(my+1)/ny;
	dr_nx2 = Width*(mx+1)/nx;	dr_ny2 = Height-Height*my/ny;
}
//-----------------------------------------------------------------------------
void mglCanvas::PutDrawReg(int nx, int ny, int m, mglCanvas *gr)
{
	if(!gr)	return;
	int mx = m%nx, my = m/nx, x1, x2, y1, y2;
	x1 = Width*mx/nx;		y1 = Height-Height*(my+1)/ny;
	x2 = Width*(mx+1)/nx;	y2 = Height-Height*my/ny;
	register long i,j;
	for(j=y1;j<y2;j++)
	{
		i = x1+Width*(Height-1-j);
		memcpy(Z+8*i,gr->Z+8*i,3*(x2-x1)*sizeof(float));
		memcpy(C+12*i,gr->C+12*i,12*(x2-x1));
	}
}
//-----------------------------------------------------------------------------
int mgl_compare_prim(const void *p1, const void *p2)
{
	mglPrim *a1=(mglPrim *)p1, *a2=(mglPrim *)p2;
	if( a1->z < a2->z-0.5 )	return -1;
	if( a1->z > a2->z+0.5 )	return 1;
	if( a1->w < a2->w )	return -1;
	if( a1->w > a2->w )	return 1;
	if( a1->style < a2->style )	return -1;
	if( a1->style > a2->style )	return 1;
	if( a2->type != a1->type )	return a2->type - a1->type;
	if( a1->type==1 && (a1->xx()!=a2->xx()) )
		return (a2->xx()<a1->xx()) ? 1 : -1;
	if( a1->type==1 )	return (a2->yy()<a1->yy()) ? 1 : -1;
	return 0;
}
//-----------------------------------------------------------------------------
void mglCanvas::Finish()
{
	register long i;
	if(!(Quality&4) && P && pNum>0)
	{
		qsort(P,pNum,sizeof(mglPrim),mgl_compare_prim);
		for(i=0;i<pNum;i++)	P[i].Draw();
	}
	long n=Width*Height;
	unsigned char c[4],alf=TranspType!=2 ? 0:255,*cc;
	if(Quality&2)	for(i=0;i<n;i++)
	{
		cc = C+12*i;
		c[0]=BDef[0];	c[1]=BDef[1];	c[2]=BDef[2];	c[3]=alf;
		combine(c,cc+8);	combine(c,cc+4);	combine(c,cc);
		memcpy(G4+4*i,c,4);
	}
	else 	for(i=0;i<n;i++)	memcpy(G4+4*i,C+12*i,4);
	for(i=0;i<n;i++)
	{
		c[0]=BDef[0];	c[1]=BDef[1];	c[2]=BDef[2];	c[3]=255;
		combine(c,G4+4*i);	memcpy(G+3*i,c,3);
	}
	Finished = true;
}
//-----------------------------------------------------------------------------
void mglCanvas::Clf(mglColor Back)
{
	Fog(0);
	posN = posC = pNum = pPos = 0;
	PDef = 0xffff;
	if(Back==0)			Back = 'w';
	if(TranspType==2)	Back = 'k';
//	col2int(Back,1,BDef);
	BDef[0]=Back.r*255;	BDef[1]=Back.g*255;	BDef[2]=Back.b*255;	BDef[3]=0;
	register long i,n=Width*Height;
	memset(C,0,12*n);	memset(OI,0,n*sizeof(int));
	for(i=0;i<3*n;i++)	Z[i] = -1e20f;	// TODO: Parallelization ?!?
	Finished = false;
}
//-----------------------------------------------------------------------------
void mglCanvas::pnt_plot(long x,long y,float z,const unsigned char ci[4])
{
	long i0=x+Width*(Height-1-y);
	if(x<dr_nx1 || x>=dr_nx2 || y<dr_ny1 || y>=dr_ny2 || ci[3]==0)	return;
	unsigned char *cc = C+12*i0, c[4];
	memcpy(c,ci,4);
	float *zz = Z+3*i0, zf = FogDist*(z/Depth-0.5-FogDz);
	if(zf<0)
	{
		int d = int(255.f-255.f*exp(5.f*zf));
		unsigned char cb[4] = {BDef[0], BDef[1], BDef[2], d};
		if(d==255)	return;
		combine(c,cb);
	}
	if(Quality&2)
	{
		if(z>zz[1])	// shift point on slice down and paste new point
		{
			zz[2] = zz[1];	combine(cc+8,cc+4);
			if(z>zz[0])
			{
				zz[1] = zz[0];	zz[0] = z;	OI[i0]=ObjId;
				memcpy(cc+4,cc,4);	memcpy(cc,c,4);
			}
			else	{	zz[1] = z;	memcpy(cc+4,c,4);	}
		}
		else
		{
			if(z>zz[2])	// shift point on slice down and paste new point
			{	zz[2] = z;	combine(cc+8,c);	}
			else		// point below the background
			{	combine(c,cc+8);	memcpy(cc+8,c,4);	}
		}
	}
	else
	{
		if(z>zz[0])	// point upper the background
		{	zz[0]=z;	memcpy(cc,c,4);		OI[i0]=ObjId;	}
	}
}
//-----------------------------------------------------------------------------
unsigned char* mglCanvas::col2int(float uu,float vv,float *n,unsigned char *r)
{
	register long i,j;
	static unsigned char u[4];
	float c[4];	txt[long(uu)].GetC(uu,vv,c);
	register float b0=c[0],b1=c[1],b2=c[2];
	if(r==0) r = u;
	if(c[3]<=0)	{	memset(r,0,4*sizeof(unsigned char));	return r;	}
	if(UseLight && n && !isnan(n[0]))
	{
		float d0,d1,d2,nn;
		b0 *= AmbBr;		b1 *= AmbBr;		b2 *= AmbBr;
		for(i=0;i<10;i++)
		{
			if(!nLight[i])	continue;
			j = 3*i;
			nn = 2*(n[0]*pLight[j]+n[1]*pLight[j+1]+n[2]*pLight[j+2]) /
					(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]+1e-6);
			d0 = pLight[j] - n[0]*nn;
			d1 = pLight[j+1]-n[1]*nn;
			d2 = pLight[j+2]-n[2]*nn;
			nn = 1 + d2/sqrt(d0*d0+d1*d1+d2*d2+1e-6);

			nn = exp(-aLight[i]*nn)*bLight[i]*2;
			b0 += nn*cLight[j];
			b1 += nn*cLight[j+1];
			b2 += nn*cLight[j+2];
		}
		b0 = b0<1 ? b0 : 1;
		b1 = b1<1 ? b1 : 1;
		b2 = b2<1 ? b2 : 1;
	}
	r[0] = (unsigned char)(255*b0);	r[1] = (unsigned char)(255*b1);
	r[2] = (unsigned char)(255*b2);
	// c[3] should be <1 but I additionally check it here
	r[3] = UseAlpha && c[3]<1 ? (unsigned char)(256*c[3]) : 255;
	return r;
}
//-----------------------------------------------------------------------------
/// color mixing: color c1 is under color c2 !!!
void mglCanvas::combine(unsigned char *c1,unsigned char *c2)
{
	if(!c2[3])	return;
	register unsigned int a1=c1[3], a2=c2[3],b1=255-a2;
	if(a1==0 || a2==255)	{	memcpy(c1,c2,4);	return; }
	if(TranspType==0)
	{
		c1[0] = (c1[0]*b1 + c2[0]*a2)/256;
		c1[1] = (c1[1]*b1 + c2[1]*a2)/256;
		c1[2] = (c1[2]*b1 + c2[2]*a2)/256;
		c1[3] = (unsigned char)(a2+a1*b1/255);
	}
	else if(TranspType==1)
	{
		c1[0] = (unsigned char)((255-a1*(255-c1[0])/256)*(255-a2*(255-c2[0])/256)/256);
		c1[1] = (unsigned char)((255-a1*(255-c1[1])/256)*(255-a2*(255-c2[1])/256)/256);
		c1[2] = (unsigned char)((255-a1*(255-c1[2])/256)*(255-a2*(255-c2[2])/256)/256);
		c1[3] = 255;
	}
	else if(TranspType==2)
	{
		unsigned int b2,b3;
		b1 = (c1[0]*a1 + c2[0]*a2)/256;		c1[0] = b1<255 ? b1 : 255;
		b2 = (c1[1]*a1 + c2[1]*a2)/256;		c1[1] = b2<255 ? b2 : 255;
		b3 = (c1[2]*a1 + c2[2]*a2)/256;		c1[2] = b3<255 ? b3 : 255;
		c1[3] = a1+a2>255? 255 : a1+a2;
	}
}
//-----------------------------------------------------------------------------
unsigned char **mglCanvas::GetRGBLines(long &w, long &h, unsigned char *&f, bool alpha)
{
	long d = alpha ? 4:3;
	unsigned char **p;
	if(!Finished)	Finish();
	p = (unsigned char **)malloc(Height * sizeof(unsigned char *));
	for(long i=0;i<Height;i++)	p[i] = (alpha?G4:G)+d*Width*i;
	w = Width;	h = Height;		f = 0;
	return p;
}
//-----------------------------------------------------------------------------
const unsigned char *mglCanvas::GetBits()
{	if(!Finished)	Finish();	return G;	}
//-----------------------------------------------------------------------------
const unsigned char *mglCanvas::GetRGBA()
{	if(!Finished)	Finish();	return G4;	}
//-----------------------------------------------------------------------------
/* Bilinear interpolation r(u,v) = r0 + (r1-r0)*u + (r2-r0)*v + (r3+r0-r1-r2)*u*v is used (where r is one of {x,y,z,R,G,B,A}. Variables u,v are determined 	for each point (x,y) and selected one pair which 0<u<1 and 0<v<1.*/
void mglCanvas::quad_draw(const float *p1, const float *p2, const float *p3, const float *p4)
{
	if(!(Quality&3))
	{
		fast_draw(p1,p2);	fast_draw(p1,p3);
		fast_draw(p4,p2);	fast_draw(p4,p3);	return;
	}
	Finished = false;
	unsigned char r[4];
	long y1,x1,y2,x2;
	float d1[8],d2[8],d3[8],p[8],dd,dsx,dsy;
	for(int i=0;i<8;i++)	// NOTE: points must have the same texture!!!
	{
		d1[i] = p2[i]-p1[i];
		d2[i] = p3[i]-p1[i];
		d3[i] = p4[i]+p1[i]-p2[i]-p3[i];
	}

	x1 = long(fmin(fmin(p1[0],p2[0]),fmin(p3[0],p4[0])));	// bounding box
	y1 = long(fmin(fmin(p1[1],p2[1]),fmin(p3[1],p4[1])));
	x2 = long(fmax(fmax(p1[0],p2[0]),fmax(p3[0],p4[0])));
	y2 = long(fmax(fmax(p1[1],p2[1]),fmax(p3[1],p4[1])));
	x1=x1>dr_nx1?x1:dr_nx1;	x2=x2<dr_nx2?x2:dr_nx2-1;
	y1=y1>dr_ny1?y1:dr_ny1;	y2=y2<dr_ny2?y2:dr_ny2-1;
	if(x1>x2 || y1>y2)	return;

	dd = d1[0]*d2[1]-d1[1]*d2[0];
	dsx =-4*(d2[1]*d3[0] - d2[0]*d3[1])*d1[1];
	dsy = 4*(d2[1]*d3[0] - d2[0]*d3[1])*d1[0];

	if((d1[0]==0 && d1[1]==0) || (d2[0]==0 && d2[1]==0) || !(Quality&2))
	{	trig_draw(p1,p2,p4,true);	trig_draw(p1,p3,p4,true);	return;	}

	mglPoint n1 = mglPoint(p2[0]-p1[0],p2[1]-p1[1],p2[2]-p1[2])^mglPoint(p3[0]-p1[0],p3[1]-p1[1],p3[2]-p1[2]);
	mglPoint n2 = mglPoint(p2[0]-p4[0],p2[1]-p4[1],p2[2]-p4[2])^mglPoint(p3[0]-p4[0],p3[1]-p4[1],p3[2]-p4[2]);
	mglPoint nr = (n1+n2)*0.5;

	register long i,j,g;
	register float u,v,s,xx,yy,q;
	float x0 = p1[0], y0 = p1[1];
	for(i=x1;i<=x2;i++)	for(j=y1;j<=y2;j++)
	{
		xx = (i-x0);	yy = (j-y0);
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
		for(g=2;g<8;g++)	p[g] = p1[g]+d1[g]*u+d2[g]*v+d3[g]*s;
		if(isnan(p[5]))
		{	p[5] = nr.x;	p[6] = nr.y;	p[7] = nr.z;	}
		pnt_plot(i,j,p[2],col2int(p[3],p[4],p+5,r));
	}
}
//-----------------------------------------------------------------------------
/* Linear interpolation r(u,v) = r0 + (r1-r0)*u + (r2-r0)*v is used (where r is one of {x,y,z,R,G,B,A}. Variables u,v are determined for each point (x,y). Point plotted is u>0 and v>0 and u+v<1.*/
void mglCanvas::trig_draw(const float *p1, const float *p2, const float *p3, bool anorm)
{
	if(!(Quality&3))
	{
		fast_draw(p1,p2);	fast_draw(p1,p3);
		fast_draw(p2,p3);	return;
	}
	Finished = false;
	unsigned char r[4];
	long y1,x1,y2,x2;
	float d1[8],d2[8],p[8],dxu,dxv,dyu,dyv;
	for(int i=0;i<8;i++)
	{	d1[i] = p2[i]-p1[i];	d2[i] = p3[i]-p1[i];	}
	dxu = d2[0]*d1[1] - d1[0]*d2[1];
	if(fabs(dxu)<1e-5)	return;		// points lies on the same line
	dyv =-d1[0]/dxu;	dxv = d1[1]/dxu;
	dyu = d2[0]/dxu;	dxu =-d2[1]/dxu;

	x1 = long(fmin(fmin(p1[0],p2[0]),p3[0]));	// bounding box
	y1 = long(fmin(fmin(p1[1],p2[1]),p3[1]));
	x2 = long(fmax(fmax(p1[0],p2[0]),p3[0]));
	y2 = long(fmax(fmax(p1[1],p2[1]),p3[1]));
	x1=x1>dr_nx1?x1:dr_nx1;	x2=x2<dr_nx2?x2:dr_nx2-1;
	y1=y1>dr_ny1?y1:dr_ny1;	y2=y2<dr_ny2?y2:dr_ny2-1;
	if(x1>x2 || y1>y2)	return;
	// default normale
	mglPoint nr = mglPoint(p2[0]-p1[0],p2[1]-p1[1],p2[2]-p1[2])^mglPoint(p3[0]-p1[0],p3[1]-p1[1],p3[2]-p1[2]);

	register float u,v,xx,yy;
	register long i,j,g;
	float x0 = p1[0], y0 = p1[1];
	for(i=x1;i<=x2;i++)	for(j=y1;j<=y2;j++)
	{
		xx = (i-x0);	yy = (j-y0);
		u = dxu*xx+dyu*yy;	v = dxv*xx+dyv*yy;
		g = u<0 || v<0 || u+v>1;
		if(g)	continue;
		if(Quality&2)	// slow but accurate
		{
			for(g=2;g<8;g++)	p[g] = p1[g]+d1[g]*u+d2[g]*v;
			if(isnan(p[5]) && anorm)
			{	p[5] = nr.x;	p[6] = nr.y;	p[7] = nr.z;	}
			pnt_plot(i,j,p[2],col2int(p[3],p[4],p+5,r));
		}
		else	pnt_plot(i,j,p1[2],col2int(p[3],p[4],p+5,r));
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::line_draw(const float *p1, const float *p2)
{
	if(!(Quality&3))	{	fast_draw(p1,p2);	return;	}
	Finished = false;
	unsigned char r[4];
	long y1,x1,y2,x2;

	float d[4],c[4],uu, pw = PenWidth, dxu,dxv,dyu,dyv,dd;
	for(int i=0;i<4;i++)	d[i] = p2[i]-p1[i];
	bool hor = fabs(d[0])>fabs(d[1]);

	x1 = long(fmin(p1[0],p2[0]));	y1 = long(fmin(p1[1],p2[1]));	// bounding box
	x2 = long(fmax(p1[0],p2[0]));	y2 = long(fmax(p1[1],p2[1]));
	x1=x1>dr_nx1?x1:dr_nx1;	x2=x2<dr_nx2?x2:dr_nx2-1;
	y1=y1>dr_ny1?y1:dr_ny1;	y2=y2<dr_ny2?y2:dr_ny2-1;
	dd = sqrt(d[0]*d[0] + d[1]*d[1]);
	if(x1>x2 || y1>y2 || dd<1e-5)	return;

	dxv = d[1]/dd;	dyv =-d[0]/dd;
	dxu = d[0]/dd;	dyu = d[1]/dd;

	bool aa=UseAlpha;
	register float u,v,xx,yy;
	register long i,j;
	UseAlpha = true;
	if(hor)	for(i=x1;i<=x2;i++)
	{
		y1 = int(p1[1]+d[1]*(i-p1[0])/d[0] - pw - 3.5);
		y2 = int(p1[1]+d[1]*(i-p1[0])/d[0] + pw + 3.5);
		y1=y1>dr_ny1?y1:dr_ny1;	y2=y2<dr_ny2?y2:dr_ny2-1;		if(y1>y2)	continue;
		for(j=y1;j<=y2;j++)
		{
			xx = (i-p1[0]);	yy = (j-p1[1]);
			u = dxu*xx+dyu*yy;	v = dxv*xx+dyv*yy;	v = v*v;
			if(u<0)			{	v += u*u;			u = 0;	}
			else if(u>dd)	{	v += (u-dd)*(u-dd);	u = dd;	}
			if(v>pw*pw)		continue;
			if(!(PDef & (1<<long(fmod(pPos+u/pw/1.5, 16)))))	continue;
			u /= dd;	uu = p1[3]+d[3]*u;
			txt[long(uu)].GetC(uu,0,c);
			r[0] = (unsigned char)(255*c[0]);
			r[1] = (unsigned char)(255*c[1]);
			r[2] = (unsigned char)(255*c[2]);
			r[3] = (unsigned char)(255/cosh(3.f*sqrt(v/pw/pw)));
			pnt_plot(i,j,p1[2]+d[2]*u+pw,r);
		}
	}
	else	for(j=y1;j<=y2;j++)
	{
		x1 = int(p1[0]+d[0]*(j-p1[1])/d[1] - pw - 3.5);
		x2 = int(p1[0]+d[0]*(j-p1[1])/d[1] + pw + 3.5);
		x1=x1>dr_nx1?x1:dr_nx1;	x2=x2<dr_nx2?x2:dr_nx2-1;
		if(x1>x2)	continue;

		for(i=x1;i<=x2;i++)
		{
			xx = (i-p1[0]);	yy = (j-p1[1]);
			u = dxu*xx+dyu*yy;	v = dxv*xx+dyv*yy;	v = v*v;
			if(u<0)			{	v += u*u;			u = 0;	}
			else if(u>dd)	{	v += (u-dd)*(u-dd);	u = dd;	}
			if(v>pw*pw)		continue;
			if(!(PDef & (1<<long(fmod(pPos+u/pw/1.5, 16)))))		continue;
			u /= dd;	uu = p1[3]+d[3]*u;
			txt[long(uu)].GetC(uu,0,c);
			r[0] = (unsigned char)(255*c[0]);
			r[1] = (unsigned char)(255*c[1]);
			r[2] = (unsigned char)(255*c[2]);
			r[3] = (unsigned char)(255/cosh(3.f*sqrt(v/pw/pw)));
			pnt_plot(i,j,p1[2]+d[2]*u+pw,r);
		}
	}
	UseAlpha = aa;
}
//-----------------------------------------------------------------------------
void mglCanvas::fast_draw(const float *p1, const float *p2)
{
	Finished = false;
	float c[4];	txt[long(p1[3])].GetC(p1[3],0,c);
	unsigned char r[4]={255*c[0], 255*c[1], 255*c[2], 255};
	long y1,x1,y2,x2;

	float d[3]={p2[0]-p1[0], p2[1]-p1[1], p2[2]-p1[2]}, pw = PenWidth;
	bool hor = fabs(d[0])>fabs(d[1]);

	x1 = long(fmin(p1[0],p2[0]));	y1 = long(fmin(p1[1],p2[1]));	// bounding box
	x2 = long(fmax(p1[0],p2[0]));	y2 = long(fmax(p1[1],p2[1]));
	x1=x1>dr_nx1?x1:dr_nx1;	x2=x2<dr_nx2?x2:dr_nx2-1;
	y1=y1>dr_ny1?y1:dr_ny1;	y2=y2<dr_ny2?y2:dr_ny2-1;
	if(x1>x2 || y1>y2)	return;

	register long i;
	if(hor)
	{
		for(i=x1;i<=x2;i++)
			pnt_plot(i, p1[1]+d[1]*(i-p1[0])/d[0], p1[2]+d[2]*(i-p1[0])/d[0]+pw, r);
	}
	else
	{
		for(i=y1;i<=y2;i++)
			pnt_plot(p1[0]+d[0]*(i-p1[1])/d[1], i, p1[2]+d[2]*(i-p1[1])/d[1]+pw, r);
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::pnt_draw(const float *p)
{
	bool aa=UseAlpha;	UseAlpha = true;
	register long i,j,s;
	float v,c[4];
	txt[long(p[3])].GetC(p[3],1,c);
	unsigned char cs[4]={255*c[0],255*c[1],255*c[2],255*c[3]};
	s = long(5.5+fabs(PenWidth));
	for(j=-s;j<=s;j++)	for(i=-s;i<=s;i++)
	{
		v = (i*i+j*j)/(9*PenWidth*PenWidth);
		cs[3] = (unsigned char)(255*c[3]*exp(-6*v));
		if(cs[3]==0)	continue;
		pnt_plot(p[0]+i,p[1]+j,p[2],cs);
	}
	UseAlpha = aa;
}
//-----------------------------------------------------------------------------
void mglCanvas::mark_draw(const float *q, char type, float size)
{
	float c[4];	txt[long(q[3])].GetC(q[3],0,c);
	unsigned char cs[4]={255*c[0], 255*c[1], 255*c[2], size>0 ? 255 : 255*q[6]};
	float p[8], ss=fabs(size)*0.35*font_factor;
	float pp[32];
	register long i,j;
	memcpy(p,q,4*sizeof(float));	memcpy(p+4,q,4*sizeof(float));
	memcpy(pp,q,4*sizeof(float));	pp[4]=0;	pp[2]+=1;
	pp[5]=pp[6]=pp[7]=NAN;			memcpy(pp+8,pp,8*sizeof(float));
	memcpy(pp+16,pp,8*sizeof(float));	memcpy(pp+24,pp,8*sizeof(float));
	if(type=='.' || ss==0)	pnt_draw(q);
	else
	{
		float pw = PenWidth;	PenWidth = 1;
		int pd = PDef;	PDef = 0xffff;
		if(!strchr("xsSoO",type))	ss *= 1.1;
		switch(type)
		{
		case 'P':
			p[0] = q[0]-ss;	p[1] = q[1]-ss;	p[4] = q[0]+ss;	p[5] = q[1]-ss;	line_draw(p,p+4);
			p[0] = q[0]+ss;	p[1] = q[1]+ss;	line_draw(p,p+4);
			p[4] = q[0]-ss;	p[5] = q[1]+ss;	line_draw(p,p+4);
			p[0] = q[0]-ss;	p[1] = q[1]-ss;	line_draw(p,p+4);
		case '+':
			p[0] = q[0]-ss;	p[1] = q[1];	p[4] = q[0]+ss;	p[5] = q[1];	line_draw(p,p+4);
			p[0] = q[0];	p[1] = q[1]-ss;	p[4] = q[0];	p[5] = q[1]+ss;	line_draw(p,p+4);
			break;
		case 'X':
			p[0] = q[0]-ss;	p[1] = q[1]-ss;	p[4] = q[0]+ss;	p[5] = q[1]-ss;	line_draw(p,p+4);
			p[0] = q[0]+ss;	p[1] = q[1]+ss;	line_draw(p,p+4);
			p[4] = q[0]-ss;	p[5] = q[1]+ss;	line_draw(p,p+4);
			p[0] = q[0]-ss;	p[1] = q[1]-ss;	line_draw(p,p+4);
		case 'x':
			p[0] = q[0]-ss;	p[1] = q[1]-ss;	p[4] = q[0]+ss;	p[5] = q[1]+ss;	line_draw(p,p+4);
			p[0] = q[0]+ss;	p[1] = q[1]-ss;	p[4] = q[0]-ss;	p[5] = q[1]+ss;	line_draw(p,p+4);
			break;
		case 'S':
			pp[0] = q[0]-ss;	pp[1] = q[1]-ss;	pp[8] = q[0]-ss;	pp[9] = q[1]+ss;
			pp[16]= q[0]+ss;	pp[17]= q[1]+ss;	pp[24]= q[0]+ss;	pp[25]= q[1]-ss;
			quad_draw(pp,pp+8,pp+24,pp+16);
		case 's':
			p[0] = q[0]-ss;	p[1] = q[1]-ss;	p[4] = q[0]+ss;	p[5] = q[1]-ss;	line_draw(p,p+4);
			p[0] = q[0]+ss;	p[1] = q[1]+ss;	line_draw(p,p+4);
			p[4] = q[0]-ss;	p[5] = q[1]+ss;	line_draw(p,p+4);
			p[0] = q[0]-ss;	p[1] = q[1]-ss;	line_draw(p,p+4);
			break;
		case 'D':
			pp[0] = q[0];	pp[1] = q[1]-ss;	pp[8] = q[0]+ss;	pp[9] = q[1];
			pp[16]= q[0];	pp[17]= q[1]+ss;	pp[24]= q[0]-ss;	pp[25]= q[1];
			quad_draw(pp,pp+8,pp+24,pp+16);
		case 'd':
			p[0] = q[0];	p[1] = q[1]-ss;	p[4] = q[0]+ss;	p[5] = q[1];	line_draw(p,p+4);
			p[0] = q[0];	p[1] = q[1]+ss;	line_draw(p,p+4);
			p[4] = q[0]-ss;	p[5] = q[1];	line_draw(p,p+4);
			p[0] = q[0];	p[1] = q[1]-ss;	line_draw(p,p+4);
			break;
		case 'Y':
			p[0] = q[0];		p[1] = q[1]-ss;		p[4] = q[0];	p[5] = q[1];	line_draw(p,p+4);
			p[0] = q[0]-0.8*ss;	p[1] = q[1]+0.6*ss;	line_draw(p,p+4);
			p[0] = q[0]+0.8*ss;	p[1] = q[1]+0.6*ss;	line_draw(p,p+4);
			break;
		case '*':
			p[0] = q[0]-ss;		p[1] = q[1];		p[4] = q[0]+ss;		p[5] = q[1];	line_draw(p,p+4);
			p[0] = q[0]-0.6*ss;	p[1] = q[1]-0.8*ss;	p[4] = q[0]+0.6*ss;	p[5] = q[1]+0.8*ss;	line_draw(p,p+4);
			p[0] = q[0]-0.6*ss;	p[1] = q[1]+0.8*ss;	p[4] = q[0]+0.6*ss;	p[5] = q[1]-0.8*ss;	line_draw(p,p+4);
			break;
		case 'T':
			pp[0] = q[0]-ss;	pp[1] = q[1]-ss/2;	pp[8] = q[0]+ss;	pp[9] = q[1]-ss/2;
			pp[16]= q[0];		pp[17]= q[1]+ss;	trig_draw(pp,pp+8,pp+16);
		case '^':
			p[0] = q[0]-ss;	p[1] = q[1]-ss/2;	p[4] = q[0];	p[5] = q[1]+ss;		line_draw(p,p+4);
			p[4] = q[0]+ss;	p[5] = q[1]-ss/2;	line_draw(p,p+4);
			p[0] = q[0];	p[1] = q[1]+ss;		line_draw(p,p+4);
			break;
		case 'V':
			pp[0] = q[0]-ss;	pp[1] = q[1]+ss/2;	pp[8] = q[0]+ss;	pp[9] = q[1]+ss/2;
			pp[16]= q[0];		pp[17]= q[1]-ss;	trig_draw(pp,pp+8,pp+16);
		case 'v':
			p[0] = q[0]-ss;	p[1] = q[1]+ss/2;	p[4] = q[0];	p[5] = q[1]-ss;		line_draw(p,p+4);
			p[4] = q[0]+ss;	p[5] = q[1]+ss/2;	line_draw(p,p+4);
			p[0] = q[0];	p[1] = q[1]-ss;		line_draw(p,p+4);
			break;
		case 'L':
			pp[0] = q[0]+ss/2;	pp[1] = q[1]+ss;	pp[8] = q[0]+ss/2;	pp[9] = q[1]-ss;
			pp[16]= q[0]-ss;	pp[17]= q[1];		trig_draw(pp,pp+8,pp+16);
		case '<':
			p[0] = q[0]+ss/2;	p[1] = q[1]-ss;	p[4] = q[0]-ss;	p[5] = q[1];	line_draw(p,p+4);
			p[0] = q[0]+ss/2;	p[1] = q[1]+ss;	line_draw(p,p+4);
			p[4] = p[0];		p[5] = q[1]-ss;	line_draw(p,p+4);
			break;
		case 'R':
			pp[0] = q[0]-ss/2;	pp[1] = q[1]+ss;	pp[8] = q[0]-ss/2;	pp[9] = q[1]-ss;
			pp[16]= q[0]+ss;	pp[17]= q[1];		trig_draw(pp,pp+8,pp+16);
		case '>':
			p[0] = q[0]-ss/2;	p[1] = q[1]-ss;	p[4] = q[0]+ss;	p[5] = q[1];	line_draw(p,p+4);
			p[0] = q[0]-ss/2;	p[1] = q[1]+ss;	line_draw(p,p+4);
			p[4] = p[0];		p[5] = q[1]-ss;	line_draw(p,p+4);
			break;
		case 'O':
			for(j=long(-ss);j<=long(ss);j++)	for(i=long(-ss);i<=long(ss);i++)
			{
				if(i*i+j*j>=ss*ss)	continue;
				pnt_plot(long(q[0])+i,long(q[1])+j,q[2]+1,cs);
			}
		case 'o':
			for(i=0;i<40;i++)
			{
				p[0] = q[0]+ss*cos(i*M_PI/10);		p[1] = q[1]+ss*sin(i*M_PI/10);
				p[4] = q[0]+ss*cos((i+1)*M_PI/10);	p[5] = q[1]+ss*sin((i+1)*M_PI/10);
				line_draw(p,p+4);
			}
			break;
		case 'C':
			pnt_draw(q);
			for(i=0;i<40;i++)
			{
				p[0] = q[0]+ss*cos(i*M_PI/10);		p[1] = q[1]+ss*sin(i*M_PI/10);
				p[4] = q[0]+ss*cos((i+1)*M_PI/10);	p[5] = q[1]+ss*sin((i+1)*M_PI/10);
				line_draw(p,p+4);
			}
			break;
		}
		PDef = pd;	PenWidth = pw;
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::glyph_draw(float *p, float f, int s, long j)
{
	int ss=s&3;
	if(s&8)
	{
		if(!(s&4))	glyph_line(p,f,true);
		glyph_line(p,f,false);
	}
	else
	{
		if(!(s&4))	glyph_fill(p,f,fnt->GetNt(ss,j),fnt->GetTr(ss,j));
		glyph_wire(p,f,fnt->GetNl(ss,j),fnt->GetLn(ss,j));
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::glyph_fill(float *pp, float f, int nt, const short *trig)
{
	long ik,ii;
	float p[30], pw = Width>2 ? fabs(PenWidth) : 1e-5*Width;
	if(!trig || nt<=0)	return;
	p[3]=p[13]=p[23]=pp[3];
	p[4]=p[14]=p[24]=0;
	p[5]=p[15]=p[25]=NAN;
	mglPoint p1,p2,p3;
	for(ik=0;ik<nt;ik++)
	{
		ii = 6*ik;	p1 = mglPoint(f*trig[ii]+pp[0],f*trig[ii+1]+pp[1],0);	PostScale(p1);
		ii+=2;		p2 = mglPoint(f*trig[ii]+pp[0],f*trig[ii+1]+pp[1],0);	PostScale(p2);
		ii+=2;		p3 = mglPoint(f*trig[ii]+pp[0],f*trig[ii+1]+pp[1],0);	PostScale(p3);
		p[0] =p1.x;	p[1] =p1.y;	p[2] =p1.z+pw;
		p[10]=p2.x;	p[11]=p2.y;	p[12]=p2.z+pw;
		p[20]=p3.x;	p[21]=p3.y;	p[22]=p3.z+pw;
		trig_draw(p,p+10,p+20);
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::glyph_wire(float *pp, float f, int nl, const short *line)
{
	if(!line || nl<=0)	return;
	long ik,ii,il=0;
	float p[14];
	p[3]=p[10]=pp[3];
	unsigned pdef=PDef;	PDef = 0xffff;
	float opw=PenWidth;	PenWidth=0.75;
	mglPoint p1,p2;
	for(ik=0;ik<nl;ik++)
	{
		ii = 2*ik;
		if(line[ii]==0x3fff && line[ii+1]==0x3fff)	// line breakthrough
		{	il = ik+1;	continue;	}
		else if(ik==nl-1 || (line[ii+2]==0x3fff && line[ii+3]==0x3fff))
		{	// enclose the circle. May be in future this block should be commented
			p1 = mglPoint(f*line[ii]+pp[0],f*line[ii+1]+pp[1],0);	ii=2*il;
			p2 = mglPoint(f*line[ii]+pp[0],f*line[ii+1]+pp[1],0);
		}
		else
		{	// normal line
			p1 = mglPoint(f*line[ii]+pp[0],f*line[ii+1]+pp[1],0);	ii+=2;
			p2 = mglPoint(f*line[ii]+pp[0],f*line[ii+1]+pp[1],0);
		}
		PostScale(p1);	PostScale(p2);
		p[0]=p1.x;	p[1]=p1.y;	p[2]=p1.z;
		p[7]=p2.x;	p[8]=p2.y;	p[9]=p2.z;
		line_draw(p,p+7);
	}
	PDef = pdef;	PenWidth = opw;
}
//-----------------------------------------------------------------------------
void mglCanvas::glyph_line(float *pp, float f, bool solid)
{
	float p[40], pw = Width>2 ? fabs(PenWidth) : 1e-5*Width;
	unsigned pdef=PDef;	PDef = 0xffff;
	float opw=PenWidth;	PenWidth=1;
	p[3]=p[13]=p[23]=p[33]=pp[3];
	p[4]=p[14]=p[24]=p[34]=0;
	p[5]=p[15]=p[25]=p[35]=NAN;
	mglPoint p1,p2,p3,p4;

	float dy = 0.004;
	p1 = mglPoint(pp[0],pp[1]+dy,0);	PostScale(p1);
	p2 = mglPoint(pp[0],pp[1]+dy,0);	PostScale(p2);
	p3 = mglPoint(fabs(f)+pp[0],pp[1]-dy,0);	PostScale(p3);
	p4 = mglPoint(fabs(f)+pp[0],pp[1]-dy,0);	PostScale(p4);
	if(solid)
	{
		p[2]+=pw;	p[12]+=pw;	p[22]+=pw;	p[32]+=pw;
		quad_draw(p,p+10,p+20,p+30);
	}
	else
	{
		line_draw(p,p+19);	line_draw(p+30,p+10);
		line_draw(p,p+20);	line_draw(p+30,p+20);
	}
	PDef = pdef;	PenWidth=opw;
}
//-----------------------------------------------------------------------------
