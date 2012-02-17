/***************************************************************************
 * mglview.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include "mgl/window.h"
#include "mgl/parser.h"
//-----------------------------------------------------------------------------
std::wstring str;
mglParse p(true);
void mgl_error_print(const char *Message, void *par);
void mgl_ask_fltk(const wchar_t *quest, wchar_t *res);
void mgl_ask_qt(const wchar_t *quest, wchar_t *res);
//-----------------------------------------------------------------------------
int show(mglGraph *gr)
{
	p.Execute(gr,str.c_str(),mgl_error_print);
	return 0;
}
//-----------------------------------------------------------------------------
int main(int narg, char **arg)
{
	long i,j=-1;
	for(i=1;i<narg;i++)	// add arguments for the script
	{
		if(arg[i][0]=='-' && arg[i][1]>='0' && arg[i][1]<='9')
			p.AddParam(arg[i][1]-'0',arg[i]+2);
		if(arg[i][0]=='-' && arg[i][1]=='L')
			setlocale(LC_CTYPE, arg[i]+2);
		if(arg[i][0]=='-' && (arg[i][1]=='h' || (arg[i][1]=='-' && arg[i][2]=='h')))
		{
			printf("mglview show MGL script and MGLD files in a window.\n");
			printf("Current version is 2.%g\n",MGL_VER2);
			printf("Usage:\tmgl2png scriptfile [outputfile parameter(s)]\n");
			printf("\tParameters have format \"-Nval\".\n");
			printf("\tHere N=0,1...9 is parameter ID and val is its value.\n");
			printf("\tOption -Lval set locale to val.\n");
		}
		if(arg[i][0]!='-' && j<0)	j=i;
	}
	mgl_ask_func = mgl_ask_gets;
#if defined(HAVE_FLTK)
	int kind=0;		mgl_ask_func = mgl_ask_fltk;
#elif defined(HAVE_QT)
	int kind=1;		mgl_ask_func = mgl_ask_qt;
#endif
	bool mgld=(j>0 && arg[j][strlen(arg[j])]=='d');
	if(!mgld)
	{
		FILE *fp = j>0?fopen(arg[j],"r"):stdin;
		while(!feof(fp))	str.push_back(fgetwc(fp));
		if(j>0)	fclose(fp);
	}
#if defined(HAVE_FLTK) || defined(HAVE_QT)
	mglWindow gr(kind, mgld?NULL:show ,j>0?arg[j]:"mglview");
	if(mgld)
	{	gr.ImportMGLD(arg[j]);	gr.Update();	}
	gr.Run();
#endif
	return 0;
}
//-----------------------------------------------------------------------------
