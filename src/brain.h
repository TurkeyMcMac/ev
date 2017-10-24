#ifndef _BRAIN_H // Whole file

#define _BRAIN_H

#define float_vary(amount) ((float)rand() / RAND_MAX * amount * 2.0 - amount)

#include <stddef.h>

typedef float* BRAIN_WEIGHTS;

struct Brain {
	BRAIN_WEIGHTS weights;
	size_t weight_num;
	size_t input_num;
	size_t* layers;
	size_t layer_num;
};

struct Brain Brain_new(size_t input_num, size_t* layers, size_t layer_num);

void Brain_link(struct Brain* self, BRAIN_WEIGHTS weights);

BRAIN_WEIGHTS Brain_clone(const struct Brain* self);

BRAIN_WEIGHTS Brain_mutate(const struct Brain* self, float amount);

char* Brain_compute(const struct Brain* self, const char* input);

BRAIN_WEIGHTS random_weights(struct Brain* template, float range);

size_t Brain_input_num(struct Brain* self);

#endif
