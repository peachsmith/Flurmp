#include "entity/door.h"
#include "entity/entity.h"
#include "core/resource.h"
#include "core/dialog.h"
#include "scene/scene.h"




/* -------------------------------------------------------------- */
/*                   entity behavior functions                    */
/* -------------------------------------------------------------- */

/**
 * The collision callback for the door entity.
 * Colliding with the door should not have any affect on the position
 * of the entities colliding with it.
 * If the interact flag for the colliding entity was toggled on while
 * colliding with the door, the context should transition to another scene.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - a door entity
 *   fl_entity - an entity with which the door has collided
 *   int - the direction from which the collision occurred
 *   int - the axis
 */
static void collide(fl_context*, fl_entity*, fl_entity*, int, int);

/**
 * Does nothing.
 * A door is not affected by physics.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - a door entity
 *   int - an axis
 */
static void update(fl_context*, fl_entity*, int);

/**
 * Renders a door entity to the screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity - the door entity
 */
static void render(fl_context*, fl_entity*);




/* -------------------------------------------------------------- */
/*            entity behavior functions (implementation)          */
/* -------------------------------------------------------------- */

static void collide(fl_context* context, fl_entity* self, fl_entity* other, int collided, int axis)
{
	if (other->flags & FLURMP_INTERACT_FLAG)
	{
		other->flags &= ~(FLURMP_INTERACT_FLAG);

		if (context->scene == FLURMP_SCENE_TEST_1)
			fl_schedule_scene_transition(context, context->scene, FLURMP_SCENE_TEST_2);
		else if (context->scene == FLURMP_SCENE_TEST_2)
			fl_schedule_scene_transition(context, context->scene, FLURMP_SCENE_TEST_1);
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

	fl_set_rect(&src, 0, 0, 30, 40);

	dest.x = self->x - context->cam_x;
	dest.y = self->y - context->cam_y;
	dest.w = self_w;
	dest.h = self_h;

	fl_draw(context, tex, &src, &dest, 0);
}



/* -------------------------------------------------------------- */
/*                     door.h implementation                      */
/* -------------------------------------------------------------- */

fl_entity* fl_create_door(int x, int y)
{
	fl_entity* door = fl_alloc(fl_entity, 1);

	if (door == NULL) return door;

	door->next = NULL;
	door->tail = NULL;
	door->type = FLURMP_ENTITY_DOOR;
	door->flags = FLURMP_ALIVE_FLAG;
	door->x_v = 0;
	door->y_v = 0;
	door->x = x;
	door->y = y;
	door->life = 1;

	return door;
}

void fl_register_door_type(fl_context* context, fl_entity_type* et)
{
	et->w = 30;
	et->h = 40;

	et->collide = collide;
	et->update = update;
	et->render = render;

	et->texture = NULL;
	et->animations = NULL;
	et->animation_count = 0;
}
