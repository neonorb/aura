/* Hardware text mode color constants. */
enum vga_color {
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
	COLOR_WHITE = 15,
};

// FIX TO -Wimplicit-function-declaration WARNING
void terminal_backup();
void terminal_newline();

uint8_t make_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}
 
uint16_t make_vgaentry(char c, uint8_t color) {
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}
 
size_t strlen(const char* str) {
	size_t ret = 0;
	while ( str[ret] != 0 )
		ret++;
	return ret;
}
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static const int TAB_SIZE = 2;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
 
void terminal_initialize() {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}
 
void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}
 
void terminal_putchar(char c) {
	if (c == '\n'){
                // new line
                terminal_newline();
		return;
        }else if(c == 8){
                // backspace
                terminal_backup();
                terminal_putchar(' ');
		terminal_backup();
        }else if (c == 9){
                // tab
                for(int i = 0; i < TAB_SIZE; i++){ // FIXME backspacing will only delete part of the tab, not a real tab
                       terminal_putchar(' ');
                }
        }else{
                // regular characters
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT) {
				terminal_row = 0;
			}
		}
	}
}

void terminal_backup(){
	if (--terminal_column == (unsigned)-1) {
	        terminal_column = VGA_WIDTH - 1;
          if (--terminal_row == (unsigned)-1) {
                        terminal_row = VGA_HEIGHT - 1;
                }
        }
}
 
void terminal_writestring(const char* data) {
	size_t datalen = strlen(data);
	for (size_t i = 0; i < datalen; i++) {
		char c = data[i];
		if (c == '\n'){
			// new line
			terminal_newline();
		}else if(c == 8){
			// backspace
			//terminal_backup();
			terminal_row = 0;
			terminal_column = 0;
			terminal_putchar(' ');
		}else if (c == 9){
			// tab
			for(int i = 0; i < TAB_SIZE; i++){ // FIXME backspacing will only delete part of the tab, not a real tab
				terminal_putchar(' ');
			}
		}else{
			// regular characters
			terminal_putchar(c);
		}
	}
}

void terminal_newline() {
	for (size_t x = terminal_column; x < VGA_WIDTH; x++) {
		terminal_putchar(' ');
	}
}
