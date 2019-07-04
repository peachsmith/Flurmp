#include "console.h"
#include "input.h"
#include "text.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FL_CON_WIDTH 48
#define FL_CON_HEIGHT 4
#define FL_CHAR_WIDTH 12
#define FL_CHAR_HEIGHT 18
#define FL_FONT_SHEET_WIDTH 576
#define FL_FONT_SHEET_HEIGHT 36
#define FL_BUFFER_LIMIT FL_CON_WIDTH * FL_CON_HEIGHT

/* for now, the buffers and stuff will be static */
static char buffer[FL_BUFFER_LIMIT] = { '\0' };

static void clear_buffer(fl_console* console);

static int get_font_index(char c, int* x, int* y)
{
	if (c >= 'a' && c <= 'z')
	{
		*x = (c - 'a') * FL_CHAR_WIDTH;
		*y = FL_CHAR_HEIGHT * 0;
		return 1;
	}

	else if (c >= 'A' && c <= 'Z')
	{
		*x = (c - 'A') * FL_CHAR_WIDTH;
		*y = FL_CHAR_HEIGHT * 1;
		return 1;
	}

	else if (c >= '0' && c <= '9')
	{
		*x = (c - '0') * FL_CHAR_WIDTH + 26 * FL_CHAR_WIDTH;
		*y = FL_CHAR_HEIGHT * 0;
		return 1;
	}

	switch (c)
	{
	case ')':  *x = 26 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case '!':  *x = 27 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case '@':  *x = 28 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case '#':  *x = 29 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case '$':  *x = 30 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case '%':  *x = 31 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case '^':  *x = 32 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case '&':  *x = 33 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case '*':  *x = 34 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case '(':  *x = 35 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;

	case '-':  *x = 36 * FL_CHAR_WIDTH; *y = 0 * FL_CHAR_HEIGHT; break;
	case '=':  *x = 37 * FL_CHAR_WIDTH; *y = 0 * FL_CHAR_HEIGHT; break;
	case '[':  *x = 38 * FL_CHAR_WIDTH; *y = 0 * FL_CHAR_HEIGHT; break;
	case ']':  *x = 39 * FL_CHAR_WIDTH; *y = 0 * FL_CHAR_HEIGHT; break;
	case ';':  *x = 40 * FL_CHAR_WIDTH; *y = 0 * FL_CHAR_HEIGHT; break;
	case '\'': *x = 41 * FL_CHAR_WIDTH; *y = 0 * FL_CHAR_HEIGHT; break;
	case ',':  *x = 42 * FL_CHAR_WIDTH; *y = 0 * FL_CHAR_HEIGHT; break;
	case '.':  *x = 43 * FL_CHAR_WIDTH; *y = 0 * FL_CHAR_HEIGHT; break;
	case '/':  *x = 44 * FL_CHAR_WIDTH; *y = 0 * FL_CHAR_HEIGHT; break;
	case '\\': *x = 45 * FL_CHAR_WIDTH; *y = 0 * FL_CHAR_HEIGHT; break;
	case '`':  *x = 46 * FL_CHAR_WIDTH; *y = 0 * FL_CHAR_HEIGHT; break;

	case '_':  *x = 36 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case '+':  *x = 37 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case '{':  *x = 38 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case '}':  *x = 39 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case ':':  *x = 40 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case '"':  *x = 41 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case '<':  *x = 42 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case '>':  *x = 43 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case '?':  *x = 44 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case '|':  *x = 45 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;
	case '~':  *x = 46 * FL_CHAR_WIDTH; *y = 1 * FL_CHAR_HEIGHT; break;

	case ' ':
	default:   *x = 47 * FL_CHAR_WIDTH; *y = 0 * FL_CHAR_HEIGHT; break;
	}

	return 1;
}

fl_console* fl_create_console(fl_context* context)
{
	fl_console* con = (fl_console*)malloc(sizeof(fl_console));

	if (con == NULL)
		return NULL;

	con->x = 10;
	con->y = 350;
	con->w = 620;
	con->h = 120;
	con->cursor_x = 0;
	con->cursor_y = 0;
	con->char_count = 0;

	return con;
}

void fl_destroy_console(fl_console* console)
{
	if (console == NULL)
		return;

	free(console);

	return;
}

/*
	font sheet currently 576 x 36
	each character is 12 x 18
*/

