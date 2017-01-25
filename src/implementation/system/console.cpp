/*
 * console.cpp
 *
 *  Created on: Sep 8, 2016
 *      Author: chris13524
 */

#include <implementation/system/console.h>

#include <mish.h>
#include <modules/modules.h>
#include <modules/screen/screen.h>
#include <kernel/events.h>

using namespace mish;

extern uint8 mishStart; // &mishStart - start of Mish code
extern uint8 mishEnd; // &mishEnd - end of Mish code

static void printShell() {
	// if we don't have a new line, print a character to indicate that
	if (screen_terminal_cursorColumn() != 0) {
		screen_terminal_setBackgroundColor(EFI_LIGHTGRAY);
		screen_terminal_setForegroundColor(EFI_BLACK);
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

uinteger cursor = 0;
List<strchar> line;
void keyboardHandler(EFI_INPUT_KEY keyEvent) {
	if (keyEvent.UnicodeChar > 0) {
		// if we have a thread running, don't
		if (keyEvent.UnicodeChar == 0x3) { // end of text (ctrl + C)
			if (currentThread != NULL) {
				mish::execute::schedule::kill(currentThread);
			} else {
				screen_terminal_writeString("\n\r");
				line.clear();
				cursor = 0;
				printShell();
			}
		} else if (keyEvent.UnicodeChar == 0xD) { // carriage return
			screen_terminal_writeString("\n\r");
			strchar* sourceCode = (strchar*) create(
					line.size() * sizeof(strchar) + 1);

			Iterator<strchar> stringIterator = line.iterator();
			uint64 strIndex = 0;
			while (stringIterator.hasNext()) {
				sourceCode[strIndex] = stringIterator.next();
				strIndex++;
			}
			sourceCode[strIndex] = '\0'; // null terminate
			line.clear();
			cursor = 0;

			::execute(sourceCode);
			delete sourceCode;
		} else if (keyEvent.UnicodeChar == 0x8) { // backspace
			if (cursor > 0) {
				screen_terminal_writeString(0x8);
				cursor--;
				line.remove(cursor);
				// update line
				uinteger i = cursor;
				for (; i < line.size(); i++) {
					screen_terminal_writeString(line.get(i));
				}
				screen_terminal_writeString(" ");
				// reset cursor
				screen_terminal_setCursorColumn(
						screen_terminal_cursorColumn()
								- (line.size() + 1 - cursor));
			}
		} else {
			line.add(cursor, keyEvent.UnicodeChar);
			// update line
			for (uinteger i = cursor; i < line.size(); i++) {
				screen_terminal_writeString(line.get(i));
			}
			cursor++;
			// reset cursor
			screen_terminal_setCursorColumn(
					screen_terminal_cursorColumn()
							- (line.size() - cursor));
		}
	} else if (keyEvent.ScanCode > 0) {
		if (keyEvent.ScanCode == 0x4) { // move cursor left
			if (cursor > 0) {
				cursor--;
				screen_terminal_setCursorColumn(
						screen_terminal_cursorColumn() - 1);
			}
		} else if (keyEvent.ScanCode == 0x3) { // move cursor right
			if (cursor < line.size()) {
				cursor++;
				screen_terminal_setCursorColumn(
						screen_terminal_cursorColumn() + 1);
			}
		} else if (keyEvent.ScanCode == 0x8) { // delete
			if (cursor < line.size()) {
				line.remove(cursor);
				// update line
				uinteger i = cursor;
				for (; i < line.size(); i++) {
					screen_terminal_writeString(line.get(i));
				}
				screen_terminal_writeString(" ");
				// reset cursor
				screen_terminal_setCursorColumn(
						screen_terminal_cursorColumn()
								- (line.size() + 1 - cursor));
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

	::execute(sourceCode);
	delete sourceCode;
}
