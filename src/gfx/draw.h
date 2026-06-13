#pragma once

#include <drivers/framebuffer.h>
#include <stdint.h>

void draw_pixel(framebuffer* fb,  uint32_t x, uint32_t y, uint32_t color);
void draw_rect(framebuffer* fb,   uint32_t width, uint32_t height, uint32_t x, uint32_t y, uint32_t color);
void draw_char(framebuffer* fb,   uint32_t x, uint32_t y, char c, uint32_t color, int scale);
void draw_string(framebuffer* fb, uint32_t x, uint32_t y, const char* str, uint32_t color, int scale);
