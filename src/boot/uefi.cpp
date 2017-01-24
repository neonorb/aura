/*
 * uefi.cpp
 *
 *  Created on: Aug 6, 2016
 *      Author: chris13524
 */

#include <boot/uefi.h>
#include <kernel/kernel.h>
#include <feta.h>

EFI_SYSTEM_TABLE* systemTable;
EFI_LOADED_IMAGE* loadedImage;

// see the UEFI specifications for more information: http://www.uefi.org/sites/default/files/resources/UEFI%20Spec%202_6.pdf

extern "C" EFIAPI EFI_STATUS efi_main(EFI_HANDLE ImageHandle,
		EFI_SYSTEM_TABLE* _systemTable) {
	systemTable = _systemTable;
	InitializeLib(ImageHandle, systemTable);

	EFI_STATUS status = uefi_call_wrapper(
			(void* ) systemTable->BootServices->HandleProtocol, 3, ImageHandle,
			&LoadedImageProtocol, (void** )&loadedImage);

	if (status != EFI_SUCCESS) {
		crash("error while checking image base");
	}

	// disable the watchdog timer - if this is enabled, the firmware will reset the system after 5 minutes
	EFI_STATUS watchdogStatus = uefi_call_wrapper(
			(void* ) systemTable->BootServices->SetWatchdogTimer, 4, 0, 0, 0,
			NULL);
	if (watchdogStatus != EFI_SUCCESS) {
		crash("error while disabling the watchdog timer");
	}

#ifdef DEBUGGING
	Print((CHAR16*) L"Image base: 0x%lx\n", loadedImage->ImageBase);
	int wait = 1;
	while (wait) {
		__asm__ __volatile__("pause");
	}
#endif
	kernel_main(loadedImage->ImageBase);

	/*uinteger memoryMapSize = 4096;
	EFI_MEMORY_DESCRIPTOR memoryMap[4096];
	uinteger mapKey;
	uinteger descriptorSize;
	uint32 descriptorVersion;

	debug("getting memory map");
	EFI_STATUS getMemoryMapStatus = uefi_call_wrapper(
			(void* ) systemTable->BootServices->GetMemoryMap, 5, &memoryMapSize,
			memoryMap, &mapKey, &descriptorSize, &descriptorVersion);
	if (getMemoryMapStatus != EFI_SUCCESS) {
		crash("error while getting memory map");
	}

	debug("exiting boot services");
	EFI_STATUS exitBootServicesStatus = uefi_call_wrapper(
			(void* ) systemTable->BootServices->ExitBootServices, 2,
			ImageHandle, mapKey);
	if (exitBootServicesStatus != EFI_SUCCESS) {
		crash("error while exiting boot services");
	}
	debug("init ACPI");
	int initAcpiStatus = initAcpi();
	if (initAcpiStatus == 0) {
		debug("ACPI power off");
		acpiPowerOff();
	} else {
		debug("error while initializing ACPI");
	}*/

	return EFI_SUCCESS;
}

void stallMilli(uinteger milliseconds) {
	stall(milliseconds * 1000);
}

void stall(uinteger microseconds) {
	EFI_STATUS status = uefi_call_wrapper(
			(void* )systemTable->BootServices->Stall, 1, microseconds);

	if (status != EFI_SUCCESS) {
		crash("error while stalling");
	}
}
