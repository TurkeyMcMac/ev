#include "world.h"

#include "brain.h"
#include "organism.h"
#include "tile.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const unsigned ORGANISM_SPAWN_TRIES = 5;
const unsigned RESEED_TRIES = 10;
const unsigned FOOD_SPAWN_TRIES = 5;

struct World World_random(size_t width, size_t height, TILE_SEED tile_seed, struct WorldConfig* conf) {
	struct World w;
	w.width = width;
	w.height = height;

	w.tiles = malloc(width * height * sizeof(struct Tile));

	Brain_calc_weight_num(&conf->brain);
	Brain_calc_max_layer_size(&conf->brain);

	w.conf = conf;

	w.alive_counter = 0;

	for (size_t i = 0; i < width * height; ++i) {
		switch (TILE_SEED_pick(tile_seed)) {
			case Tile_EMPTY:
				w.tiles[i] = Tile_empty();
				break;
			case Tile_ORGANISM:
				w.tiles[i] = Tile_organism(Organism_new(
					conf->fullness,
					(unsigned)rand() % conf->fullness_threshold_max,
					conf->lifetime,
					random_weights(&conf->brain, conf->start_mutation)
				));

				++w.alive_counter;

				break;
			case Tile_FOOD:
				w.tiles[i] = Tile_food(conf->nutrition);
				break;
			case Tile_ROCK:
				w.tiles[i] = Tile_rock();
				break;
		}
	}

	return w;
}

void World_reseed(struct World* self, size_t target) {
	unsigned tries = RESEED_TRIES;

	while (self->alive_counter < target && tries > 0) {
		struct Tile* tile = World_select(self, Tile_EMPTY, ORGANISM_SPAWN_TRIES);
		if (tile) {
			*tile = Tile_organism(Organism_new(
				self->conf->fullness,
				(unsigned)rand() % self->conf->fullness_threshold_max,
				self->conf->lifetime,
				random_weights(&self->conf->brain, self->conf->start_mutation)
			));

			++self->alive_counter;
		} else --tries;
	}
}

void World_add_food(struct World* self, size_t amount) {
	for (size_t i = 0; i < amount; ++i) {
		struct Tile* tile = World_select(self, Tile_EMPTY, FOOD_SPAWN_TRIES);
		if (tile) *tile = Tile_food(self->conf->nutrition);
	}
}

void World_update(struct World* self) {
	char* skip_on_this_line = calloc(self->width, sizeof(char));
	char* skip_on_next_line = malloc(self->width);

	for (size_t y = 0; y < self->height; ++y) {
		memset(skip_on_next_line, 0, self->width);

		for (size_t x = 0; x < self->width; ++x) {
			struct Tile* tile = World_get_unchecked(self, x, y);

			if (tile->tag == Tile_ORGANISM && !skip_on_this_line[x]) {
				Organism_tick(&tile->val.org);
				if (Organism_dead(&tile->val.org)) {
					Organism_drop(&tile->val.org);

					*tile = Tile_empty();

					--self->alive_counter;

					continue;
				}

				Brain_link(&self->conf->brain, Organism_weights(&tile->val.org));

				char* input = calloc(Brain_input_num(&self->conf->brain), sizeof(char));
				World_vicinity(self, x, y, input);

				struct Reaction reaction = Organism_react(&tile->val.org, &self->conf->brain, input);

				struct Tile* dest;
				switch (reaction.move) {
					case MOVE_UP:
						dest = World_get_unchecked(
							self,
							x,
							World_wrap_y_u(self, y, 1));

						if (Tile_solid(dest)) goto end;
						break;
					case MOVE_DOWN:
						dest = World_get_unchecked(
							self,
							x,
							World_wrap_y_d(self, y, 1));

						if (!Tile_solid(dest))
							// Skip here on the next line to prevent double-updating
							skip_on_next_line[x] = 1;
						else goto end;
						break;
					case MOVE_RIGHT:
						dest = World_get_unchecked(
							self,
							World_wrap_x_r(self, x, 1),
							y);

						// Skip the next tile to prevent double-updating
						if (!Tile_solid(dest)) ++x;
						else goto end;
						break;
					case MOVE_LEFT:
						dest = World_get_unchecked(
							self,
							World_wrap_x_l(self, x, 1),
							y);

						if (Tile_solid(dest)) goto end;
						break;
					default: goto end;
				}

				if (reaction.baby) {
					Tile_org_set(
						dest,
						Organism_baby(
							&tile->val.org,
							&self->conf->brain,
							self->conf->mutation,
							self->conf->mutation_chance,
							self->conf->lifetime
						)
					);

					++self->alive_counter;
				} else {
					Tile_org_set(dest, tile->val.org);

					tile->tag = Tile_EMPTY;
				}

				end:
				free(input);
			}
		}

		memcpy(skip_on_this_line, skip_on_next_line, self->width);
	}

	free(skip_on_this_line);
	free(skip_on_next_line);
}

