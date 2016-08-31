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

	return EFI_SUCCESS;
}
