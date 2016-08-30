#include <memory.h>

#include <log.h>
#include <modules/screen/screen.h>

#include <mish.h>
#include <syscall.h>
#include <value.h>
#include <implementation/system/syscalls.h>
#include <modules/modules.h>
#include <kernel/events.h>

extern uint8 mishStart; // &mishStart - start of Mish code
extern uint8 mishEnd; // &mishEnd - end of Mish code

void keyboardHandler(EFI_INPUT_KEY keyEvent) {
	if (keyEvent.UnicodeChar > 0) {
		screen_terminal_writeString(keyEvent.UnicodeChar);
	} else {
	}
}

void auraMain() {
	eventHandler_keyboard = keyboardHandler;

	//dumpAllocated();

	registerSyscalls();

	// convert to 16-bit string
	uint64 charCount = &mishEnd - &mishStart;
	wchar_t* sourceCode = (wchar_t*) create(charCount * 2 + 1);
	for (uint64 i = 0; i < charCount; i++) {
		sourceCode[i] = (wchar_t) ((&mishStart)[i]);
	}
	sourceCode[charCount] = 0; // NULL terminate

	status(L"compiling");
	Code* code = mish_compile(sourceCode);
	statusDone();
	code->execute();
	delete code;

	unregisterSyscalls();

	while (true) {
		modules_probe();
	}

	//dumpAllocated();
}
