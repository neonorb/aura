/* log.c - Logs to stdout and the like */
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

#include <string.h>

#include "log.h"
#include "../modules/screen/screen.h"

#include "../modules/screen/vga.h" // TODO remove this include

void log(String message) {
	log_write(NORMAL, message);
}

void warning(String message) {
	log_write(WARNING, message);
}

void fault(String message) {
	log_write(FAULT, message);
}

void log_write(LogType logType, String message) {
	switch (logType) {
	case NORMAL: // TODO change vga color change to screen
		vga_terminal_backgroundColor(COLOR_GREEN);
		screen_terminal_writeString("[NORMAL] ");
		break;
	case WARNING:
		vga_terminal_backgroundColor(COLOR_LIGHT_RED);
		screen_terminal_writeString("[WARNING] ");
		break;
	case FAULT:
		vga_terminal_backgroundColor(COLOR_RED);
		screen_terminal_writeString("[FAULT] ");
		break;
	}

	vga_terminal_backgroundColor(COLOR_LIGHT_GREY);
	screen_terminal_writeString(message);
	screen_terminal_writeString("\n");
}
