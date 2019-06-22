/*
 * an implementation of flurmp using SDL2
 */

#include "flurmp_impl.h"
#include "input.h"
#include "rectangle.h"
#include "interactable.h"
#include "player.h"
#include "console.h"

#include <stdlib.h>
#include <stdio.h>

static void fl_test_input(fl_context*);
static void fl_console_input(fl_context* context);
static void fl_pause_menu_input(fl_context* context);

int fl_initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO)) return 0;

	return 1;
}

void fl_terminate()
{
	SDL_Quit();
}

const char* fl_get_error()
{
	return SDL_GetError();
}

fl_context* fl_create_context()
{
	fl_context* context = malloc(sizeof(fl_context));

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

	int i;
	for (i = 0; i < 2; i++) context->input.inputs[i] = 0;

	context->entities = NULL;
	context->cam_x = 0;
	context->cam_y = 0;
	context->count = 0;
	context->done = 0;
	context->fps = 60;
	context->paused = 0;

	/* for now we have 3 entity types: player, rectangle, and interactable */
	fl_entity_type * entity_types = (fl_entity_type*)malloc(sizeof(fl_entity_type) * 3);

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
	fl_entity * player = fl_create_player(300, 260, 30, 40);

	/* load the sprite for the player */
	SDL_Surface * surface = SDL_LoadBMP("./images/person.bmp");
	SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 255, 0, 255));
	SDL_Texture * player_texture = SDL_CreateTextureFromSurface(context->renderer, surface);
	SDL_FreeSurface(surface);

	player->texture = player_texture;

	/* block to stand on */
	fl_entity * block_1 = fl_create_rectangle(440, 250, 70, 20);
	fl_entity * block_2 = fl_create_rectangle(500, 220, 50, 20);

	/* walls and floor */
	fl_entity * ground1 = fl_create_rectangle(0, 300, 200, 50);
	fl_entity * ground2 = fl_create_rectangle(260, 300, 480, 50);
	fl_entity * left_wall = fl_create_rectangle(-70, 100, 50, 250);
	fl_entity * right_wall = fl_create_rectangle(715, 100, 50, 250);

	fl_entity * lower_floor1 = fl_create_rectangle(0, 400, 500, 50);
	fl_entity * lower_floor2 = fl_create_rectangle(500, 450, 100, 50);
	fl_entity * lower_floor3 = fl_create_rectangle(600, 500, 100, 50);

	/* something to interact with */
	fl_entity * sign = fl_create_interactable(310, 270, 30, 30);

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
	fl_console * console = fl_create_console(context);

	context->console = console;

	return context;
}

void fl_destroy_context(fl_context * context)
{
	if (context->console != NULL) fl_destroy_console(context->console);
	if (context->renderer != NULL) SDL_DestroyRenderer(context->renderer);
	if (context->window != NULL) SDL_DestroyWindow(context->window);
	if (context->entity_types != NULL) free(context->entity_types);
	fl_entity * en = context->entities;
	fl_entity * next;
	while (en != NULL)
	{
		next = en->next;
		free(en);
		en = next;
	}
	free(context);
}

int fl_is_done(fl_context * context)
{
	return context->done;
}

void fl_add_entity(fl_context * context, fl_entity * entity)
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

int fl_detect_collision(fl_entity * a, fl_entity * b)
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

int fl_poll_event(fl_context * context)
{
	return SDL_PollEvent(&(context->event));
}

void fl_handle_event(fl_context * context)
{
	if (context->event.type == FLURMP_QUIT) context->done = 1;
}

