BUILD_DIR=build
BOOTLOADER=$(BUILD_DIR)/bootloader/bootloader.o
DISK_IMG=disk.img

all: bootdisk

.PHONY: bootdisk bootloader

bootloader:
	mkdir $(BUILD_DIR)
	make -C bootloader
    
bootdisk: bootloader
	dd if=/dev/zero of=$(DISK_IMG) bs=512 count=2880
	dd conv=notrunc if=$(BOOTLOADER) of=$(DISK_IMG) bs=512

run:
	qemu-system-i386 -machine q35 -fda $(DISK_IMG) -gdb tcp::260000

debug:
	qemu-system-i386 -machine q35 -fda $(DISK_IMG) -gdb tcp::260000 -S

clean:
	make -C bootloader clean
