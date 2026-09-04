#include "graphics.h"

void draw_rect(Framebuffer *fb,
uint32_t x, uint32_t y,
uint32_t w, uint32_t h,
uint32_t color)
{
    uint32_t stride = fb->pitch / 4;
    for (uint32_t yy = y; yy < y + h; yy++)
    {
        for (uint32_t xx = x; xx < x + w; xx++)
        {
            if (xx < fb->width && yy < fb->height)
                fb->buffer[yy * stride + xx] = color;
        }
    }
}

void draw_circle(Framebuffer *fb, uint32_t cx, uint32_t cy, uint32_t radius, uint32_t color)
{
    uint32_t stride = fb->pitch / 4;
    int r2 = radius * radius;
    for (int dy = -(int)radius; dy <= (int)radius; dy++)
    {
        for (int dx = -(int)radius; dx <= (int)radius; dx++)
        {
            if (dx*dx + dy*dy <= r2)
            {
                int xx = (int)cx + dx;
                int yy = (int)cy + dy;
                if (xx >= 0 && yy >= 0 && (uint32_t)xx < fb->width && (uint32_t)yy < fb->height)
                    fb->buffer[yy * stride + xx] = color;
            }
        }
    }
}

void clear_screen(Framebuffer *fb, uint32_t color)
{
    draw_rect(fb, 0, 0, fb->width, fb->height, color);
}
