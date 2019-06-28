#include "text.h"

#include <string.h>

fl_font* fl_load_font(const char* path, int p)
{
	if (path == NULL)
		return NULL;

	fl_font* font;
	TTF_Font* impl;

	impl = TTF_OpenFont(path, p);

	if (impl == NULL)
		return NULL;

	font = (fl_font*)malloc(sizeof(fl_font));

	if (font == NULL)
	{
		TTF_CloseFont(impl);
		return NULL;
	}

	font->impl = impl;

	return font;
}

void fl_destroy_font(fl_font* font)
{
	if (font == NULL)
		return;

	if (font->impl != NULL)
		TTF_CloseFont(font->impl);

	free(font);
}

fl_glyph* fl_create_glyph(fl_context* context, fl_font* font, char c)
{
	fl_glyph* glyph;
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Color forecol;
	SDL_Color backcol;

	glyph = (fl_glyph*)malloc(sizeof(fl_glyph));

	if (glyph == NULL)
		return NULL;

	forecol.r = 250;
	forecol.g = 250;
	forecol.b = 250;
	forecol.a = 0;

	backcol.r = 150;
	backcol.g = 50;
	backcol.b = 150;
	backcol.a = 0;

	/* convert the text to an SDL Surface */
	surface = TTF_RenderGlyph_Blended(context->font->impl, (Uint16)c, forecol);// , backcol);

	/* verify surface creation */
	if (surface == NULL)
	{
		free(glyph);
		return NULL;
	}

	/* convert the SDL Surface to an SDL Texture */
	texture = SDL_CreateTextureFromSurface(context->renderer, surface);

	/* verify texture creation */
	if (texture == NULL)
	{
		SDL_FreeSurface(surface);
		free(glyph);
		return NULL;
	}

	glyph->c = c;
	glyph->surface = surface;
	glyph->texture = texture;

	return glyph;
}

void fl_destroy_glyph(fl_glyph* glyph)
{
	if (glyph == NULL)
		return;

	if (glyph->surface != NULL)
		SDL_FreeSurface(glyph->surface);

	if (glyph->texture != NULL)
		SDL_DestroyTexture(glyph->texture);

	free(glyph);
}

fl_font_atlas* fl_create_font_atlas(fl_context* context, fl_font* font)
{
	fl_font_atlas* atlas;

	atlas = (fl_font_atlas*)malloc(sizeof(fl_font_atlas));

	if (atlas == NULL)
		return NULL;

	atlas->count = 0;

	/* for now, the printable characters are 32 - 126 of the ASCII chart */

	fl_glyph** glyphs = (fl_glyph * *)malloc(sizeof(fl_glyph*) * 95);

	if (glyphs == NULL)
	{
		free(atlas);
		return NULL;
	}

	atlas->glyphs = glyphs;

	int i;
	for (i = 32; i < 127; i++)
	{
		fl_glyph* glyph = fl_create_glyph(context, font, (char)i);

		if (glyph == NULL)
		{
			fl_destroy_font_atlas(atlas);
			return NULL;
		}

		atlas->glyphs[atlas->count++] = glyph;
	}

	return atlas;
}

void fl_destroy_font_atlas(fl_font_atlas* atlas)
{
	if (atlas == NULL)
		return;

	if (atlas->glyphs != NULL)
	{
		int i;
		for (i = 0; i < atlas->count; i++)
		{
			fl_destroy_glyph(atlas->glyphs[i]);
		}

		free(atlas->glyphs);
	}

	free(atlas);
}

fl_glyph* fl_char_to_glyph(fl_font_atlas* atlas, char c)
{
	if (atlas == NULL || atlas->glyphs == NULL || atlas->count == 0)
		return NULL;

	int i = (int)c;

	/* default to a space character */
	return (i >= 32 && i <= 126) ? atlas->glyphs[i - 32] : atlas->glyphs[0];
}

fl_static_text* fl_create_static_text(fl_context* context, const char* txt, int x, int y, int p)
{
	/* TODO: allow the font to be specified somehow */

	if (txt == NULL)
		return NULL;

	fl_static_text* stat;
	char* contents;
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Color forecol;
	SDL_Color backcol;

	int renderstyle = TTF_STYLE_NORMAL;
	int outline = 0;
	int hinting = TTF_HINTING_MONO;
	int kerning = 1;

	stat = (fl_static_text*)malloc(sizeof(fl_static_text));

	if (stat == NULL)
		return NULL;

	contents = (char*)malloc(strlen(txt) + 1);

	/* verify character buffer allocation */
	if (contents == NULL)
	{
		free(stat);
		return NULL;
	}

	/* populate the character buffer with the specified text */
	strcpy(contents, txt);

	/* set the foreground and background colors */
	forecol.r = 0;
	forecol.g = 0;
	forecol.b = 0;
	forecol.a = 255;

	backcol.r = 120;
	backcol.g = 120;
	backcol.b = 120;
	backcol.a = 255;

	/* convert the text to an SDL Surface */
	surface = TTF_RenderText_Shaded(context->font->impl, contents, forecol, backcol);

	/* verify surface creation */
	if (surface == NULL)
	{
		free(contents);
		free(stat);
		return NULL;
	}

	/* convert the SDL Surface to an SDL Texture */
	texture = SDL_CreateTextureFromSurface(context->renderer, surface);

	/* verify texture creation */
	if (texture == NULL)
	{
		SDL_FreeSurface(surface);
		free(contents);
		free(stat);
		return NULL;
	}

	stat->font = context->font;
	stat->surface = surface;
	stat->texture = texture;
	stat->text = contents;
	stat->x = x;
	stat->y = y;

	return stat;
}

void fl_destroy_static_text(fl_static_text* stat)
{
	if (stat == NULL)
		return;

	if (stat->surface != NULL)
		SDL_FreeSurface(stat->surface);

	if (stat->texture != NULL)
		SDL_DestroyTexture(stat->texture);

	if (stat->text != NULL)
		free(stat->text);

	free(stat);
}
