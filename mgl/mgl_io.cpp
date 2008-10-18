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

//----------------------------------------------------------------------------
int mglIO::pRegister(const char *nDesc, int cExt, const char **nExt, void* nptrLoad, \
	void* nptrSave, lt_dlhandle nltHandle, const char *nModuleName)
{
	mglModule temp;
	temp.Desc=nDesc;
	temp.countExt=cExt;
	temp.Ext=nExt;
	temp.ptrLoad=nptrLoad;
	temp.ptrSave=nptrSave;
	temp.ltHandle=nltHandle;
	temp.Name=nModuleName;
	mymod.push_back(temp);
	return IO_OK;
}
//----------------------------------------------------------------------------
int mglIO::Register(const char *nDesc, const int cExt, const char **nExt, \
	void* nptrLoad, void* nptrSave, const char *nModuleName)
{
	return pRegister(nDesc,cExt,nExt,nptrLoad,nptrSave,NULL,nModuleName);
}
//----------------------------------------------------------------------------
int mglIO::Register(const char *nDesc, int cExt, const char **nExt, \
	const char *nModuleName, const char *Path)
{
	lt_dlhandle module=NULL;
	void* nptrLoad;
	void* nptrSave;
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
	nptrSave=lt_dlsym(module, "save");
	if(nptrSave==NULL){
//		printf("save not found\n");
		lt_dlclose(module);
		return IO_SAVE_HANDLER_NOTFOUND;
	}
	nptrLoad=lt_dlsym(module, "load");
	if(nptrLoad==NULL){
		lt_dlclose(module);
		return IO_LOAD_HANDLER_NOTFOUND;
	}
	return pRegister(nDesc,cExt,nExt,nptrLoad,nptrSave,module,nModuleName);
}
//----------------------------------------------------------------------------
int mglIO::punRegister(int offset)
{
	if(mymod.size()==0)
	    return IO_COUNTER_ERROR;
	if(mymod[offset].ltHandle!=NULL){
		if(!lt_dlclose(mymod[offset].ltHandle))
			return IO_UNLOAD_MODULE;
		lt_dlexit();}
	mymod.erase(mymod.begin()+offset);
	return IO_OK;
}
//----------------------------------------------------------------------------
int mglIO::unRegister(void* nptrLoad, void* nptrSave)
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
void sample_save(const char *fname, int w,int h, unsigned char **p)
{
	printf("Sample save called\n");
}
//----------------------------------------------------------------------------
void sample_load(const char *fname, int w,int h, unsigned char **p)
{
	printf("Sample load called\n");
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
mglIO::mglIO(void)
{
	const char *Ext1="sml";
	const char *Ext2="sample";
	const char **nExt=new const char*[2];
	nExt[0]=Ext1; nExt[1]=Ext2;
	Register("Sample data type", 2, nExt, (void*)(&sample_save), (void*)(&sample_load),"sample");
}
//----------------------------------------------------------------------------
mglIO::~mglIO()
{
	for(unsigned int i=0;i<mymod.size();i++){
		punRegister(i);
	}
}
//----------------------------------------------------------------------------
int mglIO::Save(const char *nExt, const char *fname, int w, int h, unsigned char **p) const
{
	void (*pt2Func)(const char *, int,int,unsigned char**);
	for(unsigned int i=0;i<mymod.size();i++){
		for(int j=0;j<mymod[i].countExt;j++){
			if(!strcmp(mymod[i].Ext[j],nExt)){
				pt2Func=(void (*)(const char *, int,int,unsigned char**))mymod[i].ptrSave;
				(*pt2Func)(fname,w,h,p);
				return IO_OK;
			}
				
		}
	}
	return IO_SAVE_ERROR;
}
//----------------------------------------------------------------------------
int mglIO::Load(const char *nExt, const char *fname, int w, int h, unsigned char **p) const
{
	void (*pt2Func)(const char *, int,int,unsigned char**);
	for(unsigned int i=0;i<mymod.size();i++){
		for(int j=0;j<mymod[i].countExt;j++){
			if(!strcmp(mymod[i].Ext[j],nExt)){
				pt2Func=(void (*)(const char *, int,int,unsigned char**))mymod[i].ptrLoad;
				(*pt2Func)(fname,w,h,p);
				return IO_OK;
			}
				
		}
	}
	return IO_LOAD_ERROR;
}
