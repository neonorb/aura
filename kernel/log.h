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

#endif /* LOG_H_ */
