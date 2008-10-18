/* mgl_eps.cpp is part of Math Graphic Library
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
#include <time.h>
#include "mgl/mgl_eps.h"
#include "mgl/mgl_c.h"
#include "mgl/mgl_f.h"
//-----------------------------------------------------------------------------
struct mglPrim
{
	float x[4], y[4];	// coordinates of corners
	float z;			// z-position
	float s;			// size (if applicable)
	float w;			// width (if applicable)
	float c[4];			// color
	char m;				// mark (if applicable)
	int type;			// type of primitive (0 - point, 1 - line, 2 - trig, 3 - quad)
	int style;			// style of pen
	unsigned short dash;// real pen dashing
	void Draw(mglGraphPS *gr);
	bool IsSame(float wp,float *cp,int st);
	void SetStyle(unsigned PDef, int pPos);
	mglPrim(int t=0)	{	memset(this,0,sizeof(mglPrim));	type = t;	c[3]=1;	};
};
//-----------------------------------------------------------------------------
int mgl_compare_prim(const void *p1, const void *p2)
{
	mglPrim *a1=(mglPrim *)p1, *a2=(mglPrim *)p2;
	if( a1->z < a2->z-0.5 )	return -1;
	if( a1->z > a2->z+0.5 )	return 1;
	if( a1->w < a2->w )	return -1;
	if( a1->w > a2->w )	return 1;
	if( a1->style < a2->style )	return -1;
	if( a1->style > a2->style )	return 1;
	if( a2->type != a1->type )	return a2->type - a1->type;
	if( a1->type==1 && (a1->x[0]!=a2->x[0]) )
		return (a2->x[0]<a1->x[0]) ? 1 : -1;
	if( a1->type==1 )	return (a2->y[0]<a1->y[0]) ? 1 : -1;
	return 0;
}
//-----------------------------------------------------------------------------
/// Create mglGraph object in PostScript mode.
HMGL mgl_create_graph_ps(int width, int height)
{    return new mglGraphPS(width,height);	}
/// Create mglGraph object in PostScript mode.
uintptr_t mgl_create_graph_ps_(int *width, int *height)
{    return uintptr_t(new mglGraphPS(*width,*height));	}
//-----------------------------------------------------------------------------
mglGraphPS::mglGraphPS(int w,int h) : mglGraphAB(w,h)
{	P = 0;	pMax = pNum = 0;	}
//-----------------------------------------------------------------------------
mglGraphPS::~mglGraphPS()	{	if(P)	delete []P;	}
//-----------------------------------------------------------------------------
void mglGraphPS::Ball(float x,float y,float z,mglColor col,float alpha)
{
	if(alpha==0)	return;
	if(alpha<0)	{	alpha = -alpha;	}
	else		{	if(!ScalePoint(x,y,z))	return;	}
	if(!col.Valid())	col = mglColor(1.,0.,0.);
	float p[3] = {x,y,z};	PostScale(p,1);
	mglPrim a;		a.m = '.';
	a.x[0] = p[0];	a.y[0] = p[1];	a.z = p[2];
	a.c[0] = col.r;	a.c[1] = col.g;	a.c[2] = col.b;
	add_prim(&a);
}
//-----------------------------------------------------------------------------
void mglGraphPS::ball(float *p,float *c)
{
	mglPrim a;	PostScale(p,1);		a.m = '.';
	a.x[0] = p[0];	a.y[0] = p[1];	a.z = p[2];
	memcpy(a.c,c,3*sizeof(float));
	add_prim(&a);
}
//-----------------------------------------------------------------------------
void mglGraphPS::mark_plot(float *p, char type)
{
	mglPrim a;		a.m = type;		a.s = MarkSize;
	a.x[0] = p[0];	a.y[0] = p[1];	a.z = p[2];
	memcpy(a.c,CDef,3*sizeof(float));
	add_prim(&a);
}
//-----------------------------------------------------------------------------
void mglGraphPS::line_plot(float *p1,float *p2,float *c1,float *,bool all)
{
	if((PDef==0 && !all) || (fabs(p1[0]-p2[0])<0.01 && fabs(p1[1]-p2[1])<0.01))	return;
	mglPrim a(1);
	float pw = fabs(PenWidth);
	a.z = (p1[2]+p2[2])/2;	a.w = pw;
	if(pw>1)	a.z += pw-1;
	a.x[0]=p1[0];	a.y[0]=p1[1];	a.x[1]=p2[0];	a.y[1]=p2[1];
//	a.c[0]=(c1[0]+c2[0])/2;	a.c[1]=(c1[1]+c2[1])/2;	a.c[2]=(c1[2]+c2[2])/2;
	a.c[0]=c1[0];	a.c[1]=c1[1];	a.c[2]=c1[2];
	a.SetStyle(all? 0xffff:PDef,int(pPos));
	add_prim(&a);
	pPos = fmod(pPos+hypot(p2[0]-p1[0], p2[1]-p1[1])/pw/1.5, 16);
}
//-----------------------------------------------------------------------------
void mglGraphPS::trig_plot(float *p3,float *p1,float *p2,float *c3,float *c1,float *c2)
{
	bool pnt = fabs(p1[0]-p2[0])<0.01 && fabs(p1[1]-p2[1])<0.01 &&
				fabs(p1[0]-p3[0])<0.01 && fabs(p1[1]-p3[1])<0.01;
	if(pnt)	return;
	mglPrim a(2);
	a.z = (p1[2]+p2[2]+p3[2])/3;
	a.x[0]=p1[0];	a.y[0]=p1[1];	a.x[1]=p2[0];	a.y[1]=p2[1];	a.x[2]=p3[0];	a.y[2]=p3[1];

	float d1[3],d2[3];
	d1[0] = p1[0]-p3[0];	d2[0] = p2[0]-p3[0];	d1[1] = p1[1]-p3[1];
	d2[1] = p2[1]-p3[1];	d1[2] = p1[2]-p3[2];	d2[2] = p2[2]-p3[2];
	a.c[0]=(c1[0]+c2[0]+c3[0])/3;	a.c[1]=(c1[1]+c2[1]+c3[1])/3;
	a.c[2]=(c1[2]+c2[2]+c3[2])/3;	a.c[3]=(c1[3]+c2[3]+c3[3])/3;
	add_light(a.c, d2[2]*d1[1]-d2[1]*d1[2], d2[0]*d1[2]-d2[2]*d1[0], d2[1]*d1[0]-d2[0]*d1[1]);
	add_prim(&a);
}
//-----------------------------------------------------------------------------
void mglGraphPS::trig_plot_n(float *p3,float *p1,float *p2,
				float *c3,float *c1,float *c2,float *n3,float *n1,float *n2)
{
	bool pnt = fabs(p1[0]-p2[0])<0.01 && fabs(p1[1]-p2[1])<0.01 &&
				fabs(p1[0]-p3[0])<0.01 && fabs(p1[1]-p3[1])<0.01;
	if(pnt)	return;
	mglPrim a(2);
	a.z = (p1[2]+p2[2]+p3[2])/3;
	a.x[0]=p1[0];	a.y[0]=p1[1];	a.x[1]=p2[0];
	a.y[1]=p2[1];	a.x[2]=p3[0];	a.y[2]=p3[1];
	a.c[0]=(c1[0]+c2[0]+c3[0])/3;	a.c[1]=(c1[1]+c2[1]+c3[1])/3;
	a.c[2]=(c1[2]+c2[2]+c3[2])/3;	a.c[3]=(c1[3]+c2[3]+c3[3])/3;
	add_light(a.c, (n1[0]+n2[0]+n3[0])/3, (n1[1]+n2[1]+n3[1])/3, (n1[2]+n2[2]+n3[2])/3);
	add_prim(&a);
}
//-----------------------------------------------------------------------------
void mglGraphPS::quad_plot(float *p0,float *p1,float *p2,float *p3,
				float *c0,float *c1,float *c2,float *c3)
{
	bool pnt = fabs(p1[0]-p2[0])<0.01 && fabs(p1[1]-p2[1])<0.01 &&
				fabs(p1[0]-p3[0])<0.01 && fabs(p1[1]-p3[1])<0.01 &&
				fabs(p1[0]-p0[0])<0.01 && fabs(p1[1]-p0[1])<0.01;
	if(pnt)	return;
	mglPrim a(3);
	a.z = (p1[2]+p2[2]+p3[2]+p0[2])/4;
	a.x[0]=p0[0];	a.y[0]=p0[1];	a.x[1]=p1[0];	a.y[1]=p1[1];
	a.x[2]=p2[0];	a.y[2]=p2[1];	a.x[3]=p3[0];	a.y[3]=p3[1];
	float d1[3];
	d1[0]=-p0[2]*p1[1]+p0[1]*p1[2]+p0[2]*p2[1]-p0[1]*p2[2]-p1[2]*p3[1]+
			p2[2]*p3[1]+p1[1]*p3[2]-p2[1]*p3[2];
	d1[1]= p0[2]*p1[0]-p0[0]*p1[2]-p0[2]*p2[0]+p0[0]*p2[2]+
			p1[2]*p3[0]-p2[2]*p3[0]-p1[0]*p3[2]+p2[0]*p3[2];
	d1[2]=-p0[1]*p1[0]+p0[0]*p1[1]+p0[1]*p2[0]-p0[0]*p2[1]-p1[1]*p3[0]+
			p2[1]*p3[0]+p1[0]*p3[1]-p2[0]*p3[1];
	a.c[0]=(c1[0]+c2[0]+c3[0]+c0[0])/4;	a.c[1]=(c1[1]+c2[1]+c3[1]+c0[1])/4;
	a.c[2]=(c1[2]+c2[2]+c3[2]+c0[2])/4;	a.c[3]=(c1[3]+c2[3]+c3[3]+c0[3])/4;
	add_light(a.c, d1[0], d1[1], d1[2]);
	add_prim(&a);
}
//-----------------------------------------------------------------------------
void mglGraphPS::quad_plot_n(float *p0,float *p1,float *p2,float *p3,
				float *c0,float *c1,float *c2,float *c3,float *n0,float *n1,float *n2,float *n3)
{
	bool pnt = fabs(p1[0]-p2[0])<0.01 && fabs(p1[1]-p2[1])<0.01 &&
				fabs(p1[0]-p3[0])<0.01 && fabs(p1[1]-p3[1])<0.01 &&
				fabs(p1[0]-p0[0])<0.01 && fabs(p1[1]-p0[1])<0.01;
	if(pnt)	return;
	mglPrim a(3);
	a.z = (p1[2]+p2[2]+p3[2]+p0[2])/4;
	a.x[0]=p0[0];	a.y[0]=p0[1];	a.x[1]=p1[0];	a.y[1]=p1[1];
	a.x[2]=p2[0];	a.y[2]=p2[1];	a.x[3]=p3[0];	a.y[3]=p3[1];
	a.c[0]=(c1[0]+c2[0]+c3[0]+c0[0])/4;	a.c[1]=(c1[1]+c2[1]+c3[1]+c0[1])/4;
	a.c[2]=(c1[2]+c2[2]+c3[2]+c0[2])/4;	a.c[3]=(c1[3]+c2[3]+c3[3]+c0[3])/4;
	add_light(a.c, n1[0]+n2[0]+n3[0]+n0[0], n1[1]+n2[1]+n3[1]+n0[1], n1[2]+n2[2]+n3[2]+n0[2]);
	add_prim(&a);
}
//-----------------------------------------------------------------------------
void mglGraphPS::quad_plot_a(float *p0,float *p1,float *p2,float *p3,
				float a0,float a1,float a2,float a3, float alpha)
{
	bool pnt = fabs(p1[0]-p2[0])<0.01 && fabs(p1[1]-p2[1])<0.01 &&
				fabs(p1[0]-p3[0])<0.01 && fabs(p1[1]-p3[1])<0.01 &&
				fabs(p1[0]-p0[0])<0.01 && fabs(p1[1]-p0[1])<0.01;
	if(pnt)	return;
	mglPrim a(3);
	a.z = (p1[2]+p2[2]+p3[2]+p0[2])/4;
	a.x[0]=p0[0];	a.y[0]=p0[1];	a.x[1]=p1[0];	a.y[1]=p1[1];
	a.x[2]=p2[0];	a.y[2]=p2[1];	a.x[3]=p3[0];	a.y[3]=p3[1];
	float v = (a0+a1+a2+a3)/4;
	mglColor c(GetC(v,false));
	a.c[0]=c.r;		a.c[1]=c.g;		a.c[2]=c.b;
	a.c[3]=alpha>0 ? alpha*(v+1)*(v+1) : -alpha*(v-1)*(v-1);
	add_prim(&a);
}
//-----------------------------------------------------------------------------
void mglGraphPS::Clf(mglColor  Back)
{
	Fog(0);
	pNum=0;
	pPos = CurrPal = 0;		PDef = 0xffff;
	memset(G,0,3*Width*Height);
	memset(G4,0,4*Width*Height);
	if(Back.Valid())
	{
		BDef[0] = (unsigned char)(255*Back.r);
		BDef[1] = (unsigned char)(255*Back.g);
		BDef[2] = (unsigned char)(255*Back.b);
		if(TranspType==2)	Back = mglColor(0,0,0);
/*		mglPrim a(3);	a.z = -1e7;
		a.x[0]=0;		a.y[0]=0;	a.x[1]=0;		a.y[1]=Height;
		a.x[2]=Width;	a.y[2]=0;	a.x[3]=Width;	a.y[3]=Height;
		a.c[0]=Back.r;	a.c[1]=Back.g;	a.c[2]=Back.b;
		add_prim(&a);*/
	}
}
//-----------------------------------------------------------------------------
void mglGraphPS::add_prim(mglPrim *a)
{
	if(!P)
	{
		pMax = 1000;
		P = (mglPrim *)malloc(pMax*sizeof(mglPrim));
	}
	else if(pNum+1>pMax)
	{
		pMax += 1000;
		P = (mglPrim *)realloc(P, pMax*sizeof(mglPrim));
	}
	// fog
	float zz = FogDist*(a->z/Depth-0.5-FogDz);
	if(zz<0)
	{
		float d = 1-exp(5*zz);
		if(255*d>=254)	return;	// not visible under fog
		float cb[3] = {BDef[0]/255., BDef[1]/255., BDef[2]/255.}, b = 1-d;
		a->c[0] = a->c[0]*b + cb[0]*d;	a->c[1] = a->c[1]*b + cb[1]*d;
		a->c[2] = a->c[2]*b + cb[2]*d;	a->c[3] = a->c[3]*b + d;
	}

	a->c[0] = int(a->c[0]*100)*0.01;	a->c[1] = int(a->c[1]*100)*0.01;
	a->c[2] = int(a->c[2]*100)*0.01;	a->c[3] = UseAlpha ? int(a->c[3]*500)*0.002 : 1;
//	if(a->c[3]<0)	a->c[3] = 0;		if(a->c[3]>1)	a->c[3] = 1;
	a->x[0] = int(a->x[0]*100)*0.01;	a->x[1] = int(a->x[1]*100)*0.01;
	a->x[2] = int(a->x[2]*100)*0.01;	a->x[3] = int(a->x[3]*100)*0.01;
	a->y[0] = int(a->y[0]*100)*0.01;	a->y[1] = int(a->y[1]*100)*0.01;
	a->y[2] = int(a->y[2]*100)*0.01;	a->y[3] = int(a->y[3]*100)*0.01;
	memcpy(P+pNum,a,sizeof(mglPrim));
	pNum++;		Finished = false;
}
//-----------------------------------------------------------------------------
void mglGraphPS::add_light(float *b, float n0,float n1, float n2)
{
	if(UseLight && (n0*n0+n1*n1+n2*n2)!=0)
	{
		float d0,d1,d2,nn;
		register long i,j;

		b[0]*=AmbBr;	b[1]*=AmbBr;	b[2]*=AmbBr;
		for(i=0;i<10;i++)
		{
			if(!nLight[i])	continue;
			j = 3*i;
			nn = 2*(n0*pLight[j]+n1*pLight[j+1]+n2*pLight[j+2]) /
					(n0*n0+n1*n1+n2*n2);
			d0 = pLight[j] - n0*nn;
			d1 = pLight[j+1]-n1*nn;
			d2 = pLight[j+2]-n2*nn;
			nn = 1 + d2/sqrt(d0*d0+d1*d1+d2*d2);

			nn = exp(-aLight[i]*nn)*bLight[i]*2;
			b[0] += nn*cLight[j];
			b[1] += nn*cLight[j+1];
			b[2] += nn*cLight[j+2];
		}
		b[0] = b[0]<1 ? b[0] : 1;
		b[1] = b[1]<1 ? b[1] : 1;
		b[2] = b[2]<1 ? b[2] : 1;
	}
}
//-----------------------------------------------------------------------------
char *mgl_get_dash(unsigned short d, float w)
{
	static char s[64],b[4];
	if(d==0xffff)	{	strcpy(s,"");	return s;	}
	int f=0, p=d&1, n=p?0:1, i, j;
	strcpy(s, p ? "" : "0");
	for(i=0;i<16;i++)
	{
		j = i;//15-i;
		if(((d>>j)&1) == p)	f++;
		else
		{
			sprintf(b," %g",f*w);	strcat(s,b);
			p = (d>>j)&1;	f = 1;	n++;
		}
	}
	sprintf(b," %g",f*w);	strcat(s,b);
	strcat(s,n%2 ? "" : " 0");
	return s;
}
//-----------------------------------------------------------------------------
void mglGraphPS::WriteEPS(const char *fname,const char *descr)
{
	if(!P)	return;
	if(!Finished)	Finish();
	time_t now;
	time(&now);

	FILE *fp = fopen(fname,"wt");
	if(!fp)		{	SetWarn(mglWarnOpen,fname);	return;	}
	fprintf(fp,"%%!PS-Adobe-3.0 EPSF-3.0\n%%%%BoundingBox: 0 0 %d %d\n",Width,Height);
	fprintf(fp,"%%%%Creator: MathGL library\n%%%%Title: %s\n",descr ? descr : fname);
	fprintf(fp,"%%%%CreationDate: %s\n",ctime(&now));
	fprintf(fp,"/lw {setlinewidth} def\n");
	fprintf(fp,"/rgb {setrgbcolor} def\n");
	fprintf(fp,"/np {newpath} def\n");
	fprintf(fp,"/cp {closepath} def\n");
	fprintf(fp,"/ll {lineto} def\n");
	fprintf(fp,"/mt {moveto} def\n");
	fprintf(fp,"/rl {rlineto} def\n");
	fprintf(fp,"/rm {rmoveto} def\n");
	fprintf(fp,"/dr {stroke} def\n");
	fprintf(fp,"/ss {%g} def\n",MarkSize*0.4*font_factor);// remove *font_factor); ???
	fprintf(fp,"/s2 {%g} def\n",MarkSize*0.8*font_factor);// remove *font_factor); ???
	fprintf(fp,"/sm {-%g} def\n",MarkSize*0.4*font_factor);//remove *font_factor); ???
	fprintf(fp,"/m_c {ss 0.3 mul 0 360 arc} def\n");
	fprintf(fp,"/d0 {[] 0 setdash} def\n");
	fprintf(fp,"/sd {0 setdash} def\n");
//	fprintf(fp,"/d1 {[8 8] 0 setdash} def\n");
//	fprintf(fp,"/d2 {[4 4] 0 setdash} def\n");
//	fprintf(fp,"/d3 {[1 3] 0 setdash} def\n");
//	fprintf(fp,"/d4 {[7 4 1 4] 0 setdash} def\n");
//	fprintf(fp,"/d5 {[3 2 1 2] 0 setdash} def\n");

	bool m_p=false,m_x=false,m_d=false,m_v=false,m_t=false,
		m_s=false,m_a=false,m_o=false,m_O=false,m_T=false,m_V=false,m_S=false,m_D=false;
	register long i,j,k;
	for(i=0;i<pNum;i++)
	{
		if(P[i].type>0)	continue;
		if(P[i].m=='+')	m_p = true;
		if(P[i].m=='x')	m_x = true;
		if(P[i].m=='s')	m_s = true;
		if(P[i].m=='d')	m_d = true;
		if(P[i].m=='v')	m_v = true;
		if(P[i].m=='^')	m_t = true;
		if(P[i].m=='*')	m_a = true;
		if(P[i].m=='o')	m_o = true;
		if(P[i].m=='O')	m_O = true;
		if(P[i].m=='S')	m_S = true;
		if(P[i].m=='D')	m_D = true;
		if(P[i].m=='V')	m_V = true;
		if(P[i].m=='T')	m_T = true;
	}
	if(m_p)	fprintf(fp,"/m_p {sm 0 rm s2 0 rl sm sm rm 0 s2 rl d0} def\n");
	if(m_x)	fprintf(fp,"/m_x {sm sm rm s2 s2 rl 0 sm 2 mul rm sm 2 mul s2 rl d0} def\n");
	if(m_s)	fprintf(fp,"/m_s {sm sm rm 0 s2 rl s2 0 rl 0 sm 2 mul rl cp d0} def\n");
	if(m_d)	fprintf(fp,"/m_d {sm 0 rm ss ss rl ss sm rl sm sm rl cp d0} def\n");
	if(m_v)	fprintf(fp,"/m_v {sm ss 2 div rm s2 0 rl sm sm 1.5 mul rl d0 cp} def\n");
	if(m_t)	fprintf(fp,"/m_t {sm sm 2 div rm s2 0 rl sm ss 1.5 mul rl d0 cp} def\n");
	if(m_a)	fprintf(fp,"/m_a {sm 0 rm s2 0 rl sm 1.6 mul sm 0.8 mul rm ss 1.2 mul ss 1.6 mul rl 0 sm 1.6 mul rm sm 1.2 mul ss 1.6 mul rl d0} def\n");
	if(m_o)	fprintf(fp,"/m_o {ss 0 360 d0 arc} def\n");
	if(m_O)	fprintf(fp,"/m_O {ss 0 360 d0 arc fill} def\n");
	if(m_S)	fprintf(fp,"/m_S {sm sm rm 0 s2 rl s2 0 rl 0 sm 2 mul rl cp} def\n");
	if(m_D)	fprintf(fp,"/m_D {sm 0 rm ss ss rl ss sm rl sm sm rl cp} def\n");
	if(m_V)	fprintf(fp,"/m_V {sm ss 2 div rm s2 0 rl sm sm 1.5 mul rl cp} def\n");
	if(m_T)	fprintf(fp,"/m_T {sm sm 2 div rm s2 0 rl sm ss 1.5 mul rl cp} def\n");
	fprintf(fp,"\n");

	float cp[3]={-1,-1,-1},wp=-1;
	int st=0;
	char str[256]="";
	for(i=0;i<pNum;i++)
	{
		bool same = fabs(cp[0]-P[i].c[0])<0.01 && fabs(cp[1]-P[i].c[1])<0.01 && fabs(cp[2]-P[i].c[2])<0.01;
		bool wame = (P[i].type==1 && wp!=P[i].w) || (P[i].type==0 && wp!=1);
//		bool pame = (P[i].type==1 && P[i].style!=st);
		str[0] = 0;
		if(!same && wame)
			sprintf(str,"%.2g lw %g %g %g rgb ",(P[i].type==1 && P[i].w>1) ? P[i].w:1.,
						P[i].c[0],P[i].c[1],P[i].c[2]);
		else if(!same)	sprintf(str,"%g %g %g rgb ",P[i].c[0],P[i].c[1],P[i].c[2]);
		else if(wame)	sprintf(str,"%.2g lw ",(P[i].type==1 && P[i].w>1)  ? P[i].w:1.);
//		if(pame)	{	char s[4]="d0 ";	s[1]='0'+P[i].style;	strcat(str,s);	}

		if(P[i].type==0)
		{
			if(P[i].s!=MarkSize)
			{
				fprintf(fp,"/ss {%g} def\n",P[i].s*0.4*font_factor);
				fprintf(fp,"/s2 {%g} def\n",P[i].s*0.8*font_factor);
				fprintf(fp,"/sm {-%g} def\n",P[i].s*0.4*font_factor);
			}
			switch(P[i].m)
			{
			case '+':	fprintf(fp,"np %g %g mt m_p %sdr\n",P[i].x[0],P[i].y[0],str);	break;
			case 'x':	fprintf(fp,"np %g %g mt m_x %sdr\n",P[i].x[0],P[i].y[0],str);	break;
			case 's':	fprintf(fp,"np %g %g mt m_s %sdr\n",P[i].x[0],P[i].y[0],str);	break;
			case 'd':	fprintf(fp,"np %g %g mt m_d %sdr\n",P[i].x[0],P[i].y[0],str);	break;
			case '*':	fprintf(fp,"np %g %g mt m_a %sdr\n",P[i].x[0],P[i].y[0],str);	break;
			case 'v':	fprintf(fp,"np %g %g mt m_v %sdr\n",P[i].x[0],P[i].y[0],str);	break;
			case '^':	fprintf(fp,"np %g %g mt m_t %sdr\n",P[i].x[0],P[i].y[0],str);	break;
			case 'S':	fprintf(fp,"np %g %g mt m_S %sfill\n",P[i].x[0],P[i].y[0],str);	break;
			case 'D':	fprintf(fp,"np %g %g mt m_D %sfill\n",P[i].x[0],P[i].y[0],str);	break;
			case 'V':	fprintf(fp,"np %g %g mt m_V %sfill\n",P[i].x[0],P[i].y[0],str);	break;
			case 'T':	fprintf(fp,"np %g %g mt m_T %sfill\n",P[i].x[0],P[i].y[0],str);	break;
			case 'o':	fprintf(fp,"%g %g m_o %sdr\n",P[i].x[0],P[i].y[0],str);break;
			case 'O':	fprintf(fp,"%g %g m_O %sdr\n",P[i].x[0],P[i].y[0],str);break;
			default:	fprintf(fp,"%g %g m_c %sfill\n",P[i].x[0],P[i].y[0],str);
			}
			if(P[i].s!=MarkSize)
			{
				fprintf(fp,"/ss {%g} def\n",MarkSize*0.4*font_factor);
				fprintf(fp,"/s2 {%g} def\n",MarkSize*0.8*font_factor);
				fprintf(fp,"/sm {-%g} def\n",MarkSize*0.4*font_factor);
			}
		}
		else if(P[i].type==2)
			fprintf(fp,"np %g %g mt %g %g ll %g %g ll cp %sfill\n",P[i].x[0],P[i].y[0],P[i].x[1],P[i].y[1],P[i].x[2],P[i].y[2],str);
		else if(P[i].type==3)
			fprintf(fp,"np %g %g mt %g %g ll %g %g ll %g %g ll cp %sfill\n",P[i].x[0],P[i].y[0],P[i].x[1],P[i].y[1],P[i].x[3],P[i].y[3],P[i].x[2],P[i].y[2],str);
		else if(P[i].type==1)
		{
			k = i;	// ищем цепочки
			fprintf(fp,"np %g %g mt %g %g ll ",P[k].x[0],P[k].y[0],P[k].x[1],P[k].y[1]);
			long m=1;
			for(j=i;j<pNum && P[j].IsSame(wp,cp,st);j++)
			{
				if(P[j].x[0]==P[k].x[1] && P[j].y[0]==P[k].y[1])
				{
					fprintf(fp,"%g %g ll ",P[j].x[1],P[j].y[1]);
					k=j;	P[k].type = -1;		m++;
					if(m>10)	{	m=0;	fprintf(fp,"\n");	}
				}
			}
			const char *sd = mgl_get_dash(P[i].dash,P[i].w);
			if(sd && sd[0])
				fprintf(fp,"%s [%s] sd dr\n",str,sd);
			else
				fprintf(fp,"%s d0 dr\n",str);
		}
		memcpy(cp,P[i].c,3*sizeof(float));
		if(P[i].type==0)	wp = 1;
		if(P[i].type==1)	wp = P[i].w>1  ? P[i].w:1;
		if(P[i].type==1)	st = P[i].style;
	}
	for(i=0;i<pNum;i++)		if(P[i].type==-1)	P[i].type = 1;
	fprintf(fp,"\nshowpage\n%%%%EOF\n");
	fclose(fp);
}
//-----------------------------------------------------------------------------
bool mglPrim::IsSame(float wp,float *cp,int st)
{
	if(abs(type)!=1)	return false;
	if(w>=1 && wp!=w)	return false;
	if(w<1 && wp!=1)	return false;
	if(st!=style)		return false;
	return (cp[0]==c[0] && cp[1]==c[1] && cp[2]==c[2]);
}
//-----------------------------------------------------------------------------
void mglPrim::SetStyle(unsigned PDef, int pPos)
{
	style=0;
	switch(PDef)
	{
	case 0xff00:	style=1;	break;
	case 0xf0f0:	style=2;	break;
	case 0x8888:	style=3;	break;
	case 0xfe10:	style=4;	break;
	case 0xe4e4:	style=5;	break;
	}
	pPos = abs(pPos)%16;
	dash = (PDef>>(16-pPos))+(PDef<<pPos);
}
//-----------------------------------------------------------------------------
void mglGraphPS::WriteSVG(const char *fname,const char *descr)
{
	if(!P)	return;
	if(!Finished)	Finish();
	time_t now;
	time(&now);

	FILE *fp = fopen(fname,"wt");
	if(!fp)		{	SetWarn(mglWarnOpen,fname);	return;	}
	fprintf(fp,"<?xml version=\"1.0\" standalone=\"no\"?>\n");
	fprintf(fp,"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 20000303 Stylable//EN\" \"http://www.w3.org/TR/2000/03/WD-SVG-20000303/DTD/svg-20000303-stylable.dtd\">\n");
	fprintf(fp,"<svg width=\"%d\" height=\"%d\">\n",Width,Height);

	fprintf(fp,"<!--Creator: MathGL library-->\n");
	fprintf(fp,"<!--Title: %s-->\n<!--CreationDate: %s-->\n\n",descr?descr:fname,ctime(&now));
	fprintf(fp,"<g fill=\"none\" stroke=\"none\" stroke-width=\"0.5\">\n");

	float cp[3]={-1,-1,-1},wp=-1;
	register long i,j,k;
	int st=0;
	for(i=0;i<pNum;i++)
	{
		if(P[i].type==0)
		{
			float x=P[i].x[0],y=Height-P[i].y[0],s=0.4*font_factor*P[i].s;
			if(strchr("SDVT",P[i].m))
				fprintf(fp,"<g fill=\"#%02x%02x%02x\">\n",
					int(255*P[i].c[0]),int(255*P[i].c[1]),int(255*P[i].c[2]));
			else
				fprintf(fp,"<g stroke=\"#%02x%02x%02x\">\n",
					int(255*P[i].c[0]),int(255*P[i].c[1]),int(255*P[i].c[2]));
			switch(P[i].m)
			{
			case '+':
				fprintf(fp,"<path d=\"M %g %g L %g %g M %g %g L %g %g\"/>\n",
						x-s,y,x+s,y,x,y-s,x,y+s);	break;
			case 'x':
				fprintf(fp,"<path d=\"M %g %g L %g %g M %g %g L %g %g\"/>\n",
						x-s,y-s,x+s,y+s,x+s,y-s,x-s,y+s);	break;
			case 's':
				fprintf(fp,"<path d=\"M %g %g L %g %g L %g %g L %g %gZ\"/>\n",
						x-s,y-s,x+s,y-s,x+s,y+s,x-s,y+s);	break;
			case 'd':
				fprintf(fp,"<path d=\"M %g %g L %g %g L %g %g L %g %gZ\"/>\n",
						x-s,y,x,y-s,x+s,y,x,y+s);	break;
			case '^':
				fprintf(fp,"<path d=\"M %g %g L %g %g L %g %gZ\"/>\n",
						x-s,y+s/2,x+s,y+s/2,x,y-s);	break;
			case 'v':
				fprintf(fp,"<path d=\"M %g %g L %g %g L %g %gZ\"/>\n",
						x-s,y-s/2,x+s,y-s/2,x,y+s);	break;
			case 'S':
				fprintf(fp,"<path d=\"M %g %g L %g %g L %g %g L %g %gZ\"/>\n",
						x-s,y-s,x+s,y-s,x+s,y+s,x-s,y+s);	break;
			case 'D':
				fprintf(fp,"<path d=\"M %g %g L %g %g L %g %g L %g %gZ\"/>\n",
						x-s,y,x,y-s,x+s,y,x,y+s);	break;
			case 'T':
				fprintf(fp,"<path d=\"M %g %g L %g %g L %g %gZ\"/>\n",
						x-s,y+s/2,x+s,y+s/2,x,y-s);	break;
			case 'V':
				fprintf(fp,"<path d=\"M %g %g L %g %g L %g %gZ\"/>\n",
						x-s,y-s/2,x+s,y-s/2,x,y+s);	break;
			case 'o':
				fprintf(fp,"<circle cx=\"%g\" cy=\"%g\" r=\"%g\"/>\n",
						x,y,s);	break;
			case 'O':
				fprintf(fp,"<circle style=\"fill:#%02x%02x%02x\" cx=\"%g\" cy=\"%g\" r=\"%g\"/>\n",
						int(255*P[i].c[0]),int(255*P[i].c[1]),int(255*P[i].c[2]),x,y,s);	break;
			case '*':
				fprintf(fp,"<path d=\"M %g %g L %g %g M %g %g L %g %g M %g %g L %g %g\"/>\n",
						x-s,y,x+s,y,x-0.8*s,y-1.6*s,x+0.8*s,y+1.6*s,x+0.8*s,y-1.6*s,x-0.8*s,y+1.6*s);	break;
			default:
				fprintf(fp,"<circle style=\"fill:#%02x%02x%02x\" cx=\"%g\" cy=\"%g\" r=\"0.15\"/>\n",
						int(255*P[i].c[0]),int(255*P[i].c[1]),int(255*P[i].c[2]),x,y);	break;
			}
			fprintf(fp,"</g>\n");
		}
		else if(P[i].type==2 && P[i].c[3]>0)
		{
			fprintf(fp,"<g fill=\"#%02x%02x%02x\" opacity=\"%g\">\n",
				int(255*P[i].c[0]),int(255*P[i].c[1]),int(255*P[i].c[2]),P[i].c[3]);
			fprintf(fp,"<path d=\"M %g %g L %g %g L %g %g Z\"/> </g>\n",
					P[i].x[0],Height-P[i].y[0],P[i].x[1],Height-P[i].y[1],
					P[i].x[2],Height-P[i].y[2]);
		}
		else if(P[i].type==3 && P[i].c[3]>0)
		{
			fprintf(fp,"<g fill=\"#%02x%02x%02x\" opacity=\"%g\">\n",
				int(255*P[i].c[0]),int(255*P[i].c[1]),int(255*P[i].c[2]),P[i].c[3]);
			fprintf(fp,"<path d=\"M %g %g L %g %g L %g %g L %g %g Z\"/> </g>\n",
					P[i].x[0],Height-P[i].y[0],P[i].x[1],Height-P[i].y[1],
					P[i].x[3],Height-P[i].y[3],P[i].x[2],Height-P[i].y[2]);
		}
		else if(P[i].type==1)
		{
//			const char *dash[]={"", "8 8","4 4","1 3","7 4 1 4","3 2 1 2"};
			fprintf(fp,"<g stroke=\"#%02x%02x%02x\"",
					int(255*P[i].c[0]),int(255*P[i].c[1]),int(255*P[i].c[2]));
			if(P[i].style)	fprintf(fp," stroke-dasharray=\"%s\"", mgl_get_dash(P[i].dash,P[i].w));
//			if(P[i].style)	fprintf(fp," stroke-dasharray=\"%s\"", dash[P[i].style]);
			if(P[i].w>1)	fprintf(fp," stroke-width=\"%g\"", P[i].w);

			fprintf(fp,"><path d=\" M %g %g L %g %g",P[i].x[0],Height-P[i].y[0],P[i].x[1],Height-P[i].y[1]);
			k = i;	// ищем цепочки
			long m=1;
			for(j=i+1;j<pNum && P[j].IsSame(wp,cp,st);j++)
			{
				if(P[j].x[0]==P[k].x[1] && P[j].y[0]==P[k].y[1])
				{
					fprintf(fp," L %g %g",P[j].x[1],Height-P[j].y[1]);
					k=j;	P[k].type = -1;		m++;
					if(m>10)	{	m=0;	fprintf(fp,"\n");	}
				}
			}
			fprintf(fp,"\"/> </g>\n");
		}
		memcpy(cp,P[i].c,3*sizeof(float));
		if(P[i].type==0)	wp = 1;
		if(P[i].type==1)	wp = P[i].w>1  ? P[i].w:1;
		if(P[i].type==1)	st = P[i].style;
	}

	for(i=0;i<pNum;i++)		if(P[i].type==-1)	P[i].type = 1;
	fprintf(fp,"</g></svg>");
	fclose(fp);
}
//-----------------------------------------------------------------------------
void mglGraphPS::Finish()
{
	register long i;
	if(P && pNum>0)
	{
		qsort(P,pNum,sizeof(mglPrim),mgl_compare_prim);
		for(i=0;i<pNum;i++)	P[i].Draw(this);
	}
	unsigned char w[4];
	for(i=0;i<Width*Height;i++)
	{
		w[0]=w[1]=w[2]=w[3]=255;
		combine(w,G4+4*i);	memcpy(G+3*i,w,3);
	}
	Finished = true;
}
//-----------------------------------------------------------------------------
#define imax(a,b)	(a)>(b) ? (a) : (b)
#define imin(a,b)	(a)<(b) ? (a) : (b)
void mglPrim::Draw(mglGraphPS *gr)
{
	unsigned char r[4]={(unsigned char)(255*c[0]),(unsigned char)(255*c[1]),
				(unsigned char)(255*c[2]),(unsigned char)(255*c[3])};
	long y1,x1,y2,x2;
	float d1[2],d2[2], dxu,dxv,dyu,dyv;
	register float u,v,xx,yy;
	register long i,j,g;

	if(type==0)
		gr->mark_plot(int(x[0]),int(y[0]),m,r);
	else if(type==2 && c[3]>0)
	{
		d1[0] = x[1]-x[0];	d2[0] = x[2]-x[0];
		d1[1] = y[1]-y[0];	d2[1] = y[2]-y[0];
		x1 = imin(imin(long(x[0]),long(x[2])),long(x[1]));	// bounding box
		y1 = imin(imin(long(y[0]),long(y[2])),long(y[1]));
		x2 = imax(imax(long(x[0]),long(x[2])),long(x[1]));
		y2 = imax(imax(long(y[0]),long(y[2])),long(y[1]));

		dxu = d2[0]*d1[1] - d1[0]*d2[1];
		if(fabs(dxu)<1e-5)	return;		// points lies on the same line
		dyv =-d1[0]/dxu;	dxv = d1[1]/dxu;
		dyu = d2[0]/dxu;	dxu =-d2[1]/dxu;

		for(i=x1;i<=x2;i++)	for(j=y1;j<=y2;j++)
		{
			xx = (i-x[0]);	yy = (j-y[0]);
			u = dxu*xx+dyu*yy;	v = dxv*xx+dyv*yy;
			g = u<0 || v<0 || u+v>1;
			if(g)	continue;
			gr->pnt_plot(i,j,r);
		}
	}
	else if(type==3 && c[3]>0)
	{
		x1 = imin(imin(long(x[0]),long(x[3])),imin(long(x[1]),long(x[2])));	// bounding box
		y1 = imin(imin(long(y[0]),long(y[3])),imin(long(y[1]),long(y[2])));
		x2 = imax(imax(long(x[0]),long(x[3])),imax(long(x[1]),long(x[2])));
		y2 = imax(imax(long(y[0]),long(y[3])),imax(long(y[1]),long(y[2])));

		d1[0] = x[1]-x[0];	d2[0] = x[2]-x[0];
		d1[1] = y[1]-y[0];	d2[1] = y[2]-y[0];
		dxu = d2[0]*d1[1] - d1[0]*d2[1];
		if(fabs(dxu)<1e-5)	return;		// points lies on the same line
		dyv =-d1[0]/dxu;	dxv = d1[1]/dxu;
		dyu = d2[0]/dxu;	dxu =-d2[1]/dxu;

		for(i=x1;i<=x2;i++)	for(j=y1;j<=y2;j++)
		{
			xx = (i-x[0]);	yy = (j-y[0]);
			u = dxu*xx+dyu*yy;	v = dxv*xx+dyv*yy;
			g = u<0 || v<0 || u+v>1;
			if(g)	continue;
			gr->pnt_plot(i,j,r);
		}

		d1[0] = x[1]-x[3];	d2[0] = x[2]-x[3];
		d1[1] = y[1]-y[3];	d2[1] = y[2]-y[3];
		dxu = d2[0]*d1[1] - d1[0]*d2[1];
		if(fabs(dxu)<1e-5)	return;		// points lies on the same line
		dyv =-d1[0]/dxu;	dxv = d1[1]/dxu;
		dyu = d2[0]/dxu;	dxu =-d2[1]/dxu;

		for(i=x1;i<=x2;i++)	for(j=y1;j<=y2;j++)
		{
			xx = (i-x[3]);	yy = (j-y[3]);
			u = dxu*xx+dyu*yy;	v = dxv*xx+dyv*yy;
			g = u<0 || v<0 || u+v>1;
			if(g)	continue;
			gr->pnt_plot(i,j,r);
		}
	}
	else if(type==1)
	{
		float b=(w<1?1:w), dd;

		d1[0] = x[1]-x[0];	d1[1] = y[1]-y[0];
		dd = sqrt(d1[0]*d1[0] + d1[1]*d1[1]);
		if(dd<1e-5)	return;		// points lies on the vertical line

		dxv = d1[1]/dd;	dyv =-d1[0]/dd;
		dxu = d1[0]/dd;	dyu = d1[1]/dd;

		x1 = imin(long(x[0]),long(x[1]));	// bounding box
		y1 = imin(long(y[0]),long(y[1]));
		x2 = imax(long(x[0]),long(x[1]));
		y2 = imax(long(y[0]),long(y[1]));
		x1 -= int(w+3.5);	y1 -= int(w+3.5);
		x2 += int(w+3.5);	y2 += int(w+3.5);

		for(i=x1;i<=x2;i++)	for(j=y1;j<=y2;j++)
		{
			xx = (i-x[0]);	yy = (j-y[0]);
			u = dxu*xx+dyu*yy;	v = fabs(dxv*xx+dyv*yy);
			if(u<0)		{	v = hypot(u,v);		u = 0;	}
			if(u>dd)	{	v = hypot(u-dd,v);	u = dd;	}
			if(v>b)		continue;
			bool tt = dash & (1<<long(fmod(u/w, 16)));
			if(!tt)		continue;
			u /= dd;
			r[3] = (unsigned char)(255*exp(-6*ipow_mgl(v/b,2)));
			gr->pnt_plot(i,j,r);
		}
	}
}
//-----------------------------------------------------------------------------
void mglGraphPS::pnt_plot(int x,int y,unsigned char c[4])
{
	long i0=x+Width*(Height-1-y);
	if(x<0 || x>=Width || y<0 || y>=Height)	return;
	combine(G4+4*i0,c);
}
//-----------------------------------------------------------------------------
void mglGraphPS::mark_plot(int x,int y, char type, unsigned char cs[4])
{
//	unsigned char cs[4]={(unsigned char)(255*cs[0]), (unsigned char)(255*cs[1]),
//						(unsigned char)(255*cs[2]), (unsigned char)(255*cs[3])};
	float v;
	register long i,j,ss = long(MarkSize*0.35*font_factor);
	if(type=='.' || ss==0)
	{
		ss = long(3.5+fabs(PenWidth));
		for(i=-ss;i<=ss;i++)	for(j=-ss;j<=ss;j++)
		{
			v = hypot(i,j)/fabs(PenWidth)/3;
			cs[3] = (unsigned char)(cs[3]*exp(-6*v*v));
			if(cs[3]==0)	continue;
			pnt_plot(x+i,y+j,cs);
		}
	}
	else
	{
		switch(type)
		{
		case '+':
			for(i=-ss;i<=ss;i++)
			{
				pnt_plot(x+i, y,cs);
				pnt_plot(x, y+i,cs);
			}
			break;
		case 'x':
			for(i=-ss;i<=ss;i++)
			{
				pnt_plot(x+i, y-i,cs);
				pnt_plot(x+i, y+i,cs);
			}
			break;
		case 's':
//			ss = int(ss*0.9);
			for(i=-ss;i<=ss;i++)
			{
				pnt_plot(x+i, y-ss,cs);
				pnt_plot(x+i, y+ss,cs);
				pnt_plot(x+ss, y-i,cs);
				pnt_plot(x-ss, y+i,cs);
			}
			break;
		case 'd':
			ss = int(ss*1.1);
			for(i=0;i<=ss;i++)
			{
				pnt_plot(x+i, y+ss-i,cs);
				pnt_plot(x-i, y+ss-i,cs);
				pnt_plot(x+i, y-ss+i,cs);
				pnt_plot(x-i, y-ss+i,cs);
			}
			break;
		case '*':
			for(i=-ss;i<=ss;i++)
			{
				pnt_plot(x+i, y,cs);
				pnt_plot(int(x+i*0.6), int(y+i*0.8),cs);
				pnt_plot(int(x-i*0.6), int(y+i*0.8),cs);
			}
			break;
		case '^':
			ss = int(ss*1.1);
			for(i=-ss;i<=ss;i++)	pnt_plot(x+i, y-ss/2,cs);
			for(i=-(ss/2);i<=ss;i++)
			{
				j = ss-(i+(ss/2))*ss/(ss+(ss/2));
				pnt_plot(x+j, y+i,cs);
				pnt_plot(x-j, y+i,cs);
			}
			break;
		case 'v':
			ss = int(ss*1.1);
			for(i=-ss;i<=ss;i++)	pnt_plot(x+i, y+ss/2,cs);
			for(i=-ss;i<=(ss/2);i++)
			{
				j = (i+ss)*ss/(ss+(ss/2));
				pnt_plot(x+j, y+i,cs);
				pnt_plot(x-j, y+i,cs);
			}
			break;
		case 'o':
			for(i=-4*ss;i<=4*ss;i++)
			{
				pnt_plot(int(x+ss*cos(i*M_PI_4/ss)), int(y+ss*sin(i*M_PI_4/ss)),cs);
			}
			break;
		case 'O':
			for(i=-ss;i<=ss;i++)	for(j=-ss;j<=ss;j++)
			{
				if(i*i+j*j>=ss*ss)	continue;
				pnt_plot(x+i, y+j,cs);
			}
			break;
		case 'S':
			for(i=long(-ss);i<=long(ss);i++)	for(j=long(-ss);j<=long(ss);j++)
				pnt_plot(x+i,y+j,cs);
			break;
		case 'D':
			ss = int(ss*1.1);
			for(i=long(-ss);i<=long(ss);i++)	for(j=long(-ss);j<=long(ss);j++)
				if(abs(i)+abs(j)<=long(ss))
					pnt_plot(x+i,y+j,cs);
			break;
		case 'T':
			ss = int(ss*1.1);
			for(i=long(-ss);i<=long(ss);i++)	for(j=long(-ss/2);j<=long(ss);j++)
				if(1.5*abs(i)+j<=long(ss))
					pnt_plot(x+i,y+j,cs);
			break;
		case 'V':
			ss = int(ss*1.1);
			for(i=long(-ss);i<=long(ss);i++)	for(j=long(-ss);j<=long(ss/2);j++)
				if(1.5*abs(i)-j<=long(ss))
					pnt_plot(x+i,y+j,cs);
			break;
		}
	}
}
//-----------------------------------------------------------------------------
