#include "brain.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const float NEURON_THRESHOLD = 0.0;

void Brain_calc_weight_num(struct Brain* self) {
	self->weight_num = 0;

	size_t last_layer_size = self->input_num;
	for (size_t i = 0; i < self->layer_num; ++i) {
		self->weight_num += last_layer_size * self->layers[i];

		last_layer_size = self->layers[i];
	}
}

void Brain_calc_max_layer_size(struct Brain* self) {
	self->max_layer_size = self->layers[0];
	for (size_t i = 1; i < self->layer_num; ++i)
		if (self->layers[i] > self->max_layer_size)
			self->max_layer_size = self->layers[i];
}

void Brain_link(struct Brain* self, BRAIN_WEIGHTS weights) {
	self->weights = weights;
}

BRAIN_WEIGHTS Brain_clone(const struct Brain* self) {
	return memcpy(malloc(self->weight_num * sizeof(float)), self->weights, self->weight_num);
}

BRAIN_WEIGHTS Brain_mutate(const struct Brain* self, float amount) {
	BRAIN_WEIGHTS weights = malloc(self->weight_num * sizeof(float));

	for (size_t i = 0; i < self->weight_num; ++i)
		weights[i] = self->weights[i] + float_vary(amount);

	return weights;
}

void layer_compute(float* weights, size_t neuron_num, size_t weights_per_neuron, const char* input, char* output);

char* Brain_compute(const struct Brain* self, const char* input) {
	char* last_layer_out = malloc(self->input_num > self->max_layer_size ?
		self->input_num :
		self->max_layer_size
	);
	memcpy(last_layer_out, input, self->input_num);
	size_t last_layer_size = self->input_num;

	char* layer_out = malloc(self->max_layer_size);

	size_t layer_index = 0;
	for (size_t i = 0; i < self->layer_num; ++i) {
		layer_compute(
			&self->weights[layer_index],
			self->layers[i],
			last_layer_size,
			last_layer_out,
			layer_out
		);
		layer_index += self->layers[i] * last_layer_size;

		memcpy(last_layer_out, layer_out, self->layers[i]);
		last_layer_size = self->layers[i];
	}

	free(layer_out);

	return last_layer_out;
}

BRAIN_WEIGHTS random_weights(struct Brain* template, float range) {
	BRAIN_WEIGHTS weights = malloc(template->weight_num * sizeof(float));

	for (size_t i = 0; i < template->weight_num; ++i)
		weights[i] = float_vary(range);

	return weights;
}

char neuron_compute(float* weights, size_t weight_num, const char* input) {
	float sum = 0.0;

	for (size_t i = 0; i < weight_num; ++i)
		if (input[i]) sum += weights[i];

	return sum > NEURON_THRESHOLD;
}

void layer_compute(float* weights, size_t neuron_num, size_t weights_per_neuron, const char* input, char* output) {
	for (size_t i = 0; i < neuron_num; ++i)
		output[i] = neuron_compute(&weights[i * weights_per_neuron], weights_per_neuron, input);
}

size_t Brain_input_num(struct Brain* self) {
	return self->input_num;
}

                // Must be linked
int Brain_write(const struct Brain* self, FILE* dst) {
	return fwrite(self->weights, sizeof(float), self->weight_num, dst);
}
