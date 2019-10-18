/**
 * A pellet entity is the avatar of the user during the course of
 * a Flurmp context's lifetime.
 */
#ifndef FLURMP_PELLET_H
#define	FLURMP_PELLET_H

#include "core/flurmp_impl.h"

/**
 * Creates a pellet entity.
 *
 * Params:
 *   x position
 *   y position
 *
 * Returns:
 *   fl_entity - a pellet entity
 */
fl_entity* fl_create_pellet(int, int);

/**
 * Registers the implementation of a pellet entity.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity_type - the entity type of a pellet
 */
void fl_register_pellet_type(fl_context* context, fl_entity_type*);

/**
 * Creates the schedules for a pellet entity and adds them to a context.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - a pellet entity
 *
 * Returns:
 *   int - 1 on success, or 0 on failure
 */
int fl_load_pellet_schedules(fl_context* context, fl_entity* pellet);

int fl_schedule_pellet(fl_context* context, fl_entity* pellet);

#endif