struct Tile* World_get_unchecked(struct World* self, size_t x, size_t y) {
	return self->tiles + (y * self->width + x);
}

struct Tile* World_get(struct World* self, size_t x, size_t y) {
	if (x < self->width && y < self->height)
		return World_get_unchecked(self, x, y);
	else return NULL;
}

struct Tile* World_select(struct World* self, enum TileTag kind, unsigned tries) {
	struct Tile* tile;

	while (1) {
		tile = &self->tiles[(size_t)rand() % (self->width * self->height)];

		if (tile->tag == kind) break;
		else if (!--tries) return NULL;
	}

	return tile;
}

size_t World_wrap_x_r(const struct World* self, size_t x, size_t x_off) {
	return (x + x_off) % self->width;
}

size_t World_wrap_x_l(const struct World* self, size_t x, size_t x_off) {
	size_t ret_x = x;

	while (ret_x < x_off) ret_x += self->width;

	return ret_x - x_off;
}

size_t World_wrap_y_d(const struct World* self, size_t y, size_t y_off) {
	return (y + y_off) % self->height;
}

size_t World_wrap_y_u(const struct World* self, size_t y, size_t y_off) {
	size_t ret_y = y;

	while (ret_y < y_off) ret_y += self->height;

	return ret_y - y_off;
}

void encode_tile(struct Tile* tile, size_t i, char* dest) {
	switch (tile->tag) {
		case Tile_EMPTY:
			dest[i] = 1;
			break;
		case Tile_ORGANISM:
			dest[i + 8] = 1;
			break;
		case Tile_FOOD:
			dest[i + 16] = 1;
			break;
		case Tile_ROCK:
			dest[i + 24] = 1;
	}
}

void World_vicinity(struct World* self, size_t x, size_t y, char* dest) {
	size_t x_r = World_wrap_x_r(self, x, 1),
	       x_l = World_wrap_x_l(self, x, 1);

	size_t y_d = World_wrap_y_d(self, y, 1),
	       y_u = World_wrap_y_u(self, y, 1);

	encode_tile(World_get_unchecked(self, x_r, y_d), 0, dest);
	encode_tile(World_get_unchecked(self, x_r,   y), 1, dest);
	encode_tile(World_get_unchecked(self, x_r, y_u), 2, dest);
	encode_tile(World_get_unchecked(self, x,   y_u), 3, dest);
	encode_tile(World_get_unchecked(self, x_l, y_u), 4, dest);
	encode_tile(World_get_unchecked(self, x_l,   y), 5, dest);
	encode_tile(World_get_unchecked(self, x_l, y_d), 6, dest);
	encode_tile(World_get_unchecked(self, x,   y_d), 7, dest);
}

size_t World_alive_count(const struct World* self) {
	return self->alive_counter;
}

int World_draw(const struct World* self, FILE* dest) {
	#define TRY(fallible) do { int errno = fallible; if (errno < 0) return errno; } while (0)

	TRY(Tile_draw(&self->tiles[0], dest));

	for (size_t i = 1; i < self->width * self->height; ++i) {
		if (i % self->width == 0)
			TRY(fprintf(dest, "\x1B[0m\n"));

		TRY(Tile_draw(&self->tiles[i], dest));
	}

	TRY(fprintf(dest, "\x1B[0m"));

	return 0;
}
