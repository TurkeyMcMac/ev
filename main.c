#include "cli.h"
#include "organism.h"
#include "tile.h"
#include "world.h"

#include <stdlib.h>
#include <time.h>

const size_t NN_INPUT_NUM = 33;
const size_t NN_LAYER_NUM = 3;
const size_t NN_LAYERS[NN_LAYER_NUM] = {16, 8, ORGANISM_OUTPUT_NUM};

struct ProgConfig CONFIG = {
	.world_width = 200,
	.world_height = 50,
	.world = (struct WorldConfig) {
		.nutrition = 90,
		.fullness = 1000,
		.fullness_threshold_max = 500,
		.start_mutation = 1.0,
		.mutation = 0.01,
		.mutation_chance = 10,
		.nn_input_num = NN_INPUT_NUM,
		.nn_layers = (size_t*)&NN_LAYERS,
		.nn_layer_num = NN_LAYER_NUM
	},
	.seed = (TILE_SEED) {
		.organism = 1,
		.food = 10,
		.rock = 2
	},
	.minimum_population = 30,
	.ticks_per_frame = 100,
	.frame_delay = (struct timespec) {
		.tv_sec = 0,
		.tv_nsec = 80000000
	}
};

int main(int argc, char** argv) {
	int parse_failed = load_config_to(&CONFIG, argc, argv);
	if (parse_failed) return parse_failed;

	srand(time(NULL));

	struct World w = World_random(
		CONFIG.world_width,
		CONFIG.world_height,
		CONFIG.seed,
		CONFIG.world
	);

	unsigned long tick = 0;
	while (1) {
		World_update(&w);
		World_reseed(&w, CONFIG.minimum_population);
		World_add_food(&w, 1);

		if (++tick % CONFIG.ticks_per_frame == 0) {
			World_draw(&w, stdout);
			printf("\nalive: %ld\n", World_alive_count(&w));

			nanosleep(&CONFIG.frame_delay, NULL);
		}
	}
}
