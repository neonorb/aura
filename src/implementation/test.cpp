/*
 * test.cpp
 *
 *  Created on: May 17, 2016
 *      Author: chris
 */

#ifdef TESTING
#include <memory.h>
#include <list.h>
#include <stack.h>
#include <log.h>

static void assert(bool b, String message) {
	if(!b) {
		crash(message);
	}
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

	// our blocks didn't merge correctly
	assert(testMemory == 0, "cannot allocate the whole memory");

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

static void list() {
	log("  - list");

	List<int> list = List<int>();

	assert(list.size() == 0, "list size is not 0");

	list.add(5);
	assert(list.size() == 1, "list size is not 1");
	assert(list.get(0) == 5, "value @ 0 is not 5");

	assert(list.remove((unsigned long long int) 0) == 5, "removed value is not 5");
	assert(list.size() == 0, "list size is not 0 (2)");

	list.add(10);
	list.add(11);
	list.add(12);
	assert(list.size() == 3, "list size is not 3");
	assert(list.get(2) == 12, "value @ 2 is not 12");

	list.destroy();
}

static void stack() {
	log("  - stack");

	Stack<int> stack = Stack<int>();

	assert(stack.size() == 0, "stack size is not 0");

	stack.push(5);
	assert(stack.size() == 1, "stack size is not 1");
	assert(stack.peek() == 5, "peeked value is not 5");

	assert(stack.pop() == 5, "popped value is not 5");
	assert(stack.size() == 0, "stack size is not 0 (2)");

	stack.push(10);
	stack.push(11);
	stack.push(12);
	assert(stack.size() == 3, "stack size is not 3");
	assert(stack.peek() == 12, "peeked value is not 12");

	stack.destroy();
}

void test() {
	log(" -- TESTING -- ");
	memory();
	list();
	stack();
	log(" -- TESTS PASSED --");
}

#endif
