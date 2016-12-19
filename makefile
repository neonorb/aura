# Makefile for Aura

CSOURCES=boot/uefi \
kernel/kernel kernel/liballoc kernel/log kernel/ports kernel/events \
implementation/implementation implementation/system/auramain implementation/system/syscalls implementation/system/console \
modules/modules \
modules/clock/clock modules/clock/pit modules/clock/rtc \
modules/keyboard/keyboard modules/keyboard/uefi \
modules/screen/screen modules/screen/uefi \
utils/utils utils/icxxabi
ASOURCES=
MSOURCES=mish/main

LIBS=feta mish

-include ../make-base/make-base.mk

CFLAGS+=-nostdlib -ffreestanding -fno-rtti -fno-exceptions
MOBJECTS=$(patsubst %, build/%.o, $(MSOURCES))
OBJECTS:=$(OBJECTS) $(MOBJECTS)
INCLUDE_FLAGS:=$(INCLUDE_FLAGS) -I gnu-efi/headers -I gnu-efi/headers/x86_64

all: $(OBJECTS)

# Mish "compiling"
build/%.o: src/%.mish | $$(dir $$@)/.dirstamp
	@objcopy -I binary -O elf64-x86-64 -B i386 --rename-section .data=.mish $^ $@

# building binaries
build/aura.so: $(OBJECTS) | build/.dirstamp
	@ld $(OBJECTS)                    \
		gnu-efi/crt0-efi-x86_64.o     \
		-nostdlib                     \
		-znocombreloc                 \
		-T gnu-efi/elf_x86_64_efi.lds \
		-shared                       \
		-fPIC                         \
		-Bsymbolic                    \
		-L gnu-efi/libs               \
		-l:libgnuefi.a                \
		-l:libefi.a                   \
		$(LIBS_FLAGS)                 \
		-o build/aura.so

REGULAR_SECTIONS=-j .text    \
				 -j .sdata   \
				 -j .data    \
				 -j .dynamic \
				 -j .dynsym  \
				 -j .rel     \
				 -j .rela    \
				 -j .reloc   \
				 -j .mish
DEBUG_SECTIONS=-j .debug_info     \
			   -j .debug_abbrev   \
			   -j .debug_loc      \
			   -j .debug_aranges  \
			   -j .debug_line     \
			   -j .debug_macinfo  \
			   -j .debug_debugstr \

OBJCOPY_FLAGS=--target=efi-app-x86_64
build/aura.efi: build/aura.so | build/.dirstamp
	@objcopy $(REGULAR_SECTIONS) \
		$(OBJCOPY_FLAGS)        \
		build/aura.so           \
		build/aura.efi

build/debug.aura.efi: build/aura.so | build/.dirstamp
	@objcopy $(DEBUG_SECTIONS) \
		$(OBJCOPY_FLAGS)      \
		build/aura.so         \
		build/debug.aura.efi	
		
# ---- output files ----

.PHONY:
img: build/aura.img
build/aura.img: build/aura.efi | build/.dirstamp
	@dd if=/dev/zero of=build/aura.img bs=512 count=93750 status=none # allocate disk
	@parted build/aura.img -s -a minimal mklabel gpt # make gpt table
	@parted build/aura.img -s -a minimal mkpart EFI FAT16 2048s 93716s # make EFI partition
	@parted build/aura.img -s -a minimal toggle 1 boot # make it bootable
	@dd if=/dev/zero of=/tmp/part.img bs=512 count=91669 status=none # allocate partition
	@mformat -i /tmp/part.img -h 32 -t 32 -n 64 -c 1 # format partition
	
	@if [ -d "build/img_root" ]; then rm -r build/img_root; fi
	
	@# build FS structure
	@mkdir build/img_root
	@mkdir build/img_root/EFI
	@mkdir build/img_root/EFI/BOOT
	@cp build/aura.efi build/img_root/EFI/BOOT/BOOTX64.efi # hard coded file name and path
	
	@mcopy -s -i /tmp/part.img build/img_root/* :: # copy FS to partition
	@dd if=/tmp/part.img of=build/aura.img bs=512 count=91669 seek=2048 conv=notrunc status=none # copy parition to disk
	@rm /tmp/part.img # remove tmp partition file

.PHONY:
vdi: build/aura.vdi
build/aura.vdi: build/aura.img
	@vboxmanage convertfromraw --format VDI build/aura.img build/aura.vdi
	
# ---- running ----

.PHONY:
run: private DFLAGS = $(if $(DEBUGGING),-s)
run: img
	@qemu-system-x86_64 -serial stdio $(DFLAGS) -cpu qemu64 -bios OVMF.fd -drive file=build/aura.img,if=ide

# ---- debugging ----

.PHONY:
gdb: build/debug.aura.efi
	


