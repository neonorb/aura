#ifndef LOG_H_
#define LOG_H_

#include <string.h>

typedef enum {
	NORMAL,
	WARNING,
	FAULT
} LogType;

void log(String message);
void warning(String message);
void fault(String message);
void log_write(LogType, String);

void debug(String name, uint64 value);
void debug(String name, uint64 value, uint8 base);
void debug(String message);
void debugPrint(String message);

#endif /* LOG_H_ */
