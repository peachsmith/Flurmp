#ifndef FLURMP_ENTITY_H
#define FLURMP_ENTITY_H

#include "core/flurmp_impl.h"

/* total number of entity types available to the application */
#define FLURMP_ENTITY_TYPE_COUNT 3

/* indices for entity type registry */
#define FLURMP_ENTITY_PLAYER 0
#define FLURMP_ENTITY_SIGN 1
#define FLURMP_ENTITY_BLOCK_200_50 2

/* entity state flags */
#define FLURMP_ALIVE_FLAG    0x0001
#define FLURMP_LEFT_FLAG     0x0002
#define FLURMP_RIGHT_FLAG    0x0004
#define FLURMP_JUMP_FLAG     0x0008
#define FLURMP_AIR_FLAG      0x0010
#define FLURMP_INTERACT_FLAG 0x0020
#define FLURMP_CUTSCENE_FLAG 0x0040
#define FLURMP_DAMAGE_FLAG   0x0080
#define FLURMP_MIRROR_FLAG   0x0100
#define FLURMP_FLAG_10       0x0200
#define FLURMP_FLAG_11       0x0400
#define FLURMP_FLAG_12       0x0800
#define FLURMP_FLAG_13       0x1000
#define FLURMP_FLAG_14       0x2000
#define FLURMP_FLAG_15       0x4000
#define FLURMP_FLAG_16       0x8000

#endif
