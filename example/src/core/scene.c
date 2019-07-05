#include "scene.h"

#include "input.h"
#include "entity.h"
#include "console.h"
#include "text.h"
#include "menu.h"
#include "dialog.h"
#include "resource.h"

#include "block_200_50.h"
#include "sign.h"
#include "player.h"

static void load_basic(fl_context* context);

void fl_load_scene(fl_context* context, int id)
{
	switch (id)
	{
	case FLURMP_SCENE_BASIC:
		load_basic(context);
		break;

	default:
		break;
	}
}

void fl_clear_scene(fl_context* context)
{

}

static void load_basic(fl_context* context)
{
	/* Set the camera position. */
	context->cam_x = 0;
	context->cam_y = 0;

	/* Create a player entity. */
	fl_entity* player = fl_create_player(300, 100);

	/* Create a sign that will display a dialog. */
	fl_entity* sign = fl_create_sign(420, 260);

	/* Create a block on which the player can walk. */
	fl_entity* block_1 = fl_create_block_200_50(280, 300);

	/* Add the entities to the context. */
	fl_add_entity(context, sign);
	fl_add_entity(context, player);
	fl_add_entity(context, block_1);

	/* Set the primary control object. */
	context->pco = player;
}
