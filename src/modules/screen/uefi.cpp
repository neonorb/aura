/*
 * uefi.cpp
 *
 *  Created on: Aug 12, 2016
 *      Author: chris13524
 */

#include <string.h>
#include <boot/uefi.h>

void uefi_terminal_clear() {
	uefi_call_wrapper((void*) systemTable->ConOut->ClearScreen, 1, systemTable->ConOut);
}

void uefi_terminal_writeString(String message) {
	uefi_call_wrapper((void*) systemTable->ConOut->OutputString, 2, systemTable->ConOut, message);
}
