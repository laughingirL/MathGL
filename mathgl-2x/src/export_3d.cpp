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
void mglCanvas::WriteOBJ(const char *fname,const char *descr)
{
	if(Prm.size()<=0)	return;	// nothing to do
	register unsigned long i,j;
	long m1=0,m2=0,m;
	for(i=0;i<Grp.size();i++)	// prepare array of indirect indexing
	{	m = Grp[i].Id;	if(m<m1) m1=m;	if(m>m2) m2=m;	}
	long *ng = new long[m2-m1+1];
	for(i=0;i<Grp.size();i++)	ng[Grp[i].Id-m1] = i;
	for(i=0;i<Prm.size();i++)	// collect data for groups
	// it is rather expensive (extra 4b per primitive) but need for export to 3D
	{
		m = Prm[i].id-m1;
		if(m>=0 && m<m2-m1+1)	Grp[ng[m]].p.push_back(i);
	}
	delete []ng;

	unsigned len=strlen(fname);
	char *tname = new char[len+1];	strcpy(tname,fname);
	FILE *fp=fopen(fname,"wt");
	// vertices definition
	fprintf(fp,"# Creator: MathGL library\n#Title: %s\n",descr ? descr : fname);
	fprintf(fp,"# List of Vertices, with (x,y,z[,w]) coordinates, w is optional.\n");
	for(i=0;i<Pnt.size();i++)	fprintf(fp,"v %.2g %.2g %.2g\n",Pnt[i].x,Pnt[i].y,Pnt[i].z);
	fprintf(fp,"# Texture coordinates, in (u,v[,w]) coordinates, w is optional.\n");
	for(i=0;i<Pnt.size();i++)	fprintf(fp,"vt %.2g %.2g\n",256*Pnt[i].t,256*Pnt[i].c);
	fprintf(fp,"# Normals in (x,y,z) form; normals might not be unit.\n");
	for(i=0;i<Pnt.size();i++)	fprintf(fp,"vn %.2g %.2g %.2g\n",Pnt[i].u,Pnt[i].v,Pnt[i].w);
	// primitive definition
	tname[len-4]=0;	fprintf(fp,"# Primitives Definitions\nusemtl %s\n",tname);
	std::vector<long> p;
	mglPrim q;
	for(i=0;i<Grp.size();i++)
	{
		fprintf(fp,"g %s\n",Grp[i].Lbl.c_str());	p = Grp[i].p;
		for(j=0;j<p.size();j++)
		{
			q = Prm[p[j]];
			switch(q.type)
			{
			// TODO: Add other marks drawing here
			case 0:	fprintf(fp,"p %ld\n", q.n1);	break;
			case 1:	fprintf(fp,"l %ld/%ld %ld/%ld\n", q.n1,q.n1, q.n2,q.n2);	break;
			case 2:	fprintf(fp,"f %ld/%ld/%ld %ld/%ld/%ld %ld/%ld/%ld\n",
								q.n1,q.n1,q.n1, q.n2,q.n2,q.n2, q.n3,q.n3,q.n3);	break;
			case 3:	fprintf(fp,"f %ld/%ld/%ld %ld/%ld/%ld %ld/%ld/%ld %ld/%ld/%ld\n",
								q.n1,q.n1,q.n1, q.n2,q.n2,q.n2, q.n3,q.n3,q.n3, q.n4,q.n4,q.n4);	break;
			case 4:	break;	// TODO: add glyphs export later
			case 5:	break;	// TODO: add arrows export later
			}
		}
		Grp[i].p.clear();	// we don't need indexes anymore
	}
	// try to save "ungrouped" primitives
	fprintf(fp,"g ungrouped\n");
	for(i=0;i<Prm.size();i++)
	{
		q = Prm[i];		m = q.id-m1;
		if(m>=0 && m<m2-m1+1)	continue;
		switch(q.type)	// NOTE: should be never here!
		{
		case 0:	fprintf(fp,"p %ld/%ld\n", q.n1,q.n1);	break;
		case 1:	fprintf(fp,"l %ld/%ld %ld/%ld\n", q.n1,q.n1, q.n2,q.n2);	break;
		case 2:	fprintf(fp,"f %ld/%ld/%ld %ld/%ld/%ld %ld/%ld/%ld\n",
							q.n1,q.n1,q.n1, q.n2,q.n2,q.n2, q.n3,q.n3,q.n3);	break;
		case 3:	fprintf(fp,"f %ld/%ld/%ld %ld/%ld/%ld %ld/%ld/%ld %ld/%ld/%ld\n",
							q.n1,q.n1,q.n1, q.n2,q.n2,q.n2, q.n3,q.n3,q.n3, q.n4,q.n4,q.n4);	break;
		case 4:	break;	// TODO: add glyphs export later
		case 5:	break;	// TODO: add arrows export later
		}
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
	tname[len-4]='.';	tname[len-3]='t';	tname[len-2]='g';	tname[len-1]='a';
	fprintf(fp,"map_Ka %s\nmap_Kd %s\nmap_Ks %s\n",tname,tname,tname);
	fclose(fp);
	// prepare texture file (TGA)
	fp = fopen(tname,"wb");		// TODO: Add possibility to save into PNG instead of TGA
	char head[14]={0,0,2, 0,0,0,0,0, 0,0,0,0, 32,0};
	short w = 256, h = 256*Txt.size();
	fwrite(head,12,1,fp);
	fwrite(&w,2,1,fp);	fwrite(&h,2,1,fp);
	fwrite(head+12,2,1,fp);
	unsigned char *buf = new unsigned char[256*256];
	for(i=Txt.size();i>0;i--)
	{	Txt[i-1].GetRGBA(buf);	fwrite(buf,256*256,1,fp);	}
	fclose(fp);	delete []buf;	delete []tname;

}
//-----------------------------------------------------------------------------
#undef _GR_
#define _GR_	((mglCanvas *)(*gr))
#define _Gr_	((mglCanvas *)(gr))
void mgl_write_obj(HMGL gr, const char *fname,const char *descr)
{	_Gr_->WriteOBJ(fname,descr);	}
void mgl_write_obj_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *d=new char[n+1];	memcpy(d,descr,n);	d[n]=0;
	_GR_->WriteOBJ(s,d);	delete []s;		delete []d;	}
//-----------------------------------------------------------------------------
