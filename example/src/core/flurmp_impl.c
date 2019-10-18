#include "core/flurmp_impl.h"
#include "core/input.h"
#include "core/resource.h"
#include "core/text.h"
#include "core/image.h"
#include "core/console.h"
#include "core/dialog.h"
#include "core/data_panel.h"
#include "core/schedule.h"
#include "core/animation.h"

#include "scene/scene.h"

#include "menu/menu.h"
#include "menu/pause_menu.h"

#include "entity/entity.h"
#include "entity/block_200_50.h"
#include "entity/sign.h"
#include "entity/player.h"
#include "entity/spike.h"
#include "entity/door.h"
#include "entity/pellet.h"

/*
  TODO:

  new features:
	hostile
	start screen
	end screen
	audio

  modifications:
	multiple types of projectiles
	multiple textures per image file
*/

/* Counters to keep track of how many times malloc and free
   have been called. These do NOT take into account the calls
   to malloc and free made by libraries. */
static int allocations_ = 0;
static int frees_ = 0;

/**
 * Determines if an entity is within the screen boundaries.
 * If an entity is considered to be off screen, there's no
 * need to render it.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - an entity
 *
 * Returns:
 *   int - 1 if the entity is on screen, or 0 if
 *         the entity is off screen.
 */
static int is_on_screen(fl_context* context, fl_entity* entity);

/**
 * The root input handler is the first input handler that is added to
 * a context.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_input_handler - a pointer to the input handler
 */
static void root_input_handler(fl_context* context, fl_input_handler* self);

/**
 * Renders the camera boundaries to the screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
static void render_camera_boundaries(fl_context* context);

/**
 * Locates the next available projectile in a context.
 * For a projectile to be considered usable, it must have
 * its alive flag toggled off.
 *
 * Params:
 *   fl_context - a Flurmp context
 *
 * Returns:
 *   fl_entity - a reference to the next available projectile
 *               or NULL if no projectile is found.
 */
static fl_entity* find_projectile(fl_context* context);



const char* fl_get_error()
{
	return SDL_GetError();
}

