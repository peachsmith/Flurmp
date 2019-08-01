#ifndef FLURMP_PAUSE_SUBMENU_H
#define FLURMP_PAUSE_SUBMENU_H

#include "core/flurmp_impl.h"
#include "menu/menu.h"

/**
 * Creates a pause submenu.
 * This will be added to the pause menu.
 *
 * Params:
 *   fl_context - a Flurmp context
 *
 * Returns:
 *   fl_menu - a new menu
 */
fl_menu* fl_create_pause_submenu(fl_context* context);

#endif
