#include <time.h>
#include <stdio.h> // TODO remove
#include <stdbool.h>

typedef struct timespec Timepoint;

static Timepoint lastDisplayTime;
static int frames = 0;
static bool meteringEnabled = false;

void initPerformanceMetering() {
	if (clock_gettime(CLOCK_MONOTONIC, &lastDisplayTime) != 0) {
		fprintf(stderr, "Clock read failed, performance metering unavailable\n");
	}
	else {
		meteringEnabled = true;
	}
}

void frameRendered() {
	if (meteringEnabled) {
		++frames;
		Timepoint now;
		
		if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
			fprintf(stderr, "Clock read failed, stopping performance metering\n");
			meteringEnabled = false;
			return;
		}
		
		time_t fullSeconds = now.tv_sec - lastDisplayTime.tv_sec;
		if (now.tv_nsec < lastDisplayTime.tv_nsec) --fullSeconds;
		
		if (fullSeconds > 0) {
			float seconds = (now.tv_nsec - lastDisplayTime.tv_nsec) / 1000000000.0f;
			seconds += (float) (now.tv_sec - lastDisplayTime.tv_sec);
			printf("frametime avg %.1f ms; fps avg %.f\n", (seconds / frames) * 1000.0f, (frames / seconds));
			lastDisplayTime = now;
			frames = 0;
		}
	}
}
