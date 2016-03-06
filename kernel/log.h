#ifndef LOG_H_
#define LOG_H_

typedef enum {
	NORMAL,
	WARNING,
	FAULT
} LogType;

void log(char* message);
void warning(char* message);
void fault(char* message);
void log_write(LogType, char*);

#endif /* LOG_H_ */
