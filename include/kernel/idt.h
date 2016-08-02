/*
 * idt.h
 *
 *  Created on: May 12, 2016
 *      Author: chris
 */

#ifndef KERNEL_IDT_H_
#define KERNEL_IDT_H_

#include <int.h>

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

typedef struct regs {
	unsigned int gs, fs, es, ds; /* pushed the segs last */
	unsigned int edi, esi, ebp, useless_value, ebx, edx, ecx, eax; /* pushed by pusha. useless value is esp */
	unsigned int int_no, err_code; /* our 'push byte #' and ecodes do this */
	unsigned int eip, cs, eflags, useresp, ss; /* pushed by the processor automatically */
} registers_t;

typedef void (*interrupt_handler_t)(registers_t *);
void register_interrupt_handler(uint8 n, interrupt_handler_t h); //TODO: Rename to a more x86 specific name
void deregister_interrupt_handler(uint8 n);

void init_idt();

#endif /* KERNEL_IDT_H_ */
