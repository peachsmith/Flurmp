#include "block_200_50.h"
#include "entity.h"
#include "resource.h"

#include <stdio.h>

/* entity function prototypes */
static void collide(fl_context*, fl_entity*, fl_entity*, int, int);
static void update(fl_context*, fl_entity*, int);
static void render(fl_context*, fl_entity*);

fl_entity* fl_create_block_200_50(int x, int y)
{
	fl_entity* block = malloc(sizeof(fl_entity));

	if (block == NULL) return block;

	block->next = NULL;
	block->tail = NULL;
	block->type = FLURMP_ENTITY_BLOCK_200_50;
	block->flags = 0;
	block->x_v = 0;
	block->y_v = 0;
	block->x = x;
	block->y = y;

	return block;
}

void fl_register_block_200_50_type(fl_context* context, fl_entity_type* et)
{
	et->w = 200;
	et->h = 50;

	et->collide = collide;
	et->update = update;
	et->render = render;

	et->texture = fl_load_bmp(context, "resources/images/block_200_50.bmp");
}

static void collide(fl_context* context, fl_entity* self, fl_entity* other, int collided, int axis)
{
	switch (other->type)
	{
	case FLURMP_ENTITY_BLOCK_200_50:
	case FLURMP_ENTITY_SIGN:
		return;

	default:
		break;
	}

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

	if (axis == FLURMP_AXIS_Y)
	{
		other->y_v = 0;

		if (collided == 2 || collided == 3)
		{
			/* top */
			other->y = other->y - (other->y + other_h - self->y);
			other->flags &= ~(FLURMP_JUMP_FLAG);
		}
		else if (collided == 1 || collided == 4)
		{
			/* bottom */
			other->y = other->y + (self->y + self_h - other->y);
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
	dest.w = 50;
	dest.h = 50;

	/* This entity is 200 pixels wide, so copy a 50 pixel
	   tile 40 times */

	SDL_RenderCopy(context->renderer, tex, &src, &dest);

	dest.x += dest.w;
	SDL_RenderCopy(context->renderer, tex, &src, &dest);

	dest.x += dest.w;
	SDL_RenderCopy(context->renderer, tex, &src, &dest);

	dest.x += dest.w;
	SDL_RenderCopy(context->renderer, tex, &src, &dest);
}