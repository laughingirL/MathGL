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
//-----------------------------------------------------------------------------
int mgl_hex(char ch)
{
	int res=-1;
	if(ch>='0' && ch<='9')	res = ch-'0';
	if(ch>='a' && ch<='f')	res = ch-'a'+10;
	return res;
}
//-----------------------------------------------------------------------------
/// Parse CGI string \a buf and return usual string as \a val for given \a name.
/// The size of \a val should be the same as size of \a buf.
void mgl_get_value(const char *buf, const char *name, char *val)
{
	const char *pos = strstr(buf,name);
	memset(val,0,strlen(buf)+1);
	if(pos && (pos==buf || pos[-1]=='&'))
	{
		register size_t i,j,l=strlen(pos);
		for(i=j=0;i<l;i++,j++)
		{
			register char ch=pos[i];
			if(ch=='&')	break;
			else if(ch=='+')	val[j]=' ';
			else if(ch=='%' && i+2<l)
			{	val[j] = 16*mgl_hex(pos[i+1]) + mgl_hex(pos[i+2]);	i+=2;	}
			else		val[j]=ch;
		}
	}
}
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	mglGraph gr;
	mglParse p(true);

	mgl_ask_func = 0;
	// read script
	setlocale(LC_CTYPE, "");

	char *str, *buf;
	const char *method = getenv("REQUEST_METHOD");
	if(strcmp(method,"GET"))
	{
		long len=atol(getenv("CONTENT_LENGTH"));
		buf = new char[len+1];
		fread(buf,len,1,stdin);
		buf[len]=0;
	}
	else		buf = getenv("QUERY_STRING");
	str = new char[strlen(buf)+1];
	mgl_get_value(buf,"mgl",str);

	p.Execute(&gr,str);

	printf("Content-Type: multipart/alternative; boundary=\"myboundary\"\n\n");
	printf("--myboundary\nContent-Type: image/png\n\n");
	gr.WritePNG("-");
	printf("--myboundary\nContent-Type: image/svg\n\n");
	gr.WriteSVG("-");
	return 0;
}
//-----------------------------------------------------------------------------
