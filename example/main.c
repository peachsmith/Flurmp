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
	/* initialize the framework */
	if (!fl_initialize())
	{
		printf("initialization failure %s\n", fl_get_error());
		return 1;
	}

	/* create the context */
	fl_context* context = fl_create_context();

	/* while not done */
	while (!context->done)
	{
		fl_begin_frame(context);

		/* poll for events */
		while (fl_poll_event(context))
		{
			/* handle events */
			fl_handle_event(context);
		}

		/* handle input */
		fl_handle_input(context);

		/* update the state */
		fl_update(context);

		/* render */
		fl_render(context);

		/* regulate framerate */
		fl_end_frame(context);
	}

	/* destroy the context */
	fl_destroy_context(context);

	/* terminate the framework */
	fl_terminate();

	return 0;
}
