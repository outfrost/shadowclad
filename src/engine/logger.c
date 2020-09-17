/**
 * Copyright 2019-2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "logger.h"

#include <stdarg.h>
#include <stdio.h>

LogLevel logLevel = LOGLEVEL_DEBUG;



void logMessage(LogLevel msgLevel, const char* func, const char* message, ...) {
	if (msgLevel > logLevel) {
		return;
	}

	const char* msgLevelString;
	switch (msgLevel) {
		case LOGLEVEL_ERROR:
			msgLevelString = "ERROR ";
			break;
		case LOGLEVEL_WARNING:
			msgLevelString = "WARNING ";
			break;
		case LOGLEVEL_INFO:
			msgLevelString = "";
			break;
		case LOGLEVEL_DEBUG:
			msgLevelString = "DEBUG ";
			break;
		default:
			msgLevelString = "(invalid message level) ";
			break;
	}

	va_list args;
	va_start(args, message);

	fprintf(stderr, "%s %s:: ", func, msgLevelString);
	vfprintf(stderr, message, args);
	fputc('\n', stderr);

	va_end(args);
}
