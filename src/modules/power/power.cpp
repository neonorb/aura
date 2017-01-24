/*
 * power.cpp
 *
 *  Created on: Jan 24, 2017
 *      Author: chris13524
 */

#include <modules/power/power.h>
#include <modules/power/uefi.h>

void power_powerOff() {
	uefi_powerOff();
}

void power_reset() {
	uefi_reset();
}
