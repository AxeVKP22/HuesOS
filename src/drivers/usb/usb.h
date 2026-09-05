#pragma once

#include "include/type.h"

#include "include/sys/io/io.h"

#include "include/lib/memcpy.h"

#define USB_CONTROLLER_NOT_FOUND 0x10

#define USB_CONTROLLER_XHCI 0x30
#define USB_CONTROLLER_EHCI 0x20

typedef struct USBcontroller {
    uint8_t bus;
    uint8_t device;
    uint8_t function;
    uint8_t classCode;
    uint8_t subclassCode;
    uint8_t progIF;
    uint32_t BARAddress;
}USBcontroller;

extern struct USBcontroller initUSB();