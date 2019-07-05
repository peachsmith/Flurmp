#ifndef FLURMP_IMPL_H
#define FLURMP_IMPL_H

#include "flurmp.h"

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#define FLURMP_QUIT SDL_QUIT

/* window dimensions */
#define FLURMP_WINDOW_WIDTH 640
#define FLURMP_WINDOW_HEIGHT 480

/* axes */
#define FLURMP_AXIS_X 0
#define FLURMP_AXIS_Y 1

/* camera boundaries */
#define FLURMP_LEFT_BOUNDARY 220
#define FLURMP_RIGHT_BOUNDARY 430
#define FLURMP_UPPER_BOUNDARY 230
#define FLURMP_LOWER_BOUNDARY 320

struct fl_color {
	SDL_Color impl;
};

struct fl_image {
	SDL_Surface* surface;
	SDL_Texture* texture;
};

struct fl_glyph {
	char c;
	fl_image* image;
};

struct fl_font_atlas {
	int count;
	fl_glyph** glyphs;
};

struct fl_font {
	TTF_Font* impl;
	fl_font_atlas* atlas;
	fl_color forecolor;
	fl_color backcolor;
	int background;
};

struct fl_resource {
	int type;
	union {
		fl_image* image;
		fl_font* font;
	} impl;
};

struct fl_entity_type {
	int w;
	int h;
	fl_resource* texture;
	void(*collide) (fl_context*, fl_entity*, fl_entity*, int, int);
	void(*update) (fl_context*, fl_entity*, int);
	void(*render) (fl_context*, fl_entity*);
};

struct fl_entity {
	int type;
	unsigned char flags;
	int x;
	int y;
	int x_v;
	int y_v;
	int frame;
	fl_entity* next;
	fl_entity* tail;
};

struct fl_console {
	int x;
	int y;
	int w;
	int h;
	int cursor_x;
	int cursor_y;
	int char_count;
};

struct fl_menu_item {
	int x;
	int y;
	fl_static_text* text;
	void(*action) (fl_context*, fl_menu*);
};

struct fl_menu {
	fl_menu* child;
	fl_menu* parent;
	int open;
	int x;
	int y;
	int w;
	int h;
	int pos;
	int item_count;
	int submenu_count;
	fl_menu_item** items;
	fl_menu** submenus;
	void(*input_handler) (fl_context*, fl_menu*);
	void(*get_cursor_coords) (fl_menu*, int*, int*);
};

struct fl_static_text {
	char* text;
	int x;
	int y;
	fl_resource* font;
	fl_image* image;
};
 
struct fl_dialog {
	fl_resource* font;
	int x;
	int y;
	int w;
	int h;
	int counter;
	const char** statements;
	int statement_count;
	int current_statement;
	int open;
	char* buffer;
	int buffer_count;
	void(*update) (fl_context*, fl_dialog*);
	void(*render) (fl_context*, fl_dialog*);
	void(*input_handler) (fl_context*, fl_dialog*);
};

struct fl_context {
	SDL_Window* window;
	SDL_Renderer* renderer;
	struct {
		const Uint8* keystates;
		int* flags;
	} input;
	fl_entity_type* entity_types;
	fl_resource** fonts;
	fl_menu* pause_menu;
	fl_dialog** dialogs;
	fl_console* console;
	fl_entity* pco;
	fl_resource** resources;
	fl_entity* entities;
	fl_dialog* active_dialog;
	SDL_Event event;
	int cam_x;
	int cam_y;
	unsigned int state;
	int count;
	int fps;
	unsigned long ticks;
	int done;
	int error;
	int paused;
	int console_open;
	int font_count;
	int entity_type_count;
	int dialog_count;
};

/**
 * A helper function to set color values.
 *
 * Params:
 *   fl_color - a reference to a color structure
 *   int - the red value ranging from 0 - 255
 *   int - the green value ranging from 0 - 255
 *   int - the blue value ranging from 0 - 255
 *   int - the alpha value ranging from 0 - 255
 *   
 */
void fl_set_color(fl_color* color, int r, int g, int b, int a);

#endif
