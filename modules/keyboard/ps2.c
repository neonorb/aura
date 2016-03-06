#include <stdbool.h>
#include "ps2.h"
#include "../../kernel/ports.c"

static uint8_t getScancode() {
	//outb(0x60, 0xF0); // send EOI
	return inb(0x60);
}

static void readKeyboard() {
	uint8_t scancode = getScancode();
	//bool second = false;
	if(scancode == 0xE0){
		//second = true;
		scancode = getScancode();
	}

	keyUpdate(scanCodeSet1[scancode]);
}

void ps2_interrupt() {
	readKeyboard();
}
