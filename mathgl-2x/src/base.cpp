/***************************************************************************
 * base.cpp is part of Math Graphic Library
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
#include "mgl/base.h"
#include "mgl/define.h"
//-----------------------------------------------------------------------------
char *mgl_strdup(const char *s)
{
	char *r = (char *)malloc((strlen(s)+1)*sizeof(char));
	memcpy(r,s,(strlen(s)+1)*sizeof(char));
	return r;
}
//-----------------------------------------------------------------------------
void mgl_strtrim(char *str)
{
	char *c = mgl_strdup(str);
	unsigned long n=strlen(str);
	long k;
	for(k=0;k<long(strlen(str));k++)	// óäàëÿåì íà÷àëüíûå ïðîáåëû
		if(str[k]>' ')	break;
	strcpy(c,&(str[k]));
	n = strlen(c);
	for(k=n-1;k>=0;k--)	// óäàëÿåì íà÷àëüíûå ïðîáåëû
		if(c[k]>' ')		break;
	c[k+1] = 0;
	strcpy(str,c);	free(c);
}
//-----------------------------------------------------------------------------
void mgl_strlwr(char *str)
{
	for(long k=0;k<(long)strlen(str);k++)	// óäàëÿåì íà÷àëüíûå ïðîáåëû
		str[k] = (str[k]>='A' && str[k]<='Z') ? str[k]+'a'-'A' : str[k];
}
//-----------------------------------------------------------------------------
mglBase::mglBase()
{
//	memset(this,0,sizeof(mglBase));	// since mglBase is abstract then I can do it?!!
	Flag=0;	saved=false;
#ifdef HAVE_PTHREAD
	pthread_mutex_init(&mutexPnt,0);
	pthread_mutex_init(&mutexTxt,0);
#endif
	fnt=0;	*FontDef==0;
	fx=fy=fz=fa=fc=0;

	InUse = 1;
	// Always create default palette txt[0] and default scheme txt[1]
	Txt.reserve(3);
	Txt.push_back(mglTexture(MGL_DEF_PAL,-1));
	Txt.push_back(mglTexture("BbcyrR",1));
	memcpy(last_style,"{k5}-1\0",8);
	MinS=mglPoint(-1,-1,-1);	MaxS=mglPoint(1,1,1);
}
mglBase::~mglBase()	{	ClearEq();	}
//-----------------------------------------------------------------------------
float mglBase::GetRatio()	{	return 1;	}
//-----------------------------------------------------------------------------
void mglBase::StartGroup(const char *name, int id)
{
	LightScale();
	char buf[128];
	sprintf(buf,"%s_%d",name,id);
	StartAutoGroup(buf);
}
//-----------------------------------------------------------------------------
const char *mglWarn[mglWarnEnd] = {"data dimension(s) is incompatible",
								"data dimension(s) is too small",
								"minimal data value is negative",
								"no file or wrong data dimensions",
								"not enough memory",
								"data values are zero",
								"too many legend entries",
								"no legend entries",
								"slice value is out of range",
								"number of contours is zero or negative",
								"couldn't open file",
								"light: ID is out of range",
								"size(s) is zero or negative",
								"format is not supported for that build",
								"axis ranges are incompatible",
								"pointer is NULL",
								"not enough space for plot"};
//-----------------------------------------------------------------------------
void mglBase::SetWarn(int code, const char *who)
{
	WarnCode = code>0 ? code:0;
	if(code>0 && code<mglWarnEnd)
	{
		if(who)	Mess = Mess+"\n"+who+": ";
		else Mess += "\n";
		Mess = Mess+mglWarn[code-1];
	}
	else if(!code)	Mess="";
	else if(who)	Mess = Mess+(code==-2?"":"\n")+who;
	LoadState();
}
//-----------------------------------------------------------------------------
//		Add points to the buffer
//-----------------------------------------------------------------------------
long mglBase::AddPnt(mglPoint p, float c, mglPoint n, float a, int scl)
{
	if(scl>0)	ScalePoint(p,n,!(scl&2));
	if(isnan(p.x))	return -1;
	a = (a>=0 && a<=1) ? a : AlphaDef;
	c = (c>=0) ? c:CDef;

	mglPnt q;
	if(get(MGL_REDUCEACC))
	{
		q.x=q.xx=int(p.x*10)*0.1;	q.y=q.yy=int(p.y*10)*0.1;	q.z=q.zz=int(p.z*10)*0.1;
		q.c=int(c*100)*0.01;	q.t=int(a*100)*0.01;
		q.u=int(n.x*100)*0.01;	q.v=int(n.y*100)*0.01;	q.w=int(n.z*100)*0.01;
	}
	else
	{
		q.x=q.xx=p.x;	q.y=q.yy=p.y;	q.z=q.zz=p.z;
		q.c=c;	q.t=a;	q.u=n.x;	q.v=n.y;	q.w=n.z;
	}
	q.x=q.xx=int(p.x*100)*0.01;	q.y=q.yy=p.y;	q.z=q.zz=p.z;
	q.c=c;	q.t=a;	q.u=n.x;	q.v=n.y;	q.w=n.z;
	Txt[long(c)].GetC(c,a,q);	// RGBA color

	if(!get(MGL_ENABLE_ALPHA))	q.a=1;
	if(scl&8 && scl>0)	q.a=a;	// bypass palette for enabling alpha in Error()
	if(!get(MGL_ENABLE_LIGHT) && !(scl&4))	q.u=q.v=NAN;
	MGL_PUSH(Pnt,q,mutexPnt);	return Pnt.size()-1;
}
//-----------------------------------------------------------------------------
long mglBase::CopyNtoC(long from, float c)
{
	if(from<0)	return -1;
	mglPnt p=Pnt[from];
	if(!isnan(c))	{	p.c=c;	p.t=0;	Txt[long(c)].GetC(c,0,p);	}
	MGL_PUSH(Pnt,p,mutexPnt);	return Pnt.size()-1;
}
//-----------------------------------------------------------------------------
long mglBase::CopyProj(long from, mglPoint p, mglPoint n)
{
	if(from<0)	return -1;
	mglPnt q=Pnt[from];
	q.x=q.xx=p.x;	q.y=q.yy=p.y;	q.z=q.zz=p.z;
	q.u=n.x;	q.v=n.y;	q.w=n.z;
	MGL_PUSH(Pnt,q,mutexPnt);	return Pnt.size()-1;
}
//-----------------------------------------------------------------------------
void mglBase::Reserve(long n)
{
	if(TernAxis&4)	n*=4;
	Pnt.reserve(n);
}
//-----------------------------------------------------------------------------
//		Boundaries and scaling
//---------------------------------------------------------------------------
void mglBase::RecalcCRange()
{
	if(!fa)
	{	FMin.c = Min.c;	FMax.c = Max.c;	}
	else
	{
		FMin.c = 1e30;	FMax.c = -1e30;
		register int i;
		float a;
		int n=30;
		for(i=0;i<=n;i++)
		{
			a = fa->Calc(0,0,0,Min.c+i*(Max.c-Min.c)/n);
			if(a<FMin.c)	FMin.c=a;
			if(a>FMax.c)	FMax.c=a;
		}
	}
}
//-----------------------------------------------------------------------------
void mglBase::RecalcBorder()
{
	if(!fx &&	!fy &&	!fz)
	{	FMin = Min;	FMax = Max;	}
	else
	{
		FMin = mglPoint( 1e30, 1e30, 1e30);
		FMax = mglPoint(-1e30,-1e30,-1e30);
		register int i,j;
		int n=30;
		for(i=0;i<=n;i++)	for(j=0;j<=n;j++)	// x range
		{
			SetFBord(Min.x, Min.y+i*(Max.y-Min.y)/n, Min.z+j*(Max.z-Min.z)/n);
			SetFBord(Max.x, Min.y+i*(Max.y-Min.y)/n, Min.z+j*(Max.z-Min.z)/n);
		}
		for(i=0;i<=n;i++)	for(j=0;j<=n;j++)	// y range
		{
			SetFBord(Min.x+i*(Max.x-Min.x)/n, Min.y, Min.z+j*(Max.z-Min.z)/n);
			SetFBord(Min.x+i*(Max.x-Min.x)/n, Max.y, Min.z+j*(Max.z-Min.z)/n);
		}
		for(i=0;i<=n;i++)	for(j=0;j<=n;j++)	// x range
		{
			SetFBord(Min.x+i*(Max.x-Min.x)/n, Min.y+j*(Max.y-Min.y)/n, Min.x);
			SetFBord(Min.x+i*(Max.x-Min.x)/n, Min.y+j*(Max.y-Min.y)/n, Max.z);
		}
		if(!fx)	{	FMin.x = Min.x;	FMax.x = Max.x;	}
		if(!fy)	{	FMin.y = Min.y;	FMax.y = Max.y;	}
		if(!fz)	{	FMin.z = Min.z;	FMax.z = Max.z;	}
	}
	RecalcCRange();
}
//-----------------------------------------------------------------------------
void mglBase::SetFBord(float x,float y,float z)
{
	if(fx)
	{
		float v = fx->Calc(x,y,z);
		if(FMax.x < v)	FMax.x = v;
		if(FMin.x > v)	FMin.x = v;
	}
	if(fy)
	{
		float v = fy->Calc(x,y,z);
		if(FMax.y < v)	FMax.y = v;
		if(FMin.y > v)	FMin.y = v;
	}
	if(fz)
	{
		float v = fz->Calc(x,y,z);
		if(FMax.z < v)	FMax.z = v;
		if(FMin.z > v)	FMin.z = v;
	}
}
//-----------------------------------------------------------------------------
bool mglBase::ScalePoint(mglPoint &p, mglPoint &n, bool use_nan)
{
	float &x=p.x, &y=p.y, &z=p.z;
	if(isnan(x) || isnan(y) || isnan(z))	{	x=NAN;	return false;	}
	float x1,y1,z1,x2,y2,z2;
	x1 = x>0?x*MGL_FLT_EPS:x/MGL_FLT_EPS;	x2 = x<0?x*MGL_FLT_EPS:x/MGL_FLT_EPS;
	y1 = y>0?y*MGL_FLT_EPS:y/MGL_FLT_EPS;	y2 = y<0?y*MGL_FLT_EPS:y/MGL_FLT_EPS;
	z1 = z>0?z*MGL_FLT_EPS:z/MGL_FLT_EPS;	z2 = z<0?z*MGL_FLT_EPS:z/MGL_FLT_EPS;
	bool res = true;
	if(x2>CutMin.x && x1<CutMax.x && y2>CutMin.y && y1<CutMax.y &&
		z2>CutMin.z && z1<CutMax.z)	res = false;
	if(fc && fc->Calc(x,y,z))	res = false;

	if(get(MGL_ENABLE_CUT) || !use_nan)
	{
//		if(x1<Min.x || x2>Max.x || y1<Min.y || y2>Max.y || z1<Min.z || z2>Max.z)	res = false;
		if((x1-Min.x)*(x1-Max.x)>0 && (x2-Min.x)*(x2-Max.x)>0)	res = false;
		if((y1-Min.y)*(y1-Max.y)>0 && (y2-Min.y)*(y2-Max.y)>0)	res = false;
		if((z1-Min.z)*(z1-Max.z)>0 && (z2-Min.z)*(z2-Max.z)>0)	res = false;
	}
	else
	{
		if(x1<Min.x)	{x=Min.x;	n=mglPoint(1,0,0);}
		if(x2>Max.x)	{x=Max.x;	n=mglPoint(1,0,0);}
		if(y1<Min.y)	{y=Min.y;	n=mglPoint(0,1,0);}
		if(y2>Max.y)	{y=Max.y;	n=mglPoint(0,1,0);}
		if(z1<Min.z)	{z=Min.z;	n=mglPoint(0,0,1);}
		if(z2>Max.z)	{z=Max.z;	n=mglPoint(0,0,1);}
	}

    x1=x;	y1=y;	z1=z;
    if(fx)	{	x1 = fx->Calc(x,y,z);	n.x *= fx->CalcD('x',x,y,z);	}
    if(fy)	{	y1 = fy->Calc(x,y,z);	n.y *= fy->CalcD('y',x,y,z);	}
    if(fz)	{	z1 = fz->Calc(x,y,z);	n.z *= fz->CalcD('z',x,y,z);	}
    if(isnan(x1) || isnan(y1) || isnan(z1))	{	x=NAN;	return false;	}

    x = (2*x1 - FMin.x - FMax.x)/(FMax.x - FMin.x);
    y = (2*y1 - FMin.y - FMax.y)/(FMax.y - FMin.y);
    z = (2*z1 - FMin.z - FMax.z)/(FMax.z - FMin.z);
    n.x *= 2/(FMax.x - FMin.x);
    n.y *= 2/(FMax.y - FMin.y);
    n.z *= 2/(FMax.z - FMin.z);
    if((TernAxis&3)==1)	// usual ternary axis
    {
        if(x+y>0)
        {
            if(get(MGL_ENABLE_CUT))	res = false;
            else	y = -x;
        }
        x += (y+1)/2;	n.x += n.y/2;
    }
    else if((TernAxis&3)==2)	// quaternary axis
    {
        if(x+y+z>-1)
        {
            if(get(MGL_ENABLE_CUT))	res = false;
            else	z = -1-y-x;
        }
        x += 1+(y+z)/2;		y += (z+1)/3;
        n.x += (n.y+n.z)/2;	n.y += n.z/3;
    }
//	if(fabs(x)>MGL_FLT_EPS)	res = false;
//	if(fabs(y)>MGL_FLT_EPS)	res = false;
//	if(fabs(z)>MGL_FLT_EPS)	res = false;

	if(!res && use_nan)	x = NAN;	// extra sign that point shouldn't be plotted
	return res;
}
//-----------------------------------------------------------------------------
//		Ranges
//-----------------------------------------------------------------------------
void mglBase::SetRanges(mglPoint m1, mglPoint m2)
{
	if(m1.x!=m2.x)	{	Min.x=m1.x;	Max.x=m2.x;	}
	if(m1.y!=m2.y)	{	Min.y=m1.y;	Max.y=m2.y;	}
	if(m1.z!=m2.z)	{	Min.z=m1.z;	Max.z=m2.z;	}
	if(m1.c!=m2.c)	{	Min.c=m1.c;	Max.c=m2.c;	}
	else			{	Min.c=Min.z;Max.c=Max.z;}
//	if(AutoOrg)
	{
		if(Org.x<Min.x && !isnan(Org.x))	Org.x = Min.x;
		if(Org.x>Max.x && !isnan(Org.x))	Org.x = Max.x;
		if(Org.y<Min.y && !isnan(Org.y))	Org.y = Min.y;
		if(Org.y>Max.y && !isnan(Org.y))	Org.y = Max.y;
		if(Org.z<Min.z && !isnan(Org.z))	Org.z = Min.z;
		if(Org.z>Max.z && !isnan(Org.z))	Org.z = Max.z;
	}
	CutMin = mglPoint(0,0,0);	CutMax = mglPoint(0,0,0);
	RecalcBorder();
}
//-----------------------------------------------------------------------------
void mglBase::CRange(const mglDataA &a,bool add, float fact)
{
	float v1=a.Minimal(), v2=a.Maximal(), dv;
	dv=(v2-v1)*fact;	v1 -= dv;	v2 += dv;
	if(v1==v2)	return;
	if(!add)	{	Min.c = v1;	Max.c = v2;	}
	else if(Min.c<Max.c)
	{
		if(Min.c>v1)	Min.c=v1;
		if(Max.c<v2)	Max.c=v2;
	}
	else
	{
		dv = Min.c;
		Min.c = v1<Max.c ? v1:Max.c;
		Max.c = v2>dv ? v2:dv;
	}
	if(Org.c<Min.c && !isnan(Org.c))	Org.c = Min.c;
	if(Org.c>Max.c && !isnan(Org.c))	Org.c = Max.c;
	RecalcCRange();
}
//-----------------------------------------------------------------------------
void mglBase::XRange(const mglDataA &a,bool add,float fact)
{
	float v1=a.Minimal(), v2=a.Maximal(), dv;
	dv=(v2-v1)*fact;	v1 -= dv;	v2 += dv;
	if(v1==v2)	return;
	if(!add)	{	Min.x = v1;	Max.x = v2;	}
	else if(Min.x<Max.x)
	{
		if(Min.x>v1)	Min.x=v1;
		if(Max.x<v2)	Max.x=v2;
	}
	else
	{
		dv = Min.x;
		Min.x = v1<Max.x ? v1:Max.x;
		Max.x = v2>dv ? v2:dv;
	}
	if(Org.x<Min.x && !isnan(Org.x))	Org.x = Min.x;
	if(Org.x>Max.x && !isnan(Org.x))	Org.x = Max.x;
	RecalcBorder();
}
//-----------------------------------------------------------------------------
void mglBase::YRange(const mglDataA &a,bool add,float fact)
{
	float v1=a.Minimal(), v2=a.Maximal(), dv;
	dv=(v2-v1)*fact;	v1 -= dv;	v2 += dv;
	if(v1==v2)	return;
	if(!add)	{	Min.y = v1;	Max.y = v2;	}
	else if(Min.y<Max.y)
	{
		if(Min.y>v1)	Min.y=v1;
		if(Max.y<v2)	Max.y=v2;
	}
	else
	{
		dv = Min.y;
		Min.y = v1<Max.y ? v1:Max.y;
		Max.y = v2>dv ? v2:dv;
	}
	if(Org.y<Min.y && !isnan(Org.y))	Org.y = Min.y;
	if(Org.y>Max.y && !isnan(Org.y))	Org.y = Max.y;
	RecalcBorder();
}
//-----------------------------------------------------------------------------
void mglBase::ZRange(const mglDataA &a,bool add,float fact)
{
	float v1=a.Minimal(), v2=a.Maximal(), dv;
	dv=(v2-v1)*fact;	v1 -= dv;	v2 += dv;
	if(v1==v2)	return;
	if(!add)	{	Min.z = v1;	Max.z = v2;	}
	else if(Min.z<Max.z)
	{
		if(Min.z>v1)	Min.z=v1;
		if(Max.z<v2)	Max.z=v2;
	}
	else
	{
		dv = Min.z;
		Min.z = v1<Max.z ? v1:Max.z;
		Max.z = v2>dv ? v2:dv;
	}
	if(Org.z<Min.z && !isnan(Org.z))	Org.z = Min.z;
	if(Org.z>Max.z && !isnan(Org.z))	Org.z = Max.z;
	RecalcBorder();
}
//-----------------------------------------------------------------------------
void mglBase::SetAutoRanges(float x1, float x2, float y1, float y2, float z1, float z2, float c1, float c2)
{
	if(x1!=x2)	{	Min.x = x1;	Max.x = x2;	}
	if(y1!=y2)	{	Min.y = y1;	Max.y = y2;	}
	if(z1!=z2)	{	Min.z = z1;	Max.z = z2;	}
	if(c1!=c2)	{	Min.c = c1;	Max.c = c2;	}
}
//-----------------------------------------------------------------------------
void mglBase::Ternary(int t)
{
	static mglPoint x1(-1,-1,-1),x2(1,1,1),o(NAN,NAN,NAN);
	static bool c = true;
	TernAxis = t;
	if(t&3)
	{
		x1 = Min;	x2 = Max;	o = Org;
//		c = get(MGL_ENABLE_CUT);	clr(MGL_ENABLE_CUT);
		SetRanges(mglPoint(0,0),mglPoint(1,1,t==1?0:1));
		Org=mglPoint(0,0,0);
	}
	else	{	SetRanges(x1,x2);	Org=o;	/*SetCut(c);*/	}
}
//-----------------------------------------------------------------------------
//		Transformation functions
//-----------------------------------------------------------------------------
void mglBase::SetFunc(const char *EqX,const char *EqY,const char *EqZ,const char *EqA)
{
	if(fa)	delete fa;	if(fx)	delete fx;
	if(fy)	delete fy;	if(fz)	delete fz;
	if(EqX && *EqX && (EqX[0]!='x' || EqX[1]!=0))
		fx = new mglFormula(EqX);
	else	fx = 0;
	if(EqY && *EqY && (EqY[0]!='y' || EqY[1]!=0))
		fy = new mglFormula(EqY);
	else	fy = 0;
	if(EqZ && *EqZ && (EqZ[0]!='z' || EqZ[1]!=0))
		fz = new mglFormula(EqZ);
	else	fz = 0;
	if(EqA && *EqA && ((EqA[0]!='c' && EqA[0]!='a') || EqA[1]!=0))
		fa = new mglFormula(EqA);
	else	fa = 0;
	RecalcBorder();
}
//-----------------------------------------------------------------------------
void mglBase::CutOff(const char *EqC)
{
	if(fc)	delete fc;
	if(EqC && EqC[0])	fc = new mglFormula(EqC);	else	fc = 0;
}
//-----------------------------------------------------------------------------
void mglBase::SetCoor(int how)
{
	switch(how)
	{
	case mglCartesian:	SetFunc(0,0);	break;
	case mglPolar:
		SetFunc("x*cos(y)","x*sin(y)");	break;
	case mglSpherical:
		SetFunc("x*sin(y)*cos(z)","x*sin(y)*sin(z)","x*cos(y)");	break;
	case mglParabolic:
		SetFunc("x*y","(x*x-y*y)/2");	break;
	case mglParaboloidal:
		SetFunc("(x*x-y*y)*cos(z)/2","(x*x-y*y)*sin(z)/2","x*y");	break;
	case mglOblate:
		SetFunc("cosh(x)*cos(y)*cos(z)","cosh(x)*cos(y)*sin(z)","sinh(x)*sin(y)");	break;
//		SetFunc("x*y*cos(z)","x*y*sin(z)","(x*x-1)*(1-y*y)");	break;
	case mglProlate:
		SetFunc("sinh(x)*sin(y)*cos(z)","sinh(x)*sin(y)*sin(z)","cosh(x)*cos(y)");	break;
	case mglElliptic:
		SetFunc("cosh(x)*cos(y)","sinh(x)*sin(y)");	break;
	case mglToroidal:
		SetFunc("sinh(x)*cos(z)/(cosh(x)-cos(y))","sinh(x)*sin(z)/(cosh(x)-cos(y))",
			"sin(y)/(cosh(x)-cos(y))");	break;
	case mglBispherical:
		SetFunc("sin(y)*cos(z)/(cosh(x)-cos(y))","sin(y)*sin(z)/(cosh(x)-cos(y))",
			"sinh(x)/(cosh(x)-cos(y))");	break;
	case mglBipolar:
		SetFunc("sinh(x)/(cosh(x)-cos(y))","sin(y)/(cosh(x)-cos(y))");	break;
	case mglLogLog:	SetFunc("lg(x)","lg(y)");	break;
	case mglLogX:	SetFunc("lg(x)","");	break;
	case mglLogY:	SetFunc("","lg(y)");	break;
	default:	SetFunc(0,0);	break;
	}
}
//-----------------------------------------------------------------------------
void mglBase::ClearEq()
{
	if(fx)	delete fx;	if(fy)	delete fy;	if(fz)	delete fz;
	if(fa)	delete fa;	if(fc)	delete fc;
	fx = fy = fz = fc = fa = 0;
	RecalcBorder();
}
//-----------------------------------------------------------------------------
//		Colors ids
//-----------------------------------------------------------------------------
mglColorID mglColorIds[] = {{'k', mglColor(0,0,0)},
	{'r', mglColor(1,0,0)},		{'R', mglColor(0.5,0,0)},
	{'g', mglColor(0,1,0)},		{'G', mglColor(0,0.5,0)},
	{'b', mglColor(0,0,1)},		{'B', mglColor(0,0,0.5)},
	{'w', mglColor(1,1,1)},		{'W', mglColor(0.7,0.7,0.7)},
	{'c', mglColor(0,1,1)},		{'C', mglColor(0,0.5,0.5)},
	{'m', mglColor(1,0,1)},		{'M', mglColor(0.5,0,0.5)},
	{'y', mglColor(1,1,0)},		{'Y', mglColor(0.5,0.5,0)},
	{'h', mglColor(0.5,0.5,0.5)},	{'H', mglColor(0.3,0.3,0.3)},
	{'l', mglColor(0,1,0.5)},	{'L', mglColor(0,0.5,0.25)},
	{'e', mglColor(0.5,1,0)},	{'E', mglColor(0.25,0.5,0)},
	{'n', mglColor(0,0.5,1)},	{'N', mglColor(0,0.25,0.5)},
	{'u', mglColor(0.5,0,1)},	{'U', mglColor(0.25,0,0.5)},
	{'q', mglColor(1,0.5,0)},	{'Q', mglColor(0.5,0.25,0)},
	{'p', mglColor(1,0,0.5)},	{'P', mglColor(0.5,0,0.25)},
	{' ', mglColor(-1,-1,-1)},	{0, mglColor(-1,-1,-1)}	// the last one MUST have id=0
};
//-----------------------------------------------------------------------------
void mglColor::Set(mglColor c, float br)
{
	if(br<0)	br=0;	if(br>2.f)	br=2.f;
	r = br<=1.f ? c.r*br : 1 - (1-c.r)*(2-br);
	g = br<=1.f ? c.g*br : 1 - (1-c.g)*(2-br);
	b = br<=1.f ? c.b*br : 1 - (1-c.b)*(2-br);
	a = 1;
}
//-----------------------------------------------------------------------------
void mglColor::Set(char p, float bright)
{
	Set(-1,-1,-1);
	for(long i=0; mglColorIds[i].id; i++)
		if(mglColorIds[i].id==p)
		{	Set(mglColorIds[i].col, bright);	break;	}
}
//-----------------------------------------------------------------------------
void mglTexture::Set(const char *s, int smooth, float alpha)
{
	// NOTE: New syntax -- colors are CCCCC or {CNCNCCCN}; options inside []
	if(!s || !s[0])	return;
	register long i,j=0,m=0,l=strlen(s);
	const char *cols = MGL_COLORS;
	for(i=0;i<l;i++)		// find number of colors
	{
		if(s[i]=='[')	j++;	if(s[i]==']')	j--;
		if(strchr(cols,s[i]) && j<1)	n++;
//		if(smooth && s[i]==':')	break;	// NOTE: should use []
	}
	if(!n)
	{
		// it seems to be the only case where new color scheme should be
		if((strchr(s,'|') || strchr(s,'!')) && !smooth)
		{	n=l=6;	s="BbcyrR";	smooth = -1;	}
		else	return;
	}
	if(strchr(s,'|') && !smooth)	smooth = -1;
	mglColor *c = new mglColor[2*n];		// Colors itself
	bool map = (smooth==2), sm = smooth>=0;	// Use mapping, smoothed colors
	for(i=j=n=0;i<l;i++)	// fill colors
	{
		if(s[i]=='[')	j++;	if(s[i]==']')	j--;
		if(s[i]=='{')	m++;	if(s[i]=='}')	m--;
		if(strchr(cols,s[i]) && j<1)		// this is color
		{
			if(m>0 && s[i+1]>'0' && s[i+1]<='9')// ext color
			{	c[2*n] = mglColor(s[i],(s[i+1]-'0')/5.f);	i++;	}
			else	c[2*n] = mglColor(s[i]);	// usual color
			n++;
		}
		// NOTE: User can change alpha if it placed like {AN}
		if(s[i]=='A' && j<1 && m>0 && s[i+1]>'0' && s[i+1]<='9')
		{	alpha = 0.1*(s[i+1]-'0');	i++;	}
	}
	for(i=0;i<n;i++)	// default texture
	{	c[2*i+1]=c[2*i];	c[2*i].a=alpha;	c[2*i+1].a=sm?0:alpha;	}
	if(map && sm)		// map texture
	{
		if(n==2)
		{	c[1]=c[2];	c[2]=c[0];	c[0]=BC;	c[3]=c[1]+c[2];	}
		else if(n==3)
		{	c[1]=c[2];	c[2]=c[0];	c[0]=BC;	c[3]=c[4];	n=2;}
		else
		{	c[1]=c[4];	c[3]=c[6];	n=2;	}
	}
	register float u,v=sm?(n-1)/255.:n/256.;
	for(i=0;i<256;i++)
	{
		u = v*i;	j = long(u);	u-=j;
		if(!sm || j==n-1)
		{	col[2*i] = c[2*j];	col[2*i+1] = c[2*j+1];	}
		else if(j>n-1)	// NOTE: never should be here!
		{	col[2*i] = c[2*n-2];col[2*i+1] = c[2*n-1];	printf("AddTexture -- out of bounds");	}
		else
		{
			col[2*i] = c[2*j]*(1-u)+c[2*j+2]*u;
			col[2*i+1]=c[2*j+1]*(1-u)+c[2*j+3]*u;
		}
	}
	col[512]=col[510];	col[513]=col[511];
	delete []c;
}
//-----------------------------------------------------------------------------
void mglTexture::GetC(float u,float v,mglPnt &p)
{
	u -= long(u);
	register long i=long(256*u);	u = u*256-i;
	mglColor *s=col+2*i;
	p.r = (s[0].r*(1-u)+s[2].r*u)*(1-v) + (s[1].r*(1-u)+s[3].r*u)*v;
	p.g = (s[0].g*(1-u)+s[2].g*u)*(1-v) + (s[1].g*(1-u)+s[3].g*u)*v;
	p.b = (s[0].b*(1-u)+s[2].b*u)*(1-v) + (s[1].b*(1-u)+s[3].b*u)*v;
	p.a = (s[0].a*(1-u)+s[2].a*u)*v + (s[1].a*(1-u)+s[3].a*u)*(1-v);	// for alpha use inverted
}
//-----------------------------------------------------------------------------
bool mglTexture::IsSame(mglTexture &t)
{	return n==t.n && !memcmp(col,t.col,514*sizeof(mglColor));	}
//-----------------------------------------------------------------------------
long mglBase::AddTexture(const char *cols, int smooth)
{
	mglTexture t(cols,smooth);
	if(t.n==0)	return smooth<0 ? 0:1;
	// check if already exist
	for(unsigned long i=0;i<Txt.size();i++)	if(t.IsSame(Txt[i]))	return i;
	// create new one
	if(smooth<0)	CurrPal=0;
	MGL_PUSH(Txt,t,mutexTxt);	return Txt.size()-1;
}
//-----------------------------------------------------------------------------
float mglBase::AddTexture(mglColor c)
{
	register unsigned long i,j;
	if(!c.Valid())	return -1;
	// first lets try an existed one
	for(i=0;i<Txt.size();i++)	for(j=0;j<256;j++)
		if(c==Txt[i].col[2*j])
			return i+j/256.;
	// add new texture
	mglTexture t;
	for(i=0;i<514;i++)	t.col[i]=c;
	MGL_PUSH(Txt,t,mutexTxt);	return Txt.size()-1;
}
//-----------------------------------------------------------------------------
//		Coloring and palette
//-----------------------------------------------------------------------------
float mglBase::NextColor(long &id)
{
	long i=abs(id)/256, n=Txt[i].n, p=abs(id)&0xff;
	if(id>=0)	{	p=(p+1)%n;	id = 256*i+p;	}
	mglColor c = Txt[i].col[int(512*(p+0.5)/n)];
	float dif, dmin=1;
	// try to find closest color
	for(long j=0;mglColorIds[j].id;j++)	for(long k=1;k<10;k++)
	{
		mglColor cc;	cc.Set(mglColorIds[j].col,k/5.);
		dif = (c-cc).NormS();
		if(dif<dmin)
		{
			last_style[1] = mglColorIds[j].id;
			last_style[2] = k+'0';
			dmin=dif;
		}
	}
	if(!leg_str.empty())
	{	AddLegend(leg_str.c_str(),last_style);	leg_str.clear();	}
	CDef = i + (n>0 ? (p+0.5)/n : 0);	CurrPal++;
	return CDef;
}
//-----------------------------------------------------------------------------
char mglBase::SetPenPal(const char *p, long *Id)
{
	char mk=0;
	PDef = 0xffff;	// reset to solid line
	memcpy(last_style,"{k5}-1\0",8);

	Arrow1 = Arrow2 = 0;	PenWidth = 1;
	if(p && *p)
	{
//		const char *col = "wkrgbcymhRGBCYMHWlenuqpLENUQP";
		const char *stl = " -|;:ji=";
		const char *mrk = "*o+xsd.^v<>";
		const char *wdh = "123456789";
		const char *arr = "AKDTVISO_";
		long m=0;
		for(unsigned i=0;i<strlen(p);i++)
		{
			if(p[i]=='{')	m++;	if(p[i]=='}')	m--;
			if(m>0)	continue;
			if(strchr(stl,p[i]))
			{
				switch(p[i])
				{
				case '|': PDef = 0x00ff;	break;
				case ';': PDef = 0x0f0f;	break;
				case '=': PDef = 0x3333;	break;
				case ':': PDef = 0x1111;	break;
				case 'j': PDef = 0x087f;	break;
				case 'i': PDef = 0x2727;	break;
				case ' ': PDef = 0x0000;	break;
				default:  PDef = 0xffff;	break;	// '-'
				}
				last_style[4]=p[i];
			}
			else if(strchr(mrk,p[i]))	mk = p[i];
			else if(strchr(wdh,p[i]))
			{	last_style[5] = p[i];	PenWidth = p[i]-'0';	}
			else if(strchr(arr,p[i]))
			{
				if(!Arrow2)	Arrow2 = p[i];
				else	Arrow1 = p[i];
			}
		}
		if(Arrow1=='_')	Arrow1=0;	if(Arrow2=='_')	Arrow2=0;
		if(strchr(p,'#'))
		{
			if(mk=='.')	mk = 'C';
			if(mk=='+')	mk = 'P';
			if(mk=='x')	mk = 'X';
			if(mk=='o')	mk = 'O';
			if(mk=='d')	mk = 'D';
			if(mk=='s')	mk = 'S';
			if(mk=='^')	mk = 'T';
			if(mk=='v')	mk = 'V';
			if(mk=='<')	mk = 'L';
			if(mk=='>')	mk = 'R';
			if(mk=='*')	mk = 'Y';
		}
	}
	last_style[6]=mk;
	long tt, n;
	tt = AddTexture(p,-1);	n=Txt[tt].n;
	CDef = tt+((n+CurrPal-1)%n+0.5)/n;
	if(Id)	*Id=long(tt)*256+(n+CurrPal-1)%n;
	return mk;
}
//-----------------------------------------------------------------------------
float mglBase::GetA(float a)
{
	if(fa)		a = fa->Calc(0,0,0,a);
	a = (a-FMin.c)/(FMax.c-FMin.c);
	a = a<1?(a>0?a:0):1/MGL_FLT_EPS;	// for texture a must be <1 always!!!
	return a;
}
//-----------------------------------------------------------------------------
mglPoint GetX(const mglDataA *x, int i, int j, int k)
{
	k = k<x->GetNz() ? k : 0;
	if(x->GetNy()>1)
		return mglPoint(x->v(i,j,k),x->dvx(i,j,k),x->dvy(i,j,k));
	else
		return mglPoint(x->v(i),x->dvx(i),0);
}
//-----------------------------------------------------------------------------
mglPoint GetY(const mglDataA *y, int i, int j, int k)
{
	k = k<y->GetNz() ? k : 0;
	if(y->GetNy()>1)
		return mglPoint(y->v(i,j,k),y->dvx(i,j,k),y->dvy(i,j,k));
	else
		return mglPoint(y->v(j),0,y->dvx(j));
}
//-----------------------------------------------------------------------------
mglPoint GetZ(const mglDataA *z, int i, int j, int k)
{
	if(z->GetNy()>1)
		return mglPoint(z->v(i,j,k),z->dvx(i,j,k),z->dvy(i,j,k));
	else
		return mglPoint(z->v(k),0,0);
}
//-----------------------------------------------------------------------------
void mglBase::vect_plot(long p1, long p2, float s)
{
	if(p1<0 || p2<0)	return;
	const mglPnt &q1=Pnt[p1], &q2=Pnt[p2];
	mglPnt s1=q2,s2=q2;
	s = s<=0 ? 0.1 : s*0.1;
	s1.x=s1.xx = q2.x - 3*s*(q2.x-q1.x) + s*(q2.y-q1.y);
	s2.x=s2.xx = q2.x - 3*s*(q2.x-q1.x) - s*(q2.y-q1.y);
	s1.y=s1.yy = q2.y - 3*s*(q2.y-q1.y) - s*(q2.x-q1.x);
	s2.y=s2.yy = q2.y - 3*s*(q2.y-q1.y) + s*(q2.x-q1.x);
	s1.z=s1.zz=s2.z=s2.zz = q2.z - 3*s*(q2.z-q1.z);
	long n1,n2;
	n1=Pnt.size();	MGL_PUSH(Pnt,s1,mutexPnt);
	n2=Pnt.size();	MGL_PUSH(Pnt,s2,mutexPnt);
	line_plot(p1,p2);	line_plot(n1,p2);	line_plot(p2,n2);
}
//-----------------------------------------------------------------------------
int mglFindArg(const char *str)
{
	register long l=0,k=0,i;//,j,len=strlen(lst);
	for(i=0;i<long(strlen(str));i++)
	{
		if(str[i]=='\'') l++;
		if(str[i]=='{') k++;
		if(str[i]=='}') k--;
		if(l%2==0 && k==0 && (str[i]=='#' || str[i]==';'))	return -i;
		if(l%2==0 && k==0 && (str[i]<=' '))	return i;
	}
	return 0;
}
//-----------------------------------------------------------------------------
void mglBase::SetAmbient(float bright)	{	AmbBr = bright;	}
//-----------------------------------------------------------------------------
float mglBase::SaveState(const char *opt)
{
	if(!opt || !opt[0] || saved)	return NAN;
	MSS=MarkSize;	ASS=ArrowSize;
	FSS=FontSize;	ADS=AlphaDef;
	MNS=MeshNum;	CSS=Flag;	LSS=AmbBr;
	MinS=Min;		MaxS=Max;	saved=true;
	// parse option
	char *q=mgl_strdup(opt),*s,*a,*b,*c;
	long n;
	mgl_strtrim(q);
	// NOTE: not consider '#' inside legend entry !!!
	s=strchr(q,'#');	if(s)	*s=0;
	while(q && *q)
	{
		s=q;	q=strchr(s,';');
		if(q)	{	*q=0;	q++;	}
		mgl_strtrim(s);		a=s;
		n=mglFindArg(s);	if(n>0)	{	s[n]=0;		s=s+n+1;	}
		mgl_strtrim(a);		b=s;
		n=mglFindArg(s);	if(n>0)	{	s[n]=0;		s=s+n+1;	}
		mgl_strtrim(b);

		float ff=atof(b),ss;
		if(!strcmp(b,"on"))	ff=1;
		if(!strcmp(a+1,"range"))
		{
			n=mglFindArg(s);	c=s;
			if(n>0)	{	s[n]=0;	s=s+n+1;	}
			mgl_strtrim(c);		ss = atof(c);
			if(a[0]=='x')		{	Min.x=ff;	Max.x=ss;	}
			else if(a[0]=='y')	{	Min.y=ff;	Max.y=ss;	}
			else if(a[0]=='z')	{	Min.z=ff;	Max.z=ss;	}
//			else if(a[0]=='c')	{	Min.c=ff;	Max.c=ss;	}	// Bad idea since there is formula for coloring
		}
		else if(!strcmp(a,"cut"))		SetCut(ff!=0);
		else if(!strcmp(a,"meshnum"))	SetMeshNum(ff);
		else if(!strcmp(a,"alpha"))		{Alpha(true);	SetAlphaDef(ff);}
		else if(!strcmp(a,"light"))		Light(ff!=0);
		else if(!strcmp(a,"ambient"))	SetAmbient(ff);
		else if(!strcmp(a,"diffuse"))	SetDifLight(ff);
		else if(!strcmp(a,"marksize"))	SetMarkSize(ff);
		else if(!strcmp(a,"fontsize"))	SetFontSize(ff);
		else if(!strcmp(a,"arrowsize"))	SetArrowSize(ff);
		else if(!strcmp(a,"size"))
		{	SetMarkSize(ff);	SetFontSize(ff);	SetArrowSize(ff);	}
		else if(!strcmp(a,"num") || !strcmp(a,"number") || !strcmp(a,"value"))	return ff;
		else if(!strcmp(a,"legend"))
		{	if(*b=='\'')	{	b++;	b[strlen(b)-1]=0;	}	leg_str = b;	}
	}
	free(q);	return NAN;
}
//-----------------------------------------------------------------------------
void mglBase::LoadState()
{
	if(!saved)	return;
	MarkSize=MSS;	ArrowSize=ASS;
	FontSize=FSS;	AlphaDef=ADS;
	MeshNum=MNS;	Flag=CSS;	AmbBr=LSS;
	Min=MinS;		Max=MaxS;	saved=false;
}
//-----------------------------------------------------------------------------
void mglBase::AddLegend(const wchar_t *text,const char *style)
{	if(text)	MGL_PUSH(Leg,mglText(text,style),mutexLeg);	}
//-----------------------------------------------------------------------------
void mglBase::AddLegend(const char *str,const char *style)
{
	if(!str)	return;
	unsigned s = strlen(str)+1;
	wchar_t *wcs = new wchar_t[s];
	mbstowcs(wcs,str,s);
	AddLegend(wcs, style);
	delete []wcs;
}
//-----------------------------------------------------------------------------
