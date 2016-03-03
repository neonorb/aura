# Makefile for Project Asiago
# It can be structured way better, but it still beats manually compilation.

CC=i686-elf-gcc # Both the compiler and assembler need crosscompiled http://wiki.osdev.org/GCC_Cross-Compiler
AS=i686-elf-as
CFLAGS=-ffreestanding -Wall -Wextra -Werror # Considering removing Werror as it can be annoying
SOURCES= kernel/kernel.c kernel/interrupt.s utils/linker.ld boot/boot.s # This will likely increase
LIBS=-lgcc

compile-os: $(SOURCES)
		$(AS) boot/boot.s -o build/boot.o
		$(AS) kernel/interrupt.s -o build/interrupt.o
		$(CC) -c kernel/kernel.c -o build/kernel.o $(CFLAGS) -O2 -std=gnu99
		$(CC) -T utils/linker.ld -o build/asiago.bin  $(CFLAGS) $(LIBS) -O2 -nostdlib build/boot.o build/interrupt.o build/kernel.o
		@echo
		@echo Compiation of Asiago succeeded, boot with \"make run-os\"
		@echo
run-os: build/asiago.bin
		qemu-system-i386 -kernel build/asiago.bin
debug-os: $(SOURCES)
		$(AS) boot/boot.s -o build/boot.o
		$(AS) kernel/interrupt.s -o build/interrupt.o
		$(CC) -c kernel/kernel.c -o build/kernel.o $(CFLAGS) -g -std=gnu99 -O0
		$(CC) -T utils/linker.ld -o build/asiago.bin $(CFLAGS) $(LIBS) -g -O0 -nostdlib build/boot.o build/interrupt.o build/kernel.o
		@echo
		@echo Compilation of Asiago with debugging symbols and \-O0 succeeded, booting QEMU with debugging flags, connect with gdb.
		@echo
		qemu-system-i386 -d int,pcall -s -S -kernel build/asiago.bin
clean: 
		rm build/*o build/*bin
