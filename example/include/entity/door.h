/**
 * A primitive form of scene transition.
 */
#ifndef FLURMP_DOOR_H
#define	FLURMP_DOOR_H

#include "core/flurmp_impl.h"

/**
 * Creates a door entity.
 *
 * Params:
 *   x position
 *   y position
 *
 * Returns:
 *   fl_entity - a door entity
 */
fl_entity* fl_create_door(int, int);

/**
 * Registers the implementation of a door entity.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity_type - the entity type of a door
 */
void fl_register_door_type(fl_context* context, fl_entity_type*);

#endif
