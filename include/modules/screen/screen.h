#ifndef SCREEN_H_
#define SCREEN_H_

#include <int.h>
#include <string.h>

void screen_terminal_initialize();
void screen_terminal_writeString(String data);

void screen_graphics_initialize();
void screen_graphics_rectangle(uint32 x, uint32 y, uint32 width, uint32 height,	uint8 red, uint8 green, uint8 blue);
void screen_graphics_pixel(uint32 x, uint32 y, uint8 red, uint8 green,	uint8 blue);
void screen_graphics_flip();

void screen_graphicsMode();

#endif /* SCREEN_H_ */
