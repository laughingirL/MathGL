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
	clr(MGL_DISABLE_SCALE);
	Z=0;	C=G=G4=0;	OI=0;	PlotId=0;	gif=0;
	CurFrameId=0;
	Width=Height=Depth=ObjId=0;
	fscl=ftet=0;
	dr_nx1=dr_nx2=dr_ny1=dr_ny2=0;	// Allowed drawing region

	fnt = new mglFont;	fnt->gr = this;		ac.ch='c';
	ax.dir = mglPoint(1,0,0);	ax.a = mglPoint(0,1,0);	ax.b = mglPoint(0,0,1);	ax.ch='x';
	ay.dir = mglPoint(0,1,0);	ay.a = mglPoint(1,0,0);	ay.b = mglPoint(0,0,1);	ay.ch='y';
	az.dir = mglPoint(0,0,1);	az.a = mglPoint(0,1,0);	az.b = mglPoint(1,0,0);	az.ch='z';
	memset(&mutexSub,0,sizeof(pthread_mutex_t));	memset(&mutexLeg,0,sizeof(pthread_mutex_t));
	memset(&mutexPrm,0,sizeof(pthread_mutex_t));	memset(&mutexPtx,0,sizeof(pthread_mutex_t));
	memset(&mutexStack,0,sizeof(pthread_mutex_t));
	SetSize(w,h);	SetQuality(MGL_DRAW_NORM);	DefaultPlotParam();
}
//-----------------------------------------------------------------------------
mglCanvas::~mglCanvas()
{
	delete fnt;
	if(G)	{	delete []G;	delete []C;	delete []Z;	delete []G4;delete []OI;	}
}
//-----------------------------------------------------------------------------
float mglCanvas::GetRatio()	{	return inW/inH;	}
//-----------------------------------------------------------------------------
void mglCanvas::DefaultPlotParam()
{
/* NOTE: following variables and mutex will not be changed
std::vector<mglTexture> Txt;	///< Pointer to textures
char *Message;		///< Buffer for receiving messages
long InUse;			///< Smart pointer (number of users)
mglPoint LastMousePos;	///< Last mouse position
mglFont *fnt;		///< Class for printing vector text
int Quality;		///< Quality of plot (0x0-pure, 0x1-fast; 0x2-fine; 0x4 - low memory)
int Width;			///< Width of the image
int Height;			///< Height of the image
int Depth;			///< Depth of the image
int CurFrameId;		///< Number of automaticle created frames
GifFileType *gif;
*/
	SetTickRotate(true);	SetTickSkip(true);
	SetWarn(mglWarnNone);	ObjId = 0;
	SetFunc(0,0);			CutOff(0);
	SetRanges(mglPoint(-1,-1,-1,-1), mglPoint(1,1,1,1));
	SetBarWidth(0.7);	SetMarkSize(1);	SetArrowSize(1);
	SetAlphaDef(0.5);		FontDef[0]=0;
	SetTranspType(0);		SetMeshNum(0);
	SetRotatedText(true);	CurrPal = 0;
	SetLegendMarks();		SetFontSize(4);
	SetTuneTicks(true);
	Clf();	SetAmbient();	Ternary(0);
	PlotId = "frame";		SetPenPal("k-1");
	SetDefScheme("BbcyrR");	clr(MGL_DISABLE_SCALE);
	SetPalette(MGL_DEF_PAL);
	SetTicks('x');	SetTicks('y');	SetTicks('z');	SetTicks('c');
	_tetx=_tety=_tetz=0;	stack.clear();
	Alpha(false);		FactorPos = 1.07;
	SetTickLen(0);	SetCut(true);
	AdjustTicks("xyzc",true);

	for(int i=0;i<10;i++)	{	AddLight(i, mglPoint(0,0,1));	Light(i,false);	}
	Light(0,true);			Light(false);
	SetPlotFactor(0);		InPlot(0,1,0,1,false);
}
//-----------------------------------------------------------------------------
//	Optimal axis position
//-----------------------------------------------------------------------------
float mglCanvas::FindOptOrg(char dir, int ind)
{
	static mglPoint px, py, pz, m1, m2;
	static mglMatrix bb;	bb.b[0]=1e30;
	mglPoint nn[8]={mglPoint(0,0,0), mglPoint(0,0,1), mglPoint(0,1,0), mglPoint(0,1,1),
					mglPoint(1,0,0), mglPoint(1,0,1), mglPoint(1,1,0), mglPoint(1,1,1)}, pp[8];
	memcpy(pp, nn, 8*sizeof(mglPoint));
	// do nothing if transformation matrix the same
	if(memcmp(B.b,bb.b,9*sizeof(float)) || m1!=Min || m2!=Max)
	{
		m1 = Min;	m2 = Max;	memcpy(&bb,&B,sizeof(mglMatrix));
		PostScale(pp,8);
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
		else res = B.b[6]>0 ? Max.x:Min.x;
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
		else res = B.b[7]>0 ? Max.y:Min.y;
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
		else res = B.b[8]>0 ? Max.z:Min.z;
	}
	return res;
}
//-----------------------------------------------------------------------------
//	Put primitives
//-----------------------------------------------------------------------------
#define MGL_MARK_PLOT	if(Quality&4)	mark_draw(p,type,size?size:MarkSize);else	\
						{	mglPrim a;	a.w = fabs(PenWidth);	a.s = size?size:MarkSize;	\
							a.n1 = p;	a.n4 = type;	a.z = Pnt[p].z;	add_prim(a);	}
