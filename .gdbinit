target remote localhost:26000
layout asm
layout reg

add-symbol-file build/os/os.bin 0x100000
b *0x100060
c

