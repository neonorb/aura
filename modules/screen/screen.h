#ifndef SCREEN_H_
#define SCREEN_H_

void screen_terminal_initialize();
void screen_terminal_writeString(const char* data);

void screen_graphics_initialize();
void screen_graphics_rectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height,	uint8_t red, uint8_t green, uint8_t blue);
void screen_graphics_pixel(uint32_t x, uint32_t y, uint8_t red, uint8_t green,	uint8_t blue);
void screen_graphics_flip();

void screen_graphicsMode();

#endif /* SCREEN_H_ */
