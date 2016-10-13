#include <implementation/pong.h>
#include <implementation/system/auramain.h>

//#define TESTING
#ifdef TESTING
#include "test.cpp"
#endif

void implementation() {
#ifdef TESTING
	test();
#else
	auraMain();
	//pong();
#endif
}
