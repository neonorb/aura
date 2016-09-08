#include <memory.h>

#include <log.h>
#include <modules/screen/screen.h>

#include <mish.h>
#include <syscall.h>
#include <value.h>
#include <implementation/system/syscalls.h>
#include <modules/modules.h>
#include <kernel/events.h>
#include <memory.h>
#include <boot/uefi.h>

extern uint8 mishStart; // &mishStart - start of Mish code
extern uint8 mishEnd; // &mishEnd - end of Mish code

List<wchar_t> line;

void keyboardHandler(EFI_INPUT_KEY keyEvent) {
	if (keyEvent.UnicodeChar > 0) {
		if (keyEvent.UnicodeChar == 0xD) { // carriage return
			screen_terminal_writeString(L"\n\r");
			wchar_t* str = (wchar_t*) create(line.size() * 2 + 1);

			Iterator<wchar_t> stringIterator = line.iterator();
			uint64 strIndex = 0;
			while (stringIterator.hasNext()) {
				str[strIndex] = stringIterator.next();
				strIndex++;
			}
			str[strIndex] = NULL; // null terminate
			line.clear();

			Code* code = mish_compile(str);
			delete str;

			if (code != NULL) {
				mish_execute(code);
				delete code;
			}
		} else if (keyEvent.UnicodeChar == 0x8) { // backspace
			if (line.size() == 0) {
				return;
			}
			screen_terminal_writeString(keyEvent.UnicodeChar);
			line.remove(line.size() - 1);
		} else {
			screen_terminal_writeString(keyEvent.UnicodeChar);
			line.add(keyEvent.UnicodeChar);
		}
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

	if (code != NULL) {
		statusDone();
		mish_execute(code);
		delete code;
	} else {
		statusFail();
	}

	while (true) {
		//uefi_call_wrapper((void*) systemTable->BootServices->Stall, 1, 100000);
		//log(L"hi");
		asm("hlt");
		modules_probe();
	}

	unregisterSyscalls();

	//dumpAllocated();
}
