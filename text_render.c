#include "text_render.h"
#include "font8x8.h"
#include "fb.h"
#include "graphics.h"

void draw_char(int x, int y, char c, uint32_t color)
{
    if (c < FONT_FIRST_CHAR || c > FONT_LAST_CHAR)
        return;

    Framebuffer *fb = get_fb();
    uint32_t stride = fb->pitch / 4;
    const uint8_t *glyph = font8x8_basic[c - FONT_FIRST_CHAR];

    for (int row = 0; row < FONT_HEIGHT; row++)
    {
        uint8_t bits = glyph[row];
        for (int col = 0; col < FONT_WIDTH; col++)
        {
            if (bits & (0x80 >> col))
            {
                int px = x + col;
                int py = y + row;
                if (px >= 0 && py >= 0 && (uint32_t)px < fb->width && (uint32_t)py < fb->height)
                    fb->buffer[py * stride + px] = color;
            }
        }
    }
}

void draw_string(int x, int y, const char *s, uint32_t color)
{
    int cx = x;
    while (*s)
    {
        if (*s == '\n')
        {
            cx = x;
            y += FONT_HEIGHT + 2;
        }
        else
        {
            draw_char(cx, y, *s, color);
            cx += FONT_WIDTH;
        }
        s++;
    }
}
