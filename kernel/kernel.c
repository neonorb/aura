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

#include "../modules/screen/vga.h"
#include "../modules/keyboard/keyboard.c"

void log(const char* data) {
	terminal_writestring(data);
	terminal_writestring("\n");
}

#define DO_DEBUG_LOGGING 1

#define LOG_COMPLETE	1
#define LOG_FAIL		2
#define LOG_WARN		3
#define LOG_INFO		4
#define LOG_DEBUG		5
#define LOG_INTERNALS	6
#define LOG_UNKN		0
// Low level
void log_low(const char * str);
void low_printname();
void text_console_init();
// High level
void printk(int severity, char * type, const char *fmt, ...);
void logging_printbestunit(uint32_t bytes, uint8_t newline);

void printk(int severity, char * type, const char *fmt, ...) {
	/*switch(severity) {
	 case LOG_COMPLETE:
	 text_console_change_color(0xA);
	 break;
	 case LOG_FAIL:
	 text_console_change_color(0x4);
	 break;
	 case LOG_WARN:
	 text_console_change_color(0xE);
	 break;
	 case LOG_INFO:
	 text_console_change_color(0xB);
	 break;
	 case LOG_DEBUG:
	 text_console_change_color(0xD);
	 break;
	 case LOG_INTERNALS:
	 text_console_change_color(0xD);
	 break;
	 case LOG_UNKN:
	 default: // LOG_UNKN
	 text_console_change_color(0x8);
	 break;
	 }
	 printf("%s: ",type);
	 text_console_reset_color();
	 va_list args;
	 va_start(args, fmt);
	 vprintf(fmt,args);
	 va_end(args);*/
	log(fmt);

}

// Write len copies of val into dest.
void memset(uint8_t *dest, uint8_t val, uint32_t len) {
	uint8_t *temp = (uint8_t *) dest;
	for (; len != 0; len--)
		*temp++ = val;
}

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
extern void isr100();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

struct gdt_entry {
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
}__attribute__((packed));

struct gdt_ptr {
	unsigned short limit;
	unsigned int base;
}__attribute__((packed));

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

struct tss_entry_struct {
	uint32_t prev_tss; // The previous TSS - if we used hardware task switching this would form a linked list.
	uint32_t esp0;   // The stack pointer to load when we change to kernel mode.
	uint32_t ss0;    // The stack segment to load when we change to kernel mode.
	uint32_t esp1;       // Unused...
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;     // The value to load into ES when we change to kernel mode.
	uint32_t cs;     // The value to load into CS when we change to kernel mode.
	uint32_t ss;     // The value to load into SS when we change to kernel mode.
	uint32_t ds;     // The value to load into DS when we change to kernel mode.
	uint32_t fs;     // The value to load into FS when we change to kernel mode.
	uint32_t gs;     // The value to load into GS when we change to kernel mode.
	uint32_t ldt;        // Unused...
	uint16_t trap;
	uint16_t iomap_base;
}__attribute__((packed));

typedef struct tss_entry_struct tss_entry_t;

void gdt_set_gate(signed int num, uint32_t base, uint32_t limit, uint8_t access,
		uint8_t gran);
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel,
		unsigned char flags);
extern void gdt_flush();
extern void idt_flush();
extern void tss_flush();
typedef void (*interrupt_handler_t)(registers_t *);
void register_interrupt_handler(uint8_t n, interrupt_handler_t h); //TODO: Rename to a more x86 specific name
void deregister_interrupt_handler(uint8_t n);

struct gdt_entry gdt_entries[6];
struct gdt_ptr gdt_ptr;
struct idt_entry idt_entries[256];
struct idt_ptr idt_ptr;
tss_entry_t tss_entry;

///Sets a gate in the GDT
void gdt_set_gate(signed int num, uint32_t base, uint32_t limit, uint8_t access,
		uint8_t gran) {
	gdt_entries[num].base_low = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high = (base >> 24) & 0xFF;

	gdt_entries[num].limit_low = (limit & 0xFFFF);
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;

	gdt_entries[num].granularity |= gran & 0xF0;
	gdt_entries[num].access = access;
}

static void write_tss(int32_t num, uint32_t ss0, uint32_t esp0) {
	uint32_t base = (uint32_t) &tss_entry;
	uint32_t limit = base + sizeof(tss_entry);

	gdt_set_gate(num, base, limit, 0xE9, 0x00);

	memset(&tss_entry, 0, sizeof(tss_entry));

	tss_entry.ss0 = ss0;
	tss_entry.esp0 = esp0;

	tss_entry.cs = 0x0b;
	tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs =
			0x13;
}

