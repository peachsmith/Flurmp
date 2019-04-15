#include "console.h"

#include <stdlib.h>
#include <stdio.h>

#define FL_CON_WIDTH 48
#define FL_CON_HEIGHT 4
#define FL_CHAR_WIDTH 12
#define FL_CHAR_HEIGHT 18
#define FL_FONT_SHEET_WIDTH 576
#define FL_FONT_SHEET_HEIGHT 36
#define FL_BUFFER_LIMIT FL_CON_WIDTH * FL_CON_HEIGHT

/* for now, the buffers and stuff will be static */
static char buffer[FL_CON_WIDTH * FL_CON_HEIGHT] = { '\0' };

static int get_font_index(char c)
{
	switch (c)
	{
	case '-': return 36;
	case '=': return 37;
	case '[': return 38;
	case ']': return 39;
	case ';': return 40;
	case '\'': return 41;
	case ',': return 42;
	case '.': return 43;
	case '/': return 44;
	case '\\': return 45;
	case '`': return 46;
	case ' ': return 47;
	default: return 47;
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
	con->char_count = 0;

	SDL_Surface * surface = SDL_LoadBMP("./images/font.bmp");
	SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 255, 0, 255));
	SDL_Texture * font_texture = SDL_CreateTextureFromSurface(context->renderer, surface);
	SDL_FreeSurface(surface);

	con->font = font_texture;

	return con;
}

void fl_destroy_console(console_t * console)
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


void fl_render_console(fl_context * context, console_t * console)
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
	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.w = FL_CHAR_WIDTH;
	dest.h = FL_CHAR_HEIGHT;

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = FL_CHAR_WIDTH;
	src.h = FL_CHAR_HEIGHT;

	int i;
	int c_x = 0;
	int c_y = 0;
	for (i = 0; i < FL_CON_WIDTH * FL_CON_HEIGHT; i++)
	{
		if (buffer[i] >= 'a' && buffer[i] <= 'z')
		{
			dest.x = console->x + c_x * FL_CHAR_WIDTH;
			dest.y = console->y + c_y * FL_CHAR_HEIGHT;

			src.x = (buffer[i] - 'a') * FL_CHAR_WIDTH;
			src.y = 0;

			SDL_RenderCopy(context->renderer, console->font, &src, &dest);

			if (c_x >= FL_CON_WIDTH - 1)
			{
				c_x = 0;
				if (c_y < FL_CON_HEIGHT - 1)
					c_y++;
			}
			else
				c_x++;

		}
		else if (buffer[i] >= '0' && buffer[i] <= '9')
		{
			dest.x = console->x + c_x * FL_CHAR_WIDTH;
			dest.y = console->y + c_y * FL_CHAR_HEIGHT;

			src.x = (buffer[i] - '0') * FL_CHAR_WIDTH + 26 * FL_CHAR_WIDTH;
			src.y = 0;

			SDL_RenderCopy(context->renderer, console->font, &src, &dest);

			if (c_x >= FL_CON_WIDTH - 1)
			{
				c_x = 0;
				if (c_y < FL_CON_HEIGHT - 1)
					c_y++;
			}
			else
				c_x++;

		}
		else if (buffer[i] > 0x1F)
		{
			dest.x = console->x + c_x * FL_CHAR_WIDTH;
			dest.y = console->y + c_y * FL_CHAR_HEIGHT;

			src.x = get_font_index(buffer[i]) * FL_CHAR_WIDTH;
			src.y = 0;

			SDL_RenderCopy(context->renderer, console->font, &src, &dest);

			if (c_x >= FL_CON_WIDTH - 1)
			{
				c_x = 0;
				if (c_y < FL_CON_HEIGHT - 1)
					c_y++;
			}
			else
				c_x++;
		}
	}
}

