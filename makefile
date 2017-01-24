# Makefile for Aura

CPPSOURCES=boot/uefi \
kernel/kernel kernel/liballoc kernel/log kernel/ports kernel/events \
implementation/implementation implementation/system/auramain implementation/system/syscalls implementation/system/console \
modules/modules \
modules/clock/clock modules/clock/pit modules/clock/rtc \
modules/keyboard/keyboard modules/keyboard/uefi \
modules/screen/screen modules/screen/uefi \
modules/power/power modules/power/uefi \
utils/utils utils/icxxabi
CSOURCES=
ASOURCES=
MSOURCES=mish/main

LIBS=feta mish

ARCHS=x86_64
-include ../make-base/make-base.mk

CFLAGS+=-nostdlib -ffreestanding -fno-rtti -fno-exceptions -fPIC -DEFI_FUNCTION_WRAPPER
MOBJECTS=$(patsubst %, build/%.o, $(MSOURCES))
OBJECTS-x86_64+=$(MOBJECTS)
INCLUDE_FLAGS+=-I gnu-efi/headers -I gnu-efi/headers/x86_64

# Mish "compiling"
build/%.o: src/%.mish | $$(dir $$@)/.dirstamp
	@objcopy -I binary -O elf64-x86-64 -B i386 --rename-section .data=.mish $^ $@

# building binaries
build/%/aura.so: $$(OBJECTS-$$(CURRENT_ARCH)) | $$(dir $$@)/.dirstamp
	@ld gnu-efi/crt0-efi-x86_64.o     \
	    $^ \
		-nostdlib                     \
		-znocombreloc                 \
		-T gnu-efi/elf_x86_64_efi.lds \
		-shared                       \
		-fPIC                         \
		-Bsymbolic                    \
		-L gnu-efi/libs               \
		-l:libgnuefi.a                \
		-l:libefi.a                   \
		$(LIB_FLAGS)                  \
		$(foreach L,$(LIBS),../$L/build/$(CURRENT_ARCH)/lib$L.a) \
		-o $@

REGULAR_SECTIONS=-j .text    \
				 -j .sdata   \
				 -j .data    \
				 -j .dynamic \
				 -j .dynsym  \
				 -j .rel     \
				 -j .rela    \
				 -j .reloc \
				 -j dynamicInit
DEBUG_SECTIONS=-j .debug_info     \
			   -j .debug_abbrev   \
			   -j .debug_loc      \
			   -j .debug_aranges  \
			   -j .debug_line     \
			   -j .debug_macinfo  \
			   -j .debug_str

OBJCOPY_FLAGS=--target=efi-app-x86_64
build/%/aura.efi: build/$$(CURRENT_ARCH)/aura.so | $$(dir $$@)/.dirstamp
	@objcopy $(REGULAR_SECTIONS) \
		$(OBJCOPY_FLAGS)        \
		$^ \
		$@

build/%/debug.aura.efi: build/$$(CURRENT_ARCH)/aura.so | $$(dir $$@)/.dirstamp
	@objcopy $(REGULAR_SECTIONS) $(DEBUG_SECTIONS) \
		$(OBJCOPY_FLAGS)      \
		$^ \
		$@
		
# ---- output files ----

IMG-all=$(foreach A,$(ARCHS),build/$A/aura.img)
.PHONY:
img: $(IMG-all)
build/%/aura.img: build/$$(CURRENT_ARCH)/aura.efi | $$(dir $$@)/.dirstamp
	@dd if=/dev/zero of=$@ bs=512 count=93750 status=none # allocate disk
	@parted $@ -s -a minimal mklabel gpt # make gpt table
	@parted $@ -s -a minimal mkpart EFI FAT16 2048s 93716s # make EFI partition
	@parted $@ -s -a minimal toggle 1 boot # make it bootable
	@dd if=/dev/zero of=/tmp/part.img bs=512 count=91669 status=none # allocate partition
	@mformat -i /tmp/part.img -h 32 -t 32 -n 64 -c 1 # format partition
	
	@if [ -d "build/img_root" ]; then rm -r build/img_root; fi
	
	@# build FS structure
	@mkdir build/img_root
	@mkdir build/img_root/EFI
	@mkdir build/img_root/EFI/BOOT
	@cp $^ build/img_root/EFI/BOOT/BOOTX64.efi # hard coded file name and path
	
	@mcopy -s -i /tmp/part.img build/img_root/* :: # copy FS to partition
	@dd if=/tmp/part.img of=$@ bs=512 count=91669 seek=2048 conv=notrunc status=none # copy parition to disk
	@rm /tmp/part.img # remove tmp partition file

.PHONY:
vdi: build/$(CURRENT_ARCH)/aura.vdi
build/%/aura.vdi: build/$(CURRENT_ARCH)/aura.img
	@vboxmanage convertfromraw --format VDI $^ $@
	
# ---- running ----

.PHONY:
run: img
	@qemu-system-x86_64 $(if $(DEBUGGING),-s -daemonize -serial file:/tmp/osoutput, $(if $(TESTING),,-serial stdio)) $(if $(TESTING),-nographic) -cpu qemu64 -bios OVMF.fd -drive file=build/x86_64/aura.img,if=ide,format=raw

# ---- debugging ----

.PHONY:
gdb: build/x86_64/debug.aura.efi

all: img
