#include "boot_log.h"
#include "fb.h"
#include "graphics.h"
#include "text_render.h"

static void delay(volatile uint32_t count)
{
    while (count--)
    {
        asm volatile("nop");
    }
}

static void print_line(int *y, const char *text)
{
    draw_string(20, *y, text, 0x33FF66);
    *y += 16;
    fb_present();
    delay(30000000);
}

void boot_log_run(void)
{
    Framebuffer *fb = get_fb();
    clear_screen(fb, 0x000000);
    fb_present();

    int y = 20;

    draw_string(20, y, "SovereignX Boot Sequence", 0xFFFFFF);
    y += 24;
    fb_present();
    delay(20000000);

    print_line(&y, "[ OK ] Initializing framebuffer...");
    print_line(&y, "[ OK ] Setting up higher-half kernel...");
    print_line(&y, "[ OK ] Loading Global Descriptor Table...");
    print_line(&y, "[ OK ] Loading Interrupt Descriptor Table...");
    print_line(&y, "[ OK ] Remapping PIC...");
    print_line(&y, "[ OK ] Initializing PS/2 keyboard driver...");
    print_line(&y, "[ OK ] Initializing PS/2 mouse driver...");
    print_line(&y, "[ OK ] Initializing terminal subsystem...");
    print_line(&y, "[ OK ] Scanning PCI bus...");
    print_line(&y, "[ OK ] Initializing network interface (e1000)...");
    print_line(&y, "[ OK ] Starting window manager...");
    print_line(&y, "[ OK ] Loading desktop environment...");

    y += 16;
    draw_string(20, y, "SovereignX ready.", 0x33FF66);
    fb_present();
    delay(60000000);
}
