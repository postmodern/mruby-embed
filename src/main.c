#include <stdio.h>
#include <mruby.h>
#include <mruby/array.h>

#include "lib.h"

int main(int argc,const char *argv[])
{
	mrb_state *mrb;
	mrb_value ret;

	// initialize mruby
	if (!(mrb = mrb_open()))
	{
		fprintf(stderr,"%s: could not initialize mruby\n",argv[0]);
		return -1;
	}

	mrb_value args = mrb_ary_new(mrb);
	int i;

	// convert argv into mruby strings
	for (i=1; i<argc; i++)
	{
		 mrb_ary_push(mrb, args, mrb_str_new_cstr(mrb,argv[i]));
	}

	mrb_define_global_const(mrb, "ARGV", args);

	// load the compiled library
	ret = lib_init(mrb);

	// check for exception
	if (mrb->exc)
	{
		// print exception
		mrb_p(mrb, mrb_obj_value(mrb->exc));
	}

	// cleanup
	mrb_close(mrb);
	return 0;
}
