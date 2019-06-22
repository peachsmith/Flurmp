#ifndef FLURMP_INTERACTABLE_H
#define	FLURMP_INTERACTABLE_H

#include "flurmp.h"

/**
* Creates an interactable entity.
*
* Params:
*   x position
*   y position
*   width
*   height
*/
fl_entity* fl_create_interactable(int, int, int, int);

/**
 * Registers the implementation of an interactable entity.
 *
 * Params:
 *   fl_entity_type - the entity type of an interactable
 */
void fl_register_interactable_type(fl_entity_type*);

#endif
