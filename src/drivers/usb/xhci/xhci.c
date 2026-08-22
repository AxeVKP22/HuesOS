#include "xhci.h"

void initXHCI(struct USBcontroller* usbcontroller) {
    outl(0xCF8, (1 << 31) | (usbcontroller->bus << 16) | (usbcontroller->device << 11) | (usbcontroller->function << 8) | 0x10);
    uint32_t data = inl(0xCFC);

    uint8_t BARType = (data >> 0) & 0x01;
    uint8_t BARMemoryType = (data >> 1) & 0x03;
    uint64_t BARAddress;

    if (BARType == 0) {

        if (BARMemoryType == 0) {
            BARAddress = data & 0xFFFFFFF0;

        } else if (BARMemoryType == 2) {
            // Handle 64-bit memory BAR
            outl(0xCF8, (1 << 31) | (usbcontroller->bus << 16) | (usbcontroller->device << 11) | (usbcontroller->function << 8) | 0x14);
            uint32_t BARAddressHigh = inl(0xCFC);
            BARAddress = ((uint64_t)BARAddressHigh << 32) | (data & 0xFFFFFFF0);
        }
    }

}