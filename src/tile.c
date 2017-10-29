#include "tile.h"

#include "organism.h"
#include "pixel.h"

#include <stdlib.h>

struct Tile Tile_empty() {
	struct Tile t;
	t.tag = Tile_EMPTY;

	return t;
}

struct Tile Tile_rock() {
	struct Tile t;
	t.tag = Tile_ROCK;

	return t;
}

struct Tile Tile_food(unsigned nutrients) {
	struct Tile t;
	t.tag = Tile_FOOD;
	t.val.nutrients = nutrients;

	return t;
}

struct Tile Tile_organism(struct Organism org) {
	struct Tile t;
	t.tag = Tile_ORGANISM;
	t.val.org = org;

	return t;
}

int Tile_solid(const struct Tile* self) {
	return self->tag == Tile_ROCK || self->tag == Tile_ORGANISM;
}

void Tile_org_set(struct Tile* self, struct Organism org) {
	if (!Tile_solid(self)) {
		if (self->tag == Tile_FOOD)
			Organism_eat(&org, self->val.nutrients);

		*self = Tile_organism(org);
	}

}

enum TileTag TILE_SEED_pick(TILE_SEED self) {
	unsigned char rn = (unsigned char)rand();

	if (rn <= self.organism) return Tile_ORGANISM;

	unsigned char sum = self.organism;

	if (rn <= (sum += self.food)) return Tile_FOOD;

	if (rn <= (sum += self.rock)) return Tile_ROCK;

	return Tile_EMPTY;
}

int Tile_draw(const struct Tile* self, FILE* dest) {
	PIXEL p;
	switch (self->tag) {
		case Tile_EMPTY:
			p = TILE_EMPTY_LOOK;
			break;
		case Tile_ORGANISM:
			p = TILE_ORGANISM_LOOK;
			break;
		case Tile_FOOD:
			p = TILE_FOOD_LOOK;
			break;
		case Tile_ROCK:
			p = TILE_ROCK_LOOK;
	}

	return PIXEL_draw(p, dest);
}

