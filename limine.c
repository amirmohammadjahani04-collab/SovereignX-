#include "limine.h"
#include "fb.h"

__attribute__((used, section(".limine_requests")))
static volatile uint64_t base_revision[] = {
    LIMINE_BASE_REVISION(3)
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t start_marker[] = {
    LIMINE_REQUESTS_START_MARKER
};

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t end_marker[] = {
    LIMINE_REQUESTS_END_MARKER
};

#define MAX_FB_WIDTH 1024
#define MAX_FB_HEIGHT 768

static Framebuffer real_fb;
static Framebuffer back_fb;
static uint32_t backbuffer_data[MAX_FB_WIDTH * MAX_FB_HEIGHT];

void fb_init(void)
{
    real_fb.buffer = (uint32_t*)framebuffer_request.response->framebuffers[0]->address;
    real_fb.width  = framebuffer_request.response->framebuffers[0]->width;
    real_fb.height = framebuffer_request.response->framebuffers[0]->height;
    real_fb.pitch  = framebuffer_request.response->framebuffers[0]->pitch;

    back_fb.buffer = backbuffer_data;
    back_fb.width  = real_fb.width;
    back_fb.height = real_fb.height;
    back_fb.pitch  = real_fb.width * 4;
}

Framebuffer *get_fb(void)
{
    return &back_fb;
}

void fb_present(void)
{
    uint32_t real_stride = real_fb.pitch / 4;
    uint32_t back_stride = back_fb.pitch / 4;

    for (uint32_t y = 0; y < real_fb.height; y++)
    {
        for (uint32_t x = 0; x < real_fb.width; x++)
        {
            real_fb.buffer[y * real_stride + x] = back_fb.buffer[y * back_stride + x];
        }
    }
}
