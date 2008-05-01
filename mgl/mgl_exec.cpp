/* mgl_exec.cpp is part of Math Graphic Library
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
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#include <unistd.h>
#else
#include <direct.h>
#endif
//#include <unistd.h>
#include "mgl/mgl_parse.h"
char *mgl_strdup(const char *s);
wchar_t *mgl_str_copy(const char *s);
//-----------------------------------------------------------------------------
//	{"addlegend","Add legend entry","addlegend txt fmt",false,mgls_addlegend,mglc_addlegend}
int mgls_addlegend(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==2 && k[1]==2)	gr->AddLegend(a[0].s,a[1].s);
	else	return 1;
	return 0;
}
void mglc_addlegend(char out[1024], long n, mglArg *a, int k[9])
{	if(k[0]==2 && k[1]==2)	sprintf(out,"gr->AddLegend(\"%s\", \"%s\");",a[0].s,a[1].s);	}
//-----------------------------------------------------------------------------
//	{"addto","Add data or number","addto var|num",false,mgls_addto,mglc_addto}
int mgls_addto(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1)		*(a[0].d) += *(a[1].d);
	else if(k[0]==1 && k[1]==3)	*(a[0].d) += a[1].v;
	else	return 1;
	return 0;
}
void mglc_addto(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1)		sprintf(out,"%s += %s;",a[0].s, a[1].s);
	else if(k[0]==1 && k[1]==3)	sprintf(out,"%s += %g;",a[0].s, a[1].v);
}
//-----------------------------------------------------------------------------
//	{"alpha","Switch on/off transparency","addto [num]",false,mgls_alpha,mglc_alpha}
int mgls_alpha(mglGraph *gr, long n, mglArg *a, int k[9])
{
	gr->Alpha(k[0]==3 ? a[0].v!=0 : true);	return 0;
}
void mglc_alpha(char out[1024], long n, mglArg *a, int k[9])
{
	sprintf(out,"gr->Alpha(%s);",(k[0]==3&&a[0].v!=0)?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"alphadef","Set default transparency","alphadef num",false,mgls_alphadef,mglc_alphadef}
int mgls_alphadef(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3)	gr->AlphaDef = a[0].v;
	else	return 1;
	return 0;
}
void mglc_alphadef(char out[1024], long n, mglArg *a, int k[9])
{	if(k[0]==3)	sprintf(out,"gr->AlphaDef = %g;",a[0].v);	}
//-----------------------------------------------------------------------------
//	{"ambient","Set ambient light brightness","ambient txt fmt",false,mgls_ambient,mglc_ambient}
int mgls_ambient(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3)	gr->Ambient(a[0].v);
	else	return 1;
	return 0;
}
void mglc_ambient(char out[1024], long n, mglArg *a, int k[9])
{	if(k[0]==3)	sprintf(out,"gr->Ambient(%g);",a[0].v);	}
//-----------------------------------------------------------------------------
//	{"area","Draw area plot for 1D data","area {xvar} yvar {{zvar}} [fmt num]",false,mgls_area,mglc_area}
int mgls_area(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->Area(*(a[0].d),k[1]==2?a[1].s:0,false,k[2]==3 ? a[2].v : NAN);
	else if(k[2]!=1)	gr->Area(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,false,k[3]==3 ? a[3].v : NAN);
	else 				gr->Area(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	return 0;
}
void mglc_area(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		sprintf(out,"gr->Area(%s, \"%s\", false, %g);",a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN);
	else if(k[2]!=1)
		sprintf(out,"gr->Area(%s, %s, \"%s\", false, %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else 	sprintf(out,"gr->Area(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"aspect","Set aspect ration","aspect numx numy numz",false,mgls_aspect,mglc_aspect}
int mgls_aspect(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3 && k[2]==3)	gr->Aspect(a[0].v,a[1].v,a[2].v);
	else	return 1;
	return 0;
}
void mglc_aspect(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3 && k[2]==3)
		sprintf(out,"gr->Aspect(%g, %g, %g);",a[0].v,a[1].v,a[2].v);
}
//-----------------------------------------------------------------------------
//	{"axial","Draw surfaces of contour lines rotation","axial {xvar yvar} zvar [fmt num]",false,mgls_axial,mglc_axial}
int mgls_axial(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->Axial(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3 ? int(a[2].v) : 3);
	else if(k[1]==1 && k[2]==1)
		gr->Axial(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0,k[4]==3 ? int(a[4].v) : 3);
	else	return 1;
	return 0;
}
void mglc_axial(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		sprintf(out,"gr->Axial(%s, \"%s\", %d);", a[0].s,k[1]==2?a[1].s:"",k[2]==3 ? int(a[2].v) : 3);
	else if(k[1]==1 && k[2]==1)
		sprintf(out,"gr->Axial(%s, %s, %s, \"%s\", %d);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?int(a[4].v):3);
}
//-----------------------------------------------------------------------------
//	{"axialdir","Set axis of rotation","axialdir dir",false,mgls_axialdir,mglc_axialdir}
int mgls_axialdir(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==2)	gr->AxialDir = a[0].s[0];	else	return 1;
	return 0;
}
void mglc_axialdir(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==2)	sprintf(out,"gr->AxialDir = '%c';", a[0].s[0]);
}
//-----------------------------------------------------------------------------
//	{"axis","Setup or draw axis",0,false,mgls_axis,mglc_axis}
int mgls_axis(mglGraph *gr, long n, mglArg *a, int k[9])
{
	register int i;
	if(k[0]==2 && k[1]==2 && k[2]==2)	gr->Axis(a[0].s,a[1].s,a[2].s);
	else if(n==6)
	{
		bool ok=true;
		for(i=0;i<6;i++)	if(k[i]!=3)	ok = false;
		if(ok)	gr->Axis(mglPoint(a[0].v,a[1].v,a[2].v), mglPoint(a[3].v,a[4].v,a[5].v),gr->Org);
		else return 1;
	}
	else if(n==4)
	{
		bool ok=true;
		for(i=0;i<4;i++)	if(k[i]!=3)	ok = false;
		if(ok)	gr->Axis(mglPoint(a[0].v,a[1].v), mglPoint(a[2].v,a[3].v),gr->Org);
		else return 1;
	}
	else if(k[0]==2)	gr->Axis(a[0].s);
	else if(k[0]==0)	gr->Axis("xyz");
	else return 1;
	return 0;
}
void mglc_axis(char out[1024], long n, mglArg *a, int k[9])
{
	register int i;
	if(k[0]==2 && k[1]==2 && k[2]==2)
		sprintf(out,"gr->Axis(\"%s\", \"%s\", \"%s\");", a[0].s,a[1].s,a[2].s);
	else if(n==6)
	{
		bool ok=true;
		for(i=0;i<6;i++)	if(k[i]!=3)	ok = false;
		if(ok)	sprintf(out,"gr->Axis(mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), gr->Org);", a[0].v,a[1].v,a[2].v,a[3].v,a[4].v,a[5].v);
	}
	else if(n==4)
	{
		bool ok=true;
		for(i=0;i<4;i++)	if(k[i]!=3)	ok = false;
		if(ok)	sprintf(out,"gr->Axis(mglPoint(%g, %g), mglPoint(%g, %g), gr->Org);", a[0].v,a[1].v,a[2].v,a[3].v);
	}
	else if(k[0]==2)	sprintf(out,"gr->Axis(\"%s\");", a[0].s);
	else if(k[0]==0)	sprintf(out,"gr->Axis(\"xyz\");");
}
//-----------------------------------------------------------------------------
//	{"ball","Draw point (ball)","ball posx posy {posz} [fmt]",false,mgls_ball,mglc_ball}
int mgls_ball(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3 && k[2]==3)
		gr->Ball(mglPoint(a[0].v,a[1].v,a[2].v),k[3]==2 ? a[3].s[0]:'r');
	else if(k[0]==3 && k[1]==3)
		gr->Ball(mglPoint(a[0].v,a[1].v),k[2]==2 ? a[2].s[0]:'r');
	else	return 1;
	return 0;
}
void mglc_ball(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3 && k[2]==3)
		sprintf(out,"gr->Ball(mglPoint(%g, %g, %g), '%c');", a[0].v,a[1].v,a[2].v,k[3]==2 ? a[3].s[0]:'r');
	else if(k[0]==3 && k[1]==3)
		sprintf(out,"gr->Ball(mglPoint(%g, %g), '%c');", a[0].v, a[1].v, k[2]==2 ? a[2].s[0]:'r');
}
//-----------------------------------------------------------------------------
//	{"box","Draw bounding box","box [fmt]",false,mgls_box,mglc_box}
int mgls_box(mglGraph *gr, long n, mglArg *a, int k[9])
{
	gr->Box(k[0]==2 ? a[0].s : (gr->TranspType!=2 ?"k-":"w-"));
	return 0;
}
void mglc_box(char out[1024], long n, mglArg *a, int k[9])
{
	sprintf(out,"gr->Box(\"%s\");", k[0]==2 ? a[0].s : "");
}
//-----------------------------------------------------------------------------
//	{"bars","Draw bars for 1D data","bars {xvar} yvar {{zvar}} [fmt num num]",false,mgls_bars,mglc_bars}
int mgls_bars(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->Bars(*(a[0].d), k[1]==2?a[1].s:0, k[2]==3?a[2].v:NAN, k[3]==3?(a[3].v!=0):false);
	else if(k[2]!=1)	gr->Bars(*(a[0].d), *(a[1].d), k[2]==2?a[2].s:0, k[3]==3?a[3].v:NAN, k[4]==3?(a[4].v!=0):false);
	else 				gr->Bars(*(a[0].d), *(a[1].d), *(a[2].d), k[3]==2?a[3].s:0, k[4]==3?(a[4].v!=0):false);
	return 0;
}
void mglc_bars(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)	
		sprintf(out,"gr->Bars(%s, \"%s\", %g, %s);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN, (k[3]==3&&a[3].v!=0)?"true":"false");
	else if(k[2]!=1)
		sprintf(out,"gr->Bars(%s, %s, \"%s\", %g, %s);",a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN, (k[4]==3&&a[4].v!=0)?"true":"false");
	else
		sprintf(out,"gr->Bars(%s, %s, %s, \"%s\", %s);",a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", (k[4]==3&&a[4].v!=0)?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"belt","Draw belts","belt {xvar yvar} zvar [fmt]",false,mgls_belt,mglc_belt}
int mgls_belt(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)			gr->Belt(*(a[0].d),k[1]==2?a[1].s:0);
	else if(k[1]==1 && k[2]==1)	gr->Belt(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	else	return 1;
	return 0;
}
void mglc_belt(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		sprintf(out,"gr->Belt(%s, \"%s\");", a[0].s,k[1]==2?a[1].s:"");
	else if(k[1]==1 && k[2]==1)
		sprintf(out,"gr->Belt(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"boxs","Draw boxes","boxs {xvar yvar} zvar [fmt]",false,mgls_boxs,mglc_boxs}
int mgls_boxs(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)			gr->Boxs(*(a[0].d),k[1]==2?a[1].s:0);
	else if(k[1]==1 && k[2]==1)	gr->Boxs(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	else	return 1;
	return 0;
}
void mglc_boxs(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		sprintf(out,"gr->Boxs(%s, \"%s\");", a[0].s,k[1]==2?a[1].s:"");
	else if(k[1]==1 && k[2]==1)
		sprintf(out,"gr->Boxs(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"beam","Draw quasioptical beam",0,false,mgls_beam,mglc_beam}
int mgls_beam(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==3)
		gr->Beam(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),a[4].v,
				k[5]==2? a[5].s:0,k[6]==3?int(a[6].v):0, int(k[7]==3?a[7].v:3));
	else	return 1;
	return 0;
}
void mglc_beam(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==3)
		sprintf(out,"gr->Beam(%s, %s, %s, %s, %g, \"%s\", %d, %d);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].v, k[5]==2? a[5].s:"",k[6]==3?int(a[6].v):0, int(k[7]==3?a[7].v:3));
}
//-----------------------------------------------------------------------------
//	{"caxis","Set color range","caxis c1 c2",false,mgls_caxis,mglc_caxis}
int mgls_caxis(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3)	gr->CAxis(a[0].v,a[1].v);
	else	return 1;
	return 0;
}
void mglc_caxis(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3)	sprintf(out,"gr->CAxis(%g, %g);", a[0].v,a[1].v);
}
//-----------------------------------------------------------------------------
//	{"clearlegend","Clear legend antries","clearlegend",false,mgls_clearlegend,mglc_clearlegend}
int mgls_clearlegend(mglGraph *gr, long n, mglArg *a, int k[9])
{	gr->ClearLegend();	return 0;	}
void mglc_clearlegend(char out[1024], long n, mglArg *a, int k[9])
{	sprintf(out,"gr->ClearLegend();");	}
//-----------------------------------------------------------------------------
//	{"clf","Clear picture","clf",false,mgls_clf,mglc_clf}
int mgls_clf(mglGraph *gr, long n, mglArg *a, int k[9])
{	gr->Clf();	return 0;	}
void mglc_clf(char out[1024], long n, mglArg *a, int k[9])
{	sprintf(out,"gr->Clf();");	}
//-----------------------------------------------------------------------------
//	{"chart","Draw chart","chart var [fmt]",false,mgls_chart,mglc_chart}
int mgls_chart(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1)	gr->Chart(*(a[0].d), k[1]==2?a[1].s:0);
	else	return 1;
	return 0;
}
void mglc_chart(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3)	sprintf(out,"gr->Chart(%s, \"%s\");", a[0].s, k[1]==2?a[1].s:"");
}
//-----------------------------------------------------------------------------
//	{"cloud","Draw cloud","cloud {xvar yvar zvar} avar [fmt]",false,mgls_cloud,mglc_cloud}
int mgls_cloud(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->CloudQ(*(a[0].d),k[1]==2?a[1].s:0);
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		gr->CloudQ(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
	return 0;
}
void mglc_cloud(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		sprintf(out,"gr->CloudQ(%s,\"%s\");",a[0].s, k[1]==2?a[1].s:"");
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		sprintf(out,"gr->CloudQ(%s, %s, %s, %s, \"%s\");", a[0].s,a[1].s,a[2].s,a[3].s,k[4]==2?a[4].s:"");
}
//-----------------------------------------------------------------------------
//	{"crange","Set color range","crange {var [sym] | c1 c2}",false,mgls_crange,mglc_crange}
int mgls_crange(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1)	gr->CRange(*(a[0].d),k[1]==3?(a[1].v!=0):false);
	else if(k[0]==3 && k[1]==3)	{	gr->Cmin = a[0].v;	gr->Cmax = a[1].v;	}
	else	return 1;
	return 0;
}
void mglc_crange(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1)	sprintf(out,"gr->CRange(%s, %s);",a[0].s, (k[1]==3 && a[1].v!=0)?"true":"false");
	else if(k[0]==3 && k[1]==3)	sprintf(out,"gr->Cmin = %g;\tgr->Cmax = %g;",a[0].v,a[1].v);
}
//-----------------------------------------------------------------------------
//	{"crop","Crop edge of data","crop var n1 n2 dir",false,mgls_crop,mglc_crop}
int mgls_crop(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==3 && k[2]==3 && k[3]==2)
		a[0].d->Crop(int(a[1].v),int(a[2].v),a[3].s[0]);
	else	return 1;
	return 0;
}
void mglc_crop(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==3 && k[2]==3 && k[3]==2)
		sprintf(out,"%s.Crop(%d, %d, '%c');",a[0].s, int(a[1].v), int(a[2].v), a[3].s[0]);
}
//-----------------------------------------------------------------------------
//	{"cumsum","Crop edge of data","cumsum var dir",false,mgls_cumsum,mglc_cumsum}
int mgls_cumsum(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2)	a[0].d->CumSum(a[1].s);
	else	return 1;
	return 0;
}
void mglc_cumsum(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2)	sprintf(out,"%s.CumSum(\"%s\");",a[0].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"curve","Draw curve",0,false,mgls_curve,mglc_curve}
int mgls_curve(mglGraph *gr, long n, mglArg *a, int k[9])
{
	register long i;
	for(i=0;i<12;i++)	if(a[i].type!=2 || i>=n)	break;
	if(i==12)
		gr->Curve(mglPoint(a[0].v,a[1].v,a[2].v),
			mglPoint(a[3].v,a[4].v,a[5].v),
			mglPoint(a[6].v,a[7].v,a[8].v),
			mglPoint(a[9].v,a[10].v,a[11].v),
			(n>12 && a[12].type==1) ? a[12].s : 0);
	else if(i==8)
		gr->Curve(mglPoint(a[0].v,a[1].v),
			mglPoint(a[2].v,a[3].v),
			mglPoint(a[4].v,a[5].v),
			mglPoint(a[6].v,a[7].v),
			(n>8 && a[8].type==1) ? a[8].s : 0);
	return 0;
}
void mglc_curve(char out[1024], long n, mglArg *a, int k[9])
{
	register long i;
	for(i=0;i<12;i++)	if(a[i].type!=2 || i>=n)	break;
	if(i==12)
		sprintf(out,"gr->Curve(mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), \"%s\");", a[0].v, a[1].v, a[2].v, a[3].v, a[4].v, a[5].v, a[6].v, a[7].v, a[8].v, a[9].v, a[10].v, a[11].v, (n>12 && a[12].type==1) ? a[12].s : "");
	else if(i==8)
		sprintf(out,"gr->Curve(mglPoint(%g, %g), mglPoint(%g, %g), mglPoint(%g, %g), mglPoint(%g, %g), \"%s\");", a[0].v, a[1].v, a[2].v, a[3].v, a[4].v, a[5].v, a[6].v, a[7].v, (n>8 && a[8].type==1) ? a[8].s : "");
}
//-----------------------------------------------------------------------------
//	{"cut","Setup plot points cutting",0,false,mgls_cut,mglc_cut}
int mgls_cut(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3 && n == 1)	gr->Cut = (a[0].v != 0);
	else if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3 && k[4]==3 && k[5]==3)
	{
		gr->CutMin = mglPoint(a[0].v,a[1].v,a[2].v);
		gr->CutMax = mglPoint(a[3].v,a[4].v,a[5].v);
	}
	else if(k[0]==2 && n == 1)	gr->CutOff(a[0].s);
	else	return 1;
	return 0;
}
void mglc_cut(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3 && n == 1)	sprintf(out,"gr->Cut = %s;",(a[0].v != 0)?"true":"false");
	else if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3 && k[4]==3 && k[5]==3)
		sprintf(out,"gr->CutMin = mglPoint(%g, %g, %g);\tgr->CutMax = mglPoint(%g, %g, %g);", a[0].v, a[1].v, a[2].v, a[3].v, a[4].v, a[5].v);
	else if(k[0]==2 && n == 1)
		sprintf(out,"gr->CutOff(\"%s\");", a[0].s);
}
//-----------------------------------------------------------------------------
//	{"crust","Draw reconstructed surface for arbitrary data points","crust {xvar yvar zvar} | var [fmt]",false,mgls_crust,mglc_crust}
int mgls_crust(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1)
		gr->Crust(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	else if(k[0]==1)
		gr->Crust(*(a[0].d),k[1]==2?a[1].s:0);
	else	return 1;
	return 0;
}
void mglc_crust(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1)
		sprintf(out,"gr->Crust(%s, %s, %s, \"%s\");",a[0].s,a[1].s,a[2].s,k[3]==2?a[3].s:"");
	else if(k[0]==1)
		sprintf(out,"gr->Crust(%s, \"%s\");",a[0].s, k[1]==2?a[1].s:"");
}
//-----------------------------------------------------------------------------
//	{"colorbar","Draw colorbar","colorbar [fmt pos]",false,mgls_colorbar,mglc_colorbar}
int mgls_colorbar(mglGraph *gr, long n, mglArg *a, int k[9])
{
	gr->Colorbar(k[0]==2 ? a[0].s:0, k[1]==3 ? int(a[1].v):0);
	return 0;
}
void mglc_colorbar(char out[1024], long n, mglArg *a, int k[9])
{
	sprintf(out,"gr->Colorbar(\"%s\", %d);",k[0]==2 ? a[0].s:"", k[1]==3 ? int(a[1].v):0);
}
//-----------------------------------------------------------------------------
//	{"copy","Copy data from another variable","copy var1 var2",true,mgls_copy,mglc_copy}
int mgls_copy(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1)	a[0].d->Set(*(a[1].d));
	return 0;
}
void mglc_copy(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1)	sprintf(out,"%s.Set(%s);",a[0].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"cont","Draw contour lines","cont {vvar} {xvar yvar} zvar [fmt num zpos]",false,mgls_cont,mglc_cont}
int mgls_cont(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->Cont(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?int(a[2].v):7,k[3]==3?a[3].v:NAN);
	else if(k[1]==1 && k[2]!=1)
		gr->Cont(*(a[0].d), *(a[1].d), k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]!=1)
		gr->Cont(*(a[0].d), *(a[1].d), *(a[2].d), k[3]==2?a[3].s:0,
			k[4]==3?int(a[4].v):7, k[5]==3?a[5].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]!=1)
		gr->Cont(*(a[0].d), *(a[1].d), *(a[2].d), *(a[3].d), k[4]==2?a[4].s:0,
			k[5]==3?a[5].v:NAN);
	else	return 1;
	return 0;
}
void mglc_cont(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		sprintf(out,"gr->Cont(%s, \"%s\", %d, %g);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?int(a[2].v):7, k[3]==3?a[3].v:NAN);
	else if(k[1]==1 && k[2]!=1)
		sprintf(out,"gr->Cont(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]!=1)
		sprintf(out,"gr->Cont(%s, %s, %s, \"%s\", %d, %g);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?int(a[4].v):7, k[5]==3?a[5].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]!=1)
		sprintf(out,"gr->Cont(%s, %s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?a[5].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"contf","Draw solid contour lines","contf {vvar} {xvar yvar} zvar [fmt num zpos]",false,mgls_contf,mglc_contf}
int mgls_contf(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->ContF(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?int(a[2].v):7,k[3]==3?a[3].v:NAN);
	else if(k[1]==1 && k[2]!=1)
		gr->ContF(*(a[0].d), *(a[1].d), k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]!=1)
		gr->ContF(*(a[0].d), *(a[1].d), *(a[2].d), k[3]==2?a[3].s:0,
			k[4]==3?int(a[4].v):7, k[5]==3?a[5].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]!=1)
		gr->ContF(*(a[0].d), *(a[1].d), *(a[2].d), *(a[3].d), k[4]==2?a[4].s:0,
			k[5]==3?a[5].v:NAN);
	else	return 1;
	return 0;
}
void mglc_contf(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		sprintf(out,"gr->ContF(%s, \"%s\", %d, %g);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?int(a[2].v):7, k[3]==3?a[3].v:NAN);
	else if(k[1]==1 && k[2]!=1)
		sprintf(out,"gr->ContF(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]!=1)
		sprintf(out,"gr->ContF(%s, %s, %s, \"%s\", %d, %g);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?int(a[4].v):7, k[5]==3?a[5].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]!=1)
		sprintf(out,"gr->ContF(%s, %s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?a[5].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"cont3","Draw contour lines for 3D data","cont3 {xvar yvar zvar} avar dir [pos fmt num]",false,mgls_cont3,mglc_cont3}
int mgls_cont3(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]==2)
		gr->Cont3(*(a[0].d),a[1].s[0],k[2]==3?int(a[2].v):-1,
				k[3]==2?a[3].s:0, k[4]==3?int(a[4].v):7);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		gr->Cont3(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),a[4].s[0],
				k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0, k[7]==3?int(a[7].v):7);
	else	return 1;
	return 0;
}
void mglc_cont3(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]==2)
		sprintf(out,"gr->Cont3(%s, '%c', %d, \"%s\", %d);", a[0].s, a[1].s[0], k[2]==3?int(a[2].v):-1, k[3]==2?a[3].s:"", k[4]==3?int(a[4].v):7);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		sprintf(out,"gr->Cont3(%s, %s, %s, %s, '%c', %d, \"%s\", %d);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s[0], k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0, k[7]==3?int(a[7].v):7);
}
//-----------------------------------------------------------------------------
//	{"contf3","Draw solid contour lines for 3D data","contf3 {xvar yvar zvar} avar dir [pos fmt num]",false,mgls_contf3,mglc_contf3}
int mgls_contf3(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]==2)
		gr->ContF3(*(a[0].d),a[1].s[0],k[2]==3?int(a[2].v):-1,
				k[3]==2?a[3].s:0, k[4]==3?int(a[4].v):7);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		gr->ContF3(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),a[4].s[0],
				k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0, k[7]==3?int(a[7].v):7);
	else	return 1;
	return 0;
}
void mglc_contf3(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]==2)
		sprintf(out,"gr->ContF3(%s, '%c', %d, \"%s\", %d);", a[0].s, a[1].s[0], k[2]==3?int(a[2].v):-1, k[3]==2?a[3].s:"", k[4]==3?int(a[4].v):7);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		sprintf(out,"gr->ContF3(%s, %s, %s, %s, '%c', %d, \"%s\", %d);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s[0], k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0, k[7]==3?int(a[7].v):7);
}
//-----------------------------------------------------------------------------
//	{"conta","Draw contour lines at central slices of 3D data","conta {xvar yvar zvar} avar [fmt num]",false,mgls_conta,mglc_conta}
int mgls_conta(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		gr->ContA(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,
				k[5]==3?int(a[5].v):7);
	else	gr->ContA(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?int(a[2].v):7);
	return 0;
}
void mglc_conta(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		sprintf(out,"gr->ContA(%s, %s, %s, %s, \"%s\", %d);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?int(a[5].v):7);
	else
		sprintf(out,"gr->ContA(%s, \"%s\", %d);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?int(a[2].v):7);
}
//-----------------------------------------------------------------------------
//	{"contfa","Draw solid contour lines at central slices of 3D data","contfa {xvar yvar zvar} avar [fmt num]",false,mgls_contfa,mglc_contfa}
int mgls_contfa(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		gr->ContFA(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,
				k[5]==3?int(a[5].v):7);
	else	gr->ContFA(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?int(a[2].v):7);
	return 0;
}
void mglc_contfa(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		sprintf(out,"gr->ContFA(%s, %s, %s, %s, \"%s\", %d);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?int(a[5].v):7);
	else
		sprintf(out,"gr->ContFA(%s, \"%s\", %d);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?int(a[2].v):7);
}
//-----------------------------------------------------------------------------
//	{"contx","Draw contour lines at x-slice (or x-plane)","contx avar [fmt pos num]",false,mgls_contx,mglc_contx}
int mgls_contx(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1)	gr->ContX(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN,k[3]==3?int(a[3].v):7);
	else	return 1;
	return 0;
}
void mglc_contx(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1)	sprintf(out,"gr->ContX(%s, \"%s\", %g, %d);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN, k[3]==3?int(a[3].v):7);
}
//-----------------------------------------------------------------------------
//	{"contfx","Draw solid contour lines at x-slice (or x-plane)","contfx avar [fmt pos num]",false,mgls_contfx,mglc_contfx}
int mgls_contfx(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1)	gr->ContFX(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN,k[3]==3?int(a[3].v):7);
	else	return 1;
	return 0;
}
void mglc_contfx(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1)	sprintf(out,"gr->ContFX(%s, \"%s\", %g, %d);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN, k[3]==3?int(a[3].v):7);
}
//-----------------------------------------------------------------------------
//	{"conty","Draw contour lines at y-slice (or y-plane)","conty avar [fmt pos num]",false,mgls_conty,mglc_conty}
int mgls_conty(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1)	gr->ContY(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN,k[3]==3?int(a[3].v):7);
	else	return 1;
	return 0;
}
void mglc_conty(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1)	sprintf(out,"gr->ContY(%s, \"%s\", %g, %d);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN, k[3]==3?int(a[3].v):7);
}
//-----------------------------------------------------------------------------
//	{"contfy","Draw solid contour lines at y-slice (or y-plane)","contfy avar [fmt pos num]",false,mgls_contfy,mglc_contfy}
int mgls_contfy(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1)	gr->ContFY(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN,k[3]==3?int(a[3].v):7);
	else	return 1;
	return 0;
}
void mglc_contfy(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1)	sprintf(out,"gr->ContFY(%s, \"%s\", %g, %d);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN, k[3]==3?int(a[3].v):7);
}
//-----------------------------------------------------------------------------
//	{"contz","Draw contour lines at z-slice (or z-plane)","contz avar [fmt pos num]",false,mgls_contz,mglc_contz}
int mgls_contz(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1)	gr->ContZ(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN,k[3]==3?int(a[3].v):7);
	else	return 1;
	return 0;
}
void mglc_contz(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1)	sprintf(out,"gr->ContZ(%s, \"%s\", %g, %d);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN, k[3]==3?int(a[3].v):7);
}
//-----------------------------------------------------------------------------
//	{"contfy","Draw solid contour lines at z-slice (or z-plane)","contfz avar [fmt pos num]",false,mgls_contfz,mglc_contfz}
int mgls_contfz(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1)	gr->ContFZ(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN,k[3]==3?int(a[3].v):7);
	else	return 1;
	return 0;
}
void mglc_contfz(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1)	sprintf(out,"gr->ContFZ(%s, \"%s\", %g, %d);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN, k[3]==3?int(a[3].v):7);
}
//-----------------------------------------------------------------------------
//	{"cone","Draw cone","cone avar [fmt pos num]",false,mgls_cone,mglc_cone}
int mgls_cone(mglGraph *gr, long n, mglArg *a, int k[9])
{
	bool ok=true;
	for(int i=0;i<7;i++)	if(a[i].type!=2)	ok=false;
	if(ok)
	{
		if(a[7].type==2)
			gr->Cone(mglPoint(a[0].v,a[1].v,a[2].v), mglPoint(a[3].v,a[4].v,a[5].v),
				a[6].v, a[7].v, (a[8].type==1) ? a[8].s : 0, (a[9].type==2) ? a[9].v!=0 : false);
		else
			gr->Cone(mglPoint(a[0].v,a[1].v,a[2].v), mglPoint(a[3].v,a[4].v,a[5].v),
				a[6].v, -1, (a[7].type==1) ? a[7].s : 0, (a[8].type==2) ? a[8].v!=0 : false);
	}
	else	return 1;
	return 0;
}
void mglc_cone(char out[1024], long n, mglArg *a, int k[9])
{
	bool ok=true;
	for(int i=0;i<7;i++)	if(a[i].type!=2)	ok=false;
	if(ok)
	{
		if(a[7].type==2)
			sprintf(out,"gr->Cone(mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), %g, %g, \"%s\", %s);", a[0].v,a[1].v,a[2].v, a[3].v,a[4].v,a[5].v, a[6].v, a[7].v, (a[8].type==1) ? a[8].s : "", (a[9].type==2 && a[9].v!=0) ? "true" : "false");
		else
			sprintf(out,"gr->Cone(mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), %g, -1, \"%s\", %s);", a[0].v,a[1].v,a[2].v, a[3].v,a[4].v,a[5].v, a[6].v, (a[7].type==1) ? a[7].s : "", (a[8].type==2 && a[8].v!=0)? "true" : "false");
	}
}
//-----------------------------------------------------------------------------
//	{"dens","Draw density plot","dens {xvar yvar} zvar [fmt zpos]",false,mgls_dens,mglc_dens}
int mgls_dens(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->Dens(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]!=1)
		gr->Dens(*(a[0].d), *(a[1].d), *(a[2].d), k[3]==2?a[3].s:0, k[4]==3?a[4].v:NAN);
	else	return 1;
	return 0;
}
void mglc_dens(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		sprintf(out,"gr->Dens(%s,\"%s\", %g);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]!=1)
		sprintf(out,"gr->Dens(%s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?a[4].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"dens3","Draw density plot at slices of 3D data","dens3 {xvar yvar zvar} avar dir [pos fmt]",false,mgls_dens3,mglc_dens3}
int mgls_dens3(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]==2)
		gr->Dens3(*(a[0].d),a[1].s[0],k[2]==3?int(a[2].v):-1,
				k[3]==2?a[3].s:0);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		gr->Dens3(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),a[4].s[0],
				k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0);
	else	return 1;
	return 0;
}
void mglc_dens3(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]==2)
		sprintf(out,"gr->Dens3(%s, '%c', %d, \"%s\");", a[0].s, a[1].s[0], k[2]==3?int(a[2].v):-1, k[3]==2?a[3].s:"");
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		sprintf(out,"gr->Dens3(%s, %s, %s, %s, '%c', %d, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s[0], k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0);
}
//-----------------------------------------------------------------------------
//	{"densa","Draw density plot at central slices of 3D data","densa {xvar yvar zvar} avar [fmt num]",false,mgls_densa,mglc_densa}
int mgls_densa(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		gr->DensA(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
	else	gr->DensA(*(a[0].d),k[1]==2?a[1].s:0);
	return 0;
}
void mglc_densa(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		sprintf(out,"gr->DensA(%s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"");
	else
		sprintf(out,"gr->DensA(%s, \"%s\");", a[0].s, k[1]==2?a[1].s:"");
}
//-----------------------------------------------------------------------------
//	{"densx","Draw density plot at x-slice (or x-plane)","densx avar [fmt pos]",false,mgls_densx,mglc_densx}
int mgls_densx(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1)	gr->DensX(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN);
	else	return 1;
	return 0;
}
void mglc_densx(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1)	sprintf(out,"gr->DensX(%s, \"%s\", %g);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"densy","Draw density plot at y-slice (or y-plane)","densy avar [fmt pos]",false,mgls_densy,mglc_densy}
int mgls_densy(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1)	gr->DensY(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN);
	else	return 1;
	return 0;
}
void mglc_densy(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1)	sprintf(out,"gr->DensY(%s, \"%s\", %g);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"densz","Draw density plot at z-slice (or z-plane)","densz avar [fmt pos]",false,mgls_densz,mglc_densz}
int mgls_densz(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1)	gr->DensZ(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN);
	else	return 1;
	return 0;
}
void mglc_densz(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1)	sprintf(out,"gr->DensZ(%s, \"%s\", %g);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"divto","Divide by data or number","divto var|num",false,mgls_divto,mglc_divto}
int mgls_divto(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1)		*(a[0].d) /= *(a[1].d);
	else if(k[0]==1 && k[1]==3)	*(a[0].d) /= a[1].v;
	else	return 1;
	return 0;
}
void mglc_divto(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1)		sprintf(out,"%s /= %s;",a[0].s, a[1].s);
	else if(k[0]==1 && k[1]==3)	sprintf(out,"%s /= %g;",a[0].s, a[1].v);
}
//-----------------------------------------------------------------------------
//	{"multo","Multiply by data or number","multo var|num",false,mgls_multo,mglc_multo}
int mgls_multo(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1)		*(a[0].d) *= *(a[1].d);
	else if(k[0]==1 && k[1]==3)	*(a[0].d) *= a[1].v;
	else	return 1;
	return 0;
}
void mglc_multo(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1)		sprintf(out,"%s *= %s;",a[0].s, a[1].s);
	else if(k[0]==1 && k[1]==3)	sprintf(out,"%s *= %g;",a[0].s, a[1].v);
}
//-----------------------------------------------------------------------------
//	{"subto","Subtract data or number","subto var|num",false,mgls_subto,mglc_subto}
int mgls_subto(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1)		*(a[0].d) -= *(a[1].d);
	else if(k[0]==1 && k[1]==3)	*(a[0].d) -= a[1].v;
	else	return 1;
	return 0;
}
void mglc_subto(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1)		sprintf(out,"%s -= %s;",a[0].s, a[1].s);
	else if(k[0]==1 && k[1]==3)	sprintf(out,"%s -= %g;",a[0].s, a[1].v);
}
//-----------------------------------------------------------------------------
//	{"dots","Draw dots for arbitrary data points","dots {xvar yvar zvar} | var [fmt]",false,mgls_dots,mglc_dots}
int mgls_dots(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1)
		gr->Dots(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	else if(k[0]==1)
		gr->Dots(*(a[0].d),k[1]==2?a[1].s:0);
	else	return 1;
	return 0;
}
void mglc_dots(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1)
		sprintf(out,"gr->Dots(%s, %s, %s, \"%s\");",a[0].s,a[1].s,a[2].s,k[3]==2?a[3].s:"");
	else if(k[0]==1)
		sprintf(out,"gr->Dots(%s, \"%s\");",a[0].s, k[1]==2?a[1].s:"");
}
//-----------------------------------------------------------------------------
//	{"diff","Numerically differentiate data","diff var dir",false,mgls_diff,mglc_diff}
int mgls_diff(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2)	a[0].d->Diff(a[1].s);
	else	return 1;
	return 0;
}
void mglc_diff(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2)	sprintf(out,"%s.Diff(\"%s\");", a[0].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"diff2","Numerically double differentiate data","diff2 var dir",false,mgls_diff2,mglc_diff2}
int mgls_diff2(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2)	a[0].d->Diff2(a[1].s);
	else	return 1;
	return 0;
}
void mglc_diff2(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2)	sprintf(out,"%s.Diff2(\"%s\");", a[0].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"drop","Draw drop",0,false,mgls_drop,mglc_drop}
int mgls_drop(mglGraph *gr, long n, mglArg *a, int k[9])
{
	register int i;
	for(i=0;i<7;i++)	if(k[i]!=3)	break;
	if(i==7)
		gr->Drop(mglPoint(a[0].v,a[1].v,a[2].v), mglPoint(a[3].v,a[4].v,a[5].v), a[6].v,
				k[7]==2 ? a[7].s:"b", k[8]==3 ? a[8].v:1.,  (n>9 && a[9].type==2) ? a[9].v:1.);
	else if(i==5)
		gr->Drop(mglPoint(a[0].v,a[1].v), mglPoint(a[2].v,a[3].v), a[4].v,
				k[5]==2 ? a[5].s:"b", k[6]==3 ? a[6].v:1., k[7]==3 ? a[7].v:1.);
	else	return 1;
	return 0;
}
void mglc_drop(char out[1024], long n, mglArg *a, int k[9])
{
	register int i;
	for(i=0;i<7;i++)	if(k[i]!=3)	break;
	if(i==7)
		sprintf(out,"gr->Drop(mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), %g, \"%s\", %g, %g);", a[0].v,a[1].v,a[2].v, a[3].v,a[4].v,a[5].v, a[6].v, k[7]==2 ? a[7].s:"b", k[8]==3 ? a[8].v:1.,  (n>9 && a[9].type==2) ? a[9].v:1.);
	else if(i==5)
		sprintf(out,"gr->Drop(mglPoint(%g, %g), mglPoint(%g, %g), %g, \"%s\", %g, %g);", a[0].v,a[1].v, a[2].v,a[3].v, a[4].v, k[5]==2 ? a[5].s:"b", k[6]==3 ? a[6].v:1., k[7]==3 ? a[7].v:1.);
}
//-----------------------------------------------------------------------------
//	{"dew","Draw dew plot","dew {xvae yvar} uvar vvar [fmt]",false,mgls_dew,mglc_dew}
int mgls_dew(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		gr->Dew(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,k[5]==3?a[5].v:NAN);
	else if(k[0]==1 && k[1]==1)	gr->Dew(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
	else	return 1;
	return 0;
}
void mglc_dew(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		sprintf(out,"gr->Dew(%s, %s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?a[5].v:NAN);
	else if(k[0]==1 && k[1]==1)
		sprintf(out,"gr->Dew(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"fall","Draw waterfalls","fall {xvar yvar} zvar [fmt]",false,mgls_fall,mglc_fall}
int mgls_fall(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)			gr->Fall(*(a[0].d),k[1]==2?a[1].s:0);
	else if(k[1]==1 && k[2]==1)	gr->Fall(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	else	return 1;
	return 0;
}
void mglc_fall(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		sprintf(out,"gr->Fall(%s, \"%s\");", a[0].s,k[1]==2?a[1].s:"");
	else if(k[1]==1 && k[2]==1)
		sprintf(out,"gr->Fall(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"mesh","Draw mesh surface","mesh {xvar yvar} zvar [fmt]",false,mgls_mesh,mglc_mesh}
int mgls_mesh(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)			gr->Mesh(*(a[0].d),k[1]==2?a[1].s:0);
	else if(k[1]==1 && k[2]==1)	gr->Mesh(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	else	return 1;
	return 0;
}
void mglc_mesh(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		sprintf(out,"gr->Mesh(%s, \"%s\");", a[0].s,k[1]==2?a[1].s:"");
	else if(k[1]==1 && k[2]==1)
		sprintf(out,"gr->Mesh(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"surf","Draw solid surface","surf {xvar yvar} zvar [fmt]",false,mgls_surf,mglc_surf}
int mgls_surf(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)			gr->Surf(*(a[0].d),k[1]==2?a[1].s:0);
	else if(k[1]==1 && k[2]==1)	gr->Surf(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	else	return 1;
	return 0;
}
void mglc_surf(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		sprintf(out,"gr->Surf(%s, \"%s\");", a[0].s,k[1]==2?a[1].s:"");
	else if(k[1]==1 && k[2]==1)
		sprintf(out,"gr->Surf(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"surfc","Draw solid surface colored by other data","surfc {xvar yvar} zvar cvar [fmt]",false,mgls_surfc,mglc_surfc}
int mgls_surfc(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1 || k[1]!=1)	return 1;
	else if(k[2]!=1)			gr->SurfC(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0);
	else if(k[2]==1 && k[3]==1)	gr->SurfC(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
	else	return 1;
	return 0;
}
void mglc_surfc(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1 || k[1]!=1)	return;
	else if(k[2]!=1)
		sprintf(out,"gr->SurfC(%s, %s, \"%s\");", a[0].s,a[1].s,k[2]==2?a[2].s:"");
	else if(k[2]==1 && k[3]==1)
		sprintf(out,"gr->SurfC(%s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"");
}
//-----------------------------------------------------------------------------
//	{"surfa","Draw solid surface transpared by other data","surfa {xvar yvar} zvar cvar [fmt]",false,mgls_surfa,mglc_surfa}
int mgls_surfa(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1 || k[1]!=1)	return 1;
	else if(k[2]!=1)			gr->SurfA(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0);
	else if(k[2]==1 && k[3]==1)	gr->SurfA(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
	else	return 1;
	return 0;
}
void mglc_surfa(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1 || k[1]!=1)	return;
	else if(k[2]!=1)
		sprintf(out,"gr->SurfA(%s, %s, \"%s\");", a[0].s,a[1].s,k[2]==2?a[2].s:"");
	else if(k[2]==1 && k[3]==1)
		sprintf(out,"gr->SurfA(%s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"");
}
//-----------------------------------------------------------------------------
//	{"flow","Draw flow threads for vector field",0,false,mgls_flow,mglc_flow}
int mgls_flow(mglGraph *gr, long n, mglArg *a, int k[9])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=1)	break;
	if(i==2)
		gr->Flow(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?int(a[3].v):5,
			true,k[4]==3?a[4].v:NAN);
	else if(i==4)
		gr->Flow(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,
			k[5]==3?int(a[5].v):5,true,k[6]==3?a[6].v:NAN);
	else if(i==3)
		gr->Flow(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0,k[4]==3?int(a[4].v):3);
	else if(i==6)
		gr->Flow(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),*(a[5].d),
			k[6]==2?a[6].s:0,k[7]==3?int(a[7].v):3);
	else	return 1;
	return 0;
}
void mglc_flow(char out[1024], long n, mglArg *a, int k[9])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=1)	break;
	if(i==2)
		sprintf(out,"gr->Flow(%s, %s, \"%s\", %d, true, %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?int(a[3].v):5, k[4]==3?a[4].v:NAN);
	else if(i==4)
		sprintf(out,"gr->Flow(%s, %s, %s, %s, \"%s\", %d, true, %g);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?int(a[5].v):5, k[6]==3?a[6].v:NAN);
	else if(i==3)
		sprintf(out,"gr->Flow(%s, %s, %s, \"%s\", %d);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:0, k[4]==3?int(a[4].v):3);
	else if(i==6)
		sprintf(out,"gr->Flow(%s, %s, %s, %s, %s, %s, \"%s\", %d);", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, a[5].s, k[6]==2?a[6].s:0, k[7]==3?int(a[7].v):3);
}
//-----------------------------------------------------------------------------
//	{"fill","Fill data linearly in range [v1, v2]","fill var v1 v2 [dir]",false,mgls_fill,mglc_fill}
int mgls_fill(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==3 && k[2]==3)	a[0].d->Fill(a[1].v,a[2].v,k[3]==2?a[3].s[0]:'x');
	else	return 1;
	return 0;
}
void mglc_fill(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==3 && k[2]==3)
		sprintf(out,"%s.Fill(%g, %g, '%c');", a[0].s, a[1].v,a[2].v, k[3]==2?a[3].s[0]:'x');
}
//-----------------------------------------------------------------------------
//	{"fog","Switch on/off fog","fog var val [pos]",false,mgls_fog,mglc_fog}
int mgls_fog(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3)	gr->Fog(a[0].v,k[1]==3?a[1].v:0.25);
	else	return 1;
	return 0;
}
void mglc_fog(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3)	sprintf(out,"gr->Fog(%g, %g);", a[0].v,k[1]==3?a[1].v:0.25);
}
//-----------------------------------------------------------------------------
//	{"font","Setup font","font fmt [size]",false,mgls_font,mglc_font}
int mgls_font(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==2)
	{	strcpy(gr->FontDef,a[0].s);	if(k[1]==3)	gr->FontSize = a[1].v;	}
	else	return 1;
	return 0;
}
void mglc_font(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==2 && k[1]==3)
		sprintf(out,"strcpy(gr->FontDef,\"%s\");\tgr->FontSize = %g;", a[0].s, a[1].v);
	else if(k[0]==2)
		sprintf(out,"strcpy(gr->FontDef,\"%s\");",a[0].s);
}
//-----------------------------------------------------------------------------
//	{"grid","Draw grid",0,false,mgls_grid,mglc_grid}
int mgls_grid(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)			gr->Grid(k[0]==2?a[0].s:"xyz", k[1]==2?a[1].s:"B-");
	else if(k[1]!=1)	gr->Grid(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]!=1)
		gr->Grid(*(a[0].d), *(a[1].d), *(a[2].d), k[3]==2?a[3].s:0, k[4]==3?a[4].v:NAN);
	else	return 1;
	return 0;
}
void mglc_grid(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	sprintf(out,"gr->Grid(\"%s\", \"%s\");", k[0]==2?a[0].s:"xyz", k[1]==2?a[1].s:"B-");
	else if(k[1]!=1)
		sprintf(out,"gr->Grid(%s,\"%s\", %g);", a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN);
	else if(k[1]==1 && k[2]==1 && k[3]!=1)
		sprintf(out,"gr->Grid(%s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?a[4].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"grid3","Draw grid at slices of 3D data","grid3 {xvar yvar zvar} avar dir [pos fmt]",false,mgls_grid3,mglc_grid3}
int mgls_grid3(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]==2)
		gr->Grid3(*(a[0].d),a[1].s[0],k[2]==3?int(a[2].v):-1,
				k[3]==2?a[3].s:0);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		gr->Grid3(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),a[4].s[0],
				k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0);
	else	return 1;
	return 0;
}
void mglc_grid3(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]==2)
		sprintf(out,"gr->Grid3(%s, '%c', %d, \"%s\");", a[0].s, a[1].s[0], k[2]==3?int(a[2].v):-1, k[3]==2?a[3].s:"");
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		sprintf(out,"gr->Grid3(%s, %s, %s, %s, '%c', %d, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s[0], k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0);
}
//-----------------------------------------------------------------------------
//	{"grida","Draw grid at central slices of 3D data","grida {xvar yvar zvar} avar [fmt num]",false,mgls_grida,mglc_grida}
int mgls_grida(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		gr->GridA(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
	else	gr->GridA(*(a[0].d),k[1]==2?a[1].s:0);
	return 0;
}
void mglc_grida(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		sprintf(out,"gr->GridA(%s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"");
	else
		sprintf(out,"gr->GridA(%s, \"%s\");", a[0].s, k[1]==2?a[1].s:"");
}
//-----------------------------------------------------------------------------
//	{"light","Setup light",0,false,mgls_light,mglc_light}
int mgls_light(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==0)			gr->Light(true);
	else if(k[0]!=3)	return 1;
	else if(k[1]!=3)	gr->Light(a[0].v!=0);
	else if(k[2]==3 && k[3]==3)
		gr->Light(int(a[0].v),mglPoint(a[1].v,a[2].v,a[3].v),
			k[4]==2?a[4].s[0]:'w',k[5]==3?a[5].v:0.5,true);
	else	gr->Light(int(a[0].v),a[1].v!=0);
	return 0;
}
void mglc_light(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==0)			sprintf(out,"gr->Light(true);");
	else if(k[0]!=3)	return;
	else if(k[1]!=3)	sprintf(out,"gr->Light(%s);", a[0].v!=0 ? "true":"false");
	else if(k[2]==3 && k[3]==3)
		sprintf(out,"gr->Light(%d, mglPoint(%g, %g, %g), '%c', %g, true);", int(a[0].v), a[1].v, a[2].v, a[3].v, k[4]==2?a[4].s[0]:'w', k[5]==3?a[5].v:0.5);
	else	sprintf(out,"gr->Light(%d, %s);", int(a[0].v), a[1].v!=0 ? "true":"false");
}
//-----------------------------------------------------------------------------
//	{"line","Draw line",0,false,mgls_line,mglc_line}
int mgls_line(mglGraph *gr, long n, mglArg *a, int k[9])
{
	int i;
	if(n>5)
	{
		bool ok=true;
		for(i=0;i<6;i++)	if(a[i].type!=2)	ok=false;
		if(ok)
			gr->Line(mglPoint(a[0].v,a[1].v,a[2].v),
				mglPoint(a[3].v,a[4].v,a[5].v),
				(n==7 && a[6].type==1) ? a[6].s : 0, 100);
		else	return 1;
	}
	else if(n>3)
	{
		bool ok=true;
		for(i=0;i<4;i++)	if(a[i].type!=2)	ok=false;
		if(ok)
			gr->Line(mglPoint(a[0].v,a[1].v,gr->Min.z), mglPoint(a[2].v,a[3].v,gr->Min.z),
				(n==5 && a[4].type==1) ? a[4].s : 0, 100);
		else	return 1;
	}
	else	return 1;
	return 0;
}
void mglc_line(char out[1024], long n, mglArg *a, int k[9])
{
	int i;
	if(n>5)
	{
		bool ok=true;
		for(i=0;i<6;i++)	if(a[i].type!=2)	ok=false;
		if(ok)
			sprintf(out,"gr->Line(mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), \"%s\", 100);", a[0].v,a[1].v,a[2].v, a[3].v,a[4].v,a[5].v, (n==7 && a[6].type==1) ? a[6].s : "");
	}
	else if(n>3)
	{
		bool ok=true;
		for(i=0;i<4;i++)	if(a[i].type!=2)	ok=false;
		if(ok)
			sprintf(out,"gr->Line(mglPoint(%g, %g, gr->Min.z), mglPoint(%g, %g, gr->Min.z), \"%s\", 100);", a[0].v,a[1].v,a[2].v, a[3].v, (n==5 && a[4].type==1) ? a[4].s : "");
	}
}
//-----------------------------------------------------------------------------
//	{"legend","Draw legend","legend {xpos ypos} | pos [fmt size]",false,mgls_legend,mglc_legend}
int mgls_legend(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3)
		gr->Legend(a[0].v, a[1].v, k[2]==2?a[2].s:"rL", k[3]==3?a[3].v:-1);
	else
		gr->Legend(k[0]==3?int(a[0].v):3, k[1]==2?a[1].s:"rL", k[2]==3?a[2].v:-1);
	return 0;
}
void mglc_legend(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3)
		sprintf(out,"gr->Legend(%g, %g, \"%s\", %g);", a[0].v, a[1].v, k[2]==2?a[2].s:"rL", k[3]==3?a[3].v:-1);
	else
		sprintf(out,"gr->Legend(%d, \"%s\", %g);", k[0]==3?int(a[0].v):3, k[1]==2?a[1].s:"rL", k[2]==3?a[2].v:-1);
}
//-----------------------------------------------------------------------------
//	{"linewidth","Set default line width","linewidth val",false,mgls_linewidth,mglc_linewidth}
int mgls_linewidth(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3 && a[0].v>=1)	gr->BaseLineWidth = a[0].v;
	else	return 1;
	return 0;
}
void mglc_linewidth(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3 && a[0].v>=1)	sprintf(out,"gr->BaseLineWidth = %g;", a[0].v);
}
//-----------------------------------------------------------------------------
//	{"legendbox","Switch on/off drawing box in legend","legendbox val",false,mgls_legendbox,mglc_legendbox}
int mgls_legendbox(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3)	gr->LegendBox = (a[0].v!=0);
	else	return 1;
	return 0;
}
void mglc_legendbox(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3)	sprintf(out,"gr->LegendBox = %s;", (a[0].v!=0)?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"modify","Modify data values by formula","modify var formula [var1 var2]",false,mgls_modify,mglc_modify}
int mgls_modify(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2 && k[2]==1 && k[3]==1)
		a[0].d->Modify(a[1].s,*(a[2].d),*(a[3].d));
	else if(k[0]==1 && k[1]==2 && k[2]==1)
		a[0].d->Modify(a[1].s,*(a[2].d));
	else if(k[0]==1 && k[1]==2)
		a[0].d->Modify(a[1].s, k[2]==3 ? int(a[2].v):0);
	else	return 1;
	return 0;
}
void mglc_modify(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2 && k[2]==1 && k[3]==1)
		sprintf(out,"%s.Modify(\"%s\", %s, %s);", a[0].s, a[1].s, a[2].s, a[3].s);
	else if(k[0]==1 && k[1]==2 && k[2]==1)
		sprintf(out,"%s.Modify(\"%s\", %s);", a[0].s, a[1].s, a[2].s);
	else if(k[0]==1 && k[1]==2)
		sprintf(out,"%s.Modify(\"%s\", %d);", a[0].s, a[1].s, k[2]==3 ? int(a[2].v):0);
}
//-----------------------------------------------------------------------------
//	{"max","Find maximal value over direction","max ovar ivar dir",true,mgls_max,mglc_max}
int mgls_max(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==2)	*(a[0].d) = a[1].d->Max(a[2].s);
	else	return 1;
	return 0;
}
void mglc_max(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==2)
		sprintf(out,"%s = %s.Max(\"%s\");", a[0].s, a[1].s, a[2].s);
}
//-----------------------------------------------------------------------------
//	{"min","Find minimal value over direction","min ovar ivar dir",true,mgls_min,mglc_min}
int mgls_min(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==2)	*(a[0].d) = a[1].d->Min(a[2].s);
	else	return 1;
	return 0;
}
void mglc_min(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==2)
		sprintf(out,"%s = %s.Min(\"%s\");", a[0].s, a[1].s, a[2].s);
}
//-----------------------------------------------------------------------------
//	{"sum","Find summation over direction","sum ovar ivar dir",true,mgls_sum,mglc_sum}
int mgls_sum(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==2)	*(a[0].d) = a[1].d->Sum(a[2].s);
	else	return 1;
	return 0;
}
void mglc_sum(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==2)
		sprintf(out,"%s = %s.Sum(\"%s\");", a[0].s, a[1].s, a[2].s);
}
//-----------------------------------------------------------------------------
//	{"meshnum","Set number of lines in mesh/fall/vect and so on","meshnum val",false,mgls_meshnum,mglc_meshnum}
int mgls_meshnum(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3)	gr->MeshNum = int(a[0].v);
	else	return 1;
	return 0;
}
void mglc_meshnum(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3)	sprintf(out,"gr->MeshNum = %d;", int(a[0].v));
}
//-----------------------------------------------------------------------------
//	{"marksize","Set size of markers","marksize val",false,mgls_marksize,mglc_marksize}
int mgls_marksize(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3)	gr->MarkSize = a[0].v/50;
	else	return 1;
	return 0;
}
void mglc_marksize(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3)	sprintf(out,"gr->MarkSize = %g;", a[0].v/50);
}
//-----------------------------------------------------------------------------
//	{"mark","Draw mark plot for 1D data","mark {xvar} yvar {{zvar}} rvar [fmt num]",false,mgls_mark,mglc_mark}
int mgls_mark(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1 || k[1]!=1)	return 1;
	else if(k[2]!=1)	gr->Mark(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
	else if(k[3]!=1)	gr->Mark(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0,k[4]==3?a[4].v:NAN);
	else 				gr->Mark(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
	return 0;
}
void mglc_mark(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1 || k[1]!=1)	return;
	else if(k[2]!=1)
		sprintf(out,"gr->Mark(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else if(k[3]!=1)
		sprintf(out,"gr->Mark(%s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?a[4].v:NAN);
	else
		sprintf(out,"gr->Mark(%s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"");
}
//-----------------------------------------------------------------------------
//	{"map","Draw mapping plot","map {xvar yvar} uvar vvar [fmt]",false,mgls_map,mglc_map}
int mgls_map(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		gr->Map(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0, 0,k[5]==3?(a[5].v!=0):true);
	else if(k[0]==1 && k[1]==1)
		gr->Map(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,0,k[3]==3?(a[3].v!=0):true);
	else	return 1;
	return 0;
}
void mglc_map(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		sprintf(out,"gr->Map(%s, %s, %s, %s, \"%s\", 0, %s);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", (k[5]!=3 || a[5].v!=0)?"true":"false");
	else if(k[0]==1 && k[1]==1)
		sprintf(out,"gr->Map(%s, %s, \"%s\", 0, %s);", a[0].s, a[1].s, k[2]==2?a[2].s:"", (k[3]!=3 || a[3].v!=0)?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"read","Read data from file","read var file [nx ny nz]",true,mgls_read,mglc_read}
int mgls_read(mglGraph *gr, long n, mglArg *a, int k[9])
{
	bool rr=true;
	if(k[0]!=1 || k[1]!=2)	return 1;
	else if(n==2)	rr=a[0].d->Read(a[1].s);
	else rr=a[0].d->Read(a[1].s, k[2]==3?int(a[2].v):1,
			k[3]==3?int(a[3].v):1, k[4]==3?int(a[4].v):1);
	if(!rr)	gr->SetWarn(mglWarnFile);
	return 0;
}
void mglc_read(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1 || k[1]!=2)	return;
	else if(n==2)	sprintf(out,"%s.Read(\"%s\");", a[0].s, a[1].s);
	else
		sprintf(out,"%s.Read(\"%s\", %d, %d, %d);", a[0].s, a[1].s, k[2]==3?int(a[2].v):1, k[3]==3?int(a[3].v):1, k[4]==3?int(a[4].v):1);
}
//-----------------------------------------------------------------------------
//	{"readmat","Read data from file with sizes specified in first row","readmat var file [dim=2]",true,mgls_readmat,mglc_readmat}
int mgls_readmat(mglGraph *gr, long n, mglArg *a, int k[9])
{
	bool rr=true;
	if(k[0]!=1 || k[1]!=2)	return 1;
	else	rr=a[0].d->ReadMat(a[1].s, k[2]==3?int(a[2].v):2);
	if(!rr)	gr->SetWarn(mglWarnFile);
	return 0;
}
void mglc_readmat(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2)
		sprintf(out,"%s.ReadMat(\"%s\", %d);", a[0].s, a[1].s, k[2]==3?int(a[2].v):2);
}
//-----------------------------------------------------------------------------
//	{"readall","Read and join data from several files",0,true,mgls_readall,mglc_readall}
int mgls_readall(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1 || k[1]!=2)	return 1;
	else if(k[2]==3 && k[3]==3)
		a[0].d->ReadRange(a[1].s, a[2].v, a[3].v, k[4]==3?a[4].v:1.f, k[5]==3?(a[5].v!=0) : false);
	else a[0].d->ReadAll(a[1].s, k[2]==3?(a[2].v!=0) : false);
	return 0;
}
void mglc_readall(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1 || k[1]!=2)	return;
	else if(k[2]==3 && k[3]==3)
		sprintf(out,"%s.ReadRange(\"%s\", %g, %g, %g, %s);", a[0].s, a[1].s, a[2].v, a[3].v, k[4]==3?a[4].v:1.f, (k[5]==3&&a[5].v!=0)?"true":"false");
	else
		sprintf(out,"%s.ReadAll(\"%s\", %s);", a[0].s, a[1].s, (k[2]==3&&a[2].v!=0)?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"readhdf","Read data from HDF5 file","readhdf var file id",true,mgls_readhdf,mglc_readhdf}
int mgls_readhdf(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2 && k[2]==2)	a[0].d->ReadHDF(a[1].s, a[2].s);
	else	return 1;
	return 0;
}
void mglc_readhdf(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2 && k[2]==2)	sprintf(out,"%s.ReadHDF(\"%s\", \"%s\");", a[0].s, a[1].s, a[2].s);
}
//-----------------------------------------------------------------------------
//	{"rect","Draw rectangle",0,false,mgls_rect,mglc_rect}
int mgls_rect(mglGraph *gr, long n, mglArg *a, int k[9])
{
	long i;
	if(n>5)
	{
		bool ok=true;
		for(i=0;i<6;i++)	if(a[i].type!=2)	ok=false;
		if(ok)
		{
			if(a[0].v==a[3].v)	gr->Face(mglPoint(a[0].v,a[1].v,a[2].v),
				mglPoint(a[0].v,a[4].v,a[2].v),
				mglPoint(a[3].v,a[1].v,a[5].v),
				mglPoint(a[3].v,a[4].v,a[5].v),
				(n==7 && a[6].type==1) ? a[6].s : 0, 2);
			else	gr->Face(mglPoint(a[0].v,a[1].v,a[2].v),
				mglPoint(a[0].v,a[4].v,a[5].v),
				mglPoint(a[3].v,a[1].v,a[2].v),
				mglPoint(a[3].v,a[4].v,a[5].v),
				(n==7 && a[6].type==1) ? a[6].s : 0, 2);
		}
		else	return 1;
	}
	else if(n>3)
	{
		bool ok=true;
		float p = (gr->Min.z+gr->Max.z)/2;
		for(i=0;i<4;i++)	if(a[i].type!=2)	ok=false;
		if(ok)
			gr->Face(mglPoint(a[0].v,a[1].v,p), mglPoint(a[0].v,a[3].v,p),
				mglPoint(a[2].v,a[1].v,p), mglPoint(a[2].v,a[3].v,p),
				(n==5 && a[4].type==1) ? a[4].s : 0, 2);
		else	return 1;
	}
	else	return 1;
	return 0;
}
void mglc_rect(char out[1024], long n, mglArg *a, int k[9])
{
	long i;
	if(n>5)
	{
		bool ok=true;
		for(i=0;i<6;i++)	if(a[i].type!=2)	ok=false;
		if(ok)
		{
			if(a[0].v==a[3].v)
				sprintf(out,"gr->Face(mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), \"%s\", 2);", a[0].v,a[1].v,a[2].v, a[0].v,a[4].v,a[2].v, a[3].v,a[1].v,a[5].v, a[3].v,a[4].v,a[5].v, (n==7 && a[6].type==1) ? a[6].s : "");
			else
				sprintf(out,"gr->Face(mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), mglPoint(%g, %g, %g), \"%s\", 2);", a[0].v,a[1].v,a[2].v, a[0].v,a[4].v,a[5].v, a[3].v,a[1].v,a[2].v, a[3].v,a[4].v,a[5].v, (n==7 && a[6].type==1) ? a[6].s : "");
		}
	}
	else if(n>3)
	{
		bool ok=true;
		for(i=0;i<4;i++)	if(a[i].type!=2)	ok=false;
		if(ok)
			sprintf(out,"gr->Face(mglPoint(%g, %g, (gr->Min.z+gr->Max.z)/2), mglPoint(%g, %g, (gr->Min.z+gr->Max.z)/2), mglPoint(%g, %g, (gr->Min.z+gr->Max.z)/2), mglPoint(%g, %g, (gr->Min.z+gr->Max.z)/2), \"%s\", 2);", a[0].v,a[1].v, a[0].v,a[3].v, a[2].v,a[1].v, a[2].v,a[3].v, (n==5 && a[4].type==1) ? a[4].s : "");
	}
}
//-----------------------------------------------------------------------------
//	{"resize","Resize data","resize ovar ivar [nx ny nz]",true,mgls_resize,mglc_resize}
int mgls_resize(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1)
		*(a[0].d) = a[1].d->Resize(k[2]==3?int(a[2].v):1,
				k[3]==3?int(a[3].v):1, k[4]==3?int(a[4].v):1);
	else	return 1;
	return 0;
}
void mglc_resize(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1)
		sprintf(out,"%s = %s.Resize(%d, %d, %d);", a[0].s, a[1].s, k[2]==3?int(a[2].v):1, k[3]==3?int(a[3].v):1, k[4]==3?int(a[4].v):1);
}
//-----------------------------------------------------------------------------
//	{"rotate","Rotate plot",0,false,mgls_rotate,mglc_rotate}
int mgls_rotate(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3)
		gr->RotateN(a[0].v, a[1].v, a[2].v, a[3].v);
	else if(k[0]==3 && k[1]==3)
		gr->Rotate(a[0].v, a[1].v, k[2]==3?a[2].v:0);
	else	return 1;
	return 0;
}
void mglc_rotate(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3)
		sprintf(out,"gr->RotateN(%g, %g, %g, %g);", a[0].v, a[1].v, a[2].v, a[3].v);
	else if(k[0]==3 && k[1]==3)
		sprintf(out,"gr->Rotate(%g, %g, %g);", a[0].v, a[1].v, k[2]==3?a[2].v:0);
}
//-----------------------------------------------------------------------------
//	{"rotatetext","Set to auto rotate text or not",0,false,mgls_rotatetext,mglc_rotatetext}
int mgls_rotatetext(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3)	gr->RotatedText = (a[0].v!=0);
	else	return 1;
	return 0;
}
void mglc_rotatetext(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3)	sprintf(out,"gr->RotatedText = %s;", (a[0].v!=0)?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"save","Save data to file","save var file",false,mgls_save,mglc_save}
int mgls_save(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2)	a[0].d->Save(a[1].s);
	else	return 1;
	return 0;
}
void mglc_save(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2)	sprintf(out,"%s.Save(\"%s\");", a[0].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"savehdf","Save data to HDF5 file","savehdf var file id",false,mgls_savehdf,mglc_savehdf}
int mgls_savehdf(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2 && k[2]==2)	a[0].d->SaveHDF(a[1].s, a[2].s);
	else	return 1;
	return 0;
}
void mglc_savehdf(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2 && k[2]==2)
		sprintf(out,"%s.SaveHDF(\"%s\", \"%s\");", a[0].s, a[1].s, a[2].s);
}
//-----------------------------------------------------------------------------
//	{"smooth","Smooth data","smooth var kind dir",false,mgls_smooth,mglc_smooth}
int mgls_smooth(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1)		a[0].d->Smooth(k[1]==3?int(a[1].v):2,k[2]==2?a[2].s:"xyz");
	else	return 1;
	return 0;
}
void mglc_smooth(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1)
		sprintf(out,"%s.Smooth(%d, \"%s\");", a[0].s, k[1]==3?int(a[1].v):2, k[2]==2?a[2].s:"xyz");
}
//-----------------------------------------------------------------------------
//	{"swap","Swap data (usefull after Fourier transform)","swap var dir",false,mgls_swap,mglc_swap}
int mgls_swap(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2)	a[0].d->Swap(a[1].s);
	else	return 1;
	return 0;
}
void mglc_swap(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2)	sprintf(out,"%s.Swap(\"%s\");", a[0].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"set_id","Set column id for data","set_id var id",false,mgls_set_id,mglc_set_id}
int mgls_set_id(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2)	a[0].d->SetColumnId(a[1].s);
	else	return 1;
	return 0;
}
void mglc_set_id(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2)	sprintf(out,"%s.SetColumnId(\"%s\");", a[0].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"stem","Draw stem plot for 1D data","stem {xvar} yvar {{zvar}} [fmt num]",false,mgls_stem,mglc_stem}
int mgls_stem(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->Stem(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3 ? a[2].v : NAN);
	else if(k[2]!=1)	gr->Stem(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3 ? a[3].v : NAN);
	else 				gr->Stem(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	return 0;
}
void mglc_stem(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		sprintf(out,"gr->Stem(%s, \"%s\", %g);",a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN);
	else if(k[2]!=1)
		sprintf(out,"gr->Stem(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else 	sprintf(out,"gr->Stem(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"step","Draw step plot for 1D data","step {xvar} yvar {{zvar}} [fmt num]",false,mgls_step,mglc_step}
int mgls_step(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->Step(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3 ? a[2].v : NAN);
	else if(k[2]!=1)	gr->Step(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3 ? a[3].v : NAN);
	else 				gr->Step(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	return 0;
}
void mglc_step(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		sprintf(out,"gr->Step(%s, \"%s\", %g);",a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN);
	else if(k[2]!=1)
		sprintf(out,"gr->Step(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else 	sprintf(out,"gr->Step(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"plot","Draw usual plot for 1D data","plot {xvar} yvar {{zvar}} [fmt num]",false,mgls_plot,mglc_plot}
int mgls_plot(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->Plot(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3 ? a[2].v : NAN);
	else if(k[2]!=1)	gr->Plot(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3 ? a[3].v : NAN);
	else 				gr->Plot(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	return 0;
}
void mglc_plot(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		sprintf(out,"gr->Plot(%s, \"%s\", %g);",a[0].s, k[1]==2?a[1].s:"", k[2]==3?a[2].v:NAN);
	else if(k[2]!=1)
		sprintf(out,"gr->Plot(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else 	sprintf(out,"gr->Plot(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"squeeze","Squeeze data","squeeze var kx [ky kz]",false,mgls_squeeze,mglc_squeeze}
int mgls_squeeze(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==3)
		a[0].d->Squeeze(int(a[1].v), k[2]==3?int(a[2].v):1, k[3]==3?int(a[3].v):1);
	else	return 1;
	return 0;
}
void mglc_squeeze(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==3)
		sprintf(out,"%s.Squeeze(%d, %d, %d);", a[0].s, int(a[1].v), k[2]==3?int(a[2].v):1, k[3]==3?int(a[3].v):1);
}
//-----------------------------------------------------------------------------
//	{"stfad","Do STFA transform","",true,mgls_stfad,mglc_stfad}
int mgls_stfad(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==3)
		*(a[0].d) = ::STFA(*(a[1].d),*(a[2].d), int(a[3].v), k[4]==2?a[4].s[0]:'x');
	else	return 1;
	return 0;
}
void mglc_stfad(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==3)
		sprintf(out,"%s = ::STFA(%s, %s, %d, '%c');", a[0].s, a[1].s, a[2].s, int(a[3].v), k[4]==2?a[4].s[0]:'x');
}
//-----------------------------------------------------------------------------
//	{"setsize","Set picture size","setsize wval hval",false,mgls_setsize,mglc_setsize}
int mgls_setsize(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3 && a[1].v>1 && a[0].v>1)
		gr->SetSize(int(a[0].v), int(a[1].v));
	else	return 1;
	return 0;
}
void mglc_setsize(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3 && a[1].v>1 && a[0].v>1)
		sprintf(out,"gr->SetSize(%d, %d);", int(a[0].v), int(a[1].v));
}
//-----------------------------------------------------------------------------
//	{"sphere","Draw sphere",0,false,mgls_sphere,mglc_sphere}
int mgls_sphere(mglGraph *gr, long n, mglArg *a, int k[9])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=3)	break;
	if(i==4)
		gr->Sphere(mglPoint(a[0].v,a[1].v,a[2].v), a[3].v, k[4]==2 ? a[4].s:"r");
	else if(i==3)
		gr->Sphere(mglPoint(a[0].v,a[1].v), a[2].v, k[3]==2 ? a[3].s:"r");
	else	return 1;
	return 0;
}
void mglc_sphere(char out[1024], long n, mglArg *a, int k[9])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=3)	break;
	if(i==4)
		sprintf(out,"gr->Sphere(mglPoint(%g, %g, %g), %g, \"%s\");", a[0].v,a[1].v,a[2].v, a[3].v, k[4]==2 ? a[4].s:"r");
	else if(i==3)
		sprintf(out,"gr->Sphere(mglPoint(%g, %g), %g, \"%s\");", a[0].v,a[1].v, a[2].v, k[3]==2 ? a[3].s:"r");
}
//-----------------------------------------------------------------------------
//	{"stfa","Draw STFA diagram","stfa {xvar yvar} uvar vvar num [fmt]",false,mgls_stfa,mglc_stfa}
int mgls_stfa(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==3)
		gr->STFA(*(a[0].d),*(a[1].d), int(a[2].v), k[3]==2?a[3].s:"");
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==3)
		gr->STFA(*(a[0].d),*(a[1].d), *(a[2].d),*(a[3].d), int(a[4].v), k[5]==2?a[5].s:"");
	else	return 1;
	return 0;
}
void mglc_stfa(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==3)
		sprintf(out,"gr->STFA(%s, %s, %d, \"%s\");", a[0].s, a[1].s, int(a[2].v), k[3]==2?a[3].s:"");
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==3)
		sprintf(out,"gr->STFA(%s, %s, %s, %s, %d, \"%s\");", a[0].s, a[1].s,  a[2].s, a[3].s, int(a[4].v), k[5]==2?a[5].s:"");
}
//-----------------------------------------------------------------------------
//	{"surf3","Draw isosurface for 3D data","surf3 {xvar yvar zvar} avar {num} [fmt]",false,mgls_surf3,mglc_surf3}
int mgls_surf3(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]==0)	gr->Surf3(*(a[0].d));
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==3)
		gr->Surf3(a[4].v,*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),
				k[5]==2? a[5].s:0);
	else if(k[1]==3)	gr->Surf3(a[1].v,*(a[0].d),k[2]==2? a[2].s:0);
	else if(k[1]==2)	gr->Surf3(*(a[0].d),a[1].s,int(k[2]==3?a[2].v:3));
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		gr->Surf3(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),
				k[4]==2? a[4].s:0,int(k[5]==3?a[5].v:3));
	else	return 1;
	return 0;
}
void mglc_surf3(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]==0)	sprintf(out,"gr->Surf3(%s);", a[0].s);
	else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==3)
		sprintf(out,"gr->Surf3(%g, %s, %s, %s, %s, \"%s\");", a[4].v, a[0].s, a[1].s, a[2].s, a[3].s, k[5]==2?a[5].s:"");
	else if(k[1]==3)
		sprintf(out,"gr->Surf3(%g, %s, \"%s\");", a[1].v, a[0].s, k[2]==2? a[2].s:"");
	else if(k[1]==2)
		sprintf(out,"gr->Surf3(%s, \"%s\", %d);",  a[0].s, a[1].s, int(k[2]==3?a[2].v:3));
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		sprintf(out,"gr->Surf3(%s, %s, %s, %s, \"%s\", %d);",  a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", int(k[5]==3?a[5].v:3));
}
//-----------------------------------------------------------------------------
//	{"surf3c","Draw isosurface for 3D data colored by other data","surf3c {xvar yvar zvar} avar cvar {num} [fmt]",false,mgls_surf3c,mglc_surf3c}
int mgls_surf3c(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1 || k[1]!=1)	return 1;
	else if(k[2]==0)	gr->Surf3C(*(a[0].d),*(a[1].d));
	else if(k[2]==1 && k[3]==1 && k[4]==1 && k[5]==3)
		gr->Surf3C(a[5].v,*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),
				k[6]==2? a[6].s:0);
	else if(k[2]==3)	gr->Surf3C(a[2].v,*(a[0].d),*(a[1].d),k[3]==2? a[3].s:0);
	else if(k[2]==2)	gr->Surf3C(*(a[0].d),*(a[1].d),a[2].s,int(k[3]==3?a[3].v:3));
	else if(k[2]==1 && k[3]==1 && k[4]==1)
		gr->Surf3C(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),
				k[5]==2? a[5].s:0,int(k[6]==3?a[6].v:3));
	else	return 1;
	return 0;
}
void mglc_surf3c(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1 || k[1]!=1)	return;
	else if(k[2]==0)	sprintf(out,"gr->Surf3C(%s, %s);",  a[0].s, a[1].s);
	else if(k[2]==1 && k[3]==1 && k[4]==1 && k[5]==3)
		sprintf(out,"gr->Surf3C(%g, %s, %s, %s, %s, %s, \"%s\");", a[5].v, a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, k[6]==2? a[6].s:"");
	else if(k[2]==3)
		sprintf(out,"gr->Surf3C(%g, %s, %s, \"%s\");", a[2].v, a[0].s, a[1].s, k[3]==2?a[3].s:0);
	else if(k[2]==2)
		sprintf(out,"gr->Surf3C(%s, %s, \"%s\", %d);",  a[0].s, a[1].s, a[2].s, int(k[3]==3?a[3].v:3));
	else if(k[2]==1 && k[3]==1 && k[4]==1)
		sprintf(out,"gr->Surf3C(%s, %s, %s, %s, %s, \"%s\", %d);",  a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, k[5]==2?a[5].s:"", int(k[6]==3?a[6].v:3));
}
//-----------------------------------------------------------------------------
//	{"surf3a","Draw isosurface for 3D data transpared by other data","surf3a {xvar yvar zvar} avar cvar {num} [fmt]",false,mgls_surf3a,mglc_surf3a}
int mgls_surf3a(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1 || k[1]!=1)	return 1;
	else if(k[2]==0)	gr->Surf3A(*(a[0].d),*(a[1].d));
	else if(k[2]==1 && k[3]==1 && k[4]==1 && k[5]==3)
		gr->Surf3A(a[5].v,*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),
				k[6]==2? a[6].s:0);
	else if(k[2]==3)	gr->Surf3A(a[2].v,*(a[0].d),*(a[1].d),k[3]==2? a[3].s:0);
	else if(k[2]==2)	gr->Surf3A(*(a[0].d),*(a[1].d),a[2].s,int(k[3]==3?a[3].v:3));
	else if(k[2]==1 && k[3]==1 && k[4]==1)
		gr->Surf3A(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),
				k[5]==2? a[5].s:0,int(k[6]==3?a[6].v:3));
	else	return 1;
	return 0;
}
void mglc_surf3a(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1 || k[1]!=1)	return;
	else if(k[2]==0)	sprintf(out,"gr->Surf3A(%s, %s);",  a[0].s, a[1].s);
	else if(k[2]==1 && k[3]==1 && k[4]==1 && k[5]==3)
		sprintf(out,"gr->Surf3A(%g, %s, %s, %s, %s, %s, \"%s\");", a[5].v, a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, k[6]==2? a[6].s:"");
	else if(k[2]==3)
		sprintf(out,"gr->Surf3A(%g, %s, %s, \"%s\");", a[2].v, a[0].s, a[1].s, k[3]==2?a[3].s:0);
	else if(k[2]==2)
		sprintf(out,"gr->Surf3A(%s, %s, \"%s\", %d);",  a[0].s, a[1].s, a[2].s, int(k[3]==3?a[3].v:3));
	else if(k[2]==1 && k[3]==1 && k[4]==1)
		sprintf(out,"gr->Surf3A(%s, %s, %s, %s, %s, \"%s\", %d);",  a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, k[5]==2?a[5].s:"", int(k[6]==3?a[6].v:3));
}
//-----------------------------------------------------------------------------
//	{"subplot","Set position of plot","subplot mval nval ival",false,mgls_subplot,mglc_subplot}
int mgls_subplot(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3 && k[2]==3)
		gr->SubPlot(int(a[0].v), int(a[1].v), int(a[2].v), k[3]==3?a[3].v:0, k[4]==3?a[4].v:0);
	else	return 1;
	return 0;
}
void mglc_subplot(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3 && k[2]==3)
		sprintf(out,"gr->SubPlot(%d, %d, %d, %g, %g);", int(a[0].v), int(a[1].v), int(a[2].v), k[3]==3?a[3].v:0, k[4]==3?a[4].v:0);
}
//-----------------------------------------------------------------------------
//	{"subdata","Extract sub-array","subdata ovar ivar nx [ny nz]",true,mgls_subdata,mglc_subdata}
int mgls_subdata(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==3)
		*(a[0].d) = a[1].d->SubData(int(a[2].v), k[3]==3?int(a[3].v):-1, k[4]==3?int(a[4].v):-1);
	else	return 1;
	return 0;
}
void mglc_subdata(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==3)
		sprintf(out,"%s = %s.SubData(%d, %d, %d);",  a[0].s, a[1].s, int(a[2].v), k[3]==3?int(a[3].v):-1, k[4]==3?int(a[4].v):-1);
}
//-----------------------------------------------------------------------------
//	{"tile","Draw horizontal tiles","tile {xvar yvar} zvar {rvar} [fmt]",false,mgls_tile,mglc_tile}
int mgls_tile(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return 1;
	else if(k[1]!=1)	gr->Tile(*(a[0].d), k[1]==2? a[1].s:0);
	else if(k[1]==1 && k[2]!=1)
		gr->Tile(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0);
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		gr->Tile(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2? a[4].s:0);
	else if(k[1]==1 && k[2]==1)
		gr->Tile(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2? a[3].s:0);
	else	return 1;
	return 0;
}
void mglc_tile(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]!=1)	return;
	else if(k[1]!=1)
		sprintf(out,"gr->Tile(%s, \"%s\");", a[0].s, k[1]==2?a[1].s:"");
	else if(k[1]==1 && k[2]!=1)
		sprintf(out,"gr->Tile(%s, %s, \"%s\");", a[0].s, a[1].s, k[2]==2?a[2].s:"");
	else if(k[1]==1 && k[2]==1 && k[3]==1)
		sprintf(out,"gr->Tile(%s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"");
	else if(k[1]==1 && k[2]==1)
		sprintf(out,"gr->Tile(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"");
}
//-----------------------------------------------------------------------------
//	{"text","Draw text at some position or along curve",0,false,mgls_text,mglc_text}
int mgls_text(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==2)
		gr->Text(mglPoint(a[0].v,a[1].v,a[2].v),a[3].s,
			(k[4]==2 && a[4].s[0]!=0)?a[4].s:gr->FontDef,k[5]==3?a[5].v:-1.4);
	else if(k[0]==3 && k[1]==3 && k[2]==2)
		gr->Text(mglPoint(a[0].v,a[1].v),a[2].s,
			(k[3]==2 && a[3].s[0]!=0)?a[3].s:gr->FontDef,k[4]==3?a[4].v:-1.4);
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==2)
		gr->Text(*(a[0].d),*(a[1].d),*(a[2].d),a[3].s,k[4]==2?a[4].s:0,k[5]==3?a[5].v:-1);
	else if(k[0]==1 && k[1]==1 && k[2]==2)
		gr->Text(*(a[0].d),*(a[1].d),a[2].s,k[3]==2?a[3].s:0,
			k[4]==3?a[4].v:-1,k[5]==3?a[5].v:NAN);
	else if(k[0]==1 && k[1]==2)
		gr->Text(*(a[0].d),a[1].s,k[2]==2?a[2].s:0,k[3]==3?a[3].v:-1,k[4]==3?a[4].v:NAN);
	else	return 1;
	return 0;
}
void mglc_text(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==2)
		sprintf(out,"gr->Text(mglPoint(%g, %g, %g), \"%s\", \"%s\", %g);", a[0].v, a[1].v, a[2].v, a[3].s, (k[4]==2 && a[4].s[0]!=0)?a[4].s:"", k[5]==3?a[5].v:-1.4);
	else if(k[0]==3 && k[1]==3 && k[2]==2)
		sprintf(out,"gr->Text(mglPoint(%g, %g), \"%s\", \"%s\", %g);", a[0].v, a[1].v, a[2].s, (k[3]==2 && a[3].s[0]!=0)?a[3].s:"", k[4]==3?a[4].v:-1.4);
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==2)
		sprintf(out,"gr->Text(%s, %s, %s, \"%s\", \"%s\", %g);",  a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?a[5].v:-1);
	else if(k[0]==1 && k[1]==1 && k[2]==2)
		sprintf(out,"gr->Text(%s, %s, \"%s\", \"%s\", %g, %g);",  a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?a[4].v:-1, k[5]==3?a[5].v:NAN);
	else if(k[0]==1 && k[1]==2)
		sprintf(out,"gr->Text(%s, \"%s\", \"%s\", %g, %g);",  a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:-1, k[4]==3?a[4].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"torus","Draw surface of curve rotation","torus {zvar} rvar [fmt]",false,mgls_torus,mglc_torus}
int mgls_torus(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1)	gr->Torus(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0);
	else if(k[0]==1)		gr->Torus(*(a[0].d),k[1]==2?a[1].s:0);
	else	return 1;
	return 0;
}
void mglc_torus(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1)	sprintf(out,"gr->Torus(%s, %s, \"%s\");", a[0].s, a[1].s, k[2]==2?a[2].s:"");
	else if(k[0]==1)		sprintf(out,"gr->Torus(%s, \"%s\");", a[0].s, k[1]==2?a[1].s:"");
}
//-----------------------------------------------------------------------------
//	{"transparent","Switch off transparency","transparent val",false,mgls_transparent,mglc_transparent} //!!! OLD !!!
int mgls_transparent(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3)	gr->Transparent = (a[0].v!=0);
	else	return 1;
	return 0;
}
void mglc_transparent(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3)	sprintf(out,"gr->Transparent = %s;", (a[0].v!=0)?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"transptype","Set type transparency","transptype val",false,mgls_transptype,mglc_transptype}
int mgls_transptype(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3)	gr->TranspType = int(a[0].v);
	else	return 1;
	return 0;
}
void mglc_transptype(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3)	sprintf(out,"gr->TranspType = %d;", int(a[0].v));
}
//-----------------------------------------------------------------------------
//	{"transform","Do integral transform of data","transform ovar how rvar ivar",true,mgls_transform,mglc_transform}
int mgls_transform(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2 && k[2]==1 && k[3]==1)
		*(a[0].d) = Transform(*(a[2].d),*(a[3].d),a[1].s);
	else	return 1;
	return 0;
}
void mglc_transform(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2 && k[2]==1 && k[3]==1)
		sprintf(out,"%s = Transform(%s, %s, \"%s\");",a[0].s,  a[2].s, a[3].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"transforma","Do integral transform of data","transforma ovar how avar fvar",true,mgls_transforma,mglc_transforma}
int mgls_transforma(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2 && k[2]==1 && k[3]==1)
		*(a[0].d) = TransformA(*(a[2].d),*(a[3].d),a[1].s);
	else	return 1;
	return 0;
}
void mglc_transforma(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2 && k[2]==1 && k[3]==1)
		sprintf(out,"%s = TransformA(%s, %s, \"%s\");",a[0].s,  a[2].s, a[3].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"tube","Draw curve by tube",0,false,mgls_tube,mglc_tube}
int mgls_tube(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		gr->Tube(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==3)
		gr->Tube(*(a[0].d),*(a[1].d),*(a[2].d),a[3].v,k[4]==2?a[4].s:0);
	else if(k[0]==1 && k[1]==1 && k[2]==1)
		gr->Tube(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0,k[4]==3?a[4].v:NAN);
	else if(k[0]==1 && k[1]==1 && k[2]==3)
		gr->Tube(*(a[0].d),*(a[1].d),a[2].v,k[3]==2?a[3].s:0,k[4]==3?a[4].v:NAN);
	else if(k[0]==1 && k[1]==1)
		gr->Tube(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
	else if(k[0]==1 && k[1]==3)
		gr->Tube(*(a[0].d),a[1].v,k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
	else	return 1;
	return 0;
}
void mglc_tube(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		sprintf(out,"gr->Tube(%s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"");
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==3)
		sprintf(out,"gr->Tube(%s, %s, %s, %g, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].v, k[4]==2?a[4].s:"");
	else if(k[0]==1 && k[1]==1 && k[2]==1)
		sprintf(out,"gr->Tube(%s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?a[4].v:NAN);
	else if(k[0]==1 && k[1]==1 && k[2]==3)
		sprintf(out,"gr->Tube(%s, %s, %g, \"%s\", %g);", a[0].s, a[1].s, a[2].v, k[3]==2?a[3].s:"", k[4]==3?a[4].v:NAN);
	else if(k[0]==1 && k[1]==1)
		sprintf(out,"gr->Tube(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else if(k[0]==1 && k[1]==3)
		sprintf(out,"gr->Tube(%s, %g, \"%s\", %g);", a[0].s, a[1].v, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"textmark","Draw TeX mark at point position",0,false,mgls_textmark,mglc_textmark}
int mgls_textmark(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		gr->TextMark(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),a[4].s,k[5]==2?a[5].s:0);
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==2)
		gr->TextMark(*(a[0].d),*(a[1].d),*(a[2].d),a[3].s,k[4]==2?a[4].s:0,k[5]==3?a[5].v:NAN);
	else if(k[0]==1 && k[1]==1 && k[2]==2)
		gr->TextMark(*(a[0].d),*(a[1].d),a[2].s,k[3]==2?a[3].s:0,k[4]==3?a[4].v:NAN);
	else if(k[0]==1 && k[1]==2)
		gr->TextMark(*(a[0].d),a[1].s,k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
	else	return 1;
	return 0;
}
void mglc_textmark(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
		sprintf(out,"gr->TextMark(%s, %s, %s, %s, \"%s\", \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, k[5]==2?a[5].s:"");
	else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==2)
		sprintf(out,"gr->TextMark(%s, %s, %s, \"%s\", \"%s\", %g);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?a[5].v:NAN);
	else if(k[0]==1 && k[1]==1 && k[2]==2)
		sprintf(out,"gr->TextMark(%s, %s, \"%s\", \"%s\", %g);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?a[4].v:NAN);
	else if(k[0]==1 && k[1]==2)
		sprintf(out,"gr->TextMark(%s, \"%s\", \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"triplot","Draw surface of triangles",0,false,mgls_triplot,mglc_triplot}
int mgls_triplot(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		gr->TriPlot(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
	else if(k[0]==1 && k[1]==1 && k[2]==1)
		gr->TriPlot(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0,k[4]==3?a[4].v:NAN);
	else	return 1;
	return 0;
}
void mglc_triplot(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		sprintf(out,"gr->TriPlot(%s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"");
	else if(k[0]==1 && k[1]==1 && k[2]==1)
		sprintf(out,"gr->TriPlot(%s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?a[4].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"ternary","Switch on/off to use ternary axis","ternary val",false,mgls_ternary,mglc_ternary}
int mgls_ternary(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3)	gr->Ternary(a[0].v!=0);
	else	return 1;
	return 0;
}
void mglc_ternary(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3)	sprintf(out,"gr->Ternary(%s);", a[0].v!=0?"true":"false");
}
//-----------------------------------------------------------------------------
//	{"transpose","Transpose data array","transpose var [dir]",false,mgls_transpose,mglc_transpose}
int mgls_transpose(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1)	a[0].d->Transpose(k[1]==2?a[1].s:"yxz");
	else	return 1;
	return 0;
}
void mglc_transpose(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1)	sprintf(out,"%s.Transpose(\"%s\");", a[0].s, k[1]==2?a[1].s:"yxz");
}
//-----------------------------------------------------------------------------
//	{"vectc","Draw vector field with coloring",0,false,mgls_vectc,mglc_vectc}
int mgls_vectc(mglGraph *gr, long n, mglArg *a, int k[9])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=1)	break;
	if(i==2)
		gr->VectC(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
	else if(i==4)
		gr->VectC(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,k[5]==3?a[5].v:NAN);
	else if(i==3)
		gr->VectC(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	else if(i==6)
		gr->VectC(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),*(a[5].d),k[6]==2?a[6].s:0);
	else	return 1;
	return 0;
}
void mglc_vectc(char out[1024], long n, mglArg *a, int k[9])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=1)	break;
	if(i==2)
		sprintf(out,"gr->VectC(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else if(i==4)
		sprintf(out,"gr->VectC(%s, %s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?a[5].v:NAN);
	else if(i==3)
		sprintf(out,"gr->VectC(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:0);
	else if(i==6)
		sprintf(out,"gr->VectC(%s, %s, %s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, a[5].s, k[6]==2?a[6].s:"");
}
//-----------------------------------------------------------------------------
//	{"vect","Draw vector field",0,false,mgls_vect,mglc_vect}
int mgls_vect(mglGraph *gr, long n, mglArg *a, int k[9])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=1)	break;
	if(i==2)
		gr->Vect(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
	else if(i==4)
		gr->Vect(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,k[5]==3?a[5].v:NAN);
	else if(i==3)
		gr->Vect(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
	else if(i==6)
		gr->Vect(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),*(a[5].d),k[6]==2?a[6].s:0);
	else	return 1;
	return 0;
}
void mglc_vect(char out[1024], long n, mglArg *a, int k[9])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=1)	break;
	if(i==2)
		sprintf(out,"gr->Vect(%s, %s, \"%s\", %g);", a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
	else if(i==4)
		sprintf(out,"gr->Vect(%s, %s, %s, %s, \"%s\", %g);", a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?a[5].v:NAN);
	else if(i==3)
		sprintf(out,"gr->Vect(%s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:0);
	else if(i==6)
		sprintf(out,"gr->Vect(%s, %s, %s, %s, %s, %s, \"%s\");", a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, a[5].s, k[6]==2?a[6].s:"");
}
//-----------------------------------------------------------------------------
//	{"xlabel","Draw label for x-axis","xlabel txt [fnt pos]",false,mgls_xlabel,mglc_xlabel}
int mgls_xlabel(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==2)	gr->Label('x',a[0].s,k[1]==3?int(a[1].v):1);
	else	return 1;
	return 0;
}
void mglc_xlabel(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==2)	sprintf(out,"gr->Label('x',\"%s\",%d);", a[0].s, k[1]==3?int(a[1].v):1);
}
//-----------------------------------------------------------------------------
//	{"ylabel","Draw label for y-axis","ylabel txt [fnt pos]",false,mgls_ylabel,mglc_ylabel}
int mgls_ylabel(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==2)	gr->Label('y',a[0].s,k[1]==3?int(a[1].v):1);
	else	return 1;
	return 0;
}
void mglc_ylabel(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==2)	sprintf(out,"gr->Label('y',\"%s\",%d);", a[0].s, k[1]==3?int(a[1].v):1);
}
//-----------------------------------------------------------------------------
//	{"zlabel","Draw label for z-axis","zlabel txt [fnt pos]",false,mgls_zlabel,mglc_zlabel}
int mgls_zlabel(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==2)	gr->Label('z',a[0].s,k[1]==3?int(a[1].v):1);
	else	return 1;
	return 0;
}
void mglc_zlabel(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==2)	sprintf(out,"gr->Label('z',\"%s\",%d);", a[0].s, k[1]==3?int(a[1].v):1);
}
//-----------------------------------------------------------------------------
//	{"xrange","Set range for x-axis","xrange {var [add]} | {x1 x2}",false,mgls_xrange,mglc_xrange}
int mgls_xrange(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1)	gr->XRange(*(a[0].d),k[1]==3?(a[1].v!=0):false);
	else if(k[0]==3 && k[1]==3)
	{	gr->Min.x = a[0].v;	gr->Max.x = a[1].v;	gr->RecalcBorder();	}
	else	return 1;
	return 0;
}
void mglc_xrange(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1)
		sprintf(out,"gr->XRange(%s, %s);", a[0].s, (k[1]==3&&a[1].v!=0)?"true":"false");
	else if(k[0]==3 && k[1]==3)
		sprintf(out,"gr->Min.x = %g;\tgr->Max.x = %g;\tgr->RecalcBorder();", a[0].v, a[1].v);
}
//-----------------------------------------------------------------------------
//	{"yrange","Set range for y-axis","yrange {var [add]} | {x1 x2}",false,mgls_yrange,mglc_yrange}
int mgls_yrange(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1)	gr->YRange(*(a[0].d),k[1]==3?(a[1].v!=0):false);
	else if(k[0]==3 && k[1]==3)
	{	gr->Min.y = a[0].v;	gr->Max.y = a[1].v;	gr->RecalcBorder();	}
	else	return 1;
	return 0;
}
void mglc_yrange(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1)
		sprintf(out,"gr->YRange(%s, %s);", a[0].s, (k[1]==3&&a[1].v!=0)?"true":"false");
	else if(k[0]==3 && k[1]==3)
		sprintf(out,"gr->Min.y = %g;\tgr->Max.y = %g;\tgr->RecalcBorder();", a[0].v, a[1].v);
}
//-----------------------------------------------------------------------------
//	{"zrange","Set range for z-axis","yrange {var [add]} | {x1 x2}",false,mgls_zrange,mglc_zrange}
int mgls_zrange(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1)	gr->ZRange(*(a[0].d),k[1]==3?(a[1].v!=0):false);
	else if(k[0]==3 && k[1]==3)
	{	gr->Min.z = a[0].v;	gr->Max.z = a[1].v;	gr->RecalcBorder();	}
	else	return 1;
	return 0;
}
void mglc_zrange(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1)
		sprintf(out,"gr->ZRange(%s, %s);", a[0].s, (k[1]==3&&a[1].v!=0)?"true":"false");
	else if(k[0]==3 && k[1]==3)
		sprintf(out,"gr->Min.z = %g;\tgr->Max.z = %g;\tgr->RecalcBorder();", a[0].v, a[1].v);
}
//-----------------------------------------------------------------------------
//	{"xtick","Set ticks for x-axis","xtick {val [sub]} | tmpl",false,mgls_xtick,mglc_xtick}
int mgls_xtick(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3)	{	gr->dx = a[0].v;	if(k[1]==3)	gr->NSx = int(a[1].v);	}
	else if(k[0]==2)
	{
		if(gr->xtt)	delete []gr->xtt;
		gr->xtt = a[0].s[0] ? mgl_str_copy(a[0].s) : 0;
	}
	else	return 1;
	return 0;
}
void mglc_xtick(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3)	sprintf(out,"gr->dx = %g;\tgr->NSx = %d;", a[0].v, int(a[1].v));
	else if(k[0]==3)		sprintf(out,"gr->dx = %g;", a[0].v);
	else if(k[0]==2)
		sprintf(out,"if(gr->xtt) delete []gr->xtt;\tgr->xtt = '%c' ? mgl_str_copy(\"%s\") : 0;", a[0].s[0], a[0].s);
}
//-----------------------------------------------------------------------------
//	{"ytick","Set ticks for y-axis","ytick {val [sub]} | tmpl",false,mgls_ytick,mglc_ytick}
int mgls_ytick(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3)	{	gr->dy = a[0].v;	if(k[1]==3)	gr->NSy = int(a[1].v);	}
	else if(k[0]==2)
	{
		if(gr->ytt)	delete []gr->ytt;
		gr->ytt = a[0].s[0] ? mgl_str_copy(a[0].s) : 0;
	}
	else	return 1;
	return 0;
}
void mglc_ytick(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3)	sprintf(out,"gr->dy = %g;\tgr->NSy = %d;", a[0].v, int(a[1].v));
	else if(k[0]==3)		sprintf(out,"gr->dy = %g;", a[0].v);
	else if(k[0]==2)
		sprintf(out,"if(gr->ytt) delete []gr->ytt;\tgr->ytt = '%c' ? mgl_str_copy(\"%s\") : 0;", a[0].s[0], a[0].s);
}
//-----------------------------------------------------------------------------
//	{"ztick","Set ticks for z-axis","ztick {val [sub]} | tmpl",false,mgls_ztick,mglc_ztick}
int mgls_ztick(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3)	{	gr->dz = a[0].v;	if(k[1]==3)	gr->NSz = int(a[1].v);	}
	else if(k[0]==2)
	{
		if(gr->ztt)	delete []gr->ztt;
		gr->ztt = a[0].s[0] ? mgl_str_copy(a[0].s) : 0;
	}
	else	return 1;
	return 0;
}
void mglc_ztick(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3)	sprintf(out,"gr->dz = %g;\tgr->NSz = %d;", a[0].v, int(a[1].v));
	else if(k[0]==3)		sprintf(out,"gr->dz = %g;", a[0].v);
	else if(k[0]==2)
		sprintf(out,"if(gr->ztt) delete []gr->ztt;\tgr->ztt = '%c' ? mgl_str_copy(\"%s\") : 0;", a[0].s[0], a[0].s);
}
//-----------------------------------------------------------------------------
//	{"zoom","Zoom in/out plot","zoom x1 y1 x2 y2",false,mgls_zoom,mglc_zoom}
int mgls_zoom(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3)
		gr->Zoom(a[0].v, a[1].v, a[2].v, a[3].v);
	else	return 1;
	return 0;
}
void mglc_zoom(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3)
		sprintf(out,"gr->Zoom(%g, %g, %g, %g);", a[0].v, a[1].v, a[2].v, a[3].v);
}
//-----------------------------------------------------------------------------
//	{"error","Draw error boxes",0,false,mgls_error,mglc_error}
int mgls_error(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		gr->Error(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),
			k[4]==2?a[4].s:0, k[5]==3?a[5].v:NAN);
	else if(k[0]==1 && k[1]==1 && k[2]==1)
		gr->Error(*(a[0].d),*(a[1].d),*(a[2].d), k[3]==2?a[3].s:0, k[4]==3?a[4].v:NAN);
	else if(k[0]==1 && k[1]==1)
		gr->Error(*(a[0].d),*(a[1].d), k[2]==2?a[2].s:0, k[3]==3?a[3].v:NAN);
	else	return 1;
	return 0;
}
void mglc_error(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
		sprintf(out,"gr->Error(%s, %s, %s, %s, \"%s\", %g);",  a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?a[5].v:NAN);
	else if(k[0]==1 && k[1]==1 && k[2]==1)
		sprintf(out,"gr->Error(%s, %s, %s, \"%s\", %g);",  a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?a[4].v:NAN);
	else if(k[0]==1 && k[1]==1)
		sprintf(out,"gr->Error(%s, %s, \"%s\", %g);",  a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:NAN);
}
//-----------------------------------------------------------------------------
//	{"extend","Extend data array",0,false,mgls_extend,mglc_extend}
int mgls_extend(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==3)	a[0].d->Extend(int(a[1].v),k[2]==3?int(a[2].v):0);
	else	return 1;
	return 0;
}
void mglc_extend(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==3)
		sprintf(out,"%s.Extend(%d, %d);", a[0].s, int(a[1].v), k[2]==3?int(a[2].v):0);
}
//-----------------------------------------------------------------------------
//	{"info","Print information about data","info var",false,mgls_info,mglc_info}
int mgls_info(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1)	a[0].d->PrintInfo(gr->Message);
	else	return 1;
	return 0;
}
void mglc_info(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1)	sprintf(out,"%s.PrintInfo(gr->Message);", a[0].s);
}
//-----------------------------------------------------------------------------
//	{"integrate","Integrate data","integrate var dir",false,mgls_integrate,mglc_integrate}
int mgls_integrate(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2)	a[0].d->Integral(a[1].s);
	else	return 1;
	return 0;
}
void mglc_integrate(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2)	sprintf(out,"%s.Integral(\"%s\");", a[0].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"inplot","Set position of plot in picture",0,false,mgls_inplot,mglc_inplot}
int mgls_inplot(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3)	gr->InPlot(a[0].v,a[1].v,a[2].v,a[3].v);
	else	return 1;
	return 0;
}
void mglc_inplot(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3)
		sprintf(out,"gr->InPlot(%g, %g, %g, %g);", a[0].v, a[1].v, a[2].v, a[3].v);
}
//-----------------------------------------------------------------------------
//	{"pipe","Draw flow pipes for vector field",0,false,mgls_pipe,mglc_pipe}
int mgls_pipe(mglGraph *gr, long n, mglArg *a, int k[9])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=1)	break;
	if(i==2)
		gr->Pipe(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?a[3].v:0.05,k[4]==3?int(a[4].v):5,
			true,k[5]==3?a[5].v:NAN);
	else if(i==4)
		gr->Pipe(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,k[5]==3?a[5].v:0.05,
			k[6]==3?int(a[6].v):5,true,k[7]==3?a[7].v:NAN);
	else if(i==3)
		gr->Pipe(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0,k[4]==3?a[4].v:0.05,k[5]==3?int(a[5].v):3);
	else if(i==6)
		gr->Pipe(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),*(a[5].d),
			k[6]==2?a[6].s:0,k[7]==3?a[7].v:0.05,k[8]==3?int(a[8].v):3);
	else	return 1;
	return 0;
}
void mglc_pipe(char out[1024], long n, mglArg *a, int k[9])
{
	int i;
	for(i=0;i<7;i++)	if(k[i]!=1)	break;
	if(i==2)
		sprintf(out,"gr->Pipe(%s, %s, \"%s\", %g, %d, true, %g);",  a[0].s, a[1].s, k[2]==2?a[2].s:"", k[3]==3?a[3].v:0.05, k[4]==3?int(a[4].v):5, k[5]==3?a[5].v:NAN);
	else if(i==4)
		sprintf(out,"gr->Pipe(%s, %s, %s, %s, \"%s\", %g, %d, true, %g);",  a[0].s, a[1].s, a[2].s, a[3].s, k[4]==2?a[4].s:"", k[5]==3?a[5].v:0.05, k[6]==3?int(a[6].v):5, k[7]==3?a[7].v:NAN);
	else if(i==3)
		sprintf(out,"gr->Pipe(%s, %s, %s, \"%s\", %g, %d);",  a[0].s, a[1].s, a[2].s, k[3]==2?a[3].s:"", k[4]==3?a[4].v:0.05, k[5]==3?int(a[5].v):3);
	else if(i==6)
		sprintf(out,"gr->Pipe(%s, %s, %s, %s, %s, %s, \"%s\", %g, %d);",  a[0].s, a[1].s, a[2].s, a[3].s, a[4].s, a[5].s, k[6]==2?a[6].s:"", k[7]==3?a[7].v:0.05, k[8]==3?int(a[8].v):3);
}
//-----------------------------------------------------------------------------
//	{"origin","Set axis origin",0,false,mgls_origin,mglc_origin}
int mgls_origin(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3)	gr->Org = mglPoint(a[0].v,a[1].v,k[2]==3?a[2].v:gr->Min.z);
	else	return 1;
	return 0;
}
void mglc_origin(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3 && k[1]==3)
		sprintf(out,"gr->Org = mglPoint(%g, %g, %g);", a[0].v, a[1].v, k[2]==3?a[2].v:0);
}
//-----------------------------------------------------------------------------
//	{"plotfactor","Set plotfactor","plotfactor val",false,mgls_plotfactor,mglc_plotfactor}
int mgls_plotfactor(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3)
	{
		gr->PlotFactor = a[0].v<0 ? gr->PlotFactor:a[0].v;
		gr->AutoPlotFactor = (a[0].v<0);
	}
	else	return 1;
	return 0;
}
void mglc_plotfactor(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3)	sprintf(out,"gr->PlotFactor = %g<0 ? gr->PlotFactor:%g;\tgr->AutoPlotFactor = (%g<0);", a[0].v, a[0].v, a[0].v);
}
//-----------------------------------------------------------------------------
//	{"norm","Normalize data",0,false,mgls_norm,mglc_norm}
int mgls_norm(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==3 && k[2]==3)
		a[0].d->Norm(a[1].v,a[2].v,k[3]==3?a[3].v!=0:false,k[4]==3?int(a[4].v):0);
	else	return 1;
	return 0;
}
void mglc_norm(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==3 && k[2]==3)
		sprintf(out,"%s.Norm(%g, %g, %s, %d);", a[0].s, a[1].v, a[2].v, (k[3]==3&&a[3].v!=0)?"true":"false", k[4]==3?int(a[4].v):0);
}
//-----------------------------------------------------------------------------
//	{"hist","Create histogram (distribution) of data values",0,true,mgls_hist,mglc_hist}
int mgls_hist(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==3 && k[4]==3 && k[5]==3)
		*(a[0].d) = a[1].d->Hist(*(a[2].d),int(a[3].v),a[4].v,a[5].v);
	else if(k[0]==1 && k[1]==1 && k[2]==3 && k[3]==3 && k[4]==3)
		*(a[0].d) = a[1].d->Hist(int(a[2].v),a[3].v,a[4].v);
	else	return 1;
	return 0;
}
void mglc_hist(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==3 && k[4]==3 && k[5]==3)
		sprintf(out,"%s = %s.Hist(%s, %d, %g, %g);", a[0].s, a[1].s, a[2].s, int(a[3].v), a[4].v, a[5].v);
	else if(k[0]==1 && k[1]==1 && k[2]==3 && k[3]==3 && k[4]==3)
		sprintf(out,"%s = %s.Hist(%d, %g, %g);", a[0].s, a[1].s, int(a[2].v), a[3].v, a[4].v);
}
//-----------------------------------------------------------------------------
//	{"mirror","Mirror data at some direction",0,false,mgls_mirror,mglc_mirror}
int mgls_mirror(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2)	a[0].d->Mirror(a[1].s);
	else	return 1;
	return 0;
}
void mglc_mirror(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==2)	sprintf(out,"%s.Mirror(\"%s\");", a[0].s, a[1].s);
}
//-----------------------------------------------------------------------------
//	{"new","Create new data","new var nx [ny nz]",false,mgls_new,mglc_new}
int mgls_new(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==3)	a[0].d->Create(int(a[1].v), k[2]==3?int(a[2].v):1, k[3]==3?int(a[3].v):1);
	else	return 1;
	return 0;
}
void mglc_new(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==3)
		sprintf(out,"%s.Create(%d, %d, %d);", a[0].s, int(a[1].v), k[2]==3?int(a[2].v):1, k[3]==3?int(a[3].v):1);
}
//-----------------------------------------------------------------------------
//	{"var","Create new 1D data and fill it in range","var var nx x1 x2",false,mgls_var,mglc_var}
int mgls_var(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==3 && k[2]==3 && k[3]==3)
	{	a[0].d->Create(int(a[1].v));	a[0].d->Fill(a[2].v, a[3].v);	}
	else	return 1;
	return 0;
}
void mglc_var(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==1 && k[1]==3 && k[2]==3 && k[3]==3)
	sprintf(out,"%s.Create(%d);\t%s.Fill(%g,%g);",a[0].s, int(a[1].v), a[0].s, a[2].v, a[3].v);
}
//-----------------------------------------------------------------------------
//	{"chdir","Change current directory","chdir dir",false,mgls_chdir,mglc_chdir}
int mgls_chdir(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==2)	chdir(a[0].s);
	else	return 1;
	return 0;
}
void mglc_chdir(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==2)	sprintf(out,"chdir(\"%s\");", a[0].s);
}
//-----------------------------------------------------------------------------
//	{"perspective","Set perspective","perspective val",false,mgls_perspective,mglc_perspective}
int mgls_perspective(mglGraph *gr, long n, mglArg *a, int k[9])
{
	if(k[0]==3)	gr->Perspective(a[0].v);
	else	return 1;
	return 0;
}
void mglc_perspective(char out[1024], long n, mglArg *a, int k[9])
{
	if(k[0]==3)	sprintf(out,"gr->Perspective(%g);", a[0].v);
}
//-----------------------------------------------------------------------------
mglCommand mgls_base_cmd[] = {
	{"addlegend","Add legend entry","addlegend txt fmt", mgls_addlegend, mglc_addlegend},
	{"addto","Add data or number","addto var|num", mgls_addto, mglc_addto},
	{"alpha","Switch on/off transparency","addto [num]", mgls_alpha, mglc_alpha},
	{"alphadef","Set default transparency","alphadef num", mgls_alphadef, mglc_alphadef},
	{"ambient","Set ambient light brightness","ambient txt fmt", mgls_ambient, mglc_ambient},
	{"area","Draw area plot for 1D data","area {xvar} yvar {{zvar}} [fmt num]", mgls_area, mglc_area},
	{"aspect","Set aspect ration","aspect numx numy numz", mgls_aspect, mglc_aspect},
	{"axial","Draw surfaces of contour lines rotation","axial {xvar yvar} zvar [fmt num]", mgls_axial, mglc_axial},
	{"axialdir","Set axis of rotation","axialdir dir", mgls_axialdir, mglc_axialdir},
	{"axis","Setup or draw axis","", mgls_axis, mglc_axis},
	{"ball","Draw point (ball)","ball posx posy {posz} [fmt]", mgls_ball, mglc_ball},
	{"bars","Draw bars for 1D data","bars {xvar} yvar {{zvar}} [fmt num num]", mgls_bars, mglc_bars},
	{"beam","Draw quasioptical beam","", mgls_beam, mglc_beam},
	{"belt","Draw belts","belt {xvar yvar} zvar [fmt]", mgls_belt, mglc_belt},
	{"box","Draw bounding box","box [fmt]", mgls_box, mglc_box},
	{"boxs","Draw boxes","boxs {xvar yvar} zvar [fmt]", mgls_boxs, mglc_boxs},
	{"caxis","Set color range","caxis c1 c2", mgls_caxis, mglc_caxis},
	{"chart","Draw chart","chart var [fmt]", mgls_chart, mglc_chart},
	{"chdir","Change current directory","chdir dir", mgls_chdir, mglc_chdir},
	{"clearlegend","Clear legend antries","clearlegend", mgls_clearlegend, mglc_clearlegend},
	{"clf","Clear picture","clf", mgls_clf, mglc_clf},
	{"cloud","Draw cloud","cloud {xvar yvar zvar} avar [fmt]", mgls_cloud, mglc_cloud},
	{"colorbar","Draw colorbar","colorbar [fmt pos]", mgls_colorbar, mglc_colorbar},
	{"cone","Draw cone","cone avar [fmt pos num]", mgls_cone, mglc_cone},
	{"cont","Draw contour lines","cont {vvar} {xvar yvar} zvar [fmt num zpos]", mgls_cont, mglc_cont},
	{"cont3","Draw contour lines for 3D data","cont3 {xvar yvar zvar} avar dir [pos fmt num]", mgls_cont3, mglc_cont3},
	{"conta","Draw contour lines at central slices of 3D data","conta {xvar yvar zvar} avar [fmt num]", mgls_conta, mglc_conta},
	{"contf","Draw solid contour lines","contf {vvar} {xvar yvar} zvar [fmt num zpos]", mgls_contf, mglc_contf},
	{"contf3","Draw solid contour lines for 3D data","contf3 {xvar yvar zvar} avar dir [pos fmt num]", mgls_contf3, mglc_contf3},
	{"contfa","Draw solid contour lines at central slices of 3D data","contfa {xvar yvar zvar} avar [fmt num]", mgls_contfa, mglc_contfa},
	{"contfx","Draw solid contour lines at x-slice (or x-plane)","contfx avar [fmt pos num]", mgls_contfx, mglc_contfx},
	{"contfy","Draw solid contour lines at y-slice (or y-plane)","contfy avar [fmt pos num]", mgls_contfy, mglc_contfy},
	{"contfy","Draw solid contour lines at z-slice (or z-plane)","contfz avar [fmt pos num]", mgls_contfz, mglc_contfz},
	{"contx","Draw contour lines at x-slice (or x-plane)","contx avar [fmt pos num]", mgls_contx, mglc_contx},
	{"conty","Draw contour lines at y-slice (or y-plane)","conty avar [fmt pos num]", mgls_conty, mglc_conty},
	{"contz","Draw contour lines at z-slice (or z-plane)","contz avar [fmt pos num]", mgls_contz, mglc_contz},
	{"copy","Copy data from another variable","copy var1 var2", mgls_copy, mglc_copy},
	{"crange","Set color range","crange {var [sym] | c1 c2}", mgls_crange, mglc_crange},
	{"crop","Crop edge of data","crop var n1 n2 dir", mgls_crop, mglc_crop},
	{"crust","Draw reconstructed surface for arbitrary data points","crust {xvar yvar zvar} | var [fmt]", mgls_crust, mglc_crust},
	{"cumsum","Crop edge of data","cumsum var dir", mgls_cumsum, mglc_cumsum},
	{"curve","Draw curve","", mgls_curve, mglc_curve},
	{"cut","Setup plot points cutting","", mgls_cut, mglc_cut},
	{"dens","Draw density plot","dens {xvar yvar} zvar [fmt zpos]", mgls_dens, mglc_dens},
	{"dens3","Draw density plot at slices of 3D data","dens3 {xvar yvar zvar} avar dir [pos fmt]", mgls_dens3, mglc_dens3},
	{"densa","Draw density plot at central slices of 3D data","densa {xvar yvar zvar} avar [fmt num]", mgls_densa, mglc_densa},
	{"densx","Draw density plot at x-slice (or x-plane)","densx avar [fmt pos]", mgls_densx, mglc_densx},
	{"densy","Draw density plot at y-slice (or y-plane)","densy avar [fmt pos]", mgls_densy, mglc_densy},
	{"densz","Draw density plot at z-slice (or z-plane)","densz avar [fmt pos]", mgls_densz, mglc_densz},
	{"dew","Draw dew plot","dew {xvae yvar} uvar vvar [fmt]", mgls_dew, mglc_dew},
	{"diff","Numerically differentiate data","diff var dir", mgls_diff, mglc_diff},
	{"diff2","Numerically double differentiate data","diff2 var dir", mgls_diff2, mglc_diff2},
	{"divto","Divide by data or number","divto var|num", mgls_divto, mglc_divto},
	{"dots","Draw dots for arbitrary data points","dots {xvar yvar zvar} | var [fmt]", mgls_dots, mglc_dots},
	{"drop","Draw drop","", mgls_drop, mglc_drop},
	{"error","Draw error boxes","", mgls_error, mglc_error},
	{"extend","Extend data array","", mgls_extend, mglc_extend},
	{"fall","Draw waterfalls","fall {xvar yvar} zvar [fmt]", mgls_fall, mglc_fall},
	{"fill","Fill data linearly in range [v1, v2]","fill var v1 v2 [dir]", mgls_fill, mglc_fill},
	{"flow","Draw flow threads for vector field","", mgls_flow, mglc_flow},
	{"fog","Switch on/off fog","fog var val [pos]", mgls_fog, mglc_fog},
	{"font","Setup font","font fmt [size]", mgls_font, mglc_font},
	{"grid","Draw grid","", mgls_grid, mglc_grid},
	{"grid3","Draw grid at slices of 3D data","grid3 {xvar yvar zvar} avar dir [pos fmt]", mgls_grid3, mglc_grid3},
	{"grida","Draw grid at central slices of 3D data","grida {xvar yvar zvar} avar [fmt num]", mgls_grida, mglc_grida},
	{"hist","Create histogram (distribution) of data values","", mgls_hist, mglc_hist},
	{"info","Print information about data","info var", mgls_info, mglc_info},
	{"inplot","Set position of plot in picture","", mgls_inplot, mglc_inplot},
	{"integrate","Integrate data","integrate var dir", mgls_integrate, mglc_integrate},
	{"legend","Draw legend","legend {xpos ypos} | pos [fmt size]", mgls_legend, mglc_legend},
	{"legendbox","Switch on/off drawing box in legend","legendbox val", mgls_legendbox, mglc_legendbox},
	{"light","Setup light","", mgls_light, mglc_light},
	{"line","Draw line","", mgls_line, mglc_line},
	{"linewidth","Set default line width","linewidth val", mgls_linewidth, mglc_linewidth},
	{"map","Draw mapping plot","map {xvar yvar} uvar vvar [fmt]", mgls_map, mglc_map},
	{"mark","Draw mark plot for 1D data","mark {xvar} yvar {{zvar}} rvar [fmt num]", mgls_mark, mglc_mark},
	{"marksize","Set size of markers","marksize val", mgls_marksize, mglc_marksize},
	{"max","Find maximal value over direction","max ovar ivar dir", mgls_max, mglc_max},
	{"mesh","Draw mesh surface","mesh {xvar yvar} zvar [fmt]", mgls_mesh, mglc_mesh},
	{"meshnum","Set number of lines in mesh/fall/vect and so on","meshnum val", mgls_meshnum, mglc_meshnum},
	{"min","Find minimal value over direction","min ovar ivar dir", mgls_min, mglc_min},
	{"mirror","Mirror data at some direction","", mgls_mirror, mglc_mirror},
	{"modify","Modify data values by formula","modify var formula [var1 var2]", mgls_modify, mglc_modify},
	{"multo","Multiply by data or number","multo var|num", mgls_multo, mglc_multo},
	{"new","Create new data","new var nx [ny nz]", mgls_new, mglc_new},
	{"norm","Normalize data","", mgls_norm, mglc_norm},
	{"origin","Set axis origin","", mgls_origin, mglc_origin},
	{"perspective","Set perspective","perspective val", mgls_perspective, mglc_perspective},
	{"pipe","Draw flow pipes for vector field","", mgls_pipe, mglc_pipe},
	{"plot","Draw usual plot for 1D data","plot {xvar} yvar {{zvar}} [fmt num]", mgls_plot, mglc_plot},
	{"plotfactor","Set plotfactor","plotfactor val", mgls_plotfactor, mglc_plotfactor},
	{"read","Read data from file","read var file [nx ny nz]", mgls_read, mglc_read},
	{"readall","Read and join data from several files","", mgls_readall, mglc_readall},
	{"readhdf","Read data from HDF5 file","readhdf var file id", mgls_readhdf, mglc_readhdf},
	{"readmat","Read data from file with sizes specified in first row","readmat var file [dim=2]", mgls_readmat, mglc_readmat},
	{"rect","Draw rectangle","", mgls_rect, mglc_rect},
	{"resize","Resize data","resize ovar ivar [nx ny nz]", mgls_resize, mglc_resize},
	{"rotate","Rotate plot","", mgls_rotate, mglc_rotate},
	{"rotatetext","Set to auto rotate text or not","", mgls_rotatetext, mglc_rotatetext},
	{"save","Save data to file","save var file", mgls_save, mglc_save},
	{"savehdf","Save data to HDF5 file","savehdf var file id", mgls_savehdf, mglc_savehdf},
	{"set_id","Set column id for data","set_id var id", mgls_set_id, mglc_set_id},
	{"setsize","Set picture size","setsize wval hval", mgls_setsize, mglc_setsize},
	{"smooth","Smooth data","smooth var kind dir", mgls_smooth, mglc_smooth},
	{"sphere","Draw sphere","", mgls_sphere, mglc_sphere},
	{"squeeze","Squeeze data","squeeze var kx [ky kz]", mgls_squeeze, mglc_squeeze},
	{"stem","Draw stem plot for 1D data","stem {xvar} yvar {{zvar}} [fmt num]", mgls_stem, mglc_stem},
	{"step","Draw step plot for 1D data","step {xvar} yvar {{zvar}} [fmt num]", mgls_step, mglc_step},
	{"stfa","Draw STFA diagram","stfa {xvar yvar} uvar vvar num [fmt]", mgls_stfa, mglc_stfa},
	{"stfad","Do STFA transform","", mgls_stfad, mglc_stfad},
	{"subdata","Extract sub-array","subdata ovar ivar nx [ny nz]", mgls_subdata, mglc_subdata},
	{"subplot","Set position of plot","subplot mval nval ival", mgls_subplot, mglc_subplot},
	{"subto","Subtract data or number","subto var|num", mgls_subto, mglc_subto},
	{"sum","Find summation over direction","sum ovar ivar dir", mgls_sum, mglc_sum},
	{"surf","Draw solid surface","surf {xvar yvar} zvar [fmt]", mgls_surf, mglc_surf},
	{"surf3","Draw isosurface for 3D data","surf3 {xvar yvar zvar} avar {num} [fmt]", mgls_surf3, mglc_surf3},
	{"surf3a","Draw isosurface for 3D data transpared by other data","surf3a {xvar yvar zvar} avar cvar {num} [fmt]", mgls_surf3a, mglc_surf3a},
	{"surf3c","Draw isosurface for 3D data colored by other data","surf3c {xvar yvar zvar} avar cvar {num} [fmt]", mgls_surf3c, mglc_surf3c},
	{"surfa","Draw solid surface transpared by other data","surfa {xvar yvar} zvar cvar [fmt]", mgls_surfa, mglc_surfa},
	{"surfc","Draw solid surface colored by other data","surfc {xvar yvar} zvar cvar [fmt]", mgls_surfc, mglc_surfc},
	{"swap","Swap data (usefull after Fourier transform)","swap var dir", mgls_swap, mglc_swap},
	{"ternary","Switch on/off to use ternary axis","ternary val", mgls_ternary, mglc_ternary},
	{"text","Draw text at some position or along curve","", mgls_text, mglc_text},
	{"textmark","Draw TeX mark at point position","", mgls_textmark, mglc_textmark},
	{"tile","Draw horizontal tiles","tile {xvar yvar} zvar {rvar} [fmt]", mgls_tile, mglc_tile},
	{"torus","Draw surface of curve rotation","torus {zvar} rvar [fmt]", mgls_torus, mglc_torus},
	{"transform","Do integral transform of data","transform ovar how rvar ivar", mgls_transform, mglc_transform},
	{"transforma","Do integral transform of data","transforma ovar how avar fvar", mgls_transforma, mglc_transforma},
	{"transparent","Switch off transparency","transparent val", mgls_transparent, mglc_transparent}, //!!! OLD !!!
	{"transpose","Transpose data array","transpose var [dir]", mgls_transpose, mglc_transpose},
	{"transptype","Set type transparency","transptype val", mgls_transptype, mglc_transptype},
	{"triplot","Draw surface of triangles","", mgls_triplot, mglc_triplot},
	{"tube","Draw curve by tube","", mgls_tube, mglc_tube},
	{"var","Create new 1D data and fill it in range","var var nx x1 x2", mgls_var, mglc_var},
	{"vect","Draw vector field","", mgls_vect, mglc_vect},
	{"vectc","Draw vector field with coloring","", mgls_vectc, mglc_vectc},
	{"xlabel","Draw label for x-axis","xlabel txt [fnt pos]", mgls_xlabel, mglc_xlabel},
	{"xrange","Set range for x-axis","xrange {var [add]} | {x1 x2}", mgls_xrange, mglc_xrange},
	{"xtick","Set ticks for x-axis","xtick {val [sub]} | tmpl", mgls_xtick, mglc_xtick},
	{"ylabel","Draw label for y-axis","ylabel txt [fnt pos]", mgls_ylabel, mglc_ylabel},
	{"yrange","Set range for y-axis","yrange {var [add]} | {x1 x2}", mgls_yrange, mglc_yrange},
	{"ytick","Set ticks for y-axis","ytick {val [sub]} | tmpl", mgls_ytick, mglc_ytick},
	{"zlabel","Draw label for z-axis","zlabel txt [fnt pos]", mgls_zlabel, mglc_zlabel},
	{"zoom","Zoom in/out plot","zoom x1 y1 x2 y2", mgls_zoom, mglc_zoom},
	{"zrange","Set range for z-axis","yrange {var [add]} | {x1 x2}", mgls_zrange, mglc_zrange},
	{"ztick","Set ticks for z-axis","ztick {val [sub]} | tmpl", mgls_ztick, mglc_ztick},
{"",0,0,0,0}};
//-----------------------------------------------------------------------------
