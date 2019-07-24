#include "menu.h"
#include "input.h"

/* amount of menu items */
#define ITEM_COUNT 2

/**
 * Gets the cursor coordinates for the confirmation menu
 *
 * Params:
 *   fl_menu - a menu
 *   int* - a pointer to the cursor's x position
 *   int* - a pointer to the cursor's y position
 */
static void get_cursor_coords(fl_menu*, int*, int*);

/**
 * Handles input for the confirmaiton menu.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_input_handler -
 */
static void handle_input(fl_context*, fl_input_handler*);

/**
 * Renders the confirmation menu to the screen.
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
 * The yes action.
 * This function will set the context ret_val to 1
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void yes_action(fl_context*, fl_menu*);

/**
 * The no action.
 * This function will set the context ret_val to 2
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void no_action(fl_context*, fl_menu*);


static void get_cursor_coords(fl_menu* menu, int* x, int* y)
{
	*x = 310;
	*y = menu->pos * 30 + menu->y + 18;
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
}

static void render(fl_context* context, fl_menu* menu)
{
	int i;
	fl_rect r;

	r.x = menu->x;
	r.y = menu->y;
	r.w = menu->w;
	r.h = menu->h;

	fl_set_draw_color(context, 0, 0, 0, 255);
	fl_draw_solid_rect(context, &r);
	fl_set_draw_color(context, 250, 250, 250, 255);
	fl_draw_rect(context, &r);

	if (menu->items != NULL)
	{
		for (i = 0; i < menu->item_count; i++)
		{
			r.x = menu->items[i]->x;
			r.y = menu->items[i]->y;
			r.w = menu->items[i]->image->w;
			r.h = menu->items[i]->image->h;
			fl_draw(context, menu->items[i]->image->texture, NULL, &r, 0);
		}
	}

	if (menu->child == NULL)
	{
		menu->get_cursor_coords(menu, &r.x, &r.y);
		r.w = 10;
		r.h = 10;

		fl_draw_solid_rect(context, &r);
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
	if (menu->pos < ITEM_COUNT - 1)
		menu->pos++;
}

/* select and cancel */

static void cursor_select(fl_context* context, fl_menu* menu)
{
	menu->items[menu->pos]->action(context, menu);
}

static void cursor_cancel(fl_context* context, fl_menu* menu)
{
	no_action(context, menu);
}



/* -------------------------------------------------------------- */
/*                      Menu Item Actions                         */
/* -------------------------------------------------------------- */

static void yes_action(fl_context* context, fl_menu* menu)
{
	context->ret_val = 1;

	/* Retrieve a pointer to the active menu. */
	fl_menu* active = fl_get_active_menu(context);

	void(*callback)(fl_context*) = active->callback;

	/* Relenquish input control */
	fl_pop_input_handler(context);

	/* Remove the current active menu */
	active = fl_pop_menu(context);

	fl_destroy_menu(active);

	if (callback != NULL)
	{
		context->ret_val = 1;
		callback(context);
	}
}

static void no_action(fl_context* context, fl_menu* menu)
{
	context->ret_val = 2;

	/* Retrieve a pointer to the active menu. */
	fl_menu* active = fl_get_active_menu(context);

	void(*callback)(fl_context*) = active->callback;

	/* Relenquish input control */
	fl_pop_input_handler(context);

	/* Remove the current active menu */
	active = fl_pop_menu(context);

	fl_destroy_menu(active);

	if (callback != NULL)
	{
		context->ret_val = 2;
		callback(context);
	}
}



/* -------------------------------------------------------------- */
/*                     menu.h implementation                      */
/* -------------------------------------------------------------- */

fl_menu* fl_create_confirmation_menu(fl_context* context)
{
	fl_menu* menu;
	fl_menu_item** items;

	/* menu items */
	fl_menu_item* yes_item;
	fl_menu_item* no_item;

	menu = fl_create_menu(300, 100, 120, 80);

	if (menu == NULL)
		return NULL;

	/* Allocate memory for the menu items. */
	items = fl_alloc(fl_menu_item*, ITEM_COUNT);

	/* Verify menu item memory allocation. */
	if (items == NULL)
	{
		fl_destroy_menu(menu);
		return NULL;
	}

	/* Create the Info item. */
	yes_item = fl_create_menu_item(context, 330, 115, "Yes", yes_action);

	/* Verify Info item creation. */
	if (yes_item == NULL)
	{
		fl_free(items);
		fl_destroy_menu(menu);
		return NULL;
	}

	/* Create the Submenu item. */
	no_item = fl_create_menu_item(context, 330, 145, "No", no_action);

	/* Verify the Submenu item creation. */
	if (no_item == NULL)
	{
		fl_destroy_menu_item(yes_item);
		fl_free(items);
		fl_destroy_menu(menu);
		return NULL;
	}

	/* Populate the menu item array. */
	items[0] = yes_item;
	items[1] = no_item;

	menu->items = items;
	menu->item_count = ITEM_COUNT;

	menu->input_handler = fl_create_input_handler(handle_input);

	menu->render = render;
	menu->get_cursor_coords = get_cursor_coords;

	return menu;
}
