/* mgl_fit.cpp is part of Math Graphic Library
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
#include <gsl/gsl_multifit_nlin.h>
#include <gsl/gsl_blas.h>
#include <ctype.h>
#include "mgl/mgl_eval.h"
#include "mgl/mgl.h"
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
int	mgl_fit__f (const gsl_vector *x, void *data, gsl_vector *f)
{
	mglFitData *fd = (mglFitData *)data;
	register long i;
	float val['z'-'a'+1];
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
	float val['z'-'a'+1],a,s;
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
//-----------------------------------------------------------------------------
/// GSL based fitting procedure for formula/arguments specified by string
float mgl_fit_base(mglFitData *fd, float *ini)
{
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
}
//-----------------------------------------------------------------------------
float mglGraph::Fit(mglData &fit, mglData &y, const char *eq, const char *var, float *ini, bool print)
{
	mglData x(y.nx);	x.Fill(Min.x, Max.x);
	mglData s(y);		s.Fill(1,1);
	return FitS(fit,x,y,s,eq,var,ini,print);
}
//-----------------------------------------------------------------------------
float mglGraph::Fit2(mglData &fit, mglData &z, const char *eq, const char *var, float *ini, bool print)
{
	mglData x(z.nx);	x.Fill(Min.x, Max.x);
	mglData y(z.ny);	y.Fill(Min.y, Max.y);
	mglData s(z);		s.Fill(1,1);
	return FitS(fit,x,y,z,s,eq,var,ini,print);
}
//-----------------------------------------------------------------------------
float mglGraph::Fit3(mglData &fit, mglData &a, const char *eq, const char *var, float *ini, bool print)
{
	mglData x(a.nx);	x.Fill(Min.x, Max.x);
	mglData y(a.ny);	y.Fill(Min.y, Max.y);
	mglData z(a.nz);	z.Fill(Min.z, Max.z);
	mglData s(a);		s.Fill(1,1);
	return FitS(fit,x,y,z,a,s,eq,var,ini,print);
}
//-----------------------------------------------------------------------------
float mglGraph::Fit(mglData &fit, mglData &x, mglData &y, const char *eq, const char *var, float *ini, bool print)
{
	mglData s(y);	s.Fill(1,1);
	return FitS(fit,x,y,s,eq,var,ini,print);	
}
//-----------------------------------------------------------------------------
float mglGraph::Fit(mglData &fit, mglData &x, mglData &y, mglData &z, const char *eq, const char *var, float *ini, bool print)
{
	mglData s(z);	s.Fill(1,1);
	return FitS(fit,x,y,z,s,eq,var,ini,print);	
}
//-----------------------------------------------------------------------------
float mglGraph::Fit(mglData &fit, mglData &x, mglData &y, mglData &z, mglData &a, const char *eq, const char *var, float *ini, bool print)
{
	mglData s(a);	s.Fill(1,1);
	return FitS(fit,x,y,z,a,s,eq,var,ini,print);	
}
//-----------------------------------------------------------------------------
float mglGraph::FitS(mglData &fit, mglData &y, mglData &s, const char *eq, const char *var, float *ini, bool print)
{
	mglData x(y.nx);	x.Fill(Min.x, Max.x);
	return FitS(fit,x,y,s,eq,var,ini,print);
}

//-----------------------------------------------------------------------------
float mglGraph::FitS(mglData &fit, mglData &x, mglData &y, mglData &s, const char *eq, const char *var, float *ini, bool print)
{
	if(x.nx!=y.nx)	{	SetWarn(mglWarnDim,"Fit[S]");	return -1;	}
	if(y.nx<2)		{	SetWarn(mglWarnLow,"Fit[S]");	return -1;	}
	if(s.nx*s.ny*s.nz != y.nx*y.ny*y.nz)
	{	SetWarn(mglWarnDim,"FitS");	return -1;	}
	mglFitData fd;
	fd.n = y.nx;	fd.x = x.a;	fd.y = 0;
	fd.z = 0;		fd.a = y.a;	fd.s = s.a;
	fd.var = var;	fd.m = strlen(var);
	fd.eq = new mglFormula(eq);
	mglData in(fd.m);
	if(fit.nx<FitPnts)	fit.Create(FitPnts, y.ny, y.nz);
	float val['z'-'a'+1],res=-1;
	register long j;
	for(long i=0;i<y.ny*y.nz && i<fit.nx*fit.ny;i++)
	{
		if(ini)	in.Set(ini,fd.m);	else in.Fill(0,0);
		fd.a = y.a+i*y.nx;		fd.x = x.a+(i%x.ny)*y.nx;
		fd.s = s.a+i*y.nx;
		res = mgl_fit_base(&fd,in.a);
		for(j=0;j<fd.m;j++)	val[var[j]-'a'] = in.a[j];
		for(j=0;j<y.nx;j++)
		{
			val['x'-'a'] = x.a[j+(i%x.ny)*y.nx];
			fit.a[j+i*y.nx] = fd.eq->Calc(val);
		}
		if(ini)	memcpy(ini,in.a,fd.m*sizeof(float));
	}
	PrepareFitEq(res,eq,var,ini,print);
	delete fd.eq;
	return res;
}
//-----------------------------------------------------------------------------
float mglGraph::FitS(mglData &fit, mglData &xx, mglData &yy, mglData &z, mglData &s, const char *eq, const char *var, float *ini, bool print)
{
	if(xx.nx!=z.nx)		{	SetWarn(mglWarnDim,"Fit[S]");	return -1;	}
	if(s.nx*s.ny*s.nz != z.nx*z.ny*z.nz)
	{	SetWarn(mglWarnDim,"FitS");	return -1;	}
	if(yy.nx!=z.ny && (xx.ny!=z.ny || yy.nx!=z.nx || yy.ny!=z.ny))
	{	SetWarn(mglWarnDim);	return -1;	}
	if(z.nx<2|| z.ny<2)	{	SetWarn(mglWarnLow,"Fit[S]");	return -1;	}
	
	mglData x(z.nx, z.ny), y(z.nx, z.ny);
	register long i,j;
	for(i=0;i<z.nx;i++)	for(j=0;j<z.ny;j++)	// создаем массив точек
	{
		x.a[i+z.nx*j] = GetX(xx,i,j,0);
		y.a[i+z.nx*j] = GetY(yy,i,j,0);
	}
	mglFitData fd;
	fd.n = y.nx;	fd.x = x.a;	fd.y = y.a;
	fd.z = 0;		fd.a = z.a;	fd.s = s.a;
	fd.var = var;	fd.m = strlen(var);
	fd.eq = new mglFormula(eq);
	mglData in(fd.m);
	if(fit.nx<FitPnts || fit.ny<FitPnts)	fit.Create(FitPnts, FitPnts, z.nz);
	float val['z'-'a'+1], res = -1;
	for(long i=0;i<z.nz && i<fit.nz;i++)
	{
		if(ini)	in.Set(ini,fd.m);	else in.Fill(0,0);
		fd.a = z.a+i*z.nx*z.ny;		fd.s = s.a+i*z.nx*z.ny;
		res = mgl_fit_base(&fd,in.a);
		for(j=0;j<fd.m;j++)	val[var[j]-'a'] = in.a[j];
		for(j=0;j<z.nx*z.ny;j++)
		{
			val['x'-'a'] = x.a[j];	val['y'-'a'] = y.a[j];
			fit.a[j+i*z.nx*z.ny] = fd.eq->Calc(val);
		}
		if(ini)	memcpy(ini,in.a,fd.m*sizeof(float));
	}
	PrepareFitEq(res, eq,var,ini,print);
	delete fd.eq;
	return res;
}
//-----------------------------------------------------------------------------
float mglGraph::FitS(mglData &fit, mglData &xx, mglData &yy, mglData &zz, mglData &a, mglData &s, const char *eq, const char *var, float *ini, bool print)
{
	register long i,j,k,i0;
	i = a.nx*a.ny*a.nz;
	if(a.nx<2 || a.ny<2 || a.nz<2)	{	SetWarn(mglWarnLow,"Fit[S]");	return -1;	}
	if(s.nx*s.ny*s.nz != i)			{	SetWarn(mglWarnDim,"FitS");	return -1;	}
	bool both = xx.nx*xx.ny*xx.nz==i && yy.nx*yy.ny*yy.nz==i && zz.nx*zz.ny*zz.nz==i;
	if(!(both || (xx.nx==a.nx && yy.nx==a.ny && zz.nx==a.nz)))
	{	SetWarn(mglWarnDim,"Fit[S]");	return -1;	}
	mglData x(a), y(a), z(a);
	for(i=0;i<a.nx;i++)	for(j=0;j<a.ny;j++)	for(j=0;j<a.nz;j++)	// создаем массив точек
	{
		i0 = i+z.nx*j+z.nx*z.ny*k;
		x.a[i0] = GetX(xx,i,j,k);
		y.a[i0] = GetY(yy,i,j,k);
		z.a[i0] = GetZ(zz,i,j,k);
	}
	mglFitData fd;
	fd.n = y.nx;	fd.x = x.a;	fd.y = y.a;
	fd.z = z.a;		fd.a = a.a;	fd.s = s.a;
	fd.var = var;	fd.m = strlen(var);
	fd.eq = new mglFormula(eq);
	mglData in(fd.m);
	if(fit.nx<FitPnts || fit.ny<FitPnts)	fit.Create(FitPnts, FitPnts, z.nz);
	float val['z'-'a'+1], res = -1;
	for(long i=0;i<z.nz && i<fit.nz;i++)
	{
		if(ini)	in.Set(ini,fd.m);	else in.Fill(0,0);
		fd.a = a.a+i*a.nx*a.ny*a.nz;
		res = mgl_fit_base(&fd,in.a);
		for(j=0;j<fd.m;j++)	val[var[j]-'a'] = in.a[j];
		for(j=0;j<a.nx*a.ny*a.nz;j++)
		{
			val['x'-'a'] = x.a[j];	val['y'-'a'] = y.a[j];
			val['z'-'a'] = z.a[j];	fit.a[j] = fd.eq->Calc(val);
		}
		if(ini)	memcpy(ini,in.a,fd.m*sizeof(float));
	}
	PrepareFitEq(res, eq,var,ini,print);
	delete fd.eq;
	return res;
}
//-----------------------------------------------------------------------------
float mglGraph::Fit(mglData &fit, mglData &y, const char *eq, const char *var, mglData &ini, bool print)
{
	if(ini.nx<strlen(var))	{	SetWarn(mglWarnLow,"Fit");	return -1;	}
	return Fit(fit,y,eq,var,ini.a,print);
}
float mglGraph::Fit2(mglData &fit, mglData &z, const char *eq, const char *var, mglData &ini, bool print)
{
	if(ini.nx<strlen(var))	{	SetWarn(mglWarnLow,"Fit");	return -1;	}
	return Fit2(fit,z,eq,var,ini.a,print);
}
float mglGraph::Fit3(mglData &fit, mglData &a, const char *eq, const char *var, mglData &ini, bool print)
{
	if(ini.nx<strlen(var))	{	SetWarn(mglWarnLow,"Fit");	return -1;	}
	return Fit3(fit,a,eq,var,ini.a,print);
}
float mglGraph::Fit(mglData &fit, mglData &x, mglData &y, const char *eq, const char *var, mglData &ini, bool print)
{
	if(ini.nx<strlen(var))	{	SetWarn(mglWarnLow,"Fit");	return -1;	}
	return Fit(fit,x,y,eq,var,ini.a,print);
}
float mglGraph::Fit(mglData &fit, mglData &x, mglData &y, mglData &z, const char *eq, const char *var, mglData &ini, bool print)
{
	if(ini.nx<strlen(var))	{	SetWarn(mglWarnLow,"Fit");	return -1;	}
	return Fit(fit,x,y,z,eq,var,ini.a,print);
}
float mglGraph::Fit(mglData &fit, mglData &x, mglData &y, mglData &z, mglData &a, const char *eq, const char *var, mglData &ini, bool print)
{
	if(ini.nx<strlen(var))	{	SetWarn(mglWarnLow,"Fit");	return -1;	}
	return Fit(fit,x,y,z,a,eq,var,ini.a,print);
}
float mglGraph::FitS(mglData &fit, mglData &y, mglData &s, const char *eq, const char *var, mglData &ini, bool print)
{
	if(ini.nx<strlen(var))	{	SetWarn(mglWarnLow,"Fit");	return -1;	}
	return FitS(fit,y,s,eq,var,ini.a,print);
}
float mglGraph::FitS(mglData &fit, mglData &x, mglData &y, mglData &s, const char *eq, const char *var, mglData &ini, bool print)
{
	if(ini.nx<strlen(var))	{	SetWarn(mglWarnLow,"Fit");	return -1;	}
	return FitS(fit,x,y,s,eq,var,ini.a,print);
}
float mglGraph::FitS(mglData &fit, mglData &x, mglData &y, mglData &z, mglData &s, const char *eq, const char *var, mglData &ini, bool print)
{
	if(ini.nx<strlen(var))	{	SetWarn(mglWarnLow,"Fit");	return -1;	}
	return FitS(fit,x,y,z,s,eq,var,ini.a,print);
}
float mglGraph::FitS(mglData &fit, mglData &x, mglData &y, mglData &z, mglData &a, mglData &s, const char *eq, const char *var, mglData &ini, bool print)
{
	if(ini.nx<strlen(var))	{	SetWarn(mglWarnLow,"Fit");	return -1;	}
	return FitS(fit,x,y,z,a,s,eq,var,ini.a,print);
}
//-----------------------------------------------------------------------------
void mglGraph::PrepareFitEq(float chi, const char *eq, const char *var, float *par, bool print)
{
	if(print && Message)
	{
		sprintf(Message,"chi=%g,\t",chi);
		for(int i=0;i<strlen(var);i++)
		{
			sprintf(fit_res,"%c=%g,\t",var[i],par[i]);
			strcat(Message, fit_res);
		}
	}
	memset(fit_res, 0, 1024);	//fit_res[0] = 0;
	char buf[32]="";
	register long i,k,len=strlen(eq);
	for(i=k=0;i<len;i++)
	{
		const char *c = strchr(var,eq[i]);
		if(c && (i==0 || !isalnum(eq[i-1])) && (i==len-1 || !isalnum(eq[i+1])))
		{
			sprintf(buf,"%g",par[c-var]);
			strcat(fit_res+k, buf);	k+=strlen(buf);
		}
		else	{	fit_res[k] = eq[i];	k++;	}
	}
	fit_res[k]=0;
}
//-----------------------------------------------------------------------------
void mglGraph::PutsFit(mglPoint p, const char *pre, const char *font, float size)
{
	long n = strlen(fit_res)+(pre?strlen(pre):0)+1;
	char *buf = new char[n];
	if(pre)	sprintf(buf,"%s%s",pre,fit_res);
	else	strcpy(buf,fit_res);
	Puts(p,buf,font,size);
	delete []buf;
}
//-----------------------------------------------------------------------------
