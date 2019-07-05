#ifndef FLURMP_TEXT_H
#define FLURMP_TEXT_H

#include "flurmp_impl.h"

/* total number of fonts available to the application */
#define FLURMP_FONT_COUNT 2

/* undices for font registry */
#define FLURMP_FONT_VERA 0
#define FLURMP_FONT_COUSINE 1

/**
 * Creates a new glyph.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_resource - a font resource
 *   char - a character to be represented by the resulting glyph
 */
fl_glyph* fl_create_glyph(fl_context* context, fl_resource* res, char c);

/**
 * Frees the resources allocated for a glyph.
 *
 * Params:
 *   fl_glyph - a glyph
 */
void fl_destroy_glyph(fl_glyph* glyph);

/**
 * Creates a font atlas of a specified font and font size.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_resource - a font resource
 */
fl_font_atlas* fl_create_font_atlas(fl_context* context, fl_resource* res);

/**
 * Retrieves a glyph from a font atlas that represents
 * the specified character.
 *
 * Params:
 *   fl_font_atlas - a font atlas
 *   char - the character to be represented by the resulting glyph
 */
fl_glyph* fl_char_to_glyph(fl_font_atlas* atlas, char c);

/**
 * Frees the resources allocated for a font atlas.
 *
 * Params:
 *   fl_font_atlas - a font atlas
 */
void fl_destroy_font_atlas(fl_font_atlas* atlas);

/**
 * Prepares a string of text to be rendered graphically.
 * The font, foreground color, and background color are hard coded.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_resource - a font resource
 *   const char* - a string of characters to display
 *   int - the horizontal position
 *   int - the vertical position
 *
 * Returns:
 *   fl_static_text - a new static text structure
 */
fl_static_text* fl_create_static_text(fl_context* context, fl_resource* res, const char* txt, int x, int y);

/**
 * Frees resources allocated for static text.
 *
 * Params:
 *   fl_static_text - a static text structure
 */
void fl_destroy_static_text(fl_static_text* stat);

#endif
