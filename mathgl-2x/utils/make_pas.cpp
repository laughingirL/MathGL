#include <stdio.h>
#include <string.h>

const char *files[]={"../include/mgl2/base_cf.h","../include/mgl2/data_cf.h", "../include/mgl2/datac_cf.h", "../include/mgl2/cont.h", "../include/mgl2/fit.h", "../include/mgl2/plot.h", "../include/mgl2/surf.h", "../include/mgl2/volume.h", "../include/mgl2/vect.h", "../include/mgl2/prim.h", "../include/mgl2/other.h", "../include/mgl2/canvas_cf.h", "../include/mgl2/addon.h", ""};

const char *head="//**************************************************************************\n\
// mgl_pas.pas is part of Math Graphic Library                             *\n\
// Copyright (C) 2008-2013 Mikhail Barg, Alexey Balakin                    *\n\
//                                                                         *\n\
//   This program is free software; you can redistribute it and/or modify  *\n\
//   it under the terms of the GNU Library General Public License as       *\n\
//   published by the Free Software Foundation; either version 2 of the    *\n\
//   License, or (at your option) any later version.                       *\n\
//                                                                         *\n\
//   This program is distributed in the hope that it will be useful,       *\n\
//   but WITHOUT ANY WARRANTY; without even the implied warranty of        *\n\
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *\n\
//   GNU General Public License for more details.                          *\n\
//                                                                         *\n\
//   You should have received a copy of the GNU Library General Public     *\n\
//   License along with this program; if not, write to the                 *\n\
//   Free Software Foundation, Inc.,                                       *\n\
//   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *\n\
//**************************************************************************\n\n\
unit mgl_pas;\n\n\
{$IFDEF FPC}\n\
{$MODE DELPHI }\n\
{$PACKENUM 4}    (* use 4-byte enums *)\n\
{$PACKRECORDS C} (* C/C++-compatible record packing *)\n\
{$ELSE}\n\
{$MINENUMSIZE 4} (* use 4-byte enums *)\n\
{$ENDIF}\n\n\
{$IFDEF DARWIN}\n\
{$linklib libmgl}\n\
{$ENDIF}\n\n\
interface\n\n\
uses\n\
{$IFDEF MSWINDOWS}\n\
Windows, Graphics,\n\
{$ENDIF}\n\
Math;\n\n\
const MGL_VER2 = 2.2;\n\
//* This define enables double precision in MathGL */\n\
MGL_USE_DOUBLE = 1;\n\n\
const\n\
{$IFDEF MSWINDOWS}\n\
//win - .dll\n\
libmgl = 'libmgl.dll';\n\
libmglglut = 'libmgl-glut.dll';\n\
libmglfltk = 'libmgl-fltk.dll';\n\
libmglqt   = 'libmgl-qt.dll';\n\
{$ELSE}\n\
{$IFDEF LINUX}\n\
//linux - .so\n\
libmgl = 'libmgl.so';\n\
libmglglut = 'libmgl-glut.so';\n\
libmglfltk = 'libmgl-fltk.so';\n\
libmglqt   = 'libmgl-qt.so';\n\
{$ELSE}\n\
{$IFDEF DARWIN}\n\
//darwin - .dylib\n\
libmgl = 'libmgl.dylib';\n\
libmglglut = 'libmgl-glut.dylib';\n\
libmglfltk = 'libmgl-fltk.dylib';\n\
libmglqt   = 'libmgl-qt.dylib';\n\
{$ELSE}\n\
// other platforms?\n\n\
{$ENDIF}\n\
{$ENDIF}\n\
{$ENDIF}\n\n\
{$IF (MGL_USE_DOUBLE = 0)}\n\
type mreal = double;\n\
{$ELSE}\n\
type mreal = real;\n\
{$IFEND}\n\
Pmreal = ^mreal;\n\n\
type TNGLDraw = record\n\
end;\n\
type TMGLGraph = record\n\
end;\n\
type TMGLData = record\n\
end;\n\
type TMGLParse = record\n\
end;\n\
type TMGLFormula = record\n\
end;\n\
type TMGLFormulaC = record\n\
end;\n\
type HMDR = ^TNGLDraw;\n\
type HMGL = ^TMGLGraph;\n\
type HMDT = ^TMGLData;\n\
type HMPR = ^TMGLParse;\n\
type PPChar = ^PChar;\n\
type HMEX = ^TMGLFormula;\n\
type HAEX = ^TMGLFormulaC;\n\n\
type Preal = ^single;\n\
type Pdouble = ^double;\n\
type Pint = ^integer;\n\
type TGSLVector = record\n\
end;\n\
type TGSLMatrix = record\n\
end;\n\
type PGSLVector = ^TGSLVector;\n\
type PGSLMatrix = ^TGSLMatrix;\n\n\
type TMglDrawFunction = function (gr: HMGL; p: pointer): integereger; cdecl;\n\
function mgl_create_graph_gl(): HMGL; cdecl; external libmgl;\n\
function mgl_create_graph_glut(draw: TMglDrawFunction; const title: PChar; par: pointer): HMGL; cdecl; external libmglglut;\n\
function mgl_create_graph_fltk(draw: TMglDrawFunction; const title: PChar; par: pointer): HMGL; cdecl; external libmglfltk;\n\
procedure mgl_fltk_run(); cdecl; external libmglfltk;\n\
function mgl_create_graph_qt(draw: TMglDrawFunction; const title: PChar; par: pointer): HMGL; cdecl; external libmglqt;\n\
procedure mgl_qt_run(); cdecl; external libmglqt;\n";

