#include "brain.h"

#include <stdlib.h>

struct Neuron {
	float* weights;
	size_t weight_num;
	float threshold;
};

char Neuron_compute(struct Neuron* self, char* input) {
	float sum = 0.0;

	for (size_t i = 0; i < self->weight_num; ++i)
		if (input[i])
			sum += self->weights[i];

	return sum > self->threshold;
}

struct Neuron Neuron_mutate(struct Neuron* self, float amount) {
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

struct Layer {
	struct Neuron* neurons;
	size_t neuron_num;
};

char* Layer_compute(struct Layer* self, char* input) {
	char* output = malloc(self->neuron_num);

	for (size_t i = 0; i < self->neuron_num; ++i)
		output[i] = Neuron_compute(&self->neurons[i], input);

	return output;
}

struct Layer Layer_mutate(struct Layer* self, float amount) {
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

char* Brain_compute(struct Brain* self, char* input) {
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

struct Brain Brain_mutate(struct Brain* self, float amount) {
	struct Brain b;
	b.layers = malloc(self->layer_num * sizeof(struct Layer));
	for (size_t i = 0; i < self->layer_num; ++i)
		b.layers[i] = Layer_mutate(&self->layers[i], amount);

	b.layer_num = self->layer_num;

	return b;
}

struct Brain Brain_random(float amount, size_t input_num, size_t* layers, size_t layer_num) {
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

/*
#include <stdio.h>
#include <time.h>

int main() {
	srand(time(NULL));
	
	size_t layers[3] = {5, 6, 5};

	struct Brain b_before = Brain_random(1.0, 5, (size_t*)&layers, 3);	
	struct Brain b = Brain_mutate(&b_before, 1.0);
	
	char input[5] = {'\0', '\1', '\0', '\1', '\0'};

	char* output = Brain_compute(&b, (char*)&input);

	for (size_t i = 0; i < 5; ++i)
		printf("%d\n", output[i]);
}*/
