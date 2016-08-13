#include <utils/utils.h>

#include <log.h>

void cli() {
	asm("cli");
}

void sti() {
	asm("sti");
}

int array_length_char(char a[]) {
	return sizeof(&a) / sizeof(char);
}

String toString(int value, wchar_t* str, int base) {
	wchar_t* rc;
	wchar_t* ptr;
	wchar_t* low;
	// Check for supported base.
	if (base < 2 || base > 36) {
		*str = '\0';
		return str;
	}
	rc = ptr = str;
	// Set '-' for negative decimals.
	if (value < 0 && base == 10) {
		*ptr++ = '-';
	}
	// Remember where the numbers start.
	low = ptr;
	// The actual conversion.
	do {
		// Modulo is negative for negative value. This trick makes abs() unnecessary.
		*ptr++ =
				"zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35
						+ value % base];
		value /= base;
	} while (value);
	// Terminating the string.
	*ptr-- = '\0';
	// Invert the numbers.
	while (low < ptr) {
		char tmp = *low;
		*low++ = *ptr;
		*ptr-- = tmp;
	}
	return rc;
}

void* memcpy(uint8* dst, const uint8* src, size_t len) {
	size_t i;

	/*
	 * memcpy does not support overlapping buffers, so always do it
	 * forwards. (Don't change this without adjusting memmove.)
	 *
	 * For speedy copying, optimize the common case where both pointers
	 * and the length are word-aligned, and copy word-at-a-time instead
	 * of uint8_t-at-a-time. Otherwise, copy by uint8_ts.
	 *
	 * The alignment logic below should be portable. We rely on
	 * the compiler to be reasonably intelligent about optimizing
	 * the divides and modulos out. Fortunately, it is.
	 */

	if ((uint64) dst % sizeof(uint64) == 0 && (uint64) src % sizeof(uint64) == 0
			&& len % sizeof(long) == 0) {

		uint32* d = (uint32*) dst;
		uint32* s = (uint32*) src;

		for (i = 0; i < len / sizeof(long); i++) {
			d[i] = s[i];
		}
	} else {
		uint8* d = (uint8*) dst;
		uint8* s = (uint8*) src;

		for (i = 0; i < len; i++) {
			d[i] = s[i];
		}
	}

	return dst;
}

String dynamicString(String string) {
	String retString = (String) create(strlen(string) + 1);
	memcpy((uint8*) retString, (uint8*) string, strlen(string) + 1);
	return retString;
}

// Write len copies of val into dest.
void memset(uint8* dest, uint8 val, size_t len) {
	for (uint64 i = 0; i < len; i++) {
		dest[i] = val;
	}
}

static int memcmp_uint8_ts(unsigned long int a, unsigned long int b) {
	long int srcp1 = (long int) &a;
	long int srcp2 = (long int) &b;
	unsigned long int a0, b0;

	do {
		a0 = ((uint8 *) srcp1)[0];
		b0 = ((uint8 *) srcp2)[0];
		srcp1 += 1;
		srcp2 += 1;
	} while (a0 == b0);
	return a0 - b0;
}

#  define MERGE(w0, sh_1, w1, sh_2) (((w0) << (sh_1)) | ((w1) >> (sh_2)))
# define CMP_LT_OR_GT(a, b) memcmp_uint8_ts ((a), (b))

