#include "menu.h"
#include "input.h"
#include "dialog.h"
#include "text.h"
#include "console.h"

/* amount of menu items */
#define ITEM_COUNT 5

/**
 * Dialog messages explaining information about the application.
 * These will be displayed in a dialog triggered by selecting the
 * "Info" menu item.
 */
static const char* info_msg[3] = {
	"Application: Example",
	"Purpose: Demonstration of Flurmp.",
	"Version: 1.0.0\nAuthor: John Powell"
};

/**
 * Confirmation messages.
 * These will be displayed in response to selecting an option
 * from the confirmation menu brought up by selecting the "Confirmation"
 * menu item.
 */
static const char* confirm_msg[2] = {
	"\"Yes\" was selected.",
	"\"No\" was selected."
};

/**
 * Preconfirmaiton message.
 * This will be displayed to the user before the confirmation menu is created.
 */
static const char* preconfirm = "Would you like fries with that?";

/**
 * Gets the cursor coordinates for the pause menu.
 *
 * Params:
 *   fl_menu - a menu
 *   int* - a pointer to the cursor's x position
 *   int* - a pointer to the cursor's y position
 */
static void get_cursor_coords(fl_menu*, int*, int*);

/**
 * Handles input for the pause menu.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_input_handler - an input handler
 */
static void handle_input(fl_context*, fl_input_handler*);

/**
 * Renders the pause menu to the screen.
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
 * Moves the cursor to the left.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void cursor_left(fl_context*, fl_menu*);

/**
 * Moves the cursor to the right.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void cursor_right(fl_context*, fl_menu*);

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
 * The info action callback.
 * This is called when the "Info" option is selected.
 * It displays a dialog with information about the application.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void info_action(fl_context*, fl_menu*);

/**
 * The submenu action callback.
 * This is called when the "Submenu" option is selected.
 * It creates a submenu.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void submenu_action(fl_context*, fl_menu*);

/**
 * The console action callback.
 * This is called when the "Console" option is selected.
 * It opens a developer console that allows for text input.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void console_action(fl_context*, fl_menu*);

/**
 * The quit action callback.
 * This is called when the "Quit" option is selected.
 * This function should modify the context so that the
 * fl_is_done function returns a non zero value.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void quit_action(fl_context*, fl_menu*);

/**
 * The confirmation action callback.
 * This is called when the "Confirmation" option is selected.
 * It opens a confirmation dialog that asks the user a question.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_menu - a menu
 */
static void confirmation_action(fl_context*, fl_menu*);

/**
 * The confirmation menu callback.
 * This is called when the "Confirmation" option is selected.
 * It opens a confirmation menu that allows the user to select
 * "Yes" or "No" based on the question asked in the previous dialog.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
static void confirm_menu(fl_context* context);

/**
 * The first info dialog callback.
 * This will be called after the first info message has been displayed.
 * It will display the second info message.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
static void info_cb_1(fl_context*);

/**
 * The second info dialog callback.
 * This will be called after the second info message has been displayed.
 * It will display the third info message.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
static void info_cb_2(fl_context*);

/**
 * The confirmation callback.
 * This is called after selecting a menu item from the confirmaiton menu.
 * It will display a message indicating which menu item was selected from
 * the confirmation menu.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
static void confirm_cb(fl_context*);





static void get_cursor_coords(fl_menu* menu, int* x, int* y)
{
	if (menu->child != NULL)
	{
		menu->get_cursor_coords(menu->child, x, y);
		return;
	}

	/* Determine the cursor's x position. */
	if (menu->pos < 4)
		* x = 60;
	else
		*x = 180;

	/* Determine the cursor's y position. */
	if (menu->pos < 4)
		* y = menu->pos * 30 + menu->y + 13;
	else
		*y = (menu->pos - 4) * 30 + menu->y + 13;
}

static void handle_input(fl_context* context, fl_input_handler* self)
{
	fl_menu* menu = fl_get_active_menu(context);

	if (fl_consume_key(context, FLURMP_SC_W))
	{
		cursor_up(context, menu);
		return;
	}

	if (fl_consume_key(context, FLURMP_SC_A))
	{
		cursor_left(context, menu);
		return;
	}

	if (fl_consume_key(context, FLURMP_SC_S))
	{
		cursor_down(context, menu);
		return;
	}

	if (fl_consume_key(context, FLURMP_SC_D))
	{
		cursor_right(context, menu);
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
		menu->pos = 0;
		context->paused = 0;

		/* Relenquish input control */
		fl_pop_input_handler(context);

		/* Remove the current active menu */
		fl_menu* active = fl_pop_menu(context);

		fl_destroy_menu(active);
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



/* -------------------------------------------------------------- */
/*                      Menu Item Actions                         */
/* -------------------------------------------------------------- */

static void info_action(fl_context* context, fl_menu* menu)
{
	fl_dialog_write(context, info_msg[0], 1, info_cb_1, 0);
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
	fl_dialog_write(context, preconfirm, 1, confirm_menu, 1);
}

static void confirm_menu(fl_context* context)
{
	fl_menu* confirm = fl_create_confirmation_menu(context);

	confirm->callback = confirm_cb;

	fl_push_menu(context, confirm);
	fl_push_input_handler(context, confirm->input_handler);
}



/* -------------------------------------------------------------- */
/*                   Dialog and Menu Callbacks                    */
/* -------------------------------------------------------------- */

static void info_cb_1(fl_context* context)
{
	fl_dialog_write(context, info_msg[1], 1, info_cb_2, 0);
}

static void info_cb_2(fl_context* context)
{
	fl_dialog_write(context, info_msg[2], 1, NULL, 0);
}

static void confirm_cb(fl_context* context)
{
	/* Handle dialog destruction here, since the active
	   dialog was marked as hold. */
	if (context->active_dialog != NULL)
	{
		fl_dialog* dialog = context->active_dialog;

		/* Clear the active dialog pointer. */
		context->active_dialog = NULL;

		/* Relenquish input control. */
		fl_pop_input_handler(context);

		/* Destroy the dialog. */
		fl_destroy_dialog(dialog);
	}

	switch (context->ret_val)
	{
	case 1:
		fl_dialog_write(context, confirm_msg[0], 1, NULL, 0);
		break;

	case 2:
		fl_dialog_write(context, confirm_msg[1], 1, NULL, 0);
		break;

	default:
		break;
	}
}



/* -------------------------------------------------------------- */
/*                     menu.h implementation                      */
/* -------------------------------------------------------------- */

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

	p_menu->input_handler = fl_create_input_handler(handle_input);
	p_menu->render = render;
	p_menu->get_cursor_coords = get_cursor_coords;

	return p_menu;
}
