CC=i686-elf-gcc
AS=i686-elf-as
compile: kernel.c linker.ld boot.s
		$(AS) boot.s -o boot.o
		$(CC) -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
		$(CC) -T linker.ld -o asiago.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
run-os: asiago.bin
		qemu-system-i386 -kernel asiago.bin
