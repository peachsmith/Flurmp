#include "player.h"

#include <stdio.h>

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
	rect->type = FL_PLAYER;
	rect->flags = 0;
	rect->x_v = 0;
	rect->y_v = 0;
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
	rect->frame = 0;
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
	/* horizontal movement (x axis) */
	if (axis == FLURMP_AXIS_X && context->keystates[FLURMP_SC_A])
	{
		if (!(self->flags & FLURMP_LEFT_FLAG))
			self->flags |= FLURMP_LEFT_FLAG;

		if (self->x_v > -2) self->x_v -= 2;
	}
	if (axis == FLURMP_AXIS_X && context->keystates[FLURMP_SC_D])
	{
		if (self->flags & FLURMP_LEFT_FLAG)
			self->flags &= ~(FLURMP_LEFT_FLAG);

		if (self->x_v < 2) self->x_v += 2;
	}

	/* inertia (x axis) */
	if (axis == FLURMP_AXIS_X)
	{
		self->x += self->x_v;

		if (self->x_v < 0 && self->x - context->cam_x <= FLURMP_LEFT_BOUNDARY)
		{
			context->cam_x += self->x_v;
		}

		if (self->x_v > 0 && self->x + self->w - context->cam_x >= FLURMP_RIGHT_BOUNDARY)
		{
			context->cam_x += self->x_v;
		}

		if (self->x_v > 0) self->x_v--;
		if (self->x_v < 0) self->x_v++;
	}

	/* adjust the camera */
	if (axis == FLURMP_AXIS_X)
	{
		int dif = context->cam_x - self->x;
		printf("\rcam - x: %d        ",
			context->cam_x - self->x);

		/* -221 to -399 */
		if (!(self->flags & FLURMP_LEFT_FLAG))
		{
			if (self->x_v == 0 && dif < -290)
			{
				context->cam_x += 2;
				if (context->cam_x - self->x > -290)
				{
					int d = (-290 - (context->cam_x - self->x));
					printf("d: %d\n", d);
					context->cam_x -= d;
				}
			}
		}
		else if (self->flags & FLURMP_LEFT_FLAG)
		{
			if (self->x_v == 0 && dif > -330)
			{
				context->cam_x -= 2;
				if (context->cam_x - self->x < -330)
				{
					int d = (-330 - (context->cam_x - self->x));
					printf("d: %d\n", d);
					context->cam_x += d;
				}
			}
		}
	}


	/* gravity (y axis) */
	if (axis == FLURMP_AXIS_Y)
	{
		if (self->y_v < 4) self->y_v += 1;

		self->y += self->y_v;

		if (self->y_v < 0 && self->y - context->cam_y <= FLURMP_UPPER_BOUNDARY)
		{
			context->cam_y += self->y_v;
		}

		if (self->y_v > 0 && self->y + self->h - context->cam_y >= FLURMP_LOWER_BOUNDARY)
		{
			context->cam_y += self->y_v;
		}
	}


	/* jumping (either axis) */
	if (context->keystates[FLURMP_SC_SPACE])
	{
		if (!context->inputs[FLURMP_INPUT_SPACE])
		{
			context->inputs[FLURMP_INPUT_SPACE] = 1;

			if (!(self->flags & FLURMP_JUMP_FLAG))
			{
				self->y_v -= 12;
				self->flags |= FLURMP_JUMP_FLAG;
			}
		}
	}
	else if (context->inputs[FLURMP_INPUT_SPACE])
		context->inputs[FLURMP_INPUT_SPACE] = 0;

	/* entity interaction (either axis) */
	if (self->flags & FLURMP_INTERACT_FLAG)
		self->flags &= ~(FLURMP_INTERACT_FLAG);

	if (context->keystates[FLURMP_SC_Z])
	{
		if (!context->inputs[FLURMP_INPUT_Z])
		{
			context->inputs[FLURMP_INPUT_Z] = 1;

			if (!(self->flags & FLURMP_INTERACT_FLAG))
			{
				self->flags |= FLURMP_INTERACT_FLAG;
			}
		}
	}
	else if (context->inputs[FLURMP_INPUT_Z])
		context->inputs[FLURMP_INPUT_Z] = 0;

	/* animation frame (either axis) */
	if (self->flags & FLURMP_JUMP_FLAG)

		self->frame = 1; /* in the air */

	else if (self->x_v != 0)
	{
		/* walking */
		if (self->frame < 30) self->frame++;
		else self->frame = 0;
	}
	else
		self->frame = 0; /* standing */

	/* reset flags (depends on which flag) */
	if (axis == FLURMP_AXIS_Y)
		self->flags |= FLURMP_JUMP_FLAG;
}

static void fl_render_player(fl_context* context, fl_entity* self)
{
	/* determine where to render the player on the x axis */

	int real_x = self->x;

	//if (self->x <= FLURMP_LEFT_BOUNDARY)
	//{
	//	real_x = FLURMP_LEFT_BOUNDARY;
	//}
	//else if (self->x >= FLURMP_RIGHT_BOUNDARY)
	//{
	//	real_x = FLURMP_RIGHT_BOUNDARY;
	//}
	//else
	//	real_x = self->x;

	SDL_Rect dest;

	dest.x = self->x - 10 - context->cam_x;
	dest.y = self->y - 8 - context->cam_y;
	dest.w = self->w + 20;
	dest.h = self->h + 10;

	int f = 0;

	/* calculate the sprite coordinate based on
	 * frame count and entity state
	 */
	if (self->flags & FLURMP_JUMP_FLAG)
		f = 1;
	else if (self->frame > 10 && self->frame < 20)
		f = 1;
	else if (self->frame > 20 && self->frame < 30)
		f = 2;

	SDL_Rect src;
	src.x = 50 * f;
	src.y = 0;
	src.w = 50;
	src.h = 50;

	SDL_Rect hb;
	hb.x = self->x - context->cam_x;
	hb.y = self->y - context->cam_y;
	hb.w = self->w;
	hb.h = self->h;

	if (self->flags & FLURMP_LEFT_FLAG)
		SDL_RenderCopyEx(context->renderer, self->texture, &src, &dest, 0, NULL, SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopyEx(context->renderer, self->texture, &src, &dest, 0, NULL, SDL_FLIP_NONE);

	SDL_SetRenderDrawColor(context->renderer, 255, 0, 255, 255);
	SDL_RenderDrawRect(context->renderer, &hb);
}
