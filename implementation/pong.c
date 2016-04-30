    /* pong - Simple C program implementing the game Pong */
    /* Copyright (C) 2016  Chris Smith */

    /* This program is free software: you can redistribute it and/or modify */
    /* it under the terms of the GNU General Public License as published by */
    /* the Free Software Foundation, either version 3 of the License, or */
    /* (at your option) any later version. */

    /* This program is distributed in the hope that it will be useful, */
    /* but WITHOUT ANY WARRANTY; without even the implied warranty of */
    /* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
    /* GNU General Public License for more details. */

    /* You should have received a copy of the GNU General Public License */
    /* along with this program.  If not, see <http://www.gnu.org/licenses/>. */


#include "../modules/screen/screen.h"
#include "../modules/keyboard/keyboard.h"
#include "../modules/clock/clock.h"
#include "../kernel/log.h"

#define WIDTH 320
#define HEIGHT 200 // screen height

#define USE_HEIGHT 190 // play field height

// keys

#define P1_UP  Q
#define P1_DOWN S
#define P2_UP P
#define P2_DOWN L

#define PLAYER_MOVE_SPEED 8

// players

#define PLAYER_WIDTH 5
#define PLAYER_HEIGHT 50

static int16_t p1;
static uint8_t ps1;

static int16_t p2;
static uint8_t ps2;

// ball

#define BALL_WIDTH 10
#define BALL_HEIGHT 10

#define BALL_ACCELERATION 4

typedef struct {
	int16_t bx;
	int16_t by;

	int8_t bmx;
	int8_t bmy;
} Ball;

#define BALL_COUNT 1

static Ball balls[BALL_COUNT];

static bool paused = false;
static bool started = false;
static uint8_t collisionCount = 0;
static int win;

static void resetBall() {
	for (int i = 0; i < BALL_COUNT; i++) {
		switch (random(0, 4)) {
		case 0:
			balls[i].bx = 0;
			balls[i].by = 0;

			balls[i].bmx = random(3, 5);
			balls[i].bmy = random(3, 5);
			break;
		case 1:
			balls[i].bx = WIDTH - BALL_WIDTH;
			balls[i].by = 0;

			balls[i].bmx = random(-5, -3);
			balls[i].bmy = random(3, 5);
			break;
		case 2:
			balls[i].bx = WIDTH - BALL_WIDTH;
			balls[i].by = USE_HEIGHT - BALL_HEIGHT;

			balls[i].bmx = random(-5, -3);
			balls[i].bmy = random(-5, -3);
			break;
		case 3:
			balls[i].bx = 0;
			balls[i].by = USE_HEIGHT - BALL_HEIGHT;

			balls[i].bmx = random(3, 5);
			balls[i].bmy = random(-5, -3);
			break;
		}
	}
}

static void reset() {
	resetBall();

	p1 = USE_HEIGHT / 2 - PLAYER_HEIGHT / 2;
	p2 = USE_HEIGHT / 2 - PLAYER_HEIGHT / 2;

	ps1 = 100;
	ps2 = 100;

	started = false;
	paused = false;
}

