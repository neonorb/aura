#include "screen.h"
#include "vga.c"

// ---- terminal ----

void screen_terminal_initialize(){
	vga_terminal_initialize();
}

void screen_terminal_writeString(const char* data){
	vga_terminal_writeString(data);
}

// ---- graphics ----

void screen_graphics_initialize(){
	vga_graphics_initialize();
}
void screen_graphics_rectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height,
		uint8_t red, uint8_t green, uint8_t blue){
	vga_graphics_rectangle(x, y, width, height, red, green, blue);
}
void screen_graphics_pixel(uint32_t x, uint32_t y, uint8_t red, uint8_t green,
		uint8_t blue){
	vga_graphics_pixel(x, y, red, green, blue);
}
void screen_graphics_flip(){
	vga_graphics_flip();
}

void screen_graphicsMode(){
	vga_graphicsMode();
}