fl_context* fl_create_context()
{
	fl_context* context;

	/* entity types */
	fl_entity_type player_type;
	fl_entity_type sign_type;
	fl_entity_type block_200_50_type;
	fl_entity_type spike_type;
	fl_entity_type door_type;
	fl_entity_type pellet_type;

	/* Allocate memory for a Flurmp context. */
	context = fl_alloc(fl_context, 1);

	/* Verify context memory allocation. */
	if (context == NULL)
		return NULL;

	/* Populate the context with default values. */
	context->window = NULL;
	context->renderer = NULL;
	context->entity_types = NULL;
	context->fonts = NULL;
	context->images = NULL;
	context->entities = NULL;
	context->projectiles = NULL;
	context->schedules = NULL;
	context->input_handler = NULL;
	context->console = NULL;
	context->pco = NULL;
	context->active_dialog = NULL;
	context->active_menu = NULL;
	context->data_panel = NULL;
	context->cam_x = 0;
	context->cam_y = 0;
	context->state = 0;
	context->entity_count = 0;
	context->fps = 60;
	context->ticks = 0;
	context->done = 0;
	context->error = 0;
	context->paused = 0;
	context->scene = 0;
	context->ret_val = 0;
	context->transition.scheduled = 0;
	context->transition.to_scene = 0;
	context->transition.from_scene = 0;

	/* Create the application window. */
	context->window = fl_create_window("Flurmp",
		100, 100, FLURMP_WINDOW_WIDTH, FLURMP_WINDOW_HEIGHT);

	/* Verify window creation. */
	if (context->window == NULL)
	{
		context->error = FLURMP_ERR_WINDOW;
		return context;
	}

	/* Create the renderer. */
	context->renderer = fl_create_renderer(context->window);

	/* Verify renderer creation. */
	if (context->renderer == NULL)
	{
		context->error = FLURMP_ERR_RENDERER;
		return context;
	}

	/* Get a reference to an array of key states. */
	context->input.keystates = fl_get_key_states();

	/* Allocate memory for the input flag array. */
	context->input.flags = fl_alloc(int, FLURMP_INPUT_FLAG_COUNT);

	/* Verify input flag memory allocation. */
	if (context->input.flags == NULL)
	{
		context->error = FLURMP_ERR_INPUT_FLAGS;
		return context;
	}

	/* Set all of the input flags to 0. */
	fl_zero(context->input.flags, FLURMP_INPUT_FLAG_COUNT);

	/* Allocate memory for the entity type registry. */
	context->entity_types = fl_alloc(fl_entity_type, FLURMP_ENTITY_TYPE_COUNT);

	/* Verify entity type registry memory allocation. */
	if (context->entity_types == NULL)
	{
		context->error = FLURMP_ERR_ENTITY_TYPES;
		return context;
	}

	/* Populate the entity types. */
	fl_register_player_type(context, &player_type);
	fl_register_sign_type(context, &sign_type);
	fl_register_block_200_50_type(context, &block_200_50_type);
	fl_register_spike_type(context, &spike_type);
	fl_register_door_type(context, &door_type);
	fl_register_pellet_type(context, &pellet_type);

	/* Add the entity types to the registry. */
	context->entity_types[FLURMP_ENTITY_PLAYER] = player_type;
	context->entity_types[FLURMP_ENTITY_SIGN] = sign_type;
	context->entity_types[FLURMP_ENTITY_BLOCK_200_50] = block_200_50_type;
	context->entity_types[FLURMP_ENTITY_SPIKE] = spike_type;
	context->entity_types[FLURMP_ENTITY_DOOR] = door_type;
	context->entity_types[FLURMP_ENTITY_PELLET] = pellet_type;


	/* Create a font registry */
	context->fonts = fl_alloc(fl_resource*, FLURMP_FONT_COUNT);

	/* Verify font registry creation. */
	if (context->fonts == NULL)
	{
		context->error = FLURMP_ERR_FONTS;
		return context;
	}

	/* font colors */
	fl_color menu_fc;
	fl_color menu_bc;
	fl_color console_fc;
	fl_color console_bc;

	/* Populate the font colors with RGBA values. */
	fl_set_color(&menu_fc, 250, 250, 250, 255);
	fl_set_color(&menu_bc, 0, 0, 0, 255);
	fl_set_color(&console_fc, 250, 250, 250, 255);
	fl_set_color(&console_bc, 0, 0, 0, 0);

	/* Load fonts into the font registry. */
	context->fonts[FLURMP_FONT_VERA] = fl_load_font("resources/fonts/VeraMono.ttf", 16, menu_fc, menu_bc, 1);
	context->fonts[FLURMP_FONT_COUSINE] = fl_load_font("resources/fonts/Cousine.ttf", 16, console_fc, console_bc, 0);
	context->fonts[FLURMP_FONT_KARMILLA_BOLD] = fl_load_font("resources/fonts/Karmilla-Bold.ttf", 16, console_fc, console_bc, 0);

	/* Create font atlases. */
	fl_create_font_atlas(context, context->fonts[FLURMP_FONT_VERA]);
	fl_create_font_atlas(context, context->fonts[FLURMP_FONT_COUSINE]);
	fl_create_font_atlas(context, context->fonts[FLURMP_FONT_KARMILLA_BOLD]);

	/* Allocate memory for an image registry. */
	context->images = fl_alloc(fl_resource*, FLURMP_IMAGE_COUNT);

	/* Verify image registry allocation. */
	if (context->images == NULL)
	{
		context->error = FLURMP_ERR_IMAGES;
		return context;
	}

	/* Set all images to NULL. */
	fl_null(context->images, FLURMP_IMAGE_COUNT);

	/* Load common images. */
	context->images[FLURMP_IMAGE_PLAYER] = fl_load_image(context, "resources/images/person.bmp");

	context->entity_types[FLURMP_ENTITY_PLAYER].texture = context->images[FLURMP_IMAGE_PLAYER];

	/* Register the root input handler. */
	context->input_handler = fl_create_input_handler(root_input_handler);

	if (context->input_handler == NULL)
	{
		context->error = FLURMP_ERR_INPUT_HANDLER;
		return context;
	}

	/* Create a data panel. */
	context->data_panel = fl_create_data_panel(420, 20, 200, 150, context->fonts[FLURMP_FONT_COUSINE]->impl.font);

	if (context->data_panel == NULL)
	{
		context->error = 0x09;
		return context;
	}

	/* Load a test scene. */
	fl_load_scene(context, FLURMP_SCENE_TEST_1);

	return context;
}

