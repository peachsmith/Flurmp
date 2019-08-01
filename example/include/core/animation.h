#ifndef FLURMP_ANIMATION_H
#define FLURMP_ANIMATION_H

#include "core/flurmp_impl.h"

/**
 * Creates a new animation.
 *
 * Params:
 *   int - the number of frames in the animation
 *
 * Returns:
 *   fl_animation - a new animation
 */
fl_animation* fl_create_animation(int f);

/**
 * Frees the memory allocated for an animation.
 *
 * Params:
 *   fl_animation - an animation
 */
void fl_destroy_animation(fl_animation* a);

#endif
