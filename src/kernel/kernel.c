

void kmain(void) {
    for (;;) {}
}

/* 
qemu-system-i386 -drive format=raw,file=build/os.img -monitor stdio

ndisasm -b 16 build/boot.bin 
ndisasm -b 16 -o 0x7C00 build/os.img

dd if=/home/axe/gitProjects/wowos/build/os.img of=/dev/sda bs=4M status=progress conv=fsync
stat -c %s build/kernel.bin
*/