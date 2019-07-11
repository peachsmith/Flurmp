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
#include "dialog.h"
#include "text.h"
#include "console.h"

#include <stdio.h>
#include <string.h>

 /* amount of menu items */
#define ITEM_COUNT 5

/* determines the screen coordindates for rendering the cursor */
static void get_cursor_coords(fl_menu * context, int* x, int* y);

/* input handling */
static void input_handler(fl_context* context, fl_input_handler* self);

/* render callback */
static void render(fl_context* context, fl_menu* menu);

/* menu actions */
static void info_action(fl_context* context, fl_menu* menu);
static void submenu_action(fl_context* context, fl_menu* menu);
static void console_action(fl_context* context, fl_menu* menu);
static void quit_action(fl_context* context, fl_menu* menu);
static void confirmation_action(fl_context* context, fl_menu* menu);

/* cursor movement */
static void cursor_up(fl_context* context, fl_menu* menu);
static void cursor_down(fl_context* context, fl_menu* menu);
static void cursor_left(fl_context* context, fl_menu* menu);
static void cursor_right(fl_context* context, fl_menu* menu);

/* select and cancel */
static void cursor_select(fl_context* context, fl_menu* menu);
static void cursor_cancel(fl_context* context, fl_menu* menu);

/* dialog callbacks */
static void first_cb(fl_context* context, fl_dialog* self);
static void second_cb(fl_context* context, fl_dialog* self);

fl_menu* fl_create_pause_menu(fl_context* context)
{
	fl_menu* p_menu;
	fl_menu_item** items;

	/* menu items */
	fl_menu_item* info_item;
	fl_menu_item* submenu_item;
	fl_menu_item* console_item;
	fl_menu_item* quit_item;
	fl_menu_item* confirmation_item;

	p_menu = fl_create_menu(50, 50, 300, 200);

	if (p_menu == NULL)
		return NULL;

	/* Allocate memory for the menu items. */
	items = fl_alloc(fl_menu_item*, ITEM_COUNT);

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
		fl_free(items);
		fl_destroy_menu(p_menu);
		return NULL;
	}

	/* Create the Submenu item. */
	submenu_item = fl_create_menu_item(context, 80, 90, "Submenu", submenu_action);

	/* Verify the Submenu item creation. */
	if (submenu_item == NULL)
	{
		fl_destroy_menu_item(info_item);
		fl_free(items);
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
		fl_free(items);
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
		fl_free(items);
		fl_destroy_menu(p_menu);
		return NULL;
	}

	/* Create the Confirmation item. */
	confirmation_item = fl_create_menu_item(context, 200, 60, "Confirmation", confirmation_action);

	/* Verify the Quit item creation. */
	if (confirmation_item == NULL)
	{
		fl_destroy_menu_item(info_item);
		fl_destroy_menu_item(submenu_item);
		fl_destroy_menu_item(console_item);
		fl_destroy_menu_item(quit_item);
		fl_free(items);
		fl_destroy_menu(p_menu);
		return NULL;
	}

	/* Populate the menu item array. */
	items[0] = info_item;
	items[1] = submenu_item;
	items[2] = console_item;
	items[3] = quit_item;
	items[4] = confirmation_item;

	p_menu->items = items;
	p_menu->item_count = ITEM_COUNT;

	/*fl_input_handler* input = fl_create_input_handler();

	input->handler = input_handler;

	p_menu->input_handler = input;*/
	p_menu->input_handler = fl_create_input_handler(input_handler);
	p_menu->render = render;
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

	if (menu->pos < 4)
		*x = 60;
	else
		*x = 180;

	if (menu->pos < 4)
		*y = menu->pos * 30 + menu->y + 10;
	else
		*y = (menu->pos - 4) * 30 + menu->y + 10;
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

		/* Relenquish input control */
		fl_pop_input_handler(context);

		/* Remove the current active menu */
		fl_menu* active = fl_pop_menu(context);

		fl_destroy_menu(active);
	}
}

static const char* statements[3] = {
	"Application: Example",
	"Purpose: Demonstration of Flurmp.",
	"Version: 1.0.0\nAuthor: John Powell"
};

static void first_cb(fl_context* context, fl_dialog* self)
{
	fl_dialog* dialog = fl_create_dialog(context);

	dialog->counter = 0;
	dialog->msg = statements[1];
	dialog->len = strlen(statements[1]);
	dialog->callback = second_cb;
	
	context->active_dialog = dialog;
	fl_push_input_handler(context, dialog->input_handler);
}

static void second_cb(fl_context* context, fl_dialog* self)
{
	fl_dialog* dialog = fl_create_dialog(context);

	dialog->counter = 0;
	dialog->msg = statements[2];
	dialog->len = strlen(statements[2]);

	context->active_dialog = dialog;
	fl_push_input_handler(context, dialog->input_handler);
}

/* menu actions */

static void info_action(fl_context* context, fl_menu* menu)
{
	fl_dialog* dialog = fl_create_dialog(context);

	dialog->counter = 0;
	dialog->msg = statements[0];
	dialog->len = strlen(statements[0]);
	dialog->callback = first_cb;

	context->active_dialog = dialog;
	fl_push_input_handler(context, dialog->input_handler);
}

static void submenu_action(fl_context* context, fl_menu* menu)
{
	fl_menu* submenu = fl_create_pause_submenu(context);

	fl_push_menu(context, submenu);
	fl_push_input_handler(context, submenu->input_handler);
}

static void console_action(fl_context* context, fl_menu* menu)
{
	fl_console* console = fl_create_console(context);

	context->console = console;
	fl_push_input_handler(context, console->input_handler);
}

static void quit_action(fl_context* context, fl_menu* menu)
{
	context->done = 1;
}

static void confirmation_action(fl_context* context, fl_menu* menu)
{
	fl_menu* confirm = fl_create_confirmation_menu(context);

	fl_push_menu(context, confirm);
	fl_push_input_handler(context, confirm->input_handler);
}


/* cursor movement */

static void cursor_up(fl_context* context, fl_menu* menu)
{
	if (menu->pos > 0 && menu->pos < 4)
		menu->pos--;
}

static void cursor_down(fl_context* context, fl_menu* menu)
{
	if (menu->pos < 3)
		menu->pos++;
}

static void cursor_left(fl_context* context, fl_menu* menu)
{
	if (menu->pos > 3)
		menu->pos = 0;
}

static void cursor_right(fl_context* context, fl_menu* menu)
{
	if (menu->pos < 4)
		menu->pos = 4;
}


/* select and cancel */

static void cursor_select(fl_context* context, fl_menu* menu)
{
	menu->items[menu->pos]->action(context, menu);
}

static void cursor_cancel(fl_context* context, fl_menu* menu)
{
	menu->pos = 0;
	context->paused = 0;

	/* Relenquish input control */
	fl_pop_input_handler(context);

	fl_menu* active = fl_pop_menu(context);

	fl_destroy_menu(active);
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
