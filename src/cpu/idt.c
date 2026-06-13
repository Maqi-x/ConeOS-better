#include "idt.h"

static struct idt_entry idt[256];
static struct idt_ptr idtp;

void load_idt(void)
{
    idtp.base = (uint64_t)&idt;
    idtp.limit = sizeof(idt) - 1;

    __asm__ volatile ("lidt %0" : : "m"(idtp));
}
