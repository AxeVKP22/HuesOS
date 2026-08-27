CC = gcc
LD = ld
NASM = nasm
OBJCOPY = objcopy

CFLAGS = -m32 -ffreestanding -fno-pie -fno-pic \
         -fno-stack-protector \
         -fno-asynchronous-unwind-tables \
         -fno-unwind-tables \
         -Os

LDFLAGS = -m elf_i386 -T linker.ld

BUILD = build

all: $(BUILD)/os.img

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/kernel.o: src/kernel/kernel.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/usbCore.o: src/drivers/usb/core/usbCore.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/xhci.o: src/drivers/usb/xhci/xhci.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/memcpy.o: src/lib/asm/memcpy.asm | $(BUILD)
	$(NASM) -f elf32 $< -o $@

$(BUILD)/io.o: src/kernel/asm/io/io.asm | $(BUILD)
	$(NASM) -f elf32 $< -o $@

$(BUILD)/boot.bin: src/boot/boot.asm | $(BUILD)
	$(NASM) -f bin $< -o $@

$(BUILD)/switch.bin: src/boot/switch.asm | $(BUILD)
	$(NASM) -f bin $< -o $@

$(BUILD)/kernel.elf: \
	$(BUILD)/kernel.o \
	$(BUILD)/usbCore.o \
	$(BUILD)/xhci.o \
	$(BUILD)/memcpy.o \
	$(BUILD)/io.o
	$(LD) $(LDFLAGS) $^ -o $@

$(BUILD)/kernel.bin: $(BUILD)/kernel.elf
	$(OBJCOPY) -O binary $< $@

$(BUILD)/os.img: \
	$(BUILD)/boot.bin \
	$(BUILD)/switch.bin \
	$(BUILD)/kernel.bin
	cat $^ > $@
	truncate -s 2048 $@
	stat -c '%s' $(BUILD)/kernel.bin
	@echo ".img file in build dir"

clean:
	rm -f $(BUILD)/*.o \
	      $(BUILD)/*.bin \
	      $(BUILD)/kernel.elf \
	      $(BUILD)/os.img

.PHONY: all clean