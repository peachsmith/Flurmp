/**
 * A wrapper around SDL functionality.
 */
#ifndef FLURMP_SDL_H
#define FLURMP_SDL_H

#include "flurmp.h"

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#define FLURMP_QUIT     SDL_QUIT
#define FLURMP_SC_LIMIT SDL_NUM_SCANCODES

/* -------------------------------------------------------------- */
/*                          Scancodes                             */
/* -------------------------------------------------------------- */
#define FLURMP_SC_A            SDL_SCANCODE_A /* 1st scancode */
#define FLURMP_SC_B            SDL_SCANCODE_B
#define FLURMP_SC_C            SDL_SCANCODE_C
#define FLURMP_SC_D            SDL_SCANCODE_D
#define FLURMP_SC_E            SDL_SCANCODE_E
#define FLURMP_SC_F            SDL_SCANCODE_F
#define FLURMP_SC_G            SDL_SCANCODE_G
#define FLURMP_SC_H            SDL_SCANCODE_H
#define FLURMP_SC_I            SDL_SCANCODE_I
#define FLURMP_SC_J            SDL_SCANCODE_J
#define FLURMP_SC_K            SDL_SCANCODE_K
#define FLURMP_SC_L            SDL_SCANCODE_L
#define FLURMP_SC_M            SDL_SCANCODE_M
#define FLURMP_SC_N            SDL_SCANCODE_N
#define FLURMP_SC_O            SDL_SCANCODE_O
#define FLURMP_SC_P            SDL_SCANCODE_P
#define FLURMP_SC_Q            SDL_SCANCODE_Q
#define FLURMP_SC_R            SDL_SCANCODE_R
#define FLURMP_SC_S            SDL_SCANCODE_S
#define FLURMP_SC_T            SDL_SCANCODE_T /* 20th */
#define FLURMP_SC_U            SDL_SCANCODE_U
#define FLURMP_SC_V            SDL_SCANCODE_V
#define FLURMP_SC_W            SDL_SCANCODE_W
#define FLURMP_SC_X            SDL_SCANCODE_X
#define FLURMP_SC_Y            SDL_SCANCODE_Y
#define FLURMP_SC_Z            SDL_SCANCODE_Z
#define FLURMP_SC_0            SDL_SCANCODE_0
#define FLURMP_SC_1            SDL_SCANCODE_1
#define FLURMP_SC_2            SDL_SCANCODE_2
#define FLURMP_SC_3            SDL_SCANCODE_3 /* 30th */
#define FLURMP_SC_4            SDL_SCANCODE_4
#define FLURMP_SC_5            SDL_SCANCODE_5
#define FLURMP_SC_6            SDL_SCANCODE_6
#define FLURMP_SC_7            SDL_SCANCODE_7
#define FLURMP_SC_8            SDL_SCANCODE_8
#define FLURMP_SC_9            SDL_SCANCODE_9
#define FLURMP_SC_COMMA        SDL_SCANCODE_COMMA
#define FLURMP_SC_PERIOD       SDL_SCANCODE_PERIOD
#define FLURMP_SC_SPACE        SDL_SCANCODE_SPACE
#define FLURMP_SC_ESCAPE       SDL_SCANCODE_ESCAPE /* 40th */
#define FLURMP_SC_LSHIFT       SDL_SCANCODE_LSHIFT
#define FLURMP_SC_RSHIFT       SDL_SCANCODE_RSHIFT
#define FLURMP_SC_LCTRL        SDL_SCANCODE_LCTRL
#define FLURMP_SC_RCTRL        SDL_SCANCODE_RCTRL
#define FLURMP_SC_BACKSPACE    SDL_SCANCODE_BACKSPACE
#define FLURMP_SC_RETURN       SDL_SCANCODE_RETURN
#define FLURMP_SC_RETURN2      SDL_SCANCODE_RETURN2
#define FLURMP_SC_LEFTBRACKET  SDL_SCANCODE_LEFTBRACKET
#define FLURMP_SC_RIGHTBRACKET SDL_SCANCODE_RIGHTBRACKET
#define FLURMP_SC_SEMICOLON    SDL_SCANCODE_SEMICOLON /* 50th */
#define FLURMP_SC_APOSTRAPHE   SDL_SCANCODE_APOSTROPHE
#define FLURMP_SC_SLASH        SDL_SCANCODE_SLASH
#define FLURMP_SC_BACKSLASH    SDL_SCANCODE_BACKSLASH
#define FLURMP_SC_MINUS        SDL_SCANCODE_MINUS
#define FLURMP_SC_EQUALS       SDL_SCANCODE_EQUALS
#define FLURMP_SC_BACKTICK     SDL_SCANCODE_GRAVE /* 56th */



