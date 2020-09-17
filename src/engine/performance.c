/**
 * Copyright 2019-2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#include "performance.h"

#include <stdbool.h>
#include <stdio.h> // TODO remove
#include <time.h>

#include "logger.h"

typedef struct timespec Timepoint;

static Timepoint lastDisplayTime;
static int frames = 0;
static bool meteringEnabled = false;



void initPerformanceMetering() {
	if (clock_gettime(CLOCK_MONOTONIC, &lastDisplayTime) != 0) {
		logWarning("Clock read failed, performance metering unavailable");
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
			logWarning("Clock read failed, stopping performance metering");
			meteringEnabled = false;
			return;
		}

		time_t fullSeconds = now.tv_sec - lastDisplayTime.tv_sec;
		if (now.tv_nsec < lastDisplayTime.tv_nsec) --fullSeconds;

		if (fullSeconds > 0) {
			float seconds = (now.tv_nsec - lastDisplayTime.tv_nsec) / 1000000000.0f;
			seconds += (float) (now.tv_sec - lastDisplayTime.tv_sec);
			// This goes to STDOUT because it's, uh, temporary
			printf("frametime avg %.1f ms; fps avg %.f\n", (seconds / frames) * 1000.0f, (frames / seconds));
			lastDisplayTime = now;
			frames = 0;
		}
	}
}
