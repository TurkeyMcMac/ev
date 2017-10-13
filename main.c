#include "organism.h"
#include "tile.h"
#include "world.h"

#include <stdlib.h>
#include <time.h>

const TILE_SEED SPAWN_SEED = {
	.organism = 1,
	.food = 10,
	.rock = 10
};

const size_t NN_INPUT_NUM = 33;
const size_t NN_LAYER_NUM = 3;
const size_t NN_LAYERS[NN_LAYER_NUM] = {16, 8, ORGANISM_OUTPUT_NUM};

const size_t WORLD_WIDTH = 200;
const size_t WORLD_HEIGHT = 50;

const unsigned int FOOD_START_NUTRITION = 50;
const unsigned int ORG_START_FULLNESS = 1000;
const unsigned int ORG_FULLNESS_THRESHOLD_MAX = 500;
const float START_MUTATION_AMOUNT = 1.0;

const float MUTATION_AMOUNT = 0.01;

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
		ORG_FULLNESS_THRESHOLD_MAX,
		START_MUTATION_AMOUNT,
		NN_INPUT_NUM,
		(size_t*)&NN_LAYERS,
		NN_LAYER_NUM,
		MUTATION_AMOUNT
	);

	while (1) {
		World_update(&w);
		World_reseed(&w, TARGET_POPULATION);
		*World_get_empty(&w) = Tile_food(FOOD_START_NUTRITION);

		World_draw(&w, stdout);
		printf("\n\n\n");

		nanosleep(&TICK_DELAY, NULL);
	}
}
