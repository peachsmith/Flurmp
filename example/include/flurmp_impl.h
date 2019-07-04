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
/* TODO: create a common entity header file */
#define FLURMP_ENTITY_TYPE_COUNT 3
#define FLURMP_ENTITY_PLAYER 0
#define FLURMP_ENTITY_SIGN 1
#define FLURMP_ENTITY_BLOCK_200_50 2

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

	/* boundaries */
	int w;
	int h;

	/* sprite data */
	fl_resource* texture;

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

	/* velocity */
	int x_v;
	int y_v;

	/* animation */
	int frame;

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

	SDL_Event event;

	/* entity type registry */
	fl_entity_type* entity_types;

	/* the number of entity types in the entity type registry */
	int entity_type_count;

	/* a linked list of all the entities in the current context */
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

	/* dev console flag */
	int console_open;

	/* dev console */
	fl_console* console;

	/* a pause menu */
	fl_menu* pause_menu;

	/* font registry */
	fl_resource** fonts;

	/* the number of fonts in the font registry */
	int font_count;

	/* dialog registry */
	fl_dialog** dialogs;

	/* the number of dialogs in the dialog registry */
	int dialog_count;

	/* the current active dialog */
	fl_dialog* active_dialog;
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
