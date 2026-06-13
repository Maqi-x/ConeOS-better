#include <cpu/idt.h>
#include <cpu/irq.h>
#include <drivers/framebuffer.h>
#include <drivers/multiboot2.h>
#include <drivers/pic.h>
#include <drivers/serial.h>
#include <gfx/draw.h>

#include <stdint.h>

#define NULL ((void*)0x0)

void kmain(uint32_t magic, uint32_t mb_addr) {
    print_serial("start kernel\n");

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        print_serial("invalid magic\n");
        for (;;)
            __asm__ volatile("hlt");
    }

    fb_init(mb_addr);
    pic_remap();
    load_idt();
    irq_init();

    framebuffer* fb = fb_get();
    if (fb == NULL) {
        print_serial("no framebuffer\n");
        for (;;)
            __asm__ volatile("hlt");
    }

    print_serial("w=");
    print_uint(fb->width);

    print_serial(" h=");
    print_uint(fb->height);

    print_serial(" pitch=");
    print_uint(fb->pitch);

    print_serial("\n");

    draw_string(fb, 10, 10, "ConeOS-better", 0x00FFFFFF, 2);
    draw_string(fb, 10, 42, "Hello",         0x00FF0000, 2);

    asm volatile("sti");
    for (;;)
        __asm__ volatile("hlt");
}
