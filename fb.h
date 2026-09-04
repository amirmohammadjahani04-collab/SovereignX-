#ifndef FB_H
#define FB_H
#include <stdint.h>
#include "graphics.h"

void fb_init(void);
Framebuffer *get_fb(void);
void fb_present(void);

#endif
