/***************************************************************************
 * eps_svg.cpp is part of Math Graphic Library
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
#include "mgl/canvas.h"
#include "mgl/canvas_cf.h"
#include <stdlib.h>
#include <time.h>
#include <zlib.h>
void mgl_printf(void *fp, bool gz, const char *str, ...);
//-----------------------------------------------------------------------------
char *mgl_get_dash(unsigned short d, float w)
{
	static char s[64],b[4];
	if(d==0xffff)	{	strcpy(s,"");	return s;	}
	int f=0, p=d&1, n=p?0:1, i, j;
	strcpy(s, p ? "" : "0");
	for(i=0;i<16;i++)
	{
		j = i;//15-i;
		if(((d>>j)&1) == p)	f++;
		else
		{
			sprintf(b," %g",f*w);	strcat(s,b);
			p = (d>>j)&1;	f = 1;	n++;
		}
	}
	sprintf(b," %g",f*w);	strcat(s,b);
	strcat(s,n%2 ? "" : " 0");
	return s;
}
//-----------------------------------------------------------------------------
bool mglPrim::IsSame(float wp,float *cp,int st)
{
	if(abs(type)!=1)	return false;
	if(w>=1 && wp!=w)	return false;
	if(w<1 && wp!=1)	return false;
	if(st!=style)		return false;
	float c[4];
	gr->put_color(c,this);
	return (cp[0]==c[0] && cp[1]==c[1] && cp[2]==c[2]);
}
//-----------------------------------------------------------------------------
void mglCanvas::WriteEPS(const char *fname,const char *descr)
{
	if(!P)	return;
	if(!Finished)	Finish();
	time_t now;
	time(&now);

	bool gz = fname[strlen(fname)-1]=='z';
	void *fp = gz ? gzopen(fname,"wt") : fopen(fname,"wt");
	if(!fp)		{	SetWarn(mglWarnOpen,fname);	return;	}
	mgl_printf(fp, gz, "%%!PS-Adobe-3.0 EPSF-3.0\n%%%%BoundingBox: 0 0 %d %d\n",Width,Height);
	mgl_printf(fp, gz, "%%%%Creator: MathGL library\n%%%%Title: %s\n",descr ? descr : fname);
	mgl_printf(fp, gz, "%%%%CreationDate: %s\n",ctime(&now));
	mgl_printf(fp, gz, "/lw {setlinewidth} def\n/rgb {setrgbcolor} def\n");
	mgl_printf(fp, gz, "/np {newpath} def\n/cp {closepath} def\n");
	mgl_printf(fp, gz, "/ll {lineto} def\n/mt {moveto} def\n");
	mgl_printf(fp, gz, "/rl {rlineto} def\n/rm {rmoveto} def\n/dr {stroke} def\n");
	mgl_printf(fp, gz, "/ss {%g} def\n",MarkSize*0.4*font_factor);// remove *font_factor); ???
	mgl_printf(fp, gz, "/s2 {%g} def\n",MarkSize*0.8*font_factor);// remove *font_factor); ???
	mgl_printf(fp, gz, "/sm {-%g} def\n",MarkSize*0.4*font_factor);//remove *font_factor); ???
	mgl_printf(fp, gz, "/m_c {ss 0.3 mul 0 360 arc} def\n");
	mgl_printf(fp, gz, "/d0 {[] 0 setdash} def\n/sd {setdash} def\n");

	bool m_p=false,m_x=false,m_d=false,m_v=false,m_t=false,
		m_s=false,m_a=false,m_o=false,m_T=false,
		m_V=false,m_S=false,m_D=false,m_Y=false,m_l=false,
		m_L=false,m_r=false,m_R=false,m_X=false,m_P=false;
	register long i;
	// add mark definition if present
	for(i=0;i<pNum;i++)
	{
		if(P[i].type>0)	continue;		if(P[i].m=='+')	m_p = true;
		if(P[i].m=='x')	m_x = true;		if(P[i].m=='s')	m_s = true;
		if(P[i].m=='d')	m_d = true;		if(P[i].m=='v')	m_v = true;
		if(P[i].m=='^')	m_t = true;		if(P[i].m=='*')	m_a = true;
		if(P[i].m=='o' || P[i].m=='O' || P[i].m=='C')	m_o = true;
		if(P[i].m=='S')	m_S = true;		if(P[i].m=='D')	m_D = true;
		if(P[i].m=='V')	m_V = true;		if(P[i].m=='T')	m_T = true;
		if(P[i].m=='<')	m_l = true;		if(P[i].m=='L')	m_L = true;
		if(P[i].m=='>')	m_r = true;		if(P[i].m=='R')	m_R = true;
		if(P[i].m=='Y')	m_Y = true;
		if(P[i].m=='P')	m_P = true;		if(P[i].m=='X')	m_X = true;
	}
	if(m_P)	{	m_p=true;	m_s=true;	}
	if(m_X)	{	m_x=true;	m_s=true;	}
	if(m_p)	mgl_printf(fp, gz, "/m_p {sm 0 rm s2 0 rl sm sm rm 0 s2 rl d0} def\n");
	if(m_x)	mgl_printf(fp, gz, "/m_x {sm sm rm s2 s2 rl 0 sm 2 mul rm sm 2 mul s2 rl d0} def\n");
	if(m_s)	mgl_printf(fp, gz, "/m_s {sm sm rm 0 s2 rl s2 0 rl 0 sm 2 mul rl cp d0} def\n");
	if(m_d)	mgl_printf(fp, gz, "/m_d {sm 0 rm ss ss rl ss sm rl sm sm rl cp d0} def\n");
	if(m_v)	mgl_printf(fp, gz, "/m_v {sm ss 2 div rm s2 0 rl sm sm 1.5 mul rl d0 cp} def\n");
	if(m_t)	mgl_printf(fp, gz, "/m_t {sm sm 2 div rm s2 0 rl sm ss 1.5 mul rl d0 cp} def\n");
	if(m_a)	mgl_printf(fp, gz, "/m_a {sm 0 rm s2 0 rl sm 1.6 mul sm 0.8 mul rm ss 1.2 mul ss 1.6 mul rl 0 sm 1.6 mul rm sm 1.2 mul ss 1.6 mul rl d0} def\n");
	if(m_o)	mgl_printf(fp, gz, "/m_o {ss 0 360 d0 arc} def\n");
	if(m_S)	mgl_printf(fp, gz, "/m_S {sm sm rm 0 s2 rl s2 0 rl 0 sm 2 mul rl cp} def\n");
	if(m_D)	mgl_printf(fp, gz, "/m_D {sm 0 rm ss ss rl ss sm rl sm sm rl cp} def\n");
	if(m_V)	mgl_printf(fp, gz, "/m_V {sm ss 2 div rm s2 0 rl sm sm 1.5 mul rl cp} def\n");
	if(m_T)	mgl_printf(fp, gz, "/m_T {sm sm 2 div rm s2 0 rl sm ss 1.5 mul rl cp} def\n");
	if(m_Y)	mgl_printf(fp, gz, "/m_Y {0 sm rm 0 ss rl sm ss rl s2 0 rm sm sm rl d0} def\n");
	if(m_r)	mgl_printf(fp, gz, "/m_r {sm 2 div sm rm 0 s2 rl ss 1.5 mul sm rl d0 cp} def\n");
	if(m_l)	mgl_printf(fp, gz, "/m_l {ss 2 div sm rm 0 s2 rl sm 1.5 mul sm rl d0 cp} def\n");
	if(m_R)	mgl_printf(fp, gz, "/m_R {sm 2 div sm rm 0 s2 rl ss 1.5 mul sm rl cp} def\n");
	if(m_L)	mgl_printf(fp, gz, "/m_L {ss 2 div sm rm 0 s2 rl sm 1.5 mul sm rl cp} def\n");
	if(m_P)	mgl_printf(fp, gz, "/m_P {m_p 0 sm rm m_s} def\n");
	if(m_X)	mgl_printf(fp, gz, "/m_X {m_x ss sm rm m_s} def\n");
//	if(m_C)	mgl_printf(fp, gz, "/m_C {m_c m_o} def\n");
	mgl_printf(fp, gz, "\n");

	// write definition for all glyphs
	put_desc(fp,gz,"/%c%c_%04x { np\n", "\t%d %d mt ", "%d %d ll ", "cp\n", "} def\n");
	// write primitives
	float cp[4],wp=-1;
	int st=0;
	char str[256]="";
	for(i=0;i<pNum;i++)
	{
		if(P[i].type<0)	continue;
		put_color(cp, P+i);
		if(P[i].type>1)	sprintf(str,"%.2g %.2g %.2g rgb ", cp[0],cp[1],cp[2]);

		if(P[i].type==0)	// mark
		{
			float x0 = pntC[4*P[i].n1],y0 = pntC[4*P[i].n1+1];
			sprintf(str,"1 lw %.2g %.2g %.2g rgb ", cp[0],cp[1],cp[2]);
			wp=1;
			if(P[i].s!=MarkSize)
			{
				mgl_printf(fp, gz, "/ss {%g} def\n",P[i].s*0.4*font_factor);
				mgl_printf(fp, gz, "/s2 {%g} def\n",P[i].s*0.8*font_factor);
				mgl_printf(fp, gz, "/sm {-%g} def\n",P[i].s*0.4*font_factor);
			}
			switch(P[i].m)
			{
			case '+':	mgl_printf(fp, gz, "np %g %g mt m_p %sdr\n",x0,y0,str);	break;
			case 'x':	mgl_printf(fp, gz, "np %g %g mt m_x %sdr\n",x0,y0,str);	break;
			case 's':	mgl_printf(fp, gz, "np %g %g mt m_s %sdr\n",x0,y0,str);	break;
			case 'd':	mgl_printf(fp, gz, "np %g %g mt m_d %sdr\n",x0,y0,str);	break;
			case '*':	mgl_printf(fp, gz, "np %g %g mt m_a %sdr\n",x0,y0,str);	break;
			case 'v':	mgl_printf(fp, gz, "np %g %g mt m_v %sdr\n",x0,y0,str);	break;
			case '^':	mgl_printf(fp, gz, "np %g %g mt m_t %sdr\n",x0,y0,str);	break;
			case 'S':	mgl_printf(fp, gz, "np %g %g mt m_S %sfill\n",x0,y0,str);	break;
			case 'D':	mgl_printf(fp, gz, "np %g %g mt m_D %sfill\n",x0,y0,str);	break;
			case 'V':	mgl_printf(fp, gz, "np %g %g mt m_V %sfill\n",x0,y0,str);	break;
			case 'T':	mgl_printf(fp, gz, "np %g %g mt m_T %sfill\n",x0,y0,str);	break;
			case 'o':	mgl_printf(fp, gz, "%g %g m_o %sdr\n",x0,y0,str);break;
			case 'O':	mgl_printf(fp, gz, "%g %g m_o %sfill\n",x0,y0,str);break;
			case 'Y':	mgl_printf(fp, gz, "np %g %g mt m_Y %sdr\n",x0,y0,str);	break;
			case '<':	mgl_printf(fp, gz, "np %g %g mt m_l %sdr\n",x0,y0,str);	break;
			case '>':	mgl_printf(fp, gz, "np %g %g mt m_r %sdr\n",x0,y0,str);	break;
			case 'L':	mgl_printf(fp, gz, "np %g %g mt m_L %sfill\n",x0,y0,str);	break;
			case 'R':	mgl_printf(fp, gz, "np %g %g mt m_R %sfill\n",x0,y0,str);	break;
			case 'P':	mgl_printf(fp, gz, "np %g %g mt m_P %sdr\n",x0,y0,str);	break;
			case 'X':	mgl_printf(fp, gz, "np %g %g mt m_X %sdr\n",x0,y0,str);	break;
			case 'C':	mgl_printf(fp, gz, "%g %g m_o %g %g m_c %sdr\n",x0,y0,x0,y0,str);	break;
			default:	mgl_printf(fp, gz, "%g %g m_c %sfill\n",x0,y0,str);
			}
			if(P[i].s!=MarkSize)
			{
				mgl_printf(fp, gz, "/ss {%g} def\n",MarkSize*0.4*font_factor);
				mgl_printf(fp, gz, "/s2 {%g} def\n",MarkSize*0.8*font_factor);
				mgl_printf(fp, gz, "/sm {-%g} def\n",MarkSize*0.4*font_factor);
			}
		}
		else if(P[i].type==3)	// quad
			mgl_printf(fp, gz, "np %g %g mt %g %g ll %g %g ll %g %g ll cp %sfill\n",
						pntN[8*P[i].n1], pntN[8*P[i].n1+1], pntN[8*P[i].n2], pntN[8*P[i].n2+1],
						pntN[8*P[i].n4], pntN[8*P[i].n4+1], pntN[8*P[i].n3], pntN[8*P[i].n3+1], str);
		else if(P[i].type==2)	// trig
			mgl_printf(fp, gz, "np %g %g mt %g %g ll %g %g ll cp %sfill\n",
						pntN[8*P[i].n1], pntN[8*P[i].n1+1], pntN[8*P[i].n2], pntN[8*P[i].n2+1],
						pntN[8*P[i].n3], pntN[8*P[i].n3+1], str);
		else if(P[i].type==1)	// line
		{
			sprintf(str,"%.2g lw %.2g %.2g %.2g rgb ", P[i].w>1 ? P[i].w:1., cp[0],cp[1],cp[2]);
			wp = P[i].w>1  ? P[i].w:1;	st = P[i].style;
			put_line(fp,gz,i,wp,cp,st, "np %g %g mt ", "%g %g ll ", false);
			const char *sd = mgl_get_dash(P[i].style,P[i].w);
			if(sd && sd[0])	mgl_printf(fp, gz, "%s [%s] %g sd dr\n",str,sd,P[i].w*P[i].s);
			else			mgl_printf(fp, gz, "%s d0 dr\n",str);
		}
		else if(P[i].type==4)	// glyph
		{
			float 	ss = P[i].s/P[i].p/1.1, 	xx = pntC[4*P[i].n2],
					yy = pntC[4*P[i].n2+1], zz = pntC[4*P[i].n2+2];
			mgl_printf(fp, gz, "gsave\t%g %g translate %g %g scale %g rotate %s\n",
				pntC[4*P[i].n1], pntC[4*P[i].n1+1], ss, ss, -P[i].w, str);
			if(P[i].style&8)	// this is "line"
			{
				float dy = 0.004,f=fabs(zz);
				mgl_printf(fp, gz, "np %g %g mt %g %g ll %g %g ll %g %g ll cp ",
					xx,yy+dy, xx+f,yy+dy, xx+f,yy-dy, xx,yy-dy);
			}
			else
				mgl_printf(fp, gz, "%.3g %.3g translate %g %g scale %c%c_%04x ",
					xx, yy, zz, zz, P[i].style&1?'b':'n',
					P[i].style&2?'i':'n', P[i].m);
			if(P[i].style&4)	mgl_printf(fp, gz, "dr");
			else	mgl_printf(fp, gz, "eofill");
			mgl_printf(fp, gz, " grestore\n");
		}
	}
	for(i=0;i<pNum;i++)		if(P[i].type==-1)	P[i].type = 1;
	mgl_printf(fp, gz, "\nshowpage\n%%%%EOF\n");
	if(gz)	gzclose(fp);	else	fclose((FILE *)fp);
}
//-----------------------------------------------------------------------------
void mglCanvas::WriteSVG(const char *fname,const char *descr)
{
	if(!P)	return;
	if(!Finished)	Finish();
	time_t now;
	time(&now);

	bool gz = fname[strlen(fname)-1]=='z';
	void *fp = gz ? gzopen(fname,"wt") : fopen(fname,"wt");
	if(!fp)		{	SetWarn(mglWarnOpen,fname);	return;	}
	mgl_printf(fp, gz, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
	mgl_printf(fp, gz, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 20000303 Stylable//EN\" \"http://www.w3.org/TR/2000/03/WD-SVG-20000303/DTD/svg-20000303-stylable.dtd\">\n");
	mgl_printf(fp, gz, "<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n",Width,Height);

	mgl_printf(fp, gz, "<!--Creator: MathGL library-->\n");
	mgl_printf(fp, gz, "<!--Title: %s-->\n<!--CreationDate: %s-->\n\n",descr?descr:fname,ctime(&now));

	// write definition for all glyphs
	put_desc(fp,gz,"<symbol id=\"%c%c_%04x\"><path d=\"", "\tM %d %d ",
		"L %d %d ", "Z\n", "\"/></symbol>\n");
	// currentColor -> inherit ???
	mgl_printf(fp, gz, "<g fill=\"none\" stroke=\"none\" stroke-width=\"0.5\">\n");
	// write primitives
	float cp[4],wp=-1;
	register long i;
	int st=0;

	for(i=0;i<pNum;i++)
	{
		put_color(cp, P+i);
		if(P[i].type==0)
		{
			float x=pntC[4*P[i].n1],y=pntC[4*P[i].n1+1],s=0.4*font_factor*P[i].s;
			if(!strchr("xsSoO",P[i].m))	s *= 1.1;
			wp = 1;
			if(strchr("SDVTLR",P[i].m))
				mgl_printf(fp, gz, "<g fill=\"#%02x%02x%02x\">\n",
					int(255*cp[0]),int(255*cp[1]),int(255*cp[2]));
			else
				mgl_printf(fp, gz, "<g stroke=\"#%02x%02x%02x\">\n",
					int(255*cp[0]),int(255*cp[1]),int(255*cp[2]));
			switch(P[i].m)
			{
			case 'P':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g M %g %g L %g %g M %g %g L %g %g L %g %g L %g %g L %g %g\"/>\n", x-s,y,x+s,y,x,y-s,x,y+s, x-s,y-s,x+s,y-s,x+s,y+s,x-s,y+s,x-s,y-s);	break;
			case '+':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g M %g %g L %g %g\"/>\n",
						x-s,y,x+s,y,x,y-s,x,y+s);	break;
			case 'X':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g M %g %g L %g %g M %g %g L %g %g L %g %g L %g %g L %g %g\"/>\n", x-s,y-s,x+s,y+s,x+s,y-s,x-s,y+s, x-s,y-s,x+s,y-s,x+s,y+s,x-s,y+s,x-s,y-s);	break;
			case 'x':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g M %g %g L %g %g\"/>\n",
						x-s,y-s,x+s,y+s,x+s,y-s,x-s,y+s);	break;
			case 's':
			case 'S':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %g L %g %gZ\"/>\n",
						x-s,y-s,x+s,y-s,x+s,y+s,x-s,y+s);	break;
			case 'd':
			case 'D':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %g L %g %gZ\"/>\n",
						x-s,y,x,y-s,x+s,y,x,y+s);	break;
			case '^':
			case 'T':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %gZ\"/>\n",
						x-s,y+s/2,x+s,y+s/2,x,y-s);	break;
			case 'v':
			case 'V':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %gZ\"/>\n",
						x-s,y-s/2,x+s,y-s/2,x,y+s);	break;
			case '<':
			case 'L':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %gZ\"/>\n",
						x+s/2,y+s,x+s/2,y-s,x-s,y);	break;
			case '>':
			case 'R':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %gZ\"/>\n",
						x-s/2,y+s,x-s/2,y-s,x+s,y);	break;
			case 'Y':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %g M %g %g L %g %g\"/>\n",
						x,y+s, x,y, x+s,y-s, x,y, x-s,y-s);	break;
			case 'C':
				mgl_printf(fp, gz, "<circle style=\"fill:#%02x%02x%02x\" cx=\"%g\" cy=\"%g\" r=\"0.15\"/>\n<circle cx=\"%g\" cy=\"%g\" r=\"%g\"/>\n", int(255*cp[0]),int(255*cp[1]),int(255*cp[2]),x,y,x,y,s);	break;
			case 'o':
				mgl_printf(fp, gz, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"/>\n",
						x,y,s);	break;
			case 'O':
				mgl_printf(fp, gz, "<circle style=\"fill:#%02x%02x%02x\" cx=\"%g\" cy=\"%g\" r=\"%g\"/>\n",
						int(255*cp[0]),int(255*cp[1]),int(255*cp[2]),x,y,s);	break;
			case '*':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g M %g %g L %g %g M %g %g L %g %g\"/>\n",
						x-s,y,x+s,y,x-0.8*s,y-1.6*s,x+0.8*s,y+1.6*s,x+0.8*s,y-1.6*s,x-0.8*s,y+1.6*s);	break;
			default:
				mgl_printf(fp, gz, "<circle style=\"fill:#%02x%02x%02x\" cx=\"%g\" cy=\"%g\" r=\"0.15\"/>\n",
						int(255*cp[0]),int(255*cp[1]),int(255*cp[2]),x,y);	break;
			}
			mgl_printf(fp, gz, "</g>\n");
		}
		else if(P[i].type==2 && cp[3]>0)
		{
			mgl_printf(fp, gz, "<g fill=\"#%02x%02x%02x\" opacity=\"%g\">\n",
				int(255*cp[0]),int(255*cp[1]),int(255*cp[2]),cp[3]);
			mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %g Z\"/> </g>\n",
				pntN[8*P[i].n1], Height-pntN[8*P[i].n1+1], pntN[8*P[i].n2], Height-pntN[8*P[i].n2+1],
				pntN[8*P[i].n3], Height-pntN[8*P[i].n3+1]);
		}
		else if(P[i].type==3 && cp[3]>0)
		{
			mgl_printf(fp, gz, "<g fill=\"#%02x%02x%02x\" opacity=\"%g\">\n",
				int(255*cp[0]),int(255*cp[1]),int(255*cp[2]),cp[3]);
			mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %g L %g %g Z\"/> </g>\n",
				pntN[8*P[i].n1], Height-pntN[8*P[i].n1+1], pntN[8*P[i].n2], Height-pntN[8*P[i].n2+1],
				pntN[8*P[i].n3], Height-pntN[8*P[i].n3+1], pntN[8*P[i].n4], Height-pntN[8*P[i].n4+1]);
		}
		else if(P[i].type==1)
		{
//			const char *dash[]={"", "8 8","4 4","1 3","7 4 1 4","3 2 1 2"};
			mgl_printf(fp, gz, "<g stroke=\"#%02x%02x%02x\"",
					int(255*cp[0]),int(255*cp[1]),int(255*cp[2]));
			if(P[i].style)
			{
				mgl_printf(fp, gz, " stroke-dasharray=\"%s\"", mgl_get_dash(P[i].style,P[i].w));
				mgl_printf(fp, gz, " stroke-dashoffset=\"%g\"", P[i].s*P[i].w);
			}
			if(P[i].w>1)	mgl_printf(fp, gz, " stroke-width=\"%g\"", P[i].w);
			wp = P[i].w>1  ? P[i].w:1;	st = P[i].style;
			put_line(fp,gz,i,wp,cp,st, "><path d=\" M %g %g", " L %g %g", true);
			mgl_printf(fp, gz, "\"/> </g>\n");
		}
		else if(P[i].type==4)	// TODO: add after font enabled
		{
			float ss = P[i].s/2, xx = pntC[4*P[i].n2], yy = pntC[4*P[i].n2+1], zz = pntC[4*P[i].n2+2];
			if(P[i].style&8)	// this is "line"
			{
				mgl_printf(fp, gz, "<g transform=\"translate(%g,%g) scale(%.3g,%.3g) rotate(%g)\"",
					pntC[4*P[i].n1], Height-pntC[4*P[i].n1+1], ss, -ss, -P[i].w);
				if(P[i].style&4)
					mgl_printf(fp, gz, " stroke=\"#%02x%02x%02x\">", int(255*cp[0]),int(255*cp[1]),int(255*cp[2]));
				else
					mgl_printf(fp, gz, " fill=\"#%02x%02x%02x\">", int(255*cp[0]),int(255*cp[1]),int(255*cp[2]));
				float dy = 0.004,f=fabs(zz);
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %g L %g %g\"/></g>\n",
					xx,yy+dy, xx+f,yy+dy, xx+f,yy-dy, xx,yy-dy);
			}
			else
			{
				ss *= zz;
				mgl_printf(fp, gz, "<g transform=\"translate(%g,%g) scale(%.3g,%.3g) rotate(%g)\"",
					pntC[4*P[i].n1], Height-pntC[4*P[i].n1+1], ss, -ss, -P[i].w);
				if(P[i].style&4)
					mgl_printf(fp, gz, " stroke=\"#%02x%02x%02x\">", int(255*cp[0]),int(255*cp[1]),int(255*cp[2]));
				else
					mgl_printf(fp, gz, " fill=\"#%02x%02x%02x\">", int(255*cp[0]),int(255*cp[1]),int(255*cp[2]));
				mgl_printf(fp, gz, "<use x=\"%g\" y=\"%g\" xlink:href=\"#%c%c_%04x\"/></g>\n",
					xx/zz, yy/zz, P[i].style&1?'b':'n',
					P[i].style&2?'i':'n', P[i].m);
			}
		}
	}

	for(i=0;i<pNum;i++)		if(P[i].type==-1)	P[i].type = 1;
	mgl_printf(fp, gz, "</g></svg>");
	if(gz)	gzclose(fp);	else	fclose((FILE *)fp);
}
//-----------------------------------------------------------------------------
#define xx(i,n)	(pntC[4*P[i].n])
#define yy(i,n)	(pntC[4*P[i].n+1])
void mglCanvas::put_line(void *fp, bool gz, long i, float wp, float *cp,int st, const char *ifmt, const char *nfmt, bool neg)
{
	long k = i,j;	// first point
	float x0=xx(i,n1), y0=yy(i,n1);
	bool ok=true;
	while(ok)
	{
		for(ok=false,j=i+1;j<pNum && P[j].type<2;j++)
			if(P[j].IsSame(wp,cp,st) && P[j].type==1)
			{	// previous point
				if(xx(j,n2)==x0 && yy(j,n2)==y0)
				{
					k=j;	ok=true;	P[k].type = -2;
					x0 = xx(k,n1);	y0=yy(k,n1);
				}
				else if(xx(j,n1)==x0 && yy(j,n1)==y0)
				{
					k=j;	ok=true;	P[k].type = -2;
					x0 = xx(k,n2);	y0=yy(k,n2);
				}
			}
	}
	for(j=i;j<pNum ;j++)	if(P[j].type==-2)	P[j].type = 1;
	mgl_printf(fp, gz, ifmt,x0,neg?Height-y0:y0);	ok=true;
	long m=1;
	while(ok)
	{
		for(ok=false,j=i;j<pNum && P[j].type<2;j++)
			if(P[j].IsSame(wp,cp,st) && P[j].type==1)
			{
				if(xx(j,n1)==x0 && yy(j,n1)==y0)
				{
					k=j;	P[k].type = -1;
					x0 = xx(k,n2);	y0=yy(k,n2);
					mgl_printf(fp, gz, nfmt,x0,neg?Height-y0:y0);
					if(m>10)	{	m=0;	mgl_printf(fp, gz, "\n");	}
					ok=true;	m++;
				}
				else if(xx(j,n2)==x0 && yy(j,n2)==y0)
				{
					k=j;	P[k].type = -1;
					x0 = xx(k,n1);	y0=yy(k,n1);
					mgl_printf(fp, gz, nfmt,x0,neg?Height-y0:y0);
					if(m>10)	{	m=0;	mgl_printf(fp, gz, "\n");	}
					ok=true;	m++;
				}
			}
	}
}
//-----------------------------------------------------------------------------
//put_desc(fp,"%c%c%c_%04x {", "np %d %d mt %d %d ll %d %d ll cp fill\n",
//"np %d %d mt ", "%d %d ll ", "cp dr\n", "} def")
void mglCanvas::put_desc(void *fp, bool gz, const char *pre, const char *ln1, const char *ln2, const char *ln3, const char *suf)
{
	register long i,j,n;
	wchar_t *g;
	int *s;
	for(n=i=0;i<pNum;i++)	if(P[i].type==4)	n++;
	if(n==0)	return;		// no glyphs
	g = new wchar_t[n];	s = new int[n];
	for(n=i=0;i<pNum;i++)
	{
		if(P[i].type!=4 || (P[i].style&8))	continue;	// not a glyph
		bool is=false;
		for(j=0;j<n;j++)	if(g[j]==P[i].m && s[j]==(P[i].style&7))	is = true;
		if(is)	continue;		// glyph is described
		// have to describe
		g[n]=P[i].m;	s[n]=P[i].style&7;	n++;	// add to list of described
		// "%c%c%c_%04x {"
		mgl_printf(fp, gz, pre, P[i].style&1?'b':'n', P[i].style&2?'i':'n', P[i].m);
		int nl=fnt->GetNl(P[i].style&3,P[i].m);	// TODO: add after font enabled
		const short *ln=fnt->GetLn(P[i].style&3,P[i].m);
		long ik,ii;
		bool np=true;
		if(ln && nl>0)	for(ik=0;ik<nl;ik++)
		{
			ii = 2*ik;
			if(ln[ii]==0x3fff && ln[ii+1]==0x3fff)	// line breakthrough
			{	mgl_printf(fp, gz, "%s",ln3);	np=true;	continue;	}
			else if(np)	mgl_printf(fp, gz, ln1,ln[ii],ln[ii+1]);
			else		mgl_printf(fp, gz, ln2,ln[ii],ln[ii+1]);
			np=false;
		}
		mgl_printf(fp, gz, "%s%s",ln3,suf);	// finish glyph description suf="} def"
	}
	delete []g;		delete []s;
}
//-----------------------------------------------------------------------------
void mglCanvas::put_color(float *c, mglPrim *p)
{
	float n[3]={NAN,NAN,NAN},u,v=0;
	register long i, j = p->type==1 ? p->n2:p->n1;
	if(p->type==2 || p->type==3)
	{	u=pntN[8*j+3];	v=pntN[8*j+4];
		memcpy(n,pntN+8*j+5,3*sizeof(float));	}
	else	u=pntC[4*j+3];
	txt[long(u)].GetC(u,v,c);

	if(UseLight && !isnan(n[0]))
	{
		float d0,d1,d2,nn;
		c[0] *= AmbBr;	c[1] *= AmbBr;	c[2] *= AmbBr;
		for(i=0;i<10;i++)
		{
			if(!nLight[i])	continue;
			j = 3*i;
			nn = 2*(n[0]*pLight[j]+n[1]*pLight[j+1]+n[2]*pLight[j+2]) /
					(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]+1e-6);
			d0 = pLight[j] - n[0]*nn;
			d1 = pLight[j+1]-n[1]*nn;
			d2 = pLight[j+2]-n[2]*nn;
			nn = 1 + d2/sqrt(d0*d0+d1*d1+d2*d2+1e-6);

			nn = exp(-aLight[i]*nn)*bLight[i]*2;
			c[0] += nn*cLight[j];
			c[1] += nn*cLight[j+1];
			c[2] += nn*cLight[j+2];
		}
		c[0] = c[0]<1 ? c[0] : 1;
		c[1] = c[1]<1 ? c[1] : 1;
		c[2] = c[2]<1 ? c[2] : 1;
	}
}
//-----------------------------------------------------------------------------
#undef _GR_
#define _GR_	((mglCanvas *)(*gr))
#define _Gr_	((mglCanvas *)(gr))
void mgl_write_eps(HMGL gr, const char *fname,const char *descr)
{	_Gr_->WriteEPS(fname,descr);	}
void mgl_write_svg(HMGL gr, const char *fname,const char *descr)
{	_Gr_->WriteSVG(fname,descr);	}
//-----------------------------------------------------------------------------
void mgl_write_eps_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *d=new char[n+1];	memcpy(d,descr,n);	d[n]=0;
	_GR_->WriteEPS(s,d);	delete []s;		delete []d;	}
void mgl_write_svg_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *d=new char[n+1];	memcpy(d,descr,n);	d[n]=0;
	_GR_->WriteSVG(s,d);	delete []s;		delete []d;	}
//-----------------------------------------------------------------------------
