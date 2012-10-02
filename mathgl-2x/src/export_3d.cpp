/***************************************************************************
 * export_3d.cpp is part of Math Graphic Library
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
#include "mgl2/canvas.h"
#include "mgl2/canvas_cf.h"
#undef _GR_
#define _GR_	((mglCanvas *)(*gr))
#define _Gr_	((mglCanvas *)(gr))
int mgl_tga_save(const char *fname, int w, int h, unsigned char **p);
int mgl_pnga_save(const char *fname, int w, int h, unsigned char **p);
void mgl_printf(void *fp, bool gz, const char *str, ...);
//-----------------------------------------------------------------------------
void mglTexture::GetRGBA(unsigned char *f) const
{
	register long i,j,i0;
	mglColor c1,c2,c;
	for(i=255;i>=0;i--)
	{
		c1 = col[2*i];	c2 = col[2*i+1];
		for(j=0;j<256;j++)
		{
			i0 = 4*(j+256*i);
			c = c1 + (c2-c1)*(j/255.);
			f[i0]   = int(255*c.r);
			f[i0+1] = int(255*c.g);
			f[i0+2] = int(255*c.b);
			f[i0+3] = int(255*c.a);
		}
	}
}
//-----------------------------------------------------------------------------
/* M.Vidassov take/move it into src/obj.cpp
void mgl_obj_prim(const mglPrim &q, const mglPnt &p, FILE *fp, mreal size)
{
	char type = q.n4;	mreal ss=size*0.35;
	register long i=q.n1+1,j;
	register long n1=q.n1+1,n2=q.n2+1,n3=q.n3+1,n4=q.n4+1;
	switch(q.type)
	{
		case 0:
			if(!strchr("xsSoO",type))	ss *= 1.1;
			if(type=='.' || ss==0)	fprintf(fp,"p %ld\n", i);
			else	switch(type)
			{
			case 'P':
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y-ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y-ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y+ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y+ss,p.z);
				fprintf(fp,"l -4/%ld -3/%ld\n", i,i);
				fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"l -1/%ld -4/%ld\n", i,i);
			case '+':
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y,p.z);
				fprintf(fp,"v %g %g %g\n",p.x,p.y-ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x,p.y+ss,p.z);
				fprintf(fp,"l -4/%ld -3/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);	break;
			case 'X':
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y-ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y-ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y+ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y+ss,p.z);
				fprintf(fp,"l -4/%ld -3/%ld\n", i,i);
				fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"l -1/%ld -4/%ld\n", i,i);
				fprintf(fp,"l -1/%ld -3/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -4/%ld\n", i,i);	break;
			case 'x':
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y-ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y-ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y+ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y+ss,p.z);
				fprintf(fp,"l -1/%ld -3/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -4/%ld\n", i,i);	break;
			case 'S':
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y-ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y-ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y+ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y+ss,p.z);
				fprintf(fp,"f -4/%ld -3/%ld -2/%ld -1/%ld\n",i,i,i,i);	break;
			case 's':
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y-ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y-ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y+ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y+ss,p.z);
				fprintf(fp,"l -4/%ld -3/%ld\n", i,i);
				fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"l -1/%ld -4/%ld\n", i,i);	break;
			case 'D':
				fprintf(fp,"v %g %g %g\n",p.x,p.y-ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y,p.z);
				fprintf(fp,"v %g %g %g\n",p.x,p.y+ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y,p.z);
				fprintf(fp,"f -4/%ld -3/%ld -2/%ld -1/%ld\n",i,i,i,i);	break;
			case 'd':
				fprintf(fp,"v %g %g %g\n",p.x,p.y-ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y,p.z);
				fprintf(fp,"v %g %g %g\n",p.x,p.y+ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y,p.z);
				fprintf(fp,"l -4/%ld -3/%ld\n", i,i);
				fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"l -1/%ld -4/%ld\n", i,i);	break;
			case 'Y':
				fprintf(fp,"v %g %g %g\n",p.x,p.y-ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+0.8*ss,p.y+0.6*ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x-0.8*ss,p.y+0.6*ss,p.z);
				fprintf(fp,"l -3/%ld %ld/%ld\n", i,i,i);
				fprintf(fp,"l -2/%ld %ld/%ld\n", i,i,i);
				fprintf(fp,"l -1/%ld %ld/%ld\n", i,i,i);	break;
			case '*':
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y,p.z);
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y,p.z);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"v %g %g %g\n",p.x+0.6*ss,p.y+0.8*ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x-0.6*ss,p.y-0.8*ss,p.z);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"v %g %g %g\n",p.x+0.6*ss,p.y-0.8*ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x-0.6*ss,p.y+0.8*ss,p.z);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);		break;
			case 'T':
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y-ss/2,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y-ss/2,p.z);
				fprintf(fp,"v %g %g %g\n",p.x,p.y+ss,p.z);
				fprintf(fp,"f -3/%ld -2/%ld -1/%ld\n", i,i,i);	break;
			case '^':
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y-ss/2,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y-ss/2,p.z);
				fprintf(fp,"v %g %g %g\n",p.x,p.y+ss,p.z);
				fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"l -1/%ld -3/%ld\n", i,i);	break;
			case 'V':
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y+ss/2,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y+ss/2,p.z);
				fprintf(fp,"v %g %g %g\n",p.x,p.y-ss,p.z);
				fprintf(fp,"f -3/%ld -2/%ld -1/%ld\n", i,i,i);	break;
			case 'v':
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y+ss/2,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y+ss/2,p.z);
				fprintf(fp,"v %g %g %g\n",p.x,p.y-ss,p.z);
				fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"l -1/%ld -3/%ld\n", i,i);	break;
			case 'L':
				fprintf(fp,"v %g %g %g\n",p.x+ss/2,p.y+ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss/2,p.y-ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y,p.z);
				fprintf(fp,"f -3/%ld -2/%ld -1/%ld\n", i,i,i);	break;
			case '<':
				fprintf(fp,"v %g %g %g\n",p.x+ss/2,p.y+ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss/2,p.y-ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x-ss,p.y,p.z);
				fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"l -1/%ld -3/%ld\n", i,i);	break;
			case 'R':
				fprintf(fp,"v %g %g %g\n",p.x-ss/2,p.y+ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x-ss/2,p.y-ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y,p.z);
				fprintf(fp,"f -3/%ld -2/%ld -1/%ld\n", i,i,i);	break;
			case '>':
				fprintf(fp,"v %g %g %g\n",p.x-ss/2,p.y+ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x-ss/2,p.y-ss,p.z);
				fprintf(fp,"v %g %g %g\n",p.x+ss,p.y,p.z);
				fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"l -1/%ld -3/%ld\n", i,i);	break;
			case 'O':
				for(j=0;j<=20;j++)
					fprintf(fp,"v %g %g %g\n",p.x+ss*cos(j*M_PI/10),p.y+ss*sin(j*M_PI/10),p.z);
				for(j=0;j<20;j++)
					fprintf(fp,"f %ld/%ld %ld/%ld %ld/%ld\n", j-21,i, j-20,i, i,i);
				break;
			case 'C':	fprintf(fp,"p %ld\n", i);
			case 'o':
				for(j=0;j<=20;j++)
					fprintf(fp,"v %g %g %g\n",p.x+ss*cos(j*M_PI/10),p.y+ss*sin(j*M_PI/10),p.z);
				for(j=0;j<20;j++)
					fprintf(fp,"l %ld/%ld %ld/%ld\n", j-21,i, j-20,i);
				break;
			}
			break;
		case 1:	fprintf(fp,"l %ld/%ld %ld/%ld\n", n1,n1, n2,n2);	break;
		case 2:
			fprintf(fp,"f %ld/%ld %ld/%ld %ld/%ld\n", n1,n1, n2,n2, n3,n3);	break;
		case 3:
			fprintf(fp,"f %ld/%ld %ld/%ld %ld/%ld\n", n1,n1, n2,n2, n3,n3);
			fprintf(fp,"f %ld/%ld %ld/%ld %ld/%ld\n", n2,n2, n4,n4, n3,n3);	break;
		case 4:	break;	// TODO: add glyphs export later
	}
}
//-----------------------------------------------------------------------------
void mgl_write_obj(HMGL gr, const char *fname,const char *descr, int use_png)
{
	if(gr->GetPrmNum()==0)	return;	// nothing to do
	register size_t i,j;
	long m1=0,m2=0,m;
	for(i=0;i<gr->Grp.size();i++)	// prepare array of indirect indexing
	{	m = gr->Grp[i].Id;	if(m<m1) m1=m;	if(m>m2) m2=m;	}
	long *ng = new long[m2-m1+1];
	for(i=0;i<gr->Grp.size();i++)	ng[gr->Grp[i].Id-m1] = i;
	for(i=0;i<size_t(gr->GetPrmNum());i++)	// collect data for groups
	// it is rather expensive (extra 4b per primitive) but need for export to 3D
	{
		m = gr->GetPrm(i).id-m1;
		if(m>=0 && m<m2-m1+1)	gr->Grp[ng[m]].p.push_back(i);
	}
	delete []ng;

	size_t len=strlen(fname),ntxt=gr->GetTxtNum();
	char *tname = new char[len+5];	strcpy(tname,fname);
	FILE *fp=fopen(fname,"wt");
	// vertices definition
	fprintf(fp,"# Created by MathGL library\n# Title: %s\n",(descr && *descr) ? descr : fname);
	for(i=0;i<size_t(gr->GetPntNum());i++)
	{
		const mglPnt &pp = gr->GetPnt(i);
		fprintf(fp,"v %g %g %g\n",pp.x,pp.y,pp.z);
		fprintf(fp,"vt %g %g\n",1-pp.ta,pp.c/ntxt);
//		if(mgl_isnan(pp.u))	fprintf(fp,"vn 0 0 0\n");
//		else fprintf(fp,"vn %g %g %g\n",pp.u,pp.v,pp.w);
	}
	// primitive definition in groups
	tname[len-4]=0;	fprintf(fp,"# Primitives Definitions\nmtllib %s.mtl\nusemtl %s\n",tname,tname);
	for(i=0;i<gr->Grp.size();i++)
	{
		fprintf(fp,"g %s\n",gr->Grp[i].Lbl.c_str());
		std::vector<long> &p = gr->Grp[i].p;
		for(j=0;j<p.size();j++)
		{
			const mglPrim &q=gr->GetPrm(p[j]);
			mgl_obj_prim(q, gr->GetPnt(q.n1), fp, q.s*gr->FontFactor());
		}
		gr->Grp[i].p.clear();	// we don't need indexes anymore
	}
	// try to save "ungrouped" primitives
	fclose(fp);
	// prepare MTL file
	tname[len-4]='.';	tname[len-3]='m';	tname[len-2]='t';	tname[len-1]='l';
	fp=fopen(tname,"wt");
	tname[len-4]=0;	fprintf(fp,"newmtl %s\n",tname);
	fprintf(fp,"Ka 1.000 1.000 1.000\n");
	fprintf(fp,"Kd 1.000 1.000 1.000\n");
	fprintf(fp,"Ks 0.000 0.000 0.000\n");
	fprintf(fp,"d 1.0\nillum 2\n");
	if(use_png)	strcat(tname,"_txt.png");
//	{	tname[len-4]='.';	tname[len-3]='p';	tname[len-2]='n';	tname[len-1]='g';	}
	else		strcat(tname,"_txt.tga");
//	{	tname[len-4]='.';	tname[len-3]='t';	tname[len-2]='g';	tname[len-1]='a';	}
	fprintf(fp,"map_Ka %s\nmap_Kd %s\nmap_Ks %s\n",tname,tname,tname);
	fclose(fp);
	// prepare texture file (TGA or PNG)
	j=gr->GetTxtNum();
	unsigned char *buf = new unsigned char[4*256*256*j];
	unsigned char **pbuf= (unsigned char **)malloc(256*j*sizeof(unsigned char *));
	for(i=0;i<256*j;i++)	pbuf[i] = buf+4*256*i;
	for(i=0;i<j;i++)	gr->GetTxt(i).GetRGBA(buf+i*256*256*4);
	if(use_png)	mgl_pnga_save(tname,256,256*j,pbuf);
	else		mgl_tga_save(tname,256,256*j,pbuf);
	free(pbuf);	delete []buf;	delete []tname;
}
void mgl_write_obj_(uintptr_t *gr, const char *fname,const char *descr, int *use_png,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *d=new char[n+1];	memcpy(d,descr,n);	d[n]=0;
	mgl_write_obj(_GR_,s,d,*use_png);	delete []s;		delete []d;	}*/
