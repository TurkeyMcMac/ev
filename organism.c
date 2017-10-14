#include "organism.h"

#include "brain.h"

#include <stdlib.h>

struct Organism Organism_new(unsigned int fullness, unsigned int fullness_threshold, struct Brain brain) {
	struct Organism o;
	o.fullness = fullness;
	o.fullness_threshold = fullness_threshold;
	o.brain = brain;

	return o;
}

void Organism_eat(struct Organism* self, unsigned int nutrients) {
	self->fullness += nutrients;
}

void Organism_tick(struct Organism* self) {
	if (self->fullness) --self->fullness;
}

int Organism_dead(const struct Organism* self) {
	return self->fullness == 0;
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

struct Organism Organism_baby(struct Organism* self, float mutation, unsigned char mutation_chance) {
	if ((unsigned char)rand() > mutation_chance)
		return Organism_new(self->fullness, self->fullness_threshold, Brain_clone(&self->brain));
	else
		return Organism_new(
			self->fullness /= 2,
			rand() % 2 == 0 ?
				self->fullness_threshold + 1:
				self->fullness_threshold - 1,
			Brain_mutate(&self->brain, mutation)
		);
}
