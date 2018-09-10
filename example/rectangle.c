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