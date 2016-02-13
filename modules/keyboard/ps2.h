#define keycount 17
char scancodes[keycount] =   {'e', // escape
			      '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
			      '-', '=',
			       8, // backspace
			       9, // tab
			      'Q', 'W'}; // TODO finish this table, see http://wiki.osdev.org/Keyboard#Scan_Code_Set_1
bool keyspressed[keycount];

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ( "inb %[port], %[ret]"
                   : [ret] "=a"(ret)   // using symbolic operand names as an example, mainly because they're not used in order
                   : [port] "Nd"(port) );
    return ret;
}

uint8_t getScancode() {
	/*while(true){
		uint8_t c = inb(0x60);
		outb(0x60, 0xF0);
		if(c > 0){
			return c;
		}
	}
	*/
	outb(0x60, 0xF0); // send EOI
	return inb(0x60);
/*	char c=0;
	while(true) {
		if(inb(0x60)!=c) {
			c=inb(0x60);
			if(c>0){
				outb(0x20, 0xFA);
				return c;
			}
		}
	}*/
}

uint8_t getchar() {
	while(true){
		uint8_t scancode = getScancode() - 1;

		if(scancode > 128){
			// key released
			scancode = scancode - 128;

			// terminal_putchar(scancode - 128);

			if(scancode < keycount)
	                if(keyspressed[scancode]){ // if key is set to pressed
	                      keyspressed[scancode] = false; // set the key as NOT pressed
	                }
		}else{
	                // key pressed
			if(scancode > keycount) return '?';
	                if(!keyspressed[scancode]){ // if key is set to NOT pressed
	                        keyspressed[scancode] = true; // set the key as pressed
	                        return scancodes[scancode]; // since we set the key as pressed, we return the new key press
	                }
	        }
	}
}
