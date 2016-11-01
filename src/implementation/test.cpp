/*
 * test.cpp
 *
 *  Created on: May 17, 2016
 *      Author: chris
 */

#ifdef DO_TEST
#include <memory.h>
#include <list.h>
#include <stack.h>
#include <log.h>
#include <mish.h>
#include <feta.h>

static void assert(bool b, String message) {
	if(!b) {
		crash(message);
	}
}

/*static void memory() {
 log(L"  - memory");

 // test merging of destroy blocks
 const uint32 blockSize = 100000;

 uint64 maxBlockCount = 0;
 uint32 usedMemory = 0;
 List<void*> blocks;

 for(uint64 i = 0; true; i++) {
 void* newLocation = create(blockSize);
 if(newLocation == 0) {
 break;
 }
 blocks.add(newLocation);
 maxBlockCount++;
 usedMemory += blockSize;
 }

 // destroy every other
 for(uint64 i = blocks.size() - 1; i -2 < i && i >= 0; i -= 2) {
 destroy(blocks.remove(i));

 if(i - 2 > i) break; // check for underflow
 }

 // destroy the rest
 for(uint64 i = blocks.size() - 1; i - 1 < i && i >= 0; i--) {
 destroy(blocks.remove(i));
 }

 void* testMemory = create(maxBlockCount * blockSize);

 // our blocks didn't merge correctly
 assert(testMemory == 0, L"cannot allocate the whole memory");

 destroy(testMemory);

 for (uint64 i = 0; i < 1000; i++) {
 String x = dynamicString(L"0123456789");
 char* y = (char*) x;
 for (uint8 i = 0; i < 10; i++) {
 y[i] = 'x';
 }

 // ---------------------- when this is commented out, the thing "runs of of memory" after a few allocations, works fine when it is destroyd --------------
 destroy((void*) x);
 }
 }*/

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
		assert(value == 5, "iterator value is not 5");
	}
	assert(didFirst, "iterator didn't do first");

	assert(list.remove((uinteger) 0) == 5, "removed value is not 5");
	assert(list.size() == 0, "list size is not 0 (2)");

	list.add('a');
	list.add('b');
	list.add('c');
	list.add('d');
	list.add('e');
	list.add('f');
	list.add('g');
	list.add('h');

	assert(list.size() == 8, "list size is not 3");
	assert(list.get(2) == 'c', "value @ 2 is not c");
	assert(list.get(7) == 'h', "value @ 7 is not h");

	list.clear();
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

	assert(strequ(first, second), "string not equal");
}

int flag1 = 0;
static Value* triggerFlag1Function(List<Value*>* arguments) {
	UNUSED(arguments);

	flag1++;
	return VALUE_NOT_USED;
}

int trueFalseCounter = 0;
static Value* trueFalseFunction(List<Value*>* arguments) {
	UNUSED(arguments);

	Value* ret;

	if(trueFalseCounter == 1) {
		ret = new BooleanValue(false);
		trueFalseCounter = 0;
	} else {
		ret = new BooleanValue(true);
		trueFalseCounter++;
	}

	return ret;
}

int trueTrueFalseCounter = 0;
static Value* trueTrueFalseFunction(List<Value*>* arguments) {
	UNUSED(arguments);

	Value* ret;

	if(trueTrueFalseCounter == 2) {
		ret = new BooleanValue(false);
		trueTrueFalseCounter = 0;
	} else {
		ret = new BooleanValue(true);
		trueTrueFalseCounter++;
	}

	return ret;
}

static void resetFlags() {
	flag1 = 0;
	trueFalseCounter = 0;
	trueTrueFalseCounter = 0;
}

static void testMishCode(String sourceCode) {
	Code* code = mish_compile(sourceCode);
	mish_execute(code);
	delete code;
}

List<Function*> testSyscalls;
static void mish() {
	log("  - mish");

	// ---- register syscalls ----
	List<ValueType>* triggerFlag1ParameterTypes = new List<ValueType>();
	Function* triggerFlag1 = new Function("__triggerFlag1", triggerFlag1ParameterTypes, VOID_VALUE, triggerFlag1Function);
	mish_syscalls.add(triggerFlag1);
	testSyscalls.add(triggerFlag1);

	List<ValueType>* trueFalseParameterTypes = new List<ValueType>();
	Function* trueFalse = new Function("__trueFalse", trueFalseParameterTypes, VOID_VALUE, trueFalseFunction);
	mish_syscalls.add(trueFalse);
	testSyscalls.add(trueFalse);

	List<ValueType>* trueTrueFalseParameterTypes = new List<ValueType>();
	Function* trueTrueFalse = new Function("__trueTrueFalse", trueTrueFalseParameterTypes, VOID_VALUE, trueTrueFalseFunction);
	mish_syscalls.add(trueTrueFalse);
	testSyscalls.add(trueTrueFalse);

	// get allocated count
	uint64 origionalAllocatedCount = getAllocatedCount();

	// ---- tests ----

	resetFlags();

	testMishCode("__triggerFlag1()");
	assert(flag1 == 1, "1");
	resetFlags();

	testMishCode("if(false){ __triggerFlag1() }");
	assert(flag1 == 0, "2");
	resetFlags();

	testMishCode("if(true){ __triggerFlag1() }");
	assert(flag1 == 1, "3");
	resetFlags();

	testMishCode("if(true){ while(false){ __triggerFlag1() } }");
	assert(flag1 == 0, "4");
	resetFlags();

	testMishCode("if(false){ while(true){ __triggerFlag1() } }");
	assert(flag1 == 0, "5");
	resetFlags();

	testMishCode("while(__trueFalse()){ __triggerFlag1() }");
	assert(flag1 == 1, "6");
	resetFlags();

	testMishCode("while(__trueTrueFalse()){ __triggerFlag1() }");
	assert(flag1 == 2, "7");
	resetFlags();

	// ---- done tests ----

	// get allocated count
	uint64 laterAllocatedCount = getAllocatedCount();

	// confirm no memory leaks
	assert(origionalAllocatedCount == laterAllocatedCount, "memory leak");

	// unregister syscalls
	Iterator<Function*> iterator = testSyscalls.iterator();
	while (iterator.hasNext()) {
		Function* function = iterator.next();
		mish_syscalls.remove(function);
		delete function;
	}
	testSyscalls.clear();
}

void test() {
	log(" -- TESTING -- ");
	//memory();
	list();
	stack();
	string();
	mish();
	log(" -- TESTS PASSED --");
}

#endif
