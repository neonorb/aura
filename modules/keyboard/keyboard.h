#ifndef KEYBOARD_H_
#define KEYBOARD_H_

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
	SINGLE_QUOTE,
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
	APPS
} Key;

typedef enum {
	TYPE_UNDEFINED, PRESSED, RELEASED
} KeyEventType;

typedef struct {
	Key key;
	KeyEventType type;
} KeyEvent;

#define EVENT_NULL {KEY_UNDEFINED, TYPE_UNDEFINED}

typedef void (*KeyboardHandler)(KeyEvent);

void fire_keyboard_event(KeyEvent);

void registerKeyboardHandler(KeyboardHandler handler);

void keyboard_interrupt();

#endif /* KEYBOARD_H_ */
