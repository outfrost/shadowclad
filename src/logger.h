#ifndef LOGGER_H_
#define LOGGER_H_

typedef enum {
	LOGLEVEL_ERROR,
	LOGLEVEL_WARNING,
	LOGLEVEL_INFO,
	LOGLEVEL_DEBUG
} LogLevel;

LogLevel logLevel;

#define logError(...) logMessage(LOGLEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define logWarning(...) logMessage(LOGLEVEL_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define logInfo(...) logMessage(LOGLEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define logDebug(...) logMessage(LOGLEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)

void logMessage(LogLevel msgLevel, const char* file, int line, const char* message, ...);

#endif
