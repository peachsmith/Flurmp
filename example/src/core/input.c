#include "core/input.h"

int fl_code_to_flag(int type, int code)
{
	if (type == FLURMP_INPUT_TYPE_KEYBOARD)
	{
		switch (code)
		{
		case FLURMP_SC_A: return FLURMP_INPUT_A;
		case FLURMP_SC_B: return FLURMP_INPUT_B;
		case FLURMP_SC_C: return FLURMP_INPUT_C;
		case FLURMP_SC_D: return FLURMP_INPUT_D;
		case FLURMP_SC_E: return FLURMP_INPUT_E;
		case FLURMP_SC_F: return FLURMP_INPUT_F;
		case FLURMP_SC_G: return FLURMP_INPUT_G;
		case FLURMP_SC_H: return FLURMP_INPUT_H;
		case FLURMP_SC_I: return FLURMP_INPUT_I;
		case FLURMP_SC_J: return FLURMP_INPUT_J;
		case FLURMP_SC_K: return FLURMP_INPUT_K;
		case FLURMP_SC_L: return FLURMP_INPUT_L;
		case FLURMP_SC_M: return FLURMP_INPUT_M;
		case FLURMP_SC_N: return FLURMP_INPUT_N;
		case FLURMP_SC_O: return FLURMP_INPUT_O;
		case FLURMP_SC_P: return FLURMP_INPUT_P;
		case FLURMP_SC_Q: return FLURMP_INPUT_Q;
		case FLURMP_SC_R: return FLURMP_INPUT_R;
		case FLURMP_SC_S: return FLURMP_INPUT_S;
		case FLURMP_SC_T: return FLURMP_INPUT_T;
		case FLURMP_SC_U: return FLURMP_INPUT_U;
		case FLURMP_SC_V: return FLURMP_INPUT_V;
		case FLURMP_SC_W: return FLURMP_INPUT_W;
		case FLURMP_SC_X: return FLURMP_INPUT_X;
		case FLURMP_SC_Y: return FLURMP_INPUT_Y;
		case FLURMP_SC_Z: return FLURMP_INPUT_Z;
		case FLURMP_SC_0: return FLURMP_INPUT_0;
		case FLURMP_SC_1: return FLURMP_INPUT_1;
		case FLURMP_SC_2: return FLURMP_INPUT_2;
		case FLURMP_SC_3: return FLURMP_INPUT_3;
		case FLURMP_SC_4: return FLURMP_INPUT_4;
		case FLURMP_SC_5: return FLURMP_INPUT_5;
		case FLURMP_SC_6: return FLURMP_INPUT_6;
		case FLURMP_SC_7: return FLURMP_INPUT_7;
		case FLURMP_SC_8: return FLURMP_INPUT_8;
		case FLURMP_SC_9: return FLURMP_INPUT_9;
		case FLURMP_SC_ESCAPE: return FLURMP_INPUT_ESCAPE;
		case FLURMP_SC_COMMA: return FLURMP_INPUT_COMMA;
		case FLURMP_SC_PERIOD: return FLURMP_INPUT_PERIOD;
		case FLURMP_SC_SPACE: return FLURMP_INPUT_SPACE;
		case FLURMP_SC_LSHIFT:
		case FLURMP_SC_RSHIFT:
			return FLURMP_INPUT_SHIFT;
		case FLURMP_SC_LCTRL:
		case FLURMP_SC_RCTRL:
			return FLURMP_INPUT_CTRL;
		case FLURMP_SC_BACKSPACE: return FLURMP_INPUT_BACKSPACE;
		case FLURMP_SC_RETURN: return FLURMP_INPUT_RETURN;
		case FLURMP_SC_RETURN2: return FLURMP_INPUT_RETURN2;
		case FLURMP_SC_LEFTBRACKET: return FLURMP_INPUT_LEFTBRACKET;
		case FLURMP_SC_RIGHTBRACKET: return FLURMP_INPUT_RIGHTBRACKET;
		case FLURMP_SC_SEMICOLON: return FLURMP_INPUT_SEMICOLON;
		case FLURMP_SC_APOSTRAPHE: return FLURMP_INPUT_APOSTRAPHE;
		case FLURMP_SC_SLASH: return FLURMP_INPUT_SLASH;
		case FLURMP_SC_BACKSLASH: return FLURMP_INPUT_BACKSLASH;
		case FLURMP_SC_MINUS: return FLURMP_INPUT_MINUS;
		case FLURMP_SC_EQUALS: return FLURMP_INPUT_EQUALS;
		case FLURMP_SC_BACKTICK: return FLURMP_INPUT_BACKTICK;

		default:
			return FLURMP_INPUT_UNKNOWN;
		}
	}

	return FLURMP_INPUT_UNKNOWN;
}

