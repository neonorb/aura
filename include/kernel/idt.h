/*
 * idt.h
 *
 *  Created on: May 12, 2016
 *      Author: chris
 */

#ifndef KERNEL_IDT_H_
#define KERNEL_IDT_H_

#include <int.h>

struct idt_gate {
    uint16 offset015;
    uint16 selector;
    struct {
        uint16 ist  : 3; // interrupt stack table
        uint16 r0   : 1;
        uint16 r1   : 1;
        uint16 r2   : 3;
        uint16 type : 4;
        uint16 r3   : 1;
        uint16 dpl  : 2; // descriptor priviledge level
        uint16 p    : 1; // segment present flag
    } __attribute__((packed)) flags;
    uint16 offset1631;
    uint32 offset3263;
    uint32 reserved;
} __attribute__((packed));
typedef struct idt_gate idt_gate_t;

struct idt_ptr {
    uint16 limit;
    uint64 base;
}__attribute__((packed));
typedef struct idt_ptr idt_ptr_t;

void initialize_interrupts();

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

void initialize_interrupts();

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr32();
extern void isr33();
extern void isr34();
extern void isr35();
extern void isr36();
extern void isr37();
extern void isr38();
extern void isr39();
extern void isr40();
extern void isr41();
extern void isr42();
extern void isr43();
extern void isr44();
extern void isr45();
extern void isr46();
extern void isr47();

extern void isr255();

#define IRQ0  32
#define IRQ1  33
#define IRQ2  34
#define IRQ3  35
#define IRQ4  36
#define IRQ5  37
#define IRQ6  38
#define IRQ7  39
#define IRQ8  40
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

#define PIC1_LOCATION 0x20 /* Master PIC Address */
#define PIC2_LOCATION 0xA0 /* Slave PIC Address */
#define PIC1_COMMAND (PIC1_LOCATION+0) /* Master Command */
#define PIC2_COMMAND (PIC2_LOCATION+0) /* Slave Command */
#define PIC1_DATA (PIC1_LOCATION+1) /* Master Data */
#define PIC2_DATA (PIC2_LOCATION+1) /* Slave Data */
#define PIC_REMAP_BEG_OFFSET (PIC1_LOCATION+0) /* Remap Beginning Location */
#define PIC_REMAP_END_OFFSET (PIC1_LOCATION+8) /* Remap Ending Location */

#define PIC_EOI_MASTER 256
#define PIC_EOI_SLAVE (PIC_EOI_MASTER+1)
#define PIC_EOI_ALL (PIC_EOI_SLAVE +1)

typedef struct registers {
	uint64 r15, r14, r13, r12, r11, r10, r9, r8, rbp, rcx, rbx;
	uint64 rdx, rsi, rdi;
	uint64 es, ds, fs, rax;
	uint64 type, ecode;
	uint64 rip, cs, rflags, uesp, ss;
} registers_t;

/** Register callback function */
typedef void (*isr_t)(uint64 error_code, registers_t*);

/**
 * Registers ISR function for interrupt.
 */
void register_interrupt_handler(uint8 interrupt_id, isr_t handler_func);

void IRQ_set_mask(unsigned char IRQline);
void IRQ_clear_mask(unsigned char IRQline);

#endif /* KERNEL_IDT_H_ */
