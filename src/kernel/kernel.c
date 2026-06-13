#include "../../limine/limine.h"
#include "../cpu/idt.h"
#include "../drivers/framebuffer.h"
#include "../drivers/pic.h"
#include "../drivers/serial.h"
#include "../gfx/draw.h"
#include <stdint.h>

#define NULL ((void *)0)

LIMINE_BASE_REVISION(6);

void kmain(void) {
  print_serial("start kernel\n");
  pic_remap();
  pic_init_masks();
  load_idt();

  struct limine_framebuffer *fb = framebuffer_get();
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

  draw_string(fb, 10, 10, "ConeOS", 0x00FFFFFF, 2);
  draw_string(fb, 10, 42, "Hello", 0x00FF0000, 2);
  asm volatile("sti");
  for (;;)
    __asm__ volatile("hlt");
}
