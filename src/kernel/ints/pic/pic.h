#pragma once

#include "include/type.h"
#include "include/sys/io/io.h"
#include "include/lib/memcpy.h"


#define MASTER_CMD 0x20
#define MASTER_DATA 0x21

#define SLAVE_CMD 0xA0
#define SLAVE_DATA 0xA1

extern void remapPic();