///Inits interrupt services
void idt_init_isrs() {
	idt_set_gate(0, (unsigned) isr0, 0x08, 0x8E);
	idt_set_gate(1, (unsigned) isr1, 0x08, 0x8E);
	idt_set_gate(2, (unsigned) isr2, 0x08, 0x8E);
	idt_set_gate(3, (unsigned) isr3, 0x08, 0x8E);
	idt_set_gate(4, (unsigned) isr4, 0x08, 0x8E);
	idt_set_gate(5, (unsigned) isr5, 0x08, 0x8E);
	idt_set_gate(6, (unsigned) isr6, 0x08, 0x8E);
	idt_set_gate(7, (unsigned) isr7, 0x08, 0x8E);

	idt_set_gate(8, (unsigned) isr8, 0x08, 0x8E);
	idt_set_gate(9, (unsigned) isr9, 0x08, 0x8E);
	idt_set_gate(10, (unsigned) isr10, 0x08, 0x8E);
	idt_set_gate(11, (unsigned) isr11, 0x08, 0x8E);
	idt_set_gate(12, (unsigned) isr12, 0x08, 0x8E);
	idt_set_gate(13, (unsigned) isr13, 0x08, 0x8E);
	idt_set_gate(14, (unsigned) isr14, 0x08, 0x8E);
	idt_set_gate(15, (unsigned) isr15, 0x08, 0x8E);

	idt_set_gate(16, (unsigned) isr16, 0x08, 0x8E);
	idt_set_gate(17, (unsigned) isr17, 0x08, 0x8E);
	idt_set_gate(18, (unsigned) isr18, 0x08, 0x8E);
	idt_set_gate(19, (unsigned) isr19, 0x08, 0x8E);
	idt_set_gate(20, (unsigned) isr20, 0x08, 0x8E);
	idt_set_gate(21, (unsigned) isr21, 0x08, 0x8E);
	idt_set_gate(22, (unsigned) isr22, 0x08, 0x8E);
	idt_set_gate(23, (unsigned) isr23, 0x08, 0x8E);

	idt_set_gate(24, (unsigned) isr24, 0x08, 0x8E);
	idt_set_gate(25, (unsigned) isr25, 0x08, 0x8E);
	idt_set_gate(26, (unsigned) isr26, 0x08, 0x8E);
	idt_set_gate(27, (unsigned) isr27, 0x08, 0x8E);
	idt_set_gate(28, (unsigned) isr28, 0x08, 0x8E);
	idt_set_gate(29, (unsigned) isr29, 0x08, 0x8E);
	idt_set_gate(30, (unsigned) isr30, 0x08, 0x8E);
	idt_set_gate(31, (unsigned) isr31, 0x08, 0x8E);
	//Syscall
	idt_set_gate(0x64, (unsigned) isr100, 0x08, 0x8E);
}

///Sets a gate in the IDT
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel,
		unsigned char flags) {
	idt_entries[num].base_lo = (base & 0xFFFF);
	idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

	idt_entries[num].sel = sel;
	idt_entries[num].always0 = 0;
	idt_entries[num].flags = flags;
}

void set_kernel_stack(uint32_t stack) {
	tss_entry.esp0 = stack;
}

///Installs all irqs
void irq_install();

///Inits all descriptor tables.
uint32_t x86_init_descriptor_tables() ///Returns how many tables were initialised on error, otherwise 0.
{
	gdt_ptr.limit = (sizeof(struct gdt_entry) * 6) - 1;
	gdt_ptr.base = (uint32_t) &gdt_entries;

	gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
	write_tss(5, 0x10, 0x0);

	gdt_flush((uint32_t) &gdt_ptr);
	tss_flush();
	printk(LOG_INTERNALS, "hw", "Installed GDT and TSS structures\n");

	idt_ptr.limit = sizeof(struct idt_entry) * 256 - 1;
	idt_ptr.base = (uint32_t) &idt_entries;

	memset(&idt_entries, 0, sizeof(struct idt_entry) * 256);

	idt_init_isrs();
	idt_flush();
	irq_install();
	printk(LOG_INTERNALS, "hw", "Installed IDT and interrupt handlers\n");
	return 0;
}
///Table of all exception messages
const char *exception_messages[] = { "Division By Zero", "Debug",
		"Non Maskable Interrupt", "Breakpoint", "Into Detected Overflow",
		"Out of Bounds", "Invalid Opcode", "No Coprocessor",

		"Double Fault", "Coprocessor Segment Overrun", "Bad TSS",
		"Segment Not Present", "Stack Fault", "General Protection Fault",
		"Page Fault", "Unknown Interrupt",

		"Coprocessor Fault", "Alignment Check", "Machine Check", "Reserved",
		"Reserved", "Reserved", "Reserved", "Reserved",

		"Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved",
		"Reserved", "Reserved" };
/// Function pointer array of interrupt handlers
interrupt_handler_t interrupt_handlers[0xff];

/*
 unsigned int gs, fs, es, ds;      // pushed the segs last
 unsigned int edi, esi, ebp, useless_value, ebx, edx, ecx, eax;  // pushed by pusha. useless value is esp
 unsigned int int_no, err_code;    // our 'push byte #' and ecodes do this
 unsigned int eip, cs, eflags, useresp, ss; //pushed by the processor automatically
 */
