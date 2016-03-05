

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

///Installs all irqs
void irq_install();

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


void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel,
		unsigned char flags);
extern void idt_flush();
typedef void (*interrupt_handler_t)(registers_t *);
void register_interrupt_handler(uint8_t n, interrupt_handler_t h); //TODO: Rename to a more x86 specific name
void deregister_interrupt_handler(uint8_t n);

struct idt_entry idt_entries[256];
struct idt_ptr idt_ptr;



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

void init_idt(){
	idt_ptr.limit = sizeof(struct idt_entry) * 256 - 1;
	idt_ptr.base = (uint32_t) &idt_entries;

	memset(&idt_entries, 0, sizeof(struct idt_entry) * 256);

	idt_init_isrs();
	idt_flush();
	irq_install();
}
