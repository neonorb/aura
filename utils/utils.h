#ifndef UTILS_H_
#define UTILS_H_

#include "../kernel/log.h"

int array_length_char(char a[]) {
	return sizeof(&a) / sizeof(char);
}

size_t
strlen (const char *str)
{
  const char *char_ptr;
  const unsigned long int *longword_ptr;
  unsigned long int longword, himagic, lomagic;

  /* Handle the first few characters by reading one character at a time.
     Do this until CHAR_PTR is aligned on a longword boundary.  */
  for (char_ptr = str; ((unsigned long int) char_ptr
                        & (sizeof (longword) - 1)) != 0;
       ++char_ptr)
    if (*char_ptr == '\0')
      return char_ptr - str;

  /* All these elucidatory comments refer to 4-byte longwords,
     but the theory applies equally well to 8-byte longwords.  */

  longword_ptr = (unsigned long int *) char_ptr;

  /* Bits 31, 24, 16, and 8 of this number are zero.  Call these bits
     the "holes."  Note that there is a hole just to the left of
     each byte, with an extra at the end:

     bits:  01111110 11111110 11111110 11111111
     bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD

     The 1-bits make sure that carries propagate to the next 0-bit.
     The 0-bits provide holes for carries to fall into.  */
  himagic = 0x80808080L;
  lomagic = 0x01010101L;
  if (sizeof (longword) > 4)
    {
      /* 64-bit version of the magic.  */
      /* Do the shift in two steps to avoid a warning if long has 32 bits.  */
      himagic = ((himagic << 16) << 16) | himagic;
      lomagic = ((lomagic << 16) << 16) | lomagic;
    }
  /* Instead of the traditional loop which tests each character,
     we will test a longword at a time.  The tricky part is testing
     if *any of the four* bytes in the longword in question are zero.  */
  for (;;)
    {
      longword = *longword_ptr++;

      if (((longword - lomagic) & ~longword & himagic) != 0)
        {
          /* Which of the bytes was the zero?  If none of them were, it was
             a misfire; continue the search.  */

          const char *cp = (const char *) (longword_ptr - 1);

          if (cp[0] == 0)
            return cp - str;
          if (cp[1] == 0)
            return cp - str + 1;
          if (cp[2] == 0)
            return cp - str + 2;
          if (cp[3] == 0)
            return cp - str + 3;
          if (sizeof (longword) > 4)
            {
              if (cp[4] == 0)
                return cp - str + 4;
              if (cp[5] == 0)
                return cp - str + 5;
              if (cp[6] == 0)
                return cp - str + 6;
              if (cp[7] == 0)
                return cp - str + 7;
            }
        }
    }
}

void *memcpy(void *dst, const void *src, size_t len) {
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

static int memcmp_uint8_ts(unsigned long int a, unsigned long int b) {
	long int srcp1 = (long int) &a;
	long int srcp2 = (long int) &b;
	unsigned long int a0, b0;

	do {
		a0 = ((uint8_t *) srcp1)[0];
		b0 = ((uint8_t *) srcp2)[0];
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
			a0 = ((uint8_t *) srcp1)[0];
			b0 = ((uint8_t *) srcp2)[0];
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
		a0 = ((uint8_t *) srcp1)[0];
		b0 = ((uint8_t *) srcp2)[0];
		srcp1 += 1;
		srcp2 += 1;
		res = a0 - b0;
		if (res != 0)
			return res;
		len -= 1;
	}

	return 0;
}

uint8_t setBit(uint8_t number, uint8_t pos) {
	return number | 1 << pos;
}

uint8_t clearBit(uint8_t number, uint8_t pos) {
	return number & ~(1 << pos);
}

uint8_t toggleBit(uint8_t number, uint8_t pos) {
	return number ^ 1 << pos;
}

uint8_t getBit(uint8_t number, uint8_t pos) {
	return (number >> pos) & 1;
}

uint8_t changeBit(uint8_t number, uint8_t pos, uint8_t value) {
	return number ^ ((-value ^ number) & (1 << pos));
}

#endif /* UTILS_H_ */
