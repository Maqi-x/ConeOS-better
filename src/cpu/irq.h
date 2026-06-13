#pragma once
#include <stdint.h>

typedef struct interrupt_frame {
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t sp;
    uint32_t ss;
} interrupt_frame;

typedef void (*irq_handler)(interrupt_frame*);

void irq_init(void);
void irq_install_handler(int irq, irq_handler handler);
void irq_uninstall_handler(int irq);
void irq_set_mask(uint8_t irq);
void irq_clear_mask(uint8_t irq);
