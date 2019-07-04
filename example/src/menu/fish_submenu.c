/**
 * A submenu for a submenu.
 * This menu is meant to demonstrate the submenu functionality
 * and doesn't have any other practical purpose.
 *
  * Actions:
 *   Tuna - writes a message to stdout
 *   Salmon - writes a message to stdout
 */
#include "menu.h"
#include "input.h"

#include <stdio.h>

static void get_cursor_coords(fl_menu* context, int* x, int* y);

static void input_handler(fl_context* context, fl_menu* menu);

/* menu actions */
static void tuna_action(fl_context* context, fl_menu* menu);
static void salmon_action(fl_context* context, fl_menu* menu);

/* cursor movement */
static void cursor_up(fl_context* context, fl_menu* menu);
static void cursor_down(fl_context* context, fl_menu* menu);
static void cursor_left(fl_context* context, fl_menu* menu);
static void cursor_right(fl_context* context, fl_menu* menu);

/* select and cancel */
static void cursor_select(fl_context* context, fl_menu* menu);
static void cursor_cancel(fl_context* context, fl_menu* menu);

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

	items = (fl_menu_item * *)malloc(sizeof(fl_menu_item*) * 2);

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
	menu->item_count = 2;
	menu->input_handler = input_handler;
	menu->get_cursor_coords = get_cursor_coords;

	return menu;
}

static void get_cursor_coords(fl_menu* menu, int* x, int* y)
{
	*x = 250;
	*y = menu->pos * 30 + menu->y + 10;
}

static void input_handler(fl_context* context, fl_menu* menu)
{
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
		menu->pos = 0;
		menu->open = 0;
		menu->parent->child = NULL;
	}
}


/* menu actions */

static void tuna_action(fl_context* context, fl_menu* menu)
{
	printf("This is the tuna action.\n");
}

static void salmon_action(fl_context* context, fl_menu* menu)
{
	printf("This is the salmon action.\n");
}


/* cursor movement */

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
	menu->pos = 0;
	menu->open = 0;
	menu->parent->child = NULL;
}
