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

static const int item_count = 3;

static void get_cursor_coords(fl_menu* context, int* x, int* y);

static void input_handler(fl_context* context, fl_menu* menu);

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

	fl_menu** submenus;
	fl_menu* submenu;

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

	items = (fl_menu_item**)malloc(sizeof(fl_menu_item*) * item_count);

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

	/* allocate memory for 1 submenu for now */
	submenus = (fl_menu**)malloc(sizeof(fl_menu*));

	if (submenus == NULL)
	{
		fl_destroy_menu(menu);
		return NULL;
	}

	submenu = fl_create_fish_submenu(context);

	if (submenu == NULL)
	{
		free(submenus);
		fl_destroy_menu(menu);
		return NULL;
	}

	submenu->parent = menu;

	submenus[0] = submenu;

	menu->submenus = submenus;
	menu->submenu_count = 1;
	menu->items = items;
	menu->item_count = item_count;
	menu->input_handler = input_handler;
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

static void input_handler(fl_context* context, fl_menu* menu)
{
	if (menu->child != NULL)
	{
		menu->child->input_handler(context, menu->child);
		return;
	}

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
	menu->submenus[0]->open = 1;
	menu->child = menu->submenus[0];
}


/* cursor movement */

static void cursor_up(fl_context* context, fl_menu* menu)
{
	if (menu->pos > 0)
		menu->pos--;
}

static void cursor_down(fl_context* context, fl_menu* menu)
{
	if (menu->pos < item_count - 1)
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
