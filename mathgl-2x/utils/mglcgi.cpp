/***************************************************************************
 * mglcgi.cpp is part of Math Graphic Library
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
#include <locale.h>
#include <unistd.h>
#include "mgl2/mgl.h"
void mgl_error_print(const char *Message, void *par);
void mgl_ask_gets(const wchar_t *quest, wchar_t *res);
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	mglGraph gr;
	mglParse p(true);

	mgl_ask_func = 0;
	// read script
	std::wstring str;
	setlocale(LC_CTYPE, "");
	FILE *fp = stdin;
	while(!feof(fp))	str.push_back(fgetwc(fp));

	// test output
	FILE *tmp = fopen("/home/balakin/cgi.txt","wt");
	fprintf(tmp,"argv = %d\n",argc);
	for(int i=0;i<argc;i++)	fprintf(tmp,"%d:%s\n",i,argv[i]);
	fprintf(tmp,"------\n%ls\n",str.c_str());
	fclose(tmp);
	
	p.Execute(&gr,str.c_str());

	printf("Content-Type: multipart/alternative; boundary=\"myboundary\"\n");
	printf("--myboundary\nContent-Type: image/png\n");
	gr.WritePNG("-");
	printf("--myboundary\nContent-Type: image/svg\n");
	gr.WriteSVG("-");
	return 0;
}
//-----------------------------------------------------------------------------
