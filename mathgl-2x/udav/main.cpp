/***************************************************************************
 *   Copyright (C) 2008 by Alexey Balakin                                  *
 *   mathgl.abalakin@gmail.com                                             *
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
#include <QApplication>
#include <QTranslator>
#include <QSettings>
#include <QTextCodec>
#include <QTextCodec>
#include "udav_wnd.h"
#include "hint_dlg.h"
#include <mgl/parser.h>
#include <mgl/eval.h>
//-----------------------------------------------------------------------------
#ifndef UDAV_DIR
#ifdef WIN32
#define UDAV_DIR ""
#else
#define UDAV_DIR "/usr/local/share/udav/"
#endif
#endif
//-----------------------------------------------------------------------------
extern mglParser parser;
extern QString pathFont;
int mgl_cmd_cmp(const void *a, const void *b);
HintDialog *hintDialog=NULL;
QString pathHelp=MGL_DOC_DIR;
extern bool showHint;
//-----------------------------------------------------------------------------
void udavLoadDefCommands();
//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	QApplication a(argc, argv);
	QTranslator translator;
//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

	QString lang="";
	QSettings settings("udav","UDAV");
	settings.setPath(QSettings::IniFormat, QSettings::UserScope, "UDAV");
	settings.beginGroup("/UDAV");
	pathHelp = settings.value("/helpPath", MGL_DOC_DIR).toString();
	pathFont = settings.value("/userFont", "").toString();
	lang = settings.value("/udavLang", "").toString();
	settings.endGroup();
	if(pathHelp.isEmpty())	pathHelp=MGL_DOC_DIR;

	if(!lang.isEmpty())
	{
		if(!translator.load("udav_"+lang, UDAV_DIR))
			translator.load("udav_"+lang, pathHelp);
		a.installTranslator(&translator);
	}

	udavLoadDefCommands();
	hintDialog = new HintDialog();
	MainWindow *mw = new MainWindow();
	if(argc>1)
	{
		QTextCodec *codec = QTextCodec::codecForLocale();
		mw->load(codec->toUnicode(argv[1]), true);
	}
	mw->show();//	mw->edit->setCursorPosition(0);
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	if(showHint)	hintDialog->exec();
	return a.exec();
}
//-----------------------------------------------------------------------------
//
//		mgl addon
//
//-----------------------------------------------------------------------------
/*mglCommand udav_base_cmd[] = {
	{L"fplot",L"Plot curve by formula",L"fplot 'func' ['stl'='' num=100]", mgls_fplot, mglc_fplot},
	{L"fsurf",L"Plot surface by formula",L"fsurf 'func' ['stl'='' numx=100 numy=100]", mgls_fsurf, mglc_fsurf},
	{L"fgets",L"Print string from file",L"fgets x y {z} 'fname' [pos=0 'stl'='' size=-1.4]", mgls_fgets, mglc_fgets},
{L"",0,0,0,0}};*/
//-----------------------------------------------------------------------------
void udavAddCommands(const mglCommand *cmd)
{
	int i, mp, mc;
	// determine the number of symbols
	for(i=0;parser.Cmd[i].name[0];i++){};	mp = i;
	for(i=0;cmd[i].name[0];i++){};			mc = i;
	mglCommand *buf = new mglCommand[mp+mc+1];
	memcpy(buf, parser.Cmd, mp*sizeof(mglCommand));
	memcpy(buf+mp, cmd, (mc+1)*sizeof(mglCommand));
	qsort(buf, mp+mc, sizeof(mglCommand), mgl_cmd_cmp);
	if(parser.Cmd!=mgls_base_cmd)	delete []parser.Cmd;
	parser.Cmd = buf;
}
//-----------------------------------------------------------------------------
void udavLoadDefCommands()
{
//	udavAddCommands(udav_base_cmd);
}
//-----------------------------------------------------------------------------
#ifdef WIN32
void udavLoadCommands(const char *, const char *)	{}
#else
void udavLoadCommands(const char *, const char *)	{}
/*#include <ltdl.h>
typedef const mglCommadns *(*udav_cmd) ();
void udavLoadCommands(const char *lib, const char *func)
{
	void *jmodule=0;
	udav_cmd ff;
	if(lt_dlinit())	return;
	errors=lt_dlsetsearchpath(MOD_LIB_DIR);
	jmodule=lt_dlopenext(lib);
	if(jmodule)
	{
		ff = (udav_cmd) lt_dlsym((lt_dlhandle) jmodule, func);
		udavAddCommands(ff());
		lt_dlclose((lt_dlhandle) jmodule);
	}
	lt_dlexit();
}*/
#endif
//-----------------------------------------------------------------------------
