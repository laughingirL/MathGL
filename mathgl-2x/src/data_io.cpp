/***************************************************************************
 * data_io.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <zlib.h>
#ifdef HAVE_HDF5
#include <hdf5.h>
#endif
#ifdef HAVE_HDF4
#define intf hdf4_intf
#include <hdf/mfhdf.h>
#undef intf
#endif

#ifndef WIN32
#include <glob.h>
#endif

#include "mgl/data.h"
#include "mgl/eval.h"

//#define isn(ch)		((ch)<' ' && (ch)!='\t')
#define isn(ch)		((ch)=='\n')
//-----------------------------------------------------------------------------
HMDT mgl_create_data()	{	return new mglData;	}
HMDT mgl_create_data_size(long nx, long ny, long nz){	return new mglData(nx,ny,nz);	}
HMDT mgl_create_data_file(const char *fname)		{	return new mglData(fname);	}
void mgl_delete_data(HMDT d)	{	if(d)	delete d;	}
//-----------------------------------------------------------------------------
uintptr_t mgl_create_data_()
{	return uintptr_t(new mglData());	};
uintptr_t mgl_create_data_size_(int *nx, int *ny, int *nz)
{	return uintptr_t(new mglData(*nx,*ny,*nz));	};
uintptr_t mgl_create_data_file_(const char *fname,int l)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	uintptr_t r = uintptr_t(new mglData(s));	delete []s;	return r;	}
void mgl_delete_data_(uintptr_t *d)
{	if(_DT_)	delete _DT_;	};
//-----------------------------------------------------------------------------
void mglFromStr(HMDT d,char *buf,long NX,long NY,long NZ)
{
	if(NX<1 || NY <1 || NZ<1)	return;
	mgl_data_create(d, NX,NY,NZ);
	long nb = strlen(buf);
	register long i=0, j=0, k=0;
	while(j<nb)
	{
		while(buf[j]<=' ' && j<nb)	j++;
		while(buf[j]=='#')		// skip comment
		{
			if(i>0 || buf[j+1]!='#')	// this is columns id
				while(!isn(buf[j]) && j<nb)	j++;
			else
			{
				while(!isn(buf[j]) && j<nb)
				{
					if(buf[j]>='a' && buf[j]<='z')
						d->id[k++] = buf[j];
					j++;
				}
			}
			while(buf[j]<=' ' && j<nb)	j++;
		}
		char *s=buf+j;
		while(buf[j]>' ' && j<nb)	j++;
		buf[j]=0;
		d->a[i] = atof(s);
		i++;	if(i>=NX*NY*NZ)	break;
	}
}
//-----------------------------------------------------------------------------
void mgl_data_set(HMDT d, HCDT a)
{
	const mglData *dd = dynamic_cast<const mglData *>(a);	// NOTE: faster for mglData
	mgl_data_create(d, a->GetNx(), a->GetNy(), a->GetNz());
	if(dd)	// this one should be much faster
		memcpy(d->a, dd->a, d->nx*d->ny*d->nz*sizeof(mreal));
	else	// NOTE: very inefficient!!!
	{
		register long i,j,k;
		for(k=0;k<d->nz;k++)	for(j=0;j<d->ny;j++)	for(i=0;i<d->nx;i++)
			d->a[i+d->nx*(j+d->ny*k)] = a->v(i,j,k);
	}
}
void mgl_data_set_(uintptr_t *d, uintptr_t *a)	{	mgl_data_set(_DT_,_DA_(a));	}
//-----------------------------------------------------------------------------
void mgl_data_set_values(HMDT d, const char *v,long NX,long NY,long NZ)
{
	if(NX<1 || NY <1 || NZ<1)	return;
	register long n=strlen(v)+1;
	char *buf = new char[n];
	memcpy(buf,v,n);
	mglFromStr(d,buf,NX,NY,NZ);
	delete []buf;
}
void mgl_data_set_values_(uintptr_t *d, const char *val, int *nx, int *ny, int *nz, int l)
{	char *s=new char[l+1];	memcpy(s,val,l);	s[l]=0;
	mgl_data_set_values(_DT_,s,*nx,*ny,*nz);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_data_set_vector(HMDT d, gsl_vector *v)
{
#ifndef NO_GSL
	if(!v || v->size<1)	return;
	mgl_data_create(d, v->size,1,1);
	for(long i=0;i<d->nx;i++)	d->a[i] = v->data[i*v->stride];
#endif
}
//-----------------------------------------------------------------------------
void mgl_data_set_matrix(HMDT d, gsl_matrix *m)
{
#ifndef NO_GSL
	if(!m || m->size1<1 || m->size2<1)	return;
	mgl_data_create(d, m->size1,m->size2,1);
	register long i,j;
	for(j=0;j<d->ny;j++)	for(i=0;i<d->nx;i++)
		d->a[i+j*d->nx] = m->data[i * m->tda + j];
#endif
}
//-----------------------------------------------------------------------------
void mgl_data_set_float(HMDT d, const float *A,long NX,long NY,long NZ)
{
	if(NX<=0 || NY<=0 || NZ<=0)	return;
	mgl_data_create(d, NX,NY,NZ);	if(!A)	return;
#if(MGL_USE_DOUBLE==1)
	for(long i=0;i<NX*NY*NZ;i++)	d->a[i] = A[i];
#else
	memcpy(d->a,A,NX*NY*NZ*sizeof(float));
#endif
}
//-----------------------------------------------------------------------------
void mgl_data_set_double(HMDT d, const double *A,long NX,long NY,long NZ)
{
	if(NX<=0 || NY<=0 || NZ<=0)	return;
	mgl_data_create(d, NX,NY,NZ);	if(!A)	return;
#if(MGL_USE_DOUBLE==1)
	memcpy(d->a,A,NX*NY*NZ*sizeof(double));
#else
	for(long i=0;i<NX*NY*NZ;i++)	d->a[i] = A[i];
#endif
}
//-----------------------------------------------------------------------------
void mgl_data_set_float2(HMDT d, const float **A,long N1,long N2)
{
	if(N1<=0 || N2<=0)	return;
	mgl_data_create(d, N2,N1,1);	if(!A)	return;
#if(MGL_USE_DOUBLE==1)
	for(long i=0;i<N1;i++)	for(long j=0;j<N2;j++)	d->a[j+i*N2] = A[i][j];
#else
	for(long i=0;i<N1;i++)	memcpy(d->a+i*N2,A[i],N2*sizeof(float));
#endif
}
//-----------------------------------------------------------------------------
void mgl_data_set_double2(HMDT d, const double **A,long N1,long N2)
{
	if(N1<=0 || N2<=0)	return;
	mgl_data_create(d, N2,N1,1);	if(!A)	return;
#if(MGL_USE_DOUBLE==1)
	for(long i=0;i<N1;i++)	memcpy(d->a+i*N2,A[i],N2*sizeof(double));
#else
	for(long i=0;i<N1;i++)	for(long j=0;j<N2;j++)	d->a[j+i*N2] = A[i][j];
#endif
}
//-----------------------------------------------------------------------------
void mgl_data_set_float3(HMDT d, const float ***A,long N1,long N2,long N3)
{
	if(N1<=0 || N2<=0 || N3<=0)	return;
	mgl_data_create(d, N3,N2,N1);	if(!A)	return;
#if(MGL_USE_DOUBLE==1)
	for(long i=0;i<N1;i++)	for(long j=0;j<N2;j++)	for(long k=0;k<N3;k++)
		d->a[k+N3*(j+i*N2)] = A[i][j][k];
#else
	for(long i=0;i<N1;i++)	for(long j=0;j<N2;j++)
		memcpy(d->a+N3*(j+i*N2),A[i][j],N3*sizeof(float));
#endif
}
//-----------------------------------------------------------------------------
void mgl_data_set_double3(HMDT d, const double ***A,long N1,long N2,long N3)
{
	if(N1<=0 || N2<=0 || N3<=0)	return;
	mgl_data_create(d, N3,N2,N1);	if(!A)	return;
#if(MGL_USE_DOUBLE==1)
	for(long i=0;i<N1;i++)	for(long j=0;j<N2;j++)
		memcpy(d->a+N3*(j+i*N2),A[i][j],N3*sizeof(double));
#else
	for(long i=0;i<N1;i++)	for(long j=0;j<N2;j++)	for(long k=0;k<N3;k++)
		d->a[k+N3*(j+i*N2)] = A[i][j][k];
#endif
}
//-----------------------------------------------------------------------------
void mgl_data_set_float1_(uintptr_t *d, const float *A,int *NX)
{	mgl_data_set_float(_DT_,A,*NX,1,1);	}
void mgl_data_set_double1_(uintptr_t *d, const double *A,int *NX)
{	mgl_data_set_double(_DT_,A,*NX,1,1);	}
void mgl_data_set_float_(uintptr_t *d, const float *A,int *NX,int *NY,int *NZ)
{	mgl_data_set_float(_DT_,A,*NX,*NY,*NZ);	}
void mgl_data_set_double_(uintptr_t *d, const double *A,int *NX,int *NY,int *NZ)
{	mgl_data_set_double(_DT_,A,*NX,*NY,*NZ);	}
void mgl_data_set_float2_(uintptr_t *d, const float *A,int *N1,int *N2)
{	mgl_data_set_float(_DT_,A,*N1,*N2,1);	}
void mgl_data_set_double2_(uintptr_t *d, const double *A,int *N1,int *N2)
{	mgl_data_set_double(_DT_,A,*N1,*N2,1);	}
void mgl_data_set_float3_(uintptr_t *d, const float *A,int *N1,int *N2,int *N3)
{	mgl_data_set_float(_DT_,A,*N1,*N2,*N3);	}
void mgl_data_set_double3_(uintptr_t *d, const double *A,int *N1,int *N2,int *N3)
{	mgl_data_set_double(_DT_,A,*N1,*N2,*N3);	}
//-----------------------------------------------------------------------------
void mgl_data_rearrange(HMDT d, long mx,long my,long mz)
{
	if(mx<1)	return;	// wrong mx
	if(my<1)	{	my = d->nx*d->ny*d->nz/mx;	mz = 1;	}
	else if(mz<1)	mz = (d->nx*d->ny*d->nz)/(mx*my);
	long m = mx*my*mz;
	if(m==0 || m>d->nx*d->ny*d->nz)	return;	// too high desired dimensions
	d->nx = mx;	d->ny = my;	d->nz = mz;	d->NewId();
}
void mgl_data_rearrange_(uintptr_t *d, int *mx, int *my, int *mz)
{	mgl_data_rearrange(_DT_,*mx,*my,*mz);	}
//-----------------------------------------------------------------------------
void mgl_data_set_id(HMDT d, const char *ids)
{
	d->NewId();	// clearing + be sure about correct length
	if(ids)	for(long i=0;i<d->nx && ids[i]!=0;i++)	d->id[i]=ids[i];
}
void mgl_data_set_id_(uintptr_t *d, const char *eq,int l)
{	char *s=new char[l+1];	memcpy(s,eq,l);	s[l]=0;
	mgl_data_set_id(_DT_, s);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_data_save(HCDT d, const char *fname,long ns)
{
	FILE *fp;
	fp = fopen(fname,"w");
	register long i,j,k;
	long nx=d->GetNx(), ny=d->GetNy(), nz=d->GetNz();
	if(ns<0 || (ns>=nz && nz>1))	for(k=0;k<nz;k++)
	{	// ñîõðàíÿåì âñå äàííûå
		for(i=0;i<ny;i++)
		{
			for(j=0;j<nx;j++)	fprintf(fp,"%g\t",d->v(j,i,k));
			fprintf(fp,"\n");
		}
		fprintf(fp,"\n");
	}
	else
	{	// ñîõðàíÿåì òîëüêî ñðåç
		if(nz>1)	for(i=0;i<ny;i++)
		{
			for(j=0;j<nx;j++)	fprintf(fp,"%g\t",d->v(j,i,ns));
			fprintf(fp,"\n");
		}
		else if(ns<ny)	for(long j=0;j<nx;j++)
			fprintf(fp,"%g\t",d->v(j,ns));
	}
	fclose(fp);
}
void mgl_data_save_(uintptr_t *d, const char *fname,int *ns,int l)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	mgl_data_save(_DT_,s,*ns);		delete []s;	}
//-----------------------------------------------------------------------------
char *mgl_read_gz(gzFile fp)
{
	long size=1024,n=0,m;
	char *buf=(char*)malloc(size);
	while((m=gzread(fp,buf+size*n,size))>0)
	{
		if(m<size)	{	buf[size*n+m]=0;	break;	}
		n++;		buf=(char*)realloc(buf,size*(n+1));
	}
	return buf;
}
//-----------------------------------------------------------------------------
int mgl_data_read(HMDT d, const char *fname)
{
	long l=1,m=1,k=1;
	long nb,i;
	gzFile fp = gzopen(fname,"r");
	if(!fp)
	{
		if(!d->a)	mgl_data_create(d, 1,1,1);
		return	false;
	}
	char *buf = mgl_read_gz(fp);
	nb = strlen(buf);	gzclose(fp);

	bool first=false,com=false;
	register char ch;
	for(i=nb-1;i>=0;i--)	if(buf[i]>' ')	break;
	buf[i+1]=0;	nb = i;		// remove tailing spaces
	for(i=0;i<nb-1 && !isn(buf[i]);i++)	// determine nx
	{
		if(buf[i]=='#')		while(!isn(buf[i]) && i<nb)	i++;
		ch = buf[i];
		if(ch>' ' && !first)	first=true;
		if(first && (ch==' ' || ch=='\t') && buf[i+1]>' ') k++;
	}
	first = false;
	for(i=0;i<nb-1;i++)					// determine ny
	{
		ch = buf[i];
		if(ch=='#')	while(!isn(buf[i]) && i<nb)	i++;
		if(isn(ch))
		{
			if(isn(buf[i+1]))	{first=true;	break;	}
			m++;
		}
		if(ch=='\f')	break;
	}
	if(first)	for(i=0;i<nb-1;i++)		// determine nz
	{
		ch = buf[i];
		if(ch=='#')	com = true;	// comment
		if(isn(ch))
		{
			if(com)	{	com=false;	continue;	}
			if(isn(buf[i+1]))	l++;
		}
	}
	else	for(i=0;i<nb-1;i++)	if(buf[i]=='\f')	l++;
	mglFromStr(d,buf,k,m,l);
	free(buf);	return true;
}
int mgl_data_read_(uintptr_t *d, const char *fname,int l)
{	char *s=new char[l+1];		memcpy(s,fname,l);	s[l]=0;
	int r = mgl_data_read(_DT_, s);	delete []s;		return r;	}
//-----------------------------------------------------------------------------
void mgl_data_create(HMDT d,long mx,long my,long mz)
{
	d->nx = mx>0 ? mx:1;	d->ny = my>0 ? my:1;	d->nz = mz>0 ? mz:1;
	if(d->id && d->a)	delete []d->id;
	if(!d->link && d->a)	delete []d->a;
	d->a = new mreal[mx*my*mz];
	d->id = new char[mx];	d->link=false;
	memset(d->a,0,mx*my*mz*sizeof(mreal));
	memset(d->id,0,mx*sizeof(char));
}
void mgl_data_create_(uintptr_t *d, int *nx,int *ny,int *nz)
{	mgl_data_create(_DT_,*nx,*ny,*nz);	}
//-----------------------------------------------------------------------------
void mgl_data_link(HMDT d, float *A, long mx,long my,long mz)
{
	if(!A)	return;
	if(d->id && d->a)		delete []d->id;
	if(!d->link && d->a)	delete []d->a;
	d->nx = mx>0 ? mx:1;	d->ny = my>0 ? my:1;	d->nz = mz>0 ? mz:1;
	d->link=true;	d->NewId();
}
void mgl_data_link_(uintptr_t *d, float *A, int *nx,int *ny,int *nz)
{	mgl_data_link(_DT_,A,*nx,*ny,*nz);	}
//-----------------------------------------------------------------------------
int mgl_data_read_dim(HMDT d, const char *fname,long mx,long my,long mz)
{
	if(mx<=0 || my<=0 || mz<=0)	return false;
	gzFile fp = gzopen(fname,"r");
	if(!fp)	return false;
	char *buf = mgl_read_gz(fp);
	gzclose(fp);
	mglFromStr(d,buf,mx,my,mz);
	free(buf);	return true;
}
int mgl_data_read_dim_(uintptr_t *d, const char *fname,int *mx,int *my,int *mz,int l)
{	char *s=new char[l+1];	memcpy(s,fname,l);	s[l]=0;
	int r = mgl_data_read_dim(_DT_,s,*mx,*my,*mz);	delete []s;	return r;	}
//-----------------------------------------------------------------------------
int mgl_data_read_mat(HMDT d, const char *fname, long dim)
{
	if(dim<=0 || dim>3)	return false;
	gzFile fp = gzopen(fname,"r");
	if(!fp)	return false;
	long nx=1, ny=1, nz=1;
	char *buf = mgl_read_gz(fp);
	long nb = strlen(buf);	gzclose(fp);

	register long j=0;
	while(j<nb)
	{
		if(buf[j]=='#')	while(!isn(buf[j]))	j++;	// skip comment
		while(buf[j]<=' ' && j<nb)	j++;
		break;
	}
	if(dim==1)
	{
		sscanf(buf+j,"%ld",&nx);
		while(buf[j]>' ')	j++;
	}
	else if(dim==2)
	{
		sscanf(buf+j,"%ld%ld",&nx,&ny);
		while(buf[j]>' ' && j<nb)	j++;
		while(buf[j]<=' ' && j<nb)	j++;
		while(buf[j]>' ' && j<nb)	j++;
	}
	else if(dim==3)
	{
		sscanf(buf+j,"%ld%ld%ld",&nx,&ny,&nz);
		while(buf[j]>' ' && j<nb)	j++;
		while(buf[j]<=' ' && j<nb)	j++;
		while(buf[j]>' ' && j<nb)	j++;
		while(buf[j]<=' ' && j<nb)	j++;
		while(buf[j]>' ' && j<nb)	j++;
	}
	mglFromStr(d,buf+j,nx,ny,nz);
	free(buf);	return true;
}
int mgl_data_read_mat_(uintptr_t *d, const char *fname,int *dim,int l)
{	char *s=new char[l+1];		memcpy(s,fname,l);	s[l]=0;
	int r = mgl_data_read_mat(_DT_,s,*dim);	delete []s;	return r;	}
//-----------------------------------------------------------------------------
mreal mgl_data_max(HCDT d)
{
	register mreal m=-1e10;
	for(long i=0;i<d->GetNN();i++)	m = m > d->vthr(i) ? m : d->vthr(i);
	return m;
}
mreal mgl_data_max_(uintptr_t *d)	{	return mgl_data_max(_DT_);	}
//-----------------------------------------------------------------------------
mreal mgl_data_min(HCDT d)
{
	register mreal m=1e10;
	for(long i=0;i<d->GetNN();i++)	m = m < d->vthr(i) ? m : d->vthr(i);
	return m;
}
mreal mgl_data_min_(uintptr_t *d)	{	return mgl_data_min(_DT_);	}
//-----------------------------------------------------------------------------
mreal mgl_data_max_int(HCDT d, long *i, long *j, long *k)
{
	register mreal m=-1e10, v;
	long nx=d->GetNx(), ny=d->GetNy();
	for(long ii=0;ii<d->GetNN();ii++)
	{
		v = d->vthr(ii);
		if(!isnan(v) && m < v)
		{	m=v;	*i=ii%nx;	*j=(ii/nx)%ny;	*k=ii/(nx*ny);   }
	}
	return m;
}
mreal mgl_data_max_int_(uintptr_t *d, int *i, int *j, int *k)
{	long ii,jj,kk;	float res=mgl_data_max_int(_DT_,&ii,&jj,&kk);
	*i=ii;	*j=jj;	*k=kk;	return res;	}
//-----------------------------------------------------------------------------
mreal mgl_data_min_int(HCDT d, long *i, long *j, long *k)
{
	register mreal m=1e10, v;
	long nx=d->GetNx(), ny=d->GetNy();
	for(long ii=0;ii<d->GetNN();ii++)
	{
		v = d->vthr(ii);
		if(!isnan(v) && m > v)
		{	m=v;	*i=ii%nx;	*j=(ii/nx)%ny;	*k=ii/(nx*ny);   }
	}
	return m;
}
mreal mgl_data_min_int_(uintptr_t *d, int *i, int *j, int *k)
{	long ii,jj,kk;	float res=mgl_data_min_int(_DT_,&ii,&jj,&kk);
	*i=ii;	*j=jj;	*k=kk;	return res;	}
//-----------------------------------------------------------------------------
mreal mgl_data_max_real(HCDT d, mreal *x, mreal *y, mreal *z)
{
	long im=-1,jm=-1,km=-1;
	long nx=d->GetNx(), ny=d->GetNy(), nz=d->GetNz();
	mreal m=mgl_data_max_int(d,&im,&jm,&km), v, v1, v2;
	*x=im;	*y=jm;	*z=km;

	v = d->v(im,jm,km);
	if(nx>2)
	{
		if(im==0)	im=1;
		if(im==nx-1)im=nx-2;
		v1 = d->v(im+1,jm,km);	v2 = d->v(im-1,jm,km);
		*x = (v1+v2-2*v)==0 ? im : im+(v1-v2)/(v1+v2-2*v)/2;
	}
	if(ny>2)
	{
		if(jm==0)	jm=1;
		if(jm==ny-1)jm=ny-2;
		v1 = d->v(im,jm+1,km);	v2 = d->v(im,jm-1,km);
		*y = (v1+v2-2*v)==0 ? jm : jm+(v1-v2)/(v1+v2-2*v)/2;
	}
	if(nz>2)
	{
		if(km==0)	km=1;
		if(km==nz-1)km=nz-2;
		v1 = d->v(im,jm,km+1);	v2 = d->v(im,jm,km-1);
		*z = (v1+v2-2*v)==0 ? km : km+(v1-v2)/(v1+v2-2*v)/2;
	}
	return m;
}
mreal mgl_data_max_real_(uintptr_t *d, mreal *x, mreal *y, mreal *z)
{	return mgl_data_max_real(_DT_,x,y,z);	}
//-----------------------------------------------------------------------------
mreal mgl_data_min_real(HCDT d, mreal *x, mreal *y, mreal *z)
{
	long im=-1,jm=-1,km=-1;
	long nx=d->GetNx(), ny=d->GetNy(), nz=d->GetNz();
	mreal m=mgl_data_min_int(d,&im,&jm,&km), v, v1, v2;
	*x=im;	*y=jm;	*z=km;

	v = d->v(im,jm,km);
	if(nx>2)
	{
		if(im==0)	im=1;
		if(im==nx-1)im=nx-2;
		v1 = d->v(im+1,jm,km);	v2 = d->v(im-1,jm,km);
		*x = (v1+v2-2*v)==0 ? im : im+(v1-v2)/(v1+v2-2*v)/2;
	}
	if(ny>2)
	{
		if(jm==0)	jm=1;
		if(jm==ny-1)jm=ny-2;
		v1 = d->v(im,jm+1,km);	v2 = d->v(im,jm-1,km);
		*y = (v1+v2-2*v)==0 ? jm : jm+(v1-v2)/(v1+v2-2*v)/2;
	}
	if(nz>2)
	{
		if(km==0)	km=1;
		if(km==nz-1)km=nz-2;
		v1 = d->v(im,jm,km+1);	v2 = d->v(im,jm,km-1);
		*z = (v1+v2-2*v)==0 ? km : km+(v1-v2)/(v1+v2-2*v)/2;
	}
	return m;
}
mreal mgl_data_min_real_(uintptr_t *d, mreal *x, mreal *y, mreal *z)
{	return mgl_data_min_real(_DT_,x,y,z);	}
//-----------------------------------------------------------------------------
void *mgl_fill_x(void *par)
{
	mglThread *t=(mglThread *)par;
	register long i0, nx=t->p[0],ny=t->p[1];
	mreal *b=t->a, x1=t->b[0], dx=t->b[1];
	register char dir = t->s[0];
	for(i0=t->id;i0<t->n;i0+=mglNumThr)
	{
		if(dir=='x') b[i0] = x1+dx*(i0%nx);
		else if(dir=='y') b[i0] = x1+dx*((i0/nx)%ny);
		else if(dir=='z') b[i0] = x1+dx*(i0/(nx*ny));
	}
	return 0;
}
void mgl_data_fill(HMDT d, float x1,float x2,char dir)
{
	if(isnan(x2))	{	dir='x';	x2=x1;	}
	if(dir<'x' || dir>'z')	dir='x';
	long par[2]={d->nx,d->ny};
	mreal b[2]={x1,x2-x1};
	if(dir=='x')	b[1] *= d->nx>1 ? 1./(d->nx-1):0;
	if(dir=='y')	b[1] *= d->ny>1 ? 1./(d->ny-1):0;
	if(dir=='z')	b[1] *= d->nz>1 ? 1./(d->nz-1):0;
	mglStartThread(mgl_fill_x,0,d->nx*d->ny*d->nz,d->a,b,0,par,0,0,0,&dir);
}
void mgl_data_fill_(uintptr_t *d, float *x1,float *x2,const char *dir,int)
{	mgl_data_fill(_DT_,*x1,*x2,*dir);	}
//-----------------------------------------------------------------------------
void mgl_data_norm(HMDT d, float v1,float v2,long sym,long dim)
{
	long i,s,nn=d->nx*d->ny*d->nz;
	mreal a1=1e20,a2=-1e20,v,*a=d->a;
	if(d->nz>1)	s = dim*d->nx*d->ny;
	else		s = dim*d->ny;
	for(i=s;i<nn;i++)	// determines borders of existing data
	{
		if(isnan(a[i]))	continue;
		a1 = (a1<a[i] ? a1 : a[i]);	a2 = (a2>a[i] ? a2 : a[i]);
	}
	if(a1==a2)  {  if(a1!=0)	a1=0.;  else a2=1;  }
	if(v1>v2)	{	v=v1;	v1=v2;	v2=v;	}	// swap if uncorrect
	if(sym)				// use symmetric
	{
		v2 = -v1>v2 ? -v1:v2;	v1 = -v2;
		a2 = -a1>a2 ? -a1:a2;	a1 = -a2;
	}
	for(i=s;i<nn;i++)	// normalize
		a[i] = v1 + (v2-v1)*(a[i]-a1)/(a2-a1);
}
void mgl_data_norm_(uintptr_t *d, float *v1,float *v2,int *sym,int *dim)
{	mgl_data_norm(_DT_,*v1,*v2,*sym,*dim);	}
//-----------------------------------------------------------------------------
void mgl_data_squeeze(HMDT d, long rx,long ry,long rz,long smooth)
{
	long kx,ky,kz,i,j,k;
	long nx=d->nx, ny=d->ny, nz=d->nz;
	mreal *b;

	// simple checking
	if(rx>=nx)	rx=nx-1;	if(rx<1)	rx=1;
	if(ry>=ny)	ry=ny-1;	if(ry<1)	ry=1;
	if(rz>=nz)	rz=nz-1;	if(rz<1)	rz=1;
	// new sizes
	kx = 1+(nx-1)/rx;	ky = 1+(ny-1)/ry;	kz = 1+(nz-1)/rz;
	b = new mreal[kx*ky*kz];
	if(!smooth)	for(k=0;k<kz;k++)	for(j=0;j<ky;j++)	for(i=0;i<kx;i++)
		b[i+kx*(j+ky*k)] = d->a[i*rx+nx*(j*ry+ny*rz*k)];
	else		for(k=0;k<kz;k++)	for(j=0;j<ky;j++)	for(i=0;i<kx;i++)
	{
		long dx,dy,dz,i1,j1,k1;
		dx = (i+1)*rx<=nx ? rx : nx-i*rx;
		dy = (j+1)*ry<=ny ? ry : ny-j*ry;
		dz = (k+1)*rz<=nz ? rz : nz-k*rz;
		mreal s = 0;
		for(k1=k*rz;k1<k*rz+dz;k1++)	for(j1=j*ry;j1<j*ry+dz;j1++)	for(i1=i*rx;i1<i*rx+dx;i1++)
			s += d->a[i1+nx*(j1+ny*k1)];
		b[i+kx*(j+ky*k)] = s/dx*dy*dz;
	}
	if(!d->link)	delete []d->a;
	d->a=b;	d->nx = kx;  d->ny = ky;  d->nz = kz;	d->NewId();	d->link=false;
}
void mgl_data_squeeze_(uintptr_t *d, int *rx,int *ry,int *rz,int *smooth)
{	mgl_data_squeeze(_DT_,*rx,*ry,*rz,*smooth);	}
//-----------------------------------------------------------------------------
void mgl_data_extend(HMDT d, long n1, long n2)
{
	long nx=d->nx, ny=d->ny, nz=d->nz;
	if(nz>2 || n1==0)	return;
	long mx,my,mz;
	mreal *b=0;
	register long i,j;
	if(n1>0) // extend to higher dimension(s)
	{
		n2 = n2>0 ? n2:1;
		mx = nx;	my = ny>1?ny:n1;	mz = ny>1 ? n1 : n2;
		b = new mreal[mx*my*mz];
		if(ny>1)	for(i=0;i<n1;i++)
			memcpy(b+i*nx*ny, d->a, nx*ny*sizeof(mreal));
		else		for(i=0;i<n1*n2;i++)
			memcpy(b+i*nx, d->a, nx*sizeof(mreal));
	}
	else
	{
		mx = -n1;	my = n2<0 ? -n2 : nx;	mz = n2<0 ? nx : ny;
		if(n2>0 && ny==1)	mz = n2;
		b = new mreal[mx*my*mz];
		if(n2<0)	for(j=0;j<nx;j++)	for(i=0;i<mx*my;i++)
			b[i+mx*my*j] = d->a[j];
		else	for(j=0;j<nx*ny;j++)	for(i=0;i<mx;i++)
			b[i+mx*j] = d->a[j];
		if(n2>0 && ny==1)	for(i=0;i<n2;i++)
			memcpy(b+i*mx*my, d->a, mx*my*sizeof(mreal));
	}
	if(b)
	{	if(!d->link)	delete []d->a;
		d->a=b;	d->nx=mx;	d->ny=my;	d->nz=mz;	d->NewId();	d->link=false;	}
}
void mgl_data_extend_(uintptr_t *d, int *n1, int *n2)
{	mgl_data_extend(_DT_,*n1,*n2);	}
//-----------------------------------------------------------------------------
void mgl_data_transpose(HMDT d, const char *dim)
{
	long nx=d->nx, ny=d->ny, nz=d->nz;
	mreal *b=new mreal[nx*ny*nz], *a=d->a;
	register long i,j,k,n;
	if(!strcmp(dim,"xyz"))	memcpy(b,a,nx*ny*nz*sizeof(mreal));
	else if(!strcmp(dim,"xzy") || !strcmp(dim,"zy"))
	{
		for(j=0;j<ny;j++)	for(k=0;k<nz;k++)	for(i=0;i<nx;i++)
			b[i+nx*(k+nz*j)] = a[i+nx*(j+ny*k)];
		n=nz;	nz=ny;	ny=n;
	}
	else if(!strcmp(dim,"yxz") || !strcmp(dim,"yx"))
	{
		for(k=0;k<nz;k++)	for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
			b[j+ny*(i+nx*k)] = a[i+nx*(j+ny*k)];
		n=nx;	nx=ny;	ny=n;
	}
	else if(!strcmp(dim,"yzx"))
	{
		for(k=0;k<nz;k++)	for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
			b[j+ny*(k+nz*i)] = a[i+nx*(j+ny*k)];
		n=nx;	nx=ny;	ny=nz;	nz=n;
	}
	else if(!strcmp(dim,"zxy"))
	{
		for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
			b[k+nz*(i+nx*j)] = a[i+nx*(j+ny*k)];
		n=nx;	nx=nz;	nz=ny;	ny=n;
	}
	else if(!strcmp(dim,"zyx") || !strcmp(dim,"zx"))
	{
		for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
			b[k+nz*(j+ny*i)] = a[i+nx*(j+ny*k)];
		n=nz;	nz=nx;	nx=n;
	}
	memcpy(a,b,nx*ny*nz*sizeof(mreal));	delete []b;
	n=d->nx;	d->nx=nx;	d->ny=ny;	d->nz=nz;
	if(nx!=n)	d->NewId();
}
void mgl_data_transpose_(uintptr_t *d, const char *dim,int l)
{	char *s=new char[l+1];	memcpy(s,dim,l);	s[l]=0;
	mgl_data_transpose(_DT_,s);	delete []s;	}
//-----------------------------------------------------------------------------
void *mgl_modify(void *par)
{
	mglThread *t=(mglThread *)par;
	mglFormula *f = (mglFormula *)(t->v);
	register long i,j,k,i0, nx=t->p[0],ny=t->p[1],nz=t->p[2];
	mreal *b=t->a, dx,dy,dz;
	const mreal *v=t->b, *w=t->c;
	dx=nx>1?1/(nx-1.):0;	dy=ny>1?1/(ny-1.):0;	dz=nz>1?1/(nz-1.):0;
	for(i0=t->id;i0<t->n;i0+=mglNumThr)
	{
		i=i0%nx;	j=((i0/nx)%ny);	k=i0/(nx*ny);
		b[i0] = f->Calc(i*dx, j*dy, k*dz, b[i0], v?v[i0]:0, w?w[i0]:0);
	}
	return 0;
}
void mgl_data_modify(HMDT d, const char *eq,long dim)
{
	long nx=d->nx, ny=d->ny, nz=d->nz, par[3]={nx,ny,nz};
	mglFormula f(eq);
	if(dim<0)	dim=0;
	if(nz>1)	// 3D array
	{
		par[2] -= dim;	if(par[2]<0)	par[2]=0;
		mglStartThread(mgl_modify,0,nx*ny*par[2],d->a+nx*ny*dim,0,0,par,&f);
	}
	else		// 2D or 1D array
	{
		par[1] -= dim;	if(par[1]<0)	par[1]=0;
		mglStartThread(mgl_modify,0,nx*par[1],d->a+nx*dim,0,0,par,&f);
	}
}
void mgl_data_modify_(uintptr_t *d, const char *eq,int *dim,int l)
{	char *s=new char[l+1];	memcpy(s,eq,l);	s[l]=0;
	mgl_data_modify(_DT_,s,*dim);	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_data_modify_vw(HMDT d, const char *eq,HCDT vdat,HCDT wdat)
{
	const mglData *v = dynamic_cast<const mglData *>(vdat);	// NOTE: only for mglData
	const mglData *w = dynamic_cast<const mglData *>(wdat);
	long nn = d->nx*d->ny*d->nz, par[3]={d->nx,d->ny,d->nz};
	mglFormula f(eq);
	if(v && w && v->nx*v->ny*v->nz==nn && w->nx*w->ny*w->nz==nn)
		mglStartThread(mgl_modify,0,nn,d->a,v->a,w->a,par,&f);
	else if(v && v->nx*v->ny*v->nz==nn)
		mglStartThread(mgl_modify,0,nn,d->a,v->a,0,par,&f);
	else
		mglStartThread(mgl_modify,0,nn,d->a,0,0,par,&f);
}
void mgl_data_modify_vw_(uintptr_t *d, const char *eq, uintptr_t *v, uintptr_t *w,int l)
{	char *s=new char[l+1];	memcpy(s,eq,l);	s[l]=0;
	mgl_data_modify_vw(_DT_,s,_DA_(v),_DA_(w));	delete []s;	}
//-----------------------------------------------------------------------------
void *mgl_fill_f(void *par)
{
	mglThread *t=(mglThread *)par;
	mglFormula *f = (mglFormula *)(t->v);
	register long i,j,k,i0, nx=t->p[0],ny=t->p[1];
	mreal *b=t->a;
	const mreal *v=t->b, *w=t->c, *x=t->d;
	for(i0=t->id;i0<t->n;i0+=mglNumThr)
	{
		i=i0%nx;	j=((i0/nx)%ny);	k=i0/(nx*ny);
		b[i0] = f->Calc(x[0]+i*x[1], x[2]+j*x[3], x[4]+k*x[5], b[i0], v?v[i0]:0, w?w[i0]:0);
	}
	return 0;
}
void mgl_data_fill_eq(HMGL gr, HMDT d, const char *eq, HCDT vdat, HCDT wdat)
{
	const mglData *v = dynamic_cast<const mglData *>(vdat);	// NOTE: only for mglData
	const mglData *w = dynamic_cast<const mglData *>(wdat);
	long nn = d->nx*d->ny*d->nz, par[3]={d->nx,d->ny,d->nz};
	if(v && v->nx*v->ny*v->nz!=nn)	return;
	if(w && w->nx*w->ny*w->nz!=nn)	return;
	mreal xx[6]={gr->Min.x,0, gr->Min.y,0, gr->Min.z,0};
	if(d->nx>1)	xx[1] = (gr->Max.x-gr->Min.x)/(d->nx-1.);
	if(d->ny>1)	xx[3] = (gr->Max.y-gr->Min.y)/(d->ny-1.);
	if(d->nz>1)	xx[5] = (gr->Max.z-gr->Min.z)/(d->nz-1.);
	mglFormula f(eq);
	mglStartThread(mgl_fill_f,0,nn,d->a,v?v->a:0,w?w->a:0,par,&f,xx);
}
void mgl_data_fill_eq_(uintptr_t *gr, uintptr_t *d, const char *eq, uintptr_t *v, uintptr_t *w,int l)
{	char *s=new char[l+1];	memcpy(s,eq,l);	s[l]=0;
	mgl_data_fill_eq(_GR_,_DT_,s,_DA_(v),_DA_(w));	delete []s;	}
//-----------------------------------------------------------------------------
void mgl_data_read_hdf4(HMDT d,const char *fname,const char *data)
{
#ifdef HAVE_HDF4
	long sd = SDstart(fname,DFACC_READ), nn, i;
	if(sd==-1)	return;	// is not a HDF4 file
	char name[64];
	SDfileinfo(sd,&nn,&i);
	for(i=0;i<nn;i++)
	{
		long sds, rank, dims[32], type, attr, in[2]={0,0};
		sds = SDselect(sd,i);
		SDgetinfo(sds,name,&rank,dims,&type,&attr);
		if(!strcmp(name,data))	// as I understand there are possible many datas with the same name
		{
			if(rank==1)			{	dims[2]=dims[0];	dims[0]=dims[1]=1;	}
			else if(rank==2)	{	dims[2]=dims[1];	dims[1]=dims[0];	dims[0]=1;	}
//			else if(rank>3)		continue;
			long nn=dims[0]*dims[1]*dims[2];
			if(type==DFNT_FLOAT32)
			{
				float *b = new float[nn];
				SDreaddata(sds,in,0,dims,b);
				mgl_data_set_float(d,b,dims[2],dims[1],dims[0]);
				delete []b;
			}
			if(type==DFNT_FLOAT64)
			{
				double *b = new double[nn];
				SDreaddata(sds,in,0,dims,b);
				mgl_data_set_double(d,b,dims[2],dims[1],dims[0]);
				delete []b;
			}
		}
		SDendaccess(sds);
	}
	SDend(sd);
#endif
}
//-----------------------------------------------------------------------------
#ifdef HAVE_HDF5
void mgl_data_save_hdf(HCDT dat,const char *fname,const char *data,bool rewrite)
{
	const mglData *d = dynamic_cast<const mglData *>(dat);	// NOTE: only for mglData
	hid_t hf,hd,hs;
	hsize_t dims[3];
	long rank = 3, res;
#ifndef H5_USE_16_API
	H5Eset_auto(H5E_DEFAULT,0,0);
#else
	H5Eset_auto(0,0);
#endif
	res=H5Fis_hdf5(fname);
	if(res>0 && !rewrite)	hf = H5Fopen(fname, H5F_ACC_RDWR, H5P_DEFAULT);
	else	hf = H5Fcreate(fname, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	if(hf<0)	return;
	if(d->nz==1 && d->ny == 1)	{	rank=1;	dims[0]=d->nx;	}
	else if(d->nz==1)	{	rank=2;	dims[0]=d->ny;	dims[1]=d->nx;	}
	else	{	rank=3;	dims[0]=d->nz;	dims[1]=d->ny;	dims[2]=d->nx;	}
	hs = H5Screate_simple(rank, dims, 0);
#if(MGL_USE_DOUBLE==1)
#ifndef H5_USE_16_API
	hd = H5Dcreate(hf, data, H5T_NATIVE_DOUBLE, hs, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
#else  /* ! HAVE_HDF5_18 */
	hd = H5Dcreate(hf, data, H5T_NATIVE_DOUBLE, hs, H5P_DEFAULT);
