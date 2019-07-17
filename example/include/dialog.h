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
 * Creates a new dialog and writes a string of text to its buffer.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   const char* - a string of text
 *   int - the rate at which the individual characters should appear
 *   void(*callback) (fl_context*) - a callback function
 *   int - 1 if the dialog should remain and be destroyed later otherwise 0
 */
void fl_dialog_write(fl_context* context,
	const char* msg,
	int speed,
	void(*callback) (fl_context*),
	int hold);

#endif