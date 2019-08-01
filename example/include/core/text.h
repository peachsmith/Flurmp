#ifndef FLURMP_TEXT_H
#define FLURMP_TEXT_H

#include "core/flurmp_impl.h"

/* total number of fonts available to the application */
#define FLURMP_FONT_COUNT 3

/* indices for font registry */
#define FLURMP_FONT_VERA 0
#define FLURMP_FONT_COUSINE 1
#define FLURMP_FONT_KARMILLA_BOLD 2

/**
 * Creates a new glyph.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_resource - a font resource
 *   char - a character to be represented by the resulting glyph
 */
fl_image* fl_create_glyph(fl_context * context, fl_resource * res, char c);

/**
 * Creates a font atlas of a specified font and font size.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_resource - a font resource
 */
int fl_create_font_atlas(fl_context* context, fl_resource* res);

/**
 * Retrieves a glyph from a font atlas that represents
 * the specified character.
 *
 * Params:
 *   fl_font - a font
 *   char - the character to be represented by the resulting glyph
 */
fl_image* fl_char_to_glyph(fl_font* font, char c);


/**
 * Creates an image containing text that doesn't change.
 * The resulting image can be destroyed with the fl_destroy_image function.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_resource - a font resource
 *   const char* - a string of characters to display
 *
 * Returns:
 *   fl_static_text - a new static text structure
 */
fl_image* fl_create_static_text(fl_context* context,
	fl_resource* res,
	const char* txt);

#endif
