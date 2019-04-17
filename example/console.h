#ifndef CONSOLE_H
#define CONSOLE_H

#include "flurmp.h"

#define FLURMP_CONSOLE_MOD_SHIFT 0x01
#define FLURMP_CONSOLE_MOD_CTRL  0x02

console_t* fl_create_console(fl_context* context);

void fl_destroy_console(console_t* console);

void fl_render_console(fl_context* context, console_t* console);

void fl_putc(console_t* console, char c, unsigned char mod);

void fl_print(console_t* console, const char* s);

void submit_buffer(fl_context* context, console_t* console);

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
char fl_sc_to_char(int sc, int* flag, unsigned char mod);

#endif
