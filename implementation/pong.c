#include "../modules/screen/screen.h"
#include "../modules/keyboard/keyboard.h"

static uint8_t x = 0;
static void pongKeyboardHandler(KeyEvent keyEvent) {
	char c = keyboard_eventToChar(keyEvent);
	if (c > 0) {
		screen_graphics_rectangle(0, 0, x, x, 0, 0, 0);
		screen_graphics_rectangle(0, 0, x, x, 64, 64, 64);
		x++;
		//terminal_putchar(c);
	}

	//acpiPowerOff();
	/*char string[digitCount(e.type)];
	 toString(string, e.type);

	 char message[] = "Event type: ";

	 char result[sizeof(e.type) + sizeof(message)];
	 concat(result, message, string);

	 log(result);*/
}

void pong() {
	screen_graphics_initialize();
	screen_graphicsMode();

	keyboard_handler(&pongKeyboardHandler);

	while (true) {
	}
}
