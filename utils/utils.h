#ifndef UTILS_H_
#define UTILS_H_

#include "../kernel/log.h"

int array_length_char(char a[]) {
	return sizeof(&a) / sizeof(char);
}

void *memcpy(void *dst, const void *src, size_t len) {
	size_t i;

	/*
	 * memcpy does not support overlapping buffers, so always do it
	 * forwards. (Don't change this without adjusting memmove.)
	 *
	 * For speedy copying, optimize the common case where both pointers
	 * and the length are word-aligned, and copy word-at-a-time instead
	 * of byte-at-a-time. Otherwise, copy by bytes.
	 *
	 * The alignment logic below should be portable. We rely on
	 * the compiler to be reasonably intelligent about optimizing
	 * the divides and modulos out. Fortunately, it is.
	 */

	if ((uint32_t) dst % sizeof(long) == 0 && (uint32_t) src % sizeof(long) == 0
			&& len % sizeof(long) == 0) {

		long *d = dst;
		const long *s = src;

		for (i = 0; i < len / sizeof(long); i++) {
			d[i] = s[i];
		}
	} else {
		char *d = dst;
		const char *s = src;

		for (i = 0; i < len; i++) {
			d[i] = s[i];
		}
	}

	return dst;
}

size_t strlen(const char* str) {
	size_t ret = 0;
	while (str[ret] != 0)
		ret++;
	return ret;
}

void reverse(char s[]) {
	int i, j;
	char c;

	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

void itoa(int n, char s[]) {
	int i, sign;

	if ((sign = n) < 0) /* record sign */
		n = -n; /* make n positive */
	i = 0;
	do { /* generate digits in reverse order */
		s[i++] = n % 10 + '0'; /* get next digit */
	} while ((n /= 10) > 0); /* delete it */
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}

void concat(char* dest, char* first, char* second) {
	memcpy(dest, first, strlen(first));
	memcpy(dest + strlen(first), second, strlen(second));
}

unsigned int digitCount(unsigned int num){
	unsigned int digitCount = 0;

	do {
		digitCount++;
	} while (num /= 10);

	return digitCount;
}

void toString(char* dest, unsigned int num) {
	for (unsigned int i = 0; i < digitCount(num); i++) {
		dest[i] = 0;
	}
	itoa(num, dest);
}

#endif /* UTILS_H_ */
