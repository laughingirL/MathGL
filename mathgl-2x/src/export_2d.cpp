/***************************************************************************
 * export_2d.cpp is part of Math Graphic Library
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
#undef _GR_
#define _GR_	((mglCanvas *)(*gr))
#define _Gr_	((mglCanvas *)(gr))
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
bool mgl_is_same(HMGL gr, const mglPrim &pr,float wp,mglColor cp,int st)
{
	if(abs(pr.type)!=1)	return false;
	if(pr.w>=1 && wp!=pr.w)	return false;
	if(pr.w<1 && wp!=1)	return false;
	if(st!=pr.n3)			return false;
	mglColor c=_Gr_->GetColor(pr);
	return (cp==c);
}
//-----------------------------------------------------------------------------
void put_line(HMGL gr, void *fp, bool gz, long i, float wp, mglColor cp,int st, const char *ifmt, const char *nfmt, bool neg)
{
	const mglPnt pp = gr->GetPnt(gr->GetPrm(i).n1);
	float x0=pp.x, y0=pp.y;
	bool ok=true;
	register unsigned long k = i,j;	// first point
	while(ok)
	{
		for(ok=false,j=i+1;j<gr->GetPrmNum();j++)
		{
			mglPrim &q = gr->GetPrm(j);
			if(q.type>1)	break;
			if(mgl_is_same(gr, q,wp,cp,st) && q.type==1)	// previous point
			{
				const mglPnt p1 = gr->GetPnt(q.n1);
				const mglPnt p2 = gr->GetPnt(q.n2);
				if(p2.x==x0 && p2.y==y0)
				{
					k=j;	ok=true;	q.type = -2;
					x0 = p1.x;	y0=p1.y;
				}
				else if(p1.x==x0 && p1.y==y0)
				{
					k=j;	ok=true;	q.type = -2;
					x0 = p2.x;	y0=p2.y;
				}
			}
		}
	}
	for(j=i;j<gr->GetPrmNum();j++)
	{
		mglPrim &q = gr->GetPrm(j);
		if(q.type==-2)	q.type = 1;
	}
	mgl_printf(fp, gz, ifmt,x0,neg?_Gr_->GetHeight()-y0:y0);	ok=true;
	long m=1;
	while(ok)
	{
		for(ok=false,j=i;j<gr->GetPrmNum();j++)
		{
			mglPrim &q = gr->GetPrm(j);
			if(q.type>1)	break;
			if(mgl_is_same(gr,q,wp,cp,st) && q.type==1)
			{
				const mglPnt p1 = gr->GetPnt(q.n1);
				const mglPnt p2 = gr->GetPnt(q.n2);
				if(p1.x==x0 && p1.y==y0)
				{
					k=j;	q.type = -1;	x0 = p2.x;	y0=p2.y;
					mgl_printf(fp, gz, nfmt,x0,neg?_Gr_->GetHeight()-y0:y0);
					if(m>10)	{	m=0;	mgl_printf(fp, gz, "\n");	}
					ok=true;	m++;
				}
				else if(p2.x==x0 && p2.y==y0)
				{
					k=j;	q.type = -1;	x0 = p1.x;	y0=p1.y;
					mgl_printf(fp, gz, nfmt,x0,neg?_Gr_->GetHeight()-y0:y0);
					if(m>10)	{	m=0;	mgl_printf(fp, gz, "\n");	}
					ok=true;	m++;
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------
//put_desc(fp,"%c%c%c_%04x {", "np %d %d mt %d %d ll %d %d ll cp fill\n",
//"np %d %d mt ", "%d %d ll ", "cp dr\n", "} def")
void put_desc(HMGL gr, void *fp, bool gz, const char *pre, const char *ln1, const char *ln2, const char *ln3, const char *suf)
{
	register unsigned long i,j,n;
	wchar_t *g;
	int *s;
	for(n=i=0;i<gr->GetPrmNum();i++)	if(gr->GetPrm(i).type==4)	n++;
	if(n==0)	return;		// no glyphs
	g = new wchar_t[n];	s = new int[n];
	for(n=i=0;i<gr->GetPrmNum();i++)
	{
		const mglPrim q = gr->GetPrm(i);
		if(q.type!=4 || (q.n3&8))	continue;	// not a glyph
		bool is=false;
		for(j=0;j<n;j++)	if(g[j]==q.n4 && s[j]==(q.n3&7))	is = true;
		if(is)	continue;		// glyph is described
		// have to describe
		g[n]=q.n4;	s[n]=q.n3&7;	n++;	// add to list of described
		// "%c%c%c_%04x {"
		mgl_printf(fp, gz, pre, q.n3&1?'b':'n', q.n3&2?'i':'n', q.n4);
		int nl=gr->GetFont()->GetNl(q.n3&3,q.n4);
		const short *ln=gr->GetFont()->GetLn(q.n3&3,q.n4);
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
mglColor mglCanvas::GetColor(const mglPrim &p)
{
	const mglPnt &q=Pnt[(p.type==1 ? p.n2:p.n1)];
	mglColor c(q.r,q.g,q.b,q.a);

	if(get(MGL_ENABLE_LIGHT) && !isnan(q.u))
	{
		float d0,d1,d2,nn;
		c *= AmbBr;
		register long i;
		for(i=0;i<10;i++)
		{
			if(!light[i].n)	continue;
			nn = 2*(q.u*light[i].p.x+q.v*light[i].p.y+q.w*light[i].p.z) /
					(q.u*q.u+q.v*q.v+q.w*q.w+1e-6);
			d0 = light[i].p.x - q.u*nn;
			d1 = light[i].p.y - q.v*nn;
			d2 = light[i].p.z - q.w*nn;
			nn = 1 + d2/sqrt(d0*d0+d1*d1+d2*d2+1e-6);

			nn = exp(-light[i].a*nn)*light[i].b*2;
			c += nn*light[i].c;
		}
		c.r = c.r<1 ? c.r : 1;
		c.g = c.g<1 ? c.g : 1;
		c.b = c.b<1 ? c.b : 1;
	}
	return c;
}
//-----------------------------------------------------------------------------
void mgl_write_eps(HMGL gr, const char *fname,const char *descr)
{
	if(gr->GetPrmNum()<1)	return;
	_Gr_->Finish();
	time_t now;	time(&now);

	bool gz = fname[strlen(fname)-1]=='z';
	void *fp = gz ? (void*)gzopen(fname,"wt") : (void*)fopen(fname,"wt");
	if(!fp)		{	gr->SetWarn(mglWarnOpen,fname);	return;	}
	mgl_printf(fp, gz, "%%!PS-Adobe-3.0 EPSF-3.0\n%%%%BoundingBox: 0 0 %d %d\n", _Gr_->GetWidth(), _Gr_->GetHeight());
	mgl_printf(fp, gz, "%%%%Creator: MathGL library\n%%%%Title: %s\n",descr ? descr : fname);
	mgl_printf(fp, gz, "%%%%CreationDate: %s\n",ctime(&now));
	mgl_printf(fp, gz, "/lw {setlinewidth} def\n/rgb {setrgbcolor} def\n");
	mgl_printf(fp, gz, "/np {newpath} def\n/cp {closepath} def\n");
	mgl_printf(fp, gz, "/ll {lineto} def\n/mt {moveto} def\n");
	mgl_printf(fp, gz, "/rl {rlineto} def\n/rm {rmoveto} def\n/dr {stroke} def\n");
	mgl_printf(fp, gz, "/ss {%g} def\n",0.4*gr->mark_size());
	mgl_printf(fp, gz, "/s2 {%g} def\n",0.8*gr->mark_size());
	mgl_printf(fp, gz, "/sm {-%g} def\n",0.4*gr->mark_size());
	mgl_printf(fp, gz, "/m_c {ss 0.3 mul 0 360 arc} def\n");
	mgl_printf(fp, gz, "/d0 {[] 0 setdash} def\n/sd {setdash} def\n");

	bool m_p=false,m_x=false,m_d=false,m_v=false,m_t=false,
	m_s=false,m_a=false,m_o=false,m_T=false,
	m_V=false,m_S=false,m_D=false,m_Y=false,m_l=false,
	m_L=false,m_r=false,m_R=false,m_X=false,m_P=false;
	register unsigned long i;
	// add mark definition if present
	for(i=0;i<gr->GetPrmNum();i++)
	{
		const mglPrim q = gr->GetPrm(i);
		if(q.type>0)	continue;		if(q.n4=='+')	m_p = true;
		if(q.n4=='x')	m_x = true;		if(q.n4=='s')	m_s = true;
		if(q.n4=='d')	m_d = true;		if(q.n4=='v')	m_v = true;
		if(q.n4=='^')	m_t = true;		if(q.n4=='*')	m_a = true;
		if(q.n4=='o' || q.n4=='O' || q.n4=='C')	m_o = true;
		if(q.n4=='S')	m_S = true;		if(q.n4=='D')	m_D = true;
		if(q.n4=='V')	m_V = true;		if(q.n4=='T')	m_T = true;
		if(q.n4=='<')	m_l = true;		if(q.n4=='L')	m_L = true;
		if(q.n4=='>')	m_r = true;		if(q.n4=='R')	m_R = true;
		if(q.n4=='Y')	m_Y = true;
		if(q.n4=='P')	m_P = true;		if(q.n4=='X')	m_X = true;
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
	put_desc(gr,fp,gz,"/%c%c_%04x { np\n", "\t%d %d mt ", "%d %d ll ", "cp\n", "} def\n");
	// write primitives
	float wp=-1;
	mglColor cp;
	int st=0;
	char str[256]="";
	for(i=0;i<gr->GetPrmNum();i++)
	{
		const mglPrim &q = gr->GetPrm(i);
		if(q.type<0)	continue;	// q.n1>=0 always
		cp = _Gr_->GetColor(q);
		const mglPnt p1 = gr->GetPnt(q.n1);
		if(q.type>1)	sprintf(str,"%.2g %.2g %.2g rgb ", cp.r,cp.g,cp.b);

		if(q.type==0)	// mark
		{
			float x0 = p1.x,y0 = p1.y;
			sprintf(str,"1 lw %.2g %.2g %.2g rgb ", cp.r,cp.g,cp.b);
			wp=1;
			if(q.s!=gr->mark_size()/gr->FontFactor())
			{
				mgl_printf(fp, gz, "/ss {%g} def\n",q.s*0.4*gr->FontFactor());
				mgl_printf(fp, gz, "/s2 {%g} def\n",q.s*0.8*gr->FontFactor());
				mgl_printf(fp, gz, "/sm {-%g} def\n",q.s*0.4*gr->FontFactor());
			}
			switch(q.n4)
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
			if(q.s!=gr->mark_size()/gr->FontFactor())
			{
				mgl_printf(fp, gz, "/ss {%g} def\n",0.4*gr->mark_size());
				mgl_printf(fp, gz, "/s2 {%g} def\n",0.8*gr->mark_size());
				mgl_printf(fp, gz, "/sm {-%g} def\n",0.4*gr->mark_size());
			}
		}
		else if(q.type==3)	// quad
		{
			const mglPnt p2=gr->GetPnt(q.n2), p3=gr->GetPnt(q.n3), p4=gr->GetPnt(q.n4);
			mgl_printf(fp, gz, "np %g %g mt %g %g ll %g %g ll %g %g ll cp %sfill\n",
					   p1.x, p1.y, p2.x, p2.y, p4.x, p4.y, p3.x, p3.y, str);
		}
		else if(q.type==2)	// trig
		{
			const mglPnt p2=gr->GetPnt(q.n2), p3=gr->GetPnt(q.n3);
			mgl_printf(fp, gz, "np %g %g mt %g %g ll %g %g ll cp %sfill\n",
					   p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, str);
		}
		else if(q.type==1)	// line
		{
			sprintf(str,"%.2g lw %.2g %.2g %.2g rgb ", q.w>1 ? q.w:1., cp.r,cp.g,cp.b);
			wp = q.w>1  ? q.w:1;	st = q.n3;
			put_line(gr,fp,gz,i,wp,cp,st, "np %g %g mt ", "%g %g ll ", false);
			const char *sd = mgl_get_dash(q.n3,q.w);
			if(sd && sd[0])	mgl_printf(fp, gz, "%s [%s] %g sd dr\n",str,sd,q.w*q.s);
			else			mgl_printf(fp, gz, "%s d0 dr\n",str);
		}
		else if(q.type==4)	// glyph
		{
			float 	ss = q.s/q.p/1.1, xx = p1.u, yy = p1.v, zz = p1.w;
			mgl_printf(fp, gz, "gsave\t%g %g translate %g %g scale %g rotate %s\n",
					   p1.x, p1.y, ss, ss, -q.w, str);
			if(q.n3&8)	// this is "line"
			{
				float dy = 0.004,f=fabs(zz);
				mgl_printf(fp, gz, "np %g %g mt %g %g ll %g %g ll %g %g ll cp ",
						   xx,yy+dy, xx+f,yy+dy, xx+f,yy-dy, xx,yy-dy);
			}
			else
				mgl_printf(fp, gz, "%.3g %.3g translate %g %g scale %c%c_%04x ",
						   xx, yy, zz, zz, q.n3&1?'b':'n', q.n3&2?'i':'n', q.n4);
			if(q.n3&4)	mgl_printf(fp, gz, "dr");
			else	mgl_printf(fp, gz, "eofill");
			mgl_printf(fp, gz, " grestore\n");
		}
	}
	for(i=0;i<gr->GetPrmNum();i++)
	{
		mglPrim &q = gr->GetPrm(i);
		if(q.type==-1)	q.type = 1;
	}
	mgl_printf(fp, gz, "\nshowpage\n%%%%EOF\n");
	if(gz)	gzclose((gzFile)fp);	else	fclose((FILE *)fp);
}
void mgl_write_eps_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *d=new char[n+1];	memcpy(d,descr,n);	d[n]=0;
	mgl_write_eps(_GR_,s,d);	delete []s;		delete []d;	}
//-----------------------------------------------------------------------------
void mgl_write_svg(HMGL gr, const char *fname,const char *descr)
{
	if(gr->GetPrmNum()<1)	return;
	_Gr_->Finish();
	time_t now;	time(&now);

	bool gz = fname[strlen(fname)-1]=='z';
	long hh = _Gr_->GetHeight();
	void *fp = gz ? (void*)gzopen(fname,"wt") : (void*)fopen(fname,"wt");
	if(!fp)		{	gr->SetWarn(mglWarnOpen,fname);	return;	}
	mgl_printf(fp, gz, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
	mgl_printf(fp, gz, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 20000303 Stylable//EN\" \"http://www.w3.org/TR/2000/03/WD-SVG-20000303/DTD/svg-20000303-stylable.dtd\">\n");
	mgl_printf(fp, gz, "<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n", _Gr_->GetWidth(), hh);

	mgl_printf(fp, gz, "<!--Creator: MathGL library-->\n");
	mgl_printf(fp, gz, "<!--Title: %s-->\n<!--CreationDate: %s-->\n\n",descr?descr:fname,ctime(&now));

	// write definition for all glyphs
	put_desc(gr,fp,gz,"<symbol id=\"%c%c_%04x\"><path d=\"", "\tM %d %d ",
			 "L %d %d ", "Z\n", "\"/></symbol>\n");
	// currentColor -> inherit ???
	mgl_printf(fp, gz, "<g fill=\"none\" stroke=\"none\" stroke-width=\"0.5\">\n");
	// write primitives
	float wp=-1;
	register unsigned long i;
	int st=0;
	mglColor cp;

	for(i=0;i<gr->GetPrmNum();i++)
	{
		const mglPrim &q = gr->GetPrm(i);
		if(q.type<0)	continue;	// q.n1>=0 always
		cp = _Gr_->GetColor(q);
		const mglPnt p1=gr->GetPnt(q.n1);
		if(q.type==0)
		{
			float x=p1.x,y=p1.y,s=0.4*gr->FontFactor()*q.s;
			if(!strchr("xsSoO",q.n4))	s *= 1.1;
			wp = 1;
			if(strchr("SDVTLR",q.n4))
				mgl_printf(fp, gz, "<g fill=\"#%02x%02x%02x\">\n", int(255*cp.r),int(255*cp.g),int(255*cp.b));
			else
				mgl_printf(fp, gz, "<g stroke=\"#%02x%02x%02x\">\n", int(255*cp.r),int(255*cp.g),int(255*cp.b));
			switch(q.n4)
			{
			case 'P':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g M %g %g L %g %g M %g %g L %g %g L %g %g L %g %g L %g %g\"/>\n", x-s,y,x+s,y,x,y-s,x,y+s, x-s,y-s,x+s,y-s,x+s,y+s,x-s,y+s,x-s,y-s);	break;
			case '+':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g M %g %g L %g %g\"/>\n", x-s,y,x+s,y,x,y-s,x,y+s);	break;
			case 'X':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g M %g %g L %g %g M %g %g L %g %g L %g %g L %g %g L %g %g\"/>\n", x-s,y-s,x+s,y+s,x+s,y-s,x-s,y+s, x-s,y-s,x+s,y-s,x+s,y+s,x-s,y+s,x-s,y-s);	break;
			case 'x':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g M %g %g L %g %g\"/>\n", x-s,y-s,x+s,y+s,x+s,y-s,x-s,y+s);	break;
			case 's':
			case 'S':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %g L %g %gZ\"/>\n", x-s,y-s,x+s,y-s,x+s,y+s,x-s,y+s);	break;
			case 'd':
			case 'D':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %g L %g %gZ\"/>\n", x-s,y,x,y-s,x+s,y,x,y+s);	break;
			case '^':
			case 'T':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %gZ\"/>\n", x-s,y+s/2,x+s,y+s/2,x,y-s);	break;
			case 'v':
			case 'V':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %gZ\"/>\n", x-s,y-s/2,x+s,y-s/2,x,y+s);	break;
			case '<':
			case 'L':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %gZ\"/>\n", x+s/2,y+s,x+s/2,y-s,x-s,y);	break;
			case '>':
			case 'R':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %gZ\"/>\n", x-s/2,y+s,x-s/2,y-s,x+s,y);	break;
			case 'Y':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %g M %g %g L %g %g\"/>\n", x,y+s, x,y, x+s,y-s, x,y, x-s,y-s);	break;
			case 'C':
				mgl_printf(fp, gz, "<circle style=\"fill:#%02x%02x%02x\" cx=\"%g\" cy=\"%g\" r=\"0.15\"/>\n<circle cx=\"%g\" cy=\"%g\" r=\"%g\"/>\n", int(255*cp.r),int(255*cp.g),int(255*cp.b),x,y,x,y,s);	break;
			case 'o':
				mgl_printf(fp, gz, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"/>\n", x,y,s);	break;
			case 'O':
				mgl_printf(fp, gz, "<circle style=\"fill:#%02x%02x%02x\" cx=\"%g\" cy=\"%g\" r=\"%g\"/>\n", int(255*cp.r),int(255*cp.g),int(255*cp.b),x,y,s);	break;
			case '*':
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g M %g %g L %g %g M %g %g L %g %g\"/>\n", x-s,y,x+s,y,x-0.6*s,y-0.8*s,x+0.6*s,y+0.8*s,x+0.6*s,y-0.8*s,x-0.6*s,y+0.8*s);	break;
			default:
				mgl_printf(fp, gz, "<circle style=\"fill:#%02x%02x%02x\" cx=\"%g\" cy=\"%g\" r=\"0.15\"/>\n", int(255*cp.r),int(255*cp.g),int(255*cp.b),x,y);	break;
			}
			mgl_printf(fp, gz, "</g>\n");
		}
		else if(q.type==2 && cp.a>0)
		{
			const mglPnt p2=gr->GetPnt(q.n2), p3=gr->GetPnt(q.n3);
			mgl_printf(fp, gz, "<g fill=\"#%02x%02x%02x\" opacity=\"%g\">\n", int(255*cp.r),int(255*cp.g),int(255*cp.b),cp.a);
			mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %g Z\"/> </g>\n", p1.x, hh-p1.y, p2.x, hh-p2.y, p3.x, hh-p3.y);
		}
		else if(q.type==3 && cp.a>0)
		{
			const mglPnt p2=gr->GetPnt(q.n2), p3=gr->GetPnt(q.n3), p4=gr->GetPnt(q.n4);
			mgl_printf(fp, gz, "<g fill=\"#%02x%02x%02x\" opacity=\"%g\">\n", int(255*cp.r),int(255*cp.g),int(255*cp.b),cp.a);
			mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %g L %g %g Z\"/> </g>\n", p1.x, hh-p1.y, p2.x, hh-p2.y, p3.x, hh-p3.y, p4.x, hh-p4.y);
		}
		else if(q.type==1)
		{
			//			const char *dash[]={"", "8 8","4 4","1 3","7 4 1 4","3 2 1 2"};
			mgl_printf(fp,gz,"<g stroke=\"#%02x%02x%02x\"",int(255*cp.r),int(255*cp.g),int(255*cp.b));
			if(q.n3)
			{
				mgl_printf(fp, gz, " stroke-dasharray=\"%s\"", mgl_get_dash(q.n3,q.w));
				mgl_printf(fp, gz, " stroke-dashoffset=\"%g\"", q.s*q.w);
			}
			if(q.w>1)	mgl_printf(fp, gz, " stroke-width=\"%g\"", q.w);
			wp = q.w>1  ? q.w:1;	st = q.n3;
			put_line(gr,fp,gz,i,wp,cp,st, "><path d=\" M %g %g", " L %g %g", true);
			mgl_printf(fp, gz, "\"/> </g>\n");
		}
		else if(q.type==4)
		{
			float ss = q.s/2, xx = p1.u, yy = p1.v, zz = p1.w;
			if(q.n3&8)	// this is "line"
			{
				mgl_printf(fp, gz, "<g transform=\"translate(%g,%g) scale(%.3g,%.3g) rotate(%g)\"", p1.x, hh-p1.y, ss, -ss, -q.w);
				if(q.n3&4)
					mgl_printf(fp, gz, " stroke=\"#%02x%02x%02x\">", int(255*cp.r),int(255*cp.g),int(255*cp.b));
				else
					mgl_printf(fp, gz, " fill=\"#%02x%02x%02x\">", int(255*cp.r),int(255*cp.g),int(255*cp.b));
				float dy = 0.004,f=fabs(zz);
				mgl_printf(fp, gz, "<path d=\"M %g %g L %g %g L %g %g L %g %g\"/></g>\n", xx,yy+dy, xx+f,yy+dy, xx+f,yy-dy, xx,yy-dy);
			}
			else
			{
				ss *= zz;
				mgl_printf(fp, gz, "<g transform=\"translate(%g,%g) scale(%.3g,%.3g) rotate(%g)\"", p1.x, hh-p1.y, ss, -ss, -q.w);
				if(q.n3&4)
					mgl_printf(fp, gz, " stroke=\"#%02x%02x%02x\">", int(255*cp.r),int(255*cp.g),int(255*cp.b));
				else
					mgl_printf(fp, gz, " fill=\"#%02x%02x%02x\">", int(255*cp.r),int(255*cp.g),int(255*cp.b));
				mgl_printf(fp, gz, "<use x=\"%g\" y=\"%g\" xlink:href=\"#%c%c_%04x\"/></g>\n", xx/zz, yy/zz, q.n3&1?'b':'n', q.n3&2?'i':'n', q.n4);
			}
		}
	}

	for(i=0;i<gr->GetPrmNum();i++)
	{	mglPrim &q=gr->GetPrm(i);	if(q.type==-1)	q.type = 1;	}
	mgl_printf(fp, gz, "</g></svg>");
	if(gz)	gzclose((gzFile)fp);	else	fclose((FILE *)fp);
}
void mgl_write_svg_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *d=new char[n+1];	memcpy(d,descr,n);	d[n]=0;
	mgl_write_svg(_GR_,s,d);	delete []s;		delete []d;	}
//-----------------------------------------------------------------------------
/// Color names easely parsed by LaTeX
struct mglSVGName	{	const char *name;	float r,g,b;	};
mglSVGName mgl_names[]={{"AliceBlue",.94,.972,1},
{"AntiqueWhite",.98,.92,.844},
{"Aqua",0,1,1},
{"Aquamarine",.498,1,.83},
{"Azure",.94,1,1},
{"Beige",.96,.96,.864},
{"Bisque",1,.894,.77},
{"Black",0,0,0},
{"BlanchedAlmond",1,.92,.804},
{"Blue",0,0,1},
{"BlueViolet",.54,.17,.888},
{"Brown,.648",.165,.165},
{"BurlyWood",.87,.72,.53},
{"CadetBlue",.372,.62,.628},
{"Chartreuse",.498,1,0},
{"Chocolate",.824,.41,.116},
{"Coral",1,.498,.312},
{"CornflowerBlue",.392,.585,.93},
{"Cornsilk",1,.972,.864},
{"Crimson",.864,.08,.235},
{"Cyan",0,1,1},
{"DarkBlue",0,0,.545},
{"DarkCyan",0,.545,.545},
{"DarkGoldenrod",.72,.525,.044},
{"DarkGray",.664,.664,.664},
{"DarkGreen",0,.392,0},
{"DarkGrey",.664,.664,.664},
{"DarkKhaki",.74,.716,.42},
{"DarkMagenta",.545,0,.545},
{"DarkOliveGreen",.332,.42,.185},
{"DarkOrange",1,.55,0},
{"DarkOrchid",.6,.196,.8},
{"DarkRed",.545,0,0},
{"DarkSalmon",.912,.59,.48},
{"DarkSeaGreen",.56,.736,.56},
{"DarkSlateBlue",.284,.24,.545},
{"DarkSlateGray",.185,.31,.31},
{"DarkSlateGrey",.185,.31,.31},
{"DarkTurquoise",0,.808,.82},
{"DarkViolet",.58,0,.828},
{"DeepPink",1,.08,.576},
{"DeepSkyBlue",0,.75,1},
{"DimGray",.41,.41,.41},
{"DimGrey",.41,.41,.41},
{"DodgerBlue",.116,.565,1},
{"FireBrick",.698,.132,.132},
{"FloralWhite",1,.98,.94},
{"ForestGreen",.132,.545,.132},
{"Fuchsia",1,0,1},
{"Gainsboro",.864,.864,.864},
{"GhostWhite",.972,.972,1},
{"Gold",1,.844,0},
{"Goldenrod",.855,.648,.125},
{"Gray",.5,.5,.5},
{"Green",0,.5,0},
{"GreenYellow",.68,1,.185},
{"Grey",.5,.5,.5},
{"Honeydew",.94,1,.94},
{"HotPink",1,.41,.705},
{"IndianRed",.804,.36,.36},
{"Indigo",.294,0,.51},
{"Ivory",1,1,.94},
{"Khaki",.94,.9,.55},
{"Lavender",.9,.9,.98},
{"LavenderBlush",1,.94,.96},
{"LawnGreen",.488,.99,0},
{"LemonChiffon",1,.98,.804},
{"LightBlue",.68,.848,.9},
{"LightCoral",.94,.5,.5},
{"LightCyan",.88,1,1},
{"LightGoldenrod",.933,.867,.51},
{"LightGoldenrodYellow",.98,.98,.824},
{"LightGray",.828,.828,.828},
{"LightGreen",.565,.932,.565},
{"LightGrey",.828,.828,.828},
{"LightPink",1,.712,.756},
{"LightSalmon",1,.628,.48},
{"LightSeaGreen",.125,.698,.668},
{"LightSkyBlue",.53,.808,.98},
{"LightSlateBlue",.518,.44,1},
{"LightSlateGray",.468,.532,.6},
{"LightSlateGrey",.468,.532,.6},
{"LightSteelBlue",.69,.77,.87},
{"LightYellow",1,1,.88},
{"Lime",0,1,0},
{"LimeGreen",.196,.804,.196},
{"Linen",.98,.94,.9},
{"Magenta",1,0,1},
{"Maroon",.5,0,0},
{"MediumAquamarine",.4,.804,.668},
{"MediumBlue",0,0,.804},
{"MediumOrchid",.73,.332,.828},
{"MediumPurple",.576,.44,.86},
{"MediumSeaGreen",.235,.7,.444},
{"MediumSlateBlue",.484,.408,.932},
{"MediumSpringGreen",0,.98,.604},
{"MediumTurquoise",.284,.82,.8},
{"MediumVioletRed",.78,.084,.52},
{"MidnightBlue",.098,.098,.44},
{"MintCream",.96,1,.98},
{"MistyRose",1,.894,.884},
{"Moccasin",1,.894,.71},
{"NavajoWhite",1,.87,.68},
{"Navy",0,0,.5},
{"NavyBlue",0,0,.5},
{"OldLace",.992,.96,.9},
{"Olive",.5,.5,0},
{"OliveDrab",.42,.556,.136},
{"Orange",1,.648,0},
{"OrangeRed",1,.27,0},
{"Orchid",.855,.44,.84},
{"PaleGoldenrod",.932,.91,.668},
{"PaleGreen",.596,.985,.596},
{"PaleTurquoise",.688,.932,.932},
{"PaleVioletRed",.86,.44,.576},
{"PapayaWhip",1,.936,.835},
{"PeachPuff",1,.855,.725},
{"Peru",.804,.52,.248},
{"Pink",1,.752,.796},
{"Plum",.868,.628,.868},
{"PowderBlue",.69,.88,.9},
{"Purple",.5,0,.5},
{"Red",1,0,0},
{"RosyBrown",.736,.56,.56},
{"RoyalBlue",.255,.41,.884},
{"SaddleBrown",.545,.27,.075},
{"Salmon",.98,.5,.448},
{"SandyBrown",.956,.644,.376},
{"SeaGreen",.18,.545,.34},
{"Seashell",1,.96,.932},
{"Sienna",.628,.32,.176},
{"Silver",.752,.752,.752},
{"SkyBlue",.53,.808,.92},
{"SlateBlue",.415,.352,.804},
{"SlateGray",.44,.5,.565},
{"SlateGrey",.44,.5,.565},
{"Snow",1,.98,.98},
{"SpringGreen",0,1,.498},
{"SteelBlue",.275,.51,.705},
{"Tan",.824,.705,.55},
{"Teal",0,.5,.5},
{"Thistle",.848,.75,.848},
{"Tomato",1,.39,.28},
{"Turquoise",.25,.88,.815},
{"Violet",.932,.51,.932},
{"VioletRed",.816,.125,.565},
{"Wheat",.96,.87,.7},
{"White",1,1,1},
{"WhiteSmoke",.96,.96,.96},
{"Yellow",1,1,0},
{"YellowGreen",.604,.804,.196},
{"",-1,-1,-1}};
//-----------------------------------------------------------------------------
const char *mglColorName(mglColor c)	// return closest SVG color
{
	register long i;
	register float d, dm=10;
	const char *name="";
	for(i=0;mgl_names[i].name[0];i++)
	{
		d = fabs(c.r-mgl_names[i].r)+fabs(c.g-mgl_names[i].g)+fabs(c.b-mgl_names[i].b);
		if(d<dm)	{	dm=d;	name=mgl_names[i].name;	}
	}
	return name;
}
//-----------------------------------------------------------------------------
void mgl_write_tex(HMGL gr, const char *fname,const char *descr)
{
	if(gr->GetPrmNum()<1)	return;
	_Gr_->Finish();
	FILE *fp = fopen(fname,"wt");
	if(!fp)		{	gr->SetWarn(mglWarnOpen,fname);	return;	}
	fprintf(fp, "%% Creator: MathGL library\n%% Title: %s\n\\begin{tikzpicture}\n",descr?descr:fname);
	fprintf(fp, "\\providecommand{mgl1}{%g}\n",0.4*gr->mark_size());
	fprintf(fp, "\\providecommand{mgl2}{%g}\n",0.2*gr->mark_size());

	// write primitives
	float wp=-1;
	register unsigned long i;
	int st=0;
	mglColor cp;
	const char *cname;

	// add mark definition if present
	bool m_p=false,m_x=false,m_d=false,m_v=false,m_t=false,
	m_s=false,m_a=false,m_o=false,m_O=false,m_T=false,
	m_V=false,m_S=false,m_D=false,m_Y=false,m_l=false,
	m_L=false,m_r=false,m_R=false,m_X=false,m_P=false;
	for(i=0;i<gr->GetPrmNum();i++)
	{
		const mglPrim q = gr->GetPrm(i);
		if(q.type>0)	continue;		if(q.n4=='+')	m_p = true;
		if(q.n4=='x')	m_x = true;		if(q.n4=='s')	m_s = true;
		if(q.n4=='d')	m_d = true;		if(q.n4=='v')	m_v = true;
		if(q.n4=='^')	m_t = true;		if(q.n4=='*')	m_a = true;
		if(q.n4=='O')	m_O = true;		if(q.n4=='o' || q.n4=='C')	m_o = true;
		if(q.n4=='S')	m_S = true;		if(q.n4=='D')	m_D = true;
		if(q.n4=='V')	m_V = true;		if(q.n4=='T')	m_T = true;
		if(q.n4=='<')	m_l = true;		if(q.n4=='L')	m_L = true;
		if(q.n4=='>')	m_r = true;		if(q.n4=='R')	m_R = true;
		if(q.n4=='Y')	m_Y = true;
		if(q.n4=='P')	m_P = true;		if(q.n4=='X')	m_X = true;
	}
	if(m_P)	{	m_p=true;	m_s=true;	}
	if(m_X)	{	m_x=true;	m_s=true;	}
	if(m_p)	fprintf(fp, "\\providecommand{mglp}[3]{\\draw[#3] (#1-\\mgl1, #2) -- (#1+\\mgl1,#2) (#1,#2-\\mgl1) -- (#1,#2+\\mgl1);}\n");
	if(m_x)	fprintf(fp, "\\providecommand{mglx}[3]{\\draw[#3] (#1-\\mgl1, #2-\\mgl1) -- (#1+\\mgl1,#2+\\mgl1) (#1+\\mgl1,#2-\\mgl1) -- (#1-\\mgl1,#2+\\mgl1);}\n");
	if(m_s)	fprintf(fp, "\\providecommand{mgls}[3]{\\draw[#3] (#1-\\mgl1, #2-\\mgl1) -- (#1+\\mgl1,#2-\\mgl1) -- (#1+\\mgl1,#2+\\mgl1) -- (#1-\\mgl1,#2+\\mgl1) -- cycle;}\n");
	if(m_S)	fprintf(fp, "\\providecommand{mglS}[3]{\\fill[#3] (#1-\\mgl1, #2-\\mgl1) -- (#1+\\mgl1,#2-\\mgl1) -- (#1+\\mgl1,#2+\\mgl1) -- (#1-\\mgl1,#2+\\mgl1) -- cycle;}\n");
	if(m_d)	fprintf(fp, "\\providecommand{mgld}[3]{\\draw[#3] (#1, #2-\\mgl1) -- (#1+\\mgl1,#2) -- (#1,#2+\\mgl1) -- (#1-\\mgl1,#2) -- cycle;}\n");
	if(m_D)	fprintf(fp, "\\providecommand{mglD}[3]{\\fill[#3] (#1, #2-\\mgl1) -- (#1+\\mgl1,#2) -- (#1,#2+\\mgl1) -- (#1-\\mgl1,#2) -- cycle;}\n");
	if(m_v)	fprintf(fp, "\\providecommand{mglv}[3]{\\draw[#3] (#1-\\mgl1, #2+\\mgl2) -- (#1+\\mgl1,#2+\\mgl2) -- (#1,#2-\\mgl1) -- cycle;}\n");
	if(m_V)	fprintf(fp, "\\providecommand{mglV}[3]{\\fill[#3] (#1-\\mgl1, #2+\\mgl2) -- (#1+\\mgl1,#2+\\mgl2) -- (#1,#2-\\mgl1) -- cycle;}\n");
	if(m_t)	fprintf(fp, "\\providecommand{mglt}[3]{\\draw[#3] (#1-\\mgl1, #2-\\mgl2) -- (#1+\\mgl1,#2-\\mgl2) -- (#1,#2+\\mgl1) -- cycle;}\n");
	if(m_T)	fprintf(fp, "\\providecommand{mglT}[3]{\\fill[#3] (#1-\\mgl1, #2-\\mgl2) -- (#1+\\mgl1,#2-\\mgl2) -- (#1,#2+\\mgl1) -- cycle;}\n");
	if(m_l)	fprintf(fp, "\\providecommand{mgll}[3]{\\draw[#3] (#1+\\mgl2, #2-\\mgl1) -- (#1+\\mgl2,#2+\\mgl1) -- (#1-\\mgl1,#2) -- cycle;}\n");
	if(m_L)	fprintf(fp, "\\providecommand{mglL}[3]{\\fill[#3] (#1+\\mgl2, #2-\\mgl1) -- (#1+\\mgl2,#2+\\mgl1) -- (#1-\\mgl1,#2) -- cycle;}\n");
	if(m_r)	fprintf(fp, "\\providecommand{mglr}[3]{\\draw[#3] (#1-\\mgl2, #2-\\mgl1) -- (#1-\\mgl2,#2+\\mgl1) -- (#1+\\mgl1,#2) -- cycle;}\n");
	if(m_R)	fprintf(fp, "\\providecommand{mglR}[3]{\\fill[#3] (#1-\\mgl2, #2-\\mgl1) -- (#1-\\mgl2,#2+\\mgl1) -- (#1+\\mgl1,#2) -- cycle;}\n");
	if(m_Y)	fprintf(fp, "\\providecommand{mglR}[3]{\\draw[#3] (#1, #2-\\mgl1) -- (#1,#2) -- (#1-\\mgl1,#2+\\mgl1) (#1,#2) -- (#1+\\mgl1,#2+\\mgl1);}\n");
	if(m_a)	fprintf(fp, "\\providecommand{mgla}[3]{\\draw[#3] (#1-\\mgl1, #2) -- (#1+\\mgl1,#2) (#1-0.6*\\mgl1,#2-0.8*\\mgl1) -- (#1+0.6*\\mgl1,#2+0.8*\\mgl1) (#1-0.6*\\mgl1,#2+0.8*\\mgl1) -- (#1+0.6*\\mgl1,#2-0.8*\\mgl1);}\n");
	if(m_o)	fprintf(fp, "\\providecommand{mglo}[3]{\\draw[#3] (#1, #2) circle (\\mgl1);}\n");
	if(m_O)	fprintf(fp, "\\providecommand{mglO}[3]{\\fill[#3] (#1, #2) circle (\\mgl1);}\n");
	// dots command is provided in any case
	fprintf(fp, "\\providecommand{mglc}[3]{\\draw[#3] (#1, #2) circle (0.1*\\mgl1);}\n\n");

	for(i=0;i<gr->GetPrmNum();i++)
	{
		const mglPrim &q = gr->GetPrm(i);
		if(q.type<0)	continue;	// q.n1>=0 always
		cp = _Gr_->GetColor(q);	cname = mglColorName(cp);
		const mglPnt p1=gr->GetPnt(q.n1);
		float x=p1.x,y=p1.y,s=0.4*gr->FontFactor()*q.s;
		if(q.type==0)
		{
			if(!strchr("xsSoO",q.n4))	s *= 1.1;
			wp = 1;
			switch(q.n4)	// TODO up to here !!!!!!!!!!!!!!!!!!!!
			{
				case 'P':
					fprintf(fp, "\\mglp{%g}{%g}{%s} \\mgls{%g}{%g}{%s}\n", x,y,cname,x,y,cname);	break;
				case 'X':
					fprintf(fp, "\\mglx{%g}{%g}{%s} \\mgls{%g}{%g}{%s}\n", x,y,cname,x,y,cname);	break;
				case 'C':
					fprintf(fp, "\\mglc{%g}{%g}{%s} \\mglo{%g}{%g}{%s}\n", x,y,cname,x,y,cname);	break;
				case '+':	fprintf(fp, "\\mglp{%g}{%g}{%s}\n", x,y,cname);	break;
				case 'x':	fprintf(fp, "\\mglx{%g}{%g}{%s}\n", x,y,cname);	break;
				case 's':	fprintf(fp, "\\mgls{%g}{%g}{%s}\n", x,y,cname);	break;
				case 'S':	fprintf(fp, "\\mglS{%g}{%g}{%s}\n", x,y,cname);	break;
				case 'd':	fprintf(fp, "\\mgld{%g}{%g}{%s}\n", x,y,cname);	break;
				case 'D':	fprintf(fp, "\\mglD{%g}{%g}{%s}\n", x,y,cname);	break;
				case '^':	fprintf(fp, "\\mglt{%g}{%g}{%s}\n", x,y,cname);	break;
				case 'T':	fprintf(fp, "\\mglT{%g}{%g}{%s}\n", x,y,cname);	break;
				case 'v':	fprintf(fp, "\\mglv{%g}{%g}{%s}\n", x,y,cname);	break;
				case 'V':	fprintf(fp, "\\mglV{%g}{%g}{%s}\n", x,y,cname);	break;
				case '<':	fprintf(fp, "\\mgll{%g}{%g}{%s}\n", x,y,cname);	break;
				case 'L':	fprintf(fp, "\\mglL{%g}{%g}{%s}\n", x,y,cname);	break;
				case '>':	fprintf(fp, "\\mglr{%g}{%g}{%s}\n", x,y,cname);	break;
				case 'R':	fprintf(fp, "\\mglR{%g}{%g}{%s}\n", x,y,cname);	break;
				case 'Y':	fprintf(fp, "\\mglY{%g}{%g}{%s}\n", x,y,cname);	break;
				case 'o':	fprintf(fp, "\\mglo{%g}{%g}{%s}\n", x,y,cname);	break;
				case 'O':	fprintf(fp, "\\mglO{%g}{%g}{%s}\n", x,y,cname);	break;
				case '*':	fprintf(fp, "\\mgla{%g}{%g}{%s}\n", x,y,cname);	break;
				default:	fprintf(fp, "\\mglc{%g}{%g}{%s}\n", x,y,cname);	break;
			}
		}
		else if(q.type==2 && cp.a>0)
		{
			const mglPnt p2=gr->GetPnt(q.n2), p3=gr->GetPnt(q.n3);
			fprintf(fp, "\\fill[%s, fill opacity=%g] (%g,%g) -- (%g,%g) -- (%g,%g) -- cycle;\n", cname,cp.a, x,y, p2.x,p2.y, p3.x,p3.y);
		}
		else if(q.type==3 && cp.a>0)
		{
			const mglPnt p2=gr->GetPnt(q.n2), p3=gr->GetPnt(q.n3), p4=gr->GetPnt(q.n4);
			fprintf(fp, "\\fill[%s, fill opacity=%g] (%g,%g) -- (%g,%g) -- (%g,%g) -- (%g,%g) -- cycle;\n", cname,cp.a, x,y, p2.x,p2.y, p3.x,p3.y, p4.x,p4.y);
		}
		else if(q.type==1)	// lines
		{
			//			const char *dash[]={"", "8 8","4 4","1 3","7 4 1 4","3 2 1 2"};
			const char *w[]={"semithick","thick","very thick","ultra thick"};
			register int iw=int(q.w-0.5);	if(iw>3)	iw=3;
			if(iw<0)	fprintf(fp,"\\draw[%s] ",cname);
			else		fprintf(fp,"\\draw[%s,%s] ",cname,w[iw]);
			// TODO: add line dashing
			wp = q.w>1  ? q.w:1;	st = q.n3;
			put_line(gr,fp,false,i,wp,cp,st, "(%g,%g)", " -- (%g,%g)", false);
			fprintf(fp, ";\n");
		}
		else if(q.type==6)	// text
		{
			const mglText &t = gr->GetPtx(q.n3);
			float ftet = isnan(p1.v)||isnan(p1.u) ? 0:-180*atan2(p1.v,p1.u)/M_PI;
			int f,a;	mglGetStyle(t.stl.c_str(), &f, &a);
			std::string ss=cname;
			if((a&3)==2)	ss.append(",west");	if((a&3)==0)	ss.append(",east");
			if(f&MGL_FONT_ITAL)	ss.append(",font=\\itshape");
			if(f&MGL_FONT_BOLD)	ss.append(",font=\\bfshape");
			if(t.text.find('\\')!=std::string::npos)
				fprintf(fp,"\\draw[%s] (%g,%g) node[rotate=%.2g] $%ls$;\n", ss.c_str(),x,y, ftet, t.text.c_str());
			else
				fprintf(fp,"\\draw[%s] (%g,%g) node[rotate=%.2g] %ls;\n", ss.c_str(),x,y, ftet, t.text.c_str());
		}
	}
	fprintf(fp, "\\end{tikzpicture}\n");
	for(i=0;i<gr->GetPrmNum();i++)
	{	mglPrim &q=gr->GetPrm(i);	if(q.type==-1)	q.type = 1;	}
	fclose(fp);
}