#include "../modules/screen/screen.h"
#include "../modules/keyboard/keyboard.h"
#include "../modules/clock/clock.h"
#include "../kernel/log.h"

static const uint16_t WIDTH = 320;
static const uint8_t HEIGHT = 200;

#define USE_HEIGHT 180

// keys

#define P1_UP  23
#define P1_DOWN 19
#define P2_UP 15
#define P2_DOWN 12

static const uint8_t MOVE_SPEED = 8;

// players

#define PLAYER_WIDTH 5
#define PLAYER_HEIGHT 50

static uint8_t p1 = USE_HEIGHT / 2 - PLAYER_HEIGHT / 2;
static uint8_t ps1 = 100;

static uint8_t p2 = USE_HEIGHT / 2 - PLAYER_HEIGHT / 2;
static uint8_t ps2 = 100;

// ball

static const uint8_t BALL_WIDTH = 5;
static const uint8_t BALL_HEIGHT = 5;

static uint16_t bx;
static uint16_t by;

static int8_t bmx;
static int8_t bmy;

static void resetBall() {
	bx = 0;
	by = 0;

	bmx = 5;
	bmy = 5;
}

static bool started = false;
static uint8_t collisionCount = 0;

static void render() {
	// clear
	screen_graphics_rectangle(0, 0, WIDTH, HEIGHT, 0, 0, 0);

	// p1
	screen_graphics_rectangle(0, p1, PLAYER_WIDTH, PLAYER_HEIGHT, 255, 255,
			255);

	// p2
	screen_graphics_rectangle(WIDTH - PLAYER_WIDTH, p2, PLAYER_WIDTH,
	PLAYER_HEIGHT, 255, 255, 255);

	// ball
	screen_graphics_rectangle(bx, by, BALL_WIDTH, BALL_HEIGHT, 255, 255, 255);

	// separator
	screen_graphics_rectangle(0, USE_HEIGHT, WIDTH, 1, 255, 255, 255);

	// score

	for (uint8_t i = 0; i < ps1; i++) {
		screen_graphics_rectangle(i * 4 + 1, USE_HEIGHT + 2, 3, 3, 255, 255,
				255);
	}

	for (uint8_t i = 0; i < ps2; i++) {
		screen_graphics_rectangle(i * 4 + 1, USE_HEIGHT + 6, 3, 3, 255, 255,
				255);
	}

	vga_graphics_flip();
}

static void pongHandler() {
	static uint16_t counter;
	if (counter < 1) {
		counter++;
		return;
	}
	counter = 0;

	if (!started) {
		if (ps1 == 0) {
			resetBall();
			started = true;
			return;
		}
		ps1--;
		ps2--;
		render();
		return;
	}

	// ---- move paddles ----
	// up
	if (pressedKeys[P1_UP]) {
		if (MOVE_SPEED > p1) {
			p1 = 0;
		} else {
			p1 -= MOVE_SPEED;
		}
	}

	if (pressedKeys[P2_UP]) {
		if (MOVE_SPEED > p2) {
			p2 = 0;
		} else {
			p2 -= MOVE_SPEED;
		}
	}

	// down
	if (pressedKeys[P1_DOWN]) {
		if (p1 + PLAYER_HEIGHT + MOVE_SPEED > USE_HEIGHT) {
			p1 = USE_HEIGHT - PLAYER_HEIGHT;
		} else {
			p1 += MOVE_SPEED;
		}
	}

	if (pressedKeys[P2_DOWN]) {
		if (p2 + PLAYER_HEIGHT + MOVE_SPEED > USE_HEIGHT) {
			p2 = USE_HEIGHT - PLAYER_HEIGHT;
		} else {
			p2 += MOVE_SPEED;
		}
	}

	// ---- move ball ----

	// walls
	// bottom wall collision
	if (by + bmy + BALL_HEIGHT > USE_HEIGHT) {
		bmy = -1 * bmy;
	}

	// top wall collision
	else if (-1 * bmy > by) {
		bmy = -1 * bmy;
	}

	// paddles
	// right paddle collision
	else if (bmx + bx + BALL_WIDTH > WIDTH - PLAYER_WIDTH
			&& by + bmy > p2&& by + bmy < p2 + PLAYER_HEIGHT) {
		bmx = -1 * bmx;
		collisionCount++;
	}

	// left paddle collision
	else if (-1 * bmx > bx - PLAYER_WIDTH
			&& by + bmy > p1&& by + bmy < p1 + PLAYER_HEIGHT) {
		bmx = -1 * bmx;
		collisionCount++;
	}

	// score

	// right score
	else if (-1 * bmx > bx) {
		ps2++;
		collisionCount = 0;
		resetBall();
	}

	// left score
	else if (bx + bmx + BALL_WIDTH > WIDTH) {
		ps1++;
		collisionCount = 0;
		resetBall();
	}

	if (collisionCount > 1) {
		bmx++;
		bmy++;
		collisionCount = 0;
	}

	// move balls

	bx += bmx;
	by += bmy;

	// ---- render ----

	render();
}

void pong() {
	screen_graphics_initialize();
	screen_graphicsMode();

	clock_handler(&pongHandler);

	while (true) {
	}
}
