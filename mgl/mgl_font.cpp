/* mgl_font.cpp is part of Math Graphic Library
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
//-----------------------------------------------------------------------------
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <stdio.h>
#include <ctype.h>
#include "mgl/mgl.h"
#include "mgl/mgl_font.h"
//-----------------------------------------------------------------------------
#ifndef MGL_FONT_PATH
#define MGL_FONT_PATH "."	// path to fonts
#endif
//-----------------------------------------------------------------------------
extern unsigned mgl_numg, mgl_cur;
extern float mgl_fact;
extern long mgl_gen_fnt[516][6];
extern short mgl_buf_fnt[246080];
const float mgl_fgen = 4*14;
extern mglTeXsymb mgl_tex_symb[];
//-----------------------------------------------------------------------------
void mglFont::Puts(const char *str,const char *how)
{
	int font=0, align=0;
	if(how)
	{
		if(strchr(how,'R'))	align = 2;
		if(strchr(how,'C'))	align = 1;
		if(strchr(how,'D'))	align+= 4;
		if(strchr(how,'b'))	font = font|MGL_FONT_BOLD;
		if(strchr(how,'i'))	font = font|MGL_FONT_ITAL;
		if(strchr(how,'w'))	font = font|MGL_FONT_WIRE;
		if(strchr(how,'o'))	font = font|MGL_FONT_OLINE;
		if(strchr(how,'u'))	font = font|MGL_FONT_ULINE;
	}
	Puts(str, font, align);
}
//-----------------------------------------------------------------------------
float mglFont::Width(const char *str,const char *how)
{
	int font=0;
	if(how)
	{
		if(strchr(how,'b'))	font = font|MGL_FONT_BOLD;
		if(strchr(how,'i'))	font = font|MGL_FONT_ITAL;
		if(strchr(how,'w'))	font = font|MGL_FONT_WIRE;
		if(strchr(how,'o'))	font = font|MGL_FONT_OLINE;
		if(strchr(how,'u'))	font = font|MGL_FONT_ULINE;
	}
	return Width(str, font);
}
//-----------------------------------------------------------------------------
void mglFont::Puts(const wchar_t *str,const char *how)
{
	int font=0, align=0;
	if(how)
	{
		if(strchr(how,'R'))	align = 2;
		if(strchr(how,'C'))	align = 1;
		if(strchr(how,'D'))	align+= 4;
		if(strchr(how,'b'))	font = font|MGL_FONT_BOLD;
		if(strchr(how,'i'))	font = font|MGL_FONT_ITAL;
		if(strchr(how,'w'))	font = font|MGL_FONT_WIRE;
		if(strchr(how,'o'))	font = font|MGL_FONT_OLINE;
		if(strchr(how,'u'))	font = font|MGL_FONT_ULINE;
	}
	Puts(str, font, align);
}
//-----------------------------------------------------------------------------
float mglFont::Width(const wchar_t *str,const char *how)
{
	int font=0;
	if(how)
	{
		if(strchr(how,'b'))	font = font|MGL_FONT_BOLD;
		if(strchr(how,'i'))	font = font|MGL_FONT_ITAL;
		if(strchr(how,'w'))	font = font|MGL_FONT_WIRE;
		if(strchr(how,'o'))	font = font|MGL_FONT_OLINE;
		if(strchr(how,'u'))	font = font|MGL_FONT_ULINE;
	}
	return Width(str, font);
}
//-----------------------------------------------------------------------------
void mglFont::Puts(const char *str,int font,int align)
{
	unsigned size = strlen(str)+1;
	wchar_t *wcs = new wchar_t[size];
	mbstowcs(wcs,str,size);
	Puts(wcs,font,align);
	delete []wcs;
}
//-----------------------------------------------------------------------------
float mglFont::Width(const char *str,int font)
{
	unsigned size = strlen(str)+1;
	wchar_t *wcs = new wchar_t[size];
	mbstowcs(wcs,str,size);
	float w = Width(wcs,font);
	delete []wcs;
	return w;
}
//-----------------------------------------------------------------------------
void mglFont::Puts(const wchar_t *str,int font,int align)
{
	if(numg==0)	return;
	float w=0,h = (align&4) ? 500./fact[0] : 0;
	unsigned size = wcslen(str)+1;
	if(parse)
	{
		unsigned *wcs = new unsigned[size];
		memcpy(wcs,str,size*sizeof(unsigned));
		Convert(str, wcs);
		w = Puts(wcs,0,0,1.f,0x10|font);	// find width
		Puts(wcs,-w*(align&3)/2.f,-h,1.f,font);	// draw it really
		delete []wcs;
	}
	else
	{
		int s = (font/MGL_FONT_BOLD)&3;
		long i,j;
		h *= fact[0]/fact[s];
		for(i=0;i<size;i++)		// find width
		{
			j = str[i]!=' ' ? Internal(str[i]) : Internal('!');
			if(j==-1)	continue;
			w+= width[s][j]/fact[s];
		}
		w *= -(align&3)/2.f;
		if(gr)	for(i=0;i<size;i++)		// draw it
		{
			if(str[i]!=' ')
			{
				j = Internal(str[i]);
				if(j==-1)	continue;
				if(font & MGL_FONT_WIRE)
					gr->Glyph(w, -h, 1.f/fact[s], 0, 0, numl[s][j], buf+ln[s][j]);
				else
					gr->Glyph(w, -h, 1.f/fact[s], numt[s][j], buf+tr[s][j], numl[s][j], buf+ln[s][j]);
			}
			else	j = 0;//Internal('!');
			w+= width[s][j]/fact[s];
		}
	}
}
//-----------------------------------------------------------------------------
float mglFont::Width(const wchar_t *str,int font)
{
	if(numg==0)	return 0;
	float w=0;
	unsigned size = wcslen(str)+1;
	if(parse)
	{
		unsigned *wcs = new unsigned[size];
		memcpy(wcs,str,size*sizeof(unsigned));
		Convert(str, wcs);
		w = Puts(wcs,0,0,1.,0x10+font);
		delete []wcs;
	}
	else
	{
		long i,j;
		int s = (font/MGL_FONT_BOLD)&3;
		for(i=0;i<size;i++)
		{
			j = str[i]!=' ' ? Internal(str[i]) : Internal('!');
			if(j==-1)	continue;
			w+= width[s][j]/fact[s];
		}
	}
	return w;
}
//-----------------------------------------------------------------------------
float mglFont::Height(int font)
{
	int s = (font/MGL_FONT_BOLD)&3;
	return (500.f)/fact[s];
}
//-----------------------------------------------------------------------------
float mglFont::Height(const char *how)
{
	int s=0;
	if(how)
	{
		if(strchr(how,'b'))	s = s|1;
		if(strchr(how,'i'))	s = s|2;
	}
	return (500.f)/fact[s];
}
//-----------------------------------------------------------------------------
long mglFont::Internal(unsigned s)
{
	register long i,i1=0,i2=numg-1;
	register wchar_t j = wchar_t(s & MGL_FONT_MASK);
	// let suppose that id[i]<id[i+1]
	while(i1<i2)
	{
		i = (i1+i2)/2;
//printf("j=%d -- %d[%d] %d[%d] %d[%d]\n",j,id[i1],i1,id[i],i,id[i2],i2);	fflush(stdout);
		if(j<id[i])			i2 = i;
		else if(j>id[i])	i1=i+1;	// i is bad
		else return i;
	}
//printf("not found\n");	fflush(stdout);
	return j==id[i2] ? i2 : -1;
}
//-----------------------------------------------------------------------------
/// Table of acents and its UTF8 codes
mglTeXsymb mgl_act_symb[] = {
	{0x02c6, L"hat"}, {0x02dc, L"tilde"}, {0x02d9, L"dot"}, {0x00a8, L"ddot"}, {0x20db, L"dddot"}, {0x20dc, L"ddddot"}, {0x02ca, L"acute"}, {0x02c7, L"check"}, {0x02cb, L"grave"}, {0x20d7, L"vec"}, {0x02c9, L"bar"}, {0x02d8, L"breve"},
	/*end*/{0, L"\0"}};
