/**
 * A player entity is the avatar of the user during the course of
 * a Flurmp context's lifetime.
 */
#ifndef FLURMP_PLAYER_H
#define	FLURMP_PLAYER_H

#include "flurmp_impl.h"

/**
 * Creates a player entity.
 *
 * Params:
 *   x position
 *   y position
 *
 * Returns:
 *   fl_entity - a player entity
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

/**
 * Creates the waiters for a player entity and adds them to a context.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - a player entity
 *
 * Returns:
 *   int - 1 on success, or 0 on failure
 */
int fl_load_player_waiters(fl_context* context, fl_entity* player);

#endif
