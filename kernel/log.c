#include "log.h"
#include "../modules/screen/screen.h"

#include "../modules/screen/vga.h" // TODO remove this include

void log(char* message){
	log_write(NORMAL, message);
}

void warning(char* message){
	log_write(WARNING, message);
}

void fault(char* message){
	log_write(FAULT, message);
}

void log_write(LogType logType, char* message) {
	switch (logType) {
	case NORMAL:// TODO change vga color change to screen
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
