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
#include "mgl/canvas.h"
#include "mgl/canvas_cf.h"
#undef _GR_
#define _GR_	((mglCanvas *)(*gr))
#define _Gr_	((mglCanvas *)(gr))
int mgl_tga_save(const char *fname, int w, int h, unsigned char **p);
int mgl_png_save(const char *fname, int w, int h, unsigned char **p);
//-----------------------------------------------------------------------------
void mglTexture::GetRGBA(unsigned char *f)
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
			f[i0]   = int(255*c.b);
			f[i0+1] = int(255*c.g);
			f[i0+2] = int(255*c.r);
			f[i0+3] = int(255*c.a);
		}
	}
}
//-----------------------------------------------------------------------------
void mgl_obj_prim(const mglPrim &q, const mglPnt &p, FILE *fp, float size)
{
	char type = q.n4;	float ss=size*0.35;
	register long i=q.n1,j;
	switch(q.type)
	{
		case 0:
			if(!strchr("xsSoO",type))	ss *= 1.1;
			if(type=='.' || ss==0)	fprintf(fp,"p %ld\n", i);
			else	switch(type)
			{
			case 'P':
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y-ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y-ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y+ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y+ss,p.z);
				fprintf(fp,"l -4/%ld -3/%ld\n", i,i);
				fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"l -1/%ld -4/%ld\n", i,i);
			case '+':
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x,p.y+ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x,p.y+ss,p.z);
				fprintf(fp,"l -4/%ld -3/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);	break;
			case 'X':
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y-ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y-ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y+ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y+ss,p.z);
				fprintf(fp,"l -4/%ld -3/%ld\n", i,i);
				fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"l -1/%ld -4/%ld\n", i,i);
				fprintf(fp,"l -1/%ld -3/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -4/%ld\n", i,i);	break;
			case 'x':
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y-ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y-ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y+ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y+ss,p.z);
				fprintf(fp,"l -1/%ld -3/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -4/%ld\n", i,i);	break;
			case 'S':
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y-ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y-ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y+ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y+ss,p.z);
				fprintf(fp,"f -4/%ld -3/%ld -2/%ld -1/%ld\n",i,i,i,i);	break;
			case 's':
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y-ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y-ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y+ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y+ss,p.z);
				fprintf(fp,"l -4/%ld -3/%ld\n", i,i);
				fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"l -1/%ld -4/%ld\n", i,i);	break;
			case 'D':
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x,p.y-ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x,p.y+ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y,p.z);
				fprintf(fp,"f -4/%ld -3/%ld -2/%ld -1/%ld\n",i,i,i,i);	break;
			case 'd':
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x,p.y-ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x,p.y+ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y,p.z);
				fprintf(fp,"l -4/%ld -3/%ld\n", i,i);
				fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"l -1/%ld -4/%ld\n", i,i);	break;
			case 'Y':
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x,p.y-ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+0.8*ss,p.y+0.6*ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-0.8*ss,p.y+0.6*ss,p.z);
				fprintf(fp,"l -3/%ld %ld/%ld\n", i,i,i);
				fprintf(fp,"l -2/%ld %ld/%ld\n", i,i,i);
				fprintf(fp,"l -1/%ld %ld/%ld\n", i,i,i);	break;
			case '*':
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y,p.z);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+0.6*ss,p.y+0.8*ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-0.6*ss,p.y-0.8*ss,p.z);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+0.6*ss,p.y-0.8*ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-0.6*ss,p.y+0.8*ss,p.z);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);		break;
			case 'T':
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y-ss/2,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y-ss/2,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x,p.y+ss,p.z);
				fprintf(fp,"f -3/%ld -2/%ld -1/%ld\n", i,i,i);	break;
			case '^':
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y-ss/2,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y-ss/2,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x,p.y+ss,p.z);
				fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"l -1/%ld -3/%ld\n", i,i);	break;
			case 'V':
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y+ss/2,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y+ss/2,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x,p.y-ss,p.z);
				fprintf(fp,"f -3/%ld -2/%ld -1/%ld\n", i,i,i);	break;
			case 'v':
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y+ss/2,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y+ss/2,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x,p.y-ss,p.z);
				fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"l -1/%ld -3/%ld\n", i,i);	break;
			case 'L':
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss/2,p.y+ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss/2,p.y-ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y,p.z);
				fprintf(fp,"f -3/%ld -2/%ld -1/%ld\n", i,i,i);	break;
			case '<':
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss/2,p.y+ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss/2,p.y-ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss,p.y,p.z);
				fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"l -1/%ld -3/%ld\n", i,i);	break;
			case 'R':
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss/2,p.y+ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss/2,p.y-ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y,p.z);
				fprintf(fp,"f -3/%ld -2/%ld -1/%ld\n", i,i,i);	break;
			case '>':
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss/2,p.y+ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x-ss/2,p.y-ss,p.z);
				fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss,p.y,p.z);
				fprintf(fp,"l -3/%ld -2/%ld\n", i,i);
				fprintf(fp,"l -2/%ld -1/%ld\n", i,i);
				fprintf(fp,"l -1/%ld -3/%ld\n", i,i);	break;
			case 'O':
				for(long j=0;j<=20;j++)
					fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss*cos(j*M_PI/10),p.y+ss*sin(j*M_PI/10),p.z);
				for(long j=0;j<20;j++)
					fprintf(fp,"f %ld/%ld %ld/%ld %ld/%ld\n", j-21,i, j-20,i, i,i);
				break;
			case 'C':	fprintf(fp,"p %ld\n", i);
			case 'o':
				for(long j=0;j<=20;j++)
					fprintf(fp,"v %.2g %.2g %.2g\n",p.x+ss*cos(j*M_PI/10),p.y+ss*sin(j*M_PI/10),p.z);
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
void mgl_write_obj(HMGL gr, const char *fname,const char *descr, int use_png)
{
	if(gr->GetPrmNum()<=0)	return;	// nothing to do
	register unsigned long i,j;
	long m1=0,m2=0,m;
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

	unsigned len=strlen(fname);
	char *tname = new char[len+1];	strcpy(tname,fname);
	FILE *fp=fopen(fname,"wt");
	// vertices definition
	fprintf(fp,"# Creator: MathGL library\n# Title: %s\n",descr ? descr : fname);
	for(i=0;i<gr->GetPntNum();i++)
	{
		mglPnt pp = gr->GetPnt(i);
		fprintf(fp,"v %.2g %.2g %.2g\n",pp.x,pp.y,pp.z);
		fprintf(fp,"vt %.2g %.2g\n",256*pp.t,256*pp.c);
		fprintf(fp,"vn %.2g %.2g %.2g\n",pp.u,pp.v,pp.w);
	}
	// primitive definition in groups
	tname[len-4]=0;	fprintf(fp,"# Primitives Definitions\nusemtl %s\n",tname);
	std::vector<long> p;
	mglPrim q;
	for(i=0;i<gr->Grp.size();i++)
	{
		fprintf(fp,"g %s\n",gr->Grp[i].Lbl.c_str());	p = gr->Grp[i].p;
		for(j=0;j<p.size();j++)
		{
			const mglPrim q=gr->GetPrm(p[j]);
			if(q.n1>=0)	mgl_obj_prim(q, gr->GetPnt(q.n1), fp, q.s*gr->FontFactor());
		}
		gr->Grp[i].p.clear();	// we don't need indexes anymore
	}
	// try to save "ungrouped" primitives
	fprintf(fp,"g ungrouped\n");
	for(i=0;i<gr->GetPrmNum();i++)
	{
		const mglPrim q=gr->GetPrm(p[j]);	m = q.id-m1;
		if((m<0 || m>=m2-m1+1) && q.n1>=0)
			mgl_obj_prim(q, gr->GetPnt(q.n1), fp, q.s*gr->FontFactor());
	}
	fclose(fp);
	// prepare MTL file
	tname[len-4]='.';	tname[len-3]='m';	tname[len-2]='t';	tname[len-1]='l';
	fp=fopen(tname,"wt");
	tname[len-4]=0;	fprintf(fp,"newmtl %s\n",tname);
	fprintf(fp,"Ka 1.000 1.000 1.000\n");
	fprintf(fp,"Kd 1.000 1.000 1.000\n");
	fprintf(fp,"Ks 0.000 0.000 0.000\n");
	fprintf(fp,"d 1.0\nillum 2\n");
	if(use_png)
	{	tname[len-4]='.';	tname[len-3]='p';	tname[len-2]='n';	tname[len-1]='g';	}
	else
	{	tname[len-4]='.';	tname[len-3]='t';	tname[len-2]='g';	tname[len-1]='a';	}
	fprintf(fp,"map_Ka %s\nmap_Kd %s\nmap_Ks %s\n",tname,tname,tname);
	fclose(fp);
	// prepare texture file (TGA or PNG)
	j=gr->GetTxtNum();
	unsigned char *buf = new unsigned char[256*256*j];
	unsigned char **pbuf= (unsigned char **)malloc(256*j*sizeof(unsigned char *));
	for(i=0;i<256*j;i++)	pbuf[i] = buf+256*i;
	for(i=0;i<j;i++)	gr->GetTxt(i).GetRGBA(buf+i*256*256);
	if(use_png)	mgl_png_save(tname,256,256*j,pbuf);
	else		mgl_tga_save(tname,256,256*j,pbuf);
	free(pbuf);	delete []buf;	delete []tname;
}
void mgl_write_obj_(uintptr_t *gr, const char *fname,const char *descr, int *use_png,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *d=new char[n+1];	memcpy(d,descr,n);	d[n]=0;
	mgl_write_obj(_GR_,s,d,*use_png);	delete []s;		delete []d;	}
