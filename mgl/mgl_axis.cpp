/***************************************************************************
 * mgl_axis.cpp is part of Math Graphic Library
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
#include <ctype.h>
#include <wchar.h>
#include <stdarg.h>
#include <stdlib.h>

#include "mgl/mgl_eval.h"
#include "mgl/mgl.h"
#include "mgl/mgl_c.h"
#define FLT_EPS	(1.+1.2e-07)

#ifdef WIN32
#define swprintf    _snwprintf
#endif

//-----------------------------------------------------------------------------
wchar_t *mgl_wcsdup(const wchar_t *s)
{
	wchar_t *r = (wchar_t *)malloc((wcslen(s)+1)*sizeof(wchar_t));
	memcpy(r,s,(wcslen(s)+1)*sizeof(wchar_t));
	return r;
}
//-----------------------------------------------------------------------------
char *mgl_strdup(const char *s)
{
	char *r = (char *)malloc((strlen(s)+1)*sizeof(char));
	memcpy(r,s,(strlen(s)+1)*sizeof(char));
	return r;
}
//-----------------------------------------------------------------------------
void wcstrim_mgl(wchar_t *str)
{
	wchar_t *c = mgl_wcsdup(str);
	unsigned long n=wcslen(str);
	long k;
	for(k=0;k<long(wcslen(str));k++)	// удаляем начальные пробелы
		if(!isspace(str[k]))	break;
	wcscpy(c,&(str[k]));
	n = wcslen(c);
	for(k=n-1;k>=0;k--)	// удаляем начальные пробелы
		if(!isspace(c[k]))		break;
	c[k+1] = 0;
	wcscpy(str,c);	free(c);
}
//-----------------------------------------------------------------------------
///	Round the number to k digits. For example, x = 0.01234 -> r = 0.0123 for k=3.
float mgl_okrugl(float x,int k)
{
	if(x<=0)	return 0;
	float r;
	long n = long(log10(x));
	r = floor(x*pow(10.,k-n))/pow(10.,k-n);
	return r;
}
//-----------------------------------------------------------------------------
void mglGraph::DrawTick(float *pp,bool sub)
{
//	bool tt[3];
//	float d1,d2,f=sub?0.02:0.04;
	ScalePoint(pp[0],pp[1],pp[2]);
	ScalePoint(pp[3],pp[4],pp[5]);
	ScalePoint(pp[6],pp[7],pp[8]);
/*	pp[0] -= pp[3];		pp[1] -= pp[4];		pp[2] -= pp[5];
	pp[6] -= pp[3];		pp[7] -= pp[4];		pp[8] -= pp[5];
	d1 = sqrt(pp[0]*pp[0] + pp[1]*pp[1] + pp[2]*pp[2]);
	d2 = sqrt(pp[6]*pp[6] + pp[7]*pp[7] + pp[8]*pp[8]);
	d1 = (d1>0 ? d1:1)/f;	d2 = (d2>0 ? d2:1)/f;
	pp[0] = pp[3] + pp[0]/d1;
	pp[1] = pp[4] + pp[1]/d1;
	pp[2] = pp[5] + pp[2]/d1;
	pp[6] = pp[3] + pp[6]/d2;
	pp[7] = pp[4] + pp[7]/d2;
	pp[8] = pp[5] + pp[8]/d2;*/
	curv_plot(3,pp,0);
}
//-----------------------------------------------------------------------------
void mglGraph::DrawXGridLine(float t, float y0, float z0)
{
	register int i;
	float pp[3*31];
	bool tt[31];
	for(i=0;i<31;i++)
	{
		pp[3*i]=TernAxis ? t-t*i/30. : t;
		pp[3*i+1]=TernAxis ? t*i/30. : y0;
		pp[3*i+2]=Min.z+(Max.z-Min.z)*i/30.;
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
	}
	curv_plot(31,pp,tt);
	for(i=0;i<31;i++)
	{
		pp[3*i]=t;	pp[3*i+2]=z0;
		pp[3*i+1]=Min.y+(Max.y-(TernAxis ? t:Min.y))*i/30.;
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
	}
	curv_plot(31,pp,tt);
}
//-----------------------------------------------------------------------------
void mglGraph::DrawYGridLine(float t, float x0, float z0)
{
	register int i;
	float pp[3*31];
	bool tt[31];
	for(i=0;i<31;i++)
	{
		pp[3*i]=TernAxis ? t*i/30. : x0;
		pp[3*i+1]=TernAxis ? t-t*i/30. : t;
		pp[3*i+2]=Min.z+(Max.z-Min.z)*i/30.;
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
	}
	curv_plot(31,pp,tt);
	for(i=0;i<31;i++)
	{
		pp[3*i+2]=z0;	pp[3*i+1]=t;
		pp[3*i]=Min.x+(Max.x-(TernAxis ? t : Min.x))*i/30.;
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
	}
	curv_plot(31,pp,tt);
}
//-----------------------------------------------------------------------------
void mglGraph::DrawZGridLine(float t, float x0, float y0)
{
	register int i;
	float pp[3*31];
	bool tt[31];
	for(i=0;i<31;i++)
	{
		pp[3*i+1]=y0;	pp[3*i+2]=t;
		pp[3*i]=Min.x+(Max.x-Min.x)*i/30;
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
	}
	curv_plot(31,pp,tt);
	for(i=0;i<31;i++)
	{
		pp[3*i]=x0;	pp[3*i+2]=t;
		pp[3*i+1]=Min.y+(Max.y-Min.y)*i/30.;
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
	}
	curv_plot(31,pp,tt);
}
//-----------------------------------------------------------------------------
void mglGraph::Grid(const char *dir, const char *pen)
{
	static int cgid=1;	StartGroup("AxisGrid",cgid++);
	float x0,y0,z0,ddx,ddy,ddz,t;
	if(pen)	SelectPen(pen);
	else	SelectPen(TranspType!=2 ? "B;1" : "b;1");
	ddx = dx>=0 ? dx : mgl_okrugl((Min.x-Max.x)/(dx+1),3);
	ddy = dy>=0 ? dy : mgl_okrugl((Min.y-Max.y)/(dy+1),3);
	ddz = dz>=0 ? dz : mgl_okrugl((Min.z-Max.z)/(dz+1),3);
	if(strchr(dir,'x'))
	{
		x0 = GetOrgX('x');	y0 = GetOrgY('x');	z0 = GetOrgZ('x');
		x0 = isnan(OrgT.x) ? x0 : OrgT.x;
		x0 = x0 - ddx*floor((x0-Min.x)/ddx);
		if(xnum)	for(int i=0;i<xnum;i++)		DrawXGridLine(xval[i],y0,z0);
		else if(dx)	for(t=x0;t<=Max.x;t+=ddx)	DrawXGridLine(t,y0,z0);
		else if(Min.x>0)
		{
			x0 = exp(M_LN10*floor(0.1+log10(Min.x)));
			for(t=x0;t<=Max.x*FLT_EPS;t*=10)	DrawXGridLine(t,y0,z0);
		}
	}
	if(strchr(dir,'y'))
	{
		x0 = GetOrgX('y');	y0 = GetOrgY('y');	z0 = GetOrgZ('y');
		y0 = isnan(OrgT.y) ? y0 : OrgT.y;
		y0 = y0 - ddy*floor((y0-Min.y)/ddy);
		if(ynum)	for(int i=0;i<ynum;i++)		DrawYGridLine(yval[i],x0,z0);
		else if(dy)	for(t=y0;t<=Max.y;t+=ddy)	DrawYGridLine(t,x0,z0);
		else if(Min.y>0)
		{
			y0 = exp(M_LN10*floor(0.1+log10(Min.y)));
			for(t=y0;t<=Max.y*FLT_EPS;t*=10)	DrawYGridLine(t,x0,z0);
		}
	}
	if(strchr(dir,'z'))
	{
		x0 = GetOrgX('z');	y0 = GetOrgY('z');	z0 = GetOrgZ('z');
		z0 = isnan(OrgT.z) ? z0 : OrgT.z;
		z0 = z0 - ddz*floor((z0-Min.z)/ddz);
		if(znum)	for(int i=0;i<znum;i++)		DrawZGridLine(zval[i],x0,y0);
		else if(dz)	for(t=z0;t<=Max.z;t+=ddz)	DrawZGridLine(t,x0,y0);
		else if(Min.z>0)
		{
			z0 = exp(M_LN10*floor(0.1+log10(Min.z)));
			for(t=z0;t<=Max.z*FLT_EPS;t*=10)	DrawZGridLine(t,x0,y0);
		}
	}
	SelectPen("k-1");
	EndGroup();
}
//-----------------------------------------------------------------------------
void mglGraph::Label(char dir,const wchar_t *text,int pos,float size,float shift)
{
	float t, x0, y0, z0;
	SelectPen("k-1");
	x0 = GetOrgX(dir);	y0 = GetOrgY(dir);	z0 = GetOrgZ(dir);
	if(size<=0)	size = -size*FontSize;
	if(dir=='x')
	{
		t = pos>0 ? Max.x : (pos<0 ? Min.x : (dx ? (Min.x+Max.x)/2 : sqrt(Min.x*Max.x)));
		Putsw(mglPoint(t,y0,z0),text,FontDef,size,'X',shift);
	}
	if(dir=='y')
	{
		t = pos>0 ? Max.y : (pos<0 ? Min.y : (dy ? (Min.y+Max.y)/2 : sqrt(Min.y*Max.y)));
		if(TernAxis)
			Putsw(mglPoint(Max.x-t,t,z0),text,FontDef,size,'Y',shift);
		else
			Putsw(mglPoint(x0,t,z0),text,FontDef,size,'Y',shift);
	}
	if(dir=='z')
	{
		t = pos>0 ? Max.z : (pos<0 ? Min.z : (dz ? (Min.z+Max.z)/2 : sqrt(Min.z*Max.z)));
		if(TernAxis)
			Putsw(mglPoint(x0,Max.y-t,z0),text,FontDef,size,'Z',shift);
		else
			Putsw(mglPoint(x0,y0,t),text,FontDef,size,'Z',shift);
	}
}
//-----------------------------------------------------------------------------
int _mgl_tick_ext(float a, float b, wchar_t s[32], float &v)
{
	int kind = 0;
	if(fabs(a-b)<0.01*fabs(a))
	{
		kind = 1;
		v = fabs(a-b);
		if(v>100.f)
		{
			int k=int(log10(v)-0.01);
			kind=3;		v=mgl_ipow(10,k);
			swprintf(s, 32, L"(@{\\times{}10^{%d}})", k);
		}
		if(v<1e-2f)
		{
			int k=int(log10(v)-0.01)-1;
			kind=3;		v=mgl_ipow(10,k);
			swprintf(s, 32, L"(@{\\times{}10^{%d}})", k);
		}
	}
	else
	{
		v = fabs(b)>fabs(a)?fabs(b):fabs(a);
		if(v>100.f)
		{
			kind = 2;
			int k=int(log10(v)-0.01);
			v=mgl_ipow(10,k);
			swprintf(s, 32, L"\\times 10^{%d}", k);
		}
		if(v<1e-2f)
		{
			kind = 2;
			int k=int(log10(v)-0.01)-1;
			v=mgl_ipow(10,k);
			swprintf(s, 32, L"\\times 10^{%d}", k);
		}
	}
	return kind;
}
//-----------------------------------------------------------------------------
void _mgl_tick_text(float z, float z0, float d, float v, int kind, wchar_t str[64])
{
	float u = fabs(z)<d ? 0:z;
	if((kind&1) && z>z0)	u = fabs(z-z0)<d ? 0:(z-z0);
	if(kind==2 || (kind==3 && z>z0))	u /= v;
	if((kind&1) && z>z0)
	{
		int n1,n2;
		swprintf(str, 64, fabs(u)<1 ? L"@{(+%.2g)}" : L"@{(+%.3g)}",u);	n1=wcslen(str);
		swprintf(str, 64, L"@{(+%g)}",u);	n2=wcslen(str);
		if(n1<n2)	swprintf(str, 64, L"@{(+%.2g)}",u);
	}
	else
	{
		int n1,n2;
		swprintf(str, 64, fabs(u)<1 ? L"%.2g" :  L"%.3g",u);
		n1=wcslen(str);	swprintf(str, 64, L"%g",u);	n2=wcslen(str);
		if(n1<n2)	swprintf(str, 64, L"%.2g",u);
	}
}
//-----------------------------------------------------------------------------
void mglGraph::DrawXTick(float x, float y0, float z0, float dy, float dz, int f)
{
	float pp[9],ff=TickLen/sqrt(1.+f);
	pp[0]=x;	pp[1]=y0+dy*ff;	pp[2]=z0;
	pp[3]=x;	pp[4]=y0;	pp[5]=z0;
	pp[6]=x;	pp[7]=y0;	pp[8]=z0+dz*ff;
	DrawTick(pp,false);
}
void mglGraph::AxisX(bool text)
{
	float pp[3*31];
	bool tt[31];
	wchar_t str[64];
	long i;
	float x0,y0,z0,x,ddx,ddy,ddz,v=0;
	x0 = GetOrgX('x');	y0 = GetOrgY('x');	z0 = GetOrgZ('x');
	ddx = dx>=0 ? dx : mgl_okrugl((Min.x-Max.x)/(dx+1),3);
	ddy = dy>=0 ? dy : mgl_okrugl((Min.y-Max.y)/(dy+1),3);
	ddz = dz>=0 ? dz : mgl_okrugl((Min.z-Max.z)/(dz+1),3);
	if(ddy==0)	ddy=y0*3;//(Max.y-Min.y)/2;
 	if(ddz==0)	ddz=z0*3;//(Max.z-Min.z)/2;
	if(y0>(Max.y+Min.y)/2)	ddy = -ddy;
	if(z0>(Max.z+Min.z)/2)	ddz = -ddz;
	SelectPen(TranspType!=2 ? "k-1":"w-1");
	for(i=0;i<31;i++)	// сама ось
	{
		pp[3*i+1]=y0;	pp[3*i+2]=z0;
		pp[3*i]=Min.x+(Max.x-Min.x)*i/30.;
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
	}
	curv_plot(31,pp,tt);

	if(xnum)	for(int i=0;i<xnum;i++)
	{
		DrawXTick(xval[i],y0,z0,ddy,ddz);
		if(text)	Putsw(mglPoint(xval[i],y0,z0),xstr[i],FontDef,-1,'x');
	}
	else if(ddx>0)	// ticks drawing
	{
		int kind=0;
		wchar_t s[32]=L"";
		if(!xtt) kind = _mgl_tick_ext(Max.x, Min.x, s, v);
		if(!TuneTicks)	kind = 0;

		NSx = NSx<0 ? 0 : NSx;
		x0 = isnan(OrgT.x) ? x0 : OrgT.x;
		x0 = x0 - ddx*floor((x0-Min.x)/ddx+1e-3);
		for(x=x0;x<=Max.x+(Max.x-Min.x)*1e-6;x+=ddx)
		{
			DrawXTick(x,y0,z0,ddy,ddz);
			if(text)
			{
				if(xtt)	swprintf(str, 64, xtt, fabs(x)<ddx/100 ? 0 : x);
				else	_mgl_tick_text(x,x0,ddx/100,v,kind,str);
				wcstrim_mgl(str);
				Putsw(mglPoint(x,y0,z0),str,FontDef,-1,'x');
			}
		}
		if(text && (kind&2))
			Putsw(mglPoint(FactorPos*(Max.x-Min.x)+Min.x,y0,z0), s, FontDef, -1, 'x');
		if(NSx>0)
		{
			ddx /= (NSx+1);	// subticks drawing
			x0 = GetOrgX('x');	x0 = isnan(OrgT.x) ? x0 : OrgT.x;
			x0 = x0 - ddx*floor((x0-Min.x)/ddx+1e-3);
			for(x=x0;x<=Max.x;x+=ddx)	DrawXTick(x,y0,z0,ddy,ddz,1);
		}
	}
	else if(Min.x>0)
	{
		int k=1+int(log10(Max.x/Min.x)/5), cur=0;
		x0 = exp(M_LN10*floor(0.1+log10(Min.x)));
		for(x=x0;x<=Max.x*FLT_EPS;x*=10.f)
		{
			if(x*FLT_EPS>=Min.x && x<=Max.x*FLT_EPS)
			{
				DrawXTick(x,y0,z0,ddy,ddz);
				swprintf(str,64,L"10^{%d}",int(floor(0.1+log10(x))));
				if(text && cur%k==0)
					Putsw(mglPoint(x,y0,z0),str,FontDef,-1,'x');
				cur++;
			}
			// subticks drawing
			for(v=x;v<10*x;v+=x)	if(v>Min.x && v<Max.x)
				DrawXTick(v,y0,z0,ddy,ddz,1);
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraph::DrawYTick(float y, float x0, float z0, float dx, float dz, int f)
{
	float pp[9],ff=TickLen/sqrt(1.+f);
	pp[0]=x0+dx*ff;	pp[1]=y;	pp[2]=z0;
	pp[3]=x0;		pp[4]=y;	pp[5]=z0;
	pp[6]=x0;		pp[7]=y;	pp[8]=z0+dz*ff;
	DrawTick(pp,false);
}
void mglGraph::AxisY(bool text)
{
	float pp[3*31];
	bool tt[31];
	wchar_t str[64];
	long i;
	float x0,y0,z0,y,ddx,ddy,ddz,v=0;
	x0 = GetOrgX('y');	y0 = GetOrgY('y');	z0 = GetOrgZ('y');
	ddx = dx>=0 ? dx : mgl_okrugl((Min.x-Max.x)/(dx+1),3);
	ddy = dy>=0 ? dy : mgl_okrugl((Min.y-Max.y)/(dy+1),3);
	ddz = dz>=0 ? dz : mgl_okrugl((Min.z-Max.z)/(dz+1),3);
	if(ddx==0)	ddx=x0*3;//(Max.x-Min.x)/2;
	if(ddz==0)	ddz=z0*3;//(Max.z-Min.z)/2;
	if(x0>(Max.x+Min.x)/2)	ddx = -ddx;
	if(z0>(Max.z+Min.z)/2)	ddz = -ddz;
	SelectPen(TranspType!=2 ? "k-1":"w-1");
	for(i=0;i<31;i++)	// сама ось
	{
		pp[3*i]=x0;	pp[3*i+2]=z0;
		pp[3*i+1]=Min.y+(Max.y-Min.y)*i/30.;
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
	}
	curv_plot(31,pp,tt);

	if(ynum)	for(int i=0;i<ynum;i++)
	{
		DrawYTick(yval[i],x0,z0,ddx,ddz);
		if(text)	Putsw(mglPoint(x0,yval[i],z0),ystr[i],FontDef,-1,'y');
	}
	else if(ddy>0)	// ticks drawing
	{
		int kind=0;
		wchar_t s[32]=L"";
		if(!ytt) kind = _mgl_tick_ext(Max.y, Min.y, s, v);
		if(!TuneTicks)	kind = 0;

		NSy = NSy<0 ? 0 : NSy;
		y0 = isnan(OrgT.y) ? y0 : OrgT.y;
		y0 = y0 - ddy*floor((y0-Min.y)/ddy+1e-3);
		for(y=y0;y<=Max.y+(Max.y-Min.y)*1e-6;y+=ddy)
		{
			DrawYTick(y,x0,z0,ddx,ddz);
			if(text)
			{
				if(ytt)	swprintf(str, 64, ytt, fabs(y)<ddy/100 ? 0 : y);
				else	_mgl_tick_text(y,y0,ddy/100,v,kind,str);
				wcstrim_mgl(str);
				Putsw(mglPoint(x0,y,z0),str,FontDef,-1,'y');
			}
		}
		if(text && (kind&2))
			Putsw(mglPoint(x0,FactorPos*(Max.y-Min.y)+Min.y,z0), s, FontDef, -1, 'y');
		if(NSy>0)
		{
			ddy /= (NSy+1);	// subticks drawing
			y0 = GetOrgY('y');	y0 = isnan(OrgT.y) ? y0 : OrgT.y;
			y0 = y0 - ddy*floor((y0-Min.y)/ddy+1e-3);
			for(y=y0;y<=Max.y;y+=ddy)	DrawYTick(y,x0,z0,ddx,ddz,1);
		}
	}
	else if(Min.y>0)
	{
		int k=1+int(log10(Max.y/Min.y)/5), cur=0;
		y0 = exp(M_LN10*floor(0.1+log10(Min.y)));
		for(y=y0;y<=Max.y*FLT_EPS;y*=10)
		{
			if(y*FLT_EPS>=Min.y && y<=Max.y*FLT_EPS)
			{
				DrawYTick(y,x0,z0,ddx,ddz);
				swprintf(str,64,L"10^{%d}",int(floor(0.1+log10(y))));
				if(text && cur%k==0)
					Putsw(mglPoint(x0,y,z0),str,FontDef,-1,'y');
				cur++;
			}
			// subticks drawing
			for(v=y;v<10*y;v+=y)	if(v>Min.y && v<Max.y)
				DrawYTick(v,x0,z0,ddx,ddz,1);
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraph::DrawZTick(float z, float x0, float y0, float dx, float dy, int f)
{
	float pp[9],ff=TickLen/sqrt(1.+f);
	pp[0]=x0;	pp[1]=y0+dy*ff;	pp[2]=z;
	pp[3]=x0;	pp[4]=y0;		pp[5]=z;
	pp[6]=x0+dx*ff;	pp[7]=y0;	pp[8]=z;
	DrawTick(pp,false);
}
void mglGraph::AxisZ(bool text)
{
	float pp[3*31];
	bool tt[31];
	wchar_t str[64];
	long i;
	float x0,y0,z0,z,ddx,ddy,ddz,v=0;
	x0 = GetOrgX('z');	y0 = GetOrgY('z');	z0 = GetOrgZ('z');
	ddx = dx>=0 ? dx : mgl_okrugl((Min.x-Max.x)/(dx+1),3);
	ddy = dy>=0 ? dy : mgl_okrugl((Min.y-Max.y)/(dy+1),3);
	ddz = dz>=0 ? dz : mgl_okrugl((Min.z-Max.z)/(dz+1),3);
	if(ddx==0)	ddx=x0*3;//(Max.x-Min.x)/2;
	if(ddy==0)	ddy=y0*3;//(Max.y-Min.y)/2;
	if(x0>(Max.x+Min.x)/2)	ddx = -ddx;
	if(y0>(Max.y+Min.y)/2)	ddy = -ddy;
	SelectPen(TranspType!=2 ? "k-1":"w-1");
	for(i=0;i<31;i++)	// сама ось
	{
		pp[3*i]=x0;	pp[3*i+1]=y0;
		pp[3*i+2]=Min.z+(Max.z-Min.z)*i/30.;
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
	}
	curv_plot(31,pp,tt);

	if(znum)	for(int i=0;i<znum;i++)
	{
		DrawZTick(zval[i],x0,y0,ddx,ddy);
		if(text)	Putsw(mglPoint(x0,y0,zval[i]),zstr[i],FontDef,-1,'z');
	}
	else if(ddz>0)	// ticks drawing
	{
		int kind=0;
		wchar_t s[32]=L"";
		if(!ytt) kind = _mgl_tick_ext(Max.z, Min.z, s, v);
		if(!TuneTicks)	kind = 0;

		NSz = NSz<0 ? 0 : NSz;
		z0 = isnan(OrgT.z) ? z0 : OrgT.z;
		z0 = z0 - ddz*floor((z0-Min.z)/ddz+1e-3);
		for(z=z0;z<=Max.z+(Max.z-Min.z)*1e-6;z+=ddz)
		{
			DrawZTick(z,x0,y0,ddx,ddy);
			if(text)
			{
				if(ztt)	swprintf(str,64,ztt,fabs(z)<ddz/100 ? 0 : z);
				else	_mgl_tick_text(z,z0,ddz/100,v,kind,str);
				wcstrim_mgl(str);
				Putsw(mglPoint(x0,y0,z),str,FontDef,-1,'z');
			}
		}
		if(text && (kind&2))
			Putsw(mglPoint(x0,y0,FactorPos*(Max.z-Min.z)+Min.z), s, FontDef, -1, 'z');
		if(NSz>0)
		{
			ddz /= (NSz+1);	// subticks drawing
			z0 = GetOrgZ('z');	z0 = isnan(OrgT.z) ? z0 : OrgT.z;
			z0 = z0 - ddz*floor((z0-Min.z)/ddz+1e-3);
			for(z=z0;z<=Max.z;z+=ddz)	DrawZTick(z,x0,y0,ddx,ddy,1);
		}
	}
	else if(Min.z>0)
	{
		int k=1+int(log10(Max.z/Min.z)/5), cur=0;
		z0 = exp(M_LN10*floor(0.1+log10(Min.z)));
		for(z=z0;z<=Max.z*FLT_EPS;z*=10)
		{
			if(z*FLT_EPS>=Min.z && z<=Max.z*FLT_EPS)
			{
				DrawZTick(z,x0,y0,ddx,ddy);
				swprintf(str,64,L"10^{%d}",int(floor(0.1+log10(z))));
				if(text && cur%k==0)
					Putsw(mglPoint(x0,y0,z),str,FontDef,-1,'z');
				cur++;
			}
			// subticks drawing
			for(v=z;v<10*z;v+=z)	if(v>Min.z && v<Max.z)
				DrawZTick(v,x0,y0,ddx,ddy,1);

		}
	}
}
//-----------------------------------------------------------------------------
void mglGraph::Axis(const char *dir, bool adjust)
{
	if(adjust)	AdjustTicks(dir);
	static int cgid=1;	StartGroup("Axis",cgid++);
	bool text = strchr(dir,'_')==NULL;
	if(strchr(dir,'x'))	{	_sx = 1;	AxisX(text);	}
	if(strchr(dir,'X'))	{	_sx = -1;	AxisX(text);	}
	if(strchr(dir,'y'))
	{	_sy = 1;	if(TernAxis)	AxisTY(text);	else	AxisY(text);	}
	if(strchr(dir,'Y'))
	{	_sy = -1;	if(TernAxis)	AxisTY(text);	else	AxisY(text);	}
	if(strchr(dir,'z'))
	{	_sz = 1;	if(TernAxis)	AxisTZ(text);	else	AxisZ(text);	}
	if(strchr(dir,'Z'))
	{	_sz = -1;	if(TernAxis)	AxisTZ(text);	else	AxisZ(text);	}
	EndGroup();
}
//-----------------------------------------------------------------------------
void mglGraph::TickBox()
{
	float x0,y0,z0,x,y,z,ddx,ddy,ddz;
	x0 = GetOrgX(0);	y0 = GetOrgY(0);	z0 = GetOrgZ(0);
	ddx = dx>=0 ? dx : mgl_okrugl((Min.x-Max.x)/(dx+1),3);
	ddy = dy>=0 ? dy : mgl_okrugl((Min.y-Max.y)/(dy+1),3);
	ddz = dz>=0 ? dz : mgl_okrugl((Min.z-Max.z)/(dz+1),3);
	if(xnum)	for(int i=0;i<xnum;i++)
	{
		x = xval[i];
		DrawXTick(x,Min.y,Min.z,ddy,ddz);
		DrawXTick(x,Min.y,Max.z,ddy,-ddz);
		DrawXTick(x,Max.y,Min.z,-ddy,ddz);
		DrawXTick(x,Max.y,Max.z,-ddy,-ddz);
	}
	else if(ddx>0)	// рисуем метки по х
	{
		x0 = isnan(OrgT.x) ? x0 : OrgT.x;
		x0 = x0 - ddx*floor((x0-Min.x)/ddx);
		for(x=x0;x<Max.x;x+=ddx)
		{
			DrawXTick(x,Min.y,Min.z,ddy,ddz);
			DrawXTick(x,Min.y,Max.z,ddy,-ddz);
			DrawXTick(x,Max.y,Min.z,-ddy,ddz);
			DrawXTick(x,Max.y,Max.z,-ddy,-ddz);
		}
	}
	else if(Min.x>0)
	{
		x0 = exp(M_LN10*floor(0.1+log10(Min.x)));
		for(x=x0;x<Max.x;x*=10)
		{
			if(x<Min.x)	continue;
			DrawXTick(x,Min.y,Min.z,ddy,ddz);
			DrawXTick(x,Min.y,Max.z,ddy,-ddz);
			DrawXTick(x,Max.y,Min.z,-ddy,ddz);
			DrawXTick(x,Max.y,Max.z,-ddy,-ddz);
		}
	}
	x0 = GetOrgX(0);
	if(ynum)	for(int i=0;i<ynum;i++)
	{
		y = yval[i];
		DrawYTick(y,Min.x,Min.z,ddx,ddz);
		DrawYTick(y,Min.x,Max.z,ddx,-ddz);
		DrawYTick(y,Max.x,Min.z,-ddx,ddz);
		DrawYTick(y,Max.x,Max.z,-ddx,-ddz);
	}
	else if(ddy>0)	// рисуем метки по y
	{
		y0 = isnan(OrgT.y) ? y0 : OrgT.y;
		y0 = y0 - ddy*floor((y0-Min.y)/ddy);
		for(y=y0;y<Max.y;y+=ddy)
		{
			DrawYTick(y,Min.x,Min.z,ddx,ddz);
			DrawYTick(y,Min.x,Max.z,ddx,-ddz);
			DrawYTick(y,Max.x,Min.z,-ddx,ddz);
			DrawYTick(y,Max.x,Max.z,-ddx,-ddz);
		}
	}
	else if(Min.y>0)
	{
		y0 = exp(M_LN10*floor(0.1+log10(Min.y)));
		for(y=y0;y<Max.y;y*=10)
		{
			if(y<Min.y)	continue;
			DrawYTick(y,Min.x,Min.z,ddx,ddz);
			DrawYTick(y,Min.x,Max.z,ddx,-ddz);
			DrawYTick(y,Max.x,Min.z,-ddx,ddz);
			DrawYTick(y,Max.x,Max.z,-ddx,-ddz);
		}
	}
	y0 = GetOrgY(0);
	if(znum)	for(int i=0;i<znum;i++)
	{
		z = zval[i];
		DrawZTick(z,Min.x,Min.y,ddx,ddy);
		DrawZTick(z,Min.x,Max.y,ddx,-ddy);
		DrawZTick(z,Max.x,Min.y,-ddx,ddy);
		DrawZTick(z,Max.x,Max.y,-ddx,-ddy);
	}
	else if(ddz>0)	// рисуем метки
	{
		z0 = isnan(OrgT.z) ? z0 : OrgT.z;
		z0 = z0 - ddz*floor((z0-Min.z)/ddz);
		for(z=z0;z<Max.z;z+=ddz)
		{
			DrawZTick(z,Min.x,Min.y,ddx,ddy);
			DrawZTick(z,Min.x,Max.y,ddx,-ddy);
			DrawZTick(z,Max.x,Min.y,-ddx,ddy);
			DrawZTick(z,Max.x,Max.y,-ddx,-ddy);
		}
	}
	else if(Min.z>0)
	{
		z0 = exp(M_LN10*floor(0.1+log10(Min.z)));
		for(z=z0;z<Max.z;z*=10)
		{
			if(z<Min.z)	continue;
			DrawZTick(z,Min.x,Min.y,ddx,ddy);
			DrawZTick(z,Min.x,Max.y,ddx,-ddy);
			DrawZTick(z,Max.x,Min.y,-ddx,ddy);
			DrawZTick(z,Max.x,Max.y,-ddx,-ddy);
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraph::Box(const char *col,bool ticks)
{
	mglColor c;
	if(!col || col[0]==0)	Box(NC,ticks);
	c.Set(col[0]);
	Box(c,ticks);
}
//-----------------------------------------------------------------------------
void mglGraph::Box(mglColor p,bool ticks)
{
	static int cgid=1;	StartGroup("Box",cgid++);
	float x1=Min.x,x2=Max.x,y1=Min.y,y2=Max.y,z1=Min.z,z2=Max.z;
	Arrow1 = Arrow2 = '_';
	if(p.Valid())	Pen(p,'-',BaseLineWidth);
	else			Pen(TranspType!=2 ? BC:WC,'-',BaseLineWidth);
	mglFormula *tt = fc;	fc = 0;
	Line(mglPoint(x1,y1,z1), mglPoint(x1,y1,z2), 0, 100);
	Line(mglPoint(x1,y2,z1), mglPoint(x1,y2,z2), 0, 100);
	Line(mglPoint(x2,y1,z1), mglPoint(x2,y1,z2), 0, 100);
	Line(mglPoint(x1,y1,z1), mglPoint(x1,y2,z1), 0, 100);
	Line(mglPoint(x1,y1,z1), mglPoint(x2,y1,z1), 0, 100);
	Line(mglPoint(x1,y1,z2), mglPoint(x1,y2,z2), 0, 100);
	Line(mglPoint(x1,y1,z2), mglPoint(x2,y1,z2), 0, 100);
	if(TernAxis)
	{
		Line(mglPoint(x1,y2,z1), mglPoint(x2,y1,z1), 0, 100);
		Line(mglPoint(x1,y2,z2), mglPoint(x2,y1,z2), 0, 100);
	}
	else
	{
		Line(mglPoint(x2,y2,z1), mglPoint(x2,y2,z2), 0, 100);
		Line(mglPoint(x1,y2,z1), mglPoint(x2,y2,z1), 0, 100);
		Line(mglPoint(x2,y1,z1), mglPoint(x2,y2,z1), 0, 100);
		Line(mglPoint(x1,y2,z2), mglPoint(x2,y2,z2), 0, 100);
		Line(mglPoint(x2,y1,z2), mglPoint(x2,y2,z2), 0, 100);
	}
	if(ticks && !TernAxis)	TickBox();
	fc = tt;
	EndGroup();
}
//-----------------------------------------------------------------------------
void mglGraph::AddLegend(const wchar_t *text,const char *style)
{
	if(!text)	return;
	if(NumLeg>=100)	{	SetWarn(mglWarnLegA);	return;	}
	LegStr[NumLeg] = mgl_wcsdup(text);
	if(style)	LegStl[NumLeg] = mgl_strdup(style);
	else		LegStl[NumLeg] = mgl_strdup(last_style);
	NumLeg++;
}
//-----------------------------------------------------------------------------
void mglGraph::Legend(float x, float y, const char *font, float size, float llen)
{	Legend(NumLeg,LegStr,LegStl,x,y,font,size,llen);	}
//-----------------------------------------------------------------------------
void mglGraph::Legend(int where, const char *font, float size, float llen)
{	Legend(NumLeg,LegStr,LegStl,where,font,size,llen);	}
//-----------------------------------------------------------------------------
void mglGraph::ClearLegend()
{
	for(long i=0;i<NumLeg;i++)	{	free(LegStr[i]);	free(LegStl[i]);	}
	NumLeg = 0;
}
//-----------------------------------------------------------------------------
void mglGraph::Legend(int n, wchar_t **text,char **style, float x, float y,
					const char *font, float size, float llen)
{
	if(n<1)	{	SetWarn(mglWarnLeg);	return;	}
	if(isnan(llen))	llen=0.1;
	static int cgid=1;	StartGroup("Legend",cgid++);
	float pp[15], r=GetRatio(), rh, rw;
	if(size<=0)	size = -size*FontSize;

	rh=(r<1?r:1.)*size/12.*(Max.y-Min.y);
	rw=(r>1?1./r:1.)*size/16.;
	float w=0, h=fnt->Height(font)*rh, j, dx=Max.x-Min.x;
	register long i;
	for(i=0;i<n;i++)
	{
		j = fnt->Width(text[i],font)*rw;
		if(style[i][0]==0)	j -= llen;
		w = w>j ? w:j;
	}
	w = (w + llen*1.1f)*dx;	// запас на линию

	if(LegendBox)
	{
		pp[2] = pp[5] = pp[8] = pp[11] = pp[14] = Max.z;
		pp[0] = pp[9] = pp[12] = x;		pp[3] = pp[6] = x+w;
		pp[1] = pp[4] = pp[13] = y-0.*h;		pp[7] = pp[10] = y+h*n;
		for(i=0;i<5;i++)	ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
		SelectPen(TranspType!=2 ? "k-1":"w-1");
		curv_plot(5,pp,0);	// bounding rectangle
		pp[2] = pp[5] = pp[8] = pp[11] = Max.z;
		pp[0] = pp[6] = x;			pp[3] = pp[9] = x+w;
		pp[1] = pp[4] = y-0.*h;		pp[7] = pp[10] = y+h*n;
		DefColor(mglColor(1,1,1),1);
		for(i=0;i<4;i++)	ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
		surf_plot(2,2,pp,0,0);		// white rectangle below it
	}

	for(i=0;i<n;i++)
	{
		char m=SelectPen(style[i]);
		pp[2] = pp[5] = pp[8] = Max.z;
		pp[1] = pp[4] = pp[7] = y+i*h+0.45f*h;
		pp[0] = x+0.1f*llen*dx;	pp[3] = x+0.9f*llen*dx;	pp[6] = x+0.5f*llen*dx;
		ScalePoint(pp[0],pp[1],pp[2]);
		ScalePoint(pp[3],pp[4],pp[5]);
		ScalePoint(pp[6],pp[7],pp[8]);

		curv_plot(2,pp,0);
		if(m)	Mark(pp[6],pp[7],pp[8],m);
		SelectPen(TranspType!=2 ? "k-1":"w-1");
		Putsw(mglPoint(x+(style[i][0]!=0?llen:0)*dx, y+i*h+0.3f*h, Max.z),text[i], font, size,'n');
	}
	EndGroup();
}
//-----------------------------------------------------------------------------
void mglGraph::Legend(int n, wchar_t **text,char **style, int where,
					const char *font, float size, float llen)
{
	if(n<1)	{	SetWarn(mglWarnLeg);	return;	}
	if(isnan(llen))	llen=0.1;
	if(size<0)	size = -size*FontSize;
	float w=0, r=GetRatio(), rh, rw;

	rh=(r<1?r:1.)*size/12.*(Max.y-Min.y);	// 12 = 16/1.4
	rw=(r>1?1./r:1.)*size/16.;
	float h=fnt->Height(font)*rh, x, y, dx = 0.03*(Max.x-Min.x), dy = 0.03*(Max.y-Min.y);
	for(long i=0;i<n;i++)
	{
		x = fnt->Width(text[i],font)*rw;
		if(style[i][0]==0)	x -= llen;
		w = w>x ? w:x;
	}
	w = (w + 1.1f*llen)*(Max.x-Min.x);	// запас на линию

	switch(where)
	{
	case 0:		x = Min.x+dx;	y = Min.y+dy;		break;
	case 1:		x = Max.x-w-dx;	y = Min.y+dy;		break;
	case 2:		x = Min.x+dx;	y = Max.y-h*n-dy;	break;
	default:	x = Max.x-w-dx;	y = Max.y-h*n-dy;	break;
	}
	Legend(n,text,style,x,y,font,size,llen);
}
//-----------------------------------------------------------------------------
void mglGraph::Ternary(bool t)
{
	static mglPoint x1(-1,-1,-1),x2(1,1,1),o(NAN,NAN,NAN);
	TernAxis = t;
	if(t)
	{
		x1 = Min;	x2 = Max;	o = Org;
		Cut = false;	OrgT = mglPoint(NAN,NAN,NAN);
		Axis(mglPoint(0,0,0),mglPoint(1,1,1),mglPoint(0,0,0));
	}
	else	Axis(x1,x2,o);
}
//-----------------------------------------------------------------------------
void mglGraph::AxisTY(bool text)
{
	float pp[3*31];
	bool tt[31];
	wchar_t str[64];
	long i;
	float y0,z0,y,ddx,ddy,ddz;
	y0 = GetOrgY('y');	z0 = GetOrgZ('y');
	ddx = dx>=0 ? dx : mgl_okrugl((Min.x-Max.x)/(dx+1),3);
	ddy = dy>=0 ? dy : mgl_okrugl((Min.y-Max.y)/(dy+1),3);
	ddz = dz>=0 ? dz : mgl_okrugl((Min.z-Max.z)/(dz+1),3);
	if(ddx==0)	ddx=(Max.x-Min.x)/2;
//	if(ddy==0)	ddy=(Max.y-Min.y)/2;
	if(ddz==0)	ddz=(Max.z-Min.z)/2;
	SelectPen(TranspType!=2 ? "k-1":"w-1");
	for(i=0;i<31;i++)	// сама ось
	{
		pp[3*i]=Max.x+(Min.x-Max.x)*i/30.;
		pp[3*i+2]=z0;
		pp[3*i+1]=Min.y+(Max.y-Min.y)*i/30.;
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
	}
	curv_plot(31,pp,tt);

	if(ddy>0)	// ticks drawing
	{
		NSy = NSy<0 ? 0 : NSy;
		for(y=0;y<=Max.y+(Max.y-Min.y)*1e-6;y+=ddy)
		{
			pp[0]=Max.x-y-ddx/10;pp[1]=y;	pp[2]=z0;
			pp[3]=Max.x-y;		pp[4]=y;	pp[5]=z0;
			pp[6]=Max.x-y;		pp[7]=y;	pp[8]=z0+ddz/10;
			if(y>Min.y && y<Max.y)	DrawTick(pp,false);
			swprintf(str,64,L"%.2g",y);		wcstrim_mgl(str);
			if(text)	Putsw(mglPoint(Max.x-y,y,z0),str,FontDef,FontSize,'y');
		}
		if(NSy>0)
		{
			ddy /= (NSy+1);	// subticks drawing
			for(y=0;y<=Max.y;y+=ddy)
			{
				pp[0]=Max.x-y-ddx/14;pp[1]=y;	pp[2]=z0;
				pp[3]=Max.x-y;		pp[4]=y;	pp[5]=z0;
				pp[6]=Max.x-y;		pp[7]=y;	pp[8]=z0+ddz/14;
				DrawTick(pp,true);
			}
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraph::AxisTZ(bool text)
{
	float pp[3*31];
	bool tt[31];
	wchar_t str[64];
	long i;
	float x0,z0,z,ddx,ddy,ddz;
	x0 = GetOrgX('t');	z0 = GetOrgZ('t');
	ddx = dx>=0 ? dx : mgl_okrugl((Min.x-Max.x)/(dx+1),3);
	ddy = dy>=0 ? dy : mgl_okrugl((Min.y-Max.y)/(dy+1),3);
	ddz = dz>=0 ? dz : mgl_okrugl((Min.z-Max.z)/(dz+1),3);
	if(ddx==0)	ddx=(Max.x-Min.x)/2;
	if(ddy==0)	ddy=(Max.y-Min.y)/2;
//	if(ddz==0)	ddz=(Max.z-Min.z)/2;
	SelectPen(TranspType!=2 ? "k-1":"w-1");
	for(i=0;i<31;i++)	// сама ось
	{
		pp[3*i]=x0;
		pp[3*i+1]=Max.y+(Min.y-Max.y)*i/30.;
		pp[3*i+2]=z0;
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
	}
	curv_plot(31,pp,tt);

	if(ddz>0)	// ticks drawing
	{
		NSz = NSz<0 ? 0 : NSz;
//		z0 = z0 - ddz*floor((z0-Min.z)/ddz+1e-3);
		for(z=0;z<=Max.z+(Max.z-Min.z)*1e-6;z+=ddz)
		{
			pp[0]=x0+ddx/10;	pp[1]=Max.y-z-ddy/10;pp[2]=z0;
			pp[3]=x0;			pp[4]=Max.y-z;		pp[5]=z0;
			pp[6]=x0+ddx/10;	pp[7]=Max.y-z;		pp[8]=z0;
			if(z>Min.z && z<Max.z)	DrawTick(pp,false);
			swprintf(str, 64, L"%.2g",z);	wcstrim_mgl(str);
			if(text)	Putsw(mglPoint(x0,Max.y-z,z0),str,FontDef,FontSize,'z');
		}
		if(NSz>0)
		{
			ddz /= (NSz+1);	// subticks drawing
			for(z=0;z<=Max.z;z+=ddz)
			{
				pp[0]=x0+ddx/14;	pp[1]=Max.y-z-ddy/14;	pp[2]=z0;
				pp[3]=x0;	pp[4]=Max.y-z;			pp[5]=z0;
				pp[6]=x0+ddx/14;	pp[7]=Max.y-z;	pp[8]=z0;
				DrawTick(pp,true);
			}
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraph::SetTicks(char dir, float d, int ns, float org)
{
	if(dir=='x')
	{	dx = d;	NSx = ns;	OrgT.x = org;	if(xnum) delete []xbuf;	xnum=0;	}
	else if(dir=='y')
	{	dy = d;	NSy = ns;	OrgT.y = org;	if(ynum) delete []ybuf;	ynum=0;	}
	else if(dir=='z')
	{	dz = d;	NSz = ns;	OrgT.z = org;	if(znum) delete []zbuf;	znum=0;	}
}
//-----------------------------------------------------------------------------
void mglGraph::SetTicksVal(char dir, int n, float *val, const wchar_t **lbl)
{
	long len=0;
	register int i;
	if(dir=='x')
	{
		if(xnum)	delete []xbuf;
		if(n<1 || !val || !lbl)	{	xnum=0;	return;	}
		if(n>50)	n=50;
		for(i=0;i<n;i++)	len += wcslen(lbl[i])+1;
		xbuf = new wchar_t[len];	xnum = n;	len = 0;
		for(i=0;i<n;i++)
		{
			wcscpy(xbuf+len, lbl[i]);	xval[i] = val[i];
			xstr[i] = xbuf+len;			len += wcslen(lbl[i])+1;
		}
	}
	else if(dir=='y')
	{
		if(ynum)	delete []ybuf;
		if(n<1 || !val || !lbl)	{	ynum=0;	return;	}
		if(n>50)	n=50;
		for(i=0;i<n;i++)	len += wcslen(lbl[i])+1;
		ybuf = new wchar_t[len];	ynum = n;	len = 0;
		for(i=0;i<n;i++)
		{
			wcscpy(ybuf+len, lbl[i]);	yval[i] = val[i];
			ystr[i] = ybuf+len;			len += wcslen(lbl[i])+1;
		}
	}
	else if(dir=='z')
	{
		if(znum)	delete []zbuf;
		if(n<1 || !val || !lbl)	{	znum=0;	return;	}
		if(n>50)	n=50;
		for(i=0;i<n;i++)	len += wcslen(lbl[i])+1;
		zbuf = new wchar_t[len];	znum = n;	len = 0;
		for(i=0;i<n;i++)
		{
			wcscpy(zbuf+len, lbl[i]);	zval[i] = val[i];
			zstr[i] = zbuf+len;			len += wcslen(lbl[i])+1;
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraph::SetTicksVal(char dir, int n, float *val, const char **lbl)
{
	long len=0;
	register int i,ll;
	if(dir=='x')
	{
		if(xnum)	delete []xbuf;
		if(n<1 || !val || !lbl)	{	xnum=0;	return;	}
		if(n>50)	n=50;
		for(i=0;i<n;i++)	len += strlen(lbl[i])+1;
		xbuf = new wchar_t[len];	xnum = n;	len = 0;
		for(i=0;i<n;i++)
		{
			ll = strlen(lbl[i])+1;		xval[i] = val[i];
			mbstowcs(xbuf+len,lbl[i],ll);	xstr[i] = xbuf+len;
			len += ll;
		}
	}
	if(dir=='y')
	{
		if(ynum)	delete []ybuf;
		if(n<1 || !val || !lbl)	{	ynum=0;	return;	}
		if(n>50)	n=50;
		for(i=0;i<n;i++)	len += strlen(lbl[i])+1;
		ybuf = new wchar_t[len];	ynum = n;	len = 0;
		for(i=0;i<n;i++)
		{
			ll = strlen(lbl[i])+1;		yval[i] = val[i];
			mbstowcs(ybuf+len,lbl[i],ll);	ystr[i] = ybuf+len;
			len += ll;
		}
	}
	if(dir=='z')
	{
		if(znum)	delete []zbuf;
		if(n<1 || !val || !lbl)	{	znum=0;	return;	}
		if(n>50)	n=50;
		for(i=0;i<n;i++)	len += strlen(lbl[i])+1;
		zbuf = new wchar_t[len];	znum = n;	len = 0;
		for(i=0;i<n;i++)
		{
			ll = strlen(lbl[i])+1;		zval[i] = val[i];
			mbstowcs(zbuf+len,lbl[i],ll);	zstr[i] = zbuf+len;
			len += ll;
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraph::SetTicksVal(char dir, int n, double val, const char *lbl, ...)
{
	if(n<1)	return;
	float *v = new float[n];
	const char **l = (const char **)malloc(n*sizeof(const char *));
	v[0] = val;	l[0] = lbl;
	va_list ap;
	va_start(ap, lbl);
	for(int i=1;i<n;i++)
	{
		v[i] = va_arg(ap, double);
		l[i] = va_arg(ap, char *);
	}
	SetTicksVal(dir,n,v,l);
	va_end(ap);
	delete []v;		free(l);
}
//-----------------------------------------------------------------------------
void mgl_set_ticks_val(HMGL gr, char dir, int n, double val, const char *lbl, ...)
{	// NOTE: I have to repeat the function because I don't know how to pass variable arguments to C++ function
	if(n<1)	return;
	float *v = new float[n];
	const char **l = (const char **)malloc(n*sizeof(const char *));
	v[0] = val;	l[0] = lbl;
	va_list ap;
	va_start(ap, lbl);
	for(int i=1;i<n;i++)
	{
		v[i] = va_arg(ap, double);
		l[i] = va_arg(ap, char *);
	}
	gr->SetTicksVal(dir,n,v,l);
	va_end(ap);
	delete []v;		free(l);
}
//-----------------------------------------------------------------------------
void mglGraph::adjust(char dir, float d)
{
	float n;
	d = fabs(d);	n = floor(log10(d));	d = floor(d*pow(10,-n));
	if(d<4)		SetTicks(dir,0.5*pow(10,n),4,0);
	else if(d<7)SetTicks(dir,pow(10,n),4,0);
	else		SetTicks(dir,2*pow(10,n),3,0);
}
void mglGraph::AdjustTicks(const char *dir)
{
	if(strchr(dir,'x') || strchr(dir,'X'))	adjust('x',Max.x-Min.x);
	if(strchr(dir,'y') || strchr(dir,'Y'))	adjust('y',Max.y-Min.y);
	if(strchr(dir,'z') || strchr(dir,'Z'))	adjust('z',Max.z-Min.z);
	TuneTicks = true;
}
//-----------------------------------------------------------------------------
