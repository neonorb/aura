#ifndef VGA_H_
#define VGA_H_

/* Hardware text mode color constants. */
typedef enum {
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15
} VGATextColor;

// ---- terminal ----

#define VGA_TEXT_WIDTH 80
#define VGA_TEXT_HEIGHT 25

uint8_t vga_terminal_makeColor(VGATextColor fg, VGATextColor bg);
void vga_terminal_backgroundColor(uint8_t color);
void vga_terminal_writeString(const char* data);
void vga_terminal_initialize();

// ---- graphics ----

#define VGA_GRAPHICS_WIDTH 320
#define VGA_GRAPHICS_HEIGHT 200

// public API

void vga_graphics_initialize();
void vga_graphics_flip();
void vga_graphics_pixel(uint32_t x, uint32_t y, uint8_t red, uint8_t green,
		uint8_t blue);
void vga_graphics_rectangle(uint32_t x, uint32_t y, uint32_t width,
		uint32_t height, uint8_t red, uint8_t green, uint8_t blue);
void vga_graphicsMode();

#endif /* VGA_H_ */
