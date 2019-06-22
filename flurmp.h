/**
 * Flurmp
 * A basic framework for no particular purpose.
 *
 * Author: John Powell
 */
#ifndef FLURMP_H
#define FLURMP_H

/**
 * Represents the state of the application at any given time.
 * One of these should be created during initialization of the application
 * and destroyed upon termination of the application.
 */
typedef struct fl_context fl_context;

/**
 * Anything that can potentially interact with something else to change the
 * state of the application.
 * A context should maintain a collection of entities and handle their
 * interactions.
 */
typedef struct fl_entity fl_entity;

/**
 * The behavior of entities.
 * A context should be able to determine how to render an entity or mutate
 * its state by inspecting the contents of an entity type.
 */
typedef struct fl_entity_type fl_entity_type;

/**
 * Handles user input for the current context.
 * A context and any entity in that context should be able to observe
 * user input events through an input handler.
 */
typedef struct fl_input_handler fl_input_handler;

/**
 * A developer console.
 * This should be used to receive text input directly from the developer.
 * If using a console, it is recommended to disable it in release versions.
 */
typedef struct fl_console fl_console;

typedef struct fl_menu fl_menu;
typedef struct fl_menu_item fl_menu_item;




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
 * Determines whether a context is ready to be destroyed.
 *
 * Params:
 *   fl_context - a reference to a context
 *
 * Returns:
 *   int - 1 if the context is ready for destruction or 0
 */
int fl_is_done(fl_context*);

/**
 * Gets the last error message from the underlying implementation.
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



/* TODO: relocate these */

/**
 * Checks the state of an input event.
 * If this function determines that the input event
 * has occurred, it flags the input event as consumed.
 */
int fl_consume_input(fl_context*, int, int);

/**
 * Checks the state of an input event.
 */
int fl_peek_input(fl_context*, int, int);

#endif /* !FLURMP_H */
