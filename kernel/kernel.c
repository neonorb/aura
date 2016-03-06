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

#include "../modules/screen/vga.h"
#include "../modules/keyboard/keyboard.c"

void handler(KeyEvent keyEvent) {
	char c = keyboard_eventToChar(keyEvent);
	if (c > 0) {
		terminal_putchar(c);
	}
	/*char string[digitCount(e.type)];
	 toString(string, e.type);

	 char message[] = "Event type: ";

	 char result[sizeof(e.type) + sizeof(message)];
	 concat(result, message, string);

	 log(result);*/
}

void kernel_main() {
	/* Initialize terminal interface */
	terminal_initialize();

	log("Disabling interrupts");
	asm volatile ("cli");

	log("Setting up GDT");
	gdt_init();

	log("Setting up IDT");
	init_idt();

	log("Initializing keyboard");
	register_interrupt_handler(IRQ1, &keyboard_interrupt);
	registerKeyboardHandler(&handler);

	// we're ready to go, enable interrupts
	log("Enabling interrupts");
	asm volatile ("sti");

	log("Welcome to Aura!");

	while (true) {
		// this will eventually be replaced with more intelligent code
	}

	fault("FUCK! We're at the end of kernel_main which shouldn't happen!"); // returning from here will clear interrupts, halt the system, and enter a jmp loop (boot.s)
}
