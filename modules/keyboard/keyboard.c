#include "keyboard.h"
#include "ps2.c"

KeyboardHandler keyboardHandler;

void fire_keyboard_event(KeyEvent keyEvent){
	keyboardHandler(keyEvent);
}

void registerKeyboardHandler(KeyboardHandler handler){
	keyboardHandler = handler;
}

void keyboard_interrupt(){
	ps2_interrupt();
}
