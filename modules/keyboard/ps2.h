char scancodes[] = {'?', '1', '2'}; // TODO finish this table, see http://wiki.osdev.org/Keyboard#Scan_Code_Set_1

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

char getScancode() {
	char c=0;
	do {
	if(inb(0x60)!=c) {
	c=inb(0x60);
		if(c>0)
			return c;
		}
	}while(1);
}

char getchar() {
	int scancode = getScancode() - 1;

	if(scancode > array_length_char(scancodes)){
		return '?';
	}

	return scancodes[scancode];
}