void mglCanvas::mark_plot(long p, char type, float size)
{
	if(p<0 || isnan(Pnt[p].x))	return;
	long pp=p;
	if(size>=0)	size *= MarkSize;
	if(TernAxis&4) for(int i=0;i<4;i++)
	{	p = ProjScale(i, pp);	MGL_MARK_PLOT	}
	else	{	MGL_MARK_PLOT	}
}
//-----------------------------------------------------------------------------
#define MGL_LINE_PLOT	if(Quality&4)	line_draw(p1,p2);else	\
						{	mglPrim a(1);	a.z = (Pnt[p1].z+Pnt[p2].z)/2;	\
							if(pw>1)	a.z += pw-1;	a.n3=PDef;	a.s = pPos;	\
							a.n1 = p1;	a.n2 = p2;	a.w = pw;	add_prim(a);	}
void mglCanvas::line_plot(long p1, long p2)
{
	if(PDef==0)	return;
	if(p1<0 || p2<0 || isnan(Pnt[p1].x) || isnan(Pnt[p2].x))	return;
	long pp1=p1,pp2=p2;
	float pw = fabs(PenWidth),d;
	if(TernAxis&4) for(int i=0;i<4;i++)
	{	p1 = ProjScale(i, pp1);	p2 = ProjScale(i, pp2);
		MGL_LINE_PLOT	}
	else	{	MGL_LINE_PLOT	}
	d = hypot(Pnt[p1].x-Pnt[p2].x, Pnt[p1].y-Pnt[p2].y);
	pPos = fmod(pPos+d/pw/1.5, 16);
}
//-----------------------------------------------------------------------------
#define MGL_TRIG_PLOT	if(Quality&4)	trig_draw(p1,p2,p3,true);else	\
						{	mglPrim a(2);	a.n1 = p1;	a.n2 = p2;	a.n3 = p3;	\
							a.z = (Pnt[p1].z+Pnt[p2].z+Pnt[p3].z)/3;	add_prim(a);}
void mglCanvas::trig_plot(long p1, long p2, long p3)
{
	if(p1<0 || p2<0 || p3<0 || isnan(Pnt[p1].x) || isnan(Pnt[p2].x) || isnan(Pnt[p3].x))	return;
	long pp1=p1,pp2=p2,pp3=p3;
	if(TernAxis&4) for(int i=0;i<4;i++)
	{	p1 = ProjScale(i, pp1);	p2 = ProjScale(i, pp2);
		p3 = ProjScale(i, pp3);	MGL_TRIG_PLOT	}
	else	{	MGL_TRIG_PLOT	}
}
//-----------------------------------------------------------------------------
#define MGL_QUAD_PLOT	if(Quality&4)	quad_draw(p1,p2,p3,p4);else	\
						{	mglPrim a(3);	a.n1 = p1;	a.n2 = p2;	a.n3 = p3;	a.n4 = p4;	\
							a.z = (Pnt[p1].z+Pnt[p2].z+Pnt[p3].z+Pnt[p4].z)/4;	\
							add_prim(a);	}
