/**
 * A block of spikes.
 */
#ifndef FLURMP_SPIKE_H
#define	FLURMP_SPIKE_H

#include "flurmp.h"

 /**
  * Creates a spike entity.
  *
  * Params:
  *   x position
  *   y position
  *
  * Returns:
  *   fl_entity - a spike entity
  */
fl_entity* fl_create_spike(int, int);

/**
 * Registers the implementation of a spike entity.
 *
 * Params:
 *   fl_context - a Flurmp context
 *   fl_entity_type - the entity type of a spike
 */
void fl_register_spike_type(fl_context* context, fl_entity_type*);

#endif
