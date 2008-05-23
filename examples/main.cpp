#include "mgl/mgl_parse.h"
#include "mgl/mgl_idtf.h"
#include "mgl/mgl_fltk.h"
#include "mgl/mgl_eps.h"
#include "mgl/mgl_c.h"
#include "mgl/mgl_eval.h"
#include <locale.h>
#include <wchar.h>

int sample_font(mglGraph *gr, void *)
{
	mglData y(50), a(50,50);
	y.Modify("0.7*sin(2*pi*x) + 0.5*cos(3*pi*x) + 0.2*sin(pi*x)",0);
	a.Modify("2*rnd-1");
	//gr->PlotFactor = 1.6;	// SERGEY !!! HERE !!!
//	gr->BaseLineWidth = 5;
	
	
//	gr->SubPlot(2,2,3);
//	gr->Rotate(40,60);
//	gr->Alpha(true);
//	gr->Aspect(2,1,1);
//	gr->Tile(a);
//	gr->Colorbar();
//	setlocale(LC_ALL, "ru_RU.cp1251");
//	gr->Puts(mglPoint(0,1.2,1),"привет, это тест");
	
//	gr->Perspective(0.3);
	gr->Org = mglPoint(-1,-1,-1);
	gr->Box();	gr->Grid();
//	gr->Text(y,"This is very long string drawn along a curve",":k");
	gr->Text(y,"Another string drawn above a curve","T:r");
	gr->Plot(y);
	
/*	gr->Axis();
	gr->Label('x',"\\xi  label",0);
	gr->Label('y',"y label",0);
	gr->Label('z',"z label",0);
	gr->AddLegend("Test 1_2","r");
	gr->AddLegend("\\sqrt{\\alpha^2}","g");
	gr->AddLegend("a_{ij}","b");
	gr->AddLegend("\\beta^{2^{3^{4}}}","ro");
	gr->Legend(3);*/
	return 0;
}
int sample_transp(mglGraph *gr, void *)	// flag #
{
	gr->Alpha(true);	gr->Light(true);	gr->Light(0,mglPoint(0,0,1));
	mglData a(30,20);
	a.Modify("0.6*sin(2*pi*x)*sin(3*pi*y) + 0.4*cos(3*pi*(x*y))");

	gr->TranspType = 0;	gr->Clf();
	gr->SubPlot(2,2,0);	gr->Rotate(40,60);	gr->Surf(a);	gr->Box();
	gr->SubPlot(2,2,1);	gr->Rotate(40,60);	gr->Dens(a);	gr->Box();
	gr->SubPlot(2,2,2);	gr->Rotate(40,60);	gr->Cont(a);	gr->Box();
	gr->SubPlot(2,2,3);	gr->Rotate(40,60);	gr->Axial(a);	gr->Box();
	return 0;
}
//-----------------------------------------------------------------------------
int sample(mglGraph *gr, void *)
{
	gr->Rotate(0,0);
	gr->Org = mglPoint(NAN,NAN,NAN);
	gr->Box();
	gr->Label('x',"x");
	gr->Label('y',"y");
	gr->Label('z',"z");
	gr->Axis();
	return 0;
}

//-----------------------------------------------------------------------------
extern mglTeXsymb mgl_tex_symb[];
int tex_sort(const void *a, const void *b)
{
	const mglTeXsymb *aa = (mglTeXsymb *)a;
	const mglTeXsymb *bb = (mglTeXsymb *)b;
	return wcscmp(aa->tex, bb->tex);
}
int mgl_cmd_cmp(const void *a, const void *b);
int main(int argc,char **argv)
{
//	mglData a;
//	a.ReadRange("/home/balakin/data/Balakin/s_%
/*	long k,i;
	for(k=0;mgl_tex_symb[k].kod;k++);
	qsort(mgl_tex_symb, k, sizeof(mglTeXsymb), tex_sort);
	FILE *fp = fopen("tex.txt","w");
	for(i=0;i<k;i++)
		fprintf(fp,"\t{0x%x, L\"%ls\"},\n",mgl_tex_symb[i].kod, mgl_tex_symb[i].tex);
	fclose(fp);*/
	
/*	long k,i;
	for(k=0;mgls_base_cmd[k].name[0];k++);
	qsort(mgls_base_cmd, k, sizeof(mglCommand), mgl_cmd_cmp);
	FILE *fp = fopen("cmd.txt","w");
	for(i=0;i<k;i++)
		fprintf(fp,"\"%s\"\n",mgls_base_cmd[i].name);
	fclose(fp);*/

//	setlocale(LC_ALL, "ru_RU.cp1251");
//	setlocale(LC_CTYPE, "ru_RU.cp1251");
//	mglGraphZB gr;//(1280,800);
//	mglGraphPS gr;//(1280,800);
//	sample(&gr,0);	gr.WritePNG("1.png",0,false);	return 0;
//	mglGraphIDTF gr;//(1280,800);
//	sample(&gr,0);	gr.WriteIDTF("1.idtf");	return 0;
	mglGraphFLTK gr;//(1280,800);
	gr.Window(0,0,sample,"Test");	return mglFlRun();
//	mglGraphGLUT gr;//(1280,800);
//	gr.Window(argc,argv,sample,"Test");	return 0;

//	HMGL gr = mgl_create_graph_fltk(argc,argv,sample,"test",0,0);
//	return mglFlRun();

}
//-----------------------------------------------------------------------------
