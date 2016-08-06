//#include "pong.cpp"
#include "system/auramain.cpp"

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
