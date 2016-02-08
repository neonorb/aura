CC=i686-elf-gcc
AS=i686-elf-as
myos: kernel.c linker.ld boot.s
		$(AS) boot.s -o boot.o
		$(CC) -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
		$(CC) -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
