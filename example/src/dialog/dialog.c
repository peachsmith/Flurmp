#include "dialog.h"

fl_dialog* fl_create_dialog(fl_resource* font, int x, int y, int w, int h)
{
	fl_dialog* dialog = (fl_dialog*)malloc(sizeof(fl_dialog));

	if (dialog == NULL)
		return NULL;

	dialog->font = font;
	dialog->x = x;
	dialog->y = y;
	dialog->w = w;
	dialog->h = h;
	dialog->counter = 0;
	dialog->render = NULL;
	dialog->statements = NULL;
	dialog->statement_count = 0;
	dialog->current_statement = 0;
	dialog->counter = 0;
	dialog->open = 0;
	dialog->buffer = NULL;
	dialog->buffer_count = 0;
	dialog->input_handler = NULL;

	return dialog;
}

void fl_destroy_dialog(fl_dialog* dialog)
{
	if (dialog == NULL)
		return;

	if (dialog->buffer != NULL)
		free(dialog->buffer);

	free(dialog);
}
