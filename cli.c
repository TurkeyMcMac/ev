#include "cli.h"

#include "organism.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char* DESCRIPTION = "Start the evolution simulator.";
const char* HELP_INSTRUCTIONS = "\
Usage: ev [-<option>[<value>]...]\n\
Options:\n\
	-W<integer>	World width\n\
	-H<integer>	World height\n\
	-o<0-255>	Organism initial generation chance\n\
	-f<0-255>	Food initial generation chance\n\
	-r<0-255>	Rock initial generation chance\n\
	-L<integers>	Neural network hidden layers sizes,\n\
			separated by single characters\n\
	-n<integer>	Food nutrition\n\
	-F<integer>	Organism starting fullness\n\
	-t<integer>	Start max fullness threshold\n\
	-m<float>	Mutation of initial creatures\n\
	-M<float>	Mutation over time\n\
	-c<integer>	Mutation chance\n\
	-N<integer>	Food added per world cycle\n\
	-p<integer>	Minimum population\n\
	-l<integer>	Lifetime of creatures\n\
	-i<integer>	Number of world cycles between\n\
			frames\n\
	-S<integer>	Seconds per frame\n\
	-s<integer>	Nanoseconds per frame\n\
	-(h | ?)	Display this help information";


const int INVALID_ARGUMENT = 1;
const int SHOWED_HELP = 4319; // This looks vaguely like 'help'
const int OUT_OF_MEMORY = 14314150; // This looks a bit like 'mem is 0'

const size_t INITIAL_INT_LIST_SIZE = 4;
ssize_t parse_integer_list(char* src, size_t* dst, size_t cap) {
	// The end of the last item in the list
	char* end;
	// The length of the list
	ssize_t l;
	//                     Increment end pointer to skip separator character
	for (end = src, l = 0; *(end++) != '\0'; ++l) {
		// Reallocate if capacity is too low
		if (cap < l && !(dst = realloc(dst, (cap *= 2) * sizeof(size_t))))
			return -1;

		// Parse this item to base 10, setting end to the end of the digits
		dst[l] = (size_t)strtol(end, &end, 10);
	}

	// Find the (currently out-of-bounds) end of the list
	size_t* final = &dst[l];

	// Make room for one more, casting off unnecessary capacity in the process
	if (!(dst = realloc(dst, ++l * sizeof(size_t)))) return -1;

	// Set the now in-bounds final element to the required number of outputs
	*final = NN_OUTPUT_NUM;

	return l;
}

#define _INIT_OPTS(init_conf, arg) 							\
case 'W': /* width */									\
	init_conf.world_width = (size_t)strtol(&arg[1], NULL, 10);			\
											\
	break;										\
case 'H': /* height */									\
	init_conf.world_height = (size_t)strtol(&arg[1], NULL, 10);			\
											\
	break;										\
case 'o': /* organism generation chance */						\
	init_conf.seed.organism = (unsigned char)strtol(&arg[1], NULL, 10);		\
											\
	break;										\
case 'f': /* food generation chance */							\
	init_conf.seed.food = (unsigned char)strtol(&arg[1], NULL, 10);			\
											\
	break;										\
case 'r': /* rock generation chance */							\
	init_conf.seed.rock = (unsigned char)strtol(&arg[1], NULL, 10);			\
											\
	break;										\
case 'L': /* nn layers */ {								\
	size_t* nn_layers = malloc(INITIAL_INT_LIST_SIZE * sizeof(size_t));		\
											\
	ssize_t layer_num = parse_integer_list(arg, nn_layers, INITIAL_INT_LIST_SIZE);	\
											\
	if (layer_num < 0) return OUT_OF_MEMORY;					\
											\
	init_conf.world.nn_layers = nn_layers;						\
	init_conf.world.nn_layer_num = layer_num;					\
											\
	break;										\
}											\
case 'n': /* food nutrition */								\
	init_conf.world.nutrition = (unsigned)strtol(&arg[1], NULL, 10);		\
											\
	break;										\
case 'F': /* start fullness */								\
	init_conf.world.fullness = (unsigned)strtol(&arg[1], NULL, 10);			\
											\
	break;										\
case 't': /* max start fullness threshold */						\
	init_conf.world.fullness_threshold_max = (unsigned)strtol(&arg[1], NULL, 10);	\
											\
	break;										\
case 'm': /* start mutation */								\
	init_conf.world.start_mutation = strtof(&arg[1], NULL);				\
											\
	break;										\
case 'M': /* mutation */								\
	init_conf.world.mutation = strtof(&arg[1], NULL);				\
											\
	break;										\
case 'c': /* mutation chance */								\
	init_conf.world.mutation_chance = (unsigned)strtol(&arg[1], NULL, 10);		\
											\
	break;										\
case 'l': /* organism lifetime */							\
	init_conf.world.lifetime = (unsigned)strtol(&arg[1], NULL, 10);			\
											\
	break;

#define _RUNTIME_OPTS(runtime_conf, arg)						\
case 'N': /* food per tick */								\
											\
	runtime_conf.food_per_tick = (size_t)strtol(&arg[1], NULL, 10);			\
											\
	break;										\
case 'p': /* minimum population */							\
	runtime_conf.minimum_population = (size_t)strtol(&arg[1], NULL, 10);		\
											\
	break;										\
case 'i': /* number of world cycles between frames */					\
	runtime_conf.ticks_per_frame = (unsigned long)strtol(&arg[1], NULL, 10);	\
											\
	break;										\
case 'S': /* seconds per frame */							\
	runtime_conf.frame_delay.tv_sec = (time_t)strtol(&arg[1], NULL, 10);		\
											\
	break;										\
case 's': /* nanoseconds per frame */							\
	runtime_conf.frame_delay.tv_nsec = strtol(&arg[1], NULL, 10);			\
											\
	break;

int parse_arg(struct ProgConfig* conf, char* arg) {
	switch (*arg) {
		_INIT_OPTS(conf->init, arg)
		_RUNTIME_OPTS(conf->runtime, arg)
		case 'h': // help
		case '?':
			fprintf(stderr, "%s\n%s\n", DESCRIPTION, HELP_INSTRUCTIONS);

			return SHOWED_HELP;
		default:
			fprintf(stderr, "Invalid argument: '%c'\n%s\n", *arg, HELP_INSTRUCTIONS);

			return INVALID_ARGUMENT;
	}

	return 0;
}

int load_config_to(struct ProgConfig* conf, int argc, char** argv) {
	for (int i = 1; i < argc; ++i) {
		char* arg = argv[i];

		int err = parse_arg(conf, &arg[1]);
		if (err) return err;
	}

	return 0;
}

int edit_config(struct ProgConfig* conf, char* cmd) {
	switch (*cmd) {
		_RUNTIME_OPTS(conf->runtime, cmd)
		default:
			return INVALID_ARGUMENT;
	}

	return 0;
}

void* monitor_input(void* _args) {
	struct MonitorArgs* args = _args;
	
	int taking_input = 0;

	char* cmd = NULL;
	size_t cmd_len = 0;

	while (1) {
		if (getline(&cmd, &cmd_len, stdin) > 1) {
			if (taking_input) edit_config(args->config, cmd);
		} else
			if (taking_input) {
				taking_input = 0;

				pthread_mutex_unlock(args->monitor_flag);
			} else {
				taking_input = 1;

				pthread_mutex_lock(args->monitor_flag);
			}
	}

	return NULL;

}
