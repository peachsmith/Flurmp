#include "rectangle.h"
#include "flurmp_impl.h"

#include <stdio.h>

/* entity function prototypes */
static void fl_collide_rectangle(fl_context*, fl_entity*, fl_entity*, int, int);
static void fl_update_rectangle(fl_context*, fl_entity*, int);
static void fl_render_rectangle(fl_context*, fl_entity*);

fl_entity* fl_create_rectangle(int x, int y, int w, int h)
{
	fl_entity* rect = malloc(sizeof(fl_entity));

	if (rect == NULL) return rect;

	rect->next = NULL;
	rect->tail = NULL;
	rect->type = FLURMP_ENTITY_RECTANGLE;
	rect->flags = 0;
	rect->x_v = 0;
	rect->y_v = 0;
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
	rect->texture = NULL;

	return rect;
}

void fl_register_rectangle_type(fl_entity_type* et)
{
	//et->w = 30;
	//et->h = 40;

	et->collide = fl_collide_rectangle;
	et->update = fl_update_rectangle;
	et->render = fl_render_rectangle;
}

static void fl_collide_rectangle(fl_context* context, fl_entity* self, fl_entity* other, int collided, int axis)
{
	switch (other->type)
	{
	case FLURMP_ENTITY_RECTANGLE:
	case FLURMP_ENTITY_INTERACTABLE:
		return;

	default:
		break;
	}

	if (axis == FLURMP_AXIS_X)
	{
		other->x_v = 0;

		if (collided == 3 || collided == 4)
		{
			/* left */
			int dif = other->x + other->w - self->x;
			other->x = other->x - dif;// -1;
		}
		else if (collided == 1 || collided == 2)
		{
			/* right */
			int dif = self->x + self->w - other->x;
			other->x = other->x + dif;// +1;
		}
	}

	if (axis == FLURMP_AXIS_Y)
	{
		other->y_v = 0;

		if (collided == 2 || collided == 3)
		{
			/* top */
			other->y = other->y - (other->y + other->h - self->y);// -1;
			other->flags &= ~(FLURMP_JUMP_FLAG);
		}
		else if (collided == 1 || collided == 4)
		{
			/* bottom */
			other->y = other->y + (self->y + self->h - other->y);// +1;
		}
	}
}

static void fl_update_rectangle(fl_context* context, fl_entity* self, int axis)
{

}

static void fl_render_rectangle(fl_context* context, fl_entity* self)
{
	SDL_Rect r;
	r.x = self->x - context->cam_x;
	r.y = self->y - context->cam_y;
	r.w = self->w;
	r.h = self->h;

	SDL_SetRenderDrawColor(context->renderer, 100, 80, 0, 255);
	SDL_RenderFillRect(context->renderer, &r);
}