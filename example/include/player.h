#ifndef FLURMP_PLAYER_H
#define	FLURMP_PLAYER_H

#include "flurmp_impl.h"

/**
* Creates a rectangle.
*
* Params:
*   x position
*   y position
*/
fl_entity* fl_create_player(int, int);

/**
 * Registers the implementation of a player entity.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity_type - the entity type of a player
 */
void fl_register_player_type(fl_context* context, fl_entity_type*);

#endif
