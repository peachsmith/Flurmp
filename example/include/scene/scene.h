/**
 * A scene represents the current collection of entities and resources
 * within a context.
 * 
 * The contents of a scene may extend beyond the boundaries of the screen.
 */
#ifndef FLURMP_SCENE_H
#define FLURMP_SCENE_H

#include "core/flurmp_impl.h"

#define FLURMP_SCENE_NONE 0
#define FLURMP_SCENE_TEST_1 1

/**
 * Populates a context with entities and resources.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   int - a scene ID indicating which scene to load
 */
void fl_load_scene(fl_context* context, int id);

/**
 * Removes entities and unloads resources from a context.
 * When a scene is cleared, all entities are destroyed, but some
 * resources may be left to be reused.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
void fl_clear_scene(fl_context* context);

#endif
