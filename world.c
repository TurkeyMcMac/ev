#include "world.h"

#include "tile.h"

#include <malloc.h>
#include <stdio.h>

struct World World_random(size_t width, size_t height, TILE_SEED tile_seed) {
	struct World w;
	w.width = width;
	w.height = height;
	w.tiles = malloc(width * height * sizeof(struct Tile));
	for (size_t i = 0; i < width * height; ++i) {
		switch (TILE_SEED_pick(tile_seed)) {
			case Tile_EMPTY:
				w.tiles[i] = Tile_empty();
				break;
			case Tile_ORGANISM: {
				struct Organism o;
				w.tiles[i] = Tile_organism(o);
				}
				break;
			case Tile_FOOD:
				w.tiles[i] = Tile_food(0);
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

