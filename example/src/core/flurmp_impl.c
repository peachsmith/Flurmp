/*
 * an implementation of flurmp using SDL2
 */

#include "flurmp_impl.h"
#include "input.h"
#include "entity.h"
#include "console.h"
#include "text.h"
#include "menu.h"
#include "dialog.h"
#include "resource.h"
#include "scene.h"

#include "block_200_50.h"
#include "sign.h"
#include "player.h"

#include <stdlib.h>
#include <stdio.h>

static void fl_console_input(fl_context* context);

int fl_initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO)) return 0;

	if (TTF_Init())
	{
		SDL_Quit();
		return 0;
	}

	return 1;
}

void fl_terminate()
{
	TTF_Quit();
	SDL_Quit();
}

const char* fl_get_error()
{
	return SDL_GetError();
}

fl_context* fl_create_context()
{
	int i; /* index variable */

	fl_context* context;
	fl_resource** fonts;

	/* entity types */
	fl_entity_type player_type;
	fl_entity_type sign_type;
	fl_entity_type block_200_50_type;

	/* Allocate memory for an fl_context. */
	context = malloc(sizeof(fl_context));

	/* Verify fl_context memory allocation. */
	if (context == NULL)
		return NULL;

	/* Populate the context with default values. */
	context->window = NULL;
	context->renderer = NULL;
	context->entity_types = NULL;
	context->fonts = NULL;
	context->pause_menu = NULL;
	context->dialogs = NULL;
	context->console = NULL;
	context->pco = NULL;
	context->active_dialog = NULL;
	context->entities = NULL;
	context->cam_x = 0;
	context->cam_y = 0;
	context->state = 0;
	context->count = 0;
	context->fps = 60;
	context->ticks = 0;
	context->done = 0;
	context->error = 0;
	context->paused = 0;
	context->console_open = 0;
	context->font_count = 0;
	context->entity_type_count = 0;
	context->dialog_count = 0;
	
	/* Create the application window. */
	context->window = SDL_CreateWindow("Flurmp", 100, 100,
		FLURMP_WINDOW_WIDTH,
		FLURMP_WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN);

	/* Verify window creation. */
	if (context->window == NULL)
	{
		context->error = 1;
		return context;
	}

	/* Create the renderer. */
	context->renderer = SDL_CreateRenderer(context->window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	/* Verify renderer creation. */
	if (context->renderer == NULL)
	{
		context->error = 2;
		return context;
	}

	/* Configure the renderer. */
	SDL_SetRenderDrawBlendMode(context->renderer, SDL_BLENDMODE_BLEND);

	/* Get a reference to an array of key states. */
	context->input.keystates = SDL_GetKeyboardState(NULL);

	/* Allocate memory for the input flag array. */
	context->input.flags = (int*)malloc(sizeof(int) * FLURMP_INPUT_FLAG_COUNT);

	/* Verify input flag memory allocation. */
	if (context->input.flags == NULL)
	{
		context->error = 3;
		return context;
	}

	/* Set all of the input flags to 0. */
	for (i = 0; i < FLURMP_INPUT_FLAG_COUNT; i++)
		context->input.flags[i] = 0;

	/* Allocate memory for the entity type registry. */
	context->entity_types = (fl_entity_type*)malloc(sizeof(fl_entity_type) * FLURMP_ENTITY_TYPE_COUNT);

	/* Verify entity type registry memory allocation. */
	if (context->entity_types == NULL)
	{
		context->error = 4;
		return context;
	}
	
	/* Populate the entity types. */
	fl_register_player_type(context, &player_type);
	fl_register_sign_type(context, &sign_type);
	fl_register_block_200_50_type(context, &block_200_50_type);

	/* Add the entity types to the registry. */
	context->entity_types[FLURMP_ENTITY_PLAYER] = player_type;
	context->entity_types[FLURMP_ENTITY_SIGN] = sign_type;
	context->entity_types[FLURMP_ENTITY_BLOCK_200_50] = block_200_50_type;
	
	/* TODO: create image registry and have entity types share it. */

	context->entity_type_count = FLURMP_ENTITY_TYPE_COUNT;

	/* Create a font registry */
	fonts = (fl_resource**)malloc(sizeof(fl_resource) * FLURMP_FONT_COUNT);

	if (fonts == NULL)
	{
		context->error = 5;
		return context;
	}

	context->fonts = fonts;

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

	/* TODO: add a resource member to the menu, console, and dialog
	   structures so that they can share resources. */
	/* TODO: implement resource sharing among entities. */

	/* Load fonts into the font registry. */
	context->fonts[FLURMP_FONT_VERA] = fl_load_font("resources/fonts/VeraMono.ttf", 16, menu_fc, menu_bc, 1);
	context->fonts[FLURMP_FONT_COUSINE] = fl_load_font("resources/fonts/Cousine.ttf", 16, console_fc, console_bc, 0);

	/* Create font atlases. */
	context->fonts[FLURMP_FONT_VERA]->impl.font->atlas = fl_create_font_atlas(context, context->fonts[FLURMP_FONT_VERA]);
	context->fonts[FLURMP_FONT_COUSINE]->impl.font->atlas = fl_create_font_atlas(context, context->fonts[FLURMP_FONT_COUSINE]);

	context->font_count = FLURMP_FONT_COUNT;

	/* create a pause menu */
	context->pause_menu = fl_create_pause_menu(context);

	/* create a dialog registry */
	context->dialogs = (fl_dialog**)malloc(sizeof(fl_dialog*) * 1);

	if (context->dialogs == NULL)
	{
		context->error = 6;
		return context;
	}

	/* Create the dialogs and add them to the registry. */
	fl_dialog* example_dialog = fl_create_example_dialog(context->fonts[0], 75, 320, 500, 100);

	context->dialogs[0] = example_dialog;
	context->dialog_count = 1;
	
	/* Create a dev console. */
	context->console = fl_create_console(context);

	/* Load the basic scene. */
	fl_load_scene(context, FLURMP_SCENE_BASIC);

	return context;
}

void fl_destroy_context(fl_context* context)
{
	int i;

	if (context->console != NULL) fl_destroy_console(context->console);
	if (context->entity_types != NULL)
	{
		for (i = 0; i < context->entity_type_count; i++)
		{
			/* TODO: destroy image resources elsewhere
			   once the image registry has been implemented. */
			if (context->entity_types[i].texture != NULL)
				fl_destroy_resource(context->entity_types[i].texture);
		}

		free(context->entity_types);
	}

	if (context->fonts != NULL)
	{
		int i;
		for (i = 0; i < context->font_count; i++)
		{
			if (context->fonts[i] != NULL)
			{
				fl_destroy_resource(context->fonts[i]);
			}
		}

		free(context->fonts);
	}

	if (context->dialogs != NULL)
	{
		int i;
		for (i = 0; i < context->dialog_count; i++)
		{
			if (context->dialogs[i] != NULL)
			{
				fl_destroy_dialog(context->dialogs[i]);
			}
		}

		free(context->dialogs);
	}

	fl_entity* en = context->entities;
	fl_entity* next;
	while (en != NULL)
	{
		next = en->next;
		free(en);
		en = next;
	}

	if (context->renderer != NULL) SDL_DestroyRenderer(context->renderer);
	if (context->window != NULL) SDL_DestroyWindow(context->window);

	free(context);
}

int fl_is_done(fl_context* context)
{
	if (context == NULL) return 1;

	return context->error ? context->error : context->done;
}

void fl_add_entity(fl_context* context, fl_entity* entity)
{
	if (context->count == 0)
	{
		context->entities = entity;
		context->entities->tail = entity;
	}
	else
	{
		context->entities->tail->next = entity;
		context->entities->tail = entity;
	}

	context->count++;
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

void fl_handle_events(fl_context* context)
{
	while (SDL_PollEvent(&(context->event)))
	{
		/* This happens when the user closes the window. */
		if (context->event.type == FLURMP_QUIT)
			context->done = 1;
	}
}

void fl_handle_input(fl_context* context)
{
	/* If there is a dialog active,
	   its input handling takes priority. */
	if (context->active_dialog != NULL)
	{
		context->active_dialog->input_handler(context, context->active_dialog);
		return;
	}

	if (context->paused)
	{
		/* If the application is paused and the dev console
		   is not open, the pause menu will handle input. */
		if (!context->console_open)
		{
			context->pause_menu->input_handler(context, context->pause_menu);
		}
		else
		{
			/* TODO: Create an input handler callback
			   for the dev console. */
			fl_console_input(context);
		}

		return;
	}
	else
	{
		if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_ESCAPE))
		{
			context->paused = 1;
			context->pause_menu->open = 1;
		}
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
		context->entity_types[en->type].update(context, en, axis);
		en = en->next;
	}

	/* Put the entity pointer back to the beginning of the list. */
	en = context->entities;

	/* Detect and handle collisions between entities. */
	while (en != NULL)
	{
		next = en->next;

		while (next != NULL)
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
}

void fl_render(fl_context* context)
{
	/* Set the background color. */
	SDL_SetRenderDrawColor(context->renderer, 145, 219, 255, 255);
	/* SDL_SetRenderDrawColor(context->renderer, 120, 120, 120, 255); */

	/* Remove the previous screen contents. */
	SDL_RenderClear(context->renderer);

	fl_entity* en = context->entities;

	/* Render each entity by calling their render functions
	   from the entity type registry. */
	while (en != NULL)
	{
		context->entity_types[en->type].render(context, en);
		en = en->next;
	}

	/* Render the pause menu if the application is paused. */
	if (context->paused)
	{
		fl_render_menu(context, context->pause_menu);

		if (context->console_open)
			fl_render_console(context, context->console);
	}

	/* Render the dialog if it's being used. */
	if (context->active_dialog != NULL)
	{
		context->active_dialog->render(context, context->active_dialog);
	}

	/* Put everything on the screen. */
	SDL_RenderPresent(context->renderer);
}

void fl_sleep(int ms)
{
	SDL_Delay(ms);
}

void fl_begin_frame(fl_context* context)
{
	context->ticks = SDL_GetTicks();
}

void fl_end_frame(fl_context* context)
{
	if (1000U / context->fps > SDL_GetTicks() - context->ticks)
	{
		SDL_Delay(1000U / context->fps - (SDL_GetTicks() - context->ticks));
	}
}

static void render_camera_boundaries(fl_context* context)
{
	SDL_SetRenderDrawColor(context->renderer, 255, 255, 0, 255);
	SDL_RenderDrawLine(context->renderer, FLURMP_LEFT_BOUNDARY, 0, FLURMP_LEFT_BOUNDARY, FLURMP_WINDOW_HEIGHT);
	SDL_RenderDrawLine(context->renderer, FLURMP_RIGHT_BOUNDARY, 0, FLURMP_RIGHT_BOUNDARY, FLURMP_WINDOW_HEIGHT);

	SDL_SetRenderDrawColor(context->renderer, 110, 100, 255, 255);
	SDL_RenderDrawLine(context->renderer, 0, FLURMP_UPPER_BOUNDARY, FLURMP_WINDOW_WIDTH, FLURMP_UPPER_BOUNDARY);
	SDL_RenderDrawLine(context->renderer, 0, FLURMP_LOWER_BOUNDARY, FLURMP_WINDOW_WIDTH, FLURMP_LOWER_BOUNDARY);
}

static void fl_console_input(fl_context* context)
{
	int i;
	for (i = 0; i < FLURMP_SC_LIMIT; i++)
	{
		unsigned char mod = 0x00;

		/* Handle key combinations like shift and ctrl. */
		if (context->input.keystates[FLURMP_SC_LSHIFT] || context->input.keystates[FLURMP_SC_RSHIFT])
			mod |= FLURMP_CONSOLE_MOD_SHIFT;
		if (context->input.keystates[FLURMP_SC_LCTRL] || context->input.keystates[FLURMP_SC_RCTRL])
			mod |= FLURMP_CONSOLE_MOD_CTRL;

		/* Get the character that corresponds to the current scancode. */
		char c = fl_sc_to_char(i, mod);

		if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, i))
		{
			/* If escape is rpessed, close the console. */
			if (i == FLURMP_SC_ESCAPE)
			{
				context->console_open = 0;
			}

			/* If return is pressed, submit the current buffer,
			   otherwise append to the current buffer. */
			if (c == 0x0A)
				submit_buffer(context, context->console);
			else
				fl_putc(context->console, c, mod);
		}
	}
}

void fl_set_color(fl_color* color, int r, int g, int b, int a)
{
	if (color == NULL)
		return;

	/* Clamp the RGBA values to the range 0 to 255. */
	color->impl.r = r > 255 ? 255 : (r < 0 ? 0 : r);
	color->impl.g = g > 255 ? 255 : (g < 0 ? 0 : g);
	color->impl.b = b > 255 ? 255 : (b < 0 ? 0 : b);
	color->impl.a = a > 255 ? 255 : (a < 0 ? 0 : a);
}
