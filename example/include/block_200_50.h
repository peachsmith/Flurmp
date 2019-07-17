/**
 * A 200 x 50 pixel solid block.
 */
#ifndef FLURMP_BLOCK_200_50_H
#define	FLURMP_BLOCK_200_50_H

#include "flurmp.h"

/**
 * Creates a block_200_50 entity.
 *
 * Params:
 *   x position
 *   y position
 *
 * Returns:
 *   fl_entity - a block_200_50 entity
 */
fl_entity* fl_create_block_200_50(int, int);

/**
 * Registers the implementation of a block_200_50 entity.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity_type - the entity type of a block_200_50
 */
void fl_register_block_200_50_type(fl_context* context, fl_entity_type*);

#endif
