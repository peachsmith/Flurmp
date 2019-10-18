#include "entity/pellet.h"
#include "entity/entity.h"
#include "core/resource.h"
#include "core/schedule.h"
#include "core/input.h"
#include "core/animation.h"


/* -------------------------------------------------------------- */
/*                   entity behavior functions                    */
/* -------------------------------------------------------------- */

/**
 * The collision callback for a pellet entity.
 * If a pellet collides with a solid object, the pellet should
 * vanish from the screen and become usable again.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - the pellet entity
 *   fl_entity - the entity with which the pellet has collided
 *   int - the direction from which the collision occurred
 *   int - the axis
 */
static void collide(fl_context*, fl_entity*, fl_entity*, int, int);

/**
 * Updates the state of a pellet entity.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - a pellet entity
 *   int - an axis
 */
static void update(fl_context*, fl_entity*, int);

/**
 * Renders a pellet entity to the screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - a pellet entity
 */
static void render(fl_context*, fl_entity*);




/* -------------------------------------------------------------- */
/*                       update functions                         */
/* -------------------------------------------------------------- */

/**
 * Handles horizontal movement events such as walking and inertia.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - a pellet entity
 */
static void horizontal_movement(fl_context*, fl_entity*);

/**
 * Handles vertical movement events such as jumping and gravity.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - a pellet entity
 */
static void vertical_movement(fl_context*, fl_entity*);



/* -------------------------------------------------------------- */
/*                      schedule functions                        */
/* -------------------------------------------------------------- */

/**
 * Determines which section of the pellet texture to use when
 * rendering a pellet entity.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_schedule - a schedule
 *   void* - a reference to a pellet entity cast as a void pointer
 */
static void animate(fl_context*, fl_schedule*, void*);



/* -------------------------------------------------------------- */
/*                      utility functions                         */
/* -------------------------------------------------------------- */

/**
 * Renders the hitbox of a pellet entity.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - a pellet entity
 */
static void render_hitbox(fl_context* context, fl_entity* self);



/* -------------------------------------------------------------- */
/*                    pellet.h implementation                     */
/* -------------------------------------------------------------- */

fl_entity* fl_create_pellet(int x, int y)
{
	fl_entity* pellet = fl_alloc(fl_entity, 1);

	if (pellet == NULL) return pellet;

	pellet->next = NULL;
	pellet->tail = NULL;
	pellet->type = FLURMP_ENTITY_PELLET;
	pellet->flags = 0;
	pellet->x_v = 0;
	pellet->y_v = 0;
	pellet->x = x;
	pellet->y = y;
	pellet->frame = 0;
	pellet->life = 10;

	return pellet;
}

int fl_load_pellet_schedules(fl_context* context, fl_entity* player)
{
	/*fl_schedule* w = fl_create_schedule(animate, -1, pellet);

	if (w == NULL)
		return 0;

	fl_add_schedule(context, w);*/

	return 1;
}

void fl_register_pellet_type(fl_context* context, fl_entity_type* et)
{
	et->w = 20;
	et->h = 20;

	et->collide = collide;
	et->update = update;
	et->render = render;

	et->texture = NULL;
	et->animations = NULL;
	et->animation_count = 0;
}



/* -------------------------------------------------------------- */
/*           entity behavior functions (implementation)           */
/* -------------------------------------------------------------- */

static void collide(fl_context* context, fl_entity* self, fl_entity* other, int collided, int axis)
{
	switch (other->type)
	{
	case FLURMP_ENTITY_DOOR:
	case FLURMP_ENTITY_SIGN:
	case FLURMP_ENTITY_PLAYER:
	case FLURMP_ENTITY_PELLET:
		break;

	default:
		self->flags &= ~(FLURMP_ALIVE_FLAG);
		break;
	}
}

static void update(fl_context* context, fl_entity* self, int axis)
{
	if (axis == FLURMP_AXIS_X)
	{
		horizontal_movement(context, self);
	}

	if (axis == FLURMP_AXIS_Y)
	{
		vertical_movement(context, self);
	}
}

static void render(fl_context* context, fl_entity* self)
{
	int self_w = context->entity_types[self->type].w;
	int self_h = context->entity_types[self->type].h;
	fl_texture* tex = context->entity_types[self->type].texture->impl.image->texture;

	fl_rect src;
	fl_rect dest;

	fl_set_rect(&src, 0, 0, self_w, self_h);

	dest.x = self->x - context->cam_x;
	dest.y = self->y - context->cam_y;
	dest.w = self_w;
	dest.h = self_h;

	fl_draw(context, tex, &src, &dest, 0);
}



/* -------------------------------------------------------------- */
/*                update functions (implementation)               */
/* -------------------------------------------------------------- */

static void horizontal_movement(fl_context* context, fl_entity* self)
{
	int self_w = context->entity_types[self->type].w;
	int self_h = context->entity_types[self->type].h;

	self->x += self->x_v;
}

static void vertical_movement(fl_context* context, fl_entity* self)
{
	int self_w = context->entity_types[self->type].w;
	int self_h = context->entity_types[self->type].h;

	self->y += self->y_v;
}



/* -------------------------------------------------------------- */
/*                utility functions (implementation)              */
/* -------------------------------------------------------------- */

static void render_hitbox(fl_context* context, fl_entity* self)
{
	int self_w = context->entity_types[self->type].w;
	int self_h = context->entity_types[self->type].h;
	fl_texture* tex = context->entity_types[self->type].texture->impl.image->texture;

	fl_rect src;
	fl_rect dest;

	fl_set_rect(&src, 0, 0, 20, 20);

	dest.x = self->x - context->cam_x;
	dest.y = self->y - context->cam_y;
	dest.w = self_w;
	dest.h = self_h;

	fl_draw(context, tex, &src, &dest, 0);
}



/* -------------------------------------------------------------- */
/*               schedule functions (implementation)              */
/* -------------------------------------------------------------- */

static void animate(fl_context* context, fl_schedule* w, void* self)
{

}

/**
 * Launches a pellet.
 * The pellet will move forward for 80 iterations or until
 * it collides with a solid object.
 */
static void launch(fl_context* context, fl_schedule* w, void* self)
{
	fl_entity* en = (fl_entity*)self;

	if (w->counter >= w->limit || !(en->flags & FLURMP_ALIVE_FLAG))
	{
		w->done = 1;
		en->flags &= ~(FLURMP_ALIVE_FLAG);
	}

	w->counter++;
}

int fl_schedule_pellet(fl_context* context, fl_entity* pellet)
{
	pellet->flags |= FLURMP_ALIVE_FLAG;
	pellet->x = context->pco->x + 20;
	pellet->y = context->pco->y + 10;
	pellet->x_v = 0;
	pellet->y_v = 0;

	if (context->pco->flags & FLURMP_MIRROR_FLAG)
		pellet->x_v = -4;
	else
		pellet->x_v = 4;

	fl_schedule* w = fl_create_schedule(launch, 50, pellet);

	if (w == NULL)
		return 0;

	fl_add_schedule(context, w);

	return 1;
}
