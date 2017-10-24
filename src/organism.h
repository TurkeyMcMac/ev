#ifndef _ORGANISM_H // Whole file

#define _ORGANISM_H

#include "brain.h"

#include <stddef.h>

extern const size_t NN_INPUT_NUM;
extern const size_t NN_OUTPUT_NUM;
extern const size_t NN_LAYER_NUM;
extern size_t* NN_LAYERS;

struct Organism {
	unsigned fullness;
	unsigned fullness_threshold;
	unsigned ticks_left;
	BRAIN_WEIGHTS weights;
};

struct Organism Organism_new(unsigned fullness, unsigned fullness_threshold, unsigned ticks_left, BRAIN_WEIGHTS weights);

void Organism_eat(struct Organism* self, unsigned nutrients);

void Organism_tick(struct Organism* self);

int Organism_dead(const struct Organism* self);

void Organism_drop(struct Organism* self);

typedef enum {
	MOVE_UP,
	MOVE_DOWN,
	MOVE_RIGHT,
	MOVE_LEFT,
	MOVE_NOWHERE
} MOVE;

struct Reaction {
	MOVE move;
	char baby;
};
                                                         // Must be linked
struct Reaction Organism_react(const struct Organism* self, const struct Brain* brain, char* tiles);

                                                  // Must be linked
struct Organism Organism_baby(struct Organism* self, struct Brain* brain, float mutation, unsigned char mutation_chance, unsigned ticks_left);

BRAIN_WEIGHTS Organism_weights(const struct Organism* self);

#endif
