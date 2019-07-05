#ifndef FLURMP_SCENE_H
#define FLURMP_SCENE_H

#include "flurmp_impl.h"

#define FLURMP_SCENE_BASIC 0

/**
 * Populates a context with entities and resources.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   int - a scene ID indicating which scene to load
 */
void fl_load_scene(fl_context* context, int id);

/**
 * Decomissions the entities and resources for the
 * current scene in a context.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
void fl_clear_scene(fl_context* context);

#endif
