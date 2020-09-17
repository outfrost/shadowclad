/**
 * Copyright 2018-2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ENGINE_ENGINE_H_
#define ENGINE_ENGINE_H_

#include "string.h"

typedef struct EngineConfig EngineConfig;

struct EngineConfig {
	int windowWidth;
	int windowHeight;
	String windowTitle;
	int swapInterval;
};

void init(EngineConfig);
void run(void (*updateFn) (float));
void terminate();

EngineConfig defaultConfig();

#endif // ENGINE_ENGINE_H_
