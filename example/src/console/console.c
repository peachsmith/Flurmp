#include "console.h"
#include "input.h"
#include "text.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ROW_COUNT 4
#define FL_BUFFER_LIMIT 208

#define nonsense

/* for now, the buffers and stuff will be static */
/* TODO: make this a member of the console structure. */
static char buffer[FL_BUFFER_LIMIT] = { '\0' };

nonsense static void clear_buffer(fl_console* console);

static void handle_input(fl_context* context, fl_input_handler* self);

static void render(fl_context* context, fl_console* console);

fl_console* fl_create_console(fl_context* context)
{
	fl_console* con = fl_alloc(fl_console, 1);

	if (con == NULL)
		return NULL;

	con->x = 10;
	con->y = 350;
	con->w = 620;
	con->h = 120;
	con->cursor_x = 0;
	con->cursor_y = 0;
	con->char_count = 0;
	con->atlas = context->fonts[FLURMP_FONT_COUSINE]->impl.font->atlas;
	con->input_handler = fl_create_input_handler(handle_input);
	con->render = render;

	return con;
}

void fl_destroy_console(fl_console* console)
{
	if (console == NULL)
		return;

	fl_destroy_input_handler(console->input_handler);

	fl_free(console);

	return;
}

static void handle_input(fl_context* context, fl_input_handler* self)
{
	int i;
	for (i = 0; i < FLURMP_SC_LIMIT; i++)
	{
		unsigned char mod = 0x00;

		/* Handle key combinations like shift and ctrl. */
		if (context->input.keystates[FLURMP_SC_LSHIFT] || context->input.keystates[FLURMP_SC_RSHIFT])
			mod |= FLURMP_CONSOLE_MOD_SHIFT;
		if (context->input.keystates[FLURMP_SC_LCTRL] || context->input.keystates[FLURMP_SC_RCTRL])
			mod |= FLURMP_CONSOLE_MOD_CTRL;

		/* Get the character that corresponds to the current scancode. */
		char c = fl_sc_to_char(i, mod);

		if (fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, i))
		{
			/* If escape is rpessed, close the console. */
			if (i == FLURMP_SC_ESCAPE)
			{
				/* Relenquish input control */
				fl_pop_input_handler(context);

				fl_destroy_console(context->console);
				context->console = NULL;
			}

			/* If return is pressed, submit the current buffer,
			   otherwise append to the current buffer. */
			if (c == 0x0A)
				submit_buffer(context, context->console);
			else
				fl_putc(context->console, c, mod);
		}
	}
}