void fl_render_console(fl_context* context, fl_console* console)
{
	SDL_SetRenderDrawColor(context->renderer, 150, 50, 150, 120);

	SDL_Rect r;
	r.x = console->x;
	r.y = console->y;
	r.w = console->w;
	r.h = console->h;

	SDL_RenderFillRect(context->renderer, &r);

	SDL_SetRenderDrawColor(context->renderer, 250, 250, 250, 255);
	SDL_RenderDrawRect(context->renderer, &r);

	int w = context->fonts[FL_FONT_COUSINE]->atlas->glyphs[0]->surface->w;
	int h = context->fonts[FL_FONT_COUSINE]->atlas->glyphs[0]->surface->h;

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
	for (i = 0; i < FL_CON_WIDTH * FL_CON_HEIGHT; i++)
	{
		if (buffer[i] >= 0x20)
		{
			dest.x = console->x + c_x * w + 2;
			dest.y = console->y + c_y * h + 2;

			fl_glyph* g = fl_char_to_glyph(context->fonts[FL_FONT_COUSINE]->atlas, buffer[i]);
			SDL_RenderCopy(context->renderer, g->texture, &src, &dest);

			if (c_x >= FL_CON_WIDTH - 1)
			{
				c_x = 0;
				if (c_y < FL_CON_HEIGHT - 1)
					c_y++;
			}
			else
				c_x++;
		}
		else if (buffer[i] == 0x0A)
		{
			c_x = 0;
			if (c_y < FL_CON_HEIGHT - 1)
				c_y++;
		}
	}
}

