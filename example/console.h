#ifndef CONSOLE_H
#define CONSOLE_H

#include "flurmp.h"

console_t* fl_create_console();

void fl_destroy_console(console_t* console);

void fl_render_console(fl_context* context, console_t* console);

void fl_print(console_t* console, const char* s);

#endif
