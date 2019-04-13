#include "console.h"

#define FL_CON_WIDTH 24
#define FL_CON_HEIGHT 4
#define FL_CHAR_WIDTH 12
#define FL_CHAR_HEIGHT 18
#define FL_FONT_SHEET_WIDTH 576
#define FL_FONT_SHEET_HEIGHT 36
#define FL_BUFFER_LIMIT FL_CON_WIDTH * FL_CON_HEIGHT

/* for now, the buffers and stuff will be static */
static int char_count = 0;
static char buffer[FL_CON_WIDTH * FL_CON_HEIGHT] = { '\0' };

static void fl_putc(console_t* console, char c)
{
	/* buffer position = cursor_x + FL_CON_WIDTH * cursor_y */
	/* TODO: add support for newline, backspace, etc. */
	if (char_count >= FL_BUFFER_LIMIT)
		return;

	buffer[char_count++] = c;

	/* TODO: add support for newline, backspace, etc. */
	if (console->cursor_x >= FL_CON_WIDTH - 1)
	{
		console->cursor_x = 0;
		if (console->cursor_y < FL_CON_HEIGHT - 1)
			console->cursor_y++;
	}
	else
	{
		console->cursor_x++;
	}
}

console_t* fl_create_console(fl_context* context)
{
	console_t* con = (console_t*)malloc(sizeof(console_t));

	if (con == NULL)
		return NULL;

	con->x = 10;
	con->y = 350;
	con->w = 620;
	con->h = 120;
	con->cursor_x = 0;
	con->cursor_y = 0;

	SDL_Surface *surface = SDL_LoadBMP("./images/font.bmp");
	SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 255, 0, 255));
	SDL_Texture *font_texture = SDL_CreateTextureFromSurface(context->renderer, surface);
	SDL_FreeSurface(surface);

	con->font = font_texture;

	/* hard code a bunch of stuff */
	fl_putc(con, 'a');
	fl_putc(con, 'b');
	fl_putc(con, 'c');

	return con;
}

void fl_destroy_console(console_t* console)
{
	if (console == NULL)
		return;

	free(console);

	return;
}

/*
	font sheet currently 576 x 36
	each character is 12 x 18

	TODO: start with a console display buffer of 60 characters in a 20 * 3 grid.
	TODO: the actual buffer should probably be 1024 bytes
	TODO: for now, just put a bunch of static fields in this file
*/


void fl_render_console(fl_context* context, console_t* console)
{
	SDL_SetRenderDrawColor(context->renderer, 150, 50, 150, 120);

	SDL_Rect r;
	r.x = console->x;
	r.y = console->y;
	r.w = console->w;
	r.h = console->h;

	SDL_RenderFillRect(context->renderer, &r);

	SDL_SetRenderDrawColor(context->renderer, 240, 240, 240, 255);
	SDL_RenderDrawRect(context->renderer, &r);

	/* render buffer contents */
	int i;
	for (i = 0; i < FL_CON_WIDTH * FL_CON_HEIGHT; i++)
	{
		if (buffer[i] >= 'a' && buffer[i] <= 'z')
		{
			SDL_Rect dest;
			//dest.x = console->cursor_x * FL_CHAR_WIDTH + console->x;
			//dest.y = console->cursor_y * FL_CHAR_HEIGHT + console->y;
			dest.x = (i - FL_CON_WIDTH * console->cursor_y) * FL_CHAR_WIDTH + console->x;
			dest.y = ((i - console->cursor_x) / FL_CON_WIDTH) * FL_CHAR_HEIGHT + console->y;
			dest.w = FL_CHAR_WIDTH;
			dest.h = FL_CHAR_HEIGHT;

			/* (a, z) =  (97, 122) */

			SDL_Rect src;
			src.x = (buffer[i] - 'a') * FL_CHAR_WIDTH;
			src.y = 0;
			src.w = FL_CHAR_WIDTH;
			src.h = FL_CHAR_HEIGHT;

			SDL_RenderCopy(context->renderer, console->font, &src, &dest);

			///* TODO: add support for newline, backspace, etc. */
			//if (console->cursor_x >= FL_CON_WIDTH - 1)
			//{
			//	console->cursor_x = 0;
			//	console->cursor_y++;
			//}
			//else
			//{
			//	console->cursor_x++;
			//}
		}
	}
}

void fl_print(console_t* console, const char* s)
{

}
