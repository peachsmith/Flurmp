#ifndef CONSOLE_H
#define CONSOLE_H

#include "core/flurmp_impl.h"

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

#endif
