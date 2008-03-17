#include <stdio.h>
#include <string.h>
#include "mgl/mgl_fltk.h"
//-----------------------------------------------------------------------------
char *fname;
mglParse p(true);
char str[4096];
char buf[2048];
//-----------------------------------------------------------------------------
int show(mglGraph *gr, void *)
{
	FILE *fp = fopen(fname,"rt");
	while(!feof(fp))
	{
		fgets(str,4096,fp);
		buf[0]=0;
		int r = p.Parse(gr,str);
		if(*buf)	printf("%s\n",buf);
		if(r)
		{
			if(r==1)	printf("Wrong argument(s) in %s\n",str);
			if(r==2)	printf("Wrong command in %s\n",str);
			if(r==3)	printf("String too long in %s\n",str);
		}
	}
	fclose(fp);
	return 0;
}
//-----------------------------------------------------------------------------
int main(int narg, char **arg)
{
	mglGraphFLTK gr;

	if(narg==1)
	{
		printf("mglview show MGL script in FLTK window.\n");
		printf("Current version is 1.%g\n",MGL_VERSION);
		printf("Usage:\tmglview scriptfile [parameter(s)]\n");
		printf("\tParameters have format \"-Nval\".\n");
		printf("\tHere N=0,1...9 is parameter ID and val is its value\n");
	}
	else
	{
		FILE *fp = fopen(arg[1],"rt");
		if(fp==0)	printf("Couldn't open file %s\n",arg[1]);
		else
		{
			for(long i=2;i<narg;i++)	// add arguments for the script
				if(arg[i][0]=='-' && arg[i][1]>='0' && arg[i][1]<='9')
					p.AddParam(arg[i][1]-'0',arg[i]+2);
			fname = arg[1];
			fclose(fp);
			gr.Window(narg,arg,show,fname,NULL);
			return mglFlRun();
		}
	}
	return 0;
}
//-----------------------------------------------------------------------------
