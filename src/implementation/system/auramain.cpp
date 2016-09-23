#include <memory.h>

#include <log.h>
#include <modules/screen/screen.h>

#include <mish.h>
#include <syscall.h>
#include <value.h>
#include <implementation/system/syscalls.h>
#include <implementation/system/console.h>
#include <modules/modules.h>
#include <kernel/events.h>
#include <memory.h>
#include <boot/uefi.h>

bool probeLoop = true;

void auraMain() {
	dumpAllocated();

	registerSyscalls();

	console();

	while (probeLoop) {
		asm("hlt");
		// wait for an interrupt
		modules_probe(); // probe modules for input, input will trigger an event
		mish_runScheduler(); // execute threads
	}

	unregisterSyscalls();

	dumpAllocated();
}