void fl_destroy_context(fl_context* context)
{
	int i, j;

	fl_entity* en;
	fl_entity* next;

	/* Destroy the entities. */
	en = context->entities;
	while (en != NULL)
	{
		next = en->next;
		fl_free(en);
		en = next;
	}

	/* Removed all input handler except the root input handler.
	   They should be destroyed when the structures
	   that contain them are destroyed. */
	if (context->input_handler != NULL)
	{
		while (context->input_handler->child != NULL)
			fl_pop_input_handler(context);

		/* Destroy the root input handler. */
		fl_destroy_input_handler(context->input_handler);
	}

	/* Destroy the dev console. */
	if (context->console != NULL)
		fl_destroy_console(context->console);

	/* Destroy the entity type registry. */
	if (context->entity_types != NULL)
	{
		for (i = 0; i < FLURMP_ENTITY_TYPE_COUNT; i++)
		{
			fl_entity_type et = context->entity_types[i];
			if (et.animations != NULL)
			{
				for (j = 0; j < et.animation_count; j++)
				{
					fl_destroy_animation(et.animations[j]);
				}
			}
		}

		fl_free(context->entity_types);
	}


	/* Destroy the active menu. */
	if (context->active_menu != NULL)
		fl_destroy_menu(context->active_menu);

	/* Destroy the active dialog. */
	if (context->active_dialog != NULL)
		fl_destroy_dialog(context->active_dialog);

	/* Destroy the font registry. */
	if (context->fonts != NULL)
	{
		for (i = 0; i < FLURMP_FONT_COUNT; i++)
		{
			if (context->fonts[i] != NULL)
				fl_destroy_resource(context->fonts[i]);
		}

		fl_free(context->fonts);
	}

	/* Destroy the data panel. */
	if (context->data_panel != NULL)
		fl_destroy_data_panel(context->data_panel);

	/* Destroy the image registry. */
	if (context->images != NULL)
	{
		for (i = 0; i < FLURMP_IMAGE_COUNT; i++)
		{
			if (context->images[i] != NULL)
				fl_destroy_resource(context->images[i]);
		}

		fl_free(context->images);
	}

	/* Destroy any schedules. */
	if (context->schedules != NULL)
	{
		fl_schedule* w = context->schedules;
		fl_schedule* next;

		while (w != NULL)
		{
			next = w->next;
			fl_destroy_schedule(w);
			w = next;
		}
	}

	/* Destroy the input flags. */
	if (context->input.flags != NULL)
		fl_free(context->input.flags);

	/* Destroy the renderer. */
	if (context->renderer != NULL)
		fl_destroy_renderer(context->renderer);

	/* Destroy the window. */
	if (context->window != NULL)
		fl_destroy_window(context->window);

	fl_free(context);

	printf("allocations: %d\n", allocations_);
	printf("frees: %d\n", frees_);
}

int fl_is_done(fl_context* context)
{
	if (context == NULL) return 1;

	return context->error ? context->error : context->done;
}

void fl_add_entity(fl_context* context, fl_entity* entity)
{
	if (context->entity_count == 0)
	{
		context->entities = entity;
		context->entities->tail = entity;
	}
	else
	{
		context->entities->tail->next = entity;
		context->entities->tail = entity;
	}

	context->entity_count++;
}

int fl_detect_collision(fl_context* context, fl_entity* a, fl_entity* b)
{
	int collision = 0;

	int a_w = context->entity_types[a->type].w;
	int a_h = context->entity_types[a->type].h;
	int b_w = context->entity_types[b->type].w;
	int b_h = context->entity_types[b->type].h;

	if (a->x >= b->x && a->x < b->x + b_w)
	{
		if (a->y > b->y && a->y < b->y + b_h)
		{
			/* bottom right */
			collision = 1;
		}
		else if (a->y + a_h > b->y && a->y < b->y + b_h)
		{
			/* top right */
			collision = 2;
		}
	}

	if (b->x >= a->x && b->x < a->x + a_w)
	{
		if (b->y > a->y && b->y < a->y + a_h)
		{
			/* top left */
			collision = 3;
		}
		else if (b->y + b_h > a->y && b->y < a->y + a_h)
		{
			/* bottom left */
			collision = 4;
		}
	}

	return collision;
}

void fl_handle_input(fl_context* context)
{
	fl_input_handler* ih = fl_get_input_handler(context);

	if (ih != NULL && ih->handle_input != NULL)
	{
		ih->handle_input(context, ih);
	}
}

