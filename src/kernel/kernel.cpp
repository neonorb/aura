/* kernel.cpp - The kernel of project asiago, it's the heart the OS! */
/* Copyright (C) 2016   Chris Smith */

/* This program is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or */
/* (at your option) any later version. */

/* This program is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/* GNU General Public License for more details. */

/* You should have received a copy of the GNU General Public License */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <kernel/kernel.h>
#include <feta.h>
#include <kernel/liballoc.h>

#include <boot/uefi.h>

#include <utils/utils.h>

#include <modules/modules.h>

#include <implementation/implementation.h>

using namespace feta;

#include <boot/uefi.h>
void kernel_main() {
	cli();
	modules_init();
	sti();

	log("boot complete");

	implementation();

	// hang kernel
	log("kernel exiting");
	hang();
}

void crash(String message) {
	fault(message);
	fault("halting system!");

	hang();
}

void hang() {
	cli();
	hlt();
}
