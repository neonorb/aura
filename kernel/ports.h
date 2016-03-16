#ifndef PORTS_H_
#define PORTS_H_

inline void outb(uint16_t, uint8_t);
inline void outw(uint16_t, uint16_t);

inline uint8_t inb(uint16_t);
inline uint16_t inw(uint16_t);

inline void io_wait(void)
{
    /* TODO: This is probably fragile. */
    asm volatile ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}

#endif /* PORTS_H_ */
