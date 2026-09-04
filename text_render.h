#ifndef TEXT_RENDER_H
#define TEXT_RENDER_H
#include <stdint.h>

void draw_char(int x, int y, char c, uint32_t color);
void draw_string(int x, int y, const char *s, uint32_t color);

#endif
