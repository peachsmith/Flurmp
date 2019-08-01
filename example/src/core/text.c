#include "core/text.h"

fl_image* fl_create_glyph(fl_context* context, fl_resource* res, char c)
{
	/* glyph data */
	fl_image* image;

	/* Create an image of the character. */
	image = fl_create_glyph_image(context, res, c);

	/* Verify image creation. */
	if (image == NULL)
		return NULL;

	return image;
}

int fl_create_font_atlas(fl_context* context, fl_resource* res)
{
	if (res == NULL)
	{
		context->error = FLURMP_ERR_FONTS;
		return 0;
	}

	int i, j; /* index variables */

	/* We are using the character range 32 to 126 of the ASCII chart
	   as printable characters. This is why we allocate space for
	   95 glyphs. This range starts with the space character ' '
	   and ends with the tilde character '~'. */
	res->impl.font->glyphs = fl_alloc(fl_image*, 95);

	/* Verify glyph memory allocation. */
	if (res->impl.font->glyphs == NULL)
	{
		context->error = FLURMP_ERR_FONTS;
		return 0;
	}

	/* Create the glyphs and add them to the atlas. */
	for (i = 32; i < 127; i++)
	{
		fl_image* glyph = fl_create_glyph(context, res, (char)i);

		if (glyph == NULL)
		{
			for (j = 0; j < res->impl.font->count; i++)
				fl_destroy_image(res->impl.font->glyphs[i]);

			fl_free(res->impl.font->glyphs);
			res->impl.font->glyphs = NULL;
			res->impl.font->count = 0;

			context->error = FLURMP_ERR_FONTS;
			return 0;
		}

		res->impl.font->glyphs[res->impl.font->count++] = glyph;
	}

	return 1;
}

fl_image* fl_char_to_glyph(fl_font* font, char c)
{
	if (font == NULL || font->glyphs == NULL || font->count == 0)
		return NULL;

	int i = (int)c;

	/* If the character c is within the range of printable characters,
	   return the glyph of that character,
	   otherwise return a space glyph. */
	return (i >= 32 && i <= 126) ? font->glyphs[i - 32] : font->glyphs[0];
}

fl_image* fl_create_static_text(fl_context* context, fl_resource* res, const char* txt)
{
	/* static text data */
	char* contents;
	fl_image* image;

	if (txt == NULL)
		return NULL;

	/* Allocate memory for the character string contents. */
	contents = fl_alloc(char, (strlen(txt) + 1));

	/* Verify character string allocation. */
	if (contents == NULL)
		return NULL;

	/* Populate the character buffer with the specified text. */
	strcpy(contents, txt);

	/* Create an image of the text. */
	image = fl_create_text_image(context, res, contents);

	/* Dispose of the string. */
	fl_free(contents);

	return image;
}
