#include "ps2.h"

uint8_t getScancode() {
	//outb(0x60, 0xF0); // send EOI
	return inb(0x60);
}

void ps2_readkeyboard() {
	uint8_t scancode = getScancode();
	bool second = false;
	if(scancode == 0xE0){
		second = true;
		scancode = getScancode();
	}

	fire_keyboard_event(scanCodeSet1[scancode]);
}

void ps2_interrupt() {
	ps2_readkeyboard();
}
