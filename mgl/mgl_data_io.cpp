/* mgl_data_io.cpp is part of Math Graphic Library
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
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifndef WIN32
#include <glob.h>
#include <ltdl.h>
#endif

#include "mgl/mgl_eval.h"
#include "mgl/mgl_data.h"

#ifndef NO_GSL
#include <gsl/gsl_vector.h>
#endif

//-----------------------------------------------------------------------------
void mglData::Set(gsl_vector *v)
{
#ifndef NO_GSL
	if(!v || v->size<1)	return;
	Create(v->size);
	for(long i=0;i<nx;i++)	a[i] = v->data[i*v->stride];
#endif
}
//-----------------------------------------------------------------------------
void mglData::Set(gsl_matrix *m)
{
#ifndef NO_GSL
	if(!m || m->size1<1 || m->size2<1)	return;
	Create(m->size1,m->size2);
	register long i,j;
	for(i=0;i<nx;i++)	for(j=0;j<ny;j++)
		a[i+j*nx] = m->data[i * m->tda + j];
#endif
}
//-----------------------------------------------------------------------------
void mglData::Set(const float *A,int NX,int NY,int NZ)
{
	if(NX<=0 || NY<=0 || NZ<=0)	return;
	Create(NX,NY,NZ);
	memcpy(a,A,NX*NY*NZ*sizeof(float));
}
//-----------------------------------------------------------------------------
void mglData::Set(const double *A,int NX,int NY,int NZ)
{
	if(NX<=0 || NY<=0 || NZ<=0)	return;
	Create(NX,NY,NZ);
	for(long i=0;i<NX*NY*NZ;i++)	a[i] = A[i];
}
//-----------------------------------------------------------------------------
void mglData::Set(const float **A,int N1,int N2)
{
	if(N1<=0 || N2<=0)	return;
	Create(N2,N1);
	for(long i=0;i<N1;i++)
		memcpy(a+i*N2,A[i],N2*sizeof(float));
}
//-----------------------------------------------------------------------------
void mglData::Set(const double **A,int N1,int N2)
{
	if(N1<=0 || N2<=0)	return;
	Create(N2,N1);
	for(long i=0;i<N1;i++)	for(long j=0;j<N2;j++)	a[j+i*N2] = A[i][j];
}
//-----------------------------------------------------------------------------
void mglData::Set(const float ***A,int N1,int N2,int N3)
{
	if(N1<=0 || N2<=0 || N3<=0)	return;
	Create(N3,N2,N1);
	for(long i=0;i<N1;i++)	for(long j=0;j<N2;j++)
		memcpy(a+N3*(j+i*N2),A[i][j],N3*sizeof(float));
}
//-----------------------------------------------------------------------------
void mglData::Set(const double ***A,int N1,int N2,int N3)
{
	if(N1<=0 || N2<=0 || N3<=0)	return;
	Create(N3,N2,N1);
	for(long i=0;i<N1;i++)	for(long j=0;j<N2;j++)	for(long k=0;k<N3;k++)
		a[k+N3*(j+i*N2)] = A[i][j][k];
}
//-----------------------------------------------------------------------------
mglData &mglData::SubData(int xx,int yy,int zz)
{
	register long i,j;
	static mglData d;
	xx = xx<0 ? -1:xx;	yy = yy<0 ? -1:yy;	zz = zz<0 ? -1:zz;
	if(xx==-1 && yy==-1 && zz==-1)	// сам массив
		d.Set(*this);
	else if(xx==-1 && yy==-1 && zz<nz)	// 2d
		d.Set(a+zz*nx*ny,nx,ny,1);
	else if(xx<nx && yy==-1 && zz==-1)	// 2d
	{
		d.Create(ny,nz);
		for(i=0;i<ny*nz;i++)
			d.a[i] = a[xx+i*nx];
	}
	else if(xx==-1 && yy<ny && zz==-1)	// 2d
	{
		d.Create(nx,nz);
		for(i=0;i<nx;i++)	for(j=0;j<nz;j++)
			d.a[i+nx*j] = a[i+nx*(yy+j*ny)];
	}
	else if(xx==-1 && yy<ny && zz<nz)
		d.Set(a+nx*(yy+ny*zz),nx,1,1);
	else if(xx<nx && yy==-1 && zz<nz)
	{
		d.Create(ny);
		for(i=0;i<ny;i++)
			d.a[i] = a[xx+nx*(i+zz*ny)];
	}
	else if(xx<nx && yy<ny && zz==-1)
	{
		d.Create(nz);
		for(i=0;i<nz;i++)
			d.a[i] = a[xx+nx*(yy+i*ny)];
	}
	return d;
}
//-----------------------------------------------------------------------------
void mglData::Save(const char *fname,int ns)
{
	FILE *fp;
	fp = fopen(fname,"w");
	if(ns<0 || (ns>=nz && nz>1))	for(long k=0;k<nz;k++)
	{	// сохраняем все данные
		for(long i=0;i<ny;i++)
		{
			for(long j=0;j<nx;j++)
				fprintf(fp,"%g\t",a[j+nx*(i+ny*k)]);
			fprintf(fp,"\n");
		}
		fprintf(fp,"\n");
	}
	else
	{	// сохраняем только срез
		if(nz>1)		// для 3D -- плоскость
		{
			for(long i=0;i<ny;i++)
			{
				for(long j=0;j<nx;j++)
					fprintf(fp,"%g\t",a[j+nx*(i+ny*ns)]);
				fprintf(fp,"\n");
			}
		}
		else if(ns<ny)	// для 2D -- линия
		{
			for(long j=0;j<nx;j++)
				fprintf(fp,"%g\t",a[j+nx*ns]);
		}
	}
	fclose(fp);
}
//-----------------------------------------------------------------------------
bool mglData::Read(const char *fname)
{
	long l=1,m=1,k=1;
	long nb,i;
	FILE *fp = fopen(fname,"rb");
	if(!fp)
	{
		if(!a)	Create(1,1,1);
		return	false;
	}
	fseek(fp,0,SEEK_END);
	nb = ftell(fp);
	char *buf = new char[nb+1];
	fseek(fp,0,SEEK_SET);
	memset(buf,0,nb*sizeof(char));
	fread(buf,nb,1,fp);
	fclose(fp);

	bool first=false,sp,com=false;
	register char ch;
	for(i=nb-1;i>=0;i--)
	{
		if(buf[i]>' ')	break;
	}
	buf[i+1]=0;	nb = i;		// remove tailing spaces
	for(i=0;i<nb-1 && buf[i]!='\n';i++)	// determine nx
	{
		ch = buf[i];
		sp = (ch==' ' || ch=='\t');
		if(ch=='#')	com = true;
		if(com && ch!='\n')	continue;	else	com = false;
		if(!sp && !first)	first=true;
		if(first && sp && !isspace(buf[i+1])) k++;
	}
	first = false;
	for(i=0;i<nb-1;i++)					// determine ny
	{
		ch = buf[i];
//		if(ch=='#')	com = true;	// comment
//		if(com && ch!='\n')	continue;	else	com = false;
		if(ch=='\n')
		{
			if(buf[i+1]=='\n')	{first=true;	break;	}
			m++;
		}
		if(ch=='\f')	break;
	}
	if(first)	for(i=0;i<nb-1;i++)		// determine nz
	{
		ch = buf[i];
		if(ch=='#')	com = true;	// comment
		if(ch=='\n')
		{
			if(com)	{	com=false;	continue;	}
			if(buf[i+1]=='\n')	l++;
		}
	}
	else	for(i=0;i<nb-1;i++)	if(buf[i]=='\f')	l++;

	delete []buf;
	return Read(fname,k,m,l);
}
//-----------------------------------------------------------------------------
void mglData::Create(int mx,int my,int mz)
{
//	if(mx<=0 || my<=0 || mz<=0)	return;

	nx = mx>0 ? mx:1;	ny = my>0 ? my:1;	nz = mz>0 ? mz:1;
	if(a)	delete []a;
	a = new float[nx*ny*nz];
	memset(a,0,nx*ny*nz*sizeof(float));
}
//-----------------------------------------------------------------------------
bool mglData::Read(const char *fname,int mx,int my,int mz)
{
	if(mx<=0 || my<=0 || mz<=0)	return false;
	FILE *fp = fopen(fname,"rt");
	if(!fp)	return false;
	Create(mx,my,mz);
	for(long i=0;i<mx*my*mz;i++)
	{
		if(feof(fp))	break;
		fscanf(fp,"%g",a+i);
	}
	fclose(fp);
	return true;
}
//-----------------------------------------------------------------------------
bool mglData::ReadMat(const char *fname,int dim)
{
	if(dim<=0 || dim>3)	return false;
	FILE *fp = fopen(fname,"rt");
	if(!fp)	return false;
	nx = ny = nz = 1;
	if(dim==1)		fscanf(fp,"%d",&nx);
	else if(dim==2)	fscanf(fp,"%d%d",&nx,&ny);
	else if(dim==3)	fscanf(fp,"%d%d%d",&nx,&ny,&nz);
	Create(nx,ny,nz);
	for(long i=0;i<nx*ny*nz;i++)
	{
		if(feof(fp))	break;
		fscanf(fp,"%g",a+i);
	}
	fclose(fp);
	return true;
}
//-----------------------------------------------------------------------------
void mglData::PrintInfo(FILE *fp)
{
	if(fp==0)	return;
	char *buf = new char[512];
	PrintInfo(buf);
	fprintf(fp,buf);	fflush(fp);
	delete []buf;
}
//-----------------------------------------------------------------------------
void mglData::PrintInfo(char *buf)
{
	if(buf==0)	return;
	char s[128];
	buf[0]=0;
	sprintf(s,"nx = %ld\tny = %ld\tnz = %ld\n",nx,ny,nz);	strcat(buf,s);

	int i=0,j=0,k=0;
	float b = Maximal(i,j,k);
	sprintf(s,"Maximum is %g\t at coordinates x = %d\ty = %d\tz = %d\n", b,i,j,k);	strcat(buf,s);
	b = Minimal(i,j,k);
	sprintf(s,"Minimum is %g\t at coordinates x = %d\ty = %d\tz = %d\n", b,i,j,k);	strcat(buf,s);
	float A=0,Wa=0,X=0,Y=0,Z=0,Wx=0,Wy=0,Wz=0;
	Momentum('x',X,Wx);		Momentum('y',Y,Wy);
	Momentum('z',Z,Wz);		Momentum(0,A,Wa);
	sprintf(s,"Averages are:\n<a> = %g\t<x> = %g\t<y> = %g\t<z> = %g\n", A,X,Y,Z);	strcat(buf,s);
	sprintf(s,"Widths (dispersions) are:\nWa = %g\tWx = %g\tWy = %g\tWz = %g\n\n",
		Wa,Wx,Wy,Wz);	strcat(buf,s);
}
//-----------------------------------------------------------------------------
float mglData::v(int i,int j,int k)
{
	bool not_ok = i<0 || i>=nx || j<0 || j>=ny || k<0 || k>=nz;
	if(not_ok)	return 0;
	return a[i+nx*(j+ny*k)];
}
//-----------------------------------------------------------------------------
void mglData::Swap(const char *dir)
{
	register long i,j,k,i0,nn,j0;
	float b;
	if(strchr(dir,'z') && nz>1)
	{
		for(i=0;i<nx*ny;i++)
		{
			nn = (nz/2)*nx*ny;
			for(j=0;j<nz/2;j++)
			{	i0 = i+j*nx*ny;	b = a[i0];	a[i0] = a[i0+nn];	a[i0+nn] = b;	}
		}
	}
	if(strchr(dir,'y') && ny>1)
	{
		nn = (ny/2)*nx;
		for(i=0;i<nx;i++)  for(k=0;k<nz;k++)
		{
			j0 = i+nx*ny*k;
			for(j=0;j<ny/2;j++)
			{	i0 = j0+j*nx;	b = a[i0];	a[i0] = a[i0+nn];	a[i0+nn] = b;	}
		}
	}
	if(strchr(dir,'x') && nx>1)
	{
		nn = nx/2;
		for(j=0;j<ny*nz;j++)
		{
			j0 = j*nx;
			for(i=0;i<nx/2;i++)
			{	i0 = i+j0;	b = a[i0];	a[i0] = a[i0+nn];	a[i0+nn] = b;	}
		}
	}
}
//-----------------------------------------------------------------------------
mglData &mglData::Resize(int mx, int my, int mz, float x1, float x2,
	float y1, float y2, float z1, float z2)
{
	register long i,j,k;
	static mglData d;
	mx = mx<2 ? 2:mx;	my = my<2 ? 2:my;	mz = mz<2 ? 2:mz;
	d.Create(mx,my,mz);
	for(i=0;i<mx;i++)	for(j=0;j<my;j++)	for(k=0;k<mz;k++)
		d.a[i+mx*(j+my*k)] = Spline1(x1+i*(x2-x1)/(mx-1),	y1+j*(y2-y1)/(my-1),
									z1+k*(z2-z1)/(mz-1));
	return d;
}
//-----------------------------------------------------------------------------
void mglData::operator*=(mglData &d)
{
	register long i,j;
	if(d.nz==1 && d.ny==1 && nx==d.nx)
	{
		for(j=0;j<ny*nz;j++)	for(i=0;i<nx;i++)	a[i+nx*j] *= d.a[i];
	}
	else if(d.nz==1 && ny==d.ny && nx==d.nx)
	{
		for(j=0;j<nz;j++)	for(i=0;i<ny*nx;i++)	a[i+nx*ny*j] *= d.a[i];
	}
	else if(nz==d.nz && d.ny==ny && nx==d.nx)
	{
		for(i=0;i<ny*nz*nx;i++)		a[i] *= d.a[i];
	}
}
//-----------------------------------------------------------------------------
void mglData::operator/=(mglData &d)
{
	register long i,j;
	if(d.nz==1 && d.ny==1 && nx==d.nx)
	{
		for(j=0;j<ny*nz;j++)	for(i=0;i<nx;i++)
			a[i+nx*j] = d.a[i] ? a[i+nx*j]/d.a[i] : 0;
	}
	else if(d.nz==1 && ny==d.ny && nx==d.nx)
	{
		for(j=0;j<nz;j++)	for(i=0;i<ny*nx;i++)
			a[i+nx*ny*j] = d.a[i] ? a[i+nx*ny*j]/d.a[i] : 0;
	}
	else if(nz==d.nz && d.ny==ny && nx==d.nx)
	{
		for(i=0;i<ny*nz*nx;i++)
			a[i] = d.a[i] ? a[i]/d.a[i] : 0;
	}
}
//-----------------------------------------------------------------------------
void mglData::operator+=(mglData &d)
{
	register long i,j;
	if(d.nz==1 && d.ny==1 && nx==d.nx)
	{
		for(j=0;j<ny*nz;j++)	for(i=0;i<nx;i++)
			a[i+nx*j] += d.a[i];
	}
	else if(d.nz==1 && ny==d.ny && nx==d.nx)
	{
		for(j=0;j<nz;j++)	for(i=0;i<ny*nx;i++)
			a[i+nx*ny*j] += d.a[i];
	}
	else if(nz==d.nz && d.ny==ny && nx==d.nx)
	{
		for(i=0;i<ny*nz*nx;i++)
			a[i] += d.a[i];
	}
}
//-----------------------------------------------------------------------------
void mglData::operator-=(mglData &d)
{
	register long i,j;
	if(d.nz==1 && d.ny==1 && nx==d.nx)
	{
		for(j=0;j<ny*nz;j++)	for(i=0;i<nx;i++)
			a[i+nx*j] -= d.a[i];
	}
	else if(d.nz==1 && ny==d.ny && nx==d.nx)
	{
		for(j=0;j<nz;j++)	for(i=0;i<ny*nx;i++)
			a[i+nx*ny*j] -= d.a[i];
	}
	else if(nz==d.nz && d.ny==ny && nx==d.nx)
	{
		for(i=0;i<ny*nz*nx;i++)
			a[i] -= d.a[i];
	}
}
//-----------------------------------------------------------------------------
void mglData::operator-=(float d)
{
	register long i;
	for(i=0;i<ny*nz*nx;i++)	a[i] -= d;
}
//-----------------------------------------------------------------------------
void mglData::operator+=(float d)
{
	register long i;
	for(i=0;i<ny*nz*nx;i++)	a[i] += d;
}
//-----------------------------------------------------------------------------
void mglData::operator*=(float d)
{
	register long i;
	for(i=0;i<ny*nz*nx;i++)	a[i] *= d;
}
//-----------------------------------------------------------------------------
void mglData::operator/=(float d)
{
	register long i;
	for(i=0;i<ny*nz*nx;i++)	a[i] = d ? a[i]/d : 0;
}
//-----------------------------------------------------------------------------
float mglData::Maximal()
{
	register float m=-1e10;
	for(long i=0;i<nx*ny*nz;i++)
		m = m>a[i] ? m : a[i];
	return m;
}
//-----------------------------------------------------------------------------
float mglData::Minimal()
{
	register float m=1e10;
	for(long i=0;i<nx*ny*nz;i++)
		m = m<a[i] ? m : a[i];
	return m;
}
//-----------------------------------------------------------------------------
float mglData::Maximal(int &im,int &jm,int &km)
{
	register float m=-1e10;
	for(long i=0;i<nx*ny*nz;i++)
		if(!isnan(a[i]) && m<a[i])
		{	m=a[i];	im=i%nx;	jm=(i/nx)%ny;	km=i/(nx*ny);   }
	return m;
}
//-----------------------------------------------------------------------------
float mglData::Minimal(int &im,int &jm,int &km)
{
	register float m=1e10;
	for(long i=0;i<nx*ny*nz;i++)
		if(!isnan(a[i]) && m>a[i])
		{	m=a[i];	im=i%nx;	jm=(i/nx)%ny;	km=i/(nx*ny);   }
	return m;
}
//-----------------------------------------------------------------------------
float mglData::Maximal(float &x,float &y,float &z)
{
	int im=-1,jm=-1,km=-1;
	register long tm,i;
	float m=Maximal(im,jm,km);
	x=im;	y=jm;	z=km;

	if(nx>2)
	{
		if(im==0)	im=1;
		if(im==nx-1)im=nx-2;
		x = (a[im+1]+a[im-1]-2*a[im])==0 ? im : im+(a[im+1]-a[im-1])/(a[im+1]+a[im-1]-2*a[im])/2;
	}
	if(ny>2)
	{
		if(jm==0)	jm=1;
		if(jm==ny-1)jm=ny-2;
		i=nx;		tm = jm*nx;
		y = (a[tm+i]+a[tm-i]-2*a[tm])==0? jm : jm+(a[tm+i]-a[tm-i])/(a[tm+i]+a[tm-i]-2*a[tm])/2;
	}
	if(nz>2)
	{
		if(km==0)	km=1;
		if(km==nz-1)km=nz-2;
		i=nx*ny;	tm = km*i;
		z = (a[tm+i]+a[tm-i]-2*a[tm])==0? km : km+(a[tm+i]-a[tm-i])/(a[tm+i]+a[tm-i]-2*a[tm])/2;
	}
	return m;
}
//-----------------------------------------------------------------------------
float mglData::Minimal(float &x,float &y,float &z)
{
	int im=-1,jm=-1,km=-1;
	register long tm,i;
	float m=Minimal(im,jm,km);
	x=im;	y=jm;	z=km;
	if(nx>2)
	{
		if(im==0)	im=1;
		if(im==nx-1)im=nx-2;
		x = im+(a[im+1]-a[im-1])/(a[im+1]+a[im-1]-2*a[im])/2;
	}
	if(ny>2)
	{
		if(jm==0)	jm=1;
		if(jm==ny-1)jm=ny-2;
		i=nx;		tm = jm*nx;
		y = jm+(a[tm+i]-a[tm-i])/(a[tm+i]+a[tm-i]-2*a[tm])/2;
	}
	if(nz>2)
	{
		if(km==0)	km=1;
		if(km==nz-1)km=nz-2;
		i=nx*ny;	tm = km*i;
		z = km+(a[tm+i]-a[tm-i])/(a[tm+i]+a[tm-i]-2*a[tm])/2;
	}
	return m;
}
//-----------------------------------------------------------------------------
float mglData::Momentum(char dir,float &x,float &w)
{
	float i0=0,i1=0,i2=0,d;
	register long i;
	switch(dir)
	{
		case 'x':
			for(i=0;i<nx*ny*nz;i++)
			{
				d = i%nx;		i0+= a[i];
				i1+= a[i]*d;	i2+= a[i]*d*d;
			}
		case 'y':
			for(i=0;i<nx*ny*nz;i++)
			{
				d = (i/nx)%ny;	i0+= a[i];
				i1+= a[i]*d;	i2+= a[i]*d*d;
			}
			break;
		case 'z':
			for(i=0;i<nx*ny*nz;i++)
			{
				d = i/(nx*ny);	i0+= a[i];
				i1+= a[i]*d;	i2+= a[i]*d*d;
			}
			break;
		default:	// "self-dispersion"
			i0 = nx*ny*nz;
			for(i=0;i<nx*ny*nz;i++)
			{
				i1+= a[i];	i2+= a[i]*a[i];
			}
	}
	if(i0==0)	return 0;
	x = i1/i0;	w = i2>x*x*i0 ? sqrt(i2/i0 - x*x) : 0;
	return i0;
}
//-----------------------------------------------------------------------------
float mglData::Momentum(char dir,float &x,float &w,float &s,float &k)
{
	float i0=0,i1=0,i2=0,d,t;
	register long i;
	i0 = Momentum(dir,x,w);
	if(i0==0 || w==0)	return 0;
	switch(dir)
	{
		case 'x':
			for(i=0;i<nx*ny*nz;i++)
			{
				d = (i%nx - x)/w;		t = d*d;
				i1+= a[i]*t*d;	i2+= a[i]*t*t;
			}
		case 'y':
			for(i=0;i<nx*ny*nz;i++)
			{
				d = ((i/nx)%ny - x)/w;	t = d*d;
				i1+= a[i]*t*d;	i2+= a[i]*t*t;
			}
			break;
		case 'z':
			for(i=0;i<nx*ny*nz;i++)
			{
				d = (i/(nx*ny) - x)/w;	t = d*d;
				i1+= a[i]*t*d;	i2+= a[i]*t*t;
			}
			break;
		default:	// "self-dispersion"
			for(i=0;i<nx*ny*nz;i++)
			{
				d = (a[i] - x)/w;		t = d*d;
				i1+= t*d;	i2+= t*t;
			}
	}
	s = i1/i0;	k = i2/(i0*3);
	return i0;
}
//-----------------------------------------------------------------------------
void mglData::Modify(const char *eq,int dim)
{
	long i,j,k,i0;
	float x,y,z,dx=nx>1?1/(nx-1.):0,dy=ny>1?1/(ny-1.):0;
	mglFormula eqs(eq);
	if(dim<0)	dim=0;
	if(nz>1)	// 3D array
	{
		for(k=dim;k<nz;k++)
		{
			z = (nz>dim+1) ? (k-dim)/(nz-dim-1.) : 0;
			for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
			{
				x = dx*i;	y = dy*j;
				i0 = i+nx*(j+ny*k);
				a[i0] = eqs.Calc(x,y,z,a[i0]);
			}
		}
	}
	else		// 2D or 1D array
	{
		z = 0;
		if(ny==1)	dim = 0;
		dy = ny>dim+1 ? 1/(ny-dim-1.) : 0;
		for(j=dim;j<ny;j++)	for(i=0;i<nx;i++)
		{
			x = dx*i;	y = dy*(j-dim);
			i0 = i+nx*j;
			a[i0] = eqs.Calc(x,y,0,a[i0]);
		}
	}
}
//-----------------------------------------------------------------------------
void mglData::Fill(float x1,float x2,char dir)
{
	long i,j,k;
	register float x;
	if(dir<'x' || dir>'z')	dir='x';
	for(k=0;k<nz;k++)	for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
	{
		x = nx>1 ? i/(nx-1.):0;
		if(dir=='y')	x = ny>1 ? j/(ny-1.):0;
		if(dir=='z')	x = nz>1 ? k/(nz-1.):0;
		a[i+nx*(j+ny*k)] = x1+(x2-x1)*x;
	}
}
//-----------------------------------------------------------------------------
void mglData::Norm(float v1,float v2,bool sym,int dim)
{
	long i,s,nn=nx*ny*nz;
	float a1=1e20,a2=-1e20,v;
	if(nz>1)	s = dim*nx*ny;
	else		s = dim*ny;
	for(i=s;i<nn;i++)	// determines borders of existing data
	{	a1 = (a1<a[i] ? a1 : a[i]);	a2 = (a2>a[i] ? a2 : a[i]);	}
	if(a1==a2)  {  if(a1!=0)	a1=0.;  else a2=1;  }
	if(v1>v2)	{	v=v1;	v1=v2;	v2=v;	}	// swap if uncorrect
	if(sym)				// use symmetric
	{
		v2 = -v1>v2 ? -v1:v2;	v1 = -v2;
		a2 = -a1>a2 ? -a1:a2;	a1 = -a2;
	}
	for(i=s;i<nn;i++)	// normalize
	{
		a[i] = v1 + (v2-v1)*(a[i]-a1)/(a2-a1);
	}

}
//-----------------------------------------------------------------------------
void mglData::Squeeze(int rx,int ry,int rz,bool smooth)
{
	long kx=nx,ky=ny,kz=nz,i,j,k;
	float *b;

	// simple checking
	rx = rx<1 ? 1:rx;  ry = ry<1 ? 1:ry;  rz = rz<1 ? 1:rz;
	// new sizes
	kx /= rx;  kx = kx<1 ? 1:kx;
	ky /= ry;  ky = ky<1 ? 1:ky;
	kz /= rz;  kz = kz<1 ? 1:kz;
	b = new float[kx*ky*kz];

	if(!smooth)
		for(i=0;i<kx;i++)  for(j=0;j<ky;j++)  for(k=0;k<ky;k++)
			b[i+kx*(j+ky*k)] = a[i*rx+nx*(j*ry+ny*rz*k)];
	else
		for(i=0;i<kx;i++)  for(j=0;j<ky;j++)  for(k=0;k<ky;k++)
		{
			long dx,dy,dz,i1,j1,k1;
			dx = (i+1)*rx<=nx ? rx : nx-i*rx;
			dy = (j+1)*ry<=ny ? ry : ny-j*ry;
			dz = (k+1)*rz<=nz ? rz : nz-k*rz;
			for(i1=i*rx;i1<i*rx+dx;i1++)	for(j1=j*ry;j1<j*ry+dz;j1++)	for(k1=k*rz;k1<k*rz+dz;k1++)
				b[i+kx*(j+ky*k)] += a[i*rx+nx*(j*ry+ny*rz*k)]/(dx*dy*dz);
		}
	delete []a;
	a = b;
	nx = kx;  ny = ky;  nz = kz;
}
//-----------------------------------------------------------------------------
mglData &mglData::Combine(mglData &b)
{
	static mglData d;
	d.Create(1,1,1);
	if(nz>1 || (ny>1 && b.ny>1) || b.nz>1)	return d;
	long n1=ny,n2=b.nx;
	bool dim2=true;
	if(ny==1)	{	n1 = b.nx;	n2 = b.ny;	dim2 = false;	}
	d.Create(nx,n1,n2);
	register long i,j;
	if(dim2)	n1=nx*ny;	else	{	n1=nx;	n2=b.nx*b.ny;	}
	for(i=0;i<n1;i++)	for(j=0;j<n2;j++)	d.a[i+n1*j] = a[i]*b.a[j];
	return d;
}
//-----------------------------------------------------------------------------
void mglData::Extend(int n1, int n2)
{
	if(nz>2 || (n1==0 && n2==0))	return;
	long mx,my,mz,m1,m2;
	float *b=0;
	if(n1>=0 && n2>=0)	// extend to up dimensions
	{
		mx = nx;	my = ny;	mz = n1+1;	m1 = nx*ny;	m2=mz;
		if(ny>1)	{	my=n1+1;	mz=n2+1;	m1=nx;	m2=my*mz;	}
		b = new float[mx*my*mz];
		for(long i=0;i<m2;i++)
			memcpy(b+i*m1,a,m1*sizeof(float));
	}
	else if(n1<0)
	{
		if(ny==1 && n2<0)
		{	mx = 1-n1;	my = 1-n2;	mz = nx;	m1 = mx*my;	m2 = nx;	}
		else
		{	mx = 1-n1;	my = nx;	mz = ny;	m1 = mx;	m2 = nx*ny;	}
		b = new float[mx*my*mz];
		for(long i=0;i<m1;i++)	for(long j=0;j<m2;j++)	b[i+m1*j] = a[j];
	}
	if(b)	{	delete []a;	a = b;	}
}
//-----------------------------------------------------------------------------
void mglData::Transpose(const char *dim)
{
	float *b=new float[nx*ny*nz];
	register long i,j,k,n;
	if(!strcmp(dim,"xyz"))	memcpy(b,a,nx*ny*nz*sizeof(float));
	else if(!strcmp(dim,"xzy") || !strcmp(dim,"zy"))
	{
		for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
			b[i+nx*(k+nz*j)] = a[i+nx*(j+ny*k)];
		n=nz;	nz=ny;	ny=n;
	}
	else if(!strcmp(dim,"yxz") || !strcmp(dim,"yx"))
	{
		for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
			b[j+ny*(i+nx*k)] = a[i+nx*(j+ny*k)];
		n=nx;	nx=ny;	ny=n;
	}
	else if(!strcmp(dim,"yzx"))
	{
		for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
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
	delete []a;		a = b;
}
//-----------------------------------------------------------------------------
void mglData::Modify(const char *eq,mglData &v, mglData &w)
{
	if(v.nx*v.ny*v.nz!=nx*ny*nz || w.nx*w.ny*w.nz!=nx*ny*nz)
		return;
	long i,j,k,i0;
	float x,y,z,dx=nx>1?1/(nx-1.):0,dy=ny>1?1/(ny-1.):0,dz=nz>1?1/(nz-1.):0;
	mglFormula eqs(eq);
	for(k=0;k<nz;k++)	for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
	{
		x = dx*i;	y = dy*j;	z = dz*k;
		i0 = i+nx*(j+ny*k);
		a[i0] = eqs.Calc(x,y,z,a[i0],v.a[i0],w.a[i0]);
	}
}
//-----------------------------------------------------------------------------
void mglData::Modify(const char *eq,mglData &v)
{
	if(v.nx*v.ny*v.nz!=nx*ny*nz)	return;
	long i,j,k,i0;
	float x,y,z,dx=nx>1?1/(nx-1.):0,dy=ny>1?1/(ny-1.):0,dz=nz>1?1/(nz-1.):0;
	mglFormula eqs(eq);
	for(k=0;k<nz;k++)	for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
	{
		x = dx*i;	y = dy*j;	z = dz*k;
		i0 = i+nx*(j+ny*k);
		a[i0] = eqs.Calc(x,y,z,a[i0],v.a[i0],0);
	}
}
//-----------------------------------------------------------------------------
void mglData::SaveHDF(const char *fname,const char *data,bool rewrite)
{
#ifndef WIN32
	int (*mgl_save) (const char *fname,const char *data,bool rewrite,
	float *a, int nx, int ny, int nz);
	lt_dlhandle module = NULL;
	mgl_save=NULL;
	int errors=lt_dlinit();
	if(!errors)		errors=lt_dlsetsearchpath(MOD_LIB_DIR);
	if(!errors)		module=lt_dlopenext("mgl-hdf5.so");
	if(module)		*(void **) (&mgl_save)  = lt_dlsym(module, "mgl_save_hdf");
	if(mgl_save)	(*mgl_save)(fname,data,rewrite,a,nx,ny,nz);
	if(module)		lt_dlclose(module);
	if(!errors)		lt_dlexit();
#endif
}
//-----------------------------------------------------------------------------
void mglData::ReadHDF(const char *fname,const char *data)
{
#ifndef WIN32
	float *(*mgl_read) (const char *fname,const char *data, long *nd);
	lt_dlhandle module = NULL;
	mgl_read=NULL;
	int errors=lt_dlinit();
	if(!errors)	errors=lt_dlsetsearchpath(MOD_LIB_DIR);
	if(!errors)	module=lt_dlopenext("mgl-hdf5.so");
	if(module)	*(void **) (&mgl_read)  = lt_dlsym(module, "mgl_read_hdf");
	if(mgl_read)
	{
		long dim[3];
		float *b = (*mgl_read)(fname,data,dim);
		if(b)
		{
			Create(dim[0],dim[1],dim[2]);
			memcpy(a,b,nx*ny*nz*sizeof(float));
			free(b);
		}
//		else	SetWarn(mglWarnOpen,fname);
	}
	if(module)	lt_dlclose(module);
	if(!errors)	lt_dlexit();
#endif
}
//-----------------------------------------------------------------------------
bool mgl_add_file(long &kx,long &ky, long &kz, float *&b, mglData &d,bool as_slice)
{
	if(as_slice && d.nz==1)
	{
		if(kx==d.nx && d.ny==1)
		{
			b = (float *)realloc(b,kx*(ky+1)*sizeof(float));
			memcpy(b+kx*ky,d.a,kx*sizeof(float));		ky++;
		}
		else if(kx==d.nx && ky==d.ny)
		{
			b = (float *)realloc(b,kx*ky*(kz+1)*sizeof(float));
			memcpy(b+kx*ky*kz,d.a,kx*ky*sizeof(float));	kz++;
		}
		else	return false;
	}
	else
	{
		if(d.ny*d.nz==1 && ky*kz==1)
		{
			b = (float *)realloc(b,(kx+d.nx)*sizeof(float));
			memcpy(b+kx,d.a,d.nx*sizeof(float));	kx+=d.nx;
		}
		else if(kx==d.nx && kz==1 && d.nz==1)
		{
			b = (float *)realloc(b,kx*(ky+d.ny)*sizeof(float));
			memcpy(b+kx*ky,d.a,kx*d.ny*sizeof(float));	ky+=d.ny;
		}
		else if(kx==d.nx && ky==d.ny)
		{
			b = (float *)realloc(b,kx*kx*(kz+d.nz)*sizeof(float));
			memcpy(b+kx*ky*kz,d.a,kx*ky*d.nz*sizeof(float));	kz+=d.nz;
		}
		else	return false;
	}
	return true;
}
//-----------------------------------------------------------------------------
bool mglData::ReadRange(const char *templ, float from, float to, float step, bool as_slice)
{
	mglData d;
	float t = from, *b;
	long kx,ky,kz;
	char *fname = new char[strlen(templ)+20];

	//read first file
	do{	sprintf(fname,templ,t);	t+= step;	} while(!d.Read(fname) && t<=to);

	if(t>to)	return false;
	kx = d.nx;	ky = d.ny;	kz = d.nz;
	b = (float *)malloc(kx*ky*kz*sizeof(float));
	memcpy(b,d.a,kx*ky*kz*sizeof(float));

	// read other files
	for(;t<=to;t+=step)
	{
		sprintf(fname,templ,t);
		if(d.Read(fname))
			if(!mgl_add_file(kx,ky,kz,b,d,as_slice))
				return false;
	}
	Set(b,kx,ky,kz);
	delete []fname;		free(b);
	return true;
}
//-----------------------------------------------------------------------------
bool mglData::ReadAll(const char *templ, bool as_slice)
{
#ifndef WIN32
	mglData d;
	glob_t res;
	long i;
	float *b;
	long kx,ky,kz;
	char *fname = new char[256];
	glob (templ, GLOB_TILDE, NULL, &res);

	//read first file
	for(i=0;i<res.gl_pathc;i++)
		if(d.Read(res.gl_pathv[i]))	break;

	if(i>=res.gl_pathc)	return false;
	kx = d.nx;	ky = d.ny;	kz = d.nz;
	b = (float *)malloc(kx*ky*kz*sizeof(float));
	memcpy(b,d.a,kx*ky*kz*sizeof(float));

	for(;i<res.gl_pathc;i++)
	{
		if(d.Read(res.gl_pathv[i]))
			if(!mgl_add_file(kx,ky,kz,b,d,as_slice))
				return false;
	}
	Set(b,kx,ky,kz);

	globfree (&res);
	delete []fname;		free(b);
	return true;
#else
    return false;
#endif
}
//-----------------------------------------------------------------------------
