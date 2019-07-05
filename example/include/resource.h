#ifndef FLURMP_RESOURCE_H
#define FLURMP_RESOURCE_H

#include "flurmp_impl.h"

#define FLURMP_IMAGE_RESOURCE 1
#define FLURMP_FONT_RESOURCE 2

/**
 * Loads a BMP image.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   const char* - a string containing the path to the resource
 *
 * Returns:
 *   fl_resource - a reference to a newly created resource
 */
fl_resource* fl_load_bmp(fl_context* context, const char* path);

/**
 * Loads a font.
 *
 * Params:
 *   const char* - a string containing the path to the resource
 *   int - the font size in points
 *   fl_color - the foreground color
 *   fl_color - the background color
 *   int - a flag indicating whether or not to use a background color.
 *         If this value is 0, then no background color is used,
 *         otherwise the background color passed in is used.
 *
 *
 * Returns:
 *   fl_resource - a reference to a newly created resource
 */
fl_resource* fl_load_font(const char* path, int p, fl_color fc, fl_color bc, int background);

/**
 * Frees the memory allocated for a resource.
 *
 * Params:
 *   fl_resource - a resource
 */
void fl_destroy_resource(fl_resource* resource);

#endif
