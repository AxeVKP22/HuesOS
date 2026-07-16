#!/bin/bash
set -e

nasm -f bin src/boot/boot.asm -o build/boot.bin

gcc -m16 -ffreestanding -fno-pie -fno-stack-protector -c -mno-sse -mno-sse2 -mno-mmx src/kernel/kernel.c -o build/kernel.o
gcc -m16 -ffreestanding -fno-pie -fno-stack-protector -c -mno-sse -mno-sse2 -mno-mmx src/kernel/fs/fs.c -o build/fs.o
gcc -m16 -ffreestanding -fno-pie -fno-stack-protector -c -mno-sse -mno-sse2 -mno-mmx src/kernel/vga.c -o build/vgac.o

gcc -m16 -ffreestanding -fno-pie -fno-stack-protector -c -mno-sse -mno-sse2 -mno-mmx src/bin/con.c -o build/con.o

nasm -f elf32 src/kernel/asm/vga.asm -o build/vga.o
nasm -f elf32 src/kernel/asm/keyboard.asm -o build/keyboard.o
nasm -f elf32 src/kernel/asm/power.asm -o build/power.o
nasm -f elf32 src/kernel/asm/disk.asm -o build/disk.o
nasm -f elf32 src/kernel/asm/mem.asm -o build/mem.o

ld -m elf_i386 -T linker.ld build/kernel.o build/fs.o build/keyboard.o build/vga.o build/vgac.o  build/power.o build/disk.o build/mem.o build/con.o -o build/kernel.bin

dd if=/dev/zero bs=512 count=1 > empty.bin
cat build/boot.bin empty.bin build/kernel.bin > build/os.img
truncate -s 6144 build/os.img

echo "Build complete: build/os.img"