//-----------------------------------------------------------------------------
void mgl_write_stl(HMGL gr, const char *fname,const char *descr)
{
	if(gr->GetPrmNum()<=0)	return;	// nothing to do
	FILE *fp = fopen(fname,"wt");
	fprintf(fp,"solid %s",descr?descr:"mathgl");
	register long i,j;
	mglPnt pp;
	for(i=0;i<gr->GetPrmNum();i++)
	{
		const mglPrim &q=gr->GetPrm(i);	j = q.n1;
		if(j<0 || q.n2<0 || q.n3<0)	continue;
		if(q.type==2)	//	triangles
		{
			pp = gr->GetPnt(j);
			fprintf(fp,"facet normal %.2g %.2g %.2g\nouter loop\n",pp.u,pp.v,pp.w);
			fprintf(fp,"vertex %.2g %.2g %.2g\n",pp.x,pp.y,pp.z);
			pp = gr->GetPnt(q.n2);
			fprintf(fp,"vertex %.2g %.2g %.2g\n",pp.x,pp.y,pp.z);
			pp = gr->GetPnt(q.n3);
			fprintf(fp,"vertex %.2g %.2g %.2g\n",pp.x,pp.y,pp.z);
			fprintf(fp,"endloop\nendfacet\n");
		}
		if(q.type==3 && q.n4>=0)	//	triangles
		{
			pp = gr->GetPnt(j);
			fprintf(fp,"facet normal %.2g %.2g %.2g\nouter loop\n",pp.u,pp.v,pp.w);
			fprintf(fp,"vertex %.2g %.2g %.2g\n",pp.x,pp.y,pp.z);
			pp = gr->GetPnt(q.n2);
			fprintf(fp,"vertex %.2g %.2g %.2g\n",pp.x,pp.y,pp.z);
			pp = gr->GetPnt(q.n3);
			fprintf(fp,"vertex %.2g %.2g %.2g\n",pp.x,pp.y,pp.z);
			pp = gr->GetPnt(q.n4);
			fprintf(fp,"vertex %.2g %.2g %.2g\n",pp.x,pp.y,pp.z);
			fprintf(fp,"endloop\nendfacet\n");
		}
	}
}
void mgl_write_stl_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *d=new char[n+1];	memcpy(d,descr,n);	d[n]=0;
	mgl_write_stl(_GR_,s,d);	delete []s;		delete []d;	}
