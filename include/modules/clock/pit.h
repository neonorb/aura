/*
 * pit.h
 *
 *  Created on: Aug 1, 2016
 *      Author: chris13524
 */

#ifndef INCLUDE_MODULES_CLOCK_PIT_H_
#define INCLUDE_MODULES_CLOCK_PIT_H_

typedef void (*PITHandler)(void);
void pit_handler(PITHandler handler);

void pit_initialize();

#endif /* INCLUDE_MODULES_CLOCK_PIT_H_ */
