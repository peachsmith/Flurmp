/*
 * an example of flurmp usage
 *
 * the general flow should be something like this:
 *
 * initialize framework
 * create the context
 * while not done
 * {
 *   while polling for events
 *   {
 *     handle events
 *   }
 *   handle input
 *   update state
 *   render
 *   regulate framerate
 * }
 * destroy the context
 * terminate the framework
 */
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

		while (fl_poll_event(context))
		{
			fl_handle_event(context);
		}

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
