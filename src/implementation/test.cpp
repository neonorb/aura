/*
 * test.cpp
 *
 *  Created on: May 17, 2016
 *      Author: chris
 */

#ifdef TESTING
#include <memory.h>
#include <list.h>
#include <log.h>

void breakpoint() {
	log("breakpoint");
}

static void memory() {
	log("  - memory");

	// test merging of free blocks
	const uint32 blockSize = 100000;

	uint64 maxBlockCount = 0;
	uint32 usedMemory = 0;
	List<void*> blocks;

	for(uint64 i = 0; true; i++) {
		void* newLocation = malloc(blockSize);
		if(newLocation == 0) {
			break;
		}
		blocks.add(newLocation);
		maxBlockCount++;
		usedMemory += blockSize;

		if(i == 0xa5d) {
			breakpoint();
		}
	}

	// free every other
	for(uint64 i = blocks.size() - 1; i -2 < i && i >= 0; i -= 2) {
		free(blocks.remove(i));

		if(i - 2 > i) break; // check for underflow
	}

	// free the rest
	for(uint64 i = blocks.size() - 1; i - 1 < i && i >= 0; i--) {
		free(blocks.remove(i));
	}

	void* testMemory = malloc(maxBlockCount * blockSize);

	if(testMemory == 0) {
		// our blocks didn't merge correctly
		crash("cannot allocate the whole memory");
	}

	free(testMemory);

	for (uint64 i = 0; i < 1000; i++) {
		String x = dynamicString("0123456789");
		char* y = (char*) x;
		for (uint8 i = 0; i < 10; i++) {
			y[i] = 'x';
		}

		// ---------------------- when this is commented out, the thing "runs of of memory" after a few allocations, works fine when it is freed --------------
		free((void*) x);
	}
}

void test() {
	log(" -- TESTING -- ");
	memory();
	log(" -- TESTS PASSED --");
}

#endif