void mglCanvas::quad_plot(long p1, long p2, long p3, long p4)
{
	if(p1<0 || isnan(Pnt[p1].x))	{	trig_plot(p4,p2,p3);	return;	}
	if(p2<0 || isnan(Pnt[p2].x))	{	trig_plot(p1,p4,p3);	return;	}
	if(p3<0 || isnan(Pnt[p3].x))	{	trig_plot(p1,p2,p4);	return;	}
	if(p4<0 || isnan(Pnt[p4].x))	{	trig_plot(p1,p2,p3);	return;	}
	long pp1=p1,pp2=p2,pp3=p3,pp4=p4;
	if(TernAxis&4) for(int i=0;i<4;i++)
	{	p1 = ProjScale(i, pp1);	p2 = ProjScale(i, pp2);
		p3 = ProjScale(i, pp3);	p4 = ProjScale(i, pp4);
		MGL_QUAD_PLOT	}
	else	{	MGL_QUAD_PLOT	}
}
//-----------------------------------------------------------------------------
float mglCanvas::text_plot(long p,const wchar_t *text,const char *font,float size,float sh,float col)
{
	if(p<0 || isnan(Pnt[p].x))	return 0;
	if(size<0)	size *= -FontSize;
	if(!font)	font="";

	if(TernAxis&4)	// text at projections
	{
		float res;
		TernAxis = TernAxis&(~4);
		for(int i=0;i<4;i++)
			res = text_plot(ProjScale(i,p),text,font,size/2,sh,col);
		TernAxis = TernAxis|4;
		return res;
	}

	if(!(Quality&4))	// add text itself
	{
		mglPrim a(6);
		a.n1 = p;	a.z = Pnt[p].z;
		mglText txt(text,font);
		MGL_PUSH(Ptx,txt,mutexPtx);
		a.n3 = Ptx.size()-1;
		a.s = size;	a.w = sh;	a.p=col;
		add_prim(a);
	}
	float shift = -sh-0.2, fsize=size/8.*font_factor, h = fnt->Height(font)*fsize, w;
	if(strchr(font,'@'))	// draw box around text
	{
		long k1,k2,k3,k4;
		w = fnt->Width(text,font)*fsize;
		mglPnt pp=Pnt[p], pt;
		int align;	mglGetStyle(font,0,&align);	align = align&3;
		float d=-w*align/2.;
		pt = pp;	pt.x+= d;	MGL_PUSH(Pnt,pt,mutexPnt);	k1=Pnt.size()-1;
		pt = pp;	pt.x+= w+d;	MGL_PUSH(Pnt,pt,mutexPnt);	k2=Pnt.size()-1;
		pt = pp;	pt.x+= d;	pt.y+= h/2;	MGL_PUSH(Pnt,pt,mutexPnt);	k3=Pnt.size()-1;
		pt = pp;	pt.x+= w+d;	pt.y+= h/2;	MGL_PUSH(Pnt,pt,mutexPnt);	k4=Pnt.size()-1;
		line_plot(k1,k2);	line_plot(k1,k3);	line_plot(k4,k2);	line_plot(k4,k3);
	}
	// text drawing itself
	Push();
	if(strchr(font,'T'))	shift = sh+0.2;
	shift += 0.11;	// Correction for glyph rotation around proper point

	mglPnt q=Pnt[p];
	float ll = q.u*q.u+q.v*q.v;
	if(q.u<0)	{	q.u=-q.u;	q.v=-q.v;	q.w=-q.w;	}
	shift *= h;		B.z= q.z;
	if(ll==0)	{	Pop();	return 0;	}

	if(isnan(ll) || !get(MGL_ENABLE_RTEXT))
	{
		fscl = fsize;	ftet = 0;
		B.x = q.x;	B.y= q.y - shift;
	}
	else
	{
		if(ll==0)	{	Pop();	return 0;	}
		B.x = q.x+shift*q.v/sqrt(ll);
		B.y= q.y-shift*q.u/sqrt(ll);
		fscl = fsize;
		ftet = -180*atan2(q.v,q.u)/M_PI;
	}
	fsize *= fnt->Puts(text,font,col)/2;
	Pop();	return fsize;
}
//-----------------------------------------------------------------------------
void mglCanvas::Glyph(float x, float y, float f, int s, long j, float col)
{
	mglPrim a(4);
	a.s = fscl/B.pf;	a.w = ftet;	a.p = B.pf;
	float cc = col<0 ? AddTexture(char(0.5-col)):col;
	if(cc<0)	cc = CDef;
	a.n1 = AddPnt(mglPoint(B.x,B.y,B.z), cc, mglPoint(x,y,f/fnt->GetFact(s&3)), -1, 0);
	a.n3 = s;	a.n4 = j;	a.z = B.z;
	if(a.n1<0)	return;
	if(Quality&4)	glyph_draw(&a);
	else	add_prim(a);
}
//-----------------------------------------------------------------------------
void mglCanvas::Draw(const mglPrim &p)
{
	int pdef=PDef;
	float ss=pPos, ww=PenWidth;
	PDef=p.n3;	pPos=p.s;	PenWidth=p.w;
	switch(p.type)
	{
	case 0:	mark_draw(p.n1,p.n4,p.s);	break;
	case 1:	PDef=p.n3;	pPos=p.s;	PenWidth=p.w;
		line_draw(p.n1,p.n2);	break;
	case 2:	trig_draw(p.n1,p.n2,p.n3,true);	break;
	case 3:	quad_draw(p.n1,p.n2,p.n3,p.n4);		break;
	case 4:	glyph_draw(&p);		break;
	}
	PDef=pdef;	pPos=ss;	PenWidth=ww;
}
//-----------------------------------------------------------------------------
//	Plot positioning functions
//-----------------------------------------------------------------------------
void mglCanvas::SubPlot(int nx,int ny,int m, float dx, float dy)
{
	float x1,x2,y1,y2;
	int mx = m%nx, my = m/nx;
	if(get(MGL_AUTO_FACTOR))	{	dx /= 1.55;	dy /= 1.55;	}
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
	if(Width<=0 || Height<=0 || Depth<=0)	return;
	if(!st)		{	InPlot(x1,x2,y1,y2,false);	return;	}
	inW = Width*(x2-x1);	inH = Height*(y2-y1);	Persp = 0;
	mglPrim p;	p.id = ObjId;
	p.n1=x1*Width;	p.n2=x2*Width;	p.n3=y1*Height;	p.n4=y2*Height;
	MGL_PUSH(Sub,p,mutexSub);

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

	B.clear();
	if(get(MGL_AUTO_FACTOR)) B.pf = 1.55;	// Automatically change plot factor !!!
	B.x = (x1+x2)/2*Width;
	B.y = (y1+y2)/2*Height;
	B.b[0] = Width*(x2-x1);	B.b[4] = Height*(y2-y1);
	B.b[8] = sqrt(B.b[0]*B.b[4]);
	B.z = (1.f-B.b[8]/(2*Depth))*Depth;
	B1=B;	font_factor = B.b[0] < B.b[4] ? B.b[0] : B.b[4];
}
//-----------------------------------------------------------------------------
void mglCanvas::InPlot(float x1,float x2,float y1,float y2, bool rel)
{
	if(Width<=0 || Height<=0 || Depth<=0)	return;
	B.clear();
	if(get(MGL_AUTO_FACTOR)) B.pf = 1.55;	// Automatically change plot factor !!!
	if(rel)
	{
		B.x = B1.x + (x1+x2-1)/2*B1.b[0];
		B.y = B1.y + (y1+y2-1)/2*B1.b[4];
		B.b[0] = B1.b[0]*(x2-x1);	B.b[4] = B1.b[4]*(y2-y1);
		B.b[8] = sqrt(B.b[0]*B.b[4]);
		B.z = B1.z + (1.f-B.b[8]/(2*Depth))*B1.b[8];
	}
	else
	{
		B.x = (x1+x2)/2*Width;
		B.y = (y1+y2)/2*Height;
		B.b[0] = Width*(x2-x1);	B.b[4] = Height*(y2-y1);
		B.b[8] = sqrt(B.b[0]*B.b[4]);
		B.z = (1.f-B.b[8]/(2*Depth))*Depth;
		B1=B;
	}
	inW = B.b[0];	inH=B.b[4];		Persp = 0;
	font_factor = B.b[0] < B.b[4] ? B.b[0] : B.b[4];
	mglPrim p;	p.id = ObjId;
	p.n1=x1*Width;	p.n2=x2*Width;	p.n3=y1*Height;	p.n4=y2*Height;
	MGL_PUSH(Sub,p,mutexSub);
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
	float R[9],T[9],c=cos(Tet*M_PI/180),s=-sin(Tet*M_PI/180),r=1-c,n=sqrt(x*x+y*y+z*z);
	x/=n;	y/=n;	z/=n;
	T[0] = x*x*r+c;		T[1] = x*y*r-z*s;	T[2] = x*z*r+y*s;
	T[3] = x*y*r+z*s;	T[4] = y*y*r+c;		T[5] = y*z*r-x*s;
	T[6] = x*z*r-y*s;	T[7] = y*z*r+x*s;	T[8] = z*z*r+c;
	memcpy(R,B.b,9*sizeof(float));
	B.b[0] = T[0]*R[0] + T[3]*R[1] + T[6]*R[2];
	B.b[1] = T[1]*R[0] + T[4]*R[1] + T[7]*R[2];
	B.b[2] = T[2]*R[0] + T[5]*R[1] + T[8]*R[2];
	B.b[3] = T[0]*R[3] + T[3]*R[4] + T[6]*R[5];
	B.b[4] = T[1]*R[3] + T[4]*R[4] + T[7]*R[5];
	B.b[5] = T[2]*R[3] + T[5]*R[4] + T[8]*R[5];
	B.b[6] = T[0]*R[6] + T[3]*R[7] + T[6]*R[8];
	B.b[7] = T[1]*R[6] + T[4]*R[7] + T[7]*R[8];
	B.b[8] = T[2]*R[6] + T[5]*R[7] + T[8]*R[8];
	if(get(MGL_AUTO_FACTOR))
	{
		float w=(fabs(B.b[3])+fabs(B.b[4])+fabs(B.b[5]))/B1.b[4];
		float h=(fabs(B.b[0])+fabs(B.b[1])+fabs(B.b[2]))/B1.b[0];
		B.pf = 1.55+0.6147*(w<h ? (h-1):(w-1));
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::View(float tetx,float tetz,float tety)
{	_tetx=tetx;	_tety=tety;	_tetz=tetz;	}
//-----------------------------------------------------------------------------
void mglCanvas::Perspective(float a)	// I'm too lazy for using 4*4 matrix
{	Persp = fabs(a)/Depth;	}
//-----------------------------------------------------------------------------
int mglCanvas::GetSplId(long x,long y)
{
	register long i,id=-1;
	for(i=Sub.size()-1;i>=0;i--)
	{
		const mglPrim &p = Sub[i];
		if(p.n1<=x && p.n2>=x && p.n3<=y && p.n4>=y)
		{	id=p.id;	break;	}
	}
	return id;
}
//-----------------------------------------------------------------------------
void mglCanvas::Aspect(float Ax,float Ay,float Az)
{
	float a = fabs(Ax) > fabs(Ay) ? fabs(Ax) : fabs(Ay);
	a = a > fabs(Az) ? a : fabs(Az);
	if(a==0)	{	SetWarn(mglWarnZero,"Aspect");	return;	}
	Ax/=a;	Ay/=a;	Az/=a;
	B.b[0] *= Ax;		B.b[3] *= Ax;		B.b[6] *= Ax;
	B.b[1] *= Ay;		B.b[4] *= Ay;		B.b[7] *= Ay;
	B.b[2] *= Az;		B.b[5] *= Az;		B.b[8] *= Az;
}
//-----------------------------------------------------------------------------
void mglCanvas::StickPlot(int num, int id, float tet, float phi)
{
	float dx,dy,w0,h0;
	mglPoint p1(-1,0,0), p2(1,0,0);
	InPlot(0,1,0,1,true);	Rotate(tet, phi);
	PostScale(p1);	PostScale(p2);
	w0=1/(1+(num-1)*fabs(p2.x-p1.x)/B1.b[0]);	dx=(p2.x-p1.x)*w0/B1.b[0];
	h0=1/(1+(num-1)*fabs(p2.y-p1.y)/B1.b[4]);	dy=(p2.y-p1.y)*h0/B1.b[4];

	p1 = mglPoint(-1,0,0);	p2 = mglPoint(1,0,0);
	InPlot(dx>0?0:1-w0, dx>0?w0:1, dy>0?0:1-h0, dy>0?h0:1, true);
	Rotate(tet,phi);	PostScale(p1);	PostScale(p2);
	w0=1/(1+(num-1)*fabs(p2.x-p1.x)/B1.b[0]);	dx=(p2.x-p1.x)*w0/B1.b[0];
	h0=1/(1+(num-1)*fabs(p2.y-p1.y)/B1.b[4]);	dy=(p2.y-p1.y)*h0/B1.b[4];

	float x1=dx>0?dx*id:1-w0+dx*id, x2=dx>0?w0+dx*id:1+dx*id;
	float y1=dy>0?dy*id:1-h0+dy*id, y2=dy>0?h0+dy*id:1+dy*id;
	InPlot(x1, x2, y1, y2, true);	Rotate(tet,phi);
}
//-----------------------------------------------------------------------------
void mglCanvas::ColumnPlot(int num, int i, float dd)
{
	float d = i/(num+B.pf-1);
	float w = B.pf/(num+B.pf-1);
	InPlot(0,1,d,d+w*(1-dd),true);
}
//-----------------------------------------------------------------------------
//	Lighting and transparency
//-----------------------------------------------------------------------------
void mglCanvas::Fog(float d, float dz)	{	FogDist=d;	FogDz = dz;	}
//-----------------------------------------------------------------------------
bool mglCanvas::Alpha(bool enable)
{	bool t=get(MGL_ENABLE_ALPHA);	set(enable,MGL_ENABLE_ALPHA);	return t;	}
//-----------------------------------------------------------------------------
bool mglCanvas::Light(bool enable)
{	bool t=get(MGL_ENABLE_LIGHT);	set(enable,MGL_ENABLE_LIGHT);	return t;	}
//-----------------------------------------------------------------------------
void mglCanvas::Light(int n, bool enable)
{
	if(n<0 || n>9)	{	SetWarn(mglWarnLId);	return;	}
	light[n].n = enable;
}
//-----------------------------------------------------------------------------
void mglCanvas::AddLight(int n, mglPoint p, char col, float br, bool inf, float ap)
{
	if(n<0 || n>9)	{	SetWarn(mglWarnLId);	return;	}
	light[n].n = true;	light[n].a = ap>0?ap*ap:3;
	light[n].b = br;	light[n].i = inf;
	light[n].r = p;		light[n].c = mglColor(col);
}
//-----------------------------------------------------------------------------
void mglCanvas::arrow_plot(long n1, long n2,char st)
{
	if(n1<0 || n2<0 || !strchr("AVKSDTIO",st))	return;
	const mglPnt &p1=Pnt[n1], &p2=Pnt[n2];
	mglPnt q1=p1,q2=p1,q3=p1,q4=p1;
	q1.u=q2.u=q3.u=q4.u=NAN;

	float lx=p1.x-p2.x, ly=p1.y-p2.y, ll, kx,ky;
	ll = hypot(lx,ly)/(PenWidth*ArrowSize*0.35*font_factor);
	if(ll==0)	return;
	lx /= ll;	ly /= ll;	kx = ly;	ky = -lx;
	Reserve(6);
	long k1,k2,k3,k4;

	switch(st)
	{
	case 'I':
		q1.x = p1.x+kx;		q1.y = p1.y+ky;		k1=Pnt.size();	MGL_PUSH(Pnt,q1,mutexPnt);
		q2.x = p1.x-kx;		q2.y = p1.y-ky;		k2=Pnt.size();	MGL_PUSH(Pnt,q2,mutexPnt);
		line_plot(k1,k2);	break;
	case 'D':
		q1.x = p1.x+kx;		q1.y = p1.y+ky;		k1=Pnt.size();	MGL_PUSH(Pnt,q1,mutexPnt);
		q2.x = p1.x+lx;		q2.y = p1.y+ly;		k2=Pnt.size();	MGL_PUSH(Pnt,q2,mutexPnt);
		q3.x = p1.x-kx;		q3.y = p1.y-ky;		k3=Pnt.size();	MGL_PUSH(Pnt,q3,mutexPnt);
		q4.x = p1.x-lx;		q4.y = p1.y-ly;		k4=Pnt.size();	MGL_PUSH(Pnt,q4,mutexPnt);
		quad_plot(k1,k2,k4,k3);	break;
	case 'S':
		q1.x = p1.x+kx-lx;	q1.y = p1.y+ky-ly;	k1=Pnt.size();	MGL_PUSH(Pnt,q1,mutexPnt);
		q2.x = p1.x-kx-lx;	q2.y = p1.y-ky-ly;	k2=Pnt.size();	MGL_PUSH(Pnt,q2,mutexPnt);
		q3.x = p1.x-kx+lx;	q3.y = p1.y-ky+ly;	k3=Pnt.size();	MGL_PUSH(Pnt,q3,mutexPnt);
		q4.x = p1.x+kx+lx;	q4.y = p1.y+ky+ly;	k4=Pnt.size();	MGL_PUSH(Pnt,q4,mutexPnt);
		quad_plot(k1,k2,k4,k3);	break;
	case 'T':
		q1.x = p1.x+kx-lx;	q1.y = p1.y+ky-ly;	k1=Pnt.size();	MGL_PUSH(Pnt,q1,mutexPnt);
		q2.x = p1.x-kx-lx;	q2.y = p1.y-ky-ly;	k2=Pnt.size();	MGL_PUSH(Pnt,q2,mutexPnt);
		q3.x = p1.x+lx;		q3.y = p1.y+ly;		k3=Pnt.size();	MGL_PUSH(Pnt,q3,mutexPnt);
		trig_plot(k1,k2,k3);	break;
	case 'A':
		q1.x = p1.x;			q1.y = p1.y;			k1=Pnt.size();	MGL_PUSH(Pnt,q1,mutexPnt);
		q2.x = p1.x-kx-2*lx;	q2.y = p1.y-ky-2*ly;	k2=Pnt.size();	MGL_PUSH(Pnt,q2,mutexPnt);
		q3.x = p1.x-1.5*lx;		q3.y = p1.y-1.5*ly;		k3=Pnt.size();	MGL_PUSH(Pnt,q3,mutexPnt);
		q4.x = p1.x+kx-2*lx;	q4.y = p1.y+ky-2*ly;	k4=Pnt.size();	MGL_PUSH(Pnt,q4,mutexPnt);
		quad_plot(k1,k2,k4,k3);	break;
	case 'K':
		q1.x = p1.x;			q1.y = p1.y;			k1=Pnt.size();	MGL_PUSH(Pnt,q1,mutexPnt);
		q2.x = p1.x-kx-2*lx;	q2.y = p1.y-ky-2*ly;	k2=Pnt.size();	MGL_PUSH(Pnt,q2,mutexPnt);
		q3.x = p1.x-1.5*lx;		q3.y = p1.y-1.5*ly;		k3=Pnt.size();	MGL_PUSH(Pnt,q3,mutexPnt);
		q4.x = p1.x+kx-2*lx;	q4.y = p1.y+ky-2*ly;	k4=Pnt.size();	MGL_PUSH(Pnt,q4,mutexPnt);
		quad_plot(k1,k2,k4,k3);
		q1.x = p1.x+kx;			q1.y = p1.y+ky;			k1=Pnt.size();	MGL_PUSH(Pnt,q1,mutexPnt);
		q2.x = p1.x-kx;			q2.y = p1.y-ky;			k2=Pnt.size();	MGL_PUSH(Pnt,q2,mutexPnt);
		line_plot(k1,k2);	break;
	case 'V':
		q1.x = p1.x;			q1.y = p1.y;			k1=Pnt.size();	MGL_PUSH(Pnt,q1,mutexPnt);
		q2.x = p1.x-kx+2*lx;	q2.y = p1.y-ky+2*ly;	k2=Pnt.size();	MGL_PUSH(Pnt,q2,mutexPnt);
		q3.x = p1.x+1.5*lx;		q3.y = p1.y+1.5*ly;		k3=Pnt.size();	MGL_PUSH(Pnt,q3,mutexPnt);
		q4.x = p1.x+kx+2*lx;	q4.y = p1.y+ky+2*ly;	k4=Pnt.size();	MGL_PUSH(Pnt,q4,mutexPnt);
		quad_plot(k1,k2,k4,k3);	break;
	case 'O':
		{
			q1.x = p1.x;	q1.y = p1.y;	k1=Pnt.size();	MGL_PUSH(Pnt,q1,mutexPnt);
			double t,c,s;
			for(int i=0;i<16;i++)
			{
				t = M_PI*i/8.;		s=sin(t);	c=cos(t);
				q2.x = p1.x+kx*s+lx*c;	q2.y = p1.y+ky*s+ly*c;
				k2=Pnt.size();	MGL_PUSH(Pnt,q2,mutexPnt);
				t = M_PI*(i+1)/8.;	s=sin(t);	c=cos(t);
				q3.x = p1.x+kx*s+lx*c;	q3.y = p1.y+ky*s+ly*c;
				k3=Pnt.size();	MGL_PUSH(Pnt,q3,mutexPnt);
				trig_plot(k1,k2,k3);
			}
			break;
		}
	}
}
//-----------------------------------------------------------------------------
wchar_t *mgl_wcsdup(const wchar_t *s);
void mglCanvas::AddLegend(const char *str,const char *style)
{
	if(!str)	return;
	unsigned s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	AddLegend(wcs, style);
	delete []wcs;
}
//-----------------------------------------------------------------------------
void mglCanvas::Legend(const std::vector<mglText> &leg, float x, float y, const char *font, float size, float ll)
{
	long n=leg.size();
	if(n<1)	{	SetWarn(mglWarnLeg);	return;	}
	static int cgid=1;	StartGroup("Legend",cgid++);
	if(ll<=0 || isnan(ll))	ll=0.1;
	ll *=font_factor;
	if(size<0)	size = -size*FontSize;
	// setup font and parse absolute coordinates
	if(!font)	font="#";
	char *pA, *ff = new char[strlen(font)+3];
	strcpy(ff,font);	strcat(ff,":L");	Push();
	if((pA=strchr(ff,'A')))	{	*pA = ' ';	InPlot(0,1,0,1,false);	}
	// find sizes
	float h=TextHeight(font,size)/2;
	float dx = 0.03*inW, dy = 0.03*inH, w=0, t;
	register long i,j;
	for(i=0;i<n;i++)		// find text length
	{
		t = TextWidth(leg[i].text.c_str(),font,size)/2;
		if(leg[i].stl.empty())	t -= ll;
		w = w>t ? w:t;
	}
	w += ll+0.01*inW;	// add space for lines
	x = x*(inW-w-2*dx)+B.x-inW/2+dx;
	y = y*(inH-h*n-2*dy)+B.y-inH/2+dy;
	// draw it
	long k1,k2,k3,k4;
	mglPoint p,q=mglPoint(NAN);
	float c1=AddTexture('w'), c2=AddTexture('k');
	if((Flag&3)==2)	{	float cc=c1;	c2=c2;	c2=cc;	};
	if(strchr(font,'#'))	// draw bounding box
	{
		k1=AddPnt(mglPoint(x,y,Depth),c1,q,-1,0);
		k2=AddPnt(mglPoint(x+w,y,Depth),c1,q,-1,0);
		k3=AddPnt(mglPoint(x,y+h*n,Depth),c1,q,-1,0);
		k4=AddPnt(mglPoint(x+w,y+h*n,Depth),c1,q,-1,0);
		quad_plot(k1,k2,k3,k4);
		k1=CopyNtoC(k1,c2);	k2=CopyNtoC(k2,c2);
		k3=CopyNtoC(k3,c2);	k4=CopyNtoC(k4,c2);
		line_plot(k1,k2);	line_plot(k2,k4);
		line_plot(k4,k3);	line_plot(k3,k1);
	}
	for(i=0;i<n;i++)	// draw lines and legend
	{
		char m=SetPenPal(leg[i].stl.c_str());
		k1=AddPnt(mglPoint(x+0.1*ll,y+i*h+0.45*h,Depth),CDef,q,-1,0);
		k2=AddPnt(mglPoint(x+0.9*ll,y+i*h+0.45*h,Depth),CDef,q,-1,0);
		pPos=0;	line_plot(k1,k2);
		if(m)	for(j=0;j<LegendMarks;j++)
		{
			p = mglPoint(x+0.1f*ll + (j+1)*0.8f*ll/(1.+LegendMarks),y+i*h+0.45*h,Depth);
			mark_plot(AddPnt(p,CDef,q,-1,0),m);
		}
		p = mglPoint(x+((!leg[i].stl.empty())?ll:0), y+i*h+0.15*h, Depth);
		text_plot(AddPnt(p,-1,q,-1,0), leg[i].text.c_str(), ff, size);
	}
	Pop();	EndGroup();	delete []ff;
}
//-----------------------------------------------------------------------------
void mglCanvas::FrameBox(const char *title,const char *stl,float size)
{
	wchar_t *wcs = 0;
	if(title)
	{
		unsigned s = strlen(title)+1;
		wcs = new wchar_t[s];
		mbstowcs(wcs,title,s);
	}
	FrameBox(wcs, stl,size);
	if(wcs)	delete []wcs;
}
//-----------------------------------------------------------------------------
void mglCanvas::FrameBox(const wchar_t *title,const char *stl,float size)
{
	float s = size>0 ? size/FontSize:-size, h=TextHeight(stl,size)*s/2;
	if(h>=inH)	{	SetWarn(mglWarnSpc,"FrameBox");	return;	}
	bool box=(stl && strchr(stl,'#'));
	int align;	mglGetStyle(stl,0,&align);	align = align&3;
	float x=B1.x-inW/2, y=B1.y+inH/2-h;
	mglPoint p(B1.x + inW/2.1*(align-1),y,Depth),q(NAN);
	if(title)	text_plot(AddPnt(p,-1,q,-1,0),title,stl,size);
	if(box)	//	draw boungind box
	{
		float c1=AddTexture('w'), c2=AddTexture('k');
		if((Flag&3)==2)	{	float cc=c1;	c2=c2;	c2=cc;	};
		long k1,k2,k3,k4;
		k1=AddPnt(mglPoint(x,y,Depth),c1,q,-1,0);
		k2=AddPnt(mglPoint(x+inW,y,Depth),c1,q,-1,0);
		k3=AddPnt(mglPoint(x,y+h,Depth),c1,q,-1,0);
		k4=AddPnt(mglPoint(x+inW,y+h,Depth),c1,q,-1,0);
		quad_plot(k1,k2,k3,k4);
		k1=CopyNtoC(k1,c2);	k2=CopyNtoC(k2,c2);
		k3=CopyNtoC(k3,c2);	k4=CopyNtoC(k4,c2);
		line_plot(k1,k2);	line_plot(k2,k4);
		line_plot(k4,k3);	line_plot(k3,k1);
	}

	B.clear();	B=B1;
	B.y = B1.y - h/2;
	B.b[4] = B1.b[4]-h;
	inH-=h;		Persp = 0;
	font_factor = B.b[0] < B.b[4] ? B.b[0] : B.b[4];
}
//-----------------------------------------------------------------------------
