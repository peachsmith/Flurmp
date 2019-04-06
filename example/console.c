#include "console.h"

console_t* fl_create_console()
{
	console_t* con = (console_t*)malloc(sizeof(console_t));

	if (con == NULL)
		return NULL;

	con->x = 10;
	con->y = 350;
	con->w = 620;
	con->h = 120;

	return con;
}

void fl_destroy_console(console_t* console)
{
	if (console == NULL)
		return;

	free(console);

	return;
}

void fl_render_console(fl_context* context, console_t* console)
{
	SDL_SetRenderDrawColor(context->renderer, 150, 50, 150, 120);

	SDL_Rect r;
	r.x = console->x;
	r.y = console->y;
	r.w = console->w;
	r.h = console->h;

	SDL_RenderFillRect(context->renderer, &r);

	SDL_SetRenderDrawColor(context->renderer, 240, 240, 240, 255);
	SDL_RenderDrawRect(context->renderer, &r);
}

void fl_print(console_t* console, const char* s)
{

}
