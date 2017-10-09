#ifndef _BRAIN_H

#define _BRAIN_H

#define float_vary(amount) ((float)rand() / RAND_MAX * amount * 2.0 - amount)

#include <stddef.h>

struct Brain {
	struct Layer* layers;
	size_t layer_num;
};

char* Brain_compute(struct Brain* self, char* input);

struct Brain Brain_random(float range, size_t input_num, size_t* layers, size_t layer_num);

struct Brain Brain_mutate(struct Brain* self, float amount);

#endif
