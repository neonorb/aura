/*
 * modules.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: chris13524
 */

#include <modules/modules.h>

#include <modules/screen/screen.h>
#include <modules/keyboard/keyboard.h>

#include <feta.h>

void modules_init() {
	screen_init();

	status("initializing keyboard");
	keyboard_init();
	statusDone();
}

void modules_probe() {
	keyboard_probe();
}
