#ifndef _WORLD_H // Whole file

#define _WORLD_H

#include "tile.h"

#include <stddef.h>
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

void World_update(struct World* self);

struct Tile* World_get_unchecked(struct World* self, size_t x, size_t y);

struct Tile* World_get(struct World* self, size_t x, size_t y);

size_t World_wrap_x_r(struct World* self, size_t x, size_t x_off);

size_t World_wrap_x_l(struct World* self, size_t x, size_t x_off);

size_t World_wrap_y_d(struct World* self, size_t y, size_t y_off);

size_t World_wrap_y_u(struct World* self, size_t y, size_t y_off);

void World_vicinity(struct World* self, size_t x, size_t y, char* dest);

void World_draw(struct World* self, FILE* dest);

#endif