#endif /* HAVE_HDF5_18 */
	H5Dwrite(hd, H5T_NATIVE_DOUBLE, hs, hs, H5P_DEFAULT, d->a);
#else
#ifndef H5_USE_16_API
	hd = H5Dcreate(hf, data, H5T_NATIVE_FLOAT, hs, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
#else  /* ! HAVE_HDF5_18 */
	hd = H5Dcreate(hf, data, H5T_NATIVE_FLOAT, hs, H5P_DEFAULT);
#endif /* HAVE_HDF5_18 */
	H5Dwrite(hd, H5T_NATIVE_FLOAT, hs, hs, H5P_DEFAULT, d->a);
#endif
	H5Dclose(hd);	H5Sclose(hs);	H5Fclose(hf);
}
//-----------------------------------------------------------------------------
int mgl_data_read_hdf(HMDT d,const char *fname,const char *data)
{
	hid_t hf,hd,hs;
	hsize_t dims[3];
	long rank, res = H5Fis_hdf5(fname);
	if(res<=0)	{	mgl_data_read_hdf4(d,fname,data);	return false;	}
	hf = H5Fopen(fname, H5F_ACC_RDONLY, H5P_DEFAULT);
	if(hf<0)	return false;
#ifndef H5_USE_16_API
	hd = H5Dopen(hf,data,H5P_DEFAULT);
#else
	hd = H5Dopen(hf,data);
#endif
	if(hd<0)	return false;
	hs = H5Dget_space(hd);
	rank = H5Sget_simple_extent_ndims(hs);
	if(rank>0 && rank<=3)
	{
		H5Sget_simple_extent_dims(hs,dims,0);
		if(rank==1)			{	dims[2]=dims[0];	dims[0]=dims[1]=1;	}
		else if(rank==2)	{	dims[2]=dims[1];	dims[1]=dims[0];	dims[0]=1;	}
//		else if(rank>3)		continue;
		mgl_data_create(d,dims[2],dims[1],dims[0]);
#if(MGL_USE_DOUBLE==1)
		H5Dread(hd, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, d->a);
#else
		H5Dread(hd, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, d->a);
#endif
	}
	H5Dclose(hd);	H5Sclose(hs);	H5Fclose(hf);	return true;
}
int mgl_datas_hdf(const char *fname, char *buf, long size)
{
	hid_t hf,hg,hd,ht;
	if(!buf || size<1)	return 0;
	buf[0]=0;
	hf = H5Fopen(fname, H5F_ACC_RDONLY, H5P_DEFAULT);
	if(!hf)	return 0;
#ifndef H5_USE_16_API
	hg = H5Gopen(hf,"/",H5P_DEFAULT);
#else
	hg = H5Gopen(hf,"/");
#endif
	hsize_t num, i;
	char name[256];
	long pos=0,len;
	H5Gget_num_objs(hg, &num);
	for(i=0;i<num;i++)
	{
		if(H5Gget_objtype_by_idx(hg, i)!=H5G_DATASET)	continue;
		H5Gget_objname_by_idx(hg, i, name, 256);
#ifndef H5_USE_16_API
		hd = H5Dopen(hf,name,H5P_DEFAULT);
#else
		hd = H5Dopen(hf,name);
#endif
		ht = H5Dget_type(hd);
		len = strlen(name);		if(pos+len+2>size)	break;
		if(H5Tget_class(ht)==H5T_FLOAT || H5Tget_class(ht)==H5T_INTEGER)
		{	strcat(buf,name);	strcat(buf,"\t");	pos += len+1;	}
		H5Dclose(hd);	H5Tclose(ht);
	}
	H5Gclose(hg);	H5Fclose(hf);
	return i;
}
#else
void mgl_data_save_hdf(HCDT d,const char *fname,const char *data,bool rewrite){}
int mgl_datas_hdf(const char *fname, char *buf, long size){return 0;}
int mgl_data_read_hdf(HMDT d,const char *fname,const char *data){return false;}
#endif
//-----------------------------------------------------------------------------
int mgl_data_read_hdf_(uintptr_t *d, const char *fname, const char *data,int l,int n)
{	char *s=new char[l+1];		memcpy(s,fname,l);	s[l]=0;
	char *t=new char[n+1];		memcpy(t,data,n);	t[n]=0;
	int r = mgl_data_read_hdf(_DT_,s,t);	delete []s;	delete []t;	return r;	}
