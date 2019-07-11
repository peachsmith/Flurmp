#include "dialog.h"
#include "input.h"
#include "text.h"

#include <string.h>

#define ROW_COUNT 2
#define COLUMN_COUNT 45

static void update(fl_context* context, fl_dialog* self);
static void render(fl_context* context, fl_dialog* self);
static void input_handler(fl_context* context, fl_input_handler* self);
static void clear_buffer(fl_dialog* dialog);

fl_dialog* fl_create_dialog(fl_context* context)
{
	int i;
	fl_dialog* dialog = fl_alloc(fl_dialog, 1);

	if (dialog == NULL)
		return NULL;

	/* Currently, assume the following information:
	   font id: FLURMP_FONT_VERA 
	   x position: 75
	   y position: 320
	   width: 500
	   height: 100 */

	dialog->atlas = context->fonts[FLURMP_FONT_VERA]->impl.font->atlas;
	dialog->x = 75;
	dialog->y = 320;
	dialog->w = 500;
	dialog->h = 100;
	dialog->update = update;
	dialog->render = render;
	dialog->buffer = NULL;
	dialog->input_handler = NULL;
	dialog->msg = NULL;
	dialog->buffer_count = 0;
	dialog->counter = 0;
	dialog->open = 0;
	dialog->counter = 0;
	dialog->len = 0;
	dialog->speed = 0;
	dialog->callback = NULL;
	dialog->hold = 0;

	/* Allocate memory for the buffer. */
	char* buffer = fl_alloc(char, (ROW_COUNT * COLUMN_COUNT));

	if (buffer == NULL)
	{
		fl_destroy_dialog(dialog);
		return NULL;
	}

	for (i = 0; i < ROW_COUNT * COLUMN_COUNT; i++)
		buffer[i] = '\0';

	dialog->buffer = buffer;

	dialog->input_handler = fl_create_input_handler(input_handler);

	return dialog;
}

void fl_destroy_dialog(fl_dialog* dialog)
{
	if (dialog == NULL)
		return;

	if (dialog->buffer != NULL)
		fl_free(dialog->buffer);

	if (dialog->input_handler != NULL)
		fl_destroy_input_handler(dialog->input_handler);

	fl_free(dialog);
}

void fl_dialog_putc(fl_context* context, char c)
{
	if (context == NULL || context->active_dialog == NULL)
		return;

	if (context->active_dialog->buffer_count >= ROW_COUNT * COLUMN_COUNT)
		return;

	fl_dialog* d = context->active_dialog;

	d->buffer[d->buffer_count++] = c;
}

void fl_dialog_write(fl_context* context, const char* msg, int speed)
{
	size_t len = strlen(msg);

	context->active_dialog->msg = msg;
	context->active_dialog->len = len;
}

static void update(fl_context* context, fl_dialog* self)
{
	if (self->counter < self->len)
	{
		if (self->msg != NULL)
		{
			fl_dialog_putc(context, self->msg[self->counter]);
		}

		self->counter++;
	}
}

static void render(fl_context* context, fl_dialog* self)
{
	int i;          /* index variable     */
	int cx;         /* cursor x position  */
	int cy;         /* cursor y position  */
	SDL_Rect frame; /* dialog frame       */
	SDL_Rect src;   /* render source      */
	SDL_Rect dest;  /* render destination */

	frame.x = self->x;
	frame.y = self->y;
	frame.w = self->w;
	frame.h = self->h;

	/* Render the dialog frame. */
	SDL_SetRenderDrawColor(context->renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(context->renderer, &frame);
	SDL_SetRenderDrawColor(context->renderer, 250, 250, 250, 255);
	SDL_RenderDrawRect(context->renderer, &frame);

	dest.x = 0;
	dest.y = 0;
	src.x = 0;
	src.y = 0;

	for (i = cx = cy = 0; i < ROW_COUNT * COLUMN_COUNT; i++)
	{
		if (self->buffer[i] >= 0x20 && self->buffer[i] <= 0x7E)
		{
			/* Get the appropriate glyph from the font atlas. */
			fl_glyph* g = fl_char_to_glyph(self->atlas, self->buffer[i]);

			dest.x = self->x + cx * g->image->surface->w + 10;
			dest.y = self->y + cy * g->image->surface->h + 10;
			dest.w = g->image->surface->w;
			dest.h = g->image->surface->h;

			src.w = g->image->surface->w;
			src.h = g->image->surface->h;

			SDL_RenderCopy(context->renderer, g->image->texture, &src, &dest);

			if (cx >= COLUMN_COUNT - 1)
			{
				/* If the current line exceeds the column count,
				   increment the cursor's y position. */
				cx = 0;
				if (cy < ROW_COUNT - 1)
					cy++;
			}
			else
				cx++;
		}
		else if (self->buffer[i] == 0x0A)
		{
			/* If we encounter a newline, increment the
			   cursor's y position. */
			cx = 0;
			if (cy < ROW_COUNT - 1)
				cy++;
		}
	}
}

static void input_handler(fl_context* context, fl_input_handler* self)
{
	if (self->child != NULL)
	{
		self->child->handler(context, self->child);
		return;
	}

	if (context->active_dialog->counter < context->active_dialog->len)
		return;

	if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_J))
	{
		void(*callback) (fl_context*, fl_dialog*);


		callback = context->active_dialog->callback;

		fl_dialog* dialog = context->active_dialog;

		context->active_dialog->open = 0;
		context->active_dialog = NULL;

		/* Relenquish input control */
		fl_pop_input_handler(context);

		fl_destroy_dialog(dialog);

		if (callback != NULL)
			callback(context, dialog);
	}
}

static void clear_buffer(fl_dialog* dialog)
{
	if (dialog == NULL)
		return;

	int i;
	for (i = 0; i < ROW_COUNT * COLUMN_COUNT; i++)
		dialog->buffer[i] = '\0';

	dialog->buffer_count = 0;
}