//-----------------------------------------------------------------------------
void mgl_write_stl(HMGL gr, const char *fname,const char *descr)
{
	if(gr->GetPrmNum()==0)	return;	// nothing to do
	FILE *fp = fopen(fname,"wt");
	fprintf(fp,"solid %s\n",(descr && *descr)?descr:"mathgl");
	register long i;
	mglPnt pp;
	for(i=0;i<gr->GetPrmNum();i++)
	{
		const mglPrim &q=gr->GetPrm(i);
		if(q.type==2)	//	triangles
		{
			pp = gr->GetPnt(q.n1);
			fprintf(fp,"facet normal %.2g %.2g %.2g\nouter loop\n",pp.u,pp.v,pp.w);
			fprintf(fp,"vertex %g %g %g\n",pp.x,pp.y,pp.z);
			pp = gr->GetPnt(q.n2);
			fprintf(fp,"vertex %g %g %g\n",pp.x,pp.y,pp.z);
			pp = gr->GetPnt(q.n3);
			fprintf(fp,"vertex %g %g %g\n",pp.x,pp.y,pp.z);
			fprintf(fp,"endloop\nendfacet\n");
		}
		if(q.type==3)	//	quadrangles
		{
			pp = gr->GetPnt(q.n1);
			fprintf(fp,"facet normal %.2g %.2g %.2g\nouter loop\n",pp.u,pp.v,pp.w);
			fprintf(fp,"vertex %g %g %g\n",pp.x,pp.y,pp.z);
			pp = gr->GetPnt(q.n2);
			fprintf(fp,"vertex %g %g %g\n",pp.x,pp.y,pp.z);
			pp = gr->GetPnt(q.n3);
			fprintf(fp,"vertex %g %g %g\n",pp.x,pp.y,pp.z);
			fprintf(fp,"endloop\nendfacet\n");
			pp = gr->GetPnt(q.n1);
			fprintf(fp,"facet normal %.2g %.2g %.2g\nouter loop\n",pp.u,pp.v,pp.w);
			pp = gr->GetPnt(q.n4);
			fprintf(fp,"vertex %g %g %g\n",pp.x,pp.y,pp.z);
			pp = gr->GetPnt(q.n2);
			fprintf(fp,"vertex %g %g %g\n",pp.x,pp.y,pp.z);
			pp = gr->GetPnt(q.n3);
			fprintf(fp,"vertex %g %g %g\n",pp.x,pp.y,pp.z);
			fprintf(fp,"endloop\nendfacet\n");
		}
	}
	fprintf(fp,"endsolid %s",(descr && *descr)?descr:"mathgl");
	fclose(fp);
}
void mgl_write_stl_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *d=new char[n+1];	memcpy(d,descr,n);	d[n]=0;
	mgl_write_stl(_GR_,s,d);	delete []s;		delete []d;	}
