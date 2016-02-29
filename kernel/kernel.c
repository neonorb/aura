#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
 
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

#include "../modules/screen/vga.h"
#include "../modules/keyboard/ps2.h"


void do_interrupts(){
	terminal_writestring("doing interrupts");
}

struct IDTDescr{ // http://wiki.osdev.org/Interrupt_Descriptor_Table#Structure
   uint16_t offset_1; // offset bits 0..15
   uint16_t selector; // a code segment selector in GDT or LDT
   uint8_t zero;      // unused, set to 0
   uint8_t type_attr; // type and attributes, see below
   uint16_t offset_2; // offset bits 16..31
};

struct IDT{ // http://wiki.osdev.org/Interrupt_Descriptor_Table#Location_and_Size
	uint16_t limit; // Defines the length of the IDT in bytes - 1 (minimum value is 100h, a value of 1000h means 200h interrupts).
	uint32_t base; // This 32 bits are the linear address where the IDT starts (INT 0)
};

#define PIC_EOI		0x20		/* End-of-interrupt command code */

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

void PIC_sendEOI(unsigned char irq)
{
	if(irq >= 8)
		outb(PIC2_COMMAND,PIC_EOI);

	outb(PIC1_COMMAND,PIC_EOI);
}

void handle_keyboard_interrupt(){
	// test function, this will eventually be replaced with more intelligent code
	terminal_writestring("keyboard interrupt!");
	PIC_sendEOI(1); // maybe 33 instead of 1?

	// TODO probably have to do iret or something here
}

struct IDTDescr idt_table[34];
void initialize_idt_table(){
	// keyboard
	terminal_writestring((const char *)handle_keyboard_interrupt);
	// first 32 are exceptions, #1 (32) is Programmable Interrupt Timer Interrupt, #2 (33) is Keyboard Interrupt - http://wiki.osdev.org/Interrupts#Interrupt_Overview
	idt_table[33] = (struct IDTDescr) {
		(uintptr_t)&handle_keyboard_interrupt, // lower-half of pointer
		33 << 3 | 0b00000001, // selector - http://wiki.osdev.org/Selector
		0, // always zero
		0b01110001, // no idea what I'm doing here
		((uint32_t)&handle_keyboard_interrupt) >> 16 // upper-half of pointer
	};

	// tell CPU about descriptor table
	struct IDT idt = {sizeof(idt_table) - 1, (int)&idt_table};
	asm ("lidt (%0)" :: ""(idt));
}

void IRQ_clear_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);
}

static inline void io_wait(void)
{
    /* TODO: This is probably fragile. */
    asm volatile ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void PIC_remap(int offset1, int offset2)
{
	unsigned char a1, a2;

	a1 = inb(PIC1_DATA);                        // save masks
	a2 = inb(PIC2_DATA);

	outb(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();

	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	outb(PIC1_DATA, a1);   // restore saved masks.
	outb(PIC2_DATA, a2);
}

void kernel_main() {
	/* Initialize terminal interface */
	terminal_initialize();

	PIC_remap(0x20, 0x28); // http://wiki.osdev.org/PIC#Protected_Mode
	initialize_idt_table(); // interesting, when commenting this out, the funky characters aren't printed out at the start of the console
	IRQ_clear_mask(33);

	terminal_writestring("Pressing a key should print out:\nkeyboard interrupt!");

	while(true){}

	/*while(true){
		char c = getchar();
		if(c > 0){
			terminal_putchar(c);
		}
	}*/
}
