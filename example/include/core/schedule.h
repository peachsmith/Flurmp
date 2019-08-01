#ifndef FLURMP_SCHEDULE_H
#define FLURMP_SCHEDULE_H

#include "core/flurmp_impl.h"

/**
 * Creates a new schedule.
 *
 * Params:
 *   void(*action)(fl_context*, fl_schedule*, void*) - an action to be taken
 *   int - the limit set on the counter in the schedule
 *   void* - a pointer to the data to be modified cast as void
 *
 * Returns:
 *   fl_schedule - a new schedule
 */
fl_schedule* fl_create_schedule(
	void(*action)(fl_context*, fl_schedule*, void*),
	int limit, 
	void* target);

/**
 * Frees the memory allocated for a schedule.
 *
 * Params:
 *   fl_schedule - a schedule
 */
void fl_destroy_schedule(fl_schedule* schedule);

/**
 * Adds a schedule to a context.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_schedule - a schedule to be added to the context
 */
void fl_add_schedule(fl_context* context, fl_schedule* schedule);

/**
 * Removes a schedule from a context.
 * This function does NOT free the memory of the schedule that
 * was removed.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_schedule - a schedule to be added to the context
 */
void fl_remove_schedule(fl_context* context, fl_schedule* schedule);

#endif
