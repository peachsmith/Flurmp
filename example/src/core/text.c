#include "text.h"

#include <string.h>

fl_glyph* fl_create_glyph(fl_context* context, fl_resource* res, char c)
{
	/* glyph data */
	fl_glyph* glyph;
	fl_image* image;
	SDL_Surface* surface;
	SDL_Texture* texture;

	/* font data */
	TTF_Font* font;
	SDL_Color fc; /* foreground color */
	SDL_Color bc; /* background color */

	/* Allocate memory for an fl_glyph structure. */
	glyph = fl_alloc(fl_glyph, 1);

	/* Verify fl_glyph memory allocation. */
	if (glyph == NULL)
		return NULL;

	/* Extract the font data from the resource. */
	font = res->impl.font->impl;
	fc = res->impl.font->forecolor;
	bc = res->impl.font->backcolor;

	/* Convert the text to an SDL_Surface. */
	if (res->impl.font->background)
		surface = TTF_RenderGlyph_Shaded(font, (Uint16)c, fc, bc);
	else
		surface = TTF_RenderGlyph_Blended(font, (Uint16)c, fc);

	/* Verify SDL_Surface creation. */
	if (surface == NULL)
	{
		fl_free(glyph);
		return NULL;
	}

	/* Create an SDL_Texture from the SDL_Surface. */
	texture = SDL_CreateTextureFromSurface(context->renderer, surface);

	/* Verify texture creation. */
	if (texture == NULL)
	{
		SDL_FreeSurface(surface);
		fl_free(glyph);
		return NULL;
	}

	/* Allocate memory for an fl_image structure. */
	image = fl_alloc(fl_image, 1);

	/* Verify fl_image memory allocation. */
	if (image == NULL)
	{
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
		fl_free(glyph);
		return NULL;
	}

	/* Populate the fl_image structure. */
	image->surface = surface;
	image->texture = texture;

	/* Populate the fl_glyph structure. */
	glyph->c = c;
	glyph->image = image;

	return glyph;
}

void fl_destroy_glyph(fl_glyph* glyph)
{
	if (glyph == NULL)
		return;

	if (glyph->image != NULL)
	{
		if (glyph->image->surface != NULL)
			SDL_FreeSurface(glyph->image->surface);

		if (glyph->image->texture != NULL)
			SDL_DestroyTexture(glyph->image->texture);

		fl_free(glyph->image);
	}

	fl_free(glyph);
}

fl_font_atlas* fl_create_font_atlas(fl_context* context, fl_resource* res)
{
	/* font atlas data */
	fl_font_atlas* atlas;
	fl_glyph** glyphs;

	int i; /* index variable */

	/* Allocate memory for the font atlas. */
	atlas = fl_alloc(fl_font_atlas, 1);

	/* Verify font atlas memory allocation. */
	if (atlas == NULL)
		return NULL;

	atlas->count = 0;

	/* We are using the character range 32 to 126 of the ASCII chart
	   as printable characters. This is why we allocate space for
	   95 glyphs. This range starts with the space character ' '
	   and ends with the tilde character '~'. */
	glyphs = fl_alloc(fl_glyph*, 95);

	/* Verify glyph memory allocation. */
	if (glyphs == NULL)
	{
		fl_free(atlas);
		return NULL;
	}

	atlas->glyphs = glyphs;

	/* Create the glyphs and add them to the atlas. */
	for (i = 32; i < 127; i++)
	{
		fl_glyph* glyph = fl_create_glyph(context, res, (char)i);

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
	int i;

	if (atlas == NULL)
		return;

	if (atlas->glyphs != NULL)
	{
		for (i = 0; i < atlas->count; i++)
			fl_destroy_glyph(atlas->glyphs[i]);

		fl_free(atlas->glyphs);
	}

	fl_free(atlas);
}

fl_glyph* fl_char_to_glyph(fl_font_atlas* atlas, char c)
{
	if (atlas == NULL || atlas->glyphs == NULL || atlas->count == 0)
		return NULL;

	int i = (int)c;

	/* If the character c is within the range of printable characters,
	   return the glyph of that character,
	   otherwise return a space glyph. */
	return (i >= 32 && i <= 126) ? atlas->glyphs[i - 32] : atlas->glyphs[0];
}

fl_image* fl_create_static_text(fl_context* context, fl_resource* res, const char* txt)
{
	/* static text data */
	char* contents;
	fl_image* image;
	SDL_Surface* surface;
	SDL_Texture* texture;

	/* font data */
	TTF_Font* font;
	SDL_Color fc;
	SDL_Color bc;

	if (txt == NULL)
		return NULL;

	int renderstyle = TTF_STYLE_NORMAL;
	int outline = 0;
	int hinting = TTF_HINTING_MONO;
	int kerning = 1;

	/* Allocate memory for the character string contents. */
	contents = fl_alloc(char, (strlen(txt) + 1));

	/* Verify character string allocation. */
	if (contents == NULL)
	{
		return NULL;
	}

	/* Populate the character buffer with the specified text. */
	strcpy(contents, txt);

	/* Extract the font data from the resource. */
	font = res->impl.font->impl;
	fc = res->impl.font->forecolor;
	bc = res->impl.font->backcolor;

	/* Convert the text to an SDL_Surface. */
	if (res->impl.font->background)
		surface = TTF_RenderText_Shaded(font, contents, fc, bc);
	else
		surface = TTF_RenderText_Blended(font, contents, fc);

	/* Verify SDL_Surface creation. */
	if (surface == NULL)
	{
		fl_free(contents);
		return NULL;
	}

	/* Convert the SDL_Surface to an SDL_Texture. */
	texture = SDL_CreateTextureFromSurface(context->renderer, surface);

	/* Verify texture creation. */
	if (texture == NULL)
	{
		SDL_FreeSurface(surface);
		fl_free(contents);
		return NULL;
	}

	/* Allocate memory for an fl_image structure. */
	image = fl_alloc(fl_image, 1);

	/* Verify fl_image memory allocation. */
	if (image == NULL)
	{
		SDL_FreeSurface(surface);
		fl_free(contents);
		SDL_DestroyTexture(texture);
		return NULL;
	}

	fl_free(contents);

	/* Populate the fl_image structure. */
	image->surface = surface;
	image->texture = texture;

	return image;
}

void fl_destroy_static_text(fl_image* stat)
{
	if (stat == NULL)
		return;

	if (stat->surface != NULL)
		SDL_FreeSurface(stat->surface);

	if (stat->texture != NULL)
		SDL_DestroyTexture(stat->texture);

	fl_free(stat);
}
