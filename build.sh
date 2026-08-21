gcc -m32 -ffreestanding -fno-pie -fno-pic \
    -fno-stack-protector \
    -fno-asynchronous-unwind-tables \
    -fno-unwind-tables \
    -Os \
    -c src/kernel/kernel.c \
    -o build/kernel.o

nasm -f bin src/boot/boot.asm -o build/boot.bin
nasm -f bin src/boot/switch.asm -o build/switch.bin

nasm -f elf32 src/lib/asm/memcpy.asm -o build/memcpy.o

ld -m elf_i386 -T linker.ld \
    build/kernel.o build/memcpy.o \
    -o build/kernel.elf

objcopy -O binary build/kernel.elf build/kernel.bin

cat build/boot.bin build/switch.bin build/kernel.bin > build/os.img
truncate -s 2048 build/os.img

stat -c '%s' build/kernel.bin

rm build/*.o build/*.bin build/kernel.elf

echo "goooooood"