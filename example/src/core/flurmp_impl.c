/*
 * an implementation of flurmp using SDL2
 */

#include "flurmp_impl.h"
#include "input.h"
#include "rectangle.h"
#include "interactable.h"
#include "player.h"
#include "console.h"
#include "text.h"
#include "menu.h"
#include "dialog.h"
#include "resource.h"

#include <stdlib.h>
#include <stdio.h>

static void fl_console_input(fl_context* context);
static void fl_pause_menu_input(fl_context* context);

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
	int i;

	fl_context* context = malloc(sizeof(fl_context));

	if (context == NULL)
		return NULL;

	context->error = 0;

	context->window = SDL_CreateWindow("Flurmp", 100, 100,
		FLURMP_WINDOW_WIDTH,
		FLURMP_WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (context->window == NULL)
	{
		context->error = 1;
		return context;
	}

	context->renderer = SDL_CreateRenderer(context->window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_SetRenderDrawBlendMode(context->renderer, SDL_BLENDMODE_BLEND);

	if (context->renderer == NULL) context->error = 2;

	context->input.keystates = SDL_GetKeyboardState(NULL);

	for (i = 0; i < 2; i++) context->input.inputs[i] = 0;

	context->entities = NULL;
	context->cam_x = 0;
	context->cam_y = 0;
	context->count = 0;
	context->done = 0;
	context->fps = 60;
	context->paused = 0;
	context->console_open = 0;
	context->fonts = NULL;

	/* for now we have 3 entity types: player, rectangle, and interactable */
	fl_entity_type* entity_types = (fl_entity_type*)malloc(sizeof(fl_entity_type) * 3);

	fl_entity_type player_type;
	fl_entity_type rectangle_type;
	fl_entity_type interactable_type;

	fl_register_player_type(&player_type);
	fl_register_rectangle_type(&rectangle_type);
	fl_register_interactable_type(&interactable_type);

	entity_types[0] = player_type;
	entity_types[1] = rectangle_type;
	entity_types[2] = interactable_type;

	context->entity_types = entity_types;

	/* create a player */
	fl_entity* player = fl_create_player(300, 260, 30, 40);

	/* load the sprite for the player */
	fl_resource* player_texture = fl_load_bmp(context, "resources/images/person.bmp");

	player->texture = player_texture;

	/* block to stand on */
	fl_entity* block_1 = fl_create_rectangle(440, 250, 70, 20);
	fl_entity* block_2 = fl_create_rectangle(500, 220, 50, 20);

	/* walls and floor */
	fl_entity* ground1 = fl_create_rectangle(0, 300, 200, 50);
	fl_entity* ground2 = fl_create_rectangle(260, 300, 480, 50);
	fl_entity* left_wall = fl_create_rectangle(-70, 100, 50, 250);
	fl_entity* right_wall = fl_create_rectangle(715, 100, 50, 250);

	fl_entity* lower_floor1 = fl_create_rectangle(0, 400, 500, 50);
	fl_entity* lower_floor2 = fl_create_rectangle(500, 450, 100, 50);
	fl_entity* lower_floor3 = fl_create_rectangle(600, 500, 100, 50);

	/* something to interact with */
	fl_entity* sign = fl_create_interactable(310, 270, 30, 30);

	/* add the entities to the context */
	fl_add_entity(context, sign);
	fl_add_entity(context, player);
	fl_add_entity(context, ground1);
	fl_add_entity(context, ground2);
	fl_add_entity(context, block_1);
	fl_add_entity(context, block_2);
	fl_add_entity(context, left_wall);
	fl_add_entity(context, right_wall);
	fl_add_entity(context, lower_floor1);
	fl_add_entity(context, lower_floor2);
	fl_add_entity(context, lower_floor3);

	/* set the primary control object */
	context->pco = player;

	context->state = 0;

	/* add the dev console */
	fl_console* console = fl_create_console(context);

	context->console = console;

	/* create a font registry */
	int font_count = 2;
	fl_resource** fonts = (fl_resource**)malloc(sizeof(fl_resource) * font_count);

	if (fonts == NULL)
	{
		context->error = 4;
		context->done = 1;
		return context;
	}

	context->fonts = fonts;

	/* font colors */
	fl_color menu_fc;
	fl_color menu_bc;
	fl_color console_fc;
	fl_color console_bc;

	fl_set_color(&menu_fc, 250, 250, 250, 255);
	fl_set_color(&menu_bc, 0, 0, 0, 255);
	fl_set_color(&console_fc, 250, 250, 250, 255);
	fl_set_color(&console_bc, 0, 0, 0, 0);

	/* load fonts into the font registry */
	context->fonts[FL_FONT_VERA] = fl_load_font("resources/fonts/VeraMono.ttf", 16, menu_fc, menu_bc, 1);
	context->fonts[FL_FONT_COUSINE] = fl_load_font("resources/fonts/Cousine.ttf", 16, console_fc, console_bc, 0);

	/* create font atlases */
	context->fonts[FL_FONT_VERA]->impl.font->atlas = fl_create_font_atlas(context, context->fonts[FL_FONT_VERA]);
	context->fonts[FL_FONT_COUSINE]->impl.font->atlas = fl_create_font_atlas(context, context->fonts[FL_FONT_COUSINE]);

	context->font_count = 2;

	/* create a pause menu */
	fl_menu* pause_menu = fl_create_pause_menu(context);
	context->pause_menu = pause_menu;

	/* create a dialog registry */
	context->dialogs = (fl_dialog**)malloc(sizeof(fl_dialog*) * 1);

	if (context->dialogs == NULL)
	{
		context->error = 6;
		context->done = 1;
		return context;
	}

	/* create the dialogs and add them to the registry */
	fl_dialog* example_dialog = fl_create_example_dialog(context->fonts[0], 75, 320, 500, 100);

	context->dialogs[0] = example_dialog;

	context->dialog_count = 1;
	context->active_dialog = NULL;

	return context;
}

void fl_destroy_context(fl_context* context)
{
	if (context->console != NULL) fl_destroy_console(context->console);
	if (context->entity_types != NULL) free(context->entity_types);

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
		fl_destroy_resource(en->texture);
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

int fl_detect_collision(fl_entity* a, fl_entity* b)
{
	int collision = 0;

	/* TODO: get width and height from the entity type registry */

	if (a->x >= b->x && a->x < b->x + b->w)
	{
		if (a->y > b->y && a->y < b->y + b->h)
		{
			/* bottom right */
			collision = 1;
		}
		else if (a->y + a->h > b->y && a->y < b->y + b->h)
		{
			/* top right */
			collision = 2;
		}
	}

	if (b->x >= a->x && b->x < a->x + a->w)
	{
		if (b->y > a->y && b->y < a->y + a->h)
		{
			/* top left */
			collision = 3;
		}
		else if (b->y + b->h > a->y && b->y < a->y + a->h)
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
		if (context->event.type == FLURMP_QUIT) context->done = 1;
	}
}

void fl_handle_input(fl_context* context)
{
	if (context->active_dialog != NULL)
	{
		context->active_dialog->input_handler(context, context->active_dialog);
		return;
	}

	if (context->paused)
	{
		if (!context->console_open)
		{
			context->pause_menu->input_handler(context, context->pause_menu);
		}
		else
		{
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

	/* reset player position */
	if (fl_peek_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_C))
	{
		context->pco->x = 260;
		context->pco->y = 260;
		context->cam_x = 0;
		context->cam_y = 0;
	}
}

void fl_update(fl_context* context)
{
	if (context->active_dialog != NULL)
	{
		context->active_dialog->update(context, context->active_dialog);
	}

	if (context->paused)
		return;

	fl_entity* en = context->entities;
	fl_entity* next;

	/* update state with respect to X axis */
	while (en != NULL)
	{
		context->entity_types[en->type].update(context, en, FLURMP_AXIS_X);
		en = en->next;
	}

	en = context->entities;

	/* handle collisions with respect to X axis */
	while (en != NULL)
	{
		next = en->next;

		while (next != NULL)
		{
			int collided = fl_detect_collision(en, next);
			if (collided)
			{
				context->entity_types[en->type].collide(context, en, next, collided, FLURMP_AXIS_X);
				context->entity_types[next->type].collide(context, next, en, collided, FLURMP_AXIS_X);
			}
			next = next->next;
		}

		en = en->next;
	}

	en = context->entities;

	/* update state with respect to Y axis */
	while (en != NULL)
	{
		context->entity_types[en->type].update(context, en, FLURMP_AXIS_Y);
		en = en->next;
	}

	en = context->entities;

	/* handle collisions with respect to Y axis */
	while (en != NULL)
	{
		next = en->next;

		while (next != NULL)
		{
			int collided = fl_detect_collision(en, next);
			if (collided)
			{
				context->entity_types[en->type].collide(context, en, next, collided, FLURMP_AXIS_Y);
				context->entity_types[next->type].collide(context, next, en, collided, FLURMP_AXIS_Y);
			}
			next = next->next;
		}

		en = en->next;
	}
}

void fl_render(fl_context* context)
{
	SDL_SetRenderDrawColor(context->renderer, 145, 219, 255, 255);
	/* SDL_SetRenderDrawColor(context->renderer, 120, 120, 120, 255); */
	SDL_RenderClear(context->renderer);

	fl_entity* en = context->entities;

	/* get render from entity type registry */

	while (en != NULL)
	{
		context->entity_types[en->type].render(context, en);
		en = en->next;
	}

	if (context->paused)
	{
		fl_render_menu(context, context->pause_menu);

		if (context->console_open)
		{
			fl_render_console(context, context->console);
		}
	}

	if (context->active_dialog != NULL)
	{
		context->active_dialog->render(context, context->active_dialog);
	}

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
		int flag = FLURMP_INPUT_UNKNOWN;
		unsigned char mod = 0x00;

		/* handle key combos like shift and ctrl */
		if (context->input.keystates[FLURMP_SC_LSHIFT] || context->input.keystates[FLURMP_SC_RSHIFT])
			mod |= FLURMP_CONSOLE_MOD_SHIFT;
		if (context->input.keystates[FLURMP_SC_LCTRL] || context->input.keystates[FLURMP_SC_RCTRL])
			mod |= FLURMP_CONSOLE_MOD_CTRL;

		char c = fl_sc_to_char(i, &flag, mod);

		if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, i))
		{
			if (i == FLURMP_SC_ESCAPE)
			{
				context->console_open = 0;
			}

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

	color->impl.r = r > 255 ? 255 : (r < 0 ? 0 : r);
	color->impl.g = g > 255 ? 255 : (g < 0 ? 0 : g);
	color->impl.b = b > 255 ? 255 : (b < 0 ? 0 : b);
	color->impl.a = a > 255 ? 255 : (a < 0 ? 0 : a);
}
