#include "pong.cpp"
#include "auramain.cpp"

#ifdef TESTING
#include "test.cpp"
#endif

void implementation() {
#ifdef TESTING
	test();
#else
	auramain();
#endif
}
