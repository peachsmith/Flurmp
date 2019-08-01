#ifndef FLURMP_CONFIRMATION_H
#define FLURMP_CONFIRMATION_H

#include "core/flurmp_impl.h"
#include "menu/menu.h"

/**
 * Creates a confirmation menu.
 * A confirmation menu should have an affirmative and a negative option,
 * such as "Yes" and "No".
 *
 * Params:
 *   fl_context - a Flurmp context
 *
 * Returns:
 *   fl_menu - a new menu
 */
fl_menu* fl_create_confirmation_menu(fl_context* context);

#endif
