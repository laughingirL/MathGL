/***************************************************************************
 * fit.cpp is part of Math Graphic Library
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
#include "mgl/text.h"
//-----------------------------------------------------------------------------
extern char mglFitRes[1024];	///< Last fitted formula
/*void mgl_puts_fit(HMGL gr, float x, float y, float z, const char *pre, const char *font, float size)
{
	long n = strlen(mglFitRes)+(pre?strlen(pre):0)+1;
	char *buf = new char[n];
	if(pre)	sprintf(buf,"%s%s",pre,mglFitRes);
	else	strcpy(buf,mglFitRes);
	mgl_puts_ext(gr,x,y,z,buf,font,size,0);
	delete []buf;
}*/
//-----------------------------------------------------------------------------
