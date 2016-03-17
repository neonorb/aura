#include "../modules/screen/screen.h"
#include "../modules/keyboard/keyboard.h"
#include "../modules/clock/clock.h"
#include "../kernel/log.h"

static const uint16_t WIDTH = 320;
static const uint8_t HEIGHT = 200;

#define USE_HEIGHT 200

// keys

#define P1_UP  23
#define P1_DOWN 19
#define P2_UP 15
#define P2_DOWN 12

static const uint8_t MOVE_SPEED = 2;

// players

#define PLAYER_WIDTH 5
#define PLAYER_HEIGHT 50

static uint8_t p1 = USE_HEIGHT / 2 - PLAYER_HEIGHT / 2;
static uint8_t ps1;

static uint8_t p2 = USE_HEIGHT / 2 - PLAYER_HEIGHT / 2;
static uint8_t ps2;

// ball

static const uint8_t BALL_WIDTH = 5;
static const uint8_t BALL_HEIGHT = 5;

static uint8_t bx;
static uint8_t by;

static uint8_t bmx;
static uint8_t bmy;

static void pongHandler() {
	// up
	if (pressedKeys[P1_UP]) {
		if (p1 - MOVE_SPEED > p1) {
			p1 = 0;
		} else {
			p1 -= MOVE_SPEED;
		}
	}

	if (pressedKeys[P2_UP]) {
		if (p2 - MOVE_SPEED > p2) {
			p2 = 0;
		} else {
			p2 -= MOVE_SPEED;
		}
	}

	// down
	if (pressedKeys[P1_DOWN]) {
		if (p1 + MOVE_SPEED < p1 || p1 + MOVE_SPEED > USE_HEIGHT - PLAYER_HEIGHT) {
			p1 = USE_HEIGHT - PLAYER_HEIGHT;
		} else {
			p1 += MOVE_SPEED;
		}
	}

	if (pressedKeys[P2_DOWN]) {
		if (p2 + MOVE_SPEED < p2 || p2 + MOVE_SPEED > USE_HEIGHT - PLAYER_HEIGHT) {
			p2 = USE_HEIGHT - PLAYER_HEIGHT;
		} else {
			p2 += MOVE_SPEED;
		}
	}

	// ---- render ----

	// clear
	if (intCount > 6 && intCount < 8) {f
		// notice that the paddle moves for 3 key strokes, then stops moving,
		// if interrupts were working, getting above 8 would make the system work again...
		// note that each stroke is 2 interrupts, up and down

		// you can also try by commenting out the IF and trying with and without the below call
		screen_graphics_rectangle(0, 0, WIDTH, HEIGHT, 0, 0, 0);
	}

	// p1
	screen_graphics_rectangle(0, p1, PLAYER_WIDTH, PLAYER_HEIGHT, 255, 255,
			255);

	// p2
	screen_graphics_rectangle(WIDTH - PLAYER_WIDTH, p2, PLAYER_WIDTH,
	PLAYER_HEIGHT, 255, 255, 255);

	// ball
	screen_graphics_rectangle(bx, by, BALL_WIDTH, BALL_HEIGHT, 255, 255, 255);

	static uint8_t x;
	//screen_graphics_rectangle(0, 0, x, x, 255, 255, 255);
	x++;

	vga_graphics_flip();
}

void pong() {
	screen_graphics_initialize();
	screen_graphicsMode();

	clock_handler(&pongHandler);

	while (true) {
	}
}