/**
 * A helper function to update entity state and handle collision.
 * Since the updates and collision detection are performed once
 * for each axis of movement, this function is called twice.
 * If this function was only called once, then the code could
 * be placed in the fl_update function.
 */
static void update_and_collide(fl_context* context, int axis)
{
	/* Get a pointer to the linked list of entities. */
	fl_entity* en = context->entities;
	fl_entity* next;

	/* Update all entities.  */
	while (en != NULL)
	{
		if (en->flags & FLURMP_ALIVE_FLAG)
			context->entity_types[en->type].update(context, en, axis);

		en = en->next;
	}

	/* Put the entity pointer back to the beginning of the list. */
	en = context->entities;

	/* Detect and handle collisions between entities. */
	while (en != NULL)
	{
		next = en->next;

		/* Only check for collisions if the entity is alive. */
		while (next != NULL && en->flags & FLURMP_ALIVE_FLAG)
		{
			/* Determine if two entities have collided. */
			int collided = fl_detect_collision(context, en, next);

			/* If a collision has occurred,
			   call the collide function of each entity
			   from the entity type registry. */
			if (collided)
			{
				context->entity_types[en->type].collide(context, en, next, collided, axis);
				context->entity_types[next->type].collide(context, next, en, collided, axis);
			}
			next = next->next;
		}

		en = en->next;
	}
}

void fl_update(fl_context* context)
{
	/* Updating the data panel takes priority over all other updates. */
	if (context->data_panel != NULL)
		context->data_panel->update(context, context->data_panel);

	/* If a dialog is active, then that's the only
	   thing that needs to be updated. */
	if (context->active_dialog != NULL)
	{
		context->active_dialog->update(context, context->active_dialog);
		return;
	}

	/* If the application is paused, the state should
	   only be modified through the pause menu functionality. */
	if (context->paused)
		return;

	/* Update the entities and handle collisions. */
	update_and_collide(context, FLURMP_AXIS_X);
	update_and_collide(context, FLURMP_AXIS_Y);

	/* Call the schedules' action functions. */
	if (context->schedules != NULL)
	{
		fl_schedule* w = context->schedules;
		fl_schedule* next;

		while (w != NULL)
		{
			w->action(context, w, w->target);

			next = w->next;

			/* Dispose of the schedule
			   if it has completed its action. */
			if (w->done)
			{
				fl_remove_schedule(context, w);
				fl_destroy_schedule(w);
			}

			w = next;
		}
	}

	if (context->transition.scheduled)
	{
		fl_clear_scene(context);
		fl_load_scene(context, context->transition.to_scene);
		context->transition.scheduled = 0;
		context->transition.from_scene = 0;
		context->transition.to_scene = 0;
	}
}

void fl_render(fl_context* context)
{
	/* Set the background color. */
	fl_set_draw_color(context, 145, 219, 255, 255);

	/* Remove the previous screen contents. */
	fl_render_clear(context);

	fl_entity* en = context->entities;

	/* Render each entity by calling their render functions
	   from the entity type registry. */
	while (en != NULL)
	{
		if (en->flags & FLURMP_ALIVE_FLAG)
			context->entity_types[en->type].render(context, en);

		en = en->next;
	}

	/* Render the active menu. */
	if (context->active_menu != NULL)
		context->active_menu->render(context, context->active_menu);

	/* Render the dev console. */
	if (context->console != NULL)
		context->console->render(context, context->console);

	/* Render the data panel */
	if (context->data_panel != NULL)
		context->data_panel->render(context, context->data_panel);

	/* Render the dialog. */
	if (context->active_dialog != NULL)
		context->active_dialog->render(context, context->active_dialog);

	/* render_camera_boundaries(context); */

	/* Put everything on the screen. */
	fl_render_show(context);
}

static void render_camera_boundaries(fl_context* context)
{
	fl_set_draw_color(context, 255, 255, 0, 255);
	fl_draw_line(context, FLURMP_LEFT_BOUNDARY, 0, FLURMP_LEFT_BOUNDARY, FLURMP_WINDOW_HEIGHT);
	fl_draw_line(context, FLURMP_RIGHT_BOUNDARY, 0, FLURMP_RIGHT_BOUNDARY, FLURMP_WINDOW_HEIGHT);

	fl_set_draw_color(context, 110, 100, 255, 255);
	fl_draw_line(context, 0, FLURMP_UPPER_BOUNDARY, FLURMP_WINDOW_WIDTH, FLURMP_UPPER_BOUNDARY);
	fl_draw_line(context, 0, FLURMP_LOWER_BOUNDARY, FLURMP_WINDOW_WIDTH, FLURMP_LOWER_BOUNDARY);
}

