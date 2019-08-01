#include "core/schedule.h"

fl_schedule* fl_create_schedule(void(*action)(fl_context*, fl_schedule*, void*), int limit, void* target)
{
	fl_schedule* w = fl_alloc(fl_schedule, 1);

	if (w == NULL)
		return NULL;

	w->action = action;
	w->counter = 0;
	w->limit = limit;
	w->done = 0;
	w->target = target;
	w->next = NULL;
	w->prev = NULL;

	return w;
}

void fl_destroy_schedule(fl_schedule* schedule)
{
	if (schedule == NULL)
		return;

	fl_free(schedule);
}

void fl_add_schedule(fl_context* context, fl_schedule* schedule)
{
	if (context->schedules == NULL)
	{
		context->schedules = schedule;
		return;
	}

	fl_schedule* s = context->schedules;

	while (s->next != NULL)
	{
		s = s->next;
	}

	s->next = schedule;
	schedule->prev = s;
}

void fl_remove_schedule(fl_context* context, fl_schedule* schedule)
{
	if (context->schedules == NULL || schedule == NULL)
		return;

	fl_schedule* s = context->schedules;

	while (s != schedule && s != NULL)
	{
		s = s->next;
	}

	if (s == NULL)
		return;

	if (s->prev != NULL)
		s->prev->next = s->next;

	if (s->next != NULL)
		s->next->prev = s->prev;

	if (s->next == NULL && s->prev == NULL)
		context->schedules = NULL;
}
