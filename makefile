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

all: $(OBJECTS) build/uefi.efi

build/%.o: src/%.mish | $$(dir $$@)/.dirstamp
	objcopy -I binary -O elf32-i386 -B i386 --rename-section .data=.mish $^ $@

build/boot/uefi.o: src/boot/uefi.cpp
	gcc src/boot/uefi.cpp                             \
      -c                                 \
      -fno-stack-protector               \
      -fpic                              \
      -fshort-wchar                      \
      -mno-red-zone                      \
      -I gnu-efi/headers        \
      -I gnu-efi/headers/x86_64 \
      -DEFI_FUNCTION_WRAPPER             \
      -o build/boot/uefi.o

build/uefi.so: build/boot/uefi.o
	ld build/boot/uefi.o                         \
     gnu-efi/crt0-efi-x86_64.o     \
     -nostdlib                      \
     -znocombreloc                  \
     -T gnu-efi/elf_x86_64_efi.lds \
     -shared                        \
     -Bsymbolic                     \
     -L gnu-efi/libs               \
     -l:libgnuefi.a                 \
     -l:libefi.a                    \
     -o build/uefi.so

build/uefi.efi: build/uefi.so
	objcopy -j .text                \
          -j .sdata               \
          -j .data                \
          -j .dynamic             \
          -j .dynsym              \
          -j .rel                 \
          -j .rela                \
          -j .reloc               \
          --target=efi-app-x86_64 \
          build/uefi.so                 \
          build/uefi.efi

.PHONY:
elf: build/aura.elf
build/aura.elf: $(OBJECTS)
	ld -T make/linker.ld $(INCLUDE_FLAGS) $(OBJECTS) $(LIBS_FLAGS)

.PHONY:
run: private DFLAGS = $(if $(DEBUGGING),-s -S)
run: elf
	qemu-system-x86_64 -serial stdio $(DFLAGS) -cpu qemu64 -bios OVMF.fd #-kernel build/aura.elf

.PHONY:
iso: build/aura.iso
build/aura.iso: build/aura.elf
	cp build/aura.elf iso/boot/aura.elf
	genisoimage -R                        \
		-b boot/grub/stage2_eltorito      \
		-no-emul-boot                     \
		-boot-load-size 4                 \
		-A os                             \
		-input-charset utf8               \
		-quiet                            \
		-boot-info-table                  \
		-o build/aura.iso                 \
		iso