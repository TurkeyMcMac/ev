#ifndef _ORGANISM_H // Whole file

#define _ORGANISM_H

#define ORGANISM_OUTPUT_NUM 4

#include "brain.h"

#include <stddef.h>

struct Organism {
	unsigned int fullness;
	unsigned int unhealth;
	struct Brain brain;
};

struct Organism Organism_new(unsigned int fullness, unsigned int unhealth, struct Brain brain);

void Organism_eat(struct Organism* self, unsigned int nutrients);

void Organism_tick(struct Organism* self);

char Organism_dead(struct Organism* self);

typedef enum {
	MOVE_UP,
	MOVE_DOWN,
	MOVE_RIGHT,
	MOVE_LEFT,
	MOVE_NOWHERE
} MOVE;

MOVE Organism_react(char *tiles);

#endif
