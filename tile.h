#ifndef _TILE_H // Whole file

#define _TILE_H

#include "organism.h"

#include <stddef.h>
#include <stdio.h>

#define TILE_EMPTY_LOOK pixel('.', color(BLUE, BLACK))
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
};

struct Tile Tile_empty();

struct Tile Tile_rock();

struct Tile Tile_food(unsigned int nutrients);

struct Tile Tile_organism(struct Organism org);

int Tile_solid(const struct Tile* self);

int Tile_org_shift(struct Tile* self, struct Tile* dest);

typedef struct {
	unsigned char organism;
	unsigned char food;
	unsigned char rock;
} TILE_SEED;

enum TileTag TILE_SEED_pick(TILE_SEED self);

void Tile_draw(const struct Tile* self, FILE* dest);

#endif
