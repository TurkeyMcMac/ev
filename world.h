#ifndef _WORLD_H // Whole file

#define _WORLD_H

#include <stdio.h>

struct World {
	size_t width;
	size_t height;
	struct Tile* tiles;
};

struct World World_empty(size_t width, size_t height);

struct Tile* World_get_unchecked(struct World* self, size_t x, size_t y);

struct Tile* World_get(struct World* self, size_t x, size_t y);

void World_draw(struct World* self, FILE* dest);

#endif
