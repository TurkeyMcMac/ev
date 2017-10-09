#ifndef _BRAIN_H

#define _BRAIN_H

#define float_vary(amount) ((float)rand() / RAND_MAX * amount * 2.0 - amount)

#include <stddef.h>

struct Neuron {
	float* weights;
	size_t weight_num;
	float threshold;
};

char Neuron_compute(struct Neuron* self, char* input);

struct Neuron Neuron_random(float amount, size_t weight_num, float threshold);

struct Neuron Neuron_mutate(struct Neuron* self, float amount);

struct Layer {
	struct Neuron* neurons;
	size_t neuron_num;
};

char* Layer_compute(struct Layer* self, char* input);

struct Layer Layer_random(float amount, size_t neuron_num, size_t weight_num);

struct Layer Layer_mutate(struct Layer* self, float amount);

// PUBLIC
struct Brain {
	struct Layer* layers;
	size_t layer_num;
};

// PUBLIC
char* Brain_compute(struct Brain* self, char* input);

// PUBLIC
struct Brain Brain_random(float range, size_t input_num, size_t* layers, size_t layer_num);

// PUBLIC
struct Brain Brain_mutate(struct Brain* self, float amount);

#endif
