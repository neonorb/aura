#ifndef CLOCK_H_
#define CLOCK_H_

typedef void (*ClockHandler)(void);

void clock_initialize();
void clock_handler(ClockHandler handler);

#endif /* CLOCK_H_ */
