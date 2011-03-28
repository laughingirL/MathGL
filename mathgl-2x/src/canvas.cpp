/***************************************************************************
 * canvas.cpp is part of Math Graphic Library
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
#include <wchar.h>
#include "mgl/canvas.h"
//-----------------------------------------------------------------------------
mglCanvas::mglCanvas(int w, int h) : mglBase()
{
	G=0;	SetSize(w,h);	Quality = MGL_DRAW_NORM;
	fnt = new mglFont;	fnt->gr = this;		ac.ch='c';
	NoAutoFactor = false;
	ax.dir = mglPoint(1,0,0);	ax.a = mglPoint(0,1,0);	ax.b = mglPoint(0,0,1);	ax.ch='x';
	ay.dir = mglPoint(0,1,0);	ay.a = mglPoint(1,0,0);	ay.b = mglPoint(0,0,1);	ay.ch='y';
	az.dir = mglPoint(0,0,1);	az.a = mglPoint(0,1,0);	az.b = mglPoint(1,0,0);	az.ch='z';
	DefaultPlotParam();
}
//-----------------------------------------------------------------------------
mglCanvas::~mglCanvas()
{
	delete fnt;
	if(P)	delete []P;
	if(G)	{	delete []G;	delete []C;	delete []Z;	delete []G4;delete []OI;	}
}
//-----------------------------------------------------------------------------
void mglCanvas::DefaultPlotParam()
{
//		BaseLineWidth = 1;
//	ScalePuts = true;
//	CloudFactor = 1;
//	AutoOrg = true;
//	CirclePnts=40;	FitPnts=100;	GridPnts=50;
	LegendMarks = 1;		FontSize = 5;
	Ambient();				Ternary(0);
	PlotId = "frame";		SetPenPal("k-1");
	SetDefScheme("BbcyrR");	SetPalette(MGL_DEF_PAL);
	SetTicks('x');	SetTicks('y');	SetTicks('z');	SetTicks('c');
	SetRanges(mglPoint(-1,-1,-1,-1), mglPoint(1,1,1,1));
	SetFunc(0,0);			CutOff(0);
	SetWarn(mglWarnNone);	Message = 0;
	BarWidth = 0.7;			//fit_res[0] = 0;
	MarkSize = 0.02;		ArrowSize = 0.03;
	AlphaDef = 0.5;			FontDef[0]=0;
	TranspType = 0;			MeshNum = 0;
	RotatedText = true;		CurrPal = 0;
	SetAxisStl();
	ClearLegend();			LegendBox = true;
	SetCutBox(mglPoint(0,0,0), mglPoint(0,0,0));
	_tetx=_tety=_tetz=0;
	TuneTicks= true;		//_sx=_sy=_sz =st_t = 1;
	Alpha(false);	Fog(0);	FactorPos = 1.15;
	ax.t[0]=ay.t[0]=az.t[0]=ac.t[0]=0;
	AutoPlotFactor = true;	PlotFactor = 1.55;
	TickLen = 0.1;	Cut = true;
	TickStl[0] = SubTStl[0] = '-';
	TickStl[1] = SubTStl[1] = 0;

	for(int i=0;i<10;i++)	{	AddLight(i, mglPoint(0,0,1));	Light(i,false);	}
	Light(0,true);			Light(false);
	InPlot(0,1,0,1,false);	Zoom(0,0,1,1);
}
//-----------------------------------------------------------------------------
//	Optimal axis position
//-----------------------------------------------------------------------------
float mglCanvas::FindOptOrg(char dir, int ind)
{
	static mglPoint px, py, pz, m1, m2;
	static float	bb[9]={1e30,0,0, 0,0,0, 0,0,0};
	mglPoint nn[8]={mglPoint(0,0,0), mglPoint(0,0,1), mglPoint(0,1,0), mglPoint(0,1,1),
					mglPoint(1,0,0), mglPoint(1,0,1), mglPoint(1,1,0), mglPoint(1,1,1)}, pp[8];
	memcpy(pp, nn, 8*sizeof(mglPoint));
	// do nothing if transformation matrix the same
	if(memcmp(B,bb,9*sizeof(float)) || m1!=Min || m2!=Max)
	{
		m1 = Min;	m2 = Max;
		memcpy(bb,B,9*sizeof(float));	PostScale(pp,8);
		// find point with minimal y
		register long i,j;
		for(i=j=0;i<8;i++)	if(pp[i].y<pp[j].y)	j=i;
		pp[0]=pp[j];
		// find max angle and left point
		// first select 3 closest points
		pp[1]=nn[j];	pp[1].x=1-pp[1].x;
		pp[2]=nn[j];	pp[2].y=1-pp[2].y;
		pp[3]=nn[j];	pp[3].z=1-pp[3].z;
		PostScale(pp+1,3);
		pp[1]-=pp[0];	pp[2]-=pp[0];	pp[3]-=pp[0];
		// find cosine of axis projection
		float cxy, cxz, cyz, dx, dy, dz;
		dx = pp[1].x*pp[1].x + pp[1].y*pp[1].y;
		dy = pp[2].x*pp[2].x + pp[2].y*pp[2].y;
		dz = pp[3].x*pp[3].x + pp[3].y*pp[3].y;
		cxy= pp[1].x*pp[2].x + pp[1].y*pp[1].y;
		cxz= pp[1].x*pp[3].x + pp[1].y*pp[3].y;
		cyz= pp[3].x*pp[2].x + pp[3].y*pp[2].y;
		if(dx==0)		cxy = cxz = 1;
		else if(dy==0)	cxy = cyz = 1;
		else if(dz==0)	cyz = cxz = 1;
		else
		{	cxy /= sqrt(dx*dy);	cxz /= sqrt(dx*dz);	cyz /= sqrt(dz*dy);	}
		// find points for axis
		px = py = pz = nn[j];
		if(cxy<cxz && cxy<cyz)	// xy is lowest
		{	// px, py the same as pp
			if(pp[1].x<pp[2].x)	pz.x = 1-pz.x;
			else	pz.y = 1-pz.y;
		}
		if(cxz<cxy && cxz<cyz)	// xz is lowest
		{	// px, pz the same as pp
			if(pp[1].x<pp[3].x)	py.x = 1-py.x;
			else	py.z = 1-py.z;
		}
		if(cyz<cxz && cyz<cxy)	// yz is lowest
		{	// py, pz the same as pp
			if(pp[3].x<pp[2].x)	px.z = 1-px.z;
			else	px.y = 1-px.y;
		}
		// return to normal variables
		// NOTE: this may not work in "inverse" curvilinear coordinates like x->1-x
		px = Min+(Max-Min)/px;
		py = Min+(Max-Min)/py;
		pz = Min+(Max-Min)/pz;
	}
	float res = px.val(ind);
	if(dir=='y')	res = py.val(ind);
	if(dir=='z')	res = pz.val(ind);
	return res;
}
//-----------------------------------------------------------------------------
float mglCanvas::GetOrgX(char dir)
{
	float res = Org.x;
	if(isnan(res))
	{
		if(strchr("xyz",dir))	res = FindOptOrg(dir,0);
		else if(dir=='t')		res = Min.x;
		else res = B[6]>0 ? Max.x:Min.x;
	}
	return res;
}
//-----------------------------------------------------------------------------
float mglCanvas::GetOrgY(char dir)
{
	float res = Org.y;
	if(isnan(res))
	{
		if(strchr("xyz",dir))	res = FindOptOrg(dir,1);
		else if(dir=='t')	res = Min.y;
		else res = B[7]>0 ? Max.y:Min.y;
	}
	return res;
}
//-----------------------------------------------------------------------------
float mglCanvas::GetOrgZ(char dir)
{
	float res = Org.z;
	if(isnan(res))
	{
		if(strchr("xyz",dir))	res = FindOptOrg(dir,2);
		else if(dir=='t')	res = Min.z;
		else res = B[8]>0 ? Max.z:Min.z;
	}
	return res;
}
//-----------------------------------------------------------------------------
//	Put primitives
//-----------------------------------------------------------------------------
void mglCanvas::mark_plot(long p, char type, float size)
{
	if(isnan(pntC[4*p]))	return;
	if(size>=0)	size *= MarkSize;
	if(Quality&4)	mark_draw(pntC+4*p,type,size?size:MarkSize);
	else
	{
		mglPrim a;
		a.w = fabs(PenWidth);	a.s = size?size:MarkSize;
		a.n1 = p;	a.m = type;	a.z = pntC[4*p+2];
		add_prim(a);
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::line_plot(long p1, long p2)
{
	if(PDef==0)	return;
	if(isnan(pntC[4*p1]) || isnan(pntC[4*p2]))	return;
	float pw = fabs(PenWidth),d;
	if(Quality&4)	line_draw(pntC+4*p1,pntC+4*p2);
	else
	{
		mglPrim a(1);	a.w = pw;
		a.z = (pntC[4*p1+2]+pntC[4*p2+2])/2;
		if(pw>1)		a.z += pw-1;
		a.style=PDef;	a.s = pPos;
		a.n1 = p1;		a.n2 = p2;
		add_prim(a);
	}
	d = hypot(pntC[4*p1]-pntC[4*p2], pntC[4*p1+1]-pntC[4*p2+1]);
	pPos = fmod(pPos+d/pw/1.5, 16);
}
//-----------------------------------------------------------------------------
void mglCanvas::trig_plot(long p1, long p2, long p3)
{
	if(isnan(pntN[8*p1]) || isnan(pntN[8*p2]) || isnan(pntN[8*p3]))	return;
	if(Quality&4)	trig_draw(pntN+8*p1,pntN+8*p2,pntN+8*p3,true);
	else
	{
		mglPrim a(2);
		a.n1 = p1;	a.n2 = p2;	a.n3 = p3;
		a.z = (pntN[8*p1+2]+pntN[8*p2+2]+pntN[8*p3+2])/3;
		add_prim(a);
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::quad_plot(long p1, long p2, long p3, long p4)
{
	if(isnan(pntN[8*p1]))	{	trig_plot(p4,p2,p3);	return;	}
	if(isnan(pntN[8*p2]))	{	trig_plot(p1,p4,p3);	return;	}
	if(isnan(pntN[8*p3]))	{	trig_plot(p1,p2,p4);	return;	}
	if(isnan(pntN[8*p4]))	{	trig_plot(p1,p2,p3);	return;	}
	if(Quality&4)	quad_draw(pntN+8*p1,pntN+8*p2,pntN+8*p3,pntN+8*p4);
	else
	{
		mglPrim a(3);
		a.n1 = p1;	a.n2 = p2;	a.n3 = p3;	a.n4 = p4;
		a.z = (pntN[8*p1+2]+pntN[8*p2+2]+pntN[8*p3+2]+pntN[8*p4+2])/4;
		add_prim(a);
	}
}
//-----------------------------------------------------------------------------
float mglCanvas::text_plot(long p,const wchar_t *text,const char *font,float size,float sh)
{
	if(isnan(pntN[8*p]))	return 0;
	if(size<0)	size = -size*FontSize;
	if(!(Quality&4))	// add text itself
	{
		mglPrim a(6);
		a.n1 = p;	a.z = pntN[8*p+2];
		a.text = new wchar_t[wcslen(text)+1];
		wcscpy(a.text,text);
		strncpy(a.font,font,16);
		a.s = size;	a.w = sh;
		add_prim(a);
	}
	// text drawing itself
	Push();
	float shift = -sh-0.2, fsize=size/8.*font_factor, h = fnt->Height(font)*fsize;
	if(strchr(font,'T'))	shift = sh+0.3;

	shift *= h;

	float *pp=pntN+8*p, ll=pp[5]*pp[5]+pp[6]*pp[6];
	B[11]= pp[2];
	if(isnan(ll))
	{
		memset(B,0,12*sizeof(float));
		B[0] = B[4] = B[8] = fsize;
		fscl = fsize;	ftet = 0;
		B[9] = pp[0] - B[1]*0.02f;
		B[10]= pp[1] - B[4]*0.02f - shift;
	}
	else
	{
		if(ll==0)	{	Pop();	return 0;	}
		float tet = 180*atan2(pp[6],pp[5])/M_PI;
		memset(B,0,12*sizeof(float));
		B[0] = B[4] = B[8] = fsize;
		fscl = fsize;	ftet = -tet;
		NoAutoFactor=true;	RotateN(-tet,0,0,1);	NoAutoFactor=false;
		B[9] = pp[0]+shift*pp[6]/sqrt(ll) - B[1]*0.02f;
		B[10]= pp[1]-shift*pp[5]/sqrt(ll) - B[4]*0.02f;
	}
	fsize = fnt->Puts(text,font)*size/8.;
	Pop();	return fsize;
}
//-----------------------------------------------------------------------------
void mglCanvas::Glyph(float x, float y, float f, int s, long j, char col)
{
	mglPrim a(4);
	a.s = fscl/PlotFactor;	a.w = ftet;	a.p = PlotFactor;
	float cc = AddTexture(col);	// TODO: use real color
	if(cc<0)	cc = CDef;
	a.n1 = AddPntC(mglPoint((B[9]-zoomx1*Width) /zoomx2, (B[10]-zoomy1*Height)/zoomy2, B[11]), cc);
	a.n2 = AddPntC(mglPoint(x,y,f/fnt->GetFact(s&3)),cc);
	a.style = s;	a.m = j;
	a.z = B[11];
	add_prim(a);
	if(Quality&4)	glyph_draw(&a);
	else	add_prim(a);
}
//-----------------------------------------------------------------------------
void mglCanvas::add_prim(mglPrim &a)		// NOTE: this is not-thread-safe!!!
{
	if(!P)
	{
		pMax = 1000;
		P = (mglPrim *)malloc(pMax*sizeof(mglPrim));
	}
	else if(pNum+1>pMax)
	{
		pMax += 1000;
		P = (mglPrim *)realloc(P, pMax*sizeof(mglPrim));
	}
	a.id = ObjId;	a.gr = this;	P[pNum]=a;
	pNum++;		Finished = false;
}
//-----------------------------------------------------------------------------
void mglPrim::Draw()
{
	int pdef=gr->PDef;
	float ss=gr->pPos, ww=gr->PenWidth;
	gr->PDef=style;	gr->pPos=s;	gr->PenWidth=w;
	switch(type)
	{
	case 0:	gr->mark_draw(gr->pntC+4*n1,m,s);	break;
	case 1:	gr->PDef=style;	gr->pPos=s;	gr->PenWidth=w;
			gr->line_draw(gr->pntC+4*n1,gr->pntC+4*n2);	break;
	case 2:	gr->trig_draw(gr->pntN+8*n1,gr->pntN+8*n2,gr->pntN+8*n3,true);	break;
	case 3:	gr->quad_draw(gr->pntN+8*n1,gr->pntN+8*n2,gr->pntN+8*n3,gr->pntN+8*n4);	break;
	case 4:	gr->glyph_draw(this);	break;
	}
	gr->PDef=pdef;	gr->pPos=ss;	gr->PenWidth=ww;
}
//-----------------------------------------------------------------------------
//	Plot positioning functions
//-----------------------------------------------------------------------------
void mglCanvas::Zoom(float x1, float y1, float x2, float y2)
{
	if(x1==x2 || y1==y2)	{	x1=y1=0;	x2=y2=1;	}
	Clf();
	if(x1<x2)	{	zoomx1=x1;	zoomx2=x2-x1;	}
	else		{	zoomx1=x2;	zoomx2=x1-x2;	}
	if(y1<y2)	{	zoomy1=y1;	zoomy2=y2-y1;	}
	else		{	zoomy1=y2;	zoomy2=y1-y2;	}
}
//-----------------------------------------------------------------------------
void mglCanvas::SubPlot(int nx,int ny,int m, float dx, float dy)
{
	float x1,x2,y1,y2;
	int mx = m%nx, my = m/nx;
	if(AutoPlotFactor)	{	dx /= 1.55;	dy /= 1.55;	}
	else	{	dx /= 2;	dy /= 2;	}
	x1 = (mx+dx)/nx;		x2 = (mx+1+dx)/nx;
	y2 = 1.f-(my+dy)/ny;	y1 = 1.f-(my+1+dy)/ny;
	InPlot(x1,x2,y1,y2,false);
}
//-----------------------------------------------------------------------------
void mglCanvas::SubPlot(int nx,int ny,int m, const char *style)
{
	float x1,x2,y1,y2;
	int mx = m%nx, my = m/nx;
	x1 = float(mx)/nx;		x2 = float(mx+1)/nx;
	y2 = 1.f-float(my)/ny;	y1 = 1.f-float(my+1)/ny;
	InPlot(x1,x2,y1,y2,style);
}
//-----------------------------------------------------------------------------
void mglCanvas::MultiPlot(int nx,int ny,int m, int dx, int dy, const char *style)
{
	float x1,x2,y1,y2;
	int mx = m%nx, my = m/nx;
	dx = (dx<1 || dx+mx>=nx) ? 1 : dx;
	dy = (dy<1 || dy+my>=ny) ? 1 : dy;
	x1 = float(mx)/nx;		x2 = float(mx+dx)/nx;
	y2 = 1-float(my)/ny;	y1 = 1-float(my+dy)/ny;
	InPlot(x1,x2,y1,y2,style);
}
//-----------------------------------------------------------------------------
void mglCanvas::InPlot(float x1,float x2,float y1,float y2, const char *st)
{
	if(!st)		{	InPlot(x1,x2,y1,y2,false);	return;	}
	if(strchr(st,'T'))	{	y1*=0.9;	y2*=0.9;	}	// general title
	bool r = !(strchr(st,'r') || strchr(st,'R') || strchr(st,'>') || strchr(st,'g'));
	bool l = !(strchr(st,'l') || strchr(st,'L') || strchr(st,'<') || strchr(st,'g'));
	bool u = !(strchr(st,'u') || strchr(st,'U') || strchr(st,'_') || strchr(st,'g'));
	bool a = !(strchr(st,'a') || strchr(st,'A') || strchr(st,'^') || strchr(st,'g') || strchr(st,'t'));
	// let use simplified scheme -- i.e. no differences between axis, colorbar and/or title
	register float xs=(x1+x2)/2, ys=(y1+y2)/2, f1 = 1.3, f2 = 1.1;
	if(r && l)	{	x2=xs+(x2-xs)*f1;	x1=xs+(x1-xs)*f1;	}
	else if(r)	{	x2=xs+(x2-xs)*f1;	x1=xs+(x1-xs)*f2;	}
	else if(l)	{	x2=xs+(x2-xs)*f2;	x1=xs+(x1-xs)*f1;	}
	if(a && u)	{	y2=ys+(y2-ys)*f1;	y1=ys+(y1-ys)*f1;	}
	else if(a)	{	y2=ys+(y2-ys)*f1;	y1=ys+(y1-ys)*f2;	}
	else if(u)	{	y2=ys+(y2-ys)*f2;	y1=ys+(y1-ys)*f1;	}
	InPlot(x1,x2,y1,y2,false);
}
//-----------------------------------------------------------------------------
void mglCanvas::Rotate(float TetX,float TetZ,float TetY)
{
	RotateN(TetX+_tetx,1.,0.,0.);
	RotateN(TetY+_tety,0.,1.,0.);
	RotateN(TetZ+_tetz,0.,0.,1.);
}
//-----------------------------------------------------------------------------
void mglCanvas::RotateN(float Tet,float x,float y,float z)
{
	float R[9],C[9],c=cos(Tet*M_PI/180),s=-sin(Tet*M_PI/180),r=1-c,n=sqrt(x*x+y*y+z*z);
	x/=n;	y/=n;	z/=n;
	C[0] = x*x*r+c;		C[1] = x*y*r-z*s;	C[2] = x*z*r+y*s;
	C[3] = x*y*r+z*s;	C[4] = y*y*r+c;		C[5] = y*z*r-x*s;
	C[6] = x*z*r-y*s;	C[7] = y*z*r+x*s;	C[8] = z*z*r+c;
	memcpy(R,B,9*sizeof(float));
	B[0] = C[0]*R[0] + C[3]*R[1] + C[6]*R[2];
	B[1] = C[1]*R[0] + C[4]*R[1] + C[7]*R[2];
	B[2] = C[2]*R[0] + C[5]*R[1] + C[8]*R[2];
	B[3] = C[0]*R[3] + C[3]*R[4] + C[6]*R[5];
	B[4] = C[1]*R[3] + C[4]*R[4] + C[7]*R[5];
	B[5] = C[2]*R[3] + C[5]*R[4] + C[8]*R[5];
	B[6] = C[0]*R[6] + C[3]*R[7] + C[6]*R[8];
	B[7] = C[1]*R[6] + C[4]*R[7] + C[7]*R[8];
	B[8] = C[2]*R[6] + C[5]*R[7] + C[8]*R[8];
	if(AutoPlotFactor && !NoAutoFactor)
	{
		float m=(fabs(B[3])+fabs(B[4])+fabs(B[5]))/inH;
		float n=(fabs(B[0])+fabs(B[1])+fabs(B[2]))/inW;
		PlotFactor = 1.55+0.6147*(m<n ? (n-1):(m-1));
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::View(float tetx,float tetz,float tety)
{	_tetx=tetx;	_tety=tety;	_tetz=tetz;	}
//-----------------------------------------------------------------------------
void mglCanvas::Perspective(float a)	// I'm too lazy for using 4*4 matrix
{	Persp = fabs(a)/Depth;	}
//-----------------------------------------------------------------------------
void mglCanvas::InPlot(float x1,float x2,float y1,float y2, bool rel)
{
	if(Width<=0 || Height<=0 || Depth<=0)	return;
	memset(B,0,12*sizeof(float));
	if(rel)
	{
		B[9] = B1[9] + (x1+x2-1)/2*B1[0];
		B[10]= B1[10]+ (y1+y2-1)/2*B1[4];
		B[0] = B1[0]*(x2-x1);	B[4] = B1[4]*(y2-y1);
		B[8] = sqrt(B[0]*B[4]);
		B[11]= B1[11]+ (1.f-B[8]/(2*Depth))*B1[8];
	}
	else
	{
		B1[9] = B[9] = (x1+x2)/2*Width;
		B1[10]= B[10]= (y1+y2)/2*Height;
		B[0] = Width*(x2-x1);	B[4] = Height*(y2-y1);
		B[8] = sqrt(B[0]*B[4]);
		B1[11]= B[11]= (1.f-B[8]/(2*Depth))*Depth;
		memcpy(B1,B,9*sizeof(float));
	}
	inW = B[0];	inH=B[4];
	font_factor = B[0] < B[4] ? B[0] : B[4];
	if(AutoPlotFactor) PlotFactor = 1.55;	// Automatically change plot factor !!!
	Persp = 0;
}
//-----------------------------------------------------------------------------
void mglCanvas::Aspect(float Ax,float Ay,float Az)
{
	float a = fabs(Ax) > fabs(Ay) ? fabs(Ax) : fabs(Ay);
	a = a > fabs(Az) ? a : fabs(Az);
	if(a==0)	{	SetWarn(mglWarnZero,"Aspect");	return;	}
	Ax/=a;	Ay/=a;	Az/=a;
	B[0] *= Ax;		B[3] *= Ax;		B[6] *= Ax;
	B[1] *= Ay;		B[4] *= Ay;		B[7] *= Ay;
	B[2] *= Az;		B[5] *= Az;		B[8] *= Az;
}
//-----------------------------------------------------------------------------
void mglCanvas::StickPlot(int num, int id, float tet, float phi)
{
	float dx,dy,w0,h0;
	mglPoint p1(-1,0,0), p2(1,0,0);
	InPlot(0,1,0,1,true);	Rotate(tet, phi);
	PostScale(p1);	PostScale(p2);
	w0=1/(1+(num-1)*fabs(p2.x-p1.x)/inW);	dx=(p2.x-p1.x)*w0/inW;
	h0=1/(1+(num-1)*fabs(p2.y-p1.y)/inH);	dy=(p2.y-p1.y)*h0/inH;

	p1 = mglPoint(-1,0,0);	p2 = mglPoint(1,0,0);
	InPlot(dx>0?0:1-w0, dx>0?w0:1, dy>0?0:1-h0, dy>0?h0:1, true);
	Rotate(tet,phi);	PostScale(p1);	PostScale(p2);
	w0=1/(1+(num-1)*fabs(p2.x-p1.x)/inW);	dx=(p2.x-p1.x)*w0/inW;
	h0=1/(1+(num-1)*fabs(p2.y-p1.y)/inH);	dy=(p2.y-p1.y)*h0/inH;

	float x1=dx>0?dx*id:1-w0+dx*id, x2=dx>0?w0+dx*id:1+dx*id;
	float y1=dy>0?dy*id:1-h0+dy*id, y2=dy>0?h0+dy*id:1+dy*id;
	InPlot(x1, x2, y1, y2, true);	Rotate(tet,phi);
}
//-----------------------------------------------------------------------------
void mglCanvas::ColumnPlot(int num, int i, float dd)
{
	float d = i/(num+PlotFactor-1);
	float w = PlotFactor/(num+PlotFactor-1);
	InPlot(0,1,d,d+w*(1-dd),true);
}
//-----------------------------------------------------------------------------
void mglCanvas::Pop()
{
	if(st_pos<0)	return;
	memcpy(B,stack+13*st_pos,12*sizeof(float));
	PlotFactor = stack[13*st_pos+12];
	st_pos--;
}
//-----------------------------------------------------------------------------
void mglCanvas::Push()
{
	st_pos = st_pos<9 ? st_pos+1:9;
	if(st_pos<0)	st_pos=0;
	memcpy(stack+13*st_pos,B,12*sizeof(float));
	stack[13*st_pos+12] = PlotFactor;
}
//-----------------------------------------------------------------------------
//	Lighting and transparency
//-----------------------------------------------------------------------------
void mglCanvas::Fog(float d, float dz)	{	FogDist=d;	FogDz = dz;	}
//-----------------------------------------------------------------------------
void mglCanvas::Ambient(float bright)	{	AmbBr = bright;	}
//-----------------------------------------------------------------------------
bool mglCanvas::Alpha(bool enable)
{	bool t=UseAlpha;	UseAlpha=enable;	return t;	}
//-----------------------------------------------------------------------------
bool mglCanvas::Light(bool enable)
{	bool t=UseLight;	UseLight=enable;	return t;	}
//-----------------------------------------------------------------------------
void mglCanvas::Light(int n, bool enable)
{
	if(n<0 || n>9)	{	SetWarn(mglWarnLId);	return;	}
	nLight[n] = enable;
}
//-----------------------------------------------------------------------------
void mglCanvas::AddLight(int n, mglPoint p, char col, float br, bool inf, float ap)
{
	if(n<0 || n>9)	{	SetWarn(mglWarnLId);	return;	}
	nLight[n] = true;	aLight[n] = ap>0?ap*ap:3;
	bLight[n] = br;		iLight[n] = inf;
	rLight[3*n] = p.x;	rLight[3*n+1] = p.y;	rLight[3*n+2] = p.z;
	mglColor c = mglColor(col);
	cLight[3*n] = c.r;	cLight[3*n+1] = c.g;	cLight[3*n+2] = c.b;
}
//-----------------------------------------------------------------------------
void mglCanvas::arrow_plot(long n1, long n2,char st)
{
	if(!strchr("AVKSDTIO",st))	return;
	float *p1=pntC+4*n1, *p2=pntC+4*n2;
	float lx=p1[0]-p2[0], ly=p1[1]-p2[1], ll, kx,ky;
	ll = hypot(lx,ly)/(PenWidth*ArrowSize*0.35*font_factor);
	if(ll==0)	return;
	lx /= ll;	ly /= ll;	kx = ly;	ky = -lx;
	ReserveC(2);	ReserveN(4);
	mglPoint q1,q2,q3,q4,nn=mglPoint(NAN,NAN,NAN);
	long k1,k2,k3,k4;

	bool ul = UseLight;		UseLight = false;
	switch(st)
	{
	case 'I':
		q1 = mglPoint(p1[0]+kx, p1[1]+ky, p1[2]);	k1=AddPntC(q1,CDef);
		q2 = mglPoint(p1[0]-kx, p1[1]-ky, p1[2]);	k2=AddPntC(q2,CDef);
		line_plot(k1,k2);	break;
	case 'D':
		q1 = mglPoint(p1[0]+kx, p1[1]+ky, p1[2]);	k1=AddPntN(q1,CDef,nn);
		q2 = mglPoint(p1[0]+lx, p1[1]+ly, p1[2]);	k2=AddPntN(q2,CDef,nn);
		q3 = mglPoint(p1[0]-kx, p1[1]-ky, p1[2]);	k3=AddPntN(q3,CDef,nn);
		q4 = mglPoint(p1[0]-lx, p1[1]-ly, p1[2]);	k4=AddPntN(q4,CDef,nn);
		quad_plot(k1,k2,k3,k4);	break;
	case 'S':
		q1 = mglPoint(p1[0]+kx-lx, p1[1]+ky-ly, p1[2]);	k1=AddPntN(q1,CDef,nn);
		q2 = mglPoint(p1[0]-kx-lx, p1[1]-ky-ly, p1[2]);	k2=AddPntN(q2,CDef,nn);
		q3 = mglPoint(p1[0]-kx+lx, p1[1]-ky+ly, p1[2]);	k3=AddPntN(q3,CDef,nn);
		q4 = mglPoint(p1[0]+kx+lx, p1[1]+ky+ly, p1[2]);	k4=AddPntN(q4,CDef,nn);
		quad_plot(k1,k2,k3,k4);	break;
	case 'T':
		q1 = mglPoint(p1[0]+kx-lx, p1[1]+ky-ly, p1[2]);	k1=AddPntN(q1,CDef,nn);
		q2 = mglPoint(p1[0]-kx-lx, p1[1]-ky-ly, p1[2]);	k2=AddPntN(q2,CDef,nn);
		q3 = mglPoint(p1[0]+lx, p1[1]+ly, p1[2]);		k3=AddPntN(q3,CDef,nn);
		trig_plot(k1,k2,k3);	break;
	case 'A':
		q1 = mglPoint(p1[0], p1[1], p1[2]);					k1=AddPntN(q1,CDef,nn);
		q2 = mglPoint(p1[0]-kx-2*lx, p1[1]-ky-2*ly, p1[2]);	k2=AddPntN(q2,CDef,nn);
		q3 = mglPoint(p1[0]-1.5*lx, p1[1]-1.5*ly, p1[2]);	k3=AddPntN(q3,CDef,nn);
		q4 = mglPoint(p1[0]+kx-2*lx, p1[1]+ky-2*ly, p1[2]);	k4=AddPntN(q4,CDef,nn);
		quad_plot(k1,k2,k3,k4);	break;
	case 'K':
		q1 = mglPoint(p1[0], p1[1], p1[2]);					k1=AddPntN(q1,CDef,nn);
		q2 = mglPoint(p1[0]-kx-2*lx, p1[1]-ky-2*ly, p1[2]);	k2=AddPntN(q2,CDef,nn);
		q3 = mglPoint(p1[0]-1.5*lx, p1[1]-1.5*ly, p1[2]);	k3=AddPntN(q3,CDef,nn);
		q4 = mglPoint(p1[0]+kx-2*lx, p1[1]+ky-2*ly, p1[2]);	k4=AddPntN(q4,CDef,nn);
		quad_plot(k1,k2,k3,k4);
		q1 = mglPoint(p1[0]+kx, p1[1]+ky, p1[2]);	k1=AddPntC(q1,CDef);
		q2 = mglPoint(p1[0]-kx, p1[1]-ky, p1[2]);	k2=AddPntC(q2,CDef);
		line_plot(k1,k2);	break;
	case 'V':
		q1 = mglPoint(p1[0], p1[1], p1[2]);					k1=AddPntN(q1,CDef,nn);
		q2 = mglPoint(p1[0]-kx+2*lx, p1[1]-ky+2*ly, p1[2]);	k2=AddPntN(q2,CDef,nn);
		q3 = mglPoint(p1[0]+1.5*lx, p1[1]+1.5*ly, p1[2]);	k3=AddPntN(q3,CDef,nn);
		q4 = mglPoint(p1[0]+kx+2*lx, p1[1]+ky+2*ly, p1[2]);	k4=AddPntN(q4,CDef,nn);
		quad_plot(k1,k2,k3,k4);	break;
	case 'O':
		{
			q1 = mglPoint(p1[0], p1[1], p1[2]);		k1=AddPntN(q1,CDef,nn);
			double t,c,s;
			for(int i=0;i<16;i++)
			{
				t = M_PI*i/8.;		s=sin(t);	c=cos(t);
				q2 = mglPoint(p1[0]+kx*s+lx*c, p1[1]+ky*s+ly*c, p1[2]);
				t = M_PI*(i+1)/8.;	s=sin(t);	c=cos(t);
				q3 = mglPoint(p1[0]+kx*s+lx*c, p1[1]+ky*s+ly*c, p1[2]);
				k2=AddPntN(q2,CDef,nn);	k3=AddPntN(q3,CDef,nn);
				trig_plot(k1,k2,k3);
			}
			break;
		}
	}
	UseLight = ul;
}
//-----------------------------------------------------------------------------
