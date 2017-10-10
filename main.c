#include "organism.h"
#include "tile.h"
#include "world.h"

#include <stdlib.h>
#include <time.h>

const unsigned char ORGANISM_SPAWN_CHANCE = 1;
const unsigned char FOOD_SPAWN_CHANCE = 10;
const unsigned char ROCK_SPAWN_CHANCE = 5;

const size_t NN_INPUT_NUM = 32;
const size_t NN_LAYER_NUM = 3;
const size_t NN_LAYERS[NN_LAYER_NUM] = {16, 8, 4};

const size_t WORLD_WIDTH = 100;
const size_t WORLD_HEIGHT = 50;

const unsigned int FOOD_START_NUTRITION = 10;
const unsigned int ORG_START_FULLNESS = 10;
const unsigned int ORG_START_UNHEALTH = 0;
const float START_MUTATION_AMOUNT = 1.0;

const unsigned int TICK_SLEEP_SECS = 0;
const unsigned int TICK_SLEEP_NANOS = 100000000;

int main() {
	srand(time(NULL));
	TILE_SEED seed = {
		.organism = ORGANISM_SPAWN_CHANCE,
		.food     = FOOD_SPAWN_CHANCE,
		.rock     = ROCK_SPAWN_CHANCE
	};
	struct World w = World_random(
		WORLD_WIDTH,
		WORLD_HEIGHT,
		seed,
		FOOD_START_NUTRITION,
		ORG_START_FULLNESS,
		ORG_START_UNHEALTH,
		START_MUTATION_AMOUNT,
		NN_INPUT_NUM,
		(size_t*)&NN_LAYERS,
		NN_LAYER_NUM);

	struct timespec tick_delay = {
		.tv_sec = TICK_SLEEP_SECS,
		.tv_nsec = TICK_SLEEP_NANOS,
	};

	while (1) {
		World_update(&w);

		World_draw(&w, stdout);
		printf("\n\n\n");

		nanosleep(&tick_delay, NULL);
	}
}
