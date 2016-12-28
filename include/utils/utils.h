/*
 * utils.h
 *
 *  Created on: Jul 31, 2016
 *      Author: chris
 */

#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_

#include <feta.h>

using namespace feta;

inline void cli() {
	asm("cli");
}

inline void sti() {
	asm("sti");
}

inline void hlt() {
	asm("hlt");
}

int array_length_char(char a[]);

void* memcpy(uint8* dst, const uint8* src, size len);

String dynamicString(String string);

// Write len copies of val into dest.
void memset(uint8* dest, uint8 val, size len);

int memcmp(const void* s1, const void* s2, size len);

uint8 setBit(uint8 number, uint8 pos);
uint8 clearBit(uint8 number, uint8 pos);
uint8 toggleBit(uint8 number, uint8 pos);
uint8 getBit(uint8 number, uint8 pos);
uint8 changeBit(uint8 number, uint8 pos, uint8 value);
uint64 merge(uint32 mostSignificant, uint32 leastSignificant);

int rand();

int random(int min, int max);

#endif /* INCLUDE_UTILS_H_ */
