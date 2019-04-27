#ifndef FLURMP_RECTANGLE_H
#define	FLURMP_RECTANGLE_H

#include "flurmp.h"

/**
 * Creates a rectangle.
 *
 * Params:
 *   x position
 *   y position
 *   width
 *   height
 */
fl_entity* fl_create_rectangle(int, int, int, int);

/**
 * Registers the implementation of a rectangle entity.
 *
 * Params:
 *   fl_entity_type - the entity type of a rectangle
 */
void fl_register_rectangle_type(fl_entity_type*);

#endif