void mgl_data_save_hdf_(uintptr_t *d, const char *fname, const char *data, int *rewrite,int l,int n)
{	char *s=new char[l+1];		memcpy(s,fname,l);	s[l]=0;
	char *t=new char[n+1];		memcpy(t,data,n);	t[n]=0;
	mgl_data_save_hdf(_DT_,s,t,*rewrite);	delete []s;	delete []t;	}
//-----------------------------------------------------------------------------
bool mgl_add_file(long &kx,long &ky, long &kz, mreal *&b, mglData *d,bool as_slice)
{
	if(as_slice && d->nz==1)
	{
		if(kx==d->nx && d->ny==1)
		{
			b = (mreal *)realloc(b,kx*(ky+1)*sizeof(mreal));
			memcpy(b+kx*ky,d->a,kx*sizeof(mreal));		ky++;
		}
		else if(kx==d->nx && ky==d->ny)
		{
			b = (mreal *)realloc(b,kx*ky*(kz+1)*sizeof(mreal));
			memcpy(b+kx*ky*kz,d->a,kx*ky*sizeof(mreal));	kz++;
		}
		else	return false;
	}
	else
	{
		if(d->ny*d->nz==1 && ky*kz==1)
		{
			b = (mreal *)realloc(b,(kx+d->nx)*sizeof(mreal));
			memcpy(b+kx,d->a,d->nx*sizeof(mreal));	kx+=d->nx;
		}
		else if(kx==d->nx && kz==1 && d->nz==1)
		{
			b = (mreal *)realloc(b,kx*(ky+d->ny)*sizeof(mreal));
			memcpy(b+kx*ky,d->a,kx*d->ny*sizeof(mreal));	ky+=d->ny;
		}
		else if(kx==d->nx && ky==d->ny)
		{
			b = (mreal *)realloc(b,kx*kx*(kz+d->nz)*sizeof(mreal));
			memcpy(b+kx*ky*kz,d->a,kx*ky*d->nz*sizeof(mreal));	kz+=d->nz;
		}
		else	return false;
	}
	return true;
}
//-----------------------------------------------------------------------------
int mgl_data_read_range(HMDT dat, const char *templ, double from, double to, double step, bool as_slice)
{
	mglData d;
	mreal t = from, *b;
	long kx,ky,kz;
	char *fname = new char[strlen(templ)+20];

	//read first file
	do{	sprintf(fname,templ,t);	t+= step;	} while(!mgl_data_read(&d,fname) && t<=to);

	if(t>to)	return false;
	kx = d.nx;	ky = d.ny;	kz = d.nz;
	b = (mreal *)malloc(kx*ky*kz*sizeof(mreal));
	memcpy(b,d.a,kx*ky*kz*sizeof(mreal));

	// read other files
	for(;t<=to;t+=step)
	{
		sprintf(fname,templ,t);
		if(mgl_data_read(&d,fname))
			if(!mgl_add_file(kx,ky,kz,b,&d,as_slice))
				return false;
	}
	dat->Set(b,kx,ky,kz);
	delete []fname;		free(b);
	return true;
}
int mgl_data_read_range_(uintptr_t *d, const char *fname, double *from, double *to, double *step, bool *as_slice,int l)
{	char *s=new char[l+1];		memcpy(s,fname,l);	s[l]=0;
	int r = mgl_data_read_range(_DT_,s,*from,*to,*step,*as_slice);	delete []s;	return r;	}
