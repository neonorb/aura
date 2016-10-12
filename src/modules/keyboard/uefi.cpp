/*
 * uefi.cpp
 *
 *  Created on: Aug 13, 2016
 *      Author: chris13524
 */

#include <modules/keyboard/uefi.h>

#include <modules/keyboard/keyboard.h>
#include <boot/uefi.h>

#include <log.h>

static EFI_GUID stip_guid = EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL_GUID;
static EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL* stip;

/*
 static EFI_KEY_DATA keyData;
 static EFI_STATUS keyNotify(EFI_KEY_DATA data) {

 return EFI_SUCCESS;
 }*/

void uefi_keyboard_init() {
	EFI_STATUS status = uefi_call_wrapper(
			(void* ) systemTable->BootServices->LocateProtocol, 3, &stip_guid,
			NULL, &stip);
	if (status != EFI_SUCCESS) {
		debug("EFI_STATUS", (uint64) status);
		crash("failed to initialize keyboard");
	}

	/* EFI_STATUS registerStatus = uefi_call_wrapper((void* ) stip, 3, &keyData,
	 &keyNotify, NULL);

	 if (registerStatus != EFI_SUCCESS) {
	 debug(L"EFI_STATUS", status);
	 crash(L"failed to register keyboard notify");
	 }*/

	//EFI_STATUS setStateStatus = uefi_call_wrapper((void* ) stip->SetState, 2, &stip, EFI_KEY_STATE_EXPOSED);
}

void uefi_keyboard_probe() {
	EFI_INPUT_KEY key;
	EFI_STATUS status = uefi_call_wrapper(
			(void* ) systemTable->ConIn->ReadKeyStroke, 2, systemTable->ConIn,
			&key);
	if (!(status == EFI_SUCCESS || status == EFI_NOT_READY)) {
		debug("EFI_STATUS", (uint64) status);
		crash("failed to initialize keyboard");
	}

	if (status == EFI_SUCCESS) {
		keyboard_keyUpdate(key);
	}

	// no keystroke
}
