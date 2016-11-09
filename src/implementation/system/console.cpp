/*
 * console.cpp
 *
 *  Created on: Sep 8, 2016
 *      Author: chris13524
 */

#include <implementation/system/console.h>

#include <memory.h>
#include <log.h>
#include <mish.h>
#include <modules/modules.h>
#include <modules/screen/screen.h>
#include <kernel/events.h>
#include <feta.h>

extern uint8 mishStart; // &mishStart - start of Mish code
extern uint8 mishEnd; // &mishEnd - end of Mish code

List<strchar> line;

static void printShell() {
	// if we don't have a new line, print a character to indicate that
	if (screen_terminal_cursorColumn() != 0) {
		screen_terminal_setBackgroundColor(EFI_BLACK);
		screen_terminal_setForegroundColor(EFI_WHITE);
		screen_terminal_writeString("%");

		screen_terminal_resetForegroundColor();
		screen_terminal_resetBackgroundColor();
		screen_terminal_writeString("\n\r");
	}

	screen_terminal_setForegroundColor(EFI_LIGHTBLUE);
	screen_terminal_writeString("> ");
	screen_terminal_resetForegroundColor();
}

Thread* currentThread = NULL;
void console_onThreadExit(Thread* thread) {
	UNUSED(thread);

	currentThread = NULL;
	printShell();
}
static void newThread(Code* code) {
	Thread* thread = new Thread(code, ACTIVE);
	thread->onThreadExit = BIND_FREE_CB(console_onThreadExit);
	mish::execute::schedule::spawn(thread);
	currentThread = thread;
}

static void execute(String sourceCode) {
	Code* code = mish::compile::compile(sourceCode);

	if (code != NULL) {
		newThread(code);
	} else {
		printShell();
	}
}

void keyboardHandler(EFI_INPUT_KEY keyEvent) {
	if (keyEvent.UnicodeChar > 0) {
		// if we have a thread running, don't
		if (currentThread != NULL) {
			if (keyEvent.UnicodeChar == 0x3) { // end of text (ctrl + C)
				mish::execute::schedule::kill(currentThread);
				currentThread = NULL;
			}
		} else {
			if (keyEvent.UnicodeChar == 0xD) { // carriage return
				screen_terminal_writeString("\n\r");
				strchar* sourceCode = (strchar*) create(
						line.size() * sizeof(strchar) + 1);

				Iterator<strchar> stringIterator = line.iterator();
				uint64 strIndex = 0;
				while (stringIterator.hasNext()) {
					sourceCode[strIndex] = stringIterator.next();
					strIndex++;
				}
				sourceCode[strIndex] = NULL; // null terminate
				line.clear();

				execute(sourceCode);
				delete sourceCode;
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
}

void console() {
	eventHandler_keyboard = keyboardHandler;

	// convert to 16-bit string
	uint64 charCount = &mishEnd - &mishStart;
	strchar* sourceCode = (strchar*) create(charCount * sizeof(strchar) + 1);
	for (uint64 i = 0; i < charCount; i++) {
		sourceCode[i] = (strchar) ((&mishStart)[i]);
	}
	sourceCode[charCount] = 0; // NULL terminate

	execute(sourceCode);
	delete sourceCode;
}
