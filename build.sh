#!/bin/bash

# Compile the bootloader
nasm -f bin *.asm -o out/bootloader

# Recreate the disk image to be the correct size, and to hold the bootloader
rm out/disk.img
dd if=/dev/zero of=out/disk.img bs=512 count=2880
dd conv=notrunc if=out/bootloader of=out/disk.img bs=512
