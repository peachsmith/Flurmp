#include "entity/spike.h"
#include "entity/entity.h"
#include "core/resource.h"
#include "core/input.h"
#include "menu/pause_menu.h"
#include "core/schedule.h"


/* -------------------------------------------------------------- */
/*                   entity behavior functions                    */
/* -------------------------------------------------------------- */

/**
 * The collision callback for a spike entity.
 * When an entity collides with a solid block, it should not be allowed
 * to continue moving in its current direction.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - the spike entity
 *   fl_entity - the entity with which the spike has collided
 *   int - the direction from which the collision occurred
 *   int - the axis
 */
static void collide(fl_context*, fl_entity*, fl_entity*, int, int);

/**
 * Does nothing.
 * A spike has no reason to change.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - the spike entity
 *   int - an axis
 */
static void update(fl_context*, fl_entity*, int);

/**
 * Renders a spike entity to the screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - the spike entity
 */
static void render(fl_context*, fl_entity*);

/**
 * Applies a knockback effect to an entity.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entityt - the enityt to be knocked back
 *   int - the direction from which the collision occurred
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
static int apply_knockback(fl_context* context, fl_entity* other, int dir);



/* -------------------------------------------------------------- */
/*            entity behavior functions (implementation)          */
/* -------------------------------------------------------------- */

static void collide(fl_context* context, fl_entity* self, fl_entity* other, int collided, int axis)
{
	switch (other->type)
	{
	case FLURMP_ENTITY_PLAYER:
		break;

	default:
		return;
	}

	/* Get the entity dimensions from the entity type registry. */
	int self_w = context->entity_types[self->type].w;
	int self_h = context->entity_types[self->type].h;
	int other_w = context->entity_types[other->type].w;
	int other_h = context->entity_types[other->type].h;

	if (axis == FLURMP_AXIS_X)
	{
		other->x_v = 0;

		if (collided == 3 || collided == 4)
		{
			/* left */
			int dif = other->x + other_w - self->x;
			other->x = other->x - dif;
		}
		else if (collided == 1 || collided == 2)
		{
			/* right */
			int dif = self->x + self_w - other->x;
			other->x = other->x + dif;
		}
	}
	else if (axis == FLURMP_AXIS_Y)
	{
		other->y_v = 0;

		if (collided == 2 || collided == 3)
		{
			/* top */
			other->y = other->y - (other->y + other_h - self->y);
			other->flags &= ~(FLURMP_AIR_FLAG);
		}
		else if (collided == 1 || collided == 4)
		{
			/* bottom */
			other->y = other->y + (self->y + self_h - other->y);
		}
	}

	if (!(other->flags & FLURMP_DAMAGE_FLAG))
	{
		other->life--;
		apply_knockback(context, other, collided);
	}
}

static void update(fl_context* context, fl_entity* self, int axis)
{

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
/*                  spike.h implementation                        */
/* -------------------------------------------------------------- */

fl_entity* fl_create_spike(int x, int y)
{
	fl_entity* spike = fl_alloc(fl_entity, 1);

	if (spike == NULL) return spike;

	spike->next = NULL;
	spike->tail = NULL;
	spike->type = FLURMP_ENTITY_SPIKE;
	spike->flags = FLURMP_ALIVE_FLAG;
	spike->x_v = 0;
	spike->y_v = 0;
	spike->x = x;
	spike->y = y;
	spike->life = 1;

	return spike;
}

void fl_register_spike_type(fl_context* context, fl_entity_type* et)
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

/**
 * A scheduled action for applying knockback.
 * For the duration of this scheduled action, the damage flag
 * should be set to indicate invincibility, and the blink flag
 * should be toggled to tell the affected entity's render function
 * to perform a blinking effect.
 *
 * Approximately halfway through the duration of the action,
 * input control should be returned to the user.
 */
static void knockback(fl_context* context, fl_schedule* w, void* self)
{
	fl_entity* en = (fl_entity*)self;

	if (w->counter < w->limit)
	{
		if (w->counter == 20)
		{
			fl_input_handler* ih = fl_get_input_handler(context);
			fl_pop_input_handler(context);
			fl_destroy_input_handler(ih);
		}

		if (w->counter & 1)
			en->flags |= FLURMP_BLINK_FLAG;
		else
			en->flags &= ~(FLURMP_BLINK_FLAG);
	}
	else if (w->counter >= w->limit)
	{
		en->flags &= ~(FLURMP_BLINK_FLAG);
		en->flags &= ~(FLURMP_DAMAGE_FLAG);

		w->done = 1;
	}

	w->counter++;
}

/**
 * The input handler to be used when knockback is in effect.
 * This should be the same as the root input handler, but it
 * should limit entity movement.
 */
static void knockback_input_handler(fl_context* context, fl_input_handler* self)
{
	if (fl_consume_key(context, FLURMP_SC_ESCAPE))
	{
		context->paused = 1;

		fl_menu* pause_menu = fl_create_pause_menu(context);

		fl_push_menu(context, pause_menu);

		/* Hand input control to the pause menu. */
		fl_push_input_handler(context, pause_menu->input_handler);
	}
}

static int apply_knockback(fl_context* context, fl_entity* other, int dir)
{
	other->flags |= FLURMP_DAMAGE_FLAG;

	if (dir == 3 || dir == 4)
	{
		/* left */
		other->x_v = -6;
		other->y_v = -6;
	}
	else if (dir == 1 || dir == 2)
	{
		/* right */
		other->x_v = 6;
		other->y_v = -6;
	}

	fl_schedule* w = fl_create_schedule(knockback, 70, other);

	if (w == NULL)
		return 0;

	fl_add_schedule(context, w);

	/* Limit user input to opening the pause menu. */
	fl_input_handler* ih = fl_create_input_handler(knockback_input_handler);
	fl_push_input_handler(context, ih);

	return 1;
}
