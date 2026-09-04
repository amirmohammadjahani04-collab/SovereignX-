#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

typedef struct
{
    uint32_t *buffer;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;

} Framebuffer;

void clear_screen(Framebuffer *fb, uint32_t color);
void draw_rect(Framebuffer *fb,
uint32_t x, uint32_t y,
uint32_t w, uint32_t h,
uint32_t color);
void draw_circle(Framebuffer *fb, uint32_t cx, uint32_t cy, uint32_t radius, uint32_t color);

#endif