//-----------------------------------------------------------------------------
int mgl_tex_symb_cmp(const void *a, const void *b)
{
	const mglTeXsymb *aa = (const mglTeXsymb *)a;
	const mglTeXsymb *bb = (const mglTeXsymb *)b;
	return wcscmp(aa->tex, bb->tex);
}
//-----------------------------------------------------------------------------
// parse LaTeX commands (mostly symbols and acents, and some font-style commands)
unsigned mglFont::Parse(const wchar_t *s)
{
	register long k;
	unsigned res = unsigned(-2);		// Default is no symbol
	if(!s || !s[0])	return res;
	for(k=0;mgl_tex_symb[k].kod;k++);	// determine the number of symbols
	mglTeXsymb tst, *rts;
	tst.tex = s;
	rts = (mglTeXsymb *) bsearch(&tst, mgl_tex_symb, k, sizeof(mglTeXsymb), mgl_tex_symb_cmp);
	if(rts)	return rts->kod;
	
//	for(k=0;mgl_tex_symb[k].kod;k++)	// special symbols
//		if(!wcscmp(s,mgl_tex_symb[k].tex))
//			return mgl_tex_symb[k].kod;
	for(k=0;mgl_act_symb[k].kod;k++)	// acents
		if(!wcscmp(s,mgl_act_symb[k].tex))
			return mgl_act_symb[k].kod | MGL_FONT_ZEROW;
	// arbitrary UTF symbol
	if(s[0]=='u' && s[1]=='t' && s[2]=='f')
	{	wscanf(s+3,"%lx",&k);	return wchar_t(k);	}
	// font/style changes for next symbol
	if(!wcscmp(s,L"big"))		res = unsigned(-5);
	else if(!wcscmp(s,L"b"))	res = MGL_FONT_BOLD;
	else if(!wcscmp(s,L"i"))	res = MGL_FONT_ITAL;
	else if(!wcscmp(s,L"bi"))	res = MGL_FONT_BOLD|MGL_FONT_ITAL;
	else if(!wcscmp(s,L"r"))	res = unsigned(-1);
	else if(!wcscmp(s,L"a"))	res = MGL_FONT_OLINE;
	else if(!wcscmp(s,L"u"))	res = MGL_FONT_ULINE;
	else if(!wcscmp(s,L"overline"))		res = MGL_FONT_OLINE;
	else if(!wcscmp(s,L"underline"))	res = MGL_FONT_ULINE;
	else if(!wcscmp(s,L"textbf"))		res = MGL_FONT_BOLD;
	else if(!wcscmp(s,L"textit"))		res = MGL_FONT_ITAL;
	else if(!wcscmp(s,L"textrm"))		res = unsigned(-1);
	else if(!wcscmp(s,L"w"))			res = MGL_FONT_WIRE;
	else if(!wcscmp(s,L"wire"))			res = MGL_FONT_WIRE;
	return res;
}
//-----------------------------------------------------------------------------
void mglFont::Convert(const wchar_t *str, unsigned *res)
{
	register unsigned r,i,j,k,i0;
	wchar_t s[128], ch;		// TeX command and current char
	for(i=j=0;str[i];i++)
	{
		ch = str[i];
		if(ch=='\\')	// It can be TeX command
		{
			if(wcschr(L"{}_^\\@ ",str[i+1]))	// No, it is usual symbol
				res[j++] = str[++i];
			else		// Yes, it is TeX command
			{
				i0=i+1;
				for(k=0;isalnum(str[++i]) && k<127;k++)	s[k] = str[i];
				s[k] = 0;
				r = Parse(s);
				if(r==unsigned(-2))			// command not found, so use next symbol itself
				{	res[j++] = str[i0];	i = i0;	}
				else if(r)
				{
					res[j++] = r;
					if(str[i]>' ')	i--;
					if(str[i]==0)	break;
				}
			}
		}
		else if(ch<=' ')	res[j++] = ' ';	// no \t, no \n at this moment :(
		else if(ch=='_')	res[j++] = MGL_FONT_LOWER;
		else if(ch=='^')	res[j++] = MGL_FONT_UPPER;
		else if(ch=='@')	res[j++] = MGL_FONT_UPPER|MGL_FONT_LOWER;
		else if(ch=='{')	res[j++] = unsigned(-3);
		else if(ch=='}')	res[j++] = unsigned(-4);
		else	res[j++] = ch;				// It is just symbol
	}
	res[j] = 0;
}
//-----------------------------------------------------------------------------
float mglFont::Puts(const unsigned *str, float x,float y,float f,int style)
{
	if(numg==0)	return 0;
	register long i,j,k;
	register unsigned s,ss;
	float w=0;				// string width
	int st = style;			// current style
	unsigned *b=0;			// buffer for substring
	float yy=y, ff=f, ww;
	int a = (st/MGL_FONT_BOLD)&3;
	for(i=0;str[i];i++)
	{
		s = str[i];		ww = 0;
		if(s==unsigned(-3))	// recursion call here
		{
			k=1;
			for(j=0;k>0 && str[i];j++)
			{
				i++;
				if(str[i]==unsigned(-4))	k--;
				if(str[i]==unsigned(-3))	k++;
			}
			b = new unsigned[j+1];
			memcpy(b,str+i-j+1,j*sizeof(unsigned));	b[j]=0;
			ww = Puts(b, x, yy, ff, (st&(~MGL_FONT_OLINE)&(~MGL_FONT_ULINE)));
			delete []b;
//			if(st & MGL_FONT_ZEROW)	ww = 0;
			if(gr && !(style&0x10))	// add under-/over- line now
			{
				if(st&MGL_FONT_OLINE)	gr->Glyph(x,y+499*f/fact[a],ww,0,0,-1,0);
				if(st&MGL_FONT_ULINE)	gr->Glyph(x,y-200*f/fact[a],ww,0,0,-1,0);
			}
			st = style;	yy = y;	ff = f;		// clear style modification
		}
		else if(s==unsigned(-4))	// should be never here but if I miss sth ...
			continue;
		else if(s==unsigned(-5))	// large symbol
			ff *= 1.5;
		else if(s==unsigned(-1))	// set normal font
			st = st & MGL_FONT_ROMAN;
		else
		{
			ss = s&MGL_FONT_MASK;
			if(ss)	// draw symbol (glyph)
			{
				if(ss!=' ')
				{
					j = Internal(ss);
					if(j==-1)	continue;
					if(gr && !(style&0x10))
					{
						if(st & MGL_FONT_WIRE)
							gr->Glyph(x, yy, ff/fact[a], 0, 0, numl[a][j], buf+ln[a][j]);
						else
							gr->Glyph(x, yy, ff/fact[a], numt[a][j], buf+tr[a][j], numl[a][j], buf+ln[a][j]);
					}
				}
				else
					j = Internal('!');
				ww = ff*width[a][j]/fact[a];
				if(gr && !(style&0x10))	// add under-/over- line now
				{
					if(st&MGL_FONT_OLINE)	gr->Glyph(x,y+499*f/fact[a],ww,0,0,-1,0);
					if(st&MGL_FONT_ULINE)	gr->Glyph(x,y-200*f/fact[a],ww,0,0,-1,0);
				}
			if(s & MGL_FONT_ZEROW)	ww = 0;
			}
			st = style;	yy = y;	ff = f;		// clear style modification
			// apply new styles
			if(s/MGL_FONT_BOLD)	st = st | (s & MGL_FONT_STYLE);
			a = (st/MGL_FONT_BOLD)&3;
			ss = (s/MGL_FONT_UPPER)%4;
			if(ss)
			{
				if(ss==1)		yy += 200*ff/fact[a];	// =  500*0.4
				else if(ss==2)	yy -=  80*ff/fact[a];	// = -500*0.16
				else if(ss==3)	yy +=  60*ff/fact[a];	// =  500*0.12
				ff *=0.6;
			}
		}
		x += ww;	w += ww;
	}
	return w;
}
//-----------------------------------------------------------------------------
void mglFont::read_data(FILE *fp, float *ff, short *wdt, short *numl,
						unsigned *posl, short *numt, unsigned *post, unsigned &cur)
{
	char str[256];
	int tmpw, tmpnl, tmpnt;
	unsigned s,i,n, tmpi, tmppl, tmppt;
	long j;

	fgets(str,256,fp);				// first string is comment (not used)
	fgets(str,256,fp);				// second string have information
	sscanf(str, "%u%f%d", &n, ff, &s);
	buf = (short *)realloc(buf, (cur+s)*sizeof(short));	// prealocate buffer

	for(i=0;i<n;i++)
	{
		fscanf(fp,"%u%d%d%u%d%u", &tmpi, &tmpw, &tmpnl, &tmppl, &tmpnt, &tmppt);
		j=Internal(unsigned(tmpi));	if(j<0)	continue;
		if(wdt)	wdt[j] = tmpw;
		numl[j] = tmpnl;	posl[j] = tmppl+cur;
		numt[j] = tmpnt;	post[j] = tmppt+cur;
//printf("%u - %d %d %d %u %d %u\n",j, tmpi,tmpw,tmpnl, tmppl, tmpnt, tmppt);	fflush(stdout);
	}
	for(i=0;i<s;i++)	{	fscanf(fp,"%d", &tmpi);	buf[i+cur] = tmpi;	}
	cur += s;
	fclose(fp);	// !!! close file here !!!
}
//-----------------------------------------------------------------------------
void mglFont::mem_alloc()
{
	id = new wchar_t[numg];
	width[0] = new short[numg];	width[1] = new short[numg];
	width[2] = new short[numg];	width[3] = new short[numg];
	tr[0] = new unsigned[numg];	numt[0] = new short[numg];
	tr[1] = new unsigned[numg];	numt[1] = new short[numg];
	tr[2] = new unsigned[numg];	numt[2] = new short[numg];
	tr[3] = new unsigned[numg];	numt[3] = new short[numg];
	ln[0] = new unsigned[numg];	numl[0] = new short[numg];
	ln[1] = new unsigned[numg];	numl[1] = new short[numg];
	ln[2] = new unsigned[numg];	numl[2] = new short[numg];
	ln[3] = new unsigned[numg];	numl[3] = new short[numg];
}
//-----------------------------------------------------------------------------
// copy normal style as default for other styles
void mglFont::main_copy()
{
	memcpy(numl[1],numl[0],numg*sizeof(short));
	memcpy(numl[2],numl[0],numg*sizeof(short));
	memcpy(numl[3],numl[0],numg*sizeof(short));
	memcpy(ln[1],ln[0],numg*sizeof(unsigned));
	memcpy(ln[2],ln[0],numg*sizeof(unsigned));
	memcpy(ln[3],ln[0],numg*sizeof(unsigned));
	memcpy(numt[1],numt[0],numg*sizeof(short));
	memcpy(numt[2],numt[0],numg*sizeof(short));
	memcpy(numt[3],numt[0],numg*sizeof(short));
	memcpy(tr[1],tr[0],numg*sizeof(unsigned));
	memcpy(tr[2],tr[0],numg*sizeof(unsigned));
	memcpy(tr[3],tr[0],numg*sizeof(unsigned));
	memcpy(width[1],width[0],numg*sizeof(short));
	memcpy(width[2],width[0],numg*sizeof(short));
	memcpy(width[3],width[0],numg*sizeof(short));
}
//-----------------------------------------------------------------------------
bool mglFont::read_main(const char *base, const char *path, unsigned &cur)
{
	FILE *fp;
	char str[256];
	int tmpi, tmpw, tmpnl, tmpnt;
	unsigned s, tmppl, tmppt;
	sprintf(str,"%s/%s.vfm",path,base);	// normal weight (should have most of symbols !!!)
	fp = fopen(str,"r");
	if(!fp)	return false;			// this font must be in any case
	fgets(str,256,fp);				// first string is comment (not used)
	fgets(str,256,fp);				// second string have information
	sscanf(str, "%u%f%u", &numg, fact, &s);
	fact[1] = fact[2] = fact[3] = fact[0];	// copy default factor for other font styles;
	buf = (short *)malloc(s*sizeof(short));	// prealocate buffer
	// now allocate memory for all fonts
	mem_alloc();
	// and load symbols itself
	register long i;
	for(i=0;i<numg;i++)
	{
		fscanf(fp,"%u%d%d%u%d%u", &tmpi, &tmpw, &tmpnl, &tmppl, &tmpnt, &tmppt);
		id[i] = tmpi;		width[0][i] = tmpw;
		numl[0][i] = tmpnl; ln[0][i] = tmppl;
		numt[0][i] = tmpnt;	tr[0][i] = tmppt;
	}
	for(i=0;i<s;i++)	{	fscanf(fp,"%d", &tmpi);	buf[i] = tmpi;	}
	cur += s;
	fclose(fp);		// finish wire normal font
	main_copy();	// copy normal style as default for other styles
	return true;
}
//-----------------------------------------------------------------------------
bool mglFont::read_def(unsigned &cur)
{
	numg = mgl_numg;	cur = mgl_cur;
	// copy default factor for other font styles;
	fact[1] = fact[2] = fact[3] = fact[0] = mgl_fact*mgl_fgen;
	buf = (short *)malloc(cur*sizeof(short));	// prealocate buffer
	// now allocate memory for all fonts
	mem_alloc();
	// and load symbols itself
	register long i;
	for(i=0;i<numg;i++)
	{
		id[i] = mgl_gen_fnt[i][0];
		width[0][i] = mgl_gen_fnt[i][1];
		numl[0][i] = mgl_gen_fnt[i][2];
		ln[0][i] = mgl_gen_fnt[i][3];
		numt[0][i] = mgl_gen_fnt[i][4];
		tr[0][i] = mgl_gen_fnt[i][5];
	}
	memcpy(buf, mgl_buf_fnt, cur*sizeof(short));
	main_copy();	// copy normal style as default for other styles
	return true;
}
//-----------------------------------------------------------------------------
bool mglFont::Load(const char *base, const char *path)
{
	FILE *fp;
	char str[256];
	setlocale(LC_NUMERIC,"C");
	short *tmpi, *tmpw, *tmpn;
	unsigned s,i,j,cur=0, *tmpp;
	float ff;
	if(!path)	path = MGL_FONT_PATH;
	Clear();							// first clear old

	if(!base)	{	read_def(cur);	return true;	}
	else	if(!read_main(base,path,cur))	return false;

	//================== bold ===========================================
	sprintf(str,"%s/%s_b.vfm",path,base);
	fp = fopen(str,"rt");
	if(fp)		// this file may absent
	{
		read_data(fp, fact+1, width[1], numl[1], ln[1], numt[1], tr[1], cur);
		fact[3] = fact[1];		// copy default factor for bold-italic;
		// copy normal style as default for other styles
		memcpy(numl[3],numl[1],numg*sizeof(short));
		memcpy(ln[3],ln[1],numg*sizeof(unsigned));
		memcpy(numt[3],numt[1],numg*sizeof(short));
		memcpy(tr[3],tr[1],numg*sizeof(unsigned));
		memcpy(width[3],width[1],numg*sizeof(short));
	}

	//================== italic =========================================
	sprintf(str,"%s/%s_i.vfm",path,base);
	fp = fopen(str,"rt");
	if(fp)	read_data(fp, fact+2, width[2], numl[2], ln[2], numt[2], tr[2], cur);

	//================== bold-italic ====================================
	sprintf(str,"%s/%s_bi.vfm",path,base);
	fp = fopen(str,"rt");
	if(fp)	read_data(fp, fact+3, width[3], numl[3], ln[3], numt[3], tr[3], cur);
	numb = cur;

	// Finally normalize all factors
	fact[0] *= mgl_fgen;	fact[1] *= mgl_fgen;
	fact[2] *= mgl_fgen;	fact[3] *= mgl_fgen;
	return true;
}
//-----------------------------------------------------------------------------
mglFont::mglFont(const char *name, const char *path)
{	parse = true;	numg=0;	Load(name, path);	}
mglFont::~mglFont()	{	Clear();	}
//-----------------------------------------------------------------------------
void mglFont::Clear()
{
	if(numg)
	{
		delete []id;		free(buf);			numg = 0;
		delete []width[0];	delete []width[1];	delete []width[2];	delete []width[3];
		delete []tr[0];		delete []tr[1];		delete []tr[2];		delete []tr[3];
		delete []ln[0];		delete []ln[1];		delete []ln[2];		delete []ln[3];
		delete []numt[0];	delete []numt[1];	delete []numt[2];	delete []numt[3];
		delete []numl[0];	delete []numl[1];	delete []numl[2];	delete []numl[3];
	}
}
//-----------------------------------------------------------------------------
void mglFont::Copy(mglFont *f)
{
	if(!f || f==this)	return;
	Clear();
	numg = f->numg;		numb = f->numb;
	mem_alloc();
	// copy general data
	memcpy(id,f->id,numg*sizeof(wchar_t));
	memcpy(width[0],f->width[0],numg*sizeof(short));
	memcpy(width[1],f->width[1],numg*sizeof(short));
	memcpy(width[2],f->width[2],numg*sizeof(short));
	memcpy(width[3],f->width[3],numg*sizeof(short));
	memcpy(tr[0],f->tr[0],numg*sizeof(unsigned));
	memcpy(tr[1],f->tr[1],numg*sizeof(unsigned));
	memcpy(tr[2],f->tr[2],numg*sizeof(unsigned));
	memcpy(tr[3],f->tr[3],numg*sizeof(unsigned));
	memcpy(numt[0],f->numt[0],numg*sizeof(short));
	memcpy(numt[1],f->numt[1],numg*sizeof(short));
	memcpy(numt[2],f->numt[2],numg*sizeof(short));
	memcpy(numt[3],f->numt[3],numg*sizeof(short));
	memcpy(ln[0],f->ln[0],numg*sizeof(unsigned));
	memcpy(ln[1],f->ln[1],numg*sizeof(unsigned));
	memcpy(ln[2],f->ln[2],numg*sizeof(unsigned));
	memcpy(ln[3],f->ln[3],numg*sizeof(unsigned));
	memcpy(numl[0],f->numl[0],numg*sizeof(short));
	memcpy(numl[1],f->numl[1],numg*sizeof(short));
	memcpy(numl[2],f->numl[2],numg*sizeof(short));
	memcpy(numl[3],f->numl[3],numg*sizeof(short));
	memcpy(fact,f->fact,4*sizeof(float));
	// now copy symbols descriptions
	buf = (short *)malloc(numb*sizeof(short));
	memcpy(buf, f->buf, numb*sizeof(short));
}
//-----------------------------------------------------------------------------
