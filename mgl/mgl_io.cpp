/* mgl_io.cpp is part of Math Graphic Library
 * Copyright (C) 2008 Dmitry Kulagin <dik@kulagin.nnov.ru>
 * and Alexey Balakin <mathgl.abalakin@gmail.com>
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

#include<string.h>
#include<stdio.h>
#include<vector>
#include<mgl/mgl_io.h>

#ifdef WITH_LTDL
#include<ltdl.h>
#endif

// Maximum number or registered handlers
#define MAXCOUNT		255

// Return Codes
#define	IO_OK				0
#define IO_TOO_MANY_HANDLERS		1
#define IO_SAVE_LOAD_PTRS_DIFFER	2
#define IO_COUNTER_ERROR		3
#define IO_CANT_SET_SEARCH_PATH		4
#define IO_CANT_FIND_MODULE		5
#define IO_LOAD_HANDLER_NOTFOUND	6
#define IO_SAVE_HANDLER_NOTFOUND	7
#define IO_UNLOAD_MODULE		8
#define IO_UNKNOWN_HANDLERS		9
#define IO_SAVE_ERROR			10
#define IO_LOAD_ERROR			10

int mgl_bmp_save(const char *fname, int w, int h, unsigned char **p);
int mgl_png_save(const char *fname, int w, int h, unsigned char **p);
int mgl_pnga_save(const char *fname, int w, int h, unsigned char **p);
int mgl_png_load(const char *fname, mgl_save func);
//----------------------------------------------------------------------------
int mglIO::pRegister(const char *nDesc, int cExt, const char **type, mgl_load nptrLoad, mgl_save nptrSave, void* nltHandle, const char *nModuleName)
{
	mglModule temp;
	temp.Desc=nDesc;
	temp.countExt=cExt;
	temp.Ext=type;
	temp.ptrLoad=nptrLoad;
	temp.ptrSave=nptrSave;
	temp.ltHandle=nltHandle;
	temp.Name=nModuleName;
	mymod.push_back(temp);
	return IO_OK;
}
//----------------------------------------------------------------------------
int mglIO::Register(const char *nDesc, const int cExt, const char **type,
	mgl_load nptrLoad, mgl_save nptrSave, const char *nModuleName)
{
	return pRegister(nDesc,cExt,type,nptrLoad,nptrSave,NULL,nModuleName);
}
//----------------------------------------------------------------------------
#ifdef WITH_LTDL
int mglIO::Register(const char *nDesc, int cExt, const char **type,
	const char *nModuleName, const char *Path)
{
	lt_dlhandle module=NULL;
	mgl_load nptrLoad;
	mgl_save nptrSave;
	lt_dlinit();
	if(strlen(Path)!=0){
		if(lt_dladdsearchdir(Path)!=0){
//			printf("path %s\n",lt_dlgetsearchpath());
			return IO_CANT_SET_SEARCH_PATH;
		}
	}
	module=lt_dlopenext(nModuleName);
	if(module==NULL)
		return IO_CANT_FIND_MODULE;
	nptrSave=(mgl_save)lt_dlsym(module, "save");
	if(nptrSave==NULL){
//		printf("save not found\n");
		lt_dlclose(module);
		return IO_SAVE_HANDLER_NOTFOUND;
	}
	nptrLoad=(mgl_load)lt_dlsym(module, "load");
	if(nptrLoad==NULL){
		lt_dlclose(module);
		return IO_LOAD_HANDLER_NOTFOUND;
	}
	return pRegister(nDesc,cExt,type,nptrLoad,nptrSave,(void*)module,nModuleName);
}
#endif
//----------------------------------------------------------------------------
int mglIO::punRegister(int offset)
{
	if(mymod.size()==0)
	    return IO_COUNTER_ERROR;
	if(mymod[offset].ltHandle!=NULL){
#ifdef WITH_LTDL
		if(!lt_dlclose((lt_dlhandle)(mymod[offset].ltHandle)))
		{
#endif
			return IO_UNLOAD_MODULE;
#ifdef WITH_LTDL
		}
		lt_dlexit();}
#endif
	delete[] mymod[offset].Ext;
	mymod.erase(mymod.begin()+offset);
	return IO_OK;
}
//----------------------------------------------------------------------------
int mglIO::unRegister(mgl_load nptrLoad, mgl_save nptrSave)
{
	int posLoad=-1;
	int posSave=-1;
	for(unsigned int i=0; i<mymod.size(); i++)
		if(mymod[i].ptrLoad==nptrLoad)
			posLoad=i;
	for(unsigned int i=0; i<mymod.size(); i++)
		if(mymod[i].ptrSave==nptrSave)
			posSave=i;
	if(posSave!=posLoad)
		return IO_SAVE_LOAD_PTRS_DIFFER;
	if(posLoad==-1)
		return IO_UNKNOWN_HANDLERS;
	return punRegister(posLoad);
}
//----------------------------------------------------------------------------
int mglIO::unRegister(const char *nModuleName)
{
	int offset=-1;
	for(unsigned int i=0;i<mymod.size(); i++)
		if(!strcmp(mymod[i].Name,nModuleName))
			offset=i;
	if(offset==-1)
		return IO_UNKNOWN_HANDLERS;
	return punRegister(offset);
}
//----------------------------------------------------------------------------
int sample_save(const char *fname, int w,int h, unsigned char **p)
{
	printf("Sample save called\n");
	return 0;
}
//----------------------------------------------------------------------------
int sample_load(const char *fname, void (*func)(void*,int,int,const unsigned char **), void *par)
{
	printf("Sample load called\n");
	return 0;
}
//----------------------------------------------------------------------------
mglIO::mglIO(void)
{
	const char *Ext1="sml";
	const char *Ext2="sample";
	const char **type=new const char*[2];
	type[0]=Ext1; type[1]=Ext2;
	Register("Sample data type", 2, type, &sample_save, &sample_load,"sample");
}
//----------------------------------------------------------------------------
mglIO::~mglIO()
{
	unRegister("sample");
	for(unsigned int i=0;i<mymod.size();i++)
		punRegister(i);
}
//----------------------------------------------------------------------------
int mglIO::Save(const char *type, const char *fname, int w, int h, unsigned char **p) const
{
	ptrFunc* func;
	for(unsigned int i=0;i<mymod.size();i++)
		for(int j=0;j<mymod[i].countExt;j++)
			if(!strcmp(mymod[i].Ext[j],type)){
				func=mymod[i].ptrSave;
				return (*func)(fname,w,h,p);
			}
	return IO_SAVE_ERROR;
}
//----------------------------------------------------------------------------
int mglIO::Load(const char *type, const char *fname, int w, int h, unsigned char **p) const
{
	ptrFunc* func;
	for(unsigned int i=0;i<mymod.size();i++)
		for(int j=0;j<mymod[i].countExt;j++)
			if(!strcmp(mymod[i].Ext[j],type))
			{
				func=mymod[i].ptrLoad;
				return (*func)(fname,w,h,p);
			}
	return IO_LOAD_ERROR;
}
//----------------------------------------------------------------------------
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
void mgl_png_load(const char *fname, void (*func)(void*,int,int,const unsigned char **), void *par)
{
	FILE *fp = fopen(fname, "rb");
	if (!fp)	return;
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if (!png_ptr)	return;
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{	png_destroy_read_struct(&png_ptr,0, 0);	return;	}
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{	png_destroy_read_struct(&png_ptr, &info_ptr, 0);	return;	}

	png_init_io(png_ptr, fp);
	png_read_png(png_ptr, info_ptr,
		PNG_TRANSFORM_STRIP_ALPHA|PNG_TRANSFORM_PACKING|
		PNG_TRANSFORM_STRIP_16|PNG_TRANSFORM_EXPAND,0);
	unsigned char **rows = png_get_rows(png_ptr, info_ptr);

	long w=png_get_image_width(png_ptr, info_ptr);
	long h=png_get_image_height(png_ptr, info_ptr);
	func(par, w, h, rows);

	png_destroy_read_struct(&png_ptr, &info_ptr,&end_info);
	fclose(fp);
}
//-----------------------------------------------------------------------------
