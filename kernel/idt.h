/*
 * idt.h
 *
 *  Created on: May 12, 2016
 *      Author: chris
 */

#ifndef KERNEL_IDT_H_
#define KERNEL_IDT_H_

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

struct idt_entry {
	unsigned short base_lo;
	unsigned short sel; /* Our kernel segment goes here! */
	unsigned char always0; /* This will ALWAYS be set to 0! */
	unsigned char flags; /* Set using the above table! */
	unsigned short base_hi;
}__attribute__((packed));

struct idt_ptr {
	unsigned short limit;
	unsigned int base;
}__attribute__((packed));

typedef struct regs {
	unsigned int gs, fs, es, ds; /* pushed the segs last */
	unsigned int edi, esi, ebp, useless_value, ebx, edx, ecx, eax; /* pushed by pusha. useless value is esp */
	unsigned int int_no, err_code; /* our 'push byte #' and ecodes do this */
	unsigned int eip, cs, eflags, useresp, ss; /* pushed by the processor automatically */
} registers_t;

#endif /* KERNEL_IDT_H_ */
