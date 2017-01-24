/*
 * uefi.cpp
 *
 *  Created on: Jan 24, 2017
 *      Author: chris13524
 */

#include <modules/power/uefi.h>
#include <boot/uefi.h>

void uefi_powerOff() {
	EFI_STATUS status = uefi_call_wrapper(
			(void* ) systemTable->RuntimeServices->ResetSystem, 4,
			EfiResetShutdown, EFI_SUCCESS, 0, NULL);
	if (status != EFI_SUCCESS) {
		debug("EFI_STATUS", (uint64) status);
		crash("failed to uefi_powerOff()");
	}
}

void uefi_reset() {
	EFI_STATUS status = uefi_call_wrapper(
			(void* ) systemTable->RuntimeServices->ResetSystem, 4,
			EfiResetCold, EFI_SUCCESS, 0, NULL);
	if (status != EFI_SUCCESS) {
		debug("EFI_STATUS", (uint64) status);
		crash("failed to uefi_reset()");
	}
}
