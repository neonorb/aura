/*
 * uefi.cpp
 *
 *  Created on: Aug 12, 2016
 *      Author: chris13524
 */

#include <string.h>
#include <modules/screen/uefi.h>
#include <log.h>

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
	EFI_STATUS status = uefi_call_wrapper(
			(void* ) systemTable->ConOut->OutputString, 2, systemTable->ConOut,
			message);
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