const char *parse_name(char *name)
{
	static char res[1024];
	char *ptr,*arg[20],nul=0;	// TODO check if 20 arguments is enough
	unsigned i,j;
	for(i=0;name[i]!='(';i++)	res[i]=name[i];
	res[i]='(';	res[i+1]=0;	i++;
	while(name[i]<=' ')	i++;
	for(j=0;j<20;j++)	arg[j]=&nul;
	for(j=0;j<20;j++)
	{
		arg[j]= name[i]<=' ' ? name+i+1 : name+i;
		ptr=strchr(name+i,',');
		if(!ptr)	break;
		*ptr=0;	i=ptr-name+1;
	}
	ptr=strchr(name+i,')');	if(ptr) *ptr=0;
	if(arg[0][0]==0)	strcat(res," ");
	for(j=0;j<20;j++)
	{
		if(arg[j][0]==0)	break;
		ptr = res+strlen(res);
		if(!strncmp(arg[j],"HMGL ",5))
			sprintf(ptr, "%s: HMGL;", arg[j]+5);
//		else if(!strncmp(arg[j],"HADT ",5))
//		{	strcat(res,arg[j]+5);	strcat(res,": HADT;");	}
		else if(!strncmp(arg[j],"HCDT ",5))
			sprintf(ptr, "const %s: HMDT;", arg[j]+5);
		else if(!strncmp(arg[j],"HMDT ",5))
			sprintf(ptr, "%s: HMDT;", arg[j]+5);
		else if(!strncmp(arg[j],"HMPR ",5))
			sprintf(ptr, "%s: HMPR;", arg[j]+5);
		else if(!strncmp(arg[j],"HMEX ",5))
			sprintf(ptr, "%s: HMEX;", arg[j]+5);
		else if(!strncmp(arg[j],"const float *",13))
			sprintf(ptr, "const %s: Preal;", arg[j]+13);
		else if(!strncmp(arg[j],"const double *",14))
			sprintf(ptr, "const %s: Pdouble;", arg[j]+14);
		else if(!strncmp(arg[j],"mreal *",7))
			sprintf(ptr, "%s: Pmreal;", arg[j]+7);
		else if(!strncmp(arg[j],"double *",8))
			sprintf(ptr, "%s: Pdouble;", arg[j]+8);
		else if(!strncmp(arg[j],"char *",6))
			sprintf(ptr, "%s: PChar;", arg[j]+6);
		else if(!strncmp(arg[j],"int *",5))
			sprintf(ptr, "%s: Pint;", arg[j]+5);
		else if(!strncmp(arg[j],"long *",6))	// TODO check
			sprintf(ptr, "%s: Pint;", arg[j]+6);
		else if(!strncmp(arg[j],"const char *",12))
			sprintf(ptr, "const %s: PChar;", arg[j]+12);
		else if(!strncmp(arg[j],"const wchar_t *",15))
			sprintf(ptr, "const %s: PWideChar;", arg[j]+15);
		else if(!strncmp(arg[j],"char ",5))
			sprintf(ptr, "%s: char;", arg[j]+5);
		else if(!strncmp(arg[j],"long ",5))		// TODO check
			sprintf(ptr, "%s: integer;", arg[j]+5);
		else if(!strncmp(arg[j],"uint32_t ",9))	// TODO check
			sprintf(ptr, "%s: integer;", arg[j]+9);
		else if(!strncmp(arg[j],"int ",4))
			sprintf(ptr, "%s: integer;", arg[j]+4);
		else if(!strncmp(arg[j],"mreal ",6))
			sprintf(ptr, "%s: mreal;", arg[j]+6);
		else if(!strncmp(arg[j],"double ",7))
			sprintf(ptr, "%s: double;", arg[j]+7);
		else if(!strncmp(arg[j],"gsl_vector *",12))
			sprintf(ptr, "%s: PGSLVector;", arg[j]+12);
		else if(!strncmp(arg[j],"gsl_matrix *",12))
			sprintf(ptr, "%s: PGSLMatrix;", arg[j]+12);
		else
			sprintf(ptr, " !!! %s;", arg[j]);
	}
	
//	if(strncmp(name+i,"HMGL ",5))
	i = strlen(res);	res[i-1]=')';
	return res;
}

