#ifndef FLURMP_INPUT_IMPL_H
#define FLURMP_INPUT_IMPL_H

#include "flurmp_impl.h"

#define FLURMP_INPUT_TYPE_KEYBOARD 1
#define FLURMP_INPUT_TYPE_MOUSE 2

/* 55 scancodes are currently supported */
#define FLURMP_SC_A SDL_SCANCODE_A /* 1st scancode */
#define FLURMP_SC_B SDL_SCANCODE_B
#define FLURMP_SC_C SDL_SCANCODE_C
#define FLURMP_SC_D SDL_SCANCODE_D
#define FLURMP_SC_E SDL_SCANCODE_E
#define FLURMP_SC_F SDL_SCANCODE_F
#define FLURMP_SC_G SDL_SCANCODE_G
#define FLURMP_SC_H SDL_SCANCODE_H
#define FLURMP_SC_I SDL_SCANCODE_I
#define FLURMP_SC_J SDL_SCANCODE_J
#define FLURMP_SC_K SDL_SCANCODE_K
#define FLURMP_SC_L SDL_SCANCODE_L
#define FLURMP_SC_M SDL_SCANCODE_M
#define FLURMP_SC_N SDL_SCANCODE_N
#define FLURMP_SC_O SDL_SCANCODE_O
#define FLURMP_SC_P SDL_SCANCODE_P
#define FLURMP_SC_Q SDL_SCANCODE_Q
#define FLURMP_SC_R SDL_SCANCODE_R
#define FLURMP_SC_S SDL_SCANCODE_S
#define FLURMP_SC_T SDL_SCANCODE_T /* 20th */
#define FLURMP_SC_U SDL_SCANCODE_U
#define FLURMP_SC_V SDL_SCANCODE_V
#define FLURMP_SC_W SDL_SCANCODE_W
#define FLURMP_SC_X SDL_SCANCODE_X
#define FLURMP_SC_Y SDL_SCANCODE_Y
#define FLURMP_SC_Z SDL_SCANCODE_Z
#define FLURMP_SC_0 SDL_SCANCODE_0
#define FLURMP_SC_1 SDL_SCANCODE_1
#define FLURMP_SC_2 SDL_SCANCODE_2
#define FLURMP_SC_3 SDL_SCANCODE_3 /* 30th */
#define FLURMP_SC_4 SDL_SCANCODE_4
#define FLURMP_SC_5 SDL_SCANCODE_5
#define FLURMP_SC_6 SDL_SCANCODE_6
#define FLURMP_SC_7 SDL_SCANCODE_7
#define FLURMP_SC_8 SDL_SCANCODE_8
#define FLURMP_SC_9 SDL_SCANCODE_9
#define FLURMP_SC_COMMA SDL_SCANCODE_COMMA
#define FLURMP_SC_PERIOD SDL_SCANCODE_PERIOD
#define FLURMP_SC_SPACE SDL_SCANCODE_SPACE
#define FLURMP_SC_ESCAPE SDL_SCANCODE_ESCAPE /* 40th */
#define FLURMP_SC_LSHIFT SDL_SCANCODE_LSHIFT
#define FLURMP_SC_RSHIFT SDL_SCANCODE_RSHIFT
#define FLURMP_SC_LCTRL SDL_SCANCODE_LCTRL
#define FLURMP_SC_RCTRL SDL_SCANCODE_RCTRL
#define FLURMP_SC_BACKSPACE SDL_SCANCODE_BACKSPACE
#define FLURMP_SC_RETURN SDL_SCANCODE_RETURN
#define FLURMP_SC_RETURN2 SDL_SCANCODE_RETURN2
#define FLURMP_SC_LEFTBRACKET SDL_SCANCODE_LEFTBRACKET
#define FLURMP_SC_RIGHTBRACKET SDL_SCANCODE_RIGHTBRACKET
#define FLURMP_SC_SEMICOLON SDL_SCANCODE_SEMICOLON /* 50th */
#define FLURMP_SC_APOSTRAPHE SDL_SCANCODE_APOSTROPHE
#define FLURMP_SC_SLASH SDL_SCANCODE_SLASH
#define FLURMP_SC_BACKSLASH SDL_SCANCODE_BACKSLASH
#define FLURMP_SC_MINUS SDL_SCANCODE_MINUS
#define FLURMP_SC_EQUALS SDL_SCANCODE_EQUALS /* 55th */

