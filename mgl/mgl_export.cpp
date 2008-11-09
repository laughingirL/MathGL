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
#include <png.h>

#ifdef HAVE_JPEG
extern "C" {
#include <jpeglib.h>
}
int mgl_jpeg_save(const char *fname, int w, int h, unsigned char **p);
#endif

#include "mgl/mgl.h"
#include "mgl/mgl_c.h"
#include "mgl/mgl_f.h"

int mgl_bps_save(const char *fname, int w, int h, unsigned char **p);
int mgl_bmp_save(const char *fname, int w, int h, unsigned char **p);
int mgl_png_save(const char *fname, int w, int h, unsigned char **p);
int mgl_pnga_save(const char *fname, int w, int h, unsigned char **p);
//-----------------------------------------------------------------------------
unsigned char **mglGraph::GetRGBLines(long &w, long &h, unsigned char *&f, bool )
{	f=0;	return 0;	}
//-----------------------------------------------------------------------------
void mglGraph::WriteSVG(const char *,const char *){}
void mglGraph::WriteIDTF(const char *,const char *){}
void mglGraph::VertexColor(bool enable)  { };
void mglGraph::Compression(bool enable)  { };
void mglGraph::Unrotate(bool enable)  { };
void mglGraph::StartGroup ( const char *name ) { };
void mglGraph::StartAutoGroup ( const char *name ) { };
void mglGraph::EndGroup() { };
//-----------------------------------------------------------------------------
void mglGraph::WriteJPEG(const char *fname,const char *)
{
#ifdef HAVE_JPEG
	long w,h;
	unsigned char *f=0, **p=0;
	p = GetRGBLines(w,h,f);
	if(p)
	{
		if(mgl_jpeg_save(fname,w,h,p))	SetWarn(mglWarnOpen,fname);
		free(p);	if(f)	free(f);
	}
#else
	SetWarn(mglWarnFmt,"JPEG");
#endif
}
//-----------------------------------------------------------------------------
void mglGraph::WritePNG(const char *fname,const char *,bool alpha)
{
	long w,h;
	unsigned char *f=0, **p=0;
	p = GetRGBLines(w,h,f,alpha);
	if(p)
	{
		if(alpha && mgl_pnga_save(fname,w,h,p))	SetWarn(mglWarnOpen,fname);
		if(!alpha && mgl_png_save(fname,w,h,p))	SetWarn(mglWarnOpen,fname);
		free(p);	if(f)	free(f);
	}
}
//-----------------------------------------------------------------------------
void mglGraph::WriteBMP(const char *fname,const char *)
{
	long w,h;
	unsigned char *f=0, **p=0;
	p = GetRGBLines(w,h,f);
	if(p)
	{
		if(mgl_bmp_save(fname,w,h,p))	SetWarn(mglWarnOpen,fname);
		free(p);	if(f)	free(f);
	}
}
//-----------------------------------------------------------------------------
void mglGraph::WriteEPS(const char *fname,const char *)
{
	long w,h;
	unsigned char *f=0, **p=0;
	p = GetRGBLines(w,h,f);
	if(p)
	{
		if(mgl_bps_save(fname,w,h,p))	SetWarn(mglWarnOpen,fname);
		free(p);	if(f)	free(f);
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
//	for(int i=h-1;i>=0;i--)	fwrite(p[i],3*w,1,fp);
	register int i,j;
	for(i=h-1;i>=0;i--)	for(j=0;j<w;j++)
	{
		fwrite(p[i]+3*j+2,1,1,fp);
		fwrite(p[i]+3*j+1,1,1,fp);
		fwrite(p[i]+3*j+0,1,1,fp);
	}
	fclose(fp);
	return 0;
}
//-----------------------------------------------------------------------------
#ifdef HAVE_JPEG
int mgl_jpeg_save(const char *fname, int w, int h, unsigned char **p)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE * outfile;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	if((outfile = fopen(fname, "wb")) == 0)	return 1;
	jpeg_stdio_dest(&cinfo, outfile);
	cinfo.image_width = w;
	cinfo.image_height = h;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	jpeg_start_compress(&cinfo, TRUE);

    jpeg_write_scanlines(&cinfo, p, h);

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose(outfile);
	return 0;
}
#endif
//-----------------------------------------------------------------------------
int mgl_bps_save(const char *fname, int w, int h, unsigned char **p)
{
	time_t now;
	time(&now);
	register long i,j;

	FILE *fp = fopen(fname,"wt");
	fprintf(fp,"%%!PS-Adobe-3.0 EPSF-3.0\n%%%%BoundingBox: 0 0 %d %d\n",w,h);
	fprintf(fp,"%%%%Creator: MathGL library\n%%%%Title: %s\n", fname);
	fprintf(fp,"%%%%CreationDate: %s\n",ctime(&now));
//	fprintf(fp,"%%%%EndComments\n\n");
	fprintf(fp,"%d %d 8 [1 0 0 1 0 0] {currentfile %d string readhexstring pop} false 3 colorimage\n",
			w,h,1+w*h/40);
	for(j=0;j<h;j++)	for(i=0;i<w;i++)
	{
		if((i+w*(h-j-1))%40==0 && i+j>0)	fprintf(fp,"\n");
		fprintf(fp,"%02x%02x%02x",p[j][3*i],p[j][3*i+1],p[j][3*i+2]);
	}
	fprintf(fp,"\n\nshowpage\n%%%%EOF\n");
	fclose(fp);
	return 0;
}
//-----------------------------------------------------------------------------
/// Write the frame in file using PNG format
void mgl_write_png(HMGL gr, const char *fname,const char *descr)
{	gr->WritePNG(fname,descr);	}
/// Write the frame in file using PNG format
void mgl_write_png_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{
	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	_GR_->WritePNG(s,f);	delete []s;		delete []f;
}
/// Write the frame in file using PNG format
void mgl_write_png_solid(HMGL gr, const char *fname,const char *descr)
{	gr->WritePNG(fname,descr,false);	}
/// Write the frame in file using PNG format
void mgl_write_png_solid_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{
	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	_GR_->WritePNG(s,f,false);	delete []s;		delete []f;
}
//-----------------------------------------------------------------------------
/// Write the frame in file using JPEG format
void mgl_write_jpg(HMGL gr, const char *fname,const char *descr)
{	gr->WriteJPEG(fname,descr);	}
#include <string.h>
/// Write the frame in file using JPEG format
void mgl_write_jpg_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{
	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	_GR_->WriteJPEG(s,f);	delete []s;		delete []f;
}
//-----------------------------------------------------------------------------
/// Write the frame in file using TIFF format
void mgl_write_bmp(HMGL gr, const char *fname,const char *descr)
{	gr->WriteBMP(fname,descr);	}
#include <string.h>
/// Write the frame in file using TIFF format
void mgl_write_bmp_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{
	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	_GR_->WriteBMP(s,f);	delete []s;		delete []f;
}
//-----------------------------------------------------------------------------
/// Write the frame in file using IDTF format
void mgl_write_idtf(HMGL gr, const char *fname,const char *descr)
{	gr->WriteIDTF(fname,descr);	}
#include <string.h>
/// Write the frame in file using IDTF format
void mgl_write_idtf_(uintptr_t *gr, const char *fname,const char *descr,int l,int n)
{
	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	char *f=new char[n+1];	memcpy(f,descr,n);	f[n]=0;
	_GR_->WriteIDTF(s,f);	delete []s;		delete []f;
}
//-----------------------------------------------------------------------------
