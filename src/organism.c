#include "organism.h"

#include "brain.h"

#include <stdlib.h>

static size_t NN_LAYER_ARR[NN_LAYER_NUM] = {16, 8, NN_OUTPUT_NUM};
size_t* NN_LAYERS = (size_t*)&NN_LAYER_ARR;

struct Organism Organism_new(unsigned fullness, unsigned fullness_threshold, unsigned ticks_left, BRAIN_WEIGHTS weights) {
	struct Organism o;
	o.fullness = fullness;
	o.fullness_threshold = fullness_threshold;
	o.ticks_left = ticks_left;
	o.weights = weights;

	return o;
}

void Organism_eat(struct Organism* self, unsigned nutrients) {
	self->fullness += nutrients;
}

void Organism_tick(struct Organism* self) {
	if (self->fullness) --self->fullness;

	if (self->ticks_left) --self->ticks_left;
}

int Organism_dead(const struct Organism* self) {
	return self->fullness == 0 || self->ticks_left == 0;
}

void Organism_drop(struct Organism* self) {
	free(self->weights);
}
                                                         // Must be linked
struct Reaction Organism_react(const struct Organism* self, const struct Brain* brain, char* input) {
	#define _check_move_output(i, m) \
		do { if (out[i]) { r.move = m; goto end; } } while (0)

	struct Reaction r;

	input[32] = self->fullness > self->fullness_threshold;

	char* out = Brain_compute(brain, input);

	r.baby = out[4];

	_check_move_output(0, MOVE_UP);
	_check_move_output(1, MOVE_DOWN);
	_check_move_output(2, MOVE_RIGHT);
	_check_move_output(3, MOVE_LEFT);

	r.move = MOVE_NOWHERE;

	end:
	free(out);

	return r;
}
                                                  // Must be linked
struct Organism Organism_baby(struct Organism* self, struct Brain* brain, float mutation, unsigned char mutation_chance, unsigned ticks_left) {
	self->fullness /= 2;

	if ((unsigned char)rand() > mutation_chance) {
		return Organism_new(self->fullness, self->fullness_threshold, ticks_left, Brain_clone(brain));
	}

	return Organism_new(
		self->fullness,
		rand() % 2 == 0 ?
			self->fullness_threshold + 1:
			self->fullness_threshold - 1,
		ticks_left,
		Brain_mutate(brain, mutation)
	);
}

BRAIN_WEIGHTS Organism_weights(const struct Organism* self) {
	return self->weights;
}
