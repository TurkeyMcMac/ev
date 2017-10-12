#include "organism.h"
#include "tile.h"
#include "world.h"

#include <stdlib.h>
#include <time.h>

const TILE_SEED SPAWN_SEED = {
	.organism = 1,
	.food = 10,
	.rock = 5
};

const size_t NN_INPUT_NUM = 32;
const size_t NN_LAYER_NUM = 3;
const size_t NN_LAYERS[NN_LAYER_NUM] = {16, 8, 4};

const size_t WORLD_WIDTH = 200;
const size_t WORLD_HEIGHT = 50;

const unsigned int FOOD_START_NUTRITION = 100;
const unsigned int ORG_START_FULLNESS = 1000;
const unsigned int ORG_START_UNHEALTH = 0;
const float START_MUTATION_AMOUNT = 1.0;

const struct timespec TICK_DELAY = {
	.tv_sec = 0,
	.tv_nsec = 100000000
};

const size_t TARGET_POPULATION = 30;

int main() {
	srand(time(NULL));

	struct World w = World_random(
		WORLD_WIDTH,
		WORLD_HEIGHT,
		SPAWN_SEED,
		FOOD_START_NUTRITION,
		ORG_START_FULLNESS,
		ORG_START_UNHEALTH,
		START_MUTATION_AMOUNT,
		NN_INPUT_NUM,
		(size_t*)&NN_LAYERS,
		NN_LAYER_NUM);

	while (1) {
		World_update(&w);
		World_reseed(&w, TARGET_POPULATION);
		*World_get_empty(&w) = Tile_food(FOOD_START_NUTRITION);

		World_draw(&w, stdout);
		printf("\n\n\n");

		nanosleep(&TICK_DELAY, NULL);
	}
}
