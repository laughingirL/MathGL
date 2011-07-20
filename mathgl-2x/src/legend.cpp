/***************************************************************************
 * legend.cpp is part of Math Graphic Library
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
#include <time.h>
#include <wchar.h>
#include <ctype.h>
#include <stdlib.h>
#include "mgl/canvas.h"
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
void mglCanvas::AddLegend(const wchar_t *text,const char *style)
{
	if(!text)	return;
	mglText t(text,style);
	MGL_PUSH(Leg,t,mutexLeg);
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
	if(!font)	font="";
	char *pA, *ff = new char[strlen(font)+3];
	strcpy(ff,font);	strcat(ff,":L");	Push();
	if((pA=strchr(ff,'A')))	{	*pA = ' ';	InPlot(0,1,0,1,false);	}
	// find sizes
	float h=TextHeight(font)/2, dx = 0.03*inW, dy = 0.03*inH, w=0, t;
	register long i,j;
	for(i=0;i<n;i++)		// find text length
	{
		t = TextWidth(leg[i].text.c_str(),font)/4;
		if(leg[i].stl.empty())	t -= ll;
		w = w>t ? w:t;
	}
	w += ll+0.01*inW;	// add space for lines
	x = x*(inW-w-2*dx)+B.x-inW/2+dx;
	y = y*(inH-h*n-2*dy)+B.y-inH/2+dy;
	// draw it
	long k1,k2,k3,k4;
	mglPoint p,q=mglPoint(NAN);
	float c1=AddTexture('w'), c2=AddTexture('k'), s3=Depth;
	if((Flag&3)==2)	{	float cc=c1;	c2=c2;	c2=cc;	};
	if(get(MGL_LEGEND_BOX))	// draw bounding box
	{
		k1=AddPnt(mglPoint(x,y,s3),c1,q,0,0);
		k2=AddPnt(mglPoint(x+w,y,s3),c1,q,0,0);
		k3=AddPnt(mglPoint(x,y+h*n,s3),c1,q,0,0);
		k4=AddPnt(mglPoint(x+w,y+h*n,s3),c1,q,0,0);
		quad_plot(k1,k2,k3,k4);
		k1=CopyNtoC(k1,c2);	k2=CopyNtoC(k2,c2);
		k3=CopyNtoC(k3,c2);	k4=CopyNtoC(k4,c2);
		line_plot(k1,k2);	line_plot(k2,k4);
		line_plot(k4,k3);	line_plot(k3,k1);
	}
	for(i=0;i<n;i++)	// draw lines and legend
	{
		char m=SetPenPal(leg[i].stl.c_str());
		k1=AddPnt(mglPoint(x+0.1*ll,y+i*h+0.45*h,s3),CDef,q,0,0);
		k2=AddPnt(mglPoint(x+0.9*ll,y+i*h+0.45*h,s3),CDef,q,0,0);
		pPos=0;	line_plot(k1,k2);
		if(m)	for(j=0;j<LegendMarks;j++)
		{
			p = mglPoint(x+0.1f*ll + (j+1)*0.8f*ll/(1.+LegendMarks),y+i*h+0.45*h,s3);
			mark_plot(AddPnt(p,CDef,q,0,0),m);
		}
		p = mglPoint(x+((!leg[i].stl.empty())?ll:0), y+i*h+0.15*h, s3);
		text_plot(AddPnt(p,-1,q,0,0), leg[i].text.c_str(), ff, size);
	}
	Pop();	EndGroup();
}
//-----------------------------------------------------------------------------
