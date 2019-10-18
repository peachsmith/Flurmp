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
#define FLURMP_SCENE_TEST_2 2

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

/**
 * Signals to a Flurmp context that the scene should transition.
 * A scene transition should only occur after entity interactions
 * are completed so as not to change the locations of entities
 * in memory while they are being used.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   int - the current scene
 *   int - the next scene
 */
void fl_schedule_scene_transition(fl_context* context, int from_scene, int to_scene);

#endif
