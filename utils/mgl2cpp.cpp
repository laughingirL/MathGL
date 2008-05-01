/* mgl2eps.cpp is part of Math Graphic Library
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
#include <stdio.h>
#include <string.h>
#include "mgl/mgl_eps.h"
#include "mgl/mgl_parse.h"
//-----------------------------------------------------------------------------
int main(int narg, char **arg)
{
	mglGraphPS gr;
	mglParse p(true);

	if(narg==1)
	{
		printf("mgl2cpp convert mgl script to C++ file.\n");
		printf("Current version is 1.%g\n",MGL_VERSION);
		printf("Usage:\tmgl2cpp scriptfile [outputfile parameter(s)]\n");
		printf("\tParameters have format \"-Nval\".\n");
		printf("\tHere N=0,1...9 is parameter ID and val is its value\n");
	}
	else
	{
		FILE *fp = fopen(arg[1],"rt"), *fo;
		if(fp==0)	{	printf("Couldn't open file %s\n",arg[1]);	return 1;	}
		char str[8192], out[1024];
		for(long i=2;i<narg;i++)	// add arguments for the script
			if(arg[i][0]=='-' && arg[i][1]>='0' && arg[i][1]<='9')
				p.AddParam(arg[i][1]-'0',arg[i]+2);
		if(narg>2 && arg[2][0]!='-')	strcpy(str,arg[2]);
		else
		{
			strcpy(str,arg[1]);	strcat(str,".cpp");
			printf("Write output to %s\n",str);
		}
		fo = fopen(str,"wt");
		fprintf(fo,"int draw_func(mglGraph *gr, void *)\n{\n");
		fprintf(fo,"\tstatic bool once = false;\n");
		while(!feof(fp))
		{
			if(!fgets(str,8192,fp))	break;
			out[0] = 0;
			int r = p.Export(out,&gr,str);
			if(*out)	fprintf(fo,"\t%s\t//%s",out,str);
			if(r==1)	printf("Wrong argument(s) in %s\n",str);
			if(r==2)	printf("Wrong command in %s\n",str);
			if(r==3)	printf("String too long in %s\n",str);
		}
		fprintf(fo,"\treturn 0;\n}\n");
		fclose(fp);	fclose(fo);
	}
	return 0;
}
//-----------------------------------------------------------------------------
