#ifndef FLURMP_DATA_PANEL_H
#define FLURMP_DATA_PANEL_H

#include "flurmp_impl.h"

fl_data_panel* fl_create_data_panel(int x, int y, int w, int h, fl_font_atlas* atlas);

void fl_destroy_data_panel(fl_data_panel* panel);

#endif
