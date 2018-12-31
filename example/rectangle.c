#include "rectangle.h"

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
	rect->type = FL_SOLID;
	rect->flags = 0;
	rect->x_v = 0;
	rect->y_v = 0;
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
	rect->collide = fl_collide_rectangle;
	rect->update = fl_update_rectangle;
	rect->render = fl_render_rectangle;

	return rect;
}

static void fl_collide_rectangle(fl_context* context, fl_entity* self, fl_entity* other, int collided, int axis)
{
	if (axis == FLURMP_AXIS_X)
	{
		other->x_v = 0;

		if (collided == 3 || collided == 4)
		{
			/* left */
			other->x = other->x - (other->x + other->w - self->x);// -1;
		}
		else if (collided == 1 || collided == 2)
		{
			/* right */
			other->x = other->x + (self->x + self->w - other->x);// +1;
		}
	}

	if (axis == FLURMP_AXIS_Y)
	{
		other->y_v = 0;

		if (collided == 2 || collided == 3)
		{
			/* top */
			other->y = other->y - (other->y + other->h - self->y);// -1;
			other->flags |= FLURMP_JUMP_FLAG;
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
	r.x = self->x;
	r.y = self->y;
	r.w = self->w;
	r.h = self->h;

	SDL_SetRenderDrawColor(context->renderer, 100, 80, 0, 255);
	SDL_RenderFillRect(context->renderer, &r);
	SDL_SetRenderDrawColor(context->renderer, 0, 0, 0, 255);
}