/**
 * A sign is an entity that displays information
 * when the player interacts with it.
 */
#ifndef FLURMP_SIGN_H
#define	FLURMP_SIGN_H

#include "flurmp_impl.h"

/**
* Creates a sign entity.
*
* Params:
*   x position
*   y position
*/
fl_entity* fl_create_sign(int, int);

/**
 * Registers the implementation of a sign entity.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity_type - the entity type of a sign
 */
void fl_register_sign_type(fl_context* context, fl_entity_type*);

#endif
