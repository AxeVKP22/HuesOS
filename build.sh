#!/bin/bash
set -e

nasm -f bin src/boot.asm -o build/boot.bin
nasm -f bin src/k.asm -o build/k.bin
cat build/boot.bin build/k.bin > build/os.img
truncate -s 1024 build/os.img

echo "Build complete: build/os.img"