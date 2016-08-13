/*
 * uefi.h
 *
 *  Created on: Aug 12, 2016
 *      Author: chris13524
 */

#ifndef INCLUDE_MODULES_SCREEN_UEFI_H_
#define INCLUDE_MODULES_SCREEN_UEFI_H_

#include <string.h>
#include <boot/uefi.h>

void uefi_terminal_clear();
void uefi_terminal_writeString(String message);
void uefi_terminal_setForegroundColor(UINTN color);
void uefi_terminal_setBackgroundColor(UINTN color);

#endif /* INCLUDE_MODULES_SCREEN_UEFI_H_ */
