#ifndef FLURMP_H
#define FLURMP_H

/* implementation-specific typedefs */
#if defined (FLURMP_SDL)
#include <SDL/SDL.h>

#define FLURMP_QUIT SDL_QUIT

/* currently 42 values */
#define FLURMP_SC_A SDL_SCANCODE_A /* 1st scancode */
#define FLURMP_SC_B SDL_SCANCODE_B
#define FLURMP_SC_C SDL_SCANCODE_C
#define FLURMP_SC_D SDL_SCANCODE_D
#define FLURMP_SC_E SDL_SCANCODE_E
#define FLURMP_SC_F SDL_SCANCODE_F
#define FLURMP_SC_G SDL_SCANCODE_G
#define FLURMP_SC_H SDL_SCANCODE_H
#define FLURMP_SC_I SDL_SCANCODE_I
#define FLURMP_SC_J SDL_SCANCODE_J
#define FLURMP_SC_K SDL_SCANCODE_K
#define FLURMP_SC_L SDL_SCANCODE_L
#define FLURMP_SC_M SDL_SCANCODE_M
#define FLURMP_SC_N SDL_SCANCODE_N
#define FLURMP_SC_O SDL_SCANCODE_O
#define FLURMP_SC_P SDL_SCANCODE_P
#define FLURMP_SC_Q SDL_SCANCODE_Q
#define FLURMP_SC_R SDL_SCANCODE_R
#define FLURMP_SC_S SDL_SCANCODE_S
#define FLURMP_SC_T SDL_SCANCODE_T /* 20th scancode */
#define FLURMP_SC_U SDL_SCANCODE_U
#define FLURMP_SC_V SDL_SCANCODE_V
#define FLURMP_SC_W SDL_SCANCODE_W
#define FLURMP_SC_X SDL_SCANCODE_X
#define FLURMP_SC_Y SDL_SCANCODE_Y
#define FLURMP_SC_Z SDL_SCANCODE_Z /* 26th scancode */
#define FLURMP_SC_0 SDL_SCANCODE_0
#define FLURMP_SC_1 SDL_SCANCODE_1
#define FLURMP_SC_2 SDL_SCANCODE_2
#define FLURMP_SC_3 SDL_SCANCODE_3
#define FLURMP_SC_4 SDL_SCANCODE_4
#define FLURMP_SC_5 SDL_SCANCODE_5
#define FLURMP_SC_6 SDL_SCANCODE_6
#define FLURMP_SC_7 SDL_SCANCODE_7
#define FLURMP_SC_8 SDL_SCANCODE_8
#define FLURMP_SC_9 SDL_SCANCODE_9 /* 36 scancode */
#define FLURMP_SC_COMMA SDL_SCANCODE_COMMA
#define FLURMP_SC_PERIOD SDL_SCANCODE_PERIOD
#define FLURMP_SC_SPACE SDL_SCANCODE_SPACE
#define FLURMP_SC_ESCAPE SDL_SCANCODE_ESCAPE /* 40 scancode */
#define FLURMP_SC_LSHIFT SDL_SCANCODE_LSHIFT
#define FLURMP_SC_RSHIFT SDL_SCANCODE_RSHIFT /* 42nd scancode */


typedef SDL_Event fl_event;
typedef SDL_Window fl_window;
typedef SDL_Renderer fl_renderer;
typedef const Uint8 fl_keystate;
#endif /* FLURMP_SDL */

/* input control */
#define FLURMP_INPUT_UNKNOWN 0
#define FLURMP_INPUT_A 1
#define FLURMP_INPUT_B 2
#define FLURMP_INPUT_C 3
#define FLURMP_INPUT_D 4
#define FLURMP_INPUT_E 5
#define FLURMP_INPUT_F 6
#define FLURMP_INPUT_G 7
#define FLURMP_INPUT_H 8
#define FLURMP_INPUT_I 9
#define FLURMP_INPUT_J 10
#define FLURMP_INPUT_K 11
#define FLURMP_INPUT_L 12
#define FLURMP_INPUT_M 13
#define FLURMP_INPUT_N 14
#define FLURMP_INPUT_O 15
#define FLURMP_INPUT_P 16
#define FLURMP_INPUT_Q 17
#define FLURMP_INPUT_R 18
#define FLURMP_INPUT_S 19
#define FLURMP_INPUT_T 20
#define FLURMP_INPUT_U 21
#define FLURMP_INPUT_V 22
#define FLURMP_INPUT_W 23
#define FLURMP_INPUT_X 24
#define FLURMP_INPUT_Y 25
#define FLURMP_INPUT_Z 26
#define FLURMP_INPUT_0 27
#define FLURMP_INPUT_1 28
#define FLURMP_INPUT_2 29
#define FLURMP_INPUT_3 30
#define FLURMP_INPUT_4 31
#define FLURMP_INPUT_5 32
#define FLURMP_INPUT_6 33
#define FLURMP_INPUT_7 34
#define FLURMP_INPUT_8 35
#define FLURMP_INPUT_9 36
#define FLURMP_INPUT_COMMA 37
#define FLURMP_INPUT_PERIOD 38
#define FLURMP_INPUT_SPACE 39
#define FLURMP_INPUT_ESCAPE 40
#define FLURMP_INPUT_SHIFT 41

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

