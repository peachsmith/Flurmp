#include <stdio.h>

#include "flurmp.h"

int main(int argc, char** argv)
{
	/* initialization */
	if (!fl_initialize())
	{
		fprintf(stderr, "initialization failure %s\n", fl_get_error());
		return 1;
	}

	fl_context* context = fl_create_context();

	/* main loop */
	while (!fl_is_done(context))
	{
		fl_begin_frame(context);

		fl_handle_events(context);
		fl_handle_input(context);
		fl_update(context);
		fl_render(context);

		fl_end_frame(context);
	}

	/* cleanup */
	fl_destroy_context(context);
	fl_terminate();

	return 0;
}
