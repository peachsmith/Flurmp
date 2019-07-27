#include "waiter.h"

fl_waiter* fl_create_waiter(void(*action)(fl_context*, fl_waiter*, void*), int limit, void* target)
{
	fl_waiter* w = fl_alloc(fl_waiter, 1);

	if (w == NULL)
		return NULL;

	w->action = action;
	w->current = 0;
	w->limit = limit;
	w->done = 0;
	w->target = target;
	w->next = NULL;
	w->prev = NULL;

	return w;
}

void fl_destroy_waiter(fl_waiter* waiter)
{
	if (waiter == NULL)
		return;

	fl_free(waiter);
}

void fl_add_waiter(fl_context* context, fl_waiter* waiter)
{
	if (context->waiters == NULL)
	{
		context->waiters = waiter;
		return;
	}

	fl_waiter* w = context->waiters;

	while (w->next != NULL)
	{
		w = w->next;
	}

	w->next = waiter;
	waiter->prev = w;
}

void fl_remove_waiter(fl_context* context, fl_waiter* waiter)
{
	if (context->waiters == NULL || waiter == NULL)
		return;

	fl_waiter* w = context->waiters;

	while (w != waiter && w != NULL)
	{
		w = w->next;
	}

	if (w == NULL)
		return;

	if (w->prev != NULL)
		w->prev->next = w->next;

	if (w->next != NULL)
		w->next->prev = w->prev;

	if (w->next == NULL && w->prev == NULL)
		context->waiters = NULL;
}
