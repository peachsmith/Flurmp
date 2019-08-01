#include "entity/player.h"
#include "entity/entity.h"
#include "core/resource.h"
#include "core/schedule.h"
#include "core/input.h"
#include "core/animation.h"


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



/* -------------------------------------------------------------- */
/*                      schedule functions                        */
/* -------------------------------------------------------------- */

/**
 * Determines which section of the player texture to use when
 * rendering a player entity.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_schedule - a schedule
 *   void* - a reference to a player entity cast as a void pointer
 */
static void animate(fl_context*, fl_schedule*, void*);



/* -------------------------------------------------------------- */
/*                      utility functions                         */
/* -------------------------------------------------------------- */

/**
 * Renders the hitbox of a player entity.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - a player entity
 */
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

int fl_load_player_schedules(fl_context* context, fl_entity* player)
{
	fl_schedule* w = fl_create_schedule(animate, -1, player);

	if (w == NULL)
		return 0;

	fl_add_schedule(context, w);
	
	return 1;
}

void fl_register_player_type(fl_context* context, fl_entity_type* et)
{
	et->w = 30;
	et->h = 40;

	et->collide = collide;
	et->update = update;
	et->render = render;

	et->texture = NULL;

	fl_animation** animations = fl_alloc(fl_animation*, 3);

	if (animations == NULL)
		return;

	/* Create the standing animation. */
	fl_animation* stand = fl_create_animation(1);
	if (stand == NULL)
	{
		fl_free(animations);
		return;
	}
	fl_set_rect(&(stand->frames[0]), 0, 0, 50, 50);

	/* Create the walking animation. */
	fl_animation* walk = fl_create_animation(3);
	if (walk == NULL)
	{
		fl_destroy_animation(stand);
		fl_free(animations);
		return;
	}
	fl_set_rect(&(walk->frames[0]), 0, 0, 50, 50);
	fl_set_rect(&(walk->frames[1]), 50, 0, 50, 50);
	fl_set_rect(&(walk->frames[2]), 100, 0, 50, 50);

	/* Create the jumping animation. */
	fl_animation* jump = fl_create_animation(1);
	if (jump == NULL)
	{
		fl_destroy_animation(stand);
		fl_destroy_animation(jump);
		fl_free(animations);
		return;
	}
	fl_set_rect(&(jump->frames[0]), 50, 0, 50, 50);

	animations[0] = stand;
	animations[1] = walk;
	animations[2] = jump;

	et->animations = animations;
	et->animation_count = 3;
}



/* -------------------------------------------------------------- */
/*           entity behavior functions (implementation)           */
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
}

static void render(fl_context* context, fl_entity* self)
{
	int self_w = context->entity_types[self->type].w;
	int self_h = context->entity_types[self->type].h;
	fl_texture* tex = context->entity_types[self->type].texture->impl.image->texture;

	fl_rect dest;

	int f = 0;

	dest.x = self->x - 10 - context->cam_x;
	dest.y = self->y - 8 - context->cam_y;
	dest.w = self_w + 20;
	dest.h = self_h + 10;

	if (self->flags & FLURMP_MIRROR_FLAG)
		fl_draw(context, tex, self->frame, &dest, 1);

	else
		fl_draw(context, tex, self->frame, &dest, 0);

	/* render_hitbox(context, self); */
}



/* -------------------------------------------------------------- */
/*                update functions (implementation)               */
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

	/* Reset the air flag. */
	self->flags |= FLURMP_AIR_FLAG;

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



/* -------------------------------------------------------------- */
/*               schedule functions (implementation)              */
/* -------------------------------------------------------------- */

static void animate(fl_context* context, fl_schedule* w, void* self)
{
	fl_entity* en = (fl_entity*)self;
	fl_animation* a;

	if (en->flags & FLURMP_AIR_FLAG)
	{
		/* in the air */
		a = context->entity_types[en->type].animations[2];

		if (w->counter > 0)
			w->counter = 0;

		en->frame = &(a->frames[w->counter]);
	}
	else if (en->x_v != 0)
	{
		/* walking */
		a = context->entity_types[en->type].animations[1];

		if (w->counter / 4 >= a->frame_count)
			w->counter = 0;

		en->frame = &(a->frames[w->counter / 4]);

		w->counter++;
	}
	else
	{
		/* standing */
		a = context->entity_types[en->type].animations[0];

		if (w->counter > 0)
			w->counter = 0;

		en->frame = &(a->frames[w->counter]);
	}
}

/**
 * A scheduled action for walking to the right and jumping once.
 * For the first 80 iterations, the target will move to the right.
 *
 * On the 60th iteration, the target will jump.
 */
static void walk_to_the_right_and_jump(fl_context* context, fl_schedule* w, void* self)
{
	fl_entity* en = (fl_entity*)self;

	if (w->counter < 80)
	{
		if (en->x_v < 2)
			en->x_v += 2;

		en->flags |= FLURMP_RIGHT_FLAG;
	}

	if (w->counter == 60)
	{
		en->y_v -= 12;
		en->flags |= FLURMP_AIR_FLAG;
	}
	else if (w->counter == w->limit)
	{
		fl_input_handler* ih = fl_get_input_handler(context);
		fl_pop_input_handler(context);
		fl_destroy_input_handler(ih);
		w->done = 1;
	}

	w->counter++;
}

int fl_schedule_walk(fl_context* context, fl_entity* player)
{
	fl_schedule* w = fl_create_schedule(walk_to_the_right_and_jump, 120, player);

	if (w == NULL)
		return 0;

	fl_add_schedule(context, w);

	fl_input_handler* ih = fl_create_input_handler(NULL);

	fl_push_input_handler(context, ih);

	return 1;
}
