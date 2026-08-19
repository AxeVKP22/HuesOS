# HuesOS
most unstable os ever

Build:

```bash
./build.sh
```

## Interrupts
0x00 - open(const char* filename, uint8_t flags);

0x01 - close(int fd);

0x02 - read(int fd, void* buffer, uint16_t size);

0x03 - write(int fd, void* buffer, uint16_t size);

0x04 - new(const char* filename);

