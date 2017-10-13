#ifndef _WORLD_H // Whole file

#define _WORLD_H

#include "tile.h"

#include <stddef.h>
#include <stdio.h>

struct World {
	size_t width;
	size_t height;
	struct Tile* tiles;
	unsigned int fullness;
	unsigned int fullness_threshold_max;
	float start_mutation;
	size_t nn_input_num;
	size_t* nn_layers;
	size_t nn_layer_num;
	size_t alive_counter;
	float mutation;
};

struct WorldConfig {
	unsigned int nutrition;
	unsigned int fullness;
	unsigned int fullness_threshold_max;
	float start_mutation;
	size_t nn_input_num;
	size_t* nn_layers;
	size_t nn_layer_num;
	float mutation;
};

struct World World_random(
	size_t width,
	size_t height,
	TILE_SEED tile_seed,
	struct WorldConfig conf
);

void World_update(struct World* self);

void World_reseed(struct World* self, size_t target);

struct Tile* World_get_unchecked(struct World* self, size_t x, size_t y);

struct Tile* World_get(struct World* self, size_t x, size_t y);

struct Tile* World_get_empty(struct World* self);

size_t World_wrap_x_r(const struct World* self, size_t x, size_t x_off);

size_t World_wrap_x_l(const struct World* self, size_t x, size_t x_off);

size_t World_wrap_y_d(const struct World* self, size_t y, size_t y_off);

size_t World_wrap_y_u(const struct World* self, size_t y, size_t y_off);

void World_vicinity(struct World* self, size_t x, size_t y, char* dest);

void World_draw(const struct World* self, FILE* dest);

#endif
