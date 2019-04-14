#include "console.h"

#define FL_CON_WIDTH 48
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
	fl_putc(con, 'd');
	fl_putc(con, 'e');
	fl_putc(con, 'f');
	fl_putc(con, 'g');
	fl_putc(con, 'h');
	fl_putc(con, 'i');
	fl_putc(con, 'j');
	fl_putc(con, 'k');
	fl_putc(con, 'l');
	fl_putc(con, 'm');
	fl_putc(con, 'n');
	fl_putc(con, 'o');
	fl_putc(con, 'p');
	fl_putc(con, 'q');
	fl_putc(con, 'r');
	fl_putc(con, 's');
	fl_putc(con, 't');
	fl_putc(con, 'u');
	fl_putc(con, 'v');
	fl_putc(con, 'w');
	fl_putc(con, 'x');
	fl_putc(con, 'y');
	fl_putc(con, 'z');

	fl_putc(con, 'a');
	fl_putc(con, 'b');
	fl_putc(con, 'c');
	fl_putc(con, 'd');
	fl_putc(con, 'e');
	fl_putc(con, 'f');
	fl_putc(con, 'g');
	fl_putc(con, 'h');
	fl_putc(con, 'i');
	fl_putc(con, 'j');
	fl_putc(con, 'k');
	fl_putc(con, 'l');
	fl_putc(con, 'm');
	fl_putc(con, 'n');
	fl_putc(con, 'o');
	fl_putc(con, 'p');
	fl_putc(con, 'q');
	fl_putc(con, 'r');
	fl_putc(con, 's');
	fl_putc(con, 't');
	fl_putc(con, 'u');
	fl_putc(con, 'v');
	fl_putc(con, 'w');
	fl_putc(con, 'x');
	fl_putc(con, 'y');
	fl_putc(con, 'z');

	fl_putc(con, 'a');
	fl_putc(con, 'b');
	fl_putc(con, 'c');
	fl_putc(con, 'd');
	fl_putc(con, 'e');
	fl_putc(con, 'f');
	fl_putc(con, 'g');
	fl_putc(con, 'h');
	fl_putc(con, 'i');
	fl_putc(con, 'j');
	fl_putc(con, 'k');
	fl_putc(con, 'l');
	fl_putc(con, 'm');
	fl_putc(con, 'n');
	fl_putc(con, 'o');
	fl_putc(con, 'p');
	fl_putc(con, 'q');
	fl_putc(con, 'r');
	fl_putc(con, 's');
	fl_putc(con, 't');
	fl_putc(con, 'u');
	fl_putc(con, 'v');
	fl_putc(con, 'w');
	fl_putc(con, 'x');
	fl_putc(con, 'y');
	fl_putc(con, 'z');

	//TODO: fix rendering more than 24 characters

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
	int c_x = 0;
	int c_y = 0;
	for (i = 0; i < FL_CON_WIDTH * FL_CON_HEIGHT; i++)
	{
		if (buffer[i] >= 'a' && buffer[i] <= 'z')
		{
			SDL_Rect dest;
			dest.x = console->x + c_x * FL_CHAR_WIDTH;
			dest.y = console->y + c_y * FL_CHAR_HEIGHT;
			dest.w = FL_CHAR_WIDTH;
			dest.h = FL_CHAR_HEIGHT;

			SDL_Rect src;
			src.x = (buffer[i] - 'a') * FL_CHAR_WIDTH;
			src.y = 0;
			src.w = FL_CHAR_WIDTH;
			src.h = FL_CHAR_HEIGHT;

			SDL_RenderCopy(context->renderer, console->font, &src, &dest);

			if (c_x >= FL_CON_WIDTH - 1)
			{
				c_x = 0;
				if (c_y < FL_CON_HEIGHT - 1)
					c_y++;
			}
			else
			{
				c_x++;
			}
		}
	}
}

void fl_print(console_t* console, const char* s)
{

}
