#include "core/resource.h"
#include "core/text.h"

fl_resource* fl_load_image(fl_context* context, const char* path)
{
	fl_resource* resource;
	fl_image* image;

	image = fl_alloc(fl_image, 1);

	if (image == NULL)
		return NULL;

	if (!fl_load_bmp(context, path, image))
	{
		fl_free(image);
		return NULL;
	}

	resource = fl_alloc(fl_resource, 1);

	if (resource == NULL)
	{
		fl_destroy_image(image);
		return NULL;
	}

	resource->impl.image = image;
	resource->type = FLURMP_IMAGE_RESOURCE;

	return resource;
}

fl_resource* fl_load_font(const char* path, int p, fl_color fc, fl_color bc, int background)
{
	if (path == NULL)
		return NULL;

	fl_resource* resource;
	fl_font* font;
	fl_ttf* impl;

	resource = fl_alloc(fl_resource, 1);

	if (resource == NULL)
		return NULL;

	impl = fl_load_ttf(path, p);

	if (impl == NULL)
	{
		fl_free(resource);
		return NULL;
	}

	font = fl_alloc(fl_font, 1);

	if (font == NULL)
	{
		fl_free(resource);
		fl_close_ttf(impl);
		return NULL;
	}

	font->impl = impl;
	font->count = 0;
	font->glyphs = NULL;
	font->forecolor = fc;
	font->backcolor = bc;
	font->background = background;

	resource->impl.font = font;
	resource->type = FLURMP_FONT_RESOURCE;

	return resource;
}

void fl_destroy_resource(fl_resource* resource)
{
	if (resource == NULL)
		return;

	switch (resource->type)
	{
	case FLURMP_FONT_RESOURCE:
	{
		if (resource->impl.font != NULL)
		{
			if (resource->impl.font->impl != NULL)
				fl_close_ttf(resource->impl.font->impl);

			if (resource->impl.font->glyphs != NULL)
			{
				int i;
				for (i = 0; i < resource->impl.font->count; i++)
					fl_destroy_image(resource->impl.font->glyphs[i]);

				fl_free(resource->impl.font->glyphs);
			}

			fl_free(resource->impl.font);
		}
	}
	break;

	case FLURMP_IMAGE_RESOURCE:
	{
		if (resource->impl.image != NULL)
			fl_destroy_image(resource->impl.image);
	}
	break;

	default:
		break;
	}

	fl_free(resource);
}
