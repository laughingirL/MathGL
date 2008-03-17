/* mgl_zb_tiff.cpp is part of Math Graphic Library
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
#ifdef __cplusplus
extern "C" {
#include <tiffio.h>     /* Sam Leffler's libtiff library. */
//-----------------------------------------------------------------------------
int mgl_tiff_save(const char *fname, int w, int h, unsigned char **p)
{
	TIFF *file;
	long i;
	file = TIFFOpen(fname, "w");
	if (file == 0)	return 1;
	
	TIFFSetField(file, TIFFTAG_IMAGEWIDTH, w);
	TIFFSetField(file, TIFFTAG_IMAGELENGTH, h);
	TIFFSetField(file, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(file, TIFFTAG_COMPRESSION, 1);
	TIFFSetField(file, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	TIFFSetField(file, TIFFTAG_SAMPLESPERPIXEL, 3);
	TIFFSetField(file, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(file, TIFFTAG_ROWSPERSTRIP, 1);
//	TIFFSetField(file, TIFFTAG_IMAGEDESCRIPTION, descr);
	
	for (i = h - 1; i >= 0; i--)
	{
		if(TIFFWriteScanline(file, p[i], i, 0) < 0)
		{
			TIFFClose(file);
			return 1;
		}
	}
	TIFFClose(file);
	return 0;
}
}
#endif
