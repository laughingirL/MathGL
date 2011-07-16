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
void mglCanvas::Legend(float x, float y, const char *font, float size, float llen)
{	Legend(Leg,x,y,font,size,llen);	}
//-----------------------------------------------------------------------------
void mglCanvas::Legend(int where, const char *font, float size, float llen)
{	Legend(Leg,where,font,size,llen);	}
//-----------------------------------------------------------------------------
void mglCanvas::Legend(std::vector<mglText> leg, int where, const char *font, float size, float llen)
{
	long n=leg.size();
	if(n<1)	{	SetWarn(mglWarnLeg);	return;	}
	if(isnan(llen))	llen=0.1;
	if(size<0)	size = -size*FontSize;
	float w=0, r=GetRatio(), rh, rw;

	rh=(r<1?r:1.)*size/6.;	// 6 = 12/2 = (16/1.4)/2
	rw=(r>1?1./r:1.)*size/8.;
	float h=fnt->Height(font)*rh, x, y, dx = 0.06, dy = 0.06;
	for(long i=0;i<n;i++)		// find text length
	{
		x = fnt->Width(leg[i].text.c_str(),font)*rw;
		if(leg[i].stl.empty())	x -= llen;
		w = w>x ? w:x;
	}
	w = (w + 1.1f*llen*1.5);	// add space for lines

	switch(where)
	{
	case 0:		x = dx-1;	y = dy-1;		break;
	case 1:		x = 1-w-dx;	y = dy-1;		break;
	case 2:		x = dx-1;	y = 1-h*n-dy;	break;
	default:	x = 1-w-dx;	y = 1-h*n-dy;	break;
	}
	Legend(leg,(x+1)/2,(y+1)/2,font,size,llen);
}
//-----------------------------------------------------------------------------
void mglCanvas::Legend(std::vector<mglText> leg, float x, float y, const char *font, float size, float llen)
{
	long n=leg.size();
	if(n<1)	{	SetWarn(mglWarnLeg);	return;	}
	x = 2*x-1;	y = 2*y-1;
	if(isnan(llen))	llen=0.1;
	static int cgid=1;	StartGroup("Legend",cgid++);
	float r=GetRatio(), rh, rw, s3=B.pf;
	if(size<=0)	size = -size*FontSize;
	if(!font || !(*font))	font="L";
	char *pA, *ff = new char[strlen(font)+3];	strcpy(ff,font);
	llen *= 1.5;

	rh=(r<1?r:1.)*size/6.;	rw=(r>1?1./r:1.)*size/8.;
//	rh=size/6.;	rw=size/24.;
	float w=0, h=fnt->Height(font)*rh, l;
	register long i;
	for(i=0;i<n;i++)		// find text length
	{
		l = fnt->Width(leg[i].text.c_str(),font)*rw;
		if(leg[i].stl.empty())	l -= llen;
		w = w>l ? w:l;
	}
	w = (w + llen*1.1f);	// add space for lines

	bool rel = true;
	if((pA=strchr(ff,'A')))	{	*pA = ' ';	strcat(ff,":L");	rel = false;	}
	Push();	Identity(rel);	//	memcpy(B,B1,9*sizeof(float));

	long k1,k2,k3,k4;
	mglPoint p,q=mglPoint(NAN);
	float c1=AddTexture('w'), c2=AddTexture('k');
	if((Flag&3)==2)	{	float cc=c1;	c2=c2;	c2=cc;	};
	if(get(MGL_LEGEND_BOX))	// draw bounding box
	{
		p = mglPoint(x,y,s3-0.01);		k1=AddPnt(p,c1,q);
		p = mglPoint(x+w,y,s3-0.01);	k2=AddPnt(p,c1,q);
		p = mglPoint(x,y+h*n,s3-0.01);	k3=AddPnt(p,c1,q);
		p = mglPoint(x+w,y+h*n,s3-0.01);k4=AddPnt(p,c1,q);
		quad_plot(k1,k2,k3,k4);
		line_plot(k1,k2);	line_plot(k2,k4);
		line_plot(k4,k3);	line_plot(k3,k1);
	}
	for(i=0;i<n;i++)	// draw lines and legend
	{
		char m=SetPenPal(leg[i].stl.c_str());
		p = mglPoint(x+0.1*llen,y+i*h+0.45*h,s3);	k1=AddPnt(p,CDef);
		p = mglPoint(x+0.9*llen,y+i*h+0.45*h,s3);	k2=AddPnt(p,CDef);
		pPos=0;	line_plot(k1,k2);
		if(m)	for(int j=0;j<LegendMarks;j++)
		{
			p.x = x+0.1f*llen + (j+1)*0.8f*llen/(1.+LegendMarks);
			mark_plot(AddPnt(p,CDef),m);
		}
		p = mglPoint(x+((!leg[i].stl.empty())?llen:0), y+i*h+0.3f*h, s3);
		k1 = AddPnt(p,-1,q);
		text_plot(k1, leg[i].text.c_str(), ff, size);
	}
	Pop();	EndGroup();	delete []ff;
}
//-----------------------------------------------------------------------------
