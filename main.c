#include <stdio.h>
#include <mruby.h>

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

	// load the compiled library
	ret = lib_init(mrb);

	mrb_sym name = mrb_intern_cstr(mrb,"main");
	mrb_value args[argc-1];
	int i;

	// convert argv into mruby strings
	for (i=0; i<argc-1; i++)
	{
		args[i] = mrb_str_new_cstr(mrb,argv[i+1]);
	}

	// call main
	ret = mrb_funcall_argv(mrb, mrb_top_self(mrb), name, argc-1, args);

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
