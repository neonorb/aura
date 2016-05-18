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

#include "ports.h"

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

/****** serial debugging ********/

#define PORT 0x3f8   /* COM1 */

void init_serial() {
	outb(PORT + 1, 0x00);    // Disable all interrupts
	outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	outb(PORT + 1, 0x00);    //                  (hi byte)
	outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
	outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int is_transmit_empty() {
	return inb(PORT + 5) & 0x20;
}

void write_serial(char a) {
	while (is_transmit_empty() == 0)
		;

	outb(PORT, a);
}

void debug(String name, uint64 value) {
	debug(name, value, 16);
}

void debug(String name, uint64 value, uint8 base) {
	debugPrint(name);
	debugPrint(": ");
	debug(toString(value, "          ", base));
}

void debug(String message) {
	debugPrint(message);
	debugPrint("\n");
}

void debugPrint(String message) {
	for (uint64 i = 0; i < strlen(message); i++) {
		write_serial(message[i]);
	}
}
