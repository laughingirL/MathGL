#include <stdio.h>
#include <string.h>
#include "mgl/mgl_eps.h"
//-----------------------------------------------------------------------------
int main(int narg, char **arg)
{
	mglGraphPS gr;
	mglParse p(true);

	if(narg==1)
	{
		printf("mgl2svg convert mgl script to vector svg file.\n");
		printf("Current version is 1.%g\n",MGL_VERSION);
		printf("Usage:\tmgl2svg scriptfile [outputfile parameter(s)]\n");
		printf("\tParameters have format \"-Nval\".\n");
		printf("\tHere N=0,1...9 is parameter ID and val is its value\n");
	}
	else
	{
		FILE *fp = fopen(arg[1],"rt");
		if(fp==0)	printf("Couldn't open file %s\n",arg[1]);
		else
		{
			char str[4096];
			for(long i=2;i<narg;i++)	// add arguments for the script
				if(arg[i][0]=='-' && arg[i][1]>='0' && arg[i][1]<='9')
					p.AddParam(arg[i][1]-'0',arg[i]+2);
			char buf[2048];
			while(!feof(fp))
			{
				fgets(str,4096,fp);
				buf[0]=0;
				int r = p.Parse(&gr,str);
				if(*buf)	printf("%s\n",buf);
				if(r==1)
					printf("Wrong argument(s) in %s\n",str);
				if(r==2)
					printf("Wrong command in %s\n",str);
				if(r==3)
					printf("String too long in %s\n",str);
			}
			fclose(fp);
			if(narg>2 && arg[2][0]!='-')	strcpy(str,arg[2]);
			else
			{
				strcpy(str,arg[1]);
				strcat(str,".svg");
				printf("Write output to %s\n",str);
			}
			gr.WriteSVG(str);
		}
	}
	return 0;
}
//-----------------------------------------------------------------------------