int fl_consume_input(fl_context* context, int type, int code)
{
	if (type == FLURMP_INPUT_TYPE_KEYBOARD)
	{
		int flag = fl_code_to_flag(type, code);

		if (context->input.keystates[code])
		{
			if (!context->input.flags[flag])
			{
				context->input.flags[flag] = 1;

				return 1;
			}

			return 0;
		}
		else if (context->input.flags[flag])
			context->input.flags[flag] = 0;

		return 0;
	}

	return 0;
}

int fl_peek_input(fl_context* context, int type, int code)
{
	if (type == FLURMP_INPUT_TYPE_KEYBOARD)
	{
		if (context->input.keystates[code])
			return 1;

		return 0;
	}

	return 0;
}

int fl_consume_key(fl_context* context, int code)
{
	return fl_consume_input(context, FLURMP_INPUT_TYPE_KEYBOARD, code);
}

int fl_peek_key(fl_context* context, int code)
{
	return fl_peek_input(context, FLURMP_INPUT_TYPE_KEYBOARD, code);
}

fl_input_handler* fl_create_input_handler(void(*handler) (fl_context*, fl_input_handler*))
{
	fl_input_handler* input;

	input = fl_alloc(fl_input_handler, 1);

	if (input == NULL)
		return NULL;

	input->handle_input = handler;
	input->parent = NULL;
	input->child = NULL;

	return input;
}

void fl_destroy_input_handler(fl_input_handler* input)
{
	if (input == NULL)
		return;

	/* Destroy the child input handlers. */
	if (input->child != NULL)
		fl_destroy_input_handler(input->child);

	fl_free(input);
}

fl_input_handler* fl_get_input_handler(fl_context* context)
{
	if (context == NULL || context->input_handler == NULL)
		return NULL;

	fl_input_handler* input = context->input_handler;

	/* Traverse the input handler list until we find one with no child. */
	while (input->child != NULL)
		input = input->child;

	return input;
}

void fl_push_input_handler(fl_context* context, fl_input_handler* input)
{
	if (context == NULL || input == NULL)
		return;

	if (context->input_handler == NULL)
	{
		context->input_handler = input;
	}
	else
	{
		fl_input_handler* ih = context->input_handler;

		/* Traverse the input handler list until we find
		   an input handler with no child. */
		while (ih->child != NULL)
			ih = ih->child;

		/* Link the new input handler to the current,
		   active input handler. */
		ih->child = input;
		input->parent = ih;
	}
}

void fl_pop_input_handler(fl_context* context)
{
	if (context == NULL || context->input_handler == NULL)
		return;

	fl_input_handler* ih = context->input_handler;

	/* Traverse the input handler list until we find
	   an input handler with no child. */
	while (ih->child != NULL)
		ih = ih->child;

	/* If the current input handler has no parent,
	   clear the input handler pointer. */
	if (ih->parent == NULL)
	{
		context->input_handler = NULL;
		return;
	}
		
	/* Clear the parent and child pointers to prevent the
	   destruction of other input handlers when this
	   input handler is destroyed elsewhere. */
	ih->parent->child = NULL;
	ih->parent = NULL;
}
