#ifndef FLURMP_H

/* implementation-specific typedefs */
#if defined (FLURMP_SDL)
#include <SDL/SDL.h>

#define FLURMP_QUIT SDL_QUIT

#define FLURMP_SC_ESCAPE SDL_SCANCODE_ESCAPE

typedef SDL_Event fl_event;
typedef SDL_Window fl_window;
typedef SDL_Renderer fl_renderer;
typedef const Uint8 fl_keystate;
#endif /* FLURMP_SDL */


/* default window dimensions */
#define FLURMP_WINDOW_WIDTH 640
#define FLURMP_WINDOW_HEIGHT 480

#define FLURMP_AXIS_X 0
#define FLURMP_AXIS_Y 1

typedef enum fl_entity_type_e fl_entity_type;
typedef struct fl_entity_s fl_entity;
typedef struct fl_context_s fl_context;

enum fl_entity_type_e {
	FL_PCO,
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

	/* position */
	int x;
	int y;

	/* velocity */
	int x_v;
	int y_v;

	/* boundaries */
	int w;
	int h;

	/* entity operations */
	void(*collide) (fl_context, fl_entity*, fl_entity*, int, int);
	void(*update) (fl_context, fl_entity*, int);
	void(*render) (fl_context, fl_entity*);

	/* list pointers */
	fl_entity *next;
	fl_entity *tail;
};

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

	/* a list of all the entities in the current context */
	fl_entity *entities;

	/* the number of entities */
	int count;

	/* completion flag */
	int done;

	/* error flag */
	int error;
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
fl_context fl_create_context();

/**
* Frees resources allocated for the context
*/
void fl_destroy_context(fl_context);

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
 * Updates the state of the context
 */
void fl_update(fl_context);

/**
* Renders the current contents of the context to the screen
*/
void fl_render(fl_context);

/**
 * Waits for at least the specified number of milliseconds
 */
void fl_sleep(int);

#endif /* !FLURMP_H */
