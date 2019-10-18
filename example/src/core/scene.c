/**
 * This file contains functions for loading and unloading scenes.
 * Giving each scnene loader its own source file may improve readability.
 */
#include "scene/scene.h"

#include "core/input.h"
#include "core/console.h"
#include "core/text.h"
#include "core/dialog.h"
#include "core/resource.h"
#include "core/image.h"
#include "core/schedule.h"

#include "menu/menu.h"

#include "entity/entity.h"
#include "entity/block_200_50.h"
#include "entity/sign.h"
#include "entity/player.h"
#include "entity/spike.h"
#include "entity/door.h"
#include "entity/pellet.h"

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

static int is_common_entity(int id);

/**
 * Loads the scene called "Test 1".
 * Test 1 is a scene that is used for developing and testing new mechanics.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
static void load_test_1(fl_context* context);

/**
 * Loads the scene called "Test 2".
 * This scene is meant for testing scene transitions.
 *
 * Params:
 *   fl_context - a Flurmp context
 */
static void load_test_2(fl_context* context);




void fl_load_scene(fl_context* context, int id)
{
	switch (id)
	{
	case FLURMP_SCENE_TEST_1:
		load_test_1(context);
		break;
	case FLURMP_SCENE_TEST_2:
		load_test_2(context);
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
		if (context->entity_types[i].texture != NULL && !is_common_entity(i))
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

	/* Destroy any schedules. */
	if (context->schedules != NULL)
	{
		fl_schedule* w = context->schedules;
		fl_schedule* next;

		while (w != NULL)
		{
			next = w->next;
			fl_destroy_schedule(w);
			w = next;
		}

		context->schedules = NULL;
	}

	/* Clear the scene field. */
	context->scene = FLURMP_SCENE_NONE;
}

void fl_schedule_scene_transition(fl_context* context, int from_scene, int to_scene)
{
	context->transition.from_scene = from_scene;
	context->transition.to_scene = to_scene;
	context->transition.scheduled = 1;
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

static int is_common_entity(int id)
{
	switch (id)
	{
	case FLURMP_ENTITY_PLAYER:
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
	context->images[FLURMP_IMAGE_SPIKE] = fl_load_image(context, "resources/images/spike.bmp");
	context->images[FLURMP_IMAGE_DOOR] = fl_load_image(context, "resources/images/door.bmp");
	context->images[FLURMP_IMAGE_PELLET] = fl_load_image(context, "resources/images/pellet.bmp");

	/* Assign the image resources */
	context->entity_types[FLURMP_ENTITY_SIGN].texture = context->images[FLURMP_IMAGE_SIGN];
	context->entity_types[FLURMP_ENTITY_BLOCK_200_50].texture = context->images[FLURMP_IMAGE_BLOCK_200_50];
	context->entity_types[FLURMP_ENTITY_SPIKE].texture = context->images[FLURMP_IMAGE_SPIKE];
	context->entity_types[FLURMP_ENTITY_DOOR].texture = context->images[FLURMP_IMAGE_DOOR];
	context->entity_types[FLURMP_ENTITY_PELLET].texture = context->images[FLURMP_IMAGE_PELLET];

	/* Create a player entity. */
	fl_entity* player = fl_create_player(300, 200);

	/* Create a sign that will display a dialog. */
	fl_entity* sign = fl_create_sign(420, 260);

	/* Create terrain on which the player can walk. */
	fl_entity* block_1 = fl_create_block_200_50(280, 300);
	fl_entity* block_2 = fl_create_block_200_50(80, 350);
	fl_entity* block_3 = fl_create_block_200_50(480, 250);
	fl_entity* spike = fl_create_spike(160, 330);
	fl_entity* door = fl_create_door(520, 210);

	fl_entity* pellet = fl_create_pellet(player->x, player->y);

	/* Add the entities to the context. */
	fl_add_entity(context, sign);
	fl_add_entity(context, door);
	fl_add_entity(context, player);
	fl_add_entity(context, block_1);
	fl_add_entity(context, block_2);
	fl_add_entity(context, block_3);
	fl_add_entity(context, spike);
	fl_add_entity(context, pellet);


	/* Create the schedule for the player. */
	fl_load_player_schedules(context, player);

	/* Set the primary control object. */
	context->pco = player;

	/* Set the start point of the projectile list */
	context->projectiles = pellet;

	/* Set the scene field. */
	context->scene = FLURMP_SCENE_TEST_1;
}

static void load_test_2(fl_context* context)
{
	/* Set the camera position. */
	context->cam_x = 0;
	context->cam_y = 0;

	/* Load the image resources. */
	/* TODO: add resource loading check */
	context->images[FLURMP_IMAGE_SIGN] = fl_load_image(context, "resources/images/sign.bmp");
	context->images[FLURMP_IMAGE_BLOCK_200_50] = fl_load_image(context, "resources/images/block_200_50.bmp");
	context->images[FLURMP_IMAGE_SPIKE] = fl_load_image(context, "resources/images/spike.bmp");
	context->images[FLURMP_IMAGE_DOOR] = fl_load_image(context, "resources/images/door.bmp");
	context->images[FLURMP_IMAGE_PELLET] = fl_load_image(context, "resources/images/pellet.bmp");

	/* Assign the image resources */
	context->entity_types[FLURMP_ENTITY_SIGN].texture = context->images[FLURMP_IMAGE_SIGN];
	context->entity_types[FLURMP_ENTITY_BLOCK_200_50].texture = context->images[FLURMP_IMAGE_BLOCK_200_50];
	context->entity_types[FLURMP_ENTITY_SPIKE].texture = context->images[FLURMP_IMAGE_SPIKE];
	context->entity_types[FLURMP_ENTITY_DOOR].texture = context->images[FLURMP_IMAGE_DOOR];
	context->entity_types[FLURMP_ENTITY_PELLET].texture = context->images[FLURMP_IMAGE_PELLET];

	/* Create a player entity. */
	fl_entity* player = fl_create_player(320, 200);

	/* Create terrain on which the player can walk. */
	fl_entity* block_1 = fl_create_block_200_50(100, 250);
	fl_entity* block_2 = fl_create_block_200_50(300, 300);
	fl_entity* block_3 = fl_create_block_200_50(500, 350);
	fl_entity* door = fl_create_door(460, 260);
	/*fl_entity* spike = fl_create_spike(160, 330);
	fl_entity* door = fl_create_door(520, 210);*/

	fl_entity* pellet = fl_create_pellet(player->x, player->y);

	/* Add the entities to the context. */
	fl_add_entity(context, door);
	fl_add_entity(context, player);
	fl_add_entity(context, block_1);
	fl_add_entity(context, block_2);
	fl_add_entity(context, block_3);
	fl_add_entity(context, pellet);


	/* Create the schedule for the player. */
	fl_load_player_schedules(context, player);

	/* Set the primary control object. */
	context->pco = player;

	/* Set the start point of the projectile list */
	context->projectiles = pellet;

	/* Set the scene field. */
	context->scene = FLURMP_SCENE_TEST_2;
}
