#ifndef DRAW_H
#define DRAW_H

#include "../../limine/limine.h"
#include <stdint.h>

void draw_pixel(struct limine_framebuffer *fb, uint32_t x, uint32_t y,
                uint32_t color);
void draw_rect(struct limine_framebuffer *fb, uint32_t width, uint32_t height,
               uint32_t x, uint32_t y, uint32_t color);
void draw_char(struct limine_framebuffer *fb, int x, int y, char c,
               uint32_t color, int scale);
void draw_string(struct limine_framebuffer *fb, int x, int y, const char *str,
                 uint32_t color, int scale);
#endif
