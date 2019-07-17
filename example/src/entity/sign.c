#include "sign.h"
#include "entity.h"
#include "resource.h"
#include "dialog.h"



/**
 * The message to be displayed when an entity interacts with a sign.
 */
static const char* statements[2] = {
	"This is a sign.",
	"It's purpose is to display information."
};



/* -------------------------------------------------------------- */
/*                   entity behavior functions                    */
/* -------------------------------------------------------------- */

/**
 * The collision callback for the sign entity.
 * Colliding with the sign should not have any affect on the position
 * of the entities colliding with it.
 * If the interact flag for the colliding entity was toggled on while
 * colliding with the sign, a dialog should be produced.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - a sign entity
 *   fl_entity - an entity with which the sign has collided
 *   int - the direction from which the collision occurred
 *   int - the axis
 */
static void collide(fl_context*, fl_entity*, fl_entity*, int, int);

/**
 * Does nothing.
 * A sign is not affected by physics.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - a sign entity
 *   int - an axis
 */
static void update(fl_context*, fl_entity*, int);

/**
 * Renders a sign entity to the screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - the player entity
 */
static void render(fl_context*, fl_entity*);

/**
 * The first dialog callback.
 * This should display the second statement.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
static void first_cb(fl_context* context);



/* -------------------------------------------------------------- */
/*            entity behavior functions (implementation)          */
/* -------------------------------------------------------------- */

static void collide(fl_context* context, fl_entity* self, fl_entity* other, int collided, int axis)
{
	if (other->flags & FLURMP_INTERACT_FLAG)
	{
		other->flags &= ~(FLURMP_INTERACT_FLAG);

		fl_dialog_write(context, statements[0], 1, first_cb, 0);
	}
}

static void update(fl_context* context, fl_entity* self, int axis)
{

}

static void render(fl_context* context, fl_entity* self)
{
	int self_w = context->entity_types[self->type].w;
	int self_h = context->entity_types[self->type].h;
	SDL_Texture* tex = context->entity_types[self->type].texture->impl.image->texture;

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 50;
	src.h = 50;

	dest.x = self->x - context->cam_x;
	dest.y = self->y - context->cam_y;
	dest.w = self_w;
	dest.h = self_h;

	SDL_RenderCopy(context->renderer, tex, &src, &dest);
}

static void first_cb(fl_context* context)
{
	fl_dialog_write(context, statements[1], 1, NULL, 0);
}



/* -------------------------------------------------------------- */
/*                     sign.h implementation                      */
/* -------------------------------------------------------------- */

fl_entity* fl_create_sign(int x, int y)
{
	fl_entity* sign = fl_alloc(fl_entity, 1);

	if (sign == NULL) return sign;

	sign->next = NULL;
	sign->tail = NULL;
	sign->type = FLURMP_ENTITY_SIGN;
	sign->flags = FLURMP_ALIVE_FLAG;
	sign->x_v = 0;
	sign->y_v = 0;
	sign->x = x;
	sign->y = y;
	sign->life = 1;

	return sign;
}

void fl_register_sign_type(fl_context* context, fl_entity_type* et)
{
	et->w = 30;
	et->h = 40;

	et->collide = collide;
	et->update = update;
	et->render = render;

	et->texture = NULL;
}
