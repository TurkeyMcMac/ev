#include "organism.h"
#include "tile.h"
#include "world.h"

#include <stdlib.h>
#include <time.h>

int main() {
	srand(time(NULL));
	TILE_SEED seed = {
		.empty    = 240,
		.organism = 1,
		.food = 10,
	};
	struct World w = World_random(100, 50, seed);

	World_draw(&w, stdout);
	printf("\n");
}