int memcmp_not_common_alignment(long int srcp1, long int srcp2, size_t len) {
	unsigned long int a0, a1, a2, a3;
	unsigned long int b0, b1, b2, b3;
	unsigned long int x;
	int shl, shr;

	/* Calculate how to shift a word read at the memory operation
	 aligned srcp1 to make it aligned for comparison.  */

	shl = 8 * (srcp1 % sizeof(unsigned long int));
	shr = 8 * sizeof(unsigned long int) - shl;

	/* Make SRCP1 aligned by rounding it down to the beginning of the `unsigned long int'
	 it points in the middle of.  */
	srcp1 &= -sizeof(unsigned long int);

	switch (len % 4) {
	default: /* Avoid warning about uninitialized local variables.  */
	case 2:
		a1 = ((unsigned long int *) srcp1)[0];
		a2 = ((unsigned long int *) srcp1)[1];
		b2 = ((unsigned long int *) srcp2)[0];
		srcp1 -= 1 * sizeof(unsigned long int);
		srcp2 -= 2 * sizeof(unsigned long int);
		len += 2;
		goto do1;
	case 3:
		a0 = ((unsigned long int *) srcp1)[0];
		a1 = ((unsigned long int *) srcp1)[1];
		b1 = ((unsigned long int *) srcp2)[0];
		srcp2 -= 1 * sizeof(unsigned long int);
		len += 1;
		goto do2;
	case 0:
		if (16 <= 3 * sizeof(unsigned long int) && len == 0)
			return 0;
		a3 = ((unsigned long int *) srcp1)[0];
		a0 = ((unsigned long int *) srcp1)[1];
		b0 = ((unsigned long int *) srcp2)[0];
		srcp1 += 1 * sizeof(unsigned long int);
		goto do3;
	case 1:
		a2 = ((unsigned long int *) srcp1)[0];
		a3 = ((unsigned long int *) srcp1)[1];
		b3 = ((unsigned long int *) srcp2)[0];
		srcp1 += 2 * sizeof(unsigned long int);
		srcp2 += 1 * sizeof(unsigned long int);
		len -= 1;
		if (16 <= 3 * sizeof(unsigned long int) && len == 0)
			goto do0;
		/* Fall through.  */
	}

	do {
		a0 = ((unsigned long int *) srcp1)[0];
		b0 = ((unsigned long int *) srcp2)[0];
		x = MERGE(a2, shl, a3, shr);
		if (x != b3)
			return CMP_LT_OR_GT(x, b3);

		do3: a1 = ((unsigned long int *) srcp1)[1];
		b1 = ((unsigned long int *) srcp2)[1];
		x = MERGE(a3, shl, a0, shr);
		if (x != b0)
			return CMP_LT_OR_GT(x, b0);

		do2: a2 = ((unsigned long int *) srcp1)[2];
		b2 = ((unsigned long int *) srcp2)[2];
		x = MERGE(a0, shl, a1, shr);
		if (x != b1)
			return CMP_LT_OR_GT(x, b1);

		do1: a3 = ((unsigned long int *) srcp1)[3];
		b3 = ((unsigned long int *) srcp2)[3];
		x = MERGE(a1, shl, a2, shr);
		if (x != b2)
			return CMP_LT_OR_GT(x, b2);

		srcp1 += 4 * sizeof(unsigned long int);
		srcp2 += 4 * sizeof(unsigned long int);
		len -= 4;
	} while (len != 0);

	/* This is the right position for do0.  Please don't move
	 it into the loop.  */
	do0: x = MERGE(a2, shl, a3, shr);
	if (x != b3)
		return CMP_LT_OR_GT(x, b3);
	return 0;
}

