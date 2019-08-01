#include "core/animation.h"

fl_animation* fl_create_animation(int f)
{
	fl_animation* a = fl_alloc(fl_animation, 1);

	if (a == NULL)
		return NULL;

	fl_rect* frames = fl_alloc(fl_rect, f);

	if (frames == NULL)
	{
		fl_free(a);
		return NULL;
	}

	a->frames = frames;
	a->frame_count = f;

	return a;
}

void fl_destroy_animation(fl_animation* a)
{
	if (a == NULL)
		return;

	if (a->frames != NULL)
		fl_free(a->frames);

	fl_free(a);
}
