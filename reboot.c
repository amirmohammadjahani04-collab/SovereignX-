#include "reboot.h"
#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void system_reboot(void)
{
    uint8_t status;
    do {
        status = inb(0x64);
    } while (status & 0x02);

    outb(0x64, 0xFE);

    for (;;)
    {
        asm volatile ("hlt");
    }
}
