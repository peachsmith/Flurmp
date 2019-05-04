#include "player.h"
#include "input.h"

#include <stdio.h>




/* -------------------------------------------------------------- */
/*                   entity behavior functions                    */
/* -------------------------------------------------------------- */

/**
 * The collision callback for the player entity.
 * Currently, colliding with a player entity does not have any effect on other
 * entities.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - the player entity
 */
static void collide_player(fl_context*, fl_entity*, fl_entity*, int, int);

/**
 * Performs the following operations:
 * horizontal camera adjustment (x axis)
 * horizontal movement          (x axis, input handling)
 * vertical movement            (y axis, input handling)
 * entity interaction           (either axis, input handling)
 * animation                    (either axis)
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - the player entity
 */
static void update_player(fl_context*, fl_entity*, int);

/**
 * Renders the player entity to the screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - the player entity
 */
static void render_player(fl_context*, fl_entity*);




/* -------------------------------------------------------------- */
/*                       update functions                         */
/* -------------------------------------------------------------- */

/**
 * Updates the camera position to keep the player near the center of the
 * screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - the player entity
 */
static void adjust_camera_horizontal(fl_context*, fl_entity*);

/**
 * Handles horizontal movement events such as walking and inertia.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - the player entity
 */
static void horizontal_movement(fl_context*, fl_entity*);

/**
 * Handles vertical movement events such as jumping and gravity.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - the player entity
 */
static void vertical_movement(fl_context*, fl_entity*);

/**
 * Handles entity interaction upon user input.
 * Currently, the only interaction that is supported is the interaction
 * with the "interactable" entity by pressing the Z key.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - the player entity
 */
static void interact(fl_context*, fl_entity*);

/**
 * This function determines which section of a sprite sheet should be used
 * when rendering the player entity at any given time.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - the player entity
 */
static void animate(fl_context*, fl_entity*);



/* -------------------------------------------------------------- */
/*         entity creation functions (publicly exposed)           */
/* -------------------------------------------------------------- */

fl_entity* fl_create_player(int x, int y, int w, int h)
{
	fl_entity* rect = malloc(sizeof(fl_entity));

	if (rect == NULL) return rect;

	rect->next = NULL;
	rect->tail = NULL;
	rect->type = FLURMP_ENTITY_PLAYER;
	rect->flags = 0;
	rect->x_v = 0;
	rect->y_v = 0;
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
	rect->frame = 0;
	rect->texture = NULL;

	return rect;
}

void fl_register_player_type(fl_entity_type * et)
{
	//et->w = 30;
	//et->h = 40;

	et->collide = collide_player;
	et->update = update_player;
	et->render = render_player;
}




static void collide_player(fl_context * context, fl_entity * self, fl_entity * other, int collided, int axis)
{
	/* Nothing to see here, folks. Move along. */
}

static void update_player(fl_context * context, fl_entity * self, int axis)
{
	if (axis == FLURMP_AXIS_X)
	{
		/* horizontal camera adjustment */
		adjust_camera_horizontal(context, self);

		/* horizontal movement */
		horizontal_movement(context, self);
	}

	if (axis == FLURMP_AXIS_Y)
	{
		/* vertical movement */
		vertical_movement(context, self);
	}

	/* entity interaction */
	interact(context, self);

	/* animation */
	animate(context, self);
}

static void render_player(fl_context * context, fl_entity * self)
{
	/* determine where to render the player on the x axis */

	int real_x = self->x;

	SDL_Rect dest;

	/* int self_w = context->entity_types[self->type].w;
	int self_h = context->entity_types[self->type].h; */

	dest.x = self->x - 10 - context->cam_x;
	dest.y = self->y - 8 - context->cam_y;
	dest.w = self->w + 20;
	dest.h = self->h + 10;

	int f = 0;

	/* calculate the sprite coordinate based on
	 * frame count and entity state
	 */
	if (self->flags & FLURMP_JUMP_FLAG)
		f = 1;
	else if (self->frame > 10 && self->frame < 20)
		f = 1;
	else if (self->frame > 20 && self->frame < 30)
		f = 2;

	SDL_Rect src;
	src.x = 50 * f;
	src.y = 0;
	src.w = 50;
	src.h = 50;

	SDL_Rect hb;
	hb.x = self->x - context->cam_x;
	hb.y = self->y - context->cam_y;
	hb.w = self->w;
	hb.h = self->h;

	if (self->flags & FLURMP_LEFT_FLAG)
		SDL_RenderCopyEx(context->renderer, self->texture, &src, &dest, 0, NULL, SDL_FLIP_HORIZONTAL);

	else
		SDL_RenderCopyEx(context->renderer, self->texture, &src, &dest, 0, NULL, SDL_FLIP_NONE);

	/* render player hitbox */
	/* SDL_SetRenderDrawColor(context->renderer, 255, 0, 255, 255);
	SDL_RenderDrawRect(context->renderer, &hb); */
}




