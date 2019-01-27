#include <stdarg.h>
#include <stdio.h>

#include "logger.h"

LogLevel logLevel = LOGLEVEL_DEBUG;

void logMessage(LogLevel msgLevel, const char* file, int line, const char* message, ...) {
	if (msgLevel > logLevel) {
		return;
	}
	
	const char* msgLevelString;
	switch (msgLevel) {
		case LOGLEVEL_ERROR:
			msgLevelString = "error: ";
			break;
		case LOGLEVEL_WARNING:
			msgLevelString = "warning: ";
			break;
		case LOGLEVEL_INFO:
			msgLevelString = "";
			break;
		case LOGLEVEL_DEBUG:
			msgLevelString = "debug: ";
			break;
		default:
			msgLevelString = "(invalid message level!) ";
			break;
	}
	
	va_list args;
	va_start(args, message);
	
	fprintf(stderr, "%s:%i: %s", file, line, msgLevelString);
	vfprintf(stderr, message, args);
	fputc('\n', stderr);
	
	va_end(args);
}
