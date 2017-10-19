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
	*final = ORGANISM_OUTPUT_NUM;

	return l;
}

int parse_arg(struct ProgConfig* conf, char* arg) {
	switch (*arg) {
		case 'W': // width
			conf->world_width = (size_t)strtol(&arg[1], NULL, 10);

			break;
		case 'H': // height
			conf->world_height = (size_t)strtol(&arg[1], NULL, 10);

			break;
		case 'o': // organism generation chance
			conf->seed.organism = (unsigned char)strtol(&arg[1], NULL, 10);

			break;
		case 'f': // food generation chance
			conf->seed.food = (unsigned char)strtol(&arg[1], NULL, 10);

			break;
		case 'r': // rock generation chance
			conf->seed.rock = (unsigned char)strtol(&arg[1], NULL, 10);

			break;
		case 'L': // nn layers
			conf->world.nn_layers = malloc(INITIAL_INT_LIST_SIZE * sizeof(size_t));

			ssize_t layer_num = parse_integer_list(arg, conf->world.nn_layers, INITIAL_INT_LIST_SIZE);

			if (layer_num >= 0)
				conf->world.nn_layer_num = layer_num;
			else return OUT_OF_MEMORY;

			break;
		case 'n': // food nutrition
			conf->world.nutrition = (unsigned int)strtol(&arg[1], NULL, 10);

			break;
		case 'F': // start fullness
			conf->world.fullness = (unsigned int)strtol(&arg[1], NULL, 10);

			break;
		case 't': // max start fullness threshold
			conf->world.fullness_threshold_max = (unsigned int)strtol(&arg[1], NULL, 10);

			break;
		case 'm': // start mutation
			conf->world.start_mutation = strtof(&arg[1], NULL);

			break;
		case 'M': // mutation
			conf->world.mutation = strtof(&arg[1], NULL);

			break;
		case 'c': // mutation chance
			conf->world.mutation_chance = (unsigned int)strtol(&arg[1], NULL, 10);

			break;
		case 'N': // food per tick
			conf->food_per_tick = (size_t)strtol(&arg[1], NULL, 10);

			break;
		case 'p': // minimum population
			conf->minimum_population = (size_t)strtol(&arg[1], NULL, 10);

			break;
		case 'l':
			conf->world.lifetime = (unsigned int)strtol(&arg[1], NULL, 10);

			break;
		case 'i': // number of world cycles between frames
			conf->ticks_per_frame = (unsigned long)strtol(&arg[1], NULL, 10);

			break;
		case 'S': // seconds per frame
			conf->frame_delay.tv_sec = (time_t)strtol(&arg[1], NULL, 10);

			break;
		case 's': // nanoseconds per frame
			conf->frame_delay.tv_nsec = strtol(&arg[1], NULL, 10);

			break;
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
		case 'N': // food per tick
			conf->food_per_tick = (size_t)strtol(&cmd[1], NULL, 10);

			break;
		case 'p': // minimum population
			conf->minimum_population = (size_t)strtol(&cmd[1], NULL, 10);

			break;
		case 'i': // number of world cycles between frames
			conf->ticks_per_frame = (unsigned long)strtol(&cmd[1], NULL, 10);

			break;
		case 'S': // seconds per frame
			conf->frame_delay.tv_sec = (time_t)strtol(&cmd[1], NULL, 10);

			break;
		case 's': // nanoseconds per frame
			conf->frame_delay.tv_nsec = strtol(&cmd[1], NULL, 10);

			break;
		default:
			return INVALID_ARGUMENT;
	}

	return 0;
}