static void adjust_camera_horizontal(fl_context * context, fl_entity * self)
{
	int cam_d = context->cam_x - self->x;

	/* camera x window: [-221, -399] */
	if (!(self->flags & FLURMP_LEFT_FLAG))
	{
		if (self->x_v == 0 && cam_d < -290)
		{
			context->cam_x += 2;

			if (context->cam_x - self->x > -290)
			{
				int correction = (context->cam_x - self->x) + 290;
				context->cam_x -= correction;
			}
		}
		else if (self->x_v == 0 && cam_d > -290)
		{
			context->cam_x -= 2;

			if (context->cam_x - self->x < -290)
			{
				int correction = -290 - (context->cam_x - self->x);
				context->cam_x += correction;
			}
		}
	}
	else if (self->flags & FLURMP_LEFT_FLAG)
	{
		if (self->x_v == 0 && cam_d > -330)
		{
			context->cam_x -= 2;

			if (context->cam_x - self->x < -330)
			{
				int correction = -330 - (context->cam_x - self->x);
				context->cam_x += correction;
			}
		}
		else if (self->x_v == 0 && cam_d < -330)
		{
			context->cam_x += 2;

			if (context->cam_x - self->x > -330)
			{
				int correction = (context->cam_x - self->x) + 330;
				context->cam_x -= correction;
			}
		}
	}
}

static void horizontal_movement(fl_context * context, fl_entity * self)
{
	/* walking (input handling) */
	if (context->input.keystates[FLURMP_SC_A])
	{
		if (!(self->flags & FLURMP_LEFT_FLAG))
			self->flags |= FLURMP_LEFT_FLAG;

		if (self->x_v > -2)
			self->x_v -= 2;
	}

	if (context->input.keystates[FLURMP_SC_D])
	{
		if (self->flags & FLURMP_LEFT_FLAG)
			self->flags &= ~(FLURMP_LEFT_FLAG);

		if (self->x_v < 2)
			self->x_v += 2;
	}

	/* inertia */
	/* if the player's x velocity is not 0, then the
	   camera x axis adjustment doesn't occur. */
	self->x += self->x_v;

	/* int self_w = context->entity_types[self->type].w; */

	if (self->x_v < 0 && self->x - context->cam_x <= FLURMP_LEFT_BOUNDARY)
	{
		context->cam_x += self->x_v;
	}

	if (self->x_v > 0 && self->x + self->w - context->cam_x >= FLURMP_RIGHT_BOUNDARY)
	{
		context->cam_x += self->x_v;
	}

	if (self->x_v > 0)
		self->x_v--;

	if (self->x_v < 0)
		self->x_v++;
}

static void vertical_movement(fl_context * context, fl_entity * self)
{
	/* jumping (input handling) */
	if (context->input.keystates[FLURMP_SC_SPACE])
	{
		if (!context->input.inputs[FLURMP_INPUT_SPACE])
		{
			context->input.inputs[FLURMP_INPUT_SPACE] = 1;

			if (!(self->flags & FLURMP_JUMP_FLAG))
			{
				self->y_v -= 12;
				self->flags |= FLURMP_JUMP_FLAG;
			}
		}
	}
	else if (context->input.inputs[FLURMP_INPUT_SPACE])
		context->input.inputs[FLURMP_INPUT_SPACE] = 0;

	/* gravity (y axis) */
	if (self->y_v < 4) self->y_v += 1;

	self->y += self->y_v;

	/* int self_h = context->entity_types[self->type].h; */

	if (self->y_v < 0 && self->y - context->cam_y <= FLURMP_UPPER_BOUNDARY)
	{
		context->cam_y += self->y_v;
	}

	if (self->y_v > 0 && self->y + self->h - context->cam_y >= FLURMP_LOWER_BOUNDARY)
	{
		context->cam_y += self->y_v;
	}
}

static void interact(fl_context * context, fl_entity * self)
{
	if (self->flags & FLURMP_INTERACT_FLAG)
		self->flags &= ~(FLURMP_INTERACT_FLAG);

	if (context->input.keystates[FLURMP_SC_Z])
	{
		if (!context->input.inputs[FLURMP_INPUT_Z])
		{
			context->input.inputs[FLURMP_INPUT_Z] = 1;

			if (!(self->flags & FLURMP_INTERACT_FLAG))
			{
				self->flags |= FLURMP_INTERACT_FLAG;
			}
		}
	}
	else if (context->input.inputs[FLURMP_INPUT_Z])
		context->input.inputs[FLURMP_INPUT_Z] = 0;
}

static void animate(fl_context * context, fl_entity * self)
{
	if (self->flags & FLURMP_JUMP_FLAG)
	{
		/* in the air */
		self->frame = 1;
	}
	else if (self->x_v != 0)
	{
		/* walking */
		if (self->frame < 30) self->frame++;
		else self->frame = 0;
	}
	else
	{
		/* standing */
		self->frame = 0;
	}
}
