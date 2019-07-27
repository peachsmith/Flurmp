#ifndef FLURMP_WAITER_H
#define FLURMP_WAITER_H

/* TODO: come up with a better name for this */

#include "flurmp_impl.h"

fl_waiter* fl_create_waiter(void(*action)(fl_context*, fl_waiter*, void*), int limit, void* target);

void fl_destroy_waiter(fl_waiter* waiter);

void fl_add_waiter(fl_context* context, fl_waiter* waiter);

void fl_remove_waiter(fl_context* context, fl_waiter* waiter);

#endif
