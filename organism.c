#include "organism.h"

#include <stddef.h>

void Organism_eat(struct Organism* self, unsigned int nutrients) {
	self->fullness += nutrients;

	if (self->unhealth > 1) --(self->unhealth);
}

void Organism_tick(struct Organism* self) {
	self->fullness -= self->unhealth;
}

char Organism_dead(struct Organism* self) {
	return self->fullness <= 0;
}

MOVE Organism_react(struct Organism* self, char* tiles) {
	char* out = Brain_compute(self->brain, tiles);

	for (size_t i = 0; i < 4; ++i) {
		if (out[i]) {
			switch (i) {
				case 0:
					return MOVE_UP;
				case 1:
					return MOVE_DOWN;
				case 2:
					return MOVE_RIGHT;
				case 3:
					return MOVE_LEFT;
			}
		}
	}

	return MOVE_NOWHERE;
}

