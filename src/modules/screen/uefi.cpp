/*
 * uefi.cpp
 *
 *  Created on: Aug 12, 2016
 *      Author: chris13524
 */

#include <feta.h>
#include <modules/screen/uefi.h>

static UINTN foreground;
static UINTN background;

void uefi_terminal_init() {
	uefi_terminal_setCursorVisibility(true);
}

void uefi_terminal_setCursorVisibility(bool cursorVisibility) {
	EFI_STATUS status = uefi_call_wrapper(
			(void* ) systemTable->ConOut->EnableCursor, 2, systemTable->ConOut,
			cursorVisibility);

	if (status != EFI_SUCCESS) {
		debug("EFI_STATUS", (uint64) status);
		crash("failed to set cursor visibility");
	}
}

void uefi_terminal_clear() {
	EFI_STATUS status = uefi_call_wrapper(
			(void* ) systemTable->ConOut->ClearScreen, 1, systemTable->ConOut);

	if (status != EFI_SUCCESS) {
		debug("EFI_STATUS", (uint64) status);
		crash("failed to clear terminal");
	}
}

void uefi_terminal_writeString(String message) {
	uint16* output;
	bool didAllocate = false;
	if (sizeof(strchar) == 1) {
		uinteger length = stringlength(message);

		// create
		output = (uint16*) create(sizeof(uint16) * (length + 1));
		didAllocate = true;

		// copy
		for (uinteger i = 0; i < length; i++) {
			output[i] = (uint16) (message[i]);
		}

		// null terminate
		output[length] = '\0';
	} else if (sizeof(strchar) == 2) {
		output = (uint16*) message;
	} else {
		debug("unexpected strchar size");
		crash("unexpected strchar size"); // this will probably go in a crash loop
	}

	EFI_STATUS status = uefi_call_wrapper(
			(void* ) systemTable->ConOut->OutputString, 2, systemTable->ConOut,
			output);

	if (didAllocate) {
		destroy(output);
	}

	if (status != EFI_SUCCESS) {
		debug("EFI_STATUS", (uint64) status);
		crash("failed to write string");
	}
}

uint64 uefi_terminal_cursorColumn() {
	return systemTable->ConOut->Mode->CursorColumn;
}
uint64 uefi_terminal_cursorRow() {
	return systemTable->ConOut->Mode->CursorRow;
}

static void updateColor() {
	EFI_STATUS status = uefi_call_wrapper(
			(void* ) systemTable->ConOut->SetAttribute, 2, systemTable->ConOut,
			EFI_TEXT_ATTR(foreground,background));

	if (status != EFI_SUCCESS) {
		debug("EFI_STATUS", (uint64) status);
		crash("failed to set terminal color");
	}
}

void uefi_terminal_setForegroundColor(UINTN color) {
	foreground = color;
	updateColor();
}

void uefi_terminal_setBackgroundColor(UINTN color) {
	background = color;
	updateColor();
}
