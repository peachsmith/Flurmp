#ifndef CONSOLE_H
#define CONSOLE_H

#include "flurmp_impl.h"

#define FLURMP_CONSOLE_MOD_SHIFT 0x01
#define FLURMP_CONSOLE_MOD_CTRL  0x02

/**
 * Creates a console.
 *
 * Params:
 *   fl_context - a Flurmp context
 *
 * Returns:
 *   fl_console - a newly created console
 */
fl_console* fl_create_console(fl_context* context);

/**
 * Frees the memory allocated for a console.
 *
 * Params:
 *   fl_console - a console
 */
void fl_destroy_console(fl_console* console);

/**
 * Appends a character to the current console command buffer.
 *
 * Params:
 *   fl_console - a console
 *   char - a character
 *   unsigned char - a byte containing modifier flags that indicate
 *     when key combinations have been pressed
 */
void fl_putc(fl_console* console, char c, unsigned char mod);

/**
 * Submits the current command buffer for a console.
 * An action is taken based on the contents of the buffer at the
 * time of submission.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_console - a console
 */
void submit_buffer(fl_context* context, fl_console* console);

/**
 * Convert a keyboard scancode to a character for output.
 *
 * This method also takes a reference to an integer that can be
 * used to keep track of which keys have been pressed.
 *
 * It also takes a byte containing bit flags indicating key
 * combinations (e.g. Ctrl, Shift, etc.)
 *
 * Returns:
 *   char - a character to be placed in a buffer
 *
 * Params:
 *   sc   - the scancode
 *   flag - a reference to an integer containing response flags
 *   mod  - a byte containing flags indicating key combinations
 */
char fl_sc_to_char(int sc, unsigned char mod);

#endif
