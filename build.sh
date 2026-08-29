gcc -m32 -ffreestanding -fno-pie -fno-pic \
    -fno-stack-protector \
    -fno-asynchronous-unwind-tables \
    -fno-unwind-tables \
    -Os \
    -Isrc \
    -c src/kernel/kernel.c \
    -o build/kernel.o

gcc -m32 -ffreestanding -fno-pie -fno-pic \
    -fno-stack-protector \
    -fno-asynchronous-unwind-tables \
    -fno-unwind-tables \
    -Os \
    -Isrc \
    -c src/drivers/usb/core/usbCore.c \
    -o build/usbCore.o

gcc -m32 -ffreestanding -fno-pie -fno-pic \
    -fno-stack-protector \
    -fno-asynchronous-unwind-tables \
    -fno-unwind-tables \
    -Os \
    -Isrc \
    -c src/drivers/usb/xhci/xhci.c \
    -o build/xhci.o

nasm -f bin src/boot/boot.asm -o build/boot.bin
nasm -f bin src/boot/switch.asm -o build/switch.bin

nasm -f elf32 src/lib/asm/memcpy.asm -o build/memcpy.o
nasm -f elf32 src/kernel/asm/io/io.asm -o build/io.o

ld -m elf_i386 -T linker.ld \
    build/kernel.o build/usbCore.o build/xhci.o build/memcpy.o build/io.o \
    -o build/kernel.elf

objcopy -O binary build/kernel.elf build/kernel.bin

cat build/boot.bin build/switch.bin build/kernel.bin > build/os.img
truncate -s 2048 build/os.img

stat -c '%s' build/kernel.bin

rm build/*.o build/*.bin build/kernel.elf

echo ".img file in build dir"