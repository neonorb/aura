# Makefile for Aura

CSOURCES=kernel/kernel
ASOURCES=kernel/gdt kernel/idt boot/boot

LIBS=feta mish

-include ../make-base/make-base.mk

build/aura.bin: $(COBJECTS) $(AOBJECTS)
	$(CC) -T make/linker.ld $(CFLAGS) $(DFLAGS) $(COBJECTS) $(AOBJECTS) $(INCLUDE_FLAGS) $(LIB_FLAGS) -o build/aura.bin
