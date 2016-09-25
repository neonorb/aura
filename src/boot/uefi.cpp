/*
 * uefi.cpp
 *
 *  Created on: Aug 6, 2016
 *      Author: chris13524
 */

#include <boot/uefi.h>
#include <kernel/kernel.h>
#include <log.h>

EFI_SYSTEM_TABLE* systemTable;
EFI_LOADED_IMAGE* imageBase;

extern "C" EFIAPI EFI_STATUS efi_main(EFI_HANDLE ImageHandle,
		EFI_SYSTEM_TABLE* _systemTable) {
	systemTable = _systemTable;

	EFI_STATUS status = uefi_call_wrapper(
			(void* ) systemTable->BootServices->HandleProtocol, 3, ImageHandle,
			&LoadedImageProtocol, (void** )&imageBase);

	if (status != EFI_SUCCESS) {
		crash(L"error while checking image base");
	}

#ifdef DEBUGGING
	debug(L"image base", (uint64) imageBase);
	int wait = 1;
	while (wait) {
		__asm__ __volatile__("pause");
	}
#endif

	// disable the watchdog timer - if this is enabled, the firmware will reset the system after 5 minutes
	EFI_STATUS watchdogStatus = uefi_call_wrapper((void*) systemTable->BootServices->SetWatchdogTimer, 4, 0, 0, 0, NULL);
	if(watchdogStatus != EFI_SUCCESS) {
		crash(L"error while disabling the watchdog timer");
	}

	kernel_main();

	return EFI_SUCCESS;
}

void stallMilli(uint64 milliseconds) {
	stall(milliseconds * 1000);
}

void stall(uint64 microseconds) {
	EFI_STATUS status = uefi_call_wrapper(
			(void* )systemTable->BootServices->Stall, 1, microseconds);

	if (status != EFI_SUCCESS) {
		crash(L"error while stalling");
	}
}
