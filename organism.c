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

struct Reaction Organism_react(const struct Organism* self, char* tiles) {
	struct Reaction r;

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
	return Organism_new(self->fullness /= 2, 0, Brain_mutate(&self->brain, mutation));
}
