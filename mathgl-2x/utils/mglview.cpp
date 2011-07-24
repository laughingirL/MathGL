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
char fname[256];
mglParse p(true);
char str[8192];
char buf[2048];
//-----------------------------------------------------------------------------
int show(mglGraph *gr)
{
	FILE *fp = fopen(fname,"rb");
	p.Execute(gr, fp, true);
	fclose(fp);
	return 0;
}
//-----------------------------------------------------------------------------
int main(int narg, char **arg)
{
	if(narg==1)
	{
		printf("mglview show MGL script in FLTK window.\n");
		printf("Current version is 2.%g\n",MGL_VER2);
		printf("Usage:\tmglview scriptfile [parameter(s)]\n");
		printf("\tParameters have format \"-Nval\".\n");
		printf("\tHere N=0,1...9 is parameter ID and val is its value.\n");
		printf("\tOption -Lval set locale to val.\n");
	}
	else
	{
		FILE *fp = fopen(arg[1],"rb");
		if(fp==0)	printf("Couldn't open file %s\n",arg[1]);
		else
		{
			register long i,j;
			for(i=2;i<narg;i++)	// add arguments for the script
			{
				if(arg[i][0]=='-' && arg[i][1]>='0' && arg[i][1]<='9')
				{	p.AddParam(arg[i][1]-'0',arg[i]+2);	arg[i][0]=0;	}
				if(arg[i][0]=='-' && arg[i][1]=='L')
				{	setlocale(LC_CTYPE, arg[i]+2);		arg[i][0]=0;	}
			}
			strncpy(fname,arg[1],256);
			fclose(fp);
			arg[1][0] = 0;
			for(i=narg-1;i>=0;i--)
			{
				if(arg[i][0]==0)
				{
					for(j=i;j<narg-1;j++)	arg[j] = arg[j+1];
					narg--;
				}
			}
			mglWindow gr(0,show,fname);
//			gr.Window(narg,arg,show,fname);
			gr.Run();	return 0;
		}
	}
	return 0;
}
//-----------------------------------------------------------------------------
