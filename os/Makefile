BUILD_DIR=../build/os
OS_IMG=$(BUILD_DIR)/os.bin
LINKER=linker.ld

CC = i686-elf-gcc
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
CLINKERFLAGS = -ffreestanding -O2 -nostdlib

OS_C_SRCS := $(wildcard *.c)
OS_C_OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(OS_C_SRCS))
OS_ASM_SRCS := $(wildcard *.asm)
OS_ASM_OBJS := $(patsubst %.asm, $(BUILD_DIR)/%.o, $(OS_ASM_SRCS))

all: $(OS_C_OBJS) $(OS_ASM_OBJS) link

$(BUILD_DIR)/%.o: %.asm
	test -d $(BUILD_DIR) || mkdir $(BUILD_DIR)
	nasm -felf32 $< -o $@

$(BUILD_DIR)/%.o: %.c
	test -d $(BUILD_DIR) || mkdir $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

link:
	$(CC) -T $(LINKER) -o $(OS_IMG) $(CLINKERFLAGS) $(OS_ASM_OBJS) $(OS_C_OBJS) -lgcc

clean:
	rm -r $(BUILD_DIR)/*
