#include <cpu/idt.h>

static struct idt_entry idt[256];
static struct idt_ptr idtp;

void idt_set_gate(uint8_t vector, uint32_t handler, uint16_t selector, uint8_t type_attr) {
  struct idt_entry *e = &idt[vector];
  e->offset_low  = handler & 0xFFFF;
  e->offset_high = (handler >> 16) & 0xFFFF;
  e->selector    = selector;
  e->type_attr   = type_attr;
  e->zero        = 0;
}

void load_idt(void) {
  idtp.base = (uint32_t)&idt;
  idtp.limit = sizeof(idt) - 1;

  __asm__ volatile("lidt %0" : : "m"(idtp));
}
