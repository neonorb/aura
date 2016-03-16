#include "../modules/screen/screen.h"
#include "../modules/keyboard/keyboard.h"
#include "../modules/clock/clock.h"
#include "../kernel/log.h"

static uint8_t x = 0;
static void pongHandler() {
	//log("t");
	screen_graphics_rectangle(0, 0, x, x, 0, 0, 0);
	screen_graphics_rectangle(0, 0, x, x, 64, 64, 64);
	x++;
}

void pong() {
	screen_graphics_initialize();
	screen_graphicsMode();

	clock_handler(&pongHandler);

	while (true) {
	}
}
