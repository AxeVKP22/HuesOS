#!/bin/bash
set -e

nasm -f bin src/boot/boot.asm -o build/boot.bin

dd if=/dev/zero bs=512 count=1 > empty.bin
cat build/boot.bin >> build/os.img
truncate -s 12800 build/os.img

echo "Build complete: build/os.img"