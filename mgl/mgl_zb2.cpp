/* mgl_zb2.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <mathgl.abalakin@gmail.com>
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
#include "mgl/mgl_ab.h"
//-----------------------------------------------------------------------------
//
//	Low level plot functions for ZBuffer
//
//-----------------------------------------------------------------------------
void mglGraphAB::curv_plot(long n,float *pp,bool *tt)
{
	register long i;
	float *p;
	if(!pp || n<2)	return;
	PostScale(pp,n);
	for(i=0;i<n-1;i++)
	{
		p = pp+3*i;
		if(!tt || (tt[i] && tt[i+1]))	line_plot(p,p+3,CDef,CDef);
	}
	if(Arrow1>='A' && (!tt || tt[n-1]))	arrow_plot(pp+3*n-3,pp+3*n-6,Arrow1);
	if(Arrow2>='A' && (!tt || tt[0]))	arrow_plot(pp,pp+3,Arrow2);
}
//-----------------------------------------------------------------------------
void mglGraphAB::curv_plot(long n,float *pp,float *cc,bool *tt)
{
	register long i;
	float *p,c1[4],c2[4],a;
	mglColor c;
	if(!pp || n<2)	return;
	PostScale(pp,n);
	for(i=0;i<n-1;i++)
	{
		p = pp+3*i;
		if(!tt || (tt[i] && tt[i+1]))
		{
			memcpy(c1,CDef,4);		memcpy(c2,CDef,4);
			if(cc[i])
			{
				a = (2*cc[i]-Cmin-Cmax)/(Cmax-Cmin);
				if(a>1)	a=1;	if(a<-1)	a=-1;
				c = GetC(a,false);
				c1[0] = c.r;	c1[1] = c.g;	c1[2] = c.b;	c1[3] = (a+1)/2;
			}
			if(cc[i+1])
			{
				a = (2*cc[i+1]-Cmin-Cmax)/(Cmax-Cmin);
				if(a>1)	a=1;	if(a<-1)	a=-1;
				c = GetC(a,false);
				c2[0] = c.r;	c2[1] = c.g;	c2[2] = c.b;	c2[3] = (a+1)/2;
			}
			line_plot(p,p+3,c1,c2);
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraphAB::curv_plot(long n,float *pp,bool *tt,long *nn)
{
	register long i,k=0;
	if(!pp || !nn || n<2)	return;
	PostScale(pp,n);
	float *p1,*p2;
	for(i=0;i<n;i++)
	{
		k = nn[i];
		if(k<0)	continue;
		p1 = pp+3*i;	p2 = pp+3*k;
		if(!tt || (tt[i] && tt[k]))	line_plot(p1,p2,CDef,CDef);
	}
}
//-----------------------------------------------------------------------------
void mglGraphAB::Mark(float x,float y,float z, char type)
{
	float ps[3] = {x,y,z};
	PostScale(ps,1);
	mark_plot(ps,type);
}
//-----------------------------------------------------------------------------
void mglGraphAB::mesh_plot(long n,long m,float *pp,float *cc,bool *tt, int how)
{
	register long i,j,i0,d=3*n;
	float *p,*c;
	long dx=1,dy=1;
	if(MeshNum>1)	{	dx=(n-1)/(MeshNum-1);	dy=(m-1)/(MeshNum-1);	}
	if(!DrawFace)	{	dx=(n-1)/9;	dy=(m-1)/9;	}
	if(dx<1)	dx=1;
	if(dy<1)	dy=1;
	if(!pp || n<2 || m<2)	return;
	if(how<0)	how = -how;
	else		PostScale(pp,n*m);
	if(how&2) for(i=0;i<n-1;i++)	for(j=0;j<m;j+=dy)
	{
		i0 = i+n*j;		p = pp+3*i0;	c = cc+3*i0;
		if(!tt || (tt[i0] && tt[i0+1]))
		{
			if(cc)	line_plot(p,p+3,c,c+3);
			else	line_plot(p,p+3,CDef,CDef);
		}
	}
	if(how&1)	for(i=0;i<n;i+=dx)	for(j=0;j<m-1;j++)
	{
		i0 = i+n*j;		p = pp+3*i0;	c = cc+3*i0;
		if(!tt || (tt[i0] && tt[i0+n]))
		{
			if(cc)	line_plot(p,p+d,c,c+d);
			else	line_plot(p,p+d,CDef,CDef);
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraphAB::wire_plot(long n,long m,float *pp,float *cc,bool *tt)
{
	register long i,j,i0,d=3*n;
	float *p,*c;
	long dx=1,dy=1;
	if(!DrawFace)	{	dx=(n-1)/9;	dy=(m-1)/9;	}
	if(dx<1)	dx=1;	if(dy<1)	dy=1;
	for(i=0;i<n-1;i++)	for(j=0;j<m;j+=dy)
	{
		i0 = i+n*j;		p = pp+3*i0;	c = cc+4*i0;
		if(!tt || (tt[i0] && tt[i0+1]))
		{
			if(cc)	line_plot(p,p+3,c,c+4);
			else	line_plot(p,p+3,CDef,CDef);
		}
	}
	for(i=0;i<n;i+=dx)	for(j=0;j<m-1;j++)
	{
		i0 = i+n*j;		p = pp+3*i0;	c = cc+4*i0;
		if(!tt || (tt[i0] && tt[i0+n]))
		{
			if(cc)	line_plot(p,p+d,c,c+4*n);
			else	line_plot(p,p+d,CDef,CDef);
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraphAB::surf_plot(long n,long m,float *pp,float *cc,bool *tt)
{
	register long i,j,i0;
	float *p,*c,*ns,d1[3],d2[3],*s;
	long k=3*n;
	if(!pp || n<2 || m<2)	return;
	PostScale(pp,n*m);	LightScale();
	if(!DrawFace)	{	wire_plot(n,m,pp,cc,tt);	return;	}
	ns = new float[3*n*m];
	for(i=0;i<n-1;i++)	for(j=0;j<m-1;j++)
	{
		i0 = 3*(i+n*j);
		d1[0] = pp[i0+3]-pp[i0];	d2[0] = pp[i0+k]-pp[i0];
		d1[1] = pp[i0+4]-pp[i0+1];	d2[1] = pp[i0+k+1]-pp[i0+1];
		d1[2] = pp[i0+5]-pp[i0+2];	d2[2] = pp[i0+k+2]-pp[i0+2];
		ns[i0]  =(d2[2]*d1[1]-d2[1]*d1[2]);		// normal vector
		ns[i0+1]=(d2[0]*d1[2]-d2[2]*d1[0]);
		ns[i0+2]=(d2[1]*d1[0]-d2[0]*d1[1]);
	}
	memcpy(ns+3*(m-1)*n,ns+3*(m-2)*n,3*n*sizeof(float));
	for(i=0;i<m;i++)
	{
		i0 = 3*(n-2+i*n);
		ns[i0+3]=ns[i0];
		ns[i0+4]=ns[i0+1];
		ns[i0+5]=ns[i0+2];
	}
	if(cc)
	{
		for(i=0;i<n-1;i++)	for(j=0;j<m-1;j++)
		{
			i0 = i+n*j;		p = pp+3*i0;	c = cc+4*i0;	s = ns+3*i0;
			if(!tt || (tt[i0] && tt[i0+1] && tt[i0+n] && tt[i0+1+n]))
				quad_plot_n(p,p+3,p+k,p+k+3,c,c+4,c+4*n,c+4*n+4,s,s+3,s+k,s+k+3);
			else if(tt[i0] && tt[i0+1] && tt[i0+n])
				trig_plot_n(p,p+3,p+k,c,c+4,c+4*n,s,s+3,s+k);
			else if(tt[i0] && tt[i0+1] && tt[i0+n+1])
				trig_plot_n(p,p+3,p+k+3,c,c+4,c+4*n+4,s,s+3,s+k+3);
			else if(tt[i0] && tt[i0+n+1] && tt[i0+n])
				trig_plot_n(p,p+k+3,p+k,c,c+4*n+4,c+4*n,s,s+k+3,s+k);
			else if(tt[i0+n+1] && tt[i0+1] && tt[i0+n])
				trig_plot_n(p+k+3,p+3,p+k,c+4+4*n,c+4,c+4*n,s+3+k,s+3,s+k);
		}
	}
	else
	{
		for(i=0;i<n-1;i++)	for(j=0;j<m-1;j++)
		{
			i0 = i+n*j;		p = pp+3*i0;	s = ns+3*i0;
			if(!tt || (tt[i0] && tt[i0+1] && tt[i0+n] && tt[i0+1+n]))
				quad_plot_n(p,p+3,p+k,p+k+3,CDef,CDef,CDef,CDef,s,s+3,s+k,s+k+3);
			else if(tt[i0] && tt[i0+1] && tt[i0+n])
				trig_plot_n(p,p+3,p+k,CDef,CDef,CDef,s,s+3,s+k);
			else if(tt[i0] && tt[i0+1] && tt[i0+n+1])
				trig_plot_n(p,p+3,p+k+3,CDef,CDef,CDef,s,s+3,s+k+3);
			else if(tt[i0] && tt[i0+n+1] && tt[i0+n])
				trig_plot_n(p,p+k+3,p+k,CDef,CDef,CDef,s,s+k+3,s+k);
			else if(tt[i0+n+1] && tt[i0+1] && tt[i0+n])
				trig_plot_n(p+k+3,p+3,p+k,CDef,CDef,CDef,s+3+k,s+3,s+k);
		}
	}
	delete []ns;
}
//-----------------------------------------------------------------------------
void mglGraphAB::boxs_plot(long n, long m, float *pp, mglColor *cc, bool *tt,
						float alpha,bool line)
{
	if(!DrawFace)	line = true;
	register long i,j,i0,i1,d=3*n;
	bool h1,h2,s1,s2,s3,s4;
	float cs[4],*p0,*p1,bs[4]={0,0,0,255};
	if(!pp || !cc)	return;
	PostScale(pp,n*m+4*(n-1)*(m-1));	LightScale();
	alpha = Transparent ? AlphaDef : 1;
	for(i=0;i<n-1;i++)	for(j=0;j<m-1;j++)
	{
		i0 = i+n*j;
		i1 = 4*(i+(n-1)*j) + n*m;
		p0 = pp+3*i0;	p1 = pp+3*i1;

		h1=h2=s1=s2=s3=s4=true;
		if(tt)
		{
			if(!tt[i0])		h1 = s1 = s2 = false;
			if(!tt[i0+1])	h1 = s1 = s3 = false;
			if(!tt[i0+n])	h1 = s2 = s4 = false;
			if(!tt[i0+1+n])	h1 = s3 = s4 = false;
			if(!tt[i1])		h2 = s1 = s2 = false;
			if(!tt[i1+1])	h2 = s1 = s3 = false;
			if(!tt[i1+2])	h2 = s2 = s4 = false;
			if(!tt[i1+3])	h2 = s3 = s4 = false;
		}

		cs[0] = cc[i0].r;	cs[1] = cc[i0].g;
		cs[2] = cc[i0].b;	cs[3] = alpha;
		if(h1)
		{
			if(DrawFace)	quad_plot(p0,p0+3,p0+d,p0+d+3,cs,cs,cs,cs);
			if(line)
			{
				line_plot(p0,p0+3,bs,bs);
				line_plot(p0,p0+d,bs,bs);
				line_plot(p0+d,p0+d+3,bs,bs);
				line_plot(p0+3,p0+d+3,bs,bs);
			}
		}
		if(h2)
		{
			if(DrawFace)	quad_plot(p1,p1+3,p1+6,p1+9,cs,cs,cs,cs);
			if(line)
			{
				line_plot(p1,p1+3,bs,bs);
				line_plot(p1,p1+6,bs,bs);
				line_plot(p1+6,p1+9,bs,bs);
				line_plot(p1+3,p1+9,bs,bs);
			}
		}
		if(s1)
		{
			if(DrawFace)	quad_plot(p0,p0+3,p1,p1+3,cs,cs,cs,cs);
			if(line)
			{
				line_plot(p0,p0+3,bs,bs);
				line_plot(p0,p1,bs,bs);
				line_plot(p0+3,p1+3,bs,bs);
				line_plot(p1,p1+3,bs,bs);
			}
		}
		if(s2)
		{
			if(DrawFace)	quad_plot(p0,p0+d,p1,p1+6,cs,cs,cs,cs);
			if(line)
			{
				line_plot(p0,p0+d,bs,bs);
				line_plot(p0,p1,bs,bs);
				line_plot(p0+d,p1+6,bs,bs);
				line_plot(p1,p1+6,bs,bs);
			}
		}
		if(s3)
		{
			if(DrawFace)	quad_plot(p0+3,p0+3+d,p1+3,p1+9,cs,cs,cs,cs);
			if(line)
			{
				line_plot(p0+3,p0+3+d,bs,bs);
				line_plot(p0+3,p1+3,bs,bs);
				line_plot(p0+3+d,p1+9,bs,bs);
				line_plot(p1+3,p1+9,bs,bs);
			}
		}
		if(s4)
		{
			if(DrawFace)	quad_plot(p0+d,p0+d+3,p1+6,p1+9,cs,cs,cs,cs);
			if(line)
			{
				line_plot(p0+d,p0+3+d,bs,bs);
				line_plot(p0+d,p1+6,bs,bs);
				line_plot(p0+d+3,p1+9,bs,bs);
				line_plot(p1+6,p1+9,bs,bs);
			}
		}
	}
}
//-----------------------------------------------------------------------------
float mgl_cos_pp(float *pp,long i0,long i1,long i2)
{
	float p1=0,p2=0,pc=0,dp1,dp2;
	dp1 = pp[3*i1]-pp[3*i0];		dp2 = pp[3*i2]-pp[3*i0];
	p1 = dp1*dp1;	p2 = dp2*dp2;	pc = dp1*dp2;
	dp1 = pp[3*i1+1]-pp[3*i0+1];	dp2 = pp[3*i2+1]-pp[3*i0+1];
	p1 += dp1*dp1;	p2 += dp2*dp2;	pc += dp1*dp2;
	dp1 = pp[3*i1+2]-pp[3*i0+2];	dp2 = pp[3*i2+2]-pp[3*i0+2];
	p1 += dp1*dp1;	p2 += dp2*dp2;	pc += dp1*dp2;

	return p1*p2>1e-10 ? pc/sqrt(p1*p2) : NAN;
}
//-----------------------------------------------------------------------------
void mglGraphAB::surf3_plot(long n,long m,long *kx1,long *kx2,long *ky1,long *ky2, long *kz,float *pp,float *cc,float *kk,float *nn,bool wire)
{
	if(!DrawFace)	wire=true;	//return;	//wire=true;
	register long i,j,k,i0,ii,jj;
	long id[12],us[12],ni;
	float d,d0,p[9],s[9],*c0,*c1,*c2=cc;
	LightScale();	// set up light source

	for(i=0;i<n-1;i++)	for(j=0;j<m-1;j++)
	{
		i0 = i+n*j;
		// find ID of points of Surf3 intersection with cell i0
		memset(id,-1,12*sizeof(long));	ni = 0;
		if(kx1[i0]>=0)		id[ni++] = kx1[i0];
		if(ky1[i0]>=0)		id[ni++] = ky1[i0];
		if(kx1[i0+n]>=0)	id[ni++] = kx1[i0+n];
		if(ky1[i0+1]>=0)	id[ni++] = ky1[i0+1];
		if(kz[i0]>=0)		id[ni++] = kz[i0];
		if(kz[i0+1]>=0)		id[ni++] = kz[i0+1];
		if(kz[i0+n+1]>=0)	id[ni++] = kz[i0+n+1];
		if(kz[i0+n]>=0)		id[ni++] = kz[i0+n];
		if(kx2[i0]>=0)		id[ni++] = kx2[i0];
		if(ky2[i0]>=0)		id[ni++] = ky2[i0];
		if(kx2[i0+n]>=0)	id[ni++] = kx2[i0+n];
		if(ky2[i0+1]>=0)	id[ni++] = ky2[i0+1];
		if(ni<3)	continue;
		// remove points which is too close to first one
		for(jj=1;jj<ni;)
		{
			d = fabs(kk[3*id[jj]]-kk[3*id[0]]) + fabs(kk[3*id[jj]+1]-kk[3*id[0]+1]) + fabs(kk[3*id[jj]+2]-kk[3*id[0]+2]);
			if(d>1e-5)	jj++;
			else
			{	ni--;	for(ii=jj;ii<ni;ii++)	id[ii]=id[ii+1];	}
		}
		// continue if number of points <3 i.e. there is no triangle
		if(ni<3)	continue;
		memset(us,0,12*sizeof(long));
		// firstly let find most outstanding point
		for(jj=1,ii=2,d0=2;ii<ni;ii++)
		{
			d = mgl_cos_pp(kk,id[0],id[ii],id[1]);
			if(d<d0)	{	d0=d;	jj=ii;	}
		}
		// copy first 2 points as base
		memcpy(p,pp+3*id[0],3*sizeof(float));
		memcpy(p+3,pp+3*id[jj],3*sizeof(float));
		memcpy(p+6,pp+3*id[1],3*sizeof(float));	// for correct orientation of triangles (in IDTF)
		memcpy(s,nn+3*id[0],3*sizeof(float));
		memcpy(s+3,nn+3*id[jj],3*sizeof(float));
		PostScale(p,3);		NormScale(s,2);
		// select the same orientation of all triangles of the surface
		bool proj = (s[0]*((p[4]-p[1])*(p[8]-p[2])-(p[5]-p[2])*(p[7]-p[1])) +
				s[1]*((p[5]-p[2])*(p[6]-p[0])-(p[3]-p[0])*(p[8]-p[2])) +
				s[2]*((p[3]-p[0])*(p[7]-p[1])-(p[4]-p[1])*(p[6]-p[0])))<0;
		us[0] = us[jj] = 1;
		c0 = cc ? cc+4*id[0] : CDef;
		c1 = cc ? cc+4*id[jj] : CDef;
		if(wire)	line_plot(p,p+3,c0,c1,true);
		// find all triangles
		for(k=2;k<ni;k++)
		{
			// find closest point in sence cosine of angle
			for(i0=-1,ii=1,d0=-2;ii<ni;ii++)
			{
				if(us[ii])	continue;
				d = mgl_cos_pp(kk,id[0],id[ii],id[jj]);
				if(d>d0)	{	d0=d;	i0=ii;	}
			}
			if(i0<0)	break;	// no more triangles. NOTE: should be never here
			jj = i0;	us[jj]=1;
			memcpy(p+6,pp+3*id[jj],3*sizeof(float));
			memcpy(s+6,nn+3*id[jj],3*sizeof(float));
			PostScale(p+6,1);	NormScale(s+6,1);
			c2 = cc ? cc+4*id[jj] : CDef;
			if(!wire)
			{
				// smoothing only for Cartesian
				if(!fx && !fy && !fz)
				{
					if(proj)	trig_plot_n(p,p+6,p+3,c0,c1,c2,s,s+6,s+3);
					else		trig_plot_n(p,p+3,p+6,c0,c1,c2,s,s+3,s+6);
				}
				else
				{
					if(proj)	trig_plot(p,p+6,p+3,c0,c1,c2);
					else		trig_plot(p,p+3,p+6,c0,c1,c2);
				}
			}
			else	line_plot(p+3,p+6,c1,c2,true);
			memcpy(p+3,p+6,3*sizeof(float));	c1=c2;
			memcpy(s+3,s+6,3*sizeof(float));
		}
		if(wire)	line_plot(p,p+6,c0,c2,true);
	}
}
//-----------------------------------------------------------------------------
void mglGraphAB::axial_plot(long n,float *pp,long *nn,long np, bool wire)
{
	if(!DrawFace)	wire=true;
	register long i,j;
	bool g1,g2;
	float r1,r2,y1,y2,c1,s1,ph,p1[6],p2[6],n1[6],n2[6],d;
	if(!pp || !nn)	return;
	LightScale();	// set up light source
	long k0=0,k1=1,k2=2;
	if(AxialDir=='y')	{	k0=0;k1=2;k2=1;	}
	if(AxialDir=='x')	{	k0=2;k1=1;k2=0;	}
	for(i=0;i<n;i++)
	{
		if(nn[i]<0)	continue;
		r1 = pp[3*i];		y1 = pp[3*i+1];
		r2 = pp[3*nn[i]];	y2 = pp[3*nn[i]+1];
		g1 = g2 = false;
		for(j=0;j<=np;j++)
		{
			ph = 2.*j*M_PI/np;
			c1 = cos(ph);	s1 = sin(ph);
			p1[k0] = r1*c1;		p1[k2] = y1;	p1[k1] = r1*s1;
			p1[k0+3] = r2*c1;	p1[k2+3] = y2;	p1[k1+3] = r2*s1;
			d = y2-y1;
			n1[k0] = -d*c1;	n1[k2] = r2-r1;		n1[k1] = -d*s1;
			if(!wire && nn[nn[i]]>=0)
			{
				d = pp[3*nn[nn[i]]+1]-y2;
				n1[k0+3] = -d*c1;	n1[k2+3] = pp[3*nn[nn[i]]]-r2;	n1[k1+3] = -d*s1;
			}
			else
			{	n1[3] = n1[0];	n1[4] = n1[1];	n1[5] = n1[2];	}
			g1 = ScalePoint(p1[0],p1[1],p1[2]) && ScalePoint(p1[3],p1[4],p1[5]);
			PostScale(p1,2);
			NormScale(n1,2);
			if(g1 && g2)
			{
				if(wire)
				{
					line_plot(p1,p1+3,CDef,CDef,true);
					line_plot(p1,p2,CDef,CDef,true);
					line_plot(p1+3,p2+3,CDef,CDef,true);
					line_plot(p2,p2+3,CDef,CDef,true);
				}
				else
				{
					if((!fx && !fy && !fz))	// сглаживание только для декартовой с.к.
						quad_plot_n(p1,p1+3,p2,p2+3,CDef,CDef,CDef,CDef,n1,n1+3,n2,n2+3);
					else
						quad_plot(p1,p1+3,p2,p2+3,CDef,CDef,CDef,CDef);
				}
			}
			memcpy(p2,p1,6*sizeof(float));
			memcpy(n2,n1,6*sizeof(float));
			g2 = g1;
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraphAB::quads_plot(long n,float *pp,float *cc,bool *tt)
{
//	if(!DrawFace)	return;
	register long i;
	float *p,*c;
	PostScale(pp,4*n);	LightScale();
	if(cc)
	{
		for(i=0;i<n;i++)
		{
			if(tt && (!tt[4*i] || !tt[4*i+1] || !tt[4*i+2] || !tt[4*i+3]))
				continue;
			p = pp+12*i;	c = cc+16*i;
			if(DrawFace)
				quad_plot(p,p+3,p+9,p+6,c,c+4,c+12,c+8);
			else
			{
				line_plot(p,p+3,c,c+4);
				line_plot(p,p+9,c,c+12);
				line_plot(p+9,p+6,c+12,c+8);
				line_plot(p+3,p+6,c+4,c+8);
			}
		}
	}
	else
	{
		for(i=0;i<n;i++)
		{
			if(tt && (!tt[4*i] || !tt[4*i+1] || !tt[4*i+2] || !tt[4*i+3]))
				continue;
			p = pp+12*i;	//c = cc+16*i;
			if(DrawFace)
//				quad_plot(p,p+3,p+9,p+6,CDef,CDef,CDef,CDef);
			{
				trig_plot(p,p+3,p+9,CDef,CDef,CDef);
				trig_plot(p+3,p+6,p+9,CDef,CDef,CDef);
			}
			else
			{
				line_plot(p,p+3,CDef,CDef);
				line_plot(p,p+9,CDef,CDef);
				line_plot(p+9,p+6,CDef,CDef);
				line_plot(p+3,p+6,CDef,CDef);
			}
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraphAB::trigs_plot(long n, long *nn, long m, float *pp, float *cc, bool *tt,bool wire)
{
	if(!DrawFace)	wire=true;
	register long i,j1,j2,j3;
	PostScale(pp,m);	LightScale();
	if(cc)
	{
		for(i=0;i<n;i++)
		{
			j1 = nn[3*i];	j2 = nn[3*i+1];	j3 = nn[3*i+2];
			if(tt && (!tt[j1] || !tt[j2] || !tt[j3]))	continue;
			if(wire)
			{
				line_plot(pp+3*j1,pp+3*j2,cc+4*j1,cc+4*j2);
				line_plot(pp+3*j1,pp+3*j3,cc+4*j1,cc+4*j3);
				line_plot(pp+3*j3,pp+3*j2,cc+4*j3,cc+4*j2);
			}
			else
				trig_plot(pp+3*j1,pp+3*j2,pp+3*j3,cc+4*j1,cc+4*j2,cc+4*j3);
		}
	}
	else
	{
		for(i=0;i<n;i++)
		{
			j1 = nn[3*i];	j2 = nn[3*i+1];	j3 = nn[3*i+2];
			if(tt && (!tt[j1] || !tt[j2] || !tt[j3]))	continue;
			if(wire)
			{
				line_plot(pp+3*j1,pp+3*j2,CDef,CDef);
				line_plot(pp+3*j1,pp+3*j3,CDef,CDef);
				line_plot(pp+3*j3,pp+3*j2,CDef,CDef);
			}
			else
				trig_plot(pp+3*j1,pp+3*j2,pp+3*j3,CDef,CDef,CDef);
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraphAB::lines_plot(long n,float *pp,float *cc,bool *tt)
{
	register long i;
	float s1[4],s2[4],*p;
	mglColor col,c1=cmap[0],c2=NumCol>1?cmap[1]:cmap[0];
	s1[3] = s2[3] = AlphaDef;
	PostScale(pp,2*n);
	long pOld = PDef;
	PDef = 0xffff;
	for(i=0;i<n;i++)
	{
		p = pp+6*i;
		if(tt && (!tt[2*i] || !tt[2*i+1]))	continue;
		if(cc)
		{
			if(cc[i])
			{
				col = GetC(cc[i]-0.5,false);
				s1[0] = col.r;	s1[1] = col.g;	s1[2] = col.b;
				col = GetC(cc[i],false);
				s2[0] = col.r;	s2[1] = col.g;	s2[2] = col.b;
			}
		}
		else
		{
			s1[0] = c1.r;	s1[1] = c1.g;	s1[2] = c1.b;
			s2[0] = c2.r;	s2[1] = c2.g;	s2[2] = c2.b;
		}
		memcpy(CDef,s1,4*sizeof(float));
		line_plot(p,p+3,s1,s2);		ball(p, CDef);
	}
	PDef = pOld;
}
//-----------------------------------------------------------------------------
void mglGraphAB::cloud_plot(long nx,long ny,long nz,float *pp,float *a,float alpha)
{
	register long i,j,k,i0;
	if(!pp || !a || !DrawFace || alpha==0)	return;
	mglColor col;
	float *aa=new float[nx*ny*nz],*p,*c;
	bool *tt=new bool[nx*ny*nz];
	long dy=3*nx,dz=3*ny*nx,d1=nx,d2=nx*ny;
	if(!aa || !tt)
	{	delete []aa;	delete []tt;	return;	}
	for(i=0;i<nx*ny*nz;i++)
	{
		tt[i] = ScalePoint(pp[3*i],pp[3*i+1],pp[3*i+2]) && !isnan(a[i]);
		aa[i] = GetA(a[i]);
	}
	PostScale(pp,nx*ny*nz);

	for(i=0;i<nx;i++)	for(j=0;j<ny;j++)	for(k=0;k<nz;k++)
	{
		i0 = i+nx*(j+ny*k);	p = pp+3*i0;	c = aa+i0;
		if(!tt[i0])	continue;
		if(i<nx-1 && j<ny-1 && tt[i0+1] && tt[i0+nx] && tt[i0+nx+1])
			quad_plot_a(p,p+3,p+dy,p+3+dy,c[0],c[1],c[d1],c[1+d1],alpha/4);
		if(i<nx-1 && k<nz-1 && tt[i0+1] && tt[i0+nx*ny] && tt[i0+nx*ny+1])
			quad_plot_a(p,p+3,p+dz,p+3+dz,c[0],c[1],c[d2],c[1+d2],alpha/4);
		if(k<nz-1 && j<ny-1 && tt[i0+nx*ny] && tt[i0+nx] && tt[i0+nx*ny+nx])
			quad_plot_a(p,p+dy,p+dz,p+dz+dy,c[0],c[d1],c[d2],c[d1+d2],alpha/4);
	}
	delete []aa;	delete []tt;
}
//-----------------------------------------------------------------------------
void mglGraphAB::combine(unsigned char *c1,unsigned char *c2)
// смешение цветов; цвет c2 лежит выше цвета c1 !!!
{
	if(!c2[3])	return;
	unsigned int a1=c1[3], a2=c2[3],b1=255-a2,b2,b3;
	if(a1==0 || a2==255)	{	memcpy(c1,c2,4);	return; }
	if(!TranspType)
	{
		c1[0] = (c1[0]*b1 + c2[0]*a2)/256;
		c1[1] = (c1[1]*b1 + c2[1]*a2)/256;
		c1[2] = (c1[2]*b1 + c2[2]*a2)/256;
		c1[3] = (unsigned char)(a2+a1*b1/255);
	}
	else if(TranspType==1)
	{
		c1[0] = (unsigned char)((255-a1*(255-c1[0])/256)*(255-a2*(255-c2[0])/256)/256);
		c1[1] = (unsigned char)((255-a1*(255-c1[1])/256)*(255-a2*(255-c2[1])/256)/256);
		c1[2] = (unsigned char)((255-a1*(255-c1[2])/256)*(255-a2*(255-c2[2])/256)/256);
		c1[3] = 255;
	}
	else if(TranspType==2)
	{
		b1 = (c1[0]*a1 + c2[0]*a2)/256;		c1[0] = b1<255 ? b1 : 255;
		b2 = (c1[1]*a1 + c2[1]*a2)/256;		c1[1] = b2<255 ? b2 : 255;
		b3 = (c1[2]*a1 + c2[2]*a2)/256;		c1[2] = b3<255 ? b3 : 255;
		c1[3] = a1+a2>255? 255 : a1+a2;
	}
}
//-----------------------------------------------------------------------------
void mglGraphAB::Glyph(float x,float y, float f, int nt, const short *trig, int nl, const short *line)
{
	long ik,ii,il;
	float p[9],n[3]={0,0,0};
	if(trig && nt>0)
	{
		for(ik=0;ik<nt;ik++)
		{
			ii = 6*ik;	p[0]=f*trig[ii]+x;	p[1]=f*trig[ii+1]+y;	p[2]=0;
			ii+=2;		p[3]=f*trig[ii]+x;	p[4]=f*trig[ii+1]+y;	p[5]=0;
			ii+=2;		p[6]=f*trig[ii]+x;	p[7]=f*trig[ii+1]+y;	p[8]=0;
			PostScale(p,3);
			trig_plot_n(p,p+3,p+6,CDef,CDef,CDef,n,n,n);
		}
	}
	else if(line && nl>0)
	{
		il = 0;
		for(ik=0;ik<nl-1;ik++)
		{
			ii = 2*ik;
			if(line[ii]==0x3fff && line[ii+1]==0x3fff)	// line breakthrough
			{	il = ik+1;	continue;	}
			else if(ik==nl-1 || (line[ii+2]==0x3fff && line[ii+3]==0x3fff))
			{	// enclose the circle. May be in future this block should be commented
				p[0]=f*line[ii]+x;	p[1]=f*line[ii+1]+y;	p[2]=0;	ii=2*il;
				p[3]=f*line[ii]+x;	p[4]=f*line[ii+1]+y;	p[5]=0;
			}
			else
			{	// normal line
				p[0]=f*line[ii]+x;	p[1]=f*line[ii+1]+y;	p[2]=0;	ii+=2;
				p[3]=f*line[ii]+x;	p[4]=f*line[ii+1]+y;	p[5]=0;
			}
			PostScale(p,2);
			line_plot(p,p+3,CDef,CDef);
		}
	}
	if(nl<0)	// overline or underline
	{
		p[0]=x;		p[1]=y;	p[2]=0;
		p[3]=fabs(f)+x;	p[4]=y;	p[5]=0;
		PostScale(p,2);
		line_plot(p,p+3,CDef,CDef);
	}
}
//-----------------------------------------------------------------------------
