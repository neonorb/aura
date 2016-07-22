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
#include <functioncallvoid.h>

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

	List<char> list;

	assert(list.size() == 0, "list size is not 0");

	list.add(5);
	assert(list.size() == 1, "list size is not 1");
	assert(list.get(0) == 5, "value @ 0 is not 5");

	Iterator<char> iterator = list.iterator();
	bool didFirst = false;
	while(iterator.hasNext()) {
		char value = iterator.next();
		didFirst = true;
		debug("iterator value", value);
		assert(value == 5, "iterator value is not 5");
	}
	assert(didFirst, "iterator didn't do first");

	assert(list.remove((unsigned long long int) 0) == 5, "removed value is not 5");
	assert(list.size() == 0, "list size is not 0 (2)");

	debug("list size", list.size());
	list.add('a');
	debug("list size", list.size());
	debug("value @ 0", list.get(0));
	list.add('b');
	debug("list size", list.size());
	debug("value @ 0", list.get(0));
	debug("value @ 1", list.get(1));
	list.add('c');
	debug("list size", list.size());
	debug("value @ 0", list.get(0));
	debug("value @ 1", list.get(1));
	debug("value @ 2", list.get(2));
	list.add('d');
	debug("list size", list.size());
	debug("value @ 0", list.get(0));
	debug("value @ 1", list.get(1));
	debug("value @ 2", list.get(2));
	debug("value @ 3", list.get(3));
	list.add('e');
	debug("list size", list.size());
	debug("value @ 0", list.get(0));
	debug("value @ 1", list.get(1));
	debug("value @ 2", list.get(2));
	debug("value @ 3", list.get(3));
	debug("value @ 4", list.get(4));
	list.add('f');
	debug("list size", list.size());
	debug("value @ 0", list.get(0));
	debug("value @ 1", list.get(1));
	debug("value @ 2", list.get(2));
	debug("value @ 3", list.get(3));
	debug("value @ 4", list.get(4));
	debug("value @ 5", list.get(5));
	list.add('g');
	debug("list size", list.size());
	debug("value @ 0", list.get(0));
	debug("value @ 1", list.get(1));
	debug("value @ 2", list.get(2));
	debug("value @ 3", list.get(3));
	debug("value @ 4", list.get(4));
	debug("value @ 5", list.get(5));
	debug("value @ 6", list.get(6));
	list.add('h');
	debug("list size", list.size());
	debug("value @ 0", list.get(0));
	debug("value @ 1", list.get(1));
	debug("value @ 2", list.get(2));
	debug("value @ 3", list.get(3));
	debug("value @ 4", list.get(4));
	debug("value @ 5", list.get(5));
	debug("value @ 6", list.get(6));
	debug("value @ 7", list.get(7));

	assert(list.size() == 8, "list size is not 3");
	assert(list.get(2) == 'c', "value @ 2 is not c");
	assert(list.get(7) == 'h', "value @ 7 is not h");
}

static void stack() {
	log("  - stack");

	Stack<int> stack;

	assert(stack.size() == 0, "stack size is not 0");

	stack.push(5);
	assert(stack.size() == 1, "stack size is not 1");
	assert(stack.peek() == 5, "peeked value is not 5");

	assert(stack.pop() == 5, "popped value is not 5");
	assert(stack.size() == 0, "stack size is not 0 (2)");

	stack.push(10);
	stack.push(11);
	stack.push(12);
	stack.push(13);
	assert(stack.size() == 4, "stack size is not 4");
	assert(stack.peek() == 13, "peeked value is not 13");

	stack.pop();
	assert(stack.size() == 3, "stack size is not 3");
}

static void string() {
	log("  - string");

	String first = substring("__print", 2, 7);
	String second = "print";

	assert(strequ(first, second), "String not equal");
}

void test() {
	log(" -- TESTING -- ");
	memory();
	list();
	stack();
	string();
	log(" -- TESTS PASSED --");
}

#endif
