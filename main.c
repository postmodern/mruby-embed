#include <stdio.h>

#include "lib.h"

int main(int argc,const char *argv[])
{
	mrb_state *mrb;

	if (!(mrb = mrb_open()))
	{
		fprintf(stderr,"%s: could not initialize mruby\n",argv[0]);
		return -1;
	}

	lib_init(mrb);

	mrb_close(mrb);
	return 0;
}
