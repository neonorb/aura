#include "../../kernel/ports.h"
#include "../../kernel/log.h"

#define CLOCK_FREQUENCY 4

typedef void (*PITHandler)(void);

static PITHandler pitHandler;

static void pitInterrupt() {
	if (pitHandler != 0) {
		pitHandler();
	}
}

void pit_initialize() {
	register_interrupt_handler(IRQ0, pitInterrupt);

	// The value we send to the PIT is the value to divide it's input clock
	// (1193180 Hz) by, to get our required frequency. Important to note is
	// that the divisor must be small enough to fit into 16-bits.
	uint32_t divisor = 1193180 / CLOCK_FREQUENCY;

	// Send the command byte.
	outb(0x43, 0x36);

	// Divisor has to be sent byte-wise, so split here into upper/lower bytes.
	uint8_t l = (uint8_t) (divisor & 0xFF);
	uint8_t h = (uint8_t) ((divisor >> 8) & 0xFF);

	// Send the frequency divisor.
	outb(0x40, l);
	outb(0x40, h);
}

void pit_handler(PITHandler handler) {
	pitHandler = handler;
}
