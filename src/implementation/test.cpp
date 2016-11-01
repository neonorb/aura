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
#include <mishtest.h>

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


void test() {
	log(" -- TESTING -- ");
	//memory();
	mishtest::test();
	log(" -- TESTS PASSED --");
}

#endif
