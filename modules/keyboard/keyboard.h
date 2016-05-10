#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <bool.h>

typedef enum {
	KEY_UNDEFINED,

	// letters
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,

	// numbers (at top of keyboard)
	DIGIT0,
	DIGIT1,
	DIGIT2,
	DIGIT3,
	DIGIT4,
	DIGIT5,
	DIGIT6,
	DIGIT7,
	DIGIT8,
	DIGIT9,

	// numpad
	NUMPAD0,
	NUMPAD1,
	NUMPAD2,
	NUMPAD3,
	NUMPAD4,
	NUMPAD5,
	NUMPAD6,
	NUMPAD7,
	NUMPAD8,
	NUMPAD9,
	NUMPAD_DECIMAL,
	NUMPAD_PLUS,
	NUMPAD_MINUS,
	NUMPAD_TIMES,
	NUMPAD_DIVIDE,
	NUMPAD_ENTER,

	// escape
	ESCAPE,

	// F's
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,

	// symbols
	GRAVE,
	HYPHEN,
	EQUALS,
	SEMICOLON,
	QUOTE,
	COMMA,
	PERIOD,

	BACK_SLASH,
	FORWARD_SLASH,

	LEFT_BRACKET,
	RIGHT_BRACKET,

	// white space
	SPACE,
	TAB,

	// modifiers
	LEFT_ALT,
	RIGHT_ALT,
	LEFT_CONTROL,
	RIGHT_CONTROL,
	LEFT_SHIFT,
	RIGHT_SHIFT,

	BACKSPACE,
	ENTER,

	CAPS_LOCK,
	NUM_LOCK,
	SCROLL_LOCK,

	HOME,
	END,
	INSERT,
	DELETE,
	PAGE_UP,
	PAGE_DOWN,

	CURSOR_UP,
	CURSOR_DOWN,
	CURSOR_LEFT,
	CURSOR_RIGHT,

	LEFT_GUI,
	RIGHT_GUI,
	/* if this end marker is changed, update KEY_COUNT macro */APPS
} Key;

#define KEY_COUNT APPS - KEY_UNDEFINED

typedef enum {
	PRESSURE_UNDEFINED, PRESSED, RELEASED
} KeyPressure;

typedef struct {
	Key key;
	KeyPressure pressure;
} KeyMapping;

#define NULL_MAPPING {KEY_UNDEFINED, PRESSURE_UNDEFINED}

typedef struct {
	Key key;
	KeyPressure pressure;

	bool shift;
	bool ctrl;
	bool alt;
	bool meta;
	bool numLock;
	bool capsLock;
} KeyEvent;

bool pressedKeys[KEY_COUNT];

void keyboard_keyUpdate(KeyMapping);
typedef void (*KeyboardHandler)(KeyEvent);

//public API

void keyboard_handler(KeyboardHandler handler);
char keyboard_eventToChar(KeyEvent keyEvent);
void keyboard_initialize();

#endif /* KEYBOARD_H_ */