//-----------------------------------------------------------------------------
void mgl_write_xyz(HMGL gr, const char *fname,const char *descr)
{
	if(gr->GetPrmNum()<=0)	return;	// nothing to do

	register long i;
	FILE *fp=fopen(fname,"wt"), *ff;	// vertices definition
	fprintf(fp,"# Creator: MathGL library\n# Title: %s\n",descr ? descr : fname);
	fprintf(fp,"# List of Vertices, with (x,y,z) coordinates.\n");
	for(i=0;i<gr->GetPntNum();i++)
	{
		mglPnt pp = gr->GetPnt(i);
		fprintf(fp,"%.2g %.2g %.2g\n",pp.x,pp.y,pp.z);
	}
	fclose(fp);

	// primitive definition
	unsigned len=strlen(fname);
	char *tname = new char[len+2];	strcpy(tname,fname);	tname[len+1]=tname[len]=0;
	tname[len]='l';	fp = fopen(tname,"wt");
	tname[len]='f';	ff = fopen(tname,"wt");
	fprintf(fp,"# Creator: MathGL library\n# Title: %s\n",descr ? descr : fname);
	fprintf(fp,"# Indices of vertices to connect for lines\n");
	fprintf(ff,"# Creator: MathGL library\n# Title: %s\n",descr ? descr : fname);
	fprintf(ff,"# Indices of vertices to connect for faces\n");
	for(i=0;i<gr->GetPrmNum();i++)
	{
		const mglPrim q=gr->GetPrm(i);
		if(q.n1<0 || q.n2<0)	continue;
		if(q.type==1)	fprintf(fp,"%ld %ld\n",q.n1+1,q.n2+1);
		if(q.type==2 && q.n3>=0)
			fprintf(ff,"%ld %ld %ld\n",q.n1+1,q.n2+1,q.n3+1);
		if(q.type==3 && q.n3>=0 && q.n4>=0)
			fprintf(ff,"%ld %ld %ld %ld\n",q.n1+1,q.n2+1,q.n3+1,q.n4+1);
	}
	fclose(fp);	fclose(ff);	delete []tname;
}
void mgl_write_xyz_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *d=new char[n+1];	memcpy(d,descr,n);	d[n]=0;
	mgl_write_xyz(_GR_,s,d);	delete []s;		delete []d;	}
