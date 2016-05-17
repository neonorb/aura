/*
 * test.cpp
 *
 *  Created on: May 17, 2016
 *      Author: chris
 */

#ifdef TESTING
#include <memory.h>

void test() {
	for (uint64 i = 0; i < 1000; i++) {
		String x = dynamicString("0123456789");
		char* y = (char*) x;
		debug(y);
		for (uint8 i = 0; i < 10; i++) {
			//debug("appending");
			y[i] = 'x';
		}
		debug(y);
		log(y);

		// ---------------------- when this is commented out, the thing "runs of of memory" after a few allocations, works fine when it is freed --------------
		//free((void*) x, strlen(x) + 1);
	}
}

#endif
