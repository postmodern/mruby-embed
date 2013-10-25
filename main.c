#include <stdio.h>

#include "vm.h"

int main(int argc,const char *argv[])
{
	mrb_state *mrb;
	
	if (!(mrb = vm_load()))
	{
		fprintf(stderr,"%s: failed to load VM\n",argv[0]);
		return -1;
	}

	return 0;
}
