#include <modules/clock/pit.h>

#include <int.h>
#include <kernel/ports.h>
#include <kernel/idt.h>
#include <log.h>

#define CLOCK_FREQUENCY 60

static PITHandler pitHandler;

static void pitInterrupt(uint64 errorCode, registers_t* regs) {
	if (pitHandler != 0) {
		pitHandler();
	}
}

void pit_initialize() {
	register_interrupt_handler(IRQ0, pitInterrupt);

	// The value we send to the PIT is the value to divide it's input clock
	// (1193180 Hz) by, to get our required frequency. Important to note is
	// that the divisor must be small enough to fit into 16-bits.
	uint32 divisor = 1193180 / CLOCK_FREQUENCY;

	// Send the command byte.
	outb(0x43, 0x36);

	// Divisor has to be sent byte-wise, so split here into upper/lower bytes.
	uint8 l = (uint8) (divisor & 0xFF);
	uint8 h = (uint8) ((divisor >> 8) & 0xFF);

	// Send the frequency divisor.
	outb(0x40, l);
	outb(0x40, h);
}

void pit_handler(PITHandler handler) {
	pitHandler = handler;
}
