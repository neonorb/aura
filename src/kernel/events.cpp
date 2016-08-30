/*
 * events.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: chris13524
 */

#include <kernel/events.h>

#include <memory.h>

KeyboardHandler eventHandler_keyboard;

void fireKeyboardEvent(EFI_INPUT_KEY keyEvent) {
	if(eventHandler_keyboard != NULL) {
		eventHandler_keyboard(keyEvent);
	}
}
