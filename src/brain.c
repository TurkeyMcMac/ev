#include "brain.h"

#include <string.h>
#include <stdlib.h>

struct Neuron {
	float* weights;
	size_t weight_num;
	float threshold;
};

char Neuron_compute(const struct Neuron* self, const char* input) {
	float sum = 0.0;

	for (size_t i = 0; i < self->weight_num; ++i)
		if (input[i])
			sum += self->weights[i];

	return sum > self->threshold;
}

struct Neuron Neuron_clone(const struct Neuron* self) {
	struct Neuron n;
	n.weights = malloc(self->weight_num * sizeof(float));
	memcpy(n.weights, self->weights, self->weight_num);
	n.weight_num = self->weight_num;
	n.threshold = self->threshold;

	return n;
}

struct Neuron Neuron_mutate(const struct Neuron* self, float amount) {
	struct Neuron n;
	n.weights = malloc(self->weight_num * sizeof(float));
	for (size_t i = 0; i < self->weight_num; ++i)
		n.weights[i] = self->weights[i] + float_vary(amount);

	n.weight_num = self->weight_num;
	n.threshold = self->threshold;

	return n;
}

struct Neuron Neuron_random(float amount, size_t weight_num, float threshold) {
	struct Neuron n;
	n.weights = malloc(weight_num * sizeof(float));
	for (size_t i = 0; i < weight_num; ++i)
		n.weights[i] = float_vary(amount);
	n.weight_num = weight_num;
	n.threshold = threshold;

	return n;
}

void Neuron_drop(struct Neuron* self) {
	free(self->weights);
}

struct Layer {
	struct Neuron* neurons;
	size_t neuron_num;
};

char* Layer_compute(const struct Layer* self, const char* input) {
	char* output = malloc(self->neuron_num);

	for (size_t i = 0; i < self->neuron_num; ++i)
		output[i] = Neuron_compute(&self->neurons[i], input);

	return output;
}

struct Layer Layer_clone(const struct Layer* self) {
	struct Layer l;
	l.neurons = malloc(self->neuron_num * sizeof(struct Neuron));
	for (size_t i = 0; i < self->neuron_num; ++i)
		l.neurons[i] = Neuron_clone(&self->neurons[i]);

	l.neuron_num = self->neuron_num;

	return l;
}

struct Layer Layer_mutate(const struct Layer* self, float amount) {
	struct Layer l;
	l.neurons = malloc(self->neuron_num * sizeof(struct Neuron));
	for (size_t i = 0; i < self->neuron_num; ++i)
		l.neurons[i] = Neuron_mutate(&self->neurons[i], amount);

	l.neuron_num = self->neuron_num;

	return l;
}

struct Layer Layer_random(float amount, size_t neuron_num, size_t weight_num) {
	struct Layer l;
	l.neurons = malloc(neuron_num * sizeof(struct Neuron));

	for (size_t i = 0; i < neuron_num; ++i)
		l.neurons[i] = Neuron_random(amount, weight_num, 0.0);

	l.neuron_num = neuron_num;

	return l;
}

void Layer_drop(struct Layer* self) {
	for (size_t i = 0; i < self->neuron_num; ++i)
		Neuron_drop(&self->neurons[i]);

	free(self->neurons);
}

char* Brain_compute(const struct Brain* self, const char* input) {
	char* last_output = Layer_compute(self->layers, input);
	size_t last_size = self->layers->neuron_num;

	for (size_t i = 1; i < self->layer_num; ++i) {
		char* old_last_output = last_output;

		last_output = Layer_compute(&self->layers[i], last_output);
		last_size = (&self->layers[i])->neuron_num;

		free(old_last_output);
	}

	return last_output;
}

struct Brain Brain_clone(const struct Brain* self) {
	struct Brain b;
	b.layers = malloc(self->layer_num * sizeof(struct Layer));
	for (size_t i = 0; i < self->layer_num; ++i)
		b.layers[i] = Layer_clone(&self->layers[i]);

	b.layer_num = self->layer_num;

	return b;
}

struct Brain Brain_mutate(const struct Brain* self, float amount) {
	struct Brain b;
	b.layers = malloc(self->layer_num * sizeof(struct Layer));
	for (size_t i = 0; i < self->layer_num; ++i)
		b.layers[i] = Layer_mutate(&self->layers[i], amount);

	b.layer_num = self->layer_num;

	return b;
}

struct Brain Brain_random(float amount, size_t input_num, const size_t* layers, size_t layer_num) {
	struct Brain b;

	b.layers = malloc(layer_num * sizeof(struct Layer));

	size_t last_layer_size = input_num;
	for (size_t i = 0; i < layer_num; ++i) {
		b.layers[i] = Layer_random(amount, layers[i], last_layer_size);
		last_layer_size = layers[i];
	}

	b.layer_num = layer_num;

	return b;
}

void Brain_drop(struct Brain* self) {
	for (size_t i = 0; i < self->layer_num; ++i)
		Layer_drop(&self->layers[i]);

	free(self->layers);
}