#include "core/console.h"
#include "core/input.h"
#include "core/text.h"

#define ROW_COUNT 4
#define BUFFER_LIMIT 208
#define LINE_WIDTH 500



/* -------------------------------------------------------------- */
/*                  internal console functions                    */
/* -------------------------------------------------------------- */

/**
 * Renders a console to the screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_console - a console
 */
static void render(fl_context* context, fl_console* console);

/**
 * The input handler function for a console.
 * A console allows for text input via a keyboard, so any keys required
 * to send the desired input must be handled.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_input_handler - an input handler
 */
static void handle_input(fl_context* context, fl_input_handler* self);

/**
 * Writes a character to a console's buffer.
 *
 * Params:
 *   fl_console - a console
 *   char - a character
 *   unsigned char - modifier flags
 */
static void console_putc(fl_console* console, char c, unsigned char mod);

/**
 * Clears the contents of the current console buffer by setting all
 * characters to '\0' and setting the buffer count to 0.
 *
 * Params:
 *   fl_console - a console
 */
static void clear_buffer(fl_console* console);

/**
 * Submits the current command buffer for a console.
 * An action is taken based on the contents of the buffer at the
 * time of submission.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_console - a console
 */
static void submit_buffer(fl_context* context, fl_console* console);

/**
 * Gets a character based on the scancode from a keypress.
 * If a combination of keys is pressed, such as Shift+A, then
 * the appropriate flags should be passed in.
 *
 * Params:
 *   int - a scancode
 *   unsigned char - modifier flags indicating key combinations
 *
 * Return:
 *   char - a character
 */
static char fl_sc_to_char(int sc, unsigned char mod);



/* -------------------------------------------------------------- */
/*          internal console functions (implementation)           */
/* -------------------------------------------------------------- */

static void render(fl_context* context, fl_console* self)
{
	int i;         /* index variable     */
	int cx;        /* cursor x position  */
	int cy;        /* cursor y position  */
	fl_rect frame; /* dialog frame       */
	fl_rect src;   /* render source      */
	fl_rect dest;  /* render destination */

	fl_set_rect(&frame, self->x, self->y, self->w, self->h);

	/* Render the console frame. */
	fl_set_draw_color(context, 150, 50, 150, 120);
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
			fl_image* g = fl_char_to_glyph(self->font, self->buffer[i]);

			dest.x = self->x + cx + 4;
			dest.y = self->y + cy * 22 + 4;
			dest.w = g->w;
			dest.h = g->h;

			src.w = g->w;
			src.h = g->h;

			fl_draw(context, g->texture, &src, &dest, 0);

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
				cx += g->w;
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
	int i;
	for (i = 0; i < FLURMP_SC_LIMIT; i++)
	{
		unsigned char mod = 0x00;

		/* Handle key combinations like shift and ctrl. */
		if (fl_peek_key(context, FLURMP_SC_LSHIFT) || fl_peek_key(context, FLURMP_SC_RSHIFT))
			mod |= FLURMP_CONSOLE_MOD_SHIFT;

		if (fl_peek_key(context, FLURMP_SC_LCTRL) || fl_peek_key(context, FLURMP_SC_RCTRL))
			mod |= FLURMP_CONSOLE_MOD_CTRL;

		if (fl_consume_key(context, i))
		{
			/* Get the character that corresponds
			   to the current scancode. */
			char c = fl_sc_to_char(i, mod);

			/* If escape is rpessed, close the console. */
			if (i == FLURMP_SC_ESCAPE)
			{
				/* Relenquish input control */
				fl_pop_input_handler(context);

				fl_destroy_console(context->console);
				context->console = NULL;

				return;
			}

			/* If return is pressed, submit the current buffer,
			   otherwise append to the current buffer. */
			if (c == 0x0A)
				submit_buffer(context, context->console);
			else
				console_putc(context->console, c, mod);
		}
	}
}

static void console_putc(fl_console* console, char c, unsigned char mod)
{
	if (console->buffer_count >= BUFFER_LIMIT || c == '\0')
		return;

	/* Handle backspaces. */
	if (c == 0x08)
	{
		/* If there is at least one character in the buffer,
		   set the current character to '\0' and decrement
		   the character count. */
		if (console->buffer_count > 0)
			console->buffer[console->buffer_count-- - 1] = '\0';

		return;
	}

	/* Handle newlines. */
	if (c == 0x0A)
	{
		if (console->buffer_count < BUFFER_LIMIT - 1)
		{
			console->cursor_x = 0;
			if (console->cursor_y < ROW_COUNT - 1)
			{
				console->buffer[console->buffer_count++] = '\n';
				console->cursor_y++;
			}
		}
		return;
	}

	/* Skip unprintable characters. */
	if (c < 0x20)
		return;

	/* handle Ctrl combos */
	if (mod & FLURMP_CONSOLE_MOD_CTRL)
	{
		if ((c == 'c' || c == 'C') && mod == FLURMP_CONSOLE_MOD_CTRL)
		{
			clear_buffer(console);
			return;
		}
	}

	console->buffer[console->buffer_count++] = c;
}

static void submit_buffer(fl_context* context, fl_console* console)
{
	const char* buf = console->buffer;

	/* Command List
	   1. quit - flags the context as done.
	   2. info - prints information about the application to stdout.
	*/

	if (!strcmp("quit", buf))
		context->done = 1;

	if (!strcmp("info", buf))
		printf("Flurmp\nVersion: 1.0.0\nAuthor: John Powell\n");

	clear_buffer(console);
}

static void clear_buffer(fl_console* console)
{
	fl_zero(console->buffer, BUFFER_LIMIT);

	console->cursor_x = 0;
	console->cursor_y = 0;
	console->buffer_count = 0;
}

static char fl_sc_to_char(int sc, unsigned char mod)
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



/* -------------------------------------------------------------- */
/*                    console.h implementation                    */
/* -------------------------------------------------------------- */

fl_console* fl_create_console(fl_context* context)
{
	/* Allocate memory for a console. */
	fl_console* con = fl_alloc(fl_console, 1);

	/* Verify console allocation. */
	if (con == NULL)
		return NULL;

	con->x = 10;
	con->y = 350;
	con->w = 620;
	con->h = 120;
	con->buffer = NULL;
	con->buffer_count = 0;
	con->cursor_x = 0;
	con->cursor_y = 0;
	con->font = context->fonts[FLURMP_FONT_COUSINE]->impl.font;
	con->render = render;

	/* Create the input handler for the console. */
	con->input_handler = fl_create_input_handler(handle_input);

	/* Verify input handler creation. */
	if (con->input_handler == NULL)
	{
		fl_destroy_console(con);
		return NULL;
	}

	/* Allocate memory for the console buffer. */
	con->buffer = fl_alloc(char, BUFFER_LIMIT);

	/* Verify console buffer allocation. */
	if (con->buffer == NULL)
	{
		fl_destroy_console(con);
		return NULL;
	}

	/* Set all characters in the buffer to '\0' */
	fl_zero(con->buffer, BUFFER_LIMIT);

	return con;
}

void fl_destroy_console(fl_console* console)
{
	if (console == NULL)
		return;

	if (console->input_handler != NULL)
		fl_destroy_input_handler(console->input_handler);

	if (console->buffer != NULL)
		fl_free(console->buffer);

	fl_free(console);

	return;
}
