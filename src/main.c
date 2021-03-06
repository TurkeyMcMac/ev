#include "cli.h"
#include "tile.h"
#include "world.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
	struct ProgConfig config = PROG_CONFIG_DEFAULT;
	pthread_mutex_t config_mutex;
	int mutex_err = pthread_mutex_init(&config_mutex, NULL);
	if (mutex_err) {
		fprintf(stderr, "Could not initialize mutex.\n");

		return mutex_err;
	}

	int parse_failed = load_config_to(&config, argc, argv);
	if (parse_failed) return parse_failed;

	struct MonitorArgs monitor_args = {
		.config = &config,
		.monitor_flag = &config_mutex
	};
	pthread_t input_monitor_id;
	int monitor_err = pthread_create(&input_monitor_id, NULL, monitor_input, &monitor_args);
	if (monitor_err) {
		fprintf(stderr, "Could not initialize input monitor thread.\n");

		return monitor_err;
	}

	srand(time(NULL));

	struct World w = World_random(
		config.world_width,
		config.world_height,
		config.seed,
		&config.world
	);

	// This speeds up printing the screen to stderr
	char screen_buf[BUFSIZ];
	setbuf(stderr, screen_buf);

	unsigned long tick = 0;
	while (1) {
		pthread_mutex_lock(&config_mutex);

		World_update(&w);
		World_reseed(&w, config.minimum_population);
		World_add_food(&w, config.food_per_tick);

		if (++tick % config.ticks_per_frame == 0) {
			if (World_draw(&w, stderr) < 0)
				fprintf(stderr, "Error drawing world.\n");
			if (fprintf(stderr, "\nalive: %ld\n", World_alive_count(&w)) < 0)
				fprintf(stderr, "Error displaying population.\n");
			fflush(stderr);

			pthread_mutex_unlock(&config_mutex);

			nanosleep(&config.frame_delay, NULL);
		} else pthread_mutex_unlock(&config_mutex);
	}
}
