#include <int.h>

#include "ps2.h"

#include "../../kernel/ports.cpp"

static uint8 getScancode() {
	//outb(0x60, 0xF0); // send EOI
	return inb(0x60);
}

static void readKeyboard() {
	uint8 scancode = getScancode();
	//bool second = false;
	if(scancode == 0xE0){
		//second = true;
		scancode = getScancode();
	}

	keyboard_keyUpdate(scanCodeSet1[scancode]);
}

void ps2_interrupt() {
	readKeyboard();
}
