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

struct Tile Tile_empty();

struct Tile Tile_rock();

struct Tile Tile_food(unsigned int nutrients);

struct Tile Tile_organism(struct Organism org);

void Tile_draw(struct Tile* self, FILE* dest);

#endif
