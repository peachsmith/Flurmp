/**
 * This file contains functions for loading and unloading scenes.
 * Giving each scnene loader its own source file may improve readability.
 */
#include "scene.h"

#include "input.h"
#include "entity.h"
#include "console.h"
#include "text.h"
#include "menu.h"
#include "dialog.h"
#include "resource.h"
#include "image.h"

#include "block_200_50.h"
#include "sign.h"
#include "player.h"

/**
 * Determines if an image resource is used often.
 * Some images may be used in multiple scenes.
 * If an image is potentially used in two or more scenes between which
 * the context is switching, there's no need to repeatedly unload and
 * reload that image.
 *
 * Params:
 *   int - the ID from the image registry
 *
 * Returns:
 *   int - 1 if the image is reused often, or 0 if it's not.
 */
static int is_common_image(int id);

/**
 * Loads the scene called "Test 1".
 * Test 1 is a scene that is used for developing and testing new mechanics.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
static void load_test_1(fl_context* context);




void fl_load_scene(fl_context* context, int id)
{
	switch (id)
	{
	case FLURMP_SCENE_TEST_1:
		load_test_1(context);
		break;

	default:
		break;
	}
}

void fl_clear_scene(fl_context* context)
{
	int i;
	fl_entity* current;
	fl_entity* next;

	/* Remove all entities. */
	current = context->entities;
	while (current != NULL)
	{
		next = current->next;
		fl_free(current);
		current = next;
	}
	context->entity_count = 0;
	context->entities = NULL;
	context->pco = NULL;

	/* Clear the texture pointers from entity types. */
	for (i = 0; i < FLURMP_ENTITY_TYPE_COUNT; i++)
	{
		if (context->entity_types[i].texture != NULL)
		{
			context->entity_types[i].texture = NULL;
		}
	}

	/* Unload image data. */
	for (i = 0; i < FLURMP_IMAGE_COUNT; i++)
	{
		/* Only unload images if they're not used between
		   scenes very often. */
		if (context->images[i] != NULL && !is_common_image(i))
		{
			fl_destroy_resource(context->images[i]);
			context->images[i] = NULL;
		}
	}

	/* Clear the scene field. */
	context->scene = FLURMP_SCENE_NONE;
}




static int is_common_image(int id)
{
	switch (id)
	{
	case FLURMP_IMAGE_PLAYER:
		return 1;
	default:
		return 0;
	}
}

static void load_test_1(fl_context* context)
{
	/* Set the camera position. */
	context->cam_x = 0;
	context->cam_y = 0;

	/* Load the image resources. */
	/* TODO: add resource loading check */
	context->images[FLURMP_IMAGE_SIGN] = fl_load_image(context, "resources/images/sign.bmp");
	context->images[FLURMP_IMAGE_BLOCK_200_50] = fl_load_image(context, "resources/images/block_200_50.bmp");

	/* Assign the image resources */
	context->entity_types[FLURMP_ENTITY_SIGN].texture = context->images[FLURMP_IMAGE_SIGN];
	context->entity_types[FLURMP_ENTITY_BLOCK_200_50].texture = context->images[FLURMP_IMAGE_BLOCK_200_50];

	/* Create a player entity. */
	fl_entity* player = fl_create_player(300, 100);
	
	/* Create a sign that will display a dialog. */
	fl_entity* sign = fl_create_sign(420, 260);

	/* Create terrain on which the player can walk. */
	fl_entity* block_1 = fl_create_block_200_50(280, 300);
	fl_entity* block_2 = fl_create_block_200_50(80, 350);
	fl_entity* block_3 = fl_create_block_200_50(480, 250);

	/* Add the entities to the context. */
	fl_add_entity(context, sign);
	fl_add_entity(context, player);
	fl_add_entity(context, block_1);
	fl_add_entity(context, block_2);
	fl_add_entity(context, block_3);

	/* Create the waiter for the player. */
	fl_load_player_waiters(context, player);

	/* Set the primary control object. */
	context->pco = player;

	/* Set the scene field. */
	context->scene = FLURMP_SCENE_TEST_1;
}
