#include <stdint.h>

extern void mouse_move(int dx,int dy);
extern void mouse_click();

uint8_t mouse_cycle=0;
int8_t mouse_bytes[3];

void mouse_handler(uint8_t data)
{
    mouse_bytes[mouse_cycle]=data;
    mouse_cycle++;

    if(mouse_cycle==3)
    {
        mouse_cycle=0;

        int dx=mouse_bytes[1];
        int dy=-mouse_bytes[2];

        mouse_move(dx,dy);

        if(mouse_bytes[0]&1)
            mouse_click();
    }
}
