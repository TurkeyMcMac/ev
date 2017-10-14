#include "organism.h"
#include "tile.h"
#include "world.h"

#include <stdlib.h>
#include <time.h>

const size_t WORLD_WIDTH = 200;
const size_t WORLD_HEIGHT = 50;

const TILE_SEED SPAWN_SEED = {
	.organism = 1,
	.food = 10,
	.rock = 2
};

const size_t NN_INPUT_NUM = 33;
const size_t NN_LAYER_NUM = 3;
const size_t NN_LAYERS[NN_LAYER_NUM] = {16, 8, ORGANISM_OUTPUT_NUM};

const struct WorldConfig WORLD_CONFIG = {
	.nutrition = 90,
	.fullness = 1000,
	.fullness_threshold_max = 500,
	.start_mutation = 1.0,
	.mutation = 0.01,
	.mutation_chance = 10,
	.nn_input_num = NN_INPUT_NUM,
	.nn_layers = (size_t*)&NN_LAYERS,
	.nn_layer_num = NN_LAYER_NUM
};

const size_t TARGET_POPULATION = 30;

const unsigned long TICKS_PER_FRAME = 100;
const struct timespec FRAME_DELAY = {
	.tv_sec = 0,
	.tv_nsec = 80000000
};

int main() {
	srand(time(NULL));

	struct World w = World_random(
		WORLD_WIDTH,
		WORLD_HEIGHT,
		SPAWN_SEED,
		WORLD_CONFIG
	);

	unsigned long tick = 0;
	while (1) {
		World_update(&w);
		World_reseed(&w, TARGET_POPULATION);
		World_add_food(&w, 1);

		if (++tick % TICKS_PER_FRAME == 0) {
			World_draw(&w, stdout);
			printf("\n\n\n");

			nanosleep(&FRAME_DELAY, NULL);
		}
	}
}
