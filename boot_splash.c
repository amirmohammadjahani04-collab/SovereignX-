#include "boot_logo.h"
#include "fb.h"
#include "graphics.h"

static void delay(volatile uint64_t count)
{
    while (count--)
    {
        asm volatile("nop");
    }
}

void boot_splash_draw(void)
{
    Framebuffer *fb = get_fb();

    clear_screen(fb, 0x000000);

    uint32_t start_x = (fb->width - BOOT_LOGO_WIDTH) / 2;
    uint32_t start_y = (fb->height - BOOT_LOGO_HEIGHT) / 2;
    uint32_t stride = fb->pitch / 4;

    for (uint32_t y = 0; y < BOOT_LOGO_HEIGHT; y++)
    {
        for (uint32_t x = 0; x < BOOT_LOGO_WIDTH; x++)
        {
            uint32_t color = boot_logo_pixels[y * BOOT_LOGO_WIDTH + x];
            fb->buffer[(start_y + y) * stride + (start_x + x)] = color;
        }
    }

    delay(150000000);
}
