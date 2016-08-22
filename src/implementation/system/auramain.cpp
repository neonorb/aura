#include <memory.h>

#include <log.h>
#include <mish.h>
#include <syscall.h>
#include <value.h>
#include <implementation/system/syscalls.h>
#include <modules/keyboard/keyboard.h>

extern uint8 mishStart; // &mishStart - start of Mish code
extern uint8 mishEnd; // &mishEnd - end of Mish code

void keyboardHandler(KeyEvent keyEvent) {
	log(L"key!");
}

void auraMain() {
	log(L"Starting Aura");

	keyboard_handler(keyboardHandler);

	//dumpAllocated();

	registerSyscalls();

	// convert to 16-bit string
	uint64 charCount = &mishEnd - &mishStart;
	wchar_t* sourceCode = (wchar_t*) create(charCount * 2 + 1);
	for (uint64 i = 0; i < charCount; i++) {
		sourceCode[i] = (wchar_t) ((&mishStart)[i]);
	}
	sourceCode[charCount] = 0; // NULL terminate

	Code* code = mish_compile(sourceCode);
	code->execute();
	delete code;

	unregisterSyscalls();

	//dumpAllocated();
}
