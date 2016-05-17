#include "pong.cpp"
#include "../kernel/log.h"

#ifdef TESTING
#include "test.cpp"
#endif

void implementation() {
#ifdef TESTING
	log(" -- TESTING -- ");
	test();
#endif
}
