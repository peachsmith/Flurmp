/**
 * Scancodes, input flags and functions for handling user input.
 *
 * Scancodes indicate the actuation of certain keys on the keyboard.
 *
 * Input flags are used to keep track of what inputs have been actuated.
 * When an input is actuated, its input flag is toggled on.
 * By toggling an input flag off, we can allow one action per key press.
 * Alternatively, by leaving an input flag as toggled, we can perform an
 * action repeatedly for the duration of a key press.
 */
#ifndef FLURMP_INPUT_IMPL_H
#define FLURMP_INPUT_IMPL_H

#include "core/flurmp_impl.h"

/* input types */
#define FLURMP_INPUT_TYPE_KEYBOARD 1
#define FLURMP_INPUT_TYPE_MOUSE 2

/* total number of scancodes */
#define FLURMP_SCANCODE_COUNT 56

/* total number of input flags */
#define FLURMP_INPUT_FLAG_COUNT 55

/* input flags */
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
#define FLURMP_INPUT_BACKTICK 54

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
 *   int - the input code of the input to check
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
 *   int - the input code of the input to check
 *
 * Returns:
 *   int - an integer indicating whether the input was actuated.
 *         1 is returned if the input was actuated, otherwise
 *         0 is returned.
 */
int fl_peek_input(fl_context* context, int type, int code);

/**
 * Checks the state of a keybaord key press.
 * If this function determines that the input event
 * has occurred, it flags the input event as consumed.
 *
 * This function should be used when an action is required
 * to happen once in response to a single key press.
 *
 * This is the same as calling fl_consume_input while passing
 * the type FLURMP_INPUT_TYPE_KEYBOARD.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   int - the scancode code of the key to check
 *
 * Returns:
 *   int - an integer indicating whether the input was actuated.
 *         1 is returned if the input was actuated, otherwise
 *         0 is returned.
 */
int fl_consume_key(fl_context* context, int code);

/**
 * Checks the state of an input event.
 *
 * This function should be used when an action is required
 * to happen continuously on each iteration of the main loop
 * as long as the input is actuated.
 *
 * This is the same as calling fl_peek_input while passing
 * the type FLURMP_INPUT_TYPE_KEYBOARD.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   int - the input code of the input to check
 *
 * Returns:
 *   int - an integer indicating whether the input was actuated.
 *         1 is returned if the input was actuated, otherwise
 *         0 is returned.
 */
int fl_peek_key(fl_context* context, int code);

/**
 * Creates an input handler.
 *
 * Params:
 *   void(*handler) (fl_context*, fl_input_handler*) - a pointer to
 *       an input handler function
 *
 * Returns:
 *   fl_input_handler - a new input handler
 */
fl_input_handler* fl_create_input_handler(void(*handler) (fl_context*, fl_input_handler*));

/**
 * Frees the memory allocated for an input handler.
 *
 * Params:
 *   fl_input_handler - an fl_input_handler
 */
void fl_destroy_input_handler(fl_input_handler* input);

/**
 * Retrieves the current active input handler.
 *
 * Params:
 *   fl_context - a Flurmp context.
 *
 * Returns:
 *   fl_input_handler - an input handler
 */
fl_input_handler* fl_get_input_handler(fl_context* context);

/**
 * Appends an input handler to the current context.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_input_handler - an input handler
 */
void fl_push_input_handler(fl_context* context, fl_input_handler* input);

/**
 * Removes the most recently added input handler from a context.
 * This does NOT destroy the input handler that was removed.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
void fl_pop_input_handler(fl_context* context);

#endif
