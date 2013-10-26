#include <stdio.h>
#include <mruby.h>

#include "lib.h"

int main(int argc,const char *argv[])
{
	mrb_state *mrb;
	mrb_value ret;

	if (!(mrb = mrb_open()))
	{
		fprintf(stderr,"%s: could not initialize mruby\n",argv[0]);
		return -1;
	}

	ret = lib_init(mrb);
	ret = mrb_funcall(mrb, mrb_top_self(mrb), "main", 0);

	if (mrb->exc)
	{
		mrb_p(mrb, mrb_obj_value(mrb->exc));
	}

	mrb_close(mrb);
	return 0;
}
