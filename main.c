#include "organism.h"
#include "tile.h"
#include "world.h"

#include <stdio.h>

int main() {
	struct World w = World_empty(10, 10);
	*World_get(&w, 2, 3) = Tile_rock();
	*World_get(&w, 4, 7) = Tile_food(10);
	struct Organism o;
	*World_get(&w, 8, 1) = Tile_organism(o);

	World_draw(&w, stdout);
	printf("\n");
}
