# Makefile for Aura
# It can be structured way better, but it still beats manually compilation.

CC=i686-elf-g++ # Both the compiler and assembler need crosscompiled http://wiki.osdev.org/GCC_Cross-Compiler
AS=nasm
CFLAGS=-ffreestanding -fno-exceptions -Wall -Wextra #-Werror # Considering removing Werror as it can be annoying
LDFLAGS=-T utils/linker.ld -melf_i386
SOURCES=kernel/kernel.cpp kernel/gdt.s kernel/idt.s utils/linker.ld boot/boot.s # This will likely increase
OUT=build/kernel.o build/boot.o build/gdt.o build/idt.o
INCLUDE=-I"../mish/include" -I"../feta/include"
LIBS=-L"../feta/Debug" -lfeta #../feta/Debug/libfeta.a #-L"../feta/Debug" -L"../mish/Debug" -lmish -lfeta

all: compile
compile: compile-os
compile-os: $(SOURCES)
		$(AS) -f elf boot/boot.s -o build/boot.o
		$(AS) -f elf kernel/gdt.s -o build/gdt.o
		$(AS) -f elf kernel/idt.s -o build/idt.o
		$(CC) -c kernel/kernel.cpp -o build/kernel.o $(CFLAGS) -O2 -nostdlib $(INCLUDE)
		$(CC) -T utils/linker.ld $(CFLAGS) -O2 -nostdlib $(OUT) $(INCLUDE) -o build/aura.bin $(LIBS)
		@echo
		@echo Compiation of Asura succeeded, boot with \"make run-os\"
		@echo
run-os: build/aura.bin
		qemu-system-i386 -serial stdio -kernel build/aura.bin
debug-os: $(SOURCES)
		$(AS) -f elf boot/boot.s -o build/boot.o
		$(AS) -f elf kernel/gdt.s -o build/gdt.o
		$(AS) -f elf kernel/idt.s -o build/idt.o
		$(CC) -c kernel/kernel.cpp -o build/kernel.o $(CFLAGS) -g -O0 -nostdlib $(INCLUDE)
		$(CC) -T utils/linker.ld $(CFLAGS) -g -O0 -nostdlib $(OUT) $(INCLUDE) -o build/aura.bin $(LIBS)
		@echo
		@echo Compilation of Asiago with debugging symbols and \-O0 succeeded, booting QEMU with debugging flags, connect with gdb.
		@echo
		qemu-system-i386 -serial stdio -d int,pcall -s -S -kernel build/aura.bin
	
build/kernel.elf: elf	
elf: $(OUT)
		ld $(LDFLAGS) $(OUT) $(LIBS) -o build/kernel.elf
iso: build/kernel.elf
		cp build/kernel.elf iso/boot/kernel.elf
		genisoimage -R								\
				-b boot/grub/stage2_eltorito		\
				-no-emul-boot						\
				-boot-load-size 4					\
				-A os								\
				-input-charset utf8					\
				-quiet								\
				-boot-info-table					\
				-o build/aura.iso					\
				iso
clean: 
		rm build/*o build/*bin build/*elf build/*iso