int memcmp_common_alignment(long int srcp1, long int srcp2, size_t len) {
	unsigned long int a0, a1;
	unsigned long int b0, b1;

	switch (len % 4) {
	default: /* Avoid warning about uninitialized local variables.  */
	case 2:
		a0 = ((unsigned long int *) srcp1)[0];
		b0 = ((unsigned long int *) srcp2)[0];
		srcp1 -= 2 * sizeof(unsigned long int);
		srcp2 -= 2 * sizeof(unsigned long int);
		len += 2;
		goto do1;
	case 3:
		a1 = ((unsigned long int *) srcp1)[0];
		b1 = ((unsigned long int *) srcp2)[0];
		srcp1 -= sizeof(unsigned long int);
		srcp2 -= sizeof(unsigned long int);
		len += 1;
		goto do2;
	case 0:
		if (16 <= 3 * sizeof(unsigned long int) && len == 0)
			return 0;
		a0 = ((unsigned long int *) srcp1)[0];
		b0 = ((unsigned long int *) srcp2)[0];
		goto do3;
	case 1:
		a1 = ((unsigned long int *) srcp1)[0];
		b1 = ((unsigned long int *) srcp2)[0];
		srcp1 += sizeof(unsigned long int);
		srcp2 += sizeof(unsigned long int);
		len -= 1;
		if (16 <= 3 * sizeof(unsigned long int) && len == 0)
			goto do0;
		/* Fall through.  */
	}

	do {
		a0 = ((unsigned long int *) srcp1)[0];
		b0 = ((unsigned long int *) srcp2)[0];
		if (a1 != b1)
			return CMP_LT_OR_GT(a1, b1);

		do3: a1 = ((unsigned long int *) srcp1)[1];
		b1 = ((unsigned long int *) srcp2)[1];
		if (a0 != b0)
			return CMP_LT_OR_GT(a0, b0);

		do2: a0 = ((unsigned long int *) srcp1)[2];
		b0 = ((unsigned long int *) srcp2)[2];
		if (a1 != b1)
			return CMP_LT_OR_GT(a1, b1);

		do1: a1 = ((unsigned long int *) srcp1)[3];
		b1 = ((unsigned long int *) srcp2)[3];
		if (a0 != b0)
			return CMP_LT_OR_GT(a0, b0);

		srcp1 += 4 * sizeof(unsigned long int);
		srcp2 += 4 * sizeof(unsigned long int);
		len -= 4;
	} while (len != 0);

	/* This is the right position for do0.  Please don't move
	 it into the loop.  */
	do0: if (a1 != b1)
		return CMP_LT_OR_GT(a1, b1);
	return 0;
}

int memcmp(const void* s1, const void* s2, size_t len) {
	unsigned long int a0;
	unsigned long int b0;
	long int srcp1 = (long int) s1;
	long int srcp2 = (long int) s2;
	unsigned long int res;

	if (len >= 16) {
		/* There are at least some uint8_ts to compare.  No need to test
		 for LEN == 0 in this alignment loop.  */
		while (srcp2 % sizeof(unsigned long int) != 0) {
			a0 = ((uint8 *) srcp1)[0];
			b0 = ((uint8 *) srcp2)[0];
			srcp1 += 1;
			srcp2 += 1;
			res = a0 - b0;
			if (res != 0)
				return res;
			len -= 1;
		}

		/* SRCP2 is now aligned for memory operations on `unsigned long int'.
		 SRCP1 alignment determines if we can do a simple,
		 aligned compare or need to shuffle bits.  */

		if (srcp1 % sizeof(unsigned long int) == 0)
			res = memcmp_common_alignment(srcp1, srcp2,
					len / sizeof(unsigned long int));
		else
			res = memcmp_not_common_alignment(srcp1, srcp2,
					len / sizeof(unsigned long int));
		if (res != 0)
			return res;

		/* Number of uint8_ts remaining in the interval [0..sizeof(unsigned long int)-1].  */
		srcp1 += len & -sizeof(unsigned long int);
		srcp2 += len & -sizeof(unsigned long int);
		len %= sizeof(unsigned long int);
	}

	/* There are just a few uint8_ts to compare.  Use uint8_t memory operations.  */
	while (len != 0) {
		a0 = ((uint8 *) srcp1)[0];
		b0 = ((uint8 *) srcp2)[0];
		srcp1 += 1;
		srcp2 += 1;
		res = a0 - b0;
		if (res != 0)
			return res;
		len -= 1;
	}

	return 0;
}

uint8 setBit(uint8 number, uint8 pos) {
	return number | 1 << pos;
}

uint8 clearBit(uint8 number, uint8 pos) {
	return number & ~(1 << pos);
}

uint8 toggleBit(uint8 number, uint8 pos) {
	return number ^ 1 << pos;
}

uint8 getBit(uint8 number, uint8 pos) {
	return (number >> pos) & 1;
}

uint8 changeBit(uint8 number, uint8 pos, uint8 value) {
	return number ^ ((-value ^ number) & (1 << pos));
}

uint64 merge(uint32 mostSignificant, uint32 leastSignificant) {
	return ((uint64) mostSignificant << 32) | leastSignificant;
}

static unsigned int next = 1;
int rand() { // RAND_MAX assumed to be 32767
	next = next * 1103515245 + 12345;
	return (unsigned int) (next / 65536) % 32768;
}

int random(int min, int max) {
	return rand() % (max - min) + min;
}
