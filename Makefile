# Makefile for Aura
# It can be structured way better, but it still beats manually compilation.

CC=i686-elf-g++ # Both the compiler and assembler need crosscompiled http://wiki.osdev.org/GCC_Cross-Compiler
AS=nasm
CFLAGS=-ffreestanding -fno-exceptions -Wall -Wextra -nostdlib #-Werror # Considering removing Werror as it can be annoying
LDFLAGS=-T utils/linker.ld -melf_i386
SOURCES=src/kernel/kernel.cpp src/kernel/gdt.s src/kernel/idt.s src/utils/linker.ld src/boot/boot.s # This will likely increase
OUT=build/kernel.o build/boot.o build/gdt.o build/idt.o
INCLUDE=-I "include" -I"../mish/include" -I"../feta/include"
LIBS=-L"../feta/Debug" -L"../mish/Debug" -lmish -lfeta

all: compile
compile: private DFLAGS = $(if $(DEBUGGING), -g -Og, -O2)
compile: private TFLAGS = $(if $(TESTING),-D TESTING)
compile: $(SOURCES)
	$(AS) -f elf src/boot/boot.s -o build/boot.o
	$(AS) -f elf src/kernel/gdt.s -o build/gdt.o
	$(AS) -f elf src/kernel/idt.s -o build/idt.o
	$(CC) -c src/kernel/kernel.cpp -o build/kernel.o $(CFLAGS) $(DFLAGS) $(INCLUDE) $(TFLAGS)
	$(CC) -T src/utils/linker.ld $(CFLAGS) $(DFLAGS) $(OUT) $(INCLUDE) -o build/aura.bin $(LIBS) $(TFLAGS)
	@echo
	@echo Compiation of Aura succeeded, boot with \"make run\"
	@echo

run: private DFLAGS = $(if $(DEBUGGING),-s -S)
run: build/aura.bin
	qemu-system-i386 -serial stdio $(DFLAGS) -kernel build/aura.bin -m 2
test:
	TESTING=true make compile
	TESTING=true make run
test-debug:
	TESTING=true DEBUGGING=true make compile
	DEBUGGING=true make run
debug: $(SOURCES)
	DEBUGGING=true make compile
	DEBUGGING=true make run

build/kernel.elf: elf
elf: $(OUT)
	ld $(LDFLAGS) $(OUT) $(LIBS) -o build/kernel.elf
iso: build/kernel.elf
	cp build/kernel.elf iso/boot/kernel.elf
	genisoimage -R							  \
			-b boot/grub/stage2_eltorito		\
			-no-emul-boot					   \
			-boot-load-size 4				   \
			-A os							   \
			-input-charset utf8				 \
			-quiet							  \
			-boot-info-table					\
			-o build/aura.iso				   \
			iso
clean:
	rm build/*o build/*bin build/*elf build/*iso
