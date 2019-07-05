/**
 * An inert, 200 x 50 solid block.
 */
#ifndef FLURMP_BLOCK_200_50_H
#define	FLURMP_BLOCK_200_50_H

#include "flurmp.h"

/**
 * Creates a rectangle.
 *
 * Params:
 *   x position
 *   y position
 */
fl_entity* fl_create_block_200_50(int, int);

/**
 * Registers the implementation of a rectangle entity.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity_type - the entity type of a rectangle
 */
void fl_register_block_200_50_type(fl_context* context, fl_entity_type*);

#endif
