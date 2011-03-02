#include <time.h>
#include <stdio.h>

//#include "mgl/mgl.h"

int main()
{
//	mglGraph gr;
	time_t t = time(NULL);
	printf("time:%ld - %ld, long:%ld, float:%ld\n",sizeof(time_t), t,sizeof(long),sizeof(float));
	return 0;
}
