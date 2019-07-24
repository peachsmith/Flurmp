#include "menu.h"
#include "input.h"

/* amount of menu items */
#define ITEM_COUNT 3

/**
 * Gets the cursor coordinates for the pause submenu.
 *
 * Params:
 *   fl_menu - a menu
 *   int* - a pointer to the cursor's x position
 *   int* - a pointer to the cursor's y position
 */
static void get_cursor_coords(fl_menu*, int*, int*);

/**
 * Handles input for the pause submenu.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_input_handler -
 */
static void handle_input(fl_context*, fl_input_handler*);

/**
 * Renders the pause submenu to the screen.
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
 * The bagel action.
 * This function will write message "This is the bagel action" to stdout.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void bagel_action(fl_context*, fl_menu*);

/**
 * The toast action.
 * This function will write message "This is the toast action" to stdout.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void toast_action(fl_context*, fl_menu*);

/**
 * The fish action.
 * This function will open the fish submenu.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void fish_action(fl_context*, fl_menu*);





static void get_cursor_coords(fl_menu* menu, int* x, int* y)
{
	if (menu->child != NULL)
	{
		menu->get_cursor_coords(menu->child, x, y);
		return;
	}

	*x = 210;
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

static void bagel_action(fl_context* context, fl_menu* menu)
{
	printf("This is the bagel action\n");
}

static void toast_action(fl_context* context, fl_menu* menu)
{
	printf("This is the toast action\n");
}

static void fish_action(fl_context* context, fl_menu* menu)
{
	fl_menu* submenu = fl_create_fish_submenu(context);

	fl_push_menu(context, submenu);
	fl_push_input_handler(context, submenu->input_handler);
}



/* -------------------------------------------------------------- */
/*                     menu.h implementation                      */
/* -------------------------------------------------------------- */

fl_menu* fl_create_pause_submenu(fl_context* context)
{
	fl_menu* menu;
	fl_menu_item** items;

	fl_menu_item* bagel_item;
	fl_menu_item* toast_item;
	fl_menu_item* fish_item;

	menu = fl_create_menu(200, 100, 100, 120);

	if (menu == NULL)
	{
		return NULL;
	}

	/* Create the Bagel item. */
	bagel_item = fl_create_menu_item(context, 240, 110, "Bagel", bagel_action);

	/* Verify the Bagel item creation. */
	if (bagel_item == NULL)
	{
		fl_destroy_menu(menu);
		return NULL;
	}

	/* Create the Toast item. */
	toast_item = fl_create_menu_item(context, 240, 140, "Toast", toast_action);

	/* Verify the Toast item creation. */
	if (toast_item == NULL)
	{
		fl_destroy_menu_item(bagel_item);
		fl_destroy_menu(menu);

		return NULL;
	}

	/* Create the Fish item. */
	fish_item = fl_create_menu_item(context, 240, 170, "Fish", fish_action);

	/* Verify the Fish item creation. */
	if (fish_item == NULL)
	{
		fl_destroy_menu_item(bagel_item);
		fl_destroy_menu_item(toast_item);
		fl_destroy_menu(menu);

		return NULL;
	}

	items = fl_alloc(fl_menu_item*, ITEM_COUNT);

	if (items == NULL)
	{
		fl_destroy_menu_item(bagel_item);
		fl_destroy_menu_item(toast_item);
		fl_destroy_menu_item(fish_item);
		fl_destroy_menu(menu);

		return NULL;
	}

	/* Populate the menu item array. */
	items[0] = bagel_item;
	items[1] = toast_item;
	items[2] = fish_item;

	menu->items = items;
	menu->item_count = ITEM_COUNT;

	menu->input_handler = fl_create_input_handler(handle_input);
	menu->render = render;
	menu->get_cursor_coords = get_cursor_coords;

	return menu;
}
