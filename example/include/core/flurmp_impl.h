/**
 * An implementation of flurmp using SDL2
 */
#ifndef FLURMP_IMPL_H
#define FLURMP_IMPL_H

#include "flurmp.h"
#include "core/flurmp_sdl.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

 /* window dimensions */
#define FLURMP_WINDOW_WIDTH 640
#define FLURMP_WINDOW_HEIGHT 480

/* axes */
#define FLURMP_AXIS_X 0
#define FLURMP_AXIS_Y 1

/* camera boundaries */
#define FLURMP_LEFT_BOUNDARY 220
#define FLURMP_RIGHT_BOUNDARY 430
#define FLURMP_UPPER_BOUNDARY 180
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

/**
 * Memory allocation
 *
 * Params:
 *   t - the data type to which the resulting pointer will be cast
 *   n - the number of elements to allocate
 *
 * Returns:
 *   t - a pointer to a block of memory cast as type t
 */
#define fl_alloc(t,n) (t*)fl_allocate_(sizeof(t) * n)

/**
 * Memory release
 *
 * Params:
 *   m - a pointer to a block of memory to free.
 */
#define fl_free(m) fl_free_(m)

 /**
  * Set all elements in an array to NULL
  *
  * Params:
  *   a - an array
  *   n - the number of elements in the array
  */
#define fl_null(a,n) { int i; for (i = 0; i < n; i++) a[i] = NULL; }

/**
 * Set all elements in an array to '\0'
 *
 * Params:
 *   a - an array
 *   n - the number of elements in the array
 */
#define fl_zero(a,n) { int i; for (i = 0; i < n; i++) a[i] = '\0'; }

struct fl_image {
	int w;
	int h;
	fl_texture* texture;
};

struct fl_font {
	fl_ttf* impl;
	int count;
	fl_image** glyphs;
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
	fl_animation** animations;
	int animation_count;
	void(*collide) (fl_context*, fl_entity*, fl_entity*, int, int);
	void(*update) (fl_context*, fl_entity*, int);
	void(*render) (fl_context*, fl_entity*);
};

struct fl_entity {
	int type;
	unsigned short flags;
	int x;
	int y;
	int x_v;
	int y_v;
	fl_rect* frame;
	int life;
	fl_entity* next;
	fl_entity* tail;
};

struct fl_input_handler {
	void(*handle_input) (fl_context*, fl_input_handler*);
	fl_input_handler* child;
	fl_input_handler* parent;
};

struct fl_console {
	int x;
	int y;
	int w;
	int h;
	char* buffer;
	int buffer_count;
	int cursor_x;
	int cursor_y;
	fl_font* font;
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
	int x;
	int y;
	int w;
	int h;
	int pos;
	int item_count;
	fl_menu_item** items;
	fl_input_handler* input_handler;
	void(*render) (fl_context*, fl_menu*);
	void(*get_cursor_coords) (fl_menu*, int*, int*);
	void(*callback) (fl_context*);
	fl_menu* child;
	fl_menu* parent;
};

struct fl_dialog {
	int x;
	int y;
	int w;
	int h;
	char* buffer;
	int buffer_count;
	const char* msg;
	size_t len;
	size_t counter;
	int speed;
	int hold;
	fl_font* font;
	fl_input_handler* input_handler;
	void(*update) (fl_context*, fl_dialog*);
	void(*render) (fl_context*, fl_dialog*);
	void(*callback) (fl_context*);
};

struct fl_data_panel {
	int x;
	int y;
	int w;
	int h;
	char* buffer;
	int buffer_count;
	fl_font* font;
	void(*update) (fl_context*, fl_data_panel*);
	void(*render) (fl_context*, fl_data_panel*);
};

struct fl_animation {
	fl_rect* frames;
	int frame_count;
};

struct fl_schedule {
	int done;
	int counter;
	int limit;
	void* target;
	void(*action)(fl_context*, fl_schedule*, void*);
	fl_schedule* next;
	fl_schedule* prev;
};

typedef struct fl_transition {
	int scheduled;
	int from_scene;
	int to_scene;
}fl_transition;

struct fl_context {

	/* Windowing, rendering, events, and input */
	fl_window* window;
	fl_renderer* renderer;
	fl_event event;
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

	/* Pointer to the projectile entities */
	fl_entity* projectiles;

	/* Linked list of schedules */
	fl_schedule* schedules;

	/* Linked list of input handlers */
	fl_input_handler* input_handler;

	/* Structures directly affected by input */
	fl_console* console;
	fl_entity* pco;
	fl_dialog* active_dialog;
	fl_menu* active_menu;
	fl_data_panel* data_panel;

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

	/* Pause flag */
	int paused;

	/* Indicated the current scene */
	int scene;

	/* A return value */
	int ret_val;

	/* scene transition */
	fl_transition transition;
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

/**
 * A helper function to set the location and dimensions of a rectangle.
 *
 * Params:
 *   fl_rect - a reference to a rect structure
 *   int - the x coordinate
 *   int - the y coordinate
 *   int - the width
 *   int - the height
 *
 */
void fl_set_rect(fl_rect* r, int x, int y, int w, int h);

/**
 * A helper function used to manage memory allocation.
 *
 * Params:
 *   size_t - the size of the memory block to allocated
 *
 * Returns:
 *   void* - a pointer to a newly allocated block of memory
 */
void* fl_allocate_(size_t s);

/**
 * A helper function used to manage the release of dynamically
 * allocated memory.
 *
 * Params:
 *   void* - a pointer to a dynamically allocated block of memory
 */
void fl_free_(void* m);

#endif
