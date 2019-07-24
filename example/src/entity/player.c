#include "player.h"
#include "entity.h"
#include "resource.h"


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
 *   fl_entity - the entity with which the player has collided
 *   int - the direction from which the collision occurred
 *   int - the axis
 */
static void collide(fl_context*, fl_entity*, fl_entity*, int, int);

/**
 * Performs the following operations:
 * horizontal camera adjustment (x axis)
 * horizontal movement          (x axis)
 * vertical movement            (y axis)
 * entity interaction           (either axis)
 * animation                    (either axis)
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - the player entity
 *   int - an axis
 */
static void update(fl_context*, fl_entity*, int);

/**
 * Renders the player entity to the screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - the player entity
 */
static void render(fl_context*, fl_entity*);




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
 * Updates the camera position to keep the player near the center of the
 * screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - the player entity
 */
static void adjust_camera_vertical(fl_context*, fl_entity*);

/**
 * Handles vertical movement events such as jumping and gravity.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - the player entity
 */
static void vertical_movement(fl_context*, fl_entity*);

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
/*                 utility functions functions                    */
/* -------------------------------------------------------------- */
static void render_hitbox(fl_context* context, fl_entity* self);



/* -------------------------------------------------------------- */
/*                    player.h implementation                     */
/* -------------------------------------------------------------- */

fl_entity* fl_create_player(int x, int y)
{
	fl_entity* player = fl_alloc(fl_entity, 1);

	if (player == NULL) return player;

	player->next = NULL;
	player->tail = NULL;
	player->type = FLURMP_ENTITY_PLAYER;
	player->flags = FLURMP_ALIVE_FLAG;
	player->x_v = 0;
	player->y_v = 0;
	player->x = x;
	player->y = y;
	player->frame = 0;
	player->life = 10;

	return player;
}

void fl_register_player_type(fl_context* context, fl_entity_type* et)
{
	et->w = 30;
	et->h = 40;

	et->collide = collide;
	et->update = update;
	et->render = render;

	et->texture = NULL;
}



/* -------------------------------------------------------------- */
/*            entity behavior function implementations            */
/* -------------------------------------------------------------- */

static void collide(fl_context* context, fl_entity* self, fl_entity* other, int collided, int axis)
{
	/* Nothing to see here, folks. Move along. */
}

static void update(fl_context* context, fl_entity* self, int axis)
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
		/* vertical camera adjustment */
		adjust_camera_vertical(context, self);

		/* vertical movement */
		vertical_movement(context, self);
	}

	/* animation */
	if (axis == FLURMP_AXIS_X)
		animate(context, self);
}

static void render(fl_context* context, fl_entity* self)
{
	int self_w = context->entity_types[self->type].w;
	int self_h = context->entity_types[self->type].h;
	fl_texture* tex = context->entity_types[self->type].texture->impl.image->texture;

	fl_rect src;
	fl_rect dest;

	int f = 0;

	/* calculate the sprite coordinate based on
	 * frame count and entity state
	 */
	if (self->flags & FLURMP_JUMP_FLAG)
		f = 1;
	else if (self->frame > 5 && self->frame < 10)
		f = 1;
	else if (self->frame > 10 && self->frame < 15)
		f = 2;

	src.x = 50 * f;
	src.y = 0;
	src.w = 50;
	src.h = 50;

	dest.x = self->x - 10 - context->cam_x;
	dest.y = self->y - 8 - context->cam_y;
	dest.w = self_w + 20;
	dest.h = self_h + 10;

	if (self->flags & FLURMP_LEFT_FLAG)
		fl_draw(context, tex, &src, &dest, 1);

	else
		fl_draw(context, tex, &src, &dest, 0);

	/* render_hitbox(context, self); */
}



/* -------------------------------------------------------------- */
/*                update function implementations                 */
/* -------------------------------------------------------------- */

static void adjust_camera_horizontal(fl_context* context, fl_entity* self)
{
	int cam_d = context->cam_x - self->x;

	/* camera x range: [-221, -399] */
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

static void horizontal_movement(fl_context* context, fl_entity* self)
{
	int self_w = context->entity_types[self->type].w;
	int self_h = context->entity_types[self->type].h;

	/* inertia */
	/* if the player's x velocity is not 0, then the
	   camera x axis adjustment doesn't occur. */
	self->x += self->x_v;

	if (self->x_v < 0 && self->x - context->cam_x <= FLURMP_LEFT_BOUNDARY)
	{
		context->cam_x += self->x_v;
	}

	if (self->x_v > 0 && self->x + self_w - context->cam_x >= FLURMP_RIGHT_BOUNDARY)
	{
		context->cam_x += self->x_v;
	}

	if (self->x_v > 0)
		self->x_v--;

	if (self->x_v < 0)
		self->x_v++;
}

static void adjust_camera_vertical(fl_context* context, fl_entity* self)
{
	int cam_d = context->cam_y - self->y;

	/* camera y range: [-221, -399] */

	if (self->y_v == 0 && cam_d < -250)
	{
		context->cam_y += 2;

		if (context->cam_y - self->y > -250)
		{
			int correction = (context->cam_y - self->y) + 250;
			context->cam_y -= correction;
		}
	}
	else if (self->y_v == 0 && cam_d > -250)
	{
		context->cam_y -= 2;

		if (context->cam_y - self->y < -250)
		{
			int correction = -250 - (context->cam_y - self->y);
			context->cam_y += correction;
		}
	}
}

static void vertical_movement(fl_context* context, fl_entity* self)
{
	int self_w = context->entity_types[self->type].w;
	int self_h = context->entity_types[self->type].h;

	/* Reset the jump flag. */
	self->flags |= FLURMP_JUMP_FLAG;

	/* gravity (y axis) */
	if (self->y_v < 4) self->y_v += 1;

	self->y += self->y_v;

	if (self->y_v < 0 && self->y - context->cam_y <= FLURMP_UPPER_BOUNDARY)
	{
		context->cam_y += self->y_v;
	}

	if (self->y_v > 0 && self->y + self_h - context->cam_y >= FLURMP_LOWER_BOUNDARY)
	{
		context->cam_y += self->y_v;
	}
}

static void animate(fl_context* context, fl_entity* self)
{
	if (self->flags & FLURMP_JUMP_FLAG)
	{
		/* in the air */
		self->frame = 1;
	}
	else if (self->x_v != 0)
	{
		/* walking */
		if (self->frame < 15) self->frame++;
		else self->frame = 0;
	}
	else
	{
		/* standing */
		self->frame = 0;
	}
}

/* -------------------------------------------------------------- */
/*                utility functions (implementation)              */
/* -------------------------------------------------------------- */
static void render_hitbox(fl_context* context, fl_entity* self)
{
	int self_w = context->entity_types[self->type].w;
	int self_h = context->entity_types[self->type].h;

	fl_rect hb;
	hb.x = self->x - context->cam_x;
	hb.y = self->y - context->cam_y;
	hb.w = self_w;
	hb.h = self_h;

	fl_set_draw_color(context, 255, 0, 255, 255);
	fl_draw_rect(context, &hb);
}
