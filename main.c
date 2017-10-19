#include "cli.h"
#include "organism.h"
#include "tile.h"
#include "world.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const size_t NN_INPUT_NUM = 33;
const size_t NN_LAYER_NUM = 3;
const size_t NN_LAYERS[NN_LAYER_NUM] = {16, 8, ORGANISM_OUTPUT_NUM};

pthread_mutex_t CONFIG_MUTEX;

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
		.nn_layer_num = NN_LAYER_NUM,
		.lifetime = 2000
	},
	.seed = (TILE_SEED) {
		.organism = 1,
		.food = 10,
		.rock = 2
	},
	.food_per_tick = 1,
	.minimum_population = 30,
	.ticks_per_frame = 100,
	.frame_delay = (struct timespec) {
		.tv_sec = 0,
		.tv_nsec = 80000000
	}
};

void* monitor_input(void* args);

int main(int argc, char** argv) {
	int parse_failed = load_config_to(&CONFIG, argc, argv);
	if (parse_failed) return parse_failed;

	pthread_t input_monitor_id;
	pthread_create(&input_monitor_id, NULL, monitor_input, &CONFIG);

	srand(time(NULL));

	struct World w = World_random(
		CONFIG.world_width,
		CONFIG.world_height,
		CONFIG.seed,
		CONFIG.world
	);

	unsigned long tick = 0;
	while (1) {
		pthread_mutex_lock(&CONFIG_MUTEX);

		World_update(&w);
		World_reseed(&w, CONFIG.minimum_population);
		World_add_food(&w, CONFIG.food_per_tick);

		if (++tick % CONFIG.ticks_per_frame == 0) {
			World_draw(&w, stdout);
			printf("\nalive: %ld\n", World_alive_count(&w));

			pthread_mutex_unlock(&CONFIG_MUTEX);

			nanosleep(&CONFIG.frame_delay, NULL);
		} else pthread_mutex_unlock(&CONFIG_MUTEX);
	}
}

void* monitor_input(void* arg) {
	struct ProgConfig* conf = arg;

	int taking_input = 0;

	char* cmd = NULL;
	size_t cmd_len = 0;

	while (1) {
		if (getline(&cmd, &cmd_len, stdin) > 1) {
			if (taking_input) edit_config(conf, cmd);
		} else
			if (taking_input) {
				taking_input = 0;

				pthread_mutex_unlock(&CONFIG_MUTEX);
			} else {
				taking_input = 1;

				pthread_mutex_lock(&CONFIG_MUTEX);
			}
	}

	return NULL;
}
