/* idt.c - Sets up the IDT */
/* Copyright (C) 2016  Chris Smith */

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

#include <kernel/idt.h>

#include <int.h>
#include <kernel/ports.h>
#include <log.h>
#include <utils/utils.h>
#include <memory.h>

/** IDT gates stored in this table */
idt_gate_t idt_entries[256] __attribute__((aligned(16)));
/** ISR function vectors stored in this table */
isr_t interrupt_handlers[256] __attribute__((aligned(16)));
/** IDT PTR stored here */
idt_ptr_t idt_ptr __attribute__((aligned(16)));

extern "C" void idt_flush(idt_ptr_t* ptr);

/**
 * Sets the IDT gate to function vector.
 *
 * gn supplies numbered interrupt, while funcall is
 * function to be called when that interrupt happens.
 */
void idt_set_gate(uint8 gn, uint64 funcall) {
	idt_gate_t* gate = &idt_entries[gn];
	gate->offset015 = funcall & 0xFFFF;
	gate->offset1631 = (funcall >> 16) & 0xFFFF;
	gate->offset3263 = (funcall >> 32) & 0xFFFFFFFF;
	gate->selector = 8; // CODE descriptor, see GDT64.Code
	gate->flags.p = 1;
	gate->flags.ist = 0;
	gate->flags.type = 14 & 0b1111;
	gate->flags.dpl = 0;

	if (gn == 14) // page fault
		gate->flags.ist = 1;
	if (gn == 8) // double fault
		gate->flags.ist = 2;
	if (gn == 255) // ipi fault
		gate->flags.ist = 3;
}

/**
 * Initializes base interrupt vectors.
 *
 * Creates IDT pointer and then fills it up with generated
 * asm functions. These functions, by default, call ISR vector,
 * or kernel panics if ISR vector is unavailable.
 *
 * Also remaps IRQ to use interrupts.
 */
void initialize_interrupts() {

	memset((uint8*) idt_entries, 0, sizeof(idt_entries));
	memset((uint8*) interrupt_handlers, 0, sizeof(interrupt_handlers));

	idt_ptr.limit = (sizeof(idt_entries)) - 1;
	idt_ptr.base = (uint64) &idt_entries;

	// GENERAL CPU INTERRUPTS
	idt_set_gate(0, (uint64) isr0);
	idt_set_gate(1, (uint64) isr1);
	idt_set_gate(2, (uint64) isr2);
	idt_set_gate(3, (uint64) isr3);
	idt_set_gate(4, (uint64) isr4);
	idt_set_gate(5, (uint64) isr5);
	idt_set_gate(6, (uint64) isr6);
	idt_set_gate(7, (uint64) isr7);
	idt_set_gate(8, (uint64) isr8);
	idt_set_gate(9, (uint64) isr9);

	idt_set_gate(10, (uint64) isr10);
	idt_set_gate(11, (uint64) isr11);
	idt_set_gate(12, (uint64) isr12);
	idt_set_gate(13, (uint64) isr13);
	idt_set_gate(14, (uint64) isr14);
	idt_set_gate(15, (uint64) isr15);
	idt_set_gate(16, (uint64) isr16);
	idt_set_gate(17, (uint64) isr17);
	idt_set_gate(18, (uint64) isr18);
	idt_set_gate(19, (uint64) isr19);

	idt_set_gate(20, (uint64) isr20);
	idt_set_gate(21, (uint64) isr21);
	idt_set_gate(22, (uint64) isr22);
	idt_set_gate(23, (uint64) isr23);
	idt_set_gate(24, (uint64) isr24);
	idt_set_gate(25, (uint64) isr25);
	idt_set_gate(26, (uint64) isr26);
	idt_set_gate(27, (uint64) isr27);
	idt_set_gate(28, (uint64) isr28);
	idt_set_gate(29, (uint64) isr29);

	idt_set_gate(30, (uint64) isr30);
	idt_set_gate(31, (uint64) isr31);
	idt_set_gate(255, (uint64) isr255);

	for (unsigned int i = 32; i < 47; i++)
		IRQ_set_mask(i);

	// INTERRUPTS FROM THE BOARD
	// remapping interrupts from irq
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);

	idt_set_gate(32, (uint64) isr32);
	idt_set_gate(33, (uint64) isr33);
	idt_set_gate(34, (uint64) isr34);
	idt_set_gate(35, (uint64) isr35);
	idt_set_gate(36, (uint64) isr36);
	idt_set_gate(37, (uint64) isr37);
	idt_set_gate(38, (uint64) isr38);
	idt_set_gate(39, (uint64) isr39);
	idt_set_gate(40, (uint64) isr40);
	idt_set_gate(41, (uint64) isr41);
	idt_set_gate(42, (uint64) isr42);
	idt_set_gate(43, (uint64) isr43);
	idt_set_gate(44, (uint64) isr44);
	idt_set_gate(45, (uint64) isr45);
	idt_set_gate(46, (uint64) isr46);
	idt_set_gate(47, (uint64) isr47);

	idt_flush(&idt_ptr);

	IRQ_clear_mask(32);
}

/**
 * Clears interrupt state after interrupt was handled.
 */
void pic_sendeoi(int irq) {
	switch (irq) {
	case PIC_EOI_MASTER: {
		outb(PIC1_COMMAND, 0x20);
		break;
	}
	case PIC_EOI_SLAVE: {
		outb(PIC2_COMMAND, 0x20);
		break;
	}
	case PIC_EOI_ALL: {
		pic_sendeoi(PIC_EOI_MASTER);
		pic_sendeoi(PIC_EOI_SLAVE);
		break;
	}
	default: {
		if (irq >= 8)
			outb(PIC2_COMMAND, 0x20);
		outb(PIC1_COMMAND, 0x20);
		break;
	}
	}
}

/**
 * ISR common handler.
 *
 * Called by interrupt vector. Registers are editable state of
 * CPU before interrupt.
 */
extern "C" void isr_handler(registers_t* r) {
	log(L"interrupt!");
	if (interrupt_handlers[r->type] == 0) {
		log(L"not interrupt handler");
		//error(ERROR_NO_IV_FOR_INTERRUPT, r->type, r->ecode, &r);
	} else {
		interrupt_handlers[r->type](r->ecode, r);
	}

	if (r->type > 31 && r->type < 48) {
		if (r->type >= 40 && r->type != 47)
			pic_sendeoi(PIC_EOI_SLAVE);
		if (r->type != 39)
			pic_sendeoi(PIC_EOI_MASTER);
	} else if (r->type == 255) {
		// ipi interrupt
		//volatile uint32* eoi = (uint32*) physical_to_virtual(apicaddr + 0xB0);
		//*eoi = 0;
	}
}

/**
 * Registers ISR vector for interrupt.
 *
 * Will overwrite interrupt handler set up before.
 */
void register_interrupt_handler(uint8 interrupt_id, isr_t handler_func) {
	interrupt_handlers[interrupt_id] = handler_func;
}

void IRQ_set_mask(unsigned char IRQline) {
	uint16 port;
	uint8 value;

	if (IRQline < 8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
		IRQline -= 8;
	}
	value = inb(port) | (1 << IRQline);
	outb(port, value);
}

void IRQ_clear_mask(unsigned char IRQline) {
	uint16 port;
	uint8 value;

	if (IRQline < 8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
		IRQline -= 8;
	}
	value = inb(port) & ~(1 << IRQline);
	outb(port, value);
}