//-----------------------------------------------------------------------------
void mgl_write_xyz(HMGL gr, const char *fname,const char *descr)
{
	if(gr->GetPrmNum()==0)	return;	// nothing to do

	register long i;
	FILE *fp=fopen(fname,"wt"), *ff;	// vertices definition
	fprintf(fp,"# Created by MathGL library\n# Title: %s\n",(descr && *descr) ? descr : fname);
	fprintf(fp,"# List of Vertices, with (x,y,z) coordinates.\n");
	for(i=0;i<gr->GetPntNum();i++)
	{
		const mglPnt &pp = gr->GetPnt(i);
		fprintf(fp,"%g %g %g\n",pp.x,pp.y,pp.z);
	}
	fclose(fp);

	// primitive definition
	size_t len=strlen(fname);
	char *tname = new char[len+2];	strcpy(tname,fname);	tname[len+1]=tname[len]=0;
	tname[len]='l';	fp = fopen(tname,"wt");
	tname[len]='f';	ff = fopen(tname,"wt");
	fprintf(fp,"# Created by MathGL library\n# Title: %s\n",(descr && *descr) ? descr : fname);
	fprintf(fp,"# Indices of vertices to connect for lines\n");
	fprintf(ff,"# Created by MathGL library\n# Title: %s\n",(descr && *descr) ? descr : fname);
	fprintf(ff,"# Indices of vertices to connect for faces\n");
	for(i=0;i<gr->GetPrmNum();i++)
	{
		const mglPrim &q=gr->GetPrm(i);
		if(q.type==1)	fprintf(fp,"%ld %ld\n",q.n1+1,q.n2+1);
		if(q.type==2)	fprintf(ff,"%ld %ld %ld\n",q.n1+1,q.n2+1,q.n3+1);
		if(q.type==3)	fprintf(ff,"%ld %ld %ld\n%ld %ld %ld\n",q.n1+1,q.n2+1,q.n3+1,q.n4+1,q.n2+1,q.n3+1);
	}
	fclose(fp);	fclose(ff);	delete []tname;
}
void mgl_write_xyz_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *d=new char[n+1];	memcpy(d,descr,n);	d[n]=0;
	mgl_write_xyz(_GR_,s,d);	delete []s;		delete []d;	}
//-----------------------------------------------------------------------------
void mgl_write_off(HMGL gr, const char *fname,const char *descr, int colored)
{
	register long i,nf=0;
	for(i=0;i<gr->GetPrmNum();i++)	// find number of faces
	{
		const mglPrim &q=gr->GetPrm(i);
		if(q.type==2 || q.type==3)	nf++;
	}
	if(nf<=0)	return;	// nothing to do

	FILE *fp=fopen(fname,"wt");
	// vertices definition
	if(colored)
		fprintf(fp,"COFF\n# Created by MathGL library\n# Title: %s\n",(descr && *descr) ? descr : fname);
	else
		fprintf(fp,"OFF\n# Created by MathGL library\n# Title: %s\n",(descr && *descr) ? descr : fname);
	fprintf(fp,"# List of Vertices, with (x,y,z,r,g,b,a) coordinates.\n");
	fprintf(fp,"%ld %ld 0\n",gr->GetPntNum(), nf);
	for(i=0;i<gr->GetPntNum();i++)
	{
		const mglPnt &pp = gr->GetPnt(i);
		if(colored)
			fprintf(fp,"%g %g %g %g %g %g %g\n", pp.x, pp.y, pp.z, pp.r, pp.g, pp.b, pp.a);
		else	fprintf(fp,"%g %g %g\n", pp.x, pp.y, pp.z);
	}
	for(i=0;i<gr->GetPrmNum();i++)
	{
		const mglPrim &q=gr->GetPrm(i);
		const mglPnt &p1=gr->GetPnt(q.n1);
		if(colored)
		{
			if(q.type==2)
				fprintf(fp,"3 %ld %ld %ld\n",q.n1,q.n2,q.n3);
			if(q.type==3)
				fprintf(fp,"4 %ld %ld %ld %ld\n",q.n1,q.n2,q.n4,q.n3);
		}
		else
		{
			if(q.type==2)
			{
				const mglPnt &p2=gr->GetPnt(q.n2), &p3=gr->GetPnt(q.n3);
				if(p1.a>mgl_min_a || p2.a>mgl_min_a || p3.a>mgl_min_a)
					fprintf(fp,"3 %ld %ld %ld %.2g %.2g %.2g %.2g\n",q.n1,q.n2,q.n3,
							(p1.r+p2.r+p3.r)/3, (p1.g+p2.g+p3.g)/3, (p1.b+p2.b+p3.b)/3, (p1.a+p2.a+p3.a)/3);
			}
			if(q.type==3)
			{
				const mglPnt &p2=gr->GetPnt(q.n2), &p3=gr->GetPnt(q.n3), &p4=gr->GetPnt(q.n4);
				if(p1.a>mgl_min_a || p2.a>mgl_min_a || p3.a>mgl_min_a || p4.a>mgl_min_a)
					fprintf(fp,"4 %ld %ld %ld %ld %.2g %.2g %.2g %.2g\n",q.n1,q.n2,q.n4,q.n3,
							(p1.r+p2.r+p3.r+p4.r)/4, (p1.g+p2.g+p3.g+p4.g)/4, (p1.b+p2.b+p3.b+p4.b)/4, (p1.a+p2.a+p3.a+p4.a)/4);
			}
		}
	}
	fclose(fp);
}
void mgl_write_off_(uintptr_t *gr, const char *fname,const char *descr,int *colored,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *d=new char[n+1];	memcpy(d,descr,n);	d[n]=0;
	mgl_write_off(_GR_,s,d,*colored);	delete []s;		delete []d;	}
