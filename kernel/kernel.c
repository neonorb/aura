#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint-gcc.h>

/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

#include "../utils/utils.h"

#include "log.c"

#include "gdt.c"
#include "idt.c"

#include "../modules/clock/clock.c"
#include "../modules/screen/screen.c"
#include "../modules/keyboard/keyboard.c"

#include "../implementation/implementation.c"


void kernel_main() {
	screen_terminal_initialize();

	log("Disabling interrupts");
	asm volatile ("cli");

	log("Setting up GDT");
	gdt_init();

	log("Setting up IDT");
	init_idt();

	log("Setting up clock");
	clock_initialize();

	log("Setting up keyboard");
	keyboard_initialize();

	//log("Initializing ACPI");
	//initAcpi();

	// we're ready to go, enable interrupts
	log("Enabling interrupts");
	asm volatile ("sti");

	log("Welcome to Aura!");

	implementation();

	fault("FUCK! We're at the end of kernel_main which shouldn't happen!"); // returning from here will clear interrupts, halt the system, and enter a jmp loop (boot.s)
}
