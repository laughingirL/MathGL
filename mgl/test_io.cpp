#include<mgl/mgl_io.h>
#include<stdio.h>

int main(void){
	mglIO	test;
	const char *ext1="new";
	const char **ext=new const char*[1];
	ext[0]=ext1;
	unsigned char	**p=NULL;
	test.Save("sml","",10,20,p);
	test.Load("sml","",10,20,p);
	printf("%d\n",test.Register("Loader for \"new\" format",1,ext,"test-mod",".libs/"));
	test.Save("new","",10,20,p);
	test.Load("new","",10,20,p);
}
