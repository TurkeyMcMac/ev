#include "brain.h"

#include <stdlib.h>
#include <string.h>

const float NEURON_THRESHOLD = 0.0;

size_t max_size_t(const size_t* list, size_t len) {
	size_t max = *list;
	for (size_t i = 1; i < len; ++i)
		if (list[i] > max) max = list[i];

	return max;
}

struct Brain Brain_new(size_t input_num, size_t* layers, size_t layer_num) {
	struct Brain b;
	b.input_num = input_num;
	b.layers = layers;
	b.layer_num = layer_num;
	b.weight_num = 0;

	size_t last_layer_size = input_num;
	for (size_t i = 0; i < layer_num; ++i) {
		b.weight_num += last_layer_size * layers[i];

		last_layer_size = layers[i];
	}

	b.max_layer_size = max_size_t(b.layers, b.layer_num);

	return b;
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