bool parse_file(const char *fname, FILE *out)
{
	if(!fname || fname[0]==0)	return false;
	FILE *fp=fopen(fname,"rt");
	if(!fp)	return false;
	
	char buf[1024], *ptr;
	while(!feof(fp))
	{
		fgets(buf,1024,fp);
		// first filter unwanted strings
		if(buf[0]==0 || buf[0]=='\n' || buf[1]=='\n')	continue;
		if(buf[0]=='#' || buf[0]=='}')	continue;
		if(!strncmp(buf, "extern",6))	continue;
		if(!strncmp(buf, "class",5))	continue;
		if(!strncmp(buf, "struct",6))	continue;
		if(!strncmp(buf, "typedef",7))	continue;
		if(strstr(buf, "void *"))	continue;
		if(strstr(buf, "_("))	continue;
		if(strstr(buf, "FILE"))	continue;
		if(strstr(buf, "TODO"))	continue;
		if(strstr(buf, "...)"))	continue;
		
		// TODO following 5 lines enable later
		if(strstr(buf, "* const *"))	continue;
		if(strstr(buf, "uint64_t"))	continue;
		if(strstr(buf, "dual"))	continue;
		if(strstr(buf, "HADT"))	continue;
		if(strstr(buf, "HAEX"))	continue;

		// now filter comments
		if(buf[0]=='/' && buf[1]=='*')
		{
			do fgets(buf,1024,fp);	while(!strstr(buf,"*/"));
			continue;
		}
		ptr = strchr(buf,';');	if(ptr)	*ptr=' ';
		for(unsigned i=strlen(buf)-1;buf[i]<=' ';i--)	buf[i]=0;
		if(buf[0]=='/' && buf[1]=='/')
			fprintf(out,"%s\n",buf);
		else if(!strncmp(buf,"void MGL_EXPORT",15))
			fprintf(out,"procedure %s; cdecl; external libmgl;\n",parse_name(buf+16));
		else if(!strncmp(buf,"int MGL_EXPORT",14))
			fprintf(out,"function %s: integer; cdecl; external libmgl;\n",parse_name(buf+15));
		else if(!strncmp(buf,"double MGL_EXPORT",17))
			fprintf(out,"function %s: double; cdecl; external libmgl;\n",parse_name(buf+18));
		else if(!strncmp(buf,"mreal MGL_EXPORT",16))
			fprintf(out,"function %s: mreal; cdecl; external libmgl;\n",parse_name(buf+17));
		else if(!strncmp(buf,"long MGL_EXPORT",15))
			fprintf(out,"function %s: integer; cdecl; external libmgl;\n",parse_name(buf+16));
		else if(!strncmp(buf,"HMDT MGL_EXPORT",15))
			fprintf(out,"function %s: HMDT; cdecl; external libmgl;\n",parse_name(buf+16));
		else if(!strncmp(buf,"HMGL MGL_EXPORT",15))
			fprintf(out,"function %s: HMGL; cdecl; external libmgl;\n",parse_name(buf+16));
		else if(!strncmp(buf,"MGL_EXPORT const char *",23))
			fprintf(out,"function %s: PChar; cdecl; external libmgl;\n",parse_name(buf+24));
		else if(!strncmp(buf,"MGL_EXPORT mreal *",18))
			fprintf(out,"function %s: Pmreal; cdecl; external libmgl;\n",parse_name(buf+19));
		else if(!strncmp(buf,"MGL_EXPORT const unsigned char *",32))
			fprintf(out,"function %s: PByte; cdecl; external libmgl;\n",parse_name(buf+33));
		else if(!strncmp(buf,"HMPR MGL_EXPORT",15))
			fprintf(out,"function %s: HMPR; cdecl; external libmgl;\n",parse_name(buf+16));
		else if(!strncmp(buf,"HMEX MGL_EXPORT",15))
			fprintf(out,"function %s: HMEX; cdecl; external libmgl;\n",parse_name(buf+16));
		else if(!strncmp(buf,"HADT MGL_EXPORT",15))
			fprintf(out,"function %s: HADT; cdecl; external libmgl;\n",parse_name(buf+16));
		else
			fprintf(out,"!!!!\t%s\n",buf);	// NOTE should be never here!
	}
	return true;
}

int main()
{
	FILE *fout = fopen("../include/mgl2/mgl_pas.pas","wt");
	fprintf(fout,"%s\n",head);
	for(int i=0;parse_file(files[i],fout);i++);
	fclose(fout);
	return 0;
}
