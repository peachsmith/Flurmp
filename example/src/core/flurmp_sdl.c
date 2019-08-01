/**
 * Implementation of the Flurmp SDL wrapper.
 * Contains implementation of functions for creating rendering contexts,
 * loading images and fonts, and rendering data to the screen.
 *
 * This file also contains the implementation of the following functions
 * from the flurmp.h header file:
 *   fl_sleep
 *   fl_handle_events
 *   fl_begin_frame
 *   fl_end_frame
 *   fl_initialize
 *   fl_terminate
 */
#include "core/flurmp_impl.h"
#include "core/flurmp_sdl.h"



/* -------------------------------------------------------------- */
/*                        Core Functions                          */
/* -------------------------------------------------------------- */

fl_window* fl_create_window(const char* title, int x, int y, int w, int h)
{
	return SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_SHOWN);
}

void fl_destroy_window(fl_window* window)
{
	SDL_DestroyWindow(window);
}

fl_renderer* fl_create_renderer(fl_window* window)
{
	fl_renderer* ren = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (ren == NULL)
		return NULL;

	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

	return ren;
}

void fl_destroy_renderer(fl_renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
}

const unsigned char* fl_get_key_states()
{
	return SDL_GetKeyboardState(NULL);
}



/* -------------------------------------------------------------- */
/*                        Image Functions                         */
/* -------------------------------------------------------------- */

int fl_load_bmp(fl_context* context, const char* path, fl_image* img)
{
	SDL_Surface* surface;
	fl_texture* texture;

	/* Load a bmp file. */
	surface = SDL_LoadBMP(path);

	/* Verify that the bmp file loaded. */
	if (surface == NULL)
		return 0;

	/* Ignore the color with an RGB value of 255, 0, 255. */
	SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 255, 0, 255));

	/* Convert the SDL surface into a texture. */
	texture = SDL_CreateTextureFromSurface(context->renderer, surface);

	/* Verify texture creation. */
	if (texture == NULL)
	{
		SDL_FreeSurface(surface);
		return 0;
	}

	/* Populate the image structure. */
	img->w = surface->w;
	img->h = surface->h;
	img->texture = texture;

	/* Dispose of the surface. */
	SDL_FreeSurface(surface);

	return 1;
}

void fl_destroy_image(fl_image* image)
{
	if (image == NULL)
		return;

	if (image->texture != NULL)
		SDL_DestroyTexture(image->texture);

	fl_free(image);
}



/* -------------------------------------------------------------- */
/*                        Text Functions                          */
/* -------------------------------------------------------------- */

fl_ttf* fl_load_ttf(const char* path, int p)
{
	return TTF_OpenFont(path, p);
}

void fl_close_ttf(fl_ttf* font)
{
	if (font == NULL)
		return;

	TTF_CloseFont(font);
}

fl_image* fl_create_glyph_image(fl_context* context, fl_resource* res, char c)
{
	fl_ttf* font;
	fl_color fc;
	fl_color bc;
	SDL_Surface* surface;
	fl_texture* texture;
	fl_image* image;

	/* Extract the font data from the resource. */
	font = res->impl.font->impl;
	fc = res->impl.font->forecolor;
	bc = res->impl.font->backcolor;

	/* Create an SDL surface representing the character. */
	if (res->impl.font->background)
		surface = TTF_RenderGlyph_Shaded(font, (Uint16)c, fc, bc);
	else
		surface = TTF_RenderGlyph_Blended(font, (Uint16)c, fc);

	/* Verify surface creation. */
	if (surface == NULL)
		return NULL;

	/* Convert the SDL surface into a texture. */
	texture = SDL_CreateTextureFromSurface(context->renderer, surface);

	/* Verify texture creation. */
	if (texture == NULL)
	{
		SDL_FreeSurface(surface);
		return NULL;
	}

	/* Allocate memory for an image structure. */
	image = fl_alloc(fl_image, 1);

	/* Verify image memory allocation. */
	if (image == NULL)
	{
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
		return NULL;
	}

	/* Populate the image structure. */
	image->w = surface->w;
	image->h = surface->h;
	image->texture = texture;

	/* Dispose of the surface. */
	SDL_FreeSurface(surface);

	return image;
}

