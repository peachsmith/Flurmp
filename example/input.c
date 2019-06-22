#include "input.h"

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
			if (!context->input.inputs[flag])
			{
				context->input.inputs[flag] = 1;

				return 1;
			}

			return 0;
		}
		else if (context->input.inputs[flag])
			context->input.inputs[flag] = 0;

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
