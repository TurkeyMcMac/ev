#include "organism.h"

#include "brain.h"

#include <stdlib.h>

struct Organism Organism_new(unsigned fullness, unsigned fullness_threshold, unsigned ticks_left, struct Brain brain) {
	struct Organism o;
	o.fullness = fullness;
	o.fullness_threshold = fullness_threshold;
	o.ticks_left = ticks_left;
	o.brain = brain;

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
	Brain_drop(&self->brain);
}

struct Reaction Organism_react(const struct Organism* self, char* tiles) {
	#define _check_move_output(i, m) \
		do { if (out[i]) { r.move = m; goto end; } } while (0)

	struct Reaction r;

	tiles[32] = self->fullness > self->fullness_threshold;

	char* out = Brain_compute(&self->brain, tiles);

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

struct Organism Organism_baby(struct Organism* self, float mutation, unsigned char mutation_chance, unsigned ticks_left) {
	self->fullness /= 2;

	if ((unsigned char)rand() > mutation_chance) {
		return Organism_new(self->fullness, self->fullness_threshold, ticks_left, Brain_clone(&self->brain));
	} else
		return Organism_new(
			self->fullness,
			rand() % 2 == 0 ?
				self->fullness_threshold + 1:
				self->fullness_threshold - 1,
			ticks_left,
			Brain_mutate(&self->brain, mutation)
		);
}
