/***************************************************************************
 * mgl_ab.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#include "mgl/mgl_ab.h"
#define imax(a,b)	(a)>(b) ? (a) : (b)
#define imin(a,b)	(a)<(b) ? (a) : (b)

char *mgl_strdup(const char *s);
//-----------------------------------------------------------------------------
void strtrim_mgl(char *str);
void wcstrim_mgl(wchar_t *str);
int _mgl_tick_ext(mreal a, mreal b, wchar_t s[32], mreal &v);
void _mgl_tick_text(mreal z, mreal z0, mreal d, mreal v, int kind, wchar_t str[64]);
//-----------------------------------------------------------------------------
mglGraphAB::mglGraphAB(int w,int h) : mglGraph()
{
	G = 0;	UseLight = false;	st_pos=-1;
	memset(stack,0,MGL_STACK_ENTRY*13*sizeof(mreal));
	SetSize(w,h);
	AutoClf=true;	Delay = 100;
	NoAutoFactor = false;	ShowMousePos = true;
	BDef[0] = BDef[1] = BDef[2] = BDef[3] = 255;
	DefaultPlotParam();
}
//-----------------------------------------------------------------------------
mglGraphAB::~mglGraphAB()
{	if(G)	{	delete []G;	delete []G4;	}	}
//-----------------------------------------------------------------------------
void mglGraphAB::Pop()
{
	if(st_pos<0)	return;
	mreal *c=stack+13*st_pos;
	xPos=c[0];	yPos=c[1];	zPos=c[2];	Persp=c[3];
	memcpy(B,c+4,9*sizeof(mreal));
	st_pos--;
}
//-----------------------------------------------------------------------------
void mglGraphAB::Push()
{
	st_pos = st_pos<9 ? st_pos+1:9;
	if(st_pos<0)	st_pos=0;
	mreal *c=stack+13*st_pos;
	c[0]=xPos;	c[1]=yPos;	c[2]=zPos;	c[3]=Persp;
	memcpy(c+4,B,9*sizeof(mreal));
}
//-----------------------------------------------------------------------------
void mglGraphAB::SetPosScale(mreal xp, mreal yp, mreal zp, mreal scl)
{
	memset(B,0,9*sizeof(mreal));
	B[0] = B[4] = B[8] = scl;
	xPos=xp;	yPos=yp;	zPos=zp;
}
//-----------------------------------------------------------------------------
const unsigned char *mglGraphAB::GetBits()
{	if(!Finished)	Finish();	return G;	}
//-----------------------------------------------------------------------------
const unsigned char *mglGraphAB::GetRGBA()
{	if(!Finished)	Finish();	return G4;	}
//-----------------------------------------------------------------------------
void mglGraphAB::DefColor(mglColor c, mreal alpha)
{
	if(alpha<0 || alpha>1)	alpha = AlphaDef;
	CDef[3] = Transparent ? alpha : 1;
	if(c.Valid())
	{	CDef[0] = c.r;	CDef[1] = c.g;	CDef[2] = c.b;	}
}
//-----------------------------------------------------------------------------
void mglGraphAB::Pen(mglColor col, char style,mreal width)
{
	if(col.Valid())	DefColor(col,1);
	switch(style)
	{
	case '-': PDef = 0xffff;	break;
	case '|': PDef = 0xff00;	break;
	case ';': PDef = 0xf0f0;	break;
	case ':': PDef = 0x8888;	break;
	case 'j': PDef = 0xfe10;	break;
	case 'i': PDef = 0xe4e4;	break;
	case ' ': PDef = 0x0000;	break;
//	case 0:	  PDef = 0x0000;	break;
//	default:  PDef = 0xffff;
	}
	PenWidth = width;
}
//-----------------------------------------------------------------------------
void mglGraphAB::RotateN(mreal Tet,mreal x,mreal y,mreal z)
{
	mreal R[9],C[9],c=cos(Tet*M_PI/180),s=-sin(Tet*M_PI/180),r=1-c,n=sqrt(x*x+y*y+z*z);
	x/=n;	y/=n;	z/=n;
	C[0] = x*x*r+c;		C[1] = x*y*r-z*s;	C[2] = x*z*r+y*s;
	C[3] = x*y*r+z*s;	C[4] = y*y*r+c;		C[5] = y*z*r-x*s;
	C[6] = x*z*r-y*s;	C[7] = y*z*r+x*s;	C[8] = z*z*r+c;
	memcpy(R,B,9*sizeof(mreal));
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
		register mreal m = fabs(B[3]+B[4]+B[5]);
		m = m<fabs(B[3]+B[4]-B[5]) ? fabs(B[3]+B[4]-B[5]) : m;
		m = m<fabs(B[3]-B[4]+B[5]) ? fabs(B[3]-B[4]+B[5]) : m;
		m = m<fabs(B[3]-B[4]-B[5]) ? fabs(B[3]-B[4]-B[5]) : m;
		m /= B1[4];
		register mreal n = fabs(B[0]+B[1]+B[2]);
		n = n<fabs(B[0]+B[1]-B[2]) ? fabs(B[0]+B[1]-B[2]) : n;
		n = n<fabs(B[0]-B[1]+B[2]) ? fabs(B[0]-B[1]+B[2]) : n;
		n = n<fabs(B[0]-B[1]-B[2]) ? fabs(B[0]-B[1]-B[2]) : n;
		n /= B1[0];
		PlotFactor = 1.55+0.6147*(m<n ? (n-1):(m-1));
	}
}
//-----------------------------------------------------------------------------
void mglGraphAB::Perspective(mreal a)	// I'm too lazy for using 4*4 matrix
{
	Persp = fabs(a)/Depth;
}
//-----------------------------------------------------------------------------
void mglGraphAB::RestoreM()
{
	memcpy(B,BL,9*sizeof(mreal));
	xPos = BL[9];	yPos = BL[10];	zPos = BL[11];
	PlotFactor = BL[12];
}
//-----------------------------------------------------------------------------
void mglGraphAB::InPlot(mreal x1,mreal x2,mreal y1,mreal y2, bool rel)
{
	if(!rel)
	{
		memcpy(BL,B,9*sizeof(mreal));
		BL[9] = xPos;	BL[10] = yPos;	BL[11] = zPos;
		BL[12] = PlotFactor;
	}
	SelectPen("k-1");
	if(Width<=0 || Height<=0 || Depth<=0)	return;
	memset(B,0,9*sizeof(mreal));
	if(rel)
	{
		xPos = B1[9] + (x1+x2-1)/2*B1[0];
		yPos = B1[10]+ (y1+y2-1)/2*B1[4];
		B[0] = B1[0]*(x2-x1);	B[4] = B1[4]*(y2-y1);
		B[8] = sqrt(B[0]*B[4]);
		zPos = B1[11]+ (1.f-B[8]/(2*Depth))*B1[8];
	}
	else
	{
		B1[9] = xPos = (x1+x2)/2*Width;
		B1[10]= yPos = (y1+y2)/2*Height;
		B[0] = Width*(x2-x1);	B[4] = Height*(y2-y1);
		B[8] = sqrt(B[0]*B[4]);
		B1[11]= zPos = (1.f-B[8]/(2*Depth))*Depth;
		memcpy(B1,B,9*sizeof(mreal));
	}
	font_factor = B[0] < B[4] ? B[0] : B[4];
	PlotFactor = AutoPlotFactor ? 1.55 : 2;	// Automatically change plot factor !!!
	Persp = 0;
}
//-----------------------------------------------------------------------------
void mglGraphAB::Aspect(mreal Ax,mreal Ay,mreal Az)
{
	mreal a = fabs(Ax) > fabs(Ay) ? fabs(Ax) : fabs(Ay);
	a = a > fabs(Az) ? a : fabs(Az);
	if(a==0)	{	SetWarn(mglWarnZero,"Aspect");	return;	}
	Ax/=a;	Ay/=a;	Az/=a;
	B[0] *= Ax;		B[3] *= Ax;		B[6] *= Ax;
	B[1] *= Ay;		B[4] *= Ay;		B[7] *= Ay;
	B[2] *= Az;		B[5] *= Az;		B[8] *= Az;
}
//-----------------------------------------------------------------------------
void mglGraphAB::NormScale(mreal *p,long n)
{
	register long i;
	mreal x[3], *y;
	for(i=0;i<n;i++)
	{
		y = p+3*i;
		x[0] = (y[0]*B[0] + y[1]*B[1] + y[2]*B[2])/zoomx2;
		x[1] = (y[0]*B[3] + y[1]*B[4] + y[2]*B[5])/zoomy2;
		x[2] = (y[0]*B[6] + y[1]*B[7] + y[2]*B[8])/sqrt(zoomx2*zoomy2);
		memcpy(y,x,3*sizeof(mreal));
	}
}
//-----------------------------------------------------------------------------
void mglGraphAB::PostScale(mreal *p,long n)
{
	register mreal s3=2*PlotFactor;
	register long i;
	mreal x[3], *y;
	for(i=0;i<n;i++)
	{
		y = p+3*i;
		y[0] = y[0]/s3;		y[1] = y[1]/s3;		y[2] = y[2]/s3;
		x[0] = (xPos + y[0]*B[0] + y[1]*B[1] + y[2]*B[2] - zoomx1*Width)/zoomx2;
		x[1] = (yPos + y[0]*B[3] + y[1]*B[4] + y[2]*B[5] - zoomy1*Height)/zoomy2;
		x[2] = (zPos + y[0]*B[6] + y[1]*B[7] + y[2]*B[8])/sqrt(zoomx2*zoomy2);
		if(Persp)
		{
			register mreal d = (1-Persp*Depth/2)/(1-Persp*x[2]);
			x[0] = Width/2 + d*(x[0]-Width/2);
			x[1] = Height/2 + d*(x[1]-Height/2);
		}
		memcpy(y,x,3*sizeof(mreal));
	}
}
//-----------------------------------------------------------------------------
mglPoint mglGraphAB::CalcXYZ(int xs, int ys)
{
	mreal s3 = 2*PlotFactor, x, y, z;
	// NOTE: Perspective, transformation formulas and lists are not support just now !!! Also it use LAST InPlot parameters!!!
	ys = Height - ys;
	mreal xx = xs*zoomx2 - xPos + zoomx1*Width;
	mreal yy = ys*zoomy2 - yPos + zoomy1*Height;
	mreal d1=B[0]*B[4]-B[1]*B[3], d2=B[1]*B[5]-B[2]*B[4], d3=B[0]*B[5]-B[2]*B[3];
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
void mglGraphAB::LightScale()
{
	mreal *x;
	register mreal nn;
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
void mglGraphAB::Light(int n, bool enable)
{
	if(n<0 || n>9)	{	SetWarn(mglWarnLId);	return;	}
	nLight[n] = enable;
}
//-----------------------------------------------------------------------------
void mglGraphAB::Light(int n, mglPoint p, mglColor c, mreal br, bool inf)
{
	if(n<0 || n>9)	{	SetWarn(mglWarnLId);	return;	}
	nLight[n] = true;	aLight[n] = 3;	bLight[n] = br;	iLight[n] = inf;
	rLight[3*n] = p.x;	rLight[3*n+1] = p.y;	rLight[3*n+2] = p.z;
	cLight[3*n] = c.r;	cLight[3*n+1] = c.g;	cLight[3*n+2] = c.b;
}
//-----------------------------------------------------------------------------
int mglGraphAB::NewFrame()
{
	Clf();
	Identity();
	CurFrameId++;
	return CurFrameId-1;
}
//-----------------------------------------------------------------------------
mreal mglGraphAB::Putsw(mglPoint p,mglPoint n,const wchar_t *str,char font,mreal size)
{
	static int cgid=1;	StartGroup("PutswL",cgid++);
	mreal pp[6] = {p.x,p.y,p.z,p.x+n.x,p.y+n.y,p.z+n.z};
	Arrow1 = Arrow2 = '_';

	if(size<0)	size = -size*FontSize;
	f_size = size;	Push();
	mreal shift = 0.07, fsize=size/8.*font_factor;
	mreal x1=zoomx1, x2=zoomx2, y1=zoomy1, y2=zoomy2;
	if(font=='t')	shift = -0.07;

	shift *= fsize/2;
	if(ScalePuts)
	{
		bool tt = Cut;	Cut = true;
		ScalePoint(pp[0],pp[1],pp[2]);
		ScalePoint(pp[3],pp[4],pp[5]);
		Cut = tt;
	}
	zoomx1=zoomy1=0;	zoomx2=zoomy2=1;
	PostScale(pp,2);
	zoomx1=x1;	zoomx2=x2;	zoomy1=y1;	zoomy2=y2;	Persp=0;

	mreal ll=(pp[3]-pp[0])*(pp[3]-pp[0])+(pp[4]-pp[1])*(pp[4]-pp[1]);
	if(ll==0)	{	Pop();	EndGroup();	return 0;	}
	mreal tet = 180*atan2(pp[4]-pp[1],pp[3]-pp[0])/M_PI;
//	if(fabs(tet)>90)	tet+=180;
	memset(B,0,9*sizeof(mreal));
	B[0] = B[4] = B[8] = fsize;
	fscl = fsize;	ftet = -tet;
	NoAutoFactor=true;	RotateN(-tet,0,0,1);	NoAutoFactor=false;
	xPos = pp[0]+shift*(pp[4]-pp[1])/sqrt(ll) - B[1]*0.02f;
	yPos = pp[1]-shift*(pp[3]-pp[0])/sqrt(ll) - B[4]*0.02f;
	zPos = pp[2];

	fsize = fnt->Puts(str,"rL",0)*size/8.;
	Pop();		EndGroup();
	return fsize;
}
//-----------------------------------------------------------------------------
void mglGraphAB::Putsw(mglPoint p, const wchar_t *wcs, const char *font, mreal size, char dir, mreal sh)
{
	static int cgid=1;	StartGroup("Putsw",cgid++);
	bool upside = ( (((_sx==-1) ^ (Org.y==Max.y || Org.z==Max.z)) && (dir=='x' || dir=='X')) ||
					(((_sy==-1) ^ (Org.x==Max.x || Org.z==Max.z)) && (dir=='y' || dir=='Y')) ||
					(((_st==-1) ^ (Org.x==0 || Org.z==1)) && (dir=='t' || dir=='T')) ||
					(((_sz==-1) ^ (Org.y==Max.y || Org.x==Max.x)) && (dir=='z' || dir=='Z')) );
	mreal pp[6] = {p.x,p.y,p.z,p.x,p.y,p.z};
	Arrow1 = Arrow2 = '_';
	char *f, *font1;
	if(font)	{	font1=new char[strlen(font)+2];	strcpy(font1,font);	}
	else	{	font1=new char[strlen(FontDef)+2];	strcpy(font1,FontDef);	}
	char col=TranspType!=2 ? 'k':'w';
	f = strchr(font1,':');	if(f)	{	f[0]=0;	col=f[1];	}
//	stl[1] = col;	SelectPen(stl);

	if(size<0)	size = -size*FontSize;
	f_size = size;	Push();
	mreal shift = (sh/10+0.2)*2/PlotFactor, fsize=size/8.*font_factor;
	mreal x1=zoomx1, x2=zoomx2, y1=zoomy1, y2=zoomy2;
	zoomx1=zoomy1=0;	zoomx2=zoomy2=1;

	if(strchr("xyzt",dir))	shift = (sh/10+0.1)*2/PlotFactor;
	switch(dir)
	{
	case 'x':
	case 'X':
		pp[3] += (Max.x - Min.x)/100;	break;
	case 'y':
	case 'Y':
		if(TernAxis)	upside = !upside;
		pp[4] += (Max.y - Min.y)/10;	break;
	case 'z':
	case 'Z':
		if(TernAxis)	upside = !upside;
		pp[4] -= 0;
		pp[5] += (Max.z - Min.z)/100;	break;
	case 't':
	case 'T':
//		upside = !upside;
		pp[3] += (Max.x - Min.x)/100;
		pp[4] -= (Max.y - Min.y)/100;
		pp[5] += 0;	break;
	}
	if(upside)	shift = -shift;
	shift *= fsize/2;

	if(ScalePuts)
	{
		bool tt = Cut;	Cut = true;
		ScalePoint(pp[0],pp[1],pp[2]);
		ScalePoint(pp[3],pp[4],pp[5]);
		Cut = tt;
	}
	PostScale(pp,2);	Persp=0;

	if(dir==0)
	{
		xPos = pp[0];	yPos = pp[1];	zPos = pp[2];
		memset(B,0,9*sizeof(mreal));
		B[0] = B[4] = B[8] = fsize;
		fscl = fsize;	ftet = 0;
	}
	else if(RotatedText)
	{
		if(pp[4]==pp[1] && pp[3]==pp[0])
		{	zoomx1=x1;	zoomx2=x2;	zoomy1=y1;	zoomy2=y2;
			delete []font1;	Pop();	EndGroup();	return;	}
		mreal ll=(pp[3]-pp[0])*(pp[3]-pp[0])+(pp[4]-pp[1])*(pp[4]-pp[1]);
		mreal tet = 180*atan2(pp[4]-pp[1],pp[3]-pp[0])/M_PI;
		if(fabs(tet)>90)	tet+=180;
		memset(B,0,9*sizeof(mreal));
		B[0] = B[4] = B[8] = fsize;
		fscl = fsize;	ftet = -tet;
		NoAutoFactor=true;	RotateN(-tet,0,0,1);	NoAutoFactor=false;
		mreal ss = (pp[3]>pp[0] || tet==-90) ? 1 : -1;
		xPos = pp[0]+shift*ss*(pp[4]-pp[1])/sqrt(ll) - B[1]*0.02f;
		yPos = pp[1]-shift*ss*(pp[3]-pp[0])/sqrt(ll) - B[4]*0.02f;
		zPos = pp[2];
	}
	else
	{
		if(pp[4]==pp[1] && pp[3]==pp[0])
		{	zoomx1=x1;	zoomx2=x2;	zoomy1=y1;	zoomy2=y2;
			delete []font1;	Pop();	EndGroup();	return;	}
		mreal ll=(pp[3]-pp[0])*(pp[3]-pp[0])+(pp[4]-pp[1])*(pp[4]-pp[1]);
		mreal tet = atan2(pp[4]-pp[1],pp[3]-pp[0]);
//		if(fabs(tet)>90)	tet+=180;
		memset(B,0,9*sizeof(mreal));	B[0] = B[4] = B[8] = fsize;
		mreal ss = (pp[3]>pp[0] || tet==-M_PI/2) ? 1 : -1;
		fscl = fsize;	ftet = 0;
		xPos = pp[0]+shift*ss*(pp[4]-pp[1])/sqrt(ll);
		yPos = pp[1]-shift*ss*(pp[3]-pp[0])/sqrt(ll);
		zPos = pp[2];

		if(!strchr(font1,'R') && !strchr(font1,'C') && !strchr(font1,'L'))
		{
			char ch[2]="C";
//			if((pp[4]-pp[1])*(pp[3]-pp[0])>0) ch[0]=upside?'R':'L';	else ch[0]=upside?'L':'R';
			if(fabs(sin(tet))>0.1)	ch[0] = shift*ss*sin(tet)>0 ? 'L':'R';
			strcat(font1,ch);
		}
	}
	zoomx1=x1;	zoomx2=x2;	zoomy1=y1;	zoomy2=y2;
	fnt->Puts(wcs,font1,col);
	Pop();	delete []font1;	EndGroup();
}
//-----------------------------------------------------------------------------
void mglGraphAB::Legend(int n, wchar_t **text,char **style, mreal x, mreal y,
					const char *font, mreal size, mreal llen)
{
	if(n<1)	{	SetWarn(mglWarnLeg);	return;	}
	x = 2*x-1;	y = 2*y-1;
	if(isnan(llen))	llen=0.1;
	static int cgid=1;	StartGroup("Legend",cgid++);
	mreal pp[15], r=GetRatio(), rh, rw, s3=PlotFactor;
	if(size<=0)	size = -size*FontSize;
	if(!font || !(*font))	font="L";
	llen *= 1.5;

	rh=(r<1?r:1.)*size/6.;	rw=(r>1?1./r:1.)*size/8.;
//	rh=size/6.;	rw=size/24.;
	mreal w=0, h=fnt->Height(font)*rh, j;
	register long i;
	for(i=0;i<n;i++)		// find text length
	{
		j = fnt->Width(text[i],font)*rw;
		if(style[i][0]==0)	j -= llen;
		w = w>j ? w:j;
	}
	w = (w + llen*1.1f);	// add space for lines

	Push();	memcpy(B,B1,9*sizeof(mreal));
	if(LegendBox)	// draw bounding box
	{
		pp[2] = pp[5] = pp[8] = pp[11] = pp[14] = s3-0.01;
		pp[0] = pp[9] = pp[12] = x;		pp[3] = pp[6] = x+w;
		pp[1] = pp[4] = pp[13] = y-0.*h;		pp[7] = pp[10] = y+h*n;
//		for(i=0;i<5;i++)	ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
		SelectPen(TranspType!=2 ? "k-1":"w-1");
		curv_plot(5,pp,0);	// bounding rectangle
		pp[2] = pp[5] = pp[8] = pp[11] = s3-0.01;
		pp[0] = pp[6] = x;			pp[3] = pp[9] = x+w;
		pp[1] = pp[4] = y-0.*h;		pp[7] = pp[10] = y+h*n;
		DefColor(mglColor(1,1,1),1);
//		for(i=0;i<4;i++)	ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
		surf_plot(2,2,pp,0,0);		// white rectangle below it
	}
	ScalePuts = false;
	for(i=0;i<n;i++)	// draw lines and legend
	{
		char m=SelectPen(style[i]);
		pp[2] = pp[5] = pp[8] = s3;
		pp[1] = pp[4] = pp[7] = y+i*h+0.45f*h;
		pp[0] = x+0.1f*llen;	pp[3] = x+0.9f*llen;	pp[6] = x+0.5f*llen;
//		ScalePoint(pp[0],pp[1],pp[2]);	ScalePoint(pp[3],pp[4],pp[5]);	ScalePoint(pp[6],pp[7],pp[8]);

		curv_plot(2,pp,0);
		if(m)	Mark(pp[6],pp[7],pp[8],m);
		SelectPen(TranspType!=2 ? "k-1":"w-1");
		Putsw(mglPoint(x+(style[i][0]!=0?llen:0), y+i*h+0.3f*h, s3), text[i], font, size);
	}
	ScalePuts = true;
	Pop();	EndGroup();
}
//-----------------------------------------------------------------------------
void mglGraphAB::Colorbar(int where, mreal x, mreal y, mreal w, mreal h)
{
	static int cgid=1;	StartGroup("Colorbar",cgid++);
	register long i;
	mreal *pp,*cc,d,s3=PlotFactor,ss=s3*0.9;
	mglColor c;

	pp = new mreal[6*256];	cc = new mreal[8*256];
	x = 2*x-1;	y = 2*y-1;
	for(i=0;i<255;i++)
	{
		d = 2*i/254.-1;
		pp[6*i+0] = pp[6*i+3] = (ss*d+s3)*w+x*s3;
		pp[6*i+1] = pp[6*i+4] = (ss*d+s3)*h+y*s3;
		pp[6*i+2] = pp[6*i+5] = s3;
		switch(where)
		{
		case 1:	pp[6*i]  = x*s3;	pp[6*i+3] = (x+0.1*w)*s3;	break;
		case 2:	pp[6*i+1]= (y-0.1*h)*s3;	pp[6*i+4] = y*s3;	break;
		case 3:	pp[6*i+1]= y*s3;	pp[6*i+4] = (y+0.1*h)*s3;	break;
		default:pp[6*i]  = (x-0.1*w)*s3;	pp[6*i+3] = x*s3;	break;
		}
		c = GetC(d,false);
		cc[8*i+0] = cc[8*i+4] = c.r;
		cc[8*i+1] = cc[8*i+5] = c.g;
		cc[8*i+2] = cc[8*i+6] = c.b;
		cc[8*i+3] = cc[8*i+7] = 1;
	}
	Push();	memcpy(B,B1,9*sizeof(mreal));
	bool ll = UseLight;				UseLight = false;
	surf_plot(2, 255, pp, cc, 0);	UseLight = ll;
	delete []pp;	delete []cc;

	const char *a="rC";
	if(where==0)	a = "rR";
	if(where==1)	a = "rL";
	if(where==2)	a = "rDC";
	mglPoint p;
	ScalePuts = false;
	SelectPen(TranspType!=2 ? "k-1":"w-1");

	mreal v=0,t;
	int kind=0;
	wchar_t s[32]=L"", str[64];
	if(!ctt && TuneTicks) kind = _mgl_tick_ext(Cmax, Cmin, s, v);

	for(i=0;i<5;i++)
	{
		d = i*0.5-1;	p.z = s3+1;
		p.x = (ss*d+s3)*w+x*s3;
		p.y = (ss*d+s3)*h+y*s3;
		switch(where)
		{
		case 1:	p.x = (x+0.13*w)*s3;	break;
		case 2:	p.y = (y-0.13*h)*s3;	break;
		case 3:	p.y = (y+0.13*h)*s3;	break;
		default:p.x = (x-0.13*w)*s3;	break;
		}
		t = Cmin+i*(Cmax-Cmin)/4;
		if(ctt)	swprintf(str, 64, ctt, t);
		else	_mgl_tick_text(t,Cmin,(Cmax-Cmin)/100,v,kind,str);
		wcstrim_mgl(str);
		Putsw(p,str,a,FontSize);
	}
	switch(where)
	{
	case 1:	p = mglPoint((x+0.15*w)*s3, (y+1.75*h)*s3);	break;
	case 2:	p = mglPoint((x+1.75*w)*s3, (y-0.15*h)*s3);	break;
	case 3:	p = mglPoint((x+1.75*w)*s3, (y+0.15*h)*s3);	break;
	default:p = mglPoint((x-0.15*w)*s3, (y+1.75*h)*s3);	break;
	}
	if(kind&2)	Putsw(p,s,a,FontSize);
	ScalePuts = true;
	Pop();	EndGroup();
}
//-----------------------------------------------------------------------------
void mglGraphAB::SetSize(int w,int h)
{
	if(w<=0 || h<=0)	{	SetWarn(mglWarnSize);	return;	}
	Width = w;	Height = h;	Depth = long(sqrt(mreal(w*h)));
	zPos = 0;//long(2*Depth*sqrt(3.));
	if(G)	{	delete []G;	delete []G4;	}
	G = new unsigned char[w*h*3];
	G4= new unsigned char[w*h*4];
	Clf();
	InPlot(0,1,0,1);	Persp = 0;
}
//-----------------------------------------------------------------------------
void mglGraphAB::WriteSVG(const char *fname, const char *descr)
{
	time_t now;
	time(&now);
	char *pname = new char[strlen(fname)+5];
	sprintf(pname,"%s.png",fname);
	WritePNG(pname,descr);

	FILE *fp = fopen(fname,"wt");
	if(!fp)		{	SetWarn(mglWarnOpen,fname);	return;	}
	fprintf(fp,"<?xml version=\"1.0\" standalone=\"no\"?>\n");
	fprintf(fp,"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 20000303 Stylable//EN\" \"http://www.w3.org/TR/2000/03/WD-SVG-20000303/DTD/svg-20000303-stylable.dtd\">\n");
	fprintf(fp,"<svg width=\"%d\" height=\"%d\">\n",Width,Height);

	fprintf(fp,"<!--Creator: MathGL library-->\n");
	fprintf(fp,"<!--Title: %s-->\n<!--CreationDate: %s-->\n\n",descr?descr:fname,ctime(&now));
	fprintf(fp,"<g fill=\"none\" stroke=\"none\" stroke-width=\"0.5\">\n");

	fprintf(fp,"<image x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" xlink:href=\"%s\"/>\n", Width, Height, pname);

	fprintf(fp,"</g></svg>");
	delete []pname;
}
//-----------------------------------------------------------------------------
void mglGraphAB::arrow_plot(mreal *p1,mreal *p2,char st)
{
	if(!strchr("AVKSDTIO",st))	return;
	mreal lx=p1[0]-p2[0], ly=p1[1]-p2[1], ll, kx,ky;
	mreal pp[12];
	ll = hypot(lx,ly)/(ArrowSize*0.35*font_factor);		if(ll==0)	return;
	lx /= ll;	ly /= ll;	kx = ly;	ky = -lx;
	pp[2] = pp[5] = pp[8] = pp[11] = p1[2];

	bool ul = UseLight;		UseLight = false;
	switch(st)
	{
	case 'I':
		pp[0] = p1[0]+kx;	pp[1] = p1[1]+ky;
		pp[3] = p1[0]-kx;	pp[4] = p1[1]-ky;
		line_plot(pp,pp+3,CDef,CDef);	break;
	case 'D':
		pp[0] = p1[0]+kx;	pp[1] = p1[1]+ky;
		pp[3] = p1[0]+lx;	pp[4] = p1[1]+ly;
		pp[6] = p1[0]-kx;	pp[7] = p1[1]-ky;
		pp[9] = p1[0]-lx;	pp[10] = p1[1]-ly;
		quad_plot(pp,pp+3,pp+9,pp+6,CDef,CDef,CDef,CDef);	break;
	case 'S':
		pp[0] = p1[0]+kx-lx;	pp[1] = p1[1]+ky-ly;
		pp[3] = p1[0]-kx-lx;	pp[4] = p1[1]-ky-ly;
		pp[6] = p1[0]-kx+lx;	pp[7] = p1[1]-ky+ly;
		pp[9] = p1[0]+kx+lx;	pp[10] = p1[1]+ky+ly;
		quad_plot(pp,pp+3,pp+9,pp+6,CDef,CDef,CDef,CDef);	break;
	case 'T':
		pp[0] = p1[0]-kx-lx;	pp[1] = p1[1]-ky-ly;
		pp[3] = p1[0]+kx-lx;	pp[4] = p1[1]+ky-ly;
		pp[6] = p1[0]+lx;		pp[7] = p1[1]+ly;
		trig_plot(pp,pp+3,pp+6,CDef,CDef,CDef);	break;
	case 'A':
		pp[0] = p1[0];			pp[1] = p1[1];
		pp[3] = p1[0]-kx-2*lx;	pp[4] = p1[1]-ky-2*ly;
		pp[6] = p1[0]-1.5*lx;	pp[7] = p1[1]-1.5*ly;
		pp[9] = p1[0]+kx-2*lx;	pp[10] = p1[1]+ky-2*ly;
		quad_plot(pp,pp+3,pp+9,pp+6,CDef,CDef,CDef,CDef);	break;
	case 'K':
		pp[0] = p1[0];			pp[1] = p1[1];
		pp[3] = p1[0]-kx-2*lx;	pp[4] = p1[1]-ky-2*ly;
		pp[6] = p1[0]-1.5*lx;	pp[7] = p1[1]-1.5*ly;
		pp[9] = p1[0]+kx-2*lx;	pp[10] = p1[1]+ky-2*ly;
		quad_plot(pp,pp+3,pp+9,pp+6,CDef,CDef,CDef,CDef);
		pp[0] = p1[0]+kx;	pp[1] = p1[1]+ky;
		pp[3] = p1[0]-kx;	pp[4] = p1[1]-ky;
		line_plot(pp,pp+3,CDef,CDef);	break;
	case 'V':
		pp[0] = p1[0];			pp[1] = p1[1];
		pp[3] = p1[0]-kx+2*lx;	pp[4] = p1[1]-ky+2*ly;
		pp[6] = p1[0]+1.5*lx;	pp[7] = p1[1]+1.5*ly;
		pp[9] = p1[0]+kx+2*lx;	pp[10] = p1[1]+ky+2*ly;
		quad_plot(pp,pp+3,pp+9,pp+6,CDef,CDef,CDef,CDef);	break;
	case 'O':
	{
		pp[0] = p1[0];	pp[1] = p1[1];
		double t;
		for(int i=0;i<16;i++)
		{
			t = M_PI*i/8.;
			pp[3] = p1[0] + kx*sin(t) + lx*cos(t);
			pp[4] = p1[1] + ky*sin(t) + ly*cos(t);
			t = M_PI*(i+1)/8.;
			pp[6] = p1[0] + kx*sin(t) + lx*cos(t);
			pp[7] = p1[1] + ky*sin(t) + ly*cos(t);
			trig_plot(pp,pp+3,pp+6,CDef,CDef,CDef);
		}
		break;
	}
/*		mreal ss = MarkSize;	MarkSize = ArrowSize;
		mark_plot(p1,'O');
		MarkSize = ss;*/
	}
	UseLight = ul;
}
//-----------------------------------------------------------------------------
unsigned char **mglGraphAB::GetRGBLines(long &w, long &h, unsigned char *&f, bool alpha)
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
void mglGraphAB::SetFontSizePT(mreal pt, int dpi)
{	FontSize = pt*Height*0.0675/dpi;	}
//-----------------------------------------------------------------------------
void mglGraphAB::FindOptOrg(mreal ax[3], mreal ay[3], mreal az[3])
{
	static mreal px[3]={0,0,0}, py[3]={0,0,0}, pz[3]={0,0,0},
				bb[9]={1e30,0,0, 0,0,0, 0,0,0};
	static mglPoint m1, m2;
	mreal nn[24]={0,0,0, 0,0,1, 0,1,0, 0,1,1, 1,0,0, 1,0,1, 1,1,0, 1,1,1};
	mreal pp[24];
	memcpy(pp, nn, 24*sizeof(mreal));
	// do nothing if transformation matrix the same
	if(memcmp(B,bb,9*sizeof(mreal)) || m1!=Min || m2!=Max)
	{
		m1 = Min;	m2 = Max;
		memcpy(bb,B,9*sizeof(mreal));	PostScale(pp,8);
		// find point with minimal y
		register long i,j;
		for(i=j=1;i<24;i+=3)	if(pp[i]<pp[j])	j=i;
		j--;	memcpy(pp,pp+j,3*sizeof(mreal));
		// find max angle and left point
		// first select 3 closest points
		memcpy(pp+3,nn+j,3*sizeof(mreal));	pp[3]=1-pp[3];	// along x
		memcpy(pp+6,nn+j,3*sizeof(mreal));	pp[7]=1-pp[7];	// along y
		memcpy(pp+9,nn+j,3*sizeof(mreal));	pp[11]=1-pp[11];// along z
		PostScale(pp+3,3);
		pp[3] -= pp[0];	pp[4] -= pp[1];	pp[5] -= pp[2];
		pp[6] -= pp[0];	pp[7] -= pp[1];	pp[8] -= pp[2];
		pp[9] -= pp[0];	pp[10]-= pp[1];	pp[11]-= pp[2];
		// find cosine of axis projection
		mreal cxy, cxz, cyz, dx, dy, dz;
		dx = pp[3]*pp[3] + pp[4]*pp[4];
		dy = pp[6]*pp[6] + pp[7]*pp[7];
		dz = pp[9]*pp[9] + pp[10]*pp[10];
		cxy= pp[3]*pp[6] + pp[4]*pp[7];
		cxz= pp[3]*pp[9] + pp[4]*pp[10];
		cyz= pp[9]*pp[6] + pp[10]*pp[7];
		if(dx==0)		cxy = cxz = 1;
		else if(dy==0)	cxy = cyz = 1;
		else if(dz==0)	cyz = cxz = 1;
		else
		{	cxy /= sqrt(dx*dy);	cxz /= sqrt(dx*dz);	cyz /= sqrt(dz*dy);	}
		// find points for axis
		memcpy(px,nn+j,3*sizeof(mreal));
		memcpy(py,nn+j,3*sizeof(mreal));
		memcpy(pz,nn+j,3*sizeof(mreal));
		if(cxy<cxz && cxy<cyz)	// xy is lowest
		{	// px, py the same as pp
			if(pp[3]<pp[6])	pz[0] = 1-pz[0];
			else	pz[1] = 1-pz[1];
		}
		if(cxz<cxy && cxz<cyz)	// xz is lowest
		{	// px, pz the same as pp
			if(pp[3]<pp[9])	py[0] = 1-py[0];
			else	py[2] = 1-py[2];
		}
		if(cyz<cxz && cyz<cxy)	// yz is lowest
		{	// py, pz the same as pp
			if(pp[9]<pp[6])	px[2] = 1-px[2];
			else	px[1] = 1-px[1];
		}
		// return to normal variables
		// NOTE: this may not work in "inverse" curvilinear coordinates like x->1-x
		px[0] = Min.x + (Max.x-Min.x)*px[0];
		px[1] = Min.y + (Max.y-Min.y)*px[1];
		px[2] = Min.z + (Max.z-Min.z)*px[2];
		py[0] = Min.x + (Max.x-Min.x)*py[0];
		py[1] = Min.y + (Max.y-Min.y)*py[1];
		py[2] = Min.z + (Max.z-Min.z)*py[2];
		pz[0] = Min.x + (Max.x-Min.x)*pz[0];
		pz[1] = Min.y + (Max.y-Min.y)*pz[1];
		pz[2] = Min.z + (Max.z-Min.z)*pz[2];
	}
	// just copy saved values
	memcpy(ax,px,3*sizeof(mreal));
	memcpy(ay,py,3*sizeof(mreal));
	memcpy(az,pz,3*sizeof(mreal));
}
//-----------------------------------------------------------------------------
mreal mglGraphAB::GetOrgX(char dir)
{
	mreal res = Org.x;
	if(isnan(res))
	{
		mreal ax[3], ay[3], az[3];
		FindOptOrg(ax,ay,az);
		if(dir=='x')		res = ax[0];
		else if(dir=='y')	res = ay[0];
		else if(dir=='z')	res = az[0];
		else res = B[6]>0 ? Max.x:Min.x;
	}
	return res;
}
//-----------------------------------------------------------------------------
mreal mglGraphAB::GetOrgY(char dir)
{
	mreal res = Org.y;
	if(isnan(res))
	{
		mreal ax[3], ay[3], az[3];
		FindOptOrg(ax,ay,az);
		if(dir=='x')		res = ax[1];
		else if(dir=='y')	res = ay[1];
		else if(dir=='z')	res = az[1];
		else res = B[7]>0 ? Max.y:Min.y;
	}
	return res;
}
//-----------------------------------------------------------------------------
mreal mglGraphAB::GetOrgZ(char dir)
{
	mreal res = Org.z;
	if(isnan(res))
	{
		mreal ax[3], ay[3], az[3];
		FindOptOrg(ax,ay,az);
		if(dir=='x')		res = ax[2];
		else if(dir=='y')	res = ay[2];
		else if(dir=='z')	res = az[2];
		else res = B[8]>0 ? Max.z:Min.z;
	}
	return res;
}
//-----------------------------------------------------------------------------
bool mglGraphAB::Alpha(bool enable)
{	bool t=UseAlpha;	UseAlpha=enable;	return t;	}
bool mglGraphAB::Light(bool enable)
{	bool t=UseLight;	UseLight=enable;	return t;	}
//-----------------------------------------------------------------------------
void mglGraphAB::Update(){}
void mglGraphAB::ToggleAlpha(){}
void mglGraphAB::ToggleLight(){}
void mglGraphAB::ToggleZoom(){}
void mglGraphAB::ToggleRotate(){}
void mglGraphAB::ToggleNo(){}
void mglGraphAB::ReLoad(bool){}
void mglGraphAB::Adjust(){}
void mglGraphAB::NextFrame(){}
void mglGraphAB::PrevFrame(){}
void mglGraphAB::Animation(){}
void mglGraphAB::Window(int argc, char **argv, int (*draw)(mglGraph *gr, void *p), const char *title, void *par, void (*reload)(int next, void *p), bool maximize){}
//-----------------------------------------------------------------------------
int mgl_draw_class(mglGraph *gr, void *p)
{	return p ? ((mglDraw *)p)->Draw(gr) : 0;	}
void mgl_reload_class(int next, void *p)
{	if(p)	((mglDraw *)p)->Reload(next);	}
void mglGraphAB::Window(int argc, char **argv, mglDraw *draw, const char *title, bool maximize)
{
	Window(argc, argv, mgl_draw_class, title, draw, mgl_reload_class, maximize);
}
//-----------------------------------------------------------------------------

