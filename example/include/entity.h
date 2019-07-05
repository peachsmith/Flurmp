#ifndef FLURMP_ENTITY_H
#define FLURMP_ENTITY_H

#include "flurmp_impl.h"

/* total number of entity types available to the application */
#define FLURMP_ENTITY_TYPE_COUNT 3

/* indices for font registry */
#define FLURMP_ENTITY_PLAYER 0
#define FLURMP_ENTITY_SIGN 1
#define FLURMP_ENTITY_BLOCK_200_50 2

/* entity state flags */
#define FLURMP_JUMP_FLAG     0x01
#define FLURMP_LEFT_FLAG     0x02
#define FLURMP_INTERACT_FLAG 0x04
#define FLURMP_FLAG_4        0x08
#define FLURMP_FLAG_5        0x10
#define FLURMP_FLAG_6        0x20
#define FLURMP_FLAG_7        0x40
#define FLURMP_FLAG_8        0x80

#endif