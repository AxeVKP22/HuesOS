#!/bin/bash
set -e

nasm -f bin src/boot/boot.asm -o build/boot.bin

gcc -m16 -ffreestanding -Os -fno-pie -fno-stack-protector -c -mno-sse -mno-sse2 -mno-mmx src/kernel/kernel.c -o build/kernel.o
gcc -m16 -ffreestanding -Os -fno-pie -fno-stack-protector -c -mno-sse -mno-sse2 -mno-mmx src/fs/fs.c -o build/fs.o
gcc -m16 -ffreestanding -Os -fno-pie -fno-stack-protector -c -mno-sse -mno-sse2 -mno-mmx src/kernel/mem/fd.c -o build/fd.o

gcc -m16 -ffreestanding -Os -fno-pie -fno-stack-protector -c -mno-sse -mno-sse2 -mno-mmx src/lib/vga.c -o build/vgac.o
gcc -m16 -ffreestanding -Os -fno-pie -fno-stack-protector -c -mno-sse -mno-sse2 -mno-mmx src/lib/string.c -o build/string.o

gcc -m16 -ffreestanding -Os -fno-pie -fno-stack-protector -c -mno-sse -mno-sse2 -mno-mmx src/bin/con.c -o build/con.o
gcc -m16 -ffreestanding -Os -fno-pie -fno-stack-protector -c -mno-sse -mno-sse2 -mno-mmx src/bin/ddump.c -o build/ddump.o

nasm -f elf32 src/kernel/asm/vga.asm -o build/vga.o
nasm -f elf32 src/kernel/asm/keyboard.asm -o build/keyboard.o
nasm -f elf32 src/kernel/asm/power.asm -o build/power.o
nasm -f elf32 src/kernel/asm/disk.asm -o build/disk.o
nasm -f elf32 src/kernel/asm/mem.asm -o build/mem.o

nasm -f elf32 src/kernel/asm/syscall.asm -o build/syscall.o

ld -m elf_i386 -T linker.ld build/kernel.o build/fd.o build/fs.o build/keyboard.o build/vga.o build/vgac.o build/power.o build/disk.o build/mem.o build/syscall.o build/string.o build/con.o build/ddump.o -o build/kernel.bin -Map=build/kernel.map

dd if=/dev/zero bs=512 count=1 > empty.bin
cat build/boot.bin empty.bin build/kernel.bin > build/os.img
truncate -s 12800 build/os.img

echo "Build complete: build/os.img"