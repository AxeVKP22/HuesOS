CC = gcc
LD = ld
NASM = nasm
OBJCOPY = objcopy

CFLAGS = -m32 -ffreestanding -fno-pie -fno-pic \
         -fno-stack-protector \
         -fno-asynchronous-unwind-tables \
         -fno-unwind-tables \
         -fomit-frame-pointer \
         -fno-builtin \
         -ffunction-sections \
         -fdata-sections \
         -Os \
         -Isrc

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

$(BUILD)/ps2Keyboard.o: src/drivers/ps2/keyboard/ps2Keyboard.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/ps2Handler.o: src/drivers/ps2/keyboard/asm/ps2Handler.S | $(BUILD)
	$(CC) $(CFLAGS) -D__ASSEMBLER__ -E -P $< -o $(BUILD)/ps2Handler.i
	$(NASM) -f elf32 $(BUILD)/ps2Handler.i -o $@

$(BUILD)/vga.o: src/drivers/vga/vga.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/pic.o: src/kernel/ints/pic/pic.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/idt.o: src/kernel/ints/idt/idt.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/lidt.o: src/kernel/ints/idt/asm/lidt.asm | $(BUILD)
	$(NASM) -f elf32 $< -o $@

$(BUILD)/memcpy.o: src/lib/asm/memcpy.asm | $(BUILD)
	$(NASM) -f elf32 $< -o $@

$(BUILD)/io.o: src/kernel/asm/io/io.asm | $(BUILD)
	$(NASM) -f elf32 $< -o $@

$(BUILD)/sti.o: src/kernel/asm/sti.asm | $(BUILD)
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
	$(BUILD)/io.o \
	$(BUILD)/sti.o \
	$(BUILD)/ps2Keyboard.o \
	$(BUILD)/ps2Handler.o \
	$(BUILD)/vga.o \
	$(BUILD)/pic.o \
	$(BUILD)/idt.o \
	$(BUILD)/lidt.o
	$(LD) $(LDFLAGS) $^ -o $@

$(BUILD)/kernel.bin: $(BUILD)/kernel.elf
	$(OBJCOPY) -O binary $< $@

$(BUILD)/os.img: \
	$(BUILD)/boot.bin \
	$(BUILD)/switch.bin \
	$(BUILD)/kernel.bin
	cat $^ > $@
	truncate -s 6144 $@
	stat -c '%s' $(BUILD)/kernel.bin
	@echo ".img file in build dir"

clean:
	rm -f $(BUILD)/*.o \
	      $(BUILD)/*.bin \
	      $(BUILD)/*.elf \
	      $(BUILD)/*.i \
	      $(BUILD)/os.img

.PHONY: all clean