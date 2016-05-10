#ifndef PORTS_H_
#define PORTS_H_

#include <int.h>

inline void outb(uint16, uint8);
inline void outw(uint16, uint16);

inline uint8 inb(uint16);
inline uint16 inw(uint16);

inline void io_wait();

#endif /* PORTS_H_ */