//-----------------------------------------------------------------------------
void mgl_write_idtf(HMGL /*gr*/, const char */*fname*/,const char */*descr*/)
{	/*_Gr_->WriteIDTF(fname,descr);*/	}	// TODO: Add idtf support later
void mgl_write_idtf_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	mgl_write_idtf(_GR_,s,f);	delete []s;		delete []f;	}
//-----------------------------------------------------------------------------
/*struct mglPrim
{
	// NOTE: n4 is used as mark; n3 -- as pen style for type=0,1,4
	// NOTE: n3 is used as position of txt,font in Ptxt for type=6
	long n1,n2,n3,n4;	///< coordinates of corners
	int type;	///< primitive type (0-point, 1-line, 2-trig, 3-quad, 4-glyph, 6-text)
	int id;		///< object id
	mreal z;	///< z-position
	mreal s;	///< size (if applicable) or fscl
	mreal w;	///< width (if applicable) or ftet
	mreal p;
};*/
bool mglCanvas::WriteJSON(const char *fname)
{
	bool fl = strcmp(fname,"-");
	FILE *fp = fl ? fopen(fname, "wb") : stdout;
	if (!fp)	return true;
	// first save coordinates
	size_t i,l=Pnt.size();
	fprintf(fp,"{\n\"width\":%d,\t\"height\":%d,\t\"depth\":%d,\t\"npnts\":%lu,\t\"pnts\":[\n",
			Width, Height, Depth, (unsigned long)l);
	for(i=0;i<l;i++)
	{
		const mglPnt &q=Pnt[i];
		fprintf(fp,"[%.4g,%.4g,%.4g]%c\n", q.xx, Height-q.yy, q.zz, i+1<l?',':' ');
	}
	l = Prm.size();
	fprintf(fp,"],\t\"nprim\":%lu,\t\"prim\":[\n",(unsigned long)l);

	std::vector<mglPoint> xy;	// vector for glyphs coordinates (to be separated from pnts)
	for(i=0;i<l;i++)
	{
		const mglPrim &p=Prm[i];		mglColor c = GetColor(p);
		long n2 = p.n2;
		// manually exclude absent primitives (TODO be more accurate for quadrangles)
		if(p.n1<0 || (p.type==1 && p.n2<0) || (p.type==2 && (p.n2<0 || p.n3<0)) || (p.type==3 && (p.n2<0 || p.n3<0 || p.n4<0)))
			continue;
		if(p.type==4)
		{
			const mglPnt &q = Pnt[p.n1];
			xy.push_back(mglPoint(q.u,q.v));
			n2 = xy.size()-1;
		}
		if(c.a==1 || p.type==0 || p.type==1 || p.type==4 || p.type==6)
			fprintf(fp,"[%d,%ld,%ld,%ld,%ld,%d,%.4g,%.4g,%.4g,%.4g,\"#%02x%02x%02x\"]%c\n",
				p.type, p.n1, n2, p.n3, p.n4, p.id, p.s, p.w, p.p,	Pnt[p.n1].zz, int(255*c.r), int(255*c.g), int(255*c.b), i+1<l?',':' ');
			//	0		1		2	3		4		5	6	7		8	10
		else
			fprintf(fp,"[%d,%ld,%ld,%ld,%ld,%d,%.4g,%.4g,%.4g,%.4g,\"rgba(%d,%d,%d,%.2g)\"]%c\n",
				p.type, p.n1, n2, p.n3, p.n4, p.id, p.s, p.w, p.p,	Pnt[p.n1].zz, int(255*c.r), int(255*c.g), int(255*c.b), c.a, i+1<l?',':' ');
			//	0		1		2	3		4		5	6	7		8	10
	}
	
	l = xy.size();
	fprintf(fp,"],\t\"ncoor\":%lu,\t\"coor\":[\n",(unsigned long)l);
	for(i=0;i<l;i++)
	{
		const mglPoint &p=xy[i];
		fprintf(fp,"[%.4g,%.4g]%c\n", p.x, p.y, i+1<l?',':' ');
	}

	l = Glf.size();
	fprintf(fp,"],\t\"nglfs\":%lu,\t\"glfs\":[\n",(unsigned long)l);
	for(i=0;i<l;i++)
	{
		const mglGlyph &g=Glf[i];
		fprintf(fp,"[%ld,%ld,\n\t[", g.nt, g.nl);
		register long j;
		for(j=0;j<6*g.nt;j++)	fprintf(fp,"%d%c", g.trig[j], j+1<6*g.nt?',':' ');
		fprintf(fp,"],\n\t[");
		for(j=0;j<4*g.nl;j++)	fprintf(fp,"%d%c", g.line[j], j+1<4*g.nl?',':' ');
		fprintf(fp,"]\n]%c\n", i+1<l?',':' ');
	}
	fprintf(fp,"]\n}\n");
	if(fl)	fclose(fp);
	return false;
}
//-----------------------------------------------------------------------------
void mgl_write_json(HMGL gr, const char *fname,const char *)
{	_Gr_->WriteJSON(fname);	}
void mgl_write_json_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	mgl_write_json(_GR_,s,f);	delete []s;		delete []f;	}
//-----------------------------------------------------------------------------
bool mglCanvas::ExportMGLD(const char *fname, const char *descr)
{
	if(Pnt.size()<1 || Prm.size()<1)	return true;
	FILE *fp=fopen(fname,"wt");
	if(!fp)	return true;
	// NOTE: I'll save Ptx. So prim type=6 is useless,and no LaTeX
	fprintf(fp,"MGLD %lu %lu %lu %lu\n# %s\n", (unsigned long)Pnt.size(), (unsigned long)Prm.size(), (unsigned long)Txt.size(), (unsigned long)Glf.size(), (descr && *descr) ? descr : fname);
	register size_t i;
	fprintf(fp,"# Vertexes: x y z c t ta u v w r g b a\n");
	for(i=0;i<Pnt.size();i++)
	{
		const mglPnt &q=Pnt[i];
		fprintf(fp,"%.4g\t%.4g\t%.4g\t%.4g\t%.4g\t%.4g\t%.4g\t%.4g\t%.4g\t%.4g\t%.4g\t%.4g\t%.4g\n", q.xx, q.yy, q.zz, q.c, q.t, q.ta, q.u, q.v, q.w, q.r, q.g, q.b, q.a);
	}
	fprintf(fp,"# Primitives: type n1 n2 n3 n4 id s w p\n");
	for(i=0;i<Prm.size();i++)
	{
		const mglPrim &p=Prm[i];
		fprintf(fp,"%d\t%ld\t%ld\t%ld\t%ld\t%d\t%g\t%g\t%g\n", p.type, p.n1, p.n2, p.n3, p.n4, p.id, p.s, p.w, p.p);
	}
	fprintf(fp,"# Textures: smooth alpha colors\n");
	for(i=0;i<Txt.size();i++)
	{
		const mglTexture &t=Txt[i];
		fprintf(fp,"%d\t%.4g\t%s\n",t.Smooth,t.Alpha,t.Sch);
	}
	fprintf(fp,"# Glyphs: nt nl [trig] [line]\n");
	for(i=0;i<Glf.size();i++)
	{
		const mglGlyph &g=Glf[i];
		fprintf(fp,"%ld\t%ld\n", g.nt, g.nl);
		if(g.trig)
		{
			for(long j=0;j<6*g.nt;j++)	fprintf(fp,"%d\t",g.trig[j]);
			fprintf(fp,"\n");
		}
		if(g.line)
		{
			for(long j=0;j<4*g.nl;j++)	fprintf(fp,"%d\t",g.line[j]);
			fprintf(fp,"\n");
		}
	}
	fclose(fp);
	return false;
}
//-----------------------------------------------------------------------------
void mgl_export_mgld(HMGL gr, const char *fname,const char *descr)
{	_Gr_->ExportMGLD(fname, descr);	}
void mgl_export_mgld_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	mgl_export_mgld(_GR_,s,f);	delete []s;		delete []f;	}
//-----------------------------------------------------------------------------
bool mglCanvas::ImportMGLD(const char *fname, bool add)
{
	FILE *fp=fopen(fname,"rt");
	if(!fp)	return true;
	char *buf=new char[512];
	if(!fgets(buf,512,fp))	*buf=0;
	if(strncmp(buf,"MGLD",4))	{	delete []buf;	fclose(fp);	return true;	}
	register size_t i;
	unsigned long n=0,m=0,l=0,k=0, npnt=0, nglf=0;
	sscanf(buf+5,"%lu%lu%lu%lu",&n,&m,&l,&k);
	if(n<=0 || m<=0 || l<=0)	{	delete []buf;	fclose(fp);	return true;	}
	if(!add)	{	Clf();	Txt.clear();	}
	else	{	ClfZB();	npnt=Pnt.size();	nglf=Glf.size();	}
	Pnt.reserve(n);	Prm.reserve(m);	Txt.reserve(l);	Glf.reserve(k);
#if MGL_HAVE_PTHREAD
	pthread_mutex_lock(&mutexGlf);
	pthread_mutex_lock(&mutexPnt);
	pthread_mutex_lock(&mutexPrm);
	pthread_mutex_lock(&mutexTxt);
#endif
	mglPnt p;
	for(i=0;i<n;i++)
	{
		do {	if(!fgets(buf,512,fp))	*buf=0;	mgl_strtrim(buf);	} while(*buf=='#');
		sscanf(buf,"%g%g%g%g%g%g%g%g%g%g%g%g%g", &p.xx, &p.yy, &p.zz, &p.c, &p.t, &p.ta, &p.u, &p.v, &p.w, &p.r, &p.g, &p.b, &p.a);
		Pnt.push_back(p);
	}
	mglPrim q;
	for(i=0;i<m;i++)
	{
		do {	if(!fgets(buf,512,fp))	*buf=0;	mgl_strtrim(buf);	} while(*buf=='#');
		sscanf(buf,"%d%ld%ld%ld%ld%d%g%g%g", &q.type, &q.n1, &q.n2, &q.n3, &q.n4, &q.id, &q.s, &q.w, &q.p);
		q.n1 = q.n1>=0?q.n1+npnt:-1;
		q.n2 = q.n2>=0?q.n2+npnt:-1;
		if(q.type==2 || q.type==3)
		{	q.n3 = q.n3>=0?q.n3+npnt:-1;		q.n4 = q.n4>=0?q.n4+npnt:-1;	}
		if(q.type==4)	q.n4 = q.n4>=0?q.n4+nglf:-1;
		if(q.type<5)		Prm.push_back(q);
	}
	mglTexture t;
	for(i=0;i<l;i++)
	{
		int sm=0;	float a;
		do {	if(!fgets(buf,512,fp))	*buf=0;	mgl_strtrim(buf);	} while(*buf=='#');
		register size_t j,k=0;
		for(j=0;buf[j];j++)
		{
			if(buf[j]<=' ' && k)	{	sm++;	k=0;	}
			if(buf[j]>' ')	k=1;
			if(sm==2 && k)	break;
		}
		sscanf(buf,"%d%g", &sm, &a);
		t.Set(buf+j, sm, a);
		Txt.push_back(t);
	}
	mglGlyph g;
	for(i=0;i<k;i++)
	{
		do {	if(!fgets(buf,512,fp))	*buf=0;	mgl_strtrim(buf);	} while(*buf=='#' || *buf==0);
		long nt=0,nl=0;
		sscanf(buf,"%ld%ld", &nt, &nl);	g.Create(nt,nl);
		register long j;
		for(j=0;j<6*nt;j++)	fscanf(fp,"%hd",g.trig+j);
		for(j=0;j<4*nl;j++)	fscanf(fp,"%hd",g.line+j);
		Glf.push_back(g);
	}
#if MGL_HAVE_PTHREAD
	pthread_mutex_unlock(&mutexGlf);
	pthread_mutex_unlock(&mutexPnt);
	pthread_mutex_unlock(&mutexPrm);
	pthread_mutex_unlock(&mutexTxt);
#endif
	delete []buf;	fclose(fp);	return false;
}
//-----------------------------------------------------------------------------
void mgl_import_mgld(HMGL gr, const char *fname, int add)
{	_Gr_->ImportMGLD(fname, add);	}
void mgl_import_mgld_(uintptr_t *gr, const char *fname, int *add, int l)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	mgl_import_mgld(_GR_,s,*add);	delete []s;	}
//-----------------------------------------------------------------------------
/*void mgl_xgl_prim(const mglPrim &q, const mglPnt &p, FILE *fp, mreal size)
{
	char type = q.n4;	mreal ss=size*0.35;
	register long i=q.n1,j;
	switch(q.type)
	{
	case 0:
		if(!strchr("xsSoO",type))	ss *= 1.1;
		if(type=='.' || ss==0)	fprintf(fp,"p %ld\n", i);
		else	switch(type)	// TODO: save mark by PATCH
		{
		case 'P':
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y-ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y-ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y+ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y+ss,p.z);
			fprintf(fp,"l -4/%ld -3/%ld\n", i,i);
			fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
			fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
			fprintf(fp,"l -1/%ld -4/%ld\n", i,i);
		case '+':
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y,p.z);
			fprintf(fp,"v %g %g %g\n",p.x,p.y+ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x,p.y+ss,p.z);
			fprintf(fp,"l -4/%ld -3/%ld\n", i,i);
			fprintf(fp,"l -2/%ld -1/%ld\n", i,i);	break;
		case 'X':
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y-ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y-ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y+ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y+ss,p.z);
			fprintf(fp,"l -4/%ld -3/%ld\n", i,i);
			fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
			fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
			fprintf(fp,"l -1/%ld -4/%ld\n", i,i);
			fprintf(fp,"l -1/%ld -3/%ld\n", i,i);
			fprintf(fp,"l -2/%ld -4/%ld\n", i,i);	break;
		case 'x':
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y-ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y-ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y+ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y+ss,p.z);
			fprintf(fp,"l -1/%ld -3/%ld\n", i,i);
			fprintf(fp,"l -2/%ld -4/%ld\n", i,i);	break;
		case 'S':
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y-ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y-ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y+ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y+ss,p.z);
			fprintf(fp,"f -4/%ld -3/%ld -2/%ld -1/%ld\n",i,i,i,i);	break;
		case 's':
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y-ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y-ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y+ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y+ss,p.z);
			fprintf(fp,"l -4/%ld -3/%ld\n", i,i);
			fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
			fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
			fprintf(fp,"l -1/%ld -4/%ld\n", i,i);	break;
		case 'D':
			fprintf(fp,"v %g %g %g\n",p.x,p.y-ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y,p.z);
			fprintf(fp,"v %g %g %g\n",p.x,p.y+ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y,p.z);
			fprintf(fp,"f -4/%ld -3/%ld -2/%ld -1/%ld\n",i,i,i,i);	break;
		case 'd':
			fprintf(fp,"v %g %g %g\n",p.x,p.y-ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y,p.z);
			fprintf(fp,"v %g %g %g\n",p.x,p.y+ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y,p.z);
			fprintf(fp,"l -4/%ld -3/%ld\n", i,i);
			fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
			fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
			fprintf(fp,"l -1/%ld -4/%ld\n", i,i);	break;
		case 'Y':
			fprintf(fp,"v %g %g %g\n",p.x,p.y-ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+0.8*ss,p.y+0.6*ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x-0.8*ss,p.y+0.6*ss,p.z);
			fprintf(fp,"l -3/%ld %ld/%ld\n", i,i,i);
			fprintf(fp,"l -2/%ld %ld/%ld\n", i,i,i);
			fprintf(fp,"l -1/%ld %ld/%ld\n", i,i,i);	break;
		case '*':
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y,p.z);
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y,p.z);
			fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
			fprintf(fp,"v %g %g %g\n",p.x+0.6*ss,p.y+0.8*ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x-0.6*ss,p.y-0.8*ss,p.z);
			fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
			fprintf(fp,"v %g %g %g\n",p.x+0.6*ss,p.y-0.8*ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x-0.6*ss,p.y+0.8*ss,p.z);
			fprintf(fp,"l -2/%ld -1/%ld\n", i,i);		break;
		case 'T':
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y-ss/2,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y-ss/2,p.z);
			fprintf(fp,"v %g %g %g\n",p.x,p.y+ss,p.z);
			fprintf(fp,"f -3/%ld -2/%ld -1/%ld\n", i,i,i);	break;
		case '^':
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y-ss/2,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y-ss/2,p.z);
			fprintf(fp,"v %g %g %g\n",p.x,p.y+ss,p.z);
			fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
			fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
			fprintf(fp,"l -1/%ld -3/%ld\n", i,i);	break;
		case 'V':
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y+ss/2,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y+ss/2,p.z);
			fprintf(fp,"v %g %g %g\n",p.x,p.y-ss,p.z);
			fprintf(fp,"f -3/%ld -2/%ld -1/%ld\n", i,i,i);	break;
		case 'v':
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y+ss/2,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y+ss/2,p.z);
			fprintf(fp,"v %g %g %g\n",p.x,p.y-ss,p.z);
			fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
			fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
			fprintf(fp,"l -1/%ld -3/%ld\n", i,i);	break;
		case 'L':
			fprintf(fp,"v %g %g %g\n",p.x+ss/2,p.y+ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss/2,p.y-ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y,p.z);
			fprintf(fp,"f -3/%ld -2/%ld -1/%ld\n", i,i,i);	break;
		case '<':
			fprintf(fp,"v %g %g %g\n",p.x+ss/2,p.y+ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss/2,p.y-ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x-ss,p.y,p.z);
			fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
			fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
			fprintf(fp,"l -1/%ld -3/%ld\n", i,i);	break;
		case 'R':
			fprintf(fp,"v %g %g %g\n",p.x-ss/2,p.y+ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x-ss/2,p.y-ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y,p.z);
			fprintf(fp,"f -3/%ld -2/%ld -1/%ld\n", i,i,i);	break;
		case '>':
			fprintf(fp,"v %g %g %g\n",p.x-ss/2,p.y+ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x-ss/2,p.y-ss,p.z);
			fprintf(fp,"v %g %g %g\n",p.x+ss,p.y,p.z);
			fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
			fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
			fprintf(fp,"l -1/%ld -3/%ld\n", i,i);	break;
		case 'O':
			for(long j=0;j<=20;j++)
				fprintf(fp,"v %g %g %g\n",p.x+ss*cos(j*M_PI/10),p.y+ss*sin(j*M_PI/10),p.z);
			for(long j=0;j<20;j++)
				fprintf(fp,"f %ld/%ld %ld/%ld %ld/%ld\n", j-21,i, j-20,i, i,i);
			break;
		case 'C':	fprintf(fp,"p %ld\n", i);
		case 'o':
			for(long j=0;j<=20;j++)
				fprintf(fp,"v %g %g %g\n",p.x+ss*cos(j*M_PI/10),p.y+ss*sin(j*M_PI/10),p.z);
			for(long j=0;j<20;j++)
				fprintf(fp,"l %ld/%ld %ld/%ld\n", j-21,i, j-20,i);
			break;
		}
		break;
	case 1:	fprintf(fp,"l %ld/%ld %ld/%ld\n", q.n1,q.n1, q.n2,q.n2);	break;
	case 2:	fprintf(fp,"f %ld/%ld/%ld %ld/%ld/%ld %ld/%ld/%ld\n",
		q.n1,q.n1,q.n1, q.n2,q.n2,q.n2, q.n3,q.n3,q.n3);	break;
	case 3:	fprintf(fp,"f %ld/%ld/%ld %ld/%ld/%ld %ld/%ld/%ld %ld/%ld/%ld\n",
		q.n1,q.n1,q.n1, q.n2,q.n2,q.n2, q.n3,q.n3,q.n3, q.n4,q.n4,q.n4);	break;
	case 4:	break;	// TODO: add glyphs export later
	}
}
//-----------------------------------------------------------------------------
void mglCanvas::WriteXGL(const char *fname,const char *descr)
{
	if(GetPrmNum()==0)	return;	// nothing to do
	FILE *fp=fopen(fname,"wt");
	if(!fp)	return true;
	fprintf(fp,"<WORLD>\n<NAME>%s</NAME>\n", (descr && *descr)?descr:fname);
	fprintf(fp,"<BACKGROUND><BACKCOLOR>%g, %g, %g</BACKCOLOR></BACKGROUND>\n", BDef[0]/255., BDef[1]/255., BDef[2]/255.);
	fprintf(fp,"<LIGHTING>\n<AMBIENT>%g, %g, %g</AMBIENT>\n",AmbBr, AmbBr, AmbBr);
	register size_t i,j;
	if(get(MGL_ENABLE_LIGHT))	for(i=0;i<10;i++)
		if(light[i].n && mgl_isnan(light[i].r.x))
		{
			fprintf(fp, "<DIRECTIONALLIGHT>\n<DIRECTION>%g, %g, %g</DIRECTION>\n", light[i].d.x, light[i].d.y, light[i].d.z);
			fprintf(fp, "<SPECULAR>%g, %g, %g</SPECULAR>\n</DIRECTIONALLIGHT>\n", light[i].c.r, light[i].c.g, light[i].c.b);
		}
	fprintf(fp,"</LIGHTING>");

	// TODO: add textures

	long m1=0,m2=0,m;
	for(i=0;i<Grp.size();i++)	// prepare array of indirect indexing
	{	m = Grp[i].Id;	if(m<m1) m1=m;	if(m>m2) m2=m;	}
	long *ng = new long[m2-m1+1];
	for(i=0;i<Grp.size();i++)	ng[gr->Grp[i].Id-m1] = i;
	for(i=0;i<GetPrmNum();i++)	// collect data for groups
	// it is rather expensive (extra 4b per primitive) but need for export to 3D
	{
		m = GetPrm(i).id-m1;
		if(m>=0 && m<m2-m1+1)	Grp[ng[m]].p.push_back(i);
	}
	delete []ng;

	std::vector<long> p;
	mglPrim q;
	char *pg=new char[GetPntNum()];
	for(i=0;i<Grp.size();i++)	// first write objects
	{
		p = Grp[i].p;	memset(pg,0,GetPntNum());
		fprintf(fp,"<OBJECT>\n<NAME>%s</NAME>\n<MESH>\n",Grp[i].Lbl.c_str());
		for(j=0;j<p.size();j++)		// collect Pnt for this object
		{
			const mglPrim q=GetPrm(p[j]);	pg[q.n1]=1;
			if(q.type==3)	{	pg[q.n2]=1;	pg[q.n3]=1;	pg[q.n4]=1;	}
			else if(q.type==1)	pg[q.n2]=1;
			else if(q.type==2)	{	pg[q.n2]=1;	pg[q.n3]=1;	}
		}
		for(j=0;j<GetPntNum();j++)	if(pg[j])	// write Pnt for this object
		{
			const mglPnt s=Pnt[j];
			fprintf(fp,"<P ID=\"%u\">%g, %g, %g</P>\n",j, s.x, s.y, s.z);
			fprintf(fp,"<N ID=\"%u\">%g, %g, %g</N>\n",j, s.x, s.y, s.z);
		}
		// TODO: add line styles
		for(j=0;j<p.size();j++)	// now write primitives itself
		{
			const mglPrim q=GetPrm(p[j]);
			mgl_xgl_prim(q, GetPnt(q.n1), fp, q.s*FontFactor());
		}
		fprintf(fp,"</MESH>\n</OBJECT>");	// finish with this object
		Grp[i].p.clear();	// we don't need indexes anymore
	}
	// TODO: try to save "ungrouped" primitives

	fprintf(fp,"</WORLD>");	fclose(fp);	delete []pg;
}
//-----------------------------------------------------------------------------
void mgl_write_xgl(HMGL gr, const char *fname,const char *descr)
{	_Gr_->WriteXGL(fname,descr);	}
void mgl_write_xgl_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *d=new char[n+1];	memcpy(d,descr,n);	d[n]=0;
	mgl_write_xgl(_GR_,s,d);	delete []s;		delete []d;	}*/
