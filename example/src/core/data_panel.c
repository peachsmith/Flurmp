#include "data_panel.h"
#include "text.h"

#define ROW_COUNT 8
#define BUFFER_LIMIT 240
#define LINE_WIDTH 450



/* -------------------------------------------------------------- */
/*                  internal data_panel functions                 */
/* -------------------------------------------------------------- */

/**
 * Writes the current data to the data panel's buffer.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_data_panel - a data panel
 */
static void update(fl_context*, fl_data_panel*);

/**
 * Renders a data panel to the screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_data_panel - a data panel
 */
static void render(fl_context*, fl_data_panel*);

/**
 * Writes a string of text to a data panel's buffer.
 * This is like calling printf("some string", ...)
 * but instead of writing to stdout, it writes to an array.
 *
 * Params:
 *   fl_data_panel - a data panel
 *   const char* format - the string to write
 *   ... - the variadic arguments to be formatted as character data
 *
 * Return:
 *   int - an integer indicating success
 */
static int data_panel_printf(fl_data_panel* panel, const char* format, ...);

/**
 * Writes a single character to a data panel's buffer.
 *
 * Params:
 *   fl_data_panel - a data panel
 *   char - a character
 */
static void data_panel_putc(fl_data_panel* panel, char c);

/**
 * Clears the contents of the current data panel buffer by setting all
 * characters to '\0' and setting the buffer count to 0.
 *
 * Params:
 *   fl_data_panel - a data panel
 */
static void clear_buffer(fl_data_panel* panel);



/* -------------------------------------------------------------- */
/*          internal data_panel functions (implementation)        */
/* -------------------------------------------------------------- */

static void update(fl_context* context, fl_data_panel* panel)
{
	clear_buffer(panel);

	data_panel_printf(panel, "x: %d\n", context->pco->x);
	data_panel_printf(panel, "y: %d\n", context->pco->y);
	data_panel_printf(panel, "cam x: %d\n", context->cam_x);
	data_panel_printf(panel, "cam y: %d\n", context->cam_y);
	data_panel_printf(panel, "diff x: %d\n", context->cam_x - context->pco->x);
	data_panel_printf(panel, "diff y: %d\n", context->cam_y - context->pco->y);
}

static void render(fl_context* context, fl_data_panel* self)
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

	/* Render the data panel frame. */
	SDL_SetRenderDrawColor(context->renderer, 20, 150, 20, 80);
	SDL_RenderFillRect(context->renderer, &frame);
	SDL_SetRenderDrawColor(context->renderer, 250, 250, 250, 255);
	SDL_RenderDrawRect(context->renderer, &frame);

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
			dest.w = g->image->surface->w;
			dest.h = g->image->surface->h;

			src.w = g->image->surface->w;
			src.h = g->image->surface->h;

			SDL_RenderCopy(context->renderer, g->image->texture, &src, &dest);

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
				cx += g->image->surface->w;
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

static int data_panel_printf(fl_data_panel* panel, const char* format, ...)
{
	int result;
	size_t i;
	size_t len;
	va_list args;
	char buf[120];

	for (i = 0; i < 120; i++)
		buf[i] = '\0';

	va_start(args, format);
	result = vsprintf(buf, format, args);
	va_end(args);

	if (result > 0)
	{
		len = strlen(buf);

		for (i = 0; i < len; i++)
			data_panel_putc(panel, buf[i]);
	}

	return result;
}

static void data_panel_putc(fl_data_panel* panel, char c)
{
	if (panel == NULL || panel->buffer_count >= BUFFER_LIMIT)
		return;

	/* Add the character to the buffer and increment the buffer count. */
	panel->buffer[panel->buffer_count++] = c;
}

static void clear_buffer(fl_data_panel* panel)
{
	if (panel == NULL)
		return;

	/* Set all characters to '\0'. */
	int i;
	for (i = 0; i < BUFFER_LIMIT; i++)
		panel->buffer[i] = '\0';

	/* Reset the buffer count. */
	panel->buffer_count = 0;
}



/* -------------------------------------------------------------- */
/*                   data_panel.h implementation                  */
/* -------------------------------------------------------------- */

fl_data_panel* fl_create_data_panel(int x, int y, int w, int h, fl_font_atlas* atlas)
{
	fl_data_panel* panel = fl_alloc(fl_data_panel, 1);

	if (panel == NULL)
		return NULL;

	panel->x = x;
	panel->y = y;
	panel->w = w;
	panel->h = h;
	panel->atlas = atlas;
	panel->update = update;
	panel->render = render;
	panel->buffer_count = 0;

	panel->buffer = fl_alloc(char, BUFFER_LIMIT);

	if (panel->buffer == NULL)
	{
		free(panel);
		return NULL;
	}

	return panel;
}

void fl_destroy_data_panel(fl_data_panel* panel)
{
	if (panel == NULL)
		return;

	if (panel->buffer != NULL)
		free(panel->buffer);

	free(panel);
}
