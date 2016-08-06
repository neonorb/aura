/*
 * uefi.cpp
 *
 *  Created on: Aug 6, 2016
 *      Author: chris13524
 */

#include <efi.h>
#include <efilib.h>

EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	InitializeLib(ImageHandle, SystemTable);

	while (1) {
		Print(L"Hello, world!\n");
	}

	return EFI_SUCCESS;
}
