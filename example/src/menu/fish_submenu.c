#include "menu.h"
#include "input.h"

#include <stdio.h>

/* amount of menu items */
#define ITEM_COUNT 2

/**
 * Gets the cursor coordinates for the fish submenu.
 *
 * Params:
 *   fl_menu - a menu
 *   int* - a pointer to the cursor's x position
 *   int* - a pointer to the cursor's y position
 */
static void get_cursor_coords(fl_menu*, int*, int*);

/**
 * Handles input for the fish submenu.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_input_handler -
 */
static void handle_input(fl_context*, fl_input_handler*);

/**
 * Renders the fish menu to the screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void render(fl_context*, fl_menu*);

/**
 * Moves the cursor upward.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void cursor_up(fl_context*, fl_menu*);

/**
 * Moves the cursor downward.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void cursor_down(fl_context*, fl_menu*);

/**
 * Selects a menu item based on current cursor position.
 * Selecting a menu item should call that item's callback function.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void cursor_select(fl_context*, fl_menu*);

/**
 * Cancels the current action.
 * Cancelling a menu closes it. If the menu has affirmative and negative
 * results, then the default result should be negative.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void cursor_cancel(fl_context*, fl_menu*);

/**
 * The tuna action.
 * This function will write message "This is the tuna action" to stdout.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void tuna_action(fl_context*, fl_menu*);

/**
 * The salmon action.
 * This function will write message "This is the salmon action" to stdout.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void salmon_action(fl_context*, fl_menu*);





static void get_cursor_coords(fl_menu* menu, int* x, int* y)
{
	*x = 250;
	*y = menu->pos * 30 + menu->y + 10;
}

static void handle_input(fl_context* context, fl_input_handler* self)
{
	fl_menu* menu = fl_get_active_menu(context);

	if (fl_consume_key(context, FLURMP_SC_W))
	{
		cursor_up(context, menu);
		return;
	}

	if (fl_consume_key(context, FLURMP_SC_S))
	{
		cursor_down(context, menu);
		return;
	}

	if (fl_consume_key(context, FLURMP_SC_J))
	{
		cursor_select(context, menu);
		return;
	}

	if (fl_consume_key(context, FLURMP_SC_K))
	{
		cursor_cancel(context, menu);
		return;
	}

	if (fl_consume_key(context, FLURMP_SC_ESCAPE))
	{
		/* Relenquish input control */
		fl_pop_input_handler(context);

		/* Remove the current active menu */
		fl_menu* menu = fl_pop_menu(context);

		fl_destroy_menu(menu);
	}
}

static void render(fl_context* context, fl_menu* menu)
{
	int i;
	SDL_Rect r;

	r.x = menu->x;
	r.y = menu->y;
	r.w = menu->w;
	r.h = menu->h;

	SDL_SetRenderDrawColor(context->renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(context->renderer, &r);

	SDL_SetRenderDrawColor(context->renderer, 250, 250, 250, 255);
	SDL_RenderDrawRect(context->renderer, &r);

	if (menu->items != NULL)
	{
		for (i = 0; i < menu->item_count; i++)
		{
			r.x = menu->items[i]->x;
			r.y = menu->items[i]->y;
			r.w = menu->items[i]->image->surface->w;
			r.h = menu->items[i]->image->surface->h;
			SDL_RenderCopy(context->renderer, menu->items[i]->image->texture, NULL, &r);
		}
	}

	if (menu->child == NULL)
	{
		menu->get_cursor_coords(menu, &r.x, &r.y);
		r.w = 10;
		r.h = 10;

		SDL_RenderFillRect(context->renderer, &r);
	}
	else
	{
		menu->child->render(context, menu->child);
	}
}



/* -------------------------------------------------------------- */
/*                      Cursor Operations                         */
/* -------------------------------------------------------------- */

static void cursor_up(fl_context* context, fl_menu* menu)
{
	if (menu->pos > 0)
		menu->pos--;
}

static void cursor_down(fl_context* context, fl_menu* menu)
{
	if (menu->pos < 1)
		menu->pos++;
}

static void cursor_select(fl_context* context, fl_menu* menu)
{
	menu->items[menu->pos]->action(context, menu);
}

static void cursor_cancel(fl_context* context, fl_menu* menu)
{
	/* Relenquish input control */
	fl_pop_input_handler(context);

	/* Remove the current active menu */
	fl_menu* active = fl_pop_menu(context);

	fl_destroy_menu(active);
}



/* -------------------------------------------------------------- */
/*                      Menu Item Actions                         */
/* -------------------------------------------------------------- */

static void tuna_action(fl_context* context, fl_menu* menu)
{
	printf("This is the tuna action.\n");
}

static void salmon_action(fl_context* context, fl_menu* menu)
{
	printf("This is the salmon action.\n");
}



/* -------------------------------------------------------------- */
/*                     menu.h implementation                      */
/* -------------------------------------------------------------- */

fl_menu* fl_create_fish_submenu(fl_context* context)
{
	fl_menu* menu;
	fl_menu_item** items;

	fl_menu_item* bagel_item;
	fl_menu_item* toast_item;

	menu = fl_create_menu(240, 140, 140, 90);

	if (menu == NULL)
	{
		return NULL;
	}

	bagel_item = fl_create_menu_item(context, 280, 150, "Tuna", tuna_action);

	if (bagel_item == NULL)
	{
		fl_destroy_menu(menu);
		return NULL;
	}

	toast_item = fl_create_menu_item(context, 280, 180, "Salmon", salmon_action);

	if (toast_item == NULL)
	{
		fl_destroy_menu_item(bagel_item);
		fl_destroy_menu(menu);

		return NULL;
	}

	items = fl_alloc(fl_menu_item*, ITEM_COUNT);

	if (items == NULL)
	{
		fl_destroy_menu_item(bagel_item);
		fl_destroy_menu_item(toast_item);
		fl_destroy_menu(menu);

		return NULL;
	}

	items[0] = bagel_item;
	items[1] = toast_item;

	menu->items = items;
	menu->item_count = ITEM_COUNT;

	menu->input_handler = fl_create_input_handler(handle_input);
	menu->render = render;
	menu->get_cursor_coords = get_cursor_coords;

	return menu;
}
