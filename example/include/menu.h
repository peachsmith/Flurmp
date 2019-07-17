#ifndef FLURMP_MENU_H
#define FLURMP_MENU_H

#include "flurmp_impl.h"

/**
 * Creates a menu.
 *
 * Params:
 *   int - the x position of the menu
 *   int - the y position of the menu
 *   int - the width of the menu
 *   int - the height of the menu
 *
 * Returns:
 *   fl_menu - a new menu
 */
fl_menu* fl_create_menu(int x, int y, int w, int h);

/**
 * Frees the memory allocated for a menu.
 *
 * Params:
 *   fl_menu - a menu
 */
void fl_destroy_menu(fl_menu* menu);

/**
 * Creates a menu item.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   int - the x position of the menu item
 *   int - the y position of the menu item
 *   const char* - the text of the menu item
 *   void(*action) (fl_context*, fl_menu*) - the action to
 *       perform when the menu item is selected.
 *
 * Returns:
 *   fl_menu_item - a new menu item
 */
fl_menu_item* fl_create_menu_item(fl_context* context,
	int x,
	int y,
	const char* text,
	void(*action) (fl_context*, fl_menu*));

/**
 * Frees the memory allocated for a menu item.
 *
 * Params:
 *   fl_menu_item - a menu item
 */
void fl_destroy_menu_item(fl_menu_item* item);

/**
 * Retrieves a pointer to the current active menu.
 *
 * Params:
 *   fl_context - a Flurmp context
 *
 * Returns:
 *   fl_menu - a pointer to the active menu
 */
fl_menu* fl_get_active_menu(fl_context* context);

/**
 * Appends a menu to the current context.
 * The most recently appended menu becomes the active menu.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
void fl_push_menu(fl_context* context, fl_menu* menu);

/**
 * Removes the most recently added menu from a context.
 * Upon removal, the menu's parent becomes the active menu.
 * This does NOT destroy the menu that was removed.
 * It returns a pointer to the removed menu, and it is the
 * responsibility of the caller of this function to handle
 * menu disposal.
 *
 * Params:
 *   fl_context - a Flurmp context
 *
 * Returns:
 *   fl_menu - a pointer to the menu that was just removed.
 */
fl_menu* fl_pop_menu(fl_context* context);

/**
 * Creates a pause menu.
 *
 * Params:
 *   fl_context - a Flurmp context
 *
 * Returns:
 *   fl_menu - a new menu
 */
fl_menu* fl_create_pause_menu(fl_context* context);

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

/**
 * Creates a fish submenu.
 * This will be added as a submenu of the pause submenu.
 *
 * Params:
 *   fl_context - a Flurmp context
 *
 * Returns:
 *   fl_menu - a new menu
 */
fl_menu* fl_create_fish_submenu(fl_context* context);

/**
 * Creates a confirmation menu.
 * A confirmation menu should have an affirmative and a negative option.
 *
 * Params:
 *   fl_context - a Flurmp context
 *
 * Returns:
 *   fl_menu - a new menu
 */
fl_menu* fl_create_confirmation_menu(fl_context* context);

#endif
