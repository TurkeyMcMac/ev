#include "tile.h"

#include "organism.h"
#include "pixel.h"

#include <stdlib.h>

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

enum TileTag TILE_SEED_pick(TILE_SEED self) {
	unsigned char rn = (unsigned char)rand();

	if (rn <= self.organism) return Tile_ORGANISM;

	unsigned char sum = self.organism;

	if (rn <= (sum += self.food)) return Tile_FOOD;

	if (rn <= (sum += self.rock)) return Tile_ROCK;

	return Tile_EMPTY;
}

void Tile_draw(struct Tile* self, FILE* dest) {
	PIXEL_draw(self->look, dest);
}

