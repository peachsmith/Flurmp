#ifndef FLURMP_DATA_PANEL_H
#define FLURMP_DATA_PANEL_H

#include "core/flurmp_impl.h"

/**
 * Creates a data panel.
 *
 * Params:
 *   int - the x position
 *   int - the y position
 *   int - the width
 *   int - the height
 *   fl_font - the typeface used by the text
 *
 * Returns:
 *   fl_data_panel - a new data panel
 */
fl_data_panel* fl_create_data_panel(int x, int y, int w, int h, fl_font* font);

/**
 * Frees the memory allocated for a data panel.
 *
 * Params:
 *   fl_data_panel - a data panel
 */
void fl_destroy_data_panel(fl_data_panel* panel);

#endif
