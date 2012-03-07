/***************************************************************************
 * mgl2png.cpp is part of Math Graphic Library
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
#include "mgl/mgl.h"
void mgl_error_print(const char *Message, void *par);
void mgl_ask_gets(const wchar_t *quest, wchar_t *res);
//-----------------------------------------------------------------------------
int main(int narg, char **arg)
{
	mglGraph gr;
	mglParse p(true);

	long i,j=-1,k=-1;
	unsigned long n;
	for(i=1;i<narg;i++)	// add arguments for the script
	{
		if(arg[i][0]=='-' && arg[i][1]>='0' && arg[i][1]<='9')
			p.AddParam(arg[i][1]-'0',arg[i]+2);
		if(arg[i][0]=='-' && arg[i][1]=='L')
			setlocale(LC_CTYPE, arg[i]+2);
		if(arg[i][0]=='-' && (arg[i][1]=='h' || (arg[i][1]=='-' && arg[i][2]=='h')))
		{
			printf("mglconv convert mgl script to bitmap png file.\n");
			printf("Current version is 2.%g\n",MGL_VER2);
			printf("Usage:\tmglconv scriptfile [outputfile parameter(s)]\n");
			printf("\tParameters have format \"-Nval\".\n");
			printf("\tHere N=0,1...9 is parameter ID and val is its value.\n");
			printf("\tOption -Lval set locale to val.\n");
		}
		if(arg[i][0]!='-' && j<0)	j=i;
		if(arg[i][0]!='-' && j>0)	k=i;
	}
	mgl_ask_func = mgl_ask_gets;
	// prepare for animation
	std::wstring str;
	FILE *fp = j>0?fopen(arg[j],"r"):stdin;
	while(!feof(fp))	str.push_back(fgetwc(fp));
	if(j>0)	fclose(fp);
	std::vector<std::wstring> var;
	for(i=0;;)	// collect exact values
	{
		n = str.find(L"##a ",i);
		if(n==std::string::npos)	break;
		i = n+4;	var.push_back(str.substr(i,str.find('\n',i)));
	}
	n = str.find(L"##c ");
	if(n!=std::string::npos)
	{
		float v1,v2,dv,v;
		wscanf(str.c_str()+n+4,L"%g%g%g",&v1,&v2,&dv);
		wchar_t ss[64];
		for(v=v1;v<=v2;v+=dv)
		{	mglprintf(ss,64,L"%g",v);	var.push_back(ss);	}
	}
	char buf[2048],fname[256];
	if(k>0)	strcpy(fname,arg[k]);
	else
	{	strcpy(fname,j>0?arg[j]:"out");	strcat(fname,".png");	}
	bool gif= !strcmp(fname+strlen(fname)-4,".gif");
	if(var.size()>1)	// there is animation
	{
		if(gif)	gr.StartGIF(fname);
		for(i=0;i!=var.size()-1;i++)
		{
			gr.NewFrame();
			p.AddParam(0,var[i].c_str());
			p.Execute(&gr,str.c_str());
			printf("%s\n",gr.Message());
			gr.EndFrame();
			sprintf(buf,"%s-%ld",fname,i);
			if(!gif)	gr.WriteFrame(buf);
		}
		if(gif)	gr.CloseGIF();
	}
	else
	{
		p.Execute(&gr,str.c_str());
		printf("%s\n",gr.Message());
		gr.WriteFrame(fname);
	}
	printf("Write output to %s\n",fname);
	return 0;
}
//-----------------------------------------------------------------------------
