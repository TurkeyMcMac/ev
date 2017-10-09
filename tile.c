#include "tile.h"

#include "organism.h"
#include "pixel.h"

#include <stdio.h>

struct Tile Tile_empty() {
	struct Tile t;
	t.tag = Tile_EMPTY;
	t.look = TILE_EMPTY_LOOK;

	return t;
}

struct Tile Tile_rock() {
	struct Tile t;
	t.tag = Tile_ROCK;
	t.look = TILE_ROCK_LOOK;

	return t;
}

struct Tile Tile_food(unsigned int nutrients) {
	struct Tile t;
	t.tag = Tile_FOOD;
	t.val.nutrients = nutrients;
	t.look = TILE_FOOD_LOOK;

	return t;
}

struct Tile Tile_organism(struct Organism org) {
	struct Tile t;
	t.tag = Tile_ORGANISM;
	t.val.org = org;
	t.look = TILE_ORGANISM_LOOK;

	return t;
}

void Tile_draw(struct Tile* self, FILE* dest) {
	PIXEL_draw(self->look, dest);
}

