/*
 * uefi.h
 *
 *  Created on: Aug 11, 2016
 *      Author: chris13524
 */

#ifndef INCLUDE_BOOT_UEFI_H_
#define INCLUDE_BOOT_UEFI_H_

#include <feta.h>

using namespace feta;

extern "C" {
#include <efi.h>
#include <efilib.h>
}

extern EFI_SYSTEM_TABLE* systemTable;

void stallMilli(uinteger milliseconds);
void stall(uinteger microseconds);

#endif /* INCLUDE_BOOT_UEFI_H_ */
