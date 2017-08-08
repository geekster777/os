BUILD_DIR=build
BOOTLOADER=$(BUILD_DIR)/bootloader/bootloader.o
OS=$(BUILD_DIR)/os/os.bin
DISK_IMG=disk.img

all: bootdisk

.PHONY: bootdisk bootloader

bootloader:
	test -d $(BUILD_DIR) || mkdir $(BUILD_DIR)
	make -C bootloader
	make -C os
    
bootdisk: bootloader
	dd if=/dev/zero of=$(DISK_IMG) bs=512 count=2880
	dd conv=notrunc if=$(BOOTLOADER) of=$(DISK_IMG) bs=512 count=1 seek=0
	dd conv=notrunc if=$(OS) of=$(DISK_IMG) bs=512 count=150 seek=1

run:
	qemu-system-i386 -machine q35 -fda $(DISK_IMG) -gdb tcp::26000 -localtime

debug:
	qemu-system-i386 -machine q35 -fda $(DISK_IMG) -gdb tcp::26000 -localtime -S

clean:
	make -C bootloader clean
	make -C os clean