#define FLURMP_SC_LIMIT SDL_NUM_SCANCODES

/* Input flags are used to keep track of which inputs are actuated.
   There are currently 54 input flags implemented. */
#define FLURMP_INPUT_UNKNOWN 0
#define FLURMP_INPUT_A 1
#define FLURMP_INPUT_B 2
#define FLURMP_INPUT_C 3
#define FLURMP_INPUT_D 4
#define FLURMP_INPUT_E 5
#define FLURMP_INPUT_F 6
#define FLURMP_INPUT_G 7
#define FLURMP_INPUT_H 8
#define FLURMP_INPUT_I 9
#define FLURMP_INPUT_J 10
#define FLURMP_INPUT_K 11
#define FLURMP_INPUT_L 12
#define FLURMP_INPUT_M 13
#define FLURMP_INPUT_N 14
#define FLURMP_INPUT_O 15
#define FLURMP_INPUT_P 16
#define FLURMP_INPUT_Q 17
#define FLURMP_INPUT_R 18
#define FLURMP_INPUT_S 19
#define FLURMP_INPUT_T 20
#define FLURMP_INPUT_U 21
#define FLURMP_INPUT_V 22
#define FLURMP_INPUT_W 23
#define FLURMP_INPUT_X 24
#define FLURMP_INPUT_Y 25
#define FLURMP_INPUT_Z 26
#define FLURMP_INPUT_0 27
#define FLURMP_INPUT_1 28
#define FLURMP_INPUT_2 29
#define FLURMP_INPUT_3 30
#define FLURMP_INPUT_4 31
#define FLURMP_INPUT_5 32
#define FLURMP_INPUT_6 33
#define FLURMP_INPUT_7 34
#define FLURMP_INPUT_8 35
#define FLURMP_INPUT_9 36
#define FLURMP_INPUT_COMMA 37
#define FLURMP_INPUT_PERIOD 38
#define FLURMP_INPUT_SPACE 39
#define FLURMP_INPUT_ESCAPE 40
#define FLURMP_INPUT_SHIFT 41
#define FLURMP_INPUT_CTRL 42
#define FLURMP_INPUT_BACKSPACE 43
#define FLURMP_INPUT_RETURN 44
#define FLURMP_INPUT_RETURN2 45
#define FLURMP_INPUT_LEFTBRACKET 46
#define FLURMP_INPUT_RIGHTBRACKET 47
#define FLURMP_INPUT_SEMICOLON 48
#define FLURMP_INPUT_APOSTRAPHE 49
#define FLURMP_INPUT_SLASH 50
#define FLURMP_INPUT_BACKSLASH 51
#define FLURMP_INPUT_MINUS 52
#define FLURMP_INPUT_EQUALS 53

/**
 * Determines the input flag corresponding to the
 * specified input code and type.
 *
 * Params:
 *   int - the type of event (e.g. FLURMP_INPUT_TYPE_KEYBOARD)
 *   int - the input code (e.g. a scancode like FLURMP_SC_A)
 */
int fl_code_to_flag(int type, int code);

/**
 * Checks the state of an input event.
 * If this function determines that the input event
 * has occurred, it flags the input event as consumed.
 *
 * This function should be used when an action is required
 * to happen once in response to a single input event.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   int - the input type (e.g. FLURMP_INPUT_TYPE_KEYBOARD)
 *
 * Returns:
 *   int - an integer indicating whether the input was actuated.
 *         1 is returned if the input was actuated, otherwise
 *         0 is returned.
 */
int fl_consume_input(fl_context* context, int type, int code);

/**
 * Checks the state of an input event.
 *
 * This function should be used when an action is required
 * to happen continuously on each iteration of the main loop
 * as long as the input is actuated.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   int - the input type (e.g. FLURMP_INPUT_TYPE_KEYBOARD)
 *
 * Returns:
 *   int - an integer indicating whether the input was actuated.
 *         1 is returned if the input was actuated, otherwise
 *         0 is returned.
 */
int fl_peek_input(fl_context* context, int type, int code);

#endif
