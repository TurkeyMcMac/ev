#include "cli.h"

#include "organism.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char* HELP_INFO = "\
Usage: ev [-<option>[<value>]...]\n\
	-W<integer>   World width\n\
	-H<integer>   World height\n\
	-o<0-255>     Organism initial generation chance\n\
	-f<0-255>     Food initial generation chance\n\
	-r<0-255>     Rock initial generation chance\n\
	-L<integers>  Neural network hidden layers sizes,\n\
	              separated by single characters\n\
	-n<integer>   Food nutrition\n\
	-F<integer>   Organism starting fullness\n\
	-t<integer>   Start max fullness threshold\n\
	-m<float>     Mutation of initial creatures\n\
	-M<float>     Mutation over time\n\
	-c<integer>   Mutation chance\n\
	-p<integer>   Minimum population\n\
	-i<integer>   Number of world cycles between\n\
	              frames\n\
	-S<integer>   Seconds per frame\n\
	-s<integer>   Nanoseconds per frame\n\
	-h            Display this help information";


const int INVALID_ARGUMENT = 1;
const int SHOWED_HELP = 4319; // This looks vaguely like 'help'
const int OUT_OF_MEMORY = 14314150; // This looks a bit like 'mem is 0'

const size_t INITIAL_INT_LIST_SIZE = 1;
ssize_t parse_integer_list(char* src, size_t* dst, size_t cap) {
	char* end;
	ssize_t l;
	for (end = src, l = 0; *(end++) != '\0'; ++l) {
		if (cap < l && !(dst = realloc(dst, (cap *= 2) * sizeof(size_t))))
			return -1;

		dst[l] = (size_t)strtol(end, &end, 10);
	}

	size_t* final = &dst[l];

	if (!(dst = realloc(dst, ++l * sizeof(size_t)))) return -1;

	*final = ORGANISM_OUTPUT_NUM;

	return l;
}

int load_config_to(struct ProgConfig* conf, int argc, char** argv) {
	for (int i = 1; i < argc; ++i) {
		char* arg = argv[i];

		switch (arg[1]) {
			case 'W': // width
				conf->world_width = (size_t)strtol(&arg[2], NULL, 10);

				break;
			case 'H': // height
				conf->world_height = (size_t)strtol(&arg[2], NULL, 10);

				break;
			case 'o': // organism generation chance
				conf->seed.organism = (unsigned char)strtol(&arg[2], NULL, 10);

				break;
			case 'f': // food generation chance
				conf->seed.food = (unsigned char)strtol(&arg[2], NULL, 10);

				break;
			case 'r': // rock generation chance
				conf->seed.rock = (unsigned char)strtol(&arg[2], NULL, 10);

				break;
			case 'L': // nn layers
				conf->world.nn_layers = malloc(INITIAL_INT_LIST_SIZE * sizeof(size_t));

				ssize_t layer_num = parse_integer_list(&arg[1], conf->world.nn_layers, INITIAL_INT_LIST_SIZE);

				if (layer_num >= 0)
					conf->world.nn_layer_num = layer_num;
				else return OUT_OF_MEMORY;

				break;
			case 'n': // food nutrition
				conf->world.nutrition = (unsigned int)strtol(&arg[2], NULL, 10);

				break;
			case 'F': // start fullness
				conf->world.fullness = (unsigned int)strtol(&arg[2], NULL, 10);

				break;
			case 't': // max start fullness threshold
				conf->world.fullness_threshold_max = (unsigned int)strtol(&arg[2], NULL, 10);

				break;
			case 'm': // start mutation
				conf->world.start_mutation = strtof(&arg[2], NULL);

				break;
			case 'M': // mutation
				conf->world.mutation = strtof(&arg[2], NULL);

				break;
			case 'c': // mutation chance
				conf->world.mutation_chance = (unsigned int)strtol(&arg[2], NULL, 10);

				break;
			case 'p': // minimum population
				conf->minimum_population = (size_t)strtol(&arg[2], NULL, 10);

				break;
			case 'i': // number of world cycles between frames
				conf->ticks_per_frame = (unsigned long)strtol(&arg[2], NULL, 10);

				break;
			case 'S': // seconds per frame
				conf->frame_delay.tv_sec = (time_t)strtol(&arg[2], NULL, 10);

				break;
			case 's': // nanoseconds per frame
				conf->frame_delay.tv_nsec = strtol(&arg[2], NULL, 10);

				break;
			case 'h': // help
				fprintf(stderr, "%s\n", HELP_INFO);

				return SHOWED_HELP;
			default:
				fprintf(stderr, "Invalid argument: '%c'\n%s\n", arg[1], HELP_INFO);

				return INVALID_ARGUMENT;
		}
	}

	return 0;
}

