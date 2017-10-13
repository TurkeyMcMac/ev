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
	struct Reaction r;

	tiles[32] = self->fullness > self->fullness_threshold;

	char* out = Brain_compute(&self->brain, tiles);

	r.baby = out[4];

	for (size_t i = 0; i < 4; ++i) {
		if (out[i]) {
			MOVE move;
			switch (i) {
				case 0:
					move = MOVE_UP;
					break;
				case 1:
					move = MOVE_DOWN;
					break;
				case 2:
					move = MOVE_RIGHT;
					break;
				case 3:
					move = MOVE_LEFT;
					break;
			}

			r.move = move;

			free(out);

			return r;
		}
	}

	r.move = MOVE_NOWHERE;

	free(out);

	return r;
}

struct Organism Organism_baby(struct Organism* self, float mutation) {
	return Organism_new(
		self->fullness /= 2,
		rand() % 2 == 0 ? // TODO: Make this dependent on mutation rate
			self->fullness_threshold + 1:
			self->fullness_threshold - 1,
		Brain_mutate(&self->brain, mutation));
}