//-----------------------------------------------------------------------------
void mgl_write_off(HMGL gr, const char *fname,const char *descr)
{
	register long i,nf=0;
	for(i=0;i<gr->GetPrmNum();i++)	// find number of faces
	{
		const mglPrim q=gr->GetPrm(i);
		if(q.n1<0 || q.n2<0 || q.n3<0)	continue;
		if(q.type==2 || (q.type==3 && q.n4>=0))	nf++;
	}
	if(nf<=0)	return;	// nothing to do

	FILE *fp=fopen(fname,"wt");
	// vertices definition
	fprintf(fp,"cOFF\n# Creator: MathGL library\n# Title: %s\n",descr ? descr : fname);
	fprintf(fp,"# List of Vertices, with (x,y,z,r,g,b,a) coordinates.\n");
	fprintf(fp,"%ld %ld 0\n",gr->GetPntNum(), nf);
	for(i=0;i<gr->GetPntNum();i++)
	{
		mglPnt pp = gr->GetPnt(i);
		fprintf(fp,"%.2g %.2g %.2g %.2g %.2g %.2g %.2g\n", pp.x, pp.y, pp.z, pp.r, pp.g, pp.b, pp.a);
	}
	for(i=0;i<gr->GetPrmNum();i++)
	{
		const mglPrim q=gr->GetPrm(i);
		if(q.n1<0 || q.n2<0 || q.n3<0)	continue;
		if(q.type==2)	fprintf(fp,"3 %ld %ld %ld\n",q.n1,q.n2,q.n3);
		if(q.type==3 && q.n4>=0)
			fprintf(fp,"4 %ld %ld %ld %ld\n",q.n1,q.n2,q.n3,q.n4);
	}
	fclose(fp);
}
void mgl_write_off_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *d=new char[n+1];	memcpy(d,descr,n);	d[n]=0;
	mgl_write_off(_GR_,s,d);	delete []s;		delete []d;	}
//-----------------------------------------------------------------------------
void mgl_write_idtf(HMGL gr, const char *fname,const char *descr)
{	/*_Gr_->WriteIDTF(fname,descr);*/	}	// TODO: Add idtf support later
void mgl_write_idtf_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
mgl_write_idtf(_GR_,s,f);	delete []s;		delete []f;	}
//-----------------------------------------------------------------------------
