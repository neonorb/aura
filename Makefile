CC=i686-elf-gcc
AS=i686-elf-as

compile: kernel/kernel.c kernel/interrupt.s utils/linker.ld boot/boot.s
		$(AS) boot/boot.s -o build/boot.o
		$(AS) kernel/interrupt.s -o build/interrupt.o
		$(CC) -c kernel/kernel.c -o build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Werror
		$(CC) -T utils/linker.ld -o build/asiago.bin -ffreestanding -O2 -nostdlib build/boot.o build/interrupt.o build/kernel.o -lgcc -Wall -Wextra -Werror
run-os: build/asiago.bin
		qemu-system-i386 -kernel build/asiago.bin
debug-os: build/asiago.bin
		$(AS) boot/boot.s -o build/boot.o
		$(AS) kernel/interrupt.s -o build/interrupt.o
		$(CC) -c kernel/kernel.c -o build/kernel.o -std=gnu99 -g -ffreestanding -O0 -Wall -Wextra -Werror
		$(CC) -T utils/linker.ld -o build/asiago.bin -ffreestanding -g -O0 -nostdlib build/boot.o build/interrupt.o build/kernel.o -lgcc -Wall -Wextra -Werror
		qemu-system-i386 -d int,pcall -s -S -kernel build/asiago.bin
