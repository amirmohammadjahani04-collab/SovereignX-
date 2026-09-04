#include <stdint.h>

void draw_text(
    int x,
    int y,
    const char *text,
    uint32_t color
)
{
    volatile char *video = (volatile char*)0xb8000;

    int pos = y * 80 + x;

    for(int i=0; text[i] != 0; i++)
    {
        video[pos*2] = text[i];
        video[pos*2+1] = 0x07;
        pos++;
    }
}
