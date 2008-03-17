/* data_hdf.cpp is part of Math Graphic Library
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
#include "mgl/mgl_data.h"
#include <hdf5.h>
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
int mgl_save_hdf(const char *fname,const char *data,bool rewrite,
	float *a, int nx, int ny, int nz)
{
	hid_t hf,hd,hs;
	hsize_t dims[3];
	long rank = 3, res;

	H5Eset_auto(0,0);
	res=H5Fis_hdf5(fname);
	if(res>0 && !rewrite)
		hf = H5Fopen(fname, H5F_ACC_RDWR, H5P_DEFAULT);
	else
		hf = H5Fcreate(fname, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	if(hf<0)	return 1;
	if(nz==1 && ny == 1)
	{	rank = 1;	dims[0] = nx;	}
	else if(nz==1)
	{	rank = 2;	dims[0] = ny;	dims[1] = nx;	}
	else
	{	rank = 3;	dims[0] = nz;	dims[1] = ny;	dims[2] = nx;	}
	hs = H5Screate_simple(rank, dims, 0);
	hd = H5Dcreate(hf, data, H5T_IEEE_F32LE, hs, H5P_DEFAULT);

	H5Dwrite(hd, H5T_NATIVE_FLOAT, hs, hs, H5P_DEFAULT, a);

	H5Dclose(hd);	H5Sclose(hs);	H5Fclose(hf);
	return 0;
}
//-----------------------------------------------------------------------------
float *mgl_read_hdf(const char *fname,const char *data, long *nd)
{
	hid_t hf,hd,hs;
	hsize_t dims[3];
	long rank;
	float *a=NULL;
	hf = H5Fopen(fname, H5F_ACC_RDONLY, H5P_DEFAULT);
	hd = H5Dopen(hf,data);
	hs = H5Dget_space(hd);
	rank = H5Sget_simple_extent_ndims(hs);
	nd[0] = nd[1] = nd[3] = 1;
	if(rank>0 && rank<=3)
	{
		H5Sget_simple_extent_dims(hs,dims,0);
		switch(rank)
		{
		case 1:	nd[0] = dims[0];	break;
		case 2:	nd[0] = dims[1];	nd[1] = dims[0];	break;
		case 3:	nd[0] = dims[2];	nd[1] = dims[1];	nd[2] = dims[0];	break;
		}
		a = (float *)malloc(nd[0]*nd[1]*nd[2]*sizeof(float));
		H5Dread(hd, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, a);
	}
	H5Dclose(hd);	H5Sclose(hs);	H5Fclose(hf);
	return a;
}
//-----------------------------------------------------------------------------
}
#endif