//Handles interrupts
extern void fault_handler(struct regs *r) {
	if (interrupt_handlers[r->int_no] != 0) {
		interrupt_handlers[r->int_no](r);
		return;
	} else {
		printk(LOG_FAIL, "fault", "code     | %d (error %d),(%s)\n", r->int_no,
				r->err_code, exception_messages[r->int_no]);
		printk(LOG_FAIL, "fault",
				"segment  | gs:0x%x fs:0x%x es:0x%x ds:0x%x cs:0x%x ss:0x%x\n",
				r->gs, r->fs, r->es, r->ds, r->cs, r->ss);
		printk(LOG_FAIL, "fault", "stack    | esp: 0x%x ebp: 0x%x uesp: 0x%x\n",
				r->useless_value, r->ebp, r->useresp);
		printk(LOG_FAIL, "fault",
				"gp regs  | eax: 0x%x ebx: 0x%x ecx: 0x%x edx: 0x%x\n", r->eax,
				r->ebx, r->ecx, r->edx);
		printk(LOG_FAIL, "fault",
				".......  | esi: 0x%x edi: 0x%x eip: 0x%x eflags: 0x%x \n",
				r->esi, r->edi, r->eip, r->eflags);
		log("Unassigned interrupt!\n");
	}
	if (r->int_no < 32) {
		printk(LOG_FAIL, "fault", "code     | %d (error %d),(%s)\n", r->int_no,
				r->err_code, exception_messages[r->int_no]);
		printk(LOG_FAIL, "fault",
				"segment  | gs:0x%x fs:0x%x es:0x%x ds:0x%x cs:0x%x ss:0x%x\n",
				r->gs, r->fs, r->es, r->ds, r->cs, r->ss);
		printk(LOG_FAIL, "fault", "stack    | esp: 0x%x ebp: 0x%x uesp: 0x%x\n",
				r->useless_value, r->ebp, r->useresp);
		printk(LOG_FAIL, "fault",
				"gp regs  | eax: 0x%x ebx: 0x%x ecx: 0x%x edx: 0x%x\n", r->eax,
				r->ebx, r->ecx, r->edx);
		printk(LOG_FAIL, "fault",
				".......  | esi: 0x%x edi: 0x%x eip: 0x%x eflags: 0x%x \n",
				r->esi, r->edi, r->eip, r->eflags);
		log("Interrupt fault");
	}
}

/** This installs a custom IRQ handler for the given IRQ **/
void register_interrupt_handler(uint8_t n, interrupt_handler_t h) {
	interrupt_handlers[n] = h;

}

/** This clears the handler for a given IRQ **/
void deregister_interrupt_handler(uint8_t n) {
	interrupt_handlers[n] = 0;
}
///Remaps the irq's in the PIC
void irq_remap(void) {
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
}

void irq_install() {
	irq_remap();
	idt_set_gate(32, (unsigned) irq0, 0x08, 0x8E);
	idt_set_gate(33, (unsigned) irq1, 0x08, 0x8E);
	idt_set_gate(34, (unsigned) irq2, 0x08, 0x8E);
	idt_set_gate(35, (unsigned) irq3, 0x08, 0x8E);
	idt_set_gate(36, (unsigned) irq4, 0x08, 0x8E);
	idt_set_gate(37, (unsigned) irq5, 0x08, 0x8E);
	idt_set_gate(38, (unsigned) irq6, 0x08, 0x8E);
	idt_set_gate(39, (unsigned) irq7, 0x08, 0x8E);
	idt_set_gate(40, (unsigned) irq8, 0x08, 0x8E);
	idt_set_gate(41, (unsigned) irq9, 0x08, 0x8E);
	idt_set_gate(42, (unsigned) irq10, 0x08, 0x8E);
	idt_set_gate(43, (unsigned) irq11, 0x08, 0x8E);
	idt_set_gate(44, (unsigned) irq12, 0x08, 0x8E);
	idt_set_gate(45, (unsigned) irq13, 0x08, 0x8E);
	idt_set_gate(46, (unsigned) irq14, 0x08, 0x8E);
	idt_set_gate(47, (unsigned) irq15, 0x08, 0x8E);
	return;
}
///Handles IRQ's
void irq_handler(struct regs *r) {
	/* This is a blank function pointer */
	if (interrupt_handlers[r->int_no] != 0) {
		interrupt_handlers[r->int_no](r);
	} else {
		//printk(LOG_WARN, "irq", "Recieved unhandled interrupt %d (IRQ%d)\n",r->int_no, r->int_no - 32);
		//log("Unhandled interrupt");
	}
	/* If the IDT entry that was invoked was greater than 40
	 *  (meaning IRQ8 - 15), then we need to send an EOI to
	 *  the slave controller */
	if (r->int_no >= 40) {
		outb(0xA0, 0x20);
	}

	/* In either case, we need to send an EOI to the master
	 *  interrupt controller too */
	outb(0x20, 0x20);
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

	log("setting up GDT and IDT");
	x86_init_descriptor_tables();

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
