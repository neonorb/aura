#include <implementation/pong.h>
#include <implementation/system/auramain.h>

#ifdef AURA_DOTEST
#include "test.cpp"
#endif

void implementation() {
#ifdef AURA_DOTEST
	test();
#else
	auraMain();
#endif
}
