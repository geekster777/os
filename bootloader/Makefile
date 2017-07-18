BUILD_DIR=../build/bootloader

BOOTLOADER_SRCS := $(wildcard *.asm)
BOOTLOADER_OBJS := $(patsubst %.asm, $(BUILD_DIR)/%.o, $(BOOTLOADER_SRCS))

all: $(BOOTLOADER_OBJS)

$(BUILD_DIR)/%.o: %.asm
	test -d $(BUILD_DIR) || mkdir $(BUILD_DIR)
	nasm -f bin $< -o $@

clean:
	rm -r $(BUILD_DIR)/*
