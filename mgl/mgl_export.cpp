/* mgl_export.cpp is part of Math Graphic Library
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
#include <stdlib.h>
#include <stdio.h>
#include <png.h>

#ifndef WIN32
#include <ltdl.h>
#endif

#include "mgl/mgl.h"
#include "mgl/mgl_c.h"
#include "mgl/mgl_f.h"

int mgl_bmp_save(const char *fname, int w, int h, unsigned char **p);
int mgl_png_save(const char *fname, int w, int h, unsigned char **p);
int mgl_pnga_save(const char *fname, int w, int h, unsigned char **p);

int mglGraph::NumUseSave = 0;
mgl_save mglGraph::JPEGSave = 0;
mgl_save mglGraph::TIFFSave = 0;
mgl_save mglGraph::PNGASave = mgl_pnga_save;
mgl_save mglGraph::PNGSave = mgl_png_save;
mgl_save mglGraph::BMPSave = mgl_bmp_save;
void *mglGraph::jmodule = 0;
void *mglGraph::tmodule = 0;
//-----------------------------------------------------------------------------
void mglGraph::InitSaveFunc()
{
	NumUseSave++;
#ifndef WIN32
	if(!JPEGSave)
	{
		jmodule = 0;
		int errors=lt_dlinit();
		if(!errors)	errors=lt_dlsetsearchpath(MOD_LIB_DIR);
		if(!errors)	jmodule=lt_dlopenext("mgl-jpeg.so");
		if(jmodule)	JPEGSave = (mgl_save) lt_dlsym((lt_dlhandle) jmodule, "mgl_jpeg_save");
	}
	if(!TIFFSave)
	{
		tmodule = 0;
		int errors=lt_dlinit();
		if(!errors)	errors=lt_dlsetsearchpath(MOD_LIB_DIR);
		if(!errors)	tmodule=lt_dlopenext("mgl-tiff.so");
		if(tmodule)	TIFFSave = (mgl_save) lt_dlsym((lt_dlhandle) tmodule, "mgl_tiff_save");
	}
#endif
}
//-----------------------------------------------------------------------------
void mglGraph::FreeSaveFunc()
{
	NumUseSave--;
	if(NumUseSave<1)
	{
#ifndef WIN32
		if(JPEGSave)
		{
			if(jmodule)	lt_dlclose((lt_dlhandle) jmodule);
			lt_dlexit();	JPEGSave = 0;	jmodule = 0;
		}
		if(TIFFSave)
		{
			if(tmodule)	lt_dlclose((lt_dlhandle) tmodule);
			lt_dlexit();	TIFFSave = 0;	tmodule = 0;
		}
#endif
	}
}
//-----------------------------------------------------------------------------
unsigned char **mglGraph::GetRGBLines(long &w, long &h, unsigned char *&f, bool )
{	f=0;	return 0;	}
//-----------------------------------------------------------------------------
void mglGraph::WriteEPS(const char *,const char *){}
void mglGraph::WriteSVG(const char *,const char *){}
//-----------------------------------------------------------------------------
void mglGraph::WriteTIFF(const char *fname,const char *descr,int )
{
	if(!TIFFSave)	SetWarn(mglWarnFmt,"TIFF");
	else
	{
		long w,h;
		unsigned char *f=0, **p=0;
		p = GetRGBLines(w,h,f);
		if(p)
		{
			if(TIFFSave(fname,w,h,p))	SetWarn(mglWarnOpen,fname);
			free(p);	if(f)	free(f);
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraph::WriteJPEG(const char *fname,const char *)
{
	if(!JPEGSave)	SetWarn(mglWarnFmt,"JPEG");
	else
	{
		long w,h;
		unsigned char *f=0, **p=0;
		p = GetRGBLines(w,h,f);
		if(p)
		{
			if(JPEGSave(fname,w,h,p))	SetWarn(mglWarnOpen,fname);
			free(p);	if(f)	free(f);
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraph::WritePNG(const char *fname,const char *,bool alpha)
{
	mgl_save func = alpha ? PNGASave : PNGSave;
	if(!func)	SetWarn(mglWarnFmt,"PNG");
	else
	{
		long w,h;
		unsigned char *f=0, **p=0;
		p = GetRGBLines(w,h,f,alpha);
		if(p)
		{
			if(func(fname,w,h,p))	SetWarn(mglWarnOpen,fname);
			free(p);	if(f)	free(f);
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraph::WriteBMP(const char *fname,const char *)
{
	if(!BMPSave)	SetWarn(mglWarnFmt,"BMP");
	else
	{
		long w,h;
		unsigned char *f=0, **p=0;
		p = GetRGBLines(w,h,f);
		if(p)
		{
			if(BMPSave(fname,w,h,p))	SetWarn(mglWarnOpen,fname);
			free(p);	if(f)	free(f);
		}
	}
}
//-----------------------------------------------------------------------------
int mgl_pnga_save(const char *fname, int w, int h, unsigned char **p)
{
	FILE *fp = fopen(fname, "wb");
	if (!fp)	return 1;

	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,0,0,0);
	if (!png_ptr)	return 1;
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{	png_destroy_write_struct(&png_ptr,0);	return 1;	}

	png_init_io(png_ptr, fp);
	png_set_filter(png_ptr, 0, PNG_ALL_FILTERS);
	png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
	png_set_IHDR(png_ptr, info_ptr, w, h, 8,
				PNG_COLOR_TYPE_RGB_ALPHA,
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
				PNG_FILTER_TYPE_DEFAULT);
	png_set_rows(png_ptr, info_ptr, p);
	png_write_png(png_ptr, info_ptr,  PNG_TRANSFORM_IDENTITY, 0);
	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);
	return 0;
}
//-----------------------------------------------------------------------------
int mgl_png_save(const char *fname, int w, int h, unsigned char **p)
{
	FILE *fp = fopen(fname, "wb");
	if (!fp)	return 1;

	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,0,0,0);
	if (!png_ptr)	return 1;
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{	png_destroy_write_struct(&png_ptr,0);	return 1;	}

	png_init_io(png_ptr, fp);
	png_set_filter(png_ptr, 0, PNG_ALL_FILTERS);
	png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
	png_set_IHDR(png_ptr, info_ptr, w, h, 8,
				PNG_COLOR_TYPE_RGB,
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
				PNG_FILTER_TYPE_DEFAULT);
	png_set_rows(png_ptr, info_ptr, p);
	png_write_png(png_ptr, info_ptr,  PNG_TRANSFORM_IDENTITY, 0);
	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);
	return 0;
}
//-----------------------------------------------------------------------------
int mgl_bmp_save(const char *fname, int w, int h, unsigned char **p)
{
	FILE *fp = fopen(fname,"wb");
	if (!fp)	return 1;

	char z[4] = {0,0,0,0};
	unsigned u = w*h*3 + 54;
	// BITMAPFILEHEADER
	fwrite("BM",2,1,fp);	fwrite(&u,4,1,fp);
	fwrite(z,4,1,fp);	u=54;	fwrite(&u,4,1,fp);
	// BITMAPINFOHEADER
	u=40;	fwrite(&u,4,1,fp);	fwrite(&(w),4,1,fp);	fwrite(&(h),4,1,fp);
	unsigned short pp=1;
	fwrite(&pp,2,1,fp);	pp=24;	fwrite(&pp,2,1,fp);	u = w*h*3;
	fwrite(z,4,1,fp);	fwrite(&u,4,1,fp);
	fwrite(z,4,1,fp);	fwrite(z,4,1,fp);
	fwrite(z,4,1,fp);	fwrite(z,4,1,fp);
	// image
	for(int i=0;i<h;i++)	fwrite(p[i],3*w,1,fp);
	fclose(fp);
	return 0;
}
//-----------------------------------------------------------------------------
/// Write the frame in file using PNG format
void mgl_write_png(HMGL gr, const char *fname,const char *descr)
{	gr->WritePNG(fname,descr);	}
/// Write the frame in file using PNG format
void mgl_write_png_(long *gr, const char *fname,const char *descr,int l,int n)
{
	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	_GR_->WritePNG(s,f);	delete []s;		delete []f;
}
/// Write the frame in file using PNG format
void mgl_write_png_solid(HMGL gr, const char *fname,const char *descr)
{	gr->WritePNG(fname,descr,false);	}
/// Write the frame in file using PNG format
void mgl_write_png_solid_(long *gr, const char *fname,const char *descr,int l,int n)
{
	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	_GR_->WritePNG(s,f,false);	delete []s;		delete []f;
}
//-----------------------------------------------------------------------------
/// Write the frame in file using TIFF format
void mgl_write_tif(HMGL gr, const char *fname,const char *descr)
{	gr->WriteTIFF(fname,descr);	}
#include <string.h>
/// Write the frame in file using TIFF format
void mgl_write_tif_(long *gr, const char *fname,const char *descr,int l,int n)
{
	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	_GR_->WriteTIFF(s,f);	delete []s;		delete []f;
}
//-----------------------------------------------------------------------------
/// Write the frame in file using JPEG format
void mgl_write_jpg(HMGL gr, const char *fname,const char *descr)
{	gr->WriteJPEG(fname,descr);	}
#include <string.h>
/// Write the frame in file using JPEG format
void mgl_write_jpg_(long *gr, const char *fname,const char *descr,int l,int n)
{
	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	_GR_->WriteJPEG(s,f);	delete []s;		delete []f;
}
//-----------------------------------------------------------------------------
