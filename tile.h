#ifndef _TILE_H // Whole file

#define _TILE_H

#include "organism.h"
#include "pixel.h"

#include <stddef.h>
#include <stdio.h>

#define TILE_EMPTY_LOOK pixel('_', color(BLUE, BLACK))
#define TILE_ROCK_LOOK pixel('#', color(BLACK, BLUE))
#define TILE_FOOD_LOOK pixel(';', color(YELLOW, GREEN))
#define TILE_ORGANISM_LOOK pixel('J', color(CYAN, RED))

struct Tile {
	enum TileTag {
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

typedef struct {
	unsigned char organism;
	unsigned char food;
	unsigned char rock;
} TILE_SEED;

enum TileTag TILE_SEED_pick(TILE_SEED self);

void Tile_draw(struct Tile* self, FILE* dest);

#endif
