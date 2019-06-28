/**
 * Flurmp
 * A recommendation of concepts for a framework for 2D graphical user
 * interaction.
 *
 * Author: John Powell
 */
#ifndef FLURMP_H
#define FLURMP_H

#define FLURMP_VERSION_STRING "1.0.0"

 /* -------------------------------------------------------------- */
 /*                             Notes                              */
 /*                                                                */
 /* Terms like "allocate" and "create" are used interchangably to  */
 /* refer to the process of reserving of memory resources.         */
 /* As such, terms like "free" and "destroy" are used to refer to  */
 /* the process of releasing those memory resources.               */
 /*                                                                */
 /* Terms like "should" and "it is recommended" are used           */
 /* throughout this document when referring to how the underlying  */
 /* implementation behaves. This is because Flurmp is ultimately   */
 /* a set of recommendations, not a set of rules. No software is   */
 /* obligated to implement any of the concepts described in this   */
 /* document in any particular way.                                */
 /* -------------------------------------------------------------- */



 /* -------------------------------------------------------------- */
 /*                             Types                              */
 /* -------------------------------------------------------------- */

 /**
  * Represents the state of the application at any given time.
  * One of these should be created after initialization and before
  * entering into the main loop. When it is no longer needed, it should
  * be destroyed.
  *
  * During the course of an application, additional resources may be
  * created and destroyed within a single context.
  * The disposal of a Flurmp context should free any of these additional
  * resources left over after the main loop concludes.
  */
typedef struct fl_context fl_context;

/**
 * Anything that can potentially interact with something else to change the
 * state of the application. For example, a player may be considered
 * and entity, as well as the ground on which it stands.
 *
 * Three principal operations should be performed on each entity
 * during each iteration of the main loop:
 * 1. update: change the current state of the entity
 * 2. collide: handle collisions with other entities
 * 3. render: draw the entity to the screen
 *
 * An entity whose state never changes during an update is said to
 * be inert (e.g. a static piece of ground on which the player stands).
 *
 * A context should maintain a collection of entities and handle their
 * interactions.
 */
typedef struct fl_entity fl_entity;

/**
 * An entity type contains information common to all entities of
 * a particular category.
 * Each entity type should at least contain information allowing a
 * Flurmp context to update or render an entity, as well as handle
 * any interactions between entities.
 *
 * Ideally, an entity type should contain as much information as possible
 * in order to minimize the amount of data stored in an individual
 * instance of an entity.
 *
 * For example, an entity type called "brick_1" may be created such that
 * all entities of the type brick_1 have the same behavior and appearance.
 * A context may determine how to render and modify all entities of type
 * brick_1 by finding their entity type in a list of known entity types.
 */
typedef struct fl_entity_type fl_entity_type;

/**
 * Provides visibility into user input events.
 * A context and any entity in that context should be able to observe
 * user input events through an input handler.
 */
typedef struct fl_input_handler fl_input_handler;

/**
 * Represents a font object.
 * In the present day, a "font" is basically a "typeface".
 */
typedef struct fl_font fl_font;

/**
 * A graphical representation of an individual character.
 */
typedef struct fl_glyph fl_glyph;

/**
 * A collection of all printable characters in a font.
 * It is up to the implementation to determine which characters
 * are considered printable.
 */
typedef struct fl_font_atlas fl_font_atlas;

/**
 * Text to be rendered to the screen.
 * Static text should not change for the duration of its life cycle.
 */
typedef struct fl_static_text fl_static_text;

/**
 * A command line interface (CLI).
 * This should be used to receive text input directly from the user
 * or the developer.
 * This can be used as a developer console, in which case it is recommended
 * to disable it in release versions.
 * It could also potentially be used as a normal part of the application
 * to allow the user to type in text input.
 */
typedef struct fl_console fl_console;

/**
 * Menus allows the user to select from a list of options to perform
 * certain actions.
 * A menu is not obligated to have any particular number of options,
 * but it is recommended for each menu to have at least one option.
 * There may be any number of child menus within a menu, and it is up
 * to the implementation to determine how to handle them.
 *
 * All menus should allow some way of closing themselves. A menu that
 * cannot be closed is effectively a soft lock.
 */
typedef struct fl_menu fl_menu;

/**
 * A menu item represents a single option the user can select while
 * navigating through a menu.
 *
 * By selecting a menu item, the user should be able to trigger some
 * action to happen.
 * If no action occurs when an item is selected, that item is said to
 * be disabled.
 */
typedef struct fl_menu_item fl_menu_item;



/* -------------------------------------------------------------- */
/*                           Functions                            */
/* -------------------------------------------------------------- */

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
 * be called after fl_init has successfully been called and the application
 * has no more need for the Flurmp framework.
 *
 * The application may continue to perform other actions after terminating
 * the framework.
 */
void fl_terminate();

/**
 * Determines whether a context is ready to be destroyed.
 *
 * Params:
 *   fl_context - a Flurmp context
 *
 * Returns:
 *   int - 1 if the context is ready for disposal or 0
 */
int fl_is_done(fl_context* context);

/**
 * Gets the last error message from the underlying implementation.
 *
 * Returns:
 *   const char* - the most recent error message
 */
const char* fl_get_error();

/**
 * Creates the context. If an error occurred during context creation,
 * the error flag should be set to a non-zero value.
 *
 * Returns:
 *   fl_context - a newly created Flurmp context
 */
fl_context* fl_create_context();

/**
 * Frees resources allocated for the context.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
void fl_destroy_context(fl_context* context);

/**
 * Adds an entity to the current context.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - an entity
 */
void fl_add_entity(fl_context* context, fl_entity* entity);

/**
 * Determines if two entities have collided.
 *
 * Params:
 *   fl_entity - an entity
 *   fl_entity - another entity
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
int fl_detect_collision(fl_entity* a, fl_entity* b);

/**
 * Handles events.
 * Events are anything from user input to moving the window.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
void fl_handle_events(fl_context* context);

/**
 * Handles user input.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
void fl_handle_input(fl_context* context);

/**
 * Updates the state of the context.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
void fl_update(fl_context* context);

/**
 * Renders the current contents of the context to the screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
void fl_render(fl_context* context);

/**
 * Begins an iteration of the main loop.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
void fl_begin_frame(fl_context* context);

/**
 * Concludes an iteration of the main loop.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
void fl_end_frame(fl_context* context);

#endif /* !FLURMP_H */
