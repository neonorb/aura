# Makefile for Aura

CSOURCES=kernel/gdt kernel/idt kernel/kernel kernel/liballoc kernel/log kernel/ports kernel/acpi \
implementation/implementation \
modules/clock/clock modules/clock/pit modules/clock/rtc \
modules/keyboard/keyboard modules/keyboard/ps2 \
modules/screen/screen modules/screen/vga \
utils/utils
ASOURCES=kernel/gdtasm kernel/idtasm boot/boot
MSOURCES=mish/main

LIBS=feta mish

-include ../make-base/make-base.mk

MOBJECTS=$(patsubst %, build/%.o, $(MSOURCES))
OBJECTS:=$(OBJECTS) $(MOBJECTS)

all: $(OBJECTS)

build/%.o: src/%.mish | $$(dir $$@)/.dirstamp
	objcopy -I binary -O elf32-i386 -B i386 --rename-section .data=.mish $^ $@

build/aura.elf: $(OBJECTS)
	ld -melf_i386 -T make/linker.ld $(INCLUDE_FLAGS) $(OBJECTS) $(LIBS_FLAGS) -o build/aura.elf

.PHONY:
elf: build/aura.elf

.PHONY:
run: private DFLAGS = $(if $(DEBUGGING),-s -S)
run: elf
	qemu-system-x86_64 -serial stdio $(DFLAGS) -kernel build/aura.elf