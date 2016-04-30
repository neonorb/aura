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

extern void gdt_flush();
extern void tss_flush();

// Write len copies of val into dest.
void memset(uint8_t *dest, uint8_t val, uint32_t len) {
	uint8_t *temp = (uint8_t *) dest;
	for (; len != 0; len--)
		*temp++ = val;
}

typedef struct {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
}__attribute__((packed)) GDTEntry;

typedef struct GDTPointer {
	uint16_t limit;
	uint32_t base;
}__attribute__((packed)) GDTPointer;

void gdtSetGate(signed int num, uint32_t base, uint32_t limit, uint8_t access,
		uint8_t gran);

typedef struct {
	uint32_t prev_tss; // The previous TSS - if we used hardware task switching this would form a linked list.
	uint32_t esp0;   // The stack pointer to load when we change to kernel mode.
	uint32_t ss0;    // The stack segment to load when we change to kernel mode.
	uint32_t esp1;       // Unused...
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;     // The value to load into ES when we change to kernel mode.
	uint32_t cs;     // The value to load into CS when we change to kernel mode.
	uint32_t ss;     // The value to load into SS when we change to kernel mode.
	uint32_t ds;     // The value to load into DS when we change to kernel mode.
	uint32_t fs;     // The value to load into FS when we change to kernel mode.
	uint32_t gs;     // The value to load into GS when we change to kernel mode.
	uint32_t ldt;        // Unused...
	uint16_t trap;
	uint16_t iomap_base;
}__attribute__((packed)) TSSEntry;

GDTEntry gdtEntries[6];
GDTPointer gdtPointer;
TSSEntry tssEntry;

///Sets a gate in the GDT
void gdtSetGate(signed int num, uint32_t base, uint32_t limit, uint8_t access,
		uint8_t gran) {
	gdtEntries[num].base_low = (base & 0xFFFF);
	gdtEntries[num].base_middle = (base >> 16) & 0xFF;
	gdtEntries[num].base_high = (base >> 24) & 0xFF;

	gdtEntries[num].limit_low = (limit & 0xFFFF);
	gdtEntries[num].granularity = (limit >> 16) & 0x0F;

	gdtEntries[num].granularity |= gran & 0xF0;
	gdtEntries[num].access = access;
}

static void writeTss(int32_t num, uint32_t ss0, uint32_t esp0) {
	uint32_t base = (uint32_t) &tssEntry;
	uint32_t limit = base + sizeof(tssEntry);

	gdtSetGate(num, base, limit, 0xE9, 0x00);

	memset(&tssEntry, 0, sizeof(tssEntry));

	tssEntry.ss0 = ss0;
	tssEntry.esp0 = esp0;

	tssEntry.cs = 0x0b;
	tssEntry.ss = tssEntry.ds = tssEntry.es = tssEntry.fs = tssEntry.gs = 0x13;
}

void gdt_init() {
	gdtPointer.limit = (sizeof(GDTEntry) * 6) - 1;
	gdtPointer.base = (uint32_t) &gdtEntries;

	gdtSetGate(0, 0, 0, 0, 0);                // Null segment
	gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
	gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
	gdtSetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
	gdtSetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
	writeTss(5, 0x10, 0x0);

	gdt_flush((uint32_t) &gdtPointer);

	tss_flush();
}
