#include "organism.h"

#include "brain.h"

#include <malloc.h>
#include <stddef.h>

struct Organism Organism_new(unsigned int fullness, unsigned int unhealth, struct Brain brain) {
	struct Organism o;
	o.fullness = fullness;
	o.unhealth = unhealth;
	o.brain = brain;

	return o;
}

void Organism_eat(struct Organism* self, unsigned int nutrients) {
	self->fullness += nutrients;

	if (self->unhealth > nutrients) self->unhealth -= nutrients;
	else self->unhealth = 0;
}

void Organism_tick(struct Organism* self) {
	if (self->fullness > self->unhealth)
		self->fullness -= self->unhealth++;
	else self->fullness = 0;
}

int Organism_dead(const struct Organism* self) {
	return self->fullness == 0;
}

void Organism_drop(struct Organism* self) {
	Brain_drop(&self->brain);
}

MOVE Organism_react(const struct Organism* self, char* tiles) {
	char* out = Brain_compute(&self->brain, tiles);

	for (size_t i = 0; i < ORGANISM_OUTPUT_NUM; ++i) {
		if (out[i]) {
			switch (i) {
				case 0:
					free(out);
					return MOVE_UP;
				case 1:
					free(out);
					return MOVE_DOWN;
				case 2:
					free(out);
					return MOVE_RIGHT;
				case 3:
					free(out);
					return MOVE_LEFT;
			}
		}
	}

	free(out);
	return MOVE_NOWHERE;
}

