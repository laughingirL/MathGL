/***************************************************************************
 * data_png.cpp is part of Math Graphic Library
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
#include <stdlib.h>
#ifndef NO_PNG
#include <png.h>
#endif
#include "mgl/data.h"
//-----------------------------------------------------------------------------
long mgl_col_dif(unsigned char *c1,unsigned char *c2,bool sum)
{
	long res,d1=abs(long(c1[0])-long(c2[0])),
		d2=abs(long(c1[1])-long(c2[1])),d3=abs(long(c1[2])-long(c2[2]));
	if(sum)	res = d1+d2+d3;
	else	res = mgl_max(d1,mgl_max(d2,d3));
	return res;
}
//-----------------------------------------------------------------------------
unsigned char *mgl_create_scheme(const char *scheme,long &num)
{
	unsigned char *c=0,*cc=new unsigned char[3*strlen(scheme)+3];
	long nc=1,np=0;
	register long i,j;
	mglColor col;
	for(i=0;i<long(strlen(scheme));i++)
	{
		col = mglColor(scheme[i]);
		if(col.Valid())
		{	cc[3*np]=col.r;	cc[3*np+1]=col.g;	cc[3*np+2]=col.b;	np++;	}
	}
	if(np<2)	{	delete []cc;	return 0;	}
	for(i=0;i<np-1;i++)	nc+=mgl_col_dif(cc+3*i,cc+3*i+3,false);
	c = new unsigned char[3*nc+3];
	long dd,pos=0;
	for(i=0;i<np-1;i++)
	{
		dd=mgl_col_dif(cc+3*i,cc+3*i+3,false);
		for(j=0;j<dd;j++)
		{
			c[3*(pos+j)] = cc[3*i]+(cc[3*i+3]-cc[3*i])*j/dd;
			c[3*(pos+j)+1] = cc[3*i+1]+(cc[3*i+4]-cc[3*i+1])*j/dd;
			c[3*(pos+j)+2] = cc[3*i+2]+(cc[3*i+5]-cc[3*i+2])*j/dd;
		}
		pos += dd;
	}
	memcpy(c+3*nc-3,cc+3*np-3,3*sizeof(unsigned char));
	delete []cc;
	num=nc;
	return c;
}
//-----------------------------------------------------------------------------
void mgl_data_import(HMDT d, const char *fname, const char *scheme,float v1,float v2)
{
	if(v1>=v2)	return;
	long num=0;
#ifndef NO_PNG
	FILE *fp = fopen(fname, "rb");
	if (!fp)	return;
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if (!png_ptr)	{	fclose(fp);	return;	}
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{	png_destroy_read_struct(&png_ptr,0,0);	fclose(fp);	return;	}
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{	png_destroy_read_struct(&png_ptr,&info_ptr,0);	fclose(fp);	return;	}

	png_init_io(png_ptr, fp);
	png_read_png(png_ptr, info_ptr,
		PNG_TRANSFORM_STRIP_ALPHA|PNG_TRANSFORM_PACKING|
		PNG_TRANSFORM_STRIP_16|PNG_TRANSFORM_EXPAND,0);
	unsigned char **rows = png_get_rows(png_ptr, info_ptr);
	unsigned char *c = mgl_create_scheme(scheme,num);
	if(num>1)
	{
		long w=png_get_image_width(png_ptr, info_ptr);
		long h=png_get_image_height(png_ptr, info_ptr);
		d->Create(w,h,1);
		register long i,j,k;
		long pos=0,val,mval=256;
		for(i=0;i<d->ny;i++)	for(j=0;j<d->nx;j++)
		{
			for(mval=256,k=0;k<num;k++)
			{
				val = mgl_col_dif(c+3*k,rows[d->ny-i-1]+3*j,true);
				if(val==0)	{	pos=k;	break;	}
				if(val<mval)	{	pos=k;	mval=val;	}
			}
			d->a[j+d->nx*i] = v1 + pos*(v2-v1)/num;
		}
	}
	delete []c;
	png_destroy_read_struct(&png_ptr, &info_ptr,&end_info);
	fclose(fp);
#endif
}
//-----------------------------------------------------------------------------
void mgl_data_export(HCDT dd, const char *fname, const char *scheme,float v1,float v2,long ns)
{
	register long i,j,k;
	long nx=dd->GetNx(), ny=dd->GetNy(), nz=dd->GetNz();
	mreal vv;
	if(v1>v2)	return;
	if(ns<0 || ns>=nz)	ns=0;
	if(v1==v2)
	{
		v1 = 1e20;	v2=-1e20;
		for(i=0;i<nx*ny*nz;i++)
		{	vv = dd->vthr(i);	if(vv<v1)	v1=vv;	if(vv>v2)	v2=vv;	}
	}
	if(v1==v2)	return;
#ifndef NO_PNG
	long num=0;
	unsigned char *c = mgl_create_scheme(scheme,num);
	if(num<2)	{	delete []c;		return;		}

	unsigned char **p = (unsigned char **)malloc(ny*sizeof(unsigned char *));
	unsigned char *d = (unsigned char *)malloc(3*nx*ny*sizeof(unsigned char));
	for(i=0;i<ny;i++)	p[i] = d+3*nx*(ny-1-i);
	for(i=0;i<ny;i++)	for(j=0;j<nx;j++)
	{
		k = long(num*(dd->v(j,i,ns)-v1)/(v2-v1));
		if(k<0)	k=0;	if(k>=num) k=num-1;
		memcpy(d+3*(j+i*nx),c+3*k,3*sizeof(unsigned char));
	}
	delete []c;

	FILE *fp = fopen(fname, "wb");
	if (!fp)	return;
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if (!png_ptr)	{	fclose(fp);	return;	}
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{	png_destroy_write_struct(&png_ptr,0);	fclose(fp);	return;	}
	png_init_io(png_ptr, fp);
	png_set_filter(png_ptr, 0, PNG_ALL_FILTERS);
	png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
	png_set_IHDR(png_ptr, info_ptr, nx, ny, 8, PNG_COLOR_TYPE_RGB,
			PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_set_rows(png_ptr, info_ptr, p);
	png_write_png(png_ptr, info_ptr,  PNG_TRANSFORM_IDENTITY, 0);
	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);	free(p);	free(d);
#endif
}
//-----------------------------------------------------------------------------
void mgl_data_export_(uintptr_t *d, const char *fname, const char *scheme,float *v1,float *v2,int *ns,int l,int n)
{
	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,scheme,n);	f[n]=0;
	mgl_data_export(_DT_,s,f,*v1,*v2,*ns);
	delete []s;		delete []f;
}
void mgl_data_import_(uintptr_t *d, const char *fname, const char *scheme,float *v1,float *v2,int l,int n)
{
	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,scheme,n);	f[n]=0;
	mgl_data_import(_DT_,s,f,*v1,*v2);
	delete []s;		delete []f;
}
//-----------------------------------------------------------------------------
