/*
 * uefi.h
 *
 *  Created on: Aug 12, 2016
 *      Author: chris13524
 */

#ifndef INCLUDE_MODULES_SCREEN_UEFI_H_
#define INCLUDE_MODULES_SCREEN_UEFI_H_

#include <feta.h>
#include <boot/uefi.h>

void uefi_terminal_init();
void uefi_terminal_setCursorVisibility(bool cursorVisibility);
void uefi_terminal_clear();
void uefi_terminal_writeString(String message);
void uefi_terminal_setForegroundColor(UINTN color);
void uefi_terminal_setBackgroundColor(UINTN color);

uinteger uefi_terminal_cursorColumn();
uinteger uefi_terminal_cursorRow();
void uefi_terminal_setCursorColumn(uinteger column);
void uefi_terminal_setCursorRow(uinteger row);

#endif /* INCLUDE_MODULES_SCREEN_UEFI_H_ */
