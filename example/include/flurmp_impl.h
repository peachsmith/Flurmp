#ifndef FLURMP_IMPL_H
#define FLURMP_IMPL_H

#include "flurmp.h"

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#define FLURMP_QUIT SDL_QUIT

/* entity state flags */
#define FLURMP_JUMP_FLAG     0x01
#define FLURMP_LEFT_FLAG     0x02
#define FLURMP_INTERACT_FLAG 0x04
#define FLURMP_FLAG_4        0x08
#define FLURMP_FLAG_5        0x10
#define FLURMP_FLAG_6        0x20
#define FLURMP_FLAG_7        0x40
#define FLURMP_FLAG_8        0x80

/* default window dimensions */
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

/* entity types */
#define FLURMP_ENTITY_PLAYER 0
#define FLURMP_ENTITY_RECTANGLE 1
#define FLURMP_ENTITY_INTERACTABLE 2

struct fl_entity_type {

	/* boundaries */
	int w;
	int h;

	/* entity operations */
	void(*collide) (fl_context*, fl_entity*, fl_entity*, int, int);
	void(*update) (fl_context*, fl_entity*, int);
	void(*render) (fl_context*, fl_entity*);
};

struct fl_entity {

	int type;

	/* flags */
	unsigned char flags;

	/* position */
	int x;
	int y;

	/* boundaries (TODO: move this into entity type) */
	int w;
	int h;

	/* velocity */
	int x_v;
	int y_v;

	/* animation */
	int frame;

	/* sprite data */
	SDL_Texture* texture;

	/* list pointers */
	fl_entity* next;
	fl_entity* tail;
};

struct fl_input_handler {

	/* input states */
	const Uint8* keystates;

	/* input flags */
	int inputs[55];
};

struct fl_console {
	int x;
	int y;
	int w;
	int h;
	int cursor_x;
	int cursor_y;
	int char_count;
	SDL_Texture* font;
};

struct fl_menu_item {
	int x;
	int y;
	fl_static_text* text;
	void(*action) (fl_context*, fl_menu*);
};

struct fl_menu {
	int open;
	int active;
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

struct fl_font {
	TTF_Font* impl;
};

struct fl_glyph {
	char c;
	SDL_Surface* surface;
	SDL_Texture* texture;
};

struct fl_font_atlas {
	int count;
	fl_glyph** glyphs;
};

struct fl_static_text {
	char* text;
	int x;
	int y;
	fl_font* font;
	SDL_Surface* surface;
	SDL_Texture* texture;
};

struct fl_context {

	SDL_Window* window;

	SDL_Renderer* renderer;

	SDL_Event event;

	/* entity type registry */
	fl_entity_type* entity_types;

	/* a list of all the entities in the current context */
	fl_entity* entities;

	/* the primary control object */
	fl_entity* pco;

	fl_input_handler input;

	/* camera position */
	int cam_x;
	int cam_y;

	/* state flag */
	unsigned int state;

	/* the number of entities */
	int count;

	/* frames per second */
	int fps;

	/* tick count */
	unsigned long ticks;

	/* completion flag */
	int done;

	/* error flag */
	int error;

	/* pause flag */
	int paused;

	/* dev console */
	fl_console* console;

	/* a pause menu */
	fl_menu* pause_menu;

	/* font */
	fl_font* font;

	fl_static_text* static_text;

	fl_font_atlas* font_atlas;
};

#endif