fl_image* fl_create_text_image(fl_context* context, fl_resource* res, const char* str)
{
	fl_ttf* font;
	fl_color fc;
	fl_color bc;
	SDL_Surface* surface;
	fl_texture* texture;
	fl_image* image;

	/* Extract the font data from the resource. */
	font = res->impl.font->impl;
	fc = res->impl.font->forecolor;
	bc = res->impl.font->backcolor;

	/* Create an SDL surface representing the character. */
	if (res->impl.font->background)
		surface = TTF_RenderText_Shaded(font, str, fc, bc);
	else
		surface = TTF_RenderText_Blended(font, str, fc);

	/* Verify surface creation. */
	if (surface == NULL)
		return NULL;

	/* Convert the SDL surface into a texture. */
	texture = SDL_CreateTextureFromSurface(context->renderer, surface);

	/* Verify texture creation. */
	if (texture == NULL)
	{
		SDL_FreeSurface(surface);
		return NULL;
	}

	/* Allocate memory for an image structure. */
	image = fl_alloc(fl_image, 1);

	/* Verify image memory allocation. */
	if (image == NULL)
	{
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
		return NULL;
	}

	/* Populate the image structure. */
	image->w = surface->w;
	image->h = surface->h;
	image->texture = texture;

	/* Dispose of the surface. */
	SDL_FreeSurface(surface);

	return image;
}


/* -------------------------------------------------------------- */
/*                      Rendering Functions                       */
/* -------------------------------------------------------------- */

void fl_set_draw_color(fl_context* context, int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(context->renderer, r, g, b, a);
}

void fl_draw_rect(fl_context* context, fl_rect* r)
{
	SDL_RenderDrawRect(context->renderer, r);
}

void fl_draw_solid_rect(fl_context* context, fl_rect* r)
{
	SDL_RenderFillRect(context->renderer, r);
}

void fl_draw_line(fl_context* context, int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(context->renderer, x1, y1, x2, y2);
}

void fl_draw(fl_context* context, fl_texture* tex, fl_rect* src, fl_rect* dest, int flip)
{
	if (flip)
	{
		SDL_RenderCopyEx(context->renderer, tex, src, dest, 0, NULL, SDL_FLIP_HORIZONTAL);
	}
	else
	{
		SDL_RenderCopyEx(context->renderer, tex, src, dest, 0, NULL, SDL_FLIP_NONE);
	}
}

void fl_render_clear(fl_context* context)
{
	SDL_RenderClear(context->renderer);
}

void fl_render_show(fl_context* context)
{
	SDL_RenderPresent(context->renderer);
}



/* -------------------------------------------------------------- */
/*                    flurmp.h implementation                     */
/* -------------------------------------------------------------- */

void fl_sleep(int ms)
{
	SDL_Delay(ms);
}

void fl_handle_events(fl_context* context)
{
	while (SDL_PollEvent(&(context->event)))
	{
		/* This happens when the user closes the window. */
		if (context->event.type == FLURMP_QUIT)
			context->done = 1;
	}
}

void fl_begin_frame(fl_context* context)
{
	context->ticks = SDL_GetTicks();
}

void fl_end_frame(fl_context* context)
{
	if (1000U / context->fps > SDL_GetTicks() - context->ticks)
	{
		SDL_Delay(1000U / context->fps - (SDL_GetTicks() - context->ticks));
	}
}

int fl_initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO)) return 0;

	if (TTF_Init())
	{
		SDL_Quit();
		return 0;
	}

	return 1;
}

void fl_terminate()
{
	TTF_Quit();
	SDL_Quit();
}
