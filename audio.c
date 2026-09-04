#include "audio.h"


static inline void outb(
unsigned short port,
unsigned char value)
{
    asm volatile(
        "outb %0,%1"
        :
        :"a"(value),"Nd"(port)
    );
}


void audio_init()
{
}


void beep()
{
    outb(0x61,3);

    for(volatile int i=0;i<100000;i++)
    {
    }

    outb(0x61,0);
}
