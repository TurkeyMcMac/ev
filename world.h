#ifndef _WORLD_H // Whole file

#define _WORLD_H

#include "tile.h"

#include <stdio.h>

struct World {
	size_t width;
	size_t height;
	struct Tile* tiles;
};

struct World World_random(
	size_t width,
	size_t height,
	TILE_SEED tile_seed,
	unsigned int nutrition,
	unsigned int fullness,
	unsigned int unhealth,
	float mutation_amount,
	size_t nn_input_num,
	size_t* nn_layers,
	size_t nn_layer_num
);

struct Tile* World_get_unchecked(struct World* self, size_t x, size_t y);

struct Tile* World_get(struct World* self, size_t x, size_t y);

void World_draw(struct World* self, FILE* dest);

#endif
