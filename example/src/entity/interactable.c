#include "interactable.h"
#include "flurmp_impl.h"
#include <stdio.h>

/* entity function prototypes */
static void fl_collide_interactable(fl_context*, fl_entity*, fl_entity*, int, int);
static void fl_update_interactable(fl_context*, fl_entity*, int);
static void fl_render_interactable(fl_context*, fl_entity*);

fl_entity* fl_create_interactable(int x, int y, int w, int h)
{
	fl_entity* rect = malloc(sizeof(fl_entity));

	if (rect == NULL) return rect;

	rect->next = NULL;
	rect->tail = NULL;
	rect->type = FLURMP_ENTITY_INTERACTABLE;
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

void fl_register_interactable_type(fl_entity_type * et)
{
	//et->w = 30;
	//et->h = 40;

	et->collide = fl_collide_interactable;
	et->update = fl_update_interactable;
	et->render = fl_render_interactable;
}

static void fl_collide_interactable(fl_context * context, fl_entity * self, fl_entity * other, int collided, int axis)
{
	if (other->flags & FLURMP_INTERACT_FLAG)
	{
		other->flags &= ~(FLURMP_INTERACT_FLAG);
		printf("interacting with %d\n", other->type);
	}
}

static void fl_update_interactable(fl_context * context, fl_entity * self, int axis)
{

}

static void fl_render_interactable(fl_context * context, fl_entity * self)
{
	SDL_Rect r;
	r.x = self->x - context->cam_x;
	r.y = self->y - context->cam_y;
	r.w = self->w;
	r.h = self->h;

	SDL_SetRenderDrawColor(context->renderer, 243, 158, 0, 255);
	SDL_RenderFillRect(context->renderer, &r);
}