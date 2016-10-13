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
#include <mish.h>

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
	log(L"  - list");

	List<char> list;

	assert(list.size() == 0, L"list size is not 0");

	list.add(5);
	assert(list.size() == 1, L"list size is not 1");
	assert(list.get(0) == 5, L"value @ 0 is not 5");

	Iterator<char> iterator = list.iterator();
	bool didFirst = false;
	while(iterator.hasNext()) {
		char value = iterator.next();
		didFirst = true;
		debug(L"iterator value", value);
		assert(value == 5, L"iterator value is not 5");
	}
	assert(didFirst, L"iterator didn't do first");

	assert(list.remove((unsigned long long int) 0) == 5, L"removed value is not 5");
	assert(list.size() == 0, L"list size is not 0 (2)");

	debug(L"list size", list.size());
	list.add('a');
	debug(L"list size", list.size());
	debug(L"value @ 0", list.get(0));
	list.add('b');
	debug(L"list size", list.size());
	debug(L"value @ 0", list.get(0));
	debug(L"value @ 1", list.get(1));
	list.add('c');
	debug(L"list size", list.size());
	debug(L"value @ 0", list.get(0));
	debug(L"value @ 1", list.get(1));
	debug(L"value @ 2", list.get(2));
	list.add('d');
	debug(L"list size", list.size());
	debug(L"value @ 0", list.get(0));
	debug(L"value @ 1", list.get(1));
	debug(L"value @ 2", list.get(2));
	debug(L"value @ 3", list.get(3));
	list.add('e');
	debug(L"list size", list.size());
	debug(L"value @ 0", list.get(0));
	debug(L"value @ 1", list.get(1));
	debug(L"value @ 2", list.get(2));
	debug(L"value @ 3", list.get(3));
	debug(L"value @ 4", list.get(4));
	list.add('f');
	debug(L"list size", list.size());
	debug(L"value @ 0", list.get(0));
	debug(L"value @ 1", list.get(1));
	debug(L"value @ 2", list.get(2));
	debug(L"value @ 3", list.get(3));
	debug(L"value @ 4", list.get(4));
	debug(L"value @ 5", list.get(5));
	list.add('g');
	debug(L"list size", list.size());
	debug(L"value @ 0", list.get(0));
	debug(L"value @ 1", list.get(1));
	debug(L"value @ 2", list.get(2));
	debug(L"value @ 3", list.get(3));
	debug(L"value @ 4", list.get(4));
	debug(L"value @ 5", list.get(5));
	debug(L"value @ 6", list.get(6));
	list.add('h');
	debug(L"list size", list.size());
	debug(L"value @ 0", list.get(0));
	debug(L"value @ 1", list.get(1));
	debug(L"value @ 2", list.get(2));
	debug(L"value @ 3", list.get(3));
	debug(L"value @ 4", list.get(4));
	debug(L"value @ 5", list.get(5));
	debug(L"value @ 6", list.get(6));
	debug(L"value @ 7", list.get(7));

	assert(list.size() == 8, L"list size is not 3");
	assert(list.get(2) == 'c', L"value @ 2 is not c");
	assert(list.get(7) == 'h', L"value @ 7 is not h");

	list.clear();
}

static void stack() {
	log(L"  - stack");

	Stack<int> stack;

	assert(stack.size() == 0, L"stack size is not 0");

	stack.push(5);
	assert(stack.size() == 1, L"stack size is not 1");
	assert(stack.peek() == 5, L"peeked value is not 5");

	assert(stack.pop() == 5, L"popped value is not 5");
	assert(stack.size() == 0, L"stack size is not 0 (2)");

	stack.push(10);
	stack.push(11);
	stack.push(12);
	stack.push(13);
	assert(stack.size() == 4, L"stack size is not 4");
	assert(stack.peek() == 13, L"peeked value is not 13");

	stack.pop();
	assert(stack.size() == 3, L"stack size is not 3");
}

static void string() {
	log(L"  - string");

	String first = substring(L"__print", 2, 7);
	String second = L"print";

	assert(strequ(first, second), L"string not equal");
}

int flag1 = 0;
static Value* triggerFlag1Function(List<Value*>* arguments) {
	flag1++;
	return NULL;
}

int trueFalseCounter = 0;
static Value* trueFalseFunction(List<Value*>* arguments) {
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
	log(L"  - mish");

	// ---- register syscalls ----
	List<ValueType>* triggerFlag1ParameterTypes = new List<ValueType>();
	Function* triggerFlag1 = new Function(L"__triggerFlag1", triggerFlag1ParameterTypes, VOID_VALUE, triggerFlag1Function);
	mish_syscalls.add(triggerFlag1);
	testSyscalls.add(triggerFlag1);

	List<ValueType>* trueFalseParameterTypes = new List<ValueType>();
	Function* trueFalse = new Function(L"__trueFalse", trueFalseParameterTypes, VOID_VALUE, trueFalseFunction);
	mish_syscalls.add(trueFalse);
	testSyscalls.add(trueFalse);

	List<ValueType>* trueTrueFalseParameterTypes = new List<ValueType>();
	Function* trueTrueFalse = new Function(L"__trueTrueFalse", trueTrueFalseParameterTypes, VOID_VALUE, trueTrueFalseFunction);
	mish_syscalls.add(trueTrueFalse);
	testSyscalls.add(trueTrueFalse);

	// get allocated count
	uint64 origionalAllocatedCount = getAllocatedCount();

	// ---- tests ----

	resetFlags();

	testMishCode(L"__triggerFlag1()");
	assert(flag1 == 1, L"1");
	resetFlags();

	testMishCode(L"if(false){ __triggerFlag1() }");
	assert(flag1 == 0, L"2");
	resetFlags();

	testMishCode(L"if(true){ __triggerFlag1() }");
	assert(flag1 == 1, L"3");
	resetFlags();

	testMishCode(L"if(true){ while(false){ __triggerFlag1() } }");
	assert(flag1 == 0, L"4");
	resetFlags();

	testMishCode(L"if(false){ while(true){ __triggerFlag1() } }");
	assert(flag1 == 0, L"5");
	resetFlags();

	testMishCode(L"while(__trueFalse()){ __triggerFlag1() }");
	assert(flag1 == 1, L"6");
	resetFlags();

	testMishCode(L"while(__trueTrueFalse()){ __triggerFlag1() }");
	assert(flag1 == 2, L"7");
	resetFlags();

	// ---- done tests ----

	// get allocated count
	uint64 laterAllocatedCount = getAllocatedCount();

	// confirm no memory leaks
	assert(origionalAllocatedCount == laterAllocatedCount, L"memory leak");

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
	log(L" -- TESTING -- ");
	//memory();
	list();
	stack();
	string();
	mish();
	log(L" -- TESTS PASSED --");
}

#endif
