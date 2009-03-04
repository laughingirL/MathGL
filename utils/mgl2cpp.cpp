/***************************************************************************
 * mgl2cpp.cpp is part of Math Graphic Library
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
#include <wchar.h>
#include <string.h>
#include <locale.h>
#include "mgl/mgl_eps.h"
#include "mgl/mgl_parse.h"
void wcstrim_mgl(wchar_t *str);
//-----------------------------------------------------------------------------
int main(int narg, char **arg)
{
	mglGraphPS gr;
	mglParse p(true);
//	setlocale(LC_CTYPE,"C");

	if(narg==1)
	{
		printf("mgl2cpp convert mgl script to C++ file.\n");
		printf("Current version is 1.%g\n",MGL_VERSION);
		printf("Usage:\tmgl2cpp scriptfile [outputfile parameter(s)]\n");
		printf("\tParameters have format \"-Nval\".\n");
		printf("\tHere N=0,1...9 is parameter ID and val is its value.\n");
		printf("\tOption -Lval set locale to val.\n");
	}
	else
	{
		FILE *fp = fopen(arg[1],"rb"), *fo;
		if(fp==0)	{	printf("Couldn't open file %s\n",arg[1]);	return 1;	}
		wchar_t str[8192], out[1024];
		char fname[256];
		for(long i=2;i<narg;i++)	// add arguments for the script
		{
			if(arg[i][0]=='-' && arg[i][1]>='0' && arg[i][1]<='9')
				p.AddParam(arg[i][1]-'0',arg[i]+2);
			if(arg[i][0]=='-' && arg[i][1]=='L')
				setlocale(LC_CTYPE, arg[i]+2);
		}
		if(narg>2 && arg[2][0]!='-')	strcpy(fname,arg[2]);
		else
		{
			strcpy(fname,arg[1]);	strcat(fname,".cpp");
			wprintf(L"Write output to %s\n",fname);
		}
		fo = fopen(fname,"wt");
		fwprintf(fo,L"int draw_func(mglGraph *gr, void *)\n{\n");
		fwprintf(fo,L"\tstatic bool once = false;\n");
		while(!feof(fp))
		{
			if(!fgetws(str,8192,fp))	break;
			wcstrim_mgl(str);
			out[0] = 0;
			int r = p.Export(out,&gr,str);
			fwprintf(fo,L"\t//%ls\n",str);
			if(*out)
			{
				if(*(p.op1))	fwprintf(fo,L"%ls\n",p.op1);
				fwprintf(fo,L"\t%ls\n",out);
				if(*(p.op2))	fwprintf(fo,L"%ls\n",p.op2);
				fflush(fo);
			}
			if(r==1)	wprintf(L"Wrong argument(s) in %ls\n",str);
			if(r==2)	wprintf(L"Wrong command in %ls\n",str);
			if(r==3)	wprintf(L"String too long in %ls\n",str);
			if(r==4)	wprintf(L"Unbalanced ' in %ls\n", str);
		}
		fwprintf(fo,L"\treturn 0;\n}\n");
		fclose(fp);	fclose(fo);
	}
	return 0;
}
//-----------------------------------------------------------------------------
