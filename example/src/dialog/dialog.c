#include "dialog.h"
#include "input.h"
#include "text.h"

#define ROW_COUNT 2
#define BUFFER_LIMIT 120
#define LINE_WIDTH 450



/* -------------------------------------------------------------- */
/*                   internal dialog functions                    */
/* -------------------------------------------------------------- */

/**
 * Appends the next set of characters to the dialog buffer.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_dialog - a dialog
 */
static void update(fl_context* context, fl_dialog* self);

/**
 * Renders a dialog to the screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_dialog - a dialog
 */
static void render(fl_context* context, fl_dialog* self);

/**
 * The input handler function for dialogs.
 * A dialog should consume two inputs: the J key, and the K key.
 * By pressing either the J or K key, the current dialog should
 * be destroyed, if applicable, and the callback should be invoked.
 * Pressing the J or K key should destroy the dialog, if applicable,
 * and invoke the callback function.
 * If the K key is pressed before the message has been completely displayed,
 * then the remaining characters of the message should be written to
 * the dialog buffer.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_input_handler - an input handler
 */
static void handle_input(fl_context* context, fl_input_handler* self);

/**
 * Appends a single character to the current active dialog buffer.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   char - a character
 */
static void dialog_putc(fl_dialog* dialog, char c);

/**
 * Clears the contents of the current dialog buffer by setting all
 * characters to '\0' and setting the buffer count to 0.
 *
 * Params:
 *   fl_dialog - a dialog
 */
static void clear_buffer(fl_dialog* dialog);



/* -------------------------------------------------------------- */
/*             internal dialog functions (implementation)         */
/* -------------------------------------------------------------- */

static void update(fl_context* context, fl_dialog* self)
{
	if (self->counter < self->len)
	{
		if (self->msg != NULL)
			dialog_putc(self, self->msg[self->counter]);

		self->counter++;
	}
}

static void render(fl_context* context, fl_dialog* self)
{
	int i;         /* index variable     */
	int cx;        /* cursor x position  */
	int cy;        /* cursor y position  */
	fl_rect frame; /* dialog frame       */
	fl_rect src;   /* render source      */
	fl_rect dest;  /* render destination */

	fl_set_rect(&frame, self->x, self->y, self->w, self->h);

	/* Render the dialog frame. */
	fl_set_draw_color(context, 0, 0, 0, 255);
	fl_draw_solid_rect(context, &frame);
	fl_set_draw_color(context, 250, 250, 250, 255);
	fl_draw_rect(context, &frame);

	dest.x = 0;
	dest.y = 0;
	src.x = 0;
	src.y = 0;

	for (i = cx = cy = 0; i < BUFFER_LIMIT; i++)
	{
		if (self->buffer[i] >= 0x20 && self->buffer[i] <= 0x7E)
		{
			/* Get the appropriate glyph from the font atlas. */
			fl_glyph* g = fl_char_to_glyph(self->atlas, self->buffer[i]);

			dest.x = self->x + cx + 10;
			dest.y = self->y + cy * 22 + 10;
			dest.w = g->image->w;
			dest.h = g->image->h;

			src.w = g->image->w;
			src.h = g->image->h;

			fl_draw(context, g->image->texture, &src, &dest, 0);

			if (cx >= LINE_WIDTH)
			{
				/* If the current line exceeds the
				   line width pixel limit,
				   increment the cursor's y position. */
				cx = 0;
				if (cy < ROW_COUNT - 1)
					cy++;
			}
			else
				cx += g->image->w;
		}
		else if (self->buffer[i] == 0x0A)
		{
			/* If we encounter a newline,
			   increment the cursor's y position. */
			cx = 0;
			if (cy < ROW_COUNT - 1)
				cy++;
		}
	}
}

