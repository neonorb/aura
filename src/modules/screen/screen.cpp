#include <modules/screen/screen.h>

#include <int.h>
#include <modules/screen/vga.h>

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
void screen_graphics_rectangle(uint32 x, uint32 y, uint32 width, uint32 height,
		uint8 red, uint8 green, uint8 blue){
	vga_graphics_rectangle(x, y, width, height, red, green, blue);
}
void screen_graphics_pixel(uint32 x, uint32 y, uint8 red, uint8 green,
		uint8 blue){
	vga_graphics_pixel(x, y, red, green, blue);
}
void screen_graphics_flip(){
	vga_graphics_flip();
}

void screen_graphicsMode(){
	vga_graphicsMode();
}