static void render(fl_context* context, fl_console* console)
{
	SDL_SetRenderDrawColor(context->renderer, 150, 50, 150, 120);

	int i;
	int cx;
	int cy;
	SDL_Rect r;
	SDL_Rect dest;
	SDL_Rect src;

	r.x = console->x;
	r.y = console->y;
	r.w = console->w;
	r.h = console->h;

	SDL_RenderFillRect(context->renderer, &r);

	SDL_SetRenderDrawColor(context->renderer, 250, 250, 250, 255);
	SDL_RenderDrawRect(context->renderer, &r);
	
	dest.x = 0;
	dest.y = 0;

	src.x = 0;
	src.y = 0;

	for (i = cx = cy = 0; i < FL_BUFFER_LIMIT; i++)
	{
		if (buffer[i] >= 0x20)
		{
			/* Get the appropriate glyph from the font atlas. */
			fl_glyph* g = fl_char_to_glyph(console->atlas, buffer[i]);

			dest.x = console->x + cx + 4;
			dest.y = console->y + cy * 22 + 4;
			dest.w = g->image->surface->w;
			dest.h = g->image->surface->h;

			src.w = g->image->surface->w;
			src.h = g->image->surface->h;


			SDL_RenderCopy(context->renderer, g->image->texture, &src, &dest);

			if (cx > 500)
			{
				cx = 0;
				if (cy < ROW_COUNT - 1)
					cy++;
			}
			else
				cx += g->image->surface->w;
		}
		else if (buffer[i] == 0x0A)
		{
			cx = 0;
			if (cy < ROW_COUNT - 1)
				cy++;
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
			if (console->cursor_y < ROW_COUNT - 1)
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

char fl_sc_to_char(int sc, unsigned char mod)
{
	switch (sc)
	{
	case FLURMP_SC_A: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'A'; else return 'a';
	case FLURMP_SC_B: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'B'; else return 'b';
	case FLURMP_SC_C: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'C'; else return 'c';
	case FLURMP_SC_D: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'D'; else return 'd';
	case FLURMP_SC_E: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'E'; else return 'e';
	case FLURMP_SC_F: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'F'; else return 'f';
	case FLURMP_SC_G: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'G'; else return 'g';
	case FLURMP_SC_H: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'H'; else return 'h';
	case FLURMP_SC_I: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'I'; else return 'i';
	case FLURMP_SC_J: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'J'; else return 'j';
	case FLURMP_SC_K: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'K'; else return 'k';
	case FLURMP_SC_L: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'L'; else return 'l';
	case FLURMP_SC_M: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'M'; else return 'm';
	case FLURMP_SC_N: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'N'; else return 'n';
	case FLURMP_SC_O: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'O'; else return 'o';
	case FLURMP_SC_P: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'P'; else return 'p';
	case FLURMP_SC_Q: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'Q'; else return 'q';
	case FLURMP_SC_R: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'R'; else return 'r';
	case FLURMP_SC_S: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'S'; else return 's';
	case FLURMP_SC_T: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'T'; else return 't';
	case FLURMP_SC_U: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'U'; else return 'u';
	case FLURMP_SC_V: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'V'; else return 'v';
	case FLURMP_SC_W: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'W'; else return 'w';
	case FLURMP_SC_X: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'X'; else return 'x';
	case FLURMP_SC_Y: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'Y'; else return 'y';
	case FLURMP_SC_Z: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return 'Z'; else return 'z';
	case FLURMP_SC_0: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return ')'; else return '0';
	case FLURMP_SC_1: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '!'; else return '1';
	case FLURMP_SC_2: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '@'; else return '2';
	case FLURMP_SC_3: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '#'; else return '3';
	case FLURMP_SC_4: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '$'; else return '4';
	case FLURMP_SC_5: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '%'; else return '5';
	case FLURMP_SC_6: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '^'; else return '6';
	case FLURMP_SC_7: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '&'; else return '7';
	case FLURMP_SC_8: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '*'; else return '8';
	case FLURMP_SC_9: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '('; else return '9';

	case FLURMP_SC_COMMA:        if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '<'; else return ',';
	case FLURMP_SC_PERIOD:       if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '>'; else return '.';
	case FLURMP_SC_LEFTBRACKET:  if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '{'; else return '[';
	case FLURMP_SC_RIGHTBRACKET: if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '}'; else return ']';
	case FLURMP_SC_SEMICOLON:    if (mod & FLURMP_CONSOLE_MOD_SHIFT) return ':'; else return ';';
	case FLURMP_SC_APOSTRAPHE:   if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '"'; else return '\'';
	case FLURMP_SC_SLASH:        if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '?'; else return '/';
	case FLURMP_SC_BACKSLASH:    if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '|'; else return '\\';
	case FLURMP_SC_MINUS:        if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '_'; else return '-';
	case FLURMP_SC_EQUALS:       if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '+'; else return '=';
	case FLURMP_SC_BACKTICK:     if (mod & FLURMP_CONSOLE_MOD_SHIFT) return '~'; else return '`';

	case FLURMP_SC_SPACE:     return ' ';
	case FLURMP_SC_BACKSPACE: return (char)0x08;
	case FLURMP_SC_RETURN:    return (char)0x0A;
	case FLURMP_SC_RETURN2:   return (char)0x0A;

	default: return '\0';
	}
}
