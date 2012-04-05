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
#include <stdio.h>
#include <time.h>
#include <zlib.h>
#include "mgl/canvas.h"
#include "mgl/canvas_cf.h"

#include "oPRCFile.h"
#include <map>
#include <utility>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <png.h>

#if MGL_HAVE_PDF
#include <stdlib.h>
#include <setjmp.h>
#include <math.h>
#include <hpdf.h>
#include <hpdf_u3d.h>
#include <hpdf_annotation.h>
#endif // MGL_HAVE_PDF


#undef _GR_
#define _GR_	((mglCanvas *)(*gr))
#define _Gr_	((mglCanvas *)(gr))
int mgl_tga_save(const char *fname, int w, int h, unsigned char **p);
int mgl_pnga_save(const char *fname, int w, int h, unsigned char **p);
void mgl_printf(void *fp, bool gz, const char *str, ...);
//-----------------------------------------------------------------------------
struct prctriangle {
	uint32_t pi[3];
	uint32_t ti[3];
};
//-----------------------------------------------------------------------------
struct prctriangles {
	prctriangles(const HMGL g) : samecolour(true), colourset(false),
	gr(g), ntxt(g->GetTxtNum()) {}
	std::map<PRCVector3d,uint32_t> points;
	std::map<PRCVector2d,uint32_t> texturecoords;
	std::vector<prctriangle> triangles;
	RGBAColour colour;
	bool samecolour;
	bool colourset;
	const HMGL gr;
	const size_t ntxt;
	
	
	uint32_t addPoint(const mglPnt& p)
	{
		const PRCVector3d point(p.x,p.y,p.z);
		
		std::map<PRCVector3d,uint32_t>::iterator pPoint = points.find(point);
		if(pPoint!=points.end())
			return pPoint->second;
		else
		{
			const uint32_t point_index = points.size();
			points.insert(std::make_pair(point,point_index));
			return point_index;
		}
	}
	
	uint32_t addPoint(float x, float y, float z)
	{
		const PRCVector3d point(x,y,z);
		
		std::map<PRCVector3d,uint32_t>::iterator pPoint = points.find(point);
		if(pPoint!=points.end())
			return pPoint->second;
		else
		{
			const uint32_t point_index = points.size();
			points.insert(std::make_pair(point,point_index));
			return point_index;
		}
	}
	
	void writePoints(double (*P)[3])
	{
		for(std::map<PRCVector3d,uint32_t>::const_iterator pPoint = points.begin(); pPoint != points.end(); pPoint++)
		{
			P[pPoint->second][0] = pPoint->first.x;
			P[pPoint->second][1] = pPoint->first.y;
			P[pPoint->second][2] = pPoint->first.z;
		}
	}
	
	uint32_t addTextureCoords(const mglPnt& p)
	{
		const float u = ((1-p.ta)*(512-2)+1)/512;
		const float v = ((1-p.c/ntxt)*(512*ntxt-2) + 1)/(512*ntxt);
		
		const PRCVector2d point(u, v);
		
		if (colourset) {
			if (samecolour) {
				if (colour.R != p.r || colour.G != p.g || colour.B != p.b || colour.A != p.a)
					samecolour = false;
			}
		}
		else {
			colour.Set(p.r, p.g, p.b, p.a);
			colourset = true;
		}
		
		
		std::map<PRCVector2d,uint32_t>::iterator pPoint = texturecoords.find(point);
		if(pPoint!=texturecoords.end())
			return pPoint->second;
		else
		{
			const uint32_t point_index = texturecoords.size();
			texturecoords.insert(std::make_pair(point,point_index));
			return point_index;
		}
	}
	
