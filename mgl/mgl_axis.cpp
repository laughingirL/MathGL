/* mgl_axis.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <mathgl.abalakin@gmail.com>
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
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <ctype.h>
#include "mgl/mgl_eval.h"
#include "mgl/mgl.h"
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
	bool tt[3];
	float d1,d2,f=sub?0.02:0.04;
	tt[0]=ScalePoint(pp[0],pp[1],pp[2]);
	tt[1]=ScalePoint(pp[3],pp[4],pp[5]);
	tt[2]=ScalePoint(pp[6],pp[7],pp[8]);
	pp[0] -= pp[3];		pp[1] -= pp[4];		pp[2] -= pp[5];
	pp[6] -= pp[3];		pp[7] -= pp[4];		pp[8] -= pp[5];
	d1 = sqrt(pp[0]*pp[0] + pp[1]*pp[1] + pp[2]*pp[2]);
	d2 = sqrt(pp[6]*pp[6] + pp[7]*pp[7] + pp[8]*pp[8]);
	d1 = (d1>0 ? d1:1)/f;	d2 = (d2>0 ? d2:1)/f;
	pp[0] = pp[3] + pp[0]/d1;
	pp[1] = pp[4] + pp[1]/d1;
	pp[2] = pp[5] + pp[2]/d1;
	pp[6] = pp[3] + pp[6]/d2;
	pp[7] = pp[4] + pp[7]/d2;
	pp[8] = pp[5] + pp[8]/d2;
	curv_plot(3,pp,0);
}
//-----------------------------------------------------------------------------
void mglGraph::Grid(const char *dir, const char *pen)
{
	float x0,y0,z0,ddx,ddy,ddz,t,pp[3*31];
	bool tt[31];
	long i;
	if(pen)	SelectPen(pen);
	else	SelectPen(TranspType!=2 ? "B;1" : "b;1");
	ddx = dx>=0 ? dx : mgl_okrugl((Min.x-Max.x)/(dx+1),3);
	ddy = dy>=0 ? dy : mgl_okrugl((Min.y-Max.y)/(dy+1),3);
	ddz = dz>=0 ? dz : mgl_okrugl((Min.z-Max.z)/(dz+1),3);
	if(strchr(dir,'x') && dx!=0)
	{
		x0 = GetOrgX();	y0 = GetOrgY();	z0 = GetOrgZ();
		x0 = isnan(TOrg.x) ? x0 : TOrg.x;
		x0 = x0 - ddx*floor((x0-Min.x)/ddx);
		for(t=x0;t<=Max.x;t+=ddx)
		{
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
	}
	if(strchr(dir,'x') && dx==0 && Min.x>0)
	{
		y0 = GetOrgY();	z0 = GetOrgZ();
		x0 = exp(M_LN10*floor(0.1+log10(Min.x)));
		for(t=x0;t<=Max.x*FLT_EPS;t*=10)
		{
			for(i=0;i<31;i++)
			{
				pp[3*i]=t;
				pp[3*i+1]=y0;
				pp[3*i+2]=Min.z+(Max.z-Min.z)*i/30.;
				tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
			}
			curv_plot(31,pp,tt);
			for(i=0;i<31;i++)
			{
				pp[3*i]=t;	pp[3*i+2]=z0;
				pp[3*i+1]=Min.y+(Max.y-Min.y)*i/30.;
				tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
			}
			curv_plot(31,pp,tt);
		}
	}
	if(strchr(dir,'y') && dy!=0)
	{
		x0 = GetOrgX();	y0 = GetOrgY();	z0 = GetOrgZ();
		y0 = isnan(TOrg.y) ? y0 : TOrg.y;
		y0 = y0 - ddy*floor((y0-Min.y)/ddy);
		for(t=y0;t<=Max.y;t+=ddy)
		{
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
	}
	if(strchr(dir,'y') && dy==0 && Min.y>0)
	{
		x0 = GetOrgX();	z0 = GetOrgZ();
		y0 = exp(M_LN10*floor(0.1+log10(Min.y)));
		for(t=y0;t<=Max.y*FLT_EPS;t*=10)
		{
			for(i=0;i<31;i++)
			{
				pp[3*i]=x0;	pp[3*i+1]=t;
				pp[3*i+2]=Min.z+(Max.z-Min.z)*i/30.;
				tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
			}
			curv_plot(31,pp,tt);
			for(i=0;i<31;i++)
			{
				pp[3*i+2]=z0;	pp[3*i+1]=t;
				pp[3*i]=Min.x+(Max.x-Min.x)*i/30.;
				tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
			}
			curv_plot(31,pp,tt);
		}
	}
	if(strchr(dir,'z') && dz!=0)
	{
		x0 = GetOrgX();	y0 = GetOrgY();	z0 = GetOrgZ();
		z0 = isnan(TOrg.z) ? z0 : TOrg.z;
		z0 = z0 - ddz*floor((z0-Min.z)/ddz);
		for(t=z0;t<=Max.z;t+=ddz)
		{
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
	}
	if(strchr(dir,'z') && dz==0 && Min.z>0)
	{
		x0 = GetOrgX();	y0 = GetOrgY();
		z0 = exp(M_LN10*floor(0.1+log10(Min.z)));
		for(t=z0;t<=Max.z*FLT_EPS;t*=10)
		{
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
	}
	SelectPen("k-1");
}
//-----------------------------------------------------------------------------
void mglGraph::Label(char dir,const wchar_t *text,int pos,float size,float shift)
{
	float t, x0, y0, z0;
	SelectPen("k-1");
	x0 = GetOrgX();	y0 = GetOrgY();	z0 = GetOrgZ();
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
			kind=3;		v=ipow(10,k);
			swprintf(s, 32, L"(@{\\times{}10^{%d}})", k);
		}
		if(v<1e-2f)
		{
			int k=int(log10(v)-0.01)-1;
			kind=3;		v=ipow(10,k);
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
			v=ipow(10,k);
			swprintf(s, 32, L"\\times 10^{%d}", k);
		}
		if(v<1e-2f)
		{
			kind = 2;
			int k=int(log10(v)-0.01)-1;
			v=ipow(10,k);
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
		n1=swprintf(str, 64, L"@{(+%.2g)}",u);
		n2=swprintf(str, 64, L"@{(+%g)}",u);
		if(n1<n2)	swprintf(str, 64, L"@{(+%.2g)}",u);
	}
	else
	{
		int n1,n2;
		n1=swprintf(str, 64, L"%.2g",u);
		n2=swprintf(str, 64, L"%g",u);
		if(n1<n2)	swprintf(str, 64, L"%.2g",u);
	}
}
//-----------------------------------------------------------------------------
void mglGraph::AxisX(bool text)
{
	float pp[3*31];
	bool tt[31];
	wchar_t str[64];
	long i;
	float x0,y0,z0,x,ddx,ddy,ddz;
	x0 = GetOrgX();	y0 = GetOrgY();	z0 = GetOrgZ();
	ddx = dx>=0 ? dx : mgl_okrugl((Min.x-Max.x)/(dx+1),3);
	ddy = dy>=0 ? dy : mgl_okrugl((Min.y-Max.y)/(dy+1),3);
	ddz = dz>=0 ? dz : mgl_okrugl((Min.z-Max.z)/(dz+1),3);
//	if(ddx==0)	ddx=(Max.x-Min.x)/2;
	if(ddy==0)	ddy=(Max.y-Min.y)/2;
	if(ddz==0)	ddz=(Max.z-Min.z)/2;
	SelectPen(TranspType!=2 ? "k-1":"w-1");
	for(i=0;i<31;i++)	// сама ось
	{
		pp[3*i+1]=y0;	pp[3*i+2]=z0;
		pp[3*i]=Min.x+(Max.x-Min.x)*i/30.;
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
	}
	curv_plot(31,pp,tt);

	if(ddx>0)	// ticks drawing
	{
		float v=0;
		int kind=0;
		wchar_t s[32]=L"";
		if(!xtt) kind = _mgl_tick_ext(Max.x, Min.x, s, v);
		if(!TuneTicks)	kind = 0;

		NSx = NSx<0 ? 0 : NSx;
		x0 = isnan(TOrg.x) ? x0 : TOrg.x;
		x0 = x0 - ddx*floor((x0-Min.x)/ddx+1e-3);
		for(x=x0;x<=Max.x+(Max.x-Min.x)*1e-6;x+=ddx)
		{
			pp[0]=x;	pp[1]=y0+ddy/7;	pp[2]=z0;
			pp[3]=x;	pp[4]=y0;	pp[5]=z0;
			pp[6]=x;	pp[7]=y0;	pp[8]=z0+ddz/7;
			DrawTick(pp,false);
			if(text)
			{
				bool func = false;
				if(TickStr)		func = TickStr('x', x, str);
				if(!func)
				{
					if(xtt)	swprintf(str, 64, xtt, fabs(x)<ddx/100 ? 0 : x);
					else	_mgl_tick_text(x,x0,ddx/100,v,kind,str);
					wcstrim_mgl(str);
				}
				Putsw(mglPoint(x,y0,z0),str,FontDef,FontSize,'x');
			}
		}
		if(text && (kind&2))
		{
			bool func = false;
			if(TickStr)		func = TickStr('x', NAN, str);
			Putsw(mglPoint(FactorPos*(Max.x-Min.x)+Min.x,y0,z0),func?str:s,FontDef,FontSize,'x');
		}
		if(NSx>0)
		{
			ddx /= (NSx+1);	// subticks drawing
			x0 = GetOrgX();	x0 = x0 - ddx*floor((x0-Min.x)/ddx);
			for(x=x0;x<=Max.x;x+=ddx)
			{
				pp[0]=x;	pp[1]=y0+ddy/14;	pp[2]=z0;
				pp[3]=x;	pp[4]=y0;			pp[5]=z0;
				pp[6]=x;	pp[7]=y0;			pp[8]=z0+ddz/14;
				DrawTick(pp,true);
			}
		}
	}
	else if(Min.x>0)
	{
		float xx;
		int k=1+int(log10(Max.x/Min.x)/5), cur=0;
		x0 = exp(M_LN10*floor(0.1+log10(Min.x)));
		for(x=x0;x<=Max.x*FLT_EPS;x*=10.f)
		{
			pp[0]=x;	pp[1]=y0+ddy/7;	pp[2]=z0;
			pp[3]=x;	pp[4]=y0;	pp[5]=z0;
			pp[6]=x;	pp[7]=y0;	pp[8]=z0+ddz/7;
			if(x*FLT_EPS>=Min.x && x<=Max.x*FLT_EPS)
			{
				DrawTick(pp,false);
				swprintf(str,64,L"10^{%d}",int(floor(0.1+log10(x))));
				if(text && cur%k==0)
					Putsw(mglPoint(x,y0,z0),str,FontDef,FontSize,'x');
				cur++;
			}
			// subticks drawing
			for(xx=x;xx<10*x;xx+=x)
			{
				if(xx<Min.x || xx>Max.x)	continue;
				pp[0]=xx;	pp[1]=y0+ddy/14;	pp[2]=z0;
				pp[3]=xx;	pp[4]=y0;			pp[5]=z0;
				pp[6]=xx;	pp[7]=y0;			pp[8]=z0+ddz/14;
				DrawTick(pp,true);
			}
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraph::AxisY(bool text)
{
	float pp[3*31];
	bool tt[31];
	wchar_t str[64];
	long i;
	float x0,y0,z0,y,ddx,ddy,ddz;
	x0 = GetOrgX();	y0 = GetOrgY();	z0 = GetOrgZ();
	ddx = dx>=0 ? dx : mgl_okrugl((Min.x-Max.x)/(dx+1),3);
	ddy = dy>=0 ? dy : mgl_okrugl((Min.y-Max.y)/(dy+1),3);
	ddz = dz>=0 ? dz : mgl_okrugl((Min.z-Max.z)/(dz+1),3);
	if(ddx==0)	ddx=(Max.x-Min.x)/2;
//	if(ddy==0)	ddy=(Max.y-Min.y)/2;
	if(ddz==0)	ddz=(Max.z-Min.z)/2;
	SelectPen(TranspType!=2 ? "k-1":"w-1");
	for(i=0;i<31;i++)	// сама ось
	{
		pp[3*i]=x0;	pp[3*i+2]=z0;
		pp[3*i+1]=Min.y+(Max.y-Min.y)*i/30.;
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
	}
	curv_plot(31,pp,tt);

	if(ddy>0)	// ticks drawing
	{
		float v=0;
		int kind=0;
		wchar_t s[32]=L"";
		if(!ytt) kind = _mgl_tick_ext(Max.y, Min.y, s, v);
		if(!TuneTicks)	kind = 0;

		NSy = NSy<0 ? 0 : NSy;
		y0 = isnan(TOrg.y) ? y0 : TOrg.y;
		y0 = y0 - ddy*floor((y0-Min.y)/ddy+1e-3);
		for(y=y0;y<=Max.y+(Max.y-Min.y)*1e-6;y+=ddy)
		{
			pp[0]=x0+ddx/7;	pp[1]=y;	pp[2]=z0;
			pp[3]=x0;		pp[4]=y;	pp[5]=z0;
			pp[6]=x0;		pp[7]=y;	pp[8]=z0+ddz/7;
			DrawTick(pp,false);
			if(text)
			{
				bool func = false;
				if(TickStr)	func = TickStr('y', y, str);
				if(!func)
				{
					if(ytt)	swprintf(str, 64, ytt, fabs(y)<ddy/100 ? 0 : y);
					else	_mgl_tick_text(y,y0,ddy/100,v,kind,str);
					wcstrim_mgl(str);
				}
				Putsw(mglPoint(x0,y,z0),str,FontDef,FontSize,'y');
			}
		}
		if(text && (kind&2))
		{
			bool func = false;
			if(TickStr)		func = TickStr('y', NAN, str);
			Putsw(mglPoint(x0,FactorPos*(Max.y-Min.y)+Min.y,z0),func?str:s,FontDef,FontSize,'y');
		}
		if(NSy>0)
		{
			ddy /= (NSy+1);	// subticks drawing
			y0 = GetOrgY();	y0 = y0 - ddy*floor((y0-Min.y)/ddy);
			for(y=y0;y<=Max.y;y+=ddy)
			{
				pp[0]=x0+ddx/14;pp[1]=y;	pp[2]=z0;
				pp[3]=x0;		pp[4]=y;	pp[5]=z0;
				pp[6]=x0;		pp[7]=y;	pp[8]=z0+ddz/14;
				DrawTick(pp,true);
			}
		}
	}
	else if(Min.y>0)
	{
		float yy;
		int k=1+int(log10(Max.y/Min.y)/5), cur=0;
		y0 = exp(M_LN10*floor(0.1+log10(Min.y)));
		for(y=y0;y<=Max.y*FLT_EPS;y*=10)
		{
			pp[0]=x0+ddx/7;	pp[1]=y;	pp[2]=z0;
			pp[3]=x0;		pp[4]=y;	pp[5]=z0;
			pp[6]=x0;		pp[7]=y;	pp[8]=z0+ddz/7;
			if(y*FLT_EPS>=Min.y && y<=Max.y*FLT_EPS)
			{
				DrawTick(pp,false);
				swprintf(str,64,L"10^{%d}",int(floor(0.1+log10(y))));
				if(text && cur%k==0)
					Putsw(mglPoint(x0,y,z0),str,FontDef,FontSize,'y');
				cur++;
			}
			// subticks drawing
			for(yy=y;yy<10*y;yy+=y)
			{
				if(yy<Min.y || yy>Max.y)	continue;
				pp[0]=x0+ddx/14;pp[1]=yy;	pp[2]=z0;
				pp[3]=x0;		pp[4]=yy;	pp[5]=z0;
				pp[6]=x0;		pp[7]=yy;	pp[8]=z0+ddz/14;
				DrawTick(pp,false);
			}
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraph::AxisZ(bool text)
{
	float pp[3*31];
	bool tt[31];
	wchar_t str[64];
	long i;
	float x0,y0,z0,z,ddx,ddy,ddz;
	x0 = GetOrgX();	y0 = GetOrgY();	z0 = GetOrgZ();
	ddx = dx>=0 ? dx : mgl_okrugl((Min.x-Max.x)/(dx+1),3);
	ddy = dy>=0 ? dy : mgl_okrugl((Min.y-Max.y)/(dy+1),3);
	ddz = dz>=0 ? dz : mgl_okrugl((Min.z-Max.z)/(dz+1),3);
	if(ddx==0)	ddx=(Max.x-Min.x)/2;
	if(ddy==0)	ddy=(Max.y-Min.y)/2;
//	if(ddz==0)	ddz=(Max.z-Min.z)/2;
	SelectPen(TranspType!=2 ? "k-1":"w-1");
	for(i=0;i<31;i++)	// сама ось
	{
		pp[3*i]=x0;	pp[3*i+1]=y0;
		pp[3*i+2]=Min.z+(Max.z-Min.z)*i/30.;
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]);
	}
	curv_plot(31,pp,tt);

	if(ddz>0)	// ticks drawing
	{
		float v=0;
		int kind=0;
		wchar_t s[32]=L"";
		if(!ytt) kind = _mgl_tick_ext(Max.z, Min.z, s, v);
		if(!TuneTicks)	kind = 0;

		NSz = NSz<0 ? 0 : NSz;
		z0 = isnan(TOrg.z) ? z0 : TOrg.z;
		z0 = z0 - ddz*floor((z0-Min.z)/ddz+1e-3);
		for(z=z0;z<=Max.z+(Max.z-Min.z)*1e-6;z+=ddz)
		{
			pp[0]=x0;	pp[1]=y0+ddy/7;	pp[2]=z;
			pp[3]=x0;	pp[4]=y0;		pp[5]=z;
			pp[6]=x0+ddx/7;	pp[7]=y0;	pp[8]=z;
			DrawTick(pp,false);
			if(text)
			{
				bool func = false;
				if(TickStr)	func = TickStr('z', z, str);
				if(!func)
				{
					if(ztt)	swprintf(str,64,ztt,fabs(z)<ddz/100 ? 0 : z);
					else	_mgl_tick_text(z,z0,ddz/100,v,kind,str);
					wcstrim_mgl(str);
				}
				Putsw(mglPoint(x0,y0,z),str,FontDef,FontSize,'z');
			}
		}
		if(text && (kind&2))
		{
			bool func = false;
			if(TickStr)		func = TickStr('z', NAN, str);
			Putsw(mglPoint(x0,y0,FactorPos*(Max.z-Min.z)+Min.z),func?str:s,FontDef,FontSize,'z');
		}
		if(NSz>0)
		{
			ddz /= (NSz+1);	// subticks drawing
			z0 = GetOrgZ();	z0 = z0 - ddz*floor((z0-Min.z)/ddz);
			for(z=z0;z<=Max.z;z+=ddz)
			{
				pp[0]=x0;	pp[1]=y0+ddy/14;	pp[2]=z;
				pp[3]=x0;	pp[4]=y0;			pp[5]=z;
				pp[6]=x0+ddx/14;	pp[7]=y0;	pp[8]=z;
				DrawTick(pp,true);
			}
		}
	}
	else if(Min.z>0)
	{
		float zz;
		int k=1+int(log10(Max.z/Min.z)/5), cur=0;
		z0 = exp(M_LN10*floor(0.1+log10(Min.z)));
		for(z=z0;z<=Max.z*FLT_EPS;z*=10)
		{
			pp[0]=x0;	pp[1]=y0+ddy/7;	pp[2]=z;
			pp[3]=x0;	pp[4]=y0;		pp[5]=z;
			pp[6]=x0+ddx/7;	pp[7]=y0;	pp[8]=z;
			if(z*FLT_EPS>=Min.z && z<=Max.z*FLT_EPS)
			{
				DrawTick(pp,false);
				swprintf(str,64,L"10^{%d}",int(floor(0.1+log10(z))));
				if(text && cur%k==0)
					Putsw(mglPoint(x0,y0,z),str,FontDef,FontSize,'z');
				cur++;
			}
			// subticks drawing
			for(zz=z;zz<10*z;zz+=z)
			{
				if(zz<Min.z || zz>Max.z)	continue;
				pp[0]=x0;	pp[1]=y0+ddy/14;	pp[2]=zz;
				pp[3]=x0;	pp[4]=y0;			pp[5]=zz;
				pp[6]=x0+ddx/14;	pp[7]=y0;	pp[8]=zz;
				DrawTick(pp,true);
			}
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraph::Axis(const char *dir)
{
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
}
//-----------------------------------------------------------------------------
void mglGraph::TickBox()
{
	float pp[36];
	float x0,y0,z0,x,y,z,ddx,ddy,ddz;
	x0 = GetOrgX();	y0 = GetOrgY();	z0 = GetOrgZ();
	ddx = dx>=0 ? dx : mgl_okrugl((Min.x-Max.x)/(dx+1),3);
	ddy = dy>=0 ? dy : mgl_okrugl((Min.y-Max.y)/(dy+1),3);
	ddz = dz>=0 ? dz : mgl_okrugl((Min.z-Max.z)/(dz+1),3);
	if(ddx>0)	// рисуем метки по х
	{
		x0 = isnan(TOrg.x) ? x0 : TOrg.x;
		x0 = x0 - ddx*floor((x0-Min.x)/ddx);
		for(x=x0;x<Max.x;x+=ddx)
		{
			pp[0]=x;	pp[1]=Min.y+ddy/7;	pp[2]=Min.z;
			pp[3]=x;	pp[4]=Min.y;		pp[5]=Min.z;
			pp[6]=x;	pp[7]=Min.y;		pp[8]=Min.z+ddz/7;
			pp[9]=x;	pp[10]=Min.y+ddy/7;	pp[11]=Max.z;
			pp[12]=x;	pp[13]=Min.y;		pp[14]=Max.z;
			pp[15]=x;	pp[16]=Min.y;		pp[17]=Max.z-ddz/7;
			pp[18]=x;	pp[19]=Max.y-ddy/7;	pp[20]=Max.z;
			pp[21]=x;	pp[22]=Max.y;		pp[23]=Max.z;
			pp[24]=x;	pp[25]=Max.y;		pp[26]=Max.z-ddz/7;
			pp[27]=x;	pp[28]=Max.y-ddy/7;	pp[29]=Min.z;
			pp[30]=x;	pp[31]=Max.y;		pp[32]=Min.z;
			pp[33]=x;	pp[34]=Max.y;		pp[35]=Min.z+ddz/7;
			DrawTick(pp,false);		DrawTick(pp+9,false);
			DrawTick(pp+18,false);	DrawTick(pp+27,false);
		}
	}
	else if(Min.x>0)
	{
		x0 = exp(M_LN10*floor(0.1+log10(Min.x)));
		for(x=x0;x<Max.x;x*=10)
		{
			if(x<Min.x)	continue;
			pp[0]=x;	pp[1]=Min.y+ddy/7;	pp[2]=Min.z;
			pp[3]=x;	pp[4]=Min.y;		pp[5]=Min.z;
			pp[6]=x;	pp[7]=Min.y;		pp[8]=Min.z+ddz/7;
			pp[9]=x;	pp[10]=Min.y+ddy/7;	pp[11]=Max.z;
			pp[12]=x;	pp[13]=Min.y;		pp[14]=Max.z;
			pp[15]=x;	pp[16]=Min.y;		pp[17]=Max.z-ddz/7;
			pp[18]=x;	pp[19]=Max.y-ddy/7;	pp[20]=Max.z;
			pp[21]=x;	pp[22]=Max.y;		pp[23]=Max.z;
			pp[24]=x;	pp[25]=Max.y;		pp[26]=Max.z-ddz/7;
			pp[27]=x;	pp[28]=Max.y-ddy/7;	pp[29]=Min.z;
			pp[30]=x;	pp[31]=Max.y;		pp[32]=Min.z;
			pp[33]=x;	pp[34]=Max.y;		pp[35]=Min.z+ddz/7;
			DrawTick(pp,false);		DrawTick(pp+9,false);
			DrawTick(pp+18,false);	DrawTick(pp+27,false);
		}
	}
	x0 = GetOrgX();
	if(ddy>0)	// рисуем метки по y
	{
		y0 = isnan(TOrg.y) ? y0 : TOrg.y;
		y0 = y0 - ddy*floor((y0-Min.y)/ddy);
		for(y=y0;y<Max.y;y+=ddy)
		{
			pp[0]=Min.x+ddx/7;	pp[1]=y;	pp[2]=Min.z;
			pp[3]=Min.x;		pp[4]=y;	pp[5]=Min.z;
			pp[6]=Min.x;		pp[7]=y;	pp[8]=Min.z+ddz/7;
			pp[9]=Max.x-ddx/7;	pp[10]=y;	pp[11]=Min.z;
			pp[12]=Max.x;		pp[13]=y;	pp[14]=Min.z;
			pp[15]=Max.x;		pp[16]=y;	pp[17]=Min.z+ddz/7;
			pp[18]=Min.x+ddx/7;	pp[19]=y;	pp[20]=Max.z;
			pp[21]=Min.x;		pp[22]=y;	pp[23]=Max.z;
			pp[24]=Min.x;		pp[25]=y;	pp[26]=Max.z-ddz/7;
			pp[27]=Max.x-ddx/7;	pp[28]=y;	pp[29]=Max.z;
			pp[30]=Max.x;		pp[31]=y;	pp[32]=Max.z;
			pp[33]=Max.x;		pp[34]=y;	pp[35]=Max.z-ddz/7;
			DrawTick(pp,false);		DrawTick(pp+9,false);
			DrawTick(pp+18,false);	DrawTick(pp+27,false);
		}
	}
	else if(Min.y>0)
	{
		y0 = exp(M_LN10*floor(0.1+log10(Min.y)));
		for(y=y0;y<Max.y;y*=10)
		{
			if(y<Min.y)	continue;
			pp[0]=Min.x+ddx/7;	pp[1]=y;	pp[2]=Min.z;
			pp[3]=Min.x;		pp[4]=y;	pp[5]=Min.z;
			pp[6]=Min.x;		pp[7]=y;	pp[8]=Min.z+ddz/7;
			pp[9]=Max.x-ddx/7;	pp[10]=y;	pp[11]=Min.z;
			pp[12]=Max.x;		pp[13]=y;	pp[14]=Min.z;
			pp[15]=Max.x;		pp[16]=y;	pp[17]=Min.z+ddz/7;
			pp[18]=Min.x+ddx/7;	pp[19]=y;	pp[20]=Max.z;
			pp[21]=Min.x;		pp[22]=y;	pp[23]=Max.z;
			pp[24]=Min.x;		pp[25]=y;	pp[26]=Max.z-ddz/7;
			pp[27]=Max.x-ddx/7;	pp[28]=y;	pp[29]=Max.z;
			pp[30]=Max.x;		pp[31]=y;	pp[32]=Max.z;
			pp[33]=Max.x;		pp[34]=y;	pp[35]=Max.z-ddz/7;
			DrawTick(pp,false);		DrawTick(pp+9,false);
			DrawTick(pp+18,false);	DrawTick(pp+27,false);
		}
	}
	y0 = GetOrgY();
	if(ddz>0)	// рисуем метки
	{
		z0 = isnan(TOrg.z) ? z0 : TOrg.z;
		z0 = z0 - ddz*floor((z0-Min.z)/ddz);
		for(z=z0;z<Max.z;z+=ddz)
		{
			pp[0]=Min.x+ddx/7;	pp[1]=Min.y;		pp[2]=z;
			pp[3]=Min.x;		pp[4]=Min.y;		pp[5]=z;
			pp[6]=Min.x;		pp[7]=Min.y+ddy/7;	pp[8]=z;
			pp[9]=Max.x-ddx/7;	pp[10]=Min.y;		pp[11]=z;
			pp[12]=Max.x;		pp[13]=Min.y;		pp[14]=z;
			pp[15]=Max.x;		pp[16]=Min.y+ddy/7;	pp[17]=z;
			pp[18]=Min.x+ddx/7;	pp[19]=Max.y;		pp[20]=z;
			pp[21]=Min.x;		pp[22]=Max.y;		pp[23]=z;
			pp[24]=Min.x;		pp[25]=Max.y-ddy/7;	pp[26]=z;
			pp[27]=Max.x-ddx/7;	pp[28]=Max.y;		pp[29]=z;
			pp[30]=Max.x;		pp[31]=Max.y;		pp[32]=z;
			pp[33]=Max.x;		pp[34]=Max.y-ddy/7;	pp[35]=z;
			DrawTick(pp,false);		DrawTick(pp+9,false);
			DrawTick(pp+18,false);	DrawTick(pp+27,false);
		}
	}
	else if(Min.z>0)
	{
		z0 = exp(M_LN10*floor(0.1+log10(Min.z)));
		for(z=z0;z<Max.z;z*=10)
		{
			if(z<Min.z)	continue;
			pp[0]=Min.x+ddx/7;	pp[1]=Min.y;		pp[2]=z;
			pp[3]=Min.x;		pp[4]=Min.y;		pp[5]=z;
			pp[6]=Min.x;		pp[7]=Min.y+ddy/7;	pp[8]=z;
			pp[9]=Max.x-ddx/7;	pp[10]=Min.y;		pp[11]=z;
			pp[12]=Max.x;		pp[13]=Min.y;		pp[14]=z;
			pp[15]=Max.x;		pp[16]=Min.y+ddy/7;	pp[17]=z;
			pp[18]=Min.x+ddx/7;	pp[19]=Max.y;		pp[20]=z;
			pp[21]=Min.x;		pp[22]=Max.y;		pp[23]=z;
			pp[24]=Min.x;		pp[25]=Max.y-ddy/7;	pp[26]=z;
			pp[27]=Max.x-ddx/7;	pp[28]=Max.y;		pp[29]=z;
			pp[30]=Max.x;		pp[31]=Max.y;		pp[32]=z;
			pp[33]=Max.x;		pp[34]=Max.y-ddy/7;	pp[35]=z;
			DrawTick(pp,false);		DrawTick(pp+9,false);
			DrawTick(pp+18,false);	DrawTick(pp+27,false);
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
	float x1=Min.x,x2=Max.x,y1=Min.y,y2=Max.y,z1=Min.z,z2=Max.z;
	float pp[3*16];
	long nn[16];
	Arrow1 = Arrow2 = '_';
	if(p.Valid())	Pen(p,'-',BaseLineWidth);
	else			Pen(TranspType!=2 ? BC:WC,'-',BaseLineWidth);
	if(TernAxis)
	{
		pp[3*0+0] = x1;	pp[3*0+1] = y1;	pp[3*0+2] = z1;	nn[0] = 1;
		pp[3*1+0] = x1;	pp[3*1+1] = y1;	pp[3*1+2] = z2;	nn[1] = 2;
		pp[3*2+0] = x1;	pp[3*2+1] = y2;	pp[3*2+2] = z2;	nn[2] = 3;
		pp[3*3+0] = x1;	pp[3*3+1] = y2;	pp[3*3+2] = z1;	nn[3] = 0;

		pp[3*4+0] = x2;	pp[3*4+1] = y1;	pp[3*4+2] = z1;	nn[4] = 5;
		pp[3*5+0] = x2;	pp[3*5+1] = y1;	pp[3*5+2] = z2;	nn[5] = 6;
		pp[3*6+0] = x2;	pp[3*6+1] = y1;	pp[3*6+2] = z2;	nn[6] = 7;
		pp[3*7+0] = x2;	pp[3*7+1] = y1;	pp[3*7+2] = z1;	nn[7] = 4;

		pp[3*8+0] = x1;	pp[3*8+1] = y1;	pp[3*8+2] = z1;	nn[8] = 9;
		pp[3*9+0] = x2;	pp[3*9+1] = y1;	pp[3*9+2] = z1;	nn[9] = -1;
		pp[3*10+0]= x1;	pp[3*10+1] = y1;pp[3*10+2] = z2;nn[10] = 11;
		pp[3*11+0]= x2;	pp[3*11+1] = y1;pp[3*11+2] = z2;nn[11] = -1;
		pp[3*12+0]= x1;	pp[3*12+1] = y2;pp[3*12+2] = z2;nn[12] = 13;
		pp[3*13+0]= x2;	pp[3*13+1] = y1;pp[3*13+2] = z2;nn[13] = -1;
		pp[3*14+0]= x1;	pp[3*14+1] = y2;pp[3*14+2] = z1;nn[14] = 15;
		pp[3*15+0]= x2;	pp[3*15+1] = y1;pp[3*15+2] = z1;nn[15] = -1;
	}
	else
	{
		pp[3*0+0] = x1;	pp[3*0+1] = y1;	pp[3*0+2] = z1;	nn[0] = 1;
		pp[3*1+0] = x1;	pp[3*1+1] = y1;	pp[3*1+2] = z2;	nn[1] = 2;
		pp[3*2+0] = x1;	pp[3*2+1] = y2;	pp[3*2+2] = z2;	nn[2] = 3;
		pp[3*3+0] = x1;	pp[3*3+1] = y2;	pp[3*3+2] = z1;	nn[3] = 0;

		pp[3*4+0] = x2;	pp[3*4+1] = y1;	pp[3*4+2] = z1;	nn[4] = 5;
		pp[3*5+0] = x2;	pp[3*5+1] = y1;	pp[3*5+2] = z2;	nn[5] = 6;
		pp[3*6+0] = x2;	pp[3*6+1] = y2;	pp[3*6+2] = z2;	nn[6] = 7;
		pp[3*7+0] = x2;	pp[3*7+1] = y2;	pp[3*7+2] = z1;	nn[7] = 4;

		pp[3*8+0] = x1;	pp[3*8+1] = y1;	pp[3*8+2] = z1;	nn[8] = 9;
		pp[3*9+0] = x2;	pp[3*9+1] = y1;	pp[3*9+2] = z1;	nn[9] = -1;
		pp[3*10+0]= x1;	pp[3*10+1] = y1;pp[3*10+2] = z2;nn[10] = 11;
		pp[3*11+0]= x2;	pp[3*11+1] = y1;pp[3*11+2] = z2;nn[11] = -1;
		pp[3*12+0]= x1;	pp[3*12+1] = y2;pp[3*12+2] = z2;nn[12] = 13;
		pp[3*13+0]= x2;	pp[3*13+1] = y2;pp[3*13+2] = z2;nn[13] = -1;
		pp[3*14+0]= x1;	pp[3*14+1] = y2;pp[3*14+2] = z1;nn[14] = 15;
		pp[3*15+0]= x2;	pp[3*15+1] = y2;pp[3*15+2] = z1;nn[15] = -1;
	}
	for(long i=0;i<16;i++)
		ScalePoint(pp[3*i+0],pp[3*i+1],pp[3*i+2]);
	curv_plot(16,pp,0,nn);
	if(ticks && !TernAxis)	TickBox();
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
void mglGraph::Legend(float x, float y, const char *font, float size)
{	Legend(NumLeg,LegStr,LegStl,x,y,font,size);	}
//-----------------------------------------------------------------------------
void mglGraph::Legend(int where, const char *font, float size)
{	Legend(NumLeg,LegStr,LegStl,where,font,size);	}
//-----------------------------------------------------------------------------
void mglGraph::ClearLegend()
{
	for(long i=0;i<NumLeg;i++)	{	free(LegStr[i]);	free(LegStl[i]);	}
	NumLeg = 0;
}
//-----------------------------------------------------------------------------
void mglGraph::Legend(int n, wchar_t **text,char **style, float x, float y,
					const char *font, float size)
{
	if(n<1)	{	SetWarn(mglWarnLeg);	return;	}
	float pp[15], r=GetRatio(), rh, rw;
	if(size<=0)	size = -size*FontSize;

	rh=(r<1?r:1.)*size/12.*(Max.y-Min.y);
	rw=(r>1?1./r:1.)*size/16.;
	float w=0, h=fnt->Height(font)*rh, j, dx=Max.x-Min.x;
	register long i;
	for(i=0;i<n;i++)	{	j = fnt->Width(text[i],font)*rw;	w = w>j ? w:j;	}
	w = (w + 0.11)*dx;	// запас на линию

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
		pp[0] = x+0.01*dx;	pp[3] = x+0.09*dx;	pp[6] = x+0.05*dx;
		ScalePoint(pp[0],pp[1],pp[2]);
		ScalePoint(pp[3],pp[4],pp[5]);
		ScalePoint(pp[6],pp[7],pp[8]);

		curv_plot(2,pp,0);
		if(m)	Mark(pp[6],pp[7],pp[8],m);
		SelectPen(TranspType!=2 ? "k-1":"w-1");
		Putsw(mglPoint(x+0.10*dx, y+i*h+0.3f*h, Max.z),text[i], font, size,'n');
	}
}
//-----------------------------------------------------------------------------
void mglGraph::Legend(int n, wchar_t **text,char **style, int where,
					const char *font, float size)
{
	if(n<1)	{	SetWarn(mglWarnLeg);	return;	}
	if(size<0)	size = -size*FontSize;
	float w=0, r=GetRatio(), rh, rw;

	rh=(r<1?r:1.)*size/12.*(Max.y-Min.y);	// 12 = 16/1.4
	rw=(r>1?1./r:1.)*size/16.;
	float h=fnt->Height(font)*rh, x, y, dx = 0.03*(Max.x-Min.x), dy = 0.03*(Max.y-Min.y);
	for(long i=0;i<n;i++)	{	x = fnt->Width(text[i],font)*rw;	w = w>x ? w:x;	}
	w = (w + 0.11)*(Max.x-Min.x);	// запас на линию

	switch(where)
	{
	case 0:		x = Min.x+dx;	y = Min.y+dy;		break;
	case 1:		x = Max.x-w-dx;	y = Min.y+dy;		break;
	case 2:		x = Min.x+dx;	y = Max.y-h*n-dy;	break;
	default:	x = Max.x-w-dx;	y = Max.y-h*n-dy;	break;
	}
	Legend(n,text,style,x,y,font,size);
}
//-----------------------------------------------------------------------------
void mglGraph::Ternary(bool t)
{
	static mglPoint x1(-1,-1,-1),x2(1,1,1),o(0,0,0);
	TernAxis = t;	Cut = false;	TOrg = mglPoint(NAN,NAN,NAN);
	if(t)
	{
		x1 = Min;	x2 = Max;	o = Org;
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
	y0 = GetOrgY();	z0 = GetOrgZ();
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
			pp[0]=Max.x-y-ddx/7;pp[1]=y;	pp[2]=z0;
			pp[3]=Max.x-y;		pp[4]=y;	pp[5]=z0;
			pp[6]=Max.x-y;		pp[7]=y;	pp[8]=z0+ddz/7;
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
	x0 = GetOrgX();	z0 = GetOrgZ();
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
			pp[0]=x0+ddx/7;	pp[1]=Max.y-z-ddy/7;pp[2]=z0;
			pp[3]=x0;	pp[4]=Max.y-z;		pp[5]=z0;
			pp[6]=x0+ddx/7;	pp[7]=Max.y-z;	pp[8]=z0;
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
