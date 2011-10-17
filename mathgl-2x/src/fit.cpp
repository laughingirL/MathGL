/***************************************************************************
 * fit.cpp is part of Math Graphic Library
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
#ifndef NO_GSL
#include <gsl/gsl_multifit_nlin.h>
#include <gsl/gsl_blas.h>
#endif
#include <ctype.h>
#include "mgl/fit.h"
#include "mgl/eval.h"
#include "mgl/data.h"
//-----------------------------------------------------------------------------
int mglFitPnts=100;		///< Number of output points in fitting
char mglFitRes[1024];	///< Last fitted formula
//-----------------------------------------------------------------------------
/// Structure for keeping data and precompiled fitted formula
struct mglFitData
{
	long n;				///< number of points
	float *x;			///< x values
	float *y;			///< y values
	float *z;			///< z values
	float *a;			///< function values
	float *s;			///< value dispersions (sigma)
	mglFormula *eq;		///< approximation formula
	int m;				///< number of variables
	const char *var;	///< variables for fitting
};
//-----------------------------------------------------------------------------
#ifndef NO_GSL
int	mgl_fit__f (const gsl_vector *x, void *data, gsl_vector *f)
{
	mglFitData *fd = (mglFitData *)data;
	register long i;
	float val[MGL_VS];
	for(i=0;i<fd->m;i++)	val[fd->var[i]-'a'] = gsl_vector_get(x,i);
	for(i=0;i<fd->n;i++)
	{
		val['x'-'a'] = fd->x[i];
		val['y'-'a'] = fd->y ? fd->y[i] : 0;
		val['z'-'a'] = fd->z ? fd->z[i] : 0;
		gsl_vector_set (f, i, (fd->eq->Calc(val) - fd->a[i])/fd->s[i]);
	}
	return GSL_SUCCESS;
}
//-----------------------------------------------------------------------------
int mgl_fit__df (const gsl_vector * x, void *data, gsl_matrix * J)
{
	mglFitData *fd = (mglFitData *)data;
	register long i,j;
	float val[MGL_VS],s;
	for(i=0;i<fd->m;i++)	val[fd->var[i]-'a'] = gsl_vector_get(x,i);
	for(i=0;i<fd->n;i++)
	{
		val['x'-'a'] = fd->x[i];	s = fd->s[i];
		val['y'-'a'] = fd->y ? fd->y[i] : 0;
		val['z'-'a'] = fd->z ? fd->z[i] : 0;
		for(j=0;j<fd->m;j++)
			gsl_matrix_set (J, i, j, fd->eq->CalcD(val, fd->var[j])/s);
	}
	return GSL_SUCCESS;
}
//-----------------------------------------------------------------------------
int mgl_fit__fdf (const gsl_vector * x, void *data, gsl_vector * f, gsl_matrix * J)
{
	mgl_fit__f(x, data, f);
	mgl_fit__df(x, data, J);
	return GSL_SUCCESS;
}
#endif
//-----------------------------------------------------------------------------
/// GSL based fitting procedure for formula/arguments specified by string
float mgl_fit_base(mglFitData *fd, float *ini)
{
#ifndef NO_GSL
	register long i,m=fd->m,n=fd->n,iter=0;
	if(n<1 || fd==0 || ini==0)	return -1;
	// setup data
	double *x_init = new double[fd->m];
	for(i=0;i<m;i++)	x_init[i] = ini[i];
	// setup fitting
	gsl_matrix *covar = gsl_matrix_alloc(m, m);
	gsl_vector_view vx = gsl_vector_view_array(x_init, m);
	const gsl_multifit_fdfsolver_type *T = gsl_multifit_fdfsolver_lmsder;
	gsl_multifit_fdfsolver *s = gsl_multifit_fdfsolver_alloc(T, n, m);
	gsl_multifit_function_fdf f;
	f.f = mgl_fit__f;		f.df = mgl_fit__df;
	f.fdf = mgl_fit__fdf;	f.n = n;	f.p = m;
	f.params = fd;
	gsl_multifit_fdfsolver_set(s, &f, &vx.vector);
	int status;	// start fitting
	do
	{
		iter++;
		status = gsl_multifit_fdfsolver_iterate(s);
		if ( status )	break;
		status = gsl_multifit_test_delta (s->dx, s->x, 1e-4, 1e-4 );
	}
	while ( status == GSL_CONTINUE && iter < 500 );
	gsl_multifit_covar (s->J, 0.0, covar );
	float res = gsl_blas_dnrm2(s->f);
	for(i=0;i<m;i++)	ini[i] = gsl_vector_get(s->x, i);
	// free memory
	gsl_multifit_fdfsolver_free (s);
	gsl_matrix_free (covar);
	delete []x_init;
	return res;
#else
	return 0.0;
#endif
}
//-----------------------------------------------------------------------------
void mglPrepareFitEq(mglBase *gr,float chi, const char *eq, const char *var, float *par)
{
	char buf[32]="";
	sprintf(mglFitRes,"chi=%g",chi);
	for(int i=0;i<int(strlen(var));i++)
	{
		sprintf(buf,", %c=%g",var[i],par[i]);
		strcat(mglFitRes,buf);
	}
	gr->SetWarn(-1,mglFitRes);

	memset(mglFitRes, 0, 1024);	//mglFitRes[0] = 0;
	register long i,k,len=strlen(eq);
	for(i=k=0;i<len;i++)
	{
		const char *c = strchr(var,eq[i]);
		if(c && (i==0 || !isalnum(eq[i-1])) && (i==len-1 || !isalnum(eq[i+1])))
		{
			sprintf(buf,"%g",par[c-var]);
			strcat(mglFitRes+k, buf);	k+=strlen(buf);
		}
		else	{	mglFitRes[k] = eq[i];	k++;	}
	}
	mglFitRes[k]=0;
}
//-----------------------------------------------------------------------------
HMDT mgl_fit_1(HMGL gr, HCDT y, const char *eq, const char *var, HMDT ini, const char *opt)
{
	gr->SaveState(opt);
	mglData x(y->GetNx());	x.Fill(gr->Min.x, gr->Max.x);
	mglData s(y);		s.Fill(1,1);
	return mgl_fit_xys(gr,&x,y,&s,eq,var,ini,0);
}
//-----------------------------------------------------------------------------
HMDT mgl_fit_2(HMGL gr, HCDT z, const char *eq, const char *var, HMDT ini, const char *opt)
{
	gr->SaveState(opt);
	mglData x(z->GetNx());	x.Fill(gr->Min.x, gr->Max.x);
	mglData y(z->GetNy());	y.Fill(gr->Min.y, gr->Max.y);
	mglData s(z);		s.Fill(1,1);
	return mgl_fit_xyzs(gr,&x,&y,z,&s,eq,var,ini,0);
}
//-----------------------------------------------------------------------------
HMDT mgl_fit_3(HMGL gr, HCDT a, const char *eq, const char *var, HMDT ini, const char *opt)
{
	gr->SaveState(opt);
	mglData x(a->GetNx());	x.Fill(gr->Min.x, gr->Max.x);
	mglData y(a->GetNy());	y.Fill(gr->Min.y, gr->Max.y);
	mglData z(a->GetNz());	z.Fill(gr->Min.z, gr->Max.z);
	mglData s(a);		s.Fill(1,1);
	return mgl_fit_xyzas(gr,&x,&y,&z,a,&s,eq,var,ini,0);
}
//-----------------------------------------------------------------------------
HMDT mgl_fit_xy(HMGL gr, HCDT x, HCDT y, const char *eq, const char *var, HMDT ini, const char *opt)
{
	mglData s(y);	s.Fill(1,1);
	return mgl_fit_xys(gr,x,y,&s,eq,var,ini,opt);
}
//-----------------------------------------------------------------------------
HMDT mgl_fit_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, const char *eq, const char *var, HMDT ini, const char *opt)
{
	mglData s(z);	s.Fill(1,1);
	return mgl_fit_xyzs(gr,x,y,z,&s,eq,var,ini,opt);
}
//-----------------------------------------------------------------------------
HMDT mgl_fit_xyza(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT a, const char *eq, const char *var, HMDT ini, const char *opt)
{
	mglData s(a);	s.Fill(1,1);
	return mgl_fit_xyzas(gr,x,y,z,a,&s,eq,var,ini,opt);
}
//-----------------------------------------------------------------------------
HMDT mgl_fit_ys(HMGL gr, HCDT y, HCDT s, const char *eq, const char *var, HMDT ini, const char *opt)
{
	gr->SaveState(opt);
	mglData x(y->GetNx());	x.Fill(gr->Min.x, gr->Max.x);
	return mgl_fit_xys(gr,&x,y,s,eq,var,ini,0);
}
//-----------------------------------------------------------------------------
HMDT mgl_fit_xys(HMGL gr, HCDT xx, HCDT yy, HCDT ss, const char *eq, const char *var, HMDT ini, const char *opt)
{
	mglData *fit=new mglData;
	long m = yy->GetNx();
	if(xx->GetNx()!=m)	{	gr->SetWarn(mglWarnDim,"Fit[S]");	return fit;	}
	if(m<2)		{	gr->SetWarn(mglWarnLow,"Fit[S]");	return fit;	}
	if(ss->GetNx()*ss->GetNy()*ss->GetNz() != m*yy->GetNy()*yy->GetNz())
	{	gr->SetWarn(mglWarnDim,"FitS");	return fit;	}

	long nn = long(0.5+gr->SaveState(opt));
	if(nn<mglFitPnts)	nn = mglFitPnts;
	mglData x(xx), y(yy), s(ss);
	mglFitData fd;
	fd.n = m;	fd.x = x.a;		fd.y = 0;
	fd.z = 0;	fd.a = y.a;		fd.s = s.a;
	fd.var = var;	fd.m = strlen(var);
	fd.eq = new mglFormula(eq);
	mglData in(fd.m);
	fit->Create(nn, yy->GetNy(), yy->GetNz());
	float val[MGL_VS],res=-1;
	register long j;
	for(long i=0;i<yy->GetNy()*yy->GetNz();i++)
	{
		if(ini && ini->nx>=fd.m)	in.Set(ini->a,fd.m);
		else in.Fill(0.,0);
		fd.a = y.a+i*m;		fd.x = x.a+(i%x.ny)*m;
		fd.s = s.a+i*m;
		res = mgl_fit_base(&fd,in.a);
		for(j=0;j<fd.m;j++)	val[var[j]-'a'] = in.a[j];
		for(j=0;j<nn;j++)
		{
			val['x'-'a'] = gr->Min.x+j*(gr->Max.x-gr->Min.x)/(nn-1);
			fit->a[j+i*nn] = fd.eq->Calc(val);
		}
		if(ini && ini->nx>=fd.m)	memcpy(ini->a,in.a,fd.m*sizeof(mreal));
	}
	mglPrepareFitEq(gr,res,eq,var,in.a);
	delete fd.eq;	gr->LoadState();	return fit;
}
//-----------------------------------------------------------------------------
HMDT mgl_fit_xyzs(HMGL gr, HCDT xx, HCDT yy, HCDT zz, HCDT ss, const char *eq, const char *var, HMDT ini, const char *opt)
{
	mglData *fit=new mglData;
	long m=zz->GetNx(),n=zz->GetNy();
	if(xx->GetNx()!=m)		{	gr->SetWarn(mglWarnDim,"Fit[S]");	return fit;	}
	if(ss->GetNx()*ss->GetNy()*ss->GetNz() != m*n*zz->GetNz())
	{	gr->SetWarn(mglWarnDim,"FitS");	return fit;	}
	if(yy->GetNx()!=n && (xx->GetNy()!=n || yy->GetNx()!=m || yy->GetNy()!=n))
	{	gr->SetWarn(mglWarnDim);	return fit;	}
	if(m<2|| n<2)	{	gr->SetWarn(mglWarnLow,"Fit[S]");	return fit;	}

	long nn = long(0.5+gr->SaveState(opt));
	if(nn<mglFitPnts)	nn = mglFitPnts;
	mglData x(m, n), y(m, n), z(zz), s(ss);
	register long i,j;
	for(i=0;i<m;i++)	for(j=0;j<n;j++)	// ñîçäàåì ìàññèâ òî÷åê
	{
		x.a[i+m*j] = GetX(xx,i,j,0).x;
		y.a[i+m*j] = GetY(yy,i,j,0).x;
	}
	mglFitData fd;
	fd.n = m*n;		fd.x = x.a;	fd.y = y.a;
	fd.z = 0;		fd.a = z.a;	fd.s = s.a;
	fd.var = var;	fd.m = strlen(var);
	fd.eq = new mglFormula(eq);
	mglData in(fd.m);
	fit->Create(nn, nn, zz->GetNz());
	float val[MGL_VS], res = -1;
	for(i=0;i<zz->GetNz();i++)
	{
		if(ini && ini->nx>=fd.m)	in.Set(ini->a,fd.m);
		else in.Fill(0.,0);
		fd.a = z.a+i*m*n;		fd.s = s.a+i*m*n;
		res = mgl_fit_base(&fd,in.a);
		for(j=0;j<fd.m;j++)	val[var[j]-'a'] = in.a[j];
		for(j=0;j<nn*nn;j++)
		{
			val['x'-'a'] = gr->Min.x+(j%nn)*(gr->Max.x-gr->Min.x)/(nn-1);
			val['y'-'a'] = gr->Min.y+(j/nn)*(gr->Max.y-gr->Min.y)/(nn-1);
			fit->a[j+i*nn*nn] = fd.eq->Calc(val);
		}
		if(ini && ini->nx>=fd.m)	memcpy(ini->a,in.a,fd.m*sizeof(mreal));
	}
	mglPrepareFitEq(gr,res, eq,var,in.a);
	delete fd.eq;	gr->LoadState();	return fit;
}
//-----------------------------------------------------------------------------
HMDT mgl_fit_xyzas(HMGL gr, HCDT xx, HCDT yy, HCDT zz, HCDT aa, HCDT ss, const char *eq, const char *var, HMDT ini, const char *opt)
{
	mglData *fit=new mglData;
	register long i,j,k,i0;
	long m=aa->GetNx(), n=aa->GetNy(), l=aa->GetNz();
	i = n*m*l;
	if(m<2 || n<2 || l<2)	{	gr->SetWarn(mglWarnLow,"Fit[S]");	return fit;	}
	if(ss->GetNx()*ss->GetNy()*ss->GetNz() != i)		{	gr->SetWarn(mglWarnDim,"FitS");	return fit;	}
	bool both = xx->GetNx()*xx->GetNy()*xx->GetNz()==i && yy->GetNx()*yy->GetNy()*yy->GetNz()==i && zz->GetNx()*zz->GetNy()*zz->GetNz()==i;
	if(!(both || (xx->GetNx()==m && yy->GetNx()==n && zz->GetNx()==l)))
	{	gr->SetWarn(mglWarnDim,"Fit[S]");	return fit;	}

	long nn = long(0.5+gr->SaveState(opt));
	if(nn<mglFitPnts)	nn = mglFitPnts;
	mglData x(aa), y(aa), z(aa), a(aa), s(ss);
	for(i=0;i<m;i++)	for(j=0;j<n;j++)	for(k=0;k<l;k++)	// ñîçäàåì ìàññèâ òî÷åê
	{
		i0 = i+m*(j+n*k);
		x.a[i0] = GetX(xx,i,j,k).x;
		y.a[i0] = GetY(yy,i,j,k).x;
		z.a[i0] = GetZ(zz,i,j,k).x;
	}
	mglFitData fd;
	fd.n = m*n*l;	fd.x = x.a;	fd.y = y.a;
	fd.z = z.a;		fd.a = a.a;	fd.s = s.a;
	fd.var = var;	fd.m = strlen(var);
	fd.eq = new mglFormula(eq);
	mglData in(fd.m);
	fit->Create(nn, nn, nn);
	float val[MGL_VS], res = -1;

	if(ini && ini->nx>=fd.m)	in.Set(ini->a,fd.m);
	else in.Fill(0.,0);
	res = mgl_fit_base(&fd,in.a);
	for(j=0;j<fd.m;j++)	val[var[j]-'a'] = in.a[j];
	for(i=0;i<nn;i++)	for(j=0;j<nn*nn;j++)
	{
		val['x'-'a'] = gr->Min.x+(j%nn)*(gr->Max.x-gr->Min.x)/(nn-1);
		val['y'-'a'] = gr->Min.y+(j/nn)*(gr->Max.y-gr->Min.y)/(nn-1);
		val['z'-'a'] = gr->Min.z+i*(gr->Max.y-gr->Min.y)/(nn-1);
		fit->a[j+nn*nn*i] = fd.eq->Calc(val);
	}
	if(ini && ini->nx>=fd.m)	memcpy(ini->a,in.a,fd.m*sizeof(mreal));

	mglPrepareFitEq(gr,res, eq,var,in.a);
	delete fd.eq;	gr->LoadState();	return fit;
}
//-----------------------------------------------------------------------------
HMDT mgl_hist_x(HMGL gr, HCDT x, HCDT a, const char *opt)
{
	long nn=a->GetNx()*a->GetNy()*a->GetNz();
	if(nn!=x->GetNx()*x->GetNy()*x->GetNz())
	{	gr->SetWarn(mglWarnDim);	return (new mglData);	}
	long n = long(0.5+gr->SaveState(opt));
	if(n<mglFitPnts)	n = mglFitPnts;
	mglData *res = new mglData(n);
	register long i,j1;
	for(i=0;i<nn;i++)
	{
		j1 = long(n*(x->v(i)-gr->Min.x)/(gr->Max.x-gr->Min.x));
		if(j1>=0 && j1<n)	res->a[j1] += a->v(i);
	}
	gr->LoadState();	return res;
}
//-----------------------------------------------------------------------------
HMDT mgl_hist_xy(HMGL gr, HCDT x, HCDT y, HCDT a, const char *opt)
{
	long nn=a->GetNx()*a->GetNy()*a->GetNz();
	if(nn!=x->GetNx()*x->GetNy()*x->GetNz() || nn!=y->GetNx()*y->GetNy()*y->GetNz())
	{	gr->SetWarn(mglWarnDim);	return (new mglData);	}
	long n = long(0.5+gr->SaveState(opt));
	if(n<mglFitPnts)	n = mglFitPnts;
	mglData *res = new mglData(n, n);
	register long i,j1,j2;
	for(i=0;i<nn;i++)
	{
		j1 = long(n*(x->v(i)-gr->Min.x)/(gr->Max.x-gr->Min.x));
		j2 = long(n*(y->v(i)-gr->Min.y)/(gr->Max.y-gr->Min.y));
		if(j1>=0 && j1<n && j2>=0 && j2<n)
			res->a[j1+n*j2] += a->v(i);
	}
	gr->LoadState();	return res;
}
//-----------------------------------------------------------------------------
HMDT mgl_hist_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT a, const char *opt)
{
	long nn=a->GetNx()*a->GetNy()*a->GetNz();
	if(nn!=x->GetNx()*x->GetNy()*x->GetNz() || nn!=y->GetNx()*y->GetNy()*y->GetNz() || nn!=z->GetNx()*z->GetNy()*z->GetNz())
	{	gr->SetWarn(mglWarnDim);	return (new mglData);	}
	long n = long(0.5+gr->SaveState(opt));
	if(n<mglFitPnts)	n = mglFitPnts;
	mglData *res = new mglData(n, n, n);
	register long i,j1,j2,j3;
	for(i=0;i<nn;i++)
	{
		j1 = long(n*(x->v(i)-gr->Min.x)/(gr->Max.x-gr->Min.x));
		j2 = long(n*(y->v(i)-gr->Min.y)/(gr->Max.y-gr->Min.y));
		j3 = long(n*(z->v(i)-gr->Min.z)/(gr->Max.z-gr->Min.z));
		if(j1>=0 && j1<n && j2>=0 && j2<n && j3>=0 && j3<n)
			res->a[j1+n*(j2+n*j3)] += a->v(i);
	}
	gr->LoadState();	return res;
}
//-----------------------------------------------------------------------------
uintptr_t mgl_hist_x_(uintptr_t* gr, uintptr_t* x, uintptr_t* a, const char *opt, int lo)
{	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	uintptr_t r = (uintptr_t)mgl_hist_x(_GR_, _DA_(x), _DA_(a), o);
	delete []o;	return r;	}
uintptr_t mgl_hist_xy_(uintptr_t* gr, uintptr_t* x, uintptr_t* y, uintptr_t* a, const char *opt, int lo)
{	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	uintptr_t r = (uintptr_t)mgl_hist_xy(_GR_, _DA_(x), _DA_(y), _DA_(a), o);
	delete []o;	return r;	}
uintptr_t mgl_hist_xyz_(uintptr_t* gr, uintptr_t* x, uintptr_t* y, uintptr_t* z, uintptr_t* a, const char *opt, int lo)
{	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	uintptr_t r = (uintptr_t)mgl_hist_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(a), o);
	delete []o;	return r;	}
//-----------------------------------------------------------------------------
const char *mgl_get_fit(HMGL )	{	return mglFitRes;	}
//-----------------------------------------------------------------------------
uintptr_t mgl_fit_1_(uintptr_t* gr, uintptr_t* y, const char *eq, const char *var, uintptr_t *ini, const char *opt, int l, int n, int lo)
{
	char *s=new char[l+1];	memcpy(s,eq,l);		s[l]=0;
	char *d=new char[n+1];	memcpy(d,var,n);	d[n]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	uintptr_t r = (uintptr_t)mgl_fit_1(_GR_, _DA_(y), s, d, _DM_(ini), o);
	delete []o;	delete []s;	delete []d;	return r;
}
uintptr_t mgl_fit_2_(uintptr_t* gr, uintptr_t* z, const char *eq, const char *var, uintptr_t *ini, const char *opt, int l, int n, int lo)
{
	char *s=new char[l+1];	memcpy(s,eq,l);		s[l]=0;
	char *d=new char[n+1];	memcpy(d,var,n);	d[n]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	uintptr_t r = (uintptr_t)mgl_fit_2(_GR_, _DA_(z), s, d, _DM_(ini), o);
	delete []o;	delete []s;	delete []d;	return r;
}
uintptr_t mgl_fit_3_(uintptr_t* gr, uintptr_t* a, const char *eq, const char *var, uintptr_t *ini, const char *opt, int l, int n, int lo)
{
	char *s=new char[l+1];	memcpy(s,eq,l);		s[l]=0;
	char *d=new char[n+1];	memcpy(d,var,n);	d[n]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	uintptr_t r = (uintptr_t)mgl_fit_3(_GR_, _DA_(a), s, d, _DM_(ini), o);
	delete []o;	delete []s;	delete []d;	return r;
}
uintptr_t mgl_fit_xy_(uintptr_t* gr, uintptr_t* x, uintptr_t* y, const char *eq, const char *var, uintptr_t *ini, const char *opt, int l, int n, int lo)
{
	char *s=new char[l+1];	memcpy(s,eq,l);		s[l]=0;
	char *d=new char[n+1];	memcpy(d,var,n);	d[n]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	uintptr_t r = (uintptr_t)mgl_fit_xy(_GR_, _DA_(x), _DA_(y), s, d, _DM_(ini), o);
	delete []o;	delete []s;	delete []d;	return r;
}
uintptr_t mgl_fit_xyz_(uintptr_t* gr, uintptr_t* x, uintptr_t* y, uintptr_t* z, const char *eq, const char *var, uintptr_t *ini, const char *opt, int l, int n, int lo)
{
	char *s=new char[l+1];	memcpy(s,eq,l);		s[l]=0;
	char *d=new char[n+1];	memcpy(d,var,n);	d[n]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	uintptr_t r = (uintptr_t)mgl_fit_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), s, d, _DM_(ini), o);
	delete []o;	delete []s;	delete []d;	return r;
}
uintptr_t mgl_fit_xyza_(uintptr_t* gr, uintptr_t* x, uintptr_t* y, uintptr_t* z, uintptr_t* a, const char *eq, const char *var, uintptr_t *ini, const char *opt, int l, int n, int lo)
{
	char *s=new char[l+1];	memcpy(s,eq,l);		s[l]=0;
	char *d=new char[n+1];	memcpy(d,var,n);	d[n]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	uintptr_t r = (uintptr_t)mgl_fit_xyza(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(a), s, d, _DM_(ini), o);
	delete []o;	delete []s;	delete []d;	return r;
}
uintptr_t mgl_fit_ys_(uintptr_t* gr, uintptr_t* y, uintptr_t* ss, const char *eq, const char *var, uintptr_t *ini, const char *opt, int l, int n, int lo)
{
	char *s=new char[l+1];	memcpy(s,eq,l);		s[l]=0;
	char *d=new char[n+1];	memcpy(d,var,n);	d[n]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	uintptr_t r = (uintptr_t)mgl_fit_ys(_GR_, _DA_(y), _DA_(ss), s, d, _DM_(ini), o);
	delete []o;	delete []s;	delete []d;	return r;
}
uintptr_t mgl_fit_xys_(uintptr_t* gr, uintptr_t* x, uintptr_t* y, uintptr_t* ss, const char *eq, const char *var, uintptr_t *ini, const char *opt, int l, int n, int lo)
{
	char *s=new char[l+1];	memcpy(s,eq,l);		s[l]=0;
	char *d=new char[n+1];	memcpy(d,var,n);	d[n]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	uintptr_t r = (uintptr_t)mgl_fit_xys(_GR_, _DA_(x), _DA_(y), _DA_(ss), s, d, _DM_(ini), o);
	delete []o;	delete []s;	delete []d;	return r;
}
uintptr_t mgl_fit_xyzs_(uintptr_t* gr, uintptr_t* x, uintptr_t* y, uintptr_t* z, uintptr_t* ss, const char *eq, const char *var, uintptr_t *ini, const char *opt, int l, int n, int lo)
{
	char *s=new char[l+1];	memcpy(s,eq,l);		s[l]=0;
	char *d=new char[n+1];	memcpy(d,var,n);	d[n]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	uintptr_t r = (uintptr_t)mgl_fit_xyzs(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(ss), s, d, _DM_(ini), o);
	delete []o;	delete []s;	delete []d;	return r;
}
uintptr_t mgl_fit_xyzas_(uintptr_t* gr, uintptr_t* x, uintptr_t* y, uintptr_t* z, uintptr_t* a, uintptr_t* ss, const char *eq, const char *var, uintptr_t *ini, const char *opt, int l, int n, int lo)
{
	char *s=new char[l+1];	memcpy(s,eq,l);		s[l]=0;
	char *d=new char[n+1];	memcpy(d,var,n);	d[n]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	uintptr_t r = (uintptr_t)mgl_fit_xyzas(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(a), _DA_(ss), s, d, _DM_(ini), o);
	delete []o;	delete []s;	delete []d;	return r;
}
//-----------------------------------------------------------------------------