void fl_set_color(fl_color* color, int r, int g, int b, int a)
{
	if (color == NULL)
		return;

	/* Clamp the RGBA values to the range 0 to 255. */
	color->r = r > 255 ? 255 : (r < 0 ? 0 : r);
	color->g = g > 255 ? 255 : (g < 0 ? 0 : g);
	color->b = b > 255 ? 255 : (b < 0 ? 0 : b);
	color->a = a > 255 ? 255 : (a < 0 ? 0 : a);
}

void fl_set_rect(fl_rect* r, int x, int y, int w, int h)
{
	if (r == NULL)
		return;

	r->x = x;
	r->y = y;
	r->w = w;
	r->h = h;
}

static int is_on_screen(fl_context* context, fl_entity* entity)
{
	if (entity == NULL)
		return 0;

	int left = entity->x;
	int right = entity->x + context->entity_types[entity->type].w;

	int top = entity->y;
	int bottom = entity->y + context->entity_types[entity->type].h;

	if ((left > 0 && left < FLURMP_WINDOW_WIDTH)
		|| (right > 0 && right < FLURMP_WINDOW_WIDTH))
	{
		if (top > 0 && top < FLURMP_WINDOW_HEIGHT)
			return 1;
		else if (bottom > 0 && bottom < FLURMP_WINDOW_HEIGHT)
			return 1;
	}

	return 0;
}

static void root_input_handler(fl_context* context, fl_input_handler* self)
{
	if (fl_consume_key(context, FLURMP_SC_ESCAPE))
	{
		context->paused = 1;

		fl_menu* pause_menu = fl_create_pause_menu(context);

		fl_push_menu(context, pause_menu);

		/* Hand input control to the pause menu. */
		fl_push_input_handler(context, pause_menu->input_handler);
	}



	/* Walk to the left */
	if (fl_peek_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_A))
	{
		if (!(context->pco->flags & FLURMP_MIRROR_FLAG))
			context->pco->flags |= FLURMP_MIRROR_FLAG;


		if (context->pco->x_v > -2)
			context->pco->x_v -= 2;
	}

	/* Walk to the right */
	if (fl_peek_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_D))
	{
		if ((context->pco->flags & FLURMP_MIRROR_FLAG))
			context->pco->flags &= ~(FLURMP_MIRROR_FLAG);

		if (context->pco->x_v < 2)
			context->pco->x_v += 2;
	}

	/* Jumping */
	if (fl_consume_key(context, FLURMP_SC_SPACE))
	{
		if (!(context->pco->flags & FLURMP_AIR_FLAG))
		{
			context->pco->y_v -= 12;
			context->pco->flags |= FLURMP_AIR_FLAG;
		}
	}

	/* Reset the interaction flag. */
	if (context->pco->flags & FLURMP_INTERACT_FLAG)
		context->pco->flags &= ~(FLURMP_INTERACT_FLAG);

	if (fl_consume_key(context, FLURMP_SC_J))
	{
		if (!(context->pco->flags & FLURMP_INTERACT_FLAG))
		{
			context->pco->flags |= FLURMP_INTERACT_FLAG;
		}
	}

	if (fl_consume_key(context, FLURMP_SC_K))
	{
		fl_entity* p = find_projectile(context);
		if (p != NULL)
			fl_schedule_pellet(context, p);
	}

	/* Temporary code to reset player position.
	   This should probable be performed via a command
	   issued to the dev console. */
	if (fl_peek_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_C))
	{
		context->pco->x = 260;
		context->pco->y = 260;
		context->cam_x = 0;
		context->cam_y = 0;
	}

	if (fl_consume_key(context, FLURMP_SC_T))
	{
		fl_schedule_walk(context, context->pco);
	}
}

static fl_entity* find_projectile(fl_context* context)
{
	if (context == NULL || context->projectiles == NULL)
		return NULL;

	fl_entity* next = context->projectiles;

	while (next != NULL && (next->flags & FLURMP_ALIVE_FLAG))
		next = next->next;

	return next;
}



void* fl_allocate_(size_t s)
{
	void* p = malloc(s);

	if (p != NULL)
	{
		allocations_++;
	}

	return p;
}

void fl_free_(void* m)
{
	free(m);
	frees_++;
}
