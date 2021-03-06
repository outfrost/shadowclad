/**
 * Copyright 2019-2020 Iwo 'Outfrost' Bujkiewicz
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ENGINE_LOGGER_H_
#define ENGINE_LOGGER_H_

enum LogLevel {
	LOGLEVEL_ERROR,
	LOGLEVEL_WARNING,
	LOGLEVEL_INFO,
	LOGLEVEL_DEBUG
};

typedef enum LogLevel LogLevel;

extern LogLevel logLevel;

#define logError(...) logMessage(LOGLEVEL_ERROR, __func__, __VA_ARGS__)
#define logWarning(...) logMessage(LOGLEVEL_WARNING, __func__, __VA_ARGS__)
#define logInfo(...) logMessage(LOGLEVEL_INFO, __func__, __VA_ARGS__)
#define logDebug(...) logMessage(LOGLEVEL_DEBUG, __func__, __VA_ARGS__)

void logMessage(LogLevel msgLevel, const char* func, const char* message, ...);

#endif // ENGINE_LOGGER_H_