static void handle_input(fl_context* context, fl_input_handler* self)
{
	if (context == NULL || context->active_dialog == NULL)
		return;

	if (self->child != NULL)
	{
		self->child->handle_input(context, self->child);
		return;
	}

	fl_dialog* dialog = context->active_dialog;
	void(*callback) (fl_context*) = dialog->callback;

	/* Handle the J key */
	if (fl_consume_key(context, FLURMP_SC_J))
	{
		/* Do not proceed unless the current dialog message
		   has been completely displayed. */
		if (dialog->counter < dialog->len)
			return;

		/* If the dialog is not marked as "hold", destroy
		   it here, otherwise its destruction should be handled
		   in the callback function. */
		if (!dialog->hold)
		{
			/* Clear the active dialog pointer. */
			context->active_dialog = NULL;

			/* Relenquish input control. */
			fl_pop_input_handler(context);

			/* Destroy the dialog. */
			fl_destroy_dialog(dialog);
		}

		/* If a callback function is present, invoke it here. */
		if (callback != NULL)
			callback(context);

		return;
	}

	/* Handle the K key */
	if (fl_consume_key(context, FLURMP_SC_K))
	{
		/* If the message has not yet been completely displayed,
		   write the remaining characters in the message to the
		   dialog buffer. */
		if (dialog->counter < dialog->len && dialog->msg != NULL)
		{
			size_t i;
			for (i = dialog->counter; i < dialog->len; i++)
				dialog_putc(dialog, dialog->msg[i]);

			dialog->counter = dialog->len;

			return;
		}

		/* If the dialog is not marked as "hold", destroy
		   it here, otherwise its destruction should be handled
		   in the callback function. */
		if (!dialog->hold)
		{
			/* Clear the active dialog pointer. */
			context->active_dialog = NULL;

			/* Relenquish input control. */
			fl_pop_input_handler(context);

			/* Destroy the dialog. */
			fl_destroy_dialog(dialog);
		}

		/* If a callback function is present, invoke it here. */
		if (callback != NULL)
			callback(context);

		return;
	}
}

static void dialog_putc(fl_dialog* dialog, char c)
{
	if (dialog == NULL || dialog->buffer_count >= BUFFER_LIMIT)
		return;

	/* Add the character to the buffer and increment the buffer count. */
	dialog->buffer[dialog->buffer_count++] = c;
}

static void clear_buffer(fl_dialog* dialog)
{
	if (dialog == NULL)
		return;

	/* Set all characters in the buffer to '\0'. */
	fl_zero(dialog->buffer, BUFFER_LIMIT);

	/* Reset the buffer count. */
	dialog->buffer_count = 0;
}



/* -------------------------------------------------------------- */
/*                     dialog.h implementation                    */
/* -------------------------------------------------------------- */

fl_dialog* fl_create_dialog(fl_context* context)
{
	fl_dialog* dialog = fl_alloc(fl_dialog, 1);

	if (dialog == NULL)
		return NULL;

	/*
	   Currently, assume the following information:
		 font id:    FLURMP_FONT_VERA
		 x position: 75
		 y position: 320
		 width:      500
		 height:     100
	*/

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
	dialog->counter = 0;
	dialog->len = 0;
	dialog->speed = 0;
	dialog->callback = NULL;
	dialog->hold = 0;

	/* Allocate memory for the buffer. */
	char* buffer = fl_alloc(char, BUFFER_LIMIT);

	/* Verify buffer allocation. */
	if (buffer == NULL)
	{
		fl_destroy_dialog(dialog);
		return NULL;
	}

	/* Set the characters in the buffer to '\0'. */
	fl_zero(buffer, BUFFER_LIMIT);

	dialog->buffer = buffer;
	dialog->input_handler = fl_create_input_handler(handle_input);

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

void fl_dialog_write(fl_context* context, const char* msg, int speed, void(*callback) (fl_context*), int hold)
{
	if (context->active_dialog != NULL || msg == NULL)
		return;

	/* Determine the length of the message. */
	size_t len = strlen(msg);

	/* Create a new dialog. */
	fl_dialog* dialog = fl_create_dialog(context);

	dialog->len = len;
	dialog->counter = 0;

	dialog->msg = msg;
	dialog->speed = speed; /* TODO: implement speed */
	dialog->callback = callback;
	dialog->hold = hold;

	/* Set the new dialog as the active dialog. */
	context->active_dialog = dialog;
	fl_push_input_handler(context, dialog->input_handler);
}
