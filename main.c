#include "organism.h"
#include "tile.h"
#include "world.h"

#include <stdlib.h>
#include <time.h>

void delay (int millis) {
#ifdef WIN32
	#include <windows.h>
	Sleep(millis);
#else
	#include <unistd.h>
	usleep(millis * 1000);
#endif
}

int main() {
	srand(time(NULL));
	TILE_SEED seed = {
		.organism = 1,
		.food     = 10,
		.rock     = 5
	};
	size_t layers[3] = {16, 8, 4};
	struct World w = World_random(
		100,
		50,
		seed,
		10,
		10,
		0,
		10.0,
		32,
		(size_t*)&layers,
		3);

	while (1) {
		World_update(&w);

		World_draw(&w, stdout);
		printf("\n\n\n");
		delay(100);
	}
}
/*
struct World World_random(
	size_t width,
	size_t height,
	TILE_SEED tile_seed,
	unsigned int nutrition,
	unsigned int fullness,
	unsigned int unhealth,
	float mutation_amount,
	size_t nn_input_num,
	size_t* nn_layers,
	size_t nn_layer_num
);

*/
