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

struct Tile Tile_food(unsigned int nutrients) {
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

int Tile_org_shift(struct Tile* self, struct Tile* dest) {
	if (!Tile_solid(dest)) {
		if (dest->tag == Tile_FOOD)
			Organism_eat(&self->val.org, dest->val.nutrients);

		dest->tag = self->tag;
		dest->val = self->val;

		self->tag = Tile_EMPTY;

		return 1;
	} else return 0;

}

enum TileTag TILE_SEED_pick(TILE_SEED self) {
	unsigned char rn = (unsigned char)rand();

	if (rn <= self.organism) return Tile_ORGANISM;

	unsigned char sum = self.organism;

	if (rn <= (sum += self.food)) return Tile_FOOD;

	if (rn <= (sum += self.rock)) return Tile_ROCK;

	return Tile_EMPTY;
}

void Tile_draw(const struct Tile* self, FILE* dest) {
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

	PIXEL_draw(p, dest);
}

