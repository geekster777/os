#!/bin/bash

# Check if we're waiting on gdb to start 
GDB_Flag=""
if [ "$1" = "gdb" ]; then
    GDB_Flag="-S"
fi

# Run qemu with the disk image we made. 
qemu-system-i386 -machine q35 -fda out/disk.img -gdb tcp::26000 $GDB_Flag
