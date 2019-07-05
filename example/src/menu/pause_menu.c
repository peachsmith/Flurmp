/**
 * A pause menu.
 * This will be rendered whenever the Flurmp context
 * is in a paused state.
 *
 * Actions:
 *   Info - writes information about the application to stdout
 *   Submenu - opens a submenu
 *   Console - opens a developer console
 *   Quit - sets the done flag to 1
 */
#include "menu.h"
#include "input.h"

#include <stdio.h>

/* amount of menu items */
#define ITEM_COUNT 4

/* determines the screen coordindates for rendering the cursor */
static void get_cursor_coords(fl_menu* context, int* x, int* y);

/* input handling */
static void input_handler(fl_context* context, fl_menu* menu);

/* menu actions */
static void info_action(fl_context* context, fl_menu* menu);
static void submenu_action(fl_context* context, fl_menu* menu);
static void console_action(fl_context* context, fl_menu* menu);
static void quit_action(fl_context* context, fl_menu* menu);

/* cursor movement */
static void cursor_up(fl_context* context, fl_menu* menu);
static void cursor_down(fl_context* context, fl_menu* menu);
static void cursor_left(fl_context* context, fl_menu* menu);
static void cursor_right(fl_context* context, fl_menu* menu);

/* select and cancel */
static void cursor_select(fl_context* context, fl_menu* menu);
static void cursor_cancel(fl_context* context, fl_menu* menu);

fl_menu* fl_create_pause_menu(fl_context* context)
{
	fl_menu* p_menu;
	fl_menu_item** items;
	fl_menu** submenus;
	fl_menu* submenu;

	/* menu items */
	fl_menu_item* info_item;
	fl_menu_item* submenu_item;
	fl_menu_item* console_item;
	fl_menu_item* quit_item;

	p_menu = fl_create_menu(50, 50, 300, 200);

	if (p_menu == NULL)
		return NULL;

	/* Allocate memory for the menu items. */
	items = (fl_menu_item**)malloc(sizeof(fl_menu_item*) * ITEM_COUNT);

	/* Verify menu item memory allocation. */
	if (items == NULL)
	{
		fl_destroy_menu(p_menu);
		return NULL;
	}

	/* Create the Info item. */
	info_item = fl_create_menu_item(context, 80, 60, "Info", info_action);

	/* Verify Info item creation. */
	if (info_item == NULL)
	{
		free(items);
		fl_destroy_menu(p_menu);
		return NULL;
	}

	/* Create the Submenu item. */
	submenu_item = fl_create_menu_item(context, 80, 90, "Submenu", submenu_action);

	/* Verify the Submenu item creation. */
	if (submenu_item == NULL)
	{
		fl_destroy_menu_item(info_item);
		free(items);
		fl_destroy_menu(p_menu);
		return NULL;
	}

	/* Create the Console item. */
	console_item = fl_create_menu_item(context, 80, 120, "Console", console_action);

	/* Verify the Console item creation. */
	if (console_item == NULL)
	{

		fl_destroy_menu_item(info_item);
		fl_destroy_menu_item(submenu_item);
		free(items);
		fl_destroy_menu(p_menu);
		return NULL;
	}

	/* Create the Quit item. */
	quit_item = fl_create_menu_item(context, 80, 150, "Quit", quit_action);
	
	/* Verify the Quit item creation. */
	if (quit_item == NULL)
	{
		fl_destroy_menu_item(info_item);
		fl_destroy_menu_item(submenu_item);
		fl_destroy_menu_item(console_item);
		free(items);
		fl_destroy_menu(p_menu);
		return NULL;
	}

	/* Populate the menu item array. */
	items[0] = info_item;
	items[1] = submenu_item;
	items[2] = console_item;
	items[3] = quit_item;

	p_menu->items = items;
	p_menu->item_count = ITEM_COUNT;

	/* Allocate memory for a submenu array. */
	submenus = (fl_menu**)malloc(sizeof(fl_menu*));

	/* Verify submenu array allocation. */
	if (submenus == NULL)
	{
		fl_destroy_menu(p_menu);
		return NULL;
	}

	/* Create the submenu. */
	submenu = fl_create_pause_submenu(context);

	/* Verify submenu creation. */
	if (submenu == NULL)
	{
		free(submenus);
		fl_destroy_menu(p_menu);
		return NULL;
	}

	/* Set the submenu's parent menu. */
	submenu->parent = p_menu;

	/* Populate the submenu array. */
	submenus[0] = submenu;

	/* Populate the remaining menu fields. */
	p_menu->submenus = submenus;
	p_menu->submenu_count = 1;
	p_menu->input_handler = input_handler;
	p_menu->get_cursor_coords = get_cursor_coords;

	return p_menu;
}




static void get_cursor_coords(fl_menu* menu, int* x, int* y)
{
	if (menu->child != NULL)
	{
		menu->get_cursor_coords(menu->child, x, y);
		return;
	}

	*x = 60;
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
		cursor_left(context, menu);
	}

	if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_S))
	{
		cursor_down(context, menu);
	}

	if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_D))
	{
		cursor_right(context, menu);
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
		context->paused = 0;
		menu->open = 0;
	}
}


/* menu actions */

static void info_action(fl_context* context, fl_menu* menu)
{
	if (!context->dialogs[0]->open)
	{
		context->dialogs[0]->open = 1;
		context->active_dialog = context->dialogs[0];
	}
}

static void submenu_action(fl_context* context, fl_menu* menu)
{
	menu->submenus[0]->open = 1;
	menu->child = menu->submenus[0];
}

static void console_action(fl_context* context, fl_menu* menu)
{
	context->console_open = 1;
}

static void quit_action(fl_context* context, fl_menu* menu)
{
	/* Later, we may want to add some sort of confirmation. */
	context->done = 1;
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
	menu->pos = 0;
	menu->open = 0;
	context->paused = 0;
}
