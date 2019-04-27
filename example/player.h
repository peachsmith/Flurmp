#ifndef FLURMP_PLAYER_H
#define	FLURMP_PLAYER_H

#include "flurmp_impl.h"

/**
* Creates a rectangle.
*
* Params:
*   x position
*   y position
*   width
*   height
*/
fl_entity* fl_create_player(int, int, int, int);

/**
 * Registers the implementation of a player entity.
 *
 * Params:
 *   fl_entity_type - the entity type of a player
 */
void fl_register_player_type(fl_entity_type*);

#endif
