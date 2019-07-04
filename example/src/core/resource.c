#include "resource.h"
#include "text.h"

fl_resource* fl_load_bmp(fl_context* context, const char* path)
{
	fl_resource* resource;
	SDL_Surface* surface;
	SDL_Texture* texture;

	surface = SDL_LoadBMP(path);

	if (surface == NULL)
		return NULL;

	SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 255, 0, 255));
	texture = SDL_CreateTextureFromSurface(context->renderer, surface);
	SDL_FreeSurface(surface);

	if (texture == NULL)
		return NULL;

	resource = (fl_resource*)malloc(sizeof(fl_resource));

	if (resource == NULL)
	{
		SDL_DestroyTexture(texture);
		return NULL;
	}

	resource->impl.texture = texture;
	resource->type = FLURMP_IMAGE_RESOURCE;

	return resource;
}

fl_resource* fl_load_font(const char* path, int p, fl_color fc, fl_color bc, int background)
{
	if (path == NULL)
		return NULL;

	fl_resource* resource;
	fl_font* font;
	TTF_Font* impl;

	resource = (fl_resource*)malloc(sizeof(fl_resource));

	if (resource == NULL)
		return NULL;

	impl = TTF_OpenFont(path, p);

	if (impl == NULL)
	{
		free(resource);
		return NULL;
	}

	font = (fl_font*)malloc(sizeof(fl_font));

	if (font == NULL)
	{
		free(resource);
		TTF_CloseFont(impl);
		return NULL;
	}

	font->impl = impl;
	font->atlas = NULL;
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
				TTF_CloseFont(resource->impl.font->impl);

			if (resource->impl.font->atlas != NULL)
				fl_destroy_font_atlas(resource->impl.font->atlas);

			free(resource->impl.font);
		}

		free(resource);
	}
	break;

	case FLURMP_IMAGE_RESOURCE:
	{
		if (resource->impl.texture != NULL)
		{
			SDL_DestroyTexture(resource->impl.texture);
		}

		free(resource);
	}
	break;

	default:
		break;
	}
}
