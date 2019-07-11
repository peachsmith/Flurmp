#include "menu.h"
#include "input.h"

#include <stdio.h>

/* amount of menu items */
#define ITEM_COUNT 2

/* determines the screen coordindates for rendering the cursor */
static void get_cursor_coords(fl_menu * context, int* x, int* y);

/* input handling */
static void input_handler(fl_context* context, fl_input_handler* self);

/* render callback */
static void render(fl_context* context, fl_menu* menu);

/* menu actions */
static void yes_action(fl_context* context, fl_menu* menu);
static void no_action(fl_context* context, fl_menu* menu);

/* cursor movement */
static void cursor_up(fl_context* context, fl_menu* menu);
static void cursor_down(fl_context* context, fl_menu* menu);
static void cursor_left(fl_context* context, fl_menu* menu);
static void cursor_right(fl_context* context, fl_menu* menu);

/* select and cancel */
static void cursor_select(fl_context* context, fl_menu* menu);
static void cursor_cancel(fl_context* context, fl_menu* menu);

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

	menu->input_handler = fl_create_input_handler(input_handler);

	menu->render = render;
	menu->get_cursor_coords = get_cursor_coords;

	return menu;
}


static void get_cursor_coords(fl_menu* menu, int* x, int* y)
{
	*x = 310;
	*y = menu->pos * 30 + menu->y + 18;
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

	if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_S))
	{
		cursor_down(context, menu);
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

static void yes_action(fl_context* context, fl_menu* menu)
{
	context->ret_val = 1;

	/* Relenquish input control */
	fl_pop_input_handler(context);

	/* Remove the current active menu */
	fl_menu* active = fl_pop_menu(context);

	fl_destroy_menu(active);
}

static void no_action(fl_context* context, fl_menu* menu)
{
	context->ret_val = 2;

	/* Relenquish input control */
	fl_pop_input_handler(context);

	/* Remove the current active menu */
	fl_menu* active = fl_pop_menu(context);

	fl_destroy_menu(active);
}


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
	no_action(context, menu);
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

	if (menu->child != NULL)
	{
		render(context, menu->child);
	}
}
