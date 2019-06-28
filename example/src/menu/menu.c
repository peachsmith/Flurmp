#include "menu.h"
#include "text.h"

fl_menu_item* fl_create_menu_item(fl_context* context,
	int x,
	int y,
	int p,
	const char* text,
	void(*action) (fl_context*, fl_menu*))
{
	fl_menu_item* item = (fl_menu_item*)malloc(sizeof(fl_menu_item));

	if (item == NULL)
		return NULL;

	item->x = x;
	item->y = y;
	item->action = action;

	fl_static_text* static_text = fl_create_static_text(context, text, x, y, p);

	if (static_text == NULL)
	{
		free(item);
		return NULL;
	}

	item->text = static_text;

	return item;
}

void fl_destroy_menu_item(fl_menu_item* item)
{
	if (item == NULL)
		return;

	if (item->text != NULL)
		fl_destroy_static_text(item->text);

	free(item);
}

fl_menu* fl_create_menu(int x, int y, int w, int h)
{
	fl_menu* menu = (fl_menu*)malloc(sizeof(fl_menu));

	if (menu == NULL)
		return NULL;

	menu->open = 0;
	menu->active = 0;
	menu->x = x;
	menu->y = y;
	menu->w = w;
	menu->h = h;
	menu->pos = 0;
	menu->item_count = 0;
	menu->submenu_count = 0;
	menu->items = NULL;
	menu->submenus = NULL;
	menu->input_handler = NULL;
	menu->get_cursor_coords = NULL;

	return menu;
}

void fl_destroy_menu(fl_menu* menu)
{
	if (menu == NULL)
		return;

	int i;

	/* destroy menu items */
	if (menu->items != NULL)
	{
		for (i = 0; i < menu->item_count; i++)
		{
			fl_destroy_menu_item(menu->items[i]);
		}

		free(menu->items);
	}

	/* destroy submenus */
	if (menu->submenus != NULL)
	{
		for (i = 0; i < menu->submenu_count; i++)
		{
			fl_destroy_menu(menu->submenus[i]);
		}

		free(menu->submenus);
	}

	free(menu);
}

void fl_render_menu(fl_context* context, fl_menu* menu)
{
	if (!menu->open)
		return;

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
			r.w = menu->items[i]->text->surface->w;
			r.h = menu->items[i]->text->surface->h;
			SDL_RenderCopy(context->renderer, menu->items[i]->text->texture, NULL, &r);
		}
	}

	menu->get_cursor_coords(menu, &r.x, &r.y);
	r.w = 10;
	r.h = 10;

	SDL_RenderFillRect(context->renderer, &r);

	if (menu->submenus != NULL)
	{
		for (i = 0; i < menu->item_count; i++)
		{
			fl_render_menu(context, menu->submenus[i]);
		}
	}
}
