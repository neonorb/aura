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

extern "C" EFIAPI EFI_STATUS efi_main(EFI_HANDLE ImageHandle,
		EFI_SYSTEM_TABLE* _systemTable) {
	systemTable = _systemTable;

	kernel_main();

	EFI_PHYSICAL_ADDRESS location;
	EFI_STATUS status = uefi_call_wrapper(
			(void* ) systemTable->BootServices->AllocatePages, 4,
			AllocateAnyPages, EfiLoaderData, 16, &location);

	if (status == EFI_SUCCESS) {
		while (true) {
		}
	}

	return status;
}
