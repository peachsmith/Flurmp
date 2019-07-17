#include "data_panel.h"
#include "text.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define ROW_COUNT 4
#define BUFFER_LIMIT 240

static void update(fl_context*, fl_data_panel*);

static void render(fl_context*, fl_data_panel*);

static int data_panel_printf(fl_data_panel* panel, const char* format, ...);

static void data_panel_write(fl_data_panel* panel, char* str);

static void data_panel_putc(fl_data_panel* panel, char c);

static void clear_buffer(fl_data_panel* panel);

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

static void update(fl_context* context, fl_data_panel* panel)
{
	clear_buffer(panel);

	data_panel_printf(panel, "x: %d\n", context->pco->x);
	data_panel_printf(panel, "y: %d\n", context->pco->y);
	data_panel_printf(panel, "paused: %d", context->paused);
}

static void render(fl_context* context, fl_data_panel* panel)
{
	int i;          /* index variable     */
	int cx;         /* cursor x position  */
	int cy;         /* cursor y position  */
	SDL_Rect frame; /* dialog frame       */
	SDL_Rect src;   /* render source      */
	SDL_Rect dest;  /* render destination */

	frame.x = panel->x;
	frame.y = panel->y;
	frame.w = panel->w;
	frame.h = panel->h;

	/* Render the dialog frame. */
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
		if (panel->buffer[i] >= 0x20 && panel->buffer[i] <= 0x7E)
		{
			/* Get the appropriate glyph from the font atlas. */
			fl_glyph* g = fl_char_to_glyph(panel->atlas, panel->buffer[i]);

			dest.x = panel->x + cx + 10;
			dest.y = panel->y + cy * 22 + 10;
			dest.w = g->image->surface->w;
			dest.h = g->image->surface->h;

			src.w = g->image->surface->w;
			src.h = g->image->surface->h;

			SDL_RenderCopy(context->renderer, g->image->texture, &src, &dest);

			if (cx >= 450)
			{
				/* If the current line exceeds the column count,
				   increment the cursor's y position. */
				cx = 0;
				if (cy < ROW_COUNT - 1)
					cy++;
			}
			else
				cx += g->image->surface->w;
		}
		else if (panel->buffer[i] == 0x0A)
		{
			/* If we encounter a newline, increment the
			   cursor's y position. */
			cx = 0;
			if (cy < ROW_COUNT - 1)
				cy++;
		}
	}
}

static int data_panel_printf(fl_data_panel* panel, const char* format, ...)
{
	int result;
	va_list args;
	char buf[120];

	va_start(args, format);
	result = vsprintf(buf, format, args);
	va_end(args);

	if (result > 0)
		data_panel_write(panel, buf);

	return result;
}

static void data_panel_write(fl_data_panel* panel, char* str)
{
	size_t len = strlen(str);
	size_t i;

	for (i = 0; i < len; i++)
	{
		data_panel_putc(panel, str[i]);
	}
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
