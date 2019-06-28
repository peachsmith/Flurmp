#include "menu.h"
#include "input.h"

#include <stdio.h>

static void info_action(fl_context* context, fl_menu* menu);
static void submenu_action(fl_context* context, fl_menu* menu);
static void quit_action(fl_context* context, fl_menu* menu);
static void pause_menu_input_handler(fl_context* context, fl_menu* menu);
static void cursor_up(fl_context* context, fl_menu* menu);
static void cursor_down(fl_context* context, fl_menu* menu);
static void cursor_left(fl_context* context, fl_menu* menu);
static void cursor_right(fl_context* context, fl_menu* menu);
static void cursor_select(fl_context* context, fl_menu* menu);
static void cursor_cancel(fl_context* context, fl_menu* menu);
static void get_pause_menu_cursor_coords(fl_menu* context, int* x, int* y);

fl_menu* fl_create_pause_menu(fl_context* context)
{
	fl_menu* p_menu;
	fl_menu_item** items;
	//fl_menu** submenus;

	fl_menu_item* info_item;
	fl_menu_item* submenu_item;
	fl_menu_item* quit_item;

	//fl_menu* submenu;

	p_menu = fl_create_menu(50, 50, 300, 200);

	if (p_menu == NULL)
		return NULL;

	/* allocate memory for 3 items */
	items = (fl_menu_item**)malloc(sizeof(fl_menu_item*) * 3);

	if (items == NULL)
	{
		fl_destroy_menu(p_menu);
		return NULL;
	}

	info_item = fl_create_menu_item(context, 80, 60, 16, "Info", info_action);

	if (info_item == NULL)
	{
		free(items);
		fl_destroy_menu(p_menu);
		return NULL;
	}

	submenu_item = fl_create_menu_item(context, 80, 90, 16, "Submenu", submenu_action);

	if (submenu_item == NULL)
	{
		fl_destroy_menu_item(info_item);
		free(items);
		fl_destroy_menu(p_menu);
		return NULL;
	}

	quit_item = fl_create_menu_item(context, 80, 120, 16, "Quit", quit_action);

	if (quit_item == NULL)
	{
		fl_destroy_menu_item(submenu_item);
		fl_destroy_menu_item(info_item);
		free(items);
		fl_destroy_menu(p_menu);
		return NULL;
	}

	items[0] = info_item;
	items[1] = submenu_item;
	items[2] = quit_item;

	p_menu->items = items;
	p_menu->item_count = 3;

	/* allocate memory for 1 submenu for now */
	//submenus = (fl_menu**)malloc(sizeof(fl_menu*));

	//if (submenus == NULL)
	//{
	//	fl_destroy_menu(p_menu);
	//	return NULL;
	//}

	//submenu = fl_create_menu(80, 75, 100, 100);

	//if (submenu == NULL)
	//{
	//	free(submenus);
	//	fl_destroy_menu(p_menu);
	//	return NULL;
	//}

	p_menu->input_handler = pause_menu_input_handler;
	p_menu->get_cursor_coords = get_pause_menu_cursor_coords;

	return p_menu;
}


static void info_action(fl_context* context, fl_menu* menu)
{
	printf("This is the info action\n");
}

static void submenu_action(fl_context* context, fl_menu* menu)
{
	printf("This is the submenu action\n");
}

static void quit_action(fl_context* context, fl_menu* menu)
{
	printf("This is the quit action\n");
}

static void pause_menu_input_handler(fl_context* context, fl_menu* menu)
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
}

static void cursor_up(fl_context* context, fl_menu* menu)
{
	if (menu->pos > 0)
		menu->pos--;
}

static void cursor_down(fl_context* context, fl_menu* menu)
{
	if (menu->pos < 2)
		menu->pos++;
}

static void cursor_left(fl_context* context, fl_menu* menu)
{

}

static void cursor_right(fl_context* context, fl_menu* menu)
{

}

static void cursor_select(fl_context* context, fl_menu* menu)
{
	menu->items[menu->pos]->action(context, menu);
}

static void cursor_cancel(fl_context* context, fl_menu* menu)
{
	menu->active = 0;
	menu->open = 0;
	context->paused = 0;
}

static void get_pause_menu_cursor_coords(fl_menu* menu, int* x, int* y)
{
	*x = 60;
	*y = menu->pos * 30 + menu->y + 10;
}
