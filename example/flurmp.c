/*
 * an implementation of flurmp using SDL2
 */

#include "flurmp.h"

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

fl_context fl_create_context()
{
	fl_context context;

	context.window = SDL_CreateWindow("test", 100, 100,
		FLURMP_WINDOW_WIDTH,
		FLURMP_WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (context.window == NULL)
	{
		context.error = 1;
		return context;
	}

	context.renderer = SDL_CreateRenderer(context.window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (context.renderer == NULL) context.error = 2;

	context.keystates = SDL_GetKeyboardState(NULL);

	context.entities = NULL;
	context.done = 0;

	return context;
}

void fl_destroy_context(fl_context context)
{
	if (context.renderer != NULL) SDL_DestroyRenderer(context.renderer);
	if (context.window != NULL) SDL_DestroyWindow(context.window);
	fl_entity *en = context.entities;
	fl_entity *next;
	while (en != NULL)
	{
		next = en->next;
		free(en);
		en = next;
	}
}

void fl_add_entity(fl_context *context, fl_entity *entity)
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

	if (a->x >= b->x && a->x <= b->x + b->w)
	{
		if (a->y >= b->y && a->y <= b->y + b->h)
		{
			/* bottom right */
			collision = 1;
		}
		else if (a->y + a->h >= b->y && a->y <= b->y + b->h)
		{
			/* top right */
			collision = 2;
		}
	}

	if (b->x >= a->x && b->x <= a->x + a->w)
	{
		if (b->y >= a->y && b->y <= a->y + a->h)
		{
			/* top left */
			collision = 3;
		}
		else if (b->y + b->h >= a->y && b->y <= a->y + a->h)
		{
			/* bottom left */
			collision = 4;
		}
	}

	return collision;
}

int fl_poll_event(fl_context *context)
{
	return SDL_PollEvent(&(context->event));
}

void fl_handle_event(fl_context *context)
{
	if (context->event.type == FLURMP_QUIT) context->done = 1;
}

void fl_update(fl_context context)
{
	fl_entity *en = context.entities;
	fl_entity *next;

	/* update state with respect to X axis */
	while (en != NULL)
	{
		en->update(context, en, FLURMP_AXIS_X);
		en = en->next;
	}

	en = context.entities;

	/* handle collisions with respect to X axis */
	while (en != NULL)
	{
		next = en->next;

		while (next != NULL)
		{
			int collided = fl_detect_collision(en, next);
			if (collided) en->collide(context, en, next, collided, FLURMP_AXIS_X);
			next = next->next;
		}

		en = en->next;
	}

	en = context.entities;

	/* update state with respect to Y axis */
	while (en != NULL)
	{
		en->update(context, en, FLURMP_AXIS_Y);
		en = en->next;
	}

	en = context.entities;

	/* handle collisions with respect to Y axis */
	while (en != NULL)
	{
		next = en->next;

		while (next != NULL)
		{
			int collided = fl_detect_collision(en, next);
			if (collided) en->collide(context, en, next, collided, FLURMP_AXIS_Y);
			next = next->next;
		}

		en = en->next;
	}
}

void fl_render(fl_context context)
{
	SDL_SetRenderDrawColor(context.renderer, 0, 0, 0, 255);
	SDL_RenderClear(context.renderer);

	fl_entity *en = context.entities;

	while (en != NULL)
	{
		en->render(context, en);
		en = en->next;
	}

	SDL_RenderPresent(context.renderer);
}

void fl_sleep(int ms)
{
	SDL_Delay(ms);
}