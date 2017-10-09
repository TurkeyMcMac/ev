#ifndef _TILE_H // Whole file

#define _TILE_H

#include "organism.h"
#include "pixel.h"

#include <stddef.h>
#include <stdio.h>

#define TILE_EMPTY_LOOK pixel('_', color(BLUE, BLACK))
#define TILE_ROCK_LOOK pixel('#', color(BLACK, BLUE))
#define TILE_FOOD_LOOK pixel(';', color(YELLOW, GREEN))
#define TILE_ORGANISM_LOOK pixel('@', color(GREEN, MAGENTA))

struct Tile {
	enum {
		Tile_EMPTY,
		Tile_ORGANISM,
		Tile_FOOD,
		Tile_ROCK,
	} tag;
	union {
		struct Organism org;
		unsigned int nutrients;
	} val;
	PIXEL look;
};

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

#endif
