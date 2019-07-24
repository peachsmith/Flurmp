#include "text.h"

fl_glyph* fl_create_glyph(fl_context* context, fl_resource* res, char c)
{
	/* glyph data */
	fl_glyph* glyph;
	fl_image* image;

	/* Create an image of the character. */
	image = fl_create_glyph_image(context, res, c);

	/* Verify image creation. */
	if (image == NULL)
		return NULL;

	/* Allocate memory for an fl_glyph structure. */
	glyph = fl_alloc(fl_glyph, 1);

	/* Verify fl_glyph memory allocation. */
	if (glyph == NULL)
	{
		fl_destroy_image(image);
		return NULL;
	}

	/* Populate the glyph structure. */
	glyph->c = c;
	glyph->image = image;

	return glyph;
}

void fl_destroy_glyph(fl_glyph* glyph)
{
	if (glyph == NULL)
		return;

	if (glyph->image != NULL)
		fl_destroy_image(glyph->image);

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