#define FLURMP_AXIS_X 0
#define FLURMP_AXIS_Y 1

#define FLURMP_LEFT_BOUNDARY 220
#define FLURMP_RIGHT_BOUNDARY 430
#define FLURMP_UPPER_BOUNDARY 230
#define FLURMP_LOWER_BOUNDARY 320

typedef enum fl_entity_type_e fl_entity_type;
typedef struct fl_entity_s fl_entity;
typedef struct fl_context_s fl_context;

enum fl_entity_type_e {
	FL_PLAYER,
	FL_SOLID,
	FL_INTERACT,
	FL_HOSTILE,
	FL_DIALOG,
	FL_SCENERY
};

/**
 * represents all objects that could potentially interact
 * with one another to change the state of the application
 */
struct fl_entity_s {

	fl_entity_type type;

	/* flags */
	unsigned char flags;

	/* position */
	int x;
	int y;

	/* velocity */
	int x_v;
	int y_v;

	/* boundaries */
	int w;
	int h;

	/* animation */
	int frame;

	/* sprite data */
	SDL_Texture *texture;

	/* entity operations */
	void(*collide) (fl_context*, fl_entity*, fl_entity*, int, int);
	void(*update) (fl_context*, fl_entity*, int);
	void(*render) (fl_context*, fl_entity*);

	/* list pointers */
	fl_entity *next;
	fl_entity *tail;
};

/**
 * represents a text console for debugging and stuff
 */
typedef struct console_t
{
	int x;
	int y;
	int w;
	int h;
	int cursor_x;
	int cursor_y;
	int char_count;
	SDL_Texture* font;
}console_t;

/**
 * a structure to contain the state of the application
 */
struct fl_context_s {

	/* the area in which graphics willl be rendered */
	fl_window *window;

	/* object responsible for rendering graphics */
	fl_renderer *renderer;

	/* the most recent event */
	fl_event event;

	/* keeps track of button states for user input */
	fl_keystate *keystates;

	/* keeps track of what inputs have been received */
	int inputs[41];

	/* a list of all the entities in the current context */
	fl_entity *entities;

	/* the primary control object */
	fl_entity* pco;

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
	console_t* console;
};

/**
 * Initializes everything necessary to implement the framework. This should
 * be called once per application before using any of the functionality.
 *
 * Returns:
 *   An integer indicating success
 *   0 - error
 *   1 - success
 */
int fl_initialize();


/**
 * Frees any resources that were allocated by initialization. This should
 * be called once per application at the termination of the application
 * if fl_init has already been called.
 */
void fl_terminate();


/**
 * Gets the last error message from the underlying implementation
 *
 * Returns:
 *   a string of characters containing the most recent error message
 */
const char* fl_get_error();

/**
 * Creates the context. If an error occurred during context creation,
 * the error flag should be set to a non-zero value.
 *
 * Returns:
 *   a context
 */
fl_context* fl_create_context();

/**
* Frees resources allocated for the context
*/
void fl_destroy_context(fl_context*);

/**
 * Adds an entity to the current context
 */
void fl_add_entity(fl_context*, fl_entity*);

/**
 * Determines if two entities have collided
 *
 * Returns:
 *   An integer indicating the relative direction from which the collision
 *   occurred. The return value should be 0, 1, 2, 3, or 4.
 *   0 - no collision
 *   1 - bottom right
 *   2 - top right
 *   3 - top left
 *   4 - bottom left
 */
int fl_detect_collision(fl_entity*, fl_entity*);

/**
 * Checks for events
 *
 * Returns:
 *   an integer indicating whether or not there are any pending events
 *   0 - no events
 *   1 - events pending
 */
int fl_poll_event(fl_context*);

/**
 * handles events
 */
void fl_handle_event(fl_context*);

/**
* handles user input
*/
void fl_handle_input(fl_context*);

/**
 * Updates the state of the context
 */
void fl_update(fl_context*);

/**
* Renders the current contents of the context to the screen
*/
void fl_render(fl_context*);

/**
 * begins an iteration of the main loop
 */
void fl_begin_frame(fl_context* context);

/**
* concludes an iteration of the main loop
*/
void fl_end_frame(fl_context* context);

#endif /* !FLURMP_H */
