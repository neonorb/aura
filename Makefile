CC=i686-elf-gcc
AS=i686-elf-as
compile: kernel/kernel.c linker.ld boot.s
		$(AS) boot.s -o build/boot.o
		$(CC) -c kernel/kernel.c -o build/kernel.o -std=gnu99 -g -ffreestanding -O0 -Wall -Wextra
		$(CC) -T linker.ld -o build/asiago.bin -ffreestanding -g -O0 -nostdlib build/boot.o build/kernel.o -lgcc
run-os: build/asiago.bin
		qemu-system-i386 -kernel build/asiago.bin
