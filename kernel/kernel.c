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

#include "gdt.c"
#include "idt.c"

#include "../modules/screen/vga.h"
#include "../modules/keyboard/keyboard.c"

void log(const char* data) {
	terminal_writestring(data);
	terminal_writestring("\n");
}

void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

void handler(KeyEvent e){
	char s[] = {0};
	itoa(e.type, *s);
	log(*s);
}

void kernel_main() {
	/* Initialize terminal interface */
	terminal_initialize();

	log("disabling interrupts");
	asm volatile ("cli");

	log("setting up GDT");
	init_gdt();

	log("setting up IDT");
	init_idt();

	log("initializing keyboard");
	register_interrupt_handler(IRQ1, &keyboard_interrupt);
	registerKeyboardHandler(&handler);

	// we're ready to go, enable interrupts
	log("enabling interrupts");
	asm volatile ("sti");

	log("finished setting up");

	while (true) {
		// this will eventually be replaced with more intelligent code
	}

	log("FUCK! We're at the end of kernel_main which shouldn't happen!"); // returning from here will clear interrupts, halt the system, and enter a jmp loop (boot.s)
}
