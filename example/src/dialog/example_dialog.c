#include "dialog.h"
#include "text.h"
#include "input.h"

#include <stdio.h>

static void update(fl_context* context, fl_dialog* self);
static void render(fl_context* context, fl_dialog* self);
static void input_handler(fl_context* context, fl_dialog* self);
static void clear_buffer(fl_dialog* dialog);

static const char* statements[3] = {
	"This is the first statement.",  /* 28 */
	"This is the second statement.", /* 29 */
	"This is the\nthird statement."  /* 28 */
};

static int row_count = 2;
static int column_count = 40;

fl_dialog* fl_create_example_dialog(fl_resource* font, int x, int y, int w, int h)
{
	int i;
	fl_dialog* dialog = fl_create_dialog(font, x, y, w, h);

	if (dialog == NULL)
		return NULL;

	dialog->statements = statements;
	dialog->update = update;
	dialog->render = render;
	dialog->statement_count = 3;

	char* buffer = (char*)malloc(sizeof(char) * row_count * column_count);

	if (buffer == NULL)
	{
		fl_destroy_dialog(dialog);
		return NULL;
	}

	for (i = 0; i < row_count * column_count; i++)
		buffer[i] = '\0';

	dialog->buffer = buffer;
	dialog->input_handler = input_handler;

	return dialog;
}

void update(fl_context* context, fl_dialog* self)
{
	if (self->counter < 30)
	{
		if (self->counter < 28)
			self->buffer[self->buffer_count++] = self->statements[self->current_statement][self->counter];

		self->counter++;
	}
}

static void render(fl_context* context, fl_dialog* self)
{
	SDL_SetRenderDrawColor(context->renderer, 0, 0, 0, 255);

	SDL_Rect r;
	r.x = self->x;
	r.y = self->y;
	r.w = self->w;
	r.h = self->h;

	SDL_RenderFillRect(context->renderer, &r);

	SDL_SetRenderDrawColor(context->renderer, 250, 250, 250, 255);
	SDL_RenderDrawRect(context->renderer, &r);

	int w = context->fonts[FL_FONT_VERA]->impl.font->atlas->glyphs[0]->image->surface->w;
	int h = context->fonts[FL_FONT_VERA]->impl.font->atlas->glyphs[0]->image->surface->h;

	/* render buffer contents */
	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.w = w;
	dest.h = h;

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = w;
	src.h = h;

	int i;
	int c_x = 0;
	int c_y = 0;
	for (i = 0; i < row_count * column_count; i++)
	{
		if (self->buffer[i] >= 0x20)
		{
			dest.x = self->x + c_x * w + 10;
			dest.y = self->y + c_y * h + 10;

			fl_glyph* g = fl_char_to_glyph(context->fonts[FL_FONT_VERA]->impl.font->atlas, self->buffer[i]);
			SDL_RenderCopy(context->renderer, g->image->texture, &src, &dest);

			if (c_x >= column_count - 1)
			{
				c_x = 0;
				if (c_y < row_count - 1)
					c_y++;
			}
			else
				c_x++;
		}
		else if (self->buffer[i] == 0x0A)
		{
			c_x = 0;
			if (c_y < row_count - 1)
				c_y++;
		}
	}
}

static void input_handler(fl_context* context, fl_dialog* self)
{
	if (self->counter < 30)
		return;

	if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, FLURMP_SC_J))
	{
		if (self->current_statement < 3)
			self->current_statement++;

		self->counter = 0;
		clear_buffer(self);

		if (self->current_statement == 3)
		{
			self->open = 0;
			self->current_statement = 0;
			context->active_dialog = NULL;
		}
	}
}

static void clear_buffer(fl_dialog* dialog)
{
	if (dialog == NULL)
		return;

	int i;
	for (i = 0; i < row_count * column_count; i++)
		dialog->buffer[i] = '\0';

	dialog->buffer_count = 0;
}