static void render() {
	// clear
	screen_graphics_rectangle(0, 0, WIDTH, HEIGHT, win, 0, 0);
	if (win > 0) {
		win -= 32;
	}
	if (win < 0) {
		win = 0;
	}

	// p1
	screen_graphics_rectangle(0, p1, PLAYER_WIDTH, PLAYER_HEIGHT, 255, 255,
			255);

	// p2
	screen_graphics_rectangle(WIDTH - PLAYER_WIDTH, p2, PLAYER_WIDTH,
	PLAYER_HEIGHT, 255, 255, 255);

	// ball
	for (int i = 0; i < BALL_COUNT; i++) {
		screen_graphics_rectangle(balls[i].bx, balls[i].by, BALL_WIDTH,
				BALL_HEIGHT, 255, 255, 255);
	}

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

	if (paused) {
		render();
		return;
	}

	// ---- move paddles ----
	// up
	if (pressedKeys[P1_UP]) {
		if (p1 - PLAYER_MOVE_SPEED < 0) {
			p1 = 0;
		} else {
			p1 -= PLAYER_MOVE_SPEED;
		}
	}

	if (pressedKeys[P2_UP]) {
		if (p2 - PLAYER_MOVE_SPEED < 0) {
			p2 = 0;
		} else {
			p2 -= PLAYER_MOVE_SPEED;
		}
	}

	// down
	if (pressedKeys[P1_DOWN]) {
		if (p1 + PLAYER_MOVE_SPEED + PLAYER_HEIGHT > USE_HEIGHT) {
			p1 = USE_HEIGHT - PLAYER_HEIGHT;
		} else {
			p1 += PLAYER_MOVE_SPEED;
		}
	}

	if (pressedKeys[P2_DOWN]) {
		if (p2 + PLAYER_MOVE_SPEED + PLAYER_HEIGHT > USE_HEIGHT) {
			p2 = USE_HEIGHT - PLAYER_HEIGHT;
		} else {
			p2 += PLAYER_MOVE_SPEED;
		}
	}

	// ---- move ball ----

	for (int i = 0; i < BALL_COUNT; i++) {
		// walls
		// bottom wall collision
		if (balls[i].by + balls[i].bmy + BALL_HEIGHT > USE_HEIGHT) {
			balls[i].bmy -= 2 * balls[i].bmy;
		}

		// top wall collision
		else if (balls[i].by + balls[i].bmy < 0) {
			balls[i].bmy -= 2 * balls[i].bmy;
		}

		// paddles
		// right paddle collision
		else if (balls[i].bmx + balls[i].bx + BALL_WIDTH > WIDTH - PLAYER_WIDTH
				&& balls[i].by + balls[i].bmy + BALL_HEIGHT
						> p2&& balls[i].by + balls[i].bmy < p2 + PLAYER_HEIGHT) {
			balls[i].bmx = -1 * balls[i].bmx;
			collisionCount++;
		}

		// left paddle collision
		else if (balls[i].bx + balls[i].bmx < PLAYER_WIDTH
				&& balls[i].by + balls[i].bmy + BALL_HEIGHT
						> p1&& balls[i].by + balls[i].bmy < p1 + PLAYER_HEIGHT) {
			balls[i].bmx = -1 * balls[i].bmx;
			collisionCount++;
		}

		// score

		// right score
		else if (-1 * balls[i].bmx > balls[i].bx) {
			ps2++;
			collisionCount = 0;
			win = 255;
			resetBall();
		}

		// left score
		else if (balls[i].bx + balls[i].bmx + BALL_WIDTH > WIDTH) {
			ps1++;
			collisionCount = 0;
			win = 255;
			resetBall();
		}

		if (collisionCount >= BALL_ACCELERATION) {
			if (balls[i].bmx < 0)
				balls[i].bmx--;
			else
				balls[i].bmx++;

			if (balls[i].bmy < 0)
				balls[i].bmy--;
			else
				balls[i].bmy++;

			collisionCount = 0;
		}

		// move balls

		balls[i].bx += balls[i].bmx;
		balls[i].by += balls[i].bmy;
	}

	// ---- render ----

	render();
}

static void pongKeyboardHandler(KeyEvent event) {
	if (keyboard_eventToChar(event) == ' ') {
		paused = !paused;
	} else if (event.key == ESCAPE && event.pressure == PRESSED) {
		reset();
	}
}

void pong() {
	screen_graphics_initialize();
	screen_graphicsMode();

	clock_handler(&pongHandler);
	keyboard_handler(&pongKeyboardHandler);

	reset();
	for (int i = 0; i < BALL_COUNT; i++) {
		balls[i].bx = -1; // don't
	}

	while (true) {
	}
}
