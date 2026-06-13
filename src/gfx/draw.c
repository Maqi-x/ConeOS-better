#include <gfx/draw.h>
#include <gfx/font.h>
#include <stdint.h>

void draw_pixel(framebuffer* fb, uint32_t x, uint32_t y, uint32_t color) {
    uint32_t* base = fb->addr;
    uint32_t bpp = fb->bpp / 8;
    uint8_t* pixel = (uint8_t*)base + y * fb->pitch + x * bpp;
    *(uint32_t*)pixel = color;
}

void draw_rect(
    framebuffer* fb, uint32_t width, uint32_t height,
    uint32_t x, uint32_t y, uint32_t color
) {
    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            draw_pixel(fb, x + j, y + i, color);
        }
    }
}

void draw_char(
    framebuffer* fb, uint32_t x, uint32_t y,
    char c, uint32_t color, int scale
) {
    if (c < 0)
        return;

    if (scale < 1)
        scale = 1;

    for (int row = 0; row < 8; row++) {
        uint8_t line = font8x8_basic[(int)c][row];

        for (int col = 0; col < 8; col++) {
            if ((line & (1 << col))) {
                for (int sy = 0; sy < scale; sy++) {
                    for (int sx = 0; sx < scale; sx++) {
                        draw_pixel(fb, x + col * scale + sx, y + row * scale + sy, color);
                    }
                }
            }
        }
    }
}

void draw_string(
    framebuffer* fb, uint32_t x, uint32_t y,
    const char* str, uint32_t color, int scale
) {
    int cursor = 0;

    if (scale < 1)
        scale = 1;

    while (*str) {
        draw_char(fb, x + cursor * 8 * scale, y, *str, color, scale);
        cursor++;
        str++;
    }
}
