/*
 * test.cpp
 *
 *  Created on: May 17, 2016
 *      Author: chris
 */

#ifdef TESTING
#include <memory.h>

static void memory() {
	log("  - memory");

	// test merging of free blocks
	const uint32 blockSize = 1000000;

	uint64 maxBlockCount = 0;
	uint32 initialAvailableMemory = availableMemory();
	uint32 usedMemory = 0;
	List<void*> blocks;

	for(uint64 i = 0; canAllocate(blockSize); i++) {
		blocks.add(malloc(blockSize));
		maxBlockCount++;
		usedMemory += blockSize;
	}

	debug("free blocks", freeBlocks.size());

	// free every other
	for(uint64 i = blocks.size() - 1; i -2 < i && i >= 0; i -= 2) {
		free(blocks.remove(i), blockSize);

		if(i - 2 > i) break; // check for underflow
	}

	// free the rest
	for(uint64 i = blocks.size() - 1; i - 1 < i && i >= 0; i--) {
		free(blocks.remove(i), blockSize);
	}

	if(!canAllocate(maxBlockCount * blockSize)) {
		// our blocks didn't merge correctly
		debug("free blocks", freeBlocks.size());
		crash("cannot allocate the whole memory");
	}

	/*for (uint64 i = 0; i < 1000; i++) {
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
	 }*/
}

void test() {
	log(" -- TESTING -- ");
	memory();
	log(" -- TESTS PASSED --");
}

#endif
