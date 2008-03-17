/* mgl_zb_jpg.cpp is part of Math Graphic Library
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
#include <stdlib.h>
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#include <jpeglib.h>
int mgl_jpeg_save(const char *fname, int w, int h, unsigned char **p)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE * outfile;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	if((outfile = fopen(fname, "wb")) == 0)	return 1;
	jpeg_stdio_dest(&cinfo, outfile);
	cinfo.image_width = w;
	cinfo.image_height = h;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	jpeg_start_compress(&cinfo, TRUE);

    jpeg_write_scanlines(&cinfo, p, h);

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose(outfile);
	return 0;
}
}
#endif
