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

List<strchar> line;
uinteger cursor = 0;

List<List<strchar>> history;
integer historyIndex = -1;

static void moveCursor(integer distance) {
	uinteger currentColumn = screen_terminal_cursorColumn();
	screen_terminal_setCursorColumn(currentColumn + distance);
	cursor += distance;
}

static void writeConsole(strchar c) {
	screen_terminal_writeString(c);
	cursor++;
}
static void writeConsole(String string) {
	writeConsole(string[0]);
}

static void setCommand(List<strchar>& command) {
	uinteger currentCommandLength = line.size();
	uinteger newCommandLength = command.size();
	// delete extra letters
	if (newCommandLength < currentCommandLength) {
		moveCursor(-(currentCommandLength - newCommandLength));
		for (; cursor < currentCommandLength;) {
			writeConsole(" ");
		}
	}
	// move to beginning & clear
	moveCursor(-cursor);
	line.clear();
	// write new command
	while (cursor < newCommandLength) {
		strchar c = command.get(cursor);
		line.add(c);
		writeConsole(c);
	}
}

static Boolean linesEqual(List<strchar>& one, List<strchar>& two) {
	Iterator<strchar> oneIterator = one.iterator();
	Iterator<strchar> twoIterator = two.iterator();

	while (oneIterator.hasNext() && twoIterator.hasNext()) {
		if (oneIterator.next() != twoIterator.next()) {
			return false;
		}
	}
	return oneIterator.hasNext() == twoIterator.hasNext();
}

void keyboardHandler(EFI_INPUT_KEY keyEvent) {
	if (keyEvent.UnicodeChar > 0) {
		if (keyEvent.UnicodeChar == 0x3) { // end of text (ctrl + C)
			if (currentThread != NULL) {
				mish::execute::schedule::kill(currentThread);
			} else {
				screen_terminal_writeString("\n\r");
				historyIndex = -1;
				line.clear();
				cursor = 0;

				printShell();
			}
		} else {
			// if we have a thread going, ignore key stroke
			if (mish::execute::schedule::threadCount() > 0) {
				return;
			}

			if (keyEvent.UnicodeChar == 0xD) { // carriage return
				// construct source code from line
				String sourceCode = charListToString(&line);

				// if not blank & different from previous command, then add it to the history
				if (line.size() > 0
						&& (history.size() == 0 ?
								true : !linesEqual(line, history.get(0)))) {
					history.add(0, line);
				}

				// write to screen and update variables
				screen_terminal_writeString("\n\r");
				historyIndex = -1;
				line.clear();
				cursor = 0;

				// execute
				::execute(sourceCode);
				// delete code
				delete sourceCode;
			} else if (keyEvent.UnicodeChar == 0x8) { // backspace
				if (cursor > 0) {
					screen_terminal_writeString(0x8);
					cursor--;
					line.remove(cursor);
					historyIndex = -1;
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
				historyIndex = -1;
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
		} else if (keyEvent.ScanCode == 0x1) { // move cursor up
			if ((unsigned) historyIndex + 1 < history.size()) {
				historyIndex++;
				setCommand(history.get(historyIndex));
			}
		} else if (keyEvent.ScanCode == 0x2) { // move cursor down
			if (historyIndex > -1) {
				historyIndex--;
				if (historyIndex == -1) {
					List<strchar> emptyList;
					setCommand(emptyList);
				} else {
					setCommand(history.get(historyIndex));
				}
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

	// convert encoding of string if necessary
	uint64 charCount = &mishEnd - &mishStart;
	strchar* sourceCode = (strchar*) create(charCount * sizeof(strchar) + 1);
	for (uint64 i = 0; i < charCount; i++) {
		sourceCode[i] = (strchar) ((&mishStart)[i]);
	}
	sourceCode[charCount] = 0; // NULL terminate

	::execute(sourceCode);
	delete sourceCode;
}
