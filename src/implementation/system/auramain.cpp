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

void auraMain() {
	//dumpAllocated();

	registerSyscalls();

	console();

	while (true) {
		//uefi_call_wrapper((void*) systemTable->BootServices->Stall, 1, 100000);
		//log(L"hi");
		asm("hlt");
		modules_probe();
	}

	unregisterSyscalls();

	//dumpAllocated();
}