void fl_putc(console_t * console, char c)
{
	printf("character count: %d/%d %d\n", console->char_count, FL_BUFFER_LIMIT, (int)c);

	/* buffer position = cursor_x + FL_CON_WIDTH * cursor_y */
	/* TODO: add support for newline, backspace, etc. */
	if (c == '\0')
		return;

	/* handle backspaces */
	if (c == 0x08)
	{
		if (console->char_count > 0)
		{
			buffer[console->char_count-- - 1] = '\0';
		}
		return;
	}

	/* for now, there aren't very many unprintable characters that we can handle */
	if (c < 0x20)
		return;

	if (console->char_count >= FL_BUFFER_LIMIT || c == '\0')
		return;

	buffer[console->char_count++] = c;

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

void fl_print(console_t * console, const char* s)
{

}

char fl_sc_to_char(int sc, int* flag)
{
	switch (sc)
	{
	case FLURMP_SC_A: *flag = FLURMP_INPUT_A; return 'a';
	case FLURMP_SC_B: *flag = FLURMP_INPUT_B; return 'b';
	case FLURMP_SC_C: *flag = FLURMP_INPUT_C; return 'c';
	case FLURMP_SC_D: *flag = FLURMP_INPUT_D; return 'd';
	case FLURMP_SC_E: *flag = FLURMP_INPUT_E; return 'e';
	case FLURMP_SC_F: *flag = FLURMP_INPUT_F; return 'f';
	case FLURMP_SC_G: *flag = FLURMP_INPUT_G; return 'g';
	case FLURMP_SC_H: *flag = FLURMP_INPUT_H; return 'h';
	case FLURMP_SC_I: *flag = FLURMP_INPUT_I; return 'i';
	case FLURMP_SC_J: *flag = FLURMP_INPUT_J; return 'j';
	case FLURMP_SC_K: *flag = FLURMP_INPUT_K; return 'k';
	case FLURMP_SC_L: *flag = FLURMP_INPUT_L; return 'l';
	case FLURMP_SC_M: *flag = FLURMP_INPUT_M; return 'm';
	case FLURMP_SC_N: *flag = FLURMP_INPUT_N; return 'n';
	case FLURMP_SC_O: *flag = FLURMP_INPUT_O; return 'o';
	case FLURMP_SC_P: *flag = FLURMP_INPUT_P; return 'p';
	case FLURMP_SC_Q: *flag = FLURMP_INPUT_Q; return 'q';
	case FLURMP_SC_R: *flag = FLURMP_INPUT_R; return 'r';
	case FLURMP_SC_S: *flag = FLURMP_INPUT_S; return 's';
	case FLURMP_SC_T: *flag = FLURMP_INPUT_T; return 't';
	case FLURMP_SC_U: *flag = FLURMP_INPUT_U; return 'u';
	case FLURMP_SC_V: *flag = FLURMP_INPUT_V; return 'v';
	case FLURMP_SC_W: *flag = FLURMP_INPUT_W; return 'w';
	case FLURMP_SC_X: *flag = FLURMP_INPUT_X; return 'x';
	case FLURMP_SC_Y: *flag = FLURMP_INPUT_Y; return 'y';
	case FLURMP_SC_Z: *flag = FLURMP_INPUT_Z; return 'z';
	case FLURMP_SC_0: *flag = FLURMP_INPUT_0; return '0';
	case FLURMP_SC_1: *flag = FLURMP_INPUT_1; return '1';
	case FLURMP_SC_2: *flag = FLURMP_INPUT_2; return '2';
	case FLURMP_SC_3: *flag = FLURMP_INPUT_3; return '3';
	case FLURMP_SC_4: *flag = FLURMP_INPUT_4; return '4';
	case FLURMP_SC_5: *flag = FLURMP_INPUT_5; return '5';
	case FLURMP_SC_6: *flag = FLURMP_INPUT_6; return '6';
	case FLURMP_SC_7: *flag = FLURMP_INPUT_7; return '7';
	case FLURMP_SC_8: *flag = FLURMP_INPUT_8; return '8';
	case FLURMP_SC_9: *flag = FLURMP_INPUT_9; return '9';
	case FLURMP_SC_SPACE: *flag = FLURMP_INPUT_SPACE; return ' ';
	case FLURMP_SC_COMMA: *flag = FLURMP_INPUT_COMMA; return ',';
	case FLURMP_SC_PERIOD: *flag = FLURMP_INPUT_PERIOD; return '.';
	case FLURMP_SC_LEFTBRACKET: *flag = FLURMP_INPUT_LEFTBRACKET; return '[';
	case FLURMP_SC_RIGHTBRACKET: *flag = FLURMP_INPUT_RIGHTBRACKET; return ']';
	case FLURMP_SC_SEMICOLON: *flag = FLURMP_INPUT_SEMICOLON; return ';';
	case FLURMP_SC_APOSTRAPHE: *flag = FLURMP_INPUT_APOSTRAPHE; return '\'';
	case FLURMP_SC_SLASH: *flag = FLURMP_INPUT_SLASH; return '/';
	case FLURMP_SC_BACKSLASH: *flag = FLURMP_INPUT_BACKSLASH; return '\\';
	case FLURMP_SC_MINUS: *flag = FLURMP_INPUT_MINUS; return '-';
	case FLURMP_SC_EQUALS: *flag = FLURMP_INPUT_EQUALS; return '=';
	case FLURMP_SC_BACKSPACE: *flag = FLURMP_INPUT_BACKSPACE; return (char)0x08;
	case FLURMP_SC_RETURN: *flag = FLURMP_INPUT_RETURN; return (char)0x0A;
	case FLURMP_SC_RETURN2: *flag = FLURMP_INPUT_RETURN; return (char)0x0A;
	default: *flag = FLURMP_INPUT_UNKNOWN; return '\0';
	}
}
