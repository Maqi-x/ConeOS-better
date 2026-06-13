#include <drivers/framebuffer.h>
#include <drivers/multiboot2.h>
#include <drivers/serial.h>

#define NULL ((void *)0)

static framebuffer fb;
static int fb_initialized = 0;

void fb_init(uint32_t mb_addr) {
    mboot_tag* tag;

    /* Multiboot2 info structure starts with total size and reserved field */
    for (tag = (mboot_tag*)(mb_addr + 8);
             tag->type != 0;
             tag = (mboot_tag*)((uint8_t *)tag + ((tag->size + 7) & ~7))) {

        if (tag->type == 8) { // Framebuffer tag
            mboot_tag_fb* fb_tag = (mboot_tag_fb*)tag;
            fb.addr = (uint32_t*)(uintptr_t)fb_tag->fb_addr;
            fb.width = fb_tag->fb_width;
            fb.height = fb_tag->fb_height;
            fb.pitch = fb_tag->fb_pitch;
            fb.bpp = fb_tag->fb_bpp;
            fb_initialized = 1;
            return;
        }
    }
}

framebuffer *fb_get(void) {
    if (!fb_initialized) {
        return NULL;
    }
    return &fb;
}