void fl_handle_input(fl_context * context)
{
	/* TODO: split this into multiple functions based on state */
	if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_ESCAPE))
	{
		if (!context->paused)
			context->paused = 1;
		else
			context->paused = 0;
	}

	if (context->paused)
	{
		fl_console_input(context);
		//fl_pause_menu_input(context);
		return;
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

void fl_update(fl_context * context)
{
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

static void render_camera_boundaries(fl_context * context)
{
	SDL_SetRenderDrawColor(context->renderer, 255, 255, 0, 255);
	SDL_RenderDrawLine(context->renderer, FLURMP_LEFT_BOUNDARY, 0, FLURMP_LEFT_BOUNDARY, FLURMP_WINDOW_HEIGHT);
	SDL_RenderDrawLine(context->renderer, FLURMP_RIGHT_BOUNDARY, 0, FLURMP_RIGHT_BOUNDARY, FLURMP_WINDOW_HEIGHT);

	SDL_SetRenderDrawColor(context->renderer, 110, 100, 255, 255);
	SDL_RenderDrawLine(context->renderer, 0, FLURMP_UPPER_BOUNDARY, FLURMP_WINDOW_WIDTH, FLURMP_UPPER_BOUNDARY);
	SDL_RenderDrawLine(context->renderer, 0, FLURMP_LOWER_BOUNDARY, FLURMP_WINDOW_WIDTH, FLURMP_LOWER_BOUNDARY);
}

void fl_render(fl_context * context)
{
	SDL_SetRenderDrawColor(context->renderer, 145, 219, 255, 255);
	SDL_RenderClear(context->renderer);

	fl_entity* en = context->entities;

	/* get render from entity type registry */

	while (en != NULL)
	{
		context->entity_types[en->type].render(context, en);
		en = en->next;
	}

	/* render_camera_boundaries(context); */

	if (context->paused)
	{
		fl_render_console(context, context->console);
	}

	SDL_RenderPresent(context->renderer);
}

void fl_sleep(int ms)
{
	SDL_Delay(ms);
}

void fl_begin_frame(fl_context * context)
{
	context->ticks = SDL_GetTicks();
}

void fl_end_frame(fl_context * context)
{
	if (1000U / context->fps > SDL_GetTicks() - context->ticks)
	{
		SDL_Delay(1000U / context->fps - (SDL_GetTicks() - context->ticks));
	}
}

static void fl_test_input(fl_context * context)
{
	if (context->input.keystates[FLURMP_SC_ESCAPE])
	{
		if (!context->input.inputs[FLURMP_INPUT_ESCAPE])
		{
			context->input.inputs[FLURMP_INPUT_ESCAPE] = 1;

			if (!context->paused)
				context->paused = 1;
			else
				context->paused = 0;
		}
	}
	else if (context->input.inputs[FLURMP_INPUT_ESCAPE])
	{
		context->input.inputs[FLURMP_INPUT_ESCAPE] = 0;
	}

	if (context->paused)
	{
		fl_console_input(context);
		return;
	}

	if (context->input.keystates[FLURMP_SC_C])
	{
		context->pco->x = 260;
		context->pco->y = 260;
		context->cam_x = 0;
		context->cam_y = 0;
	}
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
			if (c == 0x0A)
				submit_buffer(context, context->console);
			else
				fl_putc(context->console, c, mod);
		}
	}
}

static void fl_pause_menu_input(fl_context * context)
{

	//if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_W))
	//{
	//	/* printf("W was pressed in the pause menu\n"); */
	//	fl_menu_cursor_up(context, context->menu);
	//}

	//if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_A))
	//{
	//	/* printf("A was pressed in the pause menu\n"); */
	//}

	//if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_S))
	//{
	//	/* printf("S was pressed in the pause menu\n"); */
	//	fl_menu_cursor_down(context, context->menu);
	//}

	//if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_D))
	//{
	//	/* printf("D was pressed in the pause menu\n"); */
	//}

	//if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_J))
	//{
	//	/* printf("J was pressed in the pause menu\n"); */
	//	fl_menu_cursor_select(context, context->menu);
	//}

	//if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_K))
	//{
	//	/* printf("K was pressed in the pause menu\n"); */
	//	fl_menu_cursor_cancel(context, context->menu);
	//}
}
