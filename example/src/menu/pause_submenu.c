/**
 * A submenu for the pause menu.
 * This menu is meant to demonstrate the submenu functionality
 * and doesn't have any other practical purpose.
 *
 * Actions:
 *   Bagel - writes a message to stdout
 *   Toast - writes a message to stdout
 *   Fish - opens a developer console
 */
#include "menu.h"
#include "input.h"

#include <stdio.h>

#define ITEM_COUNT 3

static void get_cursor_coords(fl_menu* context, int* x, int* y);

static void input_handler(fl_context* context, fl_input_handler* self);

/* menu actions */
static void bagel_action(fl_context* context, fl_menu* menu);
static void toast_action(fl_context* context, fl_menu* menu);
static void fish_action(fl_context* context, fl_menu* menu);

/* cursor movement */
static void cursor_up(fl_context* context, fl_menu* menu);
static void cursor_down(fl_context* context, fl_menu* menu);
static void cursor_left(fl_context* context, fl_menu* menu);
static void cursor_right(fl_context* context, fl_menu* menu);

/* select and cancel */
static void cursor_select(fl_context* context, fl_menu* menu);
static void cursor_cancel(fl_context* context, fl_menu* menu);

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

	bagel_item = fl_create_menu_item(context, 240, 110, "Bagel", bagel_action);

	if (bagel_item == NULL)
	{
		fl_destroy_menu(menu);
		return NULL;
	}

	toast_item = fl_create_menu_item(context, 240, 140, "Toast", toast_action);

	if (toast_item == NULL)
	{
		fl_destroy_menu_item(bagel_item);
		fl_destroy_menu(menu);

		return NULL;
	}

	fish_item = fl_create_menu_item(context, 240, 170, "Fish", fish_action);

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

	items[0] = bagel_item;
	items[1] = toast_item;
	items[2] = fish_item;

	menu->items = items;
	menu->item_count = ITEM_COUNT;

	menu->input_handler = fl_create_input_handler(input_handler);
	menu->get_cursor_coords = get_cursor_coords;

	return menu;
}

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

static void input_handler(fl_context* context, fl_input_handler* self)
{
	if (self->child != NULL)
	{
		self->child->handler(context, self->child);
		return;
	}

	fl_menu* menu = fl_get_active_menu(context);

	if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_W))
	{
		cursor_up(context, menu);
	}

	if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_A))
	{

	}

	if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_S))
	{
		cursor_down(context, menu);
	}

	if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_D))
	{

	}

	if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_J))
	{
		cursor_select(context, menu);
	}

	if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_K))
	{
		cursor_cancel(context, menu);
	}

	if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_ESCAPE))
	{
		/* Relenquish input control */
		fl_pop_input_handler(context);

		/* Remove the current active menu */
		fl_menu* menu = fl_pop_menu(context);

		fl_destroy_menu(menu);
	}
}


/* menu actions */

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


/* cursor movement */

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

static void cursor_left(fl_context* context, fl_menu* menu)
{

}

static void cursor_right(fl_context* context, fl_menu* menu)
{

}


/* select and cancel */

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
