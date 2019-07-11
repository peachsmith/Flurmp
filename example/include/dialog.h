#ifndef FLURMP_DIALOG_H
#define FLURMP_DIALOG_H

#include "flurmp_impl.h"

/**
 * Creates a new dialog.
 *
 * Params:
 *   fl_resource - a font
 *   int - horizontal position
 *   int - vertical position
 *   int - width
 *   int - height
 *
 * Returns:
 *   fl_dialog - a new dialog
 */
fl_dialog* fl_create_dialog(fl_context* context);

/**
 * Frees the memory allocated for a dialog.
 *
 * Params:
 *   fl_dialog - a dialog
 */
void fl_destroy_dialog(fl_dialog* context);

/**
 * Writes a single character to a dialog buffer.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   char - a character
 */
void fl_dialog_putc(fl_context* context, char c);

/**
 * Writes a string of text to a dialog buffer.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   const char* - a string of text
 *   int - the rate at which the individual characters should appear
 */
void fl_dialog_write(fl_context* context, const char* msg, int speed);

/* TODO: improve dialog interface.
         add function to output text and take speed as an argument. */

#endif