#include <implementation/pong.h>
#include <implementation/system/auramain.h>

#ifdef DO_TEST
#include "test.cpp"
#endif

void implementation() {
#ifdef DO_TEST
	test();
#else
	auraMain();
#endif
}