//-----------------------------------------------------------------------------
void mgl_x3d_mdef(HMGL gr, void *fp, bool gz)
{
	bool m_p=false,m_x=false,m_d=false,m_v=false,m_t=false,
	m_s=false,m_a=false,m_o=false,m_T=false,
	m_V=false,m_S=false,m_D=false,m_Y=false,m_l=false,
	m_L=false,m_r=false,m_R=false,m_X=false,m_P=false;
	for(size_t i=0;i<gr->GetPrmNum();i++)
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
	if(m_p)	mgl_printf(fp, gz, "<ProtoDeclare name='m_p'><ProtoInterface/>\n<ProtoBody>"
		"<LineSet vertexCount='0,1,2,3'>\n<Coordinate point='-1 0 0, 1 0 0, 0 -1 0, 0 1 0'/>"
		"\n</LineSet></ProtoBody></ProtoDeclare>\n");
	/*if(m_x)	mgl_printf(fp, gz, "/m_x {sm sm rm s2 s2 rl 0 sm 2 mul rm sm 2 mul s2 rl d0} def\n");	// TODO
	 *	if(m_s)	mgl_printf(fp, gz, "/m_s {sm sm rm 0 s2 rl s2 0 rl 0 sm 2 mul rl cp d0} def\n");
	 *	if(m_d)	mgl_printf(fp, gz, "/m_d {sm 0 rm ss ss rl ss sm rl sm sm rl cp d0} def\n");
	 *	if(m_v)	mgl_printf(fp, gz, "/m_v {sm ss 2 div rm s2 0 rl sm sm 1.5 mul rl d0 cp} def\n");
	 *	if(m_t)	mgl_printf(fp, gz, "/m_t {sm sm 2 div rm s2 0 rl sm ss 1.5 mul rl d0 cp} def\n");
	 *	if(m_a)	mgl_printf(fp, gz, "/m_a {sm 0 rm s2 0 rl sm 1.6 mul sm 0.8 mul rm ss 1.2 mul ss 1.6 mul rl 0 sm 1.6 mul rm sm 1.2 mul ss 1.6 mul rl d0} def\n");
	 *	if(m_o)	mgl_printf(fp, gz, "/m_o {ss 0 360 d0 arc} def\n");
	 *	if(m_S)	mgl_printf(fp, gz, "/m_S {sm sm rm 0 s2 rl s2 0 rl 0 sm 2 mul rl cp} def\n");
	 *	if(m_D)	mgl_printf(fp, gz, "/m_D {sm 0 rm ss ss rl ss sm rl sm sm rl cp} def\n");
	 *	if(m_V)	mgl_printf(fp, gz, "/m_V {sm ss 2 div rm s2 0 rl sm sm 1.5 mul rl cp} def\n");
	 *	if(m_T)	mgl_printf(fp, gz, "/m_T {sm sm 2 div rm s2 0 rl sm ss 1.5 mul rl cp} def\n");
	 *	if(m_Y)	mgl_printf(fp, gz, "/m_Y {0 sm rm 0 ss rl sm ss rl s2 0 rm sm sm rl d0} def\n");
	 *	if(m_r)	mgl_printf(fp, gz, "/m_r {sm 2 div sm rm 0 s2 rl ss 1.5 mul sm rl d0 cp} def\n");
	 *	if(m_l)	mgl_printf(fp, gz, "/m_l {ss 2 div sm rm 0 s2 rl sm 1.5 mul sm rl d0 cp} def\n");
	 *	if(m_R)	mgl_printf(fp, gz, "/m_R {sm 2 div sm rm 0 s2 rl ss 1.5 mul sm rl cp} def\n");
	 *	if(m_L)	mgl_printf(fp, gz, "/m_L {ss 2 div sm rm 0 s2 rl sm 1.5 mul sm rl cp} def\n");
	 *	if(m_P)	mgl_printf(fp, gz, "/m_P {m_p 0 sm rm m_s} def\n");
	 *	if(m_X)	mgl_printf(fp, gz, "/m_X {m_x ss sm rm m_s} def\n");*/
	//	if(m_C)	mgl_printf(fp, gz, "/m_C {m_c m_o} def\n");
	mgl_printf(fp, gz, "\n");
}
//-----------------------------------------------------------------------------
void mgl_x3d_prim(const mglPrim &q, const mglPnt &p, const long *pnt, void *fp,bool gz, mreal size)
{
	// <ProtoInstance name='EmissiveMaterial'/>
/*		if(q.type==0)	// mark
		{
			mreal x0 = p1.x,y0 = p1.y;
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
			put_line(gr,fp,gz,i,wp,cp,st, "np %g %g mt ", "%g %g ll ", false, 1);
			const char *sd = mgl_get_dash(q.n3,q.w);
			if(sd && sd[0])	mgl_printf(fp, gz, "%s [%s] %g sd dr\n",str,sd,q.w*q.s);
			else			mgl_printf(fp, gz, "%s d0 dr\n",str);
		}
		else if(q.type==4)	// glyph
		{
			mreal 	ss = q.s/2, xx = p1.u, yy = p1.v, zz = p1.w;
			mgl_printf(fp, gz, "gsave\t%g %g translate %g %g scale %g rotate %s\n",
					   p1.x, p1.y, ss, ss, -q.w, str);
			if(q.n3&8)	// this is "line"
			{
				mreal dy = 0.004,f=fabs(zz);
				mgl_printf(fp, gz, "np %g %g mt %g %g ll %g %g ll %g %g ll cp ",
						   xx,yy+dy, xx+f,yy+dy, xx+f,yy-dy, xx,yy-dy);
			}
			else
				mgl_printf(fp, gz, "%.3g %.3g translate %g %g scale %c%c_%04x ",
						   xx, yy, zz, zz, q.n3&1?'b':'n', q.n3&2?'i':'n', q.n4);
			if(q.n3&4)	mgl_printf(fp, gz, "dr");
			else	mgl_printf(fp, gz, "eofill");
			mgl_printf(fp, gz, " grestore\n");
		}*/	
}
//-----------------------------------------------------------------------------
void mgl_write_x3d(HMGL gr, const char *fname,const char *descr)
{
	if(gr->GetPrmNum()<1)	return;
	time_t now;	time(&now);

	bool gz = fname[strlen(fname)-1]=='z';
	void *fp = gz ? (void*)gzopen(fname,"wt") : (void*)fopen(fname,"wt");
	if(!fp)		{	gr->SetWarn(mglWarnOpen,fname);	return;	}
	mgl_printf(fp, gz, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	mgl_printf(fp, gz, "<!DOCTYPE X3D PUBLIC \"ISO//Web3D//DTD X3D 3.0//EN\" \"http://www.web3d.org/specifications/x3d-3.0.dtd\">\n");
	mgl_printf(fp, gz, "<X3D profile='Immersive'>\n<head>\n<meta name='filename' content='%s'/>\n",fname);
	mgl_printf(fp, gz, "<meta name='description' content='%s'/>\n",(descr && *descr)?descr:fname);
	mgl_printf(fp, gz, "<meta name='created' content='%s'/>\n",ctime(&now));
	mgl_printf(fp, gz, "<meta name='generator' content='MathGL, http://mathgl.sourceforge.net/'/>\n");
	mgl_printf(fp, gz, "</head>\n<Scene>\n");

	// 1. first we have to define proto for marks and glyphs
	mgl_x3d_mdef(gr, fp, gz);

	// here should be defined textures ... but since X3D support RGBA then omit it in this version

	// 2. now find group for primitives
	long m1=0,m2=0,m;
	register size_t i,j;
	for(i=0;i<gr->Grp.size();i++)	// prepare array of indirect indexing
	{	m = gr->Grp[i].Id;	if(m<m1) m1=m;	if(m>m2) m2=m;	}
	long *ng = new long[m2-m1+1];
	for(i=0;i<gr->Grp.size();i++)	ng[gr->Grp[i].Id-m1] = i;
	for(i=0;i<gr->GetPrmNum();i++)	// collect data for groups
	// it is rather expensive (extra 4b per primitive) but need for export to 3D
	{
		m = gr->GetPrm(i).id-m1;
		if(m>=0 && m<m2-m1+1)	gr->Grp[ng[m]].p.push_back(i);
	}
	delete []ng;

	// primitive definition in groups
	long npnt = gr->GetPntNum(), k;
	long *pnt=new long[npnt];
	mglPrim q;
	for(i=0;i<gr->Grp.size();i++)
	{
		mgl_printf(fp,gz,"<Group><!--%s-->\n",gr->Grp[i].Lbl.c_str());
		std::vector<long> &p = gr->Grp[i].p;

		// define coordinates, colors and so on
		memset(pnt,-1,npnt*sizeof(long));
		for(j=0,k=0;j<p.size();j++)	// find points for this group
		{
			const mglPrim &q=gr->GetPrm(p[j]);
			if(q.n1>=0 && pnt[q.n1]<0)	{	pnt[q.n1]=k;	k++;	}
			if(q.type>0 && q.type<4 && q.n2>=0 && pnt[q.n2]<0)	{	pnt[q.n2]=k;	k++;	}
			if(q.type>1 && q.type<4 && q.n3>=0 && pnt[q.n3]<0)	{	pnt[q.n3]=k;	k++;	}
			if(q.type==3 && q.n4>=0 && pnt[q.n4]<0)	{	pnt[q.n4]=k;	k++;	}
		}
		
		mgl_printf(fp, gz, "<Coordinate DEF='mypnts_%ld' point='",i);
		for(i=0;i<gr->GetPntNum();i++)	if(pnt[i]>=0)
		{	const mglPnt &p=gr->GetPnt(i);	mgl_printf(fp, gz, "%g %g %g, ", p.x,p.y,p.z);	}
		mgl_printf(fp, gz, "0.0 0.0 0.0'/>");
		mgl_printf(fp, gz, "<Color DEF='myclrs' color='");
		for(i=0;i<gr->GetPntNum();i++)	if(pnt[i]>=0)
		{	const mglPnt &p=gr->GetPnt(i);	mgl_printf(fp, gz, "%g %g %g, ", p.r,p.g,p.b);	}
		mgl_printf(fp, gz, "0.0 0.0 0.0'/>");
		// no normals since mathgl ones are "signless" -- x3d should calculate it by itself

		for(j=0;j<p.size();j++)
		{
			const mglPrim &q=gr->GetPrm(p[j]);	// TODO: collect by type (quads,trig,line) and draw together???
			mgl_x3d_prim(q, gr->GetPnt(q.n1), pnt, fp,gz, q.s*gr->FontFactor());
		}
		mgl_printf(fp,gz,"</Group><!--%s-->\n",gr->Grp[i].Lbl.c_str());
		gr->Grp[i].p.clear();	// we don't need indexes anymore
	}
	mgl_printf(fp, gz, "</Scene>\n");
	if(gz)	gzclose((gzFile)fp);	else	fclose((FILE *)fp);
}
void mgl_write_x3d_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
char *d=new char[n+1];	memcpy(d,descr,n);	d[n]=0;
mgl_write_x3d(_GR_,s,d);	delete []s;		delete []d;	}
//-----------------------------------------------------------------------------
