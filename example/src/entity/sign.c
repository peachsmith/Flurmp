#include "sign.h"
#include "entity.h"
#include "resource.h"

#include <stdio.h>

/* entity function prototypes */
static void collide(fl_context*, fl_entity*, fl_entity*, int, int);
static void update(fl_context*, fl_entity*, int);
static void render(fl_context*, fl_entity*);

fl_entity* fl_create_sign(int x, int y)
{
	fl_entity* sign = malloc(sizeof(fl_entity));

	if (sign == NULL) return sign;

	sign->next = NULL;
	sign->tail = NULL;
	sign->type = FLURMP_ENTITY_SIGN;
	sign->flags = 0;
	sign->x_v = 0;
	sign->y_v = 0;
	sign->x = x;
	sign->y = y;

	return sign;
}

void fl_register_sign_type(fl_context* context, fl_entity_type* et)
{
	et->w = 30;
	et->h = 40;

	et->collide = collide;
	et->update = update;
	et->render = render;

	et->texture = fl_load_bmp(context, "resources/images/sign.bmp");
}

static void collide(fl_context* context, fl_entity* self, fl_entity* other, int collided, int axis)
{
	if (other->flags & FLURMP_INTERACT_FLAG)
	{
		other->flags &= ~(FLURMP_INTERACT_FLAG);
		if (!context->dialogs[0]->open)
		{
			context->dialogs[0]->open = 1;
			context->active_dialog = context->dialogs[0];
		}
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