#ifndef PORTS_H_
#define PORTS_H_

#include <int.h>

inline void outb(uint16, uint8);
inline void outw(uint16, uint16);

inline uint8 inb(uint16);
inline uint16 inw(uint16);

inline void io_wait(void)
{
    /* TODO: This is probably fragile. */
    asm volatile ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}

#endif /* PORTS_H_ */
