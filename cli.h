#ifndef _CLI_H // Whole file

#define _CLI_H

#include "world.h"

#include <stddef.h>
#include <time.h>

struct ProgConfig {
	size_t world_width;
	size_t world_height;
	struct WorldConfig world;
	TILE_SEED seed;
	size_t food_per_tick;
	size_t minimum_population;
	long int ticks_per_frame;
	struct timespec frame_delay;

};

int load_config_to(struct ProgConfig* conf, int argc, char** argv);

#endif
