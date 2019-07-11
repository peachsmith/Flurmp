#include "sign.h"
#include "entity.h"
#include "resource.h"
#include "dialog.h"
#include "input.h"
#include "text.h"

#include <stdio.h>
#include <string.h>

static const char* statements[2] = {
	"This is a sign.",
	"It's purpose is to display information."
};

/* entity function prototypes */
static void collide(fl_context*, fl_entity*, fl_entity*, int, int);
static void update(fl_context*, fl_entity*, int);
static void render(fl_context*, fl_entity*);

/* dialog callbacks */
static void first_cb(fl_context* context, fl_dialog* self);

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

static void collide(fl_context* context, fl_entity* self, fl_entity* other, int collided, int axis)
{
	if (other->flags & FLURMP_INTERACT_FLAG)
	{
		other->flags &= ~(FLURMP_INTERACT_FLAG);



		fl_dialog* dialog = fl_create_dialog(context);

		dialog->counter = 0;
		dialog->msg = statements[0];
		dialog->len = strlen(statements[0]);
		dialog->callback = first_cb;

		context->active_dialog = dialog;
		fl_push_input_handler(context, dialog->input_handler);
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

static void first_cb(fl_context* context, fl_dialog* self)
{
	fl_dialog* dialog = fl_create_dialog(context);

	dialog->counter = 0;
	dialog->msg = statements[1];
	dialog->len = strlen(statements[1]);

	context->active_dialog = dialog;
	fl_push_input_handler(context, dialog->input_handler);
}
