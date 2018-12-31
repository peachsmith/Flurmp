#include "player.h"

/* entity function prototypes */
static void fl_collide_player(fl_context*, fl_entity*, fl_entity*, int, int);
static void fl_update_player(fl_context*, fl_entity*, int);
static void fl_render_player(fl_context*, fl_entity*);

fl_entity* fl_create_player(int x, int y, int w, int h)
{
	fl_entity* rect = malloc(sizeof(fl_entity));

	if (rect == NULL) return rect;

	rect->next = NULL;
	rect->tail = NULL;
	rect->type = FL_PCO;
	rect->flags = 0;
	rect->x_v = 0;
	rect->y_v = 0;
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
	rect->collide = fl_collide_player;
	rect->update = fl_update_player;
	rect->render = fl_render_player;
	rect->texture = NULL;

	return rect;
}

static void fl_collide_player(fl_context* context, fl_entity* self, fl_entity* other, int collided, int axis)
{

}

static void fl_update_player(fl_context* context, fl_entity* self, int axis)
{
	/* horizontal movement */
	if (context->keystates[FLURMP_SC_A])
	{
		if (self->x_v > -2)
		{
			self->x_v--;
		}
	}
	if (context->keystates[FLURMP_SC_D])
	{
		if (self->x_v < 2)
		{
			self->x_v++;
		}
	}

	/* jumping */
	if (context->keystates[FLURMP_SC_SPACE] && self->flags & FLURMP_JUMP_FLAG)
	{
		self->y_v -= 12;
		self->flags &= ~(FLURMP_JUMP_FLAG);
	}

	if (axis == FLURMP_AXIS_X)
	{
		self->x += self->x_v;

		/* x inertia */
		if (self->x_v > 0) self->x_v--;
		if (self->x_v < 0) self->x_v++;
	}
	else if (axis == FLURMP_AXIS_Y)
	{
		/* gravity */
		if (self->y_v < 4) self->y_v += 1;

		self->y += self->y_v;
	}
}

static void fl_render_player(fl_context* context, fl_entity* self)
{
	SDL_Rect dest;
	dest.x = self->x;
	dest.y = self->y;
	dest.w = self->w;
	dest.h = self->h;

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = 50;
	src.h = 50;

	SDL_RenderCopy(context->renderer, self->texture, &src, &dest);
}
