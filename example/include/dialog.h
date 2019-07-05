#ifndef FLURMP_DIALOG_H
#define FLURMP_DIALOG_H

#include "flurmp_impl.h"

/* Common Dialog Functions */

fl_dialog* fl_create_dialog(fl_resource* font, int x, int y, int w, int h);

void fl_destroy_dialog(fl_dialog*);

/* Example Dialog Functions */
fl_dialog* fl_create_example_dialog(fl_resource* font, int x, int y, int w, int h);

#endif