#include "menu.h"
#include "text.h"
#include "input.h"

fl_menu_item* fl_create_menu_item(fl_context* context,
	int x,
	int y,
	const char* text,
	void(*action) (fl_context*, fl_menu*))
{
	fl_menu_item* item;
	fl_image* image;

	/* Allocate memory for a menu item. */
	item = fl_alloc(fl_menu_item, 1);

	/* Verify item allocation. */
	if (item == NULL)
		return NULL;

	item->x = x;
	item->y = y;
	item->action = action;

	/* Create an image to render for the menu item. */
	image = fl_create_static_text(context, context->fonts[FLURMP_FONT_VERA], text);

	/* Verify image creation. */
	if (image == NULL)
	{
		fl_free(item);
		return NULL;
	}

	item->image = image;

	return item;
}

void fl_destroy_menu_item(fl_menu_item* item)
{
	if (item == NULL)
		return;

	if (item->image != NULL)
		fl_destroy_static_text(item->image);

	fl_free(item);
}

fl_menu* fl_create_menu(int x, int y, int w, int h)
{
	fl_menu* menu = fl_alloc(fl_menu, 1);

	if (menu == NULL)
		return NULL;

	menu->child = NULL;
	menu->parent = NULL;
	menu->x = x;
	menu->y = y;
	menu->w = w;
	menu->h = h;
	menu->pos = 0;
	menu->item_count = 0;
	menu->items = NULL;
	menu->input_handler = NULL;
	menu->get_cursor_coords = NULL;
	menu->callback = NULL;

	return menu;
}

void fl_destroy_menu(fl_menu* menu)
{
	if (menu == NULL)
		return;

	int i;

	/* Destroy menu items. */
	if (menu->items != NULL)
	{
		for (i = 0; i < menu->item_count; i++)
		{
			fl_destroy_menu_item(menu->items[i]);
		}

		fl_free(menu->items);
	}

	/* Destroy the input handler. */
	if (menu->input_handler != NULL)
		fl_destroy_input_handler(menu->input_handler);

	/* Destroy the child menus. */
	if (menu->child != NULL)
		fl_destroy_menu(menu->child);

	/* Destroy the menu. */
	fl_free(menu);
}

fl_menu* fl_get_active_menu(fl_context* context)
{
	if (context == NULL || context->active_menu == NULL)
		return NULL;

	fl_menu* menu = context->active_menu;

	/* Traverse the menu list until we find one with no child. */
	while (menu->child != NULL)
		menu = menu->child;

	return menu;
}

void fl_push_menu(fl_context* context, fl_menu* menu)
{
	if (context == NULL || menu == NULL)
		return;

	if (context->active_menu == NULL)
	{
		context->active_menu = menu;
	}
	else
	{
		fl_menu* active = context->active_menu;

		/* Traverse the menu list until we find one with no child. */
		while (active->child != NULL)
			active = active->child;

		/* Link the new menu to the current active menu. */
		active->child = menu;
		menu->parent = active;
	}
}

fl_menu* fl_pop_menu(fl_context* context)
{
	if (context == NULL || context->active_menu == NULL)
		return NULL;

	fl_menu* menu = context->active_menu;

	/* Traverse the menu list until we find one with no child. */
	while (menu->child != NULL)
		menu = menu->child;

	/* If the active menu has no parent, clear the active menu pointer. */
	if (menu->parent == NULL)
	{
		context->active_menu = NULL;
		return menu;
	}

	/* Clear the parent and child pointers to prevent the
	   destruction of other menus when this menu is
	   destroyed elsewhere. */
	menu->parent->child = NULL;
	menu->parent = NULL;

	return menu;
}
