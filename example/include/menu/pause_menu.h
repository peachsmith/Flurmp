#ifndef FLURMP_PAUSE_MENU_H
#define FLURMP_PAUSE_MENU_H

#include "core/flurmp_impl.h"
#include "menu/menu.h"

/**
 * Creates a pause menu.
 * This menu will appear when the user pauses the application.
 * It should have options allowing the user to perform actions
 * such as accessing information about the state of the application
 * or terminating the application.
 *
 * Params:
 *   fl_context - a Flurmp context
 *
 * Returns:
 *   fl_menu - a new menu
 */
fl_menu* fl_create_pause_menu(fl_context* context);

#endif
