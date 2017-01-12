#include <feta.h>
#include <modules/screen/screen.h>

#include <mish.h>
#include <implementation/system/syscalls.h>
#include <implementation/system/console.h>
#include <modules/modules.h>
#include <kernel/events.h>
#include <boot/uefi.h>

bool probeLoop = true;

void auraMain() {
#ifdef MEMORY_LOG
	dumpAllocated();
#endif

	debug("auraMain");

	registerSyscalls();
	debug("syscalls registered");

	console();
	debug("console");

	while (probeLoop) {
		// wait for an interrupt
		if (mish::execute::schedule::activeThreadCount() == 0) {
			// TODO don't run background threads when there are active ones
			//stallMilli(1000);
			asm("hlt");
		}

		modules_probe(); // probe modules for input, input will trigger an event
		mish::execute::schedule::run(); // execute threads
	}

	unregisterSyscalls();

#ifdef MEMORY_LOG
	dumpAllocated();
#endif
}
