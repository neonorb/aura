/*
 * events.h
 *
 *  Created on: Aug 29, 2016
 *      Author: chris13524
 */

#ifndef INCLUDE_KERNEL_EVENTS_H_
#define INCLUDE_KERNEL_EVENTS_H_

#include <modules/keyboard/keyboard.h>
#include <boot/uefi.h>

typedef void (*KeyboardHandler)(EFI_INPUT_KEY);
extern KeyboardHandler eventHandler_keyboard;

void fireKeyboardEvent(EFI_INPUT_KEY keyEvent);

#endif /* INCLUDE_KERNEL_EVENTS_H_ */
