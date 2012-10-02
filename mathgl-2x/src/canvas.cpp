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
#include <limits.h>
#include "mgl2/canvas.h"
//-----------------------------------------------------------------------------
std::string mglGlobalMess;	///< Buffer for receiving global messages
//-----------------------------------------------------------------------------
mglCanvas::mglCanvas(int w, int h) : mglBase()
{
	clr(MGL_DISABLE_SCALE);
	set(MGL_VECT_FRAME);	// NOTE: require a lot of memory!
	Z=0;	C=G=G4=0;	OI=0;	gif=0;
	CurFrameId=0;	Delay=0.5;
	Width=Height=Depth=0;	ObjId=-1;
	fscl=ftet=0;
	dr_nx1=dr_nx2=dr_ny1=dr_ny2=0;	// Allowed drawing region

	fnt = new mglFont;	fnt->gr = this;		ac.ch='c';
	ax.dir = mglPoint(1,0,0);	ax.a = mglPoint(0,1,0);	ax.b = mglPoint(0,0,1);	ax.ch='x';
	ay.dir = mglPoint(0,1,0);	ay.a = mglPoint(1,0,0);	ay.b = mglPoint(0,0,1);	ay.ch='y';
	az.dir = mglPoint(0,0,1);	az.a = mglPoint(0,1,0);	az.b = mglPoint(1,0,0);	az.ch='z';

	SetSize(w,h);	SetQuality(MGL_DRAW_NORM);	DefaultPlotParam();
}
//-----------------------------------------------------------------------------
mglCanvas::~mglCanvas()
{
	delete fnt;
	if(G)	{	delete []G;	delete []C;	delete []Z;	delete []G4;delete []OI;	}
}
//-----------------------------------------------------------------------------
long mglCanvas::PushDrwDat()
{
	mglDrawDat d;
	d.Pnt=Pnt;	d.Prm=Prm;	d.Glf=Glf;	d.Ptx=Ptx;	d.Txt=Txt;
	MGL_PUSH(DrwDat,d,mutexDrw);
	return DrwDat.size();
}
//-----------------------------------------------------------------------------
void mglCanvas::GetFrame(long i)
{
	if(i<0 || (size_t)i>=DrwDat.size())	return;
	Clf();
	const mglDrawDat &d=DrwDat[i];
	Pnt=d.Pnt;	Prm=d.Prm;	Glf=d.Glf;	Ptx=d.Ptx;	Txt=d.Txt;
}
//-----------------------------------------------------------------------------
const unsigned char *mglCanvas::GetBits()	{	Finish();	return G;	}
//-----------------------------------------------------------------------------
mreal mglCanvas::GetRatio() const	{	return inW/inH;	}
//-----------------------------------------------------------------------------
void mglCanvas::add_prim(mglPrim &a)
{
	if(a.n1>=0)
	{	a.z = Pnt[a.n1].z;	// this is a bit less accurate but simpler for transformation
		a.id = ObjId;	MGL_PUSH(Prm,a,mutexPrm);	}
}
//-----------------------------------------------------------------------------
void mglCanvas::DefaultPlotParam()
{
/* NOTE: following variables and mutex will not be changed by DefaultPlotParam()
std::vector<mglTexture> Txt;	///< Pointer to textures
char *Message;		///< Buffer for receiving messages
long InUse;			///< Smart pointer (number of users)
mglFont *fnt;		///< Class for printing vector text
int Quality;		///< Quality of plot (0x0-pure, 0x1-fast; 0x2-fine; 0x4 - low memory)
int Width;			///< Width of the image
int Height;			///< Height of the image
int Depth;			///< Depth of the image
int CurFrameId;		///< Number of automaticle created frames
GifFileType *gif;*/
	SetTickRotate(true);	SetTickSkip(true);
	SetWarn(mglWarnNone,"");	mglGlobalMess = "";
	ObjId = -1;	HighId = INT_MIN;
	SetFunc(0,0);	Stop=false;	CutOff(0);	Ternary(0);
	SetRanges(mglPoint(-1,-1,-1,-1), mglPoint(1,1,1,1));
	SetBarWidth(0.7);	SetMarkSize(1);	SetArrowSize(1);
	SetAlphaDef(0.5);		FontDef[0]=0;
	SetTranspType(0);		SetMeshNum(0);	// NOTE: default MeshNum=0
	SetRotatedText(true);	CurrPal = 0;
	SetLegendMarks();		SetFontSize(4);
	SetTuneTicks(-1);		SetAmbient();
	PlotId = "frame";		clr(MGL_DISABLE_SCALE);
	clr(MGL_USE_GMTIME);	clr(MGL_NOSUBTICKS);
	SetDifLight(false);		SetReduceAcc(false);
	SetDefScheme("BbcyrR");	SetPalette(MGL_DEF_PAL);
	SetPenPal("k-1");		Alpha(false);
	SetTicks('x');	SetTicks('y');	SetTicks('z');	SetTicks('c');
	stack.clear();	Restore();	DefColor('k');
	SetPlotFactor(0);	InPlot(0,1,0,1,false);
	SetTickLen(0);	SetCut(true);
	AdjustTicks("xyzc",true);	Clf();

	for(int i=0;i<10;i++)	{	AddLight(i, mglPoint(0,0,1));	Light(i,false);	}
	Light(0,true);	Light(false);	SetDifLight(true);
}
//-----------------------------------------------------------------------------
//	Optimal axis position
//-----------------------------------------------------------------------------
mreal mglCanvas::FindOptOrg(char dir, int ind) const
{
	static mglPoint px, py, pz, m1, m2;
	static mglMatrix bb;	bb.b[0]=1e30;
	mglPoint nn[8]={mglPoint(0,0,0), mglPoint(0,0,1), mglPoint(0,1,0,0), mglPoint(0,1,1),
	mglPoint(1,0,0), mglPoint(1,0,1), mglPoint(1,1,0), mglPoint(1,1,1)}, pp[8];
	memcpy(pp, nn, 8*sizeof(mglPoint));
	// do nothing if transformation matrix the same
	if(memcmp(B.b,bb.b,9*sizeof(mreal)) || m1!=Min || m2!=Max)
	{
		m1 = Min;	m2 = Max;	memcpy(&bb,&B,sizeof(mglMatrix));
		PostScale(pp,8);
		// find point with minimal y
		register long i,j=0;
		for(i=1;i<8;i++)	if(pp[i].y<pp[j].y)	j=i;
		pp[0]=pp[j];
		// find max angle and left point
		// first select 3 closest points
		pp[1]=nn[j];	pp[1].x=1-pp[1].x;
		pp[2]=nn[j];	pp[2].y=1-pp[2].y;
		pp[3]=nn[j];	pp[3].z=1-pp[3].z;
		PostScale(pp+1,3);
		pp[1]-=pp[0];	pp[2]-=pp[0];	pp[3]-=pp[0];
		// find cosine of axis projection
		mreal cxy, cxz, cyz, dx, dy, dz;
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
	mreal res = px.val(ind);
	if(dir=='y')	res = py.val(ind);
	if(dir=='z')	res = pz.val(ind);
	return res;
}
//-----------------------------------------------------------------------------
mreal mglCanvas::GetOrgX(char dir) const
{
	mreal res = Org.x;
	if(mgl_isnan(res))
	{
		if(strchr("xyz",dir))	res = FindOptOrg(dir,0);
		else if(dir=='t')		res = Min.x;
		else res = B.b[6]>0 ? Max.x:Min.x;
	}
	return res;
}
//-----------------------------------------------------------------------------
mreal mglCanvas::GetOrgY(char dir) const
{
	mreal res = Org.y;
	if(mgl_isnan(res))
	{
		if(strchr("xyz",dir))	res = FindOptOrg(dir,1);
		else if(dir=='t')	res = Min.y;
		else res = B.b[7]>0 ? Max.y:Min.y;
	}
	return res;
}
//-----------------------------------------------------------------------------
mreal mglCanvas::GetOrgZ(char dir) const
{
	mreal res = Org.z;
	if(mgl_isnan(res))
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
#define MGL_MARK_PLOT	if(Quality&4)	mark_draw(p,type,size?size:MarkSize,&d);else	\
						{	mglPrim a;	a.w = fabs(PenWidth);	a.s = size?size:MarkSize;	\
							a.n1 = p;	a.n4 = type;	add_prim(a);	}
void mglCanvas::mark_plot(long p, char type, mreal size)
{
	if(p<0 || mgl_isnan(Pnt[p].x))	return;
	long pp=p;
	mglDrawReg d;	d.set(this,1,1,0);
	if(size>=0)	size *= MarkSize;
	if(TernAxis&4) for(int i=0;i<4;i++)
	{	p = ProjScale(i, pp);	MGL_MARK_PLOT	}
	else	{	MGL_MARK_PLOT	}
}
//-----------------------------------------------------------------------------
#define MGL_LINE_PLOT	if(Quality&4)	line_draw(p1,p2,&dd);else	\
						{	mglPrim a(1);	a.n3=PDef;	a.s = pPos;	\
							a.n1 = p1;	a.n2 = p2;	a.w = pw;	add_prim(a);	}
void mglCanvas::line_plot(long p1, long p2)
{
	if(PDef==0)	return;
	if(p1<0 || p2<0 || mgl_isnan(Pnt[p1].x) || mgl_isnan(Pnt[p2].x))	return;
	mglDrawReg dd;	dd.set(this,1,1,0);
	long pp1=p1,pp2=p2;
	mreal pw = fabs(PenWidth),d;
	d = hypot(Pnt[p1].x-Pnt[p2].x, Pnt[p1].y-Pnt[p2].y);
	if(TernAxis&4) for(int i=0;i<4;i++)
	{	p1 = ProjScale(i, pp1);	p2 = ProjScale(i, pp2);
		MGL_LINE_PLOT	}
	else	{	MGL_LINE_PLOT	}
	pPos = fmod(pPos+d/pw/1.5, 16);
}
//-----------------------------------------------------------------------------
#define MGL_TRIG_PLOT	if(Quality&4)	trig_draw(p1,p2,p3,true,&d);else	\
						{	mglPrim a(2);	a.n1 = p1;	a.n2 = p2;	\
							a.n3 = p3;	add_prim(a);}
void mglCanvas::trig_plot(long p1, long p2, long p3)
{
	if(p1<0 || p2<0 || p3<0 || mgl_isnan(Pnt[p1].x) || mgl_isnan(Pnt[p2].x) || mgl_isnan(Pnt[p3].x))	return;
	long pp1=p1,pp2=p2,pp3=p3;
	mglDrawReg d;	d.set(this,1,1,0);
	if(TernAxis&4) for(int i=0;i<4;i++)
	{	p1 = ProjScale(i, pp1);	p2 = ProjScale(i, pp2);
		p3 = ProjScale(i, pp3);	MGL_TRIG_PLOT	}
	else	{	MGL_TRIG_PLOT	}
}
//-----------------------------------------------------------------------------
#define MGL_QUAD_PLOT	if(Quality&4)	quad_draw(p1,p2,p3,p4,&d);else	\
						{	mglPrim a(3);	a.n1 = p1;	a.n2 = p2;	\
							a.n3 = p3;	a.n4 = p4;	add_prim(a);	}
void mglCanvas::quad_plot(long p1, long p2, long p3, long p4)
{
	if(p1<0 || mgl_isnan(Pnt[p1].x))	{	trig_plot(p4,p2,p3);	return;	}
	if(p2<0 || mgl_isnan(Pnt[p2].x))	{	trig_plot(p1,p4,p3);	return;	}
	if(p3<0 || mgl_isnan(Pnt[p3].x))	{	trig_plot(p1,p2,p4);	return;	}
	if(p4<0 || mgl_isnan(Pnt[p4].x))	{	trig_plot(p1,p2,p3);	return;	}
	long pp1=p1,pp2=p2,pp3=p3,pp4=p4;
	mglDrawReg d;	d.set(this,1,1,0);
	if(TernAxis&4) for(int i=0;i<4;i++)
	{	p1 = ProjScale(i, pp1);	p2 = ProjScale(i, pp2);
		p3 = ProjScale(i, pp3);	p4 = ProjScale(i, pp4);
		MGL_QUAD_PLOT	}
	else	{	MGL_QUAD_PLOT	}
}
//-----------------------------------------------------------------------------
mreal mglCanvas::text_plot(long p,const wchar_t *text,const char *font,mreal size,mreal sh,mreal col,bool rot)
{
	if(p<0 || mgl_isnan(Pnt[p].x) || !text || *text==0)	return 0;
	if(size<0)	size *= -FontSize;
	if(!font)	font="";

	if(TernAxis&4)	// text at projections
	{
		mreal res;
		TernAxis = TernAxis&(~4);
		for(int i=0;i<4;i++)
			res = text_plot(ProjScale(i,p),text,font,size/2,sh,col);
		TernAxis = TernAxis|4;
		return res;
	}

	mglPnt q=Pnt[p];
	mreal ll = q.u*q.u+q.v*q.v;
	bool inv=false;
	if(rot && (q.u<0 || (q.u==0 && q.v<0)))
	{	q.u=-q.u;	q.v=-q.v;	q.w=-q.w;	inv=true;	}
	if(!(Quality&4))	// add text itself
	{
		mglPrim a(6);	a.n1 = p;
		mglText txt(text,font);
		MGL_PUSH(Ptx,txt,mutexPtx);
		a.n3 = Ptx.size()-1;
		a.s = size;	a.w = sh;	a.p=col;
		add_prim(a);
	}
	if(ll==0)	return 0;

	mreal fsize=size/6.5*font_factor, h = fnt->Height(font)*fsize, w, shift = -(sh+0.02)*h;
	// text drawing itself
	Push();
	inv = inv ^ (strchr(font,'T')!=0);
	if(inv)	shift = 0.2*h-shift;
	shift += 0.015*h;	// Correction for glyph rotation around proper point
//	shift *= h;

	int align;	mglGetStyle(font,0,&align);	align = align&3;
	B.x = q.x;	B.y = q.y - shift;	B.z = q.z;
	if(ll>0)
	{
		B.x += shift*q.v/sqrt(ll);	B.y += shift*(1-q.u/sqrt(ll));
		if(q.u==0 && !get(MGL_ENABLE_RTEXT))	B.y -= 0.1*h;
	}
	fscl = fsize;

	if(mgl_isnan(ll) || !get(MGL_ENABLE_RTEXT))	ftet = 0;
	else	ftet = -180*atan2(q.v,q.u)/M_PI;

	memset(B.b,0,9*sizeof(mreal));
	B.b[0] = B.b[4] = B.b[8] = fscl;
	register mreal opf = B.pf;
	RotateN(ftet,0,0,1);	B.pf = opf;
	if(strchr(font,'@'))	// draw box around text
	{
		long k1,k2,k3,k4;	mglPnt pt;	mglPoint pp;
		w = fnt->Width(text,font);	h = fnt->Height(font);
//		int align;	mglGetStyle(font,0,&align);	align = align&3;
		mreal d=-w*align/2.-h*0.2;	w+=h*0.4;
		pt = q;	pp = mglPoint(d,-h*0.4);		PostScale(pp);
		pt.x=pt.xx=pp.x;	pt.y=pt.yy=pp.y;	MGL_PUSH(Pnt,pt,mutexPnt);	k1=Pnt.size()-1;
		pt = q;	pp = mglPoint(w+d,-h*0.4);		PostScale(pp);
		pt.x=pt.xx=pp.x;	pt.y=pt.yy=pp.y;	MGL_PUSH(Pnt,pt,mutexPnt);	k2=Pnt.size()-1;
		pt = q;	pp = mglPoint(d,h*1.2);			PostScale(pp);
		pt.x=pt.xx=pp.x;	pt.y=pt.yy=pp.y;	MGL_PUSH(Pnt,pt,mutexPnt);	k3=Pnt.size()-1;
		pt = q;	pp = mglPoint(w+d,h*1.2);		PostScale(pp);
		pt.x=pt.xx=pp.x;	pt.y=pt.yy=pp.y;	MGL_PUSH(Pnt,pt,mutexPnt);	k4=Pnt.size()-1;
		line_plot(k1,k2);	line_plot(k1,k3);	line_plot(k4,k2);	line_plot(k4,k3);
	}
	fsize *= fnt->Puts(text,font,col)/2;
	Pop();	return fsize;
}
//-----------------------------------------------------------------------------
void mglCanvas::Glyph(mreal x, mreal y, mreal f, int s, long j, mreal col)
{
	mglPrim a(4);	// NOTE: no projection since text_plot() did it
	a.s = fscl/B.pf;	a.w = ftet;	a.p = f/fnt->GetFact(s&3);
	mreal cc = col<0 ? AddTexture(char(0.5-col)):col;
	if(cc<0)	cc = CDef;
	a.n1 = AddPnt(mglPoint(B.x,B.y,B.z), cc, mglPoint(x,y,NAN), -1, -1);
	a.n3 = s;	a.n4 = AddGlyph(s,j);
	if(a.n1<0)	return;
	mglDrawReg d;	d.set(this,1,1,0);
	if(Quality&4)	glyph_draw(&a,&d);
	else	add_prim(a);
}
//-----------------------------------------------------------------------------
//	Plot positioning functions
//-----------------------------------------------------------------------------
void mglCanvas::InPlot(mreal x1,mreal x2,mreal y1,mreal y2, const char *st)
{
	if(Width<=0 || Height<=0 || Depth<=0)	return;
	if(!st)		{	InPlot(x1,x2,y1,y2,false);	return;	}
	inW = Width*(x2-x1);	inH = Height*(y2-y1);
	inX=Width*x1;	inY=Height*y1;	ZMin=1;
	mglPrim p;	p.id = ObjId;
	p.n1=x1*Width;	p.n2=x2*Width;	p.n3=y1*Height;	p.n4=y2*Height;
	MGL_PUSH(Sub,p,mutexSub);

	if(strchr(st,'T'))	{	y1*=0.9;	y2*=0.9;	}	// general title
	bool r = !(strchr(st,'r') || strchr(st,'R') || strchr(st,'>') || strchr(st,'g'));
	bool l = !(strchr(st,'l') || strchr(st,'L') || strchr(st,'<') || strchr(st,'g'));
	bool u = !(strchr(st,'u') || strchr(st,'U') || strchr(st,'_') || strchr(st,'g'));
	bool a = !(strchr(st,'a') || strchr(st,'A') || strchr(st,'^') || strchr(st,'g') || strchr(st,'t'));
	// let use simplified scheme -- i.e. no differences between axis, colorbar and/or title
	register mreal xs=(x1+x2)/2, ys=(y1+y2)/2, f1 = 1.3, f2 = 1.1;
	if(strchr(st,'#'))	f1=f2=1.55;
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
void mglCanvas::InPlot(mreal x1,mreal x2,mreal y1,mreal y2, bool rel)
{
	if(Width<=0 || Height<=0 || Depth<=0)	return;
	B.clear();
	if(get(MGL_AUTO_FACTOR)) B.pf = 1.55;	// Automatically change plot factor !!!
	if(rel)
	{
		B.x = B1.x + (x1+x2-1)/2*B1.b[0]/1.55;
		B.y = B1.y + (y1+y2-1)/2*B1.b[4]/1.55;
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
	inW=B.b[0];	inH=B.b[4];	ZMin=1;
	inX=Width*x1;	inY=Height*y1;
	font_factor = B.b[0] < B.b[4] ? B.b[0] : B.b[4];
	mglPrim p;	p.id = ObjId;
	p.n1=x1*Width;	p.n2=x2*Width;	p.n3=y1*Height;	p.n4=y2*Height;
	MGL_PUSH(Sub,p,mutexSub);
}
//-----------------------------------------------------------------------------
void mglCanvas::StickPlot(int num, int id, mreal tet, mreal phi)
{
	mreal dx,dy,wx,wy,x1,y1,f1,f2;
	mglPoint p1(-1,0,0), p2(1,0,0);
	// first iteration
	InPlot(0,1,0,1,true);	Rotate(tet, phi);
	PostScale(p1);	PostScale(p2);	f1 = B.pf;
	dx=(p2.x-p1.x)*1.55/B1.b[0];	dy=(p2.y-p1.y)*1.55/B1.b[4];
	wx=1/(1+(num-1)*fabs(dx));		wy=1/(1+(num-1)*fabs(dy));
	x1=dx>0?dx*id:dx*(id-num+1);	y1=dy>0?dy*id:dy*(id-num+1);
	InPlot(x1*wx,(x1+1)*wx,y1*wy,(y1+1)*wy,true);	Rotate(tet,phi);
	f2 = B.pf;	dx*=f1/f2;	dy*=f1/f2;	// add correction due to PlotFactor
	wx=1/(1+(num-1)*fabs(dx));		wy=1/(1+(num-1)*fabs(dy));
	x1=dx>0?dx*id:dx*(id-num+1);	y1=dy>0?dy*id:dy*(id-num+1);
	InPlot(x1*wx,(x1+1)*wx,y1*wy,(y1+1)*wy,true);	Rotate(tet,phi);
	f1=f2;	f2 = B.pf;	dx*=f1/f2;	dy*=f1/f2;	// add correction due to PlotFactor
	wx=1/(1+(num-1)*fabs(dx));		wy=1/(1+(num-1)*fabs(dy));
	x1=dx>0?dx*id:dx*(id-num+1);	y1=dy>0?dy*id:dy*(id-num+1);
	InPlot(x1*wx,(x1+1)*wx,y1*wy,(y1+1)*wy,true);	Rotate(tet,phi);
}
//-----------------------------------------------------------------------------
void mglCanvas::Rotate(mreal tetz,mreal tetx,mreal tety)
{
//	RotateN(TetX,1.,0.,0.);
//	RotateN(TetY,0.,1.,0.);
//	RotateN(TetZ,0.,0.,1.);
	mreal R[9], O[9];
	mreal cx=cos(tetx*M_PI/180), sx=-sin(tetx*M_PI/180), cy=cos(tety*M_PI/180), sy=-sin(tety*M_PI/180), cz=cos(tetz*M_PI/180), sz=-sin(tetz*M_PI/180);
	R[0] = cx*cy;			R[1] = -cy*sx;			R[2] = sy;
	R[3] = cx*sy*sz+cz*sx;	R[4] = cx*cz-sx*sy*sz;	R[5] =-cy*sz;
	R[6] = sx*sz-cx*cz*sy;	R[7] = cx*sz+cz*sx*sy;	R[8] = cy*cz;
	memcpy(O,B.b,9*sizeof(mreal));
	B.b[0] = R[0]*O[0] + R[3]*O[1] + R[6]*O[2];
	B.b[1] = R[1]*O[0] + R[4]*O[1] + R[7]*O[2];
	B.b[2] = R[2]*O[0] + R[5]*O[1] + R[8]*O[2];
	B.b[3] = R[0]*O[3] + R[3]*O[4] + R[6]*O[5];
	B.b[4] = R[1]*O[3] + R[4]*O[4] + R[7]*O[5];
	B.b[5] = R[2]*O[3] + R[5]*O[4] + R[8]*O[5];
	B.b[6] = R[0]*O[6] + R[3]*O[7] + R[6]*O[8];
	B.b[7] = R[1]*O[6] + R[4]*O[7] + R[7]*O[8];
	B.b[8] = R[2]*O[6] + R[5]*O[7] + R[8]*O[8];
	if(get(MGL_AUTO_FACTOR))
	{
		mreal w=(fabs(B.b[3])+fabs(B.b[4])+fabs(B.b[5]))/B1.b[4];
		mreal h=(fabs(B.b[0])+fabs(B.b[1])+fabs(B.b[2]))/B1.b[0];
		B.pf = 1.55+0.6147*(w<h ? (h-1):(w-1));
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::RotateN(mreal Tet,mreal x,mreal y,mreal z)
{
	mreal R[9],T[9],c=cos(Tet*M_PI/180),s=-sin(Tet*M_PI/180),r=1-c,n=sqrt(x*x+y*y+z*z);
	x/=n;	y/=n;	z/=n;
	T[0] = x*x*r+c;		T[1] = x*y*r-z*s;	T[2] = x*z*r+y*s;
	T[3] = x*y*r+z*s;	T[4] = y*y*r+c;		T[5] = y*z*r-x*s;
	T[6] = x*z*r-y*s;	T[7] = y*z*r+x*s;	T[8] = z*z*r+c;
	memcpy(R,B.b,9*sizeof(mreal));
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
		mreal w=(fabs(B.b[3])+fabs(B.b[4])+fabs(B.b[5]))/B1.b[4];
		mreal h=(fabs(B.b[0])+fabs(B.b[1])+fabs(B.b[2]))/B1.b[0];
		B.pf = 1.55+0.6147*(w<h ? (h-1):(w-1));
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::View(mreal tetx,mreal tetz,mreal tety)
{
	mreal R[9], A[9];
	mreal cx=cos(tetx*M_PI/180), sx=sin(tetx*M_PI/180);
	mreal cy=cos(tety*M_PI/180), sy=sin(tety*M_PI/180);
	mreal cz=cos(tetz*M_PI/180), sz=sin(tetz*M_PI/180);
	R[0] = cx*cy;			R[1] = -cy*sx;			R[2] = sy;
	R[3] = cx*sy*sz+cz*sx;	R[4] = cx*cz-sx*sy*sz;	R[5] =-cy*sz;
	R[6] = sx*sz-cx*cz*sy;	R[7] = cx*sz+cz*sx*sy;	R[8] = cy*cz;
	memcpy(A,Bp.b,9*sizeof(mreal));		ClfZB();
	Bp.b[0] = R[0]*A[0] + R[3]*A[1] + R[6]*A[2];
	Bp.b[1] = R[1]*A[0] + R[4]*A[1] + R[7]*A[2];
	Bp.b[2] = R[2]*A[0] + R[5]*A[1] + R[8]*A[2];
	Bp.b[3] = R[0]*A[3] + R[3]*A[4] + R[6]*A[5];
	Bp.b[4] = R[1]*A[3] + R[4]*A[4] + R[7]*A[5];
	Bp.b[5] = R[2]*A[3] + R[5]*A[4] + R[8]*A[5];
	Bp.b[6] = R[0]*A[6] + R[3]*A[7] + R[6]*A[8];
	Bp.b[7] = R[1]*A[6] + R[4]*A[7] + R[7]*A[8];
	Bp.b[8] = R[2]*A[6] + R[5]*A[7] + R[8]*A[8];
}
//-----------------------------------------------------------------------------
void mglCanvas::Zoom(mreal x1, mreal y1, mreal x2, mreal y2)
{
	Bp.pf=0;	Bp.clear();		ClfZB();
	if(x1==x2 || y1==y2)	{	x1=y1=0;	x2=y2=1;	}
	x1=2*x1-1;	x2=2*x2-1;	y1=2*y1-1;	y2=2*y2-1;
	Bp.b[0]=2/fabs(x2-x1);	Bp.b[4]=2/fabs(y2-y1);
	Bp.x=(x1+x2)/fabs(x2-x1);Bp.y=(y1+y2)/fabs(y2-y1);
}
//-----------------------------------------------------------------------------
int mglCanvas::GetSplId(long x,long y) const
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
void mglCanvas::Aspect(mreal Ax,mreal Ay,mreal Az)
{
	mreal a = fabs(Ax) > fabs(Ay) ? fabs(Ax) : fabs(Ay);
	a = a > fabs(Az) ? a : fabs(Az);
	if(a==0)	{	SetWarn(mglWarnZero,"Aspect");	return;	}
	Ax/=a;	Ay/=a;	Az/=a;
	B.b[0] *= Ax;		B.b[3] *= Ax;		B.b[6] *= Ax;
	B.b[1] *= Ay;		B.b[4] *= Ay;		B.b[7] *= Ay;
	B.b[2] *= Az;		B.b[5] *= Az;		B.b[8] *= Az;
}
//-----------------------------------------------------------------------------
//	Lighting and transparency
//-----------------------------------------------------------------------------
void mglCanvas::Fog(mreal d, mreal dz)	{	FogDist=d;	FogDz = dz;	}
//-----------------------------------------------------------------------------
void mglCanvas::Light(int n, bool enable)
{
	if(n<0 || n>9)	{	SetWarn(mglWarnLId,"Light");	return;	}
	light[n].n = enable;
}
//-----------------------------------------------------------------------------
void mglCanvas::AddLight(int n, mglPoint r, mglPoint d, char col, mreal br, mreal ap)
{
	if(n<0 || n>9)	{	SetWarn(mglWarnLId,"AddLight");	return;	}
	light[n].n = true;	light[n].a = ap>0?ap*ap:3;
	light[n].b = br;	light[n].r = r;
	light[n].d = d;		light[n].c = mglColor(col);
}
//-----------------------------------------------------------------------------
void mglCanvas::arrow_plot(long n1, long n2, char st)
{
	if(get(MGL_3D_ARROW))
		arrow_plot_3d(n1, n2, st);
	else
		arrow_plot_2d(n1, n2, st);
}
//-----------------------------------------------------------------------------
void mglCanvas::arrow_plot_2d(long n1, long n2, char st)
{
	if(n1<0 || n2<0 || !strchr("AVKSDTIO",st))	return;
	const mglPnt &p1=Pnt[n1], &p2=Pnt[n2];
	mglPnt q=p1; 	q.u=q.v=NAN;

	mreal lx=p1.x-p2.x, ly=p1.y-p2.y, ll, kx,ky;
	ll = hypot(lx,ly)/(PenWidth*ArrowSize*0.35*font_factor);
	if(ll==0)	return;
	lx /= ll;	ly /= ll;	kx = ly;	ky = -lx;
	mreal lz = (p2.z-p1.z)/ll;

	Reserve(6);
	long k1,k2,k3,k4;

	switch(st)
	{
	case 'I':
		q.xx=q.x=p1.x+kx;		q.yy=q.y=p1.y+ky;	MGL_PUSH(Pnt,q,mutexPnt);	k1=Pnt.size()-1;
		q.xx=q.x=p1.x-kx;		q.yy=q.y=p1.y-ky;	MGL_PUSH(Pnt,q,mutexPnt);	k2=Pnt.size()-1;
		line_plot(k1,k2);	break;
	case 'D':
		q.xx=q.x=p1.x+kx;		q.yy=q.y=p1.y+ky;						MGL_PUSH(Pnt,q,mutexPnt);	k1=Pnt.size()-1;
		q.xx=q.x=p1.x+lx;		q.yy=q.y=p1.y+ly;	q.zz=q.z=p1.z+lz;	MGL_PUSH(Pnt,q,mutexPnt);	k2=Pnt.size()-1;
		q.xx=q.x=p1.x-kx;		q.yy=q.y=p1.y-ky;						MGL_PUSH(Pnt,q,mutexPnt);	k3=Pnt.size()-1;
		q.xx=q.x=p1.x-lx;		q.yy=q.y=p1.y-ly;	q.zz=q.z=p1.z-lz;	MGL_PUSH(Pnt,q,mutexPnt);	k4=Pnt.size()-1;
		quad_plot(k1,k2,k4,k3);	break;
	case 'S':
		q.xx=q.x=p1.x+kx-lx;	q.yy=q.y=p1.y+ky-ly;	q.zz=q.z=p1.z-lz;	MGL_PUSH(Pnt,q,mutexPnt);	k1=Pnt.size()-1;
		q.xx=q.x=p1.x-kx-lx;	q.yy=q.y=p1.y-ky-ly;	q.zz=q.z=p1.z-lz;	MGL_PUSH(Pnt,q,mutexPnt);	k2=Pnt.size()-1;
		q.xx=q.x=p1.x-kx+lx;	q.yy=q.y=p1.y-ky+ly;	q.zz=q.z=p1.z+lz;	MGL_PUSH(Pnt,q,mutexPnt);	k3=Pnt.size()-1;
		q.xx=q.x=p1.x+kx+lx;	q.yy=q.y=p1.y+ky+ly;	q.zz=q.z=p1.z+lz;	MGL_PUSH(Pnt,q,mutexPnt);	k4=Pnt.size()-1;
		quad_plot(k1,k2,k4,k3);	break;
	case 'T':
		q.xx=q.x=p1.x+kx-lx;	q.yy=q.y=p1.y+ky-ly;	q.zz=q.z=p1.z-lz;	MGL_PUSH(Pnt,q,mutexPnt);	k1=Pnt.size()-1;
		q.xx=q.x=p1.x-kx-lx;	q.yy=q.y=p1.y-ky-ly;	q.zz=q.z=p1.z-lz;	MGL_PUSH(Pnt,q,mutexPnt);	k2=Pnt.size()-1;
		q.xx=q.x=p1.x+lx;	q.yy=q.y=p1.y+ly;	q.zz=q.z=p1.z+lz;	MGL_PUSH(Pnt,q,mutexPnt);	k3=Pnt.size()-1;
		trig_plot(k1,k2,k3);	break;
	case 'K':
		q.xx=q.x=p1.x+kx;	q.yy=q.y=p1.y+ky;	MGL_PUSH(Pnt,q,mutexPnt);	k1=Pnt.size()-1;
		q.xx=q.x=p1.x-kx;	q.yy=q.y=p1.y-ky;	MGL_PUSH(Pnt,q,mutexPnt);	k2=Pnt.size()-1;
		line_plot(k1,k2);
	case 'A':
		q.xx=q.x=p1.x-kx-2*lx;	q.yy=q.y=p1.y-ky-2*ly;	q.zz=q.z=p1.z-2*lz;	MGL_PUSH(Pnt,q,mutexPnt);	k2=Pnt.size()-1;
		q.xx=q.x=p1.x-1.5*lx;	q.yy=q.y=p1.y-1.5*ly;	q.zz=q.z=p1.z-1.5*lz;MGL_PUSH(Pnt,q,mutexPnt);	k3=Pnt.size()-1;
		q.xx=q.x=p1.x+kx-2*lx;	q.yy=q.y=p1.y+ky-2*ly;	q.zz=q.z=p1.z-2*lz;	MGL_PUSH(Pnt,q,mutexPnt);	k4=Pnt.size()-1;
		trig_plot(n1,k2,k3);		trig_plot(n1,k3,k4);	break;
	case 'V':
		q.xx=q.x=p1.x-kx+2*lx;	q.yy=q.y=p1.y-ky+2*ly;	q.zz=q.z=p1.z-2*lz;	MGL_PUSH(Pnt,q,mutexPnt);	k2=Pnt.size()-1;
		q.xx=q.x=p1.x+1.5*lx;	q.yy=q.y=p1.y+1.5*ly;	q.zz=q.z=p1.z-1.5*lz;MGL_PUSH(Pnt,q,mutexPnt);	k3=Pnt.size()-1;
		q.xx=q.x=p1.x+kx+2*lx;	q.yy=q.y=p1.y+ky+2*ly;	q.zz=q.z=p1.z-2*lz;	MGL_PUSH(Pnt,q,mutexPnt);	k4=Pnt.size()-1;
		trig_plot(n1,k2,k3);		trig_plot(n1,k3,k4);	break;
	case 'O':
		{
			float t,c,s;
			q.xx=q.x=p1.x+lx;	q.yy=q.y=p1.y+ly;	q.zz=q.z=p1.z+lz;
			MGL_PUSH(Pnt,q,mutexPnt);	k3=Pnt.size()-1;
			for(int i=0;i<12;i++)
			{
				t = M_PI*(i+1)/6.;	s=sin(t);	c=cos(t);
				q.xx=q.x=p1.x+kx*s+lx*c;	q.yy=q.y=p1.y+ky*s+ly*c;	q.zz=q.z=p1.z+c*lz;
				k2=k3;	MGL_PUSH(Pnt,q,mutexPnt);	k3=Pnt.size()-1;
				trig_plot(n1,k2,k3);
			}
			break;
		}
	}
}
//-----------------------------------------------------------------------------
long mglCanvas::setPp(mglPnt &q, const mglPoint &p)
{
	q.xx=q.x=p.x;	q.yy=q.y=p.y;	q.zz=q.z=p.z;
	MGL_PUSH(Pnt,q,mutexPnt);
	return Pnt.size()-1;
}
//-----------------------------------------------------------------------------
void mglCanvas::arrow_plot_3d(long n1, long n2, char st)
{
	if(n1<0 || n2<0 || !strchr("AVKSDTIO",st))	return;
	const mglPnt &p1=Pnt[n1], &p2=Pnt[n2];
	mglPnt q=p1; 	//q.u=q.v=NAN;

	mglPoint kl=mglPoint(p1.x-p2.x,p1.y-p2.y,p1.z-p2.z), kt, kz, p0=mglPoint(p1.x,p1.y,p1.z), p;
	if(kl.norm()==0)	return;
	kl.Normalize();	kt = !kl;	kz = kl^kt;
	
	mreal ll = PenWidth*ArrowSize*0.35*font_factor;
	kl *= ll;	kt *= ll;	kz *= ll;

	Reserve(8);
	long k1,k2,k3,k4,k5, k6,k7,k8;
	
	switch(st)	// S,D -- cube, T -- sq.pyramid, I -- square, O -- sphere???, A,K,V -- cone???
	{
		case 'I':
			k1=setPp(q,p0+kt);	k2=setPp(q,p0+kz);
			k3=setPp(q,p0-kt);	k4=setPp(q,p0-kz);
			line_plot(k1,k2);	line_plot(k1,k4);
			line_plot(k3,k2);	line_plot(k3,k4);
			quad_plot(k1,k2,k3,k4);	break;
		case 'D':
			k1=setPp(q,p0+kl);	k2=setPp(q,p0-kl);	k5=k3=setPp(q,p0+kt);
			k4=setPp(q,p0+kz);	trig_plot(k1,k3,k4);	trig_plot(k2,k3,k4);	k3=k4;
			k4=setPp(q,p0-kt);	trig_plot(k1,k3,k4);	trig_plot(k2,k3,k4);	k3=k4;
			k4=setPp(q,p0-kz);	trig_plot(k1,k3,k4);	trig_plot(k2,k3,k4);	k3=k4;
			trig_plot(k1,k3,k5);	trig_plot(k2,k3,k5);	break;
		case 'S':
			k1=setPp(q,p0+kl+kt);	k2=setPp(q,p0+kl+kz);	k3=setPp(q,p0+kl-kt);	k4=setPp(q,p0+kl-kz);
			k5=setPp(q,p0-kl+kt);	k6=setPp(q,p0-kl+kz);	k7=setPp(q,p0-kl-kt);	k8=setPp(q,p0-kl-kz);
			quad_plot(k1,k2,k3,k4);	quad_plot(k1,k2,k5,k6);	quad_plot(k3,k2,k6,k7);
			quad_plot(k1,k4,k5,k8);	quad_plot(k3,k4,k7,k8);	quad_plot(k5,k6,k7,k8);	break;
		case 'T':
			k1=setPp(q,p0-kl+kt);	k2=setPp(q,p0-kl+kz);	k3=setPp(q,p0-kl-kt);
			k4=setPp(q,p0-kl-kz);	k5=setPp(q,p0+kl);
			trig_plot(k1,k2,k5);	trig_plot(k2,k3,k5);
			trig_plot(k3,k4,k5);	trig_plot(k1,k4,k5);	break;
		case 'K':
			k1=setPp(q,p0+kt);	k2=setPp(q,p0+kz);
			k3=setPp(q,p0-kt);	k4=setPp(q,p0-kz);	quad_plot(k1,k2,k3,k4);
			line_plot(k1,k2);	line_plot(k1,k4);
			line_plot(k3,k2);	line_plot(k3,k4);
		case 'A':
			k1=setPp(q,p0-2.*kl+kt);	k2=setPp(q,p0-2.*kl+kz);	k3=setPp(q,p0-2.*kl-kt);
			k4=setPp(q,p0-2.*kl-kz);	k5=setPp(q,p0-1.5*kl);
			trig_plot(n1,k5,k1);	trig_plot(n1,k5,k2);
			trig_plot(n1,k5,k3);	trig_plot(n1,k5,k4);	break;
		case 'V':
			k1=setPp(q,p0+2.*kl+kt);	k2=setPp(q,p0+2.*kl+kz);	k3=setPp(q,p0+2.*kl-kt);
			k4=setPp(q,p0+2.*kl-kz);	k5=setPp(q,p0+1.5*kl);
			trig_plot(n1,k5,k1);	trig_plot(n1,k5,k2);
			trig_plot(n1,k5,k3);	trig_plot(n1,k5,k4);	break;
		case 'O':	// let draw icosahedron
		{
			long k0,k9,k10,k11;
			double t = 0.5*(1+sqrt(5.));
			k0 = setPp(q,p0+ll*mglPoint(-1, t, 0));	k1 = setPp(q,p0+ll*mglPoint( 1, t, 0));
			k2 = setPp(q,p0+ll*mglPoint(-1,-t, 0));	k3 = setPp(q,p0+ll*mglPoint( 1,-t, 0));
			k4 = setPp(q,p0+ll*mglPoint( 0,-1, t));	k5 = setPp(q,p0+ll*mglPoint( 0, 1, t));
			k6 = setPp(q,p0+ll*mglPoint( 0,-1,-t));	k7 = setPp(q,p0+ll*mglPoint( 0, 1,-t));
			k8 = setPp(q,p0+ll*mglPoint(-1, 0, t));	k9 = setPp(q,p0+ll*mglPoint( 1, 0, t));
			k10= setPp(q,p0+ll*mglPoint(-1, 0,-t));	k11= setPp(q,p0+ll*mglPoint( 1, 0,-t));

			trig_plot(k0,k11,k5);	trig_plot(k0,k1,k5);		trig_plot(k0,k1,k7);
			trig_plot(k0,k10,k7);	trig_plot(k0,k11,k10);
			trig_plot(k1,k5,k9);		trig_plot(k5,k11,k4);	trig_plot(k10,k11,k2);
			trig_plot(k10,k6,k7);	trig_plot(k7,k1,k8);
			trig_plot(k3,k9,k4);		trig_plot(k3,k4,k2);		trig_plot(k3,k2,k6);
			trig_plot(k3,k6,k8);		trig_plot(k3,k8,k9);
			trig_plot(k4,k9,k5);		trig_plot(k2,k4,k11);	trig_plot(k6,k2,k10);
			trig_plot(k8,k6,k7);		trig_plot(k9,k8,k1);		break;
		}
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::Legend(const std::vector<mglText> &leg, mreal x, mreal y, const char *font, const char *opt)
{
	long n=leg.size(), iw, ih;
	if(n<1)	{	SetWarn(mglWarnLeg,"Legend");	return;	}
	mreal ll = SaveState(opt);	if(mgl_isnan(ll))	ll=0.1;
	static int cgid=1;	StartGroup("Legend",cgid++);
	if(ll<=0 || mgl_isnan(ll))	ll=0.1;
	ll *=font_factor;
	mreal size = 0.8*FontSize;
	// setup font and parse absolute coordinates
	if(!font)	font="#";
	char *pA, *ff = new char[strlen(font)+3];
	strcpy(ff,font);	strcat(ff,":L");	Push();
	if((pA=strchr(ff,'A')))
	{	*pA = ' ';	InPlot(0,1,0,1,false);	iw=B1.b[0];	ih=B1.b[4];	}
	else	{	iw=B1.b[0]/B1.pf;	ih=B1.b[4]/B1.pf;	}
	// find sizes
	mreal h=TextHeight(font,size);
	mreal dx = 0.03*iw, dy = 0.03*ih, w=0, t, sp=TextWidth(" ",font,size);
	register long i,j;
	for(i=0;i<n;i++)		// find text length
	{
		t = TextWidth(leg[i].text.c_str(),font,size)+sp;
		if(leg[i].stl.empty())	t -= ll;
		w = w>t ? w:t;
	}
	w += ll+0.01*iw;	// add space for lines
	j = long((ih*0.95)/h);
	long ncol = 1+(n-1)/j, nrow = (n+ncol-1)/ncol;
	x = x*(iw-w*ncol-2*dx)+B.x-iw/2+dx;	// TODO bypass very long legends
	y = y*(ih-h*nrow-2*dy)+B.y-ih/2+dy;
	// draw it
	long k1=0,k2=0,k3=0,k4=0;
	mglPoint p,q=mglPoint(NAN,NAN);

	for(i=0;ff[i] && ff[i]!=':';i++)	if(strchr(MGL_COLORS,ff[i]))
	{

		if(k1 && k2)	{	k3=ff[i];	k4++;	}
		if(k1 && !k2)	{	k2=ff[i];	k4++;	}
		if(!k1)	{	k1=ff[i];	k4++;	}
	}
	if(k4==2)	k2=0;
	if(k4==1)	k1=k2=0;
	mreal c1=AddTexture(char(k1?k1:'w')), c2=AddTexture(char(k2?k2:'k'));
	if((Flag&3)==2)	{	mreal cc=c1;	c2=c1;	c2=cc;	}

	if(strchr(ff,'#'))	// draw bounding box
	{
		SetPenPal("k-");
		k1=AddPnt(mglPoint(x,y,Depth/1.00001),c1,q,-1,0);
		k2=AddPnt(mglPoint(x+w*ncol,y,Depth/1.00001),c1,q,-1,0);
		k3=AddPnt(mglPoint(x,y+h*nrow,Depth/1.00001),c1,q,-1,0);
		k4=AddPnt(mglPoint(x+w*ncol,y+h*nrow,Depth/1.00001),c1,q,-1,0);
		quad_plot(k1,k2,k3,k4);
		k1=CopyNtoC(k1,c2);	k2=CopyNtoC(k2,c2);
		k3=CopyNtoC(k3,c2);	k4=CopyNtoC(k4,c2);
		line_plot(k1,k2);	line_plot(k2,k4);
		line_plot(k4,k3);	line_plot(k3,k1);
	}
	for(i=0;i<n;i++)	// draw lines and legend
	{
		register long iy=nrow-(i%nrow)-1,ix=i/nrow;
		char m=SetPenPal(leg[i].stl.c_str());
		k1=AddPnt(mglPoint(x+ix*w+0.1*ll,y+iy*h+0.45*h,Depth),CDef,q,-1,0);
		k2=AddPnt(mglPoint(x+ix*w+0.9*ll,y+iy*h+0.45*h,Depth),CDef,q,-1,0);	pPos=0;
		if(!leg[i].stl.empty())	line_plot(k1,k2);
		if(m)	for(j=0;j<LegendMarks;j++)
		{
			p = mglPoint(x+ix*w+0.1f*ll + (j+1)*0.8f*ll/(1.+LegendMarks),y+iy*h+0.45*h,Depth);
			mark_plot(AddPnt(p,CDef,q,-1,0),m);
		}
		p = mglPoint(x+ix*w+((!leg[i].stl.empty())?ll:0.01*iw), y+iy*h+0.15*h, Depth);
		text_plot(AddPnt(p,-1,q,-1,0), leg[i].text.c_str(), ff, size);
	}
	Pop();	EndGroup();	delete []ff;
}
//-----------------------------------------------------------------------------
void mglCanvas::Table(mreal x, mreal y, HCDT val, const wchar_t *text, const char *frm, const char *opt)
{
//	if(x>=1) 	{	SetWarn(mglWarnSpc,"Table");	return;	}
	long i,j,m=val->GetNy(),n=val->GetNx();
//	mreal pos=SaveState(opt);
	mreal vw = SaveState(opt);
	static int cgid=1;	StartGroup("Table",cgid++);
	bool grid = mglchr(frm,'#'), eqd = mglchr(frm,'='), lim = mglchr(frm,'|');
	if(mgl_isnan(vw))	vw=1;	else 	lim = true;
	if(!text)	text=L"";
	if(x<0)	x=0; 	if(y<0)	y=0; 	if(y>1)	y=1;
//	if(vw>1-x)	vw=1-x;

	wchar_t *buf = new wchar_t[m*32], sng[32];
	std::vector<std::wstring> str;
	for(i=0;i<n;i++)		// prepare list of strings first
	{
		*buf=0;
		for(j=0;j+1<m;j++)
		{
			mglprintf(sng,32,L"%.3g\n",val->v(i,j));
			wcscat(buf,sng);
		}
		mglprintf(sng,32,L"%.3g",val->v(i,m-1));
		wcscat(buf,sng);		str.push_back(buf);
	}
	delete []buf;

	mreal sp=2*TextWidth(" ",frm,-1), w=sp+TextWidth(text,frm,-1), w1=0, ww, h;
	for(i=0;i<n;i++)		// find width for given font size
	{
		ww = TextWidth(str[i].c_str(),frm,-1)+sp;
		w1 = w1<ww?ww:w1;
		if(!eqd)	w += ww;
	}
	if(eqd)	w += n*w1;
	// reduce font size if table have to be inside inplot
	mreal fsize=FontSize;
	if(lim && w>vw*inW)
	{	h=vw*inW/w;	SetFontSize(-h); 	w*=h; 	w1*=h;	sp*=h;	}
	h = TextHeight(frm,-1);	// now we can determine text height

	x = x*(inW-w)+B.x-inW/2;
	y = y*(inH-h*m)+B.y-inH/2;

	mglPoint p,q=mglPoint(NAN,NAN);
	long k1,k2;
	mreal xx,yy;
	if(grid)	// draw bounding box
	{
		SetPenPal("k-");
		k1=AddPnt(mglPoint(x,y,Depth),-1,q,-1,0);
		k2=AddPnt(mglPoint(x,y+m*h,Depth),-1,q,-1,0);
		line_plot(k1,k2);
		ww = TextWidth(text,frm,-1)+sp;
		k1=AddPnt(mglPoint(x+ww,y,Depth),-1,q,-1,0);
		k2=AddPnt(mglPoint(x+ww,y+m*h,Depth),-1,q,-1,0);
		line_plot(k1,k2);
		for(i=0,xx=x+ww,yy=y;i<n;i++)
		{
			xx += eqd ? w1:(TextWidth(str[i].c_str(),frm,-1)+sp);
			k1=AddPnt(mglPoint(xx,yy,Depth),-1,q,-1,0);
			k2=AddPnt(mglPoint(xx,yy+m*h,Depth),-1,q,-1,0);
			line_plot(k1,k2);
		}
		for(i=0,xx=x,yy=y;i<=m;i++)
		{
			k1=AddPnt(mglPoint(xx,yy,Depth),-1,q,-1,0);
			k2=AddPnt(mglPoint(xx+w,yy,Depth),-1,q,-1,0);
			line_plot(k1,k2);	yy += h;
		}
	}
	int align;	mglGetStyle(frm, 0, &align);
	ww = TextWidth(text,frm,-1)+sp;
	k1=AddPnt(mglPoint(x+ww*align/2.,y+h*(m-1),Depth),-1,q,-1,0);
	text_plot(k1,text,frm);
	for(i=0,xx=x+ww,yy=y+h*(m-1);i<n;i++)	// draw lines and legend
	{
		ww = eqd ? w1:(TextWidth(str[i].c_str(),frm,-1)+sp);
		k1=AddPnt(mglPoint(xx+ww*align/2.,yy,Depth),-1,q,-1,0);
		text_plot(k1,str[i].c_str(),frm);	xx += ww;
	}
	FontSize = fsize;	EndGroup();
}
//-----------------------------------------------------------------------------
void mglCanvas::Title(const char *title,const char *stl,mreal size)
{
	if(!title)	title="";
	size_t s = strlen(title)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,title,s);
	Title(wcs, stl,size);
	delete []wcs;
}
//-----------------------------------------------------------------------------
void mglCanvas::Title(const wchar_t *title,const char *stl,mreal size)
{
	mreal s = size>0 ? size/FontSize:-size, h=TextHeight(stl,size)*s/2;
	if(h>=inH)	{	SetWarn(mglWarnSpc,"Title");	return;	}
	bool box=mglchr(stl,'#');
	int align;	mglGetStyle(stl,0,&align);	align = align&3;
	mreal y=inY+inH-h;
	mglPoint p(inX + inW*align/2.,y,Depth),q(NAN);
	if(title)	text_plot(AddPnt(p,-1,q,-1,0),title,stl,size);
	if(box)	//	draw boungind box
	{
		mreal c1=AddTexture('w'), c2=AddTexture('k');
		if((Flag&3)==2)	{	mreal cc=c1;	c2=c1;	c2=cc;	}
		long k1,k2,k3,k4;
		k1=AddPnt(mglPoint(inX,y,Depth),c1,q,-1,0);
		k2=AddPnt(mglPoint(inX+inW,y,Depth),c1,q,-1,0);
		k3=AddPnt(mglPoint(inX,y+h,Depth),c1,q,-1,0);
		k4=AddPnt(mglPoint(inX+inW,y+h,Depth),c1,q,-1,0);
		quad_plot(k1,k2,k3,k4);
		k1=CopyNtoC(k1,c2);	k2=CopyNtoC(k2,c2);
		k3=CopyNtoC(k3,c2);	k4=CopyNtoC(k4,c2);
		line_plot(k1,k2);	line_plot(k2,k4);
		line_plot(k4,k3);	line_plot(k3,k1);
	}
	B1.y -= h/2;	B1.b[4] -= h;	B=B1;
	inH-=h;	font_factor = B.b[0] < B.b[4] ? B.b[0] : B.b[4];
}
//-----------------------------------------------------------------------------
void mglCanvas::StartAutoGroup (const char *lbl)
{
	static int id=1;
	if(lbl==NULL)	{	id=1;	grp_counter=0;	return;	}
	grp_counter++;
	if(grp_counter>1)	return;	// do nothing in "subgroups"
	if(ObjId<0)	{	ObjId = -id;	id++;	}
	register size_t len = Grp.size();
	if(ObjId>=0 && len>0 && ObjId!=Grp[len-1].Id)
	{	MGL_PUSH(Grp,mglGroup(lbl,ObjId),mutexGrp);	}
	else if(ObjId<0)
	{	MGL_PUSH(Grp,mglGroup(lbl,ObjId),mutexGrp);	}
}
//-----------------------------------------------------------------------------
void mglCanvas::EndGroup()
{
	LoadState();
	if(Quality&4)
	{
		Pnt.clear();		Prm.clear();		Ptx.clear();
		Glf.clear();		Act.clear(); 	Grp.clear();
	}
	if(grp_counter>0)	grp_counter--;
}
//-----------------------------------------------------------------------------
int mglCanvas::IsActive(int xs, int ys,int &n)
{
	long i, h = (Width>Height ? Height:Width)/100;
	for(i=0;i<(long)Act.size();i++)
	{
		const mglActivePos &p=Act[i];
		if(abs(xs-p.x)<=h && abs(ys-p.y)<=h)
		{	n=p.n;	return p.id;		}
	}
	n=-1;	return GetObjId(xs,ys);
}
//-----------------------------------------------------------------------------
