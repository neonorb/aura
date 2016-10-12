#ifndef PORTS_H_
#define PORTS_H_

#include <int.h>

using namespace feta;

// out

inline void outb(uint16 port, uint8 val) {
	asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	/* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
	 * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
	 * The  outb  %al, %dx  encoding is the only option for all other cases.
	 %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

inline void outw(uint16 port, uint16 val) {
	asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
	/* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
	 * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
	 * The  outb  %al, %dx  encoding is the only option for all other cases.
	 * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

// in

inline uint8 inb(uint16 port) {
	uint8 ret;
	asm volatile ( "inb %[port], %[ret]"
			: [ret] "=a"(ret) // using symbolic operand names as an example, mainly because they're not used in order
			: [port] "Nd"(port) );
	return ret;
}

inline uint16 inw(uint16 port) {
	uint16 ret;
	asm volatile ( "inw %[port], %[ret]"
			: [ret] "=a"(ret) // using symbolic operand names as an example, mainly because they're not used in order
			: [port] "Nd"(port) );
	return ret;
}

// wait

inline void io_wait() {
	/* TODO: This is probably fragile. */
	asm volatile ( "jmp 1f\n\t"
			"1:jmp 2f\n\t"
			"2:" );
}

#endif /* PORTS_H_ */