//-----------------------------------------------------------------------------
int mgl_data_read_all(HMDT dat, const char *templ, bool as_slice)
{
#ifndef WIN32
	mglData d;
	glob_t res;
	unsigned long i;
	mreal *b;
	long kx,ky,kz;
	char *fname = new char[256];
	glob (templ, GLOB_TILDE, NULL, &res);

	//read first file
	for(i=0;i<res.gl_pathc;i++)
		if(mgl_data_read(&d,res.gl_pathv[i]))	break;

	if(i>=res.gl_pathc)	{	delete []fname;	return false;	}
	kx = d.nx;	ky = d.ny;	kz = d.nz;
	b = (mreal *)malloc(kx*ky*kz*sizeof(mreal));
	memcpy(b,d.a,kx*ky*kz*sizeof(mreal));

	for(;i<res.gl_pathc;i++)
	{
		if(mgl_data_read(&d,res.gl_pathv[i]))
			if(!mgl_add_file(kx,ky,kz,b,&d,as_slice))
			{	delete []fname;		return false;	}
	}
	dat->Set(b,kx,ky,kz);

	globfree (&res);
	delete []fname;		free(b);
	return true;
#else
	return false;
#endif
}
int mgl_data_read_all_(uintptr_t *d, const char *fname, bool *as_slice,int l)
{	char *s=new char[l+1];		memcpy(s,fname,l);	s[l]=0;
	int r = mgl_data_read_all(_DT_,s,*as_slice);	delete []s;	return r;	}
//-----------------------------------------------------------------------------
