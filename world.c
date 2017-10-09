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

void World_update(struct World* self) {
	for (size_t y = 0; y < self->height; ++y)
		for (size_t x = 0; x < self->width; ++x) {
			struct Tile* tile = World_get_unchecked(self, x, y);

			if (tile->tag == Tile_ORGANISM) {
				char* input = calloc(32, sizeof(char));
				World_vicinity(self, x, y, input);

				struct Tile* dest;
				switch (Organism_react(&tile->val.org, input)) {
					case MOVE_UP:
						dest = World_get_unchecked(
							self,
							x,
							World_wrap_y_u(self, y, 1));
						break;
					case MOVE_DOWN:
						dest = World_get_unchecked(
							self,
							x,
							World_wrap_y_d(self, y, 1));
						break;
					case MOVE_RIGHT:
						dest = World_get_unchecked(
							self,
							World_wrap_x_r(self, x, 1),
							y);
						break;
					case MOVE_LEFT:
						dest = World_get_unchecked(
							self,
							World_wrap_x_l(self, x, 1),
							y);
						break;
					default: goto end;
				}

				dest->tag = Tile_ORGANISM;
				dest->val = tile->val;
				dest->look = TILE_ORGANISM_LOOK;

				tile->tag = Tile_EMPTY;
				tile->look = TILE_EMPTY_LOOK;

				end:
				free(input);
			}
		}
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

void encode_tile(struct Tile* tile, size_t i, char* dest) {
	switch (tile->tag) {
		case Tile_EMPTY:
			dest[i] = 1;
			break;
		case Tile_ORGANISM:
			dest[i + 8] = 1;
			break;
		case Tile_FOOD:
			dest[i + 16] = 1;
			break;
		case Tile_ROCK:
			dest[i + 24] = 1;
	}
}

void World_vicinity(struct World* self, size_t x, size_t y, char* dest) {
	size_t x_r = World_wrap_x_r(self, x, 1),
	       x_l = World_wrap_x_l(self, x, 1);

	size_t y_d = World_wrap_y_d(self, y, 1),
	       y_u = World_wrap_y_u(self, y, 1);

	encode_tile(World_get_unchecked(self, x_r, y_d), 0, dest);
	encode_tile(World_get_unchecked(self, x_r,   y), 1, dest);
	encode_tile(World_get_unchecked(self, x_r, y_u), 2, dest);
	encode_tile(World_get_unchecked(self, x,   y_u), 3, dest);
	encode_tile(World_get_unchecked(self, x_l, y_u), 4, dest);
	encode_tile(World_get_unchecked(self, x_l,   y), 5, dest);
	encode_tile(World_get_unchecked(self, x_l, y_d), 6, dest);
	encode_tile(World_get_unchecked(self, x,   y_d), 7, dest);
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

