/* mgl_font.h is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>
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
//-----------------------------------------------------------------------------
#ifndef _MGL_FONT_H_
#define _MGL_FONT_H_

#include <stdio.h>
#include <math.h>

//-----------------------------------------------------------------------------
#define MGL_FONT_BOLD		0x01000000
#define MGL_FONT_ITAL		0x02000000
#define MGL_FONT_BOLD_ITAL	0x03000000
#define MGL_FONT_ULINE		0x04000000
#define MGL_FONT_OLINE		0x08000000
#define MGL_FONT_WIRE		0x10000000
#define MGL_FONT_ZEROW		0x20000000		// internal codes
#define MGL_FONT_UPPER		0x40000000
#define MGL_FONT_LOWER		0x80000000
#define MGL_FONT_ROMAN		0xfc000000
#define MGL_FONT_MASK		0x00ffffff
#define MGL_FONT_STYLE		0x3f000000
//-----------------------------------------------------------------------------
#ifdef WIN32	// a man ask to use built-in font under Windows
#define MGL_DEF_FONT_NAME	0
#else
#define MGL_DEF_FONT_NAME	"STIX"
#endif
//-----------------------------------------------------------------------------
struct mglTeXsymb	{	unsigned kod;	const wchar_t *tex;	};
class mglGraph;
//-----------------------------------------------------------------------------
/// Class for incapsulating font plotting procedures
class mglFont
{
public:
	mglGraph *gr;	///< mglGraph class used for drawing characters
	mglFont(const char *name=MGL_DEF_FONT_NAME, const char *path=0);
	~mglFont();
	bool parse;		///< Parse LaTeX symbols

	/// Load font data to memory. Normally used by constructor.
	bool Load(const char *base, const char *path=0);
	/// Free memory
	void Clear();
	/// Get height of text
	float Height(int font);
	/// Get height of text
	float Height(const char *how);
	/// Print text string for font specified by string
	void Puts(const char *str,const char *how);
	/// Get width of text string for font specified by string
	float Width(const char *str,const char *how);
	/// Print text string for font specified by integer constant
	void Puts(const char *str,int font=0,int align=0);
	/// Get width of text string for font specified by integer constant
	float Width(const char *str,int font=0);
	
	/// Print text string for font specified by string
	void Puts(const wchar_t *str,const char *how);
	/// Get width of text string for font specified by string
	float Width(const wchar_t *str,const char *how);
	/// Print text string for font specified by integer constant
	void Puts(const wchar_t *str,int font=0,int align=0);
	/// Get width of text string for font specified by integer constant
	float Width(const wchar_t *str,int font=0);
	/// Return number of glyphs
	inline unsigned GetNumGlyph()	{	return numg;	};
protected:
	wchar_t *id;		///< Unicode ID for glyph
	unsigned *tr[4];	///< Shift of glyph description by triangles (for solid font)
	unsigned *ln[4];	///< Shift of glyph description by lines (for wire font)
	short *numt[4];		///< Number of triangles in glyph description (for solid font)
	short *numl[4];		///< Number of lines in glyph description (for wire font)
	short *width[4];	///< Width of glyph for wire font
	float fact[4];		///< Divider for width of glyph
//	float factor;		///< Factor for normalizing the font
	unsigned numg;		///< Number of glyphs
	short *buf;	///< Buffer for glyph descriptions

	/// Draw string recursively
	/* x,y - position, f - factor, style: 0x1 - italic, 0x2 - bold, 0x4 - overline, 0x8 - underline, 0x10 - empty (not draw) */
	float Puts(const unsigned *str, float x,float y,float f,int style);
	/// Replace TeX symbols by its UTF code and add font styles
	void Convert(const wchar_t *str, unsigned *res);
	/// Parse LaTeX command
	unsigned Parse(const wchar_t *s);
	/// Get internal code for symbol
	long Internal(unsigned s);
	
	/// Convert string to internal kod, also recognize TeX command
	void Convert(const wchar_t *str,unsigned *kod,int font);
	/// Get symbol for character \a ch with given \a font style
	unsigned Symbol(char ch);
private:
	void read_data(FILE *fp, float *ff, short *wdt, short *numl, unsigned *posl, short *numt, unsigned *post, unsigned &cur);
	void main_copy();
	bool read_main(const char *base, const char *path, unsigned &cur);
	void mem_alloc();
	bool read_def(unsigned &cur);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
