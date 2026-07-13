#!/bin/bash
set -e

nasm -f bin src/boot/boot.asm -o build/boot.bin
nasm -f bin src/kernel/kernel.asm -o build/kernel.bin
cat build/boot.bin build/kernel.bin > build/os.img
truncate -s 1536 build/os.img

echo "Build complete: build/os.img"