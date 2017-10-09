#include "world.h"

#include "brain.h"
#include "organism.h"
#include "tile.h"

#include <malloc.h>
#include <stdio.h>

struct World World_random(
	size_t width,
	size_t height,
	TILE_SEED tile_seed,
	unsigned int nutrition,
	unsigned int fullness,
	unsigned int unhealth,
	float mutation,
	size_t nn_input_num,
	size_t* nn_layers,
	size_t nn_layer_num
) {
	struct World w;
	w.width = width;
	w.height = height;
	w.tiles = malloc(width * height * sizeof(struct Tile));
	for (size_t i = 0; i < width * height; ++i) {
		switch (TILE_SEED_pick(tile_seed)) {
			case Tile_EMPTY:
				w.tiles[i] = Tile_empty();
				break;
			case Tile_ORGANISM:
				w.tiles[i] = Tile_organism(Organism_new(
					fullness,
					unhealth,
					Brain_random(
						mutation,
						nn_input_num,
						nn_layers,
						nn_layer_num
					)
				));
				break;
			case Tile_FOOD:
				w.tiles[i] = Tile_food(nutrition);
				break;
			case Tile_ROCK:
				w.tiles[i] = Tile_rock();
				break;
		}
	}

	return w;
}

struct Tile* World_get_unchecked(struct World* self, size_t x, size_t y) {
	return self->tiles + (y * self->width + x);
}

struct Tile* World_get(struct World* self, size_t x, size_t y) {
	if (x < self->width && y < self->height)
		return World_get_unchecked(self, x, y);
	else return NULL;
}

size_t World_wrap_x_r(struct World* self, size_t x, size_t x_off) {
	return (x + x_off) % self->width;
}

size_t World_wrap_x_l(struct World* self, size_t x, size_t x_off) {
	size_t ret_x = x;

	while (ret_x < x_off) ret_x += self->width;

	return ret_x - x_off;
}

size_t World_wrap_y_d(struct World* self, size_t y, size_t y_off) {
	return (y + y_off) % self->height;
}

size_t World_wrap_y_u(struct World* self, size_t y, size_t y_off) {
	size_t ret_y = y;

	while (ret_y < y_off) ret_y += self->height;

	return ret_y - y_off;
}

void World_draw(struct World* self, FILE* dest) {
	Tile_draw(self->tiles, dest);

	for (size_t i = 1; i < self->width * self->height; ++i) {
		if (i % self->width == 0)
			fprintf(dest, "\x1B[0m\n");

		Tile_draw(&self->tiles[i], dest);
	}

	fprintf(dest, "\x1B[0m");

	fflush(dest);
}

