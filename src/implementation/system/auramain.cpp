#include <memory.h>

#include <log.h>
#include <mish.h>
#include <syscall.h>
#include <value.h>
#include "syscalls.cpp"

extern uint8 mishStart; // &mishStart - start of Mish code
extern uint8 mishEnd; // &mishEnd - end of Mish code

void auraMain() {
	log("Starting Aura");

	//dumpAllocated();

	registerSyscalls();

	Code* code = mish_compile((String) &mishStart, &mishEnd);
	code->execute();
	delete code;

	unregisterSyscalls();

	//dumpAllocated();
}
