/* gdt.c - Implements the GDT table */
/* Copyright (C) 2016 Chris Smith */

/* This program is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or */
/* (at your option) any later version. */

/* This program is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/* GNU General Public License for more details. */

/* You should have received a copy of the GNU General Public License */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <int.h>

extern void gdt_flush();
extern void tss_flush();

// Write len copies of val into dest.
void memset(uint8 *dest, uint8 val, uint32 len) {
	uint8 *temp = (uint8 *) dest;
	for (; len != 0; len--)
		*temp++ = val;
}

typedef struct {
	uint16 limit_low;
	uint16 base_low;
	uint8 base_middle;
	uint8 access;
	uint8 granularity;
	uint8 base_high;
}__attribute__((packed)) GDTEntry;

typedef struct GDTPointer {
	uint16 limit;
	uint32 base;
}__attribute__((packed)) GDTPointer;

void gdtSetGate(signed int num, uint32 base, uint32 limit, uint8 access,
		uint8 gran);

typedef struct {
	uint32 prev_tss; // The previous TSS - if we used hardware task switching this would form a linked list.
	uint32 esp0;   // The stack pointer to load when we change to kernel mode.
	uint32 ss0;    // The stack segment to load when we change to kernel mode.
	uint32 esp1;       // Unused...
	uint32 ss1;
	uint32 esp2;
	uint32 ss2;
	uint32 cr3;
	uint32 eip;
	uint32 eflags;
	uint32 eax;
	uint32 ecx;
	uint32 edx;
	uint32 ebx;
	uint32 esp;
	uint32 ebp;
	uint32 esi;
	uint32 edi;
	uint32 es;     // The value to load into ES when we change to kernel mode.
	uint32 cs;     // The value to load into CS when we change to kernel mode.
	uint32 ss;     // The value to load into SS when we change to kernel mode.
	uint32 ds;     // The value to load into DS when we change to kernel mode.
	uint32 fs;     // The value to load into FS when we change to kernel mode.
	uint32 gs;     // The value to load into GS when we change to kernel mode.
	uint32 ldt;        // Unused...
	uint16 trap;
	uint16 iomap_base;
}__attribute__((packed)) TSSEntry;

GDTEntry gdtEntries[6];
GDTPointer gdtPointer;
TSSEntry tssEntry;

///Sets a gate in the GDT
void gdtSetGate(signed int num, uint32 base, uint32 limit, uint8 access,
		uint8 gran) {
	gdtEntries[num].base_low = (base & 0xFFFF);
	gdtEntries[num].base_middle = (base >> 16) & 0xFF;
	gdtEntries[num].base_high = (base >> 24) & 0xFF;

	gdtEntries[num].limit_low = (limit & 0xFFFF);
	gdtEntries[num].granularity = (limit >> 16) & 0x0F;

	gdtEntries[num].granularity |= gran & 0xF0;
	gdtEntries[num].access = access;
}

static void writeTss(int32 num, uint32 ss0, uint32 esp0) {
	uint32 base = (uint32) &tssEntry;
	uint32 limit = base + sizeof(tssEntry);

	gdtSetGate(num, base, limit, 0xE9, 0x00);

	memset(&tssEntry, 0, sizeof(tssEntry));

	tssEntry.ss0 = ss0;
	tssEntry.esp0 = esp0;

	tssEntry.cs = 0x0b;
	tssEntry.ss = tssEntry.ds = tssEntry.es = tssEntry.fs = tssEntry.gs = 0x13;
}

void gdt_init() {
	gdtPointer.limit = (sizeof(GDTEntry) * 6) - 1;
	gdtPointer.base = (uint32) &gdtEntries;

	gdtSetGate(0, 0, 0, 0, 0);                // Null segment
	gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
	gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
	gdtSetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
	gdtSetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
	writeTss(5, 0x10, 0x0);

	gdt_flush((uint32) &gdtPointer);

	tss_flush();
}
