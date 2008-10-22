/* mgl_io.h is part of Math Graphic Library
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

#ifndef _MGL_IO_H_
#define _MGL_IO_H_
//-----------------------------------------------------------------------------
#include<vector>
//-----------------------------------------------------------------------------
//typedef int ptrFunc (const char *, int,int,unsigned char**);
typedef int (*mgl_save) (const char *fname, int w, int h, unsigned char **);
typedef int (*mgl_load) (const char *fname, void (*func)(void*,int,int,const unsigned char **), void *par);
//-----------------------------------------------------------------------------
//using namespace std;
//-----------------------------------------------------------------------------
struct mglModule{
	const char*	Name;
	const char*	Desc;
	int		countExt;
	const char**	Ext;
	mgl_load	ptrLoad;
	mgl_save	ptrSave;
	void*		ltHandle;
};
//-----------------------------------------------------------------------------
class mglIO
{
    private:
	std::vector<mglModule>	mymod;

	int pRegister(const char *nDesc, int cExt, const char **type,
		mgl_load nptrLoad, mgl_save nptrSave, void* nltHandle=NULL,
		const char *nModuleName="");
	int punRegister(int offset);

    public:
	mglIO(void);
	~mglIO();
	int Register(const char *nDesc, int cExt, const char **type,
		mgl_load nptrLoad, mgl_save nptrSave, const char *nModuleName="");
#ifdef WITH_LTDL
	int Register(const char *nDesc, int cExt, const char **type,
		const char *nModuleName, const char *Path="");
#endif
	int unRegister(const char *nModuleName);
	int unRegister(mgl_load* nptrLoad, ptrFunc* nptrSave);
	int getNumber(void) const {return mymod.size();};
	int Save(const char *fname, const char *type, int w, int h, unsigned char **p) const;
	int Load(const char *fname, const char *type) const;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