	void writeTextureCoords(double (*P)[2])
	{
		for(std::map<PRCVector2d,uint32_t>::const_iterator pPoint = texturecoords.begin(); pPoint != texturecoords.end(); pPoint++)
		{
			P[pPoint->second][0] = pPoint->first.x;
			P[pPoint->second][1] = pPoint->first.y;
		}
	}
};
//-----------------------------------------------------------------------------
/* structure to store PNG image bytes */
struct png_buf
{
	uint8_t *data;
	size_t size;
};
//-----------------------------------------------------------------------------
void my_png_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	struct png_buf* p=(struct png_buf*)png_get_io_ptr(png_ptr);
	size_t nsize = p->size + length;
	
	/* allocate or grow buffer */
	if(p->data)
		p->data = (uint8_t*)realloc(p->data, nsize);
	else
		p->data = (uint8_t*)malloc(nsize);
	
	if(!p->data)
		png_error(png_ptr, "Write Error - no mem");
	
	/* copy new bytes to end of buffer */
	memcpy(p->data + p->size, data, length);
	p->size += length;
}
//-----------------------------------------------------------------------------
void my_png_flush(png_structp png_ptr)
{
}
//-----------------------------------------------------------------------------
void mgl_write_prc(HMGL gr, const char *fname,const char *descr, int make_pdf)
{
	if(gr->GetPrmNum()<=0)	return;	// nothing to do
//	register size_t i,j;
	{
		long m1=0,m2=0,m;
		for(size_t i=0;i<gr->Grp.size();i++)	// prepare array of indirect indexing
		{	m = gr->Grp[i].Id;	if(m<m1) m1=m;	if(m>m2) m2=m;	}
		long *ng = new long[m2-m1+1];
		for(size_t i=0;i<gr->Grp.size();i++)	ng[gr->Grp[i].Id-m1] = i;
		for(size_t i=0;i<size_t(gr->GetPrmNum());i++)	// collect data for groups
		// it is rather expensive (extra 4b per primitive) but need for export to 3D
		{
			m = gr->GetPrm(i).id-m1;
			if(m>=0 && m<m2-m1+1)	gr->Grp[ng[m]].p.push_back(i);
		}
		delete []ng;
	}
	const unsigned len=strlen(fname);
	char * const tname = new char[len+9];	strcpy(tname,fname);
	if (strncmp(tname+len-4, ".prc", 4)!=0)
	{
		tname[len]='.';	tname[len+1]='p';	tname[len+2]='r';	tname[len+3]='c'; tname[len+4]='\0';
	}
	oPRCFile file(tname);
	PRCoptions grpopt;
	grpopt.tess = true;
	grpopt.closed = false; // set to true to make only front side visible
	//	grpopt.no_break = true;
	//	grpopt.do_break = false;

	uint32_t materialMathGLid = m1;
	{
		png_buf buffer;
		buffer.data = (uint8_t*)malloc(1024);;
		buffer.size = 0;
		const size_t ntxt = gr->GetTxtNum();

		// prepare texture file (PNG)
		const png_uint_32 width=256, height=256*ntxt;
		png_bytep buf = new png_byte[4*width*height];
		png_bytepp pbuf= new png_bytep[height];
		for(size_t i=0;i<height;i++)	pbuf[i] = buf+4*width*i;
		for(size_t i=0;i<ntxt;i++)	gr->GetTxt(i).GetRGBA(buf+i*256*256*4);

		png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,0,0,0);
		png_infop info_ptr = png_create_info_struct(png_ptr);

		png_set_write_fn(png_ptr, &buffer, my_png_write_data, my_png_flush);
		png_set_filter(png_ptr, 0, PNG_ALL_FILTERS);
		png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
		png_set_IHDR(png_ptr, info_ptr, width, height, 8,
					PNG_COLOR_TYPE_RGB_ALPHA,
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
				PNG_FILTER_TYPE_DEFAULT);
		png_set_rows(png_ptr, info_ptr, pbuf);
		png_write_png(png_ptr, info_ptr,	PNG_TRANSFORM_IDENTITY, 0);
		png_write_end(png_ptr, info_ptr);

		png_destroy_write_struct(&png_ptr, &info_ptr);
		delete []pbuf;	delete []buf;
		const PRCmaterial materialMathGL(
			RGBAColour(0.1,0.1,0.1,1), // ambient
			RGBAColour(1.0,1.0,1.0,1), // diffuse
			RGBAColour(0.1,0.1,0.1,1), //emissive
			RGBAColour(0.0,0.0,0.0,1), //spectral
			1.0,0.1, // alpha, shininess
			buffer.data, KEPRCPicture_PNG, 0, 0, buffer.size, true);
		materialMathGLid = file.addMaterial(materialMathGL);
		free(buffer.data); buffer.data = NULL;
	}

	// primitive definition in groups

	mglPnt p0;
	p0.x = dynamic_cast<mglCanvas *>(gr)->GetWidth()/2.;
	p0.y = dynamic_cast<mglCanvas *>(gr)->GetHeight()/2.;
	p0.z = sqrt(p0.x*p0.y);

	for(size_t i=0;i<gr->Grp.size();i++)
	{
		const	std::vector<long>& p = gr->Grp[i].p;
		prctriangles group(gr);
		file.begingroup(gr->Grp[i].Lbl.c_str(),&grpopt);
		for(size_t j=0;j<p.size();j++)
		{
			const mglPrim &q=gr->GetPrm(p[j]);
			const double w = (q.w>1)?(q.w*sqrt(gr->FontFactor()/400.)):1;

			const mglPnt p = gr->GetPnt(q.n1) - p0;
			const float size = q.s*gr->FontFactor();
			{
				char type = q.n4;	float ss=size*0.35;
				const RGBAColour c(p.r, p.g, p.b, p.a);
				switch(q.type)
				{
					case 0:
						if(!strchr("xsSoO",type))	ss *= 1.1;
						if(type=='.' || ss==0)
						{
							const double P[3] = {p.x, p.y, p.z};
							file.addPoint(P, c, w);
						}
						else
							switch(type)
							{
								case 'P':
								{
									const double P[5][3] =
									{
										{ p.x-ss,p.y-ss,p.z },
										{ p.x+ss,p.y-ss,p.z },
										{ p.x+ss,p.y+ss,p.z },
										{ p.x-ss,p.y+ss,p.z },
										{ p.x-ss,p.y-ss,p.z }
									};
									file.addLine(5, P, c, w);
								}
								case '+':
								{
									const double P1[2][3] =
									{
										{ p.x-ss,p.y,p.z },
										{ p.x+ss,p.y,p.z }
									};
									file.addLine(2, P1, c, w);
									const double P2[2][3] =
									{
										{ p.x,p.y-ss,p.z },
										{ p.x,p.y+ss,p.z }
									};
									file.addLine(2, P2, c, w);
								}
								break;
								case 'X':
								{
									const double P[5][3] =
									{
										{ p.x-ss,p.y-ss,p.z },
										{ p.x+ss,p.y-ss,p.z },
										{ p.x+ss,p.y+ss,p.z },
										{ p.x-ss,p.y+ss,p.z },
										{ p.x-ss,p.y-ss,p.z }
									};
									file.addLine(5, P, c, w);
									const double P1[2][3] =
									{
										{ p.x-ss,p.y-ss,p.z },
										{ p.x+ss,p.y+ss,p.z }
									};
									file.addLine(2, P1, c, w);
									const double P2[2][3] =
									{
										{ p.x+ss,p.y-ss,p.z },
										{ p.x-ss,p.y+ss,p.z }
									};
									file.addLine(2, P2, c, w);
								}
								break;
								case 'x':
								{
									const double P1[2][3] =
									{
										{ p.x-ss,p.y-ss,p.z },
										{ p.x+ss,p.y+ss,p.z }
									};
									file.addLine(2, P1, c, w);
									const double P2[2][3] =
									{
										{ p.x+ss,p.y-ss,p.z },
										{ p.x-ss,p.y+ss,p.z }
									};
									file.addLine(2, P2, c, w);
								}
								break;
								case 'S':
								{
									const uint32_t ti = group.addTextureCoords(p);

									const uint32_t pi1 = group.addPoint(p.x-ss,p.y-ss,p.z);
									const uint32_t pi2 = group.addPoint(p.x+ss,p.y-ss,p.z);
									const uint32_t pi3 = group.addPoint(p.x-ss,p.y+ss,p.z);
									const uint32_t pi4 = group.addPoint(p.x+ss,p.y+ss,p.z);

									prctriangle triangle1, triangle2;
									triangle1.pi[0] = pi1;
									triangle1.pi[1] = pi2;
									triangle1.pi[2] = pi3;
									triangle1.ti[0] = ti;
									triangle1.ti[1] = ti;
									triangle1.ti[2] = ti;
									group.triangles.push_back(triangle1);
									triangle2.pi[0] = pi4;
									triangle2.pi[1] = pi3;
									triangle2.pi[2] = pi2;
									triangle2.ti[0] = ti;
									triangle2.ti[1] = ti;
									triangle2.ti[2] = ti;
									group.triangles.push_back(triangle2);
								}
								break;
								case 's':
								{
									const double P[5][3] =
									{
										{ p.x-ss,p.y-ss,p.z },
										{ p.x+ss,p.y-ss,p.z },
										{ p.x+ss,p.y+ss,p.z },
										{ p.x-ss,p.y+ss,p.z },
										{ p.x-ss,p.y-ss,p.z }
									};
									file.addLine(5, P, c, w);
								}
								break;
								case 'D':
								{
									const uint32_t ti = group.addTextureCoords(p);

									const uint32_t pi1 = group.addPoint(p.x,p.y-ss,p.z);
									const uint32_t pi2 = group.addPoint(p.x+ss,p.y,p.z);
									const uint32_t pi3 = group.addPoint(p.x-ss,p.y,p.z);
									const uint32_t pi4 = group.addPoint(p.x,p.y+ss,p.z);

									prctriangle triangle1, triangle2;
									triangle1.pi[0] = pi1;
									triangle1.pi[1] = pi2;
									triangle1.pi[2] = pi3;
									triangle1.ti[0] = ti;
									triangle1.ti[1] = ti;
									triangle1.ti[2] = ti;
									group.triangles.push_back(triangle1);
									triangle2.pi[0] = pi4;
									triangle2.pi[1] = pi3;
									triangle2.pi[2] = pi2;
									triangle2.ti[0] = ti;
									triangle2.ti[1] = ti;
									triangle2.ti[2] = ti;
									group.triangles.push_back(triangle2);
								}
								break;
								case 'd':
								{
									const double P[5][3] =
									{
										{ p.x,p.y-ss,p.z },
										{ p.x+ss,p.y,p.z },
										{ p.x,p.y+ss,p.z },
										{ p.x-ss,p.y,p.z },
										{ p.x,p.y-ss,p.z }
									};
									file.addLine(5, P, c, w);
								}
								break;
								case 'Y':
								{
									const double P1[3][3] =
									{
										{ p.x,			 p.y-ss,		p.z },
										{ p.x,			 p.y,			 p.z },
										{ p.x+0.8*ss,p.y+0.6*ss,p.z }
									};
									file.addLine(3, P1, c, w);
									const double P2[2][3] =
									{
										{ p.x,			 p.y,			 p.z },
										{ p.x-0.8*ss,p.y+0.6*ss,p.z }
									};
									file.addLine(2, P2, c, w);
								}
								break;
								case '*':
								{
									const double P1[2][3] =
									{
										{ p.x-ss,p.y,p.z },
										{ p.x+ss,p.y,p.z }
									};
									file.addLine(2, P1, c, w);
									const double P2[2][3] =
									{
										{ p.x-0.6*ss,p.y-0.8*ss,p.z },
										{ p.x+0.6*ss,p.y+0.8*ss,p.z }
									};
									file.addLine(2, P2, c, w);
									const double P3[2][3] =
									{
										{ p.x-0.6*ss,p.y+0.8*ss,p.z },
										{ p.x+0.6*ss,p.y-0.8*ss,p.z }
									};
									file.addLine(2, P3, c, w);

								}
								break;
								case 'T':
								{
									const uint32_t ti = group.addTextureCoords(p);

									const uint32_t pi1 = group.addPoint(p.x-ss,p.y-ss/2,p.z);
									const uint32_t pi2 = group.addPoint(p.x+ss,p.y-ss/2,p.z);
									const uint32_t pi3 = group.addPoint(p.x,p.y+ss,p.z);

									prctriangle triangle;
									triangle.pi[0] = pi1;
									triangle.pi[1] = pi2;
									triangle.pi[2] = pi3;
									triangle.ti[0] = ti;
									triangle.ti[1] = ti;
									triangle.ti[2] = ti;
									group.triangles.push_back(triangle);
								}
								break;
								case '^':
								{
									const double P[4][3] =
									{
										{ p.x-ss,p.y-ss/2,p.z },
										{ p.x+ss,p.y-ss/2,p.z },
										{ p.x,	 p.y+ss,	p.z },
										{ p.x-ss,p.y-ss/2,p.z }
									};
									file.addLine(4, P, c, w);
								}
								break;
								case 'V':
								{
									const uint32_t ti = group.addTextureCoords(p);

									const uint32_t pi1 = group.addPoint(p.x-ss,p.y+ss/2,p.z);
									const uint32_t pi2 = group.addPoint(p.x,p.y-ss,p.z);
									const uint32_t pi3 = group.addPoint(p.x+ss,p.y+ss/2,p.z);

									prctriangle triangle;
									triangle.pi[0] = pi1;
									triangle.pi[1] = pi2;
									triangle.pi[2] = pi3;
									triangle.ti[0] = ti;
									triangle.ti[1] = ti;
									triangle.ti[2] = ti;
									group.triangles.push_back(triangle);
								}
								break;
								case 'v':
								{
									const double P[4][3] =
									{
										{ p.x-ss,p.y+ss/2,p.z },
										{ p.x+ss,p.y+ss/2,p.z },
										{ p.x,	 p.y-ss,	p.z },
										{ p.x-ss,p.y+ss/2,p.z }
									};
									file.addLine(4, P, c, w);
								}
								break;
								case 'L':
								{
									const uint32_t ti = group.addTextureCoords(p);

									const uint32_t pi1 = group.addPoint(p.x+ss/2,p.y+ss,p.z);
									const uint32_t pi2 = group.addPoint(p.x-ss,	p.y,	 p.z);
									const uint32_t pi3 = group.addPoint(p.x+ss/2,p.y-ss,p.z);

									prctriangle triangle;
									triangle.pi[0] = pi1;
									triangle.pi[1] = pi2;
									triangle.pi[2] = pi3;
									triangle.ti[0] = ti;
									triangle.ti[1] = ti;
									triangle.ti[2] = ti;
									group.triangles.push_back(triangle);
								}
								break;
								case '<':
								{
									const double P[4][3] =
									{
										{ p.x+ss/2,p.y+ss,p.z },
										{ p.x+ss/2,p.y-ss,p.z },
										{ p.x-ss,	p.y,	 p.z },
										{ p.x+ss/2,p.y+ss,p.z }
									};
									file.addLine(4, P, c, w);
								}
								break;
								case 'R':
								{
									const uint32_t ti = group.addTextureCoords(p);

									const uint32_t pi1 = group.addPoint(p.x-ss/2,p.y+ss,p.z);
									const uint32_t pi2 = group.addPoint(p.x-ss/2,p.y-ss,p.z);
									const uint32_t pi3 = group.addPoint(p.x+ss,	p.y,	 p.z);

									prctriangle triangle;
									triangle.pi[0] = pi1;
									triangle.pi[1] = pi2;
									triangle.pi[2] = pi3;
									triangle.ti[0] = ti;
									triangle.ti[1] = ti;
									triangle.ti[2] = ti;
									group.triangles.push_back(triangle);
								}
								break;
								case '>':
								{
									const double P[4][3] =
									{
										{ p.x-ss/2,p.y+ss,p.z },
										{ p.x-ss/2,p.y-ss,p.z },
										{ p.x+ss,	p.y,	 p.z },
										{ p.x-ss/2,p.y+ss,p.z }
									};
									file.addLine(4, P, c, w);
								}
								break;
								case 'O':
								{
									const uint32_t ti = group.addTextureCoords(p);

									const uint32_t cpi=group.addPoint(p);
									uint32_t pi[21];
									for(size_t j=0;j<=20;j++)
										pi[j]=group.addPoint(p.x+ss*cos(j*M_PI/10),p.y+ss*sin(j*M_PI/10),p.z);
									for(size_t j=0;j<=20;j++) {
										prctriangle triangle;
										triangle.pi[0] = pi[j];
										triangle.pi[1] = pi[j+1];
										triangle.pi[2] = cpi;
										triangle.ti[0] = ti;
										triangle.ti[1] = ti;
										triangle.ti[2] = ti;
										group.triangles.push_back(triangle);
									}
								}
								break;
								case 'C':
								{
									const double P[3] = {p.x, p.y, p.z};
									file.addPoint(P, c, w);
								}
								case 'o':
								{
									double P[21][3];
									for(size_t j=0;j<=20;j++) {
										P[j][0] = p.x+ss*cos(j*M_PI/10);
										P[j][1] = p.y+ss*sin(j*M_PI/10);
										P[j][2] = p.z;
									}
									file.addLine(21, P, c, w);
								}
								break;
							}
							break;

								case 1:
								{
									const mglPnt p1 = gr->GetPnt(q.n1) - p0, p2 = gr->GetPnt(q.n2) - p0;

									const uint32_t n = 2;
									double P[2][3];
									P[0][0] = p1.x;
									P[0][1] = p1.y;
									P[0][2] = p1.z;
									P[1][0] = p2.x;
									P[1][1] = p2.y;
									P[1][2] = p2.z;
									const RGBAColour colour((p1.r+p2.r)/2, (p1.g+p2.g)/2, (p1.b+p2.b)/2, (p1.a+p2.a)/2);
									file.addLine(n, P, colour, w);
								}
								break;

								case 2:
								{
									const mglPnt p1 = gr->GetPnt(q.n1)	- p0, p2 = gr->GetPnt(q.n2) - p0, p3 = gr->GetPnt(q.n3) - p0;

									prctriangle triangle;
									triangle.pi[0] = group.addPoint(p1);
									triangle.pi[1] = group.addPoint(p2);
									triangle.pi[2] = group.addPoint(p3);
									triangle.ti[0] = group.addTextureCoords(p1);
									triangle.ti[1] = group.addTextureCoords(p2);
									triangle.ti[2] = group.addTextureCoords(p3);
									group.triangles.push_back(triangle);
								}
								break;
								case 3:
								{
									const mglPnt p1 = gr->GetPnt(q.n1) - p0;
									const uint32_t pi1 = group.addPoint(p1);
									const uint32_t ti1 = group.addTextureCoords(p1);

									const mglPnt p2 = gr->GetPnt(q.n2) - p0;
									const uint32_t pi2 = group.addPoint(p2);
									const uint32_t ti2 = group.addTextureCoords(p2);

									const mglPnt p3 = gr->GetPnt(q.n3) - p0;
									const uint32_t pi3 = group.addPoint(p3);
									const uint32_t ti3 = group.addTextureCoords(p3);

									const mglPnt p4 = gr->GetPnt(q.n4) - p0;
									const uint32_t pi4 = group.addPoint(p4);
									const uint32_t ti4 = group.addTextureCoords(p4);

									prctriangle triangle1, triangle2;
									triangle1.pi[0] = pi1;
									triangle1.pi[1] = pi2;
									triangle1.pi[2] = pi3;
									triangle1.ti[0] = ti1;
									triangle1.ti[1] = ti2;
									triangle1.ti[2] = ti3;
									group.triangles.push_back(triangle1);
									triangle2.pi[0] = pi4;
									triangle2.pi[1] = pi3;
									triangle2.pi[2] = pi2;
									triangle2.ti[0] = ti4;
									triangle2.ti[1] = ti3;
									triangle2.ti[2] = ti2;
									group.triangles.push_back(triangle2);
								}
								break;
								case 4:	break;	// TODO: add glyphs export later
				}
			}
		}
		if (!group.triangles.empty()) {
			const uint32_t nP = group.points.size();
			double (*P)[3] = new double[nP][3];
			group.writePoints(P);
			const uint32_t nI = group.triangles.size();
			uint32_t (*PI)[3] = new uint32_t[nI][3];
			for(uint32_t i = 0; i<nI; i++)
			{
				PI[i][0] = group.triangles[i].pi[0];
				PI[i][1] = group.triangles[i].pi[1];
				PI[i][2] = group.triangles[i].pi[2];
			}
			if (!group.samecolour) {
				const uint32_t nT = group.texturecoords.size();
				double (*T)[2] = new double[nT][2];
				group.writeTextureCoords(T);
				uint32_t (*TI)[3] = new uint32_t[nI][3];
				for(uint32_t i = 0; i<nI; i++)
				{
					TI[i][0] = group.triangles[i].ti[0];
					TI[i][1] = group.triangles[i].ti[1];
					TI[i][2] = group.triangles[i].ti[2];
				}
				const uint32_t tess_index = file.createTriangleMesh(nP, P, nI, PI, m1, 0, NULL, NULL, nT, T, TI, 0, NULL, NULL, 0, NULL, NULL);
				file.useMesh(tess_index, materialMathGLid);
				delete [] TI;
				delete [] T;
			} else {
				const uint32_t tess_index = file.createTriangleMesh(nP, P, nI, PI, m1, 0, NULL, NULL, 0, NULL, NULL, 0, NULL, NULL, 0, NULL, NULL);

				const PRCmaterial material(
					RGBAColour(0.1,0.1,0.1,1), // ambient
					group.colour, // diffuse
					RGBAColour(0.1,0.1,0.1,1), //emissive
					RGBAColour(0.0,0.0,0.0,1), //spectral
					group.colour.A,0.1); // alpha, shininess
				file.useMesh(tess_index, file.addMaterial(material));
			}
			delete [] PI;
			delete [] P;
		}
		file.endgroup();
		gr->Grp[i].p.clear();	// we don't need indexes anymore
	}
	file.finish();

	if (make_pdf) {
#if MGL_HAVE_PDF
		const HPDF_REAL width	= dynamic_cast<mglCanvas *>(gr)->GetWidth();
		const HPDF_REAL height = dynamic_cast<mglCanvas *>(gr)->GetHeight();
		const HPDF_REAL depth	= sqrt(width*height);

		const HPDF_Rect rect = {0, 0, width, height};

		HPDF_Doc	pdf;
		HPDF_Page page;
		HPDF_Annotation annot;
		HPDF_U3D u3d;

		HPDF_Dict view;
		pdf = HPDF_New (NULL, NULL);

		pdf->pdf_version = HPDF_VER_17;

		page = HPDF_AddPage (pdf);

		HPDF_Page_SetWidth (page, width);
		HPDF_Page_SetHeight (page, height);

		u3d = HPDF_LoadU3DFromFile (pdf, tname);

		//	Default view
		view = HPDF_Create3DView (u3d->mmgr, "DefaultView");

		//	Position camera
		HPDF_3DView_SetCamera (view, 0, 0, 0, 0, 0, 1, depth, 0);

		//	Set ortho projection
		HPDF_3DView_SetOrthogonalProjection (view, 1);

		//	Background color
		HPDF_3DView_SetBackgroundColor (view, 0.9, 0.9, 0.9);

		//	Lighting
		HPDF_3DView_SetLighting (view, "CAD");

		//	Add views
		HPDF_U3D_Add3DView (u3d, view);
		HPDF_U3D_SetDefault3DView(u3d, "DefaultView");

		//	Create annotation
		annot = HPDF_Page_Create3DAnnot (page, rect, u3d );

		/* save the document to a file */
		const size_t len = strlen(tname);
		tname[len-2]='p';	tname[len-2]='d';	tname[len-1]='f';
		HPDF_SaveToFile (pdf, tname);

		/* clean up */
		HPDF_Free (pdf);
#else
		const size_t len = strlen(tname);
		tname[len-2]='p';	tname[len-2]='d';	tname[len-1]='f';
		tname[len+0]='.';	tname[len+1]='t';	tname[len+2]='x';	tname[len+3]='t'; tname[len+4]='\0';
		FILE *fp=fopen(tname,"wt");
		fputs("Can not produce PDF file, MathGL compiled without PDF output support\n", fp);
		fclose(fp);
#endif // MGL_HAVE_PDF
	}
	delete []tname;
}
//-----------------------------------------------------------------------------
void mgl_write_prc_(uintptr_t *gr, const char *fname,const char *descr, int *make_pdf,int l,int n)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	mgl_write_prc(_GR_,s,f,*make_pdf);	delete []s;		delete []f;	}
//-----------------------------------------------------------------------------
