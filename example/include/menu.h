#ifndef FLURMP_MENU_H
#define FLURMP_MENU_H

#include "flurmp_impl.h"

fl_menu_item* fl_create_menu_item(fl_context* context,
	int x,
	int y,
	int p,
	const char* text,
	void(*action) (fl_context*, fl_menu*));

void fl_destroy_menu_item(fl_menu_item* item);

fl_menu* fl_create_menu(int x, int y, int w, int h);

void fl_destroy_menu(fl_menu* menu);

fl_menu* fl_create_pause_menu(fl_context* context);

void fl_render_menu(fl_context* context, fl_menu* menu);

#endif
