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

/* error codes */
#define FLURMP_ERR_CONTEXT       0x01
#define FLURMP_ERR_WINDOW        0x02
#define FLURMP_ERR_RENDERER      0x03
#define FLURMP_ERR_INPUT_FLAGS   0x04
#define FLURMP_ERR_ENTITY_TYPES  0x05
#define FLURMP_ERR_FONTS         0x06
#define FLURMP_ERR_IMAGES        0x07
#define FLURMP_ERR_INPUT_HANDLER 0x08

/* memory allocation macro */
#define fl_alloc(t,n) (t*)fl_alloc_internal_(sizeof(t) * n)

/* memory release macro */
#define fl_free(m) fl_free_internal_(m)

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
	int life;
	fl_entity* next;
	fl_entity* tail;
};

/**
 * A structure for holding the current input handler function.
 * It contains pointers to parent and child input handlers
 * which allows one input handler to pass control to another.
 */
typedef struct fl_input_handler {

	void(*handler) (fl_context*, fl_input_handler*);

	fl_input_handler* child;
	fl_input_handler* parent;

}fl_input_handler;

struct fl_console {
	fl_resource* font;
	int x;
	int y;
	int w;
	int h;
	int cursor_x;
	int cursor_y;
	int char_count;
	fl_input_handler* input_handler;
	void(*render) (fl_context*, fl_console*);
};

struct fl_menu_item {
	int x;
	int y;
	fl_image* image;
	void(*action) (fl_context*, fl_menu*);
};

struct fl_menu {
	fl_menu* child;
	fl_menu* parent;
	int x;
	int y;
	int w;
	int h;
	int pos;
	int item_count;
	int submenu_count;
	fl_menu_item** items;
	fl_input_handler* input_handler;
	void(*render) (fl_context*, fl_menu*);
	void(*get_cursor_coords) (fl_menu*, int*, int*);
};

struct fl_dialog {
	fl_font_atlas* atlas;
	int x;
	int y;
	int w;
	int h;
	size_t counter;
	int open;
	char* buffer;
	int buffer_count;
	void(*update) (fl_context*, fl_dialog*);
	void(*render) (fl_context*, fl_dialog*);
	fl_input_handler* input_handler;
	const char* msg;
	size_t len;
	int speed;
	int hold;
	void(*callback) (fl_context*, fl_dialog*);
};

struct fl_context {

	/* Windowing, rendering, events, and input */
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	struct {
		const Uint8* keystates;
		int* flags;
	} input;

	/* Registries */
	fl_entity_type* entity_types;
	fl_resource** fonts;
	fl_resource** images;

	/* Linked list of entities */
	fl_entity* entities;

	/* Input handling callback */
	fl_input_handler* input_handler;

	/* Structures directly affected by input */
	fl_console* console;
	fl_entity* pco;
	fl_dialog* active_dialog;
	fl_menu* active_menu;

	/* Camera position */
	int cam_x;
	int cam_y;

	/* The current state of the context */
	unsigned int state;

	/* Number of entities in the entity list */
	int entity_count;

	/* Estimated frames per second */
	int fps;

	/* Tick count used for regulating framerate */
	unsigned long ticks;

	/* Completion flag */
	int done;

	/* Error flag */
	int error;

	int paused;

	/* Indicated the current scene */
	int scene;

	/* A return value */
	int ret_val;
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

void* fl_alloc_internal_(size_t s);

void fl_free_internal_(void* m);

#endif
