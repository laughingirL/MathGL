#include <stdio.h>

#define load  test_mod_LTX_load
#define save  test_mod_LTX_save

#ifdef __cplusplus
extern "C" {

int load(const char *fname, int w,int h, unsigned char **p)
{
	printf("Sample module load called\n");
}

int save(const char *fname, int w,int h, unsigned char **p)
{
	printf("Sample module save called\n");
}

}
#endif