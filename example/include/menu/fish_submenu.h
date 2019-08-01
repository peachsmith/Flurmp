#ifndef FLURMP_FISH_SUBMENU_H
#define FLURMP_FISH_SUBMENU_H

#include "core/flurmp_impl.h"
#include "menu/menu.h"

/**
 * Creates a fish submenu.
 * The fish submenu will have two options: "Tuna", and "Salmon".
 * These options will print a message to stdout.
 * The purpose of the fish submenu is to demonstrate the
 * use of submenus.
 *
 * Params:
 *   fl_context - a Flurmp context
 *
 * Returns:
 *   fl_menu - a new menu
 */
fl_menu* fl_create_fish_submenu(fl_context* context);

#endif
