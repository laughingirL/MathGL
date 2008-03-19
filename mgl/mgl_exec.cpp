/* mgl_exec.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>
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
#include "mgl/mgl.h"
//-----------------------------------------------------------------------------
wchar_t *mgl_str_copy(const char *s)
{
	wchar_t *str = new wchar_t[strlen(s)+1];
	register long i;
	for(i=0;i<strlen(s);i++)	str[i] = s[i];
	str[i] = 0;
	return str;
}
//-----------------------------------------------------------------------------
// return values : 0 -- OK, 1 -- wrong arguments, 2 -- wrong command
int mglGraph::Exec(const char *com, long n, mglArg *a)
{
	char ch = com[0];
	int k[9], i;
	for(i=0;i<9;i++)	k[i] = i<n ? a[i].type + 1 : 0;
	if(ch == 'a')		return exec_a(com,n,a,k);
	else if(ch == 'b')	return exec_b(com,n,a,k);
	else if(ch == 'c')	return exec_c(com,n,a,k);
	else if(ch == 'd')	return exec_d(com,n,a,k);
	else if(ch == 'f')	return exec_f(com,n,a,k);
	else if(ch == 'g')	return exec_g(com,n,a,k);
	else if(ch == 'l')	return exec_l(com,n,a,k);
	else if(ch == 'm')	return exec_m(com,n,a,k);
	else if(ch == 'r')	return exec_r(com,n,a,k);
	else if(ch == 's')	return exec_s(com,n,a,k);
	else if(ch == 't')	return exec_t(com,n,a,k);
	else if(ch >= 'v')	return exec_vz(com,n,a,k);
	else	return exec_misc(com,n,a,k);
}
//-----------------------------------------------------------------------------
int mglGraph::exec_a(const char *com, long n, mglArg *a,int k[9])
{
	int res = 0;
	long i;
	if(!strcmp(com,"addlegend"))
	{	if(k[0]==2 && k[1]==2)	AddLegend(a[0].s,a[1].s);	else	res = 1;}
	else if(!strcmp(com,"addto"))
	{
		if(k[0]==1 && k[1]==1)		*(a[0].d) += *(a[1].d);
		else if(k[0]==1 && k[1]==3)	*(a[0].d) += a[1].v;
		else	res = 1;
	}
	else if(!strcmp(com,"alpha"))	Alpha(k[0]==3 ? a[0].v!=0 : true);
	else if(!strcmp(com,"alphadef"))
	{	if(k[0]==3)	AlphaDef = a[0].v;	else	res = 1;	}
	else if(!strcmp(com,"ambient"))
	{	if(k[0]==3)	Ambient(a[0].v);		else	res = 1;	}
	else if(!strcmp(com,"area"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==2)	Area(*(a[0].d),a[1].s,false,k[2]==3 ? a[2].v : NAN);
		else if(k[1]==1)
		{
			if(k[2]==2)
				Area(*(a[0].d),*(a[1].d),a[2].s,false,k[3]==3 ? a[3].v : NAN);
			else if(k[2]==0)	Area(*(a[0].d),*(a[1].d));
			else if(k[2]==1)	Area(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
		}
		else	Area(*(a[0].d));
	}
	else if(!strcmp(com,"aspect"))
	{
		if(k[0]==3 && k[1]==3 && k[2]==3)	Aspect(a[0].v,a[1].v,a[2].v);
		else	res = 1;
	}
	else if(!strcmp(com,"axial"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==0)	Axial(*(a[0].d));
		else if(k[1]==2)	Axial(*(a[0].d),a[1].s,k[2]==3 ? int(a[2].v) : 3);
		else if(k[1]==1 && k[2]==1)
		{
			if(k[3]==2)
				Axial(*(a[0].d),*(a[1].d),*(a[2].d),a[3].s,k[4]==3 ? int(a[4].v) : 3);
			else	Axial(*(a[0].d),*(a[1].d),*(a[2].d));
		}
		else	res = 1;
	}
	else if(!strcmp(com,"axialdir"))
	{	if(k[0]==2)	AxialDir = a[0].s[0];		else	res = 1;	}
	else if(!strcmp(com,"axis"))
	{
		if(k[0]==2 && k[1]==2 && k[2]==2)	Axis(a[0].s,a[1].s,a[2].s);
		else if(n==6)
		{
			bool ok=true;
			for(i=0;i<6;i++)	if(k[i]!=3)	ok = false;
			if(ok)	Axis(mglPoint(a[0].v,a[1].v,a[2].v), mglPoint(a[3].v,a[4].v,a[5].v),Org);
		}
		else if(n==4)
		{
			bool ok=true;
			for(i=0;i<4;i++)	if(k[i]!=3)	ok = false;
			if(ok)	Axis(mglPoint(a[0].v,a[1].v), mglPoint(a[2].v,a[3].v),Org);
		}
		else if(k[0]==2)	Axis(a[0].s);
		else if(k[0]==0)	Axis("xyz");
		else	res = 1;
	}
	else res = 2;
	return res;
}
//-----------------------------------------------------------------------------
int mglGraph::exec_b(const char *com, long , mglArg *a,int k[9])
{
	int res = 0;
	if(!strcmp(com,"ball"))
	{
		if(k[0]==3 && k[1]==3 && k[2]==3)
			Ball(mglPoint(a[0].v,a[1].v,a[2].v),k[3]==2 ? a[3].s[0]:'r');
		else	res = 1;
	}
	else if(!strcmp(com,"box"))		Box(k[0]==2 ? a[0].s : (TranspType!=2 ?"k-":"w-"));
	else if(!strcmp(com,"bars"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==2)	Bars(*(a[0].d),a[1].s,k[2]==3 ? a[2].v : NAN);
		else if(k[1]==1)
		{
			if(k[2]==2)
				Bars(*(a[0].d),*(a[1].d),a[2].s,k[3]==3 ? a[3].v : NAN);
			else if(k[2]==0)	Bars(*(a[0].d),*(a[1].d));
			else if(k[2]==1)	Bars(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
		}
		else	Bars(*(a[0].d));
	}
	else if(!strcmp(com,"belt"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==0)	Belt(*(a[0].d));
		else if(k[1]==2)	Belt(*(a[0].d),a[1].s);
		else if(k[1]==1 && k[2]==1)
			Belt(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2? a[3].s:0);
		else	res = 1;
	}
	else if(!strcmp(com,"boxs"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==0)	Boxs(*(a[0].d));
		else if(k[1]==2)	Boxs(*(a[0].d),a[1].s);
		else if(k[1]==1 && k[2]==1)
			Boxs(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2? a[3].s:0);
		else	res = 1;
	}
	else if(!strcmp(com,"beam"))
	{
		if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==3)
			Beam(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),a[4].v,
					k[5]==2? a[5].s:0,k[6]==3?int(a[6].v):0,
					int(k[7]==3?a[7].v:3));
		else	res = 1;
	}
	else res = 2;
	return res;
}
//-----------------------------------------------------------------------------
int mglGraph::exec_c(const char *com, long n, mglArg *a,int k[9])
{
	int res = 0;
	long i;
	if(com[1]=='o')	return exec_co(com,n,a,k);
	else if(!strcmp(com,"caxis"))
	{	if(k[0]==3 && k[1]==3)	CAxis(a[0].v,a[1].v);	else	res = 1;}
	else if(!strcmp(com,"chart"))
	{	if(k[0]==1)	Chart(*(a[0].d), k[1]==2?a[1].s:0);	else	res = 1;	}
	else if(!strcmp(com,"clearlegend"))	ClearLegend();
	else if(!strcmp(com,"clf"))			Clf();
	else if(!strcmp(com,"cloud"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]!=1)	CloudQ(*(a[0].d),k[1]==2?a[1].s:0);
		else if(k[1]==1 && k[2]==1 && k[3]==1)
			CloudQ(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
	}
	else if(!strcmp(com,"crange"))
	{
		if(k[0]==1)	CRange(*(a[0].d),k[1]==3?(a[1].v!=0):false);
		else if(k[0]==3 && k[1]==3)	{	Cmin = a[0].v;	Cmax = a[1].v;	}
		else	res = 1;
	}
	else if(!strcmp(com,"crop"))
	{
		if(k[0]==1 && k[1]==3 && k[2]==3 && k[3]==2)
			a[0].d->Crop(int(a[1].v),int(a[2].v),a[3].s[0]);
		else	res = 1;}
	else if(!strcmp(com,"cumsum"))
	{	if(k[0]==1 && k[1]==2)	a[0].d->CumSum(a[1].s);	else	res = 1;}
	else if(!strcmp(com,"curve"))
	{
		if(n<12)	res = 1;
		else
		{
			bool ok=true;
			for(i=0;i<12;i++)	if(a[i].type!=2)	ok=false;
			if(ok)
				Curve(mglPoint(a[0].v,a[1].v,a[2].v),
					mglPoint(a[3].v,a[4].v,a[5].v),
					mglPoint(a[6].v,a[7].v,a[8].v),
					mglPoint(a[9].v,a[10].v,a[11].v),
					(n==13 && a[12].type==1) ? a[12].s : 0);
			else	res = 1;
		}
	}
	else if(!strcmp(com,"cut"))
	{
		if(k[0]==3 && n == 1)	Cut = (a[0].v != 0);
		else if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3 && k[4]==3 && k[5]==3)
		{
			CutMin = mglPoint(a[0].v,a[1].v,a[2].v);
			CutMax = mglPoint(a[3].v,a[4].v,a[5].v);
		}
		else if(k[0]==2 && n == 1)	CutOff(a[0].s);
		else	res = 1;
	}
	else if(!strcmp(com,"crust"))
	{
		if(k[0]==1 && k[1]==1 && k[2]==1)
			Crust(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
		else if(k[0]==1)
			Crust(*(a[0].d),k[1]==2?a[1].s:0);
		else	res = 1;
	}
	else res = 2;
	return res;
}
//-----------------------------------------------------------------------------
int mglGraph::exec_co(const char *com, long , mglArg *a,int k[9])
{
	int res = 0;
	if(!strcmp(com,"colorbar"))
		Colorbar(k[0]==2 ? a[0].s:0, k[1]==3 ? int(a[1].v):0);
	else if(!strcmp(com,"copy"))
	{
		if(k[0]==1 && k[1]==1)	a[0].d->Set(*(a[1].d));
		else	res = 1;
	}
	else if(!strcmp(com,"cont"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]!=1)	Cont(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?int(a[2].v):7,k[3]==3?a[3].v:NAN);
		else if(k[1]==1 && k[2]!=1)
			Cont(*(a[0].d), *(a[1].d), k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
		else if(k[1]==1 && k[2]==1 && k[3]!=1)
				Cont(*(a[0].d), *(a[1].d), *(a[2].d), k[3]==2?a[3].s:0,
					k[4]==3?int(a[4].v):7, k[5]==3?a[5].v:NAN);
		else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]!=1)
				Cont(*(a[0].d), *(a[1].d), *(a[2].d), *(a[3].d), k[4]==2?a[4].s:0,
					k[5]==3?a[5].v:NAN);
		else	res = 1;
	}
	else if(!strcmp(com,"cont3"))	//D S v_slice sch v_num
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==2)
			Cont3(*(a[0].d),a[1].s[0],k[2]==3?int(a[2].v):-1,
					k[3]==2?a[3].s:0, k[4]==3?int(a[4].v):7);
		else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
			Cont3(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),a[4].s[0],
					k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0, k[7]==3?int(a[7].v):7);
		else	res = 1;
	}
	else if(!strcmp(com,"conta"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==0)	ContA(*(a[0].d));
		else if(k[1]==2)	ContA(*(a[0].d),a[1].s,k[2]==3?int(a[2].v):7);
		else if(k[1]==1 && k[2]==1 && k[3]==1)
			ContA(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,
					k[5]==3?int(a[5].v):7);
		else	res = 1;
	}
	else if(!strcmp(com,"contf"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]!=1)	ContF(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?int(a[2].v):7,k[3]==3?a[3].v:NAN);
		else if(k[1]==1 && k[2]!=1)
			ContF(*(a[0].d), *(a[1].d), k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
		else if(k[1]==1 && k[2]==1 && k[3]!=1)
				ContF(*(a[0].d), *(a[1].d), *(a[2].d), k[3]==2?a[3].s:0,
					k[4]==3?int(a[4].v):7, k[5]==3?a[5].v:NAN);
		else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]!=1)
				ContF(*(a[0].d), *(a[1].d), *(a[2].d), *(a[3].d), k[4]==2?a[4].s:0,
					k[5]==3?a[5].v:NAN);
		else	res = 1;
	}
	else if(!strcmp(com,"contf3"))	//D S v_slice sch v_num
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==2)
			ContF3(*(a[0].d),a[1].s[0],k[2]==3?int(a[2].v):-1,
					k[3]==2?a[3].s:0, k[4]==3?int(a[4].v):7);
		else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
			ContF3(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),a[4].s[0],
					k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0, k[7]==3?int(a[7].v):7);
		else	res = 1;
	}
	else if(!strcmp(com,"contfa"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==0)	ContFA(*(a[0].d));
		else if(k[1]==2)	ContFA(*(a[0].d),a[1].s,k[2]==3?int(a[2].v):7);
		else if(k[1]==1 && k[2]==1 && k[3]==1)
			ContFA(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,
					k[5]==3?int(a[5].v):7);
		else	res = 1;
	}
	else if(!strcmp(com,"contx"))
	{
		if(k[0]==1)
			ContX(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN,k[3]==3?int(a[3].v):7);
		else res = 1;
	}
	else if(!strcmp(com,"conty"))
	{
		if(k[0]==1)
			ContY(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN,k[3]==3?int(a[3].v):7);
		else res = 1;
	}
	else if(!strcmp(com,"contz"))
	{
		if(k[0]==1)
			ContZ(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN,k[3]==3?int(a[3].v):7);
		else res = 1;
	}
	else if(!strcmp(com,"contfx"))
	{
		if(k[0]==1)
			ContFX(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN,k[3]==3?int(a[3].v):7);
		else res = 1;
	}
	else if(!strcmp(com,"contfy"))
	{
		if(k[0]==1)
			ContFY(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN,k[3]==3?int(a[3].v):7);
		else res = 1;
	}
	else if(!strcmp(com,"contfz"))
	{
		if(k[0]==1)
			ContFZ(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN,k[3]==3?int(a[3].v):7);
		else res = 1;
	}
	else if(!strcmp(com,"cone"))
	{
		bool ok=true;
		for(int i=0;i<7;i++)	if(a[i].type!=2)	ok=false;
		if(ok)
		{
			if(a[7].type==2)
				Cone(mglPoint(a[0].v,a[1].v,a[2].v), mglPoint(a[3].v,a[4].v,a[5].v),
					a[6].v, a[7].v, (a[8].type==1) ? a[8].s : 0, (a[9].type==2) ? a[9].v!=0 : false);
			else
				Cone(mglPoint(a[0].v,a[1].v,a[2].v), mglPoint(a[3].v,a[4].v,a[5].v),
					a[6].v, -1, (a[7].type==1) ? a[7].s : 0, (a[8].type==2) ? a[8].v!=0 : false);
		}
		else	res = 1;
	}
	else res = 2;
	return res;
}
//-----------------------------------------------------------------------------
int mglGraph::exec_d(const char *com, long n, mglArg *a,int k[9])
{
	int res = 0,i;
	if(!strcmp(com,"dens"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==0)	Dens(*(a[0].d));
		else if(k[1]==2)	Dens(*(a[0].d),a[1].s,k[2]==3?a[2].v:NAN);
		else if(k[1]==1 && k[2]==1)
			Dens(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0, k[4]==3?a[4].v:NAN);
		else	res = 1;
	}
	else if(!strcmp(com,"dens3"))	//D S v_slice sch v_num
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==2)
			Dens3(*(a[0].d),a[1].s[0],k[2]==3?int(a[2].v):-1,k[3]==2?a[3].s:0);
		else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
			Dens3(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),a[4].s[0],
					k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0);
		else	res = 1;
	}
	else if(!strcmp(com,"densa"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==0)	DensA(*(a[0].d));
		else if(k[1]==2)	DensA(*(a[0].d),a[1].s);
		else if(k[1]==1 && k[2]==1 && k[3]==1)
			DensA(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
		else	res = 1;
	}
	else if(!strcmp(com,"densx"))
	{
		if(k[0]==1)
			DensX(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN);
		else res = 1;
	}
	else if(!strcmp(com,"densy"))
	{
		if(k[0]==1)
			DensY(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN);
		else res = 1;
	}
	else if(!strcmp(com,"densz"))
	{
		if(k[0]==1)
			DensZ(*(a[0].d),k[1]==2?a[1].s:0,k[2]==3?a[2].v:NAN);
		else res = 1;
	}
	else if(!strcmp(com,"diff"))
	{	if(k[0]==1 && k[1]==2)	a[0].d->Diff(a[1].s);	else	res = 1;}
	else if(!strcmp(com,"diff2"))
	{	if(k[0]==1 && k[1]==2)	a[0].d->Diff2(a[1].s);	else	res = 1;}
	else if(!strcmp(com,"divto"))
	{
		if(k[0]==1 && k[1]==1)		*(a[0].d) /= *(a[1].d);
		else if(k[0]==1 && k[1]==3)	*(a[0].d) /= a[1].v;
		else	res = 1;
	}
	else if(!strcmp(com,"dots"))
	{
		if(k[0]==1 && k[1]==1 && k[2]==1)
			Dots(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
		else if(k[0]==1)
			Dots(*(a[0].d),k[1]==2?a[1].s:0);
		else	res = 1;
	}
	else if(!strcmp(com,"drop"))
	{
		for(i=0;i<7;i++)	if(k[i]!=3)	break;
		if(i==7)
			Drop(mglPoint(a[0].v,a[1].v,a[2].v), mglPoint(a[3].v,a[4].v,a[5].v), a[6].v,
				 	k[7]==2 ? a[7].s:"b", k[8]==3 ? a[8].v:1.,  (n>9 && a[9].type==2) ? a[9].v:1.);
		else if(i==5)
			Drop(mglPoint(a[0].v,a[1].v), mglPoint(a[2].v,a[3].v), a[4].v,
				 	k[5]==2 ? a[5].s:"b", k[6]==3 ? a[6].v:1., k[7]==3 ? a[7].v:1.);
		else	res = 1;
	}
	else if(!strcmp(com,"dew"))
	{
		for(i=0;i<7;i++)	if(k[i]!=1)	break;
		if(i==2)		Dew(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
		else if(i==4)
			Dew(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,k[5]==3?a[5].v:NAN);
		else	res = 1;
	}
	else res = 2;
	return res;
}
//-----------------------------------------------------------------------------
int mglGraph::exec_f(const char *com, long , mglArg *a,int k[9])
{
	int res = 0, i;
	if(!strcmp(com,"fall"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==0)	Fall(*(a[0].d));
		else if(k[1]==2)	Fall(*(a[0].d),a[1].s);
		else if(k[1]==1 && k[2]==1)
			Fall(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
		else	res = 1;
	}
	else if(!strcmp(com,"flow"))	//D S v_slice sch v_num
	{
		for(i=0;i<7;i++)	if(k[i]!=1)	break;
		if(i==2)
			Flow(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?int(a[3].v):5,
				true,k[4]==3?a[4].v:NAN);
		else if(i==4)
			Flow(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,
				k[5]==3?int(a[5].v):5,true,k[6]==3?a[6].v:NAN);
		else if(i==3)
			Flow(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0,k[4]==3?int(a[4].v):3);
		else if(i==6)
			Flow(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),*(a[5].d),
				k[6]==2?a[6].s:0,k[7]==3?int(a[7].v):3);
		else	res = 1;
	}
	else if(!strcmp(com,"fill"))
	{	if(k[0]==1 && k[1]==3 && k[2]==3)	a[0].d->Fill(a[1].v,a[2].v,k[3]==2?a[3].s[0]:'x');	else	res = 1;}
	else if(!strcmp(com,"fog"))
	{	if(k[0]==3)	Fog(a[0].v,k[1]==3?a[1].v:0.25);	else	res = 1;}
	else if(!strcmp(com,"font"))
	{
		if(k[0]==2)
		{
			strcpy(FontDef,a[0].s);
			if(k[1]==3)	FontSize = a[1].v;
		}
		else	res = 1;
	}
	else res = 2;
	return res;
}
//-----------------------------------------------------------------------------
int mglGraph::exec_g(const char *com, long , mglArg *a,int k[9])
{
	int res = 0;
	if(!strcmp(com,"grid"))
	{
		if(k[0]!=1)			Grid(k[0]==2?a[0].s:"xyz", k[1]==2?a[1].s:"B-");
		else if(k[1]==0)	Grid(*(a[0].d));
		else if(k[1]==2)	Grid(*(a[0].d),a[1].s,k[2]==3?a[2].v:NAN);
		else if(k[1]==1 && k[2]==1)
			Grid(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0, k[4]==3?a[4].v:NAN);
		else	res = 1;
	}
	else if(!strcmp(com,"grid3"))	//D S v_slice sch v_num
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==2)
			Grid3(*(a[0].d),a[1].s[0],k[2]==3?int(a[2].v):-1,k[3]==2?a[3].s:0);
		else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==2)
			Grid3(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),a[4].s[0],
					k[5]==3?int(a[5].v):-1, k[6]==2?a[6].s:0);
		else	res = 1;
	}
	else if(!strcmp(com,"grida"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==0)	GridA(*(a[0].d));
		else if(k[1]==2)	GridA(*(a[0].d),a[1].s);
		else if(k[1]==1 && k[2]==1 && k[3]==1)
			GridA(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
		else	res = 1;
	}
	else res = 2;
	return res;
}
//-----------------------------------------------------------------------------
int mglGraph::exec_l(const char *com, long n, mglArg *a,int k[9])
{
	int res = 0;
	long i;
	if(!strcmp(com,"light"))
	{
		if(k[0]==0)			Light(true);
		else if(k[0]!=3)	res = 1;
		else if(k[1]!=3)	Light(a[0].v!=0);
		else if(k[2]==3 && k[3]==3)
			Light(int(a[0].v),mglPoint(a[1].v,a[2].v,a[3].v),
				k[4]==2?a[4].s[0]:'w',k[5]==3?a[5].v:0.5,true);
		else	Light(int(a[0].v),a[1].v!=0);
	}
	else if(!strcmp(com,"legend"))
		Legend(k[0]==3?int(a[0].v):3, k[1]==2?a[1].s:"rL", k[2]==3?a[2].v:-1);
	else if(!strcmp(com,"line"))
	{
		if(n>5)
		{
			bool ok=true;
			for(i=0;i<6;i++)	if(a[i].type!=2)	ok=false;
			if(ok)
				Line(mglPoint(a[0].v,a[1].v,a[2].v),
					mglPoint(a[3].v,a[4].v,a[5].v),
					(n==7 && a[6].type==1) ? a[6].s : 0, 100);
			else	res = 1;
		}
		else if(n>3)
		{
			bool ok=true;
			for(i=0;i<4;i++)	if(a[i].type!=2)	ok=false;
			if(ok)
				Line(mglPoint(a[0].v,a[1].v,Max.z), mglPoint(a[2].v,a[3].v,Max.z),
					(n==5 && a[4].type==1) ? a[4].s : 0, 100);
			else	res = 1;
		}
		else	res = 1;
	}
	else if(!strcmp(com,"linewidth"))
	{	if(k[0]==3 && a[0].v>=1)	BaseLineWidth = a[0].v;		else	res = 1;	}
	else if(!strcmp(com,"legendbox"))
	{	if(k[0]==3)	LegendBox = (a[0].v!=0);	else	res = 1;	}
	else res = 2;
	return res;
}
//-----------------------------------------------------------------------------
int mglGraph::exec_m(const char *com, long , mglArg *a,int k[9])
{
	int res = 0;
	if(!strcmp(com,"mesh"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==0)	Mesh(*(a[0].d));
		else if(k[1]==2)	Mesh(*(a[0].d),a[1].s);
		else if(k[1]==1 && k[2]==1)
			Mesh(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
		else	res = 1;
	}
	else if(!strcmp(com,"modify"))
	{
		if(k[0]==1 && k[1]==2 && k[2]==1 && k[3]==1)
			a[0].d->Modify(a[1].s,*(a[2].d),*(a[3].d));
		else if(k[0]==1 && k[1]==2 && k[2]==1)
			a[0].d->Modify(a[1].s,*(a[2].d));
		else if(k[0]==1 && k[1]==2)
			a[0].d->Modify(a[1].s, k[2]==3 ? int(a[2].v):0);
		else res = 1;
	}
	else if(!strcmp(com,"max"))	//D S v_slice sch v_num
	{
		if(k[0]==1 && k[1]==1 && k[2]==2)
			*(a[0].d) = a[1].d->Max(a[2].s);
		else	res = 1;
	}
	else if(!strcmp(com,"min"))	//D S v_slice sch v_num
	{
		if(k[0]==1 && k[1]==1 && k[2]==2)
			*(a[0].d) = a[1].d->Min(a[2].s);
		else	res = 1;
	}
	else if(!strcmp(com,"meshnum"))
	{	if(k[0]==3)	MeshNum = int(a[0].v);	else res = 1;	}
	else if(!strcmp(com,"marksize"))
	{	if(k[0]==3)	MarkSize = a[0].v/50;	else res = 1;	}
	else if(!strcmp(com,"mark"))
	{
		if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
			Mark(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
		else if(k[0]==1 && k[1]==1 && k[2]==1)
			Mark(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0,k[4]==3?a[4].v:NAN);
		else if(k[0]==1 && k[1]==1)
			Mark(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
		else	res = 1;
	}
	else if(!strcmp(com,"multo"))
	{
		if(k[0]==1 && k[1]==1)		*(a[0].d) *= *(a[1].d);
		else if(k[0]==1 && k[1]==3)	*(a[0].d) *= a[1].v;
		else	res = 1;
	}
	else if(!strcmp(com,"map"))
	{
		if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
			Map(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0, 0,k[5]==3?(a[5].v!=0):true);
		else if(k[0]==1 && k[1]==1)
			Map(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,0,k[3]==3?(a[3].v!=0):true);
		else	res = 1;
	}
	else res = 2;
	return res;
}
//-----------------------------------------------------------------------------
int mglGraph::exec_r(const char *com, long n, mglArg *a,int k[9])
{
	int res = 0;
	if(!strcmp(com,"read"))
	{
		bool rr=true;
		if(k[0]!=1 || k[1]!=2)	res = 1;
		else if(n==2)	rr=a[0].d->Read(a[1].s);
		else rr=a[0].d->Read(a[1].s, k[2]==3?int(a[2].v):1,
				k[3]==3?int(a[3].v):1, k[4]==3?int(a[4].v):1);
		if(!rr)	SetWarn(mglWarnFile);
	}
	else if(!strcmp(com,"readhdf"))
	{
		if(k[0]!=1 || k[1]!=2 || k[2]!=2)	res = 1;
		else a[0].d->ReadHDF(a[1].s, a[2].s);
	}
	else if(!strcmp(com,"resize"))
	{
		if(k[0]==1 && k[1]==1)
			*(a[0].d) = a[1].d->Resize(k[2]==3?int(a[2].v):1,
					k[3]==3?int(a[3].v):1, k[4]==3?int(a[4].v):1);
		else res = 1;
	}
	else if(!strcmp(com,"rotate"))	//D S v_slice sch v_num
	{
		if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3)
			RotateN(a[0].v, a[1].v, a[2].v, a[3].v);
		else if(k[0]==3 && k[1]==3)
			Rotate(a[0].v, a[1].v, k[2]==3?a[2].v:0);
		else	res = 1;
	}
	else if(!strcmp(com,"rotatetext"))	//D S v_slice sch v_num
	{	if(k[0]==3)	RotatedText = (a[0].v!=0);	else	res = 1;	}
	else if(!strcmp(com,"rect"))
	{
		long i;
		if(n>5)
		{
			bool ok=true;
			for(i=0;i<6;i++)	if(a[i].type!=2)	ok=false;
			if(ok)
			{
				if(a[0].v==a[3].v)	Face(mglPoint(a[0].v,a[1].v,a[2].v),
					mglPoint(a[0].v,a[4].v,a[2].v),
					mglPoint(a[3].v,a[1].v,a[5].v),
					mglPoint(a[3].v,a[4].v,a[5].v),
					(n==7 && a[6].type==1) ? a[6].s : 0, 2);
				else	Face(mglPoint(a[0].v,a[1].v,a[2].v),
					mglPoint(a[0].v,a[4].v,a[5].v),
					mglPoint(a[3].v,a[1].v,a[2].v),
					mglPoint(a[3].v,a[4].v,a[5].v),
					(n==7 && a[6].type==1) ? a[6].s : 0, 2);
			}
			else	res = 1;
		}
		else if(n>3)
		{
			bool ok=true;
			float p = (Min.z+Max.z)/2;
			for(i=0;i<4;i++)	if(a[i].type!=2)	ok=false;
			if(ok)
				Face(mglPoint(a[0].v,a[1].v,p), mglPoint(a[0].v,a[3].v,p),
					mglPoint(a[2].v,a[1].v,p), mglPoint(a[2].v,a[3].v,p),
					(n==5 && a[4].type==1) ? a[4].s : 0, 2);
			else	res = 1;
		}
		else	res = 1;
	}
	else res = 2;
	return res;
}
//-----------------------------------------------------------------------------
int mglGraph::exec_s(const char *com, long n, mglArg *a,int k[9])
{
	int res = 0,i;
	if(com[1]=='u')	return exec_su(com,n,a,k);
	else if(!strcmp(com,"save"))
	{	if(k[0]==1 && k[1]==2)	a[0].d->Save(a[1].s);	else	res = 1;}
	else if(!strcmp(com,"savehdf"))
	{
		if(k[0]!=1 || k[1]!=2 || k[2]!=2)	res = 1;
		else a[0].d->SaveHDF(a[1].s, a[2].s);
	}
	else if(!strcmp(com,"smooth"))
	{
		if(k[0]==1)
			a[0].d->Smooth(k[1]==3?int(a[1].v):2,k[2]==2?a[2].s:"xyz");
		else	res = 1;
	}
	else if(!strcmp(com,"stem"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==2)	Stem(*(a[0].d),a[1].s,k[2]==3 ? a[2].v : NAN);
		else if(k[1]==1)
		{
			if(k[2]==2)
				Stem(*(a[0].d),*(a[1].d),a[2].s,k[3]==3 ? a[3].v : NAN);
			else if(k[2]==0)	Stem(*(a[0].d),*(a[1].d));
			else if(k[2]==1)	Stem(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
		}
		else	Stem(*(a[0].d));
	}
	else if(!strcmp(com,"step"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==2)	Step(*(a[0].d),a[1].s,k[2]==3 ? a[2].v : NAN);
		else if(k[1]==1)
		{
			if(k[2]==2)
				Step(*(a[0].d),*(a[1].d),a[2].s,k[3]==3 ? a[3].v : NAN);
			else if(k[2]==0)	Step(*(a[0].d),*(a[1].d));
			else if(k[2]==1)	Step(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
		}
		else	Step(*(a[0].d));
	}
	else if(!strcmp(com,"squeeze"))
	{
		if(k[0]==1 && k[1]==3)
			a[0].d->Squeeze(int(a[1].v), k[2]==3?int(a[2].v):1, k[3]==3?int(a[3].v):1);
		else	res = 1;
	}
	else if(!strcmp(com,"swap"))
	{	if(k[0]==1 && k[1]==2)	a[0].d->Swap(a[1].s);	else	res = 1;}
	else if(!strcmp(com,"setsize"))
	{
		if(k[0]==3 && k[1]==3 && a[1].v>1 && a[0].v>1)
			SetSize(int(a[0].v), int(a[1].v));
		else	res = 1;
	}
	else if(!strcmp(com,"set_id"))
	{
		if(k[0]==1 && k[1]==2)
			a[0].d->SetColumnId(a[1].s);
		else	res = 1;
	}
	else if(!strcmp(com,"sphere"))
	{
		for(i=0;i<7;i++)	if(k[i]!=3)	break;
		if(i==4)
			Sphere(mglPoint(a[0].v,a[1].v,a[2].v), a[3].v, k[4]==2 ? a[4].s:"r");
		else if(i==3)
			Sphere(mglPoint(a[0].v,a[1].v), a[2].v, k[3]==2 ? a[3].s:"r");
		else	res = 1;
	}
	else if(!strcmp(com,"stfad"))
	{
		if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==3)
			*(a[0].d) = ::STFA(*(a[1].d),*(a[2].d), int(a[3].v), k[4]==2?a[4].s[0]:'x');
		else	res = 1;
	}
	else if(!strcmp(com,"stfa"))
	{
		if(k[0]==1 && k[1]==1 && k[2]==3)
			STFA(*(a[0].d),*(a[1].d), int(a[2].v), k[3]==2?a[3].s:"");
		else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1 && k[4]==3)
			STFA(*(a[0].d),*(a[1].d), *(a[2].d),*(a[3].d), int(a[4].v), k[5]==2?a[5].s:"");
		else	res = 1;
	}
	else res = 2;
	return res;
}
//-----------------------------------------------------------------------------
int mglGraph::exec_su(const char *com, long , mglArg *a,int k[9])
{
	int res = 0;
	if(!strcmp(com,"surf"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==0)	Surf(*(a[0].d));
		else if(k[1]==2)	Surf(*(a[0].d),a[1].s);
		else if(k[1]==1 && k[2]==1)
			Surf(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2? a[3].s:0);
		else	res = 1;
	}
	else if(!strcmp(com,"surf3"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==0)	Surf3(*(a[0].d));
		else if(k[1]==1 && k[2]==1 && k[3]==1 && k[4]==3)
			Surf3(a[4].v,*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),
					k[5]==2? a[5].s:0);
		else if(k[1]==3)	Surf3(a[1].v,*(a[0].d),k[2]==2? a[2].s:0);
		else if(k[1]==2)	Surf3(*(a[0].d),a[1].s,int(k[2]==3?a[2].v:3));
		else if(k[1]==1 && k[2]==1 && k[3]==1)
			Surf3(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),
					k[4]==2? a[4].s:0,int(k[5]==3?a[5].v:3));
		else	res = 1;
	}
	else if(!strcmp(com,"surfc"))
	{
		if(k[0]!=1 || k[1]!=1)	res = 1;
		else if(k[2]==0)	SurfC(*(a[0].d),*(a[1].d));
		else if(k[2]==2)	SurfC(*(a[0].d),*(a[1].d),a[2].s);
		else if(k[2]==1 && k[3]==1)
			SurfC(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2? a[4].s:0);
		else	res = 1;
	}
	else if(!strcmp(com,"surf3c"))
	{
		if(k[0]!=1 || k[1]!=1)	res = 1;
		else if(k[2]==0)	Surf3C(*(a[0].d),*(a[1].d));
		else if(k[2]==1 && k[3]==1 && k[4]==1 && k[5]==3)
			Surf3C(a[5].v,*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),
					k[6]==2? a[6].s:0);
		else if(k[2]==3)	Surf3C(a[2].v,*(a[0].d),*(a[1].d),k[3]==2? a[3].s:0);
		else if(k[2]==2)	Surf3C(*(a[0].d),*(a[1].d),a[2].s,int(k[3]==3?a[3].v:3));
		else if(k[2]==1 && k[3]==1 && k[4]==1)
			Surf3C(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),
					k[5]==2? a[5].s:0,int(k[6]==3?a[6].v:3));
		else	res = 1;
	}
	else if(!strcmp(com,"surfa"))
	{
		if(k[0]!=1 || k[1]!=1)	res = 1;
		else if(k[2]==0)	SurfA(*(a[0].d),*(a[1].d));
		else if(k[2]==2)	SurfA(*(a[0].d),*(a[1].d),a[2].s);
		else if(k[2]==1 && k[3]==1)
			SurfA(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2? a[4].s:0);
		else	res = 1;
	}
	else if(!strcmp(com,"surf3a"))
	{
		if(k[0]!=1 || k[1]!=1)	res = 1;
		else if(k[2]==0)	Surf3A(*(a[0].d),*(a[1].d));
		else if(k[2]==1 && k[3]==1 && k[4]==1 && k[5]==3)
			Surf3A(a[5].v,*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),
					k[6]==2? a[6].s:0);
		else if(k[2]==3)	Surf3A(a[2].v,*(a[0].d),*(a[1].d),k[3]==2? a[3].s:0);
		else if(k[2]==2)	Surf3A(*(a[0].d),*(a[1].d),a[2].s,int(k[3]==3?a[3].v:3));
		else if(k[2]==1 && k[3]==1 && k[4]==1)
			Surf3A(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),
					k[5]==2? a[5].s:0,int(k[6]==3?a[6].v:3));
		else	res = 1;
	}
	else if(!strcmp(com,"subdata"))
	{
		if(k[0]==1 && k[1]==1 && k[2]==3)
			*(a[0].d) = a[1].d->SubData(int(a[2].v), k[3]==3?int(a[3].v):-1,
									k[4]==3?int(a[4].v):-1);
		else	res = 1;
	}
	else if(!strcmp(com,"subplot"))
	{
		if(k[0]==3 && k[1]==3 && k[2]==3)
			SubPlot(int(a[0].v),int(a[1].v),int(a[2].v));
		else	res = 1;
	}
	else if(!strcmp(com,"subto"))
	{
		if(k[0]==1 && k[1]==1)		*(a[0].d) -= *(a[1].d);
		else if(k[0]==1 && k[1]==3)	*(a[0].d) -= a[1].v;
		else	res = 1;
	}
	else if(!strcmp(com,"sum"))
	{
		if(k[0]==1 && k[1]==1 && k[2]==2)
			*(a[0].d) = a[1].d->Sum(a[2].s);
		else	res = 1;
	}
	else res = 2;
	return res;
}
//-----------------------------------------------------------------------------
int mglGraph::exec_t(const char *com, long , mglArg *a,int k[9])
{
	int res = 0;
	if(!strcmp(com,"text"))
	{
		if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==2)
			Text(mglPoint(a[0].v,a[1].v,a[2].v),a[3].s,
				(k[4]==2 && a[4].s[0]!=0)?a[4].s:FontDef,k[5]==3?a[5].v:-1.4);
		else if(k[0]==3 && k[1]==3 && k[2]==2)
			Text(mglPoint(a[0].v,a[1].v),a[2].s,
				(k[3]==2 && a[3].s[0]!=0)?a[3].s:FontDef,k[4]==3?a[4].v:-1.4);
		else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==2)
			Text(*(a[0].d),*(a[1].d),*(a[2].d),a[3].s,k[4]==2?a[4].s:0,k[5]==3?a[5].v:-1);
		else if(k[0]==1 && k[1]==1 && k[2]==2)
			Text(*(a[0].d),*(a[1].d),a[2].s,k[3]==2?a[3].s:0,
				k[4]==3?a[4].v:-1,k[5]==3?a[5].v:NAN);
		else if(k[0]==1 && k[1]==2)
			Text(*(a[0].d),a[1].s,k[2]==2?a[2].s:0,k[3]==3?a[3].v:-1,k[4]==3?a[4].v:NAN);
		else	res = 1;
	}
	else if(!strcmp(com,"tile"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==0)	Tile(*(a[0].d));
		else if(k[1]==2)	Tile(*(a[0].d),a[1].s);
		else if(k[1]==1 && k[2]==2)
			Tile(*(a[0].d),*(a[1].d),a[2].s);
		else if(k[1]==1 && k[2]==1 && k[3]==1)
			Tile(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2? a[4].s:0);
		else if(k[1]==1 && k[2]==1)
			Tile(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2? a[3].s:0);
		else	res = 1;
	}
	else if(!strcmp(com,"torus"))
	{
		if(k[0]==1 && k[1]==1)	Torus(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0);
		else if(k[0]==1)		Torus(*(a[0].d),k[1]==2?a[1].s:0);
		else	res = 1;
	}
	else if(!strcmp(com,"transparent"))
	{	if(k[0]==3)	Transparent = (a[0].v!=0);	else	res = 1;	}
	else if(!strcmp(com,"transptype"))
	{	if(k[0]==3)	TranspType = int(a[0].v);	else	res = 1;	}
	else if(!strcmp(com,"transform"))
	{
		if(k[0]==1 && k[1]==2 && k[2]==1 && k[3]==1)
			*(a[0].d) = Transform(*(a[2].d),*(a[3].d),a[1].s);
		else	res = 1;
	}
	else if(!strcmp(com,"transforma"))
	{
		if(k[0]==1 && k[1]==2 && k[2]==1 && k[3]==1)
			*(a[0].d) = TransformA(*(a[2].d),*(a[3].d),a[1].s);
		else	res = 1;
	}
	else if(!strcmp(com,"tube"))
	{
		if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
			Tube(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
		else if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==3)
			Tube(*(a[0].d),*(a[1].d),*(a[2].d),a[3].v,k[4]==2?a[4].s:0);
		else if(k[0]==1 && k[1]==1 && k[2]==1)
			Tube(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0,k[4]==3?a[4].v:NAN);
		else if(k[0]==1 && k[1]==1 && k[2]==3)
			Tube(*(a[0].d),*(a[1].d),a[2].v,k[3]==2?a[3].s:0,k[4]==3?a[4].v:NAN);
		else if(k[0]==1 && k[1]==1)
			Tube(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
		else if(k[0]==1 && k[1]==3)
			Tube(*(a[0].d),a[1].v,k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
		else	res = 1;
	}
	else if(!strcmp(com,"transpose"))
	{	if(k[0]==1)	a[0].d->Transpose(k[1]==2?a[1].s:"yxz");	else	res = 1;	}
	else if(!strcmp(com,"triplot"))
	{
		if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
			TriPlot(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0);
		else if(k[0]==1 && k[1]==1 && k[2]==1)
			TriPlot(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0,k[4]==3?a[4].v:NAN);
		else	res = 1;
	}
	else if(!strcmp(com,"ternary"))
	{	if(k[0]==3)	Ternary(a[0].v!=0);	else	res = 1;	}
	else res = 2;
	return res;
}
//-----------------------------------------------------------------------------
int mglGraph::exec_vz(const char *com, long , mglArg *a,int k[9])
{
	int res = 0;
	long i;
	if(!strcmp(com,"vectc"))
	{
		for(i=0;i<7;i++)	if(k[i]!=1)	break;
		if(i==2)		VectC(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
		else if(i==4)
			VectC(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,k[5]==3?a[5].v:NAN);
		else if(i==3)	VectC(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
		else if(i==6)
			VectC(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),*(a[5].d),k[6]==2?a[6].s:0);
		else	res = 1;
	}
	else if(!strcmp(com,"vect"))
	{
		for(i=0;i<7;i++)	if(k[i]!=1)	break;
		if(i==2)		Vect(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?a[3].v:NAN);
		else if(i==4)
			Vect(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,k[5]==3?a[5].v:NAN);
		else if(i==3)	Vect(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
		else if(i==6)
			Vect(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),*(a[5].d),k[6]==2?a[6].s:0);
		else	res = 1;
	}
	else if(!strcmp(com,"xlabel"))
	{	if(k[0]==2)	Label('x',a[0].s,k[1]==3?int(a[1].v):1);	else	res = 1;	}
	else if(!strcmp(com,"ylabel"))
	{	if(k[0]==2)	Label('y',a[0].s,k[1]==3?int(a[1].v):1);	else	res = 1;	}
	else if(!strcmp(com,"zlabel"))
	{	if(k[0]==2)	Label('z',a[0].s,k[1]==3?int(a[1].v):1);	else	res = 1;	}
	else if(!strcmp(com,"xrange"))
	{
		if(k[0]==1)	XRange(*(a[0].d),k[1]==3?(a[1].v!=0):false);
		else if(k[0]==3 && k[1]==3)
		{	Min.x = a[0].v;	Max.x = a[1].v;	RecalcBorder();	}
		else	res = 1;
	}
	else if(!strcmp(com,"yrange"))
	{
		if(k[0]==1)	YRange(*(a[0].d),k[1]==3?(a[1].v!=0):false);
		else if(k[0]==3 && k[1]==3)
		{	Min.y = a[0].v;	Max.y = a[1].v;	RecalcBorder();	}
		else	res = 1;
	}
	else if(!strcmp(com,"zrange"))
	{
		if(k[0]==1)	ZRange(*(a[0].d),k[1]==3?(a[1].v!=0):false);
		else if(k[0]==3 && k[1]==3)
		{	Min.z = a[0].v;	Max.z = a[1].v;	RecalcBorder();	}
		else	res = 1;
	}
	else if(!strcmp(com,"xtick"))
	{
		if(k[0]==3)	{	dx = a[0].v;	if(k[1]==3)	NSx = int(a[1].v);	}
		else if(k[0]==2)
		{
			if(xtt)	delete []xtt;
			if(a[0].s[0])	xtt = mgl_str_copy(a[0].s);
			else			xtt = 0;
		}
		else	res = 1;
	}
	else if(!strcmp(com,"ytick"))
	{
		if(k[0]==3)	{	dy = a[0].v;	if(k[1]==3)	NSy = int(a[1].v);	}
		else if(k[0]==2)
		{
			if(ytt)	delete []ytt;
			if(a[0].s[0])	ytt = mgl_str_copy(a[0].s);
			else			ytt = 0;
		}
		else	res = 1;
	}
	else if(!strcmp(com,"ztick"))
	{
		if(k[0]==3)	{	dz = a[0].v;	if(k[1]==3)	NSz = int(a[1].v);	}
		else if(k[0]==2)
		{
			if(ztt)	delete []ztt;
			if(a[0].s[0])	ztt = mgl_str_copy(a[0].s);
			else			ztt = 0;
		}
		else	res = 1;
	}
	else if(!strcmp(com,"zoom"))
	{
		if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3)
			Zoom(a[0].v, a[1].v, a[2].v, a[3].v);
		else	res = 1;
	}
	else res = 2;
	return res;
}
//-----------------------------------------------------------------------------
int mglGraph::exec_misc(const char *com, long , mglArg *a,int k[9])
{
	int res = 0;
	if(!strcmp(com,"plot"))
	{
		if(k[0]!=1)	res = 1;
		else if(k[1]==2)	Plot(*(a[0].d),a[1].s,k[2]==3 ? a[2].v : NAN);
		else if(k[1]==1)
		{
			if(k[2]==2)
				Plot(*(a[0].d),*(a[1].d),a[2].s,k[3]==3 ? a[3].v : NAN);
			else if(k[2]==0)	Plot(*(a[0].d),*(a[1].d));
			else if(k[2]==1)	Plot(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0);
		}
		else	Plot(*(a[0].d));
	}
	else if(!strcmp(com,"plotfactor"))
	{
		if(k[0]==3)
		{
			PlotFactor = a[0].v<0 ? PlotFactor:a[0].v;
			AutoPlotFactor = (a[0].v<0);
		}
		else	res = 1;
	}
	else if(!strcmp(com,"pipe"))	//D S v_slice sch v_num
	{
		int i;
		for(i=0;i<7;i++)	if(k[i]!=1)	break;
		if(i==2)
			Pipe(*(a[0].d),*(a[1].d),k[2]==2?a[2].s:0,k[3]==3?a[3].v:0.05,k[4]==3?int(a[4].v):5,
				true,k[5]==3?a[5].v:NAN);
		else if(i==4)
			Pipe(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),k[4]==2?a[4].s:0,k[5]==3?a[5].v:0.05,
				k[6]==3?int(a[6].v):5,true,k[7]==3?a[7].v:NAN);
		else if(i==3)
			Pipe(*(a[0].d),*(a[1].d),*(a[2].d),k[3]==2?a[3].s:0,k[4]==3?a[4].v:0.05,k[5]==3?int(a[5].v):3);
		else if(i==6)
			Pipe(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),*(a[4].d),*(a[5].d),
				k[6]==2?a[6].s:0,k[7]==3?a[7].v:0.05,k[8]==3?int(a[8].v):3);
		else	res = 1;
	}
	else if(!strcmp(com,"error"))
	{
		if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==1)
			Error(*(a[0].d),*(a[1].d),*(a[2].d),*(a[3].d),
				k[4]==2?a[4].s:0, k[5]==3?a[5].v:NAN);
		if(k[0]==1 && k[1]==1 && k[2]==1)
			Error(*(a[0].d),*(a[1].d),*(a[2].d), k[3]==2?a[3].s:0, k[4]==3?a[4].v:NAN);
		else if(k[0]==1 && k[1]==1)
			Error(*(a[0].d),*(a[1].d), k[2]==2?a[2].s:0, k[3]==3?a[3].v:NAN);
		else	res = 1;
	}
	else if(!strcmp(com,"extend"))
	{
		if(k[0]==1 && k[1]==3)
			a[0].d->Extend(int(a[1].v),k[2]==3?int(a[2].v):0);
		else	res = 1;
	}
	else if(!strcmp(com,"origin"))
	{
		if(k[0]==3 && k[1]==3)
			Org = mglPoint(a[0].v,a[1].v,k[2]==3?a[2].v:Min.z);
		else	res = 1;
	}
	else if(!strcmp(com,"norm"))
	{
		if(k[0]==1 && k[1]==3 && k[2]==3)
			a[0].d->Norm(a[1].v,a[2].v,k[3]==3?a[3].v!=0:false,k[4]==3?int(a[4].v):0);
		else	res = 1;
	}
	else if(!strcmp(com,"inplot"))
	{
		if(k[0]==3 && k[1]==3 && k[2]==3 && k[3]==3)
			InPlot(a[0].v,a[1].v,a[2].v,a[3].v);
		else	res = 1;
	}
	else if(!strcmp(com,"integrate"))
	{	if(k[0]==1 && k[1]==2)	a[0].d->Integral(a[1].s);	else	res = 1;	}
	else if(!strcmp(com,"hist"))
	{
		if(k[0]==1 && k[1]==1 && k[2]==1 && k[3]==3 && k[4]==3 && k[5]==3)
			*(a[0].d) = a[1].d->Hist(*(a[2].d),int(a[3].v),a[4].v,a[5].v);
		else if(k[0]==1 && k[1]==1 && k[2]==3 && k[3]==3 && k[4]==3)
			*(a[0].d) = a[1].d->Hist(int(a[2].v),a[3].v,a[4].v);
		else	res = 1;
	}
	else if(!strcmp(com,"info"))
	{
		if(k[0]==1)
			a[0].d->PrintInfo(Message);
		else	res = 1;
	}
	else res = 2;
	return res;
}
//-----------------------------------------------------------------------------
void mglVar::MoveAfter(mglVar *var)
{
	if(prev)	prev->next = next;
	if(next)	next->prev = prev;
	prev = next = 0;
	if(var)
	{
		prev = var;
		next = var->next;
		var->next = this;
		if(func==0)	func = var->func;
	}
	if(next)	next->prev = this;
}
//-----------------------------------------------------------------------------
mglVar::~mglVar()
{
	if(func)	func(o);
	if(prev)	prev->next = next;
	if(next)	next->prev = prev;
}
//-----------------------------------------------------------------------------
mglParse::mglParse(bool setsize)
{
	DataList=0;
	AllowSetSize=setsize;
	Once = true;	Skip = false;
	parlen=0;	Stop=false;
	for(long i=0;i<10;i++)	{	par[i] = 0;	opt[i]=false;	}
}
//-----------------------------------------------------------------------------
bool mglParse::AddParam(int n, const char *str)
{
	if(n<0 || n>9 || strchr(str,'$'))	return false;
	parlen += strlen(str);
	if(par[n])	delete []par[n];
	par[n] = new char[strlen(str)+1];
	strcpy(par[n],str);
	return true;
}
//-----------------------------------------------------------------------------
mglParse::~mglParse()
{
	if(DataList)
	{
		while(DataList->next)	delete DataList->next;
		delete DataList;
	}
	for(long i=0;i<10;i++)	if(par[i])	delete [] par[i];
}
//-----------------------------------------------------------------------------
mglVar *mglParse::FindVar(const char *name)
{
	mglVar *v=DataList;
	char *s = strdup(name),*p;
	p = strchr(s,'.');	if(p)	{	p[0]=0;	p++;	}
	while(v)
	{
		if(!strcmp(s, v->s))	{	free(s);	return v;	}
		v = v->next;
	}
	free(s);
	return 0;
}
//-----------------------------------------------------------------------------
mglVar *mglParse::AddVar(const char *name)
{
	mglVar *v = FindVar(name);
	if(v)	return v;
	v = new mglVar;
	if(strlen(name)<64)	strcpy(v->s, name);
	else	memcpy(v->s, name, 63*sizeof(char));
	v->s[63] = 0;
	if(DataList)	v->MoveAfter(DataList);
	else			DataList = v;
	return v;
}
//-----------------------------------------------------------------------------
int mglFindArg(const char *str)
{
	register long l=0,i;//,j,len=strlen(lst);
	for(i=0;i<long(strlen(str));i++)
	{
		if(l%2==0 && (str[i]=='#' || str[i]==';'))	return -i;
		if(str[i]=='\'') l++;
		if(l%2==0 && (str[i]==' ' || str[i]=='\t' || str[i]=='\n'))	return i;
	}
	return 0;
}
//-----------------------------------------------------------------------------
void strtrim_mgl(char *str);
void strlwr_mgl(char *str);
//-----------------------------------------------------------------------------
// convert substrings to arguments
void mglParse::FillArg(int k, char **arg, mglArg *a)
{
	char *s, *t, *p=0;
	for(long n=1;n<k;n++)
	{
		mglVar *v, *u;
		a[n-1].type = -1;
		p = strchr(arg[n],'.');
		if(arg[n][0]!='\'' && (s=strchr(arg[n],'(')))	// subdata or column in argument
		{
			p = strchr(arg[n],'.');	if(p)	{	p[0]=0;	p++;	}
			s = strdup(arg[n]);
			t = strtok (s, "(,)");
			v = FindVar(t);
			if(v==0)
			{	a[n-1].type = 2;	a[n-1].v = NAN;		free(s);	return;	}
			if(strchr(arg[n],':'))	// this subdata
			{
				long k[3]={-1,-1,-1};
				t = strtok (0, "(,)");	if(t)	k[0] = t[0]==':'?-1:atoi(t);
				t = strtok (0, "(,)");	if(t)	k[1] = t[0]==':'?-1:atoi(t);
				t = strtok (0, "(,)");	if(t)	k[2] = t[0]==':'?-1:atoi(t);
				free(s);
				u = new mglVar;		u->temp = true;
				u->d = v->d.SubData(k[0],k[1],k[2]);
			}
			else	// this is column formula
			{
				char *tt = strtok(0,"'");
				t = strtok(0,"'");	t[-1]=0;
				u = new mglVar;		u->temp = true;
				u->d = v->d.Column(tt);
			}
			if(DataList)	u->MoveAfter(DataList);
			else			DataList = u;
			a[n-1].type = 0;	a[n-1].d = &(u->d);
		}
		else if(arg[n][0]=='|')	a[n-1].type = -1;
		else if(arg[n][0]=='\'')
		{
			a[n-1].type = 1;	arg[n][strlen(arg[n])-1] = 0;
			if(strlen(arg[n]+1)>=2048)	arg[n][2048]=0;
			strcpy(a[n-1].s, arg[n]+1);
		}
		else if((v = FindVar(arg[n]))!=0)
		{
			p = strchr(arg[n],'.');	if(p)	{	p[0]=0;	p++;	}
			a[n-1].type = 0;		a[n-1].d = &(v->d);
		}
		else
		{
			a[n-1].type = 2;
			if(!strcmp(arg[n],"nan"))	a[n-1].v = NAN;
			else if(!strcmp(arg[n],":"))	a[n-1].v = -1;
			else if(!strcmp(arg[n],"pi"))	a[n-1].v = M_PI;
			else if(!strcmp(arg[n],"-pi"))	a[n-1].v = -M_PI;
			else if(!strcmp(arg[n],"on"))	a[n-1].v = 1;
			else if(!strcmp(arg[n],"off"))	a[n-1].v = 0;
			else	a[n-1].v = atof(arg[n]);
		}
		// try to find numbers (momentum) for data
		if(a[n-1].type == 0 && p)
		{
			float x,y,z,k;
			if(!strcmp(p,"a"))
			{	a[n-1].type=2;	a[n-1].v=a[n-1].d->a[0];	}
			else if(!strcmp(p,"fst"))
			{
				a[n-1].type=2;
				int i=-1,j=-1,k=-1;
				a[n-1].v=a[n-1].d->Find(0,i,j,k);
			}
			else if(!strcmp(p,"lst"))
			{
				a[n-1].type=2;
				int i=-1,j=-1,k=-1;
				a[n-1].v=a[n-1].d->Last(0,i,j,k);
			}
			else if(!strcmp(p,"nx"))
			{	a[n-1].type=2;	a[n-1].v=a[n-1].d->nx;	}
			else if(!strcmp(p,"ny"))
			{	a[n-1].type=2;	a[n-1].v=a[n-1].d->ny;	}
			else if(!strcmp(p,"nz"))
			{	a[n-1].type=2;	a[n-1].v=a[n-1].d->nz;	}
			else if(!strcmp(p,"max"))
			{	a[n-1].type=2;	a[n-1].v=a[n-1].d->Maximal();	}
			else if(!strcmp(p,"min"))
			{	a[n-1].type=2;	a[n-1].v=a[n-1].d->Minimal();	}
			else if(!strcmp(p,"sum"))
			{	a[n-1].type=2;	a[n-1].v=a[n-1].d->Momentum('x',x,y);	}
			else if(!strcmp(p,"mx"))
			{	a[n-1].type=2;	a[n-1].d->Maximal(x,y,z);	a[n-1].v=x/a[n-1].d->nx;	}
			else if(!strcmp(p,"my"))
			{	a[n-1].type=2;	a[n-1].d->Maximal(x,y,z);	a[n-1].v=y/a[n-1].d->ny;	}
			else if(!strcmp(p,"mz"))
			{	a[n-1].type=2;	a[n-1].d->Maximal(x,y,z);	a[n-1].v=z/a[n-1].d->nz;	}
			else if(!strcmp(p,"ax"))
			{	a[n-1].type=2;	a[n-1].d->Momentum('x',x,y);a[n-1].v=x/a[n-1].d->nx;	}
			else if(!strcmp(p,"ay"))
			{	a[n-1].type=2;	a[n-1].d->Momentum('y',x,y);a[n-1].v=x/a[n-1].d->ny;	}
			else if(!strcmp(p,"az"))
			{	a[n-1].type=2;	a[n-1].d->Momentum('z',x,y);a[n-1].v=x/a[n-1].d->nz;	}
			else if(!strcmp(p,"wx"))
			{	a[n-1].type=2;	a[n-1].d->Momentum('x',x,y);a[n-1].v=y/a[n-1].d->nx;	}
			else if(!strcmp(p,"wy"))
			{	a[n-1].type=2;	a[n-1].d->Momentum('y',x,y);a[n-1].v=y/a[n-1].d->ny;	}
			else if(!strcmp(p,"wz"))
			{	a[n-1].type=2;	a[n-1].d->Momentum('z',x,y);a[n-1].v=y/a[n-1].d->nz;	}
			else if(!strcmp(p,"sx"))
			{	a[n-1].type=2;	a[n-1].d->Momentum('x',x,y,z,k);
				a[n-1].v=z/a[n-1].d->nx;	}
			else if(!strcmp(p,"sy"))
			{	a[n-1].type=2;	a[n-1].d->Momentum('y',x,y,z,k);
				a[n-1].v=z/a[n-1].d->ny;	}
			else if(!strcmp(p,"sz"))
			{	a[n-1].type=2;	a[n-1].d->Momentum('z',x,y,z,k);
				a[n-1].v=z/a[n-1].d->nz;	}
			else if(!strcmp(p,"kx"))
			{	a[n-1].type=2;	a[n-1].d->Momentum('x',x,y,z,k);
				a[n-1].v=k/a[n-1].d->nx;	}
			else if(!strcmp(p,"ky"))
			{	a[n-1].type=2;	a[n-1].d->Momentum('y',x,y,z,k);
				a[n-1].v=k/a[n-1].d->ny;	}
			else if(!strcmp(p,"kz"))
			{	a[n-1].type=2;	a[n-1].d->Momentum('z',x,y,z,k);
				a[n-1].v=k/a[n-1].d->nz;	}
			else if(!strcmp(p,"aa"))
			{	a[n-1].type=2;	a[n-1].d->Momentum('a',x,y);	a[n-1].v=x;	}
			else if(!strcmp(p,"wa"))
			{	a[n-1].type=2;	a[n-1].d->Momentum('a',x,y);	a[n-1].v=y;	}
			else if(!strcmp(p,"sa"))
			{	a[n-1].type=2;	a[n-1].d->Momentum('a',x,y,z,k);a[n-1].v=z;	}
			else if(!strcmp(p,"ka"))
			{	a[n-1].type=2;	a[n-1].d->Momentum('a',x,y,z,k);a[n-1].v=k;	}
		}
	}
}
//-----------------------------------------------------------------------------
bool check_for_name(const char *s)
{
	return !isalpha(s[0])||strchr(s,'.')||strchr(s,':')||strchr(s,'(')||strchr(s,')');
}
//-----------------------------------------------------------------------------
// return values: 0 - not found, 1 - OK, 2 - wrong arguments,
//				3 - wrong command, 4 - string too long
int mglParse::PreExec(mglGraph *gr, long k, char **arg, mglArg *a)
{
	long n=0;
	mglVar *v;
	if(!strcmp("new",arg[0]))	// parse commands
	{
		if(k<2 || check_for_name(arg[1]))	return 2;
		v = AddVar(arg[1]);		n=1;
		long nx, ny, nz;
		nx = k>2 ? atoi(arg[2]):1;
		ny = k>3 ? atoi(arg[3]):1;
		nz = k>4 ? atoi(arg[4]):1;
		v->d.Create(nx,ny,nz);

	}
	else if(!strcmp("var",arg[0]))	// parse command "new"
	{
		if(k<5 || check_for_name(arg[1]))	return 2;
		v = AddVar(arg[1]);		n=1;
		v->d.Create(atoi(arg[2]));
		v->d.Fill(a[2].v, a[3].v);

	}
	// parse commands for automatic data creation
	else if(!strcmp("read",arg[0]) || !strcmp("readhdf",arg[0]) || !strcmp("copy",arg[0]) || !strcmp("subdata",arg[0]))
	{
		if(k<3 || check_for_name(arg[1]))	return 2;
		v = AddVar(arg[1]);
		a[0].type = 0;	a[0].d = &(v->d);
		n = 1+(gr->Exec(arg[0],k-1,a));
	}
	else if(!strcmp("delete",arg[0]))	// parse command "delete"
	{
		if(k<2)	return 2;
		v = FindVar(arg[1]);
		if(v)
		{
			if(DataList==v)	DataList = v->next;
			delete v;
		}
		n=1;
	}
	else if(!strcmp("list",arg[0]))	// parse command "list"
	{
		if(k<3 || check_for_name(arg[1]))	return 2;
		long nx=0, ny=1,j=0,i,t=0;
		char ch;
		for(i=2;i<k;i++)
		{
			ch = arg[i][0];
			if(a[i-1].type==1)	return 2;
			if(a[i-1].type==0)
			{
				if(t==1)	return 2;
				t=2;	nx++;
			}
			if(a[i-1].type==2)
			{
				if(t==2)	return 2;
				j++;	t=1;
			}
			if(ch=='|' && t==1)		{	nx = j>nx ? j:nx;	j=0;	ny++;	}
		}
		v = AddVar(arg[1]);		n=1;
		if(t==1)	nx = j>nx ? j:nx;
		if(t==1)	// list of numeric values
		{
			v->d.Create(nx,ny);
			j=t=0;
			for(i=2;i<k;i++)
			{
				if(arg[i][0]=='|')	{	t++;	j=0;	}
				else
				{	v->d.a[j+nx*t] = a[i-1].v;	j++;	}
			}
		}
		if(t==2)	// list of data
		{
			mglData *b = a[1].d;
			long nn = 0;
			if(b->nz>1)	return 2;
			if(b->ny>1)
			{
				v->d.Create(b->nx, b->ny, nx);
				nn = b->nx*b->ny;
				for(i=2,j=0;i<k;i++)
				{
					b = a[i-1].d;
					if(b==0 || nn!=b->nx*b->ny)	continue;
					memcpy(v->d.a+j*nn,b->a,nn*(b->nz)*sizeof(float));
					j+=b->nz;
				}
			}
			else
			{
				v->d.Create(b->nx, nx);
				nn = b->nx;
				for(i=2,j=0;i<k;i++)
				{
					b = a[i-1].d;
					if(b==0 || nn!=b->nx)	continue;
					memcpy(v->d.a+j*nn,b->a,nn*(b->ny)*sizeof(float));
					j+=b->ny;
				}
			}
		}
		n=1;
	}
	return n;
}
//-----------------------------------------------------------------------------
// return values: 0 - OK, 1 - wrong arguments, 2 - wrong command, 3 - string too long
int mglParse::Parse(mglGraph *gr, const char *string)
{
	if(!gr || Stop)	return 0;
	char *str, *s = new char[strlen(string)+1+parlen],*arg[1024],*t;
	str = s;
	strcpy(str,string);
	// parse arguments (parameters $1, ..., $9)
	if(parlen>0)
	{
		char *sb = new char[strlen(string)+1];
		while((t=strchr(str,'$'))!=0)
		{
			strcpy(sb,t+2);
			t[0]=0;
			long n = t[1]-'0';
			if(n>=0 && n<=9 && par[n])	strcat(str,par[n]);
			strcat(str,sb);
		}
		delete []sb;
	}

	strtrim_mgl(str);
	if(!strcmp(str,"stop"))	{	Stop = true;	delete []s;	return 0;	}

	long k=0,n;
	while(k<1024)	// parse string to substrings (by spaces)
	{
		n = mglFindArg(str);
		if(n<1)
		{
			if(str[-n]==';')	ProcOpt(gr,str-n);
			if(n<0)	str[-n]=0;
			break;
		}
		str[n]=0;	arg[k] = str;	k++;
		str = str+n+1;	strtrim_mgl(str);
	}
	// try to find last argument
	if(str[0]!=0 && str[0]!='#' && str[0]!=';')	{	arg[k] = str;	k++;	}
	if(k<1) n =0;
	else
	{
		// fill arguments by its values
		mglArg *a = new mglArg[k];
		FillArg(k, arg, a);

		if(!strcmp(arg[0],"once"))
		{
			int res = 1;
			if(a[0].type==2)
			{
				res = 0;
				if(a[0].v)	Skip = !Once;
				else		Skip = Once = false;
			}
			delete []s;	delete []a;
			return res;
		}
		if(Skip)	{	delete []s;	delete []a;	return 0;	}
		if(!strcmp(arg[0],"call"))
		{
			int res = 1;
			if(a[0].type==1)	{	res=0; Execute(gr,a[0].s);	}
			delete []s;	delete []a;
			return res;
		}
		else if(!strcmp(arg[0],"chdir"))
		{
			int res = 1;
			if(a[0].type==1)	{	res=0; chdir(a[0].s);	}
			delete []s;	delete []a;
			return res;
		}

		// alocate new arrays and execute the command
		n = PreExec(gr, k, arg, a);
		if(n>0)	n--;
		else if(!strcmp("setsize",arg[0]) && !AllowSetSize)	n = 2;
		else	n = gr->Exec(arg[0],k-1,a);
		delete []a;
	}
	mglVar *v = DataList, *u;
	while(v)
	{
		u = v->next;
		if(v->temp)
		{	if(DataList==v)	DataList = v->next;		delete v;	}
		v = u;
	}

	ProcOpt(gr,0);
	delete []s;
	return n;
}
//-----------------------------------------------------------------------------
void mglParse::Execute(mglGraph *gr, const char *fname)
{
	if(gr==0)	return;
	FILE *fp = fopen(fname,"rt");
	if(fp==0)	return;
	char str[4096];
	Stop = false;
	while(!feof(fp))
	{
		fgets(str,4096,fp);
		Parse(gr,str);
	}
}
//-----------------------------------------------------------------------------
void mglParse::ProcOpt(mglGraph *gr, char *str)
{
	if(str==0)
	{
		if(opt[0])	{	gr->Min.x = val[0];	gr->Max.x = val[1];	gr->RecalcBorder();	}
		if(opt[1])	{	gr->Min.y = val[2];	gr->Max.y = val[3];	gr->RecalcBorder();	}
		if(opt[2])	{	gr->Min.z = val[4];	gr->Max.z = val[5];	gr->RecalcBorder();	}
		if(opt[3])	{	gr->Cmin = val[6];	gr->Cmax = val[7];	}
		if(opt[4])	gr->Ambient(val[8]);
		if(opt[5])	gr->Cut = val[10]!=0;
		if(opt[6])	gr->AlphaDef = val[12];
		if(opt[7])	gr->MeshNum  = int(val[14]);
		if(opt[8])	gr->FontSize = val[16];
		for(long i=0;i<10;i++)	opt[i]=false;
	}
	else
	{
		val[0] = gr->Min.x;		val[1] = gr->Max.x;
		val[2] = gr->Min.y;		val[3] = gr->Max.y;
		val[4] = gr->Min.z;		val[5] = gr->Max.z;
		val[6] = gr->Cmin;		val[7] = gr->Cmax;
		val[8] = gr->AmbBr;		val[10]= gr->Cut;
		val[12]= gr->AlphaDef;	val[14]= gr->MeshNum;
		val[16]= gr->FontSize;
		char *s = str,*a,*b;
		long n;
		while((s=strchr(s,';'))!=0)
		{
			s++;	strtrim_mgl(s);
			n=mglFindArg(s);
			if(n<1)	return;
			s[n]=0;		a=s;	s=s+n+1;	strtrim_mgl(a);
			if(!strcmp(a+1,"range"))
			{
				n=mglFindArg(s);	if(n<1)	return;
				s[n]=0;		b=s;	s=s+n+1;	strtrim_mgl(b);
				if(a[0]=='x')
				{	opt[0]=true;	gr->Min.x = atof(b);	gr->Max.x = atof(s);}
				else if(a[0]=='y')
				{	opt[1]=true;	gr->Min.y = atof(b);	gr->Max.y = atof(s);}
				else if(a[0]=='z')
				{	opt[2]=true;	gr->Min.z = atof(b);	gr->Max.z = atof(s);}
				else if(a[0]=='c')
				{	opt[3]=true;	gr->Cmin = atof(b);		gr->Cmax = atof(s);	}
				//gr->RecalcBorder();
			}
			else if(!strcmp(a,"cut"))
			{	opt[5]=true;	gr->Cut = atoi(s)!=0 || !strncmp(s,"on",2);	}
			else if(!strcmp(a,"meshnum"))
			{	opt[7]=true;	gr->MeshNum = atoi(s);	}
			else if(!strcmp(a,"alphadef"))
			{	opt[6]=true;	gr->AlphaDef = atof(s);	}
			else if(!strcmp(a,"alpha"))
			{	opt[6]=true;	gr->AlphaDef = atof(s);	}
			else if(!strcmp(a,"fontsize"))
			{	opt[8]=true;	gr->FontSize = atof(s);	}
			else if(!strcmp(a,"ambient"))
			{	opt[4]=true;	gr->Ambient(atof(s));	}
		}
	}
}
//-----------------------------------------------------------------------------