void fl_putc(fl_console* console, char c, unsigned char mod)
{
	/* buffer position = cursor_x + FL_CON_WIDTH * cursor_y */
	if (c == '\0')
		return;

	/* handle backspaces */
	if (c == 0x08)
	{
		if (console->char_count > 0)
			buffer[console->char_count-- - 1] = '\0';

		return;
	}

	/* handle newlines */
	if (c == 0x0A)
	{
		if (console->char_count < FL_BUFFER_LIMIT - 1)
		{
			console->cursor_x = 0;
			if (console->cursor_y < FL_CON_HEIGHT - 1)
			{
				buffer[console->char_count++] = '\n';
				console->cursor_y++;
			}
		}
		return;
	}

	/* handle Ctrl combos */
	if (mod & FLURMP_CONSOLE_MOD_CTRL)
	{
		if ((c == 'c' || c == 'C') && mod == FLURMP_CONSOLE_MOD_CTRL)
		{
			clear_buffer(console);

			return;
		}
	}

	/* skip the rest of the unprintable characters */
	if (c < 0x20)
		return;

	if (console->char_count >= FL_BUFFER_LIMIT || c == '\0')
		return;

	buffer[console->char_count++] = c;

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

void fl_print(fl_console* console, const char* s)
{

}

void submit_buffer(fl_context* context, fl_console* console)
{
	if (!strcmp("quit", buffer))
	{
		context->done = 1;
	}

	if (!strcmp("info", buffer))
	{
		printf("Flurmp\nVersion: 1.0.0\nAuthor: John Powell\n");
	}

	clear_buffer(console);
}

static void clear_buffer(fl_console* console)
{
	int i;
	for (i = 0; i < FL_BUFFER_LIMIT; i++)
		buffer[i] = '\0';

	console->cursor_x = 0;
	console->cursor_y = 0;
	console->char_count = 0;
}

char fl_sc_to_char(int sc, int* flag, unsigned char mod)
{
	switch (sc)
	{
	case FLURMP_SC_A: *flag = FLURMP_INPUT_A; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'A'; else return 'a';
	case FLURMP_SC_B: *flag = FLURMP_INPUT_B; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'B'; else return 'b';
	case FLURMP_SC_C: *flag = FLURMP_INPUT_C; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'C'; else return 'c';
	case FLURMP_SC_D: *flag = FLURMP_INPUT_D; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'D'; else return 'd';
	case FLURMP_SC_E: *flag = FLURMP_INPUT_E; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'E'; else return 'e';
	case FLURMP_SC_F: *flag = FLURMP_INPUT_F; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'F'; else return 'f';
	case FLURMP_SC_G: *flag = FLURMP_INPUT_G; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'G'; else return 'g';
	case FLURMP_SC_H: *flag = FLURMP_INPUT_H; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'H'; else return 'h';
	case FLURMP_SC_I: *flag = FLURMP_INPUT_I; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'I'; else return 'i';
	case FLURMP_SC_J: *flag = FLURMP_INPUT_J; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'J'; else return 'j';
	case FLURMP_SC_K: *flag = FLURMP_INPUT_K; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'K'; else return 'k';
	case FLURMP_SC_L: *flag = FLURMP_INPUT_L; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'L'; else return 'l';
	case FLURMP_SC_M: *flag = FLURMP_INPUT_M; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'M'; else return 'm';
	case FLURMP_SC_N: *flag = FLURMP_INPUT_N; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'N'; else return 'n';
	case FLURMP_SC_O: *flag = FLURMP_INPUT_O; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'O'; else return 'o';
	case FLURMP_SC_P: *flag = FLURMP_INPUT_P; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'P'; else return 'p';
	case FLURMP_SC_Q: *flag = FLURMP_INPUT_Q; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'Q'; else return 'q';
	case FLURMP_SC_R: *flag = FLURMP_INPUT_R; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'R'; else return 'r';
	case FLURMP_SC_S: *flag = FLURMP_INPUT_S; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'S'; else return 's';
	case FLURMP_SC_T: *flag = FLURMP_INPUT_T; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'T'; else return 't';
	case FLURMP_SC_U: *flag = FLURMP_INPUT_U; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'U'; else return 'u';
	case FLURMP_SC_V: *flag = FLURMP_INPUT_V; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'V'; else return 'v';
	case FLURMP_SC_W: *flag = FLURMP_INPUT_W; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'W'; else return 'w';
	case FLURMP_SC_X: *flag = FLURMP_INPUT_X; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'X'; else return 'x';
	case FLURMP_SC_Y: *flag = FLURMP_INPUT_Y; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'Y'; else return 'y';
	case FLURMP_SC_Z: *flag = FLURMP_INPUT_Z; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'Z'; else return 'z';
	case FLURMP_SC_0: *flag = FLURMP_INPUT_0; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return ')'; else return '0';
	case FLURMP_SC_1: *flag = FLURMP_INPUT_1; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '!'; else return '1';
	case FLURMP_SC_2: *flag = FLURMP_INPUT_2; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '@'; else return '2';
	case FLURMP_SC_3: *flag = FLURMP_INPUT_3; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '#'; else return '3';
	case FLURMP_SC_4: *flag = FLURMP_INPUT_4; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '$'; else return '4';
	case FLURMP_SC_5: *flag = FLURMP_INPUT_5; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '%'; else return '5';
	case FLURMP_SC_6: *flag = FLURMP_INPUT_6; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '^'; else return '6';
	case FLURMP_SC_7: *flag = FLURMP_INPUT_7; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '&'; else return '7';
	case FLURMP_SC_8: *flag = FLURMP_INPUT_8; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '*'; else return '8';
	case FLURMP_SC_9: *flag = FLURMP_INPUT_9; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '('; else return '9';

	case FLURMP_SC_COMMA:        *flag = FLURMP_INPUT_COMMA;        if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '<'; else return ',';
	case FLURMP_SC_PERIOD:       *flag = FLURMP_INPUT_PERIOD;       if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '>'; else return '.';
	case FLURMP_SC_LEFTBRACKET:  *flag = FLURMP_INPUT_LEFTBRACKET;  if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '{'; else return '[';
	case FLURMP_SC_RIGHTBRACKET: *flag = FLURMP_INPUT_RIGHTBRACKET; if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '}'; else return ']';
	case FLURMP_SC_SEMICOLON:    *flag = FLURMP_INPUT_SEMICOLON;    if (mod & FLURMP_CONSOLE_MOD_SHIFT) return ':'; else return ';';
	case FLURMP_SC_APOSTRAPHE:   *flag = FLURMP_INPUT_APOSTRAPHE;   if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '"'; else return '\'';
	case FLURMP_SC_SLASH:        *flag = FLURMP_INPUT_SLASH;        if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '?'; else return '/';
	case FLURMP_SC_BACKSLASH:    *flag = FLURMP_INPUT_BACKSLASH;    if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '|'; else return '\\';
	case FLURMP_SC_MINUS:        *flag = FLURMP_INPUT_MINUS;        if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '_'; else return '-';
	case FLURMP_SC_EQUALS:       *flag = FLURMP_INPUT_EQUALS;       if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '+'; else return '=';
	case FLURMP_SC_BACKTICK:     *flag = FLURMP_INPUT_BACKTICK;     if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '~'; else return '`';

	case FLURMP_SC_SPACE:     *flag = FLURMP_INPUT_SPACE;     return ' ';
	case FLURMP_SC_BACKSPACE: *flag = FLURMP_INPUT_BACKSPACE; return (char)0x08;
	case FLURMP_SC_RETURN:    *flag = FLURMP_INPUT_RETURN;    return (char)0x0A;
	case FLURMP_SC_RETURN2:   *flag = FLURMP_INPUT_RETURN2;   return (char)0x0A;

	default: *flag = FLURMP_INPUT_UNKNOWN; return '\0';
	}
}
