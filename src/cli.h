#ifndef _CLI_H // Whole file

#define _CLI_H

#include "organism.h"
#include "tile.h"
#include "world.h"

#include <pthread.h>
#include <stddef.h>
#include <time.h>

#define PROG_CONFIG_DEFAULT (struct ProgConfig) {		\
	.init = (struct InitConfig) {				\
		.world_width = 200,				\
		.world_height = 50,				\
		.world = (struct WorldConfig) {			\
			.nutrition = 90,			\
			.fullness = 1000,			\
			.fullness_threshold_max = 500,		\
			.start_mutation = 1.0,			\
			.mutation = 0.01,			\
			.mutation_chance = 10,			\
			.brain = (struct Brain) {		\
				.input_num = NN_INPUT_NUM,	\
				.layers = NN_LAYERS,		\
				.layer_num = NN_LAYER_NUM	\
			},					\
			.lifetime = 2000			\
		},						\
		.seed = (TILE_SEED) {				\
			.organism = 1,				\
			.food = 10,				\
			.rock = 2				\
		}						\
	},							\
	.runtime = (struct RuntimeConfig) {			\
		.food_per_tick = 1,				\
		.minimum_population = 30,			\
		.ticks_per_frame = 100,				\
		.frame_delay = (struct timespec) {		\
			.tv_sec = 0,				\
			.tv_nsec = 80000000			\
		}						\
	}							\
}

struct InitConfig {
	size_t world_width;
	size_t world_height;
	struct WorldConfig world;
	TILE_SEED seed;
};

struct RuntimeConfig {
	size_t food_per_tick;
	size_t minimum_population;
	long int ticks_per_frame;
	struct timespec frame_delay;
};

struct ProgConfig {
	struct InitConfig init;
	struct RuntimeConfig runtime;
};

int load_config_to(struct ProgConfig* conf, int argc, char** argv);

int edit_config(struct ProgConfig* conf, char* cmd);

struct MonitorArgs {
	struct ProgConfig* config;
	pthread_mutex_t* monitor_flag;
};

void* monitor_input(void* args);

#endif