/* -------------------------------------------------------------- */
/*                            Types                               */
/* -------------------------------------------------------------- */
typedef SDL_Rect     fl_rect;
typedef SDL_Color    fl_color;
typedef SDL_Texture  fl_texture;
typedef TTF_Font     fl_ttf;
typedef SDL_Window   fl_window;
typedef SDL_Renderer fl_renderer;
typedef SDL_Event    fl_event;



/* -------------------------------------------------------------- */
/*                           Functions                            */
/* -------------------------------------------------------------- */

fl_window* fl_create_window(const char*, int, int, int, int);

fl_renderer* fl_create_renderer(fl_window*);

/**
 * Loads a bmp file into an image structure.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   const char* - the path to the bmp file
 *   fl_image - a reference to an image structure
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
int fl_load_bmp(fl_context*, const char*, fl_image*);

/**
 * Frees the memory allocated for an image structure.
 *
 * Params:
 *   fl_image - a reference to an image
 */
void fl_destroy_image(fl_image*);

/**
 * Loads a TTF font file into a ttf structure.
 *
 * Params:
 *   const char* - the path to the ttf file
 *   int - the point size
 *
 * Returns:
 *   fl_ttf* - a ttf structure
 */
fl_ttf* fl_load_ttf(const char*, int);

/**
 * Frees the memory allocated for a ttf structure.
 *
 * Params:
 *   fl_ttf - a reference to a ttf structure
 */
void fl_close_ttf(fl_ttf*);

/**
 * Creates an image of a single character using a font.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_resource - a font resource
 *   char - the character to convert to an image
 *
 * Returns:
 *   fl_image - an image representing the character
 */
fl_image* fl_create_glyph_image(fl_context*, fl_resource*, char);

/**
 * Creates an image of a string of text.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_resource - a font resource
 *   const char* - the text to convert to an image
 *
 * Returns:
 *   fl_image - an image representing the text
 */
fl_image* fl_create_text_image(fl_context*, fl_resource*, const char*);

/**
 * Frees the memory allocated for a texture.
 *
 * Params:
 *   fl_texture - a texture
 */
void fl_destroy_texture(fl_texture*);

/**
 * Sets the current color to be used when rendering primitives.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   int - the red value
 *   int - the green value
 *   int - the blue value
 *   int - the alpha value
 */
void fl_set_draw_color(fl_context*, int, int, int, int);

/**
 * Draws a rectangular border to the screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_rext - a rectangle
 */
void fl_draw_rect(fl_context*, fl_rect*);

/**
 * Draws a rectangle filled with a color to the screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_rext - a rectangle
 */
void fl_draw_solid_rect(fl_context*, fl_rect*);

/**
 * Draws a line to the screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   int - the first x coordinate
 *   int - the first y coordinate
 *   int - the second x coordinate
 *   int - the second y coordinate
 */
void fl_draw_line(fl_context*, int, int, int, int);

/**
 * Renders a texture to the screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_texture - a texture
 *   fl_rect - a source rectangle used for cropping
 *   fl_rect - a destination rectangle
 *   int - whether or not to flip the image horizontally
 */
void fl_draw(fl_context*, fl_texture*, fl_rect*, fl_rect*, int);

/**
 * Removes everything being rendered on the screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
void fl_render_clear(fl_context*);

/**
 * Displays the current render contents on the screen.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
void fl_render_show(fl_context*);

#endif
