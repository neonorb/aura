#include <modules/keyboard/keyboard.h>

#include <feta.h>
#include <utils/utils.h>
#include <kernel/idt.h>

#include <modules/keyboard/ps2.h>
#include <modules/keyboard/uefi.h>

#include <kernel/events.h>

//bool pressedKeys[KEY_COUNT];

// callback from driver
void keyboard_keyUpdate(EFI_INPUT_KEY keyMapping) {
// determine pressure and update pressed keys map
	/*if (keyMapping.pressure == PRESSED) {
		if (!pressedKeys[keyMapping.key]) { // if key is not pressed
			// key is now pressed
			pressedKeys[keyMapping.key] = true;
		} else {
			// no-op
		}
	} else if (keyMapping.pressure == RELEASED) {
		if (pressedKeys[keyMapping.key]) { // if key is pressed
			// key is now released
			pressedKeys[keyMapping.key] = false;
		} else {
			// no-op
		}
	}

// construct event
	KeyEvent keyEvent;
	keyEvent.key = keyMapping.key;
	keyEvent.pressure = keyMapping.pressure;
	keyEvent.shift = pressedKeys[LEFT_SHIFT] || pressedKeys[RIGHT_SHIFT];
	keyEvent.ctrl = pressedKeys[LEFT_CONTROL] || pressedKeys[RIGHT_CONTROL];
	keyEvent.alt = pressedKeys[LEFT_ALT] || pressedKeys[RIGHT_ALT];
// keyEvent.meta = pressedKeys[META or LEFT_GUI];*/

//keyEvent.numLock = numLock;
//keyEvent.capsLock = capsLock;

// fire event
	fireKeyboardEvent(keyMapping);
}

/*char keyboard_eventToChar(KeyEvent keyEvent) {
	if (keyEvent.pressure == RELEASED) {
		// no char for released
		return 0;
	} else if (keyEvent.key >= A && keyEvent.key <= Z) {
		// letter
		return (keyEvent.shift ? 0x41 : 0x61) + keyEvent.key - A;
	} else if (keyEvent.key >= DIGIT0 && keyEvent.key <= DIGIT9) {
		if (keyEvent.shift) {
			// use symbols on digit keys
			switch (keyEvent.key) {
			case DIGIT1:
				return '!';
				break;
			case DIGIT2:
				return '@';
				break;
			case DIGIT3:
				return '#';
				break;
			case DIGIT4:
				return '$';
				break;
			case DIGIT5:
				return '%';
				break;
			case DIGIT6:
				return '^';
				break;
			case DIGIT7:
				return '&';
				break;
			case DIGIT8:
				return '*';
				break;
			case DIGIT9:
				return '(';
				break;
			case DIGIT0:
				return ')';
				break;
			default:
				// never happens as range is checked
				break;
			}
		} else {
			// numbers
			return 0x30 + keyEvent.key - DIGIT0;
		}
	} else if (keyEvent.key == SPACE) {
		return ' ';
	} else if (keyEvent.key == TAB) {
		return '\t';
	} else if (keyEvent.key == ENTER) {
		return '\n';
	}
	switch (keyEvent.key) {
	case GRAVE:
		return keyEvent.shift ? '~' : '`';
		break;
	case HYPHEN:
		return keyEvent.shift ? '_' : '-';
		break;
	case EQUALS:
		return keyEvent.shift ? '+' : '=';
		break;
	case LEFT_BRACKET:
		return keyEvent.shift ? '{' : '[';
		break;
	case RIGHT_BRACKET:
		return keyEvent.shift ? '}' : ']';
		break;
	case BACK_SLASH:
		return keyEvent.shift ? '|' : '\\';
		break;
	case SEMICOLON:
		return keyEvent.shift ? ':' : ';';
		break;
	case QUOTE:
		return keyEvent.shift ? '"' : '\'';
		break;
	case COMMA:
		return keyEvent.shift ? '<' : ',';
		break;
	case PERIOD:
		return keyEvent.shift ? '>' : '.';
		break;
	case FORWARD_SLASH:
		return keyEvent.shift ? '?' : '/';
		break;
	default:
		return 0;
		break;
	}
}*/

void keyboard_init() {
	//ps2_keyboard_init();
	uefi_keyboard_init();
	//register_interrupt_handler(IRQ1, &keyboardInterrupt);
}

void keyboard_probe() {
	//ps2_keyboard_probe();
	uefi_keyboard_probe();
}
