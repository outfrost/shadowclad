#ifndef LOGGER_H_
#define LOGGER_H_

typedef enum {
	LOGLEVEL_ERROR,
	LOGLEVEL_WARNING,
	LOGLEVEL_INFO,
	LOGLEVEL_DEBUG
} LogLevel;

LogLevel logLevel;

#define logError(...) logMessage(LOGLEVEL_ERROR, __func__, __VA_ARGS__)
#define logWarning(...) logMessage(LOGLEVEL_WARNING, __func__, __VA_ARGS__)
#define logInfo(...) logMessage(LOGLEVEL_INFO, __func__, __VA_ARGS__)
#define logDebug(...) logMessage(LOGLEVEL_DEBUG, __func__, __VA_ARGS__)

void logMessage(LogLevel msgLevel, const char* func, const char* message, ...);

#endif
