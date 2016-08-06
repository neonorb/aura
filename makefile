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

build/boot/uefi.o: src/boot/uefi.c | build/boot/.dirstamp
	gcc src/boot/uefi.c           \
		-c                        \
		-fno-stack-protector      \
		-fpic                     \
		-fshort-wchar             \
		-mno-red-zone             \
		-I gnu-efi/headers        \
		-I gnu-efi/headers/x86_64 \
		-DEFI_FUNCTION_WRAPPER    \
		-o build/boot/uefi.o

build/uefi.so: build/boot/uefi.o | build/.dirstamp
	ld build/boot/uefi.o              \
		gnu-efi/crt0-efi-x86_64.o     \
		-nostdlib                     \
		-znocombreloc                 \
		-T gnu-efi/elf_x86_64_efi.lds \
		-shared                       \
		-Bsymbolic                    \
		-L gnu-efi/libs               \
		-l:libgnuefi.a                \
		-l:libefi.a                   \
		-o build/uefi.so

build/uefi.efi: build/uefi.so | build/.dirstamp
	objcopy -j .text            \
		-j .sdata               \
		-j .data                \
		-j .dynamic             \
		-j .dynsym              \
		-j .rel                 \
		-j .rela                \
		-j .reloc               \
		--target=efi-app-x86_64 \
		build/uefi.so           \
		build/uefi.efi

.PHONY:
img: build/aura.img
build/aura.img: build/uefi.efi | build/.dirstamp
	dd if=/dev/zero of=build/aura.img bs=512 count=93750
	parted build/aura.img -s -a minimal mklabel gpt
	parted build/aura.img -s -a minimal mkpart EFI FAT16 2048s 93716s
	parted build/aura.img -s -a minimal toggle 1 boot
	dd if=/dev/zero of=/tmp/part.img bs=512 count=91669
	mformat -i /tmp/part.img -h 32 -t 32 -n 64 -c 1
	mcopy -i /tmp/part.img build/uefi.efi ::uefi.efi
	dd if=/tmp/part.img of=build/aura.img bs=512 count=91669 seek=2048 conv=notrunc

#.PHONY:
#elf: build/aura.elf
#build/aura.elf: $(OBJECTS)
#	ld -T make/linker.ld $(INCLUDE_FLAGS) $(OBJECTS) $(LIBS_FLAGS)

.PHONY:
run: private DFLAGS = $(if $(DEBUGGING),-s -S)
run: img
	qemu-system-x86_64 -serial stdio $(DFLAGS) -cpu qemu64 -bios OVMF.fd -drive file=build/aura.img,if=ide

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