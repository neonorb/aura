#include "log.h"
#include "../modules/screen/vga.h"

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
	case NORMAL:
		terminal_setcolor(COLOR_GREEN);
		terminal_writestring("[NORMAL] ");
		break;
	case WARNING:
		terminal_setcolor(COLOR_LIGHT_RED);
		terminal_writestring("[WARNING] ");
		break;
	case FAULT:
		terminal_setcolor(COLOR_RED);
		terminal_writestring("[FAULT] ");
		break;
	}

	terminal_setcolor(COLOR_WHITE);
	terminal_writestring(message);
	terminal_writestring("\n");
}
